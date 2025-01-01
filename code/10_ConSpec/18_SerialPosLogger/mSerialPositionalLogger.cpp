#include "mSerialPositionalLogger.h"

#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER


// char isr_rss_buffer[1000];
bool extern_flag_sd_stream_buffer_lock = false;

// {
  // uint8_t stream_tmp_buf_writer_index = 0;
  // char stream_tmp_buffer_0[1000];
  // uint16_t stream_tmp_buflen_0;
  // char stream_tmp_buffer_1[1000];
  // uint16_t stream_tmp_buflen_1;
// };


// static SemaphoreHandle_t mutex_filling_sd_writer;


/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***** INTERRUPT pin controls ******************************************************************************************************************************************************************

- Syncframe duration ie durtion of T45 on the pic32
            - started = reset RXON_TIMESLOT_COUNTER
            - ended   =
- RXON sample duration ie when sampling on the esp32 should be recorded
            - started = increment a RXON_TIMESLOT_COUNTER, which is reset on syncframe
            - ended   =
                      
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/


/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
******SYNCFRAME_EVENT =  External Pin Interrupt Triggers For ADC *****************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/

#ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS


// If I changed the CC1110 sync to be flipping, instead of rising edge, it would cut down calling this function so quick!!
void IRAM_ATTR ISR_External_Pin_Sync_Frame_Status_Event_Trigger()
{
//   // pCONT_serial_pos_log->sync_frame_data.flag_pin_active = true;
//   // // pCONT_adc_internal->adc_config[1].flag_external_interrupt_triggered_reading = true;
  
//   // gpio_num_t pin = (gpio_num_t)(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM & 0x1F);
//   // int state = state = (GPIO_REG_READ(GPIO_IN_REG)  >> pin) & 1U;

//   // if(state==LOW)
//   // {
//   //   // pCONT_serial_pos_log->sync_frame_data.flag_started = true;

//   //   //Serial2.printf("ISR_External_Pin_Sync_Frame_Status_Event_Trigger");
      
//   //   // #ifdef ENABLE_ESP32_ADC_SAMPLING
//   //   // /**
//   //   //  * toggle to the other buffer to be writting into, the read will check which is not active
//   //   //  * */
//   //   // pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index ^= 1; // Reset ADC syncframe index
//   //   // /**
//   //   //  * Use new buffer set here, to reset its counter
//   //   //  * The other buffer not being written into, the counter values here wont be reset until the next ISR, allowing it to be checked for length of data on previous frame
//   //   //  * */
//   //   //   pCONT_adc_internal->isr_capture.within_buffer_iter_counter = 0;
//   //   // #endif // ENABLE_ESP32_ADC_SAMPLING

//   // }
//   // else
//   // {
//     // pCONT_serial_pos_log->sync_frame_data.flag_ended = true;

//     // I need to force this to happen now, to ensure it happens 
//     #ifdef ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
//     // ISR_Direct_SubTask_Generate_SyncFrame_To_SDCard_Stream();

//     //MEthod 3: What is this assert just takes the RSS part of the frame, completing it in core0 with gps parts?

//       /**
//        *  RingBuffer2 is RSS buffer
//        *  ConstructJSON_SDCardSuperFrame
//        **/
//       JBI->Start_NoMemClear(); //restarts internal length but is faster

//       if(sequence_test_global == 0){
//         JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
//       }
//       JBI->Add("N", sequence_test_global++);
//       JBI->Append(",\"F\":[");

//       // Read from the alternate one
//       switch(uart_rss_tmp_buf_writer_index) //based on the active index, I should be reading the last only
//       {
//         case 0:
//           JBI->Append_P("1,");
//           // for(int i=0;i<uart_rss_tmp_buflen_1;i++)
//           // {
//           //   if(i<162){
//           //     JBI->Append_P("%d,", uart_rss_tmp_buffer_1[i]);
//           //   }else{ 
//           //     JBI->Append_P("%d",  uart_rss_tmp_buffer_1[i]); 
//           //   }
//           // }
//           //reset 
//           uart_rss_tmp_buflen_1 = 0; // this is clear, lets add a check to uart to make sure only to write if empty
//         break;
//         case 1:
//           JBI->Append_P("0,");
//           // for(int i=0;i<uart_rss_tmp_buflen_0;i++)
//           // {
//           //   if(i<162){
//           //     JBI->Append_P("%d,", uart_rss_tmp_buffer_0[i]);
//           //   }else{ 
//           //     JBI->Append_P("%d",  uart_rss_tmp_buffer_0[i]); 
//           //   }
//           // }
//           //reset 
//           uart_rss_tmp_buflen_0 = 0;
//         break;
//       }
//       // Read from the alternate one

//       // Check all buffers, using the right order

// //       uint8_t indexes[3] = {0};
// // /**
// //  * Get the correct order to check for data in buffers
// //  * */
// // if(uart_rss_tmp_buf_writer_index == 0){
// //   indexes[0] = 1;
// //   indexes[0] = 1;
// //   indexes[0] = 1;
// // }else
// // if(uart_rss_tmp_buf_writer_index == 0){
// //   indexes[0] = 1;
// //   indexes[0] = 1;
// //   indexes[0] = 1;
// // }else
// // if(uart_rss_tmp_buf_writer_index == 0){
// //   indexes[0] = 1;
// //   indexes[0] = 1;
// //   indexes[0] = 1;
// // }


// //       switch(indexes[i]) //based on the active index, I should be reading the last only
// //       {
// //         case 0:
// //           if()
// //           JBI->Append_P("0,");
// //           for(int i=0;i<uart_rss_tmp_buflen_0;i++)
// //           {
// //             if(i<162){
// //               JBI->Append_P("%d,", uart_rss_tmp_buffer_0[i]);
// //             }else{ 
// //               JBI->Append_P("%d",  uart_rss_tmp_buffer_0[i]); 
// //             }
// //           }
// //           //reset
// //           uart_rss_tmp_buflen_0 = 0;
// //         break;
// //         case 1:
// //           JBI->Append_P("1,");
// //           for(int i=0;i<uart_rss_tmp_buflen_1;i++)
// //           {
// //             if(i<162){
// //               JBI->Append_P("%d,", uart_rss_tmp_buffer_1[i]);
// //             }else{ 
// //               JBI->Append_P("%d",  uart_rss_tmp_buffer_1[i]); 
// //             }
// //           }
// //           //reset 
// //           uart_rss_tmp_buflen_1 = 0;
// //         break;
// //         case 2:
// //           JBI->Append_P("2,");
// //           for(int i=0;i<uart_rss_tmp_buflen_2;i++)
// //           {
// //             if(i<162){
// //               JBI->Append_P("%d,", uart_rss_tmp_buffer_2[i]);
// //             }else{ 
// //               JBI->Append_P("%d",  uart_rss_tmp_buffer_2[i]); 
// //             }
// //           }
// //           //reset 
// //           uart_rss_tmp_buflen_2 = 0;
// //         break;
// //       }
// // }
//       JBI->Append("]\n\r");
      
//       /**
//        * New method, must write to a few circler buffers that is meant for sdcard
//        * */
//       switch(stream_tmp_buf_writer_index)
//       {
//         case 0:
//           stream_tmp_buflen_0 = JBI->GetLength();
//           memcpy(stream_tmp_buffer_0, JBI->GetBufferPtr(), stream_tmp_buflen_0);
//           stream_tmp_buf_writer_index = 1;
//         break;
//         case 1:
//           stream_tmp_buflen_1 = JBI->GetLength();
//           memcpy(stream_tmp_buffer_1, JBI->GetBufferPtr(), stream_tmp_buflen_1);
//           stream_tmp_buf_writer_index = 0;
//         break;
//       }

//     #endif

//   // }
}
#endif // ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS






// #ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS

// void IRAM_ATTR ISR_External_Pin_Sync_Frame_Status_Event_Trigger()
// {
//   pCONT_serial_pos_log->sync_frame_data.flag_pin_active = true;
//   // pCONT_adc_internal->adc_config[1].flag_external_interrupt_triggered_reading = true;
  
//   gpio_num_t pin = (gpio_num_t)(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM & 0x1F);
//   int state = state = (GPIO_REG_READ(GPIO_IN_REG)  >> pin) & 1U;

//   if(state==LOW)
//   {
//     pCONT_serial_pos_log->sync_frame_data.flag_started = true;

//     //Serial2.printf("ISR_External_Pin_Sync_Frame_Status_Event_Trigger");
      
//     // #ifdef ENABLE_ESP32_ADC_SAMPLING
//     // /**
//     //  * toggle to the other buffer to be writting into, the read will check which is not active
//     //  * */
//     // pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index ^= 1; // Reset ADC syncframe index
//     // /**
//     //  * Use new buffer set here, to reset its counter
//     //  * The other buffer not being written into, the counter values here wont be reset until the next ISR, allowing it to be checked for length of data on previous frame
//     //  * */
//     //   pCONT_adc_internal->isr_capture.within_buffer_iter_counter = 0;
//     // #endif // ENABLE_ESP32_ADC_SAMPLING

//   }
//   else
//   {
//     pCONT_serial_pos_log->sync_frame_data.flag_ended = true;

//     // I need to force this to happen now, to ensure it happens 
//     #ifdef ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
//     // ISR_Direct_SubTask_Generate_SyncFrame_To_SDCard_Stream();

//     //MEthod 3: What is this assert just takes the RSS part of the frame, completing it in core0 with gps parts?

//       /**
//        *  RingBuffer2 is RSS buffer
//        *  ConstructJSON_SDCardSuperFrame
//        **/
//       JBI->Start_NoMemClear(); //restarts internal length but is faster

//       JBI->Add("N,", sequence_test_global++);
//       JBI->Append("\"F\":[");
//       // Read from the alternate one
//       switch(uart_rss_tmp_buf_writer_index)
//       {
//         case 0:
//           for(int i=0;i<uart_rss_tmp_buflen_1;i++)
//           {
//             if(i<161){
//               JBI->Append_P("%d,", uart_rss_tmp_buffer_1[i]);
//             }else{ 
//               JBI->Append_P("%d",  uart_rss_tmp_buffer_1[i]); 
//             }
//           }
//           //reset
//           uart_rss_tmp_buflen_1 = 0;
//         break;
//         case 1:
//           for(int i=0;i<uart_rss_tmp_buflen_0;i++)
//           {
//             if(i<161){
//               JBI->Append_P("%d,", uart_rss_tmp_buffer_0[i]);
//             }else{ 
//               JBI->Append_P("%d",  uart_rss_tmp_buffer_0[i]); 
//             }
//           }
//           //reset 
//           uart_rss_tmp_buflen_0 = 0;
//         break;
//       }
//       JBI->Append("]");


//   //     if(sequence_test_global == 0){
//   //       JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
//   //     }

//   //     JBI->Add("M",millis());

//   //     // GPS data
//   //     #ifdef USE_MODULE_SENSORS_GPS_SERIAL
//   //     JBI->Object_Start("G");
//   //       JBI->Add("t", pCONT_gps->my_gps_vals.lat); 
//   //       JBI->Add("n", pCONT_gps->my_gps_vals.lon); 
//   //       JBI->Add("a", pCONT_gps->my_gps_vals.alt); // above mean sea level, in cm 
//   //       JBI->Add("d", pCONT_gps->my_gps_vals.speed);   // nautical miles per hour
//   //       // JBI->Add("hd", pCONT_gps->my_gps_vals.heading_cd); // degrees
//   //       // JBI->Add("gh", pCONT_gps->my_gps_vals.geoidHeight_cm); // Height of the geoid above the WGS84 ellipsoid
//   //       // JBI->Object_Start("Gt");
//   //         JBI->Add("h", pCONT_gps->my_gps_vals.hours);
//   //         JBI->Add("m", pCONT_gps->my_gps_vals.minutes);
//   //         JBI->Add("s", pCONT_gps->my_gps_vals.seconds);
//   //         JBI->Add("i", pCONT_gps->my_gps_vals.dateTime_ms); // are always hundreds, so shorted to 1 sig fig then recover on matlab
//   //       // JBI->Object_End();
//   //     JBI->Object_End();
//   //     #endif // USE_MODULE_SENSORS_GPS_SERIAL
      
//   //     BufferWriterI->Append(",\"F\":[");
//   // //     // BufferWriterI->Append(",\"F\":\"");

//   //     // Read from the alternate one
//   //     switch(uart_rss_tmp_buf_writer_index)
//   //     {
//   //       case 0:
//   //         for(int i=0;i<uart_rss_tmp_buflen_1;i++)
//   //         {
//             // BufferWriterI->Append_P(PSTR("%d%s"), uart_rss_tmp_buffer_1[i], i<uart_rss_tmp_buflen_1-1? ",": ""); 
//   //         }
//   //         //reset
//   //         uart_rss_tmp_buflen_1 = 0;
//   //       break;
//   //       case 1:
//   //         for(int i=0;i<uart_rss_tmp_buflen_0;i++)
//   //         {
//   //           BufferWriterI->Append_P(PSTR("%d%s"), uart_rss_tmp_buffer_0[i], i<uart_rss_tmp_buflen_0-1? ",": ""); 
//   //         }
//   //         //reset 
//   //         uart_rss_tmp_buflen_0 = 0;
//   //       break;
//   //     }

//   // //     // size_t rss_data_read_size = 0;
//   // //     // char *rss_data_read = (char *)xRingbufferReceiveFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, &rss_data_read_size);
//   // //     // if (rss_data_read != NULL) { // Read from buffer
//   // //     //   //memcpy(isr_rss_buffer,rss_data_read,rss_data_read_size);
//   // //     //   for(int i=0;i<rss_data_read_size;i++){
//   // //     //     BufferWriterI->Append_P(PSTR("%d%s"), rss_data_read[i], i<rss_data_read_size-1? ",": ""); 
//   // //     //   }
//   // //     //   vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)rss_data_read); // Free memory
//   // //     // }

//       // BufferWriterI->Append("]");
//   //     // BufferWriterI->Append("\"");

//       // JBI->End();

//   // //     //   /**
//   // //     //    * Write all data with ISR safe method into sd output ringbuffer
//   // //     //    * */
//   // //     //   // while(extern_flag_sd_stream_buffer_lock==true); // if locked from the other side, wait briefly, assuming this will be over quick enough
//   // //     //   // extern_flag_sd_stream_buffer_lock = true;
//   // //     // char* testm = "testm";
//   // //     // BaseType_t dummyval;
//   // //     // // xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, BufferWriterI->GetPtr(), BufferWriterI->GetLength(), &dummyval);
//   // //     // // extern_flag_sd_stream_buffer_lock = false;    
//   // //     // xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, testm, 5, &dummyval);

//       /**
//        * New method, must write to a few circler buffers that is meant for sdcard
//        * */
//       switch(stream_tmp_buf_writer_index)
//       {
//         case 0:
//           stream_tmp_buflen_0 = JBI->GetLength();
//           memcpy(stream_tmp_buffer_0, JBI->GetBufferPtr(), stream_tmp_buflen_0);
//           stream_tmp_buf_writer_index = 1;
//         break;
//         case 1:
//           stream_tmp_buflen_1 = JBI->GetLength();
//           memcpy(stream_tmp_buffer_1, JBI->GetBufferPtr(), stream_tmp_buflen_1);
//           stream_tmp_buf_writer_index = 0;
//         break;
//       }

//     #endif

//   }
// }
// #endif // ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS




/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
****** RXON_TIMESLOT_ENABLED = Where the pic32 RXON sampling timeslot is happening *****************************************************************************************************************************************************************
******
* @note - This will be set during the sampling window of the pic32 timeslot, and will be recorded during this period
On each started/low, a RXON_SAMPLE_COUNTER will be incremented so all samples in item will be known which slot it comes from
Instead of using a second buffer, instead, 2 bytes of the DMA read will also have this number appended to it (bytes 1001 and 1002), these will be extracted during json format and thus not during matlab



*****************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/


#ifdef ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD

void IRAM_ATTR ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger()
{

  pCONT_serial_pos_log->sync_frame_data.flag_pin_active = true;

  gpio_num_t pin = (gpio_num_t)(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM & 0x1F);
  int state = state = (GPIO_REG_READ(GPIO_IN_REG)  >> pin) & 1U;

  if(state==0)
  {
    pCONT_serial_pos_log->sync_frame_data.flag_started = true;

    /**
     * Flag to reset the I2S ADC buffers
     * */
    extern_flag_swap_ringbuffers_before_writting_is2_data = true;
          
    pCONT_serial_pos_log->rxon_counter = 0;
    pCONT_adc_internal->adcSampler1->item_id_counter=0;
  //   // Serial.printf("RESET rxon_counter=%d\n\r",pCONT_serial_pos_log->rxon_counter);
      
  //   /**
  //    * Swap between ringbuffers for internal-ADC
  //    * */
  //   // pCONT_adc_internal->SwapReaderWritersRingbuffers();    

  //   // #ifdef ENABLE_ESP32_ADC_SAMPLING
  //   // /**
  //   //  * toggle to the other buffer to be writting into, the read will check which is not active
  //   //  * */
  //   // pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index ^= 1; // Reset ADC syncframe index
  //   // /**
  //   //  * Use new buffer set here, to reset its counter
  //   //  * The other buffer not being written into, the counter values here wont be reset until the next ISR, allowing it to be checked for length of data on previous frame
  //   //  * */
  //   // pCONT_adc_internal->isr_capture.within_buffer_iter_counter = 0;
  //   // #endif // ENABLE_ESP32_ADC_SAMPLING

  }
  else
  {
    pCONT_serial_pos_log->sync_frame_data.flag_ended = true;
    // Serial.println("ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger HIGH HERE");
  }
}
#endif // ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD






















int8_t mSerialPositionalLogger::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;

  // DEBUG_PIN1_TOGGLE();
  
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
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;  
    case TASK_EVERY_SECOND: 
      EverySecond();

      // ALOG_DBG(PSTR(D_LOG_APPLICATION "FreeRam %d"), ESP.getFreeHeap());

    break;
    case TASK_EVERY_MINUTE:
      UpdateInternalRTCTimeWithGPSTime();
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_DRIVERS_SDCARD
      pCONT_sdcard->CommandSet_SDCard_Appending_File_Method_State(2);
      #endif
      // sequence_test = 0;
      pCONT_gps->sequence_test_global = 0;
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


void mSerialPositionalLogger::parse_JSONCommand(JsonParserObject obj)
{

}

void mSerialPositionalLogger::UpdateInternalRTCTimeWithGPSTime()
{

  
    // tkr_time->SetUTCTime(
    //   2021,//pCONT_gps->gps_result_stored.dateTime.year,
    //   9,//pCONT_gps->gps_result_stored.dateTime.month,
    //   19,//pCONT_gps->gps_result_stored.dateTime.day,
    //   14,//pCONT_gps->gps_result_stored.dateTime.hours,
    //   23,//pCONT_gps->gps_result_stored.dateTime.minutes,
    //   46//pCONT_gps->gps_result_stored.dateTime.seconds
    // );



  /**
   * Only update the time IF the sdcard is not open already and being written to
   * */
  if(pCONT_sdcard->sdcard_status.isopened==0)
  {

    tkr_time->SetUTCTime(
      pCONT_gps->gps_result_stored.dateTime.year,
      pCONT_gps->gps_result_stored.dateTime.month,
      pCONT_gps->gps_result_stored.dateTime.day,
      pCONT_gps->gps_result_stored.dateTime.hours,
      pCONT_gps->gps_result_stored.dateTime.minutes,
      pCONT_gps->gps_result_stored.dateTime.seconds
    );

  }

}

void mSerialPositionalLogger::Pre_Init(void)
{
  // if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  // {
    settings.fEnableSensor = true;
  // }



/**
 * Pin "sync_frame_status" on falling edge signifies the start of a new frame being sent, while, rising edge is the completion of the data.
 * Rising edge should occur approx 2.5ms after falling, and will set a flag to begin formation of sync_frame sample data to be sent to SDCARD stream
 * The amount of data to be read from RSS_STREAM will be known as it ends with two 0xFF 0xFF EOF bytes.
 * */
  // #ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  //   pinMode(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM, INPUT_PULLUP);
  //   attachInterrupt(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM, ISR_External_Pin_Sync_Frame_Status_Event_Trigger, FALLING);
  // #endif // ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  
  #ifdef ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
    pinMode(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, INPUT_PULLUP );
    // attachInterrupt(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger_LOW, FALLING);
    attachInterrupt(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger, CHANGE);
  #endif // ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD



// PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM
}


void mSerialPositionalLogger::Init(void)
{

  WiFi.mode(WIFI_OFF);
  btStop();
  
  // Create mutex
  // mutex_filling_sd_writer = xSemaphoreCreateMutex();


  /**
   * Only on debug lipo-esp32, for simplifying the data to pc process
   * */
  #if defined(USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT) && !defined(USE_MODULE_DRIVERS_SERIAL_UART)
    // Configure here for debugging only
    Serial2.begin(2048000);
  #endif

}


void mSerialPositionalLogger::EveryLoop()
{
    // DEBUG_PIN3_SET(0);

//     if(
//     pCONT_serial_pos_log->sync_frame_data.flag_started)
//     {
    
//     pCONT_serial_pos_log->sync_frame_data.flag_started = false;

//     Serial.println("ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger");
    
// }
    // = true;

    //Serial.println("ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger");

    // pinMode(34, INPUT);

//     if(digitalRead(26)==0)
//     {
// Serial.println("if(digitalRead(26)) low");
// delay(500);
//     }else
//     {
// Serial.println("ELSE if(digitalRead(34))");

    //Serial.printf("rxon_counter=%d\n\r",rxon_counter);
      
    // rxon_counter=0;
//     }

  #ifdef ENABLE_GPS_10MS_LOG_TEST1
  if(mTime::TimeReached(&tSaved_MillisWrite, 10))
  {
    //DEBUG_PIN2_SET(0);
    SubTask_HandleSDCardLogging();
    //DEBUG_PIN2_SET(1);
  }
    // DEBUG_PIN3_SET(1);/
  #endif // ENABLE_GPS_10MS_LOG_TEST1

  #ifndef ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN  // was not happening in timely manner, trying ISR forced
  Handle_Primary_Service_RSS_Stream_To_Create_SDCard_Stream();
  #endif

}


void mSerialPositionalLogger::Handle_Primary_Service_RSS_Stream_To_Create_SDCard_Stream()
{

  // if(pCONT_serial_pos_log->sync_frame_data.flag_started)
  // {
  //   pCONT_serial_pos_log->sync_frame_data.flag_started = false;
  //   // ALOG_INF(PSTR("sync_frame_data.flag_started"));
  // }
  
  // if(pCONT_serial_pos_log->sync_frame_data.flag_ended)
  // {
  //   pCONT_serial_pos_log->sync_frame_data.flag_ended = false;
  //   // ALOG_INF(PSTR("sync_frame_data.flag_ended"));
  //   SubTask_Generate_SyncFrame_To_SDCard_Stream();
  // }

}

void mSerialPositionalLogger::SubTask_Generate_SyncFrame_To_SDCard_Stream()
{
  #ifndef ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN  // was not happening in timely manner, trying ISR forced
    // #ifdef ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    // // for(
    //   int i=2;
    // //   i<3;i++)
    // // {
    //   BufferWriterI->Clear();
    //   uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(i, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
    //   if(bytes_in_line){
    //     ALOG_TST(PSTR("SDCardStream UART%d >> [%d]"), i, bytes_in_line);
    //     // ALOG_TST(PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
    //   }
    // // }
    // #endif

    /**
     * Warning: Assume flags and this next part of code happen fast enough (approx. 7.5ms or less) so no special char is needed, what is in the buffer should be the entire pic32_syncframe
     * */
    Construct_SuperFrame_Data_From_RingBuffer();

    memset(sync_frame_data.buffer, 0, sizeof(sync_frame_data.buffer));
    uint16_t maximum_sync_frame_length = 300;
    sync_frame_data.buffer_bytes_read = pCONT_uart->GetRingBufferDataAndClear(RSS_RINGBUFFER_NUMBER_INDEX, sync_frame_data.buffer, maximum_sync_frame_length);

    // if(sync_frame_data.buffer_bytes_read){
    //   ALOG_TST(PSTR("SDCardStream bytes_read >> [%d]"), sync_frame_data.buffer_bytes_read);
    //   // AddLog_Array_P(LOG_LEVEL_INFO, PSTR("sync_frame_data.buffer"), sync_frame_data.buffer, sync_frame_data.buffer_bytes_read);
    //   // ALOG_TST(PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
    // }

    /**
     * Get data that is superframe, and save as its own small buffer read from RSS ring, instead of global buffer
     * */
    ConstructJSON_SDCardSuperFrame();

    /**
     * Append to sdcard stream
     * */
    #ifdef USE_MODULE_DRIVERS_SDCARD
      pCONT_sdcard->AppendRingBuffer(BufferWriterI->GetPtr(), BufferWriterI->GetLength());
    #else
     // ALOG_TST(PSTR("SDCardStream UART%d >> [%d] \"%s\""), 2, BufferWriterI->GetLength(), BufferWriterI->GetPtr());
    #endif //USE_MODULE_DRIVERS_SDCARD

    /**
     * Only on debug lipo-esp32, for simplifying the data to pc process
     * */
    // #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
    //   char* buff_ptr = BufferWriterI->GetPtr();
    //   for(int i=0;i<BufferWriterI->GetLength();i++){
    //     Serial.print(buff_ptr[i]);
    //   }
    //   // For pretty output
    //   Serial.println();
    // #endif

  #endif// ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN  // was not happening in timely manner, trying ISR forced


}

void ISR_Direct_SubTask_Generate_SyncFrame_To_SDCard_Stream()
{
  
  // /**
  //  *  Construct_SuperFrame_Data_From_RingBuffer();
  //  *  RingBuffer2 is RSS buffer
  //  **/
  // size_t rss_data_read_size = 0;
  // char *rss_data_read = (char *)xRingbufferReceiveFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, &rss_data_read_size);

  // memset(isr_rss_buffer, 0, sizeof(isr_rss_buffer));
  // //Check received item
  // if (rss_data_read != NULL) {
  //   // Read from buffer
  //   memcpy(isr_rss_buffer,rss_data_read,rss_data_read_size);
  //   vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)rss_data_read); // Free memory
  // } 

  // /**
  //  *  ConstructJSON_SDCardSuperFrame
  //  **/
  // JBI->Start();
    
  // if(pCONT_serial_pos_log->sequence_test == 0){
  //   JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
  // }

  // JBI->Add("N", pCONT_serial_pos_log->sequence_test++);
  // JBI->Add("M",millis());

  // // GPS data
  // #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  // JBI->Object_Start("G");
  //   JBI->Add("la", pCONT_gps->gps_result_stored.latitudeL()); 
  //   JBI->Add("lg", pCONT_gps->gps_result_stored.longitudeL()); 
  //   JBI->Add("at", pCONT_gps->gps_result_stored.altitude_cm()); // above mean sea level, in cm 
  //   JBI->Add("sd", pCONT_gps->gps_result_stored.speed());   // nautical miles per hour
  //   JBI->Add("hd", pCONT_gps->gps_result_stored.heading_cd()); // degrees
  //   JBI->Add("gh", pCONT_gps->gps_result_stored.geoidHeight_cm()); // Height of the geoid above the WGS84 ellipsoid
  //   JBI->Object_Start("Gt");
  //     JBI->Add("h", pCONT_gps->gps_result_stored.dateTime.hours);
  //     JBI->Add("m", pCONT_gps->gps_result_stored.dateTime.minutes);
  //     JBI->Add("s", pCONT_gps->gps_result_stored.dateTime.seconds);
  //     JBI->Add("i", pCONT_gps->gps_result_stored.dateTime_ms());
  //   JBI->Object_End();
  // JBI->Object_End();
  // #endif // USE_MODULE_SENSORS_GPS_SERIAL
  
  // BufferWriterI->Append(",\"SF\":[");
  // for(int i=0;i<rss_data_read_size;i++){
  //   BufferWriterI->Append_P(PSTR("%d%s"), isr_rss_buffer[i], i<rss_data_read_size-1? ",": ""); 
  // }
  // BufferWriterI->Append("]");

  // JBI->End();

  char* test = "test write";

  /**
   * Write all data with ISR safe method into sd output ringbuffer
   * */
  BaseType_t dummyval;
  // xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, BufferWriterI->GetPtr(), BufferWriterI->GetLength(), &dummyval);
  xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, test, 10, &dummyval);

  // SD data to be written
  pCONT_sdcard->stream.flag_data_waiting = true; // easier than checking for bytes each time

}



/**
 * Check ringbuffer for max size needed to get a superframe, then check for the index where the 0xFF 0xFF end-of-frame exists
 * Read it again, giving only this amount of data, return that space to the ringbuffer
 * */
void mSerialPositionalLogger::Construct_SuperFrame_Data_From_RingBuffer()
{

  #ifdef USE_MODULE_DRIVERS_SERIAL_UART

  memset(sync_frame_data.buffer, 0, sizeof(sync_frame_data.buffer));
  uint16_t maximum_sync_frame_length = 300;
  sync_frame_data.buffer_bytes_read = pCONT_uart->GetSingleItemFromNoSplitRingBuffer(RSS_RINGBUFFER_NUMBER_INDEX, sync_frame_data.buffer, maximum_sync_frame_length);

  
  if(sync_frame_data.buffer_bytes_read){
  //  ALOG_TST(PSTR("SDCardStream bytes_read >> [%d]"), sync_frame_data.buffer_bytes_read);
    // AddLog_Array_P(LOG_LEVEL_INFO, PSTR("sync_frame_data.buffer"), sync_frame_data.buffer, sync_frame_data.buffer_bytes_read);
    // ALOG_TST(PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
  }

  #endif // USE_MODULE_DRIVERS_SERIAL_UART




}




//Use gps time to set rtc time?

void mSerialPositionalLogger::EverySecond()
{

  SubTask_UpdateOLED();

  // SubTask_HandleSDCardLogging();

  // SubTask_Debug_BasicFileWriteTest();


// ConstructJSON_SDCardSuperFrame();
//   ALOG_TST(PSTR("SuperFrame=\"%s\""), JBI->GetBufferPtr());


  

}



// /**
//  * @brief Test1: Button will open the file, then each time it is called, append the current RTC time.
//  * Button will then close the file again. 
//  * This method will try writting it directly, but future method may need a ringbuffer just for the sdcard stream... "BufferSDCARDStream-> print to it"
//  * */
// void mSerialPositionalLogger::SubTask_HandleSDCardLogging()
// {
//   #ifdef USE_MODULE_DRIVERS_SDCARD

//   // #ifdef ENABLE_SDLOGGER_APPEND_SUPERFRAME
//   // if(sdcard_status.isopened)
//   // {

//   //   ConstructJSON_SDCardSuperFrame();
//   //   ALOG_TST(PSTR("sdcardframe=\"%s\""), BufferWriterI->GetPtr());

//   //   pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());

//   // }
//   // #endif // ENABLE_SDLOGGER_APPEND_SUPERFRAME

//   // #ifdef ENABLE_SDLOGGER_APPEND_TIME_TEST
//   // if(pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID)
//   // {

//   //   ConstructJSON_SDCardSuperFrame();
    
//   //   // if(mTime::TimeReached(&tSaved_MillisWrite2, 1000))
//   //   // {
//   //   //   ALOG_TST(PSTR("sdcardframe[%d]=\"%s\""), pCONT_sdcard->sdcard_status.bytes_written_to_file, BufferWriterI->GetPtr());
//   //   // }

//   //   pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());

//   // }
//   // #endif // ENABLE_SDLOGGER_APPEND_TIME_TEST


//   #ifdef ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
//   if(pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID)
//   {

//     ConstructJSON_SDCardSuperFrame();
    
//     // if(mTime::TimeReached(&tSaved_MillisWrite2, 1000))
//     // {
//     //   ALOG_TST(PSTR("sdcardframe[%d]=\"%s\""), pCONT_sdcard->sdcard_status.bytes_written_to_file, BufferWriterI->GetPtr());
//     // }

//     // pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());
//     /**
//      * Append to sdcard stream
//      * */
//     pCONT_sdcard->AppendRingBuffer(BufferWriterI->GetPtr(), BufferWriterI->GetLength());
    
//   }
//   #endif // ENABLE_SDLOGGER_APPEND_TIME_TEST

//   #endif // USE_MODULE_DRIVERS_SDCARD

// }


// /**
//  * @brief basic test, that fully opens, appends and closed the file automatically on call
//  * */
// void mSerialPositionalLogger::SubTask_Debug_BasicFileWriteTest()
// {

//   #ifdef USE_MODULE_DRIVERS_SDCARD
//   // As a test, lets open, write/append and close sd card
  

//   // Open file
//   sprintf(pCONT_sdcard->writer_settings.file_name, "/%s%d.txt", "SDCardTest",1);
//   File file = SD.open(pCONT_sdcard->writer_settings.file_name, FILE_APPEND);
//   if(!file){
//     ALOG_TST(PSTR("file \"%s\" did not open"),pCONT_sdcard->writer_settings.file_name);
//   }
//   ALOG_TST(PSTR("file \"%s\" Opened!"),pCONT_sdcard->writer_settings.file_name);

//   ConstructJSON_SDCardSuperFrame();
//   ALOG_TST(PSTR("sdcardframe=\"%s\""), BufferWriterI->GetPtr());
//   // write all bytes
//   char* buffer_to_write = BufferWriterI->GetPtr();
//   for(int i=0; i<strlen(BufferWriterI->GetPtr()); i++)
//   {
//     file.print(buffer_to_write[i]);
//   }
//   file.print('\n');

//   //close file
//   file.close();

//   #endif// USE_MODULE_DRIVERS_SDCARD

// }

/**
 * @brief Show logger status on OLED
 * @note 4x11 chars at font 1
 * 
 * "LAT LON SQ"  LAT and LONG of gps (Without decimel) and S=Satellite count and Q=[V]alid or [E]rror
 * "Open xxxxxx" where x is the time opened
 * "Byte bbbbbb" where b is the number of bytes written (changing to kB when >1000)
 * "HHMMSS     " where HHMMSS is the GPS RTC time, which is also appended to the opened file name
 * */
void mSerialPositionalLogger::SubTask_UpdateOLED()
{

#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  char line_ctr[15] = {0};
  char buffer[15] = {0};

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  /**
   * Row 1
   * GPS data
   * 
   * */
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  float latitude = pCONT_gps->gps_result_stored.latitudeL();
  uint32_t latitude_U32 = (uint32_t)latitude;
  uint8_t latitude_num_digits = mSupport::NumDigits(latitude_U32);
  uint32_t latitude_three_largest_chars = 0;
  if(latitude_num_digits>3){
    latitude_three_largest_chars = latitude_U32;
    for(int i=0;i<latitude_num_digits-3;i++)
    {
      latitude_three_largest_chars /= 10;
    }
  }else{
    latitude_three_largest_chars = latitude_U32;
  }

  float longitude = pCONT_gps->gps_result_stored.longitudeL();
  uint32_t longitude_U32 = (uint32_t)fabs(longitude);
  uint8_t longitude_num_digits = mSupport::NumDigits(longitude_U32);
  uint32_t longitude_three_largest_chars = 0;
  if(longitude_num_digits>3){
    longitude_three_largest_chars = longitude_U32;
    for(int i=0;i<longitude_num_digits-3;i++)
    {
      longitude_three_largest_chars /= 10;
    }
  }else{
    longitude_three_largest_chars = longitude_U32;
  }

  // ALOG_TST(PSTR("SubTask_UpdateOLED %d %d %d"), latitude_U32, latitude_num_digits, latitude_three_largest_chars);

  char quality[4] = {0};
  //if valid, then simply show valid, if not, show satellites
  if(pCONT_gps->gps_result_stored.valid.location)
  {
    snprintf(quality, sizeof(quality), "%s", "Va");  
  }else{
    // if(pCONT_gps->gps_result_stored.satellites<10)
    // {
      snprintf(quality, sizeof(quality), "%02d", pCONT_gps->gps_result_stored.satellites);  
    // }else{
    //   snprintf(quality, sizeof(quality), " 9+");        
    // }
  }

  //https://cdn-shop.adafruit.com/datasheets/PMTK_A08.pdf

  snprintf(line_ctr, sizeof(line_ctr), "%03d %03d %s%s",
    latitude_three_largest_chars, 
    longitude_three_largest_chars,
    quality,
    pCONT_gps->flag_incoming_data_at_correct_runtime_baud?"T":"F"
    // pCONT_gps->gps_result_stored.satellites,
    // pCONT_gps->gps_result_stored.valid.location?'V':'E' //this needs updating regardless or V/A in RMC, to include timeouts without new updates
  );
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  #endif // USE_MODULE_SENSORS_GPS_SERIAL

  //Test display messages
  //[1234567890123456]
  //[Op f123456] // Op/Cd for open and closed file, f# where # is GPS time for file name (although also using random in case gps is not working ie millis+gpsUTC)
  //[Val 123456] // Val Err for GPS fix, showing UTC time
  //[ ] //Also show lat/long so I know its working
  //[] packets received on serialRSS in thousands

  // DEBUG_PIN2_TOGGLE();

  #ifdef USE_MODULE_DRIVERS_SDCARD
  snprintf(buffer, sizeof(buffer), "%c%s%s",
    pCONT_sdcard->sdcard_status.init_error_on_boot ? 'E' : 'f',
    pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID ?"OPEN!":"cd",
    &pCONT_sdcard->writer_settings.file_name[8] //skipping "APPEND_" to get just time
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  #endif //USE_MODULE_DRIVERS_SDCARD
  
  #ifdef USE_MODULE_DRIVERS_SDCARD
  uint32_t bytes_written = pCONT_sdcard->sdcard_status.bytes_written_to_file;
  char unit_type = 'B';

  if(bytes_written>50000)
  {
    bytes_written /= 1000; //into kB
    unit_type = 'k';
  }

  snprintf(buffer, sizeof(buffer), "%d %c",bytes_written,unit_type);
  
  // ,     
  // /**
  //  * show one byte from rss feed
  //  * */  
  // );

  pCONT_iDisp->LogBuffer_AddRow(buffer,2);
  #endif // USE_MODULE_DRIVERS_SDCARD

  snprintf(buffer, sizeof(buffer), "T:%s",tkr_time->RtcTime.hhmmss_ctr);//tkr_time->GEt DT_UTC;
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mSerialPositionalLogger::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mSerialPositionalLogger::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, 0);
  return JBI->End();
    
}


/**
 * @brief Used to generate the output to be written to the SD card
 * @note
 * {"SF":[array RSS superframe ie 50 samples... CC_state, RSS2, RRS5.... for 50],
 *  "GPS":{ minimal data, with 2 byte names}
 * }
 * */
uint8_t mSerialPositionalLogger::ConstructJSON_SDCardSuperFrame(uint8_t json_level, bool json_appending){

  JBI->Start();
    
  /**
   * on first sequence number, send additional useful info
   * */
  if(sequence_test == 0)
  {
    JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
    // JBI->Add("NodeNum", DEVICENUM_NUM);
  }

  JBI->Add("N", sequence_test++);

  uint32_t elapsed_millis_from_last_construct = millis() - tSaved_Constructed_Json_SDCardSuperFrame_ms;
  tSaved_Constructed_Json_SDCardSuperFrame_ms = millis();

  // Debug data only
  JBI->Add("M",millis());
  // JBI->Add("D",elapsed_millis_from_last_construct);

  // GPS data
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  JBI->Object_Start("G");
    JBI->Add("la", pCONT_gps->gps_result_stored.latitudeL()); 
    JBI->Add("lg", pCONT_gps->gps_result_stored.longitudeL()); 
    JBI->Add("at", pCONT_gps->gps_result_stored.altitude_cm()); // above mean sea level, in cm 
    JBI->Add("sd", pCONT_gps->gps_result_stored.speed());   // nautical miles per hour
    JBI->Add("hd", pCONT_gps->gps_result_stored.heading_cd()); // degrees
    JBI->Add("gh", pCONT_gps->gps_result_stored.geoidHeight_cm()); // Height of the geoid above the WGS84 ellipsoid
    JBI->Add("s",  pCONT_gps->gps_result_stored.satellites); // Number of satellites used to calculate a fix.

    // uint32_t timeofday_seconds = 
    //   (pCONT_gps->gps_result_stored.dateTime.hours*3600) +
    //   (pCONT_gps->gps_result_stored.dateTime.minutes*60) +
    //   (pCONT_gps->gps_result_stored.dateTime.seconds);

    // uint32_t tod_millis = 
    //   (timeofday_seconds*1000) + 
    //   pCONT_gps->gps_result_stored.dateTime_ms();

    // JBI->Add("tms",  tod_millis);

    JBI->Object_Start("Gt");
      JBI->Add("h", pCONT_gps->gps_result_stored.dateTime.hours);
      JBI->Add("m", pCONT_gps->gps_result_stored.dateTime.minutes);
      JBI->Add("s", pCONT_gps->gps_result_stored.dateTime.seconds);
      JBI->Add("i", pCONT_gps->gps_result_stored.dateTime_ms());
      JBI->Add("u", pCONT_gps->gps_result_stored.dateTime_us());
    JBI->Object_End();




    // JBI->Add_FV("t",  "%02d:%02d:%02d-%03d", pCONT_gps->gps_result_stored.dateTime.hours, pCONT_gps->gps_result_stored.dateTime.minutes, pCONT_gps->gps_result_stored.dateTime.seconds, pCONT_gps->gps_result_stored.dateTime_ms());
  JBI->Object_End();
  #endif // USE_MODULE_SENSORS_GPS_SERIAL


  // /**
  //  * add esp32 rss data
  //  * */ 
  // #ifdef ENABLE_ESP32_ADC_SAMPLING
  // uint8_t read_index = pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index?0:1;
  // JBI->Array_Start("e2");
  // for(int i=0;i<40;i++)
  // {
  //   JBI->Add(pCONT_adc_internal->isr_capture.adc_readings[read_index].buffer_ch6[i]);
  // }
  // JBI->Array_End();

  // JBI->Array_Start("e5");
  // for(int i=0;i<40;i++)
  // {
  //   JBI->Add(pCONT_adc_internal->isr_capture.adc_readings[read_index].buffer_ch7[i]);
  // }
  // JBI->Array_End();
  // #endif // ENABLE_ESP32_ADC_SAMPLING

  
  BufferWriterI->Append(",\"SF\":[");
  for(int i=0;i<sync_frame_data.buffer_bytes_read;i++)
  {
    BufferWriterI->Append_P(PSTR("%d%s"), sync_frame_data.buffer[i], i<sync_frame_data.buffer_bytes_read-1? ",": ""); 
  }
  BufferWriterI->Append("]");

  /**
   * Test, add only first sample from each esp32_adc item
   * */
  #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
  JBI->Object_Start("EA");
    pCONT_adc_internal->Append_JSONPart_ESP32ADCReadings();
  JBI->Object_End();
  #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER


  return JBI->End();
    
}


/******************************************************************************************************************
 * mSerialLogger_PinInterrupts.cpp
*******************************************************************************************************************/



/******************************************************************************************************************
 * mSerialLogger_Commands.cpp
*******************************************************************************************************************/


// void mRelays::parse_JSONCommand(JsonParserObject obj){

//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;
//   JsonParserObject jobj = 0;
 
//   uint8_t relay_id= 0,state=-1;    //assume index 0 if none given

//   if(jtok = obj[PM_POWERNAME]){
//     if(jtok.isStr()){
//       relay_id = GetRelayIDbyName(jtok.getStr());
//     }else 
//     if(jtok.isNum()){
//       relay_id  = jtok.getInt();
//     }
//   }

//   // Primary method since v0.86.14.21
//   if(jtok = obj[PM_POWER_STATE]){
//     if(jtok.isStr()){
//       state = pCONT_sup->GetStateNumber(jtok.getStr());
//     }else 
//     if(jtok.isNum()){
//       state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
//     }

// 		//state needs checked for flipped
// 		// if(state == 2){

// 		// }

//   }

//   // PHASE OUT by version 0.87
//   if(jtok = obj[PM_ONOFF]){
//     if(jtok.isStr()){
//       state = pCONT_sup->GetStateNumber(jtok.getStr());
//     }else 
//     if(jtok.isNum()){
//       state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
//     }
//   }


  


//   if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON]){
//     CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
//   }else
//   if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON_SECS]){
//     CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
//   }

//   if(IsWithinRange(state, 0,10) && IsWithinRange(relay_id, 0,settings.relays_connected)){
//     CommandSet_Relay_Power(state,relay_id);
//   }


// 	//move this so its passed as object, hence "EnabledTime" can be search for by (no number and hence use powername) or "EnabledTime#" #=1,2,3 ... how does tas pull the numbers out? must match key partially

// 	// Search for match which uses relayindex
// 	if(jtok = obj["RelayEnabled"]){
// 		SubCommandSet_EnabledTime(jtok.getObject());
// 	}

//   char rule_name[20] = {0};
//   for(uint8_t ii=0;ii<3;ii++){
//     sprintf(rule_name, "RelayEnabled%d", ii);
      
// 		if(jtok = obj[rule_name]){
// 			ALOG_INF(PSTR("MATCHED %s"),rule_name);
// 			SubCommandSet_EnabledTime(jtok.getObject(), ii);
// 		}

//   }  

  


//   // if(jtok = obj["EnabledTime"]){
//   //   time_short_t ontime;
//   //   time_short_t offtime;
//   //   uint8_t index = 0;
    
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_INDEX]){
//   //     index = jtok.getInt();    
//   //   }
    
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_ONTIME]){
//   //     ontime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
//   //     relay_status[relay_id].enabled_ranges[index].ontime = ontime;
//   //   }
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_OFFTIME]){
//   //     offtime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
//   //     relay_status[relay_id].enabled_ranges[index].offtime = offtime;
//   //   }
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_ENABLED]){
//   //     relay_status[relay_id].enabled_ranges[index].enabled = jtok.getInt();
//   //   }


//   //   tkr_time->PrintDateTime(ontime);
//   //   tkr_time->PrintDateTime(offtime);


//   // }


//   mqtthandler_scheduled_teleperiod.flags.SendNow = true;

// }




void mSerialPositionalLogger::CommandSet_SDCard_OpenClose_Toggle()
{//uint16_t time_secs, uint8_t relay_id){


  logger_status.enable ^= 1;

  // sdcard_status.isopened ^= 1;
  // sdcard_status.enable_logging ^= 1;
  // sdcard_status.isopened = sdcard_status.enable_logging; //this should not be linked

  // // If closed, start logging and begin sdcard opening sequence
  // if(sdcard_status.isopened)

  ALOG_TST(PSTR("mSerialPositionalLogger CommandSet_SDCard_OpenClose_Toggle %d"),logger_status.isopened);


  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

}



void mSerialPositionalLogger::CommandSet_LoggingState(uint8_t state)
{//uint16_t time_secs, uint8_t relay_id){

  if(state == 2) //toggle
  {
    logger_status.enable ^= 1; 
    ALOG_TST(PSTR("CommandSet_LoggingState state == 2, %d"),logger_status.enable);

  }else
  {
    logger_status.enable = state;
    ALOG_TST(PSTR("CommandSet_LoggingState sdcard_status.enable_logging = state,  %d"),logger_status.enable);

  }



  // sdcard_status.isopened ^= 1;
  // sdcard_status.enable_logging ^= 1;
  // sdcard_status.isopened = sdcard_status.enable_logging; //this should not be linked

  // // If closed, start logging and begin sdcard opening sequence
  // if(sdcard_status.isopened)

  ALOG_TST(PSTR("CommandSet_LoggingState %d"),logger_status.enable);


  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

}



/******************************************************************************************************************
 * mSerialPositionalLogger_MQTT.cpp
*******************************************************************************************************************/

  #ifdef USE_MODULE_NETWORK_MQTT
void mSerialPositionalLogger::MQTTHandler_Init(){

  struct handler<mSerialPositionalLogger>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_Sensor;

  ptr = &mqtthandler_sdcard_superframe;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SDCARD_SUPERFRAME_CTR;
  ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_SDCardSuperFrame;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSerialPositionalLogger::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSerialPositionalLogger::MQTTHandler_Rate()
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
void mSerialPositionalLogger::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

  #endif // USE_MODULE_NETWORK_MQTT

#endif
