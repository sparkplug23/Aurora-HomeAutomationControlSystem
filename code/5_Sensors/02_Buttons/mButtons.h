
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

#ifndef DOUBLE_CLICK_WINDOW
 #define DOUBLE_CLICK_WINDOW 500             // Define Window size to recognize double clicks
#endif

  uint32_t      touch_threshold;           // F70

const uint8_t BUTTON_PROBE_INTERVAL = 10;      // Time in milliseconds between button input probe
const uint8_t BUTTON_FAST_PROBE_INTERVAL = 2;  // Time in milliseconds between button input probe for AC detection
const uint8_t BUTTON_AC_PERIOD = (20 + BUTTON_FAST_PROBE_INTERVAL - 1) / BUTTON_FAST_PROBE_INTERVAL;   // Duration of an AC wave in probe intervals

const char kMultiPress[40] PROGMEM = "|SINGLE|DOUBLE|TRIPLE|QUAD|PENTA|CLEAR|";

Ticker* TickerButton;

enum SendKeyOptions { KEY_BUTTON, KEY_SWITCH };

enum ButtonStates { PRESSED, NOT_PRESSED };
static const uint8_t MAX_KEYS_SET = 32;            // Max number of keys

struct BUTTON {
  uint32_t debounce = 0;                        // Button debounce timer
  uint32_t no_pullup_mask = 0;                  // key no pullup flag (1 = no pullup)
  uint32_t pulldown_mask = 0;                   // key pulldown flag (1 = pulldown)
  uint32_t inverted_mask = 0;                   // Key inverted flag (1 = inverted)
  uint32_t used = 0; //rename _mask                            // Key used bitmask
  uint32_t virtual_pin = 0;                     // Key state bitmask
  uint16_t hold_timer[MAX_KEYS_SET] = { 0 };    // Timer for button hold
  uint16_t dual_code = 0;                       // Sonoff dual received code
  uint8_t state[MAX_KEYS_SET] = { 0 };
  uint8_t last_state[MAX_KEYS_SET];             // Last button states
  uint8_t debounced_state[MAX_KEYS_SET];        // Button debounced states
  uint8_t window_timer[MAX_KEYS_SET] = { 0 };   // Max time between button presses to record press count
  uint8_t press_counter[MAX_KEYS_SET] = { 0 };  // Number of button presses within Button.window_timer
  uint8_t dual_receive_count = 0;               // Sonoff dual input flag

  uint8_t active_state[MAX_KEYS_SET];             // may remove, as this can be inferred from other values when needed. Keep for now

  uint8_t first_change = 0;
  bool probe_mutex = false;
} Button;

#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
struct TOUCH_BUTTON {
  uint32_t touch_mask = 0;                      // Touch flag (1 = enabled)
  uint32_t calibration = 0;                     // Bitfield
  uint8_t hits[MAX_KEYS_SET] = { 0 };           // Hits in a row to filter out noise
} TouchButton;
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2


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
uint8_t Serial(uint8_t serial_in_byte);
void Handler(void);
void MqttButtonTopic(uint32_t button_id, uint32_t action, uint32_t hold);
void Loop(void);
bool SendKey(uint32_t key, uint32_t device, uint32_t state);

void SetButtonUsed(uint32_t index);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mButtons>*> mqtthandler_list;    
    struct handler<mButtons> mqtthandler_settings;
    struct handler<mButtons> mqtthandler_sensor_ifchanged;
    struct handler<mButtons> mqtthandler_sensor_teleperiod;
  #endif // USE_MODULE_NETWORK_MQTT


};



#endif

#endif  // _SONOFF_H_
//#endif