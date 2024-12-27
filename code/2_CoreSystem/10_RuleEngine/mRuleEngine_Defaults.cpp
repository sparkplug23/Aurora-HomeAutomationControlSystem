
#include "mRuleEngine.h"

#ifdef USE_MODULE_CORE_RULES

/**
 * @brief Add Rules that pair buttons with toggling the relays on board
 * 
 */
#ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
void mRuleEngine::DefaultRule_Sonoff_4CHPRO()
{

    ALOG_DBM(PSTR("DefaultRule_Sonoff_4CHPRO") );
    
    mEvent::EVENT_PART* p_event = nullptr;

    if(GetConfiguredCount()>D_MAX_RULES){
        ALOG_ERR(PSTR("Unable to add rules: Out of memory"));
        return; 
    } 

    #if defined(USE_MODULE_SENSORS_BUTTONS) && defined(USE_MODULE_DRIVERS_RELAY)
    
    mEvent::EVENT_PART event_trig;
    mEvent::EVENT_PART event_comm;

    memset(&event_trig, 0, sizeof(mEvent::EVENT_PART));
    memset(&event_comm, 0, sizeof(mEvent::EVENT_PART));

    // Trigger0
    event_trig.module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
    event_trig.function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    event_trig.device_id = 0; // Button0
    event_trig.value.length = 0;
    event_trig.value.data[event_trig.value.length++] = 1;  // Pressed 
    // Command0
    event_comm.module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    event_comm.function_id = TASK_EVENT_SET_POWER_ID;
    event_comm.device_id = 0; // Button0
    event_comm.value.length = 0;
    event_comm.value.data[event_comm.value.length++] = STATE_NUMBER_TOGGLE_ID; // POWER_TOGGLE;  // STATE_NUMBER_INCREMENT_ID
    // AddRule
    AppendEventToRules(&event_trig, &event_comm);

    // Trigger0
    event_trig.module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
    event_trig.function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    event_trig.device_id = 1; // Button0
    event_trig.value.length = 0;
    event_trig.value.data[event_trig.value.length++] = 1;  // Pressed 
    // Command0
    event_comm.module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    event_comm.function_id = TASK_EVENT_SET_POWER_ID;
    event_comm.device_id = 1; // Button0
    event_comm.value.length = 0;
    event_comm.value.data[event_comm.value.length++] = STATE_NUMBER_TOGGLE_ID;  // Increment 
    // AddRule
    AppendEventToRules(&event_trig, &event_comm);

    // Trigger0
    event_trig.module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
    event_trig.function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    event_trig.device_id = 2; // Button0
    event_trig.value.length = 0;
    event_trig.value.data[event_trig.value.length++] = 1;  // Pressed 
    // Command0
    event_comm.module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    event_comm.function_id = TASK_EVENT_SET_POWER_ID;
    event_comm.device_id = 2; // Button0
    event_comm.value.length = 0;
    event_comm.value.data[event_comm.value.length++] = STATE_NUMBER_TOGGLE_ID;  // Increment 
    // AddRule
    AppendEventToRules(&event_trig, &event_comm);

    // Trigger0
    event_trig.module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
    event_trig.function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    event_trig.device_id = 3; // Button0
    event_trig.value.length = 0;
    event_trig.value.data[event_trig.value.length++] = 1;  // Pressed 
    // Command0
    event_comm.module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    event_comm.function_id = TASK_EVENT_SET_POWER_ID;
    event_comm.device_id = 3; // Button0
    event_comm.value.length = 0;
    event_comm.value.data[event_comm.value.length++] = STATE_NUMBER_TOGGLE_ID;  // Increment 
    // AddRule
    AppendEventToRules(&event_trig, &event_comm);

    settings.loaded_default_for_moduled = true;

    #endif

}
#endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03



#ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
void mRuleEngine::DefaultRule_Sonoff_iFan03()
{

    ALOG_DBM( PSTR("DefaultRule_Sonoff_iFan03"));
    
    mEvent::EVENT_PART* p_event = nullptr;

    if(GetConfiguredCount()>D_MAX_RULES){
        ALOG_ERR(PSTR("Unable to add rules: Out of memory"));
        return; 
    } 

    #if defined(USE_MODULE_SENSORS_BUTTONS) && defined(USE_MODULE_CONTROLLER_SONOFF_IFAN)
    
    mEvent::EVENT_PART event_trig;
    mEvent::EVENT_PART event_comm;

    memset(&event_trig, 0, sizeof(mEvent::EVENT_PART));
    memset(&event_comm, 0, sizeof(mEvent::EVENT_PART));

    // Trigger0
    event_trig.module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
    event_trig.function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    event_trig.device_id = 0; // Button0
    event_trig.value.length = 0;
    event_trig.value.data[event_trig.value.length++] = 1;  // Pressed 
    // Command0
    event_comm.module_id = D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID;
    event_comm.function_id = TASK_EVENT_SET_SPEED_ID;
    event_comm.device_id = 0; // Button0
    event_comm.value.length = 0;
    event_comm.value.data[event_comm.value.length++] = STATE_NUMBER_INCREMENT_ID; // Increment and wrap fan speeds
    // AddRule
    AppendEventToRules(&event_trig, &event_comm);

    settings.loaded_default_for_moduled = true;

    #endif

}

// {

//     #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//     ALOG_DBG(PSTR("DefaultRule_Sonoff_iFan03"));

//     #endif // ENABLE_LOG_LEVEL_INFO

//     mEvent::EVENT_PART* p_event = nullptr;

//     if(pCONT_rules->rules_active_index>2){ return; } //block new rules

//     #if defined(USE_MODULE_SENSORS_BUTTONS) && defined(USE_MODULE_CONTROLLER_SONOFF_IFAN)
    
//     // // Trigger0
//     // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
//     // p_event->module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
//     // p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
//     // p_event->device_id = 0; // Button0
//     // p_event->value.length = 0;
//     // p_event->value.data[p_event->value.length++] = 1;  // Pressed 
//     // // Command0
//     // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
//     // p_event->module_id = D_UNIQUE_MODULE_CONTROLLER_CEILINGFAN_ID;
//     // p_event->function_id = TASK_EVENT_SET_SPEED_ID;
//     // p_event->device_id = 0; 
//     // p_event->value.length = 0;
//     // p_event->value.data[p_event->value.length++] = STATE_NUMBER_INCREMENT_ID;  // Increment 
//     // pCONT_rules->rules_active_index++;
//     // settings.loaded_default_for_moduled = true;

//     #endif

//         //p_event->value.data[p_event->value.length++] = if just on, then here would be speed;  // 255 means next slot contains command and not value, "255" needs to be control ids 
    


// }
#endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03



#ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
void mRuleEngine::DefaultRule_Shelly_Dimmer2(){

    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    ALOG_DBG(PSTR("DefaultRule_Shelly_Dimmer2"));

    #endif // ENABLE_LOG_LEVEL_INFO

    mEvent::EVENT_PART* p_event = nullptr;

    if(pCONT_rules->rules_active_index>2){ return; } //block new rules

    #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_SHELLY_DIMMER)
    
    #if !defined(USE_HARDWARE_DEFAULT_RULES_SHELLY_DIMMER_BOTH_INPUTS_TOGGLE_OUTPUT) && !defined(USE_HARDWARE_DEFAULT_RULES_SHELLY_DIMMER_INPUTS_TOGGLE_OUTPUT_TO_HALF_AND_FULL_BRIGHTNESS)
    #define USE_HARDWARE_DEFAULT_RULES_1 // Assume default if neither are found
    #endif 
    
    /**
     * @brief Assume default
     * 
     */
    #if defined(USE_HARDWARE_DEFAULT_RULES_SHELLY_DIMMER_BOTH_INPUTS_TOGGLE_OUTPUT) && defined(USE_HARDWARE_DEFAULT_RULES_1)
    // Trigger0
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // Command0
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_SHELLY_DIMMER_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggle
    p_event->value.data[p_event->value.length++] = 50;  // Dim 50% 
    pCONT_rules->rules_active_index++;
    settings.loaded_default_for_moduled = true;
    // Trigger1
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 1; 
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // Command1
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_SHELLY_DIMMER_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 1;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggle
    p_event->value.data[p_event->value.length++] = 100;  // Full 100% 
    pCONT_rules->rules_active_index++;
    settings.loaded_default_for_moduled = true;
    #endif // USE_HARDWARE_DEFAULT_RULES_1

    #ifdef USE_HARDWARE_DEFAULT_RULES_SHELLY_DIMMER_INPUTS_TOGGLE_OUTPUT_TO_HALF_AND_FULL_BRIGHTNESS
    // Trigger0
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // Command0
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_SHELLY_DIMMER_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggle
    p_event->value.data[p_event->value.length++] = 50;  // Dim 50% 
    pCONT_rules->rules_active_index++;
    settings.loaded_default_for_moduled = true;
    // Trigger1
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 1; 
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // Command1
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_SHELLY_DIMMER_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 1;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggle
    p_event->value.data[p_event->value.length++] = 100;  // Full 100% 
    pCONT_rules->rules_active_index++;
    settings.loaded_default_for_moduled = true;
    #endif // USE_HARDWARE_DEFAULT_RULES_2

    #endif

}
#endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2





#ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
void mRuleEngine::DefaultRule_Shelly_2p5(){

    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    ALOG_DBG(PSTR("DefaultRule_Shelly_2p5"));

    #endif //  ENABLE_LOG_LEVEL_INFO
    mEvent::EVENT_PART* p_event = nullptr;

    if(pCONT_rules->rules_active_index>D_MAX_RULES){ return; } //block new rules

    #if defined(USE_MODULE_SENSORS_SWITCHES) && defined(USE_MODULE_DRIVERS_RELAY)
    
    // Trigger0
    pCONT_rules->rules[pCONT_rules->rules_active_index].flag_enabled = true;   
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggled 
    // Command0
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 0;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggle
    pCONT_rules->rules_active_index++;

    // Trigger1
    pCONT_rules->rules[pCONT_rules->rules_active_index].flag_enabled = true; 
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    p_event->module_id = D_UNIQUE_MODULE_SENSORS_SWITCHES_ID;
    p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    p_event->device_id = 1; 
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // Command1
    p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    p_event->function_id = TASK_EVENT_SET_POWER_ID;
    p_event->device_id = 1;
    p_event->value.length = 0;
    p_event->value.data[p_event->value.length++] = 2;  // Toggle
    pCONT_rules->rules_active_index++;

    // // Trigger2
    // pCONT_rules->rules[pCONT_rules->rules_active_index].enabled = true; 
    // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    // p_event->module_id = D_UNIQUE_MODULE_SENSORS_BUTTONS_ID;
    // p_event->function_id = TASK_EVENT_INPUT_STATE_CHANGED_ID;
    // p_event->device_id = 1; 
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // // Command2
    // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    // p_event->module_id = D_UNIQUE_MODULE_DRIVERS_RELAY_ID;
    // p_event->function_id = TASK_EVENT_SET_POWER_ID;
    // p_event->device_id = 1;
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = 2;  // Toggle
    // pCONT_rules->rules_active_index++;

    settings.loaded_default_for_moduled = true;

    #endif

}
#endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2























#endif // USE_MODULE_CORE_RULES