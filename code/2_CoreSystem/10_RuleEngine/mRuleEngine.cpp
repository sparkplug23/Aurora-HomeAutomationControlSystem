#include "mRuleEngine.h"

#ifdef USE_MODULE_CORE_RULES


// /* Null, because instance will be initialized on demand. */
// RuleEngine* RuleEngine::instance = nullptr;

// RuleEngine* RuleEngine::GetInstance(){
//   if (instance == nullptr){
//     instance = new RuleEngine();
//     // PRINT_FLUSHED("instance = new RuleEngine()");
//   }
//   return instance;
// }


int8_t mRuleEngine::Tasker(uint8_t function, JsonParserObject obj){

    // ALOG_INF(PSTR(D_LOG_APPLICATION D_BOOT_COUNT ));

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case TASK_PRE_INIT:
    //   Pre_Init();
    break;
    case TASK_INIT:
      // DefaultRuleForModule();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
//   if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    // /************
    //  * INIT SECTION * 
    // *******************/
    // case TASK_INIT:
    //   init();
    // break;
    // /************
    //  * SETTINGS SECTION * 
    // *******************/
    // case TASK_SETTINGS_LOAD_VALUES_INTO_MODULE: 
    //   // Settings_Load();
    // break;
    // case TASK_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   // Settings_Save();
    // break;
    // case TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   // Settings_Default();
    //   // pCONT_set->SettingsSave(2);
    // break;
    // /************
    //  * PERIODIC SECTION * 
    // *******************/
    // case TASK_LOOP: 
    //   EveryLoop();
    // break;  
    case TASK_EVERY_SECOND:
      // ALOG_TST(PSTR("DefaultRuleForModule"));   
      // // DefaultRuleForModule();
      // MQTTHandler_RefreshAll();

      // if(){ pCONT->Tasker_Interface(TASK_RULES_LOAD_FROM_PROGMEM_ID) };
      if(pCONT_time->uptime_seconds_nonreset == D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND){ RulesLoad_From_Progmem(); }

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    // /************
    //  * WEBPAGE SECTION * 
    // *******************/
    // #ifdef USE_MODULE_NETWORK_WEBSERVER
    // case TASK_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    //   break;
    // case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    //   break;
    // #endif //USE_MODULE_NETWORK_WEBSERVER
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

}

/**
 * @brief Needs updating to appendrules to add them instead of simply setting by indexing
 * Each "rule" should therefore become its own vector struct
 * 
 */
void mRuleEngine::RulesLoad_From_Progmem()
{

  ALOG_DBG(PSTR(D_LOG_RULES "RulesLoad_From_Progmem"));

  // DEBUG_DELAY(5000);

  DefaultRuleForModule();
  
  #ifdef USE_RULES_TEMPLATE
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,RULES_TEMPLATE,sizeof(RULES_TEMPLATE));
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF( PSTR("RULES_TEMPLATE Load = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_INFO

  pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);
  
  //IF TASKER RESULT WAS TRUE, THEN SUCCESS
  // pCONT_set->runtime.boot_status.rules_template_parse_success = 1;
  #endif //USE_RULES_TEMPLATE

  
}



/**
 * @brief For development, print the Rule out and what it means using basic and advanced (with words)
 * 
 * show trigger and command (not event)
 */
void mRuleEngine::ShowRuleAddLogByIndex(uint8_t show_type)
{

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR("1"
      "\n\rIndex:\t %d\n\r"
      "Trigger>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
      "Command>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
    "2"),
    pCONT_rules->rules_active_index,
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.module_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.function_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.device_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[0], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[1], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[2], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[3], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[4],
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.module_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.function_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id, 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[1], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[2], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[3], 
    pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[4]

    );

    #endif //  ENABLE_LOG_LEVEL_INFO
    
}

// Event (not command or trigger)
void mRuleEngine::ShowRuleEvent_AddLog(uint8_t show_type)
{

    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR(""
      "\n\r\n\r"
      "Event>>\n\r"
      "\tmodule:\t%d\n\r"
      "\tfunction:\t%d\n\r"
      "\tdevice_id:\t%d\n\r"
      "\tvalue:\t\t\t[%d,%d,%d,%d,%d]\n\r"
    ""),
    event_triggered.module_id, 
    event_triggered.function_id, 
    event_triggered.device_id, 
    event_triggered.value.data[0], 
    event_triggered.value.data[1], 
    event_triggered.value.data[2], 
    event_triggered.value.data[3], 
    event_triggered.value.data[4]
  );
    
    #endif //  ENABLE_LOG_LEVEL_INFO
}


/**
 * @brief Version 2 of triggering rules, adding the event but also include the type of function. Thie function can choose to pass that on, ignore or change the function type 
 * 
 * @param _module_id 
 * @param function_event 
 * @param _index 
 * @param _state 
 */
void mRuleEngine::NewEventRun(uint16_t _module_id, uint16_t function_event, uint8_t _index, uint8_t _state)
{
  Reset();
  event_triggered.module_id = _module_id;
  event_triggered.device_id = _index;
  event_triggered.function_id = function_event;
  event_triggered.value.data[0] = _state;

  // ALOG_ERR(PSTR("NewEventRun_NumArg Exceeded"));

  /**
   * @brief Mess version for now, added to here
   *  */

  // Legacy option, pass to all tasker_interfaces. This allows hard coded things to happen. ie Time runs out, turn relay off
  pCONT->Tasker_Interface(function_event);
  // New method to check the rules
  Tasker_Rules_Interface(function_event);

}


/**
 * @brief Version 2 of triggering rules, adding the event but also include the type of function. Thie function can choose to pass that on, ignore or change the function type 
 * 
 * @param _module_id 
 * @param function_event 
 * @param _index 
 * @param _data_length
 * @param ... Each byte of the data is its own argument which depends on the type of event (another method might use vectors) 
 */
void mRuleEngine::NewEventRun_NumArg(uint16_t _module_id, uint16_t function_event, uint8_t _index, uint8_t _data_length, ...)
{
  Reset();
  event_triggered.module_id = _module_id;
  event_triggered.function_id = function_event;
  event_triggered.device_id = _index;
  
  va_list arg;

  ALOG_INF( PSTR("module_id, function_id, device_id = %d,%d,%d"), event_triggered.module_id, event_triggered.function_id, event_triggered.device_id);
  
  va_start(arg, _data_length);
  for(int i = 0; i < _data_length; i++) 
  {

    if(i>D_RULE_DATA_VALUE_MAX_LENGTH-1){
      ALOG_ERR(PSTR("NewEventRun_NumArg Exceeded"));
      break;
    }

    event_triggered.value.data[i] = va_arg(arg, int);

  }
  va_end(arg);

  // AddLog_Array_Int(LOG_LEVEL_HIGHLIGHT, "data", event_triggered.value.data, ARRAY_SIZE(event_triggered.value.data));
  AddLog_Array(LOG_LEVEL_INFO, PSTR("data"), event_triggered.value.data, ARRAY_SIZE(event_triggered.value.data));
  // DEBUG_LINE_HERE; ALOG_INF(PSTR("event_triggered.module_id = %d"), event_triggered.module_id);

  // ShowRuleEvent_AddLog();

  // va_start(arg, formatP);
  // vsnprintf_P(pCONT_set->log_data, sizeof(pCONT_set->log_data), formatP, arg);
  // va_end(arg);

  /**
   * @brief Mess version for now, added to here
   *  */

  // Legacy option, pass to all tasker_interfaces. This allows hard coded things to happen. ie Time runs out, turn relay off
  pCONT->Tasker_Interface(function_event);
  // New method to check the rules
  Tasker_Rules_Interface(function_event);

}

void NewEventRun_Arg(uint16_t _module_id=0, uint16_t function_event = 0, uint8_t _index=0, ...); 

uint8_t mRuleEngine::GetConfiguredCount()
{
  uint8_t count = 0;
  for(uint8_t i=0;i<D_MAX_RULES;i++)
  {
    if(rules[i].flag_configured){ count++; }
  }
  return count;
}


uint8_t mRuleEngine::GetEnabledCount()
{
  uint8_t count = 0;
  for(uint8_t i=0;i<D_MAX_RULES;i++)
  {
    if(rules[i].flag_enabled){ count++; }
  }
  return count;
}


// All events here will only trigger based of function calls, when those occur happen throughout code
void mRuleEngine::Tasker_Rules_Interface(uint16_t function_input){


  // #ifdef ENABLE_LOG_LEVEL_INFO
  // ALOG_TST(PSTR("\n\r\n\r\n\rMATCHED Tasker_Rules_Interface function_input%d"),function_input);
  // #endif // ENABLE_LOG_LEVEL_INFO

  //maybe need to return rule(s) handled then leave taasker_interface


  for (int rule_index=0;rule_index<D_MAX_RULES;rule_index++)
  {

    // Only run if configured and enabled
    if(rules[rule_index].flag_configured && rules[rule_index].flag_enabled)
    {

    // Check this rule must act of the function
    if(rules[rule_index].trigger.function_id == function_input)
    {

      ALOG_INF(PSTR(D_LOG_RULES "MATCHED trigger.function_input[%d] to rule[%d]"), function_input, rule_index);

      rules_active_index = rule_index;

      /**
       * @brief Checking if all 3 criteria match can be one if statement. Or, 3 consequentive (not nested) for debugging and only running when matches == 3. easier to read
       * 
       */

      // Also check switch_index against rule index
      if(rules[rule_index].trigger.device_id == event_triggered.device_id)
      {

        ALOG_INF( PSTR(D_LOG_RULES "MATCHED trigger.device_id[%d] to rule[%d]"), rules[rule_index].trigger.device_id,rule_index );

        // char message[50];
        // memset(message,0,sizeof(message));
        // sprintf_P(message,PSTR("{\"Rule\":%d,\"EventIndex\":%d}"), rule_index, Event.index);
        // pCONT_mqtt->publish_device("status/debug/rules",message,false); //reconnect message
        // ^^ Add this later as a topic so I can know when rule triggers

        /**
         * Check if module_id has matched rule
         * */
        if(rules[rule_index].trigger.module_id == event_triggered.module_id)
        {
               
          ALOG_INF(PSTR(D_LOG_RULES "MATCHED trigger.module_id[%d] : Rule %d Triggered"), rules[rule_index].trigger.module_id);

          // Populate any jsoncommands to be executed, this takes precident over "State" controls
          if(rules[rule_index].command.json_commands_dlist_id>0)
          {

            D_DATA_BUFFER_CLEAR();
            pCONT_sup->GetTextIndexed(
              data_buffer.payload.ctr, 
              sizeof(data_buffer.payload.ctr), 
              pCONT_rules->rules[rule_index].command.json_commands_dlist_id, 
              pCONT_rules->jsonbuffer.data
            ); 
            data_buffer.payload.length_used += strlen(data_buffer.payload.ctr);

            ALOG_INF( PSTR("TASK_JSON_COMMAND_ID mrules=%s"), data_buffer.payload.ctr );
    
            pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

          }
          else // Execute normal state/value method if no jsoncommand was used
          {
            
            pCONT->Tasker_Interface(
              rules[rule_index].command.function_id
              // , // function the previous trigger is linked to
              // rules[rule_index].command.module_id //target module
            );

            // #ifdef ENABLE_LOG_LEVEL_ERROR
            //         ALOG_TST(PSTR("Execute Tasker_Interface(func=%d,module=%d,SourceIsRule=%d)"),
            //       rules[rule_index].command.function_id, // function the previous trigger is linked to
            //       rules[rule_index].command.module_id, //target module
            //       true  // runnig a rule, so don't call this loop back into this function
            //         );
            // #endif // ENABLE_LOG_LEVEL_INFO

          }

        } // trigger.module_id

      }
      else
      {

        ALOG_INF( PSTR(D_LOG_RULES ".............NOMATCH trigger.device_id[%d] to rule[%d]"),rules[rule_index].trigger.device_id,rule_index);
        
      }

    }

    } // configured and enabled

  } // for loop
 
}


