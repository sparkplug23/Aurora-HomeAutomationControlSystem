#include "mImmersionTankColour.h"

/**
 * @brief 
 * 
 * Use for immersion tank to use temperature sensors to show colours on wall, when the button is pressed
 * 
 * Single Press = 10 seconds
 * Multipress = 15*press_count
 * Long Press = Off 
 * 
 * 
 */

#ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR


void mImmersionTankColour::init(void)
{
  
  data.time_on = new Decounter<uint16_t>();

}


void mImmersionTankColour::Pre_Init()
{
  
  settings.fEnableModule = true;

}

int8_t mImmersionTankColour::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    init();
  }

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
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
      Event_Handle_Light_Toggle_Button(); // one press = toggle, multipress is 15 minute increment of on time, hold = stay on
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker


void mImmersionTankColour::Event_Handle_Light_Toggle_Button()
{

#ifdef USE_MODULE_SENSORS_BUTTONS

  if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_PRESS_ID)
  {

    data.time_on->Start(10); // 10 seconds
    ALOG_INF( PSTR("Start(10)") );

  }
  if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_HOLD_ID)
  {

    data.time_on->Stop(); // Stop
    ALOG_INF( PSTR("Stop()") );

  }
  if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_MULTIPLE_PRESS_ID)
  {

    uint16_t seconds_on = 0;

    //1 press = 10 SECONDS above
    //2 press = 15 minute
    //3 press = 30 minute
    //4 press = 45 minute
    
    // if(data.time_on->Value() < (120*60)) // if less than 2 hours, append 15 seconds per button press
    // {
      seconds_on = (pCONT_rules->event_triggered.value.data[2]-1) * 15 * 60;
      ALOG_INF( PSTR("presses = %d, seconds = %d"), pCONT_rules->event_triggered.value.data[2], seconds_on );

      data.time_on->Start(seconds_on); 
      // ALOG_INF( PSTR("Start(%d)"), seconds_on );

    // }

  }

#endif



}





void mImmersionTankColour::EverySecond()
{
  // ALOG_INF( PSTR("EverySecond %d"), UINT16_MAX );

  bool flag_show_enabled = false;

  if(data.time_on->UpdateTick())
  {
    flag_show_enabled = true;
    ALOG_INF( PSTR(D_LOG_GARAGE D_COMMAND_NVALUE_K("Running Value")), data.time_on->Value());
    mqtthandler_sensor_ifchanged.tRateSecs = 1;
  }
  else
  {
    flag_show_enabled = false;
    mqtthandler_sensor_ifchanged.tRateSecs = 60;
    // ALOG_INF( PSTR(D_LOG_GARAGE D_COMMAND_NVALUE_K("Not Running Value")), data.time_on->Value());
  }

  switch(data.mode)
  {
    default:
    case MODE_SHOW_COLOURS_OFF_ID:
      
      pCONT_lAni->CommandSet_Global_BrtRGB_255(0);

    break;
    case MODE_SHOW_COLOURS_GRADIENT_COLOUR_TEMP_ON_ID:

      if(flag_show_enabled)
      {

        // pCONT_lAni->CommandSet_PaletteID(mPalette::PALETTELIST_VARIABLE_GENERIC_01__ID);
        // pCONT_iLight->CommandSet_AnimationModeID(pCONT_lAni->EFFECTS_FUNCTION__STATIC_PALETTE__ID);
        pCONT_lAni->CommandSet_Animation_Transition_Time_Ms(0);
        pCONT_lAni->CommandSet_Global_BrtRGB_255(255);
        SubTask_StripSet_Showing();            
        
      }
      else
      {

        pCONT_lAni->CommandSet_Global_BrtRGB_255(0);

      }

    break;
  }


}



void mImmersionTankColour::SubTask_StripSet_Showing()
{


  uint8_t sensor_count = pCONT_db18->db18_sensors_active;

  if((sensor_count>=DB18_SENSOR_MAX)||(sensor_count==0)){ return; }

  sensor_count = 6; // only map first 6 sensors

  RgbColor colour;

  // RGB Gradient with 255 indexes
  uint8_t index = 0;
  encoded_gradient_temp_array[index++] = sensor_count;
  encoded_gradient_temp_array[index++] = MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT__ID;
  encoded_gradient_temp_array[index++] = 0;
  encoded_gradient_temp_array[index++] = 0;
  encoded_gradient_temp_array[index++] = 0;

  char name[30];

  // int test_temp[sensor_count];


  // ALOG_TST(PSTR("val=%d %d %d %d %d %d"),
  //   (int)pCONT_db18->sensor[0].reading.val,
  //   (int)pCONT_db18->sensor[1].reading.val,
  //   (int)pCONT_db18->sensor[2].reading.val,
  //   (int)pCONT_db18->sensor[3].reading.val,
  //   (int)pCONT_db18->sensor[4].reading.val,
  //   (int)pCONT_db18->sensor[5].reading.val);

  // ALOG_TST(PSTR("add=%d %d %d %d %d %d"),
  //   pCONT_db18->sensor[0].address[7],
  //   pCONT_db18->sensor[1].address[7],
  //   pCONT_db18->sensor[2].address[7],
  //   pCONT_db18->sensor[3].address[7],
  //   pCONT_db18->sensor[4].address[7],
  //   pCONT_db18->sensor[5].address[7]);

  for(uint8_t ii=0;ii<sensor_count;ii++){

    // pCONT_db18->sensor[ii].reading.val = 20+(5*ii);
    
    int8_t device_id = -1;
    uint16_t unique_module_id = pCONT_db18->GetModuleUniqueID();


    //temp solution
    switch(ii){
      case 0: sprintf(name, "TankPosition100\0"); break;
      case 1: sprintf(name, "TankPosition80\0"); break;
      case 2: sprintf(name, "TankPosition60\0"); break;
      case 3: sprintf(name, "TankPosition40\0"); break;
      case 4: sprintf(name, "TankPosition20\0"); break;
      case 5: sprintf(name, "TankPosition00\0"); break;
    }

    int16_t device_id_found = DLI->GetDeviceIDbyName(name, unique_module_id);

    // AddLog(LOG_LEVEL_INFO,PSTR("device_id_found = %d"),device_id_found);
    //AddLog(LOG_LEVEL_INFO,PSTR("device_id_found = %s %d %d"), name, device_id_found, (int)pCONT_db18->sensor[device_id_found].reading.val);


    // Check for matches with variables names  
    // if ((
      // index_found = pCONT_sup->GetDListIDbyNameCtr(buffer, sizeof(buffer), c, pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr)) >= 0) {
    //   // index_found = STATE_NUMBER_OFF_ID;    
    //     ALOG_DBM( PSTR("index_found = %d"),index_found);    
    //     return index_found;
    // }
    int8_t true_struct_id = pCONT_db18->FindStructIndexByAddressID(device_id_found);

    // ALOG_TST(PSTR("device_id_found true_struct_id %d %d"),device_id_found,true_struct_id);

    if(true_struct_id != -1){
      
      // test_temp[ii] = (int)pCONT_db18->sensor[true_struct_id].reading.val;
      
      // switch(ii)
      // {
      //   default:
      //   case 0: colour = RgbColor(255,0,0); break;
      //   case 1: colour = RgbColor(0,255,0); break;
      //   case 2: colour = RgbColor(0,0,255); break;
      //   case 3: colour = RgbColor(255,0,255); break;
      //   case 4: colour = RgbColor(255,255,0); break;
      //   case 5: colour = RgbColor(0,255,255); break;
      // }


      colour = pCONT_iLight->GetColourValueUsingMaps_FullBrightness(pCONT_db18->sensor[true_struct_id].reading.val,0);
      // colour = pCONT_iLight->GetColourValueUsingMaps_FullBrightness( map(ii, 0,5, 0,60)  ,0);

      // colour = RgbColor( map(ii, 0,5, 0,255), 0 ,0);


      // switch(ii)
      // {
      //   default:
      //   case 0: colour = RgbColor(255,0,0);   break;
      //   case 1: colour = RgbColor(0,255,0);   break;
      //   case 2: colour = RgbColor(0,0,255);   break;
      //   case 3: colour = RgbColor(255,0,255); break;
      //   case 4: colour = RgbColor(255,255,0); break;
      //   case 5: colour = RgbColor(0,255,255); break;
      // }


      encoded_gradient_temp_array[index++] = map(ii, 0,5, 0,255);
      encoded_gradient_temp_array[index++] = colour.R;
      encoded_gradient_temp_array[index++] = colour.G;
      encoded_gradient_temp_array[index++] = colour.B;
    }

    // ALOG_INF( PSTR("%d> SensIndex=%d, Temp=%d,  SensName=%s, PixelIndex=%d, Rgb=(%d,%d,%d)"),
    //   ii,
    //   true_struct_id,
    //   (int)pCONT_db18->sensor[true_struct_id].reading.val,
    //   name,
    //   map(ii, 0,5, 0,255),
    //   colour.R, colour.G, colour.B
    // );







  }


  // uint8_t vals[] = 
  // {
  //   6,5,0,0,0,
  //   0,0,0,255,
  //   50,51,0,205,
  //   100,102,0,154,
  //   150,153,0,103,
  //   200,204,0,52,
  //   255,255,0,1
  // };


  //remove the last unneeded iter
  // index--;

  // AddLog(LOG_LEVEL_ERROR, PSTR("index=%d"),index);
  // AddLog_Array(LOG_LEVEL_DEV_TEST, "test_temp", test_temp, 6);
  // delay(5000);


  pCONT_iLight->CommandSet_PaletteColour_RGBCCT_Raw_By_ID(mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID, encoded_gradient_temp_array, index);

  // pCONT_iLight->CommandSet_PaletteColour_RGBCCT_Raw_By_ID(mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID, vals, ARRAY_SIZE(vals));


  pCONT_lAni->CommandSet_PaletteID(mPaletteI->PALETTELIST_VARIABLE_GENERIC_01__ID);

  // pCONT_lAni->CommandSet_TransitionOrderID( pCONT_lAni->TRANSITION_ORDER_INORDER_ID);

  // ALOG_TST(PSTR("index=%d"),index);

  // AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_RELAY_CONTROLS is disabled"));
  
  // AddLog_Array_Int(LOG_LEVEL_DEV_TEST, "encoded_gradient_temp_array", encoded_gradient_temp_array, index);

  // AddLog_Array_Int(LOG_LEVEL_DEV_TEST, "encoded", encoded_gradient_temp_array, index );

  // AddLog_Array(LOG_LEVEL_DEV_TEST, "encoded_gradient_temp_array", encoded_gradient_temp_array, index);
  
  // if(pCONT_time->uptime_seconds_nonreset > 60){

  
  // //print result over mqtt so I can test
  // char buffer[300]; memset(&buffer, 0, sizeof(buffer));
  // for(uint8_t ii=0;ii<index;ii++){
  //   sprintf(buffer+strlen(buffer), "%d,", encoded_gradient_temp_array[ii]);
  // }
  // pCONT_mqtt->Send_Prefixed_P(PSTR("debug/1"),buffer);

  //  memset(&buffer, 0, sizeof(buffer));
  // for(uint8_t ii=0;ii<6;ii++){
  //   sprintf(buffer+strlen(buffer), "%d,", test_temp[ii]);
  // }
  // pCONT_mqtt->Send_Prefixed_P(PSTR("debug/2"), buffer);
  // pCONT_mqtt->publish_device("/debug", buffer, false);

}




uint8_t mImmersionTankColour::ConstructJSON_Settings(uint8_t json_level, bool json_appending){
  
  JBI->Start();
    JBI->Add_P("test",0);  
  JBI->End();

}

uint8_t mImmersionTankColour::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add_P(D_LIGHTPOWER, GetLightState());

    JBI->Add("TimeOn", data.time_on->Value());
    JBI->Add("Mode",   data.mode);

    JBI->Array_Start("encoded_gradient_temp_array");
      for(int i=0;i<ARRAY_SIZE(encoded_gradient_temp_array);i++)
      {
        JBI->Add(encoded_gradient_temp_array[i]);
      }
    JBI->Array_End();
  
  return JBI->End();

}

/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/



void mImmersionTankColour::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  if(jtok = obj["ColourTimeOn"])
  {

    data.time_on->Start(jtok.getInt());

    data.mode = MODE_SHOW_COLOURS_GRADIENT_COLOUR_TEMP_ON_ID;

    ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K("ColourTimeOn")), data.time_on->Value() );

  }


}

/*

{
  "ColourPalette": 15,
  "PaletteEdit": {
    "ColourPalette": 15,
    "Data": [
      6,5,0,0,0,
  0,0,0,255,
  50,51,0,205,
  100,102,0,154,
  150,153,0,103,
  200,204,0,52,
  255,255,0,1
    ]
  },
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Static"
  },
  "Rate":1,
  "BrightnessRGB":100,
  "ColourTimeOn":600
}


{"TimeOn":521,"Mode":0,"encoded_gradient_temp_array":[
  
  6,5,0,0,0,
  
  0,255,0,0,
  51,0,255,0,
  102,0,0,255,
  153,255,0,255,
  204,255,255,0,
  255,0,255,255,
  
  
  3,127,252,253,57,156,228,22,112,255,153,219,249,129,132,19,28,188,52,249,189,96,161,120,88,16,95,91,186,103,186,20,151,191,42,79,88,81,156,10,212,229,130,94,37,93,35,189,132,103,252,165,18,193,84,80,133,210,3,221,176,17,129,194,241,197,206,203,213,9,249,187,211,46,83,82,36,232,185,109,243,229,255,140,248,100,19,119,173,118,175,143,230,224,202,236,33,244,226,103,219,240,24,147,198,203,69,33,62,218,129,250,232,251,144,67,111,183,92,89,251,243,151,122,116,101,223,51,59,189,141,174,188,170,237,18,97,211,67,189,239,214,178,9,169,185,111,169,73,66,4,107,103,254,179,38,182,69,78,0,129,254,125,127,126,97,197,225,166,51,176]}




*/

/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

void mImmersionTankColour::MQTTHandler_Init(){

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 600; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mImmersionTankColour::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  ptr->ConstructJSON_function = &mImmersionTankColour::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; // water never changes fast 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_POWER_CTR;
  ptr->ConstructJSON_function = &mImmersionTankColour::ConstructJSON_Sensor;
  
}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mImmersionTankColour::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mImmersionTankColour::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mImmersionTankColour::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}


#endif
