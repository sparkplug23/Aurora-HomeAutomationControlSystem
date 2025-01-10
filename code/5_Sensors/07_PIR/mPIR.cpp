/**
 * @file mPIR.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-01-01
 * 
 * @description The PIR module, works directly with the PIR sensors, and reports motion events.
 * This module will report the event, but will also immediately send the event to the rule engine to be processed, which
 * will then trigger any rules, but also make sure that the "interface" is called to report the new unified motion event structure.
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "mPIR.h" 

#ifdef USE_MODULE_SENSORS_PIR 

int8_t mPIR::Tasker(uint8_t function, JsonParserObject obj){
  
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
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    case TASK_EVERY_50_MSECOND:
      ReadSensor();
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
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mPIR::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;
}


void mPIR::Init(void)
{

  module_state.devices = 0;

  for(uint8_t ii=0; 
              ii<MAX_SENSOR_PIR_DEVICES; 
              ii++
  )
  {
    if(pCONT_pins->PinUsed(GPIO_PIR_1_ID, ii))
    {
      pinMode(pCONT_pins->GetPin(GPIO_PIR_1_ID, ii), INPUT);
      pir_detect.push_back(event_motion_t());
      pir_detect[module_state.devices].isActiveLow = false;
      module_state.devices++;
    }
    if(pCONT_pins->PinUsed(GPIO_PIR_1_INV_ID, ii))
    {
      pinMode(pCONT_pins->GetPin(GPIO_PIR_1_INV_ID, ii), INPUT_PULLUP);
      pir_detect.push_back(event_motion_t());
      pir_detect[module_state.devices].isActiveLow = true;
      module_state.devices++;
    }
  }

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}

void mPIR::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100] = {0};
  if(module_state.devices)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", module_state.devices);  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<module_state.devices; sensor_id++)
    {      
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%s%d:\"%s\", ", "PIR", sensor_id, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)));    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_PIR "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


// Function to check PIR sensor states and update the event_motion_t vector
void mPIR::ReadSensor() 
{
  for (uint8_t sensor_id = 0; sensor_id < module_state.devices; sensor_id++) 
  {
    int8_t pin = pCONT_pins->GetPin(GPIO_PIR_1_ID, sensor_id);
    if (pin == -1) {
      pin = pCONT_pins->GetPin(GPIO_PIR_1_INV_ID, sensor_id);  // Check the inverted pin
    }

    if (pin != -1) {
      // Read the current state of the pin
      bool currentState = digitalRead(pin);

      // Adjust the `isActive` state based on `isActiveLow`
      bool isActive;
      if (pir_detect[sensor_id].isActiveLow) {
        // Active state is LOW, so active when currentState is LOW (0)
        isActive = (currentState == LOW);
      } else {
        // Active state is HIGH, so active when currentState is HIGH (1)
        isActive = (currentState == HIGH);
      }

      // Check if state has changed
      if (pir_detect[sensor_id].state != isActive) 
      {
        // Update state and set the state change flag
        pir_detect[sensor_id].state = isActive;
        pir_detect[sensor_id].ischanged = true;

        if (isActive) 
        {
          // Sensor is active
          pir_detect[sensor_id].tDetectTime = millis();
          pir_detect[sensor_id].detected_time = tkr_time->LocalTime();
          pir_detect[sensor_id].isactive = true;

          // Log or trigger events for active state
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "%d (Active)"), sensor_id);
          pCONT_rules->New_Event(GetModuleUniqueID(), sensor_id, isActive);
          pCONT->Tasker_Interface(TASK_EVENT_MOTION_STARTED_ID);               // This tied this submodule, directly into the interface, and will send the response immediately (with the rule populated)
        } 
        else 
        {
          // Sensor is inactive
          pir_detect[sensor_id].tEndedTime = millis();
          pir_detect[sensor_id].detected_time = tkr_time->LocalTime();
          pir_detect[sensor_id].isactive = false;

          // Log or trigger events for inactive state
          AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_PIR "%d (Inactive)"), sensor_id);
          pCONT_rules->New_Event(GetModuleUniqueID(), sensor_id, isActive);
          pCONT->Tasker_Interface(TASK_EVENT_MOTION_ENDED_ID);                // This tied this submodule, directly into the interface, and will send the response immediately (with the rule populated)
        }

        // Update device name and event information
        pir_detect[sensor_id].device_name.unique_module_id = GetModuleUniqueID();
        pir_detect[sensor_id].device_name.device_id        = sensor_id;

        // Set MQTT flag to send updated information
        mqtthandler_sensor_ifchanged.flags.SendNow = true;
      }else{
        pir_detect[sensor_id].ischanged = false;
      }
    }
  }
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mPIR::parse_JSONCommand(JsonParserObject obj)
{

}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mPIR::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_DEVICES, module_state.devices);
  return JBI->End();

}

/**
 * @brief Motion should be reported inside the module, but by using the rule triggering for MOTION_STARTED other modules may react to it
 * The interface should react to it by also sending out the motion occured, though will add later. 
 * "Motion" here should be renamed to PIR, or mPIR_PassiveInfrared 
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
uint8_t mPIR::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[80];

  JBI->Start();

    for(
      uint8_t sensor_id = 0;
      sensor_id<module_state.devices;
      sensor_id++
    ){

      if(pir_detect[sensor_id].ischanged)
      {
        
        pir_detect[sensor_id].ischanged = false;

        JBI->Add(D_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer))); 
        JBI->Add(PM_TIME, tkr_time->GetTimeStr(tkr_time->Rtc.local_time).c_str());
        JBI->Add(PM_UTC_TIME, tkr_time->Rtc.local_time);
        JBI->Add(D_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");

      }
    }

  return JBI->End();
    
}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mPIR::MQTTHandler_Init(){

  struct handler<mPIR>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mPIR::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPIR::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT


#endif
