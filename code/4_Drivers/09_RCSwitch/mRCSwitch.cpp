/**
  mRCSwitch.ino - RF transceiver using RcSwitch library

  Copyright (C) 2022    Michael Doone

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "mRCSwitch.h"

#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH


int8_t mRCSwitch::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {
    
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_50_MSECOND:
      ReceiveCheck();
    break;
    case TASK_UPDATE_OTA_BEFORE_ON_START:
      ALOG_INF(PSTR("disableReceive"));
      mySwitch->disableReceive();
    break;
    case TASK_EVERY_SECOND:{
      
      // AddLog(LOG_LEVEL_INFO,PSTR("tkr_set->Settings.rf_protocol_mask=%d"), tkr_set->Settings.rf_protocol_mask);

    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a TASK_MQTT_INIT and group mqtt togather
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate(); // Load teleperiod setting into local handlers
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif  
  }

}//end


void mRCSwitch::Pre_Init(void)
{

}


void mRCSwitch::Init(void)
{

  if (pCONT_pins->PinUsed(GPIO_RF_433MHZ_TX_ID)) {
    mySwitch->enableTransmit(pCONT_pins->GetPin(GPIO_RF_433MHZ_TX_ID));
    settings.fEnableSensor = true;
  }
  if (pCONT_pins->PinUsed(GPIO_RF_433MHZ_RX_ID)) 
  {
    if (tkr_set->Settings.rf_duplicate_time < 10) {
      tkr_set->Settings.rf_duplicate_time = RF_TIME_AVOID_DUPLICATE;
    }
    pinMode( pCONT_pins->GetPin(GPIO_RF_433MHZ_RX_ID), INPUT);

    mySwitch = new RCSwitch();

    mySwitch->enableReceive(pCONT_pins->GetPin(GPIO_RF_433MHZ_RX_ID));
    // if (!tkr_set->Settings.rf_protocol_mask) {
      // tkr_set->Settings.rf_protocol_mask = (1ULL << mySwitch->getNumProtos()) -1;
      // Correctly only permits protocol 1 through
      tkr_set->Settings.rf_protocol_mask = (1ULL << 1) -1; //only want number 2?
    // }
    mySwitch->setReceiveProtocolMask(tkr_set->Settings.rf_protocol_mask);
    settings.fEnableSensor = true;
  }

}


void mRCSwitch::ReceiveCheck(void) 
{

  if (mySwitch->available())
  {

    unsigned long data = mySwitch->getReceivedValue();
    unsigned int bits = mySwitch->getReceivedBitlength();
    int protocol = mySwitch->getReceivedProtocol();
    int delay = mySwitch->getReceivedDelay();

    ALOG_DBG(PSTR("RFR: Data 0x%lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);

    uint32_t now = millis();
    if ((now - rx_pkt.received_time_millis > tkr_set->Settings.rf_duplicate_time) && (data > 0))
    {
      rx_pkt.received_time_millis = now;

      /**
       * @brief If backoff period has happened, then save to struct
       **/
      rx_pkt.data = data;
      rx_pkt.bit_length = bits;
      rx_pkt.protocol = protocol;
      rx_pkt.delay = delay;
      rx_pkt.received_utc_time = tkr_time->GetTimeShortNowU32();

      /**
       * @brief Share with mqtt
       **/
			mqtthandler_state_ifchanged.flags.SendNow = true;

      /**
       * @brief TODO: Trigger Event "RF433_RECEIVED"
       **/

      //char stemp[16];
      // if (tkr_set->Settings.flag.rf_receive_decimal) {      // SetOption28 - RF receive data format (0 = hexadecimal, 1 = decimal)
      //   snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)data);
      // } else {
      //  snprintf_P(stemp, sizeof(stemp), PSTR("\"0x%lX\""), (uint32_t)data);
      // }

      // ResponseTime_P(PSTR(",\"" D_RFRECEIVED "\":{\"" D_RF_DATA "\":%s,\"" D_RF_BITS "\":%d,\"" D_RF_PROTOCOL "\":%d,\"" D_RF_PULSE "\":%d}}"),
      //   stemp, bits, protocol, delay);
      // MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_RFRECEIVED));

        ALOG_TST(PSTR("RFR: Data 0x%lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);
      
    }
    mySwitch->resetAvailable();
  }
}


// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// void CmndRfProtocol(void) {
//   if (!PinUsed(GPIO_RFRECV)) { return; }

// //  ALOG_INF(PSTR("RFR:CmndRfRxProtocol:: index:%d usridx:%d data_len:%d data:\"%s\""),XdrvMailbox.index, XdrvMailbox.usridx, XdrvMailbox.data_len,XdrvMailbox.data);

//   uint64_t thisdat;
//   if (1 == XdrvMailbox.usridx) {
//     if (XdrvMailbox.payload >= 0) {
//       thisdat = (1ULL << (XdrvMailbox.index -1));
//       if (XdrvMailbox.payload &1) {
//         tkr_set->Settings.rf_protocol_mask |= thisdat;
//       } else {
//         tkr_set->Settings.rf_protocol_mask &= ~thisdat;
//       }
//     }
//     else if (XdrvMailbox.data_len > 0) {
//       return;  // Not a number
//     }
//   } else {
//     if (XdrvMailbox.data_len > 0) {
//       if ('A' == toupper(XdrvMailbox.data[0])) {
//         tkr_set->Settings.rf_protocol_mask = (1ULL << mySwitch->getNumProtos()) -1;
//       } else {
//         thisdat = strtoull(XdrvMailbox.data, nullptr, 0);
//         if ((thisdat > 0) || ('0' == XdrvMailbox.data[0])) {
//           tkr_set->Settings.rf_protocol_mask = thisdat;
//         } else {
//           return;  // Not a number
//         }
//       }
//     }
//   }
//   mySwitch->setReceiveProtocolMask(tkr_set->Settings.rf_protocol_mask);
// //  ALOG_INF(PSTR("RFR: CmndRfProtocol:: Start responce"));
//   Response_P(PSTR("{\"" D_CMND_RFPROTOCOL "\":\""));
//   bool gotone = false;
//   thisdat = 1;
//   for (uint32_t i = 0; i < mySwitch->getNumProtos(); i++) {
//     if (tkr_set->Settings.rf_protocol_mask & thisdat) {
//       ResponseAppend_P(PSTR("%s%d"), (gotone) ? "," : "", i+1);
//       gotone = true;
//     }
//     thisdat <<=1;
//   }
//   if (!gotone) { ResponseAppend_P(PSTR(D_NONE_ENABLED)); }
//   ResponseAppend_P(PSTR("\""));
//   ResponseJsonEnd();
// }

// void CmndRfSend(void)
// {
//   if (!PinUsed(GPIO_RFSEND)) { return; }

//   bool error = false;

//   if (XdrvMailbox.data_len) {
//     unsigned long long data = 0;	// unsigned long long  => support payload >32bit
//     unsigned int bits = 24;
//     int protocol = 1;
//     int repeat = 10;
//     int pulse = 0; // 0 leave the library use the default value depending on protocol

//     JsonParser parser(XdrvMailbox.data);
//     JsonParserObject root = parser.getRootObject();
//     if (root) {
//       // RFsend {"data":0x501014,"bits":24,"protocol":1,"repeat":10,"pulse":350}
//       char parm_uc[10];
//       data = root.getULong(PSTR(D_RF_DATA), data);	// read payload data even >32bit
//       bits = root.getUInt(PSTR(D_RF_BITS), bits);
//       protocol = root.getInt(PSTR(D_RF_PROTOCOL), protocol);
//       repeat = root.getInt(PSTR(D_RF_REPEAT), repeat);
//       pulse = root.getInt(PSTR(D_RF_PULSE), pulse);
//     } else {
//       //  RFsend data, bits, protocol, repeat, pulse
//       char *p;
//       uint8_t i = 0;
//       for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 5; str = strtok_r(nullptr, ", ", &p)) {
//         switch (i++) {
//         case 0:
//           data = strtoul(str, nullptr, 0);  // Allow decimal (5246996) and hexadecimal (0x501014) input
//           break;
//         case 1:
//           bits = atoi(str);
//           break;
//         case 2:
//           protocol = atoi(str);
//           break;
//         case 3:
//           repeat = atoi(str);
//           break;
//         case 4:
//           pulse = atoi(str);
//         }
//       }
//     }

//     if (!protocol) { protocol = 1; }
//     mySwitch->setProtocol(protocol);
//     // if pulse is specified in the command, enforce the provided value (otherwise lib takes default)
//     if (pulse) { mySwitch->setPulseLength(pulse); }
//     if (!repeat) { repeat = 10; }     // Default at init
//     mySwitch->setRepeatTransmit(repeat);
//     if (!bits) { bits = 24; }         // Default 24 bits
//     if (data) {
//       mySwitch->send(data, bits);
//       ResponseCmndDone();
//     } else {
//       error = true;
//     }
//   } else {
//     error = true;
//   }
//   if (error) {
//     Response_P(PSTR("{\"" D_CMND_RFSEND "\":\"" D_NO " " D_RF_DATA ", " D_RF_BITS ", " D_RF_PROTOCOL ", " D_RF_REPEAT " " D_OR " " D_RF_PULSE "\"}"));
//   }
// }

// void CmndRfTimeOut(void) {
//   if (XdrvMailbox.payload >= 10) {
//     tkr_set->Settings.rf_duplicate_time = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(tkr_set->Settings.rf_duplicate_time);
// }

/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mRCSwitch::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["RfMask"])
	{

		if(jtok.isNum())
		{
			mySwitch->setReceiveProtocolMask(jtok.getUInt());
			mqtthandler_settings.flags.SendNow = true;
		}

		// JBI->Start();

		// pCONT->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		// bool ready_to_send = JBI->End();

		// if(!ready_to_send)
		// {
		// 	// Nothing was found, create new message
		// 	JBI->Start();
		// 		JBI->Add("SensorScan", "No Devices Found");
		// 	ready_to_send = JBI->End();
		// }


		// if(ready_to_send)
		// {			
    	ALOG_TST(PSTR("RfMask = %d / %d"), jtok.getUInt(), mySwitch->GetReceiveProtolMask());
		// 	pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		// }

	}



    
}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mRCSwitch::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);
    JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mRCSwitch::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Object_Start(D_RFRECEIVED);
  
      JBI->Add(D_DATA, rx_pkt.data);
      JBI->Add(D_RF_BITS, rx_pkt.bit_length);
      JBI->Add(D_RF_PROTOCOL, rx_pkt.protocol);
      JBI->Add(D_RF_PULSE, rx_pkt.delay);   
      JBI->Add(D_MILLIS, rx_pkt.received_time_millis);   
      JBI->Add(D_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    JBI->Object_End();
  
  

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mRCSwitch::MQTTHandler_Init()
{

  struct handler<mRCSwitch>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRCSwitch::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRCSwitch::ConstructJSON_State;

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mRCSwitch::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mRCSwitch::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mRCSwitch::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH