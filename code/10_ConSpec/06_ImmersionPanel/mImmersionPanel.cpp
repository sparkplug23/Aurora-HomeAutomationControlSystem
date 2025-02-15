/**
  
  @brief For inside the hotpress and will coordinate the group functionalities
  - Now
   * Show stuff on OLED       
          Line1: Uptime
          Line2: Immersion Heater State [ON X mins | Off]
          Line3: SW: x.x*   Warm|Hot|Cold   where warm is enough for a short shower, Hot is enough for a long one 
          Line4: BH: x.x*   Warm|Hot|Cold   Warm enough to be used, Hot is ideal
   * React to button press
   * Show state on 3 LEDs
    ** LED1(red) = Immersion Heater on
    ** LED2(grn) = Hot enough for shower
    ** LED3(blu) = Hot enough for bath


    Add LED in new Relay box to show when it is running with it closed.
      plastic black inline box
      black from db18 cut for relay data
      led drill so it faces out
      glanded
      scrwed to wood



  - Later
   * Merge ImmersionTankColour into this

  Copyright (C) 2022    Michael Doone

  @note Relys on all other modules (no timers), and acts in place of rules

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

#include "mImmersionPanel.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL


int8_t mImmersionPanel::Tasker(uint8_t function, JsonParserObject obj){

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
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      Event_InputStateChanged();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif  
  }

}//end



void mImmersionPanel::Pre_Init(void)
{

}


void mImmersionPanel::Init(void)
{
  
    settings.fEnableSensor = true;

}


void mImmersionPanel::EverySecond()
{


  /*****
   * Red
  */
  if(pCONT_hvac->zone[0].program_timer_method.IsRunning())
  {
    pCONT_led->CommandSet_LED_Power(1,0);
  }else{
    pCONT_led->CommandSet_LED_Power(0,0);
  }

  SubTask_UpdateOLED();



  // if(tkr_time->uptime_seconds_nonreset<60)
  // {
  //   pCONT_led->CommandSet_LED_Power(1,0);
  // }else
  // if(tkr_time->uptime_seconds_nonreset<120)
  // {
  //   pCONT_led->CommandSet_LED_Power(1,1);
  // }else
  // if(tkr_time->uptime_seconds_nonreset<180)
  // {
  //   pCONT_led->CommandSet_LED_Power(1,2);
  // }else
  // {
  //   pCONT_led->CommandSet_LED_Power(0,0);
  //   pCONT_led->CommandSet_LED_Power(0,1);
  //   pCONT_led->CommandSet_LED_Power(0,2);
  // }




}

/**
 * @brief 
 * 
 * LED1 (RED)
 *  Immersion Running
 * 
 * LED2 (GREEN)
 *  Warm enough for shower
 * 
 * LED3 (BLUE)
 *  Warm enough for bath
 * 
 * 
 */
void mImmersionPanel::SubTask_UpdateStatusLEDs()
{


}


/**
 * @brief 
 * Show basic info
 * 
 * IM: SECS min
 * IT: Immersion Temp used for shower
 * BT: Water temp used for bath
 * CL : Show UTC time for easy checking it is working / "UP": Every 5 seconds alternate between showing RTC and Uptime
 * 
 * 
 * 
 * */
void mImmersionPanel::SubTask_UpdateOLED()
{

#ifdef USE_MODULE_DISPLAYS_OLED_SH1106
  char line_ctr[15] = {0};
  char buffer[15] = {0};

  tkr_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  /**
   * Row 1
   * Immersion Heating
   * 
   * 1234567890
   * IM:XXX min
   * */
  #ifdef USE_MODULE_CONTROLLER_HVAC
  
  if(pCONT_hvac->zone[0].program_timer_method.GetTimer_Minutes() < 99)
  {
    snprintf(line_ctr, sizeof(line_ctr), "IM:  %02d m",
      pCONT_hvac->zone[0].program_timer_method.GetTimer_Minutes()
    );
  }
  else
  {
    snprintf(line_ctr, sizeof(line_ctr), "IM: %03d m",
      pCONT_hvac->zone[0].program_timer_method.GetTimer_Minutes()
    );
  }
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  #endif // USE_MODULE_CONTROLLER_HVAC


  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  snprintf(buffer, sizeof(buffer), "SH: %d",
    0
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  snprintf(buffer, sizeof(buffer), "BA: %d",
    0
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 2);
  #endif //USE_MODULE_SENSORS__DS18X20_ESP32_2023
  
  
  /**
   * Row 4/4
   * 
   * Showing UTC time just so I know the device is functioning as expected
   * 
   * 1234567890
   * T HH:MM:SS
   * */
  snprintf(buffer, sizeof(buffer), "%s",tkr_time->RtcTime.hhmmss_ctr);
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);

  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}


/**
 * @brief 
 * 
 * Button "Immersion"
 *  Press 1: 60m
 *  Press Hold: Turn Immersion Off 
 * 
 * Button "Tank Show"
 *  Press 1: Show Tank for 60 seconds
 *  Press Hold: Show 2hr 
 */
void mImmersionPanel::Event_InputStateChanged()
{
  
  ALOG_INF(PSTR("TASK_EVENT_INPUT_STATE_CHANGED_ID"));

  if(tkr_rules->event_triggered.module_id == D_UNIQUE_MODULE_SENSORS_BUTTONS_ID)
  {
    ALOG_INF(PSTR("TASK_EVENT_INPUT_STATE_CHANGED_ID D_UNIQUE_MODULE_SENSORS_BUTTONS_ID"));

    if(tkr_rules->event_triggered.value.data[0] == 1)
    {
      data.button1_state ^= 1;
      ALOG_INF(PSTR("TASK_EVENT_INPUT_STATE_CHANGED_ID BUTTON_PRESSED_ID button1_state=%d"), data.button1_state);
    }
    if(tkr_rules->event_triggered.value.data[0] == 1)
    {
      data.button2_state ^= 1;
      ALOG_INF(PSTR("TASK_EVENT_INPUT_STATE_CHANGED_ID BUTTON_PRESSED_ID button2_state=%d"), data.button2_state);
    }


  }






}


/******************************************************************************************************************
 * mImmersionPanel_Commands.cpp
*******************************************************************************************************************/


void mImmersionPanel::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];


    
}


/******************************************************************************************************************
 * mImmersionPanel_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mImmersionPanel::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);
    
  return JBI->End();

}

uint8_t mImmersionPanel::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mImmersionPanel_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mImmersionPanel::MQTTHandler_Init()
{

  struct handler<mImmersionPanel>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mImmersionPanel::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mImmersionPanel::ConstructJSON_State;

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mImmersionPanel::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mImmersionPanel::MQTTHandler_Rate()
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
void mImmersionPanel::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH