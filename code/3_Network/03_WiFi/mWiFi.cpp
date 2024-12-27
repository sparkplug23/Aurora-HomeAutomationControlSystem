#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

#ifdef ESP8266
extern "C" {
 #include "user_interface.h"
}
#endif

// Used for timed on or off events
int8_t mWiFi::Tasker(uint8_t function, JsonParserObject obj){

  #ifdef ENABLE_DEVFEATURE_NETWORK__BLOCK_CONNECT_PUSH_BACKOFF_LONG_AS_TEMP_SOLUTION_TO_NO_WIFI
  return 0;
  #endif

  switch(function){
    case TASK_INIT:
      #ifndef ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025
      WifiConnect();
      #endif // ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025
      #ifdef ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025

      #endif 
    break;
    case TASK_LOOP: 
    
      #if defined(USE_NETWORK_MDNS) && defined(ESP8266)
        MdnsUpdate();
      #endif // USE_NETWORK_MDNS
    
    break;
    case TASK_EVERY_SECOND:{
      
      AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "network_wifi=%d"), pCONT_set->Settings.flag_network.network_wifi);

      #ifndef ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025
      if (pCONT_set->Settings.flag_network.network_wifi) 
      {
        WifiCheck(pCONT_set->runtime.wifi_state_flag);
        pCONT_set->runtime.wifi_state_flag = WIFI_RESTART;
      }

      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_ssid[%d]=%s"),pCONT_set->Settings.sta_active, pCONT_set->SettingsText(SET_STASSID1 + pCONT_set->Settings.sta_active) );
      AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_pwd[%d]=%s"), pCONT_set->Settings.sta_active, pCONT_set->SettingsText(SET_STAPWD1 + pCONT_set->Settings.sta_active) );
      #endif
      #endif // ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025

    }
    break;
    case TASK_EVERY_MINUTE:
    
// #ifdef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
//       if (pCONT_set->Settings.flag_network.network_wifi) 
//       {
//         WifiCheck(pCONT_set->wifi_state_flag);
//         pCONT_set->wifi_state_flag = WIFI_RESTART;
//       }
// #endif // ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

// #ifdef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

      ALOG_INF( PSTR("WL_CONNECTED %s"), WiFi.localIP().toString().c_str() );

// #endif

    break;
    case TASK_EVERY_FIVE_MINUTE:
      // ALOG_INF( PSTR("WL_CONNECTED %s"), WiFi.localIP().toString().c_str() );
    break;
    case TASK_WIFI_CONNECTED:{

      #ifdef USE_NETWORK_MDNS
        StartMdns();
      #endif  // USE_NETWORK_MDNS

      #ifndef ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

        ALOG_HGL(PSTR("Start MQTTConnection with WiFi"));

        #ifdef USE_MODULE_NETWORK_MQTT

          mqtt_client = new WiFiClient();

          pCONT_mqtt->CreateConnection(mqtt_client, MQTT_HOST, MQTT_PORT, CLIENT_TYPE_WIFI_ID);
          
          pCONT_mqtt->brokers.back()->SetCredentials(MQTT_USER, MQTT_PASS);

          pCONT_mqtt->brokers.back()->SetReConnectBackoffTime(MQTT_RETRY_SECS);
          
          // char client_name[100]; snprintf_P(client_name, sizeof(client_name), PSTR("%s-%s"), pCONT_set->Settings.system_name.device, WiFi.macAddress().c_str()); 
          
          uint8_t mac[6];           WiFi.macAddress(mac);
          char client_name[100]; snprintf_P(client_name, sizeof(client_name), PSTR("%s-%02X:%02X:%02X"), pCONT_set->Settings.system_name.device, mac[3], mac[4], mac[5]); 
          pCONT_mqtt->brokers.back()->SetClientName(client_name);

          pCONT_mqtt->brokers.back()->SetTopicPrefix(pCONT_set->Settings.system_name.device);

        #endif // USE_MODULE_NETWORK_MQTT
      #endif // ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      
      DEBUG_LINE_HERE;
    }
    break;
  }


} // END function


#ifdef ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025


void mWiFi::initConnection()
{
  DEBUG_PRINTLN(F("initConnection() called."));

  #ifdef WLED_ENABLE_WEBSOCKETS
  ws.onEvent(wsEvent);
  #endif

#ifndef WLED_DISABLE_ESPNOW
  if (statusESPNow == ESP_NOW_STATE_ON) {
    DEBUG_PRINTLN(F("ESP-NOW stopping."));
    quickEspNow.stop();
    statusESPNow = ESP_NOW_STATE_UNINIT;
  }
#endif

  WiFi.disconnect(true); // close old connections
#ifdef ESP8266
  WiFi.setPhyMode(force802_3g ? WIFI_PHY_MODE_11G : WIFI_PHY_MODE_11N);
#endif

  if (multiWiFi[selectedWiFi].staticIP != 0U && multiWiFi[selectedWiFi].staticGW != 0U) {
    WiFi.config(multiWiFi[selectedWiFi].staticIP, multiWiFi[selectedWiFi].staticGW, multiWiFi[selectedWiFi].staticSN, dnsAddress);
  } else {
    WiFi.config(IPAddress((uint32_t)0), IPAddress((uint32_t)0), IPAddress((uint32_t)0));
  }

  lastReconnectAttempt = millis();

  if (!WLED_WIFI_CONFIGURED) {
    DEBUG_PRINTLN(F("No connection configured."));
    if (!apActive) initAP();        // instantly go to ap mode
  } else if (!apActive) {
    if (apBehavior == AP_BEHAVIOR_ALWAYS) {
      DEBUG_PRINTLN(F("Access point ALWAYS enabled."));
      initAP();
    } else {
      DEBUG_PRINTLN(F("Access point disabled (init)."));
      WiFi.softAPdisconnect(true);
      WiFi.mode(WIFI_STA);
    }
  }

  if (WLED_WIFI_CONFIGURED) {
    showWelcomePage = false;
    
    DEBUG_PRINT(F("Connecting to "));
    DEBUG_PRINT(multiWiFi[selectedWiFi].clientSSID);
    DEBUG_PRINTLN(F("..."));

    // convert the "serverDescription" into a valid DNS hostname (alphanumeric)
    char hostname[25];
    prepareHostname(hostname);
    WiFi.begin(multiWiFi[selectedWiFi].clientSSID, multiWiFi[selectedWiFi].clientPass); // no harm if called multiple times

#ifdef ARDUINO_ARCH_ESP32
    WiFi.setTxPower(wifi_power_t(txPower));
    WiFi.setSleep(!noWifiSleep);
    WiFi.setHostname(hostname);
#else
    wifi_set_sleep_type((noWifiSleep) ? NONE_SLEEP_T : MODEM_SLEEP_T);
    WiFi.hostname(hostname);
#endif
  }

#ifndef WLED_DISABLE_ESPNOW
  if (enableESPNow) {
    quickEspNow.onDataSent(espNowSentCB);     // see udp.cpp
    quickEspNow.onDataRcvd(espNowReceiveCB);  // see udp.cpp
    bool espNowOK;
    if (apActive) {
      DEBUG_PRINTLN(F("ESP-NOW initing in AP mode."));
      #ifdef ESP32
      quickEspNow.setWiFiBandwidth(WIFI_IF_AP, WIFI_BW_HT20); // Only needed for ESP32 in case you need coexistence with ESP8266 in the same network
      #endif //ESP32
      espNowOK = quickEspNow.begin(apChannel, WIFI_IF_AP);  // Same channel must be used for both AP and ESP-NOW
    } else {
      DEBUG_PRINTLN(F("ESP-NOW initing in STA mode."));
      espNowOK = quickEspNow.begin(); // Use no parameters to start ESP-NOW on same channel as WiFi, in STA mode
    }
    statusESPNow = espNowOK ? ESP_NOW_STATE_ON : ESP_NOW_STATE_ERROR;
  }
#endif
}

#endif // ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025



void mWiFi::parse_JSONCommand(JsonParserObject obj){};
   



void mWiFi::init(void){
  
//  AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::init");

}


// checked 
void mWiFi::WifiConfig(uint8_t type)
{
    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR(D_LOG_DEBUG "mWiFi::WifiConfig=%s"),GetWiFiConfigTypeCtr());
    #endif// ENABLE_LOG_LEVEL_INFO

  if (!connection.config_type) {
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR("!connection.config_type"));
    #endif// ENABLE_LOG_LEVEL_INFO

    if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_INF(PSTR("return"));
    #endif// ENABLE_LOG_LEVEL_INFO
      return; 
    }
    
    WiFi.disconnect();                       // Solve possible Wifi hangs
    connection.config_type = type;

    #ifndef USE_MODULE_NETWORK_WEBSERVER
    if (WIFI_MANAGER == connection.config_type) { 
      connection.config_type = WIFI_SERIAL; 
    }
    #endif  // USE_MODULE_NETWORK_WEBSERVER
//  DEBUG_LINE_HERE;
    connection.config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    connection.counter = connection.config_counter +5;
    // blinks = 1999;
    if (WIFI_RESTART == connection.config_type) {// connection.reconnects_counter++;
    
//  DEBUG_LINE_HERE;
//  #ifndef ESP8266
//       pCONT_set->restart_flag = 2; //restarts device, make this a user flag
//       #endif
    }
    else if (WIFI_SERIAL == connection.config_type) {
    #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_INF(S_LOG_WIFI, PSTR(D_WCFG_6_SERIAL " " D_ACTIVE_FOR_3_MINUTES));
    #endif// ENABLE_LOG_LEVEL_INFO
    }
    // #ifdef USE_MODULE_NETWORK_WEBSERVER
    // else if (WIFI_MANAGER == connection.config_type || WIFI_MANAGER_RESET_ONLY == connection.config_type) {
    // #ifdef ENABLE_LOG_LEVEL_INFO
    //  ALOG_INF(S_LOG_WIFI, PSTR(D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES));
    // #endif// ENABLE_LOG_LEVEL_INFO
    //  pCONT_web->WifiManagerBegin(WIFI_MANAGER_RESET_ONLY == connection.config_type);
    // }
    // #endif  // USE_MODULE_NETWORK_WEBSERVER
  }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR("else connection.config_type"));
    #endif// ENABLE_LOG_LEVEL_INFO
  }

// DEBUG_LINE_HERE;


}



void mWiFi::WifiConnectAP(uint8_t ap_index){


}


void mWiFi::WifiBegin(uint8_t flag, uint8_t channel)
{


  ALOG_INF(PSTR("mWiFi::WifiBegin %d:%d"), flag, channel);

  pCONT->Tasker_Interface(TASK_WIFI_STARTING_CONNECTION);

  const char kWifiPhyMode[] = " BGN";

// #ifdef ARDUINO_ESP8266_RELEASE_2_3_0  // (!strncmp_P(ESP.getSdkVersion(),PSTR("1.5.3"),5))
//   ALOG_DBG(S_LOG_WIFI, PSTR(D_PATCH_ISSUE_2186));
//   WiFi.mode(WIFI_OFF);      // See https://github.com/esp8266/Arduino/issues/2186
// #endif

  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
  WiFi.mode(WIFI_STA);      // Disable AP mode
  WiFiSetSleepMode();
//   #ifdef ESP8266
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) { WiFi.setPhyMode(WIFI_PHY_MODE_11N); }  // B/G/N
//  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11G) { WiFi.setPhyMode(WIFI_PHY_MODE_11G); }  // B/G
//  #endif
  if (!WiFi.getAutoConnect()) { WiFi.setAutoConnect(true); }
  // WiFi.setAutoReconnect(true);

//  SetSSIDofAPwithIndex();
  switch (flag) {
    default:
    case 0:  // AP1
    case 1:  // AP2
      pCONT_set->Settings.sta_active = flag;
    break;
    case 2:  // Toggle
      pCONT_set->Settings.sta_active ^= 1;
    break;
  }        // 3: Current AP

  if (!strlen(pCONT_set->SettingsText(SET_STASSID1 + pCONT_set->Settings.sta_active))) {
    pCONT_set->Settings.sta_active ^= 1;  // Skip empty SSID
  }





  // Set static IP  // Set static IP
  // if (pCONT_set->Settings.ip_address[0]) {
  //   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI "Settings.ip_address=%s"),"true");
  //   WiFi.config(pCONT_set->Settings.ip_address[0], 
  //               pCONT_set->Settings.ip_address[1], 
  //               pCONT_set->Settings.ip_address[2], 
  //               pCONT_set->Settings.ip_address[3]
  //               );  // Set static IP
  // }else{
  //   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI "Settings.ip_address=%s"),"false");
  // }
  
  #ifdef ESP8266
    WiFi.hostname(pCONT_set->runtime.my_hostname);   // ESP8266 needs this here (after WiFi.mode)
  #endif
  
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_ssid[%d]=%s"), pCONT_set->Settings.sta_active, pCONT_set->SettingsText(SET_STASSID1 + pCONT_set->Settings.sta_active));
    AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_pwd[%d]=%s"), pCONT_set->Settings.sta_active, pCONT_set->SettingsText(SET_STAPWD1 + pCONT_set->Settings.sta_active));
    #endif// ENABLE_LOG_LEVEL_INFO

  if (channel) {
    WiFi.begin(
      pCONT_set->SettingsText(SET_STASSID1 + pCONT_set->Settings.sta_active), 
      pCONT_set->SettingsText(SET_STAPWD1 + pCONT_set->Settings.sta_active), 
      channel, 
      connection.bssid
    );


    // Add connected BSSID and channel for multi-AP installations
    // char hex_char[18];
    // snprintf_P(stemp, sizeof(stemp), PSTR(" Channel %d BSSId %s"), channel, ToHex_P((unsigned char*)Wifi.bssid, 6, hex_char, sizeof(hex_char), ':'));
  
  } else {
    
    WiFi.begin(pCONT_set->SettingsText(SET_STASSID1 + pCONT_set->Settings.sta_active), pCONT_set->SettingsText(SET_STAPWD1 + pCONT_set->Settings.sta_active));
  }
  
  #ifdef ESP8266
    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d \"%s\" \"%s\" " D_IN_MODE " 11%c " D_AS " %s"),
    pCONT_set->Settings.sta_active +1, 
    pCONT_set->SettingsText(SET_STASSID1 + pCONT_set->Settings.sta_active), 
    pCONT_set->SettingsText(SET_STAPWD1 + pCONT_set->Settings.sta_active), 
    kWifiPhyMode[WiFi.getPhyMode() & 0x3], 
    pCONT_set->runtime.my_hostname);
    #endif// ENABLE_LOG_LEVEL_INFO
  #endif

  /*
  char stemp[40] = { 0 };
  if (channel) {
    WiFi.begin(SettingsText(SET_STASSID1 + Settings.sta_active), SettingsText(SET_STAPWD1 + Settings.sta_active), channel, Wifi.bssid);
    // Add connected BSSID and channel for multi-AP installations
    char hex_char[18];
    snprintf_P(stemp, sizeof(stemp), PSTR(" Channel %d BSSId %s"), channel, ToHex_P((unsigned char*)Wifi.bssid, 6, hex_char, sizeof(hex_char), ':'));
  } else {
    WiFi.begin(SettingsText(SET_STASSID1 + Settings.sta_active), SettingsText(SET_STAPWD1 + Settings.sta_active));
  }
  AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d %s%s " D_IN_MODE " 11%c " D_AS " %s..."),
    Settings.sta_active +1, SettingsText(SET_STASSID1 + Settings.sta_active), stemp, kWifiPhyMode[WiFi.getPhyMode() & 0x3], my_hostname);

#if LWIP_IPV6
  for (bool configured = false; !configured;) {
    uint16_t cfgcnt = 0;
    for (auto addr : addrList) {
      if ((configured = !addr.isLocal() && addr.isV6()) || cfgcnt==30) {
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Got IPv6 global address %s"), addr.toString().c_str());
        break;  // IPv6 is mandatory but stop after 15 seconds
      }
      delay(500);  // Loop until real IPv6 address is aquired or too many tries failed
      cfgcnt++;
    }
  }
#endif  // LWIP_IPV6=1
*/
}


//chcked
void mWiFi::ScanBestAndBeginWifi()
{
  #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_TST(PSTR(D_LOG_WIFI "ScanBestAndBeginWifi"));
  #endif// ENABLE_LOG_LEVEL_INFO

  static int8_t best_network_db;

  // Not active
  if (0 == connection.scan_state) { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR(D_LOG_WIFI "(0 == connection.scan_state)"));
    #endif// ENABLE_LOG_LEVEL_INFO
    return;
  }
  // Init scan when not connected
  if (1 == connection.scan_state) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR(D_LOG_WIFI "(1 == connection.scan_state) Init scan when not connected"));
    #endif// ENABLE_LOG_LEVEL_INFO
    memset((void*) &connection.bssid, 0, sizeof(connection.bssid));
    best_network_db = -127;
    connection.scan_state = 3;
  }
  // Init scan when connected
  if (2 == connection.scan_state) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR(D_LOG_WIFI "(2 == connection.scan_state) Init scan when connected"));
    #endif// ENABLE_LOG_LEVEL_INFO
    uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
    memcpy((void*) &connection.bssid, (void*) bssid, sizeof(connection.bssid));
    best_network_db = WiFi.RSSI();                  // Get current rssi and add threshold
    if (best_network_db < -WIFI_RSSI_THRESHOLD) { best_network_db += WIFI_RSSI_THRESHOLD; }
    connection.scan_state = 3;
  }

  
if(WiFi.scanComplete() == WIFI_SCAN_RUNNING){  
    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR("WiFi.scanComplete() RUNNING"));
  ALOG_INF(PSTR("WiFi.IP() %s"),WiFi.localIP().toString().c_str());
    #endif //  ENABLE_LOG_LEVEL_INFO
}


  // Init scan
  if (3 == connection.scan_state) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR(D_LOG_WIFI "(3 == connection.scan_state) Init scan"));
    #endif// ENABLE_LOG_LEVEL_INFO
    if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      connection.scan_state++;
    #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_INF(PSTR("Network (re)scan started..."));
    #endif// ENABLE_LOG_LEVEL_INFO
      return;
    }
    // else{
    //   ALOG_INF(PSTR("WiFi.scanComplete() %d"),WiFi.scanComplete());
    // }
  }


  int8_t scan_result = WiFi.scanComplete();
    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(PSTR(D_LOG_WIFI "scan_result=%d"),scan_result);
    #endif //  ENABLE_LOG_LEVEL_INFO

  // Check scan done
  if (4 == connection.scan_state) {
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR(D_LOG_WIFI "(4 == scan_state) // Check scan done"));
    #endif// ENABLE_LOG_LEVEL_INFO
    if (scan_result != WIFI_SCAN_RUNNING) {
      connection.scan_state++;
    }
  }
  // Scan done
  if (5 == connection.scan_state) {
    // #ifdef ENABLE_LOG_LEVEL_INFO
    // ALOG_INF(PSTR(D_LOG_WIFI "(5 == scan_state) Scan done"));
    // #endif// ENABLE_LOG_LEVEL_INFO
    int32_t channel = 0;                            // No scan result
    int8_t ap = 3;                                  // AP default if not found
    uint8_t last_bssid[6];                          // Save last bssid
    memcpy((void*) &last_bssid, (void*) &connection.bssid, sizeof(last_bssid));

    if (scan_result > 0) {
      // Networks found
      for (int8_t i = 0; i < scan_result; ++i) {

        String ssid_scan;
        int32_t rssi_scan;
        uint8_t sec_scan;
        uint8_t* bssid_scan;
        int32_t chan_scan;
        bool hidden_scan;
        
        // WiFiHelper::getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);
        #ifdef ESP8266
          WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);
        #else
          WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan);
        #endif
        
        bool known = false;
        uint8_t j;
        // DEBUG_LINE_HERE;
        for (j = 0; j < MAX_SSIDS; j++) {
          // DEBUG_LINE_HERE;

          // if (ssid_scan == pCONT_set->Settings.sta_ssid[j]) {  // SSID match
          if (ssid_scan == pCONT_set->SettingsText(SET_STASSID1 + j)) {  // SSID match



          // DEBUG_LINE_HERE;
          // #ifdef 
          //   ALOG_TST(PSTR("%s"),ssid_scan);

            known = true;
            if (rssi_scan > best_network_db) {      // Best network
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_TST(PSTR("rssi_scan > best_network_db"));
    #endif// ENABLE_LOG_LEVEL_INFO
               #ifdef ESP8266
                if (sec_scan == ENC_TYPE_NONE || pCONT_set->SettingsText(SET_STAPWD1 + j)) {  // Check for passphrase if not open wlan
               #else
                if (pCONT_set->SettingsText(SET_STAPWD1 + j)) {  // Check for passphrase if not open wlan               
               #endif
                best_network_db = (int8_t)rssi_scan;
                channel = chan_scan;
                ap = j;                             // AP1 or AP2
                memcpy((void*) &connection.bssid, (void*) bssid_scan, sizeof(connection.bssid));

                
            AddLog_Array(LOG_LEVEL_DEV_TEST, "break", connection.bssid, (uint8_t)6);

            
              }
            }
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_TST(PSTR("break"));
    #endif// ENABLE_LOG_LEVEL_INFO
            break;
          }else{
          
            // ALOG_TST(PSTR("ssid_scan[\"%s\"] != pCONT_set->Settings.sta_ssid[%d] \"%s\""),ssid_scan,j,pCONT_set->Settings.sta_ssid[j]);

          }
        }
        // ALOG_INF(
        //   PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %02X:%02X:%02X:%02X:%02X:%02X, RSSI %d, Encryption %d"),
        //   i, (known) ? (j) ? '2' : '1' : '-', ssid_scan.c_str(), 
        //   chan_scan, bssid_scan[0], bssid_scan[1], bssid_scan[2], bssid_scan[3], bssid_scan[4], bssid_scan[5], 
        //   rssi_scan, 
          
        //   #ifdef ESP8266
        //     (sec_scan == ENC_TYPE_NONE) ? 0 : 1); // Check for passphrase if not open wlan
        //   #else
        //     (sec_scan == 0) ? 0 : 1);  // Check for passphrase if not open wlan               
        //   #endif   

        if(known){
          char known_c = 
                    (j==0?'1':
                    (j==1?'2':
                    (j==2?'3':
                    '-')));
          
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_INF(
                        PSTR(DEBUG_INSERT_PAGE_BREAK D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, RSSI %d"), 
                        i, known_c, ssid_scan.c_str(), chan_scan, rssi_scan
                      );
    #endif // ENABLE_LOG_LEVEL_INFO
        }
          
        delay(0);
      }
      WiFi.scanDelete();                            // Clean up Ram
      delay(0);
    }
    // DEBUG_LINE_HERE;
    connection.scan_state = 0;
    // If bssid changed then (re)connect wifi
    for (uint8_t i = 0; i < sizeof(connection.bssid); i++) {
    
      if (last_bssid[i] != connection.bssid[i]) {
    #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_INF(PSTR(D_LOG_WIFI "last_bssid[i] != connection.bssid[i]"));
    #endif// ENABLE_LOG_LEVEL_INFO
        WifiBegin(ap, channel);                     // 0 (AP1), 1 (AP2) or 3 (default AP)
        break;
      }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_INF(PSTR(D_LOG_WIFI "last_bssid[i] ================= connection.bssid[i]"));
    #endif// ENABLE_LOG_LEVEL_INFO

      }
    }
  }
}






bool mWiFi::WifiHostByName(const char* aHostname, IPAddress& aResult) {
#ifdef USE_IPV6
#if ESP_IDF_VERSION_MAJOR >= 5
  // try converting directly to IP
  if (aResult.fromString(aHostname)) {
    return true;   // we're done
  }
#endif
#endif // USE_IPV6

  pCONT_set->Settings.dns_timeout = 1000;

  uint32_t dns_start = millis();
  bool success = WiFi.hostByName(aHostname, aResult);//, pCONT_set->Settings.dns_timeout);
  uint32_t dns_end = millis();
  if (success) {
    // Host name resolved
    if (0xFFFFFFFF != (uint32_t)aResult) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI "DNS resolved '%s' (%s) in %i ms"), aHostname, aResult.toString().c_str(), dns_end - dns_start);
      return true;
    }
  }
  ALOG_DBG(PSTR(D_LOG_WIFI "DNS failed for %s after %i ms"), aHostname, dns_end - dns_start);
  return false;
}



uint16_t mWiFi::WifiLinkCount()
{
  return connection.link_count;
}

// String mWiFi::WifiDowntime()
// {
//    //return GetDuration(connection.downtime);
// }

// checked
void mWiFi::WifiSetState(uint8_t state)
{

  //check for change in state
  if(connection.fConnected != state)
  {
    DEBUG_LINE_HERE;

    if(state){ //new state 
    // pinMode(2,OUTPUT);
    // digitalWrite(2,LOW);
      pCONT->Tasker_Interface(TASK_WIFI_CONNECTED);
      loglevel_with_connection_status = LOG_LEVEL_DEBUG_MORE;
    }else{
    // pinMode(2,OUTPUT);
    // digitalWrite(2,HIGH);
      pCONT->Tasker_Interface(TASK_WIFI_DISCONNECTED);
      loglevel_with_connection_status = LOG_LEVEL_INFO;
    }
  }

  connection.fConnected = state;

  
  if(state == 0){
    DEBUG_LINE_HERE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_INF(PSTR(D_LOG_DEBUG "%s=%d"),"WifiSetState",state);
    #endif// ENABLE_LOG_LEVEL_INFO
  }

  if (state == pCONT_set->runtime.global_state.wifi_down) {
    DEBUG_LINE_HERE;
    if (state) {
      // pCONT_set->rules_flag.wifi_connected = 1;
      connection.link_count++;
      connection.downtime += pCONT_time->UpTime() - connection.last_event;
    } else {
      // pCONT_set->rules_flag.wifi_disconnected = 1;
      connection.last_event = pCONT_time->UpTime();
    }DEBUG_LINE_HERE

  }
  // if(pCONT_time==NULL){
  //    ALOG_DBM( "pCONT_time==NULL");
  // }
  pCONT_set->runtime.global_state.wifi_down = state ^1;
  if (!pCONT_set->runtime.global_state.wifi_down) {
    // DEBUG_LINE_HERE;
    pCONT_set->runtime.global_state.network_down = 0;
  }

  // ALOG_INF(PSTR(D_LOG_DEBUG "%s"),"WifiSetState end");
    
}


#ifdef ESP8266
#if LWIP_IPV6
bool WifiCheckIPv6(void)
{
  bool ipv6_global=false;

  for (auto a : addrList) {
    if(!a.isLocal() && a.isV6()) ipv6_global=true;
  }
  return ipv6_global;
}

String WifiGetIPv6(void)
{
  for (auto a : addrList) {
    if(!a.isLocal() && a.isV6()) return a.toString();
  }
  return "";
}

bool WifiCheckIPAddrStatus(void)	// Return false for 169.254.x.x or fe80::/64
{
  bool ip_global=false;

  for (auto a : addrList) {
    if(!a.isLocal()) ip_global=true;
  }
  return ip_global;
}
#endif  // LWIP_IPV6=1
#endif // ESP8266


bool mWiFi::WifiCheckIpConnected()
{

  if ((WL_CONNECTED == WiFi.status()) && 
      (static_cast<uint32_t>(WiFi.localIP()) != 0) &&
      (WiFi.localIP().toString()!="(IP unset)")
  ) 
  {
    return true;  
  }
  
  DEBUG_LINE_HERE;
  Serial.println(WiFi.status());

  return false;

}

/**
 * @brief Causing issues with neopixel timing
 * 
 */
void mWiFi::WifiCheckIp(void)
{

  AddLog(loglevel_with_connection_status, PSTR(D_LOG_DEBUG "mWiFi::WifiCheckIp"));

// #if defined(LWIP_IPV6) && defined(ESP8266)
//   if(WifiCheckIPAddrStatus()) {
//     Wifi.status = WL_CONNECTED;
// #else

// bool test = (WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()));


  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) 
  {



// #endif  // LWIP_IPV6=1

    AddLog(loglevel_with_connection_status,  PSTR("WL_CONNECTED %s"), WiFi.localIP().toString().c_str() );

    WifiSetState(1);
    connection.counter = WIFI_CHECK_SEC; //20 secs
    connection.retry = connection.retry_init;

    // AddLog(
    //   (connection.status != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE,  //if it was not previously connected
    //   PSTR(D_LOG_WIFI D_CONNECTED));

    if (connection.status != WL_CONNECTED) {                                        //if it was not previously connected, get router info
      
    #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_INF(S_LOG_WIFI, PSTR(D_CONNECTED));
    #endif// ENABLE_LOG_LEVEL_INFO
      // ALOG_INF(PSTR(D_LOG_WIFI "Set IP addresses"));
      pCONT_set->Settings.ip_address[1] = (uint32_t)WiFi.gatewayIP();
      pCONT_set->Settings.ip_address[2] = (uint32_t)WiFi.subnetMask();
      pCONT_set->Settings.ip_address[3] = (uint32_t)WiFi.dnsIP();
      
      // Save current AP parameters for quick reconnect
      pCONT_set->Settings.wifi_channel = WiFi.channel();
      uint8_t *bssid = WiFi.BSSID();
      memcpy((void*) &pCONT_set->Settings.wifi_bssid, (void*) bssid, sizeof(pCONT_set->Settings.wifi_bssid));
    }

    connection.status = WL_CONNECTED;    //assert status to be connected
    

  } 
  else
  { //not connected
    
    #ifdef ENABLE_LOG_LEVEL_INFO
   // ALOG_INF(PSTR("%s" " NOT connected"),__FUNCTION__);//WiFi.status() %s, IP \"%s\" %s"),GetWiFiStatusCtr(),WiFi.localIP().toString().c_str(),WiFi.localIP().toString()=="(IP unset)"?"matched":"nomatch");
    #endif// ENABLE_LOG_LEVEL_INFO

    WifiSetState(0);DEBUG_LINE_HERE
    uint8_t wifi_config_tool = pCONT_set->Settings.sta_config;
    connection.status = WiFi.status();
    switch (connection.status) {
      case WL_CONNECTED:
    #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_NO_IP_ADDRESS));
    #endif // ENABLE_LOG_LEVEL_INFO
        connection.status = 0;
        connection.retry = connection.retry_init;        
        connection.fConnected = true;
        break;
      case WL_NO_SSID_AVAIL:
        #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_NOT_REACHED));
        #endif // ENABLE_LOG_LEVEL_INFO
        
        pCONT_set->Settings.wifi_channel = 0;  // Disable stored AP

        if (WIFI_WAIT == pCONT_set->Settings.sta_config) {
    #ifdef ENABLE_LOG_LEVEL_INFO
          ALOG_INF(PSTR(D_LOG_WIFI "1%s"),"WIFI_WAIT == pCONT_set->Settings.sta_config");
    #endif// ENABLE_LOG_LEVEL_INFO
          connection.retry = connection.retry_init;
        } else {
          if (connection.retry > (connection.retry_init / 2)) {
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_INF(PSTR(D_LOG_WIFI "2%s"),"ELSE WIFI_WAIT == pCONT_set->Settings.sta_config retry>");
    #endif// ENABLE_LOG_LEVEL_INFO
            connection.retry = connection.retry_init / 2;
          }
          else if (connection.retry) {
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_INF(PSTR(D_LOG_WIFI "3%s"),"ELSE WIFI_WAIT == pCONT_set->Settings.sta_config retry else");
    #endif// ENABLE_LOG_LEVEL_INFO
            connection.retry = 0;
          }else{
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_INF(PSTR(D_LOG_WIFI "4%s %d %s %d"),"retry else",connection.retry,"connection.retry_init",connection.retry_init);
    #endif // ENABLE_LOG_LEVEL_INFO
          }
        }
        connection.fConnected = false;
        connection.fReconnect = true;
        break;
      case WL_CONNECT_FAILED:
    #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_WRONG_PASSWORD));
    #endif// ENABLE_LOG_LEVEL_INFO
        pCONT_set->Settings.wifi_channel = 0;  // Disable stored AP
        if (connection.retry > (connection.retry_init / 2)) {
          connection.retry = connection.retry_init / 2;
        }
        else if (connection.retry) {
          connection.retry = 0;
        }
        connection.fConnected = false;
        connection.fReconnect = true;
        break;
      // case WL_IDLE_STATUS: ALOG_INF(PSTR(D_LOG_WIFI "case WL_IDLE_STATUS"));
      // case WL_DISCONNECTED: ALOG_DBM( PSTR(D_LOG_WIFI "case WL_DISCONNECTED"));
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        //ALOG_INF(PSTR(D_LOG_WIFI "default"));

        if (!connection.retry || ((connection.retry_init / 2) == connection.retry)) {
          #ifdef ENABLE_LOG_LEVEL_INFO
          ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_TIMEOUT));
          #endif// ENABLE_LOG_LEVEL_INFO
          pCONT_set->Settings.wifi_channel = 0;  // Disable stored AP
        } else {
          if (('\0' == pCONT_set->SettingsText(SET_STASSID1)) && ('\0' == pCONT_set->SettingsText(SET_STASSID2))) {
          pCONT_set->Settings.wifi_channel = 0;  // Disable stored AP
            wifi_config_tool = WIFI_CONFIG_NO_SSID; // SHOULD BE WIFI_MANAGER   // Skip empty SSIDs and start Wifi config tool
            connection.retry = 0;
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "WIFI_CONFIG_NO_SSID"));
    #endif// ENABLE_LOG_LEVEL_INFO
          } 
          else {
    #ifdef ENABLE_LOG_LEVEL_INFO
            ALOG_DBM( PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "1"));
    #endif// ENABLE_LOG_LEVEL_INFO
          }
        }

        connection.fConnected = false;
        connection.fReconnect = true;

    } // END of switch


    if (connection.retry) 
    {
      
      ALOG_INF(PSTR(D_LOG_WIFI "connection retry %d"), connection.retry_init - connection.retry);

      if (pCONT_set->Settings.flag_network.use_wifi_scan) 
      {
        if (connection.retry_init == connection.retry) 
        {
          connection.scan_state = 1;    // Select scanned SSID
          ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select scanned SSID"));          
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_WIFI "connection.retry_init == connection.retry %d=%d"), connection.retry_init, connection.retry);
        }
      } else {
        if (connection.retry_init == connection.retry) 
        {
          
          WifiBegin(WIFIBEGIN_FLAG_TOGGLE_SSIDS_ID, pCONT_set->Settings.wifi_channel);        // Select alternate SSID
          ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select default SSID"));
          
        }
        if ((pCONT_set->Settings.sta_config != WIFI_WAIT) && ((connection.retry_init / 2) == connection.retry)) 
        {

          WifiBegin(WIFIBEGIN_FLAG_TOGGLE_SSIDS_ID, 0);        // Select alternate SSID
          ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select alternate SSID"));
          
        }
      }

      /*
      if (Settings.flag3.use_connection.scan) {  // SetOption56 - Scan wifi network at restart for configured AP's
        if (Wifi.retry_init == Wifi.retry) {
          Wifi.scan_state = 1;    // Select scanned SSID
        }
      } else {
        if (Wifi.retry_init == Wifi.retry) {
          WifiBegin(3, Settings.connection.channel);  // Select default SSID
        }
        if ((Settings.sta_config != WIFI_WAIT) && ((Wifi.retry_init / 2) == Wifi.retry)) {
          WifiBegin(2, 0);        // Select alternate SSID
        }
      }*/
      connection.counter = 1;
      connection.retry--;
    } 
    else 
    {
      
    #ifdef ENABLE_LOG_LEVEL_INFO
      ALOG_INF(PSTR(D_LOG_WIFI "wifi_retry=FALSE"));
    #endif// ENABLE_LOG_LEVEL_INFO
      WifiConfig(wifi_config_tool);
      connection.counter = 1;
      connection.retry = connection.retry_init;
    }
  }

  //ALOG_INF(PSTR(D_LOG_DEBUG "%s"),"WifiSetState checkip END");


}


// Primary health checker
void mWiFi::WifiCheck(uint8_t param)
{

  AddLog(loglevel_with_connection_status, PSTR("Delay below"));

  
  #ifdef ENABLE_WIFI_DEVELOPMENT
  ALOG_DBG(PSTR("F::%s"),__FUNCTION__);
  #endif

  // ALOG_TST(PSTR("connection.config_counter=%d"),connection.config_counter);


  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI D_COMMAND_NVALUE ", " D_COMMAND_NVALUE),"connection.counter",connection.counter,"param",param);
  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI D_COMMAND_NVALUE ", " D_COMMAND_NVALUE),"config_counter",connection.config_counter,"counter",connection.counter);

  // if ((WL_CONNECTED != WiFi.status()) || (static_cast<uint32_t>(WiFi.localIP()) == 0)) {
  //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s=%d,%s=%d,%s=%d"),"connection.+_counter",connection.config_counter,"connection.counter",connection.counter,"connection.+_state",connection.scan_state);
  // }

  connection.counter--;

  switch (param) {
    case WIFI_SERIAL:
    case WIFI_MANAGER:
      WifiConfig(param);
      break;
    // case WIFI_RESTART:
    default: 
      /**
       * config_counter updates config
       * */
      if (connection.config_counter) 
      {

        ALOG_INF( PSTR(D_LOG_WIFI "WifiCheck " "config_counter=%d"), connection.config_counter);
    
        connection.config_counter--;
        connection.counter = connection.config_counter +5;
    //     if (connection.config_counter) {
          
    //       // If we reach ZERO, then retry connection on default
    //       if (!connection.config_counter) {
    //         if (strlen(WiFi.SSID().c_str())) {
    //           strlcpy(pCONT_set->Settings.sta_ssid[0], WiFi.SSID().c_str(), sizeof(pCONT_set->Settings.sta_ssid[0]));
    //         }
    //         if (strlen(WiFi.psk().c_str())) {
    //           strlcpy(pCONT_set->Settings.sta_pwd[0], WiFi.psk().c_str(), sizeof(pCONT_set->Settings.sta_pwd[0]));
    //         }
    //         pCONT_set->Settings.sta_active = 0;
    // #ifdef ENABLE_LOG_LEVEL_INFO
    //         ALOG_INF(PSTR(D_LOG_WIFI D_SSID "Retrying original config \"%s\""), pCONT_set->Settings.sta_ssid[0]);
    // #endif// ENABLE_LOG_LEVEL_INFO
    //       }

    //     }
        // Delayed by the above code by 5 seconds
        if (!connection.config_counter) 
        {
          // pCONT_set->restart_flag = 2;
    #ifdef ENABLE_LOG_LEVEL_INFO
          ALOG_INF(PSTR(D_LOG_WIFI "WifiCheck " "restart_flag = 2"));
    #endif// ENABLE_LOG_LEVEL_INFO
        }

      } else {

        // DEBUG_LINE_HERE;

        if (connection.scan_state) { 
        // DEBUG_LINE_HERE;
          #ifdef ENABLE_LOG_LEVEL_INFO
          AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_WIFI D_COMMAND_NVALUE),"scan_state",connection.scan_state);
          #endif// ENABLE_LOG_LEVEL_INFO
          ScanBestAndBeginWifi(); 
        }

        if (connection.counter <= 0) {
    #ifdef ENABLE_LOG_LEVEL_INFO
          // ALOG_INF(PSTR(D_LOG_WIFI D_CHECKING_CONNECTION));
    #endif// ENABLE_LOG_LEVEL_INFO
          connection.counter = WIFI_CHECK_SEC;
// return ;
        // DEBUG_LINE_HERE;
          WifiCheckIp();
        }

// #ifdef ENABLE_FORCED_SKIP_AP_ON_IPUNSET
//       //skip to next AP
//       if(strcmp(WiFi.localIP().toString().c_str(),"(IP unset)")==0){
//         AddLog(LOG_LEVEL_WARN, PSTR("Forcing new AP %s"),pCONT_set->Settings.sta_active+1);
//   //cant toggle, needs to shift between 3
//   if ('\0' == pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active][0]) { 
//     if(pCONT_set->Settings.sta_active++>2){ pCONT_set->Settings.sta_active = WIFIBEGIN_FLAG_SSID0_ID; } 
//   }  // Skip empty SSID
//       }
// #endif //ENABLE_FORCED_SKIP_AP_ON_IPUNSET


      // Still connected
      // if (WifiCheckIpConnected() && !connection.config_type){
          
      //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"Connected, Restarted, Begin services");

      //   WifiSetState(1);
        
      //   pCONT->Tasker_Interface(TASK_WIFI_CONNECTED);

      //   //if (pCONT_set->Settings.flag_network.use_wifi_rescan) {
      //     if (!(pCONT_time->UpTime() % (60 * WIFI_RESCAN_MINUTES))) {
      //       connection.scan_state = 2;
      //       // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"WIFI_RESCAN_MINUTES occurred connection.scan_state = 2");
      //     }
      //   //}


    // #if LWIP_IPV6
    //       if (WifiCheckIPAddrStatus()) {
    // #else

          // bool is_connected = pCONT_time->UpTime()>20;//(WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !connection.config_type;

          bool is_connected = (WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !connection.config_type;


          if (is_connected) 
          {
        // DEBUG_LINE_HERE;
    //#endif /// LWIP_IPV6=1
    
        // DEBUG_PRINTF( " if ((WL_CONNECTED == WiFi.status())\n\r");

        //resting state, connected and healthy


            WifiSetState(1);
            
            if (pCONT_set->Settings.flag_network.use_wifi_rescan) {  // SetOption57 - Scan wifi network every 44 minutes for configured AP's
              if (!(pCONT_time->UpTime() % (60 * WIFI_RESCAN_MINUTES))) {
                connection.scan_state = 2;
              }
            }
            
            
            // #ifdef USE_MODULE_NETWORK_WEBSERVER
            //   if (pCONT_set->Settings.webserver) {
            //     pCONT_web->StartWebserver(pCONT_set->Settings.webserver, WiFi.localIP());
            //   } else {
            //     pCONT_web->StopWebserver();
            //   }
            // #endif  // USE_MODULE_NETWORK_WEBSERVER


          } 
          else 
          {
            DEBUG_LINE_HERE
        Serial.println( " ELSE if ((WL_CONNECTED == WiFi.status())\n\r");
            WifiSetState(0);
            Mdns.begun = 0;
          }
        }//END else


      // } else {

      //   WifiSetState(0);
    
      //   //pCONT->Tasker_Interface(TASK_WIFI_DISCONNECTED);

      // }
    //} //if discovery
  }
}

int mWiFi::WifiState(void)
{
  int state = -1;
  if (!pCONT_set->runtime.global_state.wifi_down) { state = WIFI_RESTART; }
  if (connection.config_type) { state = connection.config_type; }
  return state;
}




void mWiFi::WifiConnect(void)
{ 
  
  // if (!Settings.flag4.network_wifi) { return; } //probably for when ethernet is used instead

  // ALOG_INF(PSTR(D_LOG_DEBUG "%s"),__FUNCTION__);
  
  WifiSetState(0);
  // WifiSetOutputPower(); //new
  WiFi.persistent(false);     // Solve possible wifi init errors
  connection.status = 0;
  
  connection.retry_init = WIFI_RETRY_OFFSET_SEC + ((mSupportHardware::ESP_getChipId() & 0xF) * 2); //ESP_getChipId function to allow esp8266/esp32
  
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "connection.retry_init=%d"),connection.retry_init);

  connection.retry = connection.retry_init;
  connection.counter = 1;


#ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
// dns server
DNSServer dnsServer;
#endif // ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL







//   memcpy((void*) &Wifi.bssid, (void*) Settings.wifi_bssid, sizeof(Wifi.bssid));
  
}

#ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

void mWiFi::initAP(bool resetAP)
{
  if (apBehavior == AP_BEHAVIOR_BUTTON_ONLY && !resetAP)
    return;

  if (resetAP) {
    WLED_SET_AP_SSID();
    strcpy_P(apPass, PSTR(WLED_AP_PASS));
  }
  DEBUG_PRINT(F("Opening access point "));
  DEBUG_PRINTLN(apSSID);
  WiFi.softAPConfig(IPAddress(4, 3, 2, 1), IPAddress(4, 3, 2, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, apPass, apChannel, apHide);
  #if defined(LOLIN_WIFI_FIX) && (defined(ARDUINO_ARCH_ESP32C3) || defined(ARDUINO_ARCH_ESP32S2))
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  #endif

  if (!apActive) // start captive portal if AP active
  {
    DEBUG_PRINTLN(F("Init AP interfaces"));
    server.begin();
    if (udpPort > 0 && udpPort != ntpLocalPort) {
      udpConnected = notifierUdp.begin(udpPort);
    }
    if (udpRgbPort > 0 && udpRgbPort != ntpLocalPort && udpRgbPort != udpPort) {
      udpRgbConnected = rgbUdp.begin(udpRgbPort);
    }
    if (udpPort2 > 0 && udpPort2 != ntpLocalPort && udpPort2 != udpPort && udpPort2 != udpRgbPort) {
      udp2Connected = notifier2Udp.begin(udpPort2);
    }
    e131.begin(false, e131Port, e131Universe, E131_MAX_UNIVERSE_COUNT);
    ddp.begin(false, DDP_DEFAULT_PORT);

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(53, "*", WiFi.softAPIP());
  }
  apActive = true;
}

#endif // ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL




void mWiFi::WifiShutdown(bool option)
{
  // option = false - Legacy disconnect also used by DeepSleep
  // option = true  - Disconnect with SDK wifi calibrate sector erase when WIFI_FORCE_RF_CAL_ERASE enabled
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b

// #ifdef USE_EMULATION
//   UdpDisconnect();
//   delay(100);                 // Flush anything in the network buffers.
// #endif  // USE_EMULATION

  // if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
  //   MqttDisconnect();
  //   delay(100);               // Flush anything in the network buffers.
  // }

// #ifdef WIFI_FORCE_RF_CAL_ERASE
//   if (option) {
//     WiFi.disconnect(false);   // Disconnect wifi
//     SettingsErase(4);         // Delete SDK wifi config and calibrate data
//   } else
// #endif  // WIFI_FORCE_RF_CAL_ERASE
  // {
    // Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
    // Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
    // Courtesy of EspEasy
    // WiFi.persistent(true);    // use SDK storage of SSID/WPA parameters
    ETS_UART_INTR_DISABLE();

  //tmp fix
#ifdef ESP8266
    wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
    #else

    // erase ap: empty ssid, ...
    WiFi.disconnect(true, true); //inside compat
    #endif
    ETS_UART_INTR_ENABLE();
    // WiFi.persistent(false);   // Do not use SDK storage of SSID/WPA parameters
  // }
  delay(100);                 // Flush anything in the network buffers.

}




void mWiFi::EspRestart(void)
{

#ifdef ENABLE_DEVFEATURE_BLOCK_RESTART

  AddLog(LOG_LEVEL_ERROR, PSTR("ENABLE_DEVFEATURE_BLOCK_RESTART"));

#else

  //ResetPwm();
  WifiShutdown(true);
  // CrashDumpClear();           // Clear the stack dump in RTC

  // if (restart_halt) {
  //   while (1) {
  //     OsWatchLoop();          // Feed OsWatch timer to prevent restart
  //     SetLedLink(1);          // Wifi led on
  //     delay(200);             // Satisfy SDK
  //     SetLedLink(0);          // Wifi led off
  //     delay(800);             // Satisfy SDK
  //   }
  // } else {
    ESP.restart(); //ESP_Restart();
  // }

  //  ResetPwm();
  // WifiShutdown(true);
  // CrashDumpClear();           // Clear the stack dump in RTC

  // if (restart_halt) {
  //   while (1) {
  //     OsWatchLoop();          // Feed OsWatch timer to prevent restart
  //     SetLedLink(1);          // Wifi led on
  //     delay(200);             // Satisfy SDK
  //     SetLedLink(0);          // Wifi led off
  //     delay(800);             // Satisfy SDK
  //   }
  // } else {
  //   ESP_Restart();
  // }
  // delay(100);                 // Allow time for message xfer - disabled v6.1.0b
  // //if (Settings.flag_system.mqtt_enabled) MqttDisconnect();
  // WifiDisconnect();
  // //pCONT_sup->CrashDumpClear();
  // ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  // #ifdef ESP8266
  //   ESP.reset();
  // #endif

#endif // ENABLE_DEVFEATURE_BLOCK_RESTART

}


void mWiFi::WiFiSetSleepMode(void)
{
/* Excerpt from the esp8266 non os sdk api reference (v2.2.1):
 * Sets sleep type for power saving. Set WIFI_NONE_SLEEP to disable power saving.
 * - Default mode: WIFI_MODEM_SLEEP.
 * - In order to lower the power comsumption, ESP8266 changes the TCP timer
 *   tick from 250ms to 3s in WIFI_LIGHT_SLEEP mode, which leads to increased timeout for
 *   TCP timer. Therefore, the WIFI_MODEM_SLEEP or deep-sleep mode should be used
 *   where there is a requirement for the accurancy of the TCP timer.
 *
 * Sleep is disabled in core 2.4.1 and 2.4.2 as there are bugs in their SDKs
 * See https://github.com/arendst/Sonoff-Tasmota/issues/2559
 */

// Sleep explanation: https://github.com/esp8266/Arduino/blob/3f0c601cfe81439ce17e9bd5d28994a7ed144482/libraries/ESP8266WiFi/src/ESP8266WiFiGeneric.cpp#L255
// #if defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
// #else  // Enabled in 2.3.0, 2.4.0 and stage
//   // if (sleep && Settings.flag_network.sleep_normal) {
//   //   WiFi.setSleepMode(WIFI_LIGHT_SLEEP);  // Allow light sleep during idle times
//   // } else {
//   //   WiFi.setSleepMode(WIFI_MODEM_SLEEP);  // Disable sleep (Esp8288/Arduino core and sdk default)
//   // }
// #endif

// Sleep explanation: https://github.com/esp8266/Arduino/blob/3f0c601cfe81439ce17e9bd5d28994a7ed144482/libraries/ESP8266WiFi/src/ESP8266WiFiGeneric.cpp#L255

/* * datasheet:
    *
   wifi_set_sleep_level():
   Set sleep level of modem sleep and light sleep
   This configuration should be called before calling wifi_set_sleep_type
   Modem-sleep and light sleep mode have minimum and maximum sleep levels.
   - In minimum sleep level, station wakes up at every DTIM to receive
     beacon.  Broadcast data will not be lost because it is transmitted after
     DTIM.  However, it can not save much more power if DTIM period is short,
     as specified in AP.
   - In maximum sleep level, station wakes up at every listen interval to
     receive beacon.  Broadcast data may be lost because station may be in sleep
     state at DTIM time.  If listen interval is longer, more power will be saved, but
     it’s very likely to lose more broadcast data.
   - Default setting is minimum sleep level.
   Further reading: https://routerguide.net/dtim-interval-period-best-setting/
   wifi_set_listen_interval():
   Set listen interval of maximum sleep level for modem sleep and light sleep
   It only works when sleep level is set as MAX_SLEEP_T
   It should be called following the order:
     wifi_set_sleep_level(MAX_SLEEP_T)
     wifi_set_listen_interval
     wifi_set_sleep_type
   forum: https://github.com/espressif/ESP8266_NONOS_SDK/issues/165#issuecomment-416121920
   default value seems to be 3 (as recommended by https://routerguide.net/dtim-interval-period-best-setting/)*/
  // if (ssleep && Settings.flag3.sleep_normal) {  // SetOption60 - Enable normal sleep instead of dynamic sleep
  //   WiFi.setSleepMode(WIFI_LIGHT_SLEEP);        // Allow light sleep during idle times
  // } else {
  //   WiFi.setSleepMode(WIFI_MODEM_SLEEP);        // Disable sleep (Esp8288/Arduino core and sdk default)
  // }
  // WifiSetOutputPower();
}




/***************************************************************************************************************************
 **********************************************************************************************************************************
 *  HELPER FUNCTIONS
 **********************************************************************************************************************************
 *******************************************************************************************************************************/




int mWiFi::WifiGetRssiAsQuality(int rssi)
{
  int quality = 0;

  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

bool mWiFi::WifiConfigCounter(void)
{
  if (connection.config_counter) {
    connection.config_counter = WIFI_CONFIG_SEC;
  }
  return connection.config_counter;
}


/*********************************************************************************************\
 * MDNS
\*********************************************************************************************/

#ifdef USE_DISCOVERY

void mWiFi::StartMdns(void) {
  // if (Settings.flag3.mdns_enabled) {  // SetOption55 - Control mDNS service
      DEBUG_LINE_HERE;
  if (!Mdns.begun) {
    // close existing or MDNS.begin will fail
    MDNS.end(); 
      DEBUG_LINE_HERE;
    // Begin with devicename
    Mdns.begun = (uint8_t)MDNS.begin(pCONT_set->Settings.system_name.device);
    
      DEBUG_LINE_HERE;
    ALOG_INF( PSTR(D_LOG_MDNS "%s" " with %s"), (Mdns.begun) ? PSTR(D_INITIALIZED) : PSTR(D_FAILED), pCONT_set->Settings.system_name.device);

    #ifdef ESP32
    // Add service to MDNS-SD
    MDNS.addService("_http", "_tcp", 80);
    #endif
  
  }
      DEBUG_LINE_HERE;
}


#ifdef MQTT_HOST_DISCOVERY
void mWiFi::MqttDiscoverServer(void)
{
  if (!Mdns.begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  ALOG_INF( PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if (n > 0) {
    uint32_t i = 0;            // If the hostname isn't set, use the first record found.
    #ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_HOSTNAME)) {
        break;                 // Stop at matching record
      }
    }
    #endif  // MDNS_HOSTNAME
    // SettingsUpdateText(SET_MQTT_HOST, MDNS.hostname(i).c_str());
    // Settings.mqtt_port = MDNS.port(i);
    // ALOG_INF(PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s," D_PORT " %d"), SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  }
}
#endif  // MQTT_HOST_DISCOVERY

#ifdef WEBSERVER_HOST_DISCOVERY
void mWiFi::MdnsAddServiceHttp(void) {
  if (1 == Mdns.begun) {
    Mdns.begun = 2;
    MDNS.addService("http", "tcp", WEB_PORT);
    MDNS.addServiceTxt("http", "tcp", "devicetype", "tasmota");
  }
}
#endif  // WEBSERVER_HOST_DISCOVERY
#endif  // USE_DISCOVERY



#if defined(USE_NETWORK_MDNS) && defined(ESP8266) //Not needed with esp32 mdns
void mWiFi::MdnsUpdate(void) 
{
  MDNS.update();
  if (2 == Mdns.begun) {
    MDNS.update(); // this is basically passpacket like a webserver
   // ALOG_DBM( PSTR(D_LOG_MDNS "MDNS.update"));
  }
}
#endif  // ESP8266


// In 1dB increments
int8_t mWiFi::GetRSSdBm(){
  return WiFi.RSSI();
}

uint8_t mWiFi::GetRSSPercentage(){
  return constrain(map(GetRSSdBm(),-40,-100,100,0),0,100);
}




const char* mWiFi::GetWiFiStatusCtr(void){                          // UNSTABLE CODE, RETURNING NULL!!

//NULL

  switch(WiFi.status()){
    case WL_IDLE_STATUS      : return PSTR("WL_IDLE_STATUS");
    case WL_NO_SSID_AVAIL    : return PSTR("WL_NO_SSID_AVAIL");
    case WL_SCAN_COMPLETED   : return PSTR("WL_SCAN_COMPLETED");
    case WL_CONNECTED        : return PSTR("WL_CONNECTED");
    case WL_CONNECT_FAILED   : return PSTR("WL_CONNECT_FAILED");
    case WL_CONNECTION_LOST  : return PSTR("WL_CONNECTION_LOST");
    case WL_DISCONNECTED     : return PSTR("WL_DISCONNECTED");
    default: PSTR("Unknown");
  }
}

const char* mWiFi::GetWiFiConfigTypeCtr(void){

  //NULL 

  switch(connection.config_type){
    case WIFI_RESTART      : return PSTR("WIFI_RESTART");
    case WIFI_SMARTCONFIG    : return PSTR("WIFI_SMARTCONFIG");
    case WIFI_MANAGER   : return PSTR("WIFI_MANAGER");
    case WIFI_WPSCONFIG        : return PSTR("WIFI_WPSCONFIG");
    case WIFI_RETRY   : return PSTR("WIFI_RETRY");
    case WIFI_WAIT  : return PSTR("WIFI_WAIT");
    case WIFI_SERIAL     : return PSTR("WIFI_SERIAL");
    case WIFI_MANAGER_RESET_ONLY  : return PSTR("WIFI_MANAGER_RESET_ONLY");
    case MAX_WIFI_OPTION     : return PSTR("MAX_WIFI_OPTION");
    default: PSTR("Unknown");
  }
}


// uint32_t ESP_getChipId(void) {
//   uint32_t id = 0;
//   for (uint32_t i = 0; i < 17; i = i +8) {
//     id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
//   }
//   return id;
// }
// uint32_t ESP_getChipId(void) {
//   return ESP.getChipId();
// }

// void WifiSetOutputPower(void)
// {
//   WiFi.setOutputPower((float)(Settings.wifi_output_power) / 10);
// }



//#endif



#endif // USE_MODULE_NETWORK_WIFI