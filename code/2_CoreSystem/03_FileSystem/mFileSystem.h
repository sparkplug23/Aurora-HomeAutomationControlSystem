#ifndef _USE_MODULE_CORE_FILESYSTEM_H
#define _USE_MODULE_CORE_FILESYSTEM_H 0.3

#define D_UNIQUE_MODULE_CORE_FILESYSTEM_ID 2003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_FILESYSTEM


#include <string.h>
#include <strings.h>


#ifdef ESP8266
  #include <LittleFS.h>
  #include <SPI.h>
  #ifdef USE_SDCARD
    #include <SD.h>
    #include <SDFAT.h>
  #endif  // USE_SDCARD
#endif  // ESP8266
#ifdef ESP32
  #include <LittleFS.h>
  #ifdef USE_SDCARD
    #include <SD.h>
  #endif  // USE_SDCARD
  #include "FFat.h"
  #include "FS.h"
#endif  // ESP32

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <SPIFFSEditor.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif
#ifdef ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define ARDUINOJSON_DECODE_UNICODE 0
  #include "3_Network/21_WebServer/ArduinoJson-v6.h"
#endif // ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

// ESP32-WROVER features SPI RAM (aka PSRAM) which can be allocated using ps_malloc()
// we can create custom PSRAMDynamicJsonDocument to use such feature (replacing DynamicJsonDocument)
// The following is a construct to enable code to compile without it.
// There is a code that will still not use PSRAM though:
//    AsyncJsonResponse is a derived class that implements DynamicJsonDocument (AsyncJson-v6.h)
#if defined(ARDUINO_ARCH_ESP32)
extern bool psramSafe;
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (psramSafe && psramFound()) return ps_malloc(size); // use PSRAM if it exists
    else                           return malloc(size);    // fallback
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (psramSafe && psramFound()) return ps_realloc(ptr, new_size); // use PSRAM if it exists
    else                           return realloc(ptr, new_size);    // fallback
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#else
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif


#define FILE_EXTENSION_JSON ".json"

// #define FILE_EXTENSION_BIN ".bin" //release version
#define FILE_EXTENSION_BIN ".txt"   //debug version so the editor can open the file


/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       12
 *    D3       13
 *    CMD      15
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      14
 *    VSS      GND
 *    D0       2  (add 1K pull up after flashing)
 *    D1       4
 */

// #include "FS.h"
// #include "SD_MMC.h"


#include "1_TaskerManager/mTaskerInterface.h"

class mFileSystem :
  public mTaskerInterface
{

  public:
    mFileSystem(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    int8_t Tasker_Web(uint8_t function);

    void FileWrite_Test();
    
    void JsonFile_Save__Stored_Module();
    bool JsonFile_Load__Stored_Module();
    void JsonFile_Load__Stored_Module_Or_Default_Template();

    void JsonFile_Save__Stored_Secure();
    void JsonFile_Load__Stored_Secure();    

    static constexpr const char* PM_MODULE_CORE_FILESYSTEM_CTR = D_MODULE_CORE_FILESYSTEM_CTR;
    PGM_P GetModuleName(){         return PM_MODULE_CORE_FILESYSTEM_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_FILESYSTEM_ID; }

    void ByteFile_Save(char* filename_With_extension, uint8_t* buffer, uint16_t buflen);
    uint32_t ByteFile_Load(char* filename_With_extension, uint8_t* buffer, uint16_t buflen);
    void JSONFile_Save(char* filename_With_extension, char* buffer, uint16_t buflen);
    void JSONFile_Load(char* filename_With_extension, char* buffer, uint16_t buflen);

    void SystemTask__Execute_Module_Data_Save();

    
    bool psramSafe = true;         // is it safe to use PSRAM (on ESP32 rev.1; compiler fix used "-mfix-esp32-psram-cache-issue")


    /*********************************************************************************************\
    This driver adds universal file system support for
    - ESP8266 (sd card or littlefs on  > 1 M devices with special linker file e.g. eagle.flash.4m2m.ld)
      (makes no sense on 1M devices without sd card)
    - ESP32 (sd card or littlefs or sfatfile system).

    The sd card chip select is the standard SDCARD_CS or when not found SDCARD_CS_PIN and initializes
    the FS System Pointer ufsp which can be used by all standard file system calls.

    The only specific call is UfsInfo() which gets the total size (0) and free size (1).

    A button is created in the setup section to show up the file directory to download and upload files
    subdirectories are supported.

    Supported commands:
    ufs       fs info
    ufstype   get filesytem type 0=none 1=SD  2=Flashfile
    ufssize   total size in kB
    ufsfree   free size in kB
    \*********************************************************************************************/

    #define UFS_TNONE         0
    #define UFS_TSDC          1
    #define UFS_TFAT          2
    #define UFS_TLFS          3

    // Global file system pointer
    FS *ufsp = nullptr;
    // Flash file system pointer
    FS *ffsp = nullptr;
    // Local pointer for file managment
    FS *dfsp = nullptr;

    char ufs_path[48];
    File ufs_upload_file;
    uint8_t ufs_dir;
    // 0 = None, 1 = SD, 2 = ffat, 3 = littlefs
    uint8_t ufs_type;
    uint8_t ffs_type;

    struct {
      char run_file[48];
      int run_file_pos = -1;
      bool run_file_mutex = 0;
      bool download_busy;
    } UfsData;

    void UfsInitOnce(void);
    void UfsInit(void);

    uint16_t test_val = 0;
    uint8_t dir = 0;

    void init();
    void Pre_Init();
    int8_t pin = -1;
    struct SETTINGS{
      uint8_t fEnableModule = false;
      uint8_t fShowManualSlider = false;
    }settings;


    char *folderOnly(char *fname);
    char *fileOnly(char *fname);
    void UfsCheckSDCardInit(void);
    uint32_t UfsInfo(uint32_t sel, uint32_t type);
    uint32_t UfsSize(void);
    uint32_t UfsFree(void);
    uint8_t UfsReject(char *name);


    bool TfsFileExists(const char *fname);
    size_t TfsFileSize(const char *fname);
    bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len);
    bool TfsInitFile(const char *fname, uint32_t len, uint8_t init_value);
    bool TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len);
    String TfsLoadString(const char *fname);
    bool TfsDeleteFile(const char *fname);
    bool TfsRenameFile(const char *fname1, const char *fname2);


    bool UfsExecuteCommandFileReady(void);
    void UfsExecuteCommandFileLoop(void);
    bool UfsExecuteCommandFile(const char *fname);
    char* UfsFilename(char* fname, char* fname_in);
    void UFSInfo(void);
    void UFSType(void);
    void UFSSize(void);
    void UFSFree(void);
    void UFSDelete(void);
    void UFSRename(void);

    // General filesystem
    size_t fsBytesUsed =0;
    size_t fsBytesTotal =0;
    unsigned long presetsModifiedTime =0L;
    JsonDocument* fileDoc;
    bool doCloseFile =false;
    byte errorFlag = 0;


    void closeFile();
    bool bufferedFind(const char *target, bool fromStart = true);
    bool bufferedFindSpace(size_t targetLen, bool fromStart = true);
    bool bufferedFindObjectEnd() ;
    void writeSpace(size_t l);
    bool appendObjectToFile(const char* key, JsonDocument* content, uint32_t s, uint32_t contentLen = 0);
    bool writeObjectToFileUsingId(const char* file, uint16_t id, JsonDocument* content);
    bool writeObjectToFile(const char* file, const char* key, JsonDocument* content);
    bool readObjectFromFileUsingId(const char* file, uint16_t id, JsonDocument* dest);
    bool readObjectFromFile(const char* file, const char* key, JsonDocument* dest);
    void updateFSInfo();

    #ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI
    String getContentType(AsyncWebServerRequest* request, String filename);
    bool handleFileRead(AsyncWebServerRequest* request, String path);
    #endif // ENABLE_WEBSERVER_LIGHTING_WEBUI


    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void createDir(fs::FS &fs, const char * path);
    void removeDir(fs::FS &fs, const char * path);
    void readFile(fs::FS &fs, const char * path);
    void writeFile(fs::FS &fs, const char * path, const char * message);
    void appendFile(fs::FS &fs, const char * path, const char * message);
    void renameFile(fs::FS &fs, const char * path1, const char * path2);
    void deleteFile(fs::FS &fs, const char * path);
    void testFileIO(fs::FS &fs, const char * path);


    void CommandSet_CreateFile_WithName(char* value);
    void CommandSet_SerialPrint_FileNames(const char* value);
    void CommandSet_WriteFile(const char* filename, const char* data = nullptr);
        
    void CommandSet_ReadFile(const char* filename);



    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Scene(uint8_t json_level, bool json_appending);

    void WebCommand_Parse(void);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mFileSystem>*> mqtthandler_list;    
    struct handler<mFileSystem> mqtthandler_settings;    
    struct handler<mFileSystem> mqtthandler_sensor_ifchanged;
    struct handler<mFileSystem> mqtthandler_sensor_teleperiod;    
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif