#ifndef _MTELEMETRY_H_
#define _MTELEMETRY_H_

#define D_UNIQUE_MODULE_CORE_TELEMETRY_ID 2009 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/06_Support/mSupport.h"
#include "2_CoreSystem/20_JSON/mJSON.h"
#include "3_Network/10_MQTT/mMQTT.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/07_Time/mTime.h"
#include <Ticker.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#ifdef USE_MODULE_NETWORK_WEBSERVER
#include "3_Network/21_WebServer/mWebServer.h"
#endif

#include "1_TaskerManager/mTaskerInterface.h"

#define SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY 600

class mTelemetry :
  public mTaskerInterface
{
  public:
    mTelemetry(){};    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void WebPage_Root_AddHandlers();

    uint8_t serial_messages_remaining_to_send = 0;
    
    static constexpr const char* PM_MODULE_CORE_TELEMETRY_CTR = D_MODULE_CORE_TELEMETRY_CTR;
    PGM_P GetModuleName(){ return PM_MODULE_CORE_TELEMETRY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TELEMETRY_ID; }
   
    uint8_t ConstructJSON_LWT_Online(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Health(uint8_t json_method = 0, bool json_appending = true);
    #ifndef FIRMWARE_MINIMAL2
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Firmware(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Log(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Memory(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Network(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_MQTT(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Time(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Reboot(uint8_t json_method = 0, bool json_appending = true);
    #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
    uint8_t ConstructJSON_Debug_Devices(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Pins(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Template(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_ModuleInterface(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Minimal(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_System_Stored_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_Debug_Tasker_Interface_Performance(uint8_t json_method = 0, bool json_appending = true);
      #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
      uint8_t ConstructJSON_Debug__Settings_Storage(uint8_t json_method = 0, bool json_appending = true);
      #endif
    #endif
    #endif // FIRMWARE_MINIMAL2

    #ifdef USE_MODULE_NETWORK_MQTT    
      void MQTTHandler_Init();      
      std::vector<struct handler<mTelemetry>*> mqtthandler_list;    
      handler<mTelemetry> mqtthandler_lwt_online;
      handler<mTelemetry> mqtthandler_health;
      #ifndef FIRMWARE_MINIMAL2
      handler<mTelemetry> mqtthandler_settings;
      handler<mTelemetry> mqtthandler_log;
      handler<mTelemetry> mqtthandler_firmware;
      handler<mTelemetry> mqtthandler_memory;
      handler<mTelemetry> mqtthandler_network;
      handler<mTelemetry> mqtthandler_mqtt;
      handler<mTelemetry> mqtthandler_time;
      handler<mTelemetry> mqtthandler_reboot;
      handler<mTelemetry> mqtthandler_reboot_event;
      #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
        handler<mTelemetry> mqtthandler_devices;
        handler<mTelemetry> mqtthandler_debug_pins;
        handler<mTelemetry> mqtthandler_debug_template;
        handler<mTelemetry> mqtthandler_debug_moduleinterface;
        handler<mTelemetry> mqtthandler_debug_minimal;
        handler<mTelemetry> mqtthandler_debug_system_saved_settings;
        #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
        handler<mTelemetry> mqtthandler_debug_tasker_interface_performance;
        #endif
        #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
        handler<mTelemetry> mqtthandler_debug__settings_storage;
        #endif
      #endif // ENABLE_MQTT_DEBUG_TELEMETRY
      #endif // FIRMWARE_MINIMAL2
    
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif  // _MTELEMETRY_H_
