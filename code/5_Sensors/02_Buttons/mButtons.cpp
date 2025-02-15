
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
  Button.used_bitmap = 0;    

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
      TouchFlag(i);
    }
    #endif // SOC_TOUCH_VERSION_1
    else{
      ALOG_DBG(PSTR(D_LOG_BUTTONS "%d None"), i);
    }

    if(pin != -1)
    {
      ALOG_INF(PSTR(D_LOG_BUTTONS "%d pin=%d %s"), i, pin, toBinaryString(Button.used_bitmap, MAX_KEYS_SET).c_str() );
      Button.last_state[i] = digitalRead(pin);
    }

  }

  if(Button.used_bitmap){ module_state.mode = ModuleStatus::Initialising; }

}

void mButtons::Init(void)
{

  ALOG_INF(PSTR(D_LOG_BUTTONS "Init"));
  
#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
  touch_threshold = ESP32_TOUCH_THRESHOLD;
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2


  bool ac_detect = (tkr_set->Settings.button_debounce % 10 == 9);
//   Button.used_bitmap = 0;
// /*
//   uint32_t last_used = 0;
// */
//   for (uint32_t i = 0; i < MAX_KEYS_SET; i++) {
//     Button.last_state[i] = NOT_PRESSED;
// #ifdef ESP8266
//     if ((0 == i) && ((SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type))) {
//       bitSet(Button.used_bitmap, i);            // This pin is used
//     } else
// #endif  // ESP8266
//     if (pCONT_pins->PinUsed(GPIO_KEY1_ID, i)) {
//       bitSet(Button.used_bitmap, i);            // This pin is used
// #ifdef ESP8266
//       pinMode(pCONT_pins->Pin(GPIO_KEY1, i), bitRead(Button.no_pullup_bitmap, i) ? INPUT : ((16 == pCONT_pins->Pin(GPIO_KEY1, i)) ? INPUT_PULLDOWN_16 : INPUT_PULLUP));
// #endif  // ESP8266
// #ifdef ESP32
//       pinMode(pCONT_pins->Pin(GPIO_KEY1_ID, i), bitRead(Button.pulldown_bitmap, i) ? INPUT_PULLDOWN : bitRead(Button.no_pullup_bitmap, i) ? INPUT : INPUT_PULLUP);
// #endif  // ESP32
//       // Set global now so doesn't change the saved power state on first button check
//       Button.last_state[i] = (digitalRead(pCONT_pins->Pin(GPIO_KEY1_ID, i)) != bitRead(Button.inverted_bitmap, i));
//       if (ac_detect) {
//         Button.state[i] = 0x80 + 2 * BUTTON_AC_PERIOD;
//         Button.last_state[i] = 0;				 // Will set later in the debouncing code
//       }
//     }
// #ifdef USE_ADC
//     else if (PinUsed(GPIO_ADC_BUTTON, i) || PinUsed(GPIO_ADC_BUTTON_INV, i)) {
//       bitSet(Button.used_bitmap, i);            // This pin is used
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
//         // bitSet(Button.used_bitmap, i);                // This pin is used
//         // bool state = (XdrvMailbox.index &1);
//         // ButtonSetVirtualPinState(i, state);    // Virtual hardware pin state
//         // if (!state) { ButtonInvertFlag(i); }   // Set inverted flag
//         // // last_state[i] must be 1 to indicate no button pressed
//         // Button.last_state[i] = (bitRead(Button.virtual_pin_bitmap, i) != bitRead(Button.inverted_bitmap, i));

//         // AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Add vButton%d, State %d"), i +1, Button.last_state[i]);
//       }
//     }
//     Button.debounced_state[i] = Button.last_state[i];
// /*
//     if (bitRead(Button.used_bitmap, i)) {
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
      bitSet(Button.used_bitmap, i);                // This pin is used
      bool state = (XdrvMailbox.index &1);
      ButtonSetVirtualPinState(i, state);    // Virtual hardware pin state
      if (!state) { ButtonInvertFlag(i); }   // Set inverted flag
      // last_state[i] must be 1 to indicate no button pressed
      Button.last_state[i] = (bitRead(Button.virtual_pin_bitmap, i) != bitRead(Button.inverted_bitmap, i));

      AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: Add vButton%d, State %d"), i +1, Button.last_state[i]);
    }

    Button.debounced_state[i] = Button.last_state[i];
  }
*/

//  AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: vPinUsed %08X, State %08X, Invert %08X"), Button.used_bitmap, Button.virtual_pin_bitmap, Button.inverted_bitmap);

  TickerButton = new Ticker();

  if (Button.used_bitmap) // Any bit set
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

void mButtons::Loop(void) {
  if (Button.used_bitmap) {
    if(mTime::TimeReached(&Button.tSaved_debounce, tkr_set->Settings.button_debounce)){
      Handler();
    }
  }
}


/********************************************************************************************/

void mButtons::PullupFlag(uint32_t button_bit) {
  bitSet(Button.no_pullup_bitmap, button_bit);
}

void mButtons::PulldownFlag(uint32_t button_bit) {
  bitSet(Button.pulldown_bitmap, button_bit);
}

void mButtons::InvertFlag(uint32_t button_bit) {
  bitSet(Button.inverted_bitmap, button_bit);
}

#if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
void mButtons::TouchFlag(uint32_t button_bit) {
  bitSet(TouchButton.touch_bitmap, button_bit);
}
#endif  // ESP32 SOC_TOUCH_VERSION_1 or SOC_TOUCH_VERSION_2

/*------------------------------------------------------------------------------------------*/

void mButtons::SetVirtualPinState(uint32_t index, uint32_t state) {
  // Set virtual pin state to be debounced as used by early detected buttons
  bitWrite(Button.virtual_pin_bitmap, index, state);
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
  return (pCONT_pins->PinUsed(GPIO_KEY1_ID, index) || bitRead(Button.used_bitmap, index));
}

void mButtons::SetButtonUsed(uint32_t index) {
  bitSet(Button.used_bitmap, index);
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
    if (!bitRead(Button.used_bitmap, i)) { continue; }

    if (pCONT_pins->PinUsed(GPIO_KEY1_ID, i)) {
      not_activated = (digitalRead(pCONT_pins->GetPin(GPIO_KEY1_ID, i)) != bitRead(Button.inverted_bitmap, i));
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
      if (bitRead(TouchButton.touch_bitmap, i)) {
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
      not_activated = (bitRead(Button.virtual_pin_bitmap, i) != bitRead(Button.inverted_bitmap, i));
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
 * 
 * I think Handler, in relation to defaults, should be split. 
 * In my case, the default power options should be a separate function in this class
 * 
\*********************************************************************************************/

void mButtons::Handler(void) {
  if (tkr_time->uptime_seconds_nonreset < 4) { return; }                    // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t hold_time_extent = IMMINENT_RESET_FACTOR;            // Since hold times are 0.1/second, this gets back into seconds
  uint16_t loops_per_second = 1000 / tkr_set->Settings.button_debounce;  // ButtonDebounce (50)
  char scmnd[20];

  for (uint32_t button_index = 0; button_index < MAX_KEYS_SET; button_index++) {
    if (!bitRead(Button.used_bitmap, button_index)) { continue; }

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

    if (Used(button_index)) {

      #if defined(SOC_TOUCH_VERSION_1) || defined(SOC_TOUCH_VERSION_2)
      if (bitRead(TouchButton.touch_bitmap, button_index) && bitRead(TouchButton.calibration, button_index +1)) {  // Touch
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

    tkr_set->XdrvMailbox.index = button_index;
    tkr_set->XdrvMailbox.payload = button;
    tkr_set->XdrvMailbox.command_code = (Button.last_state[button_index] & 0xFF) | ((Button.press_counter[button_index] & 0xFF) << 8);
    if (pCONT->Tasker_Interface(TASK_BUTTON_PRESSED) == FUNCTION_RESULT_HANDLED_ID){
      // Serviced
    }
    else 
    {

      /** BUTTON PRESSED ONCE
       * Changed from NOT_PRESSED to PRESSED
       */
      if (
        (PRESSED == button) && 
        (NOT_PRESSED == Button.last_state[button_index])
      ){

        /***
         * SINGLE-PRESS: React to single press and trigger rule
         */
        if (tkr_set->Settings.flag_system.button_single_press_only) { // SetOption13 (0) - Allow only single button press for immediate action

          AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BUTTONS "#%d Immediate: %d | %s " D_IMMEDIATE), button_index, button, button==PRESSED?"ACTIVE":"Not Active" );          
                      
          SendButton(button_index, INPUT_TYPE_SINGLE_PRESS_ID);
        
        } 
        /***
         * MULTI-PRESS: Capture timing and increment count
         */
        else 
        {
          Button.press_counter[button_index] = (Button.window_timer[button_index]) ? Button.press_counter[button_index] +1 : 1;
          Button.window_timer[button_index] = uint32_t(DOUBLE_CLICK_WINDOW * loops_per_second) / 1000;
          
          AddLog(LOG_LEVEL_INFO, PSTR("BTN: Button%d multi-press %d %d"), button_index +1, Button.press_counter[button_index], Button.window_timer[button_index]);
        
          // Multipress is recorded, but not acted on until timeout of a possible additional press
        }
      }

      /**
       * BUTTON NOT PRESSED
       * No change 
       */
      if (NOT_PRESSED == button) {
        Button.hold_timer[button_index] = 0;
        /**
         * BUTTON was released, it was pressed before
         */
        if (
          (PRESSED == Button.last_state[button_index]) &&   // Button was pressed before
          !Button.press_counter[button_index]               // no multiple counter
        ){
          ALOG_INF(PSTR(D_LOG_BUTTONS "Released %d after long press: CLEAR"), button_index);

          SendButton(button_index, INPUT_TYPE_SINGLE_HOLD_RELEASED_ID);

        }
      }       
      /**
       * BUTTON HELD (PRESSED)
       * Button held: It was pressed before, and is still pressed now, hence it is held 
       */
      else 
      {
        Button.hold_timer[button_index]++;

        /**
         * Immediate actions: Single press only
         * If single-press only was enabled, then holding the button for X time will enable multipress mode. The reason for this code is single-press mode enables immediate action, while multipress adds a timeout delay to await multiple press so may not always be the best option.
         * The following code enables a runtime way of switching from single only, to, multipress enabled.
         */
        if (tkr_set->Settings.flag_system.button_single_press_only) {                  // SetOption13 (0) - Allow only single button press for immediate action
          uint16_t hold_timer_limit = loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10;
          if (Button.hold_timer[button_index] == hold_timer_limit) {  // SetOption32 (40) - Button held for factor times longer
            Button.press_counter[button_index] = 0;
            ALOG_INF(PSTR("Disable single press only SO13"));
            tkr_set->Settings.flag_system.button_single_press_only = false;        // SetOption13 (0) - "SetOption" "13 0" // Disable single press only
          }
        }
        /**
         * Immediate actions: Multi press allowed
         */
        else 
        {
          if (Button.hold_timer[button_index] == loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) {  // SetOption32 (40) - Button hold
            Button.press_counter[button_index] = 0;

            SendButton(button_index, INPUT_TYPE_SINGLE_HOLD_ID);
          
          } else {

            /***
             * SECTION: Button hold is used to system factory reset
             */
            #ifdef ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD
            if (tkr_set->Settings.flag_system.button_restrict) {            // SetOption1 (0) - Control button multipress
              if (tkr_set->Settings.setoption_255[P_HOLD_IGNORE] > 0) {      // SetOption40 (0) - Do not ignore button hold
                if (Button.hold_timer[button_index] > loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_IGNORE] / 10) {
                  Button.hold_timer[button_index] = 0;       // Reset button hold counter to stay below hold trigger
                  Button.press_counter[button_index] = 0;    // Discard button press to disable functionality
                }
              }
            } else {

              if (Button.hold_timer[button_index] == (loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10) ) {  // SetOption32 (40) - Button held for factor times longer
                ALOG_WRN(PSTR(D_LOG_BUTTONS "FACTORY RESET")); 
                Button.press_counter[button_index] = 0;
                
                SendButton(button_index, INPUT_TYPE_SINGLE_HOLD_RESET_TIME_ID);

              }else
              if (Button.hold_timer[button_index] == (loops_per_second * hold_time_extent * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 20) ) { // Warning at half way point this will cause a reset
                ALOG_WRN(PSTR(D_LOG_BUTTONS "FACTORY RESET will occur if hold is continued"));                
              }
              
            } // reset time out
            #endif // ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD

          }
        }
      }

      if (!tkr_set->Settings.flag_system.button_single_press_only)  // SetOption13 (0) - Allow multi-press
      {                   
        if (Button.window_timer[button_index]) 
        {
          Button.window_timer[button_index]--;
        } 
        else 
        {
          if (
            !tkr_set->runtime.restart_flag && 
            !Button.hold_timer[button_index] && 
            (Button.press_counter[button_index] > 0) && 
            (Button.press_counter[button_index] < MAX_BUTTON_MULTIPRESS_COUNTER)
          ){

            bool single_press = false;
            if (Button.press_counter[button_index] < 3) {    // Single or Double press
              #ifdef ESP8266
              if ((SONOFF_DUAL_R2 == TasmotaGlobal.module_type) || (SONOFF_DUAL == TasmotaGlobal.module_type) || (CH4 == TasmotaGlobal.module_type)) {
                single_press = true;
              } else
              #endif  // ESP8266
              {
                single_press = (tkr_set->Settings.flag_system.button_swap +1 == Button.press_counter[button_index]);  // SetOption11 (0)
                if ((1 == Button.used_bitmap) && 0/* (2 == TasmotaGlobal.devices_present)*/) {  // Single Button with two devices only
                  if (tkr_set->Settings.flag_system.button_swap) {          // SetOption11 (0)
                    Button.press_counter[button_index] = (single_press) ? 1 : 2;
                  }
                }
              }
            }
            
            tkr_set->XdrvMailbox.index = button_index;
            tkr_set->XdrvMailbox.payload = button;
            if (pCONT->Tasker_Interface(TASK_BUTTON_MULTI_PRESSED) == FUNCTION_RESULT_HANDLED_ID) {
              // Serviced
              AddLog(LOG_LEVEL_DEBUG, PSTR("BTN: FUNC_BUTTON_MULTI_PRESSED serviced"));
            } else

              #ifdef ROTARY_V1
              if (!RotaryButtonPressed(button_index)) {
              #endif
                if (Button.press_counter[button_index] < MAX_BUTTON_MULTIPRESS_COUNTER) { // Single to 5 press

                  SendButton(button_index, Button.press_counter[button_index] == 1 ? INPUT_TYPE_SINGLE_PRESS_ID : INPUT_TYPE_MULTIPLE_PRESS_ID, Button.press_counter[button_index]);

                } else {
                  ALOG_ERR(PSTR(D_LOG_BUTTONS "Too manny presses: Reset counter"));
                  Button.hold_timer[button_index] = 0;       // Reset button hold counter to stay below hold trigger
                  Button.press_counter[button_index] = 0;    // Discard button press to disable functionality
                }
                
              #ifdef ROTARY_V1
              }
              #endif
            Button.press_counter[button_index] = 0;
          } // less than MAX_BUTTON_MULTIPRESS_COUNTER presses, not held
        } // window_timer
      } // Allow multi-press
    }
    Button.last_state[button_index] = button;
  }
}


uint8_t mButtons::SerialProbe(uint8_t serial_in_byte) {
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
 * @brief A new wrapper I am making in place of sendkey, this make sure the button module reacts to the press event type then broadcasts it via rules to the other modules
 * 
 * @param key 
 * @param device 
 * @param state 
 * @return true 
 * @return false 
 */
bool mButtons::SendButton(uint32_t index, uint32_t state, uint16_t count)
{

  char button_name[50];
  DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), index, button_name, sizeof(button_name));  
  char state_name_ctr[30];
  pCONT_sup->GetTextIndexed_P(state_name_ctr, sizeof(state_name_ctr), state, kInputTypeButton);
    
  ALOG_INF(PSTR(D_LOG_BUTTONS "SendButton[%d|%s] type[%d|%s] count[%d]"), index, button_name, state, state_name_ctr, count);

  event.id      = index;
  event.type    = state;
  event.presses = count;
  event.waiting = true; // new event recorded

  #ifdef USE_MODULE_CORE_RULES
  tkr_rules->NewEventRun_NumArg(
      D_UNIQUE_MODULE_SENSORS_BUTTONS_ID, // Unique module ID
      TASK_EVENT_INPUT_STATE_CHANGED_ID,  // Task Event
      index,                       // Button Index
      2,                                  // Optional Encoded Data_Length
      state,                              // Encoded Data: Byte 0   "Type of press"
      count                               // Encoded Data: Byte 1   "Number of presses"        
    ); // Event has occured, save and check it    
  #endif

  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  Tasker(TASK_MQTT_SENDER);

  event.waiting = false;

}


uint8_t mButtons::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, GetSensorCount());

    for(uint32_t i=0; i<GetSensorCount(); i++)
    {
      if(bitRead(Button.used_bitmap,i))
      {
        char button_name[50];
        DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), i, button_name, sizeof(button_name));  

        JBI->Object_Start(button_name);
          JBI->Add("ID", i);
          JBI->Add("state", Button.state[i]);
          JBI->Add("last_state", Button.last_state[i]);
          JBI->Add("debounced_state", Button.debounced_state[i]);
          JBI->Add("press_counter", Button.press_counter[i]);
          // JBI->Add("hold_timer", Button.hold_timer[i]);
          JBI->Add("window_timer", Button.window_timer[i]);
          JBI->Add("active_state", Button.active_state[i]);
          /***
           * To reduce reporting, bitmapped things are only added when set
           */
          if(bitRead(Button.no_pullup_bitmap,i))   JBI->Add("no_pullup", true);
          if(bitRead(Button.pulldown_bitmap,i))    JBI->Add("pulldown", true);
          if(bitRead(Button.inverted_bitmap,i))    JBI->Add("inverted", true);
          if(bitRead(Button.used_bitmap,i))        JBI->Add("used", true);
          if(bitRead(Button.virtual_pin_bitmap,i)) JBI->Add("virtual_pin", true);
          
        JBI->Object_End();

      }
    }

  return JBI->End();

}

/**
 * @brief I dont like having the reporting here tied to rules, it must be its own thing
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
uint8_t mButtons::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();

    if(event.waiting)
    {
      JBI->Add("ID", event.id);
      char button_name[50];
      DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), event.id, button_name, sizeof(button_name));  
      JBI->Add("Name", button_name);
      JBI->Add("Type", event.type);
      char state_name_ctr[30];
      pCONT_sup->GetTextIndexed_P(state_name_ctr, sizeof(state_name_ctr), event.type, kInputTypeButton);
      JBI->Add("TypeName", state_name_ctr);
      JBI->Add("Count", event.presses);
      JBI->Add("LocalTime", tkr_time->GetTime().c_str());
    }

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

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mButtons::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif// USE_MODULE_NETWORK_MQTT


#endif