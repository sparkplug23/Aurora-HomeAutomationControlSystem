#include "6_Lights/03_Animator/mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#define Network WiFi


#ifdef ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

// ESP32-WROVER features SPI RAM (aka PSRAM) which can be allocated using ps_malloc()
// we can create custom PSRAMDynamicJsonDocument to use such feature (replacing DynamicJsonDocument)
// The following is a construct to enable code to compile without it.
// There is a code thet will still not use PSRAM though:
//    AsyncJsonResponse is a derived class that implements DynamicJsonDocument (AsyncJson-v6.h)
#if defined(ARDUINO_ARCH_ESP32) && defined(BOARD_HAS_PSRAM) && defined(WLED_USE_PSRAM)
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (psramFound()) return ps_malloc(size); // use PSRAM if it exists
    else              return malloc(size);    // fallback
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (psramFound()) return ps_realloc(ptr, new_size); // use PSRAM if it exists
    else              return realloc(ptr, new_size);    // fallback
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#elses
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif


//threading/network callback details: https://github.com/Aircoookie/WLED/pull/2336#discussion_r762276994
bool mAnimatorLight::requestJSONBufferLock(uint16_t module)
{
  unsigned long now = millis();

  // This assumption here is another http thread must release itself to permit this function to proceed
  while (jsonBufferLock && millis()-now < 1000) delay(1); // wait for a second for buffer lock

  if (millis()-now >= 1000) {
    // DEBUG_PRINT(F("ERROR: Locking JSON buffer failed! ("));
    // DEBUG_PRINT(jsonBufferLock);
    // DEBUG_PRINTLN(")");
    return false; // waiting time-outed
  }

  jsonBufferLock = module ? module : 255;
  // DEBUG_PRINT(F("LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOCKED    JSON buffer locked. ("));
  // DEBUG_PRINT(jsonBufferLock);
  // DEBUG_PRINTLN(")");
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  pCONT_mfile->fileDoc = &doc;  // used for applying presets (presets.cpp)
  doc.clear();
  #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS
  return true;
}

void mAnimatorLight::releaseJSONBufferLock()
{
  // DEBUG_PRINT(F("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNLOCKED JSON buffer released. ("));
  // DEBUG_PRINT(jsonBufferLock);
  // DEBUG_PRINTLN(")");
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  pCONT_mfile->fileDoc = nullptr;
  #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS
  jsonBufferLock = 0;
}



void mAnimatorLight::serializeSegment(JsonObject& root, mAnimatorLight::Segment& seg, byte id, bool forPreset, bool segmentBounds)
{
  root["id"] = id;
  if (segmentBounds) {
    root["start"] = seg.start;
    root["stop"] = seg.stop;
    if (isMatrix) {
      root[F("startY")] = seg.startY;
      root[F("stopY")]  = seg.stopY;
    }
  }
  if (!forPreset) root["len"] = seg.stop - seg.start;
  root["grp"]    = seg.grouping;
  root[F("spc")] = seg.spacing;
  root[F("of")]  = seg.offset;
  root["on"]     = seg.on;
  root["frz"]    = seg.freeze;
  byte segbri    = seg._brightness_rgb;
  root["bri"]    = (segbri) ? segbri : 255;

  if (segmentBounds && seg.name != nullptr) root["n"] = reinterpret_cast<const char *>(seg.name); //not good practice, but decreases required JSON buffer

  root["cct"] = seg.palette_id < 5 ? seg.segcol[seg.palette_id].getCCT() : 255;
  
  root[F("set")] = 0;//seg.set;    // unknown, phase into newer WLED UI

  if (segmentBounds && seg.name != nullptr) root["n"] = reinterpret_cast<const char *>(seg.name); //not good practice, but decreases required JSON buffer

  /**
   * @brief Changing from WLED
   * 
   * My RGBW values, stored in RgbwwColor, are stored as 0-255 values with their maximum value. 
   * - Since white will always be 255 in single white mode, or changes when CCT is used, this value does not represent the actual white value and should be control the slider.
   * Instead, the white (brightness) needs to be controlled by the segment white brightness, and independant of the rgbcct value
   * 
   */
  char colstr[100]; colstr[0] = '['; colstr[1] = '\0';
  for (size_t i = 0; i < 5; i++)
  {
    byte segcol[4]; 
    segcol[0] = seg.segcol[i].colour.R;
    segcol[1] = seg.segcol[i].colour.G;
    segcol[2] = seg.segcol[i].colour.B;
    segcol[3] = seg.segcol[i].colour.WW; // white channels inside RgbwwColor is always stored as max value, so slider should reflect the global CCT brightness
    segcol[4] = seg.segcol[i].colour.CW; // white channels inside RgbwwColor is always stored as max value, so slider should reflect the global CCT brightness
    char tmpcol[40];
    snprintf_P(tmpcol, sizeof(tmpcol), PSTR("[%u,%u,%u,%u,%u]"), segcol[0], segcol[1], segcol[2], segcol[3], segcol[3]);
    strcat(colstr, i<4 ? strcat(tmpcol, ",") : tmpcol);
  }
  strcat(colstr, "]");
  // ALOG_INF(PSTR("colstr OUT = %s"), colstr);
  root["col"] = serialized(colstr);

  root["rgbbri"] = seg.getBrightnessRGB();
  root["cctbri"] = seg.getBrightnessCCT();

  root["fx"]  = seg.effect_id;
  root["sx"]  = seg.speed;
  root["ix"]  = seg.intensity;
  root["ep"]  = seg.cycle_time__rate_ms;
  root["pal"] = seg.palette_id;
  root["c1"]  = seg.custom1;
  root["c2"]  = seg.custom2;
  root["c3"]  = seg.custom3;
  root["v0"]  = seg.params_user[0];
  root["v1"]  = seg.params_user[1];
  root["v2"]  = seg.params_user[2];
  root["v3"]  = seg.params_user[3];
  root["sel"] = seg.isSelected();
  root["rev"] = seg.reverse;
  root["mi"]  = seg.mirror;
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (isMatrix) {
    root["rY"] = seg.reverse_y;
    root["mY"] = seg.mirror_y;
    root[F("tp")] = seg.transpose;
  }
  #endif
  root["o1"]  = seg.check1;
  root["o2"]  = seg.check2;
  root["o3"]  = seg.check3;
  root["si"]  = seg.soundSim;
  root["m12"] = seg.map1D2D;
}

void mAnimatorLight::serializeState(JsonObject root, bool forPreset, bool includeBri, bool segmentBounds, bool selectedSegmentsOnly)
{
  if (includeBri) {
    root["on"] = pCONT_iLight->_briRGB_Global > 0;
    root["bri"] = pCONT_iLight->_briRGB_Global;
  }

  if (!forPreset) {
    if (pCONT_mfile->errorFlag) {root[F("error")] = pCONT_mfile->errorFlag; pCONT_mfile->errorFlag = ERR_NONE;} //prevent error message to persist on screen

    root["ps"] = (currentPreset > 0) ? currentPreset : -1;    
    
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
    root[F("pl")] = currentPlaylist;
    #endif

    JsonObject nl = root.createNestedObject("nl");
    nl["on"] = nightlightActive;
    nl["dur"] = nightlightDelayMins;
    nl["mode"] = nightlightMode;
    nl[F("tbri")] = nightlightTargetBri;
    if (nightlightActive) {
      nl[F("rem")] = (nightlightDelayMs - (millis() - nightlightStartTime)) / 1000; // seconds remaining
    } else {
      nl[F("rem")] = -1;
    }

    JsonObject udpn = root.createNestedObject("udpn");
    udpn["send"] = notifyDirect;
    udpn["recv"] = receiveNotifications;

    root[F("lor")] = realtimeOverride;
  }

  root[F("mainseg")] = getMainSegmentId();

  JsonArray seg = root.createNestedArray("seg");
  for (size_t s = 0; s < getMaxSegments(); s++) {
    if (s >= getSegmentsNum()) {
      if (forPreset && segmentBounds && !selectedSegmentsOnly) { //disable segments not part of preset
        JsonObject seg0 = seg.createNestedObject();
        seg0["stop"] = 0;
        continue;
      } else
        break;
    }
    mAnimatorLight::Segment &sg = getSegment(s);
    if (forPreset && selectedSegmentsOnly && !sg.isSelected()) continue;
    if (sg.isActive()) {
      JsonObject seg0 = seg.createNestedObject();
      serializeSegment(seg0, sg, s, forPreset, segmentBounds);
    } else if (forPreset && segmentBounds) { //disable segments not part of preset
      JsonObject seg0 = seg.createNestedObject();
      seg0["stop"] = 0;
    }
  }


}

void mAnimatorLight::serializeInfo(JsonObject root)
{
  
  tkr_anim->force_update(); // New data in, so we should update

  root[F("ver")] = "versionString";
  root[F("vid")] = PROJECT_VERSION;
  root[F("cn")] = "WLED_CODENAME";

  JsonObject leds = root.createNestedObject("leds");
  leds[F("count")] = 123;//getLengthTotal();
  leds[F("pwr")] = 123;//currentMilliamps;
  leds["fps"] = getFps();
  leds[F("maxpwr")] = (currentMilliamps)? ablMilliampsMax : 0;
  leds[F("maxseg")] = getMaxSegments();
  leds[F("actseg")] = getActiveSegmentsNum();
  leds[F("seglock")] = false; //might be used in the future to prevent modifications to segment config

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  if (isMatrix) {
    JsonObject matrix = leds.createNestedObject("matrix");
    matrix["w"] = Segment::maxWidth;
    matrix["h"] = Segment::maxHeight;
  }
  #endif

  uint8_t totalLC = 0;
  JsonArray lcarr = leds.createNestedArray(F("seglc"));
  size_t nSegs = getSegmentsNum();
  for (size_t s = 0; s < nSegs; s++) {
    if (!getSegment(s).isActive()) continue;
    uint8_t lc = getSegment(s).getLightCapabilities();
    totalLC |= lc;
    lcarr.add(lc);  // This is what enable colour picker!!!
  }

  leds["lc"] = totalLC;

  // leds[F("rgbw")] = hasRGBWBus(); // deprecated, use info.leds.lc
  leds[F("wv")]   = totalLC & 0x02;     // deprecated, true if white slider should be displayed for any segment
  leds["cct"]     = totalLC & 0x04;     // deprecated, use info.leds.lc

  root[F("str")] = syncToggleReceive;

  root[F("name")] = serverDescription;
  root[F("udpport")] = 123;//udpPort;
  root["live"] = (bool)realtimeMode;
  root[F("liveseg")] = -1;//useMainSegmentOnly ? getMainSegmentId() : -1;  // if using main segment only for live

  switch (realtimeMode) {
    case REALTIME_MODE_INACTIVE: root["lm"] = ""; break;
    case REALTIME_MODE_GENERIC:  root["lm"] = ""; break;
    case REALTIME_MODE_UDP:      root["lm"] = F("UDP"); break;
    case REALTIME_MODE_HYPERION: root["lm"] = F("Hyperion"); break;
    case REALTIME_MODE_E131:     root["lm"] = F("E1.31"); break;
    case REALTIME_MODE_ADALIGHT: root["lm"] = F("USB Adalight/TPM2"); break;
    case REALTIME_MODE_ARTNET:   root["lm"] = F("Art-Net"); break;
    case REALTIME_MODE_TPM2NET:  root["lm"] = F("tpm2.net"); break;
    case REALTIME_MODE_DDP:      root["lm"] = F("DDP"); break;
  }

  if (realtimeIP[0] == 0)
  {
    root[F("lip")] = "";
  } else {
    root[F("lip")] = realtimeIP.toString();
  }

  #ifdef ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
  root[F("ws")] = tkr_web->ws->count();
  #else
  root[F("ws")] = -1;
  #endif

  root[F("fxcount")] = getModeCount();
  root[F("palcount")] = getPaletteCount();
  root[F("cpalcount")] = customPalettes.size(); //number of custom palettes

  JsonArray ledmaps = root.createNestedArray(F("maps"));
  // for (size_t i=0; i<WLED_MAX_LEDMAPS; i++) {
  //   if ((ledMaps>>i) & 0x00000001U) {
  //     JsonObject ledmaps0 = ledmaps.createNestedObject();
  //     ledmaps0["id"] = i;
  //     #ifndef ESP8266
  //     if (i && ledmapNames[i-1]) ledmaps0["n"] = ledmapNames[i-1];
  //     #endif
  //   }
  // }

  JsonObject wifi_info = root.createNestedObject("wifi");
  wifi_info[F("bssid")] = WiFi.BSSIDstr();
  int qrssi = WiFi.RSSI();
  wifi_info[F("rssi")] = qrssi;
  wifi_info[F("signal")] = 126;//getSignalQuality(qrssi);
  wifi_info[F("channel")] = WiFi.channel();

  JsonObject fs_info = root.createNestedObject("fs");
  fs_info["u"] = 123;//fsBytesUsed / 1000;
  fs_info["t"] = 123;//fsBytesTotal / 1000;
  fs_info[F("pmt")] = 123;//presetsModifiedTime;

  root[F("ndc")] =123;// nodeListEnabled ? (int)Nodes.size() : -1;

  #ifdef ARDUINO_ARCH_ESP32
  #ifdef WLED_DEBUG
    wifi_info[F("txPower")] = (int) WiFi.getTxPower();
    wifi_info[F("sleep")] = (bool) WiFi.getSleep();
  #endif
  #if !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32S3)
    root[F("arch")] = "esp32";
  #else
    root[F("arch")] = ESP.getChipModel();
  #endif
  root[F("core")] = ESP.getSdkVersion();
  //root[F("maxalloc")] = ESP.getMaxAllocHeap();
  #ifdef WLED_DEBUG
    root[F("resetReason0")] = (int)rtc_get_reset_reason(0);
    root[F("resetReason1")] = (int)rtc_get_reset_reason(1);
  #endif
  root[F("lwip")] = 0; //deprecated
  #else
  root[F("arch")] = "esp8266";
  root[F("core")] = ESP.getCoreVersion();
  //root[F("maxalloc")] = ESP.getMaxFreeBlockSize();
  #ifdef WLED_DEBUG
    root[F("resetReason")] = (int)ESP.getResetInfoPtr()->reason;
  #endif
  root[F("lwip")] = LWIP_VERSION_MAJOR;
  #endif

  root[F("freeheap")] = ESP.getFreeHeap();
  #if defined(ARDUINO_ARCH_ESP32) && defined(BOARD_HAS_PSRAM)
  if (psramFound()) root[F("psram")] = ESP.getFreePsram();
  #endif
  // root[F("uptime")] = millis()/1000 + rolloverMillis*4294967;

  uint16_t os = 0;
  #ifdef WLED_DEBUG
  os  = 0x80;
    #ifdef WLED_DEBUG_HOST
    os |= 0x0100;
    if (!netDebugEnabled) os &= ~0x0080;
    #endif
  #endif
  #ifndef WLED_DISABLE_ALEXA
  os += 0x40;
  #endif

  //os += 0x20; // indicated now removed Blynk support, may be reused to indicate another build-time option

  #ifdef USERMOD_CRONIXIE
  os += 0x10;
  #endif
  #ifndef WLED_DISABLE_FILESYSTEM
  os += 0x08;
  #endif
  #ifndef WLED_DISABLE_HUESYNC
  os += 0x04;
  #endif
  #ifdef WLED_ENABLE_ADALIGHT
  os += 0x02;
  #endif
  #ifndef WLED_DISABLE_OTA
  os += 0x01;
  #endif
  root[F("opt")] = os;

  root[F("brand")] = "WLED";
  root[F("product")] = F("FOSS");
  // root["mac"] = escapedMac;
  char s[16] = "";
  // if (Network.isConnected())
  // {
  //   IPAddress localIP = Network.localIP();
  //   sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  // }
  root["ip"] = s;
}

// deserializes WLED state (fileDoc points to doc object if called from web server)
// presetId is non-0 if called from handlePreset()
bool  mAnimatorLight::deserializeState(JsonObject root, byte callMode, byte presetId)
{
  ALOG_INF(PSTR("deserializeState"));

  bool stateResponse = root[F("v")] | false;

  #if defined(WLED_DEBUG) && defined(WLED_DEBUG_HOST)
  netDebugEnabled = root[F("debug")] | netDebugEnabled;
  #endif

  bool onBefore = pCONT_iLight->_briRGB_Global ; //bri
  if(getVal(root["bri"], &pCONT_iLight->_briRGB_Global))
  {
    //if it was updated, tmp update this bus
    pCONT_iLight->bus_manager->setBrightness( pCONT_iLight->getBriRGB_Global() ); // fix re-initialised bus' brightness
  }


  getVal(root["cBri"], &pCONT_iLight->_briRGB_Global);
  getVal(root["wBri"], &pCONT_iLight->_briCT_Global);


  bool on = root["on"] | (pCONT_iLight->_briRGB_Global > 0);
  if (!on != !pCONT_iLight->_briRGB_Global) toggleOnOff();

  if (root["on"].is<const char*>() && root["on"].as<const char*>()[0] == 't') {
    if (onBefore || !pCONT_iLight->_briRGB_Global) toggleOnOff(); // do not toggle off again if just turned on by bri (makes e.g. "{"on":"t","bri":32}" work)
  }

  if (pCONT_iLight->_briRGB_Global && !onBefore) { // unfreeze all segments when turning on
    for (size_t s=0; s < getSegmentsNum(); s++) {
      getSegment(s).freeze = false;
    }
    if (realtimeMode && !realtimeOverride && useMainSegmentOnly) { // keep live segment frozen if live
      getMainSegment().freeze = true;
    }
  }

  int tr = -1;
  tr = root[F("tb")] | -1;
  if (tr >= 0) timebase = ((uint32_t)tr) - millis();

  JsonObject nl       = root["nl"];
  nightlightActive    = nl["on"]      | nightlightActive;
  nightlightDelayMins = nl["dur"]     | nightlightDelayMins;
  nightlightMode      = nl["mode"]    | nightlightMode;
  nightlightTargetBri = nl[F("tbri")] | nightlightTargetBri;

  JsonObject udpn      = root["udpn"];
  notifyDirect         = udpn["send"] | notifyDirect;
  receiveNotifications = udpn["recv"] | receiveNotifications;
  if ((bool)udpn[F("nn")]) callMode = CALL_MODE_NO_NOTIFY; //send no notification just for this request

  unsigned long timein = root["time"] | UINT32_MAX; //backup time source if NTP not synced
  if (timein != UINT32_MAX) {
    // setTimeFromAPI(timein);
    // if (presetsModifiedTime == 0) presetsModifiedTime = timein;
  }

  if (root[F("psave")].isNull()) doReboot = root[F("rb")] | doReboot;

  // do not allow changing main segment while in realtime mode (may get odd results else)
  if (!realtimeMode) setMainSegmentId(root[F("mainseg")] | getMainSegmentId()); // must be before realtimeLock() if "live"

  realtimeOverride = root[F("lor")] | realtimeOverride;
  if (realtimeOverride > 2) realtimeOverride = REALTIME_OVERRIDE_ALWAYS;
  if (realtimeMode && useMainSegmentOnly) {
    getMainSegment().freeze = !realtimeOverride;
  }

  if (root.containsKey("live")) {
    if (root["live"].as<bool>()) {
      jsonTransitionOnce = true;
      realtimeLock(65000);
    } else {
      exitRealtime();
    }
  }

  int it = 0;
  JsonVariant segVar = root["seg"];
  if (segVar.is<JsonObject>())
  {

    DEBUG_LINE_HERE;

    int id = segVar["id"] | -1;
    //if "seg" is not an array and ID not specified, apply to all selected/checked segments
    if (id < 0) {
      //apply all selected segments
      //bool didSet = false;
      for (size_t s = 0; s < getSegmentsNum(); s++) {
    DEBUG_LINE_HERE;
        mAnimatorLight::Segment &sg = getSegment(s);
        if (sg.isSelected()) {
    DEBUG_LINE_HERE;
          deserializeSegment(segVar, s, presetId);
          //didSet = true;
        }
      }
      //TODO: not sure if it is good idea to change first active but unselected segment
      //if (!didSet) deserializeSegment(segVar, getMainSegmentId(), presetId);
    } else {
    DEBUG_LINE_HERE;
      deserializeSegment(segVar, id, presetId); //apply only the segment with the specified ID
    }
  } else {
    size_t deleted = 0;
    JsonArray segs = segVar.as<JsonArray>();
    for (JsonObject elem : segs) {
      if (deserializeSegment(elem, it++, presetId) && !elem["stop"].isNull() && elem["stop"]==0) deleted++;
    }
    if (getSegmentsNum() > 3 && deleted >= getSegmentsNum()/2U)
    {
      ALOG_WRN(PSTR("Deleting segments"));
      purgeSegments(); // batch deleting more than half segments
    }
  }

  // usermods.readFromJsonState(root);

  // loadLedmap = root[F("ledmap")] | loadLedmap;


  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  byte ps = root[F("psave")];
  if (ps > 0 && ps < 251) savePreset(ps, nullptr, root);
  ps = root[F("pdel")]; //deletion
  if (ps > 0 && ps < 251) deletePreset(ps);

  // HTTP API commands (must be handled before "ps")
  #ifdef ENABLE_DEVFEATURE_LIGHTING__SETTINGS   // setting with URL args, not what I want
  const char* httpwin = root["win"];
  if (httpwin) {
    String apireq = "win"; apireq += '&'; // reduce flash string usage
    apireq += httpwin;
    handleSet(nullptr, apireq, false);    // may set stateChanged
  }
  #endif // ENABLE_DEVFEATURE_LIGHTING__SETTINGS

  // applying preset (2 cases: a) API call includes all preset values ("pd"), b) API only specifies preset ID ("ps"))
  byte presetToRestore = 0;
  // a) already applied preset content (requires "seg" or "win" but will ignore the rest)
  if (!root["pd"].isNull() && stateChanged) {
    currentPreset = root[F("pd")] | currentPreset;
    if (root["win"].isNull()) presetCycCurr = currentPreset; // otherwise it was set in handleSet() [set.cpp]
    presetToRestore = currentPreset; // stateUpdated() will clear the preset, so we need to restore it after
    
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
      unloadPlaylist();// applying a preset unloads the playlist, may be needed here too?
      #endif
      
  } else if (!root["ps"].isNull()) {
    ps = presetCycCurr;
    if (root["win"].isNull() && getVal(root["ps"], &ps, 0, 0) && ps > 0 && ps < 251 && ps != currentPreset) {
      // b) preset ID only or preset that does not change state (use embedded cycling limits if they exist in getVal())
      presetCycCurr = ps;
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
      unloadPlaylist();          // applying a preset unloads the playlist
      #endif
      applyPreset(ps, callMode); // async load from file system (only preset ID was specified)
      return stateResponse;
    }
  }


  #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  JsonObject playlist = root[F("playlist")];
  if (!playlist.isNull() && loadPlaylist(playlist, presetId)) {
    //do not notify here, because the first playlist entry will do
    if (root["on"].isNull()) callMode = CALL_MODE_NO_NOTIFY;
    else callMode = CALL_MODE_DIRECT_CHANGE;  // possible bugfix for playlist only containing HTTP API preset FX=~
    ALOG_INF(PSTR("JsonObject playlist = root[F(\"playlist\")];"));
  }else{
    ALOG_DBM(PSTR("playlist.isNull()"));
  }
  DEBUG_PRINTLN("HERE");
  #endif

  if (root.containsKey(F("rmcpal")) && root[F("rmcpal")].as<bool>()) {
    if (customPalettes.size()) {
      char fileName[32];
      sprintf_P(fileName, PSTR("/palette%d.json"), customPalettes.size()-1);
      if (FILE_SYSTEM.exists(fileName)) FILE_SYSTEM.remove(fileName);
      loadCustomPalettes();
    }
  }

  // stateUpdated(callMode);
  if (presetToRestore) currentPreset = presetToRestore;
  
  #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS

  return stateResponse;
}


//helper to get int value with in/decrementing support via ~ syntax
void mAnimatorLight::parseNumber(const char* str, byte* val, byte minv, byte maxv)
{
  if (str == nullptr || str[0] == '\0') return;
  if (str[0] == 'r') {*val = random8(minv,maxv?maxv:255); return;} // maxv for random cannot be 0
  bool wrap = false;
  if (str[0] == 'w' && strlen(str) > 1) {str++; wrap = true;}
  if (str[0] == '~') {
    int out = atoi(str +1);
    if (out == 0) {
      if (str[1] == '0') return;
      if (str[1] == '-') {
        *val = (int)(*val -1) < (int)minv ? maxv : min((int)maxv,(*val -1)); //-1, wrap around
      } else {
        *val = (int)(*val +1) > (int)maxv ? minv : max((int)minv,(*val +1)); //+1, wrap around
      }
    } else {
      if (wrap && *val == maxv && out > 0) out = minv;
      else if (wrap && *val == minv && out < 0) out = maxv;
      else {
        out += *val;
        if (out > maxv) out = maxv;
        if (out < minv) out = minv;
      }
      *val = out;
    }
    return;
  } else if (minv == maxv && minv == 0) { // limits "unset" i.e. both 0
    byte p1 = atoi(str);
    const char* str2 = strchr(str,'~'); // min/max range (for preset cycle, e.g. "1~5~")
    if (str2) {
      byte p2 = atoi(++str2);           // skip ~
      if (p2 > 0) {
        while (isdigit(*(++str2)));     // skip digits
        parseNumber(str2, val, p1, p2);
        return;
      }
    }
  }
  *val = atoi(str);
}


bool mAnimatorLight::getVal(JsonVariant elem, byte* val, byte vmin, byte vmax) {
  if (elem.is<int>()) {
		if (elem < 0) return false; //ignore e.g. {"ps":-1}
    *val = elem;
    return true;
  } else if (elem.is<const char*>()) {
    const char* str = elem;
    size_t len = strnlen(str, 12);
    if (len == 0 || len > 10) return false;
    parseNumber(str, val, vmin, vmax);
    return true;
  }
  return false; //key does not exist
}

#endif // ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6



//contrary to the colorFromDecOrHexString() function, this uses the more standard RRGGBB / RRGGBBWW order
bool mAnimatorLight::colorFromHexString(byte* rgb, const char* in) {
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




void mAnimatorLight::toggleOnOff()
{
  if (pCONT_iLight->_briRGB_Global == 0)
  {
    pCONT_iLight->_briRGB_Global = briLast;
  } else
  {
    briLast = pCONT_iLight->_briRGB_Global;
    pCONT_iLight->_briRGB_Global = 0;
  }
  stateChanged = true;
}


//scales the brightness with the briMultiplier factor
byte mAnimatorLight::scaledBri(byte in)
{
  uint16_t val = ((uint16_t)in*briMultiplier)/100;
  if (val > 255) val = 255;
  return (byte)val;
}


void mAnimatorLight::realtimeLock(uint32_t timeoutMs, byte md)
{
  if (!realtimeMode && !realtimeOverride) {
    uint16_t stop, start;
    if (useMainSegmentOnly) {
      mAnimatorLight::Segment& mainseg = getMainSegment();
      start = mainseg.start;
      stop  = mainseg.stop;
      mainseg.freeze = true;
    } else {
      start = 0;
      stop  = getLengthTotal();
    }
    // clear strip/segment
    for (size_t i = start; i < stop; i++) setPixelColor(i,BLACK);
    // if WLED was off and using main segment only, freeze non-main segments so they stay off
    if (useMainSegmentOnly && pCONT_iLight->_briRGB_Global == 0) {
      for (size_t s=0; s < getSegmentsNum(); s++) {
        getSegment(s).freeze = true;
      }
    }
  }
  // if strip is off (bri==0) and not already in RTM
  if (briT == 0 && !realtimeMode && !realtimeOverride) {
    setBrightness(scaledBri(briLast), true);
  }

  if (realtimeTimeout != UINT32_MAX) {
    realtimeTimeout = (timeoutMs == 255001 || timeoutMs == 65000) ? UINT32_MAX : millis() + timeoutMs;
  }
  realtimeMode = md;

  if (realtimeOverride) return;
  if (arlsForceMaxBri) setBrightness(scaledBri(255), true);
  if (briT > 0 && md == REALTIME_MODE_GENERIC) show();
}

void mAnimatorLight::exitRealtime() {
  if (!realtimeMode) return;
  if (realtimeOverride == REALTIME_OVERRIDE_ONCE) realtimeOverride = REALTIME_OVERRIDE_NONE;
  setBrightness(scaledBri(pCONT_iLight->_briRGB_Global), true);
  realtimeTimeout = 0; // cancel realtime mode immediately
  realtimeMode = REALTIME_MODE_INACTIVE; // inform UI immediately
  realtimeIP[0] = 0;
  if (useMainSegmentOnly) { // unfreeze live segment again
    getMainSegment().freeze = false;
  }
  // updateInterfaces(CALL_MODE_WS_SEND);
}


//simple macro for ArduinoJSON's or syntax
#define CJSON(a,b) a = b | a

void mAnimatorLight::getStringFromJson(char* dest, const char* src, size_t len) {
  if (src != nullptr) strlcpy(dest, src, len);
}

bool mAnimatorLight::deserializeConfig(JsonObject doc, bool fromFS) {
  bool needsSave = false;

  #ifdef WLED_USE_ETHERNET
  JsonObject ethernet = doc[F("eth")];
  CJSON(ethernetType, ethernet["type"]);
  // NOTE: Ethernet configuration takes priority over other use of pins
  WLED::instance().initEthernet();
  #endif

  JsonObject id = doc["id"];
  getStringFromJson(serverDescription, id[F("name")], 33);
  #ifdef WLED_ENABLE_SIMPLE_UI
    CJSON(simplifiedUI, id[F("sui")]);
  #endif

  JsonObject nw_ins_0 = doc["nw"]["ins"][0];
  getStringFromJson(clientSSID, nw_ins_0[F("ssid")], 33);
  //int nw_ins_0_pskl = nw_ins_0[F("pskl")];
  //The WiFi PSK is normally not contained in the regular file for security reasons.
  //If it is present however, we will use it
  getStringFromJson(clientPass, nw_ins_0["psk"], 65);

  JsonArray nw_ins_0_ip = nw_ins_0["ip"];
  JsonArray nw_ins_0_gw = nw_ins_0["gw"];
  JsonArray nw_ins_0_sn = nw_ins_0["sn"];

  for (byte i = 0; i < 4; i++) {
    CJSON(staticIP[i], nw_ins_0_ip[i]);
    CJSON(staticGateway[i], nw_ins_0_gw[i]);
    CJSON(staticSubnet[i], nw_ins_0_sn[i]);
  }

  JsonObject ap = doc["ap"];
  getStringFromJson(apSSID, ap[F("ssid")], 33);
  getStringFromJson(apPass, ap["psk"] , 65); //normally not present due to security
  //int ap_pskl = ap[F("pskl")];

  CJSON(apChannel, ap[F("chan")]);
  if (apChannel > 13 || apChannel < 1) apChannel = 1;

  CJSON(apHide, ap[F("hide")]);
  if (apHide > 1) apHide = 1;

  CJSON(apBehavior, ap[F("behav")]);

  noWifiSleep = doc[F("wifi")][F("sleep")] | !noWifiSleep; // inverted
  noWifiSleep = !noWifiSleep;
  //int wifi_phy = doc[F("wifi")][F("phy")]; //force phy mode n?

  JsonObject hw = doc[F("hw")];

  // initialize LED pins and lengths prior to other HW (except for ethernet)
  JsonObject hw_led = hw["led"];

  uint8_t autoWhiteMode = RGBW_MODE_MANUAL_ONLY;
  CJSON(ablMilliampsMax, hw_led[F("maxpwr")]);
  CJSON(milliampsPerLed, hw_led[F("ledma")]);

  #ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
  Bus::setGlobalAWMode(hw_led[F("rgbwm")] | 255);
  #endif // ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES

  CJSON(correctWB, hw_led["cct"]);
  CJSON(cctFromRgb, hw_led[F("cr")]);
  CJSON(cctBlending, hw_led[F("cb")]);

  // Bus::setCCTBlend(cctBlending);

  setTargetFps(hw_led["fps"]); //NOP if 0, default 42 FPS
  CJSON(useLedsArray, hw_led[F("ld")]);

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  // 2D Matrix Settings
  JsonObject matrix = hw_led[F("matrix")];
  if (!matrix.isNull()) {
    isMatrix = true;
    CJSON(panels, matrix[F("mpc")]);
    panel.clear();
    JsonArray panels_j = matrix[F("panels")];
    uint8_t s = 0;
    if (!panels_j.isNull()) {
      panel.reserve(max(1U,min((size_t)panels,(size_t)WLED_MAX_PANELS)));  // pre-allocate memory for panels
      for (JsonObject pnl : panels_j) {
        Panel p;
        CJSON(p.bottomStart, pnl["b"]);
        CJSON(p.rightStart,  pnl["r"]);
        CJSON(p.vertical,    pnl["v"]);
        CJSON(p.serpentine,  pnl["s"]);
        CJSON(p.xOffset,     pnl["x"]);
        CJSON(p.yOffset,     pnl["y"]);
        CJSON(p.height,      pnl["h"]);
        CJSON(p.width,       pnl["w"]);
        panel.push_back(p);
        if (++s >= WLED_MAX_PANELS || s >= panels) break; // max panels reached
      }
    } else {
      // fallback
      Panel p;
      panels = 1;
      p.height = p.width = 8;
      p.xOffset = p.yOffset = 0;
      p.options = 0;
      panel.push_back(p);
    }
    // cannot call setUpMatrix() here due to already locked JSON buffer
  }
  #endif

  JsonArray ins = hw_led["ins"];

  if (fromFS || !ins.isNull()) {
    uint8_t s = 0;  // bus iterator
    if (fromFS) pCONT_iLight->bus_manager->removeAll(); // can't safely manipulate busses directly in network callback
    uint32_t mem = 0;
    bool busesChanged = false;
    for (JsonObject elm : ins) {
      if (s >= WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES) break;
      uint8_t pins[5] = {255, 255, 255, 255, 255};
      JsonArray pinArr = elm["pin"];
      if (pinArr.size() == 0) continue;
      pins[0] = pinArr[0];
      uint8_t i = 0;
      for (int p : pinArr) {
        pins[i++] = p;
        if (i>4) break;
      }

      // uint16_t length = elm["len"] | 1;
      // uint8_t colorOrder = (int)elm[F("order")];
      // uint8_t skipFirst = elm[F("skip")];
      // uint16_t start = elm["start"] | 0;
      // if (length==0 || start + length > MAX_LEDS) continue; // zero length or we reached max. number of LEDs, just stop
      // uint8_t ledType = elm["type"] | TYPE_WS2812_RGB;
      // bool reversed = elm["rev"];
      // bool refresh = elm["ref"] | false;
      // uint16_t freqkHz = elm[F("freq")] | 0;  // will be in kHz for DotStar and Hz for PWM (not yet implemented fully)
      // ledType |= refresh << 7; // hack bit 7 to indicate strip requires off refresh
      // uint8_t AWmode = elm[F("rgbwm")] | autoWhiteMode;
      // if (fromFS) {
      //   BusConfig bc = BusConfig(ledType, pins, start, length, colorOrder, reversed, skipFirst, AWmode, freqkHz);
      //   mem += BusManager::memUsage(bc);
      //   if (mem <= MAX_LED_MEMORY) if (busses.add(bc) == -1) break;  // finalization will be done in WLED::beginStrip()
      // } else {
      //   if (pCONT_iLight->busConfigs[s] != nullptr) delete pCONT_iLight->busConfigs[s];
      //   pCONT_iLight->busConfigs[s] = new mInterfaceLight::BusConfig(ledType, pins, start, length, colorOrder, reversed, skipFirst, AWmode);
      //   busesChanged = true;
      // }
      s++;
    }
    doInitBusses = busesChanged;
    // finalization done in beginStrip()
  }
  if (hw_led["rev"]) pCONT_iLight->bus_manager->getBus(0)->setReversed(true); //set 0.11 global reversed setting for first bus

  // read color order map configuration
  JsonArray hw_com = hw[F("com")];
  if (!hw_com.isNull()) {
    ColorOrderMap com = {};
    uint8_t s = 0;
    for (JsonObject entry : hw_com) {
      if (s > WLED_MAX_COLOR_ORDER_MAPPINGS) break;
      uint16_t start = entry["start"] | 0;
      uint16_t len = entry["len"] | 0;
      uint8_t colorOrder = (int)entry[F("order")];
      // com.add(start, len, colorOrder);
      s++;
    }
    pCONT_iLight->bus_manager->updateColorOrderMap(com);
  }

  CJSON(irEnabled, hw["ir"]["type"]);
  CJSON(irApplyToAllSelected, hw["ir"]["sel"]);

  JsonObject light = doc[F("light")];
  CJSON(briMultiplier, light[F("scale-bri")]);
  CJSON(paletteBlend, light[F("pal-mode")]);
  CJSON(autoSegments, light[F("aseg")]);

  CJSON(gammaCorrectVal, light["gc"]["val"]); // default 2.8
  float light_gc_bri = light["gc"]["bri"];
  float light_gc_col = light["gc"]["col"];

  JsonObject light_tr = light["tr"];
  CJSON(fadeTransition, light_tr["mode"]);
  int tdd = light_tr["dur"] | -1;
  // if (tdd >= 0) transitionDelay = transitionDelayDefault = tdd * 100;
  CJSON(paletteFade, light_tr["pal"]);
  CJSON(randomPaletteChangeTime, light_tr[F("rpc")]);

  JsonObject light_nl = light["nl"];
  CJSON(nightlightMode, light_nl["mode"]);
  byte prev = nightlightDelayMinsDefault;
  CJSON(nightlightDelayMinsDefault, light_nl["dur"]);
  if (nightlightDelayMinsDefault != prev) nightlightDelayMins = nightlightDelayMinsDefault;

  CJSON(nightlightTargetBri, light_nl[F("tbri")]);
  // CJSON(macroNl, light_nl["macro"]);

  JsonObject def = doc["def"];
  CJSON(bootPreset, def["ps"]);
  CJSON(turnOnAtBoot, def["on"]); // true
  CJSON(pCONT_iLight->_briRGB_Global, def["bri"]); // 128

  JsonObject interfaces = doc["if"];

  JsonObject if_sync = interfaces["sync"];
  CJSON(udpPort, if_sync[F("port0")]); // 21324
  CJSON(udpPort2, if_sync[F("port1")]); // 65506

  JsonObject if_sync_recv = if_sync["recv"];
  CJSON(receiveNotificationBrightness, if_sync_recv["bri"]);
  CJSON(receiveNotificationColor, if_sync_recv["col"]);
  CJSON(receiveNotificationEffects, if_sync_recv["fx"]);
  CJSON(receiveGroups, if_sync_recv["grp"]);
  CJSON(receiveSegmentOptions, if_sync_recv["seg"]);
  CJSON(receiveSegmentBounds, if_sync_recv["sb"]);
  //! following line might be a problem if called after boot
  receiveNotifications = (receiveNotificationBrightness || receiveNotificationColor || receiveNotificationEffects || receiveSegmentOptions);

  JsonObject if_sync_send = if_sync["send"];
  prev = notifyDirectDefault;
  CJSON(notifyDirectDefault, if_sync_send[F("dir")]);
  if (notifyDirectDefault != prev) notifyDirect = notifyDirectDefault;
  CJSON(notifyButton, if_sync_send["btn"]);
  CJSON(notifyAlexa, if_sync_send["va"]);
  CJSON(notifyHue, if_sync_send["hue"]);
  CJSON(notifyMacro, if_sync_send["macro"]);
  CJSON(syncGroups, if_sync_send["grp"]);
  if (if_sync_send[F("twice")]) udpNumRetries = 1; // import setting from 0.13 and earlier
  CJSON(udpNumRetries, if_sync_send["ret"]);

  JsonObject if_nodes = interfaces["nodes"];
  CJSON(nodeListEnabled, if_nodes[F("list")]);
  CJSON(nodeBroadcastEnabled, if_nodes[F("bcast")]);

  JsonObject if_live = interfaces["live"];
  CJSON(receiveDirect, if_live["en"]);
  CJSON(useMainSegmentOnly, if_live[F("mso")]);
  CJSON(e131Port, if_live["port"]); // 5568
  // if (e131Port == DDP_DEFAULT_PORT) e131Port = E131_DEFAULT_PORT; // prevent double DDP port allocation
  CJSON(e131Multicast, if_live[F("mc")]);

  JsonObject if_live_dmx = if_live[F("dmx")];
  CJSON(e131Universe, if_live_dmx[F("uni")]);
  CJSON(e131SkipOutOfSequence, if_live_dmx[F("seqskip")]);
  CJSON(DMXAddress, if_live_dmx[F("addr")]);
  if (!DMXAddress || DMXAddress > 510) DMXAddress = 1;
  CJSON(DMXSegmentSpacing, if_live_dmx[F("dss")]);
  if (DMXSegmentSpacing > 150) DMXSegmentSpacing = 0;
  // CJSON(e131Priority, if_live_dmx[F("e131prio")]);
  // if (e131Priority > 200) e131Priority = 200;
  CJSON(DMXMode, if_live_dmx["mode"]);

  tdd = if_live[F("timeout")] | -1;
  if (tdd >= 0) realtimeTimeoutMs = tdd * 100;
  CJSON(arlsForceMaxBri, if_live[F("maxbri")]);
  CJSON(arlsDisableGammaCorrection, if_live[F("no-gc")]); // false
  CJSON(arlsOffset, if_live[F("offset")]); // 0

  #ifdef ENABLE_FEATURE_LIGHTING__DMX
  JsonObject dmx = doc["dmx"];
  CJSON(DMXChannels, dmx[F("chan")]);
  CJSON(DMXGap,dmx[F("gap")]);
  CJSON(DMXStart, dmx["start"]);
  CJSON(DMXStartLED,dmx[F("start-led")]);

  JsonArray dmx_fixmap = dmx[F("fixmap")];
  for (int i = 0; i < dmx_fixmap.size(); i++) {
    if (i > 14) break;
    CJSON(DMXFixtureMap[i],dmx_fixmap[i]);
  }

  CJSON(e131ProxyUniverse, dmx[F("e131proxy")]);
  #endif

  DEBUG_PRINTLN(F("Starting usermod config."));
  JsonObject usermods_settings = doc["um"];
  if (!usermods_settings.isNull()) {
    // needsSave = !usermods.readFromConfig(usermods_settings);
  }

  if (fromFS) return needsSave;
  // if from /json/cfg
  doReboot = doc[F("rb")] | doReboot;
  if (doInitBusses) return false; // no save needed, will do after bus init in wled.cpp loop
  return (doc["sv"] | true);
}


//macro to convert F to const
#define SET_F(x)  (const char*)F(x)

bool mAnimatorLight::oappend(const char* txt)
{
  uint16_t len = strlen(txt);
  if (olen + len >= SETTINGS_STACK_BUF_SIZE)
    return false;        // buffer full
  strcpy(obuf + olen, txt);
  olen += len;
  return true;
}


bool mAnimatorLight::oappendi(int i)
{
  char s[11];
  sprintf(s, "%d", i);
  return oappend(s);
}


//append a string setting to buffer
void mAnimatorLight::sappends(char stype, const char* key, char* val)
{
  switch(stype)
  {
    case 's': {//string (we can interpret val as char*)
      String buf = val;
      //convert "%" to "%%" to make EspAsyncWebServer happy
      //buf.replace("%","%%");
      oappend("d.Sf.");
      oappend(key);
      oappend(".value=\"");
      oappend(buf.c_str());
      oappend("\";");
      break;}
    case 'm': //message
      oappend(SET_F("d.getElementsByClassName"));
      oappend(key);
      oappend(SET_F(".innerHTML=\""));
      oappend(val);
      oappend("\";");
      break;
  }
}

//append a numeric setting to string buffer
void mAnimatorLight::sappend(char stype, const char* key, int val)
{
  char ds[] = "d.Sf.";

  switch(stype)
  {
    case 'c': //checkbox
      oappend(ds);
      oappend(key);
      oappend(".checked=");
      oappendi(val);
      oappend(";");
      break;
    case 'v': //numeric
      oappend(ds);
      oappend(key);
      oappend(".value=");
      oappendi(val);
      oappend(";");
      break;
    case 'i': //selectedIndex
      oappend(ds);
      oappend(key);
      oappend(SET_F(".selectedIndex="));
      oappendi(val);
      oappend(";");
      break;
  }
}


bool mAnimatorLight::deserializeSegment(JsonObject elem, byte it, byte presetId)
{

  ALOG_INF(PSTR("================deserializeSegment"));

  byte id = elem["id"] | it;
  if (id >= getMaxSegments())
  {
    ALOG_INF(PSTR("deserializeSegment id >= getMaxSegments()"));
    return false;
  }

  int stop = elem["stop"] | -1;

  // if using vectors use this code to append segment
  if (id >= getSegmentsNum()) {

    if (stop <= 0) return false; // ignore empty/inactive segments

    ALOG_HGL(PSTR("DESTROYING SEGMENT, BAD %d %d"), id, getSegmentsNum());

    appendSegment(mAnimatorLight::Segment(0, getLengthTotal()));
    id = getSegmentsNum()-1; // segments are added at the end of list
  }

  mAnimatorLight::Segment& seg = getSegment(id);

  /**
   * @brief Note that making a backup will also allocate memory, this will be destroyed when we leave this function
   * 
   */
  mAnimatorLight::Segment prev = seg; //make a backup so we can tell if something changed

  uint16_t start = elem["start"] | seg.start;
  if (stop < 0) {
    uint16_t len = elem["len"];
    stop = (len > 0) ? start + len : seg.stop;
  }

  // 2D segments
  uint16_t startY = elem["startY"] | seg.startY;
  uint16_t stopY = elem["stopY"] | seg.stopY;

  //repeat, multiplies segment until all LEDs are used, or max segments reached
  bool repeat = elem["rpt"] | false;
  if (repeat && stop>0) {
    elem.remove("id");  // remove for recursive call
    elem.remove("rpt"); // remove for recursive call
    elem.remove("n");   // remove for recursive call
    uint16_t len = stop - start;
    for (size_t i=id+1; i<getMaxSegments(); i++) {
      start = start + len;
      if (start >= getLengthTotal()) break;
      //TODO: add support for 2D
      elem["start"] = start;
      elem["stop"]  = start + len;
      elem["rev"]   = !elem["rev"]; // alternate reverse on even/odd segments
      DEBUG_LINE_HERE;
      deserializeSegment(elem, i, presetId); // recursive call with new id
    }
    return true;
  }

  if (elem["n"]) {
    // name field exists
    if (seg.name) { //clear old name
      delete[] seg.name;
      seg.name = nullptr;
    }

    const char * name = elem["n"].as<const char*>();
    size_t len = 0;
    if (name != nullptr) len = strlen(name);
    if (len > 0) {
      if (len > WLED_MAX_SEGNAME_LEN) len = WLED_MAX_SEGNAME_LEN;
      seg.name = new char[len+1];
      if (seg.name) strlcpy(seg.name, name, WLED_MAX_SEGNAME_LEN+1);
    } else {
      // but is empty (already deleted above)
      elem.remove("n");
    }
  } else if (start != seg.start || stop != seg.stop) {
    // clearing or setting segment without name field
    if (seg.name) {
      delete[] seg.name;
      seg.name = nullptr;
    }
  }

  uint16_t grp = elem["grp"] | seg.grouping;
  uint16_t spc = elem[F("spc")] | seg.spacing;
  uint16_t of  = seg.offset;
  uint8_t  soundSim = elem["si"] | seg.soundSim;
  uint8_t  map1D2D  = elem["m12"] | seg.map1D2D;

  if ((spc>0 && spc!=seg.spacing) || seg.map1D2D!=map1D2D) seg.fill(BLACK); // clear spacing gaps

  seg.map1D2D  = constrain(map1D2D, 0, 7);
  seg.soundSim = constrain(soundSim, 0, 1);

  uint8_t set = elem[F("set")] | 0;//seg.set;   // unknown, phase into newer WLED UI
  // seg.set = constrain(set, 0, 3);

  uint16_t len = 1;
  if (stop > start) len = stop - start;
  int offset = elem[F("of")] | INT32_MAX;
  if (offset != INT32_MAX) {
    int offsetAbs = abs(offset);
    if (offsetAbs > len - 1) offsetAbs %= len;
    if (offset < 0) offsetAbs = len - offsetAbs;
    of = offsetAbs;
  }
  if (stop > start && of > len -1) of = len -1;
  seg.setUp(start, stop, grp, spc, of, startY, stopY);

  if (seg.reset && seg.stop == 0) return true; // segment was deleted & is marked for reset, no need to change anything else

  byte segbri = 255;//seg.opacity;
  if (getVal(elem["bri"], &segbri)) {
    // ALOG_INF(PSTR("getVal(elem[\"bri\"], &segbri) %d"), segbri);
    // if (segbri > 0) seg.setOpacity(segbri);
    // seg.setOption(SEG_OPTION_ON, segbri); // use transition
  }

  if (getVal(elem["cBri"], &seg._brightness_rgb)) {
    ALOG_INF(PSTR("getVal(elem[\"cBri\"], &seg._brightness_rgb) %d"), seg._brightness_rgb);
    // if (segbri > 0) seg.setOpacity(segbri);
    // seg.setOption(SEG_OPTION_ON, segbri); // use transition
  }
  if (getVal(elem["wBri"], &seg._brightness_cct)) {
    ALOG_INF(PSTR("getVal(elem[\"wBri\"], &seg._brightness_cct) %d"), seg._brightness_cct);
    // if (segbri > 0) seg.setOpacity(segbri);
    // seg.setOption(SEG_OPTION_ON, segbri); // use transition
  }


  bool on = elem["on"] | seg.on;
  if (elem["on"].is<const char*>() && elem["on"].as<const char*>()[0] == 't') on = !on;
  seg.setOption(SEG_OPTION_ON, on); // use transition
  bool frz = elem["frz"] | seg.freeze;
  if (elem["frz"].is<const char*>() && elem["frz"].as<const char*>()[0] == 't') frz = !seg.freeze;
  seg.freeze = frz;


  seg.setBrightnessRGB(elem["rgbbri"] | seg._brightness_rgb);
  seg.setBrightnessCCT(elem["cctbri"] | seg._brightness_cct);

  seg.setCCT(elem["cct"] | seg.cct_slider);

  JsonArray colarr = elem["col"];
  if (!colarr.isNull())
  {

    // ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "JsonArray colarr"));

    if (seg.getLightCapabilities() & 3) {
      // segment has RGB or White
      for (size_t i = 0; i < 5; i++)
      {
        int rgbw[] = {0,0,0,0};
        bool colValid = false;
        JsonArray colX = colarr[i];
        if (colX.isNull()) {
          byte brgbw[] = {0,0,0,0};
          const char* hexCol = colarr[i];
          if (hexCol == nullptr) { //Kelvin color temperature (or invalid), e.g 2400
            int kelvin = colarr[i] | -1;
            if (kelvin <  0) continue;
            if (kelvin == 0)
            {
              ALOG_INF(PSTR("seg.setColor(i, 0); %d"), i);
              seg.setColor(i, 0);
            }
            if (kelvin >  0) colorKtoRGB(kelvin, brgbw);
            colValid = true;
          } else { //HEX string, e.g. "FFAA00"
            colValid = colorFromHexString(brgbw, hexCol);
          }
          for (size_t c = 0; c < 4; c++) rgbw[c] = brgbw[c];
        } else { //Array of ints (RGB or RGBW color), e.g. [255,160,0]
          byte sz = colX.size();
          if (sz == 0) continue; //do nothing on empty array

          copyArray(colX, rgbw, 4);
          colValid = true;
        }

        if (!colValid) continue;

        ALOG_INF(PSTR("seg.setColor(%d, RGBW32(rgbw[%d],rgbw[%d],rgbw[%d],rgbw[%d]));"),i, rgbw[0], rgbw[1], rgbw[2], rgbw[3]);
    
        seg.setColor(i, RGBW32(rgbw[0],rgbw[1],rgbw[2],rgbw[3]));
        if (seg.animation_mode_id == 0) force_update(); //instant refresh
      }
    } else {
      // non RGB & non White segment (usually On/Off bus)
      seg.setColor(0, ULTRAWHITE);
      seg.setColor(1, BLACK);
    }
  }

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  bool reverse  = seg.reverse;
  bool mirror   = seg.mirror;
  #endif
  seg.selected  = elem["sel"] | seg.selected;
  seg.reverse   = elem["rev"] | seg.reverse;
  seg.mirror    = elem["mi"]  | seg.mirror;
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  bool reverse_y = seg.reverse_y;
  bool mirror_y  = seg.mirror_y;
  seg.reverse_y  = elem["rY"]  | seg.reverse_y;
  seg.mirror_y   = elem["mY"]  | seg.mirror_y;
  seg.transpose  = elem[F("tp")] | seg.transpose;
  if (seg.is2D() && seg.map1D2D == M12_pArc && (reverse != seg.reverse || reverse_y != seg.reverse_y || mirror != seg.mirror || mirror_y != seg.mirror_y)) seg.fill(BLACK); // clear entire segment (in case of Arc 1D to 2D expansion)
  #endif

  byte fx = seg.animation_mode_id;

  /**
   * @brief Adding support for JSON to be specified by its name, not just ID. 
   * Useful for manual, it may be better for auto saving inside too to prevent errors when building different levels of effects   * 
   */
  if(elem["fx"].is<const char*>())
  {
    ALOG_INF(PSTR("elem[\"fx\"].is<const char*>() == STRING"));
    const char* fxName = elem["fx"];
    ALOG_INF(PSTR("fxName %s"), fxName);


    int16_t tmp_id = -1;
    if((tmp_id=GetFlasherFunctionIDbyName(fxName))>=0)
    {
      CommandSet_Flasher_FunctionID(tmp_id, id);
    }



  }else
  {
    ALOG_INF(PSTR("elem[\"fx\"].is<const char*>() == NUMBER"));
    if (getVal(elem["fx"], &fx, 0, getModeCount())) { //load effect ('r' random, '~' inc/dec, 0-255 exact value)
      ALOG_INF(PSTR("getVal(elem[\"fx\"], &fx, 0, getModeCount()) %d"), fx);
      

      #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
      if (!presetId && currentPlaylist>=0) unloadPlaylist(); // applying a preset unloads the playlist, may be needed here too?
      #endif


      // if (fx != seg.animation_mode_id)
      DEBUG_LINE_HERE; 
      seg.setMode(fx, elem[F("fxdef")]);
    }
  }









  //getVal also supports inc/decrementing and random
  getVal(elem["sx"], &seg.speed);
  getVal(elem["ix"], &seg.intensity);

  // uint8_t transition_slider_time = 0;
  // getVal(elem["tt"], &transition_slider_time);
  // Map scale into internal rate
  
  #ifndef ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
  if (elem["tt"].is<int>()) {
    seg.time_ms = elem["tt"];//map(transition_slider_time, 0,255, 0,10000);
    ALOG_INF(PSTR("seg.time_ms = %d"), seg.time_ms);
  }
  #endif // ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

  // uint8_t transition_slider_rate = 0;
  // getVal(elem["tr"], &transition_slider_rate);
  // Map scale into internal rate
  if (elem["ep"].is<int>()) {
    seg.cycle_time__rate_ms = elem["ep"];//map(transition_slider_rate, 0,255, 0,10000);
    ALOG_INF(PSTR("seg.cycle_time__rate_ms = %d"), seg.cycle_time__rate_ms);
  }

  // getVal(elem["tr"], &seg.cycle_time__rate_ms);

  uint8_t pal = seg.palette_id;
  // if (seg.getLightCapabilities() & 1) {  // ignore palette for White and On/Off segments 
  //   ALOG_INF(PSTR("getVal(elem[\"pal\"], &pal)"));


  ALOG_HGL(PSTR("elem[\"pal\"].is<const char*>() %d"), elem["pal"].is<const char*>());

  if(elem["pal"].is<const char*>())
  {

    const char* palName = elem["pal"];

    int16_t tmp_id = -1;
    if((tmp_id=GetPaletteIDbyName((char*)palName))>=0){
      ALOG_INF(PSTR("tmp_id=%d"),tmp_id);
      CommandSet_PaletteID(tmp_id, id);
    }

  }else{

    if (getVal(elem["pal"], &pal)) seg.setPalette(pal);


  }



  // }

  getVal(elem["c1"], &seg.custom1);
  getVal(elem["c2"], &seg.custom2);
  uint8_t cust3 = seg.custom3;
  getVal(elem["c3"], &cust3); // we can't pass reference to bifield
  seg.custom3 = constrain(cust3, 0, 31);


  seg.params_user[0] = elem["v0"] | 0;
  seg.params_user[1] = elem["v1"] | 0;
  seg.params_user[2] = elem["v2"] | 0;
  seg.params_user[3] = elem["v3"] | 0;




  seg.check1 = elem["o1"] | seg.check1;
  seg.check2 = elem["o2"] | seg.check2;
  seg.check3 = elem["o3"] | seg.check3;

  JsonArray iarr = elem[F("i")]; //set individual LEDs
  if (!iarr.isNull()) {
    uint8_t oldMap1D2D = seg.map1D2D;
    seg.map1D2D = M12_Pixels; // no mapping

    // set brightness immediately and disable transition
    // transitionDelayTemp = 0;
    jsonTransitionOnce = true;

    // freeze and init to black
    if (!seg.freeze) {
      seg.freeze = true;
      seg.fill(BLACK);
    }

    uint16_t start = 0, stop = 0;
    byte set = 0; //0 nothing set, 1 start set, 2 range set

    for (size_t i = 0; i < iarr.size(); i++) {
      if(iarr[i].is<JsonInteger>()) {
        if (!set) {
          start = abs(iarr[i].as<int>());
          set++;
        } else {
          stop = abs(iarr[i].as<int>());
          set++;
        }
      } else { //color
        uint8_t rgbw[] = {0,0,0,0};
        JsonArray icol = iarr[i];
        if (!icol.isNull()) { //array, e.g. [255,0,0]
          byte sz = icol.size();
          if (sz > 0 && sz < 5) copyArray(icol, rgbw);
        } else { //hex string, e.g. "FF0000"
          byte brgbw[] = {0,0,0,0};
          const char* hexCol = iarr[i];
          if (colorFromHexString(brgbw, hexCol)) {
            for (size_t c = 0; c < 4; c++) rgbw[c] = brgbw[c];
          }
        }

        if (set < 2 || stop <= start) stop = start + 1;
        uint32_t c = gamma32(RGBW32(rgbw[0], rgbw[1], rgbw[2], rgbw[3]));
        while (start < stop) seg.setPixelColor(start++, c);
        set = 0;
      }
    }
    seg.map1D2D = oldMap1D2D; // restore mapping
    force_update(); // force segment update
  }

  
  // send UDP/WS if segment options changed (except selection; will also deselect current preset)
  if (seg.differs(prev) & 0x7F) stateChanged = true;

  seg.effect_anim_section = 0; //reset

  return true;
}
          

#ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI

#ifndef ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
bool mAnimatorLight::handleFileRead(AsyncWebServerRequest* request, String path){
  DEBUG_PRINTLN("WS FileRead: " + path);
  DEBUG_LINE_HERE;
  if(path.endsWith("/")) path += "index.htm";
  DEBUG_LINE_HERE;
  if(path.indexOf("sec") > -1) return false;
  DEBUG_LINE_HERE;
  String contentType = getContentType(request, path);
  DEBUG_LINE_HERE;
  /*String pathWithGz = path + ".gz";
  if(FILE_SYSTEM.exists(pathWithGz)){
    request->send(FILE_SYSTEM, pathWithGz, contentType);
    return true;
  }*/
  ALOG_INF(PSTR("mAnimatorLight::handleFileReadA"));
  if(FILE_SYSTEM.exists(path)) {
  ALOG_INF(PSTR("mAnimatorLight::handleFileReadB"));
    request->send(FILE_SYSTEM, path, contentType);
    return true;
  }
  ALOG_INF(PSTR("mAnimatorLight::handleFileReadC"));
  return false;
}
#endif// ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE




void mAnimatorLight::serveIndex(AsyncWebServerRequest* request)
{
  if (pCONT_mfile->handleFileRead(request, "/index.htm"))
  {
    return;
  }

  if (tkr_web->handleIfNoneMatchCacheHeader(request, 200)) //tmp fix of 200, phasing out
  {
    return;
  }

  AsyncWebServerResponse *response;
  #ifdef WLED_ENABLE_SIMPLE_UI
  if (simplifiedUI)
    response = request->beginResponse_P(200, "text/html", PAGE_simple, PAGE_simple_L);
  else
  #endif
    response = request->beginResponse_P(200, "text/html", PAGE_index, PAGE_index_L);

  response->addHeader(FPSTR(s_content_enc),"gzip");
  tkr_web->setStaticContentCacheHeaders(response);
  request->send(response);
}



#ifndef ENABLE_FEATURE_LIGHTING__XML_REQUESTS  // phase out

//get values for settings form in javascript
void mAnimatorLight::getSettingsJS(byte subPage, char* dest)
{
  //0: menu 1: wifi 2: leds 3: ui 4: sync 5: time 6: sec
  DEBUG_PRINT(F("settings resp"));
  DEBUG_PRINTLN(subPage);
  obuf = dest;
  olen = 0;

  if (subPage <0 || subPage >10) return;

  if (subPage == SUBPAGE_MENU)
  {
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS // include only if 2D is compiled in
    oappend(PSTR("gId('2dbtn').style.display='';"));
  #endif
  #ifdef ENABLE_FEATURE_LIGHTING__DMX // include only if DMX is enabled
    oappend(PSTR("gId('dmxbtn').style.display='';"));
  #endif
  }

  if (subPage == SUBPAGE_WIFI)
  {
    sappends('s',SET_F("CS"),"clientSSID");

    byte l = strlen(clientPass);
    char fpass[l+1]; //fill password field with ***
    fpass[l] = 0;
    memset(fpass,'*',l);
    sappends('s',SET_F("CP"),fpass);

    char k[3]; k[2] = 0; //IP addresses
    for (int i = 0; i<4; i++)
    {
      k[1] = 48+i; //ascii 0,1,2,3
      k[0] = 'I'; sappend('v',k,staticIP[i]);
      k[0] = 'G'; sappend('v',k,staticGateway[i]);
      k[0] = 'S'; sappend('v',k,staticSubnet[i]);
    }

    sappends('s',SET_F("CM"),tkr_set->Settings.system_name.device);
    sappend('i',SET_F("AB"),apBehavior);
    sappends('s',SET_F("AS"),apSSID);
    sappend('c',SET_F("AH"),apHide);

    l = strlen(apPass);
    char fapass[l+1]; //fill password field with ***
    fapass[l] = 0;
    memset(fapass,'*',l);
    sappends('s',SET_F("AP"),fapass);

    sappend('v',SET_F("AC"),apChannel);
    sappend('c',SET_F("WS"),noWifiSleep);

    #ifndef WLED_DISABLE_ESPNOW
    sappend('c',SET_F("RE"),enable_espnow_remote);
    sappends('s',SET_F("RMAC"),linked_remote);
    #else
    //hide remote settings if not compiled
    oappend(SET_F("document.getElementById('remd').style.display='none';"));
    #endif

    #ifdef WLED_USE_ETHERNET
    sappend('v',SET_F("ETH"),ethernetType);
    #else
    //hide ethernet setting if not compiled in
    oappend(SET_F("document.getElementById('ethd').style.display='none';"));
    #endif

    if (Network.isConnected()) //is connected
    {
      char s[32];
      IPAddress localIP = Network.localIP();
      sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

      #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_ETHERNET)
      if (Network.isEthernet()) strcat_P(s ,SET_F(" (Ethernet)"));
      #endif
      sappends('m',SET_F("(\"sip\")[0]"),s);
    } else
    {
      sappends('m',SET_F("(\"sip\")[0]"),(char*)F("Not connected"));
    }

    if (WiFi.softAPIP()[0] != 0) //is active
    {
      char s[16];
      IPAddress apIP = WiFi.softAPIP();
      sprintf(s, "%d.%d.%d.%d", apIP[0], apIP[1], apIP[2], apIP[3]);
      sappends('m',SET_F("(\"sip\")[1]"),s);
    } else
    {
      sappends('m',SET_F("(\"sip\")[1]"),(char*)F("Not active"));
    }

    #ifndef WLED_DISABLE_ESPNOW
    if (last_signal_src[0] != 0) //Have seen an ESP-NOW Remote
    {
      sappends('m',SET_F("(\"rlid\")[0]"),last_signal_src);
    } else if (!enable_espnow_remote)
    {
      sappends('m',SET_F("(\"rlid\")[0]"),(char*)F("(Enable remote to listen)"));
    } else 
    {
      sappends('m',SET_F("(\"rlid\")[0]"),(char*)F("None"));
    }
    #endif
  }

  if (subPage == SUBPAGE_LEDS)
  {
    char nS[32];

    // appendGPIOinfo();

    // set limits
    // oappend(SET_F("bLimits("));
    // oappend(itoa(WLED_MAX_BUSSES,nS,10));  oappend(",");
    // oappend(itoa(WLED_MIN_VIRTUAL_BUSSES,nS,10));  oappend(",");
    // oappend(itoa(MAX_LEDS_PER_BUS,nS,10)); oappend(",");
    // oappend(itoa(MAX_LED_MEMORY,nS,10));   oappend(",");
    // oappend(itoa(MAX_LEDS,nS,10));
    // oappend(SET_F(");"));

  //   sappend('c',SET_F("MS"),autoSegments);
  //   sappend('c',SET_F("CCT"),correctWB);
  //   sappend('c',SET_F("CR"),cctFromRgb);
  //   sappend('v',SET_F("CB"),cctBlending);
  //   sappend('v',SET_F("FR"),getTargetFps());
  //   sappend('v',SET_F("AW"),Bus::getGlobalAWMode());
  //   sappend('c',SET_F("LD"),useLedsArray);

  //   for (uint8_t s=0; s < busses.getNumBusses(); s++) {
  //     Bus* bus = busses.getBus(s);
  //     if (bus == nullptr) continue;
  //     char lp[4] = "L0"; lp[2] = 48+s; lp[3] = 0; //ascii 0-9 //strip data pin
  //     char lc[4] = "LC"; lc[2] = 48+s; lc[3] = 0; //strip length
  //     char co[4] = "CO"; co[2] = 48+s; co[3] = 0; //strip color order
  //     char lt[4] = "LT"; lt[2] = 48+s; lt[3] = 0; //strip type
  //     char ls[4] = "LS"; ls[2] = 48+s; ls[3] = 0; //strip start LED
  //     char cv[4] = "CV"; cv[2] = 48+s; cv[3] = 0; //strip reverse
  //     char sl[4] = "SL"; sl[2] = 48+s; sl[3] = 0; //skip 1st LED
  //     char rf[4] = "RF"; rf[2] = 48+s; rf[3] = 0; //off refresh
  //     char aw[4] = "AW"; aw[2] = 48+s; aw[3] = 0; //auto white mode
  //     char wo[4] = "WO"; wo[2] = 48+s; wo[3] = 0; //swap channels
  //     char sp[4] = "SP"; sp[2] = 48+s; sp[3] = 0; //bus clock speed
  //     oappend(SET_F("addLEDs(1);"));
  //     uint8_t pins[5];
  //     uint8_t nPins = bus->getPins(pins);
  //     for (uint8_t i = 0; i < nPins; i++) {
  //       lp[1] = 48+i;
  //       if (pinManager.isPinOk(pins[i]) || bus->getType()>=TYPE_NET_DDP_RGB) sappend('v',lp,pins[i]);
  //     }
  //     sappend('v',lc,bus->getLength());
  //     sappend('v',lt,bus->getType());
  //     sappend('v',co,bus->getColorOrder() & 0x0F);
  //     sappend('v',ls,bus->getStart());
  //     sappend('c',cv,bus->reversed);
  //     sappend('v',sl,bus->skippedLeds());
  //     sappend('c',rf,bus->isOffRefreshRequired());
  //     sappend('v',aw,bus->getAutoWhiteMode());
  //     sappend('v',wo,bus->getColorOrder() >> 4);
  //     uint16_t speed = bus->getFrequency();
  //     if (bus->getType() > TYPE_ONOFF && bus->getType() < 48) {
  //       switch (speed) {
  //         case WLED_PWM_FREQ/3 : speed = 0; break;
  //         case WLED_PWM_FREQ/2 : speed = 1; break;
  //         default:
  //         case WLED_PWM_FREQ   : speed = 2; break;
  //         case WLED_PWM_FREQ*2 : speed = 3; break;
  //         case WLED_PWM_FREQ*3 : speed = 4; break;
  //       }
  //     } else {
  //       switch (speed) {
  //         case  1000 : speed = 0; break;
  //         case  2000 : speed = 1; break;
  //         default:
  //         case  5000 : speed = 2; break;
  //         case 10000 : speed = 3; break;
  //         case 20000 : speed = 4; break;
  //       }
  //     }
  //     sappend('v',sp,speed);
  //   }
  //   sappend('v',SET_F("MA"),ablMilliampsMax);
  //   sappend('v',SET_F("LA"),milliampsPerLed);
  //   if (currentMilliamps)
  //   {
  //     sappends('m',SET_F("(\"pow\")[0]"),(char*)"");
  //     olen -= 2; //delete ";
  //     oappendi(currentMilliamps);
  //     oappend(SET_F("mA\";"));
  //   }

  //   oappend(SET_F("resetCOM("));
  //   oappend(itoa(WLED_MAX_COLOR_ORDER_MAPPINGS,nS,10));
  //   oappend(SET_F(");"));
  //   const ColorOrderMap& com = busses.getColorOrderMap();
  //   for (uint8_t s=0; s < com.count(); s++) {
  //     const ColorOrderMapEntry* entry = com.get(s);
  //     if (entry == nullptr) break;
  //     oappend(SET_F("addCOM("));
  //     oappend(itoa(entry->start,nS,10));  oappend(",");
  //     oappend(itoa(entry->len,nS,10));  oappend(",");
  //     oappend(itoa(entry->colorOrder,nS,10));  oappend(");");
  //   }

  //   sappend('v',SET_F("CA"),briS);

  //   sappend('c',SET_F("BO"),turnOnAtBoot);
  //   sappend('v',SET_F("BP"),bootPreset);

  //   sappend('c',SET_F("GB"),gammaCorrectBri);
  //   sappend('c',SET_F("GC"),gammaCorrectCol);
  //   dtostrf(gammaCorrectVal,3,1,nS); sappends('s',SET_F("GV"),nS);
  //   sappend('c',SET_F("TF"),fadeTransition);
  //   sappend('v',SET_F("TD"),transitionDelayDefault);
  //   sappend('c',SET_F("PF"),paletteFade);
  //   sappend('v',SET_F("TP"),randomPaletteChangeTime);
  //   sappend('v',SET_F("BF"),briMultiplier);
  //   sappend('v',SET_F("TB"),nightlightTargetBri);
  //   sappend('v',SET_F("TL"),nightlightDelayMinsDefault);
  //   sappend('v',SET_F("TW"),nightlightMode);
  //   sappend('i',SET_F("PB"),paletteBlend);
  //   sappend('v',SET_F("RL"),rlyPin);
  //   sappend('c',SET_F("RM"),rlyMde);
  //   for (uint8_t i=0; i<WLED_MAX_BUTTONS; i++) {
  //     oappend(SET_F("addBtn("));
  //     oappend(itoa(i,nS,10));  oappend(",");
  //     oappend(itoa(btnPin[i],nS,10)); oappend(",");
  //     oappend(itoa(buttonType[i],nS,10));
  //     oappend(SET_F(");"));
  //   }
  //   sappend('c',SET_F("IP"),disablePullUp);
  //   sappend('v',SET_F("TT"),touchThreshold);
  //   sappend('v',SET_F("IR"),irPin);
  //   sappend('v',SET_F("IT"),irEnabled);
  //   sappend('c',SET_F("MSO"),!irApplyToAllSelected);
  }

  if (subPage == SUBPAGE_UI)
  {
  //   sappends('s',SET_F("DS"),serverDescription);
  //   sappend('c',SET_F("ST"),syncToggleReceive);
  // #ifdef WLED_ENABLE_SIMPLE_UI
  //   sappend('c',SET_F("SU"),simplifiedUI);
  // #else
  //   oappend(SET_F("toggle('Simple');"));    // hide Simplified UI settings
  // #endif
  }

  if (subPage == SUBPAGE_SYNC)
  {
  //   sappend('v',SET_F("UP"),udpPort);
  //   sappend('v',SET_F("U2"),udpPort2);
  //   sappend('v',SET_F("GS"),syncGroups);
  //   sappend('v',SET_F("GR"),receiveGroups);

  //   sappend('c',SET_F("RB"),receiveNotificationBrightness);
  //   sappend('c',SET_F("RC"),receiveNotificationColor);
  //   sappend('c',SET_F("RX"),receiveNotificationEffects);
  //   sappend('c',SET_F("SO"),receiveSegmentOptions);
  //   sappend('c',SET_F("SG"),receiveSegmentBounds);
  //   sappend('c',SET_F("SD"),notifyDirectDefault);
  //   sappend('c',SET_F("SB"),notifyButton);
  //   sappend('c',SET_F("SH"),notifyHue);
  //   sappend('c',SET_F("SM"),notifyMacro);
  //   sappend('v',SET_F("UR"),udpNumRetries);

  //   sappend('c',SET_F("NL"),nodeListEnabled);
  //   sappend('c',SET_F("NB"),nodeBroadcastEnabled);

  //   sappend('c',SET_F("RD"),receiveDirect);
  //   sappend('c',SET_F("MO"),useMainSegmentOnly);
  //   sappend('v',SET_F("EP"),e131Port);
  //   sappend('c',SET_F("ES"),e131SkipOutOfSequence);
  //   sappend('c',SET_F("EM"),e131Multicast);
  //   sappend('v',SET_F("EU"),e131Universe);
  //   sappend('v',SET_F("DA"),DMXAddress);
  //   sappend('v',SET_F("XX"),DMXSegmentSpacing);
  //   sappend('v',SET_F("PY"),e131Priority);
  //   sappend('v',SET_F("DM"),DMXMode);
  //   sappend('v',SET_F("ET"),realtimeTimeoutMs);
  //   sappend('c',SET_F("FB"),arlsForceMaxBri);
  //   sappend('c',SET_F("RG"),arlsDisableGammaCorrection);
  //   sappend('v',SET_F("WO"),arlsOffset);
  //   sappend('c',SET_F("AL"),alexaEnabled);
  //   sappends('s',SET_F("AI"),alexaInvocationName);
  //   sappend('c',SET_F("SA"),notifyAlexa);
  //   sappend('v',SET_F("AP"),alexaNumPresets);
  //   #ifdef WLED_DISABLE_ALEXA
  //   oappend(SET_F("toggle('Alexa');"));  // hide Alexa settings
  //   #endif

  //   #ifdef WLED_ENABLE_MQTT
  //   sappend('c',SET_F("MQ"),mqttEnabled);
  //   sappends('s',SET_F("MS"),mqttServer);
  //   sappend('v',SET_F("MQPORT"),mqttPort);
  //   sappends('s',SET_F("MQUSER"),mqttUser);
  //   byte l = strlen(mqttPass);
  //   char fpass[l+1]; //fill password field with ***
  //   fpass[l] = 0;
  //   memset(fpass,'*',l);
  //   sappends('s',SET_F("MQPASS"),fpass);
  //   sappends('s',SET_F("MQCID"),mqttClientID);
  //   sappends('s',"MD",mqttDeviceTopic);
  //   sappends('s',SET_F("MG"),mqttGroupTopic);
  //   sappend('c',SET_F("BM"),buttonPublishMqtt);
  //   sappend('c',SET_F("RT"),retainMqttMsg);
  //   #else
  //   oappend(SET_F("toggle('MQTT');"));    // hide MQTT settings
  //   #endif

  //   #ifndef WLED_DISABLE_HUESYNC
  //   sappend('v',SET_F("H0"),hueIP[0]);
  //   sappend('v',SET_F("H1"),hueIP[1]);
  //   sappend('v',SET_F("H2"),hueIP[2]);
  //   sappend('v',SET_F("H3"),hueIP[3]);
  //   sappend('v',SET_F("HL"),huePollLightId);
  //   sappend('v',SET_F("HI"),huePollIntervalMs);
  //   sappend('c',SET_F("HP"),huePollingEnabled);
  //   sappend('c',SET_F("HO"),hueApplyOnOff);
  //   sappend('c',SET_F("HB"),hueApplyBri);
  //   sappend('c',SET_F("HC"),hueApplyColor);
  //   char hueErrorString[25];
  //   switch (hueError)
  //   {
  //     case HUE_ERROR_INACTIVE     : strcpy_P(hueErrorString,PSTR("Inactive"));                break;
  //     case HUE_ERROR_ACTIVE       : strcpy_P(hueErrorString,PSTR("Active"));                  break;
  //     case HUE_ERROR_UNAUTHORIZED : strcpy_P(hueErrorString,PSTR("Unauthorized"));            break;
  //     case HUE_ERROR_LIGHTID      : strcpy_P(hueErrorString,PSTR("Invalid light ID"));        break;
  //     case HUE_ERROR_PUSHLINK     : strcpy_P(hueErrorString,PSTR("Link button not pressed")); break;
  //     case HUE_ERROR_JSON_PARSING : strcpy_P(hueErrorString,PSTR("JSON parsing error"));      break;
  //     case HUE_ERROR_TIMEOUT      : strcpy_P(hueErrorString,PSTR("Timeout"));                 break;
  //     default: sprintf_P(hueErrorString,PSTR("Bridge Error %i"),hueError);
  //   }

  //   sappends('m',SET_F("(\"sip\")[0]"),hueErrorString);
  //   #else
  //   oappend(SET_F("toggle('Hue');"));    // hide Hue Sync settings
  //   #endif
  //   sappend('v',SET_F("BD"),serialBaud);
  }

  if (subPage == SUBPAGE_TIME)
  {
  //   sappend('c',SET_F("NT"),ntpEnabled);
  //   sappends('s',SET_F("NS"),ntpServerName);
  //   sappend('c',SET_F("CF"),!useAMPM);
  //   sappend('i',SET_F("TZ"),currentTimezone);
  //   sappend('v',SET_F("UO"),utcOffsetSecs);
  //   char tm[32];
  //   dtostrf(longitude,4,2,tm);
  //   sappends('s',SET_F("LN"),tm);
  //   dtostrf(latitude,4,2,tm);
  //   sappends('s',SET_F("LT"),tm);
  //   getTimeString(tm);
  //   sappends('m',SET_F("(\"times\")[0]"),tm);
  //   if ((int)(longitude*10.) || (int)(latitude*10.)) {
  //     sprintf_P(tm, PSTR("Sunrise: %02d:%02d Sunset: %02d:%02d"), hour(sunrise), minute(sunrise), hour(sunset), minute(sunset));
  //     sappends('m',SET_F("(\"times\")[1]"),tm);
  //   }
  //   sappend('c',SET_F("OL"),overlayCurrent);
  //   sappend('v',SET_F("O1"),overlayMin);
  //   sappend('v',SET_F("O2"),overlayMax);
  //   sappend('v',SET_F("OM"),analogClock12pixel);
  //   sappend('c',SET_F("OS"),analogClockSecondsTrail);
  //   sappend('c',SET_F("O5"),analogClock5MinuteMarks);

  //   sappend('c',SET_F("CE"),countdownMode);
  //   sappend('v',SET_F("CY"),countdownYear);
  //   sappend('v',SET_F("CI"),countdownMonth);
  //   sappend('v',SET_F("CD"),countdownDay);
  //   sappend('v',SET_F("CH"),countdownHour);
  //   sappend('v',SET_F("CM"),countdownMin);
  //   sappend('v',SET_F("CS"),countdownSec);

  //   sappend('v',SET_F("A0"),macroAlexaOn);
  //   sappend('v',SET_F("A1"),macroAlexaOff);
  //   sappend('v',SET_F("MC"),macroCountdown);
  //   sappend('v',SET_F("MN"),macroNl);
  //   for (uint8_t i=0; i<WLED_MAX_BUTTONS; i++) {
  //     oappend(SET_F("addRow("));
  //     oappend(itoa(i,tm,10));  oappend(",");
  //     oappend(itoa(macroButton[i],tm,10)); oappend(",");
  //     oappend(itoa(macroLongPress[i],tm,10)); oappend(",");
  //     oappend(itoa(macroDoublePress[i],tm,10));
  //     oappend(SET_F(");"));
  //   }

  //   char k[4];
  //   k[2] = 0; //Time macros
  //   for (int i = 0; i<10; i++)
  //   {
  //     k[1] = 48+i; //ascii 0,1,2,3
  //     if (i<8) { k[0] = 'H'; sappend('v',k,timerHours[i]); }
  //     k[0] = 'N'; sappend('v',k,timerMinutes[i]);
  //     k[0] = 'T'; sappend('v',k,timerMacro[i]);
  //     k[0] = 'W'; sappend('v',k,timerWeekday[i]);
  //     if (i<8) {
  //       k[0] = 'M'; sappend('v',k,(timerMonth[i] >> 4) & 0x0F);
	// 			k[0] = 'P'; sappend('v',k,timerMonth[i] & 0x0F);
  //       k[0] = 'D'; sappend('v',k,timerDay[i]);
	// 			k[0] = 'E'; sappend('v',k,timerDayEnd[i]);
  //     }
  //   }
  }

  if (subPage == SUBPAGE_SEC)
  {
  //   byte l = strlen(settingsPIN);
  //   char fpass[l+1]; //fill PIN field with 0000
  //   fpass[l] = 0;
  //   memset(fpass,'0',l);
  //   sappends('s',SET_F("PIN"),fpass);
  //   sappend('c',SET_F("NO"),otaLock);
  //   sappend('c',SET_F("OW"),wifiLock);
  //   sappend('c',SET_F("AO"),aOtaEnabled);
  //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("WLED "));
  //   olen -= 2; //delete ";
  //   oappend(versionString);
  //   oappend(SET_F(" (build "));
  //   oappendi(VERSION);
  //   oappend(SET_F(")\";"));
  //   oappend(SET_F("sd=\""));
  //   oappend(serverDescription);
  //   oappend(SET_F("\";"));
  // }

  // #ifdef ENABLE_FEATURE_LIGHTING__DMX // include only if DMX is enabled
  // if (subPage == SUBPAGE_DMX)
  // {
  //   sappend('v',SET_F("PU"),e131ProxyUniverse);

  //   sappend('v',SET_F("CN"),DMXChannels);
  //   sappend('v',SET_F("CG"),DMXGap);
  //   sappend('v',SET_F("CS"),DMXStart);
  //   sappend('v',SET_F("SL"),DMXStartLED);

  //   sappend('i',SET_F("CH1"),DMXFixtureMap[0]);
  //   sappend('i',SET_F("CH2"),DMXFixtureMap[1]);
  //   sappend('i',SET_F("CH3"),DMXFixtureMap[2]);
  //   sappend('i',SET_F("CH4"),DMXFixtureMap[3]);
  //   sappend('i',SET_F("CH5"),DMXFixtureMap[4]);
  //   sappend('i',SET_F("CH6"),DMXFixtureMap[5]);
  //   sappend('i',SET_F("CH7"),DMXFixtureMap[6]);
  //   sappend('i',SET_F("CH8"),DMXFixtureMap[7]);
  //   sappend('i',SET_F("CH9"),DMXFixtureMap[8]);
  //   sappend('i',SET_F("CH10"),DMXFixtureMap[9]);
  //   sappend('i',SET_F("CH11"),DMXFixtureMap[10]);
  //   sappend('i',SET_F("CH12"),DMXFixtureMap[11]);
  //   sappend('i',SET_F("CH13"),DMXFixtureMap[12]);
  //   sappend('i',SET_F("CH14"),DMXFixtureMap[13]);
  //   sappend('i',SET_F("CH15"),DMXFixtureMap[14]);
  }
  // #endif

  if (subPage == SUBPAGE_UM) //usermods
  {
  //   appendGPIOinfo();
  //   oappend(SET_F("numM="));
  //   oappendi(usermods.getModCount());
  //   oappend(";");
  //   sappend('v',SET_F("SDA"),i2c_sda);
  //   sappend('v',SET_F("SCL"),i2c_scl);
  //   sappend('v',SET_F("MOSI"),spi_mosi);
  //   sappend('v',SET_F("MISO"),spi_miso);
  //   sappend('v',SET_F("SCLK"),spi_sclk);
  //   oappend(SET_F("addInfo('SDA','"));  oappendi(HW_PIN_SDA);      oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('SCL','"));  oappendi(HW_PIN_SCL);      oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('MOSI','")); oappendi(HW_PIN_DATASPI);  oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('MISO','")); oappendi(HW_PIN_MISOSPI);  oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('SCLK','")); oappendi(HW_PIN_CLOCKSPI); oappend(SET_F("');"));
  //   usermods.appendConfigData();
  // }

  // if (subPage == SUBPAGE_UPDATE) // update
  // {
  //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("WLED "));
  //   olen -= 2; //delete ";
  //   oappend(versionString);
  //   oappend(SET_F("<br>("));
  //   #if defined(ARDUINO_ARCH_ESP32)
  //   oappend(ESP.getChipModel());
  //   #else
  //   oappend("esp8266");
  //   #endif
  //   oappend(SET_F(" build "));
  //   oappendi(VERSION);
  //   oappend(SET_F(")\";"));
  // }

  // if (subPage == SUBPAGE_2D) // 2D matrices
  // {
  //   sappend('v',SET_F("SOMP"),isMatrix);
  //   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  //   oappend(SET_F("maxPanels=")); oappendi(WLED_MAX_PANELS); oappend(SET_F(";"));
  //   oappend(SET_F("resetPanels();"));
  //   if (isMatrix) {
  //     if(panels>0){
  //       sappend('v',SET_F("PW"),panel[0].width); //Set generator Width and Height to first panel size for convenience
  //       sappend('v',SET_F("PH"),panel[0].height);
  //     }
  //     sappend('v',SET_F("MPC"),panels);
  //     // panels
  //     for (uint8_t i=0; i<panels; i++) {
  //       char n[5];
  //       oappend(SET_F("addPanel("));
  //       oappend(itoa(i,n,10));
  //       oappend(SET_F(");"));
  //       char pO[8] = { '\0' };
  //       snprintf_P(pO, 7, PSTR("P%d"), i);       // MAX_PANELS is 64 so pO will always only be 4 characters or less
  //       pO[7] = '\0';
  //       uint8_t l = strlen(pO);
  //       // create P0B, P1B, ..., P63B, etc for other PxxX
  //       pO[l] = 'B'; sappend('v',pO,panel[i].bottomStart);
  //       pO[l] = 'R'; sappend('v',pO,panel[i].rightStart);
  //       pO[l] = 'V'; sappend('v',pO,panel[i].vertical);
  //       pO[l] = 'S'; sappend('c',pO,panel[i].serpentine);
  //       pO[l] = 'X'; sappend('v',pO,panel[i].xOffset);
  //       pO[l] = 'Y'; sappend('v',pO,panel[i].yOffset);
  //       pO[l] = 'W'; sappend('v',pO,panel[i].width);
  //       pO[l] = 'H'; sappend('v',pO,panel[i].height);
  //     }
  //   }
  //   #else
  //   oappend(SET_F("gId(\"somp\").remove(1);")); // remove 2D option from dropdown
  //   #endif
  }
}

#endif // ENABLE_FEATURE_LIGHTING__XML_REQUESTS





void mAnimatorLight::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
  // if (!correctPIN) {
  //   if (final) request->send(500, "text/plain", FPSTR(s_unlock_cfg));
  //   return;
  // }
  if (!index) {
    String finalname = filename;
    if (finalname.charAt(0) != '/') {
      finalname = '/' + finalname; // prepend slash if missing
    }

    request->_tempFile = FILE_SYSTEM.open(finalname, "w");
    DEBUG_PRINT(F("Uploading "));
    DEBUG_PRINTLN(finalname);
    if (finalname.equals("/presets.json"))
    { 
      // presetsModifiedTime = toki.second();
    }
  }
  if (len) {
    request->_tempFile.write(data,len);
  }
  if (final) {
    request->_tempFile.close();
    if (filename.indexOf(F("cfg.json")) >= 0) { // check for filename with or without slash
      doReboot = true;
      request->send(200, "text/plain", F("Configuration restore successful.\nRebooting..."));
    } else {
      if (filename.indexOf(F("palette")) >= 0 && filename.indexOf(F(".json")) >= 0)
      {
        // loadCustomPalettes();
      }
      request->send(200, "text/plain", F("File Uploaded!"));
    }
    tkr_web->cacheInvalidate++;
  }
}



void mAnimatorLight::setPaletteColors(JsonArray json, CRGBPalette16 palette)
{
    for (int i = 0; i < 16; i++) {
      JsonArray colors =  json.createNestedArray();
      CRGB color = palette[i];
      colors.add(i<<4);
      colors.add(color.red);
      colors.add(color.green);
      colors.add(color.blue);
    }
}


void mAnimatorLight::serializePalettes(JsonObject root, int page)
{
  byte tcp[72];
  #ifdef ESP8266
  int itemPerPage = 5;
  #else
  int itemPerPage = 8;
  #endif

  DEBUG_LINE_HERE_TRACE

  bool flag_request_is_for_full_visual_output = true;

  int palettesCount = mPaletteI->GetPaletteListLength(); //includes the dynamic!
  int customPalettes = tkr_anim->customPalettes.size();

  // ALOG_HGL(PSTR("palettesCount=%d"), palettesCount); 

  int maxPage = (palettesCount + customPalettes -1) / itemPerPage;
  if (page > maxPage) page = maxPage;

  int start = itemPerPage * page;
  int end = start + itemPerPage;
  if (end > palettesCount + customPalettes) end = palettesCount + customPalettes;

  root[F("m")] = maxPage; // inform caller how many pages there are
  JsonObject palettes  = root.createNestedObject("p");
  JsonObject palettes_style  = root.createNestedObject("s");

  uint8_t encoded_gradient = 0;

  DEBUG_LINE_HERE_TRACE
  /**
   * @brief 
   * Start by sending the current palette loaded
   */
  for (int palette_id = start; palette_id < end; palette_id++) 
  {
       
  DEBUG_LINE_HERE_TRACE 
    bool palette_display_as_banded_gradient = false;

    #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
    ALOG_INF(PSTR("i=%d|p%d|m%d"),palette_id,page,maxPage);
    ALOG_INF(PSTR("p=%d|s%d|e%d"),palette_id,start,end);
    #endif

  DEBUG_LINE_HERE_TRACE

  #ifndef ENABLE_DEVFEATURE_LIGHTING__CRITICAL_DISABLE_LOAD_PALETTE  
  SEGMENT.LoadPalette(palette_id); // Assume segment 1 exists, and use it to load all palettes. Effect should reset to active palette in main loop. Or here, have it then flip back. Though this may cause flickering midanimation. Animation may also need paused on esp32.
  #endif

  DEBUG_LINE_HERE_TRACE
    uint16_t colours_in_palette = GetNumberOfColoursInPalette(palette_id);
   
  DEBUG_LINE_HERE_TRACE
    #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
    ALOG_INF(PSTR("colours_in_palette[%d]=%d"),palette_id, colours_in_palette);
    #endif

    JsonArray curPalette_obj = palettes.createNestedArray(String(palette_id));
    JsonObject curPalette_s_obj = palettes_style.createNestedObject(String(palette_id));

  DEBUG_LINE_HERE_TRACE
    /**
     * @brief To reduce memory usage, the static gradients that are stored with less than 16 colours, shall be read directly
     **/
    if(
      (palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && 
      (palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID)
    ){ 

  DEBUG_LINE_HERE_TRACE
      uint8_t adjusted_id = palette_id - mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;

      byte tcp[72];
      memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[adjusted_id])), 72);

      TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
      TRGBGradientPaletteEntryUnion u;

      // Count entries
      uint16_t count = 0;
      do {
        u = *(ent + count);
        count++;
      } while ( u.index != 255);

      #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
      ALOG_DBM(PSTR("palette_id%d,count=%d"),palette_id,count);
      #endif

      u = *ent;
      int indexstart = 0;
      while( indexstart < 255) {
        indexstart = u.index;

        JsonArray colors =  curPalette_obj.createNestedArray();
        colors.add(u.index);
        colors.add(u.r);
        colors.add(u.g);
        colors.add(u.b);

        ent++;
        u = *ent;
      }
      
      palette_display_as_banded_gradient = false;

    }
    else
    {
    
  DEBUG_LINE_HERE_TRACE
      palette_display_as_banded_gradient = false;

      encoded_gradient = 0;
      
      RgbwwColor color;


      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif

  DEBUG_LINE_HERE_TRACE
      /** first check if the palette is one that uses the colour picker*/       
      // Handle RGBCCT color palettes
      if (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID && palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID) {
          
  DEBUG_LINE_HERE_TRACE
  const char* color_id = nullptr;
          switch (palette_id) {
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID: color_id = "c1"; break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_02__ID: color_id = "c2"; break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_03__ID: color_id = "c3"; break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_04__ID: color_id = "c4"; break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_05__ID: color_id = "c5"; break;
          }
          if (color_id) {
              curPalette_obj.add(color_id);
          }
      } 
      // Handle CRGBPalette16 paired palettes
      else if (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID && palette_id <= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_REPEATED_ACTIVE__ID) {
          const char* color_ids[] = {"c1", "c2", "c3", "c4", "c5"};
          int color_count = 0;

  DEBUG_LINE_HERE_TRACE
          switch (palette_id) {
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID:
                  color_count = 2;
                  break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_THREE_123__ID:
                  color_count = 3;
                  break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FOUR_1234__ID:
                  color_count = 4;
                  break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FIVE_12345__ID:
                  color_count = 5;
                  break;
              case mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_REPEATED_ACTIVE__ID:
                  color_count = 5;
                  for (int repeat = 0; repeat < 3; ++repeat) { // Repeat active colors three times
                      for (int i = 0; i < color_count; ++i) {
                          curPalette_obj.add(color_ids[i]);
                      }
                  }
                  curPalette_obj.add("c5"); // Add one more "c5" as required
                  break;
          }

          // Add the color IDs according to the count
          for (int i = 0; i < color_count; ++i) {
              curPalette_obj.add(color_ids[i]);
              if (palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) {
                  curPalette_obj.add(color_ids[i]); // Add the same color twice for the "two" palette
              }
          }
      } 
      // Handle random hue palettes
      else if (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID && palette_id <= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID) {
          // Add "r" for random color-based palettes
          for (int i = 0; i < 4; ++i) {
              curPalette_obj.add("r");
          }
          
  DEBUG_LINE_HERE_TRACE
      }

      /**
       * @brief Palettes that the RGB data should be retrived
       * 
       */
      else
      {
  DEBUG_LINE_HERE_TRACE

        #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
        ALOG_DBM(PSTR(DEBUG_INSERT_PAGE_BREAK "palette_id=%d"),palette_id);
        #endif

        // /**
        //  * @brief If the live palettes, just for the webui to get a preview, the colours in palette are expanded to 16
        //  **/
        // if(
        //   (palette_id >= mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < mPalette::PALETTELIST_DYNAMIC__LENGTH__ID)
        // ){  
        //   flag_request_is_for_full_visual_output = true;
        //   colours_in_palette = 16; // for preview
        // }



        for (int j = 0; j < colours_in_palette; j++) 
        {
            JsonArray colors = curPalette_obj.createNestedArray();

            DEBUG_LINE_HERE_TRACE;

  DEBUG_LINE_HERE_TRACE
            // Load temporary palette
            color = GetColourFromUnloadedPalette3(
                palette_id,
                j,
                PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, // "PALETTE_DISCRETE_ON" should be the only thing to get the basic colors, without gradients
                &encoded_gradient,
                flag_request_is_for_full_visual_output
            );

  DEBUG_LINE_HERE_TRACE
            #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
            Serial.print("++++++++++++++++++++++++++++++++++++++++++++++encoded_gradient: ");
            Serial.println(encoded_gradient);
            Serial.flush();
            #endif

            // Handle the encoded gradient
            if (encoded_gradient > 0) 
            {
              // Gradient exists, use the encoded value directly
              colors.add(encoded_gradient); 
            } 
            else 
            {
              // If encoded gradient is 0, map j to create a smooth gradient
              if (colours_in_palette > 1) 
              {
                /*
                Issue with Scaling Using colours_in_palette:
                  If you scale directly with colours_in_palette (i.e., mapping 0 to 255 across the range 0 to colours_in_palette), you end up with the following issue:
                    You divide the range 0-255 into colours_in_palette equal segments.
                    However, the last color in the palette, when mapped, gets placed beyond 255 (or at the last position), meaning it effectively doesn’t get visualized properly, and the gradient appears to be missing the final color.
                Correct Scaling Using colours_in_palette - 1:
                  When you use colours_in_palette - 1:
                    You ensure that the entire range of colors, including the first and last, is properly displayed.
                    The colors will be evenly distributed from 0 to 255, and the last color will correctly align with 255.*/
                colors.add(map(j, 0, colours_in_palette - 1, 0, 255));
              } 
              else 
              {
                // If there's only one color, just add a default index
                colors.add(0);  // Add a base index for a single-color palette
              }
            }

            // Add the RGB color components
            colors.add(color.R);
            colors.add(color.G);
            colors.add(color.B);

            #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
            ALOG_DBM(PSTR("j=%d,encoded_gradient=%d,rgb=%d,%d,%d"), j, encoded_gradient, color.red, color.green, color.blue);
            #endif
        }


      }

    } //end CSL colours

            
    if (palette_id >= mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID && palette_id < mPalette::PALETTELIST_STATIC_LENGTH__ID) {  
      palette_display_as_banded_gradient = true; // Assume banded by default

      uint8_t adjusted_id = palette_id - mPalette::PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
      mPalette::PALETTE_DATA* ptr = &mPaletteI->static_palettes[adjusted_id];

      if (ptr->encoding.index_gradient) {
          palette_display_as_banded_gradient = false; // Use gradient if index_gradient is set
      }
    }

    if (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID && palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID) {  
      palette_display_as_banded_gradient = false; // No gradient for segment palettes
    }

    if (
      (palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID && palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID) ||
      (palette_id >= mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID && palette_id < mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID) ||
      (palette_id >= mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID && palette_id < mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID)
    ) {  
        palette_display_as_banded_gradient = false; // These palettes use gradients
    }

    /***
     * Custom palettes
     */
    if (palette_id >= mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED && palette_id < mPaletteI->GetPaletteListLength()) {
      uint8_t adjusted_id = palette_id - mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;
      palette_display_as_banded_gradient = true; // Assume banded for custom palettes

      if (mPaletteI->custom_palettes[adjusted_id].encoding.index_gradient) {
          palette_display_as_banded_gradient = false; // Use gradient if index_gradient is set
      }
    }

    // Set the background type based on gradient detection
    curPalette_s_obj["bg"] = palette_display_as_banded_gradient ? "B" : "L";

        
  }

}


void mAnimatorLight::serializeNetworks(JsonObject root)
{
  JsonArray networks = root.createNestedArray(F("networks"));
  int16_t status = WiFi.scanComplete();

  switch (status) {
    case WIFI_SCAN_FAILED:
      WiFi.scanNetworks(true);
      return;
    case WIFI_SCAN_RUNNING:
      return;
  }

  for (int i = 0; i < status; i++) {
    JsonObject node = networks.createNestedObject();
    node["ssid"]    = WiFi.SSID(i);
    node["rssi"]    = WiFi.RSSI(i);
    node["bssid"]   = WiFi.BSSIDstr(i);
    node["channel"] = WiFi.channel(i);
    node["enc"]     = WiFi.encryptionType(i);
  }

  WiFi.scanDelete();

  if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
    WiFi.scanNetworks(true);
  }
}

// deserializes mode data string into JsonArray
void mAnimatorLight::serializeModeData(JsonArray fxdata)
{
  char lineBuffer[256];
  for (size_t i = 0; i < getModeCount(); i++) {
    strncpy_P(lineBuffer, getModeData_Config(i), sizeof(lineBuffer) - 1);
    lineBuffer[sizeof(lineBuffer) - 1] = '\0'; // terminate string

    ALOG_DBM(PSTR("lineBuffer %d %s"), i, lineBuffer);

    if (lineBuffer[0] != 0) {
      char* dataPtr = strchr(lineBuffer, '@');
      if (dataPtr) {
#ifdef ENABLE_DEVFEATURE_LIGHTING__ADD_EFFECT_DEVSTAGE_TO_WEBUI
        // Append development stage directly to the string if within bounds
        if (i < effects.development_stage.size()) {
          uint8_t devStage = effects.development_stage[i];
          snprintf(lineBuffer + strlen(lineBuffer), sizeof(lineBuffer) - strlen(lineBuffer), "~%u", devStage);
        }
#endif
        fxdata.add(dataPtr + 1); // Add data after '@' to the JsonArray
      } else {
        fxdata.add("");
      }
    }
  }
}



// deserializes mode names string into JsonArray also removes effect data extensions (@...) from deserialised names
void mAnimatorLight::serializeModeNames(JsonArray arr, bool flag_get_first_name_only) 
{

  char lineBuffer[128] = {0};
  for(uint16_t i = 0; i < getEffectsAmount(); i++)
  {
    GetFlasherFunctionNamebyID(i, lineBuffer, sizeof(lineBuffer), true);
    #ifdef ENABLE_DEVFEATURE_LIGHTING__ADD_DEVSTAGE_TO_EFFECT_NAME
    switch(effects.development_stage[i])
    {
      default:
      case 0: break; // leave when stable
      case 1: strcat_P(lineBuffer, PSTR(" (alpha)")); break;
      case 2: strcat_P(lineBuffer, PSTR(" (beta)")); break;
      case 3: strcat_P(lineBuffer, PSTR(" (dev)")); break; 
      case 4: strcat_P(lineBuffer, PSTR(" (uns)")); break; 
    }
    #endif // ENABLE_DEVFEATURE_LIGHTING__ADD_DEVSTAGE_TO_EFFECT_NAME
    // ALOG_INF(PSTR("serializeModeNames2 %d %s"), getEffectsAmount(), lineBuffer);
    arr.add(lineBuffer);
  }

}


// New ifdef to test out websockets
#ifdef ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS


#define MAX_LIVE_LEDS 256

bool mAnimatorLight::serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient)
{
  #ifdef WLED_ENABLE_WEBSOCKETS2
  AsyncWebSocketClient * wsc = nullptr;
  if (!request) { //not HTTP, use Websockets
    wsc = tkr_web->ws->client(wsClient);
    if (!wsc || wsc->queueLength() > 0) return false; //only send if queue free
  }
  #endif

  unsigned used = getLengthTotal();
  unsigned n = (used -1) /MAX_LIVE_LEDS +1; //only serve every n'th LED if count over MAX_LIVE_LEDS
#ifndef WLED_DISABLE_2D
  if (isMatrix) {
    // ignore anything behid matrix (i.e. extra strip)
    used = Segment::maxWidth*Segment::maxHeight; // always the size of matrix (more or less than strip.getLengthTotal())
    n = 1;
    if (used > MAX_LIVE_LEDS) n = 2;
    if (used > MAX_LIVE_LEDS*4) n = 4;
  }
#endif

  DynamicBuffer buffer(9 + (9*(1+(used/n))) + 7 + 5 + 6 + 5 + 6 + 5 + 2);  
  char* buf = buffer.data();      // assign buffer for oappnd() functions
  strncpy_P(buffer.data(), PSTR("{\"leds\":["), buffer.size());
  buf += 9; // sizeof(PSTR()) from last line

  for (size_t i = 0; i < used; i += n)
  {
#ifndef WLED_DISABLE_2D
    if (isMatrix && n>1 && (i/Segment::maxWidth)%n) i += Segment::maxWidth * (n-1);
#endif
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
    RgbwwColor col = getPixelColor(i);
    uint32_t c = RGBW32(col.R, col.G, col.B, col.WW);
    #else
    uint32_t c = getPixelColor(i);
    #endif
    uint8_t r = R(c);
    uint8_t g = G(c);
    uint8_t b = B(c);
    uint8_t w = W(c);
    r = scale8(qadd8(w, r), getBrightness()); //R, add white channel to RGB channels as a simple RGBW -> RGB map
    g = scale8(qadd8(w, g), getBrightness()); //G
    b = scale8(qadd8(w, b), getBrightness()); //B
    buf += sprintf_P(buf, PSTR("\"%06X\","), RGBW32(r,g,b,0));
  }
  buf--;  // remove last comma
  buf += sprintf_P(buf, PSTR("],\"n\":%d"), n);
#ifndef WLED_DISABLE_2D
  if (isMatrix) {
    buf += sprintf_P(buf, PSTR(",\"w\":%d"), Segment::maxWidth/n);
    buf += sprintf_P(buf, PSTR(",\"h\":%d"), Segment::maxHeight/n);
  }
#endif
  (*buf++) = '}';
  (*buf++) = 0;
  
  if (request) {
    request->send(200, "application/json", toString(std::move(buffer)));
  }
  #ifdef WLED_ENABLE_WEBSOCKETS2
  else {
    wsc->text(toString(std::move(buffer)));
  }
  #endif  
  return true;
}


#else




#define MAX_LIVE_LEDS 180

bool mAnimatorLight::serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient)
{
  #ifdef WLED_ENABLE_WEBSOCKETS
  AsyncWebSocketClient * wsc = nullptr;
  if (!request) { //not HTTP, use Websockets
    wsc = ws.client(wsClient);
    if (!wsc || wsc->queueLength() > 0) return false; //only send if queue free
  }
  #endif

  uint16_t used = getLengthTotal();
  uint16_t n = (used-1) /MAX_LIVE_LEDS +1; //only serve every n'th LED if count over MAX_LIVE_LEDS
  char buffer[2000];
  strcpy_P(buffer, PSTR("{\"leds\":["));
  obuf = buffer;
  olen = 9;

  for (size_t i=0; i < used; i += n)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE  
    RgbwwColor col = getPixelColor(i);
    col.WW = 0; //ignore white channel
    uint32_t c = RGBW32(col.R, col.G, col.B, col.WW);
    #else
    uint32_t c = getPixelColor(i);
    #endif

    // if(i==0)
      // ALOG_INF(PSTR("%d c %d,%d,%d,%d"), i, R(c), G(c), B(c), W(c));

    #ifdef ENABLE_FEATURE_LIGHTS__ADD_WHITE_TO_LIVEVIEW
    uint8_t r = qadd8(W(c), R(c)); //add white channel to RGB channels as a simple RGBW -> RGB map
    uint8_t g = qadd8(W(c), G(c));
    uint8_t b = qadd8(W(c), B(c));
    #else
    uint8_t r = R(c); //add white channel to RGB channels as a simple RGBW -> RGB map
    uint8_t g = G(c);
    uint8_t b = B(c);
    #endif

    olen += sprintf(obuf + olen, "\"%06X\",", RGBW32(r,g,b,0));
    
  }
  olen -= 1;
  oappend((const char*)F("],\"n\":"));
  oappendi(n);
  oappend("}");
  if (request) {
    request->send(200, "application/json", buffer);
  }
  #ifdef WLED_ENABLE_WEBSOCKETS
  else {
    wsc->text(obuf, olen);
  }
  #endif
  return true;
}


#endif


void mAnimatorLight::serveJson(AsyncWebServerRequest* request)
{

  byte subJson = 0;
  const String& url = request->url();
  if      (url.indexOf("state") > 0) subJson = JSON_PATH_STATE;
  else if (url.indexOf("info")  > 0) subJson = JSON_PATH_INFO;
  else if (url.indexOf("si")    > 0) subJson = JSON_PATH_STATE_INFO;
  else if (url.indexOf("nodes") > 0) subJson = JSON_PATH_NODES;
  else if (url.indexOf("eff")   > 0) subJson = JSON_PATH_EFFECTS;
  else if (url.indexOf("palx")  > 0) subJson = JSON_PATH_PALETTES;
  else if (url.indexOf("fxda")  > 0) subJson = JSON_PATH_FXDATA;
  else if (url.indexOf("net")   > 0) subJson = JSON_PATH_NETWORKS;
  #ifdef WLED_ENABLE_JSONLIVE
  else if (url.indexOf("live")  > 0) { 
    serveLiveLeds(request);
    return;
  }
  #endif
  else if (url.indexOf("pal") > 0) { // "/json/palettes"

    JBI->Start();
      JBI->Array_Start();

      char lineBuffer[100] = {0};
      bool flag_get_first_name_only = true;
        
      for(uint16_t i = 0; i < mPaletteI->GetPaletteListLength(); i++)
      {

        GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
        if(flag_get_first_name_only)
        {    
          char* dataPtr = strchr(lineBuffer, PALETTE_MULTIPLE_NAME_DELIMETER);
          if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
        }
        ALOG_DBM(PSTR("pal[%d]=\"%s\""), i, lineBuffer);
        JBI->Add(lineBuffer);
      }

      JBI->Array_End();
    JBI->End();

    // remove leading and trailing json parts as temp measure
    char* data = JBI->GetBufferPtr();
    uint16_t data_len = strlen(data);
    if(data) data[data_len-1] = '\0';
    Serial.println();

    #ifdef ENABLE_DEVFEATURE_WEBSERVER__ETAGS_ENABLED_FOR_RELOADING_PALETTES_ON_FRESH_COMPILE    
    /**
     * @brief It actually makes perfect sense to embedded the ETag into the names of palettes, 
     * since its this that forces the reload of the palette colours if needed too.
     * 
     */
    // Generate the ETag
    char etag[32];
    tkr_web->generateEtag(etag, JSON_PATH_PALETTES);
    // Send the response with the ETag header
    AsyncWebServerResponse *response = request->beginResponse_P(200, "application/json", &data[1]);
    response->addHeader(F("ETag"), etag); // Add the ETag header to the response
    request->send(response);
    #else
    request->send_P(200, "application/json", &data[1]);
    #endif

    return;
  }
  else if (url.indexOf("cfg") > 0 && pCONT_mfile->handleFileRead(request, "/cfg.json")) {
    return;
  }
  else if (url.length() > 6) { //not just /json
    request->send(501, "application/json", F("{\"error\":\"Not implemented\"}"));
    return;
  }

  if (!requestJSONBufferLock(17)) {
    request->send(503, "application/json", F("{\"error\":3}"));
    return;
  }

  AsyncJsonResponse *response = new AsyncJsonResponse(&doc, subJson==JSON_PATH_FXDATA || subJson==JSON_PATH_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary

  JsonVariant lDoc = response->getRoot();

  switch (subJson)
  {
    case JSON_PATH_STATE:
      serializeState(lDoc); 
    break;
    case JSON_PATH_INFO:
      serializeInfo(lDoc);     
    break;
    case JSON_PATH_PALETTES:
      Serial.println("JSON_PATH_PALETTES"); Serial.flush();
      serializePalettes(lDoc, request->hasParam("page") ? request->getParam("page")->value().toInt() : 0); 
    break;
    case JSON_PATH_EFFECTS:
      serializeModeNames(lDoc); 
    break;
    case JSON_PATH_FXDATA:
      serializeModeData(lDoc); 
    break;
    case JSON_PATH_NETWORKS:
      serializeNetworks(lDoc); 
    break;
    default: // All
      JsonObject state = lDoc.createNestedObject("state");
      serializeState(state);
      JsonObject info = lDoc.createNestedObject("info");
      serializeInfo(info);

      tkr_anim->force_update(); // New data in, so we should update

      if (subJson != JSON_PATH_STATE_INFO)
      {
        JsonArray effects = lDoc.createNestedArray(F("effects"));
        serializeModeNames(effects);

        bool flag_get_first_name_only = true;        
        char lineBuffer[100] = {0};
        JsonArray pal = lDoc.createNestedArray(F("palettes"));
        for(uint16_t i = 0; i < mPaletteI->GetPaletteListLength(); i++)
        {
          GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
          if(flag_get_first_name_only)
          {    
            char* dataPtr = strchr(lineBuffer,'|');
            if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
            // Serial.println(lineBuffer);
          }
          pal.add(lineBuffer);
        }

      }
      lDoc["m"] = lDoc.memoryUsage(); // JSON buffer usage, for remote debugging
  }

  ALOG_DBG(PSTR("JSON buffer size: %u for request: %d\n"), lDoc.memoryUsage(), subJson);

  size_t len = response->setLength();
  ALOG_DBG(PSTR("JSON content length: %d"), len);

  request->send(response);
  releaseJSONBufferLock();
}


void mAnimatorLight::serveSettingsJS(AsyncWebServerRequest* request)
{
  static const char _common_js[] PROGMEM = "/common.js";
  if (request->url().indexOf(FPSTR(_common_js)) > 0) {
    tkr_web->handleStaticContent(request, FPSTR(_common_js), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_common, JS_common_length);
    return;
  }
  
  byte subPage = request->arg(F("p")).toInt();
  if (subPage > 10) {
    request->send_P(501, FPSTR(CONTENT_TYPE_JAVASCRIPT), PSTR("alert('Settings for this request are not implemented.');"));
    return;
  }
  // if (subPage > 0 && !correctPIN && strlen(settingsPIN)>0) {
  //   request->send_P(401, FPSTR(CONTENT_TYPE_JAVASCRIPT), PSTR("alert('PIN incorrect.');"));
  //   return;
  // }
  
  #ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  AsyncResponseStream *response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JAVASCRIPT));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  response->print(F("function GetV(){var d=document;"));
  getSettingsJS(subPage, *response);
  response->print(F("}"));
  request->send(response);
  #else // old way to be replaced

  char buf[SETTINGS_STACK_BUF_SIZE+37];
  buf[0] = 0;
  subPage = request->arg(F("p")).toInt();
  if (subPage > 10) {
    strcpy_P(buf, PSTR("alert('Settings for this request are not implemented.');"));
    request->send(501, "application/javascript", buf);
    return;
  }
  if (subPage > 0){//} && !correctPIN && strlen(settingsPIN)>0) {
    strcpy_P(buf, PSTR("alert('PIN incorrect.');")); 
    request->send(403, "application/javascript", buf);
    return;
  }
  strcat_P(buf,PSTR("function GetV(){var d=document;"));
  getSettingsJS(subPage, buf+strlen(buf));  // this may overflow by 35bytes!!!
  strcat_P(buf,PSTR("}"));
  request->send(200, "application/javascript", buf);
  #endif
}


void mAnimatorLight::serveSettings(AsyncWebServerRequest* request, bool post)
{
  byte subPage = 0, originalSubPage = 0;
  const String& url = request->url();

  if (url.indexOf("sett") >= 0)
  {
    if      (url.indexOf(".js")  > 0) subPage = SUBPAGE_JS;
    else if (url.indexOf(".css") > 0) subPage = SUBPAGE_CSS;
    else if (url.indexOf("wifi") > 0) subPage = SUBPAGE_WIFI;
    else if (url.indexOf("leds") > 0) subPage = SUBPAGE_LEDS;
    else if (url.indexOf("ui")   > 0) subPage = SUBPAGE_UI;
    else if (url.indexOf("sync") > 0) subPage = SUBPAGE_SYNC;
    else if (url.indexOf("time") > 0) subPage = SUBPAGE_TIME;
    else if (url.indexOf("sec")  > 0) subPage = SUBPAGE_SEC;
    #ifdef ENABLE_FEATURE_LIGHTING__DMX
    else if (url.indexOf("dmx")  > 0) subPage = SUBPAGE_DMX;
    #endif
    else if (url.indexOf("um")   > 0) subPage = SUBPAGE_UM;
    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    else if (url.indexOf("2D")   > 0) subPage = SUBPAGE_2D;
    #endif
    else if (url.indexOf("lock") > 0) subPage = SUBPAGE_LOCK;
  }
  else if (url.indexOf("/update") >= 0) subPage = SUBPAGE_UPDATE; // update page, for PIN check
  else subPage = SUBPAGE_WELCOME;

  if(
  #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
  !correctPIN && strlen(settingsPIN) > 0 && 
  #endif
  (subPage > 0 && subPage < 11))
  {
    originalSubPage = subPage;
    #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
    subPage = SUBPAGE_PINREQ; // require PIN
    #endif
  }

  #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
  // if OTA locked or too frequent PIN entry requests fail hard
  if ((subPage == SUBPAGE_WIFI && wifiLock && otaLock) || (post && !correctPIN && millis()-lastEditTime < PIN_RETRY_COOLDOWN))
  {
    serveMessage(request, 401, FPSTR(s_accessdenied), FPSTR(s_unlock_ota), 254); return;
  }
  #endif

  if (post) { //settings/set POST request, saving

    // if (subPage != SUBPAGE_WIFI || !(wifiLock && otaLock)) handleSettingsSet(request, subPage);   // I should have this?

    char s[32];
    char s2[45] = "";

    switch (subPage) {
      case SUBPAGE_WIFI   : strcpy_P(s, PSTR("WiFi")); strcpy_P(s2, PSTR("Please connect to the new IP (if changed)")); break;
      case SUBPAGE_LEDS   : strcpy_P(s, PSTR("LED")); break;
      case SUBPAGE_UI     : strcpy_P(s, PSTR("UI")); break;
      case SUBPAGE_SYNC   : strcpy_P(s, PSTR("Sync")); break;
      case SUBPAGE_TIME   : strcpy_P(s, PSTR("Time")); break;
      case SUBPAGE_SEC    : strcpy_P(s, PSTR("Security")); if (doReboot) strcpy_P(s2, PSTR("Rebooting, please wait ~10 seconds...")); break;
      #ifdef ENABLE_FEATURE_LIGHTING__DMX
      case SUBPAGE_DMX    : strcpy_P(s, PSTR("DMX")); break;
      #endif
      case SUBPAGE_UM     : strcpy_P(s, PSTR("Usermods")); break;
      #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
      case SUBPAGE_2D     : strcpy_P(s, PSTR("2D")); break;
      #endif
      #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
      case SUBPAGE_PINREQ : strcpy_P(s, correctPIN ? PSTR("PIN accepted") : PSTR("PIN rejected")); break;
      #endif
    }

    #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
    if (subPage != SUBPAGE_PINREQ) strcat_P(s, PSTR(" settings saved."));

    if (subPage == SUBPAGE_PINREQ ){//&& correctPIN) {
      subPage = originalSubPage; // on correct PIN load settings page the user intended
    } else {
      // if (!s2[0]) strcpy_P(s2, s_redirecting);

      // bool redirectAfter9s = (subPage == SUBPAGE_WIFI || ((subPage == SUBPAGE_SEC || subPage == SUBPAGE_UM) && doReboot));
      // serveMessage(request, 200, s, s2, redirectAfter9s ? 129 : (correctPIN ? 1 : 3));
      return;
    }
    #endif
  }

  AsyncWebServerResponse *response;
  switch (subPage)
  {
    case SUBPAGE_WIFI    : response = request->beginResponse_P(200, "text/html", PAGE_settings_wifi, PAGE_settings_wifi_length); break;
    case SUBPAGE_LEDS    : response = request->beginResponse_P(200, "text/html", PAGE_settings_leds, PAGE_settings_leds_length); break;
    case SUBPAGE_UI      : response = request->beginResponse_P(200, "text/html", PAGE_settings_ui,   PAGE_settings_ui_length);   break;
    case SUBPAGE_SYNC    : response = request->beginResponse_P(200, "text/html", PAGE_settings_sync, PAGE_settings_sync_length); break;
    case SUBPAGE_TIME    : response = request->beginResponse_P(200, "text/html", PAGE_settings_time, PAGE_settings_time_length); break;
    case SUBPAGE_SEC     : response = request->beginResponse_P(200, "text/html", PAGE_settings_sec,  PAGE_settings_sec_length);  break;
    #ifdef ENABLE_FEATURE_LIGHTING__DMX
    case SUBPAGE_DMX     : response = request->beginResponse_P(200, "text/html", PAGE_settings_dmx,  PAGE_settings_dmx_length);  break;
    #endif
    case SUBPAGE_UM      : response = request->beginResponse_P(200, "text/html", PAGE_settings_um,   PAGE_settings_um_length);   break;
    case SUBPAGE_UPDATE  : response = request->beginResponse_P(200, "text/html", PAGE_update,        PAGE_update_length);        break;
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    case SUBPAGE_2D      : response = request->beginResponse_P(200, "text/html", PAGE_settings_2D,   PAGE_settings_2D_length);   break;
    #endif
    #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
    case SUBPAGE_LOCK    : {
      correctPIN = !strlen(settingsPIN); // lock if a pin is set
      createEditHandler(correctPIN);
      serveMessage(request, 200, strlen(settingsPIN) > 0 ? PSTR("Settings locked") : PSTR("No PIN set"), FPSTR(s_redirecting), 1);
      return;
    }
    #endif
    case SUBPAGE_PINREQ  : response = request->beginResponse_P(200, "text/html", PAGE_settings_pin,  PAGE_settings_pin_length);  break;
    case SUBPAGE_CSS     : response = request->beginResponse_P(200, "text/css",  PAGE_settingsCss,   PAGE_settingsCss_length);   break;
    case SUBPAGE_JS      : serveSettingsJS(request); return;
    case SUBPAGE_WELCOME : response = request->beginResponse_P(200, "text/html", PAGE_welcome,       PAGE_welcome_length);       break;
    default:  response = request->beginResponse_P(200, "text/html", PAGE_settings,      PAGE_settings_length);      break;
  }
  response->addHeader(FPSTR(s_content_enc),"gzip");
  tkr_web->setStaticContentCacheHeaders(response);
  request->send(response);
}

//Is this an IP?
bool mAnimatorLight::isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}


bool  mAnimatorLight::captivePortal(AsyncWebServerRequest *request)
{
  if (!apActive) return false; //only serve captive in AP mode
  if (!request->hasHeader(F("Host"))) return false;

  String hostH = request->getHeader(F("Host"))->value();
  if (!isIp(hostH) && hostH.indexOf(F("wled.me")) < 0 && hostH.indexOf(tkr_web->cmDNS) < 0 && hostH.indexOf(':') < 0) {
    DEBUG_PRINTLN(F("Captive portal"));
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader(F("Location"), F("http://4.3.2.1"));
    request->send(response);
    return true;
  }
  return false;
}


void mAnimatorLight::WebPage_Root_AddHandlers()
{

  releaseJSONBufferLock();

  #ifdef ENABLE_FEATURE_LIGHTING__WEBSOCKETS
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  tkr_web->server->on("/liveview2D", HTTP_GET, [](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, "", 200, FPSTR(CONTENT_TYPE_HTML), PAGE_liveviewws2D, PAGE_liveviewws2D_length);
  });
  #endif
  #endif

  tkr_web->server->on("/liveview", HTTP_GET, [](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, "", 200, FPSTR(CONTENT_TYPE_HTML), PAGE_liveview, PAGE_liveview_length);
  });

  static const char _common_js[] PROGMEM = "/common.js";
  tkr_web->server->on(_common_js, HTTP_GET, [this](AsyncWebServerRequest *request){    
    tkr_web->handleStaticContent(request, FPSTR(_common_js), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_common, JS_common_length);
  });

  //settings page
  tkr_web->server->on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveSettings(request);
  });

  // "/settings/settings.js&p=x" request also handled by serveSettings()
  static const char _style_css[] PROGMEM = "/style.css";
  tkr_web->server->on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, FPSTR(_style_css), 200, FPSTR(CONTENT_TYPE_CSS), PAGE_settingsCss, PAGE_settingsCss_length);
  });

  static const char _favicon_ico[] PROGMEM = "/favicon.ico";
  tkr_web->server->on(_favicon_ico, HTTP_GET, [](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, FPSTR(_favicon_ico), 200, F("image/x-icon"), favicon, favicon_length, false);
  });

  static const char _skin_css[] PROGMEM = "/skin.css";
  tkr_web->server->on(_skin_css, HTTP_GET, [](AsyncWebServerRequest *request){
    if (pCONT_mfile->handleFileRead(request, FPSTR(_skin_css))) return;
    AsyncWebServerResponse *response = request->beginResponse(200, FPSTR(CONTENT_TYPE_CSS));
    request->send(response);
  });

  tkr_web->server->on("/welcome", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveSettings(request);
  });


  tkr_web->server->on("/settings", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->serveSettings(request, true);
  });

  tkr_web->server->on("/json", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveJson(request);
  });

  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/json", [this](AsyncWebServerRequest *request) {
    bool verboseResponse = false;
    bool isConfig = false;

    ALOG_INF(PSTR("AsyncCallbackJsonWebHandler"));

    Serial.println((char*)request->_tempObject);

    if (!this->requestJSONBufferLock(14)) return;

    
      #ifdef ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024

      // // Retrieve the buffer and its length
      // // char* jsonBuffer = static_cast<uint8_t*>(request->_tempObject);
      // // size_t jsonBufferLength = request->contentLength();
      // char* jsonBuffer = (char*)request->_tempObject;
      // ALOG_INF(PSTR("request->contentLength %d"),request->contentLength());
      // for(int i=0;i<request->contentLength();i++){
      //   Serial.printf("[%03d] ",i);
      //   Serial.println((char)jsonBuffer[i]);
      // }

      uint16_t jsonBufferLength = request->contentLength();
      if(jsonBufferLength < 2000)
      {
        /**
         * @brief DEBUG LOAD
         * 
         */
        // char buffer[jsonBufferLength+1];
        // // Copy the contents of jsonBuffer into buffer
        // memcpy(buffer, jsonBuffer, jsonBufferLength);
        // // Null-terminate the buffer
        // buffer[jsonBufferLength] = '\0';
        
        // ALOG_INF(PSTR("webui ui buffer \n\r%s\n\r"), buffer);
        // JsonParser parser(buffer);
        // JsonParserObject rootObj = parser.getRootObject();   
        // if (!rootObj) 
        // {
        //   ALOG_ERR(PSTR("DeserializationError with \"%s\""), buffer);
        //   return;
        // } 
        // else
        // {
        //   ALOG_INF(PSTR("Deserialization Success with \"%s\""), buffer);
        // }

        /**
         * @brief LOAD TO PARSE
         * 
         */
        if(requestDataBufferLock(GetModuleUniqueID()))
        {
          D_DATA_BUFFER_SOFT_CLEAR();

          char* jsonBuffer = (char*)request->_tempObject;

          data_buffer.payload.length_used = jsonBufferLength;
          memcpy(data_buffer.payload.ctr, jsonBuffer, data_buffer.payload.length_used);
          data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // null terminate
        
          LoggingLevels level = LOG_LEVEL_INFO;
          #ifdef ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS
          level = LOG_LEVEL_DEV_TEST;
          #endif
          #ifdef ENABLE_LOG_LEVEL_INFO
          AddLog(level, PSTR(D_LOG_LIGHT "State Payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
          #endif// ENABLE_LOG_LEVEL_INFO

          pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

          releaseDataBufferLock();

        }


      }

      #endif

    DeserializationError error = deserializeJson(doc, (uint8_t*)(request->_tempObject));
    JsonObject root = doc.as<JsonObject>();
    if (error || root.isNull()) {
      this->releaseJSONBufferLock();
      request->send(400, "application/json", F("{\"error\":9}")); // ERR_JSON
      return;
    }
    // if (root.containsKey("pin")) checkSettingsPIN(root["pin"].as<const char*>());

    const String& url = request->url();
    isConfig = url.indexOf("cfg") > -1; 
    if (!isConfig) {
      ALOG_INF(PSTR("deserializeState"));
      serializeJson(root,Serial);
      // DEBUG_PRINTLN();
      // ALOG_DBG(PSTR("deserializeState"));
        
      verboseResponse = this->deserializeState(root);
      
    } else {
      // if (!correctPIN && strlen(settingsPIN)>0) {
      //   request->send(403, "application/json", F("{\"error\":1}")); // ERR_DENIED
      //   this->releaseJSONBufferLock();
      //   return;
      // }
      ALOG_INF(PSTR("deserializeConfig"));
      verboseResponse = this->deserializeConfig(root); //use verboseResponse to determine whether cfg change should be saved immediately
    }


    this->releaseJSONBufferLock();

    if (verboseResponse) {
      if (!isConfig) {
        ALOG_INF(PSTR("Serve the json back, set CALL_MODE_WS_SEND"));
        lastInterfaceUpdate = millis(); // prevent WS update until cooldown
        interfaceUpdateCallMode = CALL_MODE_WS_SEND; // schedule WS update
        this->serveJson(request); return; //if JSON contains "v"
      } else {
        doSerializeConfig = true; //serializeConfig(); //Save new settings to FS
      }
    }
    request->send(200, CONTENT_TYPE_JSON, F("{\"success\":true}"));
  }, JSON_BUFFER_SIZE);
  tkr_web->server->addHandler(handler);



  tkr_web->server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, FPSTR(CONTENT_TYPE_PLAIN), (String)PROJECT_VERSION);
  });

  tkr_web->server->on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, FPSTR(CONTENT_TYPE_PLAIN), (String)millis());
  });

  tkr_web->server->on("/freeheap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, FPSTR(CONTENT_TYPE_PLAIN), (String)ESP.getFreeHeap());
  });

#ifdef WLED_ENABLE_USERMOD_PAGE
  tkr_web->server->on("/u", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_usermod, PAGE_usermod_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
#endif

  tkr_web->server->on("/teapot", HTTP_GET, [this](AsyncWebServerRequest *request){
    tkr_web->serveMessage(request, 418, F("418. I'm a teapot."), F("(Tangible Embedded Advanced Project Of Twinkling)"), 254);
  });

  tkr_web->server->on("/upload", HTTP_POST, [this](AsyncWebServerRequest *request) {},
        [this](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                      size_t len, bool final) {this->handleUpload(request, filename, index, data, len, final);}
  );

  /**
   * @brief 
   * /iro.js and /rangetouch.js are inlined via the compress,minify code during npm compile
   * It adds them into the index.htm file to reduce repeated loads
   **/

  #ifdef ENABLE_FEATURE_LIGHTING__DMX
  tkr_web->server->on("/dmxmap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", PAGE_dmxmap     , dmxProcessor);
  });
  #else
  tkr_web->server->on("/dmxmap", HTTP_GET, [this](AsyncWebServerRequest *request){
    tkr_web->serveMessage(request, 501, "Not implemented", F("DMX support is not enabled in this build."), 254);
  });
  #endif

  tkr_web->server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (captivePortal(request)) return;
    if (!showWelcomePage || request->hasArg(F("sliders"))) {
      tkr_web->handleStaticContent(request, F("/index.htm"), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_index, PAGE_index_L);
    } else {
      serveSettings(request);
    }
  });

  #ifdef WLED_ENABLE_PIXART
  static const char _pixart_htm[] PROGMEM = "/pixart.htm";
  tkr_web->server->on(_pixart_htm, HTTP_GET, [](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, FPSTR(_pixart_htm), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_pixart, PAGE_pixart_L);
  });
  #endif

  #ifndef WLED_DISABLE_PXMAGIC
  static const char _pxmagic_htm[] PROGMEM = "/pxmagic.htm";
  tkr_web->server->on(_pxmagic_htm, HTTP_GET, [](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, FPSTR(_pxmagic_htm), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_pxmagic, PAGE_pxmagic_L);
  });
  #endif

  static const char _cpal_htm[] PROGMEM = "/cpal.htm";
  tkr_web->server->on(_cpal_htm, HTTP_GET, [](AsyncWebServerRequest *request){
    tkr_web->handleStaticContent(request, FPSTR(_cpal_htm), 200, FPSTR(CONTENT_TYPE_HTML), PAGE_cpal, PAGE_cpal_L);
  });

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  
  //called when the url is not defined here, ajax-in; get-settings
  tkr_web->server->onNotFound([this](AsyncWebServerRequest *request)
  {
    ALOG_ERR(PSTR("HTTP URI Not-Found: %s"), request->url());    
    if (this->captivePortal(request)) return;

    //make API CORS compatible
    if (request->method() == HTTP_OPTIONS)
    {
      AsyncWebServerResponse *response = request->beginResponse(200);
      response->addHeader(F("Access-Control-Max-Age"), F("7200"));
      request->send(response);
      return;
    }
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS
    if(handleSet(request, request->url())) return;
    #endif
    tkr_web->handleStaticContent(request, request->url(), 404, FPSTR(CONTENT_TYPE_HTML), PAGE_404, PAGE_404_length);
  });

  #endif // ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE

  
}


#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI

#endif // USE_MODULE_LIGHTS_ANIMATOR