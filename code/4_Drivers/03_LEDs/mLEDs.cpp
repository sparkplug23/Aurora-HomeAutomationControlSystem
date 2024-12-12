#include "mLEDs.h"

#ifdef USE_MODULE_DRIVERS_LEDS

int8_t mLEDs::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
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
      Config_StatusEffect();
      Refresh_AllLEDs();  // This will update all the LEDs according to their modes
    break;
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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

} // END function


void mLEDs::Pre_Init(void)
{
  
  ALOG_HGL( PSTR("D_LOG_STARTUP" "LED Init") );

  inverted_bitmask = 0; // Reset all bits

  // Lets check each type on their own, normal, inverted etc
  for(uint32_t ii=0; ii<MODULE_LEDS_MAX; ii++)
  {

    int8_t pin = -1;

    if(pCONT_pins->PinUsed(GPIO_LED1_ID, ii))
    {
      SetUsed(ii);
      pin = pCONT_pins->GetPin(GPIO_LED1_ID, ii);
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW); // Default: OFF
    }else
    if(pCONT_pins->PinUsed(GPIO_LED1_INV_ID, ii))
    {
      SetUsed(ii);
      pin = pCONT_pins->GetPin(GPIO_LED1_INV_ID, ii);
      ALOG_INF(PSTR("%d %d %d"), GPIO_LED1_INV_ID, ii, pin);
      pinMode(pin, OUTPUT);
      digitalWrite(pin, HIGH); // Default: OFF
      SetInvertFlag(ii);
    }else{
      ALOG_DBG(PSTR(D_LOG_LED "%d None"), ii);
    }

    if(pin != -1)
    {
      ALOG_INF(PSTR(D_LOG_LED "%d pin=%d %s"), ii, pin, toBinaryString(used_bitmask, MODULE_LEDS_MAX).c_str() );
    }

  }

  if(used_bitmask){ module_state.mode = ModuleStatus::Initialising; }

}


void mLEDs::Init(void)
{
  // Configured already
  module_state.mode = ModuleStatus::Running;

  leds.resize( UsedCount() );  // Allocate space for MAX_LEDS

  for (uint8_t i = 0; i < UsedCount(); i++) {
    // uint8_t pin = pCONT_pins->GetPin(GPIO_LED1_ID, i);  // Get the pin for each LED
    // pinMode(pin, OUTPUT);  // Set the pin as output

    // if (i == 0) {  // LED 1 - Blink 3 times, each blink 300ms apart, cycle every 5 seconds
    //   StartEffect_Blink(i, 3, 100, 5000, 30);  
    // } else if (i == 1) {  // LED 2 - Solid ON
    //   StartEffect_Blink(i, 3, 250, 5000, 30);  
    //   // StartEffect_On(i);
    // } else if (i == 2) {  // LED 3 - Solid OFF
    //   StartEffect_Blink(i, 10, 100, 5000, 30);  
    //   // StartEffect_Off(i);
    // } else if (i == 3) {  // LED 4 - Pulsing with 1-second period
    //   StartEffect_Blink(i, 20, 100, 5000, 30);  
    //   // StartEffect_Pulse(i, 1000);
    // }
  }

}


void mLEDs::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(used_bitmask)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", UsedCount());  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<MODULE_LEDS_MAX; sensor_id++)
    {      
      //if bit0 is used as LEDSTATUS, then show this here too
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%d, ", sensor_id);    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_LOG "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


void mLEDs::Refresh_AllLEDs() {
  for (uint8_t i = 0; i < UsedCount(); i++) 
  {
    Refresh_LED(i);  // Update the state of each LED
  }
}


void mLEDs::Refresh_LED(uint8_t led_index)
{

  LedState led = leds[led_index];
  uint8_t pin = pCONT_pins->GetPin(GPIO_LED1_ID, led_index);  // Get the corresponding pin for each LED

  uint32_t currentTime = millis();
  uint32_t tElapsed = currentTime - led.lastUpdateTime;

  // Check if duration has exceeded, and only reset once
  if (led.duration_secs > 0 && (currentTime - led.startTime) >= (led.duration_secs * 1000)) {
      if (led.effect != LED_OFF) { // This check ensures that reset happens only once
          digitalWrite(pin, LOW);  // Turn off the LED after duration is exceeded
          led.effect = LED_OFF;      // Set the mode to OFF
          ALOG_INF(PSTR("LED%d Reset OFF after duration"), pin);
      }
      return;  // Stop further processing as the LED is now off
  }

  switch (led.effect) {
      case LED_ON:
          digitalWrite(pin, HIGH);
          break;

      case LED_OFF:
          digitalWrite(pin, LOW);
          break;

      case LED_BLINK:
      {
          uint32_t totalCycle = (led.period * led.count) + led.groupPause;  // Total cycle time (blinks + pause)

          if (tElapsed >= totalCycle) {
              // Reset the cycle and start again
              led.lastUpdateTime = currentTime;
              tElapsed = 0;
          }

          // If we are within the blink phase
          if (tElapsed < (led.period * led.count)) {
              uint32_t phase = tElapsed / led.period;  // Which blink are we in?

              // Toggle the LED on/off in each interval
              if ((tElapsed % led.period) < (led.period / 2)) {
                  digitalWrite(pin, HIGH);  // Turn LED on
              } else {
                  digitalWrite(pin, LOW);   // Turn LED off
              }
          } else {
              // After the blinks, keep LED off during the pause
              digitalWrite(pin, LOW);
          }
      }
      break;

      case LED_PULSE:
      {
          // Calculate the total time for pulse up and down (one complete pulse cycle)
          uint32_t totalCycle = (led.period * led.count) + led.groupPause;

          // Check if we are at the end of the pulse cycle (including pause)
          if (tElapsed >= totalCycle) {
              // ALOG_INF(PSTR("tElapsed >= totalCycle %d %d"),tElapsed,totalCycle);
              // Reset the cycle and start again
              led.lastUpdateTime = currentTime;
              tElapsed = 0;  // Reset tElapsed for the new cycle
          }

              // ALOG_INF(PSTR("tElapsed < (led.period * led.count) %d %d %d"), tElapsed,led.period,led.count);
          // If we are within the pulse phase
          if (tElapsed < (led.period * led.count)) {
              uint32_t currentPhaseTime = tElapsed % led.period;  // Get the current time within the period

              // Calculate the duty cycle as a sine wave based on the current phase
              float phase = (float)currentPhaseTime / (float)led.period;
              float dutyCycle = (sin(2 * PI * phase) + 1) / 2;  // Sine wave for smooth pulsing

              // Set PWM brightness based on the duty cycle (for smooth pulsing)
              analogWrite(pin, (int)(dutyCycle * 255));  // Set brightness based on the duty cycle

              // Debugging: Print the current duty cycle
              // char floats[20] = {0}; 
              // mSupport::float2CString(dutyCycle, 2, floats);
              // ALOG_INF(PSTR("float %s"), floats);
          } else {
              // After the pulse, keep LED off during the pause period
              analogWrite(pin, 0);  // Turn LED off (0 brightness)
              // ALOG_INF(PSTR("off"));
          }
      }
      break;

  }

}


void mLEDs::StartEffect_On(uint8_t index) {
  leds[index].effect = LED_ON;
  leds[index].lastUpdateTime = millis();
}


void mLEDs::StartEffect_Off(uint8_t index) {
  leds[index].effect = LED_OFF;
  leds[index].lastUpdateTime = millis();
}


void mLEDs::StartEffect_Blink(uint8_t index, uint8_t blinkCount, uint16_t blinkInterval, uint16_t blinkGroupPause, uint8_t duration_secs) 
{
  leds[index].effect = LED_BLINK;
  leds[index].count = blinkCount;
  leds[index].period = blinkInterval;
  leds[index].groupPause = blinkGroupPause;  // Pause time between blink groups
  leds[index].lastUpdateTime = millis();
  leds[index].state = LOW;  // Start with the LED off
  leds[index].duration_secs = duration_secs;  // Duration in seconds
  leds[index].startTime = millis();  // Record the start time
}


void mLEDs::StartEffect_Pulse(uint8_t index, uint8_t pulseCount, uint16_t period, uint16_t groupPause, uint8_t duration_secs) 
{
  leds[index].effect = LED_PULSE;
  leds[index].period = period;
  leds[index].groupPause = groupPause;  // Pause time between pulse cycles
  leds[index].count = pulseCount;       // Number of pulses in each cycle
  leds[index].lastUpdateTime = millis();
  leds[index].duration_secs = duration_secs;  // Duration in seconds
  leds[index].startTime = millis();  // Record the start time

  // Attach the PWM channel to the pin for the pulse mode
  uint8_t pin = pCONT_pins->GetPin(GPIO_LED1_ID, index);
  analogAttach(pin, index);  // The PWM channel corresponds to the index of the LED
}


void mLEDs::SetInvertFlag(uint8_t b) {
  bitSet(inverted_bitmask, b);
}

bool mLEDs::IsUsed(uint8_t index) {
  return (pCONT_pins->PinUsed(GPIO_LED1_ID, index) || pCONT_pins->PinUsed(GPIO_LED1_INV_ID, index) || bitRead(used_bitmask, index));
}

void mLEDs::SetUsed(uint8_t index) {
  bitSet(used_bitmask, index);
}

uint8_t mLEDs::UsedCount() {
  return __builtin_popcount(used_bitmask);
}


/*
 If LEDs are in this mode, update them (eg to blink/pulse etc)
index if set with index, will set the power type (so if 4 power leds, then they are 0,1,2,3 which may not actually be started from the first ones)
If index==255, then all are set regardless by index
*/
void mLEDs::Set_PowerLED(bool state, uint8_t index) 
{
  // This will cycle through and set all LEDs configured as power type


}

/*
 If LEDs are in this mode, update them (eg to blink/pulse etc)
 This function lets me set the status to show certain types, with timing, and effect (pulse/blink/on/off)
*/
void mLEDs::Set_StatusLED(uint8_t count, uint16_t interval, uint16_t event_pause, uint8_t duration_secs, LedEffect effect)
{
  // This will cycle through and set all LEDs configured as status type

}



/**
 * @brief Status LED 
 * 
 * For each LED configured as status type, this function will read all status flags across the systems and make sure the LEDs are configured to show.
 * This is a interface of sorts.
 * 
 */
void mLEDs::Config_StatusEffect()
{
  
  DEBUG_LINE;
  uint8_t blinkinterval = 1;
  
  if (pCONT_set->runtime.global_state.data) {                              // Any problem
    if (pCONT_set->runtime.global_state.mqtt_down) { blinkinterval = 7; }  // MQTT problem so blink every 2 seconds (slowest)
    if (pCONT_set->runtime.global_state.wifi_down) { blinkinterval = 3; }  // Wifi problem so blink every second (slow)
    pCONT_set->runtime.blinks = 201;                                       // Allow only a single blink in case the problem is solved
  }
  


}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mLEDs::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

int8_t led_id = -1;
uint16_t state_value = 0;

  int8_t relay_id= -1,state=-1;    //assume index 0 if none given

  if(jtok = obj["LED"].getObject()["Name"]){
    if(jtok.isStr()){
      // relay_id = GetRelayIDbyName(jtok.getStr());
      ALOG_INF( PSTR("relay_id = %s"), jtok.getStr() );
    }else 
    if(jtok.isNum()){
      relay_id  = jtok.getInt();
    }

    ALOG_INF( PSTR("relay_id = %d"), relay_id );
  }

  // Primary method since v0.86.14.21
  if(jtok = obj["LED"].getObject()["State"]){
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else 
    if(jtok.isNum()){
      state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
    }

    /**
     * @brief If off, clear any timer decounters for relays
     * 
     */
    if(state == 0)
    {
      // CommandSet_Timer_Decounter(0, relay_id);
    }

		//state needs checked for flipped
		// if(state == 2){

		// }

  }



  if(jtok = obj["Blink"])
  {
    std::vector<uint32_t> data;
    for(auto v : jtok.getArray()) 
    {
      data.push_back(v.getInt());
    }
    ALOG_INF(PSTR("TEST BLINK"));
    StartEffect_Blink(data[0], data[1], data[2], data[3], data[4]);
  }
  if(jtok = obj["Pulse"])
  {
    std::vector<uint32_t> data;
    for(auto v : jtok.getArray()) 
    {
      data.push_back(v.getInt());
    }
    ALOG_INF(PSTR("TEST PULSE"));
    StartEffect_Pulse(data[0], data[1], data[2], data[3], data[4]);
  }

  if(IsWithinRange(state, 0,10)){//} && IsWithinRange(relay_id, 0,settings.leds_found)){
    // If set manually, then override EffectMode to be manual
    // CommandSet_LED_Power(state,relay_id);
  }


}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mLEDs::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, UsedCount() );
  return JBI->End();

}

uint8_t mLEDs::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Add("LED1_INV", pCONT_pins->GetPin(GPIO_LED1_INV_ID));
    JBI->Add("LED2_INV", pCONT_pins->GetPin(GPIO_LED2_INV_ID));

  return JBI->End();

}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mLEDs::MQTTHandler_Init()
{

  struct handler<mLEDs>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLEDs::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mLEDs::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif
