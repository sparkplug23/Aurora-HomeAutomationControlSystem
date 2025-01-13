#ifndef _CONFIG__FIRMWARE_GROUP__CELLULAR_H
#define _CONFIG__FIRMWARE_GROUP__CELLULAR_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are actively being ran as test devices.
 * Aiming to move these into the garage on a shelf, with a camera looking at them. Everything should be OTA flashed and debugged.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

// #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__ON_SIM800L__OFFICE_TESTBED // Office window testbed, SIM800L, BK-880Q GPs, onboard IMU, SIM800L


// //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/**
 * @brief 2024 Device
 * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
 * 800L as smaller and lighter than 7000G
 * 
 * 
this will be a long term tester for LTE and Mavlink
create one for the office window tester
* GPS serial monitor
* LTE and SMS
* with wifi (perhaps as OTA only mode?)
* pixhawk 2 mavlink connection
* put 433 mhz radio on the pixhawk 2
* have 433mhz on desktop for mavlink connection to it

The new smaller LTE board needs testing too, might need as window tester first.
 * 
 * 
 */
#ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__ON_SIM800L__OFFICE_TESTBED
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"




  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

  #define ENABLE_FREERAM_APPENDING_SERIAL


  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//   #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
  // #define ENABLE_GROUPFEATURE__CELLULAR
  #define ENABLE_GROUPFEATURE__GPS_SERIAL
  // #define ENABLE_GROUPFEATURE__GYRO

  /***********************************
   * SECTION: System Configs
  ************************************/    
  

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define SIM_CARD_PHONE_NUMBER "07518522105"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   115200
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  // #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60" // When deployed, this will text me every 60 seconds with the GPS position
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0" // Turned off for testing
//   #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024




  #ifdef ENABLE_GROUPFEATURE__CELLULAR
    // https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800
    #define USE_MODULE_DRIVERS_MODEM_800L
  #endif


  /***********************************
   * SECTION: Network Configs
  ************************************/    

 #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS


  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

 // add OLED

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  /***********************************
   * SECTION: Controller Configs
  ************************************/  



  #define ENABLE_FEATURE_WATCHDOG_TIMER
    #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
  //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
    // #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
    #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
    // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
    // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
    #define ENABLE_DEBUG_FUNCTION_NAMES
    #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
    // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
    // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
    #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
    // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
    // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
    #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
    // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
    #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
  #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
    #define ENABLE_ADVANCED_DEBUGGING
    #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


  // *************************************************************************************

  // #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
  //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  // #endif 
  // #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
  //   #define USE_MODULE__DRIVERS_MAVLINK_DECODER
  //     #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
  //     #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
  //     #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
  //   #define USE_MODULE_CORE_SERIAL_UART
  //     #define ENABLE_HARDWARE_UART_2
  //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #endif
  // #ifdef USE_GROUPFEATURE__MODEM_GPS
  //   #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  //   #define USE_MODULE_SENSORS_GPS_MODEM
  // #endif 
  // #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
  //   #define DISABLE_NETWORK_WIFI
  //   #define USE_MODULE_NETWORK_CELLULAR
  //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
  //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  //   #define USE_MODULE_SENSORS_INTERFACE
  //   #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
  //   #define USE_MODULE_NETWORK_MQTT
  //   // #define MQTT_SOCKET_TIMEOUT 1
  //   // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
  //    #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
  //   // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  //   // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
  //   #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
  //   // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
  // #endif
  // #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
  //   #define USE_MODULE_NETWORK_WIFI
  //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  //   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
  //   #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
  //   #define MQTT_PORT     1883
  // #endif
  // #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  //   #define DISABLE_NETWORK_WIFI
  //   #define USE_MODULE_NETWORK_CELLULAR
  //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
  //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  //   // #define USE_MODULE_SENSORS_INTERFACE
  //   // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
  //   // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
  //   // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  // #endif

  /**
   *  GPS
   * */
  #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
    #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
    #define USE_MODULE_SENSORS_GPS_SERIAL
    #define ENABLE_GPS_PARSER_NMEA
    #define ENABLE_GPS_PARSER_UBX
    #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
    #define NMEAGPS_DERIVED_TYPES
    // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
    #define NMEAGPS_PARSE_SAVE_MILLIS
    // #define gpsPort Serial1
    // #define D_GPS_BAUD_RATE_FAST    921600
    // #define D_GPS_BAUD_RATE_DEFAULT 9600

    // #define USE_DEVFEATURE_GPS_POLLING_INPUT

    // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    
#define ENABLE_DEVFEATURE__START_STATIC_WHILE
#define ENABLE_DEVFEATURE__START_STATIC_INIT_PORT
#define ENABLE_DEVFEATURE__START_STATIC_LOOP

#define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

    #define NMEAGPS_PARSE_SAVE_MILLIS
    #define gpsPort Serial2
    // #define D_GPS_BAUD_RATE_FAST    921600
    // #define D_GPS_BAUD_RATE_DEFAULT 9600
    // #define D_GPS_BAUD_RATE_DEFAULT 115200
    #define D_GPS_BAUD_RATE_DEFAULT 230400
    // #define D_GPS_BAUD_RATE_DEFAULT 460800

    // #define D_GPS_BAUD_RATE_DEFAULT 115200
    // #define D_GPS_TX_PIN_DEFAULT 19
    // #define D_GPS_RX_PIN_DEFAULT 18

    #define USE_DEVFEATURE_GPS_POLLING_INPUT

    // #define USE_MODULE_CORE__SERIAL

    // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

    #define USE_DEVFEATURE_UBLOX_GLOBAL
    
    // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

    #define USE_DEVFEATURE__UBLOX_TEST_CLASS

    #define ENABLE_DEBUGFEATURE__GPS_COMMANDS_FOR_TESTING





    // #define USE_MODULE_DRIVERS_INTERFACE
    // #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_1
    #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE_NETWORK_CELLULAR
      "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
      "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
      "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
      #endif // USE_MODULE_NETWORK_CELLULAR   

      // modem is on 26/27 per datasheet


      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      // "\"32\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      // "\"33\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","



      #ifdef USE_MODULE_DRIVERS_SDCARD
      "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
      #endif // USE_MODULE_DRIVERS_SDCARD   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";

#endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01















































































// /**
//  * @brief Self contained 2024 testbed, with integrated battery for car tracking for testing
//  * 
//  */
// #ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__2024
//   #define DEVICENAME_CTR          "cellular_locator_01"
//   #define DEVICENAME_FRIENDLY_CTR "Selfcontained tracker with SIM staying" // Antennas covers SIM, but flip the board later
//   #define DEVICENAME_ROOMHINT_CTR "testbed"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//   #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"
//   #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
//   #define UART_CELLULAR_BAUD   921600
//   #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//   #define USE_GROUPFEATURE__MODEM_GPS
//   #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "10" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
//   #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0"
//   #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
// #endif
// /**
//  * @brief 2024 Device
//  * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
//  * 
//  */
// #ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__2024
//   #define DEVICENAME_CTR          "cellular_locator_02"
//   #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
//   #define DEVICENAME_ROOMHINT_CTR "roaming"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//   #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
//   #define SIM_CARD_PHONE_NUMBER "07518522105"
//   #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
//   #define UART_CELLULAR_BAUD   115200
//   #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//   #define USE_GROUPFEATURE__MODEM_GPS
//   #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
//   #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60"
//   #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024
// #endif
// #ifdef SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

  
//   // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//   #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//     #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
//   // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
//   //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

//   #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
//     #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
//     #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
//     #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
//     // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
//     #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//     #define ENABLE_DEBUG_FUNCTION_NAMES
//     #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
//     // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
//     #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
//     // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
//     #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
//     #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
//     #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
//     // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
//     // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
//     #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
//     // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
//     #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
//   #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//     #define ENABLE_ADVANCED_DEBUGGING
//     #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//   #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


//   #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


//   // *************************************************************************************

//   #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
//     #define SERIAL_DEBUG_BAUD_DEFAULT 921600
//   #endif
 
//   #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
//     #define USE_MODULE__DRIVERS_MAVLINK_DECODER
//       #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
//       #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
//       #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
//     #define USE_MODULE_CORE_SERIAL_UART
//       #define ENABLE_HARDWARE_UART_2
//       #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif

//   #ifdef USE_GROUPFEATURE__MODEM_GPS
//     #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//     #define USE_MODULE_SENSORS_GPS_MODEM
//   #endif 
//   #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
//     #define DISABLE_NETWORK_WIFI
//     #define USE_MODULE_NETWORK_CELLULAR
//     #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//     #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//     #define USE_MODULE_SENSORS_INTERFACE
//     #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//     #define USE_MODULE_NETWORK_MQTT
//     // #define MQTT_SOCKET_TIMEOUT 1
//     // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
//      #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
//     // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//     // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
//     #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
//     // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
//   #endif
//   #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
//     #define USE_MODULE_NETWORK_WIFI
//     #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//     #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
//     #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
//     #define MQTT_PORT     1883
//   #endif

//   #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//     #define DISABLE_NETWORK_WIFI
//     #define USE_MODULE_NETWORK_CELLULAR
//     #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//     #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//     // #define USE_MODULE_SENSORS_INTERFACE
//     // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//     // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
//     // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
//   #endif


//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIO_NUMBER "\":{"
//       #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
//       "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
//       #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
//       #ifdef USE_MODULE_NETWORK_CELLULAR
//       "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
//       "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
//       "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
//       "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
//       #endif // USE_MODULE_NETWORK_CELLULAR   
//       #ifdef USE_MODULE_DRIVERS_SDCARD
//       "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
//       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
//       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
//       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
//       #endif // USE_MODULE_DRIVERS_SDCARD   
//       #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
//       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
//       #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
//       "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
//       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
//     "},"
//     "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";


//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_DEVICENAME "\":{"
//       "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
//       "]"
//     "},"   
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
//     "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
//     "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
//   "}";

// #endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G
  #define DEVICENAME_CTR          "testbed_sim7000g"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define MQTT_PORT 51883 //external mqtt broker on TOWER 

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM


  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G


/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_V2
  #define DEVICENAME_CTR          "testbed_sim7000g_v2"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM


  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

    // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker02"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_01
  #define DEVICENAME_CTR          "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION //fails with no network
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define MQTT_PORT 51884 //external mqtt broker on TOWER 

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM

  #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

    // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_02
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  
  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define MQTT_PORT 51883 //external mqtt broker on TOWER 

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM


  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"  
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G


/**
 * @brief 
 * 
 * @brief 6P connector
 * Green VIN (5v)
 * Orange 18
 * Yellow 19
 * White 25
 * Red 26
 * Black gnd
 *
 *
 * @brief 
 * Connecting 6P to pixhawk 2.4.8 with Serial5 enabled will clash with modem serial
 * 
 *
 * Next priority is getting cell/wifi mqtt swapping working well
 * 
 */
#ifdef DEVICE_MAVLINK_DECODER_LOCATOR_LTE_DOWNLINK_01
  #define DEVICENAME_CTR          "mavlink_cellular_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
  // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
  #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 100
  #define ENABLE_DEBUG_FUNCTION_NAMES


  #define USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  // #define USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE
  #define USE_GROUPFEATURE__MODEM_GPS
  // #define USE_GROUPFEATURE__MAVLINK_DECODER
  // #define USE_GROUPFEATURE__OLED_SH1106_MAVLINK
  #define USE_GROUPFEATURE__MQTT_ON_WIFI_AND_CELLULAR

  // *************************************************************************************

  #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
    #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  #endif
 
  #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE_SERIAL_UART
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif

  #ifdef USE_GROUPFEATURE__MODEM_GPS
    #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define USE_MODULE_SENSORS_GPS_MODEM
  #endif 
  #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_SOCKET_TIMEOUT 1
    // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
     #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable
    // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
    // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    // #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
    #define ENABLE_DEVFEATURE_MQTT_ENABLE_CONSECUTIVE_TELEMETRY_TOPICS_SEND_LIMIT // ie if one sends, return early from sending others
  #endif

  #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
    // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  #endif

  #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883
  #endif // USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE

  #if !defined(USE_GROUPFEATURE__MQTT_AS_CELLULAR) && !defined(USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS)
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
  #endif

  #ifdef USE_GROUPFEATURE__MQTT_ON_WIFI_AND_CELLULAR

    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883

  #endif

  #ifdef USE_GROUPFEATURE__OLED_SH1106_MAVLINK
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SH1106
      #define SHOW_SPLASH
    #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
  #endif


// #define UART_BAUD   115200
// #define PIN_DTR     25
// #define PIN_TX      27
// #define PIN_RX      26
// #define PWR_PIN     4

// #define SD_MISO     2
// #define SD_MOSI     15
// #define SD_SCLK     14
// #define SD_CS       13
// #define LED_PIN     12

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    
    // "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G








/**
 * @brief 
 * Minimal ESP32 7000G Modem to log position in realtime and SMS readback
 */
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_01
  #define DEVICENAME_CTR          "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR "Selfcontained tracker with SIM staying" // Antennas covers SIM, but flip the board later
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   921600
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "10" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_02
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "MAVlink ^Pin "
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   921600
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_03
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Plane - Optional LIPO for weight on Plane"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define SIM_CARD_PHONE_NUMBER "07518522105"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   115200
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_04_MQTT_ON_WIFI
  /**
   * @brief Device should run the same code designed for LTE MQTT devices, but connect via WiFi and therefore
   * test that LTE network stuff is not breaking the normal WiFi stuff and stable code is being developed for both methods.
   **/
  #define DEVICENAME_CTR          "cellular_locator_04"
  #define DEVICENAME_FRIENDLY_CTR "Tester - Run MQTT over WiFi without LTE connected"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define USE_GROUPFEATURE__MQTT_AS_WIFI
  #define UART_CELLULAR_BAUD   115200
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  
  // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
  #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #define ENABLE_FEATURE_WATCHDOG_TIMER
    #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
  //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
    #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
    #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
    #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
    // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
    #define ENABLE_DEBUG_FUNCTION_NAMES
    #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
    // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
    #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
    // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
    #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
    // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
    // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
    #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
    // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
    #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
  #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
    #define ENABLE_ADVANCED_DEBUGGING
    #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


  // *************************************************************************************

  #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
    #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  #endif
 
  #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE_SERIAL_UART
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif

  #ifdef USE_GROUPFEATURE__MODEM_GPS
    #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define USE_MODULE_SENSORS_GPS_MODEM
  #endif 
  #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    #define USE_MODULE_NETWORK_MQTT
    // #define MQTT_SOCKET_TIMEOUT 1
    // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
     #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
    // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
    // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
  #endif
  #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883
  #endif

  #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    // #define USE_MODULE_SENSORS_INTERFACE
    // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
    // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE_NETWORK_CELLULAR
      "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
      "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
      "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
      #endif // USE_MODULE_NETWORK_CELLULAR   
      #ifdef USE_MODULE_DRIVERS_SDCARD
      "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
      #endif // USE_MODULE_DRIVERS_SDCARD   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";

#endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01


/**
 * @brief Self contained 2024 testbed, with integrate1100d battery for car tracking for testing
 * 
 */
#ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__ON_SIM9000G_2024
  #define DEVICENAME_CTR                                "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR                       "Selfcontained tracker with SIM staying" // Antennas covers SIM, but flip the board later
  #define DEVICENAME_ROOMHINT_CTR                       "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG  "CellularLocator01"
  #define SIM_CARD_PHONE_NUMBER "07708542353" // spark28007
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD                            921600
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR        "10" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR            "0"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024

  
  #define USE_MODULE_DRIVERS_MODEM_7000G

#endif
/**
 * @brief 2024 Device
 * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
 * 
 */
#ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__ON_SIM9000G_2024
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define SIM_CARD_PHONE_NUMBER "07518522105"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   115200
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  // #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60" // When deployed, this will text me every 60 seconds with the GPS position
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0" // Turned off for testing
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024

  
  #define USE_MODULE_DRIVERS_MODEM_7000G

#endif
/**
 * @brief 2024 Device
 * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
 * 800L as smaller and lighter than 7000G
 */
#ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_03__ON_SIM800L_2024
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define SIM_CARD_PHONE_NUMBER "07518522105"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   115200
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  // #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60" // When deployed, this will text me every 60 seconds with the GPS position
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0" // Turned off for testing
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024


  // #define ENABLE_GROUPFEATURE__CELLULAR
  #define ENABLE_GROUPFEATURE__GPS_SERIAL
  // #define ENABLE_GROUPFEATURE__GYRO


  #ifdef ENABLE_GROUPFEATURE__CELLULAR
    // https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800
    #define USE_MODULE_DRIVERS_MODEM_800L
  #endif

#endif
#ifdef SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

  #define ENABLE_FREERAM_APPENDING_SERIAL



  /***********************************
   * SECTION: System Configs
  ************************************/    
  

  /***********************************
   * SECTION: Network Configs
  ************************************/    

 #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS



  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
  #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #define ENABLE_FEATURE_WATCHDOG_TIMER
    #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
  //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
    // #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
    #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
    // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
    // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
    #define ENABLE_DEBUG_FUNCTION_NAMES
    #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
    // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
    // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
    #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
    // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
    // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
    #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
    // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
    #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
  #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
    #define ENABLE_ADVANCED_DEBUGGING
    #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


  // *************************************************************************************

  // #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
  //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  // #endif 
  // #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
  //   #define USE_MODULE__DRIVERS_MAVLINK_DECODER
  //     #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
  //     #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
  //     #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
  //   #define USE_MODULE_CORE_SERIAL_UART
  //     #define ENABLE_HARDWARE_UART_2
  //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #endif
  // #ifdef USE_GROUPFEATURE__MODEM_GPS
  //   #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  //   #define USE_MODULE_SENSORS_GPS_MODEM
  // #endif 
  // #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
  //   #define DISABLE_NETWORK_WIFI
  //   #define USE_MODULE_NETWORK_CELLULAR
  //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
  //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  //   #define USE_MODULE_SENSORS_INTERFACE
  //   #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
  //   #define USE_MODULE_NETWORK_MQTT
  //   // #define MQTT_SOCKET_TIMEOUT 1
  //   // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
  //    #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
  //   // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  //   // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
  //   #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
  //   // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
  // #endif
  // #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
  //   #define USE_MODULE_NETWORK_WIFI
  //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  //   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
  //   #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
  //   #define MQTT_PORT     1883
  // #endif
  // #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  //   #define DISABLE_NETWORK_WIFI
  //   #define USE_MODULE_NETWORK_CELLULAR
  //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
  //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  //   // #define USE_MODULE_SENSORS_INTERFACE
  //   // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
  //   // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
  //   // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  // #endif

  /**
   *  GPS
   * */
  #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
    #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
    #define USE_MODULE_SENSORS_GPS_SERIAL
    #define ENABLE_GPS_PARSER_NMEA
    #define ENABLE_GPS_PARSER_UBX
    #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
    #define NMEAGPS_DERIVED_TYPES
    // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
    #define NMEAGPS_PARSE_SAVE_MILLIS
    // #define gpsPort Serial1
    // #define D_GPS_BAUD_RATE_FAST    921600
    // #define D_GPS_BAUD_RATE_DEFAULT 9600

    // #define USE_DEVFEATURE_GPS_POLLING_INPUT

    // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    
#define ENABLE_DEVFEATURE__START_STATIC_WHILE
#define ENABLE_DEVFEATURE__START_STATIC_INIT_PORT
#define ENABLE_DEVFEATURE__START_STATIC_LOOP

#define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

    #define NMEAGPS_PARSE_SAVE_MILLIS
    #define gpsPort Serial2
    // #define D_GPS_BAUD_RATE_FAST    921600
    // #define D_GPS_BAUD_RATE_DEFAULT 9600
    // #define D_GPS_BAUD_RATE_DEFAULT 115200
    #define D_GPS_BAUD_RATE_DEFAULT 230400
    // #define D_GPS_BAUD_RATE_DEFAULT 460800

    // #define D_GPS_BAUD_RATE_DEFAULT 115200
    // #define D_GPS_TX_PIN_DEFAULT 19
    // #define D_GPS_RX_PIN_DEFAULT 18

    #define USE_DEVFEATURE_GPS_POLLING_INPUT

    // #define USE_MODULE_CORE__SERIAL

    // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

    #define USE_DEVFEATURE_UBLOX_GLOBAL
    
    // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

    #define USE_DEVFEATURE__UBLOX_TEST_CLASS

    #define ENABLE_DEBUGFEATURE__GPS_COMMANDS_FOR_TESTING





    // #define USE_MODULE_DRIVERS_INTERFACE
    // #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_1
    #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE_NETWORK_CELLULAR
      "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
      "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
      "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
      #endif // USE_MODULE_NETWORK_CELLULAR   


      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      // "\"32\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      // "\"33\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","



      #ifdef USE_MODULE_DRIVERS_SDCARD
      "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
      #endif // USE_MODULE_DRIVERS_SDCARD   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";

#endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM800L
  #define DEVICENAME_CTR          "testbed_sim800l"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SUN_TRACKING

  #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  // #define USE_MODULE_SENSORS_BATTERY_MODEM


  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  //   #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G



/**
 * esp32 version -- SIM800L
 * */
#ifdef DEVICE_TESTBED_FONA_800L_BASIC
  #define DEVICENAME_CTR          "testbed_fona"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Fona 800L"

  #define USE_MODULE_DRIVERS_FONA_CELLULAR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
      "\"23\":\"" D_GPIO_FUNCTION__FONA_POWER_KEY__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__FONA_POWER_STATUS__CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR   "\","
      "\"4\":\"" D_GPIO_FUNCTION__FONA_RESET__CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION__FONA_UART_TX__CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION__FONA_UART_RX__CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "BackUpTank"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif




#endif // _CONFIG__FIRMWARE_GROUP__CELLULAR_H

