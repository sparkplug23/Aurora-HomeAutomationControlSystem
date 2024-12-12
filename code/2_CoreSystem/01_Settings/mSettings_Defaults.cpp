#include "2_CoreSystem/01_Settings/mSettings.h"

void mSettings::SettingsInit(void)
{
  
  DeviceNameListI->Init(
    Settings.device_name_buffer.name_buffer,
    sizeof(Settings.device_name_buffer.name_buffer),
    Settings.device_name_buffer.class_id,
    Settings.device_name_buffer.device_id,
    DEVICENAMEBUFFER_NAME_INDEX_LENGTH
  );

}


void mSettings::SettingsDefault(void)
{
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR(D_LOG_CONFIG D_USE_DEFAULTS));
  #endif // ENABLE_LOG_LEVEL_COMMANDS

  Settings.flag_system.stop_flash_rotate = true;
  runtime.stop_flash_rotate = true;

  // Erase ALL settings to 0
  memset(&Settings, 0x00, sizeof(SETTINGS));
DEBUG_LINE_HERE2

  SystemSettings_DefaultHeader();
  DEBUG_LINE_HERE2
  
  SystemSettings_DefaultBody();
DEBUG_LINE_HERE2

  // Clear module defaults  
  pCONT->Tasker_Interface(TASK_SETTINGS_DEFAULT); // replace with below?
  DEBUG_LINE_HERE;
  
  pCONT->Tasker_Interface(TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT);
      
  DEBUG_LINE_HERE;
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_MEMORY D_LOAD " %s %d %d"), "SettingsDefault",Settings.cfg_holder,SETTINGS_HOLDER);
  #endif// ENABLE_LOG_LEVEL_INFO

  DEBUG_LINE_HERE;
  // After defaults are loaded everything should immediately be saved
  SettingsSaveAll();
   
  DEBUG_LINE_HERE;
    
}

void mSettings::SystemSettings_DefaultHeader(void)
{

  Settings.cfg_holder = (uint16_t)SETTINGS_HOLDER;
  Settings.cfg_size = sizeof(SETTINGS);
  Settings.save_flag = 0;
  Settings.version = PROJECT_VERSION;
  Settings.bootcount = 0;

  #ifdef ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  snprintf(Settings.settings_holder_ctr, sizeof(Settings.settings_holder_ctr), "%d", Settings.cfg_holder);
  #endif

}


void mSettings::SystemSettings_DefaultBody(void)
{

  /*********************************************************************************************
   ******* CoreSystem **************************************************************************
   *********************************************************************************************/

  Settings.bootcount_errors_only = 0;     // E01
  
  Settings.save_data = SAVE_DATA;
  Settings.enable_sleep = true; //on
  Settings.sleep = DEFAULT_LOOP_SLEEP;
  
  runtime.stop_flash_rotate = true;//Settings.flag_system.stop_flash_rotate;

  runtime.sleep = Settings.sleep;
  Settings.flag_system.save_state = SAVE_STATE;

  Settings.unified_interface_reporting_invalid_reading_timeout_seconds = UNIFIED_INTERFACE_REPORTING_INVALID_SECONDS;
  
 
  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2


  /*********************************************************************************************
   ******* Module/Template/GPIO ***********************************************************************
   *********************************************************************************************/

  snprintf(pCONT_set->Settings.system_name.device,sizeof(pCONT_set->Settings.system_name.device),"%s","fallback");
  snprintf(pCONT_set->Settings.system_name.friendly,sizeof(pCONT_set->Settings.system_name.friendly),"%s","fallback");
  snprintf(pCONT_set->Settings.room_hint,sizeof(pCONT_set->Settings.room_hint),"%s","none_set");
  
  Settings.module = MODULE;
  Settings.last_module = MODULE;
  
  #ifndef FIRMWARE_MINIMAL    // not needed in minimal/safeboot because of disabled feature and Settings are not saved anyways

  #endif
  // ModuleDefault(WEMOS);  // to do
  for (uint8_t i = 0; i < sizeof(Settings.module_pins); i++) { Settings.module_pins.io[i] = GPIO_NONE_ID; }
  SettingsUpdateText(SET_FRIENDLYNAME1, PSTR(DEVICENAME_FRIENDLY_CTR)); // Init
  #ifdef DEVICENAME_CTR
  SettingsUpdateText(SET_DEVICENAME, PSTR(DEVICENAME_CTR));
  #else
  SettingsUpdateText(SET_DEVICENAME, SettingsText(SET_FRIENDLYNAME1));
  #endif

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Serial *****************************************************************************
   *********************************************************************************************/

  Settings.baudrate = APP_BAUDRATE / 1200;
  Settings.serial_delimiter = 0xff;
  Settings.sbaudrate = SOFT_BAUDRATE / 1200;

  /*********************************************************************************************
   ******* Logging *****************************************************************************
   *********************************************************************************************/

  runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  Settings.logging.serial_level = runtime.seriallog_level_during_boot;
  Settings.logging.sys_port = SYS_LOG_PORT;
  Settings.logging.sys_level = SYS_LOG_LEVEL;
  Settings.logging.web_level = WEB_LOG_LEVEL;
  Settings.logging.telnet_level = TELNET_LOG_LEVEL;
  Settings.logging.mqtt_level = LOG_LEVEL_IMPORTANT;
  Settings.logging.time_isshort = LOGTIME_DEFAULT_FORMAT;
  
  SettingsUpdateText(SET_SYSLOG_HOST, PSTR(SYS_LOG_HOST));

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Networking: Wifi/Network/Cellular ***************************************************
   *********************************************************************************************/

  SettingsUpdateText(SET_OTAURL, PSTR(D_OTA_URL));
  
  DEBUG_LINE_HERE

  pCONT_sup->ParseIPv4(&Settings.ipv4_address[0], PSTR(WIFI_IP_ADDRESS));
  DEBUG_LINE_HERE
  pCONT_sup->ParseIPv4(&Settings.ipv4_address[1], PSTR(WIFI_GATEWAY));
  pCONT_sup->ParseIPv4(&Settings.ipv4_address[2], PSTR(WIFI_SUBNETMASK));
  pCONT_sup->ParseIPv4(&Settings.ipv4_address[3], PSTR(WIFI_DNS));
  pCONT_sup->ParseIPv4(&Settings.ipv4_address[4], PSTR(WIFI_DNS2));
  pCONT_sup->ParseIPv4(&Settings.ipv4_rgx_address, PSTR(WIFI_RGX_IP_ADDRESS));
  pCONT_sup->ParseIPv4(&Settings.ipv4_rgx_subnetmask, PSTR(WIFI_RGX_SUBNETMASK));
  
  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  DEBUG_LINE_HERE
  Settings.sta_config = WIFI_CONFIG_TOOL;
  Settings.sta_active = 0;
  SettingsUpdateText(SET_STASSID1, PSTR(STA_SSID1));
  SettingsUpdateText(SET_STASSID2, PSTR(STA_SSID2));
  SettingsUpdateText(SET_STAPWD1, PSTR(STA_PASS1));
  SettingsUpdateText(SET_STAPWD2, PSTR(STA_PASS2));
  SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);
  SettingsUpdateText(SET_RGX_SSID, PSTR(WIFI_RGX_SSID));
  SettingsUpdateText(SET_RGX_PASSWORD, PSTR(WIFI_RGX_PASSWORD));

  Settings.flag_network.network_wifi = 1;
  Settings.flag_network.sleep_normal = true; // USE DYNAMIC sleep

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  Settings.flag_network.timers_enable = 0;
  Settings.flag_network.use_wifi_rescan = 1;
  Settings.flag_system.stop_flash_rotate = true;

  Settings.flag_network.mdns_enabled = 1;
  runtime.mdns_delayed_start = 60;
  sprintf(runtime.my_hostname,"%s",pCONT_set->Settings.system_name.device);

  /*********************************************************************************************
   ******* Networking: WebServer ********************************************************************
   *********************************************************************************************/
  
  Settings.webserver = WEB_SERVER;

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  SettingsUpdateText(SET_WEBPWD, PSTR(WEB_PASSWORD));
  SettingsUpdateText(SET_CORS, PSTR(CORS_DOMAIN));

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Networking: MQTT ********************************************************************
   *********************************************************************************************/

  Settings.flag_system.mqtt_enabled = 1;

  Settings.flag_system.mqtt_enabled = true;
  
  Settings.flag_system.mqtt_enabled = true;
  Settings.flag_system.mqtt_power_retain = 0;//MQTT_POWER_RETAIN;
  Settings.flag_system.mqtt_button_retain = 0;//MQTT_BUTTON_RETAIN;
  Settings.flag_system.mqtt_switch_retain = 0;//MQTT_SWITCH_RETAIN;

  /*********************************************************************************************
   ******* Param Options ***********************************************************************
   *********************************************************************************************/
  
  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;
  Settings.setoption_255[P_HOLD_TIME] = KEY_HOLD_TIME;  // Default 4 seconds hold time
  Settings.setoption_255[P_MAX_POWER_RETRY] = MAX_POWER_RETRY;
  #ifdef USE_NETWORK_MDNS
  Settings.setoption_255[P_MDNS_DELAYED_START] = 0;
  #endif
  Settings.setoption_255[P_BOOT_LOOP_OFFSET] = BOOT_LOOP_OFFSET;

  /*********************************************************************************************
   ******* Time ********************************************************************************
   *********************************************************************************************/

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  if (((APP_TIMEZONE > -14) && (APP_TIMEZONE < 15)) || (99 == APP_TIMEZONE)) {
    Settings.timezone = APP_TIMEZONE;
    Settings.timezone_minutes = 0;
    Settings.timezone2 = APP_TIMEZONE;
    Settings.timezone_minutes2 = 0;
  } else {
    Settings.timezone = APP_TIMEZONE / 60;
    Settings.timezone_minutes = abs(APP_TIMEZONE % 60);
    Settings.timezone2 = APP_TIMEZONE / 60;
    Settings.timezone_minutes2 = abs(APP_TIMEZONE % 60);
  }

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  SettingsResetStd();
  SettingsResetDst();

  #ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
  SettingsUpdateText(SET_NTPSERVER1, PSTR(NTP_SERVER1));
  SettingsUpdateText(SET_NTPSERVER2, PSTR(NTP_SERVER2));
  SettingsUpdateText(SET_NTPSERVER3, PSTR(NTP_SERVER3));
  for (uint32_t i = 0; i < MAX_NTP_SERVERS; i++) {
    SettingsUpdateText(SET_NTPSERVER1 +i, pCONT_sup->ReplaceCommaWithDot(SettingsText(SET_NTPSERVER1 +i)));
  }
  #endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
  
  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Human Interface Devices (HID) e.g. Buttons/Switches *********************************
   *********************************************************************************************/
  
  Settings.button_debounce = KEY_DEBOUNCE_TIME;
  Settings.switch_debounce = SWITCH_DEBOUNCE_TIME;
  #ifdef USE_MODULE_SENSORS_SWITCHES
    for (uint8_t i = 0; i < MAX_SWITCHES_SET; i++) { Settings.switchmode[i] = SWITCH_MODE; }
  #endif
  ALOG_INF(PSTR("Settings.switch_debounce is set --- %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  Settings.flag_system.button_restrict = 0;
  Settings.flag_system.button_swap = 0;
  Settings.flag_system.button_single = 0; // support only single press to support faster button recognition (disable to allow multipress)
  Settings.flag_network.button_switch_force_local =0;

  Settings.flag_system.mqtt_switches = 0; // false default

  ALOG_INF(PSTR("switch_debounce?? %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Sensors *****************************************************************************
   *********************************************************************************************/

  Settings.sensors.altitude = ALTITUDE_ABOVE_SEALEVEL; 
  Settings.sensors.latitude = LATITUDE;//(int)((double)LATITUDE * 1000000);
  Settings.sensors.longitude = LONGITUDE;//(int)((double)LONGITUDE * 1000000);
  Settings.sensors.flags.decimal_precision = 3;
  // Settings.flag_sensor.temperature_conversion = TEMP_CONVERSION;
  // Settings.flag_sensor.pressure_conversion = PRESSURE_CONVERSION;
  // Settings.flag_sensor.pressure_resolution = PRESSURE_RESOLUTION;
  // Settings.flag_sensor.humidity_resolution = HUMIDITY_RESOLUTION;
  // Settings.flag_sensor.temperature_resolution = TEMP_RESOLUTION;
  
  // Some user descriptions of switches or buttons, eg "ON" could be "Door Open"
  SettingsUpdateText(SET_STATE_TXT1, PSTR(D_OFF));
  SettingsUpdateText(SET_STATE_TXT2, PSTR(D_ON));
  SettingsUpdateText(SET_STATE_TXT3, PSTR(D_TOGGLE));
  SettingsUpdateText(SET_STATE_TXT4, PSTR(D_HOLD));

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Drivers *****************************************************************************
   *********************************************************************************************/

  #ifdef SET_POWERONSTATE_AS_ON
    Settings.poweronstate = POWER_ALL_ON; //temporary fix for poweronstate
  #else
    Settings.poweronstate = APP_POWERON_STATE;
  #endif
  
  Settings.ledmask = APP_LEDMASK;
  Settings.ledstate = APP_LEDSTATE;
  Settings.blinktime = APP_BLINKTIME;
  Settings.blinkcount = APP_BLINKCOUNT;
  // uint32_t      monitors;                  // 7A0
  Settings.pwm_frequency = PWM_FREQ;
  Settings.pwm_range = PWM_RANGE;
  // uint16_t      pwm_value[MAX_PWMS];       // 2EC
  Settings.ledpwm_off = 0;
  Settings.ledpwm_on = 255;
  
  Settings.i2c_drivers[0] = I2CDRIVERS_0_31;
  Settings.i2c_drivers[1] = I2CDRIVERS_32_63;
  Settings.i2c_drivers[2] = I2CDRIVERS_64_95;  
  
  Settings.flag_system.pwm_control = 1;

  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
  /*********************************************************************************************
   ******* Energy ******************************************************************************
   *********************************************************************************************/

  Settings.energy_usage.energy_power_delta = DEFAULT_POWER_DELTA;
  Settings.energy_usage.energy_power_calibration = HLW_PREF_PULSE;
  Settings.energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_usage.energy_current_calibration = HLW_IREF_PULSE; 
  Settings.energy_usage.energy_kWhtoday = 0;
  Settings.energy_usage.energy_kWhyesterday = 0;
  Settings.energy_usage.energy_kWhdoy = 0;
  Settings.energy_usage.energy_min_power = 0;
  Settings.energy_usage.energy_max_power = 0;
  Settings.energy_usage.energy_min_voltage = 0;
  Settings.energy_usage.energy_max_voltage = 0;
  Settings.energy_usage.energy_min_current = 0;
  Settings.energy_usage.energy_max_current = 0;
  Settings.energy_usage.energy_max_power_limit = 0;   
  Settings.energy_usage.energy_max_power_limit_hold = MAX_POWER_HOLD;
  Settings.energy_usage.energy_max_power_limit_window = MAX_POWER_WINDOW;
  Settings.energy_usage.energy_max_power_safe_limit = 0;                       // MaxSafePowerLimit
  Settings.energy_usage.energy_max_power_safe_limit_hold = SAFE_POWER_HOLD;
  Settings.energy_usage.energy_max_power_safe_limit_window = SAFE_POWER_WINDOW;
  Settings.energy_usage.energy_max_energy = 0;                                 // MaxEnergy
  Settings.energy_usage.energy_max_energy_start = 0;                           // MaxEnergyStart
  Settings.energy_usage.energy_kWhtotal = 0;

  Settings.flag_power.current_resolution = 3;
  Settings.flag_power.voltage_resolution = 0;
  Settings.flag_power.wattage_resolution = 0;
  Settings.flag_power.energy_resolution = ENERGY_RESOLUTION;
  Settings.energy_usage.energy_power_calibration = HLW_PREF_PULSE;
  Settings.energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  Settings.energy_usage.energy_current_calibration = HLW_IREF_PULSE;

  Settings.flag_power.calc_resolution = CALC_RESOLUTION;

  /*********************************************************************************************
   ******* Display *****************************************************************************
   *********************************************************************************************/
  
  Settings.display.model = 0;
  Settings.display.mode = 0;//EM_DISPLAY_MODE_LOCAL1_ID;
  Settings.display.refresh = 2;
  Settings.display.rows = 4;
  Settings.display.cols[0] = 16;
  Settings.display.cols[1] = 8;
  Settings.display.dimmer = 7;
  Settings.display.size = 2;
  Settings.display.font = 1;
  Settings.display.rotate = 0;
  
  ALOG_INF(PSTR("switch_debounce %d"), Settings.switch_debounce);  DEBUG_LINE_HERE2
}


void mSettings::SettingsResetStd(void)
{
  
  Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  Settings.tflag[0].week  = TIME_STD_WEEK;
  Settings.tflag[0].dow   = TIME_STD_DAY;
  Settings.tflag[0].month = TIME_STD_MONTH;
  Settings.tflag[0].hour  = TIME_STD_HOUR;
  Settings.toffset[0]     = TIME_STD_OFFSET;

}


void mSettings::SettingsResetDst(void)
{
  Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  Settings.tflag[1].week = TIME_DST_WEEK;
  Settings.tflag[1].dow = TIME_DST_DAY;
  Settings.tflag[1].month = TIME_DST_MONTH;
  Settings.tflag[1].hour = TIME_DST_HOUR;
  Settings.toffset[1] = TIME_DST_OFFSET;
}
