#include "mAnimatorLight.h"

/***
 * Controls options for people without wireless controls
 * 1) Basic GPIO controller, single button per GPIO
 * add button that is "show off, have some random animations etc before returning to preffered mode"
 * 2) Other? 
 * 
 * (b0) (b1)    =====   Palette    -/+
 * (b0) (b1)    =====   Brightness -/+    (create more simple, with 4 levels only, low1, low2, medium, high)
 * (b0) (b1)    =====   Effects    -/+
 * (b0) (b1)    =====   Intensity  -/+
 * 
 * */


#ifdef USE_MODULE_LIGHTS_ANIMATOR


#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


#ifdef ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM
void mAnimatorLight::Physical_UserInput_Init()
{
  
  EEPROM.begin(EEPROM_SIZE);
  // AddLog(LOG_LEVEL_TEST, PSTR("EEPROM.begin(EEPROM_SIZE);"));
  // delay(4000);
  
  // for(int address=0;address<12;address++)
  // {
    
    // EEPROM.write(address, address);
    
    // if (EEPROM.commit()) {
    //   Serial.println("EEPROM successfully committed");
    // } else {
    //   Serial.println("ERROR! EEPROM commit failed");
    // }
    
  // }
  user_input.selected.palette_id = EEPROM.read(0);
  user_input.selected.brightness_id = EEPROM.read(1);
  user_input.selected.effects_id = EEPROM.read(2);
  user_input.selected.intensity_id = EEPROM.read(3);


}


/**
 * Aka usermods in the future
 * */
void mAnimatorLight::Physical_UserInput_Load()
{

  USER_INPUT_SAVED_IN_EEPROM::ACTIVE_OPTION preload;

  preload.palette_id = EEPROM.read(0);
  preload.brightness_id = EEPROM.read(1);
  preload.effects_id = EEPROM.read(2);
  preload.intensity_id = EEPROM.read(3);

  AddLog(LOG_LEVEL_TEST, PSTR("Load: %d:%d:%d:%d"), 
    preload.palette_id, 
    preload.brightness_id, 
    preload.effects_id, 
    preload.intensity_id
  );

  // AddLog(LOG_LEVEL_TEST, PSTR("Load: %d:%d:%d:%d"), 
  //   user_input.selected.palette_id, 
  //   user_input.selected.brightness_id, 
  //   user_input.selected.effects_id, 
  //   user_input.selected.intensity_id
  // );


// #include "0_ConfigUser/mFirmwareCustom_Secret_DevLogging.h"
  user_input.selected.palette_id = EEPROM.read(0);
  user_input.selected.brightness_id = EEPROM.read(1);
  user_input.selected.effects_id = EEPROM.read(2);
  user_input.selected.intensity_id = EEPROM.read(3);

//   AddLog(LOG_LEVEL_TEST, PSTR("Load: %d:%d:%d:%d"), user_input.selected.palette_id, user_input.selected.brightness_id, user_input.selected.effects_id, user_input.selected.intensity_id);

  /**
   * Apply from ids onto the internal values
   * Call the userinput, but make it so none of the buttons are pressed
   * thus, all ids much be applied outside button presses
   * */
    
  // byte value;
  // for(int address=0;address<12;address++)
  // {
  //   value = EEPROM.read(address);

  //   Serial.print(address);
  //   Serial.print("\t");
  //   Serial.print(value, DEC);
  //   Serial.println();
  // }

}

void mAnimatorLight::Physical_UserInput_Save()
{  

  AddLog(LOG_LEVEL_TEST, PSTR("Save: %d:%d:%d:%d"), user_input.selected.palette_id, user_input.selected.brightness_id, user_input.selected.effects_id, user_input.selected.intensity_id);

  // EEPROM.put(0, user_input);
  
  EEPROM.write(0, user_input.selected.palette_id);
  EEPROM.write(1, user_input.selected.brightness_id);
  EEPROM.write(2, user_input.selected.effects_id);
  EEPROM.write(3, user_input.selected.intensity_id);
  //EEPROM.write(3, user_input.selected.loaded_from_eeprom);

  if (EEPROM.commit()) {
    Serial.println("EEPROM successfully committed");
  }


}

#endif // ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM

  uint8_t palette_options_ids[30] = 
  {
    mPalette::PALETTELIST_STATIC_CHRISTMAS_01_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_02_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_03_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_04_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_05_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_06_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_07_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_08_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_09_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_10_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_11_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_12_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_13_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_14_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_15_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_16_ID,
    mPalette::PALETTELIST_STATIC_CHRISTMAS_17_ID,
    mPalette::PALETTELIST_STATIC_WINTER_ID,
    mPalette::PALETTELIST_STATIC_AUTUMN_ID,
    mPalette::PALETTELIST_STATIC_AUTUMN_RED_ID,
    mPalette::PALETTELIST_STATIC_FLOWER_SWEATPEAS_01_ID,
    mPalette::PALETTELIST_STATIC_FLOWER_SWEATPEAS_02_ID,
    mPalette::PALETTELIST_STATIC_HOLLOWEEN_OGP_ID,
    mPalette::PALETTELIST_STATIC_HOLLOWEEN_OP_ID,
    mPalette::PALETTELIST_STATIC_MICROLEDS_PASTEL_01_ID,
    mPalette::PALETTELIST_STATIC_PINK_PURPLE_01_ID,
    mPalette::PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY_ID,
    /**
     * Gradients applied across the output
     * */
    mPalette::PALETTELIST_STATIC_RAINBOW_ID,
    mPalette::PALETTELIST_STATIC_RAINBOW_INVERTED_ID,
    mPalette::PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE_ID,
    // mPalette::PALETTELIST_STATIC_GRADIENT_FIRE_01_ID,
  };



void mAnimatorLight::CommandSet_Physical_UserInput_Buttons()
{

// return;

  // uint8_t palette_options_ids[30] = 
  // {
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_01_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_02_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_03_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_04_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_05_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_06_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_07_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_08_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_09_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_10_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_11_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_12_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_13_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_14_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_15_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_16_ID,
  //   mPalette::PALETTELIST_STATIC_WINTER_ID,
  //   mPalette::PALETTELIST_STATIC_AUTUMN_ID,
  //   mPalette::PALETTELIST_STATIC_AUTUMN_RED_ID,
  //   mPalette::PALETTELIST_STATIC_FLOWER_SWEATPEAS_01_ID,
  //   mPalette::PALETTELIST_STATIC_FLOWER_SWEATPEAS_02_ID,
  //   mPalette::PALETTELIST_STATIC_HOLLOWEEN_OGP_ID,
  //   mPalette::PALETTELIST_STATIC_HOLLOWEEN_OP_ID,
  //   mPalette::PALETTELIST_STATIC_MICROLEDS_PASTEL_01_ID,
  //   mPalette::PALETTELIST_STATIC_PINK_PURPLE_01_ID,
  //   mPalette::PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY_ID,
  //   /**
  //    * Gradients applied across the output
  //    * */
  //   mPalette::PALETTELIST_STATIC_RAINBOW_ID,
  //   mPalette::PALETTELIST_STATIC_RAINBOW_INVERTED_ID,
  //   mPalette::PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE_ID,
  //   mPalette::PALETTELIST_STATIC_GRADIENT_FIRE_01_ID,
  // };

  char buffer[100];

  /**
   * Buttons: Palette Colours
   * */
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_1_LEFT)
  {
    // Decrement and wrap
    user_input.selected.palette_id = (user_input.selected.palette_id == 0) ? sizeof(palette_options_ids)-1: user_input.selected.palette_id-1;
    pCONT_iLight->animation.palette.id = palette_options_ids[user_input.selected.palette_id];
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"P-\" L[%d] \t (%d)\"%s\""), user_input.selected.palette_id, pCONT_iLight->animation.palette.id, mPaletteI->GetPaletteNameByID(pCONT_iLight->animation.palette.id, buffer, sizeof(buffer)));
  }else
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_1_RIGHT)
  {
    // Increment and wrap
    user_input.selected.palette_id = (user_input.selected.palette_id == (sizeof(palette_options_ids)-1)) ? 0: user_input.selected.palette_id+1;
    pCONT_iLight->animation.palette.id = palette_options_ids[user_input.selected.palette_id];
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"P+\" L[%d] \t (%d)\"%s\""), user_input.selected.palette_id, pCONT_iLight->animation.palette.id, mPaletteI->GetPaletteNameByID(pCONT_iLight->animation.palette.id, buffer, sizeof(buffer)));
  }


  /**
   * Button: Brightness
   * */
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_2_LEFT)
  {
    user_input.selected.brightness_id = (user_input.selected.brightness_id == 0) ? sizeof(brightness_options_ids)-1: user_input.selected.brightness_id-1;
    pCONT_iLight->setBriRGB_Global(map(brightness_options_ids[user_input.selected.brightness_id]  ,0,100, 0,255));  
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"B-\" L[%d] \t %d/255"), user_input.selected.brightness_id, pCONT_iLight->getBriRGB_Global());
  }
  else 
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_2_RIGHT)
  {
    user_input.selected.brightness_id = (user_input.selected.brightness_id == (sizeof(brightness_options_ids)-1)) ? 0: user_input.selected.brightness_id+1;
    pCONT_iLight->setBriRGB_Global(map(brightness_options_ids[user_input.selected.brightness_id]  ,0,100, 0,255));
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"B+\" L[%d] \t %d/255"), user_input.selected.brightness_id, pCONT_iLight->getBriRGB_Global());
  }

  /**
   * Buttons: Effects
   * */
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_3_LEFT)
  {
    user_input.selected.effects_id = (user_input.selected.effects_id == 0) ? sizeof(effects_options_ids)-1: user_input.selected.effects_id-1;
    flashersettings.function = effects_options_ids[user_input.selected.effects_id];
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"E-\" L[%d] \t\"%s\""), user_input.selected.effects_id, pCONT_lAni->GetFlasherFunctionNamebyID(flashersettings.function, buffer, sizeof(buffer)));
  }else
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_3_RIGHT)
  {
    user_input.selected.effects_id = (user_input.selected.effects_id == (sizeof(effects_options_ids)-1)) ? 0: user_input.selected.effects_id+1;
    flashersettings.function = effects_options_ids[user_input.selected.effects_id];
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"E+\" L[%d] \t\"%s\""), user_input.selected.effects_id, pCONT_lAni->GetFlasherFunctionNamebyID(flashersettings.function, buffer, sizeof(buffer)));
  }

  /**
   * Buttons: Intensity
   * */
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_4_LEFT)
  {
    user_input.selected.intensity_id = (user_input.selected.intensity_id == 0) ? sizeof(intensity_options_ids)-1: user_input.selected.intensity_id-1;
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"I-\" L[%d] \t %d"), user_input.selected.intensity_id, intensity_options_ids[user_input.selected.intensity_id]);
  }else
  if(pCONT_rules->event_triggered.device_id == USER_INPUT_BUTTON_4_RIGHT)
  {
    user_input.selected.intensity_id = (user_input.selected.intensity_id == (sizeof(intensity_options_ids)-1)) ? 0: user_input.selected.intensity_id+1;
    AddLog(LOG_LEVEL_INFO, PSTR("BUTTON\n\r \"I+\" L[%d] \t %d"), user_input.selected.intensity_id, intensity_options_ids[user_input.selected.intensity_id]);
  }

  /**
   * If event was serviced, then clear it
   * */
  pCONT_rules->Reset(&pCONT_rules->event_triggered);

  PhysicalController_Convert_IDs_to_Internal_Parameters();

  /**
   * Ensuring settings are correct that would otherwise be set in templates
   * */

  /**
   * check we are in range, or shift back into range
   * */
  pCONT_iLight->animation.flags.fForceUpdate = true;
  pCONT_iLight->animation_override.time_ms = 500; // Instant
  pCONT_set->settings_save_decounter_seconds_delayed_save = 10; // delay the saving until the lights have updated



  
  // pCONT_set->SettingsSave(2);

}

void mAnimatorLight::PhysicalController_Convert_IDs_to_Internal_Parameters()
{
  
  char buffer[100];
  char buffer2[100];


  /**
   * Constrain options, particularly important for bad EEPROM reads
   * */
  user_input.selected.palette_id = constrain(user_input.selected.palette_id, 0, sizeof(palette_options_ids));
  user_input.selected.brightness_id = constrain(user_input.selected.brightness_id, 0, sizeof(brightness_options_ids));
  user_input.selected.effects_id = constrain(user_input.selected.effects_id, 0, sizeof(effects_options_ids));
  user_input.selected.intensity_id = constrain(user_input.selected.intensity_id, 0, sizeof(intensity_options_ids));


  /**
   * Apply changes to id, if any, to internal parameters
   * This must happen independent of button presses, so loading/saving can apply the changes too
   * */
  pCONT_iLight->animation.palette.id = palette_options_ids[user_input.selected.palette_id];
  pCONT_iLight->setBriRGB_Global(map(brightness_options_ids[user_input.selected.brightness_id]  ,0,100, 0,255));  
  flashersettings.function = effects_options_ids[user_input.selected.effects_id];
  /**
   * depending on mode, change the rate/time
   * */
  switch (flashersettings.function)
  {
    default:
    case EFFECTS_FUNCTION_STATIC_PALETTE_ID:
      pCONT_iLight->animation.transition.time_ms = 2000;
      pCONT_iLight->animation.transition.rate_ms = 10000;
      pCONT_lAni->SetLEDOutAmountByPercentage(100);
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
    break;
    case EFFECTS_FUNCTION_SEQUENTIAL_ID:
      /**
       * Only enable very fast transitions for palettes that use linear palettes eg rainbow, otherwise, use slower "fast" speed
       * */
      switch(pCONT_iLight->animation.palette.id)
      { 
        case mPalette::PALETTELIST_STATIC_RAINBOW_ID:
        case mPalette::PALETTELIST_STATIC_RAINBOW_INVERTED_ID:
          pCONT_iLight->animation.transition.time_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 19,9000);
          pCONT_iLight->animation.transition.rate_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 20,10000);
        break;
        default:
          pCONT_iLight->animation.transition.time_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 200,9000);
          pCONT_iLight->animation.transition.rate_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 201,10000);
        break;
      }
      pCONT_lAni->SetLEDOutAmountByPercentage(100);
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
    break;
    case EFFECTS_FUNCTION_SLOW_GLOW_ID:
      pCONT_iLight->animation.transition.time_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 19,20000);
      pCONT_iLight->animation.transition.rate_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 20,30000);
      pCONT_lAni->SetLEDOutAmountByPercentage(map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 1,35));
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
    break;
    case EFFECTS_FUNCTION_STEPPING_PALETTE_ID:
      pCONT_iLight->animation.transition.time_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 23,9000);
      pCONT_iLight->animation.transition.rate_ms = map(intensity_options_ids[user_input.selected.intensity_id], 0,100, 25,10000);
      pCONT_lAni->SetLEDOutAmountByPercentage(100);
      pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
    break;
  }

  AddLog(LOG_LEVEL_INFO, PSTR(
    "\n\r\n\r"
    "Palette    [Bid:%d] = \"%s\"\n\r"
    "Brightness [Bid:%d] = %d%%\n\r"
    "Effect     [Bid:%d] = \"%s\"\n\r"
    "Intensity  [Bid:%d] = %d (time/rate  %d/%d)\n\r"
    "Pixels Changing     = %d"),

    user_input.selected.palette_id,     mPaletteI->GetPaletteNameByID(pCONT_iLight->animation.palette.id, buffer, sizeof(buffer)),
    user_input.selected.brightness_id,  map(pCONT_iLight->getBriRGB_Global(), 0,255, 0,100),
    user_input.selected.effects_id,     pCONT_lAni->GetFlasherFunctionNamebyID(flashersettings.function, buffer2, sizeof(buffer2)),
    user_input.selected.intensity_id, intensity_options_ids[user_input.selected.intensity_id], pCONT_iLight->animation.transition.time_ms, pCONT_iLight->animation.transition.rate_ms,
    pCONT_lAni->strip_size_requiring_update

  );

/**
 * Improve animations by making sure blend time is always less then 90% of rate
 * */
  if(pCONT_iLight->animation.transition.time_ms > (pCONT_iLight->animation.transition.rate_ms*0.90))
{
  pCONT_iLight->animation.transition.time_ms = pCONT_iLight->animation.transition.rate_ms*0.90;
}
  
  // pCONT_set->Settings.animation_settings.xmas_controller_params[0] = user_input.selected.palette_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[1] = user_input.selected.brightness_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[2] = user_input.selected.effects_id;
  // pCONT_set->Settings.animation_settings.xmas_controller_params[3] = user_input.selected.intensity_id;
  
  /**
   * check we are in range, or shift back into range
   * */
  pCONT_iLight->animation.flags.fForceUpdate = true;
  pCONT_iLight->animation_override.time_ms = 500; // Instant

  flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID; // regenerate colours before animating

}

#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS






#endif //USE_MODULE_LIGHTS_ANIMATOR




