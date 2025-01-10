#ifndef _MMQTT
#define _MMQTT 0.7

#define D_UNIQUE_MODULE_NETWORK_MQTT_ID  3010 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/mFirmwareDefaults.h"

#ifdef USE_MODULE_NETWORK_MQTT
#include <stdint.h>

#include "2_CoreSystem/mGlobalMacros.h"



// Void Arg Functions
#define CALL_MEMBER_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))


#ifdef ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
#define CODE_BLOCK__MQTTHandler_AddWebURL_PayloadRequests()  \
  char uri_buffer[70] = {0};\
  snprintf(uri_buffer, sizeof(uri_buffer), "/mqtt/%s/%S", D_TOPIC_STATUS, GetModuleFriendlyName());\
  pCONT_web->server->on(uri_buffer, HTTP_GET,\
  [this](AsyncWebServerRequest *request)\
    {\
      char handle_url[100] = {0};       \
      for(auto& connection:pCONT_mqtt->brokers)\
        for(auto& handle:mqtthandler_list)\
        { \
          connection->publish_ft(GetModuleFriendlyName(), handle->topic_type, handle->postfix_topic, handle_url, sizeof(handle_url)); \
          ALOG_INF(PSTR("handle_url \"%s\" -> \"%s\""), request->url().c_str(), handle_url);\
          const String& incoming_uri = request->url();\
          if(incoming_uri.indexOf(handle_url) > 0)\
          {        \
            uint8_t fSendPayload = CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);\
            ALOG_INF(PSTR("data_buffer=%s"), data_buffer.payload.ctr);\
            request->send(200, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, data_buffer.payload.ctr); \
            break;\
          }\
        }\
      }\
    }\
  );
#endif // ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL


enum TOPIC_TYPE_IDS
{
  MQTT_TOPIC_TYPE_SYSTEM_ID=0, 
  MQTT_TOPIC_TYPE_IFCHANGED_ID,
  MQTT_TOPIC_TYPE_ROC1M_ID,
  MQTT_TOPIC_TYPE_ROC10M_ID,
  MQTT_TOPIC_TYPE_SENSOR_TELEPERIOD_ID,   // 1 second
  MQTT_TOPIC_TYPE_SETTINGS_TELEPERIOD_ID, //show settings, but 1 hour increments
  MQTT_TOPIC_LWT_ONLINE_ID,
  MQTT_TOPIC_TYPE_TELEPERIOD_ID,
  MQTT_TOPIC_TYPE__DEBUG__ID,  
  MQTT_TOPIC_TYPE_LENGTH_ID
};

enum MQTT_FREQUENCY_REDUCTION_LEVEL_IDS
{
  MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID = 0, // leave it as original set
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_1_MINUTES_ID = 1,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID = 2,
  MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_60_MINUTES_ID = 3,
  MQTT_FREQUENCY_REDUCTION_LEVEL_LENGTH_ID = 4
};

// Put all shared ones here, eg "settings" and "sensor", with specific inside module.h when needed
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LWT_ONLINE_CTR)   "LWT";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR)       "health";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR)     "settings";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR)          "log";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR)     "firmware";        
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR)       "memory";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR)      "network";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR)         "mqtt";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR)         "time";     
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR)       "reboot";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR) "reboot/event";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_DEVICES_CTR)      "debug/devices";   
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR)           "debug/pins";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR)       "debug/template";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR) "debug/moduleinterface";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR) "debug/stored_settings";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR)  "debug/minimal";    
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TASKER_INTERFACE_PERFORMANCE) "debug/tasker_interface_performance";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SETTINGS_STORAGE_CTR) "debug/settings_storage";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR) "sensors";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR)   "power";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LIGHTSTATE_CTR)   "light/state";

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR) "state";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_TIMED_CTR) "timed";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR) "debug";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_CTR) "motion";

DEFINE_PGM_CTR(PM_MQTT_LWT_PAYLOAD_ONLINE) "{\"LWT\":\"Online\"}";
DEFINE_PGM_CTR(PM_MQTT_LWT_PAYLOAD_FORMATED)  "{\"LWT\":\"Offline\",\"ResetReason\":\"%s\",\"Uptime\":\"%s\"}";
DEFINE_PGM_CTR(PM_MQTT_LWT_TOPIC_FORMATED)    "%s/status/LWT";


typedef union {
  uint8_t data;
  struct {
    uint8_t PeriodicEnabled : 1; 
    uint8_t SendNow         : 1;
    uint8_t FrequencyRedunctionLevel  : 2;     // 0= no redunction, maintain level, 1 = reduce after 1 minute, 1= 10 minutes, 2= 60 minutes    
    uint8_t retain : 1;
    uint8_t reserved : 3; 
  };
}Handler_Flags;

template <typename Class>
struct handler {
  uint32_t      tSavedLastSent = 0;
  uint16_t      tRateSecs = 1;
  uint8_t       json_level = 0;
  uint8_t       topic_type = 0;
  const char*   postfix_topic = nullptr;
  #ifdef ENABLE_PHASEOUT__MQTT_HANDLER_ID
  uint8_t       handler_id = 0; //phase out, this is not used for anything
  #endif
  Handler_Flags flags = {0};
  uint8_t       (Class::*ConstructJSON_function)(uint8_t json_level, bool json_appending); // member-function to sender with two args. Extra "json_appending" will allow calling constructjsons directly and adding them to another without closing the main json object
};

#include "PubSubClient.h"
class PubSubClient;

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>

#include "1_TaskerManager/mTaskerInterface.h"
#define WILLQOS_CTR 2
#define WILLRETAIN_CTR true

enum ConnectionClient_t
{
  CLIENT_TYPE_WIFI_ID = 0,
  CLIENT_TYPE_ETHERNET_ID,
  CLIENT_TYPE_CELLULAR_ID
};


class MQTTConnection
{
  public:
  
    char      host_address[33];
    uint8_t   status = 0;
    uint16_t  port;
    char      user[33];
    char      password[33];
    char      prefix_topic[50]; // "<devicename>/"
    char      client_name[80]; 
    uint16_t  retry = 5; //default

    Client* network_client = nullptr;
    PubSubClient* pubsub = nullptr; //to be made private

    ConnectionClient_t client_type = CLIENT_TYPE_WIFI_ID; //default wifi, 1 ethernet, 2 cellular   
    uint32_t tSaved_LastOutGoingTopic = 0; //this needs to become inside instance, so multiple connections dont block others

    uint16_t connect_count = 0;            // MQTT re-connect count
    uint16_t retry_counter = 1;            // MQTT connection retry counter
    uint16_t retry_counter_start_value = 5; // will be set 5 for wifi, 60 for LTE    
    uint16_t downtime_counter = 0;
    uint32_t uptime_seconds = 0;
    bool connected = false;                // MQTT virtual connection status
    bool allowed = false;                  // MQTT enabled and parameters valid
    bool mqtt_tls = false;                 // MQTT TLS is enabled
    bool flag_start_reconnect = false;
    uint8_t cConnectionAttempts = 0; 
    uint8_t host_server_type = 0;


    /**
     * @brief 
     * 0: Disabled
     * 1: Enabled and connected
     * 2: Enabled, not connected
     *  
     */
    struct interface_priority_flags_s{
      uint8_t energy = 0;
      uint8_t light = 0;
    }interface_reporting_priority;
  

    struct DEBUG{
      #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
      uint32_t payload_publish_sent = 0;
      uint32_t payload_publish_missed = 0;
      float    payload_publish_success_percentage = 0;
      #endif
    }debug_stats;


    /**
     * @brief Minimal for new connection
     * 
     * @param client_in 
     * @param host_address 
     * @param _port 
     * @param _type 
     */
    MQTTConnection(Client* _client, char* _host_address, uint16_t _port, ConnectionClient_t _type)
    {
      network_client = _client;
      SetPubSubClient(_client);
      port = _port;
      snprintf(host_address, sizeof(host_address), _host_address);
      client_type = _type;
      Serial.printf("MQTT::CreateConnection broker_url B %s\n\r", host_address); Serial.flush();
    };


    MQTTConnection(IPAddress addr, uint16_t port, Client& client)
    {
      Serial.println("Creating new PubSubClient");
      pubsub = new PubSubClient(addr, port, client);
    };


    void SetCredentials(char* _user, char* _password)
    {
      if((strlen(_user) > sizeof(user)) || (strlen(_password) > sizeof(password))) 
      {
      Serial.println("Error");
      return;
      }
          
      strlcpy(user, _user, sizeof(user));
      strlcpy(password, _password, sizeof(password));
    }


    void SetReConnectBackoffTime(uint8_t _sec)
    {
      retry = _sec;
    }


    void SetClientName(char* _client_name)
    {
      snprintf(client_name, sizeof(client_name), _client_name);
    }


    void SetTopicPrefix(char* _prefix_topic)
    {
      snprintf(prefix_topic, sizeof(prefix_topic), _prefix_topic);
    }


    void TaskerHandlers();

    void EverySecond();

    void EveryLoop(){}

    void Send_LWT_Online();
    
    bool MqttIsConnected()
    {
      return pubsub->connected();
    }

    // Faster than doing socket open, try this prior to sending
    bool NetworkConnected()
    {
      if(!network_client) return false;
      return network_client->connected();
    }
    
    
    void MqttConnected(void);
    boolean subscribe_device(const char* topic);
    boolean subscribe(const char* topic);
    void MqttReconnect();  
    void MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len);     
    void MqttDisconnected(int state);
    void SetPubSubClient(Client* client);
    bool MQTTHandler_Send_Formatted_UniqueID(uint8_t topic_type, uint16_t unique_id, const char* postfix_topic_ctr, bool retain_flag = false);
    void Send_Prefixed_P(const char* topic, PGM_P formatP, ...);
    bool publish_ft( const char* module_name,uint8_t topic_type_id, const char* topic_postfix, const char* payload, uint8_t retain_flag);
    void publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag);

    boolean publish_device(const char* topic, const char* payload, boolean retained = false);
    boolean publish_device_P(const char* topic, const char* payload, boolean retained = false);
    
};



class mMQTTManager :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mMQTTManager(){};
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    char test[30];

    static constexpr const char* PM_MODULE_NETWORK_MQTT_CTR = D_MODULE_NETWORK_MQTT_CTR;;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_MQTT_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_MQTT_ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      // uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Default_Module(void);
    #if defined(ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT) && defined(USE_MODULE_CORE_FILESYSTEM)
    void Load_Module(bool erase = false);
    void Save_Module(void);
    #endif // USE_MODULE_CORE_FILESYSTEM

    typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
      uint8_t data;                           // Allow bit manipulation using SetOption
      struct {                                
        uint8_t unified_module_interface_reporting__suppress_submodule_configperiod : 1;               // bit 0 - SetOption0  - If unified sensor interface is reporting readings, then optional disable or reduce ifchanged from submodule to reduce mqtt traffic
        uint8_t unified_module_interface_reporting__suppress_submodule_teleperiod : 1;               // bit 0 - SetOption0  - If unified sensor interface is reporting readings, then optional disable or reduce ifchanged from submodule to reduce mqtt traffic
        uint8_t unified_module_interface_reporting__suppress_submodule_ifchangedperiod : 1;               // bit 0 - SetOption0  - If unified sensor interface is reporting readings, then optional disable or reduce ifchanged from submodule to reduce mqtt traffic
        uint8_t reserved : 6;          // bit 1              - SetOption1  - 
      };
    } SysBitfield_Options;


    struct MODULE_STORAGE{ // these will be saved and recovered on boot

      uint8_t mqtt_retain : 1;               // bit 0              - SetOption0  - Save power state and use after restart
      uint16_t      configperiod_secs;
      uint16_t      teleperiod_secs;
      uint16_t      ifchanged_secs;
      // Percentage of possible values that signify a large enough change has occured worth reporting.
      // A value of '0' percent means anything at all
      uint8_t       ifchanged_change_percentage_threshold;
      uint8_t       teleperiod_json_level;
      uint8_t       ifchanged_json_level;
      uint8_t       teleperiod_retain_flag;
      SysBitfield_Options options;
    }dt;

    std::vector<MQTTConnection*> brokers;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    uint16_t GetConfigPeriod(); // main
    uint16_t GetConfigPeriod_SubModule();
    uint16_t GetTelePeriod(); // main
    uint16_t GetTelePeriod_SubModule();
    uint16_t GetIfChangedPeriod(); // main
    uint16_t GetIfChangedPeriod_SubModule();

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    void Default_Module__Connection_WiFi();
    
    boolean Publish(const char* topic, const char* payload, boolean retained = false);
    boolean Subscribe(const char* topic, uint8_t qos = 0);

    void MQTTSubscribe();
    
    /**
     * @brief Cleaned method, use other functions to set other parts
     * 
     * @param client_in 
     * @param url 
     * @param _port 
     * @param type 
     * @param client_name 
     * @param prefix_topic 
     * @return true 
     * @return false 
     */
    bool CreateConnection(Client* client_in, char* host_address, uint16_t _port, ConnectionClient_t type)
    {

      /**
       * @brief Check if url already exists
       **/
      int8_t found_index = -1;
      uint8_t search_index = 0;
      for(auto& con:brokers)
      // for(uint8_t i=0;i<brokers_active;i++)
      {
        Serial.printf("Checking \"%s\" against existing \"%s\"\n\r\r\n\r\n\r\n\r\n\r\n", host_address, con->host_address);
        Serial.flush();
        if(strcmp(con->host_address, host_address)==0)
        {
          Serial.printf("found existing connection with \"%s\" Index:%d\n\r", host_address, search_index);
        search_index++;
        }
        else
        {
          Serial.printf("NOT found, needs to add \"%s\" Index:%d\n\r", host_address, search_index);
        }
      }

      if(search_index==0) // None found
      {
        Serial.printf("Adding new MQTTConnection \"%s\" Index:%d brokers_size:%d\n\r", host_address, search_index, brokers.size());
        Serial.flush();
        brokers.push_back(new MQTTConnection(client_in, host_address, _port, type));
        // brokers[0] = new MQTTConnection(client_in, url, _port, type, client_name, prefix_topic);
        // brokers_active = 1;
      }

      Serial.printf("MQTT::CreateConnection Host:\"%s\", Port:%d, Index:%d\n\r", host_address, _port, search_index);
Serial.flush();
    }

    


    void MM_EveryLoop()
    {
      for(auto& con:brokers)
      {
        con->EveryLoop();
      }
      
    }


    void MM_EverySecond()
    {
      for(auto& con:brokers)
      {
        
        con->EverySecond();
      }
    }

    void MM_Every50mSecond()
    {

      for(auto& con:brokers)
      {

        if(con->pubsub->loop())
        {
          // connected maintained by loop call. Same as pubsub->connected(), this does so by checking network client too!
          
          // cancel reconnect
            con->flag_start_reconnect = false;
            con->retry_counter = 0;

        }
        else
        {
          // Serial.println(D_LOG_PUBSUB "loop() Not Connected");

          /**
           * @brief Check if not waiting to reconnect
           **/
          if(!con->retry_counter)
          {
            con->flag_start_reconnect = true;
            con->retry_counter = 5;
          }
          
        }
      }
    }
    

    boolean subscribe_device(const char* topic)
    {
      uint8_t count = 0;
      for(auto& con:brokers)
      {
        count += con->subscribe_device(topic);
      }
      return (count == brokers.size()) ? true : false;
    }


    bool flag_uptime_reached_reduce_frequency = false;

    const char* GetState_PCtr(int8_t state);
    void Load_New_Subscriptions_From_Function_Template();
    
    void CallMQTTSenders();


    /**
     * Special MQTT functions that manupulate the generic mqtthandler_list from any class
     * handler<mAnimatorLight> *mAnimatorLight::mqtthandler_list[5]
     * Future idea:
     * Issue: Unknown how to also send length or array, so for(auto&) type loops probably wont work
     * */
    // template<typename T>
    // void MQTTHandler_Flags_SendNow(std::vector<struct handler<T>*> mqtthandler_list)
    // {     
    //   for(auto& handle:mqtthandler_list){
    //     handle->flags.SendNow = true;
    //   }
    // }

    // Centralized template function
    template <typename HandlerType>
    void MQTTHandler_RefreshAll(std::vector<struct handler<HandlerType>*>& handler_list) {
        for (auto& handle : handler_list) {
            handle->flags.SendNow = true;
        }
    }

    // Centralized template function
    template <typename HandlerType>
    void MQTTHandler_Rate(std::vector<struct handler<HandlerType>*>& handler_list) 
    {
      for (auto& handle : handler_list) 
      {
        if (handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
          handle->tRateSecs = GetTelePeriod_SubModule();
        else if (handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
          handle->tRateSecs = GetIfChangedPeriod_SubModule();
      }
    }





    // /**
    //  * @brief Function will take the struct storing the mqtt handler options, and check the variables, sending if required
    //  * @param class_ptr Pointer to the instance of a class
    //  * @param class_id  Numerical (Enum) value of the class, which is used to acquire the mqtt topic
    //  * @param handler_ptr Pointer to the handler struct with timing and function to be called
    //  * @param optional_desired_id Optional handler_id, if set (>=0) then only matching handler IDs will be sent. Used for group triggering by a type.
    //  * @note  Optional desired_id will check if the handler id was set, and if it does not match, will return without servicing handler
    //  * */
    // template<typename T>
    // void MQTTHandler_Command(T& class_ptr, uint16_t class_id, handler<T>* handler_ptr)
    // {

    //   // Sanity check
    //   if(handler_ptr == nullptr){
    //     Serial.println("handler_ptr == nullptr"); Serial.flush(); 
    //     return;
    //   }

    //   /**
    //    * @brief Only permit function to run if at least ONE broker is connected
    //    */
    //   bool is_any_broker_connected = false;
    //   for(auto& con:brokers)
    //   {
    //     if(con->uptime_seconds)
    //     {
    //       is_any_broker_connected = true;
    //     }
    //   }
    //   if(!is_any_broker_connected)
    //   {
    //     Serial.println("MQTTHandler_Command::Blocked");
    //     return;
    //   }
            
    //   #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
    //   // Serial.printf("MQTTHandler_Command::postfix_topic=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
    //   #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

    //   if(handler_ptr->flags.PeriodicEnabled){
    //     if(ABS_FUNCTION(millis()-handler_ptr->tSavedLastSent)>=handler_ptr->tRateSecs*1000){ 
    //       handler_ptr->tSavedLastSent=millis();
    //       handler_ptr->flags.SendNow = true;

    //       #ifndef ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

    //       //handler_ptr->flags.FrequencyRedunctionLevel = 1;
    //       if(flag_uptime_reached_reduce_frequency && (handler_ptr->flags.FrequencyRedunctionLevel > MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID)){
    //         // handler_ptr->flags.FrequencyRedunctionLevel = 0; // reset to don't change            
    //         //Serial.println("flag_uptime_reached_reduce_frequency");            
    //         handler_ptr->tRateSecs = handler_ptr->tRateSecs < 120 ? 120 : handler_ptr->tRateSecs; //only reduce if new rate is longer
    //       }

    //       #endif

    //     }
    //   }

    //   if(handler_ptr->flags.SendNow){   
        
    //     // #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
    //     // Serial.printf("MQTTHandler_Command::SendNow::postfix_topic\t=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
    //     // #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

    //     uint8_t fSendPayload = CALL_MEMBER_FUNCTION(class_ptr,handler_ptr->ConstructJSON_function)(handler_ptr->json_level, true);

    //     bool packet_successfully_sent = false;
         
    //     // Send MQTT payload with structured output
    //     if(fSendPayload)
    //     { 

    //       #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
    //       Serial.printf("MQTTHandler_Command::SendNow::fSendPayload::postfix_topic\t=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
    //       #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

    //       // Send on each connected broker
    //       for(auto& con:brokers)
    //       // for(uint8_t i=0;i<brokers_active;i++)
    //       {

    //         /*
    //         * prior to sending, check if limit time has passed
    //         */
    //         #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
    //         if(labs(millis()-con->tSaved_LastOutGoingTopic)>ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS)
    //         {
    //         #endif // ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS

    //           uint32_t tSaved_SendTime = millis();

    //           packet_successfully_sent = con->MQTTHandler_Send_Formatted(handler_ptr->topic_type,class_id,handler_ptr->postfix_topic); 

    //           #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
    //           Serial.printf("^^^^^^^^^^^^^^^^^^^ MQTTHandler_Send_Formatted %dms \n\r", millis()-tSaved_SendTime );
    //           #endif
              
    //           #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
    //           if(packet_successfully_sent)
    //           {
    //             #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
    //             Serial.printf("^^^^^^^^^^^^^^^^^^^ packet_successfully_sent %s DONE \n\r",handler_ptr->postfix_topic);
    //             #endif
    //             #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    //             con->debug_stats.payload_publish_sent++;
    //             con->debug_stats.payload_publish_success_percentage = (float)con->debug_stats.payload_publish_sent/(float)(con->debug_stats.payload_publish_missed+con->debug_stats.payload_publish_sent);
    //             #endif

    //             handler_ptr->flags.SendNow = false;      // Only to be reset if it sent at least once
    //             handler_ptr->tSavedLastSent = millis();  // Only to be reset if it sent at least once

    //           }
    //           else
    //           {
    //             #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
    //             Serial.printf("------------------- packet_successfully_sent %s ERROR\n\r",handler_ptr->postfix_topic);
    //             #endif
    //             #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    //             con->debug_stats.payload_publish_missed++;
    //             con->debug_stats.payload_publish_success_percentage = (float)con->debug_stats.payload_publish_sent/(float)(con->debug_stats.payload_publish_missed+con->debug_stats.payload_publish_sent);
    //             #endif
    //           }
    //         }else{
    //             Serial.printf("------------------- packet_successfully_sent %s LIMIT\n\r",handler_ptr->postfix_topic);
    //         }
    //         #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
    //         Serial.printf("last sent %d\n\r", millis()-con->tSaved_LastOutGoingTopic);
    //         #endif
          
    //         #else
          
    //           handler_ptr->flags.SendNow = false;      // Only to be reset if it sent at least once
    //           handler_ptr->tSavedLastSent = millis();  // Only to be reset if it sent at least once
          
    //         #endif // ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS

    //       }

    //     }
        

    //   }
    // };


    template<typename T>
    void MQTTHandler_Sender(std::vector<struct handler<T>*>& handler_list, T& class_ptr) {
        for (auto& handle : handler_list) {
            // Serial.printf("MQTTHandler_Sender");
            MQTTHandler_Command_UniqueID(class_ptr, class_ptr.GetModuleUniqueID(), handle);
        }
    }

    
    /**
     * @brief Function will take the struct storing the mqtt handler options, and check the variables, sending if required
     * @param class_ptr Pointer to the instance of a class
     * @param class_id  Numerical (Enum) value of the class, which is used to acquire the mqtt topic
     * @param handler_ptr Pointer to the handler struct with timing and function to be called
     * @param optional_desired_id Optional handler_id, if set (>=0) then only matching handler IDs will be sent. Used for group triggering by a type.
     * @note  Optional desired_id will check if the handler id was set, and if it does not match, will return without servicing handler
     * */
    template<typename T>
    void MQTTHandler_Command_UniqueID(T& class_ptr, uint16_t unique_id, handler<T>* handler_ptr)
    {

      // Sanity check
      if(handler_ptr == nullptr){
        Serial.println("handler_ptr == nullptr"); Serial.flush(); 
        return;
      }

      /**
       * @brief Only permit function to run if at least ONE broker is connected
       */
      bool is_any_broker_connected = false;
      for(auto& con:brokers)
      {
        if(con->uptime_seconds)
        {
          is_any_broker_connected = true;
        }
      }
      if(!is_any_broker_connected)
      {
        Serial.println("MQTTHandler_Command::Blocked");
        return;
      }

      #ifdef ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
      if(handler_ptr->tRateSecs == 0)
      {
        handler_ptr->flags.PeriodicEnabled = false; // If tRateSecs is zero, assume periodic has been disabled
      }
      #endif
            
      #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
      // Serial.printf("MQTTHandler_Command::postfix_topic=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
      #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

      if(handler_ptr->flags.PeriodicEnabled){
        if(ABS_FUNCTION(millis()-handler_ptr->tSavedLastSent)>=handler_ptr->tRateSecs*1000){ 
          handler_ptr->tSavedLastSent=millis();
          handler_ptr->flags.SendNow = true;

          #ifndef ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

          //handler_ptr->flags.FrequencyRedunctionLevel = 1;
          if(flag_uptime_reached_reduce_frequency && (handler_ptr->flags.FrequencyRedunctionLevel > MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID)){
            // handler_ptr->flags.FrequencyRedunctionLevel = 0; // reset to don't change            
            //Serial.println("flag_uptime_reached_reduce_frequency");            
            handler_ptr->tRateSecs = handler_ptr->tRateSecs < 120 ? 120 : handler_ptr->tRateSecs; //only reduce if new rate is longer
          }

          #endif

        }else
        /**
         * Initually set to 0 at boot [instead of millis()], so should be treated as never sent
         * Hence, if periodic is enabled, but has not yet been set, it should be triggered to send and therefore "SendNow" is no longer required as a default set flag.
         **/
        if(handler_ptr->tSavedLastSent == 0) 
        {
          handler_ptr->tSavedLastSent=millis();
          handler_ptr->flags.SendNow = true;
        }
      }

      if(handler_ptr->flags.SendNow){   
        
        // #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
        // Serial.printf("MQTTHandler_Command::SendNow::postfix_topic\t=%S %d\n\r",handler_ptr->postfix_topic, class_id); Serial.flush(); 
        // #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

        uint8_t fSendPayload = CALL_MEMBER_FUNCTION(class_ptr,handler_ptr->ConstructJSON_function)(handler_ptr->json_level, true);

        bool packet_successfully_sent = false;
         
        // Send MQTT payload with structured output
        if(fSendPayload)
        { 

          #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
          Serial.printf("MQTTHandler_Command::SendNow::fSendPayload::postfix_topic\t=%S %d\n\r",handler_ptr->postfix_topic, unique_id); Serial.flush(); 
          #endif // ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

          // Send on each connected broker
          for(auto& con:brokers)
          // for(uint8_t i=0;i<brokers_active;i++)
          {

            /*
            * prior to sending, check if limit time has passed
            */
            #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
            if(labs(millis()-con->tSaved_LastOutGoingTopic)>ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS)
            {
            #endif // ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS

              uint32_t tSaved_SendTime = millis();

              packet_successfully_sent = con->MQTTHandler_Send_Formatted_UniqueID(handler_ptr->topic_type,unique_id,handler_ptr->postfix_topic, handler_ptr->flags.retain); 

              #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
              Serial.printf("^^^^^^^^^^^^^^^^^^^ MQTTHandler_Send_Formatted %dms \n\r", millis()-tSaved_SendTime );
              #endif
              
              #ifdef ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS
              if(packet_successfully_sent)
              {
                #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
                Serial.printf("^^^^^^^^^^^^^^^^^^^ packet_successfully_sent %s DONE \n\r",handler_ptr->postfix_topic);
                #endif
                #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
                con->debug_stats.payload_publish_sent++;
                con->debug_stats.payload_publish_success_percentage = (float)con->debug_stats.payload_publish_sent/(float)(con->debug_stats.payload_publish_missed+con->debug_stats.payload_publish_sent);
                #endif

                handler_ptr->flags.SendNow = false;      // Only to be reset if it sent at least once
                handler_ptr->tSavedLastSent = millis();  // Only to be reset if it sent at least once

              }
              else
              {
                #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
                Serial.printf("------------------- packet_successfully_sent %s ERROR\n\r",handler_ptr->postfix_topic);
                #endif
                #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
                con->debug_stats.payload_publish_missed++;
                con->debug_stats.payload_publish_success_percentage = (float)con->debug_stats.payload_publish_sent/(float)(con->debug_stats.payload_publish_missed+con->debug_stats.payload_publish_sent);
                #endif
              }
            }else{
                Serial.printf("------------------- packet_successfully_sent %s LIMIT\n\r",handler_ptr->postfix_topic);
            }
            #ifdef ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES
            Serial.printf("last sent %d\n\r", millis()-con->tSaved_LastOutGoingTopic);
            #endif
          
            #else
          
              handler_ptr->flags.SendNow = false;      // Only to be reset if it sent at least once
              handler_ptr->tSavedLastSent = millis();  // Only to be reset if it sent at least once
          
            #endif // ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS

          }

        }
        

      }
    };

};

#endif

#endif //  USE_MODULE_NETWORK_MQTT