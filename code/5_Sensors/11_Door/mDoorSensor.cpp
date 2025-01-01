#include "mDoorSensor.h"

#ifdef USE_MODULE_SENSORS_DOOR

// Should this be a controller? Since the underlying input is from switches sensor. This would also mean motion should be controller.
// But then how does motion + others become an "event"
// Event should be anything really, not just switch ie power level threshold crossed


int8_t mDoorSensor::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;
    case TASK_EVERY_SECOND:

      // if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
      // {
      //   ALOG_TST(PSTR("DoorLockPin=%d"), digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
      // }

    break;
    case TASK_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
      break;
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
      break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
  }

} // END function



void mDoorSensor::Pre_Init(void){
  
  settings.fEnableSensor = false;

  if(pCONT_pins->PinUsed(GPIO_DOOR_OPEN_ID)) {  // not set when 255
    // pin_open = pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID);
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID), INPUT_PULLUP);
    settings.fEnableSensor = true;
    settings.fSensorCount = 1;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
    //disable pir code
  }


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID), INPUT_PULLUP);
    settings.fEnableSensor = true;
    settings.fSensorCount = 1;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID));
    //disable pir code
  }

}

/**
 * @brief "LOW" is closed, HIGH is open
 * 
 * @return uint8_t 
 */
uint8_t mDoorSensor::IsDoorOpen(){
  return (digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID))==HIGH);
}
uint8_t mDoorSensor::IsLock_Locked(){
  return (digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID))==LOW);
}


void mDoorSensor::init(void){

  door_detect.state = IsDoorOpen();
  lock_detect.state = IsLock_Locked();

}



void mDoorSensor::EveryLoop(){

  /**
   * @brief Reed Switch Door Position
   **/
  if((IsDoorOpen()!=door_detect.state)&&mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce,100)){

    ALOG_TST(PSTR("IsDoorOpen()"));

    door_detect.state = IsDoorOpen();
    door_detect.tDetectTimeforDebounce = millis();
    if(door_detect.state){ 
      door_detect.isactive = true;
      door_detect.detected_time = tkr_time->GetTimeShortNow();
      pCONT_rules->NewEventRun( GetModuleUniqueID(), TASK_EVENT_MOTION_STARTED_ID, 0, door_detect.isactive);
    }else{ 
      door_detect.isactive = false;
      pCONT_rules->NewEventRun( GetModuleUniqueID(), TASK_EVENT_MOTION_ENDED_ID, 0, door_detect.isactive);
    }
    door_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

  }

  /**
   * @brief Door lock  
   **/
  if((IsLock_Locked()!=lock_detect.state)&&mTime::TimeReachedNonReset(&lock_detect.tDetectTimeforDebounce,100)){

    ALOG_TST(PSTR("IsLock_Locked()"));

    lock_detect.state = IsLock_Locked();
    lock_detect.tDetectTimeforDebounce = millis();
    if(lock_detect.state){ 
      lock_detect.isactive = true;
      lock_detect.detected_time = tkr_time->GetTimeShortNow();
      pCONT_rules->NewEventRun( GetModuleUniqueID(), TASK_EVENT_MOTION_STARTED_ID, 1, lock_detect.isactive);
    }else{ 
      lock_detect.isactive = false;
      pCONT_rules->NewEventRun( GetModuleUniqueID(), TASK_EVENT_MOTION_ENDED_ID, 1, lock_detect.isactive);
    }
    lock_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

  }



}


const char* mDoorSensor::IsDoorOpen_Ctr(char* buffer, uint8_t buflen){
  if(door_detect.isactive){
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENED_CTR, sizeof(PM_EVENT_DOOR_OPENED_CTR));
  }else{
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_CLOSED_CTR, sizeof(PM_EVENT_DOOR_CLOSED_CTR));
  }
  return buffer;
}


void mDoorSensor::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_BME "\"%s\""),JBI->GetBufferPtr());

}



  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mDoorSensor::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    //JBI->Add_P(PM_SENSORCOUNT, settings.);
  return JBI->End();

}


uint8_t mDoorSensor::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){
  
  char buffer[50];

  JBI->Start();
  JBI->Add(D_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),0,buffer,sizeof(buffer)));
  JBI->Add("Position", IsDoorOpen_Ctr(buffer, sizeof(buffer))); // give telemetry update of position
  
  if(json_level >= JSON_LEVEL_IFCHANGED){
    JBI->Add(D_TIME, mTime::ConvertShortTimetoCtr(&door_detect.detected_time, buffer, sizeof(buffer)));
    JBI->Add(D_EVENT, IsDoorOpen_Ctr(buffer, sizeof(buffer)));
  }

  JBI->Add("DoorOpenPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID)));

    
  JBI->Add("IsDoorOpen", IsDoorOpen());


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    JBI->Add("DoorLockPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
    JBI->Add("IsLock_Locked", IsLock_Locked());
  }

  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mDoorSensor::MQTTHandler_Init(){

  struct handler<mDoorSensor>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mDoorSensor::ConstructJSON_Sensor;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mDoorSensor::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mDoorSensor::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mDoorSensor::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_WEBSERVER
void mDoorSensor::WebAppend_Root_Status_Table_Draw(){

  char buffer[10];
    
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>Door Position</td>"));
    BufferWriterI->Append_P(PSTR("<td>{dc}%s'>%s</div></td>"),"tab_door", IsDoorOpen_Ctr(buffer, sizeof(buffer)));   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  
}


//append to internal buffer if any root messages table
void mDoorSensor::WebAppend_Root_Status_Table_Data(){
  
  uint8_t sensor_counter = 0;
  char value_ctr[8];
  char colour_ctr[10];
  char inner_html[100];
  char door_pos_ctr[20];
  char time_ctr[20];

  JBI->Array_Start("tab_door");// Class name
  
  for(int sensor_id=0;sensor_id<1;sensor_id++){
    
    JBI->Object_Start();
      JBI->Add("id",sensor_id);

      char colour_ctr[8];
      uint32_t millis_elapsed = mTime::MillisElapsed(&door_detect.tEndedTime);
      // Motion in progress
      if(door_detect.isactive){
        sprintf_P(colour_ctr,PSTR("#00ff00"));
      }else
      // If movement event has just finished
      if(millis_elapsed<(1000*60)){
        // Show colour as fading back to white over X seconds SINCE EVENT OVER
        uint8_t colour_G = constrain(
                              map(millis_elapsed,0,(1000*60),0,255)
                              ,0,255 //increases with time
                            );
        pCONT_web->WebColorCtr(255,colour_G,colour_G, colour_ctr, sizeof(colour_ctr));
      }
      // no event show, just white
      else{
        sprintf(colour_ctr,"#ffffff");
      }

      // sprintf(inner_html,"%s %s",IsDoorOpen_Ctr(door_pos_ctr,sizeof(door_pos_ctr)),
      //   mTime::ConvertShortTime_HHMMSS(&door_detect.detected_time, time_ctr, sizeof(time_ctr)));
    
      JBI->Add("ih",inner_html);
      JBI->Add("fc",colour_ctr);
    
    JBI->Object_End();
  }

  JBI->Array_End();

}

#endif // USE_MODULE_NETWORK_WEBSERVER

#endif


