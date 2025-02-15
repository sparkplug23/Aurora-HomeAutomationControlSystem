#ifndef _MODULE_SENSORS_PIR_H
#define _MODULE_SENSORS_PIR_H

#define D_UNIQUE_MODULE_SENSORS_PIR_ID   5007  // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_PIR

// #include "2_CoreSystem/07_Time/mTime.h"
// #include "5_Sensors/00_Interface/mSensorsInterface.h"
// #include "2_CoreSystem/05_HardwarePins/gpio_types.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mPIR :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mPIR(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_PIR_CTR = D_MODULE_SENSORS_PIR_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_PIR_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_PIR_ID; }
   
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    #ifndef MAX_SENSOR_PIR_DEVICES
    #define MAX_SENSOR_PIR_DEVICES 3
    #endif

    // Dynamic to reduce memory
    std::vector<event_motion_t> pir_detect;

    void ReadSensor();

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
    std::vector<struct handler<mPIR>*> mqtthandler_list;
    struct handler<mPIR> mqtthandler_settings;
    struct handler<mPIR> mqtthandler_sensor_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif

#endif
