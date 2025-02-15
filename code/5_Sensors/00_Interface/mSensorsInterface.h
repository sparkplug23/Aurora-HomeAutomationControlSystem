#ifndef _MODULE_SENSORS_INTERFACE_H
#define _MODULE_SENSORS_INTERFACE_H

#define D_UNIQUE_MODULE_SENSORS_INTERFACE_ID   5000 // [(Folder_Number*100)+ID_File]  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_SENSORS_INTERFACE_ID    0    // Numerical accesending order of module within a group


#define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

#include "stdint.h"

typedef struct event_motion_s{
  uint8_t  state     = false;
  uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  bool isActiveLow = true;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  uint32_t detected_time;
  struct devove_name_t{
    uint16_t unique_module_id = 0;  // unique, not "em_##_id"
    uint8_t device_id = 0;
  }device_name;
}event_motion_t;

typedef struct sensorset_location_s
{   
  bool isvalid = false;
  float latitude = 54.5;// Latitude
  float longitude = -6.0;// Longitude
  float speed = 0;// Speed Over Ground. Unit is knots.
  float altitude = 0;// MSL Altitude. Unit is meters
  int vsat = 0;// GNSS Satellites in View
  int usat = 0;// GNSS Satellites Used
  float accuracy = 0;// Horizontal Dilution Of Precision
  int year = 0; // Four digit year
  int month = 0;// Two digit month
  int day = 0;// Two digit day
  int hour = 0;// Two digit hour
  int minute = 0;// Two digit minute
  int second = 0;// 6 digit second with subseconds
}sensorset_location_t;

#include "1_TaskerManager/mTaskerManager.h"

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR)     "unified/heatmap";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR)                 "unified";
  

#ifdef USE_MODULE_SENSORS_INTERFACE

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR) "motion_event";

#ifdef USE_MODULE_LIGHTS_INTERFACE
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#endif // USE_MODULE_LIGHTS_INTERFACE

class mSensorsInterface :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mSensorsInterface(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_INTERFACE_CTR = D_MODULE_SENSORS_INTERFACE_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_INTERFACE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_INTERFACE_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot      
      uint8_t tTicker_Splash_Sensors_To_Logs = 30;
      float sealevel_pressure; 
    }rt;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    uint32_t GetColourValueUsingMaps_FullBrightness(float value, uint8_t map_style_id = 0, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    uint32_t GetColourValueUsingMaps_AdjustedBrightness(float value, uint8_t map_style_id, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    void HsbToRgb(float h, float s, float v, uint8_t* r8, uint8_t* g8, uint8_t* b8);
    #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

    void EveryLoop();

    void MQTT_Report_Event_Button();

    #ifdef USE_MODULE_LIGHTS_INTERFACE
    uint8_t flag_unified_sensor_colour_heatmap_type = 0;
    RgbwwColor GetColourValueUsingMaps_ForUnifiedSensor(float temperature);
    #endif // USE_MODULE_LIGHTS_INTERFACE
        
    void CommandEvent_Motion(uint8_t event_type);

    float ConvertTemp(float c);
    char TempUnit(void);
    float ConvertPressure(float p);
    String PressureUnit(void);
    
    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    
    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
   
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_SensorTemperatureColours(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Motion_Event(uint8_t json_level = 0, bool json_appending = true);
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
      
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mSensorsInterface>*> mqtthandler_list;
    struct handler<mSensorsInterface> mqtthandler_settings;
    struct handler<mSensorsInterface> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsInterface> mqtthandler_sensor_teleperiod;
    struct handler<mSensorsInterface> mqtthandler_sensor_temperature_colours;
    struct handler<mSensorsInterface> mqtthandler_motion_event_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

    /******************************************************************************************************************
     * WEBSERVER
    *******************************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    #ifdef ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS

    These would be good just on the interface files, to enable HTTP requests for data overview

      /**
       * @brief MQTTHandler_AddWebURL_PayloadRequests
       * */
      void MQTTHandler_AddWebURL_PayloadRequests();

    #endif // ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
    #endif // USE_MODULE_NETWORK_WEBSERVER




};

#endif

#endif
