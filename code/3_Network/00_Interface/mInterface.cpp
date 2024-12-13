/**
 * @file mInterfaceNetwork.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Handles basic connection functions (anything connected?) and possibly priority switching between them
 *        (1) Ethernet
 *        (2) WiFi
 *        (3) Cellular 
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mInterface.h"

#ifdef USE_MODULE_NETWORK_INTERFACE

int8_t mInterfaceNetwork::Tasker(uint8_t function, JsonParserObject obj){

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
    #endif // USE_MODULE_NETWORK_MQTT 
  }

} // END function


void mInterfaceNetwork::Pre_Init(void)
{
  
  settings.fEnableSensor = true;

}


void mInterfaceNetwork::Init(void)
{


}


void mInterfaceNetwork::EveryLoop()
{


}

/**
 * @brief Probably not to be used for mqtt, as the socket can now be checked within instance
 * Adding a debug message so I know its being called, though it will be used later as general connection agent
 * 
 */
bool mInterfaceNetwork::Connected(uint8_t type)
{
  
#ifndef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

  // ALOG_ERR(PSTR("mInterfaceNetwork::Connected"));

  #ifdef USE_MODULE_NETWORK_WIFI
  if(
    (type == NETWORK_TYPE_WIFI) ||
    (type == NETWORK_TYPE_ANY)
  ){
    if(pCONT_wif->WifiCheckIpConnected())
    {
      return true;
    }
  }
  #endif // USE_MODULE_NETWORK_WIFI

/**
 * @brief WiFi needs to be first to fix extra AT commands when WiFi and Cellular are being used, but mqtt is via Wifi and not cellular
 * 
 */
  #ifdef USE_MODULE_NETWORK_CELLULAR
  if(
    (type == NETWORK_TYPE_CELLULAR) ||
    (type == NETWORK_TYPE_ANY)
  ){

    /**
     * @brief Perfect example of how this needs to become a superclass
     * 
     */
    #ifdef USE_MODULE_DRIVERS_MODEM_7000G
    if(pCONT_sim7000g->modem != nullptr)
    {
      if(pCONT_sim7000g->modem->isGprsConnected())
      {
        return true;
      }
    }
    #endif
    #ifdef USE_MODULE_DRIVERS_MODEM_800L
    if(pCONT_sim800l->modem != nullptr)
    {
      if(pCONT_sim800l->modem->isGprsConnected())
      {
        return true;
      }
    }
    #endif

  }
  #endif // USE_MODULE_NETWORK_CELLULAR


#endif //  ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

  return false;
}

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mInterfaceNetwork::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR("GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      // GPS_Enable();
    }
  } 
    
}
  

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mInterfaceNetwork::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add(D_COUNT, settings.leds_found);
  return JBI->End();

}

uint8_t mInterfaceNetwork::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    // JBI->Object_Start("GPRS");
    //     JBI->Add("ConSec", gprs.connected_seconds);
    // JBI->Object_End();  

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mInterfaceNetwork::MQTTHandler_Init()
{

  struct handler<mInterfaceNetwork>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mInterfaceNetwork::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mInterfaceNetwork::ConstructJSON_State;

} 

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_NETWORK_INTERFACE