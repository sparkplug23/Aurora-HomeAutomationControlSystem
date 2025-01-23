#include "mInterfaceLight.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE 

int8_t mInterfaceLight::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;

  // As interface module, the parsing of module_init takes precedence over the Settings.light_settings.type
  switch(function){
    case TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
      Template_Load();
    break;
    case TASK_POINTER_INIT:

    break;
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
      EveryLoop();
    break;
    case TASK_EVERY_SECOND:
      EverySecond_AutoOff();
    break;
    /************
     * STORAGE SECTION * 
    *******************/    
    case TASK_TEMPLATE_MODULE_LOAD_AFTER_INIT_DEFAULT_CONFIG_ID:
      Template_Load_DefaultConfig();
    break;
    #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    #endif
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case TASK_EVENT_SET_POWER_ID:
      RulesEvent_Set_Power();
    break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEB SECTION * 
    *******************/   
    case TASK_WEB_ADD_HANDLER:    
      #ifdef USE_MODULE_NETWORK_WEBSERVER
      // MQTTHandler_AddWebURL_PayloadRequests(); // Therefore MQTT must be initialised before webui
      #endif
    break;

  } // end switch
  
} // END function


void mInterfaceLight::Pre_Init(void)
{

  bus_manager = new BusManager();

}

void mInterfaceLight::EverySecond_AutoOff()
{

  /**
   * @brief Master AutoOff DeCounter
   **/
  //ALOG_DBM( PSTR(D_LOG_LIGHT "scene.auto_off_settings.tSaved [%d]"),animation.auto_off_settings.time_decounter_secs);
  if(auto_off_settings.time_decounter_secs==1){ //if =1 then turn off and clear to 0
    // animation.name_id = MODE_SINGLECOLOUR_FADE_OFF__ID;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_INF(PSTR(D_LOG_LIGHT "auto_off_settings.time_decounter_secs==1 and disable"));
    #endif       
    
    CommandSet_LightPowerState(LIGHT_POWER_STATE_OFF_ID);
    //#ifdef ENABLE_LOG_LEVEL_INFO
    auto_off_settings.time_decounter_secs=0;
    //#endif
  }else
  if(auto_off_settings.time_decounter_secs>1){ //if =1 then turn off and clear to 0
    auto_off_settings.time_decounter_secs--; //decrease    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_INF(PSTR(D_LOG_LIGHT "auto_off_settings.time_decounter_secs=%d dec"), auto_off_settings.time_decounter_secs);
    #endif
  }

}

void mInterfaceLight::Template_Load()
{

  bool template_loaded = false;

  /**
   * @brief Check first if alternate templates are being used, to be selected by pin
   * 
   */
  #ifdef ENABLE_FEATURE_LIGHTING__ENABLE_ALTERANTE_TEMPLATE_USING_GPIO_SWITCH_ONE

  if(pCONT_pins->PinUsed(GPIO_SWT1_INV_ID))
  {
    if(pCONT_pins->DigitalRead(GPIO_SWT1_INV_ID)==0) // switch to LOW to activate alternate template
    {
      template_loaded = true;
          
      ALOG_INF(PSTR("buffer_writer Template_Load ------- A >>>>>>>>>> %d"),JBI->GetBufferSize());
      D_DATA_BUFFER_CLEAR();


      ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------F- >>>>>>>>>> %d"),JBI->GetBufferSize());


      // memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
      // strncpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(data_buffer.payload.ctr));
      memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE_ALTERNATE,sizeof(LIGHTING_TEMPLATE_ALTERNATE));

      ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
      data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

      data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything

      ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
      ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE_ALTERNATE), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE_ALTERNATE)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

      #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
      Serial.println(data_buffer.payload.ctr);
      #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD


      ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());

      // ALOG_HGL( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

      pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

      ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------G- >>>>>>>>>> %d"),JBI->GetBufferSize());
      
      runtime.template_loading.status.lighting = TemplateSource::HEADER_TEMPLATE;

    }

  }

  #endif  // ENABLE_FEATURE_LIGHTING__ENABLE_ALTERANTE_TEMPLATE_USING_GPIO_SWITCH_ONE



  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local

  if(!template_loaded)
  {
    ALOG_INF(PSTR("buffer_writer Template_Load ------- A >>>>>>>>>> %d"),JBI->GetBufferSize());
    D_DATA_BUFFER_CLEAR();
    ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------F- >>>>>>>>>> %d"),JBI->GetBufferSize());
    // memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
    // strncpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(data_buffer.payload.ctr));
    memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
    ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);
    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything
    ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
    ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

    #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
    Serial.println(data_buffer.payload.ctr);
    #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD

    ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());

    // ALOG_HGL( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

    pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

    Serial.println("we are here");

    ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------G- >>>>>>>>>> %d"),JBI->GetBufferSize());

    #ifdef USE_LIGHTING_TEMPLATE_ANOTHER
    ALOG_INF(PSTR("buffer_writer Template_Load ------- A >>>>>>>>>> %d"),JBI->GetBufferSize());
    D_DATA_BUFFER_CLEAR();
    ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------F- >>>>>>>>>> %d"),JBI->GetBufferSize());
    // memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
    // strncpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(data_buffer.payload.ctr));
    memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE_ANOTHER,sizeof(LIGHTING_TEMPLATE_ANOTHER));
    ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);
    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything
    ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
    ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE_ANOTHER), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE_ANOTHER)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

    #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
    Serial.println(data_buffer.payload.ctr);
    #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD

    ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());

    // ALOG_HGL( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

    pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

    ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------G- >>>>>>>>>> %d"),JBI->GetBufferSize());
    #endif // USE_LIGHTING_TEMPLATE_ANOTHER




  }

  #endif // USE_LIGHTING_TEMPLATE

  #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
  Serial.println("Template_Load PAUSE");
  delay(10000);
  #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD

}


void mInterfaceLight::Init(void)
{
  
  tkr_set->Settings.pwm_range = PWM_RANGE;
  // tkr_set->Settings.light_settings.light_fade = 1;
  // tkr_set->Settings.light_settings.light_speed = 5*2;
  tkr_set->runtime.power = 1;

  auto_off_settings.time_decounter_secs = 0;

  module_state.mode = ModuleStatus::Running;

}


String mInterfaceLight::GetColourOrderString(uint8_t colour_order) 
{
    String result = "";

    // Decode the RGB part (lower 3 bits)
    switch (colour_order & 0x0F) {  // Masking to get the lower 4 bits
        case 0x00: result += "RGB"; break;
        case 0x01: result += "GRB"; break;
        case 0x02: result += "BRG"; break;
        case 0x03: result += "BGR"; break;
        case 0x04: result += "GBR"; break;
        case 0x05: result += "RBG"; break;
        default: result += "Invalid"; break;
    }

    // Decode the white part (upper 4 bits)
    switch (colour_order & 0xF0) {  // Masking to get the upper 4 bits
        case 0x00: break;  // No white channels
        case 0x10: result += "C"; break;  // Cold White only
        case 0x20: result += "W"; break;  // Warm White only
        case 0x40: result += "WC"; break;  // Warm and Cold White
        default: result += " Invalid"; break;
    }

    return result;
}



void mInterfaceLight::EveryLoop()
{


        
} // END everyloop

/**
 * @brief If this function is called, it is already assumed that the command required overriding filesystem settings (e.g. file "config_module.json" is missing, "reset" is requested)
 * 
 */
void mInterfaceLight::Template_Load_DefaultConfig()
{

  #ifdef USE_LIGHTING_TEMPLATE_DEFAULT
  
  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }

  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr, LIGHTING_TEMPLATE_DEFAULT, sizeof(LIGHTING_TEMPLATE_DEFAULT));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  ALOG_DBM( PSTR("LIGHTING_TEMPLATE_DEFAULT" " READ = \"%s\""), data_buffer.payload.ctr);

  pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

  JBI->ReleaseLock();

  #endif // LIGHTING_TEMPLATE_DEFAULT

}


void mInterfaceLight::Save_Module()
{
  
  ALOG_INF(PSTR("mInterfaceLight::Save_Module()"));

  char buffer[100] = {0};

  /********************************************************************
   * Byte Data
   * ******************************************************************/

  char filename_byte[50];
  snprintf_P(filename_byte, sizeof(filename_byte), "/lgt_%S_byte.txt", GetModuleName()); // debugging in .txt so webui/edt can read it
  // snprintf_P(filename_byte, sizeof(filename_byte), "/lgt_%S.bin", GetModuleFriendlyName()); // possibly switch to .bin for binary data, though UI wont be able to read it

  struct TEST{
    uint16_t test = 1234;
    char message[7] = "hello\0";
  }test;

  pCONT_mfile->ByteFile_Save(filename_byte, (uint8_t*)&test, sizeof(TEST));


  /********************************************************************
   * Json Data
   * ******************************************************************/

  if(!JBI->RequestLock(GetModuleUniqueID())){
    return;
  }
 
  JBI->Start();

  #ifdef ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES
    JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str() );
  #endif // ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  
    JBI->Array_Start("BusConfig");
    for(uint8_t bus_i = 0; bus_i < bus_manager->getNumBusses(); bus_i++)
    {    
      JBI->Object_Start();

        uint8_t pins[5] = {0};
        uint8_t pin_count = bus_manager->busses[bus_i]->getPins(pins);
        JBI->Array_Start("Pin");
          for(uint8_t ii=0;ii<pin_count;ii++){ JBI->Add(pins[ii]); }
        JBI->Array_End();

        JBI->Add("Start", bus_manager->busses[bus_i]->getStart());
        JBI->Add("Length", bus_manager->busses[bus_i]->getLength());
        JBI->Add_P("BusType", bus_manager->busses[bus_i]->getTypeName());


        // JBI->Add_P("ColourOrder", bus_manager->getColourOrderName(bus_manager->busses[bus_i]->getColorOrder(), buffer, sizeof(buffer)) );

      JBI->Object_End();
    }
    JBI->Array_End();

    JBI->Add("BrightnessRGB", getBriRGB_Global());
    JBI->Add("BrightnessCCT", getBriCCT_Global());



  //

  JBI->End();

  char filename_json[50];
  snprintf_P(filename_json, sizeof(filename_json), "/lgt_%S.json", GetModuleName());

  pCONT_mfile->JSONFile_Save(filename_json, JBI->GetBuffer(), JBI->GetBufferLength());

  JBI->ReleaseLock();




}


/********************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************
************* Generate colour values that are maps ******************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************
*******************************************************************************************************************/

