#ifndef _mLDRBasic_H_
#define _mLDRBasic_H_

#define D_UNIQUE_MODULE_SENSORS_LDR_BASIC_ID 5014 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_LDR_BASIC

#include <Ticker.h>

#include "2_CoreSystem/mBaseConfig.h"

#include "2_CoreSystem/07_Time/mTime.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "2_CoreSystem/07_Time/mTime.h"


#define MAX_LDR_BASIC 2             // Max number of switches

#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mLDRBasic :
  public mTaskerInterface
{
  public:
    mLDRBasic(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    void Pre_Init(void);
    void Init(void);

    static constexpr const char* PM_MODULE_SENSORS_LDR_BASIC_CTR = D_MODULE_SENSORS_LDR_BASIC_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_LDR_BASIC_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_LDR_BASIC_ID; }

    void EveryLoop();
    void EverySecond();

    struct SETTINGS{
      bool fEnableSensor = false;
    }settings;

    struct LDR_BASIC{

      uint8_t  digital_reading = 0;
      uint16_t analog_reading = 0;

    }ldr[MAX_LDR_BASIC];

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    struct handler<mLDRBasic> mqtthandler_settings;
    struct handler<mLDRBasic> mqtthandler_sensor_ifchanged;
    struct handler<mLDRBasic> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    

    struct handler<mLDRBasic>* mqtthandler_list[3] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT 


};



#endif

#endif  // _SONOFF_H_
//#endif