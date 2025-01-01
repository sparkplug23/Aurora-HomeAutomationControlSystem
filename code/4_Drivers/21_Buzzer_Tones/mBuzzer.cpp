#include "mBuzzer.h"

#ifdef USE_MODULE__DRIVERS_BUZZER_TONES

int8_t mBuzzer::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
      
      // leave as default for testing
      analogWriteRange(1023);      // Default is 1023 (Arduino.h)
      analogWriteFreq(tkr_set->Settings.pwm_frequency);  

      pinMode(23, OUTPUT);
      analogAttach(23 ,0);

    break;
    case TASK_INIT:
      Init();
      analogWrite(23, 0);
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_100_MSECOND:
      BuzzerEvery100mSec();
    break;
    case TASK_EVERY_SECOND:
      // ALOG_TST(PSTR("pinMode %d"),Buzzer.pin);
      // digitalWrite(Buzzer.pin, !digitalRead(Buzzer.pin));

      // ALOG_INF(PSTR("PinUsed = %d"), pCONT_pins->PinUsed(GPIO_BUZZER_ID));


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
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mBuzzer::Pre_Init(void)
{

  ALOG_INF(PSTR("mBuzzer::Pre_Init(void)"));

  if (pCONT_pins->PinUsed(GPIO_BUZZER_ID)) {
    pinMode(pCONT_pins->GetPin(GPIO_BUZZER_ID), OUTPUT);
    Buzzer.pin = pCONT_pins->GetPin(GPIO_BUZZER_ID);
  ALOG_INF(PSTR("mBuzzer::GPIO_BUZZER_ID(void)"));
    BuzzerSet(0);
    settings.fEnableSensor = true;
  } else
  if (pCONT_pins->PinUsed(GPIO_BUZZER_INV_ID)) {
    pinMode(pCONT_pins->GetPin(GPIO_BUZZER_INV_ID), OUTPUT);
    Buzzer.pin = pCONT_pins->GetPin(GPIO_BUZZER_INV_ID);
  ALOG_INF(PSTR("mBuzzer::GPIO_BUZZER_INV_ID(void)"));
    Buzzer.inverted = true;
    BuzzerSet(0);
    settings.fEnableSensor = true;
  }   
  else {
    settings.fEnableSensor = false;
  }
}


void mBuzzer::Init(void)
{

}


void mBuzzer::BuzzerSet(uint32_t state) {

  if (Buzzer.inverted) {
    state = !state;
  }

  if (tkr_set->Settings.flag_drivers.buzzer_freq_mode) {     // SetOption111 - Enable frequency output mode for buzzer
    static uint8_t last_state = 0;
    if (last_state != state) {
      // Set 50% duty cycle for frequency output
      // Set 0% (or 100% for inverted PWM) duty cycle which turns off frequency output either way
      analogWrite(Buzzer.pin, (state) ? tkr_set->Settings.pwm_range / 2 : 0);  // set duty cycle for frequency output
      last_state = state;
    }
  } else {
    ALOG_INF(PSTR("BUZ:pCONT_pins->DigitalWrite %d %d"),Buzzer.pin,state);
    // pCONT_pins->DigitalWrite(Buzzer.pin, 0, state);     // Buzzer On/Off
    digitalWrite(Buzzer.pin, state);     // Buzzer On/Off
  }

}


//void BuzzerBeep(uint32_t count = 1, uint32_t on = 1, uint32_t off = 1, uint32_t tune = 0, uint32_t mode = 0);
void mBuzzer::BuzzerBeep(uint32_t count, uint32_t on, uint32_t off, uint32_t tune, uint32_t mode) {
  Buzzer.set[0] = off;                       // Off duration in 100 mSec steps
  Buzzer.set[1] = on;                        // On duration in 100 mSec steps
  Buzzer.duration = 1;                       // Start buzzer on first step
  Buzzer.size = 0;
  Buzzer.tune_size = 0;
  Buzzer.tune = 0;
  Buzzer.tune_reload = 0;
  Buzzer.mode = mode;

  if (tune) {
    uint32_t tune1 = tune;
    uint32_t tune2 = tune;
    for (uint32_t i = 0; i < 32; i++) {
      if (!(tune2 & 0x80000000)) {
        tune2 <<= 1;                         // Skip leading silence
      } else {
        Buzzer.tune_size++;                  // Allow trailing silence
        Buzzer.tune_reload <<= 1;            // Add swapped tune
        Buzzer.tune_reload |= tune1 & 1;
        tune1 >>= 1;
      }
    }
    Buzzer.size = Buzzer.tune_size;
    Buzzer.tune = Buzzer.tune_reload;
  }
  Buzzer.count = count * 2;                  // Start buzzer

  ALOG_INF(PSTR("BUZ: Count %d(%d), Time %d/%d, Tune 0x%08X(0x%08X), Size %d, Mode %d"),
    count, Buzzer.count, on, off, tune, Buzzer.tune, Buzzer.tune_size, tkr_set->Settings.flag_drivers.buzzer_freq_mode);

  Buzzer.enable = (Buzzer.count > 0);
  if (Buzzer.enable) {
    if (tkr_set->Settings.sleep > PWM_MAX_SLEEP) {
      tkr_set->runtime_var.sleep = PWM_MAX_SLEEP;   // Set a maxumum value of 10 milliseconds to ensure that buzzer periods are a bit more accurate
    } else {
      tkr_set->runtime_var.sleep = tkr_set->Settings.sleep;  // Or keep the current sleep if it's lower than 10
    }
  } else {
    tkr_set->runtime_var.sleep = tkr_set->Settings.sleep;    // Restore original sleep
    BuzzerSet(0);
  }
}


void mBuzzer::BuzzerSetStateToLed(uint32_t state) {
  if (Buzzer.enable && (2 == Buzzer.mode)) {
    Buzzer.state = (state != 0);
    BuzzerSet(Buzzer.state);
  }
}


void mBuzzer::BuzzerBeep(uint32_t count) {
  BuzzerBeep(count, 1, 1, 0, 0);
}


void mBuzzer::BuzzerEnabledBeep(uint32_t count, uint32_t duration) {
  // if (tkr_set->Settings.flag_drivers.buzzer_enable) {        // SetOption67 - Enable buzzer when available
    BuzzerBeep(count, duration, 1, 0, 0);
  // }
}


void mBuzzer::BuzzerEvery100mSec(void) {
  if (Buzzer.enable && (Buzzer.mode != 2)) {
    if (Buzzer.count) {
      if (Buzzer.duration) {
        Buzzer.duration--;
        if (!Buzzer.duration) {
          if (Buzzer.size) {
            Buzzer.size--;
            Buzzer.state = Buzzer.tune & 1;
            Buzzer.tune >>= 1;
          } else {
            Buzzer.size = Buzzer.tune_size;
            Buzzer.tune = Buzzer.tune_reload;
            Buzzer.count -= (Buzzer.tune_reload) ? 2 : 1;
            Buzzer.state = Buzzer.count & 1;
            if (Buzzer.mode) {
              Buzzer.count |= 2;
            }
          }
          Buzzer.duration = Buzzer.set[Buzzer.state];
        }
      }
      BuzzerSet(Buzzer.state);
    } else {
      Buzzer.enable = false;
    }
  }
}


// const char kBuzzerCommands[] PROGMEM = "Buzzer|"  // Prefix
//   "Active|Pwm||" ;

// SO_SYNONYMS(kBuzzerSynonyms,
//   67, 111
// );

// void (* const BuzzerCommand[])(void) PROGMEM = {
//   &CmndBuzzer };

// void CmndBuzzer(void) {
//   // Buzzer <number of beeps>,<duration of beep in 100mS steps>,<duration of silence in 100mS steps>,<tune>
//   // All parameters are optional
//   //
//   // Buzzer              = Buzzer 1,1,1 = Beep once with both duration and pause set to 100mS
//   // Buzzer 0            = Stop active beep cycle
//   // Buzzer 2            = Beep twice with duration 200mS and pause 100mS
//   // Buzzer 2,3          = Beep twice with duration 300mS and pause 100mS
//   // Buzzer 2,3,4        = Beep twice with duration 300mS and pause 400mS
//   // Buzzer 2,3,4,0x0F54 = Beep a sequence twice indicated by 0x0F54 = 1111 0101 0100 with duration 300mS and pause 400mS
//   //                         Notice skipped leading zeroes but valid trailing zeroes
//   // Buzzer -1           = Beep infinite
//   // Buzzer -2           = Beep following link led

//   if (XdrvMailbox.data_len > 0) {
//     if (XdrvMailbox.payload != 0) {
//       uint32_t parm[4] = { 0 };
//       ParseParameters(4, parm);
//       uint32_t mode = 0;
//       if (XdrvMailbox.payload < 0) {
//         parm[0] = 1;                         // Default Count
//         if (XdrvMailbox.payload > -3) {
//           mode = -XdrvMailbox.payload;       // 0, 1 or 2
//         }
//       }
//       for (uint32_t i = 1; i < 3; i++) {
//         if (parm[i] < 1) { parm[i] = 1; }    // Default On time, Off time
//       }
//       BuzzerBeep(parm[0], parm[1], parm[2], parm[3], mode);
//     } else {
//       BuzzerBeep(0);
//     }
//   } else {
//     BuzzerBeep(1);
//   }
//   ResponseCmndDone();
// }

/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
void mBuzzer::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  // uint8_t relay_id= 0,state=-1;    //assume index 0 if none given

  // if(jtok = obj[PM_POWERNAME]){
  //   if(jtok.isStr()){
  //     relay_id = GetRelayIDbyName(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     relay_id  = jtok.getInt();
  //   }
  // }


  // if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }else
  // if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON_SECS]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }


  if(jtok = obj["BuzzerMode"].getObject()["FreqMode"]){
    ALOG_TST(PSTR("FreqMode=%d"),jtok.getInt());
    tkr_set->Settings.flag_drivers.buzzer_freq_mode = jtok.getInt();

  }






  uint8_t beeps = 0;

  if(jtok = obj["beeps"]){
    ALOG_TST(PSTR("bbeeps=%d"),jtok.getInt());
    BuzzerBeep(jtok.getInt());
    // pinMode(Buzzer.pin, jtok.getInt());

  }


  mqtthandler_settings.flags.SendNow = true;

}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mBuzzer::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mBuzzer::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, 0);
  return JBI->End();
    
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

void mBuzzer::MQTTHandler_Init(){

  struct handler<mBuzzer>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mBuzzer::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBuzzer::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBuzzer::ConstructJSON_Sensor;
  
} 


void mBuzzer::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} 


void mBuzzer::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mBuzzer::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mBuzzer>* list_ptr[] = {
    &mqtthandler_settings,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE__DRIVERS_BUZZER_TONES__ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
