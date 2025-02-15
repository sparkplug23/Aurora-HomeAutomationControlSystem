#pragma once

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"


enum TASKER_FUNCTION_TYPES 
{
 
 /************************************************************************************************************************************************
  * ************************************************************************************************************************************************
  * ************************************************************************************************************************************************
  * @brief Start of setup tasks used during the "setup()" only, no main "loop()" tasks
  * ************************************************************************************************************************************************
  * ************************************************************************************************************************************************
  * *************************************************************************************************************************************************/ 

  /**
   * module load should only be things that can happen before the init phase, 
   * or, I need to call this twice, so some functions required before init and after init are handled (yes, do this)
   * */
  TASK_TEMPLATES__LOAD_MODULE,  // Read progmem configs if needed, read settings configuration
  TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM,  // Read progmem configs if needed, read settings configuration

  TASK_TEMPLATE_MODULE_LOAD_AFTER_INIT_DEFAULT_CONFIG_ID, // progmem is only loaded when file system is reset


  TASK_SETTINGS_DEFAULT,   // Use defaults in code
  TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT,   // Use defaults in code
  TASK_POINTER_INIT, // Any pointers to buffers, anything not set in this (unset nullptr) must be protected 
  
  
//   TASK_MODULE_INIT,  // Read progmem configs if needed, read settings configuration
// "TASK_MODULE_INIT" is phasing out, as its going to be included within PRE_INIT, which is furthermore going to be INIT_PINS

// "PRE_INIT" should be changed to PIN_INIT, and "MODULE_POINTER" should be possibly removed as its either PIN_INIT or INIT
  
  /**
   * @brief "PRE_INIT" as TASK_INIT_GPIO_POINTERS?
   * 
   */
  TASK_PRE_INIT,     // Configure sub modules and classes as needed, should this be renamed to "INIT_PINS"?
  
  
  TASK_INIT,         // Actually complete init, read sensors, enable modules fully etc

  TASK_INIT_DELAYED_SECONDS, // To enable modules to either intentional begin with a delayed start, or, to changed at an interval if it is possible to start again (e.g. new sensor connected)

  /**
   * Flags that are used in debugging, that will override any stored or default init states. This will be optionally called at the end of setup.
   * */
  TASK_OVERRIDE_BOOT_INIT,

  // This will run AFTER json templates
  TASK_POST_INIT,    // similar to TASK_CONFIGURE_MODULES_FOR_DEVICE, will run immediately after init

  /**
   * Eg. parse_jsonCommand from template sets energy (pzem) to have X sensors during boot. Prior to leaving setup, this will be called to refresh these buffers based on the latest dynamic buffers needed
   * */
  TASK_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID,

  // Boot filesystem
  TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM,
  

  TASK_FUNCTION_LAMBDA_INIT,
  TASK_SETTINGS_LOAD_VALUES_INTO_MODULE, // Load values from settings struct and overwrite module values
  TASK_CONFIGURE_MODULES_FOR_DEVICE, // Configure any sensors that are needed for controllers, to the required settings
  TASK_MQTT_HANDLERS_INIT,
  // TASK_MQTT_INIT,
  
  TASK_TEMPLATE_DEVICE_EXECUTE_LOAD, // This is called from the above function, used to parse the object json
  
  
  /**
   * Called when we reach the end of "setup()" before loop starts
   **/
  TASK_ON_BOOT_COMPLETE, //also used as boot percentage progress divisor : Reaches end of setup
  
 /************************************************************************************************************************************************
  * ************************************************************************************************************************************************
  * ************************************************************************************************************************************************
  * @brief Start of main loop tasks
  * ************************************************************************************************************************************************
  * ************************************************************************************************************************************************
  * *************************************************************************************************************************************************/
  // System/Status message
  TASK_BOOT_MESSAGE, //at 10 seconds, show how the function is configured
  TASK_ON_BOOT_SUCCESSFUL, //should be triggered once after device is considered stable

  /**
   * @brief Mostly for developing code, these will be called after a successful boot when the device is stable, and will only then enable that module to run
   * 
   */
  TASK_PRE_INIT_DELAYED,     // Configure sub modules and classes as needed, should this be renamed to "INIT_PINS"?
  TASK_INIT_DELAYED,         // Actually complete init, read sensors, enable modules fully etc
  TASK_MQTT_HANDLERS_INIT_DELAYED,


  // Configure sensors and drivers for device
  // Looping trigger times
  TASK_LOOP, TASK_EVERY_50_MSECOND, TASK_EVERY_100_MSECOND, TASK_EVERY_250_MSECOND, 
  TASK_EVERY_SECOND, TASK_EVERY_SECOND_TP0MS_WINDOW, TASK_EVERY_SECOND_TP250MS_WINDOW, TASK_EVERY_SECOND_TP500MS_WINDOW, TASK_EVERY_SECOND_TP750MS_WINDOW, // All run only once a second, but with delays to spread out tasks which occur at that time cycle
  TASK_EVERY_FIVE_SECOND, //Used mainly as debugging
  TASK_EVERY_FIVE_MINUTE,
  TASK_EVERY_MINUTE, TASK_EVERY_HOUR, TASK_EVERY_MIDNIGHT, TASK_EVERY_MIDDAY,
  // Run once from restart/uptime
  TASK_UPTIME_10_SECONDS, //TASK_ON_BOOT_SUCCESSFUL same time slot
  TASK_UPTIME_30_SECONDS,
  TASK_UPTIME_1_MINUTES,
  TASK_UPTIME_10_MINUTES,
  TASK_UPTIME_60_MINUTES,
  // Exact minutes to work aka time cron
  // TASK_AT_TIME_1_MINUTE,
  // Welcome message to show on boot after X seconds with config
  // TASK_RESTART_SPLASH_INFORMATION, //have flag that shows level of information AND make this an ifdef as a debug option

  // New list of support items to splash debug info
  TASK_LOG__SHOW_UPTIME,

  // New method that based on time, will only show the sensor at this period to "AddLog" reducing large serial prints
  TASK_SENSOR_SHOW_LATEST_LOGGED_ID,

  TASK_UPDATE_OTA_BEFORE_ON_START,

  /**
   * @brief Special TASK_X that should be used in testing, and will run with delayed start to enable OTA to be possible when boot loop would occur
   **/
  TASK_DELAYED_START_LOOP,
  
  // TASK_PREP_BEFORE_TELEPERIOD, 
  TASK_JSON_APPEND, 
  
  // Saving/EEPROM related
  TASK_SAVE_BEFORE_RESTART, //phase out

  // Added Sep2023. This should be called prior to a restart when the system is healthy. For example, Nextion Display updating should show messages that the system is restarting
  TASK_RESTART_STABLE, 
  
  /**
   * Transfer any local variables into settings struct for saving
   * */
  TASK_SETTINGS_SAVE_VALUES_FROM_MODULE, // Save internal module values to global settings struct, prior to saving to memory

  // New call functio similar to mqtt method, called from class loop (checked in support)
  TASK_FUNCTION_LAMBDA_LOOP,


  TASK_JSON_COMMAND_CHECK_TOPIC_ID,
  TASK_JSON_COMMAND_ID,  //ie check for the topic (in the future use module name are set/<moduclename>)
  /**
   * @brief This assumes JBI is configured, and assuming the buffer is valid, will continue to append information related to scanning of sensors.
   * Example: DS18XX will scan both busses and search for addresses of connected devices. Note this also may require the init process (or force it) to run again (or preinit?) so anything that truly must happen once should be in its own section 
   * 
   */
  TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID,
  // TASK_JSON_COMMAND_START_PARSER_ID,
  // Wifi 
  TASK_WIFI_STARTING_CONNECTION,
  TASK_WIFI_CONNECTED, TASK_WIFI_DISCONNECTED,
  // Mqtt
  TASK_MQTT_SUBSCRIBE, TASK_MQTT_CONNECTED, TASK_MQTT_DISCONNECTED, TASK_MQTT_COMMAND, TASK_MQTT_SENDER,
  TASK_MQTT_STATUS_REFRESH_SEND_ALL,
  //TASK_MQTT_HANDLERS_RESET, 
  TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD,
  TASK_MQTT_CHECK_REDUNCTION_LEVEL, 
  TASK_MQTT_DATA_ID,

  /**
   * @brief Response Tasks
   * 
   * Remove these
   **/
  TASK_APPEND_RESPONSE_JSON_DRIVERS_STATUS_ID,


  
  // TASK_FILESYSTEM_APPEND__CONFIG_MODULES__ID,
  TASK_FILESYSTEM_APPEND__Stored_Secure__ID,
  TASK_FILESYSTEM_APPEND__CONFIG_SETTINGS__ID, // Temporary, will be replaced with flash settings

  TASK_FILESYSTEM__RESET__MODULE_DATA__ID,
  TASK_FILESYSTEM__SAVE__MODULE_DATA__ID,   // Each module is asked to save its data -- this should be starggered to avoid blocking
  TASK_FILESYSTEM__LOAD__MODULE_DATA__ID,   // Each module is asked to load its data on restore at boot


  TASK_TIME_SYNCED,

  /**
   * @brief Rules
   **/
  TASK_RULES_LOAD_FROM_PROGMEM_ID,
  TASK_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID,


  /**
   * @brief NNew set of alert messages that can be created depending on the type of driver
   * e.g., via STATUS_LED, STATUS_BEEPER, STATUS_DISPLAY
   */
  TASK_STATUS_ALERT__NETWORK_CONNECTION_LOST,



  // Events (new internal triggers, if something happens trigger another... this will become rules)
  // Ie if this event below happens, then have another ID which is called to run the triggered event
  TASK_EVENT_MOTION_STARTED_ID,
  TASK_EVENT_MOTION_ENDED_ID,

  // Energy
  TASK_ENERGY_RESET,
  // Calls to run drivers when sensors change rather than waiting on loop
  TASK_SENSOR_UPDATED,
  //
  TASK_STATUS_MESSAGE_APPEND, TASK_JSON_HARDWARE_APPEND, // Messages
  TASK_SET_CHANNELS, 
  /**
   * @brief Webpage
   * 
   */
  TASK_WEB_ADD_MAIN_BUTTON, 
  TASK_WEB_ADD_BUTTON, 
  TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS, //system settings 
  TASK_WEB_ADD_HANDLER, //root page
  TASK_WEB_ROOT_SEND_STYLE, // Send any style for root page
  TASK_WEB_ROOT_SEND_SCRIPT,  
  TASK_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING, 
  TASK_WEB_ROOT_SEND_BODY,   // send once on load 
  TASK_WEB_ROOT_SEND_STATUS,     //the tas way
  TASK_WEB_ADD_ROOT_SHOWS,
  TASK_WEB_ADD_FETCH_RESULT, // As part of fetch
  TASK_WEB_ADD_DATA_FETCH_URL, // Adding urls that will be requested for as part of jsonparse
  TASK_WEB_ADD_ROOT_TABLE_ROWS,
  TASK_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER,
  TASK_WEB_ADD_ROOT_SCRIPT,
  TASK_WEB_ADD_ROOT_STYLE,  
  TASK_WEB_APPEND_RUNTIME_ROOT_URLS,  
  TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED, //send table data
  TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED,
  TASK_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS,
  TASK_WEB_APPEND_ROOT_BUTTONS,
  TASK_WEB_PAGEINFORMATION_SEND_MODULE, //send parts of page info
  // parital sub functions that need to be address later by special "interface tasker"
  TASK_WEB_RESPOND_LIGHT_LIVEVIEW_JSON, //reply with liveview json data "WEB_RESPOND"=func group, "LIGHT"=interface, LIVEVIEW_JSON=type
  TASK_WEB_COMMAND,   
  TASK_WEB_SYSTEM_INFO,  //WEBSERVER

  /**
   * @brief Debugging
   * 
   */
  TASK_DEBUG_CONFIGURE,

/**
 * @note Display driver interface (I could check if within range, it direct goes to that interface sub tasks)
 * */
  TASK_DISPLAY_INIT_DRIVER, TASK_DISPLAY_INIT, TASK_DISPLAY_EVERY_50_MSECOND, 
  //TASK_DISPLAY_EVERY_SECOND,
  TASK_DISPLAY_REFRESH_SHOW_ID,
  TASK_DISPLAY_MODEL, TASK_DISPLAY_MODE, TASK_DISPLAY_POWER,
  TASK_DISPLAY_CLEAR, TASK_DISPLAY_DRAW_FRAME,
  TASK_DISPLAY_DRAW_HLINE, TASK_DISPLAY_DRAW_VLINE, TASK_DISPLAY_DRAW_LINE,
  TASK_DISPLAY_DRAW_CIRCLE, TASK_DISPLAY_FILL_CIRCLE,
  TASK_DISPLAY_DRAW_RECTANGLE, TASK_DISPLAY_FILL_RECTANGLE,
  TASK_DISPLAY_TEXT_SIZE, TASK_DISPLAY_FONT_SIZE, TASK_DISPLAY_ROTATION, TASK_DISPLAY_DRAW_STRING,
  TASK_DISPLAY_DIM, TASK_DISPLAY_BLINKRATE,
#ifdef USE_UFILESYS
  TASK_DISPLAY_BATCH,
#endif
  TASK_DISPLAY_NUMBER, TASK_DISPLAY_FLOAT, TASK_DISPLAY_NUMBERNC, TASK_DISPLAY_FLOATNC,
  TASK_DISPLAY_RAW, TASK_DISPLAY_LEVEL, TASK_DISPLAY_SEVENSEG_TEXT, TASK_DISPLAY_SEVENSEG_TEXTNC,
  TASK_DISPLAY_SCROLLDELAY, TASK_DISPLAY_CLOCK, TASK_DISPLAY_SCROLLTEXT,

  /**
   * Group of "Events" functions, which are called when sections of the code execute
   * */
  TASK_EVENT_INPUT_STATE_CHANGED_ID,
  TASK_EVENT_SET_POWER_ID,
  TASK_EVENT_SET_SPEED_ID,

  
  
  TASK_SET_POWER, TASK_SET_DEVICE_POWER, TASK_SHOW_SENSOR,
  TASK_RULES_PROCESS, TASK_SERIAL, TASK_FREE_MEM, 
  
  TASK_BUTTON_PRESSED,
  TASK_BUTTON_MULTI_PRESSED,
  TASK_BUTTON_HOLD,

  TASK_SET_POWER_ON_ID,
  TASK_SET_POWER_OFF_ID,

  TASK_LIGHT_POWER_ON,
  TASK_LIGHT_POWER_OFF,
  TASK_LIGHT_UPDATE_OUTPUT,


  TASK_CELLULAR_CONNECTION_ESTABLISHED, 
  TASK_CELLULAR_CONNECTION_LOST,
  TASK_NETWORK_CONNECTION_ESTABLISHED, // Generalised for any connection type (wifi, cellular, ethernet)
  TASK_NETWORK_CONNECTION_LOST,


  TASK_FINALLY_END_OF_LOOP, // Ran at the end of each loop, used to reset flags that should have been handled eg motion

  TASK_LENGTH
};

#ifdef ENABLE_DEBUG_FUNCTION_NAMES

DEFINE_PGM_CTR(PM_TASK_POINTER_INIT_CTR)                            D_TASK_POINTER_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_TEMPLATE_LOAD_CTR)                           D_TASK_TEMPLATE_LOAD_CTR;
// DEFINE_PGM_CTR(PM_TASK_MODULE_INIT_CTR)                             D_TASK_MODULE_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_PRE_INIT_CTR)                                D_TASK_PRE_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_INIT_CTR)                                    D_TASK_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_CONFIGURE_MODULES_FOR_DEVICE_CTR)            D_TASK_CONFIGURE_MODULES_FOR_DEVICE_CTR;
DEFINE_PGM_CTR(PM_TASK_LOOP_CTR)                                    D_TASK_LOOP_CTR;
DEFINE_PGM_CTR(PM_TASK_EVERY_50_MSECOND_CTR)                        D_TASK_EVERY_50_MSECOND_CTR;
DEFINE_PGM_CTR(PM_TASK_EVERY_100_MSECOND_CTR)                       D_TASK_EVERY_100_MSECOND_CTR;
DEFINE_PGM_CTR(PM_TASK_EVERY_250_MSECOND_CTR)                       D_TASK_EVERY_250_MSECOND_CTR;
DEFINE_PGM_CTR(PM_TASK_EVERY_SECOND_CTR)                            D_TASK_EVERY_SECOND_CTR;
DEFINE_PGM_CTR(PM_TASK_EVERY_MINUTE_CTR)                            D_TASK_EVERY_MINUTE_CTR; 
DEFINE_PGM_CTR(PM_TASK_EVERY_HOUR_CTR)                              D_TASK_EVERY_HOUR_CTR; 
DEFINE_PGM_CTR(PM_TASK_EVERY_MIDNIGHT_CTR)                          D_TASK_EVERY_MIDNIGHT_CTR;
DEFINE_PGM_CTR(PM_TASK_EVERY_MIDDAY_CTR)                            D_TASK_EVERY_MIDDAY_CTR;
DEFINE_PGM_CTR(PM_TASK_ON_SUCCESSFUL_BOOT_CTR)                      D_TASK_ON_SUCCESSFUL_BOOT_CTR;
DEFINE_PGM_CTR(PM_TASK_UPTIME_10_SECONDS_CTR)                       D_TASK_UPTIME_10_SECONDS_CTR;
DEFINE_PGM_CTR(PM_TASK_UPTIME_1_MINUTES_CTR)                        D_TASK_UPTIME_1_MINUTES_CTR;
DEFINE_PGM_CTR(PM_TASK_UPTIME_10_MINUTES_CTR)                       D_TASK_UPTIME_10_MINUTES_CTR;
DEFINE_PGM_CTR(PM_TASK_UPTIME_60_MINUTES_CTR)                       D_TASK_UPTIME_60_MINUTES_CTR;
DEFINE_PGM_CTR(PM_TASK_RESTART_SPLASH_INFORMATION_CTR)              D_TASK_RESTART_SPLASH_INFORMATION_CTR;
DEFINE_PGM_CTR(PM_TASK_PREP_BEFORE_TELEPERIOD_CTR)                  D_TASK_PREP_BEFORE_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_TASK_JSON_APPEND_CTR)                             D_TASK_JSON_APPEND_CTR;
DEFINE_PGM_CTR(PM_TASK_SAVE_BEFORE_RESTART_CTR)                     D_TASK_SAVE_BEFORE_RESTART_CTR;
DEFINE_PGM_CTR(PM_TASK_SETTINGS_DEFAULT_CTR)                        D_TASK_SETTINGS_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR)     D_TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_TASK_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR)        D_TASK_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
DEFINE_PGM_CTR(PM_TASK_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR)        D_TASK_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
DEFINE_PGM_CTR(PM_TASK_FUNCTION_LAMBDA_INIT_CTR)                    D_TASK_FUNCTION_LAMBDA_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_FUNCTION_LAMBDA_LOOP_CTR)                    D_TASK_FUNCTION_LAMBDA_LOOP_CTR;
DEFINE_PGM_CTR(PM_TASK_COMMAND_CTR)                                 D_TASK_COMMAND_CTR;
DEFINE_PGM_CTR(PM_TASK_COMMAND_SENSOR_CTR)                          D_TASK_COMMAND_SENSOR_CTR;
DEFINE_PGM_CTR(PM_TASK_COMMAND_DRIVER_CTR)                          D_TASK_COMMAND_DRIVER_CTR;
DEFINE_PGM_CTR(PM_TASK_JSON_COMMAND_CTR)                            D_TASK_JSON_COMMAND_CTR;
DEFINE_PGM_CTR(PM_TASK_JSON_COMMAND_OBJECT_CTR)                     D_TASK_JSON_COMMAND_OBJECT_CTR;
DEFINE_PGM_CTR(PM_TASK_WIFI_CONNECTED_CTR)                          D_TASK_WIFI_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_TASK_WIFI_DISCONNECTED_CTR)                       D_TASK_WIFI_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_SUBSCRIBE_CTR)                          D_TASK_MQTT_SUBSCRIBE_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_INIT_CTR)                               D_TASK_MQTT_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_CONNECTED_CTR)                          D_TASK_MQTT_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_DISCONNECTED_CTR)                       D_TASK_MQTT_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_COMMAND_CTR)                            D_TASK_MQTT_COMMAND_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_SENDER_CTR)                             D_TASK_MQTT_SENDER_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_HANDLERS_RESET_CTR)                     D_TASK_MQTT_HANDLERS_RESET_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_HANDLERS_INIT_CTR)                      D_TASK_MQTT_HANDLERS_INIT_CTR;
DEFINE_PGM_CTR(PM_TASK_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR)        D_TASK_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_TASK_SET_POWER_CTR)                               D_TASK_SET_POWER_CTR;
DEFINE_PGM_CTR(PM_TASK_SET_DEVICE_POWER_CTR)                        D_TASK_SET_DEVICE_POWER_CTR;
DEFINE_PGM_CTR(PM_TASK_SHOW_SENSOR_CTR)                             D_TASK_SHOW_SENSOR_CTR;
DEFINE_PGM_CTR(PM_TASK_RULES_PROCESS_CTR)                           D_TASK_RULES_PROCESS_CTR;
DEFINE_PGM_CTR(PM_TASK_SERIAL_CTR)                                  D_TASK_SERIAL_CTR;
DEFINE_PGM_CTR(PM_TASK_FREE_MEM_CTR)                                D_TASK_FREE_MEM_CTR;
DEFINE_PGM_CTR(PM_TASK_BUTTON_PRESSED_CTR)                          D_TASK_BUTTON_PRESSED_CTR;
DEFINE_PGM_CTR(PM_TASK_ENERGY_RESET_CTR)                            D_TASK_ENERGY_RESET_CTR;
DEFINE_PGM_CTR(PM_TASK_SENSOR_UPDATED_CTR)                          D_TASK_SENSOR_UPDATED_CTR;
DEFINE_PGM_CTR(PM_TASK_STATUS_MESSAGE_APPEND_CTR)                   D_TASK_STATUS_MESSAGE_APPEND_CTR;
DEFINE_PGM_CTR(PM_TASK_JSON_HARDWARE_APPEND_CTR)                    D_TASK_JSON_HARDWARE_APPEND_CTR;
DEFINE_PGM_CTR(PM_TASK_SET_CHANNELS_CTR)                            D_TASK_SET_CHANNELS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_MAIN_BUTTON_CTR)                     D_TASK_WEB_ADD_MAIN_BUTTON_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_BUTTON_CTR)                          D_TASK_WEB_ADD_BUTTON_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR)          D_TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_HANDLER_CTR)                         D_TASK_WEB_ADD_HANDLER_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ROOT_SEND_STYLE_CTR)                     D_TASK_WEB_ROOT_SEND_STYLE_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ROOT_SEND_SCRIPT_CTR)                    D_TASK_WEB_ROOT_SEND_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR) D_TASK_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ROOT_SEND_BODY_CTR)                      D_TASK_WEB_ROOT_SEND_BODY_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ROOT_SEND_STATUS_CTR)                    D_TASK_WEB_ROOT_SEND_STATUS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_ROOT_SHOWS_CTR)                      D_TASK_WEB_ADD_ROOT_SHOWS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_FETCH_RESULT_CTR)               D_TASK_WEB_ADD_FETCH_RESULT_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_DATA_FETCH_URL_CTR)             D_TASK_WEB_ADD_DATA_FETCH_URL_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_ROOT_TABLE_ROWS_CTR)                 D_TASK_WEB_ADD_ROOT_TABLE_ROWS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_ROOT_SCRIPT_CTR)                     D_TASK_WEB_ADD_ROOT_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_ADD_ROOT_STYLE_CTR)                      D_TASK_WEB_ADD_ROOT_STYLE_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_LOADTIME_ROOT_URLS_CTR)           D_TASK_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_LOADTIME_ROOT_RATES_CTR)          D_TASK_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_RUNTIME_ROOT_URLS_CTR)            D_TASK_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_RUNTIME_ROOT_RATES_CTR)           D_TASK_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR)  D_TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR)     D_TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR)        D_TASK_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_APPEND_ROOT_BUTTONS_CTR)                 D_TASK_WEB_APPEND_ROOT_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_PAGEINFORMATION_SEND_MODULE_CTR)         D_TASK_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_COMMAND_CTR)                             D_TASK_WEB_COMMAND_CTR;
// DEFINE_PGM_CTR(PM_TASK_CHECK_POINTERS_CTR)                          D_TASK_CHECK_POINTERS_CTR;
DEFINE_PGM_CTR(PM_TASK_WEB_SYSTEM_INFO_CTR)                         D_TASK_WEB_SYSTEM_INFO_CTR;
DEFINE_PGM_CTR(PM_TASK_DEBUG_CONFIGURE_CTR)                         D_TASK_DEBUG_CONFIGURE_CTR;

#endif // ENABLE_DEBUG_FUNCTION_NAMES

DEFINE_PGM_CTR(PM_TASK_EVENT_MOTION_STARTED_CTR)  D_TASK_EVENT_MOTION_STARTED_CTR;
DEFINE_PGM_CTR(PM_TASK_EVENT_MOTION_ENDED_CTR)  D_TASK_EVENT_MOTION_ENDED_CTR;

DEFINE_PGM_CTR(PM_TASK_EVENT_INPUT_STATE_CHANGED_CTR)   D_TASK_EVENT_INPUT_STATE_CHANGED_CTR;
DEFINE_PGM_CTR(PM_TASK_EVENT_SET_POWER_CTR)   D_TASK_EVENT_SET_POWER_CTR;
DEFINE_PGM_CTR(PM_TASK_EVENT_SET_SPEED_CTR)   D_TASK_EVENT_SET_SPEED_CTR;
