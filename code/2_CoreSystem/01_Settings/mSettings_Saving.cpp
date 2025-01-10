
#include "2_CoreSystem/01_Settings/mSettings.h"

#include "2_CoreSystem/06_Support/mSupport.h"


#define EEPROM_LOCATION (SPI_FLASH_SEC_SIZE * 200)
uint32_t settings_location = EEPROM_LOCATION;
uint32_t settings_crc32 = 0;
const uint8_t CFG_ROTATES = 7;      // Number of flash sectors used (handles uploads)


void mSettings::SettingsSaveAll(void)
{
  #ifndef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  ALOG_ERR(PSTR(D_LOG_SETTINGS "SettingsSaveAll: Not enabled"));
  #endif

  if (Settings.flag_system.save_state) {
    Settings.power = runtime.power;
  } else {
    Settings.power = 0;
  }
  pCONT->Tasker_Interface(TASK_SETTINGS_SAVE_VALUES_FROM_MODULE);
  
  SettingsSave(0);

}



/***
 * Modified version: 
 * - Removing rotating writing until location0 works well
 * - Adding that settings first gets read into a local temporary copy, then a version merges the loaded values into values I want to overwrite.
 *    This is so I can slowly add what I like to have saved in memory until I can trust it full
 * */
void mSettings::SettingsLoad(void) 
{
  
  DEBUG_LINE_HERE
  #ifndef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  ALOG_ERR(PSTR(D_LOG_SETTINGS "SettingsLoad: Not enabled"));
  #endif

  ALOG_INF(PSTR(D_LOG_MEMORY D_LOAD));

  /****************************************************************************************************************************************************
  ** Check if SETTINGS_HOLDER value has changed from code build, and thus stored settings should be erased ********************************************
  ****************************************************************************************************************************************************/
  
  #ifdef ESP8266  
    // Load configuration from optional file and flash (eeprom and 7 additonal slots) if first valid load does not stop_flash_rotate
    // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
    settings_location = 0;
    uint32_t save_flag = 0;
    uint32_t max_slots = CFG_ROTATES +1;
    uint32_t flash_location;
    uint32_t slot = 1;
    #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
      if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings))) {
        flash_location = 1;
        slot = 0;
      }
    #endif  // ENABLE_DEVFEATURE_SETTINGS__TFS
    while (slot <= max_slots) {                                  // Read all config pages in search of valid and latest
      if (slot > 0) {
        flash_location = (1 == slot) ? EEPROM_LOCATION : (2 == slot) ? SETTINGS_LOCATION : flash_location -1;
        ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SETTINGS));
      }
      if ((Settings.cfg_crc32 != 0xFFFFFFFF) && (Settings.cfg_crc32 != 0x00000000) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
        if (Settings.save_flag > save_flag) {                    // Find latest page based on incrementing save_flag
          save_flag = Settings.save_flag;
          settings_location = flash_location;
          if (Settings.flag_system.stop_flash_rotate && (1 == slot)) {  // Stop if only eeprom area should be used and it is valid
            break;
          }
        }
      }
      slot++;
      delay(1);
    }
    if (settings_location > 0) {
      #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
      if (1 == settings_location) {
        TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings));
        ALOG_INF(PSTR(D_LOG_CONFIG "Loaded from File, " D_COUNT " %lu"), Settings->save_flag);
      } else
      #endif  // ENABLE_DEVFEATURE_SETTINGS__TFS
      {
        ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SETTINGS));
        ALOG_INF(PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu"), settings_location, Settings.save_flag);
      }
    }
  #endif // ESP8266
  #ifdef ESP32
    // No temp settings, just fully load as if invalid reset will occur and defaults will be loaded
    uint32_t source = SettingsRead(&Settings, sizeof(SETTINGS));
    if (source) {
      DEBUG_LINE_HERE
      settings_location = 1;
      if (Settings.cfg_holder == (uint16_t)SETTINGS_HOLDER) {
        ALOG_INF(PSTR(D_LOG_CONFIG "Loaded from %s, " D_COUNT " %lu"), (2 == source)?"File":"NVS", Settings.save_flag);
      }
    }
  #endif  // ESP32

  /****************************************************************************************************************************************************
  ** If stored settings should be erased to defaults (using file or code inits) ********************************************
  ****************************************************************************************************************************************************/

      DEBUG_LINE_HERE

  bool settings_reset_by_defaults = true;
  bool forced_default_load = false;

  ALOG_INF(PSTR("LoadCheck Loc %d, cfgH %d, defH %d"), settings_location, Settings.cfg_holder, (uint16_t)SETTINGS_HOLDER);

  #ifdef ENABLE_FEATURE_SETTINGS__LOAD_PRECODED_SETTINGS_ON_BOOT_NO_SAVED_STATES
  ALOG_INF(PSTR("Loading precoded defaults, no saved settings"));
  settings_location = 0; // reset to cause default below
  forced_default_load = true; // force override of any loaded settings to ensure known state on boot
  #endif

  if ((0 == settings_location) || (Settings.cfg_holder != (uint16_t)SETTINGS_HOLDER)) 
  {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
    /**
     * @brief Two cases may be true here:
     *  (1) IF the original settings file is correct, but no code change occured. THEN use backup settings file to recover, but cfg_size must still match
     *  (2) IF the settings holder has been changed to indicate code change, THEN use code defaults and ignored saved settings. This is essentially a factory reset.
     */
    
      DEBUG_LINE_HERE
    #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
    /**
     * Load as temporary settings, to enable testing of cfg_size 
     **/
    SETTINGS settings_temp_load;
    if (pCONT_mfile->TfsLoadFile(TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD, (uint8_t*)&settings_temp_load, sizeof(SETTINGS))) 
    {
      DEBUG_LINE_HERE
      if(settings_temp_load.cfg_size == sizeof(SETTINGS))
      {
      DEBUG_LINE_HERE
        ALOG_INF(PSTR(D_LOG_CONFIG "Loaded from LKG File, " D_COUNT " %lu"), Settings.save_flag);
        memcpy(&Settings, &settings_temp_load, sizeof(SETTINGS));
        settings_reset_by_defaults = false;
      }
      else
      {
      DEBUG_LINE_HERE
        AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_CONFIG "LKG Settings size does not match (%d|%d), load invalid"), settings_temp_load.cfg_size, sizeof(SETTINGS));
        settings_reset_by_defaults = true;
      }
    } 
    #endif  // ENABLE_DEVFEATURE_SETTINGS__TFS

    if(settings_reset_by_defaults || forced_default_load)
    {
      AddLog(LOG_LEVEL_HIGHLIGHT, PSTR(D_LOG_CONFIG "Settings reseting to DEFAULT"));
      SettingsDefault();
      DEBUG_LINE_HERE
      runtime.settings_holder_hardcorded_stored_changed = true;
    }

  }

  settings_crc32 = GetSettingsCrc32();

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsLoad(1);
  #endif // ENABLE_DEVFEATURE_RTC_SETTINGS
  
  // Configure hostname 
  memset(runtime.my_hostname,0,sizeof(runtime.my_hostname));
  sprintf(runtime.my_hostname,PSTR("%s"),tkr_set->Settings.system_name.device);

}



uint32_t mSettings::SettingsRead(void *data, size_t size) 
{
  #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
  if (pCONT_mfile->TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)data, size)) 
  {
    return 2;
  }
  #endif
  #ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
  if (NvmLoad("main", "Settings", data, size)) {
    return 1;
  };
  #endif
  return 0;
}


void mSettings::SettingsWrite(const void *pSettings, unsigned nSettingsLen) 
{

  #ifdef ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  snprintf(Settings.local_time_ascii_debug, sizeof(Settings.local_time_ascii_debug), tkr_time->GetDateAndTime(DT_LOCAL).c_str() );
  #endif

  #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
  pCONT_mfile->TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)pSettings, nSettingsLen);
  #endif
  
  DEBUG_LINE_HERE
  #ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
  NvmSave("main", "Settings", pSettings, nSettingsLen);
  #endif

}

/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart. Required for OTA to free up the space
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
void mSettings::SettingsSave(uint8_t rotate)
{

  #ifndef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  ALOG_ERR(PSTR(D_LOG_SETTINGS "SettingsSave: Not enabled"));
  #endif

  if ((GetSettingsCrc32() != settings_crc32) || rotate) 
  {
    if (1 == rotate) {                                 // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
      runtime.stop_flash_rotate = 1;
    }

    // Saved into one place, to reduce footprint
    if (runtime.stop_flash_rotate || (2 == rotate)) {  // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
      settings_location = EEPROM_LOCATION;
    } 
    // Reduce wear by rotating data
    else {                                           // Rotate flash slots
      if (settings_location == EEPROM_LOCATION) {
        settings_location = SETTINGS_LOCATION;
      } else {
        settings_location--;
      }
      if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
        settings_location = EEPROM_LOCATION;
      }
    }

    Settings.save_flag++;
    if (tkr_time->Rtc.utc_time > START_VALID_UTC_TIME) {
      Settings.cfg_timestamp = tkr_time->Rtc.utc_time;
    } else {
      Settings.cfg_timestamp++;
    }

    Settings.cfg_size = sizeof(SETTINGS);
    Settings.cfg_crc32 = GetSettingsCrc32();

    #ifdef USE_COUNTER
      CounterInterruptDisable(true);  // may be to disable all interrupts?
    #endif

    #ifdef ESP8266
    #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
      pCONT_mfile->TfsSaveFile(TASM_FILE_SETTINGS, &Settings, sizeof(SETTINGS));
    #endif  // ENABLE_DEVFEATURE_SETTINGS__TFS
      if (ESP.flashEraseSector(settings_location)) {
        ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SETTINGS));
      }

      if (!Settings.flag_system.stop_flash_rotate && rotate) {  // SetOption12 - (Settings) Switch between dynamic (0) or fixed (1) slot flash save location
        for (uint32_t i = 0; i < CFG_ROTATES; i++) {
          ESP.flashEraseSector(SETTINGS_LOCATION -i);    // Delete previous configurations by resetting to 0xFF
          delay(1);
        }
      }
      ALOG_DBG(PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(SETTINGS));
    #endif  // ESP8266
    #ifdef ESP32
      SettingsWrite(&Settings, sizeof(SETTINGS));
      ALOG_DBG(PSTR(D_LOG_CONFIG "Saved, " D_COUNT " %d, " D_BYTES " %d"), Settings.save_flag, sizeof(SETTINGS));
    #endif  // ESP32

    settings_crc32 = Settings.cfg_crc32;
  }

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsSave();
  #endif
  
}


uint32_t mSettings::GetCfgCrc32(uint8_t *bytes, uint32_t size)
{
  // https://create.stephan-brumme.com/crc32/#bitwise
  uint32_t crc = 0;
  while (size--) {
    crc ^= *bytes++;
    for (uint32_t j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320);
    }
  }
  return ~crc;
}

uint32_t mSettings::GetSettingsCrc32(void)
{
  return GetCfgCrc32((uint8_t*)&Settings, sizeof(Settings) -4);  // Skip crc32
}


/**
 * Test load and print critical test points from primary storage location
 * */
void mSettings::TestSettingsLoad()
{
  #ifdef ESP8266
  SETTINGS settings_tmp;

  settings_location = SETTINGS_LOCATION;
  
  ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&settings_tmp, sizeof(settings_tmp));

  ALOG_TST(PSTR("TestSettingsLoad \t\t\t\t TestSettingsLoad\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
      settings_tmp.cfg_holder,
      settings_tmp.cfg_size,
      settings_tmp.save_flag,
      settings_tmp.version,
      settings_tmp.bootcount,
      0
  );
  #endif // ESP8266
  
}


void mSettings::TestSettings_ShowLocal_Header()
{

  ALOG_TST(PSTR("TestSettings_ShowLocal_Header\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
      Settings.cfg_holder,
      Settings.cfg_size,
      Settings.save_flag,
      Settings.version,
      Settings.bootcount,
      0
  );
  
}


uint32_t mSettings::GetSettingsAddress(void)
{
  return settings_location * SPI_FLASH_SEC_SIZE;
}

void mSettings::SettingsErase(uint8_t type)
{
  // SDK and System data is held in default NVS partition
  // System data is held also in file /.settings on default filesystem
  // cal_data - SDK PHY calibration data as documented in esp_phy_init.h
  // qpc      - System Quick Power Cycle state
  // main     - System Settings data
  /*
    0 = Erase from program end until end of physical flash
    1 = Erase SDK parameter area at end of linker memory model (0x0FDxxx - 0x0FFFFF) solving possible wifi errors
  */

#ifdef ESP32

  int32_t r1, r2, r3 = 0;
  switch (type) {
    case 0:               // Reset 2 = Erase all flash from program end to end of physical flash
    case 2:               // Reset 5, 6 = Erase all flash from program end to end of physical flash excluding filesystem
      #ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
      //      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
      #endif
      #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
      r3 = pCONT_mfile->TfsDeleteFile(TASM_FILE_SETTINGS);
      #endif
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
    case 1:               // Reset 3 = SDK parameter area
    case 4:               // WIFI_FORCE_RF_CAL_ERASE = SDK parameter area
      r1 = esp_phy_erase_cal_data_in_nvs();
      //      r1 = NvmErase("cal_data");
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " PHY data (%d)"), r1);
      break;
    case 3:               // QPC Reached = QPC, Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
      #ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
      //      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
      //      r3 = esp_phy_erase_cal_data_in_nvs();
      //      r3 = NvmErase("cal_data");
      #endif
      //      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota (%d,%d) and PHY data (%d)"), r1, r2, r3);
      #ifdef ENABLE_DEVFEATURE_SETTINGS__TFS
      r3 = pCONT_mfile->TfsDeleteFile(TASM_FILE_SETTINGS);
      #endif
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
  }

#endif // ESP32

}

// Copied from 2.4.0 as 2.3.0 is incomplete
bool mSettings::SettingsEraseConfig(void) 
{
  const size_t cfgSize = 0x4000;
  size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

  for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
    if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
      return false;
    }
  }
  return true;
}

void mSettings::SettingsSdkErase(void)
{
  WiFi.disconnect(true);    // Delete SDK wifi config
  SettingsErase(1);
  delay(1000);
}


#ifdef ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY
bool mSettings::NvmExists(const char *sNvsName) {
  nvs_handle_t handle;
  if (nvs_open(sNvsName, NVS_READONLY, &handle) != ESP_OK) {
    return false;
  }
  nvs_close(handle);
  return true;
}

bool mSettings::NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen) {
  nvs_handle_t handle;
  esp_err_t result = nvs_open(sNvsName, NVS_READONLY, &handle);
  if (result != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NVS: Error %d"), result);
    return false;
  }
  size_t size = nSettingsLen;
  nvs_get_blob(handle, sName, pSettings, &size);
  nvs_close(handle);
  return true;
}

void mSettings::NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen) {
#ifdef USE_WEBCAM
  WcInterrupt(0);  // Stop stream if active to fix TG1WDT_SYS_RESET
#endif
  nvs_handle_t handle;
  esp_err_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (result != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NVS: Error %d"), result);
  } else {
    nvs_set_blob(handle, sName, pSettings, nSettingsLen);
    nvs_commit(handle);
    nvs_close(handle);
  }
#ifdef USE_WEBCAM
  WcInterrupt(1);
#endif
}

int32_t mSettings::NvmErase(const char *sNvsName) {
  nvs_handle_t handle;
  int32_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (ESP_OK == result) { result = nvs_erase_all(handle); }
  if (ESP_OK == result) { result = nvs_commit(handle); }
  nvs_close(handle);
  return result;
}
#endif // ENABLE_DEVFEATURE_SETTINGS__NVM_NON_VOLATILE_MEMORY



