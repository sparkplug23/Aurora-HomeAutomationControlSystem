#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
const char* mAnimatorLight::PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR = D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;


int8_t mAnimatorLight::Tasker(uint8_t function, JsonParserObject obj)
{

  #ifdef ENABLE_DEVFEATURE_Tasker_Override_Forcing_Variables_Testing
  Tasker_Override_Forcing_Variables_Testing();
  #endif // ENABLE_DEVFEATURE_FLICKERING_TEST5

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();  
      break;
    case FUNC_INIT:
      Init();
      break;
  }
  
  if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}
  
  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    // case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
    //   Settings_Load();
    // break;
    // case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   Settings_Save();
    // break;
    // case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    // case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   Settings_Default();
    // break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:{

      #ifdef ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812

      // RgbcctColor updatedColor = pCONT_lAni->stripbus->GetPixelColor(0);

      // ALOG_INF( PSTR("colour_type=%d, desired_colour1=%d,%d,%d,%d,%d AR=%d"),SEGMENT.colour_type,updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW, analogRead(4));
      // ALOG_INF( PSTR("colour_type=%d, desired_colour1=%d,%d,%d,%d,%d AR=%d"),SEGMENT.colour_type,updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW);

      // StripUpdate();



      #endif
    
      //EverySecond();

      // const NeoRgbcctCurrentSettings settings(200,200,200,201,202);
      // uint32_t maTotal = stripbus->CalcTotalMilliAmpere(settings);

      // #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
      //   char buffer[50];
      //   AddLog(LOG_LEVEL_TEST, PSTR("GetEffectsModeNamebyID=%s"), 
      //     pCONT_lAni->GetEffectsModeNamebyID(mEffects->getMode(),buffer,sizeof(buffer))
      //   );
      // #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

      // AddLog_Array(LOG_LEVEL_WARN, "colourmap0", mPaletteI->palettelist.rgbcct_users[0].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap1", mPaletteI->palettelist.rgbcct_users[1].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap2", mPaletteI->palettelist.rgbcct_users[2].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap3", mPaletteI->palettelist.rgbcct_users[3].colour_map_id, 5);
      // AddLog_Array(LOG_LEVEL_WARN, "colourmap4", mPaletteI->palettelist.rgbcct_users[4].colour_map_id, 5);



// ALOG_INF( PSTR("brt = %d"), pCONT_iLight->getBriRGB_Global() );



    }break;
    case FUNC_LOOP: 
      EveryLoop();
    break;    
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
     parse_JSONCommand(obj);
    break;
    case FUNC_WIFI_CONNECTED:
      #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
      Test_Config();
      #endif
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
      CommandSet_Physical_UserInput_Buttons();
      #endif
    break;
    /************
     * MQTT SECTION * 
    *******************/   
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }// switch(command)

  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END FUNCTION




/*******************************************************************************************************************
********************************************************************************************************************
************ START OF GENERAL INIT PROCESS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

void mAnimatorLight::Pre_Init(void){ 
  
  // Allow runtime changes of animation size
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX; 
  pCONT_set->Settings.flag_animations.clear_on_reboot = false; //flag

  /**
   * @brief Get pins here
   **/
  #ifdef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

    int8_t pin_data = -1, pin_clock = -1;

    // for(
      uint8_t ii=0; 
      // ii<1; ii++)
    // {
      if(pCONT_pins->PinUsed(GPIO_RGB_DATA_ID, ii))
      {
        pin_data = pCONT_pins->GetPin(GPIO_RGB_DATA_ID, ii);
      }

      // if(pCONT_pins->PinUsed(GPIO_RGB_CLOCK1_ID, ii))
      // {
      //   pin_clock = pCONT_pins->GetPin(GPIO_RGB_DATA1_ID, ii);
      // }

      // Stripbus
      if(pin_data != -1)
      {
        AddLog(LOG_LEVEL_INFO, PSTR("NeoPixelBus: Segment[%d] Pin[%d]"), 0, pin_data);
        stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(STRIP_SIZE_MAX, pin_data);
      }

    // }

  #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT


    #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
    pCONT_ladd->neopixel_runner = new NeoPixelShowTask();///* neopixel_runner = nullptr;
    // constexpr
    // uint8_t kTaskRunnerCore = ARDUINO_RUNNING_CORE;//xPortGetCoreID(); // 0, 1 to select core
    uint8_t kTaskRunnerCore = xPortGetCoreID(); // 0, 1 to select core // > 1 to disable separate task
    pCONT_ladd->neopixel_runner->begin(
        [this]() {
            stripbus->Show();
        },
        kTaskRunnerCore
    );
    #endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32


}


#ifndef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT 
void mAnimatorLight::Init_NeoPixelBus(int8_t pin)
{

  // if pixel size changes, free and init again
  uint16_t strip_size_tmp = STRIP_SIZE_MAX;
  #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  strip_size_tmp = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;
  #endif
  // uint16_t strip_size_tmp = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  
  //step1:  moving the desired/starting colours into a buffer type, so it is dynamic
  //step2:  to become its own function, so strips can be changed at runtime


#ifdef ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE

// future methods will need to parse on esp8266 and only permit certain pins, or if very low pixel count, software versions

  stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, PINSET_TEMP_METHOD_RGB_PIN_RGB);//19); 3 = rx0



#else //legacy method



  stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0
  
  #endif

}
#endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT


// #ifdef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
// void mAnimatorLight::Init_NeoPixelBus(int8_t pin)
// {


//   // if pixel size changes, free and init again
//   uint16_t strip_size_tmp = STRIP_SIZE_MAX;
//   #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
//   strip_size_tmp = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;
//   #endif
//   // uint16_t strip_size_tmp = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;//pCONT_iLight->settings.light_size_count<STRIP_SIZE_MAX?pCONT_iLight->settings.light_size_count:STRIP_SIZE_MAX; // Catch values exceeding limit
  
//   /**
//    * @brief 
//    * Populate RGB pin#1 across all pins if only 1 pin is set
//    * Create number of stripbuses based on how many pins are configured
//    **/ 
//   int8_t pin = pCONT_pins->GetPin(GPIO_RGB_DATA1_ID);
//   if(pin != -1)
//   {

//   }


//   #ifdef ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
//   // future methods will need to parse on esp8266 and only permit certain pins, or if very low pixel count, software versions
//   stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, PINSET_TEMP_METHOD_RGB_PIN_RGB);//19); 3 = rx0
//   #else //legacy method
//   stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0
//   #endif


// }
// #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
     



void mAnimatorLight::Init(void){ 
    
  // ledout.length = STRIP_SIZE_MAX; 
  
  pCONT_iLight->settings.light_size_count = STRIP_SIZE_MAX;

  DEBUG_LINE;

  // stripbus->NeoRgbCurrentSettings(1,2,3);

  //step1:  moving the desired/starting colours into a buffer type, so it is dynamic
  //step2:  to become its own function, so strips can be changed at runtime
  // stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(strip_size_tmp, 23);//19); 3 = rx0

  #ifndef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT 
  Init_NeoPixelBus();
  #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT 


  // #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  // mqtthandler_debug_animations_progress.tRateSecs = 1;
  // #endif


  pCONT_iLight->Init_NeoPixelAnimator(1, NEO_ANIMATION_TIMEBASE);  

  // #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
  // // pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val = 100;  
  // SetLEDOutAmountByPercentage(100);//pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);  
  // #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  _segments[0].flags.apply_small_saturation_randomness_on_palette_colours_to_make_them_unique = false;
  _segments[0].flags.Limit_Upper_Brightness_With_BrightnessRGB = false;
  _segments[0].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = false; // FIX
    
  // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  //   flashersettings.brightness_max = 255;
  //   flashersettings.brightness_min = 0;
  // #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

  // #ifdef ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET // move to proper command options
  //   // pCONT_iLight->animation.flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice = true; // FIX
  //   pCONT_iLight->animation.flags.Limit_Upper_Brightness_With_BrightnessRGB = true;
  // #endif // ENABLE_DEVFEATURE_DIRECT_TEMPFIX_RANDOMISE_BRIGHTNESS_ON_PALETTE_GET

  #ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  pixel_group.flags.fEnabled = true;
  pixel_group.flags.multiplier_mode_id = PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
  pixel_group.multiplier = 10;
  pixel_group.mapped_array_data.values = editable_mapped_array_data_array;
  pixel_group.flags.mapped_array_editable_or_progmem = 0;
  pixel_group.mapped_array_data.length = 0;
  // flashersettings.flags.enable_startcolour_as_alternate = true;
  // flashersettings.flags.enable_endingcolour_as_alternate = true;
  #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  
  // for(ledout.index=0;
  //   ledout.index<STRIP_SIZE_MAX;
  //   ledout.index++){ 
  //   ledout.pattern[ledout.index] = ledout.index; 
  // }

  DEBUG_LINE;

  // #ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
  // // Clear stored light output
  // memset(&animation_colours,0,sizeof(animation_colours));
  
  // #endif //ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT


  // Start display

  if(stripbus == nullptr)
  {
    // not configured, no pin? disable lighting
    AddLog(LOG_LEVEL_ERROR, PSTR("stripbus == nullptr, no pin or stripbus set, setting default as temporary fix"));
    settings.flags.EnableModule = false;

    /**
     * @brief 
     * Temp fix to resolve crashing errors due to stripbus being called from interface_light without setting
     * Default values
     */
    #ifdef ESP8266
    stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(STRIP_SIZE_MAX, 3);
    #else
    stripbus = new NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType>(STRIP_SIZE_MAX, 23);
    #endif


  }



  stripbus->Begin();
  if(pCONT_set->Settings.flag_animations.clear_on_reboot){
    stripbus->ClearTo(0);
    pCONT_iLight->ShowInterface();
  }

  #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
    init_Notifications();
  #endif
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  blocking_force_animate_to_complete = true; //animate to completion on boot (for short animations)
  init_Ambilight();
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  init_mixer_defaults();
  #endif

  settings.flags.EnableModule = true;

  Init_Segments();

  #ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];
  PhysicalController_Convert_IDs_to_Internal_Parameters();
  #endif

} //end "init"


