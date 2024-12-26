#include "mSDCard.h"

/***
 * Introduce a ringbuffer for sdcard writter, hence as long as the sdcard is open, anything written to buffer will be flushed to sdcard
 
 This needs phased out into filesystem
 The special parts that allow ringbuffer to SD fast write should be moved into a controller (using filesystem and UART drivers)
 
 * */


#ifdef USE_MODULE_DRIVERS_SDCARD


// static SemaphoreHandle_t mutex;


uint8_t extern_flag_sdcard_writer_status = 0;


SPIClass spiSD(HSPI);

// char json_second_section[1000];
// uint16_t jLen = 0;


#include "mSDCard.h"

// Low level functions for writing/reading from the SD Card


File stored_file;


#define BYTE_SIZE_GLOABL_STEAM_OUT_BUFFER 10000
char global_stream_out_buffer[BYTE_SIZE_GLOABL_STEAM_OUT_BUFFER];



/**
 * Part1: Take from circle buffer into ringbuffer when not writting
 * Part2: Write into sdcard from ringbuffer_sd 
 * */
void sdcardWriterTask(void *param)
{

  uint32_t tSaved = millis();
  bool flag_send_rest_of_frame = false;
  
  while (true)
  {
    vTaskDelay(1);
    
    // /**
    //  * Part1: Read from circle buffers ==> ringbuffer_stream
    //  * */
    // switch(stream_tmp_buf_writer_index)
    // {
    //   case 0:

    //     if(stream_tmp_buflen_1)
    //     {
    //       // for(int i=0;i<stream_tmp_buflen_1;i++)
    //       // {
    //       //   Serial.print(stream_tmp_buffer_1[i]);
    //       // }
    //       // Serial.println();
    //       // Serial.printf("buflen_\t\t1=%d\n\r", stream_tmp_buflen_1);

    //       xRingbufferSend(pCONT_sdcard->stream.ringbuffer_handle, stream_tmp_buffer_1, stream_tmp_buflen_1, pdMS_TO_TICKS(500));
    //       flag_send_rest_of_frame = true;

    //       // reset to empty
    //       stream_tmp_buflen_1 = 0;

    //     }

    //   break;
    //   case 1:

    //     if(stream_tmp_buflen_0)
    //     {
    //       // for(int i=0;i<stream_tmp_buflen_0;i++)
    //       // {
    //       //   Serial.print(stream_tmp_buffer_0[i]);
    //       // }
    //       // Serial.println();
    //       // Serial.printf("buflen_\t\t0=%d\n\r", stream_tmp_buflen_0);

    //       xRingbufferSend(pCONT_sdcard->stream.ringbuffer_handle, stream_tmp_buffer_0, stream_tmp_buflen_0, pdMS_TO_TICKS(500));
    //       flag_send_rest_of_frame = true;
    //       // reset to empty
    //       stream_tmp_buflen_0 = 0;
    //     }

    //   break;
    // } //end switch

    // if(flag_send_rest_of_frame){
    //   flag_send_rest_of_frame = false;
    //   // I need a special json version in here, another 1, as it will conflict with the main one. As its a singleton,
    //   // I need to make a duplicate of the entire class for just this use case (do it manually? yes)


    //   // JBI->Add("N", sequence_test_global++);
    //   // char* testm = "test}\n\r";
    //   jLen = 0;
      
    //   // jLen += sprintf(json_second_section+jLen, ",\"N\":%d,", sequence_test_global++);
    //   #ifdef USE_MODULE_SENSORS_GPS_SERIAL
    //   // // keyed method
    //   // // jLen += sprintf(json_second_section+jLen, ",\"G\":{");
    //   // //   jLen += sprintf(json_second_section+jLen, "\"t\":%d,", pCONT_gps->my_gps_vals.lat); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"n\":%d,", pCONT_gps->my_gps_vals.lon); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"a\":%d,", pCONT_gps->my_gps_vals.alt); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"d\":%d,", pCONT_gps->my_gps_vals.speed); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"h\":%d,", pCONT_gps->my_gps_vals.hours); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"m\":%d,", pCONT_gps->my_gps_vals.minutes); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"s\":%d,", pCONT_gps->my_gps_vals.seconds); 
    //   // //   jLen += sprintf(json_second_section+jLen, "\"i\":%d,", pCONT_gps->my_gps_vals.dateTime_ms/100); // are always hundreds, so shorted to 1 sig fig then recover on matlab
    //   // // jLen += sprintf(json_second_section+jLen, "}");
      
    //   // //array method
    //   // jLen += sprintf(json_second_section+jLen, ",\"G\":[");
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.lat); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.lon); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.alt); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.speed); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.hours); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.minutes); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d,", pCONT_gps->my_gps_vals.seconds); 
    //   //   jLen += sprintf(json_second_section+jLen, "%d", pCONT_gps->my_gps_vals.dateTime_ms/100); // are always hundreds, so shorted to 1 sig fig then recover on matlab
    //   // jLen += sprintf(json_second_section+jLen, "]");
    //   #endif // USE_MODULE_SENSORS_GPS_SERIAL

    //   // jLen += sprintf(json_second_section+jLen, "}");

    //   // // Send the rest of the frame
    //   // xRingbufferSend(pCONT_sdcard->stream.ringbuffer_handle, json_second_section, jLen, pdMS_TO_TICKS(500));

    // }


    
    yield();
    // delay(1);

    // /**
    //  * Part2: Write out from ringbuffer to card if opened
    //  * */    
    if(extern_flag_sdcard_writer_status == 2){

    // if(mTime::TimeReached(&tSaved, 5)
    //   // &&(extern_flag_sdcard_writer_status == 2)
    //   ){

        uint16_t chunks_to_write_with = BYTE_SIZE_GLOABL_STEAM_OUT_BUFFER;
        uint16_t bytes_read = pCONT_sdcard->GetRingBufferDataAndClear(0, global_stream_out_buffer, chunks_to_write_with, '\n', false);


        //if any data found
        if(bytes_read)
        {     
          // Serial.println(bytes_read);
          //Serial.printf("bytes_read\t\t\t\t\t\t=%d\n\r", bytes_read);

          for(int i=0; i<bytes_read; i++)
          {
            stored_file.write(global_stream_out_buffer[i]);
            // Serial.print(global_stream_out_buffer[i]);
          }
          //Serial.println();
          pCONT_sdcard->sdcard_status.bytes_written_to_file += bytes_read;
        }
      // }

    }//end card opened
    



// 
  }//while

}




void mSDCard::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if(!root){
		Serial.println("Failed to open directory");
		return;
	}
	if(!root.isDirectory()){
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while(file){
		if(file.isDirectory()){
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if(levels){
				listDir(fs, file.name(), levels -1);
			}
		} else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}

}


void mSDCard::createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void mSDCard::removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void mSDCard::readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void mSDCard::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void mSDCard::appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
}

void mSDCard::appendFile_open_and_close(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void mSDCard::write_append_array(fs::FS &fs, const char * path, uint8_t* buffer, uint16_t buflen)
{
    // Serial.printf("Appending to file: %s\n", path);

    // I dont want this, because it keeps opening and closing!!!
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        // Serial.println("Failed to open file for appending");
        return;
    }

    for(uint16_t i=0; i<buflen; i++)
    {
        file.write(buffer[i]);
    }

    // if(file.print(message)){
    //     Serial.println("Message appended");
    // } else {
    //     Serial.println("Append failed");
    // }
}

void mSDCard::SubTask_Append_To_Open_File(char* buffer, uint16_t buflen)
{
   // DEBUG_PIN1_SET(0);

  switch(writer_settings.status)
  {
    case FILE_STATUS_OPENING_ID:

// add option to use GPS time instead of NTP time, where it updates every second inside gps

			sprintf(writer_settings.file_name, "/%s_%s_%02d%02d%02d_%03d.txt", // Unique name each time it is opened
				"APPEND", 
				DEVICENAME_FOR_SDCARD_FRIENDLY_CTR, 
				pCONT_time->RtcTime.hour, pCONT_time->RtcTime.minute, pCONT_time->RtcTime.second, 
				random(1,1000)
			);

      stored_file = SD.open(writer_settings.file_name, FILE_APPEND);
      if(!stored_file){
        ALOG_TST(PSTR(DEBUG_INSERT_PAGE_BREAK "stored_file \"%s\" did not open"),writer_settings.file_name);
      }else{
	      ALOG_TST(PSTR("stored_file \"%s\" Opened!"),writer_settings.file_name);
      	writer_settings.status = FILE_STATUS_OPENED_ID;
				sdcard_status.bytes_written_to_file = 0;
			}


    // break;
    case FILE_STATUS_OPENED_ID:

      for(int i=0; i<buflen; i++)
      {
        stored_file.write(buffer[i]);
      }
	    sdcard_status.bytes_written_to_file += buflen;

    break;
    case FILE_STATUS_CLOSING_ID:
        
      ALOG_TST(PSTR("stored_file \"%s\" Closed!"),writer_settings.file_name);

      stored_file.close();

      writer_settings.status = FILE_STATUS_CLOSED_ID;

    break;

    //no break
    // write into

    //close file
  }

  //for phd
    extern_flag_sdcard_writer_status = writer_settings.status;

  //  DEBUG_PIN1_SET(1);

}

/***
 * New ringbuffer for stream out to sdcard, every 10ms if there is new sdcard data to write, it will be taken from ringbuffer
 * 
 * */
void mSDCard::Stream_AddToBuffer(char* buffer, uint16_t buflen)
{






}







void mSDCard::renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void mSDCard::deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void mSDCard::testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}




int8_t mSDCard::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    
    // #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
    //   init_SDCARD_is_Serial_Debug_Only();
    // #else
      init();
    // #endif

    #ifdef ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER

    
  // // Create mutex before starting tasks
  // mutex = xSemaphoreCreateMutex();
  // // Take the mutex
  // xSemaphoreTake(mutex, portMAX_DELAY);

  // Create mutex
  // mutex_filling_sd_writer = xSemaphoreCreateMutex();

    // start a task to read samples from the ADC
    TaskHandle_t writerTaskHandle;
    xTaskCreatePinnedToCore(sdcardWriterTask, "SDCard Writer", 4096, this, 1, &writerTaskHandle, 0);

    #endif // ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER

  }
  else
  if(function == TASK_EVERY_SECOND){
    if(sdcard_status.init_error_on_boot)
    {
      init();
    }
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
    {

    }
    break;
    case TASK_EVERY_50_MSECOND:

      EveryLoop_Handle_Appending_File_Method();

      /**
       * check if ringbuffer stream has data, if so, write to sdcard (Assuming its open)
       * */
      #ifdef ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
      #ifndef ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
        Handle_Write_Ringbuffer_Stream_To_SDCard();
      #endif
      #endif // ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
          
    break;
    case TASK_EVERY_SECOND:  
    {

      // If not init, retry init, but ignore press to start
      if(sdcard_status.init_error_on_boot == true)
      {
        init();
      }

    } 
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
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
} // END Tasker


/**
 * @brief - Function will handle if the file should be opened or closed
 * 
 * */
void mSDCard::EveryLoop_Handle_Appending_File_Method()
{

  // Check if desired SD card status change and update saved variable
  if(sdcard_status.isopened != sdcard_status.isopened_previous_state)
  {

    // If not init, retry init, but ignore press to start
    if(sdcard_status.init_error_on_boot == true)
    {
      init();
      // If still failed, then reject opening file
      if(sdcard_status.init_error_on_boot == true)
      {
        return;
      }
    }

    ALOG_TST(PSTR("sdcard_status.isopened CHANGED =%d"), sdcard_status.isopened);
    sdcard_status.isopened_previous_state = sdcard_status.isopened;
    // If opened, close
    if(sdcard_status.isopened)
    {
      writer_settings.status = mSDCard::FILE_STATUS_CLOSING_ID;
      SubTask_Append_To_Open_File();
    }
    else
    {
      writer_settings.status = mSDCard::FILE_STATUS_OPENING_ID;
      SubTask_Append_To_Open_File();
    }
  }

}


void mSDCard::init(void)
{

  Serial.println("Initializing SD card...");

  int8_t chip_select = -1;
  int8_t clock = -1;
  int8_t mosi = -1;
  int8_t miso = -1;

  chip_select = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_CSO_ID);
  clock = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_CLK_ID);
  mosi = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_MOSI_ID);
  miso = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_MISO_ID);
  
  uint8_t sd_hardware_type = 0;

  if((chip_select>=0)&&(clock>=0)&&(mosi>=0)&&(miso>=0))
  {
    sd_hardware_type = 1;
  }

  /**
   * init VSPI
   * */
  if(sd_hardware_type == 1)
  {
    ALOG_INF(PSTR("(clock, miso, mosi, chip_select)=(%d,%d,%d,%d)"), clock, miso, mosi, chip_select);
    spiSD.begin(clock, miso, mosi, chip_select); //SCK,MISO,MOSI,SS //HSPI1
  }else{
    return;
  }

  if(!SD.begin(chip_select, spiSD )){
    Serial.println("Card Mount Failed");
    sdcard_status.init_error_on_boot = true;
    // delay(3000);
    return;
  }


  uint8_t cardType = SD.cardType();

  Serial.print("SD Card Type: ");
  switch(cardType)
  {
    default:
    case CARD_NONE: 
      Serial.println("No SD card attached");
      sdcard_status.init_error_on_boot = true;
      return;      
    break;
    case CARD_MMC:
      Serial.println("MMC");
    break;
    case CARD_SD:
      Serial.println("SDSC");
    break;
    case CARD_SDHC:
      Serial.println("SDHC");
    break;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);

  listDir(SD, "/", 0);

  Serial.printf("SD Card Size: %lluMB\n\r", cardSize);
  Serial.printf("Total space: %lluMB\n\r", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n\r", SD.usedBytes() / (1024 * 1024));

  // Success, no failure
  sdcard_status.init_error_on_boot = false;
  settings.fEnableModule = true;

}

#ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
void mSDCard::init_SDCARD_is_Serial_Debug_Only()
{

  // Success, no failure
  sdcard_status.init_error_on_boot = false;
  settings.fEnableModule = true;

}
#endif  //   USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT


void mSDCard::Pre_Init(){

  #ifdef USE_SDCARD_RINGBUFFER_STREAM_OUT
    //chip select pin needs setting
    init_SDCard_StreamOut_RingBuffer();
  #endif // USE_SDCARD_RINGBUFFER_STREAM_OUT

}

#ifdef USE_SDCARD_RINGBUFFER_STREAM_OUT
void mSDCard::init_SDCard_StreamOut_RingBuffer()
{

  stream.ringbuffer_handle = xRingbufferCreate(RINGBUFFER_HANDLE_SDCARD_WRITER_LENGTH, RINGBUF_TYPE_BYTEBUF);
  if (stream.ringbuffer_handle != NULL) {
    stream.initialised = true;
  }

  char data_ctr[] = "\0";
  UBaseType_t res =  xRingbufferSend(stream.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  if (res != pdTRUE) {
    AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
    stream.initialised = false; //disable if false
  }

}


void mSDCard::Handle_Write_Ringbuffer_Stream_To_SDCard()
{

  // #ifdef ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  // if(pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID)
  // {

  //   uint16_t maximum_bytes_to_write_in_one_chunk = 1024; // 2kB per write
  //   uint16_t byte_read_limit = maximum_bytes_to_write_in_one_chunk > BufferWriterI->GetBufferSize() ? BufferWriterI->GetBufferSize() : maximum_bytes_to_write_in_one_chunk;

  //   // BufferWriterI->Clear();
  //   uint16_t bytes_read = GetRingBufferDataAndClear(0, stream_out_buffer, byte_read_limit, '\n', false);
  //   // if(strlen(BufferWriterI->GetPtr())==0){
  //   //   ALOG_TST(PSTR("GPS UART%d >> [%d] \"%s\""), 1, bytes_to_read, BufferWriterI->GetPtr());
  //   // }

  //   //if any data found
  //   if(bytes_read)
  //   {  
  //     // char* pbuffer = BufferWriterI->GetPtr();
  //     //ALOG_TST(PSTR("buffer[%d]=\"%s\""),bytes_read, stream_out_buffer);
  //     pCONT_sdcard->SubTask_Append_To_Open_File(stream_out_buffer, bytes_read);      
  //   }


  // }
  // #endif // ENABLE_SDLOGGER_APPEND_TIME_TEST


}

/**
 * Universal to all UARTs, get RingBuffer
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * @return item_size number of bytes read from buffer
 * */
uint16_t mSDCard::GetRingBufferDataAndClear(uint8_t stream_num, char* buffer, uint16_t buflen, char optional_read_until_char, bool flag_clear_buffer_after_read)
{

  // Receive an item from no-split ring buffer
  size_t item_size = 0;
  // Wait at most 1ms to read from buffer, read a maximum number of bytes
  char*  item = (char *)xRingbufferReceiveUpTo(stream.ringbuffer_handle, &item_size, pdMS_TO_TICKS(10), buflen);

  //Check received item
  if (item != NULL) 
  {
    // Read from buffer
    memcpy(buffer,item,item_size);
    //Return Item
    vRingbufferReturnItem(stream.ringbuffer_handle, (void *)item); // Free memory
  // }
  } 

  return item_size;

}


uint16_t mSDCard::AppendRingBuffer(char* buffer, uint16_t buflen)
{
  
  /**
   * Only add if the file is opened, else it will cause overflow
   * */
  if( extern_flag_sdcard_writer_status == 2)
    {
  // UBaseType_t res =  
  xRingbufferSend(stream.ringbuffer_handle, buffer, buflen, pdMS_TO_TICKS(100));
  // if (res != pdTRUE) {
  //     printf("Failed to send item\n");
  // }

    }
  
  stream.flag_data_waiting = true; // easier than checking for bytes each time

  return 1; //res

}

#endif // USE_SDCARD_RINGBUFFER_STREAM_OUT


uint8_t mSDCard::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  char buffer[30];
  
  JBI->Start();  

    JBI->Object_Start("Debug");
      JBI->Array_Start("message1");
          for(int i=0;i<debug.write_time.opened.size();i++)
              JBI->Add(debug.write_time.closed[i]-debug.write_time.opened[i]);
      JBI->Array_End();
    JBI->Object_End();
  
    JBI->Add_P(PM_TIME, 1000);
  return JBI->End();

}


uint8_t mSDCard::ConstructJSON_FileWriter(uint8_t json_level, bool json_appending)
{

  char buffer[30];
  
  JBI->Start();  

    JBI->Add("file_name", writer_settings.file_name);
    JBI->Add("status", writer_settings.status);

  return JBI->End();

}

/**
 * @brief Created for write time tests of sector sizes
 * */
