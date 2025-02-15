// /**
//  * @file mSonoffIFan.cpp
//  * @author Michael Doone (michaeldoonehub@gmail.com)
//  * @brief 
//  * 
//  * iFan03 Hardware
//  * 4 Relays (1 Light + 3 Fan)
//  * 433 mhz controller via serial sMCU
//  * 
//  * @version 0.1
//  * @date 2023-06-19
//  * 
//  * @copyright Copyright (c) 2023
//  * 
//  */
// #include "mSonoffIFan.h"


//   #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
//   user_input.selected.palette_id = tkr_set->Settings.animation_settings.xmas_controller_params[0];
//   user_input.selected.brightness_id =  tkr_set->Settings.animation_settings.xmas_controller_params[1];
//   user_input.selected.effects_id =  tkr_set->Settings.animation_settings.xmas_controller_params[2];
//   user_input.selected.intensity_id = tkr_set->Settings.animation_settings.xmas_controller_params[3];
//   PhysicalController_Convert_IDs_to_Internal_Parameters();
//   #endif


// #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN


// int8_t mSonoffIFan::Tasker(uint8_t function, JsonParserObject obj)
// {
  
//   /************
//    * INIT SECTION * 
//   *******************/
//   if(function == TASK_INIT){
//     Init();
//   }

//   if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case TASK_EVERY_250_MSECOND:
//       SpeedRefresh();
//     break;
//     case TASK_EVERY_SECOND:
//       EverySecond();
//     break;
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     case TASK_JSON_COMMAND_ID:
//       parse_JSONCommand(obj);
//     break;
//     case TASK_SERIAL:
//       SerialInput();
//     break;
//     /************
//      * RULES SECTION * 
//     *******************/
//     #ifdef USE_MODULE_CORE_RULES
//     case TASK_EVENT_SET_SPEED_ID:
//       RulesEvent_Set_Speed();
//     break;
//     #endif
//     /************
//      * MQTT SECTION * 
//     *******************/
//     case TASK_MQTT_HANDLERS_INIT:
//       MQTTHandler_Init();
//     break;
//     case TASK_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     case TASK_MQTT_CONNECTED:
//       MQTTHandler_RefreshAll();
//     break;
//   }
  
//   /************
//    * WEBPAGE SECTION * 
//   *******************/  
//   #ifdef USE_MODULE_NETWORK_WEBSERVER
//   return Tasker_Web(function);
//   #endif

// } // END Tasker


    
// #ifdef USE_MODULE_CORE_RULES

// void mSonoffIFan::RulesEvent_Set_Speed()
// {

//   ALOG_TST(PSTR("MATCHED RulesEvent_Set_Speed"));

//   uint8_t index = tkr_rules->rules[tkr_rules->rules_active_index].command.device_id;

//   uint8_t state = tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[0];
//   uint8_t current_speed = 0;

//   if(state == STATE_NUMBER_INCREMENT_ID)
//   {
//     current_speed = GetFanspeed();
//     current_speed++;
//     if(current_speed>3){
//       current_speed = 0;
//     }
//     #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
//       pCONT_buzzer->BuzzerBeep(current_speed);
//     #endif
//     ALOG_TST(PSTR("MATCHED Increment %d"),current_speed);
//   }
//   else
//   {
//     current_speed = state; // should not run
//   }

//   SetFanSpeed(current_speed, false);

// }
// #endif // USE_MODULE_CORE_RULES


// /*********************************************************************************************/


// void mSonoffIFan::SetFanSpeed(uint8_t fanspeed, bool sequence)
// {
//   ifan_fanspeed_timer = 0;                         // Stop any sequence
//   ifan_fanspeed_goal = fanspeed;

//   uint8_t fanspeed_now = GetFanspeed();

//   if (fanspeed == fanspeed_now) { return; }

//   uint8_t fans = 0;
  
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
  
//   for (uint32_t i = 1; i < 4; i++) 
//   {
//     uint8_t state = (fans &1) + POWER_OFF_NO_STATE;         // Add no publishPowerState
//     pCONT_mry->ExecuteCommandPower(i, state, SRC_IGNORE);   // Use relay 2, 3 and 4 (index 1,2,3 skipping 0)
//     fans >>= 1;
//   }

// }


// uint8_t mSonoffIFan::GetFanspeed(void)
// {
//   if (ifan_fanspeed_timer) 
//   {
//     return ifan_fanspeed_goal;                     // Do not show sequence fanspeed
//   } 
//   else 
//   {
//     /* 
//       Fanspeed is controlled by relay 2, 3 and 4 as in Sonoff 4CH.
//       000x = 0
//       001x = 1
//       011x = 2
//       100x = 3 (ifan03)
//     */
//     uint8_t fanspeed = (uint8_t)( tkr_set->power &0xF) >> 1;
//     if (fanspeed) { fanspeed = (fanspeed >> 1) +1; }  // 0, 1, 2, 3
//     return fanspeed;
//   }
// }


// /*********************************************************************************************/


// void mSonoffIFan::SetLightState(uint8_t state)
// {
//   pCONT_mry->CommandSet_Relay_Power(state);
// }


// uint8_t mSonoffIFan::GetLightState(void)
// {
//   return pCONT_mry->CommandGet_Relay_Power(0);
// }


// /*********************************************************************************************/

// void mSonoffIFan::SonoffIfanReceived(void)
// {
  
//   #ifdef ENABLE_DEVFEATURE_IFAN__RF433_REMOTE_OVER_SERIAL
//   char svalue[32];

//   uint8_t mode = serial_in_buffer[3];
//   uint8_t action = serial_in_buffer[6];

//   if (4 == mode) {
//     if (action < 4) {
//       // AA 55 01 04 00 01 00 06 - Fan 0
//       // AA 55 01 04 00 01 01 07 - Fan 1
//       // AA 55 01 04 00 01 02 08 - Fan 2
//       // AA 55 01 04 00 01 03 09 - Fan 3
//       if (action != GetFanspeed()) {
//         snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_FANSPEED " %d"), action);
//         ExecuteCommand(svalue, SRC_REMOTE);
        
//         #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
//         BuzzerEnabledBeep((action) ? action : 1, (action) ? 1 : 4);  // Beep action times
//         #endif

//       }
//     } else {
//       // AA 55 01 04 00 01 04 0A - Light
//       ExecuteCommandPower(1, POWER_TOGGLE, SRC_REMOTE);
//     }
//   }
//   if (6 == mode) {
//     // AA 55 01 06 00 01 01 09 - Buzzer
//     Settings.flag3.buzzer_enable = !Settings.flag3.buzzer_enable;  // SetOption67 - Enable buzzer when available
//   }
//   if (7 == mode) {
//     // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
//     #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
//     BuzzerEnabledBeep(4, 1);                       // Beep four times
//     #endif
//   }

//   // Send Acknowledge - Copy first 5 bytes, reset byte 6 and store crc in byte 7
//   // AA 55 01 04 00 00 05
//   serial_in_buffer[5] = 0;                      // Ack
//   serial_in_buffer[6] = 0;                      // Crc
//   for (uint32_t i = 0; i < 7; i++) {
//     if ((i > 1) && (i < 6)) { serial_in_buffer[6] += serial_in_buffer[i]; }
//     Serial.write(serial_in_buffer[i]);
//   }
  
//   #endif // ENABLE_DEVFEATURE_IFAN__RF433_REMOTE_OVER_SERIAL
// }

// bool mSonoffIFan::SerialInput(void)
// {

//   #ifdef ENABLE_DEVFEATURE_IFAN__RF433_REMOTE_OVER_SERIAL

//   AddLog(LOG_LEVEL_DEV_TEST,PSTR("mSonoffIFan::SerialInput"));

//   if (0xAA == serial_in_byte) {               // 0xAA - Start of text
//     serial_in_byte_counter = 0;
//     ifan_receive_flag = true;
//   }
//   if (ifan_receive_flag) {
//     serial_in_buffer[serial_in_byte_counter++] = serial_in_byte;
//     if (serial_in_byte_counter == 8) {
//       // AA 55 01 01 00 01 01 04 - Wifi long press - start wifi setup
//       // AA 55 01 01 00 01 02 05 - Rf and Wifi short press
//       // AA 55 01 04 00 01 00 06 - Fan 0
//       // AA 55 01 04 00 01 01 07 - Fan 1
//       // AA 55 01 04 00 01 02 08 - Fan 2
//       // AA 55 01 04 00 01 03 09 - Fan 3
//       // AA 55 01 04 00 01 04 0A - Light
//       // AA 55 01 06 00 01 01 09 - Buzzer
//       // AA 55 01 07 00 01 01 0A - Rf long press - forget RF codes
//       AddLogSerial(LOG_LEVEL_DEBUG);
//       uint8_t crc = 0;
//       for (uint32_t i = 2; i < 7; i++) {
//         crc += serial_in_buffer[i];
//       }
//       if (crc == serial_in_buffer[7]) {
//         SonoffIfanReceived();
//         ifan_receive_flag = false;
//         return true;
//       }
//     }
//     serial_in_byte = 0;
//   }
//   return false;

//   #else
//   return false;
//   #endif // ENABLE_DEVFEATURE_IFAN__RF433_REMOTE_OVER_SERIAL

// }


// void mSonoffIFan::Init(void)
// {
//   #ifdef ENABLE_DEVFEATURE_IFAN__RF433_REMOTE_OVER_SERIAL
//     SetSerial(9600, TS_SERIAL_8N1);
//   #endif
//   settings.fEnableModule = true;
// }


// void mSonoffIFan::SpeedRefresh(void)
// {

//   if (ifan_fanspeed_timer) {
//     ifan_fanspeed_timer--;
//     if (!ifan_fanspeed_timer) {
//       SetFanSpeed(ifan_fanspeed_goal, false);
//     }
//   }

