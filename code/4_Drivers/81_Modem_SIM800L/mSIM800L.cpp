#include "mSIM800L.h"

/***
 
 To become the primary controller for the modem. It should maintain connection, reconnect, keep the modem fucntioning. 

 The network module can therefore just "attach" itself to this function with callbacks.


 * */


#ifdef USE_MODULE_DRIVERS_MODEM_800L

// #define MODEM_POWER_ON 23
// ...
// ...https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/issues/29
// ...
// void GoToSleep() {
//   esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//   digitalWrite(MODEM_POWER_ON, LOW);
//   esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
//   esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
//   esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
//   esp_deep_sleep_start();
// }

#ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
StreamDebugger debugger(SerialAT, Serial);
#endif // ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT

int8_t mSIM800L::Tasker(uint8_t function, JsonParserObject obj)
{

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
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
    {      

      Handler_ModemResponses(LOG_LEVEL_DEBUG);
      
      /**
       * @brief Here so it can be set from anywhere/method and then read later
       * 
       */
      if(sms.messages_incoming_index_list.size())
      {
        SMSReadAndEraseSavedSMS();
        DEBUG_LINE_HERE;
      } 
      
      if(flag_modem_initialized)
      {

        #ifndef ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
        ModemUpdate_GPRS();
        #endif
        
        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        ModemUpdate_GPS();
        #endif
        
        ModemUpdate_SMS();        
        
      }

    }
    break;
    case TASK_EVERY_50_MSECOND:{

      if(modem){
        modem->maintain();
      }

      bool is_connected = modem->isNetworkConnected();
      if(!is_connected)
      {
        ALOG_ERR(PSTR(D_LOG_CELLULAR "isNetworkConnected %d"), is_connected);    
      }

    }break;
    case TASK_EVERY_SECOND: {

      if(!flag_modem_initialized)
      {

        DEBUG_LINE_HERE;

        if(Modem_CheckAndRestartUnresponsiveModem() == false) // break until rebooted
        {
          break;
        }

        // Only complete config if modem is working as expected, otherwise retry init again
        if(DataNetwork__InitConfig())
        {
          
          DataNetwork__StartConnection();

          #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
          GPS_Enable();
          #endif 

          SMS_Enable();

          flag_modem_initialized = true;
          DEBUG_LINE_HERE;
        }


      }

      // int batteryLevel = analogRead(35);
      // Serial.printf(PSTR("Battery Level: %d\n"), batteryLevel);




          #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      AutoSMS_Messages_Handle();
          #endif// USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      
      #ifdef ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
      Serial.printf(PSTR("\n\r\n\r=========================SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n\r")); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "waitresponse_error_counter %d"), modem->waitresponse_error_counter); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.apn_connect_called %d"), gprs.apn_connect_called); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.reconnect_init_counts %d"), gprs.reconnect_init_counts); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.connected_seconds %d"), gprs.connected_seconds);    
      ALOG_INF(PSTR(D_LOG_CELLULAR "mqtt downtime counter %d"), pCONT_mqtt->brokers[0]->downtime_counter);    
      if(pCONT_mqtt->brokers.size()){ 
        ALOG_INF(PSTR(D_LOG_CELLULAR "pubsub->connected %d"), pCONT_mqtt->brokers[0]->pubsub->connected() );        
        char buffer[20];
        mSupport::float2CString(pCONT_mqtt->brokers[0]->debug_stats.payload_publish_success_percentage*100, 6, buffer);
        ALOG_INF(PSTR(D_LOG_CELLULAR "payload_publish_success_percentage %d/%d %s"), pCONT_mqtt->brokers[0]->debug_stats.payload_publish_sent, pCONT_mqtt->brokers[0]->debug_stats.payload_publish_missed, buffer);
      }
      ALOG_INF(PSTR(D_LOG_CELLULAR "isGprsConnected %d"), modem->isGprsConnected());   
      ALOG_INF(PSTR(D_LOG_CELLULAR "Sim Connected %d"), modem->isNetworkConnected()); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "smsauto_gps_messages.rate_seconds %d"), smsauto_gps_messages.rate_seconds);       
      ALOG_INF(PSTR(D_LOG_CELLULAR "function_event_queue %d"), pCONT->function_event_queue.size()); 
      Serial.printf(PSTR("=========================EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n\r\n\r")); 
      #endif // ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES


      #ifdef ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS // Need to enable passing back if "send failed" to set connection status as down     
      if(pCONT_mqtt->brokers.size())
      {
        if(pCONT_mqtt->brokers[0]->downtime_counter > ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS) // 10 minutes, do long modem reconnect
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "MQTT Downtime limit on LTE, forcing modem restart"));
          flag_modem_initialized = false; // no response, force restart
        }
      }
      #endif // ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS
    }
    break;
    case TASK_EVERY_FIVE_SECOND:   
    {
      
      // pCONT->function_event_queue.push_back(pCONT->FunctionEvent(TASK_LOG__SHOW_UPTIME,1000));
  
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100)); 
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    }
    break;
    case TASK_EVERY_MINUTE:

        // SendATCommand_SMSImmediateForwardOverSerial();
      // ModemUpdate_BatteryStatus();
      // SMS_Send_TimedHeartbeat();

      #ifdef ENABLE_DEVFEATURE_TEXT_LOCATION_EVERY_MINUTE_WHEN_POWERED
      SMS_GPSLocation();
      #endif 

    break;
    case TASK_EVERY_HOUR:
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      SMS_Send_TimedHeartbeat();
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
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

} // END function


void mSIM800L::Pre_Init(void){
  
  settings.fEnableSensor = true;

  pinMode(12, OUTPUT);
  
}


void mSIM800L::Init(void)
{

  #ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT  // if enabled it requires the streamDebugger lib
  modem = new TinyGsm(debugger);
  #else
  modem = new TinyGsm(SerialAT);
  #endif

  #ifdef USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    SerialAT.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
    delay(100);
    modem->setBaud(UART_CELLULAR_BAUD); // Change from default
    SerialAT.flush();
    SerialAT.end();
    delay(100);
    SerialAT.begin(UART_CELLULAR_BAUD, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
  #else 
    SerialAT.begin(115200, SERIAL_8N1, 26,27);// PIN_RX, PIN_TX);
  #endif // USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED

  SerialAT.setTimeout(100); //ms

  /**
   * @brief Power on
   **/ 
  modemPowerOn();

  #ifdef ENABLE_DEVFEATURE_SIM800L_INIT_SKIP_MODEM_RESTART
    ALOG_INF(PSTR(D_LOG_CELLULAR "Faster init modem"));
    if (!modem->init()) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Failed to init modem"));
    }
  #else
    // Restart takes quite some time, to skip it, call init() instead of restart()
    ALOG_INF(PSTR(D_LOG_CELLULAR "Slower restarting modem"));
    if (!modem->restart()) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Failed to restart modem"));
    }
  #endif

}


bool mSIM800L::Handler_ModemResponses(uint8_t response_loglevel, uint16_t wait_millis)
{

  if (SerialAT.available()) 
  {

    uint32_t receive_millis = millis();

    uint16_t buflen = 0;
    char buffer[300] = {0};


  // delay(50);


    while(SerialAT.available() || ((millis()-receive_millis)<100)) // wait at least 100 between AT_Send and receive
    {
      if (SerialAT.available()) 
      {
        buffer[buflen++] = SerialAT.read();
        if(buflen>(sizeof(buffer)-1))
        {
          ALOG_ERR(PSTR(D_LOG_CELLULAR "RX Overflow"));
          break;
        }
        // if
        // {
        //   ALOG_ERR(PSTR(D_LOG_CELLULAR "RX Timeout"));
        //   break;
        // }
      }
    }

    AddLog(response_loglevel, PSTR(D_LOG_CELLULAR "Handler_ModemResponses %d>> Buffer[%d] \"%s\""), response_loglevel, buflen, buffer);
     
    return parse_ATCommands(buffer, buflen, response_loglevel);

  }

  return false; // nothing handled or received

}



/**
 * @brief 
 * Response syntax:
  +CFUN: <fun>
  The read response parameter and its defined value are the following:
  <fun> 
    0 – Power off and store. RF circuits are disabled by deactivating LTE and Global Navigation Satellite System (GNSS) services.
    1 – Normal mode. The active mode is either LTE or GNSS, or both. Full functional mode. Active modes depend on %XSYSTEMMODE setting.
    2 – Receive only mode in active system mode. Active modes depend on %XSYSTEMMODE setting.
    4 – Flight mode. RF circuits are disabled by deactivating LTE and GNSS services. 
  **/
void mSIM800L::SendATCommand_FunctionalityMode_Minimum()
{
  modem->sendAT("+CFUN=0"); 
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN);
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Minimum %d"), result);
}
void mSIM800L::SendATCommand_FunctionalityMode_Full()
{
  modem->sendAT("+CFUN=1");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN); 
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Full %d"), result);
}



void mSIM800L::ModemUpdate_BatteryStatus()
{
  modem_status.battery.isvalid = modem->getBattStats(
    modem_status.battery.charge_state, 
    modem_status.battery.percentage, 
    modem_status.battery.volts_mv
  );

  ALOG_DBM(PSTR(D_LOG_CELLULAR "{\"Volts_mv\":%d,\"Percent\":%d,\"Charging\":%d}"),
    modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);

  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  pCONT_batt_modem->readings.battery.volts_mv = modem_status.battery.volts_mv;
  pCONT_batt_modem->readings.battery.percentage = modem_status.battery.percentage;
  #endif

}

#if defined(SIM800L_IP5306_VERSION_20190610)
// setPowerBoostKeepOn
bool setupPMU()
{
    bool en = true;
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    if (en) {
        Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
    } else {
        Wire.write(0x35); // 0x37 is default reg value
    }
    return Wire.endTransmission() == 0;
}
#endif

void mSIM800L::modemPowerOn()
{
  ALOG_INF(PSTR("modemPowerOn"));

  
    // Start power management
    if (setupPMU() == false) {
        Serial.println("Setting power error");
    }

#ifdef MODEM_RST
    // Keep reset high
    pinMode(MODEM_RST, OUTPUT);
    digitalWrite(MODEM_RST, HIGH);
#endif

    pinMode(MODEM_PWRKEY, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);

    // Turn on the Modem power first
    digitalWrite(MODEM_POWER_ON, HIGH);

    // Pull down PWRKEY for more than 1 second according to manual requirements
    digitalWrite(MODEM_PWRKEY, HIGH);
    delay(100);
    digitalWrite(MODEM_PWRKEY, LOW);
    delay(1000);
    digitalWrite(MODEM_PWRKEY, HIGH);

    // Initialize the indicator as an output
    pinMode(LED_GPIO, OUTPUT);
    digitalWrite(LED_GPIO, LED_OFF);



  // pinMode(PWR_PIN, OUTPUT);
  // digitalWrite(PWR_PIN, LOW);
  // delay(1000);    // Datasheet T_on = 72ms
  // digitalWrite(PWR_PIN, HIGH);


  flag_modem_initialized = false;
        DEBUG_LINE_HERE;

}

void mSIM800L::modemPowerOff()
{
  ALOG_INF(PSTR("modemPowerOff"));
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);    // Datasheet T_off = 1.2secs
  digitalWrite(PWR_PIN, HIGH);
}

void mSIM800L::modemRestart()
{
  ALOG_INF(PSTR("modemRestart"));
  modemPowerOff();
  delay(5000);
  modemPowerOn();
}


// ==============================================================================================




bool mSIM800L::Modem_CheckAndRestartUnresponsiveModem()
{
  
  uint16_t timeout_wait_for_AT_response = 500;

  if(modem->testAT(timeout_wait_for_AT_response))
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Running"));
    return true;
  }else{
    ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Not responding, restarting (TBC: causes 7.5 second pause)"));
    modemRestart(); // If no response, cause reboot and do something else
    return false;
  }

}




float mSIM800L::GetSignalQualityPower()
{
  return GetSignalQualityPower(modem->getSignalQuality());
}

/**
 * @brief Conversion without calling modem again
 * 
 * @param signal_quality_raw 
 * @return float 
 */
float mSIM800L::GetSignalQualityPower(int16_t signal_quality_raw)
{

  float signal_quality_rssi_dbm = 0;

  if(signal_quality_raw == 99)
  {
    signal_quality_rssi_dbm = -150;
  }
  else
  {
    signal_quality_rssi_dbm = mSupport::mapfloat(signal_quality_raw, 0, 31, -113, -51);
  }

  return signal_quality_rssi_dbm;

}



void mSIM800L::Get_Modem_Hardware()
{

  String name = modem->getModemName();
  ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Name: %s"),name.c_str());

  String modemInfo = modem->getModemInfo();
  ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Info: %s"),modemInfo.c_str());


}




/**
 * @brief Mostly to debug, but also return if it worked
 * 
 * @param buffer 
 * @return true 
 * @return false 
 */
bool mSIM800L::SendAT(const char* buffer, uint16_t wait_millis)
{
  if(!modem){ return false; }

  uint32_t tSaved_millis = millis();

  modem->sendAT(buffer);
  bool result = true;
  if(wait_millis)
  {
    result = modem->waitResponse(wait_millis);
  }
  uint32_t tSaved_Elapsed = millis()-tSaved_millis;

  ALOG_INF(PSTR(D_LOG_CELLULAR ">>>%s %s (%dms)"), buffer, result?"Success":"FAILED", tSaved_Elapsed);

  return result;
  
}


bool mSIM800L::SendAT_F(uint16_t wait_millis, PGM_P formatP, ...)
{

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);

  return SendAT(command_buffer, wait_millis);

}


bool mSIM800L::SendAT_ATParseResponse_F(uint16_t wait_millis, uint8_t response_loglevel, PGM_P formatP, ...)
{

  // AddLog(response_loglevel, PSTR("response_loglevel=%d"),response_loglevel);

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);
  
  if(!modem){ return false; }

  uint32_t tSaved_millis = millis();

  modem->sendAT(command_buffer);
  

  /**
   * @brief With timeout, expect response to be parsed
   * 
   */
  uint32_t tSaved_Elapsed = millis()-tSaved_millis;
  while(!Handler_ModemResponses(response_loglevel))
  {
    if(
      (tSaved_Elapsed = (millis()-tSaved_millis)) > wait_millis
    ){
      ALOG_ERR(PSTR(D_LOG_CELLULAR "No response (%dms)"), tSaved_Elapsed);
      return false;
    }

  }

  // ALOG_ERR(PSTR(D_LOG_CELLULAR "========================================Response (%dms)"), millis()-tSaved_millis);
  
  return true;

}







void mSIM800L::SMS_Enable()
{
    
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS_Enable++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"));

  if(modem)
  {
    
    SendATCommand_FunctionalityMode_Full();    

    SendATCommand_SMSFormatAscii();

    sms.enabled = true;
  }
}


void mSIM800L::SMS_Disable()
{
  
  sms.enabled = false;
}


void mSIM800L::ModemUpdate_SMS()
{
  


  if(mTime::TimeReached(&sms.tReached_Update, 1000))
  {
    if(sms.enabled)
    {
      // Although SMS should be forwarded, lets also check if any are waiting and read them out

      /**
       * @brief Check for waiting SMS messages
       **/
      SendAT_ATParseResponse_F(200, LOG_LEVEL_DEBUG_MORE, PSTR("+CMGD=?"));

    }
  }

}



void mSIM800L::SMSReadAndEraseSavedSMS()
{

  // #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- START %d"), sms.messages_incoming_index_list.size() );
  // #endif 

  for(auto& id:sms.messages_incoming_index_list)
  {
    if(SendAT_ATParseResponse_F(200, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), id))
    {
      // modem->waitResponse(200);
      // delay(200);
      
      // Erase from memory
      if(SendAT_F(200, PSTR("+CMGD=%d"), id))
      {
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d deleted"), id);
      }else{
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d FAILED TO delete"), id);

      }
    }
  }

  sms.messages_incoming_index_list.clear(); // for now assumed its done
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- START =======%d"), sms.messages_incoming_index_list.size() );

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- END"));
  #endif

}



void mSIM800L::SendATCommand_SMSFormatAscii()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=1"); // Set the ascii messages (not HEX)
  bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSFormatAscii %d"), result);
}


void mSIM800L::SendATCommand_SMSFormatPDU()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=0"); // Set the ascii messages (not HEX)
  bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSFormatPDU %d"), result);
}


/**
 * @brief Make it so new SMS messages are send over serial automatically
 * +CNMI: <mode>,<mt>,<bm>,<ds>,<bfr>
 * The read command parameters and their defined values are the following:
  <mode>
    0 – Do not forward unsolicited result codes to the Terminal Equipment (TE) (default)
    3 – Forward unsolicited result codes directly to the TE
  <mt>
    0 – No received message notifications, the modem acts as an SMS client
    2 – SMS-DELIVERs (except class 2 and message waiting indication group) are routed directly to the TE
  <bm>
    No CBM notifications are routed to the TE
  <ds>
    0 – No SMS-STATUS-REPORTs are routed to the TE
    1 – SMS-STATUS-REPORTs are routed to the TE using unsolicited result code: +CDS: <length><CR><LF><pdu>
  <bfr>
    1 – The buffer of unsolicited result codes is cleared when <mode> 1 to 3 is entered
**/
void mSIM800L::SendATCommand_SMSImmediateForwardOverSerial()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CNMI=3,2,0,0,0");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CNMI);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSImmediateForwardOverSerial %d"), result);
}



          #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
void mSIM800L::AutoSMS_Messages_Handle()
{

  /**
   * @brief Message1: Send GPS if requested
   * 
   */
  if(smsauto_gps_messages.rate_seconds>0) //if it set to auto
  {

    if(mTime::TimeReached(&smsauto_gps_messages.tSaved_LastSent, smsauto_gps_messages.rate_seconds*1000))
    {
      SMS_GPSLocationAuto();
    }

  }


}
          #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS






/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mSIM800L::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok2 = 0; 
  int8_t tmp_id = 0;

  
  if(jtok = obj["OpenHAB"].getObject()["SystemTime"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "OpenHAB:SystemTime"));
    ALOG_INF(PSTR(D_LOG_CELLULAR "OH Time %s"),jtok.getStr());
    //toggle led
    DIGITAL_INVERT_PIN(12); //esp32 blue led
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPS_Enable();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


          #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["SMSAuto_GPS"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMSAuto_GPS smsauto_gps_messages.rate_seconds %d"), jtok.getInt());
    smsauto_gps_messages.rate_seconds = jtok.getInt();
    
  }
          #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS



  if(jtok = obj["GPRS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPRS_Enable"));
    if(jtok.getInt() == 1)
    {
      DataNetwork__InitConfig();
    }
  }


  if(jtok = obj["SMS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMS_Enable"));
    if(jtok.getInt() == 1)
    {
      SMS_Enable();
    }
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["SMS_GPSLocation"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMS_GPSLocation"));
    if(jtok.getInt() == 1)
    {
      SMS_GPSLocation();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["SMSForward"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMSForward"));
    if(jtok.getInt() == 1)
    {
      SendATCommand_SMSImmediateForwardOverSerial();
    }
  }

  
  if(jtok = obj["BattVolt"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "BattVolt"));
    if(jtok.getInt() == 1)
    {    
      // read the battery voltage and percentage
      uint16_t vbat = modem->getBattVoltage();        
      ALOG_INF(PSTR(D_LOG_CELLULAR "%d mV"), vbat);
    }
  }

  if(jtok = obj["ModemInit"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ModemInit"));
    flag_modem_initialized = jtok.getInt();
  }

  if(jtok = obj["ADCVoltage"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ADCVoltage"));
    if(jtok.getInt() == 1)
    {      
      ModemUpdate_BatteryStatus();
      ALOG_INF(PSTR(D_LOG_CELLULAR "{\"volts_mv\":%d,\"percent\":%d,\"milliVolts\":%d}"),
        modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);
    }
  }


  if(jtok = obj["ReadSMS"]){

    ALOG_INF( PSTR(D_LOG_CELLULAR "ReadSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 

      modem->sendAT("+CMGR=0");

      //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_INF(PSTR(D_LOG_CELLULAR "ReadSMS %d"), result);
      

    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }

  
  if(jtok = obj["ATDebugCommand"])
  {

    if(jtok2 = jtok.getObject()["SMSReadOnly"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadOnly"));

      if(jtok2.isStr()) //Assumed worded group type
      {        
        SendAT_F(100, PSTR("+CMGR=\"%s\""), jtok2.getStr());
      }
      if(jtok2.isNum()) // Assumed index (From 0-max)
      {        
        SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), jtok2.getInt());
      }
    }

    if(jtok2 = jtok.getObject()["SMSReadAndClear"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadAndClear"));

      if(jtok2.isNum()) // Assumed index (From 0-max)
      {        
        if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), jtok2.getInt()))
        {
          // Clear it if it was successfully read
          SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
        }
      }
    }


    if(jtok2 = jtok.getObject()["SMSReadAndClearAll"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadAndClearAll"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        /**
         * @brief Placing in here now
         *  */
        // if(sms.messages_incoming_index_list.size())
        // {
        //   SMSReadAndEraseSavedSMS();
        // } 

      }
      
    }

    
  





    if(jtok2 = jtok.getObject()["SMSCheckWaitingIndexs"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSCheckWaitingIndexs"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        // Clear it if it was successfully read
        // SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
      }
      
    }


    if(jtok2 = jtok.getObject()["SMSClearAll"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSClearAll"));
      uint8_t flag = 4; // delete all
      SendAT_F(100, PSTR("+CMGR=%d[,%d]"), jtok2.getInt(), flag);
    }

  }

  if(jtok = obj["ReadDeleteSMS"]){

    ALOG_INF( PSTR(D_LOG_CELLULAR "ReadDeleteSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // SendAT("+CMGR=0[,4]", 100);

      // uint8_t flag = 4; // delete all

      // SendAT_F(PSTR("+CMGR=%d[,%d]"), index, flag);

      modem->sendAT("+CMGR=0[,4]");

// //+CMGD: (list_of_indexes)[,(list_of_flag_values)]

//       //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
//       ALOG_INF(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
      

      modem->sendAT("+CMGD=0");

      
       result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_INF(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }

      // modem->sendAT("AT+CMGL=\"ALL\"");

      // IPAddress local = modem->localIP();
      // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());
      
      // modem->sendAT("+CPMS?"); // How many SMS are waiting?
  // SendATCommand_SMSImmediateForwardOverSerial();



  if(jtok = obj["ATCommands"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ATCommands"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
      delay(500);
      if (SerialAT.available()) {
        String r = SerialAT.readString();
        ALOG_INF(PSTR(D_LOG_CELLULAR "ATResponse = \"%s\""), r.c_str());
      }
    }

  }

  if(jtok = obj["ATCommandsParsing"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ATCommandsParsing"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
      delay(500);

      uint32_t wait_millis = 1000;
      
      uint32_t tSaved_millis = millis();
      uint32_t tSaved_Elapsed = millis()-tSaved_millis;
      while(!Handler_ModemResponses(LOG_LEVEL_DEBUG))
      {
        if(
          (tSaved_Elapsed = (millis()-tSaved_millis)) > wait_millis
        ){
          ALOG_ERR(PSTR(D_LOG_CELLULAR "No response (%dms)"), tSaved_Elapsed);
        }

      }

    }

  }

    
}








bool mSIM800L::parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel)
{

  // #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_DBG(PSTR(D_LOG_CELLULAR "parse_ATCommands %d>> Buffer[%d] \"%s\""),response_loglevel, buflen, buffer);
  // #endif

  // +CDS:  A GSM/GPRS modem or mobile phone uses +CDS to forward a newly received SMS status report to the computer / PC.
  // +CDSI: A GSM/GPRS modem or mobile phone uses +CDSI to notify the computer / PC that a new SMS status report has been received and the memory location where it is stored.
  // +CMT:  A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
  // +CMTI: A GSM/GPRS modem or mobile phone uses +CMTI to notify the computer / PC that a new SMS message has been received and the memory location where it is stored.
  /**
   * @brief CMT is when messages are set to be forwarded, but may be missed, so leaving this as legacy to match messages
   **/
  char buffer2[300] = {0};
  char *search = "+CMT:"; // A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
  char *result = strstr(buffer, search);
  if(result)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMT\" result >>>\n\r%s\n\r<<<"), result);

    // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

    // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strcasecmp(buffer2, "GPS") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //   SMS_GPSLocation();
    // }else
    // if (strcasecmp(buffer2, "Batt") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
    //   SMS_BatteryDetailed();
    // }
    // else{
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //   SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


    // const char* delims = "\r\n"; //space + ,
    // char* tok = strtok(result, delims); // I believe all delims are replaced by NULL?

    // if(tok)
    // {  
    //   /**
    //   for (int i=0;i<5;i++)
    //   {
    //     ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
    //     tok = strtok(NULL, delims);
    //   }

    //   01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
    //   01:06 INF tok[1] = Message1
    //   01:06 INF tok[2] = OK
    //   01:06 INF tok[3] = ERROR
    //   01:06 INF tok[4] = ERROR
    //   **/
    

    //   tok = strtok(NULL, delims);   // SMS header
    //   // tok = strtok(NULL, delims); 
    //   char sms_contents[160];

    //   if(tok != NULL) 
    //   {   
    //     snprintf(sms_contents, sizeof(sms_contents), tok); 
    //     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

    //     #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    //     if (strcasecmp(sms_contents, "GPS") == 0)
    //     {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //       SMS_GPSLocation();
    //     }
    //     else
    //     if (strcasecmp(sms_contents, "Batt") == 0)
    //     {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
    //       SMS_BatteryDetailed();
    //     }
    //     else
    //     {
    //       ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //       SMS_GPSLocation();
    //     }
    //     #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    //   }

    // }


  }
  
  char *result2 = strstr(buffer, "+CMTI:"); 
  if(result2)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMTI\" result >>>\n\r%s\n\r<<<"), result2);


    const char* delims = ",\r"; //space + ,
    char* tokens = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tokens)
    {  
    tokens = strtok(NULL, delims); 
    int new_sms_index = atoi(tokens);

    /**
     * @brief Add unique name
     **/
    
    sms.messages_incoming_index_list.push_back(new_sms_index);
  ALOG_DBG(PSTR(D_LOG_CELLULAR "messages_incoming_index_list >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%d"), sms.messages_incoming_index_list.size() );
        DEBUG_LINE_HERE;
    }

 ALOG_DBG(PSTR(D_LOG_CELLULAR "messages_incoming_index_list >1>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%d"), sms.messages_incoming_index_list.size() );
 

    // /**
    //  * @brief Splitting the Indexs out
    //  **/
    // const char* delims2 = ","; //space + ,
    // char* tok_ids = strtok(buffer_id, delims2); 
    // uint8_t i = 0;
    // sms.messages_incoming_index_list.clear();
    // while(tok_ids){
    //   ALOG_INF(PSTR("tok_ids[%d] = %s"), i++, tok_ids?tok_ids:"ERROR");
    //   sms.messages_incoming_index_list.push_back(atoi(tok_ids));
    //   tok_ids = strtok(NULL, delims2);
    // }



    // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

    // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strcasecmp(buffer2, "GPS") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //   SMS_GPSLocation();
    // }
    // else{
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //   SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  }

  /**
   * @brief 
   * +CPMS: used_space1,max_space1,used_space2,max_space2,used_space3,max_space3
   * used_space1 is the number of messages currently stored in the message storage area specified by the message_storage1 parameter.
   * max_space1 is the maximum number of messages that can be stored in the message storage area specified by the message_storage1 parameter. 
   */
  result2 = strstr(buffer, "+CPMS:"); 
  if(result2)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "MATCHED \"CPMS\" >>%s<<"), result2);

    const char* delims = ","; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {  
    tok = strtok(NULL, delims); 
    int used_space1 = atoi(tok);
    tok = strtok(NULL, delims); 
    int max_space1  = atoi(tok);

    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Stored = %d/%d"), used_space1, max_space1);
    }

    // tok


    // for (int i=0;i<5;i++)
    // {
    //   ALOG_INF(PSTR("tok[%d] = %s"), i, tok);
    //   tok = strtok(NULL, delims);
    // }

  /**
   *                           +CPMS: (storage1a,storage1b,...),(storage2a,storage2b,...),(storage3a,storage3b,...)
   * INF CEL: MATCHED "CPMS" >>+CPMS: "SM",5,10,"SM",5,10,"SM",5,10
   * OK<<
   * 16:35 INF tok[0] = 5
   * 16:35 INF tok[1] = 10
   * 16:35 INF tok[2] = "SM"
   * 16:35 INF tok[3] = 5
   * 16:35 INF tok[4] = 10 
   */



    // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

    // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // // Check for request at GPS
    // if (strcasecmp(buffer2, "GPS") == 0)
    // {
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
    //   SMS_GPSLocation();
    // }
    // else{
    //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
    //   SMS_GPSLocation();
    // }
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

  }

  result2 = strstr(buffer, "+CMGR:"); 
  if(result2)
  {
    ALOG_HGL(PSTR(D_LOG_CELLULAR "MATCHED \"CMGR\"\n\r>>%s<<"), result2);

    const char* delims = "\r\n"; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {  
      /**
      for (int i=0;i<5;i++)
      {
        ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
        tok = strtok(NULL, delims);
      }

      01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
      01:06 INF tok[1] = Message1
      01:06 INF tok[2] = OK
      01:06 INF tok[3] = ERROR
      01:06 INF tok[4] = ERROR
      **/
    

      tok = strtok(NULL, delims);   // SMS header
      // tok = strtok(NULL, delims); 
      char sms_contents[160];

      if(tok != NULL) 
      {   
        snprintf(sms_contents, sizeof(sms_contents), tok); 
        ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        if (strcasecmp(sms_contents, "GPS") == 0)
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
          SMS_GPSLocation();
        }
        else
        if (strcasecmp(sms_contents, "Batt") == 0)
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
          SMS_BatteryDetailed();
        }
        else
        if (strstr(sms_contents, "Command") != NULL)
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "Future Command with format \"Command KEY VALUE\" that will be shaped into JSON and send to parser"));
          SMS_CommandIntoJSONCommand(sms_contents);
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
          SMS_GPSLocation();
        }
        #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

      }

    }

  }


  if(strstr(buffer, "+CMGD:"))
  {
    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage[%d] %s"), buflen , buffer);
    #endif
    ATParse_CMGD__CommandNameInTextDeleteMessage(buffer, buflen, response_loglevel);
  }

}



void mSIM800L::ATParse_CMGD__CommandNameInTextDeleteMessage(char* buffer, uint8_t buflen, uint8_t response_loglevel)
{

  ALOG_DBG(PSTR(D_LOG_CELLULAR "ATParse_CMGD__CommandNameInTextDeleteMessage messages_incoming_index_list %d"), sms.messages_incoming_index_list.size() );
  char* result2 = strstr(buffer, "+CMGD:"); 
  if(result2)
  {
    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage S ==========\n\r%s\n\r=================="),result2);
    #endif

    const char* delims = "()"; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {                 
      // 00:23 INF tok[0] = +CMGD: 
      // 00:23 INF tok[1] = 0,1,2,4,5
      // 00:23 INF tok[2] = ,
      // 00:23 INF tok[3] = 0-4
      // 00:23 INF tok[4] =       

      ALOG_INF(PSTR(D_LOG_CELLULAR "MATCHED \"CMGD\" >>%s<<"), result2);

      response_loglevel = LOG_LEVEL_DEV_TEST;

      tok = strtok(NULL, delims); //skip 

      if(tok)
      {

        char buffer[100];
        snprintf(buffer, sizeof(buffer), tok); // 0,1,2,4,5
        AddLog(response_loglevel,PSTR("hereeeeeeeeeeeeeeeeeeeeeeeeee buffer split = %s"), buffer);



        // I see the issue:
        // * I need to check when its multiple numbers, separated by comma, or
        // * a range Separated by a dash, then iter between the low and high of dash

        /**
         * @brief Splitting the Indexs out
         **/
        const char* delims2 = ","; //space + ,
        char* tok_ids = strtok(buffer, delims2); 
        uint8_t i = 0;
        sms.messages_incoming_index_list.clear();
        while(tok_ids){
          AddLog(response_loglevel,PSTR("CommandNameInTextDeleteMessage SMS IDs[%d] = %s"), i++, tok_ids?tok_ids:"ERROR");
          sms.messages_incoming_index_list.push_back(atoi(tok_ids));
          tok_ids = strtok(NULL, delims2);
        }

        DEBUG_LINE_HERE;

      }

    }else{
      ALOG_ERR(PSTR(D_LOG_CELLULAR "ATParse_CMGD__CommandNameInTextDeleteMessage No tok"));
    
    }
  }
ALOG_DBG(PSTR(D_LOG_CELLULAR "ATParse_CMGD__CommandNameInTextDeleteMessageE messages_incoming_index_list %d"), sms.messages_incoming_index_list.size() );
  
    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage E"));
    #endif

}









#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

void mSIM800L::GPS_Enable()
{
  if(modem)
  {
    // Set SIM800L GPIO4 LOW ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,1");
    if(modem->waitResponse(10000L) != 1) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,1 false "));
    }

    modem->enableGPS();
    gps.enabled = true;
  }
}


void mSIM800L::GPS_Disable()
{
  // Set SIM800L GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem->sendAT("+SGPIO=0,4,1,0");
  if (modem->waitResponse(10000L) != 1) {
    ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,0 false "));
  }
  modem->disableGPS();
  gps.enabled = false;
}


void mSIM800L::ModemUpdate_GPS()
{
  if(mTime::TimeReached(&gps.tReached_Update, 1000)) // Not sending any faster so no point reading it faster
  {
    if(gps.enabled)
    {
      
      if(modem->getGPS_Detailed(
        &gps.latitude, 
        &gps.longitude, 
        &gps.speed, 
        &gps.altitude, 
        &gps.vsat, 
        &gps.usat, 
        &gps.accuracy, 
        &gps.year, 
        &gps.month, 
        &gps.day, 
        &gps.hour, 
        &gps.minute, 
        &gps.second, 
        &gps.course, 
        &gps.FixMode, 
        &gps.accuracy_position, 
        &gps.accuracy_vertical, 
        &gps.usat_glonass, 
        &gps.cno_max, 
        &gps.HPA, 
        &gps.VPA
      )) 
      {
        // ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100));
        
        #ifdef USE_MODULE_SENSORS_GPS_MODEM
        pCONT_gps->readings.update_seconds = millis();
        pCONT_gps->location.latitude  = gps.latitude;
        pCONT_gps->location.longitude = gps.longitude;
        pCONT_gps->location.speed = gps.speed;
        pCONT_gps->location.altitude = gps.altitude;
        pCONT_gps->location.accuracy = gps.accuracy;
        ALOG_INF(PSTR("CEL: Updating devices location (%d)"), (int)(pCONT_gps->location.accuracy*100));
        #endif // USE_MODULE_SENSORS_GPS_MODEM

        #ifdef ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
        uint16_t minimum_update_seconds = 0; //always
        tkr_time->SystemTime_Update(gps.year, gps.month, gps.day, gps.hour, gps.minute, gps.second, minimum_update_seconds);
        #endif
      
      }else{
        ALOG_ERR(PSTR(D_LOG_CELLULAR "GPS NO FIX"));
        // GPS_Enable(); //need a backoff timer but otherwise should force it
      }

            
        
    }
  }

}




// https://www.google.com/maps?ll=xx.x,-x.xx
void mSIM800L::SMS_GPSLocation()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  pCONT_mavlink->pkt.battery_status.data.battery_remaining, pCONT_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), pCONT_mavlink->pkt.mission_current.data.seq, pCONT_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-pCONT_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}


// https://www.google.com/maps?ll=xx.x,-x.xx
void mSIM800L::SMS_GPSLocationAuto()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  pCONT_mavlink->pkt.battery_status.data.battery_remaining, pCONT_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), pCONT_mavlink->pkt.mission_current.data.seq, pCONT_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-pCONT_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("AutoSend %d secs\n"),  smsauto_gps_messages.rate_seconds);
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



/**
 * @brief New method that takes in SMS formatted as "Command:Key:Value" so test case is Command:SMS_GPS_Auto:1 or Command:SMS_GPS_Something:"string", ie key is assumed string but value must have its type added, "string", number, [array]
 * 
 * @param sms_command 
 */
void mSIM800L::SMS_CommandIntoJSONCommand(char* sms_command)
{

  /**
   * @brief SMS message will contain the repeat rate, and its handled in another call (in seconds)
   * 
   */
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_command >>%s<<"), sms_command);

  const char* delims = ":"; //space + , "GPSAuto 123" where the space splits the command and number
  char* tok = strtok(sms_command, delims); // I believe all delims are replaced by NULL?

  if(tok)
  {  
    // for (int i=0;i<5;i++)
    // {
    //   ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
    //   tok = strtok(NULL, delims);
    // }


    // tok = strtok(NULL, delims); // "Command" skip
    
    tok = strtok(NULL, delims); // Consume first split string and advance token (i.e. skip Command)
    char key[100];
    snprintf(key, sizeof(key), tok);
    ALOG_INF(PSTR(D_LOG_CELLULAR "key=%s"),key);

    tok = strtok(NULL, delims);     
    char value[100];
    snprintf(value, sizeof(value), tok);
    ALOG_INF(PSTR(D_LOG_CELLULAR "value=%s"),value);

    // Read into local
    D_DATA_BUFFER_CLEAR();

    sprintf(data_buffer.payload.ctr, "{\"%s\":%s}", key, value);

    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

    ALOG_COM( PSTR(DEBUG_INSERT_PAGE_BREAK  "SMS->JsonCommandBuffer = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);

    pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

    ALOG_INF(PSTR(D_LOG_CELLULAR "JsonCommand Finished"));

  }

    // tok




}




void mSIM800L::SMS_BatteryDetailed()
{

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending Battery"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  pCONT_mavlink->pkt.battery_status.data.battery_remaining, pCONT_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), pCONT_mavlink->pkt.mission_current.data.seq, pCONT_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-pCONT_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



// void mSIM800L::SMS_GPSLocation()
// {
//   if(modem)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
//     // --------TESTING SENDING SMS--------

//     // https://www.google.com/maps/dir//54.505,-6.299
//     // https://www.google.com/maps/dir//54.505044,-6.298891

//     char convf_lat[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
//     char convf_lon[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
//     char convf_fix[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.accuracy,2,convf_fix);

//     uint16_t buflen = 0;
//     char     buffer[STANDARD_SMS_CHAR_LENGTH] = {0};

// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
//       PSTR(
//         "Batt %d%% %dmV\n"
//         "WP  %d (%dm)\n"
//         "PKT Age  %d\n"
//         "Accuracy %s m\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       pCONT_mavlink->pkt.battery_status.data.battery_remaining,
//       pCONT_mavlink->pkt.battery_status.data.current_consumed,

//       pCONT_mavlink->pkt.mission_current.data.seq,
//       pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,

//       millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
//       convf_fix,
//       convf_lat, 
//       convf_lon
//     );
//     // buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     //   PSTR(
//     //     "Accuracy %s m\n"
//     //     "https://www.google.com/maps/dir//%s,%s"
//     //   ), 
//     //   convf_fix,
//     //   convf_lat, 
//     //   convf_lon
//     // );
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferA%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//     /**
//      * @brief MAVLink Data
//      **/    
//     float mavlink_lat = pCONT_mavlink->pkt.gps_raw_int.data.lat/10000000;
//     float mavlink_lon = pCONT_mavlink->pkt.gps_raw_int.data.lon/10000000;


//     char convf_lat2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
//     char convf_lon2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lat2 \"%s\""),convf_lat2);
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lon2 \"%s\""), convf_lon2);
// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//       PSTR(
//         "\n"
//         "MAV\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       convf_lat2, 
//       convf_lon2
//     );
//     #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferB%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     ALOG_INF(PSTR(D_LOG_CELLULAR "buffer \"%s\""), buffer);
    
// // DEBUG_LINE_HERE;
//     String res;
//     res = modem->sendSMS(SMS_TARGET, String(buffer));
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

// // DEBUG_LINE_HERE;
//   }
// }

void mSIM800L::SMS_Send_TimedHeartbeat()
{
  DEBUG_LINE_HERE;
  if(modem)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
    // --------TESTING SENDING SMS--------

    // https://www.google.com/maps/dir//54.505,-6.299
    // https://www.google.com/maps/dir//54.505044,-6.298891

  DEBUG_LINE_HERE;
    char convf_lat[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    char convf_lon[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    char convf_fix[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.accuracy,2,convf_fix);

  DEBUG_LINE_HERE;
    uint16_t buflen = 0;
    char     buffer[STANDARD_SMS_CHAR_LENGTH];

  DEBUG_LINE_HERE;
    buflen += snprintf_P(buffer+buflen, sizeof(buffer),
      PSTR(
        // "Battery  %d mV\n"
        // "Battery  %d mA\n"
        // "Mission  %d (%dm)\n"
        // "PKT Age  %d\n"
        "Timed Heartbeat\n"
        "Accuracy %s m\n"
        "\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      // pCONT_mavlink->pkt.battery_status.data.battery_remaining,
      // pCONT_mavlink->pkt.battery_status.data.current_consumed,
      // pCONT_mavlink->pkt.mission_current.data.seq,
      // pCONT_mavlink->pkt.nav_controller_output.data.wp_dist,
      // millis()-pCONT_mavlink->pkt.tSaved_Last_Response,
      convf_fix,
      convf_lat, 
      convf_lon
    );

  DEBUG_LINE_HERE;
    #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
    /**
     * @brief MAVLink Data
     **/    
    char convf_lat2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
    char convf_lon2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(pCONT_mavlink->pkt.gps_raw_int.data.lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

    // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
    //   PSTR(
    //     "\n"
    //     "MAV\n"
    //     "https://www.google.com/maps/dir//%s,%s"
    //   ), 
    //   convf_lat2, 
    //   convf_lon2
    // );
    #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
  DEBUG_LINE_HERE;
    ALOG_INF(PSTR(D_LOG_CELLULAR "buffer  %s"),buffer);
    
  DEBUG_LINE_HERE;
    String res;
    res = modem->sendSMS(SMS_TARGET, String(buffer));
  DEBUG_LINE_HERE;
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");
  DEBUG_LINE_HERE;

  }
}

#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS







// General Packet Radio Service



void mSIM800L::GPRS_Disable()
{
  
  gprs.enabled = false;
}


void mSIM800L::ModemUpdate_GPRS()
{

  if(mTime::TimeReached(&gprs.tReached_Update, 20))
  {
    
    if(gprs.enabled)
    {
      
      /**
       * @brief Check connection is still active
       */              
      
      if (modem->isGprsConnected()) 
      {

        GPRS_UpdateConnectionState(true);

        gprs.signal_quality_raw = modem->getSignalQuality();

        if(gprs.signal_quality_raw == 99)
        {
          gprs.signal_quality_rssi_dbm = -150;
          pCONT_interface_network->data.cellular_state.isvalid = false;
          pCONT_interface_network->data.mqtt_state.isvalid = false;
        }
        else
        {
          gprs.signal_quality_rssi_dbm = mSupport::mapfloat(gprs.signal_quality_raw, 0, 31, -113, -51);
          pCONT_interface_network->data.cellular_state.isvalid = true;
        }

        ALOG_DBM(PSTR(D_LOG_CELLULAR "GPRS: Connected %d dBm (%d)"), int(gprs.signal_quality_rssi_dbm), gprs.signal_quality_raw);

        gprs.last_comms_millis_updated = millis();

            
        // String ccid = modem->getSimCCID();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

        // String imei = modem->getIMEI();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

        // String cop = modem->getOperator();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

        // IPAddress local = modem->localIP();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

      } 
      else 
      {
        GPRS_UpdateConnectionState(false);
        ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS status: NOT connected Downtime: %d secs"), gprs.downtime_secs);
        DataNetwork__StartConnection();
      }

    }
    else
    {
      DataNetwork__InitConfig();
    }

  }

}


/**
 * @brief Function is called anytime the connection is checked, and will update the saved variable but configure flags when needed
 * 
 * @param state 
 */
void mSIM800L::GPRS_UpdateConnectionState(bool state)
{

  /**
   * @brief State changed
   **/
  if(state != gprs.connected)
  {

    #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE
    if(state)
    {
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(TASK_CELLULAR_CONNECTION_ESTABLISHED));
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(TASK_NETWORK_CONNECTION_ESTABLISHED));
  DEBUG_LINE_HERE;
      gprs.reconnect_init_counts++;
    }
    else
    {
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(TASK_CELLULAR_CONNECTION_LOST));
  DEBUG_LINE_HERE;
      pCONT->function_event_queue.push_back(pCONT->FunctionEvent(TASK_NETWORK_CONNECTION_LOST));
  DEBUG_LINE_HERE;
    }
    #endif //ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

    gprs.connected = state; // Save new state
  }

  if(state)
  {
    gprs.connected_seconds++;
    gprs.downtime_secs = 0;
  }
  else
  {
    gprs.connected_seconds = 0;
    gprs.downtime_secs++;
  }

}
















/**
 * @brief Check for active comms on the serial line
 * 
 */
bool mSIM800L::Modem__PowerUntilRunning(uint16_t wait_millis)
{

  Serial.println("> Check whether Modem is online");
    //test modem is online ?
    uint32_t  timeout = millis();
    while (!Modem__Running(100)) {
        Serial.print(".");
        if (millis() - timeout > wait_millis ) {
            Serial.println("> It looks like the modem is not responding, trying to restart");
            modemPowerOff();
            delay(5000);
            modemPowerOn();
            timeout = millis();
        }
    }
    Serial.println("\nModem is online");

}


/**
 * @brief Check for active comms on the serial line
 * 
 */
bool mSIM800L::Modem__Running(uint16_t wait_millis)
{

  ALOG_INF(PSTR("mSIM800L::Modem__Running"));
  bool modem_responded = false;
  modem_responded = modem->testAT(wait_millis);
  ALOG_INF(PSTR("modem_responded = %d"), modem_responded);
  return modem_responded;

}


/**
 * @brief Commands required after restart
 * 
 */
bool mSIM800L::SimNetwork__InitConfig()
{


}

/**
 * @brief Commands required to connect
 * 
 */
bool mSIM800L::SimNetwork__StartConnection()
{


}

/**
 * @brief Check status of connection
 * 
 */
bool mSIM800L::SimNetwork__CheckConnection()
{


}


/**
 * @brief Commands required after restart
 * 
 */
bool mSIM800L::DataNetwork__InitConfig()
{
  
  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__InitConfig()"));


    //Set mobile operation band
    modem->sendAT("+CBAND=ALL_MODE");
    modem->waitResponse();

    // Args:
    // 1 CAT-M
    // 2 NB-IoT
    // 3 CAT-M and NB-IoT
    // Set network preferre to auto
    // modem->setPreferredMode(3);

    // Args:
    // 2 Automatic
    // 13 GSM only
    // 38 LTE only
    // 51 GSM and LTE only
    // Set network mode to auto
    // modem->setNetworkMode(2);

    uint32_t  timeout = millis();
    // Check network signal and registration information
    // ALOG_INF(PSTR(D_LOG_CELLULAR "> SIM7000/SIM7070 uses automatic mode to access the network. The access speed may be slow. Please wait patiently"));
    RegStatus status;
    timeout = millis();
    do {
      int16_t sq =  modem->getSignalQuality();

      status = modem->getRegistrationStatus();

      if (status == REG_DENIED) {
        ALOG_INF(PSTR(D_LOG_CELLULAR "The SIM card you use has been rejected by the network operator"));
        return false;
      } else {
        ALOG_INF(PSTR(D_LOG_CELLULAR "Signal %d dBm, %d, pause %d"), (int)GetSignalQualityPower(sq), status, 10000-(timeout-millis()) );
      }

      if (millis() - timeout > 10000 ) { //!! THIS IS BLOCKING CODE, NEEDS RESOLVED TO RETRY AGAIN
        if (sq == 99) {
          ALOG_INF(PSTR(D_LOG_CELLULAR "It seems that there is no signal."));
          return false;
        }
        timeout = millis();
      }

      delay(50);
      // DEBUG_LINE_HERE;
    } while (status != REG_OK_HOME && status != REG_OK_ROAMING);


    String res;

    ALOG_INF(PSTR(D_LOG_CELLULAR "Obtain the APN issued by the network"));
    modem->sendAT("+CGNAPN");
    if (modem->waitResponse(3000, res) == 1) 
    {
      res = res.substring(res.indexOf(",") + 1);
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      res.replace("OK", "");
      ALOG_INF(PSTR(D_LOG_CELLULAR "The APN issued by the network is: %s"), res.c_str());
    }

    modem->sendAT("+CNACT=1");
    modem->waitResponse();

    modem->sendAT("+CNACT?");
    // modem->waitResponse();
    if (modem->waitResponse("+CNACT: ") == 1) 
    {
      modem->stream.read();
      modem->stream.read();
      res = modem->stream.readStringUntil('\n');
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network IP address is: %s"), res.c_str());
    }

    
    gprs.enabled = true;

}

/**
 * @brief Commands required to connect
 * 
 */
bool mSIM800L::DataNetwork__StartConnection()
{

  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__StartConnection()"));

  if(modem)
  {

    ALOG_INF(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Waiting for CELL TOWER network..."));
    if (!modem->waitForNetwork())  // Sim to cell tower connection, not the gprs
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Timeout?? I need to connect here??Waiting for network..."));
      // if this was false, I should really restart the cell connection
      // SimNetwork_Connect(); // might take long, so I need a "re-enable"
      return false;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Network connected"));
    }else{
      
    }

    
    modem->gprsDisconnect();
    delay(50);


    ALOG_INF(PSTR(D_LOG_CELLULAR "Connecting to: %s"), CELLULAR_APN);
    bool gprs_connection_successful = modem->gprsConnect(CELLULAR_APN, CELLULAR_GPRS_USERNAME, CELLULAR_GPRS_PASSWORD);

    if(gprs_connection_successful)
    {
      ALOG_INF(PSTR("modem->gprsConnect                 YEEEEEEEEEEEEEEEEEEEES"));
      gprs.apn_connect_called++;
    }else{
      ALOG_INF(PSTR("modem->gprsConnect                 NOOOOOOOOOOOOOOOOOOOOT"));

    }


    if (!gprs_connection_successful) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS !gprs_connection_successful..."));
      return false;
    }

    ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Checking..."));
    if (modem->isGprsConnected()) 
    {
      DEBUG_LINE_HERE;
      #ifdef ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Connected"));
      ALOG_HGL(PSTR("Start MQTTConnection with Cellular"));

      /**
       * @brief The idea is on a connection, the new connection should be added to the network interface
       * For now, set the mqtt directly
       **/
      if(gsm_client != nullptr)
      {
        gsm_client->stop();
        // gsm_client->setTimeout
        delay(50); //flush time
      }
      if(gsm_client == nullptr)
      {
        gsm_client = new TinyGsmClient(*modem); // Only create new if it didnt already exist
      }
      pCONT_mqtt->CreateConnection(gsm_client, MQTT_HOST_CELLULAR, MQTT_PORT_CELLULAR, CLIENT_TYPE_CELLULAR_ID);
        


      #endif // ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      
      DEBUG_LINE_HERE;
      GPRS_UpdateConnectionState(true);
      DEBUG_LINE_HERE;


    } 
    else 
    {
      ALOG_HGL(PSTR(D_LOG_CELLULAR "GPRS NOT Connected: REINIT THE LAZY WAY"));
      
      // Init();
    }

    // String ccid = modem->getSimCCID();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

    // String imei = modem->getIMEI();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

    // String cop = modem->getOperator();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

    // IPAddress local = modem->localIP();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

    // int csq = modem->getSignalQuality();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Signal quality: %d"), csq);

  }

    String res;
    modem->sendAT("+CPSI?");
    // modem->waitResponse();
    if (modem->waitResponse("+CPSI: ") == 1) 
    {
      res = modem->stream.readStringUntil('\n');
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network parameter is: %s"), res.c_str());
    }

}


/**
 * @brief Check status of connection
 * 
 */
bool mSIM800L::DataNetwork__CheckConnection()
{


}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSIM800L::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("SMSEnabled", sms.enabled);
    
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Add("SMSAutoPosition", smsauto_gps_messages.rate_seconds);
    #endif 

    JBI->Add("GPRSEnabled", gprs.enabled);
    
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Add("GPSEnabled", gps.enabled);
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    
  return JBI->End();

}


uint8_t mSIM800L::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    JBI->Add("uptime", tkr_time->uptime_seconds_nonreset);
    JBI->Add("rtcseconds", tkr_time->RtcTime.second);
    JBI->Object_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
        JBI->Add("rss_dbm", gprs.signal_quality_rssi_dbm);
    JBI->Object_End();
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Object_Start("GPS");
      JBI->Add("second", gps.second);
      JBI->Add("Speed", gps.speed);
      JBI->Add("altitude", gps.altitude);
      JBI->Add("Latitude", gps.latitude);
      JBI->Add("Longitude", gps.longitude);
      JBI->Add("Enabled", gps.enabled);
      JBI->Add("vsat", gps.vsat);
      JBI->Add("usat", gps.usat);
      JBI->Add("accuracy", gps.accuracy);
      JBI->Add("year", gps.year);
      JBI->Add("month", gps.month);
      JBI->Add("day", gps.day);
      JBI->Add("hour", gps.hour);
      JBI->Add("minute", gps.minute);
      JBI->Add("course", gps.course);
      JBI->Add("FixMode", gps.FixMode);
      JBI->Add("accuracy_position", gps.accuracy_position);
      JBI->Add("accuracy_vertical", gps.accuracy_vertical);
      JBI->Add("usat_glonass", gps.usat_glonass);
      JBI->Add("cno_max", gps.cno_max);
      JBI->Add("HPA", gps.HPA);
      JBI->Add("VPA", gps.VPA);

      char convf_lat[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
      char convf_lon[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
      char convf_fix[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.accuracy,2,convf_fix);

      snprintf_P(buffer, sizeof(buffer),
        PSTR(
          "https://www.google.com/maps/dir//%s,%s"
        ),
        convf_lat, 
        convf_lon
      );

      JBI->Add("url", buffer);
    JBI->Object_End();
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Object_Start("Battery");
      JBI->Add("Volts_mv", modem_status.battery.volts_mv);
      JBI->Add("Percentage", modem_status.battery.percentage);
      JBI->Add("ChargeState", modem_status.battery.charge_state);
      JBI->Add("Valid", modem_status.battery.isvalid);
      JBI->Add("analog", analogRead(35) );
    JBI->Object_End();
  
  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSIM800L::MQTTHandler_Init()
{

  struct handler<mSIM800L>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSIM800L::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mSIM800L::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSIM800L::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSIM800L::MQTTHandler_Rate()
{
  // While developing, set these by the Init() function to make independent

  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mSIM800L::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT



#endif
