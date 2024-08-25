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
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
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
  
  ALOG_INF(PSTR("mDeskSensorsOnOLED::SubTask_UpdateOLED"));

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

  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  
  snprintf(buffer, sizeof(buffer), "%s", pCONT_time->GetTime().c_str() );

  int sensor_id=0;
  float sensor_data = -1;
  int8_t line = 0;


  int16_t out_module_id = 0;
  int8_t  out_sensor_id = 0;

  sensors_reading_t val;

  /*****************************
   * Row 1: BME680 Temp
   *****************************/
  #ifdef USE_MODULE_SENSORS_BME

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_BME_CTR, D_DEVICE_SENSOR_BME_680_NAME, &out_module_id, &out_sensor_id ) )
  {
    ALOG_INF(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    pCONT->GetModule_IndexUnique(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);   
      sprintf(buffer_n, "%s\0", "BM6");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }else{
      snprintf(buffer, sizeof(buffer), "Invalid"); 
    }
  }

  ALOG_INF(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif
  /*****************************
   * Row 2: BME280 Temp
   *****************************/
  #ifdef USE_MODULE_SENSORS_BME

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_BME_CTR, D_DEVICE_SENSOR_BME_280_NAME, &out_module_id, &out_sensor_id ) )
  {
    ALOG_INF(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    pCONT->GetModule_IndexUnique(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);   
      sprintf(buffer_n, "%s\0", "BM2");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }else{
      snprintf(buffer, sizeof(buffer), "Invalid"); 
    }
  }

  ALOG_INF(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif
  /*****************************
   * Row 3: BH1750 Light
   *****************************/
  #ifdef USE_MODULE_SENSORS_BME

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_BH1750_CTR, D_DEVICE_SENSOR_BH1750_NAME, &out_module_id, &out_sensor_id ) )
  {
    ALOG_INF(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    pCONT->GetModule_IndexUnique(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID);   
      sprintf(buffer_n, "%s\0", "Lgt");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }else{
      snprintf(buffer, sizeof(buffer), "Invalid"); 
    }
  }

  ALOG_INF(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif
  /*****************************
   * Row 3: LED Amps
   *****************************/
  #ifdef USE_MODULE_ENERGY_INA219

  if( DLI->GetModuleAndSensorIDs(D_MODULE_SENSORS_INA219_CTR, D_DEVICE_SENSOR_CURRENT, &out_module_id, &out_sensor_id ) )
  {
    ALOG_INF(PSTR("mod %d %d"), out_module_id, out_sensor_id );
    pCONT->GetModule_IndexUnique(out_module_id)->GetSensorReading(&val, out_sensor_id);
    if(val.Valid())
    {
      sensor_data = val.GetFloat(SENSOR_TYPE_CURRENT_ID);   
      sprintf(buffer_n, "%s\0", "Amp");
      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));    
    }else{
      snprintf(buffer, sizeof(buffer), "Invalid"); 
    }
  }

  ALOG_INF(PSTR("buffer %d %s"), line, buffer );
  pCONT_iDisp->LogBuffer_AddRow(buffer, line++);

  #endif




  // /**
  //  * @brief Add each sensor on new line
  //  */
   
  // uint8_t sensors_available = 4;//pCONT_db18->GetSensorCount();

  // int8_t line = -1;

  // for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  // {
  //   line = -1;
  //   sensors_reading_t val;
  //   #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  //   pCONT_pzem->GetSensorReading(&val, sensor_id);
  //   if(val.Valid())
  //   {

  //     sensor_data = val.GetFloat(SENSOR_TYPE_ACTIVE_POWER_ID);        
  //     DLI->GetDeviceName_WithModuleUniqueID( pCONT_pzem->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

  //     /**
  //      * @brief Check for name and replace with OLED friendly short name
  //      * 
  //      */
  //     if(strcmp(buffer_n, D_DEVICE_HEATER_0_NAME)==0)
  //     {
  //       memset(buffer_n, 0, sizeof(buffer_n));
  //       sprintf(buffer_n, "%s", "TEST");
  //       line = 0;
  //     }else 
  //     if(strcmp(buffer_n, D_DEVICE_HEATER_1_NAME)==0)
  //     {
  //       memset(buffer_n, 0, sizeof(buffer_n));
  //       sprintf(buffer_n, "%s", "1Flr");
  //       line = 1;
  //     }
  //     else 
  //     if(strcmp(buffer_n, D_DEVICE_HEATER_2_NAME)==0)
  //     {
  //       memset(buffer_n, 0, sizeof(buffer_n));
  //       sprintf(buffer_n, "%s", "Fan");
  //       line = 2;
  //     }else 
  //     if(strcmp(buffer_n, D_DEVICE_HEATER_3_NAME)==0)
  //     {
  //       memset(buffer_n, 0, sizeof(buffer_n));
  //       sprintf(buffer_n, "%s", "Oil");
  //       line = 3;
  //     }

  //     if(line >= 0)
  //     {
  //       snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
  //       pCONT_iDisp->LogBuffer_AddRow(buffer, line);
  //     }

  //   }
  //   #endif

  // }

  // if(line =- 1) // no valid readings
  // {
  //   memset(buffer_n, 0, sizeof(buffer_n));
  //   sprintf(buffer_n, "%s", "PZEM d/c");
  //   line = 0;
  //   pCONT_iDisp->LogBuffer_AddRow(buffer, line);

  // }

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


  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  
  snprintf(buffer, sizeof(buffer), "%s", pCONT_time->GetTime().c_str() );

  float sensor_data = -1;

  /**
   * @brief Add each sensor on new line
   */
   
  uint8_t sensors_available = 4;//pCONT_db18->GetSensorCount();

  int8_t line = -1;

  for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  {
    line = -1;
    sensors_reading_t val;
    #ifdef USE_MODULE_ENERGY_PZEM004T_V3
    pCONT_pzem->GetSensorReading(&val, sensor_id);
    if(val.Valid())
    {

      sensor_data = val.GetFloat(SENSOR_TYPE_ACTIVE_POWER_ID);        
      DLI->GetDeviceName_WithModuleUniqueID( pCONT_pzem->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

      /**
       * @brief Check for name and replace with OLED friendly short name
       * 
       */
      if(strcmp(buffer_n, D_DEVICE_HEATER_0_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "p22");
        line = 0;
      }else 
      if(strcmp(buffer_n, D_DEVICE_HEATER_1_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "2Flr");
        line = 1;
      }
      else 
      if(strcmp(buffer_n, D_DEVICE_HEATER_2_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "Fan");
        line = 2;
      }else 
      if(strcmp(buffer_n, D_DEVICE_HEATER_3_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "Oil");
        line = 3;
      }

      if(line >= 0)
      {
        snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
        pCONT_iDisp->LogBuffer_AddRow(buffer, line);
      }

    }
    #endif

  }

  if(line =- 1) // no valid readings
  {
    memset(buffer_n, 0, sizeof(buffer_n));
    sprintf(buffer_n, "%s", "PZEM d/c");
    line = 0;
    pCONT_iDisp->LogBuffer_AddRow(buffer, line);

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

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDeskSensorsOnOLED::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mDeskSensorsOnOLED::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mDeskSensorsOnOLED::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mDeskSensorsOnOLED::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mDeskSensorsOnOLED::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED__ID, handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED