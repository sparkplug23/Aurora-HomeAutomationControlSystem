/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * To be split into the driver module.
 * Cellular should just be a wrapper/top level API that translates whichever modem is used into a common interface that works with WiFi/Ethernet and just assumes the connection is there (ie handled by the driver)
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR


int8_t mCellular::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
    {      

    }
    break;
    case TASK_EVERY_SECOND: 

      

    break;
    case TASK_EVERY_FIVE_SECOND:   
    {
      
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
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

} // END function


void mCellular::Pre_Init(void){
  
  settings.fEnableSensor = true;
  
}


void mCellular::Init(void)
{



}






/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mCellular::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok2 = 0; 
  int8_t tmp_id = 0;

  
    
}










  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mCellular::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
    
  return JBI->End();

}


uint8_t mCellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  mSIM7000G::GPRS_STATUS gprs = pCONT_sim7000g->gprs;
  mSIM7000G::GPS_STATUS gps = pCONT_sim7000g->gps;
  mSIM7000G::DATA modem_status = pCONT_sim7000g->modem_status;
  // #endif // USE_MODULE_DRIVERS_MODEM_7000G
  // #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  // mSIM7000G::GPRS_STATUS gprs = pCONT_sim7000g->gprs;
  // mSIM7000G::GPS_STATUS gps = pCONT_sim7000g->gps;
  // mSIM7000G::DATA modem_status = pCONT_sim7000g->modem_status;
  // #endif // USE_MODULE_DRIVERS_MODEM_7000G

  JBI->Start();

    JBI->Add("uptime", tkr_time->uptime_seconds_nonreset);
    JBI->Add("rtcseconds", tkr_time->RtcTime.second);
    JBI->Object_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
        JBI->Add("rss_dbm", gprs.signal_quality_rssi_dbm);
    JBI->Object_End();
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Object_Start("GPS");
      JBI->Add("second", gps.second);
      JBI->Add("Speed", gps.speed);
      JBI->Add("altitude", gps.altitude);
      JBI->Add("Latitude", gps.latitude);
      JBI->Add("Longitude", gps.longitude);
      JBI->Add("Enabled", gps.enabled);
      JBI->Add("vsat", gps.vsat);
      JBI->Add("usat", gps.usat);
      JBI->Add("accuracy", gps.accuracy);
      JBI->Add("year", gps.year);
      JBI->Add("month", gps.month);
      JBI->Add("day", gps.day);
      JBI->Add("hour", gps.hour);
      JBI->Add("minute", gps.minute);
      JBI->Add("course", gps.course);
      JBI->Add("FixMode", gps.FixMode);
      JBI->Add("accuracy_position", gps.accuracy_position);
      JBI->Add("accuracy_vertical", gps.accuracy_vertical);
      JBI->Add("usat_glonass", gps.usat_glonass);
      JBI->Add("cno_max", gps.cno_max);
      JBI->Add("HPA", gps.HPA);
      JBI->Add("VPA", gps.VPA);

      char convf_lat[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
      char convf_lon[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
      char convf_fix[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.accuracy,2,convf_fix);

      snprintf_P(buffer, sizeof(buffer),
        PSTR(
          "https://www.google.com/maps/dir//%s,%s"
        ),
        convf_lat, 
        convf_lon
      );

      JBI->Add("url", buffer);
    JBI->Object_End();
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Object_Start("Battery");
      JBI->Add("Volts_mv", modem_status.battery.volts_mv);
      JBI->Add("Percentage", modem_status.battery.percentage);
      JBI->Add("ChargeState", modem_status.battery.charge_state);
      JBI->Add("Valid", modem_status.battery.isvalid);
    JBI->Object_End();

    #endif
  
  return JBI->End();
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mCellular::MQTTHandler_Init()
{

  struct handler<mCellular>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mCellular::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mCellular::MQTTHandler_Rate()
{
  // While developing, set these by the Init() function to make independent

  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mCellular::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_NETWORK_CELLULAR