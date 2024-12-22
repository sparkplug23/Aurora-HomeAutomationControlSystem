
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

   
#ifdef ENABLE_DEVFEATURE_BUTTON__V2

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
void ButtonPullupFlag(uint32_t button_bit);
void ButtonPulldownFlag(uint32_t button_bit);
void ButtonInvertFlag(uint32_t button_bit);
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
void ButtonTouchFlag(uint32_t button_bit);
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2
void ButtonSetVirtualPinState(uint32_t index, uint32_t state);
uint8_t ButtonGetState(uint32_t index);
uint8_t ButtonLastState(uint32_t index);
bool ButtonUsed(uint32_t index);
void ButtonProbe(void);
uint8_t ButtonSerial(uint8_t serial_in_byte);
void ButtonHandler(void);
void MqttButtonTopic(uint32_t button_id, uint32_t action, uint32_t hold);
void ButtonLoop(void);
bool SendKey(uint32_t key, uint32_t device, uint32_t state);

void SetButtonUsed(uint32_t index);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
#endif // ENABLE_DEVFEATURE_BUTTON__V2

  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mButtons>*> mqtthandler_list;    
    struct handler<mButtons> mqtthandler_settings;
    struct handler<mButtons> mqtthandler_sensor_ifchanged;
    struct handler<mButtons> mqtthandler_sensor_teleperiod;
  #endif // USE_MODULE_NETWORK_MQTT

#ifdef ENABLE_DEVFEATURE_BUTTON__V1

// /*********************************************************************************************\
//  * Button support
// \*********************************************************************************************/

enum ButtonStates 
{ 
  BUTTON_PRESSED_ID, 
  BUTTON_NOT_PRESSED_ID 
};



    struct SETTINGS{
      uint8_t buttons_found = 0;
      uint8_t fEnableSensor = false;

    }settings;

    
bool ModuleEnabled();

#ifndef ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_RELAY_KEYS_DEFINES_TO_SETTINGS_HEADER

#ifndef MAX_KEYS
#define MAX_KEYS 8                 // Max number of keys or buttons
#endif // MAX_KEYS

#endif
Ticker* TickerButton;

void SetPullupFlag(uint8_t button_bit);
void SetInvertFlag(uint8_t button_bit);
void ButtonHandler(void);

void ButtonLoop(void);
bool IsButtonActive(uint8_t id);
char* IsButtonActiveCtr(uint8_t id, char* buffer, uint8_t buflen);



uint8_t GetHardwareSpecificPullMethod(uint8_t real_pin);

// unsigned long button_debounce = 0;          // Button debounce timer

struct BUTTONS{
  uint16_t hold_timer =0;      // Timer for button hold
  uint8_t window_timer = 0;//multiwindow = 0;      // Max time between button presses to record press count
  uint8_t press_counter = 0;//multipress = 0;       // Number of button presses within multiwindow
  
  // uint8_t lastbutton_active_state = BUTTON_NOT_PRESSED_ID;  // Last button states
  uint8_t last_state = BUTTON_NOT_PRESSED_ID;  // Last button states


  uint8_t active_state_value = false; //defualt active high

  /**
   * @note isactive will always signify active or not
   * */
  uint8_t  isactive     = false;
  uint8_t  state     = BUTTON_NOT_PRESSED_ID;
  // uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  int8_t pin = -1; // -1 is not active

}buttons[MAX_KEYS];

/**
 * @brief Need button event, so its stored as last event
 * Single/Multi/Hold
 * 
 * If the event is reported BEFORE rules_event can clear, then it can be used in the json mqtt message
 * 
 * 
 **/
// struct BUTTON_EVENT{
//   uint8_t type_id = INPUT_TYPE_LENGTH_ID;
//   uint8_t device_id = 0;
//   uint8_t state = 0;
//   uint8_t count = 0;
// }last_event;


uint8_t dual_hex_code = 0;                  // Sonoff dual input flag
uint8_t key_no_pullup = 0x00;                  // key no pullup flag (1 = no pullup)
uint8_t key_inverted = 0x00; // Must be set to 0, 8 bits wide                   // Key inverted flag (1 = inverted)
// uint8_t buttons_found = 0;                  // Number of buttons found flag

// timereached_t tsaved_button_debounce;
uint32_t tsaved_button_debounce;

uint16_t dual_button_code = 0;              // Sonoff dual received code



    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
  
#endif // ENABLE_DEVFEATURE_BUTTON__V1

};



#endif

#endif  // _SONOFF_H_
//#endif