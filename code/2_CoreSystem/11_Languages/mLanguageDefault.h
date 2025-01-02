/*
  mLanguageDefault.h - Base language defines

  Copyright (C) 2020 my_name_here

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef mLanguageDefault_H
#define mLanguageDefault_H

#include "2_CoreSystem/mGlobalMacros.h"


#define D_UNIQUE_MODULE_CORE_LANGUAGES_ID 2011 // [(Folder_Number*100)+ID_File]


#define STR_HELPER2(x)  #x
#ifndef STR2
#define STR2(x)  STR_HELPER2(x)
#endif

#ifdef ENABLE_LANGUAGE_DEFAULT_EXTENDED
#include "2_CoreSystem/11_Languages/mLanguageDefault_Extended.h"
#endif

#define D_MODULE_CORE_HARDWAREPINS_CTR            "hardwarepins"
#define D_MODULE_CORE_SERIAL_UART_CTR             "serialuart"
#define D_MODULE_CORE_SETTINGS_CTR                "settings"
#define D_MODULE_CORE_SUPPORT_CTR                 "support"
#define D_MODULE_CORE_LOGGING_CTR                 "logging"
#define D_MODULE_CORE_TELEMETRY_CTR               "telemetry"
#define D_MODULE_CORE_TIME_CTR                    "time"
#define D_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR   "debug_development"
#define D_MODULE_CORE_RULES_CTR                   "rules"
#define D_MODULE_CORE_UPDATES_CTR                 "update"
#define D_MODULE_SENSORS_SUN_TRACKING_CTR          "sun_track"
#define D_MODULE_SENSORS_MOON_TRACKING_CTR          "moon_track"
#define D_MODULE__NETWORK_INTERFACE__CTR          "interface_network"
#define D_MODULE_NETWORK_WIFI_CTR                 "wifi"
#define D_MODULE_NETWORK_MQTT_CTR                 "system"
#define D_MODULE_NETWORK_MQTT_MULTIPLE_CTR        "system"
#define D_MODULE_NETWORK_MQTT_CELLULAR_CTR        "mqtt_cellular"
#define D_MODULE__NETWORK_CELLULAR__CTR           "cellular"
#define D_MODULE_CORE__SERIAL__CTR                "Serial"
#define D_MODULE_NETWORK_WEBSERVER_CTR            "webserver"
#define D_MODULE_DISPLAYS_INTERFACE_CTR           "interface_displays"
#define D_MODULE_DISPLAYS_NEXTION_CTR             "nextion"
#define D_MODULE_DISPLAYS_OLED_SSD1306_CTR        "display_1306"
#define D_MODULE_DISPLAYS_OLED_SH1106_CTR         "display_sh1106"
#define D_MODULE_DRIVERS_INTERFACE_CTR            "interface_drivers"
#define D_MODULE_DRIVERS_HBRIDGE_CTR              "hbridge"
#define D_MODULE_DRIVERS_IRTRANSCEIVER_CTR        "ir"
#define D_MODULE_DRIVERS_RELAY_CTR                "relays"
#define D_MODULE_DRIVERS_PWM_CTR                  "pwm"
#define D_MODULE_DRIVERS_SDCARD_CTR               "sdcard"
#define D_MODULE_SENSORS__GPS_SERIAL__CTR         "gps"
#define D_MODULE_DRIVERS_SHELLY_DIMMER_CTR        "shellydimmer"
#define D_MODULE_DRIVERS_CAMERA_OV2640_CTR        "camera"
#define D_MODULE_DRIVERS_CAMERA_WEBCAM_CTR        "camera"
#define D_MODULE_DRIVERS_STATUS_LEDS_CTR          "statusleds"
#define D_MODULE_CORE_FILESYSTEM_CTR              "filesystem"
#define D_MODULE_CORE__TEMPLATE_LOADING__CTR      "template_loading"
#define D_MODULE_CORE__I2C__CTR                   "i2c"
#define D_MODULE_CORE__SPI__CTR                   "spi"
#define D_MODULE_DRIVERS_BUZZER_CTR               "buzzer"
#define D_MODULE__DRIVERS_BUZZER_TONES__CTR       "buzzer"
#define D_MODULE_DRIVERS_RF433_RCSWITCH_CTR       "rcswitch"
#define D_MODULE_DRIVERS_LEDS_CTR                 "leds"
#define D_MODULE_DRIVERS_IRREMOTE_CTR             "irremote"
#define D_MODULE_DRIVERS__CAMERA_ARDUINO__CTR     "camera_arduino"
#define D_MODULE_DRIVERS__CAMERA_TASMOTA__CTR     "camera_tas"
#define D_MODULE_DRIVERS__CAMERA_MULTICLIENT__CTR "camera_multiclient"
#define D_MODULE_DRIVERS__TINYGSM__CTR            "tinygsm"
#define D_MODULE__DRIVERS_MAVLINK_DECODER__CTR    "mavlink"
#define D_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__CTR "mavlink_telemetry"
#define D_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR__CTR "mavlink_telemetry"
#define D_MODULE_ENERGY_INTERFACE_CTR             "interface_energy"
#define D_MODULE_ENERGY_ADE7953_CTR               "ade7953"
#define D_MODULE_ENERGY_INA219_CTR                "ina219"
#define D_MODULE_LIGHTS_INTERFACE_CTR             "interface_lights"
#define D_MODULE_LIGHTS_ADDRESSABLE_CTR           "addressable"
#define D_MODULE_LIGHTS_ANIMATOR_CTR              "pixels"
#define D_MODULE_LIGHTS_PWM_CTR                   "pwmlight"
#define D_MODULE_LIGHTS_WLED_EFFECTS_CTR          "wled_effects"
#define D_MODULE_SENSORS_INTERFACE_CTR            "interface_sensor"
#define D_MODULE_SENSORS_BUTTONS_CTR              "buttons"
#define D_MODULE_SENSORS_SWITCHES_CTR             "switches"
#define D_MODULE_SENSORS_LDR_BASIC_CTR            "ldr_basic"
#define D_MODULE_SENSORS_ANALOG_CTR               "analog"
#define D_MODULE_SENSORS_DOORBELL_CTR             "doorbell"
#define D_MODULE_ENERGY_PZEM004T_CTR              "pzem004t"
#define D_MODULE_SENSORS_DHT_CTR                  "dht"
#define D_MODULE_SENSORS_BME_CTR                  "bme"
#define D_MODULE_SENSORS_DB18S20_CTR              "db18s20"
#define D_MODULE_SENSORS_INA219_CTR               "ina219"
#define D_MODULE_SENSORS_ULTRASONIC_CTR           "ultrasonic"
#define D_MODULE_SENSORS_DOOR_CTR                 "doorsensor"
#define D_MODULE_SENSORS_PIR_CTR                  "motion" // to be renamed pir, but only after openhab responds to inteface motion events instead
#define D_MODULE_SENSORS_LSM303D_CTR              "lsm303d"
#define D_MODULE_SENSORS_L3G_CTR                  "l3g"
#define D_MODULE_SENSORS_MPU9250_CTR              "mpu9250"
#define D_MODULE__SENSORS_BATTERY_MODEM__CTR      "battery_modem"
#define D_MODULE_SENSORS_RESISTIVE_MOISTURE_CTR   "moisture"
#define D_MODULE_SENSORS_ADC_INTERNAL_CTR         "adc_internal"
#define D_MODULE_SENSORS_ADC_I2S_INTERNAL_CTR     "adc_i2s"
#define D_MODULE_SENSORS_PULSECOUNTER_CTR         "pulsecounter"
#define D_MODULE_SENSORS_BH1750_CTR               "bh1750"
#define D_MODULE_SENSORS_SR04_CTR                 "sr04"
#define D_MODULE_SENSORS_REMOTE_DEVICE_CTR        "remotedevice"
#define D_MODULE_SENSORS_ROTARY_ENCODER_CTR       "rotary_encoder"
#define D_MODULE_SENSORS__DS18X20_ESP32_2023__CTR "db18_esp32_2023"
#define D_MODULE_SENSORS__DS18X20_ESP8266_2023__CTR "db18_esp82_2023"
#define D_MODULE__SENSORS_GPS_MODEM__CTR          "gps_modem"
#define D_MODULE_CONTROLLER_BLINDS_CTR            "blinds"
#define D_MODULE_CONTROLLER_HVAC_CTR              "hvac"
#define D_MODULE_CONTROLLER_RADIATORFAN_CTR       "radiatorfan"
#define D_MODULE_CONTROLLER_CEILINGFAN_CTR        "ceilingfan"
#define D_MODULE_CONTROLLER_IRTRANSMITTER_CTR     "infrared"
#define D_MODULE_CONTROLLER_TANKVOLUME_CTR        "tankvolume"
#define D_MODULE_CONTROLLER_FAN_PWM_CTR           "fan"
#define D_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_CTR "hvac_strip_colour"
#define D_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_CTR "sensor_colours"
#define D_MODULE_CONTROLLER_TREADMILL_CTR         "treadmill"
#define D_MODULE_CONTROLLER_DOORBELL_CTR          "doorbell"
#define D_MODULE_CONTROLLER_SDCARDLOGGER_CTR      "sdcardlogger"
#define D_MODULE_CONTROLLER_GPS_SD_LOGGER_CTR     "sdcardlogger"
#define D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR "serial_positional_logger"
#define D_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_CTR "serial_calib_pic32"
#define D_MODULE_CONTROLLER_USERMOD_01_CTR        "usermod_01"
#define D_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_CTR "imu_radiation_pattern"
#define D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_CTR "bucket_water_level"
#define D_MODULE_CONTROLLER_FURNACE_SENSOR_CTR    "furnace_sensor"
#define D_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR     "louvolite"
#define D_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_CTR "sidedoor_lights"
#define D_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL_CTR "immersion_panel"
#define D_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_CTR "blackbox"
#define D_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED_CTR "controller_mavlink_oled"
#define D_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_TESTER__CTR "webui_wled_tester"
#define D_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__CTR "webui_wled_dev"
#define D_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE_CTR "printer_enclosure"
#define D_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED_CTR "oled_show_power"
#define D_MODULE_CONTROLLER__ENERGY_OLED_CTR      "energy_oled"
#define D_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_CTR "treadmill_logger"
#define D_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_CTR "sensors_oled"
#define D_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__CTR "sensor_colourbar"
#define D_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__CTR "relay_state_ledstrip"
#define D_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS_CTR "mavlink_flying_leds"
#define D_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__CTR "light_motion_alerts"


/**
 * Time
 * */

// HTML (ISO 639-1  Language Code
#define D_HTML_LANGUAGE "en"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"
#define D_DECIMAL_SEPARATOR "."

#define D_DAY3LIST "MonTueWedThuFriSatSun"
#define D_MONTH3LIST "JanFebMarAprMayJunJulAugSepOctNovDec"

/**
 * TaskerInterface Functions
 * */
// #define D_TASK_MODULE_INIT_CTR                              "MODULE_INIT"
#define D_TASK_POINTER_INIT_CTR                             "POINTER_INIT"
#define D_TASK_TEMPLATE_LOAD_CTR                            "TEMPLATE_LOAD"
// #define D_TASK_MODULE_INIT_CTR                              "MODULE_INIT"
#define D_TASK_PRE_INIT_CTR                                 "PRE_INIT"
#define D_TASK_INIT_CTR                                     "INIT"
#define D_TASK_CONFIGURE_MODULES_FOR_DEVICE_CTR             "CONFIGURE_MODULES_FOR_DEVICE"
#define D_TASK_LOOP_CTR                                     "LOOP"
#define D_TASK_EVERY_50_MSECOND_CTR                         "EVERY_50_MSECOND"
#define D_TASK_EVERY_100_MSECOND_CTR                        "EVERY_100_MSECOND"
#define D_TASK_EVERY_250_MSECOND_CTR                        "EVERY_250_MSECOND"
#define D_TASK_EVERY_SECOND_CTR                             "EVERY_SECOND"
#define D_TASK_EVERY_MINUTE_CTR                             "EVERY_MINUTE" 
#define D_TASK_EVERY_HOUR_CTR                               "EVERY_HOUR" 
#define D_TASK_EVERY_MIDNIGHT_CTR                           "EVERY_MIDNIGHT"
#define D_TASK_EVERY_MIDDAY_CTR                             "EVERY_MIDDAY"
#define D_TASK_ON_SUCCESSFUL_BOOT_CTR                       "ON_SUCCESSFUL_BOOT"
#define D_TASK_UPTIME_10_SECONDS_CTR                        "UPTIME_10_SECONDS"
#define D_TASK_UPTIME_1_MINUTES_CTR                         "UPTIME_1_MINUTES"
#define D_TASK_UPTIME_10_MINUTES_CTR                        "UPTIME_10_MINUTES"
#define D_TASK_UPTIME_60_MINUTES_CTR                        "UPTIME_60_MINUTES"
#define D_TASK_RESTART_SPLASH_INFORMATION_CTR               "RESTART_SPLASH_INFORMATION"
#define D_TASK_PREP_BEFORE_TELEPERIOD_CTR                   "PREP_BEFORE_TELEPERIOD"
#define D_TASK_JSON_APPEND_CTR                              "JSON_APPEND"
#define D_TASK_SAVE_BEFORE_RESTART_CTR                      "SAVE_BEFORE_RESTART"
#define D_TASK_SETTINGS_DEFAULT_CTR                         "SETTINGS_DEFAULT"
#define D_TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR      "SETTINGS_OVERWRITE_SAVED_TO_DEFAULT"
#define D_TASK_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR         "SETTINGS_LOAD_VALUES_INTO_MODULE"
#define D_TASK_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR         "SETTINGS_SAVE_VALUES_FROM_MODULE"
#define D_TASK_FUNCTION_LAMBDA_INIT_CTR                     "FUNCTION_LAMBDA_INIT"
#define D_TASK_FUNCTION_LAMBDA_LOOP_CTR                     "FUNCTION_LAMBDA_LOOP"
#define D_TASK_COMMAND_CTR                                  "COMMAND"
#define D_TASK_COMMAND_SENSOR_CTR                           "COMMAND_SENSOR"
#define D_TASK_COMMAND_DRIVER_CTR                           "COMMAND_DRIVER"
#define D_TASK_JSON_COMMAND_CTR                             "JSON_COMMAND"
#define D_TASK_JSON_COMMAND_OBJECT_CTR                      "JSON_COMMAND_OBJECT"
#define D_TASK_WIFI_CONNECTED_CTR                           "WIFI_CONNECTED"
#define D_TASK_WIFI_DISCONNECTED_CTR                        "WIFI_DISCONNECTED"
#define D_TASK_MQTT_SUBSCRIBE_CTR                           "MQTT_SUBSCRIBE"
#define D_TASK_MQTT_INIT_CTR                                "MQTT_INIT"
#define D_TASK_MQTT_CONNECTED_CTR                           "MQTT_CONNECTED"
#define D_TASK_MQTT_DISCONNECTED_CTR                        "MQTT_DISCONNECTED"
#define D_TASK_MQTT_COMMAND_CTR                             "MQTT_COMMAND"
#define D_TASK_MQTT_SENDER_CTR                              "MQTT_SENDER"
#define D_TASK_MQTT_HANDLERS_RESET_CTR                      "MQTT_HANDLERS_RESET"
#define D_TASK_MQTT_HANDLERS_INIT_CTR                       "MQTT_HANDLERS_INIT"
#define D_TASK_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR         "MQTT_HANDLERS_REFRESH_TELEPERIOD"
#define D_TASK_SET_POWER_CTR                                "SET_POWER"
#define D_TASK_SET_DEVICE_POWER_CTR                         "SET_DEVICE_POWER"
#define D_TASK_SHOW_SENSOR_CTR                              "SHOW_SENSOR"
#define D_TASK_RULES_PROCESS_CTR                            "RULES_PROCESS"
#define D_TASK_SERIAL_CTR                                   "SERIAL"
#define D_TASK_FREE_MEM_CTR                                 "FREE_MEM"
#define D_TASK_BUTTON_PRESSED_CTR                           "BUTTON_PRESSED"
#define D_TASK_ENERGY_RESET_CTR                             "ENERGY_RESET"
#define D_TASK_SENSOR_UPDATED_CTR                           "SENSOR_UPDATED"
#define D_TASK_STATUS_MESSAGE_APPEND_CTR                    "STATUS_MESSAGE_APPEND"
#define D_TASK_JSON_HARDWARE_APPEND_CTR                     "JSON_HARDWARE_APPEND"
#define D_TASK_SET_CHANNELS_CTR                             "SET_CHANNELS"
#define D_TASK_WEB_ADD_MAIN_BUTTON_CTR                      "WEB_ADD_MAIN_BUTTON"
#define D_TASK_WEB_ADD_BUTTON_CTR                           "WEB_ADD_BUTTON"
#define D_TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR           "WEB_ADD_BUTTON_SYSTEM_SETTINGS"
#define D_TASK_WEB_ADD_HANDLER_CTR                          "WEB_ADD_HANDLER"
#define D_TASK_WEB_ROOT_SEND_STYLE_CTR                      "WEB_ROOT_SEND_STYLE"
#define D_TASK_WEB_ROOT_SEND_SCRIPT_CTR                     "WEB_ROOT_SEND_SCRIPT"
#define D_TASK_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR  "WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING"
#define D_TASK_WEB_ROOT_SEND_BODY_CTR                       "WEB_ROOT_SEND_BODY"
#define D_TASK_WEB_ROOT_SEND_STATUS_CTR                     "WEB_ROOT_SEND_STATUS"
#define D_TASK_WEB_ADD_ROOT_SHOWS_CTR                       "WEB_ADD_ROOT_SHOWS"
#define D_TASK_WEB_ADD_FETCH_RESULT_CTR                "WEB_ADD_FETCH_RESULT"
#define D_TASK_WEB_ADD_DATA_FETCH_URL_CTR              "WEB_ADD_DATA_FETCH_URL"
#define D_TASK_WEB_ADD_ROOT_TABLE_ROWS_CTR                  "WEB_ADD_ROOT_TABLE_ROWS"
#define D_TASK_WEB_ADD_ROOT_SCRIPT_CTR                      "WEB_ADD_ROOT_SCRIPT"
#define D_TASK_WEB_ADD_ROOT_STYLE_CTR                       "WEB_ADD_ROOT_STYLE"
#define D_TASK_WEB_APPEND_LOADTIME_ROOT_URLS_CTR            "WEB_APPEND_LOADTIME_ROOT_URLS"
#define D_TASK_WEB_APPEND_LOADTIME_ROOT_RATES_CTR           "WEB_APPEND_LOADTIME_ROOT_RATES"
#define D_TASK_WEB_APPEND_RUNTIME_ROOT_URLS_CTR             "WEB_APPEND_RUNTIME_ROOT_URLS"
#define D_TASK_WEB_APPEND_RUNTIME_ROOT_RATES_CTR            "WEB_APPEND_RUNTIME_ROOT_RATES"
#define D_TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR   "WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED"
#define D_TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR      "WEB_APPEND_ROOT_STATUS_TABLE_FORCED"
#define D_TASK_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR         "WEB_APPEND_ROOT_ADD_MAIN_BUTTONS"
#define D_TASK_WEB_APPEND_ROOT_BUTTONS_CTR                  "WEB_APPEND_ROOT_BUTTONS"
#define D_TASK_WEB_PAGEINFORMATION_SEND_MODULE_CTR          "WEB_PAGEINFORMATION_SEND_MODULE"
#define D_TASK_WEB_COMMAND_CTR                              "WEB_COMMAND"
// #define D_TASK_CHECK_POINTERS_CTR                           "CHECK_POINTERS"
#define D_TASK_WEB_SYSTEM_INFO_CTR                          "WEB_SYSTEM_INFO"
#define D_TASK_DEBUG_CONFIGURE_CTR                          "DEBUG_CONFIGURE"


#define D_TASK_EVENT_MOTION_STARTED_CTR "MotionStarted"
#define D_TASK_EVENT_MOTION_ENDED_CTR "MotionEnded"

#define   D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "StateChanged"
#define   D_TASK_EVENT_SET_POWER_CTR   "SetPower"
#define   D_TASK_EVENT_SET_SPEED_CTR   "SetSpeed"

/**
 * Common list - These are D_<exact letters in string> that are Capital first
 * */
#define D_ALTITUDE              "Altitude"
#define D_ADMIN                 "Admin"
#define D_AIR_QUALITY           "Air quality"
#define D_ANALOG_INPUT          "Analog"
#define D_AP                    "AP"
#define D_AS                    "As"
#define D_AUTO                  "AUTO"
#define D_BLINK                 "Blink"
#define D_BLINKOFF              "BlinkOff"
#define D_BOOT_COUNT            "Boot Count"
#define D_BRIGHTLIGHT           "Bright"
#define D_BSSID                 "BSSId"
#define D_BUTTON                "Button"
#define D_BY                    "by"                    
#define D_BYTES                 "Bytes"
#define D_CELSIUS               "Celsius"
#define D_CHANNEL               "Channel"
#define D_CO2                   "Carbon dioxide"
#define D_COLDLIGHT             "Cold"
#define D_COMMAND               "Command"
#define D_CONNECTED             "Connected"
#define D_COUNT                 "Count"
#define D_COUNTER               "Counter"
#define D_CURRENT               "Current"          
#define D_CYCLE "Cycle"
#define D_DATA                  "Data"
#define D_DARKLIGHT             "Dark"
#define D_DEBUG                 "Debug"
#define D_DEVICE                "Device"
#define D_DISABLED              "Disabled"
#define D_DESIRED               "Desired"
#define D_DISTANCE              "Distance"
#define D_DNS_SERVER            "DNS Server"
#define D_DONE                  "Done"
#define D_DURATION              "Duration"
#define D_DST_TIME              "DST"
#define D_EMULATION             "Emulation"
#define D_ENABLED               "Enabled"
#define D_ERASE                 "Erase"
#define D_ERROR                 "Error"
#define D_WARNING               "Warning"
#define D_FAHRENHEIT            "Fahrenheit"
#define D_FAILED                "Failed"
#define D_FALLBACK              "Fallback"
#define D_FALLBACK_TOPIC        "Fallback Topic"
#define D_FALSE                 "False"
#define D_FILE                  "File"
#define D_FLOW_RATE             "Flow rate"
#define D_FREE_MEMORY           "Free Memory"
#define D_FREQUENCY             "Frequency"
#define D_GAS                   "Gas"
#define D_GATEWAY               "Gateway"
#define D_GROUP                 "Group"
#define D_HOST                  "Host"
#define D_HOSTNAME              "Hostname"
#define D_HUM                   "Hum"
#define D_HUMIDITY              "Humidity"
#define D_ILLUMINANCE           "Illuminance"
#define D_IMMEDIATE             "Immediate"
#define D_IMPORTANCE            "Importance"
#define D_INDEX                 "Index"
#define D_INFO                  "Info"
#define D_INFRARED              "Infrared"
#define D_INITIALIZED           "Initialized"
#define D_IP_ADDRESS            "IP Address"
#define D_IP                    "IP"
#define D_ISVALID               "IsValid"
#define D_ISRUNNING             "IsRunning"
#define D_ISCHANGED             "IsChanged"
#define D_LIGHT                 "Light"
#define D_LIMIT                 "Limit"
#define D_LENGTH                "Length"
#define D_LIMIT                 "Limit"
#define D_DIMMER                "Dimmer"
#define D_LWT                   "LWT"
#define D_MEASURE               "Measure"
#define D_MESSAGE               "Message"
#define D_MAX                   "Max"
#define D_MIN                   "Min"
#define D_MODE                  "Mode"
#define D_STEP                  "Step"
#define D_MODULE                "Module"
#define D_MQTT                  "MQTT"
#define D_MULTI_PRESS           "multi-press"
#define D_NOISE                 "Noise"
#define D_NONE                  "None"
#define D_NOT_PRESSED           "Not Pressed"
#define D_OFF                   "Off"
#define D_OFFLINE               "Offline"
#define D_OK                    "Ok"
#define D_ON                    "On"
#define D_ONLINE                "Online"
#define D_ONTIME                "Ontime"
#define D_ONOFF                 "OnOff"
#define D_OFFTIME               "Offtime"
#define D_PASSWORD              "Password"
#define D_PARSING_NOMATCH       "Parsing NOT Matched "

// #define ENABLE_PARSING_DEBUG_LINE

#ifdef ENABLE_PARSING_DEBUG_LINE
#define D_PARSING_MATCHED       "Parsing Not Matched" String(__LINE__)
#else
#define D_PARSING_MATCHED       "Parsing Matched "
#endif
#define D_PORT                  "Port"
#define D_POWER_FACTOR          "Power Factor"
#define D_POWERUSAGE            "Power"
#define D_POWERUSAGE_ACTIVE     "Active Power"
#define D_POWERUSAGE_APPARENT   "Apparent Power"
#define D_POWERUSAGE_REACTIVE   "Reactive Power"
#define D_PRESSURE              "Pressure"
#define D_PRESSUREATSEALEVEL    "SeaPressure"
#define D_PRESSED               "Pressed"
#define D_PROGRAM_FLASH_SIZE    "Program Flash Size"
#define D_PROGRAM_SIZE          "Program Size"
#define D_PROJECT               "Project"
#define D_RAIN                  "Rain"
#define D_RGB                   "RGB"
#define D_READ                  "Read"
#define D_RELAY                 "Relay"
#define D_RECEIVED              "Received"
#define D_RESTART               "Restart"
#define D_RESTARTING            "Restarting"
#define D_RESTART_REASON        "Restart Reason"
#define D_RESTORE               "restore"
#define D_RETAINED              "retained"
#define D_RULE                  "Rule"
#define D_SAVE                  "Save"
#define D_SENSOR                "Sensor"
#define D_SET                   "Set"
#define D_SECS                  "Secs"
#define D_SIZE                  "Size"
#define D_SSID                  "SSId"
#define D_START                 "Start"
#define D_STATUS                "Status"
#define D_STD_TIME              "STD"
#define D_STOP                  "Stop"
#define D_SUBNET_MASK           "Subnet Mask"
#define D_SUBSCRIBE_TO          "Subscribe to"
#define D_UNSUBSCRIBE_FROM      "Unsubscribe from"
#define D_SUCCESSFUL            "Successful"
#define D_SUNRISE               "Sunrise"
#define D_SUNSET                "Sunset"
#define D_SYSTEM                "System"
#define D_FUNCTION_TASKER_INTERFACE "Tasker_Interface "
#define D_TEMP                  "Temp"
#define D_TEMPS                 "Temps"
#define D_TYPE                  "Type"
#define D_TEMPERATURE           "Temperature"
#define D_TO                    "to"
#define D_TOGGLE                "Toggle"
#define D_TIME_ON               "Time_On"
#define D_TOPIC                 "Topic"
#define D_TIMERS                "Timers"
#define D_PAYLOAD               "Payload"
#define D_TOTAL_USAGE           "Total Usage"
#define D_TRANSMIT              "Transmit"
#define D_TRUE                  "True"
#define D_UPGRADE               "Upgrade"
#define D_UPLOAD                "Upload"
#define D_UPTIME                "Uptime"
#define D_USER                  "User"
#define D_UTC_TIME              "UTC"
#define D_VERSION               "Version"
#define D_VOLTAGE               "Voltage"
#define D_VALUES                "Values"
#define D_WEIGHT                "Weight"
#define D_WARMLIGHT             "Warm"
#define D_WEB_SERVER            "Web Server"
#define D_LOAD                  "Load"
#define D_SAVE                  "Save"
#define D_TASKER "Tasker "
#define D_UNKNOWN "UNKNOWN"
#define D_DEBUG_FUNCTION "Debug Function: "
#define D_SENDING "Sending"


// main.cpp
#define D_SERIAL_LOGGING_DISABLED       "Serial logging disabled"
#define D_SYSLOG_LOGGING_REENABLED      "Syslog logging re-enabled"
#define D_SET_BAUDRATE_TO               "Set Baudrate to"

/**
 *  support
 * */
#define D_OSWATCH                       "osWatch"
#define D_BLOCKED_LOOP                  "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS        "WPSconfig FAILED with status"
#define D_ACTIVE_FOR_3_MINUTES          "active for 3 minutes"
#define D_FAILED_TO_START               "failed to start"
#define D_CONNECTING_TO_AP              "Connecting to AP"
#define D_IN_MODE                       "in mode"
#define D_CONNECT_FAILED_NO_IP_ADDRESS  "Connect failed as no IP address received"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Connect failed as AP cannot be reached"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Connect failed with AP incorrect password"
#define D_CONNECT_FAILED_AP_TIMEOUT     "Connect failed with AP timeout"
#define D_ATTEMPTING_CONNECTION         "Attempting connection..."
#define D_CHECKING_CONNECTION           "Checking connection..."
#define D_QUERY_DONE                    "Query done. MQTT services found"
#define D_MQTT_SERVICE_FOUND            "MQTT service found on"
#define D_FOUND_AT                      "found at"
#define D_SYSLOG_HOST_NOT_FOUND         "Syslog Host not found"
#define D_ERROR_UNSUPPORTED             "ERROR : Feature not currently supported"
#define D_LOG_SOME_SETTINGS_RESET "Some settings have been reset"

/**
 * settings
 * */
#define D_SAVED_TO_FLASH_AT             "Saved to flash at"
#define D_LOADED_FROM_FLASH_AT          "Loaded from flash at"
#define D_USE_DEFAULTS                  "Use defaults"
#define D_ERASED_SECTOR                 "Erased sector"

/**
 * webserver
 * */
#define D_NOSCRIPT "JavaScript Needed"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware<br/>please upgrade"
#define D_WEBSERVER_ACTIVE_ON "Web server active on"
#define D_WEBSERVER "WebServer"
#define D_WITH_IP_ADDRESS "with IP address"
#define D_WEBSERVER_STOPPED "Web server stopped"
#define D_FILE_NOT_FOUND "File Not Found"
#define D_REDIRECTED "Redirected to captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager set AccessPoint"
#define D_TRYING_TO_CONNECT "Trying to connect device to network"

#define D_RESTART_IN "Restart in"

#define D_SECONDS "Seconds"
#define D_MINUTES "Minutes"

#define D_DEVICE_WILL_RESTART "Device will restart in a few seconds"
#define D_BUTTON_TOGGLE "Toggle"
#define D_CONFIGURATION "Configuration"
#define D_INFORMATION "Information"
#define D_FIRMWARE_UPGRADE "Firmware Upgrade"
#define D_CONSOLE "Console"
#define D_CONFIRM_RESTART "Confirm Restart"
#define D_SYSTEM_SETTINGS "System Settings"

#define D_COLOUR_PALETTE_EDITOR "Colour Palette Editor"
#define D_COLOUR_MIXER_EDITOR "Colour Mixer Editor"
#define D_CONFIGURE_MODULE "Configure Module"
#define D_CONFIGURE_WIFI "Configure WiFi"
#define D_CONFIGURE_MQTT "Configure MQTT"
#define D_CONFIGURE_LOGGING "Configure Logging"
#define D_CONFIGURE_OTHER "Configure Other"
#define D_CONFIRM_RESET_CONFIGURATION "Confirm Reset Configuration"
#define D_RESET_CONFIGURATION "Reset Configuration"
#define D_BACKUP_CONFIGURATION "Backup Configuration"
#define D_RESTORE_CONFIGURATION "Restore Configuration"
#define D_MAIN_MENU "Main Menu"

#define D_MODULE_PARAMETERS "Module parameters"
#define D_MODULE_TYPE "Module type"
#define D_PULLUP_ENABLE "No Button/Switch pull-up"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Wifi parameters"
#define D_SCAN_FOR_WIFI_NETWORKS "Scan for wifi networks"
#define D_SCAN_DONE "Scan done"
#define D_NO_NETWORKS_FOUND "No networks found"
#define D_REFRESH_TO_SCAN_AGAIN "Refresh to scan again"
#define D_DUPLICATE_ACCESSPOINT "Duplicate AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Skipping due to low quality"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Password"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Password"

#define D_MQTT_PARAMETERS "MQTT parameters"
#define D_CLIENT "Client"
#define D_FULL_TOPIC "Full Topic"

#define D_LOGGING_PARAMETERS "Logging parameters"
#define D_SERIAL_LOG_LEVEL "Serial log level"
#define D_WEB_LOG_LEVEL "Web log level"
#define D_SYS_LOG_LEVEL "Syslog level"
#define D_MORE_DEBUG "More debug"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Telemetry period"

#define D_OTHER_PARAMETERS "Other parameters"
#define D_TEMPLATE "Template"
#define D_ACTIVATE "Activate"
#define D_WEB_ADMIN_PASSWORD "Web Admin Password"
#define D_MQTT_ENABLE "MQTT enable"
#define D_FRIENDLY_NAME "Friendly Name"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_CONFIGURE_TEMPLATE "Configure Template"
#define D_TEMPLATE_PARAMETERS "Template parameters"
#define D_TEMPLATE_NAME "Name"
#define D_BASE_TYPE "Based on"
#define D_TEMPLATE_FLAGS "Options"
#define D_ALLOW_ADC0 "ADC0 input"
#define D_ALLOW_ADC0_TEMP "ADC0 temperature"
#define D_ALLOW_PULLUP "User pull-up selection"

#define D_SAVE_CONFIGURATION "Save configuration"
#define D_CONFIGURATION_SAVED "Configuration saved"
#define D_CONFIGURATION_RESET "Configuration reset"

#define D_PROGRAM_VERSION "Program Version"
#define D_BUILD_DATE_AND_TIME "Build Date & Time"
#define D_CORE_AND_SDK_VERSION "Core/SDK Version"
#define D_FLASH_WRITE_COUNT "Flash write Count"
#define D_MAC_ADDRESS "MAC Address"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Port"
#define D_MQTT_CLIENT "MQTT Client"
#define D_MQTT_USER "MQTT User"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Group Topic"
#define D_MQTT_FULL_TOPIC "MQTT Full Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Flash Size"
#define D_FREE_PROGRAM_SPACE "Free Program Space"

#define D_UPGRADE_BY_WEBSERVER "Upgrade by web server"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Start upgrade"
#define D_UPGRADE_BY_FILE_UPLOAD "Upgrade by file upload"
#define D_UPLOAD_STARTED "Upload started"
#define D_UPGRADE_STARTED "Upgrade started"
#define D_UPLOAD_DONE "Upload done"
#define D_UPLOAD_ERR_1 "No file selected"
#define D_UPLOAD_ERR_2 "Not enough space"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "Program flash size is larger than real flash size"
#define D_UPLOAD_ERR_5 "Upload buffer miscompare"
#define D_UPLOAD_ERR_6 "Upload failed. Enable logging 3"
#define D_UPLOAD_ERR_7 "Upload aborted"
#define D_UPLOAD_ERR_8 "File invalid"
#define D_UPLOAD_ERR_9 "File too large"
#define D_UPLOAD_ERR_10 "Failed to init RF chip"
#define D_UPLOAD_ERR_11 "Failed to erase RF chip"
#define D_UPLOAD_ERR_12 "Failed to write to RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Upload error code"

#define D_ENTER_COMMAND "Enter command"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Enable weblog 2 if response expected"
#define D_NEED_USER_AND_PASSWORD "Need user=<username>&password=<password>"

/**
 * Pixels
 * */
#define D_NEOPIXEL_PALETTE "Palette "
#define D_NEOPIXEL_TRANSITION_ORDER "Transition order "
#define D_NEOPIXEL_BLENDTIME "Transition time "
#define D_NEOPIXEL_BLENDRATE "Transition rate "
#define D_NEOPIXEL_MODE "Mode "

/**
 * mqtt
 * */
#define D_FINGERPRINT "Verify TLS fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "TLS Connect failed to"
#define D_RETRY_IN "Retry in"
#define D_VERIFIED "Verified using Fingerprint"
#define D_INSECURE "Insecure connection due to invalid Fingerprint"
#define D_CONNECT_FAILED_TO "Connect failed to"
#define D_TOPIC_TELE      "tele"
#define D_TOPIC_IFCHANGED "ifchanged"
#define D_TOPIC_SLASH "/"
#define D_TOPIC_STATUS "status"
#define D_TOPIC_STATUS_S      D_TOPIC_STATUS D_TOPIC_SLASH
#define D_TOPIC_S_TELE_S      D_TOPIC_SLASH D_TOPIC_TELE D_TOPIC_SLASH
#define D_TOPIC_S_IFCHANGED_S D_TOPIC_SLASH D_TOPIC_IFCHANGED D_TOPIC_SLASH
// #define D_TOPIC_PIXELS "/pixels"
// #define D_TOPIC_NEXTION "/nextion"
// #define D_TOPIC_CEILINGFAN "/ceilingfan"
// #define D_TOPIC_SYSTEM "/system"
// #define D_TOPIC_HEATING "/heating"
// #define D_TOPIC_RELAYS "/relays"
// #define D_TOPIC_RADIATORFAN "/radiatorfan"
// #define D_TOPIC_BLINDS "/blinds"

#define D_TOPIC_COMMAND "set" //to be "command"
#define D_TOPIC_STATUS "status" //to be "command"
#define D_TOPIC_RESPONSE "response"


/**
 * Nextion
 * */
#define D_NEXTION_TX "HMI TX --> "
#define D_NEXTION_RX "HMI RX <-- "
#define D_NEXTION_COMMAND "TYPE"


/**
 * Timers
 * */
#define D_CONFIGURE_TIMER "Configure Timer"
#define D_TIMER_PARAMETERS "Timer parameters"
#define D_TIMER_ENABLE "Enable Timers"
#define D_TIMER_ARM "Arm"
#define D_TIMER_TIME "Time"
#define D_TIMER_DAYS "Days"
#define D_TIMER_REPEAT "Repeat"
#define D_TIMER_OUTPUT "Output"
#define D_TIMER_ACTION "Action"


/**
 * Energy
 * */
#define D_ENERGY_TODAY "Energy Today"
#define D_ENERGY_YESTERDAY "Energy Yesterday"
#define D_ENERGY_TOTAL "Energy Total"


/**
 * db18s20
 * */
#define D_SENSOR_BUSY "Sensor busy"
#define D_SENSOR_CRC_ERROR "Sensor CRC error"
#define D_SENSORS_FOUND "Sensors found"


/**
 * dht
 * */
#define D_TIMEOUT_WAITING_FOR "Timeout waiting for"
#define D_START_SIGNAL_LOW "start signal low"
#define D_START_SIGNAL_HIGH "start signal high"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Checksum failure"


/**
 * mpu6050
 * */
#define D_AX_AXIS "Accel. X-Axis"
#define D_AY_AXIS "Accel. Y-Axis"
#define D_AZ_AXIS "Accel. Z-Axis"
#define D_GX_AXIS "Gyro X-Axis"
#define D_GY_AXIS "Gyro Y-Axis"
#define D_GZ_AXIS "Gyro Z-Axis"


/****
 * 
 * List of function, that will be shown in webui
 *  keep them as short as possible to be able to fit them in GUI drop down box 
 * */
#define D_GPIO_FUNCTION_NONE_CTR          "None"
#define D_GPIO_FUNCTION_USER_CTR          "User"
#define D_GPIO_FUNCTION_DOOR_POSITION_CTR "Door Position"
#define D_GPIO_FUNCTION_RGB_DATA1_CTR      "RGBData1"
#define D_GPIO_FUNCTION_RGB_DATA2_CTR      "RGBData2"
#define D_GPIO_FUNCTION_RGB_DATA3_CTR      "RGBData3"
#define D_GPIO_FUNCTION_RGB_DATA_CTR      "WS281x Data"
#define D_GPIO_FUNCTION_RGB_CLOCK_CTR     "WS281x Clock"



#define D_GPIO_FUNCTION_PIXELBUS_INDEXED_CTR "PixelBus ##"
#define D_GPIO_FUNCTION_PIXELBUS_01_A_CTR      "PixelBus 01A"
#define D_GPIO_FUNCTION_PIXELBUS_01_B_CTR      "PixelBus 01B"
#define D_GPIO_FUNCTION_PIXELBUS_01_C_CTR      "PixelBus 01C"
#define D_GPIO_FUNCTION_PIXELBUS_01_D_CTR      "PixelBus 01D"
#define D_GPIO_FUNCTION_PIXELBUS_01_E_CTR      "PixelBus 01E"
#define D_GPIO_FUNCTION_PIXELBUS_02_A_CTR      "PixelBus 02A"
#define D_GPIO_FUNCTION_PIXELBUS_02_B_CTR      "PixelBus 02B"
#define D_GPIO_FUNCTION_PIXELBUS_02_C_CTR      "PixelBus 02C"
#define D_GPIO_FUNCTION_PIXELBUS_02_D_CTR      "PixelBus 02D"
#define D_GPIO_FUNCTION_PIXELBUS_02_E_CTR      "PixelBus 02E"
#define D_GPIO_FUNCTION_PIXELBUS_03_A_CTR      "PixelBus 03A"
#define D_GPIO_FUNCTION_PIXELBUS_03_B_CTR      "PixelBus 03B"
#define D_GPIO_FUNCTION_PIXELBUS_03_C_CTR      "PixelBus 03C"
#define D_GPIO_FUNCTION_PIXELBUS_03_D_CTR      "PixelBus 03D"
#define D_GPIO_FUNCTION_PIXELBUS_03_E_CTR      "PixelBus 03E"
#define D_GPIO_FUNCTION_PIXELBUS_04_A_CTR      "PixelBus 04A"
#define D_GPIO_FUNCTION_PIXELBUS_04_B_CTR      "PixelBus 04B"
#define D_GPIO_FUNCTION_PIXELBUS_04_C_CTR      "PixelBus 04C"
#define D_GPIO_FUNCTION_PIXELBUS_04_D_CTR      "PixelBus 04D"
#define D_GPIO_FUNCTION_PIXELBUS_04_E_CTR      "PixelBus 04E"
#define D_GPIO_FUNCTION_PIXELBUS_05_A_CTR      "PixelBus 05A"
#define D_GPIO_FUNCTION_PIXELBUS_05_B_CTR      "PixelBus 05B"
#define D_GPIO_FUNCTION_PIXELBUS_05_C_CTR      "PixelBus 05C"
#define D_GPIO_FUNCTION_PIXELBUS_05_D_CTR      "PixelBus 05D"
#define D_GPIO_FUNCTION_PIXELBUS_05_E_CTR      "PixelBus 05E"
#define D_GPIO_FUNCTION_PIXELBUS_06_A_CTR      "PixelBus 06A"
#define D_GPIO_FUNCTION_PIXELBUS_06_B_CTR      "PixelBus 06B"
#define D_GPIO_FUNCTION_PIXELBUS_06_C_CTR      "PixelBus 06C"
#define D_GPIO_FUNCTION_PIXELBUS_06_D_CTR      "PixelBus 06D"
#define D_GPIO_FUNCTION_PIXELBUS_06_E_CTR      "PixelBus 06E"
#define D_GPIO_FUNCTION_PIXELBUS_07_A_CTR      "PixelBus 07A"
#define D_GPIO_FUNCTION_PIXELBUS_07_B_CTR      "PixelBus 07B"
#define D_GPIO_FUNCTION_PIXELBUS_07_C_CTR      "PixelBus 07C"
#define D_GPIO_FUNCTION_PIXELBUS_07_D_CTR      "PixelBus 07D"
#define D_GPIO_FUNCTION_PIXELBUS_07_E_CTR      "PixelBus 07E"
#define D_GPIO_FUNCTION_PIXELBUS_08_A_CTR      "PixelBus 08A"
#define D_GPIO_FUNCTION_PIXELBUS_08_B_CTR      "PixelBus 08B"
#define D_GPIO_FUNCTION_PIXELBUS_08_C_CTR      "PixelBus 08C"
#define D_GPIO_FUNCTION_PIXELBUS_08_D_CTR      "PixelBus 08D"
#define D_GPIO_FUNCTION_PIXELBUS_08_E_CTR      "PixelBus 08E"
#define D_GPIO_FUNCTION_PIXELBUS_09_A_CTR      "PixelBus 09A"
#define D_GPIO_FUNCTION_PIXELBUS_09_B_CTR      "PixelBus 09B"
#define D_GPIO_FUNCTION_PIXELBUS_09_C_CTR      "PixelBus 09C"
#define D_GPIO_FUNCTION_PIXELBUS_09_D_CTR      "PixelBus 09D"
#define D_GPIO_FUNCTION_PIXELBUS_09_E_CTR      "PixelBus 09E"
#define D_GPIO_FUNCTION_PIXELBUS_10_A_CTR      "PixelBus 10A"
#define D_GPIO_FUNCTION_PIXELBUS_10_B_CTR      "PixelBus 10B"
#define D_GPIO_FUNCTION_PIXELBUS_10_C_CTR      "PixelBus 10C"
#define D_GPIO_FUNCTION_PIXELBUS_10_D_CTR      "PixelBus 10D"
#define D_GPIO_FUNCTION_PIXELBUS_10_E_CTR      "PixelBus 10E"

#define D_GPIO_FUNCTION_SWT1_CTR     "SWT1"
#define D_GPIO_FUNCTION_SWT2_CTR     "SWT2"
#define D_GPIO_FUNCTION_SWT3_CTR     "SWT3"
#define D_GPIO_FUNCTION_SWT4_CTR     "SWT4"
#define D_GPIO_FUNCTION_SWT5_CTR     "SWT5"
#define D_GPIO_FUNCTION_SWT6_CTR     "SWT6"
#define D_GPIO_FUNCTION_SWT7_CTR     "SWT7"
#define D_GPIO_FUNCTION_SWT8_CTR     "SWT8"
#define D_GPIO_FUNCTION_SWT9_CTR     "SWT9"
#define D_GPIO_FUNCTION_SWT10_CTR    "SWT10"
#define D_GPIO_FUNCTION_SWT11_CTR    "SWT11"
#define D_GPIO_FUNCTION_SWT12_CTR    "SWT12"
#define D_GPIO_FUNCTION_SWT13_CTR    "SWT13"
#define D_GPIO_FUNCTION_SWT14_CTR    "SWT14"
#define D_GPIO_FUNCTION_SWT15_CTR    "SWT15"
#define D_GPIO_FUNCTION_SWT16_CTR    "SWT16"
#define D_GPIO_FUNCTION_SWT17_CTR    "SWT17"
#define D_GPIO_FUNCTION_SWT18_CTR    "SWT18"
#define D_GPIO_FUNCTION_SWT19_CTR    "SWT19"
#define D_GPIO_FUNCTION_SWT20_CTR    "SWT20"
#define D_GPIO_FUNCTION_SWT21_CTR    "SWT21"
#define D_GPIO_FUNCTION_SWT22_CTR    "SWT22"
#define D_GPIO_FUNCTION_SWT23_CTR    "SWT23"
#define D_GPIO_FUNCTION_SWT24_CTR    "SWT24"
#define D_GPIO_FUNCTION_SWT25_CTR    "SWT25"
#define D_GPIO_FUNCTION_SWT26_CTR    "SWT26"
#define D_GPIO_FUNCTION_SWT27_CTR    "SWT27"
#define D_GPIO_FUNCTION_SWT28_CTR    "SWT28"
#define D_GPIO_FUNCTION_SWT29_CTR    "SWT29"
#define D_GPIO_FUNCTION_SWT30_CTR    "SWT30"
#define D_GPIO_FUNCTION_SWT31_CTR    "SWT31"
#define D_GPIO_FUNCTION_SWT32_CTR    "SWT32"

#define D_GPIO_FUNCTION_SWT1_INV_CTR     "SWT1 Inv"
#define D_GPIO_FUNCTION_SWT2_INV_CTR     "SWT2 Inv"
#define D_GPIO_FUNCTION_SWT3_INV_CTR     "SWT3 Inv"
#define D_GPIO_FUNCTION_SWT4_INV_CTR     "SWT4 Inv"
#define D_GPIO_FUNCTION_SWT5_INV_CTR     "SWT5 Inv"
#define D_GPIO_FUNCTION_SWT6_INV_CTR     "SWT6 Inv"
#define D_GPIO_FUNCTION_SWT7_INV_CTR     "SWT7 Inv"
#define D_GPIO_FUNCTION_SWT8_INV_CTR     "SWT8 Inv"
#define D_GPIO_FUNCTION_SWT9_INV_CTR     "SWT9 Inv"
#define D_GPIO_FUNCTION_SWT10_INV_CTR    "SWT10 Inv"
#define D_GPIO_FUNCTION_SWT11_INV_CTR    "SWT11 Inv"
#define D_GPIO_FUNCTION_SWT12_INV_CTR    "SWT12 Inv"
#define D_GPIO_FUNCTION_SWT13_INV_CTR    "SWT13 Inv"
#define D_GPIO_FUNCTION_SWT14_INV_CTR    "SWT14 Inv"
#define D_GPIO_FUNCTION_SWT15_INV_CTR    "SWT15 Inv"
#define D_GPIO_FUNCTION_SWT16_INV_CTR    "SWT16 Inv"
#define D_GPIO_FUNCTION_SWT17_INV_CTR    "SWT17 Inv"
#define D_GPIO_FUNCTION_SWT18_INV_CTR    "SWT18 Inv"
#define D_GPIO_FUNCTION_SWT19_INV_CTR    "SWT19 Inv"
#define D_GPIO_FUNCTION_SWT20_INV_CTR    "SWT20 Inv"
#define D_GPIO_FUNCTION_SWT21_INV_CTR    "SWT21 Inv"
#define D_GPIO_FUNCTION_SWT22_INV_CTR    "SWT22 Inv"
#define D_GPIO_FUNCTION_SWT23_INV_CTR    "SWT23 Inv"
#define D_GPIO_FUNCTION_SWT24_INV_CTR    "SWT24 Inv"
#define D_GPIO_FUNCTION_SWT25_INV_CTR    "SWT25 Inv"
#define D_GPIO_FUNCTION_SWT26_INV_CTR    "SWT26 Inv"
#define D_GPIO_FUNCTION_SWT27_INV_CTR    "SWT27 Inv"
#define D_GPIO_FUNCTION_SWT28_INV_CTR    "SWT28 Inv"
#define D_GPIO_FUNCTION_SWT29_INV_CTR    "SWT29 Inv"
#define D_GPIO_FUNCTION_SWT30_INV_CTR    "SWT30 Inv"
#define D_GPIO_FUNCTION_SWT31_INV_CTR    "SWT31 Inv"
#define D_GPIO_FUNCTION_SWT32_INV_CTR    "SWT32 Inv"

#define D_GPIO_FUNCTION_SWT1_NP_CTR     "SWT1_NP"
#define D_GPIO_FUNCTION_SWT2_NP_CTR     "SWT2_NP"
#define D_GPIO_FUNCTION_SWT3_NP_CTR     "SWT3_NP"
#define D_GPIO_FUNCTION_SWT4_NP_CTR     "SWT4_NP"
#define D_GPIO_FUNCTION_SWT5_NP_CTR     "SWT5_NP"
#define D_GPIO_FUNCTION_SWT6_NP_CTR     "SWT6_NP"
#define D_GPIO_FUNCTION_SWT7_NP_CTR     "SWT7_NP"
#define D_GPIO_FUNCTION_SWT8_NP_CTR     "SWT8_NP"
#define D_GPIO_FUNCTION_SWT9_NP_CTR     "SWT9_NP"
#define D_GPIO_FUNCTION_SWT10_NP_CTR    "SWT10_NP"
#define D_GPIO_FUNCTION_SWT11_NP_CTR    "SWT11_NP"
#define D_GPIO_FUNCTION_SWT12_NP_CTR    "SWT12_NP"
#define D_GPIO_FUNCTION_SWT13_NP_CTR    "SWT13_NP"
#define D_GPIO_FUNCTION_SWT14_NP_CTR    "SWT14_NP"
#define D_GPIO_FUNCTION_SWT15_NP_CTR    "SWT15_NP"
#define D_GPIO_FUNCTION_SWT16_NP_CTR    "SWT16_NP"
#define D_GPIO_FUNCTION_SWT17_NP_CTR    "SWT17_NP"
#define D_GPIO_FUNCTION_SWT18_NP_CTR    "SWT18_NP"
#define D_GPIO_FUNCTION_SWT19_NP_CTR    "SWT19_NP"
#define D_GPIO_FUNCTION_SWT20_NP_CTR    "SWT20_NP"
#define D_GPIO_FUNCTION_SWT21_NP_CTR    "SWT21_NP"
#define D_GPIO_FUNCTION_SWT22_NP_CTR    "SWT22_NP"
#define D_GPIO_FUNCTION_SWT23_NP_CTR    "SWT23_NP"
#define D_GPIO_FUNCTION_SWT24_NP_CTR    "SWT24_NP"
#define D_GPIO_FUNCTION_SWT25_NP_CTR    "SWT25_NP"
#define D_GPIO_FUNCTION_SWT26_NP_CTR    "SWT26_NP"
#define D_GPIO_FUNCTION_SWT27_NP_CTR    "SWT27_NP"
#define D_GPIO_FUNCTION_SWT28_NP_CTR    "SWT28_NP"
#define D_GPIO_FUNCTION_SWT29_NP_CTR    "SWT29_NP"
#define D_GPIO_FUNCTION_SWT30_NP_CTR    "SWT30_NP"
#define D_GPIO_FUNCTION_SWT31_NP_CTR    "SWT31_NP"
#define D_GPIO_FUNCTION_SWT32_NP_CTR    "SWT32_NP"

#define D_GPIO_FUNCTION_SWT1_INV_NP_CTR     "SWT1 Inv NP"
#define D_GPIO_FUNCTION_SWT2_INV_NP_CTR     "SWT2 Inv NP"
#define D_GPIO_FUNCTION_SWT3_INV_NP_CTR     "SWT3 Inv NP"
#define D_GPIO_FUNCTION_SWT4_INV_NP_CTR     "SWT4 Inv NP"
#define D_GPIO_FUNCTION_SWT5_INV_NP_CTR     "SWT5 Inv NP"
#define D_GPIO_FUNCTION_SWT6_INV_NP_CTR     "SWT6 Inv NP"
#define D_GPIO_FUNCTION_SWT7_INV_NP_CTR     "SWT7 Inv NP"
#define D_GPIO_FUNCTION_SWT8_INV_NP_CTR     "SWT8 Inv NP"
#define D_GPIO_FUNCTION_SWT9_INV_NP_CTR     "SWT9 Inv NP"
#define D_GPIO_FUNCTION_SWT10_INV_NP_CTR    "SWT10 Inv NP"
#define D_GPIO_FUNCTION_SWT11_INV_NP_CTR    "SWT11 Inv NP"
#define D_GPIO_FUNCTION_SWT12_INV_NP_CTR    "SWT12 Inv NP"
#define D_GPIO_FUNCTION_SWT13_INV_NP_CTR    "SWT13 Inv NP"
#define D_GPIO_FUNCTION_SWT14_INV_NP_CTR    "SWT14 Inv NP"
#define D_GPIO_FUNCTION_SWT15_INV_NP_CTR    "SWT15 Inv NP"
#define D_GPIO_FUNCTION_SWT16_INV_NP_CTR    "SWT16 Inv NP"
#define D_GPIO_FUNCTION_SWT17_INV_NP_CTR    "SWT17 Inv NP"
#define D_GPIO_FUNCTION_SWT18_INV_NP_CTR    "SWT18 Inv NP"
#define D_GPIO_FUNCTION_SWT19_INV_NP_CTR    "SWT19 Inv NP"
#define D_GPIO_FUNCTION_SWT20_INV_NP_CTR    "SWT20 Inv NP"
#define D_GPIO_FUNCTION_SWT21_INV_NP_CTR    "SWT21 Inv NP"
#define D_GPIO_FUNCTION_SWT22_INV_NP_CTR    "SWT22 Inv NP"
#define D_GPIO_FUNCTION_SWT23_INV_NP_CTR    "SWT23 Inv NP"
#define D_GPIO_FUNCTION_SWT24_INV_NP_CTR    "SWT24 Inv NP"
#define D_GPIO_FUNCTION_SWT25_INV_NP_CTR    "SWT25 Inv NP"
#define D_GPIO_FUNCTION_SWT26_INV_NP_CTR    "SWT26 Inv NP"
#define D_GPIO_FUNCTION_SWT27_INV_NP_CTR    "SWT27 Inv NP"
#define D_GPIO_FUNCTION_SWT28_INV_NP_CTR    "SWT28 Inv NP"
#define D_GPIO_FUNCTION_SWT29_INV_NP_CTR    "SWT29 Inv NP"
#define D_GPIO_FUNCTION_SWT30_INV_NP_CTR    "SWT30 Inv NP"
#define D_GPIO_FUNCTION_SWT31_INV_NP_CTR    "SWT31 Inv NP"
#define D_GPIO_FUNCTION_SWT32_INV_NP_CTR    "SWT32 Inv NP"

#define D_GPIO_FUNCTION_REL1_CTR          "Relay 1"
#define D_GPIO_FUNCTION_REL2_CTR          "Relay 2"
#define D_GPIO_FUNCTION_REL3_CTR          "Relay 3"
#define D_GPIO_FUNCTION_REL4_CTR          "Relay 4"
#define D_GPIO_FUNCTION_REL1_INV_CTR      "Relay 1 Inv"
#define D_GPIO_FUNCTION_REL2_INV_CTR      "Relay 2 Inv"
#define D_GPIO_FUNCTION_REL3_INV_CTR      "Relay 3 Inv"
#define D_GPIO_FUNCTION_REL4_INV_CTR      "Relay 4 Inv"
#define D_GPIO_FUNCTION_FAN_PWM1_CTR      "Fan PWM1"
#define D_GPIO_FUNCTION_I2C_SCL_CTR       "I2C SCL"
#define D_GPIO_FUNCTION_I2C_SDA_CTR       "I2C SDA"

#define D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "LDR Basic A1"
#define D_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR "LDR Basic A2"
#define D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "LDR Basic D1"
#define D_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR "LDR Basic D2"


#define D_GPIO_FUNCTION_ANALOG_INPUT0_CTR "Analog Input0"

#define D_GPIO_FUNCTION_OLED_RESET_CTR "OLED Reset"

#define D_GPIO_FUNCTION_PIR_1_CTR         "PIR 1"
#define D_GPIO_FUNCTION_PIR_2_CTR         "PIR 2"
#define D_GPIO_FUNCTION_PIR_3_CTR         "PIR 3"
#define D_GPIO_FUNCTION_PIR_1_INV_CTR     "PIR 1 Inv"
#define D_GPIO_FUNCTION_PIR_2_INV_CTR     "PIR 2 Inv"
#define D_GPIO_FUNCTION_PIR_3_INV_CTR     "PIR 3 Inv"

#define D_GPIO_FUNCTION_DHT11_CTR         "DHT11"
#define D_GPIO_FUNCTION_DHT11_1_CTR         "DHT11_1"
#define D_GPIO_FUNCTION_DHT11_2_CTR         "DHT11_2"
#define D_GPIO_FUNCTION_DHT22_CTR         "DHT22"
#define D_GPIO_FUNCTION_DHT22_1_CTR         "DHT22_1"
#define D_GPIO_FUNCTION_DHT22_2_CTR         "DHT22_2"
#define D_GPIO_FUNCTION_AM2301_CTR        "AM2301"
#define D_GPIO_FUNCTION_SI7021_CTR        "SI7021"
#define D_GPIO_FUNCTION_DS18X20_CTR       "DS18x20"
#define D_GPIO_FUNCTION_DS18X20_1_CTR       "DS18x20_1"
#define D_GPIO_FUNCTION_DS18X20_2_CTR       "DS18x20_2"

#define D_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR "ForcedLow"
#define D_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR "ForcedHigh"

#define D_GPIO_FUNCTION_BUZZER_CTR "Buzzer"
#define D_GPIO_FUNCTION_CHIME_RINGER_CTR  "Chime Ringer"
#define D_GPIO_FUNCTION_CHIME_INPUT_CTR   "Chime Input"
#define D_GPIO_FUNCTION_IRSEND_CTR        "IRsend"
#define D_GPIO_FUNCTION_SWITCH_CTR        "Switch"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_1_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 1"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_2_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 2"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_3_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 3"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_4_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 4"     // Suffix "1"
#define D_GPIO_FUNCTION_BUTTON_CTR        "Button"     // Suffix "1"
// #define D_GPIO_FUNCTION_BUTTON_1_CTR      D_GPIO_FUNCTION_BUTTON_CTR " 1"     // Suffix "1"
#define D_GPIO_FUNCTION_RELAY_CTR         "Relay"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_1_CTR       D_GPIO_FUNCTION_RELAY_CTR " 1"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_2_CTR       D_GPIO_FUNCTION_RELAY_CTR " 2"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_3_CTR       D_GPIO_FUNCTION_RELAY_CTR " 3"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_4_CTR       D_GPIO_FUNCTION_RELAY_CTR " 4"      // Suffix "1i"

#define D_GPIO_FUNCTION_DOOR_OPEN_CTR       "Door Open"
#define D_GPIO_FUNCTION_DOOR_LOCK_CTR       "Door Lock"
#define D_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR "Serial Debug TX"
#define D_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR "Serial Debug RX"

#define D_GPIO_FUNCTION_HWSERIAL0_TX_CTR "HWSERIAL0_TX"
#define D_GPIO_FUNCTION_HWSERIAL0_RX_CTR "HWSERIAL0_RX"
#define D_GPIO_FUNCTION_HWSERIAL1_TX_CTR "HWSERIAL1_TX"
#define D_GPIO_FUNCTION_HWSERIAL1_RX_CTR "HWSERIAL1_RX"
#define D_GPIO_FUNCTION_HWSERIAL2_TX_CTR "HWSERIAL2_TX"
#define D_GPIO_FUNCTION_HWSERIAL2_RX_CTR "HWSERIAL2_RX"

#define D_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR "GPS Serial0 TX"
#define D_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR "GPS Serial0 RX"
#define D_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR "GPS Serial1 TX"
#define D_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR "GPS Serial1 RX"
#define D_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR "GPS Serial2 TX"
#define D_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR "GPS Serial2 RX"


#define D_GPIO_FUNCTION__FONA_POWER_KEY__CTR       "Fona Key"
#define D_GPIO_FUNCTION__FONA_POWER_STATUS__CTR    "Fona PS"
#define D_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR  "Fona NS"
#define D_GPIO_FUNCTION__FONA_RESET__CTR           "Fona Rst"
#define D_GPIO_FUNCTION__FONA_UART_TX__CTR         "Fona TX"
#define D_GPIO_FUNCTION__FONA_UART_RX__CTR         "Fona RX"
#define D_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR  "Fona RI"



#define D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR "Modem DTR"
#define D_GPIO_FUNCTION__MODEM_TX__CTR "Modem TX"
#define D_GPIO_FUNCTION__MODEM_RX__CTR "Modem RX"
#define D_GPIO_FUNCTION__MODEM_POWER__CTR "Modem PWR"


#define D_GPIO_FUNCTION_BUILTIN_LED1_CTR              "Builtin LED 1" // only enabled if available and not being used by another pin

#define D_GPIO_FUNCTION_LED1_CTR              "Led1"
#define D_GPIO_FUNCTION_LED2_CTR              "Led2"
#define D_GPIO_FUNCTION_LED3_CTR              "Led3"
#define D_GPIO_FUNCTION_LED4_CTR              "Led4"
#define D_GPIO_FUNCTION_LED5_CTR              "Led5"
#define D_GPIO_FUNCTION_LED6_CTR              "Led6"
#define D_GPIO_FUNCTION_LED7_CTR              "Led7"
#define D_GPIO_FUNCTION_LED8_CTR              "Led8"

#define D_GPIO_FUNCTION_LED1_INV_CTR          "Led1 Inv"
#define D_GPIO_FUNCTION_LED2_INV_CTR          "Led2 Inv"
#define D_GPIO_FUNCTION_LED3_INV_CTR          "Led3 Inv"
#define D_GPIO_FUNCTION_LED4_INV_CTR          "Led4 Inv"
#define D_GPIO_FUNCTION_LED5_INV_CTR          "Led5 Inv"
#define D_GPIO_FUNCTION_LED6_INV_CTR          "Led6 Inv"
#define D_GPIO_FUNCTION_LED7_INV_CTR          "Led7 Inv"
#define D_GPIO_FUNCTION_LED8_INV_CTR          "Led8 Inv"

#define D_GPIO_FUNCTION_PWM1_CTR           "PWM1"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM2_CTR           "PWM2"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM3_CTR           "PWM3"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM4_CTR           "PWM4"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM5_CTR           "PWM5"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM1_INV_CTR           "PWM1_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM2_INV_CTR           "PWM2_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM3_INV_CTR           "PWM3_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM4_INV_CTR           "PWM4_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM5_INV_CTR           "PWM5_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_TXD_CTR            "Serial Tx"
#define D_GPIO_FUNCTION_COUNTER_CTR       "Counter"    // Suffix "1"
#define D_GPIO_FUNCTION_IRRECV_CTR        "IRrecv"
#define D_SENSOR_MHZ_RX_CTR        "MHZ Rx"
#define D_SENSOR_MHZ_TX_CTR        "MHZ Tx"
#define D_GPIO_FUNCTION_PZEM004_RX_CTR    "PZEM004 Rx"
#define D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR    "PZEM0XX_MODBUS_ Rx"
#define D_GPIO_FUNCTION_PZEM017_RX_CTR    "PZEM017 Rx"
#define D_GPIO_FUNCTION_PZEM0XX_TX_CTR    "PZEM0XX Tx"
#define D_SENSOR_SPI_CS_CTR        "SPI CS"
#define D_SENSOR_SPI_DC_CTR        "SPI DC"
#define D_SENSOR_BACKLIGHT_CTR     "BkLight"
#define D_SENSOR_PMS5003_CTR       "PMS5003"
#define D_GPIO_FUNCTION_SDS0X1_RX_CTR      "SDS0X1 Rx"
#define D_GPIO_FUNCTION_SDS0X1_TX_CTR      "SDS0X1 Tx"
#define D_GPIO_FUNCTION_SR04_ECHO_CTR      "SR04 Ech"
#define D_GPIO_FUNCTION_SR04_TRIG_CTR      "SR04 Trig"
#define D_GPIO_FUNCTION__ROTARY_ENCODER_A__CTR      "REnc A"
#define D_GPIO_FUNCTION__ROTARY_ENCODER_B__CTR      "REnc B"
#define D_GPIO_FUNCTION_NEXTION_RX_CTR     "Nextion Rx"
#define D_GPIO_FUNCTION_NEXTION_TX_CTR     "Nextion Tx"

#define D_GPIO_FUNCTION_KEY1_CTR        "Key1"
#define D_GPIO_FUNCTION_KEY1_INV_CTR        "Key1 Inv"
#define D_GPIO_FUNCTION_KEY2_CTR        "Key2"
#define D_GPIO_FUNCTION_KEY2_INV_CTR        "Key2 Inv"
#define D_GPIO_FUNCTION_KEY3_CTR        "Key3"
#define D_GPIO_FUNCTION_KEY3_INV_CTR        "Key3 Inv"
#define D_GPIO_FUNCTION_KEY4_CTR        "KeY4"
#define D_GPIO_FUNCTION_KEY4_INV_CTR        "Key4 Inv"
#define D_GPIO_FUNCTION_KEY5_CTR        "Key5"
#define D_GPIO_FUNCTION_KEY5_INV_CTR        "Key5 Inv"
#define D_GPIO_FUNCTION_KEY6_CTR        "Key6"
#define D_GPIO_FUNCTION_KEY6_INV_CTR        "Key6 Inv"
#define D_GPIO_FUNCTION_KEY7_CTR        "Key7"
#define D_GPIO_FUNCTION_KEY7_INV_CTR        "Key7 Inv"
#define D_GPIO_FUNCTION_KEY8_CTR        "Key8"
#define D_GPIO_FUNCTION_KEY8_INV_CTR        "Key8 Inv"


#define D_GPIO_FUNCTION_KEY1_TOUCH_CTR        "Key1 Touch"
#define D_GPIO_FUNCTION_KEY2_TOUCH_CTR        "Key2 Touch"
#define D_GPIO_FUNCTION_KEY3_TOUCH_CTR        "Key3 Touch"
#define D_GPIO_FUNCTION_KEY4_TOUCH_CTR        "Key4 Touch"
#define D_GPIO_FUNCTION_KEY5_TOUCH_CTR        "Key5 Touch"
#define D_GPIO_FUNCTION_KEY6_TOUCH_CTR        "Key6 Touch"
#define D_GPIO_FUNCTION_KEY7_TOUCH_CTR        "Key7 Touch"
#define D_GPIO_FUNCTION_KEY8_TOUCH_CTR        "Key8 Touch"

#define D_GPIO_FUNCTION_KEY1_NP_CTR        "Key1 NP"

#define D_GPIO_FUNCTION_KEY1_PULLDOWN_CTR        "Key1 PD" //active when high. Only available on GPIO16/D0 of esp8266


#define D_GPIO_FUNCTION__RF_433MHZ_RX__CTR "RF 433 RX"
#define D_GPIO_FUNCTION__RF_433MHZ_TX__CTR "RF 433 TX"

#define D_SENSOR_SDM120_TX_CTR     "SDMx20 Tx"
#define D_SENSOR_SDM120_RX_CTR     "SDMx20 Rx"
#define D_SENSOR_SDM630_TX_CTR     "SDM630 Tx"
#define D_SENSOR_SDM630_RX_CTR     "SDM630 Rx"
#define D_SENSOR_TM1638_CLK_CTR    "TM16 CLK"
#define D_SENSOR_TM1638_DIO_CTR    "TM16 DIO"
#define D_SENSOR_TM1638_STB_CTR    "TM16 STB"
#define D_SENSOR_HX711_SCK_CTR     "HX711 SCK"
#define D_SENSOR_HX711_DAT_CTR     "HX711 DAT"
#define D_SENSOR_TX20_TX_CTR       "TX20"
#define D_SENSOR_RFSEND_CTR        "RFSend"
#define D_SENSOR_RFRECV_CTR        "RFrecv"
#define D_SENSOR_TUYA_TX_CTR       "Tuya Tx"
#define D_SENSOR_TUYA_RX_CTR       "Tuya Rx"
#define D_SENSOR_MGC3130_XFER_CTR  "MGC3130 Xfr"
#define D_SENSOR_MGC3130_RESET_CTR "MGC3130 Rst"
#define D_SENSOR_SSPI_MISO_CTR     "SSPI MISO"
#define D_SENSOR_SSPI_MOSI_CTR     "SSPI MOSI"
#define D_SENSOR_SSPI_SCLK_CTR     "SSPI SCLK"
#define D_SENSOR_SSPI_CS_CTR       "SSPI CS"
#define D_SENSOR_SSPI_DC_CTR       "SSPI DC"
#define D_SENSOR_RF_SENSOR_CTR     "RF Sensor"
#define D_SENSOR_AZ_RX_CTR         "AZ Rx"
#define D_SENSOR_AZ_TX_CTR         "AZ Tx"
#define D_SENSOR_MAX31855_CS_CTR   "MX31855 CS"
#define D_SENSOR_MAX31855_CLK_CTR  "MX31855 CLK"
#define D_SENSOR_MAX31855_DO_CTR   "MX31855 DO"
#define D_SENSOR_HLW_CF_CTR        "HLW8012 CF"
#define D_SENSOR_HJL_CF_CTR        "BL0937 CF"
#define D_SENSOR_MCP39F5_TX_CTR    "MCP39F5 Tx"
#define D_SENSOR_MCP39F5_RX_CTR    "MCP39F5 Rx"
#define D_SENSOR_MCP39F5_RST_CTR   "MCP39F5 Rst"
#define D_SENSOR_CSE7766_TX_CTR    "CSE7766 Tx"
#define D_SENSOR_CSE7766_RX_CTR    "CSE7766 Rx"
#define D_SENSOR_PN532_TX_CTR      "PN532 Tx"
#define D_SENSOR_PN532_RX_CTR      "PN532 Rx"
#define D_SENSOR_SM16716_CLK_CTR   "SM16716 CLK"
#define D_SENSOR_SM16716_DAT_CTR   "SM16716 DAT"
#define D_SENSOR_SM16716_POWER_CTR "SM16716 PWR"
#define D_SENSOR_MY92X1_DI_CTR     "MY92x1 DI"
#define D_SENSOR_MY92X1_DCKI_CTR   "MY92x1 DCKI"
#define D_SENSOR_ARIRFRCV_CTR      "ALux IrRcv"
#define D_SENSOR_TXD_CTR           "Serial Tx"
#define D_SENSOR_RXD_CTR           "Serial Rx"
#define D_SENSOR_ROTARY_CTR        "Rotary"
#define D_SENSOR_HRE_CLOCK_CTR     "HRE Clock"
#define D_SENSOR_HRE_DATA_CTR      "HRE Data"
#define D_SENSOR_ADE7953_IRQ_CTR   "ADE7953 IRQ"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR     "HBRIDGE L9110 Input A"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR     "HBRIDGE L9110 Input B"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR     "HBRIDGE L9110 Output A"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR     "HBRIDGE L9110 Output B"
#define D_GPIO_FUNCTION_ANALOG_POSITION_CTR "Analog Position"
#define D_GPIO_FUNCTION_FAN_IRSEND_CTR "Fan IRSend"
#define D_GPIO_FUNCTION_ADE7953_IRQ_CTR "ADE7953_IRQ"
#define D_GPIO_FUNCTION_ANALOG_CTR "Analog"


#define D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR "HWSerial1 RingBuffer Rx"
#define D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR "HWSerial1 RingBuffer Tx"

#define D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR "HWSerial2 RingBuffer Rx"
#define D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR "HWSerial2 RingBuffer Tx"

#define D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR  "SDCard HSPI CSO"
#define D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR  "SDCard HSPI CLK"
#define D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR "SDCard HSPI MOSI"
#define D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR "SDCard HSPI MISO"


#define D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR  "SDCard VSPI CSO"
#define D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR  "SDCard VSPI CLK"
#define D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR "SDCard VSPI MOSI"
#define D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR "SDCard VSPI MISO"



#define D_GPIO_FUNCTION_ADC1_CH1_CTR "ADC1_CH1"
#define D_GPIO_FUNCTION_ADC1_CH2_CTR "ADC1_CH2"
#define D_GPIO_FUNCTION_ADC1_CH3_CTR "ADC1_CH3"
#define D_GPIO_FUNCTION_ADC1_CH4_CTR "ADC1_CH4_GPIO32"
#define D_GPIO_FUNCTION_ADC1_CH5_CTR "ADC1_CH5"
#define D_GPIO_FUNCTION_ADC1_CH6_CTR "ADC1_CH6"
#define D_GPIO_FUNCTION_ADC1_CH7_CTR "ADC1_CH7_GPIO35"
#define D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR "ADC1_Ext_Int"

/**
 * Special cases 
 * */
#define D_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR "RXON_SAMPLING_ENABLED"
#define D_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR "CC1110_SYNC_PULSE"


// Special generic pins that can be used as special functions eg logger or debuggers
#define D_GPIO_FUNCTION_GENERIC_GPIO1_CTR "Generic GPIO1"

// Debug pins
#define D_GPIO_FUNCTION_DEBUG_PIN1_CTR "Debug Pin1"
#define D_GPIO_FUNCTION_DEBUG_PIN2_CTR "Debug Pin2"
#define D_GPIO_FUNCTION_DEBUG_PIN3_CTR "Debug Pin3"


#define D_GPIO_WEBCAM_DATA1_CTR     "Cam Data1"
#define D_GPIO_WEBCAM_DATA2_CTR     "Cam Data2"
#define D_GPIO_WEBCAM_DATA3_CTR     "Cam Data3"
#define D_GPIO_WEBCAM_DATA4_CTR     "Cam Data4"
#define D_GPIO_WEBCAM_DATA5_CTR     "Cam Data5"
#define D_GPIO_WEBCAM_DATA6_CTR     "Cam Data6"
#define D_GPIO_WEBCAM_DATA7_CTR     "Cam Data7"
#define D_GPIO_WEBCAM_DATA8_CTR     "Cam Data8"
#define D_GPIO_WEBCAM_XCLK_CTR      "Cam XCLK"
#define D_GPIO_WEBCAM_PCLK_CTR      "Cam PCLK"
#define D_GPIO_WEBCAM_HREF_CTR      "Cam HREF"
#define D_GPIO_WEBCAM_VSYNC_CTR     "Cam VSYNC"
#define D_GPIO_WEBCAM_SIOD_CTR      "Cam SIOD"
#define D_GPIO_WEBCAM_SIOC_CTR      "Cam SIOC"
#define D_GPIO_WEBCAM_PWDN_CTR      "Cam PWDN"

/**
 *  Units
 * */
#define D_UNIT_AMPERE "A"
#define D_UNIT_CENTIMETER "cm"
#define D_UNIT_HERTZ "Hz"
#define D_UNIT_HOUR "Hr"
#define D_UNIT_GALLONS "gal"
#define D_UNIT_GALLONS_PER_MIN "g/m"
#define D_UNIT_INCREMENTS "inc"
#define D_UNIT_KILOGRAM "kg"
#define D_UNIT_KILOMETER_PER_HOUR "km/h"  // or "km/h"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "ug/m3"
#define D_UNIT_MICROMETER "um"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLIMETER "mm"
#define D_UNIT_MILLIMETER_MERCURY "mmHg"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "Min"
#define D_UNIT_PARTS_PER_BILLION "ppb"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "sec"
#define D_UNIT_SECTORS "sectors"
#define D_UNIT_VA "VA"
#define D_UNIT_VAR "VAr"
#define D_UNIT_VOLT "V"
#define D_UNIT_WATT "W"
#define D_UNIT_WATTHOUR "Wh"
#define D_UNIT_WATT_METER_QUADRAT "W/m²"

#define D_CMND_RESET "CommandReset"


// #define ENABLE_FEATURE_LOG_PREFIXES


/**
 *  Log message prefix
 * 
 * Lets add a define that optionally replaces these will nothing so the log window can be simplified when needed
 * */
#ifdef ENABLE_FEATURE_LOG_PREFIXES
  #define D_LOG_APPLICATION "APP: "  // Application
  #define D_LOG_BRIDGE "BRG: "       // Bridge
  #define D_LOG_BUTTONS "BTN: "
  #define D_LOG_CONFIG "CFG: "       // Settings
  #define D_LOG_CELLULAR "CEL: "
  #define D_LOG_CAMERA "CAM: "
  #define D_LOG_POINTERCONFIG "PCFG: "       // Settings
  #define D_LOG_COMMAND "CMD: "      // Command
  #define D_LOG_DEBUG "DBG: "        // Debug
  #define D_LOG_DHT "DHT: "          // DHT sensor
  #define D_LOG_DSB "DSB: "          // DS18xB20 sensor
  #define D_LOG_GARAGE "GAR: "         // relays
  #define D_LOG_HTTP "HTP: "         // HTTP webserver
  #define D_LOG_I2C "I2C: "          // I2C
  #define D_LOG_IRR "IRR: "          // Infra Red Received
  #define D_LOG_LOG "LOG: "          // Logging
  #define D_LOG_MODULE "MOD: "       // Module
  #define D_LOG_MDNS "DNS: "         // mDNS
  #define D_LOG_MQTT "MQT: "         // MQTT
  #define D_LOG_PUBSUB "PSB: "         // PubSub
  #define D_LOG_OTHER "OTH: "        // Other
  #define D_LOG_RESULT "RSL: "       // Result
  #define D_LOG_RFR "RFR: "          // RF Received
  #define D_LOG_SERIAL "SER: "       // Serial
  #define D_LOG_SHT1 "SHT: "         // SHT1x sensor
  #define D_LOG_UPLOAD "UPL: "       // Upload
  #define D_LOG_UPNP "UPP: "         // UPnP
  #define D_LOG_WIFI "WIF: "         // Wifi
  #define D_LOG_NEO "NEO: "         // Neopixels
  #define D_LOG_PIXEL "PIX: "         // Neopixels
  #define D_LOG_LIGHT "LGT: "         // Light interface
  #define D_LOG_DOORBELL "DRB: "         // Neopixels
  #define D_LOG_RELAYS "RLY: "         // relays
  #define D_LOG_TIME2 "TIM2: "         // Time testing v2
  #define D_LOG_TIME "TIM: "         // Time
  #define D_LOG_UPTIME "UPT: "         // Uptime
  #define D_LOG_DHT "DHT: "         // Uptime
  #define D_LOG_BME "BME: "         // Uptime
  #define D_LOG_PZEM "PZM: "         // Uptime
  #define D_LOG_SETTINGS "SET: "         // Uptime
  #define D_LOG_ULTRASONIC "ULT: "         // Uptime
  #define D_LOG_DB18 "DB8: "         // Uptime
  #define D_LOG_HEATINGPANEL "HPL: "         // Uptime
  #define D_LOG_HEATING "HET: "         // Uptime
  #define D_LOG_PIR "PIR: "         // PIR
  #define D_LOG_CEILINGFAN "CFN: "         // Ceiling Fan
  #define D_LOG_RESPONSE "RES: "
  #define D_LOG_CLASSLIST "CLL: "
  #define D_LOG_NEXTION "NEX: "
  #define D_LOG_CHIME "CHM: "         // Uptime
  #define D_LOG_BLINDS "BLD: " 
  #define D_LOG_MEMORY "MEM: "       // 
  #define D_LOG_ASYNC "ASC: "
  #define D_LOG_TEST "TST: "
  #define D_LOG_SDCARD "SDC: "
  #define D_LOG_SWITCHES "SWH: "
  #define D_LOG_RULES "RUL: "
  #define D_LOG_SR04 "SR4: "
  #define D_LOG_FONA "FON: "
  #define D_LOG_BH1750 "BH1: "
#else 
  #define D_LOG_APPLICATION "APP: "  // Application
  #define D_LOG_ASYNC "ASC: "
  #define D_LOG_BH1750 "BH1: "
  #define D_LOG_BLINDS "BLD: "
  #define D_LOG_BME "BME: "         // Uptime
  #define D_LOG_BUTTONS "BTN: "
  #define D_LOG_CAMERA "CAM: "
  #define D_LOG_CEILINGFAN "CFN: "         // Ceiling Fan
  #define D_LOG_CHIME "CHM: "         // Uptime
  #define D_LOG_CLASSLIST "CLL: "
  #define D_LOG_COMMAND "CMD: "      // Command
  #define D_LOG_CONFIG "CFG: "       // Settings
  #define D_LOG_DB18 "DB8: "         // Uptime
  #define D_LOG_DEBUG "DBG: "        // Debug
  #define D_LOG_DHT "DHT: "          // DHT sensor
  #define D_LOG_DOORBELL "DRB: "         // Neopixels
  #define D_LOG_FILESYSTEM "UFS: "
  #define D_LOG_FONA "FON: "
  #define D_LOG_GARAGE "GAR: "         // relays
  #define D_LOG_GPS "GPS: "
  #define D_LOG_HEATING "HET: "         // Uptime
  #define D_LOG_HEATINGPANEL "HPL: "         // Uptime
  #define D_LOG_HTTP "HTP: "         // HTTP webserver
  #define D_LOG_I2C "I2C: "          // I2C
  #define D_LOG_INA219 "INA: "
  #define D_LOG_IRR "IRR: "          // Infra Red Received
  #define D_LOG_LED "LED: "
  #define D_LOG_LIGHT "LGT: "         // Light interface
  #define D_LOG_LOG "LOG: "          // Logging
  #define D_LOG_MEMORY "MEM: "       // 
  #define D_LOG_MDNS "DNS: "         // mDNS
  #define D_LOG_MODULE "MOD: "       // Module
  #define D_LOG_MQTT "MQT: "         // MQTT
  #define D_LOG_NEO "NEO: "         // Neopixels
  #define D_LOG_NEXTION "NEX: "
  #define D_LOG_OTHER "OTH: "        // Other
  #define D_LOG_OTA "OTA: "
  #define D_LOG_POINTERCONFIG "PCFG: "       // Settings
  #define D_LOG_PIR "PIR: "         // PIR
  #define D_LOG_PIXEL // Neopixels
  #define D_LOG_PUBSUB "PSB: "         // PubSub
  #define D_LOG_PZEM "PZM: "         // Uptime
  #define D_LOG_RELAYS "RLY: "         // relays
  #define D_LOG_RESPONSE "RES: "
  #define D_LOG_RESULT "RSL: "       // Result
  #define D_LOG_RFR "RFR: "          // RF Received
  #define D_LOG_RULES "RUL: "
  #define D_LOG_SDCARD "SDC: "
  #define D_LOG_SERIAL "SER: "       // Serial
  #define D_LOG_SETTINGS "SET: "         // Uptime
  #define D_LOG_SHT1 "SHT: "         // SHT1x sensor
  #define D_LOG_SR04 "SR4: "
  #define D_LOG_SUBCOMMAND "SUB: " // Subcommand
  #define D_LOG_SUN_TRACKING "SUN: "
  #define D_LOG_SWITCHES "SWH: "
  #define D_LOG_TELEMETRY "TEL: "
  #define D_LOG_TASKER "TSK: "
  #define D_LOG_TEST "TST: "
  #define D_LOG_TIME "TIM: "         // Time
  #define D_LOG_TIME2 "TIM2: "         // Time testing v2
  #define D_LOG_ULTRASONIC "ULT: "         // Uptime
  #define D_LOG_UPLOAD "UPL: "       // Upload
  #define D_LOG_UPNP "UPP: "         // UPnP
  #define D_LOG_UPTIME "UPT: "         // Uptime
  #define D_LOG_WIFI "WIF: "         // Wifi

#endif // ENABLE_FEATURE_LOG_PREFIXES


//SDM220
#define D_PHASE_ANGLE     "Phase Angle"
#define D_IMPORT_ACTIVE   "Import Active"
#define D_EXPORT_ACTIVE   "Export Active"
#define D_IMPORT_REACTIVE "Import Reactive"
#define D_EXPORT_REACTIVE "Export Reactive"
#define D_TOTAL_REACTIVE  "Total Reactive"
#define D_UNIT_KWARH      "kVArh"
#define D_UNIT_ANGLE      "Deg"


/*********************************************************************************************\
 * All text used in Commands
\*********************************************************************************************/

#define D_ABORTED "Aborted"
#define D_ACTIVE "Active"
#define D_ACCURACY "Accuracy"
#define D_ADDRESS "Address"
#define D_ADDLOG "AddLog"
#define D_AIRQUALITY "AirQuality"
#define D_ANALOG_INPUT "Analog"
#define D_AP "AP"                   // Access Point
#define D_APMAC_ADDRESS "APMac"
#define D_APPENDED "Appended"
#define D_BAUDRATE "Baudrate"
#define D_BLINK "Blink"
#define D_BLOCKED_LOOP "Blocked Loop"
#define D_BOOTVERSION "Boot"
#define D_BOOTCOUNT "BootCount"
#define D_BOOTCOUNTERRORS "BootCountErrors"
#define D_BROKERHOSTNAME "BrokerHostname"
#define D_BSSID "BSSId"
#define D_BUILDDATETIME "BuildDateTime"
#define D_BUILDDATE "BuildDate"
#define D_BUILDTIME "BuildTime"
#define D_CHANNEL "Channel"
#define D_CO2 "CarbonDioxide"
#define D_COMMAND "Command"
#define D_CLEARLOG "ClearLog"
#define D_SETTINGS_HOLDER "SettingsHolder"
#define D_CONNECT_FAILED "Connect failed"
#define D_CONNECTCOUNT "ConnectCount"
#define D_CONNECTS "Connects"
#define D_CORE "Core"
#define D_COUNT "Count"
#define D_COUNTER "Counter"
#define D_CHANNELCOUNT "ChannelCount"
// #define D_DATA "Data"
#define D_DISTANCE "Distance"

#define D_DISPLAY "Display"
#define D_DISPLAY_MODE "DisplayMode"

#define D__IS_SUN_RISING "is_sun_rising"
#define D__DST_ACTIVE "dst_active"
#define D__ANGLES "Angles"
#define D__AZIMUTH "Azimuth"
#define D__ELEVATION "Elevation"
#define D__ELEVATION_MAX "ElevationMax"
#define D__ELEVATION_MIN "ElevationMin"
#define D__ZENITH "Zenith"
#define D__AIR_MASS "AirMass"
#define D__SOLAR_IRRADIANCE "SolarIrradiance"
#define D__DECLINATION_ANGLE "DeclinationAngle"
#define D__DAY_LENGTH "DayLength"
#define D__SHADOW_LENGTH "ShadowLength"
#define D__NONE "none"
#define D__INF "inf"
#define D__UNIX_EVENTS "UnixEvents"
#define D__DAWN "Dawn"
#define D__SUNRISE "Sunrise"
#define D__SOLAR_NOON "SolarNoon"
#define D__SUNSET "Sunset"
#define D__DUSK "Dusk"
#define D__DAYLIGHT_DURATION "DaylightDuration"
#define D__SECONDS_UNTIL "SecondsUntil"
#define D__TIME_UNTIL "TimeUntil"
#define D__DATE_TIME_EVENTS "DateTimeEvents"
#define D__TODAY "Today"
#define D__TOMORROW "Tomorrow"
#define D__NEXT "Next"
#define D__DAYLIGHT_DURATION_DIFF "DaylightDurationDiff"


#define D_DNSSERVER "DNSServer"
#define D_DONE "Done"
#define D_DOWNTIME "Downtime"
#define D_DOWNSECS "DownSecs"
#define D_DRIVERS "Drivers"
#define D_ECO2 "eCO2"
#define D_EMPTY "Empty"
#define D_EVENT "Event"
#define D_MQTT_ENABLE_RESTART "MQTT Enable Restart"
#define D_ENDDST "EndDST"           // End Daylight Savings Time
#define D_ERASE "Erase"
#define D_EVERY "Every"
#define D_EXPORT_ACTIVE "ExportActivePower"
#define D_EXPORT_REACTIVE "ExportReactivePower"
#define D_FAILED "Failed"
#define D_FALLBACKTOPIC "FallbackTopic"
#define D_FEATURES "Features"
#define D_FLASHCHIPID "FlashChipId"
#define D_FLASHMODE "FlashMode"
#define D_FLASHSIZE "FlashSize"
#define D_FLOWRATE "FlowRate"
#define D_FREEMEMORY "FreeMemory"
#define D_FREESKETCHSPACE "FreeSketchSpace"
#define D_FREEHEAP     "FreeHeap"
#define D_FREQUENCY "Frequency"
#define D_FROM "from"
#define D_GAS "Gas"
#define D_GATEWAY "Gateway"
#define D_GROUP "Group"
#define D_GROUPTOPIC "GroupTopic"
#define D_HEAPSIZE "HeapSize"
#define D_HIGH "High"
#define D_HOST_NOT_FOUND "Host not found"
#define D_HSBCOLOR "HSBColor"
#define D_HUMIDITY "Humidity"
#define D_I2CSCAN_DEVICES_FOUND_AT "Device(s) found at"
#define D_I2CSCAN_UNKNOWN_ERROR_AT "Unknown error at"
#define D_I2CSCAN_NO_DEVICES_FOUND "No devices found"
#define D_ID "Id"
#define D_ILLUMINANCE "Illuminance"
#define D_IMPORT_ACTIVE "ImportActivePower"
#define D_IMPORT_REACTIVE "ImportReactivePower"
#define D_INFRARED "Infrared"
#define D_INSIDE "Inside"
#define D_INSTANT "Instant"
#define D_UNKNOWN "Unknown"
#define D_LIGHT "Light"
#define D_LIGHTPOWER "LightPower"
#define D_LINKCOUNT "LinkCount"
#define D_LEVEL "Level"
#define D_LOCAL_TIME "Local"
#define D_LOGLEVEL_SERIAL "LogLevel_Serial"
#define D_LOGLEVELS "LogLevels"
#define D_LOADAVERAGE "LoadAverage"
#define D_LOCATION "Location"
#define D_LOOPDELAY "LoopDelay"
#define D_LOOPSSEC "LoopsSec"
#define D_LOOPRATIO "LoopRatio"
#define D_LIGHT_LEVEL "Light Level"
#define D_LIGHT_LUMINANCE_LUX "Lux"
#define D_LOW "Low"
#define D_MAC "Mac"
#define D_MASK "Mask"
#define D_MDNS "MDNS"
#define D_MESSAGE "Message"
#define D_MINIMAL "minimal"
#define D_MODEL "Model"
#define D_MQTT "MQTT"
#define D_MQTT_COUNT "MqttCount"
#define D_MILLIS "millis"
#define D_NO "No"
#define D_NOISE "Noise"
#define D_NONE "None"
#define D_NETWORK "Network"
#define D_NEGATIVE "Negative"
#define D_NARROW "Narrow"
#define D_OR "or"
#define D_OTAURL "OTAURL"
#define D_OPEN "Open"
#define D_OPENED "Opened"
#define D_OUTSIDE "Outside"
#define D_PERIOD "Period"
#define D_PERCENTAGE "Percentage"
#define D_PERCENT "Percent"
#define D_PHASE_ANGLE "PhaseAngle"
#define D_POWERFACTOR "PowerFactor"
#define D_POWERUSAGE "Power"
#define D_POSITIVE "Positive"
#define D_ACTIVE_POWERUSAGE "ActivePower"
#define D_APPARENT_POWERUSAGE "ApparentPower"
#define D_REACTIVE_POWERUSAGE "ReactivePower"
#define D_PRESSURE "Pressure"
#define D_PRESSUREATSEALEVEL "SeaPressure"
#define D_PRESSURE_UNIT "PressureUnit"
#define D_PROBETEMPERATURE "ProbeTemperature"
#define D_PROGRAMFLASHSIZE "ProgramFlashSize"
#define D_PROGRAMSIZE "ProgramSize"
#define D_REFERENCETEMPERATURE "ReferenceTemperature"
#define D_RELATIVE "Relative"
#define D_RESET "Reset"
#define D_RESOLUTION "Resolution"
#define D_RESTARTING "Restarting"
#define D_RESTARTREASON "RestartReason"
#define D_RSSI "RSSI"
#define D_SAVEADDRESS "SaveAddress"
#define D_SAVECOUNT "SaveCount"
#define D_SAVED "Saved"
#define D_SAVESTATE "SaveState"
#define D_SDKVERSION "SDK"
#define D_SELECTED "selected"
#define D_SERIAL "Serial"
#define D_SLEEP "Sleep"
#define D_SLEEPMODE "SleepMode"
#define D_SENSORS "Sensors"
#define D_SENSORCOUNT "SensorCount"
#define D_SENTCOUNT "SentCount"
#define D_SENTPERMINUTE "SentPerMinute"
#define D_SERIALRECEIVED "SerialReceived"
#define D_SSID "SSID"
#define D_STARTDST "StartDST"       // Start Daylight Savings Time
#define D_STARTED "Started"
#define D_STARTUPUTC "StartupUTC"
#define D_SMOOTH "Smooth"

#define D_MOTION_STARTED "MotionStarted"
#define D_MOTION_ENDED "MotionEnded"

#define D_SPEEDOFSOUND "SpeedOfSound"

#define D_STATIC_IPADDRESS "StaticIPAddress"
#define D_SUBNETMASK "Subnetmask"
#define D_SUCCESSFUL "Successful"
#define D_SUNRISE "Sunrise"
#define D_SUNSET "Sunset"
#define D_SWITCH "Switch"
#define D_SYNC "Sync"
#define D_SYSTEM "System"
#define D_SYSTEM_RESTART "SystemRestart"
#define D_SUN_AZIMUTH "SolarAzimuth"
#define D_SUN_ELEVATION "SolarElevation"
#define D_TEMPERATURE "Temperature"
#define D_TEMPERATURE_UNIT "TempUnit"
#define D_TEMPLATE_USED "TemplateUsed"
#define D_THRESHOLD "Threshold"
#define D_TEMPLATE_PARSE_SUCCESS "TemplateParseSucess"
#define D_TELNET "Telnet"
#define D_TELNET_PORT "TelnetPort"
#define D_TEMPLATE "Template"
#define D_TIMEZONE "TimeZone"
#define D_TODAY "Today"
#define D_TOTAL "Total"
#define D_TOTAL_USAGE "TotalUsage"
#define D_TOTAL_REACTIVE "TotalReactivePower"
#define D_TOTAL_START_TIME "TotalStartTime"
#define D_TVOC "TVOC"
#define D_UPTIME "Uptime"
#define D_UPSECONDS "UpSeconds"
#define D_UPPER "Upper"
#define D_UTC_TIME "UTC"
#define D_UV_INDEX "UvIndex"
#define D_UV_INDEX_TEXT "UvIndexText"
#define D_UV_LEVEL "UvLevel"
#define D_UV_POWER "UvPower"
#define D_VCC "Vcc"
#define D_VALID "Valid"
#define D_VERSION "Version"
#define D_VERSIONNAME "VersionName"
#define D_VERSIONNUMBER "VersionNumber"
#define D_VERSIONNUMBER_MINIMUM "VersionNumberMininum"
#define D_VOLTAGE "Voltage"
#define D_WATCHDOG "WatchDog"
#define D_WEIGHT "Weight"
#define D_WEB "Web"
#define D_WIFI "Wifi"
#define D_WIDE "Wide"
#define D_WRONG "Wrong"
#define D_WRONG_PARAMETERS "Wrong parameters"
#define D_WEBSERVER_ENABLED "Webserver Enabled"
#define D_WIFICONFIG_STATE "WiFiConfig State"
#define D_YESTERDAY "Yesterday"
#define D_ZERO_POINT_CALIBRATION "Zero Point Calibration"
#define D_ALTITUDE "Altitude"
#define D_AGE "Age"
#define D_ANY "Any"
#define D_ACTIVE_POWER "ActivePower"
#define D_CCT_PERCENTAGE "CCT_TempPercentage"
#define D_CCT_TEMP "CCT_Temp"

#define D_ULTRASONIC_DISTANCE_CM "UltrasonicDistanceCM"

#define D_RGBCCT_LINKED "rgbcct_linked"

#define D_BRIGHTNESS "Brightness"
#define D_BRIGHTNESS_255 "Brightness_255"
#define D_BRIGHTNESS_PERCENTAGE "brightness_percentage"
#define D_CLIMATE "climate"
#define D_CLASSID "ClassID"
#define D_CLOSED "Closed"

#define D_CURRENT "Current"
#define D_CRASHDUMP "CrashDump"
#define D_COLOUR_PALETTE "ColourPalette"
#define D_DATA "Data"
#define D_DATETIME "DateTime"
#define D_DESIRED "Desired"
#define D_DEVICE "Device"
#define D_DEVICEID "DeviceID"
#define D_DEVICES_CONNECTED "DevicesConnected"

#define D_SCENE "Scene"
#define D_COLOUR "Colour"

#define D_SENSOR_TYPE "SensorType"



#define D_HVAC_DEVICE "HeatingDevice"

#define D_DEVIATION "Deviation"

#define D_DEVICENAME "DeviceName"
#define D_DEVICEFRIENDLYNAME "DeviceFriendlyName"

#define D_DOOR "Door"
#define D_RINGDOORBELL "DoorbellRing"
#define D_DURATION "Duration"
#define D_DURATION_RAW "DurationRaw"
#define D_ERROR "Error"
#define D_EFFECTS "Effects"
#define D_EFFECT "Effect"
#define D_FUNCTION "Function"

#define D_ENABLED "Enabled"

#define D_PIXELGROUPING "PixelGrouping"
#define D_PIXELRANGE "PixelRange"
#define D_MULTIPLIER "Multiplier"

#define D_BRIGHTNESS_MIN "BrightnessMin"
#define D_BRIGHTNESS_MAX "BrightnessMax"
#define D_BRIGHTNESS_RGB "BrightnessRGB"
#define D_BRIGHTNESS_RGB_255 "BrightnessRGB_255"
#define D_BRIGHTNESS_CCT  "BrightnessCCT"
#define D_BRIGHTNESS_CCT_255  "BrightnessCCT_255"
#define D_RANDOM_AMOUNT "RandomAmount"
#define D_RANDOMISE_BRIGHTNESS_MODE "RandomiseBrightnessMode"

#define D_RUNNING_ID "RunningID"

#define D_SEQUENTIAL "Sequential"
#define D_DIRECTION "Direction"

#define D_RGB_DATA_STREAM "RGBDataStream"

#define D_ALTERNATE "Alternate"
#define D_ALTERNATE_AMOUNT "AlternateAmount"
#define D_ALTERNATE_BRIGHTNESS_MIN "AlternateBrightnessMin"
#define D_ALTERNATE_BRIGHTNESS_MAX "AlternateBrightnessMax"

#define D_COLOUR_REFRESH_RATE "ColourRefreshRate"

#define D_RUNTIME_DURATION_SCALER_PERCENTAGE "run_time_duration_scaler_as_percentage"

#define D_FAILSAFE "failsafe"
#define D_FUNCTIONS "functions"
#define D_HUM "hum"
#define D_HUE "Hue"
#define D_INDEX "Index"
#define D_ISCHANGED "ischanged"
#define D_ISCHANGEDMETHOD "IsChangedMethod"
#define D_ISRUNNING "isrunning"
#define D_ISVALID "IsValid"
#define D_CAPTURE_UPSECONDS "CaptureUpSeconds"
#define D_IMPORTANCE "importance"
#define D_INORDER "InOrder"
#define D_INTENSITY "Intensity"
#define D_LIMIT "Limit"
#define D_LASTREAD "LastRead"
#define D_LENGTH "length"
#define D_MAX "max"
#define D_MESSAGE "Message"
#define D_METHOD "method"
#define D_MINUTES "minutes"
#define D_MIN "min"
#define D_MODE "Mode"
#define D_MIXER "Mixer"
#define D_MOTION "Motion"
#define D_MODULENAME "ModuleName"
#define D_MODULEID "ModuleID"
#define D_MINUTES_ON "minutes_on"
#define D_NAME "Name"

#define D_GROUPING "Grouping"
#define D_DECIMATE "Decimate"
#define D_SPACING "Spacing"
#define D_REVERSE "Reverse"
#define D_MIRROR "Mirror"
#define D_OFFSET "Offset"

#define D_HOLD "Hold"

#define D_MEMORY_INSUFFICIENT "Memory Insufficient"

#define D_SCENENAME "SceneName"
#define D_ANIMATIONMODE "AnimationMode"

#define D_MAPPED_MULTIPLIER_DATA "MappedMultiplierData"

#define D_NOTIFICATIONS "Notifications"

#define D_POWERNAME "PowerName"
#define D_RELAY "Relay"

#define D_ANIMATIONENABLE "AnimationEnable"
#define D_AGED_COLOURING "AgedColouring"

#define D_ON "On"
// #define D_ONTIME "ontime"
#define D_ONOFF "OnOff"
#define D_ONOFF_NAME "OnOffName"
#define D_OFF "Off"
// #define D_OFFTIME "offtime"
#define D_ORDER "Order"
#define D_OUTLIERS "Outliers"
#define D_PACKETS "Packets"
#define D_PIXELSGROUPED "pixels_grouped"

#define D_LAST "Last"
#define D_ONTIME "OnTime"
#define D_OFFTIME "OffTime"

#define D_OVERRIDE "Override"

#define D_PIXELS_UPDATE_NUMBER "PixelUpdateNum"
#define D_PIXELS_UPDATE_PERCENTAGE "PixelUpdatePerc"

#define D_POWER_FACTOR "PowerFactor"

#define D_PIXELNUM "pixelnum"
#define D_PULSER "pulser"
#define D_REFRESH_ALLPIXELS "refresh_allpixels"

#define D_RATE "Rate"
#define D_RATE_MS "RateMs"
#define D_RATE_SECS "RateSecs"


#define D_RECEIVEDCOUNT "ReceivedCount"
#define D_RATIO "Ratio"

#define D_RELAY_NAME "Relay Name"

#define D_ROOMHINT "RoomHint"

#define D_MOTION_NAME "Motion Name"

#define D_SAT "Sat"
#define D_SAT255 "Sat255"
#define D_SECONDS "seconds"
#define D_SET "Set"
#define D_SENS1 "sens1"
#define D_SENSORADDRESS "SensorAddress"
#define D_SECS "Secs"
#define D_SIZE "size"
#define D_SIGNIFICANTLY "significantly"
#define D_STEP "step"
#define D_STATUS "Status"

#define D_SENSOR_MEASURERATE "Sensor_MeasureRate"

#define D_SCENE "Scene"
#define D_SCENE_COLOUR "SceneColour"
#define D_SCENE_NAME   "SceneName"

#define D_SCHEDULE "Schedule"

#define D_SCHEDULED_OFF D_OFF
#define D_SCHEDULED_SET "Scheduled Set"
#define D_SCHEDULED_ON "Scheduled ON"
#define D_MANUAL_ON "Manual ON"

#define D_TIME "Time"
#define D_TIME_MS "TimeMs"
#define D_TIME_SECS "TimeSecs"
#define D_TIME_ON "TimeOn"
#define D_ADD_TIME_ON "AddTimeOn"
#define D_TIME_ON_SECS "TimeOnSecs"
#define D_TIME_OFF_THEN_ON_SECS "TimeOffThenOnSecs"
#define D_TIME_ON_MINUTES "TimeOnMins"
#define D_TIME_ON_MS "TimeOnMs"
#define D_TIME_DELAYED "TimeDelayed"
#define D_TIME_MAINTAINING "TimeMaintaining"
#define D_TIME_RUNNING "TimeRunning"
#define D_TIME_TO_HEAT "TimeToHeat"
#define D_TEMP "Temp"
#define D_TRANSITION "Transition"
#define D_TYPE "Type"
#define D_TIME_ELASPED "TimeElapsed"
#define D_RELAY "Relay"

#define D_SUBTYPE "SubType"

#define D_MANUAL_SETPIXEL_TO_SCENE "ManualSetPixelToScene"

#define D_TIME_SCALER_AS_PERCENTAGE "TimeMultiplierAsPerc"

#define D_EXTERNAL_POWER_ONOFF "ExternalPowerOnOff"



#define D_RGB "RGB"
#define D_RBG "RBG"
#define D_GRB "GRB"

#define D_HSB "HSB"

#define D_RANDOM "Random"
#define D_VALUE "Value"


#define D_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR "GRB"

#define D_PALETTE_EDIT "PaletteEdit"
#define D_PALETTE_GENERATION "PaletteGeneration"

#define D_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR "RGB"


#define D_EFFECT_COLOUR_TYPE "ColourType" // rename later
#define D_COLOUR_TYPE "ColourType"  //phase out


#define D_HARDWARE_TYPE "HardwareType"
#define D_STRIP_SIZE "LightSize"
#define D_RGB_COLOUR_ORDER "ColourOrder"
#define D_RGB_CLOCK "RGBClock"
#define D_MANUAL_NUMBER "ManualNumber"
#define D_STRIP_REPEAT "strip_repeat"

#define D_WS2812       "WS2812"



#define D_RSLT_ENERGY "ENERGY"
#define D_RSLT_HASS_STATE "HASS_STATE"
#define D_RSLT_INFO "INFO"
#define D_RSLT_MARGINS "MARGINS"
#define D_RSLT_POWER "POWER"
#define D_RSLT_RESULT "RESULT"
#define D_RSLT_SENSOR "SENSOR"
#define D_RSLT_STATE "STATE"
#define D_RSLT_UPTIME "UPTIME"
#define D_RSLT_WARNING "WARNING"

// Commands sonoff.ino
#define D_BACKLOG "Backlog"
#define D_DELAY "Delay"
#define D_STATUS "Status"
#define D_STATE "State"
#define D_POWER "Power"
#define D_FANSPEED "FanSpeed"
#define D_FANTIMER "FanTimer"
#define D_FANSPEED_MANUAL "FanSpeed_Manual"
#define D_FANPWM "FanPWM"
#define D_POWERONSTATE "PowerOnState"
#define D_POWER_STATE "PowerState"
#define D_POWER_STATE_NAME "PowerStateName"
#define D_PULSETIME "PulseTime"
#define D_BLINKTIME "BlinkTime"
#define D_BLINKCOUNT "BlinkCount"
#define D_SENSOR "Sensor"
#define D_SENSOR_COUNT "SensorCount"
#define D_DRIVER "Driver"
#define D_SAVEDATA "SaveData"
#define D_SETOPTION "SetOption"
#define D_TEMPERATURE_RESOLUTION "TempRes"
#define D_HUMIDITY_RESOLUTION "HumRes"
#define D_PRESSURE_RESOLUTION "PressRes"
#define D_POWER_RESOLUTION "WattRes"
#define D_VOLTAGE_RESOLUTION "VoltRes"
#define D_FREQUENCY_RESOLUTION "FreqRes"
#define D_CURRENT_RESOLUTION "AmpRes"
#define D_ENERGY_RESOLUTION "EnergyRes"
#define D_WEIGHT_RESOLUTION "WeightRes"
#define D_MODULE "Module"
#define D_MODULES "Modules"
  #define D_NOT_SUPPORTED "Not supported"
#define D_GPIOS "GPIOs"
#define D_PWM "PWM"
#define D_PWMFREQUENCY "PWMFrequency"
#define D_PWMRANGE "PWMRange"
#define D_COUNTER "Counter"
#define D_COUNTERTYPE "CounterType"
#define D_COUNTERDEBOUNCE "CounterDebounce"
#define D_BUTTONDEBOUNCE "ButtonDebounce"
#define D_SWITCHDEBOUNCE "SwitchDebounce"
#define D_SLEEP "Sleep"
#define D_UPLOAD "Upload"
#define D_UPGRADE "Upgrade"
  #define D_ONE_OR_GT "1 or >%s to upgrade"
// #define D_OTAURL "OtaUrl"
#define D_SERIALLOG "SerialLog"
#define D_SYSLOG "SysLog"
#define D_LOGHOST "LogHost"
#define D_LOGPORT "LogPort"
#define D_LOWER "Lower"
#define D_ID "ID"
#define D_IPADDRESS "IPAddress"
#define D_NTPSERVER "NtpServer"
// #define D_AP "Ap"
#define D_SSID "SSID"
#define D_SSID_NUMBERED "SSID_Number"
#define D_PASSWORD "Password"
#define D_HOSTNAME "Hostname"
#define D_WIFICONFIG "WifiConfig"
  #define WCFG_MAX_STRING_LENGTH 12
  #define D_WCFG_0_RESTART "Restart"
  #define D_WCFG_1_SMARTCONFIG "SmartConfig"
  #define D_WCFG_2_WIFIMANAGER "WifiManager"
  #define D_WCFG_3_WPSCONFIG "WPSConfig"
  #define D_WCFG_4_RETRY "Retry"
  #define D_WCFG_5_WAIT "Wait"
  #define D_WCFG_6_SERIAL "Serial"
  #define D_WCFG_7_WIFIMANAGER_RESET_ONLY "ManagerRst"
  
#define D_WIFIPOWER "WifiPower"

#define D_WIFI_AP "WifiAP"

#define D_ROOMHINT "RoomHint"

#define D_FRIENDLYNAME "FriendlyName"
#define D_SWITCHMODE "SwitchMode"
#define D_INTERLOCK "Interlock"
#define D_TELEPERIOD "TelePeriod"
#define D_RESTART "Restart"
  #define D_ONE_TO_RESTART "1 to restart"
#define D_RESET "Reset"
  #define D_RESET_AND_RESTARTING "Reset and Restarting"
  #define D_ONE_TO_RESET "1 to reset"
// #define D_TIMEZONE "Timezone"
#define D_TIMESTD "TimeStd"
#define D_TIMEDST "TimeDst"
// #define D_ALTITUDE "Altitude"
#define D_LEDPOWER "LedPower"
#define D_LEDSTATE "LedState"
#define D_LEDMASK "LedMask"
#define D_I2CSCAN "I2CScan"
#define D_SERIALSEND "SerialSend"
#define D_SERIALDELIMITER "SerialDelimiter"
#define D_BAUDRATE "Baudrate"
#define D_TEMPLATE "Template"
  #define D_GPIO "GPIO"

  // [N]ame vs [F]unction as keys
  #define D_GPIO_NUMBER   "GPION"
  #define D_GPIO_FUNCTION "GPIOF"

  
  #define D_GPIOC D_GPIO_NUMBER //temporary until phased over

  #define D_FLAG "FLAG"
#define D_BASE "Base"
#define D_GAS_RESISTANCE "GasResistance"

// Commands xdrv_01_mqtt.ino
#define D_MQTTHOST "MqttHost"
#define D_MQTTPORT "MqttPort"
#define D_MQTTRETRY "MqttRetry"
#define D_STATETEXT "StateText"
#define D_MQTTFINGERPRINT "MqttFingerprint"
#define D_MQTTCLIENT "MqttClient"
#define D_MQTTUSER "MqttUser"
#define D_MQTTPASSWORD "MqttPassword"
#define D_FULLTOPIC "FullTopic"
#define D_GROUPTOPIC "GroupTopic"
#define D_TOPIC "Topic"
#define D_BUTTONTOPIC "ButtonTopic"
#define D_SWITCHTOPIC "SwitchTopic"
#define D_BUTTONRETAIN "ButtonRetain"
#define D_SWITCHRETAIN "SwitchRetain"
#define D_POWERRETAIN "PowerRetain"
#define D_SENSORRETAIN "SensorRetain"
#define D_PUBLISH "Publish"

#define D_DOOR_POSITION "DoorPosition"
#define D_DOOR_LOCKED "DoorLocked"

// Commands xdrv_02_webserver.ino
#define D_WEBSERVER "Webserver"
  #define D_WEBSERVER_MODE "WebServerMode"
  #define D_ACTIVE_FOR "Active for"
  #define D_ON_DEVICE "on"
  #define D_WITH_IP_ADDRESS "with IP address"
#define D_WEBPASSWORD "WebPassword"
#define D_WEBLOG "WebLog"
#define D_WEBREFRESH "WebRefresh"
#define D_WEBSEND "WebSend"
#define D_WEBCOLOR  "WebColor"
#define D_EMULATION "Emulation"

// Commands xdrv_03_energy.ino
#define D_POWERLOW "PowerLow"
#define D_POWERHIGH "PowerHigh"
#define D_POWERDELTA "PowerDelta"
#define D_VOLTAGELOW "VoltageLow"
#define D_VOLTAGEHIGH "VoltageHigh"
#define D_CURRENTLOW "CurrentLow"
#define D_CURRENTHIGH "CurrentHigh"
#define D_ENERGYRESET "EnergyReset"
#define D_POWERSET "PowerSet"
#define D_VOLTAGESET "VoltageSet"
#define D_CURRENTSET "CurrentSet"
#define D_FREQUENCYSET "FrequencySet"
#define D_MAXPOWER "MaxPower"
#define D_MAXPOWERHOLD "MaxPowerHold"
#define D_MAXPOWERWINDOW "MaxPowerWindow"
  #define D_MAXPOWERREACHED "MaxPowerReached"
  #define D_MAXPOWERREACHEDRETRY "MaxPowerReachedRetry"
#define D_SAFEPOWER "SafePower"
#define D_SAFEPOWERHOLD "SafePowerHold"
#define D_SAFEPOWERWINDOW "SafePowerWindow"
  #define D_POWERMONITOR "PowerMonitor"
#define D_MAXENERGY "MaxEnergy"
#define D_MAXENERGYSTART "MaxEnergyStart"
  #define D_ENERGYMONITOR "EnergyMonitor"
  #define D_ENERGY "Energy"
  #define D_ENERGYLAST "EnergyLast"
  #define D_MAXENERGYREACHED "MaxEnergyReached"

  #define D_KWH_STATS "kwh_stats"

// Commands xdrv_04_light.ino
#define D_CHANNEL "Channel"
#define D_COLOR "Color"
#define D_COLORTEMPERATURE "CT"
#define D_DIMMER "Dimmer"
#define D_HSBCOLOR "HSBColor"
#define D_LED "Led"
#define D_LEDTABLE "LedTable"
#define D_FADE "Fade"
#define D_PIXELS "Pixels"
#define D_RGBWWTABLE "RGBWWTable"
#define D_ROTATION "Rotation"
#define D_SCHEME "Scheme"
#define D_SPEED "Speed"
#define D_WAKEUP "Wakeup"
#define D_WAKEUPDURATION "WakeUpDuration"
#define D_WHITE "White"
#define D_WIDTH "Width"

// Commands xdrv_05_irremote.ino
#define D_IRSEND "IRSend"
  #define D_INVALID_JSON "Invalid JSON"
  #define D_INVALID_RAWDATA "Invalid RawData"
  #define D_NO_BUFFER_SPACE "No buffer space"
  #define D_PROTOCOL_NOT_SUPPORTED "Protocol not supported"
  #define D_IR_PROTOCOL "Protocol"
  #define D_IR_BITS "Bits"
  #define D_IR_DATA "Data"
  #define D_IR_RAWDATA "RawData"
#define D_IRHVAC "IRHVAC"
  #define D_IRHVAC_VENDOR "VENDOR"
  #define D_IRHVAC_POWER "POWER"
  #define D_IRHVAC_MODE "MODE"
  #define D_IRHVAC_FANSPEED "FANSPEED"
  #define D_IRHVAC_TEMP "TEMP"
#define D_IRRECEIVED "IrReceived"

// Commands xdrv_06_snfbridge.ino
#define D_RFCODE "RfCode"
#define D_RFHIGH "RfHigh"
#define D_RFHOST "RfHost"
#define D_RFKEY "RfKey"
  #define D_START_LEARNING "Start learning"
  #define D_SET_TO_DEFAULT "Set to default"
  #define D_DEFAULT_SENT "Default sent"
  #define D_LEARNED_SENT "Learned sent"
  #define D_LEARNING_ACTIVE "Learning active"
  #define D_LEARN_FAILED "Learn failed"
  #define D_LEARNED "Learned"
#define D_RFLOW "RfLow"
#define D_RFSYNC "RfSync"
  #define D_RFRECEIVED "RfReceived"
#define D_RFRAW "RfRaw"

// Commands xdrv_08_serial_bridge.ino
#define D_SSERIALSEND "SSerialSend"
#define D_SBAUDRATE "SBaudrate"
  #define D_SSERIALRECEIVED "SSerialReceived"

// Commands xdrv_09_timers.ino
#define D_TIMER "Timer"
  #define D_TIMER_ARM "Arm"
  #define D_TIMER_MODE "Mode"
  #define D_TIMER_TIME "Time"
  #define D_TIMER_WINDOW "Window"
  #define D_TIMER_DAYS "Days"
  #define D_TIMER_REPEAT "Repeat"
  #define D_TIMER_OUTPUT "Output"
  #define D_TIMER_ACTION "Action"
  #define D_TIMER_NO_DEVICE "No GPIO as output configured"
#define D_TIMERS "Timers"
#define D_LATITUDE "Latitude"
#define D_LONGITUDE "Longitude"

#define D_DESERIALIZATION_ERROR "DeserializationError"

  
  #define            D_MODULE_NAME_NODEMCU_CTR            "NodeMCU"
  #define            D_MODULE_NAME_USERMODULE_CTR            "User Module"
  #define            D_MODULE_NAME_SONOFF_BASIC_CTR       "Sonoff Basic"
  #define            D_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR       "Sonoff BasEx"
  #define            D_MODULE_NAME_H801_CTR               "H801"
  #define            D_MODULE_NAME_MAGICHOME_CTR          "MagicHome"
  #define            D_MODULE_NAME_SHELLY1_CTR       "Shelly 1"
  #define            D_MODULE_NAME_SHELLY2P5_CTR       "Shelly 2"
  #define            D_MODULE_NAME_SHELLY_DIMMER2_CTR       "Shelly Dimmer 2"
  #define            D_MODULE_NAME_SONOFF_4CHPRO_CTR       "Sonoff 4CHPRO"
  #define            D_MODULE_NAME_SONOFF_IFAN03_CTR       "Sonoff IFAN03"


/********************************************************************************************/

#define D_ASTERIX "********"
#define D_ASTERISK_PWD "****"

#ifndef MY_LANGUAGE
  //#include "//2_CoreSystem/11_Languages/mLanguage.h"
#else
  #define QUOTEME(x) QUOTEME_1(x)
  #define QUOTEME_1(x) #x
  #define INCLUDE_FILE(x) QUOTEME(language/x.h)
  #include INCLUDE_FILE(MY_LANGUAGE)
#endif


#endif  // _I18N_H_
