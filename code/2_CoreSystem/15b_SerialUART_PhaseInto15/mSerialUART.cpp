// #include "mSerialUART.h"

// /**
//  * All Serial buffers will store in ringbuffers, then set a flag that it is ready. Other modules will then "pop" off some of the 
//  * data from the buffers and free the memory.
//  * 
//  * 





// Consider moving this to "CoreSystem" and other "I2C" etc so they are preconfigured before drivers/sensors need them





//  * */

// #ifdef USE_MODULE_CORE_SERIAL_UART


//   uint8_t simple_uart2_receive_frame_for_calibration[4] = {0};
//   bool flag_simple_uart2_receive_frame_for_calibration_updated = false;

// int8_t mSerialUART::Tasker(uint8_t function, JsonParserObject obj){

//   /************
//    * INIT SECTION * 
//   *******************/
//   switch(function){
//     case TASK_PRE_INIT: 
//       Pre_Init_Pins(); 
//     break;
//     case TASK_INIT: 
//       Init(); 
//     break;
// //     /**
// //      * Special case, I want to loop and init this when another modules says
// //      * */
// //     case TASK_LOOP:

// //       #ifndef USE_MODULE_SENSORS_GPS_SERIAL
// //         // Finsihed with manual control, start ISRs
// //         pCONT_uart->flag_init_buffers_and_start_isrs = true;
// //       #endif
    
// //      if(flag_init_buffers_and_start_isrs && !settings.fEnableModule)
// //      {       
// //       StartISR_RingBuffers(); 
// //      }
     
// //     #ifndef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

// // This might be causing delays when not needed in measurements
     
// // //temp dump data so it doesnt fill

// //       size_t rss_data_read_size = 0;
// //       char *rss_data_read = (char *)xRingbufferReceive(pCONT_uart->settings.uart2.ringbuffer_handle, &rss_data_read_size, pdMS_TO_TICKS(1000));
// //       if (rss_data_read != NULL) { // Read from buffer
// //         //memcpy(isr_rss_buffer,rss_data_read,rss_data_read_size);
// //         // for(int i=0;i<rss_data_read_size;i++){
// //         //   BufferWriterI->Append_P(PSTR("%d%s"), rss_data_read[i], i<rss_data_read_size-1? ",": ""); 
// //         // }
// //         vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)rss_data_read); // Free memory
// //       }
// //       #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

// //     // case TASK_UPTIME_10_SECONDS: 
// //     // case TASK_UPTIME_30_SECONDS: 
// //     //   StartISR_RingBuffers(); 
// //     break;
//   }

//   // Only continue in to tasker if module was configured properly
//   if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }


// // static const char *TX_TASK_TAG = "TX_TASK";
// // sendData(TX_TASK_TAG, TX_TASK_TAG);


//   switch(function){
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//    case TASK_LOOP:{

//       // BufferWriterI->Clear();
//       // uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(2, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
//       // if(BufferWriterI->GetLength()){
//       //   ALOG_TST(PSTR("GPS UART%d >> [%d] \"%s\""), 2, bytes_in_line, BufferWriterI->GetPtr());
//       // }


//     // #ifdef ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     // // for(
//     //   int i=2;
//     // //   i<3;i++)
//     // // {
//     //   BufferWriterI->Clear();
//     //   uint16_t bytes_in_line = GetRingBufferDataAndClear(i, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
//     //   if(bytes_in_line){
//     //     ALOG_TST(PSTR("UART%d >> [%d]"), i, bytes_in_line);
//     //     // ALOG_TST(PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
//     //   }
//     // // }
//     // #endif

//       // char buffer2[500] = {0};
//       // uint16_t bytes_in_line2 = GetRingBufferDataAndClear(2, buffer2, sizeof(buffer2), '\n', false);

//       // if(buffer2[0]){
//       //   ALOG_TST(PSTR("UART2 >> [%d] \"%s\""), bytes_in_line2, buffer2);
//       // }
//    }
//    break;
//    case TASK_EVERY_SECOND:{


// // // for(
// //       int i=2;
// //     //   i<3;i++)
// //     // {
// //       BufferWriterI->Clear();
// //       uint16_t bytes_in_line = GetRingBufferDataAndClear(i, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
// //       if(bytes_in_line){//strlen(BufferWriterI->GetPtr())){
// //         ALOG_TST(PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
// //       }else{
// //         ALOG_TST(PSTR("UART%d >> EMPTY"));//, i, bytes_in_line, BufferWriterI->GetPtr());

// //       }
// //     // }
// //     // #endif


//     // ALOG_TST(PSTR("[%d] \"%s\""),pCONT_uart->urxlen2,(char*)pCONT_uart->rxbuf2);

//     // Serial.println(pCONT_uart->urxlen2);
//     // for(int i=0;i<10;i++){   Serial.print((char)pCONT_uart->rxbuf2[i]); }
//     // Serial.println();
//       // ALOG_INF(PSTR("buffU2=%d"),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
  
//       // xRingbufferPrintInfo(settings.uart2.ringbuffer_handle);
//       // //Receive an item from no-split ring buffer
//       // size_t item_size;
//       // char*  item = (char *)xRingbufferReceive(pCONT_uart->settings.uart2.ringbuffer_handle, &item_size, pdMS_TO_TICKS(1000));

//       // //Check received item
//       // if (item != NULL) {
//       //   Serial.printf("UART2:>%d %d sizeB=%d\n\r",item_size,millis(),xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
//       //   //Print item
//       //   for(int i = 0; i < item_size; i++) {
//       //     Serial.printf("%c", item[i]);
//       //   }
//       //   //Return Item
//       //   vRingbufferReturnItem(pCONT_uart->settings.uart2.ringbuffer_handle, (void *)item); // Free memory
//       //   Serial.printf("\n\r sizeA=%d\n\r",xRingbufferGetCurFreeSize(pCONT_uart->settings.uart2.ringbuffer_handle));
//       // } else {
//       //   //Failed to receive item
//       //   Serial.printf("Failed to receive item\n");
//       // }

//       #ifdef ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER


//       #endif // ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER


//     }
//     break;
//     case TASK_EVERY_MINUTE: {


//     }
//     break;
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     case TASK_JSON_COMMAND_ID:
//       parse_JSONCommand(obj);
//     break;
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_MQTT
//     case TASK_MQTT_HANDLERS_INIT:
//       MQTTHandler_Init();
//     break;
//     case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
//       MQTTHandler_Rate();
//     break;
//     case TASK_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     case TASK_MQTT_CONNECTED:
//       MQTTHandler_RefreshAll();
//     break;
//     #endif //USE_MODULE_NETWORK_MQTT
//   }
  
//   /************
//    * WEBPAGE SECTION * 
//   *******************/
  
//   #ifdef USE_MODULE_NETWORK_WEBSERVER
//   return Tasker_Web(function);
//   #endif // USE_MODULE_NETWORK_WEBSERVER

// } // END Tasker


// void mSerialUART::Pre_Init_Pins()
// {

//   #ifdef ENABLE_HARDWARE_UART_0
//   if(pCONT_pins->PinUsed(GPIO_HWSERIAL0_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL0_RING_BUFFER_RX_ID)) {
//     settings.uart0.receive_interrupts_enable = true;
//     settings.uart0.baud = 115200;
//     settings.uart0.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL0_RING_BUFFER_TX_ID);
//     settings.uart0.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL0_RING_BUFFER_RX_ID);
//     // init_UART1_pins();
//     ALOG_INF(PSTR("UART0 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart0.gpio.tx, settings.uart0.gpio.rx);
//   }else{
//     settings.uart0.receive_interrupts_enable = false;
//   }
//   #endif // ENABLE_HARDWARE_UART_0
  
//   #ifdef ENABLE_HARDWARE_UART_1
//   if(pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL1_RING_BUFFER_RX_ID)) {
//     settings.uart1.receive_interrupts_enable = true;
//     settings.uart1.baud = HARDWARE_UART_1_BAUD_RATE_SPEED;
//     settings.uart1.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_TX_ID);
//     settings.uart1.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL1_RING_BUFFER_RX_ID);
//     // init_UART1_pins();
//     ALOG_INF(PSTR("UART1 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart1.gpio.tx, settings.uart1.gpio.rx);
//   }else{
//     settings.uart1.receive_interrupts_enable = false;
//   }
//   #endif // ENABLE_HARDWARE_UART_1

//   #ifdef ENABLE_HARDWARE_UART_2
//   if(pCONT_pins->PinUsed(GPIO_HWSERIAL2_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL2_RX_ID)) {
//     settings.uart2.receive_interrupts_enable = true;
//     settings.uart2.baud = HARDWARE_UART_2_BAUD_RATE_SPEED;
//     settings.uart2.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL2_TX_ID);
//     settings.uart2.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RX_ID);
//     settings.uart2.configured = true;
//     // init_UART2_pins();
//     ALOG_INF(PSTR("UART2 pins: TX[%d] RX[%d]"),settings.uart2.gpio.tx, settings.uart2.gpio.rx);

    

//   }else{
//     settings.uart2.receive_interrupts_enable = false;
//   }

//   if(pCONT_pins->PinUsed(GPIO_HWSERIAL2_RING_BUFFER_TX_ID)&&pCONT_pins->PinUsed(GPIO_HWSERIAL2_RING_BUFFER_RX_ID)) {
//     settings.uart2.receive_interrupts_enable = true;
//     settings.uart2.baud = HARDWARE_UART_2_BAUD_RATE_SPEED;
//     settings.uart2.gpio.tx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RING_BUFFER_TX_ID);
//     settings.uart2.gpio.rx = pCONT_pins->GetPin(GPIO_HWSERIAL2_RING_BUFFER_RX_ID);
//     // init_UART2_pins();
//     ALOG_INF(PSTR("UART2 RingBuffer Interrupts pins: TX[%d] RX[%d]"),settings.uart2.gpio.tx, settings.uart2.gpio.rx);
//   }else{
//     settings.uart2.receive_interrupts_enable = false;
//   }
//   #endif // ENABLE_HARDWARE_UART_2

// }


// void mSerialUART::Init(void)
// {

//   // #ifdef ENABLE_HARDWARE_UART_1
//   // if(settings.uart1.receive_interrupts_enable)
//   // {
//   //   init_UART1_RingBuffer();
//   //   init_UART1_ISR();
//   // }
//   // #endif // ENABLE_HARDWARE_UART_1

//   // #ifdef ENABLE_HARDWARE_UART_2
//   // if(settings.uart2.receive_interrupts_enable)
//   // {
//   //   init_UART2_RingBuffer();
//   //   init_UART2_ISR();
//   // }
//   // #endif // 
  

//   #ifdef ENABLE_HARDWARE_UART_2
//   if(settings.uart2.configured) {

//     DEBUG_LINE_HERE;

//     HWSerial2 = new HardwareSerial(2);

//     HWSerial2->begin(
//       settings.uart2.baud, 
//       SERIAL_8N1, 
//       settings.uart2.gpio.rx, 
//       settings.uart2.gpio.tx
//     );

//     DEBUG_LINE_HERE;

//     ALOG_INF(PSTR("mSerialUART::Init %d %d %d"),
//       pCONT_uart->settings.uart2.baud, 
//       pCONT_uart->settings.uart2.gpio.rx, 
//       pCONT_uart->settings.uart2.gpio.tx
//     );


//     settings.uart2.initialised = true;
//     // init_UART2_pins();
//     ALOG_INF(PSTR("UART2 pins: TX[%d] RX[%d]"),settings.uart2.gpio.tx, settings.uart2.gpio.rx);

    

//   }
//   #endif


  
// }


// HardwareSerial* mSerialUART::GetSerial(uint8_t index)
// {
//   switch(index)
//   {
//     default:
//     case 0: return &Serial;
//     case 1: return &Serial1;
//     case 2: return &Serial2;
//   }
// }


// void mSerialUART::StartISR_RingBuffers()
// {

//   ALOG_TST(PSTR("delayed start of uart interrupts for methods which need basic serial.read"));

//   #ifdef ENABLE_HARDWARE_UART_1
//   if(settings.uart1.receive_interrupts_enable)
//   {
//     init_UART1_pins();
//     init_UART1_RingBuffer();
//     init_UART1_ISR();
//     settings.uart1.initialised = true;
//     settings.fEnableModule = true;
//   }
//   #endif // ENABLE_HARDWARE_UART_1

// DEBUG_LINE_HERE;
//   #ifdef ENABLE_HARDWARE_UART_2
//   if(settings.uart2.receive_interrupts_enable)
//   {
//     // settings.uart2.ring_buffer_size_rx = 
//     init_UART2_pins();
//     init_UART2_RingBuffer();
//     init_UART2_ISR();
//     settings.uart2.initialised = true;
//     settings.fEnableModule = true;
//   }
//   #endif // ENABLE_HARDWARE_UART_2


// DEBUG_LINE_HERE;

// }

// int mSerialUART::sendData(const char* logName, const char* data)
// {
//   const int len = strlen(data);
//   const int txBytes = uart_write_bytes(UART_NUM_2, data, len);
//   ESP_LOGI(logName, "Wrote %d bytes", txBytes);
//   return txBytes;
// }


// /******************************************************************************************************************************************************************************************************************************************
// ******************************************************************************************************************************************************************************************************************************************
// ****** Shared ************************************************************************************************************************************************************************************************************************************
// ******************************************************************************************************************************************************************************************************************************************
// ******************************************************************************************************************************************************************************************************************************************/

// /**
//  * Universal to all UARTs, get RingBuffer
//  * Returns: Bytes read from buffer and thus freed
//  * Arg:     Buffer to write into and its size, or simply, how much to read at a time
//  * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
//  * @return item_size number of bytes read from buffer
//  * */
// uint16_t mSerialUART::GetRingBufferDataAndClear(uint8_t uart_num, char* buffer, uint16_t buflen, char optional_read_until_char, bool flag_clear_buffer_after_read)
// {

//   // RingbufHandle_t handle_tmp;
//   // ringbuf_type_t ringbuffer_format_type_tmp;
//   // switch(uart_num)
//   // {
//   //   default:
//   //   case 0: 
//   //     if(!settings.uart0.initialised)
//   //     {
//   //       return 0;
//   //     }
//   //     handle_tmp = settings.uart0.ringbuffer_handle;
//   //   break;
//   //   case 1:
//   //     if(!settings.uart1.initialised)
//   //     {
//   //       return 0;
//   //     } 
//   //     handle_tmp = settings.uart1.ringbuffer_handle;
//   //   break;
//   //   case 2: 
//   //     if(!settings.uart2.initialised)
//   //     {
//   //       return 0;
//   //     }
//   //     handle_tmp = settings.uart2.ringbuffer_handle;
//   //   break;
//   // }

//   // //delayed start check
//   // // if(handle_tmp == nullptr)
//   // // {
//   // //   return 0; // not ready to continue
//   // // }
  
//   // // Receive an item from no-split ring buffer
//   // size_t item_size = 0;
//   // // Wait at most 1ms to read from buffer, read a maximum number of bytes
//   // char*  item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);


//   RingbufHandle_t handle_tmp;
//   ringbuf_type_t ringbuffer_format_type_tmp;
//   switch(uart_num)
//   {
//     default:
//   #ifdef ENABLE_HARDWARE_UART_0
//     case 0: 
//       if(!settings.uart0.initialised)
//       {
//         return 0;
//       }
//       handle_tmp = settings.uart0.ringbuffer_handle;
//       ringbuffer_format_type_tmp = settings.uart0.ringbuffer_format_type;
//     break;
//   #endif // ENABLE_HARDWARE_UART_0
//   #ifdef ENABLE_HARDWARE_UART_1
//     case 1:
//       if(!settings.uart1.initialised)
//       {
//         return 0;
//       } 
//       handle_tmp = settings.uart1.ringbuffer_handle;
//       ringbuffer_format_type_tmp = settings.uart1.ringbuffer_format_type;
//     break;
//   #endif // ENABLE_HARDWARE_UART_1
//   #ifdef ENABLE_HARDWARE_UART_2
//     case 2: 
//       if(!settings.uart2.initialised)
//       {
//         return 0;
//       }
//       handle_tmp = settings.uart2.ringbuffer_handle;
//       ringbuffer_format_type_tmp = settings.uart2.ringbuffer_format_type;
//     break;
//   #endif // ENABLE_HARDWARE_UART_2
//   }

//   //delayed start check
//   // if(handle_tmp == nullptr)
//   // {
//   //   return 0; // not ready to continue
//   // }
  
//   // Receive an item from no-split ring buffer
//   size_t item_size = 0;
//   // Wait at most 1ms to read from buffer, read a maximum number of bytes

// char *item = nullptr;

// switch(ringbuffer_format_type_tmp)
// {
//   case RINGBUF_TYPE_BYTEBUF:
//     item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);

//   break;
//   case RINGBUF_TYPE_NOSPLIT:
//     item = (char *)xRingbufferReceive(handle_tmp, &item_size, pdMS_TO_TICKS(1000));


//   break;


// }


//   //Check received item
//   if (item != NULL) {
//     // Read from buffer
//     memcpy(buffer,item,item_size);

//     //ALOG_TST(PSTR("xRingbufferReceiveUpTo=\"%s\""),buffer);

//     // if(flag_clear_buffer_after_read)
//     // {
//       //Return Item
//       vRingbufferReturnItem(handle_tmp, (void *)item); // Free memory
//     // }
//   } 

//   return item_size;

// }

// // /**
// //  * Universal to all UARTs, get RingBuffer
// //  * Returns: Bytes read from buffer and thus freed
// //  * Arg:     Buffer to write into and its size, or simply, how much to read at a time
// //  * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
// //  * @return item_size number of bytes read from buffer
// //  * */
// // uint16_t mSerialUART::GetRingBufferDataAndClearUntilSpecialDelimeter(uint8_t uart_num, char* buffer, uint16_t buflen)
// // {

// //   RingbufHandle_t handle_tmp;
// //   switch(uart_num)
// //   {
// //     default:
// //     case 0: 
// //       if(!settings.uart0.initialised)
// //       {
// //         return 0;
// //       }
// //       handle_tmp = settings.uart0.ringbuffer_handle;
// //     break;
// //     case 1:
// //       if(!settings.uart1.initialised)
// //       {
// //         return 0;
// //       } 
// //       handle_tmp = settings.uart1.ringbuffer_handle;
// //     break;
// //     case 2: 
// //       if(!settings.uart2.initialised)
// //       {
// //         return 0;
// //       }
// //       handle_tmp = settings.uart2.ringbuffer_handle;
// //     break;
// //   }

// // //   //delayed start check
// // //   // if(handle_tmp == nullptr)
// // //   // {
// // //   //   return 0; // not ready to continue
// // //   // }
// // //   uint16_t index_of_end_of_frame = 0;
// // //   bool flag_found_eof = false;

// // //   /**
// // //    * First read, with amount greater than expected until next frame 0xFF 0xFF
// // //    * */
// // //   // Receive an item from no-split ring buffer
// // //   size_t item1_size = 0;
// // //   // Wait at most 1ms to read from buffer, read a maximum number of bytes
// // //   char*  item1 = (char *)xRingbufferReceiveUpTo(handle_tmp, &item1_size, pdMS_TO_TICKS(100), buflen);



// // //   /**
// // //    * Read, then test this memory for the special char, and record the index (ie max length I need to read on next receiveupto)
// // //    * */
// // //   //Check received item
// // //   if (item1 != NULL) {
// // //     // Read from buffer
// // //     memcpy(buffer,item1,item1_size);


// // //   for(int i=0;i<item1_size;i++)
// // //   {
// // //     Serial.print(buffer[i]);
// // //     Serial.print('-');
// // //   }
// // //     Serial.println();
// // //     Serial.println();


// // //     for(int i=0;i<item1_size;i++)
// // //     {
// // //       //first byte
// // //       if(buffer[i] = 0xFF)
// // //       {
// // //         if(buffer[i+1] = 0xFF)
// // //         {
// // //           index_of_end_of_frame = i+1;
// // //           break;
// // //         }
// // //       }
// // //     }
// // //   }

// // //   /**
// // //    *  Read again, but only until the EOF ends
// // //    * */
// // //   if(flag_found_eof)
// // //   {
// // //     buflen = index_of_end_of_frame;
// // //   }

// // // */

  
// //   // Receive an item from no-split ring buffer
// //   size_t item_size = 0;



// //   // Wait at most 1ms to read from buffer, read a maximum number of bytes
// //   char*  item = nullptr;
  
  
// //   item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);

// //   //Check received item
// //   if (item != NULL) {
// //     // Read from buffer
// //     memcpy(buffer,item,item_size);

// //     ALOG_TST(PSTR("xRingbufferReceiveUpTo=\"%s\""),buffer);

// //     // if(flag_clear_buffer_after_read)
// //     // {
// //       //Return Item
// //       vRingbufferReturnItem(handle_tmp, (void *)item); // Free memory
// //     // }
// //   } 

// //   return item_size;

// // }



// /**
//  * Universal to all UARTs, get RingBuffer
//  * Returns: Bytes read from buffer and thus freed
//  * Arg:     Buffer to write into and its size, or simply, how much to read at a time
//  * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
//  * @return item_size number of bytes read from buffer
//  * */
// uint16_t mSerialUART::GetSingleItemFromNoSplitRingBuffer(uint8_t uart_num, char* buffer, uint16_t buflen)
// {

//   RingbufHandle_t handle_tmp;
//   ringbuf_type_t ringbuffer_format_type_tmp;
//   switch(uart_num)
//   {
//     default:
//     case 0: 
//   #ifdef ENABLE_HARDWARE_UART_0
//       if(!settings.uart0.initialised)
//       {
//         return 0;
//       }
//       handle_tmp = settings.uart0.ringbuffer_handle;
//       ringbuffer_format_type_tmp = settings.uart0.ringbuffer_format_type;
//     break;
//   #endif // ENABLE_HARDWARE_UART_0
//   #ifdef ENABLE_HARDWARE_UART_1
//     case 1:
//       if(!settings.uart1.initialised)
//       {
//         return 0;
//       } 
//       handle_tmp = settings.uart1.ringbuffer_handle;
//       ringbuffer_format_type_tmp = settings.uart1.ringbuffer_format_type;
//     break;
//   #endif // ENABLE_HARDWARE_UART_1
//   #ifdef ENABLE_HARDWARE_UART_2
//     case 2: 
//       if(!settings.uart2.initialised)
//       {
//         return 0;
//       }
//       handle_tmp = settings.uart2.ringbuffer_handle;
//       ringbuffer_format_type_tmp = settings.uart2.ringbuffer_format_type;
//     break;
//   #endif // ENABLE_HARDWARE_UART_2
//   }

//   //delayed start check
//   // if(handle_tmp == nullptr)
//   // {
//   //   return 0; // not ready to continue
//   // }
  
//   // Receive an item from no-split ring buffer
//   size_t item_size = 0;
//   // Wait at most 1ms to read from buffer, read a maximum number of bytes

// char *item = nullptr;

// switch(ringbuffer_format_type_tmp)
// {
//   case RINGBUF_TYPE_BYTEBUF:
//     item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);

//   break;
//   case RINGBUF_TYPE_NOSPLIT:
//     item = (char *)xRingbufferReceive(handle_tmp, &item_size, pdMS_TO_TICKS(1000));


//   break;


// }

//   // char*  item = (char *)xRingbufferReceiveUpTo(handle_tmp, &item_size, pdMS_TO_TICKS(100), buflen);
//   // char *item = (char *)xRingbufferReceive(handle_tmp, &item_size, pdMS_TO_TICKS(1000));

//   //Check received item
//   if (item != NULL) {
//     // Read from buffer
//     memcpy(buffer,item,item_size);

//     //ALOG_TST(PSTR("xRingbufferReceiveUpTo=\"%s\""),buffer);

//     // if(flag_clear_buffer_after_read)
//     // {
//       //Return Item
//       vRingbufferReturnItem(handle_tmp, (void *)item); // Free memory
//     // }
//   } 

//   return item_size;

// }


  
// /******************************************************************************************************************
//  * mSerial_UART1.cpp
// *******************************************************************************************************************/


// #ifdef ENABLE_HARDWARE_UART_1

// void mSerialUART::init_UART1_RingBuffer()
// {

//   settings.uart1.ringbuffer_handle = xRingbufferCreate(settings.uart1.ring_buffer_size_rx, RINGBUF_TYPE_BYTEBUF);
//   if (settings.uart1.ringbuffer_handle != NULL) {
//     settings.uart1.initialised = true;
//   }

  

//   char data_ctr[] = "UART1 RingBuf Init\0";
//   UBaseType_t res =  xRingbufferSend(settings.uart1.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
//   if (res != pdTRUE) {
//     AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
//     settings.uart1.initialised = false; //disable if false
//   }else{

//     AddLog(LOG_LEVEL_ERROR, PSTR("%s SUCCESS"),data_ctr);


//   }
// // assert(settings.uart1.ringbuffer_handle != NULL);
  

// }

// void mSerialUART::init_UART1_pins()
// {

//   /* Configure parameters of an UART driver, communication pins and install the driver */
//   uart_config_t uart_config = {
//     .baud_rate = settings.uart1.baud,
//     .data_bits = UART_DATA_8_BITS,
//     .parity = UART_PARITY_DISABLE,
//     .stop_bits = UART_STOP_BITS_1,
//     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
//   };

//   ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
//   //Set UART gpio
// 	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, settings.uart1.gpio.tx, settings.uart1.gpio.rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

// }

// /**
//  * @brief init UART1 ISR routine
//  * @note
//  * @param void none
//  * @return none
//  */
// void mSerialUART::init_UART1_ISR(){

//   ALOG_DBG(PSTR("init_UART1_ISR Starting..."));

//   // If serial2 has already been activated by a library, disable it first so the new driver can attach
//   Serial1.end();  

//   init_UART1_pins(); //reassert new baud

//   // Install UART driver, and get the queue.
//   ESP_ERROR_CHECK(
//     uart_driver_install(
//       UART_NUM_1,                           // UART Number
//       settings.uart1.ring_buffer_size_rx,   // UART RX ring buffer size.
//       settings.uart1.ring_buffer_size_tx,   // UART TX ring buffer size. If set to zero, driver will not use TX buffer, TX function will block task until all data have been sent out.
//       settings.uart1.event_queue_size,      // UART event queue size/depth.
//      &settings.uart1.event_queue_handle,    // UART event queue handle (out param). On success, a new queue handle is written here to provide access to UART events. If set to NULL, driver will not use an event queue.
//       0                                     // intr_alloc_flags (esp_intr_alloc.h)
//     )
//   );

// 	// // release the pre registered UART handler/subroutine
// 	ESP_ERROR_CHECK(
//     uart_isr_free(UART_NUM_1)       // Free UART interrupt handler registered by uart_isr_register. Must be called on the same core as uart_isr_register was called.
//   ); 

//   uart_isr_register(            // UART ISR handler will be attached to the same CPU core that this function is running on.
//     UART_NUM_1,                 // UART_NUM_0, UART_NUM_1 or UART_NUM
//     UART1_ISR_Static, // Interrupt handler function.
//     NULL,                       // parameter for handler function
//     ESP_INTR_FLAG_IRAM,         // Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info. 
//     uart1_handle_console        // Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.
//   );

//   ALOG_DBG(PSTR(DEBUG_INSERT_PAGE_BREAK "init_UART1_ISR Started %d buffer size"),settings.uart1.ring_buffer_size_rx);

//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_pad_select_gpio(BLINK_GPIO);
//   gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
//   gpio_set_level(BLINK_GPIO, LOW); // Turn off by default
//   #endif
// // esp_log_level_set("*", ESP_LOG_VERBOSE);
// // esp_log_level_set(TAG, ESP_LOG_INFO);


// // std::cout << "x" << std::flush;
  
// //     ESP_LOGI(TAG, "Initializing SD card");

// }


// #define UART1_ISR_TMP_BUFFER_SIZE 5000
// // Receive buffer to collect incoming data
// uint8_t rxbuf1[UART1_ISR_TMP_BUFFER_SIZE] = {0};
// // Register to collect data length
// uint16_t urxlen1 = 0;

// /**
//  * @brief Define UART interrupt subroutine to ackowledge interrupt
//  * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
//  **/
// void IRAM_ATTR UART1_ISR_Static(void *arg)
// { 

// // gps data

//  // DEBUG_PIN1_SET(LOW);
  
//   uint16_t rx_fifo_len, status;
//   uint16_t i = 0;

//   status = UART1.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART1.status.rxfifo_cnt; // read number of bytes in UART buffer

//   while(UART1.status.rxfifo_cnt)
//   {
//     rxbuf1[i++] = UART1.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp31s1 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))
//     if(i>=UART1_ISR_TMP_BUFFER_SIZE-1)
//     {
//       break;
//     }
//     rx_fifo_len--;
//   }
//   urxlen1 = i;

//   BaseType_t dummyval;
//   UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart1.ringbuffer_handle, rxbuf1, urxlen1, &dummyval);
  
//   // after reading bytes from buffer clear UART interrupt status
//   uart_clear_intr_status(UART_NUM_1, 
//     UART_RXFIFO_TOUT_INT_CLR |
//     // UART_BRK_DET_INT_CLR |
//     // UART_CTS_CHG_INT_CLR |
//     // UART_DSR_CHG_INT_CLR |
//     // UART_RXFIFO_OVF_INT_CLR |
//     UART_FRM_ERR_INT_CLR | // UART Frame Error
//     // UART_PARITY_ERR_INT_CLR |
//     // UART_TXFIFO_EMPTY_INT_CLR |
//     UART_RXFIFO_FULL_INT_CLR 
//   );


//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//   #endif

//  // DEBUG_PIN1_SET(HIGH);
// }


// #endif // ENABLE_HARDWARE_UART_1

  
// /******************************************************************************************************************
//  * mSerial_UART2.cpp
// *******************************************************************************************************************/

  

// #ifdef ENABLE_HARDWARE_UART_2

// char uart2_last_byte = 0x00;
// uint16_t uart2_byte_index = 0;


// #define UART2_ISR_TMP_BUFFER_SIZE 500
// // Receive buffer to collect incoming data
// uint8_t rxbuf2[UART2_ISR_TMP_BUFFER_SIZE] = {0};
// // Register to collect data length
// uint16_t urxlen2 = 0;

//   // uint8_t uart_rss_tmp_buf_writer_index = 0;
//   // char uart_rss_tmp_buffer_0[300] = {0};
//   // uint16_t uart_rss_tmp_buflen_0 = 0;
//   // char uart_rss_tmp_buffer_1[300] = {0};
//   // uint16_t uart_rss_tmp_buflen_1 = 0;
//   // char uart_rss_tmp_buffer_2[300] = {0};
//   // uint16_t uart_rss_tmp_buflen_2 = 0;
//     //  char special_json_part_of_gps_buffer[300]  = {0};
//     //  uint16_t special_json_part_of_gps_buflen = 0;



// void mSerialUART::init_UART2_RingBuffer()
// {

//   #ifdef ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   settings.uart2.ringbuffer_format_type = RINGBUF_TYPE_NOSPLIT;
//   #else
//   settings.uart2.ringbuffer_format_type = RINGBUF_TYPE_BYTEBUF;
//   #endif

//   // Serial.printf("heap size1 = %d\n\r", ESP.getHeapSize()); Serial.flush();
//   DEBUG_LINE_HERE;

//   settings.uart2.ringbuffer_handle = xRingbufferCreate(settings.uart2.ring_buffer_size_rx, settings.uart2.ringbuffer_format_type);
//   if (settings.uart2.ringbuffer_handle != NULL) {
//     settings.uart2.initialised = true;
//   // Serial.printf("settings.uart2.initialised = true;\n\r"); Serial.flush();
//   }else{
    
//   // Serial.printf("settings.uart2.initialised = false;\n\r"); Serial.flush();
//   }

//   if(settings.uart2.ringbuffer_handle == NULL)
//   {
//   // Serial.printf("settings.uart2.ringbuffer_handle == NULL\n\r"); Serial.flush();

//   }

//   // Serial.printf("heap size2 = %d\n\r", ESP.getHeapSize()); Serial.flush();
//   DEBUG_LINE_HERE;
//   // char data_ctr[] = "UART2 RingBuf Init\0";
//   // UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
//   // if (res != pdTRUE) {
//   //   AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
//   //   settings.uart2.initialised = false; //disable if false
//   // }

  
//   #ifdef ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  
  
//   char data_ctr[] = "UART2 RingBuf Init\0";
//   UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
//   if (res != pdTRUE) {
//     AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
//     settings.uart2.initialised = false; //disable if false
//   }else{
    
//     ALOG_TST(PSTR("%s SUCCESS"),data_ctr);
//   }

//   assert(settings.uart2.ringbuffer_handle != NULL);



//   #else
  
//     char data_ctr[] = "UART2 RingBuf Init\0";
//   UBaseType_t res =  xRingbufferSend(settings.uart2.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
//   if (res != pdTRUE) {
//     AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
//     settings.uart2.initialised = false; //disable if false
//   }


//   #endif




// }


// /**
//  * split from other ISR init so pins can be configured without attaching the interrupt
//  * */
// void mSerialUART::init_UART2_pins()
// {

//   /* Configure parameters of an UART driver, communication pins and install the driver */
//   uart_config_t uart_config = {
//     .baud_rate = settings.uart2.baud,
//     .data_bits = UART_DATA_8_BITS,
//     .parity = UART_PARITY_DISABLE,
//     .stop_bits = UART_STOP_BITS_1,
//     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
//   };

//   ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));
//   //Set UART gpio
// 	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, settings.uart2.gpio.tx, settings.uart2.gpio.rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

//   ESP_ERROR_CHECK(uart_set_baudrate(UART_NUM_2, settings.uart2.baud));

// }



// /**
//  * @brief init UART2 ISR routine
//  * @note
//  * @param void none
//  * @return none
//  */
// void mSerialUART::init_UART2_ISR(){

//   ALOG_DBG(PSTR("init_UART2_ISR Starting..."));

//   // If serial2 has already been activated by a library, disable it first so the new driver can attach
//   Serial2.end();
//   // Serial2.begin(2048000);
  
//   init_UART2_pins(); //reassert new baud

//   // Install UART driver, and get the queue.
//   ESP_ERROR_CHECK(
//     uart_driver_install(
//       UART_NUM_2,                           // UART Number
//       settings.uart2.ring_buffer_size_rx,   // UART RX ring buffer size.
//       settings.uart2.ring_buffer_size_tx,   // UART TX ring buffer size. If set to zero, driver will not use TX buffer, TX function will block task until all data have been sent out.
//       settings.uart2.event_queue_size,      // UART event queue size/depth.
//      &settings.uart2.event_queue_handle,    // UART event queue handle (out param). On success, a new queue handle is written here to provide access to UART events. If set to NULL, driver will not use an event queue.
//       0                                     // intr_alloc_flags (esp_intr_alloc.h)
//     )
//   );

// 	// // release the pre registered UART handler/subroutine
// 	ESP_ERROR_CHECK(
//     uart_isr_free(UART_NUM_2)       // Free UART interrupt handler registered by uart_isr_register. Must be called on the same core as uart_isr_register was called.
//   ); 
  
//   uart_isr_register(            // UART ISR handler will be attached to the same CPU core that this function is running on.
//     UART_NUM_2,                 // UART_NUM_0, UART_NUM_1 or UART_NUM
//   // #ifdef ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//     // UART2_ISR_Static_NoSplitRingBuffer, // Interrupt handler function.
//     #ifndef USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1
//     UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements, // Interrupt handler function.
//     #else
//     UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements_Calibration, // Interrupt handler function.
//     #endif
//   // #else // ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   //   UART2_ISR_Static_ByteRingBuffer, // Interrupt handler function.
//   // #endif // ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//     NULL,                       // parameter for handler function
//     ESP_INTR_FLAG_IRAM,         // Flags used to allocate the interrupt. One or multiple (ORred) ESP_INTR_FLAG_* values. See esp_intr_alloc.h for more info. 
//     uart2_handle_console        // Pointer to return handle. If non-NULL, a handle for the interrupt will be returned here.
//   );
  
//   ALOG_DBG(PSTR(DEBUG_INSERT_PAGE_BREAK "init_UART2_ISR RSS Started %d buffer size"),settings.uart2.ring_buffer_size_rx);

//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_pad_select_gpio(BLINK_GPIO);
//   gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
//   gpio_set_level(BLINK_GPIO, LOW); // Turn off by default
//   #endif

  
//   // gpio_pad_select_gpio(GPIO_NUM_21);
//   // gpio_set_direction(GPIO_NUM_21, GPIO_MODE_OUTPUT);
//   // gpio_set_level(GPIO_NUM_21, LOW); // Turn off by default

// }



// // /**
// //  * @brief Define UART interrupt subroutine to ackowledge interrupt
// //  * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
// //  **/
// // void IRAM_ATTR UART2_ISR_Static_ByteRingBuffer(void *arg)
// // { 
// // //  DEBUG_PIN1_SET(LOW);
  
// //   uint16_t rx_fifo_len, status;
// //   uint16_t i = 0;

// //   status = UART2.int_st.val; // read UART interrupt Status
// //   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

// //   while(rx_fifo_len)
// //   {
// //     pCONT_uart->rxbuf2[i++] = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))
// //     if(i>=RINGBUFFER_HANDLE_2_LENGTH-1)
// //     {
// //       break;
// //     }
// //     rx_fifo_len--;
// //   }
// //   pCONT_uart->urxlen2 = i;

// //   BaseType_t dummyval;
// //   UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, pCONT_uart->rxbuf2, pCONT_uart->urxlen2, &dummyval);
  
// //   // after reading bytes from buffer clear UART interrupt status
// //   uart_clear_intr_status(UART_NUM_2, 
// //     UART_RXFIFO_TOUT_INT_CLR |
// //     // UART_BRK_DET_INT_CLR |
// //     // UART_CTS_CHG_INT_CLR |
// //     // UART_DSR_CHG_INT_CLR |
// //     // UART_RXFIFO_OVF_INT_CLR |
// //     UART_FRM_ERR_INT_CLR | // UART Frame Error
// //     // UART_PARITY_ERR_INT_CLR |
// //     // UART_TXFIFO_EMPTY_INT_CLR |
// //     UART_RXFIFO_FULL_INT_CLR 
// //   );


// //   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
// //   gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
// //   #endif

// //  // DEBUG_PIN1_SET(HIGH);
// // }

// // char conversion_buffer[500];
// // uint16_t conversion_buflen = 0;


// /**
//  * @brief Define UART interrupt subroutine to ackowledge interrupt
//  * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
//  * 
//  * Arriving data will be read (appended) to a global small buffer, each byte in will be checked. 
//  * If special EOF chars are found, then the data in the global buffer is commited to the ringbuffer nosplit, which allows retreiving one item at a time
//  * 
//  * 
//  * METHOD AUTOMATICALLY CREATES ARRAY FOR QUICK MOVE INTO JSON
//  * 
//  **/
// void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements(void *arg)
// { 
// //  DEBUG_PIN1_SET(LOW);
//   // gpio_set_level(GPIO_NUM_21, 0);
  
//   uint16_t rx_fifo_len, status;
//   // uint16_t i = 0;
//   char byte_read = 0;
//   bool flag_end_of_frame_detected = false;

//   status = UART2.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

//   while(rx_fifo_len)
//   {

//     byte_read = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))

//     rxbuf2[uart2_byte_index++] = byte_read;//(char)uart2_byte_index;//byte_read;
//     if(uart2_byte_index>=UART2_ISR_TMP_BUFFER_SIZE-1)
//     {
//       uart2_byte_index = 0; // forced reset
//       // We should never reach here, unless no EOF are detected then wrapping around will dump/lose all data
//       break;
//     }

//     /**
//      * Check if this byte and last byte are EOF chars
//      * */
//     if((byte_read==0xFF)&&(uart2_last_byte==0xFF))
//     {
//       flag_end_of_frame_detected = true;
//       break;
//     }

//     // rxbuf2[uart2_byte_index++] = ','; // Keep adding comma's only if needed

//     // Update last byte, with anticaption of being read later
//     uart2_last_byte = byte_read; //update with this recent byte

//     rx_fifo_len--;
//   }
//   urxlen2 = uart2_byte_index;

//   /**
//    * if EOF is detected, commit data into nosplit buffer
//    * */
//   if(flag_end_of_frame_detected)
//   {
//     #ifdef ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

//     // conversion_buflen = 0;
//     //   // conversion_buflen += sprintf(conversion_buffer+conversion_buflen, "%d:", millis());
//     // for(int i=0;i<urxlen2;i++)
//     // {
//     //   conversion_buflen += sprintf(conversion_buffer+conversion_buflen, "%d,", rxbuf2[i]);
//     // }
//     // conversion_buflen-- ; // forget last comma



// //     pCONT_uart->special_json_part_of_gps_buffer[0] = 253;
// //  pCONT_uart->special_json_part_of_gps_buflen = 1;
// // if(urxlen2){
//   BaseType_t dummyval;

//   // use start and end of array "~~" so matlab can search and repair the json
//       xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, "@{\"F\":[[A", 9, &dummyval);
//       xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, rxbuf2, urxlen2-2, &dummyval); //dont send two EOL bytes
//       // xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle, conversion_buffer, conversion_buflen, &dummyval);
//       // if(pCONT_uart->special_json_part_of_gps_buflen)
//       // {
//       //   xRingbufferSendFromISR(pCONT_sdcard->stream.ringbuffer_handle,  pCONT_uart->special_json_part_of_gps_buffer,  pCONT_uart->special_json_part_of_gps_buflen, &dummyval);
//       // }
//       // xRingbufferSend(pCONT_sdcard->stream.ringbuffer_handle, "]}\n\r", 4, pdMS_TO_TICKS(2));
// // }

//       // /**
//       //  * New method, must write to a few circler buffers that is meant for sdcard
//       //  * */
//       // switch(uart_rss_tmp_buf_writer_index)
//       // {
//       //   case 0:
//       //     // rxbuf2[0] = 2;
//       //     if(uart_rss_tmp_buflen_0 == 0) // check if its empty
//       //     {
//       //       uart_rss_tmp_buflen_0 = urxlen2;
//       //       memcpy(uart_rss_tmp_buffer_0, rxbuf2, urxlen2);
//       //       uart_rss_tmp_buf_writer_index = 1;
//       //     }
//       //   break;
//       //   case 1:
//       //     // rxbuf2[0] = 3;
//       //     if(uart_rss_tmp_buflen_1 == 0) // check if its empty
//       //     {
//       //       uart_rss_tmp_buflen_1 = urxlen2;
//       //       memcpy(uart_rss_tmp_buffer_1, rxbuf2, urxlen2);
//       //       uart_rss_tmp_buf_writer_index = 0;
//       //     }
//       //   break;
//       //   // case 2:
//       //   //   // rxbuf2[0] = 3;
//       //   //   uart_rss_tmp_buflen_2 = urxlen2;
//       //   //   memcpy(uart_rss_tmp_buffer_2, rxbuf2, urxlen2);
//       //   //   uart_rss_tmp_buf_writer_index = 0;
//       //   // break;
//       // }


//     #else
//     BaseType_t dummyval;
//     UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, rxbuf2, urxlen2, &dummyval); // instead of ringbuffer, this probably also needs to be cycling buffers
//     #endif // ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

//     /**
//      * Reset byte index for next frame
//      * */
//     uart2_byte_index = 0;
//   }
  
//   // after reading bytes from buffer clear UART interrupt status
//   uart_clear_intr_status(UART_NUM_2, 
//     UART_RXFIFO_TOUT_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_BRK_DET_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_CTS_CHG_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_DSR_CHG_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_RXFIFO_OVF_INT_CLR |
//     UART_FRM_ERR_INT_CLR | // UART Frame Error
//     //clearing more, maybe its the cause of crashes?
//     UART_PARITY_ERR_INT_CLR |
//     UART_TXFIFO_EMPTY_INT_CLR | // Needed to enable transmitting on serial
//     UART_RXFIFO_FULL_INT_CLR 
//   );


//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//   #endif
//   // gpio_set_level(GPIO_NUM_21, 1);

// //  DEBUG_PIN1_SET(HIGH);
// }


// #ifdef USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1
// /**
//  * @brief Define UART interrupt subroutine to ackowledge interrupt
//  * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
//  * 
//  * Arriving data will be read (appended) to a global small buffer, each byte in will be checked. 
//  * If special EOF chars are found, then the data in the global buffer is commited to the ringbuffer nosplit, which allows retreiving one item at a time
//  * 
//  * 
//  * METHOD AUTOMATICALLY CREATES ARRAY FOR QUICK MOVE INTO JSON
//  * 
//  **/
// void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer_ForMeasurements_Calibration(void *arg)
// { 
// //  DEBUG_PIN1_SET(LOW);
//   // gpio_set_level(GPIO_NUM_21, 0);
  
//   uint16_t rx_fifo_len, status;
//   // uint16_t i = 0;
//   char byte_read = 0;
//   bool flag_end_of_frame_detected = false;

//   status = UART2.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

//   while(rx_fifo_len)
//   {

//     byte_read = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))

