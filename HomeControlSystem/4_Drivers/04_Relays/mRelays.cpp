#include "mRelays.h"

#ifdef USE_MODULE_DRIVERS_RELAY

const char* mRelays::PM_MODULE_DRIVERS_RELAY_CTR = D_MODULE_DRIVERS_RELAY_CTR;
const char* mRelays::PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR = D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR;

/**
 * @brief: Module Interface
 * */
int8_t mRelays::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:
      SubTask_Relay_Time_To_Remain_On_Seconds();
      SubTask_Relay_Time_To_Briefly_Turn_Off_Then_On_Seconds();
      
  // AddLog(LOG_LEVEL_TEST, PSTR("MATCHED FUNC_EVERY_SECOND"));
  // MQTTHandler_Set_RefreshAll();

    break;
    case FUNC_EVERY_MINUTE:
      SubTask_Every_Minute();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case FUNC_SET_POWER_ON_ID:
      CommandSet_Relay_Power(STATE_NUMBER_ON_ID);
    break;    
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case FUNC_EVENT_SET_POWER_ID:
      RulesEvent_Set_Power();
    break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    // #ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    // #endif    
    /************
     * WEBPAGE SECTION * 
    *******************/
    // #ifndef DISABLE_WEBSERVER
    // case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Draw_PageTable();
    // break;
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table();
    // break;
    // case FUNC_WEB_APPEND_ROOT_BUTTONS:
    //   WebAppend_Root_Add_Buttons();
    // break;
    // #endif // DISABLE_WEBSERVER
  } // end switch
} // END function








/*
1) MQTT control, including minutes on
2) Report relay status correctly
3) Button controls work, toggling
4) Save power state in settings
5) Restore power state from settings on boot
6) Enable mode to start with, on, off, as saved
7) Schedule automatic tod on/off, include duty cycle (with variable for scele, seconds, minutes, hours)


Time_On : Auto turn off after x seconds (or minutes)
Scheduled time, time on/off by a scheduled time, or,
Disable via internal setters (use flag to check or not) when outside permitted time windows

time_short = 4 bytes, on/off pair = 8 bytes, 4 periods = 32 bytes per relay, 4 relays = 128 bytes ram (passible)


add table (optional flag to turn this on/off)
Relay Name:    ontime, offtime, timeon, last controlled by//
  // I should add a relay "type" ie, external power, internal power, light

*/


void mRelays::Pre_Init(void){
  
  settings.fEnableSensor = false;
  settings.relays_connected = 0;

  // Lets check each type on their own, normal, inverted etc
  for(uint8_t driver_index=0; driver_index<MAX_RELAYS; driver_index++)
  {
    if(pCONT_pins->PinUsed(GPIO_REL1_ID, driver_index))
    {
      uint8_t pin_number = pCONT_pins->Pin(GPIO_REL1_ID, driver_index);
      pinMode(pin_number, OUTPUT);
      settings.fEnableSensor = true;
      pCONT_set->devices_present++;
      if(settings.relays_connected++ >= MAX_RELAYS){ break; }
    }else
    if(pCONT_pins->PinUsed(GPIO_REL1_INV_ID, driver_index))
    {
      uint8_t pin_number = pCONT_pins->Pin(GPIO_REL1_INV_ID, driver_index);
      pinMode(pin_number, OUTPUT);
      bitSet(rel_inverted, driver_index); //temp fix
      settings.fEnableSensor = true;
      pCONT_set->devices_present++;
      if(settings.relays_connected++ >= MAX_RELAYS){ break; }
    }
  }

}


void mRelays::init(void){

  // settings.relays_connected = pCONT_set->devices_present; //phase out

  // settings.relays_connected = MAX_RELAYS;


  // clear all settings to 0
  memset(&relay_status, 0, sizeof(relay_status));

  // Set defaults
  for(int relay_id=0;relay_id<MAX_RELAYS;relay_id++){
    relay_status[relay_id].timer_decounter.seconds = 0;
    relay_status[relay_id].timer_decounter.active = false;
  }

  #ifdef ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
  settings.flags.enabled_relays_allowed_time_window_checks = true;
  // relay_status[0].enabled_ranges[0].enabled = true;
  // relay_status[0].enabled_ranges[0].ontime = {8, 14, 25, 0}; //8 meaning all days   3pm to 8am
  // relay_status[0].enabled_ranges[0].offtime = {8, 7, 0, 0}; //8 meaning all days
  #else
  settings.flags.enabled_relays_allowed_time_window_checks = false;
  #endif // ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS


//{"PowerName":0,"Relay":{"TimeOn":5},"EnabledTime":{"Enabled":1,"OnTime":"01D12:34:56","OffTime":"12D34:56:78"}}


      // &relay_status[relay_id].enabled_ranges[0].ontime,
      // &relay_status[relay_id].enabled_ranges[0].offtime

  
  SetPowerOnState();
}

void mRelays::SubTask_Every_Minute(){

  #ifdef ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS
    // Check if time is outside limits and set flag


  #endif


}

#ifdef USE_MODULE_CORE_RULES

void mRelays::RulesEvent_Set_Power(){

  // struct RELAY_EVENT_PARAMETERS{
  //   uint8_t index;
  //   uint8_t state;
  //   uint8_t state;
  // }rule_event_layout;


  AddLog(LOG_LEVEL_TEST, PSTR("MATCHED RulesEvent_Set_Power"));

  uint8_t relay_index = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;

  uint8_t relay_state = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];


  ExecuteCommandPower(relay_index, relay_state, SRC_IGNORE);
  
  // MQTTHandler_Set_RefreshAll();

}
#endif // USE_MODULE_CORE_RULES


/**
 * @note: Time a relay will remain ON
 * */
void mRelays::SubTask_Relay_Time_To_Remain_On_Seconds(){
  
  // Stop if no relays connected
  if(!settings.relays_connected){
    return;
  }

  
  // Loop across each connected relay
  for(int relay_id=0;relay_id<settings.relays_connected;relay_id++){
    
    /**
     * Temporary timeoff will require blocking this command
     * */
    if(relay_status[relay_id].timer_off_then_on_decounter.seconds>0) //if active, then stop this function
    {
      AddLog(LOG_LEVEL_WARN, PSTR("SubTask_Relay_Time_To_Remain_On_Seconds BLOCKED by timeoffthenon"));
      return;     
    }

    //change seconds
    if(relay_status[relay_id].time_seconds_on){
      relay_status[relay_id].time_seconds_on++; // increment if positive, if 0, it doesnt increase
    }

    // Auto time off decounters
    if(relay_status[relay_id].timer_decounter.seconds == 1){ //if =1 then turn off and clear to 0
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "relay_status[%d].timer_decounter.seconds==1 and disable"), relay_id);
      #endif       

      CommandSet_Relay_Power(0, relay_id);

      relay_status[relay_id].timer_decounter.seconds=0;

    }else
    if(relay_status[relay_id].timer_decounter.seconds>1){ //if =1 then turn off and clear to 0
      relay_status[relay_id].timer_decounter.seconds--; //decrease

      CommandSet_Relay_Power(1, relay_id);
      
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "relay_status[%d].timer_decounter.seconds=%d dec"),relay_id, relay_status[relay_id].timer_decounter.seconds);
      #endif

      mqtthandler_state_ifchanged.flags.SendNow = true;

    }else{
      //assumed off ie == 0
    }
  }//end for

}

/**
 * @brief: Time a relay will turn off in seconds, then return to on again
 * */
void mRelays::SubTask_Relay_Time_To_Briefly_Turn_Off_Then_On_Seconds(){
  
  // Stop if no relays connected
  if(!settings.relays_connected){
    return;
  }
  
  // Loop across each connected relay
  for(int relay_id=0;relay_id<settings.relays_connected;relay_id++){

    // Auto time off decounters
    if(relay_status[relay_id].timer_off_then_on_decounter.seconds == 1){ //if =1 then turn off and clear to 0
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "relay_status[%d].timer_off_then_on_decounter.seconds==1 and disable"), relay_id);
      #endif       

      CommandSet_Relay_Power(1, relay_id); // TURN ON

      relay_status[relay_id].timer_off_then_on_decounter.seconds=0;

    }else
    if(relay_status[relay_id].timer_off_then_on_decounter.seconds>1){ //if =1 then turn off and clear to 0
      relay_status[relay_id].timer_off_then_on_decounter.seconds--; //decrease

      CommandSet_Relay_Power(0, relay_id); // TURN OFF
      
      #ifdef ENABLE_LOG_LEVEL_COMMANDS
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO "relay_status[%d].timer_off_then_on_decounter.seconds=%d dec"),relay_id, relay_status[relay_id].timer_off_then_on_decounter.seconds);
      #endif

      mqtthandler_state_ifchanged.flags.SendNow = true;

    }else{
      //assumed off ie == 0
    }
  }//end for

}





/*********************************************************************************************
* HELPER & CONVERSION FUNCTIONS ******************************************************************
*************************************************************************************************/

const char* mRelays::GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  DEBUG_LINE;
  if(device_id >= settings.relays_connected){ 
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_RELAYS "device_id >= settings.relays_connected %d %d"),device_id,settings.relays_connected);
    return PM_SEARCH_NOMATCH; 
  }
  DEBUG_LINE;
  // return DLI->GetDeviceNameWithEnumNumber(E M_MODULE_DRIVERS_RELAY_ID, device_id, buffer, buffer_length);
  return DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), device_id, buffer, buffer_length);
}

const char* mRelays::GetRelayNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length){
  
  char onoffctr[5];
  switch(CommandGet_Relay_Power(device_id)){
    case 0: sprintf(onoffctr,"%s","OFF"); break;
    case 1: sprintf(onoffctr,"%s","ON");  break;
  }

  char buffer_internal[50];
  sprintf(buffer, "%s %s", GetRelayNamebyIDCtr(device_id, buffer_internal, sizeof(buffer_internal)), onoffctr);

  ALOG_DBM( PSTR("GetRelayNameWithStateLongbyIDCtr=%s"),buffer);

  return buffer;
}



int8_t mRelays::GetRelayIDbyName(const char* c){
  if(c=='\0'){ return -1; }  

  // int8_t device_id; // not needed, to be phased out
  int16_t class_id = GetModuleUniqueID();//E M_MODULE_DRIVERS_RELAY_ID;

  // int16_t device_id_found = DLI->GetDeviceIDbyName(c,device_id,class_id);
  int16_t device_id_found = DLI->GetDeviceIDbyName(c,class_id);

  AddLog(LOG_LEVEL_HIGHLIGHT,PSTR("\n\r\n\rdevice_id_found = %d"),device_id_found);

  // show options
  if(device_id_found == -1){
    // for(int ii=0;ii<pCONT_set->GetDeviceNameCount(D_MODULE_DRIVERS_RELAY_ID);ii++){
    //   AddLog(LOG_LEVEL_INFO, PSTR("GetDeviceIDbyName option #%d"),ii,pCONT_set->GetDeviceIDbyName(c,pCONT_set->Settings.device_name_buffer.name_buffer,&ii,&class_id));
    // }
    AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\nsearching=%s"),c);
    AddLog(LOG_LEVEL_INFO,PSTR("\n\r\n\name_buffer = %s"),pCONT_set->Settings.device_name_buffer.name_buffer);


  }



  return device_id_found;

}


bool mRelays::IsRelayTimeWindowAllowed(uint8_t relay_id, uint8_t range_id){

  bool isenabled = false;

  //if range_id == 255, then check all, else check only it

  //check all, one for now
  for(int range_id=0;range_id<D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT;range_id++){
    if(
      pCONT_time->CheckBetween_Day_DateTimesShort(
        &relay_status[relay_id].enabled_ranges[range_id].ontime,
        &relay_status[relay_id].enabled_ranges[range_id].offtime
      )
    ){
      isenabled = true;
    }
  }


  return isenabled;

}




/********************************************************************************************/

