
#include "mButtons.h"

#ifdef USE_MODULE_SENSORS_BUTTONS

int8_t mButtons::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      Loop();
    break;
    /************
     * EVENTS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      // CommandSet_SDCard_OpenClose_Toggle();
   
    
      // Event for this
      if(pCONT_rules->event_triggered.module_id == D_UNIQUE_MODULE_SENSORS_BUTTONS_ID)
      {
   
        ALOG_INF(PSTR("Button State Changed1 : MQTTHandler_Sender"));
   
        // Send immediately (Button type is held in event, so ConstructJson can make the correct formatted data)
        mqtthandler_sensor_ifchanged.flags.SendNow = true;
   
      }   


    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}


/**
 * Esp8266 and esp32 will have different pull resistors
 * */
uint8_t mButtons::GetHardwareSpecificPullMethod(uint8_t real_pin)
{
  #ifdef ESP8266
  return (real_pin == 16) ? INPUT_PULLDOWN_16 : INPUT_PULLUP;
  #else //esp32
  return INPUT_PULLUP;
  #endif
}


void mButtons::Pre_Init(void)
{
  
  ALOG_INF(PSTR("Pre_Init"));

  // Check all possible pin options
  Button.used = 0;    

  // Lets check each type on their own, normal, inverted etc
  for(uint32_t i=0;i<MAX_KEYS_SET;i++)
  {
  
    int8_t pin = -1;
  
    if(pCONT_pins->PinUsed(GPIO_KEY1_ID, i))
    {
      SetButtonUsed(i);
      pin = pCONT_pins->GetPin(GPIO_KEY1_ID, i);
      pinMode(pin, GetHardwareSpecificPullMethod(pin)); // Note: GPIO16/D0 inversion is pulldown, not up
    }else    
    if(pCONT_pins->PinUsed(GPIO_KEY1_INV_ID, i))
    {
      SetButtonUsed(i);
      pin = pCONT_pins->GetPin(GPIO_KEY1_INV_ID, i);
      pinMode(pin, GetHardwareSpecificPullMethod(pin));
      InvertFlag(i);
    }else    
    if(pCONT_pins->PinUsed(GPIO_KEY1_NP_ID, i))
    {
      SetButtonUsed(i);
      pin = pCONT_pins->GetPin(GPIO_KEY1_NP_ID, i);
      pinMode(pin, INPUT);
      PullupFlag(i); 
    }else    
    if(pCONT_pins->PinUsed(GPIO_KEY1_INV_NP_ID, i))
    {
      SetButtonUsed(i);
      pin = pCONT_pins->GetPin(GPIO_KEY1_INV_NP_ID, i);
      pinMode(pin, INPUT);
      PullupFlag(i); 
      InvertFlag(i); 
    }
    #ifdef SOC_TOUCH_VERSION_1
    else
    if(pCONT_pins->PinUsed(GPIO_KEY1_TOUCH_ID, i))
    {
      SetButtonUsed(i);
      pin = pCONT_pins->GetPin(GPIO_KEY1_TOUCH_ID, i);      
      ButtonTouchFlag(i);
    }
    #endif // SOC_TOUCH_VERSION_1
    else{
      ALOG_DBG(PSTR(D_LOG_BUTTONS "%d None"), i);
    }

    if(pin != -1)
    {
      ALOG_INF(PSTR(D_LOG_BUTTONS "%d pin=%d %s"), i, pin, toBinaryString(Button.used, MAX_KEYS_SET).c_str() );
      Button.last_state[i] = digitalRead(pin);
    }

  }

  if(Button.used){ module_state.mode = ModuleStatus::Initialising; }

}

void mButtons::Init(void)
{

  ALOG_INF(PSTR(D_LOG_BUTTONS "Init"));
  
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
  touch_threshold = ESP32_TOUCH_THRESHOLD;
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2


  bool ac_detect = (tkr_set->Settings.button_debounce % 10 == 9);
//   Button.used = 0;
// /*
//   uint32_t last_used = 0;
// */
//   for (uint32_t i = 0; i < MAX_KEYS_SET; i++) {
//     Button.last_state[i] = NOT_PRESSED;
// #ifdef ESP8266
//     if ((0 == i) && ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type))) {
//       bitSet(Button.used, i);            // This pin is used
//     } else
// #endif  // ESP8266
//     if (pCONT_pins->PinUsed(GPIO_KEY1_ID, i)) {
//       bitSet(Button.used, i);            // This pin is used
// #ifdef ESP8266
//       pinMode(pCONT_pins->Pin(GPIO_KEY1, i), bitRead(Button.no_pullup_mask, i) ? INPUT : ((16 == pCONT_pins->Pin(GPIO_KEY1, i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
// #endif  // ESP8266
// #ifdef ESP32
//       pinMode(pCONT_pins->Pin(GPIO_KEY1_ID, i), bitRead(Button.pulldown_mask, i) ? INPUT_PULLDOWN : bitRead(Button.no_pullup_mask, i) ? INPUT : INPUT_PULLUP);
// #endif  // ESP32
//       // Set global now so doesn't change the saved power state on first button check
//       Button.last_state[i] = (digitalRead(pCONT_pins->Pin(GPIO_KEY1_ID, i)) != bitRead(Button.inverted_mask, i));
//       if (ac_detect) {
//         Button.state[i] = 0x80 + 2 * BUTTON_AC_PERIOD;
//         Button.last_state[i] = 0;				 // Will set later in the debouncing code
//       }
//     }
// #ifdef USE_ADC
//     else if (PinUsed(GPIO_ADC_BUTTON, i) || PinUsed(GPIO_ADC_BUTTON_INV, i)) {
//       bitSet(Button.used, i);            // This pin is used
//     }
// #endif  // USE_ADC
//     else {
//       // Insert, Skip and Append virtual buttons
//       // XdrvMailbox.index = i;
//       if (0){//XdrvCall(FUNC_ADD_BUTTON)) {
//         // At entry:
//         //   XdrvMailbox.index = button index
//         // At exit:
//         //   XdrvMailbox.index bit 0 = current state
//         // bitSet(Button.used, i);                // This pin is used
//         // bool state = (XdrvMailbox.index &1);
//         // ButtonSetVirtualPinState(i, state);    // Virtual hardware pin state
//         // if (!state) { ButtonInvertFlag(i); }   // Set inverted flag
//         // // last_state[i] must be 1 to indicate no button pressed
//         // Button.last_state[i] = (bitRead(Button.virtual_pin, i) != bitRead(Button.inverted_mask, i));

//         // AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Add vButton%d, State %d"), i +1, Button.last_state[i]);
//       }
//     }
//     Button.debounced_state[i] = Button.last_state[i];
// /*
//     if (bitRead(Button.used, i)) {
//       last_used = i +1;
//     }
// */
//   }

/*
  // Append virtual buttons
  for (uint32_t i = last_used; i < MAX_KEYS_SET; i++) {
    Button.last_state[i] = NOT_PRESSED;

    XdrvMailbox.index = i;
    if (XdrvCall(FUNC_ADD_BUTTON)) {
      // At entry:
      //   XdrvMailbox.index = button index
      // At exit:
      //   XdrvMailbox.index bit 0 = current state
      bitSet(Button.used, i);                // This pin is used
      bool state = (XdrvMailbox.index &1);
      ButtonSetVirtualPinState(i, state);    // Virtual hardware pin state
      if (!state) { ButtonInvertFlag(i); }   // Set inverted flag
      // last_state[i] must be 1 to indicate no button pressed
      Button.last_state[i] = (bitRead(Button.virtual_pin, i) != bitRead(Button.inverted_mask, i));

      AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Add vButton%d, State %d"), i +1, Button.last_state[i]);
    }

    Button.debounced_state[i] = Button.last_state[i];
  }
*/

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: vPinUsed %08X, State %08X, Invert %08X"), Button.used, Button.virtual_pin, Button.inverted_mask);

  TickerButton = new Ticker();

  if (Button.used) // Any bit set
  {
    #ifdef ESP288
    TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
      [this](void){
        this->SwitchProbe();
      }
    );
    #else // esp32
      // Using a lambda with a capture (`this`) to call the member function `ButtonProbe`.
      // The `+` is used to convert the lambda into a plain function pointer, which is required
      // because the `attach_ms` function expects a callback in the form of `void(*)(void*)`.
      // This approach is chosen because:
      // - It keeps the code compact and localized without the need for static or wrapper functions.
      // - The `this` pointer is always guaranteed to exist in the current context, ensuring safe usage.
      // - While unconventional, this method avoids unnecessary boilerplate and remains efficient.
      // Alternatives like static functions or external wrappers were avoided for simplicity.
      TickerButton->attach_ms(
          (ac_detect) ? BUTTON_FAST_PROBE_INTERVAL : BUTTON_PROBE_INTERVAL,
          +[](mButtons* instance) { instance->Probe(); },
          this
      );
    #endif
    module_state.mode = ModuleStatus::Running;
  }

}

/********************************************************************************************/

void mButtons::PullupFlag(uint32_t button_bit) {
  bitSet(Button.no_pullup_mask, button_bit);
}

void mButtons::PulldownFlag(uint32_t button_bit) {
  bitSet(Button.pulldown_mask, button_bit);
}

void mButtons::InvertFlag(uint32_t button_bit) {
  bitSet(Button.inverted_mask, button_bit);
}

#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
void mButtons::TouchFlag(uint32_t button_bit) {
  bitSet(TouchButton.touch_mask, button_bit);
}
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2

/*------------------------------------------------------------------------------------------*/

void mButtons::SetVirtualPinState(uint32_t index, uint32_t state) {
  // Set virtual pin state to be debounced as used by early detected buttons
  bitWrite(Button.virtual_pin, index, state);
}

uint8_t mButtons::GetState(uint32_t index) {
  // Get current state
  return Button.debounced_state[index];
}

uint8_t mButtons::LastState(uint32_t index) {
  // Get last state
  return Button.last_state[index];
}

/*------------------------------------------------------------------------------------------*/

bool mButtons::Used(uint32_t index) {
  return (pCONT_pins->PinUsed(GPIO_KEY1_ID, index) || bitRead(Button.used, index));
}

void mButtons::SetButtonUsed(uint32_t index) {
  bitSet(Button.used, index);
}


/*********************************************************************************************/

