#include "2_CoreSystem/08_Logging/mLogging.h"

void AddLog(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, PGM_P formatP, ...){
  if(ABS_FUNCTION(millis()-*tSaved)>=limit_ms){ *tSaved=millis();
    va_list arg;
    //AddLog(loglevel,formatP,arg);
  }
}


void ErrorMessage_P(uint8_t error_type, const char* message)
{
  AddLog(LOG_LEVEL_ERROR, PSTR("Invalid Format: %S"), message);
}


void ErrorMessage(uint8_t error_type, const char* message)
{
  AddLog(LOG_LEVEL_ERROR, PSTR("Invalid Format: %s"), message);
}


void AddLog(uint8_t loglevel, PGM_P formatP, ...)
{

  #ifdef DEBUG_FOR_FAULT
    tkr_set->Settings.logging.serial_level = LOG_LEVEL_ALL;
  #endif
  // SERIAL_DEBUG.printf("%s %d\r\n","START",millis()); 

  // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
  if(
    (loglevel>tkr_set->Settings.logging.serial_level)&&
    (loglevel>tkr_set->Settings.logging.web_level)
    ){
    return;
  }
  
  // Filtering
  if(tkr_set->runtime.enable_serial_logging_filtering){ // if true, only permit exact log level and not all above
    if(loglevel == tkr_set->Settings.logging.serial_level){
      //permit messages
    }else{
      return;
    }
  }
  if(tkr_set->runtime.enable_web_logging_filtering){ // if true, only permit exact log level and not all above
    if(loglevel == tkr_set->Settings.logging.web_level){
      //permit messages
    }else{
      return;
    }
  }

  // SERIAL_DEBUG.printf("%s %d\r\n","before vsn",millis());

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(pCONT_log->log_data, sizeof(pCONT_log->log_data), formatP, arg);
  va_end(arg);

  char mxtime[25];  // "13:45:21 " //9

  /**
   * Change to different log layouts, using a switch case. Each method (serial/web etc) can use different formats
   * */

  memset(mxtime,0,sizeof(mxtime));
  // if time is short, ie debugging, them only show uptime (not RTCTime)
  // if(tkr_set->Settings.logging.time_isshort){
  //   #ifdef ENABLE_FEATURE_LOGGING__INCLUDE_RTC_IN_LOGS
  //     // Only show hour
  //     if(tkr_time->uptime.hour<1){
  //       snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d-%02d:%02d "), tkr_time->RtcTime.hour,tkr_time->RtcTime.minute,tkr_time->RtcTime.second, tkr_time->uptime.minute,tkr_time->uptime.second);
  //     }else{
  //       snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d-%02d:%02d:%02d "), tkr_time->RtcTime.hour,tkr_time->RtcTime.minute,tkr_time->RtcTime.second, tkr_time->uptime.hour,tkr_time->uptime.minute,tkr_time->uptime.second);
  //     }
  //   #else
  //     if(tkr_time->uptime.hour<1){
  //       snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d "),
  //         tkr_time->uptime.minute,tkr_time->uptime.second
  //       );
  //     }else{
  //       snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d "), //add hour
  //         tkr_time->uptime.hour,tkr_time->uptime.minute,tkr_time->uptime.second);
  //     }
  //   #endif
    
  // }else{
    snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %s "),
    // sprintf(mxtime, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %02dT%02d:%02d:%02d "),
      tkr_time->RtcTime.hour,tkr_time->RtcTime.minute,tkr_time->RtcTime.second,
      tkr_time->GetUptime().c_str());
  // }

  // SERIAL_DEBUG.printf("%s %d\r\n","serail",millis());

  bool isconnected = false;
  #ifdef USE_MODULE_NETWORK_WIFI
  isconnected = pCONT_wif->connection.fConnected;
  #endif

  #ifndef DISABLE_SERIAL_LOGGING
  // LOG : SERIAL
  if (loglevel <= tkr_set->Settings.logging.serial_level) 
  {
    #ifdef ENABLE_FREERAM_APPENDING_SERIAL
      // register uint32_t *sp asm("a1"); 
      // SERIAL_DEBUG.printf("R%05d S%04d U%02d%02d %s %s\r\n", 
      SERIAL_DEBUG.printf(PSTR("R%05d%c %s %S %s\r\n"), 
        ESP.getFreeHeap(), // 4 * (sp - g_pcont->stack), 
        isconnected ? 'Y' : 'N',
        tkr_time->GetUptime().c_str(),
        pCONT_log->GetLogLevelNamebyID(loglevel),
        pCONT_log->log_data
      );
      
      if(loglevel == LOG_LEVEL_DEBUG_LOWLEVEL)
      {
        SERIAL_DEBUG.flush();
      }
    #else

      if(loglevel == LOG_LEVEL_HIGHLIGHT){ SERIAL_DEBUG.printf("\n\r\n\r>>HIGHLIGHT START<<\n\r\n\r"); }
      #ifdef ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS
      if(loglevel == LOG_LEVEL_ERROR){ SERIAL_DEBUG.printf("\n\rERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r"); }
      #endif // ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS


      SERIAL_DEBUG.printf(PSTR("%s%S %s\r\n"), mxtime, pCONT_log->GetLogLevelNamebyID(loglevel), pCONT_log->log_data);

      if(loglevel == LOG_LEVEL_HIGHLIGHT){ SERIAL_DEBUG.printf("\n\r\n\r>>HIGHLIGHT END<<\n\r\n\r"); }
      #ifdef ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS
      if(loglevel == LOG_LEVEL_ERROR){ SERIAL_DEBUG.printf("\n\rERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r"); }
      #endif // ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS

    #endif
    //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
    // Only flush if we all doing all for debugging
    
    //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
    // IMPORTANT!!! The code will pause here if flush is set, only for ms until the serial print has been sent
    // Normally, serial is passed to hardware internal the the chip, and serial is printed in the background. However, if a problem/bug with forced reseting exists,
    // you want to print all serial BEFORE tripping the reset, so only enable when fault tracing
    // #ifdef ENABLE_SERIAL_DEBUG_FLUSH
    #ifdef ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT
    SERIAL_DEBUG.flush(); // To ensure all serial is sent before a reset
    #endif

    if((tkr_set->Settings.logging.serial_level == LOG_LEVEL_DEBUG)||(tkr_set->Settings.logging.serial_level == LOG_LEVEL_ALL)){ SERIAL_DEBUG.flush(); } 
  } 
  #endif //DISABLE_SERIAL_LOGGING

  #ifdef ENABLE_SERIAL_FLUSH
    SERIAL_DEBUG.flush(); 
  #endif 

  // LOG : TELNET
  if (loglevel <= tkr_set->Settings.logging.telnet_level) 
  {    
    if(pCONT_log->Telnet)
    {
      pCONT_log->Telnet.printf( "%s%S %s\r\n", mxtime, pCONT_log->GetLogLevelNamebyID(loglevel), pCONT_log->log_data);
    }
  }

  // LOG : WEBSERVER
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  // if(pCONT_web->fConsole_active && !pCONT_web->fConsole_history){ //only append values when active, however, this stops history
  //   if (tkr_set->Settings.webserver && (loglevel <= tkr_set->Settings.logging.web_level)) {
  //     // Delimited, zero-terminated buffer of log lines.
  //     // Each entry has this format: [index][log data]['\1']
  //     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
      
  //     while (web_log_index == web_log[0] ||  // If log already holds the next index, remove it
  //           strlen(web_log) + strlen(log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
  //     {
  //       char* it = web_log;
  //       it++;                                  // Skip web_log_index
  //       it += pCONT_sup->strchrspn(it, '\1'); // Skip log line
  //       it++;                                  // Skip delimiting "\1"
  //       // circle uffer
  //       memmove(web_log, it, WEB_LOG_SIZE -(it-web_log));  // Move buffer forward to remove oldest log line
  //     }
  //     // creates line formatted with \1 meaning EOL
  //     snprintf_P(web_log, sizeof(web_log), PSTR("%s%c%s%S %s\1"), web_log, web_log_index++, mxtime, pCONT_log->GetLogLevelNamebyID(loglevel), log_data);
  //     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
    
  //   }
  // }
  #endif  // USE_MODULE_NETWORK_WEBSERVER

  
  // LOG : MQTT Broadcast Alerts
  /**
   * @brief New method that assuming enabled, will broadcast the error message so a central location can view any ongoing system messages
   * It will keep the device_name prefix, so the device can be identified.
   * It should get elevated previledges outside of device_name/status/ so it can be viewed as its own issue. Perhaps just logging.
   * device_name/logging/message/LOG_LEVEL  so LOG_LEVEL can be identified
   * device_name/logging/alert/LOG_LEVEL    alerts are reserved for special messages, and depending on the openhab may be broadcast as notifications on my phone
   * 
   * 
   * 
   */
  #ifdef ENABLE_LOGGING_ADDLOG__MESSAGES_OVER_MQTT
  if (loglevel <= tkr_set->Settings.logging.mqtt_level) 
  {    
    char topic[100];
    snprintf_P(topic, sizeof(topic), "logging/message/%S", pCONT_log->GetLogLevelNamebyID(loglevel));

    // Calculate the length of the timestamp, including the space
    size_t mxtime_length = strlen(mxtime);
    // Calculate the length of the current log data
    size_t log_data_length = strlen(pCONT_log->log_data);
    // Ensure there's enough space in log_data to perform the shift
    if (mxtime_length + log_data_length < sizeof(pCONT_log->log_data)) {
      // Shift the current log data to the right to make space for the timestamp
      memmove(pCONT_log->log_data + mxtime_length, pCONT_log->log_data, log_data_length + 1); // +1 to move the null terminator as well
      // Copy the timestamp to the beginning of log_data
      memcpy(pCONT_log->log_data, mxtime, mxtime_length);
    }

    pCONT_mqtt->Publish(topic, pCONT_log->log_data);
  }
  #endif // ENABLE_LOGGING_ADDLOG__MESSAGES_OVER_MQTT
  
}


void mLogging::handleTelnet(){
  if(!TelnetServer) return; //not configured yet
  if (TelnetServer->hasClient()){
  	// client is connected
    if (!Telnet || !Telnet.connected()){
      if(Telnet) Telnet.stop();          // client disconnected
      Telnet = TelnetServer->available(); // ready for new client
    } else {
      TelnetServer->available().stop();  // have client, block new conections
    }
  }

  if (Telnet && Telnet.connected() && Telnet.available()){
    // client input processing
    while(Telnet.available())
      char c = Telnet.read();//dump values
      // Serial.write(Telnet.read()); // pass through
      // do other stuff with client input here
  } 
}


void AddLog_NoTime(uint8_t loglevel, PGM_P formatP, ...)
{

  // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
  if(
    (loglevel>tkr_set->Settings.logging.serial_level)&&
    (loglevel>tkr_set->Settings.logging.web_level)
    ){
    return;
  }

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(pCONT_log->log_data, sizeof(pCONT_log->log_data), formatP, arg);
  va_end(arg);

  //AddLogAddLog(loglevel);

  char mxtime[25];  // "13:45:21 " //9
  memset(mxtime,0,sizeof(mxtime));

  char level_buffer[10];

  // uint8_t hour = 0;
  // uint8_t minute = 0;
  // uint8_t second = 0;
  // uint8_t uday = 0;
  // uint8_t uhour = 0;
  // uint8_t uminute = 0;
  // uint8_t usecond = 0;
  // if(tkr_time!=NULL){ 
  //   hour = tkr_time->RtcTime.hour; 
  //   minute = tkr_time->RtcTime.minute; 
  //   second = tkr_time->RtcTime.second; 
  //   uday = tkr_time->uptime.Mday; 
  //   usecond = tkr_time->uptime.second; 
  //   uminute = tkr_time->uptime.minute; 
  //   uhour = tkr_time->uptime.hour;   
  // }

  // memset(mxtime,0,sizeof(mxtime));
  // snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %02dT%02d:%02d:%02d "),hour,minute,second,uday,uhour,uminute,usecond);

  // Overrides
  //uint8_t seriallog_level = LOG_LEVEL_DEBUG_MORE;
  //pCONT_log->seriallog_level = LOG_LEVEL_DEBUG_MORE;
  //tkr_set->Settings.logging.serial_level = LOG_LEVEL_DEBUG;
  //tkr_set->Settings.logging.web_level = LOG_LEVEL_INFO;

  // LOG : SERIAL
  if (loglevel <= tkr_set->Settings.logging.serial_level) {
    SERIAL_DEBUG.printf("%s%S %s\r\n", 
    mxtime,
    pCONT_log->GetLogLevelNamebyID(loglevel),  
    pCONT_log->log_data);
    //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
    // SERIAL_DEBUG.flush();
  }

  // LOG : TELNET
  // if (loglevel <= tkr_set->Settings.telnetlog_level) {
  //   if((pCONT_log!=NULL)&&(pCONT_log->server!=NULL)){
  //     if(!pCONT_log->client.connected()) {
  //       pCONT_log->client = pCONT_log->server->available();
  //     }
  //     if(pCONT_log->client.connected()) {
  //       pCONT_log->client.printf("%s%s %s\r\n", mxtime,pCONT_log->GetLogLevelNamebyID(loglevel, level_buffer), log_data);
  //     }
  //   }
  // }
  if(pCONT_log->Telnet){
    //   if(!pCONT_log->client.connected()) {
      
    //     pCONT_log->client = pCONT_log->server->available();
    //   }
    
  pCONT_log->Telnet.println("uptime: ");
    //   if(pCONT_log->client.connected()) {
    //     pCONT_log->client.printf("%s%s %s\r\n", mxtime, pCONT_log->GetLogLevelNamebyID(loglevel, level_buffer), log_data);
    //   }
    // }
    }

  // LOG : WEBSERVER
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  if (tkr_set->Settings.webserver && (loglevel <= tkr_set->Settings.logging.web_level)) {
    // Delimited, zero-terminated buffer of log lines.
    // Each entry has this format: [index][log data]['\1']
    if (!pCONT_log->web_log_index) pCONT_log->web_log_index++;   // Index 0 is not allowed as it is the end of char string
    
    while (pCONT_log->web_log_index == pCONT_log->web_log[0] ||  // If log already holds the next index, remove it
           strlen(pCONT_log->web_log) + strlen(pCONT_log->log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
    {
      char* it = pCONT_log->web_log;
      it++;                                  // Skip web_log_index
      it += pCONT_sup->strchrspn(it, '\1'); // Skip log line
      it++;                                  // Skip delimiting "\1"
      memmove(pCONT_log->web_log, it, WEB_LOG_SIZE -(it-pCONT_log->web_log));  // Move buffer forward to remove oldest log line
    }

    // creates line formatted with \1 meaning EOL
    snprintf_P(pCONT_log->web_log, sizeof(pCONT_log->web_log), PSTR("%s%c%s%S %s\1"), pCONT_log->web_log, 
    pCONT_log->web_log_index++, mxtime, pCONT_log->GetLogLevelNamebyID(loglevel), pCONT_log->log_data);
    if (!pCONT_log->web_log_index) pCONT_log->web_log_index++;   // Index 0 is not allowed as it is the end of char string
  
  // SERIAL_DEBUG.printf("WEBLOG");
  // SERIAL_DEBUG.printf(web_log_index);
  // // for(int i=web_log_index;i<500;i++){
  // //   SERIAL_DEBUG.printf(web_log[i]);
  // // }
  // SERIAL_DEBUG.printf("END WEBLOG");

  }
  #endif  // USE_MODULE_NETWORK_WEBSERVER

}



// void AddLog_Basic(uint8_t loglevel, PGM_P formatP, ...)
// {

//   // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
//   if(
//     (loglevel>tkr_set->Settings.logging.serial_level)&&
//     (loglevel>tkr_set->Settings.logging.web_level)
//     ){
//     return;
//   }

//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
//   va_end(arg);

//   char level_buffer[10];

//   // LOG : SERIAL
//   if (loglevel <= tkr_set->Settings.logging.serial_level) {
//     SERIAL_DEBUG.printf("%s%s %s\r\n", 
//       mxtime,
//       pCONT_log->GetLogLevelNamebyID(loglevel, level_buffer),  
//       log_data
//     );
//   }

// }

// uint8_t seriallog_level = LOG_LEVEL_DEBUG;
// char log_data[300];

// void AddLog_Basic(uint8_t loglevel, PGM_P formatP, ...)
// {

//   // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
//   if(loglevel>seriallog_level){
//     return;
//   }

//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
//   va_end(arg);

//   // LOG : SERIAL
//   if (loglevel <= seriallog_level) {
//     SERIAL_DEBUG.printf("%s %s\r\n", 
//       "INF: ",  
//       log_data
//     );
//   }

// }






/*********************************************************************************************\
 * Syslog
 *
 * Example:
 *   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_LOG "Any value %d"), value);
 *
\*********************************************************************************************/

void mLogging::SetSeriallog(uint8_t loglevel)
{
  tkr_set->Settings.logging.serial_level = loglevel;
  // seriallog_level = loglevel;
  // seriallog_timer = 0;
}


void mLogging::GetLog(uint8_t idx, char** entry_pp, size_t* len_p)
{
  char* entry_p = nullptr;
  size_t len = 0;

  if (idx) {
    char* it = web_log;
    // get line
    do {
      uint8_t cur_idx = *it;
      it++;
      size_t tmp = pCONT_sup->strchrspn(it, '\1');
      tmp++;                             // Skip terminating '\1'
      if (cur_idx == idx) {              // Found the requested entry
        len = tmp;
        entry_p = it;
        break;
      }
      it += tmp;
    } while (it < web_log + WEB_LOG_SIZE && *it != '\0');
  }
  *entry_pp = entry_p;
  *len_p = len;
}
//#endif  // USE_MODULE_NETWORK_WEBSERVER

void mLogging::Syslog(void)
{
  // // Destroys log_data
  // char syslog_preamble[64];  // Hostname + Id

  // uint32_t current_hash = GetHash(Settings.syslog_host, strlen(Settings.syslog_host));
  // if (syslog_host_hash != current_hash) {
  //   syslog_host_hash = current_hash;
  //   WiFi.hostByName(Settings.syslog_host, syslog_host_addr);  // If sleep enabled this might result in exception so try to do it once using hash
  // }
  // if (PortUdp.beginPacket(syslog_host_addr, Settings.syslog_port)) {
  //   snprintf_P(syslog_preamble, sizeof(syslog_preamble), PSTR("%s ESP-"), my_hostname);
  //   memmove(log_data + strlen(syslog_preamble), log_data, sizeof(log_data) - strlen(syslog_preamble));
  //   log_data[sizeof(log_data) -1] = '\0';
  //   memcpy(log_data, syslog_preamble, strlen(syslog_preamble));
  //   PortUdp.write(log_data);
  //   PortUdp.endPacket();
  //   delay(1);  // Add time for UDP handling (#5512)
  // } else {
  //   syslog_level = 0;
  //   syslog_timer = SYSLOG_TIMER;
  //   AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
  // }
}

void mLogging::AddLogAddLog(uint8_t loglevel)
{
//   char mxtime[10];  // "13:45:21 "

//   snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d "), RtcTime.hour, RtcTime.minute, RtcTime.second);

//   if (loglevel <= seriallog_level) {
//     SERIAL_DEBUG.printf("%s%s\r\n", mxtime, log_data);
//   }
// #ifdef USE_MODULE_NETWORK_WEBSERVER
//   if (Settings.webserver && (loglevel <= Settings.weblog_level)) {
//     // Delimited, zero-terminated buffer of log lines.
//     // Each entry has this format: [index][log data]['\1']
//     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
//     while (web_log_index == web_log[0] ||  // If log already holds the next index, remove it
//            strlen(web_log) + strlen(log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
//     {
//       char* it = web_log;
//       it++;                                // Skip web_log_index
//       it += strchrspn(it, '\1');           // Skip log line
//       it++;                                // Skip delimiting "\1"
//       memmove(web_log, it, WEB_LOG_SIZE -(it-web_log));  // Move buffer forward to remove oldest log line
//     }
//     snprintf_P(web_log, sizeof(web_log), PSTR("%s%c%s%s\1"), web_log, web_log_index++, mxtime, log_data);
//     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
//   }
// #endif  // USE_MODULE_NETWORK_WEBSERVER
//   if (!global_state.wifi_down && (loglevel <= syslog_level)) { Syslog(); }
}

// Static wrapper
void mLogging::AddLog_Static(uint8_t loglevel, PGM_P formatP, ...) {
  va_list args;
  va_start(args, formatP);

  // Use the global AddLog function, assuming it has a `va_list` version
  ::AddLog(loglevel, formatP, args);

  va_end(args);
}


void mLogging::AddLog(uint8_t loglevel, const char *formatP)
{
  // snprintf_P(log_data, sizeof(log_data), formatP);
  // AddLogAddLog(loglevel);
}

void mLogging::AddLog(uint8_t loglevel, const char *formatP, const char *formatP2)
{
  // char message[100];

  // snprintf_P(log_data, sizeof(log_data), formatP);
  // snprintf_P(message, sizeof(message), formatP2);
  // strncat(log_data, message, sizeof(log_data) - strlen(log_data) -1);
  // AddLogAddLog(loglevel);
}

void mLogging::AddLog_P2(uint8_t loglevel, PGM_P formatP, ...)
{
  // va_list arg;
  // va_start(arg, formatP);
  // vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
  // va_end(arg);

  // AddLogAddLog(loglevel);
}

void mLogging::AddLogBuffer(uint8_t loglevel, uint8_t *buffer, int count)
{
  // snprintf_P(log_data, sizeof(log_data), PSTR("DMP:"));
  // for (int i = 0; i < count; i++) {
  //   snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, *(buffer++));
  // }
  // AddLogAddLog(loglevel);
}

void mLogging::AddLogSerial(uint8_t loglevel)
{
  //AddLogBuffer(loglevel, (uint8_t*)serial_in_buffer, serial_in_byte_counter);
}

void mLogging::AddLogMissed(char *sensor, uint8_t misses)
{
  // ALOG_DBG(PSTR("SNS: %s missed %d"), sensor, SENSOR_MAX_MISS - misses);
}


/*********************************************************************************************\
 * Response data handling -- "AddLog" that are pushed to mqtt channel "status/response"
\*********************************************************************************************/

// return if response was sent, else use leds?
int Response_mP(const char* format, ...)     // Content send snprintf_P char data
{
  
  // if(tkr_time->uptime_seconds_nonreset<60){ return 0 ;}

  memset(&tkr_set->response_msg,0,sizeof(tkr_set->response_msg));

  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int len = vsnprintf_P(tkr_set->response_msg, RESPONSE_MESSAGE_BUFFER_SIZE, format, args);
  va_end(args);

//   //Share on serial/telnet
  ALOG_DBG(PSTR(D_LOG_RESPONSE "%s"), tkr_set->response_msg);
//   //Send via mqtt
//   #ifdef USE_MODULE_NETWORK_MQTT
//   pCONT_mqtt->publish_device("status/result",tkr_set->response_msg,false);
//   #endif
  
  return 0;// len;
}

int ResponseAppend_mP(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  // va_list args;
  // va_start(args, format);
  // int mlen = strlen(tkr_set->response_msg);
  // int len = vsnprintf_P(tkr_set->response_msg + mlen, sizeof(tkr_set->response_msg) - mlen, format, args);
  // va_end(args);
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_RESPONSE "Response_P %s"),tkr_set->response_msg);
  return 0;// len + mlen;
}


void mLogging::StartTelnetServer()
{  
  // if(tkr_set->global_state.network_down) return;
  TelnetServer = new WiFiServer(TELNET_PORT);  // set port here
  TelnetServer->begin();
  telnet_started = true;
  // if(seriallog)
  // ALOG_INF(PSTR("Telnet server started on port %d"),(uint8_t)TELNET_PORT);
}



//enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, 
//LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_DEBUG_LOWLEVEL, LOG_LEVEL_ALL};


const char* mLogging::GetLogLevelNamebyID(uint8_t id) {
  switch(id) {
    default:
    case LOG_LEVEL_NONE:           return PM_LOG_LEVEL_NONE_CTR;
    case LOG_LEVEL_DEBUG_TRACE:    return PM_LOG_LEVEL_DEBUG_TRACE_CTR;
    case LOG_LEVEL_ERROR:          return PM_LOG_LEVEL_ERROR_CTR;
    case LOG_LEVEL_WARNING:        return PM_LOG_LEVEL_WARN_CTR;
    case LOG_LEVEL_DEV_TEST:           return PM_LOG_LEVEL_TEST_CTR;
    case LOG_LEVEL_HIGHLIGHT:      return PM_LOG_LEVEL_HIGHLIGHT_CTR;
    case LOG_LEVEL_IMPORTANT:      return PM_LOG_LEVEL_IMPORTANT_CTR;
    case LOG_LEVEL_INFO:           return PM_LOG_LEVEL_INFO_CTR;
    case LOG_LEVEL_COMMANDS:       return PM_LOG_LEVEL_COMMANDS_CTR;
    case LOG_LEVEL_DEBUG:          return PM_LOG_LEVEL_DEBUG_CTR;
    case LOG_LEVEL_DEBUG_MORE:     return PM_LOG_LEVEL_DEBUG_MORE_CTR;
    case LOG_LEVEL_DEBUG_LOWLEVEL: return PM_LOG_LEVEL_DEBUG_LOWLEVEL_CTR;
    case LOG_LEVEL_ALL:            return PM_LOG_LEVEL_ALL_CTR;
  }
}

uint8_t mLogging::GetLogLevelIDbyName(const char* name) {
  if (name == nullptr) return LOG_LEVEL_NONE;  // Handle null input

  if (strcmp_P(name, PM_LOG_LEVEL_NONE_CTR) == 0)            return LOG_LEVEL_NONE;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_TRACE_CTR) == 0)     return LOG_LEVEL_DEBUG_TRACE;
  if (strcmp_P(name, PM_LOG_LEVEL_ERROR_CTR) == 0)           return LOG_LEVEL_ERROR;
  if (strcmp_P(name, PM_LOG_LEVEL_WARN_CTR) == 0)            return LOG_LEVEL_WARNING;
  if (strcmp_P(name, PM_LOG_LEVEL_TEST_CTR) == 0)            return LOG_LEVEL_DEV_TEST;
  if (strcmp_P(name, PM_LOG_LEVEL_HIGHLIGHT_CTR) == 0)       return LOG_LEVEL_HIGHLIGHT;
  if (strcmp_P(name, PM_LOG_LEVEL_IMPORTANT_CTR) == 0)       return LOG_LEVEL_IMPORTANT;
  if (strcmp_P(name, PM_LOG_LEVEL_INFO_CTR) == 0)            return LOG_LEVEL_INFO;
  if (strcmp_P(name, PM_LOG_LEVEL_COMMANDS_CTR) == 0)        return LOG_LEVEL_COMMANDS;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_CTR) == 0)           return LOG_LEVEL_DEBUG;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_MORE_CTR) == 0)      return LOG_LEVEL_DEBUG_MORE;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_LOWLEVEL_CTR) == 0)  return LOG_LEVEL_DEBUG_LOWLEVEL;
  if (strcmp_P(name, PM_LOG_LEVEL_ALL_CTR) == 0)             return LOG_LEVEL_ALL;

  return LOG_LEVEL_NONE;  // Default return for no match
}



int8_t mLogging::Tasker(uint8_t function, JsonParserObject obj)
{ // KEEP TASKER ON TOP


  switch(function){
    case TASK_INIT:
      // init();
      // StartTelnetServer();
    break;
    case TASK_LOOP: 
    // Serial.println("mLogging::Tasker");
      // if(!Telnet) StartTelnetServer();
      if(telnet_started){      handleTelnet();    }
    break;
    case TASK_EVERY_SECOND:
      // Serial.println("mLogging::Tasker");
      
      #ifdef ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING
      if(tkr_time->uptime_seconds_nonreset == 60*1)
      {
        SetSeriallog(LOG_LEVEL_INFO);
        Serial.printf("Reducing log level to %d to improve performance when not debugging", tkr_set->Settings.logging.serial_level);
        // ALOG_INF(PSTR(D_LOG_APPLICATION "Reducing log level to %d to improve performance when not debugging"), tkr_set->Settings.logging.serial_level);
      }
      #endif

    break;
    case TASK_EVERY_MINUTE:



  /**** For increasing log level temporarily then reseting
   * 
   * */
  // if (tkr_set->seriallog_timer) {
  //   seriallog_timer--;
  //   if (!seriallog_timer) {
  //     if (seriallog_level) {
  //       ALOG_INF(PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
  //     }
  //     seriallog_level = 0;
  //   }
  // }

  // if (syslog_timer) {  // Restore syslog level
  //   syslog_timer--;
  //   if (!syslog_timer) {
  //     syslog_level = Settings.syslog_level;
  //     if (Settings.syslog_level) {
  //       ALOG_INF(PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
  //     }
  //   }
  // }

    break;
    case TASK_WIFI_CONNECTED:
      StartTelnetServer();
    break;
  }

}


void mLogging::init(void)
{
  
}
