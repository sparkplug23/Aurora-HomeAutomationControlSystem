
#include "mSwitches.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

int8_t mSwitches::Tasker(uint8_t function, JsonParserObject obj)
{

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

  switch(function){
    case TASK_LOOP: 
      SwitchLoop();
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

}

#ifdef ENABLE_DEVFEATURE_SWITCHES__V2


void mSwitches::Pre_Init(void) 
{

  ALOG_HGL( PSTR("D_LOG_STARTUP" "Switches Init") );

  // Clear the bitmask so all 32 bits are 0
  Switch.used_bitmask = 0;

  // Check all possible pin options
  for(uint8_t i=0;i<MAX_SWITCHES_SET;i++)
  {
    int8_t pin = -1;
    /**
     * Note: This loop checks each number, for each type within each loop (only one else may match)
     * All types of SWT1 (INV, NP etc)
     **/
    if(pCONT_pins->PinUsed(GPIO_SWT1_ID, i))
    { 
      SetSwitchUsed(i);
      pin = pCONT_pins->GetPin(GPIO_SWT1_ID, i);
      pinMode(pin, INPUT_PULLUP);
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_INV_ID, i)) // Inverted pin, active low, with pulls
    {    
      SetSwitchUsed(i);
      pin = pCONT_pins->GetPin(GPIO_SWT1_INV_ID, i);
      pinMode(pin, INPUT_PULLUP);
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_NP_ID, i)) // Standard pin, active high, NO pulls
    {
      SetSwitchUsed(i);
      pin = pCONT_pins->GetPin(GPIO_SWT1_NP_ID, i);
      pinMode(pin, INPUT);
      SwitchPullupFlag(i);
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_INV_NP_ID, i))
    {    
      SetSwitchUsed(i);
      pin = pCONT_pins->GetPin(GPIO_SWT1_INV_NP_ID, i);
      pinMode(pin, INPUT);
      SwitchPulldownFlag(i);
    }else{
      ALOG_DBG(PSTR(D_LOG_SWITCHES "%d None"), i);
    }

    if(pin != -1)
    {
      ALOG_INF(PSTR(D_LOG_SWITCHES "%d pin=%d %s"), i, pin, toBinaryString(Switch.used_bitmask, MAX_SWITCHES_SET).c_str() );
      Switch.last_state[i] = digitalRead(pin);
    }

  }

}


void mSwitches::Init(void) {

  bool ac_detect = (pCONT_set->Settings.switch_debounce % 10 == 9);
  
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) 
  {
    if(bitRead(Switch.used_bitmask, i))
    {
      if (ac_detect) {
        Switch.state[i] = 0x80 + 2 * SWITCH_AC_PERIOD;
        Switch.last_state[i] = 0;				 // Will set later in the debouncing code
      }
    }
    Switch.debounced_state[i] = Switch.last_state[i];
  }

  if (Switch.used_bitmask)  // Any bit set
  {   
  
    TickerSwitch = new Ticker();
    
    #ifdef ESP288
    TickerSwitch->attach_ms(
      (ac_detect) ? SWITCH_FAST_PROBE_INTERVAL : SWITCH_PROBE_INTERVAL, 
      [this](void){
        this->SwitchProbe();
      }
    );
    #else // esp32
    TickerSwitch->attach_ms(
      (ac_detect) ? SWITCH_FAST_PROBE_INTERVAL : SWITCH_PROBE_INTERVAL,
      +[](mSwitches* testInstance){ testInstance->SwitchProbe();}, this);
    #endif
  }
}


void mSwitches::SwitchPullupFlag(uint32_t switch_bit) {
  bitSet(Switch.no_pullup_bitmask, switch_bit);
}


void mSwitches::SwitchPulldownFlag(uint32_t switch_bit) {
  bitSet(Switch.pulldown_bitmask, switch_bit);
}


void mSwitches::SwitchSetVirtualPinState(uint32_t index, uint32_t state) {
  // Set virtual pin state to be debounced as used by early detected switches
  bitWrite(Switch.virtual_pin_bitmask, index, state);
}


uint8_t mSwitches::SwitchLastState(uint32_t index) {
  // Get last state
  return Switch.last_state[index];
}


uint8_t mSwitches::SwitchGetState(uint32_t index) {
  // Get current state
  return Switch.debounced_state[index];
}


void mSwitches::SwitchSetState(uint32_t index, uint32_t state) {
  // Set debounced pin state to be used by late detected switches
  if (!bitRead(Switch.used_bitmask, index)) {
    for (uint32_t i = 0; i <= index; i++) {
      if (!bitRead(Switch.used_bitmask, i)) {
        bitSet(Switch.used_bitmask, i);
        AddLog(LOG_LEVEL_DEBUG, PSTR("SWT: Add vSwitch%d, State %d"), i +1, Switch.debounced_state[i]);
      }
    }
  }
  Switch.debounced_state[index] = state;
}


bool mSwitches::SwitchUsed(uint32_t index) {
  return bitRead(Switch.used_bitmask, index);
}


void mSwitches::SetSwitchUsed(uint32_t index) {
  bitSet(Switch.used_bitmask, index);
}


bool mSwitches::SwitchState(uint32_t index) {
  uint32_t switchmode = pCONT_set->Settings.switchmode[index];
  return ((FOLLOW_INV == switchmode) ||
          (PUSHBUTTON_INV == switchmode) ||
          (PUSHBUTTONHOLD_INV == switchmode) ||
          (FOLLOWMULTI_INV == switchmode) ||
          (PUSHHOLDMULTI_INV == switchmode) ||
          (PUSHON_INV == switchmode) ||
          (PUSH_IGNORE_INV == switchmode)
         ) ^ Switch.last_state[index];
}


void mSwitches::SwitchProbe(void) 
{
  
  if (Switch.probe_mutex || (pCONT_time->uptime_seconds_nonreset < 4)) { return; }  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit
  Switch.probe_mutex = true;

  uint32_t state_filter;
  uint32_t first_change = Switch.first_change;
  uint32_t debounce_flags = pCONT_set->Settings.switch_debounce % 10;
  bool force_high = (debounce_flags &1);                     // 51, 101, 151 etc
  bool force_low = (debounce_flags &2);                      // 52, 102, 152 etc
  bool ac_detect = (debounce_flags == 9);

  if (ac_detect) {
    if (pCONT_set->Settings.switch_debounce < 2 * SWITCH_AC_PERIOD * SWITCH_FAST_PROBE_INTERVAL + 9) {
      state_filter = 2 * SWITCH_AC_PERIOD;
    } else if (pCONT_set->Settings.switch_debounce > (0x7f - 2 * SWITCH_AC_PERIOD) * SWITCH_FAST_PROBE_INTERVAL) {
      state_filter = 0x7f;
    } else {
      state_filter = (pCONT_set->Settings.switch_debounce - 9) / SWITCH_FAST_PROBE_INTERVAL;
    }
  } else {
    state_filter = pCONT_set->Settings.switch_debounce / SWITCH_PROBE_INTERVAL;	// 5, 10, 15
  }


  uint32_t not_activated;
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (!bitRead(Switch.used_bitmask, i)) { continue; }

    if(pCONT_pins->PinUsed(GPIO_SWT1_ID, i))
    { 
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_SWT1_ID, i));
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_INV_ID, i)) // Inverted pin, active low, with pulls
    {    
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_SWT1_INV_ID, i));
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_ID, i)) // Standard pin, active high, NO pulls
    {
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_SWT1_ID, i));
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_INV_NP_ID, i))
    {    
      not_activated = digitalRead(pCONT_pins->GetPin(GPIO_SWT1_INV_NP_ID, i));
    } else {
      not_activated = bitRead(Switch.virtual_pin_bitmask, i);
    }

    // ALOG_INF(PSTR("not_activated[%d] %d"), i, not_activated);

    // Olimex user_switch2.c code to fix 50Hz induced pulses
    if (not_activated) {

      if (ac_detect) {                                     // Enabled with SwitchDebounce x9
        Switch.state[i] |= 0x80;
        if (Switch.state[i] > 0x80) {
          Switch.state[i]--;
          if (0x80 == Switch.state[i]) {
            Switch.debounced_state[i] = 0;
            Switch.first_change = false;
          }
        }
      } else {

        if (force_high) {                                  // Enabled with SwitchDebounce x1
          if (1 == Switch.debounced_state[i]) {
            Switch.state[i] = state_filter;                // With noisy input keep current state 1 unless constant 0
          }
        }

        if (Switch.state[i] < state_filter) {
          Switch.state[i]++;
          if (state_filter == Switch.state[i]) {
            Switch.debounced_state[i] = 1;
          }
        }
      }
    } else {

      if (ac_detect) {                                     // Enabled with SwitchDebounce x9
        /*
          * Moes MS-104B and similar devices using an AC detection circuitry
          * on their switch inputs generating an ~4 ms long low pulse every
          * AC wave. We start the time measurement on the falling edge.
          *
          * state: bit7: previous state, bit6..0: counter
          */
        if (Switch.state[i] & 0x80) {
          Switch.state[i] &= 0x7f;
          if (Switch.state[i] < state_filter - 2 * SWITCH_AC_PERIOD) {
            Switch.state[i] += 2 * SWITCH_AC_PERIOD;
          } else {
            Switch.state[i] = state_filter;
            Switch.debounced_state[i] = 1;
            if (first_change) {
              Switch.last_state[i] = 1;
              Switch.first_change = false;
            }
          }
        } else {
          if (Switch.state[i] > 0x00) {
            Switch.state[i]--;
            if (0x00 == Switch.state[i]) {
              Switch.debounced_state[i] = 0;
              Switch.first_change = false;
            }
          }
        }
      } else {

        if (force_low) {                                   // Enabled with SwitchDebounce x2
          if (0 == Switch.debounced_state[i]) {
            Switch.state[i] = 0;                           // With noisy input keep current state 0 unless constant 1
          }
        }

        if (Switch.state[i] > 0) {
          Switch.state[i]--;
          if (0 == Switch.state[i]) {
            Switch.debounced_state[i] = 0;
          }
        }
      }
    }
  }

  Switch.probe_mutex = false;
}


void mSwitches::SwitchHandler(void) {
  if (pCONT_time->uptime_seconds_nonreset < 4) { return; }                  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint32_t loops_per_second = 1000 / pCONT_set->Settings.switch_debounce;

  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (!bitRead(Switch.used_bitmask, i)) { continue; }

    uint32_t button = Switch.debounced_state[i];
    uint32_t switchflag = POWER_TOGGLE +1;
    uint32_t mqtt_action = POWER_NONE;
    uint32_t switchmode = pCONT_set->Settings.switchmode[i];

    if (Switch.hold_timer[i] & (((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)) ? SM_TIMER_MASK: SM_NO_TIMER_MASK)) {
      Switch.hold_timer[i]--;
      if ((Switch.hold_timer[i] & SM_TIMER_MASK) == loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 25) {
        if ((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)){
          if (((switchmode == PUSHHOLDMULTI) & (NOT_PRESSED == Switch.last_state[i])) | ((switchmode == PUSHHOLDMULTI_INV) & (PRESSED == Switch.last_state[i]))) {
            SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);     // Execute command via MQTT
          }
          else if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) {
            SendKey(KEY_SWITCH, i +1, POWER_DELAYED);  // Execute command via MQTT
            mqtt_action = POWER_DELAYED;
            Switch.hold_timer[i] = 0;
          }
        }
      }
      if (0 == (Switch.hold_timer[i] & (((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)) ? SM_TIMER_MASK: SM_NO_TIMER_MASK))) {
        switch (switchmode) {
          case TOGGLEMULTI:
            switchflag = POWER_TOGGLE;                     // Toggle after hold
            break;
          case FOLLOWMULTI:
            switchflag = button &1;                        // Follow wall switch state after hold
            break;
          case FOLLOWMULTI_INV:
            switchflag = ~button &1;                       // Follow inverted wall switch state after hold
            break;
          case PUSHHOLDMULTI:
            if (NOT_PRESSED == button) {
              Switch.hold_timer[i] = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 25;
              SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);  // Execute command via MQTT
              mqtt_action = POWER_INCREMENT;
            } else {
              Switch.hold_timer[i]= 0;
              SendKey(KEY_SWITCH, i +1, POWER_CLEAR);      // Execute command via MQTT
              mqtt_action = POWER_CLEAR;
            }
            break;
          case PUSHHOLDMULTI_INV:
            if (PRESSED == button) {
              Switch.hold_timer[i] = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 25;
              SendKey(KEY_SWITCH, i +1, POWER_INCREMENT);  // Execute command via MQTT
              mqtt_action = POWER_INCREMENT;
            } else {
              Switch.hold_timer[i]= 0;
              SendKey(KEY_SWITCH, i +1, POWER_CLEAR);      // Execute command via MQTT
              mqtt_action = POWER_CLEAR;
            }
            break;
        default:
          SendKey(KEY_SWITCH, i +1, POWER_HOLD);           // Execute command via MQTT
          mqtt_action = POWER_HOLD;
          break;
        }
      }
    }

    if (button != Switch.last_state[i]) {  // This implies if ((PRESSED == button) then (NOT_PRESSED == Switch.last_state[i]))
      switch (switchmode) {
      case TOGGLE:                         // SwitchMode 0
      case PUSHBUTTON_TOGGLE:              // SwitchMode 7
        switchflag = POWER_TOGGLE;         // Toggle
        break;
      case FOLLOW:                         // SwitchMode 1
        switchflag = button &1;            // Follow wall switch state
        break;
      case FOLLOW_INV:                     // SwitchMode 2
        switchflag = ~button &1;           // Follow inverted wall switch state
        break;
      case PUSHBUTTON:                     // SwitchMode 3
        if (PRESSED == button) {
          switchflag = POWER_TOGGLE;       // Toggle with pushbutton to Gnd
        }
        break;
      case PUSHBUTTON_INV:                 // SwitchMode 4
        if (NOT_PRESSED == button) {
          switchflag = POWER_TOGGLE;       // Toggle with releasing pushbutton from Gnd
        }
        break;
      case PUSHBUTTONHOLD:                 // SwitchMode 5
        if (PRESSED == button) {
          Switch.hold_timer[i] = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;  // Start timer on button press
        }
        if ((NOT_PRESSED == button) && (Switch.hold_timer[i])) {
          Switch.hold_timer[i] = 0;        // Button released and hold timer not expired : stop timer...
          switchflag = POWER_TOGGLE;       // ...and Toggle
        }
        break;
      case PUSHBUTTONHOLD_INV:             // SwitchMode 6
        if (NOT_PRESSED == button) {
          Switch.hold_timer[i] = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;  // Start timer on button press...
        }
        if ((PRESSED == button) && (Switch.hold_timer[i])) {
          Switch.hold_timer[i] = 0;        // Button released and hold timer not expired : stop timer.
          switchflag = POWER_TOGGLE;       // ...and Toggle
        }
        break;
      case TOGGLEMULTI:                    // SwitchMode 8
      case FOLLOWMULTI:                    // SwitchMode 9
      case FOLLOWMULTI_INV:                // SwitchMode 10
        if (Switch.hold_timer[i]) {
          Switch.hold_timer[i] = 0;
          SendKey(KEY_SWITCH, i +1, POWER_HOLD);              // Execute command via MQTT
          mqtt_action = POWER_HOLD;
        } else {
          Switch.hold_timer[i] = loops_per_second / 2;        // 0.5 second multi press window
        }
        break;
      case PUSHHOLDMULTI:                  // SwitchMode 11
        if (NOT_PRESSED == button) {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) != 0) {
            Switch.hold_timer[i] = ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) ? SM_SECOND_PRESS : 0;
            SendKey(KEY_SWITCH, i +1, POWER_INV);             // Execute command via MQTT
            mqtt_action = POWER_INV;
          }
        } else {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) > loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 25) {
            if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) != SM_SECOND_PRESS) {
              Switch.hold_timer[i]= SM_FIRST_PRESS;
              switchflag = POWER_TOGGLE;                      // Toggle with pushbutton
            }
            else{
              SendKey(KEY_SWITCH, i +1, POWER_100);           // Execute command via MQTT
              mqtt_action = POWER_100;
              Switch.hold_timer[i]= 0;
            }
          } else {
            Switch.hold_timer[i]= 0;
            SendKey(KEY_SWITCH, i +1, POWER_RELEASE);         // Execute command via MQTT
            mqtt_action = POWER_RELEASE;
          }
        }
        Switch.hold_timer[i] = (Switch.hold_timer[i] & ~SM_TIMER_MASK) | loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;
        break;
      case PUSHHOLDMULTI_INV:              // SwitchMode 12
        if (PRESSED == button) {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) != 0) {
            Switch.hold_timer[i] = ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) ? SM_SECOND_PRESS : 0;
            SendKey(KEY_SWITCH, i +1, POWER_INV);             // Execute command via MQTT
            mqtt_action = POWER_INV;
          }
        } else {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK)> loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 25) {
            if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) != SM_SECOND_PRESS) {
              Switch.hold_timer[i]= SM_FIRST_PRESS;
              switchflag = POWER_TOGGLE;                      // Toggle with pushbutton
            }
            else{
              SendKey(KEY_SWITCH, i +1, POWER_100);           // Execute command via MQTT
              mqtt_action = POWER_100;
              Switch.hold_timer[i]= 0;
            }
          } else {
            Switch.hold_timer[i]= 0;
            SendKey(KEY_SWITCH, i +1, POWER_RELEASE);         // Execute command via MQTT
            mqtt_action = POWER_RELEASE;
          }
        }
        Switch.hold_timer[i] = (Switch.hold_timer[i] & ~SM_TIMER_MASK) | loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;
        break;
      case PUSHON:                         // SwitchMode 13
        if (PRESSED == button) {
          switchflag = POWER_ON;                              // Power ON with pushbutton to Gnd
        }
        break;
      case PUSHON_INV:                     // SwitchMode 14
        if (NOT_PRESSED == button) {
          switchflag = POWER_ON;                              // Power ON with releasing pushbutton from Gnd
        }
        break;
      case PUSH_IGNORE:                    // SwitchMode 15
      case PUSH_IGNORE_INV:                // SwitchMode 16
        Switch.last_state[i] = button;                        // Update switch state before publishing
        mqtthandler_sensor_ifchanged.flags.SendNow = true;
        ALOG_INF(PSTR("MqttPublishSensor();"));
        break;
      }
      Switch.last_state[i] = button;
    }


    // if (switchflag <= POWER_TOGGLE) { // ie where <3 was before
    //   if (!pCONT_set->Settings.flag_system.mqtt_switches) {                   // SetOption114 (0) - Detach Switches from relays and enable MQTT action state for all the SwitchModes
    //     // if (!SendKey(KEY_SWITCH, i +1, switchflag)) {         // Execute command via MQTT
    //     //   pCONT_mry->ExecuteCommandPower(i +1, switchflag, SRC_SWITCH);  // Execute command internally (if i < TasmotaGlobal.devices_present)
    //     //   // Should the rules of mine be here?
    //     // }                 // SetOption114 (0) - Detach Switches from relays and enable MQTT action state for all the SwitchModes
    //     // if (!SendKey(KEY_SWITCH, i +1, switchflag)) {         // Execute command via MQTT

    //     ALOG_INF(PSTR("WHERE MY RULE SHOULD BE"));
          
    //       // #ifdef USE_MODULE_DRIVERS_RELAY    I DO NOT WANT INTERNAL RELAY CONTROL, BUT SHOULD BE RULE BASED!
    //       // pCONT_mry->ExecuteCommandPower(i, switchflag, SRC_SWITCH);  // Execute command internally (if i < TasmotaGlobal.devices_present)
    //       // #endif
    //       // Should the rules of mine be here?
    //     // }
    //   } else { mqtt_action = switchflag; }
    // }



    if (switchflag <= POWER_TOGGLE)
    {
      #ifndef ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG

        #ifdef USE_MODULE_CORE_RULES
          // Active high means start of motion always, so check for inversion
          uint8_t new_state = (button);  // ? /*invert*/ !state : /*else, just follow*/ state;

          ALOG_INF( PSTR("switchflag=%d, state=%d"), switchflag, new_state);

          pCONT_rules->NewEventRun_NumArg(
            D_UNIQUE_MODULE_SENSORS_SWITCHES_ID, // Unique module ID
            TASK_EVENT_INPUT_STATE_CHANGED_ID,   // FUNC ID
            i, // SWitch index
            1, // Embedded data length
            new_state); // Event has occured, save and check it            
            
        #endif
        
      #endif // ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG
    }







    // if ((mqtt_action != POWER_NONE) && pCONT_set->Settings.flag_system.mqtt_switches) {  // SetOption114 (0) - Detach Switches from relays and enable MQTT action state for all the SwitchModes
    //   // if (!pCONT_set->Settings.flag.hass_discovery) {                   // SetOption19 - Control Home Assistant automatic discovery (See SetOption59)
    //     char mqtt_state_str[16];
    //     char *mqtt_state = mqtt_state_str;
    //     if (mqtt_action <= 3) {
    //       if (mqtt_action != 3) { SendKey(KEY_SWITCH, i +1, mqtt_action); }
    //       mqtt_state = pCONT_set->SettingsText(SET_STATE_TXT1 + mqtt_action);
    //     } else {
    //       pCONT_sup->GetTextIndexed(mqtt_state_str, sizeof(mqtt_state_str), mqtt_action, kSwitchPressStates);
    //     }
    //     pCONT_sup->Response_P(S_JSON_SVALUE_ACTION_SVALUE, pCONT_sup->GetSwitchText(i).c_str(), mqtt_state);
    //     char scommand[10];
    //     snprintf_P(scommand, sizeof(scommand), PSTR(D_SWITCH "%d"), i +1);
    //     ALOG_INF(PSTR("MqttPublishPrefixTopicRulesProcess_P %s %s"), RESULT_OR_STAT, scommand);
    //     // MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT, scommand);
    //   // }
    //   mqtt_action = POWER_NONE;
    //   ALOG_INF(PSTR("mqtt switch removed, should be to send the switch"));
    // }

    
    
  } // for switches
}

/**
 * @brief Lets keep this function, as it plays nice with the code above. 
 * My code will then convert it to the ways I want
 * ** mqtt 
 * ** internal rules
 * ** 
 * 
 * @param key 
 * @param device 
 * @param state 
 * @return true 
 * @return false 
 */
bool mSwitches::SendKey(uint32_t key, uint32_t device, uint32_t state)
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

ALOG_INF(PSTR("SendKey %d %d %d"), key, device, state);

// or rules here?

  // if (switchflag < 3) 
  // {
    #ifndef ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG

      #ifdef USE_MODULE_CORE_RULES
        // Active high means start of motion always, so check for inversion
        uint8_t new_state = state;//switches[i].active_state_value == LOW ? /*invert*/ !state : /*else, just follow*/ state;
        uint8_t i = device;
        DEBUG_LINE_HERE
        ALOG_INF( PSTR("switchflag=%d, new_state=%d, state=%d"),0,new_state,state);

        pCONT_rules->NewEventRun_NumArg(
          D_UNIQUE_MODULE_SENSORS_SWITCHES_ID, // Unique module ID
          TASK_EVENT_INPUT_STATE_CHANGED_ID,   // FUNC ID
          i, // SWitch index
          1, // Embedded data length
          new_state); // Event has occured, save and check it            
          DEBUG_LINE_HERE
      #endif
      
    #endif // ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG
  // }

  // switches[i].lastwallswitch = state;
  // switches[i].state = state;


}




void mSwitches::SwitchLoop(void) {
  if (Switch.used_bitmask) {
    if(mTime::TimeReached(&Switch.debounce, pCONT_set->Settings.switch_debounce)){
      SwitchHandler();
    }
  }
}

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, GetSensorCount());
  return JBI->End();

}
uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, 0);
  return JBI->End();

}
#endif // ENABLE_DEVFEATURE_SWITCHES__V2


#ifndef ENABLE_DEVFEATURE_SWITCHES__V2
void mSwitches::SwitchInit(void)
{

  ALOG_HGL( PSTR("D_LOG_STARTUP" "Switches Init") );

  // Init states
  for(uint8_t pin_id=0;pin_id<MAX_SWITCHES_SET;pin_id++){
    switches[pin_id].lastwallswitch = 1;  // Init global to virtual switch state;
    switches[pin_id].active_state_value = 1; // default is active high
    switches[pin_id].switch_virtual = switches[pin_id].lastwallswitch;
  }

  // Check all possible pin options
  settings.switches_found = 0;    
  for(uint8_t pin_id=GPIO_SWT1_ID;pin_id<GPIO_SWT1_ID+(MAX_SWITCHES_SET*4);pin_id++){

        Serial.printf("pin=%d/%d\n\r",pin_id,GPIO_SWT1_ID+(MAX_SWITCHES_SET*4));
    if(pCONT_pins->PinUsed(pin_id)){
        Serial.printf("PinUsed\t\tpin=%d\n\r",pin_id);
      
      switches[settings.switches_found].pin = pCONT_pins->GetPin(pin_id);

      // Standard pin, active high, with pulls 
      if(
        (pin_id >= GPIO_SWT1_ID)&&
        (pin_id < GPIO_SWT1_ID+MAX_SWITCHES_SET)
      ){
        pinMode(switches[settings.switches_found].pin, INPUT_PULLUP);
        switches[settings.switches_found].active_state_value = HIGH;
      }else
      // Inverted pin, active low, with pulls
      if(
        (pin_id >= GPIO_SWT1_INV_ID)&&
        (pin_id < GPIO_SWT1_INV_ID+MAX_SWITCHES_SET)
      ){
        pinMode(switches[settings.switches_found].pin, INPUT_PULLUP);
        switches[settings.switches_found].active_state_value = LOW;
      }else
      // Standard pin, active high, NO pulls
      if(
        (pin_id >= GPIO_SWT1_NP_ID)&&
        (pin_id < GPIO_SWT1_NP_ID+MAX_SWITCHES_SET)
      ){
        Serial.printf("GPIO_SWT1_NP_ID pin=%d\n\r\n\r\n\r\n\r\n\r\n\r",pin_id);
        pinMode(switches[settings.switches_found].pin, INPUT);
        switches[settings.switches_found].active_state_value = HIGH;
      }else
      // 
      if(
        (pin_id >= GPIO_SWT1_INV_NP_ID)&&
        (pin_id < GPIO_SWT1_INV_NP_ID+MAX_SWITCHES_SET)
      ){
        pinMode(switches[settings.switches_found].pin, INPUT);
        switches[settings.switches_found].active_state_value = LOW;
      }else{
        Serial.printf("NO MATCH GPIO_SWT1_NP_ID pin=%d\n\r\n\r\n\r\n\r\n\r\n\r",pin_id);

      }

      // Set global now so doesn't change the saved power state on first switch check
      switches[settings.switches_found].lastwallswitch = digitalRead(switches[settings.switches_found].pin);  
      switches[settings.switches_found].switch_virtual = digitalRead(switches[settings.switches_found].pin);  
      
      #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_TST(PSTR("Switch %d %d %d"), pin_id, settings.switches_found, switches[settings.switches_found].pin);
      #endif // ENABLE_LOG_LEVEL_INFO
      
      if(settings.switches_found++ >= MAX_SWITCHES_SET){ break; }

    } // if PinUsed

  }

  if(TickerSwitch == nullptr){
    TickerSwitch = new Ticker();
  }

  if (settings.switches_found) { 

    // #ifdef ESP288
    // TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
    //   [this](void){
    //     this->SwitchProbe();
    //   }
    // );

    // #else // esp32
    // TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
    //   [this](void){
    //     this->SwitchProbe();
    //   }
    // );

    TickerSwitch->attach_ms(
      SWITCH_PROBE_INTERVAL, 


      +[](mSwitches* testInstance){ testInstance->SwitchProbe();}, this); //hacky solution to be fixed

    // #endif
  }

}




// /*********************************************************************************************/

void mSwitches::SwitchProbe(void)
{
  
  if (pCONT_time->uptime_seconds_nonreset < 4) { return; }                           // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint8_t state_filter = pCONT_set->Settings.switch_debounce / SWITCH_PROBE_INTERVAL;   // 5, 10, 15
  uint8_t force_high = (pCONT_set->Settings.switch_debounce % 50) &1;                   // 51, 101, 151 etc
  uint8_t force_low = (pCONT_set->Settings.switch_debounce % 50) &2;                    // 52, 102, 152 etc

  for (uint8_t i = 0; i < MAX_SWITCHES_SET; i++) {

    // if (pCONT_pins->PinUsed(GPIO_SWT1_ID,i)) {      
    if(switches[i].pin != -1){
      
      // Olimex user_switch2.c code to fix 50Hz induced pulses
      if (1 == digitalRead(switches[i].pin)) {

        if (force_high) {                               // Enabled with SwitchDebounce x1
          if (1 == switches[i].switch_virtual) {
            switches[i].switch_state_buf = state_filter;         // With noisy input keep current state 1 unless constant 0
          }
        }

        if (switches[i].switch_state_buf < state_filter) {
          switches[i].switch_state_buf++;
          if (state_filter == switches[i].switch_state_buf) {
            switches[i].switch_virtual = 1;
          }
        }

      } else {

        if (force_low) {                                // Enabled with SwitchDebounce x2
          if (0 == switches[i].switch_virtual) {
            switches[i].switch_state_buf = 0;                    // With noisy input keep current state 0 unless constant 1
          }
        }

        if (switches[i].switch_state_buf > 0) {
          switches[i].switch_state_buf--;
          if (0 == switches[i].switch_state_buf) {
            switches[i].switch_virtual = 0;
          }
        }
        
      } // END if digitalRead

    } // if PinUsed
  } // for all switches

  // TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
  //   [this](void){
  //     this->SwitchProbe();
  //   }
  // );
    #ifdef ESP288
    TickerSwitch->attach_ms(SWITCH_PROBE_INTERVAL, 
      [this](void){
        this->SwitchProbe();
      }
    );

    #else // esp32

    TickerSwitch->attach_ms(
      SWITCH_PROBE_INTERVAL, 


      +[](mSwitches* testInstance){ testInstance->SwitchProbe();}, this); //hacky solution to be fixed


    //   +[this](void){
    //     this->SwitchProbe();
    //   }
    // );

    #endif

}



/**
 * @brief 
 * Actually not right for rules, I should probably make "SwitchMode_GetID_by_Name" and "GetStateNumber" together
 * 
 * @param c 
 * @return int16_t 
 */
int16_t mSettings::SwitchMode_GetID_by_Name(const char* c)
// D_DATE_TIME_SEPARATOR
// RuleCommand? I need to be able to react to trigger, or simply directly set, so needs both switchmode and getstate range, create new LIST
{
  if(*c=='\0'){    return -1; }
  if(strcasecmp_P(c,PM_SWITCHMODE_TOGGLE_CTR)==0){ return SWITCHMODE_TOGGLE_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_FOLLOW_CTR)==0){ return SWITCHMODE_FOLLOW_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_FOLLOW_INV_CTR)==0){ return SWITCHMODE_FOLLOW_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_CTR)==0){ return SWITCHMODE_PUSHBUTTON_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_INV_CTR)==0){ return SWITCHMODE_PUSHBUTTON_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTONHOLD_CTR)==0){ return SWITCHMODE_PUSHBUTTONHOLD_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR)==0){ return SWITCHMODE_PUSHBUTTONHOLD_INV_ID; }
  if(strcasecmp_P(c,PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR)==0){ return SWITCHMODE_PUSHBUTTON_TOGGLE_ID; }
  return -1;
}


const char* mSettings::SwitchMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen){
  switch(id){
    default:
    case SWITCHMODE_TOGGLE_ID:                memcpy_P(buffer, PM_SWITCHMODE_TOGGLE_CTR, sizeof(PM_SWITCHMODE_TOGGLE_CTR)); break;
    case SWITCHMODE_FOLLOW_ID:                memcpy_P(buffer, PM_SWITCHMODE_FOLLOW_CTR, sizeof(PM_SWITCHMODE_FOLLOW_CTR)); break; 
    case SWITCHMODE_FOLLOW_INV_ID:            memcpy_P(buffer, PM_SWITCHMODE_FOLLOW_INV_CTR, sizeof(PM_SWITCHMODE_FOLLOW_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_ID:            memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_INV_ID:        memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_INV_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTONHOLD_ID:        memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTONHOLD_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTONHOLD_CTR)); break; 
    case SWITCHMODE_PUSHBUTTONHOLD_INV_ID:    memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTONHOLD_INV_CTR)); break; 
    case SWITCHMODE_PUSHBUTTON_TOGGLE_ID:     memcpy_P(buffer, PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR, sizeof(PM_SWITCHMODE_PUSHBUTTON_TOGGLE_CTR)); break; 
  }
  return buffer;
}


/*********************************************************************************************\
 * Switch handler
\*********************************************************************************************/

void mSwitches::SwitchHandler(uint8_t mode) //mode needs removed
{
  if (pCONT_time->uptime_seconds_nonreset < 4) { return; }  
DEBUG_LINE_HERE

  uint8_t state = SWITCH_NOT_PRESSED_ID;
  uint8_t switchflag;
  // DEBUG_LINE_HERE
  // Serial.printf("Settings.switch_debounce %d\n\r", pCONT_set->Settings.switch_debounce);
  // Serial.flush();
  uint16_t loops_per_second = safeDivideWithDefault(1000, pCONT_set->Settings.switch_debounce, 20);
  // Serial.printf("loops_per_second %d\n\r", loops_per_second);
  // DEBUG_LINE_HERE
  uint8_t active_state = LOW;

  for (uint8_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (
      (switches[i].pin != -1) // pCONT_pins->PinUsed(GPIO_SWT1_ID,i)
      || (mode)
    ){

      if (switches[i].holdwallswitch) 
      {
        switches[i].holdwallswitch--;
        if (0 == switches[i].holdwallswitch) 
        {
         // SendKey(1, i +1, 3);           // Execute command via MQTT
        }
      }

      state = switches[i].switch_virtual;

      // enum SwitchModeOptions {TOGGLE, FOLLOW, FOLLOW_INV, PUSHBUTTON, PUSHBUTTON_INV, PUSHBUTTONHOLD, PUSHBUTTONHOLD_INV, PUSHBUTTON_TOGGLE, MAX_SWITCH_OPTION};

      // device  = Relay number 1 and up
      // state 0 = POWER_OFF = Relay Off
      // state 1 = POWER_ON = Relay On (turn off after Settings.pulse_timer * 100 mSec if enabled)
      // state 2 = POWER_TOGGLE = Toggle relay
      // state 3 = POWER_BLINK = Blink relay
      // state 4 = POWER_BLINK_STOP = Stop blinking relay
      // state 8 = POWER_OFF_NO_STATE = Relay Off and no publishPowerState
      // state 9 = POWER_ON_NO_STATE = Relay On and no publishPowerState
      // state 10 = POWER_TOGGLE_NO_STATE = Toggle relay and no publishPowerState
      // state 16 = POWER_SHOW_STATE = Show power state

      if (state != switches[i].lastwallswitch) 
      {
        switches[i].ischanged = true;

        ALOG_INF( PSTR(D_LOG_SWITCHES "#%d Changed : Level %d | %s"), 
                  i, 
                  state,
                  state==active_state?"ACTIVE":"Not Active"
        );
      
        ALOG_INF( PSTR("state%d != lastwallswitch[%d]%d\n\r\n\r\n\r"),state,i,switches[i].lastwallswitch);
        
        switchflag = 3;
        switch (pCONT_set->Settings.switchmode[i]) {
        case SWITCHMODE_TOGGLE_ID:
          switchflag = POWER_TOGGLE;    // Toggle
          break;
        case SWITCHMODE_FOLLOW_ID:
          switchflag = state &1;        // Follow wall switch state
          break;
        case SWITCHMODE_FOLLOW_INV_ID:
          switchflag = ~state &1;       // Follow inverted wall switch state
          break;
        case SWITCHMODE_PUSHBUTTON_ID:
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case SWITCHMODE_PUSHBUTTON_INV_ID:
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch)) {
            switchflag = 2;              // Toggle with releasing pushbutton from Gnd
          }
          break;
        case SWITCHMODE_PUSHBUTTON_TOGGLE_ID:
          if (state != switches[i].lastwallswitch) {
            switchflag = 2;              // Toggle with any pushbutton change
          }
          break;
        case SWITCHMODE_PUSHBUTTONHOLD_ID:
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch)) {
            switches[i].holdwallswitch = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;
          }
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch) && (switches[i].holdwallswitch)) {
            switches[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        case SWITCHMODE_PUSHBUTTONHOLD_INV_ID:
          if ((SWITCH_NOT_PRESSED_ID == state) && (SWITCH_PRESSED_ID == switches[i].lastwallswitch)) {
            switches[i].holdwallswitch = loops_per_second * pCONT_set->Settings.setoption_255[P_HOLD_TIME] / 10;
          }
          if ((SWITCH_PRESSED_ID == state) && (SWITCH_NOT_PRESSED_ID == switches[i].lastwallswitch) && (switches[i].holdwallswitch)) {
            switches[i].holdwallswitch = 0;
            switchflag = 2;              // Toggle with pushbutton to Gnd
          }
          break;
        }

        mqtthandler_sensor_ifchanged.flags.SendNow = true;
        mqtthandler_sensor_teleperiod.flags.SendNow = true;

        if (switchflag < 3) 
        {
          #ifndef ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG

            #ifdef USE_MODULE_CORE_RULES
              // Active high means start of motion always, so check for inversion
              uint8_t new_state = switches[i].active_state_value == LOW ? /*invert*/ !state : /*else, just follow*/ state;
              DEBUG_LINE_HERE
              ALOG_INF( PSTR("switchflag=%d, new_state=%d, state=%d"),switchflag,new_state,state);
    
              pCONT_rules->NewEventRun_NumArg(
                D_UNIQUE_MODULE_SENSORS_SWITCHES_ID, // Unique module ID
                TASK_EVENT_INPUT_STATE_CHANGED_ID,   // FUNC ID
                i, // SWitch index
                1, // Embedded data length
                new_state); // Event has occured, save and check it            
                DEBUG_LINE_HERE
            #endif
            
          #endif // ENABLE_DEVFEATURE_TO_PARTIAL_DISABLE_SWITCH_FOR_DEBUG
        }

        switches[i].lastwallswitch = state;
        switches[i].state = state;

      }
    }
    else
    {
      switches[i].ischanged = false;
    }
  }
}

void mSwitches::SwitchLoop(void)
{
  if (settings.switches_found) {
    if(mTime::TimeReached(&switch_debounce, pCONT_set->Settings.switch_debounce)){
      SwitchHandler(0);
    }
  }
}



bool mSwitches::IsSwitchActive(uint8_t id){

  // #ifdef ENABLE_DEVFEATURE_ISSWITCHACTIVE_CHANGE

  if( // Is Active?
    switches[id].switch_virtual ==     // internal switch state
    switches[id].active_state_value    // logical value when the switch is considered on, ie, active low 0==0, active high 1==1 
  ){
    return true;
  }
  return false;

  // #else // ENABLE_DEVFEATURE_ISSWITCHACTIVE_CHANGE // PHASE OUT IF NOT NEEDED
  // // Needs to know what type the button is, low, high, no pullup etc
  // if(switches[id].lastwallswitch){
  //   return switches[id].active_state_value ? false : true;
  // }
  // return switches[id].active_state_value ? true : false;  
  // #endif // ENABLE_DEVFEATURE_ISSWITCHACTIVE_CHANGE

}


/********************************************************************************************/

void mSwitches::SetPullupFlag(uint16_t switch_bit)
{
  bitSet(switch_no_pullup, switch_bit);
}

uint8_t mSwitches::GetLastState(uint8_t index)
{
  return switches[index].lastwallswitch;
}

void mSwitches::SetVirtual(uint8_t index, uint8_t state)
{
  switches[index].switch_virtual = state;
}

uint8_t mSwitches::GetVirtual(uint8_t index)
{
  return switches[index].switch_virtual;
}



/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, settings.switches_found);

    // JBI->Add("pin0", switches[0].pin);
    // JBI->Add("pin1", switches[1].pin);
    // JBI->Add("pin2", switches[2].pin);
    // JBI->Add("read0", digitalRead(switches[0].pin));
    // JBI->Add("read1", digitalRead(switches[1].pin));
    // JBI->Add("read2", digitalRead(switches[2].pin));
    
  return JBI->End();

}

uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[50]; 
  
  JBI->Start();

  for(uint8_t sensor_id=0;sensor_id<settings.switches_found;sensor_id++){
    if(switches[sensor_id].ischanged || (json_level>JSON_LEVEL_IFCHANGED) ){ 
      
      JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));
        JBI->Add(D_STATE, IsSwitchActive(sensor_id));
        JBI->Add(D_STATE "_ctr", IsSwitchActive(sensor_id)?"On":"Off");


        JBI->Add("digitalRead", digitalRead(switches[sensor_id].pin));

        


        JBI->Add("ischanged", switches[sensor_id].ischanged);
        JBI->Add("lastwallswitch", switches[sensor_id].lastwallswitch);
        JBI->Add("holdwallswitch", switches[sensor_id].holdwallswitch);
        JBI->Add("switch_state_buf", switches[sensor_id].switch_state_buf);
        JBI->Add("switch_virtual", switches[sensor_id].switch_virtual);
        // JBI->Add("is_linked_to_internal_relay", switches[sensor_id].is_linked_to_internal_relay);
        JBI->Add("linked_internal_relay_id", switches[sensor_id].linked_internal_relay_id);
      JBI->Object_End();
      
    }

  }

  return JBI->End();

}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

#endif // ! ENABLE_DEVFEATURE_SWITCHES__V2
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSwitches::MQTTHandler_Init(){

  struct handler<mSwitches>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT



#endif