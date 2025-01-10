/**

  Copyright (C) 2022    Michael Doone

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

#include "mDeskSensorsOnOLED.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED

int8_t mDeskSensorsOnOLED::Tasker(uint8_t function, JsonParserObject obj){

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

  switch(function)
  {    
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
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif  
  }

}//end



void mDeskSensorsOnOLED::Pre_Init(void)
{

}


void mDeskSensorsOnOLED::Init(void)
{

  module_state.mode = ModuleStatus::Running;

}


void mDeskSensorsOnOLED::EverySecond()
{
  
  SubTask_UpdateOLED();

}


/**
 * @brief 
 * Show basic info
 * 
 * IM: SECS min
 * IT: Immersion Temp used for shower
 * BT: Water temp used for bath
 * CL : Show UTC time for easy checking it is working / "UP": Every 5 seconds alternate between showing RTC and Uptime
 * 
 * 
 * 
 * */
void mDeskSensorsOnOLED::SubTask_UpdateOLED()
{
  
  switch(page_showing_index)
  {
    default:
    case 0:
      SubTask_UpdateOLED_Page1();
    break;
    case 1:
      SubTask_UpdateOLED_Page2();
    break;
  }

  if(page_showing_index++ > pages_active)
  {
    page_showing_index = 0;
  }

}


/**
 * @brief 
 * 
 * BME680 Temp
 * BME280 Temp
 * Light
 * LED Amps
 * 
 */
void mDeskSensorsOnOLED::SubTask_UpdateOLED_Page1()
{

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  
  snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetTime().c_str() );

  int sensor_id=0;
  float sensor_data = -1;
  int8_t line = 0;

  uint16_t out_module_id = 0;
  uint8_t  out_sensor_id = 0;

  /*****************************
   * Row 1: BME680 Temp
   *****************************/
  #ifdef USE_MODULE_SENSORS_BME

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_BME_CTR, D_DEVICE_SENSOR_BME_680_NAME, &out_module_id, &out_sensor_id ) )
  {
    ALOG_DBM(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    sensors_reading_t val;
    pCONT->GetModule(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);   
      sprintf(buffer_n, "%s\0", "BM6");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }
  }

  ALOG_DBM(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif
  /*****************************
   * Row 2: BME280 Temp
   *****************************/
  #ifdef USE_MODULE_SENSORS_BME

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_BME_CTR, D_DEVICE_SENSOR_BME_280_NAME, &out_module_id, &out_sensor_id ) )
  {
    ALOG_DBM(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    sensors_reading_t val;
    pCONT->GetModule(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);  
      sprintf(buffer_n, "%s\0", "BM2");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }
  }

  ALOG_DBM(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif
  /*****************************
   * Row 3: BH1750 Light
   *****************************/
  #ifdef USE_MODULE_SENSORS_BME

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_BH1750_CTR, D_DEVICE_SENSOR_BH1750_NAME, &out_module_id, &out_sensor_id ) )
  {
    ALOG_DBM(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    sensors_reading_t val;
    pCONT->GetModule(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID);   
      sprintf(buffer_n, "%s\0", "Lgt");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }
  }

  ALOG_DBM(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif
  /*****************************
   * Row 3: LED Amps
   *****************************/
  #ifdef USE_MODULE_ENERGY_INA219

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_INA219_CTR, D_DEVICE_SENSOR_CURRENT, &out_module_id, &out_sensor_id ) )
  {
    ALOG_DBM(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    sensors_reading_t val;
    pCONT->GetModule(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_CURRENT_ID);   
      sprintf(buffer_n, "%s\0", "Amp");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }
  }

  ALOG_DBM(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif


}

/**
 * @brief 
 * 
 * DB18 1
 * DB18 2
 * DB18 3
 * DB18 4
 * 
 */
void mDeskSensorsOnOLED::SubTask_UpdateOLED_Page2()
{

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  float sensor_data = -1;
  int8_t line = -1;
  
  snprintf(buffer, sizeof(buffer), "%s", tkr_time->GetTime().c_str() );
   
  uint8_t sensors_available = 4;

  for(int sensor_id=0; sensor_id<sensors_available; sensor_id++)
  {

    line = -1;
    sensors_reading_t val;
    #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
    pCONT->GetModule_P(D_MODULE_SENSORS_DB18S20_CTR)->GetSensorReading(&val, sensor_id);   
    if(val.Valid())
    {

      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);        
      DLI->GetDeviceName_WithModuleUniqueID( pCONT->GetModule_P(D_MODULE_SENSORS_DB18S20_CTR)->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

      /**
       * @brief Check for name and replace with OLED friendly short name
       **/
      if     (strcmp(buffer_n, D_DEVICE_SENSOR_DB18S20_01_NAME)==0){ line = 0; sprintf(buffer_n, "%s\0", "DB1"); }
      else if(strcmp(buffer_n, D_DEVICE_SENSOR_DB18S20_02_NAME)==0){ line = 1; sprintf(buffer_n, "%s\0", "DB2"); }
      else if(strcmp(buffer_n, D_DEVICE_SENSOR_DB18S20_03_NAME)==0){ line = 2; sprintf(buffer_n, "%s\0", "DB3"); }
      else if(strcmp(buffer_n, D_DEVICE_SENSOR_DB18S20_04_NAME)==0){ line = 3; sprintf(buffer_n, "%s\0", "DB4"); }

      if(line >= 0)
      {
        snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
        pCONT_iDisp->LogBuffer_AddRow(buffer, line);
      }

    }
    #endif // USE_MODULE_SENSORS__DS18X20_ESP32_2023

  }

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_Commands.cpp
*******************************************************************************************************************/


void mDeskSensorsOnOLED::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];
    
}


/******************************************************************************************************************
 * mTempSensorOLEDBath_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mDeskSensorsOnOLED::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  return JBI->End();

}

uint8_t mDeskSensorsOnOLED::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mDeskSensorsOnOLED::MQTTHandler_Init()
{

  struct handler<mDeskSensorsOnOLED>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDeskSensorsOnOLED::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mDeskSensorsOnOLED::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mDeskSensorsOnOLED::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mDeskSensorsOnOLED::MQTTHandler_Rate()
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
void mDeskSensorsOnOLED::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED