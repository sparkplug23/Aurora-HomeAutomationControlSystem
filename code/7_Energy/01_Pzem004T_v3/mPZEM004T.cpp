/*
  mPZEM004T.cpp - PZEM004T v3 MODBUS

  Copyright (C) 2021  Michael Doone

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

  https://github.com/mandulaj/PZEM-004T-v30/blob/master/src/PZEM004Tv30.cpp
*/
#include "mPZEM004T.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3

int8_t mEnergyPZEM004T::Tasker(uint8_t function, JsonParserObject obj)
{
  
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

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      if(rt.waiting_address_response)                  rt.waiting_address_response--;
      if(rt.enable_search_for_devices_seconds_delayed) rt.enable_search_for_devices_seconds_delayed--;

      if(rt.enable_search_for_devices_seconds_delayed==1) DeviceSearch();

    break;
    case TASK_LOOP: 
      if(data_v.size()) // Only if there are devices
        EveryLoop();
    break;
    case TASK_EVERY_MINUTE:

      if(rt.expected_device_count) // Only if set
      {
        if(data_v.size() < rt.expected_device_count) // Only if mismatch
        {
          ALOG_WRN(PSTR(D_LOG_PZEM "Expected device count %d not met, searching for devices"), rt.expected_device_count);
          rt.enable_search_for_devices_seconds_delayed = 2; // Delay search for devices
        }
      }

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
    #endif
  }
  
  return function_result;

} // END function


void mEnergyPZEM004T::Pre_Init(void)
{

  if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  {
    module_state.mode = ModuleStatus::Initialising;
  }
  
}


void mEnergyPZEM004T::Init(void)
{

  modbus = new TasmotaModbus(pCONT_pins->GetPin(GPIO_PZEM0XX_RX_MODBUS_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID));

  uint8_t result = modbus->Begin(9600);

  ALOG_DBG(PSTR("modbus result = %d"),result);

  if (result) {
    // Change this to another function, that doesnt check pin, it just calls claimserial but internally checks if its being used
    pCONT_sup->ClaimSerial();    
    module_state.mode = ModuleStatus::Initialising;
  } else {
    module_state.mode = ModuleStatus::NoGPIOConfigured;
    return;
  }

  module_state.mode = ModuleStatus::Running;

}


void mEnergyPZEM004T::EveryLoop()
{

  /**
   * @brief Check for incoming messages
   **/
  bool data_ready = modbus->ReceiveReady();
  bool successful_read = false;

  if(data_ready)
  {
    uint8_t modbus_buffer[30] = {0};  // At least 5 + (2 * 10) = 25
    uint8_t registers = 10;
    if(rt.waiting_address_response) registers = 2;     // Need 1 byte extra as response is F8 06 00 02 00 01 FD A3
    uint8_t error = modbus->ReceiveBuffer(modbus_buffer, registers);

    ALOG_DBM(D_LOG_PZEM "RX:%d", modbus->ReceiveCount());
    // AddLog_Array(LOG_LEVEL_DEBUG, PSTR("bufferA"), modbus_buffer, (uint8_t)30);

    if(!error)
    {
      ALOG_DBM( PSTR(D_LOG_PZEM "Read id=%d \tvolt=%d"), rt.device_current, (int)data[rt.device_current].voltage);      
      ParseModbusBuffer(modbus_buffer, &data_v[rt.device_current]);
      successful_read = true;
      rt.waiting_address_response = 0; // reset
    }
    else
    {
      ALOG_ERR(D_LOG_PZEM "%d %d", modbus->ReceiveReady(), error);
    }
  }


  if(rt.waiting_address_response)
  {
    return; // wait for address response
  }


  /**
   * @brief Move to next sensor if previous was a success
   **/
  if(successful_read)
  {
    if(rt.device_current < data_v.size()-1)
    {
      rt.device_current++;
      // request_reading.trigger(); // Immediately request next reading
      request_reading.trigger_with_backoff_ms(millis()+100); // move the saved millis ahead by 100ms 
      ALOG_DBM(PSTR(D_LOG_PZEM "request_reading.trigger() %d"), millis());
      // Note: since the request_reading is not cleared here, it will immediately request the next reading
    }
    else
    {
      rt.device_current = 0;
      request_reading.millis = millis();
      request_reading.run = false; // make sure to clear this too
      ALOG_DBG(PSTR(D_LOG_PZEM "All sensors read %d(%d)"), rt.rate_measure_ms, millis());
    }
  }
  

  /**
   * @brief Request new readings (or timeout)
   * This section will ask each sensor for a reading when measure time has started, and immediately iterate to the next sensor.
   * Only when all sensors have been recorded (which takes EVERY_LOOP*SENSORS) will this timereached be reset.
   * OR successful_read, allows immediate request of next sensor
   **/
  if(mTime::TimeReached(&request_reading, rt.rate_measure_ms))
  {
    uint8_t address = data_v[rt.device_current].address;
    modbus->Send(address, 0x04, 0, 10);   
    ALOG_DBM(PSTR(D_LOG_PZEM "RQST [%d]%d %d"), rt.device_current, address, millis());  
  }

} // END EveryLoop


void mEnergyPZEM004T::ParseModbusBuffer(uint8_t* buffer, mEnergyPZEM004T::DeviceData_t* data){
  //           0     1     2     3     4     5     6     7     8     9           = ModBus register
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  = Buffer index
  // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
  // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
  data->voltage       = (float)((buffer[3]  << 8)  + buffer[4]) / 10.0;                                                  // 6553.0 V
  data->current       = (float)((buffer[7]  << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
  data->active_power  = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
  data->frequency     = (float)((buffer[17] << 8)  + buffer[18]) / 10.0;                                              // 50.0 Hz
  data->power_factor  = (float)((buffer[19] << 8)  + buffer[20]) / 100.0;  
  data->import_active = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]); 
  data->measured_time = millis();
}


void mEnergyPZEM004T::SetIDWithAddress(uint8_t address_id, uint8_t address_to_save)
{

  if (address_id < data_v.size()) {
    data_v[address_id].address = address_to_save;  // update address
  } else {
    DeviceData_t mod; // created new
    mod.address = address_to_save; // load new address
    data_v.push_back(mod);
  }

}

void mEnergyPZEM004T::AddDeviceIfNotExists(uint8_t address_to_save) {
    // Check if the address already exists in the vector
    for (const auto& device : data_v) {
        if (device.address == address_to_save) {
            // Address already exists, so we return without adding it again
            return;
        }
    }
    // If we reach here, it means the address does not exist in the vector
    DeviceData_t newDevice;  // Create a new DeviceData_t object
    newDevice.address = address_to_save;  // Set the address
    data_v.push_back(newDevice);  // Add the new device to the vector
}




void mEnergyPZEM004T::DeviceSearch(uint8_t address_limit)
{

  uint8_t found_address = 0;
  uint8_t modbus_buffer[30] = {0}; 
  bool flag_timeout = false;

  uint16_t millis_limit = 10000; // total search for all devices must occur within 2 seconds

  for(
    uint8_t address_search=0;
            address_search<address_limit;
            address_search++
  ){

    ALOG_INF(PSTR("MODBUS Address %d: Searching"), address_search);
    modbus->Send(address_search, 0x04, 0, 10);      
    found_address = 0;
    flag_timeout = true;
    timeout = millis();
    uint16_t wait_time = 200;
    if(address_limit>100) wait_time = 100;
    while(abs(millis()-timeout)<wait_time)
    {
      WDT_Reset();

      if(modbus->ReceiveReady())
      { 
        uint8_t error = modbus->ReceiveBuffer(modbus_buffer, 10);
        found_address = modbus_buffer[2]; // addres byte
        ALOG_INF(PSTR("MODBUS Address = %d FOUND %d"), address_search, found_address);
        flag_timeout = false;
        /**
         * @brief Only add if valid voltage is detected
         **/
        DeviceData_t mod; // created new
        ParseModbusBuffer(modbus_buffer, &mod);
        if(mod.voltage > 0.0)
        {
          AddDeviceIfNotExists(address_search);
        }
        break; // out of the while
      }
    
    }

    if(flag_timeout)
    {
      ALOG_INF(PSTR("MODBUS Address %d: No Response"), address_search);
    }
  }

}


void mEnergyPZEM004T::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  JsonParserObject jobj = 0;

  if(!(jobj = obj[D_MODULE_ENERGY_PZEM004T_CTR].getObject()))
  {
    ALOG_DBM(PSTR("No valid %s JSON object"), D_MODULE_ENERGY_PZEM004T_CTR);
    return;
  }
  
  if(jtok = jobj["SearchForDevices"])
  {
    uint8_t address_check_maxrange = jtok.getInt();
    ALOG_HGL(PSTR("address_check_maxrange = %d"), address_check_maxrange);
    DeviceSearch(address_check_maxrange);
  }

  if(jtok = jobj["ExpectedDevices"])
  {
    rt.expected_device_count = jtok.getInt();
    ALOG_HGL(PSTR("expected_device_count = %d"), rt.expected_device_count);
  }

  // Only commands below are for running mode ////////////////////////////////////////////////////////////////////////////////////////////////
  if(module_state.mode != ModuleStatus::Running) return; 
  // Only commands below are for running mode ////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  if(jtok = jobj["SetAddress"])
  {
    if(data_v.size() > 1)
    {
      ALOG_WRN(PSTR("More than one device detected, SetAddress disabled"));
      return;
    }

    uint16_t addr = jtok.getInt();
    uint8_t slave_address = 0xF8; // broadcast so all devices except command
    uint8_t return_code = modbus->Send(slave_address, CMD_WSR, WREG_ADDR, 1, (uint16_t *) &addr);

    rt.waiting_address_response = true;
    
    ALOG_DBM(PSTR("return_code = %d"), return_code);

  }
   
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mEnergyPZEM004T::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add_P(PM_SENSOR_COUNT, data_v.size());
  return JBI->End();

}

uint8_t mEnergyPZEM004T::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();
  for(int ii=0; ii < data_v.size(); ii++)
  {
    JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), ii, buffer, sizeof(buffer)));
      JBI->Add_P(PM_ADDRESS,      data_v[ii].address );
      JBI->Add_P(PM_VOLTAGE,      data_v[ii].voltage);
      JBI->Add_P(PM_CURRENT,      data_v[ii].current);
      JBI->Add_P(PM_ACTIVE_POWER, data_v[ii].active_power);
      JBI->Add_P(PM_FREQUENCY,    data_v[ii].frequency);
      JBI->Add_P(PM_POWER_FACTOR, data_v[ii].power_factor);
      JBI->Add_P(PM_ENERGY,       data_v[ii].import_active);
    JBI->Object_End();
  }
  // ALOG_INF(PSTR("ConstructJSON_Sensor %s"), JBI->GetBufferPtr());
  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mEnergyPZEM004T::MQTTHandler_Init(){

  struct handler<mEnergyPZEM004T>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod_SubModule();
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
}
#endif // USE_MODULE_NETWORK_MQTT



#endif
