/**

  Copyright (C) 2025    Michael Doone

  @note Relys on all other modules (no timers), and acts in place of rules

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "mLightNeo_MotionAlerts.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

/**
 * @brief: Module Interface
 * */
int8_t mLightNeo_MotionAlerts::Tasker(uint8_t function, JsonParserObject obj)
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
     * STORAGE SECTION * 
    *******************/  
    #ifdef USE_MODULE_CORE_FILESYSTEM
    #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
      Load_Module();
    break;
    #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
    #endif // USE_MODULE_CORE_FILESYSTEM
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    
    #endif// USE_MODULE_CORE_RULES
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
    #endif // USE_MODULE_NETWORK_MQTT
  } // end switch
} // END function


#ifdef USE_MODULE_CORE_FILESYSTEM
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS

void mLightNeo_MotionAlerts::Save_Module()
{
  ALOG_INF(PSTR(D_LOG_RELAYS "Save_Module"));
  pCONT_mfile->ByteFile_Save("/relays" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));
}

void mLightNeo_MotionAlerts::Load_Module(bool erase)
{
  ALOG_INF(PSTR(D_LOG_RELAYS "Load_Module"));
  pCONT_mfile->ByteFile_Load("/relays" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));
}

#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
#endif // USE_MODULE_CORE_FILESYSTEM


void mLightNeo_MotionAlerts::Pre_Init(void){
  
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;

 
  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}


void mLightNeo_MotionAlerts::Init(void)
{


}


void mLightNeo_MotionAlerts::EverySecond()
{


}


/**
 * @brief Will react to button presses, then change the state of this module accordinly
 * 
 */
void mLightNeo_MotionAlerts::Handle_ButtonEvents()
{

  ALOG_DBM(PSTR("mLightNeo_MotionAlerts::Handle_ButtonEvents"));


}

/**
 * @brief When mqtt topic is received in json commands, this function will parse the command
 * It will set up the lights to react to motion
 * 
 * Most likely, I will have different segments, but in all, the lights will turn on by a colour, with a slow fade off.
 * 
 */
void mLightNeo_MotionAlerts::Handle_MotionEvents()
{

  ALOG_DBM(PSTR("mLightNeo_MotionAlerts::Handle_MotionEvents"));


}




void mLightNeo_MotionAlerts::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR("mLightNeo_MotionAlerts::parse_JSONCommand"));

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 

}





uint8_t mLightNeo_MotionAlerts::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

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


uint8_t mLightNeo_MotionAlerts::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(json_appending)
  {
    JBI->Start();
  }


  for(int device_id=0;device_id<module_state.devices;device_id++){
    
    
  }
  
  if(json_appending)
  {
    return JBI->End();
  }
  return JBI->Length();

}



#ifdef USE_MODULE_NETWORK_MQTT

void mLightNeo_MotionAlerts::MQTTHandler_Init()
{

  struct handler<mLightNeo_MotionAlerts>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLightNeo_MotionAlerts::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mLightNeo_MotionAlerts::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mLightNeo_MotionAlerts::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT



#endif // USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS