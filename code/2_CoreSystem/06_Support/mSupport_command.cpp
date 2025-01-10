#include "2_CoreSystem/06_Support/mSupport.h"


void mSupport::parse_JSONCommand(JsonParserObject obj)
{

  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  

  if(jtok = obj["Restart"])
  {    
    ALOG_COM( PSTR("Restart %d"), jtok.getInt() );
    CommandSet_Restart(jtok.getInt());   
  }
  

  if(jtok = obj["Logging"].getObject()["SerialLevel"])
  {
    char buffer[100];
    // const char* name = jtok.getStr();  //["loglevel"];
    ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED "\"loglevel\":\"%s\""), jtok.getStr());
    tkr_set->Settings.logging.serial_level = pCONT_log->GetLogLevelIDbyName(jtok.getStr());
    ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED "\"loglevel\":\"%S\""), pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.serial_level));
    // Add save log here
    data_buffer.isserviced++;
  }
  if(jtok = obj["Logging"].getObject()["MQTTLevel"])
  {
    char buffer[100];
    // const char* name = jtok.getStr();  //["loglevel"];
    ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED "\"loglevel\":\"%s\""), jtok.getStr());
    tkr_set->Settings.logging.mqtt_level = pCONT_log->GetLogLevelIDbyName(jtok.getStr());
    ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED "\"loglevel\":\"%S\""), pCONT_log->GetLogLevelNamebyID(tkr_set->Settings.logging.mqtt_level));
    // Add save log here
    data_buffer.isserviced++;
  }



  #ifdef ENABLE_DEVFEATURE_DEVICELIST_DEBUG_WITH_COMMANDS
  uint16_t module_id = 0;
  if(jtok = obj["DeviceNameList"].getObject()["ModuleID"])
  {
    module_id = jtok.getInt();
    ALOG_INF( PSTR("DeviceNameList ModuleID=%d"), module_id );
  }
  if(jtok = obj["DeviceNameList"].getObject()["SearchName"])
  {
    ALOG_INF( PSTR("DeviceNameList %s Result=%d ModuleID=%d"), jtok.getStr(), DLI->GetDeviceIDbyName(jtok.getStr(), module_id), module_id );
  }
  #endif // ENABLE_DEVFEATURE_DEVICELIST_DEBUG_WITH_COMMANDS

}


void mSupport::CommandSet_Restart(int8_t command)
{

  switch(command) 
  {
    // case 1:
    // restart_flag = 2;
    // //ResponseCmndChar(D_RESTARTING);
    // break;
    case -1:
      CmndCrash();    // force a crash
    break;
    case -2:
      CmndWDT();
    break;
    // case -3:
    // // OSWATCH_RESET_TIME
    //   CmndBlockedLoop();
    // break;
  // case 99:
  //   ALOG_INF(PSTR(D_LOG_APPLICATION D_RESTARTING));
  //   EspRestart();
  //   break;
    case 98:
      ALOG_INF(PSTR("ESP.restart();")); 
      delay(2000);
      ESP.restart();
    break;
    default:
      ALOG_INF(PSTR(D_ONE_TO_RESTART));
    break;
  }

}
