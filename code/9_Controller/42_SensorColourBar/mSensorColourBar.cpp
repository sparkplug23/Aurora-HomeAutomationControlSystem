#include "mSensorColourBar.h"

#ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR

/**
 * @brief: Module Interface
 * */
int8_t mSensorColourBar::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_APPEND_RESPONSE_JSON_DRIVERS_STATUS_ID:
      AppendJSONResponse_Drivers_Unified(); // should this be an override? prob too complex to be useful
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_SET_POWER_ID: 
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a TASK_MQTT_INIT and group mqtt togather
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate(); // Load teleperiod setting into local handlers
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  } // end switch
} // END function

void mSensorColourBar::Pre_Init(void){
  
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

  module_state.devices++;

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}


void mSensorColourBar::Init(void){

  // module_state.devices = tkr_set->runtime.devices_present; //phase out

  // module_state.devices = MAX_RELAYS;


  // clear all settings to 0
  memset(&rt.relay_status, 0, sizeof(rt.relay_status));

  // Set defaults
  for(int relay_id=0;relay_id<MAX_RELAYS;relay_id++){
    rt.relay_status[relay_id].timer_decounter.seconds = 0;
    rt.relay_status[relay_id].timer_decounter.active = false;
  }
  
}


void mSensorColourBar::EverySecond(void)
{

  /*
    heatmap palette here? 
    I should maybe go from matlab, and create some common "colour maps" then simply create the mapper into segment (the max/min) etc

    colormap_parula
    colormap_turbo
    colormap_hsv
    colormap_hot
    colormap_cool
    colormap_spring
    colormap_autumn  as CRGB16PALETTE??, probably best for speed

    generate the RGB values using matlab 

    Do I need to make a "compression" method. ie instead of having linear mapping, I should make a 0 to 255 map
    ie be able to pass an array that is used for mapping
    colourmap_points[0 to 16 of them] = [10 degrees, 40 degrees] would be 0=10, 255=40
    colourmap_points[4 of them] = [10,17,18,20] would be 0=10, 64=17, 128=18, 255=20

    palettes above should be stored  in original, then I can "load" them into a CRGB16Palette16 and can adjust it to what I want (ie compress)


  */

  // std::vector<float> values;
  // values.push_back(10.1);
  // values.push_back(20.2);

  // Serial.println("mSensorColourBar::EverySecond");
  // for(uint8_t i=0;i<values.size();i++)
  // {
  //   Serial.println(values[i]);
  // }










}


void mSensorColourBar::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR("mSensorColourBar::parse_JSONCommand"));

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  int8_t relay_id= -1,state=-1;    //assume index 0 if none given

  // if(jtok = obj[PM_POWERNAME]){
  //   if(jtok.isStr()){
  //     relay_id = GetRelayIDbyName(jtok.getStr());
  //   ALOG_INF( PSTR("relay_id = %s"), jtok.getStr() );
  //   }else 
  //   if(jtok.isNum()){
  //     relay_id  = jtok.getInt();
  //   }

  //   ALOG_INF( PSTR("relay_id = %d"), relay_id );
  // }

  // // Primary method since v0.86.14.21
  // if(jtok = obj[PM_POWER_STATE]){
  //   if(jtok.isStr()){
  //     state = pCONT_sup->GetStateNumber(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
  //   }

  //   /**
  //    * @brief If off, clear any timer decounters for relays
  //    * 
  //    */
  //   if(state == 0)
  //   {
  //     CommandSet_Timer_Decounter(0, relay_id);
  //   }

	// 	//state needs checked for flipped
	// 	// if(state == 2){

	// 	// }

  // }


}






uint8_t mSensorColourBar::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  JBI->Start();
    // JBI->Add(PM_DEVICES_CONNECTED, module_state.devices);

    // JBI->Array_Start_P(PSTR("rel_inverted"));
    //   for(int8_t bits=0; bits<sizeof(rt.rel_inverted)*8; bits++)
    //   {
    //     JBI->Add(bitRead(rt.rel_inverted,bits));
    //   }
    // JBI->Array_End();



  JBI->End();

}


uint8_t mSensorColourBar::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(json_appending)
  {
    JBI->Start();
  }


  
  if(json_appending)
  {
    return JBI->End();
  }
  return JBI->Length();

}

/**
 * @brief I should consider passing another argument into normal ConstructJson to disabled adding Start/Stop so this function could simply call another without them?
 * 
 * @return uint_t 
 */
uint8_t mSensorColourBar::AppendJSONResponse_Drivers_Unified()
{
  JBI->Level_Start_P(PM_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__CTR);
    ConstructJSON_State(JSON_LEVEL_SHORT, false);
  JBI->Object_End();
}



#ifdef USE_MODULE_NETWORK_MQTT

void mSensorColourBar::MQTTHandler_Init()
{

  struct handler<mSensorColourBar>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorColourBar::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mSensorColourBar::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mSensorColourBar::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorColourBar::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorColourBar::MQTTHandler_Rate()
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
void mSensorColourBar::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif

