/**
 * @file    HomeControlSystem.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   Primary code setup() and loop()
 * @version 0.1
 * @date    2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

// #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY


#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
// In Main_PulSar.cpp or another relevant file
// int channels_resolution = 10;  // or whatever the appropriate value/type is
// uint8_t channels_resolution[30];
#include <gpio_viewer.h> // Must me the first include in your project
GPIOViewer gpio_viewer;
#endif



/*********************************************************************************************
 * Hardware related
\*********************************************************************************************/

#ifdef ESP32
  #include "soc/soc.h"
  #include "soc/rtc_cntl_reg.h"
  #define DISABLE_ESP32_BROWNOUT
  void DisableBrownout(void) 
  { // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
  }
  #ifdef ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING
  void hw_wdt_disable(){
    *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF 
  }
  void hw_wdt_enable(){ 
    *((volatile uint32_t*) 0x60000900) |= 1; // Hardware WDT ON 
  }
  #endif  
#endif // ESP32

#ifdef ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT
/*********************************************************************************************\
 * Emergency reset if Rx and Tx are tied together
\*********************************************************************************************/
void EmergencySerial_SettingsReset(void) {
  Serial.begin(115200);
  Serial.write(0xA5);
  Serial.write(0x5A);
  delay(1);
  if (Serial.available() == 2) {
    if ((Serial.read() == 0xA5) && (Serial.read() == 0x5A)) {
      tkr_set->SettingsErase(3);       // Reset all settings including QuickPowerCycle flag

      do {                    // Wait for user to remove Rx Tx jumper and power cycle
        Serial.write(0xA5);
        delay(1000);          // Satisfy SDK
      } while (Serial.read() == 0xA5);  // Poll for removal of jumper

      mSupportHardware::ESP_Restart();          // Restart to init default settings
    }
  }
  Serial.println();
  Serial.flush();
#ifdef ESP32
  delay(10);                  // Allow time to cleanup queues - if not used hangs ESP32
  Serial.end();
  delay(10);                  // Allow time to cleanup queues - if not used hangs ESP32
#endif  // ESP32
}
#endif  // ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT

/********************************************************************************************/
/*********************SETUP******************************************************************/
/********************************************************************************************/

void setup(void)
{ 

/********************************************************************************************
 ** Brownout ********************************************************************************
 ********************************************************************************************/
 
  #ifdef ESP32
    #ifdef DISABLE_ESP32_BROWNOUT
      DisableBrownout();      // Workaround possible weak LDO resulting in brownout detection during Wifi connection
    #endif  // DISABLE_ESP32_BROWNOUT

    #ifdef CONFIG_IDF_TARGET_ESP32
    // restore GPIO16/17 if no PSRAM is found
    if (!SupportESP32::FoundPSRAM()) {
      // test if the CPU is not pico
      uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
      uint32_t pkg_version = chip_ver & 0x7;
      if (pkg_version <= 3) {   // D0WD, S0WD, D2WD
        gpio_reset_pin(GPIO_NUM_16);
        gpio_reset_pin(GPIO_NUM_17);
      }
    }
    #endif  // CONFIG_IDF_TARGET_ESP32
  #endif  // ESP32

  /**
   * @brief WatchDog timer
   * Code priority: Highest (Before Fastboot, Primary Recovery)
   **/
  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Init();
  #endif

  /********************************************************************************************
   ** Fastboot: >> Base Setup Recovery <<  
   *     - If Settings, TaskerManager etc is corrupt
   *     - Minimal code must run above this (Serial start, RTC check)
   * Code priority: High (Secondary Recovery)
   * @note AddLog can not be used
  ********************************************************************************************/

  /**
   * @brief Only enable serial first if fastboot debugging
   **/
  #ifdef DEBUG_FASTBOOT
  Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT);
  Serial.println(F("\n\rSerial Enabled Early for FastBoot Debug" DEBUG_INSERT_PAGE_BREAK));
  #endif

  #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION

    RtcFastboot_Load();

    if (!RtcFastboot_Valid())
    {
      RtcFastboot.fast_reboot_count = 0;
    }
    
    /**
     * @brief Good Boot: Waking from sleep, keep reseting the counter
     **/
    if (ResetReason_g() == REASON_DEEP_SLEEP_AWAKE) 
    {
      RtcFastboot.fast_reboot_count = 0;  // Disable fast reboot and quick power cycle detection
    } 
    /**
     * @brief Bad Boot: Increment count
     **/
    else
    {
      RtcFastboot.fast_reboot_count++;
      #ifdef DEBUG_FASTBOOT
      Serial.printf("FastBoot: Count %d\n\r", RtcFastboot.fast_reboot_count); 
      #endif
    }
    
    RtcFastboot_Save(); // Save reboot

    /**
     * @brief If fastboot has exceeded OTA fallback bootcount, then immediately enter safemode/recoverymode
     * @note:  Code below will first attempt to recover device by disabling feature, this is a last step measure
     **/
    #if defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
    if(RtcFastboot.fast_reboot_count > 10)
    {
      SafeMode_StartAndAwaitOTA();
    }
    #endif // ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
    /**
     * @brief If fastboot has exceeded OTA fallback bootcount, then immediately enter safemode/recoverymode
     * @note:  Code below will first attempt to recover device by disabling feature, this is a last step measure
     **/
    #if defined(ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
    if(RtcFastboot.fast_reboot_count > 10)
    {
      SafeMode_CellularConnectionAndSendLocation();
    }
    #endif // ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

  #ifdef ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING
  Serial.flush();
  delay(1000);
  pCONT_sup->CmndCrash();
  #endif  // ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

/********************************************************************************************
 ** RTC Settings ********************************************************************************
 ********************************************************************************************/
  
  // Load the baudrate from RTC into temp value, and after FlashSettings are loaded, move its value into flash settings
  uint32_t baudrate_tmp = 115200;

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  if (RtcSettingsLoad(0)) {
    uint32_t baudrate = (RtcSettings.baudrate / 300) * 300;  // Make it a valid baudrate
    if (baudrate) { baudrate_tmp = baudrate; } // Only modify if valid
  }
  #endif // ENABLE_DEVFEATURE_RTC_SETTINGS

/********************************************************************************************
 ** Serial **********************************************************************************
 ********************************************************************************************/
 
  #ifndef DISABLE_SERIAL0_CORE
  Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT); // to be baudrate_tmp later
  #endif
  #ifdef USE_SERIAL_ALTERNATE_TX
    #ifndef ESP32
      Serial.set_tx(2);
    #endif
  #endif
  Serial.println(F("\n\rRebooting..." DEBUG_INSERT_PAGE_BREAK));
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
  #endif

  Serial.printf("baudrate_tmp = %d\n\r", baudrate_tmp);

  #ifdef ENABLE_DEVFEATURE_SETDEBUGOUTPUT
  Serial.setDebugOutput(true);
  #endif 

/********************************************************************************************
 ** Debug: When debugging is crashing, priority is to start wifi and get OTA going and waiting a few seconds to catch an OTA **************************************************************************
 ********************************************************************************************/
  // #ifdef ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // if (RtcFastboot.fast_reboot_count > 0) {
  //   SafeMode_StartAndAwaitOTA(10);
  // }
  // #endif
  
/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  // Init Json builder with memory address and size
  JsonBuilderI ->Start(data_buffer.payload.ctr, data_buffer.payload.length_used, DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  BufferWriterI->Start(data_buffer.payload.ctr, data_buffer.payload.length_used, DATA_BUFFER_PAYLOAD_MAX_LENGTH); //length prob doesnt need to be set either after its defined in the class
  
  /**
   * @brief Start the Tasker_Interface module
   **/
  pCONT->Instance_Init();
  
/********************************************************************************************
 ** LOGGING: Set boot log levels *********************************************************************
 ********************************************************************************************/

  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;

  DEBUG_LINE_HERE
  
/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  ALOG_DBM(PSTR("AddLog Started"));

  DEBUG_LINE_HERE

/********************************************************************************************
 ** Splash boot reason ***************************************************************************
 ********************************************************************************************/

  ALOG_INF(PSTR("ResetReason=%d"), ResetReason_g());

/********************************************************************************************
 ** Show PSRAM Present **********************************************************************
 ********************************************************************************************/

  #ifdef ESP32
    // ALOG_INF(PSTR("HDW: %s %s"), GetDeviceHardware().c_str(),
    //           SupportESP32::FoundPSRAM() ? (SupportESP32::CanUsePSRAM() ? "(PSRAM)" : "(PSRAM disabled)") : "" );
    // ALOG_DBG(PSTR("HDW: FoundPSRAM=%i CanUsePSRAM=%i"), SupportESP32::FoundPSRAM(), SupportESP32::CanUsePSRAM());
    // #if !defined(HAS_PSRAM_FIX)
    // if (SupportESP32::FoundPSRAM() && !SupportESP32::CanUsePSRAM()) {
    //   ALOG_INF(PSTR("HDW: PSRAM is disabled, requires specific compilation on this hardware (see doc)"));
    // }
    // #endif
  #else // ESP32
    // ALOG_INF(PSTR("HDW: %s"), GetDeviceHardware().c_str());
  #endif // ESP32
  DEBUG_LINE_HERE


/********************************************************************************************
 ** File System : Init *****************************************************************************
 ********************************************************************************************/

  #ifdef USE_MODULE_CORE_FILESYSTEM
    pCONT_mfile->UfsInit();  // xdrv_50_filesystem.ino
  #endif

  #ifdef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
  if (tkr_set->Settings2 == nullptr) {
    tkr_set->Settings2 = (mSettings::TSettings2*)malloc(sizeof(mSettings::TSettings2));
  }
  #endif

/********************************************************************************************
 ** Settings ********************************************************************************
 ********************************************************************************************/

  tkr_set->SettingsInit();

  #ifdef ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT
    EmergencySerial_SettingsReset();
  #endif  // ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT

  pCONT_sup->init_FirmwareVersion();

   //preload minimal required
  tkr_set->SettingsDefault();
  ALOG_DBG(PSTR("Loading minimal defaults"));
   // Overwrite with latest values, including template if new SETTINGS_CONFIG exists  
  ALOG_DBG(PSTR("Loading settings from saved memory"));
  tkr_set->SettingsLoad();                   // Only the system level settings are loaded here, not the module settings which should happen below
  
  // sprintf(tkr_set->Settings.debug, "debug12\0");

/********************************************************************************************
 ** LOGGING: Set boot log levels again to override settings load *********************************************************************
 ********************************************************************************************/

  uint8_t saved_serial_loglevel = tkr_set->Settings.logging.serial_level;
  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;

  ALOG_INF(PSTR("Log level for boot: %d"), saved_serial_loglevel);

/********************************************************************************************
 ** System OSWatch: To detect loop hangs that might happen during (OTA) upgrades  ******************
 ********************************************************************************************/

#ifdef ENABLE_FEATURE_SYSTEM__OSWATCH_FOR_LOOP_HANGS
  // OsWatchInit();
#endif

/********************************************************************************************
 ** Fastboot ********************************************************************************
 ********************************************************************************************/

  /********************************************************************************************
   ** Fastboot: >> Configuration Recovery <<  
   *     - Loaded settings to determine how fastboot is configured
   *     - This code must run before drivers/sensors are initiated, so they may be disabled if recovery is required
  ********************************************************************************************/
  #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  
    ALOG_DBM( PSTR("ARESET TWICE! \t\t\t%d"), RtcFastboot.fast_reboot_count);

    if (tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET]) // SetOption36
    {         
      // Disable functionality as possible cause of fast restart within BOOT_LOOP_TIME seconds (Exception, WDT or restarts)
      if (RtcFastboot.fast_reboot_count > tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET]) {       // Restart twice
        
        // Settings->flag3.user_esp8285_enable = 0;       // SetOption51 - Enable ESP8285 user GPIO's - Disable ESP8285 Generic GPIOs interfering with flash SPI
        if (RtcFastboot.fast_reboot_count > tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +1) {  // Restart 3 times
          // for (
            uint32_t i = 0; //i < MAX_RULE_SETS; i++) {
          //   if (bitRead(Settings->rule_stop, i)) {
          //     bitWrite(Settings->rule_enabled, i, 0);  // Disable rules causing boot loop
          ALOG_INF( PSTR("Fastboot: Disable Rule %d"), i );
          //   }
          // }
        }
        if (RtcFastboot.fast_reboot_count > tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +2) {  // Restarted 4 times
          // Settings->rule_enabled = 0;                  // Disable all rules
          // TasmotaGlobal.no_autoexec = true;
          ALOG_INF( PSTR("Fastboot: Disable All Rules") );
        }
        if (RtcFastboot.fast_reboot_count > tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +3) {  // Restarted 5 times
          // for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
          //   Settings->my_gp.io[i] = GPIO_NONE;         // Reset user defined GPIO disabling sensors
          // }
          ALOG_INF( PSTR("Fastboot: Disable GPIO Functions") );
        }
        if (RtcFastboot.fast_reboot_count > tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +4) {  // Restarted 6 times
          // Settings->module = Settings->fallback_module;  // Reset module to fallback module
          // Settings->last_module = Settings->fallback_module;
          ALOG_INF( PSTR("Fastboot: Reset Module") );
        }
        ALOG_INF( PSTR("FRC: " D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcFastboot.fast_reboot_count);
      }
    }

  #endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

/********************************************************************************************
 ** SERIAL: Change baud to module default if module has changed ****************************************
 ********************************************************************************************/

#ifdef ENABLE_FEATURE_BOOT__RESET_BAUDRATE_ON_BOOT_WITH_MODULE_CHANGE
  // TasmotaGlobal.module_changed = (Settings->module != Settings->last_module);
  // if (TasmotaGlobal.module_changed) {
  //   Settings->baudrate = APP_BAUDRATE / 300;
  //   Settings->serial_config = TS_SERIAL_8N1;
  // }
  // SetSerialBaudrate(Settings->baudrate * 300);  // Reset serial interface if current baudrate is different from requested baudrate
#endif // ENABLE_FEATURE_BOOT__RESET_BAUDRATE_ON_BOOT_WITH_MODULE_CHANGE

/********************************************************************************************
 ** Quick Power Cycle ***********************************************************************
 ********************************************************************************************/

#ifdef ENABLE_FEATURE_BOOT__QUICK_POWER_CYCLE_TO_CAUSE_MANUAL_RESET
  // if (1 == RtcReboot.fast_reboot_count) {      // Allow setting override only when all is well
  //   UpdateQuickPowerCycle(true);
  // }
#endif // ENABLE_FEATURE_BOOT__QUICK_POWER_CYCLE_TO_CAUSE_MANUAL_RESET

/********************************************************************************************
 ** Load Templates **************************************************************************
 ********************************************************************************************/
  
  // configure any memory address needed as part of module init or templates
  pCONT->Tasker_Interface(TASK_POINTER_INIT);

  #ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
  pCONT->Tasker_Interface(TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM);
  #endif
  
  /**
   * @brief DEBUG: Load template from progmem and override settings to ensure my developing devices always enter a known state.
   *               This should be disabled for production devices, and only enabled for development devices.
   **/
  #ifdef ENABLE_FEATURE_TEMPLATES__LOAD_FROM_PROGMEM_TO_OVERRIDE_STORED_SETTINGS_TO_MAINTAIN_KNOWN_WORKING_VALUES
  ALOG_DBM(PSTR(D_LOG_MEMORY D_LOAD " Temporary loading any progmem templates"));
  pCONT->Tasker_Interface(TASK_TEMPLATES__LOAD_MODULE); // loading module, only interface modules will have these
  #else
  #warning "FORCE_TEMPLATE_LOADING is disabled, and production/release is assumed. This REQUIRES valid settings/storage or device may be unstable"
  #endif
  
/********************************************************************************************
 ** Set RUNTIME log values *********************************************************************
 ********************************************************************************************/

  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;  
    
  /********************************************************************************************
   ** Initialise System and Modules ***********************************************************
  ********************************************************************************************/

  #ifdef ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // Must be at the end of your setup
  // gpio_viewer.setSamplingInterval(25); // You can set the sampling interval in ms, if not set default is 100ms
  gpio_viewer.begin();
  #endif

  // Init the GPIOs
  pCONT_pins->GpioInit();
  // Start pins in modules
  pCONT->Tasker_Interface(TASK_PRE_INIT);
  // Init devices
  pCONT->Tasker_Interface(TASK_INIT);
  // Init devices after others have been configured fully
  pCONT->Tasker_Interface(TASK_POST_INIT);
  // Run system functions 
  pCONT->Tasker_Interface(TASK_FUNCTION_LAMBDA_INIT);
  // Load any stored user values into module
  pCONT->Tasker_Interface(TASK_SETTINGS_LOAD_VALUES_INTO_MODULE); // to be used 2023, this will load module config from filesystem
  
  DEBUG_LINE_HERE
  // Init any dynamic memory buffers
  pCONT->Tasker_Interface(TASK_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID);
  DEBUG_LINE_HERE


  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif  

  /********************************************************************************************
   ** File System : Load after settings for now, so this method overrides any defaults   ******
  ********************************************************************************************/

  #ifdef ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
  #ifdef ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    pCONT_mfile->JsonFile_Load__Stored_Module_Or_Default_Template();
  #endif
  #endif // ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
  #ifdef ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT
  pCONT->Tasker_Interface(TASK_FILESYSTEM__LOAD__MODULE_DATA__ID);
  #endif // ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  /**
   * This can only happen AFTER each module is running/enabled (port init checks). This will override the settings load, so should be tested if needed when settings work
   * */
  pCONT->Tasker_Interface(TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM); //USED
  // Configure sensor/drivers to values desired for modules
  pCONT->Tasker_Interface(TASK_CONFIGURE_MODULES_FOR_DEVICE); //??

  /********************************************************************************************
   ** MQTT: Configure mqtt handlers in modules   ******
  ********************************************************************************************/

  pCONT->Tasker_Interface(TASK_MQTT_HANDLERS_INIT);  

  // Init the refresh periods for mqtt
  #ifndef ENABLE_DEBUGFEATURE_MQTT__DISABLE_SETTING_DYNAMIC_REFRESH_RATES
  pCONT->Tasker_Interface(TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD);
  #endif

  /********************************************************************************************
   ** RULES: Configure mqtt handlers in modules   ******
  ********************************************************************************************/

  #ifdef USE_MODULE_CORE_RULES
  pCONT->Tasker_Interface(TASK_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID);
  #endif 
  
/********************************************************************************************
 ** LOGGING: Set runtime log levels *********************************************************************
 ********************************************************************************************/

  tkr_set->Settings.logging.serial_level = saved_serial_loglevel;

  /********************************************************************************************
   ** // For debugging, allow method to override init/loaded values **************************************************************************
  ********************************************************************************************/
  
  #ifdef ENABLE_FUNCTION_DEBUG
    pCONT->Tasker_Interface(TASK_DEBUG_CONFIGURE);
  #endif

  #ifdef ENABLE_BOOT_OVERRIDE_INIT
  pCONT->Tasker_Interface(TASK_OVERRIDE_BOOT_INIT);
  #endif

  /********************************************************************************************
   ** Boot Completed **************************************************************************
  ********************************************************************************************/

  pCONT->Tasker_Interface(TASK_ON_BOOT_COMPLETE);

  // Serial.println("END OF SETUP REACHED"); Serial.flush();

  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif  

  #ifdef ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT
  for(int i=0;i<(70*25)+1;i++){ // +1 so final line has \n
    Serial.print('='); 
    if(i % 70 == 0)
      Serial.println();
  }
  #endif

  pCONT->Tasker_Interface(TASK_BOOT_MESSAGE); // Display status of system
  

}


void LoopTasker()
{
  // Serial.println("LOOP STARTED"); Serial.flush();

  #ifdef USE_ARDUINO_OTA
    pCONT_sup->ArduinoOtaLoop();
  #endif
  // Serial.println("ArduinoOtaLoop passed STARTED"); Serial.flush();
   
  pCONT->Tasker_Interface(TASK_LOOP); DEBUG_LINE;
 
  if(tkr_time->UpTime() > 30){ pCONT->Tasker_Interface(TASK_FUNCTION_LAMBDA_LOOP); } // Only run after stable boot
 
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop50mSec ,50  )){ pCONT->Tasker_Interface(TASK_EVERY_50_MSECOND);  }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop100mSec,100 )){ pCONT->Tasker_Interface(TASK_EVERY_100_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop250mSec,250 )){ pCONT->Tasker_Interface(TASK_EVERY_250_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop1Sec   ,1000))
  {

    /**Since this only gets checked every second, we can use the uptime ticking to make sure it runs just once*/
    #ifdef ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS
    if(tkr_time->UpTime()==ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS){
      pCONT->Tasker_Interface(TASK_PRE_INIT_DELAYED);     // Configure sub modules and classes as needed, should this be renamed to "INIT_PINS"?
      pCONT->Tasker_Interface(TASK_INIT_DELAYED);         // Actually complete init, read sensors, enable modules fully etc
      pCONT->Tasker_Interface(TASK_MQTT_HANDLERS_INIT_DELAYED);
    }
    #endif // ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS


    pCONT->Tasker_Interface(TASK_EVERY_SECOND); 

    if((tkr_time->UpTime()%60)==0){                  pCONT->Tasker_Interface(TASK_EVERY_MINUTE); }
    
    if(
      ((tkr_time->UpTime()%5)==0)&&
      (tkr_time->UpTime()>20)
    ){                                      pCONT->Tasker_Interface(TASK_EVERY_FIVE_SECOND); }

    if(
      ((tkr_time->UpTime()%300)==0)&&
      (tkr_time->UpTime()>60)
    ){                                    pCONT->Tasker_Interface(TASK_EVERY_FIVE_MINUTE); }

    // Uptime triggers: Fire Once (based on uptime seconds, but due to this function being called every second, it will only fire once)
    if(tkr_time->UpTime() == 10){   pCONT->Tasker_Interface(TASK_UPTIME_10_SECONDS); }
    if(tkr_time->UpTime() == 30){   pCONT->Tasker_Interface(TASK_UPTIME_30_SECONDS); }
    if(tkr_time->UpTime() == 60){   pCONT->Tasker_Interface(TASK_UPTIME_1_MINUTES); }
    if(tkr_time->UpTime() == 600){   pCONT->Tasker_Interface(TASK_UPTIME_10_MINUTES); }
    if(tkr_time->UpTime() == 36000){ pCONT->Tasker_Interface(TASK_UPTIME_60_MINUTES); }

    // Check for midnight
    if((tkr_time->RtcTime.hour==0)&&(tkr_time->RtcTime.minute==0)&&(tkr_time->RtcTime.second==0)&&(tkr_time->lastday_run != tkr_time->RtcTime.day_of_year)){
      tkr_time->lastday_run = tkr_time->RtcTime.day_of_year;
      pCONT->Tasker_Interface(TASK_EVERY_MIDNIGHT); 
    }

    if(tkr_time->UpTime()==10){       pCONT->Tasker_Interface(TASK_BOOT_MESSAGE);}

    if(tkr_time->UpTime()==120){       pCONT->Tasker_Interface(TASK_ON_BOOT_SUCCESSFUL);}
      
    pCONT->Tasker_Interface(TASK_INIT_DELAYED_SECONDS);

  } // END secondloop

    
  #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE
  if(pCONT->function_event_queue.size())
  {
    DEBUG_LINE_HERE;
    bool execute_function = false;
    uint8_t iter_count = 0;
    for(auto& queue:pCONT->function_event_queue)
    {
      if(queue.delay_millis == 0){ execute_function = true; } // no delay
      if(mTime::TimeReached(&queue.tSaved_millis,queue.delay_millis)){ execute_function = true; }
    DEBUG_LINE_HERE;

      if(execute_function)
      {
        ALOG_HGL(PSTR("Executing Event Queue Item [%d]: func_id %d"), iter_count, queue.function_id);
    DEBUG_LINE_HERE;
        pCONT->Tasker_Interface(queue.function_id);
    DEBUG_LINE_HERE;
    // std::vector<mTaskerManager::FUNCTION_EXECUTION_EVENT>::iterator index = pCONT->function_event_queue.begin()+iter_count;

    
        ALOG_INF(PSTR("erase %d/%d"), iter_count, pCONT->function_event_queue.size());

        pCONT->function_event_queue.erase(pCONT->function_event_queue.begin()+iter_count);    
    DEBUG_LINE_HERE;   
      }
      iter_count++;
    }
    DEBUG_LINE_HERE;
  }
  #endif // ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

}

/********************************************************************************************/
/*********************loop*******************************************************************/
/********************************************************************************************/

#ifdef ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY
void SmartLoopDelay()
{
  pCONT_sup->SleepDelay(20);

  // #ifndef DISABLE_SLEEP
  // if(tkr_set->Settings.enable_sleep){
  //   if (tkr_set->Settings.flag_network.sleep_normal) {
  //     pCONT_sup->SleepDelay(tkr_set->runtime.sleep);
  //   } else {
  //     // Loop time < sleep length of time
  //     if (pCONT_sup->loop_runtime_millis < (uint32_t)tkr_set->runtime.sleep) {
  //       //delay by loop time
  //       pCONT_sup->SleepDelay((uint32_t)tkr_set->runtime.sleep - pCONT_sup->loop_runtime_millis);  // Provide time for background tasks like wifi
  //     } else {

  //       // if loop takes longer than sleep period, no delay, IF wifi is down, devote half loop time to wifi connect 
  //       // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
  //       if (tkr_set->global_state.wifi_down) {
  //         pCONT_sup->SleepDelay(pCONT_sup->loop_runtime_millis /2);
  //       }

  //     }
  //   }
  // }

  
  // #endif
}
#endif // ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY


void loop(void)
{
  pCONT_sup->activity.loop_counter++;
  pCONT_sup->loop_start_millis = millis();
  
  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif
  
  LoopTasker();
  
  pCONT_sup->loop_runtime_millis = millis() - pCONT_sup->loop_start_millis;

  if(mTime::TimeReached(&tkr_set->runtime.tSavedUpdateLoopStatistics, 1000)){
    pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
    #ifdef ENABLE_DEBUGFEATURE__SPLASH__LOOPS_PER_SECOND
    ALOG_INF(PSTR("LOOPSEC = \t\t\t\tLPS=%d, LoopTime=%d"), pCONT_sup->activity.cycles_per_sec, pCONT_sup->loop_runtime_millis);
    #endif
    pCONT_sup->activity.loop_counter=0;
  }

  // if(pCONT_sup->loop_runtime_millis > 500)
  // {
  //   ALOG_ERR(PSTR("LONG_LOOP ============= %d %d %d"), pCONT_sup->activity.loop_counter, pCONT_sup->activity.cycles_per_sec, pCONT_sup->loop_runtime_millis);
  // }

  #ifdef ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY
  SmartLoopDelay();
  #endif

  if (!pCONT_sup->loop_runtime_millis) { pCONT_sup->loop_runtime_millis++; } // We cannot divide by 0
  pCONT_sup->loop_delay_temp = tkr_set->runtime.sleep; 
  if (!pCONT_sup->loop_delay_temp) { pCONT_sup->loop_delay_temp++; }              // We cannot divide by 0
  pCONT_sup->loops_per_second = 1000 / pCONT_sup->loop_delay_temp;  // We need to keep track of this many loops per second, 20ms delay gives 1000/20 = 50 loops per second (50hz)
  pCONT_sup->this_cycle_ratio = 100 * pCONT_sup->loop_runtime_millis / pCONT_sup->loop_delay_temp;
  tkr_set->runtime.loop_load_avg = tkr_set->runtime.loop_load_avg - (tkr_set->runtime.loop_load_avg / pCONT_sup->loops_per_second) + (pCONT_sup->this_cycle_ratio / pCONT_sup->loops_per_second); // Take away one loop average away and add the new one

}
