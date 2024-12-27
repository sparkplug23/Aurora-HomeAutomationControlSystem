/**
 * @file mBME.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief BME280 Temperature, Humidity and Pressure sensor 
 * @version 1.0
 * @date 2022-04-20
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

#include "mBME.h"

#ifdef USE_MODULE_SENSORS_BME


int8_t mBME::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function){
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
      BmpRead();
    break;
    case TASK_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
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

  return FUNCTION_RESULT_SUCCESS_ID;

}


void mBME::Pre_Init()
{

  module_state.mode = ModuleStatus::Initialising;
  bmp_count = 0;
  
  uint8_t bmp_addresses[] = { BMP_ADDR1, BMP_ADDR2 };
  
  int bmp_sensor_size = BMP_MAX_SENSORS * sizeof(bmp_sensors_t);
  if (!bmp_sensors) {
    bmp_sensors = (bmp_sensors_t*)malloc(bmp_sensor_size);
  }
  if (!bmp_sensors) { return; }
  memset(bmp_sensors, 0, bmp_sensor_size);  // Init defaults to 0


  for (uint8_t i = 0; i < BMP_MAX_SENSORS; i++) 
  {
    
    if (pCONT_i2c->I2cActive(bmp_addresses[i])) 
    { 
      ALOG_INF(PSTR(D_LOG_BME "Addr 0x%x not found"), bmp_addresses[i]); 
      continue; 
    }
    
    uint8_t bmp_type = pCONT_i2c->I2cRead8(bmp_addresses[i], BMP_REGISTER_CHIPID);
    if (bmp_type) {
      bmp_sensors[bmp_count].i2c_address = bmp_addresses[i];
      bmp_sensors[bmp_count].bmp_type = bmp_type;
      bmp_sensors[bmp_count].bmp_model = 0;

      ALOG_INF(PSTR("i2c_address %d"), bmp_sensors[bmp_count].i2c_address);
      ALOG_INF(PSTR("bmp_type %d"), bmp_sensors[bmp_count].bmp_type);

      bool success = false;
      switch (bmp_type) {
        case BMP180_CHIPID:
          success = Bmp180Calibration(bmp_count);
          break;
        case BME280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 2  
          // No break intentional
        case BMP280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 1
          success = Bmx280Calibrate(bmp_count);
          break;
        case BME680_CHIPID:
          bmp_sensors[bmp_count].bmp_model = 3;  // 3
          success = Bme680Init(bmp_count);
          break;
      }
      if (success) {
        pCONT_sup->GetTextIndexed_P(bmp_sensors[bmp_count].bmp_name, sizeof(bmp_sensors[bmp_count].bmp_name), bmp_sensors[bmp_count].bmp_model, kBmpTypes);
        pCONT_i2c->I2cSetActiveFound(bmp_sensors[bmp_count].i2c_address, bmp_sensors[bmp_count].bmp_name);
        bmp_count++;
      }
    }else{
          
      #ifdef ESP32
      AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("getErrorText =\"%s\""), pCONT_i2c->wire->getErrorText(pCONT_i2c->wire->lastError()));
      #endif 

    }
  }


  if(bmp_count){
    module_state.mode = ModuleStatus::Running;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BME "Running"));
  }

}


void mBME::Init(void)
{
  
}

void mBME::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(bmp_count)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", bmp_count);  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++)
    {      
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%s:\"%s\", ", bmp_sensors[sensor_id].bmp_name, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)));    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_BME "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


/******************************************************************************************************************
 * @SECTION: Commands
*******************************************************************************************************************/




/******************************************************************************************************************
 * @SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mBME::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  char buffer[50];
  JBI->Start();
    JBI->Add_P(PM_SENSOR_COUNT, bmp_count);

    for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++){
      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));   
        JBI->Add_P(PM_TYPE,bmp_sensors[sensor_id].bmp_type);
      JBI->Object_End();
    }

  return JBI->End();

}

uint8_t mBME::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();

  char buffer[50];

  for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++){
    if(
      bmp_sensors[sensor_id].ischanged_over_threshold || 
      (json_level >  JSON_LEVEL_IFCHANGED) || 
      (json_level == JSON_LEVEL_SHORT)
    ){
      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));   
        JBI->Add_P(PM_TEMPERATURE, bmp_sensors[sensor_id].temperature);
        JBI->Add_P(PM_HUMIDITY, bmp_sensors[sensor_id].humidity);
        JBI->Add_P(PM_PRESSURE, bmp_sensors[sensor_id].pressure);
        JBI->Add_P(PM_ALTITUDE, bmp_sensors[sensor_id].altitude);
        if(bmp_sensors[sensor_id].bmp_type == BME680_CHIPID)
        {
          JBI->Add_P(PM_GAS, bmp_sensors[sensor_id].bmp_gas_resistance);
        }
        JBI->Add_P(PM_UTC_TIME, bmp_sensors[sensor_id].utc_measured_timestamp);
        uint32_t sensor_elapsed_time = pCONT_time->UtcTime() - bmp_sensors[sensor_id].utc_measured_timestamp;
        JBI->Add_P(PM_AGE, sensor_elapsed_time);
      JBI->Object_End();
    }
  }
  
  return JBI->End();

}

  
/******************************************************************************************************************
 * @SECTION: MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mBME::MQTTHandler_Init()
{

  struct handler<mBME>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0; // as 0, with SendNow=false, will trigger to send for the first time
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_SENSORS_BME
