#include "mSunTracking.h"

#ifdef USE_MODULE_SENSORS_SUN_TRACKING

int8_t mSunTracking::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:{


    }break;
    case TASK_EVERY_FIVE_SECOND:{


		Update_Solar_Tracking_Data();


        #ifdef ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY
            double latitude = LATITUDE;
            double longitude = LONGITUDE;

            time_t current_time = tkr_time->UtcTime();  // Get current time (UTC)
            time_t start_of_day = tkr_time->GetStartOfDayUTC(current_time);

            std::cout << "Current time: " << CTimeFormat(current_time).c_str();
            std::cout << "Start of the day (midnight): " << CTimeFormat(start_of_day).c_str();
            Serial.flush();


            time_t utc_start_time = start_of_day;
            
            uint16_t interval_minutes = 60; // 1 per hour
            std::vector<SunPosition> accurate_positions;

            if (!accurate_positions.empty()) {
                PrintSunPositions("Accurate Sun Positions", accurate_positions.data(), accurate_positions.size());
            }


            CalculateSunPositionOverDay(accurate_positions, latitude, longitude, utc_start_time, ALTITUDE_ABOVE_SEALEVEL, 60);
            if (!accurate_positions.empty()) {
                PrintSunPositions("Accurate Sun Positions", accurate_positions.data(), accurate_positions.size());
            }


            std::vector<SunPosition> approx_positions; 
            
            if (!approx_positions.empty()) {
                PrintSunPositions("Approximate Sun Positions", approx_positions.data(), approx_positions.size());
            }

            ApproximateSunPositionOverDay(approx_positions, latitude, longitude, utc_start_time, ALTITUDE_ABOVE_SEALEVEL, 60);
            
            if (!approx_positions.empty()) {
                PrintSunPositions("Approximate Sun Positions", approx_positions.data(), approx_positions.size());
            }
        #endif // ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

    }
    break;   
    case TASK_EVERY_MIDNIGHT:
        // Takes 144ms
        CalculateMaxMinElevationForDay(LATITUDE, LONGITUDE, tkr_time->GetUTCTime(), ALTITUDE_ABOVE_SEALEVEL); //will only need to run once a day
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
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

}


void mSunTracking::Pre_Init()
{

    module_state.mode = ModuleStatus::Initialising;

}


void mSunTracking::Init()
{

    memset(&calc, 0, sizeof(calc));
    module_state.mode = ModuleStatus::Running;

}


void mSunTracking::BootMessage()
{
    char buffer[100];
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
        mSupport::appendToBuffer(buffer, sizeof(buffer), "Angles, ");    
    #endif
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
        mSupport::appendToBuffer(buffer, sizeof(buffer), "Advanced, Times");
    #else
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
            mSupport::appendToBuffer(buffer, sizeof(buffer), "Today, ");
        #endif
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
            mSupport::appendToBuffer(buffer, sizeof(buffer), "Tomorrow, ");
        #endif
    #endif
    mSupport::removeTrailingComma(buffer);
    ALOG_IMP(PSTR(D_LOG_SUN_TRACKING "%s"), buffer);
}

void mSunTracking::Update_Solar_Tracking_Data()
{

	if(!tkr_time->RtcTime.valid){
		return; // Cant compute, invalid data
	}

	float altitude = ALTITUDE_ABOVE_SEALEVEL;
	float latitude = tkr_set->Settings.sensors.latitude;
	float longitude = tkr_set->Settings.sensors.longitude;
    time_t utc_time  = tkr_time->UtcTime();
    
    // ALOG_INF(PSTR("utc_time %d"), utc_time);

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
    // Now you have both today's and tomorrow's solar event times stored in structs
    calc.position = CalculateSolarAzEl(utc_time, latitude, longitude, altitude);
    calc.is_daytime = calc.position.elevation > 0 ? true : false;
    if(calc.max_elevation == 0){ // Not set, run here once
        CalculateMaxMinElevationForDay(latitude, longitude, utc_time, altitude); //will only need to run once a day
    }
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__ANGLES


    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
    // Get today's solar event times
    // ALOG_HGL(PSTR("TODAY"));
    // Get the start of today (midnight UTC)
    time_t start_of_today_utc = tkr_time->GetStartOfDayUTC(utc_time);
    calc.today = CalculateSolarEventTimes_Day(latitude, longitude, start_of_today_utc, altitude, tkr_time->IsDst());
    // Update is_sun_rising based on the current time
    calc.is_sun_rising = IsSunRising(utc_time);
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY


    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
    calc.zenith = CalculateSolarZenith();
    calc.irradiance = CalculateSolarIrradiance();
    calc.air_mass = CalculateAirMass();
    calc.shadow_length = CalculateShadowLength();
    calc.solar_time_based_on_longitude = CalculateSolarTime(longitude, utc_time);
    calc.declination_angle = CalculateDeclinationAngle( tkr_time->RtcTime.day_of_year );
    calc.incidence_angle = CalculateSolarIncidenceAngle(0,0);
    calc.day_length = CalculateDayLength(latitude, tkr_time->RtcTime.day_of_year);  
    // Get tomorrow's solar event times (utc_time + 86400 seconds for 24 hours)
    // ALOG_HGL(PSTR("TOMORROW"));
    // Get the start of tomorrow by adding 24 hours (86400 seconds)
    time_t start_of_tomorrow_utc = start_of_today_utc + 86400;
    // calc.tomorrow = CalculateSolarEventTimes_Day(latitude, longitude, start_of_tomorrow_utc, altitude, tkr_time->IsDst());
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

    calc.isvalid = true;
	calc.tUpdated_millis = millis();

}


#if defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY) || defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL)    
// Returns the next occurring dawn based on the current UTC time
time_t mSunTracking::GetNext_Dawn(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow) {
    return (utc_time < today.dawn) ? today.dawn : tomorrow.dawn;
}

// Returns the next occurring sunrise based on the current UTC time
time_t mSunTracking::GetNext_Sunrise(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow) {
    return (utc_time < today.sunrise) ? today.sunrise : tomorrow.sunrise;
}


// Returns the next occurring solar noon based on the current UTC time
time_t mSunTracking::GetNext_SolarNoon(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow) {
    return (utc_time < today.solar_noon) ? today.solar_noon : tomorrow.solar_noon;
}

// Returns the next occurring sunset based on the current UTC time
time_t mSunTracking::GetNext_Sunset(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow) {
    return (utc_time < today.sunset) ? today.sunset : tomorrow.sunset;
}

// Returns the next occurring dusk based on the current UTC time
time_t mSunTracking::GetNext_Dusk(time_t utc_time, const SolarDayTimes& today, const SolarDayTimes& tomorrow) {
    return (utc_time < today.dusk) ? today.dusk : tomorrow.dusk;
}

// Returns the daylight duration for today (in seconds)
double mSunTracking::Get_Daylight_Duration(const SolarDayTimes& today) {
    return today.daylight_duration;  // Already in seconds
}


// Returns the difference in daylight duration between today and tomorrow in seconds
double mSunTracking::Get_Daylight_Duration_Difference(const SolarDayTimes& today, const SolarDayTimes& tomorrow) {
    double difference = tomorrow.daylight_duration - today.daylight_duration;

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Daylight Duration Today: " << today.daylight_duration << " seconds\n";
    std::cout << "Daylight Duration Tomorrow: " << tomorrow.daylight_duration << " seconds\n";
    std::cout << "Difference in Daylight Duration: " << difference << " seconds\n";
    #endif

    return difference;  // Positive if tomorrow is longer, negative if shorter
}
#endif // #if defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY) || defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL)
    


constexpr double DEG_TO_RAD_CONST = M_PI / 180.0;
constexpr double RAD_TO_DEG_CONST = 180.0 / M_PI;
constexpr double AU = 149598000;  // Astronomical Unit in kilometers

#ifdef USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

double mSunTracking::CalculateSolarZenith() {
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Calculating Solar Zenith...\n";
    #endif

    double zenith = 90.0 - Get_Elevation();  // Zenith angle in degrees
    
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Zenith angle: " << zenith << " degrees\n";
    #endif

    return zenith;
}

double mSunTracking::CalculateSolarIrradiance() {
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Calculating Solar Irradiance...\n";
    #endif

    if (Get_Elevation() <= 0) {
        #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
        std::cout << "Sun below horizon, irradiance = 0 W/m²\n";
        #endif
        return 0;  // No irradiance when the sun is below the horizon
    }

    double air_mass_ratio = CalculateAirMass();
    double irradiance = SOLAR_CONSTANT / air_mass_ratio;

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Irradiance: " << irradiance << " W/m²\n";
    #endif

    return irradiance;
}

double mSunTracking::CalculateAirMass() {
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Calculating Air Mass...\n";
    #endif

    double zenith = CalculateSolarZenith();
    if (zenith > 90) {
        #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
        std::cout << "Zenith angle > 90 degrees, no air mass\n";
        #endif
        return 0;  // No air mass if the sun is below the horizon
    }

    double air_mass = 1.0 / (cos(zenith * DEG_TO_RAD_CONST) + 0.50572 * pow((96.07995 - zenith), -1.6364));

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Air Mass: " << air_mass << "\n";
    #endif

    return air_mass;
}


float mSunTracking::CalculateShadowLength() {
    if (!calc.is_daytime) {
        return INFINITY;  // No shadow at night
    }

    double elevation = calc.position.elevation;

    if (elevation <= 0.0) {
        return INFINITY;  // No shadow if the sun is below the horizon
    }

    return 1.0 / tan(elevation * DEG_TO_RAD_CONST);  // Example formula for shadow length
}




// Function to calculate Solar Time
double mSunTracking::CalculateSolarTime(double longitude, time_t utc_time) {
    // Convert UTC time to solar time based on longitude
    double solar_time = (utc_time / 3600.0) + (longitude / 15.0);  // Divide by 3600 to get hours
    
    // Apply the equation of time if precision is needed (not implemented in this function for simplicity)

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Time: " << solar_time << " hours\n";
    #endif
    return solar_time;
}


// Function to calculate Solar Declination Angle
double mSunTracking::CalculateDeclinationAngle(int day_of_year) {
    double declination_angle = 23.45 * sin(DEG_TO_RAD * (360.0 / 365.0 * (day_of_year - 81)));
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Declination Angle: " << declination_angle << " degrees\n";
    #endif
    return declination_angle;
}

// Function to calculate Solar Incidence Angle
double mSunTracking::CalculateSolarIncidenceAngle(double surface_tilt, double surface_azimuth) {
    double zenith = CalculateSolarZenith();
    double incidence_angle = acos(sin(zenith * DEG_TO_RAD) * cos(surface_tilt * DEG_TO_RAD) +
                                  cos(zenith * DEG_TO_RAD) * sin(surface_tilt * DEG_TO_RAD) * cos(surface_azimuth - calc.today.solar_noon * DEG_TO_RAD)) * RAD_TO_DEG;
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Incidence Angle: " << incidence_angle << " degrees\n";
    #endif
    return incidence_angle;
}

// Function to calculate Day Length
double mSunTracking::CalculateDayLength(double latitude, int day_of_year) {
    double declination = CalculateDeclinationAngle(day_of_year);
    double hour_angle = acos(-tan(latitude * DEG_TO_RAD) * tan(declination * DEG_TO_RAD)) * RAD_TO_DEG;
    double day_length = (2.0 * hour_angle) / 15.0;  // Convert hour angle to hours
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Day Length: " << day_length << " hours\n";
    #endif
    return day_length;
}

#endif // USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED



#ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY

double mSunTracking::j2ts(double j) {
    return (j - 2440587.5) * 86400.0;
}

/**
 * @brief This function is much simpler and is designed to quickly convert a Unix timestamp (seconds since January 1, 1970) to a Julian Date.
Method:

    It divides the Unix timestamp by the number of seconds in a day (86,400) to convert the timestamp into days.
    Then it adds the Julian Date equivalent of the Unix epoch (January 1, 1970), which is 2440587.5, to obtain the final Julian Day.

Use Case: This is a more straightforward conversion, where the input is a Unix timestamp and you don't need the detailed breakdown of the date. It's faster and simpler but assumes the timestamp is based on the Unix epoch.
 * 
 * @param ts 
 * @return double 
 */
double mSunTracking::ts2j(double ts) {
    return ts / 86400.0 + 2440587.5;
}

double mSunTracking::calculateM(double J_) {
    double M_degrees = fmod(357.5291 + 0.98560028 * J_, 360.0);
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Mean Anomaly (M_degrees): " << M_degrees << " degrees\n";
    #endif
    return M_degrees;
}

double mSunTracking::calculateC(double M_radians) {
    double C_degrees = 1.9148 * sin(M_radians) + 0.02 * sin(2 * M_radians) + 0.0003 * sin(3 * M_radians);
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Equation of Center (C_degrees): " << C_degrees << " degrees\n";
    #endif
    return C_degrees;
}

double mSunTracking::calculateL(double M_degrees, double C_degrees) {
    double L_degrees = fmod(M_degrees + C_degrees + 180.0 + 102.9372, 360.0);
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Ecliptic Longitude (L_degrees): " << L_degrees << " degrees\n";
    #endif
    return L_degrees;
}

double mSunTracking::calculateJtransit(double J_, double M_radians, double Lambda_radians) {
    double J_transit = 2451545.0 + J_ + 0.0053 * sin(M_radians) - 0.0069 * sin(2 * Lambda_radians);
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Transit Time (J_transit): " << J_transit << " Julian days\n";
    #endif
    return J_transit;
}

double mSunTracking::calculateDeclination(double Lambda_radians) {
    double sin_d = sin(Lambda_radians) * sin(23.4397 * DEG_TO_RAD_CONST);
    double delta = asin(sin_d) * RAD_TO_DEG_CONST;
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Declination (delta): " << delta << " degrees\n";
    #endif
    return delta;
}

double mSunTracking::calculateHourAngle(double latitude, double delta, double elevation, double twilight_angle) {
    // Adjusted angle to account for twilight, where -0.833 is for sunrise/sunset and -6 is for civil twilight
    double angle = twilight_angle * DEG_TO_RAD_CONST;

    double some_cos = (sin(angle - 2.076 * sqrt(elevation) / 60.0 * DEG_TO_RAD_CONST) - sin(latitude * DEG_TO_RAD_CONST) * sin(delta * DEG_TO_RAD_CONST)) / (cos(latitude * DEG_TO_RAD_CONST) * cos(delta * DEG_TO_RAD_CONST));
    
    // Clamp the cosine value to prevent errors
    if (some_cos < -1.0) some_cos = -1.0;
    if (some_cos > 1.0) some_cos = 1.0;

    double w0_radians = acos(some_cos);
    double w0_degrees = w0_radians * RAD_TO_DEG_CONST;

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Hour Angle (w0_degrees): " << w0_degrees << " degrees\n";
    #endif

    return w0_degrees;
}


// Function to calculate the solar event times for a specific day (using UTC timestamp)
mSunTracking::SolarDayTimes mSunTracking::CalculateSolarEventTimes_Day(double latitude, double longitude, time_t utc_time, double height_above_sealevel, bool daylight_savings_active) {
    mSunTracking::SolarDayTimes result;

    // Debugging: Start of function
    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "----- Calculating Solar Event Times for UTC: " << std::ctime(&utc_time) << "-----\n";
    std::cout << "Current Unix Time: " << utc_time << " seconds since epoch\n";
    #endif

    // Convert the current UTC time into Julian Date
    double julian_date = ts2j(utc_time);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Julian Date: " << julian_date << " Julian days\n";
    #endif

    // Calculate days since J2000.0
    double days_since_j2000 = ceil(julian_date - (2451545.0 + 0.0009) + longitude / 360.0);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Days since J2000.0: " << days_since_j2000 << " days\n";
    #endif

    // Calculate solar time, mean anomaly, equation of center, true longitude, and solar transit
    double mean_solar_time = days_since_j2000 + 0.0009 - longitude / 360.0;
    double solar_mean_anomaly_degrees = calculateM(mean_solar_time);
    double solar_mean_anomaly_radians = solar_mean_anomaly_degrees * DEG_TO_RAD_CONST;
    double equation_of_center_degrees = calculateC(solar_mean_anomaly_radians);
    double solar_true_longitude_degrees = calculateL(solar_mean_anomaly_degrees, equation_of_center_degrees);
    double solar_true_longitude_radians = solar_true_longitude_degrees * DEG_TO_RAD_CONST;
    double solar_transit_julian = calculateJtransit(mean_solar_time, solar_mean_anomaly_radians, solar_true_longitude_radians);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Mean Anomaly (degrees): " << solar_mean_anomaly_degrees << "\n";
    std::cout << "Equation of Center (degrees): " << equation_of_center_degrees << "\n";
    std::cout << "True Solar Longitude (degrees): " << solar_true_longitude_degrees << "\n";
    std::cout << "Solar Transit (Julian): " << solar_transit_julian << " Julian days\n";
    #endif

    // Calculate solar declination
    double solar_declination = calculateDeclination(solar_true_longitude_radians);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Solar Declination (degrees): " << solar_declination << "\n";
    #endif

    // Calculate hour angle for sunrise/sunset
    double hour_angle_sunrise_degrees = calculateHourAngle(latitude, solar_declination, height_above_sealevel);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Hour Angle for Sunrise/Sunset (degrees): " << hour_angle_sunrise_degrees << "\n";
    #endif

    // Calculate Julian dates for sunrise, sunset, and solar noon
    double sunrise_julian_today = solar_transit_julian - hour_angle_sunrise_degrees / 360.0;
    double sunset_julian_today = solar_transit_julian + hour_angle_sunrise_degrees / 360.0;

    // Convert to Unix timestamps for the struct
    result.sunrise = j2ts(sunrise_julian_today);
    result.sunset = j2ts(sunset_julian_today);
    result.solar_noon = j2ts(solar_transit_julian);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Sunrise (Unix time): " << result.sunrise << " (" << std::ctime(&result.sunrise) << ")\n";
    std::cout << "Sunset (Unix time): " << result.sunset << " (" << std::ctime(&result.sunset) << ")\n";
    std::cout << "Solar Noon (Unix time): " << result.solar_noon << " (" << std::ctime(&result.solar_noon) << ")\n";
    #endif

    // Calculate the hour angles for dawn and dusk (-6 degrees for civil twilight)
    double twilight_angle = 6.0;  // Civil twilight

    // Dusk calculation: Dusk occurs when the Sun is 6 degrees below the horizon AFTER sunset
    double hour_angle_dusk_degrees = calculateHourAngle(latitude, solar_declination, height_above_sealevel, -twilight_angle);
    double dusk_julian_today = solar_transit_julian + hour_angle_dusk_degrees / 360.0;

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Hour Angle for Dusk (degrees): " << hour_angle_dusk_degrees << "\n";
    std::cout << "Julian Date for Dusk: " << dusk_julian_today << "\n";
    #endif

    // Dawn calculation: Dawn occurs when the Sun is 6 degrees below the horizon BEFORE sunrise
    double hour_angle_dawn_degrees = calculateHourAngle(latitude, solar_declination, height_above_sealevel, -twilight_angle);
    double dawn_julian_today = solar_transit_julian - hour_angle_dawn_degrees / 360.0;

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Hour Angle for Dawn (degrees): " << hour_angle_dawn_degrees << "\n";
    std::cout << "Julian Date for Dawn: " << dawn_julian_today << "\n";
    #endif

    // Convert dawn and dusk to Unix timestamps for the struct
    result.dawn = j2ts(dawn_julian_today);
    result.dusk = j2ts(dusk_julian_today);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Dawn (Unix time): " << result.dawn << " (" << std::ctime(&result.dawn) << ")\n";
    std::cout << "Dusk (Unix time): " << result.dusk << " (" << std::ctime(&result.dusk) << ")\n";
    #endif

    // Adjust for daylight savings if active
    if (daylight_savings_active) {
        result.sunrise += 3600;
        result.sunset += 3600;
        result.solar_noon += 3600;
        result.dawn += 3600;
        result.dusk += 3600;

        #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
        std::cout << "Daylight Savings Active - Times Adjusted by +1 hour\n";
        #endif
    }

    // Calculate daylight duration in seconds
    result.daylight_duration = result.sunset - result.sunrise;

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Daylight Duration (seconds): " << result.daylight_duration << "\n";
    std::cout << "Daylight Duration (hours): " << result.daylight_duration / 3600.0 << " hours\n";
    #endif

    // Logging the final solar event times
    time_t ts_today_dawn = static_cast<time_t>(result.dawn);
    time_t ts_today_sunrise = static_cast<time_t>(result.sunrise);
    time_t ts_today_sunset = static_cast<time_t>(result.sunset);
    time_t ts_today_dusk = static_cast<time_t>(result.dusk);

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "----- Today's Solar Event Times (with DST) -----\n";
    std::cout << "Dawn: " << ts_today_dawn << " (" << std::ctime(&ts_today_dawn) << ")\n";
    std::cout << "Sunrise: " << ts_today_sunrise << " (" << std::ctime(&ts_today_sunrise) << ")\n";
    std::cout << "Sunset: " << ts_today_sunset << " (" << std::ctime(&ts_today_sunset) << ")\n";
    std::cout << "Dusk: " << ts_today_dusk << " (" << std::ctime(&ts_today_dusk) << ")\n";
    #endif

    return result;
}

bool mSunTracking::IsDaytime(time_t utc_time) {
    // Check if the current time is between sunrise and sunset
    return (utc_time >= calc.today.sunrise && utc_time <= calc.today.sunset);
}
 
bool mSunTracking::IsSunRising(time_t utc_time) {
    // If the current time is after dawn but before solar noon, the sun is rising
    if (utc_time >= calc.today.dawn && utc_time < calc.today.solar_noon) {
        return true;
    }
    return false;
}

#endif // USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY



#ifdef ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY


// void mSunTracking::CalculateSunPositionOverDay(std::vector<SunPosition>& sun_positions, double Lat, double Lon, time_t start_time, double Alt, int interval_minutes) {
//     // Preallocate space for the expected number of positions
//     int num_positions = (24 * 60) / interval_minutes; 
//     ALOG_INF(PSTR("num_pos %d"), num_positions); Serial.flush();
//     sun_positions.resize(num_positions);  // Resize to the number of positions

//     int count = 0;
//     for (int i = 0; i < 24 * 60; i += interval_minutes) {
//         time_t current_time = start_time + i * 60;  // Increment by interval_minutes
        
//         SunPosition position;
//         position = CalculateSolarAzEl(current_time, Lat, Lon, Alt);
//         position.time = current_time;

//         // Instead of using push_back, assign directly to the preallocated vector
//         sun_positions[count++] = position;
//     }
// }



// void mSunTracking::ApproximateSunPositionOverDay(std::vector<SunPosition>& sun_positions, double Lat, double Lon, time_t start_time, double Alt, int interval_minutes) {
//     // Clear the vector if it has previous data
//     sun_positions.clear();

//     double meanAz = 180.0;  // Approximate midday azimuth (for example)
//     double meanEl = 45.0;   // Approximate midday elevation
//     int total_positions = (24 * 60) / interval_minutes;

//     // Reserve space for the expected number of positions
//     sun_positions.reserve(total_positions);  

//     for (int i = 0; i < 24 * 60; i += interval_minutes) {
//         time_t current_time = start_time + i * 60;  // Increment by interval_minutes
//         double hour_angle = (i / 60.0 - 12.0) * 15.0;  // 15 degrees per hour

//         // Simple sinusoidal approximation
//         double Az = meanAz + 90.0 * sin(hour_angle * M_PI / 180.0);
//         double El = meanEl * cos(hour_angle * M_PI / 180.0);

//         // Push back new SunPosition into the vector
//         sun_positions.push_back({current_time, Az, El});
//     }
// }

void mSunTracking::CalculateSunPositionOverDay(std::vector<SunPosition>& sun_positions, double Lat, double Lon, time_t start_time, double Alt, int interval_minutes) {
    sun_positions.clear();  // Clear existing entries
    for (int i = 0; i < 24 * 60; i += interval_minutes) {
        time_t current_time = start_time + i * 60;  // Increment by interval_minutes

        SunPosition position;
        position = CalculateSolarAzEl(current_time, Lat, Lon, Alt);
        position.time = current_time;
        sun_positions.push_back(position);  // Add to the vector
    }
}

void mSunTracking::ApproximateSunPositionOverDay(std::vector<SunPosition>& sun_positions, double Lat, double Lon, time_t start_time, double Alt, int interval_minutes) {
    sun_positions.clear();  // Clear existing entries
    double meanAz = 180.0;  // Approximate midday azimuth (for example)
    double meanEl = 45.0;   // Approximate midday elevation

    for (int i = 0; i < 24 * 60; i += interval_minutes) {
        time_t current_time = start_time + i * 60;  // Increment by interval_minutes
        double hour_angle = (i / 60.0 - 12.0) * 15.0;  // 15 degrees per hour

        // Simple sinusoidal approximation
        double Az = meanAz + 90.0 * sin(hour_angle * M_PI / 180.0);
        double El = meanEl * cos(hour_angle * M_PI / 180.0);

        SunPosition position{current_time, Az, El};
        sun_positions.push_back(position);  // Add to the vector
    }
}

#endif // ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

#ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
void mSunTracking::PrintSunPositions(const char* label, SunPosition* sun_positions, int count) {
    std::cout << label << "\n";
    for (int i = 0; i < count; ++i) {
        std::cout << "Time: " << CTimeFormat(sun_positions[i].time).c_str()
                  << " Azimuth: " << sun_positions[i].azimuth 
                  << " Elevation: " << sun_positions[i].elevation << "\n";
    }
}
#endif // ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS



#ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
/**
 * @brief 
 * 
 *
 * Year and Month Adjustment:
 *     If the month is January or February (month <= 2), the year is adjusted by subtracting 1, and the month is adjusted by adding 12. This is because the calculation treats March as the start of the year.
 * Julian Day Calculation:
 *     The formula adds up contributions from the year, month, day, and time of day, applying necessary corrections for the Gregorian calendar.
 *     The result is the Julian Day number that corresponds to the input utc_time_point.
 * 
 * This function converts a given time_t UTC time (which represents seconds since the Unix epoch) into a Julian Day number by extracting the year, month, day, hour, minute, and second components of the UTC time and applying a detailed formula.
Method:

    It manually converts the broken-down time (year, month, day, etc.) into a Julian Day using specific rules for adjusting January and February.
    It applies corrections for the Gregorian calendar and leap year rules, making this a more detailed and precise approach for calculating the Julian Date.
    The formula is complex, taking into account many factors like fractional days (hours, minutes, and seconds).

Use Case: It's useful when you need to work directly with date components and ensure precise conversion across different calendar systems.
 * @param utc_time_point 
 * @return double 
 */
double mSunTracking::julian_day(time_t utc_time_point) 
{
    // Convert UTC time to a struct tm for extracting time components
    struct tm* tm = gmtime(&utc_time_point); 

    // Extract time components
    double year = tm->tm_year + 1900;  // tm_year is years since 1900
    double month = tm->tm_mon + 1;     // tm_mon is months since January (0-11)
    double day = tm->tm_mday;
    double hour = tm->tm_hour;
    double min = tm->tm_min;
    double sec = tm->tm_sec;

    // Adjust year and month for January and February
    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    // Calculate the Julian Day
    double jd = 
        floor(365.25 * (year + 4716.0))    // Days from the year
        + floor(30.6001 * (month + 1.0))   // Days from the month
        + day                              // Add the day of the month
        + (hour + min / 60.0 + sec / 3600.0) / 24.0 // Fraction of the day from the time
        - 1524.5                           // Offset to align with Julian calendar system
        + 2.0                              // Correction for Gregorian calendar
        - floor(year / 100.0)              // Subtract leap year rule corrections
        + floor(floor(year / 100.0) / 4.0);

    return jd;
}

