#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_COLORADO24_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_COLORADO24_H


/*********************************************************************************************\
 * Devices to be used around christmas time
 * - I should keep this file as backup each year and add when needed, perhaps install on USB stick in light boxes with compiled versions for flashing .bin (or just copy of project at that state for recompile) each year if needed


Through whatever means, at boot, the device name (and mac) should be printed asap so reset can tell me what the device is.

Redboard 4X 5V number 1
Redboard 4X 5V number 2

100 Green wire x 4
narrow strip, 160/m, 5v strip


Bring 5V PSU that can work in the states, no USB


esp32 x5 on their own for testing

esp32 dev board buttons and switches

esp32 sdcard board for measurements

sd card reader


USE 2A BLACK WITH BLACK SONOFF FOR WREATH



*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * 4x201 Black 2.5cm Pixels
 * Santa hat, matrix test
 * Status: Only used when LED indexes are manually found, another eps32 with WLED is in use
**/
// #define DEVICE_COLORADO24__MANUAL_FIND_LEDS_ON_HAT

/**
 * String of 100 on the wall
 * Use: Nightlight only
 * Status: On all the time
**/
// #define DEVICE_COLORADO24__STRING_01__SLOW_GLOW_ONLY__100LEDS

/**
 * Tester
 * Use: Running "Sequencer" without LEDs
 * Not using sequencer this year, if playlists are working. 
 * Openhab will just need to be used to set the locked times to switch playlist
**/
// #define DEVICE_COLORADO24__STRING_02__SEQUENCER__NOLEDS

/**
 * String of 250x2 outside, replicating outside_tree but as only 500 LEDs wide
 * Testing "Playlists" and "Presets"
**/
// #define DEVICE_COLORADO24__STRING_03__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__250LEDS

/**
 * To run as outside tree for home
 * No connected LEDs, will run STRING_03 but with 2000 LEDs are expected for tree at home
 * On REDBOARD_01
**/
// #define DEVICE_COLORADO24__STRING_04__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__VIRTUAL2000LEDS

/**
 * String of 100 green, will run all so I can see them working
 * Testing "Playlists" and "Presets"
**/
// #define DEVICE_COLORADO24__STRING_05__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__100LEDS

/**
 * Testing on plane, trying to make it so they can work with their own wifi and no access point.
 * MQTT will not be used.
**/
// #define DEVICE_COLORADO24__STRING_06__AP_MODE_TESTING

/**
 * With White/Green 250 12V
 * Used for?
**/
// #define DEVICE_TESTBED_WEBCAM_ARDUINO

/**
 * My WLED converted 2D matrix tester on the santa hat
**/
// #define DEVICE_COLORADO24__MATRIX_MAPPED_PULSAR

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
// #define DEVICE_ACTIVE_DEVELOPMENT__SWITCHES_AND_BUTTONS_COLORADO24


/** ONLY POWERED/USED DAILY BELOW *************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 **************************************************************************************************************************************************************************************************************************************************************************************************************************** 
 */




#ifdef DEVICE_COLORADO24__STRING_01__SLOW_GLOW_ONLY__100LEDS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__string01__wall100leds"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
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
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024
  #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"BGR",
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
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Slow Glow",
        "Speed":10,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__STRING_01__SLOW_GLOW_ONLY__100LEDS




/**
 * @brief At desk with 100 green leds, but ability to have serial debug
 * Should be primary tester for colorado as I dev code
 * ** presets
 * ** playlists
 * ** sequencer (test as backup to playlists)
 */
#ifdef DEVICE_COLORADO24__STRING_02__SEQUENCER__NOLEDS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__colorado__string_02"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
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
  #define SERIAL_LOG_LEVEL_DURING_BOOT 12
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT

  ///////////////////////////////////////////// Module Logs
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  #define SETTINGS_HOLDER 1239

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER

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
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    // #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    

  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  #define ENABLE_FEATURE_LIGHTING__EFFECTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
  // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS

  // #define ENABLE_FEATURE_LIGHTING__SEQUENCER
  //   #define ENABLE_FEATURE_SEQUENCER__LOAD_DEVICE_LIST
  //   // #define ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE
  //   #define ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE
  //   #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS 
  //   // #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_TWO__ADDED_FLASHING_EFFECTS

// #define ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS 20

  /**
   * @brief Testing the sequencer
   * Get this working first, then work on playlists/presets later
   * Make work on the 
   * 
   */
  #define ENABLE_FEATURE_LIGHTING__SEQUENCER
     #define ENABLE_FEATURE_SEQUENCER__LOAD_DEVICE_LIST
    //  #define ENABLE_DEVFEATURE_SEQUENCER__ENABLE_TIME_RESTRAINTS
     // #define ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE
     #define ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE_2024_100LED
     #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS 
     // #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_TWO__ADDED_FLASHING_EFFECTS

  #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS




  #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM

  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER



  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023


  // #define ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL

  #define DEBUG_ASYNC
  #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH


  #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE

  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE


  

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"BGR",
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
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":127,
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
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\"" // Default of esp32 and lighting should be demo/test sequence when pressed.
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif // DEVICE_COLORADO24__STRING_02__SEQUENCER__NOLEDS


#ifdef DEVICE_COLORADO24__STRING_03__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__250LEDS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__string03__outside_demo"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"  //SLS
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.106" // Billie Left
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
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024
  #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connection 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   */
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
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        500
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__STRING_03__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__250LEDS


#ifdef DEVICE_COLORADO24__STRING_04__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__VIRTUAL2000LEDS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__string04__virtual_outside"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
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
  // #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
  #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT

  ///////////////////////////////////////////// Module Logs
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024
  #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  /**
   * @brief Outside Tree as 300*8 LEDs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":300
      },
      {
        "Pin":16,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":300
      },
      {
        "Pin":17,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":300
      },
      {
        "Pin":18,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":900,
        "Length":300
      },
      {
        "Pin":19,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":300
      },
      {
        "Pin":21,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1500,
        "Length":300
      },
      {
        "Pin":22,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":300
      },
      {
        "Pin":23,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":2100,
        "Length":300
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        2400
      ],
      "ColourPalette":"Christmas Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":1,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 50,
    "BrightnessCCT": 0
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__STRING_04__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__VIRTUAL2000LEDS


#ifdef DEVICE_COLORADO24__STRING_05__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__100LEDS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__string05__outside_demo"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"  //SLS
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.106" // Billie Left
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
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024
  #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connection 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
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
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__STRING_05__PLAYLIST_PRESETS__OUTSIDE_TREE_DEMO__100LEDS



#ifdef DEVICE_COLORADO24__STRING_06__AP_MODE_TESTING
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__string05__outside_demo"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"  //SLS
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.106" // Billie Left
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
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

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

  #define ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025 // Use WLED methods instead of Tas, make my own

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024
  #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connection 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
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
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__STRING_06__AP_MODE_TESTING

/** ONLY RUNNING SOMETIMES BELOW eg fetching LED indexs *************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 **************************************************************************************************************************************************************************************************************************************************************************************************************************** 
 */



#ifdef DEVICE_COLORADO24__MANUAL_FIND_LEDS_ON_HAT
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__colorado__redboard_02"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  #define SERIAL_LOG_LEVEL_DURING_BOOT 12

  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY

  // #define ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
  // #define ENABLE_DEBUG_LINE_HERE3
  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS


  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  #define SETTINGS_HOLDER 1239

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER

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
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
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

  #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM

  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER

  /**
   * @brief Testing the sequencer
   * Get this working first, then work on playlists/presets later
   * Make work on the 
   * 
   */
  // #define ENABLE_FEATURE_LIGHTING__SEQUENCER
  //    #define ENABLE_FEATURE_SEQUENCER__LOAD_DEVICE_LIST
  //   //  #define ENABLE_DEVFEATURE_SEQUENCER__ENABLE_TIME_RESTRAINTS
  //    // #define ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE
  //    #define ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE_2024_100LED
  //    #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS 
  //    // #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_TWO__ADDED_FLASHING_EFFECTS

  #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023


  // #define ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL

  #define DEBUG_ASYNC
  #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH

  
  // #define ENABLE_FEATURE_LIGHTING__SEQUENCER
  //   #define ENABLE_FEATURE_SEQUENCER__LOAD_DEVICE_LIST
  //   // #define ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE
  //   #define ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE
  //   #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS 
  //   // #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_TWO__ADDED_FLASHING_EFFECTS


  #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000

  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
 #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

  #define ENABLE_BUSCONFIG_OPTION_01


  #ifdef ENABLE_BUSCONFIG_16X_3200

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connections
   * 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   * 
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
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":200
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":200
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":200
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":800,
        "Length":200
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      },
      {
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":200
      },
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2200,
        "Length":200
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2400,
        "Length":200
      },
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2600,
        "Length":200
      },
      {
        "Pin":25,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2800,
        "Length":100
      },
      {
        "Pin":33,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB", 
        "Start":3000,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3200
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Sweep Random",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 10,
    "BrightnessCCT": 0
  }
  )=====";
  #endif


  #ifdef ENABLE_BUSCONFIG_OPTION_01


  #define ENABLE_FEATURE_LIGHTING__REALTIME_MODES
  #define ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connections
   * 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   * 
   * 
   * 
   * 
   {
  "BrightnessRGB": 100,
  "SegColour0": {
    "RGBWC": [
      0,
      0,
      0,
      0,
      0
    ]
  },
  "MQTTPixel": {
    "OnPixels": [
      25,65,105,144,180,223,258,291,324,354,383,412,438,463,487,510,532,554,575,594,613,630,646,661,676,691,705,719,732,744,755,765,774,783,791,797
    ]
  }
}{
  "BrightnessRGB": 100,
  "SegColour0": {
    "RGBWC": [
      0,
      0,
      0,
      0,
      0
    ]
  },
  "MQTTPixel": {
    "OnPixels": [
      25,65,105,143,180,223,258,291,324,354,383,412,438,463,487,510,532,554,575,594,613,630,646,661,676,691,705,719,732,744,755,765,774,783,791,797,803,
      26,66,106,144,181,224,256,292,325,355,384,413,439,464,488,511,533,555,576,595,614,631,647,662,677,692,706,720,733,745,756,766,775,784,792,798,804
    ]
  }
}



{
  "BrightnessRGB": 100,
  "SegColour0": {
    "RGBWC": [
      0,
      0,
      0,
      0,
      0
    ]
  },
  "MQTTPixel": {
    "OnPixels": [
      25,65,105,143,180,223,258,291,324,354,383,412,438,463,487,510,532,554,575,594,613,630,646,661,676,691,705,719,732,744,755,765,774,783,791,797,803,
      26,66,106,144,181,224,256,292,325,355,384,413,439,464,488,511,533,555,576,595,614,631,647,662,677,692,706,720,733,745,756,766,775,784,792,798,804,
      3,45,85,124,162,206,241,275,308,340,370,399,426,452,476,500,522,544,565,585,605,623,639,655,670,685,699,713,726,738,749,760,770,779,787,794,800    ]
  }
}

   * 
   * 
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":201
      },
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":201,
        "Length":201
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":402,
        "Length":201
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":603,
        "Length":201
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        804
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Sweep Random",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 10,
    "BrightnessCCT": 0
  }
  )=====";

  #endif

  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__MANUAL_FIND_LEDS_ON_HAT



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
#ifdef DEVICE_ACTIVE_DEVELOPMENT__SWITCHES_AND_BUTTONS_COLORADO24
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "dev_switches_and_buttons"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif 
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
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


/** ONLY NOT BEING USED YET BELOW *************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 ****************************************************************************************************************************************************************************************************************************************************************************************************************************
 **************************************************************************************************************************************************************************************************************************************************************************************************************************** 
 */


#ifdef DEVICE_COLORADO24__MATRIX_MAPPED_PULSAR
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__colorado__redboard_01_matrix"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "colorado"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  #define SERIAL_LOG_LEVEL_DURING_BOOT 12

  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY

  // #define ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
  // #define ENABLE_DEBUG_LINE_HERE3
  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  #define ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

  #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS


  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  #define SETTINGS_HOLDER 1239

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

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTON__V2
    /**
     * @brief 
     * Button 1: Preset iter is press, hold in back to playlist
     * Button 2: Demo/Test mode (Do rainbow moving), or bus show, bus count,
     * 
     */

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER

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
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    // #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    


