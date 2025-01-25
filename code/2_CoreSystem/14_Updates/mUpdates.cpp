/**
 * @file mUpdates.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-01
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * DESCRIPTION: This should be the main entry point of any upgrades to the system, either with OTA, HTTP, or MQTT
 *              Upgrades such as OTA for nextion panels, will be handled inside that module
 * 
 */

#include "mUpdates.h"


#ifdef USE_MODULE_CORE_UPDATES


void mUpdates::init(void)
{
  
  // analogWriteFreq(25000);
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chai


}


void mUpdates::Pre_Init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

}


int8_t mUpdates::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  //if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:  
    
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

     case TASK_LOOP:
    //   UfsExecuteCommandFileLoop();
      break;
#ifdef USE_SDCARD
    case TASK_PRE_INIT:
      UfsCheckSDCardInit();
      break;
#endif // USE_SDCARD
    // case TASK_MQTT_INIT:
    //   if (!TasmotaGlobal.no_autoexec) {
    //     UfsExecuteCommandFile(TASM_FILE_AUTOEXEC);
    //   }
    //   break;
    // case TASK_COMMAND:
    //   result = DecodeCommand(kUFSCommands, kUFSCommand);
    //   break;
#ifdef USE_WEBSERVER
    case TASK_WEB_ADD_MANAGEMENT_BUTTON:
      if (ufs_type) {
        WSContentSend_PD(UFS_WEB_DIR, PSTR(D_MANAGE_FILE_SYSTEM));
      }
      break;
    case TASK_WEB_ADD_HANDLER:
//      Webserver->on(F("/ufsd"), UfsDirectory);
//      Webserver->on(F("/ufsu"), HTTP_GET, UfsDirectory);
//      Webserver->on(F("/ufsu"), HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      Webserver->on("/ufsd", UfsDirectory);
      Webserver->on("/ufsu", HTTP_GET, UfsDirectory);
      Webserver->on("/ufsu", HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      break;
#endif // USE_WEBSERVER
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER



  }
  
  

} // END Tasker


#ifdef USE_MODULE_NETWORK_WEBSERVER



void mUpdates::WebPage_Root_AddHandlers()
{
  
  
#ifndef WLED_DISABLE_OTA
  //init ota page
  tkr_web->server->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_GET \"/update\""));
    // if (otaLock) {
    //   serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_ota), 254);
    // } else
#ifdef USE_MODULE_LIGHTS_ANIMATOR
      tkr_anim->serveSettings(request); // checks for "upd" in URL and handles PIN
#endif // USE_MODULE_LIGHTS_ANIMATOR
  });

  tkr_web->server->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_POST \"/update\""));
    // if (!correctPIN) {
    //   serveSettings(request, true); // handle PIN page POST request
    //   return;
    // }
    if (Update.hasError() ) {
      tkr_web->serveMessage(request, 500, F("Update failed!"), F("Please check your file and retry!"), 254);
    } else {
      tkr_web->serveMessage(request, 200, F("Update successful!"), F("Rebooting..."), 131);
      // doReboot = true;
    }
  },[this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    // if (!this->correctPIN || this->otaLock) return;
    if(!index){
      DEBUG_PRINTLN(F("OTA Update Start"));
      // WLED::instance().disableWatchdog();
      // usermods.onUpdateBegin(true); // notify usermods that update is about to begin (some may require task de-init)
      // lastEditTime = millis(); // make sure PIN does not lock during update
      #ifdef ESP8266
      Update.runAsync(true);
      #endif
      Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000);
    }
    if(!Update.hasError()) Update.write(data, len);
    if(final){
      if(Update.end(true)){
        DEBUG_PRINTLN(F("Update Success"));
      } else {
        DEBUG_PRINTLN(F("Update Failed"));
        // usermods.onUpdateBegin(false); // notify usermods that update has failed (some may require task init)
        // WLED::instance().enableWatchdog();
      }
    }
  });
#else
  tkr_web->server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    serveMessage(request, 501, "Not implemented", F("OTA updating is disabled in this build."), 254);
  });
#endif



}









#endif //     #ifdef USE_MODULE_NETWORK_WEBSERVER


void mUpdates::parse_JSONCommand(JsonParserObject obj)
{


}


uint8_t mUpdates::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  //Serial.println("mUpdates::ConstructJSON_Settings");

  char buffer[30];
  
  JBI->Start();  

  // Got to ConstructJson_Scene out, or rename all the parameters as something else, or rgbcctactivepalette, or show them all? though that would need to run through, can only show
  // active_id, plus the values below
  // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  //   JBI->Add_P(PM_SCENE_NAME, GetSceneName(buffer, sizeof(buffer)));  
  //   #endif //  ENABLE_DEVFEATURE_PHASING_SCENE_OUT
  
    // JBI->Add_P(PM_HUE, rgbcct_controller.getHue360());
    // JBI->Add_P(PM_SAT, rgbcct_controller.getSat255());
    // JBI->Add_P(PM_BRIGHTNESS_RGB, rgbcct_controller.getBrightnessRGB());
    JBI->Add_P(PM_TIME, 1000);
    // JBI->Add_P(PM_TIME_MS, animation.time_ms);
  return JBI->End();

}

#ifdef USE_MODULE_NETWORK_MQTT

void mUpdates::MQTTHandler_Init(){

  struct handler<mUpdates>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mUpdates::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mUpdates::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mUpdates::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mUpdates::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}





#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CORE_UPDATES
