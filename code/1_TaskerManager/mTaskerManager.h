#ifndef mInterfaceController_H2
#define mInterfaceController_H2 1.1

#define USE_USER_MICHAEL

#ifdef USE_USER_MICHAEL
#warning "------------------------------------------------------------------------------- Development by Author, may not compile for others"
#endif

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include <ext_printf.h>

#define SET_FLAG   true
#define RESET_FLAG false

#ifdef CONFIG_IDF_TARGET_ESP32
#include "soc/efuse_reg.h"
#endif

#include <vector>

#include "JsonParser.h"

#include "2_CoreSystem/esp32_compat.h"
#include "2_CoreSystem/mGlobalMacros.h"

#include "2_CoreSystem/mBaseConfig.h"           //DEFAULTS
#ifndef USE_USER_MICHAEL // Include my personally named secret file
  #include "0_ConfigUser/mFirmwareCustom_Secret.h"
#endif // USE_USER_MICHAEL // Include my personally named secret file
#ifdef USE_USER_MICHAEL // Include my personally named secret file
  /**
   * In order of importance
   * */
  #include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_LongTerm.h"
  #include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_Temporary.h"
  #include "0_ConfigUser/01_mFirmwareCustom_Secret_ExampleTemplates.h"
  #include "0_ConfigUser/02_mFirmwareCustom_Secret_DevTestbeds.h"
  #include "0_ConfigUser/TestGroups/FirmwareGroup_LightingEffects.h"
  #include "0_ConfigUser/TestGroups/FirmwareGroup_HVAC.h"
  #include "0_ConfigUser/TestGroups/FirmwareGroup_PZEM.h"
  #include "0_ConfigUser/BuiltUsingGroups/GroupUsing_LightingEffects.h"
  #include "0_ConfigUser/TestGroups/FirmwareGroup_CellularDatalinks.h"
#endif // USE_USER_MICHAEL

#include "2_CoreSystem/06_Support/mSensorType.h"

#include "0_ConfigUser/G1_mUserConfig_Secret.h"  //wrong place??
#include "2_CoreSystem/16_Events/mEvents.h"

#include <DeCounter.h>

#ifdef USE_MODULE_CORE_RULES
#include "2_CoreSystem/10_RuleEngine/mRuleEngine.h"
#endif

#include "2_CoreSystem/mFirmwareDefaults.h"                    // Configuration overrides for all previous includes

#include "2_CoreSystem/11_Languages/mLanguageDefault.h"                           // Language support configured by .h
#include "2_CoreSystem/05_HardwarePins/mHardwarePins_Templates.h"                // Hardware configuration
#include "2_CoreSystem/06_Support/BufferWriter.h"

#include "1_TaskerManager/mTasks.h"
#include <WiFiClient.h>
#ifdef ESP8266
  #include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#endif

#ifdef ESP8266
  #include "2_CoreSystem/06_Support/SupportESP8266.h"
  #define mSupportHardware SupportESP8266
#endif
#ifdef ESP32
  #include "2_CoreSystem/06_Support/SupportESP32.h"
  #define mSupportHardware SupportESP32
#endif

#ifdef USE_MODULE_NETWORK_WEBSERVER
    
  #define LOROL_LITTLEFS 

  #ifdef ESP8266
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
    #include <ESPAsyncTCP.h>
    #include <LittleFS.h>
    extern "C"
    {
    #include <user_interface.h>
    }
    #ifndef WLED_DISABLE_ESPNOW
      #include <espnow.h>
    #endif
  #else // ESP32
    #include <HardwareSerial.h>  // ensure we have the correct "Serial" on new MCUs (depends on ARDUINO_USB_MODE and ARDUINO_USB_CDC_ON_BOOT)
    #include <WiFi.h>
    #include <ETH.h>
    #include "esp_wifi.h"
    #include <ESPmDNS.h>
    #include <AsyncTCP.h>
    #include "esp_task_wdt.h"

    #ifndef WLED_DISABLE_ESPNOW
      #include <esp_now.h>
    #endif
  #endif

#endif // USE_MODULE_NETWORK_WEBSERVER


#ifdef USE_MODULE_CORE_FILESYSTEM
  #define LOROL_LITTLEFS   
    #ifdef LOROL_LITTLEFS
      #ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
        #define CONFIG_LITTLEFS_FOR_IDF_3_2
      #endif
      #include <LittleFS.h>
    #else
      #include <LittleFS.h>
    #endif
  //Filesystem to use for preset and config files. SPIFFS or LittleFS on ESP8266, SPIFFS only on ESP32 (now using LITTLEFS port by lorol)
  #ifdef ESP8266
    #define FILE_SYSTEM LittleFS
  #else
    #ifdef LOROL_LITTLEFS
      #define FILE_SYSTEM LITTLEFS
    #else
      #define FILE_SYSTEM LittleFS
    #endif
  #endif
#endif // USE_MODULE_CORE_FILESYSTEM


// Returns via tasker that report special status
// Can also be interpeted as basic numbers
enum FUNCTION_RESULT_IDS{
  // Errors
  FUNCTION_RESULT_ERROR_POINTER_INVALID_ID = -2,
  FUNCTION_RESULT_MODULE_DISABLED_ID = -1,
  // Unknown
  FUNCTION_RESULT_UNKNOWN_ID = 0,
  // Good results
  FUNCTION_RESULT_SUCCESS_ID,
  FUNCTION_RESULT_HANDLED_ID,
  FUNCTION_RESULT_ERROR_ID,
  // Length
  FUNCTION_RESULT_LENGTH_ID
};

enum ModuleStatus{
  Disabled,
  NoGPIOConfigured,
  Initialising,
  Running,
  DevicesPresent // Running means searching is enabled, but this is only set when devices are found
};


#include "2_CoreSystem/mFirmwareDefaults.h"
#include "2_CoreSystem/mSystemConfig.h"


// Libraries
// #include <StreamString.h>                   // Webserver, Updater

#ifdef USE_ARDUINO_OTA
  #include <ArduinoOTA.h>                   // Arduino OTA
#endif  // USE_ARDUINO_OTA

#ifdef USE_MODULE_CORE_I2C
  #include <Wire.h>                         // I2C support library
#endif

#ifdef ESP32
  #include <WiFi.h>
  #ifdef USE_NETWORK_MDNS
  #include <ESPmDNS.h>
  #endif // #ifdef USE_NETWORK_MDNS
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include "Ticker.h"
  #include "esp_system.h"
  #include "soc/soc.h"
  #include "soc/rtc_cntl_reg.h"
#endif
#ifdef ESP8266
  #include <ESP8266HTTPClient.h>
  #include <ESP8266httpUpdate.h>  
  #ifdef USE_DISCOVERY
    #include <ESP8266mDNS.h>                  // MQTT, Webserver, Arduino OTA
  #endif  // USE_DISCOVERY
  #include <ArduinoOTA.h>
  #include <WiFiUdp.h>
  #include <ESP8266WiFi.h>
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    // #include <ESPAsyncTCP.h>
    //?#include <ESPAsyncWebServer.h>
  #endif //USE_MODULE_NETWORK_WEBSERVER
#endif

#include "2_CoreSystem/20_JSON/mJSON.h"
#include "2_CoreSystem/06_Support/mSupport.h"

#include "2_CoreSystem/02_RtcMemory/mRtcSettings.h"


enum MODULE_SUBTYPE_IDS{ //ignores the "interface"
  MODULE_SUBTYPE_CORE_ID,
  MODULE_SUBTYPE_NETWORK_ID,
  MODULE_SUBTYPE_DISPLAY_ID,
  MODULE_SUBTYPE_SENSOR_ID,
  MODULE_SUBTYPE_DRIVERS_ID,
  MODULE_SUBTYPE_LIGHTS_ID,
  MODULE_SUBTYPE_ENERGY_ID,
};

#define pCONT mTaskerManager::GetInstance()


// macro to know if a module is a type (ie within a range)
#define IS_MODULE_SENSOR_SUBMODULE(x)   (x >= 5001 && x <= 5999) // skipping the interface
#define IS_MODULE_LIGHTING_SUBMODULE(x) (x >= 6001 && x <= 6999) // skipping the interface
#define IS_MODULE_ENERGY_SUBMODULE(x)   (x >= 7001 && x <= 7999) // skipping the interface
#define IS_MODULE_DISPLAY_SUBMODULE(x)  (x >= 8001 && x <= 8999) // skipping the interface

// Added to maybe fix header include issue
#include "3_Network/10_MQTT/mMQTT.h"

/**
 * @brief Core Modules
 **/
#ifdef USE_MODULE_CORE_SETTINGS
  #include "2_CoreSystem/01_Settings/mSettings.h"
  #define   pCONT_set                               static_cast<mSettings*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_SETTINGS_ID))
#endif 
#ifdef USE_MODULE_CORE_FILESYSTEM
  #include "2_CoreSystem/03_FileSystem/mFileSystem.h"
  #define pCONT_mfile                               static_cast<mFileSystem*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_FILESYSTEM_ID))
#endif
#ifdef USE_MODULE_CORE__JSON_TEMPLATE
  #include "2_CoreSystem/04_JsonTemplate/mJsonTemplate.h"
  #define pCONT_json_template                       static_cast<mJsonTemplate*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_JSON_TEMPLATE_ID))
#endif
#ifdef USE_MODULE_CORE_HARDWAREPINS
  #include "2_CoreSystem/05_HardwarePins/mHardwarePins.h"
  #define   pCONT_pins                              static_cast<mHardwarePins*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID))
#endif 
#ifdef USE_MODULE_CORE_TIME
  #include "2_CoreSystem/07_Time/mTime.h"
  #define   pCONT_time                              static_cast<mTime*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_TIME_ID))
#endif 
#ifdef USE_MODULE_CORE_LOGGING
  #include "2_CoreSystem/08_Logging/mLogging.h"
  #define   pCONT_log                               static_cast<mLogging*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_LOGGING_ID))
#endif 
#ifdef USE_MODULE_CORE_TELEMETRY
  #include "2_CoreSystem/09_Telemetry/mTelemetry.h"
  #define   pCONT_tel                               static_cast<mTelemetry*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_TELEMETRY_ID))
#endif 
#ifdef USE_MODULE_CORE_RULES
  #include "2_CoreSystem/10_RuleEngine/mRuleEngine.h"
  #define   pCONT_rules                             static_cast<mRuleEngine*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID))
#endif
#ifdef USE_MODULE_CORE_UPDATES
  #include "2_CoreSystem/14_Updates/mUpdates.h"
  #define   pCONT_updates                           static_cast<mUpdates*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_UPDATES_ID))
#endif
#ifdef USE_MODULE_CORE__SERIAL
  #include "2_CoreSystem/15_SerialUART/mSerial.h"
  #define   tkr_Serial                           static_cast<mSerial*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__SERIAL__ID))
#endif
// #ifdef USE_MODULE_CORE_SERIAL_UART
//   #include "2_CoreSystem/04b_SerialUART/mSerialUART.h"
//   #define pCONT_uart                                static_cast<mSerialUART*>(pCONT->pModule[EM_MODULE_CORE_SERIAL_UART_ID])
// #endif
#ifdef USE_MODULE_CORE_I2C
  #include "2_CoreSystem/31_I2C/mI2C.h"
  #define pCONT_i2c                       static_cast<mI2C*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__I2C__ID))
#endif
#ifdef USE_MODULE_CORE_SPI
  #include "2_CoreSystem/32_SPI/mSPI.h"
  #define pCONT_spi                       static_cast<mSPI*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__SPI__ID))
#endif
#ifdef USE_MODULE_CORE_SUPPORT
  #include "2_CoreSystem/06_Support/mSupport.h"
  #define   pCONT_sup                               static_cast<mSupport*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_SUPPORT_ID))
#endif 
#ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  #include "2_CoreSystem/99_DevelopmentDebugging/mDevelopmentDebugging.h"
  #define   pCONT_debug             static_cast<mDevelopmentDebugging*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID))
#endif 
/**
 * @brief Network
 **/
#ifdef USE_MODULE_NETWORK_INTERFACE
  #include "3_Network/00_Interface/mInterface.h"
  #define pCONT_interface_network                                static_cast<mInterfaceNetwork*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__NETWORK_INTERFACE__ID))
#endif 
#ifdef USE_MODULE_NETWORK_WIFI
  #include "3_Network/03_WiFi/mWiFi.h"
  #define pCONT_wif                                 static_cast<mWiFi*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_WIFI_ID))
#endif 
#ifdef USE_MODULE_NETWORK_ETHERNET
  //EM_MODULE_NETWORK_ETHERNET_ID,
#endif 
#ifdef USE_MODULE_NETWORK_CELLULAR
#include "3_Network/05_Cellular/mCellular.h"
  #define pCONT_cell                               static_cast<mCellular*>(pCONT->pModule[EM_MODULE__NETWORK_CELLULAR__ID])
#endif
#ifdef USE_MODULE_NETWORK_MQTT
  #include "3_Network/10_MQTT/mMQTT.h"
  #define pCONT_mqtt                                static_cast<mMQTTManager*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_MQTT_ID))
#endif 
#ifdef USE_MODULE_NETWORK_WEBSERVER
  #include "3_Network/21_WebServer/mWebServer.h"
  #define pCONT_web                                 static_cast<mWebServer*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_WEBSERVER_ID))
#endif
/**
 * @brief Displays
 **/
#ifdef USE_MODULE_DRIVERS_INTERFACE
  #include "4_Drivers/00_Interface/mDriverInterface.h"
  #define pCONT_iDrivers                           static_cast<mDriverInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_DRIVERS_LEDS
#include "4_Drivers/03_LEDs/mLEDs.h"
  #define pCONT_led                                static_cast<mLEDs*>(pCONT->pModule[EM_MODULE_DRIVERS_LEDS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RELAY
  #include "4_Drivers/04_Relays/mRelays.h"
  #define pCONT_mry                                 static_cast<mRelays*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS_RELAY_ID))
#endif
#ifdef USE_MODULE_DRIVERS_PWM
  #include "4_Drivers/PWM/mPWM.h"
  #define pCONT_pwm                                 static_cast<mPWM*>(pCONT->pModule[EM_MODULE_DRIVERS_PWM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  #define pCONT_mdirt                               static_cast<mIRtransceiver*>(pCONT->pModule[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRREMOTE
#include "4_Drivers/IRRemote/mIRRemote.h"
  #define pCONT_ir_remote                           static_cast<mIRRemote*>(pCONT->pModule[EM_MODULE_DRIVERS_IRREMOTE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
  #include "4_Drivers/09_RCSwitch/mRCSwitch.h"
  #define pCONT_rcswitch                            static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  #include "4_Drivers/10_RCSwitch_Extended/mRCSwitch_Extended.h"
  #define pCONT_rcswitch                            static_cast<mRCSwitch*>(pCONT->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID])
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pModule[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SDCARD
  #include "4_Drivers/SD/mSDCard.h"
  #define pCONT_sdcard                              static_cast<mSDCard*>(pCONT->pModule[EM_MODULE_DRIVERS_SDCARD_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  #include "4_Drivers/15_ShellyDimmer/mShellyDimmer.h"
  #define pCONT_shelly                              static_cast<mShellyDimmer*>(pCONT->pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID])
#endif
#ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  #include "4_Drivers/20_Buzzer_Basic/mBuzzer.h"
  #define pCONT_buzzer                              static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE__DRIVERS_BUZZER_BASIC__ID])
#endif
#ifdef USE_MODULE__DRIVERS_BUZZER_TONES
  #include "4_Drivers/21_Buzzer_Tones/mBuzzer.h"
  #define pCONT_buzzer                              static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE__DRIVERS_BUZZER_TONES__ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
  #include "4_Drivers/50_CAM_OV2640/mCamera.h"
  #define pCONT_mdhbridge                           static_cast<mCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
  #include "4_Drivers/Camera_OV2640/mCameraOV2640.h"
  #define pCONT_camera                              static_cast<mCameraOV2640*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_OV2640_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM
  #include "4_Drivers/51_WebCam/mWebCam.h"
  #define pCONT_camera                              static_cast<mWebCam*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
  #include "4_Drivers/52_WebCamera/mWebCamera.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO
  #include "4_Drivers/60_WebCam_Arduino/mWebCam.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_ARDUINO__ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA
  #include "4_Drivers/61_WebCam_Tas/mWebCam.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS__CAMERA_TASMOTA__ID))
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_MULTICLIENT
  #include "4_Drivers/52_WebCamera/mWebCamera.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_MULTICLIENT__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  #include "4_Drivers/70_MAVLink_Decoder/mMAVLink_Decoder.h"
  #define tkr_mavlink                              static_cast<mMAVLink_Decoder*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_DECODER__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI
  #include "4_Drivers/71_MAVLink_Telemetry_WiFi/mMAVLink_Telemetry_WiFi.h"
  #define pCONT_mavlink                              static_cast<mMAVLink_Telemetry_WiFi*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR
  #include "4_Drivers/72_MAVLink_Telemetry_Cellular/mMAVLink_Telemetry_Cellular.h"
  #define pCONT_mavlink                              static_cast<mMAVLink_Telemetry_Cellular*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR__ID])
#endif
#ifdef USE_MODULE_DRIVERS_MODEM_7000G
#include "4_Drivers/80_Modem_SIM7000G/mSIM7000G.h"
  #define pCONT_sim7000g                           static_cast<mSIM7000G*>(pCONT->pModule[EM_MODULE_DRIVERS__MODEM_7000G__ID])
#endif
#ifdef USE_MODULE_DRIVERS_MODEM_800L
#include "4_Drivers/81_Modem_SIM800L/mSIM800L.h"
  #define pCONT_sim800l                           static_cast<mSIM800L*>(pCONT->pModule[EM_MODULE_DRIVERS__MODEM_800L__ID])
#endif
/**
 * @brief Sensors
 **/
#ifdef USE_MODULE_SENSORS_INTERFACE
  #include "5_Sensors/00_Interface/mSensorsInterface.h"
  #define pCONT_iSensors                         static_cast<mSensorsInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  #include "5_Sensors/01_Switches/mSwitches.h"
  #define pCONT_swt                            static_cast<mSwitches*>(pCONT->pModule[EM_MODULE_SENSORS_SWITCHES_ID])
#endif
#ifdef USE_MODULE_SENSORS_BUTTONS
  #include "5_Sensors/02_Buttons/mButtons.h"
  #define pCONT_sbutton                         static_cast<mButtons*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_SENSORS_BME
  #include "5_Sensors/03_BME/mBME.h"
  #define pCONT_bme                             static_cast<mBME*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_BME_ID))
#endif
#ifdef USE_MODULE_SENSORS_DHT
  #include "5_Sensors/05_DHT/mSensorsDHT.h"
  #define pCONT_dht                             static_cast<mSensorsDHT*>(pCONT->pModule[EM_MODULE_SENSORS_DHT_ID])
#endif
#ifdef USE_MODULE_SENSORS_BH1750
  #include "5_Sensors/06_BH1750Light/mBH1750.h"
  #define pCONT_bh1750                          static_cast<mBH1750*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_BH1750_ID))
#endif
#ifdef USE_MODULE_SENSORS_PIR
  #include "5_Sensors/07_PIR/mPIR.h"
  #define pCONT_motion                            static_cast<mPIR*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_PIR_ID))
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  #include "5_Sensors/11_Door/mDoorSensor.h"
  #define pCONT_sdoor                           static_cast<mDoorSensor*>(pCONT->pModule[EM_MODULE_SENSORS_DOOR_ID])
#endif
#ifdef USE_MODULE_SENSORS_L3G
  #include "5_Sensors/L3GD20_3Axis_Gryo/mSensorsL3G.h"
  #define pCONT_L3G                      static_cast<mSensorsL3G*>(pCONT->pModule[EM_MODULE_SENSORS_L3G_ID])
#endif
#ifdef USE_MODULE_SENSORS_LDR_BASIC
  #include "5_Sensors/14_LDRBasic/mLDRBasic.h"
  #define pCONT_ldr_basic                      static_cast<mLDRBasic*>(pCONT->pModule[EM_MODULE_SENSORS_LDR_BASIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_LSM303D
  #include "5_Sensors/LSM303D_3Axis_AccMag/mSensorsLSM303D.h"
  #define pCONT_LSM303D                      static_cast<mSensorsLSM303D*>(pCONT->pModule[EM_MODULE_SENSORS_LSM303D_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOISTURE
  #include "5_Sensors/Moisture/mMoistureSensor.h"
  #define pCONT_srmoisture                      static_cast<mMoistureSensor*>(pCONT->pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID])
#endif
#ifdef USE_MODULE_SENSORS_SR04
  #include "5_Sensors/17_SR04/mSR04.h"
  #define pCONT_sr04                              static_cast<mSR04*>(pCONT->pModule[EM_MODULE_SENSORS_SR04_ID])
#endif
#ifdef USE_MODULE_SENSORS_MPU9250
  #include "5_Sensors/MPU9250/mSensorsMPU9250.h"
  #define pCONT_MPU9250                      static_cast<mSensorsMPU9250*>(pCONT->pModule[EM_MODULE_SENSORS_MPU9250_ID])
#endif
#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #include "5_Sensors/PulseCounter/mPulseCounter.h"
  #define pCONT_spulse                          static_cast<mPulseCounter*>(pCONT->pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID])
#endif
#ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
  #include "5_Sensors/20_RemoteDevice/mRemoteDevice.h"
  #define pCONT_sremote                           static_cast<mRemoteDevice*>(pCONT->pModule[EM_MODULE_SENSORS_REMOTE_DEVICE_ID])
#endif
#ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
  #include "5_Sensors/21_RotaryEncoder/mRotaryEncoder.h"
  #define pCONT_rotary_encoder               static_cast<mRotaryEncoder*>(pCONT->pModule[EM_MODULE_SENSORS_ROTARY_ENCODER_ID])
#endif
#ifdef USE_MODULE_SENSORS_SUN_TRACKING
  #include "5_Sensors/22_SunTracking/mSunTracking.h"
  #define   pCONT_solar                             static_cast<mSunTracking*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_ID))
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #include "5_Sensors/UltraSonic/mUltraSonicSensor.h"
  #define pCONT_ult                             static_cast<mUltraSonicSensor*>(pCONT->pModule[EM_MODULE_SENSORS_ULTRASONIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_ADC_INTERNAL
  #include "5_Sensors/30_ADCInternal/mADCInternal.h"
  #define pCONT_adc_internal                          static_cast<mADCInternal*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_ADC_INTERNAL_ID))
#endif
// #ifdef USE_MODULE_SENSORS_ADC_INTERNAL  I2S may be best phased into above as option, since most code remains the same
//   #include "5_Sensors/31_ADCInternal_ESP32/mADCInternal.h"
//   #define pCONT_iLight                          static_cast<mADCInternal*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_ADC_INTERNAL_ID))
// #endif
#ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
  #include "5_Sensors/40_ds18x20/mDB18x20.h"
  #define pCONT_db18                      static_cast<mDB18x20_ESP32*>(pCONT->pModule[EM_MODULE_SENSORS__DS18X20__ID])
#endif
#ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  #include "5_Sensors/41_ds18x20_esp32/mDB18x20_ESP32.h"
  #define pCONT_db18                      static_cast<mDB18x20_ESP32*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID))
#endif
#ifdef USE_MODULE_SENSORS_GPS_SERIAL
  #include "5_Sensors/50_GPS_Serial/mGPS_Serial.h"
  #define pCONT_gps                                static_cast<mGPS_Serial*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID))
#endif
#ifdef USE_MODULE_SENSORS_GPS_MODEM
  #include "5_Sensors/51_GPS_Modem/mGPS_Modem.h"
  #define pCONT_gps                                 static_cast<mGPS_Modem*>(pCONT->pModule[EM_MODULE__SENSORS_GPS_MODEM__ID])
#endif
#ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  #include "5_Sensors/52_Battery_Modem/mBattery_Modem.h"
  #define pCONT_batt_modem                                 static_cast<mBattery_Modem*>(pCONT->pModule[EM_MODULE__SENSORS_BATTERY_MODEM__ID])
#endif
/**
 * @brief Lights
 **/
#ifdef USE_MODULE_LIGHTS_INTERFACE
  #include "6_Lights/00_Interface/mInterfaceLight.h"
  #define pCONT_iLight                          static_cast<mInterfaceLight*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_LIGHTS_ANIMATOR
  #include "6_Lights/03_Animator/mAnimatorLight.h"
  #define tkr_lAni                            static_cast<mAnimatorLight*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID))
  #ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
  #define tkr_anim tkr_extern_lAni // using a more direct access method, with a local pointer in the class header
  #else
  #define tkr_anim tkr_lAni // pointer to the instance of the mAnimatorLight class
  #endif
#endif
/**
 * @brief Energy
 **/
#ifdef USE_MODULE_ENERGY_INTERFACE
  #include "7_Energy/00_Interface/mEnergyInterface.h"
  #define pCONT_iEnergy                           static_cast<mEnergyInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_ENERGY_INTERFACE_ID))
#endif
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  #include "7_Energy/01_Pzem004T_v3/mPZEM004T.h"
  #define pCONT_pzem                              static_cast<mEnergyPZEM004T*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_ENERGY_PZEM004T_ID))
#endif
#ifdef USE_MODULE_ENERGY_ADE7953
  #include "7_Energy/02_ADE7953/mADE7953.h"
  #define pCONT_ade7953                         static_cast<mEnergyADE7953*>(pCONT->pModule[EM_MODULE_ENERGY_ADE7953_ID])
#endif
#ifdef USE_MODULE_ENERGY_INA219
  #include "7_Energy/03_INA219/mINA219.h"
  #define pCONT_mina219                         static_cast<mEnergyINA219*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_ENERGY_INA219_ID))
#endif
/**
 * @brief Displays
 **/
#ifdef USE_MODULE_DISPLAYS_INTERFACE
  #include "8_Displays/00_Interface/mDisplaysInterface.h"
  #define pCONT_iDisp                               static_cast<mDisplaysInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DISPLAYS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_DISPLAYS_NEXTION
  #include "8_Displays/01_Nextion/mNextionPanel.h"
  #define pCONT_nex                                 static_cast<mNextionPanel*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DISPLAYS_NEXTION_ID))
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  #include "8_Displays/02_OLED_SSD1606/mOLED_SSD1306.h"
  #define pCONT_oled1306                            static_cast<mOLED_SSD1306*>(pCONT->pModule[EM_MODULE_DISPLAYS_OLED_SSD1306_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SH1106
  #include "8_Displays/03_OLED_SH1106/mOLED_SH1106.h"
  #define pCONT_oled1306                            static_cast<mOLED_SH1106*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DISPLAYS_OLED_SH1106_ID))
#endif
/**
 * @brief Controller Generic (Generic)
 **/
#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  #include "9_Controller/03_Sonoff_iFan/mSonoffIFan.h"
  #define pCONT_ifan                            static_cast<mSonoffIFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_SONOFF_IFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FAN
  #include "9_Controller/Fan/mFan.h"
  #define pCONT_mfan                            static_cast<mFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_FAN_ID])
#endif
// 3d printer encoder here
#ifdef USE_MODULE_CONTROLLER_TANKVOLUME
  #include "9_Controller/08_TankVolume/mTankVolume.h"
  #define pCONT_tankvolume                      static_cast<mTankVolume*>(pCONT->pModule[EM_MODULE_CONTROLLER_TANKVOLUME_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_BLINDS
  #include "9_Controller/Blinds/mBlinds.h"
  #define pCONT_sbut                            static_cast<mBlinds*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  #include "9_Controller/BucketWaterLevel/mBucketWaterLevel.h"
  #define pCONT_bucket_water_level                static_cast<mBucketWaterLevel*>(pCONT->pModule[EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_DOORCHIME
  #include "9_Controller/DoorBell/mDoorBell.h"
  #define pCONT_doorbell                        static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_DOORBELL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
  #include "9_Controller/GPS_SD_Logger/mGPS_SD_Logger.h"
  #define pCONT_gps_sd_log                  static_cast<mGPS_SD_Logger*>(pCONT->pModule[EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID])
  #define pCONT_serial_pos_log  pCONT_gps_sd_log
#endif
#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
  #include "9_Controller/SDCardLogger/mSDCardLogger.h"
  #define pCONT_sdcardlogger                    static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDCARDLOGGER_ID])
#endif
// USE_MODULE_CONTROLLER_UARTLOGGER
// USE_MODULE_CONTROLLER_INTERNAL_CLOCK
#ifdef USE_MODULE_CONTROLLER_HVAC
  #include "9_Controller/40_HVAC/mHVAC.h"
  #define pCONT_hvac                        static_cast<mHVAC*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLER_HVAC_ID))
#endif
#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE
  #include "9_Controller/41_HVAC_Remote/mHVAC.h"
  #define pCONT_hvac_remote                        static_cast<mHVAC_Remote*>(pCONT->pModule[EM_MODULE_CONTROLLER__HVAC_REMOTE__ID])
#endif
#ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  #include "9_Controller/42_SensorColourBar/mSensorColourBar.h"
  #define pCONT_colourbar                        static_cast<mSensorColourBar*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID))
#endif
#ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  #include "9_Controller/43_RelayStateLEDStrip/mRelayStateLEDStrip.h"
  #define pCONT_relay_strip                      static_cast<mRelayStateLEDStrip*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__ID))
#endif
// 50_Animator_Input_Controller
// 51_Animator_Playlists
#ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
  #include "9_Controller/60_EnergyOnOLED/mEnergyOnOLED.h"
  #define pCONT_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER__ENERGY_OLED__ID])
#endif
/**
 * @brief 10 Controller Custom
 **/  
#ifdef USE_MODULE_CONTROLLER_RADIATORFAN
  #include "10_ConSpec/00_RadiatorFan/mRadiatorFan.h"
  #define pCONT_sbut                            static_cast<mRadiatorFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_RADIATORFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
  #include "10_ConSpec/01_ImmersionTankColour/mImmersionTankColour.h"
  #define pCONT_msenscol                        static_cast<mImmersionTankColour*>(pCONT->pModule[EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
  #include "10_ConSpec/03_FurnaceSensor/mFurnaceSensor.h"
  #define pCONT_furnace_sensor                static_cast<mFurnaceSensor*>(pCONT->pModule[EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID])
#endif
#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
  #include "10_ConSpec/04_LouvoliteHub/mLouvoliteHub.h"
  #define pCONT_louv                static_cast<mLouvoliteHub*>(pCONT->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
#endif
#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2
  #include "10_ConSpec/04v2_LouvoliteHub/mLouvoliteHub.h"
  #define pCONT_louv                static_cast<mLouvoliteHub*>(pCONT->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
  #include "10_ConSpec/05_SideDoorLight/mSideDoorLight.h"
  #define pCONT_sdlight                static_cast<mSideDoorLight*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
  #include "10_ConSpec/06_ImmersionPanel/mImmersionPanel.h"
  #define pCONT_immersion_cont         static_cast<mImmersionPanel*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
  #include "10_ConSpec/07_TempSensorOnOLED/mTempSensorOLEDBath.h"
  #define pCONT_immersion_cont         static_cast<mTempSensorOLEDBath*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX
  #include "10_ConSpec/08_CellularBlackBox/mCellularBlackBox.h"
  #define pCONT_immersion_cont         static_cast<mCellularBlackBox*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
  #include "10_ConSpec/09_mMAVLink_Decoder_OLED/mMAVLink_Decoder_OLED.h"
  #define pCONT_cont_mavlinoled         static_cast<mMAVLink_Decoder_OLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
  #include "10_ConSpec/10_EnergyOnOLED/mEnergyOnOLED.h"
  #define pCONT_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
  #include "10_ConSpec/11_3DPrinterEnclosure/Printer3D.h"
  #define pCONT_3dprinter        static_cast<mPrinter3D*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
  #include "10_ConSpec/12_TreadmillLogger/mTreadmillLogger.h"
  #define pCONT_treadmill        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
  #include "9_Controller/SDLoggerIMURadiationPattern/mSDLogger.h"
  #define pCONT_cont_imu_rad                static_cast<mSDLoggerIMURadiationPattern*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  #include "9_Controller/SerialPositionalLogger/mSerialPositionalLogger.h"
  #define pCONT_serial_pos_log                  static_cast<mSerialPositionalLogger*>(pCONT->pModule[EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
  #include "9_Controller/SerialCalibrationMeasurmentLogger/mSerialCalibrationMeasurmentLogger.h"
  #define pCONT_serial_calibration_log                  static_cast<mSerialCalibrationMeasurmentLogger*>(pCONT->pModule[EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS
  #include "10_ConSpec/19_MAVLinkFlyingLEDS/mMAVLinkFlyingLEDS.h"
  #define tkr_mavlink_leds                            static_cast<mMavlinkFlyingLEDS*>(pCONT->pModule[TaskerID::CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED
  #include "10_ConSpec/20_DeskSensorsOnOLED/mDeskSensorsOnOLED.h"
  #define pCONT_DeskSensorsOnOLED        static_cast<mDeskSensorsOnOLED*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_ID))
  // Can the above be removed and instead lets get it via the tasker? pCONT->GetModule()
#endif
#ifdef USE_MODULE_CONTROLLER_USERMOD_01
  #include "9_Controller/UserMod_01/mUserMod_01.h"
  #define pCONT_usermod_01                  static_cast<mUserMod_01*>(pCONT->pModule[EM_MODULE_CONTROLLER_USERMOD_01_ID])
#endif


#include  "1_TaskerManager/mTaskerInterface.h"

class mTaskerManager{

  friend class mTaskerInterface;

  public:

    std::vector<mTaskerInterface*> pModule;
    
  private:
    /* Prevent others from being created */
    mTaskerManager(mTaskerManager const& other) = delete;
    mTaskerManager(mTaskerManager&& other) = delete;
    /* Private constructor to prevent instancing. */
    mTaskerManager(){};
    /* Here will be the instance stored. */
    static mTaskerManager* instance;
  public:
    // External function to get instance
    static mTaskerManager* GetInstance(){
      if (instance == nullptr){
        instance = new mTaskerManager();
      }
      return instance;
    };

    // Register a new module
    void addTasker(mTaskerInterface* module);

    // Find a module by unique ID
    mTaskerInterface* GetModule(uint16_t uniqueID) const {
        auto it = std::find_if(pModule.begin(), pModule.end(), 
            [uniqueID](mTaskerInterface* module) {
                return module->GetModuleUniqueID() == uniqueID;
            });
        return (it != pModule.end()) ? *it : nullptr;
    }

    // Find a module by name
    // Flash only
    mTaskerInterface* GetModule_F(const char* name) const {
        char buffer[64];  // Adjust size as needed
        auto it = std::find_if(pModule.begin(), pModule.end(),
            [name, &buffer](mTaskerInterface* module) {
                strcpy_P(buffer, module->GetModuleName());  // Copy from PROGMEM to RAM
                return strcmp(name, buffer) == 0;
            });
        return (it != pModule.end()) ? *it : nullptr;
    }
    mTaskerInterface* GetModule_P(const char* name) const {
        auto it = std::find_if(pModule.begin(), pModule.end(),
            [name](mTaskerInterface* module) {
                return strcmp_P(name, module->GetModuleName()) == 0; // GetModuleName is PROGMEM, and strcmp is RAM only, PROGMEM is read only
            });
        return (it != pModule.end()) ? *it : nullptr;
    }


    // Function to find a module class name by unique ID
    const char* GetModuleName(uint16_t uniqueID) const {
        auto it = std::find_if(pModule.begin(), pModule.end(),
            [uniqueID](mTaskerInterface* module) {
                return module->GetModuleUniqueID() == uniqueID;
            });

        if (it != pModule.end()) {
            return (*it)->GetModuleName();
        }

        return nullptr; // Return nullptr if the module is not found
    }
    
    // Function to find a module unique ID by class name
    // From now on, no module is allowed to be called number 0 as unique ID so it can be reserved for no match
    uint16_t GetModuleID(const char* name) const {
      auto it = std::find_if(pModule.begin(), pModule.end(),
          [name](mTaskerInterface* module) {
              // Use strcmp_P if name is stored in PROGMEM
              return strcmp_P(name, module->GetModuleName()) == 0;
          });

          if (it != pModule.end()) {
              return (*it)->GetModuleUniqueID();
        }

        return 0; // Return 0 or another appropriate invalid ID if the module is not found
    }


    uint8_t Instance_Init();
    
    int8_t Tasker_Interface(uint16_t function);    
        
    uint16_t GetClassCount()
    {
      return pModule.size();
    }

    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
    const char* GetTaskName(uint8_t task);
    #endif
    
    void JSONCommand_Run(char* json);
    

    uint16_t last_function = 255; // 0 will be first

    #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

    struct FUNCTION_EXECUTION_EVENT
    {
      TASKER_FUNCTION_TYPES function_id;
      uint16_t delay_millis;
      uint32_t tSaved_millis;
    };
    FUNCTION_EXECUTION_EVENT FunctionEvent(TASKER_FUNCTION_TYPES function_id, uint16_t delay_millis = 0)
    {
      FUNCTION_EXECUTION_EVENT task;
      task.tSaved_millis = millis();
      task.delay_millis = delay_millis;
      task.function_id = function_id;
      return task;
    }
    std::vector<FUNCTION_EXECUTION_EVENT> function_event_queue;
    #endif // ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE



    #if defined(ENABLE_ADVANCED_DEBUGGING) || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
      char buffer_taskname[50];
    #endif

    #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
    struct DEBUG_MODULE_TIME
    {
      uint16_t seconds_to_keep_stats_before_reset = 0;
      uint16_t max_time = 0;
      // uint16_t min_time = 0;
      uint16_t avg_time = 0;
      uint32_t last_loop_time = 0;
      uint16_t max_function_id = 0; // func_task that caused the max_time
      // Only record them after boot sucessful
    };
    std::vector<DEBUG_MODULE_TIME> debug_module_time;
    #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

    #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
    struct TaskMetrics {
      TASKER_FUNCTION_TYPES task_id;   // Task being monitored
      uint16_t unique_id;
      uint32_t max_time;      // Maximum time in microseconds
      uint32_t min_time;      // Minimum time in microseconds
      uint32_t total_time;    // Total time for calculating the average
      uint32_t count;         // Number of times the task was executed
      uint32_t avg_time;      // Average time in microseconds
    };
    #endif
    #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
    std::vector<TASKER_FUNCTION_TYPES> monitor_task; // Vector to hold the tasks to monitor
    std::vector<TaskMetrics> task_metrics;
    #endif



    

};


#endif