uint8_t mSDCard::ConstructJSON_Debug_WriteTimes(uint8_t json_level, bool json_appending)
{

  char buffer[30];
  
  JBI->Start();  

    JBI->Object_Start("512B");
      JBI->Add("CompleteWriteTime", debug_write_times.complete_write_duration);
      JBI->Add("BytesWritten", debug_write_times.write_byte_count);
    JBI->Object_End();


  return JBI->End();

}


  #ifdef USE_MODULE_NETWORK_MQTT
void mSDCard::MQTTHandler_Init(){

  struct handler<mSDCard>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSDCard::ConstructJSON_Settings;

  ptr = &mqtthandler_file_writer;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FILE_WRITER_CTR;
  ptr->ConstructJSON_function = &mSDCard::ConstructJSON_FileWriter;

  ptr = &mqtthandler_debug_write_times;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_WRITE_TIMES_CTR;
  ptr->ConstructJSON_function = &mSDCard::ConstructJSON_Debug_WriteTimes;

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSDCard::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSDCard::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

void mSDCard::MQTTHandler_Sender(){
    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }

}


  #endif// USE_MODULE_NETWORK_MQTT




/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mSDCard::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 


  if(jtok = obj["ListDir"]){

    CommandSet_SerialPrint_FileNames(jtok.getStr());
    
  }
  

  if(jtok = obj["WriteFile"]){

    // Also check for datafile
    JsonParserToken jtok_data = obj["DataFile"];
    if(!jtok_data.isNull()){
      CommandSet_WriteFile(jtok.getStr(), jtok_data.getStr());
    }
    else{
      CommandSet_WriteFile(jtok.getStr());
    }


  //   if(jtok.isStr()){
  //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  //       CommandSet_PaletteID(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
    // if(jtok.isNum()){
    //   CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
    //   set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
    //   set_fan_speed = jtok.getInt();
    //   data_buffer.isserviced++;
    // }
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  if(jtok = obj["ReadFile"]){

    CommandSet_ReadFile(jtok.getStr());
    
  }


  /**
   * Set flag, write 100 single bytes, 1 per second
   * */
  if(jtok = obj["Debug"].getObject()["WriteTest"]){

    // CommandSet_ReadFile(jtok.getStr());
    debug.bytes_to_write = jtok.getInt();
    debug.test_mode = WRITE_BYTES_ID;

  }

  if(jtok = obj["Debug"].getObject()["OpenFile"]){

    // CommandSet_ReadFile(jtok.getStr());
    // debug.bytes_to_write = jtok.getInt();
    // debug.test_mode = WRITE_BYTES_ID;
    char buffer[5] = {0,1,2,3,4};
    uint8_t close_decounter = 10;
    writer_settings.status = FILE_STATUS_OPENING_ID;
    SubTask_Append_To_Open_File(buffer, 5);

  }

  

  // /**
  //  * Set flag, write 512 bytes, same time
  //  * */
  // if(jtok = obj["Debug"].getObject()["LargeWriteTest"]){

  //   // CommandSet_ReadFile(jtok.getStr());

  // }
  
  #ifdef USE_MODULE_NETWORK_MQTT
  MQTTHandler_RefreshAll();
  #endif// USE_MODULE_NETWORK_MQTT

}



void mSDCard::CommandSet_SDCard_Appending_File_Method_State(uint8_t state)
{

  if(state == 2) //toggle
  {
    sdcard_status.isopened ^= 1; 
    ALOG_TST(PSTR("CommandSet_LoggingState sdcard_status.isopened == 2, %d"),sdcard_status.isopened);

    /**
     * temp fix, when opening, try set the internal time from gps time once
     * */
    if(sdcard_status.isopened)
    {
      #ifdef USE_MODULE_SENSORS_GPS_SERIAL
      pCONT_time->SetUTCTime(
                    pCONT_gps->gps_result_stored.dateTime.year,
                    pCONT_gps->gps_result_stored.dateTime.month,
                    pCONT_gps->gps_result_stored.dateTime.day,
                    pCONT_gps->gps_result_stored.dateTime.hours,
                    pCONT_gps->gps_result_stored.dateTime.minutes,
                    pCONT_gps->gps_result_stored.dateTime.seconds
                  );
      #endif // USE_MODULE_SENSORS_GPS_SERIAL

    }


  }else
  {
    sdcard_status.isopened = state;
    ALOG_TST(PSTR("CommandSet_LoggingState sdcard_status.isopened = state,  %d"),sdcard_status.isopened);

  }

}















/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_ReadFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_ReadFile(const char* filename){

  readFile(SD, filename);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ReadFile")), filename);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_WriteFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_WriteFile(const char* filename, const char* data){

  char filename_with_extention[50];
  sprintf(filename_with_extention, "%s.txt", filename);
  if(data == nullptr){
    writeFile(SD, filename_with_extention, "Empty File!");
  }else{
    writeFile(SD, filename_with_extention, "Hello ");
  }

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("WriteFile")), filename_with_extention);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_CreateFile_WithName(char* value){

//   analogWrite(pin, value);
  createDir(SD, "/mydir");

// AddLog(LOG_LEVEL_DEV_TEST,PSTR("pwm %d value = %d"),pin,value);


  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ListDir")), dirname);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** PixelHardwareType *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSDCard::CommandSet_SerialPrint_FileNames(const char* dirname){

  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("TESTListDir")), dirname);
  listDir(SD, dirname, 0);

  // listDir(SD, "/", 0);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ListDir")), dirname);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

} 

void mSDCard::SDCardSpeedDebug()
{

  // switch(write_test_count)
  debug_write_times.write_byte_count = 300;
  // { 
  //   default:
    // case 0: 
    SDCardBulkSpeedTest(debug_write_times.write_test_count,debug_write_times.write_byte_count); 
    // break;
  //   case 1: SDCardBulkSpeedTest(write_test_count,100); break;
  //   case 2: SDCardBulkSpeedTest(write_test_count,1000); break;
  //   case 3: SDCardBulkSpeedTest(write_test_count,10000); break;
  //   case 4: SDCardBulkSpeedTest(write_test_count,100000); break;
  // }

  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_file_writer.flags.SendNow = true;
  #endif// USE_MODULE_NETWORK_MQTT

  // if(write_test_count++ > 1)
  // {
  //   write_test_count = 0;
  // }

}


void mSDCard::SDCardBulkSpeedTest(uint8_t test_number, uint32_t bytes_to_write)
{
  ALOG_TST(PSTR("SDCardBulkSpeedTest %d %d Starting in 5 seconds"),test_number,bytes_to_write);
  // delay(5000);

  uint32_t time_start = millis();
 // DEBUG_PIN1_SET(LOW);

  // Open file
  sprintf(writer_settings.file_name, "/%s%d.txt", "TestWriter",test_number);
  File file = SD.open(writer_settings.file_name, FILE_APPEND);
  if(!file){
    ALOG_TST(PSTR("file \"%s\" did not open"),writer_settings.file_name);
  }
  ALOG_TST(PSTR("file \"%s\" Opened!"),writer_settings.file_name);

  // write all bytes (numbers 0 to 9 single digits only)
  uint8_t bytes_for_card = 0;
  for(int i=0; i<bytes_to_write; i++)
  {
    //DEBUG_PIN2_SET(LOW);
    file.write(bytes_for_card);
    //DEBUG_PIN2_SET(HIGH);
    if(bytes_for_card++>9)
    {
      bytes_for_card = 0;
    }
  }

  //close file
  file.close();
 // DEBUG_PIN1_SET(HIGH);

  uint32_t time_duration = millis() - time_start;
  debug_write_times.complete_write_duration = time_duration;
  ALOG_TST(PSTR("file_name \"%s\" bytes: %d, time: %d ms"),writer_settings.file_name, bytes_to_write, time_duration);

}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif
