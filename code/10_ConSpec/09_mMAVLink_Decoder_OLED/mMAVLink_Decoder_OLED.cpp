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


#include "mMAVLink_Decoder_OLED.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED

int8_t mMAVLink_Decoder_OLED::Tasker(uint8_t function, JsonParserObject obj){

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
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      MQTTHandler_RefreshAll();
    break;
    #endif  
  }

}//end



void mMAVLink_Decoder_OLED::Pre_Init(void)
{

}


void mMAVLink_Decoder_OLED::Init(void)
{
  
    settings.fEnableSensor = true;

    
}


void mMAVLink_Decoder_OLED::EverySecond()
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
void mMAVLink_Decoder_OLED::SubTask_UpdateOLED()
{
  
  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  pCONT_iDisp->renderer->setTextSize(1);

  page_settings.seconds_decounter--;
  if(page_settings.seconds_decounter==0)
  {
    //reset counter
    page_settings.seconds_decounter = 4;
    //change page
    if(page_settings.active_page++ > 1){ page_settings.active_page = 0; } // increase or restart
  }

  switch(page_settings.active_page)
  {
    default:
    case 0:
      SubSubTask_ShowDisplay_Page0();
    break;
    case 1:
      SubSubTask_ShowDisplay_Page1();
    break;
  }

}

void mMAVLink_Decoder_OLED::SubSubTask_ShowDisplay_Page0()
{

  char buffer[30] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  

  #ifdef USE_MODULE_SENSORS_GPS_MODEM
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);

  char convf_altitude[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.altitude,1,convf_altitude);
  char convf_speed_kph[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.speed,1,convf_speed_kph);
  char convf_speed_mph[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.speed*0.621371,1,convf_speed_mph);


  char convf_accuracy[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.accuracy,1,convf_accuracy);
  char convf_usat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.usat,0,convf_usat);
  char convf_vsat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_gps->location.vsat,0,convf_vsat);

  
  uint16_t gps_seconds_updated = 0;
  uint32_t gps_millis_updated = abs(millis()-pCONT_gps->readings.update_seconds);
  gps_millis_updated /= 1000;


  #endif // USE_MODULE_SENSORS_GPS_MODEM

  #ifdef USE_MODULE_NETWORK_CELLULAR
  char convf_gprs_upsecs[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_cell->gprs.connected_seconds,0,convf_gprs_upsecs);

  char convf_batt_mv[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_cell->modem_status.battery.volts_mv,0,convf_batt_mv);
  char convf_batt_per[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_cell->modem_status.battery.percentage,0,convf_batt_per);

  uint32_t gprs_millis_updated = abs(millis()-pCONT_cell->gprs.last_comms_millis_updated);
  gprs_millis_updated /= 1000;
  #endif // USE_MODULE_NETWORK_CELLULAR
 
  #ifdef USE_MODULE_SENSORS_GPS_MODEM
  /**
   * @brief 21x8
   * 
   */

  // rintf(buffer, sizeof(buffer), "123456789123456789123"); pCONT_iDisp->LogBuffer_AddRow(buffer, 0);
  snprintf(buffer, sizeof(buffer), "Lat %s C", convf_lat); pCONT_iDisp->LogBuffer_AddRow(buffer, 0); //cellular/connection page
  snprintf(buffer, sizeof(buffer), "Lon %s", convf_lon); pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  snprintf(buffer, sizeof(buffer), "Alt %sm  Spd %smph", convf_altitude, convf_speed_mph); pCONT_iDisp->LogBuffer_AddRow(buffer, 2);  
  snprintf(buffer, sizeof(buffer), "Acc %sm Fix %s|%s", convf_accuracy, convf_usat, convf_vsat); pCONT_iDisp->LogBuffer_AddRow(buffer, 3);  
  snprintf(buffer, sizeof(buffer), "GPRS u%s secs %d dBm", convf_gprs_upsecs, int(pCONT_cell->gprs.signal_quality_rssi_dbm));  pCONT_iDisp->LogBuffer_AddRow(buffer, 4);
  snprintf(buffer, sizeof(buffer), "Bat %s mv, %s%%", convf_batt_mv, convf_batt_per);  pCONT_iDisp->LogBuffer_AddRow(buffer, 5);
  snprintf(buffer, sizeof(buffer), "GT %02d:%02d:%02d", pCONT_gps->location.hour, pCONT_gps->location.minute, pCONT_gps->location.second);  pCONT_iDisp->LogBuffer_AddRow(buffer, 6);
  
  if(
    (gps_millis_updated<10)&&
    (gprs_millis_updated<10)
  ){
    snprintf(buffer, sizeof(buffer), "OH %s  %s", display_message.line7, "GOOD");  pCONT_iDisp->LogBuffer_AddRow(buffer, 7);
  }else{
    snprintf(buffer, sizeof(buffer), "OH %s  m%d sec", display_message.line7, gps_millis_updated);  pCONT_iDisp->LogBuffer_AddRow(buffer, 7);
  }

  #endif // USE_MODULE_SENSORS_GPS_MODEM
  
}
void mMAVLink_Decoder_OLED::SubSubTask_ShowDisplay_Page1()
{

  char buffer[30] = {0};
  char buffer_f[100] = {0};
  char buffer_n[100] = {0};
  

  #if defined(USE_MODULE_NETWORK_CELLULAR) && defined(USE_MODULE__DRIVERS_MAVLINK_DECODER)

  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(float(pCONT_mavlink->pkt.gps_raw_int.data.lat)/10000000,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);

  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(float(pCONT_mavlink->pkt.gps_raw_int.data.lon)/10000000,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);

  char convf_altitude[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(float(pCONT_mavlink->pkt.gps_raw_int.data.alt)/1000,1,convf_altitude);
  char convf_speed_kph[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(float(pCONT_mavlink->pkt.gps_raw_int.data.vel)/1000,1,convf_speed_kph);
  char convf_speed_mph[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(float(pCONT_mavlink->pkt.gps_raw_int.data.vel)/1000*0.621371,1,convf_speed_mph);


  char convf_accuracy[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.h_acc,1,convf_accuracy);
  char convf_usat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.fix_type,0,convf_usat);
  char convf_vsat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.satellites_visible,0,convf_vsat);

  char convf_gprs_upsecs[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_cell->gprs.connected_seconds,0,convf_gprs_upsecs);

  char convf_batt_mv[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_mavlink->pkt.sys_status.data.voltage_battery,0,convf_batt_mv);
  char convf_batt_per[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(pCONT_mavlink->pkt.sys_status.data.battery_remaining,0,convf_batt_per);

  uint16_t gps_seconds_updated = 0;
  uint32_t gps_millis_updated = abs(millis()-pCONT_gps->readings.update_seconds);
  gps_millis_updated /= 1000;
  uint32_t gprs_millis_updated = abs(millis()-pCONT_cell->gprs.last_comms_millis_updated);
  gprs_millis_updated /= 1000;
 
  
  /**
   * @brief 21x8
   * 
   */

  // rintf(buffer, sizeof(buffer), "123456789123456789123"); pCONT_iDisp->LogBuffer_AddRow(buffer, 0);
  snprintf(buffer, sizeof(buffer), "Lat %s M", convf_lat); pCONT_iDisp->LogBuffer_AddRow(buffer, 0); //mavlink page
  snprintf(buffer, sizeof(buffer), "Lon %s", convf_lon); pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  snprintf(buffer, sizeof(buffer), "Alt %sm  Spd %smph", convf_altitude, convf_speed_mph); pCONT_iDisp->LogBuffer_AddRow(buffer, 2);  
  snprintf(buffer, sizeof(buffer), "Acc %sm Fix %s|%s", convf_accuracy, convf_usat, convf_vsat); pCONT_iDisp->LogBuffer_AddRow(buffer, 3);  
  snprintf(buffer, sizeof(buffer), "GPRS u%s secs", convf_gprs_upsecs);  pCONT_iDisp->LogBuffer_AddRow(buffer, 4);
  snprintf(buffer, sizeof(buffer), "UAV Batt %s V, %s%%", convf_batt_mv, convf_batt_per);  pCONT_iDisp->LogBuffer_AddRow(buffer, 5);
  snprintf(buffer, sizeof(buffer), "GT %02d:%02d:%02d", pCONT_gps->location.hour, pCONT_gps->location.minute, pCONT_gps->location.second);  pCONT_iDisp->LogBuffer_AddRow(buffer, 6);
  
  snprintf(buffer, sizeof(buffer), "%s", pCONT_mavlink->pkt.statustext.data.text);  pCONT_iDisp->LogBuffer_AddRow(buffer, 7);
  
  #endif // USE_MODULE_NETWORK_CELLULAR
  
}


/******************************************************************************************************************
 * mTempSensorOLEDBath_Commands.cpp
*******************************************************************************************************************/


void mMAVLink_Decoder_OLED::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];

  if(jtok = obj["OpenHAB"].getObject()["SystemTime"])
  {
    snprintf(display_message.line7, sizeof(display_message.line7), "%s", jtok.getStr());
  }
    
}


/******************************************************************************************************************
 * mTempSensorOLEDBath_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mMAVLink_Decoder_OLED::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);    
  return JBI->End();

}

uint8_t mMAVLink_Decoder_OLED::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mMAVLink_Decoder_OLED::MQTTHandler_Init()
{

  struct handler<mMAVLink_Decoder_OLED>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder_OLED::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder_OLED::ConstructJSON_State;

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mMAVLink_Decoder_OLED::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMAVLink_Decoder_OLED::MQTTHandler_Rate()
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
void mMAVLink_Decoder_OLED::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED