
#include "2_CoreSystem/01_Settings/mSettings.h"

struct DATA_BUFFER data_buffer;

int8_t mSettings::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function)
  {
    
    case TASK_EVERY_SECOND:

    break;
    case TASK_EVERY_FIVE_SECOND:{

    }break;

    case TASK_EVERY_MINUTE:
      
      #ifdef USE_MODULE_CORE_FILESYSTEM
      #ifdef ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
        // Copy Settings as Last Known Good if no changes have been saved since 30 minutes
        if (!runtime.settings_lkg && (tkr_time->UtcTime() > START_VALID_UTC_TIME) && (Settings.cfg_timestamp < tkr_time->UtcTime() - (3 * 60))) 
        {
          pCONT_mfile->TfsSaveFile(TASM_FILE_SETTINGS_LKG_LAST_KNOWN_GOOD, (const uint8_t*)&Settings, sizeof(SETTINGS));
          runtime.settings_lkg = true;
        }
        else
        {
          ALOG_INF(PSTR("UtcTime()%d > START_VALID_UTC_TIME%d) && (Settings.cfg_timestamp%d < UtcTime() - (30 * 60)) %d"), 
            tkr_time->UtcTime(),
            START_VALID_UTC_TIME,
            Settings.cfg_timestamp,
            tkr_time->UtcTime() - (3 * 60)
         );
        }
      #endif // ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
      #endif // USE_MODULE_CORE_FILESYSTEM
      
    break;
    case TASK_EVERY_HOUR:

      #ifdef ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
        #ifdef ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
        tkr_set->SettingsSaveAll();
        #endif
      #else 
      #ifdef ENABLE_LOG_LEVEL_INFO
      DEBUG_PRINTLN("SettingsSave dis");
      #endif // ifdef ENABLE_LOG_LEVEL_INFO
      #endif // ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR

    break;   
    case TASK_ON_BOOT_SUCCESSFUL:
      Settings.bootcount++;              // Moved to here to stop flash writes during start-up

      ALOG_INF( PSTR(D_LOG_APPLICATION D_BOOT_COUNT "SUCCESSFUL BOOT %d after %d seconds"), Settings.bootcount, 120);
  
      #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
      RtcFastboot_Reset(); // ie reset the value so bootloops wont be detected after this point (eg 10 seconds)
      #endif

      #ifdef ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3
      flag_rtc_reboot_reset_on_success = true;
      ALOG_HGL(PSTR("flag_rtc_reboot_reset_on_success"));
      #endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3

      #ifdef USE_DEEPSLEEP
          if (!(DeepSleepEnabled() && !Settings->flag3.bootcount_update)) {  
            // SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
      #endif
            // Settings->bootcount++;              // Moved to here to stop flash writes during start-up
            // ALOG_DBG(PSTR(D_LOG_APPLICATION D_BOOT_COUNT " %d"), Settings->bootcount);
      #ifdef USE_DEEPSLEEP
          }
      #endif

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_FILESYSTEM_APPEND__CONFIG_SETTINGS__ID:
      JsonAppend_Settings();
    break;
    /************
     * xx SECTION * 
    *******************/
    case TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
      Function_Template_Load();
    break;
  }

} 




// load in driver and sensor template settings
void mSettings::Function_Template_Load(){

  #ifndef DISABLE_SERIAL_LOGGING
  //DEBUG_PRINTF("mSettings::Function_Template_Load"); Serial.flush();
  #endif

  #ifdef USE_FUNCTION_TEMPLATE  
  // Read into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

  ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK  "FUNCTION_TEMPLATE READ = \"%d|%s\""),data_buffer.payload.length_used, data_buffer.payload.ctr);
  
  pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

  runtime.template_loading.status.function = TemplateSource::HEADER_TEMPLATE;

  #endif //USE_FUNCTION_TEMPLATE
  
}

int16_t mSettings::GetFunctionIDbyName(const char* c)
{
  if(strcasecmp_P(c,PM_TASK_EVENT_INPUT_STATE_CHANGED_CTR)==0){ return TASK_EVENT_INPUT_STATE_CHANGED_ID; }
  if(strcasecmp_P(c,PM_TASK_EVENT_MOTION_STARTED_CTR)==0){ return TASK_EVENT_MOTION_STARTED_ID; }
  if(strcasecmp_P(c,PM_TASK_EVENT_MOTION_STARTED_CTR)==0){ return TASK_EVENT_MOTION_STARTED_ID; }
  if(strcasecmp_P(c,PM_TASK_EVENT_MOTION_ENDED_CTR)==0){ return TASK_EVENT_MOTION_ENDED_ID; }
  if(strcasecmp_P(c,PM_TASK_EVENT_SET_POWER_CTR)==0){ return TASK_EVENT_SET_POWER_ID; }
  if(strcasecmp_P(c,PM_TASK_EVENT_SET_SPEED_CTR)==0){ return TASK_EVENT_SET_SPEED_ID; }
  return -1;
}


void mSettings::JsonAppend_Settings()
{

  // JBI->Object_Start_F(GetModuleFriendlyName());  //json file parser will pass them to the modules, but should strip out the level_object from commands
    JBI->Add("BootCount", Settings.bootcount);
    #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
    JBI->Add("FastBootCount", RtcFastboot.fast_reboot_count);
    #endif
  // JBI->Object_End();

}


uint16_t mSettings::CountCharInCtr(const char* tosearch, char tofind){
  uint16_t count = 0;
  for(uint16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind){ count++; }
  }
  return count;
}


const char* mSettings::Get_Json_Level_Name(uint8_t id) 
{
    switch(id){
        case JSON_LEVEL_NONE:      
            return PM_LEVEL_NONE_CTR;
        case JSON_LEVEL_IFCHANGED: 
            return PM_LEVEL_IFCHANGED_CTR;
        case JSON_LEVEL_SHORT:     
            return PM_LEVEL_SHORT_CTR;
        case JSON_LEVEL_DETAILED:  
            return PM_LEVEL_DETAILED_CTR;
        case JSON_LEVEL_ALL:       
            return PM_LEVEL_ALL_CTR;
        case JSON_LEVEL_DEBUG:     
            return PM_LEVEL_DEBUG_CTR;
        default:
            return PM_LEVEL_NONE_CTR; // Default to "None" if an unknown id is passed
    }
}


/*
 * Based on cores/esp8266/Updater.cpp
 */
void mSettings::SetFlashModeDout(void)
{
  uint8_t *_buffer;
  uint32_t address;

    #ifdef ESP8266
  eboot_command ebcmd;
  eboot_command_read(&ebcmd);
  address = ebcmd.args[0];
  _buffer = new uint8_t[FLASH_SECTOR_SIZE];

  if (ESP.flashRead(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE)) {
    if (_buffer[2] != 3) {  // DOUT
      _buffer[2] = 3;
      if (ESP.flashEraseSector(address / FLASH_SECTOR_SIZE)) ESP.flashWrite(address, (uint32_t*)_buffer, FLASH_SECTOR_SIZE);
    }
  }
  delete[] _buffer;
  
    #endif // ESP8266
}


#ifdef ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER


/*********************************************************************************************\
 * Config Settings->text char array support
\*********************************************************************************************/

uint32_t mSettings::GetSettingsTextLen(void) {
  char* position = Settings.text_pool;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
  }
  return position - Settings.text_pool;
}

bool settings_text_mutex = false;
uint32_t settings_text_busy_count = 0;

bool mSettings::SettingsUpdateFinished(void) {
  uint32_t wait_loop = 10;
  while (settings_text_mutex && wait_loop) {  // Wait for any update to finish
    yield();
    delayMicroseconds(1);
    wait_loop--;
  }
  return (wait_loop > 0);  // true if finished
}

bool mSettings::SettingsUpdateText(uint32_t index, const char* replace_me) 
{

  ALOG_DBM(PSTR("SettingsUpdateText %d %s"),index,replace_me);

  // Serial.println();
  // for(int i=0;i<138;i++){ Serial.print(Settings.text_pool[i]); } Serial.println();

  if (index >= SET_MAX) {
    return false;  // Setting not supported - internal error
  }

  DEBUG_LINE_HERE

  // Make a copy first in case we use source from Settings->text
  uint32_t replace_len = strlen_P(replace_me);
  char replace[replace_len +1];
  memcpy_P(replace, replace_me, sizeof(replace));
  uint32_t index_save = index;

  uint32_t start_pos = 0;
  uint32_t end_pos = 0;
  char* position = Settings.text_pool;
  for (uint32_t size = 0; size < SET_MAX; size++) {
    while (*position++ != '\0') { }
    if (1 == index) {
      start_pos = position - Settings.text_pool;
    }
    else if (0 == index) {
      end_pos = position - Settings.text_pool -1;
    }
    index--;
  }
  uint32_t char_len = position - Settings.text_pool;

  DEBUG_LINE_HERE
  uint32_t current_len = end_pos - start_pos;
  int diff = replace_len - current_len;

 ALOG_DBM(PSTR("TST: start %d, end %d, len %d, current %d, replace %d, diff %d"),
   start_pos, end_pos, char_len, current_len, replace_len, diff);

  DEBUG_LINE_HERE
  int too_long = (char_len + diff) - settings_text_size;
    ALOG_DBM(PSTR(D_LOG_CONFIG "Text test by %d char(s)"), too_long);
  if (too_long > 0) {
    ALOG_DBM(PSTR(D_LOG_CONFIG "Text overflow by %d char(s)"), too_long);
    return false;  // Replace text too long
  }

  DEBUG_LINE_HERE
  if (settings_text_mutex && !SettingsUpdateFinished()) {
    settings_text_busy_count++;
  } else {
    settings_text_mutex = true;

  DEBUG_LINE_HERE
    if (diff != 0) {
      // Shift Settings->text up or down
      memmove_P(Settings.text_pool + start_pos + replace_len, Settings.text_pool + end_pos, char_len - end_pos);
    }
  DEBUG_LINE_HERE
    // Replace text
    memmove_P(Settings.text_pool + start_pos, replace, replace_len);
  DEBUG_LINE_HERE
    // Fill for future use
    memset(Settings.text_pool + char_len + diff, 0x00, settings_text_size - char_len - diff);
  DEBUG_LINE_HERE

    settings_text_mutex = false;
  }

#ifdef DEBUG_TASK_SETTINGSUPDATETEXT
  ALOG_DBG(PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d, Id %02d = \"%s\""), GetSettingsTextLen(), settings_text_size, settings_text_busy_count, index_save, replace);
  Serial.println("test_pool");
  for(int i=0;i<100;i++)
  {
    Serial.print(Settings.text_pool[i]);
  }
  Serial.println();
#else
  ALOG_DBM(PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d"), GetSettingsTextLen(), settings_text_size, settings_text_busy_count);
#endif


  return true;
}


char* mSettings::SettingsText(uint32_t index) {
  char* position = Settings.text_pool;

  if (index >= SET_MAX) { // Index above SET_MAX are not stored in Settings
#ifdef USE_WEBSERVER
    if (SET_BUTTON17 <= index && index <= SET_BUTTON32)
      return (char*)GetWebButton(index-SET_BUTTON17+16);
#endif
    position += settings_text_size -1;  // Setting not supported - internal error - return empty string
  } else {
    SettingsUpdateFinished();
    for (;index > 0; index--) {
      while (*position++ != '\0') { }
    }
  }
  return position;
}


#endif // ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER



/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mSettings::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  if(jtok = obj[PM_SYSTEM_RESTART]){
    // if(jtok.isStr()){
    //   if((tmp_id=GetPixelHardwareTypeIDbyName(jtok.getStr()))>=0){
    //     CommandSet_PixelHardwareTypeID(tmp_id);
    //     data_buffer.isserviced++;
    //   }
    // }else
    if(jtok.isNum()){
      CommandSet_SystemRestartID(jtok.getInt());
      data_buffer.isserviced++;
    }
  }


  /**
   * Debug options
   * */
  if(jtok = obj["SettingSave"])
  {
    SettingsSaveAll();
  }
  if(jtok = obj["SettingLoad"])
  {
    SettingsLoad();
  }


  if(jtok = obj[PM_DEVICENAME])
  { 

    char module_friendlyname_buffer[30];
    uint16_t module_id = 0;

    // Search across all module names
    for(auto& module : pCONT->pModule){

      sprintf_P(module_friendlyname_buffer,"%S", module->GetModuleName());

      ALOG_DBM( PSTR("CHECKING module_friendlyname_buffer = %s"),module_friendlyname_buffer); 
  
      if(jtok = obj[PM_DEVICENAME].getObject()[module_friendlyname_buffer])
      { 
        ALOG_DBM( PSTR("found module_friendlyname_buffer = %s"),module_friendlyname_buffer); 

        JsonParserArray arr = obj[PM_DEVICENAME].getObject()[module_friendlyname_buffer];
      
        if(arr){  
          //Get devices already present
          uint8_t device_count = DLI->GetDeviceNameCount( module->GetModuleUniqueID()  );
          
          for(uint8_t id =0;id<arr.size();id++){
            jtok = arr[id];
            const char* device_name_ctr = jtok.getStr();
            DLI->AddDeviceName(device_name_ctr,module->GetModuleUniqueID(),device_count++);
            ALOG_DBM( PSTR(D_LOG_RELAYS "device_name_ctr = %s"),device_name_ctr); 
            ALOG_DBM( PSTR(D_LOG_RELAYS "device_count = %d"),device_count);  
          } //if array
        }//if array
      }
    } //for

  }
  

  if(jtok = obj["BootCount"])
  {
    Settings.bootcount = jtok.getInt();
    AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("BootCount %d"),Settings.bootcount);
  }

}


/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_SystemRestartID *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mSettings::CommandSet_SystemRestartID(uint8_t value){

  /**
   * 0 = invalid
   * 1 = soft restart
   * 2 = save reboot, save settings, then restart
   * 3 = reboot into captive portal mode
   * 4 = reboot and wait for 1 minute for OTA capture
   * 5 = force crash with hardware WDT
   * 6 = reboot and reset to default setting (not wifi)
   * 7 (requires second command) = forced reset and all settings including wifi
   * 3+ is disabling things for stability ie fastboot or erasing settings to default
   * 
   * 
   * */
  
  #ifdef USE_MODULE_NETWORK_WIFI   
  if(value == 1)
  {
    pCONT_wif->EspRestart();
  }
  else
  if(value == 2)
  {

    ALOG_DBM(PSTR("REBOOT TEST" DEBUG_INSERT_PAGE_BREAK));
    ALOG_DBM(PSTR("Current bootcount is %d"), Settings.bootcount);

    tkr_set->TestSettings_ShowLocal_Header();
    tkr_set->TestSettingsLoad();

    ALOG_DBM(PSTR("Modying bootcount to %d"), Settings.bootcount++);
    
    tkr_set->SettingsSaveAll();

    ALOG_DBM(PSTR("Settings should be saved now to %d"), Settings.bootcount);

    tkr_set->TestSettings_ShowLocal_Header();
    tkr_set->TestSettingsLoad();

    pCONT_wif->EspRestart();

  }  
  #endif // ifdef USE_MODULE_NETWORK_WIFI
  
}