/**
 * @brief future global way to map colours using palette
 * 
 * Option in the future to apply different colours to the map.
 * 
 * Global temp colours reporting should have flag otpion to pick between full/adjusted brightness inside primary unified sensor.
 * I should still produce them all for the sub mqtt colours
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
RgbColor mInterfaceLight::GetColourValueUsingMaps_AdjustedBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

    // Generate Hue and Brt values
    if(value<20){
      hue = 240;
      brt = 10;
    }else
    if((value>=20)&&(value<50)){
      hue = mSupport::mapfloat(value, 20,50, 180,0);
      brt = mSupport::mapfloat(value, 20,50, 10,100);
    }else
    if((value>=50)&&(value<60)){      
      hue = mSupport::mapfloat(value, 50,60, 359,345);
      brt = 100;
    }else
    if(value>=60){
      hue = 340;
      brt = 100;
    }

    RgbColor colour = HsbColor(HUE_N2F(hue),1.0f,BRT_N2F(brt));

    // colour = RgbColor((int)value, 2, 3);

    return colour;

}

/**
 * @brief maximum sat/brt, hue changes
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
RgbColor mInterfaceLight::GetColourValueUsingMaps_FullBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

    // Generate Hue and Brt values
    if(value<20){
      hue = 240;
      brt = 100;
    }else
    if((value>=20)&&(value<50)){
      hue = mSupport::mapfloat(value, 20,50, 180,0);
      brt = mSupport::mapfloat(value, 20,50, 10,100);
    }else
    if((value>=50)&&(value<60)){      
      hue = mSupport::mapfloat(value, 50,60, 359,345);
      brt = 100;
    }else
    if(value>=60){
      hue = 340;
      brt = 100;
    }

    RgbColor colour = HsbColor(HUE_N2F(hue),1.0f,1.0f);

    // colour = RgbColor((int)value, 2, 3);

    return colour;

}



#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

/*********************************************************************************************\
 * Change scales from 8 bits to 10 bits and vice versa
\*********************************************************************************************/
// 8 to 10 to 8 is garanteed to give the same result
uint16_t mInterfaceLight::change8to10(uint8_t v) {
  return mapvalue(v, 0, 255, 0, 1023);
}
// change from 10 bits to 8 bits, but any non-zero input will be non-zero
uint8_t mInterfaceLight::change10to8(uint16_t v) {
  return (0 == v) ? 0 : mapvalue(v, 4, 1023, 1, 255);
}

/*********************************************************************************************\
 * Gamma correction
\*********************************************************************************************/
// compute actual PWM min/max values from DimmerRange
// must be called when DimmerRange is changed or LedTable
void mInterfaceLight::LightCalcPWMRange(void) {
  uint16_t _pwm_min=0, _pwm_max=1023;

  _pwm_min = change8to10(0);//DimmerToBri(tkr_set->Settings.dimmer_hw_min));   // default 0
  _pwm_max = change8to10(255);//DimmerToBri(tkr_set->Settings.dimmer_hw_max));   // default 100
  // if (tkr_set->Settings.light_correction) {
    _pwm_min = ledGamma10_10(_pwm_min);       // apply gamma correction
    _pwm_max = ledGamma10_10(_pwm_max);       // 0..1023
  // }
  _pwm_min = _pwm_min > 0 ? mapvalue(_pwm_min, 1, 1023, 1, tkr_set->Settings.pwm_range) : 0;  // adapt range but keep zero and non-zero values
  _pwm_max = mapvalue(_pwm_max, 1, 1023,  1, tkr_set->Settings.pwm_range);  // _pwm_max cannot be zero

    pwm_min = _pwm_min;
    pwm_max = _pwm_max;
  // AddLog_P2(LOG_LEVEL_DEV_TEST, PSTR("LightCalcPWMRange %d %d - %d %d"), settings.dimmer_hw_min, settings.dimmer_hw_max, pwm_min, pwm_max);
}
// Calculate the gamma corrected value for LEDS
uint16_t mInterfaceLight::ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (v <= to_src) {
      return mapvalue(v, from_src, to_src, from_gamma, to_gamma);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}
// Calculate the reverse gamma value for LEDS
uint16_t mInterfaceLight::ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr) {
  uint16_t from_src = 0;
  uint16_t from_gamma = 0;

  for (const gamma_table_t *gt = gt_ptr; ; gt++) {
    uint16_t to_src = gt->to_src;
    uint16_t to_gamma = gt->to_gamma;
    if (vg <= to_gamma) {
      return mapvalue(vg, from_gamma, to_gamma, from_src, to_src);
    }
    from_src = to_src;
    from_gamma = to_gamma;
  }
}

// 10 bits in, 10 bits out
uint16_t mInterfaceLight::ledGamma10_10(uint16_t v) {
  return ledGamma_internal(v, gamma_table);
}
// 10 bits resolution, 8 bits in
uint16_t mInterfaceLight::ledGamma10(uint8_t v) {
  return ledGamma10_10(change8to10(v));
}

// Legacy function
uint8_t mInterfaceLight::ledGamma(uint8_t v) {
  return change10to8(ledGamma10(v));
}

// Just apply basic Gamma to each channel
void mInterfaceLight::calcGammaMultiChannels(uint16_t cur_col_10[5]) {
  // Apply gamma correction for 8 and 10 bits resolutions, if needed
  // if (tkr_set->Settings.light_settings.light_correction) {
    for (uint32_t i = 0; i < 5; i++) {
      cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
    }
  // }
}

void mInterfaceLight::calcGammaBulbs(uint16_t cur_col_10[5]) {
//   // Apply gamma correction for 8 and 10 bits resolutions, if needed

//   // First apply combined correction to the overall white power
//   if ((LST_COLDWARM == subtype) || (LST_RGBCW == subtype)) {
//     // channels for white are always the last two channels
//     uint32_t cw1 = subtype - 1;       // address for the ColorTone PWM
//     uint32_t cw0 = subtype - 2;       // address for the White Brightness PWM
//     uint16_t white_bri10 = cur_col_10[cw0] + cur_col_10[cw1];   // cumulated brightness
//     uint16_t white_bri10_1023 = (white_bri10 > 1023) ? 1023 : white_bri10;    // max 1023

// #ifdef ESP8266
//     if (
//       // (MODULE_PHILIPS__ID == tkr_set->my_module_type) || 
//       (tkr_set->Settings.flag4.pwm_ct_mode)) {   // channel 1 is the color tone, mapped to cold channel (0..255)
//       // Xiaomi Philips bulbs follow a different scheme:
//       cur_col_10[cw1] = getCT10bits();
//       // channel 0=intensity, channel1=temperature
//       if (tkr_set->Settings.light_settings.light_correction) { // gamma correction
//         cur_col_10[cw0] = ledGamma10_10(white_bri10_1023);    // 10 bits gamma correction
//       } else {
//         cur_col_10[cw0] = white_bri10_1023;  // no gamma, extend to 10 bits
//       }
//     } else
// #endif  // ESP8266
//     if (tkr_set->Settings.light_settings.light_correction) {
//       // if sum of both channels is > 255, then channels are probably uncorrelated
//       if (white_bri10 <= 1031) {      // take a margin of 8 above 1023 to account for rounding errors
//         // we calculate the gamma corrected sum of CW + WW
//         uint16_t white_bri_gamma10 = ledGamma10_10(white_bri10_1023);
//         // then we split the total energy among the cold and warm leds
//         cur_col_10[cw0] = mapvalue(cur_col_10[cw0], 0, white_bri10_1023, 0, white_bri_gamma10);
//         cur_col_10[cw1] = mapvalue(cur_col_10[cw1], 0, white_bri10_1023, 0, white_bri_gamma10);
//       } else {
//         cur_col_10[cw0] = ledGamma10_10(cur_col_10[cw0]);
//         cur_col_10[cw1] = ledGamma10_10(cur_col_10[cw1]);
//       }
//     }
//   }

//   if (tkr_set->Settings.light_settings.light_correction) {
//     // then apply gamma correction to RGB channels
//     if (LST_RGB <= subtype) {
//       for (uint32_t i = 0; i < 3; i++) {
//         cur_col_10[i] = ledGamma10_10(cur_col_10[i]);
//       }
//     }
//     // If RGBW or Single channel, also adjust White channel
//     if ((LST_SINGLE == subtype) || (LST_RGBW == subtype)) {
//       cur_col_10[subtype - 1] = ledGamma10_10(cur_col_10[subtype - 1]);
//     }
//   }
}

bool mInterfaceLight::isChannelGammaCorrected(uint32_t channel) {
  // if (!tkr_set->Settings.light_settings.light_correction) { return false; }   // Gamma correction not activated
  // if (channel >= tkr_anim->subtype) { return false; }     // Out of range
#ifdef ESP8266
//   if (
//     // (MODULE_PHILIPS__ID == tkr_set->my_module_type) || 
//   (tkr_set->Settings.flag4.pwm_ct_mode)) {
//     if ((LST_COLDWARM == subtype) && (1 == channel)) { return false; }   // PMW reserved for CT
//     if ((LST_RGBCW == subtype) && (4 == channel)) { return false; }   // PMW reserved for CT
//   }
#endif  // ESP8266
  return true;
}


// Calculate the Gamma correction, if any, for fading, using the fast Gamma curve (10 bits in+out)
uint16_t mInterfaceLight::fadeGamma(uint32_t channel, uint16_t v) {
  if (isChannelGammaCorrected(channel)) {
    return ledGamma_internal(v, gamma_table_fast);
  } else {
    return v;
  }
}
uint16_t mInterfaceLight::fadeGammaReverse(uint32_t channel, uint16_t vg) {
  if (isChannelGammaCorrected(channel)) {
    return ledGammaReverse_internal(vg, gamma_table_fast);
  } else {
    return vg;
  }
}

#endif // ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION


bool mInterfaceLight::CommandGet_LightPowerState()
{
  return 
  light_power_state
  // getBri_Global() 
  ? true : false;
}

#ifdef USE_MODULE_CORE_RULES
void mInterfaceLight::RulesEvent_Set_Power()
{

  ALOG_TST(PSTR("MATCHED RulesEvent_Set_Power"));

  uint8_t index = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;
  uint8_t state = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];

  bool get_state = CommandGet_LightPowerState(); 

  ALOG_TST(PSTR("CommandGet_LightPowerState() = %d"), get_state);

  // get state
  ModifyStateNumberIfToggled(&state, CommandGet_LightPowerState());

  // apply invert if needed
  CommandSet_LightPowerState(state);

}
#endif // USE_MODULE_CORE_RULES



void mInterfaceLight::BusManager_Create_DefaultSingleNeoPixel()
{

  const uint16_t _STRIP_SIZE_MAX = 100;
  const uint8_t defDataPins[] = {4};
  const uint16_t defCounts[] = {_STRIP_SIZE_MAX};
  const uint8_t defNumBusses = ((sizeof defDataPins) / (sizeof defDataPins[0]));
  const uint8_t defNumCounts = ((sizeof defCounts)   / (sizeof defCounts[0]));

  DEBUG_PRINTF("defDataPins %d, defCounts %d, defNumBusses %d, defNumCounts %d \n\r", defDataPins[0], defCounts[0], defNumBusses, defNumCounts);

  uint16_t prevLen = 0;
  for (uint8_t i = 0; i < defNumBusses && i < WLED_MAX_BUSSES; i++) 
  {
    uint8_t defPin[] = {defDataPins[i]};
    uint16_t start = prevLen;
    uint16_t count = defCounts[(i < defNumCounts) ? i : defNumCounts -1];
    prevLen += count;
    BusConfig defCfg = BusConfig(DEFAULT_LED_TYPE, defPin, start, count);
    if(bus_manager->add(defCfg) == -1) 
    {
      ALOG_ERR(PSTR("bus_manager->add(defCfg) == -1"));
      break;
    }
  }

}


void mInterfaceLight::BusManager_Create_DefaultSinglePWM_5CH()
{

  const uint16_t _STRIP_SIZE_MAX = 100;
  const uint8_t defDataPins[] = {15,13,12,14,4};
  const uint16_t defCounts[] = {_STRIP_SIZE_MAX};
  const uint8_t defNumBusses = ((sizeof defDataPins) / (sizeof defDataPins[0]));
  const uint8_t defNumCounts = ((sizeof defCounts)   / (sizeof defCounts[0]));

  DEBUG_PRINTF("defDataPins %d, defCounts %d, defNumBusses %d, defNumCounts %d \n\r", defDataPins[0], defCounts[0], defNumBusses, defNumCounts);

  uint16_t prevLen = 0;
  for (uint8_t i = 0; i < defNumBusses && i < WLED_MAX_BUSSES; i++) 
  {
    uint8_t defPin[] = {defDataPins[i]};
    uint16_t start = prevLen;
    uint16_t count = defCounts[(i < defNumCounts) ? i : defNumCounts -1];
    prevLen += count;


    BusConfig defCfg = BusConfig(BUSTYPE_ANALOG_4CH, defPin, start, count);
    if(bus_manager->add(defCfg) == -1) 
    {
      ALOG_ERR(PSTR("bus_manager->add(defCfg) == -1"));
      break;
    }else{
      ALOG_DBG(PSTR("bus_manager->add(defCfg) "));
    }
  }

}



/**
 * @brief Testing that each object passed into here is a new busconfig
 * 
 * @param obj 
 */
void mInterfaceLight::parseJSONObject__BusConfig(JsonParserObject obj)
{
  JsonParserToken jtok = 0;
  JsonParserToken jtok2 = 0;

  DEBUG_LINE_HERE;

  // busConfigs
  uint8_t bus_count = 0;//bus_manager->getNumBusses();
  for(uint8_t ii=0;ii<(WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES);ii++)
  {
    if (busConfigs[ii] != nullptr)
    {
      bus_count++;
    }
  }

  ALOG_DBM(PSTR("getNumBusses %d"), bus_count);

  uint16_t start = 0;
  uint16_t length = 10;
  int8_t bus_type = BUSTYPE_NONE;
  uint8_t reversed = 0;
  uint8_t ColourOrder = 0;//{COLOUR_ORDER_INIT_DISABLED};
  uint8_t pins[5] = {255}; // 255 is unset

  if(jtok2 = obj["Pin"])
  {
    if(jtok2.isNum())
    {
      pins[0] = jtok2.getInt();
    }
    else
    if(jtok2.isArray())
    {
      uint8_t ii = 0;
      JsonParserArray arrobj = jtok2;
      for(auto value : arrobj) 
      {
        pins[ii++] = value.getInt();
      }
    }
    AddLog_Array(LOG_LEVEL_INFO, PSTR("pins"), pins, 5);  
  }


  if(jtok = obj["Start"])
  {
    start = jtok.getInt();
    ALOG_INF(PSTR("start %d"), start);
  }


  if(jtok = obj["Length"])
  {
    length = jtok.getInt();
    ALOG_INF(PSTR("length %d"), length);
  }


  if(jtok = obj["BusType"])
  {
    if(jtok.isInt())
    {
      bus_type = jtok.getInt();
    }
    else
    if(jtok.isStr())
    {
      bus_type = Bus::getTypeIDbyName(jtok.getStr());
    }
    ALOG_INF(PSTR("bus_type %d"), bus_type);
  }
  

  if(jtok = obj[PM_RGB_COLOUR_ORDER])
  {
    if(jtok.isStr())
    {
      ColourOrder = GetColourOrder_FromName(jtok.getStr());
    }
  }


  if(jtok = obj["Reversed"])
  {
    reversed = jtok.getInt();
    ALOG_INF(PSTR("reversed %d"), reversed);
  }


  uint8_t bus_index = bus_count; // next bus space 
  if (busConfigs[bus_index] != nullptr) delete busConfigs[bus_index];


  ALOG_HGL(PSTR("============BusConfig(type%d,pin0=%d,start%d,len%d,CO%d)"),
    bus_type,
    pins[0],
    start,
    length,
    ColourOrder
  );


  #ifdef ENABLE_DEVFEATURE_LIGHTING__DOUBLE_BUFFER

  DEBUG_LINE_HERE;
  busConfigs[bus_index] = new BusConfig(
    bus_type, 
    pins, 
    start, 
    length,
    ColourOrder,
    reversed, 
    0, // skip 
    RGBW_MODE_MANUAL_ONLY, // autowhite
    0, // clock
    true // double buffer
  );    

  #else

  DEBUG_LINE_HERE;
  busConfigs[bus_index] = new BusConfig(
    bus_type, 
    pins, 
    start, 
    length,
    ColourOrder,
    reversed, 
    0, 
    RGBW_MODE_MANUAL_ONLY
  );    
  
  #endif

  DEBUG_LINE_HERE;
  tkr_anim->doInitBusses = true; // adds checks

  ALOG_DBG( PSTR("mInterfaceLight::parseJSONObject__BusConfig Finished"));
  DEBUG_LINE_HERE;

}


#ifdef ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER

#include <ctype.h>  // for toupper

uint8_t mInterfaceLight::GetColourOrder_FromName(const char* c)
{

    uint8_t colour_order = 0;  // Initialize as 0 (default to RGB with no whites)

    // Validate length (must be 3 to 5 characters)
    size_t len = strlen(c);
    Serial.println("len");
    Serial.println(len);
    if (!c || len < 3 || len > 5) {
        ALOG_ERR(PSTR("INVALID Length"));
        return colour_order;
    }

    // Convert all characters to uppercase to simplify checks
    char chars[5] = {0};  // Array to hold converted characters
    for (size_t i = 0; i < len; i++) {
        chars[i] = toupper(c[i]);
    }

    // Check RGB order from the first three characters
    if (chars[0] == 'R' && chars[1] == 'G' && chars[2] == 'B') {
        colour_order = 0x00; // RGB
    } else if (chars[0] == 'G' && chars[1] == 'R' && chars[2] == 'B') {
        colour_order = 0x01; // GRB
    } else if (chars[0] == 'B' && chars[1] == 'R' && chars[2] == 'G') {
        colour_order = 0x02; // BRG
    } else if (chars[0] == 'B' && chars[1] == 'G' && chars[2] == 'R') {
        colour_order = 0x03; // BGR
    } else if (chars[0] == 'G' && chars[1] == 'B' && chars[2] == 'R') {
        colour_order = 0x04; // GBR
    } else if (chars[0] == 'R' && chars[1] == 'B' && chars[2] == 'G') {
        colour_order = 0x05; // RBG
    } else {
        ALOG_ERR(PSTR("INVALID RGB Order"));
        return colour_order;  // Invalid RGB order
    }

    // Check for optional white channel settings (4th and 5th characters)
    if (len >= 4) {
        if (chars[3] == 'W') {
            if (len == 4) {
                colour_order |= 0x20;  // Only WW (Warm White)
            } else if (chars[4] == 'C') {
                colour_order |= 0x40;  // Both WW and CW
            }
        } else if (chars[3] == 'C') {
            if (len == 4) {
                colour_order |= 0x10;  // Only CW (Cold White)
            }
        }
    }

    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_ERR(PSTR("colour_order == \n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r%X"), colour_order);
    #endif  

    return colour_order;
}


#else


uint8_t mInterfaceLight::GetColourOrder_FromName(const char* c)
{

  uint8_t colour_order = {COLOUR_ORDER_INIT_DISABLED};

  if(!c){ return colour_order; }
  if(strlen(c)<=5){
    ALOG_DBM( PSTR("Valid Length"));
  }else{
    ALOG_INF(PSTR("INVALID Length"));
    return colour_order;
  }

  for(uint8_t index=0;index<strlen(c);index++)
  {
    if((c[index]=='R')||(c[index]=='r')){
      colour_order.red = index;
    }else
    if((c[index]=='G')||(c[index]=='g')){
      colour_order.green = index;
    }else
    if((c[index]=='B')||(c[index]=='b')){
      colour_order.blue = index;
    }else
    if((c[index]=='C')||(c[index]=='c')){
      colour_order.white_cold = index;
    }else
    if((c[index]=='W')||(c[index]=='w')){
      colour_order.white_warm = index;
    }
  }

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  ALOG_INF( PSTR("colour_order == R=%d, G=%d, B=%d, CW=%d, WW=%d, dec%d, %X"),
    colour_order.red,
    colour_order.green,
    colour_order.blue,
    colour_order.white_cold,
    colour_order.white_warm,
    colour_order.data,
    colour_order.data
  );
  #endif  

  return colour_order;

}

#endif // ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER


/******************************************************************************************************************
 * mInterfaceLight_Commands.cpp
*******************************************************************************************************************/


void mInterfaceLight::parse_JSONCommand(JsonParserObject obj)
{

  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  /**
   * @brief Bus needs to be configured first, and probably built immediately from within here so plattes etc next are added properly, otherwise EVERY_LOOP would have to happen
   **/
  if(jtok = obj["BusConfig"])
  { 
    if(jtok.isArray())
    {      
      ALOG_INF(PSTR("BusConfig **************  \t%d"), jtok.getType());
      JsonParserArray arrobj = jtok;      
      for(auto value : arrobj) 
      {
        parseJSONObject__BusConfig(value.getObject());
      }
    }
  }

  /**
   * @brief Master (previously global) shall control the final output, but per segment within animator can exist. 
   * Ie the true colour out is 
   * 
   * SetBrightnessOutput <= scale8( scale8(Raw255Colour, SegmentBrightness), MasterBrightness) 
   * 
   */
  if(jtok = obj[PM_BRIGHTNESS]){ // Range 0-100
    float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
    CommandSet_Brt_255( (uint8_t)value );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS)), getBri_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_255]){ // Range 0-255
    CommandSet_Brt_255( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS)), getBri_Global());
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_BRIGHTNESS_RGB]){ // Range 0-100
    float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
    CommandSet_Global_BrtRGB_255( (uint8_t)value );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), getBriRGB_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_RGB_255]){ // Range 0-255
    CommandSet_Global_BrtRGB_255( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB_255)), getBriRGB_Global());
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_BRIGHTNESS_CCT]){ // Range 0-100
    float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
    CommandSet_Global_BrtCCT_255( (uint8_t)value );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT)), getBriCCT_Global());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_CCT_255]){ // Range 0-255
    CommandSet_Global_BrtCCT_255( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT_255)), getBriCCT_Global());
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_LIGHTPOWER])
  {
    int8_t state = 0;
    if(jtok.isStr()){
      state = pCONT_sup->GetStateNumber(jtok.getStr());
    }else
    if(jtok.isNum()){
      state = jtok.getInt(); 
    }
    ModifyStateNumberIfToggled(&state, light_power_state);
    CommandSet_LightPowerState(state);
    ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_LIGHTPOWER)), light_power_state);
  }

  if(jtok = obj[PM_LIGHT].getObject()[PM_TIME_ON]){ // default to secs
    CommandSet_Auto_Time_Off_Secs(jtok.getInt());
    ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_TIME_ON)), auto_off_settings.time_decounter_secs ); 
    data_buffer.isserviced++;
  }

  ALOG_DBM(PSTR("void mInterfaceLight::parse_JSONCommand(JsonParserObject obj)======================"));
  
}


void mInterfaceLight::CommandSet_Auto_Time_Off_Secs(uint16_t value){    
  auto_off_settings.time_decounter_secs = value;
}