//     rxbuf2[uart2_byte_index++] = byte_read;//(char)uart2_byte_index;//byte_read;
//     if(uart2_byte_index>=UART2_ISR_TMP_BUFFER_SIZE-1)
//     {
//       uart2_byte_index = 0; // forced reset
//       // We should never reach here, unless no EOF are detected then wrapping around will dump/lose all data
//       break;
//     }

//     /**
//      * Check if this byte and last byte are EOF chars
//      * */
//     if((byte_read==0xFF)&&(uart2_last_byte==0xFF))
//     {
//       flag_end_of_frame_detected = true;
//       break;
//     }

//     // rxbuf2[uart2_byte_index++] = ','; // Keep adding comma's only if needed

//     // Update last byte, with anticaption of being read later
//     uart2_last_byte = byte_read; //update with this recent byte

//     rx_fifo_len--;
//   }
//   urxlen2 = uart2_byte_index;

//   /**
//    * if EOF is detected, commit data into nosplit buffer
//    * */
//   if(flag_end_of_frame_detected)
//   {
//     // Only 4 bytes
//     simple_uart2_receive_frame_for_calibration[0] = rxbuf2[0];
//     simple_uart2_receive_frame_for_calibration[1] = rxbuf2[1];
//     simple_uart2_receive_frame_for_calibration[2] = rxbuf2[2];
//     simple_uart2_receive_frame_for_calibration[3] = rxbuf2[3];
//     flag_simple_uart2_receive_frame_for_calibration_updated = true;
//     /**
//      * Reset byte index for next frame
//      * */
//     uart2_byte_index = 0;
//   }
  
//   // after reading bytes from buffer clear UART interrupt status
//   uart_clear_intr_status(UART_NUM_2, 
//     UART_RXFIFO_TOUT_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_BRK_DET_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_CTS_CHG_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_DSR_CHG_INT_CLR |
//     //clearing more, maybe its the cause of crashes?
//     UART_RXFIFO_OVF_INT_CLR |
//     UART_FRM_ERR_INT_CLR | // UART Frame Error
//     //clearing more, maybe its the cause of crashes?
//     UART_PARITY_ERR_INT_CLR |
//     UART_TXFIFO_EMPTY_INT_CLR | // Needed to enable transmitting on serial
//     UART_RXFIFO_FULL_INT_CLR 
//   );


//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//   #endif
//   // gpio_set_level(GPIO_NUM_21, 1);

// //  DEBUG_PIN1_SET(HIGH);
// }

// #endif

// /**
//  * @brief Define UART interrupt subroutine to ackowledge interrupt
//  * @note As this function is static, variables used within it need to be referenced using their instances ie "pCONT"
//  * 
//  * Arriving data will be read (appended) to a global small buffer, each byte in will be checked. 
//  * If special EOF chars are found, then the data in the global buffer is commited to the ringbuffer nosplit, which allows retreiving one item at a time
//  * 
//  * 
//  **/
// void IRAM_ATTR UART2_ISR_Static_NoSplitRingBuffer(void *arg)
// { 
// //  DEBUG_PIN1_SET(LOW);
  
//   uint16_t rx_fifo_len, status;
//   // uint16_t i = 0;
//   char byte_read = 0;
//   bool flag_end_of_frame_detected = false;

//   status = UART2.int_st.val; // read UART interrupt Status
//   rx_fifo_len = UART2.status.rxfifo_cnt; // read number of bytes in UART buffer

//   while(rx_fifo_len)
//   {

//     byte_read = UART2.fifo.rw_byte; // You can not directly access the UART0.fifo.rw_byte on esp32s2 but have to use READ_PERI_REG(UART_FIFO_AHB_REG(0))

//     rxbuf2[uart2_byte_index++] = byte_read;
//     if(uart2_byte_index>=UART2_ISR_TMP_BUFFER_SIZE-1)
//     {
//       uart2_byte_index = 0; // forced reset
//       // We should never reach here, unless no EOF are detected then wrapping around will dump/lose all data
//       break;
//     }

//     /**
//      * Check if this byte and last byte are EOF chars
//      * */
//     if((byte_read==0xFF)&&(uart2_last_byte==0xFF))
//     {
//       flag_end_of_frame_detected = true;
//       break;
//     }

//     // Update last byte, with anticaption of being read later
//     uart2_last_byte = byte_read; //update with this recent byte

//     rx_fifo_len--;
//   }
//   urxlen2 = uart2_byte_index;

//   /**
//    * if EOF is detected, commit data into nosplit buffer
//    * */
//   if(flag_end_of_frame_detected)
//   {
//     #ifdef ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER


//       /**
//        * New method, must write to a few circler buffers that is meant for sdcard
//        * */
//       // switch(uart_rss_tmp_buf_writer_index)
//       // {
//       //   case 0:
//       //     uart_rss_tmp_buflen_0 = urxlen2;
//       //     memcpy(uart_rss_tmp_buffer_0, rxbuf2, urxlen2);
//       //     uart_rss_tmp_buf_writer_index = 1;
//       //   break;
//       //   case 1:
//       //     uart_rss_tmp_buflen_1 = urxlen2;
//       //     memcpy(uart_rss_tmp_buffer_1, rxbuf2, urxlen2);
//       //     uart_rss_tmp_buf_writer_index = 0;
//       //   break;
//       // }


//     #else
//     BaseType_t dummyval;
//     UBaseType_t res =  xRingbufferSendFromISR(pCONT_uart->settings.uart2.ringbuffer_handle, rxbuf2, urxlen2, &dummyval); // instead of ringbuffer, this probably also needs to be cycling buffers
//     #endif // ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

//     /**
//      * Reset byte index for next frame
//      * */
//     uart2_byte_index = 0;
//   }
  
//   // after reading bytes from buffer clear UART interrupt status
//   uart_clear_intr_status(UART_NUM_2, 
//     UART_RXFIFO_TOUT_INT_CLR |
//     // UART_BRK_DET_INT_CLR |
//     // UART_CTS_CHG_INT_CLR |
//     // UART_DSR_CHG_INT_CLR |
//     // UART_RXFIFO_OVF_INT_CLR |
//     UART_FRM_ERR_INT_CLR | // UART Frame Error
//     // UART_PARITY_ERR_INT_CLR |
//     UART_TXFIFO_EMPTY_INT_CLR | // Needed to enable transmitting on serial
//     UART_RXFIFO_FULL_INT_CLR 
//   );


//   #ifdef ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
//   gpio_set_level(BLINK_GPIO, !gpio_get_level(BLINK_GPIO));
//   #endif

// //  DEBUG_PIN1_SET(HIGH);
// }


// #endif // ENABLE_HARDWARE_UART_2


// /******************************************************************************************************************
//  * mSerialUART_Commands.cpp
// *******************************************************************************************************************/



// void mSerialUART::parse_JSONCommand(JsonParserObject obj){

  

// //   if(obj.containsKey(D_FANSPEED)){
// //     int speed = obj[D_FANSPEED];
// //     test_val = speed;
// //     if(speed>0){
// //       analogWrite(pin, test_val);
// //     }else
// //     if(speed==1023){
// //       digitalWrite(pin, HIGH);      
// //     }else{
// //       digitalWrite(pin, LOW);
// //     }
// //     // if(speed>3){
// //     //   Response_mP(S_JSON_COMMAND_SVALUE, D_FANSPEED,D_PARSING_NOMATCH);
// //     //   speed=0; //default off
// //     // }      
// //     // SetFanSpeed(speed, false);
// //     // AddLog(LOG_LEVEL_INFO,PSTR("GetFanspeed=%d"),GetFanspeed());
// //     ALOG_INF(PSTR(D_LOG_CEILINGFAN D_PARSING_MATCHED D_COMMAND_NVALUE),D_FANSPEED,speed);
// //     // Response_mP(S_JSON_COMMAND_NVALUE,D_FANSPEED,speed);
// //     // isserviced++;
// //   }
  
  

// }

// // #ifdef USE_MODULE_DRIVERS_SDCARD


// // int8_t mSerialUART::CheckAndExecute_JSONCommands(){

// //   // Check if instruction is for me
// //   if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CONTROLLER_FAN_FRIENDLY_CTR)>=0){
// //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_TOPIC_COMMAND D_MODULE_CONTROLLER_FAN_FRIENDLY_CTR));
// //     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //     parse_JSONCommand();
// //     return FUNCTION_RESULT_HANDLED_ID;
// //   }else{
// //     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
// //   }

// // }

// // void mSerialUART::parse_JSONCommand(void){

// //   char buffer[50];

// //   // Need to parse on a copy
// //   char parsing_buffer[data_buffer.payload.len+1];
// //   memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
// //   JsonParser parser(parsing_buffer);
// //   JsonParserObject obj = parser.getRootObject();   
// //   if (!obj) { 
// //     #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //     AddLog(LOG_LEVEL_ERROR, PSTR(D_DESERIALIZATION_ERROR));
// //     #endif //ENABLE_LOG_LEVEL_COMMANDS
// //     return;
// //   }  
// //   JsonParserToken jtok = 0; 
// //   int8_t tmp_id = 0;
    

// //   if(jtok = obj["ListDir"]){

// //     CommandSet_SerialPrint_FileNames(jtok.getStr());
    
// //   }
  

// //   if(jtok = obj["WriteFile"]){

// //     // Also check for datafile
// //     JsonParserToken jtok_data = obj["DataFile"];
// //     if(!jtok_data.isNull()){
// //       CommandSet_WriteFile(jtok.getStr(), jtok_data.getStr());
// //     }
// //     else{
// //       CommandSet_WriteFile(jtok.getStr());
// //     }


// //   //   if(jtok.isStr()){
// //   //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
// //   //       CommandSet_PaletteID(tmp_id);
// //   //       data_buffer.isserviced++;
// //   //     }
// //   //   }else
// //     // if(jtok.isNum()){
// //     //   CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
// //     //   set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
// //     //   set_fan_speed = jtok.getInt();
// //     //   data_buffer.isserviced++;
// //     // }
// //   //   #ifdef ENABLE_LOG_LEVEL_DEBUG
// //   //   ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
// //   //   #endif // ENABLE_LOG_LEVEL_DEBUG
// //   }
  
// //   if(jtok = obj["ReadFile"]){

// //     CommandSet_ReadFile(jtok.getStr());
    
// //   }
  
// //   mqtthandler_sensor_ifchanged.flags.SendNow = true;

// // }


// // /******************************************************************************************************************************
// // *******************************************************************************************************************************
// // ****************** CommandSet_ReadFile *****************************************************************************************
// // *******************************************************************************************************************************
// // *******************************************************************************************************************************/

// // void mSerialUART::CommandSet_ReadFile(const char* filename){

// //   readFile(SD_MMC, filename);

// //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ReadFile")), filename);
// //   #endif // ENABLE_LOG_LEVEL_COMMANDS

// // } 


// // /******************************************************************************************************************************
// // *******************************************************************************************************************************
// // ****************** CommandSet_WriteFile *****************************************************************************************
// // *******************************************************************************************************************************
// // *******************************************************************************************************************************/

// // void mSerialUART::CommandSet_WriteFile(const char* filename, const char* data){

// //   char filename_with_extention[50];
// //   sprintf(filename_with_extention, "%s.txt", filename);
// //   if(data == nullptr){
// //     writeFile(SD_MMC, filename_with_extention, "Empty File!");
// //   }else{
// //     writeFile(SD_MMC, filename_with_extention, "Hello ");
// //   }

// //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("WriteFile")), filename_with_extention);
// //   #endif // ENABLE_LOG_LEVEL_COMMANDS

// // } 


// // /******************************************************************************************************************************
// // *******************************************************************************************************************************
// // ****************** PixelHardwareType *****************************************************************************************
// // *******************************************************************************************************************************
// // *******************************************************************************************************************************/

// // void mSerialUART::CommandSet_CreateFile_WithName(char* value){

// // //   analogWrite(pin, value);
// //   createDir(SD_MMC, "/mydir");

// // // AddLog(LOG_LEVEL_DEV_TEST,PSTR("pwm %d value = %d"),pin,value);


// //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ListDir")), dirname);
// //   #endif // ENABLE_LOG_LEVEL_COMMANDS

// // } 

// // /******************************************************************************************************************************
// // *******************************************************************************************************************************
// // ****************** PixelHardwareType *****************************************************************************************
// // *******************************************************************************************************************************
// // *******************************************************************************************************************************/

// // void mSerialUART::CommandSet_SerialPrint_FileNames(const char* dirname){

// //   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("TESTListDir")), dirname);
// //   listDir(SD_MMC, dirname, 0);

// //   // listDir(SD_MMC, "/", 0);

// //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ListDir")), dirname);
// //   #endif // ENABLE_LOG_LEVEL_COMMANDS

// // } 

  
// /******************************************************************************************************************
//  * mSerialUART_MQTT.cpp
// *******************************************************************************************************************/



// uint8_t mSerialUART::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

//   char buffer[30];
  
//   JBI->Start();  

//     JBI->Add_P(PM_TIME, 1000);
//     // JBI->Add_P(PM_TIME_MS, animation.time_ms);
//   return JBI->End();

// }
// uint8_t mSerialUART::ConstructJSON_UARTInfo(uint8_t json_level, bool json_appending){

//   char buffer[30];
  
//   JBI->Start();  

//     JBI->Add_P(PM_TIME, 1000);

//     // JBI->Object_Start("UART1");
//     //   JBI->Add("receive_interrupts_enable", settings.uart1.receive_interrupts_enable);
      
//     //   JBI->Array_Start("Buffer1");
//     //     for(int i=0;i<100;i++)
//     //       JBI->Add(rxbuf1[i]);
//     //   JBI->Array_End();
      

//     // JBI->Object_End();

//     JBI->Object_Start("UART2");
//       JBI->Add("receive_interrupts_enable", settings.uart2.receive_interrupts_enable);
      
//       // JBI->Array_Start("Buffer1");
//       //   for(int i=0;i<100;i++)
//       //     JBI->Add(rxbuf2[i]);
//       // JBI->Array_End();
      

//     JBI->Object_End();

//     // JBI->Add_P(PM_TIME_MS, animation.time_ms);
//   return JBI->End();

// }


// ////////////////////// START OF MQTT /////////////////////////

// #ifdef USE_MODULE_NETWORK_MQTT

// /**
//  * @brief Init the mqtthandlers
//  * */
// void mSerialUART::MQTTHandler_Init(){

//   struct handler<mSerialUART>* ptr;

//   ptr = &mqtthandler_settings;
//   ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 1;
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_Settings;

//   ptr = &mqtthandler_uartinfo_teleperiod;
//   ptr->handler_id = MQTT_HANDLER_MODULE_UARTINFO_IFCHANGED_ID;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 1; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_UARTINFO_CTR;
//   ptr->ConstructJSON_function = &mSerialUART::ConstructJSON_UARTInfo;

// } 

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mSerialUART::MQTTHandler_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mSerialUART::MQTTHandler_Rate()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
//   }
// }

// /**
//  * @brief Check all handlers if they require action
//  * */
// void mSerialUART::MQTTHandler_Sender()
// {
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
//   }
// }
// #endif // USE_MODULE_NETWORK_MQTT


// #endif
