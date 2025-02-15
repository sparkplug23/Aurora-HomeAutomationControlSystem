#ifndef MODULE_SENSORS_SUN_TRACKING_H
#define MODULE_SENSORS_SUN_TRACKING_H

#define D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_ID 5022 // [(Folder_Number*100)+ID_File]

/**
 * Three levels of sun tracking for memory efficiency
 * 
 * USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
 * USE_MODULE_SENSORS_SUN_TRACKING__SUNRISE_SUNSET
 * USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
 * 
 */

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SUN_TRACKING
#include "3_Network/10_MQTT/mMQTT.h"

#include <stdio.h>      /* printf */
#include <math.h>       /* fmod */
  
#include <cmath>
#include <ctime>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "1_TaskerManager/mTaskerInterface.h"

class mSunTracking :
  public mTaskerInterface
{
  
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSunTracking(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();

    static constexpr const char* PM_MODULE_SENSORS_SUN_TRACKING_CTR = D_MODULE_SENSORS_SUN_TRACKING_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SUN_TRACKING_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_ID; }
  

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    #if defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY) || defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL)
    struct SolarDayTimes {
        time_t dawn;
        time_t sunrise;
        time_t solar_noon;
        time_t sunset;
        time_t dusk;
        double daylight_duration;  // In seconds
    };
    #endif
    
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
    struct SunPosition {
      time_t time; // Time of the position
      double azimuth;
      double elevation;      
    };
    #endif

    struct SolarEventTimes 
    {
      bool is_daytime;
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
      bool is_sun_rising; 
      bool daylight_savings_active;
      #endif

      #if defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY) || defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL)
      SolarDayTimes today;
      #endif
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
      SolarDayTimes tomorrow;
      #endif 
      
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
      SunPosition position;
      double max_elevation;  // Maximum elevation of the sun for the day
      double min_elevation;  // Minimum elevation of the sun for the day (typically at dawn or dusk)
      #endif 

      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
      double solar_time_based_on_longitude;
      double shadow_length;
      double day_length;
      double zenith;
      double irradiance;
      double air_mass;
      double declination_angle;
      float incidence_angle;
      #endif

      uint32_t tUpdated_millis = 0;
      bool isvalid = false;
    }calc;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void Update_Solar_Tracking_Data();

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
    double j2ts(double j);
    double ts2j(double ts);
    double calculateM(double J_);
    double calculateC(double M_radians);
    double calculateL(double M_degrees, double C_degrees);
    double calculateJtransit(double J_, double M_radians, double Lambda_radians);
    double calculateDeclination(double Lambda_radians);
    double calculateHourAngle(double latitude, double delta, double elevation = 0.0, double twilight_angle = -0.833);
    SolarDayTimes CalculateSolarEventTimes_Day(double latitude, double longitude, time_t utc_time, double height_above_sealevel, bool daylight_savings_active) ;
        
    // Converts a time_t value to a trimmed String
    String TimeToString(time_t t) {
        String timeStr = String(ctime(&t));  // Convert to a String
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    String Get_DawnTime() const {
        time_t t = calc.today.dawn;
        String timeStr = String(ctime(&t));
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    String Get_SunriseTime() const {
        time_t t = calc.today.sunrise;
        String timeStr = String(ctime(&t));
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    String Get_SolarNoonTime() const {
        time_t t = calc.today.solar_noon;
        String timeStr = String(ctime(&t));
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    String Get_SunsetTime() const {
        time_t t = calc.today.sunset;
        String timeStr = String(ctime(&t));
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    String Get_DuskTime() const {
        time_t t = calc.today.dusk;
        String timeStr = String(ctime(&t));
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    uint32_t Get_DaylightDuration() const {
        return calc.today.daylight_duration; // in seconds
    }
    
    bool IsSunRising(time_t utc_time);
    bool IsDaytime(time_t utc_time);

    #endif // USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY

    
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
    double CalculateSolarZenith();
    double CalculateSolarTime(double longitude, time_t utc_time);
    double CalculateAirMass();
    double CalculateSolarIrradiance();
    double CalculateDeclinationAngle(int day_of_year);
    double CalculateSolarIncidenceAngle(double surface_tilt, double surface_azimuth);
    double CalculateDayLength(double latitude, int day_of_year);
    float CalculateShadowLength();
    #endif
        
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
    time_t GetNext_Dawn(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow);
    time_t GetNext_Sunrise(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow);
    time_t GetNext_SolarNoon(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow);
    time_t GetNext_Sunset(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow);
    time_t GetNext_Dusk(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow);
    double Get_Daylight_Duration(const SolarDayTimes& today);
    double Get_Daylight_Duration_Difference(const SolarDayTimes& today, const SolarDayTimes& tomorrow);
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
    
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
    double julian_day(time_t utc_time_point);
    SunPosition CalculateSolarAzEl(time_t utc_time, double latitude, double longitude, double altitude) ;
    void CalculateMaxMinElevationForDay(time_t utc_time, double latitude, double longitude, double altitude);
    #endif

    #ifdef ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY
    void CalculateSunPositionOverDay(std::vector<SunPosition>& sun_positions, double Lat, double Lon, time_t start_time, double Alt = 0.0, int interval_minutes = 60);
    void ApproximateSunPositionOverDay(std::vector<SunPosition>& sun_positions, double Lat, double Lon, time_t start_time, double Alt, int interval_minutes);
    #endif

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    void PrintSunPositions(const char* label, SunPosition* sun_positions, int count);
    #endif

    String CTimeFormat(time_t t) const {
        String timeStr = String(ctime(&t));
        timeStr.trim();  // Remove any trailing newline or spaces
        return timeStr;
    }

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
    struct DEBUG
    {
      bool enabled = false;
      double azimuth = 0;
      double elevation = 0;
      double max_elevation = 0;  // Maximum elevation of the sun for the day
      double min_elevation = 0;  // Minimum elevation of the sun for the day (typically at dawn or dusk)
    }debug;
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING

    double Get_Azimuth()
    {
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
        if(debug.enabled)
          return debug.azimuth;
        else
          return calc.position.azimuth;
      #else
        return calc.position.azimuth;
      #endif
    }

    double Get_Elevation()
    {
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
        if(debug.enabled)
          return debug.elevation;
        else
          return calc.position.elevation;
      #else
        return calc.position.elevation;
      #endif
    }

    double Get_Elevation_Max()
    {
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
        if(debug.enabled)
          return debug.max_elevation;
        else
          return calc.max_elevation;
      #else
        return calc.max_elevation;
      #endif
    }

    double Get_Elevation_Min()
    {
      #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
        if(debug.enabled)
          return debug.min_elevation;
        else
          return calc.min_elevation;
      #else
        return calc.pmin_elevation;
      #endif
    }

    bool Valid()
    {
      return calc.isvalid;
    }

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/

    uint8_t GetSensorCount(void) override
    {
      return 1;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      value->timestamp = 0; // Constantly updated, so timestamp is not required. Assume "0" from now on means reading can be skipped as timeout
      value->sensor_type.push_back(SENSOR_TYPE_SUN_AZIMUTH_ID);
      value->data_f.push_back((float)Get_Azimuth());
      value->sensor_type.push_back(SENSOR_TYPE_SUN_ELEVATION_ID);
      value->data_f.push_back((float)Get_Elevation());
      value->sensor_id = index;
    };
    

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
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    std::vector<struct handler<mSunTracking>*> mqtthandler_list;    
    struct handler<mSunTracking> mqtthandler_settings;
    struct handler<mSunTracking> mqtthandler_sensor_ifchanged;
    struct handler<mSunTracking> mqtthandler_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT 

  private: 

    const double SOLAR_CONSTANT = 1361.0; // W/m^2


};

#endif // USE_MODULE_SENSORS_SUN_TRACKING

#endif // HEADER GUARD
