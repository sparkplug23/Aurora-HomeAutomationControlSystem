#include "mMQTT.h"

#if defined(USE_MODULE_NETWORK_MQTT)

int8_t mMQTTManager::Tasker(uint8_t function, JsonParserObject obj){ DEBUG_PRINT_FUNCTION_NAME;

  // ALOG_INF(PSTR("M0host_address===================: %s"), dt.connection[0].host_address);


  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_INIT:
      Init();
    break;
  }

  if(!tkr_set->Settings.flag_system.mqtt_enabled){ return 0; }

  switch(function){
  /************
   * PERIODIC SECTION * 
  *******************/
    case TASK_LOOP:
      MM_EveryLoop();
      CallMQTTSenders();
    break;
    case TASK_MQTT_CONNECTED:
    // ALOG_ERR(PSTR("MQTT_CONNECTED hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"));
      Load_New_Subscriptions_From_Function_Template();
    break;
    case TASK_NETWORK_CONNECTION_ESTABLISHED:

      /**
       * @brief If status is down, and awaiting connection, immediate try reconnect
       * */
      if(brokers.size()) 
      // if(brokers_active)   
        if(brokers[0]->retry_counter)
          brokers[0]->retry_counter = 1; //retry immediate
          ALOG_HGL(PSTR(D_LOG_PUBSUB "retry_counter IMMEDIATE = %d"),brokers[0]->retry_counter);

    break;
    case TASK_EVERY_50_MSECOND:
DEBUG_LINE_HERE_MILLIS

DEBUG_LINE_HERE_MILLIS
      MM_Every50mSecond();
DEBUG_LINE_HERE_MILLIS

      // if(brokers.size())   
      //   if(brokers[0]->retry_counter)
      //     Serial.printf(D_LOG_PUBSUB "retry_counter = %d\n\r", brokers[0]->retry_counter);

    break;
    case TASK_EVERY_SECOND:      {
      
      // if(brokers.size())
      //   ALOG_INF(PSTR("M4host_address: %s"), brokers[0]->host_address);

  
      strlcpy(test, "tesT MESSAGE FROM INIT", sizeof(test));

      MM_EverySecond();  

//   ALOG_INF(PSTR("M5host_address: %s"), dt.connection[0].host_address);

//       for(int i=0;i<50;i++)
//       {
//         Serial.write(dt.connection[0].prefixtopic[i]);
//       }
//       Serial.println();

//   ALOG_INF(PSTR("M6host_address: %s"), dt.connection[0].host_address);

// uint8_t idx=0;
//   ALOG_INF(PSTR("Fhost_address: %s"), dt.connection[idx].host_address);
//   ALOG_INF(PSTR("port: %d"), dt.connection[idx].port);
//   ALOG_INF(PSTR("user: %s"), dt.connection[idx].user);
//   ALOG_INF(PSTR("pwd: %s"), dt.connection[idx].pwd);
//   ALOG_INF(PSTR("retry: %d"), dt.connection[idx].retry);
//   ALOG_INF(PSTR("client: %s"), dt.connection[idx].client);
//   ALOG_INF(PSTR("prefixtopic: %s"), dt.connection[idx].prefixtopic);
//   ALOG_INF(PSTR("status: %d"), dt.connection[idx].status);

//   pCONT_tel->mqtthandler_mqtt.flags.SendNow = true;



    }
    break;
    case TASK_EVERY_HOUR:
      if(brokers.size())
      // if(brokers_active)   
      {
        brokers[0]->Send_LWT_Online(); // this does work, but how about wrapping the LWT into the normal status messages? Just the LWT offline would be its own.
      }
    break;
    case TASK_UPTIME_10_MINUTES:
      flag_uptime_reached_reduce_frequency = true;
    break;     
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    #ifdef USE_MODULE_CORE_FILESYSTEM
    #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
    case TASK_FILESYSTEM__RESET__MODULE_DATA__ID:
      Default_Module();
    break;
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
      // Load_Module();
    break;
    #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
    #endif // USE_MODULE_CORE_FILESYSTEM
    /************
     * MQTT SECTION * 
    *******************/
    case TASK_MQTT_SUBSCRIBE:
      MQTTSubscribe();
    break;
  } // END switch

} // END function

/**
 * @brief Should be called from Init now, its the new way of setting up as a new device with default settings
 * 
 */
void mMQTTManager::Default_Module()
{
  ALOG_INF(PSTR(D_LOG_MQTT "Default_Module"));
  
  // memset((uint8_t*)&dt, 0, sizeof(dt));

  // MODULE_STORAGE::CONNECTION* con = &dt.connection[0];

  
  dt.ifchanged_secs = SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS; // ifchanged etc timing should be moved into mqtt substruct
  dt.ifchanged_json_level = JSON_LEVEL_IFCHANGED; //default
  dt.teleperiod_secs = 120;
  dt.teleperiod_json_level = JSON_LEVEL_DETAILED; //default
  dt.mqtt_retain = 1;// = JSON_METHOD_SHORT; //default
  dt.configperiod_secs = SEC_IN_HOUR;

  #ifdef ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
  dt.options.unified_module_interface_reporting__suppress_submodule_configperiod = 1;
  dt.options.unified_module_interface_reporting__suppress_submodule_teleperiod = 1;
  dt.options.unified_module_interface_reporting__suppress_submodule_ifchangedperiod = 1;
  #else
  dt.options.unified_module_interface_reporting__suppress_submodule_configperiod = 0;
  dt.options.unified_module_interface_reporting__suppress_submodule_teleperiod = 0;
  dt.options.unified_module_interface_reporting__suppress_submodule_ifchangedperiod = 0;
  #endif


  strlcpy(test, "TEST MESSAGE FROM INIT", sizeof(test));

  uint8_t idx = 0;

  /**
   * @brief Init with a wifi one
   **/
  Default_Module__Connection_WiFi();

  
  // strlcpy(dt.connection[idx].host_address, MQTT_HOST, sizeof(dt.connection[idx].host_address));
  // dt.connection[idx].port = MQTT_PORT;
  // strlcpy(dt.connection[idx].user, MQTT_USER, sizeof(dt.connection[idx].user));
  // strlcpy(dt.connection[idx].pwd, MQTT_PASS, sizeof(dt.connection[idx].pwd));
  // dt.connection[idx].retry = MQTT_RETRY_SECS;
  // snprintf_P(dt.connection[idx].client, sizeof(dt.connection[idx].client), PSTR("%s-%s"), tkr_set->Settings.system_name.device, WiFi.macAddress().c_str());
  // strlcpy(dt.connection[idx].prefixtopic, tkr_set->Settings.system_name.device, sizeof(dt.connection[idx].prefixtopic));
  // dt.connection[idx].status = 1;
    
  // ALOG_INF(PSTR("Ghost_address: %s"), dt.connection[idx].host_address);
  // ALOG_INF(PSTR("port: %d"), dt.connection[idx].port);
  // ALOG_INF(PSTR("user: %s"), dt.connection[idx].user);
  // ALOG_INF(PSTR("pwd: %s"), dt.connection[idx].pwd);
  // ALOG_INF(PSTR("retry: %d"), dt.connection[idx].retry);
  // ALOG_INF(PSTR("client: %s"), dt.connection[idx].client);
  // ALOG_INF(PSTR("prefixtopic: %s"), dt.connection[idx].prefixtopic);
  // ALOG_INF(PSTR("status: %d"), dt.connection[idx].status);

}


void mMQTTManager::Default_Module__Connection_WiFi()
{

  uint8_t broker_index_next = brokers.size();

  uint8_t idx = broker_index_next; //size with index from 0, will already give next slot

  // pCONT_mqtt->CreateConnection(mqtt_client, MQTT_HOST, MQTT_PORT, CLIENT_TYPE_WIFI_ID, pCONT_mqtt->dt.connection[idx].client, pCONT_mqtt->dt.connection[idx].prefixtopic);
      

  // MQTTConnection* con = brokers[idx];

  // strlcpy(con->host_address, MQTT_HOST, sizeof(con->host_address));
  // con->port = MQTT_PORT;
  // strlcpy(con->user, MQTT_USER, sizeof(con->user));
  // strlcpy(con->pwd, MQTT_PASS, sizeof(con->pwd));
  // con->retry = MQTT_RETRY_SECS;
  // snprintf_P(con->client_name, sizeof(con->client_name), PSTR("%s-%s"), tkr_set->Settings.system_name.device, WiFi.macAddress().c_str());
  // strlcpy(con->prefix_topic, tkr_set->Settings.system_name.device, sizeof(con->prefix_topic));
  // con->status = 1;
    
  // ALOG_INF(PSTR("Ghost_address: %s"), con->host_address);
  // ALOG_INF(PSTR("port: %d"), con->port);
  // ALOG_INF(PSTR("user: %s"), con->user);
  // ALOG_INF(PSTR("pwd: %s"), con->pwd);
  // ALOG_INF(PSTR("retry: %d"), con->retry);
  // ALOG_INF(PSTR("client: %s"), con->client_name);
  // ALOG_INF(PSTR("prefixtopic: %s"), con->prefix_topic);
  // ALOG_INF(PSTR("status: %d"), con->status);



}




#ifdef USE_MODULE_CORE_FILESYSTEM
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT

void mMQTTManager::Save_Module()
{
  ALOG_INF(PSTR(D_LOG_MQTT "Save_Module"));
  pCONT_mfile->ByteFile_Save("/mqtt" FILE_EXTENSION_BIN, (uint8_t*)&dt, sizeof(dt));
}

void mMQTTManager::Load_Module(bool erase)
{
  ALOG_INF(PSTR(D_LOG_MQTT "Load_Module"));
  // pCONT_mfile->ByteFile_Load("/mqtt" FILE_EXTENSION_BIN, (uint8_t*)&dt, sizeof(dt));
}

#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
#endif // USE_MODULE_CORE_FILESYSTEM




void mMQTTManager::MQTTSubscribe()
{

  // Probably needs moved into main parsing, if this does not need redoing with each new connection, parsing it once may be enough. Or else, reparsing again.
  #ifdef USE_FUNCTION_TEMPLATE
  uint16_t progmem_size = sizeof(FUNCTION_TEMPLATE);
  char buffer[progmem_size];
  memcpy_P(buffer,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));

  JsonParser parser(buffer);
  JsonParserObject rootObj = parser.getRootObject();   
  if (!rootObj) 
  {
    ALOG_ERR(PSTR("DeserializationError with \"%s\""), buffer);
    return;
  } 
  else
  {
    // ALOG_DBG(PSTR("Deserialization Success with \"%s\""), buffer);
  }
  
  JsonParserToken jtok = 0;
  JsonParserObject obj1 = 0;
  char item_name[100] = {0};

  // ALOG_INF(PSTR("ObjectNameID size %d"), rootObj["ObjectNameID"].size());

  if(jtok = rootObj["MQTTSubscribe"])
  {

    for(auto val : jtok.getArray()) 
    {
      const char* val_string = val.getStr();
      ALOG_HGL(PSTR("val = %s"), val_string);
  
      Subscribe(val_string, 0);

    }
      
  }

  #endif // USE_FUNCTION_TEMPLATE



}

/**
 * @brief 
 * New methods to get the periods indirectly, so other logic can modify them
 * SubModule: When interface is set to periority, the submodule values will be supressed
 */
uint16_t mMQTTManager::GetConfigPeriod()
{
  return dt.configperiod_secs;
}
uint16_t mMQTTManager::GetConfigPeriod_SubModule()
{
  if(dt.options.unified_module_interface_reporting__suppress_submodule_configperiod)
    return 3600; // disable submodule config
  else
    return dt.configperiod_secs;
}
uint16_t mMQTTManager::GetTelePeriod()
{
  return dt.teleperiod_secs;
}
uint16_t mMQTTManager::GetTelePeriod_SubModule()
{
  if(dt.options.unified_module_interface_reporting__suppress_submodule_teleperiod)
    return 3600; // reduced to once an hour
  else
    return dt.teleperiod_secs;  
}
uint16_t mMQTTManager::GetIfChangedPeriod()
{
  return dt.ifchanged_secs;
}
uint16_t mMQTTManager::GetIfChangedPeriod_SubModule()
{
  if(dt.options.unified_module_interface_reporting__suppress_submodule_ifchangedperiod)
    return 0; // disable submodule ifchanged
  else
    return dt.ifchanged_secs;  
}




void mMQTTManager::Load_New_Subscriptions_From_Function_Template()
{

  DEBUG_LINE_HERE

  #ifdef USE_FUNCTION_TEMPLATE  
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr, FUNCTION_TEMPLATE, sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK  "Load_New_Subscriptions_From_Function_Template READ = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);
  #endif //USE_FUNCTION_TEMPLATE

  JsonParser parser(data_buffer.payload.ctr);
  JsonParserObject obj = parser.getRootObject();   
  JsonParserToken jtok = 0; 

  if(jtok = obj["MQTTSubscribe"])
  {
    if(jtok.isArray())
    {

      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) 
      {
        const char* new_topic = v.getStr();
        ALOG_DBM(PSTR("New Subscribe = \"%s\""), new_topic);
        

      }

    }

  }
}


void mMQTTManager::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  uint8_t connection_idx = 0; // should be like Segments, assumes 0 when only 1

  if(jtok = obj["MQTT"].getObject()["RetrySecs"])
  {
    // dt.connection[0].retry = jtok.getInt();
  }

  if(jtok = obj["MQTTSend"])
  {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("mMQTTManager::parse_JSONCommand MQTTSend"));
    #endif //
    JsonParserToken jtok_topic = jtok.getObject()["Topic"];
    JsonParserToken jtok_payload = jtok.getObject()["Payload"];

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("mMQTTManager::parse_JSONCommand MQTTSend %d"),jtok_topic.size());
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("mMQTTManager::parse_JSONCommand MQTTSend %d"),jtok_payload.size());
    #endif // 

    char topic_ctr[100] = {0};
    char payload_ctr[300] = {0};

    snprintf(topic_ctr, sizeof(topic_ctr), jtok_topic.getStr());
    snprintf(payload_ctr, sizeof(payload_ctr), jtok_payload.getStr());

    char buffer_escaped[200] = {0};
    uint8_t len  = 0;
    for(int i=0;i<strlen(payload_ctr);i++){
      if(payload_ctr[i] == '~'){
        len+=sprintf(buffer_escaped+len,"\"");
      }else{    
        buffer_escaped[len++] = payload_ctr[i];
      }
    }

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("Topic=%s"),topic_ctr);
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("Payload=%s"),payload_ctr);
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("buffer_escaped=%s"),buffer_escaped);
    #endif // ENABLE_LOG_LEVEL_INFO

    // publish_device(jtok_topic.getStr(),jtok_payload.getStr(),false);
    brokers[0]->pubsub->publish(topic_ctr,buffer_escaped,false);

  }


  if(jtok = obj["MQTT"].getObject()["StatusAll"]) //change all to be value
  {    
    pCONT->Tasker_Interface(TASK_MQTT_STATUS_REFRESH_SEND_ALL);
  }


  JsonParserToken jtok_sub = 0; 
  if(jtok = obj["MQTTUpdateSeconds"])
  {
    if(jtok_sub = jtok.getObject()["IfChanged"])
    {
      dt.ifchanged_secs = jtok_sub.getInt();
      ALOG_TST(PSTR("MQTTUpdateSeconds IfChanged %d"), dt.ifchanged_secs);
    }
    if(jtok_sub = jtok.getObject()["TelePeriod"])
    {
      dt.teleperiod_secs = jtok_sub.getInt();
      ALOG_TST(PSTR("MQTTUpdateSeconds TelePeriod %d"), dt.teleperiod_secs);
    }
    if(jtok_sub = jtok.getObject()["ConfigPeriod"])
    {
      dt.configperiod_secs = jtok_sub.getInt();
      ALOG_TST(PSTR("MQTTUpdateSeconds ConfigPeriod %d"), dt.configperiod_secs);
    }
    pCONT->Tasker_Interface(TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD);
  }

}//end function


void mMQTTManager::Init(void)
{
  Default_Module();
  
}



void mMQTTManager::CallMQTTSenders()
{

  if(brokers.size())  
      // if(brokers_active)   
  {
    if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    {
      pCONT->Tasker_Interface(TASK_MQTT_SENDER);
    }
  }

}

/**
 * @brief Transmit on any connected brokers with connection checking
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
boolean mMQTTManager::Publish(const char* topic, const char* payload, boolean retained)
{
  
  if(brokers.size())
      // if(brokers_active)   
  {
    if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    {
      for (auto& broker : brokers)
      {
        if(broker->pubsub->connected())
        {
          return broker->publish_device(topic,payload,retained);
        }
      }
    }
  }

}



/**
 * @brief Transmit on any connected brokers with connection checking
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
boolean mMQTTManager::Subscribe(const char* topic, uint8_t qos)
{
  
  if(brokers.size())
      // if(brokers_active)   
  {
    // if(brokers[0]->uptime_seconds && brokers[0]->downtime_counter==0)
    // {
      for (auto& broker : brokers)
      {
        if(broker->pubsub->connected())
        {
          return broker->pubsub->subscribe(topic);
        }
      }
    }
  // }

}


/**
 * @brief This is not thread safe
 * 
 * @return const char* 
 */
const char* mMQTTManager::GetState_PCtr(int8_t state)
{
  switch(state){
    case MQTT_CONNECTION_TIMEOUT:       return PSTR("Connection Timeout");
    case MQTT_CONNECTION_LOST:          return PSTR("Connecttion Lost");
    case MQTT_CONNECT_FAILED:           return PSTR("Connect Failed");
    case MQTT_DISCONNECTED:             return PSTR("Disconnected");
    case MQTT_CONNECTED:                return PSTR("Connected");
    case MQTT_CONNECT_BAD_PROTOCOL:     return PSTR("Bad Protocol");
    case MQTT_CONNECT_BAD_CLIENT_ID:    return PSTR("Bad Client ID");
    case MQTT_CONNECT_UNAVAILABLE:      return PSTR("Unavailable");
    case MQTT_CONNECT_BAD_CREDENTIALS:  return PSTR("Bad Credentials");
    case MQTT_CONNECT_UNAUTHORIZED:     return PSTR("Unauthorized");
  }
}

#endif // USE_MODULE_NETWORK_MQTT