void mInterfaceLight::CommandSet_LightPowerState(uint8_t state)
{

  ALOG_INF( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_LIGHTPOWER)), light_power_state);

  if(state == LIGHT_POWER_STATE_OFF_ID) // turn off
  {
    // CommandSet_Animation_Transition_Rate_Ms(10000);
    pSEGMENT_I(0).intensity = 255;    
    pSEGMENT_I(0).single_animation_override.time_ms =  pSEGMENT_I(0).single_animation_override_turning_off.time_ms; // slow turn on
    ALOG_INF(PSTR("Setting override for off %d"), pSEGMENT_I(0).single_animation_override.time_ms);
    tkr_anim->force_update();
    pSEGMENT_I(0).effect_anim_section = 0; // for effects that are only generated once, we need to trigger it again to make the brightness dim down
    CommandSet_Brt_255(0);    
  }
  else
  if(state == 1) // turn on
  {

    // CommandSet_Animation_Transition_Time_Ms(1000);

    pSEGMENT_I(0).single_animation_override.time_ms = 1000; // slow turn on
    tkr_anim->force_update();


    // CommandSet_Animation_Transition_Rate_Ms(1000);
    // CommandSet_LightsCountToUpdateAsPercentage(100);
    
    CommandSet_Brt_255(255);

    //make sure both are set
    // CommandSet_Global_BrtRGB_255(255);
    // CommandSet_Global_BrtCCT_255(255);
    
    // CommandSet_PaletteID(10, 0);
    
    // CommandSet_Flasher_FunctionID(0 /**Add define later for "DEFAULT_EFFECT" */);//EFFECTS_FUNCTION__SOLID_COLOUR__ID);



  }

}


