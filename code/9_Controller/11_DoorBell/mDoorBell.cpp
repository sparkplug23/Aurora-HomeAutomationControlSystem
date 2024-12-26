#include "mDoorBell.h"

#ifdef USE_MODULE_CONTROLLER_DOORCHIME


/**
 * phase out, this will be a button instead, linked via rules
 * so "Init()" will append the neccesary rule
 * mqtt into own file.cpp
 * 
 * relay for chime could also be changed, to be added into relay, and add as control options for relays ie some sort of interlock/sequence, or, does 
 * this become some sort of buzzer thats simple logic?
 * 
 * event will also fire motion style event via rules, so app can send notificaiton on phone.
 * 
 * Lets also consider adding a "knock sensor" to the door
 * 
 * */

void mDoorBell::Pre_Init(void)
{

//These should configure the relay and switch input indirectly, never used pinmode studd here



//should be rewritten to only pull from buttons/relays which are configured internally
  if(pCONT_pins->PinUsed(GPIO_CHIME_INPUT_ID)) {  // not set when 255
    pin_doorbell_button = pCONT_pins->GetPin(GPIO_CHIME_INPUT_ID);
    pinMode(pin_doorbell_button,INPUT_PULLUP);
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin_doorbell_button);
    //disable pir code
  }

  if(pCONT_pins->PinUsed(GPIO_CHIME_RINGER_ID)) {  // not set when 255
    pin_relay_chime = pCONT_pins->GetPin(GPIO_CHIME_RINGER_ID);
    pinMode(pin_relay_chime,OUTPUT);
    digitalWrite(pin_relay_chime,HIGH); //active low
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin_relay_chime);
    //disable pir code
  }

}



void mDoorBell::init(void){

  // sprintf(ringer.friendly_name_ctr,"Hallway Wall");
  // sprintf(doorbell_switch.friendly_name_ctr,"Front");

  // sprintf(ringer.trigger_time_ctr,"%s","--:--:--");
  // sprintf(doorbell_switch.trigger_time_ctr,"%s","--:--:--");

}




// Set parameters to ring it
void mDoorBell::RingDoorBellSet(uint8_t seconds, uint16_t freq){
  ringer.fIsRinging = true;
  ringer.start_millis = millis();
  ringer.end_millis = millis()+(seconds*1000);
  ringer.freq = freq;
  ringer.seconds = seconds;
  AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CHIME "Set"));
  // memcpy(ringer.trigger_time_ctr,pCONT_time->RtcTime.hhmmss_ctr,sizeof(pCONT_time->RtcTime.hhmmss_ctr)); 
}

// Function called each time
// Return if ring is not finished
uint8_t mDoorBell::RingDoorBellLoop(uint8_t reset){

  if(ringer.fIsRinging){ //not finished

    //calculate toggle as seconds/freq
    if(mTime::MillisReached(&ringer.toggle_millis)){
      BellChime_Toggle();
      if(BellSwitch_OnOff()){
        ringer.closed_millis_end = millis()+500;
      }
      ringer.toggle_millis = millis()+(ringer.freq);//*500); //set new toggle millis
      AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CHIME "Toggled"));
    }

    //timeout/finished reset
    if(mTime::MillisReached(&ringer.end_millis)){
      BellChime_Set(BELLCHIME_OFF); // turn off
      ringer.fIsRinging = false;
      AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CHIME "Ended ringer.end_millis"));
    }

    //Only have relay on for short time
    if(mTime::TimeReached(&ringer.closed_millis_end,500)){
      //BELLCHIME_SET(BELLCHIME_OFF); // turn off
      AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CHIME "Ended ringer.closed_millis_end"));
    }

  }

}


void mDoorBell::BellChime_Set(uint8_t onoff){
  digitalWrite(pin_relay_chime,!onoff);
}

void mDoorBell::BellChime_Toggle(){
  digitalWrite(pin_relay_chime,!digitalRead(pin_relay_chime));
}



uint8_t mDoorBell::BellSwitch_OnOff(){
  return !digitalRead(pin_doorbell_button);
}


const char* mDoorBell::BellSwitch_OnOff_Ctr(){
  return BellSwitch_OnOff() ? "ON" : "OFF";
}




int8_t mDoorBell::Tasker(uint8_t function, JsonParserObject obj){
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      init();
    break;
    case TASK_LOOP: 
      EveryLoop();
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER


} // END function


void mDoorBell::EveryLoop(){

  ALOG_DBM( PSTR("Tasker::mDoorBell"));

  if((BellSwitch_OnOff()!=doorbell_switch.event.state)
      &&mTime::TimeReachedNonReset(&tDetectTimeforDebounce,100)
  ){

    doorbell_switch.event.state = BellSwitch_OnOff(); //tDetectTime = millis();
    
    if(doorbell_switch.event.state){ Serial.print("Active high doorbell_switch");
      doorbell_switch.event.isactive = true;
      tDetectTimeforDebounce = millis();   

      // memcpy(doorbell_switch.trigger_time_ctr,pCONT_time->RtcTime.hhmmss_ctr,sizeof(pCONT_time->RtcTime.hhmmss_ctr)); 

      if(settings.fEnable_Switch_Relay_Binding){
        RingDoorBellSet(2, 500); //1 per sec for 2 secs (2 rings)
      }

    }else{ Serial.print("Active low doorbell_switch");
      doorbell_switch.event.isactive = false;
    }
    doorbell_switch.event.ischanged = true;
    // doorbell_switch.changedtime = pCONT_time->RtcTime;
    doorbell_switch.event.detected_time = pCONT_time->GetTimeShortNowU32();
    // fUpdateSendDoorSensor = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    
  }

  // Handle ringer
  RingDoorBellLoop();

}



uint8_t mDoorBell::ConstructJSON_Settings(uint8_t json_level, bool json_appending){
  
  JBI->Start();
    JBI->Add_P("test",0);  
  JBI->End();

}

uint8_t mDoorBell::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  if(doorbell_switch.event.ischanged){doorbell_switch.event.ischanged=false;

    JBI->Start();
      char buffer[80];
      JBI->Add(D_LOCATION,  DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), 0, buffer, sizeof(buffer)));
      JBI->Add(D_TIME, mTime::ConvertU32TimetoCtr(&doorbell_switch.event.detected_time, buffer, sizeof(buffer)));
      JBI->Add(D_EVENT, doorbell_switch.event.isactive ? "detected": "over");
    JBI->End();

  }

}




/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mDoorBell::MQTTHandler_Init(){

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDoorBell::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_CTR;
  ptr->ConstructJSON_function = &mDoorBell::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_CTR;
  ptr->ConstructJSON_function = &mDoorBell::ConstructJSON_Sensor;
  
} 


void mDoorBell::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} 


void mDoorBell::MQTTHandler_Rate(){

  // mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mDoorBell::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mDoorBell>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_CONTROLLER_DOORBELL_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

////////////////////// END OF MQTT /////////////////////////



#endif
