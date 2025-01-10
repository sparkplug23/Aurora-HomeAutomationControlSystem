#include "mFan.h"

// FanPWM name change

#ifdef USE_MODULE_CONTROLLER_FAN

uint8_t mFan::GetFanspeed(void)
{
  // if (ifan_fanspeed_timer) {
  //   return ifan_fanspeed_goal;                     // Do not show sequence fanspeed
  // } else {
  //   /* Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
  //     000x = 0
  //     001x = 1
  //     011x = 2
  //     101x = 3 (ifan02) or 100x = 3 (ifan03)
  //   */
  //   uint8_t fanspeed = (uint8_t)( tkr_set->power &0xF) >> 1;
  //   if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
  //   return fanspeed;
  // }

  // Map and round it?
  return set_fan_speed;

}

// Probably to be handled using "light_interface?"
uint8_t mFan::GetLightState(void)
{
  // return pCONT_mry->CommandGet_Relay_Power(0);
  return 0;
}
void mFan::SetLightState(uint8_t state)
{
  // pCONT_mry->CommandSet_Relay_Power(state);
}

/*********************************************************************************************/

void mFan::SetFanspeed(uint8_t fanspeed, bool sequence)
{
  ifan_fanspeed_timer = 0;                         // Stop any sequence
  ifan_fanspeed_goal = fanspeed;

  uint8_t fanspeed_now = GetFanspeed();

  if (fanspeed == fanspeed_now) { return; }

  uint8_t fans;// = kIFan02Speed[fanspeed];
  // if (SONOFF_IFAN03 == my_module_type) {
  //   if (sequence) {
  //     fanspeed = kIFan03Sequence[fanspeed_now][ifan_fanspeed_goal];
  //     if (fanspeed != ifan_fanspeed_goal) {
  //       if (0 == fanspeed_now) {
  //         ifan_fanspeed_timer = 20;                // Need extra time to power up fan
  //       } else {
  //         ifan_fanspeed_timer = 2;
  //       }
  //     }
  //   }
  //   fans = kIFan03Speed[fanspeed];
  // // }
  // for (uint32_t i = 2; i < 5; i++) {
  //   uint8_t state = (fans &1) + POWER_OFF_NO_STATE;  // Add no publishPowerState
  //   pCONT_mry->ExecuteCommandPower(i, state, SRC_IGNORE);     // Use relay 2, 3 and 4
  //   fans >>= 1;
  // }

}


void mFan::init(void)
{

  analogWrite(pin, pwm_range_min);

}


void mFan::Pre_Init(){
  
  // #ifndef ENABLE_DEVFEATURE_FANTYPE_IFAN03
  if(pCONT_pins->PinUsed(GPIO_FAN_PWM1_ID)) {  // not set when 255
    pin = pCONT_pins->GetPin(GPIO_FAN_PWM1_ID);
    pinMode(pin, OUTPUT);
    Serial.println("PIMODE HERE ");
    Serial.println(pin);
    settings.fEnableModule = true;
  }
  // #endif

}

int8_t mFan::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:    

    break;
    case TASK_EVERY_250_MSECOND:
      // SpeedRefresh();
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

}


uint8_t mFan::ConstructJSON_Settings(uint8_t json_level, bool json_appending){
  
  JBI->Start();
    JBI->Add_P("test",0);  
  JBI->End();

}

uint8_t mFan::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add_P(D_FANSPEED, GetFanspeed());  
    JBI->Add_P(D_FANPWM, set_fan_pwm);  
  JBI->End();

}


void mFan::MQTTHandler_Init(){

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mFan::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  ptr->ConstructJSON_function = &mFan::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  ptr->ConstructJSON_function = &mFan::ConstructJSON_Sensor;
  
}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mFan::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mFan::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mFan::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif
