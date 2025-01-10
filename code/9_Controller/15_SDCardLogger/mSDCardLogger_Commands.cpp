
#include "mSDCardLogger.h"

#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER


// void mRelays::parse_JSONCommand(JsonParserObject obj){

//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;
//   JsonParserObject jobj = 0;
 
//   uint8_t relay_id= 0,state=-1;    //assume index 0 if none given

//   if(jtok = obj[PM_POWERNAME]){
//     if(jtok.isStr()){
//       relay_id = GetRelayIDbyName(jtok.getStr());
//     }else 
//     if(jtok.isNum()){
//       relay_id  = jtok.getInt();
//     }
//   }

//   // Primary method since v0.86.14.21
//   if(jtok = obj[PM_POWER_STATE]){
//     if(jtok.isStr()){
//       state = pCONT_sup->GetStateNumber(jtok.getStr());
//     }else 
//     if(jtok.isNum()){
//       state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
//     }

// 		//state needs checked for flipped
// 		// if(state == 2){

// 		// }

//   }

//   // PHASE OUT by version 0.87
//   if(jtok = obj[PM_ONOFF]){
//     if(jtok.isStr()){
//       state = pCONT_sup->GetStateNumber(jtok.getStr());
//     }else 
//     if(jtok.isNum()){
//       state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
//     }
//   }


  


//   if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON]){
//     CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
//   }else
//   if(jtok = obj[PM_RELAY].getObject()[PM_TIME_ON_SECS]){
//     CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
//   }

//   if(IsWithinRange(state, 0,10) && IsWithinRange(relay_id, 0,settings.relays_connected)){
//     CommandSet_Relay_Power(state,relay_id);
//   }


// 	//move this so its passed as object, hence "EnabledTime" can be search for by (no number and hence use powername) or "EnabledTime#" #=1,2,3 ... how does tas pull the numbers out? must match key partially

// 	// Search for match which uses relayindex
// 	if(jtok = obj["RelayEnabled"]){
// 		SubCommandSet_EnabledTime(jtok.getObject());
// 	}

//   char rule_name[20] = {0};
//   for(uint8_t ii=0;ii<3;ii++){
//     sprintf(rule_name, "RelayEnabled%d", ii);
      
// 		if(jtok = obj[rule_name]){
// 			ALOG_INF(PSTR("MATCHED %s"),rule_name);
// 			SubCommandSet_EnabledTime(jtok.getObject(), ii);
// 		}

//   }  

  


//   // if(jtok = obj["EnabledTime"]){
//   //   time_short_t ontime;
//   //   time_short_t offtime;
//   //   uint8_t index = 0;
    
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_INDEX]){
//   //     index = jtok.getInt();    
//   //   }
    
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_ONTIME]){
//   //     ontime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
//   //     relay_status[relay_id].enabled_ranges[index].ontime = ontime;
//   //   }
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_OFFTIME]){
//   //     offtime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
//   //     relay_status[relay_id].enabled_ranges[index].offtime = offtime;
//   //   }
//   //   if(jtok = obj["EnabledTime"].getObject()[PM_ENABLED]){
//   //     relay_status[relay_id].enabled_ranges[index].enabled = jtok.getInt();
//   //   }


//   //   tkr_time->PrintDateTime(ontime);
//   //   tkr_time->PrintDateTime(offtime);


//   // }


//   mqtthandler_scheduled_teleperiod.flags.SendNow = true;

// }




void mSDCardLogger::CommandSet_SDCard_OpenClose_Toggle()
{//uint16_t time_secs, uint8_t relay_id){

  sdcard_status.isopened ^= 1;

  ALOG_TST(PSTR("CommandSet_SDCard_OpenClose_Toggle %d"),sdcard_status.isopened);


  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

}



#endif // USE_MODULE_CONTROLLER_SDCARDLOGGER