void mRuleEngine::DefaultRuleForModule(){

rules_active_index = 0;

#ifdef ESP8266
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
    if(pCONT_set->Settings.module == mHardwarePins::MODULE_SONOFF_4CHPRO_ID){
      DefaultRule_Sonoff_4CHPRO();
    }else
  #endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    if(pCONT_set->Settings.module == mHardwarePins::MODULE_SONOFF_IFAN03_ID){
      DefaultRule_Sonoff_iFan03();
    }else
  #endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    if(pCONT_set->Settings.module == mHardwarePins::MODULE_SHELLY_DIMMER2_ID){
      DefaultRule_Shelly_Dimmer2();
    }else
  #endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    if(pCONT_set->Settings.module == mHardwarePins::MODULE_SHELLY2P5_ID){
      DefaultRule_Shelly_2p5();
    }
  #endif // USE_MODULE_TEMPLATE_SHELLY_2P5
#endif

}

/**
 * @brief: Function that will append a rule to the current list
 * 
 * Perhaps this needs added into each module, ie the way the rule is encoded/decoded is contained within the module eg RF433 
 * */
bool mRuleEngine::AppendEventToRules(mEvent::EVENT_PART* trigger_new, mEvent::EVENT_PART* command_new) // CHANGE TO ADD RULE
{

  uint8_t rule_count = GetConfiguredCount();
  uint8_t new_index = rule_count;

  if(rule_count>D_MAX_RULES){ return false; } //block new rules

  // #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)

  // TBD: Add check for unique rule

  // Clear rule
  memset(&rules[rule_count].trigger, 0, sizeof(mEvent::EVENT_PART));
  memset(&rules[rule_count].command, 0, sizeof(mEvent::EVENT_PART));

  // Copy rule
  memcpy(&rules[rule_count].trigger, trigger_new, sizeof(mEvent::EVENT_PART));
  memcpy(&rules[rule_count].command, command_new, sizeof(mEvent::EVENT_PART));
  // Activate rule
  rules[rule_count].flag_configured = true;
  rules[rule_count].flag_enabled = true;
  // If succesfully added
  return true;
  // pCONT_rules->rules_active_index++;

}


  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mRuleEngine::parsesub_Rule_Part(JsonParserObject jobj, mEvent::EVENT_PART* event){

    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_TST(PSTR("parsesub_Rule_Part"));
    #endif //  ENABLE_LOG_LEVEL_INFO

  JsonParserToken jtok;
  // JsonParserObject jobj2 = &jobj;

    // if(jtok = jobj["Module"]){
    //   ALOG_INF(PSTR("JTOK FOUND jobj[rule_name] 123"));    
    // } 
    uint16_t matched_id = 0;

    // Reset value length to 0
    event->value.length = 0;


    if(jtok = jobj["Module"]){
      if(jtok.isStr()){
        // if((matched_id=pCONT->GetModule_UniqueID_byName(jtok.getStr()))>=0){
        if((matched_id=pCONT->GetModuleID(jtok.getStr()))>0){
          event->module_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->module_id = matched_id;
        data_buffer.isserviced++;
      }
      // #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_INF(PSTR("JTOK FOUND Trigger Module module_id = %d"),matched_id);
      // ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
      // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger


    if(jtok = jobj["Function"]){
      if(jtok.isStr()){
        if((matched_id=pCONT_set->GetFunctionIDbyName(jtok.getStr()))>=0){
          event->function_id = matched_id;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->function_id = jtok.getInt();
        data_buffer.isserviced++;
      }
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_INF(PSTR("JTOK FOUND Trigger Module Function = %d"),matched_id);
      // ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger


    if(jtok = jobj["DeviceName"]){
      //Use module_id to know which devicelist item to try find
      // if(jtok.isStr()){
      //   if((matched_id=pCONT_set->GetFunctionIDbyFriendlyName(jtok.getStr()))>=0){
      //     event->function_id = matched_id;
      //     data_buffer.isserviced++;
      //   }
      // }else
      if(jtok.isNum()){
        event->device_id = jtok.getInt();
        data_buffer.isserviced++;
      }
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_INF(PSTR("JTOK FOUND Trigger Module DeviceName = %d"),event->device_id);
      // ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger


    if(jtok = jobj["State"]){
      if(jtok.isStr()){
        if((matched_id = pCONT_sup->GetStateNumber(jtok.getStr()))>=0){
          event->value.data[0] = matched_id;
          event->value.length++;// = 0;
          event->value.encoding = ENCODING_BYTES_ID;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->value.data[0] = jtok.getInt();
        event->value.length++;// = 0;
        event->value.encoding = ENCODING_BYTES_ID;
        data_buffer.isserviced++;
      }

      // Use state here to also set encoding, as it will know what the value is eg float = 4 bytes

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_INF(PSTR("JTOK FOUND Trigger Module State = %d"),event->value.data[0]);
      // ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger
    if(jtok = jobj["Value"]){
      if(jtok.isStr()){
        if((matched_id = pCONT_sup->GetStateNumber(jtok.getStr()))>=0){
          event->value.data[1] = matched_id;
          event->value.length++;// = 0;
          event->value.encoding = ENCODING_BYTES_ID;
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){
        event->value.data[1] = jtok.getInt();
        event->value.length++;// = 0;
        event->value.encoding = ENCODING_BYTES_ID;
        data_buffer.isserviced++;
      }

      // Use state here to also set encoding, as it will know what the value is eg float = 4 bytes

      #ifdef ENABLE_LOG_LEVEL_DEBUG
      ALOG_INF(PSTR("JTOK FOUND Trigger Module State = %d"),event->value.data[0]);
      // ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    
    }//end trigger

    if(jtok = jobj["JsonCommands"]){

    #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_INF(PSTR("JTOK FOUND Trigger Module JsonCommands = %s"), jtok.getStr());
    #endif //  ENABLE_LOG_LEVEL_INFO


      // if(jsonbuffer.data != nullptr){
        if(strlen(jsonbuffer.data) < sizeof(jsonbuffer.data)){

          // uint16_t available_space = 255 - strlen(event->p_json_commands);

          // event->p_json_commands = &jsonbuffer.data[0];

          pCONT_sup->AppendDList(jsonbuffer.data,jtok.getStr());


          event->json_commands_dlist_id = jsonbuffer.delims_used;

          jsonbuffer.delims_used++;

          // sprintf(event->p_json_commands,"%s|", jtok.getStr());
          jsonbuffer.bytes_used += strlen(jtok.getStr());

          // char dlist[200]; memset(dlist,0,sizeof(dlist));
          // pCONT_sup->AppendDList(dlist, D_TRANSITION "," D_PIXELS_UPDATE_PERCENTAGE);
  
          // I need to create the ability to move to add/edit buffer (like tas)
          // Rules can therefore only be created once at starttime for now          

          #ifdef ENABLE_LOG_LEVEL_INFO
          ALOG_INF(PSTR("JTOK FOUND jsonbuffer.data = %s"), jsonbuffer.data);
          ALOG_INF(PSTR("JTOK FOUND jsonbuffer.bytes_used = %d"), jsonbuffer.bytes_used);
          #endif // ENABLE_LOG_LEVEL_INFO

          // snprintf(event->p_json_commands+strlen(event->p_json_commands),available_space,)
        }
      // }

    
    }//end trigger

//Execute test



}


void mRuleEngine::parse_JSONCommand(JsonParserObject obj)
{

// return;


  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  JsonParserObject jobj = 0;
  int16_t tmp_id = 0;

  uint8_t rule_index = 0;
  char rule_name[10] = {0};
  for(int rule_index=0;rule_index<MAX_RULE_VARS;rule_index++)
  {
    sprintf(rule_name, "Rule%d", rule_index);
    
    if(jtok = obj[rule_name])
    {
      
      #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_HGL(PSTR("MATCHED Rule%d"),rule_index);
        // DEBUG_DELAY(1000);
      #endif // ENABLE_LOG_LEVEL_INFO

      mEvent::EVENT_PART* p_event = nullptr;

      jobj = obj[rule_name].getObject()["Trigger"];
      if(!jobj.isNull()){
        parsesub_Rule_Part(jobj, &pCONT_rules->rules[rule_index].trigger);
        // Activate rule
        rules[rule_index].flag_configured = true;
        rules[rule_index].flag_enabled = true;
      }

      jobj = obj[rule_name].getObject()["Command"];
      if(!jobj.isNull()){
        parsesub_Rule_Part(jobj, &pCONT_rules->rules[rule_index].command);        
      }

      mqtthandler_settings.flags.SendNow = true;

    }

  }  


  if(jtok = obj["AddRule"])
  { // Assuming going forward vectors will be used, but for now just slot relative from 0 index 

    ALOG_INF(PSTR( D_LOG_RULES "AddRule"));

    if(jtok_sub = jtok.getObject()["Default"])
    {
      
      if(jtok_sub.isArray())
      {
        
        JsonParserArray array = jtok_sub;
        uint8_t index = 0;
        for(auto& object:array)
        {
          // ALOG_INF(PSTR(D_LOG_RULES "AddRule Relay1Follow %s"),object.getStr());
          AppendRule_FromDefault_UsingName((char*)object.getStr());
        }

      }
    }
    
    // if(strcmp(jtok.getStr(), "Switch1Change->Relay1Follow")==0)
    // {
    
    
    //   ALOG_INF(PSTR(D_LOG_RULES "AddRule Relay1Follow"));

    // }


    
    
    
    // mEvent::EVENT_PART* p_event = nullptr;


    // jobj = obj[rule_name].getObject()["Trigger"];
    // if(!jobj.isNull()){
    //   parsesub_Rule_Part(jobj, &pCONT_rules->rules[rule_index].trigger);

    //   // tmp fix, just set but later needs made dynamic

    //   // Activate rule
    //   rules[rule_index].flag_configured = true;
    //   rules[rule_index].flag_enabled = true;


    // }

    // jobj = obj[rule_name].getObject()["Command"];
    // if(!jobj.isNull()){
    //   parsesub_Rule_Part(jobj, &pCONT_rules->rules[rule_index].command);
      
    // }

    
    #ifdef USE_MODULE_NETWORK_MQTT
    mqtthandler_state_ifchanged.flags.SendNow = true;
    #endif // USE_MODULE_NETWORK_MQTT

  }





    /**
     * Get modules
     * */
    // int8_t module_id = 0;
    
    // if(jtok = jobj["Command"].getObject()["Module"]){

    //   if(jtok.isStr()){
    //     if((tmp_id=pCONT->GetModuleIDbyFriendlyName(jtok.getStr()))>=0){
    //       ALOG_INF(PSTR("22JTOK FOUND Trigger Module tmp_id = %d"),tmp_id);
    //       p_event->module_id = tmp_id;
    //       data_buffer.isserviced++;
    //     }
    //   }else
    //   if(jtok.isNum()){
    //     ALOG_INF(PSTR("JTOK FOUND Trigger Module"));
    //     // get pointer to rule via rule_index
    //     // CommandSet_Rule_Module_ID(jtok.getInt(), );
    //     data_buffer.isserviced++;
    //   }
    //   // #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   // ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
    //   // #endif // ENABLE_LOG_LEVEL_DEBUG
    
    // }//end trigger

    //Devicename will need to search the shared buffer, possibly just read it in otherwise


    //SetPower is getstate (if string), else, just put value in







}



void mRuleEngine::AppendRule_FromDefault_UsingName(const char* name)
{

  if(strcmp(name, "Switch1Change->Relay1Follow")==0)
  {
  
    ALOG_INF(PSTR(D_LOG_RULES "AddRule Relay1Follow NEW Switch1Change->Relay1Follow"));

    mEvent::EVENT_PART* p_event = nullptr;

    if(pCONT_rules->rules_active_index>D_MAX_RULES){ return; } //block new rules

    #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)
    
    // Trigger0
    pCONT_rules->rules[pCONT_rules->rules_active_index].flag_enabled = true;   
    pCONT_rules->rules[pCONT_rules->rules_active_index].flag_configured = true;   
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = STATE_NUMBER_TOGGLE_ID;  // Toggled 
    // Command0
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = STATE_NUMBER_FOLLOW_ID;  // Toggle

    pCONT_rules->rules_active_index++;

    #endif // defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)

  }

  // if(strcmp(name, "Switch1ON->Relay1On2Hrs?Switch1OFF->Relay1Off")==0)
  // {
  
  //   ALOG_INF(PSTR(D_LOG_RULES "AddRule Relay1Follow NEW Switch1Change->Relay1Follow"));

  //   mEvent::EVENT_PART* p_event = nullptr;

  //   if(pCONT_rules->rules_active_index>D_MAX_RULES){ return; } //block new rules

  //   #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)
    
  //   // Trigger0
  //   pCONT_rules->rules[pCONT_rules->rules_active_index].flag_enabled = true;   
  //   pCONT_rules->rules[pCONT_rules->rules_active_index].flag_configured = true;   
  //   p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
  //   p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
  //   p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
  //   p_event->device_id = 0;
  //   p_event->value.length = 0;
  //   p_event->value.data[p_event->value.length++] = STATE_NUMBER_ON_ID;  // Toggled 
  //   // Command0
  //   p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
  //   p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
  //   p_event->function_id = TASK_EVENT_SET_POWER_ID;
  //   p_event->device_id = 0;
  //   p_event->value.length = 0;
  //   p_event->value.data[p_event->value.length++] = STATE_NUMBER_ON_ID;  // Toggle

  //   pCONT_rules->rules_active_index++;


  //   // Trigger1
  //   pCONT_rules->rules[pCONT_rules->rules_active_index].flag_enabled = true;   
  //   pCONT_rules->rules[pCONT_rules->rules_active_index].flag_configured = true;   
  //   p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
  //   p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
  //   p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
  //   p_event->device_id = 0;
  //   p_event->value.length = 0;
  //   p_event->value.data[p_event->value.length++] = STATE_NUMBER_OFF_ID;  // Toggled 
  //   // Command1
  //   p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
  //   p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
  //   p_event->function_id = TASK_EVENT_SET_POWER_ID;
  //   p_event->device_id = 0;
  //   p_event->value.length = 0;
  //   p_event->value.data[p_event->value.length++] = STATE_NUMBER_OFF_ID;  // Toggle

    
  //   pCONT_sup->AppendDList(jsonbuffer.data,jtok.getStr());
  //   event->json_commands_dlist_id = jsonbuffer.delims_used;
  //   jsonbuffer.delims_used++;
  //   jsonbuffer.bytes_used += strlen(jtok.getStr());




  //   pCONT_rules->rules_active_index++;

  //   #endif // defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)

  // }




  ALOG_INF(PSTR(D_LOG_RULES "AppendRule_FromDefault_UsingName=%d"), pCONT_rules->rules_active_index);


}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_SystemRestartID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mRuleEngine::CommandSet_Rule(uint8_t value){

//   /**
//    * 0 = invalid
//    * 1 = soft restart
//    * 2 = hardware wdt restart
//    * 3+ is disabling things for stability ie fastboot or erasing settings to default
//    * 
//    * 
//    * */

//   // animation.transition.pixels_to_update_as_number = GetPixelsToUpdateAsPercentageFromNumber(value);
//   // animation.transition.pixels_to_update_as_percentage = value;

//   if(value == 1){
//     pCONT_wif->EspRestart();
//   }
  
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_TRANSITION,D_PIXELS_UPDATE_PERCENTAGE)), value);
//   // #endif

// }

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
uint8_t mRuleEngine::ConstructJSON_Settings(uint8_t json_method, bool json_appending){


  JBI->Start();
    // JBI->Add(PM_DEVICES_CONNECTED, settings.relays_connected);

    char buffer[200];

    char name[10]={0};

      JBI->Object_Start("Settings");
        JBI->Add("Default",settings.loaded_default_for_moduled);
      JBI->Object_End();

    for(uint8_t id=0;id<GetConfiguredCount();id++){

        sprintf(name, "Rule%d", id);

      // JBI->Object_Start("Settings");
      
      // JBI->Object_End();

      JBI->Level_Start_P(name);//DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   

        JBI->Add("Conf", rules[id].flag_configured);
        JBI->Add("Enab", rules[id].flag_enabled);
        JBI->Level_Start_P("Source");
            JBI->Add("module_id", rules[id].trigger.module_id);
            JBI->Add("function_id", rules[id].trigger.function_id);
            JBI->Add("device_id", rules[id].trigger.device_id);
            JBI->Add("json", rules[id].trigger.json_commands_dlist_id);
            JBI->Array_AddArray("params", rules[id].trigger.value.data, 5);
            JBI->Add("len", rules[id].trigger.value.length);

            
            // if(rules[id].trigger.p_json_commands!=nullptr){
            //     JBI->Add("json", rules[id].trigger.p_json_commands);
            // }
        JBI->Object_End(); 

        JBI->Level_Start_P("Destination");
            JBI->Add("module_id", rules[id].command.module_id);
            JBI->Add("function_id", rules[id].command.function_id);
            JBI->Add("device_id", rules[id].command.device_id);
            JBI->Add("json", rules[id].command.json_commands_dlist_id);
            JBI->Array_AddArray("params", rules[id].command.value.data, 5);
            JBI->Add("len", rules[id].command.value.length);
            // if(rules[id].command.p_json_commands!=nullptr){

            if(rules[id].command.json_commands_dlist_id>0){
            //     JBI->Add("json", rules[id].command.p_json_commands);


                // pCONT_sup->GetTextIndexed(buffer, sizeof(buffer), rules[id].command.json_commands_dlist_id, jsonbuffer.data);  // should this be _P?
                //     JBI->Add("json", buffer); 

                    
                char buffer_unescaped[D_COMMAND_BUFFER_LENGTH] = {0};
                char buffer_escaped[D_COMMAND_BUFFER_LENGTH+50] = {0};
                uint8_t len  = 0;
                
                pCONT_sup->GetTextIndexed(
                    buffer_unescaped, 
                    sizeof(buffer_unescaped), 
                    pCONT_rules->rules[id].command.json_commands_dlist_id, 
                    pCONT_rules->jsonbuffer.data
                ); 

                for(int i=0;i<strlen(buffer_unescaped);i++){
                    if(buffer_unescaped[i] == '\"'){
                    len+=sprintf(buffer_escaped+len,"\\\"");
                    }else{    
                    buffer_escaped[len++] = buffer_unescaped[i];
                    }
                }
                JBI->Add("json_es", buffer_escaped); 

            }
        JBI->Object_End(); 


      JBI->Object_End(); 
    }

    // JBI->Add("data", jsonbuffer.data); 
    JBI->Add("size",sizeof(rules));

    JBI->Add("ConfigCount", GetConfiguredCount());
    // JBI->Add("ConfigCount2", rule_count2);
    JBI->Add("EnabledCount", GetEnabledCount());

      // JBI->Add("jsonbuffer_data", jsonbuffer.data);    
      JBI->Add("jsonbuffer_bytes_used", jsonbuffer.bytes_used);    
  
  return JBI->End();

}

uint8_t mRuleEngine::ConstructJSON_State(uint8_t json_method, bool json_appending){

  char buffer[100];
  JBI->Start();

    JBI->Object_Start("EventTriggered");

      JBI->Add("DeviceID", event_triggered.device_id);
      JBI->Add("FunctionID", event_triggered.function_id);    
      JBI->Add("ModuleID", event_triggered.module_id);    




      

    JBI->Object_End();



  return JBI->End();
}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mRuleEngine::MQTTHandler_Init()
{

  struct handler<mRuleEngine>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRuleEngine::ConstructJSON_State;

} 

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif // USE_MODULE_CORE_RULES