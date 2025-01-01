/**
 * @file mRtcSettings.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 * @note RTC Settings are loaded before primary settings from flash are loaded.
 *       Only used for data that is needed quickly during startup
 **/
#include "mRtcSettings.h"

#ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
TRtcFastboot RtcFastboot;
#ifdef ESP32
RTC_NOINIT_ATTR TRtcFastboot RtcDataFastboot;
#endif  // ESP32

uint32_t rtc_fastboot_crc = 0;
const uint16_t RTC_MEM_VALID = 0xA55A; // Value does not matter

uint32_t RtcFastboot_CRC()
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcFastboot;
  for (uint32_t i = 0; i < sizeof(RtcFastboot); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void RtcFastboot_Save(void) 
{

  if (RtcFastboot_CRC() != rtc_fastboot_crc) // ie if the data has changed at all
  {
    #ifdef DEBUG_FASTBOOT
    Serial.println( "if (GetRtcFastbootCrc() != rtc_fastboot_crc) NEW data" );
    #endif
    RtcFastboot.valid = RTC_MEM_VALID;

    #ifdef ESP8266
    ESP.rtcUserMemoryWrite(100 - sizeof(RtcFastboot), (uint32_t*)&RtcFastboot, sizeof(RtcFastboot));
    #endif  // ESP8266
    #ifdef ESP32
    RtcDataFastboot = RtcFastboot;
    #endif  // ESP32

    rtc_fastboot_crc = RtcFastboot_CRC();
  }
  else
  {
    #ifdef DEBUG_FASTBOOT
    Serial.println( "ELSE if (GetRtcFastbootCrc() != rtc_fastboot_crc)" );
    #endif
  }

}

/**
 * @brief Used to ensure the next reboot does not continue with previous fastboot count
 **/
void RtcFastboot_Reset(void) 
{
  RtcFastboot.fast_reboot_count = 0;
  RtcFastboot_Save();
}

/**
 * @brief Load Reboot data, if not valid then reset it and save
 **/
void RtcFastboot_Load(void) 
{

  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcRebootLoad START");
  #endif

  #ifdef ESP8266
    ESP.rtcUserMemoryRead(100 - sizeof(RtcFastboot), (uint32_t*)&RtcFastboot, sizeof(RtcFastboot));  // 0x280
  #endif  // ESP8266
  #ifdef ESP32
    RtcFastboot = RtcDataFastboot; // Set the pointer so the struct points to the data saved
  #endif  // ESP32

  if (RtcFastboot.valid != RTC_MEM_VALID) 
  {

    #ifdef DEBUG_FASTBOOT
    Serial.println("RtcReboot.valid != RTC_MEM_VALID");
    #endif
  
    memset(&RtcFastboot, 0, sizeof(RtcFastboot));
    RtcFastboot.valid = RTC_MEM_VALID;
    
    RtcFastboot_Save();
    
  }
  
  rtc_fastboot_crc = RtcFastboot_CRC();
  
  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcRebootLoad DONE");
  #endif
  
}

/**
 * @brief Check if the fastboot data saved contains the known value of 'RTC_MEM_VALID'
 * 
 * @return true 
 * @return false 
 */
bool RtcFastboot_Valid(void)
{
  #ifdef DEBUG_FASTBOOT
  Serial.printf("!RtcRebootValid() reset fastbot count\n\r", RTC_MEM_VALID == RtcFastboot.valid);
  #endif
  return (RTC_MEM_VALID == RtcFastboot.valid);
}

#else

const uint16_t RTC_MEM_VALID = 0xA55A; // Value does not matter


#endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION


/***********************************************************************************************************************************************************
 * *********************************************************************************************************************************************************
 * *********************************************************************************************************************************************************
 * *********************************************************************************************************************************************************
 * *********************************************************************************************************************************************************
 * *********************************************************************************************************************************************************/

#ifdef ENABLE_DEVFEATURE_RTC_SETTINGS

TRtcSettings RtcSettings;
#ifdef ESP32
RTC_NOINIT_ATTR TRtcSettings RtcDataSettings;
#endif  // ESP32

uint32_t rtc_settings_crc = 0;

uint32_t GetRtcSettingsCrc(void) 
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint32_t i = 0; i < sizeof(RtcSettings); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

/**
 * @brief Not threadsafe, only called after device has been succesfully started
 **/
void RtcSettingsSave(void) 
{

  // Check at least the first module has been initialised (change to function later, to check variable)  
  if(!pCONT->pModule.size()){ return; }
  
  RtcSettings.baudrate = tkr_set->Settings.baudrate * 300;
  if (tkr_time->GetUTCTime() > START_VALID_UTC_TIME) {  // 2016-01-01
    RtcSettings.utc_time = tkr_time->GetUTCTime();
  }
  if (GetRtcSettingsCrc() != rtc_settings_crc) {

    if (RTC_MEM_VALID != RtcSettings.valid) {
      memset(&RtcSettings, 0, sizeof(RtcSettings));
      RtcSettings.valid = RTC_MEM_VALID;
      // RtcSettings.energy_kWhtoday = Settings->energy_kWhtoday;
      // RtcSettings.energy_kWhtotal = Settings->energy_kWhtotal;
      // for (uint32_t i = 0; i < 3; i++) {
      //   RtcSettings.energy_kWhtoday_ph[i] = Settings->energy_kWhtoday_ph[i];
      //   RtcSettings.energy_kWhtotal_ph[i] = Settings->energy_kWhtotal_ph[i];
      //   RtcSettings.energy_kWhexport_ph[i] = Settings->energy_kWhexport_ph[i];
      // }
      // RtcSettings.energy_usage = Settings->energy_usage;
      // for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
      //   RtcSettings.pulse_counter[i] = Settings->pulse_counter[i];
      // }
      // RtcSettings.power = Settings->power;
  //    RtcSettings.baudrate = Settings->baudrate * 300;
      RtcSettings.baudrate = APP_BAUDRATE;
    }

#ifdef ESP8266
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RtcSettings));
#endif  // ESP8266
#ifdef ESP32
    RtcDataSettings = RtcSettings;
#endif  // ESP32

    rtc_settings_crc = GetRtcSettingsCrc();
  }
}


bool RtcSettingsLoad(uint32_t update) {
#ifdef ESP8266
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RtcSettings));  // 0x290
#endif  // ESP8266
#ifdef ESP32
  RtcSettings = RtcDataSettings;
#endif  // ESP32

  bool read_valid = (RTC_MEM_VALID == RtcSettings.valid);
  if (update) {
    if (!read_valid) {
      RtcSettingsSave();
    }
  }
  return read_valid;
}

bool RtcSettingsValid(void) {
  return (RTC_MEM_VALID == RtcSettings.valid);
}

#endif // ENABLE_DEVFEATURE_RTC_SETTINGS