#include "mUserMod_01.h"

// User modified controller
// Named with generic name, so I can easily change it for testing hardware without requiring large modifications to TaskerInterface

#ifdef USE_MODULE_CONTROLLER_USERMOD_01

int8_t mUserMod_01::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    Init();
  }

  // Only continue in to tasker if module was configured properly
  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:    

    break;
    case TASK_LOOP:
      EveryLoop();
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
    // case TASK_MQTT_HANDLERS_INIT:
    // case TASK_MQTT_HANDLERS_RESET:
    //   MQTTHandler_Init();
    // break;
    // case TASK_MQTT_SENDER:
    //   MQTTHandler_Sender(); //optional pass parameter
    // break;
    // case TASK_MQTT_CONNECTED:
    //   MQTTHandler_RefreshAll();
    // break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

}


void mUserMod_01::Init(void)
{
 analogWriteRange(tkr_set->Settings.pwm_range);      // Default is 1023 (Arduino.h)
  analogWriteFreq(tkr_set->Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)

  // analogWriteFreqRange(0, tkr_set->Settings.pwm_frequency, tkr_set->Settings.pwm_range);
  pinMode(22, OUTPUT);
  pinMode(2, OUTPUT);
  analogWrite(22, pic32.adc_low);
  analogWrite(2, pic32.adc_high);

  Serial2.begin(1536000, SERIAL_8N1, 16, 17);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(21, OUTPUT);
}

void mUserMod_01::EveryLoop()
{
  EveryLoop_EmulatePic32Measurement();
}


/**
 * @brief emulate the data from a PIC32 during measurements with dual radio controllers
 * 
 * On each call, ramp the ADC1 and ADC2 pin outputs through inverse levels
 * Send 1 frame per call, with superframe ending every 50 then reset
 * Include digitalIO to mark each frame, and superframe 
 * 
 * 22 = analogOut 1
 * 23 = analogOut 2
 * UART2_RX 16
 * UART2_TX 17
 * */
void mUserMod_01::EveryLoop_EmulatePic32Measurement()
{

  // digitalWrite(21, !digitalRead(21));

  if(mTime::TimeReached(&pic32.tSaved_AdcRampTick, 100))
  {

    digitalWrite(4, LOW); //start adc slot

    // Ramp up
    // switch(pic32.adc_dir)
    // {
    //   case 1: // accending
            
    //     if(pic32.adc_value < pic32.adc_high)
    //     {
    //       pic32.adc_value+=100;
    //     }
    //     else{
    //       pic32.adc_dir = 0; // decending
    //     }

    //   break;
    //   case 0: // decending
            
    //     if(pic32.adc_value > pic32.adc_low)
    //     {
    //       pic32.adc_value-=100;
    //     }
    //     else{
    //       pic32.adc_dir = 1; // accending
    //     }

    //   break;
    // }
    pic32.adc_value = 1234;

    // Write analog out
    analogWrite(22, pic32.adc_value);
    analogWrite(2, pic32.adc_high-pic32.adc_value); //inverse
    // ALOG_TST(PSTR("pic32.adc_value=%d"),pic32.adc_value);
    digitalWrite(4, HIGH); //ending adc slot
  }


  /**
   * @note Old superframe
   * */
  // Preample        SequenceNumber   "tx_board_number"    RSS_2400      RSS_5805
  // "FFFFFFFF11"     "11223344"     [ "TX"                "2730"        "5805" ] = repeated 3 times for each tx_board

  // Need to build this json in parts and send each bit, that way I can have the esp32 measure its ADC Values into a vector to be appending onto the superframe!

  /**
   * SuperFrame, is 50 radio cycles, triggered via one 868mhz pulse... I will actually need to have te 868mhz internally retrain when this happens, so missing packets do not mean missing data
    
Store the SF inside the pic32 in RAM, then send after its full, maybe I need two versions as writing and reading with pointer swapping


   * // Expected SF every 10ms, so sd write needs to be all this * 100 per second

    {
     "SF":              // array of pic32 measurements
        {
          sq: [0, 2, 3, 4, 5, 6, 7]
          r2: [2730, 2730, 2730, 2730, 2730, 2730],
          r5: [5805, 5805, 5805, 5805, 5805, 5805]
        }
      ],
      "e2": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ], // vector/array of readings, triggered by gpio to be max 50, and reset by SF gpio],
      "e5": [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ], // vector/array of readings, triggered by gpio to be max 50, and reset by SF gpio],
      "gp":{"la":5,"lg":-6,"at":5017,"sd":0.408}
    }
   




   * */

  // if(mTime::TimeReached(&pic32.tSaved_MeasureTest, 1000))
  // {




  // }



  if(mTime::TimeReached(&pic32.tSaved_MeasureTest, 1000))
  {
    memset(buffer, 0, sizeof(buffer));

    digitalWrite(15, HIGH);

    snprintf(buffer + strlen(buffer), sizeof(buffer), "{\"sq\":[");
    for(int i=0;i<50;i++)
    {
      snprintf(buffer + strlen(buffer), sizeof(buffer), "%d%s",i+pic32.sequencenum, i<49?",":"");
    }
    snprintf(buffer + strlen(buffer), sizeof(buffer), "],\"r2\":[");
    for(int i=0;i<50;i++)
    {
      snprintf(buffer + strlen(buffer), sizeof(buffer), "%d%s",i, i<49?",":"");
    }
    snprintf(buffer + strlen(buffer), sizeof(buffer), "],\"r5\":[");
    for(int i=0;i<50;i++)
    {
      snprintf(buffer + strlen(buffer), sizeof(buffer), "%d%s",pic32.adc_value, i<49?",":"");
    }
    snprintf(buffer + strlen(buffer), sizeof(buffer), "]}");

    // digitalWrite(5, HIGH);
    ALOG_TST(PSTR("buffer=\"%s\""), buffer);
    Serial2.println(buffer);
    // digitalWrite(5, LOW);

    pic32.sequencenum+=50;
    
    digitalWrite(15, LOW);

  }

  // snprintf(buffer, sizeof(buffer), "")

//{"SF":[00020310002000010203100020000202031000200003020310002000040203100020000502031000200006020310002000070203100020000802031000200009020310002000],"SQ":0,"MS":63033,"Uptime":"00:00:35","GPS":{"la":5,"lg":-6,"at":5017,"sd":0.408}}

/*


  // Json method
  [
    "sn":1,
    "t":1,
    "2":"2730",
    "5":"5805"
  ],
  // sending a GPIO message will cause this to be stored as frame, and have the esp32 adc values added too
  [],
  [],


ff ff ff ff 11 
00 00 01 8f [00, 00 00, 00 00]
00 00 01 8f 01 03 fe 00 00 
00 00 01 8f 02 03 fe 00 00 
00 00 01 8f 03 03 fe 00 00 
00 00 01 90 00 00 00 00 00 
00 00 01 90 01 03 fe 00 00 
00 00 01 90 02 03 fe 00 00 
00 00 01 90 03 03 fe 00 00 
00 00 01 91 00 00 00 00 00 
00 00 01 91 01 03 fe 00 00 
00 00 01 91 02 03 fe 00 00 
00 00 01 91 03 03 fc 00 00 
00 00 01 92 00 00 00 00 00 
00 00 01 92 01 03 fc 00 00 
00 00 01 92 02 03 fe 00 00 
00 00 01 92 03 03 fc 00 00 
00 00 01 93 00 00 00 00 00 
00 00 01 93 01 03 fb 00 00 
00 00 01 93 02 03 fe 00 00 
00 00 01 93 03 03 fb 00 00 
00 00 01 94 00 00 00 00 00 
00 00 01 94 01 03 fb 00 00 
00 00 01 94 02 03 fe 00 00 
00 00 01 94 03 03 fa 00 00 
00 00 01 95 00 00 00 00 00 
00 00 01 95 01 03 fa 00 00 
00 00 01 95 02 03 fe 00 00 
00 00 01 95 03 03 f9 00 00 
00 00 01 96 00 00 00 00 00 
00 00 01 96 01 03 fb 00 00 
00 00 01 96 02 03 fe 00 00 
00 00 01 96 03 03 fb 00 00 
00 00 01 97 00 00 00 00 00 
00 00 01 97 01 03 fb 00 00 
00 00 01 97 02 03 ff 00 00 
00 00 01 97 03 03 fb 00 00 
00 00 01 98 00 00 00 00 00 
00 00 01 98 01 03 fb 00 00 
00 00 01 98 02 03 ff 00 00 
00 00 01 98 03 03 fb 00 00 
00 00 01 99 00 00 00 00 00 
00 00 01 99 01 03 fb 00 00 
00 00 01 99 02 03 ff 00 00 
00 00 01 99 03 03 fb 00 00 
00 00 01 9a 00 00 00 00 00 
00 00 01 9a 01 03 fe 00 00 
00 00 01 9a 02 03 fe 00 00 
00 00 01 9a 03 03 fe 00 00 
00 00 01 9b 00 00 00 00 00 
00 00 01 9b 01 03 fe 00 00 
00 00 01 9b 02 03 fe 00 00 
00 00 01 9b 03 03 fe 00 00 
00 00 01 9c 00 00 00 00 00 
00 00 01 9c 01 03 fe 00 00 
00 00 01 9c 02 03 fe 00 00 
00 00 01 9c 03 03 fe 00 00 
00 00 01 9d 00 00 00 00 00 
00 00 01 9d 01 03 fe 00 00 
00 00 01 9d 02 03 fe 00 00 
00 00 01 9d 03 03 fd 00 00 
*/


}


void mUserMod_01::Pre_Init(){
  
  // #ifndef ENABLE_DEVFEATURE_FANTYPE_IFAN03
  // if(pCONT_pins->PinUsed(GPIO_FAN_PWM1_ID)) {  // not set when 255
  //   pin = pCONT_pins->GetPin(GPIO_FAN_PWM1_ID);
  //   pinMode(pin, OUTPUT);
  //   Serial.println("PIMODE HERE ");
  //   Serial.println(pin);
    settings.fEnableModule = true;
  // }
  // #endif

}


uint8_t mUserMod_01::ConstructJSON_Settings(uint8_t json_level, bool json_appending){
  
  JBI->Start();
    JBI->Add_P("test",0);  
  JBI->End();

}

uint8_t mUserMod_01::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
  JBI->End();

}


void mUserMod_01::MQTTHandler_Init(){

  struct handler<mUserMod_01>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mUserMod_01::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  ptr->ConstructJSON_function = &mUserMod_01::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  ptr->ConstructJSON_function = &mUserMod_01::ConstructJSON_Sensor;
  
}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mUserMod_01::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mUserMod_01::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mUserMod_01::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_CONTROLLER_USERMOD_01