mSunTracking::SunPosition mSunTracking::CalculateSolarAzEl(time_t utc_time, double latitude, double longitude, double altitude) {
    // Step 1: Calculate the Julian day from UTC time
    double jd = julian_day(utc_time); // Replace with your own Julian day calculation function
    double days_since_j2000 = jd - 2451543.5;  // Days since J2000 epoch

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS_LOOPS
    std::cout << "Julian Day: " << jd << " Days since J2000: " << days_since_j2000 << "\n";
    #endif

    // Step 2: Keplerian elements for the Sun
    double w = 282.9404 + 4.70935e-5 * days_since_j2000;  // Longitude of perihelion (degrees)
    double e = 0.016709 - 1.151e-9 * days_since_j2000;    // Eccentricity of Earth's orbit
    double M = fmod(356.0470 + 0.9856002585 * days_since_j2000, 360.0);  // Mean anomaly (degrees)

    // Step 3: Sun's mean longitude
    double L = w + M;

    // Step 4: Obliquity of the ecliptic
    double obliquity = 23.4393 - 3.563e-7 * days_since_j2000;

    // Step 5: Solve Kepler's Equation (Eccentric Anomaly)
    double E = M + (180 / M_PI) * e * sin(M * DEG_TO_RAD) * (1 + e * cos(M * DEG_TO_RAD));

    // Step 6: Rectangular coordinates in the plane of the ecliptic
    double x_eclip = cos(E * DEG_TO_RAD) - e;
    double y_eclip = sin(E * DEG_TO_RAD) * sqrt(1 - e * e);

    // Step 7: Distance and true anomaly
    double distance = sqrt(x_eclip * x_eclip + y_eclip * y_eclip);  // Distance in AU
    double true_anomaly = atan2(y_eclip, x_eclip) * RAD_TO_DEG;

    // Step 8: Solar coordinates (Sun's longitude)
    double solar_longitude = true_anomaly + w;

    // Step 9: Convert ecliptic coordinates to equatorial coordinates
    double x_equat = distance * cos(solar_longitude * DEG_TO_RAD);
    double y_equat = distance * sin(solar_longitude * DEG_TO_RAD) * cos(obliquity * DEG_TO_RAD);
    double z_equat = distance * sin(solar_longitude * DEG_TO_RAD) * sin(obliquity * DEG_TO_RAD);

    // Step 10: Right Ascension and Declination
    double r = sqrt(x_equat * x_equat + y_equat * y_equat + z_equat * z_equat);
    double RA = atan2(y_equat, x_equat) * RAD_TO_DEG;
    double dec = asin(z_equat / r) * RAD_TO_DEG;  // Declination

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS_LOOPS
    std::cout << "Right Ascension: " << RA << " Declination: " << dec << "\n";
    #endif

    // Step 11: Calculate local sidereal time
    tm* ptm = gmtime(&utc_time);  // Get UTC time components
    double UTH = ptm->tm_hour + ptm->tm_min / 60.0 + ptm->tm_sec / 3600.0;
    double GMST0 = fmod(L + 180, 360.0) / 15;  // Greenwich Mean Sidereal Time at 0h UT
    double local_sidereal_time = GMST0 + UTH + longitude / 15;

    // Step 12: Hour Angle
    double HA = local_sidereal_time * 15 - RA;

    // Step 13: Convert to rectangular coordinate system
    double x_hor = cos(HA * DEG_TO_RAD) * cos(dec * DEG_TO_RAD);
    double y_hor = sin(HA * DEG_TO_RAD) * cos(dec * DEG_TO_RAD);
    double z_hor = sin(dec * DEG_TO_RAD);

    // Step 14: Rotate coordinates for observer's latitude
    double x_rotated = x_hor * cos((90 - latitude) * DEG_TO_RAD) - z_hor * sin((90 - latitude) * DEG_TO_RAD);
    double y_rotated = y_hor;
    double z_rotated = x_hor * sin((90 - latitude) * DEG_TO_RAD) + z_hor * cos((90 - latitude) * DEG_TO_RAD);

    // Step 15: Calculate Azimuth and Elevation
    SunPosition AzEl;
    AzEl.azimuth = atan2(y_rotated, x_rotated) * RAD_TO_DEG + 180.0;  // Azimuth (degrees)
    AzEl.elevation = asin(z_rotated) * RAD_TO_DEG;  // Elevation (degrees)

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS_LOOPS
    std::cout << "Azimuth: " << AzEl.azimuth << " degrees, Elevation: " << AzEl.elevation << " degrees\n";
    #endif

    return AzEl;
}


/*

How it Works:

    Coarse Calculation: The code first calculates the solar position every hour and finds the hours in which the maximum and minimum elevations occur.
    Fine Calculation: After identifying the hours with the max and min elevations, the code refines the calculation by checking the elevation at every minute within those hours to determine the exact max and min.

Performance Benefit:

    Instead of calculating solar positions for all 1440 minutes in a day, this approach only calculates solar positions for 24 hours (coarse) + 120 minutes (fine), which is 144 calculations in total — a significant reduction compared to 1440 calculations.


*/
void mSunTracking::CalculateMaxMinElevationForDay(time_t utc_time, double latitude, double longitude, double altitude) {
    // Reset the max and min elevation for a new calculation
    calc.max_elevation = -90.0;  // Start with the lowest possible elevation
    calc.min_elevation = 90.0;   // Start with the highest possible elevation

    SunPosition AzEl;
    int max_elevation_hour = 0;
    int min_elevation_hour = 0;
    const int coarse_interval_minutes = 60;  // Coarse step: every hour
    const int fine_interval_minutes = 1;     // Fine step: every minute

    // Coarse calculation: Calculate solar position every hour to identify potential max/min hours
    for (int hour_offset = 0; hour_offset < 24; ++hour_offset) {
        time_t current_time = utc_time + (hour_offset * 3600);  // Offset in seconds (1 hour intervals)

        // Calculate the elevation at this time
        AzEl = CalculateSolarAzEl(current_time, latitude, longitude, altitude);

        // Check if we have a new max or min elevation
        if (AzEl.elevation > calc.max_elevation) {
            calc.max_elevation = AzEl.elevation;
            max_elevation_hour = hour_offset;
        }

        if (AzEl.elevation < calc.min_elevation) {
            calc.min_elevation = AzEl.elevation;
            min_elevation_hour = hour_offset;
        }
    }

    // Fine calculation: Calculate solar position per minute within the max and min elevation hours
    // Fine calculation for max elevation hour
    for (int minutes_offset = 0; minutes_offset < 60; ++minutes_offset) {
        time_t current_time = utc_time + (max_elevation_hour * 3600) + (minutes_offset * 60);  // Per minute in the max elevation hour

        AzEl = CalculateSolarAzEl(current_time, latitude, longitude, altitude);
        if (AzEl.elevation > calc.max_elevation) {
            calc.max_elevation = AzEl.elevation;
        }
    }

    // Fine calculation for min elevation hour
    for (int minutes_offset = 0; minutes_offset < 60; ++minutes_offset) {
        time_t current_time = utc_time + (min_elevation_hour * 3600) + (minutes_offset * 60);  // Per minute in the min elevation hour

        AzEl = CalculateSolarAzEl(current_time, latitude, longitude, altitude);
        if (AzEl.elevation < calc.min_elevation) {
            calc.min_elevation = AzEl.elevation;
        }
    }

    #ifdef ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS
    std::cout << "Final Max Elevation Today: " << calc.max_elevation << " degrees\n";
    std::cout << "Final Min Elevation Today: " << calc.min_elevation << " degrees\n";
    #endif
}

#endif // USE_MODULE_SENSORS_SUN_TRACKING__ANGLES


void mSunTracking::parse_JSONCommand(JsonParserObject obj)
{

	
  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  if(jtok = obj["SunAngles"].getObject()["Enabled"])
  {
    debug.enabled = jtok.getBool();
  }

  if(jtok = obj["SunAngles"].getObject()["Elevation"])
  {
    debug.elevation = jtok.getFloat();
  }
  if(jtok = obj["SunAngles"].getObject()["ElevationMin"])
  {
    debug.min_elevation = jtok.getFloat();
  }
  if(jtok = obj["SunAngles"].getObject()["ElevationMax"])
  {
    debug.max_elevation = jtok.getFloat();
  }

  if(jtok = obj["SunAngles"].getObject()["Azimuth"])
  {
    debug.azimuth = jtok.getFloat();
  }
  #endif // USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  
}


time_t CalculateTimeUntilEvent(time_t current_time, time_t event_time) {
    if (event_time > current_time) {
        return event_time - current_time;
    } else {
        // If the event already passed today, calculate for the next day
        return (event_time + 86400) - current_time; // 86400 seconds = 1 day
    }
}

String FormatTime(time_t time) {
    String time_str(ctime(&time));  // Get the time string
    time_str.trim();                // Trim newline characters
    return time_str;
}



uint8_t mSunTracking::ConstructJSON_Sensor(uint8_t json_method, bool json_appending)
{

  JBI->Start();

  if(calc.isvalid) 
  {
    time_t current_time = time(nullptr);

    
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
    JBI->Object_Start("Debug");      
        JBI->Add("Enabled", (uint8_t)debug.enabled);
        JBI->Add("Elevation", (float)debug.elevation);
        JBI->Add("ElevationMin", (float)debug.min_elevation);
        JBI->Add("ElevationMax", (float)debug.max_elevation);
        JBI->Add("Azimuth", (float)debug.azimuth);
    JBI->Object_End();
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
    JBI->Add(PM__IS_SUN_RISING, (uint8_t)calc.is_sun_rising);
    #endif
   
   #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
    JBI->Object_Start(PM__ANGLES);    
        JBI->Add(PM__AZIMUTH, (float)calc.position.azimuth);
        JBI->Add(PM__ELEVATION, (float)calc.position.elevation);
        JBI->Add(PM__ELEVATION_MAX, calc.max_elevation);
        JBI->Add(PM__ELEVATION_MIN, calc.min_elevation);
    JBI->Object_End();
    #endif

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
    JBI->Add(PM__ZENITH, (float)calc.zenith);
    JBI->Add(PM__AIR_MASS, (float)calc.air_mass);
    JBI->Add(PM__SOLAR_IRRADIANCE, (float)calc.irradiance);
    JBI->Add(PM__DECLINATION_ANGLE, (float)calc.declination_angle);
    JBI->Add(PM__DAY_LENGTH, (float)calc.day_length);

    if (!calc.is_daytime) {
        JBI->Add(PM__SHADOW_LENGTH, PM__NONE);
    } else {
        if (isinf(CalculateShadowLength())) {
            JBI->Add(PM__SHADOW_LENGTH, PM__INF);
        } else {
            calc.shadow_length = CalculateShadowLength();
            JBI->Add(PM__SHADOW_LENGTH, calc.shadow_length);
        }
    }
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX
    if(json_method == JSON_LEVEL_DETAILED)
    {
        JBI->Object_Start(PM__UNIX_EVENTS);
            JBI->Add(PM__DAWN, (float)calc.today.dawn);
            JBI->Add(PM__SUNRISE, (float)calc.today.sunrise);
            JBI->Add(PM__SOLAR_NOON, (float)calc.today.solar_noon);
            JBI->Add(PM__SUNSET, (float)calc.today.sunset);
            JBI->Add(PM__DUSK, (float)calc.today.dusk);
        JBI->Object_End();
    }
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX

    #if defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY) || defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL)  
    JBI->Add(PM__DST_ACTIVE, (uint8_t)calc.daylight_savings_active);
    JBI->Object_Start(PM__TODAY);
        JBI->Object_Start(PM__DATE_TIME_EVENTS);
            JBI->Add(PM__DAWN, FormatTime(static_cast<time_t>(calc.today.dawn)).c_str());
            JBI->Add(PM__SUNRISE, FormatTime(static_cast<time_t>(calc.today.sunrise)).c_str());
            JBI->Add(PM__SOLAR_NOON, FormatTime(static_cast<time_t>(calc.today.solar_noon)).c_str());
            JBI->Add(PM__SUNSET, FormatTime(static_cast<time_t>(calc.today.sunset)).c_str());
            JBI->Add(PM__DUSK, FormatTime(static_cast<time_t>(calc.today.dusk)).c_str());
            JBI->Add(PM__DAYLIGHT_DURATION, (float)calc.today.daylight_duration);
        JBI->Object_End();
        #ifdef ENABLE_MQTT_REPORTING__SUN_TRACKING__SECONDS_UNTIL
        JBI->Object_Start(PM__SECONDS_UNTIL);
            JBI->Add(PM__DAWN, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.dawn)));
            JBI->Add(PM__SUNRISE, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.sunrise)));
            JBI->Add(PM__SOLAR_NOON, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.solar_noon)));
            JBI->Add(PM__SUNSET, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.sunset)));
            JBI->Add(PM__DUSK, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.dusk)));
        JBI->Object_End();
        #endif // ENABLE_MQTT_REPORTING__SUN_TRACKING__SECONDS_UNTIL
        JBI->Object_Start(PM__TIME_UNTIL);
            JBI->Add(PM__DAWN, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.dawn))).c_str());
            JBI->Add(PM__SUNRISE, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.sunrise))).c_str());
            JBI->Add(PM__SOLAR_NOON, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.solar_noon))).c_str());
            JBI->Add(PM__SUNSET, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.sunset))).c_str());
            JBI->Add(PM__DUSK, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.today.dusk))).c_str());
        JBI->Object_End();
    JBI->Object_End();
    #endif // defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY) || defined(USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL)
      
    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
    JBI->Object_Start(PM__TOMORROW);
        JBI->Object_Start(PM__DATE_TIME_EVENTS);
            JBI->Add(PM__DAWN, FormatTime(static_cast<time_t>(calc.tomorrow.dawn)).c_str());
            JBI->Add(PM__SUNRISE, FormatTime(static_cast<time_t>(calc.tomorrow.sunrise)).c_str());
            JBI->Add(PM__SOLAR_NOON, FormatTime(static_cast<time_t>(calc.tomorrow.solar_noon)).c_str());
            JBI->Add(PM__SUNSET, FormatTime(static_cast<time_t>(calc.tomorrow.sunset)).c_str());
            JBI->Add(PM__DUSK, FormatTime(static_cast<time_t>(calc.tomorrow.dusk)).c_str());
        JBI->Object_End();
        #ifdef ENABLE_MQTT_REPORTING__SUN_TRACKING__SECONDS_UNTIL
        JBI->Object_Start(PM__SECONDS_UNTIL);
            JBI->Add(PM__DAWN, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.dawn)));
            JBI->Add(PM__SUNRISE, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.sunrise)));
            JBI->Add(PM__SOLAR_NOON, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.solar_noon)));
            JBI->Add(PM__SUNSET, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.sunset)));
            JBI->Add(PM__DUSK, CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.dusk)));
        JBI->Object_End();
        #endif // ENABLE_MQTT_REPORTING__SUN_TRACKING__SECONDS_UNTIL
        JBI->Object_Start(PM__TIME_UNTIL);
            JBI->Add(PM__DAWN, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.dawn))).c_str());
            JBI->Add(PM__SUNRISE, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.sunrise))).c_str());
            JBI->Add(PM__SOLAR_NOON, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.solar_noon))).c_str());
            JBI->Add(PM__SUNSET, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.sunset))).c_str());
            JBI->Add(PM__DUSK, tkr_time->formatTimeUntil(CalculateTimeUntilEvent(current_time, static_cast<time_t>(calc.tomorrow.dusk))).c_str());
        JBI->Object_End();
    JBI->Object_End();
    #endif // USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL

    #ifdef USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
    JBI->Object_Start(PM__NEXT);
        time_t utc_time = tkr_time->UtcTime();   
        JBI->Add(PM__DAWN, FormatTime(GetNext_Dawn(utc_time, calc.today, calc.tomorrow)).c_str());
        JBI->Add(PM__SUNRISE, FormatTime(GetNext_Sunrise(utc_time, calc.today, calc.tomorrow)).c_str());
        JBI->Add(PM__SOLAR_NOON, FormatTime(GetNext_SolarNoon(utc_time, calc.today, calc.tomorrow)).c_str());
        JBI->Add(PM__SUNSET, FormatTime(GetNext_Sunset(utc_time, calc.today, calc.tomorrow)).c_str());
        JBI->Add(PM__DUSK, FormatTime(GetNext_Dusk(utc_time, calc.today, calc.tomorrow)).c_str());
        JBI->Add(PM__DAYLIGHT_DURATION, (float)Get_Daylight_Duration(calc.today));
        JBI->Add(PM__DAYLIGHT_DURATION_DIFF, (float)Get_Daylight_Duration_Difference(calc.today, calc.tomorrow));
    JBI->Object_End();
    #endif

  }

  return JBI->End();
}


uint8_t mSunTracking::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  char buffer[20];

  JBI->Start();
	JBI->Add("mSunTracking",0);
  return JBI->End();

}



#ifdef USE_MODULE_NETWORK_MQTT

void mSunTracking::MQTTHandler_Init()
{

  struct handler<mSunTracking>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSunTracking::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSunTracking::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSunTracking::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT



#endif // USE_MODULE_SENSORS_SUN_TRACKING
