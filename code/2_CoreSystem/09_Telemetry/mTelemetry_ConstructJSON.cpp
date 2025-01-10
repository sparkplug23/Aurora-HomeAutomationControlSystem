/**
 * @file mTelemetry_ConstructJSON.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "mTelemetry.h"


uint8_t mTelemetry::ConstructJSON_LWT_Online(uint8_t json_level, bool json_appending)
{ 

  JBI->Start();
    JBI->Add("LWT", "Online");
    JBI->Add("ResetReason", "TBA");
  return JBI->End();
    
}


uint8_t mTelemetry::ConstructJSON_Health(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    JBI->Add(PM_TIME,           tkr_time->GetTime().c_str());
    JBI->Add(PM_UPTIME,         tkr_time->GetUptime().c_str());
    JBI->Add(PM_UPSECONDS,      tkr_time->UpTime());
    JBI->Add(PM_SLEEPMODE,      tkr_set->runtime.sleep ? "Dynamic" : "Unknown");
    JBI->Add(PM_SLEEP,          tkr_set->runtime.sleep); // typ. 20
    JBI->Add(PM_LOOPSSEC,       pCONT_sup->activity.cycles_per_sec); // typ. 50hz
    JBI->Add(PM_LOADAVERAGE,    tkr_set->runtime.loop_load_avg); // average loops_per_second
    JBI->Add(PM_FREEHEAP,       ESP.getFreeHeap());
    JBI->Add(PM_DEVICEFRIENDLYNAME, tkr_set->Settings.system_name.friendly);

    IPAddress localip = WiFi.localIP();
    JBI->Add_FV(PM_IPADDRESS, PSTR("\"%d.%d.%d.%d\""), localip[0],localip[1],localip[2],localip[3]);
    JBI->Add(PM_SSID,         WiFi.SSID().c_str());
    JBI->Add(PM_RSSI,         WiFi.RSSI());
    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    // JBI->Add(PM_CONNECTCOUNT, wifi_reconnects_counter);
    JBI->Add(PM_DOWNTIME,     "00T00:00:00");
    JBI->Add(PM_DOWNSECS,     (uint8_t)0);
    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
      
    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    // JBI->Object_Start(PM_MQTT);
    //   JBI->Add(PM_SENTCOUNT,       pCONT_mqtt->pubsub->stats.packets_sent_counter);
    //   JBI->Add(PM_RECEIVEDCOUNT,   pCONT_mqtt->pubsub->stats.packets_sent_counter);
    //   JBI->Add(PM_SENTPERMINUTE,   pCONT_mqtt->pubsub->stats.packets_sent_per_minute);
    // JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    
    // JBI->Add(PM_PAYLOAD_RATE,      tkr_time->RtcTime.hhmmss_ctr);
    // ALOG_INF(PSTR("JBI=\"%s\""),JBI->GetPtr());
  return JBI->End();
    
}

#ifndef FIRMWARE_MINIMAL2

uint8_t mTelemetry::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{ 
  char buffer[50];

  JBI->Start();
    JBI->Add(PM_MODULENAME,     tkr_set->Settings.system_name.friendly);
    JBI->Add_P(PM_FRIENDLYNAME,   pCONT_pins->ModuleName()); 
    JBI->Add(PM_ROOMHINT, tkr_set->Settings.room_hint);

    JBI->Add(PM_POWER,          tkr_set->runtime.power); 
    
    JBI->Add(PM_SETTINGS_HOLDER,tkr_set->Settings.cfg_holder);
    JBI->Add_FV(PM_SAVEADDRESS, PSTR("\"%X\""), tkr_set->GetSettingsAddress());
    JBI->Add(PM_SAVECOUNT,      tkr_set->Settings.save_flag);

    JBI->Add("Textbuffer", tkr_set->Settings.text_pool);
    
    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    JBI->Add(PM_POWERONSTATE,   tkr_set->Settings.poweronstate); 
    JBI->Add(PM_LEDSTATE,       tkr_set->Settings.ledstate);
    JBI->Add_FV(PM_LEDMASK,     PSTR("\"%04X\""), 0); 
    JBI->Add(PM_SAVEDATA,       0); 
    JBI->Add(PM_SAVESTATE,      0); 
    JBI->Add(PM_SWITCHMODE,     0);
    JBI->Add(PM_BAUDRATE,       (uint16_t)115200);
    JBI->Add(PM_BUTTONRETAIN,   tkr_set->Settings.flag_system.mqtt_button_retain); 
    JBI->Add(PM_SWITCHRETAIN,   tkr_set->Settings.flag_system.mqtt_switch_retain); 
    JBI->Add(PM_SENSORRETAIN,   tkr_set->Settings.flag_system.mqtt_sensor_retain); 
    JBI->Add(PM_POWERRETAIN,    tkr_set->Settings.flag_system.mqtt_power_retain);
    JBI->Add(PM_OTAURL,         D_OTA_URL);
    JBI->Add(PM_STARTUPUTC,     "2019-12-10T21:35:44");
    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Firmware(uint8_t json_level, bool json_appending){ //BuildHealth
  char buffer[30];
  JBI->Start();
  
    JBI->Add(PM_VERSION_NAME,     tkr_set->runtime.firmware_version.current.name_ctr);
    JBI->Add(PM_BUILDDATE,       __DATE__);
    
    JBI->Add_P(PM_SERIAL,          pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.serial_level), VALUE_IS_PROGMEM);
    JBI->Add(PM_BOOTCOUNT,       tkr_set->Settings.bootcount);
    JBI->Add(PM_BOOTCOUNTERRORS, tkr_set->Settings.bootcount_errors_only);
    JBI->Add(PM_BUILDDATETIME,   tkr_time->GetBuildDateAndTime().c_str());
    JBI->Add(PM_BUILDTIME,       __TIME__);
    JBI->Add(PM_VERSION_NUMBER,   (uint32_t)PROJECT_VERSION);
    JBI->Add(PM_VERSION_NUMBER_MINIMUM,   (uint32_t)PROJECT_VERSION_MINIMAL);
    JBI->Add(PM_VERSION_NUMBER_NOTYPE,   (uint32_t)PROJECT_VERSION & 0x3FFFFFFF); //suppres 2 MSBs

    JBI->Object_Start(PM_VERSION_PARTS);
      JBI->Object_Start(PM_CURRENT);
        JBI->Add(PM_TYPE,   (uint8_t)FIRMWARE_VERSION_TYPE);
        JBI->Add(PM_MAJOR,   (uint8_t)FIRMWARE_VERSION_MAJOR);
        JBI->Add(PM_MINOR,   (uint8_t)FIRMWARE_VERSION_MINOR);
        JBI->Add(PM_CORE,   (uint8_t)FIRMWARE_VERSION_CORE);
        JBI->Add(PM_MODULE,   (uint8_t)FIRMWARE_VERSION_MODULE);
      JBI->Object_End();
      JBI->Object_Start(PM_MINIMAL);
        JBI->Add(PM_TYPE,   (uint8_t)FIRMWARE_VERSION_TYPE_MINIMAL);
        JBI->Add(PM_MAJOR,   (uint8_t)FIRMWARE_VERSION_MAJOR_MINIMAL);
        JBI->Add(PM_MINOR,   (uint8_t)FIRMWARE_VERSION_MINOR_MINIMAL);
        JBI->Add(PM_CORE,   (uint8_t)FIRMWARE_VERSION_CORE_MINIMAL);
        JBI->Add(PM_MODULE,   (uint8_t)FIRMWARE_VERSION_MODULE_MINIMAL);
      JBI->Object_End();
    JBI->Object_End();

    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    JBI->Add(PM_ARDUINO_CORE,     ARDUINO_ESP8266_RELEASE); 
    // JBI->Add(PM_DRIVERS,         "1,2,3,4,5,6,7,8,9,10,12,14,16,17,18,19,20,21,22,24,26,30");
    // JBI->Add(PM_SENSORS,         "1,2,3,4,5,6,7,8,9,10,12,14,16,17,18,19,20,21,22,24,26,30");
    JBI->Add_FV(PM_FEATURES,     PSTR("[\"%08X\",\"%08X\",\"%08X\",\"%08X\"]"), 0,0,0,0);
    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    
    #ifdef ESP8266
    JBI->Add(PM_BOOTVERSION,     ESP.getBootVersion());
    #endif // ESP8266
    JBI->Add(PM_SDKVERSION,      ESP.getSdkVersion());    

    JBI->Add(PM_SDKVERSION,      ESP.getSdkVersion());    
    JBI->Add(PM_FREESKETCHSPACE,      ESP.getFreeSketchSpace());

    
    JBI->Array_Start(PM_FASTBOOT_RECOVERY);
    #ifdef ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
    JBI->Add("OTA");
    #endif
    #ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
    JBI->Add("HTTP");
    #endif
    #if !defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) && !defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
    JBI->Add("None");
    #endif
    JBI->Array_End();

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    JBI->Add(PM_WATCHDOG, "Enabled");
    // JBI->Add("WatchDogTimeout", D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS);
    #else
    JBI->Add(PM_WATCHDOG, "Disabled");
    #endif

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Log(uint8_t json_level, bool json_appending){ 
  char buffer[30];
  JBI->Start();
    JBI->Object_Start(PM_LOGLEVELS);
      JBI->Add_P(PM_SERIAL, pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.serial_level), VALUE_IS_PROGMEM);
      JBI->Add_P(PM_SYSTEM, pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.sys_level), VALUE_IS_PROGMEM);
      JBI->Add_P(PM_WEB,    pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.web_level), VALUE_IS_PROGMEM);
      JBI->Add_P(PM_TELNET, pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.telnet_level), VALUE_IS_PROGMEM);
    JBI->Object_End();
  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Memory(uint8_t json_level, bool json_appending){ // Debug info
  JBI->Start();
    JBI->Add(PM_PROGRAMSIZE,      ESP.getSketchSize()/1024);
    #ifdef ESP8266
    JBI->Add(PM_FREEMEMORY,       ESP.getFreeSketchSpace()/1024); // this takes seconds on esp32, what about here?... should I get and store on boot
    JBI->Add(PM_HEAPSIZE,         ESP.getFreeHeap()/1024);
    JBI->Add(PM_PROGRAMFLASHSIZE, ESP.getFlashChipSize()/1024);
    JBI->Add(PM_FLASHSIZE,        ESP.getFlashChipRealSize()/1024);
    JBI->Add(PM_FLASHCHIPID,      ESP.getFlashChipId());
    #else
    JBI->Add(PM_FREEMEMORY,       ESP.getFreeSketchSpace()/1024); // this takes seconds on esp32, what about here?... should I get and store on boot
    JBI->Add(PM_HEAPSIZE,         ESP.getFreeHeap()/1024);
    JBI->Add(PM_PROGRAMFLASHSIZE, ESP.getFlashChipSize()/1024);
    JBI->Add(PM_FLASHSIZE,        ESP.getFlashChipRealSize()/1024);
    // JBI->Add(PM_FLASHCHIPID,      ESP.getFlashChipId());
    #endif // ESP8266
    JBI->Add(PM_FLASHMODE,        (uint8_t)ESP.getFlashChipMode()); //FlashMode_t
  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Network(uint8_t json_level, bool json_appending)
{
   
  IPAddress localip   = WiFi.localIP();
  IPAddress staticip  = IPAddress(tkr_set->Settings.ip_address[0]);
  IPAddress gatewayip = IPAddress(tkr_set->Settings.ip_address[1]);
  IPAddress subnetip  = IPAddress(tkr_set->Settings.ip_address[2]);
  IPAddress dnsip     = IPAddress(tkr_set->Settings.ip_address[3]);

  JBI->Start();
    JBI->Add_FV(PM_IPADDRESS,PSTR("\"%d.%d.%d.%d\""),localip[0],localip[1],localip[2],localip[3]);
    JBI->Add(PM_SSID, WiFi.SSID().c_str());
    JBI->Add(PM_SSID_NUMBERED, tkr_set->Settings.sta_active); // Used to debug switching in grafana
    JBI->Add(PM_RSSI, WiFi.RSSI());
    // JBI->Add(PM_CONNECTCOUNT, wifi_reconnects_counter);
    JBI->Add(PM_HOSTNAME, tkr_set->runtime.my_hostname);
    JBI->Add_P(PM_TELNET_PORT, TELNET_PORT);
    JBI->Add_FV(PM_STATIC_IPADDRESS,PSTR("\"%d.%d.%d.%d\""),staticip[0],staticip[1],staticip[2],staticip[3]);
    JBI->Add_FV(PM_GATEWAY,PSTR("\"%d.%d.%d.%d\""),gatewayip[0],gatewayip[1],gatewayip[2],gatewayip[3]);
    JBI->Add_FV(PM_SUBNETMASK,PSTR("\"%d.%d.%d.%d\""),subnetip[0],subnetip[1],subnetip[2],subnetip[3]);
    JBI->Add_FV(PM_DNSSERVER,PSTR("\"%d.%d.%d.%d\""),dnsip[0],dnsip[1],dnsip[2],dnsip[3]);
    JBI->Add(PM_BSSID, WiFi.BSSIDstr().c_str());
    JBI->Add(PM_MAC, WiFi.macAddress().c_str());
    JBI->Add(PM_WEBSERVER_ENABLED, tkr_set->Settings.webserver);
    JBI->Add(PM_WIFICONFIG_STATE, tkr_set->Settings.sta_config);

    JBI->Array_Start(PM_AP_LIST);
      JBI->Add(tkr_set->SettingsText(SET_STASSID1));
      JBI->Add(tkr_set->SettingsText(SET_STASSID2));
    JBI->Array_End();

  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_MQTT(uint8_t json_level, bool json_appending){

  JBI->Start();


    /**
     * @brief Show wrapper stuff
     * 
     */

    // JBI->Add("RetrySecs", pCONT_mqtt->dt.connection[0].retry);

    JBI->Object_Start(PM_REFRESH_RATES);
      JBI->Add(PM_MQTT_REFRESH_RATE_IFCHANGED, pCONT_mqtt->dt.ifchanged_secs);
      JBI->Add(PM_MQTT_REFRESH_RATE_TELEPERIOD, pCONT_mqtt->dt.teleperiod_secs);
    JBI->Object_End();
    
    JBI->Add(PM_MQTT_ENABLE_RESTART,   (uint8_t)0);

// {
//   "ClientName":"tg_lighting_70-08:D1:F9:CA:0B:74",
//   "RetrySecs":10,
//   "RefreshRates":{
//     "IfChanged":10,
//     "Teleperiod":60},
//     "MQTT Enable Restart":0,
//     "Instance":[
//       [
//         "broker_url":"192.168.1.70",
//         "port":1883,"connect_count":0,"retry_counter":0,"downtime_counter":0,"connected":1,"allowed":0,"mqtt_tls":0,,"tSaved_LastOutGoingTopic":18664,"flag_start_reconnect":0,"cConnectionAttempts":0,"host_server_type":0]],"Connection":[["status":1,"client_name":"tg_lighting_70-08:D1:F9:CA:0B:74","prefixtopic":"tg_lighting_70"]]}



    #ifdef ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY
    JBI->Object_Start("Interface_Priority");
      // JBI->Add(D_MODULE_ENERGY_INTERFACE_CTR, pCONT_mqtt->dt.connection[0].interface_reporting_priority.energy);
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY


    /**
     * @brief Show each instance info
     * 
    **/
    // #ifdef USE_MODULE_NETWORK_MQTT
    JBI->Array_Start("Connection");
    for(auto& con:pCONT_mqtt->brokers)
    {      
      JBI->Object_Start();
        JBI->Add("host_address", con->host_address);
        JBI->Add("port", con->port);

    //     JBI->Add("connect_count", con->connect_count);
    //     JBI->Add("retry_counter", con->retry_counter);
    //     JBI->Add("downtime_counter", con->downtime_counter);
    //     // JBI->Add("initial_connection_state", con->initial_connection_state);
    //     JBI->Add("connected", con->connected);
    //     JBI->Add("allowed", con->allowed);
    //     JBI->Add("mqtt_tls", con->mqtt_tls);

    //     JBI->Add("mqtt_client_type", con->client_type);

    //     JBI->Add("tSaved_LastOutGoingTopic", con->tSaved_LastOutGoingTopic);

    //     JBI->Add("flag_start_reconnect", con->flag_start_reconnect);
    //     JBI->Add("cConnectionAttempts", con->cConnectionAttempts);

    //     JBI->Add("host_server_type", con->host_server_type);

    //     #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    //     JBI->Add("payload_publish_sent", con->debug_stats.payload_publish_sent);
    //     JBI->Add("payload_publish_missed", con->debug_stats.payload_publish_missed);
    //     JBI->Add("payload_publish_success_percentage", con->debug_stats.payload_publish_success_percentage);
    //     #endif
      JBI->Object_End();

    }
    JBI->Array_End();

    JBI->Array_Start("Manager");
    for(auto& con:pCONT_mqtt->brokers)
    {
      JBI->Object_Start();
        JBI->Add("status", con->status);
        JBI->Add("host_address", con->host_address);
        JBI->Add("port", con->port);
        JBI->Add("client", con->client_name);
        JBI->Add("user", con->user);
        JBI->Add("pwd", con->password);
        JBI->Add("retry", con->retry);
        JBI->Add("prefixtopic", con->prefix_topic);
      JBI->Object_End();
    }
    JBI->Array_End();
    
  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Time(uint8_t json_level, bool json_appending){ 

  char buffer[100];
       
  JBI->Start();
    JBI->Add(PM_UTC_TIME,   tkr_time->GetDateAndTime(DT_UTC).c_str() );
    JBI->Add(PM_LOCAL_TIME, tkr_time->GetDateAndTime(DT_LOCAL).c_str() );
    JBI->Add(PM_STARTDST,   tkr_time->GetDateAndTime(DT_DST).c_str() );
    JBI->Add(PM_ENDDST,     tkr_time->GetDateAndTime(DT_STD).c_str() );
    JBI->Add(PM_TIMEZONE,   tkr_time->GetDateAndTime(DT_TIMEZONE).c_str() );
    JBI->Add(PM_SUNRISE,    tkr_time->GetDateAndTime(DT_SUNRISE).c_str() );
    JBI->Add(PM_SUNSET,     tkr_time->GetDateAndTime(DT_SUNSET).c_str() );
  return JBI->End();

}


/**
 * needs refactored into something else, perhaps everything moved into better named topics
 * 
 * 
 * */
uint8_t mTelemetry::ConstructJSON_Debug_Devices(uint8_t json_level, bool json_appending)
{ 

  JBI->Start();

  /**
   * @brief DeviceNameBuffer, should become a debug topic
   * 
   */
    JBI->Add(PM_MODULENAME,          tkr_set->Settings.module);
    // If debug mode, show entire message
    // JBI->Array_AddArray(PM_DEVICEID, tkr_set->Settings.device_name_buffer.device_id, ARRAY_SIZE(tkr_set->Settings.device_name_buffer.device_id));
    // JBI->Array_AddArray(PM_CLASSID,  tkr_set->Settings.device_name_buffer.class_id,  ARRAY_SIZE(tkr_set->Settings.device_name_buffer.class_id));
    // else, only show ysed
    JBI->Add("DLI->GetLengthIndexUsed()", DLI->GetLengthIndexUsed());
    JBI->Array_AddArray(PM_DEVICEID, tkr_set->Settings.device_name_buffer.device_id, DLI->GetLengthIndexUsed());
    JBI->Array_AddArray(PM_CLASSID,  tkr_set->Settings.device_name_buffer.class_id,  DLI->GetLengthIndexUsed());  
    JBI->Add(PM_BUFFER,        tkr_set->Settings.device_name_buffer.name_buffer);
    JBI->Add(PM_BUFLEN,        strlen(tkr_set->Settings.device_name_buffer.name_buffer));

    uint8_t count = 0;
    if(strlen(tkr_set->Settings.device_name_buffer.name_buffer)){ //if anything in buffer
      count = DLI->CountCharInCtr(tkr_set->Settings.device_name_buffer.name_buffer,'|'); // first | indicates index from 0
    }
    JBI->Add("ItemCount", count);

    
    if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID)&&pCONT_pins->PinUsed(GPIO_I2C_SDA_ID))
    {

      #ifdef ESP32
      JBI->Add("I2C_BusSpeed", pCONT_i2c->wire->getClock());
      #endif

      char mqtt_data[300];
      pCONT_i2c->I2cScan(mqtt_data, sizeof(mqtt_data));
      // Serial.println(mqtt_data);

      //need to escape option to function above
      // JBI->Add("I2C_Scan",          mqtt_data);

      JBI->Append(",\"I2C_Scan\":");
      JBI->Append(mqtt_data);

    }

    // BufferWriterI->Append(",\"I2C_Scan\":");

    #ifdef USE_FUNCTION_TEMPLATE
    //JBI->Add("Function Template",   FUNCTION_TEMPLATE);
    #endif

  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Reboot(uint8_t json_level, bool json_appending){ // 

  JBI->Start();
  
  JBI->Add(PM_DEVICE, tkr_set->Settings.system_name.device);
  JBI->Add(PM_DEVICEFRIENDLYNAME, tkr_set->Settings.system_name.friendly);
  JBI->Add_FV(PM_DATETIME, PSTR("\"%02d-%02d-%02d %02d:%02d:%02d\""),
                      tkr_time->RtcTime.day_of_month, tkr_time->RtcTime.month, tkr_time->RtcTime.year,
                      tkr_time->RtcTime.hour, tkr_time->RtcTime.minute, tkr_time->RtcTime.second
                    );
  JBI->Object_Start(PM_COUNTER);
    JBI->Add("All", (uint8_t)0);
    // JBI->Add("WDT", (uint8_t)0);
  JBI->Object_End();

  // if (pCONT_sup->CrashFlag()) {
    
  JBI->Object_Start(PM_CRASHDUMP);
    pCONT_sup->WriteBuffer_P(PSTR(","));
    pCONT_sup->CrashDump_AddJson();
  JBI->Object_End();

  // } else {
  //   char buffer[30];
  //   JBI->Add("Reason", pCONT_sup->GetResetReason(buffer, sizeof(buffer)));
  // }

  return JBI->End();

}




/****
 * 
 * 
 * 
 * Debug packets that are not part of the main telemetry and only used for debugging/developing the device
 * 
 * 
 * 
 * ****/

uint8_t mTelemetry::ConstructJSON_Debug_Minimal(uint8_t json_level, bool json_appending)
{

  IPAddress localip = WiFi.localIP();
  
  JBI->Start();
    JBI->Add(PM_UPTIME,         tkr_time->GetUptime().c_str());// PSTR("\"%02dT%02d:%02d:%02d\""), tkr_time->uptime.day_of_year,tkr_time->uptime.hour,tkr_time->uptime.minute,tkr_time->uptime.second);
    JBI->Add(PM_UPSECONDS,      tkr_time->UpTime());//uptime_seconds_nonreset);
    JBI->Add(PM_SLEEP,          pCONT_sup->loop_delay_temp);
    JBI->Add(PM_LOOPSSEC,       pCONT_sup->activity.cycles_per_sec);
    JBI->Add(PM_LOOPRATIO,      pCONT_sup->this_cycle_ratio);
    // #ifdef USE_NETWORK_MDNS
    // JBI->Add(PM_MDNS,           tkr_set->runtime.boot_status.mdns_started_succesfully);
    // #endif // #ifdef USE_NETWORK_MDNS
    JBI->Add(PM_FREEHEAP,       ESP.getFreeHeap());
    JBI->Add(PM_VERSION_NAME,    tkr_set->runtime.firmware_version.current.name_ctr);
    JBI->Add_FV(PM_IPADDRESS,   PSTR("\"%d.%d.%d.%d\""), localip[0],localip[1],localip[2],localip[3]);
    JBI->Add(PM_BOOTCOUNT,      tkr_set->Settings.bootcount);
  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Debug_Pins(uint8_t json_level, bool json_appending)
{

  // return 0;

  char buffer[30];
  JBI->Start();
  JBI->Add("flag_serial_set_tx_set",pCONT_pins-> flag_serial_set_tx_set);
    // JBI->Object_Start(PM_GPIO);
    // for(uint16_t i=0;i<sizeof(tkr_set->pin);i++){ 
    //   if(pCONT_pins->PinUsed(i)){ // skip pins not configured
    //     sprintf_P(buffer, PSTR("TASK_%d"), i);
    //     JBI->Add(buffer, pCONT_pins->GetPin(i));
    //   }
    // }
    // JBI->Object_End();


    JBI->Object_Start(PM_GPIO);
    for(uint16_t i=0;i<ARRAY_SIZE(pCONT_pins->pin_attached_gpio_functions);i++)
    {
      if(pCONT_pins->PinUsed(pCONT_pins->pin_attached_gpio_functions[i]))
      {
        sprintf_P(buffer, PSTR("%s"), pCONT_pins->GetGPIOFunctionNamebyID(pCONT_pins->pin_attached_gpio_functions[i], buffer, sizeof(buffer)));
        JBI->Add(buffer, pCONT_pins->GetPin(pCONT_pins->pin_attached_gpio_functions[i]));
      }
    }    
    JBI->Object_End();


    // JBI->Object_Start(D_GPIO "_map");
    // for(uint16_t i=0;i<MAX_USER_PINS;i++){ 
    //   sprintf_P(buffer, PSTR("%d"),
      
    //   gpio_pin_by_index[i]
    //   //  i
    //    );
    //   JBI->Add(buffer, pCONT_pins->GetPin(i));
    // }
    // JBI->Object_End();

    // Debug by printing all arrays out
    JBI->Array_Start("pin_attached_gpio_functions");
    for(int i=0; i<ARRAY_SIZE(pCONT_pins->pin_attached_gpio_functions);i++)
      JBI->Add(pCONT_pins->pin_attached_gpio_functions[i]);
    JBI->Array_End();

    JBI->Array_Start("user_template_io");
    for(int i=0; i<ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io);i++)
      JBI->Add(tkr_set->Settings.user_template.hardware.gp.io[i]);
    JBI->Array_End();

    JBI->Array_Start("getpin");
    for(int i=0; i<ARRAY_SIZE(pCONT_pins->pin_attached_gpio_functions);i++)
    {
// DEBUG_LINE_HERE;
      JBI->Add(
        pCONT_pins->GetPin(pCONT_pins->pin_attached_gpio_functions[i])
        
        
        );
// DEBUG_LINE_HERE;
    }
    JBI->Array_End();





  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Debug_Template(uint8_t json_level, bool json_appending){ //BuildHealth
  char buffer[50];
  JBI->Start();

    JBI->Object_Start("TemplateLoading");
      JBI->Add("Module",             tkr_set->runtime.template_loading.status.module);
      JBI->Add("Function",           tkr_set->runtime.template_loading.status.function);
      JBI->Add("Lighting",           tkr_set->runtime.template_loading.status.lighting);
      JBI->Add("NextionHMIInputMap", tkr_set->runtime.template_loading.status.nextion_hmi_input_map);
      JBI->Add("Rules",              tkr_set->runtime.template_loading.status.rules);
    JBI->Object_End();

    JBI->Add_P(PM_MODULENAME, pCONT_pins->AnyModuleName(tkr_set->Settings.module));
    JBI->Add(PM_MODULEID,   tkr_set->Settings.module);
    JBI->Add("MyModuleType",tkr_set->runtime.my_module_type);
    myio cmodule;
    pCONT_pins->TemplateGPIOs(&cmodule);
    // JBI->Array_AddArray(PM_GPIO, cmodule.io, (uint8_t)sizeof(cmodule.io));
    JBI->Array_Start_P(PM_GPIO);
    for(int i=0;i<ARRAY_SIZE(cmodule.io);i++)
      JBI->Add(cmodule.io[i]);
    JBI->Array_End();
  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Debug_ModuleInterface(uint8_t json_level, bool json_appending){ //BuildHealth

  JBI->Start();
  
  // #ifdef DEBUG_EXECUTION_TIME
  //   char buffer[50];
  //   JBI->Object_Start(pCONT->GetModuleFriendlyName(pCONT->module_settings.list[ii], buffer));
  //     JBI->Array_AddArray("average", pCONT->module_settings.execution_time_average_ms, sizeof(pCONT->module_settings.execution_time_average_ms));
  //     JBI->Array_AddArray("max",     pCONT->module_settings.execution_time_max_ms,     sizeof(pCONT->module_settings.execution_time_max_ms));
  //   JBI->Object_End();
  // #endif

  // JBI->Object_Start("ModuleSize");

  
  // // for(uint8_t i=0;i<pCONT->module_settings.count;i++){
  // //   JBI->Add_P(pCONT->GetModuleFriendlyName(pCONT->module_settings.list[i]),pCONT->GetClassSizeByID(pCONT->module_settings.list[i]));
  // //   // if(pCONT->GetClassSizeByID(i)>10000){
  // //   //   JBI->Add("bad",i);
  // //   // }
  // // }

  // /**
  //  * @brief Add array of all unique id's (in a json array, this will just replace... so maybe use name+id? or rather ID_NAME so it will be easier to spot numbers the same)
  //  * I could also run a "append" id but check its not in it already (easier with vector?)
  //  * 
  //  */
  // char buffer[100] = {0};

  // JBI->Array_Start("ModuleIDs");
  // for(auto& mod: pCONT->pModule)
  // {
  //   snprintf_P(buffer, sizeof(buffer), PSTR("%04d_%S"), mod->GetModuleUniqueID(), mod->GetModuleName()  );
  //   JBI->Add(buffer);
  // }
  // JBI->Array_End();

  // JBI->Object_End();


  return JBI->End();
}


/**
 * mSettings::SystemSettings_DefaultBody_Sensors
 * Debugging of all values listed inside the settings struct, it shall all be outputted here, mqtt size permitted
 * */
uint8_t mTelemetry::ConstructJSON_Debug_System_Stored_Settings(uint8_t json_level, bool json_appending)
{ //BuildHealth
  char buffer[50];
  JBI->Start();

  JBI->Object_Start("Header");
    JBI->Add("cfg_holder", tkr_set->Settings.cfg_holder);
    JBI->Add("cfg_size", tkr_set->Settings.cfg_size);
    JBI->Add("save_flag", tkr_set->Settings.save_flag);
    JBI->Add("version", tkr_set->Settings.version);
    JBI->Add("bootcount", tkr_set->Settings.bootcount);
  JBI->Object_End();


  // JBI->Object_Start("Animations");
  // //   JBI->Array_AddArray("controller", 
  // //   &tkr_set->Settings.animation_settings.xmas_controller_params, 
  // //   sizeof(tkr_set->Settings.animation_settings.xmas_controller_params));
  // // JBI->Object_End();
  // // JBI->Object_Start("Animations1");
  //   // JBI->Array_Start("controller"); 
  //   // &tkr_set->Settings.animation_settings.xmas_controller_params, 
  //   // sizeof(tkr_set->Settings.animation_settings.xmas_controller_params));

  //   // for(int i=0;i<10;i++)
  //   // {
  //   //   JBI->Add(tkr_set->Settings.animation_settings.xmas_controller_params[i]);
  //   // }

  //   // JBI->Array_End();
  // JBI->Object_End();

  /**
   * Read stored values into temp struct, not use local variables
   * */

//   struct SYSCFGH {
//   uint16_t      cfg_holder;                // 000 v6 header
//   uint16_t      cfg_size;                  // 002
//   unsigned long save_flag;                 // 004
//   unsigned long version;                   // 008
//   uint16_t      bootcount;                 // 00C
//   uint16_t      cfg_crc;                   // 00E
//   } _SettingsH;

//     extern "C" uint32_t _FS_end;
// const uint32_t SPIFFS_END = ((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;
// const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area
// uint32_t flash_location = SETTINGS_LOCATION;
// // Just read the header
//  ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(_SettingsH));
    




    // // JBI->Add("ALTITUDE_ABOVE_SEALEVEL",       (float)ALTITUDE_ABOVE_SEALEVEL);
    // JBI->Add("altitude",     tkr_set->Settings.sensors.altitude);
    // // JBI->Add("LATITUDE",                      (float)LATITUDE);
    // JBI->Add("latitude",     tkr_set->Settings.sensors.latitude);
    // // JBI->Add("LONGITUDE",       (float)LONGITUDE);
    // JBI->Add("longitude",                     tkr_set->Settings.sensors.longitude);

  return JBI->End();
}


#ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
uint8_t mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance(uint8_t json_level, bool json_appending)
{
  
    // //test devices
    // JBI->Object_Start("Test");
    //   JBI->Add("activity.loop_counter", pCONT_sup->activity.loop_counter);
    // //   JBI->Add("sleep", tkr_set->sleep);
    // //   JBI->Add("loop_runtime_millis", pCONT_sup->loop_runtime_millis);
    // //   JBI->Add("loops_per_second", pCONT_sup->loops_per_second);
    // //   JBI->Add("this_cycle_ratio", pCONT_sup->this_cycle_ratio);
    // //   JBI->Add("loop_load_avg", tkr_set->loop_load_avg);
    // //   JBI->Add("enable_sleep", tkr_set->Settings.enable_sleep);
    // JBI->Object_End();
    // // test end

  JBI->Start();

  char buffer2[100];
  
  // for(int ii=0;ii<pCONT->GetClassCount();ii++)
  // {
  //   JBI->Level_Start_P(pCONT->pModule[ii]->GetModuleName());

  //     JBI->Add("max_time", pCONT->debug_module_time[ii].max_time);
  //     JBI->Add("avg_time", pCONT->debug_module_time[ii].avg_time);
  //     JBI->Add("max_function_id", pCONT->debug_module_time[ii].max_function_id);

  //   JBI->Object_End();




  // }







  //   JBI->Add_P(pCONT->GetModuleFriendlyName(pCONT->module_settings.list[i]),pCONT->GetClassSizeByID(pCONT->module_settings.list[i]));
  //   // if(pCONT->GetClassSizeByID(i)>10000){
  //   //   JBI->Add("bad",i);
  //   // }



  // }

  // /**
  //  * @brief Add array of all unique id's (in a json array, this will just replace... so maybe use name+id? or rather ID_NAME so it will be easier to spot numbers the same)
  //  * I could also run a "append" id but check its not in it already (easier with vector?)
  //  * 
  //  */
  // char buffer[100];



  // JBI->Array_Start("ModuleIDs");
  // for(int ii=0;ii<pCONT->GetClassCount();ii++)
  // {
  //   snprintf_P(buffer, sizeof(buffer), PSTR("%04d_%S"), pCONT->pModule[ii]->GetModuleUniqueID(), pCONT->pModule[ii]->GetModuleFriendlyName()  );

  //   JBI->Add(buffer);
  // }
  // JBI->Array_End();

  // JBI->Object_End();


  return JBI->End();
}
#endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES


/**
 * @brief Debug time
 * 
 */


  //   #ifdef DEBUG_MODULE_TIME_STD
  //   JBI->Object_Start("debug_v2");
  //     JBI->Add("utc_time",tkr_time->Rtc.utc_time);
  //     JBI->Add("local_time",tkr_time->Rtc.local_time);
  //     JBI->Add("daylight_saving_time",tkr_time->Rtc.daylight_saving_time);
  //     JBI->Add("standard_time",tkr_time->Rtc.standard_time);
  //     JBI->Add("ntp_time",tkr_time->Rtc.ntp_time);
  //     JBI->Add("midnight",tkr_time->Rtc.midnight);
  //     JBI->Add("restart_time",tkr_time->Rtc.restart_time);
  //     JBI->Add("millis",tkr_time->Rtc.millis);
  //     JBI->Add("last_sync",tkr_time->Rtc.last_sync);
  //     JBI->Add("time_timezone",tkr_time->Rtc.time_timezone);
  //     JBI->Add("ntp_sync_minute",tkr_time->Rtc.ntp_sync_minute);
  //     JBI->Add("midnight_now",tkr_time->Rtc.midnight_now);
  //     JBI->Add("user_time_entry",tkr_time->Rtc.user_time_entry);
  //     JBI->Add("ntp_last_active_secs", (millis()-tkr_time->Rtc.ntp_last_active)/1000);
  //     JBI->Add("last_sync_secs", (tkr_time->Rtc.utc_time-tkr_time->Rtc.last_sync)/1000);
  //     JBI->Add("GetUptime",tkr_time->GetUptime().c_str());
  //     JBI->Object_Start("DST");
  //       JBI->Add("IsDst", tkr_time->IsDst());
  //       int32_t dstoffset = tkr_set->Settings.toffset[1] * tkr_time->SECS_PER_MIN;
  //       int32_t stdoffset = tkr_set->Settings.toffset[0] * tkr_time->SECS_PER_MIN;
  //       JBI->Add("utc_time>=dst_eoy",tkr_time->Rtc.utc_time >= (tkr_time->Rtc.daylight_saving_time - stdoffset));
  //       JBI->Add("utc_time<standard_soy",tkr_time->Rtc.utc_time < (tkr_time->Rtc.standard_time - dstoffset));

  //       JBI->Add("diff_sod", tkr_time->Rtc.utc_time - (tkr_time->Rtc.daylight_saving_time - stdoffset));
  //       JBI->Add("dif_eod",tkr_time->Rtc.utc_time - (tkr_time->Rtc.standard_time - dstoffset));
  //     JBI->Object_End();
  //     JBI->Add("toffset[0]", tkr_set->Settings.toffset[0]);
  //     JBI->Add("toffset[1]", tkr_set->Settings.toffset[1]);
  //   JBI->Object_End();    
  //   // JBI->Object_Start("RtcTime");
  //   //   JBI->Add("valid",tkr_time->RtcTime.valid);
  //   //   JBI->Add_FV("time","\"%02d:%02d:%02d\"",tkr_time->RtcTime.hour,tkr_time->RtcTime.minute,tkr_time->RtcTime.second);
  //   // JBI->End();
  // #endif //  DEBUG_MODULE_TIME_STD





#ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
uint8_t mTelemetry::ConstructJSON_Debug__Settings_Storage(uint8_t json_level, bool json_appending)
{

  mqtthandler_debug__settings_storage.tRateSecs = 1;

  JBI->Start();

  char buffer2[100];
  
  JBI->Add("bootcount", tkr_set->Settings.bootcount);

  return JBI->End();
}
#endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES

#endif // #ifndef FIRMWARE_MINIMAL2