
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER


#include "mWebServer.h"


String messageHead, messageSub;
byte optionType;

// uint8_t webserver_state = HTTP_OFF;

// void mWebServer::init(void){

  
//   #ifdef DEBUG_WEBSERVER_MEMORY
//   freemem_usage_json_shared.name_ptr = freemem_usage_name_json_shared;
//   #endif



// }

#ifdef ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023

// Helper function to avoid code duplication (saves 4k Flash)
void mWebServer::WebGetArg(AsyncWebServerRequest *request, const char* arg, char* out, size_t max)
{
  
  // #ifndef DISABLE_SERIAL_LOGGING
  // P_PHASE_OUT();
  // #endif

  String s = request->arg(arg);
  strlcpy(out, s.c_str(), max);

}

#endif // ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023









void mWebServer::createEditHandler(bool enable) 
{
  if (editHandler != nullptr) pCONT_web->server->removeHandler(editHandler);
  if (enable) 
  {
    #ifdef WLED_ENABLE_FS_EDITOR
      #ifdef ARDUINO_ARCH_ESP32
      editHandler = &pCONT_web->server->addHandler(new SPIFFSEditor(FILE_SYSTEM));
      #else
      editHandler = &pCONT_web->server->addHandler(new SPIFFSEditor("","",FILE_SYSTEM));
      #endif
    #else
      editHandler = &pCONT_web->server->on("/edit", HTTP_GET, [this](AsyncWebServerRequest *request){
        this->serveMessage(request, 501, "Not implemented", F("The FS editor is disabled in this build."), 254);
      });
    #endif
  } 
  else 
  {
    editHandler = &pCONT_web->server->on("/edit", HTTP_ANY, [this](AsyncWebServerRequest *request){
      this->serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_cfg), 254);
    });
  }
}

String mWebServer::msgProcessor(const String& var)
{
  if (var == "MSG") {
    String messageBody = messageHead;
    messageBody += F("</h2>");
    messageBody += messageSub;
    uint32_t optt = optionType;

    if (optt < 60) //redirect to settings after optionType seconds
    {
      messageBody += F("<script>setTimeout(RS,");
      messageBody +=String(optt*1000);
      messageBody += F(")</script>");
    } else if (optt < 120) //redirect back after optionType-60 seconds, unused
    {
      //messageBody += "<script>setTimeout(B," + String((optt-60)*1000) + ")</script>";
    } else if (optt < 180) //reload parent after optionType-120 seconds
    {
      messageBody += F("<script>setTimeout(RP,");
      messageBody += String((optt-120)*1000);
      messageBody += F(")</script>");
    } else if (optt == 253)
    {
      messageBody += F("<br><br><form action=/settings><button class=\"bt\" type=submit>Back</button></form>"); //button to settings
    } else if (optt == 254)
    {
      messageBody += F("<br><br><button type=\"button\" class=\"bt\" onclick=\"B()\">Back</button>");
    }
    return messageBody;
  }
  return String();
}


void mWebServer::serveMessage(AsyncWebServerRequest* request, uint16_t code, const String& headl, const String& subl, byte optionT)
{
  messageHead = headl;
  messageSub = subl;
  optionType = optionT;

  request->send_P(code, "text/html", PAGE_msg, msgProcessor);
}




















// bool mWebServer::WifiIsInManagerMode(){
//   return (HTTP_MANAGER == webserver_state || HTTP_MANAGER_RESET_ONLY == webserver_state);
// }

// // void mWebServer::ShowWebSource(int source)
// // {
// //   // if ((source > 0) && (source < SRC_MAX)) {
// //   //   char stemp1[20];
// //   //   ALOG_DBG(PSTR("SRC: %s from %s"), 
// //   //   pCONT_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), source, kCommandSource), request->client().remoteIP().toString().c_str());
// //   // }
// // }

// // void mWebServer::ExecuteWebCommand(char* svalue, int source)
// // {
// //   ShowWebSource(source);
// //   //ALOG_DBG(PSTR("CODE NOT DONE %s"),"ExecuteCommand(svalue, SRC_IGNORE);");
// //   ExecuteCommand(svalue, SRC_IGNORE);
// // }


// void mWebServer::handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len,
//     size_t index, size_t total) {
//   // ESP_LOGD(TAG, "handleBody [len=%u, index=%u, total=%u]", len, index, total);

//   Serial.println("mWebServer::handleBody");

//   // If whole data is not available, do not botter making sense of it.
//   if (!data || len != total) {
//     return;
//   }

//   // `POST /sys/master`
//   if (request->method() == HTTP_POST && request->url() == "/setmaster") {
//     // if (this->set_master_(data, len)) {
//     //   request->send(200, APPLICATION_JSON, bool_response(true).c_str());
//     // } else  {
//       request->send(500);
//     // }
//     return;
//   }
// }


// void mWebServer::StartWebserver(int type, IPAddress ipweb)
// {
//   // Ensure valid refresh values
//   // if(!pCONT_set->Settings.web_refresh){ pCONT_set->Settings.web_refresh = HTTP_REFRESH_TIME; }

//   ALOG_INF(PSTR(D_LOG_DEBUG "StartWebserver"));
  
//   if(!webserver_state){
//     if(!server){    

//       #ifdef ESP8266
//         server = new AsyncWebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
//       #else
//         server = new AsyncWebServer((HTTP_MANAGER == type || HTTP_MANAGER_RESET_ONLY == type) ? 80 : WEB_PORT);
//       #endif
            
//       DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*"); // ALLOW CROSS SITE JAVASCRIPT ACCESS (CORS) BY DEFAULT
//       DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST");
//       DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "authorization");    

//       // Must add handlers BEFORE begin
//       pCONT->Tasker_Interface(TASK_WEB_ADD_HANDLER);
//     }
//     reset_web_log_flag = false;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_INF(PSTR(D_LOG_HTTP "StartWebserver starting..."));
//     #endif// ENABLE_LOG_LEVEL_INFO
//     server->begin();
//   }
//   if(webserver_state != type){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_INF(PSTR(D_LOG_HTTP D_WEBSERVER " %s%s " D_IPADDRESS " %s"), pCONT_set->my_hostname, 1 ? ".local" : "", ipweb.toString().c_str());
    
//     #endif// ENABLE_LOG_LEVEL_INFO
//     // pCONT_set->rules_flag.http_init = 1;
//   }
//   if(type){ webserver_state = type; }
// }


// void mWebServer::WebAppend_Root_Draw_Table_dList(uint8_t row_count, char const* value_handle, const char* dList_titles){
//   char listheading[50];

//   //row_count=2;

//   BufferWriterI->Append_P(PSTR("{t}"));
//   for(int ii=0;ii<row_count;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed(listheading, sizeof(listheading), ii, dList_titles));
//       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }


// void mWebServer::WebAppend_Root_Draw_Table_dList_P(uint8_t row_count, char const* value_handle, const char* dList_titles){
//   char listheading[50];

//   //row_count=2;

//   BufferWriterI->Append_P(PSTR("{t}"));
//   for(int ii=0;ii<row_count;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, dList_titles));
//       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }

// void mWebServer::WebAppend_Root_Draw_Table_Repeat_Row_Name_Numbers(uint8_t row_count, char const* value_handle, const char* dList_titles){
  
//   BufferWriterI->Append_P(PSTR("{t}"));
//   for(int ii=0;ii<row_count;ii++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       BufferWriterI->Append_P(PSTR("<td>%s %d</td>"), dList_titles);
//       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }    
//   BufferWriterI->Append_P(PSTR("{t2}"));
// }

// //pass row title using array/arrlen, include prefix format
// //leave contents blank

// // void mWebServer::WebAppend_Draw_Table_FP(uint8_t row_count, char const* value_handle, const char* dList_titles){
// //   char listheading[50];
// //   BufferWriterI->Append_P(PSTR("{t}"));
// //   for(int ii=0;ii<row_count;ii++){
// //     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
// //       BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, dList_titles));
// //       BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),value_handle);   
// //     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
// //   }    
// //   BufferWriterI->Append_P(PSTR("{t2}"));
// // }



// void mWebServer::WebAppend_Draw_Table_FP(const char* table_class_col2, const char* formatP_row1, ...)
// {
//   char* buff = BufferWriterI->GetBufferPtr();
//   uint16_t* len = BufferWriterI->GetLengthPtr();
//   uint16_t size = BufferWriterI->GetBufferSize();

//   BufferWriterI->Append_P(PSTR("{t}"));
//   va_list arg;
//   va_start(arg, formatP_row1);
//   //Write cell 1
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   BufferWriterI->Append_P(PSTR("<td>"));
//     *len += vsnprintf_P(&buff[*len], size, formatP_row1, arg);
//   BufferWriterI->Append_P(PSTR("</td>"));
//   //Write cell 2
//   BufferWriterI->Append_P(PSTR("<td><div class='%s'></div></td>"),table_class_col2);
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   va_end(arg);
//   BufferWriterI->Append_P(PSTR("{t2}"));

// }




void mWebServer::WebPage_Root_AddHandlers(){

//   /**
//    * Shared resources
//    * */
//   pCONT_web->server->on("/base_page_fill.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Base_Page_Draw(request);
//   });   
//   server->on(WEB_HANLDE_JSON_WEB_TOP_BAR, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     WebSend_JSON_WebServer_TopBar(request);    
//   });  
//   server->on(WEB_HANLDE_JSON_WEB_STATUS_POPOUT_DATA, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     WebSend_JSON_WebServer_StatusPopoutData(request);    
//   });
//   pCONT_web->server->on(WEB_HANDLE_JSON_ROOT_STATUS_ANY, HTTP_GET, [this](AsyncWebServerRequest *request){
//     WebSend_JSON_RootStatus_Table(request);
//   });

//   pCONT_web->server->on("/console_test.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Console_JSON_Data(request);
//   });  
      
//   pCONT_web->server->onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//     if(!index)
//       Serial.printf("BodyStart: %u\n", total);
//     Serial.printf("%s", (const char*)data);
//     if(index + len == total)
//       Serial.printf("BodyEnd: %u\n", total);
//   });

  
//   pCONT_web->server->on("/json_command.json", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
//     Serial.println("onRequestBody " "/json_command.json" );
//     if ((request->url() == "/json_command.json") && (request->method() == HTTP_POST))
//     {

//       //copy into buffer
//       D_DATA_BUFFER_CLEAR();
//       memcpy(data_buffer.payload.ctr,data,sizeof(char)*total);
//       data_buffer.payload.len = strlen(data_buffer.payload.ctr);
//       data_buffer.flags.source_id = DATA_BUFFER_FLAG_SOURCE_WEBUI;

//       pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);     
        
//       request->send(200, CONTENT_TYPE_APPLICATION_JSON_ID, "{\"status\":\"success\"}");

//     }
//   });

//   /**
//    * Root Page
//    * */
//   // server->redirect("/", "/main/page");
//   // pCONT_web->server->rewrite("/", WEB_HANDLE_ROOT);
//   pCONT_web->server->on(WEB_HANDLE_ROOT "/", [this](AsyncWebServerRequest *request){
//     HandlePage_Root(request);
//   });
//   pCONT_web->server->on(WEB_HANDLE_ROOT "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Root_Draw(request);
//   });
//   pCONT_web->server->on(WEB_HANDLE_ROOT "/module_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
//     Web_Root_Draw_Modules(request);
//   });
//   pCONT_web->server->on(WEB_HANDLE_ROOT "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
//     Web_Root_UpdateURLs(request);
//   }); 

#ifdef ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023
  /**
   * Console Page
   * */
  pCONT_web->server->on(D_WEB_HANDLE_CONSOLE_PAGE, [this](AsyncWebServerRequest *request){
    HandlePage_Console(request);
  });
#endif // ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023
  // pCONT_web->server->on(D_WEB_HANDLE_CONSOLE "/page_draw.json", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   Web_Console_Draw(request);
  // });
  // pCONT_web->server->on(D_WEB_HANDLE_CONSOLE "/update_urls.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
  //   // Web_Root_UpdateURLs(request);
  // }); 
  // pCONT_web->server->on(D_WEB_HANDLE_CONSOLE "/update_data.json", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   // Web_Root_Draw_Modules(request); //get console log
  // });

//   /**
//    * Information Page
//    * */
//   // server->on(PM_WEBURL_PAGE_INFO, [this](AsyncWebServerRequest *request){
//   //   HandleInformation(request); 
//   // });
//   // server->on(PM_WEBURL_PAGE_INFO_LOAD_URLS, HTTP_GET, [this](AsyncWebServerRequest *request){  
//   //   Web_All_LoadTime_Minimum_URLs(request);
//   // });  
//   // server->on(PM_WEBURL_PAGE_INFO_DRAW_TABLE, [this](AsyncWebServerRequest *request){
//   //   HandleInformation_TableDraw(request); 
//   // });
//   // server->on(PM_WEBURL_PAGE_INFO_LOAD_SCRIPT, HTTP_GET, [this](AsyncWebServerRequest *request){ 
//   //   //Web_Info_LoadScript(request);
//   //   HandleNotFound(request);
//   // });

//   /**
//    * Upgrades
//    * */  
//   // server->on("/up", [this](AsyncWebServerRequest *request){HandleUpgradeFirmware(request); });
//   // server->on("/u1", [this](AsyncWebServerRequest *request){HandleUpgradeFirmwareStart(request); });  // OTA
//   // // server->on("/u2", HTTP_POST,
//   // //     [this](AsyncWebServerRequest *request){HandleUploadDone(request); },
//   // //     [this](AsyncWebServerRequest *request){HandleUploadLoop(request); });
//   // server->on("/u2", HTTP_OPTIONS, [this](AsyncWebServerRequest *request){HandlePreflightRequest(request); });

//   /**
//    * System Settings
//    * */  
//   // server->on("/ss", [this](AsyncWebServerRequest *request){
//   //   HandleSystemSettings(request); 
//   // });


//   /**
//    * Resources Settings
//    * */ 
//   // server->on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
//   //   // HandleFavicon(request);
//   // });
          
// //       server->on("/cm", [this](AsyncWebServerRequest *request){HandleHttpCommand(request); });
  
// // #ifndef FIRMWARE_MINIMAL
// //       server->on("/cn", [this](AsyncWebServerRequest *request){HandleConfiguration(request); });
// //       server->on("/md", [this](AsyncWebServerRequest *request){HandleModuleConfiguration(request); });
// //       server->on("/wi", [this](AsyncWebServerRequest *request){HandleWifiConfiguration(request); });
// //       server->on("/lg", [this](AsyncWebServerRequest *request){HandleLoggingConfiguration(request); });
// //       server->on("/tp", [this](AsyncWebServerRequest *request){HandleTemplateConfiguration(request); });
// //       server->on("/co", [this](AsyncWebServerRequest *request){HandleOtherConfiguration(request); });
// //       server->on("/dl", [this](AsyncWebServerRequest *request){HandleBackupConfiguration(request); });
// //       server->on("/rs", [this](AsyncWebServerRequest *request){HandleRestoreConfiguration(request); });
//       // server->on("/rt", [this](AsyncWebServerRequest *request){HandleResetConfiguration(request); });
//       server->on("/reset", [this](AsyncWebServerRequest *request){HandleReset(request); });
// //#endif
//   server->onNotFound([this](AsyncWebServerRequest *request){HandleNotFound(request); });
  
}


// bool mWebServer::CheckWebSendFreeMemoryTooLow(uint16_t memory_needed){
//   if(!memory_needed){
//     memory_needed = 3000;
//   }
//   if(ESP.getFreeHeap()>memory_needed){
//     return true;
//   }
//   return false;
// }

// bool mWebServer::RespondWebSendFreeMemoryTooLow(AsyncWebServerRequest *request, uint16_t memory_needed){
//   if(!memory_needed){
//     memory_needed = 2000;
//   }
//   if(ESP.getFreeHeap()<memory_needed){
//     char message[45];
//     sprintf(message,PSTR("Memory too low - %d needed (%d free)"),memory_needed,ESP.getFreeHeap());
//     request->send(413, CONTENT_TYPE_TEXT_HTML_ID, message);
//     return true;
//   }
//   return false;
// }

// bool mWebServer::RespondWebSend_UnableToAllocateBuffer(AsyncWebServerRequest *request){
//   char message[45];
//   sprintf(message,PSTR("Unable to allocate buffer (FreeHeap %d)"),ESP.getFreeHeap());
//   request->send(413, CONTENT_TYPE_TEXT_HTML_ID, message);
// }


/**
 * @brief 
 * 
 * @param id 
 * @param buffer 
 * @param buflen 
 * @return char* 
 * 
 * I dont need to copy as its progmem
 */
const char* mWebServer::GetContentTypeCtrP_By_ID(uint8_t id)
{
  
  // switch(_contentType_id){
  //   default:
  //   case CONTENT_TYPE_TEXT_HTML_ID: 
  //     // snprintf_P(buffer, sizeof(PM_WEB_CONTENT_TYPE_TEXT_HTML), PM_WEB_CONTENT_TYPE_TEXT_HTML); 
  //     return PM_WEB_CONTENT_TYPE_TEXT_HTML;
  //   break; // Necessary casts and dereferencing, just copy.
  //   case CONTENT_TYPE_TEXT_JAVASCRIPT_ID: 
  //     // memcpy_P(buffer, PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT, sizeof(PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT)); 
  //     return PM_WEB_CONTENT_TYPE_TEXT_JAVASCRIPT;
  //   break;
  //   case CONTENT_TYPE_APPLICATION_JSON_ID: 
  //     // memcpy_P(buffer, PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT, sizeof(PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT));
  //     return PM_WEB_CONTENT_TYPE_APPLICATION_JSON_JAVASCRIPT;
  //   break;
  //   case CONTENT_TYPE_TEXT_CSS_ID: 
  //     // memcpy_P(buffer, PM_WEB_CONTENT_TYPE_TEXT_CSS, sizeof(PM_WEB_CONTENT_TYPE_TEXT_CSS)); 
  //     return PM_WEB_CONTENT_TYPE_TEXT_CSS;
  //   break;

  // }
  

  // return buffer;

}




// Handle all webpage sends to allow for checking of freespace
void mWebServer::WebSend_Response(AsyncWebServerRequest *request, int code, uint8_t contentType_id, char* content_ptr){

  // Work out memory needed relative to body we want to send?
  // Serial.println("HERE"); Serial.flush();

  // char content_type[30]; // Should exist until send completes, might not as its async though
  #ifdef DEBUG_WEBSERVER_MEMORY
    FreeMem_Usage_Before(&freemem_usage_json_shared);
  #endif
  // WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  
 
  // Check if there is enough RAM space, or else respond with 
  // if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 
  // Contine to send requested data 
  // request->send(code, contentType_id, content_ptr); 

   #ifdef DEBUG_WEBSERVER_MEMORY
    FreeMem_Usage_After(&freemem_usage_json_shared);
  #endif 

}


// #ifdef DEBUG_WEBSERVER_MEMORY
// void mWebServer::FreeMem_Usage_Before(freemem_usage_t* freemem){
//   freemem->free_bytes = ESP.getFreeHeap();
// }
// void mWebServer::FreeMem_Usage_After(freemem_usage_t* freemem){
//   freemem->bytes_used = freemem->free_bytes - ESP.getFreeHeap();
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_ASYNC "Free \"%s\"=%d/%d %d space"), //used of previously free, additional space that remained free
//     freemem->name_ptr,
//     freemem->bytes_used,freemem->free_bytes,
//     ESP.getFreeHeap());
// }
// #endif




// void mWebServer::StopWebserver(void)
// {
//   //if(webserver_state) {
//     server->reset(); // asyncedit
//     webserver_state = HTTP_OFF;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_INF(PSTR(D_LOG_HTTP D_WEBSERVER_STOPPED));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   //}
// }

// void mWebServer::WifiManagerBegin(bool reset_only)
// {
  
//   ALOG_INF(PSTR(D_LOG_DEBUG "mWebServer::WifiManagerBegin"));

//   // setup AP
//   if (!pCONT_set->global_state.wifi_down) {
//     WiFi.mode(WIFI_AP_STA);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_INF(PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   } else {
//     WiFi.mode(WIFI_AP);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     ALOG_INF(PSTR(D_LOG_WIFI D_WIFIMANAGER_SET_ACCESSPOINT));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   }

//   StopWebserver();

//   DnsServer = new DNSServer();

//   int channel = WIFI_SOFT_AP_CHANNEL;
//   if ((channel < 1) || (channel > 13)) { channel = 1; }
//   WiFi.softAP(pCONT_set->my_hostname, nullptr, channel);

//   delay(500); // Without delay I've seen the IP address blank
//   /* Setup the DNS server redirecting all the domains to the apIP */
//   DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
//   DnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

//   StartWebserver((reset_only ? HTTP_MANAGER_RESET_ONLY : HTTP_MANAGER), WiFi.softAPIP());

// }

// void mWebServer::PollDnsWebserver(void)
// {
//   if (DnsServer) { DnsServer->processNextRequest(); }
// }

// /*********************************************************************************************/

// bool mWebServer::WebAuthenticate(void)
// {
//   // if (pCONT_set->Settings.web_password[0] != 0 && HTTP_MANAGER_RESET_ONLY != webserver_state) {
//   //   // return server->authenticate(WEB_USERNAME, pCONT_set->Settings.web_password);
//   // } else {
//     return true;
//   // }
// }

//bool mWebServer::HttpCheckPriviledgedAccess(bool autorequestauth = true)
bool mWebServer::HttpCheckPriviledgedAccess()
{

//     bool autorequestauth = true;

//   if (HTTP_USER == webserver_state) {
//     HandleRoot();
//     return false;
//   }
//   if (autorequestauth && !WebAuthenticate()) {
//     server->requestAuthentication();
//     return false;
//   }
  return true; // admin by default
}



// void mWebServer::send_mP(AsyncWebServerRequest *request, int code, uint8_t content_type, const char* formatP, ...)     // Content send snprintf_P char data
// {
//   D_DATA_BUFFER_CLEAR();
//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
//   va_end(arg);  
//   request->send_P(code, content_type, data_buffer.payload.ctr);
// }

// // void mWebServer::WSContentSend_PD(AsyncWebServerRequest *request, const char* formatP, ...)    // Content send snprintf_P char data checked for decimal separator
// // {

// //   // This uses char strings. Be aware of sending %% if % is needed
// //   va_list arg;
// //   va_start(arg, formatP);
// //   int len = vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
// //   va_end(arg);

// //   if (D_DECIMAL_SEPARATOR[0] != '.') {
// //     for (int i = 0; i < len; i++) {
// //       if ('.' == data_buffer.payload.ctr[i]) {
// //         data_buffer.payload.ctr[i] = D_DECIMAL_SEPARATOR[0];
// //       }
// //     }
// //   }
// // }







// // void mWebServer::WebHexCode(uint8_t i, const char* code)
// // {
// //   char scolor[10];

// //   strlcpy(scolor, code, sizeof(scolor));
// //   char* p = scolor;
// //   if ('#' == p[0]) { p++; }  // Skip

// //   if (3 == strlen(p)) {  // Convert 3 character to 6 character color code
// //     p[6] = p[3];  // \0
// //     p[5] = p[2];  // 3
// //     p[4] = p[2];  // 3
// //     p[3] = p[1];  // 2
// //     p[2] = p[1];  // 2
// //     p[1] = p[0];  // 1
// //   }

// //   uint32_t color = strtol(p, nullptr, 16);
// // /*
// //   if (3 == strlen(p)) {  // Convert 3 character to 6 character color code
// //     uint32_t w = ((color & 0xF00) << 8) | ((color & 0x0F0) << 4) | (color & 0x00F);  // 00010203
// //     color = w | (w << 4);                                                            // 00112233
// //   }
// // */

// //   pCONT_set->Settings.web_color[i][0] = (color >> 16) & 0xFF;  // Red
// //   pCONT_set->Settings.web_color[i][1] = (color >> 8) & 0xFF;   // Green
// //   pCONT_set->Settings.web_color[i][2] = color & 0xFF;          // Blue
// // }

// // uint32_t mWebServer::WebColor(uint8_t i)
// // {
// //   return (pCONT_set->Settings.web_color[i][0] << 16) | (pCONT_set->Settings.web_color[i][1] << 8) | pCONT_set->Settings.web_color[i][2];
// // }

// uint32_t mWebServer::WebColor(uint8_t r, uint8_t g, uint8_t b){
//   return (r << 16) | (g << 8) | b;
// }

// const char* mWebServer::WebColorCtr(uint8_t r, uint8_t g, uint8_t b, char* buffer, uint8_t buflen){
//   snprintf_P(buffer, buflen, PSTR("#%02X%02X%02X"), r, g, b);
//   return buffer;
// }

// // const char* mWebServer::WebColorCtr(char* buffer, uint8_t r,uint8_t g,uint8_t b){
// //   sprintf(buffer,PSTR("#%02X%02X%02X\0"),r,g,b);
// //   return buffer;
// // }






// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleFavicon(AsyncWebServerRequest *request){
//   // request->send_P(200, "image/x-icon", (char*)favicon, 156);
//   // request->send_P(200, "image/x-icon", (char*)favicon, sizeof(favicon));  
//   // request->send_P(200, "image/x-icon", favicon, 156);
// }







// /*-------------------------------------------------------------------------------------------*/

// // String mWebServer::htmlEscape(String s)
// // {
// //   s.replace("&", "&amp;");
// //   s.replace("<", "&lt;");
// //   s.replace(">", "&gt;");
// //   s.replace("\"", "&quot;");
// //   s.replace("'", "&#x27;");
// //   s.replace("/", "&#x2F;");
// //   return s;
// // }
// /*-------------------------------------------------------------------------------------------*/

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleHttpCommand(AsyncWebServerRequest *request)
// {
//   // //if (!HttpCheckPriviledgedAccess(false)) { return; }

//   // ALOG_DBG(PSTR(D_LOG_HTTP D_COMMAND));

//   // uint8_t valid = 1;
//   // if (pCONT_set->Settings.web_password[0] != 0) {
//   //   char tmp1[sizeof(pCONT_set->Settings.web_password)];
//   //   WebGetArg(request,"user", tmp1, sizeof(tmp1));
//   //   char tmp2[sizeof(pCONT_set->Settings.web_password)];
//   //   WebGetArg(request,"password", tmp2, sizeof(tmp2));
//   //   if (!(!strcmp(tmp1, WEB_USERNAME) && !strcmp(tmp2, pCONT_set->Settings.web_password))) { valid = 0; }
//   // }

//   // WSContentBegin(request, 200, CT_JSON);
//   // if (valid) {
//   //   uint8_t curridx = pCONT_set->web_log_index;
//   //   String svalue = request->arg("cmnd");
//   //   if (svalue.length() && (svalue.length() < INPUT_BUFFER_SIZE)) {
//   //     ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCOMMAND);
//   //     if (pCONT_set->web_log_index != curridx) {
//   //       uint8_t counter = curridx;
//   //       WSBufferAppend_P(response, PSTR("{"));
//   //       bool cflg = false;
//   //       do {
//   //         char* tmp;
//   //         size_t len;
//   //         pCONT->mso->GetLog(counter, &tmp, &len);
//   //         if (len) {
//   //           // [14:49:36 MQTT: stat/wemos5/RESULT = {"POWER":"OFF"}] > [{"POWER":"OFF"}]
//   //           char* JSON = (char*)memchr(tmp, '{', len);
//   //           if (JSON) { // Is it a JSON message (and not only [15:26:08 MQT: stat/wemos5/POWER = O])
//   //             size_t JSONlen = len - (JSON - tmp);
//   //             if (JSONlen > sizeof(data_buffer.payload.ctr)) { JSONlen = sizeof(data_buffer.payload.ctr); }
//   //             char stemp[JSONlen];
//   //             strlcpy(stemp, JSON +1, JSONlen -2);
//   //             WSBufferAppend_P(response, PSTR("%s%s"), (cflg) ? "," : "", stemp);
//   //             cflg = true;
//   //           }
//   //         }
//   //         counter++;
//   //         if (!counter) counter++;  // Skip 0 as it is not allowed
//   //       } while (counter != pCONT_set->web_log_index);
//   //       WSBufferAppend_P(response, PSTR("}"));
//   //     } else {
//   //       WSBufferAppend_P(response, PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENABLE_WEBLOG_FOR_RESPONSE "\"}"));
//   //     }
//   //   } else {
//   //     WSBufferAppend_P(response, PSTR("{\"" D_RSLT_WARNING "\":\"" D_ENTER_COMMAND " cmnd=\"}"));
//   //   }
//   // } else {
//   //   WSBufferAppend_P(response, PSTR("{\"" D_RSLT_WARNING "\":\"" D_NEED_USER_AND_PASSWORD "\"}"));
//   // }
//   // WSContentEnd(request);
// }

// /*-------------------------------------------------------------------------------------------*/




// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Other helpers
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// /* Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not try to handle the request again. */
// bool mWebServer::CaptivePortal(AsyncWebServerRequest *request)
// {
//   // Possible hostHeader: connectivitycheck.gstatic.com or 192.168.4.1
//   if ((WifiIsInManagerMode())){//} && !pCONT_sup->ValidIpAddress(request->hostHeader().c_str())) {
//     // ALOG_DBG(PSTR(D_LOG_HTTP D_REDIRECTED));
//     // request->sendHeader(F("Location"), String("http://") + request->client().localIP().toString(), true);


//     // WSSend(302, CT_PLAIN, "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
    
//     // AsyncWebServerResponse *response = request->beginResponse(302);
//     // response->addHeader("Location", "http://4.3.2.1");
//     // request->send(response);

//     // request->client().stop();  // Stop is needed because we sent no content length
//     return true;
//   }
//   return false;
// }

// /*********************************************************************************************/

// // String mWebServer::UrlEncode(const String& text)
// // {
// //   // #ifndef DISABLE_SERIAL_LOGGING
// //   // P_PHASE_OUT();
// //   // #endif
// //   const char hex[] = "0123456789ABCDEF";

// // 	String encoded = "";
// // 	int len = text.length();
// // 	int i = 0;
// // 	while (i < len)	{
// // 		char decodedChar = text.charAt(i++);

// // /*
// //     if (('a' <= decodedChar && decodedChar <= 'z') ||
// //         ('A' <= decodedChar && decodedChar <= 'Z') ||
// //         ('0' <= decodedChar && decodedChar <= '9') ||
// //         ('=' == decodedChar)) {
// //       encoded += decodedChar;
// // 		} else {
// //       encoded += '%';
// // 			encoded += hex[decodedChar >> 4];
// // 			encoded += hex[decodedChar & 0xF];
// //     }
// // */
// //     if (' ' == decodedChar) {
// //       encoded += '%';
// // 			encoded += hex[decodedChar >> 4];
// // 			encoded += hex[decodedChar & 0xF];
// //     } else {
// //       encoded += decodedChar;
// //     }

// // 	}
// // 	return encoded;
// // }

// // int mWebServer::WebSend(char *buffer)
// // {
// //   // [sonoff] POWER1 ON                                               --> Sends http://sonoff/cm?cmnd=POWER1 ON
// //   // [192.168.178.86:80,admin:joker] POWER1 ON                        --> Sends http://hostname:80/cm?user=admin&password=joker&cmnd=POWER1 ON
// //   // [sonoff] /any/link/starting/with/a/slash.php?log=123             --> Sends http://sonoff/any/link/starting/with/a/slash.php?log=123
// //   // [sonoff,admin:joker] /any/link/starting/with/a/slash.php?log=123 --> Sends http://sonoff/any/link/starting/with/a/slash.php?log=123

// //   char *host;
// //   char *user;
// //   char *password;
// //   char *command;
// //   int status = 1;                             // Wrong parameters

// //                                               // buffer = |  [  192.168.178.86  :  80  ,  admin  :  joker  ]    POWER1 ON   |
// //   host = strtok_r(buffer, "]", &command);     // host = |  [  192.168.178.86  :  80  ,  admin  :  joker  |, command = |    POWER1 ON   |
// //   if (host && command) {
// //     pCONT_sup->RemoveSpace(host);                        // host = |[192.168.178.86:80,admin:joker|
// //     host++;                                   // host = |192.168.178.86:80,admin:joker| - Skip [
// //     host = strtok_r(host, ",", &user);        // host = |192.168.178.86:80|, user = |admin:joker|
// //     String url = F("http://");                // url = |http://|
// //     url += host;                              // url = |http://192.168.178.86:80|

// //     command = pCONT_sup->Trim(command);                  // command = |POWER1 ON| or |/any/link/starting/with/a/slash.php?log=123|
// //     if (command[0] != '/') {
// //       url += F("/cm?");                       // url = |http://192.168.178.86/cm?|
// //       if (user) {
// //         user = strtok_r(user, ":", &password);  // user = |admin|, password = |joker|
// //         if (user && password) {
// //           char userpass[128];
// //           snprintf_P(userpass, sizeof(userpass), PSTR("user=%s&password=%s&"), user, password);
// //           url += userpass;                    // url = |http://192.168.178.86/cm?user=admin&password=joker&|
// //         }
// //       }
// //       url += F("cmnd=");                      // url = |http://192.168.178.86/cm?cmnd=| or |http://192.168.178.86/cm?user=admin&password=joker&cmnd=|
// //     }
// //     url += command;                           // url = |http://192.168.178.86/cm?cmnd=POWER1 ON|

// // //ALOG_DBG(PSTR("DBG: Uri |%s|"), url.c_str());

// // #if defined(ARDUINO_ESP8266_RELEASE_2_3_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_0) || defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
// //     HTTPClient http;
// //     if (http.begin(UrlEncode(url))) {         // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
// // #else
// //     WiFiClient http_client;
// //     HTTPClient http;
// //     if (http.begin(http_client, UrlEncode(url))) {  // UrlEncode(url) = |http://192.168.178.86/cm?cmnd=POWER1%20ON|
// // #endif
// //       int http_code = http.GET();             // Start connection and send HTTP header
// //       if (http_code > 0) {                    // http_code will be negative on error
// //         if (http_code == HTTP_CODE_OK || http_code == HTTP_CODE_MOVED_PERMANENTLY) {
// // /*
// //           // Return received data to the user - Adds 900+ bytes to the code
// //           String result = http.getString();   // File found at server - may need lot of ram or trigger out of memory!
// //           uint16_t j = 0;
// //           for (uint16_t i = 0; i < result.length(); i++) {
// //             char text = result.charAt(i);
// //             if (text > 31) {                  // Remove control characters like linefeed
// //               data_buffer.payload.ctr[j++] = text;
// //               if (j == sizeof(data_buffer.payload.ctr) -2) { break; }
// //             }
// //           }
// //           data_buffer.payload.ctr[j] = '\0';
// //           MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_WEBSEND));
// // */
// //         }
// //         status = 0;                           // No error - Done
// //       } else {
// //         status = 2;                           // Connection failed
// //       }
// //       http.end();                             // Clean up connection data
// //     } else {
// //       status = 3;                             // Host not found or connection error
// //     }
// //   }
// //   return status;
// // }

// /*********************************************************************************************/

// // bool mWebServer::JsonWebColor(const char* dataBuf)
// // {
// //   // Default (light)
// //   // {"WebColor":["#000000","#ffffff","#f2f2f2","#000000","#ffffff","#000000","#ffffff","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000"]}
// //   // Alternative (Dark)
// //   // {"webcolor":["#eeeeee","#181818","#4f4f4f","#000000","#dddddd","#008000","#222222","#ff0000","#008000","#ffffff","#1fa3ec","#0e70a4","#d43535","#931f1f","#47c266","#5aaf6f","#ffffff","#999999","#000000"]}

// //   // char dataBufLc[strlen(dataBuf) +1];
// //   // pCONT_sup->LowerCase(dataBufLc, dataBuf);
// //   // pCONT_sup->RemoveSpace(dataBufLc);
// //   // if (strlen(dataBufLc) < 9) { return false; }  // Workaround exception if empty JSON like {} - Needs checks

// //   // StaticJsonDocument<450>  doc;
// //   // DeserializationError error =  deserializeJson(doc, dataBuf);
// //   // JsonObject obj = doc.to<JsonObject>();
// //   // if (error) { return false; }

// //   // // StaticJsonBuffer<450> jb;  // 421 from https://arduinojson.org/v5/assistant/
// //   // // JsonObject& obj = jb.parseObject(dataBufLc);
// //   // // if (!obj.success()) { return false; }

// //   // char parm_lc[10];
// //   // if (obj[pCONT_sup->LowerCase(parm_lc, D_WEBCOLOR)].isNull()) {
// //   //   for (uint8_t i = 0; i < pCONT_set->COL_LAST; i++) {
// //   //     const char* color = obj[parm_lc][i];
// //   //     if (color != nullptr) {
// //   //       WebHexCode(i, color);
// //   //     }
// //   //   }
// //   // }
// //   return true;
// // }

// enum WebCommands { CMND_WEBSERVER, CMND_WEBPASSWORD, CMND_WEBLOG, CMND_WEBREFRESH, CMND_WEBSEND, CMND_WEBCOLOR, CMND_EMULATION };
// // const char kWebCommands[] PROGMEM = D_WEBSERVER "|" D_WEBPASSWORD "|" D_WEBLOG "|" D_WEBREFRESH "|" D_WEBSEND "|" D_WEBCOLOR "|" D_EMULATION ;
// // const char kWebSendStatus[] PROGMEM = D_DONE "|" D_WRONG_PARAMETERS "|" D_CONNECT_FAILED "|" D_HOST_NOT_FOUND ;

// // bool mWebServer::WebCommand(void)
// // {
// //   char command[CMDSZ];
// //   bool serviced = true;

// // //   int command_code = pCONT_sup->GetCommandCode(command, sizeof(command), pCONT_set->XdrvMailbox.topic, kWebCommands);
// // //   if (-1 == command_code) {
// // //     serviced = false;  // Unknown command
// // //   }
// // //   if (CMND_WEBSERVER == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.payload >= 0) && (pCONT_set->XdrvMailbox.payload <= 2)) { pCONT_set->Settings.webserver = pCONT_set->XdrvMailbox.payload; }
// // //     if (pCONT_set->Settings.webserver) {
// // //       pCONT_sup->Response_P(PSTR("{\"" D_WEBSERVER "\":\"" D_ACTIVE_FOR " %s " D_ON_DEVICE " %s " D_WITH_IP_ADDRESS " %s\"}"),
// // //         (2 == pCONT_set->Settings.webserver) ? D_ADMIN : D_USER, pCONT_set->my_hostname, WiFi.localIP().toString().c_str());
// // //     } else {
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_SVALUE, command, pCONT_sup->GetStateText(0));
// // //     }
// // //   }
// // //   else if (CMND_WEBPASSWORD == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.data_len > 0) && (pCONT_set->XdrvMailbox.data_len < sizeof(pCONT_set->Settings.web_password))) {
// // //       strlcpy(pCONT_set->Settings.web_password, (SC_CLEAR == pCONT_sup->Shortcut(pCONT_set->XdrvMailbox.data)) ? "" : (SC_DEFAULT == pCONT_sup->Shortcut(pCONT_set->XdrvMailbox.data)) ? WEB_PASSWORD : pCONT_set->XdrvMailbox.data, sizeof(pCONT_set->Settings.web_password));
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_SVALUE, command, pCONT_set->Settings.web_password);
// // //     } else {
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_ASTERIX, command);
// // //     }
// // //   }
// // //   else if (CMND_WEBLOG == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.payload >= LOG_LEVEL_NONE) && (pCONT_set->XdrvMailbox.payload <= LOG_LEVEL_ALL)) { pCONT_set->Settings.logging.web_level = pCONT_set->XdrvMailbox.payload; }
// // //     pCONT_sup->Response_P(S_JSON_COMMAND_NVALUE, command, pCONT_set->Settings.logging.web_level);
// // //   }
// // //   else if (CMND_WEBREFRESH == command_code) {
// // //     if ((pCONT_set->XdrvMailbox.payload > 999) && (pCONT_set->XdrvMailbox.payload <= 10000)) { pCONT_set->Settings.web_refresh = pCONT_set->XdrvMailbox.payload; }
// // //     pCONT_sup->Response_P(S_JSON_COMMAND_NVALUE, command, pCONT_set->Settings.web_refresh);
// // //   }
// // //   else if (CMND_WEBSEND == command_code) {
// // //     if (pCONT_set->XdrvMailbox.data_len > 0) {
// // //       uint8_t result = WebSend(request, pCONT_set->XdrvMailbox.data);
// // //       char stemp1[20];
// // //       pCONT_sup->Response_P(S_JSON_COMMAND_SVALUE, command, pCONT_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), result, kWebSendStatus));
// // //     }
// // //   }
// // //   else if (CMND_WEBCOLOR == command_code) {
// // //     if (pCONT_set->XdrvMailbox.data_len > 0) {
// // //       if (strstr(pCONT_set->XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be parameter
// // //         if ((pCONT_set->XdrvMailbox.data_len > 3) && (pCONT_set->XdrvMailbox.index > 0) && (pCONT_set->XdrvMailbox.index <= pCONT_set->COL_LAST)) {
// // //           pCONT_sup->WebHexCode(pCONT_set->XdrvMailbox.index -1, pCONT_set->XdrvMailbox.data);
// // //         }
// // //         else if (0 == pCONT_set->XdrvMailbox.payload) {
// // //           pCONT_set->SettingsDefaultWebColor();
// // //         }
// // //       }
// // //       else {
// // //         JsonWebColor(pCONT_set->XdrvMailbox.data);
// // //       }
// // //     }
// // //     pCONT_sup->Response_P(PSTR("{\"" D_WEBCOLOR "\":["));
// // //     for (uint8_t i = 0; i < pCONT_set->COL_LAST; i++) {
// // //       if (i) { pCONT_sup->ResponseAppend_P(PSTR(",")); }
// // //       pCONT_sup->ResponseAppend_P(PSTR("\"#%06x\""), WebColor(i));
// // //     }
// // //     pCONT_sup->ResponseAppend_P(PSTR("]}"));
// // //   }
// // // #ifdef USE_EMULATION
// // //   else if (CMND_EMULATION == command_code) {
// // //     if ((XdrvMailbox.payload >= EMUL_NONE) && (XdrvMailbox.payload < EMUL_MAX)) {
// // //       pCONT_set->Settings.flag_power.emulation = XdrvMailbox.payload;
// // //       restart_flag = 2;
// // //     }
// // //     Response_P(S_JSON_COMMAND_NVALUE, command, pCONT_set->Settings.flag_power.emulation);
// // //   }
// // // #endif  // USE_EMULATION
// // //   else serviced = false;  // Unknown command

// //   return serviced;
// // }



// // void mWebServer::ExecuteCommand(char *cmnd, int source)
// // {
// // //   char stopic[CMDSZ];
// // //   char svalue[INPUT_BUFFER_SIZE];
// // //   char *start;
// // //   char *token;

// // //  //ShowFreeMem(PSTR("ExecuteCommand"));
// // //   pCONT_sup->ShowSource(source);

// // // //In the first call to the strtok() function for a given string1, the strtok() function searches for the first token in string1, 
// // // //skipping over leading delimiters. A pointer to the first token is returned.

// // //   token = strtok(cmnd, " "); // split, on each call returns next split
// // //   if (token != NULL) { //if end is not reached
// // //     start = strrchr(token, '/');   // Skip possible cmnd/sonoff/ preamble
// // //     if (start) { token = start +1; }
// // //   }
// // //   snprintf_P(stopic, sizeof(stopic), PSTR("%s"), (token == NULL) ? "" : token);
// // //   token = strtok(NULL, "");
// // // //  snprintf_P(svalue, sizeof(svalue), (token == NULL) ? "" : token);  // Fails with command FullTopic home/%prefix%/%topic% as it processes %p of %prefix%
// // //   strlcpy(svalue, (token == NULL) ? "" : token, sizeof(svalue));       // Fixed 5.8.0b
// // //   ALOG_INF(PSTR(D_LOG_COMMAND "ExecuteCommand" " stopic %s" " svalue %s" " strlen(svalue) %d" ),stopic,svalue,strlen(svalue));

// // //   //MqttDataHandler(stopic, (uint8_t*)svalue, strlen(svalue));
// // //   ParseAndExecuteWebCommands(stopic, (uint8_t*)svalue, strlen(svalue));
 
// // }


// /**
//  Commands have form: "<name><index> <value>" e.g."button2 1" or "slider3 123"
//  <name> is in format classname::command but can contain NO numbers (ascii string only)
// */
// // void mWebServer::ParseAndExecuteWebCommands(char* topic, uint8_t* data, unsigned int data_len)
// // {

// // //   if (data_len > MQTT_MAX_PACKET_SIZE) { return; }  // Do not allow more data than would be feasable within stack space

// // //   char *str;

// // //   char topicBuf[TOPSZ];
// // //   char dataBuf[data_len+1];
// // //   char command [CMDSZ];
// // //   char stemp1[TOPSZ];
// // //   char *p;
// // //   char *type = nullptr;
// // //   uint8_t lines = 1;
// // //   bool jsflg = false;
// // //   bool grpflg = false;
// // //   bool user_append_index = false;
// // //   uint32_t i = 0;
// // //   uint32_t index;
// // //   uint32_t address;
  
// // //   // memset(&webcommand,0,sizeof(webcommand));

// // // /******************************************************************************************************
// // //  *******Extract <name><index> as topicBuf, <value> as dataBuf using space delimeter ***********************************************************************************************
// // //  ******************************************************************************************************/
 
// // //   // "string numbervalue" - split into topic and data buffers
// // //   strlcpy(topicBuf, topic, sizeof(topicBuf));
// // //   for (i = 0; i < data_len; i++) {
// // //     if (!isspace(data[i])) { break; } 
// // //   }
// // //   data_len -= i;
// // //   memcpy(dataBuf, data +i, sizeof(dataBuf));
// // //   dataBuf[sizeof(dataBuf)-1] = 0;
  
// // //   //copy command value
// // //   memcpy(&webcommand.command_value,dataBuf,sizeof(dataBuf));

// // //   //if (topicBuf[0] != '/') { ShowSource(SRC_MQTT); } //?

// // //   ALOG_INF(PSTR(D_LOG_RESULT D_RECEIVED_TOPIC " %s, " D_DATA_SIZE " %d, " D_DATA " %s"), topicBuf, data_len, dataBuf);
 
// // //   // Check classname exists
// // //   if(strstr(topicBuf,"::")){
// // //     // copy classname
// // //     for (i = 0; i < strlen(topicBuf); i++) {
// // //       if (topicBuf[i]==':') { break; } 
// // //       webcommand.classname[i] = topicBuf[i];
// // //     }
// // //   }

// // //   //from i, we have the command_key_short (skip :: values)
// // //   memcpy(&webcommand.command_key_short,&topicBuf[i+2],sizeof(char)*(strlen(topicBuf)-i-2));

// // // /******************************************************************************************************
// // //  *******Extract <index> from topicBuf with isdigit ***********************************************************************************************
// // //  ******************************************************************************************************/
 
// // //   //type = strrchr(topicBuf, '/');  // Last part of received topic is always the command (type)
// // //   type = topicBuf;//, '/');  // Last part of received topic is always the command (type)

// // //   index = 1;
// // //   if (type != nullptr) {
// // //     type++;
// // //     // Convert everything to same case
// // //     for (i = 0; i < strlen(type); i++) {
// // //       //type[i] = toupper(type[i]);
// // //     }
// // //     // From end of array, move backwards checking for numbers (ie stops when it hits " " space)
// // //     while (isdigit(type[i-1])) {
// // //       i--;
// // //     }
// // //     // Move from start of number to end of array to get number
// // //     if (i < strlen(type)) {
// // //       index = atoi(type +i);
// // //       user_append_index = true;
// // //     }
// // //     type[i] = '\0';
// // //   }

// // //   // remove index from 
// // //   if(user_append_index){
// // //     webcommand.command_key_short[strlen(webcommand.command_key_short)-mSupport::NumDigits(index)] = 0;
// // //   }

// // //   //copy index
// // //   webcommand.command_index = index;

// // //   webcommand.fWaiting = true;
  
// // //   ALOG_DBG(PSTR(D_LOG_RESULT D_GROUP " %d, " D_INDEX " %d, " 
// // //     D_COMMAND " %s, " D_DATA " %s"),
// // //    grpflg, index, type, dataBuf);


// // //   ALOG_INF(PSTR(D_LOG_RESULT "WEBCOMMANDS " "classname %s"),webcommand.classname);
// // //   ALOG_INF(PSTR(D_LOG_RESULT "WEBCOMMANDS " "command_key_short %s"),webcommand.command_key_short);
// // //   ALOG_INF(PSTR(D_LOG_RESULT "WEBCOMMANDS " "command_value %s"),webcommand.command_value);
// // //   ALOG_INF(PSTR(D_LOG_RESULT "WEBCOMMANDS " "command_index %d"),webcommand.command_index);
  
// // //   pCONT->Tasker_Interface(TASK_WEB_COMMAND);

// // } // END function




// *********************************************************************************************
//  * Interface
// *********************************************************************************************

int8_t mWebServer::Tasker(uint8_t function, JsonParserObject obj)
{

// DEBUG_LINE_HERE;

  switch(function)
  {
    case TASK_INIT:
     
     server = new AsyncWebServer(80);
      
    break;
  }


  switch(function){
    case TASK_INIT:
      // init();


// DEBUG_LINE_HERE;
          
//       //CORS compatiblity
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

// DEBUG_LINE_HERE;
//       server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send(200, "text/plain", (String)millis());
//       });

// DEBUG_LINE_HERE;
//       server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){ 
//         handleConfig(request);
//       });

//   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//     // if (captivePortal(request)) return;
//     // if (!showWelcomePage || request->hasArg(F("sliders"))){
//     //   serveIndex(request);
//     // } else {
//     //   serveSettings(request);
//     // }

//       AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
    
// DEBUG_LINE_HERE;
//       request->send(response);
//   });

//     server.onNotFound(notFound);
// DEBUG_LINE_HERE;
// //     //called when the url is not defined here, ajax-in; get-settings
// //     server.onNotFound([](AsyncWebServerRequest *request){
// //       // DEBUG_PRINTLN("Not-Found HTTP call:");
// //       // DEBUG_PRINTLN("URI: " + request->url());
// //       // if (captivePortal(request)) return;

// //       // //make API CORS compatible
// //       // if (request->method() == HTTP_OPTIONS)
// //       // {
// //       //   AsyncWebServerResponse *response = request->beginResponse(200);
// //       //   response->addHeader(F("Access-Control-Max-Age"), F("7200"));
// //       //   request->send(response);
// //       //   return;
// //       // }

// // DEBUG_LINE_HERE;
// //       // if(handleSet(request, request->url())) return;
// //       // #ifndef WLED_DISABLE_ALEXA
// //       // if(espalexa.handleAlexaApiCall(request)) return;
// //       // #endif
// //       // if(handleFileRead(request, request->url())) return;
// //       AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
// //       // response->addHeader(FPSTR(s_content_enc),"gzip");
// //       // setStaticContentCacheHeaders(response);
// // DEBUG_LINE_HERE;
// //       request->send(response);
// //     });

// DEBUG_LINE_HERE;

//     server.begin();

    break;
  //   case TASK_LOOP:
  //     PollDnsWebserver();
  //   break; 
  case TASK_WIFI_CONNECTED:


    #ifdef ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE

    initServer();

    pCONT_web->server->begin();

    #endif
// DEBUG_LINE_HERE;
// return 0;
    // server.begin();
    
// DEBUG_LINE_HERE;
          
//       //CORS compatiblity
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
//       DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

// DEBUG_LINE_HERE;
//       server.on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
//         request->send(200, "text/plain", (String)millis());
//       });

// DEBUG_LINE_HERE;
//       server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request){ 
//         handleConfig(request);
//       });



  break;
  //   case TASK_EVERY_SECOND:

  //   break;
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
  //   case TASK_WEB_APPEND_RUNTIME_ROOT_URLS:
  //       JBI->Add("/module_draw.json",-1); 
  //       // JBI->Add("/web_top_bar.json",1000); 
  //       JBI->Add("/root_status_any.json",1100);
  //   break;
  //   case TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED:
  //     //set value to force sending all below
  //   //no break
  //   case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:


  //   break;
  }

  return 0;

// DEBUG_LINE_HERE;
}




#ifdef ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE


void mWebServer::initServer()
{
  //CORS compatiblity
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

  pCONT_web->server->on("/root", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleRoot(request);
  });
  
  // releaseJSONBufferLock();

  // #ifdef WLED_ENABLE_WEBSOCKETS
  // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  // pCONT_web->server->on("/liveview2D", HTTP_GET, [](AsyncWebServerRequest *request){
  //   if (handleIfNoneMatchCacheHeader(request)) return;
  //   AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_liveviewws2D, PAGE_liveviewws2D_length);
  //   response->addHeader(FPSTR(s_content_enc),"gzip");
  //   setStaticContentCacheHeaders(response);
  //   request->send(response);
  // });
  // #endif
  // #endif
  
  // pCONT_web->server->on("/liveview", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   if (this->handleIfNoneMatchCacheHeader(request)) return;
  //   AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_liveview, PAGE_liveview_length);
  //   response->addHeader(FPSTR(s_content_enc),"gzip");
  //   this->setStaticContentCacheHeaders(response);
  //   request->send(response);
  // });

  // //settings page
  // pCONT_web->server->on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   this->serveSettings(request);
  // });

  // // "/settings/settings.js&p=x" request also handled by serveSettings()

  // pCONT_web->server->on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   if (handleIfNoneMatchCacheHeader(request)) return;
  //   AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", PAGE_settingsCss, PAGE_settingsCss_length);
  //   response->addHeader(FPSTR(s_content_enc),"gzip");
  //   this->setStaticContentCacheHeaders(response);
  //   request->send(response);
  // });

  // pCONT_web->server->on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   if(!handleFileRead(request, "/favicon.ico"))
  //   {
  //     request->send_P(200, "image/x-icon", favicon, 156);
  //   }
  // });

  // pCONT_web->server->on("/welcome", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   this->serveSettings(request);
  // });

  // pCONT_web->server->on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   this->serveMessage(request, 200,F("Rebooting now..."),F("Please wait ~10 seconds..."),129);
  //   // doReboot = true;
  // });

  // pCONT_web->server->on("/settings", HTTP_POST, [this](AsyncWebServerRequest *request){
  //   this->serveSettings(request, true);
  // });

  // pCONT_web->server->on("/json", HTTP_GET, [this](AsyncWebServerRequest *request){
  //   this->serveJson(request);
  // });

  // AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/json", [this](AsyncWebServerRequest *request) {
  //   bool verboseResponse = false;
  //   bool isConfig = false;

  //   ALOG_INF(PSTR("AsyncCallbackJsonWebHandler"));

  //   Serial.println((char*)request->_tempObject);

  //   if (!this->requestJSONBufferLock(14)) return;

  //   DeserializationError error = deserializeJson(doc, (uint8_t*)(request->_tempObject));
  //   JsonObject root = doc.as<JsonObject>();
  //   if (error || root.isNull()) {
  //     this->releaseJSONBufferLock();
  //     request->send(400, "application/json", F("{\"error\":9}")); // ERR_JSON
  //     return;
  //   }
  //   // if (root.containsKey("pin")) checkSettingsPIN(root["pin"].as<const char*>());

  //   const String& url = request->url();
  //   isConfig = url.indexOf("cfg") > -1; 
  //   if (!isConfig) {
  //     /*
  //     #ifdef WLED_DEBUG
  //       DEBUG_PRINTLN(F("Serialized HTTP"));
  //       serializeJson(root,Serial);
  //       DEBUG_PRINTLN();
  //     #endif
  //     */
  //     ALOG_INF(PSTR("deserializeState"));
  //     verboseResponse = this->deserializeState(root);
  //   } else {
  //     if (!correctPIN && strlen(settingsPIN)>0) {
  //       request->send(403, "application/json", F("{\"error\":1}")); // ERR_DENIED
  //       this->releaseJSONBufferLock();
  //       return;
  //     }
  //   ALOG_INF(PSTR("deserializeConfig"));
  //     verboseResponse = this->deserializeConfig(root); //use verboseResponse to determine whether cfg change should be saved immediately
  //   }
  //   this->releaseJSONBufferLock();

  //   if (verboseResponse) {
  //     if (!isConfig) {
  //       this->serveJson(request); return; //if JSON contains "v"
  //     } else {
  //       // doSerializeConfig = true; //serializeConfig(); //Save new settings to FS
  //     }
  //   }
  //   request->send(200, "application/json", F("{\"success\":true}"));
  // }, JSON_BUFFER_SIZE);
  // pCONT_web->server->addHandler(handler);

  pCONT_web->server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)PROJECT_VERSION);
  });

  pCONT_web->server->on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)millis());
  });

//   pCONT_web->server->on("/freeheap", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send(200, "text/plain", (String)ESP.getFreeHeap());
//   });


  pCONT_web->server->on("/reboot", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleReboot(request);
  });

// #ifdef WLED_ENABLE_USERMOD_PAGE
//   pCONT_web->server->on("/u", HTTP_GET, [](AsyncWebServerRequest *request){
//     if (handleIfNoneMatchCacheHeader(request)) return;
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_usermod, PAGE_usermod_length);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     setStaticContentCacheHeaders(response);
//     request->send(response);
//   });
// #endif

//   pCONT_web->server->on("/teapot", HTTP_GET, [this](AsyncWebServerRequest *request){
//     this->serveMessage(request, 418, F("418. I'm a teapot."), F("(Tangible Embedded Advanced Project Of Twinkling)"), 254);
//   });

//   pCONT_web->server->on("/upload", HTTP_POST, [this](AsyncWebServerRequest *request) {},
//         [this](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
//                       size_t len, bool final) {this->handleUpload(request, filename, index, data, len, final);}
//   );

// #ifdef WLED_ENABLE_SIMPLE_UI
//   pCONT_web->server->on("/simple.htm", HTTP_GET, [](AsyncWebServerRequest *request){
//     if (handleFileRead(request, "/simple.htm")) return;
//     if (handleIfNoneMatchCacheHeader(request)) return;
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_simple, PAGE_simple_L);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     setStaticContentCacheHeaders(response);
//     request->send(response);
//   });
// #endif

//   pCONT_web->server->on("/iro.js", HTTP_GET, [this](AsyncWebServerRequest *request){
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", iroJs, iroJs_length);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     this->setStaticContentCacheHeaders(response);
//     request->send(response);
//   });

//   pCONT_web->server->on("/rangetouch.js", HTTP_GET, [this](AsyncWebServerRequest *request){
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", rangetouchJs, rangetouchJs_length);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     this->setStaticContentCacheHeaders(response);
//     request->send(response);
//   });

  createEditHandler(true);//correctPIN);

// #ifndef WLED_DISABLE_OTA
//   //init ota page
//   pCONT_web->server->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request){
//     ALOG_INF(PSTR("URL HTTP_GET \"/update\""));
//     // if (otaLock) {
//     //   serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_ota), 254);
//     // } else
//       this->serveSettings(request); // checks for "upd" in URL and handles PIN
//   });

//   pCONT_web->server->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request){
//     ALOG_INF(PSTR("URL HTTP_POST \"/update\""));
//     // if (!correctPIN) {
//     //   serveSettings(request, true); // handle PIN page POST request
//     //   return;
//     // }
//     if (Update.hasError() || otaLock) {
//       this->serveMessage(request, 500, F("Update failed!"), F("Please check your file and retry!"), 254);
//     } else {
//       this->serveMessage(request, 200, F("Update successful!"), F("Rebooting..."), 131);
//       // doReboot = true;
//     }
//   },[this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
//     if (!this->correctPIN || this->otaLock) return;
//     if(!index){
//       DEBUG_PRINTLN(F("OTA Update Start"));
//       // WLED::instance().disableWatchdog();
//       // usermods.onUpdateBegin(true); // notify usermods that update is about to begin (some may require task de-init)
//       // lastEditTime = millis(); // make sure PIN does not lock during update
//       #ifdef ESP8266
//       Update.runAsync(true);
//       #endif
//       Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000);
//     }
//     if(!Update.hasError()) Update.write(data, len);
//     if(final){
//       if(Update.end(true)){
//         DEBUG_PRINTLN(F("Update Success"));
//       } else {
//         DEBUG_PRINTLN(F("Update Failed"));
//         // usermods.onUpdateBegin(false); // notify usermods that update has failed (some may require task init)
//         // WLED::instance().enableWatchdog();
//       }
//     }
//   });
// #else
//   pCONT_web->server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
//     serveMessage(request, 501, "Not implemented", F("OTA updating is disabled in this build."), 254);
//   });
// #endif


//   #ifdef WLED_ENABLE_DMX
//   pCONT_web->server->on("/dmxmap", HTTP_GET, [](AsyncWebServerRequest *request){
//     request->send_P(200, "text/html", PAGE_dmxmap     , dmxProcessor);
//   });
//   #else
//   pCONT_web->server->on("/dmxmap", HTTP_GET, [this](AsyncWebServerRequest *request){
//     this->serveMessage(request, 501, "Not implemented", F("DMX support is not enabled in this build."), 254);
//   });
//   #endif

//   pCONT_web->server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
//     // if (captivePortal(request)) return;
//     bool showWelcomePage = false;
//     if (!showWelcomePage || request->hasArg(F("sliders"))){
//       this->serveIndex(request);
//     } else {
//       this->serveSettings(request);
//     }
//   });

//   #ifdef WLED_ENABLE_PIXART
//   pCONT_web->server->on("/pixart.htm", HTTP_GET, [](AsyncWebServerRequest *request){
//     if (handleFileRead(request, "/pixart.htm")) return;
//     if (handleIfNoneMatchCacheHeader(request)) return;
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_pixart, PAGE_pixart_L);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     setStaticContentCacheHeaders(response);
//     request->send(response);
//   });
//   #endif

//   #ifndef WLED_DISABLE_PXMAGIC
//   pCONT_web->server->on("/pxmagic.htm", HTTP_GET, [this](AsyncWebServerRequest *request){
//     if (this->handleFileRead(request, "/pxmagic.htm")) return;
//     if (this->handleIfNoneMatchCacheHeader(request)) return;
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_pxmagic, PAGE_pxmagic_L);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     this->setStaticContentCacheHeaders(response);
//     request->send(response);
//   });
//   #endif

//   pCONT_web->server->on("/cpal.htm", HTTP_GET, [this](AsyncWebServerRequest *request){
//     if (this->handleFileRead(request, "/cpal.htm")) return;
//     if (this->handleIfNoneMatchCacheHeader(request)) return;
//     AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_cpal, PAGE_cpal_L);
//     response->addHeader(FPSTR(s_content_enc),"gzip");
//     this->setStaticContentCacheHeaders(response);
//     request->send(response);
//   });

//   #ifdef WLED_ENABLE_WEBSOCKETS
//   pCONT_web->server->addHandler(&ws);
//   #endif


  // pCONT->Tasker_Interface(TASK_WEB_ADD_HANDLER);/
  pCONT->Tasker_Interface(TASK_WEB_ADD_HANDLER);




}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mWebServer::webHandleReboot(AsyncWebServerRequest* request)
{ 

  // CAUTION: This is an async file, and may cause unknown behavior so needs testing

  pCONT->Tasker_Interface(TASK_RESTART_STABLE);
  
  request->redirect("/"); // redirect to root, to avoid webui forever requested a reboot

  delay(2000); // Allow redirect to be sent

#ifdef ESP32
  esp_restart();
#else

#endif
}


void mWebServer::webHandleRoot(AsyncWebServerRequest* request)
{ 

#ifdef ESP32
  ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK "HTTP: Sending root page to client connected from: %s"), request->host());
#endif


  String conv = String();

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone ");
  conv += httpHeader;
  conv += FPSTR(HTTP_SCRIPT3);
  conv += FPSTR(HTTP_STYLE3);
  conv += FPSTR(HASP_STYLE);
  conv += FPSTR(HTTP_HEAD_END3);

  conv += (F("<br/><hr><button type='submit'>save settings</button></form>"));

  conv += (F("<hr><form method='get' action='firmware'>"));
  conv += (F("<button type='submit'>update firmware</button></form>"));

  conv += (F("<hr><form method='get' action='reboot'>"));
  conv += (F("<button type='submit'>reboot device</button></form>"));

  conv += (F("<hr><form method='get' action='resetConfig'>"));
  conv += (F("<button type='submit'>factory reset settings</button></form>"));

  conv += FPSTR(HTTP_END3);
  
  request->send(200, "text/html", conv);

}


bool  mWebServer::captivePortal(AsyncWebServerRequest *request)
{
  if (ON_STA_FILTER(request)) return false; //only serve captive in AP mode
  String hostH;
  if (!request->hasHeader("Host")) return false;
  hostH = request->getHeader("Host")->value();

  if (!isIp(hostH) && hostH.indexOf("wled.me") < 0 && hostH.indexOf(pCONT_set->Settings.system_name.device) < 0) {
    DEBUG_PRINTLN("Captive portal");
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader(F("Location"), F("http://4.3.2.1"));
    request->send(response);
    return true;
  }
  return false;
}

void mWebServer::setStaticContentCacheHeaders(AsyncWebServerResponse *response)
{
  char tmp[40];
  // https://medium.com/@codebyamir/a-web-developers-guide-to-browser-caching-cc41f3b73e7c
  #ifndef WLED_DEBUG
  // This header name is misleading, "no-cache" will not disable cache,
  // it just revalidates on every load using the "If-None-Match" header with the last ETag value
  response->addHeader(F("Cache-Control"),"no-cache");
  #else
  response->addHeader(F("Cache-Control"),"no-store,max-age=0"); // prevent caching if debug build
  #endif
  snprintf_P(tmp, sizeof(tmp), PSTR("%d-%02x"), PROJECT_VERSION, false);// tkr_anim->cacheInvalidate);
  response->addHeader(F("ETag"), tmp);
}


//Is this an IP?
bool mWebServer::isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}



#endif // ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE




#endif