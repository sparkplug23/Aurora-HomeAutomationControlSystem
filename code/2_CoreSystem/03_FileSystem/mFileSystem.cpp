/**
 * @file mFileSystem.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-07
 * 
 * There are two types of filesystems:
 * 
 * 1) Internal flash filesystem
 * 2) SD Card filesystem
 * https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
 * @copyright Copyright (c) 2023
 * 
 * Step 1: build now and be able to view files on the SD card in the webui editor
 * 
 * Three types of files, stored as (.byte, .json, .txt)
 * 
 * sys_ = system files
 * drv_
 * sns_
 * lgt_anim_0# = When data is  more than signal file can hold, it is split into multiple files
 * egy_
 * dsp_
 * con_
 * cns_
 * 
 * 
 * When possible, all data from a module should be held within a "DATA"/"data" struct, and this will be called to save as ".byte" and when requested, "restore state at boot"
 * "If data cant fit in a struct because of its format, then save and load as .json". Anything in json will be passed through the command parser.
 * ".txt" files are for "logs" and "debugging". 
 */

#include "mFileSystem.h"

#ifdef USE_MODULE_CORE_FILESYSTEM

#ifndef ERR_FS_QUOTA
#define ERR_FS_QUOTA    11  // The FS is full or the maximum file size is reached
#endif

/*
 * Utility for SPIFFS filesystem
 */

#ifdef ARDUINO_ARCH_ESP32 //FS info bare IDF function until FS wrapper is available for ESP32
#if FILE_SYSTEM != LITTLEFS && ESP_IDF_VERSION_MAJOR < 4
  #include "esp_spiffs.h"
#endif
#endif

#define WLED_DEBUG_FS2

#define FS_BUFSIZE 256

/*
 * Structural requirements for files managed by writeObjectToFile() and readObjectFromFile() utilities:
 * 1. File must be a string representation of a valid JSON object
 * 2. File must have '{' as first character
 * 3. There must not be any additional characters between a root-level key and its value object (e.g. space, tab, newline)
 * 4. There must not be any characters between an root object-separating ',' and the next object key string
 * 5. There may be any number of spaces, tabs, and/or newlines before such object-separating ','
 * 6. There must not be more than 5 consecutive spaces at any point except for those permitted in condition 5
 * 7. If it is desired to delete the first usable object (e.g. preset file), a dummy object '"0":{}' is inserted at the beginning.
 *    It shall be disregarded by receiving software.
 *    The reason for it is that deleting the first preset would require special code to handle commas between it and the 2nd preset
 */

// There are no consecutive spaces longer than this in the file, so if more space is required, findSpace() can return false immediately
// Actual space may be lower
constexpr size_t MAX_SPACE = UINT16_MAX * 2U;           // smallest supported config has 128Kb flash size
volatile size_t knownLargestSpace = MAX_SPACE;

File f; // don't export to other cpp files

// wrapper to find out how long closing takes
void mFileSystem::closeFile() {
  #ifdef WLED_DEBUG_FS2
    Serial.println("Close -> ");
    uint32_t s = millis();
  #endif
  f.close();
  Serial.printf("took %d ms\n", millis() - s);
  doCloseFile = false;
}


// find() that reads and buffers data from file stream in 256-byte blocks.
// Significantly faster, f.find(key) can take SECONDS for multi-kB files
bool mFileSystem::bufferedFind(const char *target, bool fromStart)
{
  #ifdef WLED_DEBUG_FS2
    Serial.printf("Find ");
    Serial.printf(target);
    uint32_t s = millis();
  #endif

  if (!f || !f.size()) return false;
  size_t targetLen = strlen(target);

  size_t index = 0;
  byte buf[FS_BUFSIZE];
  if (fromStart) f.seek(0);

  while (f.position() < f.size() -1) {
    size_t bufsize = f.read(buf, FS_BUFSIZE); // better to use size_t instead if uint16_t
    size_t count = 0;
    while (count < bufsize) {
      if(buf[count] != target[index])
      index = 0; // reset index if any char does not match

      if(buf[count] == target[index]) {
        if(++index >= targetLen) { // return true if all chars in the target match
          f.seek((f.position() - bufsize) + count +1);
          Serial.printf("Found at pos %d, took %d ms", f.position(), millis() - s);
          return true;
        }
      }
      count++;
    }
  }
  Serial.printf("No match, took %d ms\n", millis() - s);
  return false;
}


// find empty spots in file stream in 256-byte blocks.
bool mFileSystem::bufferedFindSpace(size_t targetLen, bool fromStart) 
{

  #ifdef WLED_DEBUG_FS2
    Serial.printf("Find %d spaces\n", targetLen);
    uint32_t s = millis();
  #endif

  if (knownLargestSpace < targetLen) {
    Serial.printf("No match, KLS ");
    Serial.println(knownLargestSpace);
    return false;
  }

  if (!f || !f.size()) return false;

  size_t index = 0; // better to use size_t instead if uint16_t
  byte buf[FS_BUFSIZE];
  if (fromStart) f.seek(0);

  while (f.position() < f.size() -1) {
    size_t bufsize = f.read(buf, FS_BUFSIZE);
    size_t count = 0;

    while (count < bufsize) {
      if(buf[count] == ' ') {
        if(++index >= targetLen) { // return true if space long enough
          if (fromStart) {
            f.seek((f.position() - bufsize) + count +1 - targetLen);
            knownLargestSpace = MAX_SPACE; //there may be larger spaces after, so we don't know
          }
          Serial.printf("Found at pos %d, took %d ms", f.position(), millis() - s);
          return true;
        }
      } else {
        if (!fromStart) return false;
        if (index) {
          if (knownLargestSpace < index || (knownLargestSpace == MAX_SPACE)) knownLargestSpace = index;
          index = 0; // reset index if not space
        }
      }

      count++;
    }
  }
  Serial.printf("No match, took %d ms\n", millis() - s);
  return false;
}


// find the closing bracket corresponding to the opening bracket at the file pos when calling this function
bool mFileSystem::bufferedFindObjectEnd() 
{
  #ifdef WLED_DEBUG_FS2
    Serial.println("Find obj end");
    uint32_t s = millis();
  #endif

  if (!f || !f.size()) return false;

  uint16_t objDepth = 0; //num of '{' minus num of '}'. return once 0
  //size_t start = f.position();
  byte buf[FS_BUFSIZE];

  while (f.position() < f.size() -1) {
    size_t bufsize = f.read(buf, FS_BUFSIZE); // better to use size_t instead of uint16_t
    size_t count = 0;

    while (count < bufsize) {
      if (buf[count] == '{') objDepth++;
      if (buf[count] == '}') objDepth--;
      if (objDepth == 0) {
        f.seek((f.position() - bufsize) + count +1);
        Serial.printf("} at pos %d, took %d ms", f.position(), millis() - s);
        return true;
      }
      count++;
    }
  }
  Serial.printf("No match, took %d ms\n", millis() - s);
  return false;
}


// fills n bytes from current file pos with ' ' characters
void mFileSystem::writeSpace(size_t l)
{
  byte buf[FS_BUFSIZE];
  memset(buf, ' ', FS_BUFSIZE);

  while (l > 0) {
    size_t block = (l>FS_BUFSIZE) ? FS_BUFSIZE : l;
    f.write(buf, block);
    l -= block;
  }

  if (knownLargestSpace < l) knownLargestSpace = l;
}


bool mFileSystem::appendObjectToFile(const char* key, JsonDocument* content, uint32_t s, uint32_t contentLen)
{
  #ifdef WLED_DEBUG_FS2
    Serial.println("Append");
    uint32_t s1 = millis();
  #endif
  uint32_t pos = 0;
  if (!f) return false;

  if (f.size() < 3) {
    char init[10];
    strcpy_P(init, PSTR("{\"0\":{}}"));
    f.print(init);
  }

  if (content->isNull()) {
    doCloseFile = true;
    return true; //nothing  to append
  }

  //if there is enough empty space in file, insert there instead of appending
  if (!contentLen) contentLen = measureJson(*content);
  Serial.printf("CLen %d\n", contentLen);
  if (bufferedFindSpace(contentLen + strlen(key) + 1)) {
    if (f.position() > 2) f.write(','); //add comma if not first object
    f.print(key);
    serializeJson(*content, f);
    Serial.printf("Inserted, took %d ms (total %d)", millis() - s1, millis() - s);
    doCloseFile = true;
    return true;
  }

  //not enough space, append at end

  //permitted space for presets exceeded
  updateFSInfo();

  if (f.size() + 9000 > (fsBytesTotal - fsBytesUsed)) { //make sure there is enough space to at least copy the file once
    errorFlag = ERR_FS_QUOTA;
    doCloseFile = true;
    return false;
  }

  //check if last character in file is '}' (typical)
  uint32_t eof = f.size() -1;
  f.seek(eof, SeekSet);
  if (f.read() == '}') pos = eof;

  if (pos == 0) //not found
  {
    Serial.printf("not }");
    f.seek(0);
    while (bufferedFind("}",false)) //find last closing bracket in JSON if not last char
    {
      pos = f.position();
    }
    if (pos > 0) pos--;
  }
  Serial.printf("pos "); Serial.println(pos);
  if (pos > 2)
  {
    f.seek(pos, SeekSet);
    f.write(',');
  } else { //file content is not valid JSON object
    f.seek(0, SeekSet);
    f.print('{'); //start JSON
  }

  f.print(key);

  //Append object
  serializeJson(*content, f);
  f.write('}');

  doCloseFile = true;
  Serial.printf("Appended, took %d ms (total %d)", millis() - s1, millis() - s);
  return true;
}


bool mFileSystem::writeObjectToFileUsingId(const char* file, uint16_t id, JsonDocument* content)
{
  char objKey[10];
  sprintf(objKey, "\"%d\":", id);
  DEBUG_LINE_HERE;
  return writeObjectToFile(file, objKey, content);
}


bool mFileSystem::writeObjectToFile(const char* file, const char* key, JsonDocument* content)
{
  uint32_t s = 0; // timing

  #ifdef WLED_DEBUG_FS2
    Serial.printf("Write to %s with key %s >>>\n\r", file, (key==nullptr)?"nullptr":key);
    serializeJson(*content, Serial); 
    Serial.println();
    s = millis();
  #endif

  size_t pos = 0;
  f = FILE_SYSTEM.open(file, "r+");
  if (!f && !FILE_SYSTEM.exists(file)) f = FILE_SYSTEM.open(file, "w+");
  if (!f) {
    Serial.println("Failed to open!");
    return false;
  }

  DEBUG_LINE_HERE;
  if (!bufferedFind(key)) //key does not exist in file
  {
  DEBUG_LINE_HERE;
    return appendObjectToFile(key, content, s);
  }

  DEBUG_LINE_HERE;
  //an object with this key already exists, replace or delete it
  pos = f.position();
  //measure out end of old object
  bufferedFindObjectEnd();
  size_t pos2 = f.position();

  uint32_t oldLen = pos2 - pos;
  Serial.printf("Old obj len %d\n", oldLen);

  //Three cases:
  //1. The new content is null, overwrite old obj with spaces
  //2. The new content is smaller than the old, overwrite and fill diff with spaces
  //3. The new content is larger than the old, but smaller than old + trailing spaces, overwrite with new
  //4. The new content is larger than old + trailing spaces, delete old and append

  size_t contentLen = 0;
  if (!content->isNull()) contentLen = measureJson(*content);

  if (contentLen && contentLen <= oldLen) { //replace and fill diff with spaces
    Serial.println("replace");
    f.seek(pos);
    serializeJson(*content, f);
    writeSpace(pos2 - f.position());
  } else if (contentLen && bufferedFindSpace(contentLen - oldLen, false)) { //enough leading spaces to replace
    Serial.println("replace (trailing)");
    f.seek(pos);
    serializeJson(*content, f);
  } else {
    Serial.println("delete");
    pos -= strlen(key);
    if (pos > 3) pos--; //also delete leading comma if not first object
    f.seek(pos);
    writeSpace(pos2 - pos);
    if (contentLen) return appendObjectToFile(key, content, s, contentLen);
  }

  doCloseFile = true;
  Serial.printf("Replaced/deleted, took %d ms\n", millis() - s);
  return true;
}


bool mFileSystem::readObjectFromFileUsingId(const char* file, uint16_t id, JsonDocument* dest)
{
  char objKey[10];
  sprintf(objKey, "\"%d\":", id);
  return readObjectFromFile(file, objKey, dest);
}


//if the key is a nullptr, deserialize entire object
bool mFileSystem::readObjectFromFile(const char* file, const char* key, JsonDocument* dest)
{
  if (doCloseFile) closeFile();
  #ifdef WLED_DEBUG_FS2
    Serial.printf("Read from %s with key %s >>>\n", file, (key==nullptr)?"nullptr":key);
    uint32_t s = millis();
  #endif
  f = FILE_SYSTEM.open(file, "r");
  if (!f) return false;

  if (key != nullptr && !bufferedFind(key)) //key does not exist in file
  {
    f.close();
    dest->clear();
    Serial.println("Obj not found.");
    return false;
  }

  deserializeJson(*dest, f);

  f.close();
  Serial.printf("Read, took %d ms\n", millis() - s);
  return true;
}


void mFileSystem::updateFSInfo() 
{
  #ifdef ARDUINO_ARCH_ESP32
    #if FILE_SYSTEM == LITTLEFS || ESP_IDF_VERSION_MAJOR >= 4
    fsBytesTotal = FILE_SYSTEM.totalBytes();
    fsBytesUsed = FILE_SYSTEM.usedBytes();
    #else
    esp_spiffs_info(nullptr, &fsBytesTotal, &fsBytesUsed);
    #endif
  #else
    FSInfo fsi;
    FILE_SYSTEM.info(fsi);
    fsBytesUsed  = fsi.usedBytes;
    fsBytesTotal = fsi.totalBytes;
  #endif
}


#ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI


//Un-comment any file types you need
String mFileSystem::getContentType(AsyncWebServerRequest* request, String filename)
{

  if(request->hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".json")) return "application/json";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
//  else if(filename.endsWith(".xml")) return "text/xml";
//  else if(filename.endsWith(".pdf")) return "application/x-pdf";
//  else if(filename.endsWith(".zip")) return "application/x-zip";
//  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


#ifdef ARDUINO_ARCH_ESP32
// caching presets in PSRAM may prevent occasional flashes seen when HomeAssitant polls WLED
// original idea by @akaricchi (https://github.com/Akaricchi)
// returns a pointer to the PSRAM buffer, updates size parameter
static const uint8_t *getPresetCache(size_t &size) {
  static unsigned long presetsCachedTime = 0;
  static uint8_t *presetsCached = nullptr;
  static size_t presetsCachedSize = 0;
  static byte presetsCachedValidate = 0;

  //if (presetsModifiedTime != presetsCachedTime) DEBUG_PRINTLN(F("getPresetCache(): presetsModifiedTime changed."));
  //if (presetsCachedValidate != cacheInvalidate) DEBUG_PRINTLN(F("getPresetCache(): cacheInvalidate changed."));

  if ((tkr_anim->presetsModifiedTime != presetsCachedTime) || (presetsCachedValidate != pCONT_web->cacheInvalidate)) {
    if (presetsCached) {
      free(presetsCached);
      presetsCached = nullptr;
    }
  }

  if (!presetsCached) {
    File file = FILE_SYSTEM.open(FPSTR( tkr_anim->getPresetsFileName() ), "r");
    if (file) {
      presetsCachedTime = tkr_anim->presetsModifiedTime;
      presetsCachedValidate = pCONT_web->cacheInvalidate;
      presetsCachedSize = 0;
      presetsCached = (uint8_t*)ps_malloc(file.size() + 1);
      if (presetsCached) {
        presetsCachedSize = file.size();
        file.read(presetsCached, presetsCachedSize);
        presetsCached[presetsCachedSize] = 0;
        file.close();
      }
    }
  }

  size = presetsCachedSize;
  return presetsCached;
}
#endif

bool mFileSystem::handleFileRead(AsyncWebServerRequest* request, String path){
  
  ALOG_DBG(PSTR("WS FileRead: %s"), path);
  if(path.endsWith("/")) path += "index.htm";
  if(path.indexOf(F("sec")) > -1) return false;
  #ifdef ARDUINO_ARCH_ESP32
  if (psramSafe && psramFound() && path.endsWith(FPSTR(  tkr_anim->getPresetsFileName() ))) {
    size_t psize;
    const uint8_t *presets = getPresetCache(psize);
    if (presets) {
      AsyncWebServerResponse *response = request->beginResponse_P(200, FPSTR(CONTENT_TYPE_JSON), presets, psize);
      request->send(response);
      return true;
    }
  }
  #endif
  if(FILE_SYSTEM.exists(path) || FILE_SYSTEM.exists(path + ".gz")) {
    request->send(request->beginResponse(FILE_SYSTEM, path, {}, request->hasArg(F("download")), {}));
    return true;
  }
  return false;



  // ALOG_DBG(PSTR("WS FileRead: %s"), path);
  
  // if(path.endsWith("/")) path += "index.htm";
  // if(path.indexOf("sec") > -1) return false;
  // String contentType = getContentType(request, path);
  // if(FILE_SYSTEM.exists(path)) {
  //   ALOG_INF(PSTR("Sending file %s from FILE_SYSTEM"), path.c_str());
  //   request->send(FILE_SYSTEM, path, contentType);
  //   return true;
  // }
  // return false;
  
}


#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI


int8_t mFileSystem::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      init();
    break;
  }

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:  
    break;
     case TASK_LOOP:
    //   UfsExecuteCommandFileLoop();
    break;
    case TASK_EVERY_FIVE_SECOND:    
      #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
      SystemTask__Execute_Module_Data_Save();
      #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
    break;
    case TASK_EVERY_MINUTE:
      // #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA // This will in the future only occur once an hour, or before planned boot
      #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
      SystemTask__Execute_Module_Data_Save();
      #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
      // #endif     
    break;  
    case TASK_EVERY_FIVE_MINUTE:
      #ifdef ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
      JsonFile_Save__Stored_Module();
      JsonFile_Save__Stored_Secure();
      #endif // ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
       
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

    }  

} // END Tasker



void mFileSystem::SystemTask__Execute_Module_Data_Save()
{

  ALOG_INF(PSTR("SystemTask__Execute_Module_Data_Save"));

  pCONT->Tasker_Interface(TASK_FILESYSTEM__SAVE__MODULE_DATA__ID);

}


void mFileSystem::FileWrite_Test()
{

  File f; // don't export to other cpp files

  size_t pos = 0;

  // listDir(FILE_SYSTEM, "/", 0);

  const char* file = "/test.txt";
  
  f = FILE_SYSTEM.open(file, "r+");
  
  if (!f && !FILE_SYSTEM.exists(file))
  {
    f = FILE_SYSTEM.open(file, "w+");
    ALOG_INF(PSTR("File did not exist, creating a new one"));
  }
  
  if (!f) {
    Serial.println("Failed to open!");
    return;
  }


  JBI->Start();

  JBI->Add(PSTR("tEST"), millis());


  JBI->End();

  f.print(JBI->GetBufferPtr());
  
  f.close();
  
  ALOG_INF(PSTR("Writing file \"%s\""), JBI->GetBufferPtr());


}


void mFileSystem::init(void)
{
  
  // analogWriteFreq(25000);
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chain

  
  // if(!SD_MMC.begin()){
  //     Serial.println("Card Mount Failed");
  //     return;
  // }
  // uint8_t cardType = SD_MMC.cardType();

  // if(cardType == CARD_NONE){
  //     Serial.println("No SD_MMC card attached");
  //     return;
  // }

  // Serial.print("SD_MMC Card Type: ");
  // if(cardType == CARD_MMC){
  //     Serial.println("MMC");
  // } else if(cardType == CARD_SD){
  //     Serial.println("SDSC");
  // } else if(cardType == CARD_SDHC){
  //     Serial.println("SDHC");
  // } else {
  //     Serial.println("UNKNOWN");
  // }

  // uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  // Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  // listDir(SD_MMC, "/", 0);
  // createDir(SD_MMC, "/mydir");
  // listDir(SD_MMC, "/", 0);
  // //removeDir(SD_MMC, "/mydir");
  // listDir(SD_MMC, "/", 2);
  // writeFile(SD_MMC, "/hello.txt", "Hello ");
  // appendFile(SD_MMC, "/hello.txt", "World!\n");
  // readFile(SD_MMC, "/hello.txt");
  // //deleteFile(SD_MMC, "/foo.txt");
  // renameFile(SD_MMC, "/hello.txt", "/foo.txt");
  // readFile(SD_MMC, "/foo.txt");
  // testFileIO(SD_MMC, "/test.txt");
  // Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

  settings.fEnableModule = true;

}


// This inits flash file only
void mFileSystem::UfsInit(void) 
{

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "mFileSystem::UfsInit"));
  
  // #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  bool fsinit = false;
  DEBUG_PRINTLN(F("Mount FS"));
  #ifdef ARDUINO_ARCH_ESP32
    fsinit = FILE_SYSTEM.begin(true);
  #else
    fsinit = FILE_SYSTEM.begin();
    // if(!fsinit)
    // {
    //   LittleFS.format();
    //   fsinit = FILE_SYSTEM.begin();
    // }
  #endif
  if (!fsinit) {
    DEBUG_PRINTLN(F("FS failed!"));
    // errorFlag = ERR_FS_BEGIN;
    // delay(5000);
  }else{
    DEBUG_PRINTLN(F("FS mounted."));
  }
  // #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS

  // #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE

  UfsData.run_file_pos = -1;
  UfsInitOnce();
  if (ufs_type) {
    ALOG_INF(PSTR(D_LOG_FILESYSTEM "FlashFS mounted with %d kB free"), UfsInfo(1, 0));
  }

  // #endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE

}


// // Init flash file system
void mFileSystem::UfsInitOnce(void) {
  ALOG_INF(PSTR(D_LOG_FILESYSTEM "mFileSystem::UfsInitOnce"));
  ufs_type = 0;
  ffsp = 0;
  ufs_dir = 0;

#ifdef ESP8266
  ffsp = &LittleFS;
  if (!LittleFS.begin()) {
  	ALOG_INF(PSTR(D_LOG_FILESYSTEM "!LittleFS.begin()"));
    ffsp = nullptr;
    return;
  }
  ALOG_INF(PSTR(D_LOG_FILESYSTEM  "LittleFS.begin()"));
#endif  // ESP8266

#ifdef ESP32
  // try lfs first
  ffsp = &FILE_SYSTEM;
 if (!FILE_SYSTEM.begin(true, "") && !FILE_SYSTEM.begin(true, "", 5, "fs_1")) {         // force empty mount point to make it the fallback FS
    // ffat is second
    ffsp = &FFat;
   if (!FFat.begin(true, "")) {
      ffsp = nullptr;
      return;
    }
    ffs_type = UFS_TFAT;
    ufs_type = ffs_type;
    ufsp = ffsp;
    dfsp = ffsp;
    return;
  }
#endif // ESP32
  ffs_type = UFS_TLFS;
  ufs_type = ffs_type;
  ufsp = ffsp;
  dfsp = ffsp;
}



uint32_t mFileSystem::UfsInfo(uint32_t sel, uint32_t type) {
  uint64_t result = 0;
  FS *ifsp = ufsp;
  uint8_t itype = ufs_type;
  if (type) {
    ifsp = ffsp;
    itype = ffs_type;
  }

#ifdef ESP8266
  FSInfo64 fsinfo;
#endif  // ESP8266

  switch (itype) {
    case UFS_TSDC:
#ifdef USE_SDCARD
#ifdef ESP8266
      ifsp->info64(fsinfo);
      if (sel == 0) {
        result = fsinfo.totalBytes;
      } else {
        result = (fsinfo.totalBytes - fsinfo.usedBytes);
      }
#endif  // ESP8266
#ifdef ESP32
      if (sel == 0) {
        result = SD.totalBytes();
      } else {
        result = (SD.totalBytes() - SD.usedBytes());
      }
#endif  // ESP32
#endif  // USE_SDCARD
      break;

    case UFS_TLFS:
#ifdef ESP8266
      ifsp->info64(fsinfo);
      if (sel == 0) {
        result = fsinfo.totalBytes;
      } else {
        result = (fsinfo.totalBytes - fsinfo.usedBytes);
      }
#endif  // ESP8266
#ifdef ESP32
      if (sel == 0) {
        result = LITTLEFS.totalBytes();
      } else {
        result = LITTLEFS.totalBytes() - LITTLEFS.usedBytes();
      }
#endif  // ESP32
      break;

    case UFS_TFAT:
#ifdef ESP32
      if (sel == 0) {
        result = FFat.totalBytes();
      } else {
        result = FFat.freeBytes();
      }
#endif  // ESP32
      break;

  }
  return result / 1024;
}


/*********************************************************************************************\
 * Tfs low level functions
\*********************************************************************************************/

bool mFileSystem::TfsFileExists(const char *fname)
{
  if (!ffs_type) { return false; }

  bool yes = ffsp->exists(fname);
  if (!yes) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TFS: File '%s' not found"), fname +1);  // Skip leading slash
  }
  return yes;
}


size_t mFileSystem::TfsFileSize(const char *fname)
{
  if (!ffs_type) { return 0; }

  File file = ffsp->open(fname, "r");
  if (!file) { return 0; }
  size_t flen = file.size();
  file.close();
  return flen;
}


bool mFileSystem::TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len) 
{
  
  if (!ffs_type) { return false; }
  
#ifdef USE_WEBCAM
  WcInterrupt(0);  // Stop stream if active to fix TG1WDT_SYS_RESET
#endif
  bool result = false;
  
  File file = ffsp->open(fname, "w");
  
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Save failed"));
  } else {
    // This will timeout on ESP32-webcam
    // But now solved with WcInterrupt(0) in support_esp.ino
  DEBUG_LINE_HERE
    file.write(buf, len);
  DEBUG_LINE_HERE
  /*
    // This will still timeout on ESP32-webcam when wcresolution 10
    uint32_t count = len / 512;
    uint32_t chunk = len / count;
    for (uint32_t i = 0; i < count; i++) {
      file.write(buf + (i * chunk), chunk);
      // do actually wait a little to allow ESP32 tasks to tick
      // fixes task timeout in ESP32Solo1 style unicore code and webcam.
      delay(10);
      OsWatchLoop();
    }
    uint32_t left = len % count;
    if (left) {
      file.write(buf + (count * chunk), left);
    }
  */
    file.close();
  DEBUG_LINE_HERE
    result = true;
  }
#ifdef USE_WEBCAM
  WcInterrupt(1);
#endif
  DEBUG_LINE_HERE
  return result;
}

bool mFileSystem::TfsInitFile(const char *fname, uint32_t len, uint8_t init_value) {
  if (!ffs_type) { return false; }

  File file = ffsp->open(fname, "w");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Erase failed"));
    return false;
  }

  for (uint32_t i = 0; i < len; i++) {
    file.write(&init_value, 1);
  }
  file.close();
  return true;
}

bool mFileSystem::TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len) {
  if (!ffs_type) { return false; }

  File file = ffsp->open(fname, "r");
  if (!file) {
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("TFS: File '%s' not found"), fname +1);  // Skip leading slash
    return false;
  }

  size_t flen = file.size();
  if (len > flen) { len = flen; }           // Adjust requested length to smaller file length
  file.read(buf, len);
  file.close();
  return true;
}

String mFileSystem::TfsLoadString(const char *fname) {
  // Use a reasonable amount of stack space considering 4k/8k available on ESP8266/ESP32 and manageable string length
  char buf[2048] = { 0 };                   // Prepare empty string of max 2047 characters on stack
  TfsLoadFile(fname, (uint8_t*)buf, 2047);  // Leave last position as end of string ('\0')
  return String(buf);                       // Received string or empty on error
}

bool mFileSystem::TfsDeleteFile(const char *fname) {
  if (!ffs_type) { return false; }

  if (!ffsp->remove(fname)) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Delete failed"));
    return false;
  }
  return true;
}

bool mFileSystem::TfsRenameFile(const char *fname1, const char *fname2) {
  if (!ffs_type) { return false; }

  if (!ffsp->rename(fname1, fname2)) {
    AddLog(LOG_LEVEL_INFO, PSTR("TFS: Rename failed"));
    return false;
  }
  return true;
}


void mFileSystem::Pre_Init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

    #ifdef USE_SDCARD
    UfsCheckSDCardInit();
    #endif // USE_SDCARD
}


uint8_t mFileSystem::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  
  JBI->Start();
    JBI->Add_P("test",0);  
  JBI->End();

}

#ifdef USE_MODULE_NETWORK_MQTT

void mFileSystem::MQTTHandler_Init(){

  struct handler<mFileSystem>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mFileSystem::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mFileSystem::MQTTHandler_Rate()
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
void mFileSystem::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT



void mFileSystem::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];


  if(jtok = obj["Debug"].getObject()["TriggerJSONSave"]){

    // CommandSet_SerialPrint_FileNames(jtok.getStr());
    // listDir(FILE_SYSTEM, "/", 0);

    JsonFile_Save__Stored_Module();
    
  }
  

  if(jtok = obj["Debug"].getObject()["TriggerJSONLoad"]){

    // CommandSet_SerialPrint_FileNames(jtok.getStr());
    // listDir(FILE_SYSTEM, "/", 0);

    JsonFile_Load__Stored_Module();
    
  }


  if(jtok = obj["ListDir"]){

    // CommandSet_SerialPrint_FileNames(jtok.getStr());
    listDir(FILE_SYSTEM, "/", 0);
    
  }


  if(jtok = obj["ReadFile"]){

    CommandSet_ReadFile(jtok.getStr());
    
  }
  

}


void mFileSystem::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  
  Serial.printf("Listing directory: %s\n", dirname);

  #ifdef ESP32
  File root = fs.open(dirname);
  if(!root){
      Serial.println("Failed to open directory");
      return;
  }
  if(!root.isDirectory()){
      Serial.println("Not a directory");
      return;
  }

  File file = root.openNextFile();
  while(file){
      if(file.isDirectory()){
          Serial.print("  DIR : ");
          Serial.println(file.name());
          if(levels){
              listDir(fs, file.name(), levels -1);
          }
      } else {
          Serial.print("  FILE: ");
          Serial.print(file.name());
          Serial.print("  SIZE: ");
          Serial.println(file.size());
      }
      file = root.openNextFile();
  }
  #endif 

}


void mFileSystem::readFile(fs::FS &fs, const char * path)
{
    
  Serial.printf("Reading file: %s\n\r", path);

  #ifdef ESP32
  File file = fs.open(path);
  if(!file){
      Serial.println("Failed to open file for reading");
      return;
  }

  Serial.print("Read from file: \n\r");
  while(file.available()){
      Serial.write(file.read());
  }
  Serial.println();
  #endif

}


void mFileSystem::CommandSet_ReadFile(const char* filename){

  readFile(FILE_SYSTEM, filename);

  ALOG_COM(PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ReadFile")), filename);

} 


#endif // USE_MODULE_CORE_FILESYSTEM
