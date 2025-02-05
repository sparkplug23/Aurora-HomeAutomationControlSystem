#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
// Define the global instance
mAnimatorLight* tkr_extern_lAni = nullptr;
#endif

int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
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
  
  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  
  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:{
      EverySecond_AutoOff(); 

      updateInterfaces(CALL_MODE_WS_SEND); //tmp fix sending here


      #ifdef USE_DEVFEATURE_LIGHTS__CUSTOM_MAPPING_TABLE_SPLASH
      DEBUG_PRINT(F("Matrix ledmap:"));
      for (unsigned i=0; i<customMappingSize; i++) {
        if (!(i%mAnimatorLight::Segment::maxWidth)) DEBUG_PRINTLN();
        DEBUG_PRINTF("%4d,", customMappingTable[i]);
      }
      DEBUG_PRINTLN();
      #endif // USE_DEVFEATURE_LIGHTS__CUSTOM_MAPPING_TABLE_SPLASH


      #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING
      uint8_t log_level = tkr_time->UpTime() < 3600 ? LOG_LEVEL_DEV_TEST : LOG_LEVEL_DEBUG_MORE;
      AddLog(log_level, PSTR("getFPS %d"), getFps());
      AddLog(log_level, PSTR("starting %d%s"), lighting_time_critical_logging.time_unit_output_ms ? lighting_time_critical_logging.dynamic_buffer__starting_colour / 1000 : lighting_time_critical_logging.dynamic_buffer__starting_colour, lighting_time_critical_logging.time_unit_output_ms ? "ms" : "us");
      AddLog(log_level, PSTR("part1  ---------> %d%s"), lighting_time_critical_logging.time_unit_output_ms ? lighting_time_critical_logging.dynamic_buffer__starting_colour_part1 / 1000 : lighting_time_critical_logging.dynamic_buffer__starting_colour_part1, lighting_time_critical_logging.time_unit_output_ms ? "ms" : "us");
      AddLog(log_level, PSTR("part2  ---------> %d%s"), lighting_time_critical_logging.time_unit_output_ms ? lighting_time_critical_logging.dynamic_buffer__starting_colour_part2 / 1000 : lighting_time_critical_logging.dynamic_buffer__starting_colour_part2, lighting_time_critical_logging.time_unit_output_ms ? "ms" : "us");
      AddLog(log_level, PSTR("desired  %d%s"), lighting_time_critical_logging.time_unit_output_ms ? lighting_time_critical_logging.dynamic_buffer__desired_colour / 1000 : lighting_time_critical_logging.dynamic_buffer__desired_colour, lighting_time_critical_logging.time_unit_output_ms ? "ms" : "us");
      AddLog(log_level, PSTR("effect_call  %d%s"), lighting_time_critical_logging.time_unit_output_ms ? lighting_time_critical_logging.effect_call / 1000 : lighting_time_critical_logging.effect_call, lighting_time_critical_logging.time_unit_output_ms ? "ms" : "us");
      AddLog(log_level, PSTR("segment_effects  ---------> %d%s"), lighting_time_critical_logging.time_unit_output_ms ? lighting_time_critical_logging.segment_effects / 1000 : lighting_time_critical_logging.segment_effects, lighting_time_critical_logging.time_unit_output_ms ? "ms" : "us");
      ALOG_INF( PSTR(PM_COMMAND_SVALUE_NVALUE), PM_LOOPSSEC, pCONT_sup->activity.cycles_per_sec);    
      #endif // ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

    }break;
    case TASK_LOOP:     
      EveryLoop();  
    break;        
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    #endif
    /************
     * TRIGGERS SECTION * 
    *******************/
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
      KeyInput__ControlLights();
      #endif
    break;
    // #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
    // CommandSet_Physical_UserInput_Buttons();
    // #endif
    /************
     * MQTT SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      // pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif // USE_MODULE_NETWORK_MQTT
    /************
     * WIFI SECTION * 
    *******************/   
    case TASK_WIFI_CONNECTED:
      #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
      Test_Config();
      #endif
    break;
    /************
     * WEBUI SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER
  } // switch(command)


} // END FUNCTION


#ifdef ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
void mAnimatorLight::KeyInput__ControlLights()
{
  ALOG_INF(PSTR("KeyInput__ControlLights"));

  if(!keyinput_control.mode) return;

  switch(keyinput_control.mode)
  {
    case 1:


    break; 
  }



}
#endif // ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS


void mAnimatorLight::Save_Module()
{

  ALOG_INF(PSTR("mAnimatorLight::Save_Module()"));

  char buffer[100] = {0};

  /********************************************************************
   * Byte Data
   * ******************************************************************/


  /********************************************************************
   * Json Data
   * ******************************************************************/

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  JBI->Start();

  #ifdef ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES
    JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str() );
  #endif // ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  uint8_t bus_appended = 0;


  for(uint8_t seg_i = 0; seg_i< getSegmentsNum(); seg_i++)
  {
    JBI->Object_Start_F("Segment%d",seg_i);      

      JBI->Array_Start("PixelRange");
        JBI->Add(segments[seg_i].start);
        JBI->Add(segments[seg_i].stop);
      JBI->Array_End();

      JBI->Add("ColourPalette", GetPaletteNameByID(segments[seg_i].palette_id, buffer, sizeof(buffer)) );

      JBI->Object_Start_F("Effects");      
        JBI->Add("Function",  GetFlasherFunctionNamebyID(segments[seg_i].effect_id, buffer, sizeof(buffer), true));
        JBI->Add("Speed",     segments[seg_i].speed );
        JBI->Add("Intensity", segments[seg_i].intensity );
      JBI->Object_End();
      JBI->Object_Start_F("Transition");
        #ifndef ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
        JBI->Add("TimeMs",  segments[seg_i].time_ms );
        #endif // ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
        JBI->Add("RateMs",  segments[seg_i].cycle_time__rate_ms );
      JBI->Object_End();

      for(uint8_t seg_col = 0; seg_col < 5; seg_col++)
      {
        JBI->Array_Start_P(PSTR("SegColour%d"), seg_col);
          // for(uint8_t p=0;p<5;p++){ JBI->Add(segments[seg_i].segcol[seg_col][p]); }
        JBI->Array_End();
      }

    JBI->Object_End();
  }

  JBI->End();

  char filename_json[50];
  snprintf_P(filename_json, sizeof(filename_json), "/lgt_%S.json", GetModuleName());

  pCONT_mfile->JSONFile_Save(filename_json, JBI->GetBuffer(), JBI->GetBufferLength());

  JBI->ReleaseLock();


}

// #include "wled.h"

/*
 * WebSockets server for bidirectional communication
 */
#ifdef WLED_ENABLE_WEBSOCKETS2

uint16_t wsLiveClientId = 0;
unsigned long wsLastLiveTime = 0;
//uint8_t* wsFrameBuffer = nullptr;

#define WS_LIVE_INTERVAL 40

void sendDataWs(AsyncWebSocketClient * client = nullptr);

void sendDataWs(AsyncWebSocketClient * client)
{
  if (!tkr_web->ws->count())
  {
    ALOG_ERR(PSTR("No WS clients connected"));
    return;
  }

  if (!tkr_anim->requestJSONBufferLock(12)) {
    const char* error = PSTR("{\"error\":3}");
    if (client) {
      client->text(FPSTR(error)); // ERR_NOBUF
    } else {
      tkr_web->ws->textAll(FPSTR(error)); // ERR_NOBUF
    }
    return;
  }

  JsonObject state = tkr_anim->pDoc->createNestedObject("state");
  tkr_anim->serializeState(state);
  JsonObject info  = tkr_anim->pDoc->createNestedObject("info");
  tkr_anim->serializeInfo(info);

  size_t len = measureJson(*tkr_anim->pDoc);
  DEBUG_PRINTF_P(PSTR("JSON buffer size: %u for WS request (%u).\n"), tkr_anim->pDoc->memoryUsage(), len);

  // the following may no longer be necessary as heap management has been fixed by @willmmiles in AWS
  size_t heap1 = ESP.getFreeHeap();
  DEBUG_PRINTF_P(PSTR("heap %u\n"), ESP.getFreeHeap());
  #ifdef ESP8266
  if (len>heap1) {
    DEBUG_PRINTLN(F("Out of memory (WS)!"));
    return;
  }
  #endif
  AsyncWebSocketBuffer buffer(len);
  #ifdef ESP8266
  size_t heap2 = ESP.getFreeHeap();
  DEBUG_PRINTF_P(PSTR("heap %u\n"), ESP.getFreeHeap());
  #else
  size_t heap2 = 0; // ESP32 variants do not have the same issue and will work without checking heap allocation
  #endif
  if (!buffer || heap1-heap2<len) {
    tkr_anim->releaseJSONBufferLock();
    DEBUG_PRINTLN(F("WS buffer allocation failed."));
    tkr_web->ws->closeAll(1013); //code 1013 = temporary overload, try again later
    tkr_web->ws->cleanupClients(0); //disconnect all clients to release memory
    return; //out of memory
  }
  serializeJson(*tkr_anim->pDoc, (char *)buffer.data(), len);

  DEBUG_PRINT(F("Sending WS data "));
  if (client) {
    DEBUG_PRINTLN(F("to a single client."));
    client->text(std::move(buffer));
  } else {
    DEBUG_PRINTLN(F("to multiple clients."));
    tkr_web->ws->textAll(std::move(buffer));
  }

  tkr_anim->releaseJSONBufferLock();
}


void wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{

  uint8_t log = LOG_LEVEL_INFO;

  ALOG_INF(PSTR("wsEvent_________________________________________________________%d"),type);

  if(type == WS_EVT_CONNECT){
    //client connected
    DEBUG_PRINTLN(F("WS client connected."));
    sendDataWs(client);
  } else if(type == WS_EVT_DISCONNECT){
    //client disconnected
    if (client->id() == wsLiveClientId) wsLiveClientId = 0;
    DEBUG_PRINTLN(F("WS client disconnected."));
  } else if(type == WS_EVT_DATA){
    ALOG(log, PSTR("WS data received."));
    // data packet
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    if(info->final && info->index == 0 && info->len == len){
      // the whole message is in a single frame and we got all of its data (max. 1450 bytes)
      if(info->opcode == WS_TEXT)
      {
        if (len > 0 && len < 10 && data[0] == 'p') {
          // application layer ping/pong heartbeat.
          // client-side socket layer ping packets are unanswered (investigate)
          client->text(F("pong"));
          return;
        }

        bool verboseResponse = false;
        if (!tkr_anim->requestJSONBufferLock(11)) {
          client->text(F("{\"error\":3}")); // ERR_NOBUF
          return;
        }

        DeserializationError error = deserializeJson(*tkr_anim->pDoc, data, len);
        JsonObject root = tkr_anim->pDoc->as<JsonObject>();
        if (error || root.isNull()) {
          tkr_anim->releaseJSONBufferLock();
          ALOG(log, PSTR("{\"error\":2}")); // ERR_JSON
          return;
        }
        if (root["v"] && root.size() == 1) {
          //if the received value is just "{"v":true}", send only to this client
          verboseResponse = true;
        } else if (root.containsKey("lv")) {
          wsLiveClientId = root["lv"] ? client->id() : 0;
        } else {
          ALOG(log, PSTR("we are here"));
          verboseResponse = tkr_anim->deserializeState(root);
        }
        tkr_anim->releaseJSONBufferLock();
        ALOG(log, PSTR("we are here 2"));

        if (!tkr_anim->interfaceUpdateCallMode) { // individual client response only needed if no WS broadcast soon
          if (verboseResponse) {
            ALOG(log, PSTR("{\"success\":true} verboseResponse"));
            sendDataWs(client);
          } else {
            ALOG(log, PSTR("{\"success\":true}"));
            // we have to send something back otherwise WS connection closes
            client->text(F("{\"success\":true}"));
          }
          // force broadcast in 500ms after updating client
          //lastInterfaceUpdate = millis() - (INTERFACE_UPDATE_COOLDOWN -500); // ESP8266 does not like this
        }
      }
        ALOG(log, PSTR("we are here 3"));
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      //if(info->index == 0){
        //if (!wsFrameBuffer && len < 4096) wsFrameBuffer = new uint8_t[4096];
      //}

      //if (wsFrameBuffer && len < 4096 && info->index + info->)
      //{

      //}
        ALOG(log, PSTR("we are here 4"));

      if((info->index + len) == info->len){
        if(info->final){
          if(info->message_opcode == WS_TEXT) {
            client->text(F("{\"error\":9}")); // ERR_JSON we do not handle split packets right now
          }
        }
      }
      DEBUG_PRINTLN(F("WS multipart message."));
    }
  } else if(type == WS_EVT_ERROR){
    //error was received from the other end
    DEBUG_PRINTLN(F("WS error."));

  } else if(type == WS_EVT_PONG){
    //pong message was received (in response to a ping request maybe)
    DEBUG_PRINTLN(F("WS pong."));

  } else {
    DEBUG_PRINTLN(F("WS unknown event."));
  }
}

bool sendLiveLedsWs(uint32_t wsClient)
{
  AsyncWebSocketClient * wsc = tkr_web->ws->client(wsClient);
  if (!wsc || wsc->queueLength() > 0) return false; //only send if queue free

  size_t used = tkr_anim->getLengthTotal();
#ifdef ESP8266
  const size_t MAX_LIVE_LEDS_WS = 256U;
#else
  const size_t MAX_LIVE_LEDS_WS = 1024U;
#endif
  size_t n = ((used -1)/MAX_LIVE_LEDS_WS) +1; //only serve every n'th LED if count over MAX_LIVE_LEDS_WS
  size_t pos = 2;  // start of data
#ifndef WLED_DISABLE_2D
  if (tkr_anim->isMatrix) {
    // ignore anything behid matrix (i.e. extra strip)
    used = mAnimatorLight::Segment::maxWidth*mAnimatorLight::Segment::maxHeight; // always the size of matrix (more or less than strip.getLengthTotal())
    n = 1;
    if (used > MAX_LIVE_LEDS_WS) n = 2;
    if (used > MAX_LIVE_LEDS_WS*4) n = 4;
    pos = 4;
  }
#endif
  size_t bufSize = pos + (used/n)*3;

  AsyncWebSocketBuffer wsBuf(bufSize);
  if (!wsBuf) return false; //out of memory
  uint8_t* buffer = reinterpret_cast<uint8_t*>(wsBuf.data());
  if (!buffer) return false; //out of memory
  buffer[0] = 'L';
  buffer[1] = 1; //version

#ifndef WLED_DISABLE_2D
  if (tkr_anim->isMatrix) {
    buffer[1] = 2; //version
    buffer[2] = mAnimatorLight::Segment::maxWidth/n;
    buffer[3] = mAnimatorLight::Segment::maxHeight/n;
  }
#endif

  Serial.println("Sending live data to WS client");
  for (size_t i = 0; pos < bufSize -2; i += n)
  {
#ifndef WLED_DISABLE_2D
    if (tkr_anim->isMatrix && n>1 && (i/mAnimatorLight::Segment::maxWidth)%n) i += mAnimatorLight::Segment::maxWidth * (n-1);
#endif


    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
    RgbwwColor col = tkr_anim->getPixelColor(i);
    uint32_t c = RGBW32(col.R, col.G, col.B, col.WW);
    #else
    uint32_t c = tkr_anim->getPixelColor(i);
    #endif
    
    uint8_t r = R(c);
    uint8_t g = G(c);
    uint8_t b = B(c);
    uint8_t w = W(c);
    buffer[pos++] = r;//pCONT_iLight->_briRGB_Global ? qadd8(w, r) : 0; //R, add white channel to RGB channels as a simple RGBW -> RGB map
    buffer[pos++] = g;//pCONT_iLight->_briRGB_Global ? qadd8(w, g) : 0; //G
    buffer[pos++] = b;//pCONT_iLight->_briRGB_Global ? qadd8(w, b) : 0; //B
    Serial.println(r);
  }


  wsc->binary(std::move(wsBuf));
  return true;
}

void handleWs()
{
  if (millis() - wsLastLiveTime > WS_LIVE_INTERVAL)
  {
    #ifdef ESP8266
    tkr_web->ws->cleanupClients(3);
    #else
    tkr_web->ws->cleanupClients();
    #endif
    bool success = true;
    if (wsLiveClientId) success = sendLiveLedsWs(wsLiveClientId);
    wsLastLiveTime = millis();
    if (!success) wsLastLiveTime -= 20; //try again in 20ms if failed due to non-empty WS queue
  }
}

#else
void handleWs() {}
void sendDataWs(AsyncWebSocketClient * client) {}
#endif




void mAnimatorLight::EveryLoop()
{
    
    
  if (doInitBusses) 
  {
    doInitBusses = false;
    ALOG_INF(PSTR("Re-init busses"));
    
    bool aligned = checkSegmentAlignment(); //see if old segments match old bus(ses)
    pCONT_iLight->bus_manager->removeAll();

    uint32_t mem = 0;

    /*****************************************************************************
     * Detect type of NPB methods
    ******************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
      // determine if it is sensible to use parallel I2S outputs on ESP32 (i.e. more than 5 outputs = 1 I2S + 4 RMT)
      bool useParallel = false;
      #if defined(ARDUINO_ARCH_ESP32) && !defined(ARDUINO_ARCH_ESP32S2) && !defined(ARDUINO_ARCH_ESP32S3) && !defined(ARDUINO_ARCH_ESP32C3)
      unsigned digitalCount = 0;
      unsigned maxLedsOnBus = 0;
      unsigned maxChannels = 0;
      for (unsigned i = 0; i < WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES; i++) {
        if (pCONT_iLight->busConfigs[i] == nullptr) break;
        if (!Bus::isDigital(pCONT_iLight->busConfigs[i]->type)) continue;
        if (!Bus::is2Pin(pCONT_iLight->busConfigs[i]->type)) {
          digitalCount++;
          unsigned channels = Bus::getNumberOfChannels(pCONT_iLight->busConfigs[i]->type);
          if (pCONT_iLight->busConfigs[i]->count > maxLedsOnBus) maxLedsOnBus = pCONT_iLight->busConfigs[i]->count;
          if (channels > maxChannels) maxChannels  = channels;
        }
      }
      DEBUG_PRINTF_P(PSTR("Maximum LEDs on a bus: %u\n\rDigital buses: %u\n\r"), maxLedsOnBus, digitalCount);
      /**
       * Assign to use parallel only when pixels per bus are low, and channels are more than 2
       * Will use combined I2Sx2 + RMTx8 when pixels per bus are more than 300
       * 
       */
      if (maxLedsOnBus <= 300 && digitalCount > 2) 
      {  // I will want >2, as I0 and I1 are for 2 pins only, then immediately switch to parallel
        DEBUG_PRINTF_P(PSTR("Switching to parallel I2S\n\r"));
        useParallel = true;
        pCONT_iLight->bus_manager->useParallelOutput(true);
        pCONT_iLight->bus_manager->setRequiredChannels(digitalCount);
        mem = BusManager::memUsage(maxChannels, maxLedsOnBus, 8); // use alternate memory calculation (hse to be used *after* useParallelOutput())
      }else
      if (maxLedsOnBus > 300 && digitalCount > 2) 
      {
        ALOG_ERR(PSTR("Parallel is required to avoid RMT, but per bus count exceeded. Using anyway for now (%d,%d)"), maxLedsOnBus, digitalCount);
        pCONT_iLight->bus_manager->setRequiredChannels(digitalCount);
        pCONT_iLight->bus_manager->useParallelOutput(true);
      }
      else{
        ALOG_INF(PSTR("Parallel is not required for %d channels"), digitalCount);
        pCONT_iLight->bus_manager->setRequiredChannels(digitalCount);
        pCONT_iLight->bus_manager->useParallelOutput(false);
      }
      #endif
    #endif // ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

    DEBUG_DELAY(1000);

  
    /*****************************************************************************
     * Create NPB methods
    ******************************************************************************/
    for (uint8_t i = 0; i < WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES; i++) 
    {
      if (pCONT_iLight->busConfigs[i] == nullptr) break;
      // mem += BusManager::memUsage(*pCONT_iLight->busConfigs[i]);

      #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
      if (useParallel && i < 16) {
        // if for some unexplained reason the above pre-calculation was wrong, update
        unsigned memT = BusManager::memUsage(*pCONT_iLight->busConfigs[i]); // includes x8 memory allocation for parallel I2S
        if (memT > mem) mem = memT; // if we have unequal LED count use the largest
      } 
      else
      #endif // ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
      {
        mem += BusManager::memUsage(*pCONT_iLight->busConfigs[i]); // includes global buffer
      }

      if (mem <= MAX_LED_MEMORY) 
      {        
        pCONT_iLight->bus_manager->add(*pCONT_iLight->busConfigs[i]);        
      }
      else
      {        
        ALOG_ERR(PSTR("MEMORY ISSUE"));        
      }
      delete pCONT_iLight->busConfigs[i]; 
      pCONT_iLight->busConfigs[i] = nullptr;
    }
    
    DEBUG_DELAY(1000);

    finalizeInit(); // also loads default ledmap if present

    uint8_t bri = 128; // temporary brightness value
    
    BusManager::setBrightness(bri); // fix re-initialised bus' brightness #4005
    
    if (aligned) makeAutoSegments();
    else fixInvalidSegments();

    /**
     * @brief Added by me, during 1D strips (no matrix) te customTable will be loaded from another file
     * 
     */
    ALOG_HGL(PSTR("Loading 1D LED map."));
    bool flag_deserializeMap = deserializeMap(0);
    ALOG_HGL(PSTR("Loading 1D LED map. %s"), flag_deserializeMap ? "YES" : "NO");
    
    BusManager::setBrightness(bri); // fix re-initialised bus' brightness

    doSerializeConfig = true;
    // serializeConfig(); // in WLED This saved everything to json memory
  }

  #ifdef ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP
  // loadLedmap = 0;
  isMatrix = true;
  
    // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    // if (i1 < mAnimatorLight::Segment::maxWidth) seg.start = i1;
    // seg.stop = i2 > mAnimatorLight::Segment::maxWidth ? mAnimatorLight::Segment::maxWidth : i2;
    // if (startY < mAnimatorLight::Segment::maxHeight) seg.startY = startY;

    #ifdef ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP
    SEGMENT.startY = ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y;
    SEGMENT.stopY = ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y;
    SEGMENT.stop = ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP;
    #else
    SEGMENT.startY = 0;
    SEGMENT.stopY = 16;
    SEGMENT.stop = 16;
    #endif


    // ALOG_INF(PSTR("setSegment(%d, %d, %d, %d, %d, %d, %d, %d)"),n,i1,i2,grouping,spacing,offset,startY,stopY);
    // delay(1000);
    // #endif



  // 2D Matrix Settings
  // JsonObject matrix = hw_led[F("matrix")];
  // if (!matrix.isNull()) {
  //   tkr_anim->isMatrix = true;
    
    panels = 1;
    panel.clear();
    // JsonArray panels = matrix[F("panels")];
    uint8_t s = 0;
    // if (!panels.isNull()) {
      panel.reserve(max(1U,min((size_t)panels,(size_t)WLED_MAX_PANELS)));  // pre-allocate memory for panels
      // for (JsonObject pnl : panels) {

      //   	var px = parseInt(Sf[`P${p}X`].value); //first led x
			// var py = parseInt(Sf[`P${p}Y`].value); //first led y
			// var pw = parseInt(Sf[`P${p}W`].value); //width
			// var ph = parseInt(Sf[`P${p}H`].value); //height

			// var pb = Sf[`P${p}B`].value == "1"; //bottom
			// var pr = Sf[`P${p}R`].value == "1"; //right
			// var pv = Sf[`P${p}V`].value == "1"; //vertical
			// var ps = Sf[`P${p}S`].checked; //serpentine

			// var topLeftX = px*ppL + space; //left margin
			// var topLeftY = py*ppL + space; //top margin

        /**
         * @brief These are temporary define configs until the proper json commands are inserted for setting up matrix panels
         * 
         */
        #if defined(ENABLE_DEVFEATURE_LIGHT__MATRIX_32by8_PANEL)


        Panel p;
        p.bottomStart = 0; //, pnl["b"]);
        p.rightStart = 0;//,  pnl["r"]);
        p.vertical = 1;//,    pnl["v"]);` // works for 16x16
        p.serpentine = true; //,  pnl["s"]);
        p.xOffset = 0;//,     pnl["x"]);
        p.yOffset = 0;//,     pnl["y"]);
        p.height = 8;//,      pnl["h"]);
        p.width = 32;//,       pnl["w"]);
        panel.push_back(p);


        #elif defined(ENABLE_DEVFEATURE_LIGHT__MATRIX_SEGMENT_TESTER)


        Panel p;
        p.bottomStart = 0; //, pnl["b"]);
        p.rightStart = 0;//,  pnl["r"]);
        p.vertical = 1;//,    pnl["v"]);` // works for 16x16
        p.serpentine = true; //,  pnl["s"]);
        p.xOffset = 0;//,     pnl["x"]);
        p.yOffset = 0;//,     pnl["y"]);
        p.height = 8;//,      pnl["h"]);
        p.width = 32;//,       pnl["w"]);
        panel.push_back(p);


        #elif defined(ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL)


        Panel p;
        p.bottomStart = 0; //, pnl["b"]);
        p.rightStart = 0;//,  pnl["r"]);
        p.vertical = 1;//,    pnl["v"]);` // works for 16x16
        p.serpentine = true; //,  pnl["s"]);
        p.xOffset = 0;//,     pnl["x"]);
        p.yOffset = 0;//,     pnl["y"]);
        p.height = 8;//,      pnl["h"]);
        p.width = 32;//,       pnl["w"]);
        panel.push_back(p);



        #elif defined(ENABLE_DEVFEATURE_LIGHT__MATRIX_COLORADO_MATRIX_TREE)


        Panel p;
        p.bottomStart = 0; //, pnl["b"]);
        p.rightStart = 0;//,  pnl["r"]);
        p.vertical = 1;//,    pnl["v"]);` // works for 16x16
        p.serpentine = true; //,  pnl["s"]);
        p.xOffset = 0;//,     pnl["x"]);
        p.yOffset = 0;//,     pnl["y"]);
        p.height = 8;//,      pnl["h"]);
        p.width = 32;//,       pnl["w"]);
        panel.push_back(p);



        #else
        // default when not overriding

        Panel p;
        p.bottomStart = 0; //, pnl["b"]);
        p.rightStart = 0;//,  pnl["r"]);
        #ifdef ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL
        p.vertical = ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL;//,    pnl["v"]);`
        #else
        p.vertical = 1;//,    pnl["v"]);` // works for 16x16
        #endif
        p.serpentine = true; //,  pnl["s"]);
        p.xOffset = 0;//,     pnl["x"]);
        p.yOffset = 0;//,     pnl["y"]);
        p.height = SEGMENT.stopY;//,      pnl["h"]);
        p.width = SEGMENT.stop;//,       pnl["w"]);
        panel.push_back(p);



        #endif



      //   if (++s >= WLED_MAX_PANELS || s >= tkr_anim->panels) break; // max panels reached
      // }
    // } else {
    //   // fallback
    //   WS2812FX::Panel p;
    //   tkr_anim->panels = 1;
    //   p.height = p.width = 8;
    //   p.xOffset = p.yOffset = 0;
    //   p.options = 0;
    //   tkr_anim->panel.push_back(p);
    // }
    // cannot call tkr_anim->setUpMatrix() here due to already locked JSON buffer
  // }

  // THIS HAS BEEN MOVED IN 2025 INTO THE READ MAP FILE, AND THE SECTION BELOW SHOULD MOVE TOO
  if (loadLedmap >= 0) {
    ALOG_HGL(PSTR("Loading LED map."));
    bool flag_deserializeMap = deserializeMap(loadLedmap);
    if (!flag_deserializeMap && isMatrix && loadLedmap == 0)
    {
      ALOG_INF(PSTR("Matrix setup"));
      setUpMatrix();
    }
    loadLedmap = -1;
  }
  yield();
  #endif // ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP
  
  #ifdef WLED_ENABLE_WEBSOCKETS2
  handleWs();
  #endif
  
  // if (doSerializeConfig) serializeConfig();

  // Tmp fix to set brightness
  // pCONT_iLight->bus_manager->setBrightness( pCONT_iLight->getBriRGB_Global() ); // fix re-initialised bus' brightness

  if (doReboot && !doInitBusses) // if busses have to be inited & saved, wait until next iteration
    reset();
    
  // This should be removed, as realtime mode will cause this to switch anyway
  /**
   * @brief If RealTime modes first
   * - when realtime is used, effects are not to keep performance up and since the realtime aspect (network) needs priority, it is not going to be
   *   sustainable to also have internal animations running 
   * 
   * realtimeModes will always be loaded into the first segment, with other segments paused when realtimeMode is active
   */
  switch(realtimeMode)
  {
    case ANIMATION_MODE__EFFECTS:{ // Effects created on device, local control

      #ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
      if (apActive) dnsServer.processNextRequest();
      #endif 

      #ifdef ENABLE_FEATURE_LIGHTS__DEMO_MODE
      SubTask_Demo();
      #endif

      #ifdef ENABLE_FEATURE_LIGHTING__EFFECTS
      DEBUG_LIGHTING__START_TIME_RECORDING(1)
      SubTask_Effects();
      DEBUG_LIGHTING__SAVE_TIME_RECORDING(1, lighting_time_critical_logging.segment_effects); 
      #endif  

      DEBUG_LINE_HERE

      #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
      SubTask_Playlist();
      #endif

      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
      SubTask_Presets();
      #endif

    }break;
    #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
    case ANIMATION_MODE__REALTIME_MQTT_SETPIXEL:
      SubTask_RealTime_SetPixel();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_REALTIME_UDP
    case ANIMATION_MODE__REALTIME_UDP:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_HYPERION
    case ANIMATION_MODE__REALTIME_HYPERION:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_E131
    case ANIMATION_MODE__REALTIME_E131:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_ADALIGHT
    case ANIMATION_MODE__REALTIME_ADALIGHT:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_ARTNET
    case ANIMATION_MODE__REALTIME_ARTNET:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_TPM2NET
    case ANIMATION_MODE__REALTIME_TPM2NET:
      SubTask_Effects();
    break;
    #endif
    #ifdef ENABLE_ANIMATION_MODE__REALTIME_DDP
    case ANIMATION_MODE__REALTIME_DDP:
      SubTask_Effects();
    break;
    #endif
  } // END switch

  yield();

}


void mAnimatorLight::BootMessage()
{
  #ifndef ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // ALOG_INF(PSTR("BOOT: " "LightCount=%d"), settings.light_size_count);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS
  #endif // ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER

}


void mAnimatorLight::Pre_Init(void)
{

  #ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
  // On preinit, make sure to init the local class pointer to the global instance
  tkr_extern_lAni = this;
  #endif

}




void mAnimatorLight::Init(void)
{ 
  
  #ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI

  #ifdef USE_DEBUGFEATURE_DEVICE_CLONE_TESTBED
  #ifdef ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
  snprintf(serverDescription, sizeof(serverDescription), "PulSar %s \"%s\" [%s]", tkr_set->Settings.system_name.friendly, DEVICENAME_DESCRIPTION_CTR, tkr_time->GetBuildDateAndTime().c_str() );
  #else
  snprintf(serverDescription, sizeof(serverDescription), tkr_set->Settings.system_name.friendly);
  #endif
  #else
  #ifdef ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
  #ifdef DEVICENAME_DESCRIPTION_CTR
  snprintf(serverDescription, sizeof(serverDescription), "PulSar \"%s\" [%s] %s", tkr_set->Settings.system_name.friendly, tkr_time->GetBuildDateAndTime().c_str() , tkr_set->runtime.firmware_version.current.name_ctr);
  #else
  snprintf(serverDescription, sizeof(serverDescription), "PulSar %s [%s]", tkr_set->Settings.system_name.friendly, tkr_time->GetBuildDateAndTime().c_str());
  #endif
  #else
  snprintf(serverDescription, sizeof(serverDescription), tkr_set->Settings.system_name.friendly);
  #endif
  #endif

  sprintf(ntpServerName, NTP_SERVER1);  
  sprintf(apPass, CLIENT_SSID);
  sprintf(otaPass, "PulSar");
  sprintf(clientSSID, CLIENT_SSID);
  sprintf(clientPass, CLIENT_PASS);
  #endif // ENABLE_WEBSERVER_LIGHTING_WEBUI


  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  initPresetsFile();
  #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS

  Reset_CustomPalette_NamesDefault();  

  #ifdef WLED_ENABLE_WEBSOCKETS2
  tkr_web->ws->onEvent(wsEvent);
  #endif
  
  #ifdef ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP
  loadLedmap = 0; // To enable it to load once
  #endif // ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP

  paletteFade=0;
  paletteBlend = 0;
  milliampsPerLed = 5;
  cctBlending = 0;
  ablMilliampsMax = 0;
  currentMilliamps = 0;
  effect_start_time = millis();
  timebase = 0;
  isMatrix = false;
  _virtualSegmentLength = 0;
  _length = DEFAULT_LED_COUNT;
  _brightness = DEFAULT_BRIGHTNESS;
  _targetFps = WLED_FPS;
  _frametime = FRAMETIME;
  _cumulativeFps = 2;
  _isServicing = false;
  _isOffRefreshRequired = false;
  _hasWhiteChannel = false;
  _force_update = false;
  effects.count = getEffectsAmount();
  _callback = nullptr;
  customMappingTable = nullptr;
  customMappingSize = 0;
  _lastShow = 0;
  segment_current_index = 0;
  _mainSegment = 0;

  effects.function.reserve(effects.count);     // allocate memory to prevent initial fragmentation (does not increase size())
  effects.config.reserve(effects.count); // allocate memory to prevent initial fragmentation (does not increase size())
    
  LoadEffects();

  Init_Segments();

  module_state.mode = ModuleStatus::Running;

} // END "Init"


#ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MODES 
void mAnimatorLight::SubTask_RealTime_SetPixel()
{
  // Do nothing
}
#endif


void mAnimatorLight::Reset_CustomPalette_NamesDefault()
{
  
  char buffer[30];
  
  for (int ii=0;ii< mPaletteI->user_defined_palette_count;ii++){ 
    sprintf(buffer, D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR, ii+1);
    DLI->AddDeviceName(buffer, GetModuleUniqueID(), ii);
  }

}


void mAnimatorLight::EverySecond_AutoOff()
{

  for (segment &seg : segments) 
  {
  
    if(seg.auto_timeoff.UpdateTick())
    {
      ALOG_INF( PSTR(D_LOG_GARAGE D_COMMAND_NVALUE_K("Running Value")), seg.auto_timeoff.Value());
    }

    if(seg.auto_timeoff.IsLastTick())
    {
      ALOG_INF(PSTR("Segment Turn OFF"));
      // Set intensity to make all LEDs refresh
      seg.intensity = 255;
      seg.single_animation_override.time_ms = 1000; // slow and smooth turn off

      ALOG_INF(PSTR("Setting override for off %d"), seg.single_animation_override.time_ms);

      tkr_anim->force_update();

      seg.setBrightnessRGB(0);
      seg.setBrightnessCCT(0);    
    }

  }

} // END EverySecond_AutoOff




#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
mAnimatorLight& mAnimatorLight::setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE)
{
  this->anim_progress_mqtt_function_callback = anim_progress_mqtt_function_callback;
  return *this;
}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


/**
 * @brief Loads a palette into RAM for the segment, handling multiple palette types.
 *
 * This function loads the specified palette ID into RAM for active use within a segment. Depending on the type of the palette,
 * the function determines how to process and store the palette data. Supported palette types include:
 *
 * - **Static CRGBPalette16 Palettes**: Predefined palettes like Rainbow, Party, and Heat, which are directly assigned from
 *   flash (PROGMEM) to RAM.
 * - **Gradient Palettes**: Palettes containing gradient information that are processed dynamically from flash into RAM.
 * - **Static Custom Palettes**: Custom-defined palettes stored in flash, loaded directly into the segment's palette container.
 * - **Dynamic Palettes**: Palettes that are computed at runtime, such as solar azimuth palettes or dynamically generated color gradients.
 * - **Segment-Specific Palettes**: Palettes generated from segment-defined colors, including random colors or combinations of segment colors.
 *
 * The function also supports:
 * - Ensuring palette loading is thread-safe using an asynchronous lock.
 * - Using CRGBPalette16 for efficient rendering of effects that require fast access.
 * - Conversion from encoded gradient palettes into CRGBPalette16 to preserve gradient fidelity.
 * - Generation of random palettes based on parameters like intensity or hue ranges.
 *
 * @param palette_id The ID of the palette to load.
 * @param _palette_container The palette container to store the loaded data. If null, the segment's default container is used.
 *
 * @note This function assumes the palette data will be used exclusively from RAM after loading.
 *       For palette types with dynamic runtime behavior, updates are handled internally.
 *
 * @warning Ensure that palettes requiring gradient fidelity are loaded correctly, especially when converting from 
 *          gradient palettes to CRGBPalette16.
 *
 * @details 
 * - CRGBPalette16 palettes are directly assigned from predefined PROGMEM pointers.
 * - Gradient palettes are dynamically parsed into RAM, with gradient indices encoded for efficient lookups.
 * - Randomized palettes are generated based on the effect's intensity or timing requirements.
 *
 * @remarks The function releases the asynchronous lock upon completion and ensures that loaded palettes are 
 *          consistent and ready for use.
 */
void IRAM_ATTR mAnimatorLight::Segment::LoadPalette(uint8_t palette_id, mPaletteLoaded* _palette_container)
{

  #ifdef ENABLE_DEVFEATURE_LIGHTING__LOAD_PALETTE_ASYNC_LOCK
  while(LoadPalette_AsyncLock){ delay(1); }
  LoadPalette_AsyncLock = true; // take control of the lock
  #endif

  uint8_t segment_index = 0;

  DEBUG_PRINT_F("Palette ID: %d", palette_id);


  if (_palette_container == nullptr) 
  {
    if (palette_container == nullptr) 
    {
      ALOG_ERR(PSTR("No palette container passed and no default container set"));
      return;
    }
    _palette_container = palette_container;
  }


  palette_container->loaded_palette_id = palette_id;

  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){   

    DEBUG_PRINT_F("Palette ID: %d", palette_id);

    /******************************************************
     * PALETTELIST_STATIC_CRGBPALETTE16__IDS
     * No gradient information in palette bytes, CRGB16 will scale equally
     ******************************************************/
    switch (palette_id)
    {
      default:
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID:        _palette_container->CRGB16Palette16_Palette.data = RainbowColors_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID:          _palette_container->CRGB16Palette16_Palette.data = PartyColors_p;   break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID:         _palette_container->CRGB16Palette16_Palette.data = CloudColors_p;   break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID:          _palette_container->CRGB16Palette16_Palette.data = LavaColors_p;    break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID:          _palette_container->CRGB16Palette16_Palette.data = OceanColors_p;   break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID:         _palette_container->CRGB16Palette16_Palette.data = ForestColors_p;  break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID: _palette_container->CRGB16Palette16_Palette.data = RainbowStripeColors_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__HEAT_COLOUR__ID:           _palette_container->CRGB16Palette16_Palette.data = HeatColors_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_PARULA__ID:      _palette_container->CRGB16Palette16_Palette.data = Matlab_Purula_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_TURBO__ID:       _palette_container->CRGB16Palette16_Palette.data = Matlab_Turbo_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_HOT__ID:         _palette_container->CRGB16Palette16_Palette.data = Matlab_Hot_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_COOL__ID:        _palette_container->CRGB16Palette16_Palette.data = Matlab_Cool_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_SPRING__ID:      _palette_container->CRGB16Palette16_Palette.data = Matlab_Spring_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_AUTUMN__ID:      _palette_container->CRGB16Palette16_Palette.data = Matlab_Autumn_p; break;
      case mPalette::PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_JET__ID:         _palette_container->CRGB16Palette16_Palette.data = Matlab_Jet_p; break;
    }

    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    for(uint8_t i=0;i<16;i++){
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(map(i, 0,15, 0, 255));
    }

  }else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){   
  // DEBUG_LINE_HERE

    uint16_t gradient_id = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    byte tcp[72]; //support gradient palettes with up to 18 entries
    memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[gradient_id])), 72);

    /**
     * @brief Loading uses the CRGBPalette to get the colours
     **/
    _palette_container->CRGB16Palette16_Palette.data.loadDynamicGradientPalette(tcp);

    /**
     * @brief To get the gradients data exactly, manually parse them 
     * 
     */    
    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
    TRGBGradientPaletteEntryUnion u;
    // Count entries
    uint16_t count = 0;
    do {
        u = *(ent + count);
        count++;
    } while ( u.index != 255);
    u = *ent;
    int indexstart = 0;
    while( indexstart < 255) {
      indexstart = u.index;
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(u.index);
      ent++;
      u = *ent;
    }

    // ALOG_INF(PSTR("palette_container%d (seg%d) %d %d %d"), gradient_id, seg_i, _palette_container->CRGB16Palette16_Palette.data[0].r, _palette_container->CRGB16Palette16_Palette.data[0].g, _palette_container->CRGB16Palette16_Palette.data[0].b );
   
  }else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))     
  ){      
  // DEBUG_LINE_HERE
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    // ALOG_HGL(PSTR("ERROR HERE palette_id_adj %d"),palette_id_adj); 

    mPalette::PALETTE_DATA *ptr = &mPaletteI->static_palettes[palette_id_adj];
    _palette_container->pData = ptr->data;
    

    #ifdef ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS
// Gradient palettes are loaded into CRGB16Palettes in such a way
    // that, if possible, every color represented in the gradient palette
    // is also represented in the CRGBPalette16.
    // For example, consider a gradient palette that is all black except
    // for a single, one-element-wide (1/256th!) spike of red in the middle:
    //     0,   0,0,0
    //   124,   0,0,0
    //   125, 255,0,0  // one 1/256th-palette-wide red stripe
    //   126,   0,0,0
    //   255,   0,0,0
    // A naive conversion of this 256-element palette to a 16-element palette
    // might accidentally completely eliminate the red spike, rendering the
    // palette completely black.
    // However, the conversions provided here would attempt to include a
    // the red stripe in the output, more-or-less as faithfully as possible.
    // So in this case, the resulting CRGBPalette16 palette would have a red
    // stripe in the middle which was 1/16th of a palette wide -- the
    // narrowest possible in a CRGBPalette16.
    // This means that the relative width of stripes in a CRGBPalette16
    // will be, by definition, different from the widths in the gradient
    // palette.  This code attempts to preserve "all the colors", rather than
    // the exact stripe widths at the expense of dropping some colors.


    // Lets to show its possible with a default RGBP

    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    for(uint8_t i=0;i<16;i++){
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(map(i, 0,15, 0, 255));
    }

    _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                      CRGB(255,0,0),
                      CRGB(0,255,0),
                      CRGB(0,0,255),
                      CRGB(255,0,255)
      );

      // loadDynamicGradientPalette should enable with one (of 255) width to define edges of my palettes, and hence give them as non gradients when not a gradient, or as gradient when they are.



    #endif // ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS

  }else
  // Only some dynamic needs loading
  // The colours should have been placed into the dynamic_palette memory, then here it gets copied/loaded again
  if(
    ((palette_id >= mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC__LENGTH__ID))     
  ){      
  // DEBUG_LINE_HERE
    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;
    
    #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
    ALOG_HGL(PSTR("LOADING PALETTELIST_DYNAMIC palette_id_adj %d %d %d"),palette_id_adj, palette_id, mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID); 
    #endif
    
    mPalette::PALETTE_DATA *ptr = &mPaletteI->dynamic_palettes[palette_id_adj];
    _palette_container->pData = ptr->data;
    
  }else
  if(
    ((palette_id >= mPalette::PALETTELIST_DYNAMIC__LENGTH__ID)  && (palette_id < mPaletteI->GetPaletteListLength())) // Custom palettes
  ){
  // DEBUG_LINE_HERE
    // Preloading is not needed, already in ram
  
    
  }else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
  ){  
  // DEBUG_LINE_HERE
    // Preloading is not needed, already in ram
  }
  
  
  else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)
  ){  
  // DEBUG_LINE_HERE

    // ALOG_ERR(PSTR("Dynamic Palette  NEEDS MOVES WITH OTHER DYANMIC %d"), palette_id);
    
    _palette_container->CRGB16Palette16_Palette.encoded_index.clear();
    for(uint8_t i=0;i<16;i++){
      _palette_container->CRGB16Palette16_Palette.encoded_index.push_back(map(i, 0,15, 0, 255));
    }

    switch(palette_id)
    {
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID:
      { //primary + secondary
        CRGB prim = segcol[0].getU32();
        CRGB sec  = segcol[1].getU32();
        _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,prim,sec,sec); 
      }
      break;
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_THREE_123__ID:
      { //primary + secondary + tertiary
        CRGB prim = segcol[0].getU32();
        CRGB sec  = segcol[1].getU32();
        CRGB ter  = segcol[2].getU32();
        _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,sec,ter); 
      }
      break;    
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FOUR_1234__ID:
      { //primary + secondary + tertiary
        CRGB prim = segcol[0].getU32();
        CRGB sec  = segcol[1].getU32();
        CRGB ter  = segcol[2].getU32();
        CRGB four = segcol[3].getU32();
        _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,sec,ter,four); 
      }
      break;    
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FIVE_12345__ID:
      { //primary + secondary + tertiary
        CRGB prim = segcol[0].getU32();
        CRGB sec  = segcol[1].getU32();
        CRGB ter  = segcol[2].getU32();
        CRGB four = segcol[3].getU32();
        CRGB five = segcol[4].getU32();
        _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,prim,prim, sec,sec,sec, ter,ter,ter, four,four,four, five,five,five,five); 
      }
      break;    
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_REPEATED_ACTIVE__ID:
      { //primary + secondary + tertiary
        CRGB prim = segcol[0].getU32();
        CRGB sec  = segcol[1].getU32();
        CRGB ter  = segcol[2].getU32();
        CRGB four  = segcol[3].getU32();
        CRGB five  = segcol[4].getU32();
        _palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(prim,sec,ter,four,five, prim,sec,ter,four,five, prim,sec,ter,four,five, five); 
      }
      break;    
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID:
      {        
        /*
        intensity ranges from 0 to 255.
        The term (255 - intensity) ranges from 255 (when intensity = 0) to 0 (when intensity = 255).
        The multiplication ((255 - intensity) * 100) ranges from 255 * 100 = 25500 ms to 0 * 100 = 0 ms.
        Adding 1000 gives the final range for new_colour_rate_ms:
            Minimum: 1000 + 0 = 1000 ms (1 second).
            Maximum: 1000 + 25500 = 26500 ms (26.5 seconds).
        */
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-intensity))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - aux3 > new_colour_rate_ms)        
        {
          palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), 255, random8(128, 255)),
                          CHSV(random8(), 255, random8(128, 255)),
                          CHSV(random8(), 192, random8(128, 255)),
                          CHSV(random8(), 255, random8(128, 255))
          );
          ALOG_INF(PSTR("new_colour_rate_ms=%d"), new_colour_rate_ms);
          // ALOG_INF(PSTR("new_colour_rate_ms=%d - %d > %d"), millis() , aux3 , new_colour_rate_ms);
          aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_02_RANDOM_HUE_80TO100_SATURATIONS__ID: // Random Hue, Slight Random Saturation (80 to 100%) ie 200/255 is 80%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-intensity))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - aux3 > new_colour_rate_ms)        
        {
          palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(  // currentPalette needs moved into the segment? not palette, since each segment needs its own. 
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255),
                          CHSV(random8(), random8(204, 255), 255)
          );                          
          aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_03_RANDOM_HUE_60TO100_SATURATIONS__ID: // S60-S100%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-intensity))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - aux3 > new_colour_rate_ms)        
        {
          palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255),
                          CHSV(random8(), random8(153, 255), 255)
          );                          
          aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_04_RANDOM_HUE_60TO85_SATURATIONS__ID: // S60-S85%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-intensity))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - aux3 > new_colour_rate_ms)        
        {
          palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255),
                          CHSV(random8(), random8(153, 217), 255)
          );                          
          aux3 = millis();
        }
      }
      break;
      case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID: // S0-S100%
      {        
        uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-intensity))*100);
        // ALOG_INF(PSTR("new_colour_rate_ms=%d"),new_colour_rate_ms);
        if (millis() - aux3 > new_colour_rate_ms)        
        {
          palette_container->CRGB16Palette16_Palette.data = CRGBPalette16(
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255),
                          CHSV(random8(), random8(0, 255), 255)
                          );
                          
          aux3 = millis();
        }
      }
      break;    




    }

    
  }

  // DEBUG_LINE_HERE

  #ifdef ENABLE_DEVFEATURE_LIGHTING__LOAD_PALETTE_ASYNC_LOCK
  LoadPalette_AsyncLock = false; // release lock
  #endif
  

}


/**
 * @brief Gets palette names directly or from palette class when needed
 * 
 * @param palette_id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
const char* mAnimatorLight::GetPaletteNameByID(uint16_t palette_id, char* buffer, uint8_t buflen)
{

  // ALOG_INF(PSTR("GetPaletteNameByID %d"), palette_id);
  // Serial.flush();

    
  /**
   * @brief All static (progmem) palette names are stored in palette class
   * 
   * PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   */
  if(
    ((palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED))
  ){       
    // ALOG_INF(PSTR("GetPaletteNameByID  A%d"), palette_id);
    mPaletteI->GetPaletteNameByID(palette_id, buffer, buflen);
  }
  
  /**************************************************************
   * 
   * CUSTOM_PALETTE
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED) && (palette_id < mPaletteI->GetPaletteListLength()))
  ){  
  // ALOG_INF(PSTR("GetPaletteNameByID B %d"), palette_id);
    int8_t adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; //also skip the rgbcct colour names, though, they should be static?
    DLI->GetDeviceName_WithModuleUniqueID(GetModuleUniqueID(), adjusted_id, buffer, buflen);
    ALOG_DBM(PSTR("device name %s %d"),buffer, adjusted_id );
  }

  return buffer;

}



/**
 * @brief Gets palette names directly or from palette class when needed
 * 
 * @param palette_id 
 * @param buffer 
 * @param buflen 
 * @return const char* 
 */
int16_t mAnimatorLight::GetPaletteIDbyName(char* buffer)
{

  ALOG_DBG(PSTR("GetPaletteIDbyName %s"), buffer);

  int16_t found_id = -1;

  /**
   * @brief All static (progmem) palette names are stored in palette class
   * 
   * PALETTELIST_STATIC__IDS
   * PALETTELIST_SEGMENTS_STORED_VARIABLE_CRGBPALETTE16_PALETTES__IDS
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_STATIC_HTML_COLOUR_CODES__IDS
   */
  if((found_id=mPaletteI->Get_Static_PaletteIDbyName(buffer))>=0)
  {
    ALOG_DBG(PSTR("FOUND Static Name(%s) -> %d"), buffer, found_id);
  }else
  /**************************************************************
   * Dynamic User Defined Names
   * 
   * PALETTELIST_VARIABLE_HSBID__IDS
   * PALETTELIST_SEGMENT__RGBCCT_COLOUR__IDS
   * PALETTELIST_VARIABLE_GENERIC__IDS
   * 
  ***************************************************************/
  if((found_id = DLI->GetDeviceIDbyName(buffer, GetModuleUniqueID()))>=0)
  {
    ALOG_DBG(PSTR("FOUND Dynamic Name(%s) -> %d"), buffer, found_id);
  }
  else
  {
    ALOG_ERR(PSTR("ERROR NO PALETTE ID FOUND FOR Name(%s)"), buffer);

  }
  
  return found_id;

}


//get RGB values from color temperature in K (https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html)
void mAnimatorLight::colorKtoRGB(uint16_t kelvin, byte* rgb) //white spectrum to rgb, calc
{
  float r = 0, g = 0, b = 0;
  float temp = kelvin / 100;
  if (temp <= 66) {
    r = 255;
    g = round(99.4708025861 * log(temp) - 161.1195681661);
    if (temp <= 19) {
      b = 0;
    } else {
      b = round(138.5177312231 * log((temp - 10)) - 305.0447927307);
    }
  } else {
    r = round(329.698727446 * pow((temp - 60), -0.1332047592));
    g = round(288.1221695283 * pow((temp - 60), -0.0755148492));
    b = 255;
  } 
  //g += 12; //mod by Aircoookie, a bit less accurate but visibly less pinkish
  rgb[0] = (uint8_t) constrain(r, 0, 255);
  rgb[1] = (uint8_t) constrain(g, 0, 255);
  rgb[2] = (uint8_t) constrain(b, 0, 255);
  rgb[3] = 0;
}


// adjust RGB values based on color temperature in K (range [2800-10200]) (https://en.wikipedia.org/wiki/Color_balance)
uint32_t mAnimatorLight::colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb)
{
  //remember so that slow colorKtoRGB() doesn't have to run for every setPixelColor()
  if (lastKelvin != kelvin) colorKtoRGB(kelvin, correctionRGB);  // convert Kelvin to RGB
  lastKelvin = kelvin;
  byte rgbw[4];
  rgbw[0] = ((uint16_t) correctionRGB[0] * R(rgb)) /255; // correct R
  rgbw[1] = ((uint16_t) correctionRGB[1] * G(rgb)) /255; // correct G
  rgbw[2] = ((uint16_t) correctionRGB[2] * B(rgb)) /255; // correct B
  rgbw[3] =                                W(rgb);
  return RGBW32(rgbw[0],rgbw[1],rgbw[2],rgbw[3]);
}


//approximates a Kelvin color temperature from an RGB color.
//this does no check for the "whiteness" of the color,
//so should be used combined with a saturation check (as done by auto-white)
//values from http://www.vendian.org/mncharity/dir3/blackbody/UnstableURLs/bbr_color.html (10deg)
//equation spreadsheet at https://bit.ly/30RkHaN
//accuracy +-50K from 1900K up to 8000K
//minimum returned: 1900K, maximum returned: 10091K (range of 8192)
uint16_t mAnimatorLight::approximateKelvinFromRGB(uint32_t rgb) {
  //if not either red or blue is 255, color is dimmed. Scale up
  uint8_t r = R(rgb), b = B(rgb);
  if (r == b) return 6550; //red == blue at about 6600K (also can't go further if both R and B are 0)

  if (r > b) {
    //scale blue up as if red was at 255
    uint16_t scale = 0xFFFF / r; //get scale factor (range 257-65535)
    b = ((uint16_t)b * scale) >> 8;
    //For all temps K<6600 R is bigger than B (for full bri colors R=255)
    //-> Use 9 linear approximations for blackbody radiation blue values from 2000-6600K (blue is always 0 below 2000K)
    if (b < 33)  return 1900 + b       *6;
    if (b < 72)  return 2100 + (b-33)  *10;
    if (b < 101) return 2492 + (b-72)  *14;
    if (b < 132) return 2900 + (b-101) *16;
    if (b < 159) return 3398 + (b-132) *19;
    if (b < 186) return 3906 + (b-159) *22;
    if (b < 210) return 4500 + (b-186) *25;
    if (b < 230) return 5100 + (b-210) *30;
                 return 5700 + (b-230) *34;
  } else {
    //scale red up as if blue was at 255
    uint16_t scale = 0xFFFF / b; //get scale factor (range 257-65535)
    r = ((uint16_t)r * scale) >> 8;
    //For all temps K>6600 B is bigger than R (for full bri colors B=255)
    //-> Use 2 linear approximations for blackbody radiation red values from 6600-10091K (blue is always 0 below 2000K)
    if (r > 225) return 6600 + (254-r) *50;
    uint16_t k = 8080 + (225-r) *86;
    return (k > 10091) ? 10091 : k;
  }
}



void mAnimatorLight::Init_Segments()
{ 

  //reset segment runtimes
  ALOG_ERR(PSTR("Phase out: duplicate code"));
  
  //reset segment runtimes
  for (segment &seg : segments) 
  {
    // ALOG_ERR(PSTR("seg.markForReset();A"));  
    seg.markForReset();
    seg.resetIfRequired();
  }

  for (segment &seg : segments) 
  {    
    // ALOG_ERR(PSTR("seg.markForReset();B"));

    seg.grouping_set(1);

    // SEGMENT_I(i).setOption(SEG_OPTION_ON, 1);
    // SEGMENT_I(i).opacity = 255;    
  }
  
}


void mAnimatorLight::Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index)
{

  ALOG_DBG(PSTR("Segment_AppendNew:: getSegmentsNum() new index %d of count:%d"), seg_index, getSegmentsNum());

  if (seg_index >= getSegmentsNum()) 
  {

    ALOG_DBG(PSTR("Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "), seg_index, getSegmentsNum());
    appendSegment(Segment(start_pixel, stop_pixel));
    seg_index = getSegmentsNum()-1; // segments are added at the end of list, -1 for index of LENGTH minus 1
    ALOG_DBG(PSTR("Segment_AppendNew::new seg_index %d"), seg_index);
      
  }
  else
  {  
    ALOG_DBG(PSTR("ELSEEEEEEEEEEEEEEEEEEEE Segment_AppendNew::new seg_index %d > %d getSegmentsNum(): Creating new segment "), seg_index, getSegmentsNum());
  }

}


/**
 * @brief Disables animator neopixel and configures so effect function will be called again
 * 
 * @param seg_i 
 */
void mAnimatorLight::SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i)
{
  SEGMENT_I(seg_i).anim_function_callback = nullptr; // disabled
  SEGMENT_I(seg_i).transitional = true;              // Set that basic manual animator will be called
  SEGMENT_I(seg_i).cycle_time__rate_ms = FRAMETIME;
}


/**
 * @brief 
 * 
 * @param id 
 * @param function 
 * @param name 
 * @param effect_config 
 * @param development_stage Added to allow for effects to be added but not shown in the UI. 0 = normal, 1 = beta, 2 = alpha, 3 = dev
 */
void mAnimatorLight::addEffect(uint8_t id, EffectFunction function, const char* effect_config, uint8_t development_stage)
{

  if (id < effects.function.size()) {
    effects.function[id]     = function;
    effects.config[id] = effect_config;
    effects.development_stage[id] = development_stage;
  } else {
    effects.function.push_back(function);
    effects.config.push_back(effect_config);
    effects.development_stage.push_back(development_stage);
    if (effects.count < effects.function.size()) effects.count++;
  }
  
}


void mAnimatorLight::SubTask_Effects()
{

  uint32_t nowUp = millis(); // Be aware, millis() rolls over every 49 days
  effect_start_time = nowUp + timebase;
  if (nowUp - _lastShow < MIN_SHOW_DELAY) return;
  bool doShow = false;

  _isServicing = true;
  segment_current_index = 0; 

  for (segment &seg : segments) 
  {
    
    // reset the segment runtime data if needed
    seg.resetIfRequired();

    if (!seg.isActive()) continue;   

    // _force_update = true; // temp force it

    #ifdef ENABLE_DEVFEATURE_LIGHT__EFFECT_SHOW_TIME_NEW
    // last condition ensures all solid segments are updated at the same time
    if(nowUp >= seg.next_time || _force_update || (doShow && seg.effect_id == EFFECTS_FUNCTION__SOLID_COLOUR__ID))
    {
    #else
    if((mTime::TimeReached(&seg.tSaved_AnimateRunTime, seg.get_transition_rate_ms())) || (_force_update))
    {
    #endif

      doShow = true;
      unsigned frameDelay = FRAMETIME;
            
      // @brief To be safe, always clear and if the effect has a callback, it will be activated inside its function again.
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
      #endif

      _virtualSegmentLength = seg.virtualLength();

      seg.UpdateBrightness();

      #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
      seg.performance.effect_build_us = micros();
      #endif
      DEBUG_LINE_HERE_TRACE
      #ifdef ENABLE_EFFECTS_TIMING_DEBUG_GPIO
      DEBUG_PIN1_SET(LOW);
      #endif

      seg.animation_has_anim_callback = false; // Any animations required will reset this to true in the effect call
        
      ALOG_DBM(PSTR("seg.effect_id %d/%d"), seg.effect_id, effects.function.size());
      
      frameDelay = (this->*effects.function[seg.effect_id])(); // Call Effect Function (passes and returns nothing)
      
      #ifdef ENABLE_EFFECTS_TIMING_DEBUG_GPIO
      DEBUG_PIN1_SET(HIGH);
      #endif
      DEBUG_LINE_HERE_TRACE
      #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
      seg.performance.effect_build_us = micros() - seg.performance.effect_build_us;
      #endif
      
      seg.next_time = nowUp + frameDelay; 

      #ifndef ENABLE_DEVFEATURE_LIGHT__EFFECT_SHOW_TIME_NEW
      // Tmp fix
      if(
        (seg.animation_has_anim_callback==false)&&   // WLED method
        (1)
      ){
        seg.cycle_time__rate_ms = frameDelay; // use returned timing to ensure next call
      } 
      #endif

//       /**
//        * @brief Temporary fix to enable phasing out of animator
//        **/
//       if(frameDelay != USE_ANIMATOR) //assume we do not want animator
//       {
//         seg.anim_function_callback = nullptr;
//         Serial.println("DISABLE ANIM");
//       }else{
//         Serial.println("A---");
//         // Temp, cycle time needs to be set here? or is assumed set elsewhere
        
// // #define DIRECT_MODE(x)  SEGMENT.anim_function_callback = nullptr; SEGMENT.cycle_time__rate_ms = x;

//       }

//       /**
//        * @brief Only calls Animator if effects are not directly handled
//        **/
//       if(seg.anim_function_callback == nullptr) // assumes direct update
//       {
//         seg.flags.animator_first_run = false; // Animator not used, so reset first_run here
//       }
//       else

      if(seg.animation_has_anim_callback)
      { 
        StartSegmentAnimation_AsAnimUpdateMemberFunction(segment_current_index); // First run must be reset after StartAnimation is first called 
        // Serial.println("ANIM CALLED");
      }
      
      if(!seg.animation_has_anim_callback)  // Direct method, which the effect has ran above once so can be reset here
      {
        SEGMENT.flags.animator_first_run = RESET_FLAG; // if not
      }
      
      seg.call++; // Used as progress counter for animations eg rainbow across all hues
      ALOG_DBL(PSTR("seg=%d,call=%d"),seg_i, seg.call);
                
    } // END if effect needs to be called

      
    /**
     * @brief If animator is used, then the animation will be called from the animator
     * TODO: Animator should be totally phased out
     **/    
    if(seg.animator->IsAnimating())
    {
      // Serial.println("ISAnimating");

      seg.animator->UpdateAnimations();
      doShow = true; // Animator updated, so trigger SHOW
      SEGMENT.flags.animator_first_run = RESET_FLAG;     // CHANGE to function: reset here for all my methods
      
      if(!seg.animator->IsAnimating()){
        seg.transitional = false; // Since this is already inside "IsAnimating" then it should only be set if it was animating but is now stopping
      }

    } // IsAnimating

    DEBUG_LINE_HERE

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    if(doShow)
    {
      seg.performance.elapsed_last_show = millis() - seg.performance.millis_last_show;
      seg.performance.millis_last_show = millis();
      seg.performance.fps = 1000.0f/seg.performance.elapsed_last_show;
    }
    #endif 


    /**
     * @brief Now iter forward on active segment
     **/
    if( segment_current_index < getSegmentsNum() - 1 ) segment_current_index++; // fix for segment_current_index final iter going beyond getSegmentsNum() causing invalid index memory access
    
  } // END for
  
  #ifdef WLED_DEBUG
  if ((_targetFps != FPS_UNLIMITED) && (millis() - nowUp > _frametime)) DEBUG_PRINTF_P(PSTR("Slow effects %u/%d.\n\r"), (unsigned)(millis()-nowUp), (int)_frametime);
  #endif  
  if(doShow)
  {
    // Serial.println("SHOW");
    yield();
    // Segment::handleRandomPalette(); NOTE THIS IS RANDOMISE, NOW MOVED OUT OF THE PALETTE // slowly transition random palette; move it into for loop when each segment has individual random palette
    show();
    _lastShow = nowUp;
  }   
  #ifdef WLED_DEBUG
  if ((_targetFps != FPS_UNLIMITED) && (millis() - nowUp > _frametime)) DEBUG_PRINTF_P(PSTR("Slow strip %u/%d.\n\r"), (unsigned)(millis()-nowUp), (int)_frametime);
  #endif
      
  _force_update = false;
  _isServicing = false;
  
} // SubTask_Effects


uint8_t mAnimatorLight::GetNumberOfColoursInPalette(uint16_t palette_id)
{

  uint8_t palette_colour_count = 0;

  // Pass pointer to memory location to load, so I can have additional palettes. If none passed, assume primary storage of segment

  // ALOG_INF(PSTR("============LoadPalette %d %d %d"), palette_id, segment_index, tkr_anim->segment_current_index);

  /**
   * @brief PaletteList Vectors should have the length stored in it. Actual pixel count depends on encoding type
   **/
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID))
    //  ||
    // ((palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID)    && (palette_id < mPalette::PALETTELIST_VARIABLE_HSBID_LENGTH__ID)) ||
    // ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS)  && (palette_id < mPaletteI->GetPaletteListLength()))
  ){   

    uint16_t encoded_colour_width = 0;

    uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    mPalette::PALETTE_DATA pal = mPaletteI->static_palettes[constrain(palette_id_adj,0,mPaletteI->static_palettes.size()-1)];

    // Serial.println(ptr->encoding.data, BIN);

    if(pal.encoding.red_enabled){ encoded_colour_width++; }
    if(pal.encoding.green_enabled){ encoded_colour_width++; }
    if(pal.encoding.blue_enabled){ encoded_colour_width++; }
    if(pal.encoding.white_warm_enabled){ encoded_colour_width++; }

    if(pal.encoding.white_cold_enabled){ encoded_colour_width++; }
    if(pal.encoding.encoded_value_byte_width){ encoded_colour_width += pal.encoding.encoded_value_byte_width; }

    // if(pal.encoding.index_exact){ encoded_colour_width++; }
    if(pal.encoding.index_gradient){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
    if(pal.encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
    
    // if(pal.encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
    if(pal.encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
    if(pal.encoding.palette_can_be_modified){ encoded_colour_width++; }


    if(encoded_colour_width==0)
    {
      // ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
      return palette_colour_count;
    }
    
    // ALOG_INF(PSTR("============  pal.data_length/encoded_colour_width %d %d"),  pal.data_length, encoded_colour_width);
  
    palette_colour_count = pal.data.size()/encoded_colour_width; 
 
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
  ){
    palette_colour_count = 1;
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)
  ){
    palette_colour_count = 16;
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID)
  ){  
    palette_colour_count = 16;    
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  
    palette_colour_count = 1;    
  }
  else
  if(
    (palette_id >= mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC__LENGTH__ID)
  ){  
    ALOG_INF(PSTR("Temporary fix, needs its own palette count"));
    palette_colour_count = 1;    
    /**
     * @brief These are all different, doing a temporary fix for now
     * 
     */
    switch(palette_id)
    {
      default:
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID:
        palette_colour_count = 1;
      break;
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID:
        palette_colour_count = 1;
      break;
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID:
        palette_colour_count = 16;
     break;
      case mPalette::PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID:
        palette_colour_count = sizeof(PM_PALETTE_DYNAMIC__SOLAR_SKY_01__DATA)/6;
      break;
      // case mPalette::PALETTELIST_DYNAMIC__ENCODED_GENERIC__ID:{
      //   // if(SEGMENT.palette_container->pData.size())
      //   // {
      //   //   palette_colour_count = (SEGMENT.palette_container->pData.size()-2)/3;
      //   // }else{
      //     palette_colour_count = 1;
      //   // }
            
      //   uint16_t encoded_colour_width = 0;

      //   uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;
      //   uint8_t index = constrain(palette_id_adj,0,mPaletteI->dynamic_palettes.size()-1);
        
      //   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
      //   ALOG_INF(PSTR("addDynamicPalette bytes added[3] %d"), mPaletteI->dynamic_palettes[3].data.size());
      //   for(int i=0;i<mPaletteI->dynamic_palettes[3].data.size();i++){ Serial.print( mPaletteI->dynamic_palettes[3].data[i]); Serial.print( "," ); } Serial.println();
      //   ALOG_INF(PSTR("index = %d"), index);
      //   #endif

      //   mPalette::PALETTE_DATA pal = mPaletteI->dynamic_palettes[index];

      //   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
      //   for(int i=0;i<pal.data.size();i++)
      //   {
      //     Serial.print(i, DEC);
      //     Serial.print("\t");
      //     Serial.print(pal.data[i], DEC);
      //     Serial.println();
      //   }
      //   Serial.println();
      //   Serial.println(pal.encoding.data, BIN);
      //   #endif // ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS

      //   if(pal.encoding.red_enabled){ encoded_colour_width++; }
      //   if(pal.encoding.green_enabled){ encoded_colour_width++; }
      //   if(pal.encoding.blue_enabled){ encoded_colour_width++; }
      //   if(pal.encoding.white_warm_enabled){ encoded_colour_width++; }

      //   if(pal.encoding.white_cold_enabled){ encoded_colour_width++; }
      //   if(pal.encoding.encoded_value_byte_width){ encoded_colour_width += pal.encoding.encoded_value_byte_width; }

      //   // if(pal.encoding.index_exact){ encoded_colour_width++; }
      //   if(pal.encoding.index_gradient){ encoded_colour_width++; }
      //   if(pal.encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
      //   if(pal.encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
        
      //   // if(pal.encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
      //   if(pal.encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
      //   if(pal.encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
      //   if(pal.encoding.palette_can_be_modified){ encoded_colour_width++; }


      //   if(encoded_colour_width==0)
      //   {
      //     // ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
      //     return palette_colour_count;
      //   }

            
      
      //   palette_colour_count = pal.data.size()/encoded_colour_width; 

      //   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
      //   ALOG_INF(PSTR("============  pal.data.SIZE/width/count %d %d %D"),  pal.data.size(), encoded_colour_width, palette_colour_count);
      //   #endif


      // }break;
      case mPalette::PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID:
        palette_colour_count = 1;
      break;
    }
  }
  else
  if(
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){   
    palette_colour_count = 16;
  }else

  /**
   * @brief CustomPalettes
   * 
   */
  if(
    (palette_id >= mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED) && (palette_id < mPaletteI->GetPaletteListLength())
  ){  

    uint16_t palette_adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; // adjust back into correct indexing

    uint8_t colour_width  = mPaletteI->GetEncodedColourWidth(mPaletteI->custom_palettes[palette_adjusted_id].encoding); 
    palette_colour_count = mPaletteI->custom_palettes[palette_adjusted_id].data.size()/colour_width;

  }

    
  return palette_colour_count;

}


/*
 * Fills segment with color
 */
void mAnimatorLight::fill(uint32_t c)//, bool apply_brightness) 
{
  for(uint16_t i = 0; i < _virtualSegmentLength; i++) 
  {
    SEGMENT.setPixelColor(i, c);//, apply_brightness);
  }
}

void mAnimatorLight::fill(RgbwwColor c)//, bool apply_brightness) 
{
  for(uint16_t i = 0; i < _virtualSegmentLength; i++) 
  {
    SEGMENT.setPixelColor(i, c);//, apply_brightness);
  }
}


void mAnimatorLight::fill_ranged(uint32_t c)//, bool apply_brightness) 
{

  for(uint16_t i = SEGMENT.start; i <= SEGMENT.stop; i++) {
    SEGMENT.setPixelColor(i, c);//, apply_brightness);
  }

}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks
 * @note : Converted from WLED Effects
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint32_t mAnimatorLight::crgb_to_col(CRGB crgb)
{
  uint32_t colour = 0; // suppress White colour (MSB)
  colour = (((uint32_t)crgb.red << 16) | ((uint32_t)crgb.green << 8) | crgb.blue);
  return colour;
}


CRGB mAnimatorLight::col_to_crgb(uint32_t color)
{
  CRGB crgb;
  crgb.red =   (color >> 16 & 0xFF);
  crgb.green = (color >> 8  & 0xFF);
  crgb.blue =  (color       & 0xFF);
  return crgb;
}


mAnimatorLight& mAnimatorLight::SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE)
{
  this->SEGMENT_I(segment_index).anim_function_callback = anim_function_callback;
  SEGMENT_I(segment_index).animation_has_anim_callback = true;
  return *this;
}


/**
 * Begin at "StartingColor" at 0% then return to "DesiredColor" at 100%
 * */
void mAnimatorLight::StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index)
{ 

  uint16_t time_tmp = 0;    
  time_tmp = SEGMENT_I(segment_index).animator_blend_time_ms();//    SEGMENT_I(segment_index).time_ms; 

  /**
   * @brief Testing as a temp solution, if neopixelbus gamma is not being used 
   * then to avoid needing complex apply brightness is the effects, the brightness is applied here
   * once to the starting position
   * 
   */
  #ifdef ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG_BRIGHTNESS_ON_START_OF_ANIMATION //does nto work for firefly
  error
  ALOG_INF(PSTR("Adjusting Brightness %d %d"), SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied());
  SEGMENT.Update_DynamicBuffer_DesiredColour_Brightness( SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied() );
  #endif

  /**
   * @brief Before starting animation, check the override states to see if they are active
   **/
  // Overwriting single SEGMENT_I(0) methods, set, then clear
  if(SEGMENT_I(segment_index).single_animation_override.time_ms>0)
  {
    time_tmp = SEGMENT_I(segment_index).single_animation_override.time_ms;
    SEGMENT_I(segment_index).single_animation_override.time_ms = 0; // reset overwrite
    ALOG_DBM(PSTR("Override: TimeMs: %d"), time_tmp);
  }

  if(SEGMENT_I(segment_index).animation_has_anim_callback == true)
  {
    SEGMENT_I(segment_index).animator->StartAnimation(0, time_tmp, SEGMENT_I(segment_index).anim_function_callback);
  }

  SEGMENT_I(segment_index).transitional = true;

} //end function


/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}


/********************************************************************************************************************************
**********Flasher Function ******************************************************************************************************
********************************************************************************************************************************/
void mAnimatorLight::CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index ){
  
  if(SEGMENT_I(segment_index).effect_id != value)
  {
    SEGMENT_I(segment_index).markForReset();
    SEGMENT_I(segment_index).effect_id = value;      //make function "changeFlasherFunction" so then the region is automatically updated internally
  }
  SEGMENT_I(segment_index).aux0 = 0;//EFFECTS_REGION_COLOUR_SELECT_ID;
  SEGMENT_I(segment_index).flags.animator_first_run= true; // first run, so do extra things

  ALOG_DBM(PSTR("segments[segment_index].effect_id=%d"),segments[segment_index].effect_id);
  
  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  setCallback_ConstructJSONBody_Debug_Animations_Progress(nullptr); // clear to be reset
  #endif
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  char buffer[30];
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_NEO D_COMMAND_SVALUE_SVALUE_K(D_EFFECTS, D_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


int16_t mAnimatorLight::GetFlasherFunctionIDbyName(const char* f)
{
  if (f == nullptr) return -2;

  char lineBuffer[256];
  for (size_t i = 0; i < getModeCount(); i++) {
    strncpy_P(lineBuffer, getModeData_Config(i), sizeof(lineBuffer) - 1);
    lineBuffer[sizeof(lineBuffer) - 1] = '\0'; // terminate string

    ALOG_DBM(PSTR("lineBuffer %d %s"), i, lineBuffer);

    char* dataPtr = strchr(lineBuffer, '@');
    if (dataPtr) *dataPtr = '\0'; // replace name divider with null termination. Escape "name@data"

    // Check for a match with the provided name
    if (strcmp(f, lineBuffer) == 0
#ifdef ENABLE_DEVFEATURE_LIGHTING__SLOW_GLOW_LEGACY_FIX
        || (strcmp(lineBuffer, "Firefly") == 0 && strcmp(f, "Slow Glow") == 0)
#endif
    ) {
      ALOG_INF(PSTR("GetFlasherFunctionIDbyName %s %d"), f, i);
      return i;
    }
  }

  return -1;
}



const char* mAnimatorLight::GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index )
{
  return GetFlasherFunctionNamebyID(SEGMENT_I(segment_index).effect_id, buffer, buflen);
}


const char* mAnimatorLight::GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen, bool return_first_option_only)
{

  if(id<getEffectsAmount()){
    snprintf_P(buffer, buflen, effects.config[id]);  
    
    char* dataPtr = strchr(buffer,'@');
    if (dataPtr) *dataPtr = 0; // replace name dividor with null termination. Escape "name@data"

  }else{
    snprintf(buffer, buflen, "id_err %d", id);  
  }

  return buffer;
  
}


void mAnimatorLight::FileSystem_JsonAppend_Save_Module()
{

  uint8_t bus_appended = 0;

  char buffer[100] = {0};

  for(uint8_t seg_i = 0; seg_i< getSegmentsNum(); seg_i++)
  {
    JBI->Object_Start_F("Segment%d",seg_i);      

      JBI->Array_Start("PixelRange");
        JBI->Add(segments[seg_i].start);
        JBI->Add(segments[seg_i].stop);
      JBI->Array_End();

      JBI->Add("ColourPalette", GetPaletteNameByID(segments[seg_i].palette_id, buffer, sizeof(buffer)) );

      JBI->Object_Start_F("Effects");      
        JBI->Add("Function",  GetFlasherFunctionNamebyID(segments[seg_i].effect_id, buffer, sizeof(buffer), true));
        JBI->Add("Speed",     segments[seg_i].speed );
        JBI->Add("Intensity", segments[seg_i].intensity );
      JBI->Object_End();
      JBI->Object_Start_F("Transition");
        JBI->Add("TimeMs",  segments[seg_i].animator_blend_time_ms() );
        JBI->Add("RateMs",  segments[seg_i].cycle_time__rate_ms );
      JBI->Object_End();

      for(uint8_t seg_col = 0; seg_col < 5; seg_col++)
      {
        JBI->Array_Start_P(PSTR("SegColour%d"), seg_col);
          for(uint8_t p=0;p<5;p++)
          { 
            JBI->Add(segments[seg_i].segcol[seg_col].colour.R); 
            JBI->Add(segments[seg_i].segcol[seg_col].colour.G); 
            JBI->Add(segments[seg_i].segcol[seg_col].colour.B); 
            JBI->Add(segments[seg_i].segcol[seg_col].colour.WW); 
            JBI->Add(segments[seg_i].segcol[seg_col].colour.CW); 
          }
        JBI->Array_End();
      }

    JBI->Object_End();
  }


}


bool mAnimatorLight::isAsterisksOnly(const char* str, byte maxLen)
{
  for (unsigned i = 0; i < maxLen; i++) {
    if (str[i] == 0) break;
    if (str[i] != '*') return false;
  }
  //at this point the password contains asterisks only
  return (str[0] != 0); //false on empty string
}


/**
 * @brief Blends between Rgbcct with 0 to 255 range
 * 
 * @param color1 
 * @param color2 
 * @param blend 
 * @return RgbwwColor 
 */
RgbwwColor mAnimatorLight::ColourBlend(RgbwwColor color1, RgbwwColor color2, uint8_t blend) 
{
  Serial.println("here1");
  return RgbwwColor::LinearBlend(color1, color2, mSupport::mapfloat(blend, 0,255, 0.0f, 1.0f));
}

uint32_t mAnimatorLight::ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend) 
{
  if(blend == 0)   return color1;
  if(blend == 255) return color2;

  uint32_t w1 = (color1 >> 24) & 0xff;
  uint32_t r1 = (color1 >> 16) & 0xff;
  uint32_t g1 = (color1 >>  8) & 0xff;
  uint32_t b1 =  color1        & 0xff;

  uint32_t w2 = (color2 >> 24) & 0xff;
  uint32_t r2 = (color2 >> 16) & 0xff;
  uint32_t g2 = (color2 >>  8) & 0xff;
  uint32_t b2 =  color2        & 0xff;

  uint32_t w3 = ((w2 * blend) + (w1 * (255 - blend))) >> 8;
  uint32_t r3 = ((r2 * blend) + (r1 * (255 - blend))) >> 8;
  uint32_t g3 = ((g2 * blend) + (g1 * (255 - blend))) >> 8;
  uint32_t b3 = ((b2 * blend) + (b1 * (255 - blend))) >> 8;

  return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));
}

///////////////////////////////////////////////////////////////////////////////
// Segment class implementation
///////////////////////////////////////////////////////////////////////////////
uint16_t mAnimatorLight::Segment::_usedSegmentData = 0U; // amount of RAM all segments use for their data[]
// CRGB    *mAnimatorLight::Segment::_globalLeds = nullptr;
uint16_t mAnimatorLight::Segment::maxWidth = DEFAULT_LED_COUNT;
uint16_t mAnimatorLight::Segment::maxHeight = 1;

// copy assignment
// mAnimatorLight::Segment& mAnimatorLight::Segment::operator= (const mAnimatorLight::Segment &orig) {
  
  
//   //DEBUG_PRINTLN(F("-- Copying segment --"));
//   if (this != &orig) {
//     // clean destination
//     if (name) delete[] name;
//     // if (leds && !mAnimatorLight::Segment::_globalLeds) free(leds);
//     deallocateData();
//         deallocateColourData();
//     // copy source
//     memcpy(this, &orig, sizeof(mAnimatorLight::Segment));
//     // erase pointers to allocated data
//     name = nullptr;
//     data = nullptr;
//     _dataLen = 0;
//     // if (!mAnimatorLight::Segment::_globalLeds) leds = nullptr;
//     // copy source data
//     if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
//     if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
//     // if (orig.leds && !mAnimatorLight::Segment::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
//   }
      
//   return *this;
// }

mAnimatorLight::Segment& mAnimatorLight::Segment::operator=(const mAnimatorLight::Segment& orig) 
{
  if (this != &orig) 
  {
    // Clean destination
    if (name) {
        delete[] name;
        name = nullptr;
    }
    deallocateData();
    deallocateColourData();

    // Copy all members from the source
    memcpy(this, &orig, sizeof(mAnimatorLight::Segment));

    // Reset pointers in destination (they will be reallocated if needed)
    name = nullptr;
    data = nullptr;
    coldata = nullptr;
    _dataLen = 0;
    _coldataLen = 0;

    // Copy allocated data
    if (orig.name) {
        name = new char[strlen(orig.name) + 1];
        if (name) {
            strcpy(name, orig.name);
        }
    }
    if (orig.data && orig._dataLen > 0) {
        if (allocateData(orig._dataLen)) {
            memcpy(data, orig.data, orig._dataLen);
        }
    }
    if (orig.coldata && orig._coldataLen > 0) {
          DEBUG_LINE_HERE
        if (allocateColourData(orig._coldataLen)) {
          DEBUG_LINE_HERE
            memcpy(coldata, orig.coldata, orig._coldataLen);
          DEBUG_LINE_HERE
        }
    }
  }
          DEBUG_LINE_HERE

          delay(3000);

  return *this;
}


// move assignment
// mAnimatorLight::Segment& mAnimatorLight::Segment::operator= (mAnimatorLight::Segment &&orig) noexcept {
  
      
//   //DEBUG_PRINTLN(F("-- Moving segment --"));
//   if (this != &orig) {
//     if (name) delete[] name; // free old name
//     deallocateData(); // free old runtime data
//     if (leds && !mAnimatorLight::Segment::_globalLeds) free(leds);
//     memcpy(this, &orig, sizeof(mAnimatorLight::Segment));
//     orig.name = nullptr;
//     orig.data = nullptr;
//     orig._dataLen = 0;
//     orig.leds = nullptr;
//   }
      
//   return *this;
// }

// move assignment
mAnimatorLight::Segment& mAnimatorLight::Segment::operator=(mAnimatorLight::Segment&& orig) noexcept {
    if (this != &orig) {
        // Free any existing resources
        if (name) {
            delete[] name;
            name = nullptr;
        }
        deallocateData();
        deallocateColourData();

        // Move all members from the source
        memcpy(this, &orig, sizeof(mAnimatorLight::Segment));

        // Nullify pointers in the source object
        orig.name = nullptr;
        orig.data = nullptr;
        orig.coldata = nullptr;
        orig._dataLen = 0;
        orig._coldataLen = 0;
    }

    return *this;
}


bool mAnimatorLight::Segment::allocateData(size_t len) 
{
  
  if (data && _dataLen == len) return true; //already allocated
  deallocateData();
  if (mAnimatorLight::Segment::getUsedSegmentData() + len > MAX_SEGMENT_DATA)
  { 
    ALOG_ERR( PM_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
    effect_id = 0;//EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return false; //not enough memory
  }
  // if possible use SPI RAM on ESP32
  #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
  if (psramFound())
    data = (byte*) ps_malloc(len);
  else
  #endif

  data = (byte*) malloc(len);

  if (!data){
    ALOG_ERR( PM_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
    effect_id = 0;//EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return false; //allocation failed
  }

  addUsedSegmentData(len);
  _dataLen = len;
  memset(data, 0, len);

  DEBUG_LINE_HERE

  return true;
}

void mAnimatorLight::Segment::deallocateData()
{
      // 
  if (!data) return;
      // 
  free(data);
  data = nullptr;
      // 
  addUsedSegmentData(-_dataLen);
      // 
  _dataLen = 0;
}

bool mAnimatorLight::Segment::allocateColourData(uint16_t len) {
    DEBUG_LINE_HERE;

    // Check if memory is already allocated and matches the required length
    if (coldata && _coldataLen == len) {
        DEBUG_LINE_HERE;
        return true; // Already allocated
    }

    // Free existing memory safely
    DEBUG_LINE_HERE;
    deallocateColourData();

    // Ensure len does not exceed maximum allowed segment data
    if (mAnimatorLight::Segment::getUsedSegmentData() + len > MAX_SEGMENT_DATA) {
        DEBUG_LINE_HERE;
        ALOG_ERR(PM_MEMORY_INSUFFICIENT); // Log memory insufficient error
        effect_id = 0; // Reset effect ID
        return false; // Not enough memory
    }

    // Attempt to allocate memory, prioritizing PSRAM on ESP32 if available
    #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
    if (psramFound()) {
        coldata = (byte*)ps_malloc(len);
    } else
    #endif
    {
        coldata = (byte*)malloc(len);
    }

    // Check if allocation was successful
    if (!coldata) {
        DEBUG_LINE_HERE;
        ALOG_ERR(PM_MEMORY_INSUFFICIENT); // Log memory insufficient error
        effect_id = 0; // Reset effect ID
        return false; // Allocation failed
    }

    // Update memory usage and initialize
    DEBUG_LINE_HERE;
    addUsedSegmentData(len);
    DEBUG_LINE_HERE;
    _coldataLen = len;
    DEBUG_LINE_HERE;
    memset(coldata, 0, len); // Initialize allocated memory to zero
    DEBUG_LINE_HERE;

    return true;
}

void mAnimatorLight::Segment::deallocateColourData() {
    // Safely free memory and reset pointers
    if (coldata) {
        free(coldata);
        coldata = nullptr; // Prevent dangling pointer
    }
    _coldataLen = 0;
  addUsedSegmentData(-_coldataLen);
    // subUsedSegmentData(_coldataLen); // Adjust used memory counter
}

// bool mAnimatorLight::Segment::allocateColourData(uint16_t len) {
//     // if (coldata) {
//     //     free(coldata);
//     //     coldata = nullptr;
//     // }

//     // if (length > 0) {
//     //     coldata = (uint8_t*)malloc(length);
//     //     if (coldata) {
//     //         _coldataLen = length;
//     //         memset(coldata, 0, length); // Initialize with zeros
//     //         return true;
//     //     } else {
//     //         _coldataLen = 0; // Allocation failed
//     //         return false;
//     //     }
//     // }

//     // _coldataLen = 0;
//     // return true; // No data to allocate

    
//   DEBUG_LINE_HERE
//   if (coldata && _coldataLen == len)
//   {
    
//   DEBUG_LINE_HERE
//     return true; //already allocated

//   }
//   DEBUG_LINE_HERE
//   deallocateColourData();
//   DEBUG_LINE_HERE
//   if (mAnimatorLight::Segment::getUsedSegmentData() + len > MAX_SEGMENT_DATA)
//   { 
//   DEBUG_LINE_HERE
//     ALOG_ERR( PM_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
//     effect_id = 0;//EFFECTS_FUNCTION__STATIC_PALETTE__ID;
//     return false; //not enough memory
//   }
//   DEBUG_LINE_HERE
//   // if possible use SPI RAM on ESP32
//   #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
//   if (psramFound())
//     coldata = (byte*) ps_malloc(len);
//   else
//   #endif

//   DEBUG_LINE_HERE
//   coldata = (byte*) malloc(len);
//   DEBUG_LINE_HERE

//   if (!coldata){
//   DEBUG_LINE_HERE
//     ALOG_ERR( PM_MEMORY_INSUFFICIENT ); // This is the base case, none will be fallback
//     effect_id = 0;//EFFECTS_FUNCTION__STATIC_PALETTE__ID;
//     return false; //allocation failed
//   }

//   DEBUG_LINE_HERE
//   addUsedSegmentData(len);
//   DEBUG_LINE_HERE
//   _coldataLen = len;
//   DEBUG_LINE_HERE
//   memset(coldata, 0, len);
//   DEBUG_LINE_HERE

//   DEBUG_LINE_HERE

// }

// void mAnimatorLight::Segment::deallocateColourData() 
// {
//   if (coldata) 
//   {
//     free(coldata);
//     coldata = nullptr;
//   }
//   _coldataLen = 0;
// }




/** 
  * If reset of this segment was requested, clears runtime
  * settings of this segment.
  * Must not be called while an effect mode function is running
  * because it could access the data buffer and this method 
  * may free that data buffer.
  */
void mAnimatorLight::Segment::resetIfRequired() {
  if (reset) {
    
  // ALOG_INF(PSTR(D_LOG_PIXEL "resetIfRequired AuxOptions Segment = %d,%d,%d,%d"),
  //   aux0,
  //   aux1,
  //   aux2,
  //   aux3
  // );

    // if (leds && !mAnimatorLight::Segment::_globalLeds) { free(leds); leds = nullptr; }
    //if (_t) { delete _t; _t = nullptr; transitional = false; }
    next_time = 0; step = 0; call = 0; 
    
    /**
     * @brief Potential issue with WLED effects, but removing aux options from reset since they may be used as config options
     * 
     */
    aux0 = 0; aux1 = 0;  aux2 = 0;  aux3 = 0; 
    
    
    reset = false; // setOption(SEG_OPTION_RESET, false);
    // Serial.println(DEBUG_INSERT_PAGE_BREAK "mAnimatorLight::Segment::resetIfRequired()"); //delay(5000);
  }
}

// void mAnimatorLight::Segment::setUpLeds() {
//   // deallocation happens in resetIfRequired() as it is called when segment changes or in destructor
//   if (mAnimatorLight::Segment::_globalLeds)
//     #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//     leds = &mAnimatorLight::Segment::_globalLeds[start + startY*mAnimatorLight::Segment::maxWidth];
//     #else
//     leds = &mAnimatorLight::Segment::_globalLeds[start];
//     #endif
//   else if (!leds) {
//     #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
//     if (psramFound())
//       leds = (CRGB*)ps_malloc(sizeof(CRGB)*length());
//     else
//     #endif
//       leds = (CRGB*)malloc(sizeof(CRGB)*length());
//   }
// }


void mAnimatorLight::Segment::setUp(uint16_t i1, uint16_t i2, uint8_t grp, uint8_t spc, uint16_t ofs, uint16_t i1Y, uint16_t i2Y) {
  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (start == i1 && stop == i2);
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (Segment::maxHeight>1) boundsUnchanged &= (startY == i1Y && stopY == i2Y); // 2D
  #endif
  if (boundsUnchanged
      && (!grp || (grouping_get() == grp && spacing == spc))
      && (ofs == UINT16_MAX || ofs == offset)) return;

  if (stop) fill(BLACK); //turn old segment range off
  if (i2 <= i1) { //disable segment
    stop = 0;
    markForReset();
    return;
  }
  if (i1 < Segment::maxWidth || (i1 >= Segment::maxWidth*Segment::maxHeight && i1 < tkr_anim->getLengthTotal())) start = i1; // Segment::maxWidth equals tkr_anim->getLengthTotal() for 1D
  stop = i2 > Segment::maxWidth*Segment::maxHeight ? MIN(i2,tkr_anim->getLengthTotal()) : (i2 > Segment::maxWidth ? Segment::maxWidth : MAX(1,i2));
  startY = 0;
  stopY  = 1;
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (Segment::maxHeight>1) { // 2D
    if (i1Y < Segment::maxHeight) startY = i1Y;
    stopY = i2Y > Segment::maxHeight ? Segment::maxHeight : MAX(1,i2Y);
  }
  #endif
  if (grp) {
    grouping_set(grp);
    spacing = spc;
  }
  if (ofs < UINT16_MAX) offset = ofs;
  markForReset();
  if (!boundsUnchanged) refreshLightCapabilities();
}




bool mAnimatorLight::Segment::setColor(uint8_t slot, uint32_t c) { //returns true if changed


  // segcol[slot] = RgbcctColor::GetRgbcctFromU32Colour(c); 
  segcol[slot].colour = RgbwwColor(R(c), G(c), B(c), W(c), W(c));
  
  // RgbcctColor(c); 

  // if (slot >= NUM_COLORS || c == colors[slot]) return false;
  // if (fadeTransition) startTransition(tkr_anim->getTransition()); // start transition prior to change
  // colors[slot] = c;
  // stateChanged = true; // send UDP/WS broadcast
  return true;
}

bool mAnimatorLight::Segment::setColor(uint8_t slot, RgbwwColor c) { //returns true if changed


  segcol[slot].colour = c;

  // if (slot >= NUM_COLORS || c == colors[slot]) return false;
  // if (fadeTransition) startTransition(tkr_anim->getTransition()); // start transition prior to change
  // colors[slot] = c;
  // stateChanged = true; // send UDP/WS broadcast
  return true;
}

void mAnimatorLight::Segment::setCCT(uint16_t k) {

  ALOG_INF(PSTR("setCCT: slider shows current RGBCCT active, setting updates that colour if active %d"), k);

  if(palette_id < 5)
  {
    ALOG_INF(PSTR("setCCT: palette_id%d < 5, updating colour %d"), palette_id, k);

    // segcol[palette_id].setCCT_Kelvin(k);
    segcol[palette_id].setCCT(k);

    SERIAL_DEBUG_COLRGBWWi("aftercct", segcol[palette_id].colour, palette_id);
    
  }
  
  // stateChanged = true; // send UDP/WS broadcast
}


void mAnimatorLight::Segment::setOption(uint8_t n, bool val) {
  bool prevOn = on;
  // if (fadeTransition && n == SEG_OPTION_ON && val != prevOn) startTransition(tkr_anim->getTransition()); // start transition prior to change
  if (val) options |=   0x01 << n;
  else     options &= ~(0x01 << n);
  // if (!(n == SEG_OPTION_SELECTED || n == SEG_OPTION_RESET || n == SEG_OPTION_TRANSITIONAL)) stateChanged = true; // send UDP/WS broadcast
}

void mAnimatorLight::Segment::setMode(uint8_t fx, bool loadDefaults) 
{
  ALOG_INF(PSTR("Load FXData Defaults %d"), fx);
  // if we have a valid mode & is not reserved
  // if (fx < tkr_anim->getModeCount() && strncmp_P("RSVD", tkr_anim->getModeData(fx), 4)) {
  //   if (fx != mode) {
  //     startTransition(tkr_anim->getTransition()); // set effect transitions
      //markForReset(); // transition will handle this
      effect_id = fx;

    ALOG_INF(PSTR("setMode These are likely where the parser takes the defaults from effects %d"), effect_id);


      // load default values from effect string
      // if (loadDefaults) {

        int16_t sOpt = -1;

        if ((sOpt = tkr_anim->extractModeDefaults(fx, "sx")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default sx=%d"), sOpt);
          speed = sOpt;
        }

        if ((sOpt = tkr_anim->extractModeDefaults(fx, "ix")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default ix=%d"), sOpt);
          intensity = sOpt;
        }

        if ((sOpt = tkr_anim->extractModeDefaults(fx, "pal")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default pal=%d"), sOpt);
          setPalette(sOpt);
        } 
        
        char paletteName[32]; // Buffer to store the extracted palette name
        // Check for the "paln" command (palette name)
        if (tkr_anim->extractModeDefaults(fx, "paln", paletteName, sizeof(paletteName)))
        {
          ALOG_INF(PSTR("Loading named palette: %s"), paletteName);
          int16_t tmp_id = -1;
          if((tmp_id=tkr_anim->GetPaletteIDbyName(paletteName))>=0){
            ALOG_INF(PSTR("tmp_id=%d"),tmp_id);
            palette_id = tmp_id;
          }
        }

        if ((sOpt = tkr_anim->extractModeDefaults(fx, "ra")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default ra=%d"), sOpt);
          cycle_time__rate_ms = sOpt;
        }

        if ((sOpt = tkr_anim->extractModeDefaults(fx, "ti")) >= 0) 
        {
          ALOG_INF(PSTR("Loading default ti=%d"), sOpt);
          // time_ms = sOpt;
        }






        // sOpt = tkr_anim->extractModeDefaults(fx, "c1");   if (sOpt >= 0) custom1   = sOpt;
        // sOpt = tkr_anim->extractModeDefaults(fx, "c2");   if (sOpt >= 0) custom2   = sOpt;
        // sOpt = tkr_anim->extractModeDefaults(fx, "c3");   if (sOpt >= 0) custom3   = sOpt;
        // sOpt = extractModeDefaults(fx, "m12");  if (sOpt >= 0) map1D2D   = constrain(sOpt, 0, 7);
        // sOpt = extractModeDefaults(fx, "si");   if (sOpt >= 0) soundSim  = constrain(sOpt, 0, 7);
        // sOpt = extractModeDefaults(fx, "rev");  if (sOpt >= 0) reverse   = (bool)sOpt;
        // sOpt = extractModeDefaults(fx, "mi");   if (sOpt >= 0) mirror    = (bool)sOpt; // NOTE: setting this option is a risky business
        // sOpt = extractModeDefaults(fx, "rY");   if (sOpt >= 0) reverse_y = (bool)sOpt;
        // sOpt = extractModeDefaults(fx, "mY");   if (sOpt >= 0) mirror_y  = (bool)sOpt; // NOTE: setting this option is a risky business
        
      // }



      // stateChanged = true; // send UDP/WS broadcast
    // }
  // }
}






// extracts mode parameter defaults from last section of mode data (e.g. "Juggle@!,Trail;!,!,;!;sx=16,ix=240,1d")
//;sx=16,ix=240,1d
int16_t mAnimatorLight::extractModeDefaults(uint8_t mode, const char *segVar)
{



  if (mode < getModeCount()) {
    char lineBuffer[128] = "";
    strncpy_P(lineBuffer, getModeData_Config(mode), 127);
    lineBuffer[127] = '\0'; // terminate string


    // ALOG_INF(PSTR("if (mode < getModeCount()) %d < %d %s"), mode, getModeCount(), lineBuffer);

    if (lineBuffer[0] != 0) {
      char* startPtr = strrchr(lineBuffer, ';'); // last ";" in FX data
      if (!startPtr) return -1;

      char* stopPtr = strstr(startPtr, segVar);
      if (!stopPtr) return -1;

      stopPtr += strlen(segVar) +1; // skip "="
      return atoi(stopPtr);
    }
  }
  return -1;
}

bool mAnimatorLight::extractModeDefaults(uint8_t mode, const char *segVar, char *outBuffer, size_t bufferSize)
{
  if (mode < getModeCount())
  {
    char lineBuffer[128] = "";
    strncpy_P(lineBuffer, getModeData_Config(mode), 127);
    lineBuffer[127] = '\0'; // Ensure null-termination

    if (lineBuffer[0] != 0)
    {
      char *startPtr = strrchr(lineBuffer, ';'); // Find the last ";"
      if (!startPtr) return false;

      char *stopPtr = strstr(startPtr, segVar);
      if (!stopPtr) return false;

      stopPtr += strlen(segVar) + 1; // Skip past the "segVar=" part

      // Extract the value into outBuffer
      char *valueEnd = strchr(stopPtr, ','); // Look for a delimiter (comma or null terminator)
      if (!valueEnd) valueEnd = strchr(stopPtr, '\0'); // If no comma, find the end of the string

      size_t length = valueEnd - stopPtr;
      if (length >= bufferSize) return false; // Ensure it fits in outBuffer

      strncpy(outBuffer, stopPtr, length);
      outBuffer[length] = '\0'; // Null-terminate the result

      return true;
    }
  }
  return false;
}


void mAnimatorLight::Segment::setPalette(uint8_t pal) 
{
  palette_id = pal;  
  ALOG_INF(PSTR("setPalette(%d)"), palette_id);
  
  #ifdef ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
  // Need to add this, since CRGBPalettes will not reload internally, so must load now
  tkr_anim->LoadPalette(palette_id, tkr_anim->getCurrSegmentId());  
  #endif 
}


// 2D matrix
uint16_t mAnimatorLight::Segment::virtualWidth() const {
  // 
  uint16_t groupLen = groupLength();
  uint16_t vWidth = ((transpose ? height() : width()) + groupLen - 1) / groupLen;
  // ALOG_INF(PSTR("virtualWidth() %d"), vWidth);
  if (mirror) vWidth = (vWidth + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  // 
  return vWidth;
}

uint16_t mAnimatorLight::Segment::virtualHeight() const {
  // 
  uint16_t groupLen = groupLength();
  uint16_t vHeight = ((transpose ? width() : height()) + groupLen - 1) / groupLen;
  // ALOG_INF(PSTR("virtualHeight() %d"), vHeight);
  if (mirror_y) vHeight = (vHeight + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  // 
  return vHeight;
}

uint16_t mAnimatorLight::Segment::nrOfVStrips() const {
  uint16_t vLen = 1;
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    switch (map1D2D) {
      case M12_pBar:
        vLen = virtualWidth();
        break;
    }
  }
#endif
  return vLen;
}

// // 1D strip
// uint16_t mAnimatorLight::Segment::virtualLength() const {
// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   if (is2D()) {
//     uint16_t vW = virtualWidth();
//     uint16_t vH = virtualHeight();
//     uint16_t vLen = vW * vH; // use all pixels from segment
//     switch (map1D2D) {
//       case M12_pBar:
//         vLen = vH;
//         break;
//       case M12_pCorner:
//       case M12_pArc:
//         vLen = max(vW,vH); // get the longest dimension
//         break;
//     }
//     return vLen;
//   }
// #endif
//   uint16_t groupLen = groupLength();
//   uint16_t vLength = (length() + groupLen - 1) / groupLen;
//   if (mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a single LED
//   return vLength;
// }


// Constants for mapping mode "Pinwheel"
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
constexpr int Pinwheel_Steps_Small = 72;       // no holes up to 16x16
constexpr int Pinwheel_Size_Small  = 16;       // larger than this -> use "Medium"
constexpr int Pinwheel_Steps_Medium = 192;     // no holes up to 32x32
constexpr int Pinwheel_Size_Medium  = 32;      // larger than this -> use "Big"
constexpr int Pinwheel_Steps_Big = 304;        // no holes up to 50x50
constexpr int Pinwheel_Size_Big  = 50;         // larger than this -> use "XL"
constexpr int Pinwheel_Steps_XL  = 368;
constexpr float Int_to_Rad_Small = (DEG_TO_RAD * 360) / Pinwheel_Steps_Small;  // conversion: from 0...72 to Radians
constexpr float Int_to_Rad_Med =   (DEG_TO_RAD * 360) / Pinwheel_Steps_Medium; // conversion: from 0...192 to Radians
constexpr float Int_to_Rad_Big =   (DEG_TO_RAD * 360) / Pinwheel_Steps_Big;    // conversion: from 0...304 to Radians
constexpr float Int_to_Rad_XL =    (DEG_TO_RAD * 360) / Pinwheel_Steps_XL;     // conversion: from 0...368 to Radians

constexpr int Fixed_Scale = 512;               // fixpoint scaling factor (9bit for fraction)

// Pinwheel helper function: pixel index to radians
static float getPinwheelAngle(int i, int vW, int vH) {
  int maxXY = max(vW, vH);
  if (maxXY <= Pinwheel_Size_Small)  return float(i) * Int_to_Rad_Small;
  if (maxXY <= Pinwheel_Size_Medium) return float(i) * Int_to_Rad_Med;
  if (maxXY <= Pinwheel_Size_Big)    return float(i) * Int_to_Rad_Big;
  // else
  return float(i) * Int_to_Rad_XL;
}
// Pinwheel helper function: matrix dimensions to number of rays
static int getPinwheelLength(int vW, int vH) {
  int maxXY = max(vW, vH);
  if (maxXY <= Pinwheel_Size_Small)  return Pinwheel_Steps_Small;
  if (maxXY <= Pinwheel_Size_Medium) return Pinwheel_Steps_Medium;
  if (maxXY <= Pinwheel_Size_Big)    return Pinwheel_Steps_Big;
  // else
  return Pinwheel_Steps_XL;
}
#endif



// 1D strip
uint16_t mAnimatorLight::Segment::virtualLength() const {
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    unsigned vW = virtualWidth();
    unsigned vH = virtualHeight();
    unsigned vLen;
    switch (map1D2D) {
      case M12_pBar:
        vLen = vH;
        break;
      case M12_pCorner:
        vLen = max(vW,vH); // get the longest dimension
        break;
      case M12_pArc:
        vLen = sqrt16(vH*vH + vW*vW); // use diagonal
        break;
      case M12_sPinwheel:
        vLen = getPinwheelLength(vW, vH);
        break;
      default:
        vLen = vW * vH; // use all pixels from segment
        break;
    }
    return vLen;
  }
#endif
  unsigned groupLen = groupLength(); // is always >= 1
  unsigned vLength = (length() + groupLen - 1) / groupLen;
  if (mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a single LED
  return vLength;
}

// void IRAM_ATTR mAnimatorLight::Segment::setPixelColor(int i, uint32_t col)
// {

//   if (!isActive()) return; // not active
// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   int vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows)
// #endif
//   i &= 0xFFFF;

//   if (i >= virtualLength() || i<0) return;  // if pixel would fall out of segment just exit

// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   if (is2D()) {
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     switch (map1D2D) {
//       case M12_Pixels:
//         // use all available pixels as a long strip
        
//         setPixelColorXY(i % vW, i / vW, col);
        
//         break;
//       case M12_pBar:
//         // expand 1D effect vertically or have it play on virtual strips
//         if (vStrip>0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
//         else          for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
//         break;
//       case M12_pArc:
//         // expand in circular fashion from center
//         if (i==0)
//           setPixelColorXY(0, 0, col);
//         else {
//           float step = HALF_PI / (2.85f*i);
//           for (float rad = 0.0f; rad <= HALF_PI+step/2; rad += step) {
//             // may want to try float version as well (with or without antialiasing)
//             int x = roundf(sin_t(rad) * i);
//             int y = roundf(cos_t(rad) * i);
//             setPixelColorXY(x, y, col);
//           }
//           // Bresenham’s Algorithm (may not fill every pixel)
//           //int d = 3 - (2*i);
//           //int y = i, x = 0;
//           //while (y >= x) {
//           //  setPixelColorXY(x, y, col);
//           //  setPixelColorXY(y, x, col);
//           //  x++;
//           //  if (d > 0) {
//           //    y--;
//           //    d += 4 * (x - y) + 10;
//           //  } else {
//           //    d += 4 * x + 6;
//           //  }
//           //}
//         }
//         break;
//       case M12_pCorner:
//         for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
//         for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
//         break;
//     }
//     return;
//   } else if (Segment::maxHeight!=1 && (width()==1 || height()==1)) {
//     if (start < Segment::maxWidth*Segment::maxHeight) {
//       // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
//       int x = 0, y = 0;
//       if (virtualHeight()>1) y = i;
//       if (virtualWidth() >1) x = i;
//       setPixelColorXY(x, y, col);
//       return;
//     }
//   }
// #endif

//   uint16_t len = length();
//   uint8_t _bri_t = currentBri();
//   if (_bri_t < 255) {
//     byte r = scale8(R(col), _bri_t);
//     byte g = scale8(G(col), _bri_t);
//     byte b = scale8(B(col), _bri_t);
//     byte w = scale8(W(col), _bri_t);
//     col = RGBW32(r, g, b, w);
//   }
  
//   // expand pixel (taking into account start, grouping, spacing [and offset])
//   i = i * groupLength();
//   if (reverse) { // is segment reversed?
//     if (mirror) { // is segment mirrored?
//       i = (len - 1) / 2 - i;  //only need to index half the pixels
//     } else {
//       i = (len - 1) - i;
//     }
//   }
//   i += start; // starting pixel in a group

//   uint32_t tmpCol = col;
//   // set all the pixels in the group
//   for (int j = 0; j < grouping_get(); j++) {
//     uint16_t indexSet = i + ((reverse) ? -j : j);
//     if (indexSet >= start && indexSet < stop) {
//       if (mirror) { //set the corresponding mirrored pixel
//         uint16_t indexMir = stop - indexSet + start - 1;
//         indexMir += offset; // offset/phase
//         if (indexMir >= stop) indexMir -= len; // wrap
// // #ifndef WLED_DISABLE_MODE_BLEND
// //         if (_modeBlend) tmpCol = color_blend( getPixelColor(indexMir), col, 0xFFFFU - progress(), true);
// // #endif
//         tkr_anim->setPixelColor(indexMir, tmpCol);
//       }
//       indexSet += offset; // offset/phase
//       if (indexSet >= stop) indexSet -= len; // wrap
// // #ifndef WLED_DISABLE_MODE_BLEND
// //       if (_modeBlend) tmpCol = color_blend(tkr_anim->getPixelColor(indexSet), col, 0xFFFFU - progress(), true);
// // #endif
//       tkr_anim->setPixelColor(indexSet, tmpCol);
//     }
//   }

// }

#ifdef USE_AA_PIXELS
// anti-aliased normalized version of setPixelColor()
void mAnimatorLight::Segment::setPixelColor(float i, uint32_t col, bool aa)
{

  Serial.println(F("setPixelColor(float i, uint32_t col, bool aa)"));

  if (!isActive()) return; // not active
  int vStrip = int(i/10.0f); // hack to allow running on virtual strips (2D segment columns/rows)
  i -= int(i);

  if (i<0.0f || i>1.0f) return; // not normalized

  float fC = i * (virtualLength()-1);
  if (aa) {
    uint16_t iL = roundf(fC-0.49f);
    uint16_t iR = roundf(fC+0.49f);
    float    dL = (fC - iL)*(fC - iL);
    float    dR = (iR - fC)*(iR - fC);
    ColourBaseType cIL = getPixelColor(iL | (vStrip<<16));
    ColourBaseType cIR = getPixelColor(iR | (vStrip<<16));
    if (iR!=iL) {
      // blend L pixel
      cIL = color_blend(col, cIL, uint8_t(dL*255.0f));
      setPixelColor(iL | (vStrip<<16), cIL);
      // blend R pixel
      cIR = color_blend(col, cIR, uint8_t(dR*255.0f));
      setPixelColor(iR | (vStrip<<16), cIR);
    } else {
      // exact match (x & y land on a pixel)
      setPixelColor(iL | (vStrip<<16), col);
    }
  } else {
    setPixelColor(uint16_t(roundf(fC)) | (vStrip<<16), col);
  }

}
#endif

// uint32_t mAnimatorLight::Segment::getPixelColor(int i)
// {

//   if (!isActive()) return 0; // not active
// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   int vStrip = i>>16;
// #endif
//   i &= 0xFFFF;

// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   if (is2D()) {
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     switch (map1D2D) {
//       case M12_Pixels:
//         return getPixelColorXY(i % vW, i / vW);
//         break;
//       case M12_pBar:
//         if (vStrip>0) return getPixelColorXY(vStrip - 1, vH - i -1);
//         else          return getPixelColorXY(0, vH - i -1);
//         break;
//       case M12_pArc:
//       case M12_pCorner:
//         // use longest dimension
//         return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
//         break;
//     }
//     return 0;
//   }
// #endif

//   if (reverse) i = virtualLength() - i - 1;
//   i *= groupLength();
//   i += start;
//   /* offset/phase */
//   i += offset;
//   if ((i >= stop) && (stop>0)) i -= length(); // avoids negative pixel index (stop = 0 is a possible value)
//   // return tkr_anim->BUS_getPixelColor(i);
//   return tkr_anim->getPixelColor(i);
// }




/**
 * @brief Since we cant do overloading via the return, we need a special case for RGBWW
 * 
 * @param i 
 * @return RgbwwColor 
 */
RgbwwColor IRAM_ATTR mAnimatorLight::Segment::getPixelColorRgbww(int i) const
{
  
  // if (!isActive()) return 0; // not active

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
    const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
    switch (map1D2D) {
      case M12_Pixels:
        return getPixelColorXY(i % vW, i / vW);
        break;
      case M12_pBar: {
        int vStrip = i>>16; // virtual strips are only relevant in Bar expansion mode
        if (vStrip > 0) return getPixelColorXY(vStrip - 1, vH - (i & 0xFFFF) -1);
        else            return getPixelColorXY(0, vH - i -1);
        break; }
      case M12_pArc:
        if (i >= vW && i >= vH) {
          unsigned vI = sqrt16(i*i/2);
          return getPixelColorXY(vI,vI); // use diagonal
        }
      case M12_pCorner:
        // use longest dimension
        return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
        break;
      case M12_sPinwheel:
        // not 100% accurate, returns pixel at outer edge
        // i = angle --> 0 - 296  (Big), 0 - 192  (Medium), 0 - 72 (Small)
        float centerX = roundf((vW-1) / 2.0f);
        float centerY = roundf((vH-1) / 2.0f);
        float angleRad = getPinwheelAngle(i, vW, vH); // angle in radians
        float cosVal = cos_t(angleRad);
        float sinVal = sin_t(angleRad);

        int posx = (centerX + 0.5f * cosVal) * Fixed_Scale; // X starting position in fixed point 18 bit
        int posy = (centerY + 0.5f * sinVal) * Fixed_Scale; // Y starting position in fixed point 18 bit
        int inc_x = cosVal * Fixed_Scale; // X increment per step (fixed point) 10 bit
        int inc_y = sinVal * Fixed_Scale; // Y increment per step (fixed point) 10 bit
        int32_t maxX = vW * Fixed_Scale; // X edge in fixedpoint
        int32_t maxY = vH * Fixed_Scale; // Y edge in fixedpoint

        // trace ray from center until we hit any edge - to avoid rounding problems, we use the same method as in setPixelColor
        int x = INT_MIN;
        int y = INT_MIN;
        while ((posx >= 0) && (posy >= 0) && (posx < maxX)  && (posy < maxY))  {
          // scale down to integer (compiler will replace division with appropriate bitshift)
          x = posx / Fixed_Scale;
          y = posy / Fixed_Scale;
          // advance to next position
          posx += inc_x;
          posy += inc_y;
        }
        return getPixelColorXY(x, y);
        break;
      }
    return 0;
  }
  #endif

  if (reverse) i = vLength() - i - 1;
  i *= groupLength();
  i += start;
  // offset/phase
  i += offset;
  if (i >= stop) i -= length();
  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  return tkr_anim->getPixelColor(i);
  #else
  RgbwwColor rgbww = tkr_anim->getPixelColor(i);
  return RGBW32(rgbww.R, rgbww.G, rgbww.B, rgbww.WW);
  #endif
}
uint32_t IRAM_ATTR mAnimatorLight::Segment::getPixelColor(int i) const
{
  if (!isActive()) return 0; // not active

  // Serial.println("Should not be here");

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
    const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
    // ALOG_INF(PSTR("map %d"), map1D2D);
    switch (map1D2D) {
      case M12_Pixels:
        return getPixelColorXY(i % vW, i / vW);
        break;
      case M12_pBar: {
        int vStrip = i>>16; // virtual strips are only relevant in Bar expansion mode
        if (vStrip > 0) return getPixelColorXY(vStrip - 1, vH - (i & 0xFFFF) -1);
        else            return getPixelColorXY(0, vH - i -1);
        break; }
      case M12_pArc:
        if (i >= vW && i >= vH) {
          unsigned vI = sqrt16(i*i/2);
          return getPixelColorXY(vI,vI); // use diagonal
        }
      case M12_pCorner:
        // use longest dimension
        return vW>vH ? getPixelColorXY(i, 0) : getPixelColorXY(0, i);
        break;
      case M12_sPinwheel:
        // not 100% accurate, returns pixel at outer edge
        // i = angle --> 0 - 296  (Big), 0 - 192  (Medium), 0 - 72 (Small)
        float centerX = roundf((vW-1) / 2.0f);
        float centerY = roundf((vH-1) / 2.0f);
        float angleRad = getPinwheelAngle(i, vW, vH); // angle in radians
        float cosVal = cos_t(angleRad);
        float sinVal = sin_t(angleRad);

        int posx = (centerX + 0.5f * cosVal) * Fixed_Scale; // X starting position in fixed point 18 bit
        int posy = (centerY + 0.5f * sinVal) * Fixed_Scale; // Y starting position in fixed point 18 bit
        int inc_x = cosVal * Fixed_Scale; // X increment per step (fixed point) 10 bit
        int inc_y = sinVal * Fixed_Scale; // Y increment per step (fixed point) 10 bit
        int32_t maxX = vW * Fixed_Scale; // X edge in fixedpoint
        int32_t maxY = vH * Fixed_Scale; // Y edge in fixedpoint

        // trace ray from center until we hit any edge - to avoid rounding problems, we use the same method as in setPixelColor
        int x = INT_MIN;
        int y = INT_MIN;
        while ((posx >= 0) && (posy >= 0) && (posx < maxX)  && (posy < maxY))  {
          // scale down to integer (compiler will replace division with appropriate bitshift)
          x = posx / Fixed_Scale;
          y = posy / Fixed_Scale;
          // advance to next position
          posx += inc_x;
          posy += inc_y;
        }
        return getPixelColorXY(x, y);
        break;
      }
    return 0;
  }
  #endif

  if (reverse) i = vLength() - i - 1;
  i *= groupLength();
  i += start;
  // offset/phase
  i += offset;
  if (i >= stop) i -= length();
  
  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  RgbwwColor rgbww = tkr_anim->getPixelColor(i);
  // SERIAL_DEBUG_COLRGBWWi("getAni", rgbww, i);
  // rgbww = RgbwwColor(10,11,12,13,14);
  return RGBW32(rgbww.R, rgbww.G, rgbww.B, rgbww.WW);
  #else
  return tkr_anim->getPixelColor(i);
  #endif
}


/**
 * @brief 
 * 
 * @param indexPixel 
 * @return RgbcctColor 
 */
// RgbcctColor IRAM_ATTR mAnimatorLight::Segment::getPixelColor(uint16_t indexPixel)
// {
  
//   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
//   performance.bus_read_single_us = micros();
//   #endif

//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   int vStrip = indexPixel>>16;
//   indexPixel &= 0xFFFF;
//   if (is2D()) {DEBUG_LINE_HERE3
//     uint16_t vH = virtualHeight();  // segment height in logical pixels
//     uint16_t vW = virtualWidth();
//     // ALOG_ERR(PSTR("\n\rmap1D2D %d set"), map1D2D);
//     switch (map1D2D) {
//       case M12_Pixels:
//         return RgbcctColor( getPixelColorXY(indexPixel % vW, indexPixel / vW) );
//         break;
//       case M12_pBar:DEBUG_LINE_HERE3
//         if (vStrip>0) return RgbcctColor(getPixelColorXY(vStrip - 1, vH - indexPixel -1));
//         else          return RgbcctColor(getPixelColorXY(0, vH - indexPixel -1));
//         break;
//       case M12_pArc:DEBUG_LINE_HERE3
//       case M12_pCorner:DEBUG_LINE_HERE3
//         // use longest dimension
//         return vW>vH ? RgbcctColor(getPixelColorXY(indexPixel, 0)) : RgbcctColor(getPixelColorXY(0, indexPixel));
//         break;
//     }DEBUG_LINE_HERE3
//     ALOG_ERR(PSTR("No map1D2D set"));
//     return RgbcctColor((uint32_t)0);
//   }
//   #endif

//   if(reverse) indexPixel = virtualLength() - indexPixel - 1;
//   indexPixel *= groupLength();
//   indexPixel += start + offset;
//   if (indexPixel >= stop){ indexPixel -= length(); } // Adjust if beyond stop

//   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//     RgbcctColor colour_hardware = tkr_anim->getPixelColor_Rgbcct(indexPixel);
//     ALOG_DBG(PSTR("colour_hardware[%d] = %d,%d,%d,%d,%d"), 
//       indexPixel, 
//       colour_hardware.R, colour_hardware.G, colour_hardware.B, 
//       colour_hardware.W1, colour_hardware.W2);
//     return colour_hardware;
//   #else    
//     #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
//       // If enabled, intermediate value needs stored before return. Can be removed for performance without
//       RgbcctColor c = tkr_anim->getPixelColor_Rgbcct(indexPixel);
//       performance.bus_read_single_us = micros() - performance.bus_read_single_us;
//       return c;
//     #else
//       return tkr_anim->getPixelColor_Rgbcct(indexPixel);
//     #endif
//   #endif

// }

// uint8_t mAnimatorLight::Segment::differs(mAnimatorLight::Segment& b) const {
//   // uint8_t d = 0;
//   // if (start != b.start)         d |= SEG_DIFFERS_BOUNDS;
//   // if (stop != b.stop)           d |= SEG_DIFFERS_BOUNDS;
//   // if (offset != b.offset)       d |= SEG_DIFFERS_GSO;
//   // if (grouping != b.grouping)   d |= SEG_DIFFERS_GSO;
//   // if (spacing != b.spacing)     d |= SEG_DIFFERS_GSO;
//   // if (opacity != b.opacity)     d |= SEG_DIFFERS_BRI;
//   // if (mode != b.mode)           d |= SEG_DIFFERS_FX;
//   // if (speed != b.speed)         d |= SEG_DIFFERS_FX;
//   // if (intensity != b.intensity) d |= SEG_DIFFERS_FX;
//   // if (palette != b.palette)     d |= SEG_DIFFERS_FX;
//   // if (custom1 != b.custom1)     d |= SEG_DIFFERS_FX;
//   // if (custom2 != b.custom2)     d |= SEG_DIFFERS_FX;
//   // if (custom3 != b.custom3)     d |= SEG_DIFFERS_FX;
//   // if (startY != b.startY)       d |= SEG_DIFFERS_BOUNDS;
//   // if (stopY != b.stopY)         d |= SEG_DIFFERS_BOUNDS;

//   // //bit pattern: (msb first) sound:3, mapping:3, transposed, mirrorY, reverseY, [transitional, reset,] paused, mirrored, on, reverse, [selected]
//   // if ((options & 0b1111111110011110U) != (b.options & 0b1111111110011110U)) d |= SEG_DIFFERS_OPT;
//   // if ((options & 0x0001U) != (b.options & 0x0001U))                         d |= SEG_DIFFERS_SEL;
//   // for (uint8_t i = 0; i < NUM_COLORS; i++) if (colors[i] != b.colors[i])    d |= SEG_DIFFERS_COL;

//   // return d;
// }

// void mAnimatorLight::Segment::refreshLightCapabilities() {
//   // uint8_t capabilities = 0x01;

//   // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
//   //   Bus *bus = busses.getBus(b);
//   //   if (bus == nullptr || bus->getLength()==0) break;
//   //   if (!bus->isOk()) continue;
//   //   if (bus->getStart() >= stop) continue;
//   //   if (bus->getStart() + bus->getLength() <= start) continue;

//   //   uint8_t type = bus->getType();
//   //   if (type == TYPE_ANALOG_1CH || (!cctFromRgb && type == TYPE_ANALOG_2CH)) capabilities &= 0xFE; // does not support RGB
//   //   if (bus->isRgbw()) capabilities |= 0x02; // segment supports white channel
//   //   if (!cctFromRgb) {
//   //     switch (type) {
//   //       case TYPE_ANALOG_5CH:
//   //       case TYPE_ANALOG_2CH:
//   //         capabilities |= 0x04; //segment supports white CCT 
//   //     }
//   //   }
//   //   if (correctWB && type != TYPE_ANALOG_1CH) capabilities |= 0x04; //white balance correction (uses CCT slider)
//   //   uint8_t aWM = Bus::getAutoWhiteMode()<255 ? Bus::getAutoWhiteMode() : bus->getAWMode();
//   //   bool whiteSlider = (aWM == RGBW_MODE_DUAL || aWM == RGBW_MODE_MANUAL_ONLY); // white slider allowed
//   //   if (bus->isRgbw() && (whiteSlider || !(capabilities & 0x01))) capabilities |= 0x08; // allow white channel adjustments (AWM allows or is not RGB)
//   // }
//   // _capabilities = capabilities;
// }

/*
 * Fills segment with color
 */
// void mAnimatorLight::Segment::fill(uint32_t c) 
// {
//   const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
//   const uint16_t rows = virtualHeight(); // will be 1 for 1D
//   for(uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) 
//   {
//     if (is2D())
//     {
//       setPixelColorXY(x, y, c);
//     }
//     else
//     {
//       setPixelColor(x, c);
//     }        
//   }  
// }

// Blends the specified color with the existing pixel color.
void mAnimatorLight::Segment::blendPixelColor(int n, uint32_t color, uint8_t blend) {
  setPixelColor(n, color_blend(getPixelColor(n), color, blend));
}

// Adds the specified color with the existing pixel color perserving color balance.
void mAnimatorLight::Segment::addPixelColor(int n, uint32_t color) {
  setPixelColor(n, color_add(getPixelColor(n), color));
}


void mAnimatorLight::Segment::fadePixelColor(uint16_t n, uint8_t fade) {
#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  RgbwwColor col = getPixelColor(n);
  uint32_t col32 = RGBW32(col.R, col.G, col.B, col.WW);
  CRGB pix = CRGB(col32).nscale8_video(fade);
#else
  CRGB pix = CRGB(getPixelColor(n)).nscale8_video(fade);
#endif
  setPixelColor(n, pix);
}

/*
 * fade out function, higher rate = quicker fade
 */

// void mAnimatorLight::Segment::fade_out(uint8_t rate) {
//   if (!isActive()) return; // not active
//   const int cols = is2D() ? vWidth() : vLength();
//   const int rows = vHeight(); // will be 1 for 1D

//   rate = (255-rate) >> 1;
//   float mappedRate = 1.0f / (float(rate) + 1.1f);

//   uint32_t color = segcol[1].WithBrightness().getU32();//colors[1]; // SEGCOLOR(1); // target color
//   uint32_t color_check = color;
//   int w2 = W(color);
//   int r2 = R(color);
//   int g2 = G(color);
//   int b2 = B(color);

//   for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
//     color = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
//     if (color == color_check) continue; // already at target color
//     int w1 = W(color);
//     int r1 = R(color);
//     int g1 = G(color);
//     int b1 = B(color);

//     int wdelta = (w2 - w1) * mappedRate;
//     int rdelta = (r2 - r1) * mappedRate;
//     int gdelta = (g2 - g1) * mappedRate;
//     int bdelta = (b2 - b1) * mappedRate;

//     // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
//     wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
//     rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
//     gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
//     bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

//     if (is2D()) setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
//     else        setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
//   }
// }
// void mAnimatorLight::Segment::fade_out(uint8_t rate) {
//   DEBUG_LINE_HERE
//   const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
//   const uint16_t rows = virtualHeight(); // will be 1 for 1D
// DEBUG_LINE_HERE
//   // ALOG_INF(PSTR("Segment::maxHeight3 %d\n\r"), Segment::maxHeight);

//   // ALOG_INF(PSTR("fade_out(%d)"), rate);
//   // ALOG_INF(PSTR("cols=%d rows=%d"), cols, rows);

//   // 
//   rate = (255-rate) >> 1;
//   float mappedRate = float(rate) +1.1;

//   // 
//   DEBUG_LINE_HERE
//   uint32_t color = segcol[1].WithBrightness().getU32();
//   DEBUG_LINE_HERE
  
//   // RgbcctColor::GetU32ColourBrightnessApplied(segcol[1]);   //tkr_anim->SEGCOLOR_RGBCCT(1).G; //  tkr_anim->segments[0].segcol[1].getU32(); // getPixelColor(0);
  
//   // 
//   int w2 = W(color);
//   int r2 = R(color);
//   int g2 = G(color);
//   int b2 = B(color);

//   // 
//   for (int y = 0; y < rows; y++){
//   // 
//     for (int x = 0; x < cols; x++) {
//   // 

//     //  ALOG_INF(PSTR("is2D() %d"), is2D() );

//   // 
//   DEBUG_LINE_HERE
//       color = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
//       DEBUG_LINE_HERE
//   // 
//       int w1 = W(color);
//       int r1 = R(color);
//       int g1 = G(color);
//       int b1 = B(color);

//       int wdelta = (w2 - w1) / mappedRate;
//       int rdelta = (r2 - r1) / mappedRate;
//       int gdelta = (g2 - g1) / mappedRate;
//       int bdelta = (b2 - b1) / mappedRate;
// DEBUG_LINE_HERE
//       // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
//       wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
//       rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
//       gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
//       bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

//   // 
//   DEBUG_LINE_HERE
//       if (is2D())
//       {
//   // 
//   DEBUG_LINE_HERE
//         setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
//         DEBUG_LINE_HERE
//   // 
//       }
//       else{
//   // 
//   DEBUG_LINE_HERE
//         setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
//         DEBUG_LINE_HERE
//   // 
//       }   
//       DEBUG_LINE_HERE     
//   // 
//     }
//   // 
//   }
//   // 
// }

// // fades all pixels to black using nscale8()
// void mAnimatorLight::Segment::fadeToBlackBy(uint8_t fadeBy) {

  
//   if (!isActive() || fadeBy == 0) return;   // optimization - no scaling to apply
//   const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
//   const uint16_t rows = virtualHeight(); // will be 1 for 1D

//   for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
//     if (is2D()) setPixelColorXY(x, y, color_fade(getPixelColorXY(x,y), 255-fadeBy));
//     else        setPixelColor(x, color_fade(getPixelColor(x), 255-fadeBy));
//   }



//   // const uint16_t cols = is2D() ? virtualWidth() : virtualLength();
//   // const uint16_t rows = virtualHeight(); // will be 1 for 1D

//   // for (uint16_t y = 0; y < rows; y++) for (uint16_t x = 0; x < cols; x++) {
//   //   if (is2D()) setPixelColorXY(x, y, CRGB(getPixelColorXY(x,y)).nscale8(255-fadeBy));
//   //   else        setPixelColor(x, CRGB(getPixelColor(x)).nscale8(255-fadeBy));
//   // }
// }




uint8_t mAnimatorLight::Segment::differs(const Segment& b) const {
  uint8_t d = 0;
  if (start != b.start)         d |= SEG_DIFFERS_BOUNDS;
  if (stop != b.stop)           d |= SEG_DIFFERS_BOUNDS;
  if (offset != b.offset)       d |= SEG_DIFFERS_GSO;
  if (grouping != b.grouping)   d |= SEG_DIFFERS_GSO;
  if (spacing != b.spacing)     d |= SEG_DIFFERS_GSO;
  // if (opacity != b.opacity)     d |= SEG_DIFFERS_BRI;
  // if (mode != b.mode)           d |= SEG_DIFFERS_FX;
  if (speed != b.speed)         d |= SEG_DIFFERS_FX;
  if (intensity != b.intensity) d |= SEG_DIFFERS_FX;
  // if (palette != b.palette)     d |= SEG_DIFFERS_FX;
  if (custom1 != b.custom1)     d |= SEG_DIFFERS_FX;
  if (custom2 != b.custom2)     d |= SEG_DIFFERS_FX;
  if (custom3 != b.custom3)     d |= SEG_DIFFERS_FX;
  if (startY != b.startY)       d |= SEG_DIFFERS_BOUNDS;
  if (stopY != b.stopY)         d |= SEG_DIFFERS_BOUNDS;

  //bit pattern: (msb first)
  // set:2, sound:2, mapping:3, transposed, mirrorY, reverseY, [reset,] paused, mirrored, on, reverse, [selected]
  if ((options & 0b1111111111011110U) != (b.options & 0b1111111111011110U)) d |= SEG_DIFFERS_OPT;
  if ((options & 0x0001U) != (b.options & 0x0001U))                         d |= SEG_DIFFERS_SEL;
  // for (unsigned i = 0; i < NUM_COLORS; i++) if (colors[i] != b.colors[i])   d |= SEG_DIFFERS_COL;

  return d;
}

void mAnimatorLight::Segment::refreshLightCapabilities() {
  unsigned capabilities = 0;
  unsigned segStartIdx = 0xFFFFU;
  unsigned segStopIdx  = 0;

  if (!isActive()) {
    _capabilities = 0;
    return;
  }

  if (start < Segment::maxWidth * Segment::maxHeight) {
    // we are withing 2D matrix (includes 1D segments)
    for (int y = startY; y < stopY; y++) for (int x = start; x < stop; x++) {
      unsigned index = tkr_anim->getMappedPixelIndex(x + Segment::maxWidth * y); // convert logical address to physical
      if (index < 0xFFFFU) {
        if (segStartIdx > index) segStartIdx = index;
        if (segStopIdx  < index) segStopIdx  = index;
      }
      if (segStartIdx == segStopIdx) segStopIdx++; // we only have 1 pixel segment
    }
  } else {
    // we are on the strip located after the matrix
    segStartIdx = start;
    segStopIdx  = stop;
  }

  for (unsigned b = 0; b < BusManager::getNumBusses(); b++) {
    Bus *bus = pCONT_iLight->bus_manager->getBus(b);
    if (bus == nullptr || bus->getLength()==0) break;
    if (!bus->isOk()) continue;
    if (bus->getStart() >= segStopIdx) continue;
    if (bus->getStart() + bus->getLength() <= segStartIdx) continue;

    if (bus->hasRGB() || (tkr_anim->cctFromRgb && bus->hasCCT())) capabilities |= SEG_CAPABILITY_RGB;
    if (!tkr_anim->cctFromRgb && bus->hasCCT())                   capabilities |= SEG_CAPABILITY_CCT;
    if (tkr_anim->correctWB && (bus->hasRGB() || bus->hasCCT()))  capabilities |= SEG_CAPABILITY_CCT; //white balance correction (CCT slider)
    if (bus->hasWhite()) {
      unsigned aWM = Bus::getGlobalAWMode() == AW_GLOBAL_DISABLED ? bus->getAutoWhiteMode() : Bus::getGlobalAWMode();
      bool whiteSlider = (aWM == RGBW_MODE_DUAL || aWM == RGBW_MODE_MANUAL_ONLY); // white slider allowed
      // if auto white calculation from RGB is active (Accurate/Brighter), force RGB controls even if there are no RGB busses
      if (!whiteSlider) capabilities |= SEG_CAPABILITY_RGB;
      // if auto white calculation from RGB is disabled/optional (None/Dual), allow white channel adjustments
      if ( whiteSlider) capabilities |= SEG_CAPABILITY_W;
    }
  }
  _capabilities = capabilities;
}

/*
 * Fills segment with color
 */
void mAnimatorLight::Segment::fill(uint32_t c) {
  if (!isActive()) return; // not active
  const int cols = is2D() ? vWidth() : vLength();
  const int rows = vHeight(); // will be 1 for 1D
  // pre-scale color for all pixels
  c = color_fade(c, _brightness_rgb);//_segBri);
  _colorScaled = true;
  for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
    if (is2D()) setPixelColorXY(x, y, c);
    else        setPixelColor(x, c);
  }
  _colorScaled = false;
}

/*
 * fade out function, higher rate = quicker fade
 */
#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
void mAnimatorLight::Segment::fade_out(uint8_t rate) {
  if (!isActive()) return; // not active
  const int cols = is2D() ? vWidth() : vLength();
  const int rows = vHeight(); // will be 1 for 1D

  rate = (255-rate) >> 1;
  float mappedRate = 1.0f / (float(rate) + 1.1f);

  RgbwwColor targetColor = segcol[1].WithBrightness(); // SEGCOLOR(1); // target color
  RgbwwColor currentColor;
  int w1, r1, g1, b1, w2, r2, g2, b2;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      currentColor = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
      if (currentColor == targetColor) continue; // already at target color

      w1 = currentColor.WW;
      r1 = currentColor.R;
      g1 = currentColor.G;
      b1 = currentColor.B;

      w2 = targetColor.WW;
      r2 = targetColor.R;
      g2 = targetColor.G;
      b2 = targetColor.B;

      int wdelta = (w2 - w1) * mappedRate;
      int rdelta = (r2 - r1) * mappedRate;
      int gdelta = (g2 - g1) * mappedRate;
      int bdelta = (b2 - b1) * mappedRate;

      // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
      wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
      rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
      gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
      bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

      if (is2D()) setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
      else        setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
    }
  }
}
#else
void mAnimatorLight::Segment::fade_out(uint8_t rate) {
  if (!isActive()) return; // not active
  const int cols = is2D() ? vWidth() : vLength();
  const int rows = vHeight(); // will be 1 for 1D

  rate = (255-rate) >> 1;
  float mappedRate = 1.0f / (float(rate) + 1.1f);

  RgbwwColor color1 = segcol[1].WithBrightness();//.getU32(); // SEGCOLOR(1); // target color
  uint32_t color = RGBW32(color1.R,color1.G,color1.B,color1.WW);//segcol[1].WithBrightness();//.getU32(); // SEGCOLOR(1); // target color
  uint32_t color_check = color;
  int w2 = W(color);
  int r2 = R(color);
  int g2 = G(color);
  int b2 = B(color);

  for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
    color = is2D() ? getPixelColorXY(x, y) : getPixelColor(x);
    if (color == color_check) continue; // already at target color
    int w1 = W(color);
    int r1 = R(color);
    int g1 = G(color);
    int b1 = B(color);

    int wdelta = (w2 - w1) * mappedRate;
    int rdelta = (r2 - r1) * mappedRate;
    int gdelta = (g2 - g1) * mappedRate;
    int bdelta = (b2 - b1) * mappedRate;

    // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
    wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
    rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
    gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
    bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

    if (is2D()) setPixelColorXY(x, y, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
    else        setPixelColor(x, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  }
}
#endif

// fades all pixels to black using nscale8()
void mAnimatorLight::Segment::fadeToBlackBy(uint8_t fadeBy) {
  if (!isActive() || fadeBy == 0) return;   // optimization - no scaling to apply
  const int cols = is2D() ? vWidth() : vLength();
  const int rows = vHeight(); // will be 1 for 1D

  for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) {
    if (is2D()) setPixelColorXY(x, y, color_fade(getPixelColorXY(x,y), 255-fadeBy));
    else        setPixelColor(x, color_fade(getPixelColor(x), 255-fadeBy));
  }
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 * Note: for blur_amount > 215 this function does not work properly (creates alternating pattern)
 */
void mAnimatorLight::Segment::blur(uint8_t blur_amount, bool smear) {
  if (!isActive() || blur_amount == 0) return; // optimization: 0 means "don't blur"
#ifndef WLED_DISABLE_2D
  if (is2D()) {
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    // compatibility with 2D
    blur2D(blur_amount, blur_amount, smear); // symmetrical 2D blur
    //box_blur(map(blur_amount,1,255,1,3), smear);
    #endif
    return;
  }
#endif
  uint8_t keep = smear ? 255 : 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  unsigned vlength = vLength();
  uint32_t carryover = BLACK;
  uint32_t lastnew;
  uint32_t last;
  uint32_t curnew = BLACK;
  for (unsigned i = 0; i < vlength; i++) {
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
      RgbwwColor crgbww = getPixelColor(i);
      uint32_t cur = RGBW32(crgbww.R, crgbww.G, crgbww.B, crgbww.WW);
    #else
      uint32_t cur = getPixelColor(i);
    #endif
    uint32_t part = color_fade(cur, seep);
    curnew = color_fade(cur, keep);
    if (i > 0) {
      if (carryover) curnew = color_add(curnew, carryover);
      uint32_t prev = color_add(lastnew, part);
      // optimization: only set pixel if color has changed
      if (last != prev) setPixelColor((int)i - 1, prev);
    } else setPixelColor((int)i, curnew); // first pixel
    lastnew = curnew;
    last = cur; // save original value for comparison on next iteration
    carryover = part;
  }
  setPixelColor((int)vlength - 1, curnew);
}

// /*
//  * Color conversion & utility methods
//  */

// /*
//  * color blend function
//  */
// uint32_t mAnimatorLight::Segment::color_blend(uint32_t color1, uint32_t color2, uint16_t blend, bool b16) {
//   if(blend == 0)   return color1;
//   uint16_t blendmax = b16 ? 0xFFFF : 0xFF;
//   if(blend == blendmax) return color2;
//   uint8_t shift = b16 ? 16 : 8;

//   uint32_t w1 = W(color1);
//   uint32_t r1 = R(color1);
//   uint32_t g1 = G(color1);
//   uint32_t b1 = B(color1);

//   uint32_t w2 = W(color2);
//   uint32_t r2 = R(color2);
//   uint32_t g2 = G(color2);
//   uint32_t b2 = B(color2);

//   uint32_t w3 = ((w2 * blend) + (w1 * (blendmax - blend))) >> shift;
//   uint32_t r3 = ((r2 * blend) + (r1 * (blendmax - blend))) >> shift;
//   uint32_t g3 = ((g2 * blend) + (g1 * (blendmax - blend))) >> shift; 
//   uint32_t b3 = ((b2 * blend) + (b1 * (blendmax - blend))) >> shift;

//   return RGBW32(r3, g3, b3, w3);
// }

// /*
//  * color add function that preserves ratio
//  * idea: https://github.com/Aircoookie/WLED/pull/2465 by https://github.com/Proto-molecule
//  */
// uint32_t mAnimatorLight::Segment::color_add(uint32_t c1, uint32_t c2)
// {
//   uint32_t r = R(c1) + R(c2);
//   uint32_t g = G(c1) + G(c2);
//   uint32_t b = B(c1) + B(c2);
//   uint32_t w = W(c1) + W(c2);
//   uint16_t max = r;
//   if (g > max) max = g;
//   if (b > max) max = b;
//   if (w > max) max = w;
//   if (max < 256) return RGBW32(r, g, b, w);
//   else           return RGBW32(r * 255 / max, g * 255 / max, b * 255 / max, w * 255 / max);
// }

void mAnimatorLight::Segment::setRandomColor(byte* rgb)
{
  // lastRandomIndex = tkr_anim->getMainSegment().get_random_wheel_index(lastRandomIndex);
  // colorHStoRGB(lastRandomIndex*256,255,rgb);
}

void mAnimatorLight::Segment::colorHStoRGB(uint16_t hue, byte sat, byte* rgb) //hue, sat to rgb
{
  float h = ((float)hue)/65535.0f;
  float s = ((float)sat)/255.0f;
  int   i = floorf(h*6);
  float f = h * 6.0f - i;
  int   p = int(255.0f * (1.0f-s));
  int   q = int(255.0f * (1.0f-f*s));
  int   t = int(255.0f * (1.0f-(1.0f-f)*s));
  p = constrain(p, 0, 255);
  q = constrain(q, 0, 255);
  t = constrain(t, 0, 255);
  switch (i%6) {
    case 0: rgb[0]=255,rgb[1]=t,  rgb[2]=p;  break;
    case 1: rgb[0]=q,  rgb[1]=255,rgb[2]=p;  break;
    case 2: rgb[0]=p,  rgb[1]=255,rgb[2]=t;  break;
    case 3: rgb[0]=p,  rgb[1]=q,  rgb[2]=255;break;
    case 4: rgb[0]=t,  rgb[1]=p,  rgb[2]=255;break;
    case 5: rgb[0]=255,rgb[1]=p,  rgb[2]=q;  break;
  }
}

//get RGB values from color temperature in K (https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html)
void mAnimatorLight::Segment::colorKtoRGB(uint16_t kelvin, byte* rgb) //white spectrum to rgb, calc
{
  int r = 0, g = 0, b = 0;
  float temp = kelvin / 100.0f;
  if (temp <= 66.0f) {
    r = 255;
    g = roundf(99.4708025861f * logf(temp) - 161.1195681661f);
    if (temp <= 19.0f) {
      b = 0;
    } else {
      b = roundf(138.5177312231f * logf((temp - 10.0f)) - 305.0447927307f);
    }
  } else {
    r = roundf(329.698727446f * powf((temp - 60.0f), -0.1332047592f));
    g = roundf(288.1221695283f * powf((temp - 60.0f), -0.0755148492f));
    b = 255;
  }
  //g += 12; //mod by Aircoookie, a bit less accurate but visibly less pinkish
  rgb[0] = (uint8_t) constrain(r, 0, 255);
  rgb[1] = (uint8_t) constrain(g, 0, 255);
  rgb[2] = (uint8_t) constrain(b, 0, 255);
  rgb[3] = 0;
}

void mAnimatorLight::Segment::colorCTtoRGB(uint16_t mired, byte* rgb) //white spectrum to rgb, bins
{
  //this is only an approximation using WS2812B with gamma correction enabled
  if (mired > 475) {
    rgb[0]=255;rgb[1]=199;rgb[2]=92;//500
  } else if (mired > 425) {
    rgb[0]=255;rgb[1]=213;rgb[2]=118;//450
  } else if (mired > 375) {
    rgb[0]=255;rgb[1]=216;rgb[2]=118;//400
  } else if (mired > 325) {
    rgb[0]=255;rgb[1]=234;rgb[2]=140;//350
  } else if (mired > 275) {
    rgb[0]=255;rgb[1]=243;rgb[2]=160;//300
  } else if (mired > 225) {
    rgb[0]=250;rgb[1]=255;rgb[2]=188;//250
  } else if (mired > 175) {
    rgb[0]=247;rgb[1]=255;rgb[2]=215;//200
  } else {
    rgb[0]=237;rgb[1]=255;rgb[2]=239;//150
  }
}

#ifndef WLED_DISABLE_HUESYNC
void mAnimatorLight::Segment::colorXYtoRGB(float x, float y, byte* rgb) //coordinates to rgb (https://www.developers.meethue.com/documentation/color-conversions-rgb-xy)
{
  float z = 1.0f - x - y;
  float X = (1.0f / y) * x;
  float Z = (1.0f / y) * z;
  float r = (int)255*(X * 1.656492f - 0.354851f - Z * 0.255038f);
  float g = (int)255*(-X * 0.707196f + 1.655397f + Z * 0.036152f);
  float b = (int)255*(X * 0.051713f - 0.121364f + Z * 1.011530f);
  if (r > b && r > g && r > 1.0f) {
    // red is too big
    g = g / r;
    b = b / r;
    r = 1.0f;
  } else if (g > b && g > r && g > 1.0f) {
    // green is too big
    r = r / g;
    b = b / g;
    g = 1.0f;
  } else if (b > r && b > g && b > 1.0f) {
    // blue is too big
    r = r / b;
    g = g / b;
    b = 1.0f;
  }
  // Apply gamma correction
  r = r <= 0.0031308f ? 12.92f * r : (1.0f + 0.055f) * powf(r, (1.0f / 2.4f)) - 0.055f;
  g = g <= 0.0031308f ? 12.92f * g : (1.0f + 0.055f) * powf(g, (1.0f / 2.4f)) - 0.055f;
  b = b <= 0.0031308f ? 12.92f * b : (1.0f + 0.055f) * powf(b, (1.0f / 2.4f)) - 0.055f;

  if (r > b && r > g) {
    // red is biggest
    if (r > 1.0f) {
      g = g / r;
      b = b / r;
      r = 1.0f;
    }
  } else if (g > b && g > r) {
    // green is biggest
    if (g > 1.0f) {
      r = r / g;
      b = b / g;
      g = 1.0f;
    }
  } else if (b > r && b > g) {
    // blue is biggest
    if (b > 1.0f) {
      r = r / b;
      g = g / b;
      b = 1.0f;
    }
  }
  rgb[0] = byte(255.0f*r);
  rgb[1] = byte(255.0f*g);
  rgb[2] = byte(255.0f*b);
}

void mAnimatorLight::Segment::colorRGBtoXY(byte* rgb, float* xy) //rgb to coordinates (https://www.developers.meethue.com/documentation/color-conversions-rgb-xy)
{
  float X = rgb[0] * 0.664511f + rgb[1] * 0.154324f + rgb[2] * 0.162028f;
  float Y = rgb[0] * 0.283881f + rgb[1] * 0.668433f + rgb[2] * 0.047685f;
  float Z = rgb[0] * 0.000088f + rgb[1] * 0.072310f + rgb[2] * 0.986039f;
  xy[0] = X / (X + Y + Z);
  xy[1] = Y / (X + Y + Z);
}
#endif // WLED_DISABLE_HUESYNC

//RRGGBB / WWRRGGBB order for hex
void mAnimatorLight::Segment::colorFromDecOrHexString(byte* rgb, char* in)
{
  if (in[0] == 0) return;
  char first = in[0];
  uint32_t c = 0;

  if (first == '#' || first == 'h' || first == 'H') //is HEX encoded
  {
    c = strtoul(in +1, NULL, 16);
  } else
  {
    c = strtoul(in, NULL, 10);
  }

  rgb[0] = R(c);
  rgb[1] = G(c);
  rgb[2] = B(c);
  rgb[3] = W(c);
}

//contrary to the colorFromDecOrHexString() function, this uses the more standard RRGGBB / RRGGBBWW order
bool mAnimatorLight::Segment::colorFromHexString(byte* rgb, const char* in) {
  if (in == nullptr) return false;
  size_t inputSize = strnlen(in, 9);
  if (inputSize != 6 && inputSize != 8) return false;

  uint32_t c = strtoul(in, NULL, 16);

  if (inputSize == 6) {
    rgb[0] = (c >> 16);
    rgb[1] = (c >>  8);
    rgb[2] =  c       ;
  } else {
    rgb[0] = (c >> 24);
    rgb[1] = (c >> 16);
    rgb[2] = (c >>  8);
    rgb[3] =  c       ;
  }
  return true;
}





// /*
//  * blurs segment content, source: FastLED colorutils.cpp
//  */
// void mAnimatorLight::Segment::blur(uint8_t blur_amount)
// {
// #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   if (is2D()) {
//     // compatibility with 2D
//     const uint16_t cols = virtualWidth();
//     const uint16_t rows = virtualHeight();
//     for (uint16_t i = 0; i < rows; i++) blurRow(i, blur_amount); // blur all rows
//     for (uint16_t k = 0; k < cols; k++) blurCol(k, blur_amount); // blur all columns
//     return;
//   }
// #endif
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
//   CRGB carryover = HTMLColorCode::Black;
//   for(uint16_t i = 0; i < virtualLength(); i++)
//   {
//     CRGB cur = CRGB(getPixelColor(i));
//     CRGB part = cur;
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;
//     if(i > 0) {
//       uint32_t c = getPixelColor(i-1);
//       uint8_t r = R(c);
//       uint8_t g = G(c);
//       uint8_t b = B(c);
//       setPixelColor(i-1, qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue));
//     }
//     setPixelColor(i,cur.red, cur.green, cur.blue);
//     carryover = part;
//   }
// }

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t mAnimatorLight::Segment::color_wheel(uint8_t pos) { // TODO
  // if (palette_id) return color_from_palette(pos, false, true, 0);
  pos = 255 - pos;
  if(pos < 85) {
    return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  } else if(pos < 170) {
    pos -= 85;
    return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  } else {
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t mAnimatorLight::Segment::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}


//do not call this method from system context (network callback)
void mAnimatorLight::finalizeInit(void)
{

  // ALOG_INF(PSTR("mAnimatorLight::finalizeInit_PreInit:\n\r bus_manager->getNumBusses() C%d"), pCONT_iLight->bus_manager->getNumBusses());

  #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  // // for the lack of better place enumerate ledmaps here
  // // if we do it in json.cpp (serializeInfo()) we are getting flashes on LEDs
  // // unfortunately this means we do not get updates after uploads
  // enumerateLedmaps();

  // _hasWhiteChannel = _isOffRefreshRequired = false;

  if(pCONT_iLight->bus_manager == nullptr)
  {
    ALOG_ERR(PSTR("busses null"));
    return;
  }

  //if busses failed to load, add default (fresh install, FS issue, ...)
  if (pCONT_iLight->bus_manager->getNumBusses() == 0) 
  {
    DEBUG_PRINTLN(F("No busses, init default"));
    pCONT_iLight->BusManager_Create_DefaultSingleNeoPixel();
  }

  DEBUG_PRINTF("busses->getNumBusses() %d\n\r", pCONT_iLight->bus_manager->getNumBusses());

  _length = 0;
  for (uint8_t i=0; i<pCONT_iLight->bus_manager->getNumBusses(); i++) 
  {
    
    DEBUG_PRINTF("getNumBusses %d\n\r", i);

    Bus *bus = pCONT_iLight->bus_manager->getBus(i);
    if (bus == nullptr)
    {
      DEBUG_PRINTF("bus == nullptr\n\r");
      continue; // continue breaks one loop occurance only (unlike break)
    }

    if (bus->getStart() + bus->getLength() > PIXEL_RANGE_LIMIT)
    {
      DEBUG_PRINTF("bus->getStart() + bus->getLength() > PIXEL_RANGE_LIMIT MAX_LEDS\n\r");
      break;
    }
    //RGBW mode is enabled if at least one of the strips is RGBW
    _hasWhiteChannel |= bus->hasWhite();
    
    //refresh is required to remain off if at least one of the strips requires the refresh.
    _isOffRefreshRequired |= bus->isOffRefreshRequired();
    uint16_t busEnd = bus->getStart() + bus->getLength();
    
    DEBUG_PRINTF("_hasWhiteChannel %d, _isOffRefreshRequired %d, busEnd %d\n\r", _hasWhiteChannel, _isOffRefreshRequired, busEnd);

    if (busEnd > _length)
    {
      _length = busEnd;
      DEBUG_PRINTF("_length = busEnd;\n\r");
    }

    #ifdef ESP8266
    if ((!IS_BUSTYPE_DIGITAL(bus->getType()) || IS_BUSTYPE_2PIN(bus->getType()))) continue;
    uint8_t pins[5];
    if (!bus->getPins(pins)) continue;
    BusDigital* bd = static_cast<BusDigital*>(bus);
    if (pins[0] == 3) bd->reinit();
    #endif
  }

  #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  //initialize leds array. TBD: realloc if nr of leds change
  // if (mAnimatorLight::Segment::_globalLeds) {
  //   purgeSegments(true);
  //   free(mAnimatorLight::Segment::_globalLeds);
  //   mAnimatorLight::Segment::_globalLeds = nullptr;
  // }
  // if (useLedsArray) {
  //   #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
  //   if (psramFound())
  //     mAnimatorLight::Segment::_globalLeds = (CRGB*) ps_malloc(sizeof(CRGB) * _length);
  //   else
  //   #endif
  //     mAnimatorLight::Segment::_globalLeds = (CRGB*) malloc(sizeof(CRGB) * _length);
  //   memset(mAnimatorLight::Segment::_globalLeds, 0, sizeof(CRGB) * _length);
  // }

  //segments are created in makeAutoSegments();
  loadCustomPalettes(); // (re)load all custom palettes
  
  // ALOG_INF(PSTR("mAnimatorLight::finalizeInit_PreInit:\n\r bus_manager->getNumBusses() D%d"), pCONT_iLight->bus_manager->getNumBusses());

}

// Setter for RGB brightness, with optional global brightness parameter
void mAnimatorLight::Segment::UpdateBrightness()
{
  _brightness_rgb_combined = scale8(_brightness_rgb, pCONT_iLight->getBriRGB_Global());
  _brightness_cct_combined = scale8(_brightness_cct, pCONT_iLight->getBriCCT_Global());

}





// #ifdef ENABLE_DEVFEATURE_LIGHTING__REMOVE_RGBCCT

void IRAM_ATTR mAnimatorLight::setPixelColor(uint32_t i, ColourBaseType col) {
  // Serial.printf(" mAnimatorLight::setPixelColor[%d] = %d,%d,%d,%d)\n\r", i, col.R, col.G, col.B, col.WW);
  i = getMappedPixelIndex(i);
  if (i >= _length) {
    // Serial.printf("BBBBBBBBBBBvoid IRAM_ATTR mAnimatorLight::%d,%dsetPixelColor(uint32_t i, %d,%d,%d)\n\r", i ,_length,col.R, col.G, col.B);
    
    
    
    return;}
  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
  Serial.printf("void IRAM_ATTR mAnimatorLight::setPixelColor(uint32_t i, %d,%d,%d)\n\r", col.R, col.G, col.B);
  #endif
  pCONT_iLight->bus_manager->setPixelColor(i, col);
}

ColourBaseType IRAM_ATTR mAnimatorLight::getPixelColor(uint32_t i) const {
  i = getMappedPixelIndex(i);
  if (i >= _length) return 0;
  return pCONT_iLight->bus_manager->getPixelColor(i);
}



// #else // In this case, Rgbcct must be mapped back into U32 as a temporary measure








// // IS contained within segment so busses etc are known (ie the right pin will be used)
// // void IRAM_ATTR mAnimatorLight::BUS_setPixelColor(int i, uint32_t col)
// void IRAM_ATTR mAnimatorLight::setPixelColor(int i, uint32_t col)
// {




//   #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
//   ALOG_INF(PSTR("i0--------setPixelColor %d, %d, %d, %d"), i, R(col), G(col), B(col));
//   #endif 
//   if (i >= _length) return;
//   if (i < customMappingSize)
//   {
//     // ALOG_INF(PSTR("setPixelColor customMappingTable %d -> %d"), i, customMappingTable[i]);
//     i = customMappingTable[i];
//   }
//   RgbcctColor c = RgbcctColor( R(col),G(col),B(col),W(col),W(col) );




//   #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
//   ALOG_INF(PSTR("i1--------setPixelColor %d, %d, %d, %d"), i, R(col), G(col), B(col));
//   #endif
//   pCONT_iLight->bus_manager->setPixelColor(i, c);
// }

// // uint32_t mAnimatorLight::BUS_getPixelColor(uint16_t i)
// uint32_t mAnimatorLight::getPixelColor(uint16_t i)
// {
//   if (i >= _length) { ALOG_ERR(PSTR("return early")); return 0; }
//   if (i < customMappingSize) i = customMappingTable[i];
//   return pCONT_iLight->bus_manager->getPixelColor(i).getU32();
// }

// /**
//  * @brief NEw method which are the only entry point into the busmanager for RgbcctColour Types and hence enable mapping
//  * 
//  */
// void IRAM_ATTR mAnimatorLight::setPixelColor_Rgbcct(int i, RgbcctColor col)
// {
//   #ifndef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
//   #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
//   ALOG_INF(PSTR("i0--------setPixelColor %d, %d, %d, %d"), i, col.R, col.G, col.B);
//   #endif 
//   if (i >= _length)
//   {
//     ALOG_INF(PSTR("i len %d %d"),i,_length);
//     return;
//   }
//   if (i < customMappingSize)
//   {
//     ALOG_INF(PSTR("setPixelColor customMappingSize %d -> %d"), i, customMappingSize);
//     // ALOG_INF(PSTR("setPixelColor customMappingTable %d -> %d"), i, customMappingTable[i]);
//     i = customMappingTable[i];
//   }
//   #ifdef ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
//   ALOG_INF(PSTR("i1--------setPixelColor %d, %d, %d, %d"), i, col.R, col.G, col.B);
//   #endif

//   // ALOG_INF(PSTR("busnum %d"), pCONT_iLight->bus_manager->getNumBusses());
//   #endif // ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING

//   // DEBUG_TIME__START
//   pCONT_iLight->bus_manager->setPixelColor(i, col);
//   // if(i==0) DEBUG_TIME__SHOW_MESSAGE("set")


// }

// RgbcctColor mAnimatorLight::getPixelColor_Rgbcct(uint16_t i)
// {
//   // if (i >= _length) {
//   //   return RgbcctColor((uint32_t)0);  // Early exit if index exceeds length
//   // }
  
//   if (i < customMappingSize) {
//     i = customMappingTable[i];  // Update index if it's within custom mapping size
//   }
  
//   // Directly return the color retrieved from the bus manager
//   return pCONT_iLight->bus_manager->getPixelColor(i);
// }


// #endif




//DISCLAIMER
//The following function attemps to calculate the current LED power usage,
//and will limit the brightness to stay below a set amperage threshold.
//It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
//Stay safe with high amperage and have a reasonable safety margin!
//I am NOT to be held liable for burned down garages!

//fine tune power estimation constants for your setup                  
#define MA_FOR_ESP        100 //how much mA does the ESP use (Wemos D1 about 80mA, ESP32 about 120mA)
                              //you can set it to 0 if the ESP is powered by USB and the LEDs by external

void mAnimatorLight::estimateCurrentAndLimitBri() {
  //power limit calculation
  //each LED can draw up 195075 "power units" (approx. 53mA)
  //one PU is the power it takes to have 1 channel 1 step brighter per brightness step
  //so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
  bool useWackyWS2815PowerModel = false;
  byte actualMilliampsPerLed = milliampsPerLed;

  // if(milliampsPerLed == 255) {
  //   useWackyWS2815PowerModel = true;
  //   actualMilliampsPerLed = 12; // from testing an actual strip
  // }

  // if (ablMilliampsMax < 150 || actualMilliampsPerLed == 0) { //0 mA per LED and too low numbers turn off calculation
  //   currentMilliamps = 0;
  //   busses.setBrightness(_brightness);
  //   return;
  // }

  // uint16_t pLen = getLengthPhysical();
  // uint32_t puPerMilliamp = 195075 / actualMilliampsPerLed;
  // uint32_t powerBudget = (ablMilliampsMax - MA_FOR_ESP) * puPerMilliamp; //100mA for ESP power
  // if (powerBudget > puPerMilliamp * pLen) { //each LED uses about 1mA in standby, exclude that from power budget
  //   powerBudget -= puPerMilliamp * pLen;
  // } else {
  //   powerBudget = 0;
  // }

  // uint32_t powerSum = 0;

  // for (uint8_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   uint16_t len = bus->getLength();
  //   uint32_t busPowerSum = 0;
  //   for (uint16_t i = 0; i < len; i++) { //sum up the usage of each LED
  //     uint32_t c = bus->getPixelColor(i);
  //     byte r = R(c), g = G(c), b = B(c), w = W(c);

  //     if(useWackyWS2815PowerModel) { //ignore white component on WS2815 power calculation
  //       busPowerSum += (MAX(MAX(r,g),b)) * 3;
  //     } else {
  //       busPowerSum += (r + g + b + w);
  //     }
  //   }

  //   if (bus->isRgbw()) { //RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
  //     busPowerSum *= 3;
  //     busPowerSum = busPowerSum >> 2; //same as /= 4
  //   }
  //   powerSum += busPowerSum;
  // }

  // uint32_t powerSum0 = powerSum;
  // powerSum *= _brightness;
  
  // if (powerSum > powerBudget) //scale brightness down to stay in current limit
  // {
  //   float scale = (float)powerBudget / (float)powerSum;
  //   uint16_t scaleI = scale * 255;
  //   uint8_t scaleB = (scaleI > 255) ? 255 : scaleI;
  //   uint8_t newBri = scale8(_brightness, scaleB);
  //   busses.setBrightness(newBri); //to keep brightness uniform, sets virtual busses too
  //   currentMilliamps = (powerSum0 * newBri) / puPerMilliamp;
  // } else {
  //   currentMilliamps = powerSum / puPerMilliamp;
  //   busses.setBrightness(_brightness);
  // }
  // currentMilliamps += MA_FOR_ESP; //add power of ESP back to estimate
  // currentMilliamps += pLen; //add standby power back to estimate
}

// turns all LEDs off and restarts ESP
void mAnimatorLight::reset()
{
  // briT = 0;
  #ifdef WLED_ENABLE_WEBSOCKETS2
  tkr_web->ws->closeAll(1012);
  #endif
  long dly = millis();
  while (millis() - dly < 450) {
    yield();        // enough time to send response to client
  }
  // applyBri();
  DEBUG_PRINTLN(F("WLED RESET"));
  ESP.restart();
}


void mAnimatorLight::show(void) 
{  

  // avoid race condition, caputre _callback value
  show_callback callback = _callback;
  if (callback) callback();

  unsigned long now = millis();

  BusManager::show();

  unsigned long diff = now - _lastShow;
  uint16_t fpsCurr = 200;
  if (diff > 0) fpsCurr = 1000 / diff;
  _cumulativeFps = (3 * _cumulativeFps + fpsCurr) >> 2;
  _lastShow = now;
  // Serial.printf("%d lastshow\n\r", _lastShow);

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL
  DEBUG_TIME__START
  uint32_t elapsed = millis() - tSaved_LoopTime;
  if(elapsed > 10) Serial.printf("LoopElapsed %d\n\r", elapsed);
  tSaved_LoopTime = millis();
  #endif
}

/**
 * Returns a true value if any of the strips are still being updated.
 * On some hardware (ESP32), strip updates are done asynchronously.
 */
bool mAnimatorLight::isUpdating() {
  return !pCONT_iLight->bus_manager->canAllShow();
}

/**
 * Returns the refresh rate of the LED tkr_anim-> Useful for finding out whether a given setup is fast enough.
 * Only updates on show() or is set to 0 fps if last show is more than 2 secs ago, so accurary varies
 */
uint16_t mAnimatorLight::getFps() {
  if (millis() - _lastShow > 2000) return 0;
  return _cumulativeFps +1;
}

void mAnimatorLight::setTargetFps(uint8_t fps) {
  if (fps > 0 && fps <= 120) _targetFps = fps;
  _frametime = 1000 / _targetFps;
}

void mAnimatorLight::setMode(uint8_t segid, uint8_t m) {
  if (segid >= segments.size()) return;
   
  if (m >= getModeCount()) m = getModeCount() - 1;

  if (segments[segid].animation_mode_id != m) {
    // segments[segid].startTransition(_transitionDur); // set effect transitions
    //segments[segid].markForReset();
    segments[segid].animation_mode_id = m;
  }
}

//applies to all active and selected segments
void mAnimatorLight::setColor(uint8_t slot, uint32_t c) {
  if (slot >= RGBCCTCOLOURS_SIZE) return;

  for (segment &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setColor(slot, c);
    }
  }
}

void mAnimatorLight::setCCT(uint16_t k) {
  for (segment &seg : segments) {
    if (seg.isActive() && seg.isSelected()) {
      seg.setCCT(k);
    }
  }
}

void mAnimatorLight::setBrightness(uint8_t b, bool direct) {
  ALOG_INF(PSTR("setBrightness(%d, %d)"),b,direct);
  // if (gammaCorrectBri) b = gamma8(b);  
  if (_brightness == b) return;
  _brightness = b;
  if (_brightness == 0) { //unfreeze all segments on power off
    for (segment &seg : segments) {
      seg.freeze = false;
    }
  }
  BusManager::setBrightness(b);
  if (direct) {
    // would be dangerous if applied immediately (could exceed ABL), but will not output until the next show()
    // busses.setBrightness(b);
  } else {
    unsigned long t = millis();
    if (segments[0].next_time > t + 22 && t - _lastShow > MIN_SHOW_DELAY) show(); //apply brightness change immediately if no refresh soon
  }
}

uint8_t mAnimatorLight::getFirstSelectedSegId(void)
{
  size_t i = 0;
  for (segment &seg : segments) {
    if (seg.isActive() && seg.isSelected()) return i;
    i++;
  }
  // if none selected, use the main segment
  return getMainSegmentId();
}

void mAnimatorLight::setMainSegmentId(uint8_t n) {
  _mainSegment = 0;
  if (n < segments.size()) {
    _mainSegment = n;
  }
  return;
}

uint8_t mAnimatorLight::getLastActiveSegmentId(void) {
  for (size_t i = segments.size() -1; i > 0; i--) {
    if (segments[i].isActive()) return i;
  }
  return 0;
}

uint8_t mAnimatorLight::getActiveSegmentsNum(void) {
  uint8_t c = 0;
  for (size_t i = 0; i < segments.size(); i++) {
    if (segments[i].isActive()) c++;
  }
  return c;
}

uint16_t mAnimatorLight::getLengthPhysical(void) {
  uint16_t len = 0;
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus->getType() >= TYPE_NET_DDP_RGB) continue; //exclude non-physical network busses
  //   len += bus->getLength();
  // }
  return len;
}

//used for JSON API info.leds.rgbw. Little practical use, deprecate with info.leds.rgbw.
//returns if there is an RGBW bus (supports RGB and White, not only white)
//not influenced by auto-white mode, also true if white slider does not affect output white channel
bool mAnimatorLight::hasRGBWBus(void) {
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   switch (bus->getType()) {
  //     case TYPE_SK6812_RGBW:
  //     case TYPE_TM1814:
  //     case TYPE_ANALOG_4CH:
  //       return true;
  //   }
  // }
  // return false;
}

bool mAnimatorLight::hasCCTBus(void) {
  // if (cctFromRgb && !correctWB) return false;
  // for (size_t b = 0; b < busses.getNumBusses(); b++) {
  //   Bus *bus = busses.getBus(b);
  //   if (bus == nullptr || bus->getLength()==0) break;
  //   switch (bus->getType()) {
  //     case TYPE_ANALOG_5CH:
  //     case TYPE_ANALOG_2CH:
  //       return true;
  //   }
  // }
  // return false;
}

void mAnimatorLight::purgeSegments(bool force) 
{
  ALOG_INF(PSTR("purgeSegments(%d)"),force);
  ALOG_INF(PSTR("segments.size=%d"),segments.size());
  // remove all inactive segments (from the back)
  int deleted = 0;
  if (segments.size() <= 1) return;

  for (size_t i = segments.size()-1; i > 0; i--)
  {
    ALOG_INF(PSTR("i=%d"),i);
    if (segments[i].stop == 0 || force) {
      DEBUG_PRINT(F("Purging segment segment: ")); DEBUG_PRINTLN(i);
      deleted++;
      segments.erase(segments.begin() + i);
    }
  }
  if (deleted) {
    segments.shrink_to_fit();
    if (_mainSegment >= segments.size()) setMainSegmentId(0);
  }
}

mAnimatorLight::Segment& mAnimatorLight::getSegment(uint8_t id) {
  return segments[id >= segments.size() ? getMainSegmentId() : id]; // vectors
}

/**
 * @brief For turning on a new effect/segment
 * 
 * @param n 
 * @param i1 
 * @param i2 
 * @param grouping 
 * @param spacing 
 * @param offset 
 * @param startY 
 * @param stopY 
 */
void mAnimatorLight::setSegment(uint8_t n, uint16_t i1, uint16_t i2, uint8_t grouping, uint8_t spacing, uint16_t offset, uint16_t startY, uint16_t stopY) {
  if (n >= segments.size()) return;
  mAnimatorLight::Segment& seg = segments[n];

  //return if neither bounds nor grouping have changed
  bool boundsUnchanged = (seg.start == i1 && seg.stop == i2);
  if (isMatrix) {
    boundsUnchanged &= (seg.startY == startY && seg.stopY == stopY);
  }
  if (boundsUnchanged
      && (!grouping || (seg.grouping_get() == grouping && seg.spacing == spacing))
      && (offset == UINT16_MAX || offset == seg.offset)) return;

  //if (seg.stop) setRange(seg.start, seg.stop -1, BLACK); //turn old segment range off
  if (seg.stop) seg.fill(BLACK); //turn old segment range off
  if (i2 <= i1) //disable segment
  {
    // disabled segments should get removed using purgeSegments()
    DEBUG_PRINT(F("-- mAnimatorLight::Segment ")); DEBUG_PRINT(n); DEBUG_PRINTLN(F(" marked inactive."));
    seg.stop = 0;
    //if (seg.name) {
    //  delete[] seg.name;
    //  seg.name = nullptr;
    //}
    // if main segment is deleted, set first active as main segment
    if (n == _mainSegment) setMainSegmentId(0);
    seg.markForReset();
    return;
  }
  if (isMatrix) {
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    if (i1 < mAnimatorLight::Segment::maxWidth) seg.start = i1;
    seg.stop = i2 > mAnimatorLight::Segment::maxWidth ? mAnimatorLight::Segment::maxWidth : i2;
    if (startY < mAnimatorLight::Segment::maxHeight) seg.startY = startY;
    seg.stopY = stopY > mAnimatorLight::Segment::maxHeight ? mAnimatorLight::Segment::maxHeight : MAX(1,stopY);

    ALOG_INF(PSTR("setSegment(%d, %d, %d, %d, %d, %d, %d, %d)"),n,i1,i2,grouping,spacing,offset,startY,stopY);
    delay(1000);
    #endif
  } else {
    if (i1 < _length) seg.start = i1;
    seg.stop = i2 > _length ? _length : i2;
    seg.startY = 0;
    seg.stopY  = 1;
  }
  if (grouping) {
    seg.grouping_set(grouping);
    seg.spacing = spacing;
  }
  if (offset < UINT16_MAX) seg.offset = offset;
  seg.markForReset();
  if (!boundsUnchanged) seg.refreshLightCapabilities();
}

void mAnimatorLight::restartRuntime() {
  for (segment &seg : segments) seg.markForReset();
}

void mAnimatorLight::resetSegments2() {
  segments.clear(); // destructs all mAnimatorLight::Segment as part of clearing
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  segment seg = isMatrix ? mAnimatorLight::Segment(0, mAnimatorLight::Segment::maxWidth, 0, mAnimatorLight::Segment::maxHeight) : mAnimatorLight::Segment(0, _length);
  #else
  segment seg = mAnimatorLight::Segment(0, _length);
  #endif
  segments.push_back(seg);
  _mainSegment = 0;
}

void mAnimatorLight::makeAutoSegments(bool forceReset) {
  if (isMatrix) {
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    // only create 1 2D segment
    if (forceReset || getSegmentsNum() == 0) resetSegments2(); // initialises 1 segment
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      segments[i].start  = 0;
      segments[i].stop   = mAnimatorLight::Segment::maxWidth;
      segments[i].startY = 0;
      segments[i].stopY  = mAnimatorLight::Segment::maxHeight;
      segments[i].grouping = 1;
      segments[i].spacing  = 0;
      _mainSegment = i;
    }
    #endif
  } else 
  if (autoSegments) { //make one segment per bus
    uint16_t segStarts[MAX_NUM_SEGMENTS] = {0};
    uint16_t segStops [MAX_NUM_SEGMENTS] = {0};
    uint8_t s = 0;
    for (uint8_t i = 0; i < pCONT_iLight->bus_manager->getNumBusses(); i++) {
      Bus* b = pCONT_iLight->bus_manager->getBus(i);

      segStarts[s] = b->getStart();
      segStops[s]  = segStarts[s] + b->getLength();

      //check for overlap with previous segments
      for (size_t j = 0; j < s; j++) {
        if (segStops[j] > segStarts[s] && segStarts[j] < segStops[s]) {
          //segments overlap, merge
          segStarts[j] = min(segStarts[s],segStarts[j]);
          segStops [j] = max(segStops [s],segStops [j]); segStops[s] = 0;
          s--;
        }
      }
      s++;
    }
    segments.clear();
    for (size_t i = 0; i < s; i++) {
      mAnimatorLight::Segment seg = mAnimatorLight::Segment(segStarts[i], segStops[i]);
      seg.selected = true;
      segments.push_back(seg);
    }
    _mainSegment = 0;
  } else 
  {
    if (forceReset || getSegmentsNum() == 0) resetSegments2();
    //expand the main seg to the entire length, but only if there are no other segments, or reset is forced
    else if (getActiveSegmentsNum() == 1) {
      size_t i = getLastActiveSegmentId();
      segments[i].start = 0;
      segments[i].stop  = _length;
      _mainSegment = 0;
    }
  }

  fixInvalidSegments();
}

void mAnimatorLight::fixInvalidSegments() {
  //make sure no segment is longer than total (sanity check)
  for (size_t i = getSegmentsNum()-1; i > 0; i--) {
    if (segments[i].start >= _length) { segments.erase(segments.begin()+i); continue; }
    if (segments[i].stop  >  _length) segments[i].stop = _length;
    // this is always called as the last step after finalizeInit(), update covered bus types
    segments[i].refreshLightCapabilities();
  }
}

//true if all segments align with a bus, or if a segment covers the total length
bool mAnimatorLight::checkSegmentAlignment() {
  bool aligned = false;
  for (segment &seg : segments) {
    for (uint8_t b = 0; pCONT_iLight->bus_manager->getNumBusses(); b++) {
      Bus *bus = pCONT_iLight->bus_manager->getBus(b);
      if (seg.start == bus->getStart() && seg.stop == bus->getStart() + bus->getLength()) aligned = true;
    }
    if (seg.start == 0 && seg.stop == _length) aligned = true;
    if (!aligned) return false;
  }
  return true;
}

//After this function is called, setPixelColor() will use that segment (offsets, grouping, ... will apply)
//Note: If called in an interrupt (e.g. JSON API), original segment must be restored,
//otherwise it can lead to a crash on ESP32 because _segment_index is modified while in use by the main thread
uint8_t mAnimatorLight::setPixelSegment(uint8_t n)
{
  uint8_t prevSegId = segment_current_index;
  if (n < segments.size()) {
    segment_current_index = n;
    _virtualSegmentLength = segments[segment_current_index].virtualLength();
  }
  return prevSegId;
}

void mAnimatorLight::setRange(uint16_t i, uint16_t i2, uint32_t col)
{
  if (i2 >= i)
  {
    for (uint16_t x = i; x <= i2; x++) setPixelColor(x, col);
  } else
  {
    for (uint16_t x = i2; x <= i; x++) setPixelColor(x, col);
  }
}

void mAnimatorLight::setTransitionMode(bool t)
{
  // for (segment &seg : segments) if (!seg.transitional) seg.startTransition(t ? _transitionDur : 0);
}

#ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
// void mAnimatorLight::printSize()
// {
//   size_t size = 0;
//   for (const mAnimatorLight::Segment &seg : segments) size += seg.getSize();
//   DEBUG_PRINTF("Segments: %d -> %uB\n", segments.size(), size);
//   DEBUG_PRINTF("Modes: %d*%d=%uB\n", sizeof(mode_ptr), effects.function.size(), (effects.function.capacity()*sizeof(mode_ptr)));
//   DEBUG_PRINTF("Data: %d*%d=%uB\n", sizeof(const char *), effects.config.size(), (effects.config.capacity()*sizeof(const char *)));
//   DEBUG_PRINTF("Map: %d*%d=%uB\n", sizeof(uint16_t), (int)customMappingSize, customMappingSize*sizeof(uint16_t));
//   if (useLedsArray) DEBUG_PRINTF("Buffer: %d*%d=%uB\n", sizeof(CRGB), (int)_length, _length*sizeof(CRGB));
// }
#endif

void mAnimatorLight::loadCustomPalettes()
{
  byte tcp[72]; //support gradient palettes with up to 18 entries
  CRGBPalette16 targetPalette;
  customPalettes.clear(); // start fresh
  // for (int index = 0; index<10; index++) {
  //   char fileName[32];
  //   sprintf_P(fileName, PSTR("/palette%d.json"), index);

  //   StaticJsonDocument<1536> pDoc; // barely enough to fit 72 numbers
  //   if (FILE_SYSTEM.exists(fileName)) {
  //     DEBUG_PRINT(F("Reading palette from "));
  //     DEBUG_PRINTLN(fileName);

  //     if (readObjectFromFile(fileName, nullptr, &pDoc)) {
  //       JsonArray pal = pDoc[F("palette")];
  //       if (!pal.isNull() && pal.size()>7) { // not an empty palette (at least 2 entries)
  //         size_t palSize = MIN(pal.size(), 72);
  //         palSize -= palSize % 4; // make sure size is multiple of 4
  //         for (size_t i=0; i<palSize && pal[i].as<int>()<256; i+=4) {
  //           tcp[ i ] = (uint8_t) pal[ i ].as<int>(); // index
  //           tcp[i+1] = (uint8_t) pal[i+1].as<int>(); // R
  //           tcp[i+2] = (uint8_t) pal[i+2].as<int>(); // G
  //           tcp[i+3] = (uint8_t) pal[i+3].as<int>(); // B
  //           DEBUG_PRINTF("%d(%d) : %d %d %d\n", i, int(tcp[i]), int(tcp[i+1]), int(tcp[i+2]), int(tcp[i+3]));
  //         }
  //         customPalettes.push_back(targetPalette.loadDynamicGradientPalette(tcp));
  //       }
  //     }
  //   } else {
  //     break;
  //   }
  // }
}


//load custom mapping table from JSON file (called from finalizeInit() or deserializeState())
// bool mAnimatorLight::deserializeMap(uint8_t n) {
//   // 2D support creates its own ledmap (on the fly) if a ledmap.json exists it will overwrite built one.
  
//   ALOG_INF(PSTR("deserializeMap"));

//   char fileName[32];
//   strcpy_P(fileName, PSTR("/ledmap"));
//   if (n) sprintf(fileName +7, "%d", n);
//   strcat_P(fileName, PSTR(".json"));

//   ALOG_INF(PSTR("deserializeMap: %s"), fileName);

//   bool isFile = FILE_SYSTEM.exists(fileName);

//   if (!isFile) {

//     ALOG_INF(PSTR("deserializeMap: !isFile"));

//     // erase custom mapping if selecting nonexistent ledmap.json (n==0)
//     if (!isMatrix && !n && customMappingTable != nullptr) {
//       customMappingSize = 0;
//       delete[] customMappingTable;
//       customMappingTable = nullptr;
//     }
//     return false;
//   }

//   if (!requestJSONBufferLock(7)) return false;

//   if (!pCONT_mfile->readObjectFromFile(fileName, nullptr, &doc)) {
//     releaseJSONBufferLock();
//     return false; //if file does not exist just exit
//   }

//   DEBUG_PRINT(F("Reading LED map from "));
//   DEBUG_PRINTLN(fileName);

//   // erase old custom ledmap
//   if (customMappingTable != nullptr) {
//     customMappingSize = 0;
//     delete[] customMappingTable;
//     customMappingTable = nullptr;
//   }

//   JsonArray map = doc[F("map")];
//   if (!map.isNull() && map.size()) {  // not an empty map
//     customMappingSize  = map.size();
//     customMappingTable = new uint16_t[customMappingSize];
//     for (unsigned i=0; i<customMappingSize; i++) {
//       customMappingTable[i] = (uint16_t) (map[i]<0 ? 0xFFFFU : map[i]);
//     }
//   }

//   releaseJSONBufferLock();
//   return true;
// }


bool mAnimatorLight::deserializeMap(uint8_t n) {
  // 2D support creates its own ledmap (on the fly) if a ledmap.json exists it will overwrite built one.

  char fileName[32];
  strcpy_P(fileName, PSTR("/ledmap"));
  if (n) sprintf(fileName +7, "%d", n);
  strcat_P(fileName, PSTR(".json"));
  bool isFile = FILE_SYSTEM.exists(fileName);

  customMappingSize = 0; // prevent use of mapping if anything goes wrong
  currentLedmap = 0;
  if (n == 0 || isFile) interfaceUpdateCallMode = CALL_MODE_WS_SEND; // schedule WS update (to inform UI)

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (!isFile && n==0 && isMatrix) {
    setUpMatrix();
    return false;
  }
  #endif

  if (!isFile || !requestJSONBufferLock(7)) return false;

  if (!pCONT_mfile->readObjectFromFile(fileName, nullptr, pDoc)) {
    DEBUG_PRINT(F("ERROR Invalid ledmap in ")); DEBUG_PRINTLN(fileName);
    releaseJSONBufferLock();
    return false; // if file does not load properly then exit
  }

  JsonObject root = pDoc->as<JsonObject>();
  // if we are loading default ledmap (at boot) set matrix width and height from the ledmap (compatible with WLED MM ledmaps)
  if (isMatrix && n == 0 && (!root[F("width")].isNull() || !root[F("height")].isNull())) {
    Segment::maxWidth  = min(max(root[F("width")].as<int>(), 1), 128);
    Segment::maxHeight = min(max(root[F("height")].as<int>(), 1), 128);
  }

  if (customMappingTable) delete[] customMappingTable;
  customMappingTable = new uint16_t[getLengthTotal()];

  if (customMappingTable) {
    DEBUG_PRINT(F("Reading LED map from ")); DEBUG_PRINTLN(fileName);
    JsonArray map = root[F("map")];
    if (!map.isNull() && map.size()) {  // not an empty map
      customMappingSize = min((unsigned)map.size(), (unsigned)getLengthTotal());
      for (unsigned i=0; i<customMappingSize; i++) customMappingTable[i] = (uint16_t) (map[i]<0 ? 0xFFFFU : map[i]);
      currentLedmap = n;
    }
  } else {
    DEBUG_PRINTLN(F("ERROR LED map allocation error."));
  }

  releaseJSONBufferLock();
  return (customMappingSize > 0);
}





// // PHASE THIS OUT
// RgbwwColor 
// #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
// IRAM_ATTR 
// #endif 
// mAnimatorLight::Segment::GetPaletteColour(
//   /**
//    * @brief _pixel_position
//    * ** [0-SEGLEN]
//    * ** [0-255]   
//    */
//   uint16_t pixel_position,
//   /**
//    * @brief flag_spanned_segment
//    * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
//    * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
//    * ** [2]: preffered
//    */
//   uint8_t     flag_spanned_segment, 
//   /**
//    * @brief flag_wrap_hard_edge
//    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
//    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
//    */
//   uint8_t     flag_wrap_hard_edge,
//   /**
//    * @brief flag_crgb_exact_colour
//    * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
//    * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
//    */
//   uint8_t     flag_crgb_exact_colour,
//   /**
//    * @brief encoded_value
//    * ** [uint32_t*] : encoded value from palette
//    */
//   uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly

//   bool apply_brightness
// ){
  
//   if(palette_id != palette_container->loaded_palette_id)
//   {
//     LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
//   }
  
//   /**
//    * @brief These functions always need called as they are dynamic
//    * I should make this a check here, if palette is dynamic, then load everytime
//    * 
//    * perhaps also add a timer here, so it has a backoff and is only called the minimum amount needed
//    * ie have a new tSaved_DynamicUpdate 
//    */
//   else // else so it only tries this if the above "if" did not occur to stop double loads
//   if(
//     (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && 
//     (palette_id <= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID)
//   ){
//     LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods    
//   }

//   RgbcctColor colour = mPaletteI->GetColourFromPreloadedPaletteBuffer_2023(
//     palette_id,
//     (uint8_t*)palette_container->pData.data(),
//     pixel_position,
//     encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
//     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
//     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
//     flag_crgb_exact_colour
//   );

//   // Apply brightness if needed
//   if (apply_brightness) {
//     uint8_t brightness = scale8(_brightness_rgb, pCONT_iLight->getBriRGB_Global());

//     uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range

//     colour.R  = (colour.R * scale) >> 8;
//     colour.G  = (colour.G * scale) >> 8;
//     colour.B  = (colour.B * scale) >> 8;
//     colour.WW = (colour.WW * scale) >> 8;
//     colour.CW = (colour.CW * scale) >> 8;

//   }



//   return colour;

// }


RgbwwColor mAnimatorLight::Segment::GetPaletteColour_Rgbww(
  /**
   * @brief _pixel_position
   * ** [0-SEGLEN]
   * ** [0-255]   
   */
  uint16_t pixel_position,
  /**
   * @brief flag_spanned_segment
   * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
   * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
   * ** [2]: preffered
   */
  uint8_t     flag_spanned_segment, 
  /**
   * @brief flag_wrap_hard_edge
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_wrap_hard_edge,
  /**
   * @brief flag_crgb_exact_colour
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_crgb_exact_colour,
  /**
   * @brief encoded_value
   * ** [uint32_t*] : encoded value from palette
   */
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly

  bool apply_brightness
){
  
  if(palette_id != palette_container->loaded_palette_id)
  {
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }
  
  /**
   * @brief These functions always need called as they are dynamic
   * I should make this a check here, if palette is dynamic, then load everytime
   * 
   * perhaps also add a timer here, so it has a backoff and is only called the minimum amount needed
   * ie have a new tSaved_DynamicUpdate 
   */
  else // else so it only tries this if the above "if" did not occur to stop double loads
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && 
    (palette_id <= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID)
  ){
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods    
  }

  RgbwwColor colour = mPaletteI->GetColourFromPreloadedPaletteBuffer_2023(
    palette_id,
    (uint8_t*)palette_container->pData.data(),
    pixel_position,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour
  );

  // Apply brightness if needed
  if (apply_brightness) {
    uint8_t brightness = scale8(_brightness_rgb, pCONT_iLight->getBriRGB_Global());

    uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range

    colour.R  = (colour.R * scale) >> 8;
    colour.G  = (colour.G * scale) >> 8;
    colour.B  = (colour.B * scale) >> 8;
    colour.WW = (colour.WW * scale) >> 8;
    colour.CW = (colour.CW * scale) >> 8;

  }



  return colour;

}



uint32_t mAnimatorLight::Segment::GetPaletteColour(
  /**
   * @brief _pixel_position
   * ** [0-SEGLEN]
   * ** [0-255]   
   */
  uint16_t pixel_position,
  /**
   * @brief flag_spanned_segment
   * ** [1] : If spanned segment, then indexing (0-255) is expanded into the SEGLEN 
   * ** [0]: Unchanged, index coming in will be 0-SEGLEN but never scaled into 255. Or should it be?
   * ** [2]: preffered
   */
  uint8_t     flag_spanned_segment, 
  /**
   * @brief flag_wrap_hard_edge
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_wrap_hard_edge,
  /**
   * @brief flag_crgb_exact_colour
   * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
   * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
   */
  uint8_t     flag_crgb_exact_colour,
  /**
   * @brief encoded_value
   * ** [uint32_t*] : encoded value from palette
   */
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  /**
   * @brief apply_brightness
   * ** [false] : Apply brightness to the colour
   * ** [true]  : Get the "full" 255 range colour object
   */
  bool apply_brightness,
  
      uint8_t pbri,

      uint8_t mcol
){
  
  if(palette_id != palette_container->loaded_palette_id)
  {
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }


  // uint32_t color = SEGCOLOR(mcol < NUM_COLORS ? mcol : 0);
  // // default palette or no RGB support on segment
  // if ((palette == 0 && mcol < NUM_COLORS) || !_isRGB) {
  //   return color_fade(color, pbri, true);
  // }
  
  /**
   * @brief These functions always need called as they are dynamic
   * I should make this a check here, if palette is dynamic, then load everytime
   * 
   * perhaps also add a timer here, so it has a backoff and is only called the minimum amount needed
   * ie have a new tSaved_DynamicUpdate 
   */
  else // else so it only tries this if the above "if" did not occur to stop double loads
  if(
    (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID) && 
    (palette_id <= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID)
  ){
    LoadPalette(palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods    
  }

  uint32_t colour = mPaletteI->GetColourFromPreloadedPaletteBuffer_2025(
    palette_id,
    (uint8_t*)palette_container->pData.data(),
    pixel_position,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour
  );

  // Apply brightness if needed
  if (apply_brightness) {
    uint8_t brightness = scale8(_brightness_rgb, pCONT_iLight->getBriRGB_Global());
    // ALOG_INF(PSTR("brightness getpal %d"),brightness);
    uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range

    // Extract, scale, and repack in one step
    colour = RGBW32(
      (R(colour) * scale) >> 8,  // Red
      (G(colour) * scale) >> 8,  // Green
      (B(colour) * scale) >> 8,  // Blue
      (W(colour) * scale) >> 8   // White
    );
  }




  return colour;

}

CRGB mAnimatorLight::ColorFromPalette_WithLoad(const CRGBPalette16 &pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
  if(SEGMENT.palette_id != SEGMENT.palette_container->loaded_palette_id)
  {
    SEGMENT.LoadPalette(SEGMENT.palette_id);  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
  }
  return ColorFromPalette(pal, index, brightness, blendType);
}

uint8_t mAnimatorLight::Segment::GetPaletteDiscreteWidth()
{
//   if(palette_id != palette_container->loaded_palette_id)
//   {
//     tkr_anim->LoadPalette(palette_id, tkr_anim->getCurrSegmentId());  //loadPalette perhaps needs to be a segment instance instead. Though this will block unloaded methods
//   }

//   return mPaletteI->GetPaletteDiscreteWidth(
//     palette_id,
//     (uint8_t*)palette_container->pData.data()
//   );

  return mPaletteI->GetColoursInPalette(palette_id);

}

/**
 * @brief 
 * The problem is this function will still cause a reload if a LoadPalette type is used. So preference is to use a CRGBPalette type as overdraw when needed
 */
RgbwwColor IRAM_ATTR mAnimatorLight::GetColourFromUnloadedPalette3(
  uint16_t palette_id,
  uint16_t _pixel_position,
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  uint8_t* encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_request_is_for_full_visual_output
){
  /**
   * @brief Directly handle certain palette types that don't need loading.
   * These palette types are handled directly, bypassing the LoadPalette function.
   */
  if (
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)) ||
    ((palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID))
  ) {
    // These palettes do not require loading into RAM. Directly call GetColourFromPreloadedPaletteBuffer_2023
    return mPaletteI->GetColourFromPreloadedPaletteBuffer_2023(
      palette_id,
      nullptr, // No buffer required for these types
      _pixel_position,
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour,
      flag_request_is_for_full_visual_output
    );
  }

  /**
   * @brief Load is required for other palette types, so we call LoadPalette
   */
  mPaletteLoaded palette_container_temp = mPaletteLoaded();
  SEGMENT.LoadPalette(palette_id, &palette_container_temp);
  
  return mPaletteI->GetColourFromPreloadedPaletteBuffer_2023(
    palette_id,
    &palette_container_temp.pData[0],
    _pixel_position,
    encoded_value,
    flag_spanned_segment,
    flag_wrap_hard_edge,
    flag_crgb_exact_colour,
    flag_request_is_for_full_visual_output
  );
}


#ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  // #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
// ColourBaseType
void IRAM_ATTR mAnimatorLight::Segment::setPixelColor(int i, uint32_t col)//, bool flag_brightness_already_applied)
{

  // #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
  // if(i<10)
  // Serial.printf("Segment::setPixelColor(%d, col %d,%d,%d\n\r", i, R(col), G(col), B(col));
  // #endif

  if (!isActive() || i < 0) return; // not active or invalid index
#ifndef WLED_DISABLE_2D
  int vStrip = 0;
#endif
  int vL = vLength();
  // if the 1D effect is using virtual strips "i" will have virtual strip id stored in upper 16 bits
  // in such case "i" will be > virtualLength()
  if (i >= vL) {
    // check if this is a virtual strip
    #ifndef WLED_DISABLE_2D
    vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows)
    i &= 0xFFFF;    //truncate vstrip index
    if (i >= vL) return;  // if pixel would still fall out of segment just exit
    #else
    return;
    #endif
  }

  // #ifndef ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG
  // // // Apply brightness if needed
  // // if (!flag_brightness_already_applied) {
  //   uint8_t brightness = scale8(_brightness_rgb, pCONT_iLight->getBriRGB_Global());
  //   uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range
  //   // Extract, scale, and repack in one step
  //   col = RGBW32(
  //     (R(col) * scale) >> 8,  // Red
  //     (G(col) * scale) >> 8,  // Green
  //     (B(col) * scale) >> 8,  // Blue
  //     (W(col) * scale) >> 8   // White
  //   );
  // // }
  // #endif


#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
    const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
    // pre-scale color for all pixels
    col = color_fade(col, _segBri);
    _colorScaled = true;
    switch (map1D2D) {
      case M12_Pixels:
        // use all available pixels as a long strip
        setPixelColorXY(i % vW, i / vW, col);
        break;
      case M12_pBar:
        // expand 1D effect vertically or have it play on virtual strips
        if (vStrip > 0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
        else for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
        break;
      case M12_pArc:
        // expand in circular fashion from center
        if (i == 0)
          setPixelColorXY(0, 0, col);
        else {
          float r = i;
          float step = HALF_PI / (2.8284f * r + 4); // we only need (PI/4)/(r/sqrt(2)+1) steps
          for (float rad = 0.0f; rad <= (HALF_PI/2)+step/2; rad += step) {
            int x = roundf(sin_t(rad) * r);
            int y = roundf(cos_t(rad) * r);
            // exploit symmetry
            setPixelColorXY(x, y, col);
            setPixelColorXY(y, x, col);
          }
          // Bresenham’s Algorithm (may not fill every pixel)
          //int d = 3 - (2*i);
          //int y = i, x = 0;
          //while (y >= x) {
          //  setPixelColorXY(x, y, col);
          //  setPixelColorXY(y, x, col);
          //  x++;
          //  if (d > 0) {
          //    y--;
          //    d += 4 * (x - y) + 10;
          //  } else {
          //    d += 4 * x + 6;
          //  }
          //}
        }
        break;
      case M12_pCorner:
        for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
        for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
        break;
      case M12_sPinwheel: {
        // i = angle --> 0 - 296  (Big), 0 - 192  (Medium), 0 - 72 (Small)
        float centerX = roundf((vW-1) / 2.0f);
        float centerY = roundf((vH-1) / 2.0f);
        float angleRad = getPinwheelAngle(i, vW, vH); // angle in radians
        float cosVal = cos_t(angleRad);
        float sinVal = sin_t(angleRad);

        // avoid re-painting the same pixel
        int lastX = INT_MIN; // impossible position
        int lastY = INT_MIN; // impossible position
        // draw line at angle, starting at center and ending at the segment edge
        // we use fixed point math for better speed. Starting distance is 0.5 for better rounding
        // int_fast16_t and int_fast32_t types changed to int, minimum bits commented
        int posx = (centerX + 0.5f * cosVal) * Fixed_Scale; // X starting position in fixed point 18 bit
        int posy = (centerY + 0.5f * sinVal) * Fixed_Scale; // Y starting position in fixed point 18 bit
        int inc_x = cosVal * Fixed_Scale; // X increment per step (fixed point) 10 bit
        int inc_y = sinVal * Fixed_Scale; // Y increment per step (fixed point) 10 bit

        int32_t maxX = vW * Fixed_Scale; // X edge in fixedpoint
        int32_t maxY = vH * Fixed_Scale; // Y edge in fixedpoint

        // Odd rays start further from center if prevRay started at center.
        static int prevRay = INT_MIN; // previous ray number
        if ((i % 2 == 1) && (i - 1 == prevRay || i + 1 == prevRay)) {
          int jump = min(vW/3, vH/3); // can add 2 if using medium pinwheel
          posx += inc_x * jump;
          posy += inc_y * jump;
        }
        prevRay = i;

        // draw ray until we hit any edge
        while ((posx >= 0) && (posy >= 0) && (posx < maxX)  && (posy < maxY))  {
          // scale down to integer (compiler will replace division with appropriate bitshift)
          int x = posx / Fixed_Scale;
          int y = posy / Fixed_Scale;
          // set pixel
          if (x != lastX || y != lastY) setPixelColorXY(x, y, col);  // only paint if pixel position is different
          lastX = x;
          lastY = y;
          // advance to next position
          posx += inc_x;
          posy += inc_y;
        }
        break;
      }
    }
    _colorScaled = false;
    return;
  } else if (Segment::maxHeight != 1 && (width() == 1 || height() == 1)) {
    if (start < Segment::maxWidth*Segment::maxHeight) {
      // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
      int x = 0, y = 0;
      if (vHeight() > 1) y = i;
      if (vWidth()  > 1) x = i;
      setPixelColorXY(x, y, col);
      return;
    }
  }
#endif

  unsigned len = length();
  // if color is unscaled
  if (!_colorScaled) col = color_fade(col, _brightness_rgb);

  // expand pixel (taking into account start, grouping, spacing [and offset])
  i = i * groupLength();
  if (reverse) { // is segment reversed?
    if (mirror) { // is segment mirrored?
      i = (len - 1) / 2 - i;  //only need to index half the pixels
    } else {
      i = (len - 1) - i;
    }
  }
  i += start; // starting pixel in a group

  uint32_t tmpCol = col;
  // set all the pixels in the group
  for (int j = 0; j < grouping; j++) {
    unsigned indexSet = i + ((reverse) ? -j : j);
    if (indexSet >= start && indexSet < stop) {
      if (mirror) { //set the corresponding mirrored pixel
        unsigned indexMir = stop - indexSet + start - 1;
        indexMir += offset; // offset/phase
        if (indexMir >= stop) indexMir -= len; // wrap
#ifndef WLED_DISABLE_MODE_BLEND
        // if (_modeBlend) tmpCol = color_blend16(tkr_anim->getPixelColor(indexMir), col, uint16_t(0xFFFFU - progress()));
#endif
        tkr_anim->setPixelColor(indexMir, tmpCol);
      }
      indexSet += offset; // offset/phase
      if (indexSet >= stop) indexSet -= len; // wrap
#ifndef WLED_DISABLE_MODE_BLEND
      // if (_modeBlend) tmpCol = color_blend16(tkr_anim->getPixelColor(indexSet), col, uint16_t(0xFFFFU - progress()));
#endif
      #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
      Serial.printf("::Segment::setPixelColor(int i, tmpCol %d,%d,%d\n\r", tmpCol.R, tmpCol.G, tmpCol.B);
      #endif
      tkr_anim->setPixelColor(indexSet, tmpCol);
    }
  }
}

#endif



#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
void IRAM_ATTR mAnimatorLight::Segment::setPixelColor(int i, RgbwwColor col)//, bool flag_brightness_already_applied)
{

  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
  Serial.printf("::Segment::setPixelColor(int %D, RgbwwColor col %d,%d,%d,%d,%d\n\r", i, col.R, col.G, col.B, col.WW, col.CW);
  #endif

  DEBUG_LINE_HERE;
  if (!isActive() || i < 0) return; // not active or invalid index
#ifndef WLED_DISABLE_2D
  int vStrip = 0;
#endif
  int vL = vLength();
  // if the 1D effect is using virtual strips "i" will have virtual strip id stored in upper 16 bits
  // in such case "i" will be > virtualLength()
  if (i >= vL) {
    // check if this is a virtual strip
    #ifndef WLED_DISABLE_2D
    vStrip = i>>16; // hack to allow running on virtual strips (2D segment columns/rows)
    i &= 0xFFFF;    //truncate vstrip index
    if (i >= vL) return;  // if pixel would still fall out of segment just exit
    #else
    return;
    #endif
  }

  // // Apply brightness if needed
  // if (!flag_brightness_already_applied) {
  //   uint8_t brightness = scale8(_brightness_rgb, pCONT_iLight->getBriRGB_Global());
  //   uint16_t scale = brightness + 1;  // Avoid division by zero and maintain full range
  //   // Extract, scale, and repack in one step
  //   col = RGBW32(
  //     (R(col) * scale) >> 8,  // Red
  //     (G(col) * scale) >> 8,  // Green
  //     (B(col) * scale) >> 8,  // Blue
  //     (W(col) * scale) >> 8   // White
  //   );
  // }


#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (is2D()) {
    const int vW = vWidth();   // segment width in logical pixels (can be 0 if segment is inactive)
    const int vH = vHeight();  // segment height in logical pixels (is always >= 1)
    // pre-scale color for all pixels
    col = color_fade(col, _segBri);
    _colorScaled = true;
    switch (map1D2D) {
      case M12_Pixels:
        // use all available pixels as a long strip
        setPixelColorXY(i % vW, i / vW, col);
        break;
      case M12_pBar:
        // expand 1D effect vertically or have it play on virtual strips
        if (vStrip > 0) setPixelColorXY(vStrip - 1, vH - i - 1, col);
        else for (int x = 0; x < vW; x++) setPixelColorXY(x, vH - i - 1, col);
        break;
      case M12_pArc:
        // expand in circular fashion from center
        if (i == 0)
          setPixelColorXY(0, 0, col);
        else {
          float r = i;
          float step = HALF_PI / (2.8284f * r + 4); // we only need (PI/4)/(r/sqrt(2)+1) steps
          for (float rad = 0.0f; rad <= (HALF_PI/2)+step/2; rad += step) {
            int x = roundf(sin_t(rad) * r);
            int y = roundf(cos_t(rad) * r);
            // exploit symmetry
            setPixelColorXY(x, y, col);
            setPixelColorXY(y, x, col);
          }
          // Bresenham’s Algorithm (may not fill every pixel)
          //int d = 3 - (2*i);
          //int y = i, x = 0;
          //while (y >= x) {
          //  setPixelColorXY(x, y, col);
          //  setPixelColorXY(y, x, col);
          //  x++;
          //  if (d > 0) {
          //    y--;
          //    d += 4 * (x - y) + 10;
          //  } else {
          //    d += 4 * x + 6;
          //  }
          //}
        }
        break;
      case M12_pCorner:
        for (int x = 0; x <= i; x++) setPixelColorXY(x, i, col);
        for (int y = 0; y <  i; y++) setPixelColorXY(i, y, col);
        break;
      case M12_sPinwheel: {
        // i = angle --> 0 - 296  (Big), 0 - 192  (Medium), 0 - 72 (Small)
        float centerX = roundf((vW-1) / 2.0f);
        float centerY = roundf((vH-1) / 2.0f);
        float angleRad = getPinwheelAngle(i, vW, vH); // angle in radians
        float cosVal = cos_t(angleRad);
        float sinVal = sin_t(angleRad);

        // avoid re-painting the same pixel
        int lastX = INT_MIN; // impossible position
        int lastY = INT_MIN; // impossible position
        // draw line at angle, starting at center and ending at the segment edge
        // we use fixed point math for better speed. Starting distance is 0.5 for better rounding
        // int_fast16_t and int_fast32_t types changed to int, minimum bits commented
        int posx = (centerX + 0.5f * cosVal) * Fixed_Scale; // X starting position in fixed point 18 bit
        int posy = (centerY + 0.5f * sinVal) * Fixed_Scale; // Y starting position in fixed point 18 bit
        int inc_x = cosVal * Fixed_Scale; // X increment per step (fixed point) 10 bit
        int inc_y = sinVal * Fixed_Scale; // Y increment per step (fixed point) 10 bit

        int32_t maxX = vW * Fixed_Scale; // X edge in fixedpoint
        int32_t maxY = vH * Fixed_Scale; // Y edge in fixedpoint

        // Odd rays start further from center if prevRay started at center.
        static int prevRay = INT_MIN; // previous ray number
        if ((i % 2 == 1) && (i - 1 == prevRay || i + 1 == prevRay)) {
          int jump = min(vW/3, vH/3); // can add 2 if using medium pinwheel
          posx += inc_x * jump;
          posy += inc_y * jump;
        }
        prevRay = i;

        // draw ray until we hit any edge
        while ((posx >= 0) && (posy >= 0) && (posx < maxX)  && (posy < maxY))  {
          // scale down to integer (compiler will replace division with appropriate bitshift)
          int x = posx / Fixed_Scale;
          int y = posy / Fixed_Scale;
          // set pixel
          if (x != lastX || y != lastY) setPixelColorXY(x, y, col);  // only paint if pixel position is different
          lastX = x;
          lastY = y;
          // advance to next position
          posx += inc_x;
          posy += inc_y;
        }
        break;
      }
    }
    _colorScaled = false;
    return;
  } else if (Segment::maxHeight != 1 && (width() == 1 || height() == 1)) {
    if (start < Segment::maxWidth*Segment::maxHeight) {
      // we have a vertical or horizontal 1D segment (WARNING: virtual...() may be transposed)
      int x = 0, y = 0;
      if (vHeight() > 1) y = i;
      if (vWidth()  > 1) x = i;
      setPixelColorXY(x, y, col);
      return;
    }
  }
#endif

  unsigned len = length();
  // if color is unscaled
  if (!_colorScaled) col = color_fade(col, _brightness_rgb);

  // expand pixel (taking into account start, grouping, spacing [and offset])
  i = i * groupLength();
  if (reverse) { // is segment reversed?
    if (mirror) { // is segment mirrored?
      i = (len - 1) / 2 - i;  //only need to index half the pixels
    } else {
      i = (len - 1) - i;
    }
  }
  i += start; // starting pixel in a group

  RgbwwColor tmpCol = col;
  // set all the pixels in the group
  for (int j = 0; j < grouping; j++) {
    unsigned indexSet = i + ((reverse) ? -j : j);
    if (indexSet >= start && indexSet < stop) {
      if (mirror) { //set the corresponding mirrored pixel
        unsigned indexMir = stop - indexSet + start - 1;
        indexMir += offset; // offset/phase
        if (indexMir >= stop) indexMir -= len; // wrap
#ifndef WLED_DISABLE_MODE_BLEND
        // if (_modeBlend) tmpCol = color_blend16(tkr_anim->getPixelColor(indexMir), col, uint16_t(0xFFFFU - progress()));
#endif
        tkr_anim->setPixelColor(indexMir, tmpCol);
      }
      indexSet += offset; // offset/phase
      if (indexSet >= stop) indexSet -= len; // wrap
#ifndef WLED_DISABLE_MODE_BLEND
      // if (_modeBlend) tmpCol = color_blend16(tkr_anim->getPixelColor(indexSet), col, uint16_t(0xFFFFU - progress()));
#endif
      #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
      Serial.printf("::Segment::setPixelColor(int i, tmpCol %d,%d,%d\n\r", tmpCol.R, tmpCol.G, tmpCol.B);
      #endif
      tkr_anim->setPixelColor(indexSet, tmpCol);
    }
  }
}
#endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

/*
 * color blend function
 */
uint32_t mAnimatorLight::Segment::color_blend(uint32_t color1, uint32_t color2, uint16_t blend, bool b16) {
  if(blend == 0)   return color1;
  uint16_t blendmax = b16 ? 0xFFFF : 0xFF;
  if(blend == blendmax) return color2;
  uint8_t shift = b16 ? 16 : 8;

  uint32_t w1 = W(color1);
  uint32_t r1 = R(color1);
  uint32_t g1 = G(color1);
  uint32_t b1 = B(color1);

  uint32_t w2 = W(color2);
  uint32_t r2 = R(color2);
  uint32_t g2 = G(color2);
  uint32_t b2 = B(color2);

  uint32_t w3 = ((w2 * blend) + (w1 * (blendmax - blend))) >> shift;
  uint32_t r3 = ((r2 * blend) + (r1 * (blendmax - blend))) >> shift;
  uint32_t g3 = ((g2 * blend) + (g1 * (blendmax - blend))) >> shift;
  uint32_t b3 = ((b2 * blend) + (b1 * (blendmax - blend))) >> shift;

  return RGBW32(r3, g3, b3, w3);
}


RgbwwColor mAnimatorLight::Segment::color_blend(RgbwwColor color1, RgbwwColor color2, uint16_t blend, bool b16) {
  if(blend == 0)   return color1;
  uint16_t blendmax = b16 ? 0xFFFF : 0xFF;
  if(blend == blendmax) return color2;
  uint8_t shift = b16 ? 16 : 8;

  return RgbwwColor(
    ((color2.R * blend) + (color1.R * (blendmax - blend))) >> shift,
    ((color2.G * blend) + (color1.G * (blendmax - blend))) >> shift,
    ((color2.B * blend) + (color1.B * (blendmax - blend))) >> shift,
    ((color2.WW * blend) + (color1.WW * (blendmax - blend))) >> shift,
    ((color2.CW * blend) + (color1.CW * (blendmax - blend))) >> shift
  );

  // uint32_t w1 = W(color1);
  // uint32_t r1 = R(color1);
  // uint32_t g1 = G(color1);
  // uint32_t b1 = B(color1);

  // uint32_t w2 = W(color2);
  // uint32_t r2 = R(color2);
  // uint32_t g2 = G(color2);
  // uint32_t b2 = B(color2);

  // uint32_t w3 = ((w2 * blend) + (w1 * (blendmax - blend))) >> shift;
  // uint32_t r3 = ((r2 * blend) + (r1 * (blendmax - blend))) >> shift;
  // uint32_t g3 = ((g2 * blend) + (g1 * (blendmax - blend))) >> shift;
  // uint32_t b3 = ((b2 * blend) + (b1 * (blendmax - blend))) >> shift;

  // return RGBW32(r3, g3, b3, w3);
}

/*
 * color add function that preserves ratio
 * idea: https://github.com/Aircoookie/WLED/pull/2465 by https://github.com/Proto-molecule
 */
// #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
uint32_t mAnimatorLight::Segment::color_add(RgbwwColor c1, RgbwwColor c2, bool fast)
{
  if (fast) {
    uint8_t r = c1.R;
    uint8_t g = c1.G;
    uint8_t b = c1.B;
    uint8_t w = c1.WW;
    r = qadd8(r, c2.R);
    g = qadd8(g, c2.G);
    b = qadd8(b, c2.B);
    w = qadd8(w, c2.WW);
    return RGBW32(r, g, b, w);
  } else {
    uint32_t r = c1.R + c2.R;
    uint32_t g = c1.G + c2.G;
    uint32_t b = c1.B + c2.B;
    uint32_t w = c1.WW + c2.WW;
    uint16_t max = r;
    if (g > max) max = g;
    if (b > max) max = b;
    if (w > max) max = w;
    if (max < 256) return RGBW32(r, g, b, w);
    else           return RGBW32(r * 255 / max, g * 255 / max, b * 255 / max, w * 255 / max);
  }
}
// #else
uint32_t mAnimatorLight::Segment::color_add(uint32_t c1, uint32_t c2, bool fast)
{
  if (fast) {
    uint8_t r = R(c1);
    uint8_t g = G(c1);
    uint8_t b = B(c1);
    uint8_t w = W(c1);
    r = qadd8(r, R(c2));
    g = qadd8(g, G(c2));
    b = qadd8(b, B(c2));
    w = qadd8(w, W(c2));
    return RGBW32(r,g,b,w);
  } else {
    uint32_t r = R(c1) + R(c2);
    uint32_t g = G(c1) + G(c2);
    uint32_t b = B(c1) + B(c2);
    uint32_t w = W(c1) + W(c2);
    uint16_t max = r;
    if (g > max) max = g;
    if (b > max) max = b;
    if (w > max) max = w;
    if (max < 256) return RGBW32(r, g, b, w);
    else           return RGBW32(r * 255 / max, g * 255 / max, b * 255 / max, w * 255 / max);
  }
}
// #endif

/*
 * fades color toward black
 * if using "video" method the resulting color will never become black unless it is already black
 */
#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
RgbwwColor mAnimatorLight::Segment::color_fade(RgbwwColor c1, uint8_t amount, bool video)
{
  
  return c1;// tmp fix



  uint8_t r = c1.R;
  uint8_t g = c1.G;
  uint8_t b = c1.B;
  uint8_t w = c1.WW;
  uint8_t cw = c1.CW;
  if (video) {
    r = scale8_video(r, amount);
    g = scale8_video(g, amount);
    b = scale8_video(b, amount);
    w = scale8_video(w, amount);
    cw = scale8_video(cw, amount);
  } else {
    r = scale8(r, amount);
    g = scale8(g, amount);
    b = scale8(b, amount);
    w = scale8(w, amount);
    cw = scale8(cw, amount);
  }
  return RgbwwColor(r, g, b, w, cw);
}
uint32_t mAnimatorLight::Segment::color_fade(uint32_t c1, uint8_t amount, bool video)
{
  
  return c1;// tmp fix


  uint8_t r = R(c1);
  uint8_t g = G(c1);
  uint8_t b = B(c1);
  uint8_t w = W(c1);
  if (video) {
    r = scale8_video(r, amount);
    g = scale8_video(g, amount);
    b = scale8_video(b, amount);
    w = scale8_video(w, amount);
  } else {
    r = scale8(r, amount);
    g = scale8(g, amount);
    b = scale8(b, amount);
    w = scale8(w, amount);
  }
  return RGBW32(r, g, b, w);
}
#else


RgbwwColor mAnimatorLight::Segment::color_fade(RgbwwColor c1, uint8_t amount, bool video)
{
  
  // return c1;// tmp fix



  uint8_t r = c1.R;
  uint8_t g = c1.G;
  uint8_t b = c1.B;
  uint8_t w = c1.WW;
  uint8_t cw = c1.CW;
  if (video) {
    r = scale8_video(r, amount);
    g = scale8_video(g, amount);
    b = scale8_video(b, amount);
    w = scale8_video(w, amount);
    cw = scale8_video(cw, amount);
  } else {
    r = scale8(r, amount);
    g = scale8(g, amount);
    b = scale8(b, amount);
    w = scale8(w, amount);
    cw = scale8(cw, amount);
  }
  return RgbwwColor(r, g, b, w, cw);
}
uint32_t mAnimatorLight::Segment::color_fade(uint32_t c1, uint8_t amount, bool video)
{
  
  // return c1;// tmp fix


  uint8_t r = R(c1);
  uint8_t g = G(c1);
  uint8_t b = B(c1);
  uint8_t w = W(c1);
  if (video) {
    r = scale8_video(r, amount);
    g = scale8_video(g, amount);
    b = scale8_video(b, amount);
    w = scale8_video(w, amount);
  } else {
    r = scale8(r, amount);
    g = scale8(g, amount);
    b = scale8(b, amount);
    w = scale8(w, amount);
  }
  return RGBW32(r, g, b, w);
}


#endif


#define modd(x, y) ((x) - (int)((x) / (y)) * (y))

float mAnimatorLight::cos_t(float phi)
{
  float x = modd(phi, TWO_PI);
  if (x < 0) x = -1 * x;
  int8_t sign = 1;
  if (x > PI)
  {
      x -= PI;
      sign = -1;
  }
  float xx = x * x;

  float res = sign * (1 - ((xx) / (2)) + ((xx * xx) / (24)) - ((xx * xx * xx) / (720)) + ((xx * xx * xx * xx) / (40320)) - ((xx * xx * xx * xx * xx) / (3628800)) + ((xx * xx * xx * xx * xx * xx) / (479001600)));
  #ifdef WLED_DEBUG_MATH
  Serial.printf("cos: %f,%f,%f,(%f)\n",phi,res,cos(x),res-cos(x));
  #endif
  return res;
}

float mAnimatorLight::sin_t(float x) {
  float res =  cos_t(HALF_PI - x);
  #ifdef WLED_DEBUG_MATH
  Serial.printf("sin: %f,%f,%f,(%f)\n",x,res,sin(x),res-sin(x));
  #endif
  return res;
}

float mAnimatorLight::tan_t(float x) {
  float c = cos_t(x);
  if (c==0.0f) return 0;
  float res = sin_t(x) / c;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("tan: %f,%f,%f,(%f)\n",x,res,tan(x),res-tan(x));
  #endif
  return res;
}

//https://stackoverflow.com/questions/3380628
// Absolute error <= 6.7e-5
float mAnimatorLight::acos_t(float x) {
  float negate = float(x < 0);
  float xabs = std::abs(x);
  float ret = -0.0187293f;
  ret = ret * xabs;
  ret = ret + 0.0742610f;
  ret = ret * xabs;
  ret = ret - 0.2121144f;
  ret = ret * xabs;
  ret = ret + HALF_PI;
  ret = ret * sqrt(1.0f-xabs);
  ret = ret - 2 * negate * ret;
  float res = negate * PI + ret;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("acos: %f,%f,%f,(%f)\n",x,res,acos(x),res-acos(x));
  #endif
  return res;
}

float mAnimatorLight::asin_t(float x) {
  float res = HALF_PI - acos_t(x);
  #ifdef WLED_DEBUG_MATH
  Serial.printf("asin: %f,%f,%f,(%f)\n",x,res,asin(x),res-asin(x));
  #endif
  return res;
}

// // declare a template with no implementation, and only one specialization
// // this allows hiding the constants, while ensuring ODR causes optimizations
// // to still apply.  (Fixes issues with conflicting 3rd party #define's)
// template <typename T> T mAnimatorLight::atan_t(T x);
// template<>
// float mAnimatorLight::atan_t(float x) {
//   //For A/B/C, see https://stackoverflow.com/a/42542593
//   static const double A { 0.0776509570923569 };
//   static const double B { -0.287434475393028 };
//   static const double C { ((HALF_PI/2) - A - B) };
//   // polynominal factors for approximation between 1 and 5
//   static const float C0 {  0.089494f };
//   static const float C1 {  0.974207f };
//   static const float C2 { -0.326175f };
//   static const float C3 {  0.05375f  };
//   static const float C4 { -0.003445f };

//   #ifdef WLED_DEBUG_MATH
//   float xinput = x;
//   #endif
//   bool neg = (x < 0);
//   x = std::abs(x);
//   float res;
//   if (x > 5.0f) { // atan(x) converges to pi/2 - (1/x) for large values
//     res = HALF_PI - (1.0f/x);
//   } else if (x > 1.0f) { //1 < x < 5
//     float xx = x * x;
//     res = (C4*xx*xx)+(C3*xx*x)+(C2*xx)+(C1*x)+C0;
//   } else { // this approximation is only for x <= 1
//     float xx = x * x;
//     res = ((A*xx + B)*xx + C)*x;
//   }
//   if (neg) {
//     res = -res;
//   }
//   #ifdef WLED_DEBUG_MATH
//   Serial.printf("atan: %f,%f,%f,(%f)\n",xinput,res,atan(xinput),res-atan(xinput));
//   #endif
//   return res;
// }



//gamma 2.8 lookup table used for color correction
uint8_t mAnimatorLight::NeoGammaWLEDMethod::gammaT[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// re-calculates & fills gamma table
void mAnimatorLight::NeoGammaWLEDMethod::calcGammaTable(float gamma)
{
  for (size_t i = 0; i < 256; i++) {
    gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
  }
}

uint8_t mAnimatorLight::NeoGammaWLEDMethod::Correct(uint8_t value)
{
  // if (!gammaCorrectCol) return value;
  return gammaT[value];
}

// used for color gamma correction
uint32_t mAnimatorLight::NeoGammaWLEDMethod::Correct32(uint32_t color)
{
  // if (!gammaCorrectCol) return color;
  uint8_t w = W(color);
  uint8_t r = R(color);
  uint8_t g = G(color);
  uint8_t b = B(color);
  w = gammaT[w];
  r = gammaT[r];
  g = gammaT[g];
  b = gammaT[b];
  return RGBW32(r, g, b, w);
}

uint8_t mAnimatorLight::sin_gap(uint16_t in) {
  if (in & 0x100) return 0;
  return sin8_t(in + 192); // correct phase shift of sine so that it starts and stops at 0
}
uint16_t mAnimatorLight::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

/*
 * Generates a tristate square wave w/ attac & decay
 * @param x input value 0-255
 * @param pulsewidth 0-127
 * @param attdec attack & decay, max. pulsewidth / 2
 * @returns signed waveform value
 */
int8_t mAnimatorLight::tristate_square8(uint8_t x, uint8_t pulsewidth, uint8_t attdec) {
  int8_t a = 127;
  if (x > 127) {
    a = -127;
    x -= 127;
  }

  if (x < attdec) { //inc to max
    return (int16_t) x * a / attdec;
  }
  else if (x < pulsewidth - attdec) { //max
    return a;
  }
  else if (x < pulsewidth) { //dec to 0
    return (int16_t) (pulsewidth - x) * a / attdec;
  }
  return 0;
}



#ifdef ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD

///////////////////////////////////////////////////////////////////////////////
// Begin simulateSound (to enable audio enhanced effects to display something)
///////////////////////////////////////////////////////////////////////////////
// Currently 4 types defined, to be fine tuned and new types added
// (only 2 used as stored in 1 bit in segment options, consider switching to a single global simulation type)
typedef enum UM_SoundSimulations {
  UMS_BeatSin = 0,
  UMS_WeWillRockYou,
  UMS_10_13,
  UMS_14_3
} um_soundSimulations_t;

mAnimatorLight::um_data_t* mAnimatorLight::simulateSound(uint8_t simulationId)
{
  static uint8_t samplePeak;
  static float   FFT_MajorPeak;
  static uint8_t maxVol;
  static uint8_t binNum;

  static float    volumeSmth;
  static uint16_t volumeRaw;
  static float    my_magnitude;

  //arrays
  uint8_t *fftResult;

  static um_data_t* um_data = nullptr;

  if (!um_data) {
    //claim storage for arrays
    fftResult = (uint8_t *)malloc(sizeof(uint8_t) * 16);

    // initialize um_data pointer structure
    // NOTE!!!
    // This may change as AudioReactive usermod may change
    um_data = new um_data_t;
    um_data->u_size = 8;
    um_data->u_type = new um_types_t[um_data->u_size];
    um_data->u_data = new void*[um_data->u_size];
    um_data->u_data[0] = &volumeSmth;
    um_data->u_data[1] = &volumeRaw;
    um_data->u_data[2] = fftResult;
    um_data->u_data[3] = &samplePeak;
    um_data->u_data[4] = &FFT_MajorPeak;
    um_data->u_data[5] = &my_magnitude;
    um_data->u_data[6] = &maxVol;
    um_data->u_data[7] = &binNum;
  } else {
    // get arrays from um_data
    fftResult =  (uint8_t*)um_data->u_data[2];
  }

  uint32_t ms = millis();

  switch (simulationId) {
    default:
    case UMS_BeatSin:
      for (int i = 0; i<16; i++)
        fftResult[i] = beatsin8(120 / (i+1), 0, 255);
        // fftResult[i] = (beatsin8(120, 0, 255) + (256/16 * i)) % 256;
        volumeSmth = fftResult[8];
      break;
    case UMS_WeWillRockYou:
      if (ms%2000 < 200) {
        volumeSmth = random8(255);
        for (int i = 0; i<5; i++)
          fftResult[i] = random8(255);
      }
      else if (ms%2000 < 400) {
        volumeSmth = 0;
        for (int i = 0; i<16; i++)
          fftResult[i] = 0;
      }
      else if (ms%2000 < 600) {
        volumeSmth = random8(255);
        for (int i = 5; i<11; i++)
          fftResult[i] = random8(255);
      }
      else if (ms%2000 < 800) {
        volumeSmth = 0;
        for (int i = 0; i<16; i++)
          fftResult[i] = 0;
      }
      else if (ms%2000 < 1000) {
        volumeSmth = random8(255);
        for (int i = 11; i<16; i++)
          fftResult[i] = random8(255);
      }
      else {
        volumeSmth = 0;
        for (int i = 0; i<16; i++)
          fftResult[i] = 0;
      }
      break;
    case UMS_10_13:
      for (int i = 0; i<16; i++)
        fftResult[i] = inoise8(beatsin8(90 / (i+1), 0, 200)*15 + (ms>>10), ms>>3);
        volumeSmth = fftResult[8];
      break;
    case UMS_14_3:
      for (int i = 0; i<16; i++)
        fftResult[i] = inoise8(beatsin8(120 / (i+1), 10, 30)*10 + (ms>>14), ms>>3);
      volumeSmth = fftResult[8];
      break;
  }

  samplePeak    = random8() > 250;
  FFT_MajorPeak = 21 + (volumeSmth*volumeSmth) / 8.0f; // walk thru full range of 21hz...8200hz
  maxVol        = 31;  // this gets feedback fro UI
  binNum        = 8;   // this gets feedback fro UI
  volumeRaw = volumeSmth;
  my_magnitude = 10000.0f / 8.0f; //no idea if 10000 is a good value for FFT_Magnitude ???
  if (volumeSmth < 1 ) my_magnitude = 0.001f;             // noise gate closed - mute

  return um_data;
}

#endif // ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD


// similar to NeoPixelBus NeoGammaTableMethod but allows dynamic changes (superseded by NPB::NeoGammaDynamicTableMethod)
class NeoGammaWLEDMethod {
  public:
    static uint8_t Correct(uint8_t value);      // apply Gamma to single channel
    static uint32_t Correct32(uint32_t color);  // apply Gamma to RGBW32 color (WLED specific, not used by NPB)
    static void calcGammaTable(float gamma);    // re-calculates & fills gamma table
    static inline uint8_t rawGamma8(uint8_t val) { return gammaT[val]; }  // get value from Gamma table (WLED specific, not used by NPB)
  private:
    static uint8_t gammaT[];
};
#define gamma32(c) NeoGammaWLEDMethod::Correct32(c)


//gamma 2.8 lookup table used for color correction
uint8_t NeoGammaWLEDMethod::gammaT[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// re-calculates & fills gamma table
void NeoGammaWLEDMethod::calcGammaTable(float gamma)
{
  for (size_t i = 0; i < 256; i++) {
    gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
  }
}

uint8_t NeoGammaWLEDMethod::Correct(uint8_t value)
{
  if (!tkr_anim->gammaCorrectCol) return value;
  return gammaT[value];
}

// used for color gamma correction
uint32_t NeoGammaWLEDMethod::Correct32(uint32_t color)
{
  if (!tkr_anim->gammaCorrectCol) return color;
  uint8_t w = W(color);
  uint8_t r = R(color);
  uint8_t g = G(color);
  uint8_t b = B(color);
  w = gammaT[w];
  r = gammaT[r];
  g = gammaT[g];
  b = gammaT[b];
  return RGBW32(r, g, b, w);
}



float mAnimatorLight::floor_t(float x) {
  bool neg = x < 0;
  int val = x;
  if (neg) val--;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("floor: %f,%f,%f\n",x,(float)val,floor(x));
  #endif
  return val;
}

float mAnimatorLight::fmod_t(float num, float denom) {
  int tquot = num / denom;
  float res = num - tquot * denom;
  #ifdef WLED_DEBUG_MATH
  Serial.printf("fmod: %f,%f,(%f)\n",res,fmod(num,denom),res-fmod(num,denom));
  #endif
  return res;
}

/******************************************************************************************************************
 * mInterfaceLight_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mAnimatorLight::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    // JBI->Add("light_size_count", settings.light_size_count);

    // JBI->Add("BriRGB_Global",  getBriRGB_Global());
    // JBI->Add("BriCCT_Global",  getBriCCT_Global());

  return JBI->End();

}

/**
 * @brief Unlike debug_segments, only the useful info here
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
uint8_t mAnimatorLight::ConstructJSON_Segments(uint8_t json_level, bool json_appending)
{

  char buffer[100];

  JBI->Start();

    uint8_t seg_count = getSegmentsNum();

    JBI->Add("SegmentCount", seg_count);


    // JBI->Add("millis", millis());

    // for(uint8_t seg_i =0; seg_i < getSegmentsNum(); seg_i++)
    // {
    //   JBI->Add("Start", SEGMENT_I(seg_i).start);
    //   JBI->Add("Stop",  SEGMENT_I(seg_i).stop);
    //   JBI->Add("StartY", SEGMENT_I(seg_i).startY);
    //   JBI->Add("StopY",  SEGMENT_I(seg_i).stopY);
    //   JBI->Add("EffectMicros",   SEGMENT_I(seg_i).performance.effect_build_us);

    // }




    JBI->Add("Brightness_Master",    pCONT_iLight->getBri_Global());
    JBI->Add("BrightnessRGB_Master", pCONT_iLight->getBriRGB_Global());
    JBI->Add("BrightnessCCT_Master", pCONT_iLight->getBriCCT_Global());

  JBI->Add("FPS", getFps());

    seg_count = seg_count < 4 ? seg_count : 4; //limit memory overrun, or else later instead of reducing the seg count, reduce the data shared in another topic as overview

    for(uint8_t seg_i =0; seg_i < seg_count; seg_i++)
    {

      JBI->Object_Start_F("Segment%d", seg_i);

        JBI->Add("BrightnessRGB", SEGMENT_I(seg_i).getBrightnessRGB());
        JBI->Add("BrightnessCCT", SEGMENT_I(seg_i).getBrightnessCCT());
        
        JBI->Add("BrightnessRGB_wMaster", SEGMENT_I(seg_i).getBrightnessRGB_WithGlobalApplied());
        JBI->Add("BrightnessCCT_wMaster", SEGMENT_I(seg_i).getBrightnessCCT_WithGlobalApplied());
        
        JBI->Array_Start("PixelRange");
          JBI->Add(SEGMENT_I(seg_i).start);
          JBI->Add(SEGMENT_I(seg_i).stop);
          JBI->Add(SEGMENT_I(seg_i).startY);
          JBI->Add(SEGMENT_I(seg_i).stopY);
        JBI->Array_End();
        JBI->Add("Effect",    SEGMENT_I(seg_i).effect_id);
        JBI->Add("EffectName",    GetFlasherFunctionNamebyID( SEGMENT_I(seg_i).effect_id , buffer, sizeof(buffer), true) );
        JBI->Add("Offset",    SEGMENT_I(seg_i).offset);
        JBI->Add("Speed",     SEGMENT_I(seg_i).speed);
        JBI->Add("Intensity", SEGMENT_I(seg_i).intensity);
        JBI->Object_Start("Options");
          JBI->Add("Selected",     SEGMENT_I(seg_i).selected);
          JBI->Add("Reverse",      SEGMENT_I(seg_i).reverse);
          JBI->Add("On",           SEGMENT_I(seg_i).on);
          JBI->Add("Mirror",       SEGMENT_I(seg_i).mirror);
          JBI->Add("Freeze",       SEGMENT_I(seg_i).freeze);
          if(seg_i<3)
          {
            JBI->Add("Reset",        SEGMENT_I(seg_i).reset);
            JBI->Add("Transitional", SEGMENT_I(seg_i).transitional);
            JBI->Add("Reverse_y",    SEGMENT_I(seg_i).reverse_y);
            JBI->Add("Mirror_y",     SEGMENT_I(seg_i).mirror_y);
            JBI->Add("Transpose",    SEGMENT_I(seg_i).transpose);
            JBI->Add("Map1D2D",      SEGMENT_I(seg_i).map1D2D);
            JBI->Add("SoundSim",     SEGMENT_I(seg_i).soundSim);
          }
        JBI->Object_End();
        JBI->Add("ColourType",     (uint8_t)SEGMENT_I(seg_i).colour_width__used_in_effect_generate);
        JBI->Object_Start("Transition");
          JBI->Add("Rate",         SEGMENT_I(seg_i).cycle_time__rate_ms);
          JBI->Add("Time",         SEGMENT_I(seg_i).animator_blend_time_ms() );
        JBI->Object_End();
        JBI->Object_Start("RgbcctColours");
        for(uint8_t rgb_i = 0; rgb_i<2; rgb_i++)
        {
          JBI->Array_Start_P("Colour%d", rgb_i);
          for(uint8_t c_i=0;c_i<5;c_i++)
          {
            // JBI->Add(SEGMENT_I(seg_i).segcol[rgb_i].raw[c_i]);
          }
          JBI->Array_End();
          JBI->Object_Start("ColourTemp");
            // JBI->Add("Min",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMin());
            // JBI->Add("Max",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMax());
            // JBI->Add("Set",      SEGMENT_I(seg_i).segcol[rgb_i].getCCT());
          JBI->Object_End();
        }
        JBI->Object_End();
          
      JBI->Object_End();

    } // END seg_i





  return JBI->End();

}

/**
 * @brief Unlike debug_segments, only the useful info here
 * 
 * @param json_level 
 * @param json_appending 
 * @return uint8_t 
 */
#ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
uint8_t mAnimatorLight::ConstructJSON_Matrix(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("isMatrix", isMatrix);
    JBI->Add("panels", panels);

    // for(uint8_t seg_i =0; seg_i < getSegmentsNum(); seg_i++)
    // {
    //   JBI->Add("Start", SEGMENT_I(seg_i).start);
    //   JBI->Add("Stop",  SEGMENT_I(seg_i).stop);
    //   JBI->Add("EffectMicros",   SEGMENT_I(seg_i).performance.effect_build_us);

    // }

    uint8_t i = 0;
    JBI->Object_Start_F("Panel");
    for (Panel p : panel)
    {
      JBI->Object_Start_F("%d", i++);
        JBI->Add("Width", p.width);
        JBI->Add("Height", p.height);
        JBI->Add("xOffset", p.xOffset);
        JBI->Add("yOffset", p.yOffset);
        JBI->Add("bottomStart", p.bottomStart);
        JBI->Add("rightStart", p.rightStart);
        JBI->Add("vertical", p.vertical);
        JBI->Add("serpentine", p.serpentine);
      JBI->Object_End();
    }
    JBI->Object_End();







  return JBI->End();

}
#endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  



uint8_t mAnimatorLight::ConstructJSON_Playlist(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
    JBI->Add("Length", playlistLen);

    JBI->Array_Start("Loaded");
    for(int i=0;i<playlistLen;i++)
    {
      JBI->Object_Start();
        JBI->Add("preset",  playlistEntries[i].preset);
        JBI->Add("dur",     playlistEntries[i].dur);
        JBI->Add("tr",      playlistEntries[i].tr);
      JBI->Object_End();
    }
    JBI->Array_End();
    #endif 




    JBI->Add("millis", millis());

  return JBI->End();

}



#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
uint8_t mAnimatorLight::ConstructJSON_Mode_Ambilight(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    JBI->Add("millis", millis());

  return JBI->End();

}
#endif


#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
uint8_t mAnimatorLight::ConstructJSON_Mode_SetManual(uint8_t json_level, bool json_appending)
{

JBI->Start();

  JBI->Add("millis", millis());

return JBI->End();

}
#endif


/**
 * @brief Debug 
 */
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette(uint8_t json_level, bool json_appending)
{

char buffer[100];

JBI->Start();

    JBI->Add("AvailablePalettes", (uint16_t)mPaletteI->GetPaletteListLength() );

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    for(uint8_t seg_i = 0; seg_i<  segments.size(); seg_i++)
    {
      JBI->Object_Start_F("Segment%d",seg_i);      
        JBI->Add("dataLen", SEGMENT_I(seg_i).palette_container->pData.size());
        JBI->Array_Start("data");
        for(uint8_t i=0;i<SEGMENT_I(seg_i).palette_container->pData.size();i++)
        {
          JBI->Add(SEGMENT_I(seg_i).palette_container->pData[i]);
        }
        JBI->Array_End();
      JBI->Object_End();
    }
    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER

    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    uint8_t seg_i = 0;

    if(segments.size())
    {

      JBI->Add("PaletteSize", mPaletteI->GetColoursInCRGB16Palette(SEGMENT.palette_id) );
      JBI->Array_Start("CRGB16Palette16");   

      for(uint8_t elem_i=0;elem_i<16;elem_i++)
      {
        JBI->Array_Start();
          JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].r );
          JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].g );
          JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].b );
        JBI->Array_End();          
      }
      JBI->Array_End();

      JBI->Array_Start("CRGB16Palette16MAN");
      for(uint8_t elem_i=0;elem_i<16;elem_i++)
      {
        JBI->Array_Start();
          JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].r );
          JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].g );
          JBI->Add( SEGMENT_I(seg_i).palette_container->CRGB16Palette16_Palette.data[elem_i].b );
        JBI->Array_End();          
      }
      JBI->Array_End();

    }
    #endif// ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    JBI->Object_Start("Encoding");
      for(uint8_t palette_id=mPalette::PALETTELIST_STATIC_HOLLOWEEN_OP__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_01__ID;palette_id++)
      {
        // JBI->Array_Start_P("%s", mPaletteI->GetPaletteNameByID( palette_id, buffer, sizeof(buffer) ));
        JBI->Array_Start_P("P_%d", palette_id );        
          mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);
          uint16_t value = ptr->encoding.data;
          char buffer[33] = {0}; //null terminated
          for(uint8_t i=0;i<16;i++)
          {
            buffer[15-i] = bitRead(value,i) ? '1' : '0';
          }
          JBI->Add(buffer);
        JBI->Array_End();
      }
    JBI->Object_End();
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING


    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    JBI->Array_Start_P("DataLength");
      for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++)
      {
        mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);
        JBI->Add(ptr->data_length);
      }
    JBI->Array_End();
    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH

    // #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // JBI->Array_Start_P("PaletteNames");
    //   for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;palette_id++) // Some
    //   // for(uint16_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_VARIABLE_GENERIC_LENGTH__ID;palette_id++) // All
    //   {
    //     JBI->Add(GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
    //     // ALOG_INF(PSTR("Name[%d] = %s"),palette_id,GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
    //   }
    // JBI->Array_End();
    // #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    JBI->Array_Start_P("PaletteNames");
      for(uint16_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_STATIC_CHRISTMAS_28__ID;palette_id++) // Some
      // for(uint8_t palette_id=mPalette::PALETTELIST_VARIABLE_HSBID_01__ID;palette_id<mPaletteI->PALETTELIST_TOTAL_LENGTH;palette_id++) // All
      {
        // Option 
        // ALOG_INF(PSTR("Name[%d] = %s"),GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));

        // JBI->Add(GetPaletteNameByID(palette_id, buffer, sizeof(buffer)));
      }
    JBI->Array_End();
    #endif  // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST


    uint16_t id = 0;
    uint16_t pixel = 0;
    uint8_t encoded_value = 0;
    
    // // uint16_t count   = mPaletteI->GetLengthFromPaletteAdvanced(id,pixel,&encoded_value,true,true,255);
    // RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(id,pixel,&encoded_value,true,true,255);

    // JBI->Array_Start("Palette");
    // for(int i=0;i<MAX_NUM_SEGMENTS;i++)
    // {
    //   JBI->Add(encoded_value);
    //   JBI->Add(colour.R);
    //   JBI->Add(colour.G);
    //   JBI->Add(colour.B);
    //   JBI->Add(colour.W1);
    //   JBI->Add(colour.W2);
    // }
    // JBI->Array_End();
    
//   JBI->Start();  
//     JBI->Add_P(PM_SIZE, pCONT_iLight->settings.light_size_count);
//     JBI->Add("PaletteMaxID", (uint8_t)mPalette::PALETTELIST_STATIC_LENGTH__ID);
//     JBI->Add("ColourPaletteID", tkr_anim->SEGMENT_I(0).palette_id );
//     JBI->Add("ColourPalette", mPaletteI->GetPaletteNameByID( SEGMENT_I(0).palette_id, buffer, sizeof(buffer)));
//     // JBI->Array_Start("rgb");
//     // for(int i=0;i<numpixels;i++){
//     //   RgbTypeColor c = getPixelColor(i);
//     //   JBI->Add_FV(PSTR("%02X%02X%02X"),c.R,c.G,c.B);
//     // }
//     // JBI->Array_End();
//   return JBI->End();


    /**
     * @brief Moving towards preloading palettes from memory into ram/heap for speed (then iram will work)
     * 
     */
    JBI->Object_Start("LoadPalette");

      uint8_t segment_index = 0;

    JBI->Object_End();
    

return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
uint8_t mAnimatorLight::ConstructJSON_Debug__CustomMappingTable(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add("customMappingSize",    customMappingSize);

    if(customMappingSize)
    {
      JBI->Array_Start("MappingTable");
      uint16_t send_size = customMappingSize < 400 ? customMappingSize : 400;
      for(int i=0;i<send_size;i++)
      {
        JBI->Add(customMappingTable[i]);
      }
      JBI->Array_End();
    }

  return JBI->End();
}
#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS

/**
 * @brief Multiple large segments may require mutiple topics
 */
uint8_t mAnimatorLight::ConstructJSON_Debug_Segments(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  JBI->Add("Brightness_Master",    pCONT_iLight->getBri_Global());
  JBI->Add("BrightnessRGB_Master", pCONT_iLight->getBriRGB_Global());
  JBI->Add("BrightnessCCT_Master", pCONT_iLight->getBriCCT_Global());
  
  uint8_t seg_count = getSegmentsNum();
  seg_count = seg_count < 3 ? seg_count : 3; //limit memory overrun, or else later instead of reducing the seg count, reduce the data shared in another topic as overview

  for(uint8_t seg_i =0; seg_i < seg_count; seg_i++)
  {

    JBI->Object_Start_F("Segment%d", seg_i);

      JBI->Add("BrightnessRGB", SEGMENT_I(seg_i).getBrightnessRGB());
      JBI->Add("BrightnessCCT", SEGMENT_I(seg_i).getBrightnessCCT());
      
      JBI->Add("BrightnessRGB_wMaster", SEGMENT_I(seg_i).getBrightnessRGB_WithGlobalApplied());
      JBI->Add("BrightnessCCT_wMaster", SEGMENT_I(seg_i).getBrightnessCCT_WithGlobalApplied());
      
      JBI->Array_Start("PixelRange");
        JBI->Add(SEGMENT_I(seg_i).start);
        JBI->Add(SEGMENT_I(seg_i).stop);
        JBI->Add(SEGMENT_I(seg_i).startY);
        JBI->Add(SEGMENT_I(seg_i).stopY);
      JBI->Array_End();
      JBI->Add("Offset",    SEGMENT_I(seg_i).offset);
      JBI->Add("Speed",     SEGMENT_I(seg_i).speed);
      JBI->Add("Intensity", SEGMENT_I(seg_i).intensity);
      JBI->Object_Start("Options");
        JBI->Add("Selected",     SEGMENT_I(seg_i).selected);
        JBI->Add("Reverse",      SEGMENT_I(seg_i).reverse);
        JBI->Add("On",           SEGMENT_I(seg_i).on);
        JBI->Add("Mirror",       SEGMENT_I(seg_i).mirror);
        JBI->Add("Freeze",       SEGMENT_I(seg_i).freeze);
        JBI->Add("Reset",        SEGMENT_I(seg_i).reset);
        JBI->Add("Transitional", SEGMENT_I(seg_i).transitional);
        JBI->Add("Reverse_y",    SEGMENT_I(seg_i).reverse_y);
        JBI->Add("Mirror_y",     SEGMENT_I(seg_i).mirror_y);
        JBI->Add("Transpose",    SEGMENT_I(seg_i).transpose);
        JBI->Add("Map1D2D",      SEGMENT_I(seg_i).map1D2D);
        JBI->Add("SoundSim",     SEGMENT_I(seg_i).soundSim);
      JBI->Object_End();
      JBI->Add("ColourType",     (uint8_t)SEGMENT_I(seg_i).colour_width__used_in_effect_generate);
      JBI->Object_Start("Transition");
        JBI->Add("Rate",         SEGMENT_I(seg_i).cycle_time__rate_ms);
        JBI->Add("Time",         SEGMENT_I(seg_i).animator_blend_time_ms() );
      JBI->Object_End();
      JBI->Object_Start("RgbcctColours");
      for(uint8_t rgb_i = 0; rgb_i<2; rgb_i++)
      {
        JBI->Array_Start_P("Colour%d", rgb_i);
        for(uint8_t c_i=0;c_i<5;c_i++)
        {
          // JBI->Add(SEGMENT_I(seg_i).segcol[rgb_i].raw[c_i]);
        }
        JBI->Array_End();
        JBI->Object_Start("ColourTemp");
          // JBI->Add("Min",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMin());
          // JBI->Add("Max",      SEGMENT_I(seg_i).segcol[rgb_i].get_CTRangeMax());
          // JBI->Add("Set",      SEGMENT_I(seg_i).segcol[rgb_i].getCCT());
        JBI->Object_End();
      }
      JBI->Object_End();
        
    JBI->Object_End();

  } // END seg_i

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR

/**
 * @brief Multiple large segments may require mutiple topics
 */
uint8_t mAnimatorLight::ConstructJSON_Debug_Palette_Vector(uint8_t json_level, bool json_appending)
{

  JBI->Start();
  JBI->Add("size", mPaletteI->static_palettes.size());


    char lineBuffer[100] = {0};
    bool flag_get_first_name_only = true;

    JBI->Array_Start("pals");
    // for(uint8_t i=0;i<20;i++)//auto& pal: mPaletteI->palettelist)

    uint16_t i = 0;

    for(auto& pal: mPaletteI->static_palettes)
    {
      // mPalette::PALETTE pal = mPaletteI->palettelist[i];
      JBI->Object_Start();
          
          // tkr_anim->GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
          // // snprintf_P(lineBuffer,sizeof(lineBuffer),"%S",pal.friendly_name_ctr); 
          // if(flag_get_first_name_only)
          // {    
          //   char* dataPtr = strchr(lineBuffer,'|');
          //   if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
          // }
          // // ALOG_INF(PSTR("pal=\"%s\""), lineBuffer);
          // // ALOG_INF(PSTR("pal[%d]=\"%s\""), i, lineBuffer);
          // if(i<10)
          //   JBI->Add("n",lineBuffer);



        // JBI->Add("n", pal.friendly_name_ctr);
        // JBI->Add("i", pal.id);
        
        uint8_t colours_in_palette = tkr_anim->GetNumberOfColoursInPalette(i);
        JBI->Add("s",colours_in_palette);



    
        // JBI->Add_FV("e", "%4X", pal.encoding);
        // JBI->Array_Start("d");
        //   for(uint8_t i=0;i<pal.data_length;i++)
        //   {
        //     JBI->Add(pal.data[i]);
        //   }
        // JBI->Array_End();
      JBI->Object_End();

      i++;
    }
    JBI->Array_End();

  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR



#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
uint8_t mAnimatorLight::ConstructJSON_Debug_Performance(uint8_t json_level, bool json_appending)
{
  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE_FAST_MQTT_UPDATE
  mqtthandler_debug__performance.tRateSecs = 1; // Force update to 1 second
  #endif

  JBI->Start();  

    JBI->Add("targetFPS", getTargetFps() );
    JBI->Add("FPS", getFps());
    
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE


    // Time: Generate Pixels
    // JBI->Array_Start_P("EffectCall_us");
    // for (segment &seg : segments){ JBI->Add(seg.performance.effect_build_us); }
    // JBI->Array_End();
    JBI->Array_Start_P("EffectCall_ms");
    for (segment &seg : segments){ JBI->Add(seg.performance.effect_build_us/1000); }
    JBI->Array_End();


    // Time: Bus Writing Single
    JBI->Array_Start_P("BusWrite_us");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_single_us); }
    JBI->Array_End();
    JBI->Array_Start_P("BusRead_us");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_single_us); }
    JBI->Array_End();

    // JBI->Array_Start_P("BusWrite_ms");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_us/1000); }
    // JBI->Array_End();
    // JBI->Array_Start_P("BusRead_ms");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_us/1000); }
    // JBI->Array_End();


    // Time: Bus Writing Complate (From Static Palette Effect)
    // JBI->Array_Start_P("BusWriteTotal_us");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_total_us); }
    // JBI->Array_End();
    // JBI->Array_Start_P("BusReadTotal_us");
    // for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_total_us); }
    // JBI->Array_End();

    JBI->Array_Start_P("BusWriteTotal_ms");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_write_total_us/1000); }
    JBI->Array_End();
    JBI->Array_Start_P("BusReadTotal_ms");
    for (segment &seg : segments){ JBI->Add(seg.performance.bus_read_total_us/1000); }
    JBI->Array_End();
    #endif

  
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE    
    JBI->Array_Start_P("elapsed_last_show");
    for (segment &seg : segments){ JBI->Add(seg.performance.elapsed_last_show); }
    JBI->Array_End();
    JBI->Array_Start_P("millis_last_show");
    for (segment &seg : segments){ JBI->Add(seg.performance.millis_last_show); }
    JBI->Array_End();
    JBI->Array_Start_P("fps");
    for (segment &seg : segments){ JBI->Add(seg.performance.fps); }
    JBI->Array_End();
    #endif 

    // Show allocated data from segments
    JBI->Array_Start_P("dataSize");
    for (segment &seg : segments){ JBI->Add(seg.dataSize()); }
    JBI->Array_End();

    JBI->Array_Start_P("seglen");
    for (segment &seg : segments){ JBI->Add(seg.virtualLength()); }
    JBI->Array_End();


  return JBI->End();

}

#endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE




#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
  mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
uint8_t mAnimatorLight::ConstructJSON_Debug_Animations_Progress(uint8_t json_level, bool json_appending)
{

  if(anim_progress_mqtt_function_callback)
  {
    JBI->Start();
    anim_progress_mqtt_function_callback(); // Call the function
    return JBI->End();
  }
  return false;

}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


/******************************************************************************************************************
 * mInterfaceLight_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mAnimatorLight::MQTTHandler_Init()
{

  struct handler<mAnimatorLight>* ptr;
  
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_segments_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Segments;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_playlists_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Playlist;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  ptr = &mqtthandler_matrix_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MATRIX_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Matrix;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  ptr = &mqtthandler_mode_ambilight_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_Ambilight;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT

  #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  ptr = &mqtthandler_manual_setpixel;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    
  // #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  // ptr = &mqtthandler_automation_presets;
  // ptr->tSavedLastSent = 0;
  // ptr->flags.PeriodicEnabled = true;
  // ptr->flags.SendNow = true;
  // ptr->tRateSecs = 1;//pCONT_mqtt->dt.teleperiod_secs; 
  // ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  // ptr->json_level = JSON_LEVEL_DETAILED;
  // ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR;
  // ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Auto_Presets;
  // mqtthandler_list.push_back(ptr);
  // #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
  ptr = &mqtthandler_manual_setpixel;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  ptr = &mqtthandler_debug_palette;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  ptr = &mqtthandler_debug__custom_mapping_table;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  ptr                         = &mqtthandler_debug_segments;
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level             = JSON_LEVEL_DETAILED;
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_CUSTOM_MAPPING_TABLE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug__CustomMappingTable;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  ptr = &mqtthandler_debug_animations_progress;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Animations_Progress;
  mqtthandler_list.push_back(ptr);
  #endif

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
  ptr                         = &mqtthandler_debug_palette_vector;
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = 1; 
  ptr->topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level             = JSON_LEVEL_DETAILED;
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE_VECTOR__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette_Vector;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR


  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
  ptr                         = &mqtthandler_debug__performance;
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = 1; 
  ptr->topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level             = JSON_LEVEL_DETAILED;
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PERFORMANCE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Performance;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE

} 
  
#endif// USE_MODULE_NETWORK_MQTT



/*
 * UDP sync notifier / Realtime / Hyperion / TPM2.NET
 */

#define UDP_SEG_SIZE 36
#define SEG_OFFSET (41+(MAX_NUM_SEGMENTS*UDP_SEG_SIZE))
#define WLEDPACKETSIZE (41+(MAX_NUM_SEGMENTS*UDP_SEG_SIZE)+0)
#define UDP_IN_MAXSIZE 1472
#define PRESUMED_NETWORK_DELAY 3 //how many ms could it take on avg to reach the receiver? This will be added to transmitted times



void realtimeLock2(uint32_t timeoutMs, byte md);

#define WLED_GLOBAL
#define _INIT(x) = x


//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w = 0);

//scales the brightness with the briMultiplier factor
byte scaledBri2(byte in)
{
  uint16_t val = ((uint16_t)in*tkr_anim->briMultiplier)/100;
  if (val > 255) val = 255;
  return (byte)val;
}


void mAnimatorLight::notify(byte callMode, bool followUp)
{
  if (!tkr_anim->udpConnected) return;
  if (!tkr_anim->syncGroups) return;
  switch (callMode)
  {
    case CALL_MODE_INIT:          return;
    case CALL_MODE_DIRECT_CHANGE: if (!tkr_anim->notifyDirect) return; break;
    case CALL_MODE_BUTTON:        if (!tkr_anim->notifyButton) return; break;
    case CALL_MODE_BUTTON_PRESET: if (!tkr_anim->notifyButton) return; break;
    case CALL_MODE_NIGHTLIGHT:    if (!tkr_anim->notifyDirect) return; break;
    case CALL_MODE_HUE:           if (!tkr_anim->notifyHue)    return; break;
    case CALL_MODE_PRESET_CYCLE:  if (!tkr_anim->notifyDirect) return; break;
    case CALL_MODE_ALEXA:         if (!tkr_anim->notifyAlexa)  return; break;
    default: return;
  }
  byte udpOut[WLEDPACKETSIZE];
  mAnimatorLight::Segment& mainseg = tkr_anim->getMainSegment();
  udpOut[0] = 0; //0: wled notifier protocol 1: WARLS protocol
  udpOut[1] = callMode;
  udpOut[2] = pCONT_iLight->getBri_Global();
  uint32_t col = mainseg.segcol[0].getU32();
  udpOut[3] = R(col);
  udpOut[4] = G(col);
  udpOut[5] = B(col);
  udpOut[6] = tkr_anim->nightlightActive;
  udpOut[7] = tkr_anim->nightlightDelayMins;
  udpOut[8] = mainseg.effect_id;
  udpOut[9] = mainseg.speed;
  udpOut[10] = W(col);
  //compatibilityVersionByte:
  //0: old 1: supports white 2: supports secondary color
  //3: supports FX intensity, 24 byte packet 4: supports transitionDelay 5: sup palette
  //6: supports timebase syncing, 29 byte packet 7: supports tertiary color 8: supports sys time sync, 36 byte packet
  //9: supports sync groups, 37 byte packet 10: supports CCT, 39 byte packet 11: per segment options, variable packet length (40+MAX_NUM_SEGMENTS*3)
  //12: enhanced effct sliders, 2D & mapping options
  udpOut[11] = 12;
  col = mainseg.segcol[1].getU32();
  udpOut[12] = R(col);
  udpOut[13] = G(col);
  udpOut[14] = B(col);
  udpOut[15] = W(col);
  udpOut[16] = mainseg.intensity;
  udpOut[17] = (tkr_anim->transitionDelay >> 0) & 0xFF;
  udpOut[18] = (tkr_anim->transitionDelay >> 8) & 0xFF;
  udpOut[19] = mainseg.palette_id;
  col = mainseg.segcol[2].getU32();
  udpOut[20] = R(col);
  udpOut[21] = G(col);
  udpOut[22] = B(col);
  udpOut[23] = W(col);

  udpOut[24] = followUp;
  uint32_t t = millis() + tkr_anim->timebase;
  udpOut[25] = (t >> 24) & 0xFF;
  udpOut[26] = (t >> 16) & 0xFF;
  udpOut[27] = (t >>  8) & 0xFF;
  udpOut[28] = (t >>  0) & 0xFF;

  //sync system time
  udpOut[29] = tkr_anim->toki.getTimeSource();
  Toki::Time tm = tkr_anim->toki.getTime();
  uint32_t unix = tm.sec;
  udpOut[30] = (unix >> 24) & 0xFF;
  udpOut[31] = (unix >> 16) & 0xFF;
  udpOut[32] = (unix >>  8) & 0xFF;
  udpOut[33] = (unix >>  0) & 0xFF;
  uint16_t ms = tm.ms;
  udpOut[34] = (ms >> 8) & 0xFF;
  udpOut[35] = (ms >> 0) & 0xFF;

  //sync groups
  udpOut[36] = tkr_anim->syncGroups;

  //Might be changed to Kelvin in the future, receiver code should handle that case
  //0: byte 38 contains 0-255 value, 255: no valid CCT, 1-254: Kelvin value MSB
  udpOut[37] = tkr_anim->hasCCTBus() ? 0 : 255; //check this is 0 for the next value to be significant
  udpOut[38] = mainseg.cct_slider;

  udpOut[39] = tkr_anim->getActiveSegmentsNum();
  udpOut[40] = UDP_SEG_SIZE; //size of each loop iteration (one segment)
  size_t s = 0, nsegs = tkr_anim->getSegmentsNum();
  for (size_t i = 0; i < nsegs; i++) {
    mAnimatorLight::Segment &selseg = tkr_anim->getSegment(i);
    if (!selseg.isActive()) continue;
    uint16_t ofs = 41 + s*UDP_SEG_SIZE; //start of segment offset byte
    udpOut[0 +ofs] = s;
    udpOut[1 +ofs] = selseg.start >> 8;
    udpOut[2 +ofs] = selseg.start & 0xFF;
    udpOut[3 +ofs] = selseg.stop >> 8;
    udpOut[4 +ofs] = selseg.stop & 0xFF;
    udpOut[5 +ofs] = selseg.grouping_get();
    udpOut[6 +ofs] = selseg.spacing;
    udpOut[7 +ofs] = selseg.offset >> 8;
    udpOut[8 +ofs] = selseg.offset & 0xFF;
    udpOut[9 +ofs] = selseg.options & 0x8F; //only take into account selected, mirrored, on, reversed, reverse_y (for 2D); ignore freeze, reset, transitional
    udpOut[10+ofs] = 255;//selseg.opacity;
    udpOut[11+ofs] = selseg.effect_id;
    udpOut[12+ofs] = selseg.speed;
    udpOut[13+ofs] = selseg.intensity;
    udpOut[14+ofs] = selseg.palette_id;
    udpOut[15+ofs] = R(selseg.segcol[0].getU32());
    udpOut[16+ofs] = G(selseg.segcol[0].getU32());
    udpOut[17+ofs] = B(selseg.segcol[0].getU32());
    udpOut[18+ofs] = W(selseg.segcol[0].getU32());
    udpOut[19+ofs] = R(selseg.segcol[1].getU32());
    udpOut[20+ofs] = G(selseg.segcol[1].getU32());
    udpOut[21+ofs] = B(selseg.segcol[1].getU32());
    udpOut[22+ofs] = W(selseg.segcol[1].getU32());
    udpOut[23+ofs] = R(selseg.segcol[2].getU32());
    udpOut[24+ofs] = G(selseg.segcol[2].getU32());
    udpOut[25+ofs] = B(selseg.segcol[2].getU32());
    udpOut[26+ofs] = W(selseg.segcol[2].getU32());
    udpOut[27+ofs] = selseg.cct_slider;
    udpOut[28+ofs] = (selseg.options>>8) & 0xFF; //mirror_y, transpose, 2D mapping & sound
    udpOut[29+ofs] = selseg.custom1;
    udpOut[30+ofs] = selseg.custom2;
    udpOut[31+ofs] = selseg.custom3 | (selseg.check1<<5) | (selseg.check2<<6) | (selseg.check3<<7);
    udpOut[32+ofs] = selseg.startY >> 8;
    udpOut[33+ofs] = selseg.startY & 0xFF;
    udpOut[34+ofs] = selseg.stopY >> 8;
    udpOut[35+ofs] = selseg.stopY & 0xFF;
    ++s;
  }

  //uint16_t offs = SEG_OFFSET;
  //next value to be added has index: udpOut[offs + 0]

  IPAddress broadcastIp;
  broadcastIp = ~uint32_t(Network.subnetMask()) | uint32_t(Network.gatewayIP());

  notifierUdp.beginPacket(broadcastIp, udpPort);
  notifierUdp.write(udpOut, WLEDPACKETSIZE);
  notifierUdp.endPacket();
  notificationSentCallMode = callMode;
  notificationSentTime = millis();
  notificationCount = followUp ? notificationCount + 1 : 0;
}


#ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI


void realtimeLock2(uint32_t timeoutMs, byte md)
{
  if (!tkr_anim->realtimeMode && !tkr_anim->realtimeOverride) {
    uint16_t stop, start;
    if (tkr_anim->useMainSegmentOnly) {
      mAnimatorLight::Segment& mainseg = tkr_anim->getMainSegment();
      start = mainseg.start;
      stop  = mainseg.stop;
      mainseg.freeze = true;
    } else {
      start = 0;
      stop  = tkr_anim->getLengthTotal();
    }
    // clear strip/segment
    for (size_t i = start; i < stop; i++) tkr_anim->setPixelColor(i,BLACK);
    // if WLED was off and using main segment only, freeze non-main segments so they stay off
    if (tkr_anim->useMainSegmentOnly && pCONT_iLight->getBri_Global() == 0) {
      for (size_t s=0; s < tkr_anim->getSegmentsNum(); s++) {
        tkr_anim->getSegment(s).freeze = true;
      }
    }
  }
  // if strip is off (bri==0) and not already in RTM
  if (tkr_anim->briT == 0 && !tkr_anim->realtimeMode && !tkr_anim->realtimeOverride) {
    tkr_anim->setBrightness(scaledBri2(tkr_anim->briLast), true);
  }

  if (tkr_anim->realtimeTimeout != UINT32_MAX) {
    tkr_anim->realtimeTimeout = (timeoutMs == 255001 || timeoutMs == 65000) ? UINT32_MAX : millis() + timeoutMs;
  }
  tkr_anim->realtimeMode = md;

  if (tkr_anim->realtimeOverride) return;
  if (tkr_anim->arlsForceMaxBri) tkr_anim->setBrightness(scaledBri2(255), true);
  if (tkr_anim->briT > 0 && md == REALTIME_MODE_GENERIC) tkr_anim->show();
}

void exitRealtime2() {
  if (!tkr_anim->realtimeMode) return;
  if (tkr_anim->realtimeOverride == REALTIME_OVERRIDE_ONCE) tkr_anim->realtimeOverride = REALTIME_OVERRIDE_NONE;
  tkr_anim->setBrightness(scaledBri2(pCONT_iLight->getBri_Global()), true);
  tkr_anim->realtimeTimeout = 0; // cancel realtime mode immediately
  tkr_anim->realtimeMode = REALTIME_MODE_INACTIVE; // inform UI immediately
  tkr_anim->realtimeIP[0] = 0;
  if (tkr_anim->useMainSegmentOnly) { // unfreeze live segment again
    tkr_anim->getMainSegment().freeze = false;
  }
  // updateInterfaces(CALL_MODE_WS_SEND);
}


#define TMP2NET_OUT_PORT 65442

void sendTPM2Ack() {
  tkr_anim->notifierUdp.beginPacket(tkr_anim->notifierUdp.remoteIP(), TMP2NET_OUT_PORT);
  uint8_t response_ack = 0xac;
  tkr_anim->notifierUdp.write(&response_ack, 1);
  tkr_anim->notifierUdp.endPacket();
}


void handleNotifications()
{
  IPAddress localIP;

  //send second notification if enabled
  if(tkr_anim->udpConnected && (tkr_anim->notificationCount < tkr_anim->udpNumRetries) && ((millis()-tkr_anim->notificationSentTime) > 250)){
    // notify(tkr_anim->notificationSentCallMode,true);
  }

  if (tkr_anim->e131NewData && millis() - tkr_anim->getLastShow() > 15)
  {
    tkr_anim->e131NewData = false;
    tkr_anim->show();
  }

  //unlock strip when realtime UDP times out
  if (tkr_anim->realtimeMode && millis() > tkr_anim->realtimeTimeout) exitRealtime2();

  //receive UDP notifications
  if (!tkr_anim->udpConnected) return;

  bool isSupp = false;
  size_t packetSize = tkr_anim->notifierUdp.parsePacket();
  if (!packetSize && tkr_anim->udp2Connected) {
    packetSize = tkr_anim->notifier2Udp.parsePacket();
    isSupp = true;
  }

  //hyperion / raw RGB
  if (!packetSize && tkr_anim->udpRgbConnected) {
    packetSize = tkr_anim->rgbUdp.parsePacket();
    if (packetSize) {
      if (!tkr_anim->receiveDirect) return;
      if (packetSize > UDP_IN_MAXSIZE || packetSize < 3) return;
      tkr_anim->realtimeIP = tkr_anim->rgbUdp.remoteIP();
      DEBUG_PRINTLN( tkr_anim->rgbUdp.remoteIP());
      uint8_t lbuf[packetSize];
      tkr_anim->rgbUdp.read(lbuf, packetSize);
      realtimeLock2(tkr_anim->realtimeTimeoutMs, REALTIME_MODE_HYPERION);
      if (tkr_anim->realtimeOverride && !(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) return;
      uint16_t id = 0;
      uint16_t totalLen = tkr_anim->getLengthTotal();
      for (size_t i = 0; i < packetSize -2; i += 3)
      {
        setRealtimePixel(id, lbuf[i], lbuf[i+1], lbuf[i+2], 0);
        id++; if (id >= totalLen) break;
      }
      if (!(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) tkr_anim->show();
      return;
    }
  }

  if (!(tkr_anim->receiveNotifications || tkr_anim->receiveDirect)) return;

  localIP = Network.localIP();
  //notifier and UDP realtime
  if (!packetSize || packetSize > UDP_IN_MAXSIZE) return;
  if (!isSupp && tkr_anim->notifierUdp.remoteIP() == localIP) return; //don't process broadcasts we send ourselves

  uint8_t udpIn[packetSize +1];
  uint16_t len;
  if (isSupp) len = tkr_anim->notifier2Udp.read(udpIn, packetSize);
  else        len =  tkr_anim->notifierUdp.read(udpIn, packetSize);

  // // WLED nodes info notifications
  // if (isSupp && udpIn[0] == 255 && udpIn[1] == 1 && len >= 40) {
  //   if (!tkr_anim->nodeListEnabled || tkr_anim->notifier2Udp.remoteIP() == localIP) return;

  //   uint8_t unit = udpIn[39];
  //   NodesMap::iterator it = Nodes.find(unit);
  //   if (it == Nodes.end() && Nodes.size() < WLED_MAX_NODES) { // Create a new element when not present
  //     Nodes[unit].age = 0;
  //     it = Nodes.find(unit);
  //   }

  //   if (it != Nodes.end()) {
  //     for (size_t x = 0; x < 4; x++) {
  //       it->second.ip[x] = udpIn[x + 2];
  //     }
  //     it->second.age = 0; // reset 'age counter'
  //     char tmpNodeName[33] = { 0 };
  //     memcpy(&tmpNodeName[0], reinterpret_cast<byte *>(&udpIn[6]), 32);
  //     tmpNodeName[32]     = 0;
  //     it->second.nodeName = tmpNodeName;
  //     it->second.nodeName.trim();
  //     it->second.nodeType = udpIn[38];
  //     uint32_t build = 0;
  //     if (len >= 44)
  //       for (size_t i=0; i<sizeof(uint32_t); i++)
  //         build |= udpIn[40+i]<<(8*i);
  //     it->second.build = build;
  //   }
  //   return;
  // }

  //wled notifier, ignore if realtime packets active
  if (udpIn[0] == 0 && !tkr_anim->realtimeMode && tkr_anim->receiveNotifications)
  {
    //ignore notification if received within a second after sending a notification ourselves
    if (millis() - tkr_anim->notificationSentTime < 1000) return;
    if (udpIn[1] > 199) return; //do not receive custom versions

    //compatibilityVersionByte:
    byte version = udpIn[11];

    // if we are not part of any sync group ignore message
    if (version < 9 || version > 199) {
      // legacy senders are treated as if sending in sync group 1 only
      if (!(tkr_anim->receiveGroups & 0x01)) return;
    } else if (!(tkr_anim->receiveGroups & udpIn[36])) return;

    bool someSel = (tkr_anim->receiveNotificationBrightness || tkr_anim->receiveNotificationColor || tkr_anim->receiveNotificationEffects);

    //apply colors from notification to main segment, only if not syncing full segments
    if ((tkr_anim->receiveNotificationColor || !someSel) && (version < 11 || !tkr_anim->receiveSegmentOptions)) {
      // primary color, only apply white if intented (version > 0)
      tkr_anim->setColor(0, RGBW32(udpIn[3], udpIn[4], udpIn[5], (version > 0) ? udpIn[10] : 0));
      if (version > 1) {
        tkr_anim->setColor(1, RGBW32(udpIn[12], udpIn[13], udpIn[14], udpIn[15])); // secondary color
      }
      if (version > 6) {
        tkr_anim->setColor(2, RGBW32(udpIn[20], udpIn[21], udpIn[22], udpIn[23])); // tertiary color
        if (version > 9 && version < 200 && udpIn[37] < 255) { // valid CCT/Kelvin value
          uint16_t cct = udpIn[38];
          if (udpIn[37] > 0) { //Kelvin
            cct |= (udpIn[37] << 8);
          }
          tkr_anim->setCCT(cct);
        }
      }
    }

    bool timebaseUpdated = false;
    //apply effects from notification
    bool applyEffects = (tkr_anim->receiveNotificationEffects || !someSel);
    if (version < 200)
    {
      // if (applyEffects && currentPlaylist >= 0) unloadPlaylist();
      if (version > 10 && (tkr_anim->receiveSegmentOptions || tkr_anim->receiveSegmentBounds)) {
        uint8_t numSrcSegs = udpIn[39];
        for (size_t i = 0; i < numSrcSegs; i++) {
          uint16_t ofs = 41 + i*udpIn[40]; //start of segment offset byte
          uint8_t id = udpIn[0 +ofs];
          if (id > tkr_anim->getSegmentsNum()) break;

          mAnimatorLight::Segment& selseg = tkr_anim->getSegment(id);
          if (!selseg.isActive() || !selseg.isSelected()) continue; //do not apply to non selected segments

          uint16_t startY = 0, start  = (udpIn[1+ofs] << 8 | udpIn[2+ofs]);
          uint16_t stopY  = 1, stop   = (udpIn[3+ofs] << 8 | udpIn[4+ofs]);
          uint16_t offset = (udpIn[7+ofs] << 8 | udpIn[8+ofs]);
          if (!tkr_anim->receiveSegmentOptions) {
            selseg.setUp(start, stop, selseg.grouping_get(), selseg.spacing, offset, startY, stopY);
            continue;
          }
          //for (size_t j = 1; j<4; j++) selseg.setOption(j, (udpIn[9 +ofs] >> j) & 0x01); //only take into account mirrored, on, reversed; ignore selected
          selseg.options = (selseg.options & 0x0071U) | (udpIn[9 +ofs] & 0x0E); // ignore selected, freeze, reset & transitional
          // selseg.setOpacity(udpIn[10+ofs]);
          if (applyEffects) {
            tkr_anim->setMode(id,  udpIn[11+ofs]);
            selseg.speed     = udpIn[12+ofs];
            selseg.intensity = udpIn[13+ofs];
            selseg.palette_id   = udpIn[14+ofs];
          }
          if (tkr_anim->receiveNotificationColor || !someSel) {
            selseg.setColor(0, RGBW32(udpIn[15+ofs],udpIn[16+ofs],udpIn[17+ofs],udpIn[18+ofs]));
            selseg.setColor(1, RGBW32(udpIn[19+ofs],udpIn[20+ofs],udpIn[21+ofs],udpIn[22+ofs]));
            selseg.setColor(2, RGBW32(udpIn[23+ofs],udpIn[24+ofs],udpIn[25+ofs],udpIn[26+ofs]));
            selseg.setCCT(udpIn[27+ofs]);
          }
          if (version > 11) {
            // when applying synced options ignore selected as it may be used as indicator of which segments to sync
            // freeze, reset & transitional should never be synced
            selseg.options = (selseg.options & 0x0071U) | (udpIn[28+ofs]<<8) | (udpIn[9 +ofs] & 0x8E); // ignore selected, freeze, reset & transitional
            if (applyEffects) {
              selseg.custom1 = udpIn[29+ofs];
              selseg.custom2 = udpIn[30+ofs];
              selseg.custom3 = udpIn[31+ofs] & 0x1F;
              selseg.check1  = (udpIn[31+ofs]>>5) & 0x1;
              selseg.check1  = (udpIn[31+ofs]>>6) & 0x1;
              selseg.check1  = (udpIn[31+ofs]>>7) & 0x1;
            }
            startY = (udpIn[32+ofs] << 8 | udpIn[33+ofs]);
            stopY  = (udpIn[34+ofs] << 8 | udpIn[35+ofs]);
          }
          if (tkr_anim->receiveSegmentBounds) {
            selseg.setUp(start, stop, udpIn[5+ofs], udpIn[6+ofs], offset, startY, stopY);
          } else {
            selseg.setUp(selseg.start, selseg.stop, udpIn[5+ofs], udpIn[6+ofs], selseg.offset, selseg.startY, selseg.stopY);
          }
        }
        tkr_anim->stateChanged = true;
      }

      // simple effect sync, applies to all selected segments
      if (applyEffects && (version < 11 || !tkr_anim->receiveSegmentOptions)) {
        for (size_t i = 0; i < tkr_anim->getSegmentsNum(); i++) {
          mAnimatorLight::Segment& seg = tkr_anim->getSegment(i);
          if (!seg.isActive() || !seg.isSelected()) continue;
          seg.setMode(udpIn[8]);
          seg.speed =  udpIn[9];
          if (version > 2) seg.intensity = udpIn[16];
          if (version > 4) seg.setPalette(udpIn[19]);
        }
        tkr_anim->stateChanged = true;
      }

      if (applyEffects && version > 5) {
        uint32_t t = (udpIn[25] << 24) | (udpIn[26] << 16) | (udpIn[27] << 8) | (udpIn[28]);
        t += PRESUMED_NETWORK_DELAY; //adjust trivially for network delay
        t -= millis();
        tkr_anim->timebase = t;
        timebaseUpdated = true;
      }
    }

    //adjust system time, but only if sender is more accurate than self
    if (version > 7 && version < 200)
    {
      Toki::Time tm;
      tm.sec = (udpIn[30] << 24) | (udpIn[31] << 16) | (udpIn[32] << 8) | (udpIn[33]);
      tm.ms = (udpIn[34] << 8) | (udpIn[35]);
      if (udpIn[29] > tkr_anim->toki.getTimeSource()) { //if sender's time source is more accurate
        tkr_anim->toki.adjust(tm, PRESUMED_NETWORK_DELAY); //adjust trivially for network delay
        uint8_t ts = TOKI_TS_UDP;
        if (udpIn[29] > 99) ts = TOKI_TS_UDP_NTP;
        else if (udpIn[29] >= TOKI_TS_SEC) ts = TOKI_TS_UDP_SEC;
        tkr_anim->toki.setTime(tm, ts);
      } else if (timebaseUpdated && tkr_anim->toki.getTimeSource() > 99) { //if we both have good times, get a more accurate timebase
        Toki::Time myTime = tkr_anim->toki.getTime();
        uint32_t diff = tkr_anim->toki.msDifference(tm, myTime);
        tkr_anim->timebase -= PRESUMED_NETWORK_DELAY; //no need to presume, use difference between NTP times at send and receive points
        if (tkr_anim->toki.isLater(tm, myTime)) {
          tkr_anim->timebase += diff;
        } else {
          tkr_anim->timebase -= diff;
        }
      }
    }

    if (version > 3)
    {
      // tkr_anim->transitionDelayTemp = ((udpIn[17] << 0) & 0xFF) + ((udpIn[18] << 8) & 0xFF00);
    }

    tkr_anim->nightlightActive = udpIn[6];
    if (tkr_anim->nightlightActive) tkr_anim->nightlightDelayMins = udpIn[7];

    if (tkr_anim->receiveNotificationBrightness || !someSel) pCONT_iLight->setBriRGB_Global( udpIn[2] );
    // stateUpdated(CALL_MODE_NOTIFICATION);
    return;
  }

  if (!tkr_anim->receiveDirect) return;

  //TPM2.NET
  if (udpIn[0] == 0x9c)
  {
    //WARNING: this code assumes that the final TMP2.NET payload is evenly distributed if using multiple packets (ie. frame size is constant)
    //if the number of LEDs in your installation doesn't allow that, please include padding bytes at the end of the last packet
    byte tpmType = udpIn[1];
    if (tpmType == 0xaa) { //TPM2.NET polling, expect answer
      sendTPM2Ack(); return;
    }
    if (tpmType != 0xda) return; //return if notTPM2.NET data

    tkr_anim->realtimeIP = (isSupp) ? tkr_anim->notifier2Udp.remoteIP() : tkr_anim->notifierUdp.remoteIP();
    realtimeLock2(tkr_anim->realtimeTimeoutMs, REALTIME_MODE_TPM2NET);
    if (tkr_anim->realtimeOverride && !(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) return;

    tkr_anim->tpmPacketCount++; //increment the packet count
    if (tkr_anim->tpmPacketCount == 1) tkr_anim->tpmPayloadFrameSize = (udpIn[2] << 8) + udpIn[3]; //save frame size for the whole payload if this is the first packet
    byte packetNum = udpIn[4]; //starts with 1!
    byte numPackets = udpIn[5];

    uint16_t id = (tkr_anim->tpmPayloadFrameSize/3)*(packetNum-1); //start LED
    uint16_t totalLen = tkr_anim->getLengthTotal();
    for (size_t i = 6; i < tkr_anim->tpmPayloadFrameSize + 4U; i += 3)
    {
      if (id < totalLen)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);
        id++;
      }
      else break;
    }
    if (tkr_anim->tpmPacketCount == numPackets) //reset packet count and show if all packets were received
    {
      tkr_anim->tpmPacketCount = 0;
      tkr_anim->show();
    }
    return;
  }

  //UDP realtime: 1 warls 2 drgb 3 drgbw
  if (udpIn[0] > 0 && udpIn[0] < 5)
  {
    tkr_anim->realtimeIP = (isSupp) ? tkr_anim->notifier2Udp.remoteIP() : tkr_anim->notifierUdp.remoteIP();
    DEBUG_PRINTLN(tkr_anim->realtimeIP);
    if (packetSize < 2) return;

    if (udpIn[1] == 0)
    {
      tkr_anim->realtimeTimeout = 0;
      return;
    } else {
      realtimeLock2(udpIn[1]*1000 +1, REALTIME_MODE_UDP);
    }
    if (tkr_anim->realtimeOverride && !(tkr_anim->realtimeMode && tkr_anim->useMainSegmentOnly)) return;

    uint16_t totalLen = tkr_anim->getLengthTotal();
    if (udpIn[0] == 1) //warls
    {
      for (size_t i = 2; i < packetSize -3; i += 4)
      {
        setRealtimePixel(udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3], 0);
      }
    } else if (udpIn[0] == 2) //drgb
    {
      uint16_t id = 0;
      for (size_t i = 2; i < packetSize -2; i += 3)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);

        id++; if (id >= totalLen) break;
      }
    } else if (udpIn[0] == 3) //drgbw
    {
      uint16_t id = 0;
      for (size_t i = 2; i < packetSize -3; i += 4)
      {
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3]);

        id++; if (id >= totalLen) break;
      }
    } else if (udpIn[0] == 4) //dnrgb
    {
      uint16_t id = ((udpIn[3] << 0) & 0xFF) + ((udpIn[2] << 8) & 0xFF00);
      for (size_t i = 4; i < packetSize -2; i += 3)
      {
        if (id >= totalLen) break;
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], 0);
        id++;
      }
    } else if (udpIn[0] == 5) //dnrgbw
    {
      uint16_t id = ((udpIn[3] << 0) & 0xFF) + ((udpIn[2] << 8) & 0xFF00);
      for (size_t i = 4; i < packetSize -2; i += 4)
      {
        if (id >= totalLen) break;
        setRealtimePixel(id, udpIn[i], udpIn[i+1], udpIn[i+2], udpIn[i+3]);
        id++;
      }
    }
    tkr_anim->show();
    return;
  }

  // API over UDP
  udpIn[packetSize] = '\0';

  // if (requestJSONBufferLock(18)) {
  //   if (udpIn[0] >= 'A' && udpIn[0] <= 'Z') { //HTTP API
  //     String apireq = "win"; apireq += '&'; // reduce flash string usage
  //     apireq += (char*)udpIn;
  //     handleSet(nullptr, apireq);
  //   } else if (udpIn[0] == '{') { //JSON API
  //     DeserializationError error = deserializeJson(doc, udpIn);
  //     JsonObject root = doc.as<JsonObject>();
  //     if (!error && !root.isNull()) deserializeState(root);
  //   }
  //   releaseJSONBufferLock();
  // }
}


void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w)
{
  uint16_t pix = i + tkr_anim->arlsOffset;
  if (pix < tkr_anim->getLengthTotal()) {
    if (!tkr_anim->arlsDisableGammaCorrection && tkr_anim->gammaCorrectCol) {
      // r = tkr_anim->gamma8(r);
      // g = tkr_anim->gamma8(g);
      // b = tkr_anim->gamma8(b);
      // w = tkr_anim->gamma8(w);
    }
    if (tkr_anim->useMainSegmentOnly) {
      mAnimatorLight::Segment &seg = tkr_anim->getMainSegment();
      if (pix<seg.length()) seg.setPixelColor(pix, r, g, b, w);
    } else {
      // tkr_anim->setPixelColor(pix, r, g, b, w);
    }
  }
}

/*********************************************************************************************\
   Refresh aging for remote units, drop if too old...
\*********************************************************************************************/
void refreshNodeList()
{
  // for (NodesMap::iterator it = Nodes.begin(); it != Nodes.end();) {
  //   bool mustRemove = true;

  //   if (it->second.ip[0] != 0) {
  //     if (it->second.age < 10) {
  //       it->second.age++;
  //       mustRemove = false;
  //       ++it;
  //     }
  //   }

  //   if (mustRemove) {
  //     it = Nodes.erase(it);
  //   }
  // }
}

/*********************************************************************************************\
   Broadcast system info to other nodes. (to update node lists)
\*********************************************************************************************/
void sendSysInfoUDP()
{
  if (!tkr_anim->udp2Connected) return;

  IPAddress ip = Network.localIP();
  if (!ip || ip == IPAddress(255,255,255,255)) ip = IPAddress(4,3,2,1);

  // TODO: make a nice struct of it and clean up
  //  0: 1 byte 'binary token 255'
  //  1: 1 byte id '1'
  //  2: 4 byte ip
  //  6: 32 char name
  // 38: 1 byte node type id
  // 39: 1 byte node id
  // 40: 4 byte version ID
  // 44 bytes total

  // send my info to the world...
  uint8_t data[44] = {0};
  data[0] = 255;
  data[1] = 1;

  for (size_t x = 0; x < 4; x++) {
    data[x + 2] = ip[x];
  }
  memcpy((byte *)data + 6, tkr_anim->serverDescription, 32);
  #ifdef ESP8266
  data[38] = NODE_TYPE_ID_ESP8266;
  #elif defined(CONFIG_IDF_TARGET_ESP32C3)
  data[38] = NODE_TYPE_ID_ESP32C3;
  #elif defined(CONFIG_IDF_TARGET_ESP32S3)
  data[38] = NODE_TYPE_ID_ESP32S3;
  #elif defined(CONFIG_IDF_TARGET_ESP32S2)
  data[38] = NODE_TYPE_ID_ESP32S2;
  #elif defined(ARDUINO_ARCH_ESP32)
  data[38] = 32;//NODE_TYPE_ID_ESP32;
  #else
  data[38] = NODE_TYPE_ID_UNDEFINED;
  #endif
  data[39] = ip[3]; // unit ID == last IP number

  uint32_t build = PROJECT_VERSION;
  for (size_t i=0; i<sizeof(uint32_t); i++)
    data[40+i] = (build>>(8*i)) & 0xFF;

  IPAddress broadcastIP(255, 255, 255, 255);
  tkr_anim->notifier2Udp.beginPacket(broadcastIP, tkr_anim->udpPort2);
  tkr_anim->notifier2Udp.write(data, sizeof(data));
  tkr_anim->notifier2Udp.endPacket();
}


/*********************************************************************************************\
 * Art-Net, DDP, E131 output - work in progress
\*********************************************************************************************/

#define DDP_HEADER_LEN 10
#define DDP_SYNCPACKET_LEN 10

#define DDP_FLAGS1_VER 0xc0  // version mask
#define DDP_FLAGS1_VER1 0x40 // version=1
#define DDP_FLAGS1_PUSH 0x01
#define DDP_FLAGS1_QUERY 0x02
#define DDP_FLAGS1_REPLY 0x04
#define DDP_FLAGS1_STORAGE 0x08
#define DDP_FLAGS1_TIME 0x10

#define DDP_ID_DISPLAY 1
#define DDP_ID_CONFIG 250
#define DDP_ID_STATUS 251

// 1440 channels per packet
#define DDP_CHANNELS_PER_PACKET 1440 // 480 leds

//
// Send real time UDP updates to the specified client
//
// type   - protocol type (0=DDP, 1=E1.31, 2=ArtNet)
// client - the IP address to send to
// length - the number of pixels
// buffer - a buffer of at least length*4 bytes long
// isRGBW - true if the buffer contains 4 components per pixel

static       size_t sequenceNumber = 0; // this needs to be shared across all outputs
static const size_t ART_NET_HEADER_SIZE = 12;
static const byte   ART_NET_HEADER[] PROGMEM = {0x41,0x72,0x74,0x2d,0x4e,0x65,0x74,0x00,0x00,0x50,0x00,0x0e};

uint8_t realtimeBroadcast(uint8_t type, IPAddress client, uint16_t length, uint8_t *buffer, uint8_t bri, bool isRGBW)  {
  // if (!(apActive || interfacesInited) || !client[0] || !length) return 1;  // network not initialised or dummy/unset IP address  031522 ajn added check for ap

  WiFiUDP ddpUdp;

  switch (type) {
    case 0: // DDP
    {
      // // calculate the number of UDP packets we need to send
      // size_t channelCount = length * (isRGBW? 4:3); // 1 channel for every R,G,B value
      // size_t packetCount = ((channelCount-1) / DDP_CHANNELS_PER_PACKET) +1;

      // // there are 3 channels per RGB pixel
      // uint32_t channel = 0; // TODO: allow specifying the start channel
      // // the current position in the buffer
      // size_t bufferOffset = 0;

      // for (size_t currentPacket = 0; currentPacket < packetCount; currentPacket++) {
      //   if (sequenceNumber > 15) sequenceNumber = 0;

      //   if (!ddpUdp.beginPacket(client, DDP_DEFAULT_PORT)) {  // port defined in ESPAsyncE131.h
      //     DEBUG_PRINTLN(F("WiFiUDP.beginPacket returned an error"));
      //     return 1; // problem
      //   }

      //   // the amount of data is AFTER the header in the current packet
      //   size_t packetSize = DDP_CHANNELS_PER_PACKET;

      //   uint8_t flags = DDP_FLAGS1_VER1;
      //   if (currentPacket == (packetCount - 1U)) {
      //     // last packet, set the push flag
      //     // TODO: determine if we want to send an empty push packet to each destination after sending the pixel data
      //     flags = DDP_FLAGS1_VER1 | DDP_FLAGS1_PUSH;
      //     if (channelCount % DDP_CHANNELS_PER_PACKET) {
      //       packetSize = channelCount % DDP_CHANNELS_PER_PACKET;
      //     }
      //   }

      //   // write the header
      //   /*0*/ddpUdp.write(flags);
      //   /*1*/ddpUdp.write(sequenceNumber++ & 0x0F); // sequence may be unnecessary unless we are sending twice (as requested in Sync settings)
      //   /*2*/ddpUdp.write(isRGBW ?  DDP_TYPE_RGBW32 : DDP_TYPE_RGB24);
      //   /*3*/ddpUdp.write(DDP_ID_DISPLAY);
      //   // data offset in bytes, 32-bit number, MSB first
      //   /*4*/ddpUdp.write(0xFF & (channel >> 24));
      //   /*5*/ddpUdp.write(0xFF & (channel >> 16));
      //   /*6*/ddpUdp.write(0xFF & (channel >>  8));
      //   /*7*/ddpUdp.write(0xFF & (channel      ));
      //   // data length in bytes, 16-bit number, MSB first
      //   /*8*/ddpUdp.write(0xFF & (packetSize >> 8));
      //   /*9*/ddpUdp.write(0xFF & (packetSize     ));

      //   // write the colors, the write write(const uint8_t *buffer, size_t size)
      //   // function is just a loop internally too
      //   for (size_t i = 0; i < packetSize; i += (isRGBW?4:3)) {
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // R
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // G
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // B
      //     if (isRGBW) ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // W
      //   }

      //   if (!ddpUdp.endPacket()) {
      //     DEBUG_PRINTLN(F("WiFiUDP.endPacket returned an error"));
      //     return 1; // problem
      //   }

      //   channel += packetSize;
      // }
    } break;

    case 1: //E1.31
    {
    } break;

    case 2: //ArtNet
    {
      // // calculate the number of UDP packets we need to send
      // const size_t channelCount = length * (isRGBW?4:3); // 1 channel for every R,G,B,(W?) value
      // const size_t ARTNET_CHANNELS_PER_PACKET = isRGBW?512:510; // 512/4=128 RGBW LEDs, 510/3=170 RGB LEDs
      // const size_t packetCount = ((channelCount-1)/ARTNET_CHANNELS_PER_PACKET)+1;

      // uint32_t channel = 0; 
      // size_t bufferOffset = 0;

      // sequenceNumber++;

      // for (size_t currentPacket = 0; currentPacket < packetCount; currentPacket++) {

      //   if (sequenceNumber > 255) sequenceNumber = 0;

      //   if (!ddpUdp.beginPacket(client, ARTNET_DEFAULT_PORT)) {
      //     DEBUG_PRINTLN(F("Art-Net WiFiUDP.beginPacket returned an error"));
      //     return 1; // borked
      //   }

      //   size_t packetSize = ARTNET_CHANNELS_PER_PACKET;

      //   if (currentPacket == (packetCount - 1U)) {
      //     // last packet
      //     if (channelCount % ARTNET_CHANNELS_PER_PACKET) {
      //       packetSize = channelCount % ARTNET_CHANNELS_PER_PACKET;
      //     }
      //   }

      //   byte header_buffer[ART_NET_HEADER_SIZE];
      //   memcpy_P(header_buffer, ART_NET_HEADER, ART_NET_HEADER_SIZE);
      //   ddpUdp.write(header_buffer, ART_NET_HEADER_SIZE); // This doesn't change. Hard coded ID, OpCode, and protocol version.
      //   ddpUdp.write(sequenceNumber & 0xFF); // sequence number. 1..255
      //   ddpUdp.write(0x00); // physical - more an FYI, not really used for anything. 0..3
      //   ddpUdp.write((currentPacket) & 0xFF); // Universe LSB. 1 full packet == 1 full universe, so just use current packet number.
      //   ddpUdp.write(0x00); // Universe MSB, unused.
      //   ddpUdp.write(0xFF & (packetSize >> 8)); // 16-bit length of channel data, MSB
      //   ddpUdp.write(0xFF & (packetSize     )); // 16-bit length of channel data, LSB

      //   for (size_t i = 0; i < packetSize; i += (isRGBW?4:3)) {
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // R
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // G
      //     ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // B
      //     if (isRGBW) ddpUdp.write(scale8(buffer[bufferOffset++], bri)); // W
      //   }

      //   if (!ddpUdp.endPacket()) {
      //     DEBUG_PRINTLN(F("Art-Net WiFiUDP.endPacket returned an error"));
      //     return 1; // borked
      //   }
      //   channel += packetSize;
      // }
    } break;
  }
  return 0;
}



//called after every state changes, schedules interface updates, handles brightness transition and nightlight activation
//unlike colorUpdated(), does NOT apply any colors or FX to segments
void mAnimatorLight::stateUpdated(byte callMode) {
  //call for notifier -> 0: init 1: direct change 2: button 3: notification 4: nightlight 5: other (No notification)
  //                     6: fx changed 7: hue 8: preset cycle 9: blynk 10: alexa 11: ws send only 12: button preset
  // setValuesFromFirstSelectedSeg();

  // if (bri != briOld || stateChanged) {
  //   if (stateChanged) currentPreset = 0; //something changed, so we are no longer in the preset

  //   if (callMode != CALL_MODE_NOTIFICATION && callMode != CALL_MODE_NO_NOTIFY) notify(callMode);
  //   if (bri != briOld && nodeBroadcastEnabled) sendSysInfoUDP(); // update on state

  //   //set flag to update ws and mqtt
  //   interfaceUpdateCallMode = callMode;
  //   stateChanged = false;
  // } else {
  //   if (nightlightActive && !nightlightActiveOld && callMode != CALL_MODE_NOTIFICATION && callMode != CALL_MODE_NO_NOTIFY) {
  //     notify(CALL_MODE_NIGHTLIGHT);
  //     interfaceUpdateCallMode = CALL_MODE_NIGHTLIGHT;
  //   }
  // }

  // if (callMode != CALL_MODE_NO_NOTIFY && nightlightActive && (nightlightMode == NL_MODE_FADE || nightlightMode == NL_MODE_COLORFADE)) {
  //   briNlT = bri;
  //   nightlightDelayMs -= (millis() - nightlightStartTime);
  //   nightlightStartTime = millis();
  // }
  // if (briT == 0) {
  //   if (callMode != CALL_MODE_NOTIFICATION) strip.resetTimebase(); //effect start from beginning
  // }

  // if (bri > 0) briLast = bri;

  // //deactivate nightlight if target brightness is reached
  // if (bri == nightlightTargetBri && callMode != CALL_MODE_NO_NOTIFY && nightlightMode != NL_MODE_SUN) nightlightActive = false;

  // // notify usermods of state change
  // UsermodManager::onStateChange(callMode);

  // if (fadeTransition) {
  //   if (strip.getTransition() == 0) {
  //     jsonTransitionOnce = false;
  //     transitionActive = false;
  //     applyFinalBri();
  //     strip.trigger();
  //     return;
  //   }

  //   if (transitionActive) {
  //     briOld = briT;
  //   } else
  //     strip.setTransitionMode(true); // force all segments to transition mode
  //   transitionActive = true;
  //   transitionStartTime = millis();
  // } else {
  //   applyFinalBri();
  //   strip.trigger();
  // }
}


void mAnimatorLight::updateInterfaces(uint8_t callMode)
{
  if (!interfaceUpdateCallMode || millis() - lastInterfaceUpdate < INTERFACE_UPDATE_COOLDOWN) return;

  ALOG_INF(PSTR("Sending Update"));

  sendDataWs();
  lastInterfaceUpdate = millis();
  interfaceUpdateCallMode = 0; //disable further updates

  if (callMode == CALL_MODE_WS_SEND) return;

  // #ifndef WLED_DISABLE_ALEXA
  // if (espalexaDevice != nullptr && callMode != CALL_MODE_ALEXA) {
  //   espalexaDevice->setValue(bri);
  //   espalexaDevice->setColor(col[0], col[1], col[2]);
  // }
  // #endif
  // #ifndef WLED_DISABLE_MQTT
  // publishMqtt();
  // #endif
}


// legacy method, applies values from col, effectCurrent, ... to selected segments
void mAnimatorLight::colorUpdated(byte callMode) {
  // applyValuesToSelectedSegs();
  // stateUpdated(callMode);
}



#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI








#endif //USE_MODULE_LIGHTS_ANIMATOR