void mButtons::Probe(void) 
{

  if (Button.probe_mutex || (tkr_time->uptime_seconds_nonreset < 4)) { return; }  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit
  Button.probe_mutex = true;

  uint32_t state_filter;
  uint32_t first_change = Button.first_change;
  uint32_t debounce_flags = tkr_set->Settings.button_debounce % 10;
  bool force_high = (debounce_flags &1);                       // 51, 101, 151 etc
  bool force_low = (debounce_flags &2);                        // 52, 102, 152 etc
  bool ac_detect = (debounce_flags == 9);                      // 39, 49, 59 etc

  if (ac_detect) {
    if (tkr_set->Settings.button_debounce < 2 * BUTTON_AC_PERIOD * BUTTON_FAST_PROBE_INTERVAL + 9) {
      state_filter = 2 * BUTTON_AC_PERIOD;
    } else if (tkr_set->Settings.button_debounce > (0x7f - 2 * BUTTON_AC_PERIOD) * BUTTON_FAST_PROBE_INTERVAL) {
      state_filter = 0x7f;
    } else {
      state_filter = (tkr_set->Settings.button_debounce - 9) / BUTTON_FAST_PROBE_INTERVAL;
    }
  } else {
    state_filter = tkr_set->Settings.button_debounce / BUTTON_PROBE_INTERVAL;	// 5, 10, 15
  }

  uint32_t not_activated;
  for (uint32_t i = 0; i < MAX_KEYS_SET; i++) {
    if (!bitRead(Button.used, i)) { continue; }

    if (pCONT_pins->PinUsed(GPIO_KEY1_ID, i)) {
      not_activated = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID, i)) != bitRead(Button.inverted_mask, i));
    } else if (pCONT_pins->PinUsed(GPIO_KEY1_INV_ID, i)) { // Inverted pin, active low
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_KEY1_INV_ID, i));
    } else if (pCONT_pins->PinUsed(GPIO_KEY1_NP_ID, i)) { // No pull, active high
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_KEY1_NP_ID, i));
    } else if (pCONT_pins->PinUsed(GPIO_KEY1_INV_NP_ID, i)) { // Inverted pin, no pull, active low
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_KEY1_INV_NP_ID, i));
    } 
    #if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
    else
    if (pCONT_pins->PinUsed(GPIO_KEY1_TOUCH_ID, i)) {
      if (bitRead(TouchButton.touch_mask, i)) {
        if (ac_detect || bitRead(TouchButton.calibration, i +1)) { continue; }  // Touch is slow. Takes 21mS to read
        uint32_t value = touchRead( pCONT_pins->Pin(GPIO_KEY1_TOUCH_ID, i));
        #ifdef SOC_TOUCH_VERSION_2
        not_activated = (value < touch_threshold);  // ESPS3 No touch = 24200, Touch > 40000
        #else
        not_activated = ((value == 0) || (value > touch_threshold));  // ESP32 No touch = 74, Touch < 40
        #endif
      }
    }   
    #endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2
    else {
      not_activated = (bitRead(Button.virtual_pin, i) != bitRead(Button.inverted_mask, i));
    }

    ALOG_DBM(PSTR(D_LOG_BUTTONS "not_activated %d"), not_activated);

    if (not_activated) {

      if (ac_detect) {                                         // Enabled with ButtonDebounce x9
        Button.state[i] |= 0x80;
        if (Button.state[i] > 0x80) {
          Button.state[i]--;
          if (0x80 == Button.state[i]) {
            Button.debounced_state[i] = 0;
            Button.first_change = false;
          }
        }
      } else {

        if (force_high) {                                      // Enabled with ButtonDebounce x1
          if (1 == Button.debounced_state[i]) {
            Button.state[i] = state_filter;                    // With noisy input keep current state 1 unless constant 0
          }
        }

        if (Button.state[i] < state_filter) {
          Button.state[i]++;
          if (state_filter == Button.state[i]) {
            Button.debounced_state[i] = 1;
          }
        }
      }
    } else {

      if (ac_detect) {                                         // Enabled with ButtonDebounce x9
        /*
          * Moes MS-104B and similar devices using an AC detection circuitry
          * on their switch inputs generating an ~4 ms long low pulse every
          * AC wave. We start the time measurement on the falling edge.
          *
          * state: bit7: previous state, bit6..0: counter
          */
        if (Button.state[i] & 0x80) {
          Button.state[i] &= 0x7f;
          if (Button.state[i] < state_filter - 2 * BUTTON_AC_PERIOD) {
            Button.state[i] += 2 * BUTTON_AC_PERIOD;
          } else {
            Button.state[i] = state_filter;
            Button.debounced_state[i] = 1;
            if (first_change) {
              Button.last_state[i] = 1;
              Button.first_change = false;
            }
          }
        } else {
          if (Button.state[i] > 0x00) {
            Button.state[i]--;
            if (0x00 == Button.state[i]) {
              Button.debounced_state[i] = 0;
              Button.first_change = false;
            }
          }
        }
      } else {

        if (force_low) {                                       // Enabled with ButtonDebounce x2
          if (0 == Button.debounced_state[i]) {
            Button.state[i] = 0;                               // With noisy input keep current state 0 unless constant 1
          }
        }

        if (Button.state[i] > 0) {
          Button.state[i]--;
          if (0 == Button.state[i]) {
            Button.debounced_state[i] = 0;
          }
        }
      }
    }
  }
  Button.probe_mutex = false;
}

/*********************************************************************************************\
 * Button handler with single press only or multi-press and hold on all buttons
 *
 * ButtonDebounce (50) - Debounce time in mSec
 * SetOption1  (0)     - If set do not execute commands WifiConfig and Reset
 * SetOption11 (0)     - If set perform single press action on double press and reverse (on two relay devices only)
 * SetOption13 (0)     - If set act on single press only
 * SetOption32 (40)    - Button held for factor times longer
 * SetOption40 (0)     - Do not ignore button hold
 * SetOption73 (0)     - Decouple button from relay and send just mqtt topic
\*********************************************************************************************/

void mButtons::Handler(void) {
  if (tkr_time->uptime_seconds_nonreset < 4) { return; }                    // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;            // Extent hold time factor in case of iminnent Reset command
  uint16_t loops_per_second = 1000 / tkr_set->Settings.button_debounce;  // ButtonDebounce (50)
  char scmnd[20];

  for (uint32_t button_index = 0; button_index < MAX_KEYS_SET; button_index++) {
    if (!bitRead(Button.used, button_index)) { continue; }

    uint8_t button = Button.debounced_state[button_index];

    // ALOG_INF(PSTR("button %d %d"), button_index, button);

#ifdef ESP8266
    if (!button_index && ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type))) {
      if (Button.dual_code) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Code %04X"), Button.dual_code);
        button = PRESSED;
        if (0xF500 == Button.dual_code) {                      // Button hold
          Button.hold_timer[button_index] = (loops_per_second * tkr_set->Settings.param[P_HOLD_TIME] / 10) -1;  // SetOption32 (40)
          hold_time_extent = 1;
        }
        Button.dual_code = 0;
      } else {
        button = NOT_PRESSED;
      }
    } else
#endif  // ESP8266
    // if (pCONT_pins->PinUsed(GPIO_KEY1_ID, button_index)) { // cant do this, since we must check every button, best to use bitmask check
    if (Used(button_index)) {

#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
      if (bitRead(TouchButton.touch_mask, button_index) && bitRead(TouchButton.calibration, button_index +1)) {  // Touch
        uint32_t _value = touchRead( pCONT_pins->Pin(GPIO_KEY1_TOUCH_ID, button_index) );
#ifdef SOC_TOUCH_VERSION_2
        if (_value > tkr_set->Settings.touch_threshold) {  // ESPS3 No touch = 24200, Touch = 100000
#else
        if ((_value > 0) && (_value < touch_threshold)) {  // ESP32 No touch = 74, Touch = 20 (Probably read-error (0))
#endif
          TouchButton.hits[button_index]++;
        } else {
          TouchButton.hits[button_index] = 0;
        }
        AddLog(LOG_LEVEL_INFO, PSTR("PLOT: %u, %u, %u,"), button_index +1, _value, TouchButton.hits[button_index]);  // Button number (1..4), value, continuous hits under threshold
        continue;
      }
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2

    }
#ifdef USE_ADC
#ifndef FIRMWARE_MINIMAL
    else if (PinUsed(GPIO_ADC_BUTTON, button_index)) {
      button = AdcGetButton(Pin(GPIO_ADC_BUTTON, button_index));
    }
    else if (PinUsed(GPIO_ADC_BUTTON_INV, button_index)) {
      button = AdcGetButton(Pin(GPIO_ADC_BUTTON_INV, button_index));
    }
#endif  // FIRMWARE_MINIMAL
#endif  // USE_ADC

    // XdrvMailbox.index = button_index;
    // XdrvMailbox.payload = button;
    // XdrvMailbox.command_code = (Button.last_state[button_index] & 0xFF) | ((Button.press_counter[button_index] & 0xFF) << 8);
    if (0){//XdrvCall(FUNC_BUTTON_PRESSED)) {
      // Serviced
    }
#ifdef ESP8266
    else if (SONOFF_4CHPRO == TasmotaGlobal.module_type) {
      if (Button.hold_timer[button_index]) { Button.hold_timer[button_index]--; }

      bool button_pressed = false;
      if ((PRESSED == button) && (NOT_PRESSED == Button.last_state[button_index])) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Button%d level 1-0"), button_index +1);
        Button.hold_timer[button_index] = loops_per_second;
        button_pressed = true;
      }
      if ((NOT_PRESSED == button) && (PRESSED == Button.last_state[button_index])) {
        AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Button%d level 0-1"), button_index +1);
        if (!Button.hold_timer[button_index]) { button_pressed = true; }  // Do not allow within 1 second
      }
      if (button_pressed) {
        if (!Settings->flag3.mqtt_buttons) {                 // SetOption73 (0) - Decouple button from relay and send just mqtt topic
          if (!SendKey(KEY_BUTTON, button_index +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
            ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
          }
        } else {
          MqttButtonTopic(button_index +1, 1, 0);            // SetOption73 (0) - Decouple button from relay and send just mqtt topic
        }
      }
    }
#endif  // ESP8266
    else {
      if ((PRESSED == button) && (NOT_PRESSED == Button.last_state[button_index])) {

        if (tkr_set->Settings.flag_system.button_single) {                  // SetOption13 (0) - Allow only single button press for immediate action,
          if (!tkr_set->Settings.flag3.mqtt_buttons) {               // SetOption73 (0) - Decouple button from relay and send just mqtt topic
            AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Button%d immediate"), button_index +1);
            // if (!SendKey(KEY_BUTTON, button_index +1, POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
            //   ExecuteCommandPower(button_index +1, POWER_TOGGLE, SRC_BUTTON);  // Execute Toggle command internally
            // }
            AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BUTTONS "#%d Changed : Level %d | %s " D_IMMEDIATE), button_index, button, button==PRESSED?"ACTIVE":"Not Active" );          
                                                                                                          // <length of data>,<state>,<type ie single/multi/hold><count>  
            pCONT_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, TASK_EVENT_INPUT_STATE_CHANGED_ID, button_index, 2, button, INPUT_TYPE_SINGLE_PRESS_ID); // 1 press event
        
          } else {
            MqttButtonTopic(button_index +1, 1, 0);          // SetOption73 1 - Decouple button from relay and send just mqtt topic
          }
        } else {
          Button.press_counter[button_index] = (Button.window_timer[button_index]) ? Button.press_counter[button_index] +1 : 1;
          AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Button%d multi-press %d"), button_index +1, Button.press_counter[button_index]);
          Button.window_timer[button_index] = uint32_t(DOUBLE_CLICK_WINDOW * loops_per_second) / 1000;
        }
        // TasmotaGlobal.blinks = 201;
      }

      if (NOT_PRESSED == button) {
        Button.hold_timer[button_index] = 0;
        if (tkr_set->Settings.flag3.mqtt_buttons && (PRESSED == Button.last_state[button_index]) && !Button.press_counter[button_index]) { // SetOption73 (0) - Decouple button from relay and send just mqtt topic
          MqttButtonTopic(button_index +1, 6, 0);
        }
      } else {
        Button.hold_timer[button_index]++;
        if (tkr_set->Settings.flag_system.button_single) {                  // SetOption13 (0) - Allow only single button press for immediate action
          if (Button.hold_timer[button_index] == loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button held for factor times longer
            snprintf_P(scmnd, sizeof(scmnd), PSTR("SetOption" "13 0"));  // Disable single press only
            ALOG_INF(PSTR("button was held : ExecuteCommand(scmnd, SRC_BUTTON);"));
            pCONT_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, TASK_EVENT_INPUT_STATE_CHANGED_ID, button_index, 2, button, INPUT_TYPE_SINGLE_HOLD_ID);    // ERROR - Not sure what this section will do, long press no multi?
          
          }
        } else {
          if (Button.hold_timer[button_index] == loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button hold
            Button.press_counter[button_index] = 0;
            if (tkr_set->Settings.flag3.mqtt_buttons) {              // SetOption73 (0) - Decouple button from relay and send just mqtt topic
              ALOG_INF(PSTR("Decouple button from relay and send just mqtt topic MqttButtonTopic(button_index +1, 3, 1);"));
            } else {
              ALOG_INF(PSTR("SendKey(KEY_BUTTON, button_index +1, POWER_HOLD);"));  // Execute Hold command via MQTT if ButtonTopic is set
            }
            pCONT_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, TASK_EVENT_INPUT_STATE_CHANGED_ID, button_index, 2, button, INPUT_TYPE_SINGLE_HOLD_ID); 
          
          } else {
            if (tkr_set->Settings.flag_system.button_restrict) {            // SetOption1 (0) - Control button multipress
              if (tkr_set->Settings.setoption_255[P_HOLD_IGNORE] > 0) {      // SetOption40 (0) - Do not ignore button hold
                if (Button.hold_timer[button_index] > loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_IGNORE] / 10) {
                  Button.hold_timer[button_index] = 0;       // Reset button hold counter to stay below hold trigger
                  Button.press_counter[button_index] = 0;    // Discard button press to disable functionality
                }
              }
            } else {
              if ((Button.hold_timer[button_index] == loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10)) {  // SetOption32 (40) - Button held for factor times longer
                Button.press_counter[button_index] = 0;
                snprintf_P(scmnd, sizeof(scmnd), PSTR(D_CMND_RESET " 1"));
                ALOG_INF(PSTR("The tasmota way was this caused a system reset, I want to decouple : ExecuteCommand(scmnd, SRC_BUTTON);"));
                // pCONT_rules->NewEventRun_NumArg(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, TASK_EVENT_INPUT_STATE_CHANGED_ID, id, 2, state, INPUT_TYPE_SINGLE_HOLD_ID);    // Resetting command
                
              }
            }
          }
        }
      }

      if (!tkr_set->Settings.flag_system.button_single) {                   // SetOption13 (0) - Allow multi-press
        if (Button.window_timer[button_index]) {
          Button.window_timer[button_index]--;
        } else {
          if (!tkr_set->runtime.restart_flag && !Button.hold_timer[button_index] && (Button.press_counter[button_index] > 0) && (Button.press_counter[button_index] < 7)) {

            bool single_press = false;
            if (Button.press_counter[button_index] < 3) {    // Single or Double press
#ifdef ESP8266
              if ((SONOFF_DUAL_R2 == TasmotaGlobal.module_type) || (SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
                single_press = true;
              } else
#endif  // ESP8266
              {
                single_press = (tkr_set->Settings.flag_system.button_swap +1 == Button.press_counter[button_index]);  // SetOption11 (0)
                if ((1 == Button.used) && 0/* (2 == TasmotaGlobal.devices_present)*/) {  // Single Button with two devices only
                  if (tkr_set->Settings.flag_system.button_swap) {          // SetOption11 (0)
                    Button.press_counter[button_index] = (single_press) ? 1 : 2;
                  }
                }
              }
            }
/*********************************
 * 
 * 
 * 
 * Section below is what to do with multipress events, but I want these as rules only, so likely to be removed (merged into "Events")
 * 
 */
            // XdrvMailbox.index = button_index;
            // XdrvMailbox.payload = Button.press_counter[button_index];
            if (0){//XdrvCall(FUNC_BUTTON_MULTI_PRESSED)) {       // I actually do want to implement this, though really rules should handle it anyway
              // Serviced
//                AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: FUNC_BUTTON_MULTI_PRESSED serviced"));
            } else

#ifdef ROTARY_V1
            if (!RotaryButtonPressed(button_index)) {
#endif
              if (!tkr_set->Settings.flag3.mqtt_buttons && single_press && 0){//SendKey(KEY_BUTTON, button_index + Button.press_counter[button_index], POWER_TOGGLE)) {  // Execute Toggle command via MQTT if ButtonTopic is set
                // Success
              } else {
                if (Button.press_counter[button_index] < 6) { // Single to Penta press
                  if (!tkr_set->Settings.flag3.mqtt_buttons) {       // SetOption73 - Detach buttons from relays and enable MQTT action state for multipress
                    if (Button.press_counter[button_index] == 1) {  // By default first press always send a TOGGLE (2)
                      ALOG_INF(PSTR("By default first press always send a TOGGLE (2) ExecuteCommandPower(button_index + Button.press_counter[button_index], POWER_TOGGLE, SRC_BUTTON);"));
                    } else {
                      SendKey(KEY_BUTTON, button_index +1, Button.press_counter[button_index] +9);    // 2,3,4 and 5 press send just the key value (11,12,13 and 14) for rules
                      // if (0 == button_index) {               // BUTTON1 can toggle up to 5 relays if present. If a relay is not present will send out the key value (2,11,12,13 and 14) for rules
                      //   uint32_t max_device = (TasmotaGlobal.devices_present < MAX_RELAY_BUTTON1) ? TasmotaGlobal.devices_present : MAX_RELAY_BUTTON1;
                      //   if ((Button.press_counter[button_index] > 1) && (Button.press_counter[button_index] <= max_device)) {
                      //     ALOG_INF(PSTR("ExecuteCommandPower(button_index + Button.press_counter[button_index], POWER_TOGGLE, SRC_BUTTON);"));   // Execute Toggle command internally
                      //   }
                      // }
                    }
                  }

                } else {    // 6 press start wificonfig 2
                  if (!tkr_set->Settings.flag_system.button_restrict) {     // SetOption1  - Control button multipress
                    snprintf_P(scmnd, sizeof(scmnd), PSTR("WifiConfig" " 2"));
                    ALOG_INF(PSTR("ExecuteCommand(scmnd, SRC_BUTTON);"));
                  }
                }
                if (tkr_set->Settings.flag3.mqtt_buttons) {          // SetOption73 (0) - Decouple button from relay and send just mqtt topic
                  if (Button.press_counter[button_index] >= 1 && Button.press_counter[button_index] <= 5) {
                    ALOG_INF(PSTR("MqttButtonTopic(button_index +1, Button.press_counter[button_index], 0);"));
                  }
                }
              }
#ifdef ROTARY_V1
            }
#endif
            Button.press_counter[button_index] = 0;
          }
        }
      }
    }
    Button.last_state[button_index] = button;
  }
}


bool mButtons::SendKey(uint32_t key, uint32_t device, uint32_t state)
{
// key 0 = KEY_BUTTON = button_topic
// key 1 = KEY_SWITCH = switch_topic
// state 0 = POWER_OFF = off
// state 1 = POWER_ON = on
// state 2 = POWER_TOGGLE = toggle
// state 3 = POWER_HOLD = hold
// state 4 = POWER_INCREMENT = button still pressed
// state 5 = POWER_INV = button released
// state 6 = POWER_CLEAR = button released
// state 7 = POWER_RELEASE = button released
// state 9 = CLEAR_RETAIN = clear retain flag
// state 10 = POWER_DELAYED = button released delayed

ALOG_INF(PSTR(D_LOG_BUTTONS "SendKey %d %d %d"), key, device, state);


}

uint8_t mButtons::Serial(uint8_t serial_in_byte) {
  if (Button.dual_receive_count) {
    Button.dual_receive_count--;
    if (Button.dual_receive_count) {
      Button.dual_code = (Button.dual_code << 8) | serial_in_byte;
      serial_in_byte = 0;
    } else {
      if (serial_in_byte != 0xA1) {
        Button.dual_code = 0;                                  // 0xA1 - End of Sonoff dual button code
      }
    }
  }
  if (0xA0 == serial_in_byte) {                                // 0xA0 - Start of Sonoff dual button code
    serial_in_byte = 0;
    Button.dual_code = 0;
    Button.dual_receive_count = 3;
  }

  return serial_in_byte;
}

/**
 * @brief Event of button topic, bypassing my mqtt method to format what the message should be, refine later
 * 
 * @param button_id 
 * @param action 
 * @param hold 
 */
void mButtons::MqttButtonTopic(uint32_t button_id, uint32_t action, uint32_t hold) {

  ALOG_INF(PSTR("MqttButtonTopic"));

  // SendKey(KEY_BUTTON, button_id, (hold) ? 3 : action +9);

  // if (!tkr_set->Settings.flag.hass_discovery) {                        // SetOption19 - Control Home Assistant automatic discovery (See SetOption59)
  //   char scommand[10];
  //   snprintf_P(scommand, sizeof(scommand), PSTR(D_JSON_BUTTON "%d"), button_id);
  //   char mqttstate[7];
  //   Response_P(S_JSON_SVALUE_ACTION_SVALUE, scommand, (hold) ? SettingsText(SET_STATE_TXT4) : GetTextIndexed(mqttstate, sizeof(mqttstate), action, kMultiPress));
  //   MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, scommand);
  // }
}

void mButtons::Loop(void) {
  if (Button.used) {
    if(mTime::TimeReached(&Button.debounce, tkr_set->Settings.button_debounce)){
      Handler();
    }
  }
}


uint8_t mButtons::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, Button.used);
  return JBI->End();

}

uint8_t mButtons::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Array_AddArray("lastbutton", lastbutton, sizeof(lastbutton));

    // JBI->Object_Start("ButtonPressed");
    //   JBI->Add("IsButtonActiveCtr", buttons[0].isactive);
    // JBI->Object_End();


    // /**
    //  * @brief New method to show type of press (short/long/multi)
    //  **/
    // JBI->Object_Start("Event"); // asumes only one button at a time, will need nicer formatting later (arrays?)
    //   JBI->Add("ID", pCONT_rules->event_triggered.device_id);
    //   // JBI->Add("func", pCONT_rules->event_triggered.function_id);
    //   // JBI->Array_AddArray("data1", pCONT_rules->event_triggered.value.data, pCONT_rules->event_triggered.value.length);

    //   // [state][type][opt. count]
    //   if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_PRESS_ID)
    //   {
    //     JBI->Add("type", "Single");
    //     JBI->Add("count", 1);
    //   }
    //   if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_HOLD_ID)
    //   {
    //     JBI->Add("type", "Hold");
    //     JBI->Add("count", pCONT_rules->event_triggered.value.data[2]);
    //   }
    //   if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_MULTIPLE_PRESS_ID)
    //   {
    //     JBI->Add("type", "Multiple");
    //     JBI->Add("count", pCONT_rules->event_triggered.value.data[2]);
    //   }

    // JBI->End();



    // JBI->Add("pin",    buttons[0].pin);
    // JBI->Add("dpin",   digitalRead(buttons[0].pin));
    // JBI->Array_Start("bit_set_invert");
    //   for(int i=0;i<MAX_KEYS;i++){ JBI->Add(bitRead(key_inverted, i)); }
    // JBI->Array_End();
    // JBI->Array_Start("state");
    //   for(int i=0;i<MAX_KEYS;i++){ JBI->Add(buttons[i].state); }
    // JBI->Array_End();

  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mButtons::MQTTHandler_Init(){

  struct handler<mButtons>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mButtons::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif// USE_MODULE_NETWORK_MQTT


#endif