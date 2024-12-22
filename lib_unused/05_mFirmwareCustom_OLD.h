#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_OLD_PHASED_OUT_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_OLD_PHASED_OUT_H




#ifdef DEVICE_BEDROOM_CONTROLLER_BUTTONS_01
  #define DEVICENAME_CTR          "bedroom_controller_buttons_01"
  #define DEVICENAME_FRIENDLY_CTR "Bedside Controller for Room 01"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
    #define ENABLE_DEVFEATURE_BUTTONS_SEND_EVENT_MESSAGES
    #define ENABLE_DEVFEATURE_DISABLE_BUTTON_CAN_RESET_DEVICE

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"D1\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"D3\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
      "\"D4\":\"" D_GPIO_FUNCTION_KEY4_INV_CTR  "\","
      "\"D5\":\"" D_GPIO_FUNCTION_KEY5_INV_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_KEY6_INV_CTR  "\","
      "\"D7\":\"" D_GPIO_FUNCTION_KEY7_INV_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif




#ifdef DEVICE_H801_INSIDE_BEDROOM_WARDROBE_OLD
  #define DEVICENAME_CTR          "h801_bedroom_wardrobe"
  #define DEVICENAME_FRIENDLY_CTR "H801 h801_bedroom_wardrobe 3"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  
  // #define USE_SERIAL_ALTERNATE_TX
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // //#define FORCE_TEMPLATE_LOADING
  // // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_SENSORS_BUTTONS

  // #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  // #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM
  
  // 
  // #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  // #define MAX_NUM_SEGMENTS 5
  
  
  // #define 
  
  
  


  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  
  #define USE_MODULE_CORE_RULES

  // #define USE_MODULE_SENSORS_SUN_TRACKING
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"0\":\""  D_GPIO_FUNCTION_KEY1_INV_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 2
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":0,\"" D_RATE "\":1\"},"
    "\"" D_COLOUR_PALETTE "\":10,"
    "\"Hue\":120,\"Sat\":100\","
    "\"" D_ANIMATIONMODE  "\":\"" D_EFFECTS "\","
    "\"Effects\":{\"Function\":\"Solid\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"BrightnessRGB\": 100,"
    "\"CCT_TempPercentage\":0,"
    "\"BrightnessCCT\":100,"
    "\"Light\":{\"TimeOn\":60}"
  "}";


  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";


  // //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1   

  
  // // #define USE_MODULE_CORE_RULES
  
  // // #define USE_MODULE_SENSORS_INTERFACE
  // // #define USE_MODULE_SENSORS_BUTTONS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM


  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  // // need to add motion here

  // // // #define USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE

  // //#define USE_MODULE_SENSORS_SUN_TRACKING

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //    "\"" D_GPIOC "\":{"
  //     #ifdef USE_MODULE_SENSORS_BUTTONS
  //     "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
  //     #endif    
  //   "},"
  //   "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  // "}";
  
  // #define STRIP_SIZE_MAX 1 // PWM type, set size to 1
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_RGB_COLOUR_ORDER "\":\"RGBwc\","
  //   "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
  //   "\"" D_EFFECTS "\":{" 
  //     // "\"" D_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
  //     "\"" D_FUNCTION "\":8"//\"Sun Elevation RGBCCT Solid Palette 01\""
  //   "},"
  //   "\"" D_TRANSITION       "\":{"
  //     "\"" D_TIME "\":1,"
  //     "\"" D_RATE "\":5,"
  //     "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_ORDER "\":\"" D_RANDOM "\""
  //   "},"
  //   "\"" D_CCT_TEMP "\":300,"
  //   "\"" D_HUE "\":25,"
  //   "\"" D_SAT "\":100,"
  //   "\"" D_COLOUR_PALETTE "\":67,"
  //   "\"" D_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_BRIGHTNESS_RGB "\":100"
  // "}";


  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"
  //   "\"Rule0\":{" //switch example
  //     "\"Trigger\":{"
  //       "\"Module\":\"Buttons\","    //sensor
  //       "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
  //       "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
  //       "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
  //       "\"DeviceName\":0," //number, name, or all
  //       "\"State\":2" // toggle
  //     "}"
  //   "}"
  // "}"
    
#endif



#ifdef DEVICE_SHELLYDIMMER_BEDROOM_LAMP
  #define DEVICENAME_CTR          "dimmer_bedroom_lamp"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  // #define ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES
  #define USE_HARDWARE_DEFAULT_RULES_1

  #define DISABLE_SERIAL_LOGGING //temp measure
  // #define DISABLE_SERIAL0_CORE //dont think its needed

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "Switch2"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif




#ifdef DEVICE_BEDROOM_BEDLIGHT
  #define DEVICENAME_CTR          "bedroom_bedlight"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Bedlight H801"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_STRIP_SIZE       "\":1,"
  //   "\"" D_RGB_COLOUR_ORDER   "\":\"RGBCW\","
  //   "\"" D_TRANSITION     "\":{\"" D_TIME "\":2,\"" D_RATE "\":20},"
  //   "\"" D_COLOUR_PALETTE "\":\"Solid Rgbcct 00\","
  //   "\"" D_ANIMATIONMODE  "\":\"Effects\","
  //   "\"" D_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
  //   "\"" D_BRIGHTNESS     "\":100"
  // "}";

  
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_STRIP_SIZE       "\":1,"
    "\"" D_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_TRANSITION       "\":{\"" D_TIME "\":2,\"" D_RATE "\":20},"
    "\"" D_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_BRIGHTNESS_RGB   "\":100,"
    "\"" D_BRIGHTNESS_CCT   "\":100"
  "}";



#endif



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_LIGHT_SEGMENT_ESP8266__BEDROOM_ROOF
  #define DEVICENAME_CTR          "testbed_segment_esp82"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Animation Segment String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
      #endif 
      #ifdef USE_MODULE_LIGHTS_PWM
      "\"18\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"22\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"19\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","      
      #endif 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

/**
 * @brief The issue is template loading is not boot safe.
 * I need to move this to init() and have it configure after boot has happened using the new segment method
 * */
  #define STRIP_SIZE_MAX 100
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  #endif // USE_MODULE_LIGHTS_INTERFACE

  #define USE_LIGHTING_TEMPLATE__SINGLE
  // #define USE_LIGHTING_TEMPLATE__MULTIPLE_1
  // #define USE_LIGHTING_TEMPLATE_3

  #ifdef USE_LIGHTING_TEMPLATE__SINGLE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "ColourPalette":74,
    "Effects": {
      "Function":0,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__SINGLE

// {
//   "ColourPalette": 11,
//   "Effects": {
//     "Function": 0,
//     "Intensity": 127,
//     "Speed": 255
//   },
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 5000
//   },
//   "BrightnessRGB": 100,
//   "SegColour1": {
//     "Hue": 240,
//     "Sat":70
//   }
// }



// {
//   "HardwareType": "WS28XX",
//   "AnimationMode": "Effects",
//   "BrightnessRGB": 100,
//   "Segment0": {
//     "PixelRange": [
//       0,
//       5
//     ],
//     "ColourPalette": 10,
//     "Effects": {
//       "Function": 0,
//       "Intensity": 255
//     },
//     "SegColour":{"Hue":120,"Sat":100},
//     "Transition": {
//       "TimeMs": 900,
//       "RateMs": 1000
//     }
//   },
//   "Segment1": {
//     "PixelRange": [
//       5,
//       10
//     ],
//     "ColourPalette": "Christmas 01",
//     "Effects": {
//       "Function": 24,
//       "Speed": 255,
//       "Intensity": 255,
//       "SegBrightness":255
//     },
//     "Transition": {
//       "TimeMs": 0,
//       "RateMs": 23
//     }
//   }
// }

  #ifdef USE_LIGHTING_TEMPLATE__MULTIPLE_1
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "Segment0":{
      "PixelRange": [
        0,
        10
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 10
    },
    "Segment1": {
      "PixelRange": [
        10,
        20
      ],
      "ColourPalette": "Christmas 01",
      "Effects": {
        "Function": 24,
        "Speed":255,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "Segment2": {
      "PixelRange": [
        20,
        80
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":1,
        "Speed":100
      },
      "Transition": {
        "TimeMs": 400,
        "RateMs": 500
      },
      "BrightnessRGB": 100
    },
    "Segment3": {
      "PixelRange": [
        80,
        100
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":22,
        "Speed":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    }
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE_1

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__BEDROOM_ROOF
  #define DEVICENAME_CTR          "testbed_segment_esp32"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Animation Segment String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
      #endif 
      #ifdef USE_MODULE_LIGHTS_PWM
      "\"18\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"22\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"19\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","      
      #endif 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

/**
 * @brief The issue is template loading is not boot safe.
 * I need to move this to init() and have it configure after boot has happened using the new segment method
 * */
  #define STRIP_SIZE_MAX 100
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  #endif // USE_MODULE_LIGHTS_INTERFACE

  #define USE_LIGHTING_TEMPLATE__SINGLE
  // #define USE_LIGHTING_TEMPLATE__MULTIPLE_1
  // #define USE_LIGHTING_TEMPLATE_3

  #ifdef USE_LIGHTING_TEMPLATE__SINGLE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "Segment0":{
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "Effects": {
        "Function":1,
        "Intensity":50
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    }
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__SINGLE

// {
//   "ColourPalette": 11,
//   "Effects": {
//     "Function": 0,
//     "Intensity": 127,
//     "Speed": 255
//   },
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 5000
//   },
//   "BrightnessRGB": 100,
//   "SegColour1": {
//     "Hue": 240,
//     "Sat":70
//   }
// }



// {
//   "HardwareType": "WS28XX",
//   "AnimationMode": "Effects",
//   "BrightnessRGB": 100,
//   "Segment0": {
//     "PixelRange": [
//       0,
//       5
//     ],
//     "ColourPalette": 10,
//     "Effects": {
//       "Function": 0,
//       "Intensity": 255
//     },
//     "SegColour":{"Hue":120,"Sat":100},
//     "Transition": {
//       "TimeMs": 900,
//       "RateMs": 1000
//     }
//   },
//   "Segment1": {
//     "PixelRange": [
//       5,
//       10
//     ],
//     "ColourPalette": "Christmas 01",
//     "Effects": {
//       "Function": 24,
//       "Speed": 255,
//       "Intensity": 255,
//       "SegBrightness":255
//     },
//     "Transition": {
//       "TimeMs": 0,
//       "RateMs": 23
//     }
//   }
// }

  #ifdef USE_LIGHTING_TEMPLATE__MULTIPLE_1
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "Segment0":{
      "PixelRange": [
        0,
        10
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 10
    },
    "Segment1": {
      "PixelRange": [
        10,
        20
      ],
      "ColourPalette": "Christmas 01",
      "Effects": {
        "Function": 24,
        "Speed":255,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "Segment2": {
      "PixelRange": [
        20,
        80
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":1,
        "Speed":100
      },
      "Transition": {
        "TimeMs": 400,
        "RateMs": 500
      },
      "BrightnessRGB": 100
    },
    "Segment3": {
      "PixelRange": [
        80,
        100
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":22,
        "Speed":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    }
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE_1

#endif // DEVICE_RGBSTRING_CHRISTMAS_ESP32_TEST_TREE_XMAS2022




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN
  #define DEVICENAME_CTR          "testbed_segment_multipin_esp32"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

    // multipin should only be tested in lighting branch, now to remove all blocking code in wifi etc

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS


  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  #define ENABLE_DEBUG_FUNCTION_NAMES

  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  #define ESP32
  #undef ESP8266

  #define  ENABLE_DEBUG_MULTIPIN

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    

    

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812

      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_A_CTR "\":16,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_B_CTR "\":17,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_C_CTR "\":5,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_D_CTR "\":21,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_E_CTR "\":22,"  // PWM RGBCCT

      // "\"" D_GPIO_FUNCTION_PIXELBUS_06_A_CTR "\":23"                // PWM CCT
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


/**
 * @brief The issue is template loading is not boot safe.
 * I need to move this to init() and have it configure after boot has happened using the new segment method
 * */
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  // #endif // USE_MODULE_LIGHTS_INTERFACE

  #define USE_LIGHTING_TEMPLATE__SINGLE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT
  // #define USE_LIGHTING_TEMPLATE__MULTIPLE_1

  #ifdef USE_LIGHTING_TEMPLATE__SINGLE
  #define STRIP_SIZE_MAX 100
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
      "ColourPalette":"Christmas 24",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "ColourOrder":"rgb",
      "BrightnessRGB": 100
    }
    )=====";
  #endif // USE_LIGHTING_TEMPLATE__SINGLE

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":24,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG
  #define STRIP_SIZE_MAX 1000


  // Try my code without network to see if RMT and network is still the issue!

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"grb",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":1000
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":0,
    "Effects": {
      "Function":2,
      "Intensity":255
    },
    "Transition": {
      "TimeMs": 1000,
      "RateMs": 2000
    },
    "BrightnessRGB": 10
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS
  #define STRIP_SIZE_MAX 1001
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 120
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        16
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":2,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        16,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT
  #define STRIP_SIZE_MAX 120
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":1,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT



  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT
  #define STRIP_SIZE_MAX 120
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        16
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":129,
        "Intensity":127
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        16,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT





  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":24,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS

  #ifdef USE_LIGHTING_TEMPLATE__MULTIPLE_1
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "Segment0":{
      "PixelRange": [
        0,
        10
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 10
    },
    "Segment1": {
      "PixelRange": [
        10,
        20
      ],
      "ColourPalette": "Christmas 01",
      "Effects": {
        "Function": 24,
        "Speed":255,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "Segment2": {
      "PixelRange": [
        20,
        80
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":1,
        "Speed":100
      },
      "Transition": {
        "TimeMs": 400,
        "RateMs": 500
      },
      "BrightnessRGB": 100
    },
    "Segment3": {
      "PixelRange": [
        80,
        100
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":22,
        "Speed":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    }
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE_1


  /*
  
  
  {
  "BusConfig": [
    {
      "Pin": 4,
      "ColourOrder": "GRB",
      "BusType": "WS2812_RGB",
      "Start": 1,
      "Length": 16
    }
  ],
  "AnimationMode": "Effects",
  "ColourPalette": "Christmas 01",
  "Effects": {
    "Function": 21,
    "Intensity": 50,
    "Params": [
      10000,
      11,
      12,
      13
    ]
  },
  "BrightnessRGB": 100,
  "SegColour0": {
    "Hue": 120,
    "Sat": 100,
    "BrightnessRGB": 100
  },
  "SegColour1": {
    "Hue": 120,
    "Sat": 100,
    "BrightnessRGB": 100
  }
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  */

#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN







/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_LIGHT_SEGMENT_ESP8266__MULTIPIN_H801
  #define DEVICENAME_CTR          "testbed_segment_multipin_h801"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

    // multipin should only be tested in lighting branch, now to remove all blocking code in wifi etc

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS



  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  #undef ESP32
  #define ESP8266

  #define  ENABLE_DEBUG_MULTIPIN

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    


    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM


// #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

// #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
    //   #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      // "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":16,"                // Digital WS2812
    //   "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
    //   "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
    //   "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812

      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT

    //   // "\"" D_GPIO_FUNCTION_PIXELBUS_06_A_CTR "\":23"                // PWM CCT
    //   #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


/**
 * @brief The issue is template loading is not boot safe.
 * I need to move this to init() and have it configure after boot has happened using the new segment method
 * */
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  // #endif // USE_MODULE_LIGHTS_INTERFACE

  // #define USE_LIGHTING_TEMPLATE__SINGLE
  // #define USE_LIGHTING_TEMPLATE__SINGLE_H801
  // #define USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
  // #define USE_LIGHTING_TEMPLATE__H801_SPLIT_DOUBLE_CCT_CHANNELS_TWO_SEGMENTS
  // #define USE_LIGHTING_TEMPLATE__H801_SPLIT_DOUBLE_CCT_CHANNELS_ONE_WHITE_THREE_SEGMENTS
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
  // #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_TWO_SEGMENTS //ie have 2 segments showing across 2+3 outputs
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT
  // #define USE_LIGHTING_TEMPLATE__MULTIPLE_1

  #ifdef USE_LIGHTING_TEMPLATE__SINGLE
  #define STRIP_SIZE_MAX 100
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
      "ColourPalette":"Christmas 24",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "ColourOrder":"rgb",
      "BrightnessRGB": 100
    }
    )=====";
  #endif // USE_LIGHTING_TEMPLATE__SINGLE

  #ifdef USE_LIGHTING_TEMPLATE__SINGLE_H801
  #define STRIP_SIZE_MAX 100
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12,14,4],
          "ColourOrder":"RGBCW",
          "BusType":"ANALOG_5CH",
          "Start":0,
          "Length":1
        }
      ],
      "SegColour0": {
        "Hue": 240,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":100,
        "ColourType":5
      },
      "ColourPalette":"Christmas 24",
      "Effects": {
        "Function":2,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "ColourOrder":"rgbcw",
      "ColourType":5, 
      "BrightnessRGB": 1,
      "BrightnessCCT": 1
    }
    )=====";
  #endif // USE_LIGHTING_TEMPLATE__SINGLE_H801

  
  #ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 5
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12],
          "ColourOrder":"RGB",
          "BusType":"ANALOG_3CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":"Christmas 01",
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 0,
          "CCT_TempPercentage":100,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 900,
          "RateMs": 1000
        },
        "BrightnessRGB": 5
      },
      "Segment1": {
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette": "Orange White Blue",
        "SegColour0": {
          "Hue": 120,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":95,
          "ColourType":5
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 23
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      }
    }
    )=====";
/**
 * @brief 
 * 
 * 
 * {
  "Segment0": {
    "PixelRange": [
      0,
      1
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 0,
      "Sat": 100,
      "BrightnessRGB": 100,
      "BrightnessCCT": 0,
      "CCT_TempPercentage": 0,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0,
      "Intensity": 255
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 10,
    "BrightnessCCT": 0
  },
  "Segment1": {
    "PixelRange": [
      1,
      2
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 0,
      "Sat": 100,
      "BrightnessRGB": 100,
      "BrightnessCCT": 100,
      "CCT_TempPercentage": 50,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 100
  },
  "BrightnessRGB":100,
  "BrightnessCCT": 1
}




 {
  "Segment0": {
    "PixelRange": [
      0,
      1
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 240,
      "Sat": 100,
      "BrightnessRGB": 100,
      "BrightnessCCT": 0,
      "CCT_TempPercentage":0,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0,
      "Intensity": 255
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100,
    "BrightnessCCT":0
  },
  "Segment1": {
    "PixelRange": [
      1,
      2
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 240,
      "Sat": 100,
      "BrightnessRGB": 100,
      "BrightnessCCT": 100,
      "CCT_TempPercentage":0,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0,
      "Intensity": 255
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100,
    "BrightnessCCT":100
  }
}



{
  "Segment0": {
    "PixelRange": [
      0,
      1
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 240,
      "Sat": 100,
      "BrightnessRGB": 100,
      "BrightnessCCT": 0,
      "CCT_TempPercentage": 100,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0,
      "Intensity": 255
    },
    "Transition": {
      "TimeMs": 900,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  },
  "Segment1": {
    "PixelRange": [
      1,
      2
    ],
    "ColourPalette": "Orange White Blue",
    "SegColour0": {
      "Hue": 330,
      "Sat": 0,
      "BrightnessRGB": 100,
      "BrightnessCCT": 0,
      "CCT_TempPercentage": 100,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0,
      "Speed": 1,
      "Intensity": 1
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
}

{
  "Segment0": {
    "PixelRange": [
      0,
      1
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 240,
      "Sat": 100,
      "BrightnessRGB": 100,
      "BrightnessCCT": 0,
      "CCT_TempPercentage": 100,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0,
      "Intensity": 255
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
}


 */


  #endif // USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS

#ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_DOUBLE_CCT_CHANNELS_TWO_SEGMENTS


  #define STRIP_SIZE_MAX 2
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":"Christmas 01",
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 0,
          "CCT_TempPercentage":100,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 900,
          "RateMs": 1000
        },
        "BrightnessRGB": 5
      },
      "Segment1": {
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette": "Orange White Blue",
        "SegColour0": {
          "Hue": 120,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":95,
          "ColourType":5
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 23
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      }
    }
    )=====";


#endif // USE_LIGHTING_TEMPLATE__H801_SPLIT_DOUBLE_CCT_CHANNELS_TWO_SEGMENTS


#ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_DOUBLE_CCT_CHANNELS_ONE_WHITE_THREE_SEGMENTS


  #define STRIP_SIZE_MAX 3
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":"Christmas 01",
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 0,
          "CCT_TempPercentage":100,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 900,
          "RateMs": 1000
        },
        "BrightnessRGB": 5
      },
      "Segment1": {
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette": "Orange White Blue",
        "SegColour0": {
          "Hue": 120,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":95,
          "ColourType":5
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 23
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      }
    }
    )=====";


#endif // USE_LIGHTING_TEMPLATE__H801_SPLIT_DOUBLE_CCT_CHANNELS_ONE_WHITE_THREE_SEGMENTS


#ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_ONE_SEGMENTS

// #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE



  #define STRIP_SIZE_MAX 5
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 5
      }
    }
    )=====";

/*

{
  "Segment0": {
    "PixelRange": [
      0,
      5
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 240,
      "Sat": 0,
      "BrightnessRGB": 100,
      "BrightnessCCT": 100,
      "CCT_TempPercentage": 100,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0
    },
    "Transition": {
      "TimeMs":0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100,
    "BrightnessCCT_255": 255,
    "TimeOnSecs":10,
    "Override":{"Animation_Off":{"TimeMs":10000}}
  },
  "BrightnessRGB":100,
  "BrightnessCCT": 100
}






*/

#endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_ONE_SEGMENTS


#ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

// #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE



  #define STRIP_SIZE_MAX 5
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 5,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":20,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },      
      "Segment1":{
        "PixelRange": [
          2,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 5,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      }
    }
    )=====";

/*

{
  "Segment0": {
    "PixelRange": [
      0,
      5
    ],
    "ColourPalette": 10,
    "SegColour0": {
      "Hue": 240,
      "Sat": 0,
      "BrightnessRGB": 100,
      "BrightnessCCT": 100,
      "CCT_TempPercentage": 100,
      "ColourType": 4
    },
    "Effects": {
      "Function": 0
    },
    "Transition": {
      "TimeMs":0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100,
    "BrightnessCCT_255": 255,
    "TimeOnSecs":10,
    "Override":{"Animation_Off":{"TimeMs":10000}}
  },
  "BrightnessRGB":100,
  "BrightnessCCT": 100
}






*/

#endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":24,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG
  #define STRIP_SIZE_MAX 1000


  // Try my code without network to see if RMT and network is still the issue!

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"grb",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":1000
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":0,
    "Effects": {
      "Function":2,
      "Intensity":255
    },
    "Transition": {
      "TimeMs": 1000,
      "RateMs": 2000
    },
    "BrightnessRGB": 10
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS
  #define STRIP_SIZE_MAX 1001
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 120
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        16
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":2,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        16,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT
  #define STRIP_SIZE_MAX 120
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":1,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT



  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT
  #define STRIP_SIZE_MAX 120
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        16
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":129,
        "Intensity":127
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        16,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT





  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":24,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS

  #ifdef USE_LIGHTING_TEMPLATE__MULTIPLE_1
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "Segment0":{
      "PixelRange": [
        0,
        10
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 10
    },
    "Segment1": {
      "PixelRange": [
        10,
        20
      ],
      "ColourPalette": "Christmas 01",
      "Effects": {
        "Function": 24,
        "Speed":255,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "Segment2": {
      "PixelRange": [
        20,
        80
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":1,
        "Speed":100
      },
      "Transition": {
        "TimeMs": 400,
        "RateMs": 500
      },
      "BrightnessRGB": 100
    },
    "Segment3": {
      "PixelRange": [
        80,
        100
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":22,
        "Speed":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    }
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE_1


  /*
  
  
  {
  "BusConfig": [
    {
      "Pin": 4,
      "ColourOrder": "GRB",
      "BusType": "WS2812_RGB",
      "Start": 1,
      "Length": 16
    }
  ],
  "AnimationMode": "Effects",
  "ColourPalette": "Christmas 01",
  "Effects": {
    "Function": 21,
    "Intensity": 50,
    "Params": [
      10000,
      11,
      12,
      13
    ]
  },
  "BrightnessRGB": 100,
  "SegColour0": {
    "Hue": 120,
    "Sat": 100,
    "BrightnessRGB": 100
  },
  "SegColour1": {
    "Hue": 120,
    "Sat": 100,
    "BrightnessRGB": 100
  }
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  */

#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP8266__MULTIPIN_H801





/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_LIGHT__NOTIFICATIONS
  #define DEVICENAME_CTR          "testbed_notifications"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Notifications"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

    // multipin should only be tested in lighting branch, now to remove all blocking code in wifi etc

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS


  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  #define ENABLE_DEBUG_FUNCTION_NAMES

  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  #define ESP32
  #undef ESP8266

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20


  #define  ENABLE_DEBUG_MULTIPIN

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    

    

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812

      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_A_CTR "\":16,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_B_CTR "\":17,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_C_CTR "\":5,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_D_CTR "\":21,"  // PWM RGBCCT
      // "\"" D_GPIO_FUNCTION_PIXELBUS_05_E_CTR "\":22,"  // PWM RGBCCT

      // "\"" D_GPIO_FUNCTION_PIXELBUS_06_A_CTR "\":23"                // PWM CCT
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


/**
 * @brief 
 * 
 * 
 * {
  "LightNotif":[
    {
      "Segment":[0,0,4],
      "Colour":[120,100,100],
      "Params":[18, 998, 10, 50, 0]
    },
    {
      "Segment":[1,5,10],
      "Colour":[240,100,10],
      "Params":[18, 999, 100, 50, 0]
    }
  ]
}

{
  "LightNotif":[
    {
      "Segment":[0,0,4],
      "Colour":[255,2,3,4,5],
      "Params":[0, 998, 10, 50, 0]
    },
    {
      "Segment":[1,5,10],
      "Colour":[6,255,8,9,10],
      "Params":[0, 999, 100, 50, 0]
    },
    {
      "Segment":[2,11,15],
      "Colour":[11,12,255,14,15],
      "Params":[0, 999, 100, 50, 0]
    },
    {
      "Segment":[3,16,20],
      "Colour":[16,17,18,19,20],
      "Params":[0, 999, 100, 50, 0]
    }
  ]
}

{"Segment0":{
      "PixelRange": [
        0,
        8
      ],
      "ColourPalette":0,
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":100
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        8,
        50
      ],
      "ColourPalette": 0,
      "SegColour0": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 10
    }
}


 * 
 *
 * @brief The issue is template loading is not boot safe.
 * I need to move this to init() and have it configure after boot has happened using the new segment method
 * */
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  // #endif // USE_MODULE_LIGHTS_INTERFACE

  #define USE_LIGHTING_TEMPLATE__SINGLE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT
  // #define USE_LIGHTING_TEMPLATE__MULTIPLE_1
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__SINGLE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "ColourPalette":"Christmas 24",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "ColourOrder":"rgb",
      "BrightnessRGB": 100
    }
    )=====";
  #endif // USE_LIGHTING_TEMPLATE__SINGLE

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":24,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_CHANNELS

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG
  #define STRIP_SIZE_MAX 1000


  // Try my code without network to see if RMT and network is still the issue!

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"grb",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":1000
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":0,
    "Effects": {
      "Function":2,
      "Intensity":255
    },
    "Transition": {
      "TimeMs": 1000,
      "RateMs": 2000
    },
    "BrightnessRGB": 10
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_SINGLE_RMT_ONLY_DEBUG

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS
  #define STRIP_SIZE_MAX 1001
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 120
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        16
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":2,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        16,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 100
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":100
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        8
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":2,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        8,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_TWO_SEGMENTS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT
  #define STRIP_SIZE_MAX 120
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":1,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_ONE_I2S_CHANNELS_WITH_ONE_SEGMENTS_NOTIFICATION_SEGMENT



  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT
  #define STRIP_SIZE_MAX 120
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":1,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":16,
        "Length":984
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        16
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":129,
        "Intensity":127
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 5
    },
    "Segment1": {
      "PixelRange": [
        16,
        120
      ],
      "ColourPalette": "Orange White Blue",
      "SegColour1": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS_NOTIFICATION_SEGMENT





  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 5
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FOUR_CHANNELS


  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":16
      },
      {
        "Pin":13,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":15,
        "Length":3
      },
      {
        "Pin":14,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":18,
        "Length":3
      },
      {
        "Pin":27,
        "ColourOrder":"RGBW",
        "BusType":"SK6812_RGBW",
        "Start":21,
        "Length":3
      },
      {
        "Pin":[16,17,5,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":24,
        "Length":1
      }
    ],
    "AnimationMode":"Effects",
    "ColourPalette":"Christmas 01",
    "Effects": {
      "Function":11,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 500,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_FIVE_CHANNELS

  #ifdef USE_LIGHTING_TEMPLATE__MULTIPLE_1
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"WS28XX",
    "AnimationMode":"Effects",
    "Segment0":{
      "PixelRange": [
        0,
        10
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 10
    },
    "Segment1": {
      "PixelRange": [
        10,
        20
      ],
      "ColourPalette": "Christmas 01",
      "Effects": {
        "Function": 24,
        "Speed":255,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    },
    "Segment2": {
      "PixelRange": [
        20,
        80
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":1,
        "Speed":100
      },
      "Transition": {
        "TimeMs": 400,
        "RateMs": 500
      },
      "BrightnessRGB": 100
    },
    "Segment3": {
      "PixelRange": [
        80,
        100
      ],
      "ColourPalette": "Rainbow",
      "Effects": {
        "Function":22,
        "Speed":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 23
      },
      "BrightnessRGB": 100
    }
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE_1


  /*
  
  
  {
  "BusConfig": [
    {
      "Pin": 4,
      "ColourOrder": "GRB",
      "BusType": "WS2812_RGB",
      "Start": 1,
      "Length": 16
    }
  ],
  "AnimationMode": "Effects",
  "ColourPalette": "Christmas 01",
  "Effects": {
    "Function": 21,
    "Intensity": 50,
    "Params": [
      10000,
      11,
      12,
      13
    ]
  },
  "BrightnessRGB": 100,
  "SegColour0": {
    "Hue": 120,
    "Sat": 100,
    "BrightnessRGB": 100
  },
  "SegColour1": {
    "Hue": 120,
    "Sat": 100,
    "BrightnessRGB": 100
  }
}
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  */

#endif // DEVICE_TESTBED_LIGHT__NOTIFICATIONS





/**
 * @brief 
 * 
 * Complete replica of Whitehall heating, with leds in place of relays. Full board of sensors, nextion panel, rgb leds.
 * 
 * 
 * Cat5e Cable to Generic Room Sensor with BH1750, BME280 and HC-SR501 PIR
 * "*" pin = confirmed soldering
 * Ethernet      Function       ESP32         Note        
 * 
 * ** Blue (Upstairs Link) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           R_IH           21*           Immersion relay (5v IO)
 * bl/w          I2D            12*           DHT22?     
 * w/bl          I2C            13*           UNUSED   
 * g/w           3V3            3V3           
 * w/br                      
 * br/w          DSX            14*           ds18b20 water, 4k7 pulled high        (comes from blue by connector)   
 * ** Green (Downstairs Sensors) **************************************************************************
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           1Wire          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME in Dinning Room (Alternatively, DHT22 data pin)
 * w/bl          I2C            23*           BME in Dinning Room
 * g/w           3V3            3V3           White
 * w/br                                       NC
 * br/w          DHT22          25*           DHT22 (until BME is working)
 * ** Red (Relay Board) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g                          5*             
 * bl/w          R_DS           18*                   
 * w/bl                         19*            
 * g/w           R_WB           3V3           
 * w/br           -                           
 * br/w          R_US                         
 * ** Orange (Nextion Display) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO            27*             SK6812 4 pixels of rgbW
 * bl/w                                      
 * w/bl                                      
 * g/w           3V3            3V3           
 * w/br          RX2            17*              Nextion TX
 * br/w          TX2            16*              Nextion RX   -- "SERIAL TX" of Serial will always be dominant colour (brown) as its important to know the output pin
 * Twin          Switch
 * Twin          Switch
 * ** Power Screw Jacks **************************************************************************
 * 4 (Top)       12V
 * 3             5V
 * 2
 * 1 (Bottom)    GND
 * ** ADC Input **************************************************************************
 * 4 (Top)       LDR_US         33
 *               LDR_DS         32 
 *               LDR_WB         35
 * Extra Ethernet for LDRs hot glued onto the red led of the servos?
 * 
 * Upstairs Connectors
 * 3pin (DHT22)    - gnd,5v,dht_data 
 * 3pin (relay US) - gnd,5v,relay_ih
 * 3pin (water sensors) - gnd,3v3,ds18b20
 * 
 **/
#ifdef DEVICE_TESTBED_HVAC_HEATING_DUPLICATE
  #define DEVICENAME_CTR          "testbed_heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC House Heating"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SLEEP // loops per second less than 1hz // I need to make an "mqtt/alert" channel that lets me know this
  
  /**
   * @brief Put macbook air under stairs for arduino wiping with bad code (use old macbook?)
   **/

  /**
   * @brief Increasing buffers for the larger controller than default minimums
   * 
   */
  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 1000
  #define DEVICENAMEBUFFER_NAME_INDEX_LENGTH  100
  #define DB18_SENSOR_MAX                     15
  #define DATA_BUFFER_PAYLOAD_MAX_LENGTH      3000 //needed for db sensosrs, but currently causes crash in lighting
  #define MQTT_MAX_PACKET_SIZE                3000

  #define DISABLE_WEBSERVER

  #define USE_MODULE_CONTROLLER_HVAC
    // #define USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS
    #define ENABLE_DEVFEATURE_GET_SENSOR_READINGS_FOR_HVAC_ZONES

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_BME //removing for now, will place short wire one understairs and use for long term debugging
  #define USE_MODULE_SENSORS_DHT

  //add 3 LDRs onto the motor neons, so I can check if they are turned on ((hot glue them on)) 30+ pins == use this to know what has been turned on manually.
  //Also need to add a mains detector for the furnace trigger (orange wire from servos)
  
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  //  // Not ready to remove
  // #define STRIP_SIZE_MAX 4
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT
  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
  // #define USE_DEVFEATURE_FIX_TO_PIXEL_LENGTH
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  //   
  //   #define STRIP_SIZE_MAX 5
  //   #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //   #define USE_SK6812_METHOD_DEFAULT
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
  
  //   #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
  //   #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   
  //   #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY_V2
  
  // #define USE_MODULE_DISPLAYS_NEXTION
  // #define ENABLE_DEVFEATURE_NEXTION_DISPLAY

  // Actual
  #define GPIO_NAME_ZONE0_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL1_INV_CTR
  #define GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL2_INV_CTR
  #define GPIO_NAME_ZONE3_IMMERISON_RELAY   D_GPIO_FUNCTION_REL3_CTR
  #define GPIO_NAME_ZONE4_BOILER_RELAY      D_GPIO_FUNCTION_REL4_INV_CTR

  // Test GPIO
  // #define GPIO_NAME_ZONE0_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL1_CTR
  // #define GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL2_CTR
  // #define GPIO_NAME_ZONE3_IMMERISON_RELAY   D_GPIO_FUNCTION_REL3_CTR
  // #define GPIO_NAME_ZONE4_BOILER_RELAY      D_GPIO_FUNCTION_REL4_CTR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"5\":\""  GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\","
      "\"19\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
      "\"21\":\"" GPIO_NAME_ZONE3_IMMERISON_RELAY   "\","
      "\"18\":\"" GPIO_NAME_ZONE4_BOILER_RELAY      "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"25\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\"," // DS_DHT 
      // "\"12\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\"," // US_DHT 
      #endif
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"27\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"14\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // US_DB - 3 pin
      "\"4\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\"," // DS_DB - 3 pin
      #endif    
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Upstairs_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Downstairs_DHT"

  /**
   * @brief HVAC zones
   **/
  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"

  /**
   * @brief Pin_US 
   */
  #define D_DEVICE_SENSOR_DB18S20_00_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_00_ADDRESS     "[40,255,136,105,53,22,4,114]"

  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,255,50,176,193,23,4,197]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,255,216,108,53,22,4,102]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "Tank_Bottom"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,255,162,167,53,22,4,27]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "Tank_Out"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,255,219,93,53,22,4,239]"

  /** 
   * Pin_DS
   * */
  #define D_DEVICE_SENSOR_DB18S20_21_NAME        "Water21-Upstairs"
  #define D_DEVICE_SENSOR_DB18S20_21_ADDRESS     "[40,208,174,149,240,1,60,127]"

  #define D_DEVICE_SENSOR_DB18S20_22_NAME        "Water22-HotCross"
  #define D_DEVICE_SENSOR_DB18S20_22_ADDRESS     "[40,168,253,149,240,1,60,157]"

  #define D_DEVICE_SENSOR_DB18S20_23_NAME        "Water23-R/C"
  #define D_DEVICE_SENSOR_DB18S20_23_ADDRESS     "[40,12,164,2,0,0,0,72]"

  #define D_DEVICE_SENSOR_DB18S20_24_NAME        "Water24-Mains"
  #define D_DEVICE_SENSOR_DB18S20_24_ADDRESS     "[40,9,77,4,0,0,0,131]"

  #define D_DEVICE_SENSOR_DB18S20_25_NAME        "Water25-HotFromBoiler"
  #define D_DEVICE_SENSOR_DB18S20_25_ADDRESS     "[40,121,172,3,0,0,0,138]"

  #define D_DEVICE_SENSOR_DB18S20_26_NAME        "Water26-Downstairs"
  #define D_DEVICE_SENSOR_DB18S20_26_ADDRESS     "[40,205,241,149,240,1,60,148]"

  #define D_DEVICE_SENSOR_DB18S20_27_NAME        "Water27-R/H"
  #define D_DEVICE_SENSOR_DB18S20_27_ADDRESS     "[40,195,112,2,0,0,0,178]"

  #define D_DEVICE_SENSOR_DB18S20_28_NAME        "Water28-HotFromFurnace"
  #define D_DEVICE_SENSOR_DB18S20_28_ADDRESS     "[40,103,49,3,0,0,0,153]"

  #define D_DEVICE_SENSOR_DB18S20_29_NAME        "Water29-WaterBoiler"
  #define D_DEVICE_SENSOR_DB18S20_29_ADDRESS     "[40,183,162,149,240,1,60,24]"


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Upstairs
        "\"" D_DEVICE_SENSOR_DB18S20_00_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_21_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_22_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_23_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_24_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_25_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_26_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_27_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_28_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_29_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":[" 
        // Upstairs
        D_DEVICE_SENSOR_DB18S20_00_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","
        // Downstairs
        D_DEVICE_SENSOR_DB18S20_21_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_22_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_23_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_24_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_25_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_26_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_27_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_28_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_29_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        // "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        // "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\"," //debug fix
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\"," //debig fix
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "SK6812" "\","
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grbw\","
    "\"ColourPalette\":\"Christmas 01\","
    "\"Effects\": {"
      "\"Function\": \"Static\""
    "},"
    "\"CCT_TempPercentage\":100,"
    "\"BrightnessRGB\":10,"
    "\"BrightnessCCT\":10,"
    "\"Transition\": {"
      "\"Time\":0,"
      "\"RateMs\":1000"
    "}"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

  
#endif





/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_NEXTION_DISPLAY_GENERIC
  #define DEVICENAME_CTR            "testbed_nextion_display"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_PIR
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_STRIP_SIZE       "\":40,"
  //   "\"" D_RGB_COLOUR_ORDER "\":\"rgbwc\","
  //   "\"" D_TRANSITION       "\":{"
  //     "\"" D_TIME_MS "\":1000,"
  //     "\"" D_RATE_MS "\":1000,"
  //     "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_ORDER "\":\"" D_RANDOM "\""
  //   "},"
  //   "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
  //   "\"" D_EFFECTS "\":{" 
  //     "\"" D_FUNCTION "\":8"
  //   "},"
  //   "\"" D_CCT_TEMP "\":300,"
  //   "\"" D_HUE "\":25,"
  //   "\"" D_SAT "\":100,"
  //   "\"" D_COLOUR_PALETTE "\":\"Solid Rgbcct 00\"," //ie 10
  //   "\"" D_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_BRIGHTNESS_RGB "\":100"
  // "}";
  
  // #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  // #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}"
  // "}";

  // #define USE_FUNCTION_NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  // DEFINE_PGM_CTR(NEXTION_INIT_PANEL_COMMAND_TEMPLATE)
  // "{"
  //   // "\"" D_DEVICENAME "\":{"
  //   //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //   //     "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //   //   "],"
  //   //   "\"" D_MODULE_SENSORS_BME_CTR "\":["
  //   //     "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //   //   "]"
  //   // "}"
  //   "p[0]."


  // "}";

#endif


/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_WEBUI_BASIC_GUI_ESP32_JUNE23
  #define DEVICENAME_CTR            "testbed_webui_esp32"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed webui_esp32"
  #define DEVICENAME_ROOMHINT_CTR "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_NETWORK_WEBSERVER
  #define USE_MODULE_NETWORK_WEBSERVER

  #define ESP32
  #undef ESP8266

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif


/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_WEBUI_BASIC_GUI_ESP8266_JUNE23
  #define DEVICENAME_CTR            "testbed_webui_esp82"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  
  #define ENABLE_ADVANCED_DEBUGGING
  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    #define ENABLE_DEVFEATURE_REMOVING_HSBID



  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  #define ENABLE_DEVFEATURE_PALETTE__VECTORED


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  #define  ENABLE_DEBUG_MULTIPIN

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    




  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":2"   
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  

  #define USE_LIGHTING_TEMPLATE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

        // "ColourOrder":"RGBW",
        // "BusType":"SK6812_RGBW",
  #define STRIP_SIZE_MAX 30
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":30
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        30
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 3,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";


#endif





/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED__LIGHTING_MULTIPIN__ESP32_AS_NEOPIXEL_DUAL_I2S_BUS_WEBUI
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

    #define ENABLE_DEVFEATURE_REMOVING_HSBID


// Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// Gain complete understanding/implemenation as is, only then start changes. 
// WebUI of WLED style should become its own webserver option (not just the debug version).

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_NETWORK_WEBSERVER
  #define USE_MODULE_NETWORK_WEBSERVER

  #define ESP32
  #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  #define ENABLE_DEVFEATURE_PALETTE__VECTORED


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  #define  ENABLE_DEBUG_MULTIPIN

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


    #define ENABLE_WEBSERVER_LIGHTING_WEBUI

    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 30
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":30
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        30
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS


  
  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING
  #define STRIP_SIZE_MAX 100
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
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
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING
  
  #ifdef USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
  #define STRIP_SIZE_MAX 59
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":59
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        59
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 0,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__PALETTE_TESTING_CEILING


#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN





/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_NEXTION_DISPLAY_GENERIC_WITH_WEBUI
  #define DEVICENAME_CTR            "testbed_nextion_display"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_PIR
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


#endif






/**
 * This will become the final version of this panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_NEXTION_HEATING
  #define DEVICENAME_CTR            "testbed_nextion_heating"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Heating"

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_PIR

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_PIR
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_STRIP_SIZE       "\":40,"
  //   "\"" D_RGB_COLOUR_ORDER "\":\"rgbwc\","
  //   "\"" D_TRANSITION       "\":{"
  //     "\"" D_TIME_MS "\":1000,"
  //     "\"" D_RATE_MS "\":1000,"
  //     "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_ORDER "\":\"" D_RANDOM "\""
  //   "},"
  //   "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
  //   "\"" D_EFFECTS "\":{" 
  //     "\"" D_FUNCTION "\":8"
  //   "},"
  //   "\"" D_CCT_TEMP "\":300,"
  //   "\"" D_HUE "\":25,"
  //   "\"" D_SAT "\":100,"
  //   "\"" D_COLOUR_PALETTE "\":\"Solid Rgbcct 00\"," //ie 10
  //   "\"" D_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_BRIGHTNESS_RGB "\":100"
  // "}";
  
  // #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  // #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}"
  // "}";

  // #define USE_FUNCTION_NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  DEFINE_PGM_CTR(NEXTION_INIT_PANEL_COMMAND_TEMPLATE)
  "{"
    // "\"" D_DEVICENAME "\":{"
    //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
    //   "],"
    //   "\"" D_MODULE_SENSORS_BME_CTR "\":["
    //     "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
    //   "]"
    // "}"
    "p[0]."


  "}";

#endif




#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_3PIXELS
  #define DEVICENAME_CTR          "testbed_animation_segment_01"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}
{
  "PixelRange": [
    0,
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif


/**
 * @brief Used to test SK6812 rgbW (ie addressable 4 pixel)
 * 
 */
#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_4PIXELS
  #define DEVICENAME_CTR          "testbed_segment_4p_01"               APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Secondary Testbed for Segments 01"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 55                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   **/
  #define LIGHTING_TEMPLATE_DEFAULT_DOWNSTAIRS_TOILET

   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
   // cant be removed or does not compile, phase out, should always be included

  // I need to make a define difference between "h801" effects, and addressable effects, since there is overlapping effects

  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  /**
   * @brief Debug flags, used mostly be me
   * */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  

  /**
   * @brief The following templates are tested examples
   * */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_DEFAULT_DOWNSTAIRS_TOILET
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"Effects\"," 
    "\"Effects\":{"
      "\"Function\":\"Solid RGBCCT\""
    "},"
    "\"ColourPalette\":\"RGBCCTColour 00\"," 
    "\"Hue\":20,"
    "\"Sat\":100,"
    "\"CCT_TempPercentage\":100,"
    "\"Transition\":{"
      "\"TimeMs\":1000,"
      "\"RateMs\":2000"
    "},"    
    "\"BrightnessCCT\":100,"
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 01\"," 
    "\"Effects\":{"
      "\"Function\":\"" "Slow Glow" "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":1000,"
      "\"RateMs\":2000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
#endif




/**
 * @brief Using PWM manually, with blend time between previous and new number
 * This will fill the gap until PWM single channels in lighting is properly added
 * This will mean, each segment in lighting would be a single channel led with assigned pwm output
 * eg. 5 segments 1 per pin ||| 2 segments with 2 pin CCT only and segment with 3 pin RGB ||| etc
 * */
#ifdef DEVICE_TESTBED_H801_PWM_MANUAL
  #define DEVICENAME_CTR          "testbed_h801_manual"
  #define DEVICENAME_FRIENDLY_CTR "Testbed PWM H801 Manual"
  
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_DRIVERS_PWM

  #define USE_MODULE_SENSORS_BUTTONS

  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define MAX_NUM_SEGMENTS 5
  
  


  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_SUN_TRACKING
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"0\":\""  D_GPIO_FUNCTION_KEY1_INV_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":10,\"" D_RATE "\":20\"},"
    "\"" D_COLOUR_PALETTE "\":10,"
    "\"" D_ANIMATIONMODE  "\":\"" D_EFFECTS "\","
    "\"" D_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_BRIGHTNESS     "\":100"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"light\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";

    
#endif


/**
 * @brief Testing of some special palettes that change their progress based on sun elevation
 * */
#ifdef DEVICE_TESTBED_H801_SUNELEVATION_REACTIVE_PALETTES
  #define DEVICENAME_CTR          "testbed_h801_sun_palettes"
  #define DEVICENAME_FRIENDLY_CTR "Testbed H801 Sun Palettes"
    
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define ENABLE_DEVFEATURE_SOLAR_PALETTES
  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
  #define DEBUG_TARGET_ANIMATOR_SEGMENTS
  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_SUN_TRACKING
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"0\":\""  D_GPIO_FUNCTION_KEY1_INV_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":10,\"" D_RATE "\":20\"},"
    "\"" D_COLOUR_PALETTE "\":10,"
    "\"" D_ANIMATIONMODE  "\":\"" D_EFFECTS "\","
    "\"" D_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_BRIGHTNESS     "\":100"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"light\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";

    
#endif




/**
 * Development of LinearBlendVariable
 * */
#ifdef DEVICE_TESTBED_BLENDING_VARIABLES
  #define DEVICENAME_CTR          "testbed_h801_manual"
  #define DEVICENAME_FRIENDLY_CTR "Testbed PWM H801 Manual"
  
  
  // #define USE_SERIAL_ALTERNATE_TX
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_DRIVERS_PWM

  #define USE_MODULE_SENSORS_BUTTONS

  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM
  
  // 
  // #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define MAX_NUM_SEGMENTS 5
  
  


  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_SUN_TRACKING
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"D4\":\""  D_GPIO_FUNCTION_PWM1_CTR "\","
      "\"D1\":\""  D_GPIO_FUNCTION_PWM2_CTR "\","
      "\"D2\":\""  D_GPIO_FUNCTION_PWM3_CTR "\","
      "\"D6\":\""  D_GPIO_FUNCTION_PWM4_CTR "\","
      "\"16\":\""  D_GPIO_FUNCTION_PWM5_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":10,\"" D_RATE "\":20\"},"
    "\"" D_COLOUR_PALETTE "\":10,"
    "\"" D_ANIMATIONMODE  "\":\"" D_EFFECTS "\","
    "\"" D_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_BRIGHTNESS     "\":100"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"light\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";

    
#endif




#ifdef DEVICE_TESTBED_ANIMATION_ADDING_NOTIFICATION_MODE
  #define DEVICENAME_CTR          "testbed_animation_segment_01"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work


  // Create a class that is just for notifications, which can be added into the buffer of segment runtime


  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  

#endif




/**
 * @brief Used to test status leds on both hardware, needing analog (pwm) and digital drivers (the PWM vs IO needs to be lower level controls)
 * 
 */
#ifdef DEVICE_TESTBED_STATUS_LEDS
  #define DEVICENAME_CTR          "testbed_status_leds"               APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Testbed Status LEDs"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define USE_MODULE_DRIVERS_LEDS

   
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 
      "\"" "2" "\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
      "\"" "16" "\":\"" D_GPIO_FUNCTION_LED2_CTR  "\""
    #else
      "\"" "LBI" "\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
#endif

#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_DUAL_MQTT
  #define DEVICENAME_CTR          "testbed_animation_segment_01_dual"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 50                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  //#define ENABLE_DEVFEATURE_DUAL_MQTT_BROKER
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED  192,168,0,55

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}
{
  "PixelRange": [
    0,
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif





#ifdef DEVICE_TESTBED_WEBUI_ANIMATION_SEGMENTS_3PIXELS
  #define DEVICENAME_CTR          "testbed_webui_3pixels"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}
{
  "PixelRange": [
    0,
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif


#ifdef DEVICE_TESTBED_WEBUI_ANIMATION_SEGMENTS_4PIXELS
  #define DEVICENAME_CTR          "testbed_webui_4pixels"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 3                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  // #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEVFEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_COMPILE_TIME_DEFINES_OF_MODULE_LIST


  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW

  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  // #define ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}
{
  "PixelRange": [
    0,
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif





/**
 * @brief For the development and inclusion of the E131 protocol for xLight stream data
 * 
 */
#ifdef DEVICE_TESTBED_3PIXELS_MATRIX_E131_PROTOCOL
  #define DEVICENAME_CTR          "testbed_matrix_3p_e131"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"grb\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}
{
  "PixelRange": [
    0,
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif


#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_DUAL_MQTT
  #define DEVICENAME_CTR          "testbed_animation_segment_01_dual"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 50                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  //#define ENABLE_DEVFEATURE_DUAL_MQTT_BROKER
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED  192,168,0,55

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  
  
  
  
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
    #ifdef ESP8266
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * @brief The following templates are tested examples
   * 
   */
  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif


#endif




#ifdef DEVICE_TESTBED_MOTION_CLIMATE_SENSOR
  #define DEVICENAME_CTR          "testbed_mot_clim_sens"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_PIR
  #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D7\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
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
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
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
    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";
  

#endif

#ifdef DEVICE_TESTBED_RJ45_ESP8266_STANDARD_GPIO_CONFIG_TOGGLE_ALL_PINS
  #define DEVICENAME_CTR          "testbed_rj45_gpio_toggle"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D1\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" "BLUE_WHITE_B_W" "\","
        "\"" "WHITE_BLUE_W_B" "\""
      "]"
    "}"
  "}";

  // #define INSERT_CODE_INIT
  // #define INSERT_CODE_EVERY_SECOND digitalWrite(4,!digitalRead(4)); digitalWrite(5,!digitalRead(5)); 

  // WILL ADD THIS toggle via rules (per second) in the future
  
#endif


/**
 * @brief Test sensor code
 * 
 */
#ifdef DEVICE_TESTBED_HARDWARE_BME_ESP8266
  #define DEVICENAME_CTR          "testbed_bme"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_PIR
  // #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
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
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
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
    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";
  

#endif

#ifdef DEVICE_TESTBED_HARDWARE_BME_ESP32
  #define DEVICENAME_CTR          "testbed_bme"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_PIR
  // #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"18\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
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
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
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
    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";
  

#endif



/**
 * Testing motion triggers and rules
 * PIR and doppler, single GPIO input with timed output
 * */
#ifdef DEVICE_TESTBED_MOTION
  #define DEVICENAME_CTR            "testbed_motion" APPEND_ESP_TYPE_MQTT_STRING
  #define DEVICENAME_FRIENDLY_CTR   "testbed_motion" APPEND_ESP_TYPE_NAME_STRING
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_PIR

  #define USE_MODULE_CORE_RULES

  // next step, create a that unkown "getsensoranme" will return the friendly class name

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef ESP8266
        #ifdef USE_MODULE_SENSORS_PIR
        "\"D6\":\""  D_GPIO_FUNCTION_SWT1_INV_CTR "\","
        "\"D1\":\""  D_GPIO_FUNCTION_SWT2_INV_CTR "\","
        "\"D2\":\""  D_GPIO_FUNCTION_SWT3_INV_CTR "\","
        #endif
        "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #else
        #ifdef USE_MODULE_SENSORS_PIR
        "\"23\":\""  D_GPIO_FUNCTION_SWT1_INV_CTR "\","
        #endif
        "\"19\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIR1"
  #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "PIR2"
  #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "DOP1"

  /**
   * @brief 
   * On value/off value (use | naming) ie On|Off,Open|Closed,Locked|unlocked, 1|0
   *  
   */
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
    
  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // PIR detected
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "},"
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":2,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":2,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "}"
  "}";

#endif






/**
 * Basic version of measurement system
 * GPS will be recorded at 10Hz, and logged to SD card in json format for matlab parsing
 * */
#ifdef DEVICE_TESTBED_GPS_SDCARD_LOGGER

#define DEVICENAME_CTR            "node0"
  #define DEVICENAME_FRIENDLY_CTR   "node0"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "GPS"

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
  #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
  #define USE_SYSTEM_GPS_SD_LOGGER_CONTROLLER
  #define USE_SYSTEM_SDCARD_LOGGING
  //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  /**
   * Debug methods
   * */
  /**
   * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
   * */
  // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17
  #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

  /**
   * If enabled, disable normal logging methods
   * */
  #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
    #define DISABLE_SERIAL_LOGGING
    #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
  #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  /**
   * General defines to disable systems not needed
   * */
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  /**
   *  GPS
   * */
  #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
    #define USE_MODULE_SENSORS_GPS_SERIAL
    #define ENABLE_GPS_PARSER_NMEA
    #define ENABLE_GPS_PARSER_UBX
    #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
    #define NMEAGPS_DERIVED_TYPES
    #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
    #define NMEAGPS_PARSE_SAVE_MILLIS
    #define gpsPort Serial1
    #define D_GPS_BAUD_RATE_FAST    921600
    #define D_GPS_BAUD_RATE_DEFAULT 9600
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_1
    #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

  /**
   * Comms with pic32
   * */
  #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_2
    #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
    #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
    #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

  /**
   * SDCard
   * */
  #ifdef USE_SYSTEM_SDCARD_LOGGING
    #define USE_MODULE_DRIVERS_SDCARD
    #define USE_SDCARD_RINGBUFFER_STREAM_OUT
  #endif // USE_SYSTEM_SDCARD_LOGGING

  /**
   * Logger Controller
   * */
  #ifdef USE_SYSTEM_GPS_SD_LOGGER_CONTROLLER
    #define USE_MODULE_CONTROLLER_GPS_SD_LOGGER
    #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
    #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
  #endif // USE_SYSTEM_LOGGER_CONTROLLER

  /**
   * Button input
   * */
  #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
    #define USE_MODULE_CORE_RULES
    #define USE_MODULE_SENSORS_BUTTONS
  #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

  /**
   * OLED display
   * */
  #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SSD1306
    #define SHOW_SPLASH
  #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

  /**
   * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
   * */
  #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_2
    #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
    #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
    #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
  /**
   * I2S Internal Fast Sampling
   * */
  #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
    // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
    // // Next phase, will include saving adc values
    // // #define USE_MODULE_SENSORS_ADC_INTERNAL
    // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    // #define ENABLE_SMOOTHING_ON_ADC_READING
    // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // // #define ADC_CAPTURE_EXTERNAL_PIN 32
    // #define ADC_CAPTURE_INPUT0_PIN 34
    // #define ADC_CAPTURE_INPUT1_PIN 35
  #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     0 //USED
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing




#endif // DEVICE_GPS_SDCARD_LOGGER







#ifdef DEVICE_TESTBED_ULTRASONIC
  #define DEVICENAME_CTR          "testbed_ultrasonic"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"

  #define USE_MODULE_CONTROLLER_TANKVOLUME
    
  // #define USE_MODULE_SENSORS_DS18X
  
  #define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif        
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
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






/**
 * esp8266 version -- gy89
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_9AXIS_GRYO
  #define DEVICENAME_CTR          "testbed_9axis_gyro"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"

  //#define USE_MODULE_CONTROLLER_TANKVOLUME
    
  // #define USE_MODULE_SENSORS_DS18X

  #define USE_MODULE_SENSORS_LSM303D
  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !
  
  //#define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
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



/**
 * esp32 version -- mpu9250
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_9AXIS_GRYO_GY89
  #define DEVICENAME_CTR          "testbed_gy89"
  #define DEVICENAME_FRIENDLY_CTR "Testbed LSM303D2"


  #define DISABLE_NETWORK
  #define DISABLE_SLEEP
  
    #define USE_MODULE_SENSORS_LSM303D
    #define USE_MODULE_SENSORS_L3G

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1

  // #ifdef SENSOR_MULTIPLE_MODULE // new way of defined multiple sensors contained on one hardware  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
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



/**
 * esp32 version -- mpu9250
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_6DOF_ECOMPASS_LSM303D
  #define DEVICENAME_CTR          "testbed_6dof_ecompass"
  #define DEVICENAME_FRIENDLY_CTR "Testbed LSM303D2"

  // #define DISABLE_NETWORK
  // #define DISABLE_SLEEP
  
  #define USE_MODULE_SENSORS_LSM303D
  // #define USE_MODULE_SENSORS_L3G

  // #define I2C_ADDRESS_LSM303D 0x1d

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1

  // #ifdef SENSOR_MULTIPLE_MODULE // new way of defined multiple sensors contained on one hardware  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "ARM"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "LEG"
  

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_LSM303D_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";


#endif


/**
 * esp32 version -- mpu9250
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_9AXIS_GRYO_MPU9250
  #define DEVICENAME_CTR          "testbed_9axis_gyro"
  #define DEVICENAME_FRIENDLY_CTR "Testbed MPU9260"

  //#define USE_MODULE_CONTROLLER_TANKVOLUME
    
  // #define USE_MODULE_SENSORS_DS18X

  #define USE_MODULE_SENSORS_MPU9250

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !
  
  //#define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_MPU9250)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
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








#ifdef DEVICE_TESTBED_WEBCAM4   //based on arduino core example : Feb 2023
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
  // #define USE_MODULE_DRIVERS__CAMERA_ARDUINO
  #define USE_MODULE_DRIVERS__CAMERA_TASMOTA
    #define ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER

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

#endif // DEVICE_ESP32_DEVKIT_BASIC




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




/**
 * New heating controller, designed to work from single device to multizone system
 * For development of hvac with pzem to monitor power
 * 
 * To include:
 *  - BME? If I could place this on the plug into the wall, it would be elevated heat sensing? with 1m cable
 *  - Optional Ds18 that can be added to oil heater
 *  - Relay output (requiring level shifter)
 *  - PZEM at 5v, via level shifter
 *  - Power with mains internal? (possible danger with sensors?) -- 
 *  - Since pzem, this needs to esp32, but good practive for house hvac
 * 
 * */
#ifdef DEVICE_TESTBED_HVAC_HEAT_ONLY_WITH_ENERGY_SENSOR 
  #define DEVICENAME_CTR          "testbed_hvac_energy_sens"
  #define DEVICENAME_FRIENDLY_CTR "Testbed HVAC with Energy Sensor"

  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  #define ENABLE_HVAC_DEBUG_PINS_FOR_TESTING

  #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER

  
    /**
     * @brief 
     * Add special debug method here that will count loglevel by type so "error" messages can be counted and shared every X seconds on terminal
     * 
     */
  #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  

  // #define USE_MODULE_CONTROLLER_HVAC
    #ifdef ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
      #define HEATING_DEVICE_MAX 4
      #define HEATING_DEVICE_MAX 4
    #else  
      #define HEATING_DEVICE_MAX 1
      #define HEATING_DEVICE_MAX 1
    #endif // ENABLE_HVAC_DEBUG_PINS_FOR_TESTING

    #define ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS   

  
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X  // not working well on esp32, unreliable
  #define USE_MODULE_SENSORS_REMOTE_DEVICE

  #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  #define REMOTE_SENSOR_JSON_NAME "Bedroom"
  
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_ENERGY_PZEM004T_V3
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #endif // USE_MODULE_ENERGY_PZEM004T_V3
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif // USE_MODULE_SENSORS_BME
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"19\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR     "\","
      #endif // USE_MODULE_SENSORS_DS18X
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"21\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      #ifdef ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
      "\"25\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      #endif // ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
      #endif // USE_MODULE_DRIVERS_RELAY
      "\"LBI\":\"" D_GPIO_FUNCTION_BUILTIN_LED1_CTR  "\""      
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "DriverZone0"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "DriverZone1"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "DriverZone2"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "DriverZone3"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Zone0"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Zone1"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Zone2"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Zone3"

  #define D_DEVICE_SENSOR_ZONE_0_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_1_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_2_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_3_NAME REMOTE_SENSOR_JSON_NAME // tESTING REMOTE SENSOR VIA MQTT (LATER OPTIONS SHOULD INCLUDE DIRECT udp) "BME0"

  #define D_DEVICE_SENSOR_BME_0_NAME "BME0"
  #define D_DEVICE_SENSOR_BME_1_NAME "BME1"
  #define D_DEVICE_SENSOR_BME_2_NAME "BME2"
  #define D_DEVICE_SENSOR_BME_3_NAME "BME3"

  #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,170,67,3,30,19,2,25]"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,68,132,149,240,1,60,87]"



  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"

  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Cooker"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      // "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
      //   "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\"" //","
      //   // "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\""
      // "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      // "\"" D_MODULE_SENSORS_DB18S20_CTR "\":[" 
      //   D_DEVICE_SENSOR_DB18S20_0_ADDRESS //","
      //   // D_DEVICE_SENSOR_DB18S20_1_ADDRESS ""
      // "],"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS 
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_ZONE_0_NAME "\","
        "\"" D_DEVICE_SENSOR_ZONE_1_NAME "\"," // All use the same
        "\"" D_DEVICE_SENSOR_ZONE_2_NAME "\","
        "\"" D_DEVICE_SENSOR_ZONE_3_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "},"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "}"
  "}";
  
#endif




#ifdef DEVICE_TESTBED_LILYGO_SIM7000G
  #define DEVICENAME_CTR          "testbed_sim7000g"
  #define DEVICENAME_FRIENDLY_CTR "Neopixel RTOS Show"
  #define DEVICENAME_ROOMHINT_CTR "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define USE_MODULE_DRIVERS__CELLULAR_SIM7000
    // #define ENABLE_DEBUG_FEATURE_MQTT__CELLULAR_SIM__DEBUG_POLL_LATEST
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"18\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      "\"19\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
      #endif
      #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
      // "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Bedroom"
  #define D_DEVICE_SENSOR_CLIMATE "Bedroom"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["         // so this is probably to be phased out?
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "_Door" "\""
      "]"  
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // MOTION
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
    // "},"
    // // Door Opening
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":0,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1," 
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";


#endif



#ifdef DEVICE_TESTBED_LILYGO_SIM7000G_WITH_OLED
  #define DEVICENAME_CTR          "testbed_sim7000g_oled"
  #define DEVICENAME_FRIENDLY_CTR "Testbed SIM7000G with OLED"
  #define DEVICENAME_ROOMHINT_CTR "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define USE_MODULE_DRIVERS__CELLULAR_SIM7000
    // #define ENABLE_DEBUG_FEATURE_MQTT__CELLULAR_SIM__DEBUG_POLL_LATEST
    
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      /** 
       * OLED
       * */
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE_SENSORS_BME
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"18\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      "\"19\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
      #endif
      #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
      // "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Bedroom"
  #define D_DEVICE_SENSOR_CLIMATE "Bedroom"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["         // so this is probably to be phased out?
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "_Door" "\""
      "]"  
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // MOTION
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
    // "},"
    // // Door Opening
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":0,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1," 
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";


#endif

/**
 * @brief Device will largely remain active, as the primary test device for H801 code considered for deployment
 * 
 */
#ifdef DEVICE_TESTBED_H801_LONGTERM_DESK
  #define DEVICENAME_CTR          "testbed_h801_desk"
  #define DEVICENAME_FRIENDLY_CTR "H801 Primary Testbed"
  #define DEVICENAME_ROOMHINT_CTR "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    
    // #define ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS
    // #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_OLD_FUNCTIONS
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

    
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE



/***
 * 
 * 
 * 

{
  "ColourOrder":"RGBCW",
  "ColourPalette": "Solid Rgbcct 00",
  "Hue": 350,
  "Sat": 100,
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Solid"
  },
  "BrightnessRGB": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "Transition": {
    "RateMs": 4000,
    "TimeMs": 5000
  },
  "TimeOn": 3600
}

{
  "ColourPalette": "Solid Rgbcct 00",
  "Hue": 350,
  "Sat": 100,
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Solid"
  },
  "BrightnessRGB": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "Transition": {
    "RateMs": 0,
    "TimeMs": 100
  },
  "TimeOn": 3600
}


 * 
 * 
 * */


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_STRIP_SIZE       "\":1,"
    "\"" D_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_TRANSITION       "\":{\"" D_TIME "\":2,\"" D_RATE "\":20},"
    "\"" D_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_BRIGHTNESS_RGB   "\":100,"
    "\"" D_BRIGHTNESS_CCT   "\":100"
  "}";

#endif


#ifdef DEVICE_H801__TESTBED_H801V2_2023
  #define DEVICENAME_CTR          "h801_testbed_2023"
  #define DEVICENAME_FRIENDLY_CTR "H801 h801_bedroom_wardrobe 3"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801

  // #define USE_SERIAL_ALTERNATE_TX

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_PWM  
  //   
  //   /********* Group: Needed to build ************************/
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
  //   /********* Group: Ready for full integration ************************/
  //   // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   /********* Group: Testing ************************/
  //   #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
  //   
  //   #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
  //   
  //   
  //   // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
  //   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  //   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  //   // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
  //   /********* Group: Debug options only ************************/
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
  //   #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
  //   #define ENABLE_DEBUG_SERIAL
  //   // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  //   // #define ENABLE_LOG_LEVEL_DEBUG
  //   // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
  //   // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items





  // 
      
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define DISABLE_WEBSERVER
    
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // 
  //   #define ENABLE_DEVFEATURE_SOLAR_PALETTES
  //   #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
  //   #define DEBUG_TARGET_ANIMATOR_SEGMENTS
  //   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

  //   #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  //   #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  
  // // #define USE_SERIAL_ALTERNATE_TX
  // // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // // //#define FORCE_TEMPLATE_LOADING
  // // // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"0\":\""  D_GPIO_FUNCTION_KEY1_INV_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
 
  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"RGBCCT_PWM",
    "AnimationMode":"Effects",
    "ColourOrder":"RGBCW",
    "ColourPalette":"Rgbcct 01",
    "Effects": {
      "Function":0
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 120,
      "Sat": 100,
      "SubType":4,
      "CCT_TempPercentage":100
    },
    "BrightnessRGB_255": 0,
    "BrightnessCCT_255": 1
  }
  )=====";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";


  // //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1   

  
  // // #define USE_MODULE_CORE_RULES
  
  // // #define USE_MODULE_SENSORS_INTERFACE
  // // #define USE_MODULE_SENSORS_BUTTONS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM


  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  // // need to add motion here

  // // // #define USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE

  // //#define USE_MODULE_SENSORS_SUN_TRACKING

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //    "\"" D_GPIOC "\":{"
  //     #ifdef USE_MODULE_SENSORS_BUTTONS
  //     "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
  //     #endif    
  //   "},"
  //   "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  // "}";
  
  // #define STRIP_SIZE_MAX 1 // PWM type, set size to 1
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_RGB_COLOUR_ORDER "\":\"RGBwc\","
  //   "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
  //   "\"" D_EFFECTS "\":{" 
  //     // "\"" D_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
  //     "\"" D_FUNCTION "\":8"//\"Sun Elevation RGBCCT Solid Palette 01\""
  //   "},"
  //   "\"" D_TRANSITION       "\":{"
  //     "\"" D_TIME "\":1,"
  //     "\"" D_RATE "\":5,"
  //     "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_ORDER "\":\"" D_RANDOM "\""
  //   "},"
  //   "\"" D_CCT_TEMP "\":300,"
  //   "\"" D_HUE "\":25,"
  //   "\"" D_SAT "\":100,"
  //   "\"" D_COLOUR_PALETTE "\":67,"
  //   "\"" D_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_BRIGHTNESS_RGB "\":100"
  // "}";


  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"
  //   "\"Rule0\":{" //switch example
  //     "\"Trigger\":{"
  //       "\"Module\":\"Buttons\","    //sensor
  //       "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
  //       "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
  //       "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
  //       "\"DeviceName\":0," //number, name, or all
  //       "\"State\":2" // toggle
  //     "}"
  //   "}"
  // "}"
    
#endif




#ifdef DEVICE_LED_MATRIX_SEGMENTS //WHERE_EACH_ROW_IS_ANIMATION_SEGMENT
  #define DEVICENAME_CTR          "testbed_led_matrix"
  #define DEVICENAME_FRIENDLY_CTR "LED Matrix"
  #define DEVICENAME_ROOMHINT_CTR "Testbed"
  
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_CREATE_SSID_AP
  // #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  //#define DEBUG_FASTBOOT
  
  // #define ENABLE_DEVFEATURE_SETTINGS_JULY2022


  /**
   * @brief Fastboot should probably have its own RTC functions that are thread safe, then detailed RTC memory read in settings when stable and fastboot passed (ie read memory twice)
   *
   * 
   */

  // #define DISABLE_WEBSERVER
  //   #define USE_DEVFEATURE_NETWORK_WIFI_VERSION2
/**
 * @brief 
 * 
 
{
  "Segment0": {
    "PixelRange": [
      0,
      256
    ],
    "Effects": {
      "Function": 1
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "ColourPalette": 0,
    "Hue": 25,
    "Sat": 100,
    "BrightnessRGB": 2
  }
}

 * 
 */
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
     // Not ready to remove
    #define STRIP_SIZE_MAX 256+50
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define MAX_NUM_SEGMENTS 25
    // #define ENABLE_DEVFEATURE_PIXEL_MATRIX
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXEL_BUS
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
    // #define ENABLE_DEVFEATURE_FIREWORK_EFFECT_GETS_COLOUR_FROM_MY_PALETTES //to be joint later
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    


    // #define ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS


    // #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_OLD_FUNCTIONS
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grb\","
    "\"ColourPalette\":1,"
    "\"Effects\":{"
      "\"Function\":0,"
      "\"Intensity\":255,"
      "\"Speed\":255"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":1000"
    "},"    
    // "\"WLED\":{\"Colour1\":[0,0,0,0]},"
    "\"BrightnessRGB\":5"
    ",\"TimeOn\":10"
  "}";

  // show matrix
  // "{"
  //   // "\"Segment0\":{"
  //   //   "\"PixelRange\":[0,4],"
  //     "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //     "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //     "\"AnimationMode\": \"Effects\","
  //     "\"ColourOrder\": \"grb\","
  //     "\"ColourPalette\":96,"
  //     "\"Effects\":{"
  //       "\"Function\":16,"
  //       "\"Intensity\":255,"
  //       "\"Speed\":255"
  //     "},"
  //     "\"Transition\":{"
  //       "\"TimeMs\":25,"
  //       "\"RateMs\":25"
  //     "},"    
  //     "\"WLED\":{\"Colour1\":[0,0,0,0]},"
  //     "\"BrightnessRGB\":10"
  //     ",\"TimeOn\":10"
  //   // "}"
  // "}";

  
  // "{"
  //   // "\"Segment0\":{"
  //   //   "\"PixelRange\":[0,4],"
  //     "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //     "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //     "\"AnimationMode\": \"Effects\","
  //     "\"ColourOrder\": \"grb\","
  //     "\"ColourPalette\":96,"
  //     "\"Effects\":{"
  //       "\"Function\":14,"
  //       "\"Intensity\":255,"
  //       "\"Speed\":255"
  //     "},"
  //     "\"Transition\":{"
  //       "\"TimeMs\":25,"
  //       "\"RateMs\":25"
  //     "},"    
  //     "\"BrightnessRGB\":100,"
  //     "\"TimeOn\":30"
  //   // "}"
  // "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE

  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"Segment0\":{"
  //     "\"PixelRange\":[0,7],"
  //     "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  //     "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //     "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
  //     "\"Effects\":{"
  //       "\"Function\":\"Static\""
  //     "},"
  //     "\"ColourPalette\":\"Christmas 09\","
  //     "\"Transition\":{"
  //       "\"TimeMs\":3000,"
  //       "\"RateMs\":10000"
  //     "},"    
  //     "\"BrightnessRGB\":1"
  //   "},"
  //   "\"Segment1\":{"
  //     "\"PixelRange\":[8,10],"
  //     "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  //     "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //     "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
  //     "\"Effects\":{"
  //       "\"Function\":\"Solid RGBCCT\""
  //     "},"
  //     "\"ColourPalette\":\"Solid Rgbcct 01\","
  //     "\"Hue\":240"
  //     "\"Sat\":100"
  //     "\"Transition\":{"
  //       "\"TimeMs\":500,"
  //       "\"RateMs\":1000"
  //     "},"    
  //     "\"BrightnessRGB\":1"
  //   "},"
  //   "\"Segment2\":{"
  //     "\"PixelRange\":[10,13],"
  //     "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
  //     "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //     "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
  //     "\"Effects\":{"
  //       "\"Function\":\"Slow Glow\""
  //     "},"
  //     "\"ColourPalette\":\"Christmas 01\","
  //     "\"Transition\":{"
  //       "\"TimeMs\":500,"
  //       "\"RateMs\":1000"
  //     "},"    
  //     "\"BrightnessRGB\":1"
  //   "},"
  //   "\"" D_ANIMATIONMODE    "\":\"Effects\","
  //   "\"BrightnessRGB\":100"
  // "}";



#endif




/**
 * Primary code development of using external SD Card for storing and logging data 
 * - Storing for slow call back
 * - Logging/Appending for larger data files that will append a data entry (json format) for each new data block
 *    -- this might require ovverriding the end of the data file, appending a block, then reclosing the json data
 
 Figure out has tas foes filesystem, then merge sdcard into this (phasing it out, I may want it for a while)
 
 * */
#ifdef DEVICE_TESTBED_SDCARD_STORAGE
  #define DEVICENAME_CTR          "rgbcooker"
  #define DEVICENAME_FRIENDLY_CTR "RGB Cooker H801"
  #define DEVICENAME_ROOMHINT_CTR "Kitchen"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  
  // // Disable MQTT, or else run it temporarily on SLS for debugging
  
  //   #define DEVICENAME_CTR            "node0"
  // #define DEVICENAME_FRIENDLY_CTR   "node0"  //white wire, blue tape, to be uav 
  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "GPS"

  #define USE_MODULE_CORE_FILESYSTEM


  // /**
  //  * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
  //  * */
  // #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  // #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  // #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  // #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
  // #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
  // #define USE_SYSTEM_GPS_SD_LOGGER_CONTROLLER
  // #define USE_SYSTEM_SDCARD_LOGGING
  // //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  // /**
  //  * Debug methods
  //  * */
  // /**
  //  * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
  //  * */
  // // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  // // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  // // Also defining so "HardwareSerial.cpp" will use these
  // #define RX1 18
  // #define TX1 19
  // #define RX2 16
  // #define TX2 17
  // #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

  // /**
  //  * If enabled, disable normal logging methods
  //  * */
  // #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
  //   #define DISABLE_SERIAL_LOGGING
  //   #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
  // #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  // /**
  //  * General defines to disable systems not needed
  //  * */
  // #define DISABLE_NETWORK
  // #define DISABLE_SLEEP
  // #define ESP32
  // #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // /**
  //  *  GPS
  //  * */
  // #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  //   #define USE_MODULE_SENSORS_GPS_SERIAL
  //   #define ENABLE_GPS_PARSER_NMEA
  //   #define ENABLE_GPS_PARSER_UBX
  //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  //   #define NMEAGPS_DERIVED_TYPES
  //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  //   #define NMEAGPS_PARSE_SAVE_MILLIS
  //   #define gpsPort Serial1
  //   #define D_GPS_BAUD_RATE_FAST    921600
  //   #define D_GPS_BAUD_RATE_DEFAULT 9600
  //   #define USE_MODULE_DRIVERS_INTERFACE
  //   #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_1
  //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

  // /**
  //  * Comms with pic32
  //  * */
  // #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
  //   #define USE_MODULE_DRIVERS_INTERFACE
  //   #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_2
  //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  //   #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  //   #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  //   #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  // #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

  // /**
  //  * SDCard
  //  * */
  // #ifdef USE_SYSTEM_SDCARD_LOGGING
  //   #define USE_MODULE_DRIVERS_SDCARD
  //   #define USE_SDCARD_RINGBUFFER_STREAM_OUT
  // #endif // USE_SYSTEM_SDCARD_LOGGING

  // /**
  //  * Logger Controller
  //  * */
  // #ifdef USE_SYSTEM_GPS_SD_LOGGER_CONTROLLER
  //   #define USE_MODULE_CONTROLLER_GPS_SD_LOGGER
  //   #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  //   #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  //   #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
  // #endif // USE_SYSTEM_LOGGER_CONTROLLER

  // /**
  //  * Button input
  //  * */
  // #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  //   #define USE_MODULE_CORE_RULES
  //   #define USE_MODULE_SENSORS_BUTTONS
  // #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

  // /**
  //  * OLED display
  //  * */
  // #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
  //   #define USE_MODULE_DISPLAYS_INTERFACE
  //   #define USE_MODULE_DISPLAYS_OLED_SSD1306
  //   #define SHOW_SPLASH
  // #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

  // /**
  //  * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
  //  * */
  // #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  //   #define USE_MODULE_DRIVERS_INTERFACE
  //   #define USE_MODULE_DRIVERS_SERIAL_UART
  //   #define ENABLE_HARDWARE_UART_2
  //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  //   #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  //   #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  //   #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  // #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
  // /**
  //  * I2S Internal Fast Sampling
  //  * */
  // #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
  //   // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
  //   // // Next phase, will include saving adc values
  //   // // #define USE_MODULE_SENSORS_ADC_INTERNAL
  //   // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
  //   // #define ENABLE_SMOOTHING_ON_ADC_READING
  //   // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  //   // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  //   // // #define ADC_CAPTURE_EXTERNAL_PIN 32
  //   // #define ADC_CAPTURE_INPUT0_PIN 34
  //   // #define ADC_CAPTURE_INPUT1_PIN 35
  // #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     0 //USED
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing




#endif // DEVICE_GPS_SDCARD_LOGGER


/**
 * @brief 
 * 
 * To enable testing of "remote" sensors and drivers
 * Right now, just DHT22 and builtin LED as relay
 * 
 */
#ifdef DEVICE_DESKSENSOR_SLAVE_01
  #define DEVICENAME_CTR          "desksensor_slave"
  #define DEVICENAME_FRIENDLY_CTR "Desk Sensor Slave"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_DHT

  #define USE_MODULE_DRIVERS_INTERFACE
    #define ENABLE_FEATURE_DRIVERS_INTERFACE_UNIFIED_DRIVER_REPORTING
  #define USE_MODULE_DRIVERS_RELAY

  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      #ifdef USE_MODULE_SENSORS_DHT
      "\"13\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\"," 
      #endif
      "\"2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define D_DEVICE_DRIVER_RELAY_0_NAME "Immersion"
  
  #define D_DEVICE_SENSOR_CLIMATE_DHT1 "BedroomDeskRemote-DHT1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_DHT1 "\""
      "]," 
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "]" 
    "}," 
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";

     

#endif


#ifdef DEVICE_RGBCLOCK_TVROOM
  #define DEVICENAME_CTR          "rgbclock_tvroom"
  #define DEVICENAME_FRIENDLY_CTR "RGBW Clock 01"
  #define DEVICENAME_ROOMHINT_CTR "TVRoom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_SK6812_METHOD_DEFAULT
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    #define ENABLE_DEVFEATURE_INTERFACELIGHT_NEW_UNIQUE_TIMEON
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 94
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "ColourOrder":"grbw",
    "AnimationMode":"Effects",
    "ColourPalette":2,
    "Effects": {
      "Function":"Clock Basic 01",
      "Intensity":50,
      "Grouping":1
    },
    "Transition": {
      "TimeMs": 1000,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif



#ifdef DEVICE_RGB_SEVEN_SEGMENT_WEIGHT
  #define DEVICENAME_CTR          "rgbdisplay_weight"
  #define DEVICENAME_FRIENDLY_CTR "RGBW Clock 01"
  #define DEVICENAME_ROOMHINT_CTR "TVRoom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_SK6812_METHOD_DEFAULT
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    #define ENABLE_DEVFEATURE_INTERFACELIGHT_NEW_UNIQUE_TIMEON
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 94
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "ColourOrder":"grbw",
    "AnimationMode":"Effects",
    "ColourPalette":2,
    "Effects": {
      "Function":"Clock Basic 01",
      "Intensity":50,
      "Grouping":1
    },
    "Transition": {
      "TimeMs": 1000,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE

#endif



#ifdef DEVICE_RGBSTRING_DEVBOARD_SEGMENT
  #define DEVICENAME_CTR          "devboard_string_segment_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
    // #define USE_BUILD_TYPE_LIGHTING
    // #define USE_MODULE_LIGHTS_INTERFACE
    // #define USE_MODULE_LIGHTS_ANIMATOR
    // #define USE_MODULE_LIGHTS_ADDRESSABLE
    // 
    // // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    // #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // // enable some wled conerted aniamtions
    // 
    // #define DEBUG_WLED_EFFECT_FUNCTIONS
    // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    // #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    // #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // 

    // #define STRIP_SIZE_MAX 100
    
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // to merge h801 rgbcct animation into the new method
    #define DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

    

    #define STRIP_SIZE_MAX 1300
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS


    /**
     * @brief 
     * structs in headers
     * 13k to 18k
     * 
     */
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 
  #ifdef USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
 #define STRIP_SIZE_MAX 50
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #endif 
  #ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
 #define STRIP_SIZE_MAX 50
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
    #define LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "WLED"
    #define DEBUG_WLED_EFFECT_FUNCTIONS
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      //"\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 Segment command for single segment to replace old effects
 Without using a "Segment#" the commands will be assumed as for the entire strip and internally be segment0
 * 
 */
 #define USE_LIGHTING_TEMPLATE

//  #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":3000,"
      "\"RateMs\":10000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}

{
  "PixelRange": [
    0,
    49
  ],
  "AnimationMode": 3,
  "Strip": {
    "ClearTo": [
      0,
      0,
      0,
      0,0
    ],
    "ManualSetPixelToScene":[0,1,2,3,4,5,40,41,42,43,44,45,46,47,48,49]
  },
  "Effects": {
    "Function": 109
  },
  "ColourPalette": 10,
  "Hue": 20,
  "Sat": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 900,
    "RateMs": 20
  },
  "BrightnessRGB": 100
}

*/

#endif




#ifdef DEVICE_RGBSTRING_DEVBOARD_SEGMENT_ESP32
  #define DEVICENAME_CTR          "devboard_string_segment_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
/**
 * @brief 
 * 
 * Outside show
 * 
 * 6 - red/green
 * 12
 * 31
 */

 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD

    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // to merge h801 rgbcct animation into the new method
    #define DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

    

    #define STRIP_SIZE_MAX 1300

    
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 
  #ifdef USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #endif 
  #ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
    #define LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "WLED"
    #define DEBUG_WLED_EFFECT_FUNCTIONS
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      //"\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 Segment command for single segment to replace old effects
 Without using a "Segment#" the commands will be assumed as for the entire strip and internally be segment0
 * 
 */
//  #define STRIP_SIZE_MAX 1300
 #define USE_LIGHTING_TEMPLATE

//  #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":3000,"
      "\"RateMs\":10000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"RGB\","    
    "\"" D_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}

*/

#endif




#ifdef DEVICE_PZEM_TESTER
  #define DEVICENAME_CTR          "pzem_tester"
  #define DEVICENAME_FRIENDLY_CTR "PZEM Tester"
  
//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 2

//   #define USE_BUILD_TYPE_ENERGY
//   #define USE_MODULE_SENSORS_PZEM004T_V3
//   #define MAX_ENERGY_SENSORS 2

//   // #define ENABLE_BUG_TRACING
  
//   #define USE_SOFTWARE_SERIAL_DEBUG
//   #define DISABLE_SERIAL_LOGGING //temp measure

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_GPIOC "\":{"
//       "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
//       "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
//       "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
//       "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
//     "},"
//     "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Closed CT 100A"
//   #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Open CT 100A"
//   #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Inline 100ohm 10A"
  
//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_DEVICENAME "\":{"
//       "\"" D_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\""
//       "]"
//     "}"
//   "}";
// #endif

  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_BUILD_TYPE_ENERGY
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
  #define USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2

  // #define USE_MODULE_SENSORS_BME
  #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"

  #define ESP32

  // #define ENABLE_BUG_TRACING
  
  // #define USE_SOFTWARE_SERIAL_DEBUG
  //#define DISABLE_SERIAL_LOGGING //temp measure

  // Switching to esp32 for dual serial ports, to enable isolated pzem serial traffic until I can be assured I can disable serial0 on the esp8266

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      // #ifdef ESP266
      //   #ifdef USE_MODULE_SENSORS_BME
      //   "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      //   "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      //   #endif
      //   "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      //   "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      //   "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      //   "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
      // #endif //ESP266
      #ifdef ESP32
        #ifdef USE_MODULE_SENSORS_BME
        // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
        // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
        #endif
        // "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
        // "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
        // "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
        "\"LBI\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
      #endif //ESP32
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Cooker"
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Tumble Dryer"
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "Washing Machine"
  #define D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "Dish Washer"
  #define D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "Shower"
  #define D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "Sockets"
  #define D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "Kitchen Sockets"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_BEDROOM_PZEM_TESTER
  #define DEVICENAME_CTR          "bedroom_pzem"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom PZEM Tester"
  #define ESP32
    
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
  #define USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"      
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Test"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "24"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "25"
  #define ENERGY_SENSORS_DYNAMIC_TEMPFIX 2
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ""
      "]"  
    "},"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":2"    
    "}"
  "}";

#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_LIGHTING == RGB Lighting] ----------------------------


#ifdef DEVICE_CAM_TESTER 
  #define DEVICENAME_CTR          "cam_tester"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Tester"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_ENERGY_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  //#define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  //#define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   
  #define ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW    "v79.31.22+" 
  #define ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"    
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//  "{\"Na\":\"rgb\",\"FNa\":\"fname\",\"GC\":{\"pin1\":\"func1\",\"pin2\":\"func2\"}}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" D_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"" D_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR "\","
    "\"" D_TRANSITION       "\":{\"" D_TIME "\":10,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_RANDOM "\"},"
    "\"" D_COLOUR_PALETTE   "\":43,"
    "\"" D_ANIMATIONMODE    "\":\"" D_//Scene "\","
    "\"" D_//Scene_COLOUR     "\":{\"" D_HSB "\":[120,100,0]" "}," //this set the brightness
    "\"" D_BRIGHTNESS       "\":0,"
    "\"" D_BRIGHTNESS_RGB          "\":0"
  "}";

  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "]"
    "}"
  "}";

#endif




#ifdef DEVICE_RGBSTRING_LIGHTS1
  #define DEVICENAME_CTR            "rgbstring_lights1"
  #define DEVICENAME_FRIENDLY_CTR   "String Lights 1"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME_MS "\":9000,"
      "\"" D_RATE_MS "\":30000,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_RANDOM "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_BRIGHTNESS_RGB "\":0"
  "}";

#endif



#ifdef DEVICE_RGBSTRING_LIGHTS2
  #define DEVICENAME_CTR            "rgbstring_lights2"
  #define DEVICENAME_FRIENDLY_CTR   "String Lights 2"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 50

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME_MS "\":9000,"
      "\"" D_RATE_MS "\":30000,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_RANDOM "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";

#endif




#ifdef DEVICE_RGBSTRING_GARDEN_TREE_1
  #define DEVICENAME_CTR            "rgbstring_garden_tree1"
  #define DEVICENAME_FRIENDLY_CTR   "Garden Tree Lights 1"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 100

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME_MS "\":0,"
      "\"" D_RATE_MS "\":10,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_RANDOM "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_BRIGHTNESS_RGB "\":10"
  "}";

#endif


#ifdef DEVICE_TESTER_RGBW
  #define DEVICENAME_CTR            "tester_rgbw"
  #define DEVICENAME_FRIENDLY_CTR   "Tester RGBW WS2812 Lights"

  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define STRIP_SIZE_MAX 50

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_ANIMATIONMODE    "\":\""  D_//Scene  "\","
    "\"" D_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":0,"
    "\"" D_COLOUR_PALETTE "\":\"" "User 01" "\","
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";

#endif


/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_BEDROOM
  #define DEVICENAME_CTR          "hvacbedroom"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Bedroom"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER
  
  #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_REMOTE_DEVICE

  #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  #define REMOTE_SENSOR_JSON_NAME "Bedroom"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","

      // "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      
      // "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR   "\""
      
      "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":[" 
        D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_5_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif





/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_MASTERBEDROOM
  #define DEVICENAME_CTR          "hvac_masterbedroom"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Master Bedroom"
  #define DEVICENAME_ROOMHINT_CTR "Master Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  // #define DISABLE_WEBSERVER
  
  // #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  // #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define HEATING_DEVICE_MAX 1


  #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X
  // #define USE_MODULE_SENSORS_REMOTE_DEVICE

  // #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  // #define REMOTE_SENSOR_JSON_NAME "Bedroom"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{" 
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\""
      // "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      // "\"D1\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\""
      // "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","

      // "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      
      // "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR   "\""
      
      // "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  // #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  // #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif


/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_KITCHEN
  #define DEVICENAME_CTR          "hvac_kitchen"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Kitchen"

  #define HEATING_DEVICE_MAX 1

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  
  #define USE_MODULE_CONTROLLER_HVAC

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D2\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      #endif 
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  // #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  // #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";

  
  //#define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":0"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":1}}\""
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":30}}\""
      "}"
    "}"
  "}";
  
#endif





/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_BEDROOM_RAD
  #define DEVICENAME_CTR          "hvac_bedroom_rad"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Bedroom Oil Rad"

  #define HEATING_DEVICE_MAX 1

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_CONTROLLER_HVAC

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR  "\","
      #endif 
      "\"D7\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  // #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  // #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";

  
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":0"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOnSecs\\\":60}}\""
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"PowerState\\\":0}\""
      "}"
    "}"
  "}";
  
#endif



#ifdef DEVICE_HEATING_ESP32_TESTER
  #define DEVICENAME_CTR          "heating_esp32_tester"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Heating Gen 2"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP
  #define ENABLE_DEBUG_DELAYS

  #define DISABLE_WEBSERVER
  #define ESP32

  // #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18X

  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT 1
  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT 1

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      // "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"12\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // "\"14\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"27\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      // "\"25\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      // "\"26\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"23\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      // "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      // "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\"," //future bme
      // "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\"" //builtin led
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Upstairs_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Downstairs_DHT"
//flip these
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Downstairs_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,152,171,193,23,4,231]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Upstairs_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,131,6,194,23,4,59]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,204,226,193,23,4,30]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,136,105,53,22,4,114]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,50,176,193,23,4,197]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,216,108,53,22,4,102]"
  #define D_DEVICE_SENSOR_DB18S20_6_NAME        "Tank_Bottom"
  #define D_DEVICE_SENSOR_DB18S20_6_ADDRESS     "[40,255,162,167,53,22,4,27]"
  #define D_DEVICE_SENSOR_DB18S20_7_NAME        "Tank_Out"
  #define D_DEVICE_SENSOR_DB18S20_7_ADDRESS     "[40,255,219,93,53,22,4,239]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_6_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_7_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":[" 
        D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_5_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_6_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_7_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif



#ifdef DEVICE_RGBMICRO1
  #define DEVICENAME_CTR          "rgbmicro1"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  
  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  
  // #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 50

 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","
    // "\"" D_TRANSITION       "\":{\"" D_TIME "\":10,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_RANDOM "\"},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":5000,"
    "\"" D_COLOUR_PALETTE "\":19," //c12    43 is the colours for this christmas
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";

#endif

#ifdef DEVICE_RGBMICRO2 //projector
  #define DEVICENAME_CTR          "rgbmicro2"
  #define DEVICENAME_FRIENDLY_CTR "Projector Micro Lights"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_ENERGY_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

  //#define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  //#define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   
  #define ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW    "v79.31.22+" 
  #define ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"    
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//  "{\"Na\":\"rgb\",\"FNa\":\"fname\",\"GC\":{\"pin1\":\"func1\",\"pin2\":\"func2\"}}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" D_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"" D_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR "\","
    "\"" D_TRANSITION       "\":{\"" D_TIME "\":10,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_RANDOM "\"},"
    "\"" D_COLOUR_PALETTE   "\":43,"
    "\"" D_ANIMATIONMODE    "\":\"" D_//Scene "\","
    "\"" D_//Scene_COLOUR     "\":{\"" D_HSB "\":[120,100,0]" "}," //this set the brightness
    "\"" D_BRIGHTNESS       "\":0,"
    "\"" D_BRIGHTNESS_RGB          "\":0"
  "}";

  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_RGBMICRO3 //room speaker test
  #define DEVICENAME_CTR          "rgbmicro3"
  #define DEVICENAME_FRIENDLY_CTR "Micro Lights 3"  
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define STRIP_SIZE_MAX 100

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME_MS "\":9000,"
      "\"" D_RATE_MS "\":30000,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_RANDOM "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";

#endif


/**
 * Using a RGB matrix, display a simple moving fire pattern
 * Aim is to get stability of esp32 animations 
 * */
#ifdef DEVICE_RGBFIREPLACE_TESTER
  #define DEVICENAME_CTR          "rgbfireplace_tester"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fire Place 32"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER 
  // #define DISABLE_NETWORK
  // #define USE_MODULE_NETWORK_MQTT

  #define ENABLE_DEVFEATURE_DEBUG_GET_PIXEL_ZERO
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP
  // #define ENABLE_DEVFEATURE_BASIC_NEOPIXEL_TEST

  // #define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"23\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 300//256//100//256
  // #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 256
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_TRANSITION       "\":{"
      // "\"" D_TIME_MS "\":2000,"
      // "\"" D_RATE_MS "\":5000,"
      "\"" D_TIME_MS "\":20,"
      "\"" D_RATE_MS "\":50,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_ORDER "\":\"" D_RANDOM "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":\"" "FirePlace01" "\""
      // "\"" D_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_COLOUR_PALETTE "\":\"Single Fire 01\","
    // "\"" D_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_BRIGHTNESS_RGB "\":1"
  "}";

// "{HardwareType":"WS28XX","Transition":{"TimeMs":400,"RateMs":100,"PixelUpdatePerc":10,"Order":"Random"},"AnimationMode":"Effects","Effects":{"Function":"FirePlace01"},"ColourPalette":"Single Fire 01","BrightnessRGB":5}

#endif



#ifdef DEVICE_ENSUITEFAN_SENSOR
  #define DEVICENAME_CTR          "ensuitefan_sensor"
  #define DEVICENAME_FRIENDLY_CTR "ensuitefan Sensor"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  // #define USE_MODULE_SENSORS_PIR

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "ExtractorEnsuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "ExtractorEnsuite"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif




#ifdef DEVICE_RGBDISPLAY_GARAGE
  #define DEVICENAME_CTR          "rgbdisplay_garage"
  #define DEVICENAME_FRIENDLY_CTR "RGB Display Garage"
 
  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  #define STRIP_SIZE_MAX 93// 750   *15 //changing gazebo to be 12
    
  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME_MS "\":0,"
      "\"" D_RATE_MS "\":1000,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_INORDER "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"" D_COLOUR_PALETTE "\":41,"
    "\"BrightnessCCT\":10,"
    "\"" D_BRIGHTNESS_RGB "\":100"

  "}";


#endif



#ifdef DEVICE_RGBDININGROOMTREE 
  #define DEVICENAME_CTR          "rgbdiningroomtree"
  #define DEVICENAME_FRIENDLY_CTR "RGB Dining Room Tree"

 #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME

  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  

  #define STRIP_SIZE_MAX 150

  #define DISABLE_WEBSERVER
  // #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"    
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

   #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"RGB\","
    // "\"" D_TRANSITION       "\":{\"" D_TIME "\":10,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_RANDOM "\"},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":30,\"RateMs\":2000},"
    "\"TimeMs\":1000,"
    "\"" D_COLOUR_PALETTE "\":43," //c12    43 is the colours for this christmas
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";

#endif





//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBBACKDOORWREATH
  #define DEVICENAME_CTR          "rgbbackdoorwreath"
  #define DEVICENAME_FRIENDLY_CTR "RGB Backdoor Wreath"
  
  // #define ENABLE_DEVFEATURE_FLICKERING_TEST5

  // #define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  // DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)
  // {  // trans method, flasher, colours,   rate/time, mutlipler
  //   "Static/InOrder, SlowGlow, Few Colour, %d/%d, Single" "|" // 1
  //   "Medium/Random, SlowGlow, Few Colour, %d/%d, Single" "|" // 2
  //   "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Single" "|" // 3
  //   "Slow/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 4
  //   "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 5
  //   "Static/InOrder, SlowGlow, Few Colour 9, %d/%d, Rows" "|" // 6
  //   "Rotate/InOrder, Sequential, Many Colour 10, %d/%d, Rows" "|" // 7
  //   "Static/InOrder, SlowGlow, Berry Green, %d/%d, Rows" "|" // 8
  //   "Static/InOrder, SlowGlow, Berry Green, %d/%d, Single" "|" // 9
  //   "Fast/InOrder, TwinkleColour, Many Colour 10, %d/%d, Single" "|" // 10
  //   "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
  // };

/*
RAM:   [======    ]  56.2% (used 46024 bytes from 81920 bytes)
Flash: [======    ]  56.9% (used 582400 bytes from 1023984 bytes)*/
  #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  

  #define STRIP_SIZE_MAX 100

  // #define DISABLE_WEBSERVER
  // #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_TRANSITION       "\":{\"" D_TIME "\":10,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_RANDOM "\"},"
    "\"" D_COLOUR_PALETTE   "\":\"User 00\","
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":30,\"RateMs\":2000},"
    "\"TimeMs\":1000,"
    "\"" D_COLOUR_PALETTE "\": 43,"
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";


#endif






#ifdef DEVICE_H801_TESTER // for PWM dev
  #define DEVICENAME_CTR          "h801_tester"
  #define DEVICENAME_FRIENDLY_CTR "H801 Tester Strip"
    
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM

  #define USE_MODULE_SENSORS_SUN_TRACKING
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":1,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":0,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_INORDER "\"},"
    "\"" D_COLOUR_PALETTE "\":67,"
    "\"" D_ANIMATIONMODE  "\":\"" D_EFFECTS "\","
    "\"" D_EFFECTS        "\"{\"Function\":21},"
    "\"" D_BRIGHTNESS     "\":100"
  "}";

#endif



#ifdef DEVICE_H801_SUNLIGHT
  #define DEVICENAME_CTR          "h801_sunlight"
  #define DEVICENAME_FRIENDLY_CTR "H801 Sunlight"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1   

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM

  // #define USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
  #define USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK // ie, use a function handler that will allow adding a function to enable a constructjson using a callback, debug handler only, as it will use too much memory up on finished product

  /**
   * Add extra option that makes the palette (or else brightness) more emphasis on the ends, ie change to colours in evening, but take less time in the morning to make transition
   * This will maybe need a different animation type, evening stay brighter, morning wait until after sunrise by X time then quickly ramp up birghtness by a time amount 
   * 
   * */

  #define USE_MODULE_SENSORS_SUN_TRACKING

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1 // PWM type, set size to 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_RGB_COLOUR_ORDER "\":\"RGBcw\","
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      // "\"" D_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
      "\"" D_FUNCTION "\":\"Sun Elevation RGBCCT Solid Palette 01\""
    "},"
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME "\":1,"
      "\"" D_RATE "\":5,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_RANDOM "\""
    "},"
    "\"" D_CCT_TEMP "\":300,"
    "\"" D_HUE "\":25,"
    "\"" D_SAT "\":100,"
    "\"" D_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
    "\"" D_BRIGHTNESS_CCT "\":100,"
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";
    
#endif


#ifdef DEVICE_RGBBEDROOMFLOOR
  #define DEVICENAME_CTR          "rgbbedroomfloor"
  #define DEVICENAME_FRIENDLY_CTR "H801 Bedroom Floor Light"
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2 

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_SENSORS_SUN_TRACKING
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":10,\"" D_RATE "\":20\"},"
    "\"" D_COLOUR_PALETTE "\":67,"
    "\"" D_ANIMATIONMODE  "\":\"" D_EFFECTS "\","
    "\"" D_EFFECTS        "\"{\"Function\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_BRIGHTNESS     "\":100"
  "}";
  
#endif



#ifdef DEVICE_RGBCLOCK_01
  #define DEVICENAME_CTR            "rgbclock_01"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Clock 0l"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  #define STRIP_SIZE_MAX 93
  
  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_TRANSITION       "\":{"
      "\"" D_TIME_MS "\":0,"
      "\"" D_RATE_MS "\":1000,"
      "\"" D_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_ORDER "\":\"" D_INORDER "\""
    "},"
    "\"" D_ANIMATIONMODE    "\":\""  D_EFFECTS  "\","
    "\"" D_EFFECTS "\":{" 
      "\"" D_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"" D_COLOUR_PALETTE "\":41,"
    "\"BrightnessCCT\":10,"
    "\"" D_BRIGHTNESS_RGB "\":100"
  "}";
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif



#ifdef DEVICE_BLACKDOORBELL
  #define DEVICENAME_CTR          "blackdoorbell"
  #define DEVICENAME_FRIENDLY_CTR "Black Doorbell"

  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
     
  #define USE_MODULE_SENSORS_PIR
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_PIR
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"D2\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR     "\","
      
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "BlackDoorbell"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif



/**
 * Need to add cc1110 input, simple low/high into json output so I can show sync messages in recording
 * need 12v power source
 * 
 * */
#ifdef DEVICE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_UAV
  #define DEVICENAME_CTR            "radpattern"
  #define DEVICENAME_FRIENDLY_CTR   "radpattern"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "RP0"
  #define DEVICENUM_NUM   0

  // #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
  // #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER
  #define ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
  #define USE_SYSTEM_SDCARD_LOGGING
  #define USE_SYSTEM_IMU_RADIATIONPATTERN

  /**
   * Debug methods
   * */
  /**
   * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
   * */
  // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
    // #define USE_MODULE_SENSORS_LSM303D

  /**
   * ADC written directly into controller, not as a module
   * */
  #define USE_DEVFEATURE_ADC_IN_CONTROLLER

  /**
   *  IMU recording
   * */
  #ifdef USE_SYSTEM_IMU_RADIATIONPATTERN
    #define USE_MODULE_SENSORS_LSM303D
    #define USE_MODULE_SENSORS_L3G
    #define USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
  #endif

  /**
   * General defines to disable systems not needed
   * */
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  /**
   * SDCard
   * */
  #ifdef USE_SYSTEM_SDCARD_LOGGING
    #define USE_MODULE_DRIVERS_SDCARD
    #define USE_SDCARD_RINGBUFFER_STREAM_OUT
    #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
  #endif // USE_SYSTEM_SDCARD_LOGGING

  /**
   * Button input
   * */
  #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
    #define USE_MODULE_CORE_RULES
    #define USE_MODULE_SENSORS_BUTTONS
  #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

  /**
   * OLED display
   * */
  #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SSD1306
    #define SHOW_SPLASH
  #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     32(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      // "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_ADC1_CH4_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      /**
       * Optional added switches for controls eg radiation pattern enabling
       * GPIO21 - manual digitalread/set in radpat
       * GPIO22 - manual digitalread/set in radpat
       * */
      // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define GPIO_FUNCTION_MANUAL_ENABLE_SAMPLING_NUMBER 22
  #define GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER 27

  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "ARM"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "LEG"
  

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_LSM303D_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";



#endif // DEVICE_GPSPARSER_TESTER



#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
  #warning "--------------------------------------------------------------- DEVICE_FORCED_TO_BE_TESTER" is enabled for this compile
#endif



/**
 * @brief 
 * Minimal ESP32 7000G Modem to log position in realtime and SMS readback
 */
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01
  #define DEVICENAME_CTR          "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
    #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
  //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

/**
 * @brief 
 * 
 * 
// Add function to "settime_update" ie with GPS ticks. When it happens, reset the 
// update millis so it does not double update, same for uptime
 * 
 * 
 * If LTE connects, then set the mqtt retry counter to immediate
 * 
 */



  #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
  // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
  #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
  #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
  #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
  #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
  #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
  #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
  #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY

  #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 100
  #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  // #define USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS

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
     #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
    // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
    // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
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
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":20}"
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



#endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01








#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_BETA_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_BETA_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** New code under test, not yet ready to be used as a template (ie may not work yet)
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 * DEVICE_TYPE NETWORK: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01

/**
 * DEVICE_TYPE LIGHTING: Any testgroups related to lighting
 * */
// #define DEVICE_TESTGROUP__LIGHTING_MULTIPIN
  // #define SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS
  // #define SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS
  // #define SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__ESP32_AS_NEOPIXEL_DUAL_I2S_BUS

/**
 * DEVICE_TYPE DRIVERS: Any testbeds related to lighting
 * */
// #define DEVICE_ESP32_WEBCAM1

/**
 * DEVICE_TYPE CONTROLLERS: Any testbeds related to lighting
 * */


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************
  @brief TESTGROUP__LIGHTING_MULTIPIN
 ************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************/

// define all shared params here, and only device related in "subdevice"
#ifdef DEVICE_TESTGROUP__LIGHTING_MULTIPIN

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS



  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  #define  ENABLE_DEBUG_MULTIPIN

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    


    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



// #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

// #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    #define ENABLE_DEVFEATURE__PIXEL_USE_I2S_FOR_BUS
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    



#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS
  #define DEVICENAME_CTR          "testgroup_multipin_h801_rgbsplitcct"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 RGB Split CCT Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
    #define STRIP_SIZE_MAX 2
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12],
          "ColourOrder":"RGB",
          "BusType":"ANALOG_3CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],    
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":90,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB":10,
      "BrightnessCCT":10
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS
  #define DEVICENAME_CTR          "testgroup_multipin_h801_5white"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 Five White Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
    #define STRIP_SIZE_MAX 5
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          2,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      "BrightnessRGB":100,
      "BrightnessCCT":1
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS








/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__ESP32_AS_NEOPIXEL_DUAL_I2S_BUS
  #define DEVICENAME_CTR          "testgroup_multipin_esp32_neodual"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 RGB Dual Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
  #define STRIP_SIZE_MAX 120
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":3
      },
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":3,
        "Length":117
      }
    ],
    "Segment0":{
      "PixelRange": [
        0,
        3
      ],
      "ColourPalette":"Christmas 01",
      "Effects": {
        "Function":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "Segment1": {
      "PixelRange": [
        3,
        120
      ],
      "ColourPalette":"Christmas 01",
      "SegColour0": {
        "Hue": 330,
        "Sat":0,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 2,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS
/*




*/
#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN


















#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_BETA_H















#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_OLD_PHASED_OUT_H

