#include "1_TaskerManager/mTaskerManager.h"

mTaskerManager* mTaskerManager::instance = nullptr;


int8_t mTaskerManager::Tasker_Interface(uint16_t task)
{

  int8_t result = 0;
  JsonParserObject obj = 0;

  /****************************************************************************************************************
   * JSON Parsing
   *****************************************************************************************************************/  
  if(task == TASK_JSON_COMMAND_ID)
  { 

    JsonParser parser(data_buffer.payload.ctr);
    obj = parser.getRootObject();   
    if (!obj) {
      ALOG_ERR(PM_DESERIALIZATION_ERROR);
      return FUNCTION_RESULT_ERROR_ID;
    }

    for(auto& mod:pModule)
    { 
      DEBUG_LINE_HERE_MILLIS
      mod->Tasker(task, obj);
    }
    
    return FUNCTION_RESULT_SUCCESS_ID;
  } 

  /****************************************************************************************************************
   * Threaded Processing Loop
   *****************************************************************************************************************/ 

  #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  uint16_t debug_idx = 0;
  #endif


  for(auto& mod:pModule)
  {     
              
    /****************************************************************************************************************
     * Debug: Stats
     *****************************************************************************************************************/ 
    #ifdef ENABLE_ADVANCED_DEBUGGING
    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      #ifdef ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS
      if(tkr_time->uptime_seconds_nonreset>ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS)
      {
      #endif
      #ifdef ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS
      if(tkr_time->uptime_seconds_nonreset<ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS)
      {
      #endif
        AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_CLASSLIST "TIS %d\t%S \t%S \t%d"), mod->GetModuleUniqueID(), mod->GetModuleName(), GetTaskName(task), millis() );
      #ifdef ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS
      }
      #endif
      #ifdef ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS
      }
      #endif
    #endif // ENABLE_DEBUG_FUNCTION_NAMES
    #endif // ENABLE_ADVANCED_DEBUGGING    

    #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)  || defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES) || defined(ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS)
    uint32_t start_millis = millis();
    #endif

    
    /****************************************************************************************************************
     * Manual Debug: Testing particular modules or functions
     *****************************************************************************************************************/ 
    // if(task == TASK_JSON_COMMAND_ID)
    if(task == TASK_JSON_COMMAND_ID)
    {
      // ALOG_COM( PSTR(D_LOG_TASKER "switch_index %d}"), switch_index);
      // ALOG_COM( PSTR(D_LOG_MQTT "{\"CommandsMatched\":%d}"), data_buffer.isserviced);
      DEBUG_LINE_HERE_MILLIS
    }
    
    #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
    // Record start time in microseconds
    uint32_t start_time = micros();
    #endif
    
    /****************************************************************************************************************
     * Thread: Call each module with the task
     *****************************************************************************************************************/ 
    #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
      // Check if the current task is in the list of tasks to monitor
      bool shouldMonitorAllTasks = (std::find(monitor_task.begin(), monitor_task.end(), TASKER_FUNCTION_TYPES(0)) != monitor_task.end());

      // If monitoring all tasks, or if the current task is in the monitored list
      if (shouldMonitorAllTasks || std::find(monitor_task.begin(), monitor_task.end(), static_cast<TASKER_FUNCTION_TYPES>(task)) != monitor_task.end()) {
          // Record start time in microseconds
          uint32_t start_time = micros();

          /****************************************************************************************************************
           * Thread: Call each module with the task
           *****************************************************************************************************************/ 
          mod->Tasker(task, obj);

          // Calculate elapsed time
          uint32_t elapsed_time = micros() - start_time;

          // Get the unique ID of the module
          uint16_t unique_id = mod->GetModuleUniqueID();

          // Update task metrics
          auto it = std::find_if(task_metrics.begin(), task_metrics.end(), [unique_id](const TaskMetrics& metrics) {
              return metrics.unique_id == unique_id;
          });

          if (it != task_metrics.end()) {
              // Update existing metrics
              it->total_time += elapsed_time;
              it->count++;
              it->avg_time = it->total_time / it->count;

              // Check if this task took the longest time for this tasker
              if (elapsed_time > it->max_time) {
                  it->max_time = elapsed_time;
                  it->task_id = static_cast<TASKER_FUNCTION_TYPES>(task);  // Save the task ID that took the longest
              }

              // Update min time
              it->min_time = std::min(it->min_time, elapsed_time);

          } else {
              // Add new entry for this tasker
              TaskMetrics new_metrics;
              new_metrics.task_id = static_cast<TASKER_FUNCTION_TYPES>(task);
              new_metrics.unique_id = unique_id;
              new_metrics.max_time = elapsed_time;
              new_metrics.min_time = elapsed_time;
              new_metrics.total_time = elapsed_time;
              new_metrics.count = 1;
              new_metrics.avg_time = elapsed_time;
              task_metrics.push_back(new_metrics);
          }
      } else {
          // If the task is not being monitored, just run it
          mod->Tasker(task, obj);
      }
  #else
      // If metrics are disabled, just run the task
      mod->Tasker(task, obj);
  #endif

    

    /****************************************************************************************************************
     * Debug: Stats
     *****************************************************************************************************************/ 
    #if defined(DEBUG_EXECUTION_TIME)  || defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES)
    uint32_t end_millis = millis(); // Remember start millis
    uint32_t this_millis = end_millis - start_millis; // Get this execution time 
    debug_module_time[debug_idx].last_loop_time = this_millis;
    debug_module_time[debug_idx].avg_time += this_millis;
    debug_module_time[debug_idx].avg_time /= 2;
    if(this_millis > debug_module_time[debug_idx].max_time)
    {
      debug_module_time[debug_idx].max_time = this_millis; // Save Max
      debug_module_time[debug_idx].max_function_id = task;
    }
    #endif 
    
    #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CLASSLIST "TIE_%d FUNC time %dms"), millis(), millis()-start_millis);
    #if defined(ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS)
      if(this_millis > ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS){
        AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CLASSLIST "TASKER %d ms %S %S"), millis()-start_millis, GetTaskName(task), mod->GetModuleName());
      }
    #endif // ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS
    #endif // ENABLE_ADVANCED_DEBUGGING

    #ifdef ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS
    uint32_t long_end_millis = millis() - start_millis;
    if(long_end_millis > ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS)
    {
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      ALOG_WRN(PSTR(D_LOG_CLASSLIST "LONG: %S %d-%S %d"), mod->GetModuleName(), task,GetTaskName(task), long_end_millis);
      #else // without task name ENABLE_DEBUG_FUNCTION_NAMES
      ALOG_WRN(PSTR(D_LOG_CLASSLIST "LONG: %S %d %d"),    mod->GetModuleName(), task,                   long_end_millis);
      #endif
    }
    #endif
 
  } // end for


  /****************************************************************************************************************
   * Debug: Boot progress
   *****************************************************************************************************************/ 
  #ifdef ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL
  if(!tkr_set->flag_boot_complete){
    char buffer_taskname[50];
    if(task != last_function){
      uint8_t boot_percentage = map(task,0,TASK_ON_BOOT_COMPLETE,0,100);
      //5% = 1 bar, 20 bars total [B                   ]
      //if(tkr_set->Settings.logging.serial_level >= LOG_LEVEL_INFO){
      #ifndef DISABLE_SERIAL_LOGGING
      DEBUG_PRINTF("[");
      for(uint8_t percent=0;percent<100;percent+=5){  
        if(percent<boot_percentage){ Serial.print((char)219); }else{ DEBUG_PRINTF(" "); }
      }      
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      DEBUG_PRINTF("] %03d %s\n\r",boot_percentage, GetTaskName(task, buffer_taskname));
      #else
      DEBUG_PRINTF("] %03d\n\r",boot_percentage);
      #endif // ENABLE_DEBUG_FUNCTION_NAMES
      #endif
      //}
      last_function = task;
    }
  }
  #endif // ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL


  if(task == TASK_ON_BOOT_COMPLETE){ tkr_set->runtime.flag_boot_complete = true; }
  
  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
  #endif

  return result;

}


void mTaskerManager::JSONCommand_Run(char* json)
{

  #ifdef USE_LIGHTING_TEMPLATE
  
    D_DATA_BUFFER_CLEAR();

    sprintf(data_buffer.payload.ctr, "%s", json);
    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);
    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything

    ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

    Tasker_Interface(TASK_JSON_COMMAND_ID);

  #endif // USE_LIGHTING_TEMPLATE

}


void mTaskerManager::addTasker(mTaskerInterface* mod)
{
  pModule.push_back(mod);
  Serial.printf("AddTasker[%d]\t%S\n\r", pModule.size(), mod->GetModuleName());
}


uint8_t mTaskerManager::Instance_Init()
{
    
  /**
   * @brief Core Modules
   **/
  #ifdef USE_MODULE_CORE_SETTINGS
  addTasker(new mSettings());
  #endif 
  #ifdef USE_MODULE_CORE_FILESYSTEM
  addTasker(new mFileSystem());
  #endif
  #ifdef USE_MODULE_CORE__JSON_TEMPLATE
  addTasker(new mJsonTemplate());
  #endif
  #ifdef USE_MODULE_CORE_HARDWAREPINS
  addTasker(new mHardwarePins());
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
  addTasker(new mSupport());
  #endif 
  #ifdef USE_MODULE_CORE_TIME
  addTasker(new mTime());
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
  addTasker(new mLogging());
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
  addTasker(new mTelemetry());
  #endif 
  #ifdef USE_MODULE_CORE_RULES
  addTasker(new mRuleEngine());
  #endif
  #ifdef USE_MODULE_CORE_UPDATES
  addTasker(new mUpdates());
  #endif
  #ifdef USE_MODULE_CORE__SERIAL
  addTasker(new mSerial());
  #endif
  #ifdef USE_MODULE_CORE_I2C
  addTasker(new mI2C());
  #endif
  #ifdef USE_MODULE_CORE_SPI
  addTasker(new mSPI());
  #endif
  #ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  addTasker(new mDevelopmentDebugging());
  #endif 
  /**
   * @brief Network
   **/
  #ifdef USE_MODULE_NETWORK_INTERFACE
  addTasker(new mInterfaceNetwork());
  #endif 
  #ifdef USE_MODULE_NETWORK_WIFI
  addTasker(new mWiFi());
  #endif 
  #ifdef USE_MODULE_NETWORK_ETHERNET
  addTasker(new mEthernet());
  #endif 
  #ifdef USE_MODULE_NETWORK_CELLULAR
  addTasker(new mCellular());
  #endif
  #ifdef USE_MODULE_NETWORK_MQTT
  addTasker(new mMQTTManager());
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  addTasker(new mWebServer());
  #endif
  /**
   * @brief Drivers
   **/
  #ifdef USE_MODULE_DRIVERS_INTERFACE
  addTasker(new mDriverInterface());
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
  addTasker(new mLEDs());
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
  addTasker(new mRelays());
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
  addTasker(new mPWM());
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  addTasker(new mIRtransceiver());
  #endif
  #ifdef USE_MODULE_DRIVERS_IRREMOTE
  addTasker(new mIRRemote());
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
  addTasker(new mRCSwitch());
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  addTasker(new mRCSwitch());
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
  addTasker(new mHBridge());
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
  addTasker(new mSDCard());
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  addTasker(new mShellyDimmer());
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  addTasker(new mBuzzer());
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_TONES
  addTasker(new mBuzzer());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
  addTasker(new OV2640());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
  addTasker(new mCameraOV2640());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM
  addTasker(new mWebCam());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
  addTasker(new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO
  addTasker(new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA
  addTasker(new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_MULTICLIENT
  addTasker(new mWebCamera());
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  addTasker(new mMAVLink_Decoder());
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI
  addTasker(new mMAVLink_Telemetry_WiFi());
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR
  addTasker(new mMAVLink_Telemetry_Cellular());
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  addTasker(new mSIM7000G());
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_800L
  addTasker(new mSIM800L());
  #endif
  /**
   * @brief Sensors
   **/
  #ifdef USE_MODULE_SENSORS_INTERFACE
  addTasker(new mSensorsInterface());
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
  addTasker(new mSwitches());
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
  addTasker(new mButtons());
  #endif
  #ifdef USE_MODULE_SENSORS_BME
  addTasker(new mBME());
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
  addTasker(new mSensorsDHT());
  #endif
  #ifdef USE_MODULE_SENSORS_BH1750
  addTasker(new mBH1750());
  #endif
  #ifdef USE_MODULE_SENSORS_PIR
  addTasker(new mPIR());
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
  addTasker(new mDoorSensor());
  #endif
  #ifdef USE_MODULE_SENSORS_L3G
  addTasker(new mSensorsL3G());
  #endif
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  addTasker(new mLDRBasic());
  #endif
  #ifdef USE_MODULE_SENSORS_LSM303D
  addTasker(new mSensorsLSM303D());
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
  addTasker(new mMoistureSensor());
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
  addTasker(new mSR04());
  #endif
  #ifdef USE_MODULE_SENSORS_MPU9250
  addTasker(new mSensorsMPU9250());
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  addTasker(new mPulseCounter());
  #endif
  #ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
  addTasker(new mRemoteDevice());
  #endif
  #ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
  addTasker(new mRotaryEncoder());
  #endif
  #ifdef USE_MODULE_SENSORS_SUN_TRACKING
  addTasker(new mSunTracking());
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
  addTasker(new mUltraSonicSensor());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL
  addTasker(new mADCInternal());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
  addTasker(new mADCInternal());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
  addTasker(new mDB18x20_ESP32());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  addTasker(new mDB18x20_ESP32());
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  addTasker(new mGPS_Serial());
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_MODEM
  addTasker(new mGPS_Modem());
  #endif
  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  addTasker(new mBattery_Modem());
  #endif
  /**
   * @brief Lights
   **/
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  addTasker(new mInterfaceLight());
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
  addTasker(new mAnimatorLight());
  #endif
  /**
   * @brief Energy
   **/
  #ifdef USE_MODULE_ENERGY_INTERFACE
  addTasker(new mEnergyInterface());
  #endif
  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  addTasker(new mEnergyPZEM004T());
  #endif
  #ifdef USE_MODULE_ENERGY_ADE7953
  addTasker(new mEnergyADE7953());
  #endif
  #ifdef USE_MODULE_ENERGY_INA219
  addTasker(new mEnergyINA219());
  #endif
  /**
   * @brief Displays
   **/
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
  addTasker(new mDisplaysInterface());
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  addTasker(new mNextionPanel());
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  addTasker(new mOLED_SSD1306());
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
  addTasker(new mOLED_SH1106());
  #endif
  /**
   * @brief Controller Generic
   **/
  #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  addTasker(new mSonoffIFan());
  #endif
  #ifdef USE_MODULE_CONTROLLER_FAN
  addTasker(new mFan());
  #endif
  // 3d printer encoder here
  #ifdef USE_MODULE_CONTROLLER_TANKVOLUME
  addTasker(new mTankVolume());
  #endif
  #ifdef USE_MODULE_CONTROLLER_BLINDS
  addTasker(new mBlinds());
  #endif
  #ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  addTasker(new mBucketWaterLevel());
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
  addTasker(new mDoorBell());
  #endif
  #ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
  addTasker(new mGPS_SD_Logger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
  addTasker(new mDoorBell());
  #endif
  #ifdef USE_MODULE_CONTROLLER_HVAC
  addTasker(new mHVAC());
  #endif
  #ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE
  addTasker(new mHVAC_Remote());
  #endif
  #ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  addTasker(new mSensorColourBar());
  #endif
  #ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  addTasker(new mRelayStateLEDStrip());
  #endif
  #ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
  addTasker(new mEnergyOnOLED());
  #endif
  /**
   * @brief Controller Custom
   **/  
  #ifdef USE_MODULE_CONTROLLER_RADIATORFAN
  addTasker(new mRadiatorFan());
  #endif
  #ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
  addTasker(new mImmersionTankColour());
  #endif
  #ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
  addTasker(new mFurnaceSensor());
  #endif
  #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
  addTasker(new mLouvoliteHub());
  #endif
  #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2
  addTasker(new mLouvoliteHub());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
  addTasker(new mSideDoorLight());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
  addTasker(new mImmersionPanel());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
  addTasker(new mTempSensorOLEDBath());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX
  addTasker(new mCellularBlackBox());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
  addTasker(new mMAVLink_Decoder_OLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
  addTasker(new mEnergyOLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
  addTasker(new mPrinter3D());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
  addTasker(new mEnergyOnOLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
  addTasker(new mSDLoggerIMURadiationPattern());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  addTasker(new mSerialPositionalLogger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
  addTasker(new mSerialCalibrationMeasurmentLogger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED
  addTasker(new mDeskSensorsOnOLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS
  addTasker(new mLightNeo_MotionAlerts());
  #endif
  #ifdef USE_MODULE_CONTROLLER_USERMOD_01
  addTasker(new mUserMod_01());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS
  addTasker(new mMavlinkFlyingLEDS());
  #endif

  Serial.printf(D_LOG_CLASSLIST "Loaded %d Modules\n\r",  pModule.size() );

  #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  debug_module_time.resize(pModule.size());
  #endif

};


#ifdef ENABLE_DEBUG_FUNCTION_NAMES
const char* mTaskerManager::GetTaskName(uint8_t task)
{
  
  switch(task){
    default:
                                                      return PM_SEARCH_NOMATCH;
    case TASK_POINTER_INIT:                           return PM_TASK_POINTER_INIT_CTR;
    case TASK_TEMPLATES__LOAD_MODULE:                 return PM_TASK_TEMPLATE_LOAD_CTR;
    case TASK_PRE_INIT:                               return PM_TASK_PRE_INIT_CTR;
    case TASK_INIT:                                   return PM_TASK_INIT_CTR;
    case TASK_CONFIGURE_MODULES_FOR_DEVICE:           return PM_TASK_CONFIGURE_MODULES_FOR_DEVICE_CTR;
    case TASK_LOOP:                                   return PM_TASK_LOOP_CTR;
    case TASK_EVERY_50_MSECOND:                       return PM_TASK_EVERY_50_MSECOND_CTR;
    case TASK_EVERY_100_MSECOND:                      return PM_TASK_EVERY_100_MSECOND_CTR;
    case TASK_EVERY_250_MSECOND:                      return PM_TASK_EVERY_250_MSECOND_CTR;
    case TASK_EVERY_SECOND:                           return PM_TASK_EVERY_SECOND_CTR;
    case TASK_EVERY_MINUTE:                           return PM_TASK_EVERY_MINUTE_CTR; 
    case TASK_EVERY_HOUR:                             return PM_TASK_EVERY_HOUR_CTR; 
    case TASK_EVERY_MIDNIGHT:                         return PM_TASK_EVERY_MIDNIGHT_CTR;
    case TASK_EVERY_MIDDAY:                           return PM_TASK_EVERY_MIDDAY_CTR;
    case TASK_ON_BOOT_SUCCESSFUL:                     return PM_TASK_ON_SUCCESSFUL_BOOT_CTR;
    case TASK_UPTIME_10_SECONDS:                      return PM_TASK_UPTIME_10_SECONDS_CTR;
    case TASK_UPTIME_1_MINUTES:                       return PM_TASK_UPTIME_1_MINUTES_CTR;
    case TASK_UPTIME_10_MINUTES:                      return PM_TASK_UPTIME_10_MINUTES_CTR;
    case TASK_UPTIME_60_MINUTES:                      return PM_TASK_UPTIME_60_MINUTES_CTR;
    case TASK_JSON_APPEND:                            return PM_TASK_JSON_APPEND_CTR;
    case TASK_SAVE_BEFORE_RESTART:                    return PM_TASK_SAVE_BEFORE_RESTART_CTR;
    case TASK_SETTINGS_DEFAULT:                       return PM_TASK_SETTINGS_DEFAULT_CTR;
    case TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:    return PM_TASK_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
    case TASK_SETTINGS_LOAD_VALUES_INTO_MODULE:       return PM_TASK_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
    case TASK_SETTINGS_SAVE_VALUES_FROM_MODULE:       return PM_TASK_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
    case TASK_FUNCTION_LAMBDA_INIT:                   return PM_TASK_FUNCTION_LAMBDA_INIT_CTR;
    case TASK_FUNCTION_LAMBDA_LOOP:                   return PM_TASK_FUNCTION_LAMBDA_LOOP_CTR;
    // case TASK_COMMAND:                                return PM_TASK_COMMAND_CTR;
    // case TASK_COMMAND_SENSOR:                         return PM_TASK_COMMAND_SENSOR_CTR;
    // case TASK_COMMAND_DRIVER:                         return PM_TASK_COMMAND_DRIVER_CTR;
    // case TASK_JSON_COMMAND:                           return PM_TASK_JSON_COMMAND_CTR;
    // case TASK_JSON_COMMAND_OBJECT:                    return PM_TASK_JSON_COMMAND_OBJECT_CTR;
    case TASK_WIFI_CONNECTED:                         return PM_TASK_WIFI_CONNECTED_CTR;
    case TASK_WIFI_DISCONNECTED:                      return PM_TASK_WIFI_DISCONNECTED_CTR;
    case TASK_MQTT_SUBSCRIBE:                         return PM_TASK_MQTT_SUBSCRIBE_CTR;
    // case TASK_MQTT_INIT:                              return PM_TASK_MQTT_INIT_CTR;
    case TASK_MQTT_CONNECTED:                         return PM_TASK_MQTT_CONNECTED_CTR;
    case TASK_MQTT_DISCONNECTED:                      return PM_TASK_MQTT_DISCONNECTED_CTR;
    case TASK_MQTT_COMMAND:                           return PM_TASK_MQTT_COMMAND_CTR;
    case TASK_MQTT_SENDER:                            return PM_TASK_MQTT_SENDER_CTR;
    case TASK_MQTT_HANDLERS_INIT:                     return PM_TASK_MQTT_HANDLERS_INIT_CTR;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:       return PM_TASK_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
    case TASK_SET_POWER:                              return PM_TASK_SET_POWER_CTR;
    case TASK_SET_DEVICE_POWER:                       return PM_TASK_SET_DEVICE_POWER_CTR;
    case TASK_SHOW_SENSOR:                            return PM_TASK_SHOW_SENSOR_CTR;
    case TASK_RULES_PROCESS:                          return PM_TASK_RULES_PROCESS_CTR;
    case TASK_SERIAL:                                 return PM_TASK_SERIAL_CTR;
    case TASK_FREE_MEM:                               return PM_TASK_FREE_MEM_CTR;
    case TASK_BUTTON_PRESSED:                         return PM_TASK_BUTTON_PRESSED_CTR;
    case TASK_ENERGY_RESET:                           return PM_TASK_ENERGY_RESET_CTR;
    case TASK_SENSOR_UPDATED:                         return PM_TASK_SENSOR_UPDATED_CTR;
    case TASK_STATUS_MESSAGE_APPEND:                  return PM_TASK_STATUS_MESSAGE_APPEND_CTR;
    case TASK_JSON_HARDWARE_APPEND:                   return PM_TASK_JSON_HARDWARE_APPEND_CTR;
    case TASK_SET_CHANNELS:                           return PM_TASK_SET_CHANNELS_CTR;
    case TASK_WEB_ADD_MAIN_BUTTON:                    return PM_TASK_WEB_ADD_MAIN_BUTTON_CTR;
    case TASK_WEB_ADD_BUTTON:                         return PM_TASK_WEB_ADD_BUTTON_CTR;
    case TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS:         return PM_TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
    case TASK_WEB_ADD_HANDLER:                        return PM_TASK_WEB_ADD_HANDLER_CTR;
    case TASK_WEB_ROOT_SEND_STYLE:                    return PM_TASK_WEB_ROOT_SEND_STYLE_CTR;
    case TASK_WEB_ROOT_SEND_SCRIPT:                   return PM_TASK_WEB_ROOT_SEND_SCRIPT_CTR;
    case TASK_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING:return PM_TASK_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
    case TASK_WEB_ROOT_SEND_BODY:                     return PM_TASK_WEB_ROOT_SEND_BODY_CTR;
    case TASK_WEB_ROOT_SEND_STATUS:                   return PM_TASK_WEB_ROOT_SEND_STATUS_CTR;
    case TASK_WEB_ADD_ROOT_SHOWS:                     return PM_TASK_WEB_ADD_ROOT_SHOWS_CTR;
    case TASK_WEB_ADD_FETCH_RESULT:              return PM_TASK_WEB_ADD_FETCH_RESULT_CTR;
    case TASK_WEB_ADD_DATA_FETCH_URL:            return PM_TASK_WEB_ADD_DATA_FETCH_URL_CTR;
    case TASK_WEB_ADD_ROOT_TABLE_ROWS:                return PM_TASK_WEB_ADD_ROOT_TABLE_ROWS_CTR;
    case TASK_WEB_ADD_ROOT_SCRIPT:                    return PM_TASK_WEB_ADD_ROOT_SCRIPT_CTR;
    case TASK_WEB_ADD_ROOT_STYLE:                     return PM_TASK_WEB_ADD_ROOT_STYLE_CTR;
    // case TASK_WEB_APPEND_LOADTIME_ROOT_URLS:          return PM_TASK_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
    // case TASK_WEB_APPEND_LOADTIME_ROOT_RATES:         return PM_TASK_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
    case TASK_WEB_APPEND_RUNTIME_ROOT_URLS:           return PM_TASK_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
    // case TASK_WEB_APPEND_RUNTIME_ROOT_RATES:          return PM_TASK_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED: return PM_TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED:    return PM_TASK_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
    case TASK_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS:       return PM_TASK_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
    case TASK_WEB_APPEND_ROOT_BUTTONS:                return PM_TASK_WEB_APPEND_ROOT_BUTTONS_CTR;
    case TASK_WEB_PAGEINFORMATION_SEND_MODULE:        return PM_TASK_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
    case TASK_WEB_COMMAND:                            return PM_TASK_WEB_COMMAND_CTR;
    // case TASK_CHECK_POINTERS:                         return PM_TASK_CHECK_POINTERS_CTR;
    case TASK_WEB_SYSTEM_INFO:                        return PM_TASK_WEB_SYSTEM_INFO_CTR;
    case TASK_DEBUG_CONFIGURE:                        return PM_TASK_DEBUG_CONFIGURE_CTR;
  }

}
#endif // ENABLE_DEBUG_FUNCTION_NAMES
