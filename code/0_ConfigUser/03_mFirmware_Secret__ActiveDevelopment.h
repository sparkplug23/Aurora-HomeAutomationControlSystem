#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H


/*********************************************************************************************\
 * Devices I am currently working on, kept as its own file to make it easier
 * 
 * 
 * Restructing logging levels
 * INF - a module should only give one per minute (ie general health of module)
 * DBG - detailed logging, sensor readings each time they are read
 * 
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

// #define DEVICE_HVAC_DESK
// #define DEVICE_DESK_SENSOR
// #define DEVICE_TREADMILL_POWER_MONITOR
// #define DEVICE_LIGHTING__LED_MATRIX_BOX_01
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__10INCH
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
// #define DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_01
// #define DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_02
// #define DEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
// #define DEVICE_TESTBED__LED_MATRIX
// #define DEVICE_CAMERA_XIAO_TESTBED
// #define DEVICE_DESKSENSOR // tester with ring led/
// #define DEVICE_TESTBED__FLIGHT__LED_CONTROL_MAVLINK
// #define DEVICE_DOLPHIN__FLIGHT__LED_CONTROL_MAVLINK
// #define DEVICE_TESTBED__ULTRASONIC
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__70__ESP32_PARALLEL_4CH_TRIPLE_CONNECTOR_TESTER
// #define DEVICE_LIGHTING__LIGHTING_EFFECTS__MATRIX_SEGMENT_TESTER
// #define DEVICE_ACTIVE_DEVELOPMENT__DOOR_LIGHTING__OFFICE
// #define DEVICE_TESTBED__GPS_SERIAL
// #define DEVICE_ACTIVE_DEVELOPMENT__SWITCHES_AND_BUTTONS


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION: Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * 
 * Change to ESP32
 * Add ultrasonic sensor to the cold water tank (glue, cut in 2 inches on lid so wire can be brought over and ziptie so it cant fall into the water ever)
 *  DONT FLASH UNTIL ITS ESP32
 * 
 * */
#ifdef DEVICE_TESTBED__ULTRASONIC
  #define DEVICENAME_CTR          "testbed_ultrasonic"
  #define DEVICENAME_FRIENDLY_CTR "Attic Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Attic"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  
  // #define USE_MODULE_SENSORS_SUN_TRACKING

  //   #define USE_MODULE_SENSORS_INTERFACE
  //   #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  #define USE_MODULE_SENSORS_SR04
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    // #define SONAR_SERIAL_BAUD 115200
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SR04
      "\"19\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif 
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\"," 
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Attic"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Attic"  
  #define D_DEVICE_SENSOR_SR04_FRIENDLY_NAME_LONG "ColdWaterHeaderTank"  

  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Cold Water Tank"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,50,199,3,0,0,0,102]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Attic Top"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,100,30,7,142,92,154]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Attic Bottom"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,100,30,7,143,73,252]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SR04_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_SR04_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ""
      "}"   
    "}"
  "}";

#endif


/*
 * ** Main board **************************************************************************


Current plan, summary for myself:

Main board

esp32
multiple ds18b20 soldered in place onto 1 pin
at least one ds18b20 on a wire (eg. to attach to oil radiator), on another pin
buzzer/peizo speaker (new code)
stuck in place, one strip of 4 sk6812 (to show when relays are running)
with wire, a connector for a strip that will represent heatmap like my tank, for developing code. It will use those ds18b20 on board and show their temp as colour gradient.
strip of maybe 60 leds, powered via INA219 power meter
BME280 (hum, temp, pressure)
BME680 (hum, temp, pressure and gas) [to test 280 and 680 side by side]
DHT22 (maybe, just to clean that code up)
OLED display showing relay time on, temp and energy

via 4pin header connection

Nextion 2.5 inch, mostly to dev making one of them control the relays with schedules (ie to further dev my hvac heating)

via RJ45

The 3D printed board
now printing again, to include a 5V 4A PSU
May need to add two power connections too, so its not just the cat5e wire to let me drive the LEDs full power
4 30A relays
4 PZEM





  ESP32
  5v Jack input (screw terminals that can accept jack)
  RJ45 Power
  RJ45 Sensor
  4P Nextion
  dual pixel strip (relay state and colourbar)

 * ** Power board **************************************************************************
  - To keep power cleaner, make a daugter board that holds all relays and PZEM then use RJ45 to connect back to main board

  - create small PCB to breakout the RJ45 and distribute power to relays and PZEM. This board should also hold the diodes for PZEM. Screw this board to a 3d print bed.

  3D print should actually be mounted into an enclosed box, use glands to secure mains wires. Box could also include 5V PSU and backfeed the 5V. A jumper should be added on the main board to isolate this backfeed for USB.

  Relay1: Oil heater
  Relay2: Fan heater
  Relay3: Dryer
  Relay4: Foot heater (with "power_cycle" that includes a new opiton to turn off/on again the source. Say floor is set to say on for 5 hours, it should recycle every 45 minutes within that 5 hours)

 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           REL2           21*           Immersion relay (5v IO)
 * bl/w          REL3            12*           DHT22?     
 * w/bl          REL4            13*           UNUSED   
 * g/w           REL1            3V3           
 * w/br          SER2_TX            
 * br/w          SER2_RX         14*           ds18b20 water, 4k7 pulled high        (comes from blue by connector)   NOT WORKING, wiring too long, Needs rerouted
 * 
 * 
 * 
 * ** Sensor board **************************************************************************
 * 
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           1Wire          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME
 * w/bl          I2C            23*           BME
 * g/w           3V3            3V3           White
 * w/br                                       NC
 * br/w          DHT22          25*                  
 * 
 * 
 * 
 * The usual devices, connected back via RJ45 as a "all in one tester".
 * 
 * 1 BME280
 * 1 BME680
 * X DS18B20 (across 2 pins)
 * NO DHT, bedroom desk can test that
 * 
 * 
 * ** Nextion board **************************************************************************
 * 
 * Just use the already designed 3D desk print for the RJ45, then use a 4pin header back (the screw ones used for printer) to give me 2m nextion panel
 * 
 * 
 * 
 * 3D print design with screw like holders inside plugs
 * 
 * 
 **/
/**
 * 
 * 
 * Ethernet R
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    left LED (upstairs)              D27
 * bl/w   centre Relay (downstairs)        D5
 * w/bl   centre LED (downstairs)          D33
 * g/w    left Relay (upstairs)            D18
 * w/br   right LED (boiler)               D26
 * br/w   right Relay (boiler)             D19
 * 
 * Ethernet N
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    SK6812 D4
 * bl/w   NC
 * w/bl   NC
 * g/w    NC
 * w/br   RX2 of esp32 from nextion
 * br/w   TX2 of esp32 from nextion    
 * 

*/
#ifdef DEVICE_HVAC_DESK
  #define DEVICENAME_CTR          "hvac_desk"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1241
  #define ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  #define ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
  #define USE_MODULE_CORE_FILESYSTEM
  #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT

  #define ESP32

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
  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRASNMITTED
  // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS
  
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRASNMITTED
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING

  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 200

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT

  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE


  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__LIGHTING
  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  #define ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
  #define ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  /***********************************
   * SECTION: System Configs
  ************************************/     

  // #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES


  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.

  #define ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

  // #define ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY

  #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_MINUTE

  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI


  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SUN_TRACKING     
      #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
        #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
      #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
        // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

      // #define ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

      // #define USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX


  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define USE_MODULE_SENSORS_PIR
    #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
  // #define USE_MODULE_SENSORS_SWITCHES
  #endif

  // #define ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
    #define USE_MODULE_DISPLAYS_NEXTION
    #define NEXTION_DEFAULT_PAGE_NUMBER 6  
      #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
      // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
      #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
      #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
      // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
      #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
      #define ENABLE_DEVFEATURE_NEXTION_WEBUI
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
      #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
      #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
      #define USE_MODULE_NETWORK_WEBSERVER
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
        #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
        
      DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
      R"=====(
      {
        "PageNames": [
          "boot_flash",
          "boot",
          "message",
          "multiline",
          "logger",
          "heating",
          "hvacdesk",
          "main"
        ],
        "DefaultPageName":"hvacdesk",
        "DefaultBaud":115200,
        "TargetBaud":115200,
        "ObjectNameID": {
          "hdIconHAI": 2,
          "hdTimeHAI": 6,
          "hdBoostHAI": 11,
          "hdAutoHAI": 16,
          "powHAI": 21,
          "hdIconFLR": 3,
          "hdTimeFLR": 7,
          "hdBoostFLR": 12,
          "hdAutoFLR": 17,
          "powFLR": 22,
          "hdIconFAN": 4,
          "hdTimeFAN": 8,
          "hdBoostFAN": 13,
          "hdAutoFAN": 18,
          "powFAN": 23,
          "hdIconOIL": 5,
          "hdTimeOIL": 9,
          "hdBoostOIL": 14,
          "hdAutoOIL": 19,
          "powOIL": 24
        },
        "MQTTSubscribe":[
          "openhab_broadcast/nextion/group/hvac_home",
          "openhab_broadcast/nextion/group/hvac_desk_power"
        ]
      }
      )=====";

  #endif // USE_MODULE_DISPLAYS_NEXTION
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
   #define USE_MODULE_DRIVERS_INTERFACE
   #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    
    
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":21,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          144
        ],
        "ColourPalette":"Live Solar Sky",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          144,
          148
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment2": {
        "Name":"Leg",
        "PixelRange": [
          148,
          184
        ],
        "ColourPalette":"Rainbow 16",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment3": {
        "Name":"Floor",
        "PixelRange": [
          184,
          192
        ],
        "ColourPalette":"Live SunEL CCT",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";

    

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
    #define USE_MODULE_ENERGY_INTERFACE
  #endif
  
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
    #define USE_MODULE_ENERGY_PZEM004T_V3
      #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
    #define MAX_ENERGY_SENSORS 4
    #define MAX_PZEM004T_DEVICES 4
    #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
    #define USE_MODULE_ENERGY_INA219
    // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  #define USE_MODULE_CONTROLLER_HVAC
     #define HEATING_DEVICE_MAX 4
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

    #ifdef USE_LIGHTING_TEMPLATE
      #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
      #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
        #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
    #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
   #define USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED

  #endif // ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

   // add new controller custom that instead shows the other sensors as PZEM is on nextion
   /**
    * @brief 
    * BME680 Temp
    * BME280 Temp
    * Lux
    * LED current
    * 
    * Display 2, show all 4 db18s20. So every 5 seconds, flip between 1 and 2
    * 
    */

   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

  #define USE_MQTT_TEMPLATE
  DEFINE_PGM_CTR(MQTT_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"15\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 15 - PIR
      // 2
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  

  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME
  
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Desk"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "]"
  "}";



  
#endif



/*
 * ** Main board **************************************************************************

This is a new device
To be known as "testbed_presense_sensor". It will be installed in the office, perhaps under the desk

 
TOF VL53L1X
TOF VL53L0X (I got both types)
Ultrasonic sensor (the cheap blue kind)
The new small PIR sensor I am testing
new 26GHz radar sensor





*/
#ifdef DEVICE_DESK_SENSOR   // is this the one moved into the garage??
  #define DEVICENAME_CTR          "desk_sensor"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1241
  #define ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  #define ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
  #define USE_MODULE_CORE_FILESYSTEM
  #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT

  #define ESP32

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
  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRASNMITTED
  // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS
  
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRASNMITTED
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING

  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 200

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT

  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE


  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__LIGHTING
  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  #define ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
  #define ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  /***********************************
   * SECTION: System Configs
  ************************************/     

  // #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES


  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.

  #define ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

  // #define ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY

  #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_MINUTE

  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI


  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SUN_TRACKING     
      #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
        #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
      #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
        // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

      // #define ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

      // #define USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX


  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define USE_MODULE_SENSORS_PIR
    #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
  // #define USE_MODULE_SENSORS_SWITCHES
  #endif

  // #define ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
    #define USE_MODULE_DISPLAYS_NEXTION
    #define NEXTION_DEFAULT_PAGE_NUMBER 6  
      #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
      // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
      #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
      #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
      // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
      #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
      #define ENABLE_DEVFEATURE_NEXTION_WEBUI
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
      #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
      #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
      #define USE_MODULE_NETWORK_WEBSERVER
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
        #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
        
      DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
      R"=====(
      {
        "PageNames": [
          "boot_flash",
          "boot",
          "message",
          "multiline",
          "logger",
          "heating",
          "hvacdesk",
          "main"
        ],
        "DefaultPageName":"hvacdesk",
        "DefaultBaud":115200,
        "TargetBaud":115200,
        "ObjectNameID": {
          "hdIconHAI": 2,
          "hdTimeHAI": 6,
          "hdBoostHAI": 11,
          "hdAutoHAI": 16,
          "powHAI": 21,
          "hdIconFLR": 3,
          "hdTimeFLR": 7,
          "hdBoostFLR": 12,
          "hdAutoFLR": 17,
          "powFLR": 22,
          "hdIconFAN": 4,
          "hdTimeFAN": 8,
          "hdBoostFAN": 13,
          "hdAutoFAN": 18,
          "powFAN": 23,
          "hdIconOIL": 5,
          "hdTimeOIL": 9,
          "hdBoostOIL": 14,
          "hdAutoOIL": 19,
          "powOIL": 24
        },
        "MQTTSubscribe":[
          "openhab_broadcast/nextion/group/hvac_home",
          "openhab_broadcast/nextion/group/hvac_desk_power"
        ]
      }
      )=====";

  #endif // USE_MODULE_DISPLAYS_NEXTION
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
   #define USE_MODULE_DRIVERS_INTERFACE
   #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    
    
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":21,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          144
        ],
        "ColourPalette":"Live Solar Sky",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          144,
          148
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment2": {
        "Name":"Leg",
        "PixelRange": [
          148,
          184
        ],
        "ColourPalette":"Rainbow 16",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment3": {
        "Name":"Floor",
        "PixelRange": [
          184,
          192
        ],
        "ColourPalette":"Live SunEL CCT",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";

    

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
    #define USE_MODULE_ENERGY_INTERFACE
  #endif
  
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
    #define USE_MODULE_ENERGY_PZEM004T_V3
      #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
    #define MAX_ENERGY_SENSORS 4
    #define MAX_PZEM004T_DEVICES 4
    #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
    #define USE_MODULE_ENERGY_INA219
    // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  #define USE_MODULE_CONTROLLER_HVAC
     #define HEATING_DEVICE_MAX 4
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

    #ifdef USE_LIGHTING_TEMPLATE
      #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
      #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
        #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
    #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
   #define USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED

  #endif // ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

   // add new controller custom that instead shows the other sensors as PZEM is on nextion
   /**
    * @brief 
    * BME680 Temp
    * BME280 Temp
    * Lux
    * LED current
    * 
    * Display 2, show all 4 db18s20. So every 5 seconds, flip between 1 and 2
    * 
    */

   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

  #define USE_MQTT_TEMPLATE
  DEFINE_PGM_CTR(MQTT_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"15\":\""  D_GPIO_FUNCTION_PIR_1_CTR "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 15 - PIR
      // 2
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  

  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME
  
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Desk"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "]"
  "}";



  
#endif


/**
 * @brief 
 * Testbed for LED control via Mavlink
 * 
 * Test 1:
 *  - Control LED strip and set to simply be colours, use my segments to make different patterns!
 * 
 * Test 2:
 *  - Decode mavlink, change wing colours based an altitude. Or at least, when below 30, then 20m, switch to "landing" colours
 * 
 * Test 3:
 *  - Based on TX16S left slider, have maybe 4 modes of lighting and switch between them. Have "altitude" mode be a mode
 *    -- Mode 1: 
 *        -- Altitude changes speed of flashing, and or colour?
 * 
 * 
 */
#ifdef DEVICE_TESTBED__FLIGHT__LED_CONTROL_MAVLINK
  #define DEVICENAME_CTR          "testbed_mavlink_led"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

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
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  #define ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING  
  #define ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
  #define ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  /***********************************
   * SECTION: System Configs
  ************************************/     

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define USE_MODULE_SENSORS_SUN_TRACKING

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.


  /***********************************
   * SECTION: Network Configs
  ************************************/    

 #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 20

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_INTERFACE
  //     #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  //   #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //     #define DS18X20_MAX_SENSORS 20
  //       #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  // #endif 
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_BME
  //     #define ENABLE_DEVFEATURE_BME680
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  //   #define USE_MODULE_SENSORS_SUN_TRACKING     
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  //   #define USE_MODULE_SENSORS_BH1750
  // #endif
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     #define NEXTION_DEFAULT_PAGE_NUMBER 6  
  //       #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  //       // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
  //       #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  //       #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
  //       // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
  //       #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
  //       #define ENABLE_DEVFEATURE_NEXTION_WEBUI
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
  //       #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  //       #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
  //       #define USE_MODULE_NETWORK_WEBSERVER
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
  //         #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
  //         // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS
  //         // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC1  "openhab_broadcast/nextion/group/hvac_home"
  //         // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC2  "openhab_broadcast/nextion/group/hvac_desk_power"
      
  //       DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  //       R"=====(
  //       {
  //         "PageNames": [
  //           "boot_flash",
  //           "boot",
  //           "message",
  //           "multiline",
  //           "logger",
  //           "heating",
  //           "hvacdesk",
  //           "main"
  //         ],
  //         "DefaultPageName":"hvacdesk",
  //         "DefaultBaud":115200,
  //         "TargetBaud":115200,
  //         "ObjectNameID": {
  //           "hdIconHAI": 2,
  //           "hdTimeHAI": 6,
  //           "hdBoostHAI": 11,
  //           "hdAutoHAI": 16,
  //           "powHAI": 21,
  //           "hdIconFLR": 3,
  //           "hdTimeFLR": 7,
  //           "hdBoostFLR": 12,
  //           "hdAutoFLR": 17,
  //           "powFLR": 22,
  //           "hdIconFAN": 4,
  //           "hdTimeFAN": 8,
  //           "hdBoostFAN": 13,
  //           "hdAutoFAN": 18,
  //           "powFAN": 23,
  //           "hdIconOIL": 5,
  //           "hdTimeOIL": 9,
  //           "hdBoostOIL": 14,
  //           "hdAutoOIL": 19,
  //           "powOIL": 24
  //         }
  //       }
  //       )=====";

  //   #endif // USE_MODULE_DISPLAYS_NEXTION


  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  #ifdef ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE__SERIAL
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST

      #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

      #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE 3

      // #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3500


  #endif

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING

    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    // #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    // #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
    // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    // #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    #define USE_MODULE_NETWORK_WEBSERVER
    #define ENABLE_WEBSERVER_LIGHTING_WEBUI


    #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

    
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    




  #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

  
  #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
  

  #define USE_LIGHTING_TEMPLATE

  
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":13,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":0,
          "Length":16
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          16
        ],
        "ColourPalette":"IceCream Floats",
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 0
    }
    )=====";

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
  //   #define USE_MODULE_ENERGY_INTERFACE
  // #endif
  
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  //   #define USE_MODULE_ENERGY_PZEM004T_V3
  //     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  //   #define MAX_ENERGY_SENSORS 4
  //   #define MAX_PZEM004T_DEVICES 4
  //   #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  //   #define USE_MODULE_ENERGY_INA219
  //   // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  // #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  // #define USE_MODULE_CONTROLLER_HVAC
  //    #define HEATING_DEVICE_MAX 4
  //    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
  //    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

  //   #ifdef USE_LIGHTING_TEMPLATE
  //     #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  //     #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  //       #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
  //   #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED

  
  #ifdef ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

      #define USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS


  #endif
   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      // "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      // "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";



  
#endif


/**
 * @brief 
 * Testbed for LED control via Mavlink
 * 
 * Test 1:
 *  - Control LED strip and set to simply be colours, use my segments to make different patterns!
 * 
 * Test 2:
 *  - Decode mavlink, change wing colours based an altitude. Or at least, when below 30, then 20m, switch to "landing" colours
 * 
 * Test 3:
 *  - Based on TX16S left slider, have maybe 4 modes of lighting and switch between them. Have "altitude" mode be a mode
 *    -- Mode 1: 
 *        -- Altitude changes speed of flashing, and or colour?
 * 
 * 
 */
#ifdef DEVICE_DOLPHIN__FLIGHT__LED_CONTROL_MAVLINK
  #define DEVICENAME_CTR          "flight_mavlink_led"
  #define DEVICENAME_FRIENDLY_CTR "MAVLink Dolphin Lighting"
  #define DEVICENAME_ROOMHINT_CTR "Dolphin"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

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
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  #define ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING  
  #define ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
  #define ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  /***********************************
   * SECTION: System Configs
  ************************************/     

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define USE_MODULE_SENSORS_SUN_TRACKING

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.


  /***********************************
   * SECTION: Network Configs
  ************************************/    

 #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 20
// #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 
// #define DISABLE_NETWORK

#define ENABLE_DEVFEATURE_NETWORK__BLOCK_CONNECT_PUSH_BACKOFF_LONG_AS_TEMP_SOLUTION_TO_NO_WIFI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_INTERFACE
  //     #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  //   #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //     #define DS18X20_MAX_SENSORS 20
  //       #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  // #endif 
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_BME
  //     #define ENABLE_DEVFEATURE_BME680
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  //   #define USE_MODULE_SENSORS_SUN_TRACKING     
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  //   #define USE_MODULE_SENSORS_BH1750
  // #endif
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     #define NEXTION_DEFAULT_PAGE_NUMBER 6  
  //       #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  //       // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
  //       #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  //       #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
  //       // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
  //       #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
  //       #define ENABLE_DEVFEATURE_NEXTION_WEBUI
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
  //       #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  //       #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
  //       #define USE_MODULE_NETWORK_WEBSERVER
  //       #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
  //         #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
  //         // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS
  //         // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC1  "openhab_broadcast/nextion/group/hvac_home"
  //         // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC2  "openhab_broadcast/nextion/group/hvac_desk_power"
      
  //       DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  //       R"=====(
  //       {
  //         "PageNames": [
  //           "boot_flash",
  //           "boot",
  //           "message",
  //           "multiline",
  //           "logger",
  //           "heating",
  //           "hvacdesk",
  //           "main"
  //         ],
  //         "DefaultPageName":"hvacdesk",
  //         "DefaultBaud":115200,
  //         "TargetBaud":115200,
  //         "ObjectNameID": {
  //           "hdIconHAI": 2,
  //           "hdTimeHAI": 6,
  //           "hdBoostHAI": 11,
  //           "hdAutoHAI": 16,
  //           "powHAI": 21,
  //           "hdIconFLR": 3,
  //           "hdTimeFLR": 7,
  //           "hdBoostFLR": 12,
  //           "hdAutoFLR": 17,
  //           "powFLR": 22,
  //           "hdIconFAN": 4,
  //           "hdTimeFAN": 8,
  //           "hdBoostFAN": 13,
  //           "hdAutoFAN": 18,
  //           "powFAN": 23,
  //           "hdIconOIL": 5,
  //           "hdTimeOIL": 9,
  //           "hdBoostOIL": 14,
  //           "hdAutoOIL": 19,
  //           "powOIL": 24
  //         }
  //       }
  //       )=====";

  //   #endif // USE_MODULE_DISPLAYS_NEXTION


  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  #ifdef ENABLE_TEMPLATE_SECTION__DRIVERS_MAVLINK
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE__SERIAL
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
      // #define HARDWARE_UART_2_BAUD_RATE_SPEED  115200  //D_GPS_BAUD_RATE_FAST

      #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

      // #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE 2

      // #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3500


  #endif

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING__TESTRING

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    // #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    // #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
    // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    // #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    #define USE_MODULE_NETWORK_WEBSERVER
    #define ENABLE_WEBSERVER_LIGHTING_WEBUI


    // #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

    
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    


  /***********************************
   * SECTION: System Configs
  ************************************/     

 #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  


  /***********************************
   * SECTION: Network Configs
  ************************************/    
 #ifndef ESP8266
  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
#endif
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  //  #define ENABLE_DEVFEATURE_MAVLINK__USE_TIME_FOR_RANGE_TESTING
  // #define ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING
  // #define ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM

  #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

  #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER

  #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
  
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
  #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
    /********* Group: Testing ************************/
      // Phase out
    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    // #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    

    
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  #define ENABLE_FEATURE_LIGHTING__EFFECTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
  // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS



  // #define FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

  
  #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
  

  #define USE_LIGHTING_TEMPLATE

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

  // #undef  ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

  // #define ENABLE_TEMPLATE_LIGHTING__SHOW_BUS_CONNECTED_RGB
  // #define ENABLE_TEMPLATE_LIGHTING__SHOW_BUS_CONNECTED_RGBW
  #define ENABLE_TEMPLATE_LIGHTING__DESIGN_1
  // #define ENABLE_TEMPLATE_LIGHTING__TEST_WING_ONLY

  #ifdef ENABLE_TEMPLATE_LIGHTING__DESIGN_1
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":18,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":41,
          "Reversed":1
        },
        {
          "Pin":13,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":41,
          "Length":41
        },
        {
          "Pin":14,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":82,
          "Length":18,
          "Reversed":1
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":100,
          "Length":18
        },
        {
          "Pin":12,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":118,
          "Length":21
        },
        {
          "Pin":2,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":139,
          "Length":25
        },
        {
          "Pin":21,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":164,
          "Length":16,
          "Reversed":1
        },
        {
          "Pin":19,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":180,
          "Length":16
        },
        {
          "Pin":5,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":196,
          "Length":16
        }
      ]
    }
    )=====";
    // ,
    //     {
    //       "Pin":15,
    //       "ColourOrder":"GRBC",
    //       "BusType":"SK6812_RGBW",
    //       "Start":190,
    //       "Length":10
    //     }

    #define USE_LIGHTING_TEMPLATE_ANOTHER
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER)
    R"=====(
    {
      "Segment0": {
        "Name":"Wing",
        "PixelRange": [
          0,
          82
        ],
        "ColourPalette":0,
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment1": {
        "Name":"Fuselage",
        "ColourPalette":1,
        "PixelRange": [
          82,
          118
        ],
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment2": {
        "Name":"Hatches",
        "PixelRange": [
          118,
          164
        ],
        "ColourPalette":2,
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment3": {
        "Name":"Rudder",
        "PixelRange": [
          164,
          196
        ],
        "ColourPalette":3,
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "Segment4": {
        "Name":"Nose",
        "PixelRange": [
          196,
          212
        ],
        "ColourPalette":4,
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "CCT_TempPercentage":90,
        },
        "Effects": {
          "Function":"Solid Colour"
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    }
    )=====";

    // #define USE_LIGHTING_TEMPLATE_ANOTHER
    // DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER)
    // R"=====(
    // {
    //   "Segment0": {
    //     "Name":"View Bus",
    //     "PixelRange": [
    //       0,
    //       214
    //     ],
    //     "ColourPalette":"Rainbow 16",
    //     "Effects": {
    //       "Function":"Gradient"
    //     },
    //     "ColourType":3,
    //     "Transition": {
    //       "TimeMs": 200,
    //       "RateMs": 1000
    //     },
    //     "BrightnessRGB": 100,
    //     "BrightnessCCT": 0
    //   },
    //   "BrightnessRGB": 5,
    //   "BrightnessCCT": 0
    // }
    // )=====";


  #endif // ENABLE_TEMPLATE_LIGHTING__DESIGN_1
  
  #ifdef ENABLE_TEMPLATE_LIGHTING__TEST_WING_ONLY
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":18,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":41
        }
      ]
    }
    )=====";
    
    #define USE_LIGHTING_TEMPLATE_ANOTHER
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER)
    R"=====(
    {
      "Segment0": {
        "Name":"Wing",
        "PixelRange": [
          0,
          41
        ],
        "ColourPalette":0,
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 2,
        "BrightnessCCT": 2
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    }
    )=====";

  #endif // ENABLE_TEMPLATE_LIGHTING__TEST_WING_ONLY


  #ifdef ENABLE_TEMPLATE_LIGHTING__SHOW_BUS_CONNECTED_RGB

    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":0,
          "Length":41,
          "Reversed":1
        },
        {
          "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":41,
          "Length":41
        },
        {
          "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":82,
          "Length":18
        },
        {
          "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":100,
          "Length":18
        },
        {
          "Pin":15,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":118,
          "Length":21
        },
        {
          "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":139,
          "Length":21
        },
        {
          "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":160,
          "Length":10
        },
        {
          "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":170,
          "Length":10
        },
        {
          "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":180,
          "Length":10
        },
        {
          "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
          "Start":190,
          "Length":10
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          200
        ],
        "ColourPalette":"IceCream Floats",
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 0
    }
    )=====";

  #endif
  #ifdef ENABLE_TEMPLATE_LIGHTING__FLIGHT_TYPE_1

  
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    /**
     * @brief 
     * 
     * Wing Length: 41
     * Fuselage Length: 18
     * Front Cover: 21
     * Rear Cover: 25
     * Rudder: --
     * 
     * Bus Order
     * ** Wing Right
     * ** Wing Left
     * ** Fuselage Right
     * ** Fuselage Left
     * ** Front Cover
     * ** Rear Cover
     * ** Rudder Right
     * ** Rudder Left
     * 
     * 
     */
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":41,
          "Reversed":1
        },
        {
          "Pin":13,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":41,
          "Length":41
        },
        {
          "Pin":18,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":82,
          "Length":18
        },
        {
          "Pin":5,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":100,
          "Length":18
        },
        {
          "Pin":15,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":118,
          "Length":21
        },
        {
          "Pin":12,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":139,
          "Length":25
        },
        {
          "Pin":2,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":160,
          "Length":10
        },
        {
          "Pin":14,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":170,
          "Length":10
        },
        {
          "Pin":19,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":180,
          "Length":10
        },
        {
          "Pin":21,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":190,
          "Length":10
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          82
        ],
        "ColourPalette":"IceCream Floats",
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Gradient",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "ColourType":3,
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          82,
          118
        ],
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3
      },
      "Segment2": {
        "PixelRange": [
          118,
          200
        ],
        "ColourPalette":"IceCream Floats",
        "Effects": {
          "Function":"Gradient"
        },
        "ColourType":3,
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 0
    }
    )=====";

  #endif // ENABLE_TEMPLATE_LIGHTING__FLIGHT_TYPE_1

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
  //   #define USE_MODULE_ENERGY_INTERFACE
  // #endif
  
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  //   #define USE_MODULE_ENERGY_PZEM004T_V3
  //     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  //   #define MAX_ENERGY_SENSORS 4
  //   #define MAX_PZEM004T_DEVICES 4
  //   #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  // #endif
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  //   #define USE_MODULE_ENERGY_INA219
  //   // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  // #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  // #define USE_MODULE_CONTROLLER_HVAC
  //    #define HEATING_DEVICE_MAX 4
  //    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
  //    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

  //   #ifdef USE_LIGHTING_TEMPLATE
  //     #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  //     #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  //       #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
  //   #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED

  
  #ifdef ENABLE_TEMPLATE_SECTION__CUSTOM__MAVLINK_FLYING_LEDS

      #define USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS


  #endif
   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      // "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      // "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";



  
#endif


/**
 * @brief 
 * 
 * - This will be the test device, full of as many sensors as possible, and also use 4 LEDs to emulate relay outputs
 * - This will be used as a rapid/testbed device for improving as many sensors as possible (at minimum, the "...sensor" devices)
 * 
 * Modules to be tested
 * - DS18B20
 * - DHT22
 * - BME280
 * - BH1750
 * - Status LEDs (mqtt/wifi down)
 * - Nextion = OTA updating
 * - rgbW lights = Notification lights as a mode
 * - Motion sensor = including nicely having interface showing "event" of motion
 * - ESP32 Touch inputs
 * - DAC Output - Mono Speaker Audio
 * - ADC Input (probably with LDR)
 * - Audio Input? MCP3208
 * 
 * development branch only
 * 
 * Use one pixel to show segment1 effect paired with sun elevation, where sunrise to midday, then midday to sunset is different colours
 * another pixel to show segment2 effect paired with sun azimuth, where sunrise point to sunset point is full colour wheel
 * 
 * add solar value I can poll to get time of day (day/dusk/twilight/) and direction of sun movement (rising/falling)
 * 
 * Temporary sensor tester with dual DHT
 * 
 * 
 **/
#ifdef DEVICE_DESKSENSOR
  #define DEVICENAME_CTR          "desksensor"
  #define DEVICENAME_FRIENDLY_CTR "Desk Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define MQTT_PORT     1883

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

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  /***********************************
   * SECTION: System Configs
  ************************************/     

  

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SUN_TRACKING

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SUN_TRACKING     
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif


  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
    #define USE_MODULE_SENSORS_DHT
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
    #define USE_MODULE_SENSORS_BH1750
    #define USE_MODULE_SENSORS_SWITCHES
    #define USE_MODULE_SENSORS_BUTTONS
    #define USE_MODULE_SENSORS_PIR
    #define USE_MODULE_SENSORS_LDR_BASIC

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
    #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
    #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  


    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #define USE_MODULE_DISPLAYS_NEXTION
    #ifdef USE_MODULE_DISPLAYS_NEXTION
      #define NEXTION_DEFAULT_PAGE_NUMBER 6  
        #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
        // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
        #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
        #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
        // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
        #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
          #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC1  "openhab_broadcast/nextion/group/hvac_home"
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC2  "openhab_broadcast/nextion/group/hvac_desk_power"
      
        DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
        R"=====(
        {
          "PageNames": [
            "boot_flash",
            "boot",
            "message",
            "multiline",
            "logger",
            "heating",
            "hvacdesk",
            "main"
          ],
          "DefaultPageName":"hvacdesk",
          "DefaultBaud":115200,
          "TargetBaud":115200,
          "ObjectNameID": {
            "hdIconHAI": 2,
            "hdTimeHAI": 6,
            "hdBoostHAI": 11,
            "hdAutoHAI": 16,
            "powHAI": 21,
            "hdIconFLR": 3,
            "hdTimeFLR": 7,
            "hdBoostFLR": 12,
            "hdAutoFLR": 17,
            "powFLR": 22,
            "hdIconFAN": 4,
            "hdTimeFAN": 8,
            "hdBoostFAN": 13,
            "hdAutoFAN": 18,
            "powFAN": 23,
            "hdIconOIL": 5,
            "hdTimeOIL": 9,
            "hdBoostOIL": 14,
            "hdAutoOIL": 19,
            "powOIL": 24
          }
        }
        )=====";

    #endif // USE_MODULE_DISPLAYS_NEXTION

  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  


    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
    #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    #define USE_MODULE_NETWORK_WEBSERVER
    #define ENABLE_WEBSERVER_LIGHTING_WEBUI

    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT

    
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT

    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":0,
          "Length":16
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          16
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 100
    }
    )=====";


  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_GPIOC "\":{"
  //     #ifdef USE_MODULE_DRIVERS_RELAY
  //     "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
  //     "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
  //     "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
  //     "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
  //     #endif
  //     "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
  //     "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
  //     #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //     "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
  //     #endif    
  //     #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
  //     "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
  //     "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
  //     #endif
  //     #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
  //     "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
  //     #endif
  //     "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
  //     // 32 - LED Strip External
  //     // 21 - LED Strip Onboard
  //     // 25?
  //     // 
  //   "},"
  //   "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  //   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";
  
  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      /**
       * @brief Right side
       **/
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023   
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif // USE_MODULE_SENSORS__DS18X20_ESP32_2023
      // GPIO1 - TX0 - Debug Serial TX
      // GPIO3 - RX0 - Debug Serial RX
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
      "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
      "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","      
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
      "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
      #endif
      // #if defined(USE_MODULE_DRIVERS_RELAY)// && !defined(USE_MODULE_DISPLAYS_NEXTION_V2) // Only debug pins
      // "\"2\":\""  GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\"," //pin2 - blue led
      // "\"15\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
      // #endif
      // GPIO0 - ADC2 CH1
      /**
       * @brief Left side
       **/
      // EN
      // 36 - INPUT ONLY - VP
      // 39 - INPUT ONLY - VN
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
      "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
      #endif
      // 35 - INPUT ONLY - adc1_7
      // 32 - Touch9 (Debug Header 1)
      // 33 - Touch8 (Debug Header 2)
      // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      // 14 (Debug Header 4)
      // 12 (Debug Header 5)
      // 13 (Debug Header 6)
      // Can I introduce a way that a comma at the end, does not make a broken json?
      "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "}"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";




  
  // /***********************************
  //  * SECTION: Lighting Configs
  // ************************************/    

  // #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS 
  // #define ANIMATION_UPDATOR_TIME_MINIMUM 20
  // #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
  // #define ENABLE_DEBUG_MANUAL_DELAYS

  // #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE

  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023

  // #define USE_LIGHTING_TEMPLATE
  // // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  // #define STRIP_SIZE_MAX 14
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"GRBW",
  //       "BusType":"SK6812_RGBW",
  //       "Start":0,
  //       "Length":14
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       14
  //     ],
  //     "ColourPalette":96,
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 2000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 5,
  //   "BrightnessCCT": 0
  // }
  // )=====";
  

  // // /***********************************
  // //  * SECTION: Template Configs
  // // ************************************/    
  // // // I need to add a mqtt send limit to telemetry to prevent race conditions in OH. Add as option in configm with 0 meaning none and then MS values going up
  // // // mqtt.h will need to check if original is from telemetry type, then only permit sending when last was greater than config amount. 
  // // // The effect should be hourly update are stagarded by few seconds

  // // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // // // #define ENABLE_ADVANCED_DEBUGGING
  // // // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // // // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // // // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // // #define   ENABLE_DEBUG_LINE_HERE


  // // #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // // #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  // // // #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // //   #define USE_MODULE_NETWORK_WEBSERVER
  // // //   #define USE_MODULE_NETWORK_WEBSERVER
  // // // #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // // // #define ENABLE_FREERAM_APPENDING_SERIAL

  // // // #define USE_MODULE_SENSORS_INTERFACE
  // // // #define USE_MODULE_SENSORS_BME
  // // // #define USE_MODULE_SENSORS_PIR

  // // // #define USE_MODULE_DISPLAYS_INTERFACE
  // // #define USE_MODULE_DISPLAYS_NEXTION

  
  // // #define USE_MODULE_DISPLAYS_NEXTION
  // //   #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  // // #define NEXTION_DEFAULT_PAGE_NUMBER 2  
  // //   #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  // //   // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
  // //   #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  // //   #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  // // #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  // // #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // // // #define USE_MODULE_NETWORK_WEBSERVER






  // #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  // #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  //   #define USE_MODULE_NETWORK_WEBSERVER
  //   #define USE_MODULE_NETWORK_WEBSERVER
  // #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // #define ENABLE_FREERAM_APPENDING_SERIAL


  // // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_NEXTION

  
  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  // #define NEXTION_DEFAULT_PAGE_NUMBER 2  
  //   #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  //   // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
  //   #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  //   #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  // #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  // #define ENABLE_DEVFEATURE_NEXTION_WEBUI














  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // // #define ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES

  // // #define USE_MODULE_NETWORK_WEBSERVER

  // // #define ENABLE_LOG_LEVEL__DEBUG_TRACE
  // // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // // #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  
  // #define ENABLE_DEVFEATURE_DEVICELIST_DEBUG_WITH_COMMANDS

  // #define DISABLE_SLEEP

  // #define USE_MODULE_CORE_RULES
  //   #define D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND 5
  //   #define ENABLE_DEVFEATURE_RULES_COMMAND_CAN_USE_TRIGGER_VALUE

  // // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  // #define USE_MODULE_SENSORS_SUN_TRACKING

  // // #define USE_MODULE_DRIVERS_LEDS

  // // #define USE_MODULE_DRIVERS_IRREMOTE
  // // #define USE_IR_RECEIVE

  // #define USE_MODULE_SENSORS_INTERFACE
  //   #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  //   #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  //   // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
  //   #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //   #define USE_MODULE_SENSORS_DHT
  //   #define USE_MODULE_SENSORS_BME
  //     #define ENABLE_DEVFEATURE_BME680
  //   #define USE_MODULE_SENSORS_BH1750
  //   #define USE_MODULE_SENSORS_SWITCHES
  //   #define USE_MODULE_SENSORS_BUTTONS
  //   #define USE_MODULE_SENSORS_PIR
  //   #define USE_MODULE_SENSORS_LDR_BASIC

  // #ifdef USE_MODULE_SENSORS_LDR_BASIC
  //   #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
  //   #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  // #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  









  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  // // #define USE_MODULE_DISPLAYS_NEXTION
  // //   // Note from now on, the top few pages will also follow the exaxt same so logging/error pages will always be the same!! 
  // //   // Any special pages will follow
  // //   #define NEXTION_DEFAULT_PAGE_NUMBER 2   // I should add "p[c]" where c means current page, so I need to search and replace "p[c]" as "p[0]"
  // //   #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  // //   #define ENABLE_DEVFEATURE_NEXTION_INCREASE_BAUDRATE

  // // //  Work on ability to increase nextion baud if possible via code

  // // //   #define ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
  // // //   #define ENABLE_DEVFEATURE_CAUTION__BLOCK_ANIMATION_LOOP_FOR_DIRECT_TESTING


  // // /**
  // //  * @brief USe Logic analyser to find out where the delays in lighting code are
  // //  * - around drawing new palette
  // //  * - time to update each blend function
  // //  * - time to output "Show"
  // //  * Add header that breaks out multiple pins into a nice female header for easy logic analyzer connection
  // //  * 
  // //  */

  // // #define USE_MODULE_CONTROLLER_HVAC
  // //   #define HEATING_DEVICE_MAX 2
  // //   #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
  // //   // #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  // // // #define USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS
  // // #define USE_MODULE_DRIVERS_INTERFACE
  // // #define USE_MODULE_DRIVERS_RELAY

  

  // // // Actual
  // // #define GPIO_NAME_ZONE0_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL1_CTR
  // // #define GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL2_CTR
  // // // #define GPIO_NAME_ZONE3_IMMERISON_RELAY   D_GPIO_FUNCTION_REL3_CTR
  // // // #define GPIO_NAME_ZONE4_BOILER_RELAY      D_GPIO_FUNCTION_REL4_INV_CTR












  // // //here
  // // /**
  // //  * Debug pins
  // //  * */
  // // // #define ENABLE_FEATURE_DEBUG_PINS

  // // // #define DEBUG_PIN1_GPIO     32
  // // // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  // // // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  // // // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  // // // #define DEBUG_PIN2_GPIO     33
  // // // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  // // // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  // // // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  // // // #define DEBUG_PIN3_GPIO     25
  // // // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  // // // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  // // // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  // // // #define DEBUG_PIN4_GPIO     14
  // // // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  // // // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  // // // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  // // // #define DEBUG_PIN5_GPIO     12
  // // // #define DEBUG_PIN5_INIT()   pinMode(DEBUG_PIN5_GPIO, OUTPUT); digitalWrite(DEBUG_PIN5_GPIO, HIGH);
  // // // #define DEBUG_PIN5_SET(X)   digitalWrite(DEBUG_PIN5_GPIO, X);
  // // // #define DEBUG_PIN5_TOGGLE()   digitalWrite(DEBUG_PIN5_GPIO, !digitalRead(DEBUG_PIN5_GPIO));

  // // // #define DEBUG_PIN6_GPIO     13
  // // // #define DEBUG_PIN6_INIT()   pinMode(DEBUG_PIN6_GPIO, OUTPUT); digitalWrite(DEBUG_PIN6_GPIO, HIGH);
  // // // #define DEBUG_PIN6_SET(X)   digitalWrite(DEBUG_PIN6_GPIO, X);
  // // // #define DEBUG_PIN6_TOGGLE()   digitalWrite(DEBUG_PIN6_GPIO, !digitalRead(DEBUG_PIN6_GPIO));

  // #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_GPIOC "\":{"   
  //     /**
  //      * @brief Right side
  //      **/
  //     #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023   
  //     "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
  //     "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
  //     #endif // USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //     // GPIO1 - TX0 - Debug Serial TX
  //     // GPIO3 - RX0 - Debug Serial RX
  //     #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
  //     "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_SENSORS_DHT
  //     "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
  //     "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","      
  //     #endif
  //     #ifdef USE_MODULE_SENSORS_PIR
  //     "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
  //     "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  //     "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
  //     #endif 
  //     #ifdef USE_MODULE_DRIVERS_LEDS
  //     "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
  //     "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
  //     #endif
  //     #if defined(USE_MODULE_DRIVERS_RELAY)// && !defined(USE_MODULE_DISPLAYS_NEXTION_V2) // Only debug pins
  //     "\"2\":\""  GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\"," //pin2 - blue led
  //     "\"15\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
  //     #endif
  //     // GPIO0 - ADC2 CH1
  //     /**
  //      * @brief Left side
  //      **/
  //     // EN
  //     // 36 - INPUT ONLY - VP
  //     // 39 - INPUT ONLY - VN
  //     #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  //     "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
  //     #endif
  //     // 35 - INPUT ONLY - adc1_7
  //     // 32 - Touch9 (Debug Header 1)
  //     // 33 - Touch8 (Debug Header 2)
  //     // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
  //     #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
  //     "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
  //     #endif
  //     "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
  //     // 14 (Debug Header 4)
  //     // 12 (Debug Header 5)
  //     // 13 (Debug Header 6)
  //     // Can I introduce a way that a comma at the end, does not make a broken json?
  //     "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
  //   "},"
  //   "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  //   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";

  // /***********************************
  //  * SECTION: Device Configs
  // ************************************/    

  // /**
  //  * @brief HVAC zones
  //  **/
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"


  // #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  // #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "BedroomDesk"
  // #define D_DEVICE_SENSOR_CLIMATE_BME1 "BedroomDesk-BME1"
  // #define D_DEVICE_SENSOR_CLIMATE_BME2 "BedroomDesk-BME2"
  // #define D_DEVICE_SENSOR_CLIMATE_DHT1 "BedroomDesk-DHT1"
  // #define D_DEVICE_SENSOR_CLIMATE_DHT2 "BedroomDesk-DHT2" 
  

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "DB_01"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "DB_02"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,205,206,170,25]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "DB_03"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,134,175,63]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "DB_04"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "DB_05"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,237,161,47,0,0,0,105]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "DB_06"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,135,47,0,0,0,56]"


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME1 "\","
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME2 "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_DHT_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_DHT1 "\","
  //       "\"" D_DEVICE_SENSOR_CLIMATE_DHT2 "\""
  //     "]," 
  //     "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
  //       "\"" "Bedroom" "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
  //     "]," 
  //     "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
  //       "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
  //       "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
  //       "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
  //       "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
  //     "]"
  //   "},"
  //   "\"" D_SENSORADDRESS "\":{"
  //     "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
  //       "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\":" D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\":" D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\":" D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\":" D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\":" D_DEVICE_SENSOR_DB18S20_5_ADDRESS ""
  //     "}"  
  //   "}," 
  //   "\"" "HVACZone" "\":{"
  //     "\"" "SetSensor" "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME1 "\","
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME2 "\"," 
  //       "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\""
  //     "],"
  //     "\"" "SetOutput" "\":["
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "},"
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "},"
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "},"
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_3_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "}"
  //     "]"
  //   "}," 
  //   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
  //   "\"MQTTSubscribe\":["
  //       "\"desksensor_slave/status/drivers_interface/+/drivers/unified\""
  //   "]"
  // "}";

  
  // // #define USE_RULES_TEMPLATE
  // // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // // R"=====(
  // // {
  // //   "AddRule": {
  // //     "Default":["Switch1Change->Relay1Follow"]
  // //   }
  // // }
  // // )=====";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)  // needs appending method 
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // // #define USE_LIGHTING_TEMPLATE
  // // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // // R"=====(
  // // {
  // //   "HardwareType":"SK6812",
  // //   "ColourOrder":"grbw",
  // //   "AnimationMode":"Effects",
  // //   "ColourPalette":"Christmas 01",
  // //   "Effects": {
  // //     "Function":1,
  // //     "Intensity":50,
  // //     "Grouping":2
  // //   },
  // //   "Transition": {
  // //     "TimeMs": 0,
  // //     "RateMs": 1000
  // //   },
  // //   "BrightnessRGB": 1
  // // }
  // // )=====";
  // // #endif // USE_MODULE_LIGHTS_INTERFACE



  // /**
  //  * @brief New group of commands that will run after 1 minute, ideal for testing code 
  //  * 
  //  */
  // #define USE_DELAYED_COMMANDS_TEMPLATE
  // DEFINE_PGM_CTR(FIRMWARE_CUSTOM_DELAYED_START_TEMPLATE)
  // "{"
  //   "\"MQTTUpdateSeconds\":{\"IfChanged\":1},"
  //   "\"MQTTSubscribe\":["
  //       "\"desksensor_slave/status/drivers_interface/+/drivers/unified\""
  //   "]"
  // "}";


  // DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  // R"=====(
  // {
  //   "ObjectNameID": {
  //     "hIconUS": 2,
  //     "hTimeUS": 6,
  //     "hBoostUS": 11,
  //     "hAutoUS": 16,
  //     "hIconDS": 3,
  //     "hTimeDS": 7,
  //     "hBoostDS": 12,
  //     "hAutoDS": 17,
  //     "hIconIH": 4,
  //     "hTimeIH": 8,
  //     "hBoostIH": 13,
  //     "hAutoIH": 18,
  //     "hIconWB": 5,
  //     "hTimeWB": 9,
  //     "hBoostWB": 14,
  //     "hAutoWB": 19,
  //     "hIconDryer": 124,
  //     "hTimeDryer": 125,
  //     "hBoostDryer": 126,
  //     "hAutoDryer": 127
  //   }
  // }
  // )=====";

#endif



/**
 * @brief 2024 Device
 * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
 * 800L as smaller and lighter than 7000G
 */
#ifdef DEVICE_TESTBED__GPS_SERIAL
  #define DEVICENAME_CTR          "testbed_gps_serial"
  #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  
  /***********************************
   * SECTION: System Debug Options
  ************************************/  

 
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

//   #define ENABLE_FREERAM_APPENDING_SERIAL



  /***********************************
   * SECTION: System Configs
  ************************************/    
  
  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  #define ENABLE_GROUPFEATURE__GPS__WITH_SERIAL_POLLING
  #define ENABLE_GROUPFEATURE__GPS__WITH_SERIAL_INTERRUPTS_BUFFER
  #define ENABLE_GROUPFEATURE__COMPASS__QMC5883


  /***********************************
   * SECTION: Network Configs
  ************************************/    

//  #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS



//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/    


//   // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//   #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//     #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
//   // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
//   //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

//   #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
//     // #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
//     #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
//     // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
//     // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
//     #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//     #define ENABLE_DEBUG_FUNCTION_NAMES
//     #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
//     // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
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

//   // #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
//   //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600
//   // #endif 
//   // #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
//   //   #define USE_MODULE__DRIVERS_MAVLINK_DECODER
//   //     #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
//   //     #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
//   //     #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
//   //   #define USE_MODULE_CORE_SERIAL_UART
//   //     #define ENABLE_HARDWARE_UART_2
//   //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   // #endif
//   // #ifdef USE_GROUPFEATURE__MODEM_GPS
//   //   #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//   //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//   //   #define USE_MODULE_SENSORS_GPS_MODEM
//   // #endif 
//   // #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
//   //   #define DISABLE_NETWORK_WIFI
//   //   #define USE_MODULE_NETWORK_CELLULAR
//   //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//   //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//   //   #define USE_MODULE_SENSORS_INTERFACE
//   //   #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//   //   #define USE_MODULE_NETWORK_MQTT
//   //   // #define MQTT_SOCKET_TIMEOUT 1
//   //   // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
//   //    #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
//   //   // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//   //   // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
//   //   #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
//   //   // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
//   // #endif
//   // #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
//   //   #define USE_MODULE_NETWORK_WIFI
//   //   #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//   //   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
//   //   #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
//   //   #define MQTT_PORT     1883
//   // #endif
//   // #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//   //   #define DISABLE_NETWORK_WIFI
//   //   #define USE_MODULE_NETWORK_CELLULAR
//   //   #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//   //   #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//   //   // #define USE_MODULE_SENSORS_INTERFACE
//   //   // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//   //   // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
//   //   // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
//   // #endif

//   /**
//    *  GPS
//    * */
//   #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
//     #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
//     #define USE_MODULE_SENSORS_GPS_SERIAL
//     #define ENABLE_GPS_PARSER_NMEA
//     #define ENABLE_GPS_PARSER_UBX
//     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
//     #define NMEAGPS_DERIVED_TYPES
//     // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     // #define gpsPort Serial1
//     // #define D_GPS_BAUD_RATE_FAST    921600
//     // #define D_GPS_BAUD_RATE_DEFAULT 9600

//     // #define USE_DEVFEATURE_GPS_POLLING_INPUT

//     // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    
// #define ENABLE_DEVFEATURE__START_STATIC_WHILE
// #define ENABLE_DEVFEATURE__START_STATIC_INIT_PORT
// #define ENABLE_DEVFEATURE__START_STATIC_LOOP

// #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     #define gpsPort Serial2
//     // #define D_GPS_BAUD_RATE_FAST    921600
//     // #define D_GPS_BAUD_RATE_DEFAULT 9600
//     // #define D_GPS_BAUD_RATE_DEFAULT 115200
//     #define D_GPS_BAUD_RATE_DEFAULT 230400
//     // #define D_GPS_BAUD_RATE_DEFAULT 460800

//     // #define D_GPS_BAUD_RATE_DEFAULT 115200
//     // #define D_GPS_TX_PIN_DEFAULT 19
//     // #define D_GPS_RX_PIN_DEFAULT 18

//     #define USE_DEVFEATURE_GPS_POLLING_INPUT

//     // #define USE_MODULE_CORE__SERIAL

//     // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

//     #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

//     #define USE_DEVFEATURE_UBLOX_GLOBAL
    
//     // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

//     #define USE_DEVFEATURE__UBLOX_TEST_CLASS

//     #define ENABLE_DEBUGFEATURE__GPS_COMMANDS_FOR_TESTING





//     // #define USE_MODULE_DRIVERS_INTERFACE
//     // #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_1
//     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

//   /***********************************
//    * SECTION: Template Configs
//   ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_CORE__SERIAL
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
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


//       /** 5P small - UART1 GPS Stream
//        * Orange      19, UART1_TX
//        * Yellow      18, UART1_RX
//        * White        
//        * Red         VCC, 3V3
//        * Black       GND
//        * */
//       // "\"32\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
//       // "\"33\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","



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


#define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "gps"
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" 

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
    // "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";


  /***********************************
   * SECTION: Core Configs
  ************************************/     
  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // /***********************************
  //  * SECTION: Network Configs
  // ************************************/    

  // // #define USE_MODULE_NETWORK_WEBSERVER

  // // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE


  // // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_DEVFEATURE__START_STATIC_WHILE
  // #define ENABLE_DEVFEATURE__START_STATIC_INIT_PORT
  // #define ENABLE_DEVFEATURE__START_STATIC_LOOP

  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  // /**
  //  *  GPS
  //  * */
  // // #ifdef ENABLE_GROUPFEATURE__GPS_SERIAL
  //   #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
  //   #define USE_MODULE_SENSORS_GPS_SERIAL
  //   #define ENABLE_GPS_PARSER_NMEA
  //   #define ENABLE_GPS_PARSER_UBX
  //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  //   #define NMEAGPS_DERIVED_TYPES
  //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   // #define gpsPort Serial1
  //   // #define D_GPS_BAUD_RATE_FAST    921600
  //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

  //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

    
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   #define gpsPort Serial2
  //   #define D_GPS_BAUD_RATE_DEFAULT 9600
  //   // #define D_GPS_BAUD_RATE_DEFAULT 115200
  //   // #define D_GPS_BAUD_RATE_DEFAULT 230400
  //   // #define D_GPS_BAUD_RATE_DEFAULT 460800

  //   // #define D_GPS_BAUD_RATE_DEFAULT 115200
  //   // #define D_GPS_TX_PIN_DEFAULT 19
  //   // #define D_GPS_RX_PIN_DEFAULT 18

  //   #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   #define USE_MODULE_CORE__SERIAL

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

  //   #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

  //   #define USE_DEVFEATURE_UBLOX_GLOBAL
    
  //   // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

  //   #define USE_DEVFEATURE__UBLOX_TEST_CLASS

  //   #define ENABLE_DEBUGFEATURE__GPS_COMMANDS_FOR_TESTING





  //   // #define USE_MODULE_DRIVERS_INTERFACE
  //   // #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_1
  //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // // #endif // ENABLE_GROUPFEATURE__GPS_SERIAL

  // /**
  //  *  GPS
  //  * */
  // #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  // #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  //   #define USE_MODULE_SENSORS_GPS_SERIAL //remove?
  //   #define USE_MODULE_SENSORS_GPS_SERIAL
  //   #define ENABLE_GPS_PARSER_NMEA
  //   #define ENABLE_GPS_PARSER_UBX
  //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  //   #define NMEAGPS_DERIVED_TYPES
  //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   #define gpsPort Serial2
  //   #define D_GPS_BAUD_RATE_FAST    921600
  //   #define D_GPS_BAUD_RATE_DEFAULT 9600

  //   #define USE_DEVFEATURE_GPS_POLLING_INPUT

  //   // #define USE_MODULE_CORE__SERIAL

  //   // #define ENABLE_DEVFEATURE_GPS_SERIAL__NEW_CODE

  //   #define ENABLE_DEVFEATURE_USE_HARDWARE_SERIAL2_FOR_GPS

  //   #define USE_DEVFEATURE_UBLOX_GLOBAL
    
  //   // #define ENABLE_DEVFEATURE__ENABLE_UBX_PARSER_IN_CLASS

  //   #define USE_DEVFEATURE__UBLOX_TEST_CLASS

  //   // #define ENABLE_DEVFEATURE_NEOGPS__CLASS_AS_INSTANCE


  //   // #define USE_MODULE_DRIVERS_INTERFACE
  //   // #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_2
  //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600  //D_GPS_BAUD_RATE_FAST
  // #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS




#endif // DEVICE_TESTBED__GPS_SERIAL

              



// /**
//  * 
//  * 
//  * Ethernet R
//  * 
//  * w/o  gnd
//  * o/w     5v
//  * w/g    left LED (upstairs)              D27
//  * bl/w   centre Relay (downstairs)        D5
//  * w/bl   centre LED (downstairs)          D33
//  * g/w    left Relay (upstairs)            D18
//  * w/br   right LED (boiler)               D26
//  * br/w   right Relay (boiler)             D19
//  * 
//  * Ethernet N
//  * 
//  * w/o  gnd
//  * o/w     5v
//  * w/g    SK6812 D4
//  * bl/w   NC
//  * w/bl   NC
//  * g/w    NC
//  * w/br   RX2 of esp32 from nextion
//  * br/w   TX2 of esp32 from nextion    
//  * 

// */
// #ifdef DEVICE_TESTBED__LED_MATRIX
//   #define DEVICENAME_CTR          "testbed_ledmatrix"
//   #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
//   #define DEVICENAME_ROOMHINT_CTR "Bedroom"
//   // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
//     #define MQTT_PORT     1883
    
//   #define SETTINGS_HOLDER 1239


//   /***********************************
//    * SECTION: System Debug Options
//   ************************************/    
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUG_LINE_HERE

//   // #define ENABLE_FREERAM_APPENDING_SERIAL

//   // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

//   #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

//   /***********************************
//    * SECTION: System Configs
//   ************************************/     

  

  

//   #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

//   // #define USE_MODULE_CORE_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

//   // Settings saving and loading
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

//   #define USE_MODULE_SENSORS_SUN_TRACKING


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
//   #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
//     #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MATRIX
//     #define ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP  
//     #define WLED_DEBUG
//     #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
//     // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
//   #endif 

//   #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM


//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.



//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
//   #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI


  

//   // 13, 18, 19, 22, 23, 25, 26, 27       USED
//   // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
//   // 21 = on PCB (4 pixels)
//   // 32 = external
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":32,
//         "ColourOrder":"GRB",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":256
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         256
//       ],
//       "ColourPalette":"Party 16",
//       "PaletteMappingValues":[10,15,20],
//       "SegColour0": {
//         "Hue": 0,
//         "Sat":100,
//         "BrightnessRGB":1
//       },
//       "SegColour1": {
//         "Hue": 120,
//         "Sat":20,
//         "BrightnessRGB":1
//       },
//       "SegColour2": {
//         "Hue": 240,
//         "Sat":100,
//         "BrightnessRGB":1
//       },
//       "Effects": {
//         "Function":"Gradient",
//         "Speed":127,
//         "Intensity":127,
//         "Decimate":0,
//         "Grouping":1
//       },
//       "Transition": {
//         "TimeMs": 0,
//         "RateMs": 25
//       },
//       "BrightnessRGB": 100,
//       "BrightnessCCT": 0
//     },
//     "BrightnessRGB": 20,
//     "BrightnessCCT": 0
//   }
//   )=====";

//   /***********************************
//    * SECTION: Energy Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Controller Configs
//   ************************************/  

//   /***********************************
//    * SECTION: GPIO Template
//   ************************************/  

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIOC "\":{"
//       #ifdef USE_MODULE_DRIVERS_RELAY
//       "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
//       "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
//       "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
//       "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
//       #endif
//       "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
//       "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
//       #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
//       "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
//       #endif    
//       #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
//       "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
//       #endif
//       #ifdef USE_MODULE_DISPLAYS_NEXTION
//       "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
//       "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
//       #endif
//       "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
//       // 32 - LED Strip External
//       // 21 - LED Strip Onboard
//       // 25?
//       // 
//     "},"
//     "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";


//   /**
//    * @brief Drivers and Sensors for HVAC zones
//    **/
//   #define D_DEVICE_HEATER_0_NAME "Dryer"
//   #define D_DEVICE_HEATER_1_NAME "FloorMat"
//   #define D_DEVICE_HEATER_2_NAME "FanHeater"
//   #define D_DEVICE_HEATER_3_NAME "OilRadiator"

//   #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// // {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

//   /** 
//    * MainBoard
//    * */
//   #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
//   #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

//   #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
//   #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

//   #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
//   #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

//   #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
//   #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

//   #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
//   #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

//   #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

//   #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
//   #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
//   #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
//   #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
//   #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
//   #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
//   #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
//   #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
//   #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
//   #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
//   #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
//   #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_ENERGY "\":{"
//         "\"DeviceCount\":4"    
//     "},"
//     "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
//         "\"DeviceCount\":4"    
//     "},"
//     "\"" D_DEVICENAME "\":{"
//       "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
//         // Downstairs
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DHT_CTR "\":["
//         "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
//         "\"" "Desk" "\""
//       "],"  
//       "\"" D_MODULE_SENSORS_BME_CTR "\":["
//         "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
//         "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_INA219_CTR "\":["
//         "\"" D_DEVICE_SENSOR_CURRENT "\""
//       "],"
//       "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
//         "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
//       "],"
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
//         "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "]"
//     "},"
//     "\"" D_SENSORADDRESS "\":{"
//       "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
//         // Downstairs
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
//       "},"  
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
//         D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
//       "]"  
//     "},"
//     "\"" "HVACZone" "\":{"
//       "\"" "SetSensor" "\":["
//         "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
//       "],"
//       "\"" "SetOutput" "\":["
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "}"
//       "]"
//     "},"
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
//   "}";



  
// #endif






#ifdef DEVICE_CAMERA_XIAO_TESTBED
  #define DEVICENAME_CTR          "treadmill_power_monitor"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


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

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

  

  

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SUN_TRACKING

  #define USE_MODULE_CORE_FILESYSTEM

  
      #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   #define USE_MODULE_SENSORS_INTERFACE  
//   #define USE_MODULE_SENSORS_SWITCHES
//     #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   #define USE_MODULE_DISPLAYS_INTERFACE
//   #define USE_MODULE_DISPLAYS_OLED_SH1106
//     #define SHOW_SPLASH

//   // Add this, 4 rows so show the power, current, voltage and energy

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Energy Configs
//   ************************************/  

//   #define USE_MODULE_ENERGY_INTERFACE
//   #define USE_MODULE_ENERGY_PZEM004T_V3
//     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
//   #define MAX_ENERGY_SENSORS 1
//   #define MAX_PZEM004T_DEVICES 1

//   /***********************************
//    * SECTION: Controller Configs
//   ************************************/  

// //  #define USE_MODULE_CONTROLLER__ENERGY_OLED
//  #define USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"23\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


//   /**
//    * @brief Drivers and Sensors for HVAC zones
//    **/
//   #define D_DEVICE_HEATER_0_NAME "Dryer"
//   #define D_DEVICE_HEATER_1_NAME "FloorMat"
//   #define D_DEVICE_HEATER_2_NAME "FanHeater"
//   #define D_DEVICE_HEATER_3_NAME "OilRadiator"

//   #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// // {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

//   /** 
//    * MainBoard
//    * */
//   #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
//   #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

//   #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
//   #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

//   #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
//   #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

//   #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
//   #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

//   #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
//   #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

//   #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

//   #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
//   #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"

//   #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Treadmill"
  
  
//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
//     "\"" D_ENERGY "\":{"
//         "\"DeviceCount\":1"    
//     "},"
//     "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
//         "\"DeviceCount\":1"    
//     "},"
//     "\"" D_DEVICENAME "\":{"
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
//         "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
//         "\"" "TrackingActive" "\""
//       "]"
//     "},"
//     "\"" D_SENSORADDRESS "\":{"
//       "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
//         D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
//       "]"  
//     "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";



  
#endif






// /**
//  * @brief 
//  * Make several of these, one in the garage to show progress
//  * One in my bedroom to motivate me
//  * 
//  */
// #ifdef DEVICE_LIGHTING__LED_MATRIX_BOX_01
//   #ifndef DEVICENAME_CTR
//     #define DEVICENAME_CTR          "led_matrix_box_01"
//   #endif
//   #ifndef DEVICENAME_FRIENDLY_CTR
//     #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
//   #endif
//   #ifndef DEVICENAME_DESCRIPTION_CTR
//     #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
//   #endif
//   #define DEVICENAME_ROOMHINT_CTR "Roaming"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//     #define MQTT_PORT     1883
    
//   #define SETTINGS_HOLDER 1239

//   /***********************************
//    * SECTION: System Debug Options
//   ************************************/    
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   #define ENABLE_FREERAM_APPENDING_SERIAL

//   /***********************************
//    * SECTION: System Configs
//   ************************************/     

//   // #define USE_MODULE_CORE_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

//   // Settings saving and loading
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/    
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_MATRIX

//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE

//   #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY
  
//   // 13, 18, 19, 22, 23, 25, 26, 27       USED
//   // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":19,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":256
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         256
//       ],
//       "ColourPalette":"Christmas Snowy 02",
//       "SegColour0": {
//         "Hue": 0,
//         "Sat":100,
//         "BrightnessRGB":5
//       },
//       "Effects": {
//         "Function":"Shimmer",
//         "Speed":127,
//         "Intensity":101,
//         "Decimate":0,
//         "Grouping":1
//       },
//       "Transition": {
//         "TimeMs": 0,
//         "RateMs": 1000
//       },
//       "BrightnessRGB": 100,
//       "BrightnessCCT": 0
//     },
//     "BrightnessRGB": 100,
//     "BrightnessCCT": 0
//   }
//   )=====";
  
//   /***********************************
//    * SECTION: Template Configs
//   ************************************/    

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME          "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIO_FUNCTION "\":{},"
//     "\"" D_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

//   /***********************************
//    * SECTION: Device Configs
//   ************************************/    

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
//     "\"Logging\":{\"SerialLevel\":\"Info\"}" 
//   "}";

// #endif // DEVICE_CHRISTMAS__OUTSIDE_WREATH




#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__10INCH //10inch display
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "nextion_testbed"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  ///////////////////////////////////////////// Enable Logs
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
  ///////////////////////////////////////////// System Logs
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
  #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT

  ///////////////////////////////////////////// Module Logs
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  
  /***********************************
   * SECTION: System Configs
  ************************************/   
 
  #define SETTINGS_HOLDER 1239

  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device
  #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI  

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK



// #define USE_FEATURE_NEXTION__SERIAL_DEFAULT_BUAD_NEW_PANEL_FIRST_OTA

// #define USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT 115200

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define USE_MODULE_NETWORK_WEBSERVER
  
  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";


#endif



#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
  #define DEVICENAME_CTR            "nextion_desk"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

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

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 2  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


#endif


#ifdef DEVICE_INSTALLED__NEXTION_DISPLAY__KITCHEN_7INCH
  #define DEVICENAME_CTR            "nextion_kitchen"
  #define DEVICENAME_FRIENDLY_CTR   "Installed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Installed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883
  #define SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
#endif
#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
  #define DEVICENAME_CTR            "nextion_kitchen_testbed"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883
  #define SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
#endif
#ifdef SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH

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

  #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID



  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 5  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

    #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK


  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 3,
      "hTimeUS": 7,
      "hBoostUS": 12,
      "hAutoUS": 16,
      "hIconDS": 4,
      "hTimeDS": 8,
      "hBoostDS": 13,
      "hAutoDS": 17,
      "hIconIH": 5,
      "hTimeIH": 9,
      "hBoostIH": 14,
      "hAutoIH": 18,
      "hIconWB": 6,
      "hTimeWB": 10,
      "hBoostWB": 15,
      "hAutoWB": 19,
      "hIconCook": 55,
      "hTimeCook": 56,
      "hBoostCook": 57,
      "hAutoCook": 58,
      "powGardenLgt0": 151,
      "powGardenLgt10": 152,
      "powGardenLgt1": 153,
      "powOutsLght0": 154,
      "powOutsLght10": 155,
      "powOutsLght1": 156,
      "powGazLED": 157,
      "powGazWater": 158,
      "posGazSpeaker": 159
    }
  }
  )=====";

  //start here, probably roll into general command structure. I maybe need to create "COMMAND_SET_01" and 02,03 up to 5 as needed




  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";




#endif


#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_01
  #define DEVICENAME_CTR            "nextion_treadmill_01"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

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

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL
    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 230400

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION


  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

    #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


#endif


#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_02
  #define DEVICENAME_CTR            "nextion_treadmill_02"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

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

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6 
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 230400

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


#endif


// /**
//  * @brief 
//  * Primary testbed for all new lighting code on nodemcu
//  */
// #ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__70__ESP32_PARALLEL_4CH_TRIPLE_CONNECTOR_TESTER
//   #ifndef DEVICENAME_CTR
//   #define DEVICENAME_CTR          "testbed_default"
//   #endif
//   #ifndef DEVICENAME_FRIENDLY_CTR
//   #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
//   #endif
//   #ifndef DEVICENAME_DESCRIPTION_CTR
//   #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
//   #endif
//   #define DEVICENAME_ROOMHINT_CTR "testgroup"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//     #define MQTT_PORT     1883

// // #define ENABLE_DEBUG_LINE_HERE

//   #define ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
//   #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
//   #define ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
//   #define ENABLE_DEBUGFEATURE_SETTINGS_STORAGE__ENABLED_SETTINGS_SAVE_EVERY_MINUTE_FOR_DEBUG
//   #define USE_MODULE_CORE_FILESYSTEM
//   #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT
//   #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
//   #define ENABLE_DEVFEATURE_MQTT__ESTIMATED_INCOMING_COMMANDS_AND_REPORT_ISSERVICED

//   /***********************************
//    * SECTION: System Debug Options
//   ************************************/    
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUG_LINE_HERE
//   // #define ENABLE_DEBUG_LINE_HERE2

//   // #define ENABLE_FREERAM_APPENDING_SERIAL

//   // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

//   // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

//   // #define ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY

//   /***********************************
//    * SECTION: Storage Configs
//   ************************************/  
 
//   #define ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

//   /**
//    * For debugging and short term I may want to store everything as JSON, so I can view the data?
//    * Longer term, a mixture of JSON/Binary for space.
//    * Options should just be ifdef to switch between methods. 
//   */
//   // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

//   // New way to start the save into memory periodically, and then recover if available on boot. Init phase of full system.
//   #define ENABLE_FILESYSTEM__MODULES_CORE__SAVE
//   // #define ENABLE_FILESYSTEM__MODULES_CORE__RESTORE_ON_BOOT
//   // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__SAVE
//   // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__RESTORE_ON_BOOT
//   // #define ENABLE_FILESYSTEM__MODULES_SENSORS__SAVE
//   // #define ENABLE_FILESYSTEM__MODULES_SENSORS__RESTORE_ON_BOOT
//   // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__SAVE
//   // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__RESTORE_ON_BOOT
  

//   /***********************************
//    * SECTION: System Configs
//   ************************************/     

//   #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

//   // #define USE_MODULE_CORE_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

//   // Settings saving and loading
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  
 
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

//     // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
//     // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
//     #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
//     // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES

//     // #undef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//     // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS

//     #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS


//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIO_FUNCTION "\":{},"
//     "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

//   #define USE_LIGHTING_TEMPLATE

//   #define ENABLE_DEVFEATURE__LIGHTING_TEMPLATE__TESTCASE_SINGLE_OUTPUT

//   #ifdef ENABLE_DEVFEATURE__LIGHTING_TEMPLATE__TESTCASE_SINGLE_OUTPUT
  
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":2,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":1000
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         1000
//       ],
//       "ColourPalette":"RGPBO",
//       "Effects": {
//         "Function":"Static",
//         "Speed":127,
//         "Intensity":127,
//         "Grouping":10,
//         "RateMs": 1000
//       },
//       "BrightnessRGB": 100
//     },
//     "BrightnessRGB": 100,
//     "BrightnessCCT": 0
//   }
//   )=====";

//   #else

//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":2,
//         "ColourOrder":"BGR",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":10
//       },
//       {
//         "Pin":4,
//         "ColourOrder":"BGR",
//         "BusType":"WS2812_RGB",
//         "Start":10,
//         "Length":10
//       },
//       {
//         "Pin":18,
//         "ColourOrder":"BGR",
//         "BusType":"WS2812_RGB",
//         "Start":20,
//         "Length":10
//       },
//       {
//         "Pin":19,
//         "ColourOrder":"BGR",
//         "BusType":"WS2812_RGB",
//         "Start":30,
//         "Length":10
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         40
//       ],
//       "ColourPalette":"Snowy 02",
//       "Effects": {
//         "Function":"Slow Glow",
//         "Speed":127,
//         "Intensity":127,
//         "Grouping":1
//       },
//       "Transition": {
//         "TimeMs": 500,
//         "RateMs": 1000
//       },
//       "BrightnessRGB": 100
//     },
//     "BrightnessRGB": 100,
//     "BrightnessCCT": 0
//   }
//   )=====";
  
//   #endif // 
  
// #endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_I2S_PARALLEL_4CH






#ifdef DEVICE_LIGHTING__LIGHTING_EFFECTS__MATRIX_SEGMENT_TESTER
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


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

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

//   

  

//   #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

//   // #define USE_MODULE_CORE_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

//   // Settings saving and loading
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

//   #define USE_MODULE_SENSORS_SUN_TRACKING


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  // #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

  // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
  //   #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MATRIX
  //   #define ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP  
  //   #define WLED_DEBUG
  //   #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
  //   #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
  //   #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 8 // Height
  //   #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 32 // Width
  //   #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
  //   #define ENABLE_DEVFEATURE_LIGHT__MATRIX_SEGMENT_TESTER// give up, just override
  //   // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
  // #endif 

//   #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM


//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.



//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
//   #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI


  

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // 21 = on PCB (4 pixels)
  // 32 = external

/**
 * @brief 
 * 
 * segments needs to be updated to handle matrix allocation
 * 
 * lets keep 8x32 as 2d matrix, and use it to show time of day, where the background is "elevation turned into skycolour ie cyan, orange, sunset, dark blue, black" as new effect
 * 
 */

// #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
// #define USE_DEBUG_PRINT
// #define USE_DEVFEATURE_LIGHTS__CUSTOM_MAPPING_TABLE_SPLASH

#define MAX_NUM_SEGMENTS 16

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":32,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":256
      }
    ],
    "Segment0": {
      "PixelRange": [0,16],
      "ColourPalette":"Rainbow 16",
      "Effects": {
        "Function":"Static",
        "RateMs": 1000
      }
    },
    "Segment1": {
      "PixelRange": [16,32],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Reverse":1,
        "RateMs": 1000
      }
    },
    "Segment2": {
      "PixelRange": [32,48],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Sweep Random",
        "Speed":229,
        "RateMs": 25
      }
    },
    "Segment3": {
      "PixelRange": [48,64],
      "ColourPalette":"RGPBO",
      "Effects": {
        "Function":"Fireworks 1D",
        "Speed":229,
        "Intensity":127,
        "Reverse":1,
        "RateMs": 25
      }
    },
    "Segment4": {
      "PixelRange": [64,80],
      "ColourPalette":"RGPBO",
      "SegColour1": {
          "Hue": 0,
          "Sat":0,
          "BrightnessRGB":5
        },
      "Effects": {
        "Function":"Theater",
        "Speed":229,
        "Intensity":127,
        "RateMs": 25
      }
    },
    "Segment5": {
      "PixelRange": [80,96],
      "ColourPalette":"RGPBO",
      "Effects": {
        "Function":"Dissolve Random",
        "Speed":229,
        "Intensity":127,
        "Reverse":1,
        "RateMs": 25
      }
    },
    "Segment6": {
      "PixelRange": [96,112],
      "ColourPalette":"RGPBO",
      "Effects": {
        "Function":"Static",
        "Speed":229,
        "Intensity":127,
        "RateMs": 100
      }
    },
    "Segment7": {
      "PixelRange": [112,128],
      "ColourPalette":"Blue and White",
      "Effects": {
        "Function":"Static",
        "Speed":229,
        "Intensity":127,
        "Reverse":1,
        "RateMs": 100
      }
    },
    "BrightnessRGB": 10,
    "BrightnessCCT": 0
  }
  )=====";

  #define USE_LIGHTING_TEMPLATE_ANOTHER
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE_ANOTHER) 
  R"=====(
  {
    "Segment8": {
      "PixelRange": [128,144],
      "ColourPalette":"Hot 16",
      "Effects": {
        "Function":"Sine",
        "Speed":229,
        "Intensity":127,
        "RateMs": 100
      },
      "SegColour0": {
        "Hue": 0,
        "Sat":0,
        "BrightnessRGB":0
      }
    },
    "Segment9": {
      "PixelRange": [144,160],
      "ColourPalette":"Rainbow 16",
      "Effects": {
        "Function":"Meteor Smooth",
        "Speed":229,
        "Intensity":127,
        "Reverse":1,
        "RateMs": 100
      }
    },
    "Segment10": {
      "PixelRange": [160,176],
      "ColourPalette":"Turbo 16",
      "Effects": {
        "Function":"Bouncing Balls",
        "Speed":127,
        "Intensity":127,
        "RateMs": 100
      }
    },
    "Segment11": {
      "PixelRange": [176,192],
      "ColourPalette":"Sunset",
      "Effects": {
        "Function":"Hour Progress",
        "Speed":229,
        "Intensity":127,
        "Reverse":1,
        "RateMs": 1000
      }
    },
    "Segment12": {
      "PixelRange": [192,208],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Twinkle Palette Two on One",
        "Speed":255,
        "Intensity":30,
        "RateMs": 25,
        "Param0":0
      },
      "SegColour0": {
        "Hue": 0,
        "Sat":0,
        "BrightnessRGB":3
      }
    },
    "Segment13": {
      "PixelRange": [208,224],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Stepping Palette",
        "Speed":229,
        "Intensity":127,
        "Reverse":1,
        "RateMs": 1000
      }
    },
    "Segment14": {
      "PixelRange": [224,240],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Wipe Random",
        "Speed":229,
        "Intensity":127,
        "RateMs": 100
      }
    },
    "Segment15": {
      "PixelRange": [240,256],
      "ColourPalette":"Random 01",
      "Effects": {
        "Function":"Gradient",
        "Speed":229,
        "Intensity":20,
        "Reverse":1,
        "RateMs": 1000
      }
    },
    "BrightnessRGB": 7,
    "BrightnessCCT": 0
  }
  )=====";


  
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":32,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":256
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       256
  //     ],
  //     "ColourPalette":"RGPBY",
  //     "Effects": {
  //       "Function":"Static",
  //       "Speed":1,
  //       "Intensity":127,
  //       "Decimate":0,
  //       "Grouping":1,
  //       "Spacing":14,
  //       "Offset":0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 10
  //   },
  //   "Segment1": {
  //     "PixelRange": [
  //       0,
  //       256
  //     ],
  //     "ColourPalette":"RGPBY",
  //     "Effects": {
  //       "Function":"Static",
  //       "Speed":1,
  //       "Intensity":127,
  //       "Decimate":0,
  //       "Grouping":1,
  //       "Spacing":16,
  //       "Offset":1,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100
  //   },
  //   "BrightnessRGB": 10
  // }
  // )=====";

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif




/**
 * @brief 
 * To test the ensuite code
 */
#ifdef DEVICE_ACTIVE_DEVELOPMENT__DOOR_LIGHTING__OFFICE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

    

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

 #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
 
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":143
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        143
      ],
      "ColourPalette":"Snowy 01",
      "Effects": {
        "Function":"Gradient",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__01__ESP32_1CH



/**
 * @brief 
 * To test the ensuite code
 */
#ifdef DEVICE_ACTIVE_DEVELOPMENT__HALLWAY_FLOWERS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

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

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

    

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_CORE_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

 #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
 
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":"Colourful Greenless",
      "Effects": {
        "Function":"Static Palette Aged",
        "Speed":127,
        "Intensity":47,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__01__ESP32_1CH



/**
 * @brief Device will be made that contains different GPIO testing for the esp32
 * * leave 22,21 for possible OLED later
 * * 4 Buttons  (Pull down when active) [12, 14, 18, 19]
 * * 2 Touch Buttons [32,33]                                 (Solder wires to a pad, then tape or something over it so I can test touch through it (and other materials)) 
 * * 2 Switches (Pull down when active) [25, 26, 27, 13]
 * * 4 LEDs
 * * * 2 as Relays
 * * * 2 as LED module for status 
 * 
 */
#ifdef DEVICE_ACTIVE_DEVELOPMENT__SWITCHES_AND_BUTTONS
  #define DEVICENAME_CTR          "dev_switches_and_buttons"
  #define DEVICENAME_FRIENDLY_CTR "dev_switches_and_buttons #1"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  /***********************************
   * SECTION: System Debug Options
  ************************************/  
#define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE
 
  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT
  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME

  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

 
  /***********************************
   * SECTION: Storage Configs
  ************************************/  


  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  //   #define USE_MODULE_SENSORS_INTERFACE
  //     #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  //   #define USE_MODULE_SENSORS_BME
  // #endif

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    #define SOC_TOUCH_VERSION_1

  #define USE_MODULE_SENSORS_SWITCHES
    #define ENABLE_DEVFEATURE_SWITCHES__V2

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
 
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
 
  #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_RELAY
  #define USE_MODULE_DRIVERS_LEDS
 
  /***********************************
   * SECTION: Lighting Configs
  ************************************/  
        
  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
  //   #define USE_MODULE_ENERGY_INTERFACE
  // #endif
  
  // #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  //   #define USE_MODULE_ENERGY_PZEM004T_V3
  //     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  //   #define MAX_ENERGY_SENSORS 1
  //   #define MAX_PZEM004T_DEVICES 17
  //   #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  // #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

//  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"27\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
      "\"14\":\"" D_GPIO_FUNCTION_LED2_CTR  "\","
      #ifndef USE_MODULE_DRIVERS_RELAY    // if no relays, we want to use all LEDs directly
      "\"12\":\"" D_GPIO_FUNCTION_LED3_CTR  "\","
      "\"13\":\"" D_GPIO_FUNCTION_LED4_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED5_CTR  "\","
      #else
      "\"2\":\""  D_GPIO_FUNCTION_LED3_CTR  "\"," //builtin BLUE
      #endif
      #endif  
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"5\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"4\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"26\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
      "\"15\":\"" D_GPIO_FUNCTION_KEY4_INV_CTR  "\","
      #ifdef SOC_TOUCH_VERSION_1
      "\"32\":\"" D_GPIO_FUNCTION_KEY5_TOUCH_CTR  "\","
      "\"33\":\"" D_GPIO_FUNCTION_KEY6_TOUCH_CTR  "\","
      #endif
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"18\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR  "\","
      "\"19\":\"" D_GPIO_FUNCTION_SWT2_INV_CTR  "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RELAY
      #ifndef USE_MODULE_DRIVERS_LEDS // When LEDs are not used, set 4 relays
      "\"27\":\"" D_GPIO_FUNCTION_REL3_CTR  "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_CTR  "\","
      #endif
      "\"12\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
      "\"13\":\"" D_GPIO_FUNCTION_REL2_CTR  "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  /***********************************
   * SECTION: TEMPLATE: Names
  ************************************/    

  #define D_DEVICE_UNIQUE_NAME "OilRadiator01"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"
  #define D_DEVICE_SENSOR_ZONE_0_NAME "OilRadiator01-BME0"
  #define D_DEVICE_DRIVER_RELAY_0_NAME "OilRadiator01-DriverZone0"
  
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Radiator"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,143,81,7,51,20,1,189]"

  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\""
      "],"      
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_DEVICE_UNIQUE_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
  "}";


#endif



#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
