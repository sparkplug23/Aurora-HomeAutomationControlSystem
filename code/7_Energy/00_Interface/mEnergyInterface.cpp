#include "mEnergyInterface.h" 


#ifdef USE_MODULE_ENERGY_INTERFACE 

int8_t mEnergyInterface::Tasker(uint8_t function, JsonParserObject obj){
  
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

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;
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
    // case TASK_EVENT_SET_POWER_ID:
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
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


   

void mEnergyInterface::Pre_Init(void)
{

}


void mEnergyInterface::Init(void)
{
  settings.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  settings.fEnableSensor = true;
}


void mEnergyInterface::EveryLoop()
{

}


  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
void mEnergyInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;



    
}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/



uint8_t mEnergyInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}

/**
 * @brief 
 * 
 * Format with multiple locations? maybe should be flipped and the name added. ie
 * 
 * {"Temperature":{"Master Bedroom":23.580,"Landing":25.020}}
 * {"Humidity":{name:12,other:13} //therefore "for each key"
 * {"lum"..}
 * 
 * Step 1: Using module pointer, request all sensor readings which will be returned in sensors_reading_t
 * Step 2: Go through all sensors_reading_t values, if configured, then append their values to the JSON array with Key based on the module pointer
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mEnergyInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    // JBI->Add("Redunction", mqtthandler_sensor_ifchanged.flags.FrequencyRedunctionLevel);
  // return 0;
  
  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  uint16_t type_id_adjusted = 0;

  for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      if( IS_MODULE_ENERGY_SUBMODULE( pmod->GetModuleUniqueID() ) )
      {
        //Get any sensors in module
        uint8_t sensors_available = pmod->GetSensorCount();
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
        
        if(sensors_available) 
        {
          // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

          for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
          {
            sensors_reading_t val;
            pmod->GetSensorReading(&val, sensor_id);
            
            if(val.Valid())
            {

              /**
               * @brief Special cases
               * 
               */
              if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              {
                type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;              
              }
              else
              {
                type_id_adjusted = type_id;
              }

              if(val.isFloatWaiting_WithSensorType(type_id_adjusted))
              {
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                // Only add sensor type if any has been found
                if(flag_level_started != true)
                {     
                  JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                  flag_level_started = true;
                  flag_level_ended_needed = true;
                }
                
                sensor_data = val.GetFloat(type_id);
                JBI->Add(buffer, sensor_data);              
                
              }

            }
          }

        }
      } // oNLY USE SENSOR MODULES

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }

  } // END sensor_type


  JBI->Add("Rate", mqtthandler_sensor_ifchanged.tRateSecs);


  return JBI->End();
    
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mEnergyInterface::MQTTHandler_Init(){

  struct handler<mEnergyInterface>* ptr;
 
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__ENERGY_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__ENERGY_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mEnergyInterface::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mEnergyInterface::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->GetTelePeriod();
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->GetIfChangedPeriod();
  }
}


/**
 * @brief MQTTHandler_Sender
 * */
void mEnergyInterface::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // end header guard






// #include "mEnergyInterface.h"

// #ifdef USE_MODULE_ENERGY_INTERFACE


// int8_t mEnergyInterface::Tasker(uint8_t function, JsonParserObject obj)
// {

//   int8_t function_result = 0;
  
//   switch(function){
//     case TASK_PRE_INIT:
//       // Pre_Init();
//     break;
//     case TASK_SETTINGS_DEFAULT:
//       Settings_Default();
//     break;
//     case TASK_JSON_COMMAND_ID:
//       parse_JSONCommand_BootSafe(obj);    // Commands should be moved into here that are required to start the module. Or restructure of parseJSON that has a break if device is not started.
//     break; 
//   }

//   if(!tkr_set->runtime_var.energy_driver){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * INIT SECTION * 
//     *******************/
//     case TASK_INIT:
//       Init();
//     break;
//     /************
//      * SETTINGS SECTION * 
//     *******************/
//     // case TASK_SETTINGS_LOAD_VALUES_INTO_MODULE: 
//     //   Settings_Load();
//     // break;
//     // case TASK_SETTINGS_SAVE_VALUES_FROM_MODULE: 
//     //   Settings_Save();
//     // break;
//     // case TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
//     //   Settings_Default();
//     //   // tkr_set->SettingsSave(2);
//     // break;
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case TASK_LOOP: 
//       // EveryLoop();
//     break;  
//     // case TASK_EVERY_200_MSECOND:
//     //   //Energy200ms();
//     // break;
//     case TASK_EVERY_SECOND:
//       EnergyEverySecond();
//       UpdateThresholdLimits();
//       //  mqtthandler_sensor_ifchanged.flags.SendNow = true;

//     break;
//     case TASK_EVERY_MINUTE:
//       //UpdateEnergyUsagePerMinute();
//     break;
//     case TASK_EVERY_MIDNIGHT:
    
//     break;


//     case TASK_SENSOR_UPDATED:

// //recalculate energy

//     break;


//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     case TASK_JSON_COMMAND_ID:
//       parse_JSONCommand(obj);
//     break; 
//     /************
//      * WEBPAGE SECTION * 
//     *******************/
//     // #ifdef USE_MODULE_NETWORK_WEBSERVER
//     // case TASK_WEB_ADD_ROOT_TABLE_ROWS:
//     //   WebAppend_Root_Draw_Table();
//     // break;
//     // case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
//     //   WebAppend_Root_Status_Table();
//     // break;
//     // #endif //USE_MODULE_NETWORK_WEBSERVER
//     /************
//      * MQTT SECTION * 
//     *******************/
//     // #ifdef USE_MODULE_NETWORK_MQTT
//     // case TASK_MQTT_HANDLERS_INIT:
//     //   MQTTHandler_Init();
//     // break;
//     // case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
//     //   MQTTHandler_Rate();
//     // break;
//     // case TASK_MQTT_SENDER:
//     //   // MQTTHandler_Sender();
//     // break;
//     // #endif //USE_MODULE_NETWORK_MQTT
//   }
  
//   return function_result;

// }








// void mEnergyInterface::Init(void)
// {


// // This should really be completely dynamic, but right now size is forced
// for(int i=0;i<MAX_ENERGY_SENSORS;i++){
//   Energy.voltage[i] = 0;
//   Energy.current[i] = 0;
//   Energy.active_power[i] = 0;
//   Energy.apparent_power[i] = 0;
//   Energy.reactive_power[i] = 0;
//   Energy.power_factor[i] = 0;
//   Energy.frequency[i] = 0;
//   Energy.energy_read[i] = 0;
// }




//   if (tkr_set->runtime.energy_driver) {
//     // // if (RtcSettingsValid()) {
//     //   Energy.kWhtoday_offset = tkr_set->RtcSettings.energy_kWhtoday;
//     // // }
//     // else if (RtcTime.day_of_year == Settings.energy_kWhdoy) {
//     //   Energy.kWhtoday_offset = Settings.energy_kWhtoday;
//     // }
//     // else {
//       Energy.kWhtoday_offset = 0;
//     // }
//     Energy.kWhtoday = 0;
//     Energy.kWhtoday_delta = 0;
//     Energy.period = Energy.kWhtoday_offset;
//     EnergyUpdateToday();
//     //ticker_energy.attach_ms(200, Energy200ms);
//   }

//   for(uint8_t ii=0;ii<MAX_ENERGY_SENSORS;ii++){

//     // Disable all values
//     parameter_thresholds[ii].flags.data = (uint16_t)0; //disable all
//     // Enable select flags
//     parameter_thresholds[ii].flags.mqtt_report_threshold_transition = 1; //disable all

//     // Voltage
//     parameter_thresholds[ii].voltage.upper_limit = 260;
//     parameter_thresholds[ii].voltage.lower_limit = 210;
//     parameter_thresholds[ii].voltage.over_limit_seconds_counter = 0;
//     parameter_thresholds[ii].voltage.over_limit_seconds_trigger_value = 120;
//     // Current
//     parameter_thresholds[ii].current.upper_limit = 4.0f;
//     parameter_thresholds[ii].current.lower_limit = 1.0f;
//     parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
//     parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 5;
//     // active_power
//     parameter_thresholds[ii].active_power.upper_limit = 260;
//     parameter_thresholds[ii].active_power.lower_limit = 210;
//     parameter_thresholds[ii].active_power.over_limit_seconds_counter = 0;
//     parameter_thresholds[ii].active_power.over_limit_seconds_trigger_value = 120;
//     // Current
//     // parameter_thresholds[ii].current.upper_limit = 260;
//     // parameter_thresholds[ii].current.lower_limit = 210;
//     // parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 120;
//     // frequency
//     parameter_thresholds[ii].frequency.upper_limit = 260;
//     parameter_thresholds[ii].frequency.lower_limit = 210;
//     parameter_thresholds[ii].frequency.over_limit_seconds_counter = 0;
//     parameter_thresholds[ii].frequency.over_limit_seconds_trigger_value = 120;
//     // power_factor
//     parameter_thresholds[ii].power_factor.upper_limit = 260;
//     parameter_thresholds[ii].power_factor.lower_limit = 210;
//     parameter_thresholds[ii].power_factor.over_limit_seconds_counter = 0;
//     parameter_thresholds[ii].power_factor.over_limit_seconds_trigger_value = 120;
//     // energy
//     parameter_thresholds[ii].energy.upper_limit = 260;
//     parameter_thresholds[ii].energy.lower_limit = 210;
//     parameter_thresholds[ii].energy.over_limit_seconds_counter = 0;
//     parameter_thresholds[ii].energy.over_limit_seconds_trigger_value = 120;

//   } // END for

//   //single changes
  
//     parameter_thresholds[0].current.lower_limit = 0;


  
// }


// /**
//  * Init the default values, these will be configured before reading templates
//  * */
// void mEnergyInterface::Settings_Default()
// {
//   Energy.phase_count = 0;


// }



// void mEnergyInterface::SetIDWithAddress(uint8_t address_id, uint8_t address_to_save)//, uint8_t address_length)
// {
  
//   // for(int ii=0;ii<address_length;ii++){
    
//   //   address[address_id][ii] = address_to_save[ii];

//     // address.insert(address_id, 2);//*address_to_save);
//     address.push_back(address_to_save);

//   // }

// }

// uint8_t mEnergyInterface::GetAddressWithID(uint8_t address_id)//, uint8_t* address_to_get, uint8_t address_length)
// {
//   // if(address_id > address.size()-1)
//   // {
//   // return 0;
//   //   }
//   // ALOG_TST(PSTR("address.size() = %d"),address.size());

// #ifdef DEVICE_HVAC_DESK
//   return address_id + 1;
//   #endif

//   if(address.size())
//   {
    
//   // for(int ii=0;ii<address_length;ii++){
//     return address[address_id];
//   //   address[address_id][ii] = address_to_save[ii];
//   }
//     // address.insert(address_id, 2);//*address_to_save);
//     // address.push_back(*address_to_save);

//   ALOG_ERR(PSTR("Not address size"));

// return 0;
//   // }

// }


// void mEnergyInterface::SetEnergyDeviceCount(uint8_t address_length)
// {

//   Energy.phase_count = address_length;

//   ALOG_TST(PSTR("Energy.phase_count=%d"),Energy.phase_count);

// }





// // /********************************************************************************************/

// bool mEnergyInterface::EnergyTariff1Active()  // Off-Peak hours
// {

//   // uint8_t dst = 0;
//   // if (tkr_time->IsDst() && (tkr_set->Settings.energy_usage.tariff[0][1] != tkr_set->Settings.energy_usage.tariff[1][1])) {
//   //   dst = 1;
//   // }
//   // if (tkr_set->Settings.energy_usage.tariff[0][dst] != tkr_set->Settings.energy_usage.tariff[1][dst]) {
//   //   if (tkr_set->Settings.flag_network.energy_weekend && ((
//   //     tkr_time->RtcTime.day_of_week == 1) ||   // CMND_TARIFF
//   //                                         (tkr_time->RtcTime.day_of_week == 7))) {
//   //     return true;
//   //   }
//   //   uint32_t minutes = tkr_time->MinutesPastMidnight();
//   //   if (tkr_set->Settings.energy_usage.tariff[0][dst] > tkr_set->Settings.energy_usage.tariff[1][dst]) {
//   //     // {"Tariff":{"Off-Peak":{"STD":"22:00","DST":"23:00"},"Standard":{"STD":"06:00","DST":"07:00"},"Weekend":"OFF"}}
//   //     return ((minutes >= tkr_set->Settings.energy_usage.tariff[0][dst]) || (minutes < tkr_set->Settings.energy_usage.tariff[1][dst]));
//   //   } else {
//   //     // {"Tariff":{"Off-Peak":{"STD":"00:29","DST":"01:29"},"Standard":{"STD":"07:29","DST":"08:29"},"Weekend":"OFF"}}
//   //     return ((minutes >= tkr_set->Settings.energy_usage.tariff[0][dst]) && (minutes < tkr_set->Settings.energy_usage.tariff[1][dst]));
//   //   }
//   // } else {
//   //   return false;
//   // }
// }

// void mEnergyInterface::EnergyUpdateToday(void)
// {
 
//   //  if (Energy.kWhtoday_delta > 1000) {
//   //   unsigned long delta = Energy.kWhtoday_delta / 1000;
//   //   Energy.kWhtoday_delta -= (delta * 1000);
//   //   Energy.kWhtoday += delta;
//   // }

//   // tkr_set->RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset + Energy.kWhtoday;
//   // Energy.daily = (float)(tkr_set->RtcSettings.energy_kWhtoday) / 100000;
//   // Energy.total = (float)(tkr_set->RtcSettings.energy_kWhtotal + tkr_set->RtcSettings.energy_kWhtoday) / 100000;

//   // //if (tkr_set->RtcTime.valid){ // We calc the difference only if we have a valid RTC time.

//   //   uint32_t energy_diff = (uint32_t)(Energy.total * 100000) - tkr_set->RtcSettings.energy_usage.last_usage_kWhtotal;
//   //   tkr_set->RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 100000);

//   //   uint32_t return_diff = 0;
//   //   if (!isnan(Energy.export_active)) {
//   //     return_diff = (uint32_t)(Energy.export_active * 100000) - tkr_set->RtcSettings.energy_usage.last_return_kWhtotal;
//   //     tkr_set->RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(Energy.export_active * 100000);
//   //   }

//   //   if (EnergyTariff1Active()) {  // Tarrif1 = Off-Peak
//   //     tkr_set->RtcSettings.energy_usage.usage1_kWhtotal += energy_diff;
//   //     tkr_set->RtcSettings.energy_usage.return1_kWhtotal += return_diff;
//   //   } else {
//   //     tkr_set->RtcSettings.energy_usage.usage2_kWhtotal += energy_diff;
//   //     tkr_set->RtcSettings.energy_usage.return2_kWhtotal += return_diff;
//   //   }
//   // //}
  
// }

// void mEnergyInterface::EnergyUpdateTotal(float value, bool kwh)
// {
  
//   char energy_total_chr[FLOATSZ];
//   // pCONT_sup->dtostrfd(value, 4, energy_total_chr);
//   // ALOG_DBG(PSTR("NRG: Energy Total %s %sWh"), energy_total_chr, (kwh) ? "k" : "");

//   uint32_t multiplier = (kwh) ? 100000 : 100;  // kWh or Wh to deca milli Wh

//   if (0 == Energy.start_energy || (value < Energy.start_energy)) {
//     Energy.start_energy = value;  // Init after restart and handle roll-over if any
//   }
//   else if (value != Energy.start_energy) {
//     Energy.kWhtoday = (unsigned long)((value - Energy.start_energy) * multiplier);
//   }

//   // if((Energy.total < (value - 0.01)) &&       // We subtract a little offset to avoid continuous updates
//   //     tkr_set->Settings.flag_network.hardware_energy_total) {  // SetOption72 - Enable hardware energy total counter as reference (#6561)
//   //   tkr_set->RtcSettings.energy_kWhtotal = (unsigned long)((value * multiplier) - Energy.kWhtoday_offset - Energy.kWhtoday);
//   //   tkr_set->Settings.energy_usage.energy_kWhtotal = tkr_set->RtcSettings.energy_kWhtotal;
//   //   Energy.total = (float)(tkr_set->RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
//   //   tkr_set->Settings.energy_usage.energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? tkr_time->LocalTime() : tkr_time->Midnight();
//   //   ALOG_DBG(PSTR("NRG: Energy Total updated with hardware value"));
//   // }
//   EnergyUpdateToday();
  
// }

// /*********************************************************************************************/

// void mEnergyInterface::Energy200ms(void)
// {
  
//   // Energy.power_on = (tkr_set->power != 0) | tkr_set->Settings.flag_system.no_power_on_check;  // SetOption21 - Show voltage even if powered off

//   // Energy.fifth_second++;
//   // if (5 == Energy.fifth_second) {
//   //   Energy.fifth_second = 0;

//   //   //XnrgCall(TASK_ENERGY_EVERY_SECOND);

//   //   if (tkr_set->RtcTime.valid) {
//   //     if (tkr_time->LocalTime() == tkr_time->Midnight()) {
//   //       tkr_set->Settings.energy_usage.energy_kWhyesterday = tkr_set->RtcSettings.energy_kWhtoday;

//   //       tkr_set->RtcSettings.energy_kWhtotal += tkr_set->RtcSettings.energy_kWhtoday;
//   //       tkr_set->Settings.energy_usage.energy_kWhtotal = tkr_set->RtcSettings.energy_kWhtotal;
//   //       Energy.kWhtoday = 0;
//   //       Energy.kWhtoday_offset = 0;
//   //       tkr_set->RtcSettings.energy_kWhtoday = 0;
//   //       Energy.start_energy = 0;

//   //       Energy.kWhtoday_delta = 0;
//   //       Energy.period = Energy.kWhtoday;
//   //       EnergyUpdateToday();
//   //       #if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
//   //         Energy.max_energy_state  = 3;
//   //       #endif  // USE_ENERGY_POWER_LIMIT
//   //     }
//   //     #if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
//   //       if ((RtcTime.hour == Settings.energy_max_energy_start) && (3 == Energy.max_energy_state )) {
//   //         Energy.max_energy_state  = 0;
//   //       }
//   //     #endif  // USE_ENERGY_POWER_LIMIT
//   //   }
//   // }

//   //XnrgCall(TASK_EVERY_200_MSECOND);
// }

// void mEnergyInterface::EnergySaveState(void)
// {
//   // Settings.energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;

//   // Settings.energy_kWhtoday = tkr_set->RtcSettings.energy_kWhtoday;
//   // Settings.energy_kWhtotal = tkr_set->RtcSettings.energy_kWhtotal;

//   // Settings.energy_usage = tkr_set->RtcSettings.energy_usage;
// }

// // #ifdef USE_ENERGY_MARGIN_DETECTION
// // bool mEnergyInterface::EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag){//, bool &save_flag)
// // {
// //   // bool change;

// //   // if (!margin) return false;
// //   // change = save_flag;
// //   // if (type) {
// //   //   flag = (value > margin);
// //   // } else {
// //   //   flag = (value < margin);
// //   // }
// //   // save_flag = flag;
// //   // return (change != save_flag);
// // }

// void mEnergyInterface::EnergyMarginCheck(void)
// {
// //   if (Energy.power_steady_counter) {
// //     Energy.power_steady_counter--;
// //     return;
// //   }

// //   uint16_t energy_power_u = (uint16_t)(Energy.active_power[0]);

// //   if (Settings.energy_power_delta) {
// //     uint16_t delta = abs(Energy.power_history[0] - energy_power_u);
// //     uint16_t min_power = (Energy.power_history[0] > energy_power_u) ? energy_power_u : Energy.power_history[0];

// //     DEBUG_DRIVER_LOG(PSTR("NRG: Delta %d, Power %d"), delta, min_power);

// //     if ((delta > 0) && (min_power > 0)) {  // Fix divide by 0 exception (#6741)
// //       if (((Settings.energy_power_delta < 101) && (((delta * 100) / min_power) > Settings.energy_power_delta)) ||  // 1..100 = Percentage
// //           ((Settings.energy_power_delta > 100) && (delta > (Settings.energy_power_delta -100)))) {                 // 101..32000 = Absolute
// //         Energy.power_delta = true;
// //         Energy.power_history[1] = Energy.active_power[0];  // We only want one report so reset history
// //         Energy.power_history[2] = Energy.active_power[0];
// //       }
// //     }
// //   }
// //   Energy.power_history[0] = Energy.power_history[1];  // Shift in history every second allowing power changes to settle for up to three seconds
// //   Energy.power_history[1] = Energy.power_history[2];
// //   Energy.power_history[2] = energy_power_u;

// //   if (Energy.power_on && (Settings.energy_min_power || Settings.energy_max_power || Settings.energy_min_voltage || Settings.energy_max_voltage || Settings.energy_min_current || Settings.energy_max_current)) {
// //     uint16_t energy_voltage_u = (uint16_t)(Energy.voltage[0]);
// //     uint16_t energy_current_u = (uint16_t)(Energy.current[0] * 1000);

// //     DEBUG_DRIVER_LOG(PSTR("NRG: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);

// //     Response_P(PSTR("{"));
// //     bool flag;
// //     bool jsonflg = false;
// //     if (EnergyMargin(false, Settings.energy_min_power, energy_power_u, flag, Energy.min_power_flag)) {
// //       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_POWERLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
// //       jsonflg = true;
// //     }
// //     if (EnergyMargin(true, Settings.energy_max_power, energy_power_u, flag, Energy.max_power_flag)) {
// //       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_POWERHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
// //       jsonflg = true;
// //     }
// //     if (EnergyMargin(false, Settings.energy_min_voltage, energy_voltage_u, flag, Energy.min_voltage_flag)) {
// //       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_VOLTAGELOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
// //       jsonflg = true;
// //     }
// //     if (EnergyMargin(true, Settings.energy_max_voltage, energy_voltage_u, flag, Energy.max_voltage_flag)) {
// //       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_VOLTAGEHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
// //       jsonflg = true;
// //     }
// //     if (EnergyMargin(false, Settings.energy_min_current, energy_current_u, flag, Energy.min_current_flag)) {
// //       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_CURRENTLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
// //       jsonflg = true;
// //     }
// //     if (EnergyMargin(true, Settings.energy_max_current, energy_current_u, flag, Energy.max_current_flag)) {
// //       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_CURRENTHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
// //       jsonflg = true;
// //     }
// //     if (jsonflg) {
// //       ResponseJsonEnd();
// //       MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_MARGINS), MQTT_TELE_RETAIN);
// //       EnergyMqttShow();
// //     }
// //   }

// // #ifdef USE_ENERGY_POWER_LIMIT
// //   // Max Power
// //   if (Settings.energy_max_power_limit) {
// //     if (Energy.active_power[0] > Settings.energy_max_power_limit) {
// //       if (!Energy.mplh_counter) {
// //         Energy.mplh_counter = Settings.energy_max_power_limit_hold;
// //       } else {
// //         Energy.mplh_counter--;
// //         if (!Energy.mplh_counter) {
// //           ResponseTime_P(PSTR(",\"" D_MAXPOWERREACHED "\":%d}"), energy_power_u);
// //           MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
// //           EnergyMqttShow();
// //           SetAllPower(POWER_ALL_OFF, SRC_MAXPOWER);
// //           if (!Energy.mplr_counter) {
// //             Energy.mplr_counter = Settings.setoption_255[P_MAX_POWER_RETRY] +1;
// //           }
// //           Energy.mplw_counter = Settings.energy_max_power_limit_window;
// //         }
// //       }
// //     }
// //     else if (power && (energy_power_u <= Settings.energy_max_power_limit)) {
// //       Energy.mplh_counter = 0;
// //       Energy.mplr_counter = 0;
// //       Energy.mplw_counter = 0;
// //     }
// //     if (!power) {
// //       if (Energy.mplw_counter) {
// //         Energy.mplw_counter--;
// //       } else {
// //         if (Energy.mplr_counter) {
// //           Energy.mplr_counter--;
// //           if (Energy.mplr_counter) {
// //             ResponseTime_P(PSTR(",\"" D_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
// //             MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_POWERMONITOR));
// //             RestorePower(true, SRC_MAXPOWER);
// //           } else {
// //             ResponseTime_P(PSTR(",\"" D_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
// //             MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
// //             EnergyMqttShow();
// //           }
// //         }
// //       }
// //     }
// //   }

// //   // Max Energy
// //   if (Settings.energy_max_energy) {
// //     uint16_t energy_daily_u = (uint16_t)(Energy.daily * 1000);
// //     if (!Energy.max_energy_state  && (RtcTime.hour == Settings.energy_max_energy_start)) {
// //       Energy.max_energy_state  = 1;
// //       ResponseTime_P(PSTR(",\"" D_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
// //       MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_ENERGYMONITOR));
// //       RestorePower(true, SRC_MAXENERGY);
// //     }
// //     else if ((1 == Energy.max_energy_state ) && (energy_daily_u >= Settings.energy_max_energy)) {
// //       Energy.max_energy_state  = 2;
// //       char stemp[FLOATSZ];
// //       pCONT_sup->dtostrfd(Energy.daily, 3, stemp);
// //       ResponseTime_P(PSTR(",\"" D_MAXENERGYREACHED "\":%s}"), stemp);
// //       MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
// //       EnergyMqttShow();
// //       SetAllPower(POWER_ALL_OFF, SRC_MAXENERGY);
// //     }
// //   }
// // #endif  // USE_ENERGY_POWER_LIMIT

// //   if (Energy.power_delta) { EnergyMqttShow(); }
// }


// void mEnergyInterface::EnergyEverySecond(void)
// {
//   // Overtemp check
//   // if (global_update) {
//   //   if (power && (global_temperature != 9999) && (global_temperature > Settings.setoption_255[P_OVER_TEMP])) {  // Device overtemp, turn off relays
//   //     SetAllPower(POWER_ALL_OFF, SRC_OVERTEMP);
//   //   }
//   // }
    
//   // Invalid data reset
//   uint32_t data_valid = Energy.phase_count;
//   for (uint32_t i = 0; i < Energy.phase_count; i++) {
//     if (Energy.data_valid[i] <= ENERGY_WATCHDOG) {
//       Energy.data_valid[i]++;
//       if (Energy.data_valid[i] > ENERGY_WATCHDOG) {
//         // Reset energy registers
//         Energy.voltage[i] = 0;
//         Energy.current[i] = 0;
//         Energy.active_power[i] = 0;
//         if (!isnan(Energy.apparent_power[i])) { Energy.apparent_power[i] = 0; }
//         if (!isnan(Energy.reactive_power[i])) { Energy.reactive_power[i] = 0; }
//         if (!isnan(Energy.frequency[i]))      { Energy.frequency[i] = 0; }
//         if (!isnan(Energy.power_factor[i]))   { Energy.power_factor[i] = 0; }
//         data_valid--;
//       }
//     }
//   }
//   if (!data_valid) {
//     if (!isnan(Energy.export_active)) { Energy.export_active = 0; }
//     Energy.start_energy = 0;
//     pCONT->Tasker_Interface(TASK_ENERGY_RESET);
//   }

//   // #ifdef USE_ENERGY_MARGIN_DETECTION
//   //   EnergyMarginCheck();
//   // #endif  // USE_ENERGY_MARGIN_DETECTION
// }




// void mEnergyInterface::UpdateEnergyUsagePerMinute(){

//   // needs to add guard to wait on valid previous data

//   // Get index values checking for loop around
//   uint8_t index = Energy.stats.kwh_per_minute_index;
//   uint8_t last_index = (index==0)?59:index-1;

//   AddLog(LOG_LEVEL_DEV_TEST,PSTR("index=%d,last_index=%d"),index,last_index);

//   // Get new power last minute
//   Energy.stats.last_minutes_energy = Energy.stats.kwh_per_minute[last_index];
//   // Get new power
//   // Energy.stats.current_energy = pCONT_pzem->PzemAc.last_energy;
//   //this minutes usage
//   Energy.stats.this_minutes_energy = Energy.stats.current_energy - Energy.stats.last_minutes_energy;
//   //store into array
//   // Energy.stats.kwh_each_minute[index] = pCONT_pzem->PzemAc.last_energy;
//   Energy.stats.kwh_per_minute[index] = Energy.stats.this_minutes_energy;
  
//   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_COMMAND_NVALUE),"last_minutes_energy",(int)Energy.stats.last_minutes_energy);
//   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_COMMAND_NVALUE),"current_energy",(int)Energy.stats.current_energy);
//   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_COMMAND_NVALUE),"this_minutes_energy",(int)Energy.stats.this_minutes_energy);

//   //increment index and save
//   Energy.stats.kwh_per_minute_index = (index+1);

//   if(Energy.stats.kwh_per_minute_index>59){
//     Energy.stats.kwh_per_minute_index = 0;
//   }

//   // mqtthandler_sensor_ifchanged.flags.SendNow = true;

// }



// void mEnergyInterface::Settings_Load(){

//   // name_buffer = tkr_set->Settings.energy_usage.name_buffer;

// }


// void mEnergyInterface::Settings_Save(){

//   // tkr_set->Settings.energy_usage.name_buffer = name_buffer;

// }


// void mEnergyInterface::UpdateThresholdLimits(){



//   for(uint8_t ii=0;ii<MAX_ENERGY_SENSORS;ii++){

//     // Current
//     if(!IsWithinLimits(parameter_thresholds[ii].current.lower_limit, Energy.current[ii], parameter_thresholds[ii].current.upper_limit)){
            
//       parameter_thresholds[ii].current.over_limit_seconds_counter++; // add time over boundaries

//       if(parameter_thresholds[ii].current.over_limit_seconds_counter > parameter_thresholds[ii].current.over_limit_seconds_trigger_value){
//         parameter_thresholds[ii].current.over_limit_seconds_trigger_value_exceeded = true;
//       }
//     }else{
//       parameter_thresholds[ii].current.over_limit_seconds_counter = 0; // reset counter
//       parameter_thresholds[ii].current.over_limit_seconds_trigger_value_exceeded = false;
//     }

//   } // END for




// }









// /******************************************************************************************************************
//  * Commands
// *******************************************************************************************************************/

// // /*********************************************************************************************\
// //  * Energy
// // \*********************************************************************************************/

// // // #define XDRV_03                3
// // // #define XSNS_03                3

// // //#define USE_ENERGY_MARGIN_DETECTION
// //  #define USE_ENERGY_POWER_LIMIT


// // enum EnergyCommands {
// //   CMND_POWERCAL, CMND_VOLTAGECAL, CMND_CURRENTCAL,
// //   CMND_POWERSET, CMND_VOLTAGESET, CMND_CURRENTSET, CMND_FREQUENCYSET, CMND_MODULEADDRESS };


// // Integrate these with my code ie
// // ./set/energy/<kEnergyCommands> list, then use ID to call function (eventually make it function handler.. maybe try this now?)

// // const char kEnergyCommands[] PROGMEM = "|"  // No prefix
// //   D_POWERCAL "|" D_VOLTAGECAL "|" D_CURRENTCAL "|"
// //   D_POWERSET "|" D_VOLTAGESET "|" D_CURRENTSET "|" D_FREQUENCYSET "|" D_MODULEADDRESS "|"
// // // #ifdef USE_ENERGY_MARGIN_DETECTION
// //   D_POWERDELTA "|" D_POWERLOW "|" D_POWERHIGH "|" D_VOLTAGELOW "|" D_VOLTAGEHIGH "|" D_CURRENTLOW "|" D_CURRENTHIGH "|"
// // // #ifdef USE_ENERGY_POWER_LIMIT
// //   D_MAXENERGY "|" D_MAXENERGYSTART "|"
// //   D_MAXPOWER "|" D_MAXPOWERHOLD "|" D_MAXPOWERWINDOW "|"
// //   D_SAFEPOWER "|" D_SAFEPOWERHOLD "|"  D_SAFEPOWERWINDOW "|"
// // // #endif  // USE_ENERGY_POWER_LIMIT
// // // #endif  // USE_ENERGY_MARGIN_DETECTION
// //   D_ENERGYRESET "|" D_TARIFF ;

// // // void (* const EnergyCommand[])(void) PROGMEM = {
// // //   &CmndPowerCal, &CmndVoltageCal, &CmndCurrentCal,
// // //   &CmndPowerSet, &CmndVoltageSet, &CmndCurrentSet, &CmndFrequencySet, &CmndModuleAddress,
// // // #ifdef USE_ENERGY_MARGIN_DETECTION
// // //   &CmndPowerDelta, &CmndPowerLow, &CmndPowerHigh, &CmndVoltageLow, &CmndVoltageHigh, &CmndCurrentLow, &CmndCurrentHigh,
// // // #ifdef USE_ENERGY_POWER_LIMIT
// // //   &CmndMaxEnergy, &CmndMaxEnergyStart,
// // //   &CmndMaxPower, &CmndMaxPowerHold, &CmndMaxPowerWindow,
// // //   &CmndSafePower, &CmndSafePowerHold, &CmndSafePowerWindow,
// // // #endif  // USE_ENERGY_POWER_LIMIT
// // // #endif  // USE_ENERGY_MARGIN_DETECTION
// // //   &CmndEnergyReset, &CmndTariff };


// // void mEnergyInterface::parse_JSONCommand(JsonParserObject obj)
// // {

// //   JsonParserToken jtok = 0; 
// //   int8_t tmp_id = 0;

// // 	// if(jtok = obj["MQTT"].getObject()["ScanSensors"])
// // 	// {
// // 	// 	JBI->Start();

// // 	// 	pCONT->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

// // 	// 	bool ready_to_send = JBI->End();

// // 	// 	if(!ready_to_send)
// // 	// 	{
// // 	// 		// Nothing was found, create new message
// // 	// 		JBI->Start();
// // 	// 			JBI->Add("SensorScan", "No Devices Found");
// // 	// 		ready_to_send = JBI->End();
// // 	// 	}


// // 	// 	if(ready_to_send)
// // 	// 	{			
// //     // 	ALOG_TST(PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
// // 	// 		pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
// // 	// 	}

// // 	// }



    
// // }



// /**
//  * @brief "BootSafe" means these are commands that may be called before init
//  * Phase out, this should instead invoke a delayed pre_init/init phase. I need to work out how to safely call init/pri-init during runtime (ie need checks for when things are not preconfigured)
//  * */
// void mEnergyInterface::parse_JSONCommand_BootSafe(JsonParserObject obj)
// {

//   ALOG_TST(PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand_BootSafe"));
//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;

//   // #ifndef  DEVICE_HVAC_DESK
//   // Using a desired address, the sensor is searched for, then index (id) is updated
//   if(jtok = obj["Energy"].getObject()["DeviceCount"])
//   {
//     SetEnergyDeviceCount(jtok.getInt());
//   }
//   // #else
//   // Energy.phase_count = MAX_ENERGY_SENSORS;

//   // #endif 

//   ALOG_TST(PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand_BootSafe"));
// }


// void mEnergyInterface::parse_JSONCommand(JsonParserObject obj){

//     // ALOG_TST(PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand"));
//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;

//   // Using a desired address, the sensor is searched for, then index (id) is updated
//   if(jtok = obj[PM_SENSORADDRESS].getObject()[D_MODULE_ENERGY_INTERFACE_CTR]){
     
//     JsonParserArray array_group = obj[PM_SENSORADDRESS].getObject()[D_MODULE_ENERGY_INTERFACE_CTR];
      
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), F(D_SENSORADDRESS),array_group.size()); 
//     #endif // LOG_LEVEL_COMMANDS
    
//     uint8_t address_temp[8];
//     uint8_t address_index = 0;
//     uint8_t original_device_id = 0;

//     uint8_t device_count = array_group.size();
//       // SetEnergyDeviceCount(device_count);
    
//     for(auto group_iter : array_group) {

//       // JsonParserArray array_sensor_address_iter = group_iter;
//       // memset(address_temp,0,sizeof(address_temp));
//       // address_index = 0;            
//       // for(auto address_id : array_sensor_address_iter) {
//       //   int address = address_id.getInt();
//       //   // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
//       //   //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
//       //   // #endif
//       //   address_temp[address_index++] = address;
//       //   // if(address_index>7){ break; } //error!
//       // }
//       // AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)4);
//       // AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)4);
//       ALOG_TST(PSTR(" group_iter.getInt()) = %d"),  group_iter.getInt());
//       SetIDWithAddress(original_device_id++, group_iter.getInt());//, address_index);

//     }
     
//   }

//   // else{
    
//   //     ALOG_TST(PSTR(" NOT FOUND group_iter.getInt()) = %d"));
//   // }

//   if(jtok = obj["MQTT_Interface_Priority"].getObject()[D_MODULE_ENERGY_INTERFACE_CTR])
//   {
//     CommandSet_MQTT_Reporting_Priority(jtok.getInt());
//   }




// }


// void mEnergyInterface::CommandSet_MQTT_Reporting_Priority(uint8_t value)
// {
  
//   tkr_set->Settings.mqtt.interface_reporting_priority.energy = value;

//   // if(tkr_set->Settings.mqtt.interface_reporting_priority.energy == 1)
//   // {
//   //   mqtt
//   // }

// }

// // void mEnergyInterface::parse_JSONCommand(JsonParserObject obj){

// //   JsonParserToken jtok = 0; 
// //   int8_t tmp_id = 0;

// //   // Using a desired address, the sensor is searched for, then index (id) is updated
// //   // if(jtok = obj["Set"].getObject()[""]){
     
// //   //   JsonParserArray array_group = obj[PM_SENSORADDRESS].getObject()[D_MODULE_ENERGY_PZEM004T_CTR];
      
// //   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), F(D_SENSORADDRESS),array_group.size()); 
// //   //   #endif // LOG_LEVEL_COMMANDS
    
// //   //   uint8_t address_temp[8];
// //   //   uint8_t address_index = 0;
// //   //   uint8_t original_device_id = 0;
    
// //   //   for(auto group_iter : array_group) {

// //   //     JsonParserArray array_sensor_address_iter = group_iter;
// //   //     memset(address_temp,0,sizeof(address_temp));
// //   //     address_index = 0;            
// //   //     for(auto address_id : array_sensor_address_iter) {
// //   //       int address = address_id.getInt();
// //   //       // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
// //   //       //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
// //   //       // #endif
// //   //       address_temp[address_index++] = address;
// //   //       // if(address_index>7){ break; } //error!
// //   //     }
// //   //     AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)4);
// //   //     SetIDWithAddress(original_device_id++, address_temp, address_index);

// //   //   }
     
// //   // }

// // }

// /*
// #ifdef ENABLE_POWER_MONITORING

// uint8_t mTelemetry::ConstructJSON_PowerMonitor(uint8_t json_level){ 

//   // // clear entire mqtt packet
//   // D_DATA_BUFFER_CLEAR();


//   // root[F("tbd")] = 0;

//   // D_STATUS D_STATUS9_MARGIN "\":{\"" D_POWERDELTA "\":%d,\"" D_POWERLOW "\":%d,\"" D_POWERHIGH "\":%d,\""
//   //                           D_VOLTAGELOW "\":%d,\"" D_VOLTAGEHIGH "\":%d,\"" D_CURRENTLOW "\":%d,\"" D_CURRENTHIGH "\":%d}}"),
//   //                           Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power,
//   //                           Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
 
//   // data_buffer.payload.len = measureJson(root)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);

//   return 0;
// }
// #endif

// leave energy in its own module, not telemetry here

//   #define D_STATUS8_POWER "PWR"
  
//   #define D_STATUS9_MARGIN "PTH"
// #if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_MARGIN_DETECTION)
//   if (energy_flg) {
//     if ((0 == payload) || (9 == payload)) {
//       Response_P(PSTR("{\"" D_STATUS D_STATUS9_MARGIN "\":{\"" D_POWERDELTA "\":%d,\"" D_POWERLOW "\":%d,\"" D_POWERHIGH "\":%d,\""
//                             D_VOLTAGELOW "\":%d,\"" D_VOLTAGEHIGH "\":%d,\"" D_CURRENTLOW "\":%d,\"" D_CURRENTHIGH "\":%d}}"),
//                             Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power,
//                             Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
//       MqttPublishPrefixTopic_P(option, PSTR(D_STATUS "9"));
//     }
//   }
// #endif  // USE_ENERGY_MARGIN_DETECTION

// */




// // /*********************************************************************************************\
// //  * Commands
// // \*********************************************************************************************/

// // void mEnergyInterface::EnergyCommandCalResponse(uint32_t nvalue)
// // {
// //   // snprintf_P(XdrvMailbox.command, CMDSZ, PSTR("%sCal"), XdrvMailbox.command);
// //   // ResponseCmndNumber(nvalue);
// // }

// // void mEnergyInterface::CmndEnergyReset(void)
// // {
// // //   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
// // //     char *p;
// // //     unsigned long lnum = strtoul(XdrvMailbox.data, &p, 10);
// // //     if (p != XdrvMailbox.data) {
// // //       switch (XdrvMailbox.index) {
// // //       case 1:
// // //         // Reset Energy Today
// // //         Energy.kWhtoday_offset = lnum *100;
// // //         Energy.kWhtoday = 0;
// // //         Energy.kWhtoday_delta = 0;
// // //         Energy.start_energy = 0;
// // //         Energy.period = Energy.kWhtoday_offset;
// // //         Settings.energy_kWhtoday = Energy.kWhtoday_offset;
// // //         tkr_set->RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset;
// // //         Energy.daily = (float)Energy.kWhtoday_offset / 100000;
// // //         if (!tkr_set->RtcSettings.energy_kWhtotal && !Energy.kWhtoday_offset) {
// // //           Settings.energy_kWhtotal_time = LocalTime();
// // //         }
// // //         break;
// // //       case 2:
// // //         // Reset Energy Yesterday
// // //         Settings.energy_kWhyesterday = lnum *100;
// // //         break;
// // //       case 3:
// // //         // Reset Energy Total
// // //         tkr_set->RtcSettings.energy_kWhtotal = lnum *100;
// // //         Settings.energy_kWhtotal = tkr_set->RtcSettings.energy_kWhtotal;
// // // //        Energy.total = (float)(tkr_set->RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
// // //         Settings.energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? LocalTime() : Midnight();
// // //         tkr_set->RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 1000);
// // //         break;
// // //       }
// // //     }
// // //   }
// // //   else if ((XdrvMailbox.index > 3) && (XdrvMailbox.index <= 5)) {
// // //     char *p;
// // //     char *str = strtok_r(XdrvMailbox.data, ", ", &p);
// // //     int32_t position = -1;
// // //     uint32_t values[2];

// // //     while ((str != nullptr) && (position < 1)) {
// // //       uint32_t value = strtoul(str, nullptr, 10);
// // //       position++;
// // //       values[position] = value *100;
// // //       str = strtok_r(nullptr, ", ", &p);
// // //     }

// // //     switch (XdrvMailbox.index)
// // //     {
// // //       case 4:
// // //         // Reset energy_usage.usage totals
// // //         if (position > -1) {
// // //           tkr_set->RtcSettings.energy_usage.usage1_kWhtotal = values[0];
// // //         }
// // //         if (position > 0) {
// // //           tkr_set->RtcSettings.energy_usage.usage2_kWhtotal = values[1];
// // //         }
// // //         Settings.energy_usage.usage1_kWhtotal = tkr_set->RtcSettings.energy_usage.usage1_kWhtotal;
// // //         Settings.energy_usage.usage2_kWhtotal = tkr_set->RtcSettings.energy_usage.usage2_kWhtotal;
// // //         break;
// // //       case 5:
// // //         // Reset energy_usage.return totals
// // //         if (position > -1) {
// // //           tkr_set->RtcSettings.energy_usage.return1_kWhtotal = values[0];
// // //         }
// // //         if (position > 0) {
// // //           tkr_set->RtcSettings.energy_usage.return2_kWhtotal = values[1];
// // //         }
// // //         Settings.energy_usage.return1_kWhtotal = tkr_set->RtcSettings.energy_usage.return1_kWhtotal;
// // //         Settings.energy_usage.return2_kWhtotal = tkr_set->RtcSettings.energy_usage.return2_kWhtotal;
// // //         break;
// // //       }
// // //   }

// // //   Energy.total = (float)(tkr_set->RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;

// //   char energy_total_chr[FLOATSZ];
// //   pCONT_sup->dtostrfd(Energy.total, tkr_set->Settings.flag_power.energy_resolution, energy_total_chr);
// //   char energy_daily_chr[FLOATSZ];
// //   pCONT_sup->dtostrfd(Energy.daily, tkr_set->Settings.flag_power.energy_resolution, energy_daily_chr);
// //   char energy_yesterday_chr[FLOATSZ];




// // //   pCONT_sup->dtostrfd((float)Settings.energy_kWhyesterday / 100000, tkr_set->Settings.flag_power.energy_resolution, energy_yesterday_chr);

// // //   char energy_usage1_chr[FLOATSZ];
// // //   pCONT_sup->dtostrfd((float)Settings.energy_usage.usage1_kWhtotal / 100000, tkr_set->Settings.flag_power.energy_resolution, energy_usage1_chr);
// // //   char energy_usage2_chr[FLOATSZ];
// // //   pCONT_sup->dtostrfd((float)Settings.energy_usage.usage2_kWhtotal / 100000, tkr_set->Settings.flag_power.energy_resolution, energy_usage2_chr);
// // //   char energy_return1_chr[FLOATSZ];
// // //   pCONT_sup->dtostrfd((float)Settings.energy_usage.return1_kWhtotal / 100000, tkr_set->Settings.flag_power.energy_resolution, energy_return1_chr);
// // //   char energy_return2_chr[FLOATSZ];
// // //   pCONT_sup->dtostrfd((float)Settings.energy_usage.return2_kWhtotal / 100000, tkr_set->Settings.flag_power.energy_resolution, energy_return2_chr);

// // //   Response_P(PSTR("{\"%s\":{\"" D_TOTAL "\":%s,\"" D_YESTERDAY "\":%s,\"" D_TODAY "\":%s,\"" D_USAGE "\":[%s,%s],\"" D_EXPORT "\":[%s,%s]}}"),
// // //     XdrvMailbox.command, energy_total_chr, energy_yesterday_chr, energy_daily_chr, energy_usage1_chr, energy_usage2_chr, energy_return1_chr, energy_return2_chr);
// // }

// // void mEnergyInterface::CmndTariff(void)
// // {
// //   // // Tariff1 22:00,23:00 - Tariff1 start hour for Standard Time and Daylight Savings Time
// //   // // Tariff2 6:00,7:00   - Tariff2 start hour for Standard Time and Daylight Savings Time
// //   // // Tariffx 1320, 1380  = minutes and also 22:003:00
// //   // // Tariffx 223      = hours and also 22:003:00
// //   // // Tariff9 0/1

// //   // if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
// //   //   uint32_t tariff = XdrvMailbox.index -1;
// //   //   uint32_t time_type = 0;
// //   //   char *p;
// //   //   char *str = strtok_r(XdrvMailbox.data, ", ", &p);  // 23:152:30
// //   //   while ((str != nullptr) && (time_type < 2)) {
// //   //     char *q;
// //   //     uint32_t value = strtol(str, &q, 10);            // 23 or 22
// //   //     Settings.tariff[tariff][time_type] = value;
// //   //     if (value < 24) {                                // Below 24 is hours
// //   //       Settings.tariff[tariff][time_type] *= 60;      // Multiply hours by 60 minutes
// //   //       char *minute = strtok_r(nullptr, ":", &q);
// //   //       if (minute) {
// //   //         value = strtol(minute, nullptr, 10);         // 15 or 30
// //   //         if (value > 59) {
// //   //           value = 59;
// //   //         }
// //   //         Settings.tariff[tariff][time_type] += value;
// //   //       }
// //   //     }
// //   //     if (Settings.tariff[tariff][time_type] > 1439) {
// //   //       Settings.tariff[tariff][time_type] = 1439;     // Max is 23:59
// //   //     }
// //   //     str = strtok_r(nullptr, ", ", &p);
// //   //     time_type++;
// //   //   }
// //   // }
// //   // else if (XdrvMailbox.index == 9) {
// //   //   Settings.flag_network.energy_weekend = XdrvMailbox.payload & 1;  // CMND_TARIFF
// //   // }
// //   // Response_P(PSTR("{\"%s\":{\"Off-Peak\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Standard\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Weekend\":\"%s\"}}"),
// //   //   XdrvMailbox.command,
// //   //   GetMinuteTime(Settings.tariff[0][0]).c_str(),GetMinuteTime(Settings.tariff[0][1]).c_str(),
// //   //   GetMinuteTime(Settings.tariff[1][0]).c_str(),GetMinuteTime(Settings.tariff[1][1]).c_str(),
// //   //   GetStateText(Settings.flag_network.energy_weekend));             // CMND_TARIFF
// // }

