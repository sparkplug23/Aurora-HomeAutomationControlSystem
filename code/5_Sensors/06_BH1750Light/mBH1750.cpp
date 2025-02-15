
/**
 * @file mBH1750.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 16-bit Ambient Light sensor
 * @version 1.0
 * @date 2022-05-01
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "mBH1750.h"

#ifdef USE_MODULE_SENSORS_BH1750

int8_t mBH1750::Tasker(uint8_t function, JsonParserObject obj)
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
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
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


void mBH1750::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
  module_state.devices = 0;
}


void mBH1750::Init(void)
{
  
  module_state.devices = 0;

  for (uint32_t i = 0; i < sizeof(i2c_addresses); i++) 
  {

    if (pCONT_i2c->I2cActive(i2c_addresses[i])) { continue; }

    device_data[module_state.devices].address = i2c_addresses[i];

    if (Set_MeasurementTimeRegister(module_state.devices, device_data[module_state.devices].mtreg)) 
    {
      module_state.devices++;
    }

  }

  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }

}


void mBH1750::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(module_state.devices)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", module_state.devices);  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<module_state.devices; sensor_id++)
    {      
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%d:\"%s\", ", sensor_id, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)));    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_BH1750 "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}

uint8_t mBH1750::Get_Resolution_Mode(uint8_t sensor_id) 
{
  return device_data[sensor_id].resolution_mode;
}

// Get the command byte for the given resolution mode
uint8_t mBH1750::Get_Resolution_RegisterValue(uint8_t mode) 
{
  return i2c_resolution_register_value[mode];
}


bool mBH1750::Set_Resolution_Mode(uint8_t sensor_id, uint8_t resolution_mode) 
{
  if (resolution_mode >= 3) {
    return false; // Invalid resolution mode
  }
  
  // Save the resolution mode to the device configuration
  device_data[sensor_id].resolution_mode = resolution_mode;
  
  // Get the corresponding command byte for the given resolution mode
  uint8_t command = Get_Resolution_RegisterValue(resolution_mode);
  
  // Transmit the command byte to the sensor
  pCONT_i2c->wire->beginTransmission(device_data[sensor_id].address);
  pCONT_i2c->wire->write(command);
  return (!pCONT_i2c->wire->endTransmission());
}


bool mBH1750::Set_MeasurementTimeRegister(uint8_t sensor_id, uint8_t mtreg)
{
  // Save the measurement time register value
  device_data[sensor_id].mtreg = mtreg;

  // Transmit high bits of mtreg
  pCONT_i2c->wire->beginTransmission(device_data[sensor_id].address);
  uint8_t data_high = BH1750_MEASUREMENT_TIME_HIGH | ((device_data[sensor_id].mtreg >> 5) & 0x07);
  pCONT_i2c->wire->write(data_high);
  if (pCONT_i2c->wire->endTransmission()) { return false; }

  // Transmit low bits of mtreg
  pCONT_i2c->wire->beginTransmission(device_data[sensor_id].address);
  uint8_t data_low = BH1750_MEASUREMENT_TIME_LOW | (device_data[sensor_id].mtreg & 0x1F);
  pCONT_i2c->wire->write(data_low);
  if (pCONT_i2c->wire->endTransmission()) { return false; }

  // Reapply the resolution mode after setting the measurement time
  return Set_Resolution_Mode(sensor_id, device_data[sensor_id].resolution_mode);

}


bool mBH1750::Get_SensorReading(uint8_t sensor_index) 
{

  if (device_data[sensor_index].valid) { device_data[sensor_index].valid--; }

  if (2 != pCONT_i2c->wire->requestFrom(device_data[sensor_index].address, (uint8_t)2)) 
  { 
    return false;
  }

  float level = (pCONT_i2c->wire->read() << 8) | pCONT_i2c->wire->read();
  float illuminance = level;
  Serial.println(level);
  illuminance /= (1.2 * (69 / (float)device_data[sensor_index].mtreg));
  if (1 == Get_Resolution_Mode(sensor_index)) {
    illuminance /= 2;
  }
  device_data[sensor_index].level = level*1000;
  device_data[sensor_index].illuminance = illuminance*1000;

  ALOG_INF( PSTR(D_LOG_BH1750 "level=%d, lum=%d"), level, illuminance);

  device_data[sensor_index].valid = SENSOR_MAX_MISS;

  return true;

}


/********************************************************************************************/

void mBH1750::ReadSensor(void) 
{

  for (uint32_t i = 0; i < module_state.devices; i++) 
  {
    if (!Get_SensorReading(i)) 
    {
      ALOG_ERR( PSTR(D_LOG_BH1750 "Failed Read") );
    }else{
      ALOG_INF(PSTR(D_LOG_BH1750 "Read Add %02X"), device_data[i].address);
    }
  }

}




/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

/*********************************************************************************************\
 * BH1750 - Ambient Light Intensity
 *
 * Bh1750Resolution1   0..2    - Set BH1750 1 resolution mode
 * Bh1750Resolution2   0..2    - Set BH1750 2 resolution mode
 * Bh1750MTime1       30..255  - Set BH1750 1 MT register
 * Bh1750MTime2       30..255  - Set BH1750 2 MT register
 *
 * I2C Address: 0x23 or 0x5C
\*********************************************************************************************/
void mBH1750::parse_JSONCommand(JsonParserObject obj)
{
  
  char buffer[50];
  JsonParserToken jtok = 0; 

  if(jtok = obj[GetModuleName()])
  {
    JsonParserObject jobj = jtok.getObject();
    JsonParserToken jtok_sub = 0;

    uint8_t sensor_index = 0;
    if(jtok_sub = jobj[PM_INDEX])
    {
      sensor_index = jtok_sub.getInt();
    }

    if(jtok_sub = jobj[PM_RESOLUTION])
    {
      Set_Resolution_Mode(sensor_index, jtok_sub.getInt());
    }

    if(jtok_sub = jobj[PM_TIME])
    {
      Set_MeasurementTimeRegister(sensor_index, jtok_sub.getInt());
    }
  }

}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mBH1750::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  JBI->Add(D_COUNT, module_state.devices);    
  for (uint32_t ii = 0; ii < module_state.devices; ii++)
  {
    JBI->Add(D_ADDRESS, device_data[ii].address);
  }
  return JBI->End();

}


uint8_t mBH1750::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    
  for (uint32_t sensor_index = 0; sensor_index < module_state.devices; sensor_index++) {
    if (device_data[sensor_index].valid) {
      JBI->Add(D_ILLUMINANCE, device_data[sensor_index].illuminance);
      JBI->Add(D_LEVEL, device_data[sensor_index].level);
    }
  }

  return JBI->End();
    
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mBH1750::MQTTHandler_Init()
{

  struct handler<mBH1750>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mBH1750::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBH1750::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBH1750::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 
  
#endif // USE_MODULE_NETWORK_MQTT


#endif

