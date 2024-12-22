#include "mAnimatorLight.h"



#ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS

/*
 * Methods to handle saving and loading presets to/from the filesystem



 I want to serialise the preset file and send over mqtt anytime it changes as debug

 */

#ifdef ARDUINO_ARCH_ESP32
static char *tmpRAMbuffer = nullptr;
#endif


const char *mAnimatorLight::getFileName(bool persist) 
{
  return persist ? "/presets.json" : "/tmp.json";
}


void mAnimatorLight::doSaveState() 
{

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  ALOG_INF(PSTR("doSaveState() START"));
  // CommandSet_ReadFile("/presets.json");
  #endif

  bool persist = (presetToSave < 251);
  const char *filename = getFileName(persist);
  
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  if (!requestJSONBufferLock(10)) return; // will set fileDoc

  initPresetsFile(); // just in case if someone deleted presets.json using /edit

  JsonObject sObj = doc.to<JsonObject>();

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  DEBUG_PRINTLN(F("Serialize current state"));
  
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif
  
  if (playlistSave) 
  {
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
    serializePlaylist(sObj);
    #endif
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    if (includeBri) sObj["on"] = true;
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

  } 
  else 
  {
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    serializeState(sObj, true, includeBri, segBounds, selectedOnly);
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif
  }
  
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  sObj["n"] = saveName;
  if (quickLoad[0]) sObj[F("ql")] = quickLoad;
  if (saveLedmap >= 0) sObj[F("ledmap")] = saveLedmap;

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  DEBUG_PRINTLN(F("Serialized preset"));
  serializeJson(doc,Serial);
  DEBUG_PRINTLN();
  #endif

  #if defined(ARDUINO_ARCH_ESP32)
  if (!persist) 
  {

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    if (tmpRAMbuffer!=nullptr) free(tmpRAMbuffer);

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    size_t len = measureJson(*pCONT_mfile->fileDoc) + 1;

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    ALOG_INF(PSTR("JSON Len=%d"),len);
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    // if possible use SPI RAM on ESP32
    #if defined(BOARD_HAS_PSRAM) && defined(WLED_USE_PSRAM)
    if (psramFound())
      tmpRAMbuffer = (char*) ps_malloc(len);
    else
    #endif
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    tmpRAMbuffer = (char*) malloc(len);
 
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif
    
    if (tmpRAMbuffer!=nullptr) 
    {
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif
      
      serializeJson(*pCONT_mfile->fileDoc, tmpRAMbuffer, len);
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif
    
    }
    else
    {
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif

      pCONT_mfile->writeObjectToFileUsingId(filename, presetToSave, pCONT_mfile->fileDoc);
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif
    }
  } 
  else
  {
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    pCONT_mfile->writeObjectToFileUsingId(filename, presetToSave, pCONT_mfile->fileDoc);
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif
  
  }
  #endif

  if (persist) pCONT_mfile->presetsModifiedTime = toki.second(); //unix time
  
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  releaseJSONBufferLock();

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  pCONT_mfile->updateFSInfo();

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  // clean up
  saveLedmap   = -1;
  presetToSave = 0;
  saveName[0]  = '\0';
  quickLoad[0] = '\0';
  playlistSave = false;

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  ALOG_INF(PSTR("doSaveState() END"));
  // CommandSet_ReadFile("/presets.json");
  #endif

}


bool mAnimatorLight::getPresetName(byte index, String& name)
{

  if (!requestJSONBufferLock(9)) return false;
  bool presetExists = false;
  if (pCONT_mfile->readObjectFromFileUsingId(getFileName(), index, &doc))
  {
    JsonObject fdo = doc.as<JsonObject>();
    if (fdo["n"]) {
      name = (const char*)(fdo["n"]);
      presetExists = true;
    }
  }
  releaseJSONBufferLock();
  return presetExists;

}


void mAnimatorLight::initPresetsFile()
{
  
  if (FILE_SYSTEM.exists(getFileName()))
  {
    ALOG_INF(PSTR("initPresetsFile() -- already exists, exiting"));   
    return;
  }

  StaticJsonDocument<64> doc;
  JsonObject sObj = doc.to<JsonObject>();
  sObj.createNestedObject("0");
  File f = FILE_SYSTEM.open(getFileName(), "w");
  
  ALOG_INF(PSTR("initPresetsFile() -- creating init file"));

  if (!f) {
    pCONT_mfile->errorFlag = ERR_FS_GENERAL;
    return;
  }
  serializeJson(doc, f);
  f.close();

}


bool mAnimatorLight::applyPreset(byte index, byte callMode)
{
  ALOG_INF(PSTR("Request to apply preset: %d"), index);
  presetToApply = index;
  callModeToApply = callMode;
  return true;
}


// apply preset or fallback to a effect and palette if it doesn't exist
void mAnimatorLight::applyPresetWithFallback(uint8_t index, uint8_t callMode, uint8_t effectID, uint8_t paletteID)
{
  applyPreset(index, callMode);  
  effectCurrent = effectID; // these two will be overwritten if preset exists in SubTask_Presets()
  effectPalette = paletteID; // these two will be overwritten if preset exists in SubTask_Presets()
}


/**
 * @brief Gets called every loop, returns if none are waiting
 * 
 */
void mAnimatorLight::SubTask_Presets()
{

  if (presetToSave) 
  {
    // ALOG_INF(PSTR(" (presetToSave)()"));
    doSaveState();
    return;
  }

  if (presetToApply == 0 || pCONT_mfile->fileDoc)
  {
    // ALOG_INF(PSTR("(presetToApply == 0 || fileDoc)()"));    
    return; // no preset waiting to apply, or JSON buffer is already allocated, return to loop until free
  }

  ALOG_INF(PSTR("mAnimatorLight::SubTask_Presets presetToApply %d"), presetToApply);


  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  bool changePreset = false;
  uint8_t tmpPreset = presetToApply; // store temporary since deserializeState() may call applyPreset()
  uint8_t tmpMode   = callModeToApply;
 
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  JsonObject fdo;
  const char *filename = getFileName(tmpPreset < 255);

  // allocate buffer
  if (!requestJSONBufferLock(9)) return;  // will also assign fileDoc

  presetToApply = 0; //clear request for preset
  callModeToApply = 0;

  ALOG_INF(PSTR("Applying preset:%d"), tmpPreset);

  #ifdef ARDUINO_ARCH_ESP32
  if (tmpPreset==255 && tmpRAMbuffer!=nullptr) 
  { 
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif
    deserializeJson(*pCONT_mfile->fileDoc,tmpRAMbuffer);
    pCONT_mfile->errorFlag = ERR_NONE;
  } 
  else
  #endif
  { 

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    pCONT_mfile->errorFlag = pCONT_mfile->readObjectFromFileUsingId(filename, tmpPreset, pCONT_mfile->fileDoc) ? ERR_NONE : ERR_FS_PLOAD;

  } 

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  fdo = pCONT_mfile->fileDoc->as<JsonObject>();

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  // ALOG_HGL(PSTR("mAnimatorLight::SubTask_Presets is now being read as fileDoc and not compatable with my command structure"));

  // delay(4000);

  /**
   * @brief Run through my command structure.
   * 
   */
  if(requestDataBufferLock(GetModuleUniqueID()))
  {
    D_DATA_BUFFER_SOFT_CLEAR();

    // Serialise from ArduinoJson into buffer for parser to load
    serializeJson(*pCONT_mfile->fileDoc, data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr));

    LoggingLevels level = LOG_LEVEL_INFO;
    #ifdef ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS
    level = LOG_LEVEL_TEST;
    #endif
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(level, PSTR(D_LOG_LIGHT "My parser payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
    #endif// ENABLE_LOG_LEVEL_INFO

    pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

    releaseDataBufferLock();

  }


  //HTTP API commands
  const char* httpwin = fdo["win"];
  if (httpwin) { 

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    String apireq = "win"; // reduce flash string usage
    apireq += F("&IN&"); // internal call
    apireq += httpwin;
    #ifdef ENABLE_DEVFEATURE_LIGHTING__SETTINGS
    handleSet(nullptr, apireq, false); // may call applyPreset() via PL=
    setValuesFromFirstSelectedSeg(); // fills legacy values
    #endif // ENABLE_DEVFEATURE_LIGHTING__SETTINGS
    changePreset = true;

  } 
  else
  { 

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    ALOG_INF(PSTR("Here might be what resets the segments without my commands"));

    if (!fdo["seg"].isNull() || !fdo["on"].isNull() || !fdo["bri"].isNull() || !fdo["nl"].isNull() || !fdo["ps"].isNull() || !fdo[F("playlist")].isNull()) changePreset = true;
    if (!(tmpMode == CALL_MODE_BUTTON_PRESET && fdo["ps"].is<const char *>() && strchr(fdo["ps"].as<const char *>(),'~') != strrchr(fdo["ps"].as<const char *>(),'~')))
      fdo.remove("ps"); // remove load request for presets to prevent recursive crash (if not called by button and contains preset cycling string "1~5~")
    deserializeState(fdo, CALL_MODE_NO_NOTIFY, tmpPreset); // may change presetToApply by calling applyPreset()

  }

  if (!pCONT_mfile->errorFlag && tmpPreset < 255 && changePreset)
  {
    presetCycCurr = currentPreset = tmpPreset;
  }
 
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  #if defined(ARDUINO_ARCH_ESP32)
  //Aircoookie recommended not to delete buffer
  if (tmpPreset==255 && tmpRAMbuffer!=nullptr) {
    free(tmpRAMbuffer);
    tmpRAMbuffer = nullptr;
  }
  #endif

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

  releaseJSONBufferLock(); // will also clear fileDoc

  if (changePreset) notify(tmpMode); // force UDP notification

  // stateUpdated(tmpMode);  // was colorUpdated() if anything breaks
  Serial.println("stateUpdated() missing");
  // updateInterfaces(tmpMode);
  Serial.println("updateInterfaces() missing");

}

#ifdef ENABLE_FEATURE_LIGHTS__DEMO_MODE
void mAnimatorLight::SubTask_Demo()
{

}
#endif


//called from handleSet(PS=) [network callback (fileDoc==nullptr), IR (irrational), deserializeState, UDP] and deserializeState() [network callback (filedoc!=nullptr)]
void mAnimatorLight::savePreset(byte index, const char* pname, JsonObject sObj)
{

  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "savePreset()"));

  if (index == 0 || (index > 250 && index < 255)) return;
  if (pname) strlcpy(saveName, pname, 33);
  else {
    if (sObj["n"].is<const char*>()) strlcpy(saveName, sObj["n"].as<const char*>(), 33);
    else                             sprintf_P(saveName, PSTR("Preset %d"), index);
  }

  DEBUG_PRINT(F("Saving preset (")); DEBUG_PRINT(index); DEBUG_PRINT(F(") ")); DEBUG_PRINTLN(saveName);

  presetToSave = index;
  playlistSave = false;
  if (sObj[F("ql")].is<const char*>())
  {
    strlcpy(quickLoad, sObj[F("ql")].as<const char*>(), 9); // client limits QL to 2 chars, buffer for 8 bytes to allow unicode
  } 

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif
  
  if (sObj["o"].isNull()) 
  { // no "o" means not a playlist or custom API call, saving of state is async (not immediately)

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif
    includeBri   = sObj["ib"].as<bool>() || index==255; // temporary preset needs brightness
    segBounds    = sObj["sb"].as<bool>() || index==255; // temporary preset needs bounds
    selectedOnly = sObj[F("sc")].as<bool>();
    saveLedmap   = sObj[F("ledmap")] | -1;

  } 
  else 
  {

    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
    DEBUG_LINE_HERE;
    #endif

    // this is a playlist or API call
    if (sObj[F("playlist")].isNull()) 
    {
      // we will save API call immediately (often causes presets.json corruption)
      presetToSave = 0;
      if (index > 250 || !pCONT_mfile->fileDoc) return; // cannot save API calls to temporary preset (255)
      sObj.remove("o");
      sObj.remove("v");
      sObj.remove("time");
      sObj.remove(F("error"));
      sObj.remove(F("psave"));
      if (sObj["n"].isNull()) sObj["n"] = saveName;
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif

      initPresetsFile(); // just in case if someone deleted presets.json using /edit
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif

      pCONT_mfile->writeObjectToFileUsingId(getFileName(index<255), index, pCONT_mfile->fileDoc);
    
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif
      
      pCONT_mfile->presetsModifiedTime = toki.second(); //unix time
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif

      pCONT_mfile->updateFSInfo();

    } 
    else 
    {
  
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
      DEBUG_LINE_HERE;
      #endif

      // store playlist
      // WARNING: playlist will be loaded in json.cpp after this call and will have repeat counter increased by 1
      includeBri   = true; // !sObj["on"].isNull();
      playlistSave = true;

    }
  }
  
  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  #endif

}

void mAnimatorLight::deletePreset(byte index) 
{

  StaticJsonDocument<24> empty;
  pCONT_mfile->writeObjectToFileUsingId(getFileName(), index, &empty);
  pCONT_mfile->presetsModifiedTime = toki.second(); //unix time
  pCONT_mfile->updateFSInfo();

}


#endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS