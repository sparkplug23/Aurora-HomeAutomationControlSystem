
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

}


void mSwitches::Pre_Init(void) 
{

  ALOG_HGL( PSTR("D_LOG_STARTUP" "Switches Init") );

  // Clear the bitmask so all 32 bits are 0
  Switch.used_bitmap = 0;

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
      PullupFlag(i);
    }else    
    if(pCONT_pins->PinUsed(GPIO_SWT1_INV_NP_ID, i))
    {    
      SetSwitchUsed(i);
      pin = pCONT_pins->GetPin(GPIO_SWT1_INV_NP_ID, i);
      pinMode(pin, INPUT);
      PulldownFlag(i);
    }else{
      ALOG_DBG(PSTR(D_LOG_SWITCHES "%d None"), i);
    }

    if(pin != -1)
    {
      ALOG_INF(PSTR(D_LOG_SWITCHES "%d pin=%d %s"), i, pin, toBinaryString(Switch.used_bitmap, MAX_SWITCHES_SET).c_str() );
      Switch.last_state[i] = digitalRead(pin);
    }

  }

}


void mSwitches::Init(void) {

  bool ac_detect = (tkr_set->Settings.switch_debounce % 10 == 9);

  // bitmask has been set during PreInit
  
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) 
  {
    if(bitRead(Switch.used_bitmap, i))
    {
      if (ac_detect) {
        Switch.state[i] = 0x80 + 2 * SWITCH_AC_PERIOD;
        Switch.last_state[i] = 0;				 // Will set later in the debouncing code
      }
    }else{
      // Optional: Later add call to other modules to add virtual switches
    }
    Switch.debounced_state[i] = Switch.last_state[i];
  }

  
  if (Switch.used_bitmap)  // Any bit set
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
      +[](mSwitches* testInstance){ testInstance->Probe();}, this);
    #endif
  }
}

/********************************************************************************************/

void mSwitches::PullupFlag(uint32_t switch_bit) {
  bitSet(Switch.no_pullup_bitmap, switch_bit);
}


void mSwitches::PulldownFlag(uint32_t switch_bit) {
  bitSet(Switch.pulldown_bitmap, switch_bit);
}

/*------------------------------------------------------------------------------------------*/

void mSwitches::SetVirtualPinState(uint32_t index, uint32_t state) {
  // Set virtual pin state to be debounced as used by early detected switches
  bitWrite(Switch.virtual_pin_bitmap, index, state);
}


uint8_t mSwitches::LastState(uint32_t index) {
  // Get last state
  return Switch.last_state[index];
}


uint8_t mSwitches::GetState(uint32_t index) {
  // Get current state
  return Switch.debounced_state[index];
}


void mSwitches::SetState(uint32_t index, uint32_t state) {
  // Set debounced pin state to be used by late detected switches
  if (!bitRead(Switch.used_bitmap, index)) {
    for (uint32_t i = 0; i <= index; i++) {
      if (!bitRead(Switch.used_bitmap, i)) {
        bitSet(Switch.used_bitmap, i);
        AddLog(LOG_LEVEL_DEBUG, PSTR("SWT: Add vSwitch%d, State %d"), i +1, Switch.debounced_state[i]);
      }
    }
  }
  Switch.debounced_state[index] = state;
}

/*------------------------------------------------------------------------------------------*/

bool mSwitches::Used(uint32_t index) {
  return bitRead(Switch.used_bitmap, index);
}


void mSwitches::SetSwitchUsed(uint32_t index) {
  bitSet(Switch.used_bitmap, index);
}


bool mSwitches::State(uint32_t index) {
  uint32_t switchmode = tkr_set->Settings.switchmode[index];
  return ((FOLLOW_INV == switchmode) ||
          (PUSHBUTTON_INV == switchmode) ||
          (PUSHBUTTONHOLD_INV == switchmode) ||
          (FOLLOWMULTI_INV == switchmode) ||
          (PUSHHOLDMULTI_INV == switchmode) ||
          (PUSHON_INV == switchmode) ||
          (PUSH_IGNORE_INV == switchmode)
         ) ^ Switch.last_state[index];
}

/*********************************************************************************************/

void mSwitches::Probe(void) 
{
  
  if (Switch.probe_mutex || (tkr_time->uptime_seconds_nonreset < 4)) { return; }  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit
  Switch.probe_mutex = true;

  uint32_t state_filter;
  uint32_t first_change = Switch.first_change;
  uint32_t debounce_flags = tkr_set->Settings.switch_debounce % 10;
  bool force_high = (debounce_flags &1);                     // 51, 101, 151 etc
  bool force_low = (debounce_flags &2);                      // 52, 102, 152 etc
  bool ac_detect = (debounce_flags == 9);

  if (ac_detect) {
    if (tkr_set->Settings.switch_debounce < 2 * SWITCH_AC_PERIOD * SWITCH_FAST_PROBE_INTERVAL + 9) {
      state_filter = 2 * SWITCH_AC_PERIOD;
    } else if (tkr_set->Settings.switch_debounce > (0x7f - 2 * SWITCH_AC_PERIOD) * SWITCH_FAST_PROBE_INTERVAL) {
      state_filter = 0x7f;
    } else {
      state_filter = (tkr_set->Settings.switch_debounce - 9) / SWITCH_FAST_PROBE_INTERVAL;
    }
  } else {
    state_filter = tkr_set->Settings.switch_debounce / SWITCH_PROBE_INTERVAL;	// 5, 10, 15
  }


  uint32_t not_activated;
  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) {
    if (!bitRead(Switch.used_bitmap, i)) { continue; }

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
      not_activated = bitRead(Switch.virtual_pin_bitmap, i);
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


void mSwitches::Handler(void) {
  if (tkr_time->uptime_seconds_nonreset < 4) { return; }                  // Block GPIO for 4 seconds after poweron to workaround Wemos D1 / Obi RTS circuit

  uint32_t loops_per_second = 1000 / tkr_set->Settings.switch_debounce;

  for (uint32_t i = 0; i < MAX_SWITCHES_SET; i++) 
  {
    if (!bitRead(Switch.used_bitmap, i)) { continue; }

    uint32_t button = Switch.debounced_state[i];
    uint32_t switchflag = POWER_TOGGLE +1;
    uint32_t switchmode = tkr_set->Settings.switchmode[i];

    /***
     * Switch: Remains the same as previous loop (i.e. could be "held" timer)
     **/
    if (Switch.hold_timer[i] & (((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)) ? SM_TIMER_MASK: SM_NO_TIMER_MASK)) {
      Switch.hold_timer[i]--;
      if ((Switch.hold_timer[i] & SM_TIMER_MASK) == loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 25) {
        if ((switchmode == PUSHHOLDMULTI) | (switchmode == PUSHHOLDMULTI_INV)){
          if (((switchmode == PUSHHOLDMULTI) & (NOT_PRESSED == Switch.last_state[i])) | ((switchmode == PUSHHOLDMULTI_INV) & (PRESSED == Switch.last_state[i]))) {
            SendSwitch(i, POWER_INCREMENT);
          }
          else if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) {
            SendSwitch(i, POWER_DELAYED);
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
              Switch.hold_timer[i] = loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 25;
              SendSwitch(i, POWER_INCREMENT);
            } else {
              Switch.hold_timer[i]= 0;
              SendSwitch(i, POWER_CLEAR);      // Execute command via MQTT
            }
            break;
          case PUSHHOLDMULTI_INV:
            if (PRESSED == button) {
              Switch.hold_timer[i] = loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 25;
              SendSwitch(i, POWER_INCREMENT); // Execute command via MQTT
            } else {
              Switch.hold_timer[i]= 0;
              SendSwitch(i, POWER_CLEAR);      // Execute command via MQTT
            }
            break;
        default:
          SendSwitch(i, POWER_HOLD);           // Execute command via MQTT
          break;
        }
      }
    }

    /**
     * Switch: Has CHANGED since last probed
     */
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
          Switch.hold_timer[i] = loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10;  // Start timer on button press
        }
        if ((NOT_PRESSED == button) && (Switch.hold_timer[i])) {
          Switch.hold_timer[i] = 0;        // Button released and hold timer not expired : stop timer...
          switchflag = POWER_TOGGLE;       // ...and Toggle
        }
        break;
      case PUSHBUTTONHOLD_INV:             // SwitchMode 6
        if (NOT_PRESSED == button) {
          Switch.hold_timer[i] = loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10;  // Start timer on button press...
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
          SendSwitch(i, POWER_HOLD);              // Execute command via MQTT
        } else {
          Switch.hold_timer[i] = loops_per_second / 2;        // 0.5 second multi press window
        }
        break;
      case PUSHHOLDMULTI:                  // SwitchMode 11
        if (NOT_PRESSED == button) {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) != 0) {
            Switch.hold_timer[i] = ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) ? SM_SECOND_PRESS : 0;
            SendSwitch(i, POWER_INV);             // Execute command via MQTT
          }
        } else {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) > loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 25) {
            if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) != SM_SECOND_PRESS) {
              Switch.hold_timer[i]= SM_FIRST_PRESS;
              switchflag = POWER_TOGGLE;                      // Toggle with pushbutton
            }
            else{
              SendSwitch(i, POWER_100);           // Execute command via MQTT
              Switch.hold_timer[i]= 0;
            }
          } else {
            Switch.hold_timer[i]= 0;
            SendSwitch(i, POWER_RELEASE);         // Execute command via MQTT
          }
        }
        Switch.hold_timer[i] = (Switch.hold_timer[i] & ~SM_TIMER_MASK) | loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10;
        break;
      case PUSHHOLDMULTI_INV:              // SwitchMode 12
        if (PRESSED == button) {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK) != 0) {
            Switch.hold_timer[i] = ((Switch.hold_timer[i] & ~SM_TIMER_MASK) == SM_FIRST_PRESS) ? SM_SECOND_PRESS : 0;
            SendSwitch(i, POWER_INV);             // Execute command via MQTT
          }
        } else {
          if ((Switch.hold_timer[i] & SM_TIMER_MASK)> loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 25) {
            if ((Switch.hold_timer[i] & ~SM_TIMER_MASK) != SM_SECOND_PRESS) {
              Switch.hold_timer[i]= SM_FIRST_PRESS;
              switchflag = POWER_TOGGLE;                      // Toggle with pushbutton
            }
            else{
              SendSwitch(i, POWER_100);           // Execute command via MQTT
              Switch.hold_timer[i]= 0;
            }
          } else {
            Switch.hold_timer[i]= 0;
            SendSwitch(i, POWER_RELEASE);         // Execute command via MQTT
          }
        }
        Switch.hold_timer[i] = (Switch.hold_timer[i] & ~SM_TIMER_MASK) | loops_per_second * tkr_set->Settings.setoption_255[P_HOLD_TIME] / 10;
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
        Tasker(TASK_MQTT_SENDER); // MqttPublishSensor
        break;
      }
      Switch.last_state[i] = button;
    }

    /**
     * Trigger events not already triggered by SendSwitch above
     * Type: POWER_OFF, POWER_ON, POWER_TOGGLE
     */
    if (switchflag <= POWER_TOGGLE) { 
      SendSwitch(i, switchflag);
    }
    
  } // for switches
}