void mInterfaceLight::CommandSet_Brt_255(uint8_t brt_new){
    
  // tkr_anim->SEGMENT_I(0).rgbcct_controller->setBrightness255(brt_new);

  // if(!tkr_anim->segments.size()){ return; } // Global does not rely on segments, and will be called before segments are created

  tkr_anim->force_update();
  
  
 if(tkr_anim->segments.size())
  {
    tkr_anim->SEGMENT_I(0).effect_anim_section = 0; // for effects that are only generated once, we need to trigger it again to make the brightness dim down
  }
   setBriRGB_Global(brt_new);
  // probably needs to check if they are linked here, or internally
  setBriCT_Global(brt_new);

  
    bus_manager->setBrightness( getBriRGB_Global() ); // fix re-initialised bus' brightness



  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // ALOG_INF(PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_BRIGHTNESS)), tkr_anim->SEGMENT_I(0).rgbcct_controller->getBrightness255());
  // #endif // ENABLE_LOG_LEVEL_COMMANDS
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtRGB *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_Global_BrtRGB_255(uint8_t bri, uint8_t segment_index)
{
  // if(!tkr_anim->segments.size()){ return; } // Global does not rely on segments, and will be called before segments are created

  // SEGMENT_I(segment_index).rgbcct_controller->setBrightnessRGB255(bri);
 tkr_anim->force_update();
 
 if(tkr_anim->segments.size())
  {
    tkr_anim->SEGMENT_I(0).effect_anim_section = 0; // for effects that are only generated once, we need to trigger it again to make the brightness dim down
  }

  _briRGB_Global = bri;
  setBriRGB_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // ALOG_INF(PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_BRIGHTNESS)), SEGMENT_I(segment_index).rgbcct_controller->getBrightnessRGB());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** BrtCCT *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

void mInterfaceLight::CommandSet_Global_BrtCCT_255(uint8_t bri, uint8_t segment_index) 
{
  // if(!tkr_anim->segments.size()){ return; }
  tkr_anim->force_update();
  
 if(tkr_anim->segments.size())
  {
    tkr_anim->SEGMENT_I(0).effect_anim_section = 0; // for effects that are only generated once, we need to trigger it again to make the brightness dim down
  }

  setBriCT_Global(bri);
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // ALOG_INF(PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).rgbcct_controller->getBrightnessCCT255());
  #endif // ENABLE_LOG_LEVEL_COMMANDS
}



/******************************************************************************************************************
 * mInterfaceLight_Web.cpp
*******************************************************************************************************************/



/******************************************************************************************************************
 * mInterfaceLight_ConstructJSON.cpp
*******************************************************************************************************************/


uint8_t mInterfaceLight::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JBI->Start();

  // JBI->Add_P(PM_TYPE, tkr_set->Settings.light_settings.type);

  // // JBI->Add_P(PM_PIXELS_UPDATE_PERCENTAGE, animation.transition.pixels_to_update_as_percentage);
  // #ifdef USE_MODULE_LIGHTS_ANIMATOR
  // JBI->Add_P(PM_PIXELS_UPDATE_NUMBER, tkr_anim->SEGMENT_I(0).transition.pixels_to_update_as_number);
  // #endif // USE_MODULE_LIGHTS_ANIMATOR

  return JBI->End();

}

/**
 * @brief Ovverview of what is being show regardless of animation/hardware.
 * I should be able to look here and get values needed for openhab/integration with other software
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mInterfaceLight::ConstructJSON_State(uint8_t json_level, bool json_appending){

  // Active rgbcct palette used as scene

  char buffer[30];
  
  JBI->Start();  

    #ifdef ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT
    mqtthandler__state__ifchanged.tRateSecs = 1; // force this to be 1 second for this debug message
    JBI->Array_Start("PWM_Channels_Read");
    for (uint8_t i = 0; i < 5; i++) {
      if (pCONT_pins->PinUsed(GPIO_PWM1_ID, i)) 
      {
        JBI->Add(analogRead(pCONT_pins->GetPin(GPIO_PWM1_ID, i)));
      }
    }
    JBI->Array_End();
    #endif  // ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT


  return JBI->End();

}


#ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
uint8_t mInterfaceLight::ConstructJSON_Debug_Module_Config(uint8_t json_level, bool json_appending){

  char buffer[30];
  
  JBI->Start();

  JBI->Object_Start("RgbcctController");
  
    // JBI->Object_Start("raw");
    //   JBI->Add("R", rgbcct_controller.R); 
    //   JBI->Add("G", rgbcct_controller.G); 
    //   JBI->Add("B", rgbcct_controller.B); 
    //   JBI->Add("WW", rgbcct_controller.WW); 
    //   JBI->Add("WC", rgbcct_controller.WC); 
    // JBI->Object_End();
    JBI->Object_Start("type");
    
    JBI->Add("R", tkr_anim->SEGMENT_I(0).hardware_element_colour_order.r); 
    JBI->Add("G", tkr_anim->SEGMENT_I(0).hardware_element_colour_order.g); 
    JBI->Add("B", tkr_anim->SEGMENT_I(0).hardware_element_colour_order.b); 
    JBI->Add("WW", tkr_anim->SEGMENT_I(0).hardware_element_colour_order.w); 
    JBI->Add("WC", tkr_anim->SEGMENT_I(0).hardware_element_colour_order.c); 


    JBI->Object_End();

    // JBI->Add("mPaletteI->active_scene_palette_id",mPaletteI->active_scene_palette_id);

  JBI->Object_End();


  if(tkr_anim->SEGMENT_I(0).palette_id == mPalette::PALETTELIST_VARIABLE_GENERIC_01__ID)
  {

    JBI->Array_AddArray("encoded", tkr_set->Settings.animation_settings.palette_encoded_users_colour_map, ARRAY_SIZE(tkr_set->Settings.animation_settings.palette_encoded_users_colour_map));

  }


  // JBI->Object_Start("singlecolour");
  //   JBI->Add_P(PM_R", mode_singlecolour.colour.R);
  //   JBI->Add_P(PM_G", mode_singlecolour.colour.G);
  //   JBI->Add_P(PM_B", mode_singlecolour.colour.B);
  //   JBI->Add_P(PM_WW", mode_singlecolour.colour.WW);
  //   JBI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  //   JBI->Add_P(PM_WC", mode_singlecolour.colour.WC);
  // JBI->Object_End();
  // JBI->Object_Start("active_rgbcct_colour_p");
  //   JBI->Add_P(PM_R", active_rgbcct_colour_p->R);
  //   JBI->Add_P(PM_G", active_rgbcct_colour_p->G);
  //   JBI->Add_P(PM_B", active_rgbcct_colour_p->B);
  //   JBI->Add_P(PM_WW", active_rgbcct_colour_p->WW);
  //   JBI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  //   JBI->Add_P(PM_WC", active_rgbcct_colour_p->WC);
  // JBI->Object_End();

  // JBI->Array_AddArray("singlecolour", current_color, 5);   

  // JBI->Object_Start("fade");
  //   JBI->Add("running", fade.running);
  // JBI->Object_End();

  return JBI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG

#ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
uint8_t mInterfaceLight::ConstructJSON_Debug__BusConfig(uint8_t json_level, bool json_appending)
{

  JBI->Start();

  for(uint8_t bus_i = 0; bus_i < bus_manager->getNumBusses(); bus_i++)
  {
    JBI->Object_Start_F("Bus%d", bus_i);

      JBI->Add("getLength", bus_manager->busses[bus_i]->getLength());
      JBI->Add("s", bus_manager->busses[bus_i]->_start);
      JBI->Add("l", bus_manager->busses[bus_i]->_len);

      uint8_t colour_order = bus_manager->busses[bus_i]->getColorOrder();
      JBI->Array_Start("CO");
        JBI->Add(GetColourOrderString(colour_order).c_str());// colour_order.red);
        // JBI->Add(colour_order.green);
        // JBI->Add(colour_order.blue);
        // JBI->Add(colour_order.white_cold);
        // JBI->Add(colour_order.white_warm);
      JBI->Array_End();

      JBI->Add("getType", (uint8_t)bus_manager->busses[bus_i]->getType());

      JBI->Add("getTypeName", bus_manager->busses[bus_i]->getTypeName());

      if(IS_BUSTYPE_DIGITAL(bus_manager->busses[bus_i]->getType()))
      {
        JBI->Add("interfaceType", bus_manager->busses[bus_i]->getInterfaceType()); 
      }

      uint8_t pins[5] = {0};
      uint8_t pin_count = 0;
      pin_count = bus_manager->busses[bus_i]->getPins(pins);
      JBI->Array_Start("p");
      // JBI->Add(pin_count);
      for(uint8_t ii=0;ii<pin_count;ii++)
      {
        JBI->Add(pins[ii]);
      }
      JBI->Array_End();

    JBI->Object_End();
  }

  return JBI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG


/******************************************************************************************************************
 * mInterfaceLight_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mInterfaceLight::MQTTHandler_Init()
{

  struct handler<mInterfaceLight>* ptr;  

  ptr = &mqtthandler__settings__teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler__state__ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);
  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
  ptr = &mqtthandler__debug_module_config__teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Debug_Module_Config;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG

  
  #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
  ptr = &mqtthandler__debug_bus_config__teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_BUS_CONFIG__CTR;
  ptr->ConstructJSON_function = &mInterfaceLight::ConstructJSON_Debug__BusConfig;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG


} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mInterfaceLight::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mInterfaceLight::MQTTHandler_Rate()
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
void mInterfaceLight::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}
  
#ifdef USE_MODULE_NETWORK_WEBSERVER
#ifdef ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
void mInterfaceLight::MQTTHandler_AddWebURL_PayloadRequests()
{    
  CODE_BLOCK__MQTTHandler_AddWebURL_PayloadRequests();
}
#endif // ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
#endif // USE_MODULE_NETWORK_WEBSERVER

#endif// USE_MODULE_NETWORK_MQTT


#endif // USE_DRIVER
