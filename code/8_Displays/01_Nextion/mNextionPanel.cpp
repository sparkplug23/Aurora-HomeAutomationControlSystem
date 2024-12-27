/**
 * @file mNextionPanel.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 * @note Initially based on oneHASP but should become its own code in due time
 *       Latest code revision with oneHASP is version 1.03 on 9Aug22

 * @desc To enable control of Nextion panels using mqtt, allowing pass through commands and improved automation (eg dimmer, regexp of colours)

https://www.youtube.com/watch?v=v8Uyam9u6QI


Should I implement a parser for the commands? This would make openhab much easier
ie p[2].b[2] command, should be able to be parsed json style message and sharing as normal mqtt. 

 */
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

int8_t mNextionPanel::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();  
      break;
    case TASK_INIT:
      Init();
    break;
  }

  // if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
    
      if(!update_in_progress)
        EveryLoop();

    break;
    case TASK_EVERY_SECOND:
      if(!update_in_progress)
      {
        EverySecond_ActivityCheck();
        if(splash_page_seconds)
        {
          splash_page_seconds--;
          if(splash_page_seconds==0) // The final time the if above is true, is when it is 1, then -- makes this 0 and thus true
          {
            Command_SetPage(splash_page_saved_page); // return to saved page
          }
        }
      }
    break;
    case TASK_EVERY_MINUTE:
      // EverySecond_SendScreenInfo();
    break;
    case TASK_EVERY_HOUR:
      // Command_SetPage(settings.page);   //temp fix
    break;
    case TASK_RESTART_STABLE:      
      nextionSendCmd("page message");
      nextionSetAttr("message.main.txt", "\"System Rebooting...\"");
      nextionSetAttr("message.main.bco", NEXTION_16BIT_COLOUR_GREEN_STR);
      display->flush();
    break;
    /************
     * Network SECTION * 
    *******************/
    case TASK_WIFI_STARTING_CONNECTION:      
      nextionSendCmd("page message");
      nextionSetAttr("message.main.txt", "\"WiFi Connecting...\"");
      nextionSetAttr("message.main.bco", NEXTION_16BIT_COLOUR_RED_STR);
    break;
    case TASK_WEB_ADD_HANDLER:
      WebPage_AddHandlers();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
    
      if(!update_in_progress && !pCONT_sup->arduino_ota_triggered)
        parse_JSONCommand(obj);

    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      mqttConnected();
      Show_ConnectionWorking();
    break;
    case TASK_MQTT_DISCONNECTED:
      mqttDisconnected();
      Show_ConnectionNotWorking();
    break;
    case TASK_MQTT_SUBSCRIBE:
      MQTTSubscribe();
    break;
    #endif
  }

}



void mNextionPanel::Command_SplashPage(char* pagename, uint8_t time_on_page)
{
  // saved current page to return to later
  splash_page_saved_page = settings.page;
  // Change page by name
  Command_SetPage(pagename);
  // start timer to return to saved page
  splash_page_seconds = time_on_page;

}


void mNextionPanel::Show_ConnectionWorking()
{
  Command_SplashPage("message", 2);
  
  nextionSetAttr("message.main.txt", "\"Computer connected!\"");
  uint16_t colour565 = RGB888_to_RGB565(0,255,255);
  nextionSetAttr("message.main.bco", colour565);

}


void mNextionPanel::Show_ConnectionNotWorking()
{
  nextionSendCmd("page message");
  nextionSetAttr("message.main.txt", "\"Connection Lost!\"");
  nextionSetAttr("message.main.bco", NEXTION_16BIT_COLOUR_RED_STR);
}



void mNextionPanel::Pre_Init(void)
{

  module_state.mode = ModuleStatus::Initialising;

  // Plan to make Serial the primary code, then everything here will interface into it. 
  // For now, just using locally

  uint8_t serial_port = 1; // default
  uint8_t rx_pin = 0;
  uint8_t tx_pin = 0;
  if(pCONT_pins->PinUsed(GPIO_NEXTION_RX_ID) && pCONT_pins->PinUsed(GPIO_NEXTION_TX_ID))
  {
    serial_port = 1;
    rx_pin = pCONT_pins->GetPin(GPIO_NEXTION_RX_ID);
    tx_pin = pCONT_pins->GetPin(GPIO_NEXTION_TX_ID);
  }
  else if(pCONT_pins->PinUsed(GPIO_NEXTION_RX2_ID) && pCONT_pins->PinUsed(GPIO_NEXTION_TX2_ID))
  {
    serial_port = 2;
    rx_pin = pCONT_pins->GetPin(GPIO_NEXTION_RX2_ID);
    tx_pin = pCONT_pins->GetPin(GPIO_NEXTION_TX2_ID);
  }else{
    module_state.mode = ModuleStatus::NoGPIOConfigured;
    return;
  }

  display = new HardwareSerial(serial_port);
  
  ALOG_COM(PSTR(D_LOG_NEXTION "Using GPIO%d for Nextion RX"), rx_pin);

  pinMode( rx_pin, OUTPUT); // RX - try forcing these to GPIO to stop serial comms
  pinMode( tx_pin, OUTPUT); // TX - try forcing these to GPIO to stop serial comms

  /**
   * @brief ESP32 is hanging when starting Serial 1 or 2.
   * A work around is to start at a low baud, then switch to the desired baud to avoid hanging
   **/
  display->begin(
    9600,
    SERIAL_8N1,
    rx_pin, // RX
    tx_pin  // TX
  );
  display->updateBaudRate(ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT);


  module_state.mode = ModuleStatus::Initialising; 
  
}


void mNextionPanel::Init()
{ 

  // display->begin(115200);

  if (!nextionConnect())
  {
    if (lcdConnected)
    {
      ALOG_INF(PSTR("HMI: LCD responding but initialization wasn't completed. Continuing program load anyway."));
      module_state.devices++;
    }
    else
    {
      ALOG_INF(PSTR("HMI: LCD not responding, continuing program load"));
      module_state.devices++;
    }
  }else{
    module_state.devices++;
  }

  delay(1000);
  
  // init variables
  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));
  // memcpy(lcdVersionQuery,"p[0].b[2].val",sizeof("p[0].b[2].val"));

  memset(flash_message.message,0,sizeof(flash_message.message));
  sprintf(flash_message.message,"%s","Awaiting a message to show");

  char page_default_command[10];
  sprintf(page_default_command,"page %d",NEXTION_DEFAULT_PAGE_NUMBER);



  #ifdef USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT  
  CommandSet_Baud(USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT);
  #endif 


  #ifdef NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  Template_Load_Init_Display_Command();
  #endif // NEXTION_INIT_PANEL_COMMAND_TEMPLATE

  settings.page = NEXTION_DEFAULT_PAGE_NUMBER;
  Command_SetPage(settings.page);

  
  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }



}




void mNextionPanel::EverySecond_ActivityCheck()
{

  //  if(fOpenHABDataStreamActive_last_secs){ //only if positive
  //     if(fOpenHABDataStreamActive_last_secs++>OPENHAB_DATA_STREAM_ACTIVITY_TIMEOUT_SECS){
  //       fOpenHABDataStreamActive_last_secs = -1;
  //       fOpenHABDataStreamActive = false;
  //     }
  //   }

  ALOG_DBG(PSTR("fOpenHABDataStreamActive_last_secs = %d"), settings.timeout_check.timeout_period );

  if(settings.timeout_check.timeout_period==1)
  {
    ALOG_INF(PSTR("No data"));
    Show_ConnectionNotWorking();
    settings.page = -1; // unset to force update when available
  }
  else
  if(settings.timeout_check.timeout_period > 1)
  {
    settings.timeout_check.timeout_period--;
  }
  else
  {
    settings.timeout_check.timeout_period = 0; //stop
  }
    

}



void mNextionPanel::EveryLoop()
{

  nextionHandleInput();     // Nextion serial communications loop

  // // if ((lcdVersion < 1) && (millis() <= (nextionRetryMax * nextionCheckInterval)))
  // // { // Attempt to connect to LCD panel to collect model and version info during startup
  // //   nextionConnect();
  // //   // ALOG_ERR(PSTR("Invalid response, disabling nextionConnect for now"));
  // // }
    
  // Check if long press threshold reached
  if(screen_press.fEnableImmediateButtonTime){
    if(mTime::TimeReachedNonReset(&screen_press.tSavedButtonONEvent,LONG_PRESS_DURATION)){
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "LONG_PRESS_DURATION reached"));
      screen_press.fEnableImmediateButtonTime=false;
      fEnableIgnoreNextOffEvent = true;
      MQTTSend_LongPressEvent();
      MQTTSend_PressEvent();
    }
  }

}


  
/******************************************************************************************************************
 * SECTION: Commands
*******************************************************************************************************************/


void mNextionPanel::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken  jtok = 0; 
  JsonParserObject jobj = 0;
  int8_t tmp_id = 0;

  if(jtok = obj["Debug"].getObject()["Nextion"])
  {
    settings.dynamic_log_level = jtok.getInt();
    ALOG_COM(PSTR(D_LOG_NEXTION D_COMMAND_NVALUE_K("Debug")), settings.dynamic_log_level);
  }

  
  if(jtok = obj["Display"].getObject()["SetPageIfChanged"])
  {
    // if(jtok.isStr())
    // {
    //   Command_SetPageIfChanged(jtok.getInt());
    // }
    // else
    // if(jtok.isInt())
    // {
      Command_SetPageIfChanged(jtok.getInt());
    // }
  }


  if(jtok = obj["commands"]){
      JsonParserArray array = jtok;
      for(auto val : array) {
          ALOG_INF(PSTR("F::%s %s"),__FUNCTION__,val.getStr());
          nextionSendCmd(val.getStr());
          data_buffer.isserviced++;
      }
  }




  /**
   * @brief Construct a new if object
   * I need to add check, if failed (ie due to wrong screen) then stop attempting to update the rest of the messages to stop overflow of UART
   * 
   */
  #ifdef ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED
  if(jtok = obj["commands_formatted"]){
      JsonParserArray array = jtok;
      for(auto val : array) {
          ALOG_DBM( PSTR("F::%s %s"),__FUNCTION__,val.getStr());
          // need to add bool for bad response
          nextionSendCmd_ContainingFormattedText(val.getStr());
          data_buffer.isserviced++;
      }
      settings.timeout_check.timeout_period = 120; //make fucntion to reset it
  }
  #endif // ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED


    if(jtok = obj["Nextion"])
    {      
      SubParse_DisplaySet_JSON(jtok.getObject());
      settings.timeout_check.timeout_period = 10; //make fucntion to reset it
    }



    
    if(jtok = obj["Display"].getObject()["Println"]){
        // JsonParserArray array = jtok;
        // for(auto val : array) {
        //     // ALOG_INF(PSTR("F::%s %s"),__FUNCTION__,val.getStr());
        //     nextionSendCmd(val.getStr());
        // }
        display->println(jtok.getStr());
    }


    if(jtok = obj["Display"].getObject()[PM_BAUDRATE])
    {
      CommandSet_Baud(jtok.getInt());
      ALOG_COM(PSTR(D_LOG_NEXTION D_COMMAND_NVALUE_K(D_BAUDRATE)), jtok.getInt());
    }



  if(jtok = obj["Nextion"].getObject()["GetBaud"])
  {
    ALOG_COM(PSTR("display->baudRate=%d"), display->baudRate());
  }

  /***
   * 
   * Create new functions below that assume they are under "Nextion" json, but just parse that one bit out here for each the once
   * 
   * 
  */
  JsonParserObject obj1 = 0;
  JsonParserToken jtok_nextion = 0;
  JsonParserToken jtok_parse = 0;

  if(jtok_nextion = obj["Nextion"])
  {
    JsonParserObject json_obj = jtok_nextion.getObject();

    if(jtok_parse = json_obj["SetBrightness"])
    {
      Command_SetBrightness255(jtok_parse.getInt());
    }

  }




}

void mNextionPanel::SubParse_DisplaySet_JSON(JsonParserObject obj)
{

  ALOG_DBG(PSTR("mNextionPanel::SubParse_DisplaySet_JSON"));

  JsonParserToken jtok_items = 0;
  JsonParserToken jtok_items2 = 0;
  JsonParserToken jtok = 0;
  JsonParserToken jtok_item_attribute = 0;

  JsonParserObject obj1 = 0;

  
  char page_name[100] = {0};
  char item_name[100] = {0};
  char item_value[100] = {0};
  char command_ctr[100] = {0};

  /**
   * @brief Construct a new if object
   * If no page name is sent, then send commands without page name
   * 
   */

  if(jtok_items = obj["Page"])
  {
    sprintf(page_name,"%s",jtok_items.getStr());
    Command_SetPageIfChanged(6); //tmp fix
    ALOG_WRN(PSTR("Page is forced as 6 by default, need to fix this"));
  }


  if(obj1 = obj["Items"])
  {

    if(!strlen(page_name))
    {
      ALOG_DBG(PSTR("No page name sent, sending commands without page name"));
    }else{
      sniprintf(page_name, sizeof(page_name), "%s.", page_name); // add "." to page name to make code below easier
    }
    
    for(auto key : obj1) 
    {
      
      const char* key_string = key.getStr();
      ALOG_DBG(PSTR("Key = \"%s\""), key_string);
      sprintf(item_name,"%s",key_string);

      if(jtok = obj1[key_string])
      {

        if(jtok_item_attribute = jtok.getObject()["txt"])
        {
          ALOG_DBG(PSTR("txt = \"%s\""), jtok_item_attribute.getStr());
          sprintf(command_ctr,"%s%s.txt=\"%s\"", page_name, item_name, jtok_item_attribute.getStr());
          nextionSendCmd(command_ctr); 
        }

        if(jtok_item_attribute = jtok.getObject()["val"])
        {
          ALOG_DBG(PSTR("val = %d"), jtok_item_attribute.getInt());
          sprintf(command_ctr,"%s%s.val=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr);  
        }
        
        if(jtok_item_attribute = jtok.getObject()["valf"])
        {
          char convf[TBUFFER_SIZE_FLOAT];
          mSupport::float2CString(jtok_item_attribute.getFloat(), 1, convf);
          ALOG_DBG(PSTR("val = \"%s\""), convf);          
          sprintf(command_ctr,"%s%s.val=%s", page_name, item_name, convf); //another conversion will be needed for float here later
          nextionSendCmd(command_ctr); 
        }

        if(jtok_item_attribute = jtok.getObject()["pco"])
        {
          ALOG_DBG(PSTR("pco = %d"), jtok_item_attribute.getInt());          
          sprintf(command_ctr,"%s%s.pco=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr); 
        }

        if(jtok_item_attribute = jtok.getObject()["pco8"]) //8 bit standard R,G,B, needs conversion to 16 bit
        {
          if(jtok_item_attribute.isStr())
          {
            ALOG_DBG(PSTR("pco8 = %s"), jtok_item_attribute.getStr());   
            char colour_string[10] = {0};
            snprintf(colour_string, sizeof(colour_string), "%s", jtok_item_attribute.getStr());
            if(colour_string[0] == '#') // hex format of RGB in RGB888
            {
              uint32_t colour32bit = 0;
              colour32bit = (long) strtol( &colour_string[1], NULL, 16);
              sprintf(command_ctr,"%s%s.pco=%d", page_name, item_name, RGB888_to_RGB565(colour32bit));
              nextionSendCmd(command_ctr); 
            }
          }
        }

        if(jtok_item_attribute = jtok.getObject()["bco"])
        {
          ALOG_DBG(PSTR("bco = %d"), jtok_item_attribute.getInt());          
          sprintf(command_ctr,"%s%s.bco=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr); 
        }
        
        if(jtok_item_attribute = jtok.getObject()["bco8"]) //8 bit standard R,G,B, needs conversion to 16 bit
        {
          if(jtok_item_attribute.isStr())
          {
            ALOG_DBG(PSTR("bco8 = %s"), jtok_item_attribute.getStr());   
            char colour_string[10] = {0};
            snprintf(colour_string, sizeof(colour_string), "%s", jtok_item_attribute.getStr());
            if(colour_string[0] == '#') // hex format of RGB in RGB888
            {
              uint32_t colour32bit = 0;
              colour32bit = (long) strtol( &colour_string[1], NULL, 16);
              sprintf(command_ctr,"%s%s.bco=%d", page_name, item_name, RGB888_to_RGB565(colour32bit));
              nextionSendCmd(command_ctr); 
            }
          }
        }
        
        if(jtok_item_attribute = jtok.getObject()["pic"])
        {
          ALOG_DBG(PSTR("pic = %d"), jtok_item_attribute.getInt());          
          sprintf(command_ctr,"%s%s.pic=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr); 
        }
        
        data_buffer.isserviced++;      
        
      }

    }

  }

}


std::string& replace(std::string& s, const std::string& from, const std::string& to)
{
    if(!from.empty())
        for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
            s.replace(pos, from.size(), to);
    return s;
}


void mNextionPanel::CommandSet_Baud(uint32_t baud)
{

  // else if ((strTopic == (mqttCommandTopic + "/nextionbaud") || strTopic == (mqttGroupCommandTopic + "/nextionbaud")) &&
  //          ((strPayload.toInt() == 2400) ||
  //           (strPayload.toInt() == 4800) ||
  //           (strPayload.toInt() == 9600) ||
  //           (strPayload.toInt() == 19200) ||
  //           (strPayload.toInt() == 31250) ||
  //           (strPayload.toInt() == 38400) ||
  //           (strPayload.toInt() == 57600) ||
  //           (strPayload.toInt() == 115200) ||
  //           (strPayload.toInt() == 230400) ||
  //           (strPayload.toInt() == 250000) ||
  //           (strPayload.toInt() == 256000) ||
  //           (strPayload.toInt() == 512000) ||
  //           (strPayload.toInt() == 921600)))
  // {                                         // '[...]/device/command/nextionbaud' -m '921600' == nextionBaud = 921600


    // strPayload.toCharArray(nextionBaud, 7); // set nextionBaud to value provided in payload
    nextionAckEnable = false;

    char command_ctr[30];
    
    sprintf(command_ctr,"bauds=%d", baud);

    nextionSendCmd(command_ctr); // send baud rate to nextion

    nextionAckEnable = true;


    display->flush();
    display->end();
    display->begin(baud); // Serial2 - LCD TX, no RX

    ALOG_INF(PSTR("Set Baud Rate = %d"), baud);

}
 


void mNextionPanel::MQTTSubscribe()
{


}



void mNextionPanel::SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr){
  char command_ctr[100];
  sprintf(command_ctr,"p[%d].b[%d].txt=\"%s\"",page,element_id,ctr);
  serial_print_suffixed(command_ctr);  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].bco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].pco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}




void mNextionPanel::nextionSetAttr(const char* hmiAttribute, const char* hmiValue)
{ // Set the value of a Nextion component attribute
  display->print(hmiAttribute);
  display->print("=");
  // display->print(utf8ascii((char*)hmiValue));
  display->print(utf8ascii((String)hmiValue));
  display->write(nextionSuffix, sizeof(nextionSuffix));
  
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}

void mNextionPanel::nextionSetAttr(const char* hmiAttribute, uint32_t value)
{ // Set the value of a Nextion component attribute

  display->print(hmiAttribute);
  display->print("=");
  display->print(value);
  display->write(nextionSuffix, sizeof(nextionSuffix));
  
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}

void mNextionPanel::nextionGetAttr(const char* c_str)
{ // Get the value of a Nextion component attribute
  // This will only send the command to the panel requesting the attribute, the actual
  // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic

  char hmiattribute_ctr[100]; 
  memset(hmiattribute_ctr,0,sizeof(hmiattribute_ctr));
  sprintf(hmiattribute_ctr,"get %s",c_str);
  serial_print_suffixed(hmiattribute_ctr);
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " GET 'get %s'"),hmiattribute_ctr);
}


void mNextionPanel::nextionSendCmd(const char* c_str)
{ // Send a raw command to the Nextion panel
  serial_print(utf8ascii(c_str));
  serial_print_suffix();
  AddLog(settings.dynamic_log_level,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),c_str);
}


void mNextionPanel::nextionSendCmd_String(const String &nextionCmd)
{ // Send a raw command to the Nextion panel
  display->print(nextionCmd);
  display->write(nextionSuffix, sizeof(nextionSuffix));
  display->flush();
  ALOG_INF(PSTR(D_LOG_NEXTION "HMI OUT: %s"), nextionCmd);

  if (nextionAckEnable)
  {
    nextionAckReceived = false;
    nextionAckTimer = millis();

    while ((!nextionAckReceived) && (millis() - nextionAckTimer < nextionAckTimeout))
    {
      nextionHandleInput();
    }
    if (!nextionAckReceived)
    {
      ALOG_INF(PSTR(D_LOG_NEXTION "HMI ERROR: Nextion Ack timeout"));
      String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
      // mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      // ALOG_INF(PSTR(D_LOG_NEXTION "MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
    }
  }
  else
  {
    nextionHandleInput();
  }
}



  #ifdef ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED
void mNextionPanel::nextionSendCmd_ContainingFormattedText(const char* c_str)
{ // Send a raw command to the Nextion panel

//move format checks here
  char conversion_buffer[100] = {0};

  snprintf(conversion_buffer, sizeof(conversion_buffer), "%s", c_str);

  // ALOG_INF(PSTR("before %s"),conversion_buffer);

  uint8_t command_length = strlen(conversion_buffer);  // use this later to check if "matched index + key + 3 data hue bytes" = "length" for vailidity check

  uint16_t converted_565_number = 0;
  uint16_t hue_input = 0;


  char* pos_start_of_token = nullptr;
  char* pos_start_of_response_field = nullptr;

  if((pos_start_of_token=strstr(conversion_buffer, "co=h"))!=nullptr) //pco and bco, returns pointer to substring
  {              
    // ALOG_INF(PSTR("MATCHED %s"),pos_start_of_token);
    pos_start_of_response_field = pos_start_of_token+3; // Only 3, since "h" is not part of the output command

    hue_input = pCONT_sup->TextToInt(pos_start_of_response_field+1); //skipping h to numbers only

    float r,g,b;
    
    HueToRgb(hue_input, &r,&g,&b);

    uint8_t red = (int)r;
    uint8_t green = (int)g;
    uint8_t blue = (int)b;

    uint16_t Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));


    // ALOG_INF(PSTR("Rgb565 = %d"), Rgb565);


    sprintf(pos_start_of_response_field, "%d\0", Rgb565);

  }
  else
  if((pos_start_of_token=strstr(conversion_buffer, "co=#"))!=nullptr) //pco and bco, returns pointer to substring, "#" (capital) for byte packed rgb
  {               
    // ALOG_INF(PSTR("MATCHED %s"),pos_start_of_token);
    pos_start_of_response_field = pos_start_of_token+3; // Only 3, since "h" is not part of the output command

    // hue_input = pCONT_sup->TextToInt(pos_start_of_response_field+1); //skipping h to numbers only

    
    // ALOG_INF(PSTR("pos_start_of_response_field = %s"), pos_start_of_response_field);

    uint32_t colour32bit = 0;
    if(pos_start_of_response_field[0]=='#'){ colour32bit = (long) strtol( &pos_start_of_response_field[1], NULL, 16);
    }else{ colour32bit = (long) strtol( &pos_start_of_response_field[0], NULL, 16); }

    // RgbColor rgb;
    // rgb.R = colour32bit >> 16; //RGB
    // rgb.G = colour32bit >> 8 & 0xFF; //RGB
    // rgb.B = colour32bit & 0xFF; //RGB


    // float r,g,b;
    
    // HueToRgb(hue_input, &r,&g,&b);

    uint8_t red   = colour32bit >> 16; //RGB
    uint8_t green = colour32bit >> 8 & 0xFF; //RGB
    uint8_t blue  = colour32bit & 0xFF; //RGB

    uint16_t Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));

// 
    // ALOG_INF(PSTR("Rgb565 = %d"), Rgb565);


    sprintf(pos_start_of_response_field, "%d\0", Rgb565);

  }
  else{
    // ALOG_INF(PSTR("NO match"));

  }

  nextionSendCmd(conversion_buffer);
}

void mNextionPanel::HueToRgb(uint16_t hue, float* r, float* g, float* b)
{
    float h = hue/360.0f;
    float s = 1.0f;
    float v = 1.0f;

    if (s == 0.0f){ 
        *r = *g = *b = v; // achromatic or black
    }
    else
    {
        if (h < 0.0f)
        {
            h += 1.0f;
        }
        else if (h >= 1.0f)
        {
            h -= 1.0f;
        }
        h *= 6.0f;
        int i = (int)h;
        float f = h - i;
        float q = v * (1.0f - s * f);
        float p = v * (1.0f - s);
        float t = v * (1.0f - s * (1.0f - f));
        switch (i)
        {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:
            *r = v;
            *g = p;
            *b = q;
            break;
        }
    }

    *r *= 255;
    *g *= 255;
    *b *= 255;

}


  #endif // ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED


/******************************************************************************************************************
 * SECTION: HardwareInterface
*******************************************************************************************************************/

void mNextionPanel::nextionReset()
{
  AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD");
  
  
  display->print("rest");
  display->write(nextionSuffix, sizeof(nextionSuffix));
  display->flush();
  
  delay(100);
  //digitalWrite(nextionResetPin, HIGH);

  unsigned long lcdResetTimer = millis();
  const unsigned long lcdResetTimeout = 5000;

  lcdConnected = false;
  while (!lcdConnected && (millis() < (lcdResetTimer + lcdResetTimeout)))
  {
    
    display->print("rest");
    display->write(nextionSuffix, sizeof(nextionSuffix));
    display->flush();

    delay(100);

    // Serial.println("while-nextionReset");
    nextionHandleInput();
  }
  if (lcdConnected)
  {
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD completed");
    if (settings.page)
    {
      char command_ctr[15];
      sprintf(command_ctr,"page %d\0",settings.page);
      nextionSendCmd(command_ctr);
    }
  }
  else
  {
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "%s"),"ERROR: Rebooting LCD completed, but LCD is not responding.");
  }
  //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
}


bool mNextionPanel::nextionConnect()
{


  // if ((millis() - nextionCheckTimer) >= nextionCheckInterval)
  // {
  //   static unsigned int nextionRetryCount = 0;
  //   if ((nextionModel.length() == 0) && (nextionRetryCount < (nextionRetryMax - 2)))
  //   { // Try issuing the "connect" command a few times
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion connect request"));
  //     nextionSendCmd("connect");
  //     nextionRetryCount++;
  //     nextionCheckTimer = millis();
  //   }
  //   else if ((nextionModel.length() == 0) && (nextionRetryCount < nextionRetryMax))
  //   { // If we still don't have model info, try to change nextion serial speed from 9600 to 115200
  //     nextionSetSpeed();
  //     nextionRetryCount++;
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion serial speed 115200 request"));
  //     nextionCheckTimer = millis();
  //   }
  //   else if ((lcdVersion < 1) && (nextionRetryCount <= nextionRetryMax))
  //   {
  //     if (nextionModel.length() == 0)
  //     { // one last hail mary, maybe the serial speed is set correctly now
  //       nextionSendCmd("connect");
  //     }
  //     //nextionSendCmd("get " + lcdVersionQuery.toString().c_str());
  //     lcdVersionQueryFlag = true;
  //     nextionRetryCount++;
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion version query"));
  //     nextionCheckTimer = millis();
  //   }
  // }

DEBUG_LINE_HERE;

  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));

  const unsigned long nextionCheckTimeout = 2000; // Max time in msec for nextion connection check
  unsigned long nextionCheckTimer = millis();     // Timer for nextion connection checks

  display->write(nextionSuffix, sizeof(nextionSuffix));

  nextionSendCmd("connect"); // 
  // S:comok 1,30601-0,NX3224T024_011R,163,61488,DE6064B7E70C6521,4194304ÿÿÿ // response from connect


  if (!lcdConnected)
  { // Check for some traffic from our LCD
    ALOG_INF(PSTR("HMI: Waiting for LCD connection"));
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && !lcdConnected)
    {
      nextionHandleInput();
    }
  }

  if (!lcdConnected)
  { // No response from the display using the configured speed, so scan all possible speeds
    nextionSetSpeed();

    nextionCheckTimer = millis(); // Reset our timer
    ALOG_INF(PSTR("HMI: Waiting again for LCD connection"));
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && !lcdConnected)
    {
      display->write(nextionSuffix, sizeof(nextionSuffix));
      nextionHandleInput();
    }
    if (!lcdConnected)
    {
      ALOG_INF(PSTR("HMI: LCD connection timed out"));
      return false;
    }
  }

  // Query backlight status.  This should always succeed under simulation or non-HASPone HMI
  lcdBacklightQueryFlag = true;
  ALOG_INF(PSTR("HMI: Querying LCD backlight status"));
  display->write(nextionSuffix, sizeof(nextionSuffix));
  nextionSendCmd("get dim");
  while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && lcdBacklightQueryFlag)
  {
    nextionHandleInput();
  }
  if (lcdBacklightQueryFlag)
  { // Our flag is still set, meaning we never got a response.
    ALOG_INF(PSTR("HMI: LCD backlight query timed out"));
    lcdBacklightQueryFlag = false;
    return false;
  }

  // We are now communicating with the panel successfully.  Enable ACK checking for all future commands.
  nextionAckEnable = true;
  nextionSendCmd("bkcmd=3");

  // This check depends on the HMI having been designed with a version number in the object
  // defined in lcdVersionQuery.  It's OK if this fails, it just means the HMI project is
  // not utilizing the version capability that the HASPone project makes use of.
  lcdVersionQueryFlag = true;
  ALOG_INF(PSTR("HMI: Querying LCD firmware version number"));
  nextionSendCmd_String("get " + lcdVersionQuery);
  while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && lcdVersionQueryFlag)
  {
    nextionHandleInput();
  }
  if (lcdVersionQueryFlag)
  { // Our flag is still set, meaning we never got a response.  This should only happen if
    // there's a problem.  Non-HASPone projects should pass this check with lcdVersion = 0
    ALOG_INF(PSTR("HMI: LCD version query timed out"));
    lcdVersionQueryFlag = false;
    return false;
  }

  if (nextionModel.length() == 0)
  { // Check for LCD model via `connect`.  The Nextion simulator does not support this command,
    // so if we're running under that environment this process should timeout.
    ALOG_INF(PSTR("HMI: Querying LCD model information"));
    nextionSendCmd("connect");
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && (nextionModel.length() == 0))
    {
      nextionHandleInput();
    }
  }
  return true;


}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSetSpeed()
{
  
  ////#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    Serial.println(F("HMI: No Nextion response, attempting 9600bps connection\n\n\n\n\n\n\n"));
    swSer->begin(9600);
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->print("bauds=38400");
    //swSer->print("bauds=9600");
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
    swSer->begin(38400);
  #endif

//   debugPrintln(String(F("HMI: No Nextion response, attempting to set serial speed to ")) + String(nextionBaud));
//   for (unsigned int nextionSpeedsIndex = 0; nextionSpeedsIndex < nextionSpeedsLength; nextionSpeedsIndex++)
//   {
//     debugPrintln(String(F("HMI: Sending bauds=")) + String(nextionBaud) + " @" + String(nextionSpeeds[nextionSpeedsIndex]) + " baud");
//     display->flush();
//     display->begin(nextionSpeeds[nextionSpeedsIndex]);
//     display->write(nextionSuffix, sizeof(nextionSuffix));
//     display->write(nextionSuffix, sizeof(nextionSuffix));
//     display->write(nextionSuffix, sizeof(nextionSuffix));
//     display->print("bauds=" + String(nextionBaud));
//     display->write(nextionSuffix, sizeof(nextionSuffix));
//     display->flush();
//   }
//   display->begin(atoi(nextionBaud));

}





////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionHandleInput()
{ // Handle incoming serial data from the Nextion panel
  // This will collect serial data from the panel and place it into the global buffer
  // nextionReturnBuffer[nextionReturnIndex]
  unsigned long handlerTimeout = millis() + 100;
  bool nextionCommandComplete = false;
  static uint8_t nextionTermByteCnt = 0; // counter for our 3 consecutive 0xFFs

  while (display->available() && !nextionCommandComplete && (millis() < handlerTimeout))
  {
    byte nextionCommandByte = display->read();
    // Serial.write(nextionCommandByte);
    if (nextionCommandByte == 0xFF)
    { // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
      nextionTermByteCnt++;
      if (nextionTermByteCnt >= 3)
      { // We have received a complete command
        // Serial.println("END");
        lcdConnected = true;
        nextionCommandComplete = true;
        nextionTermByteCnt = 0; // reset counter
      }
    }
    else
    {
      nextionTermByteCnt = 0; // reset counter if a non-term byte was encountered
    }
    nextionReturnBuffer[nextionReturnIndex] = nextionCommandByte;
    nextionReturnIndex++;
    if (nextionCommandComplete)
    {
      nextionAckReceived = true;
      nextionProcessInput();
    }
    yield();
  }


  if (millis() > handlerTimeout)
  {
    ALOG_INF(PSTR(D_LOG_NEXTION "HMI ERROR: nextionHandleInput timeout"));
  }

}




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionProcessInput()
{ // Process incoming serial commands from the Nextion panel
  // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
  // tl;dr, command byte, command data, 0xFF 0xFF 0xFF

  // Serial.printf("nextionProcessInput Instruction= %d\n\r", nextionReturnBuffer[0]);

  // ALOG_INF(PSTR("nextionProcessInput = %02x, %d, %d, %d, %d"), nextionReturnBuffer[0], nextionReturnBuffer[1], nextionReturnBuffer[2], nextionReturnBuffer[3], nextionReturnBuffer[4] );


  // DEBUG_LINE_HERE;
  
  char event_ctr[30];
  //https://www.itead.cc/wiki/Nextion_Instruction_Set

  // first instructions byte
  switch(nextionReturnBuffer[0]){
    case NEXTION_COMMAND_INVALID_INSTRUCTION:
      //AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX D_NEXTION_COMMAND D_NEXTION_COMMAND_INVALID_INSTRUCTION_CTR));   
    break;

  }
  

  if (nextionReturnBuffer[0] == INSTRUCTION_SET_RETURN_CODE__TOUCH_EVENT)
  { // Handle incoming touch command
    // 0x65+Page ID+Component ID+TouchEvent+End
    // Return this data when the touch event created by the user is pressed.
    // Definition of TouchEvent: Press Event 0x01, Release Event 0X00
    // Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
    // Meaning: Touch Event, Page 0, Object 2, Press
    String nextionPage = String(nextionReturnBuffer[1]);
    String nextionButtonID = String(nextionReturnBuffer[2]);
    byte nextionButtonAction = nextionReturnBuffer[3];

    screen_press.page = nextionReturnBuffer[1];
    screen_press.event = nextionReturnBuffer[2];

    if (nextionButtonAction == 0x01) // ON=PRESSED
    {
  
      screen_press.tSavedButtonONEvent = millis();
      screen_press.fEnableImmediateButtonTime = true; 

      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_ON);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);

      JBI->Start();
        JBI->Add("Page", screen_press.page);
        JBI->Add("ID", screen_press.event);
        JBI->Add("event", event_ctr);
        JBI->Add("value", D_ON);
      JBI->End();

      pCONT_mqtt->Publish("status/nextion/event",JBI->GetBufferPtr(),0);
      pCONT_mqtt->Publish("status/nextion/event/start",JBI->GetBufferPtr(),0);

    }
    if (nextionButtonAction == 0x00) // OFF - LET_GO
    {
  
      screen_press.tSavedButtonOFFEvent = millis();
      screen_press.fEnableImmediateButtonTime = false; //start timer
      screen_press.tSavedButtonONDurationEvent = screen_press.tSavedButtonOFFEvent - screen_press.tSavedButtonONEvent;
      screen_press.duration = screen_press.tSavedButtonONDurationEvent;
      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_OFF);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
      
      JBI->Start();
        JBI->Add("Page", screen_press.page);
        JBI->Add("ID", screen_press.event);
        JBI->Add("event", event_ctr);
        JBI->Add("value", D_OFF);
        JBI->Add("duration", screen_press.tSavedButtonONDurationEvent);
      JBI->End();

      if(!fEnableIgnoreNextOffEvent){
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = NOT set"));
        pCONT_mqtt->Publish("status/nextion/event",JBI->GetBufferPtr(),0);
        pCONT_mqtt->Publish("status/nextion/event/end",JBI->GetBufferPtr(),0);
        MQTTSend_PressEvent();
      }else{
        fEnableIgnoreNextOffEvent = false;// reset to listen to next event
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = reset"));
      }


      // Now see if this object has a .val that might have been updated.  Works for sliders,
      // two-state buttons, etc, throws a 0x1A error for normal buttons which we'll catch and ignore
      // mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // mqttGetSubtopicJSON = "p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
    }
  }
  else if (nextionReturnBuffer[0] == INSTRUCTION_SET_RETURN_CODE__CURRENT_PAGE_NUMBER)
  { 
  DEBUG_LINE_HERE;
  // Handle incoming "sendme" page number
    // 0x66+PageNum+End
    // Example: 0x66 0x02 0xFF 0xFF 0xFF
    // Meaning: page 2
    String nextionPage = String(nextionReturnBuffer[1]);
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "[sendme Page] \"%s\""),nextionPage.c_str());

    // if ((settings.page != nextionPage.toInt()) && ((nextionPage != "0") || nextionReportPage0))
    // { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )
    //   settings.page = nextionPage.toInt();
    //   String mqttPageTopic = mqttStateTopic + "/page";      
    //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: mqttPageTopic=\"%s\" nextionPage=\"%s\""),mqttPageTopic.c_str(),nextionPage.c_str());
    //   //mqttClient.publish(mqttPageTopic, nextionPage);
    //   pCONT_mqtt->Publish("status/nextion/event4",nextionPage.c_str(),0);
    // }

  }
  else if (nextionReturnBuffer[0] == 0x67)
  { 
  DEBUG_LINE_HERE;
  // Handle touch coordinate data
    // 0X67+Coordinate X High+Coordinate X Low+Coordinate Y High+Coordinate Y Low+TouchEvent+End
    // Example: 0X67 0X00 0X7A 0X00 0X1E 0X01 0XFF 0XFF 0XFF
    // Meaning: Coordinate (122,30), Touch Event: Press
    // issue Nextion command "sendxy=1" to enable this output
    uint16_t xCoord = nextionReturnBuffer[1];
    xCoord = xCoord * 256 + nextionReturnBuffer[2];
    uint16_t yCoord = nextionReturnBuffer[3];
    yCoord = yCoord * 256 + nextionReturnBuffer[4];
    String xyCoord = String(xCoord) + ',' + String(yCoord);
    byte nextionTouchAction = nextionReturnBuffer[5];
    if (nextionTouchAction == 0x01)
    {  
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch ON] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOn";
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->Publish("status/nextion/xyCoord",xyCoord.c_str(),0);
    }
    else if (nextionTouchAction == 0x00)
    {
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch OFF] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOff";
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->Publish("status/nextion/event6",xyCoord.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x68)
  { 
    ALOG_ERR(PSTR("Touch coordinate data not enabled"));
  }
  else if (nextionReturnBuffer[0] == 0x70)
  { 
  DEBUG_LINE_HERE;
  // Handle get string return
    // 0x70+ASCII string+End
    // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
    // Meaning: String data, ABCD1234
    String getString;
    for (int i = 1; i < nextionReturnIndex - 3; i++)
    { // convert the payload into a string
      getString += (char)nextionReturnBuffer[i];
    }
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [String Return] '%s'"),getString.c_str());
  
    if (mqttGetSubtopic == "")
    { // If there's no outstanding request for a value, publish to mqttStateTopic
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttStateTopic.c_str(),getString.c_str());
      pCONT_mqtt->Publish("status/nextion/getString",getString.c_str(),0);
    }
    else
    { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttReturnTopic.c_str(),getString.c_str());
      pCONT_mqtt->Publish("status/nextion/getString",getString.c_str(),0);
      mqttGetSubtopic = "";
    }
  }
  else if (nextionReturnBuffer[0] == 0x71)
  { 
  DEBUG_LINE_HERE;
  // Handle get int return
    // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
    // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
    // Meaning: Integer data, 123
    unsigned long getInt = nextionReturnBuffer[4];
    getInt = getInt * 256 + nextionReturnBuffer[3];
    getInt = getInt * 256 + nextionReturnBuffer[2];
    getInt = getInt * 256 + nextionReturnBuffer[1];
    String getString = String(getInt);
    //AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: [Int Return] '")) + getString + "'");

    if (lcdVersionQueryFlag)
    {
      lcdVersion = getInt;
      lcdVersionQueryFlag = false;
      ////AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: lcdVersion '")) + String(lcdVersion) + "'");
    }
    else if (mqttGetSubtopic == "")
    {
  DEBUG_LINE_HERE;
      //mqttClient.publish(mqttStateTopic, getString);
      Serial.println(getString);
      pCONT_mqtt->Publish("status/nextion/event9",getString.c_str(),0);
    }
    // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
    else
    {
  DEBUG_LINE_HERE;
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
      //mqttClient.publish(mqttReturnTopic, getString);
      pCONT_mqtt->Publish("status/nextion/event10",getString.c_str(),0);
      String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopicJSON + String(F("\", \"value\":")) + getString + String(F("}"));
      //mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      pCONT_mqtt->Publish("status/nextion/event11",mqttButtonJSONEvent.c_str(),0);
      mqttGetSubtopic = "";
    }
  DEBUG_LINE_HERE;

  }
  else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
  { 
  DEBUG_LINE_HERE;
  // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol

    ALOG_INF(PSTR("Catch 'comok' response to 'connect' command \"%s\""), nextionReturnBuffer);

    String comokField;
    uint8_t comokFieldCount = 0;
    byte comokFieldSeperator = 0x2c; // ","

    for (uint8_t i = 0; i <= nextionReturnIndex; i++)
    { // cycle through each byte looking for our field seperator
      if (nextionReturnBuffer[i] == comokFieldSeperator)
      { // Found the end of a field, so do something with it.  Maybe.
        if (comokFieldCount == 2)
        {
          nextionModel = comokField;
          // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: nextionModel: ")) + nextionModel);
        }
        comokFieldCount++;
        comokField = "";
      }
      else
      {
        comokField += String(char(nextionReturnBuffer[i]));
      }
    }
  }
  
  
//   else if (nextionReturnBuffer[0] == 0x04)
//   { // Invalid Picture ID
//     debugPrintln(String(F("HMI IN: [Invalid Picture ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Picture ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x05)
//   { // Invalid Font ID
//     debugPrintln(String(F("HMI IN: [Invalid Font ID	] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Font ID	\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x06)
//   { // Invalid File Operation
//     debugPrintln(String(F("HMI IN: [Invalid File Operation] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid File Operation\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x09)
//   { // Invalid CRC
//     debugPrintln(String(F("HMI IN: [Invalid CRC] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid CRC\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x11)
//   { // Invalid Baud rate Setting
//     debugPrintln(String(F("HMI IN: [Invalid Baud rate Setting] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Baud rate Setting\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x12)
//   { // Invalid Waveform ID or Channel #
//     debugPrintln(String(F("HMI IN: [Invalid Waveform ID or Channel #] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Waveform ID or Channel #\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1A)
//   { // Invalid Variable name or attribute
//     debugPrintln(String(F("HMI IN: [Invalid Variable name or attribute] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Variable name or attribute\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1B)
//   { // Invalid Variable Operation
//     debugPrintln(String(F("HMI IN: [Invalid Variable Operation] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Variable Operation\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1C)
//   { // Assignment failed to assign
//     debugPrintln(String(F("HMI IN: [Assignment failed to assign] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Assignment failed to assign\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1D)
//   { // EEPROM Operation failed
//     debugPrintln(String(F("HMI IN: [EEPROM Operation failed] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"EEPROM Operation failed\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1E)
//   { // Invalid Quantity of Parameters
//     debugPrintln(String(F("HMI IN: [Invalid Quantity of Parameters] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Quantity of Parameters\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1F)
//   { // IO Operation failed
//     debugPrintln(String(F("HMI IN: [IO Operation failed] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"IO Operation failed\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x20)
//   { // Escape Character Invalid
//     debugPrintln(String(F("HMI IN: [Escape Character Invalid] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Escape Character Invalid\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x23)
//   { // Variable name too long
//     debugPrintln(String(F("HMI IN: [Variable name too long] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Variable name too long\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x24)
//   { // Serial Buffer Overflow
//     debugPrintln(String(F("HMI IN: [Serial Buffer Overflow] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Serial Buffer Overflow\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }

//   else if (nextionReturnBuffer[0] == 0x65)
//   { // Handle incoming touch command
//     // 0x65+Page ID+Component ID+TouchEvent+End
//     // Return this data when the touch event created by the user is pressed.
//     // Definition of TouchEvent: Press Event 0x01, Release Event 0X00
//     // Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
//     // Meaning: Touch Event, Page 0, Object 2, Press
//     String nextionPage = String(nextionReturnBuffer[1]);
//     String nextionButtonID = String(nextionReturnBuffer[2]);
//     byte nextionButtonAction = nextionReturnBuffer[3];

//     if (nextionButtonAction == 0x01)
//     {
//       debugPrintln(String(F("HMI IN: [Button ON] 'p[")) + nextionPage + "].b[" + nextionButtonID + "]'");
//       if (mqttClient.connected())
//       {
//         // Only process touch events if screen backlight is on and configured to do so.
//         if (ignoreTouchWhenOff && !lcdBacklightOn)
//         {
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_press_disabled\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//         else
//         {
//           String mqttButtonTopic = mqttStateTopic + "/p[" + nextionPage + "].b[" + nextionButtonID + "]";
//           mqttClient.publish(mqttButtonTopic, "ON");
//           debugPrintln(String(F("MQTT OUT: '")) + mqttButtonTopic + "' : 'ON'");
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//       }
//       if (beepEnabled)
//       {
//         beepOnTime = 500;
//         beepOffTime = 100;
//         beepCounter = 1;
//       }
//       if (rebootOnp0b1 && (nextionPage == "0") && (nextionButtonID == "1"))
//       {
//         debugPrintln(String(F("HMI IN: p[0].b[1] pressed during HASPone configuration, rebooting.")));
//         pCONT_wif->EspRestart();
//       }
//     }
//     else if (nextionButtonAction == 0x00)
//     {
//       debugPrintln(String(F("HMI IN: [Button OFF] 'p[")) + nextionPage + "].b[" + nextionButtonID + "]'");
//       if (mqttClient.connected())
//       {
//         // Only process touch events if screen backlight is on and configured to do so.
//         if (ignoreTouchWhenOff && !lcdBacklightOn)
//         {
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_release_disabled\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//         else
//         {
//           String mqttButtonTopic = mqttStateTopic + "/p[" + nextionPage + "].b[" + nextionButtonID + "]";
//           mqttClient.publish(mqttButtonTopic, "OFF");
//           debugPrintln(String(F("MQTT OUT: '")) + mqttButtonTopic + "' : 'OFF'");
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_release\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"OFF\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//           // Now see if this object has a .val that might have been updated.  Works for sliders,
//           // two-state buttons, etc, returns 0 for normal buttons
//           mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
//           // This right here is dicey.  We're done w/ this command so reset the index allowing this to be kinda-reentrant
//           // because the call to nextionGetAttr is going to call us back.
//           nextionReturnIndex = 0;
//           nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
//         }
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x66)
//   { // Handle incoming "sendme" page number
//     // 0x66+PageNum+End
//     // Example: 0x66 0x02 0xFF 0xFF 0xFF
//     // Meaning: page 2
//     String nextionPage = String(nextionReturnBuffer[1]);
//     debugPrintln(String(F("HMI IN: [sendme Page] '")) + nextionPage + String(F("'")));
//     if ((nextionPage != "0") || nextionReportPage0)
//     { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )

//       if (mqttClient.connected())
//       {
//         String mqttButtonJSONEvent = String(F("{\"event\":\"page\",\"value\":")) + nextionPage + String(F("}"));
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         String mqttPageTopic = mqttStateTopic + "/page";
//         debugPrintln(String(F("MQTT OUT: '")) + mqttPageTopic + String(F("' : '")) + nextionPage + String(F("'")));
//         mqttClient.publish(mqttPageTopic, nextionPage, false, 0);
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x67 || nextionReturnBuffer[0] == 0x68)
//   { // Handle touch coordinate data
//     // 0X67+Coordinate X High+Coordinate X Low+Coordinate Y High+Coordinate Y Low+TouchEvent+End
//     // Example: 0X67 0X00 0X7A 0X00 0X1E 0X01 0XFF 0XFF 0XFF
//     // Meaning: Coordinate (122,30), Touch Event: Press
//     // issue Nextion command "sendxy=1" to enable this output
//     // 0x68 is the same, but returned when the screen touch has awakened the screen from sleep
//     uint16_t xCoord = nextionReturnBuffer[1];
//     xCoord = xCoord * 256 + nextionReturnBuffer[2];
//     uint16_t yCoord = nextionReturnBuffer[3];
//     yCoord = yCoord * 256 + nextionReturnBuffer[4];
//     String xyCoord = String(xCoord) + String(',') + String(yCoord);
//     byte nextionTouchAction = nextionReturnBuffer[5];
//     if (nextionTouchAction == 0x01)
//     {
//       debugPrintln(String(F("HMI IN: [Touch ON] '")) + xyCoord + String(F("'")));
//       if (mqttClient.connected())
//       {
//         String mqttTouchTopic = mqttStateTopic + "/touchOn";
//         mqttClient.publish(mqttTouchTopic, xyCoord);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttTouchTopic + String(F("' : '")) + xyCoord + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"touchxy\",\"touch_event\":\"ON\",\"touchx\":\"")) + String(xCoord) + String(F("\",\"touchy\":\"")) + String(yCoord) + String(F("\",\"screen_state\":\""));
//         if (nextionReturnBuffer[0] == 0x67)
//         {
//           mqttButtonJSONEvent += "awake\"}";
//         }
//         else
//         {
//           mqttButtonJSONEvent += "asleep\"}";
//         }
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//     }
//     else if (nextionTouchAction == 0x00)
//     {
//       debugPrintln(String(F("HMI IN: [Touch OFF] '")) + xyCoord + String(F("'")));
//       if (mqttClient.connected())
//       {
//         String mqttTouchTopic = mqttStateTopic + "/touchOff";
//         mqttClient.publish(mqttTouchTopic, xyCoord);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttTouchTopic + String(F("' : '")) + xyCoord + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"touchxy\",\"touch_event\":\"OFF\",\"touchx\":\"")) + String(xCoord) + String(F("\",\"touchy\":\"")) + String(yCoord) + String(F("\",\"screen_state\":\""));
//         if (nextionReturnBuffer[0] == 0x67)
//         {
//           mqttButtonJSONEvent += "awake\"}";
//         }
//         else
//         {
//           mqttButtonJSONEvent += "asleep\"}";
//         }
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x70)
//   { // Handle get string return
//     // 0x70+ASCII string+End
//     // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
//     // Meaning: String data, ABCD1234
//     String getString;
//     for (int i = 1; i < nextionReturnIndex - 3; i++)
//     { // convert the payload into a string
//       getString += (char)nextionReturnBuffer[i];
//     }
//     debugPrintln(String(F("HMI IN: [String Return] '")) + getString + String(F("'")));
//     if (mqttClient.connected())
//     {
//       if (mqttGetSubtopic == "")
//       { // If there's no outstanding request for a value, publish to mqttStateTopic
//         mqttClient.publish(mqttStateTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateTopic + String(F("' : '")) + getString + String(F("'")));
//       }
//       else
//       { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
//         String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
//         mqttClient.publish(mqttReturnTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttReturnTopic + String(F("' : '")) + getString + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopic.substring(1) + String(F("\",\"value\":\"")) + getString + String(F("\"}"));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         mqttGetSubtopic = "";
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x71)
//   { // Handle get int return
//     // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
//     // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
//     // Meaning: Integer data, 123
//     long getInt = nextionReturnBuffer[4];
//     getInt = getInt * 256 + nextionReturnBuffer[3];
//     getInt = getInt * 256 + nextionReturnBuffer[2];
//     getInt = getInt * 256 + nextionReturnBuffer[1];
//     String getString = String(getInt);
//     debugPrintln(String(F("HMI IN: [Int Return] '")) + getString + String(F("'")));

//     if (lcdVersionQueryFlag)
//     {
//       lcdVersion = getInt;
//       lcdVersionQueryFlag = false;
//       debugPrintln(String(F("HMI IN: lcdVersion '")) + String(lcdVersion) + String(F("'")));
//     }
//     else if (lcdBacklightQueryFlag)
//     {
//       lcdBacklightDim = getInt;
//       lcdBacklightQueryFlag = false;
//       if (lcdBacklightDim > 0)
//       {
//         lcdBacklightOn = 1;
//       }
//       else
//       {
//         lcdBacklightOn = 0;
//       }
//       debugPrintln(String(F("HMI IN: lcdBacklightDim '")) + String(lcdBacklightDim) + String(F("'")));
//     }
//     else if (mqttGetSubtopic == "")
//     {
//       if (mqttClient.connected())
//       {
//         mqttClient.publish(mqttStateTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateTopic + String(F("' : '")) + getString + String(F("'")));
//       }
//     }
//     // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
//     else
//     {
//       if (mqttClient.connected())
//       {
//         String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
//         mqttClient.publish(mqttReturnTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttReturnTopic + String(F("' : '")) + getString + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopic.substring(1) + String(F("\",\"value\":")) + getString + String(F("}"));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//       mqttGetSubtopic = "";
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
//   { // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol
//     String comokField;
//     uint8_t comokFieldCount = 0;
//     byte comokFieldSeperator = 0x2c; // ","

//     for (uint8_t i = 0; i <= nextionReturnIndex; i++)
//     { // cycle through each byte looking for our field seperator
//       if (nextionReturnBuffer[i] == comokFieldSeperator)
//       { // Found the end of a field, so do something with it.  Maybe.
//         if (comokFieldCount == 2)
//         {
//           nextionModel = comokField;
//           debugPrintln(String(F("HMI IN: nextionModel: ")) + nextionModel);
//         }
//         comokFieldCount++;
//         comokField = "";
//       }
//       else
//       {
//         comokField += String(char(nextionReturnBuffer[i]));
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x86)
//   { // Returned when Nextion enters sleep automatically. Using sleep=1 will not return an 0x86
//     // 0x86+End
//     if (mqttClient.connected())
//     {
//       lcdBacklightOn = 0;
//       mqttClient.publish(mqttLightStateTopic, "OFF", true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"sleep\",\"value\":\"ON\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x87)
//   { // Returned when Nextion leaves sleep automatically. Using sleep=0 will not return an 0x87
//     // 0x87+End
//     if (mqttClient.connected())
//     {
//       lcdBacklightOn = 1;
//       mqttClient.publish(mqttLightStateTopic, "ON", true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//       mqttClient.publish(mqttLightBrightStateTopic, String(lcdBacklightDim), true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : ")) + String(lcdBacklightDim));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"sleep\",\"value\":\"OFF\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x88)
//   { // Returned when Nextion powers on
//     // 0x88+End
//     ALOG_INF(PSTR("HMI: Nextion panel connected."));
//   }

  // else if (nextionReturnBuffer[0] == 0x1A)
  // { // Catch 0x1A error, possibly from .val query against things that might not support that request
  //   // 0x1A+End
  //   // ERROR: Variable name invalid
  //   // We'll be triggering this a lot due to requesting .val on every component that sends us a Touch Off
  //   // Just reset mqttGetSubtopic and move on with life.
  //   mqttGetSubtopic = "";
  // }
  nextionReturnIndex = 0; // Done handling the buffer, reset index back to 0
}


/******************************************************************************************************************
 * SECTION: Helper Functions
*******************************************************************************************************************/


/**
 * @brief This function going forward should only send the command if the page is not already set, so will need a way to poll this
 * 
 * @param page 
 */
void mNextionPanel::Command_SetPage(uint8_t page){
  settings.page = page;
  char ctr[10];
  sprintf(ctr,"page %d",page);
  nextionSendCmd(ctr);
}
void mNextionPanel::Command_SetPage(char* pagename)
{
  nextionSendCmd(pagename);
}

void mNextionPanel::Command_SetPageIfChanged(uint8_t page){
  if(settings.page == page) {return;}
  Command_SetPage(page);
}


// void mNextionPanel::Command_SetPageIfChanged(char* page){
//   if(settings.page == page) {return;}
//   Command_SetPage(page);
// }
// void mNextionPanel::Command_SetPage(char* page){
//   settings.page = page;
//   nextionSendCmd(page);
// }


void mNextionPanel::Command_SetBrightness(uint8_t brightness_percentage){
  char ctr[10];
  sprintf(ctr,"dims=%d",brightness_percentage); //dims range 0-100
  nextionSendCmd(ctr);
  settings.brightness_percentage = brightness_percentage;
}

void mNextionPanel::Command_SetBrightness255(uint8_t brightness){
  settings.brightness_percentage = map(brightness,0,255,0,100);
  Command_SetBrightness(settings.brightness_percentage);
}

void mNextionPanel::Command_ToggleBrightness(){  
  Command_SetBrightness(settings.brightness_percentage>0?0:settings.brightness_percentage);
}


uint16_t mNextionPanel::RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b){
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16_t mNextionPanel::RGB888_to_RGB565(uint32_t rgb_888) // W,R,G,B
{
  uint8_t r = (rgb_888 >> 16) & 0xFF;
  uint8_t g = (rgb_888 >> 8) & 0xFF;
  uint8_t b = rgb_888 & 0xFF;
  uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  // ALOG_INF(PSTR("%08X r,g,b = %d,%d,%d --> %d"),rgb_888, r,g,b, rgb565);
  return rgb565;
}



void mNextionPanel::espWifiSetup()
{ // Connect to WiFi
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"WiFi Connecting\"");
}


// UTF8-Decoder: convert UTF8-String to extended ASCII http://playground.arduino.cc/main/Utf8ascii
// Workaround for issue here: https://github.com/home-assistant/home-assistant/issues/9528
// Nextion claims that "Unicode and UTF will not be among the supported encodings", so this should
// be safe to run against all attribute values coming in.
byte mNextionPanel::utf8ascii(byte ascii)
{ 

  static byte c1; // Last character buffer
  // Convert a single Character from UTF8 to Extended ASCII. Return "0" if a byte has to be ignored.
  if (ascii < 128)
  { // Standard ASCII-set 0..0x7F handling
    c1 = 0;
    return (ascii);
  }
  // get previous input
  byte last = c1; // get last char
  c1 = ascii;     // remember actual character
  switch (last)
  { // conversion depending on first UTF8-character
  case 0xC2:
    return (ascii);
    break;
  case 0xC3:
    return (ascii | 0xC0);
    break;
  case 0x82:
    if (ascii == 0xAC)
      return (0x80); // special case Euro-symbol
  }
  return (0); // otherwise: return zero, if character has to be ignored
}

String mNextionPanel::utf8ascii(String s)
{ // convert String object from UTF8 String to Extended ASCII
  String r = "";
  char c;
  for (uint16_t i = 0; i < s.length(); i++)
  {
    c = utf8ascii(s.charAt(i));
    if (c != 0)
      r += c;
  }
  return r;
}


void mNextionPanel::EverySecond_FlashScreen(){

  if(flash_message.cShowSeconds==0){
    // Return screen to previous
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION D_COMMAND_NVALUE),"settings.page_saved",settings.page_saved);
    Command_SetPage(settings.page_saved);
    flash_message.cShowSeconds = -1;
  }else
  if(flash_message.cShowSeconds>0){
    flash_message.cShowSeconds--;
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION D_COMMAND_NVALUE),"flash_message.cShowSeconds",flash_message.cShowSeconds);
  }

} //end F

// https://seithan.com/Easy-Nextion-Library/Use-Nextion-General-View/
void mNextionPanel::EverySecond_SendScreenInfo(){

  char rtc_ctr[40]; memset(rtc_ctr,'\0',sizeof(rtc_ctr));
  sprintf(rtc_ctr, "%02d:%02d:%02d\n\r",
  pCONT_time->RtcTime.hour,pCONT_time->RtcTime.minute,pCONT_time->RtcTime.second);

  SetAttribute_Txt(settings.page,1,rtc_ctr);
  
  // char health_ctr[40]; memset(health_ctr,'\0',sizeof(health_ctr));
  // sprintf(health_ctr, "%c%c%c %d",
  //   WiFi.status() == WL_CONNECTED ? 'N' : 'n',
  //   pCONT_mqtt->pubsub->connected() ? 'M' : 'm',
  //   fOpenHABDataStreamActive ? 'O' : 'o',
  //   0
  //   // pCONT_wif->WifiGetRssiAsQuality(WiFi.RSSI())
  // );

  // Serial.println(health_ctr);

  // SetAttribute_Txt(settings.page,5,health_ctr);

}



/******************************************************************************************************************
 *SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mNextionPanel::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    JBI->Add("test", 1);
    JBI->Add("lcdConnected", lcdConnected);
  JBI->End();
}


uint8_t mNextionPanel::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  
  JBI->Start();
    JBI->Add("test", 1);
  JBI->End();

}


/******************************************************************************************************************
 * SECTION: MQTT
*******************************************************************************************************************/

void mNextionPanel::mqttConnected()
{ // MQTT connection and subscriptions
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_LOG "mNextionPanel::mqttConnected"));
}


void mNextionPanel::mqttDisconnected(){
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected"));
  
}


void mNextionPanel::MQTTSend_PressEvent(){

  if(!mTime::TimeReached(&tSaved_MQTTSend_PressEvent,200)){
    // Debounce and only send once per event (ie ignore release trigger following immediate trigger)
    return;
  }
  char objname_buffer[50];
  
  // screen_press.page = nextionReturnBuffer[1];
  // screen_press.event = nextionReturnBuffer[2];
  uint32_t tSavedTimeSincePressOn = abs(millis() - screen_press.tSavedButtonONEvent);

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s elapsed=%d of %dthreshold"), screen_press.page, screen_press.event, (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS", tSavedTimeSincePressOn, LONG_PRESS_DURATION);
  
  JBI->Start();  
    JBI->Add("Page", screen_press.page);
    JBI->Add("ID", screen_press.event);

    char event_ctr[20]; memset(event_ctr,0,sizeof(event_ctr));
    sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
    JBI->Add("Event", event_ctr);
    
    JBI->Add("PressType", (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    JBI->Add("Duration", tSavedTimeSincePressOn);
    JBI->Add("ObjectName", GetObjectName_FromID(screen_press.event, objname_buffer, sizeof(objname_buffer)));
  JBI->End();

  tSaved_MQTTSend_PressEvent = millis();

  pCONT_mqtt->Publish("status/nextion/event/press",JBI->GetBufferPtr(),false);

}

// Adding translation layer?? IDs to names?
/**
 * @brief 
 *
 * objname->id
 *  
 * IDlist:{
 *  "TimeUS":6,
 *  "TimeDS":7
 * 
 * }
 * 
 */


void mNextionPanel::MQTTSend_LongPressEvent(){

  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];

  char objname_buffer[50];

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"), screen_press.page, screen_press.event, "LONG_PRESS");

  char event_ctr[20];
  memset(event_ctr,0,sizeof(event_ctr));
  sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);

  JBI->Start();
    JBI->Add("Page", screen_press.page);
    JBI->Add("ID", screen_press.event);
    JBI->Add("Event", event_ctr);
    JBI->Add("PressType", "LONG_PRESS");
    JBI->Add("DurationThreshold", LONG_PRESS_DURATION);
    JBI->Add("ObjectName", GetObjectName_FromID(screen_press.event, objname_buffer, sizeof(objname_buffer)));
  JBI->End();

  pCONT_mqtt->Publish("status/nextion/event",JBI->GetBufferPtr(),0);
  pCONT_mqtt->Publish("status/nextion/event/start",JBI->GetBufferPtr(),0);

}


const char*  mNextionPanel::GetObjectName_FromID(uint8_t id, char* objname, uint8_t objname_size)
{
  

  uint16_t progmem_size = sizeof(DISPLAY_TEMPLATE);
  char buffer[progmem_size];
  memcpy_P(buffer,DISPLAY_TEMPLATE,sizeof(DISPLAY_TEMPLATE));

  JsonParser parser(buffer);
  JsonParserObject rootObj = parser.getRootObject();   
  if (!rootObj) 
  {
    ALOG_ERR(PSTR("DeserializationError with \"%s\""), buffer);
    return PM_SEARCH_NOMATCH;
  } 
  else
  {
    // ALOG_DBG(PSTR("Deserialization Success with \"%s\""), buffer);
  }
  
  JsonParserToken jtok = 0;
  JsonParserObject obj1 = 0;
  char item_name[100] = {0};

  // ALOG_INF(PSTR("ObjectNameID size %d"), rootObj["ObjectNameID"].size());

  if(jtok = rootObj["ObjectNameID"])
  {

    uint8_t jsonpair_count = jtok.size();

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){

      jtok.nextOne(); // Skip over "ObjectNameID" to first key
      const char* key = jtok.getStr();
      // ALOG_INF(PSTR("ObjectNameID key %s"), key);
      jtok.nextOne(); // Skip to value
      const char* value = jtok.getStr();
      // ALOG_INF(PSTR("ObjectNameID value %s"), value);

      if(jtok.getInt() == id)
      {
        snprintf(objname, objname_size, "%s", key);
        ALOG_INF(PSTR("ObjectNameID objname %s"), objname);
        return objname;
      }

    }
  }

  return PM_SEARCH_NOMATCH;
}



#ifdef USE_MODULE_NETWORK_MQTT

void mNextionPanel::MQTTHandler_Init(){

  struct handler<mNextionPanel>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mNextionPanel::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mNextionPanel::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mNextionPanel::MQTTHandler_Rate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  // }
}

/**
 * @brief Check all handlers if they require action
 * */
void mNextionPanel::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}
#endif// USE_MODULE_NETWORK_MQTT


/******************************************************************************************************************
 * SECTION: WebServer
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_WEBSERVER

void mNextionPanel::WebPage_AddHandlers()
{
    
  pCONT_web->server->on("/nextion_update", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->WebPage_LCD_Update_TFT(request);
  });

  pCONT_web->server->on("/firmware", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleFirmware(request);
  });
          
  pCONT_web->server->on("/tftFileSize", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleTftFileSize(request);
  });

  pCONT_web->server->on(
                        "/lcdupload", 
                        HTTP_POST, 
                        [this](AsyncWebServerRequest *request){ request->send(200); }, // On Completion
                        [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
                          this->webHandleLcdUpload(request, filename, index, data, len, final);
                        }
                      );

  #ifdef ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL
  pCONT_web->server->on("/lcddownload", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleLcdDownload(request);
  });
  #endif // ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL

  pCONT_web->server->on("/lcdOtaSuccess", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleLcdUpdateSuccess(request);
  });

  pCONT_web->server->on("/lcdOtaFailure", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->webHandleLcdUpdateFailure(request);
  });

}


void mNextionPanel::webHandleFirmware(AsyncWebServerRequest *request)
{ // http://plate01/firmware
 
  String data = "";

  // ALOG_INF(PSTR(D_LOG_NEXTION "HTTP: Sending /firmware page to client connected from: %s"), webServer->client().remoteIP().toString());
  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", "HASPone " + String(pCONT_set->Settings.system_name.friendly) + " Firmware updates");
  
  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);
  data += FPSTR(HTTP_STYLE3);
  data += FPSTR(HASP_STYLE);
  data += FPSTR(HTTP_HEAD_END3);

  data += F("<h1>");
  data += String(pCONT_set->Settings.system_name.friendly);
  data += F("</h1>");
  data += F("<hr><h1>");
  data += F("Nextion Firmware Update</h1>");

  /**
   * @brief Update LCD via URL
   **/
  #ifdef ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL
  data += F("<form method='get' action='lcddownload'>");  
  data += F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>");
  data += F("<br/><input id='lcdFirmware' name='lcdFirmware' value='");
  data += "lcdFirmwareUrl";
  data += F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>");
  #endif // ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL

  /**
   * @brief Update LCD via tft file upload
   **/
  data += F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>");
  data += F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>");
  data += F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>");

  // Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
  data += F("<script>function handleLcdFileSelect(evt) {");
  data += F("var uploadFile = evt.target.files[0];");
  data += F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;");
  data += F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;");
  data += F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}");
  data += F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}");
  data += F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}");
  data += F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}");
  data += F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);");
  data += F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>");

  data += FPSTR(HTTP_END3);
  
  request->send(200, "text/html", data);

}


/**
 * @brief 
 * 
 */
void mNextionPanel::webHandleLcdUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{ 

  if(update_in_progress == false) // Not previously set, and hence upload
  {
    update_in_progress = true;
    ALOG_INF(PSTR("LCDOTA: LCD update started, update_in_progress SET, short delay for async"));
    delay(100);
  }

  ALOG_INF(PSTR(D_LOG_NEXTION "Upload i=%d|len=%d|rem=%d|f=%d|tx=%d"), index, len, tftFileSize-len, final, transmitted_bytes);

  static uint32_t lcdOtaTransferred = 0;
  static uint32_t lcdOtaRemaining;
  static uint16_t lcdOtaParts;
  const uint32_t lcdOtaTimeout = 120000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for upload timeout

  bool success = false;

  if (tftFileSize == 0)
  {

    String data2 = "";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: FAILED, no filesize sent."));
    String httpHeader = FPSTR(HTTP_HEAD_START);
    httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD update error");

    data2 += httpHeader;
    data2 += FPSTR(HTTP_SCRIPT3);
    data2 += FPSTR(HTTP_STYLE3);
    data2 += FPSTR(HASP_STYLE);
    data2 += (F("<meta http-equiv='refresh' content='5;url=/firmware' />"));
    data2 += FPSTR(HTTP_HEAD_END3);

    data2 += (F("<h1>"));
    data2 += String(pCONT_set->Settings.system_name.friendly);
    data2 += (F(" LCD update FAILED</h1>"));
    data2 += (F("No update file size reported. You must use a modern browser with Javascript enabled."));
    
    data2 += FPSTR(HTTP_END3);
  
    request->send(200, "text/html", data2);

    return;
  }
  
   
  // if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
  // { // Our timer expired so reset
  //   ALOG_INF(PSTR("LCDOTA: ERROR: LCD upload timeout.  Restarting."));
  //   pCONT_wif->EspRestart();
  // }
  // else if (upload.status == UPLOAD_FILE_START)
  


  if(index == 0) // First byte of file, initialise the start of the upload
  {

    Serial.println("UPLOAD_FILE_START");  
    // ALOG_INF(PSTR("(upload.status == UPLOAD_FILE_START)"));
    // logmessage = "Upload Start: " + String(filename);    
    // Serial.println(logmessage);
    // ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Attempting firmware upload"));
    // ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: TFTfileSize: %d"), tftFileSize);

    // WiFiUDP::stopAll(); // Stop UDP to prevent any issues with OTA

    lcdOtaRemaining = tftFileSize;
    lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File upload beginning. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);

    transmitted_bytes = 0;

    // display->begin(115200); delay(200);
    // while (display->available() > 0) { display->read(); } // Clear out any old data
    
    display->write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command to LCD
    display->flush();
    nextionHandleInput();

    String lcdOtaNextionCmd = "whmi-wri " + String(tftFileSize) + "," + String(ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT) + ",0";
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd.c_str());
    display->print(lcdOtaNextionCmd);
    display->write(nextionSuffix, sizeof(nextionSuffix));
    display->flush();
    
    ALOG_INF(PSTR("LCDOTA: nextionOtaResponse------------------?"));

    if (nextionOtaResponse())
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command accepted"));
    }
    else
    {
      ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED."));

      delay(5000);
      pCONT_wif->EspRestart();
    }

    lcdOtaTimer = millis();
  }
  
  /**
   * @brief UPLOAD_FILE_WRITE
   **/
  if ( index >= 0)
  { 
    
    static int lcdOtaChunkCounter = 0;
    static uint16_t lcdOtaPartNum = 0;
    static int lcdOtaPercentComplete = 0;
    static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
    static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};
    uint16_t lcdOtaUploadIndex = 0;

    int32_t lcdOtaPacketRemaining = len;//upload.currentSize;

    // Serial.printf("UPLOAD: Receiving: '%s'\n\r", filename.c_str());
    // Serial.printf( "%i bytes received.\n\r", index );
    // const char* FILESIZE_HEADER{"FileSize"};
    // Serial.printf("UPLOAD: fileSize: %s\n\r", request->header(FILESIZE_HEADER));
    // Serial.printf("UPLOAD: lcdOtaPacketRemaining: %d\n\r", lcdOtaPacketRemaining);


    while (lcdOtaPacketRemaining > 0)
    { 

      // DEBUG_LINE_HERE;
      
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif
                
      // Write incoming data to panel as it arrives
      // determine chunk size as lowest value of lcdOtaPacketRemaining, lcdOtaBufferSize, or 4096 - lcdOtaChunkCounter
      uint16_t lcdOtaChunkSize = 0;
      if ((lcdOtaPacketRemaining <= lcdOtaBufferSize) && (lcdOtaPacketRemaining <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaPacketRemaining;
      }
      else 
      if ((lcdOtaBufferSize <= lcdOtaPacketRemaining) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
      {
        lcdOtaChunkSize = lcdOtaBufferSize;
      }
      else
      {
        lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
      }
      
      for (uint16_t i = 0; i < lcdOtaChunkSize; i++)
      { // Load up the UART buffer
        lcdOtaBuffer[i] = data[lcdOtaUploadIndex];
        // ALOG_INF(PSTR("lcdOtaBuffer[%d|%d] = [%d]%d   %d"), i, lcdOtaChunkSize, lcdOtaUploadIndex, lcdOtaBuffer[i], data[lcdOtaUploadIndex]);
        lcdOtaUploadIndex++;
        transmitted_bytes++;
      }
      display->flush();                              // Clear out current UART buffer
      display->write(lcdOtaBuffer, lcdOtaChunkSize); // And send the most recent data

      // ALOG_INF(PSTR("Sending lcdOtaBuffer %d\t\ttransmitted_bytes=%d"), lcdOtaBuffer[0], transmitted_bytes);

      // AddLog_Array5(LOG_LEVEL_INFO, PSTR("lcdOtaBuffer"), lcdOtaBuffer, 10);

      lcdOtaChunkCounter += lcdOtaChunkSize;
      lcdOtaTransferred += lcdOtaChunkSize;
      
      if (lcdOtaChunkCounter >= 4096) // Everytime we exceed the max buffer, check the status of the display
      {
        // DEBUG_LINE_HERE;
        display->flush();
        lcdOtaPartNum++;
        lcdOtaPercentComplete = (lcdOtaTransferred * 100) / tftFileSize;
        lcdOtaChunkCounter = 0;
        if (nextionOtaResponse())
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d OK (%d%%|%d)"), lcdOtaPartNum, lcdOtaPercentComplete, lcdOtaTransferred);
        }
        else
        {
          ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d FAILED, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
        }
      }
      else
      {
        delay(10);
      }
      
      if(lcdOtaRemaining > 0)
      {
        lcdOtaRemaining -= lcdOtaChunkSize;
      }

      if(lcdOtaPacketRemaining > 0)
      {
        lcdOtaPacketRemaining -= lcdOtaChunkSize;
      }

      // if(lcdOtaPacketRemaining==0){
      //   Serial.println("About to leave while, did I want to?");
      // }

    }


    if (lcdOtaTransferred >= tftFileSize)
    {
      ALOG_INF(PSTR("(lcdOtaTransferred >= tftFileSize) \t Total bytes should have been sent, checking completion?"));
      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        request->redirect("/lcdOtaSuccess"); 
        // delay(10);
        // pCONT_wif->EspRestart();
        success = true;
      ALOG_INF(PSTR("update_in_progress CLEARED"));
        update_in_progress = false;
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        request->redirect("/lcdOtaFailure"); 
        // delay(10);
        // pCONT_wif->EspRestart();
      }
    }
    lcdOtaTimer = millis();
  }

  /******************************************************************
  ***  UPLOAD_FILE_END)  
  ******************************************************************/
  if (final && !success)
  {

    Serial.println("upload.status == UPLOAD_FILE_END");
    if (lcdOtaTransferred >= tftFileSize)
    {
      if (nextionOtaResponse())
      { 
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        request->redirect("/lcdOtaSuccess"); 
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Delay to allow LCD to update"));
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: Failure"));
        request->redirect("/lcdOtaFailure");
      }
      pCONT_wif->EspRestart(); // Change later to schedule a restart
    }
  }

}


#ifdef ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL
void mNextionPanel::webHandleLcdDownload(AsyncWebServerRequest *request)
{ // http://plate01/lcddownload
  
  ALOG_INF(PSTR("HTTP: webHandleLcdDownload HTTP download: %s"), request->host());

  String data = String();


  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD Firmware updates");

  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);
  data += FPSTR(HTTP_STYLE3);
  data += FPSTR(HASP_STYLE);
  data += FPSTR(HTTP_HEAD_END3);
  
  data += F("<h1>");
  data += pCONT_set->Settings.system_name.friendly;
  data += F(" LCD Update</h1>");
  
  data += F("<br/>Updating LCD firmware using HTTP from: ");
  data += request->arg("lcdFirmware");

  nextionOtaStartDownload(request, request->arg("lcdFirmware") );

  data += FPSTR(HTTP_END3);  
  
  request->send(200, "text/html", data);

}
#endif // ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL


void mNextionPanel::webHandleLcdUpdateSuccess(AsyncWebServerRequest *request)
{ // http://plate01/lcdOtaSuccess

  ALOG_INF(PSTR("HTTP: Sending /lcdOtaSuccess page to client connected from: %s"), request->host());

  String data = String();

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD firmware update success");

  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);
  data += FPSTR(HTTP_STYLE3);
  data += FPSTR(HASP_STYLE);
  data += (F("<meta http-equiv='refresh' content='15;url=/' />"));
  data += FPSTR(HTTP_HEAD_END3);
  
  data += F("<h1>");
  data += pCONT_set->Settings.system_name.friendly;
  data += F(" LCD Update</h1>");
  
  data += FPSTR(HTTP_END3);  
  
  request->send(200, "text/html", data);

}


void mNextionPanel::webHandleLcdUpdateFailure(AsyncWebServerRequest *request)
{ // http://plate01/lcdOtaFailure  

  ALOG_INF(PSTR("HTTP: Sending /lcdOtaFailure page to client connected from: %s"), request->host());

  String data = String();

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " LCD firmware update success");

  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);
  data += FPSTR(HTTP_STYLE3);
  data += FPSTR(HASP_STYLE);
  data += (F("<meta http-equiv='refresh' content='15;url=/' />"));
  data += FPSTR(HTTP_HEAD_END3);
  
  data += F("<h1>");
  data += pCONT_set->Settings.system_name.friendly;
  data += F(" LCD Update</h1>");
  
  data += F("<br/>Updating LCD firmware using HTTP from: ");

  data += (F(" LCD update failed :(</h1>"));
  data += (F("Restarting HASwitchPlate to apply changes..."));

  data += FPSTR(HTTP_END3);  
  
  request->send(200, "text/html", data);  
  
}


void mNextionPanel::nextionOtaStartDownload(AsyncWebServerRequest *request, const String &lcdOtaUrl)
{ // Upload firmware to the Nextion LCD via HTTP download
  // based in large part on code posted by indev2 here:
  // http://support.iteadstudio.com/support/discussions/topics/11000007686/page/2

  uint32_t lcdOtaFileSize = 0;
  String lcdOtaNextionCmd;
  uint32_t lcdOtaChunkCounter = 0;
  uint16_t lcdOtaPartNum = 0;
  uint32_t lcdOtaTransferred = 0;
  uint8_t lcdOtaPercentComplete = 0;
  const uint32_t lcdOtaTimeout = 30000; // timeout for receiving new data in milliseconds
  static uint32_t lcdOtaTimer = 0;      // timer for lcdOtaTimeout

  HTTPClient lcdOtaHttp;
  WiFiClientSecure lcdOtaWifiSecure;
  WiFiClient lcdOtaWifi;
  if (lcdOtaUrl.startsWith(F("https")))
  {
    ALOG_INF(PSTR("LCDOTA: Attempting firmware update from HTTPS host: %s"), lcdOtaUrl.c_str());

    lcdOtaHttp.begin(lcdOtaWifiSecure, lcdOtaUrl);
    lcdOtaWifiSecure.setInsecure();
    // lcdOtaWifiSecure.setBufferSizes(512, 512);
  }
  else
  {
    ALOG_INF(PSTR("LCDOTA: Attempting firmware update from HTTP host: %s"), lcdOtaUrl.c_str());
    lcdOtaHttp.begin(lcdOtaWifi, lcdOtaUrl);
  }

  int lcdOtaHttpReturn = lcdOtaHttp.GET();
  if (lcdOtaHttpReturn > 0)
  { // HTTP header has been sent and Server response header has been handled
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP GET return code: %d"), lcdOtaHttpReturn);
    if (lcdOtaHttpReturn == HTTP_CODE_OK)
    {                                                 // file found at server
      int32_t lcdOtaRemaining = lcdOtaHttp.getSize(); // get length of document (is -1 when Server sends no Content-Length header)
      lcdOtaFileSize = lcdOtaRemaining;
      static uint16_t lcdOtaParts = (lcdOtaRemaining / 4096) + 1;
      static const uint16_t lcdOtaBufferSize = 1024; // upload data buffer before sending to UART
      static uint8_t lcdOtaBuffer[lcdOtaBufferSize] = {};

      ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: File found at Server. Size %d bytes in %d 4k chunks"), lcdOtaRemaining, lcdOtaParts);

      // WiFiUDP::stop(); // Keep mDNS responder and MQTT traffic from breaking things
      // if (mqttClient.connected())
      // {
      //   ALOG_INF(PSTR("LCDOTA: LCD firmware upload starting, closing MQTT connection."));
      //   mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
      //   debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
      //   mqttClient.disconnect();
      // }

      WiFiClient *stream = lcdOtaHttp.getStreamPtr();      // get tcp stream
      display->write(nextionSuffix, sizeof(nextionSuffix)); // Send empty command
      display->flush();
      nextionHandleInput();

      String lcdOtaNextionCmd = "whmi-wri " + String(lcdOtaFileSize) + "," + String(ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT) + ",0";
      ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Sending LCD upload command: %s"), lcdOtaNextionCmd);

      display->print(lcdOtaNextionCmd);
      display->write(nextionSuffix, sizeof(nextionSuffix));
      display->flush();

      if (nextionOtaResponse())
      {
        ALOG_INF(PSTR("LCDOTA: LCD upload command accepted."));
      }
      else
      {
        ALOG_INF(PSTR("LCDOTA: LCD upload command FAILED.  Restarting device."));
        pCONT_wif->EspRestart();
      }
      ALOG_INF(PSTR("LCDOTA: Starting update"));
      lcdOtaTimer = millis();
      while (lcdOtaHttp.connected() && (lcdOtaRemaining > 0 || lcdOtaRemaining == -1))
      {                                                // Write incoming data to panel as it arrives
        uint16_t lcdOtaHttpSize = stream->available(); // get available data size

        if (lcdOtaHttpSize)
        {
          uint16_t lcdOtaChunkSize = 0;
          if ((lcdOtaHttpSize <= lcdOtaBufferSize) && (lcdOtaHttpSize <= (4096 - lcdOtaChunkCounter)))
          {
            lcdOtaChunkSize = lcdOtaHttpSize;
          }
          else if ((lcdOtaBufferSize <= lcdOtaHttpSize) && (lcdOtaBufferSize <= (4096 - lcdOtaChunkCounter)))
          {
            lcdOtaChunkSize = lcdOtaBufferSize;
          }
          else
          {
            lcdOtaChunkSize = 4096 - lcdOtaChunkCounter;
          }
          stream->readBytes(lcdOtaBuffer, lcdOtaChunkSize);
          display->flush();                              // make sure any previous writes the UART have completed
          display->write(lcdOtaBuffer, lcdOtaChunkSize); // now send buffer to the UART
          lcdOtaChunkCounter += lcdOtaChunkSize;
          if (lcdOtaChunkCounter >= 4096)
          {
            display->flush();
            lcdOtaPartNum++;
            lcdOtaTransferred += lcdOtaChunkCounter;
            lcdOtaPercentComplete = (lcdOtaTransferred * 100) / lcdOtaFileSize;
            lcdOtaChunkCounter = 0;
            if (nextionOtaResponse())
            { // We've completed a chunk
              ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Part %d OK, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
              lcdOtaTimer = millis();
            }
            else
            {
              ALOG_ERR(PSTR(D_LOG_NEXTION "LCDOTA: Part %d FAILED, %d%% complete"), lcdOtaPartNum, lcdOtaPercentComplete);
              delay(2000); // extra delay while the LCD does its thing
              pCONT_wif->EspRestart();
            }
          }
          else
          {
            delay(20); //check these delays 
          }
          if (lcdOtaRemaining > 0)
          {
            lcdOtaRemaining -= lcdOtaChunkSize;
          }
        }
        delay(10);
        if ((lcdOtaTimer > 0) && ((millis() - lcdOtaTimer) > lcdOtaTimeout))
        { // Our timer expired so reset
          ALOG_INF(PSTR("LCDOTA: ERROR: LCD upload timeout. Restarting."));
          pCONT_wif->EspRestart();
        }
      }
      lcdOtaPartNum++;
      lcdOtaTransferred += lcdOtaChunkCounter;
      if ((lcdOtaTransferred == lcdOtaFileSize) && nextionOtaResponse())
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Success, wrote %d of %d bytes"), lcdOtaTransferred, tftFileSize);
        uint32_t lcdOtaDelay = millis();
        ALOG_INF(PSTR("LCDOTA: Waiting 5 seconds to allow LCD to apply updates we've sent."));
        while ((millis() - lcdOtaDelay) < 5000)
        { // extra 5sec delay while the LCD handles any local firmware updates from new versions of code sent to it
          // request->handleClient();
          yield();
        }
        pCONT_wif->EspRestart();
      }
      else
      {
        ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: Failure, lcdOtaTransferred: %d lcdOtaFileSize: %d"), lcdOtaTransferred, lcdOtaFileSize);
        pCONT_wif->EspRestart();
      }
    }
  }
  else
  {
    ALOG_INF(PSTR(D_LOG_NEXTION "LCDOTA: HTTP GET failed, error code %s"), lcdOtaHttp.errorToString(lcdOtaHttpReturn));
    pCONT_wif->EspRestart();
  }
  lcdOtaHttp.end();
}


void mNextionPanel::WebPage_LCD_Update_TFT(AsyncWebServerRequest *request)
{ 
  
  ALOG_INF(PSTR("HTTP: WebPage_LCD_Update_TFT: %s"), request->host());

  String data = String();

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " Firmware updates");
  // webServer->setContentLength(CONTENT_LENGTH_UNKNOWN);
  // webServer->send(200, "text/html", httpHeader);
  
  data += httpHeader;
  data += FPSTR(HTTP_SCRIPT3);
  data += FPSTR(HTTP_STYLE3);
  data += FPSTR(HASP_STYLE);
  data += FPSTR(HTTP_HEAD_END3);

  data += F("<h1>");
  data += pCONT_set->Settings.system_name.friendly;
  data += F(" Nextion Firmware Update</h1>");  

  /**
   * @brief Update with URL only
   **/
  #ifdef ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL
  data += F("<br/><hr><form method='get' action='lcddownload'>");
  if(updateLcdAvailable){ data += F("<font color='green'><b>LCD update available!</b></font>"); }
  data += F("<br/><b>Update Nextion LCD from URL</b><small><i> http only</i></small>");
  data += F("<br/><input id='lcdFirmware' name='lcdFirmware' value='");
  data += "No Url Set";
  data += F("'><br/><br/><button type='submit'>Update LCD from URL</button></form>");
  #endif // ENABLE_DEBUGFEATURE_NEXTION__LCD_UPDATE_VIA_URL

  /**
   * @brief Update with File
   **/
  data += F("<br/><form method='POST' action='/lcdupload' enctype='multipart/form-data'>");
  data += F("<br/><b>Update Nextion LCD from file</b><input type='file' id='lcdSelect' name='files[]' accept='.tft'/>");
  data += F("<br/><br/><button type='submit' id='lcdUploadSubmit' onclick='ackLcdUploadSubmit()'>Update LCD from file</button></form>");

  /**
   * @brief Javascript to collect the filesize of the LCD upload and send it to /tftFileSize
   **/
  data += F("<script>function handleLcdFileSelect(evt) {");
  data += F("var uploadFile = evt.target.files[0];");
  data += F("document.getElementById('lcdUploadSubmit').innerHTML = 'Upload LCD firmware ' + uploadFile.name;");
  data += F("var tftFileSize = '/tftFileSize?tftFileSize=' + uploadFile.size;");
  data += F("var xhttp = new XMLHttpRequest();xhttp.open('GET', tftFileSize, true);xhttp.send();}");
  data += F("function ackLcdUploadSubmit() {document.getElementById('lcdUploadSubmit').innerHTML = 'Uploading LCD firmware...';}");
  data += F("function handleEspFileSelect(evt) {var uploadFile = evt.target.files[0];document.getElementById('espUploadSubmit').innerHTML = 'Upload ESP firmware ' + uploadFile.name;}");
  data += F("function ackEspUploadSubmit() {document.getElementById('espUploadSubmit').innerHTML = 'Uploading ESP firmware...';}");
  data += F("document.getElementById('lcdSelect').addEventListener('change', handleLcdFileSelect, false);");
  data += F("document.getElementById('espSelect').addEventListener('change', handleEspFileSelect, false);</script>");

  data += FPSTR(HTTP_END3);
  
  
  request->send(200, "text/html", data);

}


void mNextionPanel::webHandleTftFileSize(AsyncWebServerRequest *request)
{ // http://plate01/tftFileSize

  // ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK  "HTTP: Sending /tftFileSize page to client connected from: %s"), webServer->client().remoteIP().toString());

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", String(pCONT_set->Settings.system_name.friendly) + " TFT Filesize");
  httpHeader += FPSTR(HTTP_END3);

  request->send(200, "text/html", httpHeader);

  // webServer->sendContent_P(HTTP_HEAD_END);
  tftFileSize =  request->arg(F("tftFileSize")).toInt();
  
  ALOG_INF(PSTR(D_LOG_NEXTION "Received tftFileSize: %d"), tftFileSize);

}


void mNextionPanel::webHandleRoot(AsyncWebServerRequest* request)
{ 

  ALOG_INF(PSTR(D_LOG_NEXTION DEBUG_INSERT_PAGE_BREAK "HTTP: Sending root page to client connected from: %s"), request->host());

  String conv = String();

  String httpHeader = FPSTR(HTTP_HEAD_START);
  httpHeader.replace("{v}", pCONT_set->Settings.system_name.friendly);
  conv += httpHeader;
  conv += FPSTR(HTTP_SCRIPT3);
  conv += FPSTR(HTTP_STYLE3);
  conv += FPSTR(HASP_STYLE);
  conv += FPSTR(HTTP_HEAD_END3);

  conv += (F("<br/><hr><button type='submit'>save settings</button></form>"));

  conv += (F("<hr><form method='get' action='firmware'>"));
  conv += (F("<button type='submit'>update firmware</button></form>"));

  conv += (F("<hr><form method='get' action='reboot'>"));
  conv += (F("<button type='submit'>reboot device</button></form>"));

  conv += (F("<hr><form method='get' action='resetConfig'>"));
  conv += (F("<button type='submit'>factory reset settings</button></form>"));

  conv += FPSTR(HTTP_END3);
  
  request->send(200, "text/html", conv);

}


bool mNextionPanel::nextionOtaResponse()
{                                               // Monitor the serial port for a 0x05 response within our timeout
  unsigned long nextionCommandTimeout = 2000;   // timeout for receiving termination string in milliseconds
  unsigned long nextionCommandTimer = millis(); // record current time for our timeout
  bool otaSuccessVal = false;
  while ((millis() - nextionCommandTimer) < nextionCommandTimeout)
  {
    if (display->available())
    {
      byte inByte = display->read();
      // Serial.println(inByte, HEX);
      if (inByte == 0x5)
      {
        otaSuccessVal = true;
        break;
      }
    }
    else
    {
      delay(1);
    }
  }

  if(!otaSuccessVal){
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION "QUIT OTA and NOOOOOOOOOT Restart display"));
    // nextionReset();
  }


  return otaSuccessVal;
}


void mNextionPanel::nextionUpdateProgress(const unsigned int &progress, const unsigned int &total)
{
  uint8_t progressPercent = (float(progress) / float(total)) * 100;
  nextionSetAttr("p[0].b[4].val", String(progressPercent).c_str());
}


#endif // USE_MODULE_NETWORK_WEBSERVER


#endif