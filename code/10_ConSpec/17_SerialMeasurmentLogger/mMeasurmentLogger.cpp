// #include "mSerialCalibrationMeasurmentLogger.h"

// #ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER

// /***
//  * Do this the super simple way.
//  * if(uart EOF was detected, in next loop create and append json) 
//  * */




// int8_t mSerialCalibrationMeasurmentLogger::Tasker(uint8_t function, JsonParserObject obj){
    
//     // Serial.printf("Tasker=%d\r\n", millis());

//     // return 1;

//   switch(function){
//     /************
//      * INIT SECTION * 
//     *******************/
//     case TASK_PRE_INIT:
//       Pre_Init();
//     break;
//     case TASK_INIT:
//       Init();
//     break;
//   }

//   if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case TASK_LOOP: 
//       EveryLoop();
//     break;  
//     case TASK_EVERY_SECOND: 
//       EverySecond();
//     break;
//     /************
//      * TRIGGERS SECTION * 
//     *******************/
//     case TASK_EVENT_INPUT_STATE_CHANGED_ID:
//       #ifdef USE_MODULE_DRIVERS_SDCARD
//       pCONT_sdcard->CommandSet_SDCard_Appending_File_Method_State(2);
//       #endif
//       // sequence_test = 0;
//       //pCONT_gps->sequence_test_global = 0;
//     break;
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_MQTT
//     case TASK_MQTT_HANDLERS_INIT:
//     case TASK_MQTT_HANDLERS_RESET:
//       MQTTHandler_Init();
//     break;
//     case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
//       MQTTHandler_Rate();
//     break;
//     case TASK_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     #endif //USE_MODULE_NETWORK_MQTT
//   }
  
//   return FUNCTION_RESULT_SUCCESS_ID;

// } // END function


// void mSerialCalibrationMeasurmentLogger::Pre_Init(void)
// {

//   settings.fEnableSensor = true;

//   /**
//    * Pin "sync_frame_status" on falling edge signifies the start of a new frame being sent, while, rising edge is the completion of the data.
//    * Rising edge should occur approx 2.5ms after falling, and will set a flag to begin formation of sync_frame sample data to be sent to SDCARD stream
//    * The amount of data to be read from RSS_STREAM will be known as it ends with two 0xFF 0xFF EOF bytes.
//    * */
  
//   #ifdef ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
//     pinMode(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, INPUT_PULLUP );
//     // attachInterrupt(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger_LOW, FALLING);
//     attachInterrupt(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger, CHANGE);
//   #endif // ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD

//   settings.fSensorCount = 0;

//   if(pCONT_pins->PinUsed(GPIO_ADC1_CH4_ID))
//   {
//     adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH4_ID);
//     adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
//     adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_4;
//     adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
//     adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
//     adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
//     pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
//     settings.fSensorCount++;
//   }

//   if(pCONT_pins->PinUsed(GPIO_ADC1_CH7_ID))
//   {
//     adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH7_ID);
//     adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
//     adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_7;
//     adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
//     adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
//     adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
//     pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
//     settings.fSensorCount++;
//   }

//   if(settings.fSensorCount){
//     settings.fEnableSensor = true;
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
//   }



// }


// void mSerialCalibrationMeasurmentLogger::Init(void)
// {

//   WiFi.mode(WIFI_OFF);
//   btStop();

//   /**
//    * Only on debug lipo-esp32, for simplifying the data to pc process
//    * */
//   #if defined(USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT) && !defined(USE_MODULE_DRIVERS_SERIAL_UART)
//     // Configure here for debugging only
//     Serial2.begin(2048000);
//   #endif

  
//   // Configure all channel atten and width
//   for(int i=0; i<settings.fSensorCount; i++)
//   {
//     switch(adc_config[i].input_pin)
//     {
//       case 32:
//         adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
//         adc1_config_width(ADC_WIDTH_BIT_12);
//         ALOG_TST(PSTR("ADC1_CHANNEL_4 set"));
//       break;
//       case 34:
//         adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );
//         adc1_config_width(ADC_WIDTH_BIT_12);
//         ALOG_TST(PSTR("ADC1_CHANNEL_6 set"));
//       break;
//       case 35:
//         adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11 );
//         adc1_config_width(ADC_WIDTH_BIT_12);
//         ALOG_TST(PSTR("ADC1_CHANNEL_7 set"));
//       break;
//     }

//   }

// }


// void mSerialCalibrationMeasurmentLogger::EveryLoop()
// {
  
//     // Serial.printf("EveryLoop=%d\r\n", millis());
//     // uint32_t start1 = millis();
// #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//   if(flag_simple_uart2_receive_frame_for_calibration_updated)
//   {
//     flag_simple_uart2_receive_frame_for_calibration_updated = false;

//     /**
//      * Create json
//      * */   

//     //create method to short wires to start and stop ability to log, in case the sd card wont disconnect

//     // uint32_t start = millis();
//     ConstructJSON_SDCard_Calibration_Frame();
//     pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());

//     // Serial.printf("deltaF=%d\n\r", millis()-start);

//   }
//   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//     // Serial.printf("delta1=%d\r\n", millis()-start1);
  

// }





// //Use gps time to set rtc time?

// void mSerialCalibrationMeasurmentLogger::EverySecond()
// {

//   // ALOG_TST(PSTR("frame = %d\t%d\t%d\t%d"), 
//   // simple_uart2_receive_frame_for_calibration[0],simple_uart2_receive_frame_for_calibration[1],simple_uart2_receive_frame_for_calibration[2],simple_uart2_receive_frame_for_calibration[3]);

//   SubTask_UpdateOLED();

//   // SubTask_HandleSDCardLogging();

//   // SubTask_Debug_BasicFileWriteTest();


//   // ConstructJSON_SDCard_Calibration_Frame();
//   //   ALOG_TST(PSTR("SuperFrame=\"%s\""), JBI->GetBufferPtr());
 

// }




// /**
//  * @brief Show logger status on OLED
//  * @note 4x11 chars at font 1
//  * 
//  * "LAT LON SQ"  LAT and LONG of gps (Without decimel) and S=Satellite count and Q=[V]alid or [E]rror
//  * "Open xxxxxx" where x is the time opened
//  * "Byte bbbbbb" where b is the number of bytes written (changing to kB when >1000)
//  * "HHMMSS     " where HHMMSS is the GPS RTC time, which is also appended to the opened file name
//  * */
// void mSerialCalibrationMeasurmentLogger::SubTask_UpdateOLED()
// {

// #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
//   char line_ctr[15] = {0};
//   char buffer[15] = {0};

//   tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

//   /**
//    * pic32 adc values 
//    * */
  
//   snprintf(buffer, sizeof(buffer), "%04d %04d",adc_values.pic32.adc2,adc_values.pic32.adc5);
//   pCONT_iDisp->LogBuffer_AddRow(buffer, 0);
//   snprintf(buffer, sizeof(buffer), "%04d %04d",adc_values.esp32.adc2,adc_values.esp32.adc5);
//   pCONT_iDisp->LogBuffer_AddRow(buffer, 1);



//   #ifdef USE_MODULE_DRIVERS_SDCARD
//   snprintf(buffer, sizeof(buffer), "%c%s%s",
//     pCONT_sdcard->sdcard_status.init_error_on_boot ? 'E' : 'f',
//     pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID ?"OPEN!":"cd",
//     &pCONT_sdcard->writer_settings.file_name[12] //skipping "APPEND_RP0_" to get just time
//   );
//   pCONT_iDisp->LogBuffer_AddRow(buffer, 2);
//   #endif //USE_MODULE_DRIVERS_SDCARD
  
//   #ifdef USE_MODULE_DRIVERS_SDCARD
//   uint32_t bytes_written = pCONT_sdcard->sdcard_status.bytes_written_to_file;
//   char unit_type = 'B';

//   if(bytes_written>50000)
//   {
//     bytes_written /= 1000; //into kB
//     unit_type = 'k';
//   }

//   snprintf(buffer, sizeof(buffer), "%d %c",bytes_written,unit_type);
  
//   // ,     
//   // /**
//   //  * show one byte from rss feed
//   //  * */  
//   // );

//   pCONT_iDisp->LogBuffer_AddRow(buffer,3);
//   #endif // USE_MODULE_DRIVERS_SDCARD

//   #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

// }





// uint8_t mSerialCalibrationMeasurmentLogger::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

//   JBI->Start();
//     JBI->Add(D_CHANNELCOUNT, 0);
//   return JBI->End();

// }


// uint8_t mSerialCalibrationMeasurmentLogger::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

//   JBI->Start();
//     JBI->Add(D_VOLTAGE, 0);
//   return JBI->End();
    
// }


// /**
//  * @brief Used to generate the output to be written to the SD card
//  * @note
//  * {"SF":[array RSS superframe ie 50 samples... CC_state, RSS2, RRS5.... for 50],
//  *  "GPS":{ minimal data, with 2 byte names}
//  * }
//  * */
// uint8_t mSerialCalibrationMeasurmentLogger::ConstructJSON_SDCard_Calibration_Frame(uint8_t json_level, bool json_appending){

//   JBI->Start();
    
//   // Debug data only
//   JBI->Add("M",millis());

//   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//   adc_values.pic32.adc2 = (uint16_t)((simple_uart2_receive_frame_for_calibration[0] << 8) | simple_uart2_receive_frame_for_calibration[1]);
//   adc_values.pic32.adc5 = (uint16_t)((simple_uart2_receive_frame_for_calibration[2] << 8) | simple_uart2_receive_frame_for_calibration[3]);
//   JBI->Add("p2", adc_values.pic32.adc2);
//   JBI->Add("p5", adc_values.pic32.adc5);
//   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

//   /**
//    * on node1 and node2 (not uav_node0), the esp32 is NOT connected to rss pins
//    * node1, this has been confirm, adc4(gpio32) interfaces pic32 digital pin
//    *                               adc7(gpio35) interfaces pic32 digital pin (not connected to anything)
//    * */
//   #ifdef DEVICE_NODE0_UAV_PIC32ADC_CALIBRATION
//   adc_values.esp32.adc2 = adc1_get_raw(ADC1_CHANNEL_4);
//   adc_values.esp32.adc5 = adc1_get_raw(ADC1_CHANNEL_7);
//   JBI->Add("e2", adc_values.esp32.adc2);
//   JBI->Add("e5", adc_values.esp32.adc5);
//   #endif

//   return JBI->End();
    
// }

// #endif