void mAnimatorLight::Settings_Load(){
    
  // pCONT_iLight->animation.brightness = pCONT_set->Settings.light_settings.light_brightness_as_percentage/100.0f;
  
  pCONT_iLight->setBriRGB_Global(pCONT_set->Settings.light_settings.light_brightness_as_percentage);

  // uint8_t list = 0;
  // SetPaletteListPtrFromID(list);
  // AddLog(LOG_LEVEL_TEST,PSTR("LOADED %d amount = %d"),list,palettelist.ptr->colour_map_size);
  // for(uint8_t element=0;element<20;element++){
  //   AddLog(LOG_LEVEL_TEST,PSTR("LOADED %d i%d amount = %d"),list,element,pCONT_set->Settings.palette_user_colour_map_ids[list][element]);
  // }

  // Save colour map IDS
  // for(uint8_t list=0;list<20;list++){
  //   SetPaletteListPtrFromID(list);
  //   palettelist.ptr->colour_map_size = pCONT_set->Settings.animation_settings.palette_user_amounts[list]<20?pCONT_set->Settings.animation_settings.palette_user_amounts[list]:20;
  //   for(uint8_t element=0;element<20;element++){
  //     palettelist.ptr->colour_map_id[element] = pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[list][element];
  //   }
  // }
  
  // for(uint8_t list=0;list<20;list++){
  //   memcpy(palettelist_variable_users_ctr,pCONT_set->Settings.animation_settings.palette_user_variable_name_ctr,sizeof(palettelist_variable_users_ctr));
  // }


// these need reversed!!!
  // pCONT_set->Settings.animation_settings.animation_mode               = _segments[0].mode_id;
  // pCONT_set->Settings.animation_settings.animation_palette            = _segments[0].palette.id;
  // pCONT_set->Settings.animation_settings.animation_transition_order   = _segments[0].transition.order_id;
  // pCONT_set->Settings.animation_settings.animation_transition_method  = _segments[0].transition.method_id;
  // pCONT_set->Settings.animation_settings.animation_transition_time_ms = _segments[0].transition.time_ms;
  // pCONT_set->Settings.animation_settings.animation_transition_rate_ms = _segments[0].transition.rate_ms;

#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
  user_input.selected.palette_id = pCONT_set->Settings.animation_settings.xmas_controller_params[0];
  user_input.selected.brightness_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[1];
  user_input.selected.effects_id =  pCONT_set->Settings.animation_settings.xmas_controller_params[2];
  user_input.selected.intensity_id = pCONT_set->Settings.animation_settings.xmas_controller_params[3];
  AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("LOADED user_input.selected.palette_id %d"),user_input.selected.palette_id);
#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


}

void mAnimatorLight::Settings_Save(){
  
  // pCONT_set->Settings.light_settings.light_brightness_as_percentage = pCONT_iLight->animation.brightness*100;

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST,PSTR("SAVED pCONT_iLight->animation.brightness = %d"),pCONT_set->Settings.light_settings.light_brightness_as_percentage);
  // #endif
  
  // Save colour map IDS
  // for(uint8_t list=0;list<20;list++){
  //   SetPaletteListPtrFromID(list);
  //   pCONT_set->Settings.animation_settings.palette_user_amounts[list] = palettelist.ptr->colour_map_size;
  //   for(uint8_t element=0;element<20;element++){
  //     pCONT_set->Settings.animation_settings.palette_user_colour_map_ids[list][element] = palettelist.ptr->colour_map_id[element];
  //   }
  // }

  // for(uint8_t list=0;list<20;list++){
  //   memcpy(pCONT_set->Settings.animation_settings.palette_user_variable_name_ctr,palettelist_variable_users_ctr,sizeof(palettelist_variable_users_ctr));
  // }

  pCONT_set->Settings.animation_settings.animation_mode               = _segments[0].mode_id;
  pCONT_set->Settings.animation_settings.animation_palette            = _segments[0].palette.id;
  pCONT_set->Settings.animation_settings.animation_transition_method  = _segments[0].transition.method_id;
  pCONT_set->Settings.animation_settings.animation_transition_time_ms = _segments[0].transition.time_ms;
  pCONT_set->Settings.animation_settings.animation_transition_rate_ms = _segments[0].transition.rate_ms;

  // pCONT_set->Settings.animation_settings.animation_transition_pixels_to_update_as_percentage = pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val;

  // pCONT_set->Settings.animation_settings.xmas_controller_params[0] = user_input.selected.palette_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[1] = user_input.selected.brightness_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[2] = user_input.selected.effects_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[3] = user_input.selected.intensity_id;
  
#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS

  pCONT_set->Settings.animation_settings.xmas_controller_params[0] = user_input.selected.palette_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[1] = user_input.selected.brightness_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[2] = user_input.selected.effects_id;
  pCONT_set->Settings.animation_settings.xmas_controller_params[3] = user_input.selected.intensity_id;

  // PhysicalController_Convert_IDs_to_Internal_Parameters();

  pCONT_set->Settings.animation_settings.xmas_controller_params[5] = 6;

#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


}



void mAnimatorLight::Settings_Default(){

DEBUG_LINE;
  
  init(); //default values

  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST,PSTR("mAnimatorLight::Settings_Default"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
    settings.flags.TemplateProvidedInProgMem = true;
  #endif
  
  if(settings.flags.TemplateProvidedInProgMem){// use template provided in progmem || override template
  
    //pCONT_iLight->Template_Load();

  }else{
    
    //pCONT_iLight->animation.transition.method_id = TRANSITION_METHOD_BLEND_ID;


    // // Check for user defined defaults
    // #ifdef DEFAULT_LIGHTING_TRANSITION_ORDER_ID
    //   pCONT_iLight->animation.transition.order_id = 6;/*DEFAULT_LIGHTING_TRANSITION_ORDER_ID<TRANSITION_ORDER_LENGTH_ID?
    //                                         DEFAULT_LIGHTING_TRANSITION_ORDER_ID:TRANSITION_ORDER_INORDER_ID;*/
    // #else
    //   pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_INORDER_ID;
    // #endif
    
    // #ifdef DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID
    //   // pCONT_iLight->animation.transition.time_ms.map_id = 10;
    //   /*DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID<TIME_MAP_SECS_LENGTH_ID?
    //                                             DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID:TIME_MAP_SECS_15_ID;*/
    //   pCONT_iLight->animation.transition.time_ms = 2000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
    //   pCONT_iLight->animation.flags.ftime_use_map = true;
    // #else
    //   pCONT_iLight->animation.transition.time_ms.map_id = TIME_MAP_SECS_15_ID;
    //   pCONT_iLight->animation.transition.time_ms.val = 1000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.time_ms.map_id]*1000;
    //   pCONT_iLight->animation.flags.ftime_use_map = true;
    // // #endif

    // // #ifdef DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID
    //   // pCONT_iLight->animation.transition.rate_ms.map_id = 6;
    //   /*DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID<RATE_MAP_SECS_LENGTH_ID?
    //                                             DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID:RATE_MAP_SECS_15_ID;*/
    //   pCONT_iLight->animation.transition.rate_ms = 10000;//PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
    //   pCONT_iLight->animation.flags.frate_use_map = true;
    // // #else
    // //   pCONT_iLight->animation.transition.rate_ms.map_id = RATE_MAP_SECS_15_ID;
    // //   pCONT_iLight->animation.transition.rate_ms = PROGMEM rate_map_secs[pCONT_iLight->animation.transition.rate_ms.map_id]*1000;
    // //   pCONT_iLight->animation.flags.frate_use_map = true;
    // // #endif

    // // #ifdef DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID
    // //   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = 4;/*DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID<PIXELS_UPDATE_PERCENTAGE_LENGTH_ID?
    // //                         DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID:PIXELS_UPDATE_PERCENTAGE_20_ID;*/
    // // #else
    // //   pCONT_iLight->animation.transition.pixels_to_update_as_percentage.map_id = PIXELS_UPDATE_PERCENTAGE_20_ID;
    // // #endif

    #ifdef DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE
      // pCONT_iLight->animation.brightness = (float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f;//mSupport::WithinLimits(0,(float)DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE/100.0f,1)
    #else
      // pCONT_iLight->animation.brightness = WithinLimits(0,(float)1/100.0f,1);
      // pCONT_iLight->animation.brightness = 1; //default ot 50% normally for power reasons
    #endif

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    #ifdef DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ
      liveview.refresh_rate = DEFAULT_LIGHTING_LIVEVIEW_REFRESH_RATE_HZ;
    #else
      pCONT_iLight->liveview.refresh_rate = 1000;
    #endif
    #endif // USE_MODULE_NETWORK_WEBSERVER
    
    
    // #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
    // #ifdef DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID
    //   flashersettings.function = 1;/*DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID<EFFECTS_FUNCTION_LENGTH_ID?
    //                         DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID:EFFECTS_FUNCTION_SLOW_GLOW_ID;*/
    // #else
    //   flashersettings.function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
    // #endif
    // #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT

  }


  // uint16_t buffer_length = 0;
  // char* buffer = pCONT_set->Settings.animation_settings.palette_user_variable_name_list_ctr;
  // for(uint8_t i=0;i<20;i++){
  //  buffer_length+=sprintf(buffer+buffer_length,"%s%02d|\0","User",i);
  // }
  // #ifndef DISABLE_SERIAL_LOGGING
  // //  Serial.println(buffer); log debuf more
  // #endif

  
  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  memset(&editable_mapped_array_data_array,0,sizeof(editable_mapped_array_data_array));
  
  uint16_t single_row_count_array[] = {
    31,35,39,42,43,//31,44,39,42,43,
    43,43,42,42,42,
    41,39,38,36,33,
    30,29,28,25,25,
    23,22,22,20,18,
    19,17,18,16,14,
    14,13,10,10,11,
    11,8,4,4,5,6,6,
    5,5,5,5,5,
    0,3
  }; // 0 is filled last
  uint8_t single_row_count_array_size = 49;//sizeof(single_row_count_array)/single_row_count_array[0];

  // Serial.println(single_row_count_array_size);
  // delay(5000);
  
  uint16_t count = 0;
  for(int i=0;i<single_row_count_array_size;i++){
    count += single_row_count_array[i];
  }
  single_row_count_array[single_row_count_array_size-2] = 1050-count; // use 2nd last one as filler
  

  memcpy(editable_mapped_array_data_array, single_row_count_array, sizeof(single_row_count_array));


  pixel_group.mapped_array_data.length = ArrayCountUntilNull(editable_mapped_array_data_array, (uint8_t)D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH); //values before 0

DEBUG_LINE;
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING


}










/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************* Helper Functions ie rarely edited ************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

const char* mAnimatorLight::GetAnimationStatusCtr(char* buffer, uint8_t buflen){

  if(pCONT_iLight->animator_controller->IsAnimating()){
    snprintf_P(buffer, buflen, PSTR("Animating"));
    return buffer;
  }
  if(_segments[0].flags.fEnable_Animation){
    // (millis since) + (next event millis)
    int16_t until_next_millis = _segments[0].transition.rate_ms-(millis()-pCONT_iLight->runtime.animation_changed_millis);
    int16_t secs_until_next_event = until_next_millis/1000;
    // secs_until_next_event/=1000;
    // Serial.println(secs_until_next_event);

    // char float_ctr[10];
    // dtostrf(round(secs_until_next_event),3,1,float_ctr);
    // Serial.println(float_ctr);

    // AddLog(LOG_LEVEL_INFO,PSTR("GetAnimationStatusCtr %d %d"),
    //   until_next_millis,
    //   //millis(),pCONT_iLight->runtime.animation_changed_millis,pCONT_iLight->animation.transition.rate_ms,
    //   secs_until_next_event
    // );  
    
    if(secs_until_next_event>=0){
      snprintf_P(buffer, buflen, PSTR("Enabled (in %d secs)"), secs_until_next_event);//float_ctr);
    }else{
      snprintf_P(buffer, buflen, PSTR("Not currently running"));
      //  sprintf(buffer,PSTR("Not currently running"));//float_ctr);
    }

    // sprintf(ctr,PSTR("Enabled (%d secs)\0"),secs_until_next_event);
    // sprintf(ctr,PSTR("Enabled (123 secs)\0"),secs_until_next_event);
    // Serial.println(ctr);
    return buffer;
  }else{
    return D_CSTRING_ERROR_MESSAGE_CTR;
    // return "Paused";
  }
  return D_CSTRING_ERROR_MESSAGE_CTR;
}



/*******************************************************************************************************************
********************************************************************************************************************
************CODE ABOVE THIS LINE IS PART OF THE NEWEST REWRITE********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/




/*******************************************************************************************************************
********************************************************************************************************************
************ANIMATION AND BLENDING**************************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/


RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness){

  RgbcctColor colour_adjusted_with_brightness; // No init for speed// = RgbcctColor();//0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightness);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightness);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightness);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightness);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightness);
  return colour_adjusted_with_brightness;

}

RgbcctColor mAnimatorLight::ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT){

  RgbcctColor colour_adjusted_with_brightness = RgbcctColor(0);
  colour_adjusted_with_brightness.R  = mapvalue(full_range_colour.R,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.G  = mapvalue(full_range_colour.G,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.B  = mapvalue(full_range_colour.B,  0,255, 0,brightnessRGB);
  colour_adjusted_with_brightness.WW = mapvalue(full_range_colour.WW, 0,255, 0,brightnessCCT);
  colour_adjusted_with_brightness.WC = mapvalue(full_range_colour.WC, 0,255, 0,brightnessCCT);
  return colour_adjusted_with_brightness;

}


RgbcctColor mAnimatorLight::ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness){
  
  uint8_t new_brightness_255 = 0;

// Create new function of ApplyBrightnesstoDesiredColour to do this if statement generically
// if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){


  if(_segments[0].flags.Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice)
  {

    // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
    // uint8_t temp_max_brightness = flashersettings.brightness_max;
    // #else
    uint8_t temp_max_brightness = 255;
    // #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION

    if(_segments[0].flags.Limit_Upper_Brightness_With_BrightnessRGB)
    {
      temp_max_brightness = pCONT_iLight->getBriRGB_Global();
    }
    // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
    // new_brightness_255 = random(flashersettings.brightness_min, temp_max_brightness);
    // #else
    new_brightness_255 = random(0, temp_max_brightness);

    // #endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  }
  // Default: apply global brightness
  else{
    new_brightness_255 = brightness;
  }

  // AddLog(LOG_LEVEL_TEST, PSTR("ledGamma=new_brightness_255=%d - >%d"), 
  //   new_brightness_255, pCONT_iLight->ledGamma(new_brightness_255)
  // );

  #ifdef ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  if(_segments[0].flags.use_gamma_for_brightness)
  {
    new_brightness_255 = pCONT_iLight->ledGamma(new_brightness_255);
  }
  #endif // ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  return ApplyBrightnesstoRgbcctColour(full_range_colour, new_brightness_255);

}



#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
mAnimatorLight& mAnimatorLight::setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE)
{
  this->anim_progress_mqtt_function_callback = anim_progress_mqtt_function_callback;
  return *this;
}
#endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


#ifdef ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
bool mAnimatorLight::OverwriteUpdateDesiredColourIfMultiplierIsEnabled(){

  //Use starting pixel as temporary buffer, record desired as already set
  for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    animation_colours[pixel].StartingColor = animation_colours[pixel].DesiredColour;
  } // END for


  for(uint16_t indexPixel=0; indexPixel<pCONT_iLight->settings.light_size_count; indexPixel++){
    
    uint8_t pixel_multiplier_count = 0;
    if(indexPixel == 0){
      setpixel_variable_index_counter = 0; // reset
      pixel_group.mapped_array_data.index = 0;
    }

    if(setpixel_variable_index_counter>pCONT_iLight->settings.light_size_count-1){ return true; }

    if(pixel_group.flags.fEnabled){

      switch(pixel_group.flags.multiplier_mode_id){
        default:
        case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID:
          pixel_multiplier_count = pixel_group.multiplier;
          break;
        //case PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID:
          //pixel_multiplier_count = random(1,pixel_group.multiplier);
          //break;
        case PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID:
          if(pixel_group.mapped_array_data.values != nullptr){
            // if any match, then jump index of lights forward
            pixel_multiplier_count = pixel_group.mapped_array_data.values[pixel_group.mapped_array_data.index];
            //AddLog(LOG_LEVEL_WARN, PSTR("PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID %d %d %d"),pixel_multiplier_count,pixel_group.mapped_array_data.index,pixel_group.mapped_array_data.length);
            if(pixel_group.mapped_array_data.index++ >= pixel_group.mapped_array_data.length-1){
              pixel_group.mapped_array_data.index = 0;
            }
          }else{
            pixel_multiplier_count = 1;//pixel_group.multiplier;
          }
          break;
      }
  AddLog(LOG_LEVEL_TEST, PSTR("pixel_multiplier_count=%d"),pixel_multiplier_count);

      for(uint8_t ii = 0; ii < pixel_multiplier_count; ii++){
        animation_colours[setpixel_variable_index_counter++].DesiredColour = animation_colours[indexPixel].StartingColor;
        if(setpixel_variable_index_counter>pCONT_iLight->settings.light_size_count-1){ return true; }
      }

    }
    else{
      animation_colours[indexPixel].DesiredColour = animation_colours[indexPixel].StartingColor;
    } // pixel_group.flags.fEnabled

    AddLog(LOG_LEVEL_TEST, PSTR("indexPixel=%d,setpixel_variable_index_counterhere=%d %d"),indexPixel,setpixel_variable_index_counter,pixel_multiplier_count);

  }

  return true; // returns if this function changed the desiredcolour

}

  #endif // ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS



void mAnimatorLight::SetRefreshLEDs(){

ALOG_ERR(PSTR("This function should never be called going forward"));

  _segments[0].single_animation_override.time_ms = 10;
  segment_animation_override.fRefreshAllPixels = true;
  // segment_animation_override.time_ms = 10;





  _segments[0].flags.fForceUpdate = true;

}











// Final output, but check for power limit
void mAnimatorLight::StripUpdate(){

  DEBUG_PIN2_SET(LOW);
  // Serial.print("-");

  // STRIP_SIZE_REPEAT_MAX

  // if(settings.strip_size_repeat_animation)

  // Calculate output pixel size
  // uint16_t pixel_output_length = pCONT_iLight->settings.light_size_count;
  // if(pCONT_iLight->settings.light_size_count>STRIP_SIZE_MAX){
  //   pixel_output_length = STRIP_SIZE_MAX;
  // }
  // // Replicate SetPixel for repeated output
  // #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // pixel_output_length = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;  
  // #endif // ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES



// AddLog(LOG_LEVEL_WARN, PSTR("Disabled Replicate SetPixel for repeated output"));
  // // Replicate SetPixel for repeated output
  // #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // int pixels_existing_index = 0;
  // for(int i=0;i<ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;i++){
  //   SetPixelColor(i,GetPixelColor(pixels_existing_index++));
  //   if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  // }
  // #endif // ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES



  
#ifdef ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION
if(mTime::TimeReached(&tSavedCalculatePowerUsage,1000)){
// #ifdef USE_SPEED_TESTING
  uint8_t channel_count = 3;

//ADD TO ONLY Check power level every 1 second to improve speed

  RgbTypeColor c;
  // float estimated_power_new_mA = 0;
  // uint32_t last_power = 0;
  // float pixel_units = 0;
  // float this_power = 0;
  // for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
  //   // colour_tmp = desired_colour[ledout.index];
  //   // estimated_power_new +=  WebColorFromColourType(desired_colour[ledout.index]) * channel_count * 20;
  //   c = desired_colour[ledout.index];
  //   pixel_units = c.R + c.G + c.B;
  //   this_power = (pixel_units * power_rating.Average_mA_Per_Pixel_Step;
  //   estimated_power_new_mA +=  this_power;
  //   //last_power = (c.R + c.G + c.B) * power_rating.Average_mA_Per_Pixel_Step;
  //   // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //   //   c.R,c.G,c.B,
  //   //   (int)pixel_units,
  //   //   (int)this_power,
  //   //   (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //   //   (int)estimated_power_new_mA
  //   // );
  
  //   // estimated_power_new +=  colour_tmp.W * channel_count * 20;
  // }
  // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //     c.R,c.G,c.B,
  //     (int)pixel_units,
  //     (int)this_power,
  //     (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //     (int)estimated_power_new_mA
  //   );

  float estimated_power_new_mA = 0;
  uint32_t power = 0;
  float pixel_units = 0;
  float this_power = 0;
  for(ledout.index=0;ledout.index<ledout.length;ledout.index++){ 
    // colour_tmp = desired_colour[ledout.index];
    // estimated_power_new +=  WebColorFromColourType(desired_colour[ledout.index]) * channel_count * 20;
    c = desired_colour[ledout.index];, 19
    power += c.R + c.G + c.B;
    // this_power = (pixel_units * power_rating.Average_mA_Per_Pixel_Step;
    // estimated_power_new_mA +=  this_power;
    //last_power = (c.R + c.G + c.B) * power_rating.Average_mA_Per_Pixel_Step;
    // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
    //   c.R,c.G,c.B,
    //   (int)pixel_units,
    //   (int)this_power,
    //   (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
    //   (int)estimated_power_new_mA
    // );
  
    // estimated_power_new +=  colour_tmp.W * channel_count * 20;
  }
  // AddLog(LOG_LEVEL_TEST,PSTR("%d.%d.%d\t%d\t%d\t%d\t%d"),
  //     c.R,c.G,c.B,
  //     (int)pixel_units,
  //     (int)this_power,
  //     (int)(pixel_units * power_rating.Average_mA_Per_Pixel_Step),
  //     (int)estimated_power_new_mA
  //   );
  // Store current power usage
  power_rating.current_mA = (float)(power);// * power_rating.Average_mA_Per_Pixel_Step);//estimated_power_new_mA;
  power_rating.power = (power_rating.current_mA/1000)*5;  // /1000*5

  // AddLog(LOG_LEVEL_TEST,PSTR("Estimated Power Consumption = %d (mA) p%d"),(int)estimated_power_new_mA,(int)last_power);
  
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("Estimated Power Consumption = %d (mA)"),(int)estimated_power_new_mA);
    #endif
}
#endif // ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION


/**
 * @brief Before showing, duplicate the output by taking the 0-X pixels and replicating it... though this might not work since setpixellength is not long enough anymore? (or should I make a library edit to do this same feature)
 * 
 */

// Serial.print("i");
  pCONT_iLight->ShowInterface();

  // for (auto copies = 0; copies < 3; copies++) {
  //   image.Blt(stripbus, copies * image.PixelCount());
  // }
  
  /**
   * Replicate output if enabled
   * Notice this has been moved the the function above, so needs to be called there
   * It sould be changed to a runtime variable/flag option for mqtt command
   * */
  // #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // uint16_t internal_length = pCONT_iLight->settings.light_size_count;
  // uint16_t external_length = STRIP_OUTPUT_REPEATED_LENGTH;

  // uint16_t pixel_output_length = pCONT_iLight->settings.light_size_count;
  // if(pCONT_iLight->settings.light_size_count>STRIP_SIZE_MAX){
  //   pixel_output_length = STRIP_SIZE_MAX;
  // }
  // // Replicate SetPixel for repeated output
  // #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // pixel_output_length = ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;  
  // #endif // ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES

  // // AddLog(LOG_LEVEL_WARN, PSTR("Disabled Replicate SetPixel for repeated output"));
  // // Replicate SetPixel for repeated output
  // // #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // #ifdef ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // int pixels_existing_index = 0;
  // for(int i=0;i<ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES;i++){
  //   SetPixelColor(i,GetPixelColor(pixels_existing_index++));
  //   if(pixels_existing_index>=STRIP_SIZE_MAX){ pixels_existing_index = 0;}
  // }
  // #endif // ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES
  // #endif // ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES

  DEBUG_PIN2_SET(HIGH);

}


#endif //USE_MODULE_LIGHTS_ANIMATOR



