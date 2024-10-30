#ifndef _DRIVERS__CAMERA_ARDUINO_H1
#define _DRIVERS__CAMERA_ARDUINO_H1

#define D_UNIQUE_MODULE_DRIVERS__CAMERA_ARDUINO__ID 4060 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO

#include "1_TaskerManager/mTaskerInterface.h"


//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15 
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD


#include "esp_camera.h"
#ifndef CAMERA_MODEL_WROVER_KIT
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#endif
#include "camera_pins.h"


class mWebCamera :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mWebCamera(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_DRIVERS__CAMERA_ARDUINO__CTR = D_MODULE_DRIVERS__CAMERA_ARDUINO__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS__CAMERA_ARDUINO__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS__CAMERA_ARDUINO__ID; }    
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
    void EveryLoop();
    void EverySecond();
    
    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mWebCamera>*> mqtthandler_list;    
    struct handler<mWebCamera> mqtthandler_settings;
    struct handler<mWebCamera> mqtthandler_sensor_ifchanged;
    struct handler<mWebCamera> mqtthandler_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif // USE_MODULE_DRIVERS__CAMERA_ARDUINO

#endif // _DRIVERS__CAMERA_ARDUINO_H
