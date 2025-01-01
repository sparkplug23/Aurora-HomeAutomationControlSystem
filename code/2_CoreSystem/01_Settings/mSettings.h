#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define D_UNIQUE_MODULE_CORE_SETTINGS_ID 2001 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/mBaseConfig.h"

#define DATA_BUFFER_TOPIC_MAX_LENGTH    100

#ifdef USE_MODULE_NETWORK_WEBSERVER
  #ifndef DATA_BUFFER_PAYLOAD_MAX_LENGTH
    #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
  #endif
#else
  #ifndef DATA_BUFFER_PAYLOAD_MAX_LENGTH
    #ifdef ESP32
      #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000
    #else
      #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 2000
    #endif
  #endif
#endif //USE_MODULE_NETWORK_WEBSERVER


typedef union {
  uint16_t data;
  struct {
    uint16_t hemis : 1;                    // bit 0        = 0=Northern, 1=Southern Hemisphere (=Opposite DST/STD)
    uint16_t week : 3;                     // bits 1 - 3   = 0=Last week of the month, 1=First, 2=Second, 3=Third, 4=Fourth
    uint16_t month : 4;                    // bits 4 - 7   = 1=Jan, 2=Feb, ... 12=Dec
    uint16_t dow : 3;                      // bits 8 - 10  = day of week, 1=Sun, 2=Mon, ... 7=Sat
    uint16_t hour : 5;                     // bits 11 - 15 = 0-23
  };
} TimeRule;


enum DATA_BUFFER_FLAG_SOURCE_IDS
{
  DATA_BUFFER_FLAG_SOURCE_MQTT=0,
  DATA_BUFFER_FLAG_SOURCE_WEBUI
};


typedef union 
{
  uint16_t data;
  struct { 
    // 3 bits (9 values)
    // (0) DATA_BUFFER_FLAG_SOURCE_MQTT  // Defaulted to 0
    // (1) DATA_BUFFER_FLAG_SOURCE_WEBUI
    uint16_t source_id : 4;
    // Waiting
    uint16_t waiting : 1;
    // Encoding format
    uint16_t encoded_type_id : 1; // json,raw
    uint16_t reserved : 10;
  };
} DATA_BUFFER_FLAGS;


struct DATA_BUFFER{
  struct TOPIC{
    char ctr[DATA_BUFFER_TOPIC_MAX_LENGTH];
    uint16_t length_used = 0;
  }topic;
  struct PAYLOAD{
    char ctr[DATA_BUFFER_PAYLOAD_MAX_LENGTH];
    uint16_t length_used = 0;
  }payload;
  uint16_t isserviced = 0; // Set to 0 on new mqtt
  uint16_t moduleLock = 0;
  DATA_BUFFER_FLAGS flags;
};
extern struct DATA_BUFFER data_buffer;

#include <Arduino.h> // Include Arduino header for millis() and delay()

#define ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING

#ifdef ENABLE_DEVFEATURE_DATABUFFER_LOCK

//threading/network callback details: https://github.com/Aircoookie/WLED/pull/2336#discussion_r762276994
inline bool requestDataBufferLock(uint16_t module)
{
  unsigned long now = millis();

  // This assumption here is another http thread must release itself to permit this function to proceed
  while (data_buffer.moduleLock && millis()-now < 1000) delay(1); // wait for a second for buffer lock

  if (millis()-now >= 1000) 
  {
    #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
    Serial.printf(PSTR("ERROR: Locking data buffer failed! (%d)\n\r"),data_buffer.moduleLock);
    #endif
    return false; // waiting time-outed
  }

  data_buffer.moduleLock = module ? module : 255;

  #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
  Serial.printf(PSTR("DATA buffer locked (%d)\n\r"), data_buffer.moduleLock);
  #endif

  return true;
}

inline void releaseDataBufferLock()
{
  #ifdef ENABLE_FEATURE_DATABUFFER__LOCK_LOGGING
  Serial.printf(PSTR("DATA buffer released (%d)\n\r"), data_buffer.moduleLock);
  #endif
  data_buffer.moduleLock = 0;
}

#else

//threading/network callback details: https://github.com/Aircoookie/WLED/pull/2336#discussion_r762276994
inline bool requestDataBufferLock(uint16_t module){ return true; };
inline void releaseDataBufferLock(){};

#endif


/**
 * @brief Complete, slow
 **/
#define D_DATA_BUFFER_CLEAR()             \
    memset(&data_buffer,0,sizeof(data_buffer))
/**
 * @brief Minimal, fast
 **/
#define D_DATA_BUFFER_SOFT_CLEAR()             \
    data_buffer.topic.ctr[0]=0;           \
    data_buffer.topic.length_used = 0;    \
    data_buffer.payload.ctr[0]=0;         \
    data_buffer.payload.length_used = 0; 


// Easy way to add to the counter
#define D_MQTT_COMMAND_HANDLED_COUNT_INC data_buffer.isserviced++


#include "2_CoreSystem/06_Support/mSupport.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/05_HardwarePins/mHardwarePins_Templates.h"

#include "2_CoreSystem/05_HardwarePins/mHardwarePins.h"

#include "2_CoreSystem/mFirmwareDefaults.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/07_Time/mTime.h"


#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  // #include "avr/pgmspace.h"
  #if (defined(__AVR__))
  #include <avr\pgmspace.h>
  #else
  #include <pgmspace.h>
  #endif
#endif

#ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
// Handle 20k of NVM
#include <nvs_flash.h>
#include <nvs.h>
#endif // ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY


#ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
enum SettingsTextIndex { 
    SET_OTAURL,
    SET_STASSID1, SET_STASSID2,  // MAX_SSIDS
    SET_STAPWD1, SET_STAPWD2,  // MAX_SSIDS
    SET_HOSTNAME, SET_SYSLOG_HOST,
    SET_WEBPWD, SET_CORS,
    SET_STATE_TXT1, SET_STATE_TXT2, SET_STATE_TXT3, SET_STATE_TXT4,  // MAX_STATE_TEXT
    SET_NTPSERVER1, SET_NTPSERVER2, SET_NTPSERVER3,  // MAX_NTP_SERVERS
    SET_MEM1, SET_MEM2, SET_MEM3, SET_MEM4, SET_MEM5, SET_MEM6, SET_MEM7, SET_MEM8,
    SET_MEM9, SET_MEM10, SET_MEM11, SET_MEM12, SET_MEM13, SET_MEM14, SET_MEM15, SET_MEM16,  // MAX_RULE_MEMS
    SET_FRIENDLYNAME1, SET_FRIENDLYNAME2, SET_FRIENDLYNAME3, SET_FRIENDLYNAME4,
    SET_FRIENDLYNAME5, SET_FRIENDLYNAME6, SET_FRIENDLYNAME7, SET_FRIENDLYNAME8,  // MAX_FRIENDLYNAMES
    SET_BUTTON1, SET_BUTTON2, SET_BUTTON3, SET_BUTTON4, SET_BUTTON5, SET_BUTTON6, SET_BUTTON7, SET_BUTTON8,
    SET_BUTTON9, SET_BUTTON10, SET_BUTTON11, SET_BUTTON12, SET_BUTTON13, SET_BUTTON14, SET_BUTTON15, SET_BUTTON16,  // MAX_BUTTON_TEXT
    SET_TEMPLATE_NAME,
    SET_DEV_GROUP_NAME1, SET_DEV_GROUP_NAME2, SET_DEV_GROUP_NAME3, SET_DEV_GROUP_NAME4,  // MAX_DEV_GROUP_NAMES
    SET_DEVICENAME,
    SET_TELEGRAM_TOKEN, SET_TELEGRAM_CHATID,
  #ifdef ESP8266
    SET_ADC_PARAM1,
    SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
  #endif  // ESP8266
  #ifdef ESP32
    SET_ADC_PARAM1, SET_ADC_PARAM2, SET_ADC_PARAM3, SET_ADC_PARAM4, SET_ADC_PARAM5, SET_ADC_PARAM6, SET_ADC_PARAM7, SET_ADC_PARAM8,  // MAX_ADCS
    SET_SWITCH_TXT1, SET_SWITCH_TXT2, SET_SWITCH_TXT3, SET_SWITCH_TXT4, SET_SWITCH_TXT5, SET_SWITCH_TXT6, SET_SWITCH_TXT7, SET_SWITCH_TXT8,  // MAX_SWITCHES_TXT
    SET_SWITCH_TXT9, SET_SWITCH_TXT10, SET_SWITCH_TXT11, SET_SWITCH_TXT12, SET_SWITCH_TXT13, SET_SWITCH_TXT14, SET_SWITCH_TXT15, SET_SWITCH_TXT16,  // MAX_SWITCHES_TXT
    SET_SWITCH_TXT17, SET_SWITCH_TXT18, SET_SWITCH_TXT19, SET_SWITCH_TXT20, SET_SWITCH_TXT21, SET_SWITCH_TXT22, SET_SWITCH_TXT23, SET_SWITCH_TXT24,  // MAX_SWITCHES_TXT
    SET_SWITCH_TXT25, SET_SWITCH_TXT26, SET_SWITCH_TXT27, SET_SWITCH_TXT28,  // MAX_SWITCHES_TXT
  #endif  // ESP32
    SET_SHD_PARAM,
    SET_RGX_SSID, SET_RGX_PASSWORD,
    SET_INFLUXDB_HOST, SET_INFLUXDB_PORT, SET_INFLUXDB_ORG, SET_INFLUXDB_TOKEN, SET_INFLUXDB_BUCKET, SET_INFLUXDB_RP,
    SET_CANVAS,
    SET_TELEGRAM_FINGERPRINT,
    SET_MAX, // limit of texts stored in Settings
    // Index above are not stored in Settings and should be handled specifically in SettingText()
    SET_BUTTON17, SET_BUTTON18, SET_BUTTON19, SET_BUTTON20, SET_BUTTON21, SET_BUTTON22, SET_BUTTON23, SET_BUTTON24,
    SET_BUTTON25, SET_BUTTON26, SET_BUTTON27, SET_BUTTON28, SET_BUTTON29, SET_BUTTON30, SET_BUTTON31, SET_BUTTON32,
    SET_FINAL_MAX
   };
#endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER


enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|Timer|Rule|MaxPower|MaxEnergy|Light|Knx|Display|Wemo|Hue|Retry";


enum DATABUILDER_JSON_LEVEL{ //in order of importance
  JSON_LEVEL_NONE=0,
  // Only show what has changed over mqtt
  JSON_LEVEL_IFCHANGED,
  // Used to show in serial the most basic stuff anytime it is called (ie above IFCHANGED) but not as detailed as short would be 
  // Basic and short may be the same, so remove base
  JSON_LEVEL_SHORT,
  JSON_LEVEL_DETAILED,
  JSON_LEVEL_ALL,
  JSON_LEVEL_DEBUG // Share extra info relating to the generation of the json
};


DEFINE_PGM_CTR(PM_LEVEL_NONE_CTR)        "None";
DEFINE_PGM_CTR(PM_LEVEL_IFCHANGED_CTR)   "IfChanged";
DEFINE_PGM_CTR(PM_LEVEL_SHORT_CTR)       "Short";
DEFINE_PGM_CTR(PM_LEVEL_DETAILED_CTR)    "Detailed";
DEFINE_PGM_CTR(PM_LEVEL_ALL_CTR)         "All";
DEFINE_PGM_CTR(PM_LEVEL_DEBUG_CTR)       "Debug";
  

const uint32_t settings_text_size = 699;   // Settings->text_pool[size] = Settings->display_model (2D2) - Settings->text_pool (017)
const uint8_t MAX_TUYA_FUNCTIONS = 16;
const uint8_t PARAM8_SIZE = 18;            // Number of param bytes (SetOption)


/*********************************************************************************************\
 * Power Type
\*********************************************************************************************/

typedef unsigned long power_t;              // Power (Relay) type

/*********************************************************************************************\
 * Constants
\*********************************************************************************************/

/**
 * @brief 
 * I need to recheck these sizes match my GPIO enums, as its causing issues
 * 
 */


const uint8_t MAX_RELAYS_SET = 32;          // Max number of relays
const uint8_t MAX_KEYS_SET = 32;            // Max number of keys
// Changes to the following MAX_ defines will impact settings layout
const uint8_t MAX_INTERLOCKS_SET = 14;      // Max number of interlock groups (MAX_RELAYS_SET / 2)
const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
const uint8_t MAX_DIGITAL_LEDS = 8;                 // MAX_LEDS Max number of leds
const uint8_t MAX_PWMS_LEGACY = 5;          // Max number of PWM channels in first settings block - Legacy limit for ESP8266, but extended for ESP32 (see below)
#ifdef ESP32                                // Max number of PWM channels (total including extended) - ESP32 only
  #if CONFIG_IDF_TARGET_ESP32
  const uint8_t MAX_PWMS = 5;              // ESP32: 16 ledc PWM channels in total - TODO for now
  #elif CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
  const uint8_t MAX_PWMS = 8;               // ESP32S2/S3: 8 ledc PWM channels in total
  #elif CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_PWMS = 6;               // ESP32C2/C3/C6: 6 ledc PWM channels in total
  #else
  const uint8_t MAX_PWMS = 5;               // Unknown - revert to 5 PWM max
  #endif
#else
const uint8_t MAX_PWMS = 5;                 // (not used on ESP8266)
#endif
const uint32_t POWER_MASK = 0xffffffffUL;   // Power (Relay) full mask
const uint8_t MAX_COUNTERS = 4;             // Max number of counter sensors
const uint8_t MAX_TIMERS = 16;              // Max number of Timers
const uint8_t MAX_PULSETIMERS = 32;         // Max number of supported pulse timers
const uint8_t MAX_DOMOTICZ_IDX = 4;         // Max number of Domoticz device, key and switch indices
const uint8_t MAX_DOMOTICZ_SNS_IDX = 12;    // Max number of Domoticz sensors indices
const uint8_t MAX_KNX_GA = 10;              // Max number of KNX Group Addresses to read that can be set
const uint8_t MAX_KNX_CB = 10;              // Max number of KNX Group Addresses to write that can be set
const uint8_t MAX_XNRG_DRIVERS = 32;        // Max number of allowed energy drivers
const uint8_t MAX_XDRV_DRIVERS = 96;        // Max number of allowed driver drivers
const uint8_t MAX_XSNS_DRIVERS = 128;       // Max number of allowed sensor drivers
const uint8_t MAX_I2C_DRIVERS = 96;         // Max number of allowed i2c drivers
const uint8_t MAX_SHUTTERS = 4;             // Max number of shutters
const uint8_t MAX_SHUTTER_KEYS = 4;         // Max number of shutter keys or buttons
const uint8_t MAX_PCF8574 = 4;              // Max number of PCF8574 devices
const uint8_t MAX_DS3502 = 4;               // Max number of DS3502 digitsal potentiometer devices
const uint8_t MAX_IRSEND = 16;              // Max number of IRSEND GPIOs
const uint8_t MAX_RULE_SETS = 3;            // Max number of rule sets of size 512 characters
const uint16_t MAX_RULE_SIZE = 512;         // Max number of characters in rules
const uint16_t VL53LXX_MAX_SENSORS = 8;     // Max number of VL53L0X sensors
const char WIFI_HOSTNAME[]  = "%s-%04d"; //!PROGMEM    // Expands to <MQTT_TOPIC>-<last 4 decimal chars of MAC address>
const uint32_t HLW_PREF_PULSE = 12530;      // was 4975us = 201Hz = 1000W
const uint32_t HLW_UREF_PULSE = 1950;       // was 1666us = 600Hz = 220V
const uint32_t HLW_IREF_PULSE = 3500;       // was 1666us = 600Hz = 4.545A
const uint8_t MQTT_RETRY_SECS = 10;         // Minimum seconds to retry MQTT connection
const uint32_t PWM_RANGE = 1023;            // 255..1023 needs to be devisible by 256
const uint16_t PWM_FREQ = 880;              // 100..1000 Hz led refresh (BN-SZ01 value)
const uint16_t PWM_MAX = 4000;              // [PWM_MAX] Maximum frequency - Default: 4000
const uint16_t PWM_MIN = 100;               // [PWM_MIN] Minimum frequency - Default: 100
                                            //    For Dimmers use double of your mains AC frequecy (100 for 50Hz and 120 for 60Hz)
                                            //    For Controlling Servos use 50 and also set PWM_FREQ as 50 (DO NOT USE THESE VALUES FOR DIMMERS)
const uint8_t DEFAULT_POWER_DELTA = 80;     // Power change percentage
const uint16_t MAX_POWER_HOLD = 10;         // Time in SECONDS to allow max agreed power
const uint16_t MAX_POWER_WINDOW = 30;       // Time in SECONDS to disable allow max agreed power
const uint16_t SAFE_POWER_HOLD = 10;        // Time in SECONDS to allow max unit safe power
const uint16_t SAFE_POWER_WINDOW = 30;      // Time in MINUTES to disable allow max unit safe power
const uint8_t MAX_POWER_RETRY = 5;          // Retry count allowing agreed power limit overflow
const uint8_t IMMINENT_RESET_FACTOR = 10;   // Factor to extent button hold time for imminent Reset to default 40 seconds using KEY_HOLD_TIME of 40
const uint8_t BOOT_LOOP_TIME = 10;         // Number of seconds to stop detecting boot loops
const uint16_t FLOATSZ = 16;                // Max number of characters in float result from dtostrfd (max 32)
const uint16_t TBUFFER_SIZE_FLOAT = 16; // TBUFFER are (T)emporary Buffers that are used for conversions
#define STANDARD_SMS_CHAR_LENGTH 160
const uint8_t SENSOR_MAX_MISS = 5;          // Max number of missed sensor reads before deciding it's offline
const uint32_t SOFT_BAUDRATE = 9600;        // Default software serial baudrate
const uint32_t APP_BAUDRATE = 115200;       // Default serial baudrate
const uint8_t MAX_MQTT_CONNECTIONS = 2;

// Changes to the following MAX_ defines need to be in line with enum SettingsTextIndex
const uint8_t MAX_MQTT_PREFIXES = 3;        // Max number of MQTT prefixes (cmnd, stat, tele)
const uint8_t MAX_SSIDS = 2;                // Max number of SSIDs
const uint8_t MAX_STATE_TEXT = 4;           // Max number of State names (OFF, ON, TOGGLE, HOLD)
const uint8_t MAX_NTP_SERVERS = 3;          // Max number of NTP servers
const uint8_t MAX_RULE_MEMS = 16;           // Max number of saved vars
const uint8_t MAX_FRIENDLYNAMES = 8;        // Max number of Friendly names
const uint8_t MAX_BUTTON_TEXT = 32;         // Max number of GUI button labels
const uint8_t MAX_GROUP_TOPICS = 4;         // Max number of Group Topics
const uint8_t MAX_DEV_GROUP_NAMES = 4;      // Max number of Device Group names

#ifdef ESP8266
const uint8_t MAX_ADCS = 1;                 // Max number of ESP8266 ADC pins
const uint8_t MAX_SWITCHES_TXT = 8;         // Max number of switches user text
#endif  // ESP8266
#ifdef ESP32
  #if CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C3
  const uint8_t MAX_ADCS = 5;               // Max number of ESP32-C3 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #elif CONFIG_IDF_TARGET_ESP32C6
  const uint8_t MAX_ADCS = 7;               // Max number of ESP32 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #else   // ESP32
  const uint8_t MAX_ADCS = 8;               // Max number of ESP32 ADC pins (ADC2 pins are unusable with Wifi enabled)
  #endif  // ESP32C3
const uint8_t MAX_SWITCHES_TXT = 28;        // Max number of switches user text
#endif  // ESP32


const uint32_t START_VALID_UTC_TIME = 1697014158;  // Time is synced and after 2023 October 11, this will need adjusting when NTP code is updated


/*********************************************************************************************\
 * Defines
\*********************************************************************************************/

#define MAX_RULE_TIMERS        8            // Max number of rule timers (4 bytes / timer)
#define MAX_RULE_VARS          5            // Max number of rule variables (10 bytes / variable)

#define tmin(a,b) ((a)<(b)?(a):(b))
#define tmax(a,b) ((a)>(b)?(a):(b))

#define STR_HELPER(x) #x
#ifndef STR
#define STR(x) STR_HELPER(x)
#endif

enum LedStateOptions {LED_OFF1, LED_POWER, LED_MQTTSUB, LED_POWER_MQTTSUB, LED_MQTTPUB, LED_POWER_MQTTPUB, LED_MQTT, LED_POWER_MQTT, MAX_LED_OPTION};

// Change these to have matching words
enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };
                                  
enum PowerOnStateOptions { 
  POWER_ALL_OFF, 
  POWER_ALL_ON, 
  POWER_ALL_SAVED_TOGGLE, 
  POWER_ALL_SAVED, 
  POWER_ALL_ALWAYS_ON, 
  POWER_ALL_OFF_PULSETIME_ON 
};


// Ordered in levels, if(>0 then active)
enum MODULE_STATUS{
  MODULE_STATUS_DISABLED = 0,       // Disabled = User has chosen not to use module, block tasker from executing
  MODULE_STATUS_PRE_INIT_PASS,      // Pre-init completely successfully
  MODULE_STATUS_INIT_PASS,          // Init completed successfully
  MODULE_STATUS_ENABLED_NOT_ACTIVE, // Sensor not found, or driver didnt complete init, retry (pre-)init process
  MODULE_STATUS_ENABLED_ACTIVE,     // Working = Normal state
  MODULE_STATUS_NOTSET,             // Default start state
  MODULE_STATUS_LENGTH
};

/**
 * @brief Initial used with buttons, but should be expanded into rule type events
 **/
enum INPUT_TYPE_IDS{ //style fron nextion
  INPUT_TYPE_SINGLE_PRESS_ID,
  INPUT_TYPE_MULTIPLE_PRESS_ID,
  INPUT_TYPE_SINGLE_HOLD_ID,
  INPUT_TYPE_LENGTH_ID
};

/**
 * @brief 
 * 
 * Just delete these, dont like the params8
 * 
 */
enum SO32_49Index { P_HOLD_TIME,              // SetOption32 - (Button/Switch) Key hold time detection in decaseconds (default 40)
                    P_MAX_POWER_RETRY,        // SetOption33 - (Energy) Maximum number of retries before deciding power limit overflow (default 5)
                    P_BACKLOG_DELAY,          // SetOption34 - (Backlog) Minimal delay in milliseconds between executing backlog commands (default 200)
                    P_MDNS_DELAYED_START,     // SetOption35 - (mDNS) Number of seconds before mDNS is started (default 0) - Obsolete
                    P_BOOT_LOOP_OFFSET,       // SetOption36 - (Restart) Number of restarts to start detecting boot loop (default 1)
                    P_SO37_FREE,              // SetOption37 - (Light) RGB and White channel separation (default 0)
                    P_IR_UNKNOW_THRESHOLD,    // SetOption38 - (IR) Set the smallest sized "UNKNOWN" message packets we actually care about (default 6, max 255)
                    P_CSE7766_INVALID_POWER,  // SetOption39 - (CSE7766) Number of invalid power measurements before declaring it invalid allowing low load measurments (default 128)
                    P_HOLD_IGNORE,            // SetOption40 - (Button/Shutter) Ignore button change in seconds (default 0)
                    P_ARP_GRATUITOUS,         // SetOption41 - (Wifi) Interval in seconds between gratuitous ARP requests (default 60)
                    P_OVER_TEMP,              // SetOption42 - (Energy) Turn all power off at or above this temperature (default 90C)
                    P_ROTARY_MAX_STEP,        // SetOption43 - (Rotary) Rotary step boundary (default 10)
                    P_IR_TOLERANCE,           // SetOption44 - (IR) Base tolerance percentage for matching incoming IR messages (default 25, max 100)
                    P_SO45_FREE,              // SetOption45
                    P_SO46_FREE,              // SetOption46
                    P_SO47_FREE,              // SetOption47
                    P_SO48_FREE,              // SetOption48
                    P_SO49_FREE               // SetOption49
                  };  // Max is PARAM8_SIZE (18) - SetOption32 until SetOption49


#ifdef ESP8266
extern "C" {
#include "spi_flash.h"
}
#include "eboot_command.h"
// extern "C" uint32_t _SPIFFS_end;
#endif

#include "2_CoreSystem/13_DeviceNameList/mDeviceNameList.h"

#ifdef ESP8266
  #if AUTOFLASHSIZE

  #else
    extern "C" uint32_t _FS_end;
  #endif  // AUTOFLASHSIZE
#endif

#include "1_TaskerManager/mTaskerInterface.h"

class mSettings :
  public mTaskerInterface
{
  public:
    mSettings(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_CORE_SETTINGS_CTR = D_MODULE_CORE_SETTINGS_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_SETTINGS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SETTINGS_ID; }

  #ifdef ESP8266
    #if AUTOFLASHSIZE
      #include "flash_hal.h"
      // From libraries/EEPROM/EEPROM.cpp EEPROMClass
      const uint32_t SPIFFS_END = (FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;
    #else
      // extern "C" uint32_t _FS_end;
      // From libraries/EEPROM/EEPROM.cpp EEPROMClass
      const uint32_t SPIFFS_END = ((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;
    #endif  // AUTOFLASHSIZE
    // Version 4.2 config = eeprom area
    const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area

    // Version 5.2 allow for more flash space
    const uint8_t CFG_ROTATES = 1;//8;          // Number of flash sectors used (handles uploads)

    uint32_t settings_location = SETTINGS_LOCATION;
    // uint32_t settings_crc = 0;
    uint32_t settings_crc32 = 0;
    uint8_t *settings_buffer = nullptr;
  #endif // ESP8266

  void JsonAppend_Settings();    
  void init(void);
  uint16_t CountCharInCtr(const char* tosearch, char tofind);
  int16_t GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count);
  void Function_Template_Load();
  void SettingsWrite(const void *pSettings, unsigned nSettingsLen);
  int16_t GetFunctionIDbyName(const char* c);
  void SettingsInit();
  void SetFlashModeDout(void);
  void SettingsBufferFree(void);
  bool SettingsBufferAlloc(void);
  uint32_t SettingsRead(void *data, size_t size);
  void SettingsSaveAll(void);
  uint32_t GetSettingsAddress(void);
  void SettingsSave(uint8_t rotate);
  void SettingsLoad(void);
  void SettingsErase(uint8_t type);
  bool SettingsEraseConfig(void) ;
  void SettingsSdkErase(void);
  void SettingsDefault(void);
  void SystemSettings_DefaultHeader(void);
  void SystemSettings_DefaultBody(void);

  uint32_t GetCfgCrc32(uint8_t *bytes, uint32_t size);
  uint32_t GetSettingsCrc32(void);

  void SettingsResetStd(void);
  void SettingsResetDst(void);

  typedef union {
    uint8_t data;
    struct {
      uint8_t network_down : 1;
      uint8_t mqtt_down : 1;
      uint8_t wifi_down : 1;
      uint8_t eth_down : 1;
      uint8_t spare04 : 1;
      uint8_t spare05 : 1;
      uint8_t spare06 : 1;
      uint8_t spare07 : 1;
    };
  } StateBitfield;


  typedef union {
    uint32_t data;
    struct {
      uint32_t time : 11;                    // bits 0 - 10 = minutes in a day
      uint32_t window : 4;                   // bits 11 - 14 = minutes random window
      uint32_t repeat : 1;                   // bit 15
      uint32_t days : 7;                     // bits 16 - 22 = week day mask
      uint32_t device : 4;                   // bits 23 - 26 = 16 devices
      uint32_t power : 2;                    // bits 27 - 28 = 4 power states - Off, On, Toggle, Blink or Rule
      uint32_t mode : 2;                     // bits 29 - 30 = timer modes - 0 = Scheduler, 1 = Sunrise, 2 = Sunset
      uint32_t arm : 1;                      // bit 31
    };
  } Timer;

  /**
   * SECTION
   * Templates
   * - MODULE_TEMPLATE
   * - FUNCTION_TEMPLATE
   * - LIGHTING_TEMPLATE
   * - DISPLAY_TEMPLATE
   * 
   */

  // a flag status group showing how succesful the boot was
  // 3 state template, used, succuss, fail, none

  #define MODULE_TEMPLATE_MAX_SIZE 500
  enum TemplateSource
  {
    NONE,
    HEADER_CUSTOM,   // custom
    HEADER_TEMPLATE, // from a predefined template
    FILE,            // from a file (loading into internal storage with webui or mqtt command)
    FAILED_LOAD      // error occurs when loaded
  };
  struct TemplateLoading
  {
    struct STATUS{
      uint8_t module                = TemplateSource::NONE;
      uint8_t function              = TemplateSource::NONE;
      uint8_t lighting              = TemplateSource::NONE;
      uint8_t nextion_hmi_input_map = TemplateSource::NONE;
      uint8_t rules                 = TemplateSource::NONE;
    }status;

    // Other
  };  
 

#include "2_CoreSystem/05_HardwarePins/mHardwarePins.h"

struct Template_Config{
  uint8_t flags;
  uint8_t base;       
  mytmplt hardware;   
};


struct SystemName{ 
  char          friendly[50]; // Used in titles, set by templates "FriendlyName"
  char          device[50];   // USed as mqtt topic by default, set by templates "Name"
};
#ifdef DEVICENAME_CTR
// static_assert(sizeof(DEVICENAME_CTR) - 1 <= 50, "DEVICENAME_CTR exceeds max length");
#endif 


#define PARAM8_SIZE 18            // Number of param bytes (SetOption)
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint32_t save_state : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint32_t button_restrict : 1;          // bit 1              - SetOption1  - Control button multipress
    uint32_t value_units : 1;              // bit 2              - SetOption2  - Add units to JSON status messages
    uint32_t mqtt_enabled : 1;             // bit 3              - SetOption3  - Control MQTT
    uint32_t mqtt_response : 1;            // bit 4              - SetOption4  - Switch between MQTT RESULT or COMMAND
    uint32_t mqtt_power_retain : 1;        // bit 5              - CMND_POWERRETAIN
    uint32_t mqtt_button_retain : 1;       // bit 6              - CMND_BUTTONRETAIN
    uint32_t mqtt_switch_retain : 1;       // bit 7              - CMND_SWITCHRETAIN
    uint32_t temperature_conversion : 1;   // bit 8              - SetOption8  - Switch between Celsius or Fahrenheit
    uint32_t mqtt_sensor_retain : 1;       // bit 9              - CMND_SENSORRETAIN
    uint32_t mqtt_offline : 1;             // bit 10             - SetOption10 - Control MQTT LWT message format
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)    - SetOption11 - Swap button single and double press functionality
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)    - SetOption12 - Switch between dynamic or fixed slot flash save location
    uint32_t button_single : 1;            // bit 13 (v5.4.0)    - SetOption13 - Support only single press to speed up button press recognition
    uint32_t interlock : 1;                // bit 14 (v5.6.0)    - CMND_INTERLOCK
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)    - SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)    - SetOption16 - Switch between clockwise or counter-clockwise
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)    - SetOption17 - Switch between decimal or hexadecimal output
    uint32_t light_signal : 1;             // bit 18 (v5.10.0c)  - SetOption18 - Pair light signal with CO2 sensor
    uint32_t hass_discovery : 1;           // bit 19 (v5.11.1a)  - SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    uint32_t not_power_linked : 1;         // bit 20 (v5.11.1f)  - SetOption20 - Control power in relation to Dimmer/Color/Ct changes
    uint32_t no_power_on_check : 1;        // bit 21 (v5.11.1i)  - SetOption21 - Show voltage even if powered off
    uint32_t mqtt_serial : 1;              // bit 22 (v5.12.0f)  - CMND_SERIALSEND and CMND_SERIALLOG
    uint32_t mqtt_serial_raw : 1;          // bit 23 (v6.1.1c)   - CMND_SERIALSEND3
    uint32_t pressure_conversion : 1;      // bit 24 (v6.3.0.2)  - SetOption24 - Switch between hPa or mmHg pressure unit
    uint32_t knx_enabled : 1;              // bit 25 (v5.12.0l)  - CMND_KNX_ENABLED
    uint32_t device_index_enable : 1;      // bit 26 (v5.13.1a)  - SetOption26 - Switch between POWER or POWER1
    uint32_t knx_enable_enhancement : 1;   // bit 27 (v5.14.0a)  - CMND_KNX_ENHANCED
    uint32_t rf_receive_decimal : 1;       // bit 28 (v6.0.0a)   - SetOption28 - RF receive data format
    uint32_t ir_receive_decimal : 1;       // bit 29 (v6.0.0a)   - SetOption29 - IR receive data format
    uint32_t mqtt_switches : 1;               // bit 30 (v6.0.0b)   - SetOption30 - Enforce HAss autodiscovery as light
    uint32_t global_state : 1;             // bit 31 (v6.1.0)    - SetOption31 - Control link led blinking
  };
} SysBitfield_System;


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51 - Enable ESP8285 user GPIO's
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52 - Append timezone to JSON time
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53 - Show hostanme and IP address in GUI main menu
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54 - Apply SetOption20 settings to Tuya device
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55 - Control mDNS service
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56 - Scan wifi network at restart for configured AP's
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57 - Scan wifi network every 44 minutes for configured AP's
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58 - Add IR Raw data to JSON message
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60 - Enable normal sleep instead of dynamic sleep
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61 - Force local operation when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62 - Don't use retain flag on HOLD messages
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63 - Don't scan relay power state at restart
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64 - Enable "_" instead of "-" as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65 - Disable fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66 - Enable TuyaMcuReceived messages over Mqtt
    uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67 - Enable buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68 - Enable multi-channels PWM instead of Color PWM
    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69 - Limits Tuya dimmers to minimum of 10% (25) when enabled.
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71 - Select different Modbus registers for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72 - Enable hardware energy total counter as reference (#6561)
    uint32_t ex_cors_enabled : 1;          // bit 23 (v7.0.0.1)  - SetOption73 - Enable HTTP CORS
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74 - Enable internal pullup for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75 - GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76 - Enable incrementing bootcount when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77 - Do not power off if slider moved to far left
    uint32_t compatibility_check : 1;      // bit 28 (v7.1.2.6)  - SetOption78 - Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79 - Enable resetting of counters after telemetry was sent
    uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80 - Enable shutter support
    uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81 - Invert all ports on PCF8574 devices
    uint32_t network_wifi : 1;             // bit 13 (v8.3.1.3)  - CMND_WIFI
    uint32_t network_ethernet : 1;         // bit 14 (v8.3.1.3)  - CMND_ETHERNET
  };
} SysBitfield_Network;


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t buzzer_freq_mode : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint8_t buzzer_enable : 1;
    uint8_t reserved : 6;          // bit 1              - SetOption1  - Control button multipress
   
  };
} SysBitfield_Drivers;


typedef union {
  uint16_t data;
  struct {
    uint16_t spare0 : 1;
    uint16_t reserved : 15;     
  };
} SysBitfield_Lighting;


// struct LightSettings{
//   uint8_t light_brightness_as_percentage;
//   uint8_t light_fade;          // 4A1
//   uint8_t light_speed;         // 4A2
//   uint8_t light_scheme;        // 4A3
//   uint8_t light_width;         // 4A4
//   uint16_t light_rotation;     // 39E
//   uint16_t light_pixels;       // 496
//   uint8_t light_correction;    // 49D
//   uint8_t light_dimmer;        // 49E
//   uint16_t light_wakeup;       // 4A6
//   uint8_t type; //phase out now with multipin?
//   uint16_t size;
// };


// Buffer that stores names of sensors as delimeter list
#ifndef DEVICENAMEBUFFER_NAME_INDEX_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_INDEX_LENGTH 70 
#endif // DEVICENAMEBUFFER_NAME_INDEX_LENGTH
#ifndef DEVICENAMEBUFFER_NAME_BUFFER_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 400 
#endif // DEVICENAMEBUFFER_NAME_BUFFER_LENGTH
struct DeviceNameBuffer{ // size(230)
  // delimeter name list
  char name_buffer[DEVICENAMEBUFFER_NAME_BUFFER_LENGTH];
  // index array that holds name_list sensor (class name, sensor number)
  /**
   * This needs to be the large unique identifier
   * "Unused" will be the largest possible value, instead of using signed value and setting to "-1", which makes -2->minimum wasted 
   * */
  uint16_t class_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH]  = { D_MAX_UINT16 }; //hold class id
  uint8_t  device_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH] = { D_MAX_UINT8  };  //max of X sensors per module
};


typedef union {
  uint8_t data;
  struct {
    uint8_t spare0 : 1;
    uint8_t spare1 : 1;
    // uint8_t bh1750_1_resolution : 2;       // Sensor10 1,2,3
    // uint8_t bh1750_2_resolution : 2;
    uint8_t hx711_json_weight_change : 1;  // Sensor34 8,x - Enable JSON message on weight change
    uint8_t mhz19b_abc_disable : 1;        // Disable ABC (Automatic Baseline Correction for MHZ19(B) (0 = Enabled (default), 1 = Disabled with Sensor15 command)
  };
} SensorCfg1;

// Bitfield to be used for any SetOption50 .. SetOption81 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51  - (GPIO) Enable ESP8285 user GPIO's (1)
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52  - (Time) Append timezone to JSON time (1)
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53  - (GUI) Show hostname and IP address in GUI main menu (1)
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54  - (Tuya) Apply SetOption20 settings to Tuya device (1)
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55  - (mDNS) Service on (1) or off (0)
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56  - (Wifi) Scan network at restart for configured AP's (1) or used stored AP (0)
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57  - (Wifi) Scan network every 44 minutes for configured AP's (1)
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58  - (IR) Add IR Raw data to JSON message (1)
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59  - (MQTT) Send tele/%topic%/STATE in addition to stat/%topic%/RESULT (1)
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60  - (Sleep) Enable normal sleep (1) instead of dynamic sleep (0)
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61  - (Button, Switch) Force local operation (1) when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62  - (MQTT) Don't use retain flag on HOLD messages (1)
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63  - (Power) Don't scan relay power state at restart (1)
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64  - (JSON) Enable "_" (1) instead of "-" (0) as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65  - (QPC) Disable (1) fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66  - (Tuya) Enable (1) TuyaMcuReceived messages over Mqtt
    uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67  - (Buzzer) Enable (1) buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68  - (Light) Enable multi-channels PWM (1) instead of Color PWM (0)
    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69  - (not used) Limits Tuya dimmers to minimum of 10% (25) when enabled
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71  - (DDS2382) Select different Modbus registers (1) for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72  - (Energy) Enable (1) hardware energy total counter as reference (#6561)
    uint32_t mqtt_buttons : 1;             // bit 23 (v8.2.0.3)  - SetOption73  - (Button) Detach buttons from relays (1) and enable MQTT action state for multipress
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74  - (DS18x20) Enable internal pullup (1) for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75  - (MQTT) GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77  - (Light) Do not power off (1) if slider moved to far left
    uint32_t ex_compatibility_check : 1;   // bit 28 (v7.1.2.6)  - SetOption78  - (not used) Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79  - (Counter) Enable resetting of counters (1) after telemetry was sent
    uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80  - (Shutter) Enable shutter support (1)
    uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81  - (PCF8574) Invert all ports on PCF8574 devices (1)
  };
} SOBitfield3;





typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t button_restrict : 1;          // bit 1              - SetOption1  - Control button multipress
    uint8_t decimal_precision : 2;        // bit 2,3   4 levels [0,1,2,3]

  };
} SysBitfield_Sensors;



struct SensorSettings{
  /**
   * Stored as ints for data savings, flat change to int
   * */
  int16_t       altitude;            
  float           latitude;      //54.5 shall be 54500000
  float           longitude;               
  SysBitfield_Sensors flags;
};


struct EnergyUsageNew{
  uint32_t usage1_kWhtotal;
  uint32_t usage2_kWhtotal;
  uint32_t return1_kWhtotal;
  uint32_t return2_kWhtotal;
  uint32_t last_return_kWhtotal;
  uint32_t last_usage_kWhtotal;  
  uint8_t       energy_power_delta;        // 33F
  unsigned long energy_power_calibration;  // 364
  unsigned long energy_voltage_calibration;  // 368
  unsigned long energy_current_calibration;  // 36C
  unsigned long energy_kWhtoday;           // 370
  unsigned long energy_kWhyesterday;       // 374
  uint16_t      energy_kWhdoy;             // 378
  uint16_t      energy_min_power;          // 37A
  uint16_t      energy_max_power;          // 37C
  uint16_t      energy_min_voltage;        // 37E
  uint16_t      energy_max_voltage;        // 380
  uint16_t      energy_min_current;        // 382
  uint16_t      energy_max_current;        // 384
  uint16_t      energy_max_power_limit;    // 386 MaxPowerLimit
  uint16_t      energy_max_power_limit_hold;         // 388 MaxPowerLimitHold
  uint16_t      energy_max_power_limit_window;       // 38A MaxPowerLimitWindow
  uint16_t      energy_max_power_safe_limit;         // 38C MaxSafePowerLimit
  uint16_t      energy_max_power_safe_limit_hold;    // 38E MaxSafePowerLimitHold
  uint16_t      energy_max_power_safe_limit_window;  // 390 MaxSafePowerLimitWindow
  uint16_t      energy_max_energy;         // 392 MaxEnergy
  uint16_t      energy_max_energy_start;   // 394 MaxEnergyStart
  uint32_t      energy_kWhtotal_time;      // 7B4
  unsigned long energy_frequency_calibration;  // 7C8
  unsigned long energy_kWhtotal;           // 554
  uint16_t      tariff[4][2];              // E30
};// EnergyUsage;


// Sensors
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;
    uint32_t spare01 : 1;
    uint32_t spare02 : 1;
    uint32_t spare03 : 1;
    uint32_t spare04 : 1;
    uint32_t spare05 : 1;
    uint32_t calc_resolution : 3;
    uint32_t weight_resolution : 2;
    uint32_t frequency_resolution : 2;
    uint32_t axis_resolution : 2;
    uint32_t current_resolution : 2;
    uint32_t voltage_resolution : 2;
    uint32_t wattage_resolution : 2;
    uint32_t emulation : 2;
    uint32_t energy_resolution : 3;
    uint32_t pressure_resolution : 2;
    uint32_t humidity_resolution : 2;
    uint32_t temperature_resolution : 2;
  };
} SysBitfield_Power;

struct DisplaySettings{
  uint8_t       model; 
  uint8_t       mode;
  uint8_t       refresh;
  uint8_t       rows;
  uint8_t       cols[2];
  uint8_t       address[8];
  uint8_t       dimmer;
  uint8_t       size;
  uint8_t       font;
  uint8_t       rotate;
  uint16_t      width;
  uint16_t      height;
};


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t awb : 1;
    uint32_t awb_gain : 1;
    uint32_t aec : 1;
    uint32_t aec2 : 1;
    uint32_t agc : 1;
    uint32_t raw_gma : 1;
    uint32_t lenc : 1;
    uint32_t colorbar : 1;
    uint32_t wpc : 1;
    uint32_t dcw : 1;
    uint32_t bpc : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t feature : 2;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;

typedef union {
  uint32_t data;
  struct {
    uint32_t wb_mode : 3;
    uint32_t ae_level : 3;
    uint32_t aec_value : 11;
    uint32_t gainceiling : 3;
    uint32_t agc_gain: 5;
    uint32_t special_effect : 3;
    uint32_t auth : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t upgraded : 1;
  };
} WebCamCfg2;

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint16_t data;                           // Allow bit manipulation
  struct {
    uint16_t system_init : 1;              // Changing layout here needs adjustments in xdrv_10_rules.ino too
    uint16_t system_boot : 1;
    uint16_t time_init : 1;
    uint16_t time_set : 1;
    uint16_t mqtt_connected : 1;
    uint16_t mqtt_disconnected : 1;
    uint16_t wifi_connected : 1;
    uint16_t wifi_disconnected : 1;
    uint16_t eth_connected : 1;
    uint16_t eth_disconnected : 1;
    uint16_t http_init : 1;
    uint16_t shutter_moved : 1;
    uint16_t shutter_moving : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} RulesBitfield;

struct LoggingSettings{
  uint8_t       serial_level;           // 09E
  uint16_t      sys_port;               // 1A8
  uint8_t       sys_level;              // 1AA
  uint8_t       web_level;              // 1AC
  uint8_t       telnet_level;              // 1AC
  uint8_t       mqtt_level;              // 1AC
  uint8_t       time_isshort;  
};


struct SETTINGS {
  // Header (Minimal data load required to validate settings - order must never change)
  uint16_t      cfg_holder;                // 000 v6 header
  uint16_t      cfg_size;                  // 002
  uint32_t      save_flag;                 // 004
  uint32_t      version;                   // 008
  uint16_t      bootcount;              // 00C
  // Body (All other settings)
  // Modules
  uint16_t      bootcount_errors_only;     // E01
  uint8_t       module;                    // 474
  uint8_t       last_module;               // 399
  // Templates
  Template_Config user_template; 
  SystemName      system_name;                             // Move into SettingsText
  char room_hint[50];                                      // Move into SettingsText
  SysBitfield_System   flag_system;                      // 010
  RulesBitfield rules_flag;                 // Rule state flags (16 bits)
  int16_t       save_data;                 // 014
  myio          module_pins;                     // 484     
  uint8_t       baudrate;                  // 09D  // saved as (/300) value. ie 9600/300 => 32, 115200=>384?? I want to change this to full uint32_t for higher speed bauds
  uint8_t       rule_stop;                 // 1A7
  LoggingSettings logging;
  DeviceNameBuffer        device_name_buffer; // this might need moving and then changed to use vectors, it would actually remove the need for delims
  uint8_t       enable_sleep;        // E03
  char          serial_delimiter;          // 451
  uint8_t       sbaudrate;                 // 452
  uint8_t       sleep;                     // 453
  uint8_t       setoption_255[PARAM8_SIZE];
  // Core
  uint16_t      unified_interface_reporting_invalid_reading_timeout_seconds; // 0 is ignored, anything else is the seconds of age above which a sensor should not be reporting (ie is invalid)
  // Network
  uint8_t       sta_config;                // 09F
  uint8_t       sta_active;                // 0A0
  uint32_t      ip_address[5];             // 544
  uint8_t       wifi_channel;
  uint8_t       wifi_bssid[6];             // F0A
  SysBitfield_Network  flag_network;                     // 3A0
  // Webserver
  uint8_t       webserver;                 // 1AB
  uint16_t      web_refresh;               // 7CC
  // Time
  int8_t        timezone;                  // 016
  uint8_t       timezone_minutes;          // 66D 
  int8_t        timezone2;                  // 016
  uint8_t       timezone_minutes2;          // 66D
  SysBitfield_Drivers    flag_drivers;  
  int16_t       toffset[2];                // 30E

  // Previously other char arrays followed this memory space that was reserved as "overflow" fom text pool to be read in another format
  // From now on, the text pool must be the hardcoded full length
  char          text_pool[settings_text_size];            // 017  Size is settings_text_size
  // Lighting
  SysBitfield_Lighting    flag_lighting;
  // Pulse Counter
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  // Sensors
  SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands  
  uint16_t      button_debounce;           // 542
  uint16_t      switch_debounce;           // 66E
  uint8_t       switchmode[8];
  SensorSettings sensors;
  // Drivers
  uint16_t      ledmask;                   // 7BC
  uint8_t       ledstate;                  // 2FB
  uint8_t       ledpwm_mask;               // E8F  
  uint8_t       ledpwm_on;                 // F3F
  uint8_t       ledpwm_off;                // F40
  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  uint32_t      monitors;                  // 7A0
  uint16_t      pwm_range;                 // 342
  uint16_t      pwm_frequency;             // 2E6
  uint16_t      rf_duplicate_time;         // 522
  // Power
  unsigned long power;                     // 2E8
  uint8_t       poweronstate;              // 398
  // Energy
  EnergyUsageNew   energy_usage;           // 77C 
  SysBitfield_Power  flag_power;           // 5BC
  // Displays
  DisplaySettings   display;  
  uint32_t      i2c_drivers[3];            // FEC
  uint64_t      rf_protocol_mask;          // FA8
  #ifdef ESP32
  WebCamCfg     webcam_config;             // 44C
  WebCamCfg2    webcam_config2;            // 460
  #endif
  #ifdef ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  char settings_holder_ctr[10];
  #endif
  uint32_t      bootcount_reset_time;      // FD4
  TimeRule      tflag[2];                  // 2E2
  SOBitfield3   flag3;                     // 3A0
  uint32_t      ipv4_address[5];           // 544
  uint32_t      ipv4_rgx_address;          // 558
  uint32_t      ipv4_rgx_subnetmask;       // 55C
  uint16_t      dns_timeout;               // 4C8
  #ifdef ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  char local_time_ascii_debug[20];
  #endif
  // E00 - FFF (4095 ie eeprom size) free locations
  uint32_t      cfg_timestamp;
  uint32_t      cfg_crc32;                 // 32 bit CRC, must remain at last 4 bytes
} Settings;



  void TestSettingsLoad();
  void TestSettings_ShowLocal_Header();
    
  struct FIRMWARE_VERSION{
    struct type{
      uint32_t number;
      uint8_t part_branch = 0;
      uint8_t part_major = 0;
      uint8_t part_minor = 0;
      uint8_t part_system = 0;
      uint8_t part_module = 0;
      char name_ctr[15];
    };
    struct type current;
    struct type latest;
    struct type lowest;
    uint8_t fNewVersionAvailable = false;
    uint8_t fCurrentVersionNotSupported = false;
  };

  #ifdef ESP8266
  SerialConfig serial_config = SERIAL_8N1;    // Serial interface configuration 8 data bits, No parity, 1 stop bit
  #endif

  // These are used only at runtime, and not saved eg TasmotaGlobals
  struct RUNTIME_GLOBALS{
    uint8_t sleep;                              // Current copy of Settings.sleep
    uint32_t tSavedUpdateLoopStatistics;
    uint8_t light_driver;                     // Light module configured
    uint8_t light_type;                       // Light types
    TemplateLoading template_loading;
    FIRMWARE_VERSION firmware_version;    
    power_t power = 0;                          // Current copy of Settings.power
    int ota_state_flag = 0;                     // OTA state flag
    int ota_result = 0;                         // OTA result
    int restart_flag = 0;                       // Sonoff restart flag
    int blinks = 201;                           // Number of LED blinks
    uint32_t loop_load_avg = 0;                 // Indicative loop load average
    char *ota_url;                              // OTA url string pointer
    int16_t save_data_counter;                  // Counter and flag for config save to Flash
    uint8_t latching_relay_pulse = 0;           // Latching relay pulse timer
    uint8_t backlog_index = 0;                  // Command backlog index
    uint8_t backlog_pointer = 0;                // Command backlog pointer
    uint8_t blinkspeed = 1;                     // LED blink rate
    uint8_t settings_save_decounter_seconds_delayed_save = 0;    
    uint8_t active_device = 1;                  // Active device in ExecuteCommandPower
    uint8_t pwm_inverted = 0;                   // PWM inverted flag (1 = inverted)
    uint8_t counter_no_pullup = 0;              // Counter input pullup flag (1 = No pullup)
    uint8_t serial_in_byte;                     // Received byte
    uint8_t seriallog_level;                    // Current copy of Settings.seriallog_level
    uint8_t syslog_level;                       // Current copy of Settings.syslog_level
    uint8_t seriallog_level_during_boot;
    uint8_t my_module_type;                     // Current copy of Settings.module or user template type
    uint8_t last_source = 0;                    // Last command source
    uint8_t mdns_delayed_start = 0;             // mDNS delayed start
    bool stop_flash_rotate = false;             // Allow flash configuration rotation
    bool blinkstate = false;                    // LED state
    bool pwm_present = false;                   // Any PWM channel configured with SetOption15 0
    bool i2c_enabled = false;                       // I2C configured
    #ifdef ESP32
    bool i2c_enabled_2 = false;                       // I2C configured, second controller on ESP32, Wire1
    #endif
    bool spi_flg = false;                       // SPI configured
    bool soft_spi_flg = false;                  // Software SPI configured
    bool ntp_force_sync = false;                // Force NTP sync
    myio my_module;                             // Active copy of Module GPIOs (17 x 8 bits)
    gpio_flag my_module_flag;                   // Active copy of Module GPIO flags
    StateBitfield global_state;                 // Global states (currently Wifi and Mqtt) (8 bits)
    char my_hostname[33];                       // Composed Wifi hostname
    uint8_t flag_boot_complete = false;
    int wifi_state_flag = 0;         // WIFI_RESTART Wifi state flag    
    bool settings_lkg = false;  // Settings saved as Last Known Good
    uint8_t enable_web_logging_filtering = false;
    uint8_t enable_serial_logging_filtering = false;    
    bool settings_holder_hardcorded_stored_changed = false; // if true, other files may want to reset too
  }runtime;

  #define RESET_BOOT_STATUS() memset(&tkr_set->runtime.boot_status,0,sizeof(tkr_set->runtime.boot_status))

  void CommandSet_SystemRestartID(uint8_t value);

  int16_t     SwitchMode_GetID_by_Name(const char* c);
  const char* SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen);

  #define RESPONSE_MESSAGE_BUFFER_SIZE 100
  char response_msg[RESPONSE_MESSAGE_BUFFER_SIZE] = {0};

  const char* Get_Json_Level_Name(uint8_t id);

  

  #ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
  bool NvmExists(const char *sNvsName);
  bool NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen);
  void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen);
  int32_t NvmErase(const char *sNvsName);
  #endif // ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY





  #ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
  uint32_t GetSettingsTextLen(void);
  bool SettingsUpdateFinished(void);
  bool SettingsUpdateText(uint32_t index, const char* replace_me);
  char* SettingsText(uint32_t index);
  #endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER

};

#endif  // _SETTINGS_H_

