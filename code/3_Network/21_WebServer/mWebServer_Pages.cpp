// File contains the formation of pages, while "mWebServer" contains all parsing and commands
// Mostly for refactoring

#include "mWebServer.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER21

// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Shared resources
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// void mWebServer::WebSend_JSON_RootStatus_Table(AsyncWebServerRequest *request){

//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     pCONT->Tasker_Interface(TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED);
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// }


// void mWebServer::Web_Base_Page_Draw(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     WebAppend_Root_Draw_PageTitleFields();
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mWebServer::WebSend_JSON_WebServer_TopBar(AsyncWebServerRequest *request){
    
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 

//   char buffer[30];

//   JBI->Start();
//   JBI->Array_Start("info_row");// Class name
//     for(int row=0;row<4;row++){
//     JBI->Object_Start();
//       JBI->Add("id",row);
//       switch(row){
//         case 0:
//           JBI->Add_FV("ih",PSTR("\"%s U%s\""), tkr_time->RtcTime.hhmmss_ctr, tkr_time->uptime.hhmmss_ctr);
//           JBI->Add("fc", tkr_time->uptime_seconds_nonreset<SEC_IN_HOUR?PSTR("#ff0000"):PSTR("#ffffff"));    
//         break;
//         case 1:{        
//           int8_t wifi_perc = pCONT_wif->GetRSSPercentage();
//           char colour_ctr[7];
//           if(wifi_perc<20){      sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ff0000")); }
//           else if(wifi_perc<30){ sprintf_P(colour_ctr,PSTR("%s"),PSTR("#fcba03")); }
//           else{                  sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ffffff")); }
//           JBI->Add_FV("ih",PSTR("\"%s %d%% (%d&nbsp;dBm)\""), WiFi.SSID().c_str(),wifi_perc,pCONT_wif->GetRSSdBm());
//           JBI->Add("fc", colour_ctr);    
//         }break;
//         case 2:
//           JBI->Add("ihr",tkr_set->runtime.firmware_version.current.name_ctr);
//           JBI->Add("fc", pCONT_sup->GetVersionColour(buffer));    

//         break;
//         case 3:
//             JBI->Add_FV("ih",PSTR("\"ROW3%dc %d %s|%s PT(%s) LPS(%d)\""), 
//                 tkr_set->Settings.bootcount, 
//                 ESP.getFreeHeap(), 
//                 F(__DATE__), 
//                 F(__TIME__), 
//                 tkr_set->runtime.boot_status.module_template_used ? "Y" : "N", 
//                 pCONT_sup->activity.cycles_per_sec
//             );
//         break;
//       } //end switch 
//    JBI->Object_End();
//    } // end for

//   JBI->Array_End();
  
//   // JBI->Array_Start("debug_line");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add_FV("ih",PSTR("\"%dc %d %s|%s PT(%s) LPS(%d)\""), tkr_set->Settings.bootcount, ESP.getFreeHeap(), F(__DATE__), F(__TIME__), tkr_set->runtime.boot_status.module_template_used ? "Y" : "N", pCONT_sup->activity.cycles_per_sec);
//   //   JBI->Object_End();
//   // JBI->Array_End();
//   JBI->End();


//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } // end function




// void mWebServer::WebSend_JSON_WebServer_StatusPopoutData(AsyncWebServerRequest *request){
    
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 

//   char buffer[30];

//   JBI->Start();
//   JBI->Array_Start("info_row");// Class name
//     for(int row=0;row<6;row++){
//     JBI->Object_Start();
//       JBI->Add("id",row);
//       switch(row){
//         case 0:
//           JBI->Add_FV("ih",PSTR("\"%s U%s\""), tkr_time->RtcTime.hhmmss_ctr, tkr_time->uptime.hhmmss_ctr);
//           JBI->Add("fc", tkr_time->uptime_seconds_nonreset<SEC_IN_HOUR?PSTR("#ff0000"):PSTR("#ffffff"));    
//         break;
//         case 1:{        
//           int8_t wifi_perc = pCONT_wif->GetRSSPercentage();
//           char colour_ctr[7];
//           if(wifi_perc<20){      sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ff0000")); }
//           else if(wifi_perc<30){ sprintf_P(colour_ctr,PSTR("%s"),PSTR("#fcba03")); }
//           else{                  sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ffffff")); }
//           JBI->Add_FV("ih",PSTR("\"%s %d%% (%d&nbsp;dBm)\""), WiFi.SSID().c_str(),wifi_perc,pCONT_wif->GetRSSdBm());
//           JBI->Add("fc", colour_ctr);   
//         }break;
//         case 2:
//           JBI->Add("ihr",tkr_set->runtime.firmware_version.current.name_ctr);
//           JBI->Add("fc", pCONT_sup->GetVersionColour(buffer));    
//         break;
//         case 3:
//           JBI->Add_FV("ih",PSTR("\"Boot: %dc PT(%s)\""), tkr_set->Settings.bootcount, tkr_set->runtime.boot_status.module_template_used ? "Y" : "N");
//         break;
//         case 4:
//           JBI->Add_FV("ih",PSTR("\"Firmware: %s %s|%s\""), tkr_set->runtime.firmware_version.current.name_ctr, F(__DATE__), F(__TIME__));
//           JBI->Add("fc", pCONT_sup->GetVersionColour(buffer));    
//         break;
//         case 5:
//           JBI->Add_FV("ih",PSTR("\"Runtime: LPS(%d Hz | %d ms) FreeHeap:%d\""), pCONT_sup->activity.cycles_per_sec, 1000/pCONT_sup->activity.cycles_per_sec, ESP.getFreeHeap());
//         break;
//         case 6:
//           JBI->Add_FV("ih",PSTR("\"Templates: M%d R%d L%d D%d\""), 0, 0, 0, 0);
//         break;
//       } //end switch 
//     JBI->Object_End();
//     } // end for

//   JBI->Array_End();
  
//   // JBI->Array_Start("debug_line");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add_FV("ih",PSTR("\"%dc %d %s|%s PT(%s) LPS(%d)\""), tkr_set->Settings.bootcount, ESP.getFreeHeap(), F(__DATE__), F(__TIME__), tkr_set->runtime.boot_status.module_template_used ? "Y" : "N", pCONT_sup->activity.cycles_per_sec);
//   //   JBI->Object_End();
//   // JBI->Array_End();
//   JBI->End();


//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } // end function



// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Root Page
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// void mWebServer::HandlePage_Root(AsyncWebServerRequest *request){

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_TST(PSTR("mWebServer::HandlePage_Root"));
//     #endif //  ENABLE_LOG_LEVEL_INFO

//   //AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_ASYNC WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER_RUNTIME_URLS_RATES_VAR "Sf::%s"),"HandlePage_Root");

//   // if (CaptivePortal(request)) { return; }  // If captive portal redirect instead of displaying the page.

//   // if (WifiIsInManagerMode()) {
//   //   #ifndef FIRMWARE_MINIMAL
//   //     if (
//   //         (tkr_set->Settings.web_password[0] != 0) && 
//   //         !(request->hasParam("USER1")) && !(request->hasParam("PASS1")
//   //       ) && HTTP_MANAGER_RESET_ONLY != webserver_state) {
//   //       HandleWifiLogin(request);
//   //     } else {
//   //       if (!(tkr_set->Settings.web_password[0] != 0) || 
//   //           (((request->arg("USER1") == WEB_USERNAME ) && (request->arg("PASS1") == tkr_set->Settings.web_password )) || HTTP_MANAGER_RESET_ONLY == webserver_state)) {
//   //         HandleWifiConfiguration(request);
//   //       } else {
//   //         // wrong user and pass
//   //         HandleWifiLogin(request);
//   //       }
//   //     }
//   //   #endif  // Not FIRMWARE_MINIMAL
//   //   return;
//   // }
  
//   // delay(1000);


//   // JBI->Start();
//   //   JBI->Object_Start("function");
//   //     JBI->Object_Start("Parse_Urls");
//   //       // pCONT->Tasker_Interface(TASK_WEB_APPEND_RUNTIME_ROOT_URLS);
//   //     JBI->Object_End();
//   //   JBI->Object_End();
//   // JBI->End();

//   // request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);

//   AsyncWebServerResponse *response = request->beginResponse_P(200, CONTENT_TYPE_TEXT_HTML_ID, PAGE_ROOT, PAGE_ROOT_L);
//   response->addHeader("Content-Encoding","gzip");
//   request->send(response);

// }


// void mWebServer::Web_Root_Draw(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     WebAppend_Root_Draw_PageTitleFields();
//     WebAppend_Root_Draw_PageTable();
//     WebAppend_Root_Draw_PageButtons();
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mWebServer::Web_Root_Draw_Modules(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     WebAppend_Root_Draw_ModuleTable();
//     WebAppend_Root_Draw_ModuleButtons();
//   JBI->End();
  
//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  
  
// } //end function


// //append to internal buffer if any root messages table
// void mWebServer::WebAppend_Root_Draw_ModuleButtons(){

//   JBI->Array_Start("container_4");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"");
//         pCONT->Tasker_Interface(TASK_WEB_APPEND_ROOT_BUTTONS);
//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();

// }

// void mWebServer::WebAppend_Root_Draw_PageButtons(){
  
//   // JBI->Array_Start("container_5");// Class name
//   //   JBI->Object_Start();
//   //     JBI->AddKey("ihr");           // function
//   //       JBI->AppendBuffer("\"");
//   //       WebAppend_Button_Spaced(BUTTON_CONSOLE);
//   //       WebAppend_Button(BUTTON_SYSTEM_SETTINGS);
//   //     JBI->AppendBuffer("\"");
//   //   JBI->Object_End();
//   // JBI->Array_End();

// }


// void mWebServer::Web_Root_UpdateURLs(AsyncWebServerRequest *request){
  
//   JBI->Start();
//     JBI->Object_Start("function");
//       JBI->Object_Start("Parse_Urls");
//         pCONT->Tasker_Interface(TASK_WEB_APPEND_RUNTIME_ROOT_URLS);
//       JBI->Object_End();
//     JBI->Object_End();
//   JBI->End();

//   request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
// }



// void mWebServer::WebAppend_Root_Draw_PageTitleFields(){

//   JBI->Array_Start("page_title");// Class name
//     JBI->Object_Start();
//       JBI->Add("ihr",tkr_set->Settings.system_name.friendly);
//       JBI->Add("fc", D_COLOUR_PAGE_TITLE);           
//     JBI->Object_End();
//   JBI->Array_End();

//   // char buffer[10];
//   // JBI->Array_Start("row_version_data");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add("ihr",tkr_set->runtime.firmware_version.current.name_ctr);
//   //     JBI->Add("fc", pCONT_sup->GetVersionColour(buffer));           
//   //   JBI->Object_End();
//   // JBI->Array_End();

//   // JBI->Object_Start("function");
//   //     JBI->Add("SetTitle",tkr_set->Settings.system_name.friendly);
//   // JBI->Object_End();  

  
//   // JBI->Array_Start("function");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add("SetTitle","Heelo");//tkr_set->Settings.system_name.friendly);
//   //   JBI->Object_End();
//   // JBI->Array_End();


// }

// void mWebServer::WebAppend_Root_Draw_PageTable(){

//   JBI->Array_Start("container_2");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"{t}");
//         pCONT->Tasker_Interface(TASK_WEB_ADD_ROOT_TABLE_ROWS);
//       JBI->AppendBuffer("{t2}\"");
//     JBI->Object_End();
//   JBI->Array_End();

// }


// void mWebServer::WebAppend_Root_Draw_ModuleTable(){

//   JBI->Array_Start("container_3");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"");
//         pCONT->Tasker_Interface(TASK_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER);
//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();

// }


/*************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
   * Console Page
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
  ************************************************************************************************************************************************************************************
*/

void mWebServer::HandlePage_Console(AsyncWebServerRequest *request){

  fConsole_active = true;

  if (!HttpCheckPriviledgedAccess()) { return; }
  
  if (request->hasParam("c2")) {      // Console refresh requested
    HandleConsoleRefresh(request);
    return;
  }

  // request->send_P(200,CONTENT_TYPE_TEXT_HTML_ID,PAGE_ROOT);
  // return;

  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_ROOT, PAGE_ROOT_L);

  response->addHeader("Content-Encoding","gzip");
  
  request->send(response);

}


// void mWebServer::Web_Console_Draw(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
    
//   JBI->Array_Start("container_1");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"");
//         JBI->AppendBuffer(PSTR("<fieldset><legend><b>&nbsp;Web Commands&nbsp;</b></legend>"));
//         JBI->AppendBuffer(PSTR("<textarea readonly='' id='console_textbox' cols='340' wrap='off' name='console_textbox'></textarea>"
//           "<br><br>"
//           "<form method='get' onsubmit='return l(1);'>"
//               "<input id='c1'  style='background:#1d1d1d' placeholder='Enter Module Name eg pixels' autofocus='' name='c1'>"
//               "<br>"
//           "</form>")
//         );
//         JBI->AppendBuffer(PSTR(
//           "<form method='get' onsubmit='return l(1);'>"
//           "<input id='com_web' name='com_web' style='background:#1d1d1d' placeholder='" "Enter command eg {name:value} or name value'" "' autofocus><br/>"
//             "<button  class='buttonh bform1' type='submit'>Execute command</button>"
//           "</form>"
//         ));            
//       JBI->AppendBuffer(PSTR("</fieldset>"));
//       // topic = module name only, in code, add "set/modulename"
//       //payload = json message for multple inputs, OR, single input where {"a":"b"} can simply be "a b"

//       JBI->AppendBuffer(PSTR("<fieldset>"));
//         JBI->AppendBuffer(PSTR("<legend><b>&nbsp;MQTT Commands&nbsp;</b></legend>"));
//         JBI->AppendBuffer(PSTR(
//         "<form method='get' onsubmit='return l(1);'>"
//         "<input id='com_top' name='com_top' style='background:#1d1d1d' placeholder='" "Enter topic" "' autofocus><br/>"
//         "</form>" ));
//         JBI->AppendBuffer(PSTR(
//         "<form method='get' onsubmit='return l(1);'>"
//         "<input id='com_pay' name='com_pay' style='background:#1d1d1d' placeholder='" "Enter payload" "' autofocus><br/>"
//         "<button class='buttonh bform1' type='submit'>Execute Command</button>"
//         "</form>"  ));
//       JBI->AppendBuffer(PSTR("</fieldset>"));

//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();

//   JBI->Array_Start("container_5");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"");
//         WebAppend_Button_Spaced(BUTTON_MAIN);
//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();
    
//   JBI->Array_Start("function");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("Parse_AddScript");
//         JBI->AppendBuffer("\"");
//         JBI->AppendBuffer(PSTR(
//           "set_console_as_page();"
//           "enable_get_console_data();"
//         )
//       );
//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();
    
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function

void mWebServer::HandleConsoleRefresh(AsyncWebServerRequest *request)
{
  bool cflg = true;
  uint8_t counter = 0;                // Initial start, should never be 0 again

  // String svalue = request->arg("c1");
  // if (svalue.length() && (svalue.length() < INPUT_BUFFER_SIZE)) {
  //   ALOG_INF(PSTR(D_LOG_COMMAND "%s"), svalue.c_str());
  //   ExecuteWebCommand((char*)svalue.c_str(), SRC_WEBCONSOLE);
  // }

  char stmp[8];
  WebGetArg(request,"c2", stmp, sizeof(stmp));
  if (strlen(stmp)) { counter = atoi(stmp); }

  BufferWriterI->Start();

  BufferWriterI->Append_P(
    PSTR(
      "%d" //web_log_index
      "}1"
      "%d" //reset_web_log_flag
      "}1")
    , tkr_set->web_log_index, reset_web_log_flag);

  if (!reset_web_log_flag) {
    counter = 0;                  //reset counter from webpage 
    reset_web_log_flag = true;
  }
  if (counter != tkr_set->web_log_index) {   //if webpage counter does not match internal counter
    if (!counter) {    //and counter is not FIRST position
      counter = tkr_set->web_log_index;  //use internal counter
      cflg = false;     //no NEW line
    }

    // get the webindex, and get all internal indexes until internal catches up with web
    do {
      char* tmp;
      size_t len;
      pCONT_log->GetLog(counter, &tmp, &len);
      if (len) { //if there is new log data
      // and is not larger than buffer
        if (len > sizeof(data_buffer.payload.ctr) -2) { len = sizeof(data_buffer.payload.ctr); }
        char stemp[len +1]; //leak!
        strlcpy(stemp, tmp, len);
        // add new line if not first, then text
        BufferWriterI->Append_P(PSTR("%s%s"), (cflg) ? "\n" : "", stemp);
        cflg = true;
      }
      counter++; //internal counter
      if (!counter) { counter++; }  // Skip log index 0 as it is not allowed
      if(counter>100) break;
    } while (counter != tkr_set->web_log_index);

  }

  BufferWriterI->Append_P(PSTR("}1"));
  
  request->send(200,CONTENT_TYPE_TEXT_HTML_ID,data_buffer.payload.ctr);
 
}

// void mWebServer::Console_JSON_Data(AsyncWebServerRequest *request){

//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
    
//   JBI->Array_Start("function");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("Append_Console");

//         char buffer[500];
//         // sprintf(buffer, "{'link':1,'text':'hello','reset':0}");
//         sprintf(buffer, "{\\\"link\\\":1}");

// // I need to think how ANYTHING can pass through serial, json might not work


//         // sprintf(buffer, "{}");
//         JBI->AppendBuffer("\"");
//         JBI->AppendBuffer(buffer);
//         JBI->AppendBuffer("\"");

//     JBI->Object_End();
//   JBI->Array_End();
        
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  




// }




// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Information Page
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */


// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Configuration and Templates
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */



// void mWebServer::HandleWifiConfiguration(AsyncWebServerRequest *request)
// {
// //   //if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

// //   ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_WIFI);

// //   if (request->hasParam("save") && HTTP_MANAGER_RESET_ONLY != webserver_state) {
// //     WifiSaveSettings(request);
// //     WebRestart(request, 2);
// //     return;
// //   }

// //   WSStartAppend_P(request, S_CONFIGURE_WIFI, !WifiIsInManagerMode());
// //   WSBufferAppend_P(response, HTTP_SCRIPT_WIFI);
// //   WSContentSendStyle(request);

// //   if (HTTP_MANAGER_RESET_ONLY != webserver_state) {
// //     if (request->hasParam("scan")) {
// // #ifdef USE_EMULATION
// //       //UdpDisconnect();
// // #endif  // USE_EMULATION
// //       int n = WiFi.scanNetworks();
// //       ALOG_DBG(PSTR(D_LOG_WIFI D_SCAN_DONE));

// //       if (0 == n) {
// //         ALOG_DBG(S_LOG_WIFI, S_NO_NETWORKS_FOUND);
// //         WSBufferAppend_P(response, S_NO_NETWORKS_FOUND);
// //         WSBufferAppend_P(response, PSTR(". " D_REFRESH_TO_SCAN_AGAIN "."));
// //       } else {
// //         //sort networks
// //         int indices[n];
// //         for (int i = 0; i < n; i++) {
// //           indices[i] = i;
// //         }

// //         // RSSI SORT
// //         for (int i = 0; i < n; i++) {
// //           for (int j = i + 1; j < n; j++) {
// //             if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
// //               std::swap(indices[i], indices[j]);
// //             }
// //           }
// //         }

// //         // remove duplicates ( must be RSSI sorted )
// //         if (remove_duplicate_access_points) {
// //           String cssid;
// //           for (int i = 0; i < n; i++) {
// //             if (-1 == indices[i]) { continue; }
// //             cssid = WiFi.SSID(indices[i]);
// //             for (int j = i + 1; j < n; j++) {
// //               if (cssid == WiFi.SSID(indices[j])) {
// //                 ALOG_DBG(PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
// //                 indices[j] = -1;  // set dup aps to index -1
// //               }
// //             }
// //           }
// //         }

// //         //display networks in page
// //         for (int i = 0; i < n; i++) {
// //           if (-1 == indices[i]) { continue; }  // skip dups
// //           ALOG_DBG(PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"), WiFi.SSID(indices[i]).c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), WiFi.RSSI(indices[i]));
          
// //           int quality = pCONT_wif->WifiGetRssiAsQuality(WiFi.RSSI(indices[i]));
// //           int rss = WiFi.RSSI(indices[i]);

// //           if (minimum_signal_quality == -1 || minimum_signal_quality < quality) {
// //             uint8_t auth = WiFi.encryptionType(indices[i]);
            
// //             #ifdef ESP8266
// //             WSBufferAppend_P(response, PSTR("<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%s %d dBm (%d%%)</span></div>"),
// //               htmlEscape(WiFi.SSID(indices[i])).c_str(),
// //               WiFi.channel(indices[i]),
// //               (ENC_TYPE_WEP == auth) ? D_WEP : (ENC_TYPE_TKIP == auth) ? D_WPA_PSK : (ENC_TYPE_CCMP == auth) ? D_WPA2_PSK : (ENC_TYPE_AUTO == auth) ? D_AUTO : "",
// //               rss,
// //               quality
// //             );
// //             #endif
// //             delay(0);
// //           } else {
// //             ALOG_DBG(PSTR(D_LOG_WIFI D_SKIPPING_LOW_QUALITY));
// //           }

// //         }
// //         WSBufferAppend_P(response, PSTR("<br/>"));
// //       }
// //     } else {
// //       WSBufferAppend_P(response, PSTR("<div><a href='/wi?scan='>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br/>"));
// //     }

// //     // As WIFI_HOSTNAME may contain %s-%04d it cannot be part of HTTP_FORM_WIFI where it will exception
// //     WSBufferAppend_P(response, HTTP_FORM_WIFI, tkr_set->Settings.sta_ssid[0], tkr_set->Settings.sta_ssid[1], WIFI_HOSTNAME, WIFI_HOSTNAME, tkr_set->Settings.hostname);
// //     WSBufferAppend_P(response, HTTP_FORM_END);
// //   }

// //   if (WifiIsInManagerMode()) {
// //     WSContentSpaceButton(request, BUTTON_RESTART);
// // #ifndef FIRMWARE_MINIMAL
// //     WSContentSpaceButton(request, BUTTON_RESET_CONFIGURATION);
// // #endif  // FIRMWARE_MINIMAL
// //   } else {
// //     WSContentSpaceButton(request, BUTTON_CONFIGURATION);
// //   }
// //   WSContentStop(request);
// }

// void mWebServer::WifiSaveSettings(AsyncWebServerRequest *request)
// {
//   // char tmp[sizeof(tkr_set->Settings.sta_pwd[0])];  // Max length is currently 65

//   // WebGetArg(request,"h", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.hostname, (!strlen(tmp)) ? WIFI_HOSTNAME : tmp, sizeof(tkr_set->Settings.hostname));
//   // if (strstr(tkr_set->Settings.hostname, "%") != nullptr) {
//   //   strlcpy(tkr_set->Settings.hostname, WIFI_HOSTNAME, sizeof(tkr_set->Settings.hostname));
//   // }
//   // WebGetArg(request,"s1", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_ssid[0], (!strlen(tmp)) ? STA_SSID1 : tmp, sizeof(tkr_set->Settings.sta_ssid[0]));
//   // WebGetArg(request,"s2", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_ssid[1], (!strlen(tmp)) ? STA_SSID2 : tmp, sizeof(tkr_set->Settings.sta_ssid[1]));
//   // WebGetArg(request,"s3", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_ssid[2], (!strlen(tmp)) ? STA_SSID3 : tmp, sizeof(tkr_set->Settings.sta_ssid[2]));
//   // WebGetArg(request,"p1", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_pwd[0], (!strlen(tmp)) ? "" : (strlen(tmp) < 5) ? tkr_set->Settings.sta_pwd[0] : tmp, sizeof(tkr_set->Settings.sta_pwd[0]));
//   // WebGetArg(request,"p2", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_pwd[1], (!strlen(tmp)) ? "" : (strlen(tmp) < 5) ? tkr_set->Settings.sta_pwd[1] : tmp, sizeof(tkr_set->Settings.sta_pwd[1]));
//   // ALOG_INF(PSTR(D_LOG_WIFI D_HOSTNAME " %s, " D_SSID "1 %s, " D_SSID "2 %s"), tkr_set->Settings.hostname, tkr_set->Settings.sta_ssid[0], tkr_set->Settings.sta_ssid[1]);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleLoggingConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_LOGGING);

//   // if (request->hasParam("save")) {
//   //   LoggingSaveSettings(request);
//   //   HandleConfiguration(request);
//   //   return;
//   // }

//   // WSStartAppend_P(request, S_CONFIGURE_LOGGING);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_LOG1);
//   // char stemp1[32];
//   // char stemp2[32];
//   // uint8_t dlevel[3] = { LOG_LEVEL_INFO, LOG_LEVEL_INFO, LOG_LEVEL_NONE };
//   // for (uint8_t idx = 0; idx < 3; idx++) {
//   //   uint8_t llevel = (0==idx)?tkr_set->Settings.logging.serial_level:(1==idx)?tkr_set->Settings.logging.web_level:tkr_set->Settings.syslog_level;
//   //   WSBufferAppend_P(response, PSTR("<p><b>%s</b> (%s)<br/><select id='l%d' name='l%d'>"),
//   //     pCONT_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), idx, kLoggingOptions),
//   //     pCONT_sup->GetTextIndexed_P(stemp2, sizeof(stemp2), dlevel[idx], kLoggingLevels),
//   //     idx, idx);
//   //   for (uint8_t i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
//   //     WSBufferAppend_P(response, PSTR("<option%s value='%d'>%d %s</option>"),
//   //       (i == llevel) ? " selected" : "", i, i,
//   //       pCONT_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), i, kLoggingLevels));
//   //   }
//   //   WSBufferAppend_P(response, PSTR("</select></p>"));
//   // }
//   // WSBufferAppend_P(response, HTTP_FORM_LOG2, tkr_set->Settings.syslog_host, tkr_set->Settings.syslog_port, tkr_set->Settings.tele_period);
//   // WSBufferAppend_P(response, HTTP_FORM_END);
//   // WSContentSpaceButton(request, BUTTON_CONFIGURATION);
//   // WSContentStop(request);
// }

// void mWebServer::LoggingSaveSettings(AsyncWebServerRequest *request)
// {
//   // char tmp[sizeof(tkr_set->Settings.syslog_host)];  // Max length is currently 33

//   // WebGetArg(request,"l0", tmp, sizeof(tmp));
//   // tkr_set->Settings.logging.serial_level = (!strlen(tmp)) ? SERIAL_LOG_LEVEL : atoi(tmp);
//   // WebGetArg(request,"l1", tmp, sizeof(tmp));
//   // tkr_set->Settings.logging.web_level = (!strlen(tmp)) ? WEB_LOG_LEVEL : atoi(tmp);
//   // WebGetArg(request,"l2", tmp, sizeof(tmp));
//   // tkr_set->Settings.syslog_level = (!strlen(tmp)) ? SYS_LOG_LEVEL : atoi(tmp);
//   // tkr_set->syslog_level = tkr_set->Settings.syslog_level;
//   // tkr_set->syslog_timer = 0;
//   // WebGetArg(request,"lh", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.syslog_host, (!strlen(tmp)) ? SYS_LOG_HOST : tmp, sizeof(tkr_set->Settings.syslog_host));
//   // WebGetArg(request,"lp", tmp, sizeof(tmp));
//   // tkr_set->Settings.syslog_port = (!strlen(tmp)) ? SYS_LOG_PORT : atoi(tmp);
//   // WebGetArg(request,"lt", tmp, sizeof(tmp));
//   // tkr_set->Settings.tele_period = (!strlen(tmp)) ? TELE_PERIOD : atoi(tmp);
//   // if ((tkr_set->Settings.tele_period > 0) && (tkr_set->Settings.tele_period < 10)) {
//   //   tkr_set->Settings.tele_period = 10;   // Do not allow periods < 10 seconds
//   // }
//   // ALOG_INF(PSTR(D_LOG_LOG D_SERIALLOG " %d, " D_WEBLOG " %d, " D_SYSLOG " %d, " D_LOGHOST " %s, " D_LOGPORT " %d, " D_TELEPERIOD " %d"),
//   //   tkr_set->Settings.logging.serial_level, tkr_set->Settings.logging.web_level, tkr_set->Settings.syslog_level, tkr_set->Settings.syslog_host, tkr_set->Settings.syslog_port, tkr_set->Settings.tele_period);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleOtherConfiguration(AsyncWebServerRequest *request)
// {
// //   if (!HttpCheckPriviledgedAccess()) { return; }

// //   ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_OTHER);

// //   if (request->hasParam("save")) {
// //     OtherSaveSettings(request);
// //     WebRestart(request, 1);
// //     return;
// //   }

// //   WSStartAppend_P(request, S_CONFIGURE_OTHER);
// //   WSContentSendStyle(request);

// //   pCONT_sup->TemplateJson(); 
// //   char stemp[strlen(data_buffer.payload.ctr) +1];
// //   strlcpy(stemp, data_buffer.payload.ctr, sizeof(stemp));  // Get JSON template
// //   WSBufferAppend_P(response, HTTP_FORM_OTHER, stemp, (USER_MODULE == tkr_set->Settings.module) ? " checked disabled" : "", (tkr_set->Settings.flag_system.mqtt_enabled) ? " checked" : "");

// //   uint8_t maxfn = (tkr_set->devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!tkr_set->devices_present) ? 1 : tkr_set->devices_present;
// //   if (MODULE_SONOFF_IFAN02 == tkr_set->my_module_type) { maxfn = 1; }
// //   for (uint8_t i = 0; i < maxfn; i++) {
// //     snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i +1);
// //     WSBufferAppend_P(response, PSTR("<b>" D_FRIENDLY_NAME " %d</b> (" FRIENDLY_NAME "%s)<br/><input id='a%d' name='a%d' placeholder='" FRIENDLY_NAME "%s' value='%s'><p></p>"),
// //       i +1,
// //       (i) ? stemp : "",
// //       i, i,
// //       (i) ? stemp : "",
// //       tkr_set->Settings.system_name.friendly[i]);
// //   }

// // #ifdef USE_EMULATION
// //   WSBufferAppend_P(response, PSTR("<p></p><fieldset><legend><b>&nbsp;" D_EMULATION "&nbsp;</b></legend><p>"));  // Keep close to Friendlynames so do not use <br/>
// //   for (uint8_t i = 0; i < EMUL_MAX; i++) {
// //     WSBufferAppend_P(response, PSTR("<input id='r%d' name='b2' type='radio' value='%d'%s><b>%s</b> %s<br/>"),  // Different id only used for labels
// //       i, i,
// //       (i == tkr_set->Settings.flag_power.emulation) ? " checked" : "",
// //       pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), i, kEmulationOptions),
// //       (i == EMUL_NONE) ? "" : (i == EMUL_WEMO) ? D_SINGLE_DEVICE : D_MULTI_DEVICE);
// //   }
// //   WSBufferAppend_P(response, PSTR("</p></fieldset>"));
// // #endif  // USE_EMULATION

// //   WSBufferAppend_P(response, HTTP_FORM_END);
// //   WSContentSpaceButton(request, BUTTON_CONFIGURATION);
// //   WSContentStop(request);
// }

// void mWebServer::OtherSaveSettings(AsyncWebServerRequest *request)
// {
// //   char tmp[128];
// //   char webindex[5];
// //   char friendlyname[sizeof(tkr_set->Settings.system_name.friendly[0])];

// //   WebGetArg(request,"wp", tmp, sizeof(tmp));
// //   strlcpy(tkr_set->Settings.web_password, (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? tkr_set->Settings.web_password : tmp, sizeof(tkr_set->Settings.web_password));
// //   tkr_set->Settings.flag_system.mqtt_enabled = request->hasParam("b1");
// // #ifdef USE_EMULATION
// //   WebGetArg(request,"b2", tmp, sizeof(tmp));
// //   tkr_set->Settings.flag_power.emulation = (!strlen(tmp)) ? 0 : atoi(tmp);
// // #endif  // USE_EMULATION
// //   snprintf_P(tkr_set->log_data, sizeof(tkr_set->log_data), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_EMULATION " %d, " D_FRIENDLYNAME), pCONT_sup->GetStateText(tkr_set->Settings.flag_system.mqtt_enabled), tkr_set->Settings.flag_power.emulation);
// //   for (uint8_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
// //     snprintf_P(webindex, sizeof(webindex), PSTR("a%d"), i);
// //     WebGetArg(request, webindex, tmp, sizeof(tmp));
// //     snprintf_P(friendlyname, sizeof(friendlyname), PSTR(FRIENDLY_NAME"%d"), i +1);
// //     strlcpy(tkr_set->Settings.system_name.friendly[i], (!strlen(tmp)) ? (i) ? friendlyname : FRIENDLY_NAME : tmp, sizeof(tkr_set->Settings.system_name.friendly[i]));
// //     snprintf_P(tkr_set->log_data, sizeof(tkr_set->log_data), PSTR("%s%s %s"), tkr_set->log_data, (i) ? "," : "", tkr_set->Settings.system_name.friendly[i]);
// //   }
// //   //AddLogAddLog(LOG_LEVEL_INFO);
// //   WebGetArg(request,"t1", tmp, sizeof(tmp));
// //   if (strlen(tmp)) {  // {"NAME":"12345678901234","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":255,"BASE":255}
// //     char svalue[128];
// //     snprintf_P(svalue, sizeof(svalue), PSTR(D_TEMPLATE " %s"), tmp);
// //     ExecuteWebCommand(svalue, SRC_WEBGUI);

// //     if (request->hasParam("t2")) {
// //       snprintf_P(svalue, sizeof(svalue), PSTR(D_MODULE " 0"));
// //       ExecuteWebCommand(svalue, SRC_WEBGUI);
// //     }

// //   }
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleBackupConfiguration(AsyncWebServerRequest *request)
// {
// //   if (!HttpCheckPriviledgedAccess()) { return; }

// //   ALOG_DBG(PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

// //   if (!tkr_set->SettingsBufferAlloc()) { return; }

// //   // WiFiClient myClient = pWebServer->client();
// //   pWebServer->setContentLength(sizeof(tkr_set->Settings));

// //   char attachment[100];
// //   char friendlyname[sizeof(tkr_set->Settings.system_name.friendly)];
// //   snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), pCONT_sup->NoAlNumToUnderscore(friendlyname, tkr_set->Settings.system_name.friendly), tkr_set->my_version);
// //   //request->sendHeader(F("Content-Disposition"), attachment);// asyncedit

// //   WSSend(200, CT_STREAM, "");

// //   uint16_t cfg_crc = tkr_set->Settings.cfg_crc;
// //   tkr_set->Settings.cfg_crc = tkr_set->GetSettingsCrc();  // Calculate crc (again) as it might be wrong when savedata = 0 (#3918)

// //   memcpy(tkr_set->settings_buffer, &tkr_set->Settings, sizeof(tkr_set->Settings));
// //   if (config_xor_on_set) {
// //     for (uint16_t i = 2; i < sizeof(tkr_set->Settings); i++) {
// //       tkr_set->settings_buffer[i] ^= (config_xor_on_set +i);
// //     }
// //   }

// // #ifdef ARDUINO_ESP8266_RELEASE_2_3_0
// //   size_t written = myClient.write((const char*)tkr_set->settings_buffer, sizeof(tkr_set->Settings));
// //   if (written < sizeof(tkr_set->Settings)) {  // https://github.com/esp8266/Arduino/issues/3218
// //     myClient.write((const char*)tkr_set->settings_buffer +written, sizeof(tkr_set->Settings) -written);
// //   }
// // #else
// //   myClient.write((const char*)tkr_set->settings_buffer, sizeof(tkr_set->Settings));
// // #endif

// //   tkr_set->SettingsBufferFree();

// //   tkr_set->Settings.cfg_crc = cfg_crc;  // Restore crc in case savedata = 0 to make sure settings will be noted as changed
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleResetConfiguration(AsyncWebServerRequest *request)
// {
//   // //if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_RESET_CONFIGURATION);

//   // WSStartAppend_P(request, S_RESET_CONFIGURATION, !WifiIsInManagerMode());
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, PSTR("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>"));
//   // WSBufferAppend_P(response, HTTP_MSG_RSTRT);
//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);

//   // char command[CMDSZ];
//   // snprintf_P(command, sizeof(command), PSTR(D_RESET " 1"));
//   // ExecuteWebCommand(command, SRC_WEBGUI);
// }

// void mWebServer::HandleReset(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(S_LOG_HTTP, S_RESET_CONFIGURATION);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   pCONT_wif->EspRestart();

// }

// void mWebServer::HandleRestoreConfiguration(AsyncWebServerRequest *request)
// {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(S_LOG_HTTP, S_RESTORE_CONFIGURATION);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   // WSStartAppend_P(request, S_RESTORE_CONFIGURATION);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_RST);
//   // WSBufferAppend_P(response, HTTP_FORM_RST_UPG, D_RESTORE);
//   // WSContentSpaceButton(request, BUTTON_CONFIGURATION);
//   // WSContentStop(request);

//   // upload_error = 0;
//   // upload_file_type = UPL_SETTINGS;
// }

// /*-------------------------------------------------------------------------------------------*/

// // #endif  // Not FIRMWARE_MINIMAL


// void mWebServer::HandleConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURATION);

//   // WSStartAppend_P(request, S_CONFIGURATION);
//   // WSContentSendStyle(request);

//   // WSContentButton(request, BUTTON_MODULE);
//   // WSContentButton(request, BUTTON_WIFI);

//   // pCONT->Tasker_Interface(TASK_WEB_ADD_BUTTON);

//   // WSContentButton(request, BUTTON_LOGGING);
//   // WSContentButton(request, BUTTON_OTHER);
//   // WSContentButton(request, BUTTON_TEMPLATE);

//   // WSContentSpaceButton(request, BUTTON_RESET_CONFIGURATION);
//   // WSContentButton(request, BUTTON_BACKUP);
//   // WSContentButton(request, BUTTON_RESTORE);

//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleTemplateConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // if (pWebServer->hasParam("save")) {
//   //   TemplateSaveSettings();
//   //   WebRestart(1);
//   //   return;
//   // }

//   // char stemp[20];                                           // Template number and Sensor name

//   // if (pWebServer->hasParam("m")) {
//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   for (uint8_t i = 0; i < sizeof(ModuleList_IDs); i++) {  // "}2'%d'>%s (%d)}3" - "}2'0'>Sonoff Basic (1)}3"
//   //     uint8_t midx = pgm_read_byte(ModuleList_IDs + i);
//   //     WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, midx, pCONT_sup->AnyModuleName(midx).c_str(), midx +1);
//   //   }
//   //   WSContentEnd(request);
//   //   return;
//   // }

//   // WebGetArg(request,"t", stemp, sizeof(stemp));                     // 0 - 69 Template number
//   // if (strlen(stemp)) {
//   //   uint8_t module = atoi(stemp);
//   //   uint8_t module_save = tkr_set->Settings.module;
//   //   tkr_set->Settings.module = module;
//   //   myio cmodule;
//   //   pCONT_sup->TemplateGPIOs(&cmodule);
//   //   gpio_flag flag = pCONT_sup->ModuleFlag();
//   //   tkr_set->Settings.module = module_save;

//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   WSBufferAppend_P(response, PSTR("%s}1"), pCONT_sup->AnyModuleName(module).c_str());  // NAME: Generic
//   //   for (uint8_t i = 0; i < sizeof(kGpioNiceList); i++) {   // GPIO: }2'0'>None (0)}3}2'17'>Button1 (17)}3...

//   //     if (1 == i) {
//   //       WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, 255, D_SENSOR_USER, 255);  // }2'255'>User (255)}3
//   //     }

//   //     uint8_t midx = pgm_read_byte(kGpioNiceList + i);
//   //     WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, midx, pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), midx, kSensorNames), midx);
//   //   }

//   //   WSBufferAppend_P(response, PSTR("}1"));                                   // Field separator
//   //   for (uint8_t i = 0; i < sizeof(cmodule); i++) {         // 17,148,29,149,7,255,255,255,138,255,139,255,255
//   //     if ((i < 6) || ((i > 8) && (i != 11))) {              // Ignore flash pins GPIO06, 7, 8 and 11
//   //       WSBufferAppend_P(response, PSTR("%s%d"), (i>0)?",":"", cmodule.io[i]);
//   //     }
//   //   }
//   //   WSBufferAppend_P(response, PSTR("}1%d}1%d"), flag, tkr_set->Settings.user_template_base);  // FLAG: 1  BASE: 17
//   //   WSContentEnd(request);
//   //   return;
//   // }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_TEMPLATE);

//   // WSStartAppend_P(S_CONFIGURE_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_TEMPLATE);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_TABLE100);
//   // WSBufferAppend_P(response, PSTR("<tr><td><b>" D_TEMPLATE_NAME "</b></td><td style='width:200px'><input id='s1' name='s1' placeholder='" D_TEMPLATE_NAME "'></td></tr>"
//   //                      "<tr><td><b>" D_BASE_TYPE "</b></td><td><select id='g99' name='g99' onchange='st(this.value)'></select></td></tr>"
//   //                      "</table>"
//   //                      "<hr/>"));
//   // WSBufferAppend_P(response, HTTP_TABLE100);
//   // for (uint8_t i = 0; i < 17; i++) {
//   //   if ((i < 6) || ((i > 8) && (i != 11))) {                // Ignore flash pins GPIO06, 7, 8 and 11
//   //     WSBufferAppend_P(response, PSTR("<tr><td><b><font color='#%06x'>" D_GPIO "%d</font></b></td><td%s><select id='g%d' name='g%d'></select></td></tr>"),
//   //       ((9==i)||(10==i)) ? WebColor(tkr_set->COL_TEXT_WARNING) : WebColor(tkr_set->COL_TEXT), i, (0==i) ? " style='width:200px'" : "", i, i);
//   //   }
//   // }
//   // WSBufferAppend_P(response, PSTR("</table>"));
//   // WSBufferAppend_P(response, HTTP_FORM_TEMPLATE_FLAG);
//   // WSBufferAppend_P(response, HTTP_FORM_END);
//   // WSContentSpaceButton(BUTTON_CONFIGURATION);
//   // WSContentStop();
// }

// void mWebServer::TemplateSaveSettings(void)
// {
//   // char tmp[sizeof(tkr_set->Settings.user_template.name)];            // WebGetArg NAME and GPIO/BASE/FLAG byte value
//   // char webindex[5];                                         // WebGetArg name
//   // char svalue[128];                                         // Template command string

//   // WebGetArg(request,"s1", tmp, sizeof(tmp));                        // NAME
//   // snprintf_P(svalue, sizeof(svalue), PSTR(D_TEMPLATE " {\"" D_NAME "\":\"%s\",\"" D_GPIO "\":["), tmp);

//   // uint8_t j = 0;
//   // for (uint8_t i = 0; i < sizeof(tkr_set->Settings.user_template.hardware.gp); i++) {
//   //   if (6 == i) { j = 9; }
//   //   if (8 == i) { j = 12; }
//   //   snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), j);
//   //   WebGetArg(webindex, tmp, sizeof(tmp));                  // GPIO
//   //   uint8_t gpio = atoi(tmp);
//   //   snprintf_P(svalue, sizeof(svalue), PSTR("%s%s%d"), svalue, (i>0)?",":"", gpio);
//   //   j++;
//   // }

//   // uint8_t flag = 0;
//   // for (uint8_t i = 0; i < GPIO_FLAG_USED; i++) {
//   //   snprintf_P(webindex, sizeof(webindex), PSTR("c%d"), i);
//   //   uint8_t state = pWebServer->hasParam(webindex) << i;       // FLAG
//   //   flag += state;
//   // }
//   // WebGetArg(request,"g99", tmp, sizeof(tmp));                       // BASE
//   // uint8_t base = atoi(tmp) +1;

//   // snprintf_P(svalue, sizeof(svalue), PSTR("%s],\"" D_FLAG "\":%d,\"" D_BASE "\":%d}"), svalue, flag, base);
//   // ExecuteWebCommand(svalue, SRC_WEBGUI);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleModuleConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // if (pWebServer->hasParam("save")) {
//   //   ModuleSaveSettings();
//   //   //WebRestart(1);
//   //   return;
//   // }

//   // char stemp[20];  // Sensor name
//   // uint8_t midx;
//   // myio cmodule;
//   // pCONT_sup->TemplateGPIOs(&cmodule);

//   // if (pWebServer->hasParam("m")) {
//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   uint8_t vidx = 0;
//   //   for (uint8_t i = 0; i <= sizeof(ModuleList_IDs); i++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//   //     if (0 == i) {
//   //       midx = USER_MODULE;
//   //       vidx = 0;
//   //     } else {
//   //       midx = pgm_read_byte(ModuleList_IDs + (i-1)); // -1 to offset USER_MODULE
//   //       vidx = midx +1;
//   //     }
//   //     //ALOG_TST(PSTR(D_LOG_HTTP "hasParam(\"m\"),i=%d,midx=%d,vidx=%d"),i,midx,vidx);
//   //     WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, 
//   //       midx, 
//   //       pCONT_sup->AnyModuleName(midx).c_str(), 
//   //       vidx
//   //     );
//   //   }
//   //   WSContentEnd(request);

//   //   ALOG_TST(PSTR(D_LOG_HTTP "sizeof(ModuleList_IDs)=%d"),sizeof(ModuleList_IDs));
    
//   //   return;
//   // }

//   // if (pWebServer->hasParam("g")) {
//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   for (uint8_t j = 0; j < sizeof(kGpioNiceList); j++) {
//   //     midx = pgm_read_byte(kGpioNiceList + j);
//   //     if (!pCONT_sup->GetUsedInModule(midx, cmodule.io)) {
//   //       WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, 
//   //                       midx, 
//   //                       pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), midx, kSensorNames), 
//   //                       midx
//   //                     );

//   //     // ALOG_TST(PSTR(D_LOG_HTTP "GetTextIndexed_P=%s,midx=%d,j=%d"),
//   //     // pCONT_sup->GetTextIndexed_P(stemp, sizeof(stemp), midx, kSensorNames),midx,j);
        
//   //     }
//   //   }
//   //   WSContentEnd(request);
//   //   return;
//   // }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_MODULE);

//   // ALOG_TST(PSTR(D_LOG_HTTP "tkr_set->Settings.module=%d,sizeof(cmodule)=%d"),tkr_set->Settings.module,sizeof(cmodule));
  

//   // WSStartAppend_P(S_CONFIGURE_MODULE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE1, tkr_set->Settings.module);
//   // for (uint8_t i = 0; i < sizeof(cmodule); i++) {
//   //   if (pCONT_sup->ValidGPIO(i, cmodule.io[i])) {
//   //     WSBufferAppend_P(response, PSTR("sk(%d,%d);"), tkr_set->my_module.io[i], i);  // g0 - g16
//   //   }
//   // }
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE2);
//   // WSContentSendStyle(request);
  
//   // // ALOG_TST(PSTR(D_LOG_HTTP "AnyModuleName=%s,%d,%d"),pCONT_sup->AnyModuleName(MODULE).c_str(),MODULE,sizeof(cmodule));

//   // uint8_t show_pin_names = false;
//   // if((tkr_set->Settings.module==MODULE_WEMOS)||(tkr_set->Settings.module==MODULE_NODEMCU)){
//   //   show_pin_names = true;
//   // }

//   // WSBufferAppend_P(response, HTTP_FORM_MODULE, pCONT_sup->AnyModuleName(tkr_set->Settings.module).c_str());
//   // for (uint8_t i = 0; i < sizeof(cmodule); i++) {
//   //   if (pCONT_sup->ValidGPIO(i, cmodule.io[i])) {
//   //     snprintf_P(stemp, 3, PINS_WEMOS +i*2);
//   //     char sesp8285[40];
//   //     snprintf_P(sesp8285, sizeof(sesp8285), PSTR("<font color='#%06x'>ESP8285</font>"), WebColor(tkr_set->COL_TEXT_WARNING));
//   //     WSBufferAppend_P(response, PSTR("<tr><td style='width:190px'>%s <b>" D_GPIO "%d</b> %s</td><td style='width:176px'><select id='g%d' name='g%d'></select></td></tr>"),
//   //       show_pin_names?stemp:"", 
//   //       i,
//   //       (0==i)? D_SENSOR_BUTTON "1":
//   //       (1==i)? D_SERIAL_OUT :
//   //       (3==i)? D_SERIAL_IN :
//   //       ((9==i)||(10==i))? sesp8285 :
//   //       (12==i)? D_SENSOR_RELAY "1":
//   //       (13==i)? D_SENSOR_LED "1i":
//   //       (14==i)? D_SENSOR :"",
//   //       i,
//   //       i
//   //     );
//   //   }
//   // }
//   // WSBufferAppend_P(response, PSTR("</table>"));
//   // WSBufferAppend_P(response, HTTP_FORM_END);
//   // WSContentSpaceButton(BUTTON_CONFIGURATION);
//   // WSContentStop();
// }



// void mWebServer::ModuleSaveSettings(AsyncWebServerRequest *request)
// {
//   // char tmp[8];         // WebGetArg numbers only
//   // char webindex[5];    // WebGetArg name

//   // WebGetArg(request,"g99", tmp, sizeof(tmp));
//   // uint8_t new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
//   // tkr_set->Settings.last_module = tkr_set->Settings.module;
//   // tkr_set->Settings.module = new_module;
//   // pCONT_sup->SetModuleType();
//   // myio cmodule;
//   // pCONT_sup->TemplateGPIOs(&cmodule);
//   // String gpios = "";
//   // for (uint8_t i = 0; i < sizeof(cmodule); i++) {
//   //   if (tkr_set->Settings.last_module != new_module) {
//   //     tkr_set->Settings.module_pins.io[i] = GPIO_NONE;
//   //   } else {
//   //     if (pCONT_sup->ValidGPIO(i, cmodule.io[i])) {
//   //       snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), i);
//   //       WebGetArg(webindex, tmp, sizeof(tmp));
//   //       tkr_set->Settings.module_pins.io[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
//   //       gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(tkr_set->Settings.module_pins.io[i]);
//   //     }
//   //   }
//   // }
//   // ALOG_INF(PSTR(D_LOG_MODULE "%s " D_MODULE "%s"), pCONT_sup->ModuleName().c_str(), gpios.c_str());
// }


// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * System Settings
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */


// void mWebServer::HandleSystemSettings(AsyncWebServerRequest *request)
// {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//   if (request->hasParam("rst")) {
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_DEV_TEST,PSTR("hasParam rst"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//     WebRestart(request, 0);
//     return;
//   }
  
//   // ALOG_DBG(S_LOG_HTTP, S_SYSTEM_SETTINGS);

//   // D_DATA_BUFFER_CLEAR();
//   // char *buf = data_buffer.payload.ctr;
//   // char **buffer = &buf;
//   // buffer_writer_len = 0;

//   // ALOG_INF(PSTR(D_LOG_HTTP D_CONSOLE));

//   // WSStartAppend_P2(buffer, S_SYSTEM_SETTINGS);

//   // //? basic style
//   // WSBufferAppend_P2(buffer, "<style>");
//   // pCONT_web->WSBufferAppend_P2(buffer, PM_HTTP_HEAD_STYLE1_MINIMAL,
//   //   pCONT_web->WebColor(tkr_set->COL_FORM), 
//   //   pCONT_web->WebColor(tkr_set->COL_BACKGROUND),
//   //   pCONT_web->WebColor(tkr_set->COL_BUTTON),  
//   //   pCONT_web->WebColor(tkr_set->COL_BUTTON_TEXT)
//   // );
//   // WSBufferAppend_P2(buffer, "</style>");

//   // WSBufferAppend_P2(buffer, "</head>" "<body>");
//   // WSBufferAppend_P2(buffer, PSTR("<div class='tb_con'></div>"));
//   // WSBufferAppend_P2(buffer, HTTP_STYLE_TITLE_CONTAINER_HEAD, 
//   //   WebColor(tkr_set->COL_TEXT),
//   //   WebColor(tkr_set->COL_TEXT_MODULE_TITLE), 
//   //   //PROJECT_NAME_CTR, 
//   //   tkr_set->Settings.system_name.friendly
//   // );
//   //   // pCONT->Tasker_Interface(TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS);

//   //   WSButtonAppend2(buffer, BUTTON_CONFIGURATION);
//   //   WSButtonAppend2(buffer, BUTTON_INFORMATION);
//   //   WSButtonAppend2(buffer, BUTTON_FIRMWARE_UPGRADE);
//   //   WSButtonAppend2(buffer, BUTTON_RESTART);

//   // WSSpaceButtonAppend2(buffer, BUTTON_MAIN);
  
//   // // End of body
//   // WSBufferAppend_SystemVersionBar(buffer);

//   // WSBufferAppend_P2(buffer, PSTR("<link rel='stylesheet' href='" WEB_HANDLER_STYLE_WEB_BASIC_ROOT "'>"));

//   // WSBufferAppend_P2(buffer, "</body></html>");
  
//   // // #ifdef DEBUG_WEBSERVER_MEMORY
//   // //   FreeMem_Usage_Before(&freemem_usage_html_console);
//   // // #endif
//   // WebSend_Response(request,200,CONTENT_TYPE_TEXT_HTML_ID,data_buffer.payload.ctr);
//   // #ifdef DEBUG_WEBSERVER_MEMORY
//   //   FreeMem_Usage_After(&freemem_usage_html_console);
//   // #endif 

// }


// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Restart and Update
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */



// void mWebServer::WebRestart(AsyncWebServerRequest *request, uint8_t type)
// {
//   // type 0 = restart
//   // type 1 = restart after config change
//   // type 2 = restart after config change with possible ip address change too
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(S_LOG_HTTP, S_RESTART);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   // bool reset_only = (HTTP_MANAGER_RESET_ONLY == webserver_state);

//   // WSStartAppend_P(request, (type) ? S_SAVE_CONFIGURATION : S_RESTART, !reset_only);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_RELOAD);
//   // WSContentSendStyle(request);
//   // if (type) {
//   //   WSBufferAppend_P(response, PSTR("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br/>"));
//   //   if (2 == type) {
//   //     WSBufferAppend_P(response, PSTR("<br/>" D_TRYING_TO_CONNECT "<br/>"));
//   //   }
//   //   WSBufferAppend_P(response, PSTR("</div>"));
//   // }
//   // WSBufferAppend_P(response, HTTP_MSG_RSTRT);
//   // if (HTTP_MANAGER == webserver_state || reset_only) {
//   //   webserver_state = HTTP_ADMIN;
//   // } else {
//   //   WSContentSpaceButton(request, BUTTON_MAIN);
//   // }
//   // WSContentStop(request);

//   // ShowWebSource(SRC_WEBGUI);
//   tkr_set->restart_flag = 2;
// }


// void mWebServer::HandleWifiLogin(AsyncWebServerRequest *request)
// {
// //   WSStartAppend_P(request, S_CONFIGURE_WIFI, false);  // false means show page no matter if the client has or has not credentials
// //   WSContentSendStyle(request);
// //   WSBufferAppend_P(response, HTTP_FORM_LOGIN);

// //   if (HTTP_MANAGER_RESET_ONLY == webserver_state) {
// //     WSContentSpaceButton(request, BUTTON_RESTART);
// // #ifndef FIRMWARE_MINIMAL
// //     WSContentSpaceButton(request, BUTTON_RESET_CONFIGURATION);
// // #endif  // FIRMWARE_MINIMAL
// //   }

// //   WSContentStop(request);
// }


// void mWebServer::HandleUpgradeFirmware(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_FIRMWARE_UPGRADE);

//   // WSStartAppend_P(request, S_FIRMWARE_UPGRADE);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_UPG, tkr_set->Settings.ota_url);
//   // WSBufferAppend_P(response, HTTP_FORM_RST_UPG, D_UPGRADE);
//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);

//   // upload_error = 0;
//   // upload_file_type = UPL_TASMOTA;
// }

// void mWebServer::HandleUpgradeFirmwareStart(AsyncWebServerRequest *request)
// {
//   // //if (!HttpCheckPriviledgedAccess()) { return; }

//   // char command[sizeof(tkr_set->Settings.ota_url) + 10];  // OtaUrl

//   // ALOG_DBG(PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
//   // pCONT_wif->WifiConfigCounter();

//   // char otaurl[sizeof(tkr_set->Settings.ota_url)];
//   // WebGetArg(request,"o", otaurl, sizeof(otaurl));
//   // if (strlen(otaurl)) {
//   //   snprintf_P(command, sizeof(command), PSTR(D_OTAURL " %s"), otaurl);
//   //   ExecuteWebCommand(command, SRC_WEBGUI);
//   // }

//   // WSStartAppend_P(request, PM_INFORMATION);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_RELOAD_OTA);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
//   // WSBufferAppend_P(response, HTTP_MSG_RSTRT);
//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);

//   // snprintf_P(command, sizeof(command), PSTR(D_UPGRADE " 1"));
//   // ExecuteWebCommand(command, SRC_WEBGUI);
// }

// void mWebServer::HandleUploadDone(AsyncWebServerRequest *request)
// {
// //   if (!HttpCheckPriviledgedAccess()) { return; }

// //   ALOG_DBG(PSTR(D_LOG_HTTP D_UPLOAD_DONE));

// //   char error[100];

// //   pCONT_wif->WifiConfigCounter();
// //   tkr_set->restart_flag = 0;
// //   //MqttRetryCounter(0);

// //   WSStartAppend_P(request, PM_INFORMATION);
// //   if (!upload_error) {
// //     WSBufferAppend_P(response, HTTP_SCRIPT_RELOAD_OTA);  // Refesh main web ui after OTA upgrade
// //   }
// //   WSContentSendStyle(request);
// //   WSBufferAppend_P(response, PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
// //   if (upload_error) {
// // //    WSBufferAppend_P(response, PSTR(COLOR_TEXT_WARNING "'>" D_FAILED "</font></b><br/><br/>"));
// //     WSBufferAppend_P(response, PSTR("%06x'>" D_FAILED "</font></b><br/><br/>"), WebColor(tkr_set->COL_TEXT_WARNING));
// // #ifdef USE_RF_FLASH
// //     if (upload_error < 14) {
// // #else
// //     if (upload_error < 10) {
// // #endif
// //       pCONT_sup->GetTextIndexed_P(error, sizeof(error), upload_error -1, kUploadErrors);
// //     } else {
// //       snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), upload_error);
// //     }
// //     WSBufferAppend_P(response, error);
// //     ALOG_DBG(PSTR(D_UPLOAD ": %s"), error);
// //     tkr_set->stop_flash_rotate = tkr_set->Settings.flag_system.stop_flash_rotate;
// //   } else {
// //     WSBufferAppend_P(response, PSTR("%06x'>" D_SUCCESSFUL "</font></b><br/>"), WebColor(tkr_set->COL_TEXT_SUCCESS));
// //     WSBufferAppend_P(response, HTTP_MSG_RSTRT);
// //     ShowWebSource(SRC_WEBGUI);
// //     tkr_set->restart_flag = 2;  // Always restart to re-enable disabled features during update
// //   }
// //   //SettingsBufferFree();
// //   WSBufferAppend_P(response, PSTR("</div><br/>"));
// //   WSContentSpaceButton(request, BUTTON_MAIN);
// //   WSContentStop(request);
// }

// void mWebServer::HandleUploadLoop(AsyncWebServerRequest *request)
// {
  
// //   //AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HandleUploadLoop1"));
// //   // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
// //   bool _serialoutput = (LOG_LEVEL_DEBUG <= tkr_set->seriallog_level);

// //   // if (HTTP_USER == webserver_state) { 
// //   //   AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HTTP_USER == webserver_state"));
// //   //   return; 
// //   // }
// //   if (upload_error) {
// //     AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "upload_error"));
// //     #ifdef ESP8266
// //       if (UPL_TASMOTA == upload_file_type) { Update.end(); }
// //     #endif
// //     return;
// //   }

// //   //AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HandleUploadLoop2"));

// //   HTTPUpload& upload = pWebServer->upload();

// //   //AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "upload.status=%d"),upload.status);

// //   if (UPLOAD_FILE_START == upload.status) { 

// //     tkr_set->restart_flag = 60;
// //     if (0 == upload.filename.c_str()[0]) {
// //       upload_error = 1;  // No file selected
// //       return;
// //     }
// //     //SettingsSave(1);  // Free flash for upload
// //     AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
// //     if (UPL_SETTINGS == upload_file_type) {
// //       if (!tkr_set->SettingsBufferAlloc()) {
// //         upload_error = 2;  // Not enough space
        
// //   AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "Not enough space"));
// //         return;
// //       }
// //     } else {
// //      // MqttRetryCounter(60);
// // #ifdef USE_EMULATION
// //       //UdpDisconnect();
// // #endif  // USE_EMULATION

// //       //if (tkr_set->Settings.flag_system.mqtt_enabled) MqttDisconnect();
// //       uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      
// //   AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "maxSketchSpace=%d"),maxSketchSpace);

// //           #ifdef ESP8266
// //       if (!Update.begin(maxSketchSpace)) {         //start with max available size

// //        if (_serialoutput) Update.printError(Serial);
// //        if (Update.getError() == UPDATE_ERROR_BOOTSTRAP) {
// //          if (_serialoutput) Serial.println("Device still in UART update mode, perform powercycle");
// //        }

// //         upload_error = 2;  // Not enough space
// //         return;
// //       }
// //       #endif
// //     }
// //     upload_progress_dot_count = 0;
// //   } else if (!upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
// //     if (0 == upload.totalSize) {
// //       if (UPL_SETTINGS == upload_file_type) {
// //         config_block_count = 0;
// //       }
// //       else {
// // // #ifdef USE_RF_FLASH
// // //         if ((SONOFF_BRIDGE == tkr_set->my_module_type) && (upload.buf[0] == ':')) {  // Check if this is a RF bridge FW file
// // //           Update.end();              // End esp8266 update session
// // //           upload_file_type = UPL_EFM8BB1;
// // //
// // //           upload_error = SnfBrUpdateInit();
// // //           if (upload_error != 0) { return; }
// // //         } else
// // // #endif  // USE_RF_FLASH
// //         {
// //           if (upload.buf[0] != 0xE9) {
// //             upload_error = 3;  // Magic byte is not 0xE9
// //             return;
// //           }
// //           uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
// //           #ifdef ESP8266
// //           if(bin_flash_size > ESP.getFlashChipRealSize()) {
// //             upload_error = 4;  // Program flash size is larger than real flash size
// //             return;
// //           }
// //           #endif
// // //          upload.buf[2] = 3;  // Force DOUT - ESP8285
// //         }
// //       }
// //     }
// //     if (UPL_SETTINGS == upload_file_type) {
// //       if (!upload_error) {
// //         if (upload.currentSize > (sizeof(tkr_set->Settings) - (config_block_count * HTTP_UPLOAD_BUFLEN))) {
// //           upload_error = 9;  // File too large
// //           return;
// //         }
// //         memcpy(tkr_set->settings_buffer + (config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
// //         config_block_count++;
// //       }
// //     }
// // // #ifdef USE_RF_FLASH
// // //     else if (UPL_EFM8BB1 == upload_file_type) {
// // //       if (efm8bb1_update != nullptr) {    // We have carry over data since last write, i. e. a start but not an end
// // //         ssize_t result = rf_glue_remnant_with_new_data_and_write(efm8bb1_update, upload.buf, upload.currentSize);
// // //         free(efm8bb1_update);
// // //         efm8bb1_update = nullptr;
// // //         if (result != 0) {
// // //           upload_error = abs(result);  // 2 = Not enough space, 8 = File invalid
// // //           return;
// // //         }
// // //       }
// // //       ssize_t result = rf_search_and_write(upload.buf, upload.currentSize);
// // //       if (result < 0) {
// // //         upload_error = abs(result);
// // //         return;
// // //       } else if (result > 0) {
// // //         if ((size_t)result > upload.currentSize) {
// // //           // Offset is larger than the buffer supplied, this should not happen
// // //           upload_error = 9;  // File too large - Failed to decode RF firmware
// // //           return;
// // //         }
// // //         // A remnant has been detected, allocate data for it plus a null termination byte
// // //         size_t remnant_sz = upload.currentSize - result;
// // //         efm8bb1_update = (uint8_t *) malloc(remnant_sz + 1);
// // //         if (efm8bb1_update == nullptr) {
// // //           upload_error = 2;  // Not enough space - Unable to allocate memory to store new RF firmware
// // //           return;
// // //         }
// // //         memcpy(efm8bb1_update, upload.buf + result, remnant_sz);
// // //         // Add null termination at the end of of remnant buffer
// // //         efm8bb1_update[remnant_sz] = '\0';
// // //       }
// // //     }
// // // #endif  // USE_RF_FLASH
// //     else {  // firmware
    
// //           #ifdef ESP8266
// //       if (!upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
// //         upload_error = 5;  // Upload buffer miscompare
// //         return;
// //       }
// //       #endif
// //       if (_serialoutput) {
// //         Serial.printf(".");
// //         upload_progress_dot_count++;
// //         if (!(upload_progress_dot_count % 80)) { Serial.println(); }
// //       }
// //     }
// //   } else if(!upload_error && (UPLOAD_FILE_END == upload.status)) {
// //     if (_serialoutput && (upload_progress_dot_count % 80)) {
// //       Serial.println();
// //     }
// //     if (UPL_SETTINGS == upload_file_type) {
// //       if (config_xor_on_set) {
// //         for (uint16_t i = 2; i < sizeof(tkr_set->Settings); i++) {
// //           tkr_set->settings_buffer[i] ^= (config_xor_on_set +i);
// //         }
// //       }
// //       bool valid_settings = false;
// //       unsigned long buffer_version = tkr_set->settings_buffer[11] << 24 | tkr_set->settings_buffer[10] << 16 | tkr_set->settings_buffer[9] << 8 | tkr_set->settings_buffer[8];
// //       if (buffer_version > 0x06000000) {
// //         uint16_t buffer_size = tkr_set->settings_buffer[3] << 8 | tkr_set->settings_buffer[2];
// //         uint16_t buffer_crc = tkr_set->settings_buffer[15] << 8 | tkr_set->settings_buffer[14];
// //         uint16_t crc = 0;
// //         for (uint16_t i = 0; i < buffer_size; i++) {
// //           if ((i < 14) || (i > 15)) { crc += tkr_set->settings_buffer[i]*(i+1); }  // Skip crc
// //         }
// //         valid_settings = (buffer_crc == crc);
// //       } else {
// //         valid_settings = (tkr_set->settings_buffer[0] == CONFIG_FILE_SIGN);
// //       }
// //       if (valid_settings) {
// //         //SystemSettings_DefaultBody();
// //         memcpy((char*)&tkr_set->Settings +16, tkr_set->settings_buffer +16, sizeof(tkr_set->Settings) -16);
// //         tkr_set->Settings.version = buffer_version;  // Restore version and auto upgrade after restart
// //         //SettingsBufferFree();
// //       } else {
// //         upload_error = 8;  // File invalid
// //         return;
// //       }
// //     }
// // #ifdef USE_RF_FLASH
// //     else if (UPL_EFM8BB1 == upload_file_type) {
// //       // RF FW flash done
// //       upload_file_type = UPL_TASMOTA;
// //     }
// // #endif  // USE_RF_FLASH
// //     else {
// //           #ifdef ESP8266
// //       if (!Update.end(true)) { // true to set the size to the current progress
// //         if (_serialoutput) { Update.printError(Serial); }
// //         upload_error = 6;  // Upload failed. Enable logging 3
// //         return;
// //       }
// //       #endif
// //     }
// //     if (!upload_error) {
// //       ALOG_INF(PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
// //     }
// //   } else if (UPLOAD_FILE_ABORTED == upload.status) {
// //     tkr_set->restart_flag = 0;
// //     //MqttRetryCounter(0);
// //     upload_error = 7;  // Upload aborted
// //     #ifdef ESP8266
// //       if (UPL_TASMOTA == upload_file_type) { Update.end(); }
// //     #endif
// //   }
// //   delay(0);
// }



// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Handle Not Found
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// void mWebServer::HandleNotFound(AsyncWebServerRequest *request)
// {
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(PSTR(D_LOG_HTTP "Not found (%s)"), request->url().c_str());
//     #endif// ENABLE_LOG_LEVEL_INFO

//   // if (CaptivePortal(request)) { return; }  // If captive portal redirect instead of displaying the error page.

//   // AsyncResponseStream *response = request->beginResponseStream(404,"html/text");//, root.measureJson() + 1);
  
//   // // WSBufferAppend_P(response, PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"), 
//   // //   request->uri().c_str(), (request->method() == HTTP_GET) ? "GET" : "POST", request->args());

//   // for (uint8_t i = 0; i < request->args(); i++) {
//   //   WSBufferAppend_P(response, PSTR(" %s: %s\n"), request->argName(i).c_str(), request->arg(i).c_str());
//   // }
//   char message[45];
//     sprintf_P(message,PSTR("Missing"));// too low - %d needed (%d free)"),memory_needed,ESP.getFreeHeap());
//     request->send(404, CONTENT_TYPE_TEXT_HTML_ID, message);

//   // No subscriber handled the request, return a 404 with implicit "Connection: close"
//   // request->send(404);

// }





// /*****************************************************************************************************************************************************************************************************************
//  **************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ********HELPERS FUNCTION ONLY BELOW********************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************
//  *************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  *****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************/


// void mWebServer::WebAppend_Start_Head_P(const char* title)
// {
//   WebAppend_Start_Head_P(title, true);
// }
// void mWebServer::WebAppend_Start_Head_P(const char* title, bool auth)
// {
//   // if (auth && (tkr_set->Settings.web_password[0] != 0) && !pWebServer->authenticate(WEB_USERNAME, tkr_set->Settings.web_password)) {
//   //  return pWebServer->requestAuthentication();
//   // }

//   // if (title != nullptr) {
//   //   char ctitle[strlen_P(title) +1];
//   //   strcpy_P(ctitle, title);                       // Get title from flash to RAM
    
//   //   #ifdef ESP8266
//   //     BufferWriterI->Append_P(PM_HTTP_HEAD3, tkr_set->Settings.system_name.friendly, ctitle);
//   //   #else
//   //     BufferWriterI->Append_P(HTTP_HEAD2, tkr_set->Settings.system_name.friendly, ctitle);
//   //   #endif

//   // }
// }

// void mWebServer::WebAppend_Button_Spaced(uint8_t title_index)
// {
//   BufferWriterI->Append_P(PSTR("<div></div>"));            // 5px padding
//   WebAppend_Button(title_index);
// }
// void mWebServer::WebAppend_Button(uint8_t title_index)
// {
//   char action[20];
//   char title[32];
//   if (title_index <= BUTTON_RESET_CONFIGURATION) {
//     char confirm[64];
//     BufferWriterI->Append_P(PSTR("<p><form action='%s' method='get' onsubmit='return confirm(\"%s\");'><button name='%s' class='buttonh bred'>%s</button></form></p>"),
//       pCONT_sup->GetTextIndexed_P(action, sizeof(action), title_index, kButtonAction),
//       pCONT_sup->GetTextIndexed_P(confirm, sizeof(confirm), title_index, kButtonConfirm),
//       (!title_index) ? "rst" : "non",
//       pCONT_sup->GetTextIndexed_P(title, sizeof(title), title_index, kButtonTitle));
//   } else {
//     BufferWriterI->Append_P(PSTR("<p><form action='%s' method='get'><button class='buttonh'>%s</button></form></p>"),
//       pCONT_sup->GetTextIndexed_P(action, sizeof(action), title_index, kButtonAction),
//       pCONT_sup->GetTextIndexed_P(title, sizeof(title), title_index, kButtonTitle));
//   }
// }


// // MainButton? PageLoadButton?
// void mWebServer::WebAppend_Button_Spaced(const char* button_title_ctr, const char* action_ctr, const char* confirm_message_ctr, const char* confirm_action_ctr)
// {
//   BufferWriterI->Append_P(PSTR("<div></div>"));            // 5px padding
//   WebAppend_Button(button_title_ctr, action_ctr, confirm_message_ctr, confirm_action_ctr);
// }
// void mWebServer::WebAppend_Button(const char* button_title_ctr, const char* action_ctr, const char* confirm_message_ctr, const char* confirm_action_ctr)
// {
//   // long press button options?
//   // check for confirmation popout needed
//   if(confirm_message_ctr != nullptr){
//     char confirm[64];
//     BufferWriterI->Append_P(PSTR(
//       "<p>"
//         "<form action='%s' method='get' onsubmit='return confirm(\"%s\");'>"
//           "<button name='%s' class='buttonh bred'>%s</button>"
//         "</form>"
//       "</p>"),
//       action_ctr,
//       confirm_message_ctr,
//       confirm_action_ctr!=nullptr?confirm_action_ctr:"non",//(!title_index) ? "rst" : "non", //leave for now, but add passing button colour, else assume default
//       button_title_ctr
//     );
//   } else {
//     BufferWriterI->Append_P(PSTR("<p><form action='%s' method='get'><button class='buttonh'>%s</button></form></p>"),
//       action_ctr,
//       button_title_ctr
//     );
//   }
// }


// void mWebServer::WebAppend_Button2(const char* button_title_ctr, const char* action_ctr, const char* button_css_extra_style_ctr)
// {
//   // long press button options?
//   // check for confirmation popout needed
//   // if(confirm_message_ctr != nullptr){
//   //   char confirm[64];
//   //   BufferWriterI->Append_P(PSTR(
//   //     "<p>"
//   //       "<form action='%s' method='get' onsubmit='return confirm(\"%s\");'>"
//   //         "<button name='%s' class='buttonh bred'>%s</button>"
//   //       "</form>"
//   //     "</p>"),
//   //     action_ctr,
//   //     confirm_message_ctr,
//   //     confirm_action_ctr!=nullptr?confirm_action_ctr:"non",//(!title_index) ? "rst" : "non", //leave for now, but add passing button colour, else assume default
//   //     button_title_ctr
//   //   );
//   // } else {
//     BufferWriterI->Append_P(PSTR(
      
//       // "<p><form action='%s' method='get'><button>%s</button></form></p>"

//     "<button name='%s' type='submit' class='buttonh %s'>%s</button>"),

//       action_ctr,
//       button_css_extra_style_ctr,
//       button_title_ctr
//     );
  
// }




// /*****************************************************************************************************************************************************************************************************************
//  **************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ******** UNUSED FUNCTION BELOW ********************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************
//  *************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  *****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************/




// // void mWebServer::HandleInformation(AsyncWebServerRequest *request)
// // {
// //     if (!HttpCheckPriviledgedAccess()) { return; }

// //     ALOG_DBG(S_LOG_HTTP, PM_INFORMATION);

// //     BufferWriterI->Start();

// //     // Head
// //     WebAppend_Start_Head_P(PM_INFORMATION); //<head>
// //         WebAppend_Minimal_Style();
// //         BufferWriterI->Append_P("</head>");

// //         // Body
// //         BufferWriterI->Append_P("<body>");
// //             WebAppend_Title();

// //             // Add information table position
// //             BufferWriterI->Append_P(PSTR("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>"));
// //             BufferWriterI->Append_P(PSTR("<style>td{padding:0px 5px;}</style>"
// //                                         "<div class='info_table_draw'></div>"));
// //             BufferWriterI->Append_P(PSTR("</fieldset>"));

// //             WebAppend_Button_Spaced(BUTTON_MAIN);
        
// //             WebAppend_SystemVersionBar();

// //             BufferWriterI->Append_P(PSTR("</div>"));

// //             WebAppend_Script_P(PM_WEBURL_PAGE_INFO_LOAD_SCRIPT);    

// //         BufferWriterI->Append_P("</body>");
// //     BufferWriterI->Append_P("</html>");

// //     WebSend_Response(request,200,CONTENT_TYPE_TEXT_HTML_ID,data_buffer.payload.ctr);
// // }



// // void mWebServer::WebSend_Information_Fetcher_URLs(AsyncWebServerRequest *request){

// //   // // Serial.println(WEB_HANDLER_SCRIPT_INFROMATION_DATA_FETCHER_URLS_RATES_VAR); Serial.flush();      

// //   // D_DATA_BUFFER_CLEAR();
// //   // char *buf = data_buffer.payload.ctr;
// //   // char **iter = &buf;
// //   // buffer_writer_internal = iter;

// //   // AppendBuffer_PI2(PSTR("const dfurls=["));

// //   //     AppendBuffer_PI2(PSTR("\"%s\","),"/script/parse_jsondata.js");
// //   //     AppendBuffer_PI2(PSTR("\"%s\","),WEB_HANDLER_SCRIPT_WEB_PARSE_ADDSCRIPTSTYLE_FUNCTION);
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),"/style/web_notif_bar.css");
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),"/div/web_drawdiv_root.json");


      
// //   //     AppendBuffer_PI2(PSTR("\"%s\","),"/div/web_info_table.json");
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),WEB_HANDLER_SCRIPT_ROOT_MICHAEL);
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),"/runtime/data_urls.json");
    
// //   //   // pCONT->Tasker_Interface(TASK_WEB_APPEND_LOADTIME_ROOT_URLS);
// //   //   *buffer_writer_internal = (*buffer_writer_internal) - 1;// remove extra comma
// //   // AppendBuffer_PI2(PSTR("];var dfrates=["));



// //   //     AppendBuffer_PI2(PSTR("%d,"),-1);
// //   //     AppendBuffer_PI2(PSTR("%d,"),-50);
// //   //     // AppendBuffer_PI2(PSTR("%d,"),-1000);
// //   //     AppendBuffer_PI2(PSTR("%d,"),-100);
// //   //     // AppendBuffer_PI2(PSTR("%d,"),-1500);
// //   //     // AppendBuffer_PI2(PSTR("%d,"),-2500);
// //   //   // pCONT->Tasker_Interface(TASK_WEB_APPEND_LOADTIME_ROOT_RATES);
// //   //   *buffer_writer_internal = (*buffer_writer_internal) - 1;// remove extra comma
// //   // AppendBuffer_PI2(PSTR("];"));

// //   // #ifdef DEBUG_WEBSERVER_MEMORY
// //   //   FreeMem_Usage_Before(&freemem_usage_js_fetcher_urls);
// //   // #endif
// //   // WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);
// //   // #ifdef DEBUG_WEBSERVER_MEMORY
// //   //   FreeMem_Usage_After(&freemem_usage_js_fetcher_urls);
// //   // #endif 

// // }



// // // this table exists on the root page and is there to take over tasmota m=1 duty
// // // when drawn, each module is asked for its row that sends its title and unique number class name
// // // when asked to update, each module will append their data to a shared sender here.
// // void mWebServer::HandleInformation_TableDraw(AsyncWebServerRequest *request){
    
// //     return ;
// //   // if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
// //   // uint16_t freemem_start = ESP.getFreeHeap();  
  
// //   // D_DATA_BUFFER_CLEAR();
// //   // char *buf = data_buffer.payload.ctr;
// //   // char **buffer = &buf;

// //   // AppendBuffer_P2(buffer,PSTR("{"));
// //   //   buffer_writer_internal = buffer;
// //   //   // pCONT->Tasker_Interface(TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED);
// //   //   // WebAppend_Root_Draw_TopBar();
// //   //   WebAppend_Page_InformationTable();
// //   //   // extra "," is automatically appending for repeated cases across modules, and should be removed
// //   //   *buffer_writer_internal = (*buffer_writer_internal) - 1;// remove extra comma
// //   // AppendBuffer_P2(buffer,PSTR("}"));

// //   // #ifdef DEBUG_WEBSERVER_MEMORY
// //   //   FreeMem_Usage_Before(&freemem_usage_json_root_draw);
// //   // #endif
// //   // WebSend_Response(request,200,CONTENT_TYPE_TEXT_JAVASCRIPT_ID,data_buffer.payload.ctr);  
// //   // #ifdef DEBUG_WEBSERVER_MEMORY
// //   //   FreeMem_Usage_After(&freemem_usage_json_root_draw);
// //   // #endif 

// //   // buffer_writer_internal = nullptr; // Anytime I use it, clear to back to null when finished
// // }

// // void mWebServer::WebAppend_Page_InformationTable(){

// //   return ;

// //   // AppendBuffer_PI2("\"%s\":[{\"ihr\":\"","info_table_draw");
// //   // AppendBuffer_PI2("%s","{t}");

// //   // AppendBuffer_PI2("<tr><th>");

// //   // // move sections into progmem
    
// //   // AppendBuffer_PI2(PSTR(D_PROGRAM_VERSION "}2%s(%s)"), tkr_set->my_version, "my_image");//tkr_set->my_image
// //   // // #ifdef ESP8266
// //   // //   AppendBuffer_PI2(PSTR("}1" D_BUILD_DATE_AND_TIME "}2%s"), tkr_time->GetBuildDateAndTime(dd));
// //   // //   AppendBuffer_PI2(PSTR("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_ESP8266_RELEASE "/%s"), ESP.getSdkVersion());
// //   // // #endif
// //   // // AppendBuffer_PI2(PSTR("}1" D_UPTIME "}2%s"), tkr_time->getFormattedUptime());
// //   // // #ifdef ESP8266
// //   // //   AppendBuffer_PI2(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d at 0x%X"), tkr_set->Settings.save_flag, tkr_set->GetSettingsAddress());
// //   // // #endif
// //   // AppendBuffer_PI2(PSTR("}1" D_BOOT_COUNT "}2%d"), tkr_set->Settings.bootcount);
// //   // #ifdef ESP8266
// //   //   AppendBuffer_PI2(PSTR("}1" D_RESTART_REASON "}2%s"), pCONT_sup->GetResetReason().c_str());
// //   // #endif
// //   // // uint8_t maxfn = 1;//(tkr_set->devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : tkr_set->devices_present;
// //   // // //if (SONOFF_IFAN02 == tkr_set->my_module_type) { maxfn = 1; }
// //   // // for (uint8_t i = 0; i < maxfn; i++) {
// //   // //   AppendBuffer_PI2(PSTR("}1" D_FRIENDLY_NAME " %d}2%s"), i +1, tkr_set->Settings.system_name.friendly[i]);
// //   // // }
// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // // Show SSID direct for testing
// //   // AppendBuffer_PI2(PSTR("}1" "SSID (RSS)" "}2%s (%d dBm)"), WiFi.SSID().c_str(), WiFi.RSSI());
    
// //   // AppendBuffer_PI2(PSTR("}1" D_AP "%d " D_SSID " (" D_RSSI ")}2%s (%d%%)"), tkr_set->Settings.sta_active +1, tkr_set->Settings.sta_ssid[tkr_set->Settings.sta_active], pCONT_wif->WifiGetRssiAsQuality(WiFi.RSSI()));
// //   // AppendBuffer_PI2(PSTR("}1" D_HOSTNAME "}2%s%s"), tkr_set->my_hostname, (pCONT_wif->mdns_begun) ? ".local" : "");
// //   // if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
// //   //   AppendBuffer_PI2(PSTR("}1" D_IP_ADDRESS "}2%s"), WiFi.localIP().toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_GATEWAY "}2%s"), IPAddress(tkr_set->Settings.ip_address[1]).toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_SUBNET_MASK "}2%s"), IPAddress(tkr_set->Settings.ip_address[2]).toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_DNS_SERVER "}2%s"), IPAddress(tkr_set->Settings.ip_address[3]).toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.macAddress().c_str());
// //   // }
// //   // if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
// //   //   AppendBuffer_PI2(PSTR("}1" D_IP_ADDRESS "}2%s"), WiFi.softAPIP().toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_GATEWAY "}2%s"), WiFi.softAPIP().toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.softAPmacAddress().c_str());
// //   // }
// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // // if (tkr_set->Settings.flag_system.mqtt_enabled) {
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_HOST "}2%s"), tkr_set->Settings.mqtt_host);
// //   // //   AppendBuffer_PI2(PSTR("}1" MQTT_PORT "}2%d"), tkr_set->Settings.mqtt_port);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_USER "}2%s"), tkr_set->Settings.mqtt_user);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_CLIENT "}2%s"), tkr_set->mqtt_client);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_TOPIC "}2%s"), tkr_set->Settings.mqtt_topic);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_GROUP_TOPIC "}2%s"), tkr_set->Settings.mqtt_grptopic);
// //   // // } else {
// //   // //   //AppendBuffer_PI2(PSTR("}1" D_MQTT "}2%s"), D_DISABLED);
// //   // // }
// //   // // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line
// //   // AppendBuffer_PI2(PSTR("}1" "Module Config" "}2"));//,       ESP.getChipId());
// //   // // Class/Tasks info
// //   // // buffer_writer_internal = buffer;
// //   // // pCONT->Tasker_Interface(TASK_WEB_PAGEINFORMATION_SEND_MODULE);
// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // #ifdef ESP8266
// //   //   AppendBuffer_PI2(PSTR("}1" D_ESP_CHIP_ID "}2%d"),       ESP.getChipId());
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X"), ESP.getFlashChipId());
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), ESP.getFlashChipRealSize() / 1024);
// //   // #endif
// //   // #ifdef ESP32
// //   //   AppendBuffer_PI2(PSTR("}1" D_ESP_CHIP_ID "}2%d"),       1);
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X"), 2);
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), 3);
// //   // #endif
// //   // AppendBuffer_PI2(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%dkB"), ESP.getFlashChipSize() / 1024);
// //   // AppendBuffer_PI2(PSTR("}1" D_PROGRAM_SIZE "}2%dkB"),       ESP.getSketchSize() / 1024);
// //   // AppendBuffer_PI2(PSTR("}1" D_FREE_PROGRAM_SPACE "}2%dkB"), ESP.getFreeSketchSpace() / 1024);
// //   // AppendBuffer_PI2(PSTR("}1" D_FREE_MEMORY "}2%dkB"),        ESP.getFreeHeap() / 1024);
// //   // AppendBuffer_PI2(PSTR("</td></tr>"));

// //   // pCONT_web->AppendBuffer_PI2("%s","{t2}");
// //   // pCONT_web->AppendBuffer_PI2("\"}]");  
// //   // pCONT_web->AppendBuffer_PI2(PSTR(","));  

// // } //end function




//         //   "var x2=null,lt2='';"
//         //   "var sn2=0,id2=0;" //sn2 starts at top of page, web_log_index starts at 0
//         //   "function l(p){"
//         //     "var c,o='',t;"
//         //     "clearTimeout(lt2);"
//         //     "t=document.getElementById('t1');"
//         //     "if(p==1){"
//         //       "c=document.getElementById('c1');"
//         //       "o='&c1='+encodeURIComponent(c.value);"
//         //       "c.value='';"
//         //       "t.scrollTop=sn2;"
//         //     "}"
//         //     //scrolltop == 0 is top of textbox, larger number = bottom
//         //     "if(t.scrollTop>=sn2){" //if scrolled up at all, don't update
//         //       "if(x2!=null){ x2.abort(); }"
//         //       "x2=new XMLHttpRequest();"
//         //       "x2.onreadystatechange=function(){"
//         //         "if(x2.readyState==4&&x2.status==200){"
//         //           "var z,d;"
//         //           //[web_log_index][reset_web_log_flag][text]
//         //           "d=x2.responseText.split(/}1/);"
//         //           "id2=d.shift();"   //web_log_index //removes first to last element
//         //           "if(d.shift()==0){" //reset_web_log_flag == 0
//         //             "t.value='';" //clear value back to start
//         //           "}"
//         //           "z=d.shift();" //get the text
//         //           "if(z.length>0){" //if new text
//         //             "t.value+=z;"  //append text
//         //           "}"
//         //           "t.scrollTop=99999;" //force to the very bottom
//         //           "sn2=t.scrollTop;"   //get scroll of the bottom line now 
//         //         "}"
//         //       "};"
//         //       "x2.open('GET','" D_WEB_HANDLE_CONSOLE_PAGE "?c2='+id2+o,true);" //current weblog_index + any new commands
//         //       "x2.send();"
//         //     "}"
//         //   "lt2=setTimeout(l,200);"
//         //   "return false;"
//         // "}"
//         // "l(0);"
//         // "function jd(){"
//         //   "var t=0,i=document.querySelectorAll('input,button,textarea,select');"
//         //   "while(i.length>=t){"
//         //     "if(i[t]){"
//         //       "i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name'];"
//         //     "}"
//         //     "t++;"
//         //   "}"
//         // "}"
//         // "jd();"

// // bool mWebServer::HandleRootStatusRefresh(AsyncWebServerRequest *request)
// // {
// //   // if (!WebAuthenticate()) {
// //   //   request->requestAuthentication();
// //   //   return true;
// //   // }

// //   if (!request->hasParam("m")) {     // Status refresh requested
// //     // AddLog(LOG_LEVEL_DEV_TEST,PSTR("!request->hasParam(\"m\")"));
// //     return false; 
// //   }else{
// //     AddLog(LOG_LEVEL_DEV_TEST,PSTR("request->hasParam(\"m\")"));
// //     // continue through root function
// //   }

// //   request_web_command = request;

// //   char tmp[30];                       // WebGetArg numbers only
// //   char svalue[32];                   // Command and number parameter

// //   // WebGetArg(request, "o", tmp, sizeof(tmp));  // 1 - 16 Device number for button Toggle or Fanspeed
// //   // if (strlen(tmp)) {
// //   //   ShowWebSource(SRC_WEBGUI);
// //   //   uint8_t device = atoi(tmp);
// //   //   if (MODULE_SONOFF_IFAN02 == tkr_set->my_module_type) {
// //   //     if (device < 2) {
// //   //       //ExecuteCommandPower(1, POWER_TOGGLE, SRC_IGNORE);
// //   //     } else {
// //   //       snprintf_P(svalue, sizeof(svalue), PSTR(D_FANSPEED " %d"), device -2);
// //   //       //ExecuteCommand(svalue, SRC_WEBGUI);
// //   //     }
// //   //   } else {
// //   //     //ExecuteCommandPower(device, POWER_TOGGLE, SRC_IGNORE);
// //   //   }
// //   // }
// //   WebGetArg(request,"d", tmp, sizeof(tmp));  // 0 - 100 Dimmer value
// //   if (strlen(tmp)) {
// //     snprintf_P(svalue, sizeof(svalue), PSTR(D_DIMMER " %s"), tmp);
// //     ExecuteWebCommand(svalue, SRC_WEBGUI);
// //   }
// //   // WebGetArg(request,"t", tmp, sizeof(tmp));  // 153 - 500 Color temperature
// //   // if (strlen(tmp)) {
// //   //   // Convert ?command to longer (t -> CT)
// //   //   snprintf_P(svalue, sizeof(svalue), PSTR(D_COLORTEMPERATURE " %s"), tmp);
// //   //   ExecuteWebCommand(svalue, SRC_WEBGUI);
// //   // }
// //   // WebGetArg(request,"k", tmp, sizeof(tmp));  // 1 - 16 Pre defined RF keys
// //   // if (strlen(tmp)) {
// //   //   snprintf_P(svalue, sizeof(svalue), PSTR(D_RFKEY "%s"), tmp);
// //   //   ExecuteWebCommand(svalue, SRC_WEBGUI);
// //   // }

// //   pCONT->Tasker_Interface(TASK_WEB_COMMAND); //parse any webcommands


// //   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return true;} 

// //   JBI->Start();
// //     JBI->AppendBuffer(PSTR("t}")); //temp fix
// //     // all but phased out 
// //     // REMOVE html part
// //     // pCONT->Tasker_Interface(TASK_WEB_SHOW_PARAMETERS);
// //     JBI->AppendBuffer(PSTR("{t2")); //temp fix
// //   JBI->End();

// //   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// //   return true;

// // }


// // void mWebServer::WebAppend_SystemVersionBar(){  

// //   uint32_t text_colour = WebColor(255,255,255);
// //   if(tkr_set->runtime.firmware_version.fNewVersionAvailable){
// //     text_colour = WebColor(tkr_set->COL_TEXT_SUCCESS);
// //   }
// //   if(tkr_set->runtime.firmware_version.fCurrentVersionNotSupported){
// //     text_colour = WebColor(tkr_set->COL_TEXT_WARNING);
// //   }

// //   char message_version1[100];
// //   sprintf(message_version1,PSTR("%s %s%s"),
// //     PROJECT_NAME_CTR,
// //     tkr_set->runtime.firmware_version.current.name_ctr,
// //     tkr_set->runtime.firmware_version.fNewVersionAvailable ? " Update Available" : ""  
// //   );

// //   BufferWriterI->Append_P(PM_HTTP_END_WITH_UPDATE_NOEND,
// //     "https://github.com/sparkplug23/HomeAutomationControlSystem",  // URL to open when clicked
// //     text_colour,
// //     message_version1
// //   );

// // }//end function





// // void mWebServer::WebAppend_Minimal_Style(){

// //   BufferWriterI->Append_P("<style>");
// //   BufferWriterI->Append_P(PM_HTTP_HEAD_STYLE1_MINIMAL,
// //     pCONT_web->WebColor(tkr_set->COL_FORM), 
// //     pCONT_web->WebColor(tkr_set->COL_BACKGROUND),
// //     pCONT_web->WebColor(tkr_set->COL_BUTTON),  
// //     pCONT_web->WebColor(tkr_set->COL_BUTTON_TEXT)
// //   );
// //   BufferWriterI->Append_P("</style>");

// // }

// // void mWebServer::WebAppend_Title(){
// //   BufferWriterI->Append_P(HTTP_STYLE_TITLE_CONTAINER_HEAD, 
// //     WebColor(tkr_set->COL_TEXT),
// //     WebColor(tkr_set->COL_TEXT_MODULE_TITLE), 
// //     tkr_set->Settings.system_name.friendly
// //   );
// // }

// // void mWebServer::WebAppend_Script_P(PGM_P url){
// //     BufferWriterI->Append_P(PSTR("<script type='text/javascript' src='%s'></script>"), url);    
// // }


#endif //   #ifdef USE_MODULE_NETWORK_WEBSERVER21