// #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
    #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MATRIX
    #define ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP  
    #define WLED_DEBUG
    #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
    // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
  
  
   #define ENABLE_DEVFEATURE_LIGHT__MATRIX_COLORADO_MATRIX_TREE
    #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
    #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 8 // Height
    #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 32 // Width
    #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical

  #endif 


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

  #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM

  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023


  // #define ENABLE_DEBUG_SPLASH_SYSTEM_PERFORMANCE_METRICS_TO_SERIAL

  #define DEBUG_ASYNC
  #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH

  
  // #define ENABLE_FEATURE_LIGHTING__SEQUENCER
  //   #define ENABLE_FEATURE_SEQUENCER__LOAD_DEVICE_LIST
  //   // #define ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE
  //   #define ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE
  //   #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS 
  //   // #define ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_TWO__ADDED_FLASHING_EFFECTS


  #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 4000


  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE

#define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

  #define ENABLE_BUSCONFIG_OPTION_01


  #ifdef ENABLE_BUSCONFIG_16X_3200

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connections
   * 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   * 
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
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":200
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":200
      },
      {
        "Pin":21,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":200
      },
      {
        "Pin":16,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":800,
        "Length":200
      },
      {
        "Pin":17,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1000,
        "Length":200
      },
      {
        "Pin":22,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1200,
        "Length":200
      },
      {
        "Pin":23,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1400,
        "Length":200
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1600,
        "Length":200
      },
      {
        "Pin":12,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":1800,
        "Length":200
      },
      {
        "Pin":26,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2000,
        "Length":200
      },
      {
        "Pin":32,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2200,
        "Length":200
      },
      {
        "Pin":14,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2400,
        "Length":200
      },
      {
        "Pin":27,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2600,
        "Length":200
      },
      {
        "Pin":25,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":2800,
        "Length":100
      },
      {
        "Pin":33,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB", 
        "Start":3000,
        "Length":200
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3200
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Sweep Random",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 10,
    "BrightnessCCT": 0
  }
  )=====";
  #endif


  #ifdef ENABLE_BUSCONFIG_OPTION_01

  // 4, 16, 17, 18, 19, 21, 22, 23, 2, 13, 14, 27, 26, 25, 33, 32
  /**
   * @brief 2023 Snow Tree physical wiring connections
   * 
   * 35
   * 34
   * RX0
   * TX0
   * 5
   * 2
   * 15
   * 
   * 
   * 
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        250
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Sweep Random",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  #endif

  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_COLORADO24__MATRIX_MAPPED_PULSAR





#ifdef DEVICE_TESTBED_WEBCAM_ARDUINO   //based on arduino core example : Feb 2023
  #define DEVICENAME_CTR                            "testbed_camera"
  #define DEVICENAME_FRIENDLY_CTR                   "testbed_camera"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SLEEP
  #define ENABLE_DEVFEATURE_SETDEBUGOUTPUT

  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
  #define USE_MODULE_DRIVERS__CAMERA_ARDUINO
    #define CAMERA_MODEL_WROVER_KIT
  // #define USE_MODULE_DRIVERS__CAMERA_TASMOTA
  //   #define ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER

  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_TESTBED_WEBCAM_ARDUINO




#ifdef DEVICE_COLORADO__WEBCAM_WROVER_01   //based on arduino core example : Feb 2023
  #define DEVICENAME_CTR                            "colorado_camera_01"
  #define DEVICENAME_FRIENDLY_CTR                   "colorado_camera_01"
  #define DEVICENAME_ROOMHINT_CTR                   "Colorado"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.50.206"
    #define MQTT_PORT 1883

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SLEEP
  #define ENABLE_DEVFEATURE_SETDEBUGOUTPUT

  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  // #define USE_MODULE_DRIVERS_CAMERA_OV2640
  #define USE_MODULE_DRIVERS__CAMERA_ARDUINO
    #define CAMERA_MODEL_WROVER_KIT
  // #define USE_MODULE_DRIVERS__CAMERA_TASMOTA
  //   #define ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER

  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // DEVICE_TESTBED_WEBCAM_ARDUINO






/**NEEDS SPORTING BELOW************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*************************************************************************************************************************************************
****** SECTION: Boards for testing ******************************************************************************************************************
****************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*******************************************************************************************************************************************/










/***
 * 
{
    "0": {},
    "1": {
        "n": "Slot 1",
        "Segment0": {
            "PixelRange": [
                0,
                100
            ],
            "ColourPalette": "Snowy 02",
            "Effects": {
                "Function": "Slow Glow",
                "Speed": 127,
                "Intensity": 127,
                "Grouping": 1,
                "RateMs": 1000
            },
            "BrightnessRGB": 100
        },
        "BrightnessRGB": 100
    },
    "2": {
        "n": "Slot 2",
        "Segment0": {
            "PixelRange": [
                0,
                100
            ],
            "ColourPalette": "Snowy 02",
            "Effects": {
                "Function": "Slow Glow",
                "Speed": 127,
                "Intensity": 127,
                "Grouping": 1,
                "RateMs": 1000
            },
            "BrightnessRGB": 100
        },
        "BrightnessRGB": 100
    }
}


{
  "0": {},
  "1": {
    "n": "Static,Snowy02,S127,R4000,G1,D0",
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Static Palette",
      "Intensity":127,
      "Speed":127,
      "Grouping":1,
      "Decimate":0,
      "Reverse":0,
      "RateMs": 4000
    },
    "BrightnessRGB": 100
  },
  "2": {
    "n": "FastPops,Snowy02,S255,R50,G1,D10",
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Slow Glow",
      "Intensity":20,
      "Speed":255,
      "Grouping":1,
      "Decimate":10,
      "RateMs": 50
    },
    "BrightnessRGB": 100
  },
  "3": {
    "n": "StepPalette,ColourfulPairs,S200,R2000,G1,D0",
    "ColourPalette":"Colourful Pairs 01",
    "Effects": {
      "Function":"Stepping Palette",
      "Intensity":20,
      "Speed":200,
      "Grouping":1,
      "Decimate":0,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "4": {
    "n": "StepPalette,ColourfulPairs,S200,R3000,G10,D0",
    "ColourPalette":"Colourful Pairs 01",
    "Effects": {
      "Function":"Stepping Palette",
      "Intensity":20,
      "Speed":200,
      "Grouping":10,
      "Decimate":0,
      "RateMs": 3000
    },
    "BrightnessRGB": 100
  },
  "5": {
    "n": "Static,Snowy02,S200,R4000,G10,D0",
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Static Palette",
      "Intensity":20,
      "Speed":200,
      "Grouping":10,
      "Decimate":0,
      "RateMs": 4000
    },
    "BrightnessRGB": 100
  },  
  "6": {
    "n":"RotatingPrevious,Snowy02,S255,R20,G1,D0",    
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Rotating Previous",
      "Intensity":0,
      "Speed":255,
      "Grouping":1,
      "Decimate":0,
      "RateMs": 20
    },
    "BrightnessRGB": 100
  },
  "7": {
    "n":"SpannedPalette,Random01,S126,R2000,G1,D0",    
    "ColourPalette":"Random 01",
    "Effects": {
      "Function":"Spanned Palette",
      "Intensity":255,
      "Speed":126,
      "Grouping":1,
      "Decimate":0,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "8": {
    "n":"SweepRandom,Snowy02,S255,R25,G1,D0",    
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Sweep Random",
      "Intensity":255,
      "Speed":255,
      "Grouping":1,
      "Decimate":0,
      "RateMs": 25
    },
    "BrightnessRGB": 100
  },
  "9": {
    "n":"Twinkle Snowy on White, Snowy 02, 0/100, G1, D0",    
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Twinkle Palette Two on One",
      "Intensity":3,
      "Speed":255,
      "Grouping":1,
      "Decimate":0,
      "Param0":99,
      "RateMs": 50
    },
    "BrightnessRGB": 95
  },
  "10": {
    "n":"Twinkle Snowy on White, Snowy 02, 0/100, G1, D0",    
    "ColourPalette":"Cold White",
    "Effects": {
      "Function":"Twinkle Palette Two on One",
      "Intensity":20,
      "Speed":255,
      "Grouping":1,
      "Decimate":0,
      "Param0":125,
      "RateMs": 100
    },
    "BrightnessRGB": 20
  },
  "11": {
    "n":"Shimmer, Snowy 02, 0/25, G1, D0",    
    "ColourPalette":"Snowy 02",
    "Effects": {
      "Function":"Shimmer",
      "Intensity":255,
      "Speed":200,
      "Grouping":1,
      "Decimate":0,
      "RateMs": 25
    },
    "BrightnessRGB": 100
  },
  "12": {
    "n":"Meteor Smooth, Orange & Teal",    
    "ColourPalette":"Orange & Teal",
    "Effects": {
      "Function":"Meteor Smooth",
      "Intensity":142,
      "Speed":70,
      "Grouping":1,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


 */

  










#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS23_H
