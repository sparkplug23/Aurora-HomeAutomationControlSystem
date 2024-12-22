/*
  mPZEM004T.h - PZEM004T v3 MODBUS

  Copyright (C) 2021  Michael

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
#ifndef HEADER_MODULE_DRIVERS_LEDS_H
#define HEADER_MODULE_DRIVERS_LEDS_H
  
#define D_UNIQUE_MODULE_DRIVERS_LEDS_ID 4003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_LEDS

#include <Arduino.h>
#include <vector>

#include "1_TaskerManager/mTaskerInterface.h"

// enum LedMode{
//   LED_MANUAL=0,
//   LED_SHOW_POWER,
//   LED_SHOW_STATUS, // Network, plus others
// };

enum LedEffect {
  LED_OFF,
  LED_ON,
  LED_BLINK,
  LED_PULSE
};

typedef union {
  uint8_t data;
  struct {
    uint8_t network_down : 1;  // No wifi/ethernet connect
    uint8_t mqtt_down : 1;     // No mqtt connect
    uint8_t wifi_ap_active : 1;       // In AP mode, waiting config
    uint8_t show_power : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t slave : 1;     // Controlled by another task
    uint8_t manual : 1;    // Direct control
  };
} LedMode_BitField;

struct LedState {
  LedEffect effect;
  uint8_t state;            // Current state (HIGH or LOW)
  uint8_t count;            // Number of blinks/pulses
  uint16_t period;          // Period for both blinking and pulsing in milliseconds
  uint16_t groupPause;      // Pause time between blink/pulse groups in milliseconds
  uint8_t duration_secs;    // Duration in seconds for how long the LED should blink/pulse
  uint32_t lastUpdateTime;  // Time tracking for blinking/pulsing
  uint32_t startTime;       // To track when the blinking/pulsing started
  LedMode_BitField mode;
};


class mLEDs :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mLEDs(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_DRIVERS_LEDS_CTR = D_MODULE_DRIVERS_LEDS_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_LEDS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_LEDS_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
    
    void SetInvertFlag(uint8_t b);
    bool IsUsed(uint8_t index);
    void SetUsed(uint8_t index);


    std::vector<LedState> leds;  // Vector of LedState structs to track each LED
    void Refresh_LED(uint8_t index); // Updates a specific LED by index
    void Refresh_AllLEDs();          // Loops through all LEDs and updates them

    void StartEffect_On(uint8_t index);  // Set specific LED to ON mode
    void StartEffect_Off(uint8_t index); // Set specific LED to OFF mode
    void StartEffect_Blink(uint8_t index, uint8_t blinkCount, uint16_t blinkInterval, uint16_t blinkGroupPause, uint8_t duration_secs = 0);
    void StartEffect_Pulse(uint8_t index, uint8_t pulseCount, uint16_t period, uint16_t groupPause, uint8_t duration_secs = 0) ;

    void UpdateLEDEffect(uint8_t led_index);  // Helper function to update an individual LED

    uint8_t UsedCount();

    void Set_PowerLED(bool state, uint8_t index = 255);
    void Set_StatusLED(uint8_t count, uint16_t interval, uint16_t event_pause, uint8_t duration_secs, LedEffect effect);

    void Config_StatusEffect();

    uint16_t GetLED_Output(uint8_t index);

    #define MODULE_LEDS_MAX 4

    /**
     * @brief Bitpacked LEDs
     * 
     */
    uint8_t leds_present = 0;                   // Max number of LED supported
    uint8_t inverted_bitmask = 0;                   // LED inverted flag (1 = (0 = On, 1 = Off))
    uint8_t led_power = 0;                      // LED power state
    uint8_t ledlnk_inverted = 0;                // Link LED inverted flag (1 = (0 = On, 1 = Off))
    uint8_t used_bitmask = 0; 

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true); //default is true
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mLEDs>*> mqtthandler_list;    
    struct handler<mLEDs> mqtthandler_settings;
    struct handler<mLEDs> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT 


    
};

#endif // USE_MODULE_DRIVERS_LEDS

#endif