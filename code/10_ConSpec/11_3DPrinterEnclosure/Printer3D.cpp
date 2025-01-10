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

#include "Printer3D.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE

int8_t mPrinter3D::Tasker(uint8_t function, JsonParserObject obj){

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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

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



void mPrinter3D::Pre_Init(void)
{

}


void mPrinter3D::Init(void)
{
  
  settings.fEnableSensor = true;

}


void mPrinter3D::EverySecond()
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
void mPrinter3D::SubTask_UpdateOLED()
{
  
  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[100] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  
  snprintf(buffer, sizeof(buffer), "%s", tkr_time->RtcTime.hhmmss_ctr);
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);

  // #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306

  float sensor_data = -1;

  /**
   * @brief Add each sensor on new line
   */
   
  uint8_t sensors_available = pCONT_bme->GetSensorCount();

  for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  {
    sensors_reading_t val;
    pCONT_bme->GetSensorReading(&val, sensor_id);
    if(val.Valid())
    {

      sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);        
      DLI->GetDeviceName_WithModuleUniqueID( pCONT_bme->GetModuleUniqueID(), val.sensor_id, buffer_n, sizeof(buffer_n));

      /**
       * @brief Check for name and replace with OLED friendly short name
       * 
       */
      if(strcmp(buffer_n, D_DEVICE_SENSOR_BME_LONG_WIRE_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "TOP");
      }else 
      if(strcmp(buffer_n, D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME)==0)
      {
        memset(buffer_n, 0, sizeof(buffer_n));
        sprintf(buffer_n, "%s", "BOT");
      }

      snprintf(buffer, sizeof(buffer), "%s: %s", buffer_n, mSupport::float2CString(sensor_data,2,buffer_f));
      pCONT_iDisp->LogBuffer_AddRow(buffer, sensor_id);
    
    }

  }

  // #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}


/******************************************************************************************************************
 * mPrinter3D_Commands.cpp
*******************************************************************************************************************/


void mPrinter3D::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];
    
}


/******************************************************************************************************************
 * mPrinter3D_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mPrinter3D::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);    
  return JBI->End();

}

uint8_t mPrinter3D::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mPrinter3D_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mPrinter3D::MQTTHandler_Init()
{

  struct handler<mPrinter3D>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mPrinter3D::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mPrinter3D::ConstructJSON_State;

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mPrinter3D::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mPrinter3D::MQTTHandler_Rate()
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
void mPrinter3D::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE