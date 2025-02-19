#ifndef MSENSORSDHT_H
#define MSENSORSDHT_H 0.2

#define D_UNIQUE_MODULE_SENSORS_DHT_ID 5005 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_DHT

#include "stdint.h"
// #include "2_CoreSystem/07_Time/mTime.h"
// class mTime;
#include "DHTesp.h"
class DHTesp;
#include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_NETWORK_WEBSERVER
// #include <ESPAsyncTCP.h>
// //?#include <ESPAsyncWebServer.h>
// #endif //USE_MODULE_NETWORK_WEBSERVER

#include "1_TaskerManager/mTaskerInterface.h"

class mSensorsDHT :
  public mTaskerInterface
{

  private:
  public:
    mSensorsDHT(){};
    // Configure pins, call sensor sub classes and init if need be
    void Pre_Init(void);
    // Configure variables
    void Init(void);
    #define MAX_SENSORS 4
    int8_t pin[MAX_SENSORS] = {-1,-1,-1,-1};
    // DHTesp* dht[MAX_SENSORS];
    
    struct SETTINGS{
      uint8_t  fEnableSensor = true;
      uint8_t  sensor_active_count = 0; // count of sensors found    n:number found, c:case number for switches
      uint16_t rate_measure_ms = 1000;
      uint8_t  group_count = 0;
    }settings;


    static constexpr const char* PM_MODULE_SENSORS_DHT_CTR = D_MODULE_SENSORS_DHT_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_DHT_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DHT_ID; }

    uint8_t GetSensorCount(void) override
    {
      return settings.sensor_active_count;
    }
    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      // Serial.printf("OVERRIDE ACCESSED DHT %d\n\r",index);Serial.println(sensor[index].instant.temperature);
      if(index > MAX_SENSORS-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data_f.push_back(sensor[index].instant.temperature);
      value->sensor_type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
      value->data_f.push_back(sensor[index].instant.humidity);
      value->sensor_id = index;
    };



// Phase out
enum SWITCH_SPLIT_TASK_IDS{
  SPLIT_TASK_NOT_RUNNING_ID=0,
  SPLIT_TASK_SUCCESS_ID=1,
  SPLIT_TASK_SEC1_ID,
  SPLIT_TASK_SEC2_ID,
  SPLIT_TASK_SEC3_ID,
  SPLIT_TASK_SEC4_ID,
  SPLIT_TASK_SEC5_ID,
  SPLIT_TASK_SEC6_ID,
  SPLIT_TASK_SEC7_ID,
  SPLIT_TASK_SEC8_ID,
  SPLIT_TASK_TIMEOUT_ID,
  SPLIT_TASK_ERROR_ID,
  SPLIT_TASK_DONE_ID
};

  enum {
    AUTO_DETECT,
    DHT11,
    DHT22,
    AM2302,  // Packaged DHT22
    RHT03    // Equivalent to DHT22
  };

    void ShowSensor_AddLog();

    //#define D_MODULE_TOPIC_NAME "dht"

    // #define WEB_HANLDE_JSON_DHT_SENSOR_TABLE "/fetch/tab_dht_sensor.json"
    void EveryLoop();

    // void WebAppend_Root_Status_Table_Draw();
    // void WebAppend_Root_Status_Table_Data();

    // uint8_t fSensorCount = 0; // 0 also means not enabled

    // sensors_event_t* GetSensorValue();
    
// void GetSensorValue(sensors_reading_t* value);

// struct DHTSTRUCT {
//   uint8_t     pin;
//   uint8_t     type;
//   char     stype[12];
//   uint32_t lastreadtime;
//   uint8_t  lastresult;
//   float    t = NAN;
//   float    h = NAN;
// } Dht[MAX_SENSORS];




    uint8_t WITHINLIMITS(int minv, float var, int maxv);

     void SplitTask_UpdateClimateSensors(uint8_t sensor_id, uint8_t require_completion);;


    // char name_buffer[100]; //use single 1D klist array in memory


     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

         #define DONE_COMPLETE false
         #define DONE_NOTCOMPLETE true
         #define REQUIRE_COMPLETE true
         #define DONTREQUIRE_COMPLETE false
    float tmp_float;

    struct SENSORDATA{
      DHTesp* dht;
      struct TYPE{
        float temperature;
        float humidity;
        float heatIndex;
        float dewPoint;
        float cr;       
        uint8_t isvalid=false;
        uint8_t ischanged=false;
        uint8_t ischanged_over_threshold=false; //phase out and just add changed percentage guards ie change more than 0.1 or its lowest precision
        // time later 
        uint32_t tWithinLimit;
        uint32_t tSavedMeasureClimate;
        uint32_t ischangedtLast = millis();
        uint8_t fWithinLimit;
        uint8_t sUpdateClimateSensors;
      };
      TYPE instant;
      TYPE roc10s; // rate of change
      TYPE roc1m; // rate of change
      TYPE roc10m; // rate of change
      // const char* name_ptr = nullptr;
    }sensor[MAX_SENSORS];

    const char* sensor_unset_ctr = "unset";

    void WebPage_Root_AddHandlers();
    void WebPage_Root_SendStatus();



    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  

    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    struct handler<mSensorsDHT> mqtthandler_settings;
    struct handler<mSensorsDHT> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsDHT> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    
      
    struct handler<mSensorsDHT>* mqtthandler_list[3] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };
};
#endif

#endif
