
#ifndef _MBUTTON_H_
#define _MBUTTON_H_

#define D_UNIQUE_MODULE_SENSORS_BUTTONS_ID 5002 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BUTTONS

#include "2_CoreSystem/08_Logging/mLogging.h"

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "2_CoreSystem/07_Time/mTime.h"

#ifdef ENABLE_DEVFEATURE_BUTTON__USE_ASYNCTIMER
#include <ESPAsyncTimer.h>
#else
#include <Ticker.h>
#endif

#include "1_TaskerManager/mTaskerInterface.h"

class mButtons :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mButtons(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};
    
    static constexpr const char* PM_MODULE_SENSORS_BUTTONS_CTR = D_MODULE_SENSORS_BUTTONS_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BUTTONS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BUTTONS_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

   
/*********************************************************************************************\
 * Button support with input filter
 *
 * Inspired by (https://github.com/OLIMEX/olimex-iot-firmware-esp8266/blob/master/olimex/user/user_switch2.c)
\*********************************************************************************************/

#define MAX_RELAY_BUTTON1       5            // Max number of relay controlled by BUTTON1

#define MAX_BUTTON_MULTIPRESS_COUNTER 7

#ifndef DOUBLE_CLICK_WINDOW
 #define DOUBLE_CLICK_WINDOW 500             // Define Window size to recognize double clicks
#endif

  uint32_t      touch_threshold;           // F70

const uint8_t BUTTON_PROBE_INTERVAL = 10;      // Time in milliseconds between button input probe
const uint8_t BUTTON_FAST_PROBE_INTERVAL = 2;  // Time in milliseconds between button input probe for AC detection
const uint8_t BUTTON_AC_PERIOD = (20 + BUTTON_FAST_PROBE_INTERVAL - 1) / BUTTON_FAST_PROBE_INTERVAL;   // Duration of an AC wave in probe intervals

const char kMultiPress[38] PROGMEM = "SINGLE|DOUBLE|TRIPLE|QUAD|PENTA|CLEAR";

const char kInputTypeButton[43] PROGMEM = "Single|Multi|Hold|Hold Released|Hold Reset";

Ticker* TickerButton;

enum SendKeyOptions { KEY_BUTTON, KEY_SWITCH };

enum SendKeyPowerOptions { POWER_HOLD = 3, POWER_INCREMENT = 4, POWER_INV = 5, POWER_CLEAR = 6, POWER_RELEASE = 7,
                           POWER_100 = 8, CLEAR_RETAIN = 9, POWER_DELAYED = 10 };

enum ButtonStates { PRESSED, NOT_PRESSED };
static const uint8_t MAX_KEYS_SET = 32;            // Max number of keys 32-bit packed

struct BUTTON {
  uint32_t tSaved_debounce = 0;                 // Button debounce timer
  /***
   * 32-bitmaps
   */
  uint32_t no_pullup_bitmap = 0;                // key no pullup flag (1 = no pullup)
  uint32_t pulldown_bitmap = 0;                 // key pulldown flag (1 = pulldown)
  uint32_t inverted_bitmap = 0;                 // Key inverted flag (1 = inverted)
  uint32_t used_bitmap = 0;                     // Key used bitmask
  uint32_t virtual_pin_bitmap = 0;              // Key state bitmask
  /***
   * values held in 32 byte arrays
   */
  uint16_t hold_timer[MAX_KEYS_SET] = { 0 };    // Timer for button hold
  uint8_t  state[MAX_KEYS_SET] = { 0 };
  uint8_t  last_state[MAX_KEYS_SET];            // Last button states
  uint8_t  debounced_state[MAX_KEYS_SET];       // Button debounced states
  uint8_t  window_timer[MAX_KEYS_SET] = { 0 };  // Max time between button presses to record press count
  uint8_t  press_counter[MAX_KEYS_SET] = { 0 }; // Number of button presses within Button.window_timer
  uint8_t  active_state[MAX_KEYS_SET];             // may remove, as this can be inferred from other values when needed. Keep for now
  

  uint16_t dual_code = 0;                       // Sonoff dual received code
  uint8_t  dual_receive_count = 0;              // Sonoff dual input flag
  uint8_t first_change = 0;
  bool    probe_mutex = false;
} Button;

#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
struct TOUCH_BUTTON {
  uint32_t touch_bitmap = 0;                      // Touch flag (1 = enabled)
  uint32_t calibration = 0;                     // Bitfield
  uint8_t hits[MAX_KEYS_SET] = { 0 };           // Hits in a row to filter out noise
} TouchButton;
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2

struct EVENT{
  uint8_t id = 0;
  uint8_t presses = 0;
  uint8_t type = 0;
  bool waiting = false;
}event;


uint8_t GetHardwareSpecificPullMethod(uint8_t real_pin);
void PullupFlag(uint32_t button_bit);
void PulldownFlag(uint32_t button_bit);
void InvertFlag(uint32_t button_bit);
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
void TouchFlag(uint32_t button_bit);
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2
void SetVirtualPinState(uint32_t index, uint32_t state);
uint8_t GetState(uint32_t index);
uint8_t LastState(uint32_t index);
bool Used(uint32_t index);
void Probe(void);
uint8_t SerialProbe(uint8_t serial_in_byte);
void Handler(void);
void Loop(void);

void SetButtonUsed(uint32_t index);


bool SendButton(uint32_t device, uint32_t state, uint16_t count = 1);


  #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  uint8_t GetSensorCount(void) override
  {
    uint8_t count = 0;
    for (uint32_t i = 0; i < MAX_KEYS_SET; i++) {
      if (bitRead(Button.used_bitmap, i)) {
        count++;
      }
    }
    return count;
  }
  void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
  {
    if(index > MAX_KEYS_SET-1) {value->sensor_type.push_back(0); return ;}
    value->timestamp = Button.tSaved_debounce; // Switches are constantly updated, so timestamp is not required. Assume "0" from now on means reading can be skipped as timeout
    value->sensor_type.push_back(SENSOR_TYPE_STATE_ACTIVE_ID);
    value->data_f.push_back(GetState(index));
    value->sensor_id = index;
  };
  #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING


  uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
  uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mButtons>*> mqtthandler_list;    
    struct handler<mButtons> mqtthandler_settings;
    struct handler<mButtons> mqtthandler_sensor_ifchanged;
  #endif // USE_MODULE_NETWORK_MQTT


};



#endif

#endif  // _SONOFF_H_
//#endif