/**
 * New method for triggering system/mqtt/rules based on a switch being operated
 * 
 * (1) Regardless of internal controls, the input event should be broadcast on mqtt
 * (2) Internal controls should be linked via rules, with default hardware having a set of predefined rules
 * (3) Rule engine is called to report an event, that modules can read via standard TASK_EVENT_INPUT_STATE_CHANGED_ID or rely on binding rules
 * 
 * (A) internal mqtt commands are default, but later the sensor_interface should unify a reporting system
 */
bool mSwitches::SendSwitch(uint32_t index, uint32_t state)
{
  
  char switch_name[50];
  DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), index, switch_name, sizeof(switch_name));  
  char state_name_ctr[30]; char *name = state_name_ctr;    
  if (state <= 3) { // First 4 options are stored in SettingsText
    name = tkr_set->SettingsText(SET_STATE_TXT1 + state);
    Serial.println(name);
  } else {
    pCONT_sup->GetTextIndexed(state_name_ctr, sizeof(state_name_ctr), state, kSwitchPressStates);
  }
  ALOG_INF(PSTR(D_LOG_SWITCHES "SendSwitch[%d|%s] type[%d|%s]"), index, switch_name, state, name);

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

  event.id      = index;
  event.type    = state;
  event.waiting = true; // new event recorded

  #ifdef USE_MODULE_CORE_RULES
  tkr_rules->NewEventRun_NumArg(
    D_UNIQUE_MODULE_SENSORS_SWITCHES_ID, // Unique module ID
    TASK_EVENT_INPUT_STATE_CHANGED_ID,   // FUNC ID
    index, // SWitch index
    1, // Embedded data length
    state); // Event has occured, save and check it      
  #endif
      
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  Tasker(TASK_MQTT_SENDER);

  event.waiting = false;

}


void mSwitches::Loop(void) {
  if (Switch.used_bitmap) {
    if(mTime::TimeReached(&Switch.tSaved_debounce, tkr_set->Settings.switch_debounce)){
      Handler();
    }
  }
}

uint8_t mSwitches::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, GetSensorCount());

    for(uint32_t i=0; i<GetSensorCount(); i++)
    {
      if(bitRead(Switch.used_bitmap,i))
      {
        char Switch_name[50];
        DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), i, Switch_name, sizeof(Switch_name));  

        JBI->Object_Start(Switch_name);
          JBI->Add("ID", i);
          JBI->Add("state", Switch.state[i]);
          JBI->Add("last_state", Switch.last_state[i]);
          JBI->Add("debounced_state", Switch.debounced_state[i]);
          /***
           * To reduce reporting, bitmapped things are only added when set
           */
          if(bitRead(Switch.no_pullup_bitmap,i))   JBI->Add("no_pullup", true);
          if(bitRead(Switch.pulldown_bitmap,i))    JBI->Add("pulldown", true);
          if(bitRead(Switch.used_bitmap,i))        JBI->Add("used", true);
          if(bitRead(Switch.virtual_pin_bitmap,i)) JBI->Add("virtual_pin", true);

        JBI->Object_End();

      }
    }

  return JBI->End();

}


uint8_t mSwitches::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
  
    if(event.waiting)
    {
      JBI->Add("ID", event.id);
      JBI->Add("Type", event.type);

      char switch_name[50];
      DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), event.id, switch_name, sizeof(switch_name));  
      JBI->Add("Name", switch_name);

      char state_name_ctr[30]; char *name = state_name_ctr;    
      if (event.type <= 3) { // First 4 options are stored in SettingsText
        name = tkr_set->SettingsText(SET_STATE_TXT1 + event.type);
        Serial.println(name);
      } else {
        pCONT_sup->GetTextIndexed(state_name_ctr, sizeof(state_name_ctr), event.type, kSwitchPressStates);
      }

      JBI->Add("TypeName", name);
      JBI->Add("LocalTime", tkr_time->GetTime().c_str());
    }

  return JBI->End();

}


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