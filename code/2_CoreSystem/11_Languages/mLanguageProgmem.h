#ifndef mLanguageProgmem_H2
#define mLanguageProgmem_H2

// Take list of defines and create matched progmem list for use with arduinojson
// REMEMBER: you need to ifdef the ones not needed, as everything here uses flash memory whether used or not

#include "2_CoreSystem/mGlobalMacros.h"
#include "mLanguageDefault.h"

//"_JSON_" is probably not needed and can be removed from naming for both PM_ and D_

//A
DEFINE_PGM_CTR(PM_ADDRESS)           D_ADDRESS;
DEFINE_PGM_CTR(PM_ARDUINO_CORE)      "ArduinoCore";
DEFINE_PGM_CTR(PM_AP_LIST)           "APList";
DEFINE_PGM_CTR(PM_ALTITUDE) "Altitude";
DEFINE_PGM_CTR(PM_STATE_ACTIVE) "Active";
DEFINE_PGM_CTR(PM_ACTIVE_POWER) "ActivePower";
//B
DEFINE_PGM_CTR(PM_BASE)              D_BASE;
DEFINE_PGM_CTR(PM_BAUDRATE)          D_BAUDRATE;
DEFINE_PGM_CTR(PM_BOOTCOUNT)         D_BOOTCOUNT;
DEFINE_PGM_CTR(PM_BOOTCOUNTERRORS)   D_BOOTCOUNTERRORS;
DEFINE_PGM_CTR(PM_BOOTVERSION)       D_BOOTVERSION;
DEFINE_PGM_CTR(PM_BUILDDATE)         D_BUILDDATE;
DEFINE_PGM_CTR(PM_BUILDTIME)         D_BUILDTIME;
DEFINE_PGM_CTR(PM_BUILDDATETIME)     D_BUILDDATETIME;
DEFINE_PGM_CTR(PM_BROKERHOSTNAME)    D_BROKERHOSTNAME;
DEFINE_PGM_CTR(PM_BSSID)             D_BSSID;
DEFINE_PGM_CTR(PM_BUTTONRETAIN)      D_BUTTONRETAIN;
DEFINE_PGM_CTR(PM_BRIGHTNESS_MAX)           D_BRIGHTNESS_MAX;
DEFINE_PGM_CTR(PM_BRIGHTNESS_MIN)           D_BRIGHTNESS_MIN;
DEFINE_PGM_CTR(PM_BRIGHTNESS)               D_BRIGHTNESS;
DEFINE_PGM_CTR(PM_BRIGHTNESS_255)           D_BRIGHTNESS_255;
DEFINE_PGM_CTR(PM_BUFFER)           "Buffer";
DEFINE_PGM_CTR(PM_BUFLEN)           "BufLen";
//C
DEFINE_PGM_CTR(PM_CLASSID)           D_CLASSID;
DEFINE_PGM_CTR(PM_CLIENT_NAME)       "ClientName";
DEFINE_PGM_CTR(PM_CONNECTS)          D_CONNECTS;
DEFINE_PGM_CTR(PM_CONNECTCOUNT)      D_CONNECTCOUNT;
DEFINE_PGM_CTR(PM_COUNT)             D_COUNT;
DEFINE_PGM_CTR(PM_COUNTER)             D_COUNTER;
DEFINE_PGM_CTR(PM_CORE)              D_CORE;
DEFINE_PGM_CTR(PM_CURRENT)           "Current";
DEFINE_PGM_CTR(PM_CRASHDUMP)         D_CRASHDUMP;




//D
DEFINE_PGM_CTR(PM_DATA)              D_DATA;
DEFINE_PGM_CTR(PM_DEVICE)            D_DEVICE;
DEFINE_PGM_CTR(PM_DEVICEID)          D_DEVICEID;
DEFINE_PGM_CTR(PM_DEVICENAME)        D_DEVICENAME;
DEFINE_PGM_CTR(PM_DEVICES_CONNECTED) D_DEVICES_CONNECTED;
DEFINE_PGM_CTR(PM_DATETIME)          D_DATETIME;
DEFINE_PGM_CTR(PM_DOWNSECS)          D_DOWNSECS;
DEFINE_PGM_CTR(PM_DOWNTIME)          D_DOWNTIME;
DEFINE_PGM_CTR(PM_DNSSERVER)         D_DNSSERVER;
DEFINE_PGM_CTR(PM_DRIVERS)           D_DRIVERS;
DEFINE_PGM_CTR(PM_DEVICEFRIENDLYNAME) D_DEVICEFRIENDLYNAME;
DEFINE_PGM_CTR(PM_DESERIALIZATION_ERROR) D_DESERIALIZATION_ERROR;

//E
DEFINE_PGM_CTR(PM_ENDDST)            D_ENDDST;
DEFINE_PGM_CTR(PM_ENABLED)           D_ENABLED;
DEFINE_PGM_CTR(PM_ENERGY) "Energy";
//F
DEFINE_PGM_CTR(PM_FASTBOOT_RECOVERY) "FastBoot_Recovery";
DEFINE_PGM_CTR(PM_FREEHEAP)          D_FREEHEAP;
DEFINE_PGM_CTR(PM_FREEMEMORY)        D_FREEMEMORY;   
DEFINE_PGM_CTR(PM_FREESKETCHSPACE)   D_FREESKETCHSPACE;         
DEFINE_PGM_CTR(PM_FEATURES)          D_FEATURES;
DEFINE_PGM_CTR(PM_FLASHSIZE)         D_FLASHSIZE;
DEFINE_PGM_CTR(PM_FLASHCHIPID)       D_FLASHCHIPID;
DEFINE_PGM_CTR(PM_FLASHMODE)         D_FLASHMODE;
DEFINE_PGM_CTR(PM_FRIENDLYNAME)      D_FRIENDLYNAME;
DEFINE_PGM_CTR(PM_FREQUENCY) "Frequency";
DEFINE_PGM_CTR(PM_FUNCTION)          D_FUNCTION;
//G
DEFINE_PGM_CTR(PM_GATEWAY)           D_GATEWAY;
DEFINE_PGM_CTR(PM_GPIO)              D_GPIO;
DEFINE_PGM_CTR(PM_GPIOC)              D_GPIOC;
DEFINE_PGM_CTR(PM_GAS_RESISTANCE)   D_GAS_RESISTANCE;

//H
DEFINE_PGM_CTR(PM_HOSTNAME)          D_HOSTNAME;
DEFINE_PGM_CTR(PM_HEAPSIZE)          D_HEAPSIZE;
DEFINE_PGM_CTR(PM_HUMIDITY)          D_HUMIDITY;

//I
DEFINE_PGM_CTR(PM_ID)                D_ID;
DEFINE_PGM_CTR(PM_IPADDRESS)         D_IPADDRESS;
DEFINE_PGM_CTR(PM_INDEX)             D_INDEX;
//L
DEFINE_PGM_CTR(PM_LEDMASK)           D_LEDMASK;
DEFINE_PGM_CTR(PM_LEDSTATE)          D_LEDSTATE;
DEFINE_PGM_CTR(PM_LEVEL)             D_LEVEL;
DEFINE_PGM_CTR(PM_LOCAL_TIME)        D_LOCAL_TIME;
DEFINE_PGM_CTR(PM_LOOPSSEC)          D_LOOPSSEC;
DEFINE_PGM_CTR(PM_LOOPDELAY)         D_LOOPDELAY;
DEFINE_PGM_CTR(PM_LOADAVERAGE)       D_LOADAVERAGE;
DEFINE_PGM_CTR(PM_LOOPRATIO)         D_LOOPRATIO;
DEFINE_PGM_CTR(PM_LOGLEVELS)         D_LOGLEVELS;
DEFINE_PGM_CTR(PM_LIGHT_LEVEL)         D_LIGHT_LEVEL;
DEFINE_PGM_CTR(PM_LIGHT_LUMINANCE_LUX)         D_LIGHT_LUMINANCE_LUX;

DEFINE_PGM_CTR(PM_LATITUDE) D_LATITUDE;
DEFINE_PGM_CTR(PM_LONGITUDE) D_LONGITUDE;


//M
DEFINE_PGM_CTR(PM_MAC)               D_MAC;
DEFINE_PGM_CTR(PM_MAJOR)             "Major";
DEFINE_PGM_CTR(PM_MDNS)              D_MDNS;
DEFINE_PGM_CTR(PM_MINOR)             "Minor";
DEFINE_PGM_CTR(PM_MINIMAL)           "Minimal";
DEFINE_PGM_CTR(PM_MODULE)            "Module";
DEFINE_PGM_CTR(PM_MODULENAME)        D_MODULENAME;
DEFINE_PGM_CTR(PM_MODULEID)          D_MODULEID;
DEFINE_PGM_CTR(PM_MOTION)            D_MOTION;
DEFINE_PGM_CTR(PM_MODE)              D_MODE;
DEFINE_PGM_CTR(PM_MQTT_REFRESH_RATE_IFCHANGED)   "IfChanged";
DEFINE_PGM_CTR(PM_MQTT_REFRESH_RATE_TELEPERIOD)   "Teleperiod";


DEFINE_PGM_CTR(PM_MOTION_STARTED)    D_MOTION_STARTED;
DEFINE_PGM_CTR(PM_MOTION_ENDED)      D_MOTION_ENDED;

DEFINE_PGM_CTR(PM_MQTT)              D_MQTT;
DEFINE_PGM_CTR(PM_MQTT_ENABLE_RESTART)  D_MQTT_ENABLE_RESTART;
DEFINE_PGM_CTR(PM_MESSAGE)           D_MESSAGE;


DEFINE_PGM_CTR(PM_MEMORY_INSUFFICIENT)       D_MEMORY_INSUFFICIENT;


//N
DEFINE_PGM_CTR(PM_NETWORK)           D_NETWORK;
DEFINE_PGM_CTR(PM_NAME)              D_NAME;
DEFINE_PGM_CTR(PM_NOTIFICATIONS)     D_NOTIFICATIONS;
//O
DEFINE_PGM_CTR(PM_OTAURL)            D_OTAURL;
DEFINE_PGM_CTR(PM_ORDER)             D_ORDER;
DEFINE_PGM_CTR(PM_ONOFF)             D_ONOFF;
DEFINE_PGM_CTR(PM_ONOFF_NAME)        D_ONOFF_NAME;
DEFINE_PGM_CTR(PM_LAST)              D_LAST;
DEFINE_PGM_CTR(PM_ONTIME)            D_ONTIME;
DEFINE_PGM_CTR(PM_OFFTIME)           D_OFFTIME;


DEFINE_PGM_CTR(PM_OVERRIDE)           D_OVERRIDE;



//P
DEFINE_PGM_CTR(PM_PACKETS)           D_PACKETS;
DEFINE_PGM_CTR(PM_POWER)             D_POWER;
DEFINE_PGM_CTR(PM_POWERONSTATE)      D_POWERONSTATE;
DEFINE_PGM_CTR(PM_POWER_STATE)       D_POWER_STATE;
DEFINE_PGM_CTR(PM_POWER_STATE_NAME)  D_POWER_STATE_NAME;
DEFINE_PGM_CTR(PM_POWERRETAIN)       D_POWERRETAIN;
DEFINE_PGM_CTR(PM_POWER_FACTOR) "PowerFactor";

DEFINE_PGM_CTR(PM_DOOR_POSITION)     D_DOOR_POSITION;
DEFINE_PGM_CTR(PM_DOOR_LOCKED)       D_DOOR_LOCKED;


DEFINE_PGM_CTR(PM_PRESSURE)          D_PRESSURE;
DEFINE_PGM_CTR(PM_PROGRAMSIZE)       D_PROGRAMSIZE;
DEFINE_PGM_CTR(PM_PROGRAMFLASHSIZE)  D_PROGRAMFLASHSIZE;



//Q
//R
DEFINE_PGM_CTR(PM_REFRESH_RATES)     "RefreshRates";
DEFINE_PGM_CTR(PM_RELAY_NAME)        D_RELAY_NAME;
DEFINE_PGM_CTR(PM_ROOMHINT)          D_ROOMHINT;
DEFINE_PGM_CTR(PM_RSSI)              D_RSSI;
DEFINE_PGM_CTR(PM_RECEIVEDCOUNT)     D_RECEIVEDCOUNT;
DEFINE_PGM_CTR(PM_RESOLUTION)     D_RESOLUTION;


DEFINE_PGM_CTR(PM_RATE)              D_RATE;
DEFINE_PGM_CTR(PM_RATE_SECS)         D_RATE_SECS;
DEFINE_PGM_CTR(PM_RATE_MS)           D_RATE_MS;


//S
DEFINE_PGM_CTR(PM_SAVEADDRESS)       D_SAVEADDRESS;
DEFINE_PGM_CTR(PM_SAVEDATA)          D_SAVEDATA;
DEFINE_PGM_CTR(PM_SAVESTATE)         D_SAVESTATE;
DEFINE_PGM_CTR(PM_SAVECOUNT)         D_SAVECOUNT;
DEFINE_PGM_CTR(PM_SENTCOUNT)         D_SENTCOUNT;
// DEFINE_PGM_CTR(PM_SENTPERMINUTE)     D_SENTPERMINUTE;
DEFINE_PGM_CTR(PM_SENSORRETAIN)      D_SENSORRETAIN;
DEFINE_PGM_CTR(PM_SENSORADDRESS)     D_SENSORADDRESS;
DEFINE_PGM_CTR(PM_SDKVERSION)        D_SDKVERSION;
// DEFINE_PGM_CTR(PM_SENSORS)           D_SENSORS;
// DEFINE_PGM_CTR(PM_SENSORCOUNT)       D_SENSORCOUNT;
DEFINE_PGM_CTR(PM_SETTINGS_HOLDER)   D_SETTINGS_HOLDER;
DEFINE_PGM_CTR(PM_SIZE)              D_SIZE;
DEFINE_PGM_CTR(PM_SLEEP)             D_SLEEP;
DEFINE_PGM_CTR(PM_SPEED)             D_SPEED;
DEFINE_PGM_CTR(PM_SLEEPMODE)         D_SLEEPMODE;
DEFINE_PGM_CTR(PM_SUNRISE)           D_SUNRISE;
DEFINE_PGM_CTR(PM_SUNSET)            D_SUNSET;
DEFINE_PGM_CTR(PM_SSID)              D_SSID;
DEFINE_PGM_CTR(PM_SSID_NUMBERED)     D_SSID_NUMBERED;
DEFINE_PGM_CTR(PM_STARTDST)          D_STARTDST;
DEFINE_PGM_CTR(PM_STARTUPUTC)        D_STARTUPUTC;
// DEFINE_PGM_CTR(PM_STATUS)            D_STATUS;
DEFINE_PGM_CTR(PM_STATIC_IPADDRESS)  D_STATIC_IPADDRESS;
DEFINE_PGM_CTR(PM_SUBNETMASK)        D_SUBNETMASK;
DEFINE_PGM_CTR(PM_SERIAL)            D_SERIAL;
DEFINE_PGM_CTR(PM_SWITCHMODE)        D_SWITCHMODE;
DEFINE_PGM_CTR(PM_SWITCHRETAIN)      D_SWITCHRETAIN;
DEFINE_PGM_CTR(PM_SYSTEM)            D_SYSTEM;
DEFINE_PGM_CTR(PM_SYSTEM_RESTART)         D_SYSTEM_RESTART;
DEFINE_PGM_CTR(PM_SUN_AZIMUTH)            D_SUN_AZIMUTH;
DEFINE_PGM_CTR(PM_SUN_ELEVATION)          D_SUN_ELEVATION;


DEFINE_PGM_CTR(PM_REVERSE)          D_REVERSE;
DEFINE_PGM_CTR(PM_MIRROR)          D_MIRROR;
DEFINE_PGM_CTR(PM_GROUPING)          D_GROUPING;
DEFINE_PGM_CTR(PM_DECIMATE)          D_DECIMATE;
DEFINE_PGM_CTR(PM_SPACING)          D_SPACING;
DEFINE_PGM_CTR(PM_OFFSET)             D_OFFSET;




//T
// DEFINE_PGM_CTR(PM_TEMPLATE_USED)     D_TEMPLATE_USED;
// DEFINE_PGM_CTR(PM_TEMPLATE_PARSE_SUCCESS) D_TEMPLATE_PARSE_SUCCESS;
DEFINE_PGM_CTR(PM_TIMEZONE)          D_TIMEZONE;
DEFINE_PGM_CTR(PM_TELNET)            D_TELNET;
DEFINE_PGM_CTR(PM_TELNET_PORT)       D_TELNET_PORT;
DEFINE_PGM_CTR(PM_TEMPLATE)          D_TEMPLATE;
DEFINE_PGM_CTR(PM_TIME)              D_TIME;
DEFINE_PGM_CTR(PM_TIME_SECS)         D_TIME_SECS;
DEFINE_PGM_CTR(PM_TIME_MS)           D_TIME_MS;
DEFINE_PGM_CTR(PM_TIME_ON)              D_TIME_ON;
DEFINE_PGM_CTR(PM_TIME_ON_SECS)         D_TIME_ON_SECS;
DEFINE_PGM_CTR(PM_TIME_OFF_THEN_ON_SECS)  D_TIME_OFF_THEN_ON_SECS;
DEFINE_PGM_CTR(PM_TIME_ON_MINUTES)         D_TIME_ON_MINUTES;
DEFINE_PGM_CTR(PM_TIME_ON_MS)           D_TIME_ON_MS;
DEFINE_PGM_CTR(PM_TEMPERATURE)           D_TEMPERATURE;
DEFINE_PGM_CTR(PM_TYPE)              D_TYPE;

DEFINE_PGM_CTR(PM_TEMPERATURE_HEATMAP_RGBSTRING) "TemperatureHeatmap";

DEFINE_PGM_CTR(PM_TEMPERATURE_HEATMAP_ADJUSTED_BRIGHTNESS_RGBSTRING) "TemperaTESTtureHeatMap_AdjustedBrightness";


DEFINE_PGM_CTR(PM_TIME_ELASPED)        D_TIME_ELASPED;



//U
DEFINE_PGM_CTR(PM_ULTRASONIC_DISTANCE_CM) D_ULTRASONIC_DISTANCE_CM;
DEFINE_PGM_CTR(PM_UTC_TIME)          D_UTC_TIME;
DEFINE_PGM_CTR(PM_UPSECONDS)         D_UPSECONDS;
DEFINE_PGM_CTR(PM_UPTIME)            D_UPTIME;
//V
DEFINE_PGM_CTR(PM_VERSION)           D_VERSION;
DEFINE_PGM_CTR(PM_VERSION_NAME)       D_VERSIONNAME;
DEFINE_PGM_CTR(PM_VERSION_NUMBER)     D_VERSIONNUMBER;
DEFINE_PGM_CTR(PM_VERSION_NUMBER_MINIMUM)     D_VERSIONNUMBER_MINIMUM;
DEFINE_PGM_CTR(PM_VERSION_NUMBER_NOTYPE) "VersionNumberNoType";
DEFINE_PGM_CTR(PM_VERSION_PARTS)        "VersionParts";
DEFINE_PGM_CTR(PM_VALID)               D_VALID;
DEFINE_PGM_CTR(PM_VOLTAGE) "Voltage";
//W
DEFINE_PGM_CTR(PM_WATCHDOG)            D_WATCHDOG;
DEFINE_PGM_CTR(PM_WEBSERVER_ENABLED)   D_WEBSERVER_ENABLED;
DEFINE_PGM_CTR(PM_WIFICONFIG_STATE)    D_WIFICONFIG_STATE;
DEFINE_PGM_CTR(PM_WEB)                 D_WEB;



DEFINE_PGM_CTR(WARNING_NOTHANDLED_CTR) "NOT_HANDLED";

DEFINE_PGM_CTR(PM_SEARCH_NOMATCH)   "No Match";
// DEFINE_PGM_CTR(PM_FRIENDLYNAME_DEFAULT_HARDWARE)   "No Match";



/*************************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
************* ABOVE will always be included **********************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
************** BELOW will only be included if ifdef exists *********************************************************************************************************************************************
***********************************************************************************************************************************************************
***********************************************************************************************************************************************************
********************************************************************************************************************************************************/


#ifdef USE_MODULE_LIGHTS_INTERFACE

DEFINE_PGM_CTR(PM_ANIMATIONENABLE)    D_ANIMATIONENABLE;
DEFINE_PGM_CTR(PM_ANIMATIONMODE)      D_ANIMATIONMODE;

//B
DEFINE_PGM_CTR(PM_BRIGHTNESS_RGB)           D_BRIGHTNESS_RGB;
DEFINE_PGM_CTR(PM_BRIGHTNESS_RGB_255)       D_BRIGHTNESS_RGB "_255";
DEFINE_PGM_CTR(PM_BRIGHTNESS_CCT)           D_BRIGHTNESS_CCT;
DEFINE_PGM_CTR(PM_BRIGHTNESS_CCT_255)       D_BRIGHTNESS_CCT "_255";
//C
DEFINE_PGM_CTR(PM_COLOUR_PALETTE)      D_COLOUR_PALETTE;
DEFINE_PGM_CTR(PM_COLOUR)      D_COLOUR;

DEFINE_PGM_CTR(PM_PIXELRANGE) D_PIXELRANGE;


//H
DEFINE_PGM_CTR(PM_HARDWARE_TYPE)           D_HARDWARE_TYPE;
DEFINE_PGM_CTR(PM_HSB)                 D_HSB;
DEFINE_PGM_CTR(PM_HUE)                 D_HUE;
//I
DEFINE_PGM_CTR(PM_INTENSITY)      D_INTENSITY;
//L
DEFINE_PGM_CTR (PM_LIGHT) D_LIGHT;
DEFINE_PGM_CTR (PM_LIGHTPOWER) D_LIGHTPOWER;
//P
DEFINE_PGM_CTR(PM_PIXELS_UPDATE_NUMBER)        D_PIXELS_UPDATE_NUMBER;
DEFINE_PGM_CTR(PM_PIXELS_UPDATE_PERCENTAGE)        D_PIXELS_UPDATE_PERCENTAGE;
DEFINE_PGM_CTR(PM_PALETTE_GENERATION)               D_PALETTE_GENERATION;
//R
DEFINE_PGM_CTR(PM_RGB)                 D_RGB;
DEFINE_PGM_CTR(PM_RGB_COLOUR_ORDER)                    D_RGB_COLOUR_ORDER;
// DEFINE_PGM_CTR(PM_RGB_COLOUR_ORDER2)                    D_RGB_COLOUR_ORDER "2";
DEFINE_PGM_CTR(PM_RGB_CLOCK)                             D_RGB_CLOCK;
DEFINE_PGM_CTR(PM_RANDOMISE_BRIGHTNESS_MODE)               D_RANDOMISE_BRIGHTNESS_MODE;

DEFINE_PGM_CTR(PM_MANUAL_NUMBER)           D_MANUAL_NUMBER;


DEFINE_PGM_CTR(PM_PALETTE_EDIT)      D_PALETTE_EDIT;


DEFINE_PGM_CTR(PM_COLOUR_TYPE)      D_COLOUR_TYPE;
DEFINE_PGM_CTR(PM_EFFECT_COLOUR_TYPE)      D_EFFECT_COLOUR_TYPE;



DEFINE_PGM_CTR(PM_CCT_PERCENTAGE)      D_CCT_PERCENTAGE;
DEFINE_PGM_CTR(PM_CCT_TEMP)            D_CCT_TEMP;
DEFINE_PGM_CTR(PM_RGBCCT_LINKED)       D_RGBCCT_LINKED;
DEFINE_PGM_CTR(PM_SCENE_COLOUR)        D_SCENE_COLOUR;
DEFINE_PGM_CTR(PM_SCENE_NAME)        D_SCENE_NAME;
DEFINE_PGM_CTR(PM_SAT)        D_SAT;
DEFINE_PGM_CTR(PM_SAT_255)        D_SAT "_255";
DEFINE_PGM_CTR(PM_STRIP_SIZE)  D_STRIP_SIZE;
DEFINE_PGM_CTR(PM_SCENE) D_SCENE;

DEFINE_PGM_CTR(PM_TRANSITION)        D_TRANSITION;

DEFINE_PGM_CTR(PM_SUBTYPE)      D_SUBTYPE;


#endif //USE_MODULE_LIGHTS_INTERFACE



#ifdef USE_MODULE_LIGHTS_ANIMATOR
//A
DEFINE_PGM_CTR(PM_AGED_COLOURING)    D_AGED_COLOURING;
DEFINE_PGM_CTR(PM_ALTERNATE)          D_ALTERNATE;
DEFINE_PGM_CTR(PM_ALTERNATE_AMOUNT)   D_ALTERNATE_AMOUNT;
DEFINE_PGM_CTR(PM_ALTERNATE_BRIGHTNESS_MAX)   D_ALTERNATE_BRIGHTNESS_MAX;
DEFINE_PGM_CTR(PM_ALTERNATE_BRIGHTNESS_MIN)   D_ALTERNATE_BRIGHTNESS_MIN;
//B
//D
DEFINE_PGM_CTR(PM_RGB_DATA_STREAM)          D_RGB_DATA_STREAM;
DEFINE_PGM_CTR(PM_DIRECTION)           D_DIRECTION;
//F
DEFINE_PGM_CTR(PM_EFFECTS)            D_EFFECTS;
DEFINE_PGM_CTR(PM_EFFECT)            D_EFFECT;
//M
DEFINE_PGM_CTR(PM_MIXER)           D_MIXER;
// DEFINE_PGM_CTR(PM_MULTIPLIER)     D_MULTIPLIER;
// DEFINE_PGM_CTR(PM_MAPPED_MULTIPLIER_DATA)     D_MAPPED_MULTIPLIER_DATA;
//P
DEFINE_PGM_CTR(PM_PIXELGROUPING)     D_PIXELGROUPING;
DEFINE_PGM_CTR(PM_PIXELNUM)     D_PIXELNUM;

//R
DEFINE_PGM_CTR (PM_COLOUR_REFRESH_RATE)                D_COLOUR_REFRESH_RATE;
DEFINE_PGM_CTR(PM_RUNTIME_DURATION_SCALER_PERCENTAGE)  D_RUNTIME_DURATION_SCALER_PERCENTAGE;
DEFINE_PGM_CTR(PM_RUNNING_ID)                          D_RUNNING_ID;
//S
DEFINE_PGM_CTR(PM_MANUAL_SETPIXEL_TO_SCENE) D_MANUAL_SETPIXEL_TO_SCENE;
//T
DEFINE_PGM_CTR (PM_TIME_SCALER_AS_PERCENTAGE) D_TIME_SCALER_AS_PERCENTAGE;

#endif //USE_MODULE_LIGHTS_ANIMATOR



#ifdef USE_MODULE_DRIVERS_RELAY
DEFINE_PGM_CTR(PM_POWERNAME)         D_POWERNAME;
DEFINE_PGM_CTR(PM_RELAY)             D_RELAY;
#endif //USE_MODULE_DRIVERS_RELAY

#ifdef USE_MODULE_SENSORS_BUTTONS
DEFINE_PGM_CTR(PM_PRESSED)                D_PRESSED;
DEFINE_PGM_CTR(PM_NOT_PRESSED)                D_NOT_PRESSED;
#endif // USE_MODULE_SENSORS_BUTTONS

DEFINE_PGM_CTR(PM_INVALID_FORMAT) "Invalid Format";

#if defined(USE_MODULE_CONTROLLER_FAN) || defined(USE_MODULE_CONTROLLER_SONOFF_IFAN)
DEFINE_PGM_CTR(PM_FANSPEED)            D_FANSPEED;
DEFINE_PGM_CTR(PM_FANSPEED_MANUAL)     D_FANSPEED_MANUAL;

#endif // USE_MODULE_CONTROLLER_FAN


#ifdef USE_MODULE_DISPLAYS_INTERFACE

DEFINE_PGM_CTR(PM_DISPLAY_MODE)        D_DISPLAY_MODE;



#endif // USE_MODULE_DISPLAYS_INTERFACE

#ifdef USE_MODULE_SENSORS_SUN_TRACKING
DEFINE_PGM_CTR(PM__IS_SUN_RISING)              D__IS_SUN_RISING;
DEFINE_PGM_CTR(PM__DST_ACTIVE)                 D__DST_ACTIVE;
DEFINE_PGM_CTR(PM__ANGLES)                     D__ANGLES;
DEFINE_PGM_CTR(PM__AZIMUTH)                    D__AZIMUTH;
DEFINE_PGM_CTR(PM__ELEVATION)                  D__ELEVATION;
DEFINE_PGM_CTR(PM__ELEVATION_MAX)              D__ELEVATION_MAX;
DEFINE_PGM_CTR(PM__ELEVATION_MIN)              D__ELEVATION_MIN;
DEFINE_PGM_CTR(PM__ZENITH)                     D__ZENITH;
DEFINE_PGM_CTR(PM__AIR_MASS)                   D__AIR_MASS;
DEFINE_PGM_CTR(PM__SOLAR_IRRADIANCE)           D__SOLAR_IRRADIANCE;
DEFINE_PGM_CTR(PM__DECLINATION_ANGLE)          D__DECLINATION_ANGLE;
DEFINE_PGM_CTR(PM__DAY_LENGTH)                 D__DAY_LENGTH;
DEFINE_PGM_CTR(PM__SHADOW_LENGTH)              D__SHADOW_LENGTH;
DEFINE_PGM_CTR(PM__NONE)                       D__NONE;
DEFINE_PGM_CTR(PM__INF)                        D__INF;
DEFINE_PGM_CTR(PM__UNIX_EVENTS)                D__UNIX_EVENTS;
DEFINE_PGM_CTR(PM__DAWN)                       D__DAWN;
DEFINE_PGM_CTR(PM__SUNRISE)                    D__SUNRISE;
DEFINE_PGM_CTR(PM__SOLAR_NOON)                 D__SOLAR_NOON;
DEFINE_PGM_CTR(PM__SUNSET)                     D__SUNSET;
DEFINE_PGM_CTR(PM__DUSK)                       D__DUSK;
DEFINE_PGM_CTR(PM__DAYLIGHT_DURATION)          D__DAYLIGHT_DURATION;
DEFINE_PGM_CTR(PM__SECONDS_UNTIL)              D__SECONDS_UNTIL;
DEFINE_PGM_CTR(PM__TIME_UNTIL)                 D__TIME_UNTIL;
DEFINE_PGM_CTR(PM__DATE_TIME_EVENTS)           D__DATE_TIME_EVENTS;
DEFINE_PGM_CTR(PM__TODAY)                      D__TODAY;
DEFINE_PGM_CTR(PM__TOMORROW)                   D__TOMORROW;
DEFINE_PGM_CTR(PM__NEXT)                       D__NEXT;
DEFINE_PGM_CTR(PM__DAYLIGHT_DURATION_DIFF)     D__DAYLIGHT_DURATION_DIFF;
#endif // USE_MODULE_SENSORS_SUN_TRACKING



DEFINE_PGM_CTR(PM_GAS)     D_GAS;
DEFINE_PGM_CTR(PM_AGE)     D_AGE;
DEFINE_PGM_CTR(PM_SENSOR_COUNT)     D_SENSOR_COUNT;
























// Common
// enum UnitNames {
//   UNIT_AMPERE,
//   UNIT_HOUR,
//   UNIT_KILOOHM,
//   UNIT_KILOWATTHOUR,
//   UNIT_LUX,
//   UNIT_MICROSECOND,
//   UNIT_MILLIAMPERE,
//   UNIT_MILLIMETER_MERCURY,
//   UNIT_MILLISECOND,
//   UNIT_MINUTE,
//   UNIT_PPB,
//   UNIT_PPD,
//   UNIT_PPM,
//   UNIT_PERCENTAGE,
//   UNIT_PRESSURE,
//   UNIT_SECOND,
//   UNIT_SECTORS,
//   UNIT_VOLT,
//   UNIT_WATT,
//   UNIT_WATTHOUR,
//   UNIT_HERTZ };
// const char kUnitNames[] PROGMEM =
//   D_UNIT_AMPERE "|"
//   D_UNIT_HOUR "|"
//   D_UNIT_KILOOHM "|"
//   D_UNIT_KILOWATTHOUR "|"
//   D_UNIT_LUX "|"
//   D_UNIT_MICROSECOND "|"
//   D_UNIT_MILLIAMPERE "|"
//   D_UNIT_MILLIMETER_MERCURY "|"
//   D_UNIT_MILLISECOND "|"
//   D_UNIT_MINUTE "|"
//   D_UNIT_PARTS_PER_BILLION "|"
//   D_UNIT_PARTS_PER_DECILITER "|"
//   D_UNIT_PARTS_PER_MILLION "|"
//   "%|"
//   D_UNIT_PRESSURE "|"
//   D_UNIT_SECOND "|"
//   D_UNIT_SECTORS "|"
//   D_UNIT_VOLT "|"
//   D_UNIT_WATT "|"
//   D_UNIT_WATTHOUR "|"
//   D_UNIT_HERTZ ;

// const char S_JSON_COMMAND_NVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%d %s\"}";
// const char S_JSON_COMMAND_LVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%lu %s\"}";
// const char S_JSON_COMMAND_SVALUE_SPACE_UNIT[] PROGMEM =       "{\"%s\":\"%s %s\"}";
// const char S_JSON_COMMAND_NVALUE_UNIT[] PROGMEM =             "{\"%s\":\"%d%s\"}";
// const char S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT[] PROGMEM = "{\"%s\":\"%d%s (%d%s)\"}";

// const char S_JSON_COMMAND_NVALUE_SVALUE[] PROGMEM =           "{\"%s\":\"%d (%s)\"}";
// const char S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE[] PROGMEM =    "{\"%s\":\"%d (" D_ACTIVE " %d)\"}";

// const char S_JSON_COMMAND_NVALUE[] PROGMEM =                  "{\"%s\":%d}";
#define    D_COMMAND_NVALUE                              "{\"%s\":%d}"
// #define    D_COMMAND_NVALUE_P                            "{\"%S\":%d}"
// #define    D_COMMAND_NVALUE_K(KEY)                       "{\"" KEY "\":%d}"

// #define    D_COMMAND_FVALUE                              "{\"%s\":%.1f}"
// const char S_JSON_COMMAND_LVALUE[] PROGMEM =                  "{\"%s\":%lu}";
// const char S_JSON_COMMAND_FVALUE[] PROGMEM =                  "{\"%s\":%.1f}";
// const char S_JSON_COMMAND_SVALUE[] PROGMEM =                  "{\"%s\":\"%s\"}";
#define    D_COMMAND_SVALUE                              "{\"%s\":\"%s\"}"
// #define    D_COMMAND_SVALUE_P                            "{\"%S\":\"%s\"}"


#define    D_COMMAND_SVALUE_K(KEY)                       "{\"" KEY ":\"%s\"}"

// #define    D_COMMAND_SVALUE_K1                       "{\""
// #define    D_COMMAND_SVALUE_K2                       ":\"%s\"}"


const char S_JSON_COMMAND_SVALUE_SVALUE[] PROGMEM =           "{\"%s\":{\"%s\":\"%s\"}}";
#define    D_COMMAND_SVALUE_SVALUE                       "{\"%s\":{\"%s\":\"%s\"}}"
#define    D_COMMAND_SVALUE_SVALUE_K(KEY1, KEY2)         "{\"" KEY1 "\":{\"" KEY2 "\":\"%s\"}}"

const char S_JSON_COMMAND_SVALUE_NVALUE[] PROGMEM =           "{\"%s\":{\"%s\":%d}}";
const char PM_KEYS_JSON_COMMAND_SVALUE_NVALUE[] PROGMEM =           "{\"%S\":{\"%S\":%d}}";

const char PM_COMMAND_PM_SVALUE_NVALUE[] PROGMEM =           "{\"%S\":%d}";
const char PM_COMMAND_PM_SVALUE_SVALUE_NVALUE[] PROGMEM =    "{\"%S\":{\"%S\":%d}}";
const char PM_COMMAND_PM_SVALUE_SVALUE_SVALUE[] PROGMEM =    "{\"%S\":{\"%S\":\"%S\"}}";
const char PM_COMMAND_SVALUE_NVALUE[] PROGMEM =           "{\"%s\":%d}";
const char PM_COMMAND_SVALUE_SVALUE_NVALUE[] PROGMEM =    "{\"%s\":{\"%s\":%d}}";
const char PM_COMMAND_SVALUE_SVALUE_SVALUE[] PROGMEM =    "{\"%s\":{\"%s\":\"%s\"}}";


#define    D_COMMAND_SVALUE_NVALUE                       "{\"%s\":{\"%s\":%d}}"
#define    D_COMMAND_SVALUE_NVALUE_K(KEY1, KEY2)         "{\"" KEY1 "\":{\"" KEY2 "\":%d}}"


#define    D_COMMAND_NVALUE_K(KEY1)                "{\"" KEY1 "\":%d}"
#define    D_COMMAND_2KEY_NVALUE_K(KEY1, KEY2)     "{\"" KEY1 "\":{\"" KEY2 "\":%d}}"



const char S_JSON_COMMAND_ASTERIX[] PROGMEM =                 "{\"%s\":\"" D_ASTERIX "\"}";
const char S_JSON_COMMAND_XVALUE[] PROGMEM =                  "{\"%s\":%s}";  // %s must provide quotes on non-number


// #define    D_KEYVALUE_NVALUE                             "\"%s\":%d"
// DEFINE_PGM_CTR(PM_KEYVALUE_NVALUE)                   D_KEYVALUE_NVALUE;
// #define    D_KEYVALUE_SVALUE                             "\"%s\":\"%s\""
// DEFINE_PGM_CTR(PM_KEYVALUE_SVALUE)                   D_KEYVALUE_SVALUE;
// #define    D_KEYVALUE_FVALUE                             "\"%s\":%s"
// DEFINE_PGM_CTR(PM_KEYVALUE_FVALUE)                   D_KEYVALUE_FVALUE;



#define    D_FUNCTION_NAME_SVALUE                              "f::\"%s\""

// const char S_JSON_COMMAND_INDEX_NVALUE[] PROGMEM =            "{\"%s%d\":%d}";
// const char S_JSON_COMMAND_INDEX_LVALUE[] PROGMEM =            "{\"%s%d\":%lu}";
// const char S_JSON_COMMAND_INDEX_SVALUE[] PROGMEM =            "{\"%s%d\":\"%s\"}";
// const char S_JSON_COMMAND_INDEX_ASTERIX[] PROGMEM =           "{\"%s%d\":\"" D_ASTERIX "\"}";
// const char S_JSON_COMMAND_INDEX_SVALUE_SVALUE[] PROGMEM =     "{\"%s%d\":\"%s%s\"}";
// const char S_JSON_COMMAND_INDEX_NVALUE_ACTIVE_NVALUE[] PROGMEM = "{\"%s%d\":\"%d (" D_ACTIVE " %d)\"}";

// const char S_JSON_SENSOR_INDEX_NVALUE[] PROGMEM =             "{\"" D_SENSOR "%d\":%d}";
// const char S_JSON_SENSOR_INDEX_SVALUE[] PROGMEM =             "{\"" D_SENSOR "%d\":\"%s\"}";

// const char S_JSON_DRIVER_INDEX_NVALUE[] PROGMEM =             "{\"" D_DRIVER "%d\":%d}";
// const char S_JSON_DRIVER_INDEX_SVALUE[] PROGMEM =             "{\"" D_DRIVER "%d\":\"%s\"}";

// const char JSON_SNS_TEMP[] PROGMEM = ",\"%s\":{\"" D_TEMPERATURE "\":%s}";
// const char JSON_SNS_TEMPHUM[] PROGMEM = ",\"%s\":{\"" D_TEMPERATURE "\":%s,\"" D_HUMIDITY "\":%s}";

// const char JSON_SNS_GNGPM[] PROGMEM = ",\"%s\":{\"" D_TOTAL_USAGE "\":%s,\"" D_FLOWRATE "\":%s}";

const char S_LOG_I2C_FOUND_AT[] PROGMEM = D_LOG_I2C "%s " D_FOUND_AT " 0x%x";

const char S_JSON_SVALUE_ACTION_SVALUE[] PROGMEM =            "{\"%s\":{\"Action\":\"%s\"}}";

const char S_LOG_HTTP[] PROGMEM = D_LOG_HTTP;
const char S_LOG_WIFI[] PROGMEM = D_LOG_WIFI;
// const char S_LOG_MQTT[] PROGMEM = D_LOG_MQTT;
// const char S_RSLT_POWER[] PROGMEM = D_RSLT_POWER;
// const char S_RSLT_RESULT[] PROGMEM = D_RSLT_RESULT;
// const char S_RSLT_WARNING[] PROGMEM = D_RSLT_WARNING;
// const char S_LWT[] PROGMEM = D_LWT;
// const char S_OFFLINE[] PROGMEM = D_OFFLINE;

// sonoff.ino
// #define MAX_BUTTON_COMMANDS  5  // Max number of button commands supported
// const char kCommands[MAX_BUTTON_COMMANDS][14] PROGMEM = {
//   D_WIFICONFIG " 1",   // Press button three times
//   D_WIFICONFIG " 2",   // Press button four times
//   D_WIFICONFIG " 3",   // Press button five times
//   D_RESTART " 1",      // Press button six times
//   D_UPGRADE " 1" };    // Press button seven times
// #ifdef ESP8266
// const char kWifiConfig[MAX_WIFI_OPTION][WCFG_MAX_STRING_LENGTH] PROGMEM = {
//   D_WCFG_0_RESTART,
//   D_WCFG_1_SMARTCONFIG,
//   D_WCFG_2_WIFIMANAGER,
//   D_WCFG_3_WPSCONFIG,
//   D_WCFG_4_RETRY,
//   D_WCFG_5_WAIT,
//   D_WCFG_6_SERIAL,
//   D_WCFG_7_WIFIMANAGER_RESET_ONLY };
//   #endif
// const char kPrefixes[3][PRFX_MAX_STRING_LENGTH] PROGMEM = {
//   D_CMND,
//   D_STAT,
//   D_TELE };



// support.ino
static const char kMonthNames[] PROGMEM = D_MONTH3LIST;

DEFINE_PGM_CTR(kOptionOff)        "OFF|" "0|" D_OFF "|" D_FALSE "|" D_STOP "|" D_OFF ;
DEFINE_PGM_CTR(kOptionOn)         "ON|"  "1|" D_ON "|" D_TRUE "|" D_START "|" D_ON "|" D_USER ;
DEFINE_PGM_CTR(kOptionToggle)     "TOGGLE|" "2|" D_TOGGLE "|" "OnChanged";
DEFINE_PGM_CTR(kOptionBlink)      "BLINK|" D_BLINK ;
DEFINE_PGM_CTR(kOptionBlinkOff)   "BLINKOFF|" D_BLINKOFF ;
DEFINE_PGM_CTR(kOptionIncrement)        "Increment|" "Increase|+|plus|" D_CYCLE ;
DEFINE_PGM_CTR(kOptionDecrement)        "Decrement|" "Decrease|-|minus";

DEFINE_PGM_CTR(kOptionFollow)        "Follow";
DEFINE_PGM_CTR(kOptionFollowInv)        "FollowInv|Inverted";




// xdrv_02_webserver.ino
// #ifdef USE_MODULE_NETWORK_WEBSERVER
// const char HTTP_SNS_TEMP[] PROGMEM = "{s}%s " D_TEMPERATURE "{m}%s&deg;%c{e}";                    // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_HUM[] PROGMEM = "{s}%s " D_HUMIDITY "{m}%s%%{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_PRESSURE[] PROGMEM = "{s}%s " D_PRESSURE "{m}%s %s{e}";                       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_SEAPRESSURE[] PROGMEM = "{s}%s " D_PRESSUREATSEALEVEL "{m}%s %s{e}";          // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_ANALOG[] PROGMEM = "{s}%s " D_ANALOG_INPUT "%d{m}%d{e}";                      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_ILLUMINANCE[] PROGMEM = "{s}%s " D_ILLUMINANCE "{m}%d " D_UNIT_LUX "{e}";     // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_CO2[] PROGMEM = "{s}%s " D_CO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}";       // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_CO2EAVG[] PROGMEM = "{s}%s " D_ECO2 "{m}%d " D_UNIT_PARTS_PER_MILLION "{e}";  // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_GALLONS[] PROGMEM = "{s}%s " D_TOTAL_USAGE "{m}%s " D_UNIT_GALLONS " {e}";    // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_GPM[] PROGMEM = "{s}%s " D_FLOW_RATE "{m}%s " D_UNIT_GALLONS_PER_MIN" {e}";   // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
// const char HTTP_SNS_DISTANCE[] PROGMEM = "{s}%s " "Distance" "{m}%s%s{e}";                             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

// const char HTTP_SNS_GENERIC[] PROGMEM = "{s}%s{m}%s{e}";
// const char HTTP_SNS_GENERIC_NVALUE[] PROGMEM = "{s}%s{m}%d{e}";
// const char HTTP_SNS_GENERIC_SVALUE[] PROGMEM = "{s}%s{m}%s{e}";
// const char HTTP_SNS_GENERIC_UNIT[] PROGMEM = "{s}%s{m}%s %s{e}";
// const char HTTP_SNS_GENERIC_NVALUE_UNIT[] PROGMEM = "{s}%s{m}%d %s{e}";
// const char HTTP_SNS_GENERIC_SVALUE_UNIT[] PROGMEM = "{s}%s{m}%s %s{e}";
 
// const char HTTP_SNS_VOLTAGE[] PROGMEM = "{s}" D_VOLTAGE "{m}%s " D_UNIT_VOLT "{e}";
// const char HTTP_SNS_CURRENT[] PROGMEM = "{s}" D_CURRENT "{m}%s " D_UNIT_AMPERE "{e}";
// const char HTTP_SNS_POWER[] PROGMEM = "{s}" D_POWERUSAGE "{m}%s " D_UNIT_WATT "{e}";
// const char HTTP_SNS_ENERGY_TOTAL[] PROGMEM = "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";


// const char S_MAIN_MENU[] PROGMEM = D_MAIN_MENU;
// const char S_CONFIGURATION[] PROGMEM = D_CONFIGURATION;
// const char S_SYSTEM_SETTINGS[] PROGMEM = D_SYSTEM_SETTINGS;
// const char S_CONFIGURE_TEMPLATE[] PROGMEM = D_CONFIGURE_TEMPLATE;
// const char S_CONFIGURE_MODULE[] PROGMEM = D_CONFIGURE_MODULE;
// const char S_COLOUR_PALETTE_EDITOR[] PROGMEM = D_COLOUR_PALETTE_EDITOR;
// const char S_COLOUR_MIXER_EDITOR[] PROGMEM = D_COLOUR_MIXER_EDITOR;
// const char S_CONFIGURE_WIFI[] PROGMEM = D_CONFIGURE_WIFI;
// const char S_NO_NETWORKS_FOUND[] PROGMEM = D_NO_NETWORKS_FOUND;
// const char S_CONFIGURE_LOGGING[] PROGMEM = D_CONFIGURE_LOGGING;
// const char S_CONFIGURE_OTHER[] PROGMEM = D_CONFIGURE_OTHER;
// const char S_SAVE_CONFIGURATION[] PROGMEM = D_SAVE_CONFIGURATION;
const char S_RESET_CONFIGURATION[] PROGMEM = D_RESET_CONFIGURATION;
const char S_RESTORE_CONFIGURATION[] PROGMEM = D_RESTORE_CONFIGURATION;
// const char S_FIRMWARE_UPGRADE[] PROGMEM = D_FIRMWARE_UPGRADE;
// const char S_CONSOLE[] PROGMEM = D_CONSOLE;
// const char PM_INFORMATION[] PROGMEM = D_INFORMATION;
const char S_RESTART[] PROGMEM = D_RESTART;
// #endif  // USE_MODULE_NETWORK_WEBSERVER



#endif