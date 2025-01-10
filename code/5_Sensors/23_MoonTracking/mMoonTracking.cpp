#include "mMoonTracking.h"

#ifdef USE_MODULE_SENSORS_MOON_TRACKING





#include <math.h>
#include <stdio.h>


#include <cmath>
#include <iostream>
#include <iomanip>
#include <ctime>


// const double DEG_TO_RAD = M_PI / 180.0;
// const double RAD_TO_DEG = 180.0 / M_PI;

double j2ts(double j) {
    return (j - 2440587.5) * 86400.0;
}

double ts2j(double ts) {
    return ts / 86400.0 + 2440587.5;
}

double calculateM(double J_) {
    double M_degrees = fmod(357.5291 + 0.98560028 * J_, 360.0);
    std::cout << "Solar Mean Anomaly (M_degrees): " << M_degrees << " degrees\n";
    return M_degrees;
}

double calculateC(double M_radians) {
    double C_degrees = 1.9148 * sin(M_radians) + 0.02 * sin(2 * M_radians) + 0.0003 * sin(3 * M_radians);
    std::cout << "Equation of Center (C_degrees): " << C_degrees << " degrees\n";
    return C_degrees;
}

double calculateL(double M_degrees, double C_degrees) {
    double L_degrees = fmod(M_degrees + C_degrees + 180.0 + 102.9372, 360.0);
    std::cout << "Ecliptic Longitude (L_degrees): " << L_degrees << " degrees\n";
    return L_degrees;
}

double calculateJtransit(double J_, double M_radians, double Lambda_radians) {
    double J_transit = 2451545.0 + J_ + 0.0053 * sin(M_radians) - 0.0069 * sin(2 * Lambda_radians);
    std::cout << "Solar Transit Time (J_transit): " << J_transit << " Julian days\n";
    return J_transit;
}

double calculateDeclination(double Lambda_radians) {
    double sin_d = sin(Lambda_radians) * sin(23.4397 * DEG_TO_RAD);
    double delta = asin(sin_d) * RAD_TO_DEG;
    std::cout << "Solar Declination (delta): " << delta << " degrees\n";
    return delta;
}

double calculateHourAngle(double latitude, double delta, double elevation = 0.0) {
    double some_cos = (sin(-0.833 * DEG_TO_RAD - 2.076 * sqrt(elevation) / 60.0 * DEG_TO_RAD) - sin(latitude * DEG_TO_RAD) * sin(delta * DEG_TO_RAD)) / (cos(latitude * DEG_TO_RAD) * cos(delta * DEG_TO_RAD));
    
    if (some_cos < -1.0) some_cos = -1.0;
    if (some_cos > 1.0) some_cos = 1.0;

    double w0_radians = acos(some_cos);
    double w0_degrees = w0_radians * RAD_TO_DEG;
    std::cout << "Hour Angle (w0_degrees): " << w0_degrees << " degrees\n";
    return w0_degrees;
}

double calculateHourAngle2(double latitude, double delta, double elevation = 0.0, double twilight_angle = -0.833) {
    // Adjusted angle to account for twilight, where -0.833 is for sunrise/sunset and -6 is for civil twilight
    double angle = twilight_angle * DEG_TO_RAD;

    double some_cos = (sin(angle - 2.076 * sqrt(elevation) / 60.0 * DEG_TO_RAD) - sin(latitude * DEG_TO_RAD) * sin(delta * DEG_TO_RAD)) / (cos(latitude * DEG_TO_RAD) * cos(delta * DEG_TO_RAD));
    
    if (some_cos < -1.0) some_cos = -1.0;
    if (some_cos > 1.0) some_cos = 1.0;

    double w0_radians = acos(some_cos);
    double w0_degrees = w0_radians * RAD_TO_DEG;
    std::cout << "Hour Angle (w0_degrees): " << w0_degrees << " degrees\n";
    return w0_degrees;
}


void CalculateSolarTimes2(double latitude, double longitude, time_t utc_time, double elevation = 0.0) {
    std::cout << "----- Debugging Solar Time Calculation2 -----\n\n";

    double J_date = ts2j(utc_time);
    std::cout << "Julian Date: " << J_date << " Julian days\n";

    double n = ceil(J_date - (2451545.0 + 0.0009) + 69.184 / 86400.0);
    std::cout << "Julian Day (n): " << n << " days\n";

    double J_ = n + 0.0009 - longitude / 360.0;
    std::cout << "Mean Solar Time (J_): " << J_ << " Julian days\n";

    double M_degrees = calculateM(J_);
    double M_radians = M_degrees * DEG_TO_RAD;

    double C_degrees = calculateC(M_radians);
    double L_degrees = calculateL(M_degrees, C_degrees);
    double Lambda_radians = L_degrees * DEG_TO_RAD;

    double J_transit = calculateJtransit(J_, M_radians, Lambda_radians);

    double delta = calculateDeclination(Lambda_radians);
    double w0_degrees = calculateHourAngle(latitude, delta, elevation);

    double j_rise = J_transit - w0_degrees / 360.0;
    double j_set = J_transit + w0_degrees / 360.0;

    std::cout << "Sunrise (j_rise): " << j_rise << " Julian days\n";
    std::cout << "Sunset (j_set): " << j_set << " Julian days\n";

    double ts_rise = j2ts(j_rise);
    double ts_set = j2ts(j_set);

    time_t ts_rise_time_t = static_cast<time_t>(ts_rise);
    time_t ts_set_time_t = static_cast<time_t>(ts_set);

    std::cout << "Calculated Sunrise: " << std::ctime(&ts_rise_time_t);
    std::cout << "Calculated Sunset: " << std::ctime(&ts_set_time_t);

    std::cout << "----- End of Debugging Solar Time Calculation -----\n";
}

// void CalculateSolarTimes3(double latitude, double longitude, time_t utc_time, double elevation = 0.0, bool daylight_savings = false) {
//     std::cout << "----- Debugging Solar Time Calculation3 -----\n\n";

//     double J_date = ts2j(utc_time);
//     std::cout << "Julian Date: " << J_date << " Julian days\n";

//     // Use the same method for Julian day (n) calculation as in CalculateSolarTimes2
//     double n = ceil(J_date - (2451545.0 + 0.0009) + longitude / 360.0);
//     std::cout << "Julian Day (n): " << n << " days\n";

//     double J_ = n + 0.0009 - longitude / 360.0;
//     std::cout << "Mean Solar Time (J_): " << J_ << " Julian days\n";

//     double M_degrees = calculateM(J_);
//     double M_radians = M_degrees * DEG_TO_RAD;

//     double C_degrees = calculateC(M_radians);
//     double L_degrees = calculateL(M_degrees, C_degrees);
//     double Lambda_radians = L_degrees * DEG_TO_RAD;

//     double J_transit = calculateJtransit(J_, M_radians, Lambda_radians);

//     double delta = calculateDeclination(Lambda_radians);
//     double w0_degrees = calculateHourAngle(latitude, delta, elevation);

//     double j_rise = J_transit - w0_degrees / 360.0;
//     double j_set = J_transit + w0_degrees / 360.0;

//     std::cout << "Sunrise (j_rise): " << j_rise << " Julian days\n";
//     std::cout << "Sunset (j_set): " << j_set << " Julian days\n";

//     double ts_rise = j2ts(j_rise);
//     double ts_set = j2ts(j_set);

//     time_t ts_rise_time_t = static_cast<time_t>(ts_rise);
//     time_t ts_set_time_t = static_cast<time_t>(ts_set);

//     std::cout << "Calculated Sunrise: " << std::ctime(&ts_rise_time_t);
//     std::cout << "Calculated Sunset: " << std::ctime(&ts_set_time_t);

//     std::cout << "----- End of Debugging Solar Time Calculation -----\n";
// }


void CalculateSolarTimes3(double latitude, double longitude, time_t utc_time, double elevation = 0.0, bool daylight_savings = false) {
    std::cout << "----- Debugging Solar Time Calculation3 -----\n\n";

    double J_date = ts2j(utc_time);
    double n = ceil(J_date - (2451545.0 + 0.0009) + longitude / 360.0);
    double J_ = n + 0.0009 - longitude / 360.0;
    double M_degrees = calculateM(J_);
    double M_radians = M_degrees * DEG_TO_RAD;
    double C_degrees = calculateC(M_radians);
    double L_degrees = calculateL(M_degrees, C_degrees);
    double Lambda_radians = L_degrees * DEG_TO_RAD;
    double J_transit = calculateJtransit(J_, M_radians, Lambda_radians);
    double delta = calculateDeclination(Lambda_radians);
    double w0_degrees = calculateHourAngle(latitude, delta, elevation);

    double j_rise = J_transit - w0_degrees / 360.0;
    double j_set = J_transit + w0_degrees / 360.0;
    double j_noon = J_transit;

    double ts_rise = j2ts(j_rise);
    double ts_set = j2ts(j_set);
    double ts_noon = j2ts(j_noon);

    // Apply daylight savings adjustment
    if (daylight_savings) {
        ts_rise += 3600; // Add one hour
        ts_set += 3600;
        ts_noon += 3600;
    }

    // Check if sunset is after sunrise but on the next day
    if (ts_set < ts_rise) {
        ts_set += 86400; // Adjust to the next day if sunset is earlier than sunrise
    }

    // Calculate dawn and dusk using the -6 degrees (civil twilight) angle
    double twilight_angle = 6.0;
    double ha_dawn_degrees = calculateHourAngle2(latitude, delta, elevation, -twilight_angle);
    double ha_dusk_degrees = calculateHourAngle2(latitude, delta, elevation, twilight_angle);
    double j_dawn = J_transit - ha_dawn_degrees / 360.0;
    double j_dusk = J_transit + ha_dusk_degrees / 360.0;

    double ts_dawn = j2ts(j_dawn);
    double ts_dusk = j2ts(j_dusk);

    if (daylight_savings) {
        ts_dawn += 3600;
        ts_dusk += 3600;
    }

    time_t ts_rise_time_t = static_cast<time_t>(ts_rise);
    time_t ts_set_time_t = static_cast<time_t>(ts_set);
    time_t ts_noon_time_t = static_cast<time_t>(ts_noon);
    time_t ts_dawn_time_t = static_cast<time_t>(ts_dawn);
    time_t ts_dusk_time_t = static_cast<time_t>(ts_dusk);

    std::cout << "Calculated Dawn: " << std::ctime(&ts_dawn_time_t);
    std::cout << "Calculated Sunrise: " << std::ctime(&ts_rise_time_t);
    std::cout << "Calculated Culmination: " << std::ctime(&ts_noon_time_t);
    std::cout << "Calculated Sunset: " << std::ctime(&ts_set_time_t);
    std::cout << "Calculated Dusk: " << std::ctime(&ts_dusk_time_t);
    
    // Calculate daylight duration
    double daylight_duration = (ts_set - ts_rise) / 3600.0;
    std::cout << "Daylight Duration: " << daylight_duration << " hours\n";

    // Calculate time until dawn
    double time_until_dawn = difftime(ts_dawn, utc_time);
    if (time_until_dawn < 0) {
        time_until_dawn += 86400; // Adjust for next day
    }
    std::cout << "Time until Dawn: " << time_until_dawn / 3600.0 << " hours\n";

    // Calculate time until dusk
    double time_until_dusk = difftime(ts_dusk, utc_time);
    if (time_until_dusk < 0) {
        time_until_dusk += 86400; // Adjust for next day
    }
    std::cout << "Time until Dusk: " << time_until_dusk / 3600.0 << " hours\n";

    std::cout << "----- End of Debugging Solar Time Calculation -----\n";
}
void CalculateSolarTimes4(double latitude, double longitude, time_t utc_time, double elevation = 0.0, bool daylight_savings = false) {
    std::cout << "----- Debugging Solar Time Calculation4 -----\n\n";

    std::cout << "Current Time (UTC): " << std::ctime(&utc_time);
    std::cout << "Current Unix Time: " << utc_time << " seconds since epoch\n";

    double J_date = ts2j(utc_time);
    std::cout << "Julian Date: " << J_date << " Julian days\n";

    double n = ceil(J_date - (2451545.0 + 0.0009) + longitude / 360.0);
    std::cout << "Julian Day (n): " << n << "\n";

    double J_ = n + 0.0009 - longitude / 360.0;
    std::cout << "Mean Solar Time (J_): " << J_ << " Julian days\n";

    double M_degrees = calculateM(J_);
    std::cout << "Solar Mean Anomaly (M_degrees): " << M_degrees << " degrees\n";

    double M_radians = M_degrees * DEG_TO_RAD;

    double C_degrees = calculateC(M_radians);
    std::cout << "Equation of Center (C_degrees): " << C_degrees << " degrees\n";

    double L_degrees = calculateL(M_degrees, C_degrees);
    std::cout << "Ecliptic Longitude (L_degrees): " << L_degrees << " degrees\n";

    double Lambda_radians = L_degrees * DEG_TO_RAD;

    double J_transit = calculateJtransit(J_, M_radians, Lambda_radians);
    std::cout << "Solar Transit Time (J_transit): " << J_transit << " Julian days\n";

    double delta = calculateDeclination(Lambda_radians);
    std::cout << "Solar Declination (delta): " << delta << " degrees\n";

    double w0_degrees = calculateHourAngle(latitude, delta, elevation);
    std::cout << "Hour Angle (w0_degrees): " << w0_degrees << " degrees\n";

    double j_rise = J_transit - w0_degrees / 360.0;
    double j_set = J_transit + w0_degrees / 360.0;
    double j_noon = J_transit;

    double ts_rise = j2ts(j_rise);
    double ts_set = j2ts(j_set);
    double ts_noon = j2ts(j_noon);

    // Apply daylight savings adjustment
    if (daylight_savings) {
        ts_rise += 3600; // Add one hour
        ts_set += 3600;
        ts_noon += 3600;
    }

    std::cout << "Calculated Sunrise: " << ts_rise << " (Unix time)\n";
    std::cout << "Calculated Sunset: " << ts_set << " (Unix time)\n";

    // Calculate dawn and dusk using the -6 degrees (civil twilight) angle
    double twilight_angle = 6.0;
    double ha_dawn_degrees = calculateHourAngle2(latitude, delta, elevation, -twilight_angle);
    double ha_dusk_degrees = calculateHourAngle2(latitude, delta, elevation, twilight_angle);
    double j_dawn = J_transit - ha_dawn_degrees / 360.0;
    double j_dusk = J_transit + ha_dusk_degrees / 360.0;

    double ts_dawn = j2ts(j_dawn);
    double ts_dusk = j2ts(j_dusk);

    if (daylight_savings) {
        ts_dawn += 3600;
        ts_dusk += 3600;
    }

    std::cout << "Calculated Dawn: " << ts_dawn << " (Unix time)\n";
    std::cout << "Calculated Dusk: " << ts_dusk << " (Unix time)\n";

    // Log the current time and calculated times
    std::cout << "UTC Time: " << utc_time << "\n";
    std::cout << "Sunset Time (ts_set): " << ts_set << "\n";
    std::cout << "Dusk Time (ts_dusk): " << ts_dusk << "\n";
    std::cout << "UTC Time + 86400 (next day): " << utc_time + 86400 << "\n";

    // Ensure that dusk and sunset are calculated correctly for today if the current time is before these events
    if (ts_dusk > utc_time && ts_dusk < utc_time + 86400) {
        std::cout << "Dusk is occurring today.\n";
    } else if (ts_dusk < utc_time) {
        std::cout << "Dusk has already passed today, adjusting to tomorrow.\n";
        ts_dusk += 86400;
    } else if (ts_dusk > utc_time + 86400) {
        std::cout << "Dusk is already calculated for tomorrow, adjusting to today.\n";
        ts_dusk -= 86400;
    }

    if (ts_set > utc_time && ts_set < utc_time + 86400) {
        std::cout << "Sunset is occurring today.\n";
    } else if (ts_set < utc_time) {
        std::cout << "Sunset has already passed today, adjusting to tomorrow.\n";
        ts_set += 86400;
    } else if (ts_set > utc_time + 86400) {
        std::cout << "Sunset is already calculated for tomorrow, adjusting to today.\n";
        ts_set -= 86400;
    }

    // Convert double times to time_t for ctime compatibility
    time_t ts_rise_time_t = static_cast<time_t>(ts_rise);
    time_t ts_set_time_t = static_cast<time_t>(ts_set);
    time_t ts_noon_time_t = static_cast<time_t>(ts_noon);
    time_t ts_dawn_time_t = static_cast<time_t>(ts_dawn);
    time_t ts_dusk_time_t = static_cast<time_t>(ts_dusk);

    std::cout << "Final Calculated Dawn: " << std::ctime(&ts_dawn_time_t);
    std::cout << "Final Calculated Sunrise: " << std::ctime(&ts_rise_time_t);
    std::cout << "Final Calculated Culmination: " << std::ctime(&ts_noon_time_t);
    std::cout << "Final Calculated Sunset: " << std::ctime(&ts_set_time_t);
    std::cout << "Final Calculated Dusk: " << std::ctime(&ts_dusk_time_t);

    // Calculate daylight duration
    double daylight_duration = (ts_set_time_t > ts_rise_time_t) ? (ts_set_time_t - ts_rise_time_t) / 3600.0 : ((ts_set_time_t + 86400) - ts_rise_time_t) / 3600.0;
    std::cout << "Daylight Duration: " << daylight_duration << " hours\n";

    // Calculate time until dawn
    double time_until_dawn = difftime(ts_dawn, utc_time);
    if (time_until_dawn < 0) {
        time_until_dawn += 86400; // Adjust for next day
    }
    std::cout << "Time until Dawn: " << time_until_dawn / 3600.0 << " hours\n";

    // Calculate time until dusk
    double time_until_dusk = difftime(ts_dusk, utc_time);
    if (time_until_dusk < 0) {
        time_until_dusk += 86400; // Adjust for next day
    }
    std::cout << "Time until Dusk: " << time_until_dusk / 3600.0 << " hours\n";

    // Calculate time until sunrise
    double time_until_sunrise = difftime(ts_rise, utc_time);
    if (time_until_sunrise < 0) {
        time_until_sunrise += 86400; // Adjust for next day
    }
    std::cout << "Time until Sunrise: " << time_until_sunrise / 3600.0 << " hours\n";

    // Calculate time until sunset
    double time_until_sunset = difftime(ts_set, utc_time);
    if (time_until_sunset < 0) {
        time_until_sunset += 86400; // Adjust for next day
    }
    std::cout << "Time until Sunset: " << time_until_sunset / 3600.0 << " hours\n";

    std::cout << "----- End of Debugging Solar Time Calculation -----\n";
}



struct SolarTimes {
    double solar_mean_anomaly;
    double equation_of_center;
    double true_solar_longitude;
    double solar_declination;
    double equation_of_time;
    double apparent_solar_time;
    double omega;
    double sunrise;
    double sunset;
    double solar_noon;
    double daylight_duration;
    double dawn;
    double dusk;
    bool is_rising;
};

double CalculateJulianDay(time_t utc_time);
double CalculateSolarMeanAnomaly(double days_since_j2000);
double CalculateEquationOfCenter(double M);
double CalculateTrueSolarLongitude(double M, double C);
double CalculateSolarDeclination(double L);
double CalculateOmega(double days_since_j2000);
double CalculateApparentSolarTime(double L, double Omega);
double CalculateEquationOfTime(double M, double AST);

// SolarTimes CalculateSolarTimes2() {
//     SolarTimes times = {};

//     // Set specific values for Belfast, Ireland on September 2, 2024, 12:00:00 UTC
//     double latitude = 54.597;
//     double longitude = -5.930;
//     time_t utc_time = 1725283200; // This corresponds to Mon Sep 2 2024 12:00:00 UTC

//     // Logging the initialization values
//     Serial.println("----- Debugging Solar Time Calculation -----\n\r");
//     Serial.printf("Latitude: %.6f\n\r", latitude);  // Expected: 54.597000
//     Serial.printf("Longitude: %.6f\n\r", longitude);  // Expected: -5.930000
//     Serial.printf("Raw UTC Time (Unix): %lu\n\r", utc_time);  // Expected: 1725283200
//     Serial.printf("Formatted UTC Time: %s\n\r", ctime(&utc_time));  // Expected: "Mon Sep 2 12:00:00 2024"

//     // Step 1: Calculate Julian Day
//     double jd_2 = CalculateJulianDay(utc_time);
//     double d_2 = jd_2 - 2451545.0;  // Offset for Julian date (J2000.0 epoch)
//     Serial.printf("Julian Day: %.10f\n\r", jd_2);  // Expected: 2460556.0555555555
//     Serial.printf("Days since J2000.0: %.10f\n\r", d_2);  // Expected: 9011.0555555555

//     // Step 2: Calculate Solar Mean Anomaly (M_2)
//     times.solar_mean_anomaly = CalculateSolarMeanAnomaly(d_2);
//     Serial.printf("Solar Mean Anomaly (M_2): %.10f\n\r", times.solar_mean_anomaly);  // Expected: around 234.837

//     // Step 3: Calculate the Sun's Equation of Center (C_2)
//     times.equation_of_center = CalculateEquationOfCenter(times.solar_mean_anomaly);
//     Serial.printf("Sun's Equation of Center (C_2): %.10f\n\r", times.equation_of_center);  // Expected: around -1.516

//     // Step 4: Calculate True Solar Longitude (L_2)
//     times.true_solar_longitude = CalculateTrueSolarLongitude(times.solar_mean_anomaly, times.equation_of_center);
//     Serial.printf("True Solar Longitude (L_2): %.10f\n\r", times.true_solar_longitude);  // Expected: around 336.258

//     // Step 5: Calculate Solar Declination (δ)
//     times.solar_declination = CalculateSolarDeclination(times.true_solar_longitude);
//     Serial.printf("Solar Declination (delta_2): %.10f\n\r", times.solar_declination);  // Expected: around -14.491

//     // Step 6: Calculate Omega
//     times.omega = CalculateOmega(d_2);
//     Serial.printf("Omega_2: %.10f\n\r", times.omega);  // Expected: Calculated omega value (in degrees)

//     // Step 7: Calculate Apparent Solar Time (AST)
//     times.apparent_solar_time = CalculateApparentSolarTime(times.true_solar_longitude, times.omega);
//     Serial.printf("Apparent Solar Time (AST_2): %.10f\n\r", times.apparent_solar_time);  // Expected: around 336.256

//     // Step 8: Calculate Equation of Time (EoT)
//     times.equation_of_time = CalculateEquationOfTime(times.solar_mean_anomaly, times.apparent_solar_time);
//     Serial.printf("Equation of Time (EoT): %.10f minutes\n\r", times.equation_of_time);  // Expected: around 6.072 minutes

//     // The function can be extended to calculate solar noon, sunrise, sunset, etc., similar to the previous functions.
    
//     Serial.println("----- End of Debugging Solar Time Calculation -----\n\r");
//     return times;
// }

double CalculateJulianDay(time_t utc_time) {
    // Julian Day for the provided UTC time
    return (utc_time / 86400.0) + 2440587.5;  // Convert Unix time to Julian Day
}

double CalculateSolarMeanAnomaly(double days_since_j2000) {
    return fmod(357.5291 + 0.98560028 * days_since_j2000, 360.0);
}

double CalculateEquationOfCenter(double M) {
    double sin_M = sin(M * DEG_TO_RAD);
    double sin_2M = sin(2 * M * DEG_TO_RAD);
    double sin_3M = sin(3 * M * DEG_TO_RAD);
    return 1.9148 * sin_M + 0.0200 * sin_2M + 0.0003 * sin_3M;
}

double CalculateTrueSolarLongitude(double M, double C) {
    return fmod(M + C + 102.9372, 360.0);
}

double CalculateSolarDeclination(double L) {
    return asin(sin(L * DEG_TO_RAD) * sin(23.44 * DEG_TO_RAD)) * RAD_TO_DEG;
}

double CalculateOmega(double days_since_j2000) {
    double Omega = fmod(125.04 - 1934.136 * days_since_j2000, 360.0);
    if (Omega < 0) Omega += 360.0;
    return Omega;
}

double CalculateApparentSolarTime(double L, double Omega) {
    double AST_correction = 0.00478 * sin(Omega * DEG_TO_RAD);
    return L - 0.00569 - AST_correction;
}

double CalculateEquationOfTime(double M, double AST) {
    double term1 = M - 0.0057183;
    double term_diff = term1 - AST;
    return term_diff * 4.0;
}





int8_t mMoonTracking::Tasker(uint8_t function, JsonParserObject obj)
{

	// +- 6% of elevation is dust/dawn

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
  }
  
  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
	case TASK_EVERY_50_MSECOND:
	{
		Update_Solar_Tracking_Data();
	}
	break;
    case TASK_EVERY_SECOND:{

		// uint32_t local_time = tkr_time->Rtc.local_time;
		// double latitude = tkr_set->Settings.sensors.latitude;
		// double longitude = tkr_set->Settings.sensors.longitude;

		// Serial.println("Latitude: " + String(latitude));
		// Serial.println("Longitude: " + String(longitude));
		// Serial.println("Raw Local Time (Unix): " + String(local_time));

		// time_t local_time2 = tkr_time->Rtc.local_time;
		// Serial.printf("Formatted Local Time: %s\n\r", ctime(&local_time2));

		// // Now, let's break down the date and time being used
		// tm* local_tm = localtime(&local_time2);  // Convert to tm structure
		// Serial.printf("Year: %d, Month: %d, Day: %d\n\r", local_tm->tm_year + 1900, local_tm->tm_mon + 1, local_tm->tm_mday);
		// Serial.printf("Hour: %d, Minute: %d, Second: %d\n\r", local_tm->tm_hour, local_tm->tm_min, local_tm->tm_sec);

		// SolarTimes times = CalculateSolarTimes(latitude, longitude, local_time);

		// Serial.printf("Calculated Sunrise: %s\r", ctime(&times.sunrise));
		// Serial.printf("Calculated Sunset: %s\r", ctime(&times.sunset));
		// Serial.printf("Calculated Dawn: %s\r", ctime(&times.dawn));
		// Serial.printf("Calculated Dusk: %s\r", ctime(&times.dusk));
		// Serial.printf("Calculated Solar Noon: %s\r", ctime(&times.solar_noon));
		// Serial.printf("Calculated Daylight Duration: %.2f hours\n\r", times.daylight_duration / 3600.0);
		// Serial.printf("Sun is currently %s\n\r", times.is_rising ? "rising" : "setting");

    double latitude = 54.508659;
    double longitude = -6.295350;
    // time_t utc_time = 1725283200; // This corresponds to Mon Sep 2 2024 12:00:00 UTC
    // CalculateSolarTimes2(latitude, longitude, utc_time);

    time_t utc_time = tkr_time->LocalTime(); // This corresponds to Mon Sep 2 2024 12:00:00 UTC
	// CalculateSolarTimes3(latitude, longitude, utc_time, 0.0, true);
	CalculateSolarTimes4(latitude, longitude, utc_time, 0.0, true);


    }
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
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

}


// solar_position_t mMoonTracking::Get_Solar_Tracking_Data(uint32_t utc_time)
// {


// }


void mMoonTracking::Pre_Init()
{

	settings.fEnableSensor = true; 

}


void mMoonTracking::Init()
{


}


void mMoonTracking::Update_Solar_Tracking_Data()
{

	// if(!tkr_time->RtcTime.isvalid)
	// {
	// 	return; // Cant compute, invalid data
	// }

	float altitude = 0;
	float lat = tkr_set->Settings.sensors.latitude;
	float lon = tkr_set->Settings.sensors.longitude;

	double elevation_tmp = 0;

	SolarAzEl(tkr_time->Rtc.utc_time, lat, lon, 0, &solar_position.azimuth, &elevation_tmp);

	/**
	 * Get direction of change ie sunrise or sunset
	 * */
	if(elevation_tmp > solar_position.elevation)
	{
		solar_position.direction.is_ascending = true;
	}else{
		solar_position.direction.is_ascending = false;
	}
	solar_position.elevation = elevation_tmp+elevation_adjusted_for_debugging;


	/**
	 * @brief Can only be set when the time has been set first
	 **/
	if(tkr_time->RtcTime.valid)
	{
		solar_position.isvalid = true;
	}else{
		solar_position.isvalid = false;
	}

	/**
	 * @brief Update period of day based on sun elevation
	 **/
	if((solar_position.elevation>5)&&(solar_position.direction.is_ascending == false)) // 
	{
		// time of day is "Daytime-Increasing"
		// Daytime-Decreasing


	}

	if((solar_position.elevation>5)&&(solar_position.direction.is_ascending == false)) // 
	{

	}

	solar_position.tUpdated_millis = millis();

}

/**
 * @brief Calculate sun position (azimuth and elevation)
 * @param utc_time_point utc_time (32 bit) eg time right now (can use days range to for animate days pattern)
 * @param latitude 
 * @param longitude 
 * @param altitude
 * @param azimuth 
 * @param elevation 
 * @note
 * Calculations will be performed with double-percision (64 bit float)
 * Values returned only need float-precision (32 bit)
 * */
void mMoonTracking::SolarAzEl(time_t utc_time_point, double Lat, double Lon, double Alt, double* Az, double* El)
{
	
	double jd = julian_day(utc_time_point);

	double d = jd - 2451543.5;
	
	// Keplerian Elements for the Sun(geocentric)
	double w = 282.9404 + 4.70935e-5*d; // (longitude of perihelion degrees)
	// a = 1.000000; % (mean distance, a.u.)
	double e = 0.016709 - 1.151e-9*d; // (eccentricity)
	double M = fmod(356.0470 + 0.9856002585*d, 360.0); // (mean anomaly degrees)
		
	double L = w + M; // (Sun's mean longitude degrees)

	double oblecl = 23.4393 - 3.563e-7*d; // (Sun's obliquity of the ecliptic)

	// auxiliary angle
	double  E = M + (180 / M_PI)*e*sin(M*(M_PI / 180))*(1 + e*cos(M*(M_PI / 180)));

	// rectangular coordinates in the plane of the ecliptic(x axis toward perhilion)
	double x = cos(E*(M_PI / 180)) - e;
	double y = sin(E*(M_PI / 180))*sqrt(1 - pow(e, 2));

	// find the distance and true anomaly
	double r = sqrt(pow(x,2) + pow(y,2));
	double v = atan2(y, x)*(180 / M_PI);

	// find the longitude of the sun
	double lon = v + w;

	// compute the ecliptic rectangular coordinates
	double xeclip = r*cos(lon*(M_PI / 180));
	double yeclip = r*sin(lon*(M_PI / 180));
	double zeclip = 0.0;

	//rotate these coordinates to equitorial rectangular coordinates
	double xequat = xeclip;
	double yequat = yeclip*cos(oblecl*(M_PI / 180)) + zeclip * sin(oblecl*(M_PI / 180));
	double zequat = yeclip*sin(23.4406*(M_PI / 180)) + zeclip * cos(oblecl*(M_PI / 180));

	// convert equatorial rectangular coordinates to RA and Decl:
	r = sqrt(pow(xequat, 2) + pow(yequat, 2) + pow(zequat, 2)) - (Alt / 149598000); //roll up the altitude correction
	double RA = atan2(yequat, xequat)*(180 / M_PI);

	double delta = asin(zequat / r)*(180 / M_PI);
	
	// Get UTC representation of time / C++ Specific
	time_t utc_time = (time_t)tkr_time->Rtc.utc_time;
	tm *ptm = gmtime(&utc_time); 
        
	double UTH = (double)ptm->tm_hour + (double)ptm->tm_min / 60 + (double)ptm->tm_sec / 3600;

	// Calculate local siderial time
	double GMST0 = fmod(L + 180, 360.0) / 15;

	double SIDTIME = GMST0 + UTH + Lon / 15;
	
	// Replace RA with hour angle HA
	double HA = (SIDTIME*15 - RA);

	// convert to rectangular coordinate system
	x = cos(HA*(M_PI / 180))*cos(delta*(M_PI / 180));

	y = sin(HA*(M_PI / 180))*cos(delta*(M_PI / 180));
	double z = sin(delta*(M_PI / 180));

	// rotate this along an axis going east - west.
	double xhor = x*cos((90 - Lat)*(M_PI / 180)) - z*sin((90 - Lat)*(M_PI / 180));

	double yhor = y;
	double zhor = x*sin((90 - Lat)*(M_PI / 180)) + z*cos((90 - Lat)*(M_PI / 180));

	// float azimuth_f = atan2(yhor, xhor)*(180 / M_PI) + 180;
	// float elevation_f = asin(zhor)*(180 / M_PI);
	
	// // Find the h and AZ
	// *Az = azimuth_f;
	// *El = elevation_f;
	
	*Az = atan2(yhor, xhor)*(180 / M_PI) + 180;
	*El =  asin(zhor)*(180 / M_PI);

}

double mMoonTracking::julian_day(time_t utc_time_point) 
{
	/**
	 * Method A
	 * */
	struct tm* tm = gmtime(&utc_time_point); // Extract UTC Time

	double year = tm->tm_year + 1900;
	double month = tm->tm_mon + 1;
	double day = tm->tm_mday;
	double hour = tm->tm_hour;
	double min = tm->tm_min;
	double sec = tm->tm_sec;

	if (month == 2) {
		year -= 1;
		month += 12;
	}

	double jd = 
		floor(365.25*(year + 4716.0)) + floor(30.6001*(month + 1.0)) + 2.0 -
		floor(year / 100.0) + floor(floor(year / 100.0) / 4.0) + day - 1524.5 +
		(hour + min / 60 + sec / 3600) / 24;

	return jd;


	/**
	 * Method B
	 * */
	//   uint32_t Year = tkr_time->RtcTime.year;             // Year ex:2020
	//   uint32_t Month = tkr_time->RtcTime.month;            // 1..12
	//   uint32_t Day = tkr_time->RtcTime.Wday;     // 1..31
	//   uint32_t Julian;                          // Julian day number

	//   if (Month <= 2) {
	//     Month += 12;
	//     Year -= 1;
	//   }
	//   // Warning, this formula works only for the 20th century, afterwards be are off by 1 day - which does not impact Sunrise much
	//   // Julian = (1461 * Year + 6884472) / 4 + (153 * Month - 457) / 5 + Day -1 -13;
	//   Julian = (1461 * Year + 6884416) / 4 + (153 * Month - 457) / 5 + Day;   // -1 -13 included in 6884472 - 14*4 = 6884416
	//   return Julian;

}

const double DEG_TO_RAD_2 = M_PI / 180.0;  // Converts degrees to radians
const double RAD_TO_DEG_2 = 180.0 / M_PI;  // Converts radians to degrees
const double JULIAN_DATE_OFFSET_2 = 2451545.0; // Offset for Julian date calculation
const double SOLAR_DECLINATION_ANGLE_2 = 23.44; // Solar declination angle in degrees
const double SOLAR_ELEVATION_AT_HORIZON_2 = -0.833; // Angle for sunrise/sunset (approximately -0.833 degrees)


mMoonTracking::SolarTimes mMoonTracking::CalculateSolarTimes(double latitude, double longitude, time_t utc_time) {
    SolarTimes times = {};

    // Override with specific values for Belfast, Ireland on September 2, 2024, 12:00:00 UTC
    latitude = 54.597;
    longitude = -5.930;
    utc_time = 1725283200; // This corresponds to Mon Sep 2 2024 12:00:00 UTC

    Serial.println("----- Debugging Solar Time Calculation -----\n\r");
    Serial.printf("Latitude: %.6f\n\r", latitude);  // Expected: 54.597000
    Serial.printf("Longitude: %.6f\n\r", longitude);  // Expected: -5.930000
    Serial.printf("Raw UTC Time (Unix): %lu\n\r", utc_time);  // Expected: 1725283200

    // Convert UTC time to human-readable format
    Serial.printf("Formatted UTC Time: %s\n\r", ctime(&utc_time));  // Expected: "Mon Sep 2 12:00:00 2024"

    // Julian date calculation
    double jd_2 = julian_day(utc_time);
    double d_2 = jd_2 - 2451545.0;  // Offset for Julian date
    Serial.printf("Julian Day: %.10f\n\r", jd_2);  // Expected: 2460556.0000000000
    Serial.printf("Days since J2000.0: %.10f\n\r", d_2);  // Expected: 9011.0000000000

    // Calculate Solar Mean Anomaly (M_2) with detailed breakdown
    double base_value = 357.5291;
    double rate_of_change = 0.98560028;
    double product = rate_of_change * d_2;  // Multiplying rate of change by days since J2000.0
    double M_2 = fmod(base_value + product, 360.0); // Adding the base value to the product and applying modulus

    Serial.printf("Base Value: %.10f\n\r", base_value);  // Expected: 357.5291
    Serial.printf("Rate of Change: %.10f\n\r", rate_of_change);  // Expected: 0.98560028
    Serial.printf("Product (Rate of Change * Days since J2000.0): %.10f\n\r", product);  // Expected: should be close to 8881.000000
    Serial.printf("Solar Mean Anomaly (M_2): %.10f\n\r", M_2);  // Expected: around 9238.8279786511 (or 357.5291 + product)

    // Calculate Sun's Equation of Center (C_2) with detailed breakdown
    double sin_M2 = sin(M_2 * DEG_TO_RAD);
    double sin_2M2 = sin(2 * M_2 * DEG_TO_RAD);
    double sin_3M2 = sin(3 * M_2 * DEG_TO_RAD);

    double C_2_term1 = 1.9148 * sin_M2;
    double C_2_term2 = 0.0200 * sin_2M2;
    double C_2_term3 = 0.0003 * sin_3M2;

    double C_2 = C_2_term1 + C_2_term2 + C_2_term3;

    Serial.printf("sin(M_2): %.10f\n\r", sin_M2);  // Expected: Should be the sine of `M_2b`, a small number
    Serial.printf("sin(2*M_2): %.10f\n\r", sin_2M2);  // Expected: Should be the sine of `2*M_2b`, also a small number
    Serial.printf("sin(3*M_2): %.10f\n\r", sin_3M2);  // Expected: Should be the sine of `3*M_2b`, very small number
    Serial.printf("C_2_term1 (1.9148 * sin(M_2)): %.10f\n\r", C_2_term1);  // Expected: Around 1.9148 * sin(M_2b)
    Serial.printf("C_2_term2 (0.0200 * sin(2*M_2)): %.10f\n\r", C_2_term2);  // Expected: Around 0.0200 * sin(2*M_2b)
    Serial.printf("C_2_term3 (0.0003 * sin(3*M_2)): %.10f\n\r", C_2_term3);  // Expected: Around 0.0003 * sin(3*M_2b)
    Serial.printf("Sun's Equation of Center (C_2): %.10f\n\r", C_2);  // Expected: A small value, typically between -2 to +2

    // Calculate the True Solar Longitude (L_2) with detailed breakdown
    double M_C_combined = M_2 + C_2;  // Combine M_2 and C_2 first
    Serial.print("M_2 + C_2: ");
    Serial.println(M_C_combined, 10); // Expected: around 237-240, after the modular reduction of M_2

    double add_constant = M_C_combined + 102.9372;  // Add the constant 102.9372
    Serial.print("M_2 + C_2 + 102.9372: ");
    Serial.println(add_constant, 10); // Expected: Should be in the range close to 340

    double L_2 = fmod(add_constant, 360.0);  // Apply modulo 360.0 to the result
    Serial.print("True Solar Longitude (L_2): ");
    Serial.println(L_2, 10); // Expected: around 159.8413580016




// Step-by-step breakdown of the Omega_2 calculation for AST_2

// Step 1: Base value for Omega
double Omega_base = 125.04;
Serial.print("Omega_base: ");
Serial.println(Omega_base, 10);  // Expected: 125.04

// Step 2: Rate of change
double Omega_rate_of_change = 1934.136;
Serial.print("Omega_rate_of_change: ");
Serial.println(Omega_rate_of_change, 10);  // Expected: 1934.136

// Step 3: Calculate the product of the rate of change and days since J2000.0
double Omega_product = fmod(Omega_rate_of_change * d_2, 360.0);  // Apply modulo here
Serial.print("Omega_product (mod 360): ");
Serial.println(Omega_product, 10);  // Expected: Large number but within the range [0, 360)

// Step 4: Calculate the Omega_2 value by subtracting the product from the base value
double Omega_2 = Omega_base - Omega_product;
Serial.print("Omega_2 (with product mod 360): ");
Serial.println(Omega_2, 10);  // Omega_2 might be negative here

// Step 5: Correct Omega_2 if it's negative by adding 360 to bring it within [0, 360)
if (Omega_2 < 0) {
    Omega_2 += 360.0;
}
Serial.print("Normalized Omega_2 (within [0, 360)): ");
Serial.println(Omega_2, 10);  // Expected: Should now be within [0, 360)

// VALID ABOVE
// Ω2​ (Normalized)=−17428481.9079998992mod360.0=198.09200010



double sin_Omega_2 = sin(Omega_2 * DEG_TO_RAD);
Serial.print("sin(Omega_2): ");
Serial.println(sin_Omega_2, 10);  // Expected: -0.3105437113 (from the earlier result)

double AST_2_correction = 0.00478 * sin_Omega_2;
Serial.print("AST_2 Correction: ");
Serial.println(AST_2_correction, 10);  // Expected: -0.0014843989 (calculated as 0.00478 * -0.3105437113)

double AST_2_base = L_2 - 0.00569;
Serial.print("AST_2 Base: ");
Serial.println(AST_2_base, 10);  // Expected: 340.1388496031 (calculated as 340.1445396031 - 0.00569)

double AST_2 = AST_2_base - AST_2_correction;
Serial.print("Apparent Solar Time (AST_2): ");
Serial.println(AST_2, 10);  // Expected: 340.1403340021 (calculated as 340.1388496031 - (-0.0014843989))

// The Apparent Solar Time (AST2AST2​) should be 340.140 degrees.
// This value represents the apparent solar longitude, adjusted for the nutation effects, and is a key component in calculating the Equation of Time and other solar parameters.

// Calculate the Equation of Time (EoT)
double term1 = M_2 - 0.0057183;
Serial.print("Term1 (M_2 - 0.0057183): ");
Serial.println(term1, 10);  // Expected: 238.8222603511 (calculated as 238.8279786511 - 0.0057183)

double term_diff = term1 - AST_2;
Serial.print("Term Difference (Term1 - AST_2): ");
Serial.println(term_diff, 10);  // Expected: -101.3180736510 (calculated as 238.8222603511 - 340.1403340021)

double eqtime_2 = term_diff * 4.0;
Serial.print("Equation of Time (EoT): ");
Serial.println(eqtime_2, 10);  // Expected: -405.2722946040 minutes (calculated as -101.3180736510 * 4.0)



    // Intermediate values for hour angle calculation
    double sin_lat = sin(latitude * DEG_TO_RAD);
    double cos_lat = cos(latitude * DEG_TO_RAD);
    double delta_2 = asin(sin(L_2 * DEG_TO_RAD) * sin(23.44 * DEG_TO_RAD)) * RAD_TO_DEG;  // Solar Declination
    double sin_delta = sin(delta_2 * DEG_TO_RAD);
    double cos_delta = cos(delta_2 * DEG_TO_RAD);
    Serial.printf("Solar Declination (delta_2): %.10f\n\r", delta_2);  // Expected: Declination angle in degrees
    Serial.printf("sin(latitude): %.10f\n\r", sin_lat);  // Expected: around 0.815000
    Serial.printf("cos(latitude): %.10f\n\r", cos_lat);  // Expected: around 0.579000
    Serial.printf("sin(declination): %.10f\n\r", sin_delta);  // Expected: around 0.137000
    Serial.printf("cos(declination): %.10f\n\r", cos_delta);  // Expected: around 0.990000

    // Hour angle at sunrise
    double cos_ha_sunrise_2 = (cos(90.833 * DEG_TO_RAD) - (sin_lat * sin_delta)) / (cos_lat * cos_delta);
    if (cos_ha_sunrise_2 < -1.0) cos_ha_sunrise_2 = -1.0;
    if (cos_ha_sunrise_2 > 1.0) cos_ha_sunrise_2 = 1.0;
    double ha_sunrise_2 = acos(cos_ha_sunrise_2) * RAD_TO_DEG;
    Serial.printf("cos_ha_sunrise_2: %.10f\n\r", cos_ha_sunrise_2);  // Expected: should be between -1.0 and 1.0
    Serial.printf("Hour Angle at Sunrise (ha_sunrise_2): %.10f\n\r", ha_sunrise_2);  // Expected: around 102.7 degrees

    // Solar noon calculation
    double solar_noon_2 = fmod((720.0 - (4.0 * longitude) - eqtime_2) / 1440.0, 1.0);
    Serial.printf("Solar Noon (fraction of day): %.10f\n\r", solar_noon_2);  // Expected: around 0.5 (meaning 12:00 PM local time)

    // Sunrise and sunset in fractional day format
    double sunrise_2 = solar_noon_2 - ((ha_sunrise_2 * 4.0) / 1440.0);
    double sunset_2 = solar_noon_2 + ((ha_sunrise_2 * 4.0) / 1440.0);
    Serial.printf("Sunrise (fraction of day): %.10f\n\r", sunrise_2);  // Expected: around 0.25 (meaning 6:00 AM local time)
    Serial.printf("Sunset (fraction of day): %.10f\n\r", sunset_2);  // Expected: around 0.75 (meaning 6:00 PM local time)

    // Convert to UTC time
    times.sunrise = utc_time + (sunrise_2 * 86400.0);
    times.sunset = utc_time + (sunset_2 * 86400.0);
    times.solar_noon = utc_time + (solar_noon_2 * 86400.0);
    times.daylight_duration = (sunset_2 - sunrise_2) * 86400.0;
    Serial.printf("Calculated Sunrise: %s\n\r", ctime(&times.sunrise));  // Expected: time close to local sunrise in Belfast on Sep 2, 2024
    Serial.printf("Calculated Sunset: %s\n\r", ctime(&times.sunset));  // Expected: time close to local sunset in Belfast on Sep 2, 2024
    Serial.printf("Calculated Solar Noon: %s\n\r", ctime(&times.solar_noon));  // Expected: time around 12:00 PM local time in Belfast
    Serial.printf("Calculated Daylight Duration: %.2f hours\n\r", times.daylight_duration / 3600.0);  // Expected: around 13 to 14 hours

    // Assuming dawn and dusk are 6 degrees below the horizon
    double ha_twilight_2 = acos((cos(96.0 * DEG_TO_RAD) - (sin_lat * sin_delta)) / (cos_lat * cos_delta)) * RAD_TO_DEG;
    times.dawn = utc_time + ((solar_noon_2 - (ha_twilight_2 * 4.0 / 1440.0)) * 86400.0);
    times.dusk = utc_time + ((solar_noon_2 + (ha_twilight_2 * 4.0 / 1440.0)) * 86400.0);
    Serial.printf("Calculated Dawn: %s\n\r", ctime(&times.dawn));  // Expected: time slightly before sunrise
    Serial.printf("Calculated Dusk: %s\n\r", ctime(&times.dusk));  // Expected: time slightly after sunset

    // Determine if the sun is rising or setting at the given UTC time
    times.is_rising = (utc_time < times.solar_noon);
    Serial.printf("Sun is currently %s\n\r", times.is_rising ? "rising" : "setting");  // Expected: "rising" if before solar noon, "setting" if after

    Serial.println("----- End of Debugging Solar Time Calculation -----\n\r");
    return times;
}







void mMoonTracking::parse_JSONCommand(JsonParserObject obj)
{

	
  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  if(jtok = obj["SolarElevation"]){
    solar_position_testing.elevation = (double)jtok.getFloat();

	Serial.printf("solar_position_testing.elevation=%f\n\r",solar_position_testing.elevation);

	// ALOG_INF(PSTR("SolarElevation=%d %d"),(int)solar_position_testing.elevation,jtok.getInt() );
	// delay(3000);
  }

  if(jtok = obj["SolarElevationAdjusted"]){
    elevation_adjusted_for_debugging = (double)jtok.getFloat();

	Serial.printf("elevation_adjusted_for_debugging=%f\n\r",elevation_adjusted_for_debugging);

	// ALOG_INF(PSTR("SolarElevation=%d %d"),(int)solar_position_testing.elevation,jtok.getInt() );
	// delay(3000);
  }


	#ifdef USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  if(jtok = obj["DebugSolar"].getObject()["Enabled"])
  {
    debug.enabled = jtok.getBool();
  }

  if(jtok = obj["DebugSolar"].getObject()["Elevation"])
  {
    debug.elevation = jtok.getFloat();
  }

  if(jtok = obj["DebugSolar"].getObject()["Azimuth"])
  {
    debug.azimuth = jtok.getFloat();
  }
	#endif // USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  
  

}

uint8_t mMoonTracking::ConstructJSON_Sensor(uint8_t json_method, bool json_appending){

  JBI->Start();

	if(solar_position.isvalid)
	{
		JBI->Add("Az",(float)solar_position.azimuth);
		JBI->Add("El",(float)solar_position.elevation);
		JBI->Add("age",millis()-solar_position.tUpdated_millis);
		JBI->Add("elevation_adjusted_for_debugging", elevation_adjusted_for_debugging);


		// time until/from
		/**
		 * @brief 
		 * 
		 * sunrise
		 * sunset
		 * dust
		 * dawn
		 * culmination
		 * daylight duration
		 * sun_direction : rising, falling
		 * 
		 * when the event has passed for this day, then add 24 hours until the next one. 
		 * 
		 */
	}

  return JBI->End();

}

uint8_t mMoonTracking::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  char buffer[20];

  JBI->Start();
	JBI->Add("mMoonTracking",0);
  return JBI->End();

}



#ifdef USE_MODULE_NETWORK_MQTT

void mMoonTracking::MQTTHandler_Init()
{

  struct handler<mMoonTracking>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMoonTracking::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMoonTracking::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMoonTracking::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMoonTracking::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mMoonTracking::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}



#endif // USE_MODULE_NETWORK_MQTT



#endif // USE_MODULE_SENSORS_SUN_TRACKING