void mRelays::SetLatchingRelay(power_t lpower, uint32_t state)
{
  // power xx00 - toggle REL1 (Off) and REL3 (Off) - device 1 Off, device 2 Off
  // power xx01 - toggle REL2 (On)  and REL3 (Off) - device 1 On,  device 2 Off
  // power xx10 - toggle REL1 (Off) and REL4 (On)  - device 1 Off, device 2 On
  // power xx11 - toggle REL2 (On)  and REL4 (On)  - device 1 On,  device 2 On

  if (state && !pCONT_set->latching_relay_pulse) {  // Set latching relay to power if previous pulse has finished
    latching_power = lpower;
    pCONT_set->latching_relay_pulse = 2;            // max 200mS (initiated by stateloop())
  }

  for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
    uint32_t port = (i << 1) + ((latching_power >> i) &1);
    pCONT_pins->DigitalWrite(GPIO_REL1_ID + port, bitRead(rel_inverted, port) ? !state : state);
  }
}

void mRelays::SetDevicePower(power_t rpower, uint32_t source)
{

  DEBUG_LINE;

  pCONT_sup->ShowSource(source);
  pCONT_set->last_source = source;
  DEBUG_LINE;
  
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS "SetDevicePower(%d,%d)"),rpower,source);

  if (POWER_ALL_ALWAYS_ON == pCONT_set->Settings.poweronstate) {  // All on and stay on
    pCONT_set->power = (1 << pCONT_set->devices_present);// -1;
    rpower = pCONT_set->power;
  }

  // Allow only one or no relay set - CMND_INTERLOCK - Enable/disable interlock
  // if (pCONT_set->Settings.flag_system.interlock) {        
  //   for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
  //     power_t mask = 1;
  //     uint32_t count = 0;
  //     for (uint32_t j = 0; j < pCONT_set->devices_present; j++) {
  //       if ((pCONT_set->Settings.interlock[i] & mask) && (rpower & mask)) {
  //         count++;
  //       }
  //       mask <<= 1;
  //     }
  //     if (count > 1) {
  //       mask = ~pCONT_set->Settings.interlock[i];    // Turn interlocked group off as there would be multiple relays on
  //       power &= mask;
  //       rpower &= mask;
  //     }
  //   }
  // }
DEBUG_LINE;
  
  if (rpower) {                           // Any power set
    last_power = rpower;
  }

  // PHASE OUT and replace with something else
  // pCONT_set->XdrvMailbox.index = rpower;
  // pCONT->Tasker_Interface(FUNC_SET_POWER);               // Signal power state
  // pCONT_set->XdrvMailbox.index = rpower;
  // pCONT_set->XdrvMailbox.payload = source;

  // if (pCONT->Tasker_Interface(FUNC_SET_DEVICE_POWER)) {  // Set power state and stop if serviced
  //   // Serviced
  // }
  // else if ((MODULE_SONOFF_DUAL == pCONT_set->my_module_type) || (MODULE_CH4 == pCONT_set->my_module_type)) {
  //   Serial.write(0xA0);
  //   Serial.write(0x04);
  //   Serial.write(rpower &0xFF);
  //   Serial.write(0xA1);
  //   Serial.write('\n');
  //   Serial.flush();
  // }
  // else if (MODULE_EXS_RELAY == pCONT_set->my_module_type) {
  //   SetLatchingRelay(rpower, 1);
  // }
  // else {

// #ifdef USE_NETWORK_MDNS
//     #ifdef USE_VIRTUAL_REMOTE_URL_RELAY

//       char remote_url[100];
//       // URL
//       // sprintf(remote_url, "http://%s/json_command.json", VIRTUAL_DEVICE_MDNS_NAME);
//       //MQTT
//       sprintf(remote_url, "%s/set/relays", VIRTUAL_DEVICE_MDNS_NAME);

//       power_t state = rpower &1;
//       uint8_t state_level = bitRead(rel_inverted, 0) ? !state : state;

//       // State  is controlled remotely, so we can only toggle, until I use HTTP_GET to remain in sync
//       char remote_command[100];
//       sprintf(remote_command,"{\"relay\":0,\"onoff\":2}");//,state_level);

//       AddLog(LOG_LEVEL_INFO, PSTR("Sending USE_VIRTUAL_REMOTE_URL_RELAY"));

//       pCONT_mqtt->ppublish(remote_url,remote_command,false);

//       // AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html);
//       // response->addHeader("Server","ESP Async Web Server");
//       // request->send(response);

//       return; // not local control

// #endif // #ifdef USE_NETWORK_MDNS
    // #endif

    uint16_t gpio_pin = 0;

    for (uint32_t i = 0; i < pCONT_set->devices_present; i++) {
      power_t state = rpower &1;
      if (i < MAX_RELAYS) {

//        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_RELAYS "i=%d,state=%d"),i,state);

        //tmp fix
        if(bitRead(rel_inverted, i))
        { //add the gpio mpin shift back in
          gpio_pin = GPIO_REL1_INV_ID;          
        }else{
          gpio_pin = GPIO_REL1_ID;
        }

        pCONT_pins->DigitalWrite(gpio_pin +i, bitRead(rel_inverted, i) ? !state : state);

        // pCONT_pins->DigitalWrite(GPIO_REL1_ID +i, bitRead(rel_inverted, i) ? !state : state);


      }else{
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_RELAYS "ELSE i=%d,state=%d"),i,state);
      }
      rpower >>= 1;
    }
  //}
}

void mRelays::RestorePower(bool publish_power, uint32_t source)
{
  if (pCONT_set->power != last_power) {
    SetDevicePower(last_power, source);
    if (publish_power) {
      mqtthandler_state_teleperiod.flags.SendNow = true;
      mqtthandler_state_ifchanged.flags.SendNow = true;
    }
  }
}


void mRelays::SetAllPower(uint32_t state, uint32_t source)
{
  // state 0 = POWER_OFF = Relay Off
  // state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
  // state 2 = POWER_TOGGLE = Toggle relay
  // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
  // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
  // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
  // state 16 = POWER_SHOW_STATE = Show power state

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                           // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }
  if ((state >= POWER_OFF) && (state <= POWER_TOGGLE)) {
    power_t all_on = (1 << pCONT_set->devices_present);// -1;
    switch (state) {
    case POWER_OFF:
      pCONT_set->power = 0;
      break;
    case POWER_ON:
      pCONT_set->power = all_on;
      break;
    case POWER_TOGGLE:
      pCONT_set->power ^= all_on;                    // Complement current state
    }
    SetDevicePower(pCONT_set->power, source);
  }
  if (publish_power) {
    mqtthandler_state_teleperiod.flags.SendNow = true;
    mqtthandler_state_ifchanged.flags.SendNow = true;
  }
}

void mRelays::SetPowerOnState(void)
{
  
  if (POWER_ALL_ALWAYS_ON == pCONT_set->Settings.poweronstate) {
    SetDevicePower(1, SRC_RESTART);
  } else {
    
    if (
      (pCONT_sup->ResetReason() == REASON_DEFAULT_RST) || 
      (pCONT_sup->ResetReason() == REASON_EXT_SYS_RST)
    ){

      switch (pCONT_set->Settings.poweronstate)
      {
      case POWER_ALL_OFF:
      case POWER_ALL_OFF_PULSETIME_ON:
        pCONT_set->power = 0;
        SetDevicePower(pCONT_set->power, SRC_RESTART);
        break;
      case POWER_ALL_ON:  // All on
        pCONT_set->power = (1 << pCONT_set->devices_present);// -1;
        SetDevicePower(pCONT_set->power, SRC_RESTART);
        break;
      case POWER_ALL_SAVED_TOGGLE:
        pCONT_set->power = (pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) )) ^ POWER_MASK;
        if (pCONT_set->Settings.flag_system.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(pCONT_set->power, SRC_RESTART);
        }
        break;
      case POWER_ALL_SAVED:
        pCONT_set->power = pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) );
        if (pCONT_set->Settings.flag_system.save_state) {  // SetOption0 - Save power state and use after restart
          SetDevicePower(pCONT_set->power, SRC_RESTART);
        }
        break;
      }

    } else {
      pCONT_set->power = pCONT_set->Settings.power & ((1 << pCONT_set->devices_present) );
      if (pCONT_set->Settings.flag_system.save_state) {    // SetOption0 - Save power state and use after restart
        SetDevicePower(pCONT_set->power, SRC_RESTART);
      }
    }
    
  }

  blink_powersave = pCONT_set->power;
}


void mRelays::ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{

/**
 * Adding locking time based method in here, so if set, relays will only turn on during set windows. 
 * Note: Allow the off command without time limits
 * */




// device  = Relay number 1 and up
// state 0 = POWER_OFF = Relay Off
// state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
// state 2 = POWER_TOGGLE = Toggle relay
// state 3 = POWER_BLINK = Blink relay
// state 4 = POWER_BLINK_STOP = Stop blinking relay
// state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
// state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
// state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
// state 16 = POWER_SHOW_STATE = Show power state

  // pCONT_sup->ShowSource(source);

// #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
//   if (IsModuleIfan()) {
//     blink_mask &= 1;                 // No blinking on the fan relays
//     Settings.flag_system.interlock = 0;     // No interlock mode as it is already done by the microcontroller - CMND_INTERLOCK - Enable/disable interlock
//     Settings.pulse_timer[1] = 0;     // No pulsetimers on the fan relays
//     Settings.pulse_timer[2] = 0;
//     Settings.pulse_timer[3] = 0;
//   }
// #endif  // USE_MODULE_CONTROLLER_SONOFF_IFAN

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_RELAYS "ExecuteComPow(device%d,state%d,source%d)=devices_present%d"),device,state,source,pCONT_set->devices_present);

  bool publish_power = true;
  if ((state >= POWER_OFF_NO_STATE) && (state <= POWER_TOGGLE_NO_STATE)) {
    state &= 3;                      // POWER_OFF, POWER_ON or POWER_TOGGLE
    publish_power = false;
  }

  if (
    // (device < 1) || 
  (device > pCONT_set->devices_present)) {
    device = 0;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_RELAYS DEBUG_INSERT_PAGE_BREAK "device>1\tfall back to single relay"));
  }
  active_device = device;

  // if (device <= MAX_PULSETIMERS) {
  //   SetPulseTimer(device, 0);
  // }
  // power_t mask = 1 << (device);        // Device to control

  // Indexing is now from 0!!
  power_t mask = 1 << device;        // Device to control
  if (state <= POWER_TOGGLE)
  {
    // if ((blink_mask & mask)) {
    //   blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
    //   MqttPublishPowerBlinkState(device);
    // }
    #ifdef USE_RELAY_INTERLOCKS
    // if (Settings.flag_system.interlock &&        // CMND_INTERLOCK - Enable/disable interlock
    //     !interlock_mutex &&
    //     ((POWER_ON == state) || ((POWER_TOGGLE == state) && !(power & mask)))
    //    ) {
    //   interlock_mutex = true;                           // Clear all but masked relay in interlock group if new set requested
    //   for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
    //     if (Settings.interlock[i] & mask) {             // Find interlock group
    //       for (uint32_t j = 0; j < devices_present; j++) {
    //         power_t imask = 1 << j;
    //         if ((Settings.interlock[i] & imask) && (power & imask) && (mask != imask)) {
    //           ExecuteCommandPower(j +1, POWER_OFF, SRC_IGNORE);
    //           delay(50);                                // Add some delay to make sure never have more than one relay on
    //         }
    //       }
    //       break;                                        // An interlocked relay is only present in one group so quit
    //     }
    //   }
    //   interlock_mutex = false;
    // }
    #endif

    switch (state) {
    case POWER_OFF: {
      pCONT_set->power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      pCONT_set->power |= mask;
      break;
    case POWER_TOGGLE: // STATE_NUMBER_TOGGLE_ID
      pCONT_set->power ^= mask;
    }
    SetDevicePower(pCONT_set->power, source);

    // if (device <= MAX_PULSETIMERS) {  // Restart PulseTime if powered On
    //   SetPulseTimer(device , (((POWER_ALL_OFF_PULSETIME_ON == Settings.poweronstate) ? ~power : power) & mask) ? Settings.pulse_timer[device -1] : 0);
    // }
  }
  // else if (POWER_BLINK == state) {
  //   if (!(blink_mask & mask)) {
  //     blink_powersave = (blink_powersave & (POWER_MASK ^ mask)) | (power & mask);  // Save state
  //     blink_power = (power >> (device ))&1;  // Prep to Toggle
  //   }
  //   blink_timer = millis() + 100;
  //   blink_counter = ((!Settings.blinkcount) ? 64000 : (Settings.blinkcount *2)) +1;
  //   blink_mask |= mask;  // Set device mask
  //   MqttPublishPowerBlinkState(device);
  //   return;
  // }
  // else if (POWER_BLINK_STOP == state) {
  //   bool flag = (blink_mask & mask);
  //   blink_mask &= (POWER_MASK ^ mask);  // Clear device mask
  //   MqttPublishPowerBlinkState(device);
  //   if (flag) {
  //     ExecuteCommandPower(device, (blink_powersave >> (device ))&1, SRC_IGNORE);  // Restore state
  //   }
  //   return;
  // }

  if (publish_power) {
    mqtthandler_state_teleperiod.flags.SendNow = true;
    mqtthandler_state_ifchanged.flags.SendNow = true;
  }

//AddLog(LOG_LEVEL_TEST, PSTR("mqtthandler_state_teleperiod.flags.SendNow=%d"),mqtthandler_state_teleperiod.flags.SendNow);


}


uint8_t mRelays::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(PM_JSON_DEVICES_CONNECTED, settings.relays_connected);

    JBI->Array_Start_P(PSTR("rel_inverted"));
      for(int8_t bits=0; bits<sizeof(rel_inverted)*8; bits++)
      {
        JBI->Add(bitRead(rel_inverted,bits));
      }
    JBI->Array_End();



  JsonBuilderI->End();

}


uint8_t mRelays::ConstructJSON_State(uint8_t json_level){

  char buffer[100];


  JsonBuilderI->Start();
    for(int device_id=0;device_id<settings.relays_connected;device_id++){
      if(relay_status[device_id].ischanged||(json_level>JSON_LEVEL_IFCHANGED)){ relay_status[device_id].ischanged=false;
        
        JsonBuilderI->Level_Start(GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
        //phase onoff out
          JsonBuilderI->Add_P(PM_JSON_ONOFF,        CommandGet_Relay_Power(device_id));
          JsonBuilderI->Add_P(PM_JSON_ONOFF_NAME,   CommandGet_Relay_Power(device_id)?"ON":"OFF");

          JsonBuilderI->Add_P(PM_JSON_POWER_STATE,        CommandGet_Relay_Power(device_id));
          JsonBuilderI->Add_P(PM_JSON_POWER_STATE_NAME,   CommandGet_Relay_Power(device_id)?"ON":"OFF");
          JsonBuilderI->Add_P(PM_JSON_FRIENDLYNAME, GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
          JBI->Add("TimerDeCounter", CommandGet_SecondsToRemainOn(device_id)); //Phase out
          JBI->Add("SecondsToRemainOn", CommandGet_SecondsToRemainOn(device_id));          
          JBI->Add("SecondsToRemainOff", CommandGet_SecondsRelayHasBeenOn(device_id));


          JsonBuilderI->Level_Start_P(PM_JSON_LAST);
            snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", relay_status[device_id].last.ontime.hour,relay_status[device_id].last.ontime.minute,relay_status[device_id].last.ontime.second);
            JsonBuilderI->Add_P(PM_JSON_ONTIME, buffer);
            snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", relay_status[device_id].last.offtime.hour,relay_status[device_id].last.offtime.minute,relay_status[device_id].last.offtime.second);
            JsonBuilderI->Add_P(PM_JSON_OFFTIME, buffer);
          JsonBuilderI->Level_End();
        
        // Schedules time periods the relays are allowed to run (or should this be locking off?)
        JsonBuilderI->Level_Start_P("RelayEnabled");
        for(int ii=0;ii<D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT;ii++){
          JsonBuilderI->Level_Start_P("Range%d",ii);

            time_short_t t = relay_status[device_id].enabled_ranges[ii].ontime;
            snprintf(buffer, sizeof(buffer), "%02dD%02d:%02d:%02d", t.Wday, t.hour, t.minute, t.second);
            JsonBuilderI->Add_P(PM_JSON_ONTIME, buffer);

            t = relay_status[device_id].enabled_ranges[ii].offtime;
            snprintf(buffer, sizeof(buffer), "%02dD%02d:%02d:%02d", t.Wday, t.hour, t.minute, t.second);
            JsonBuilderI->Add_P(PM_JSON_OFFTIME, buffer);

            // Add if relay is within window etc here
            JBI->Add("IsRelayTimeWindowAllowed", IsRelayTimeWindowAllowed(device_id));

          JsonBuilderI->Level_End();
        }
        JsonBuilderI->Level_End();

        JsonBuilderI->Level_End();
        
      }
    }
  // AddLog(LOG_LEVEL_INFO, PSTR("mRelays::ConstructJSON_State %d"),JBI->GetLength());
  JsonBuilderI->End();

}



uint8_t mRelays::ConstructJSON_Scheduled(uint8_t json_level){

  char buffer[50];

  JsonBuilderI->Start();

  
    for(int device_id=0;device_id<settings.relays_connected;device_id++){
      if(relay_status[device_id].ischanged||(json_level>JSON_LEVEL_IFCHANGED)){ relay_status[device_id].ischanged=false;
        
        JsonBuilderI->Level_Start(GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
          JsonBuilderI->Level_Start("enabled_ranges");

            JsonBuilderI->Array_Start("ontime");
              JsonBuilderI->Add(mTime::ConvertShortTimetoCtr(&relay_status[device_id].enabled_ranges[0].ontime, buffer, sizeof(buffer)));
            JsonBuilderI->Array_End();
            JsonBuilderI->Array_Start("offtime");
              JsonBuilderI->Add(mTime::ConvertShortTimetoCtr(&relay_status[device_id].enabled_ranges[0].offtime, buffer, sizeof(buffer)));
            JsonBuilderI->Array_End();


            JBI->Add("IsRelayTimeWindowAllowed", IsRelayTimeWindowAllowed(device_id));

            //  AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "IsRelayTimeWindowAllowed(num)=%d"), IsRelayTimeWindowAllowed(num));
    




          JsonBuilderI->Level_End();

          // JsonBuilderI->Add_P(PM_JSON_ONOFF,        CommandGet_Relay_Power(device_id));
          // JsonBuilderI->Add_P(PM_JSON_ONOFF_NAME,   CommandGet_Relay_Power(device_id)?"ON":"OFF");
          // JsonBuilderI->Add_P(PM_JSON_FRIENDLYNAME, GetRelayNamebyIDCtr(device_id,buffer,sizeof(buffer)));
          // JsonBuilderI->Level_Start_P(PM_JSON_LAST);
          //   snprintf(buffer, sizeof(buffer), "\"%02d:%02d:%02d\"", relay_status[device_id].last.ontime.hour,relay_status[device_id].last.ontime.minute,relay_status[device_id].last.ontime.second);
          //   JsonBuilderI->Add_P(PM_JSON_ONTIME, buffer);
          //   snprintf(buffer, sizeof(buffer), "\"%02d:%02d:%02d\"", relay_status[device_id].last.offtime.hour,relay_status[device_id].last.offtime.minute,relay_status[device_id].last.offtime.second);
          //   JsonBuilderI->Add_P(PM_JSON_OFFTIME, buffer);
          // JsonBuilderI->Level_End();
        JsonBuilderI->Level_End();
        
      }
    }
  JsonBuilderI->End();

}




#endif
