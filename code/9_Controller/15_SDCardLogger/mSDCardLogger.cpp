#include "mSDCardLogger.h"

#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER

int8_t mSDCardLogger::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
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
      EverySecond();
    break;  
    //Rule based on button toggle for sd open and close 
    //have buitin led on when sd card has shown write activity in last 100ms
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_BUTTON_PRESSED:
    //   RulesEvent_Button_Pressed();
    // break;
    #endif// USE_MODULE_CORE_RULES

    /************
     * TRIGGERS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      CommandSet_SDCard_OpenClose_Toggle();
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
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mSDCardLogger::parse_JSONCommand(JsonParserObject obj)
{

}


void mSDCardLogger::Pre_Init(void)
{
  // if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  // {
    settings.fEnableSensor = true;
  // }

}


void mSDCardLogger::Init(void)
{


  //For ease, lets hard code the button rule

  
    #if defined(USE_MODULE_SENSORS_BUTTONS)
    
    // // Trigger0
    // p_event = &tkr_rules->rules[tkr_rules->rules_active_index].trigger;   
    // p_event->module_id = EM_MODULE_SENSORS_BUTTONS_ID;
    // p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // // Command0
    // p_event = &tkr_rules->rules[tkr_rules->rules_active_index].command;   
    // p_event->module_id = EM_MODULE_CONTROLLER_SONOFF_IFAN_ID;
    // p_event->function_id = TASK_EVENT_SET_SPEED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = STATE_NUMBER_INCREMENT_ID;  // Increment 
    // tkr_rules->rules_active_index++;
    // settings.loaded_default_for_moduled = true;

    #endif


}


void mSDCardLogger::EveryLoop()
{

      char timectr[30];
      snprintf(timectr, sizeof(timectr), "%s\n", tkr_time->RtcTime.hhmmss_ctr);

      appendFile_open_and_close(SD, "/time_test.txt", timectr);

      readFile(SD, "/time_test.txt");


}

//Use gps time to set rtc time?

void mSDCardLogger::EverySecond()
{

  SubTask_UpdateOLED();

}

void mSDCardLogger::SubTask_UpdateOLED()
{

  //Test display messages
  //[1234567890123456]
  //[Op f123456] // Op/Cd for open and closed file, f# where # is GPS time for file name (although also using random in case gps is not working ie millis+gpsUTC)
  //[Val 123456] // Val Err for GPS fix, showing UTC time
  //[ ] //Also show lat/long so I know its working
  //[] packets received on serialRSS in thousands
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%s %s","Op","fMillis123456");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 0);

  snprintf(buffer, sizeof(buffer), "%s",sdcard_status.isopened?"Open":"CLOSED");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);


  
  snprintf(buffer, sizeof(buffer), "%s %s","Op",tkr_time->RtcTime.hhmmss_ctr);//tkr_time->GEt DT_UTC;
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mSDCardLogger::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mSDCardLogger::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, 0);
  return JBI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mSDCardLogger::MQTTHandler_Init(){

  struct handler<mSDCardLogger>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSDCardLogger::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSDCardLogger::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSDCardLogger::ConstructJSON_Sensor;
  
} 


void mSDCardLogger::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} 


void mSDCardLogger::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mSDCardLogger::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_CONTROLLER_SDCARDLOGGER_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