// // void mEnergyInterface::CmndPowerCal(void)
// // {
// //   // Energy.command_code = CMND_POWERCAL;
// //   // if (XnrgCall(TASK_COMMAND)) {  // microseconds
// //   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
// //   //     Settings.energy_power_calibration = XdrvMailbox.payload;
// //   //   }
// //   //   ResponseCmndNumber(Settings.energy_power_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndVoltageCal(void)
// // {
// //   // Energy.command_code = CMND_VOLTAGECAL;
// //   // if (XnrgCall(TASK_COMMAND)) {  // microseconds
// //   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
// //   //     Settings.energy_voltage_calibration = XdrvMailbox.payload;
// //   //   }
// //   //   ResponseCmndNumber(Settings.energy_voltage_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndCurrentCal(void)
// // {
// //   // Energy.command_code = CMND_CURRENTCAL;
// //   // if (XnrgCall(TASK_COMMAND)) {  // microseconds
// //   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
// //   //     Settings.energy_current_calibration = XdrvMailbox.payload;
// //   //   }
// //   //   ResponseCmndNumber(Settings.energy_current_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndPowerSet(void)
// // {
// //   // Energy.command_code = CMND_POWERSET;
// //   // if (XnrgCall(TASK_COMMAND)) {  // Watt
// //   //   EnergyCommandCalResponse(Settings.energy_power_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndVoltageSet(void)
// // {
// //   // Energy.command_code = CMND_VOLTAGESET;
// //   // if (XnrgCall(TASK_COMMAND)) {  // Volt
// //   //   EnergyCommandCalResponse(Settings.energy_voltage_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndCurrentSet(void)
// // {
// //   // Energy.command_code = CMND_CURRENTSET;
// //   // if (XnrgCall(TASK_COMMAND)) {  // milliAmpere
// //   //   EnergyCommandCalResponse(Settings.energy_current_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndFrequencySet(void)
// // {
// //   // Energy.command_code = CMND_FREQUENCYSET;
// //   // if (XnrgCall(TASK_COMMAND)) {  // Hz
// //   //   EnergyCommandCalResponse(Settings.energy_frequency_calibration);
// //   // }
// // }

// // void mEnergyInterface::CmndModuleAddress(void)
// // {
// //   // if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 4) && (1 == Energy.phase_count)) {
// //   //   Energy.command_code = CMND_MODULEADDRESS;
// //   //   if (XnrgCall(TASK_COMMAND)) {  // Module address
// //   //     ResponseCmndDone();
// //   //   }
// //   // }
// // }

// // // #ifdef USE_ENERGY_MARGIN_DETECTION
// // void mEnergyInterface::CmndPowerDelta(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32000)) {
// //   //   Settings.energy_power_delta = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_power_delta);
// // }

// // void mEnergyInterface::CmndPowerLow(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_min_power = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_min_power);
// // }

// // void mEnergyInterface::CmndPowerHigh(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_power = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power);
// // }

// // void mEnergyInterface::CmndVoltageLow(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
// //   //   Settings.energy_min_voltage = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_min_voltage);
// // }

// // void mEnergyInterface::CmndVoltageHigh(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
// //   //   Settings.energy_max_voltage = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_voltage);
// // }

// // void mEnergyInterface::CmndCurrentLow(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
// //   //   Settings.energy_min_current = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_min_current);
// // }

// // void mEnergyInterface::CmndCurrentHigh(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
// //   //   Settings.energy_max_current = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_current);
// // }

// // // #ifdef USE_ENERGY_POWER_LIMIT
// // void mEnergyInterface::CmndMaxPower(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_power_limit = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power_limit);
// // }

// // void mEnergyInterface::CmndMaxPowerHold(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power_limit_hold);
// // }

// // void mEnergyInterface::CmndMaxPowerWindow(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power_limit_window);
// // }

// // void mEnergyInterface::CmndSafePower(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_power_safe_limit = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power_safe_limit);
// // }

// // void mEnergyInterface::CmndSafePowerHold(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power_safe_limit_hold);
// // }

// // void mEnergyInterface::CmndSafePowerWindow(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
// //   //   Settings.energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_power_safe_limit_window);
// // }

// // void mEnergyInterface::CmndMaxEnergy(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
// //   //   Settings.energy_max_energy = XdrvMailbox.payload;
// //   //   Energy.max_energy_state  = 3;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_energy);
// // }

// // void mEnergyInterface::CmndMaxEnergyStart(void)
// // {
// //   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
// //   //   Settings.energy_max_energy_start = XdrvMailbox.payload;
// //   // }
// //   // ResponseCmndNumber(Settings.energy_max_energy_start);
// // }
// // // #endif  // USE_ENERGY_POWER_LIMIT
// // // #endif  // USE_ENERGY_MARGIN_DETECTION



  
// /******************************************************************************************************************
//  * ConstructJson
// *******************************************************************************************************************/

// uint8_t mEnergyInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

//   // Serial.println("mEnergyInterface::ConstructJSON_Sensor"); Serial.flush();


// // Instead of this way, it should take in via GetSensor like sensor_interface to accumulate all present and future energy devices


//   JBI->Start();

//   JBI->Add(D_CHANNELCOUNT,         Energy.phase_count);
//   JBI->Array_AddArray_F(D_VOLTAGE,              Energy.voltage,       Energy.phase_count);
//   JBI->Array_AddArray_F(D_CURRENT,              Energy.current,       Energy.phase_count);
//   JBI->Array_AddArray_F(D_ACTIVE_POWERUSAGE,    Energy.active_power,  Energy.phase_count);

//   JBI->Add("tRateSecs", mqtthandler_sensor_ifchanged.tRateSecs);


//   // JBI->Array_AddArray_F(D_APPARENT_POWERUSAGE,  Energy.apparent_power,Energy.phase_count);
//   // JBI->Array_AddArray_F(D_REACTIVE_POWERUSAGE,  Energy.reactive_power,Energy.phase_count);
//   // JBI->Array_AddArray_F(D_POWERFACTOR,          Energy.power_factor,  Energy.phase_count);
//   // JBI->Array_AddArray_F(D_FREQUENCY,            Energy.frequency,     Energy.phase_count);
//   // JBI->Add(D_ENERGY,               Energy.energy);
//   // JBI->Add(D_ENERGYLAST,           Energy.last_energy);
  
//   // if(json_method >= JSON_LEVEL_DETAILED){
//   //   JBI->Object_Start(D_KWH_STATS);
//   //     JBI->Add(D_INDEX, Energy.stats.kwh_per_minute_index);
//   //     JBI->Add(D_ENERGY "LastMinute", Energy.stats.last_minutes_energy);
//   //     JBI->Add(D_ENERGY "Currently",  Energy.stats.current_energy);
//   //     JBI->Add(D_ENERGY "ThisMinute", Energy.stats.this_minutes_energy);
//   //     JBI->Array_AddArray_F("kwh_per_minute",           Energy.stats.kwh_per_minute,     sizeof(Energy.stats.kwh_per_minute)/4);
//   //     JBI->Array_AddArray_F("kwh_each_minute",          Energy.stats.kwh_each_minute,    sizeof(Energy.stats.kwh_each_minute)/4);
//   //   JBI->Object_End();
//   // }

//   // Serial.println(JBI->GetBufferPtr()); Serial.flush();
//   // delay(2000);

//   // ALOG_TST(PSTR("mEnergyInterface::ConstructJSON_Sensor"));

//   return JBI->End();

// }


// uint8_t mEnergyInterface::ConstructJSON_ThresholdLimits(uint8_t json_level, bool json_appending){

//   JBI->Start();

//   // Create a new "Array" method that just lets me pass infinite amount of parameters using vsprintf

//     JBI->Object_Start("Current");
    
//       JBI->Array_Start("IsExceeded"); //show value right now
//       for(int ii=0;ii<Energy.phase_count;ii++){
//         JBI->Add(parameter_thresholds[ii].current.over_limit_seconds_trigger_value_exceeded);
//       }
//       JBI->Array_End();

//       // JBI->Array_Start("OverLimitCounter");
//       // for(int ii=0;ii<Energy.phase_count;ii++){
//       //   JBI->Add_FV(PSTR("%d"),parameter_thresholds[ii].current.over_limit_seconds_counter);
//       // }
//       // JBI->Array_End();

//       // JBI->Array_Start("Value"); //show value right now
//       // for(int ii=0;ii<Energy.phase_count;ii++){
//       //   JBI->Add(Energy.current[ii]);
//       // }
//       // JBI->Array_End();

//       // JBI->Array_Start("UpperLimit");
//       // for(int ii=0;ii<Energy.phase_count;ii++){
//       //   JBI->Add(parameter_thresholds[ii].current.upper_limit);
//       // }
//       // JBI->Array_End();

//       // JBI->Array_Start("LowerLimit");
//       // for(int ii=0;ii<Energy.phase_count;ii++){
//       //   JBI->Add(parameter_thresholds[ii].current.lower_limit);
//       // }
//       // JBI->Array_End();

