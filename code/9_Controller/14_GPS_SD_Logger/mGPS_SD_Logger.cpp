#include "mGPS_SD_Logger.h"

#ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER

/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
Prototyped after the serial_logger, it will store GPS data onto sd card
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
*************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/


int8_t mGPS_SD_Logger::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;

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
    break;
    case TASK_EVERY_100_MSECOND:
      // SubTask_UpdateOLED_Detailed_GPS();
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_DRIVERS_SDCARD
      pCONT_sdcard->CommandSet_SDCard_Appending_File_Method_State(2);
      #endif
      sequence_test = 0;
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


void mGPS_SD_Logger::Pre_Init(void)
{
  settings.fEnableSensor = true;      
}


void mGPS_SD_Logger::Init(void)
{

}


void mGPS_SD_Logger::EveryLoop()
{

  if(mTime::TimeReached(&tSaved_MillisWrite, 100))
  {
    SubTask_Generate_GPS_Json_SDCard_Stream();
  }

}


void mGPS_SD_Logger::SubTask_Generate_GPS_Json_SDCard_Stream()
{
  
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
  #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
    char* buff_ptr = BufferWriterI->GetPtr();
    for(int i=0;i<BufferWriterI->GetLength();i++){
      Serial.print(buff_ptr[i]);
    }
    // For pretty output
    Serial.println();
  #endif

}


void mGPS_SD_Logger::EverySecond()
{

  SubTask_UpdateOLED_Detailed_GPS();
  // SubTask_HandleSDCardLogging();
  // SubTask_Debug_BasicFileWriteTest();

}


/**
 * @brief Show logger status on OLED: Detailed GPS View for field tests
 * @note 4x11 chars at font 1
 * 
 * "Latitude" = " 54.5000000"
 * "Longitude" = " 54.5000000"
 * "Altitude + Sat fix count %02d" = " 54.5000000"
 * "file writing status" (open/close + bytes)
 * */
void mGPS_SD_Logger::SubTask_UpdateOLED_Detailed_GPS()
{

  char line_ctr[15] = {0};
  char buffer[15] = {0};

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306

  snprintf(line_ctr, sizeof(line_ctr), "%d", pCONT_gps->gps_result_stored.latitudeL());
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  snprintf(line_ctr, sizeof(line_ctr), "%d", pCONT_gps->gps_result_stored.longitudeL());
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 1);
  // snprintf(line_ctr, sizeof(line_ctr), "%f", pCONT_gps->gps_result_stored.latitude());
  // pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  // snprintf(line_ctr, sizeof(line_ctr), "%f", pCONT_gps->gps_result_stored.longitude());
  // pCONT_iDisp->LogBuffer_AddRow(line_ctr, 1);
  
  uint32_t bytes_written = pCONT_sdcard->sdcard_status.bytes_written_to_file;
  char unit_type = 'B';
  if(bytes_written>50000){
    bytes_written /= 1000; //into kB
    unit_type = 'k';
  }
  snprintf(buffer, sizeof(buffer), "%d %c s%d",
    bytes_written,
    unit_type,
    pCONT_gps->gps_result_stored.satellites
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer,2);

  snprintf(buffer, sizeof(buffer), "%c%s%s",
    pCONT_sdcard->sdcard_status.init_error_on_boot ? 'E' : 'f',
    pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID ?"OPEN!":"cd",
    &pCONT_sdcard->writer_settings.file_name[8] //skipping "APPEND_" to get just time
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);

  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}




uint8_t mGPS_SD_Logger::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mGPS_SD_Logger::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

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
uint8_t mGPS_SD_Logger::ConstructJSON_SDCardSuperFrame(uint8_t json_level, bool json_appending){

  JBI->Start();
    
  /**
   * on first sequence number, send additional useful info
   * */
  if(sequence_test == 0)
  {
    JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
  }

  JBI->Add("N", sequence_test++);

  uint32_t elapsed_millis_from_last_construct = millis() - tSaved_Constructed_Json_SDCardSuperFrame_ms;
  tSaved_Constructed_Json_SDCardSuperFrame_ms = millis();

  // Debug data only
  JBI->Add("M",millis());
  JBI->Add("D",elapsed_millis_from_last_construct);

  // GPS data
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  JBI->Object_Start("G");
    JBI->Add("la", pCONT_gps->gps_result_stored.latitudeL()); 
    JBI->Add("lg", pCONT_gps->gps_result_stored.longitudeL()); 
    JBI->Add("at", pCONT_gps->gps_result_stored.altitude_cm()); //above mean sea level, in cm 
    JBI->Add("sd", pCONT_gps->gps_result_stored.speed());
    JBI->Add("hd", pCONT_gps->gps_result_stored.heading_cd());
    JBI->Add("gh", pCONT_gps->gps_result_stored.geoidHeight_cm()); // Height of the geoid above the WGS84 ellipsoid
    JBI->Add("s",  pCONT_gps->gps_result_stored.satellites);

    uint32_t timeofday_seconds = 
      (pCONT_gps->gps_result_stored.dateTime.hours*3600) +
      (pCONT_gps->gps_result_stored.dateTime.minutes*60) +
      (pCONT_gps->gps_result_stored.dateTime.seconds*1000);

    uint32_t tod_millis = 
      (timeofday_seconds*1000) + 
      pCONT_gps->gps_result_stored.dateTime_ms();

    JBI->Add("tms",  tod_millis);
    JBI->Add_FV("t",  "\"%02d:%02d:%02d-%03d\"", pCONT_gps->gps_result_stored.dateTime.hours, pCONT_gps->gps_result_stored.dateTime.minutes, pCONT_gps->gps_result_stored.dateTime.seconds, pCONT_gps->gps_result_stored.dateTime_ms());
  JBI->Object_End();
  #endif // USE_MODULE_SENSORS_GPS_SERIAL
  
  return JBI->End();
    
}

#endif
