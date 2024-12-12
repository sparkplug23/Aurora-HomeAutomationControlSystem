#include "mMQTT.h"

#ifdef USE_MODULE_NETWORK_MQTT

void MQTTConnection::MqttConnected(void)
{
  
  connected = true;
  retry_counter = 0;
  cConnectionAttempts = 0; // reset

  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr, PM_MQTT_LWT_PAYLOAD_FORMATED, pCONT_sup->GetResetReason().c_str(), pCONT_time->GetUptime().c_str() );
  
  #ifdef ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT // Show disconnect occured if we have reconnected inside timeout
    char lwt_topic[40];
    snprintf_P(lwt_topic, sizeof(lwt_topic), PSTR("%s/status/LWT"), prefix_topic);
    pubsub->publish(lwt_topic, lwt_message_ondisconnect_ctr, true); // onconnect message
    delay(100);
  #endif
  
  Send_LWT_Online();
    
  // Group name for setting all devices
  subscribe(PSTR("group_all/#"));
    
  // subscribe("<devicename>/set/#");
  subscribe_device(PSTR(D_MQTT_COMMAND "/#"));
  
  flag_start_reconnect = false;
  
  #ifndef ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
  pCONT->Tasker_Interface(TASK_MQTT_CONNECTED);
  pCONT->Tasker_Interface(TASK_MQTT_SUBSCRIBE);
  pCONT->Tasker_Interface(TASK_MQTT_STATUS_REFRESH_SEND_ALL);
  #endif

}


void MQTTConnection::Send_LWT_Online()
{
  char lwt_topic[80]; snprintf_P(lwt_topic, sizeof(lwt_topic), PM_MQTT_LWT_TOPIC_FORMATED, prefix_topic);
  char payload[40];   snprintf_P(payload, sizeof(payload), PM_MQTT_LWT_PAYLOAD_ONLINE); // Required for ESP8266
  pubsub->publish(lwt_topic, payload, true);
}
    

void MQTTConnection::EverySecond()
{
  
  #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
  ALOG_HGL(PSTR("MQTTConnection::CheckConnection \"%s\""), host_address);
  #endif
  
  if (MqttIsConnected()==false) 
  {

    #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    ALOG_INF(PSTR("MqttIsConnected == FALSE"));
    #endif

    pCONT_set->runtime.global_state.mqtt_down = 1;
    uptime_seconds = 0;
    downtime_counter++;

    if(retry_counter == 0)
    {
      ALOG_INF(PSTR("reconnect disabled, will be enabled elsewhere"));
    }
    else
    if (retry_counter==1) 
    {
      ALOG_INF(PSTR("retry_counter==1"));
      MqttReconnect();
    } 
    else 
    {
      retry_counter--;
      ALOG_INF( PSTR("retry_counter=%d"), retry_counter );
    }

  } 
  else 
  {
    #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    ALOG_INF(PSTR("MqttIsConnected == TRUE"));
    #endif 
    pCONT_set->runtime.global_state.mqtt_down = 0;
    downtime_counter = 0;
    uptime_seconds++;
  }  

}


void MQTTConnection::MqttReconnect(void){ DEBUG_PRINT_FUNCTION_NAME;
  
  uint32_t before_millis = millis();

  ALOG_HGL(PSTR(D_LOG_MQTT D_ATTEMPTING_CONNECTION " to \"%s:%d\""), host_address, port);
  
  connected = false;
  retry_counter = retry_counter_start_value;// pCONT_mqtt->dt.connection[0].retry;
  pCONT_set->runtime.global_state.mqtt_down = 1;

  if(pubsub!=nullptr)
  {
    if (pubsub->connected()) { pubsub->disconnect(); }
  }  

  pubsub->setCallback(
    [this](char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){
      this->MqttDataHandler(mqtt_topic, mqtt_data, data_len);
    }
  );

  pubsub->setServer(host_address, port); // main broker
  
  // Generate will message
  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr, PM_MQTT_LWT_PAYLOAD_FORMATED, pCONT_sup->GetResetReason().c_str(), pCONT_time->GetUptime().c_str());

  char lwt_topic[40];
  snprintf_P(lwt_topic, sizeof(lwt_topic), PM_MQTT_LWT_TOPIC_FORMATED, pCONT_set->Settings.system_name.device);

  uint8_t loglevel = LOG_LEVEL_INFO;
  #ifdef ENABLE_DEVFEATURE_DEBUG_MQTT_RECONNECT
  loglevel = LOG_LEVEL_TEST;
  #endif
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(loglevel, PSTR("client_name = %s"), client_name);
  AddLog(loglevel, PSTR("lwt_topic = %s"), lwt_topic);
  AddLog(loglevel, PSTR("lwt_message_ondisconnect_ctr = %s"), lwt_message_ondisconnect_ctr);
  #endif// ENABLE_LOG_LEVEL_INFO

  ALOG_INF(PSTR("mMQTTManager::MqttReconnect START              Connect"));

  if(pubsub->connect(client_name, lwt_topic, WILLQOS_CTR, WILLRETAIN_CTR, lwt_message_ondisconnect_ctr)){  //boolean connect (clientID, willTopic, willQoS, willRetain, willMessage)

    ALOG_INF(PSTR("mMQTTManager::MqttReconnect Connected"));
    
    MqttConnected();
    
    flag_start_reconnect = false;
    retry_counter = 0;    // disables it running again

    downtime_counter = 0; // reset

  }
  else
  { 
    ALOG_INF(PSTR("mMQTTManager::MqttReconnect Failed %d"),pubsub->state());
    MqttDisconnected(pubsub->state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
  }
  
  uint32_t elapsed_millis = millis() - before_millis;

  ALOG_INF(PSTR("MqttReconnect ElapsedTime = %d"), elapsed_millis);

} // END function


void MQTTConnection::MqttDisconnected(int state)
{
  DEBUG_PRINT_FUNCTION_NAME;
  
  cConnectionAttempts++;    

  ALOG_DBG(PSTR(D_LOG_MQTT "Connection FAILED, state = [%d]"), pubsub->state());

  connected = false;
  retry_counter = retry_counter_start_value;
  
}


void MQTTConnection::MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len)
{ 
  
  if (data_len >= DATA_BUFFER_PAYLOAD_MAX_LENGTH) 
  {  
    ALOG_INF(PSTR("MqttDataHandler"));
    return;
  }
  
  if(requestDataBufferLock(D_UNIQUE_MODULE_NETWORK_MQTT_ID))
  {

    D_DATA_BUFFER_SOFT_CLEAR();

    data_buffer.topic.length_used = strlen(mqtt_topic);
    strlcpy(data_buffer.topic.ctr, mqtt_topic, data_buffer.topic.length_used);
    data_buffer.topic.ctr[data_buffer.topic.length_used] = '\0'; // null terminate

    data_buffer.payload.length_used = data_len;
    memcpy(data_buffer.payload.ctr, mqtt_data, data_buffer.payload.length_used);
    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // null terminate

    if(data_len){
      data_buffer.flags.waiting = true;
    }

    if(data_buffer.flags.waiting)
    {
      data_buffer.flags.waiting = false;
      // if (LOG_LEVEL_DEBUG_MORE <= pCONT_set->Settings.logging.serial_level) {
        LoggingLevels level = LOG_LEVEL_DEBUG_MORE;
        #ifdef ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS
        level = LOG_LEVEL_TEST;
        #endif
        #ifdef ENABLE_LOG_LEVEL_INFO
          AddLog(level, PSTR(D_LOG_MQTT "<-- Topic   [len:%d] %s"), data_buffer.topic.length_used,  data_buffer.topic.ctr);
          AddLog(level, PSTR(D_LOG_MQTT "<-- Payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
        #endif// ENABLE_LOG_LEVEL_INFO
      // }

      data_buffer.isserviced = 0;

      #ifdef ENABLE_DEVFEATURE_MQTT__ESTIMATED_INCOMING_COMMANDS_AND_REPORT_ISSERVICED
      uint16_t estimated_commands = JBI->estimateJsonKeyValuePairs(data_buffer.payload.ctr, data_buffer.payload.length_used);
      // ALOG_INF(PSTR(D_LOG_MQTT "<-- Payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
      ALOG_DBM(PSTR("estimated_commands %d"), estimated_commands);
      #endif

      pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);
      
      ALOG_DBM( PSTR(D_LOG_MQTT "{\"CommandsMatched\":%d}"), data_buffer.isserviced);

      if(data_buffer.isserviced != estimated_commands)
      {
        uint8_t log_level = LOG_LEVEL_WARNING;
        #ifdef USE_MODULE_DISPLAYS_NEXTION
        log_level = LOG_LEVEL_DEBUG_MORE; // Don't report when nextion is active
        #endif
        AddLog(log_level, PSTR("Commands mismatch %d/%d"), data_buffer.isserviced, estimated_commands);
      }
      
    }
    releaseDataBufferLock();

  }

}

boolean MQTTConnection::subscribe(const char* topic) {  
  char ttopic[70] = {0};
  strncpy_P(ttopic, topic, strlen(topic));
  ALOG_INF( PSTR("Subscribing to \"%s\""), ttopic );
  if(pubsub)
    return pubsub->subscribe(ttopic, 0); // Expects topic in RAM (not PSTR)
}

boolean MQTTConnection::subscribe_device(const char* topic) {
  char ttopic[70] = {0};
  sprintf(ttopic, PSTR("%s/%s"), prefix_topic, topic);
  ALOG_INF( PSTR("Subscribing to \"%s\""), ttopic );
  return pubsub->subscribe(ttopic, 0);
}


/**
 * @brief 
 * @param client_in 
 */
void MQTTConnection::SetPubSubClient(Client* client_in)
{
  pubsub = new PubSubClient(*client_in);
  flag_start_reconnect = true;
}


void MQTTConnection::Send_Prefixed_P(const char* topic, PGM_P formatP, ...)
{

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
  va_end(arg);

  publish_device(topic, data_buffer.payload.ctr, false);

}


/**
 * @brief 
 * 
 * @param topic_type 
 * @param module_id 
 * @param postfix_topic_ctr 
 * @return true  if packet was sent
 * @return false if not successful
 */
bool MQTTConnection::MQTTHandler_Send_Formatted_UniqueID(uint8_t topic_type, uint16_t unique_id, const char* postfix_topic_ctr, bool retain_flag)
{

  PGM_P module_ctr = pCONT->GetModuleName(unique_id);

  #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  Serial.printf("buffer length = %d\n\r", strlen(data_buffer.payload.ctr));
  #endif

  bool sent_status = false;

  sent_status =  publish_ft(module_ctr,
             topic_type,
             postfix_topic_ctr,
             data_buffer.payload.ctr,
             retain_flag
            );

  if(sent_status)
  {
    tSaved_LastOutGoingTopic = millis();
  }

  return sent_status;

}


bool MQTTConnection::publish_ft(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic    [100]; memset(topic,0,sizeof(topic));
  char topic_type[30]; memset(topic_type,0,sizeof(topic_type));

  if(topic_type_id == MQTT_TOPIC_LWT_ONLINE_ID)
  {
    snprintf_P(topic, sizeof(topic), "%s/LWT", D_TOPIC_STATUS);
  }
  else
  {    
    switch(topic_type_id){
      default:    
      case MQTT_TOPIC_TYPE_IFCHANGED_ID:  sprintf(topic_type,"ifchanged/"); break;
      case MQTT_TOPIC_TYPE_ROC1M_ID:      sprintf(topic_type,"roc1m/");     break;
      case MQTT_TOPIC_TYPE_ROC10M_ID:     sprintf(topic_type,"roc10m/");    break;
      case MQTT_TOPIC_TYPE_TELEPERIOD_ID: sprintf(topic_type,"tele/");      break;
      case MQTT_TOPIC_TYPE__DEBUG__ID:    sprintf(topic_type,"debug/");     break;
      case MQTT_TOPIC_TYPE_SYSTEM_ID:      /*** none ***/                   break;
    }
    snprintf_P(topic, sizeof(topic), "%s/%s/%s%S", D_TOPIC_STATUS, module_name, topic_type, topic_postfix); // PSTR around string will crash
  }

  #ifdef ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  ALOG_INF( PSTR(D_LOG_MQTT "topic=\"%s\""), topic );
  #endif
  #ifdef ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRASNMITTED
  ALOG_INF( PSTR(D_LOG_MQTT "payload=\"%s\""), payload_ctr );
  #endif
  
  return publish_device(topic, payload_ctr, retain_flag);

}

 
void MQTTConnection::publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  sprintf_P(topic_ctr,PSTR("%s/%s/%s"),D_TOPIC_STATUS,module_name,topic_postfix); 
  
  publish_device(topic_ctr,payload_ctr,retain_flag);

}


boolean MQTTConnection::publish_device(const char* topic, const char* payload, boolean retained)
{

  /**
   * @brief THIS SHOULD BE THE ONLY checks of network.
   * In reality wifi/cellular should always be checking itself directly
   * 
   */
  #ifdef ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
  if(network_client->connected())
  {
    #ifdef ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    ALOG_ERR(PSTR(D_LOG_PUBSUB "MQTTConnection::ppublish::network->Connected"));
    #endif
  }else{
    ALOG_ERR(PSTR(D_LOG_PUBSUB "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- Unable to publish, No Network connection B"));
    flag_start_reconnect = true;
    return false;
  }
  #endif // ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS

  if(!strlen(payload))
  {
    ALOG_ERR(PSTR(D_LOG_PUBSUB "Unable to publish, empty payload"));
    return false;
  }

  char convctr[100] = {0};
  snprintf(convctr, sizeof(convctr), PSTR("%s/%S"), prefix_topic, topic);
  
  ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_TOPIC   "%d [%s]"), strlen(convctr), convctr);
  ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_PAYLOAD "%d [%s]"), strlen(payload), payload);

  return pubsub->publish(convctr, (const uint8_t*)payload, strlen(payload), retained);

}


/**
 * @brief progmem payload version  My function for adding prefix by device name
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
boolean MQTTConnection::publish_device_P(const char* topic, const char* payload, boolean retained){

  char convctr[100] = {0};
  snprintf(convctr,sizeof(convctr),PSTR("%s/%S"), prefix_topic, topic);
  return pubsub->publish_P(convctr, payload, retained);
    
}

#endif // USE_MODULE_NETWORK_MQTT