// }


// void mSonoffIFan::EverySecond()
// {
  
//   #if FIRMWARE_VERSION_MINOR > 114
//   #warning "DEPRECIATION_WARNING"
//   #endif
//   #if FIRMWARE_VERSION_MINOR > 115
//   #error "DEPRECIATION_ERROR"
//   #endif

//   if(time_on->UpdateTick())
//   {
//     ALOG_INF( PSTR(D_LOG_GARAGE D_COMMAND_NVALUE_K("Running Value")), time_on->Value());
//     mqtthandler_power_ifchanged.tRateSecs = 1;
//   }
//   else
//   {
//     mqtthandler_power_ifchanged.tRateSecs = 60;
//   }

//   if(time_on->IsLastTick())
//   {
//     SetFanSpeed(0, false);
//   }

// }



// /******************************************************************************************************************
//  *SECTION: Commands
// *******************************************************************************************************************/


// void mSonoffIFan::parse_JSONCommand(JsonParserObject obj){

//   char buffer[50];

//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;
  
//   /***
//    * As order of importance, others that rely on previous commands must come after
//    * */
//   if(jtok = obj[D_LIGHTPOWER]){
//     int light = jtok.getInt();
//     if(light == 2){
//       SetLightState(!GetLightState());
//     }else{
//       SetLightState(light);      
//     }

//     ALOG_COM( PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_COMMAND_SVALUE), D_LIGHTPOWER, GetLightState()?"On":"Off");

//     Response_mP(S_JSON_COMMAND_SVALUE_NVALUE, D_LIGHTPOWER, D_TOGGLE);

//   }


//   if(jtok = obj[D_FANSPEED]){
//     int speed = jtok.getInt();
//     if(speed>3){
//       // Response_mP(S_JSON_COMMAND_SVALUE, D_FANSPEED,D_PARSING_NOMATCH);
//       speed=0; //default off
//     }      

//     if(speed == 0) // clear timeon
//     {
//       time_on->Stop();
//     }

//     SetFanSpeed(speed, false);
//     AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
//     ALOG_INF(PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_COMMAND_NVALUE),D_FANSPEED,speed);
//     // Response_mP(S_JSON_COMMAND_NVALUE,D_FANSPEED,speed);
//   }
  
//   if(jtok = obj[D_FANTIMER]){
//     int time_on_seconds = jtok.getInt();

//     time_on->Start(time_on_seconds*60); // incoming command is minutes, internally held as seconds

//     /**
//      * @brief If a timer is set, when fan is already off, then assume maximum speed
//      * For now, disabled, too complicated for others to know when to set which
//      * 
//      */

//     ALOG_COM( PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_COMMAND_NVALUE), D_FANTIMER, time_on->Value());
    
//   }
  
//   mqtthandler_power_teleperiod.flags.SendNow = true;
//   mqtthandler_power_ifchanged.flags.SendNow = true;

// }


// /******************************************************************************************************************
//  *SECTION: ConstructJson
// *******************************************************************************************************************/


// uint8_t mSonoffIFan::ConstructJSON_Settings(uint8_t json_level, bool json_appending){
  
//   JBI->Start();
//     JBI->Add_P("test",0);  
//   JBI->End();

// }

// uint8_t mSonoffIFan::ConstructJSON_Power(uint8_t json_level, bool json_appending){

//   JBI->Start();
//     JBI->Add_P(D_LIGHTPOWER, GetLightState());
//     JBI->Add_P(D_FANSPEED, GetFanspeed());  
//     JBI->Add_P(D_FANTIMER "Seconds", time_on->ValueWithDisabledAsZero());  
//     JBI->Add_P(D_FANTIMER "Minutes", (int)(time_on->ValueWithDisabledAsZero()/60));  
//   JBI->End();

// }


// /******************************************************************************************************************
//  * SECTION: MQTT
// *******************************************************************************************************************/


// #ifdef USE_MODULE_NETWORK_MQTT

// void mSonoffIFan::MQTTHandler_Init(){

//   struct handler<mSonoffIFan>* ptr;

//   ptr = &mqtthandler_settings;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 600; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mSonoffIFan::ConstructJSON_Settings;

//   ptr = &mqtthandler_power_teleperiod;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 600; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
//   ptr->ConstructJSON_function = &mSonoffIFan::ConstructJSON_Power;

//   ptr = &mqtthandler_power_ifchanged;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 60; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
//   ptr->ConstructJSON_function = &mSonoffIFan::ConstructJSON_Power;
  
// }

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mSonoffIFan::MQTTHandler_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mSonoffIFan::MQTTHandler_Rate()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
//   }
// }

// /**
//  * @brief Check all handlers if they require action
//  * */
// void mSonoffIFan::MQTTHandler_Sender()
// {
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
//   }
// }

// #endif // USE_MODULE_NETWORK_MQTT

// #endif
