#ifndef _MSWITCHES_H_
#define _MSWITCHES_H_

#define D_UNIQUE_MODULE_SENSORS_SWITCHES_ID  5001 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

#include <Ticker.h>

#include "2_CoreSystem/mBaseConfig.h"

#include "2_CoreSystem/07_Time/mTime.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "3_Network/10_MQTT/mMQTT.h"
#include "2_CoreSystem/07_Time/mTime.h"


// #ifdef ESP32
//   #include <WiFi.h>
// #endif
// #ifdef ESP8266
//   #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
//   #include <ESP8266httpUpdate.h>
// #endif




#include <Ticker.h>

enum SwitchStates { SWITCH_PRESSED_ID, SWITCH_NOT_PRESSED_ID };


enum SwitchModeOptions_IDS {
  SWITCHMODE_TOGGLE_ID, 
  SWITCHMODE_FOLLOW_ID, 
  SWITCHMODE_FOLLOW_INV_ID, 
  SWITCHMODE_PUSHBUTTON_ID, 
  SWITCHMODE_PUSHBUTTON_INV_ID, 
  SWITCHMODE_PUSHBUTTONHOLD_ID, 
  SWITCHMODE_PUSHBUTTONHOLD_INV_ID, 
  SWITCHMODE_PUSHBUTTON_TOGGLE_ID, 
  SWITCHMODE_MAX_SWITCH_OPTION_ID
};

#define D_SWITCHMODE_TOGGLE_CTR               "Toggle"
#define D_SWITCHMODE_FOLLOW_CTR               "Follow"
#define D_SWITCHMODE_FOLLOW_INV_CTR           "Follow Inv"
#define D_SWITCHMODE_PUSHBUTTON_CTR           "PushButton"
#define D_SWITCHMODE_PUSHBUTTON_INV_CTR       "PushButton Inv"
#define D_SWITCHMODE_PUSHBUTTONHOLD_CTR       "PushButton Hold"
#define D_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR   "PushButton Hold Inv"
#define D_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR    "PushButton Toggle"

DEFINE_PGM_CTR(PM_SWITCHMODE_TOGGLE_CTR) D_SWITCHMODE_TOGGLE_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_FOLLOW_CTR) D_SWITCHMODE_FOLLOW_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_FOLLOW_INV_CTR) D_SWITCHMODE_FOLLOW_INV_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTON_CTR) D_SWITCHMODE_PUSHBUTTON_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTON_INV_CTR) D_SWITCHMODE_PUSHBUTTON_INV_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTONHOLD_CTR) D_SWITCHMODE_PUSHBUTTONHOLD_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR) D_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR;
DEFINE_PGM_CTR(PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR) D_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR;



#include "1_TaskerManager/mTaskerInterface.h"

class mSwitches :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSwitches(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};
    
    static constexpr const char* PM_MODULE_SENSORS_SWITCHES_CTR = D_MODULE_SENSORS_SWITCHES_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SWITCHES_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SWITCHES_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/


    #ifdef ENABLE_DEVFEATURE_SWITCHES__V2

enum SwitchModeOptions {TOGGLE,                              // 0
                        FOLLOW, FOLLOW_INV,                  // 1, 2   - Follow switch state
                        PUSHBUTTON, PUSHBUTTON_INV,          // 3, 4   - Pushbutton (default 1, 0 = toggle)
                        PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV,  // 5, 6   - Pushbutton with hold (default 1, 0 = toggle, Hold = hold)
                        PUSHBUTTON_TOGGLE,                   // 7      - = 0
                        TOGGLEMULTI,                         // 8      - = 0 with multi toggle
                        FOLLOWMULTI, FOLLOWMULTI_INV,        // 9, 10  - Multi change follow (0 = off, 1 = on, 2x change = hold)
                        PUSHHOLDMULTI, PUSHHOLDMULTI_INV,    // 11, 12 - Pushbutton with dimmer mode
                        PUSHON, PUSHON_INV,                  // 13, 14 - Pushon mode (1 = on, switch off using PulseTime)
                        PUSH_IGNORE, PUSH_IGNORE_INV,        // 15, 16 - Send only MQTT message on switch change
                        MAX_SWITCH_OPTION};

enum ButtonStates { PRESSED, NOT_PRESSED };
enum SendKeyOptions { KEY_BUTTON, KEY_SWITCH };
enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP, POWER_OFF_FORCE,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };
enum SendKeyPowerOptions { POWER_HOLD = 3, POWER_INCREMENT = 4, POWER_INV = 5, POWER_CLEAR = 6, POWER_RELEASE = 7,
                           POWER_100 = 8, CLEAR_RETAIN = 9, POWER_DELAYED = 10 };

                           
enum TopicOptions { CMND, STAT, TELE, nu1, RESULT_OR_CMND, RESULT_OR_STAT, RESULT_OR_TELE };


/*********************************************************************************************\
 * Switch support with input filter
 *
 * Inspired by (https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/master/olimex/user/user_switch2.c)
\*********************************************************************************************/

const uint8_t SWITCH_PROBE_INTERVAL = 10;      // Time in milliseconds between switch input probe
const uint8_t SWITCH_FAST_PROBE_INTERVAL = 2;  // Time in milliseconds between switch input probe for AC detection
const uint8_t SWITCH_AC_PERIOD = (20 + SWITCH_FAST_PROBE_INTERVAL - 1) / SWITCH_FAST_PROBE_INTERVAL;   // Duration of an AC wave in probe intervals

// Switch Mode definietions
#define SM_TIMER_MASK         0x3F
#define SM_NO_TIMER_MASK      0xFF
#define SM_FIRST_PRESS        0x40
#define SM_SECOND_PRESS       0x80
#define POWER_NONE            99

const char kSwitchPressStates[81] PROGMEM =
  "||||POWER_INCREMENT|POWER_INV|POWER_CLEAR|POWER_RELEASE|POWER_100||POWER_DELAYED";

#include <Ticker.h>

Ticker* TickerSwitch;

static const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
struct SWITCH {
  uint32_t debounce = 0;                         // Switch debounce timer

  uint32_t no_pullup_bitmask = 0;                   // Switch pull-up bitmask flags
  uint32_t pulldown_bitmask = 0;                    // Switch pull-down bitmask flags
  uint32_t used_bitmask = 0;                             // Switch used bitmask
  uint32_t virtual_pin_bitmask = 0;                      // Switch state bitmask


  uint8_t state[MAX_SWITCHES_SET] = { 0 };
  uint8_t last_state[MAX_SWITCHES_SET];          // Last wall switch states
  uint8_t hold_timer[MAX_SWITCHES_SET] = { 0 };  // Timer for wallswitch push button hold
  uint8_t debounced_state[MAX_SWITCHES_SET];     // Switch debounced states
  // int8_t pin[MAX_SWITCHES_SET] = { -1 };;
  // uint8_t active_state_value[MAX_SWITCHES_SET] = { 0 };

  uint8_t first_change = 0;
  bool probe_mutex = false;

  
      // uint8_t switch_virtual[MAX_SWITCHES_SET] = { 0 };;   //[MAX_SWITCHES_SET];


} Switch;

/********************************************************************************************/

bool SendKey(uint32_t key, uint32_t device, uint32_t state);
void SwitchPullupFlag(uint32_t switch_bit);
void SwitchPulldownFlag(uint32_t switch_bit);
void SwitchSetVirtualPinState(uint32_t index, uint32_t state);
uint8_t SwitchLastState(uint32_t index);
uint8_t SwitchGetState(uint32_t index);
void SwitchSetState(uint32_t index, uint32_t state);

void SetSwitchUsed(uint32_t index);
bool SwitchUsed(uint32_t index);
bool SwitchState(uint32_t index);
void SwitchProbe(void);
void SwitchHandler(void);
void SwitchLoop(void);



    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      uint8_t count = 0;
      for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
        if (bitRead(Switch.used_bitmask, i)) {
          count++;
        }
      }
      return count;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > MAX_SWITCHES_SET-1) {value->sensor_type.push_back(0); return ;}
      value->timestamp = 0; // Switches are constantly updated, so timestamp is not required. Assume "0" from now on means reading can be skipped as timeout
      value->sensor_type.push_back(SENSOR_TYPE_STATE_ACTIVE_ID);
      value->data_f.push_back(SwitchGetState(index));
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING



    #endif // ENABLE_DEVFEATURE_SWITCHES__V2
    #ifndef ENABLE_DEVFEATURE_SWITCHES__V2
    
#define SWITCH_PROBE_INTERVAL 10   // Time in milliseconds between switch input probe



static const uint8_t MAX_SWITCHES_SET = 28;        // Max number of switches
    struct SETTINGS{
      uint8_t switches_found = 0;
    }settings;

    struct SWITCHES{
      uint8_t  state     = false;
      // uint8_t  isactive  = false;
      uint8_t  ischanged = false;

      // I need to adopt tas method, as switches can expand to much larger on esp32
      // pins can stay here, but inverted will need set as flag
      int8_t pin = -1; // -1 is not active

      uint8_t active_state_value = false; //defualt active high

      uint8_t lastwallswitch;
      uint8_t switch_virtual;   //[MAX_SWITCHES_SET]; 
      uint8_t holdwallswitch;
            
      uint8_t switch_state_buf; //[MAX_SWITCHES_SET] = { 0 };

      // uint8_t is_linked_to_internal_relay = false;
      int8_t linked_internal_relay_id = -1; // -1 inactive

      // uint32_t tDetectTime;
      // uint32_t tEndedTime;
      // time_short_t detected_time;
    }switches[MAX_SWITCHES_SET];

    bool IsSwitchActive(uint8_t id);

    



    Ticker* TickerSwitch = nullptr;


    uint32_t tSavedSwitchProbe = millis();
    uint32_t switch_debounce = 0;          // Switch debounce timer
    uint16_t switch_no_pullup = 0;              // Switch pull-up bitmask flags
    
    void SetPullupFlag(uint16_t switch_bit);
    uint8_t GetLastState(uint8_t index);
    void SetVirtual(uint8_t index, uint8_t state);
    uint8_t GetVirtual(uint8_t index);


    void SwitchHandler(uint8_t mode);
    void SwitchInit(void);
    void SwitchLoop();
    void SwitchProbe(void);

    
    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return settings.switches_found;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > MAX_SWITCHES_SET-1) {value->sensor_type.push_back(0); return ;}
      value->timestamp = 0; // Switches are constantly updated, so timestamp is not required. Assume "0" from now on means reading can be skipped as timeout
      value->sensor_type.push_back(SENSOR_TYPE_STATE_ACTIVE_ID);
      value->data_f.push_back(IsSwitchActive(index));
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING



    #endif // ENABLE_DEVFEATURE_SWITCHES__V2

    

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mSwitches>*> mqtthandler_list;    
    struct handler<mSwitches> mqtthandler_settings;
    struct handler<mSwitches> mqtthandler_sensor_ifchanged;
    struct handler<mSwitches> mqtthandler_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT 


};



#endif

#endif  // _SONOFF_H_
//#endif