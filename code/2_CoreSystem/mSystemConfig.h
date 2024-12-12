#ifndef MSYSTEMCONFIG2_H
#define MSYSTEMCONFIG2_H

// File contains settings for the project that should not be edited by user

#include "stdint.h"
// DEVICE named defines that set modules to use below
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"

enum FIRMWARE_VERSION_BRANCH_TYPE_IDS{
  FIRMWARE_VERSION_TYPE_RELEASE_ID = 0,  
  FIRMWARE_VERSION_TYPE_RELEASE_CANDIDATE_ID,
  FIRMWARE_VERSION_TYPE_BETA_ID,  
  FIRMWARE_VERSION_TYPE_DEVELOPING_ID
};

#define FIRMWARE_VERSION_TYPE     FIRMWARE_VERSION_TYPE_DEVELOPING_ID
#define FIRMWARE_VERSION_MAJOR    0 // Reserved for webserver working, settings saving, and being able to export (as json AND bytes) that settings

/**@@@@@ Minor Changes - aim to make these quarterly
 * #126 : During Colorado
 * #125 : Rework of MQTT
 * #124 : 11Aug24, optimising starting and partial NPB update but using older methods with double buffer.
 * #122 : April 2024, lighting refactor and 2D
 * #121 : Jan 2024, starting with LTE tracing
 * #120 : XMAS 2023
 * #119 : Updated to latest neopixelbus library, and added support for 8/16 pin parallel output.
 * #118 : Nextion support added. TFT Upload via WebUI Async Webserver.
 * #117 : Introduction of first lighting testgroup (ie 6 candidate template default options to provide new users)
 * #116 : Multipin support on I2S (dual neopixel) and PWM. Introduction of TestGroups.
 * #115 : Improvements from networks and lighting merged into dev_main. dev_main will be primary focus of developing until LTE and multipin are stable.
 * #114 : Cleaned up mqtt topic for interface_## to keep all interface types together. Requires updating path.
 * #113 : Main development branch for 2023
 * #112 : After Colorado, still the lighting only branch
 * #111 : Working on lighting as its own branch. Removing any WLED palette usage until my palatte works!
 * #110 : Before colorado
 */
#define FIRMWARE_VERSION_MINOR    126 // Update "ChangeLogManual.md" when incrementing

/**@@@@@ Core Changes
 * #06 : New BME680 added, bme/p 180/280 manual without library
 * #02 : immersion controller added, more bug fixes pushes throughout more devices
 * #01 : db18 esp32 working
 * #00 : Restarting for 2023
 * #28 : Stable boot only happens after 120 seconds now, as work around for not checking for mqtt/network valid. ie, currently if mqtt connects and some bad pointer exists then the mqtt will try to send it before it works. This will catch that and cause fastboot recovery. Recovery period will be (120 seconds * recovery counter)
 * #28 : Added multiple methods to trigger crashes on mqtt command
 * #27 : Fixed WDT to work inside OTA
 *       Set WDT to 60 seconds on esp32
 * #26 : Enabled fastboot detection, OTA fallback and WDT on ESP32 devices as default
 */
#define FIRMWARE_VERSION_CORE     0

/**@@@@@ Module changes
 * #02 : db18 esp32 working
 * #01 : Ability to fade shelly dimmer with fader
 * #00 : Restarting for 2023
 * #33 : Fixed incorrect use of virtual/segment length, grouping of up to 200 has been tested for some effects
 * #32 : Moved lighting branch back to desktop 
 * #31 : Adding a palette step that splits progmem loads to only when a palette is loaded 
 * #30 : Added ultrasonic to unified sensor
 * #29 : Flashed testbed shelly2.5 to try new modeC for outside lights that simply reports switches as motion events, with relays to be remote OH controlled
 */
#define FIRMWARE_VERSION_MODULE   0

#define FIRMWARE_BRANCH_NAME_STRING "development"

// #define ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE

/**
 * @brief Version names used to set limits on when code can be included
 * 
 * 0V124 ie MAJORvMINOR
 */
#define FIRMWARE_VERSION_MIN(major, minor) \
  ((FIRMWARE_VERSION_MAJOR > (major)) || \
   (FIRMWARE_VERSION_MAJOR == (major) && FIRMWARE_VERSION_MINOR >= (minor)))

#define FIRMWARE_VERSION_MAX(major, minor) \
  ((FIRMWARE_VERSION_MAJOR < (major)) || \
   (FIRMWARE_VERSION_MAJOR == (major) && FIRMWARE_VERSION_MINOR <= (minor)))



#ifndef SETTINGS_HOLDER
#define SETTINGS_HOLDER           1//                (random(1,1000))
#endif

/**
 * @brief Depreciation code
 **/
#define DEPRECIATION_FIRMWARE_ERROR_MESSAGE "Depreciated Error: Must remove before compile should proceed"
// https://en.cppreference.com/w/cpp/language/attributes/deprecated

// #define DEPRECIATION_WARN(warning_firmware_version_minor, error_firmware_version_minor) \                          
//                     if(FIRMWARE_VERSION_MINOR > 114)   \
//                       #error "DEPRECIATION_ERROR"        \
//                     if(FIRMWARE_VERSION_MINOR > 114)   \
//                       #error "DEPRECIATION_ERROR"        \


// #define ENABLE_BUG_TRACING

// BUG TRACING
#ifdef ENABLE_BUG_TRACING
  #define ENABLE_SERIAL_FLUSH
  #define ENABLE_ADVANCED_DEBUGGING // new feature that only includes lowest level debugging when needed
  #define DEBUG_SERIAL_TESTING // Sets debug level of serial, else info for performance improvements
  #define DEBUG_MQTT_HANDLER
  #define DEBUG_FOR_FAULT
  #define USE_DEBUG_LINE  
  #define USE_DEBUG_LINE_LED
  #define ENABLE_DELAYED_BOOT
  #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_LOG_LEVEL_NONE 
  #define ENABLE_LOG_LEVEL_ERROR 
  #define ENABLE_LOG_LEVEL_WARNING 
  #define ENABLE_LOG_LEVEL_TEST
  #define ENABLE_LOG_LEVEL_INFO 
  #define ENABLE_LOG_LEVEL_COMMANDS
  #define ENABLE_LOG_LEVEL_DEBUG 
  #define ENABLE_LOG_LEVEL_DEBUG_MORE 
  #define ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL 
  #define ENABLE_LOG_LEVEL_ALL

  #define ENABLE_LOG

  // Enable debugging on only this module parts
  #define ENABLE_DEBUG_MODULE_LIGHTS_ADDRESSABLE

#endif

// Enable special d ebugs that trigger only regions
// #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

#define ENABLE_LOG


// To reduce ram usage
#define ENABLE_LOG_LEVEL_NONE 
#define ENABLE_LOG_LEVEL_ERROR 
#define ENABLE_LOG_LEVEL_WARNING
#define ENABLE_LOG_LEVEL_TEST
#define ENABLE_LOG_LEVEL_IMPORTANT 
#define ENABLE_LOG_LEVEL_INFO 
#define ENABLE_LOG_LEVEL_COMMANDS
#define ENABLE_LOG_LEVEL_HIGHLIGHT
//#define ENABLE_LOG_LEVEL_DEBUG 
//#define ENABLE_LOG_LEVEL_DEBUG_MORE 
//#define ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
//#define ENABLE_LOG_LEVEL__DEBUG_TRACE 
//#define ENABLE_LOG_LEVEL_ALL

#define DEBUG_WEBSERVER_LIB  //used in improving core lib
// #define DEBUG_WEBSERVER_MEMORY


// // Muted blue/green
// #define COLOR_BUTTON_HIGHLIGHT      "#1be7f1"
// #define COLOR_BUTTON                "#4d8d90"    // [WebColor11] Button color - Blueish
// #define COLOR_BUTTON_HOVER          "#31595b"
// #define D_COLOUR_PAGE_TITLE         "#4bb7e6"
// #define D_COL_TEXT_MODULE_TITLE_CTR "#4bb7e6"

#define ENABLE_USER_CONFIG_OVERRIDE

//  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES


/**
 *  Extra Debugging methods
 **/
//#define ENABLE_FREERAM_APPENDING_SERIAL
// #define ADD_DEBUG_PERIODS_FOR_SERIAL_MONITORING
// #ifdef ENABLE_ADVANCED_DEBUGGING
//   #define DEBUG_FOR_FAULT
// #endif

//--------------------------------[Defines needed here]-------------------------------------

// Project version is bit-packed as follows
// bits 31-30  (2)  (Range 0-4)     Type/Branch (e.g. Stable, Developing)
// bits 29-25  (6)  (Range 0-77)    Major version 
// bits 24-16  (8)  (Range 0-255)   Minor version 
// bits 15-8   (8)  (Range 0-255)   System version (e.g. Wifi, Support)
// bits 7-0    (8)  (Range 0-255)   Module version (e.g. Sensors, Drivers)

#define FIRMWARE_VERSION_TYPE_MINIMAL     FIRMWARE_VERSION_TYPE_DEVELOPING_ID
#define FIRMWARE_VERSION_MAJOR_MINIMAL    0
#define FIRMWARE_VERSION_MINOR_MINIMAL    105
#define FIRMWARE_VERSION_CORE_MINIMAL     0
#define FIRMWARE_VERSION_MODULE_MINIMAL   1

// Generate Version AFTER Hardware defaults have been called, this allows undef VERSION_TYPE for overriding
const uint32_t PROJECT_VERSION = ((FIRMWARE_VERSION_TYPE & 0x03) << 30) | ((FIRMWARE_VERSION_MAJOR & 0x3F) << 24) | (FIRMWARE_VERSION_MINOR << 16) | (FIRMWARE_VERSION_CORE << 8) | (FIRMWARE_VERSION_MODULE);
const uint32_t PROJECT_VERSION_MINIMAL = ((FIRMWARE_VERSION_TYPE_MINIMAL & 0x03) << 30) | ((FIRMWARE_VERSION_MAJOR_MINIMAL & 0x3F) << 24) | (FIRMWARE_VERSION_MINOR_MINIMAL << 16) | (FIRMWARE_VERSION_CORE_MINIMAL << 8) | (FIRMWARE_VERSION_MODULE_MINIMAL);

//--------------------------------[AUTOSET - DEFINE HARDWARE needed to run tasks requested]-------------------------------------

#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
#endif

// #ifdef ENABLE_TRACING
//   #define ARDUINOTRACE_ENABLE_FULLPATH
//   #define ARDUINOTRACE_ENABLE
//   #include <ArduinoTrace.h>
// #endif


#define PROJECT_NAME_CTR "Home Automation Control System (TBD)"


#ifdef DEVICE_FORCED_TO_BE_TESTER
  #warning "DEVICE_FORCED_TO_BE_TESTER is set!"
#endif



#endif