//       // JBI->Array_Start("OverLimitTrigger");
//       // for(int ii=0;ii<Energy.phase_count;ii++){
//       //   JBI->Add_FV(PSTR("%d"),parameter_thresholds[ii].current.over_limit_seconds_trigger_value);
//       // }
//       // JBI->Array_End();

//     JBI->Object_End();


//     // Voltage
//     // parameter_thresholds[ii].voltage.upper_limit = 260;
//     // parameter_thresholds[ii].voltage.lower_limit = 210;
//     // parameter_thresholds[ii].voltage.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].voltage.over_limit_seconds_trigger_value = 120;
//     // Current
//     // parameter_thresholds[ii].current.upper_limit = 260;
//     // parameter_thresholds[ii].current.lower_limit = 210;
//     // parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 120;
//     // // active_power
//     // parameter_thresholds[ii].active_power.upper_limit = 260;
//     // parameter_thresholds[ii].active_power.lower_limit = 210;
//     // parameter_thresholds[ii].active_power.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].active_power.over_limit_seconds_trigger_value = 120;
//     // // Current
//     // parameter_thresholds[ii].current.upper_limit = 260;
//     // parameter_thresholds[ii].current.lower_limit = 210;
//     // parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 120;
//     // // frequency
//     // parameter_thresholds[ii].frequency.upper_limit = 260;
//     // parameter_thresholds[ii].frequency.lower_limit = 210;
//     // parameter_thresholds[ii].frequency.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].frequency.over_limit_seconds_trigger_value = 120;
//     // // power_factor
//     // parameter_thresholds[ii].power_factor.upper_limit = 260;
//     // parameter_thresholds[ii].power_factor.lower_limit = 210;
//     // parameter_thresholds[ii].power_factor.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].power_factor.over_limit_seconds_trigger_value = 120;
//     // // energy
//     // parameter_thresholds[ii].energy.upper_limit = 260;
//     // parameter_thresholds[ii].energy.lower_limit = 210;
//     // parameter_thresholds[ii].energy.over_limit_seconds_counter = 0;
//     // parameter_thresholds[ii].energy.over_limit_seconds_trigger_value = 120;
//   // if(json_method >= JSON_LEVEL_DETAILED){
//   //   JBI->Object_Start(D_KWH_STATS);
//   //     JBI->Add(D_INDEX, Energy.stats.kwh_per_minute_index);
//   //     JBI->Add(D_ENERGY "LastMinute", Energy.stats.last_minutes_energy);
//   //     JBI->Add(D_ENERGY "Currently",  Energy.stats.current_energy);
//   //     JBI->Add(D_ENERGY "ThisMinute", Energy.stats.this_minutes_energy);
//   //     JBI->Array_AddArray_F("kwh_per_minute",           Energy.stats.kwh_per_minute,     sizeof(Energy.stats.kwh_per_minute)/4);
//   //     JBI->Array_AddArray_F("kwh_each_minute",          Energy.stats.kwh_each_minute,    sizeof(Energy.stats.kwh_each_minute)/4);
//   //   JBI->Object_End();
//   // }

//   return JBI->End();

// }
// /*********************************************************************************************************************************************
// ******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// uint8_t mEnergyInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

//   JBI->Start();

//     // JBI->Add(D_CHANNELCOUNT"232",         0);
    
//     JBI->Object_Start("Address");
//     // for(int ii=0;ii<pCONT_iEnergy->Energy.phase_count;ii++)
//     for(int ii=0;ii<address.size();ii++)
//     {
//       JBI->Array_Start_P(PSTR("device%d"), ii);
//         // for(int i=0;i<4;i++)
//         // {
//           JBI->Add(address[ii]);
//         // }
//       JBI->Array_End();
//     }
//     JBI->Object_End();


  
//   return JBI->End();

// }


// uint8_t mEnergyInterface::ConstructJSON_EnergyStats(uint8_t json_level, bool json_appending){

//   JBI->Start();

//     // JBI->Add(D_CHANNELCOUNT"232",         0);
  
//   return JBI->End();
//   // D_DATA_BUFFER_CLEAR();

//   // // StaticJsonDocument<1000> doc;
//   // // JsonObject root = doc.to<JsonObject>();

//   // // uint8_t ischanged=false;

//   // // char channel_ctr[3];
//   // // memset(channel_ctr,0,sizeof(channel_ctr));
//   // // // sprintf(channel_ctr,"%02d",channel);
//   // // //JsonObject energy_total_obj = root.createNestedObject("Energy_Total");

//   // //   JsonObject kwh_per_minute_obj = root.createNestedObject("kwh_stats"); 
    
    
//   // //   kwh_per_minute_obj["index"] = Energy.stats.kwh_per_minute_index;
//   // //   kwh_per_minute_obj["last_minutes_energy"] = Energy.stats.last_minutes_energy;
//   // //   kwh_per_minute_obj["current_energy"] = Energy.stats.current_energy;
//   // //   kwh_per_minute_obj["this_minutes_energy"] = Energy.stats.this_minutes_energy;


//   // // // if(json_method >= JSON_LEVEL_DETAILED){
//   // // //   JsonArray kwh_per_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_per_minute"); 
//   // // //   JsonArray kwh_each_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_each_minute");  
//   // // //   for (uint8_t min = 0; min < 60; min++) {
//   // // //     kwh_per_minute_arr.add(Energy.stats.kwh_per_minute[min]);
//   // // //     kwh_each_minute_arr.add(Energy.stats.kwh_each_minute[min]);
//   // // //   }
//   // // // }

//   // // data_buffer.payload.len = measureJson(root)+1;
//   // // serializeJson(doc,data_buffer.payload.ctr);
//   // // return data_buffer.payload.len>3?true:false;

//   // return 0;
    
// }

// /**
//  * @brief From EnergyShow, its basically my construstjson
//  * 
//  */
//   // if (json) {
//     // bool show_energy_period = (0 == tkr_set->tele_period);

//     // pCONT_sup->ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_TOTAL_START_TIME "\":\"%s\",\"" D_TOTAL "\":%s,\"" D_YESTERDAY "\":%s,\"" D_TODAY "\":%s"),
//     //   tkr_time->GetDateAndTimeCtr(DT_ENERGY),
//     //   EnergyFormatIndex(value_chr, energy_total_chr[0], json, energy_total_fields),
//     //   energy_yesterday_chr,
//     //   energy_daily_chr);

//     // if (!isnan(Energy.export_active)) {
//     //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_EXPORT_ACTIVE "\":%s"),
//     //     EnergyFormatIndex(value_chr, export_active_chr[0], json, energy_total_fields));
//     // }

//     // if (show_energy_period) {
//     //   float energy = 0;
//     //   if (Energy.period) {
//     //     energy = (float)(tkr_set->RtcSettings.energy_kWhtoday - Energy.period) / 100;
//     //   }
//     //   Energy.period = tkr_set->RtcSettings.energy_kWhtoday;
//     //   char energy_period_chr[FLOATSZ];
//     //   pCONT_sup->dtostrfd(energy, tkr_set->Settings.flag_power.wattage_resolution, energy_period_chr);
//     //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_PERIOD "\":%s"), energy_period_chr);
//     // }
//     // pCONT_sup->ResponseAppend_P(PSTR(",\"" D_POWERUSAGE "\":%s"),
//     //   EnergyFormat(value_chr, active_power_chr[0], json));
//     // if (!Energy.type_dc) {
//     //   if (Energy.current_available && Energy.voltage_available) {
//     //     pCONT_sup->ResponseAppend_P(PSTR(",\"" D_APPARENT_POWERUSAGE "\":%s,\"" D_REACTIVE_POWERUSAGE "\":%s,\"" D_POWERFACTOR "\":%s"),
//     //       EnergyFormat(value_chr, apparent_power_chr[0], json),
//     //       EnergyFormat(value2_chr, reactive_power_chr[0], json),
//     //       EnergyFormat(value3_chr, power_factor_chr[0], json));
//     //   }
//     //   if (!isnan(Energy.frequency[0])) {
//     //     pCONT_sup->ResponseAppend_P(PSTR(",\"" D_FREQUENCY "\":%s"),
//     //       EnergyFormat(value_chr, frequency_chr[0], json, Energy.voltage_common));
//     //   }
//     // }
//     // if (Energy.voltage_available) {
//     //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_VOLTAGE "\":%s"),
//     //     EnergyFormat(value_chr, voltage_chr[0], json, Energy.voltage_common));
//     // }
//     // if (Energy.current_available) {
//     //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_CURRENT "\":%s"),
//     //     EnergyFormat(value_chr, current_chr[0], json));
//     // }

//     // AddLog(LOG_LEVEL_DEV_TEST,PSTR(" mEnergyInterface::TASK_JSON_APPEND "));

//     // Tasker(TASK_JSON_APPEND);
//     // pCONT_sup->ResponseJsonEnd();

// // #ifdef USE_DOMOTICZ
// //     if (show_energy_period) {  // Only send if telemetry
// //       pCONT_sup->dtostrfd(Energy.total * 1000, 1, energy_total_chr[0]);
// //       DomoticzSensorPowerEnergy((int)Energy.active_power[0], energy_total_chr[0]);  // PowerUsage, EnergyToday

// //       pCONT_sup->dtostrfd((float)tkr_set->RtcSettings.energy_usage.usage1_kWhtotal / 100, 1, energy_total_chr[1]);  // Tariff1
// //       pCONT_sup->dtostrfd((float)tkr_set->RtcSettings.energy_usage.usage2_kWhtotal / 100, 1, energy_total_chr[2]);  // Tariff2
// //       pCONT_sup->dtostrfd((float)tkr_set->RtcSettings.energy_usage.return1_kWhtotal / 100, 1, export_active_chr[1]);
// //       pCONT_sup->dtostrfd((float)tkr_set->RtcSettings.energy_usage.return2_kWhtotal / 100, 1, export_active_chr[2]);
// //       DomoticzSensorP1SmartMeter(energy_total_chr[1], energy_total_chr[2], export_active_chr[1], export_active_chr[2], (int)Energy.active_power[0]);

// //       if (Energy.voltage_available) {
// //         DomoticzSensor(DZ_VOLTAGE, voltage_chr[0]);  // Voltage
// //       }
// //       if (Energy.current_available) {
// //         DomoticzSensor(DZ_CURRENT, current_chr[0]);  // Current
// //       }
// //     }
// // #endif  // USE_DOMOTICZ
// // #ifdef USE_KNX
// //     if (show_energy_period) {
// //       if (Energy.voltage_available) {
// //         KnxSensor(KNX_ENERGY_VOLTAGE, Energy.voltage[0]);
// //       }
// //       if (Energy.current_available) {
// //         KnxSensor(KNX_ENERGY_CURRENT, Energy.current[0]);
// //       }
// //       KnxSensor(KNX_ENERGY_POWER, Energy.active_power[0]);
// //       if (!Energy.type_dc) {
// //         KnxSensor(KNX_ENERGY_POWERFACTOR, power_factor_knx);
// //       }
// //       KnxSensor(KNX_ENERGY_DAILY, Energy.daily);
// //       KnxSensor(KNX_ENERGY_TOTAL, Energy.total);
// //       KnxSensor(KNX_ENERGY_START, Energy.start_energy);
// //     }
// // #endif  // USE_KNX
// // #ifdef USE_MODULE_NETWORK_WEBSERVER
// //   // } else {

// // /**
// //  * @brief 
// //  * Part of tas that generate json when needed, I will make this its own section
// //  * 
// //  */

// //       if (Energy.voltage_available) {
// //         pCONT_web->WSContentSend_PD(HTTP_SNS_VOLTAGE, 
// //           EnergyFormat(value_chr, voltage_chr[0], json, Energy.voltage_common)
// //         );
// //       }
// //       if (Energy.current_available) {
// //         pCONT_web->WSContentSend_PD(HTTP_SNS_CURRENT, 
// //           EnergyFormat(value_chr, current_chr[0], json)
// //         );
// //       }
// //       pCONT_web->WSContentSend_PD(HTTP_SNS_POWER, EnergyFormat(value_chr, active_power_chr[0], json));
// //       if (!Energy.type_dc) {
// //         if (Energy.current_available && Energy.voltage_available) {
// //           pCONT_web->WSContentSend_PD(HTTP_ENERGY_SNS1, 
// //             EnergyFormat(value_chr, apparent_power_chr[0], json),
// //             EnergyFormat(value2_chr, reactive_power_chr[0], json),
// //             EnergyFormat(value3_chr, power_factor_chr[0], json)
// //           );
// //         }
// //         if (!isnan(Energy.frequency[0])) { 
// //           pCONT_web->WSContentSend_PD(PSTR("{s}" D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"),
// //             EnergyFormat(value_chr, frequency_chr[0], json, Energy.voltage_common)
// //           );
// //         }
// //       }
// //       pCONT_web->WSContentSend_PD(HTTP_ENERGY_SNS2, energy_daily_chr, energy_yesterday_chr, energy_total_chr[0]);
// //       if (!isnan(Energy.export_active)) {
// //         pCONT_web->WSContentSend_PD(HTTP_ENERGY_SNS3, export_active_chr[0]);
// //       }
// // #endif  // USE_MODULE_NETWORK_WEBSERVER
//   //} //if json

  
// /******************************************************************************************************************
//  * MQTT
// *******************************************************************************************************************/


// #ifdef USE_MODULE_NETWORK_MQTT

// void mEnergyInterface::MQTTHandler_Init(){

//   struct handler<mEnergyInterface>* ptr;
  
//   ptr = &mqtthandler_settings;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Settings;

//   ptr = &mqtthandler_sensor_teleperiod;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Sensor;

//   ptr = &mqtthandler_sensor_ifchanged;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
//   ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_IFCHANGED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Sensor;
  
//   ptr = &mqtthandler_energystats_teleperiod;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_ENERGY_STATS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_EnergyStats;
  
//   ptr = &mqtthandler_energystats_ifchanged;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
//   ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_ENERGY_STATS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_EnergyStats;

//   ptr = &mqtthandler_thresholdlimits_teleperiod;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_THRESHOLDLIMITS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_ThresholdLimits;

//   ptr = &mqtthandler_thresholdlimits_ifchanged;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_IFCHANGED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_THRESHOLDLIMITS_CTR;
//   ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_ThresholdLimits;

// } 

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mEnergyInterface::MQTTHandler_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mEnergyInterface::MQTTHandler_Rate()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
//   }
// }

// /**
//  * @brief MQTTHandler_Sender
//  * */
// void mEnergyInterface::MQTTHandler_Sender()
// {    
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
//   }
// }
  
// #endif// USE_MODULE_NETWORK_MQTT

// /******************************************************************************************************************
//  * WebServer
// *******************************************************************************************************************/



// //   #ifdef USE_MODULE_NETWORK_WEBSERVER
// // void mEnergyInterface::WebAppend_Root_Draw_Table(){
// //   char buffer[30];
// //   BufferWriterI->Append_P(PSTR("{t}"));
// //   // BufferWriterI->Append_P(PSTR("<table style='border: 1px solid white;border-collapse:collapse;'>"));
// //   //headers
// //   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
// //   for(int col=0;col<Energy.phase_count+1;col++){
// //     if(col==0){ //first column blank
// //       // BufferWriterI->Append_P(PSTR("<th>    Parameter /\nDevice      </th>"));
// //       BufferWriterI->Append_P(PSTR("<th></th>"));
// //     }else{
// //       // BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), ii, PM_DLIM_LIST_TABLE_HEADERS));
// //         BufferWriterI->Append_P(PSTR("<td>%s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_DRIVERS_ENERGY_ID,col-1,buffer,sizeof(buffer)));//"Animation List Tester");      //titles are fixed, so send them here using getindex    
// //     }
// //   }    
// //   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
// //   //rows
// //   for(int row=0;row<7;row++){
// //     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
// //     for(int col=0;col<Energy.phase_count+1;col++){
// //       if(col==0){ //row name
// //         BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), row, PM_DLIM_LIST_TABLE_HEADERS));
// //         // BufferWriterI->Append_P(PSTR("<td>%s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_DRIVERS_ENERGY_ID,row,buffer,sizeof(buffer)));//"Animation List Tester");      //titles are fixed, so send them here using getindex
// //       }else{
// //         BufferWriterI->Append_P(PSTR("<td>{dc}%s'>" D_DEFAULT_HTML_VALUE "</div></td>"),"ener_tab");  
// //       }
// //     }
// //     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
// //   }
// //   BufferWriterI->Append_P(PSTR("{t2}")); 
// // }

// // //append to internal buffer if any root messages table
// // void mEnergyInterface::WebAppend_Root_Status_Table(){

// //   char buffer[50];
// //   uint8_t count = 0;
  
// //   JBI->Array_Start("ener_tab");// Class name
// //   for(int row=0;row<6;row++){
// //     for(int device=0;device<Energy.phase_count;device++){//Energy.phase_count;device++){
// //       JBI->Object_Start();
// //         JBI->Add("id",count++);
// //         //add for flag, to highlight row where power is "in use"/high
// //         if(pCONT_pzem->data_modbus[device].active_power>30){
// //           JBI->Add("fc","#ff0000");
// //         }else{
// //           JBI->Add("fc","#ffffff");
// //         }
// //         // JBI->Add_FV("ih","\"c%d d%d\"", count-1, device);
// //         switch(row){
// //           case 0: JBI->Add("ih",pCONT_pzem->data_modbus[device].voltage); break;
// //           case 1: JBI->Add("ih",pCONT_pzem->data_modbus[device].current); break;
// //           case 2: JBI->Add("ih",pCONT_pzem->data_modbus[device].active_power); break;
// //           case 3: JBI->Add("ih",pCONT_pzem->data_modbus[device].frequency); break;
// //           case 4: JBI->Add("ih",pCONT_pzem->data_modbus[device].power_factor); break;
// //           case 5: JBI->Add("ih",pCONT_pzem->data_modbus[device].energy); break;
// //         } //switch      
// //       JBI->Object_End();
// //     }
// //   }//end for
// //   JBI->Array_End();
  

// //   // char colour_button[8];
// //   // char button_ids[] = {ANIMATION_MODE_EFFECTS_ID, ANIMATION_MODE_SCENE_ID, 
// //   // #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
// //   // ANIMATION_MODE_NOTIFICATIONS_ID, 
// //   // #endif
// //   // ANIMATION_MODE_NONE_ID};
  
// //   // JBI->Array_Start("animod");// Class name
// //   // for(int row=0;row<sizeof(button_ids);row++){
// //   //   if(button_ids[row] == animation.mode_id){
// //   //     sprintf(colour_button,"%s\0",COLOR_BUTTON_HIGHLIGHT); //selected
// //   //   }else{
// //   //     sprintf(colour_button,"%s\0",COLOR_BUTTON); //NOT selected
// //   //   }        
// //   //   JBI->Object_Start();
// //   //     JBI->Add("id",row);
// //   //     JBI->Add("bc",colour_button);
// //   //   JBI->Object_End();
// //   // }  
// //   // JBI->Array_End();

// // }


// //   #endif// USE_MODULE_NETWORK_WEBSERVER



// // const char HTTP_ENERGY_SNS1[] PROGMEM =
// //   "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
// //   "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
// //   "{s}" D_POWER_FACTOR "{m}%s{e}";

// // const char HTTP_ENERGY_SNS2[] PROGMEM =
// //   "{s}" D_ENERGY_TODAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
// //   "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
// //   "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

// // const char HTTP_ENERGY_SNS3[] PROGMEM =
// //   "{s}" D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}";



















// #endif

