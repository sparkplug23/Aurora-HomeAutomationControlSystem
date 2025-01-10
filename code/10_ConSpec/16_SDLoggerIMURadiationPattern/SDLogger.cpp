#include "SDLogger.h"

#ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN


int8_t mSDLoggerIMURadiationPattern::Tasker(uint8_t function, JsonParserObject obj)
{
  
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
    case TASK_LOOP: {
      EveryLoop();

      // uint16_t adc_level_1 = adc1_get_raw(ADC1_CHANNEL_6);
      // // ets_delay_us(1);
      // uint16_t adc_level_2 = adc1_get_raw(ADC1_CHANNEL_7);
      // ALOG_TST(PSTR("adc_level = \t%d\t%d"), adc_level_1, adc_level_2);

    }break;  
    case TASK_EVERY_SECOND: 
      EverySecond();
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



void mSDLoggerIMURadiationPattern::UpdateInternalRTCTimeWithGPSTime()
{

  /**
   * Only update the time IF the sdcard is not open already and being written to
   * */
  #if defined(USE_MODULE_SENSORS_GPS_SERIAL) && defined(USE_SYSTEM_SDCARD_LOGGING)
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
  #endif // gps & sdcard

}

void mSDLoggerIMURadiationPattern::Pre_Init(void)
{

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;
  
  if(pCONT_pins->PinUsed(GPIO_ADC1_CH4_ID))
  {
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH4_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_4;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
    settings.fSensorCount++;
  }

  if(pCONT_pins->PinUsed(GPIO_ADC1_CH7_ID))
  {
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH7_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_7;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
    settings.fSensorCount++;
  }

  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  }

  #ifdef ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING
  pinMode(GPIO_FUNCTION_MANUAL_ENABLE_SAMPLING_NUMBER, INPUT_PULLUP);
  #endif
  #ifdef GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER
  pinMode(GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER, INPUT_PULLUP);
  #endif

}


void mSDLoggerIMURadiationPattern::Init(void)
{

  #ifdef DISABLE_NETWORK
  WiFi.mode(WIFI_OFF);
  btStop();  
  #endif // DISABLE_NETWORK
  
  // Configure all channel atten and width
  for(int i=0; i<settings.fSensorCount; i++)
  {
    switch(adc_config[i].input_pin)
    {
      case 32:
        adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        ALOG_TST(PSTR("ADC1_CHANNEL_4 set"));
      break;
      case 34:
        adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        ALOG_TST(PSTR("ADC1_CHANNEL_6 set"));
      break;
      case 35:
        adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        ALOG_TST(PSTR("ADC1_CHANNEL_7 set"));
      break;
    }

  }

}


void mSDLoggerIMURadiationPattern::EveryLoop()
{

  // The freq the Construct is called will be the sample rate, lets try 100ms (ie 10 per second)
  if(mTime::TimeReached(&tSaved_SampleAndSave, 20))
  {

    ConstructJSON_SDCardSuperFrame();

    /**
     * Append to sdcard stream
     * */
    #ifdef USE_MODULE_DRIVERS_SDCARD
      #ifdef ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING
      if(digitalRead(GPIO_FUNCTION_MANUAL_ENABLE_SAMPLING_NUMBER)==0)
      {
      #endif
        pCONT_sdcard->AppendRingBuffer(JBI->GetPtr(), JBI->GetLength());
      #ifdef ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING
      }
      #endif
      // ALOG_TST(PSTR("SDCardStream >> [%d] \"%s\""), JBI->GetLength(), JBI->GetPtr());
    #else
      ALOG_TST(PSTR("SDCardStream >> [%d] \"%s\""), JBI->GetLength(), JBI->GetPtr());
    #endif //USE_MODULE_DRIVERS_SDCARD

  }



}


void mSDLoggerIMURadiationPattern::EverySecond()
{

  SubTask_UpdateOLED();

}


/**
 * @brief Show logger status on OLED
 * @note 4x11 chars at font 1
 * 
 * "Open xxxxxx" where x is the time opened
 * "Byte bbbbbb" where b is the number of bytes written (changing to kB when >1000)
 * 
 * "ADC2 ADC2"  %04d of adc readings
 * "HHMMSS     " where HHMMSS is the GPS RTC time, which is also appended to the opened file name
 * */
void mSDLoggerIMURadiationPattern::SubTask_UpdateOLED()
{

#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  char line_ctr[15] = {0};
  char buffer[15] = {0};

      float heading;
      float roll;
      float pitch;
  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  /**
   * Row 1
   * adc 
   * */
  #ifdef USE_DEVFEATURE_ADC_IN_CONTROLLER
  snprintf(line_ctr, sizeof(line_ctr), "%04d %04d%c",
    adc_values.adc2, 
    adc_values.adc5,digitalRead(GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER)?'N':'S'
  );
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  #endif // USE_DEVFEATURE_ADC_IN_CONTROLLER

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

      pCONT_LSM303D->CalculateOrientation(
        pCONT_LSM303D->sensor[0].lsm303d->a.x, pCONT_LSM303D->sensor[0].lsm303d->a.y, pCONT_LSM303D->sensor[0].lsm303d->a.z, 
        pCONT_LSM303D->sensor[0].lsm303d->m.x, pCONT_LSM303D->sensor[0].lsm303d->m.y, pCONT_LSM303D->sensor[0].lsm303d->m.z, 
        &heading, &roll, &pitch
      );

  snprintf(buffer, sizeof(buffer), "%d %c H%d",bytes_written,unit_type, (int)heading);
  
  pCONT_iDisp->LogBuffer_AddRow(buffer,2);
  #endif // USE_MODULE_DRIVERS_SDCARD

  /**
   * Show magnetometer as example, x-axis value
   * */
  #ifdef USE_MODULE_SENSORS_LSM303D

// pCONT_LSM303D->sensor[0].lsm303d
//   heading


  snprintf(buffer, sizeof(buffer), "R%d P:%d", (int)roll, (int)pitch);
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #else // USE_MODULE_SENSORS_LSM303D
  snprintf(buffer, sizeof(buffer), "LSM303D d/c");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_SENSORS_LSM303D

  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mSDLoggerIMURadiationPattern::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mSDLoggerIMURadiationPattern::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

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
uint8_t mSDLoggerIMURadiationPattern::ConstructJSON_SDCardSuperFrame(uint8_t json_level, bool json_appending){

  JBI->Start();
    
    /**
     * on first sequence number, send additional useful info
     * */
    // if(sequence_test == 0)
    // {
    //   JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
    // }

    // JBI->Add("N", sequence_test++);

    // Debug data only
    JBI->Add("M",millis());
    JBI->Add("S", (uint8_t)digitalRead(GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER));

    #if defined(USE_MODULE_SENSORS_LSM303D) || defined(USE_MODULE_SENSORS_L3G)
    // Force update of sensor here

    JBI->Object_Start("IA"); // 9 axis gyro

      #ifdef USE_MODULE_SENSORS_LSM303D
      pCONT_LSM303D->sensor[0].lsm303d->read();
      JBI->Object_Start("M");
        JBI->Add("x", pCONT_LSM303D->sensor[0].lsm303d->m.x);
        JBI->Add("y", pCONT_LSM303D->sensor[0].lsm303d->m.y);
        JBI->Add("z", pCONT_LSM303D->sensor[0].lsm303d->m.z);
      JBI->Object_End();
      JBI->Object_Start("A");
        JBI->Add("x", pCONT_LSM303D->sensor[0].lsm303d->a.x);
        JBI->Add("y", pCONT_LSM303D->sensor[0].lsm303d->a.y);
        JBI->Add("z", pCONT_LSM303D->sensor[0].lsm303d->a.z);
      JBI->Object_End();
      
      float heading;
      float roll;
      float pitch;
      pCONT_LSM303D->CalculateOrientation(
        pCONT_LSM303D->sensor[0].lsm303d->a.x, pCONT_LSM303D->sensor[0].lsm303d->a.y, pCONT_LSM303D->sensor[0].lsm303d->a.z, 
        pCONT_LSM303D->sensor[0].lsm303d->m.x, pCONT_LSM303D->sensor[0].lsm303d->m.y, pCONT_LSM303D->sensor[0].lsm303d->m.z, 
        &heading, &roll, &pitch
      );

      JBI->Object_Start("O");
        JBI->Add("H", heading);
        JBI->Add("R", roll);
        JBI->Add("P", pitch);
      JBI->Object_End();

      #endif
      #ifdef USE_MODULE_SENSORS_L3G
      pCONT_L3G->gyro->read();
      JBI->Object_Start("G");
        JBI->Add("x", pCONT_L3G->gyro->g.x);
        JBI->Add("y", pCONT_L3G->gyro->g.y);
        JBI->Add("z", pCONT_L3G->gyro->g.z);
      JBI->Object_End();
      #endif
    
    
    JBI->Object_End();
    
    JBI->Object_Start("IL"); // 6 axis -- leg
      #ifdef USE_MODULE_SENSORS_LSM303D
      pCONT_LSM303D->sensor[1].lsm303d->read();
      JBI->Object_Start("M");
        JBI->Add("x", pCONT_LSM303D->sensor[1].lsm303d->m.x);
        JBI->Add("y", pCONT_LSM303D->sensor[1].lsm303d->m.y);
        JBI->Add("z", pCONT_LSM303D->sensor[1].lsm303d->m.z);
      JBI->Object_End();
      JBI->Object_Start("A");
        JBI->Add("x", pCONT_LSM303D->sensor[1].lsm303d->a.x);
        JBI->Add("y", pCONT_LSM303D->sensor[1].lsm303d->a.y);
        JBI->Add("z", pCONT_LSM303D->sensor[1].lsm303d->a.z);
      JBI->Object_End();
      
      pCONT_LSM303D->CalculateOrientation(
        pCONT_LSM303D->sensor[1].lsm303d->a.x, pCONT_LSM303D->sensor[1].lsm303d->a.y, pCONT_LSM303D->sensor[1].lsm303d->a.z, 
        pCONT_LSM303D->sensor[1].lsm303d->m.x, pCONT_LSM303D->sensor[1].lsm303d->m.y, pCONT_LSM303D->sensor[1].lsm303d->m.z, 
        &heading, &roll, &pitch
      );

      JBI->Object_Start("O");
        JBI->Add("H", heading);
        JBI->Add("R", roll);
        JBI->Add("P", pitch);
      JBI->Object_End();

      #endif    
    
    JBI->Object_End();


    #endif // USE_MODULE_SENSORS_LSM303D


    JBI->Object_Start("ADC");
      adc_values.adc2 = adc1_get_raw(ADC1_CHANNEL_4);
      adc_values.adc5 = adc1_get_raw(ADC1_CHANNEL_7);
      JBI->Add("2", adc_values.adc2);
      JBI->Add("5", adc_values.adc5);
    JBI->Object_End();

  return JBI->End();
    
}

/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


  #ifdef USE_MODULE_NETWORK_MQTT
void mSDLoggerIMURadiationPattern::MQTTHandler_Init(){

  struct handler<mSDLoggerIMURadiationPattern>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSDLoggerIMURadiationPattern::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSDLoggerIMURadiationPattern::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSDLoggerIMURadiationPattern::ConstructJSON_Sensor;

  ptr = &mqtthandler_sdcard_superframe;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SDCARD_SUPERFRAME_CTR;
  ptr->ConstructJSON_function = &mSDLoggerIMURadiationPattern::ConstructJSON_SDCardSuperFrame;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSDLoggerIMURadiationPattern::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSDLoggerIMURadiationPattern::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    // if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      // handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSDLoggerIMURadiationPattern::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

  #endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/



void mSDLoggerIMURadiationPattern::parse_JSONCommand(JsonParserObject obj)
{

}

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




void mSDLoggerIMURadiationPattern::CommandSet_SDCard_OpenClose_Toggle()
{//uint16_t time_secs, uint8_t relay_id){


  logger_status.enable ^= 1;

  // sdcard_status.isopened ^= 1;
  // sdcard_status.enable_logging ^= 1;
  // sdcard_status.isopened = sdcard_status.enable_logging; //this should not be linked

  // // If closed, start logging and begin sdcard opening sequence
  // if(sdcard_status.isopened)

  ALOG_TST(PSTR("mSDLoggerIMURadiationPattern CommandSet_SDCard_OpenClose_Toggle %d"),logger_status.isopened);


  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

}



void mSDLoggerIMURadiationPattern::CommandSet_LoggingState(uint8_t state)
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
 * 
*******************************************************************************************************************/

#endif
