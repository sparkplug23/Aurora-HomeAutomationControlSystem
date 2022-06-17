#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Solid Colour
 * @note : Shows only 1 colour, preferably rgbcct palette or else the first of the palette (Add optional index for this later?)
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Segment_Animate_Function__Solid_Static_Single_Colour()
{
  
  mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);


  /**
   * @brief 
   * Temp fix for rgbcct types, need to consoladate colour types
   * 
   */
  // AddLog(LOG_LEVEL_TEST, PSTR("pCONT_set->Settings.light_settings.type = %d"), pCONT_set->Settings.light_settings.type);

  
  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2; // allocate space for single colour pair

  // AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  // Brightness is generated internally, and rgbcct solid palettes are output values
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = false;

  #ifdef ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  RgbcctColor desired_colour  = mPaletteI->GetColourFromPalette_Joint(_segments[segment_active_index].palette.id);
  #else
  RgbcctColor desired_colour  = mPaletteI->GetColourFromPalette(_segments[segment_active_index].palette.id);
  #endif // ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD

  // AddLog(LOG_LEVEL_TEST, PSTR("desired_colour1=%d,%d,%d,%d,%d"),desired_colour.R,desired_colour.G,desired_colour.B,desired_colour.WC,desired_colour.WW);

  if(!_segment_runtimes[segment_active_index].rgbcct_controller->getApplyBrightnessToOutput()){ // If not already applied, do it using global values
    desired_colour = ApplyBrightnesstoRgbcctColour(
      desired_colour, 
      _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessRGB255(),
      _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessCCT255()
    );
  }

  // AddLog(LOG_LEVEL_TEST, PSTR("desired_colour2=%d,%d,%d,%d,%d"),desired_colour.R,desired_colour.G,desired_colour.B,desired_colour.WC,desired_colour.WW);
  
  SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_active_index].data, _segment_runtimes[segment_active_index]._dataLen, 0, _segments[segment_active_index].colour_type, desired_colour); 
  SetTransitionColourBuffer_StartingColour(_segment_runtimes[segment_active_index].data, _segment_runtimes[segment_active_index]._dataLen, 0, _segments[segment_active_index].colour_type, GetPixelColor(0));

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_SingleColour_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Static Palette
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Segment_Animate_Function__Static_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__INORDER__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Slow Glow
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animate_Function__Slow_Glow()
{

  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

}


/**
 * @brief New allocated buffers must contain colour info
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(const AnimationParam& param)
{    

  if(_segment_runtimes[segment_active_index].data == nullptr)
  { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("_segment_runtimes[%d].data == nullptr = %d"),segment_active_index);
    #endif // ENABLE_LOG_LEVEL_INFO
    return;
  }

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;
  uint16_t pixel_within_data_iter = 0;

  for (uint16_t pixel =  _segments[segment_active_index].pixel_range.start; 
                pixel <= _segments[segment_active_index].pixel_range.stop; 
                pixel++
  ){ 

    GetTransitionColourBuffer(_segment_runtimes[segment_active_index].data, _segment_runtimes[segment_active_index]._dataLen, pixel_within_data_iter, _segments[segment_active_index].colour_type, &colour_pairs);
    pixel_within_data_iter++;

    updatedColor = RgbTypeColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);    
    SetPixelColor(pixel, updatedColor, segment_active_index);

  }
  
}

/**
 * @brief New allocated buffers must contain colour info
 * 
 * @param param 
 */
void mAnimatorLight::AnimationProcess_Generic_SingleColour_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(const AnimationParam& param)
{    

  if(_segment_runtimes[segment_active_index].data == nullptr)
  { 
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST, PSTR("_segment_runtimes[%d].data == nullptr = %d"),segment_active_index);
    #endif // ENABLE_LOG_LEVEL_INFO
    return;
  }

  RgbcctColor updatedColor = RgbcctColor(0);
  TransitionColourPairs colour_pairs;
  //uint16_t pixel_within_data_iter = 0;

  /**
   * @brief Replicate one colour across whole string (ie scences for h801 or addressable)
   * 
   */

  GetTransitionColourBuffer(_segment_runtimes[segment_active_index].data, _segment_runtimes[segment_active_index]._dataLen, 0, _segments[segment_active_index].colour_type, &colour_pairs);
  
    //pixel_within_data_iter++;

    updatedColor = RgbcctColor::LinearBlend(colour_pairs.StartingColour, colour_pairs.DesiredColour, param.progress);    
    
// AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.StartingColour=%d,%d,%d,%d,%d"),colour_pairs.StartingColour.R,colour_pairs.StartingColour.G,colour_pairs.StartingColour.B,colour_pairs.StartingColour.WC,colour_pairs.StartingColour.WW);
// AddLog(LOG_LEVEL_TEST, PSTR("Acolour_pairs.DesiredColour=%d,%d,%d,%d,%d"),colour_pairs.DesiredColour.R,colour_pairs.DesiredColour.G,colour_pairs.DesiredColour.B,colour_pairs.DesiredColour.WC,colour_pairs.DesiredColour.WW);

      

  // AddLog(LOG_LEVEL_TEST, PSTR("updatedColor=%d,%d,%d,%d,%d"),updatedColor.R,updatedColor.G,updatedColor.B,updatedColor.WC,updatedColor.WW);

  // AddLog(LOG_LEVEL_TEST, PSTR("getSubType=%d"),_segment_runtimes[segment_active_index].rgbcct_controller->getSubType());


  for (uint16_t pixel =  _segments[segment_active_index].pixel_range.start; 
                pixel <= _segments[segment_active_index].pixel_range.stop; 
                pixel++
  ){ 

    SetPixelColor(pixel, updatedColor, segment_active_index);

  }
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Segment_Animation__Rotating_Palette()
{

  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  uint8_t segment_index = segment_active_index;
  uint16_t* region_p             = &_segment_runtimes[segment_active_index].aux0;
  uint16_t* movement_direction_p = &_segment_runtimes[segment_active_index].aux1;
  uint16_t* flag_finish_flasher_pair_p = &_segment_runtimes[segment_active_index].aux2;
  uint16_t* force_finish_flasher_pair_once_p = &_segment_runtimes[segment_active_index].aux3;
  
  
  switch(*region_p){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
      // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
      // #endif

      /**
       * @brief 
       * Draw the static palette
       * 
       */




      // Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id);   // change to new method
      
      // #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
      // // Check if output multiplying has been set, if so, change desiredcolour array
      // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();      
      // #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING

      // // Get starting positions already on show
      // Segments_UpdateStartingColourWithGetPixel();

      
      *region_p = EFFECTS_REGION_ANIMATE_ID;
    // break; not into next right away
    case EFFECTS_REGION_ANIMATE_ID: //shift along
            
      if(*movement_direction_p)
      { // direction==1 move right ie AWAY from start

        // Save last pixel
        RgbcctColor colourlast = GetPixelColor(_segments[segment_active_index].pixel_range.stop-1); 
        // Shift towards first pixel
        for(int 
            p = _segments[segment_active_index].pixel_range.stop-1; //last to first
            p >= _segments[segment_active_index].pixel_range.start;
            p--
          ){ 
            SetPixelColor(p, GetPixelColor(p-1));
        }
        // Put first from end to start
        SetPixelColor(_segments[segment_active_index].pixel_range.start, colourlast);

      }
      else
      {
      
        // Save last pixel
        RgbcctColor colourfirst = GetPixelColor(_segments[segment_active_index].pixel_range.start); 
        // Shift towards first pixel
        for(int 
            p = _segments[segment_active_index].pixel_range.start; //last to first
            p <= _segments[segment_active_index].pixel_range.stop;
            p++
          ){ 
            SetPixelColor(p, GetPixelColor(p+1));
        }
        // Put first from end to start
        SetPixelColor(_segments[segment_active_index].pixel_range.stop, colourfirst);

      }

      _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
      StripUpdate();

    break;
  }

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Slow Glow
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Segment_Animation__Sequential_Palette()
{

  uint8_t segment_index = segment_active_index;

  uint16_t* region_p          = &_segment_runtimes[segment_active_index].aux0;
  uint16_t* indexes_active_p  = &_segment_runtimes[segment_active_index].aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &_segment_runtimes[segment_active_index].aux2; // shared_flasher_parameters_segments.indexes.counter

  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;

  desired_pixel=0;

  switch(*region_p){
    case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours

      // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "EFFECTS_REGION_COLOUR_SELECT_ID"));
     
      uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

        //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

        if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
        {
          
    #ifdef ENABLE_LOG_LEVEL_ERROR
          AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
          #endif
          _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
        }


      _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
      mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);
        
      int16_t pixel_position = -2;
      uint8_t pixels_in_map = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
    
      // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);
      
      desired_pixel = *indexes_active_p;
      uint8_t pixels_map_upper_limit = *indexes_active_p+1;
      uint8_t pixels_map_lower_limit = *indexes_active_p;

      uint8_t index_1, index_2;
      uint8_t counter = 0;
          
      //if last pixel, then include it and the first, else include it and the next
      if(*indexes_active_p == pixels_in_map-1){ //wrap wround
        index_1 = 0;
        index_2 = *indexes_active_p;
        counter = 0;
      }else{
        index_1 = *indexes_active_p;
        index_2 = *indexes_active_p+1;
        counter = 1;

      }

      *indexes_counter_p ^= 1;

      // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "shared_flasher_parameters = %d/%d/%d"), shared_flasher_parameters_segments.indexes.active,index_1,index_2);

      RgbTypeColor colour;

      for(uint16_t index=_segments[segment_active_index].pixel_range.start;
                   index<=_segments[segment_active_index].pixel_range.stop;
                   index++
      ){

        if(counter^=1){
          desired_pixel = *indexes_counter_p ? index_2 : index_1;
        }else{
          desired_pixel = *indexes_counter_p ? index_1 : index_2;
        }
        
        colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
        if(_segments[segment_active_index].flags.brightness_applied_during_colour_generation){
          colour = ApplyBrightnesstoRgbcctColour(colour, pCONT_iLight->getBriRGB_Global());
        }

        SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_active_index].data, _segment_runtimes[segment_active_index]._dataLen, index, _segments[segment_active_index].colour_type, colour);
           
        //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "desired_pixel= %d/%d/%d"),pixels_map_lower_limit,desired_pixel,pixels_map_upper_limit);
  
        // if(++desired_pixel>pixels_map_upper_limit){
        //   desired_pixel = pixels_map_lower_limit;
        // }

      } 

      //progress active index by 1 or reset
      // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "shared_flasher_parameters_segments.indexes.active=%d"), shared_flasher_parameters_segments.indexes.active);
      
      if(++*indexes_active_p>pixels_in_map-1){
        *indexes_active_p=0;
      }
      
      *region_p = EFFECTS_REGION_ANIMATE_ID;
  
    }break;
    case EFFECTS_REGION_ANIMATE_ID: //shift along
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "EFFECTS_REGION_ANIMATE_ID"));
    #endif //  ENABLE_LOG_LEVEL_INFO

      // Check if output multiplying has been set, if so, change desiredcolour array
      // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();  // THIS SHOULD PROBABLY JUST BE MOVED INTO THE SETPIXEL AND RAN BEFORE STRIPUPDATE
      // Get starting positions already on show
      DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
      // Call the animator to blend from previous to new
     
      setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
        [this](const AnimationParam& param){ 
          this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
        }
      );

      *region_p = EFFECTS_REGION_COLOUR_SELECT_ID;
      // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO DEBUG_INSERT_PAGE_BREAK "part2 region = %d"), _segment_runtimes[segment_active_index].aux0);

      break;
  }

}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireplace light effect using matrix of rgb
 * @note : Custom to whitehall, but should be used elsewhere
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animate__Fireplace_1D_01()
{

  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Segment_Animate__Fireplace_1D_01 dataSize = %d"), dataSize);
  // AddLog(LOG_LEVEL_TEST, PSTR("_segments[segment_active_index].colour_type  = %d"), _segments[segment_active_index].colour_type);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );


// #ifdef ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE
//   HsbColor colour_in = HsbColor(RgbColor(0));
  
//  #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
//   //Overwrite random brightness on special range
//   for(uint16_t index=256;index<300;index++){

//     colour_in = animation_colours[index].DesiredColour;

//     if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
//       colour_in.H = 0.0f;
//       colour_in.S = 1.0f;
//     }
//     colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

//     // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
//   // This will be the introduction of segments into my code!!
//     animation_colours[random(256,299)].DesiredColour = colour_in;

//     // animation_colours[random(40,49)].DesiredColour = colour_in;


}





#ifdef ENABLE_EXTRA_EFFECTS_SUNPOSITIONS 

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_LinearBlend
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * Best to develop this with 12 pixel ring, esp32
 * Start with RGPBO palette
 * 
 * Add test command cpp file, that lets me set anything in struct for development with mqtt. Only include in development build
 * 
 * Step 1: Scale palette progress with elevation (manual) 
 * Step 2: Use user defined max/min elevations, or, enable suns max/min of the day (is this directly in the math, or does it need to be calculated? perhaps at midnight for the next day)
 *         Save as uint16_t as it uses less memory than float, ie 12.34 will be 1234, what about minus, signed int16_t gives 32k +-, for only 2 decimel places, this is enough is +/-32768 can store 327.68 
 * 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_Step()
{

// for sun thing
// I will still use this function to get the raw colour, I just need another intermediate function that does the conversion with sun elevation
// also add sun elevation and azimuth into settings struct, that way I can update it locally or via command 

  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_LinearBlend
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_LinearBlend()
{

  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

}



// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01()
{
 
//  #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   uint8_t segment_index = segment_active_index;
//   uint16_t start_pixel = _segments[segment_active_index].pixel_range.start;
//   uint16_t end_pixel = _segments[segment_active_index].pixel_range.stop;


//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   _segments[segment_active_index].flags.brightness_applied_during_colour_generation = false;

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

//   // delay(1000);

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(_segments[segment_active_index].palette.id);
//   uint8_t pixels_max = mPaletteI->GetPixelsInMap(palette_p);
//   // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   int16_t indexing = 0;  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;  
//   float upper_boundary_value = 90;
//   float sun_positions_from_palette_index[pixels_max];  
//   uint8_t searching_matched_index = 0;

//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }


//   for(int i=0;i<pixels_max;i++)
//   {
//     // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       //Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }

//     // Directly, manually, check the last memory space

//     if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
//     {
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       break;

//     }




//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */
//   if(lower_boundary_index>=pixels_max)
//   {
//     lower_boundary_index = 0;
//     // Serial.printf("lower_boundary_index>=pixels_max\n\r");
//   }
//   if(upper_boundary_index>=pixels_max)
//   {
//     upper_boundary_index = pixels_max;
//     // Serial.printf("upper_boundary_index>=pixels_max\n\r");
//   }

//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   // Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   // Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   // Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   // Serial.printf("numer=\t%f\n\r",numer);
//   // Serial.printf("denum=\t%f\n\r",denum);
//   // Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

//   /**
//    * Showing the colours
//    * 1) previous
//    * 2) next
//    * 3) linearblend of the exact new colour
//    * */

//   RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//   RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//   // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//   //   lower_boundary_value, lower_boundary_index, 
//   //   upper_boundary_value, upper_boundary_index, progress_between_colours);

//   RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

//   RgbcctColor c = c_lower; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   _segments[segment_active_index].flags.fForceUpdate = true;

// //set desired colour
//   // _segment_runtimes[segment_active_index].active_rgbcct_colour_p->  = c_blended;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!_segment_runtimes[segment_active_index].rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessRGB255(),
//       _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = GetPixelColor();

//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   // this->setAnimFunctionCallback([this](const AnimationParam& param){
//   //     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//         // Call the animator to blend from previous to new
//   setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(param);
//     }
//   );

//   #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
   
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_05
//  * 
//  * CCT_Mapped, day white to warm white around +-20, then >20 is max cct
//  * 

// This needs fixing, so multiple scene (rgbcct controllers) can be used together


//  * *************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01()
{
 
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

// #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD


//   _segments[segment_active_index].palette.id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

//   mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values

// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif

//   if(sun_elevation < -20)
//   {
//     _segment_runtimes[segment_active_index].rgbcct_controller->setCCT(pCONT_iLight->get_CTRangeMax());      
//   }else
//   if(sun_elevation > 20)
//   {
//     _segment_runtimes[segment_active_index].rgbcct_controller->setCCT(pCONT_iLight->get_CTRangeMin());      
//   }else{
//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-20,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMax(),pCONT_iLight->get_CTRangeMin());
 
//     // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "cct_val=%d"),cct_val);
//     // Set the colour temp
//     _segment_runtimes[segment_active_index].rgbcct_controller->setCCT(cct_val);    
//   }

//   _segments[segment_active_index].flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);
//   _segments[segment_active_index].flags.fForceUpdate = true;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!_segment_runtimes[segment_active_index].rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessRGB255(),
//       _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = GetPixelColor();

//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   // this->setAnimFunctionCallback([this](const AnimationParam& param){
//   //     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//         // Call the animator to blend from previous to new
//   setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(param);
//     }
//   );
   
// #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

}


#endif // ENABLE_EXTRA_EFFECTS_SUNPOSITIONS 


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fade_Gradient
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// Fade solid colour from 0 to 75%, and a palette from 25 to 100% (50% overlap)
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Fade_Gradient(){
    
// #ifdef ENABLE_EFFECTS_ANIMATE_FUNCTION_FADE_GRADIENT

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   // Apply green gradient, brightest at start

//   uint16_t start = pCONT_iLight->settings.light_size_count/2;
//   uint16_t end = pCONT_iLight->settings.light_size_count; 
//   RgbTypeColor colour_gradient = HsbColor(
//                                           pCONT_iLight->HueN2F(120),
//                                           pCONT_iLight->SatN2F(100),
//                                           pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100))  
//                                           );
//   RgbTypeColor colour_random = RgbTypeColor(255,0,0); 
//   HsbColor colour_random_adjusted = HsbColor(RgbColor(0));
//   uint8_t gradient_end_percentage = 75;
//   uint16_t strip_size_gradient = pCONT_iLight->settings.light_size_count*(gradient_end_percentage/100.0f);
//   uint16_t strip_size_single   = pCONT_iLight->settings.light_size_count*(75/100.0f);
  
//   start = 0;
//   end = pCONT_iLight->settings.light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     animation_colours[ledout.index].DesiredColour = RgbTypeColor(0);
//   }
  
//   //0 to 75% 
//   start = 0;
//   end = map(75,0,100,0,pCONT_iLight->settings.light_size_count);
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     animation_colours[ledout.index].DesiredColour.R = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.R,0));
//     animation_colours[ledout.index].DesiredColour.G = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.G,0));
//     animation_colours[ledout.index].DesiredColour.B = pCONT_iLight->ledGamma(map(ledout.index,start,end,colour_gradient.B,0));
//   }

//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
//   uint8_t desired_pixel;
  
//   // 25 to 100%
//   start = map(25,0,100,0,pCONT_iLight->settings.light_size_count);
//   end = pCONT_iLight->settings.light_size_count;
//   for(ledout.index=start;ledout.index<end;ledout.index++){ 
//     desired_pixel = random(0,pixels-1);
//     colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);
//     if((ledout.index%3)==0){
//       colour_random_adjusted = RgbTypeColor(colour_random);
//       colour_random_adjusted.B = pCONT_iLight->BrtN2F(map(pCONT_iLight->getBriRGB(),0,255,0,100));
//       animation_colours[ledout.index].DesiredColour = colour_random_adjusted;
//     }
//   }

//   #endif

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireplace 1D
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// /**
//  * 1D - meaning there is no significant geography to it, similar to slow glow.
//  * 2D - means an effect will be layered onto a matrix, where flames will be shown to go up the row-axis
//  * 3D - eg cubes, effects will be drawn in 3 dimensions
//  * 
//  * */


// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function__FirePlace_1D_01()
// {
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   //Display on all pixels
//   UpdateDesiredColourFromPaletteSelected();

//   // this should probably force order as random, then introduce static "inorder" option
//   pCONT_iLight->animation.transition.order_id = TRANSITION_ORDER_RANDOM_ID;
  

// #ifdef ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE
//   HsbColor colour_in = HsbColor(RgbColor(0));
  
//  #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
//   //Overwrite random brightness on special range
//   for(uint16_t index=256;index<300;index++){

//     colour_in = animation_colours[index].DesiredColour;

//     if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
//       colour_in.H = 0.0f;
//       colour_in.S = 1.0f;
//     }
//     colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

//     // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
//   // This will be the introduction of segments into my code!!
//     animation_colours[random(256,299)].DesiredColour = colour_in;

//     // animation_colours[random(40,49)].DesiredColour = colour_in;

//   }
  
// #endif 
// #endif // ENABLE_DEVFEATURE_FIREPLACE_SEGMENT_EXTRA_GENERATE

// AddLog(LOG_LEVEL_TEST,PSTR("SubTask_Flasher_Animate_Function__FirePlace_1D_01 %d"),animation_colours[0].DesiredColour.R);

//   // Check if output multiplying has been set, if so, change desiredcolour array
//   // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
//   // Get starting positions already on show
//   UpdateStartingColourWithGetPixel();
//   // Call the animator to blend from previous to new
//   this->setAnimFunctionCallback(
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
//     }
//   );
// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : RGB CLock
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


#ifdef ENABLE_DEVFEATURE_RGB_CLOCK

//add "LCDDisplay_" to relevant functions
void mAnimatorLight::LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing) {
  byte posOffset = 0;                                                                     // this offset will be used to move hours and minutes...
  if ( LED_DIGITS / 2 > 2 ) posOffset = 2;                                                // ... to the left so we have room for the seconds when there's 6 digits available
  if ( displayMode == 0 ) {                                                               // if 12h mode is selected...
    if ( pCONT_time->hourFormat12(t) >= 10 ){
      LCDDisplay_showDigit(1, color + colorSpacing * 2, 3 + posOffset);   // ...and hour > 10, display 1 at position 3
    }
    LCDDisplay_showDigit((pCONT_time->hourFormat12(t) % 10), color + colorSpacing * 3, 2  + posOffset);          // display 2nd digit of HH anyways
  } else if ( displayMode == 1 ) {                                                        // if 24h mode is selected...
    if ( pCONT_time->hour(t) > 9 ) LCDDisplay_showDigit(pCONT_time->hour(t) / 10, color + colorSpacing * 2, 3 + posOffset);  // ...and hour > 9, show 1st digit at position 3 (this is to avoid a leading 0 from 0:00 - 9:00 in 24h mode)
    LCDDisplay_showDigit(pCONT_time->hour(t) % 10, color + colorSpacing * 3, 2 + posOffset);                     // again, display 2nd digit of HH anyways
  }
  LCDDisplay_showDigit((pCONT_time->minute(t) / 10), color + colorSpacing * 4, 1 + posOffset);                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((pCONT_time->minute(t) % 10), color + colorSpacing * 5, 0 + posOffset);                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  if ( posOffset > 0 ) {
    LCDDisplay_showDigit((pCONT_time->second(t) / 10), color + colorSpacing * 6, 1);
    LCDDisplay_showDigit((pCONT_time->second(t) % 10), color + colorSpacing * 7, 0);
  }
  // if ( second(t) % 2 == 0 ) 
  LCDDisplay_showDots(2, 5);//pCONT_time->second(t) * 4.25);                                // show : between hours and minutes on even seconds with the color cycling through the palette once per minute
  lastSecond = pCONT_time->second(t);
}

void mAnimatorLight::LCDDisplay_showSegment(byte segment, byte color, byte segDisplay) {
  
  // This shows the segments from top of the sketch on a given position (segDisplay).
  // pos 0 is the most right one (seen from the front) where data in is connected to the arduino
  byte leds_per_segment = 1 + abs( segGroups[segment][1] - segGroups[segment][0] );            // get difference between 2nd and 1st value in array to get led count for this segment
  if ( segDisplay % 2 != 0 ) segment += 7;                                                  // if segDisplay/position is odd add 7 to segment

  uint16_t pixel_index = 0;

  for (byte i = 0; i < leds_per_segment; i++) {                                             // fill all leds inside current segment with color
    // animation_colours[( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);

    pixel_index = ( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i;

    RgbcctColor colour = ColorFromPaletteLCD(_segments[0].palette.id, color);//RgbColor(255,0,0);

    // RgbcctColor colour = RgbwColor(1,2,3,4);

    uint8_t segment_index = 0;


    SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].data, _segment_runtimes[segment_index]._dataLen, pixel_index, _segments[segment_active_index].colour_type, colour);
  




    // animation_colours[( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  
  
  
  
  
  }
}

void mAnimatorLight::LCDDisplay_showDigit(byte digit, byte color, byte pos) {
  // This draws numbers using the according segments as defined on top of the sketch (0 - 9)
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) LCDDisplay_showSegment(i, color, pos);
  }
}

/**
 * @brief Temporary method: Normal getcolour should work, but needs to include wrap functionality ie when index exceeds size then repeat pattern
 * 
 * @param palette_id 
 * @param desired_index 
 * @param apply_global_brightness 
 * @return RgbcctColor 
 */
RgbcctColor mAnimatorLight::ColorFromPaletteLCD(uint16_t palette_id, uint8_t desired_index, bool apply_global_brightness)
{
  
  mPalette::PALETTELIST::PALETTE *ptr = mPaletteI->GetPalettePointerByID(palette_id);

  uint8_t pixels_max = mPaletteI->GetPixelsInMap(ptr);

  if(desired_index >= pixels_max){
    desired_index %= pixels_max;
    // ALOG_INF(PSTR("desired_index2 = %d"), desired_index);
  }
  
  RgbcctColor colour = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(palette_id), desired_index);

  if(apply_global_brightness){
    pCONT_iLight->ApplyGlobalBrightnesstoColour(&colour);
  }

  return colour;

}



void mAnimatorLight::LCDDisplay_showDots(byte dots, byte color) {

  // // in 12h mode and while in setup upper dots resemble AM, all dots resemble PM
  // byte startPos = LED_PER_DIGITS_STRIP;
  // if ( LED_BETWEEN_DIGITS_STRIPS % 2 == 0 ) {                                                                 // only SE/TE should have a even amount here (0/2 leds between digits)
  //   animation_colours[startPos].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //   if ( dots == 2 ) animation_colours[startPos + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  // } else {                                                                                                    // Regular and XL have 5 leds between digits
  //   animation_colours[startPos].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //   animation_colours[startPos + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //   if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(mPaletteI->currentPalette, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 1].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(mPaletteI->currentPalette, color, brightness, LINEARBLEND);
  //     }
  //   if ( dots == 2 ) {
  //     animation_colours[startPos + 3].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //     animation_colours[startPos + 4].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);
  //     if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 3].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(mPaletteI->currentPalette, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 4].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, color);//colour;// = ColorFromPalette(mPaletteI->currentPalette, color, brightness, LINEARBLEND);
  //     }
  //   }
  // }



}

/****
 * @brief Basic clock with no animations
 */
void mAnimatorLight::SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_01()
{

  AddLog(LOG_LEVEL_TEST, PSTR("_segments[%d].colour_type = %d"), segment_active_index, _segments[segment_active_index].colour_type);

    
  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  ALOG_DBM(PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    ALOG_ERR(PSTR("Not Enough Memory"));
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // Will only work with first segment
  uint8_t segment_index=0;

  for(int i=0;i<93;i++){
    SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].data, _segment_runtimes[segment_index]._dataLen, i, _segments[segment_active_index].colour_type, RgbwColor(0,0,0,0));
  }

  // if(tempcol++>5){
    tempcol=0;
    // } //startcolour

  AddLog(LOG_LEVEL_TEST, PSTR("tempcol=%d"), tempcol);

  LCDDisplay_updateDisplay(tempcol, colorOffset);

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

  this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
    [this](void){
      this->ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
    }
  );

}

void mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}




/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02 *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/


/**
 * Cycles over leds that are on and applies palette
 * */
void mAnimatorLight::LCDDisplay_colorOverlay() {                                                                                       // This "projects" colors on already drawn leds before showing leds in updateDisplay();
  // for (byte i = 0; i < LED_COUNT; i++) {                                                                    // check each led...
  //   if (animation_colours[i].DesiredColour.CalculateBrightness())  {
      
  //     animation_colours[i].DesiredColour = ColorFromPalette(pCONT_iLight->animation.palette.id, startColor + (colorOffset * i));

  //   }
    
  //                                                                                             // ...and if it is lit...
  //     // leds[i] = ColorFromPalette(mPaletteI->currentPalette, startColor + (colorOffset * i), brightness, LINEARBLEND);  // ...assign increasing color from current palette
  // }
}

void mAnimatorLight::LCDDisplay_updateDisplay(byte color, byte colorSpacing) {                                                         // this is what redraws the "screen"
  // FastLED.clear();                                                                                          // clear whatever the leds might have assigned currently...
  
  // displayTime(now(), color, colorSpacing);                                                                  // ...set leds to display the time...
  
  LCDDisplay_displayTime(pCONT_time->Rtc.local_time, color, colorSpacing);
  
  
  // if (overlayMode == 1) LCDDisplay_colorOverlay();                                                                     // ...and if using overlayMode = 1 draw custom colors over single leds
  
  
  
  // if (brightnessAuto == 1) {                                                                                // If brightness is adjusted automatically by using readLDR()...
  //   FastLED.setBrightness(avgLDR);                                                                          // ...set brightness to avgLDR
  // } else {                                                                                                  // ...otherwise...
  //   FastLED.setBrightness(brightness);                                                                      // ...assign currently selected brightness
  // }
}


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02(){
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   //Display on all pixels
//   UpdateDesiredColourFromPaletteSelected();
  pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE_SK6812;
    
  _segments[segment_active_index].colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGBW__ID;
  
  AddLog(LOG_LEVEL_TEST, PSTR("02    _segments[segment_active_index].colour_type = %d"), _segments[segment_active_index].colour_type);

    
  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  
  // for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor(0);}
  uint8_t segment_index=0;

  for(int i=0;i<93;i++){
    // animation_colours[i].DesiredColour = RgbcctColor(0);
    // }
  SetTransitionColourBuffer_DesiredColour(_segment_runtimes[segment_index].data, _segment_runtimes[segment_index]._dataLen, i, _segments[segment_active_index].colour_type, RgbwColor(0,0,0,0));
  }

  // Pick new colours
  //DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);


  // if(tempcol++>5){
    tempcol=0;
    // } //startcolour

  AddLog(LOG_LEVEL_TEST, PSTR("tempcol=%d"), tempcol);

  LCDDisplay_updateDisplay(tempcol, colorOffset);


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10), 0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  


  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

  this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
    [this](void){
      this->ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();
    }
  );


//   // LCDDisplay_displayTime(pCONT_time->Rtc.utc_time,tempcol,0);

// //       uint16_t tempnumber = testnum;

// //       // if(tempnumber%2){
// //       //   showDots();
// //       // }
// //       if ( second(0) % 2 == 0 ) showDots(2, second(0) * 4.25);  


// //       showDigit(
// //         (tempnumber/1)%10,
// //          64, 0);
// //       showDigit(
// //       (tempnumber/10)%10,
// //          64, 1);
// //       showDigit(
// //         (tempnumber/100)%10,
// //          64, 2);
// //       showDigit(
// //         (tempnumber/1000)%10,
// //          64, 3);


// //       // showDigit(11, 64, 2);
// //       // showDigit(12, 64, 1);

// // if(testnum++>9999){testnum=0;}

  
// //  #ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE
// //   //Overwrite random brightness on special range
// //   for(uint16_t index=256;index<300;index++){

// //     colour_in = animation_colours[index].DesiredColour;

// //     if(colour_in.B==0){ //if colour was off, I need to set the colour to a defined value or it willl turn up brightness to show white
// //       colour_in.H = 0.0f;
// //       colour_in.S = 1.0f;
// //     }
// //     colour_in.B = pCONT_iLight->BrtN2F(random(0,10)*10);

// //     // colour_in.H = pCONT_iLight->BrtN2F(random(0,100));
  
// //     animation_colours[random(256,299)].DesiredColour = colour_in;

// //     // animation_colours[random(40,49)].DesiredColour = colour_in;

// //   }
//   /*
//   if (  ( lastLoop - lastColorChange >= colorChangeInterval ) && ( overlayMode == 0 )         // if colorChangeInterval has been reached and overlayMode is disabled...
//      || ( lastLoop - lastColorChange >= overlayInterval ) && ( overlayMode == 1 ) ) {         // ...or if overlayInterval has been reached and overlayMode is enabled...
//     startColor++;                                                                             // increase startColor to "move" colors slowly across the digits/leds
//     updateDisplay(startColor, colorOffset);
//     lastColorChange = millis();
//   }
//   if ( lastSecond != second() ) {                                                             // if current second is different from last second drawn...
//     updateDisplay(startColor, colorOffset);                                                   // lastSecond will be set in displayTime() and will be used for
//     lastSecond = second();                                                                    // redrawing regardless the digits count (HH:MM or HH:MM:SS)
//   }
//   if ( lastKeyPressed == 1 ) {                                                                // if buttonA is pressed...
//     switchBrightness();                                                                       // ...switch to next brightness level
//     updateDisplay(startColor, colorOffset);
//     if ( btnRepeatCounter >= 20 ) {                                                           // if buttonA is held for a few seconds change overlayMode 0/1 (using colorOverlay())
//       if ( overlayMode == 0 ) overlayMode = 1; else overlayMode = 0;
//       updateDisplay(startColor, colorOffset);
//       EEPROM.put(3, overlayMode);                                                             // ...and write setting to eeprom
//       #ifdef nodeMCU                                                                          // on nodeMCU we need to commit the changes from ram to flash to make them permanent
//         EEPROM.commit();
//       #endif
//       btnRepeatStart = millis();
//     }
//   }
//   if ( lastKeyPressed == 2 ) {                                                                // if buttonB is pressed...
//     switchPalette();                                                                          // ...switch between color palettes
//     updateDisplay(startColor, colorOffset);
//     if ( btnRepeatCounter >= 20 ) {                                                           // if buttonB is held for a few seconds change displayMode 0/1 (12h/24h)...
//       if ( displayMode == 0 ) displayMode = 1; else displayMode = 0;
//       updateDisplay(startColor, colorOffset);
//       EEPROM.put(2, displayMode);                                                             // ...and write setting to eeprom
//       #ifdef nodeMCU
//         EEPROM.commit();
//       #endif
//       btnRepeatStart = millis();
//     }
//   }
//   if ( ( lastLoop - valueLDRLastRead >= intervalLDR ) && ( brightnessAuto == 1 ) ) {          // if LDR is enabled and sample interval has been reached...
//     readLDR();                                                                                // ...call readLDR();
//     if ( abs(avgLDR - lastAvgLDR) >= 5 ) {                                                    // only adjust current brightness if avgLDR has changed for more than +/- 5.
//       updateDisplay(startColor, colorOffset);
//       lastAvgLDR = avgLDR;
//       if ( dbg ) { Serial.print(F("Updated display with avgLDR of: ")); Serial.println(avgLDR); }
//     }
//     valueLDRLastRead = millis();
//   }
//   if ( lastKeyPressed == 12 ) {                                                               // if buttonA + buttonB are pushed at the same time....
//     #ifdef useWiFi                                                                            // ...and if using WiFi...
//       initWPS();                                                                              // ...start WPS
//     #else                                                                                     // otherwise (arduino + rtc or nodemcu + rtc)...
//       setupClock();                                                                           // ...start manual setup
//     #endif
//   }
//   #ifdef nodeMCU                                                                              // On Arduino SetSyncProvider will be used. So this will sync internal time to rtc/ntp on nodeMCU only
//     if ( ( hour() == 3 || hour() == 9 || hour() == 15 || hour() == 21 ) &&                    // if hour is 3, 9, 15 or 21 and...
//          ( minute() == 3 && second() == 0 ) ) {                                               // minute is 3 and second is 0....
//       if ( dbg ) Serial.print(F("Current time: ")); Serial.println(now());
//       #ifdef useWiFi
//         syncNTP();                                                                            // ...either sync using ntp or...
//       #else
//         setTime(Rtc.GetDateTime());                                                           // ...set internal time to rtc time...
//       #endif
//       if ( dbg ) Serial.print(F("New time: ")); Serial.println(now());
//     }
//     ESP.wdtFeed();                                                                            // feed the watchdog each time loop() is cycled through, just in case...
//   #endif
//   FastLED.show();                                                                             // run FastLED.show() every time to avoid color flickering at low brightness settings
//   lastKeyPressed = readButtons();
//   lastLoop = millis();
//   if ( dbg ) dbgInput();   */
// // #endif 

//   // Check if output multiplying has been set, if so, change desiredcolour array
//   // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
//   // Get starting positions already on show
//   UpdateStartingColourWithGetPixel();
//   // Call the animator to blend from previous to new
//   this->setAnimFunctionCallback(
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_AnimationColour_LinearBlend(param);
//     }
//   );
}

void mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01()
{
  
  AddLog(LOG_LEVEL_TEST, PSTR("Numbers_Basic    _segments[].colour_type = %d"), _segments[segment_active_index].colour_type);
  AddLog(LOG_LEVEL_TEST, PSTR("lcd_display_show_number = %d"), lcd_display_show_number);


  

  _segments[segment_active_index].colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID;
    


  
  uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    _segments[segment_active_index].mode_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  
  // Pick new colours
  //DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);

/**
 * @brief Temporary force until handled in code
 * 
 */
  // pCONT_set->Settings.light_settings.type = LT_ADDRESSABLE_WS281X;
  _segments[segment_active_index].colour_type = RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID;


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  
  // lcd_display_show_number = 67;

  LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  


  // Check if output multiplying has been set, if so, change desiredcolour array
  // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
    }
  );

}





#endif // ENABLE_DEVFEATURE_RGB_CLOCK



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Popping Brightness (the accidental "Pop fade")
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary(){

//    switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
//       // }

//       _segment_runtimes[segment_active_index].rgbcct_controller->setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along

    
//       UpdateStartingColourWithGetPixel();

//       uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//       //#ifndef USE_WS28XX_FEATURE_4_PIXEL_TYPE //tmp fix
//       //HsbColor hsb = RgbColor(GetPixelColor(index_random));
//       //#else
//       HsbColor hsb = RgbColor(GetPixelColor(index_random));
//       //#endif

//       uint8_t brightness_now = mapvalue(pCONT_iLight->BrtF2N(hsb.B),0,100,0,255);

//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE %d %d"), brightness_now, flashersettings.brightness_max);

//       if(brightness_now < flashersettings.brightness_max){ //+5 to margins so its not the same
//         hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//         // animation_colours[index_random].DesiredColour = ApplyBrightnesstoDesiredColour(animation_colours[index_random].DesiredColour, flashersettings.brightness_max);
//         animation_colours[index_random].DesiredColour = hsb;
//       }

//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_Generic_AnimationColour_LinearBlend(param); });
//     break;
//   }

// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On_Fade_Off(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   do{ //must complete the pair together
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//         //#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_COLOUR_SELECT"));
//         //#endif

//         pCONT_iLight->settings.light_size_count = 100;

//         // Go through strip, randomly change some to coloured or black
//         mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//         uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
//         RgbTypeColor colour_random = RgbTypeColor(0);

//         uint16_t random_pixel_index = 0;

//         // Serial.printf("DesiredColour1=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
//         //   animation_colours[0].DesiredColour = GetPixelColor(0);
//         // Serial.printf("DesiredColour2=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
//         // animation_colours[0].DesiredColour.Darken(20);
//         // Serial.printf("DesiredColour3=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
//         // Serial.printf("DesiredColour1=%d,%d,%d\n\r",animation_colours[0].DesiredColour.R,animation_colours[0].DesiredColour.G,animation_colours[0].DesiredColour.B);
        
//         // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//         //   animation_colours[index].DesiredColour = GetPixelColor(index);
//         //   // animation_colours[index].DesiredColour.Darken(20);//Darken(10);// = GetPixelColor(index);
        
//         // }


//         UpdateDesiredColourWithGetPixel();

//         // randomly blank most of them out again
// //        uint8_t colour_or_black = 0;
//         uint16_t pixels_to_update = 5;//pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage.val);
//         Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         for(uint16_t index=0; index<pixels_to_update; index++ ){
//           random_pixel_index = index;//random(0,100);//pCONT_iLight->settings.light_size_count);
//           // colour_or_black = random(0,1);

//           // // Check if pixels is already on, if so, make dark
//           // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
//           //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//           // }else
//           // // Previously off, pick new state
//           // {
//             // if(colour_or_black){ // 80% in favour of black/off
//             //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//             // }else{
              
//               uint8_t desired_pixel = random(0,4);//pixels-1);
//               colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  

// //if already on, dont change the colour
//             if(!animation_colours[random_pixel_index].DesiredColour.CalculateBrightness()){// if off, allow new colour 
//               if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//                 colour_random = ApplyBrightnesstoRgbcctColour(colour_random,pCONT_iLight->getBriRGB_Global());
//               }
//               animation_colours[random(0,10)].DesiredColour = colour_random;//RgbColor(0,0,255);//
//             }else{
              
//               animation_colours[index].DesiredColour.Darken(2);//Darken(10);// = GetPixelColor(index);
//             }
           
//           // }
//         }
//         // if(test_flag){
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,0,255);
//         // }else{
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,255,0);
//         // }
//         //  for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//         //   animation_colours[index].DesiredColour.Darken(10);//Darken(10);// = GetPixelColor(index);
//         // }


//         AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

//         // Get starting positions already on show
//         UpdateStartingColourWithGetPixel();
//         // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


//         // dont automatically run animate again, add timeout for it
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;

//       }break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         //#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_ANIMATE"));
//         //#endif

//         this->setAnimFunctionCallback(
//           [this](const AnimationParam& param){
//             this->AnimationProcess_Pulse_Random_On_Fade_Off(param);
//           }
//         );

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           ALOG_DBM( PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
        
//         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

// }


// // 50% progress is full brightness, 0 and 100% is off
// void mAnimatorLight::AnimationProcess_Pulse_Random_On_Fade_Off(const AnimationParam& param)
// {    
//  RgbTypeColor updatedColor;
//  pCONT_iLight->settings.light_size_count = 10;
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
    
//     if(pixel==0){
//       // Serial.printf("[%d]\t%d | %d\n\r",pixel,animation_colours[pixel].StartingColor.CalculateBrightness(),animation_colours[pixel].DesiredColour.CalculateBrightness());
//     }
//     //if desired is brighter than starting, don't blend, do instant (ie pulsed on not fading off)
//     if(animation_colours[pixel].StartingColor.CalculateBrightness() <= animation_colours[pixel].DesiredColour.CalculateBrightness()){
//       // Serial.println("<<<<<<<<<<<<<<<<<<<<<");
//       updatedColor = animation_colours[pixel].DesiredColour; // instant to brighter colour
//     }
//     // Fade down if new brightness is lower
//      else{
//        //Serial.println(">>>>>>>>>>>>>>>>>");
//       updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     }


//     SetPixelColor(pixel, updatedColor);
//   } // END for
  
//   // AddLog(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On(){

//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   UpdateDesiredColourFromPaletteSelected();
//   // randomly blank most of them out again
//   uint16_t pixels_to_clear = 
//   pCONT_iLight->animation.transition.pixels_to_update_as_number;
//   //pCONT_iLight->settings.light_size_count-pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//   Serial.printf("pixels_to_clear=%d\n\r",pixels_to_clear);
//   for(uint16_t index=0; index<pixels_to_clear; index++ ){
//     animation_colours[random(0,pCONT_iLight->settings.light_size_count)].DesiredColour = RgbColor(0);
//   }
//   #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
//   // Check if output multiplying has been set, if so, change desiredcolour array
//   OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
//   #endif// ENABLE_PIXEL_FUNCTION_PIXELGROUPING
//   // Get starting positions already on show
//   // UpdateStartingColourWithGetPixel();
//   // Call the animator to blend from previous to new
        
//   this->setAnimFunctionCallback(
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Pulse_Random_On(param);
//     }
//   );

// }


// // 50% progress is full brightness, 0 and 100% is off
// void mAnimatorLight::AnimationProcess_Pulse_Random_On(const AnimationParam& param)
// {    

//   // float progress_percentage = param.progress*100; 
//   float progress_half_range = param.progress < 0.5 ? 
//                                   mSupport::mapfloat(param.progress, 0,0.5, 0,1) : 
//                                   mSupport::mapfloat(param.progress, 0.5,1, 1,0) ;

//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//         RgbColor(0),
//         animation_colours[pixel].DesiredColour,
//         progress_half_range);    //0-50%
//     SetPixelColor(pixel, updatedColor);
//   } // END for

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Pulse_Random_On_2(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
  
//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;
  
//   do{ //must complete the pair together
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_COLOUR_SELECT"));
//         #endif

//         // Check if output multiplying has been set, if so, change desiredcolour array
//         // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();

//         // test_flag^=1;

//         // Go through strip, randomly change some to coloured or black
//         mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//         uint8_t pixels = mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr);
//         RgbTypeColor colour_random = RgbTypeColor(0);

//         uint16_t random_pixel_index = 0;
        
//         for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//           animation_colours[index].DesiredColour = GetPixelColor(index);
//         }

//         // randomly blank most of them out again
//         uint8_t colour_or_black = 0;
//         uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
//         // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//         Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         for(uint16_t index=0; index<pixels_to_update; index++ ){
//           random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
//           colour_or_black = random(0,1);

//           // // Check if pixels is already on, if so, make dark
//           // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
//           //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//           // }else
//           // // Previously off, pick new state
//           // {
//             if(colour_or_black){ // 80% in favour of black/off
//               animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//             }else{
//               uint8_t desired_pixel = random(0,pixels-1);
//               colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  
//               // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//               //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//               // }
//               animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//            }
//           // }
//         }
//         // if(test_flag){
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,0,255);
//         // }else{
//         //   animation_colours[random_pixel_index].DesiredColour =  RgbColor(0,255,0);
//         // }

//         // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].DesiredColour.G,animation_colours[random_pixel_index].DesiredColour.B);

//         // Get starting positions already on show
//         UpdateStartingColourWithGetPixel();
//         // AddLog(LOG_LEVEL_TEST, PSTR("From G,B=%d,%d"),animation_colours[random_pixel_index].StartingColor.G,animation_colours[random_pixel_index].StartingColor.B);


//         // dont automatically run animate again, add timeout for it
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;

//       }break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "Random_On_2 EFFECTS_ANIMATE"));
//         #endif

//         this->setAnimFunctionCallback(
//           [this](const AnimationParam& param){
//             this->AnimationProcess_Pulse_Random_On_2(param);
//           }
//         );

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           ALOG_DBM( PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
        
//         flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
        
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);

// }


// // 50% progress is full brightness, 0 and 100% is off
// void mAnimatorLight::AnimationProcess_Pulse_Random_On_2(const AnimationParam& param)
// {    
//  RgbTypeColor updatedColor;
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SetPixelColor(pixel, updatedColor);
//   } // END for
  
//   // AddLog(LOG_LEVEL_TEST, PSTR("-->> G,B=%d,%d"),updatedColor.G, updatedColor.B);

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Christmas style "slow fade brightness"
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturation fade (all together)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturation fade (wipe)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturatoin fade (randomly selected)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// //to be merged with slow glow maybe? or advanced slow glow with colour and/or brightness are glows, equally, saturation can be manipulated.
// // A struct with what aspects of the "glows" should be could be idenitified here (brt, sat, colours, palettes)


// void mAnimatorLight::SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness(){

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

//   flashersettings.flag_finish_flasher_pair = false;
//   flashersettings.flags.enable_random_rate = true;

//   // Best "inorder" to compute colour again then brightness
  
//   do{ //must complete the pair together //move inside functions
//     switch(flashersettings.region){
//       case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//         #endif
//         UpdateDesiredColourFromPaletteSelected();
//         flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//       break;
//       case EFFECTS_REGION_ANIMATE_ID: //shift along
//         #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
//         //AddLog(LOG_LEVEL_DEBUG_MORE_LOWLEVEL,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//         #endif
//         // RotateDesiredColour(1,flashersettings.flags.movement_direction);

//         // Calculate new pattern to change
//         RefreshLEDIndexPattern();

//         // Global brightness is already applied, and will be known as "max range"
//         // Min range will be another map change here
//         uint8_t max_brightness = _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessRGB255();
//         uint8_t min_brightness = flashersettings.brightness_min;
//         uint8_t random_brightness = 0;

//         if(min_brightness > max_brightness){
//           min_brightness = max_brightness;
//         }

//         // Change only selected pixel brightness levels
//         for(ledout.index=0;ledout.index<strip_size_requiring_update;ledout.index++){
//           random_brightness = random(min_brightness,max_brightness);          
//           animation_colours[ledout.pattern[ledout.index]].DesiredColour = ApplyBrightnesstoRgbcctColour(
//                                                                               animation_colours[ledout.pattern[ledout.index]].DesiredColour,
//                                                                               random_brightness);
//         }

//         if(flashersettings.flags.force_finish_flasher_pair_once){
//           ALOG_DBM( PSTR("flashersettings.flags.force_finish_flasher_pair_once WAS SET"));
//           flashersettings.flags.force_finish_flasher_pair_once = false;
//         }
//       break;
//     }
//   }while(flashersettings.flag_finish_flasher_pair || flashersettings.flags.force_finish_flasher_pair_once);


// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// // Make this an "Effects" which is public/friendly of animator, but not the same class
// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01(){

  
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_SunRise_Alarm_01"));

//   //using group 01
//   // struct{}
//   //use struct and memcpy to get data, then store results

//   //effects_data_buffer
//   //each needs 64 byte
// // typedef struct particle {
// //   // CRGB     color;
// //   uint32_t birth  =0;
// //   uint32_t last   =0;
// //   float    vel    =0;
// //   uint16_t pos    =-1;
// //   float    fragment[12];
// // } star;

// /**
//  * How the effects_data_buffer is formatted during effect runtime
//  * */
// struct EFFECT_CONFIG {
//   uint8_t starting_palette_id = 0;
//   uint8_t ending_palette_id = 10;
//   uint8_t step_changes_remaining = 10;
//   uint8_t colour_select_progress_counter = 0;
//   uint8_t active_palette_id;

// } effect_config;


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster




//   // switch(flashersettings.region){
//   //   case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours


//       // if(effect_config2->active_palette_id++>mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
//       //   effect_config2->active_palette_id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
//       // }
//       if(pCONT_iLight->animation.palette.id++>=mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
//         pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
//       }


// AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);


//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       UpdateDesiredColourFromPaletteSelected();



//       UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_SunPositions_Gradient_Alarm_01(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }

// void mAnimatorLight::AnimationProcess_SunPositions_Gradient_Alarm_01(const AnimationParam& param)
// {   
//   // Need to make rgbcct pixels possible in the future, with dynamic animation_state

//    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SetPixelColor(pixel, updatedColor);
//   } // END for

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// // Make this an "Effects" which is public/friendly of animator, but not the same class
// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(){

  
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SunPositions_Gradient_Sun_Elevation_And_Azimuth_01"));

//   //using group 01
//   // struct{}
//   //use struct and memcpy to get data, then store results

//   //effects_data_buffer
//   //each needs 64 byte
// // typedef struct particle {
// //   // CRGB     color;
// //   uint32_t birth  =0;
// //   uint32_t last   =0;
// //   float    vel    =0;
// //   uint16_t pos    =-1;
// //   float    fragment[12];
// // } star;

// /**
//  * How the effects_data_buffer is formatted during effect runtime
//  * */
// struct EFFECT_CONFIG {
//   uint16_t index_circle_center;
  
//   uint8_t sun_width;
//   uint8_t sun_bloom;
//   RgbcctColor sky_near_colour;
//   RgbcctColor sky_far_colour;

//   uint8_t value = 0;





//   uint8_t starting_palette_id;
//   uint8_t ending_palette_id;
//   uint8_t step_changes_remaining;
//   uint8_t colour_select_progress_counter;
//   uint8_t active_palette_id;
//   uint8_t sun_elevation_degrees;
//   uint8_t sun_azimuth_degrees; //left/right from sunrise/sunset position
//   // uint8_t sun_bloom; //how wide the main sunlight is
//   //store colours in here too
//   // uint8_t sun_elevation_degrees;


//   uint8_t direction;

//   //map "visible" azimuth into far left/right of strip

// } effect_config;


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

//   AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

//   pCONT_iLight->settings.light_size_count = 100;
  
//   // Get all the current state
//   // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = GetPixelColor(index);
//   // }
//     for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//     animation_colours[index].DesiredColour = RgbcctColor(0);//,0,30,0,0);//,0,10,0,0);
//   }

//   //scan back and forth
//   // if(effect_config2->index_circle_center<=0){
//   //   effect_config2->direction = 1;
//   // }else
//   // if(effect_config2->index_circle_center>100){
//   //   effect_config2->direction = 0;
//   // }

//   // if(effect_config2->direction){          
//   //   effect_config2->index_circle_center++;
//   // }else{
//   //   effect_config2->index_circle_center--;
//   // }

//   effect_config2->index_circle_center = 50;
  
//   if(effect_config2->value >= 0){
//     // Draw full sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(0,0,50,0,0), //center
//       RgbcctColor(0,0,0,0,0), //circum
//       effect_config2->index_circle_center, //
//       50,  // tail_size
//       5, // center region
//       true
//     );
//   }


//   // widden
//   if(effect_config2->value >= 1){

//     // Draw near sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(20,5,0,0,0), //center
//       RgbcctColor(0,0,0,0,0), //circum
//       effect_config2->index_circle_center, //
//       25,  // tail_size
//       10, // center region
//       true
//     );
//   }

//   if(effect_config2->value >= 2){
//     // Draw near sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(100,20,0,0,0), //center
//       RgbcctColor(0,0,0,0,0), //circum
//       effect_config2->index_circle_center, //
//       30,  // tail_size
//       2, // center region
//       true
//     );
//   }

//   // widden
//   if(effect_config2->value >= 3){

//     // Draw near sky
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(0,50,50,0,0), //center
//       RgbcctColor(0,100,75,0,0), //circum
//       effect_config2->index_circle_center, //
//       60,  // tail_size
//       10, // center region
//       true
//     );
//   }


//   if(effect_config2->value >= 4){
//     // Draw sun bloom
//     Draw_DesiredColour_LinearGradientMirrored2(
//       RgbcctColor(200,100,0,0,0), //center
//       RgbcctColor(50, 44, 3,     0,0), //circum
//       effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
//       50,
//       5, 
//       true
//     );
//   }

//   // if(effect_config2->value == 4){
//   //   // Draw sun bloom
//   //   Remove_DesiredColour_LinearGradientMirrored(
//   //     RgbcctColor(0,0,  50,       0,0), //center
//   //     RgbcctColor(0, 0, 10,     0,0), //circum
//   //     effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
//   //     50,
//   //     5, 
//   //     true
//   //   );
//   // }


  
//   if(effect_config2->value++ > 6){ effect_config2->value = 0; }

//         // pixel_radius_as_number = 10;
//         // pixel_position_center = effect_config2->index_circle_center;
//         // pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
//         // for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//         //   // Create new index
//         //   index_generate = effect_config2->index_circle_center-index;              
//         //   //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//         //   if((index_generate>=0)&&(index_generate<100)){
//         //     //Draw half, larger index side
//         //     progress = mSupport::mapfloat(index_generate,
//         //                                     pixel_position_center,pixel_position_circum,
//         //                                     0,1
//         //                                   );
//         //     animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
//         //                                                                                 colour_circle_center,
//         //                                                                                 colour_circle_circumference, 
//         //                                                                                 //RgbColor(255,0,0), RgbColor(0,255,0)
//         //                                                                                 progress);
//         //   } // if test

//         // }





// // //use signed value to hold index_adjusted
// // int32_t index_generate = 0;
// // uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center+pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center+index;   
          
// //           //Draw half, larger index side
// //           progress = mSupport::mapfloat(index_generate,
// //                                           pixel_position_center,pixel_position_circum,
// //                                           0,1
// //                                         );
// //           colour_draw = RgbcctColor::LinearBlend(
// //                                         colour_circle_center,
// //                                         colour_circle_circumference, 
// //                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                         progress);



// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             animation_colours[index_generate].DesiredColour = colour_draw;
// //           } // if test

// //         }

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center-index;              
// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             //Draw half, larger index side
// //             progress = mSupport::mapfloat(index_generate,
// //                                             pixel_position_center,pixel_position_circum,
// //                                             0,1
// //                                           );
// //             animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
// //                                                                                         colour_circle_center,
// //                                                                                         colour_circle_circumference, 
// //                                                                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                                                                         progress);
// //           } // if test

// //         }





//           // index_generate = effect_config2->index_circle_center-index+10;
//           // start_pixel_position = index_generate;
//           // end_pixel_position = index_generate+10;
//           // if((index_generate>=0)&&(index_generate<100)){
//           //   //Draw half, samller index side
//           //   progress = mSupport::mapfloat(index_generate,start_pixel_position,end_pixel_position,0,1);
//           //   animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(RgbColor(255,0,255), RgbColor(0,0,255), progress);
              
              
//           //     // colour_circle_circumference, colour_circle_center, progress);
//           // }// out of range

//         // }


//         // randomly blank most of them out again
//         // uint8_t colour_or_black = 0;
//         // uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
//         // // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//         // Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         // for(uint16_t index=0; index<pixels_to_update; index++ ){
//         //   random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
//         //   colour_or_black = random(0,1);

//         //   // // Check if pixels is already on, if so, make dark
//         //   // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
//         //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//         //   // }else
//         //   // // Previously off, pick new state
//         //   // {
//         //     if(colour_or_black){ // 80% in favour of black/off
//         //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
//         //     }else{
//         //       uint8_t desired_pixel = random(0,pixels-1);
//         //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  
//         //       // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//         //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//         //       // }
//         //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//         //    }
//         //   // }
//         // }




//       // UpdateDesiredColourFromPaletteSelected();


//       UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }

// void mAnimatorLight::AnimationProcess_SunPositions_Gradient_Sun_Elevation_And_Azimuth_01(const AnimationParam& param)
// {   
//   // Need to make rgbcct pixels possible in the future, with dynamic animation_state

//    for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SetPixelColor(pixel, updatedColor);
//   } // END for

// }




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * Instead of direct Elevation to palette, this animation will perform different versions of the mapping. Should this be one animation with subtypes? or different types
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01()
// {

//    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//    /**
//     * Setting that an mqtt debug exists
//     * */
//    pCONT_iLight->animation.debug_mqtt_response_available = 1;

//   #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   if(pCONT_iLight->animation.debug_mqtt_response_available && pCONT_iLight->animation.flags.animator_first_run)
//   {
//     this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
//       [this](void){
//         this->ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01();
//       }
//     );
//   //   AddLog(LOG_LEVEL_INFO, PSTR("ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01 FIRST RUN"));
//   //   delay(5000);
//   // }else{
//   //   AddLog(LOG_LEVEL_INFO, PSTR("NOT SETTING"));

//   }
//   #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   // delay(5000);

//   // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
 
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

//   // delay(1000);

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels_max = mPaletteI->GetPixelsInMap(palette_p);
//   // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   int16_t indexing = 0;  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;  
//   float upper_boundary_value = 90;
//   float sun_positions_from_palette_index[pixels_max];  
//   uint8_t searching_matched_index = 0;

//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }


//   for(int i=0;i<pixels_max;i++)
//   {
//     // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       //Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }

//     // Directly, manually, check the last memory space

//     if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
//     {
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       break;

//     }




//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */
//   if(lower_boundary_index>=pixels_max)
//   {
//     lower_boundary_index = 0;
//     Serial.printf("lower_boundary_index>=pixels_max\n\r");
//   }
//   if(upper_boundary_index>=pixels_max)
//   {
//     upper_boundary_index = pixels_max;
//     Serial.printf("upper_boundary_index>=pixels_max\n\r");
//   }

//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   Serial.printf("numer=\t%f\n\r",numer);
//   Serial.printf("denum=\t%f\n\r",denum);
//   Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

//   /**
//    * Showing the colours
//    * 1) previous
//    * 2) next
//    * 3) linearblend of the exact new colour
//    * */

//   RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//   RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//   // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//   //   lower_boundary_value, lower_boundary_index, 
//   //   upper_boundary_value, upper_boundary_index, progress_between_colours);

//   RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

//   RgbcctColor c = c_lower; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;


//   /**
//    * Evening/Sunset, keep the brightness up
//    * Morning/Sunrise, wait until closer before increasing brighntess (adjust by elevation_adjust or by time_adjust)
//    * Instead of elevation mapping, use sunrise and sunset time to control animations
//    * */


//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!_segment_runtimes[segment_active_index].rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessRGB255(),
//       _segment_runtimes[segment_active_index].rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//   pCONT_iLight->animation.flags.animator_first_run = false; // process completed, so lets not redo things above
   
// }



// void mAnimatorLight::ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01()
// {   
// // Serial.println("AnimationProcess_Generic_AnimationColour_LinearBlend");
//   // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//   //       animation_colours[pixel].StartingColor,
//   //       animation_colours[pixel].DesiredColour,
//   //       param.progress);    
//   //   SetPixelColor(pixel, updatedColor);
//   // } // END for

//   // DEBUG_LINE_HERE;

//   JBI->Add("test2","debug2");

//     // SetPixelColor(0, RgbColor(0,random(0,255),0));
// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/




// // Make this an "Effects" which is public/friendly of animator, but not the same class
// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01(){

  
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_01"));

//   //using group 01
//   // struct{}
//   //use struct and memcpy to get data, then store results

//   //effects_data_buffer
//   //each needs 64 byte
// // typedef struct particle {
// //   // CRGB     color;
// //   uint32_t birth  =0;
// //   uint32_t last   =0;
// //   float    vel    =0;
// //   uint16_t pos    =-1;
// //   float    fragment[12];
// // } star;

// /**
//  * How the effects_data_buffer is formatted during effect runtime
//  * */
// struct EFFECT_CONFIG {
//   uint16_t index_circle_center;
  
//   uint8_t sun_width;
//   uint8_t sun_bloom;
//   RgbcctColor sky_near_colour;
//   RgbcctColor sky_far_colour;

//   uint8_t value = 0;





//   uint8_t starting_palette_id;
//   uint8_t ending_palette_id;
//   uint8_t step_changes_remaining;
//   uint8_t colour_select_progress_counter;
//   uint8_t active_palette_id;
//   uint8_t sun_elevation_degrees;
//   uint8_t sun_azimuth_degrees; //left/right from sunrise/sunset position
//   // uint8_t sun_bloom; //how wide the main sunlight is
//   //store colours in here too
//   // uint8_t sun_elevation_degrees;


//   uint8_t direction;

//   //map "visible" azimuth into far left/right of strip

// } effect_config;

// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

//   AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

//   // pCONT_iLight->settings.light_size_count = 1;
  
//   // Get all the current state
//   // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = GetPixelColor(index);
//   // }
//   //   for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = RgbcctColor(9,255,0,0,0);//,0,30,0,0);//,0,10,0,0);
//   // }


//   /**
//    * Colour temp of white channels when day time
//    * Amount of blue when sun is down
//    * */

//   if(sun_elevation)
//   {
//     /**
//      * Max daily elevation will need to be known later
//      * */
//     // Mapping elevation to CCT percentage

//     // pCONT_iLight->CommandSet_ActiveSolidPalette_ColourTemp_Percentage(percentage_cct);

//   }else{
    
//     for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//       animation_colours[index].DesiredColour = RgbcctColor(0,0,255,0,0);//,0,30,0,0);//,0,10,0,0);
//     }
//   }

// /**
//  * colour channels
//  * */

//   if((sun_elevation>-50)&&(sun_elevation<10))
//   {
//     // HsbColor hsb = HsbColor(RgbColor(0));
//     // hsb.H = map(sun_elevation,-50,10,255,0)

//     uint8_t blue =  map(sun_elevation,-50,10,255,0);

//     _segment_runtimes[segment_active_index].rgbcct_controller->setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




//     uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

//     _segment_runtimes[segment_active_index].rgbcct_controller->setBrightnessRGB255(brightness_255);

//   }

// /**
//  * WHITE CHANNEL
//  * */
//   if((sun_elevation>-10)&&(sun_elevation<25))
//   {


//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-10,25,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
//     // Set the colour temp
//     _segment_runtimes[segment_active_index].rgbcct_controller->setCCT(cct_val);


//     uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);

//     _segment_runtimes[segment_active_index].rgbcct_controller->setBrightnessCCT255(brightness_255);

//   }




//   //scan back and forth
//   // if(effect_config2->index_circle_center<=0){
//   //   effect_config2->direction = 1;
//   // }else
//   // if(effect_config2->index_circle_center>100){
//   //   effect_config2->direction = 0;
//   // }

//   // if(effect_config2->direction){          
//   //   effect_config2->index_circle_center++;
//   // }else{
//   //   effect_config2->index_circle_center--;
//   // }

//   effect_config2->index_circle_center = 50;
  
//   // if(effect_config2->value >= 0){
//   //   // Draw full sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(0,0,50,0,0), //center
//   //     RgbcctColor(0,0,0,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     50,  // tail_size
//   //     5, // center region
//   //     true
//   //   );
//   // }


//   // // widden
//   // if(effect_config2->value >= 1){

//   //   // Draw near sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(20,5,0,0,0), //center
//   //     RgbcctColor(0,0,0,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     25,  // tail_size
//   //     10, // center region
//   //     true
//   //   );
//   // }

//   // if(effect_config2->value >= 2){
//   //   // Draw near sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(100,20,0,0,0), //center
//   //     RgbcctColor(0,0,0,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     30,  // tail_size
//   //     2, // center region
//   //     true
//   //   );
//   // }

//   // // widden
//   // if(effect_config2->value >= 3){

//   //   // Draw near sky
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(0,50,50,0,0), //center
//   //     RgbcctColor(0,100,75,0,0), //circum
//   //     effect_config2->index_circle_center, //
//   //     60,  // tail_size
//   //     10, // center region
//   //     true
//   //   );
//   // }


//   // if(effect_config2->value >= 4){
//   //   // Draw sun bloom
//   //   Draw_DesiredColour_LinearGradientMirrored2(
//   //     RgbcctColor(200,100,0,0,0), //center
//   //     RgbcctColor(50, 44, 3,     0,0), //circum
//   //     effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
//   //     50,
//   //     5, 
//   //     true
//   //   );
//   // }

//   // if(effect_config2->value == 4){
//   //   // Draw sun bloom
//   //   Remove_DesiredColour_LinearGradientMirrored(
//   //     RgbcctColor(0,0,  50,       0,0), //center
//   //     RgbcctColor(0, 0, 10,     0,0), //circum
//   //     effect_config2->index_circle_center, //D_RGB255_ORANGE_DEEP
//   //     50,
//   //     5, 
//   //     true
//   //   );
//   // }


  
//   if(effect_config2->value++ > 6){ effect_config2->value = 0; }

//         // pixel_radius_as_number = 10;
//         // pixel_position_center = effect_config2->index_circle_center;
//         // pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
//         // for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
//         //   // Create new index
//         //   index_generate = effect_config2->index_circle_center-index;              
//         //   //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
//         //   if((index_generate>=0)&&(index_generate<100)){
//         //     //Draw half, larger index side
//         //     progress = mSupport::mapfloat(index_generate,
//         //                                     pixel_position_center,pixel_position_circum,
//         //                                     0,1
//         //                                   );
//         //     animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
//         //                                                                                 colour_circle_center,
//         //                                                                                 colour_circle_circumference, 
//         //                                                                                 //RgbColor(255,0,0), RgbColor(0,255,0)
//         //                                                                                 progress);
//         //   } // if test

//         // }





// // //use signed value to hold index_adjusted
// // int32_t index_generate = 0;
// // uint16_t pixel_position_center, pixel_position_circum, pixel_radius_as_number;

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center+pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center+index;   
          
// //           //Draw half, larger index side
// //           progress = mSupport::mapfloat(index_generate,
// //                                           pixel_position_center,pixel_position_circum,
// //                                           0,1
// //                                         );
// //           colour_draw = RgbcctColor::LinearBlend(
// //                                         colour_circle_center,
// //                                         colour_circle_circumference, 
// //                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                         progress);



// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             animation_colours[index_generate].DesiredColour = colour_draw;
// //           } // if test

// //         }

// //         pixel_radius_as_number = 10;
// //         pixel_position_center = effect_config2->index_circle_center;
// //         pixel_position_circum = effect_config2->index_circle_center-pixel_radius_as_number;
// //         for(uint16_t index=0; index<pixel_radius_as_number; index++ ){
// //           // Create new index
// //           index_generate = effect_config2->index_circle_center-index;              
// //           //Only write into colour buffer if valid index. Otherwise, consider this an "overscan"
// //           if((index_generate>=0)&&(index_generate<100)){
// //             //Draw half, larger index side
// //             progress = mSupport::mapfloat(index_generate,
// //                                             pixel_position_center,pixel_position_circum,
// //                                             0,1
// //                                           );
// //             animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(
// //                                                                                         colour_circle_center,
// //                                                                                         colour_circle_circumference, 
// //                                                                                         //RgbColor(255,0,0), RgbColor(0,255,0)
// //                                                                                         progress);
// //           } // if test

// //         }





//           // index_generate = effect_config2->index_circle_center-index+10;
//           // start_pixel_position = index_generate;
//           // end_pixel_position = index_generate+10;
//           // if((index_generate>=0)&&(index_generate<100)){
//           //   //Draw half, samller index side
//           //   progress = mSupport::mapfloat(index_generate,start_pixel_position,end_pixel_position,0,1);
//           //   animation_colours[index_generate].DesiredColour = RgbcctColor::LinearBlend(RgbColor(255,0,255), RgbColor(0,0,255), progress);
              
              
//           //     // colour_circle_circumference, colour_circle_center, progress);
//           // }// out of range

//         // }


//         // randomly blank most of them out again
//         // uint8_t colour_or_black = 0;
//         // uint16_t pixels_to_update = pCONT_iLight->animation.transition.pixels_to_update_as_number;
//         // // pCONT_iLight->GetPixelsToUpdateAsNumberFromPercentage(pCONT_iLight->animation.transition.pixels_to_update_as_percentage);
//         // Serial.printf("pixels_to_clear=%d\n\r",pixels_to_update);
//         // for(uint16_t index=0; index<pixels_to_update; index++ ){
//         //   random_pixel_index = random(0,pCONT_iLight->settings.light_size_count);
//         //   colour_or_black = random(0,1);

//         //   // // Check if pixels is already on, if so, make dark
//         //   // if(pCONT_iLight->RgbColorto32bit(GetPixelColor(random_pixel_index))>0){
//         //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//         //   // }else
//         //   // // Previously off, pick new state
//         //   // {
//         //     if(colour_or_black){ // 80% in favour of black/off
//         //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
//         //     }else{
//         //       uint8_t desired_pixel = random(0,pixels-1);
//         //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel);  
//         //       // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//         //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//         //       // }
//         //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//         //    }
//         //   // }
//         // }

// pCONT_iLight->animation.palette.id = 10;

//  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   animation_colours_rgbcct.StartingColor = GetPixelColor();
//   // GetPixelColourHardwareInterface();

//   // AddLog(LOG_LEVEL_TEST, PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

//   // // Call the animator to blend from previous to new
//   // this->setAnimFunctionCallback(
//   //   [this](const AnimationParam& param){
//   //     this->AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(param);
//   //   }
//   // );


//   //     // UpdateDesiredColourFromPaletteSelected();


//   //     UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//         this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette.id=%d"),pCONT_iLight->animation.palette.id);

// /**
//  * 
//  * 
//  * */


//   // pCONT_iLight->CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(10);

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
  
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "sun_elevation=%d"), (int)sun_elevation);

//   // struct COLOUR_REGIONS{
//   //   uint8_t rgb_
//   // }

//   // uint8_t

//   /**
//    * Generate the rgb channels
//    * */
//   // Add yellow to red hue, with max to lowering rgbbrightness
//   if(sun_elevation>10)
//   {
//     pCONT_iLight->rgbcct_controller.setRGB(0,255,0);    

//   }else
//   if(
//     (sun_elevation<10)&&
//     (sun_elevation>-5)
//     )
//   {
//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H = pCONT_iLight->HueN2F(mapvalue(sun_elevation,10,-5,35,0)); // yellow to red
//     hsb.S = 1;

//     /**
//      * Calculate brightness gradient
//      * */
//     hsb.B = mapvalue(sun_elevation,10,-5,0.0f,1.0f);

//     uint8_t brightness_255 = map(sun_elevation,10,-5,0,255);
//     pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


//     pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

//     // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

//     // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   }else
//   // Just below hoeizon, sky blue to night blue
//   // "blue" remains bright
//  if(
//     (sun_elevation<=-5)&&
//     (sun_elevation>=-10)
//     )
//   {
//     // HsbColor hsb = HsbColor(RgbColor(0));
//     // hsb.H = map(sun_elevation,-50,10,255,0)

//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H =  pCONT_iLight->HueN2F(mapvalue(sun_elevation,-5,-10,180,240)); // yellow to red
//     hsb.S = 1;

//     /**
//      * Calculate brightness gradient
//      * */
//     hsb.B = mapvalue(sun_elevation,-5,-10,1.0f,1.0f);

//     uint8_t brightness_255 = map(sun_elevation,-5,-10,255,200);
//     pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


//     pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

//     // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

//     // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   }else
//   // "night" blue, slowing fade to low brightness
//  if(
//     (sun_elevation>=-50)&&
//     (sun_elevation<=-5)
//     )
//   {
//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H = pCONT_iLight->HueN2F(240); // yellow to red
//     hsb.S = 1;

//     /**
//      * Calculate brightness gradient
//      * */
//     hsb.B = mapvalue(sun_elevation,-5,-50,1.0f,0.0f);


//     pCONT_iLight->rgbcct_controller.setHsbColor(hsb);

//     uint8_t brightness_255 = map(sun_elevation,-5,-50,255,1);
//     pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);


//     // uint8_t blue =  map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//     // AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, blue=%d"), (int)sun_elevation, blue);

//     // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

//     // uint8_t brightness_255 = map(sun_elevation,-50,-5,255,0);

//     // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   }
  
//   // else
//   // if((sun_elevation>-50)&&(sun_elevation<10))
//   // {
//   //   // HsbColor hsb = HsbColor(RgbColor(0));
//   //   // hsb.H = map(sun_elevation,-50,10,255,0)

//   //   uint8_t blue =  map(sun_elevation,-50,10,255,0);

//   //   pCONT_iLight->rgbcct_controller.setRGB(0,0,blue);

//   //   // AddLog(LOG_LEVEL_INFO, PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




//   //   // uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

//   //   // pCONT_iLight->rgbcct_controller.setBrightnessRGB255(brightness_255);

//   // }
//   // else
//   // if(
//   //   (sun_elevation>5)
//   //   )
//   // {
//   //   pCONT_iLight->rgbcct_controller.setRGB(1,0,0);    
//   // }
//   else
//   {
//     //unknown, therfore turn off
//     pCONT_iLight->rgbcct_controller.setRGB(0,0,0);
//   }



//   /**
//    * Generate the CCT channels
//    * */
//   if(
//     (sun_elevation<-5)
//     )
//   {

//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMin()); // set as daywhite

//     uint8_t brightness_255 = 0;//map(sun_elevation,-10,25,0,255);
//     pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }else
//   if(
//     (sun_elevation>=10)&&
//     (sun_elevation<=20)
//     )
//   {

//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,10,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(cct_val);

    
//     AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, elev_perc=%d, cct_val=%d, cct=%d"), (int)sun_elevation, elev_perc, cct_val, pCONT_iLight->rgbcct_controller.getCCT());

//     uint8_t brightness_255 = map(sun_elevation,10,20,100,255);
//     pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }else
//   if(
//     (sun_elevation>=-5)&&
//     (sun_elevation<10)
//     )
//   {

//     // Convert elevation into percentage
//     uint8_t elev_perc = 0;//map(sun_elevation,-5,10,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMin(),pCONT_iLight->get_CTRangeMax());
//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(cct_val);

    
//     AddLog(LOG_LEVEL_TEST,PSTR("sun_elevation=%d, elev_perc=%d, cct_val=%d, cct=%d"), (int)sun_elevation, elev_perc, cct_val, pCONT_iLight->rgbcct_controller.getCCT());

//     uint8_t brightness_255 = map(sun_elevation,-5,10,0,100);
//     pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }else
//   if(
//     (sun_elevation>20)
//     )
//   {

//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMax()); // set as daywhite

//     // uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);
//     // pCONT_iLight->rgbcct_controller.setBrightnessCCT255(brightness_255);

//   }
  




// //  pCONT_iLight->rgbcct_controller.setCCT(0); // set as daywhite

  
//   pCONT_iLight->animation.flags.fForceUpdate = true;




//   pCONT_iLight->animation.palette.id = 10;

//  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   animation_colours_rgbcct.StartingColor = GetPixelColor();
//   // GetPixelColourHardwareInterface();

//   // AddLog(LOG_LEVEL_TEST, PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

//   // // Call the animator to blend from previous to new
//   // this->setAnimFunctionCallback(
//   //   [this](const AnimationParam& param){
//   //     this->AnimationProcess_BlendStartingToDesiredColour_SOLID_COLOUR(param);
//   //   }
//   // );


//   //     // UpdateDesiredColourFromPaletteSelected();


//   //     UpdateStartingColourWithGetPixel();



//     //   flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // // NO 
//     // }break;
//     // case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//         this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
      
//   //     flashersettings.region = EFFECTS_REGION_COLOUR_SELECT_ID;
//   //     break;
//   // }

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_03()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//  // AddLog(LOG_LEVEL_TEST, PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;



//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;

//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID);
//   uint8_t pixels_max = mPaletteI->GetPixelsInMap(palette_p);
//   AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   double last_index_difference = 0;
//   double current_index_difference = 0;
//   uint8_t lowest_difference_pixel_index = 0;
//   int16_t indexing = 0;
//   int16_t adjusted_index_back_to_minus90_to_plus90 = 0;

  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;
//   float upper_boundary_value = 90;




//   // Generate all the errors into an array
//   float sun_error[pixels_max];  
//   int16_t palette_indexes[pixels_max];  
//   // for(int i=0;i<pixels_max;i++)
//   // {
//   //   mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//   //   palette_indexes[i] = indexing - 90;
//   //   sun_error[i] = (float)sun_elevation - palette_indexes[i];
//   //   AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d %d \t %d"),i, indexing,  (int)sun_error[i], palette_indexes[i]);
//   //   // Also record zero-crossing point
//   //   if((indexing-90) == 0)
//   //   {
//   //     zero_crossing_index = i;
//   //     AddLog(LOG_LEVEL_INFO, PSTR("zero_crossing_index = %d FOUND"), zero_crossing_index);
//   //   }
//   // }

//   //     AddLog(LOG_LEVEL_INFO, PSTR(DEBUG_INSERT_PAGE_BREAK "zero_crossing_index = %d FOUND"), zero_crossing_index);
//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   float sun_positions_from_palette_index[pixels_max];    
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }





//   uint8_t searching_matched_index = 0;
//   for(int i=0;i<pixels_max;i++)
//   {
//     Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }
//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;
//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */

//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   Serial.printf("numer=\t%f\n\r",numer);
//   Serial.printf("denum=\t%f\n\r",denum);
//   Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);










// //   float progress_between_colours = 0;

// //   /**
// //    * Change to working out the positive or negative side
// //    * */
// //   if(sun_elevation>=0)
// //   {

// //     AddLog(LOG_LEVEL_INFO, PSTR("Checking POSITIVE elevation\n\r"));

// //     uint8_t smallest_error_index = 0;
// //     float smallest_elev_error = fabs(sun_error[zero_crossing_index]);
// //     for(int i=zero_crossing_index+1;i<pixels_max;i++) // skip first
// //     {
// //       if(fabs(sun_error[i])<smallest_elev_error)
// //       {
// //         smallest_error_index = i;
// //         smallest_elev_error = fabs(sun_error[i]);
// //       }
// //       // AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d\t smallest_elev[%d]=%d"),
// //       //     i, (int)sun_error[i], smallest_error_index, (int)smallest_elev_error );
// //     }

// //     /**
// //      * previous colour will always equal the matched index (bring to tersary)
// //      * Work out the transitions
// //      * */
// //     if(sun_error[smallest_error_index] == 0) // EXACT ERROR
// //     {
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error EXACT"));
// //       // next index depends on direction
// //       if (sun_is_ascending){
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index + 1;
// //         progress_between_colours = ((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //       Serial.printf("prev=%d, next=%d, progress=%d\n\r", index.previous, index.next, progress_between_colours);
// //     }else{                                    // CLOSE ERROR
// //       // depends on direction
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error CLOSE"));
// //       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
// //         AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
        
// //         sun_elevation = 50;
// //         // smallest_error_index=11;

// //         float lower_boundary_index = 13;
// //         float lower_boundary_value = 45;
// //         float upper_boundary_index = 14;
// //         float upper_boundary_value = 90;


// //         // index.previous = lower_boundary_index; // equals this exact index
// //         // index.next = upper_boundary_index;



// //         // float numer = smallest_error_index - (float)palette_indexes[index.next];
// //         // float denum = smallest_error_index - (float)palette_indexes[index.previous];


// //         float numer = sun_elevation - lower_boundary_value;
// //         float denum = upper_boundary_value - lower_boundary_value;
// //         float ratio = numer/denum;



// //         Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
// //         Serial.printf("numer=%f\n\r",numer);
// //         Serial.printf("denum=%f\n\r",denum);
// //         Serial.printf("ratio=%f\n\r",ratio);





// //         // Serial.printf("%d %d %f %f\n\r", palette_indexes[index.next],palette_indexes[index.previous],numer,denum);
// //         // Serial.printf("%d %d %f %f\n\r", palette_indexes[index.next],palette_indexes[index.previous],numer,denum);

// //         /**
// //          * 10 ======= 20
// //          *      15
// //          * 
// //          * 20-10 = 10
// //          * 15-10 = 5
// //          * 5/10 = 0.5
// //          * 
// //          * 2-1 = 1
// //          * 1.5-1=0.5      * 
// //          * 0.5/1 => 0.5
// //          *  
// //          * 10*(1+0.5) = 15
// //          * 
// //          * In equation
// //          * 
// //          * full_range = next_max_val - prev_min_val
// //          * current_range = current_inbetween_val - prev_min_val
// //          * 
// //          * ratio_of_range = current_range/full_range
// //          * 
// //          * */

// //         for(int i=0;i<pixels_max;i++)
// //         {
// //           Serial.printf("palette_indexes[%02d] = %d\n\r",i, palette_indexes[i]);
// //         }
 
// //         float next_max_val = palette_indexes[index.next];
// //         float prev_min_val = palette_indexes[index.previous];
// //         float current_inbetween_val = smallest_elev_error;

// //         float full_range = next_max_val - prev_min_val;
// //         float current_range = current_inbetween_val - prev_min_val;
// //         float ratio_of_range = current_range/full_range;

// //         // Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
// //         // Serial.printf("smallest_error_index=%d\n\r",smallest_error_index);
// //         // Serial.printf("index.previous=%d\n\r",index.previous);
// //         // Serial.printf("index.next=%d\n\r",index.next);
// //         // Serial.printf("next_max_val=%f\n\r",next_max_val);
// //         // Serial.printf("prev_min_val=%f\n\r",prev_min_val);
// //         // Serial.printf("current_inbetween_val=%f\n\r",current_inbetween_val);
// //         // Serial.printf("full_range=%f\n\r",full_range);
// //         // Serial.printf("current_range=%f\n\r",current_range);
// //         // Serial.printf("ratio_of_range=%f\n\r",ratio_of_range);

       
// //         // AddLog_f

// //         progress_between_colours = (numer/denum);


// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //       Serial.printf("prev=%d, next=%d, progress=%f\n\r", index.previous, index.next, progress_between_colours);
// //     }


// //   }
// //   else // sun_elevation<0
// //   {

// //     AddLog(LOG_LEVEL_INFO, PSTR("Checking NEGATIVE elevation"));
    
// //     uint8_t smallest_error_index = 0;
// //     float smallest_elev_error = fabs(sun_error[0]);
// //     for(int i=1;i<zero_crossing_index;i++) // skip first
// //     {
// //       if(fabs(sun_error[i])<smallest_elev_error)
// //       {
// //         smallest_error_index = i;
// //         smallest_elev_error = fabs(sun_error[i]);
// //       }
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[%d] = %d smallest_elev_error%d smallest_error_index%d"),i, (int)sun_error[i], (int)smallest_elev_error, smallest_error_index);
// //     }

// //     /**
// //      * previous colour will always equal the matched index (bring to tersary)
// //      * Work out the transitions
// //      * */
// //     if(sun_error[smallest_error_index] == 0)
// //     {
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] == 0"));
// //       // next index depends on direction
// //       if (sun_is_ascending){
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index + 1;
// //         progress_between_colours = 0;//((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = 0;//((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //     }else{
// //       // depends on direction
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0"));
// //       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
// //       AddLog(LOG_LEVEL_INFO, PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index + 1;
// //         progress_between_colours = ((float)palette_indexes[index.next]/(float)palette_indexes[index.previous]);
// //       }else{
// //         index.previous = smallest_error_index; // equals this exact index
// //         index.next = smallest_error_index - 1;
// //         progress_between_colours = ((float)palette_indexes[index.previous]/(float)palette_indexes[index.next]);
// //       }
// //     }


// //   }

// /**
//  * Showing the colours
//  * 1) previous
//  * 2) next
//  * 3) linearblend of the exact new colour
//  * */

//     RgbcctColor c_prev = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//     RgbcctColor c_next = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//     Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//       lower_boundary_value, lower_boundary_index, 
//       upper_boundary_value, upper_boundary_index, progress_between_colours);

//     RgbcctColor c_blended = RgbcctColor::LinearBlend(c_prev,c_next,progress_between_colours);

//     RgbcctColor c = c_prev; 
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//     c = c_blended; 
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//     c = c_next; 
//     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//     /**
//      * Load new colour into animation
//      * */

// //   /**
// //    * Debug, show all colours
// //    * */
// //   int16_t adjusted_index = 0;
// //   for(int desired_index=0;desired_index<pixels_max;desired_index++)
// //   {
// //     RgbcctColor c = mPaletteI->GetColourFromPalette(mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID), desired_index, &indexing);
// //     adjusted_index = indexing - 90;
// //     AddLog(LOG_LEVEL_INFO, PSTR("rgbcct[%d]=\t%d\t%d\t%d,%d,%d,%d,%d"),desired_index,indexing,adjusted_index,c.R,c.G,c.B,c.WW,c.WC);
// //   }

//   // delay(100);

  
//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;//mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }
  
//   animation_colours_rgbcct.StartingColor = GetPixelColor();
  
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_04()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   bool sun_is_ascending = true;//pCONT_solar->solar_position_testing.direction.is_ascending;
//   // Serial.printf("\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette.id);
//   uint8_t pixels_max = mPaletteI->GetPixelsInMap(palette_p);
//   // AddLog(LOG_LEVEL_INFO,PSTR("pixels_max=%d"),pixels_max);

//   // Lets assume we need a zero crossing index, thus, we can use it to identity AS and DE modes
//   uint8_t zero_crossing_index = 0;

//   struct INDEXES_MATCHES{
//     uint8_t previous = 0; //ie colour moving away from
//     uint8_t next = 0; //colour moving towards
//   }index;

//   /**
//    * Steps to finding index
//    * 1) Find the zero-crossing index from the palette (ie the colour where its index is 0)
//    * 2) Decide if elevation is pos or neg, begin searching that part of the array
//    * 3) Find index of closest in array
//    * 4) Next and previous index will depend on direction of sun, and will be equal to current index if error is exactly 0
//    * */

//   /**
//    * Step X: Find zero crossing point
//    * Step X: Find all differences
//    * */
//   int16_t indexing = 0;  
//   uint8_t lower_boundary_index = 13;
//   float lower_boundary_value = 45;
//   uint8_t upper_boundary_index = 14;  
//   float upper_boundary_value = 90;
//   float sun_positions_from_palette_index[pixels_max];  
//   uint8_t searching_matched_index = 0;

//   /**
//    * Ascending method for finding right region between points
//    * Check all, but once sun_elev is greater, then thats the current region
//    * */
//   for(int i=0;i<pixels_max;i++)
//   {
//     mPaletteI->GetColourFromPalette(palette_p, i, &indexing);
//     sun_positions_from_palette_index[i] = indexing - 90;
//     // Serial.printf("sun_pos=[%02d]=\t%f\n\r", i, sun_positions_from_palette_index[i]);
//   }


//   for(int i=0;i<pixels_max;i++)
//   {
//     // Serial.printf("sun=%f > index[%d]=%f\n\r", sun_elevation, i, sun_positions_from_palette_index[i]);
//     if(sun_elevation >= sun_positions_from_palette_index[i])
//     {
      
//       // searching_matched_index = i;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       //Serial.printf("Still less\n\r");

//     }else{
      
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       // Serial.printf("searching_matched_index = %d\n\r", searching_matched_index);
//       break;

//     }

//     // Directly, manually, check the last memory space

//     if(sun_elevation == sun_positions_from_palette_index[pixels_max-1])
//     {
//       searching_matched_index = i-1;
//       // Serial.printf("sun=%f > index[%d]=%f   MATCH=%d\n\r", 
//       //   sun_elevation, i, sun_positions_from_palette_index[i], searching_matched_index
//       // );
//       break;

//     }




//   }

//   lower_boundary_index = searching_matched_index;
//   upper_boundary_index = searching_matched_index+1;

//   /**
//    * Check ranges are valid, if not, reset to 0 and 1
//    * */
//   if(lower_boundary_index>=pixels_max)
//   {
//     lower_boundary_index = 0;
//     Serial.printf("lower_boundary_index>=pixels_max\n\r");
//   }
//   if(upper_boundary_index>=pixels_max)
//   {
//     upper_boundary_index = pixels_max;
//     Serial.printf("upper_boundary_index>=pixels_max\n\r");
//   }

//   lower_boundary_value = sun_positions_from_palette_index[lower_boundary_index];
//   upper_boundary_value = sun_positions_from_palette_index[upper_boundary_index];


//   float numer = sun_elevation        - lower_boundary_value;
//   float denum = upper_boundary_value - lower_boundary_value;
//   float progress_between_colours = numer/denum;

//   Serial.printf("\n\r\n\r\n\rsun_elevation\t => %f\n\r", sun_elevation);
//   Serial.printf("lower_boundary_value[%02d]=%f\n\r", lower_boundary_index, lower_boundary_value);
//   Serial.printf("upper_boundary_value[%02d]=%f\n\r", upper_boundary_index, upper_boundary_value);
//   Serial.printf("numer=\t%f\n\r",numer);
//   Serial.printf("denum=\t%f\n\r",denum);
//   Serial.printf("progress_between_colours=\t%f\n\r",progress_between_colours);

//   /**
//    * Showing the colours
//    * 1) previous
//    * 2) next
//    * 3) linearblend of the exact new colour
//    * */

//   RgbcctColor c_lower = mPaletteI->GetColourFromPalette(palette_p, lower_boundary_index);
//   RgbcctColor c_upper = mPaletteI->GetColourFromPalette(palette_p, upper_boundary_index);

//   // Serial.printf("progress_between_colours\t %f(%d)/%f(%d) => %f\n\r", 
//   //   lower_boundary_value, lower_boundary_index, 
//   //   upper_boundary_value, upper_boundary_index, progress_between_colours);

//   RgbcctColor c_blended = RgbcctColor::LinearBlend(c_lower, c_upper, progress_between_colours);

//   RgbcctColor c = c_lower; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // AddLog(LOG_LEVEL_INFO, PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /****
//  * Changes pixels randomly to new colour, with slow blending
//  * Requires new colour calculation each call
//  */
// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(){

// // Finish this mode later in december
// // Split strip into parts
// // Take every X pixels as the animated pixel, then I only need to remember a count 0:X
// // Move across that each time (or make it random)
// // Use this, to manipulate brightness from low to high... on the next animate_id call, return birghtness back (with require another counter for "progress of animation")
// // Once that group has changed, or, even during it, pick another group to manipulate
// // Effect should be "Group of pixels pulsing on, while the rest remain at a darker brightness"

//   return;

//    switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
//       // }

//       pCONT_iLight->rgbcct_controller.setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along




    
//         // UpdateStartingColourWithGetPixel();

//         // uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//         // HsbColor hsb = GetPixelColor(index_random);

//         // if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
//         //   hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//         //   animation_colours[index_random].DesiredColour = hsb;
//         // }

//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(param); });
//       break;

      
//       // Stay on this until restarted
//   }

// }


// void mAnimatorLight::AnimationProcess_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(const AnimationParam& param)
// {   

//   // As integer so the if statement checks will not fail due to rounding errors
//   // uint8_t progress_percentage = param.progress*100; 
//   // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
//   // uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

//   //       uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//   // /*
//   // 0-10    20-30   40-50   60-70    80-90      //coloured
//   //     10-20   30-40   50-60   70-80   90-0    //white
//   // */
//   // if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
//   //   shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
//   //   switch(progress_percentage){
//   //     case 50:


//   //       HsbColor hsb = GetPixelColor(index_random);

//   //       if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
//   //         hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//   //         // animation_colours[index_random].DesiredColour = hsb;
//   //       }

//   //       SetPixelColor(index_random, hsb);



//   //       // for (uint16_t ii = 0; ii < random_amount; ii++){
//   //       //   SetPixelColor(
//   //       //     random(0,pCONT_iLight->settings.light_size_count), 
//   //       //     HsbColor(pCONT_iLight->HueN2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
//   //       //   );
//   //       // }
//   //     break;
//   //     case 0:
//   //     case 100:
//   //       for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //         SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
//   //       }
//   //     break;
//   //   }
//   // }



//    RgbTypeColor updatedColor;
//   for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//     updatedColor = RgbTypeColor::LinearBlend(
//         animation_colours[pixel].StartingColor,
//         animation_colours[pixel].DesiredColour,
//         param.progress);    
//     SetPixelColor(pixel, updatedColor);
//   } // END for
  

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_05
//  * 
//  * CCT_Mapped, day white to warm white around +-20, then >20 is max cct
//  * 
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

//   pCONT_iLight->animation.palette.id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values


// float sun_elevation = 0;
// #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif
//   if(sun_elevation < -20)
//   {
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMax());      
//   }else
//   if(sun_elevation > 20)
//   {
//     pCONT_iLight->rgbcct_controller.setCCT(pCONT_iLight->get_CTRangeMin());      
//   }else{
//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-20,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMax(),pCONT_iLight->get_CTRangeMin());
 
//     // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "cct_val=%d"),cct_val);
//     // Set the colour temp
//     pCONT_iLight->rgbcct_controller.setCCT(cct_val);    
//   }

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr);
//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   // AddLog(LOG_LEVEL_TEST, PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB255(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random(){

//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       UpdateDesiredColourFromPaletteSelected();
//       UpdateStartingColourWithGetPixel();
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     // NO 
//     }break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//       this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_TwinkleSingleColourRandom(param); });
//       break;
//   }

// }

// void mAnimatorLight::AnimationProcess_TwinkleSingleColourRandom(const AnimationParam& param)
// {   

//   // As integer so the if statement checks will not fail due to rounding errors
//   uint8_t progress_percentage = param.progress*100; 
//   uint8_t brightness_as_percentage = map(pCONT_iLight->rgbcct_controller.getBrightnessRGB255(), 0,255, 0,100);
//   uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);

//   /*
//   0-10    20-30   40-50   60-70    80-90      //coloured
//       10-20   30-40   50-60   70-80   90-0    //white
//   */
//   if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
//     shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
//     switch(progress_percentage){
//       case 10:
//       case 30:
//       case 50:
//       case 70:
//       case 90:
//         for (uint16_t ii = 0; ii < random_amount; ii++){
//           SetPixelColor(
//             random(0,pCONT_iLight->settings.light_size_count), 
//             HsbColor(pCONT_iLight->HueN2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
//           );
//         }
//       break;
//       case 0:
//       case 20:
//       case 40:
//       case 60:
//       case 80: //go back to coloured
//         for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//           SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
//         }
//       break;
//     }
//   }

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random(){
  
//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor(0));
//       // }
//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor(0));
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//   //   case EFFECTS_REGION_ANIMATE_ID: //shift along
//   //     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   //     this->setAnimFunctionCallback([this](const AnimationParam& param){ this->AnimationProcess_TwinkleUsingPaletteColourRandom(param); });
//   //     break;
//   }

// }



// void mAnimatorLight::AnimationProcess_TwinkleUsingPaletteColourRandom(const AnimationParam& param)
// {   

//   // As integer so the if statement checks will not fail due to rounding errors
//   uint8_t progress_percentage = param.progress*100; 
//   // uint8_t brightness_as_percentage = map(pCONT_iLight->getBriRGB(), 0,255, 0,100);
//   uint8_t random_amount = map(shared_flasher_parameters.alternate_random_amount_as_percentage, 0,100, 0,pCONT_iLight->settings.light_size_count);
//   RgbTypeColor flash_colour = RgbTypeColor(0);
//   uint8_t desired_pixel = 0;
//   int16_t pixel_position = -1;
//   uint8_t flashed_brightness = 0; // use set brightness or flash brighter as option
  
//   // Update pointer of struct
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette.id);

  
//   // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//   //       animation_colours[pixel].StartingColor,
//   //       animation_colours[pixel].DesiredColour,
//   //       param.progress);    
//   //   SetPixelColor(pixel, updatedColor);
//   // } // END for

//   /*
//   0-10    20-30   40-50   60-70    80-90      //coloured
//       10-20   30-40   50-60   70-80   90-0    //white
//   */
//   if(progress_percentage != shared_flasher_parameters.progress_percentage_last_animated_on){
//     shared_flasher_parameters.progress_percentage_last_animated_on = progress_percentage; //update stored value
//       switch(progress_percentage){
//       case 10:
//       case 30:
//       case 50:
//       case 70:
//       case 90:
//         for (uint16_t ii = 0; ii < random_amount; ii++){
//           flashed_brightness = random(0,shared_flasher_parameters.alternate_brightness_max);        
//           // For random, desired pixel from map will also be random
//           desired_pixel = random(0,mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr));
//           // get colour from palette
//           flash_colour = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr,desired_pixel,&pixel_position);
//           flash_colour = ApplyBrightnesstoRgbcctColour(flash_colour,flashed_brightness);
//           SetPixelColor(
//             random(0,pCONT_iLight->settings.light_size_count), 
//             flash_colour
//           );
//         }
//       break;
//       case 0:
//       case 20:
//       case 40:
//       case 60:
//       case 80: //go back to coloured
//         for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//           SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
//         }
//       break;
//     }
//   }

// }




/***
 * 
 * Create an animator for each segment, or do one by hand?
 * 
 * */



// #define IBN 5100
// #define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)



//each needs 12 bytes
//Spark type is used for popcorn, 1D fireworks, and drip
typedef struct Spark {
  float pos;
  float vel;
  uint16_t col;
  uint8_t colIndex;
} spark;


    // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Static
 ****************************************************************************************************************************
 ****************************************************************************************************************************/

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Segment_Animation__Static()
// {
//   uint8_t segment_index  = segment_active_index;
//   uint16_t segment_length = _segments[segment_active_index].length();
//   uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
//   uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
//   _virtualSegmentLength = segment_length;

// /*
//  * No blinking. Just plain old static light.
//  */
// // void mAnimatorLight::mode_static(void) {
//   /**
//    * Generate new colours
//    * */
//   UpdatePalette_FastLED_TargetPalette();

//   fill_ranged(SEGCOLOR(0));
//   // return 
//   (_segments[segment_active_index].getOption(SEG_OPTION_TRANSITIONAL)) ? FRAMETIME : 500; //update faster if in transition

  
//   _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
//   StripUpdate();

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// void mAnimatorLight::mode_tri_static_pattern()
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Tri_Static_Pattern()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint8_t segSize = (_segments[segment_active_index].intensity() >> 5) +1;
  uint8_t currSeg = 0;
  uint16_t currSegCount = 0;

  for (uint16_t i = 0; i < segment_length; i++) 
  {

    if ( currSeg % 3 == 0 ) 
    {
      SetPixelColor(i+start_pixel, GetSegmentColour(_segments[segment_active_index].palette.id, 0));
    } else if( currSeg % 3 == 1) {
      SetPixelColor(i+start_pixel, GetSegmentColour(_segments[segment_active_index].palette.id, 1));
    } else {
      SetPixelColor(i+start_pixel, (GetSegmentColour(_segments[segment_active_index].palette.id, 2).CalculateBrightness() > 0 ? GetSegmentColour(_segments[segment_active_index].palette.id, 2) : WHITE));
    }   

    currSegCount += 1;
    if (currSegCount >= segSize) {
      currSeg +=1;
      currSegCount = 0;
    }
  }

  // return FRAMETIME;
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************
   @brief: Colour_Wipe_Base
   @note : Converted from WLED Effects
           LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
           if (bool rev == true) then LEDs are turned off in reverse order
   @note   Rate: FRAME_RATE

   @param rev             Reversed
   @param useRandomColors 
*******************************************************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::BaseSubTask_Segment_Animation__Base_Colour_Wipe(bool rev, bool useRandomColors)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  uint32_t cycleTime = 750 + (255 - _segments[segment_active_index].speed())*150;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  bool back = (prog > 32767);
  if (back) {
    prog -= 32767;
    if (_segment_runtimes[segment_active_index].step == 0) _segment_runtimes[segment_active_index].step = 1;
  } else {
    if (_segment_runtimes[segment_active_index].step == 2) _segment_runtimes[segment_active_index].step = 3; //trigger color change
  }
  
  if (useRandomColors) {
    if (_segment_runtimes[segment_active_index].call == 0) {
      _segment_runtimes[segment_active_index].aux0 = random8();
      _segment_runtimes[segment_active_index].step = 3;
    }
    if (_segment_runtimes[segment_active_index].step == 1) { //if flag set, change to new random color
      _segment_runtimes[segment_active_index].aux1 = get_random_wheel_index(_segment_runtimes[segment_active_index].aux0);
      _segment_runtimes[segment_active_index].step = 2;
    }
    if (_segment_runtimes[segment_active_index].step == 3) {
      _segment_runtimes[segment_active_index].aux0 = get_random_wheel_index(_segment_runtimes[segment_active_index].aux1);
      _segment_runtimes[segment_active_index].step = 0;
    }
  }

  uint16_t ledIndex = (prog * segment_length) >> 15;
  uint16_t rem = 0;
  rem = (prog * segment_length) * 2; //mod 0xFFFF
  rem /= (_segments[segment_active_index].intensity() +1);
  if (rem > 255) rem = 255;

  // uint32_t col1 = useRandomColors ? color_wheel(_segment_runtimes[segment_active_index].aux1) : SEGCOLOR(1);
  uint32_t col1 = color_wheel(_segment_runtimes[segment_active_index].aux1);// : SEGCOLOR(1);

  for (uint16_t i = 0; i < segment_length; i++)
  {
    uint16_t index = (rev && back)? segment_length -1 -i : i;
    uint32_t col0 = useRandomColors? color_wheel(_segment_runtimes[segment_active_index].aux0) : color_from_palette(index, true, PALETTE_SOLID_WRAP, 0);
    
    if (i < ledIndex) 
    {
      SetPixelColor(index, back? col1 : col0);
    } else
    {
      SetPixelColor(index, back? col0 : col1);
      if (i == ledIndex) SetPixelColor(index, color_blend(back? col0 : col1, back? col1 : col0, rem));
    }
  } 

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}






// /*
//  * Lights all LEDs one after another.
//  */
// void mAnimatorLight::mode_color_wipe(void) { // the default calls directly
//   return color_wipe(false, false);
// }
void mAnimatorLight::SubTask_Segment_Animation__Colour_Wipe()
{
  BaseSubTask_Segment_Animation__Base_Colour_Wipe(false, false);
}

// /*
//  * Turns all LEDs after each other to a random color.
//  * Then starts over with another color.
//  */
// void mAnimatorLight::mode_color_wipe_random(void) {
//   return color_wipe(false, true);
// }

void mAnimatorLight::SubTask_Segment_Animation__Colour_Wipe_Random()
{
  BaseSubTask_Segment_Animation__Base_Colour_Wipe(false, true);
}


// /*
//  * Lights all LEDs one after another. Turns off opposite
//  */
// void mAnimatorLight::mode_color_sweep(void) {
//   return color_wipe(true, false);
// }
void mAnimatorLight::SubTask_Segment_Animation__Colour_Sweep()
{
  BaseSubTask_Segment_Animation__Base_Colour_Wipe(true, false);
}


// /*
//  * Random color introduced alternating from start and end of strip->
//  */
// void mAnimatorLight::mode_color_sweep_random(void) {
//   return color_wipe(true, true);
// }
void mAnimatorLight::SubTask_Segment_Animation__Colour_Sweep_Random()
{
  BaseSubTask_Segment_Animation__Base_Colour_Wipe(true, true);
}


/*
 * Fills segment with color
 */


/*
 * Gets a single color from the currently selected palette.
 * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
 * @param mapping if true, LED position in segment is considered for color
 * @param wrap FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
 * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
 * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
 * @returns Single color from palette
 */
uint32_t mAnimatorLight::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
{

  if (_segments[segment_active_index].palette.id == 0 && mcol < 3) return SEGCOLOR(mcol); //WS2812FX default
  uint8_t paletteIndex = i;
  if (mapping) paletteIndex = (i*255)/(_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
  // AddLog(LOG_LEVEL_TEST, PSTR("paletteIndex=%d"),paletteIndex);
  if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
  CRGB fastled_col;
  fastled_col = ColorFromPalette( mPaletteI->currentPalette, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);
  return  fastled_col.r*65536 +  fastled_col.g*256 +  fastled_col.b;

}




uint32_t mAnimatorLight::color_blend(uint32_t color1, uint32_t color2, uint8_t blend) {
  if(blend == 0)   return color1;
  if(blend == 255) return color2;

  uint32_t w1 = (color1 >> 24) & 0xff;
  uint32_t r1 = (color1 >> 16) & 0xff;
  uint32_t g1 = (color1 >>  8) & 0xff;
  uint32_t b1 =  color1        & 0xff;

  uint32_t w2 = (color2 >> 24) & 0xff;
  uint32_t r2 = (color2 >> 16) & 0xff;
  uint32_t g2 = (color2 >>  8) & 0xff;
  uint32_t b2 =  color2        & 0xff;

  uint32_t w3 = ((w2 * blend) + (w1 * (255 - blend))) >> 8;
  uint32_t r3 = ((r2 * blend) + (r1 * (255 - blend))) >> 8;
  uint32_t g3 = ((g2 * blend) + (g1 * (255 - blend))) >> 8;
  uint32_t b3 = ((b2 * blend) + (b1 * (255 - blend))) >> 8;

  return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));
}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t mAnimatorLight::color_wheel(uint8_t pos) {

  uint8_t segment_index  = segment_active_index;

  float hue_f = mSupport::mapfloat(pos, 0, 255, 0.0f, 1.0f);

  // Serial.println(hue_f);

  HsbColor colh = HsbColor(hue_f,1,1);


  return RgbcctColor::GetU32Colour(colh);
  // if (_segments[segment_active_index].palette) return color_from_palette(pos, false, true, 0);
  // pos = 255 - pos;
  // if(pos < 85) {
  //   return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  // } else if(pos < 170) {
  //   pos -= 85;
  //   return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  // } else {
  //   pos -= 170;
  //   return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  // }
}



/*
 * Fills segment with color
 */
void mAnimatorLight::fill(uint32_t c) {

  // for(uint16_t i = 0; i < _virtualSegmentLength; i++) {
  //   SetPixelColor(i, c);
  // }
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;

  for(uint16_t i = start_pixel; i <= stop_pixel; i++) 
  {
    SetPixelColor(i, c);
  }

}



void mAnimatorLight::fill_ranged(uint32_t c) 
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;

  for(uint16_t i = start_pixel; i <= stop_pixel; i++) {
    SetPixelColor(i, c);
  }

}

void mAnimatorLight::seg_fill_ranged(uint32_t c) 
{

  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  

  for(uint16_t i = start_pixel; i <= stop_pixel; i++) {
    SetPixelColor(i, c);
    // pCONT_lAni->animation_colours[i].DesiredColour = RgbColor(c);
  }

}




  #ifdef ENABLE_EXTRA_WLED_EFFECTS



/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Chase
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// /*
//  * color chase function.
//  * color1 = background color
//  * color2 and color3 = colors of two adjacent leds
//  *
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  
// void mAnimatorLight::chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette) {
  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 2) + 1);
  uint16_t a = counter * segment_length  >> 16;

  bool chase_random = (_segments[segment_active_index].mode_id == EFFECTS_FUNCTION_WLED_CHASE_RANDOM_ID);
  if (chase_random) {
    if (a < _segment_runtimes[segment_active_index].step) //we hit the start again, choose new color for Chase random
    {
      _segment_runtimes[segment_active_index].aux1 = _segment_runtimes[segment_active_index].aux0; //store previous random color
      _segment_runtimes[segment_active_index].aux0 = get_random_wheel_index(_segment_runtimes[segment_active_index].aux0);
    }
    color1 = color_wheel(_segment_runtimes[segment_active_index].aux0);
  }
  _segment_runtimes[segment_active_index].step = a;

  // Use intensity() setting to vary chase up to 1/2 string length
  uint8_t size = 1 + (_segments[segment_active_index].intensity() * segment_length >> 10);

  uint16_t b = a + size; //"trail" of chase, filled with color1 
  if (b > segment_length) b -= segment_length;
  uint16_t c = b + size;
  if (c > segment_length) c -= segment_length;

  //background
  if (do_palette)
  {
    for(uint16_t i = 0; i < segment_length; i++) {
      SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
    }
  } else fill(color1);

  //if random, fill old background between a and end
  if (chase_random)
  {
    color1 = color_wheel(_segment_runtimes[segment_active_index].aux1);
    for (uint16_t i = a; i < segment_length; i++)
      SetPixelColor(i, color1);
  }

  //fill between points a and b with color2
  if (a < b)
  {
    for (uint16_t i = a; i < b; i++)
      SetPixelColor(i, color2);
  } else {
    for (uint16_t i = a; i < segment_length; i++) //fill until end
      SetPixelColor(i, color2);
    for (uint16_t i = 0; i < b; i++) //fill from start until b
      SetPixelColor(i, color2);
  }

  //fill between points b and c with color2
  if (b < c)
  {
    for (uint16_t i = b; i < c; i++)
      SetPixelColor(i, color3);
  } else {
    for (uint16_t i = b; i < segment_length; i++) //fill until end
      SetPixelColor(i, color3);
    for (uint16_t i = 0; i < c; i++) //fill from start until c
      SetPixelColor(i, color3);
  }

  // return FRAMETIME;
  // return FRAMETIME;
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/*
 * Bicolor chase, more primary color.
 */
// void mAnimatorLight::mode_chase_color(void) {
//   return chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), true);
// }

void mAnimatorLight::SubTask_Segment_Animation__Chase_Colour()
{
  SubTask_Segment_Animation__Base_Chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), true);
}



/*
 * Primary running followed by random color.
 */
// void mAnimatorLight::mode_chase_random(void) {
//   return chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), false);
// }

// void mAnimatorLight::SubTask_Segment_Animation__Chase_Random()
// {
//   SubTask_Segment_Animation__Base_Chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), false);
// }

/*
 * Primary, secondary running on rainbow.
 */
// void mAnimatorLight::mode_chase_rainbow(void) {
//   return chase();
// }
void mAnimatorLight::SubTask_Segment_Animation__Chase_Rainbow()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  uint8_t color_sep = 256 / segment_length;
  uint8_t color_index = _segment_runtimes[segment_active_index].call & 0xFF;
  uint32_t color = color_wheel(((_segment_runtimes[segment_active_index].step * color_sep) + color_index) & 0xFF);

  SubTask_Segment_Animation__Base_Chase(color, SEGCOLOR(0), SEGCOLOR(1), false);
}


/*
 * Sec flashes running on prim.
 */
// #define FLASH_COUNT 4

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// /*
//  * color chase function.
//  * color1 = background color
//  * color2 and color3 = colors of two adjacent leds
//  *

// void mAnimatorLight::mode_chase_flash(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Chase_Flash()
// Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint8_t flash_step = _segment_runtimes[segment_active_index].call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  uint16_t delay = 10 + ((30 * (uint16_t)(255 - _segments[segment_active_index].speed())) / segment_length);
  if(flash_step < (FLASH_COUNT * 2)) {
    if(flash_step % 2 == 0) {
      uint16_t n = _segment_runtimes[segment_active_index].step;
      uint16_t m = (_segment_runtimes[segment_active_index].step + 1) % segment_length;
      SetPixelColor( n, SEGCOLOR(1));
      SetPixelColor( m, SEGCOLOR(1));
      delay = 20;
    } else {
      delay = 30;
    }
  } else {
    _segment_runtimes[segment_active_index].step = (_segment_runtimes[segment_active_index].step + 1) % segment_length;
  }
  // return delay;
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// /*
//  * Prim flashes running, followed by random color.
//  *
// void mAnimatorLight::mode_chase_flash_random(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Chase_Flash_Random()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint8_t flash_step = _segment_runtimes[segment_active_index].call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < _segment_runtimes[segment_active_index].step; i++) {
    SetPixelColor(i, color_wheel(_segment_runtimes[segment_active_index].aux0));
  }

  uint16_t delay = 1 + ((10 * (uint16_t)(255 - _segments[segment_active_index].speed())) / segment_length);
  if(flash_step < (FLASH_COUNT * 2)) {
    uint16_t n = _segment_runtimes[segment_active_index].step;
    uint16_t m = (_segment_runtimes[segment_active_index].step + 1) % segment_length;
    if(flash_step % 2 == 0) {
      SetPixelColor( n, SEGCOLOR(0));
      SetPixelColor( m, SEGCOLOR(0));
      delay = 20;
    } else {
      SetPixelColor( n, color_wheel(_segment_runtimes[segment_active_index].aux0));
      SetPixelColor( m, SEGCOLOR(1));
      delay = 30;
    }
  } else {
    _segment_runtimes[segment_active_index].step = (_segment_runtimes[segment_active_index].step + 1) % segment_length;

    if(_segment_runtimes[segment_active_index].step == 0) {
      _segment_runtimes[segment_active_index].aux0 = get_random_wheel_index(_segment_runtimes[segment_active_index].aux0);
    }
  }
  // return delay;

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


// /*
//  * Primary running on rainbow.
//  */
// void mAnimatorLight::mode_chase_rainbow_white(void) {
//   uint16_t n = _segment_runtimes[segment_active_index].step;
//   uint16_t m = (_segment_runtimes[segment_active_index].step + 1) % segment_length;
//   uint32_t color2 = color_wheel(((n * 256 / segment_length) + (_segment_runtimes[segment_active_index].call & 0xFF)) & 0xFF);
//   uint32_t color3 = color_wheel(((m * 256 / segment_length) + (_segment_runtimes[segment_active_index].call & 0xFF)) & 0xFF);

//   return chase(SEGCOLOR(0), color2, color3, false);
// }
void mAnimatorLight::SubTask_Segment_Animation__Chase_Rainbow_White()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t n = _segment_runtimes[segment_active_index].step;
  uint16_t m = (_segment_runtimes[segment_active_index].step + 1) % segment_length;
  uint32_t color2 = color_wheel(((n * 256 / segment_length) + (_segment_runtimes[segment_active_index].call & 0xFF)) & 0xFF);
  uint32_t color3 = color_wheel(((m * 256 / segment_length) + (_segment_runtimes[segment_active_index].call & 0xFF)) & 0xFF);

  SubTask_Segment_Animation__Base_Chase(SEGCOLOR(0), color2, color3, false);
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * theater chase function
 *
void mAnimatorLight::theater_chase(uint32_t color1, uint32_t color2, bool do_palette) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette)
{

  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  byte gap = 2 + ((255 - _segments[segment_active_index].intensity()) >> 5);
  uint32_t cycleTime = 50 + (255 - _segments[segment_active_index].speed())*2;
  uint32_t it = millis() / cycleTime;
  if (it != _segment_runtimes[segment_active_index].step) //new color
  {
    _segment_runtimes[segment_active_index].aux0 = (_segment_runtimes[segment_active_index].aux0 +1) % gap;
    _segment_runtimes[segment_active_index].step = it;
  }
  
  for(uint16_t i = 0; i < segment_length; i++) {
    if((i % gap) == _segment_runtimes[segment_active_index].aux0) {
      if (do_palette)
      {
        SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
      } else {
        SetPixelColor(i, color1);
      }
    } else {
      SetPixelColor(i, color2);
    }
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/*
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 */
void mAnimatorLight::SubTask_Segment_Animation__Chase_Theater(void) {
  return SubTask_Segment_Animation__Base_Chase_Theater(SEGCOLOR(0), SEGCOLOR(1), true);
}


/*
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 */
void mAnimatorLight::SubTask_Segment_Animation__Chase_Theatre_Rainbow(void) {
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  return SubTask_Segment_Animation__Base_Chase_Theater(color_wheel(_segment_runtimes[segment_active_index].step), SEGCOLOR(1), false);
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Tricolor chase function
 *
void mAnimatorLight::tricolor_chase(uint32_t color1, uint32_t color2) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Chase_TriColour(uint32_t color1, uint32_t color2)
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  uint32_t cycleTime = 50 + (255 - _segments[segment_active_index].speed())*2;
  uint32_t it = millis() / cycleTime;
  uint8_t width = (1 + _segments[segment_active_index].intensity()/32) * 3; //value of 1-8 for each colour
  uint8_t index = it % width;
  
  for(uint16_t i = 0; i < segment_length; i++, index++) {
    if(index > width-1) index = 0;

    uint32_t color = color1;
    if(index > width*2/3-1) color = color_from_palette(i, true, PALETTE_SOLID_WRAP, 1);
    else if(index > width/3-1) color = color2;

    SetPixelColor(segment_length - i -1, color);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/*
 * Tricolor chase mode
 */
void mAnimatorLight::SubTask_Segment_Animation__Chase_TriColour(void) {

  return SubTask_Segment_Animation__Base_Chase_TriColour(SEGCOLOR(2), SEGCOLOR(0));
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Running random pixels
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/RandomChase.h
 *
void mAnimatorLight::mode_random_chase(void)
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Chase_Random()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - _segments[segment_active_index].speed()));
  uint32_t it = millis() / cycleTime;
  if (_segment_runtimes[segment_active_index].step == it){return;}// return FRAMETIME;

  for(uint16_t i = segment_length -1; i > 0; i--) {
    SetPixelColor(i, GetPixelColor(i-1));
  }
  uint32_t color = RgbcctColor::GetU32Colour(GetPixelColor(0));
  if (segment_length > 1) color = RgbcctColor::GetU32Colour(GetPixelColor( 1));
  uint8_t r = random8(6) != 0 ? (color >> 16 & 0xFF) : random8();
  uint8_t g = random8(6) != 0 ? (color >> 8  & 0xFF) : random8();
  uint8_t b = random8(6) != 0 ? (color       & 0xFF) : random8();
  SetPixelColor(0, r, g, b);

  _segment_runtimes[segment_active_index].step = it;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/*
 * Alternating white/red/black pixels running. PLACEHOLDER
 */
void mAnimatorLight::SubTask_Segment_Animation__Circus_Combustus(void) {
  return SubTask_Segment_Animation__Base_Chase_TriColour(RED, WHITE);
}





/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Breathe/Fade/Pulse
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effect
/*
 * Does the "standby-breathing" of well known i-Devices.
 *
void mAnimatorLight::mode_breath(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


void mAnimatorLight::SubTask_Segment_Animation__Breath()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t var = 0;
  uint16_t counter = (millis() * ((_segments[segment_active_index].speed() >> 3) +10));
  counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
  if (counter < 16384) {
    if (counter > 8192) counter = 8192 - (counter - 8192);
    var = sin16(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
  }
  
  uint8_t lum = 30 + var;
  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), lum));
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effects
 * 
/*
 * Fades the LEDs between two colors
 *
void mAnimatorLight::mode_fade(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


void mAnimatorLight::SubTask_Segment_Animation__Fade()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = (millis() * ((_segments[segment_active_index].speed() >> 3) +10));
  uint8_t lum = triwave16(counter) >> 8;

  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), lum));
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


  #endif // ENABLE_EXTRA_WLED_EFFECTS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks
 * @note : Converted from WLED Effects
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


uint32_t mAnimatorLight::crgb_to_col(CRGB fastled)
{
  return (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
}


CRGB mAnimatorLight::col_to_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void mAnimatorLight::blur(uint8_t blur_amount)
{
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = HTMLColorCode::Black;

  for(uint16_t i = 0; i < _virtualSegmentLength; i++)
  {

    CRGB cur = col_to_crgb( RgbcctColor::GetU32Colour(GetPixelColor(i)));
    CRGB part = cur;
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if(i > 0) {
      uint32_t c = RgbcctColor::GetU32Colour(GetPixelColor(i-1));
      uint8_t r = (c >> 16 & 0xFF);
      uint8_t g = (c >> 8  & 0xFF);
      uint8_t b = (c       & 0xFF);
      SetPixelColor(i-1, RgbColor(qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue)));
    }
    SetPixelColor(i, RgbColor(cur.red, cur.green, cur.blue));
    carryover = part;
  }

  
}


// void mAnimatorLight::blur(uint8_t blur_amount)
// {
//   uint8_t keep = 255 - blur_amount;
//   uint8_t seep = blur_amount >> 1;
  
//   // CRGB carryover = HTMLColorCode::Black;
//   RgbcctColor carryover = RgbcctColor(0);

//   for(uint16_t i = 0; i < _virtualSegmentLength; i++)
//   {

//     // CRGB cur = col_to_crgb( RgbcctColor::GetU32Colour(test));//GetPixelColor(i)));
//     RgbcctColor cur = GetPixelColor(i);
//     RgbcctColor part = cur;
//     part.nscale8(seep);
//     cur.nscale8(keep);
//     cur += carryover;

//     if(i > 0) {
//       RgbcctColor c = GetPixelColor(i-1);
//       SetPixelColor(i-1, RgbColor(qadd8(c.R, part.red), qadd8(c.G, part.green), qadd8(c.B, part.blue)));
//     }
//     SetPixelColor(i, RgbColor(cur.red, cur.green, cur.blue));
//     carryover = part;
//   }
// }

/*
 * fade out function, higher rate = quicker fade
 Adding this helper function temporarily until I convert it to linearblend methods
 */
void mAnimatorLight::fade_out(uint8_t rate) 
{
  rate = (255-rate) >> 1;
  float mappedRate = float(rate) +1.1;

  uint32_t color = RgbcctColor::GetU32Colour(RgbColor(0));// SEGCOLOR(1); // target color
  int w2 = (color >> 24) & 0xff;
  int r2 = (color >> 16) & 0xff;
  int g2 = (color >>  8) & 0xff;
  int b2 =  color        & 0xff;

  // r2 = 255;

  for(uint16_t i = 0; i < _virtualSegmentLength; i++) {
    color = RgbcctColor::GetU32Colour(GetPixelColor(i));
    int w1 = (color >> 24) & 0xff;
    int r1 = (color >> 16) & 0xff;
    int g1 = (color >>  8) & 0xff;
    int b1 =  color        & 0xff;

    int wdelta = (w2 - w1) / mappedRate;  //linearblend
    int rdelta = (r2 - r1) / mappedRate;
    int gdelta = (g2 - g1) / mappedRate;
    int bdelta = (b2 - b1) / mappedRate;

    // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
    wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
    rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
    gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
    bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

    SetPixelColor(i, RgbwColor(r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta));
  }

}

/*
 * Fireworks function.

 @param aux0 Current center index of firework (which blurs outwards)
 @param aux1 Previous location of center index of firework
 @param aux2 Iter to wrap palette
 */
void mAnimatorLight::SubTask_Segment_Animation__Fireworks()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  // AddLog(LOG_LEVEL_TEST, PSTR("segment_length=%d"), segment_length);
  // AddLog(LOG_LEVEL_TEST, PSTR("_segment_runtimes[segment_active_index].call=%d"), _segment_runtimes[segment_active_index].call);
  
  fade_out(0);

  if (_segment_runtimes[segment_active_index].call == 0) {
    _segment_runtimes[segment_active_index].aux0 = UINT16_MAX;
    _segment_runtimes[segment_active_index].aux1 = UINT16_MAX;
  }
  bool valid1 = (_segment_runtimes[segment_active_index].aux0 < segment_length);
  bool valid2 = (_segment_runtimes[segment_active_index].aux1 < segment_length);
  RgbcctColor sv1 = 0, sv2 = 0;
  if (valid1) sv1 = GetPixelColor(_segment_runtimes[segment_active_index].aux0);
  if (valid2) sv2 = GetPixelColor(_segment_runtimes[segment_active_index].aux1);
  blur(255-_segments[segment_active_index].speed());
  if (valid1)       SetPixelColor(_segment_runtimes[segment_active_index].aux0, sv1);
  if (valid2)       SetPixelColor(_segment_runtimes[segment_active_index].aux1, sv2);


  // This actually only runs when a new colour is made, and its index is stored so the next animation call will propogate it out

  for(uint16_t i=0; i<MAX(1, segment_length/20); i++) { 
    if(random8(129 - (_segments[segment_active_index].intensity() >> 1)) == 0) {
      uint16_t index = random(segment_length);
      // AddLog(LOG_LEVEL_TEST, "index=%d\t%d",i,index);

      
      // Set palette to be sure
      mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);
      uint8_t pixels_in_palette = mPaletteI->GetPixelsInMap();
      _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;


      SetPixelColor(index, mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, random(0,pixels_in_palette-1)), 0);
      // SetPixelColor(index, mPaletteI->GetColourFromPalette(nullptr, 0));//random(0,pixels_in_palette-1)));
      // SetPixelColor(index, color_from_palette(random8(), false, false, 0));
      _segment_runtimes[segment_active_index].aux1 = _segment_runtimes[segment_active_index].aux0;
      _segment_runtimes[segment_active_index].aux0 = index;
    }
  }
   /**
   * Direct update, disable neopixel callback 
   **/
  StripUpdate();
  _segment_runtimes[segment_active_index].anim_function_callback = nullptr; // When no animation callback is needed


  // return FRAMETIME;
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks Starburst
 * @note : Converted from WLED Effects
 * 
// void mAnimatorLight::mode_starburst(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


#define STARBURST_MAX_FRAG 12

//each needs 64 byte
typedef struct particle {
  CRGB     color;
  uint32_t birth  =0;
  uint32_t last   =0;
  float    vel    =0;
  uint16_t pos    =-1;
  float    fragment[STARBURST_MAX_FRAG];
} star;

void mAnimatorLight::SubTask_Segment_Animation__Fireworks_Starburst()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint8_t numStars = 1 + (segment_length >> 3);
  if (numStars > 15) numStars = 15;
  uint16_t dataSize = sizeof(star) * numStars;

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){
    return;
  }
  //return mode_static(); //allocation failed
  
  uint32_t it = millis();
  
  star* stars = reinterpret_cast<star*>(_segment_runtimes[segment_active_index].data);
  
  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time
     
  for (int j = 0; j < numStars; j++)
  {
    // speed() to adjust chance of a burst, max is nearly always.
    if (random8((144-(_segments[segment_active_index].speed() >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.  
      uint16_t startPos = random16(segment_length-1);
      float multiplier = (float)(random8())/255.0 * 1.0;

      stars[j].color = col_to_crgb(color_wheel(random8()));
      stars[j].pos = startPos; 
      stars[j].vel = maxSpeed * (float)(random8())/255.0 * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = random8(3,6 + (_segments[segment_active_index].intensity() >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }
  
  fill(SEGCOLOR(1));
  
  for (int j=0; j<numStars; j++)
  {
    if (stars[j].birth != 0) {
      float dt = (it-stars[j].last)/1000.0;

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        int var = i >> 1;
        
        if (stars[j].fragment[i] > 0) {
          //all fragments travel right, will be mirrored on other side
          stars[j].fragment[i] += stars[j].vel * dt * (float)var/3.0;
        }
      }
      stars[j].last = it;
      stars[j].vel -= 3*stars[j].vel*dt;
    }
  
    CRGB c = stars[j].color;

    // If the star is brand new, it flashes white briefly.  
    // Otherwise it just fades over time.
    float fade = 0.0f;
    float age = it-stars[j].birth;

    if (age < particleIgnition) {
      c = col_to_crgb(color_blend(WHITE, crgb_to_col(c), 254.5f*((age / particleIgnition))));
    } else {
      // Figure out how much to fade and shrink the star based on 
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = col_to_crgb(SEGCOLOR(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        byte f = 254.5f*fade;
        c = col_to_crgb(color_blend(crgb_to_col(c), SEGCOLOR(1), f));
      }
    }
    
    float particleSize = (1.0 - fade) * 2;

    for (uint8_t index=0; index < STARBURST_MAX_FRAG*2; index++) {
      bool mirrored = index & 0x1;
      uint8_t i = index >> 1;
      if (stars[j].fragment[i] > 0) {
        float loc = stars[j].fragment[i];
        if (mirrored) loc -= (loc-stars[j].pos)*2;
        int start = loc - particleSize;
        int end = loc + particleSize;
        if (start < 0) start = 0;
        if (start == end) end++;
        if (end > segment_length) end = segment_length;    
        for (int p = start; p < end; p++) {
          SetPixelColor(p, c.r, c.g, c.b);
        }
      }
    }
  }
  // return FRAMETIME;

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed

  StripUpdate();


}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effects
 * 
// /*
//  * Exploding fireworks effect
//  * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
//  *

 @param aux0 Current center index of firework (which blurs outwards)
 @param aux1 Previous location of center index of firework
 @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


void mAnimatorLight::SubTask_Segment_Animation__Exploding_Fireworks()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

// AddLog(LOG_LEVEL_TEST, "segment_index=%d", segment_index);
// AddLog(LOG_LEVEL_TEST, "_segment_runtimes[segment_active_index].anim_function_callback=%d", _segment_runtimes[segment_active_index].anim_function_callback);

  //allocate segment data
  uint16_t numSparks = 2 + (segment_length >> 1); 
  if (numSparks > 80) numSparks = 80;
  uint16_t dataSize = sizeof(spark) * numSparks;
  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){
    return;
  // } return mode_static(); //allocation failed
  }

  fill(BLACK);
  
  bool actuallyReverse = _segments[segment_active_index].getOption(SEG_OPTION_REVERSED);
  //have fireworks start in either direction based on intensity()
  _segments[segment_active_index].setOption(SEG_OPTION_REVERSED, _segment_runtimes[segment_active_index].step);
  
  Spark* sparks = reinterpret_cast<Spark*>(_segment_runtimes[segment_active_index].data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004 - (_segments[segment_active_index].speed()/800000.0); // m/s/s
  gravity *= segment_length;
  
  if (_segment_runtimes[segment_active_index].aux0 < 2) { //FLARE
    if (_segment_runtimes[segment_active_index].aux0 == 0) { //init flare
      flare->pos = 0;
      uint16_t peakHeight = 75 + random8(180); //0-255
      peakHeight = (peakHeight * (segment_length -1)) >> 8;
      flare->vel = sqrt(-2.0 * gravity * peakHeight);
      flare->col = 255; //brightness

      _segment_runtimes[segment_active_index].aux0 = 1; 
    }
    
    // launch 
    if (flare->vel > 12 * gravity) {
      // flare
      SetPixelColor(int(flare->pos),flare->col,flare->col,flare->col);
  
      flare->pos += flare->vel;
      flare->pos = constrain(flare->pos, 0, segment_length-1);
      flare->vel += gravity;
      flare->col -= 2;
    } else {
      _segment_runtimes[segment_active_index].aux0 = 2;  // ready to explode
    }
  } else if (_segment_runtimes[segment_active_index].aux0 < 4) {
    /*
     * Explode!
     * 
     * Explosion happens where the flare ended.
     * Size is proportional to the height.
     */
    int nSparks = flare->pos;
    nSparks = constrain(nSparks, 0, numSparks);
    static float dying_gravity;
  
    // initialize sparks
    if (_segment_runtimes[segment_active_index].aux0 == 2) {
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos = flare->pos; 
        sparks[i].vel = (float(random16(0, 20000)) / 10000.0) - 0.9; // from -0.9 to 1.1
        sparks[i].col = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright 
        //sparks[i].col = constrain(sparks[i].col, 0, 345); 
        sparks[i].colIndex = random8();
        sparks[i].vel *= flare->pos/segment_length; // proportional to height 
        sparks[i].vel *= -gravity *50;
      } 
      //sparks[1].col = 345; // this will be our known spark 
      dying_gravity = gravity/2; 
      _segment_runtimes[segment_active_index].aux0 = 3;
    }
  
    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos += sparks[i].vel; 
        sparks[i].vel += dying_gravity; 
        if (sparks[i].col > 3) sparks[i].col -= 4; 

        if (sparks[i].pos > 0 && sparks[i].pos < segment_length) {
          uint16_t prog = sparks[i].col;
          // uint32_t spColor = (_segments[segment_active_index].palette) ? color_wheel(sparks[i].colIndex) : SEGCOLOR(0);
          uint32_t spColor = RgbcctColor::GetU32Colour(RgbColor(0,255,10));
          CRGB c = HTMLColorCode::Black; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = col_to_crgb(color_blend(spColor, WHITE, (prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = col_to_crgb(color_blend(BLACK, spColor, prog - 45));
            uint8_t cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          SetPixelColor(int(sparks[i].pos), c.red, c.green, c.blue);
        }
      }
      dying_gravity *= .99; // as sparks burn out they fall slower
    } else {
      _segment_runtimes[segment_active_index].aux0 = 6 + random8(10); //wait for this many frames
    }
  } else {
    _segment_runtimes[segment_active_index].aux0--;
    if (_segment_runtimes[segment_active_index].aux0 < 4) {
      _segment_runtimes[segment_active_index].aux0 = 0; //back to flare
      _segment_runtimes[segment_active_index].step = (_segments[segment_active_index].intensity() > random8()); //decide firing side
    }
  }

  _segments[segment_active_index].setOption(SEG_OPTION_REVERSED, actuallyReverse);
  
  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed

  StripUpdate();


//   // return FRAMETIME;  
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
// /*
//  * Exploding fireworks effect
//  * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
//  *

 @param aux0 Current center index of firework (which blurs outwards)
 @param aux1 Previous location of center index of firework
 @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



//Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
void mAnimatorLight::SubTask_Segment_Animation__Rain() // Firework_Rain
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;



  _segment_runtimes[segment_active_index].step += FRAMETIME;
  if (_segment_runtimes[segment_active_index].step > SPEED_FORMULA_L) {
    _segment_runtimes[segment_active_index].step = 0;
    //shift all leds right
    RgbcctColor ctemp = GetPixelColor(segment_length -1);
    for(uint16_t i = segment_length -1; i > 0; i--) {
      SetPixelColor(i, GetPixelColor(i-1));
    }
    SetPixelColor(0, ctemp);
    _segment_runtimes[segment_active_index].aux0++;
    _segment_runtimes[segment_active_index].aux1++;
    if (_segment_runtimes[segment_active_index].aux0 == 0) _segment_runtimes[segment_active_index].aux0 = UINT16_MAX;
    if (_segment_runtimes[segment_active_index].aux1 == 0) _segment_runtimes[segment_active_index].aux0 = UINT16_MAX;
    if (_segment_runtimes[segment_active_index].aux0 == segment_length) _segment_runtimes[segment_active_index].aux0 = 0;
    if (_segment_runtimes[segment_active_index].aux1 == segment_length) _segment_runtimes[segment_active_index].aux1 = 0;
  }
  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed

  SubTask_Segment_Animation__Fireworks();


  // StripUpdate();

  // return mode_fireworks();
}

  #ifdef ENABLE_EXTRA_WLED_EFFECTS

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Sparkle/Twinkle
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
 * 
/*
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *
// void mAnimatorLight::mode_sparkle(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



//Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
void mAnimatorLight::SubTask_Segment_Animation__Sparkle() // Firework_Rain
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }
  uint32_t cycleTime = 10 + (255 - _segments[segment_active_index].speed())*2;
  uint32_t it = millis() / cycleTime;
  if (it != _segment_runtimes[segment_active_index].step)
  {
    _segment_runtimes[segment_active_index].aux0 = random16(segment_length); // aux0 stores the random led index
    _segment_runtimes[segment_active_index].step = it;
  }
  
  SetPixelColor(_segment_runtimes[segment_active_index].aux0, SEGCOLOR(0));
  

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
/*
 * Lights all LEDs in the color. Flashes single white pixels randomly.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *

//Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
void mAnimatorLight::mode_flash_sparkle(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


void mAnimatorLight::SubTask_Segment_Animation__Sparkle_Flash() // Firework_Rain
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  if(random8(5) == 0) {
    _segment_runtimes[segment_active_index].aux0 = random16(segment_length); // aux0 stores the random led index
    SetPixelColor(_segment_runtimes[segment_active_index].aux0, SEGCOLOR(1));
    _segments[segment_active_index].transition.time_ms = 20;//return 20;
  } 
  _segments[segment_active_index].transition.time_ms =  20 + (uint16_t)(255-_segments[segment_active_index].speed());
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
/*
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *
void mAnimatorLight::SubTask_Segment_Animation__Sparkle_Hyper(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
void mAnimatorLight::SubTask_Segment_Animation__Sparkle_Hyper() // Firework_Rain
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  if(random8(5) < 2) {
    for(uint16_t i = 0; i < MAX(1, segment_length/3); i++) {
      SetPixelColor(random16(segment_length), SEGCOLOR(1));
    }
    _segments[segment_active_index].transition.time_ms =  20;
  }
  _segments[segment_active_index].transition.time_ms =  20 + (uint16_t)(255-_segments[segment_active_index].speed());
}




  #endif // ENABLE_EXTRA_WLED_EFFECTS


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * TO SORT UNDER
 ****************************************************************************************************************************
 ****************************************************************************************************************************/



  #ifdef ENABLE_EXTRA_WLED_EFFECTS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
void mAnimatorLight::mode_twinkleup(void) {                 // A very short twinkle routine with fade-in and dual controls. By Andrew Tuline.
  
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
void mAnimatorLight::SubTask_Segment_Animation__Twinkle_Up() // Firework_Rain
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


random16_set_seed(535);                                 // The randomizer needs to be re-set each time through the loop in order for the same 'random' numbers to be the same each time through.

  for (int i = 0; i<segment_length; i++) {
    uint8_t ranstart = random8();                         // The starting value (aka brightness) for each pixel. Must be consistent each time through the loop for this to work.
    uint8_t pixBri = sin8(ranstart + 16 * millis()/(256-_segments[segment_active_index].speed()));
    if (random8() > _segments[segment_active_index].intensity()) pixBri = 0;
    SetPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i*20, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



  #endif // ENABLE_EXTRA_WLED_EFFECTS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Random_Colour()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
/*
 * Lights all LEDs in one random color up. Then switches them
 * to the next random color.
 */
// void mAnimatorLight::mode_random_color(void) {
  uint32_t cycleTime = 200 + (255 - _segments[segment_active_index].speed())*50;
  uint32_t it = millis() / cycleTime;
  uint32_t rem = millis() % cycleTime;
  uint16_t fadedur = (cycleTime * _segments[segment_active_index].intensity()) >> 8;

  uint32_t fade = 255;
  if (fadedur) {
    fade = (rem * 255) / fadedur;
    if (fade > 255) fade = 255;
  }

  if (_segment_runtimes[segment_active_index].call == 0) {
    _segment_runtimes[segment_active_index].aux0 = random8();
    _segment_runtimes[segment_active_index].step = 2;
  }
  if (it != _segment_runtimes[segment_active_index].step) //new color
  {
    _segment_runtimes[segment_active_index].aux1 = _segment_runtimes[segment_active_index].aux0;
    _segment_runtimes[segment_active_index].aux0 = get_random_wheel_index(_segment_runtimes[segment_active_index].aux0); //aux0 will store our random color wheel index
    _segment_runtimes[segment_active_index].step = it;
  }

  fill(color_blend(color_wheel(_segment_runtimes[segment_active_index].aux1), color_wheel(_segment_runtimes[segment_active_index].aux0), fade));
  // return FRAMETIME;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

#ifdef ENABLE_EXTRA_WLED_EFFECTS

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Dynamic()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
/*
 * Lights every LED in a random color. Changes all LED at the same time
// * to new random colors.
 */
// void mAnimatorLight::mode_dynamic(void) {
  if (!_segment_runtimes[segment_active_index].allocateData(segment_length))
  {return;}
  //return mode_static(); //allocation failed
  
  if(_segment_runtimes[segment_active_index].call == 0) {
    for (uint16_t i = 0; i < segment_length; i++) _segment_runtimes[segment_active_index].data[i] = random8();
  }

  uint32_t cycleTime = 50 + (255 - _segments[segment_active_index].speed())*15;
  uint32_t it = millis() / cycleTime;
  if (it != _segment_runtimes[segment_active_index].step && _segments[segment_active_index].speed() != 0) //new color
  {
    for (uint16_t i = 0; i < segment_length; i++) {
      if (random8() <= _segments[segment_active_index].intensity()) _segment_runtimes[segment_active_index].data[i] = random8();
    }
    _segment_runtimes[segment_active_index].step = it;
  }
  
  for (uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_wheel(_segment_runtimes[segment_active_index].data[i]));
  }
  // return FRAMETIME;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Rainbow_Cycle()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

/*
 * Cycles a rainbow over the entire string of LEDs.
 */
// void mAnimatorLight::mode_rainbow_cycle(void) {
  uint16_t counter = (millis() * ((_segments[segment_active_index].speed() >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;
  
  for(uint16_t i = 0; i < segment_length; i++) {
    //intensity()/29 = 0 (1/16) 1 (1/8) 2 (1/4) 3 (1/2) 4 (1) 5 (2) 6 (4) 7 (8) 8 (16)
    uint8_t index = (i * (16 << (_segments[segment_active_index].intensity() /29)) / segment_length) + counter;
    SetPixelColor(i, color_wheel(index));
  }

  // return FRAMETIME;

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Running(bool saw)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
/*
 * Running lights effect with smooth sine transition base.
 */
// void mAnimatorLight::running_base(bool saw) {
  uint8_t x_scale = _segments[segment_active_index].intensity() >> 2;
  uint32_t counter = (millis() * _segments[segment_active_index].speed()) >> 9;

  for(uint16_t i = 0; i < segment_length; i++) {
    uint8_t s = 0;
    uint8_t a = i*x_scale - counter;
    if (saw) {
      if (a < 16)
      {
        a = 192 + a*8;
      } else {
        a = map(a,16,255,64,192);
      }
    }
    s = sin8(a);
    SetPixelColor(i, color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), s));
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


// /*
//  * Running lights effect with smooth sine transition.
//  */
// void mAnimatorLight::mode_running_lights(void) {
//   return running_base(false);
// }

void mAnimatorLight::SubTask_Segment_Animation__Running_Lights()
{
  SubTask_Segment_Animation__Base_Running(false);
}

// /*
//  * Running lights effect with sawtooth transition.
//  */
// void mAnimatorLight::mode_saw(void) {
//   return running_base(true);
// }

void mAnimatorLight::SubTask_Segment_Animation__Saw()
{
  SubTask_Segment_Animation__Base_Running(true);
}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Twinkle()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

// /*
//  * Blink several LEDs in random colors on, reset, repeat.
//  * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
//  */
// void mAnimatorLight::mode_twinkle(void) {
  fill(SEGCOLOR(1));

  uint32_t cycleTime = 20 + (255 - _segments[segment_active_index].speed())*5;
  uint32_t it = millis() / cycleTime;
  if (it != _segment_runtimes[segment_active_index].step)
  {
    uint16_t maxOn = map(_segments[segment_active_index].intensity(), 0, 255, 1, segment_length); // make sure at least one LED is on
    if (_segment_runtimes[segment_active_index].aux0 >= maxOn)
    {
      _segment_runtimes[segment_active_index].aux0 = 0;
      _segment_runtimes[segment_active_index].aux1 = random16(); //new seed for our PRNG
    }
    _segment_runtimes[segment_active_index].aux0++;
    _segment_runtimes[segment_active_index].step = it;
  }
  
  uint16_t PRNG16 = _segment_runtimes[segment_active_index].aux1;

  for (uint16_t i = 0; i < _segment_runtimes[segment_active_index].aux0; i++)
  {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 13849; // next 'random' number
    uint32_t p = (uint32_t)segment_length * (uint32_t)PRNG16;
    uint16_t j = p >> 16;
    SetPixelColor(j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }

  // return FRAMETIME;
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Dissolve(uint32_t color)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
// /*
//  * Dissolve function
//  */

//tmp added
uint8_t _brightness = 255;

// void mAnimatorLight::dissolve(uint32_t color) {
  bool wa = (SEGCOLOR(1) != 0 && _brightness < 255); //workaround, can't compare getPixel to color if not full brightness
  
  for (uint16_t j = 0; j <= segment_length / 15; j++)
  {
    if (random8() <= _segments[segment_active_index].intensity()) {
      for (uint8_t times = 0; times < 10; times++) //attempt to spawn a new pixel 5 times
      {
        uint16_t i = random16(segment_length);
        if (_segment_runtimes[segment_active_index].aux0) { //dissolve to primary/palette
          if (RgbcctColor::GetU32Colour(GetPixelColor(i)) == SEGCOLOR(1) || wa)
          {
            if (color == SEGCOLOR(0))
            {
              SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
            } else { SetPixelColor(i, color); }     
            break; //only spawn 1 new pixel per frame per 50 LEDs
          }
        } else { //dissolve to secondary
          if (RgbcctColor::GetU32Colour(GetPixelColor(i)) != SEGCOLOR(1)) { SetPixelColor(i, SEGCOLOR(1)); break; }
        }
      }
    }
  }

  if (_segment_runtimes[segment_active_index].call > (255 - _segments[segment_active_index].speed()) + 15) 
  {
    _segment_runtimes[segment_active_index].aux0 = !_segment_runtimes[segment_active_index].aux0;
    _segment_runtimes[segment_active_index].call = 0;
  }
  
  // return FRAMETIME;
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


// /*
//  * Blink several LEDs on and then off
//  */
// void mAnimatorLight::mode_dissolve(void) {
//   return dissolve(SEGCOLOR(0));
// }

void mAnimatorLight::SubTask_Segment_Animation__Dissolve()
{
  SubTask_Segment_Animation__Base_Dissolve(SEGCOLOR(0));
}


// /*
//  * Blink several LEDs on and then off in random colors
//  */
// void mAnimatorLight::mode_dissolve_random(void) {
//   return dissolve(color_wheel(random8()));
// }

void mAnimatorLight::SubTask_Segment_Animation__Dissolve_Random()
{
  SubTask_Segment_Animation__Base_Dissolve(color_wheel(random8()));
}





/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Android loading circle
 *
// void mAnimatorLight::mode_android(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Android()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }

  if (_segment_runtimes[segment_active_index].aux1 > ((float)_segments[segment_active_index].intensity()/255.0)*(float)segment_length)
  {
    _segment_runtimes[segment_active_index].aux0 = 1;
  } else
  {
    if (_segment_runtimes[segment_active_index].aux1 < 2) _segment_runtimes[segment_active_index].aux0 = 0;
  }

  uint16_t a = _segment_runtimes[segment_active_index].step;
  
  if (_segment_runtimes[segment_active_index].aux0 == 0)
  {
    if (_segment_runtimes[segment_active_index].call %3 == 1) {a++;}
    else {_segment_runtimes[segment_active_index].aux1++;}
  } else
  {
    a++;
    if (_segment_runtimes[segment_active_index].call %3 != 1) _segment_runtimes[segment_active_index].aux1--;
  }
  
  if (a >= segment_length) a = 0;

  if (a + _segment_runtimes[segment_active_index].aux1 < segment_length)
  {
    for(int i = a; i < a+_segment_runtimes[segment_active_index].aux1; i++) {
      SetPixelColor(i, SEGCOLOR(0));
    }
  } else
  {
    for(int i = a; i < segment_length; i++) {
      SetPixelColor(i, SEGCOLOR(0));
    }
    for(int i = 0; i < _segment_runtimes[segment_active_index].aux1 - (segment_length -a); i++) {
      SetPixelColor(i, SEGCOLOR(0));
    }
  }
  _segment_runtimes[segment_active_index].step = a;

  // return 
  
  _segments[segment_active_index].transition.rate_ms =  3 + ((8 * (uint32_t)(255 - _segments[segment_active_index].speed())) / segment_length);
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Red - Amber - Green - Blue lights running
 *
// void mAnimatorLight::mode_colorful(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__ColourFul()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint32_t cols[]{0x00FF0000,0x00EEBB00,0x0000EE00,0x000077CC,0x00FF0000,0x00EEBB00,0x0000EE00};
  if (_segments[segment_active_index].intensity() < 127) //pastel (easter) colors
  {
    cols[0] = 0x00FF8040;
    cols[1] = 0x00E5D241;
    cols[2] = 0x0077FF77;
    cols[3] = 0x0077F0F0;
    for (uint8_t i = 4; i < 7; i++) cols[i] = cols[i-4];
  }
  
  uint32_t cycleTime = 50 + (15 * (uint32_t)(255 - _segments[segment_active_index].speed()));
  uint32_t it = millis() / cycleTime;
  if (it != _segment_runtimes[segment_active_index].step)
  {
    if (_segments[segment_active_index].speed() > 0) _segment_runtimes[segment_active_index].aux0++;
    if (_segment_runtimes[segment_active_index].aux0 > 3) _segment_runtimes[segment_active_index].aux0 = 0;
    _segment_runtimes[segment_active_index].step = it;
  }
  
  uint16_t i = 0;
  for (i; i < segment_length -3; i+=4)
  {
    SetPixelColor(i, cols[_segment_runtimes[segment_active_index].aux0]);
    SetPixelColor(i+1, cols[_segment_runtimes[segment_active_index].aux0+1]);
    SetPixelColor(i+2, cols[_segment_runtimes[segment_active_index].aux0+2]);
    SetPixelColor(i+3, cols[_segment_runtimes[segment_active_index].aux0+3]);
  }
  if(i < segment_length)
  {
    SetPixelColor(i, cols[_segment_runtimes[segment_active_index].aux0]);
    
    if(i+1 < segment_length)
    {
      SetPixelColor(i+1, cols[_segment_runtimes[segment_active_index].aux0+1]);
      
      if(i+2 < segment_length)
      {
        SetPixelColor(i+2, cols[_segment_runtimes[segment_active_index].aux0+2]);
      }
    }
  }
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Emulates a traffic light.
 *
// void mAnimatorLight::mode_traffic_light(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Traffic_Light()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  for(uint16_t i=0; i < segment_length; i++)
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  uint32_t mdelay = 500;
  for (int i = 0; i < segment_length-2 ; i+=3)
  {
    switch (_segment_runtimes[segment_active_index].aux0)
    {
      case 0: SetPixelColor(i, 0x00FF0000); mdelay = 150 + (100 * (uint32_t)(255 - _segments[segment_active_index].speed()));break;
      case 1: SetPixelColor(i, 0x00FF0000); mdelay = 150 + (20 * (uint32_t)(255 - _segments[segment_active_index].speed())); SetPixelColor(i+1, 0x00EECC00); break;
      case 2: SetPixelColor(i+2, 0x0000FF00); mdelay = 150 + (100 * (uint32_t)(255 - _segments[segment_active_index].speed()));break;
      case 3: SetPixelColor(i+1, 0x00EECC00); mdelay = 150 + (20 * (uint32_t)(255 - _segments[segment_active_index].speed()));break;
    }
  }

  if (millis() - _segment_runtimes[segment_active_index].step > mdelay)
  {
    _segment_runtimes[segment_active_index].aux0++;
    if (_segment_runtimes[segment_active_index].aux0 == 1 && _segments[segment_active_index].intensity() > 140) _segment_runtimes[segment_active_index].aux0 = 2; //skip Red + Amber, to get US-style sequence
    if (_segment_runtimes[segment_active_index].aux0 > 3) _segment_runtimes[segment_active_index].aux0 = 0;
    _segment_runtimes[segment_active_index].step = millis();
  }
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


#endif // ENABLE_EXTRA_WLED_EFFECTS


#ifdef ENABLE_EXTRA_WLED_EFFECTS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Alternating pixels running function.
 *
// void mAnimatorLight::running(uint32_t color1, uint32_t color2) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Running(uint32_t color1, uint32_t color2)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint8_t pxw = 1 + (_segments[segment_active_index].intensity() >> 5);
  uint32_t cycleTime = 35 + (255 - _segments[segment_active_index].speed());
  uint32_t it = millis() / cycleTime;
  if (_segments[segment_active_index].speed() == 0) it = 0;

  for(uint16_t i = 0; i < segment_length; i++) {
    if((i + _segment_runtimes[segment_active_index].aux0) % (pxw*2) < pxw) {
      if (color1 == SEGCOLOR(0))
      {
        SetPixelColor(segment_length -i -1, color_from_palette(segment_length -i -1, true, PALETTE_SOLID_WRAP, 0));
      } else
      {
        SetPixelColor(segment_length -i -1, color1);
      }
    } else {
      SetPixelColor(segment_length -i -1, color2);
    }
  }

  if (it != _segment_runtimes[segment_active_index].step )
  {
    _segment_runtimes[segment_active_index].aux0 = (_segment_runtimes[segment_active_index].aux0 +1) % (pxw*2);
    _segment_runtimes[segment_active_index].step = it;
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}

/*
 * Alternating red/blue pixels running.
 */
void mAnimatorLight::SubTask_Segment_Animation__Running_Red_Blue(void) {
  return SubTask_Segment_Animation__Base_Running(RED, BLUE);
}

/*
 * Alternating color/sec pixels running.
 */
void mAnimatorLight::SubTask_Segment_Animation__Running_Colour(void) {
  return SubTask_Segment_Animation__Base_Running(SEGCOLOR(0), SEGCOLOR(1));
}

/*
 * Alternating red/green pixels running.
 */
void mAnimatorLight::SubTask_Segment_Animation__Merry_Christmas(void) {
  return SubTask_Segment_Animation__Base_Running(RED, GREEN);
}


/*
 * Alternating orange/purple pixels running.
 */
void mAnimatorLight::SubTask_Segment_Animation__Halloween(void) {
  return SubTask_Segment_Animation__Base_Running(PURPLE, ORANGE);
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Random colored pixels running.
 *
// void mAnimatorLight::mode_running_random(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Running_Random()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - _segments[segment_active_index].speed()));
  uint32_t it = millis() / cycleTime;
  if (_segment_runtimes[segment_active_index].aux1 == it){return;}// return FRAMETIME;

  for(uint16_t i=segment_length-1; i > 0; i--) {
    SetPixelColor( i, GetPixelColor( i - 1));
  }

  if(_segment_runtimes[segment_active_index].step == 0) {
    _segment_runtimes[segment_active_index].aux0 = get_random_wheel_index(_segment_runtimes[segment_active_index].aux0);
    SetPixelColor(0, color_wheel(_segment_runtimes[segment_active_index].aux0));
  }

  _segment_runtimes[segment_active_index].step++;
  if (_segment_runtimes[segment_active_index].step > ((255-_segments[segment_active_index].intensity()) >> 4))
  {
    _segment_runtimes[segment_active_index].step = 0;
  }

  _segment_runtimes[segment_active_index].aux1 = it;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}

#endif // ENABLE_EXTRA_WLED_EFFECTS




#ifdef ENABLE_EXTRA_WLED_EFFECTS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Gradient run base function
 *
// void mAnimatorLight::gradient_base(bool loading) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Gradient(bool loading)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 2) + 1);
  uint16_t pp = counter * segment_length >> 16;
  if (_segment_runtimes[segment_active_index].call == 0) pp = 0;
  float val; //0.0 = sec 1.0 = pri
  float brd = loading ? _segments[segment_active_index].intensity() : _segments[segment_active_index].intensity()/2;
  if (brd <1.0) brd = 1.0;
  int p1 = pp-segment_length;
  int p2 = pp+segment_length;

  for(uint16_t i = 0; i < segment_length; i++)
  {
    if (loading)
    {
      val = abs(((i>pp) ? p2:pp) -i);
    } else {
      val = MIN(abs(pp-i),MIN(abs(p1-i),abs(p2-i)));
    }
    val = (brd > val) ? val/brd * 255 : 255;
    SetPixelColor(i, color_blend(SEGCOLOR(0), color_from_palette(i, true, PALETTE_SOLID_WRAP, 1), val));
  }

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/*
 * Gradient run
 */
void mAnimatorLight::SubTask_Segment_Animation__Gradient(void) {
  return SubTask_Segment_Animation__Base_Gradient(false);
}


/*
 * Gradient run with hard transition
 */
void mAnimatorLight::SubTask_Segment_Animation__Loading(void) {
  return SubTask_Segment_Animation__Base_Gradient(true);
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//American Police Light with all LEDs Red and Blue 
// void mAnimatorLight::police_base(uint32_t color1, uint32_t color2, bool all)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Police(uint32_t color1, uint32_t color2, bool all)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 2) +1);
  uint16_t idexR = (counter * segment_length) >> 16;
  if (idexR >= segment_length) idexR = 0;

  uint16_t topindex = segment_length >> 1;
  uint16_t idexB = (idexR > topindex) ? idexR - topindex : idexR + topindex;
  if (_segment_runtimes[segment_active_index].call == 0) _segment_runtimes[segment_active_index].aux0 = idexR;
  if (idexB >= segment_length) idexB = 0; //otherwise overflow on odd number of LEDs

  if (all) { //different algo, ensuring immediate fill
    if (idexB > idexR) {
      fill(color2);
      for (uint16_t i = idexR; i < idexB; i++) SetPixelColor(i, color1);
    } else {
      fill(color1);
      for (uint16_t i = idexB; i < idexR; i++) SetPixelColor(i, color2);
    } 
  } else { //regular dot-only mode
    uint8_t size = 1 + _segments[segment_active_index].intensity() >> 3;
    if (size > segment_length/2) size = 1+ segment_length/2;
    for (uint8_t i=0; i <= size; i++) {
      SetPixelColor(idexR+i, color1);
      SetPixelColor(idexB+i, color2);
    }
    if (_segment_runtimes[segment_active_index].aux0 != idexR) {
      uint8_t gap = (_segment_runtimes[segment_active_index].aux0 < idexR)? idexR - _segment_runtimes[segment_active_index].aux0:segment_length - _segment_runtimes[segment_active_index].aux0 + idexR;
      for (uint8_t i = 0; i <= gap ; i++) {
        if ((idexR - i) < 0) idexR = segment_length-1 + i;
        if ((idexB - i) < 0) idexB = segment_length-1 + i;
        SetPixelColor(idexR-i, color1);
        SetPixelColor(idexB-i, color2);
      }
      _segment_runtimes[segment_active_index].aux0 = idexR;
    }
  }
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


//American Police Light with all LEDs Red and Blue 
void mAnimatorLight::SubTask_Segment_Animation__Polce_All()
{
  return SubTask_Segment_Animation__Base_Police(RED, BLUE, true);
}


//Police Lights Red and Blue 
void mAnimatorLight::SubTask_Segment_Animation__Police()
{
  fill(SEGCOLOR(1));

  return SubTask_Segment_Animation__Base_Police(RED, BLUE, false);
}


//Police All with custom colors
void mAnimatorLight::SubTask_Segment_Animation__Two_Areas()
{
  return SubTask_Segment_Animation__Base_Police(SEGCOLOR(0), SEGCOLOR(1), true);
}


//Police Lights with custom colors 
void mAnimatorLight::SubTask_Segment_Animation__Two_Dots()
{
  fill(SEGCOLOR(2));
  uint32_t color2 = (SEGCOLOR(1) == SEGCOLOR(2)) ? SEGCOLOR(0) : SEGCOLOR(1);

  return SubTask_Segment_Animation__Base_Police(SEGCOLOR(0), color2, false);
}

#endif // ENABLE_EXTRA_WLED_EFFECTS

#ifdef ENABLE_EXTRA_WLED_EFFECTS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Custom mode by Aircoookie. Color Wipe, but with 3 colors
 *
// void mAnimatorLight::mode_tricolor_wipe(void)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__TriColour()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint32_t cycleTime = 1000 + (255 - _segments[segment_active_index].speed())*200;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t ledIndex = (prog * segment_length * 3) >> 16;
  uint16_t ledOffset = ledIndex;

  for (uint16_t i = 0; i < segment_length; i++)
  {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 2));
  }
  
  if(ledIndex < segment_length) { //wipe from 0 to 1
    for (uint16_t i = 0; i < segment_length; i++)
    {
      SetPixelColor(i, (i > ledOffset)? SEGCOLOR(0) : SEGCOLOR(1));
    }
  } else if (ledIndex < segment_length*2) { //wipe from 1 to 2
    ledOffset = ledIndex - segment_length;
    for (uint16_t i = ledOffset +1; i < segment_length; i++)
    {
      SetPixelColor(i, SEGCOLOR(1));
    }
  } else //wipe from 2 to 0
  {
    ledOffset = ledIndex - segment_length*2;
    for (uint16_t i = 0; i <= ledOffset; i++)
    {
      SetPixelColor(i, SEGCOLOR(0));
    }
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Fades between 3 colors
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/TriFade.h
 * Modified by Aircoookie
 *
// void mAnimatorLight::mode_tricolor_fade(void)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Fade_TriColour()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 3) +1);
  uint32_t prog = (counter * 768) >> 16;

  uint32_t color1 = 0, color2 = 0;
  byte stage = 0;

  if(prog < 256) {
    color1 = SEGCOLOR(0);
    color2 = SEGCOLOR(1);
    stage = 0;
  } else if(prog < 512) {
    color1 = SEGCOLOR(1);
    color2 = SEGCOLOR(2);
    stage = 1;
  } else {
    color1 = SEGCOLOR(2);
    color2 = SEGCOLOR(0);
    stage = 2;
  }

  byte stp = prog; // % 256
  uint32_t color = 0;
  for(uint16_t i = 0; i < segment_length; i++) {
    if (stage == 2) {
      color = color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), color2, stp);
    } else if (stage == 1) {
      color = color_blend(color1, color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), stp);
    } else {
      color = color_blend(color1, color2, stp);
    }
    SetPixelColor(i, color);
  }

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Creates random comets
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/MultiComet.h
 *
// void mAnimatorLight::mode_multi_comet(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Multi_Comet()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint32_t cycleTime = 10 + (uint32_t)(255 - _segments[segment_active_index].speed());
  uint32_t it = millis() / cycleTime;
  if (_segment_runtimes[segment_active_index].step == it){
    // return FRAMETIME;

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

  }
  if (!_segment_runtimes[segment_active_index].allocateData(sizeof(uint16_t) * 8)){return;}// return mode_static(); //allocation failed
  
  fade_out(_segments[segment_active_index].intensity());
  
  uint16_t* comets = reinterpret_cast<uint16_t*>(_segment_runtimes[segment_active_index].data);

  for(uint8_t i=0; i < 8; i++) {
    if(comets[i] < segment_length) {
      uint16_t index = comets[i];
      if (SEGCOLOR(2) != 0)
      {
        SetPixelColor(index, i % 2 ? color_from_palette(index, true, PALETTE_SOLID_WRAP, 0) : SEGCOLOR(2));
      } else
      {
        SetPixelColor(index, color_from_palette(index, true, PALETTE_SOLID_WRAP, 0));
      }
      comets[i]++;
    } else {
      if(!random(segment_length)) {
        comets[i] = 0;
      }
    }
  }

  _segment_runtimes[segment_active_index].step = it;
  // return FRAMETIME;
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
/  Oscillating bars of color, updated with standard framerate
*
// void mAnimatorLight::mode_oscillate(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

typedef struct Oscillator {
  int16_t pos;
  int8_t  size;
  int8_t  dir;
  int8_t  speed;
} oscillator;

void mAnimatorLight::SubTask_Segment_Animation__Oscillate()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint8_t numOscillators = 3;
  uint16_t dataSize = sizeof(oscillator) * numOscillators;

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){return;}// return mode_static(); //allocation failed
  
  Oscillator* oscillators = reinterpret_cast<Oscillator*>(_segment_runtimes[segment_active_index].data);

  if (_segment_runtimes[segment_active_index].call == 0)
  {
    // oscillators[0] = {(uint16_t)segment_length/4,   segment_length/8,  1, 1};
    oscillators[0].pos = segment_length/4;
    oscillators[0].size = segment_length/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 1;
    // oscillators[1] = {segment_length/4*3, segment_length/8,  1, 2};
    oscillators[0].pos = segment_length/4*3;
    oscillators[0].size = segment_length/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 2;
    // oscillators[2] = {segment_length/4*2, segment_length/8, -1, 1};
    oscillators[0].pos = segment_length/4*2;
    oscillators[0].size = segment_length/8;
    oscillators[0].dir = -1;
    oscillators[0].speed = 1;

    
  }

  uint32_t cycleTime = 20 + (2 * (uint32_t)(255 - _segments[segment_active_index].speed()));
  uint32_t it = millis() / cycleTime;

  for(uint8_t i = 0; i < numOscillators; i++) {
    // if the counter has increased, move the oscillator by the random step
    if (it != _segment_runtimes[segment_active_index].step) oscillators[i].pos += oscillators[i].dir * oscillators[i].speed;
    oscillators[i].size = segment_length/(3+_segments[segment_active_index].intensity()/8);
    if((oscillators[i].dir == -1) && (oscillators[i].pos <= 0)) {
      oscillators[i].pos = 0;
      oscillators[i].dir = 1;
      // make bigger steps for faster speeds
      oscillators[i].speed = _segments[segment_active_index].speed() > 100 ? random8(2, 4):random8(1, 3);
    }
    if((oscillators[i].dir == 1) && (oscillators[i].pos >= (segment_length - 1))) {
      oscillators[i].pos = segment_length - 1;
      oscillators[i].dir = -1;
      oscillators[i].speed = _segments[segment_active_index].speed() > 100 ? random8(2, 4):random8(1, 3);
    }
  }

  for(uint16_t i=0; i < segment_length; i++) {
    uint32_t color = BLACK;
    for(uint8_t j=0; j < numOscillators; j++) {
      if(i >= oscillators[j].pos - oscillators[j].size && i <= oscillators[j].pos + oscillators[j].size) {
        color = (color == BLACK) ? SEGCOLOR(j) : color_blend(color, SEGCOLOR(j), 128);
      }
    }
    SetPixelColor(i, color);
  }
 
  _segment_runtimes[segment_active_index].step = it;

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// Pride2015
// Animated, ever-changing rainbows.
// by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// void mAnimatorLight::mode_pride_2015(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Pride_2015()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint16_t duration = 10 + _segments[segment_active_index].speed();
  uint16_t sPseudotime = _segment_runtimes[segment_active_index].step;
  uint16_t sHue16 = _segment_runtimes[segment_active_index].aux0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for (uint16_t i = 0 ; i < segment_length; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);
    fastled_col = col_to_crgb(RgbcctColor::GetU32Colour(GetPixelColor(i)));

    nblend(fastled_col, newcolor, 64);
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _segment_runtimes[segment_active_index].step = sPseudotime;
  _segment_runtimes[segment_active_index].aux0 = sHue16;
  // return FRAMETIME;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//eight colored dots, weaving in and out of sync with each other
// void mAnimatorLight::mode_juggle(void){

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Juggle()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  fade_out(_segments[segment_active_index].intensity());
  CRGB fastled_col;
  byte dothue = 0;
  for ( byte i = 0; i < 8; i++) {
    uint16_t index = 0 + beatsin88((128 + _segments[segment_active_index].speed())*(i + 7), 0, segment_length -1);
    fastled_col = RgbcctColor::GetU32Colour(GetPixelColor(index));
    fastled_col |= (_segments[segment_active_index].palette.id==0)?CHSV(dothue, 220, 255):ColorFromPalette(mPaletteI->currentPalette, dothue, 255);
    SetPixelColor(index, fastled_col.red, fastled_col.green, fastled_col.blue);
    dothue += 32;
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// void mAnimatorLight::mode_palette()
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Palette()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = 0;
  if (_segments[segment_active_index].speed() != 0) 
  {
    counter = (millis() * ((_segments[segment_active_index].speed() >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && _segments[segment_active_index].speed() == 0));
  for (uint16_t i = 0; i < segment_length; i++)
  {
    uint8_t colorIndex = (i * 255 / segment_length) - counter;
    
    if (noWrap) colorIndex = map(colorIndex, 0, 255, 0, 240); //cut off blend at palette "end"
    
    SetPixelColor(i, color_from_palette(colorIndex, false, true, 255));
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


// // WLED limitation: Analog Clock overlay will NOT work when Fire2012 is active
// // Fire2012 by Mark Kriegsman, July 2012
// // as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
// //// 
// // This basic one-dimensional 'fire' simulation works roughly as follows:
// // There's a underlying array of 'heat' cells, that model the temperature
// // at each point along the line.  Every cycle through the simulation, 
// // four steps are performed:
// //  1) All cells cool down a little bit, losing heat to the air
// //  2) The heat from each cell drifts 'up' and diffuses a little
// //  3) Sometimes randomly new 'sparks' of heat are added at the bottom
// //  4) The heat from each cell is rendered as a color into the leds array
// //     The heat-to-color mapping uses a black-body radiation approximation.
// //
// // Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
// //
// // This simulation scales it self a bit depending on NUM_LEDS; it should look
// // "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
// //
// // I recommend running this simulation at anywhere from 30-100 frames per second,
// // meaning an interframe delay of about 10-35 milliseconds.
// //
// // Looks best on a high-density LED setup (60+ pixels/meter).
// //
// //
// // There are two main parameters you can play with to control the look and
// // feel of your fire: COOLING (used in step 1 above) (Speed = COOLING), and SPARKING (used
// // in step 3 above) (Effect intensity() = Sparking).



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
// void mAnimatorLight::mode_colorwaves()
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__ColourWaves()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t duration = 10 + _segments[segment_active_index].speed();
  uint16_t sPseudotime = _segment_runtimes[segment_active_index].step;
  uint16_t sHue16 = _segment_runtimes[segment_active_index].aux0;

  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  // uint16_t hueinc16 = beatsin88(113, 300, 1500);
  uint16_t hueinc16 = beatsin88(113, 60, 300)*_segments[segment_active_index].intensity()*10/255;  // Use the intensity() Slider for the hues

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for ( uint16_t i = 0 ; i < segment_length; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = ColorFromPalette(mPaletteI->currentPalette, hue8, bri8);
    fastled_col = RgbcctColor::GetU32Colour(GetPixelColor(i));

    nblend(fastled_col, newcolor, 128);
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _segment_runtimes[segment_active_index].step = sPseudotime;
  _segment_runtimes[segment_active_index].aux0 = sHue16;
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
// void mAnimatorLight::mode_bpm()
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__BPM()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  CRGB fastled_col;
  uint32_t stp = (millis() / 20) & 0xFF;
  uint8_t beat = beatsin8(_segments[segment_active_index].speed(), 64, 255);
  for (uint16_t i = 0; i < segment_length; i++) {
    fastled_col = ColorFromPalette(mPaletteI->currentPalette, stp + (i * 2), beat - stp + (i * 10));
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}





/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//based on https://gist.github.com/kriegsman/5408ecd397744ba0393e
// void mAnimatorLight::mode_colortwinkle()
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Twinkle_Colour()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t dataSize = (segment_length+7) >> 3; //1 bit per LED
  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){return;}// return mode_static(); //allocation failed
  
  CRGB fastled_col, prev;
  fract8 fadeUpAmount = 8 + (_segments[segment_active_index].speed()/4), fadeDownAmount = 5 + (_segments[segment_active_index].speed()/7);
  for (uint16_t i = 0; i < segment_length; i++) {
    fastled_col = RgbcctColor::GetU32Colour(GetPixelColor(i));
    prev = fastled_col;
    uint16_t index = i >> 3;
    uint8_t  bitNum = i & 0x07;
    bool fadeUp = bitRead(_segment_runtimes[segment_active_index].data[index], bitNum);
    
    if (fadeUp) {
      CRGB incrementalColor = fastled_col;
      incrementalColor.nscale8_video( fadeUpAmount);
      fastled_col += incrementalColor;

      if (fastled_col.red == 255 || fastled_col.green == 255 || fastled_col.blue == 255) {
        bitWrite(_segment_runtimes[segment_active_index].data[index], bitNum, false);
      }
      SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);

      // if (RgbcctColor::GetU32Colour(GetPixelColor(i)) == prev) //fix "stuck" pixels
      // {
      //   fastled_col += fastled_col;
      //   SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
      // }
    } else {
      fastled_col.nscale8( 255 - fadeDownAmount);
      SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
    }
  }

  for (uint16_t j = 0; j <= segment_length / 50; j++)
  {
    if (random8() <= _segments[segment_active_index].intensity()) {
      for (uint8_t times = 0; times < 5; times++) //attempt to spawn a new pixel 5 times
      {
        int i = random16(segment_length);
        if(RgbcctColor::GetU32Colour(GetPixelColor(i)) == 0) {
          fastled_col = ColorFromPalette(mPaletteI->currentPalette, random8(), 64, NOBLEND);
          uint16_t index = i >> 3;
          uint8_t  bitNum = i & 0x07;
          bitWrite(_segment_runtimes[segment_active_index].data[index], bitNum, true);
          SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
          break; //only spawn 1 new pixel per frame per 50 LEDs
        }
      }
    }
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Calm effect, like a lake at night
// void mAnimatorLight::mode_lake() {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Lake()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint8_t sp = _segments[segment_active_index].speed()/10;
  int wave1 = beatsin8(sp +2, -64,64);
  int wave2 = beatsin8(sp +1, -64,64);
  uint8_t wave3 = beatsin8(sp +2,   0,80);
  CRGB fastled_col;

  for (uint16_t i = 0; i < segment_length; i++)
  {
    int index = cos8((i*15)+ wave1)/2 + cubicwave8((i*23)+ wave2)/2;           
    uint8_t lum = (index > wave3) ? index - wave3 : 0;
    fastled_col = ColorFromPalette(mPaletteI->currentPalette, map(index,0,255,0,240), lum, LINEARBLEND);
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// meteor effect
// send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
// void mAnimatorLight::mode_meteor() {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Meteor()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  if (!_segment_runtimes[segment_active_index].allocateData(segment_length)){return;}// return mode_static(); //allocation failed

  byte* trail = _segment_runtimes[segment_active_index].data;
  
  byte meteorSize= 1+ segment_length / 10;
  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 2) +8);
  uint16_t in = counter * segment_length >> 16;

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < segment_length; i++) {
    if (random8() <= 255 - _segments[segment_active_index].intensity())
    {
      byte meteorTrailDecay = 128 + random8(127);
      trail[i] = scale8(trail[i], meteorTrailDecay);
      SetPixelColor(i, color_from_palette(trail[i], false, true, 255));
    }
  }

  // draw meteor
  for(int j = 0; j < meteorSize; j++) {
    uint16_t index = in + j;
    if(index >= segment_length) {
      index = (in + j - segment_length);
    }

    trail[index] = 240;
    SetPixelColor(index, color_from_palette(trail[index], false, true, 255));
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// smooth meteor effect
// send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
// void mAnimatorLight::mode_meteor_smooth() {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Metoer_Smooth()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  if (!_segment_runtimes[segment_active_index].allocateData(segment_length)){return;}// return mode_static(); //allocation failed

  byte* trail = _segment_runtimes[segment_active_index].data;
  
  byte meteorSize= 1+ segment_length / 10;
  uint16_t in = map((_segment_runtimes[segment_active_index].step >> 6 & 0xFF), 0, 255, 0, segment_length -1);

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < segment_length; i++) {
    if (trail[i] != 0 && random8() <= 255 - _segments[segment_active_index].intensity())
    {
      int change = 3 - random8(12); //change each time between -8 and +3
      trail[i] += change;
      if (trail[i] > 245) trail[i] = 0;
      if (trail[i] > 240) trail[i] = 240;
      SetPixelColor(i, color_from_palette(trail[i], false, true, 255));
    }
  }
  
  // draw meteor
  for(int j = 0; j < meteorSize; j++) {  
    uint16_t index = in + j;   
    if(in + j >= segment_length) {
      index = (in + j - segment_length);
    }
    SetPixelColor(index, color_blend(RgbcctColor::GetU32Colour(GetPixelColor(index)), color_from_palette(240, false, true, 255), 48));
    trail[index] = 240;
  }

  _segment_runtimes[segment_active_index].step += _segments[segment_active_index].speed() +1;
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// CRGB mAnimatorLight::twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//  TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
//
//  TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
//  Colors are chosen from a palette. Read more about this effect using the link above!

// If COOL_LIKE_INCANDESCENT is set to 1, colors will
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1


CRGB mAnimatorLight::SubTask_Segment_Animation__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  // Overall twinkle speed() (changed)
  uint16_t ticks = ms / _segment_runtimes[segment_active_index].aux0;
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8(slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).
  // Default is 5.
  uint8_t twinkleDensity = (_segments[segment_active_index].intensity() >> 5) +1;

  uint8_t bright = 0;
  if (((slowcycle8 & 0x0E)/2) < twinkleDensity) {
    uint8_t ph = fastcycle8;
    // This is like 'triwave8', which produces a
    // symmetrical up-and-down triangle sawtooth waveform, except that this
    // function produces a triangle wave with a faster attack and a slower decay
    if (cat) //twinklecat, variant where the leds instantly turn on
    {
      bright = 255 - ph;
    } else { //vanilla twinklefox
      if (ph < 86) {
      bright = ph * 3;
      } else {
        ph -= 86;
        bright = 255 - (ph + (ph/2));
      }
    }
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if (bright > 0) {
    c = ColorFromPalette(mPaletteI->currentPalette, hue, bright, NOBLEND);
    if(COOL_LIKE_INCANDESCENT == 1) {
      // This code takes a pixel, and if its in the 'fading down'
      // part of the cycle, it adjusts the color a little bit like the
      // way that incandescent bulbs fade toward 'red' as they dim.
      if (fastcycle8 >= 128) 
      {
        uint8_t cooling = (fastcycle8 - 128) >> 4;
        c.g = qsub8(c.g, cooling);
        c.b = qsub8(c.b, cooling * 2);
      }
    }
  } else {
    c = HTMLColorCode::Black;
  }
  return c;
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
// void mAnimatorLight::twinklefox_base(bool cat)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Twinkle_Fox(bool cat)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  // Calculate speed()
  if (_segments[segment_active_index].speed() > 100) _segment_runtimes[segment_active_index].aux0 = 3 + ((255 - _segments[segment_active_index].speed()) >> 3);
  else _segment_runtimes[segment_active_index].aux0 = 22 + ((100 - _segments[segment_active_index].speed()) >> 1);

  // Set up the background color, "bg".
  CRGB bg;
  bg = col_to_crgb(SEGCOLOR(1));
  uint8_t bglight = bg.getAverageLight();
  if (bglight > 64) {
    bg.nscale8_video(16); // very bright, so scale to 1/16th
  } else if (bglight > 16) {
    bg.nscale8_video(64); // not that bright, so scale to 1/4th
  } else {
    bg.nscale8_video(86); // dim, scale to 1/3rd.
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for (uint16_t i = 0; i < segment_length; i++) {
  
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed() adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((millis() * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = SubTask_Segment_Animation__Base_Twinkle_Fox_One_Twinkle(myclock30, myunique8, cat);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if (deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      SetPixelColor(i, c.red, c.green, c.blue);
    } else if (deltabright > 0) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      SetPixelColor(i, color_blend(crgb_to_col(bg), crgb_to_col(c), deltabright * 8));
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      SetPixelColor(i, bg.r, bg.g, bg.b);
    }
  }
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}

void mAnimatorLight::SubTask_Segment_Animation__Twinkle_Fox()
{
  return SubTask_Segment_Animation__Base_Twinkle_Fox(false);
}

void mAnimatorLight::SubTask_Segment_Animation__Twinkle_Cat()
{
  return SubTask_Segment_Animation__Base_Twinkle_Fox(true);
}


#endif // ENABLE_EXTRA_WLED_EFFECTS


  #ifdef ENABLE_EXTRA_WLED_EFFECTS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Spots(uint16_t threshold)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

// void mAnimatorLight::spots_base(uint16_t threshold)
// {
  fill(SEGCOLOR(1));
  
  uint16_t maxZones = segment_length >> 2;
  uint16_t zones = 1 + ((_segments[segment_active_index].intensity() * maxZones) >> 8);
  uint16_t zoneLen = segment_length / zones;
  uint16_t offset = (segment_length - zones * zoneLen) >> 1;

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint16_t wave = triwave16((i * 0xFFFF) / zoneLen);
      if (wave > threshold) {
        uint16_t index = 0 + pos + i;
        uint8_t s = (wave - threshold)*255 / (0xFFFF - threshold);
        SetPixelColor(index, color_blend(color_from_palette(index, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), 255-s));
      }
    }
  }
  
//   return FRAMETIME;

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


// //intensity() slider sets number of "lights", speed() sets LEDs per light
// void mAnimatorLight::mode_spots()
// {
//   return spots_base((255 - _segments[segment_active_index].speed()) << 8);
// }

void mAnimatorLight::SubTask_Segment_Animation__Spots()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  SubTask_Segment_Animation__Base_Spots((255 - _segments[segment_active_index].speed()) << 8);
}


// //intensity() slider sets number of "lights", LEDs per light fade in and out
// void mAnimatorLight::mode_spots_fade()
// {
// }

void mAnimatorLight::SubTask_Segment_Animation__Fade_Spots()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 2) +8);
  uint16_t t = triwave16(counter);
  uint16_t tr = (t >> 1) + (t >> 2);
//   return spots_base(tr);
  SubTask_Segment_Animation__Base_Spots(tr);
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Rainbow with glitter, inspired by https://gist.github.com/kriegsman/062e10f7f07ba8518af6
void mAnimatorLight::mode_glitter()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Glitter()
{

  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  mode_palette();

  if (_segments[segment_active_index].intensity() > random8())
  {
    SetPixelColor(random16(segment_length), ULTRAWHITE);
  }
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
*  POPCORN
*  modified from https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Popcorn.h
*
// void mAnimatorLight::mode_popcorn(void) {#
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Popcorn()
{

  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  //allocate segment data
  uint16_t maxNumPopcorn = 24; 
  uint16_t dataSize = sizeof(spark) * maxNumPopcorn;
  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    return;
  }// return mode_static(); //allocation failed
  
  Spark* popcorn = reinterpret_cast<Spark*>(_segment_runtimes[segment_active_index].data);

  float gravity = -0.0001 - (_segments[segment_active_index].speed()/200000.0); // m/s/s
  gravity *= segment_length;

  bool hasCol2 = SEGCOLOR(2);
  fill(hasCol2 ? BLACK : SEGCOLOR(1));

  uint8_t numPopcorn = _segments[segment_active_index].intensity()*maxNumPopcorn/255;

  if (numPopcorn == 0) numPopcorn = 1;

  for(uint8_t i = 0; i < numPopcorn; i++) {
    bool isActive = popcorn[i].pos >= 0.0f;

    if (isActive) { // if kernel is active, update its position
      popcorn[i].pos += popcorn[i].vel;
      popcorn[i].vel += gravity;
      uint32_t col = color_wheel(popcorn[i].colIndex);
      if (!_segments[segment_active_index].palette.id && popcorn[i].colIndex < NUM_COLORS)
      {
        col = SEGCOLOR(popcorn[i].colIndex);
      }
      
      uint16_t ledIndex = popcorn[i].pos;
      if (ledIndex < segment_length) SetPixelColor(ledIndex, col);
    } else { // if kernel is inactive, randomly pop it
      if (random8() < 2) { // POP!!!
        popcorn[i].pos = 0.01f;
        
        uint16_t peakHeight = 128 + random8(128); //0-255
        peakHeight = (peakHeight * (segment_length -1)) >> 8;
        popcorn[i].vel = sqrt(-2.0 * gravity * peakHeight);
        
        if (_segments[segment_active_index].palette.id)
        {
          popcorn[i].colIndex = random8();
        } else {
          byte col = random8(0, NUM_COLORS);
          if (!hasCol2 || !SEGCOLOR(col)) col = 0;
          popcorn[i].colIndex = col;
        }
      }
    }
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

  #endif // ENABLE_EXTRA_WLED_EFFECTS

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Candle Flicker
 * @note : Part of WLED Effects
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
//values close to 100 produce 5Hz flicker, which looks very candle-y
//Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
//and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/

 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Candle_Single()
{
  SubTask_Segment_Animation__Candle_Base(false);
}

void mAnimatorLight::SubTask_Segment_Animation__Candle_Multi()
{
  SubTask_Segment_Animation__Candle_Base(true);
}

void mAnimatorLight::SubTask_Segment_Animation__Candle_Base(uint8_t use_multi)
{

  uint8_t segment_index = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel = _segments[segment_active_index].pixel_range.stop;

  // Set palette pointer
  mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);
  uint8_t pixels_in_palette = mPaletteI->GetPixelsInMap();
  // Brightness is generated internally, and rgbcct solid palettes are output values
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;

  if (use_multi)
  {
    //allocate segment data
    uint16_t dataSize = (segment_length -1) *3;
    #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
    // AddLog(LOG_LEVEL_TEST, PSTR("WS2812FX::candle dataSize=%d"),dataSize);
    #endif
    if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
    {
      // AddLog(LOG_LEVEL_TEST, PSTR("WS2812FX::candle dataSize=%d FAILED TO ALLOCATE" DEBUG_INSERT_PAGE_BREAK),dataSize);
      // either just fail, or force animation to use simple version instead
      // return candle(false); //allocation failed
    } 
  }

  //max. flicker range controlled by intensity()
  uint8_t valrange = _segments[segment_active_index].intensity();
  uint8_t rndval = valrange >> 1;

  #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
  // AddLog(LOG_LEVEL_TEST, PSTR("step=%d"),_segment_runtimes[segment_active_index].step);
  // AddLog(LOG_LEVEL_TEST, PSTR("valrange=%d"),valrange);
  // AddLog(LOG_LEVEL_TEST, PSTR("rndval=%d"),rndval);
  #endif

  //step (how much to move closer to target per frame) coarsely set by speed()
  uint8_t speedFactor = 4;
  if (_segments[segment_active_index].speed() > 252) { //epilepsy
    speedFactor = 1;
  } else if (_segments[segment_active_index].speed() > 99) { //regular candle (mode called every ~25 ms, so 4 frames to have a new target every 100ms)
    speedFactor = 2;
  } else if (_segments[segment_active_index].speed() > 49) { //slower fade
    speedFactor = 3;
  } //else 4 (slowest)

  uint16_t numCandles = (use_multi) ? segment_length : 1;

  for (uint16_t i = 0; i < numCandles; i++)
  {
    uint16_t d = 0; //data location

    uint8_t s = _segment_runtimes[segment_active_index].aux0, 
            s_target = _segment_runtimes[segment_active_index].aux1, 
            fadeStep = _segment_runtimes[segment_active_index].step;

    if (i > 0) {
      d = (i-1) *3;
      s = _segment_runtimes[segment_active_index].data[d]; 
      s_target = _segment_runtimes[segment_active_index].data[d+1]; 
      fadeStep = _segment_runtimes[segment_active_index].data[d+2];
    }
    if (fadeStep == 0) { //init vals
      s = 128; s_target = 130 + random8(4); fadeStep = 1;
    }

    bool newTarget = false;
    if (s_target > s) { //fade up

      #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
      // AddLog(LOG_LEVEL_TEST, PSTR("fade up s_target > s %d=%d"),s_target,s);
      #endif

      s = qadd8(s, fadeStep);
      if (s >= s_target) newTarget = true;
    } else {
      s = qsub8(s, fadeStep);
      if (s <= s_target) newTarget = true;
          
      #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
      // AddLog(LOG_LEVEL_TEST, PSTR("fade down=%d"),s);
      #endif

    }

    if (newTarget) {
      s_target = random8(rndval) + random8(rndval);
      if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
      uint8_t offset = (255 - valrange) >> 1;
      s_target += offset;

      uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
      fadeStep = dif >> speedFactor;
      if (fadeStep == 0) fadeStep = 1;
    }

    // uint8_t palette_with_multi_is_random = 1;

    /**
     * Generate show colour 
     */
    // blend ratio from WLED uses 255 range, neopixel is 0 to 1 range
    // blend ratio is really a brightness level
    float blend_ratio = mSupport::mapfloat(s, 0.0f, 255.0f, 0.0f, 1.0f);

    /**
     * If palette has only 1 colour, then second/bottom colour defaults to OFF
     * If pixels > 1, then use first two colours only or else horrible flicker
     * */
    RgbcctColor colour1 = RgbcctColor(0);
    RgbcctColor colour2 = RgbcctColor(0);
    RgbcctColor colour_blended = RgbcctColor(0);
    if(mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr) == 1)
    {
      colour1 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 0);
      colour2 = RgbcctColor(0);
      colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 

      // AddLog(LOG_LEVEL_TEST, PSTR("if(mPaletteI->GetPixelsInMap(telist.ptr) == %d)"),
      // // mPaletteI->palettelist.
      // );
    }else{
      // AddLog(LOG_LEVEL_TEST, PSTR("ELSEif(mPaletteI->GetPixelsInMap(telist.ptr) == 1)"));
      // if(palette_with_multi_is_random)
      // {
      //   colour1 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 0);
      //   colour2 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, random(0,pixels_in_palette-1));
      //   colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 
      //   colour_blended = ApplyBrightnesstoRgbcctColour(colour_blended, s);
      // }else{        
        colour1 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 0);
        colour2 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 1);
        colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 
      // }
    }

    // AddLog(LOG_LEVEL_TEST, PSTR("active_rgbcct_colour_p = %d,%d,%d,%d,%d"),
    // _segment_runtimes[0].active_rgbcct_colour_p[0],
    // _segment_runtimes[0].active_rgbcct_colour_p[1],
    // _segment_runtimes[0].active_rgbcct_colour_p[2],
    // _segment_runtimes[0].active_rgbcct_colour_p[3],
    // _segment_runtimes[0].active_rgbcct_colour_p[4]);

    /**
     * Apply colour to output: different per pixel
     **/
    if(i > 0) 
    {
   
      SetPixelColor(start_pixel + i, colour_blended, segment_index);

      _segment_runtimes[segment_active_index].data[d  ] = s; 
      _segment_runtimes[segment_active_index].data[d+1] = s_target; 
      _segment_runtimes[segment_active_index].data[d+2] = fadeStep;

    } 
    /**
     * Single mode, one colour applied across all leds
     * */
    else
    {
      
      for (uint16_t j = start_pixel; j <= stop_pixel; j++) {
        SetPixelColor(j, colour_blended, segment_index);
      }

      _segment_runtimes[segment_active_index].aux0 = s; 
      _segment_runtimes[segment_active_index].aux1 = s_target; 
      _segment_runtimes[segment_active_index].step = fadeStep;

    }
  }

  /**
   * Direct update, disable neopixel callback 
   **/
  StripUpdate();
  _segment_runtimes[segment_active_index].anim_function_callback = nullptr; // When no animation callback is needed

}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Candle_Flicker_Over_Palette -- this looks like shimmering lights
 * @note : Based on WLED candle effect
 * 
 * Static palette with pixels in order is redrawn, then blend is used to create flickering
 * 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Shimmering_Palette()
{

  uint8_t segment_index = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel = _segments[segment_active_index].pixel_range.stop;

  bool use_multi = true;

  // Set palette pointer
  mPaletteI->SetPaletteListPtrFromID(_segments[segment_active_index].palette.id);
  uint8_t pixels_in_palette = mPaletteI->GetPixelsInMap();
  // Brightness is generated internally, and rgbcct solid palettes are output values
  _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;

  if (use_multi)
  {
    //allocate segment data
    uint16_t dataSize = (segment_length -1) *3;
    #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
    // AddLog(LOG_LEVEL_TEST, PSTR("WS2812FX::candle dataSize=%d"),dataSize);
    #endif
    if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
    {
      // AddLog(LOG_LEVEL_TEST, PSTR("WS2812FX::candle dataSize=%d FAILED TO ALLOCATE" DEBUG_INSERT_PAGE_BREAK),dataSize);
      // either just fail, or force animation to use simple version instead
      // return candle(false); //allocation failed
    } 
  }

  //max. flicker range controlled by intensity()
  uint8_t valrange = _segments[segment_active_index].intensity();
  uint8_t rndval = valrange >> 1;

    uint8_t pixel_palette_counter = 0;

  #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
  // AddLog(LOG_LEVEL_TEST, PSTR("step=%d"),_segment_runtimes[segment_active_index].step);
  // AddLog(LOG_LEVEL_TEST, PSTR("valrange=%d"),valrange);
  // AddLog(LOG_LEVEL_TEST, PSTR("rndval=%d"),rndval);
  #endif

  //step (how much to move closer to target per frame) coarsely set by speed()
  uint8_t speedFactor = 4;
  if (_segments[segment_active_index].speed() > 252) { //epilepsy
    speedFactor = 1;
  } else if (_segments[segment_active_index].speed() > 99) { //regular candle (mode called every ~25 ms, so 4 frames to have a new target every 100ms)
    speedFactor = 2;
  } else if (_segments[segment_active_index].speed() > 49) { //slower fade
    speedFactor = 3;
  } //else 4 (slowest)

  uint16_t numCandles = (use_multi) ? segment_length : 1;

  for (uint16_t i = 0; i < numCandles; i++)
  {
    uint16_t d = 0; //data location

    uint8_t s = _segment_runtimes[segment_active_index].aux0, 
            s_target = _segment_runtimes[segment_active_index].aux1, 
            fadeStep = _segment_runtimes[segment_active_index].step;

    if (i > 0) {
      d = (i-1) *3;
      s = _segment_runtimes[segment_active_index].data[d]; 
      s_target = _segment_runtimes[segment_active_index].data[d+1]; 
      fadeStep = _segment_runtimes[segment_active_index].data[d+2];
    }
    if (fadeStep == 0) { //init vals
      s = 128; s_target = 130 + random8(4); fadeStep = 1;
    }

    bool newTarget = false;
    if (s_target > s) { //fade up

      #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
      // AddLog(LOG_LEVEL_TEST, PSTR("fade up s_target > s %d=%d"),s_target,s);
      #endif

      s = qadd8(s, fadeStep);
      if (s >= s_target) newTarget = true;
    } else {
      s = qsub8(s, fadeStep);
      if (s <= s_target) newTarget = true;
          
      #ifdef DEBUG_WLED_EFFECT_FUNCTIONS
      // AddLog(LOG_LEVEL_TEST, PSTR("fade down=%d"),s);
      #endif

    }

    if (newTarget) {
      s_target = random8(rndval) + random8(rndval);
      if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
      uint8_t offset = (255 - valrange) >> 1;
      s_target += offset;

      uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
      fadeStep = dif >> speedFactor;
      if (fadeStep == 0) fadeStep = 1;
    }


    // /**
    //  * @brief 
    //  * Trying here, can rgbbrightness be used as constraint on fadestep? and thus, its maximum brightness
    //  * 
    //  */
    // #ifdef ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
    // AddLog(LOG_LEVEL_TEST, PSTR("s1 = %d"), s);
    // s = constrain(s,0,pCONT_iLight->getBriRGB_Global());
    // AddLog(LOG_LEVEL_TEST, PSTR("\t\t\ts2 = %d"), s);
    // #endif // ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT

    // uint8_t palette_with_multi_is_random = 1;


    // GetColourFromPalette();
    // counter for segment length, reset 2nd counter for pixel_palette_size
    // blend with "off"




    /**
     * If palette has only 1 colour, then second/bottom colour defaults to OFF
     * If pixels > 1, then use first two colours only or else horrible flicker
     * */
    // if(mPaletteI->GetPixelsInMap(mPaletteI->palettelist.ptr) == 1)
    // {
    //   colour1 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 0);
    //   colour2 = RgbcctColor(0);
    //   colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 

    //   // AddLog(LOG_LEVEL_TEST, PSTR("if(mPaletteI->GetPixelsInMap(telist.ptr) == %d)"),
    //   // // mPaletteI->palettelist.
    //   // );
    // }else{
    //   // AddLog(LOG_LEVEL_TEST, PSTR("ELSEif(mPaletteI->GetPixelsInMap(telist.ptr) == 1)"));
    //   // if(palette_with_multi_is_random)
    //   // {
    //   //   colour1 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 0);
    //   //   colour2 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, random(0,pixels_in_palette-1));
    //   //   colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 
    //   //   colour_blended = ApplyBrightnesstoRgbcctColour(colour_blended, s);
    //   // }else{        
    //     colour1 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 1);
    //     colour2 = mPaletteI->GetColourFromPalette(mPaletteI->palettelist.ptr, 2);
    //     colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 
    //   // }
    // }

    // AddLog(LOG_LEVEL_TEST, PSTR("active_rgbcct_colour_p = %d,%d,%d,%d,%d"),
    // _segment_runtimes[0].active_rgbcct_colour_p[0],
    // _segment_runtimes[0].active_rgbcct_colour_p[1],
    // _segment_runtimes[0].active_rgbcct_colour_p[2],
    // _segment_runtimes[0].active_rgbcct_colour_p[3],
    // _segment_runtimes[0].active_rgbcct_colour_p[4]);

    /**
     * Apply colour to output: different per pixel
     **/
    if(i > 0) 
    {
   
    RgbcctColor colour1 = RgbcctColor(0);
    RgbcctColor colour2 = RgbcctColor(0);
    RgbcctColor colour_blended = RgbcctColor(0);
    // blend ratio from WLED uses 255 range, neopixel is 0 to 1 range
    // blend ratio is really a brightness level
    float blend_ratio = mSupport::mapfloat(s, 0.0f, 255.0f, 0.0f, 1.0f);
    // for(uint16_t p = start_pixel;
    //              p <= stop_pixel;
    //              p++
    //   ){

        colour1 = mPaletteI->GetColourFromPalette(nullptr, pixel_palette_counter);

        /**
         * @brief 
         * To apply constrain, should I change the "full" colour brightness? this might work (at least temporarily)
         * 
         */
        // #ifdef ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
        colour1 = ApplyBrightnesstoRgbcctColour(colour1, pCONT_iLight->getBriRGB_Global());
        // #endif // ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT

        colour2 = RgbcctColor(0);
        colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 

        if(pixel_palette_counter++ >= pixels_in_palette-1)
        {
          pixel_palette_counter = 0;
        }

       SetPixelColor(start_pixel + i, colour_blended, segment_index);

      // }


      _segment_runtimes[segment_active_index].data[d  ] = s; 
      _segment_runtimes[segment_active_index].data[d+1] = s_target; 
      _segment_runtimes[segment_active_index].data[d+2] = fadeStep;

    } 
    /**
     * Single mode, one colour applied across all leds
     * */
    else
    {
      
    RgbcctColor colour1 = RgbcctColor(0);
    RgbcctColor colour2 = RgbcctColor(0);
    RgbcctColor colour_blended = RgbcctColor(0);
    // blend ratio from WLED uses 255 range, neopixel is 0 to 1 range
    // blend ratio is really a brightness level
    float blend_ratio = mSupport::mapfloat(s, 0.0f, 255.0f, 0.0f, 1.0f);
    uint8_t pixel_palette_counter = 0;
    for(uint16_t p = start_pixel;
                 p <= stop_pixel;
                 p++
      ){
        colour1 = mPaletteI->GetColourFromPalette(nullptr, pixel_palette_counter);
        
        /**
         * @brief 
         * To apply constrain, should I change the "full" colour brightness? this might work (at least temporarily)
         * 
         */
        // #ifdef ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
        colour1 = ApplyBrightnesstoRgbcctColour(colour1, pCONT_iLight->getBriRGB_Global());
        // #endif // ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT


        colour2 = RgbcctColor(0);
        colour_blended = RgbcctColor::LinearBlend(colour1, colour2, blend_ratio); 

        SetPixelColor(p, colour_blended, segment_index);

        if(pixel_palette_counter++ > pixels_in_palette)
        {
          pixel_palette_counter = 0;
        }


      }

      // for (uint16_t j = start_pixel; j <= stop_pixel; j++) {
      //   SetPixelColor(j, colour_blended, segment_index);
      // }

      _segment_runtimes[segment_active_index].aux0 = s; 
      _segment_runtimes[segment_active_index].aux1 = s_target; 
      _segment_runtimes[segment_active_index].step = fadeStep;

    }
  }

  /**
   * Direct update, disable neopixel callback 
   **/
  StripUpdate();
  _segment_runtimes[segment_active_index].anim_function_callback = nullptr; // When no animation callback is needed

}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks Starburst
 * @note : Converted from WLED Effects
 * 
// /*
// / Fireworks in starburst effect
// / based on the video: https://www.reddit.com/r/arduino/comments/c3sd46/i_made_this_fireworks_effect_for_my_led_strips/
// / Speed sets frequency of new starbursts, intensity() is the intensity() of the burst
// *
 @param aux0 Current center index of firework (which blurs outwards)
 @param aux1 Previous location of center index of firework
 @param aux2 Iter to wrap palette

This appear functionally identical to candle single, and I will likely delete it


 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Fire_Flicker()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


// /*
//  * Fire flicker function
//  */
// void mAnimatorLight::mode_fire_flicker(void) {

  uint32_t cycleTime = 40 + (255 - _segments[segment_active_index].speed());
  uint32_t it = millis() / cycleTime;
  if (_segment_runtimes[segment_active_index].step == it) return;// FRAMETIME;
  
  byte w = (SEGCOLOR(0) >> 24) & 0xFF;
  byte r = (SEGCOLOR(0) >> 16) & 0xFF;
  byte g = (SEGCOLOR(0) >>  8) & 0xFF;
  byte b = (SEGCOLOR(0)        & 0xFF);
  byte lum = (_segments[segment_active_index].palette.id == 0) ? MAX(w, MAX(r, MAX(g, b))) : 255;
  lum /= (((256-_segments[segment_active_index].intensity())/16)+1);
  for(uint16_t i = 0; i < segment_length; i++) {
    byte flicker = random8(lum);
    if (_segments[segment_active_index].palette.id == 0) {
      SetPixelColor(i, MAX(r - flicker, 0), MAX(g - flicker, 0), MAX(b - flicker, 0), MAX(w - flicker, 0));
    } else {
      SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0, 255 - flicker));
    }
  }

  _segment_runtimes[segment_active_index].step = it;
  // return FRAMETIME;

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed

  StripUpdate();

}

// #ifdef ENABLE_ADVANCED_EFFECTS


  #ifdef ENABLE_EXTRA_WLED_EFFECTS
/********************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************
   @brief: Plasma
   @note : Converted from WLED Effects
           Draws growing/shrinking spans of concentric faded light
           adapted from https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino
   @note   Rate: FRAME_RATE

   @param rev             Reversed
   @param useRandomColors 
*******************************************************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Segment_Animation__Plasma()
{

  uint8_t thisPhase = beatsin8(6,-64,64);                       // Setting phase change for a couple of waves.
  uint8_t thatPhase = beatsin8(7,-64,64);

  for (int i = 0; i < _segments[segment_active_index].length(); i++) {   // For each of the LED's in the strand, set color &  brightness based on a wave as follows:
    uint8_t colorIndex = cubicwave8((i*(1+ 3*(_segments[segment_active_index].speed() >> 5)))+(thisPhase) & 0xFF)/2   // factor=23 // Create a wave and add a phase change and add another wave with its own phase change.
                             + cos8((i*(1+ 2*(_segments[segment_active_index].speed() >> 5)))+(thatPhase) & 0xFF)/2;  // factor=15 // Hey, you can even change the frequencies if you wish.
    uint8_t thisBright = qsub8(colorIndex, beatsin8(6,0, (255 - _segments[segment_active_index].intensity())|0x01 ));
    CRGB color = ColorFromPalette(mPaletteI->currentPalette, colorIndex, thisBright, LINEARBLEND);
    SetPixelColor(i, color.red, color.green, color.blue);
  }

} 

  #endif // ENABLE_EXTRA_WLED_EFFECTS

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Percentage display
 * Intesity values from 0-100 turn on the leds.
 *
void mAnimatorLight::mode_percent(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Percent()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;

	uint8_t percent = MAX(0, MIN(200, _segments[segment_active_index].intensity()));
	uint16_t active_leds = (percent < 100) ? segment_length * percent / 100.0
                                         : segment_length * (200 - percent) / 100.0;
  
  uint8_t size = (1 + ((_segments[segment_active_index].speed() * segment_length) >> 11));
  if (_segments[segment_active_index].speed() == 255) size = 255;
    
  if (percent < 100) {
    for (uint16_t i = 0; i < segment_length; i++) {
	  	if (i < _segment_runtimes[segment_active_index].step) {
        SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
	  	}
	  	else {
        SetPixelColor(i, SEGCOLOR(1));
	  	}
	  }
  } else {
    for (uint16_t i = 0; i < segment_length; i++) {
	  	if (i < (segment_length - _segment_runtimes[segment_active_index].step)) {
        SetPixelColor(i, SEGCOLOR(1));
	  	}
	  	else {
        SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
	  	}
	  }
  }

  if(active_leds > _segment_runtimes[segment_active_index].step) {  // smooth transition to the target value
    _segment_runtimes[segment_active_index].step += size;
    if (_segment_runtimes[segment_active_index].step > active_leds) _segment_runtimes[segment_active_index].step = active_leds;
  } else if (active_leds < _segment_runtimes[segment_active_index].step) {
    if (_segment_runtimes[segment_active_index].step > size) _segment_runtimes[segment_active_index].step -= size; else _segment_runtimes[segment_active_index].step = 0;
    if (_segment_runtimes[segment_active_index].step < active_leds) _segment_runtimes[segment_active_index].step = active_leds;
  }

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


  #ifdef ENABLE_EXTRA_WLED_EFFECTS
//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//
//
// In this animation, there are four "layers" of waves of light.  
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then 
// another filter is applied that adds "whitecaps" of brightness where the 
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed() and scale and motion each layer varies slowly within independent 
// hand-chosen ranges, which is why the code has a lot of low-speed() 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
// Modified for WLED, based on https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino
//

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::mode_pacifica()
{
/*
*  Bouncing Balls Effect
*
void mAnimatorLight::mode_bouncing_balls(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Pacifica()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  
  CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
  CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
  CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

  if (_segments[segment_active_index].palette.id) {
    pacifica_palette_1 = mPaletteI->currentPalette;
    pacifica_palette_2 = mPaletteI->currentPalette;
    pacifica_palette_3 = mPaletteI->currentPalette;
  }

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed(), and the speeds vary over time.
  uint16_t sCIStart1 = _segment_runtimes[segment_active_index].aux0, sCIStart2 = _segment_runtimes[segment_active_index].aux1, sCIStart3 = _segment_runtimes[segment_active_index].step, sCIStart4 = _segment_runtimes[segment_active_index].step >> 16;
  //static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  uint32_t deltams = 26 + (_segments[segment_active_index].speed() >> 3);
  
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));
  _segment_runtimes[segment_active_index].aux0 = sCIStart1; _segment_runtimes[segment_active_index].aux1 = sCIStart2;
  _segment_runtimes[segment_active_index].step = sCIStart4; _segment_runtimes[segment_active_index].step = (_segment_runtimes[segment_active_index].step << 16) + sCIStart3;

  // Clear out the LED array to a dim background blue-green
  //fill(132618);

  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < segment_length; i++) {
    CRGB c = CRGB(2, 6, 10);
    // Render each of four layers, with different scales and speeds, that vary over time
    c += pacifica_one_layer(i, pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0-beat16(301));
    c += pacifica_one_layer(i, pacifica_palette_2, sCIStart2, beatsin16(4,  6 * 256,  9 * 256), beatsin8(17, 40,  80),   beat16(401));
    c += pacifica_one_layer(i, pacifica_palette_3, sCIStart3,                         6 * 256 , beatsin8(9, 10,38)   , 0-beat16(503));
    c += pacifica_one_layer(i, pacifica_palette_3, sCIStart4,                         5 * 256 , beatsin8(8, 10,28)   ,   beat16(601));
    
    // Add extra 'white' to areas where the four layers of light have lined up brightly
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = c.getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      c += CRGB(overage, overage2, qadd8(overage2, overage2));
    }

    //deepen the blues and greens
    c.blue  = scale8(c.blue,  145); 
    c.green = scale8(c.green, 200); 
    c |= CRGB( 2, 5, 7);

    SetPixelColor(i, c.red, c.green, c.blue);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

// Add one layer of waves into the led array
CRGB mAnimatorLight::pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale >> 1) + 20;
  
  waveangle += ((120 + _segments[segment_active_index].intensity()) * i); //original 250 * i
  uint16_t s16 = sin16(waveangle) + 32768;
  uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += (cs * i);
  uint16_t sindex16 = sin16(ci) + 32768;
  uint8_t sindex8 = scale16(sindex16, 240);
  return ColorFromPalette(p, sindex8, bri, LINEARBLEND);
}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Solid colour background with glitter
void mAnimatorLight::mode_solid_glitter()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Solid_Glitter()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  fill(SEGCOLOR(0));

  if (_segments[segment_active_index].intensity() > random8())
  {
    SetPixelColor(random16(segment_length), ULTRAWHITE);
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Mode simulates a gradual sunrise
 *
// void mAnimatorLight::mode_sunrise() {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Sunrise()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  //speed() 0 - static sun
  //speed() 1 - 60: sunrise time in minutes
  //speed() 60 - 120 : sunset time in minutes - 60;
  //speed() above: "breathing" rise and set
  if (_segment_runtimes[segment_active_index].call == 0 || _segments[segment_active_index].speed() != _segment_runtimes[segment_active_index].aux0) {
	  _segment_runtimes[segment_active_index].step = millis(); //save starting time, millis() because now can change from sync
    _segment_runtimes[segment_active_index].aux0 = _segments[segment_active_index].speed();
  }
  
  fill(0);
  uint16_t stage = 0xFFFF;
  
  uint32_t s10SinceStart = (millis() - _segment_runtimes[segment_active_index].step) /100; //tenths of seconds
  
  if (_segments[segment_active_index].speed() > 120) { //quick sunrise and sunset
	  uint16_t counter = (millis() >> 1) * (((_segments[segment_active_index].speed() -120) >> 1) +1);
	  stage = triwave16(counter);
  } else if (_segments[segment_active_index].speed()) { //sunrise
	  uint8_t durMins = _segments[segment_active_index].speed();
	  if (durMins > 60) durMins -= 60;
	  uint32_t s10Target = durMins * 600;
	  if (s10SinceStart > s10Target) s10SinceStart = s10Target;
	  stage = map(s10SinceStart, 0, s10Target, 0, 0xFFFF);
	  if (_segments[segment_active_index].speed() > 60) stage = 0xFFFF - stage; //sunset
  }
  
  for (uint16_t i = 0; i <= segment_length/2; i++)
  {
    //default palette is Fire
    uint32_t c = color_from_palette(0, false, true, 255); //background

    uint16_t wave = triwave16((i * stage) / segment_length);

    wave = (wave >> 8) + ((wave * _segments[segment_active_index].intensity()) >> 15);

    if (wave > 240) { //clipped, full white sun
      c = color_from_palette( 240, false, true, 255);
    } else { //transition
      c = color_from_palette(wave, false, true, 255);
    }
    SetPixelColor(i, c);
    SetPixelColor(segment_length - i - 1, c);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 


// Sine waves that have controllable phase change speed(), frequency and cutoff. By Andrew Tuline.
// _segments[segment_active_index].speed() ->Speed, _segments[segment_active_index].intensity() -> Frequency (_segments[segment_active_index].fft1 -> Color change, _segments[segment_active_index].fft2 -> PWM cutoff)
//
void mAnimatorLight::mode_sinewave(void) {             // Adjustable sinewave. By Andrew Tuline

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Sinewave()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  //#define qsuba(x, b)  ((x>b)?x-b:0)               // Analog Unsigned subtraction macro. if result <0, then => 0

  uint16_t colorIndex = millis() /32;//(256 - _segments[segment_active_index].fft1);  // Amount of colour change.

  _segment_runtimes[segment_active_index].step += _segments[segment_active_index].speed()/16;                   // Speed of animation.
  uint16_t freq = _segments[segment_active_index].intensity()/4;//_segments[segment_active_index].fft2/8;                       // Frequency of the signal.

  for (int i=0; i<segment_length; i++) {                   // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int pixBri = cubicwave8((i*freq)+_segment_runtimes[segment_active_index].step);//qsuba(cubicwave8((i*freq)+_segment_runtimes[segment_active_index].step), (255-_segments[segment_active_index].intensity())); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    //setPixCol(i, i*colorIndex/255, pixBri);
    SetPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i*colorIndex/255, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }


  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 


/*
 * Best of both worlds from Palette and Spot effects. By Aircoookie
 *
// void mAnimatorLight::mode_flow(void)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Flow()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  uint16_t counter = 0;
  if (_segments[segment_active_index].speed() != 0) 
  {
    counter = millis() * ((_segments[segment_active_index].speed() >> 2) +1);
    counter = counter >> 8;
  }
  
  uint16_t maxZones = segment_length / 6; //only looks good if each zone has at least 6 LEDs
  uint16_t zones = (_segments[segment_active_index].intensity() * maxZones) >> 8;
  if (zones & 0x01) zones++; //zones must be even
  if (zones < 2) zones = 2;
  uint16_t zoneLen = segment_length / zones;
  uint16_t offset = (segment_length - zones * zoneLen) >> 1;

  fill(color_from_palette(-counter, false, true, 255));

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint8_t colorIndex = (i * 255 / zoneLen) - counter;
      uint16_t led = (z & 0x01) ? i : (zoneLen -1) -i;
      if (IS_REVERSE) led = (zoneLen -1) -led;
      SetPixelColor(pos + led, color_from_palette(colorIndex, false, true, 255));
    }
  }

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


  #endif // ENABLE_EXTRA_WLED_EFFECTS

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Static
 ****************************************************************************************************************************
 ****************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * No blinking. Just plain old static light.
 *
void mAnimatorLight::mode_static(void)
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Static()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;

  fill(SEGCOLOR(0));

  // return 
  _segments[segment_active_index].transition.time_ms = 
  (_segments[segment_active_index].getOption(SEG_OPTION_TRANSITIONAL)) ? FRAMETIME : 500; //update faster if in transition

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Speed slider sets amount of LEDs lit, intensity() sets unlit
void mAnimatorLight::mode_static_pattern()
{

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Static_Pattern()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t lit = 1 + _segments[segment_active_index].speed();
  uint16_t unlit = 1 + _segments[segment_active_index].intensity();
  bool drawingLit = true;
  uint16_t cnt = 0;

  for (uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, 
      (drawingLit) ? 
      color_from_palette(i, true, PALETTE_SOLID_WRAP, 0) : 
      SEGCOLOR(1)
    );
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::mode_tri_static_pattern()
{

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Segment_Animation__Static_Pattern()
// {
  
//   uint8_t segment_index  = segment_active_index;
//   uint16_t segment_length = _segments[segment_active_index].length();
//   uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
//   uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
//   _virtualSegmentLength = segment_length;

//   uint8_t segSize = (_segments[segment_active_index].intensity() >> 5) +1;
//   uint8_t currSeg = 0;
//   uint16_t currSegCount = 0;

//   for (uint16_t i = 0; i < segment_length; i++) {
//     if ( currSeg % 3 == 0 ) {
//       SetPixelColor(i, SEGCOLOR(0));
//     } else if( currSeg % 3 == 1) {
//       SetPixelColor(i, SEGCOLOR(1));
//     } else {
//       SetPixelColor(i, (SEGCOLOR(2) > 0 ? SEGCOLOR(2) : WHITE));
//     }
//     currSegCount += 1;
//     if (currSegCount >= segSize) {
//       currSeg +=1;
//       currSegCount = 0;
//     }
//   }

//   // return FRAMETIME;  
//   _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
//   StripUpdate();

// }

    #ifdef ENABLE_EXTRA_WLED_EFFECTS


// /****************************************************************************************************************************
//  **************************************************************************************************************************** 
//  * Blink
//  ****************************************************************************************************************************
//  ****************************************************************************************************************************/

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
/*
 * Blink/strobe function
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 * NOTE: Maybe re-rework without timer
 */
// void mAnimatorLight::blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette) {
  uint16_t stateTime = _segment_runtimes[segment_active_index].aux1;
  uint32_t cycleTime = (255 - _segments[segment_active_index].speed())*20;
  uint32_t onTime = 0;
  uint32_t offTime = cycleTime;

  if (!strobe) {
    onTime = (cycleTime * _segments[segment_active_index].intensity()) >> 8;
    offTime = cycleTime - onTime;
  }
  
  stateTime = ((_segment_runtimes[segment_active_index].aux0 & 1) == 0) ? onTime : offTime;
  stateTime += 20;
    
  if (millis() - _segment_runtimes[segment_active_index].step > stateTime)
  {
    _segment_runtimes[segment_active_index].aux0++;
    _segment_runtimes[segment_active_index].aux1 = stateTime;
    _segment_runtimes[segment_active_index].step = millis();
  }

  uint32_t color = ((_segment_runtimes[segment_active_index].aux0 & 1) == 0) ? color1 : color2;
  if (color == color1 && do_palette)
  {
    for(uint16_t i = 0; i < segment_length; i++) {
      SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  } else fill(color);

  // return FRAMETIME;
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Normal blinking. 50% on/off time.
 *
// void mAnimatorLight::mode_blink(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Blink()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  // return 
  SubTask_Segment_Animation__Base_Blink(SEGCOLOR(0), SEGCOLOR(1), false, true);
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * Classic Blink effect. Cycling through the rainbow.
 *
// void mAnimatorLight::mode_blink_rainbow(void) {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Blink_Rainbow()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  // return 
  SubTask_Segment_Animation__Base_Blink(color_wheel(_segment_runtimes[segment_active_index].call & 0xFF), SEGCOLOR(1), false, false);
  
  
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Classic Strobe effect.
 *
// void mAnimatorLight::mode_strobe(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Strobe()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  // return blink(SEGCOLOR(0), SEGCOLOR(1), true, true);
  SubTask_Segment_Animation__Base_Blink(SEGCOLOR(0), SEGCOLOR(1), true, true);

}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Strobe effect with different strobe count and pause, controlled by speed().
 *
// void mAnimatorLight::mode_multi_strobe(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Strobe_Multi()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  for(uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }
  //blink(SEGCOLOR(0), SEGCOLOR(1), true, true);

  uint16_t delay = 50 + 20*(uint16_t)(255-_segments[segment_active_index].speed());
  uint16_t count = 2 * ((_segments[segment_active_index].speed() / 10) + 1);
  if(_segment_runtimes[segment_active_index].step < count) {
    if((_segment_runtimes[segment_active_index].step & 1) == 0) {
      for(uint16_t i = 0; i < segment_length; i++) {
        SetPixelColor(i, SEGCOLOR(0));
      }
      delay = 20;
    } else {
      delay = 50;
    }
  }
  _segment_runtimes[segment_active_index].step = (_segment_runtimes[segment_active_index].step + 1) % (count + 1);
  // return delay;
  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();


}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Classic Strobe effect. Cycling through the rainbow.
 *
// void mAnimatorLight::mode_strobe_rainbow(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Strobe_Rainbow()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  // return blink(color_wheel(_segment_runtimes[segment_active_index].call & 0xFF), SEGCOLOR(1), true, false);
  SubTask_Segment_Animation__Base_Blink(color_wheel(_segment_runtimes[segment_active_index].call & 0xFF), SEGCOLOR(1), true, false);

}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Rainbow()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

/*
 * Cycles all LEDs at once through a rainbow.
 */
// void mAnimatorLight::mode_rainbow(void) {

  uint16_t counter = (millis() * ((_segments[segment_active_index].speed() >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  if (_segments[segment_active_index].intensity() < 128){
    fill(color_blend(color_wheel(counter),WHITE,128-_segments[segment_active_index].intensity()));
  } else {
    fill(color_wheel(counter));
  }

  // return FRAMETIME;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * // void mAnimatorLight::mode_lightning(void)
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Lightning()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  uint16_t ledstart = random16(segment_length);               // Determine starting location of flash
  uint16_t ledlen = 1 + random16(segment_length -ledstart);    // Determine length of flash (not to go beyond NUM_LEDS-1)
  uint8_t bri = 255/random8(1, 3);

  if (_segment_runtimes[segment_active_index].step == 0)
  {
    _segment_runtimes[segment_active_index].aux0 = random8(3, 3 + _segments[segment_active_index].intensity()/20); //number of flashes
    bri = 52;
    _segment_runtimes[segment_active_index].aux1 = 1;
  }

  fill(SEGCOLOR(1));

  if (_segment_runtimes[segment_active_index].aux1) {
    for (int i = ledstart; i < ledstart + ledlen; i++)
    {
      if (_segments[segment_active_index].palette.id == 0)
      {
        SetPixelColor(i,bri,bri,bri,bri);
      } else {
        SetPixelColor(i,color_from_palette(i, true, PALETTE_SOLID_WRAP, 0, bri));
      }
    }
    _segment_runtimes[segment_active_index].aux1 = 0;
    _segment_runtimes[segment_active_index].step++;
    // return random8(4, 10);                                    // each flash only lasts 4-10 milliseconds
    _segments[segment_active_index].transition.rate_ms = random8(4, 10);                                    // each flash only lasts 4-10 milliseconds
  }

  _segment_runtimes[segment_active_index].aux1 = 1;
  if (_segment_runtimes[segment_active_index].step == 1) _segments[segment_active_index].transition.rate_ms = 200;//return (200);                       // longer delay until next flash after the leader

  if (_segment_runtimes[segment_active_index].step <= _segment_runtimes[segment_active_index].aux0) _segments[segment_active_index].transition.rate_ms = /*return*/ (50 + random8(100));  // shorter delay between strokes

  _segment_runtimes[segment_active_index].step = 0;
  // return 
  
  
  _segments[segment_active_index].transition.rate_ms = (random8(255 - _segments[segment_active_index].speed()) * 100);                            // delay between strikes


  
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// void mAnimatorLight::mode_fire_2012()
// {

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Fire_2012()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint32_t it = millis() >> 5; //div 32

  if (!_segment_runtimes[segment_active_index].allocateData(segment_length)){return;} // return mode_static(); //allocation failed
  
  byte* heat = _segment_runtimes[segment_active_index].data;

  if (it != _segment_runtimes[segment_active_index].step)
  {
    // Step 1.  Cool down every cell a little
    for (uint16_t i = 0; i < segment_length; i++) {
      _segment_runtimes[segment_active_index].data[i] = qsub8(heat[i],  random8(0, (((20 + _segments[segment_active_index].speed() /3) * 10) / segment_length) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint16_t k= segment_length -1; k > 1; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() <= _segments[segment_active_index].intensity()) {
      uint8_t y = random8(7);
      if (y < segment_length) heat[y] = qadd8(heat[y], random8(160,255));
    }
    _segment_runtimes[segment_active_index].step = it;
  }

  // Step 4.  Map from heat cells to LED colors
  for (uint16_t j = 0; j < segment_length; j++) {
    CRGB color = ColorFromPalette(mPaletteI->currentPalette, MIN(heat[j],240), 255, LINEARBLEND);
    SetPixelColor(j, color.red, color.green, color.blue);
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Railway Crossing / Christmas Fairy lights
// void mAnimatorLight::mode_railway()
// {


 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Railway()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t dur = 40 + (255 - _segments[segment_active_index].speed()) * 10;
  uint16_t rampdur = (dur * _segments[segment_active_index].intensity()) >> 8;
  if (_segment_runtimes[segment_active_index].step > dur)
  {
    //reverse direction
    _segment_runtimes[segment_active_index].step = 0;
    _segment_runtimes[segment_active_index].aux0 = !_segment_runtimes[segment_active_index].aux0;
  }
  uint8_t pos = 255;
  if (rampdur != 0)
  {
    uint16_t p0 = (_segment_runtimes[segment_active_index].step * 255) / rampdur;
    if (p0 < 255) pos = p0;
  }
  if (_segment_runtimes[segment_active_index].aux0) pos = 255 - pos;
  for (uint16_t i = 0; i < segment_length; i += 2)
  {
    SetPixelColor(i, color_from_palette(255 - pos, false, false, 255));
    if (i < segment_length -1)
    {
      SetPixelColor(i + 1, color_from_palette(pos, false, false, 255));
    }
  }
  _segment_runtimes[segment_active_index].step += FRAMETIME;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}





/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
/ Modulates the brightness similar to a heartbeat
*
void mAnimatorLight::mode_heartbeat(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Heartbeat()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint8_t bpm = 40 + (_segments[segment_active_index].speed() >> 4);
  uint32_t msPerBeat = (60000 / bpm);
  uint32_t secondBeat = (msPerBeat / 3);

  uint32_t bri_lower = _segment_runtimes[segment_active_index].aux1;
  bri_lower = bri_lower * 2042 / (2048 + _segments[segment_active_index].intensity());
  _segment_runtimes[segment_active_index].aux1 = bri_lower;

  unsigned long beatTimer = millis() - _segment_runtimes[segment_active_index].step;
  if((beatTimer > secondBeat) && !_segment_runtimes[segment_active_index].aux0) { // time for the second beat?
    _segment_runtimes[segment_active_index].aux1 = UINT16_MAX; //full bri
    _segment_runtimes[segment_active_index].aux0 = 1;
  }
  if(beatTimer > msPerBeat) { // time to reset the beat timer?
    _segment_runtimes[segment_active_index].aux1 = UINT16_MAX; //full bri
    _segment_runtimes[segment_active_index].aux0 = 0;
    _segment_runtimes[segment_active_index].step = millis();
  }

  for (uint16_t i = 0; i < segment_length; i++) {
    SetPixelColor(i, color_blend(color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), 255 - (_segment_runtimes[segment_active_index].aux1 >> 8)));
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Noise
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
void mAnimatorLight::mode_fillnoise8()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__FillNoise8()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  if (_segment_runtimes[segment_active_index].call == 0) _segment_runtimes[segment_active_index].step = random16(12345);
  CRGB fastled_col;
  for (uint16_t i = 0; i < segment_length; i++) {
    uint8_t index = inoise8(i * segment_length, _segment_runtimes[segment_active_index].step + i * segment_length);
    fastled_col = ColorFromPalette(mPaletteI->currentPalette, index, 255, LINEARBLEND);
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _segment_runtimes[segment_active_index].step += beatsin8(_segments[segment_active_index].speed(), 1, 6); //10,1,4

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::mode_noise16_1()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Noise16_1()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t scale = 320;                                      // the "zoom factor" for the noise
  CRGB fastled_col;
  _segment_runtimes[segment_active_index].step += (1 + _segments[segment_active_index].speed()/16);

  for (uint16_t i = 0; i < segment_length; i++) {

    uint16_t shift_x = beatsin8(11);                           // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = _segment_runtimes[segment_active_index].step/42;             // the y position becomes slowly incremented


    uint16_t real_x = (i + shift_x) * scale;                  // the x position of the noise field swings @ 17 bpm
    uint16_t real_y = (i + shift_y) * scale;                  // the y position becomes slowly incremented
    uint32_t real_z = _segment_runtimes[segment_active_index].step;                          // the z position becomes quickly incremented

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                         // map LED color based on noise data

    fastled_col = ColorFromPalette(mPaletteI->currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::mode_noise16_2()
{

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Noise16_2()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t scale = 1000;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  _segment_runtimes[segment_active_index].step += (1 + (_segments[segment_active_index].speed() >> 1));

  for (uint16_t i = 0; i < segment_length; i++) {

    uint16_t shift_x = _segment_runtimes[segment_active_index].step >> 6;                         // x as a function of time
    uint16_t shift_y = _segment_runtimes[segment_active_index].step/42;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field

    uint8_t noise = inoise16(real_x, 0, 4223) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(mPaletteI->currentPalette, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::mode_noise16_3()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Noise16_3()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t scale = 800;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  _segment_runtimes[segment_active_index].step += (1 + _segments[segment_active_index].speed());

  for (uint16_t i = 0; i < segment_length; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = _segment_runtimes[segment_active_index].step*8;  

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(mPaletteI->currentPalette, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 


//https://github.com/aykevl/ledstrip-spark/blob/master/ledstrip->ino
void mAnimatorLight::mode_noise16_4()
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Noise16_4()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  CRGB fastled_col;
  uint32_t stp = (millis() * _segments[segment_active_index].speed()) >> 7;
  for (uint16_t i = 0; i < segment_length; i++) {
    int16_t index = inoise16(uint32_t(i) << 12, stp);
    fastled_col = ColorFromPalette(mPaletteI->currentPalette, index);
    SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// Peaceful noise that's slow and with gradually changing palettes. Does not support WLED palettes or default colours or controls.
void mAnimatorLight::mode_noisepal(void) {                                    // Slow noise palette by Andrew Tuline.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Noise_Pal()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t scale = 15 + (_segments[segment_active_index].intensity() >> 2); //default was 30
  //#define scale 30

  uint16_t dataSize = sizeof(CRGBPalette16) * 2; //allocate space for 2 Palettes
  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){return;}// return mode_static(); //allocation failed

  CRGBPalette16* palettes = reinterpret_cast<CRGBPalette16*>(_segment_runtimes[segment_active_index].data);

  uint16_t changePaletteMs = 4000 + _segments[segment_active_index].speed() *10; //between 4 - 6.5sec
  if (millis() - _segment_runtimes[segment_active_index].step > changePaletteMs)
  {
    _segment_runtimes[segment_active_index].step = millis();

    uint8_t baseI = random8();
    palettes[1] = CRGBPalette16(CHSV(baseI+random8(64), 255, random8(128,255)), CHSV(baseI+128, 255, random8(128,255)), CHSV(baseI+random8(92), 192, random8(128,255)), CHSV(baseI+random8(92), 255, random8(128,255)));
  }

  CRGB color;

  //EVERY_N_MILLIS(10) { //(don't have to time this, effect function is only called every 24ms)
  nblendPaletteTowardPalette(palettes[0], palettes[1], 48);               // Blend towards the target palette over 48 iterations.

  if (_segments[segment_active_index].palette.id > 0) palettes[0] = mPaletteI->currentPalette;

  for(int i = 0; i < segment_length; i++) {
    uint8_t index = inoise8(i*scale, _segment_runtimes[segment_active_index].aux0+i*scale);                // Get a value from the noise function. I'm using both x and y axis.
    color = ColorFromPalette(palettes[0], index, 255, LINEARBLEND);       // Use the my own palette.
    SetPixelColor(i, color.red, color.green, color.blue);
  }

  _segment_runtimes[segment_active_index].aux0 += beatsin8(10,1,4);                                        // Moving along the distance. Vary it a bit with a sine wave.

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Effects by Andrew Tuline
 *
// void mAnimatorLight::phased_base(uint8_t moder) {                  // We're making sine waves here. By Andrew Tuline.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Phased(uint8_t moder)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  

  uint8_t allfreq = 16;                                          // Base frequency.
  //float* phasePtr = reinterpret_cast<float*>(_segment_runtimes[segment_active_index].step);       // Phase change value gets calculated.
  static float phase = 0;//phasePtr[0];
  uint8_t cutOff = (255-_segments[segment_active_index].intensity());                      // You can change the number of pixels.  AKA intensity() (was 192).
  uint8_t modVal = 5;//_segments[segment_active_index].fft1/8+1;                         // You can change the modulus. AKA FFT1 (was 5).

  uint8_t index = millis()/64;                                    // Set color rotation speed()
  phase += _segments[segment_active_index].speed()/32.0;                                   // You can change the speed() of the wave. AKA SPEED (was .4)
  //phasePtr[0] = phase; 

  for (int i = 0; i < segment_length; i++) {
    if (moder == 1) modVal = (inoise8(i*10 + i*10) /16);         // Let's randomize our mod length with some Perlin noise.
    uint16_t val = (i+1) * allfreq;                              // This sets the frequency of the waves. The +1 makes sure that leds[0] is used.
    if (modVal == 0) modVal = 1;
    val += phase * (i % modVal +1) /2;                           // This sets the varying phase change of the waves. By Andrew Tuline.
    uint8_t b = cubicwave8(val);                                 // Now we make an 8 bit sinewave.
    b = (b > cutOff) ? (b - cutOff) : 0;                         // A ternary operator to cutoff the light.
    SetPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(index, false, false, 0), b));
    index += 256 / segment_length;
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}


void mAnimatorLight::SubTask_Segment_Animation__PhasedNoise(void) {
  return SubTask_Segment_Animation__Base_Phased(1);
}


void mAnimatorLight::SubTask_Segment_Animation__Phased(void) {
  return SubTask_Segment_Animation__Base_Phased(0);
}







/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Scan
 ****************************************************************************************************************************
 ****************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Base function for scanners
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Scan(bool dual)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

/*
 * Scan mode parent function
 */
// void mAnimatorLight::scan(bool dual)
// {
  uint32_t cycleTime = 750 + (255 - _segments[segment_active_index].speed())*150;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t size = 1 + ((_segments[segment_active_index].intensity() * segment_length) >> 9);
  uint16_t ledIndex = (prog * ((segment_length *2) - size *2)) >> 16;

  fill(SEGCOLOR(1));

  int led_offset = ledIndex - (segment_length - size);
  led_offset = abs(led_offset);

  if (dual) {
    for (uint16_t j = led_offset; j < led_offset + size; j++) {
      uint16_t i2 = segment_length -1 -j;
      SetPixelColor(i2, color_from_palette(i2, true, PALETTE_SOLID_WRAP, (SEGCOLOR(2))? 2:0));
    }
  }

  for (uint16_t j = led_offset; j < led_offset + size; j++) {
    SetPixelColor(j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}


/*
 * Runs a single pixel back and forth.
 */
// void mAnimatorLight::mode_scan(void) {
//   return scan(false);
// }
void mAnimatorLight::SubTask_Segment_Animation__Scan()
{
  SubTask_Segment_Animation__Base_Scan(false);
}



/*
 * Runs two pixel back and forth in opposite directions.
 */
// void mAnimatorLight::mode_dual_scan(void) {
//   return scan(true);
// }
void mAnimatorLight::SubTask_Segment_Animation__Scan_Dual()
{
  SubTask_Segment_Animation__Base_Scan(true);
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::larson_scanner(bool dual) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Larson_Scanner(bool dual)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >> 2) +8);
  uint16_t index = counter * segment_length  >> 16;

  fade_out(_segments[segment_active_index].intensity());

  if (_segment_runtimes[segment_active_index].step > index && _segment_runtimes[segment_active_index].step - index > segment_length/2) {
    _segment_runtimes[segment_active_index].aux0 = !_segment_runtimes[segment_active_index].aux0;
  }
  
  for (uint16_t i = _segment_runtimes[segment_active_index].step; i < index; i++) {
    uint16_t j = (_segment_runtimes[segment_active_index].aux0)?i:segment_length-1-i;
    SetPixelColor( j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }
  if (dual) {
    uint32_t c;
    if (SEGCOLOR(2) != 0) {
      c = SEGCOLOR(2);
    } else {
      c = color_from_palette(index, true, PALETTE_SOLID_WRAP, 0);
    }

    for (uint16_t i = _segment_runtimes[segment_active_index].step; i < index; i++) {
      uint16_t j = (_segment_runtimes[segment_active_index].aux0)?segment_length-1-i:i;
      SetPixelColor(j, c);
    }
  }

  _segment_runtimes[segment_active_index].step = index;
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

/*
 * Creates two Larson scanners moving in opposite directions
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/DualLarson.h
 */
void mAnimatorLight::SubTask_Segment_Animation__Larson_Scanner_Dual(void){
  return SubTask_Segment_Animation__Base_Larson_Scanner(true);
}

/*
 * K.I.T.T.
 */
// void mAnimatorLight::mode_larson_scanner(void){
//   return larson_scanner(false);
// }
void mAnimatorLight::SubTask_Segment_Animation__Larson_Scanner()
{
  SubTask_Segment_Animation__Base_Larson_Scanner(false);
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
/*
 * ICU mode
 *
// void mAnimatorLight::mode_icu(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__ICU()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t dest = _segment_runtimes[segment_active_index].step & 0xFFFF;
  uint8_t space = (_segments[segment_active_index].intensity() >> 3) +2;

  fill(SEGCOLOR(1));

  byte pindex = map(dest, 0, segment_length-segment_length/space, 0, 255);
  uint32_t col = color_from_palette(pindex, false, false, 0);

  SetPixelColor(dest, col);
  SetPixelColor(dest + segment_length/space, col);

  if(_segment_runtimes[segment_active_index].aux0 == dest) { // pause between eye movements
    if(random8(6) == 0) { // blink once in a while
      SetPixelColor(dest, SEGCOLOR(1));
      SetPixelColor(dest + segment_length/space, SEGCOLOR(1));
      _segments[segment_active_index].transition.rate_ms = 200;//return 200;
    }
    _segment_runtimes[segment_active_index].aux0 = random16(segment_length-segment_length/space);
    _segments[segment_active_index].transition.rate_ms = 1000 + random16(2000);//return 1000 + random16(2000);
  }

  if(_segment_runtimes[segment_active_index].aux0 > _segment_runtimes[segment_active_index].step) {
    _segment_runtimes[segment_active_index].step++;
    dest++;
  } else if (_segment_runtimes[segment_active_index].aux0 < _segment_runtimes[segment_active_index].step) {
    _segment_runtimes[segment_active_index].step--;
    dest--;
  }

  SetPixelColor(dest, col);
  SetPixelColor(dest + segment_length/space, col);

  // return SPEED_FORMULA_L;

_segments[segment_active_index].transition.rate_ms = SPEED_FORMULA_L;

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// void mAnimatorLight::ripple_base(bool rainbow)
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
//Water ripple
//propagation velocity from speed()
//drop rate from intensity()

//4 bytes
typedef struct Ripple {
  uint8_t state;
  uint8_t color;
  uint16_t pos;
} ripple;



void mAnimatorLight::SubTask_Segment_Animation__Base_Ripple(bool rainbow)
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t maxRipples = 1 + (segment_length >> 2);
  if (maxRipples > 100) maxRipples = 100;
  uint16_t dataSize = sizeof(ripple) * maxRipples;

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){return;} //return mode_static(); //allocation failed
 
  Ripple* ripples = reinterpret_cast<Ripple*>(_segment_runtimes[segment_active_index].data);

  // ranbow background or chosen background, all very dim.
  if (rainbow) {
    if (_segment_runtimes[segment_active_index].call ==0) {
      _segment_runtimes[segment_active_index].aux0 = random8();
      _segment_runtimes[segment_active_index].aux1 = random8();
    }
    if (_segment_runtimes[segment_active_index].aux0 == _segment_runtimes[segment_active_index].aux1) {
      _segment_runtimes[segment_active_index].aux1 = random8();
    }
    else if (_segment_runtimes[segment_active_index].aux1 > _segment_runtimes[segment_active_index].aux0) {
      _segment_runtimes[segment_active_index].aux0++;
    } else {
      _segment_runtimes[segment_active_index].aux0--;
    }
    fill(color_blend(color_wheel(_segment_runtimes[segment_active_index].aux0),BLACK,235));
  } else {
    fill(SEGCOLOR(1));
  }
  
  //draw wave
  for (uint16_t i = 0; i < maxRipples; i++)
  {
    uint16_t ripplestate = ripples[i].state;
    if (ripplestate)
    {
      uint8_t rippledecay = (_segments[segment_active_index].speed() >> 4) +1; //faster decay if faster propagation
      uint16_t rippleorigin = ripples[i].pos;
      uint32_t col = color_from_palette(ripples[i].color, false, false, 255);
      uint16_t propagation = ((ripplestate/rippledecay -1) * _segments[segment_active_index].speed());
      int16_t propI = propagation >> 8;
      uint8_t propF = propagation & 0xFF;
      int16_t left = rippleorigin - propI -1;
      uint8_t amp = (ripplestate < 17) ? triwave8((ripplestate-1)*8) : map(ripplestate,17,255,255,2);

      for (int16_t v = left; v < left +4; v++)
      {
        uint8_t mag = scale8(cubicwave8((propF>>2)+(v-left)*64), amp);
        if (v < segment_length && v >= 0)
        {
          SetPixelColor(v, color_blend(RgbcctColor::GetU32Colour(GetPixelColor(v)), col, mag));
        }
        int16_t w = left + propI*2 + 3 -(v-left);
        if (w < segment_length && w >= 0)
        {
          SetPixelColor(w, color_blend(RgbcctColor::GetU32Colour(GetPixelColor(w)), col, mag));
        }
      }  
      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else //randomly create new wave
    {
      if (random16(IBN + 10000) <= _segments[segment_active_index].intensity())
      {
        ripples[i].state = 1;
        ripples[i].pos = random16(segment_length);
        ripples[i].color = random8(); //color
      }
    }
  }
  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}

void mAnimatorLight::SubTask_Segment_Animation__Ripple(void) {
  return SubTask_Segment_Animation__Base_Ripple(false);
}

void mAnimatorLight::SubTask_Segment_Animation__Ripple_Rainbow(void) {
  return SubTask_Segment_Animation__Base_Ripple(true);
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
 * Firing comets from one end. "Lighthouse"
 *
// void mAnimatorLight::mode_comet(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Comet()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  uint16_t counter = millis() * ((_segments[segment_active_index].speed() >>2) +1);
  uint16_t index = counter * segment_length >> 16;
  if (_segment_runtimes[segment_active_index].call == 0) _segment_runtimes[segment_active_index].aux0 = index;

  fade_out(_segments[segment_active_index].intensity());

  SetPixelColor( index, color_from_palette(index, true, PALETTE_SOLID_WRAP, 0));
  if (index > _segment_runtimes[segment_active_index].aux0) {
    for (uint16_t i = _segment_runtimes[segment_active_index].aux0; i < index ; i++) {
       SetPixelColor( i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  } else if (index < _segment_runtimes[segment_active_index].aux0 && index < 10) {
    for (uint16_t i = 0; i < index ; i++) {
       SetPixelColor( i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }      
  }
  _segment_runtimes[segment_active_index].aux0 = index++;

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 


/*
 * Dots waving around in a sine/pendulum motion.
 * Little pixel birds flying in a circle. By Aircoookie
 *
void mAnimatorLight::mode_chunchun(void)
{
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Chunchun()
{
  
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;
  
  fill(SEGCOLOR(1));
  uint16_t counter = millis()*(6 + (_segments[segment_active_index].speed() >> 4));
  uint16_t numBirds = segment_length >> 2;
  uint16_t span = _segments[segment_active_index].intensity() << 8;

  for (uint16_t i = 0; i < numBirds; i++)
  {
    counter -= span/numBirds;
    int megumin = sin16(counter) + 0x8000;
    uint32_t bird = (megumin * segment_length) >> 16;
    uint32_t c = color_from_palette((i * 255)/ numBirds, false, true, 0);
    SetPixelColor(bird, c);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}

//each needs 12 bytes
//Spark type is used for popcorn and 1D fireworks
typedef struct Ball {
  unsigned long lastBounceTime;
  float impactVelocity;
  float height;
} ball;

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

/*
*  Bouncing Balls Effect
*
void mAnimatorLight::mode_bouncing_balls(void) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Bouncing_Balls()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  //allocate segment data
  uint16_t maxNumBalls = 16; 
  uint16_t dataSize = sizeof(ball) * maxNumBalls;
  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)){return;}// return mode_static(); //allocation failed
  
  Ball* balls = reinterpret_cast<Ball*>(_segment_runtimes[segment_active_index].data);
  
  // number of balls based on intensity() setting to max of 7 (cycles colors)
  // non-chosen color is a random color
  uint8_t numBalls = int(((_segments[segment_active_index].intensity() * (maxNumBalls - 0.8f)) / 255) + 1);
  
  float gravity                           = -9.81; // standard value of gravity
  float impactVelocityStart               = sqrt( -2 * gravity);

  unsigned long time = millis();

  if (_segment_runtimes[segment_active_index].call == 0) {
    for (uint8_t i = 0; i < maxNumBalls; i++) balls[i].lastBounceTime = time;
  }
  
  bool hasCol2 = SEGCOLOR(2);
  fill(hasCol2 ? BLACK : SEGCOLOR(1));
  
  for (uint8_t i = 0; i < numBalls; i++) {
    float timeSinceLastBounce = (time - balls[i].lastBounceTime)/((255-_segments[segment_active_index].speed())*8/256 +1);
    balls[i].height = 0.5 * gravity * pow(timeSinceLastBounce/1000 , 2.0) + balls[i].impactVelocity * timeSinceLastBounce/1000;

    if (balls[i].height < 0) { //start bounce
      balls[i].height = 0;
      //damping for better effect using multiple balls
      float dampening = 0.90 - float(i)/pow(numBalls,2);
      balls[i].impactVelocity = dampening * balls[i].impactVelocity;
      balls[i].lastBounceTime = time;

      if (balls[i].impactVelocity < 0.015) {
        balls[i].impactVelocity = impactVelocityStart;
      }
    }
    
    uint32_t color = SEGCOLOR(0);
    if (_segments[segment_active_index].palette.id) {
      color = color_wheel(i*(256/MAX(numBalls, 8)));
    } else if (hasCol2) {
      color = SEGCOLOR(i % NUM_COLORS);
    }

    uint16_t pos = round(balls[i].height * (segment_length - 1));
    SetPixelColor(pos, color);
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();

}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

void mAnimatorLight::sinelon_base(bool dual, bool rainbow=false) {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Base_Sinelon(bool dual, bool rainbow)
{

  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  fade_out(_segments[segment_active_index].intensity());
  uint16_t pos = beatsin16(_segments[segment_active_index].speed()/10,0,segment_length-1);
  if (_segment_runtimes[segment_active_index].call == 0) _segment_runtimes[segment_active_index].aux0 = pos;
  uint32_t color1 = color_from_palette(pos, true, false, 0);
  uint32_t color2 = SEGCOLOR(2);
  if (rainbow) {
    color1 = color_wheel((pos & 0x07) * 32);
  }
  SetPixelColor(pos, color1);
  if (dual) {
    if (!color2) color2 = color_from_palette(pos, true, false, 0);
    if (rainbow) color2 = color1; //rainbow
    SetPixelColor(segment_length-1-pos, color2);
  }
  if (_segment_runtimes[segment_active_index].aux0 != pos) { 
    if (_segment_runtimes[segment_active_index].aux0 < pos) {
      for (uint16_t i = _segment_runtimes[segment_active_index].aux0; i < pos ; i++) {
        SetPixelColor(i, color1);
        if (dual) SetPixelColor(segment_length-1-i, color2);
      }
    } else {
      for (uint16_t i = _segment_runtimes[segment_active_index].aux0; i > pos ; i--) {
        SetPixelColor(i, color1);
        if (dual) SetPixelColor(segment_length-1-i, color2);
      }
    }
    _segment_runtimes[segment_active_index].aux0 = pos;
  }

  // return FRAMETIME;  
  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed
  StripUpdate();
}

void mAnimatorLight::SubTask_Segment_Animation__Sinelon(void) {
  return SubTask_Segment_Animation__Base_Sinelon(false);
}

void mAnimatorLight::SubTask_Segment_Animation__Sinelon_Dual(void) {
  return SubTask_Segment_Animation__Base_Sinelon(true);
}

void mAnimatorLight::SubTask_Segment_Animation__Sinelon_Rainbow(void) {
  return SubTask_Segment_Animation__Base_Sinelon(true, true);
}



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Drip Effect
 * @note : Part of WLED Effects
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * // /*
//  * Drip Effect
//  * ported of: https://www.youtube.com/watch?v=sru2fXh4r7k
//  *


//values close to 100 produce 5Hz flicker, which looks very candle-y
//Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
//and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/

 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animation__Drip()
{
  uint8_t segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel   = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel    = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;


  //allocate segment data
  uint16_t numDrops = 4; 
  uint16_t dataSize = sizeof(spark) * numDrops;
  if (!_segment_runtimes[segment_active_index].allocateData(dataSize)) 
  {
    return;
   }
   // mode_static(); //allocation failed

  fill(SEGCOLOR(1));
  
  Spark* drops = reinterpret_cast<Spark*>(_segment_runtimes[segment_active_index].data);

  numDrops = 1 + (_segments[segment_active_index].intensity() >> 6);

  float gravity = -0.001 - (_segments[segment_active_index].speed()/50000.0);
  gravity *= segment_length;
  int sourcedrop = 12;

  for (int j=0;j<numDrops;j++) {
    if (drops[j].colIndex == 0) { //init
      drops[j].pos = segment_length-1;    // start at end
      drops[j].vel = 0;           // speed()
      drops[j].col = sourcedrop;  // brightness
      drops[j].colIndex = 1;      // drop state (0 init, 1 forming, 2 falling, 5 bouncing) 
    }
    
    SetPixelColor(segment_length-1,color_blend(BLACK,SEGCOLOR(0), sourcedrop));// water source
    if (drops[j].colIndex==1) {
      if (drops[j].col>255) drops[j].col=255;
      SetPixelColor(int(drops[j].pos),color_blend(BLACK,SEGCOLOR(0),drops[j].col));
      
      drops[j].col += map(_segments[segment_active_index].speed(), 0, 255, 1, 6); // swelling
      
      if (random8() < drops[j].col/10) {               // random drop
        drops[j].colIndex=2;               //fall
        drops[j].col=255;
      }
    }  
    if (drops[j].colIndex > 1) {           // falling
      if (drops[j].pos > 0) {              // fall until end of segment
        drops[j].pos += drops[j].vel;
        if (drops[j].pos < 0) drops[j].pos = 0;
        drops[j].vel += gravity;

        for (int i=1;i<7-drops[j].colIndex;i++) { // some minor math so we don't expand bouncing droplets
          SetPixelColor(int(drops[j].pos)+i,color_blend(BLACK,SEGCOLOR(0),drops[j].col/i)); //spread pixel with fade while falling
        }
        
        if (drops[j].colIndex > 2) {       // during bounce, some water is on the floor
          SetPixelColor(0,color_blend(SEGCOLOR(0),BLACK,drops[j].col));
        }
      } else {                             // we hit bottom
        if (drops[j].colIndex > 2) {       // already hit once, so back to forming
          drops[j].colIndex = 0;
          drops[j].col = sourcedrop;
          
        } else {

          if (drops[j].colIndex==2) {      // init bounce
            drops[j].vel = -drops[j].vel/4;// reverse velocity with damping 
            drops[j].pos += drops[j].vel;
          } 
          drops[j].col = sourcedrop*2;
          drops[j].colIndex = 5;           // bouncing
        }
      }
    }
  }
//   return FRAMETIME;  

  _segment_runtimes[segment_active_index].animation_has_anim_callback = false; // When no animation callback is needed

  StripUpdate();
}


    
    #endif //  ENABLE_EXTRA_WLED_EFFECTS



          #ifdef ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2   
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : SubTask_Flasher_Animate_Function__Static_Palette_Spanning_Segment
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Flasher_Animate_Function__Static_Palette_Spanning_Segment()
{


 mPaletteI->currentPalette = Test_p;
 pCONT_lAni->_virtualSegmentLength = 50;
 pCONT_lAni->paletteBlend = 3;
 
uint8_t index = 15;

uint8_t paletteIndex = 0; 

CRGB colour;
      uint8_t pbri = 255;

 uint16_t j = 0;
 uint16_t i = 0;
    pCONT_lAni->SetPixelColor(j, pCONT_lAni->color_from_palette(i, true, false, 10));
  for(uint16_t i = 0; i < 50; i++) {
    
    j = i;//map(i,0,50,0,255);
  //  pCONT_lAni->SetPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
    // pCONT_lAni->SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  index = i;
  
paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);

uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


    pCONT_lAni->SetPixelColor(i, col);
  
  }
    // pCONT_lAni->SetPixelColor(0, RgbColor(255,0,0));
  pCONT_lAni->_segment_runtimes[0].animation_has_anim_callback = false; // When no animation callback is needed
  pCONT_lAni->StripUpdate();

// CRGB colour;
//  = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);
// AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);







  // uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  // //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  // if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
  //   _segments[segment_active_index].mode_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
  // }
  
  // // this should probably force order as random, then introduce static "inorder" option
  // _segments[segment_active_index].transition.order_id = TRANSITION_ORDER_INORDER_ID;  
  // // So colour region does not need to change each loop to prevent colour crushing
  // _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // // Pick new colours
  // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // // Check if output multiplying has been set, if so, change desiredcolour array
  // // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // // Get starting positions already on show
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // // Call the animator to blend from previous to new
  // setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
  //   [this](const AnimationParam& param){ 
  //     this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
  //   }
  // );



}


          #endif // ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2   


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Test case used for developing new animations
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester()
{


 mPaletteI->currentPalette = Test_p;
 pCONT_lAni->_virtualSegmentLength = 50;
 pCONT_lAni->paletteBlend = 3;
 
uint8_t index = 15;

uint8_t paletteIndex = 0; 

CRGB colour;
      uint8_t pbri = 255;

 uint16_t j = 0;
 uint16_t i = 0;
    pCONT_lAni->SetPixelColor(j, pCONT_lAni->color_from_palette(i, true, false, 10));
  for(uint16_t i = 0; i < 50; i++) {
    
    j = i;//map(i,0,50,0,255);
  //  pCONT_lAni->SetPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
    // pCONT_lAni->SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  index = i;
  
paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
colour = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);

uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


    pCONT_lAni->SetPixelColor(i, col);
  
  }
    // pCONT_lAni->SetPixelColor(0, RgbColor(255,0,0));
  pCONT_lAni->_segment_runtimes[0].animation_has_anim_callback = false; // When no animation callback is needed
  pCONT_lAni->StripUpdate();

// CRGB colour;
//  = ColorFromPalette( Test_p, paletteIndex, pbri, NOBLEND);
// AddLog(LOG_LEVEL_TEST, PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);







  // uint16_t dataSize = GetSizeOfPixel(_segments[segment_active_index].colour_type) * 2 * _segments[segment_active_index].length(); //allocate space for 10 test pixels

  // //AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  // if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  // {
  //   AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
  //   _segments[segment_active_index].mode_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
  // }
  
  // // this should probably force order as random, then introduce static "inorder" option
  // _segments[segment_active_index].transition.order_id = TRANSITION_ORDER_INORDER_ID;  
  // // So colour region does not need to change each loop to prevent colour crushing
  // _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
  // // Pick new colours
  // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
  // // Check if output multiplying has been set, if so, change desiredcolour array
  // // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
  // // Get starting positions already on show
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // // Call the animator to blend from previous to new
  // setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
  //   [this](const AnimationParam& param){ 
  //     this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
  //   }
  // );



}




#ifdef ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Slow Glow
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Static_On()
{

  uint16_t dataSize = sizeof(notification_data);

  AddLog(LOG_LEVEL_TEST, PSTR("dataSize = %d"), dataSize);

  if (!_segment_runtimes[segment_active_index].allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    AddLog(LOG_LEVEL_TEST, PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    return;
  }

  uint8_t  segment_index  = segment_active_index;
  uint16_t segment_length = _segments[segment_active_index].length();
  uint16_t start_pixel    = _segments[segment_active_index].pixel_range.start;
  uint16_t stop_pixel     = _segments[segment_active_index].pixel_range.stop;
  _virtualSegmentLength = segment_length;

  NOTIFICATIONS_DATA* notif_data = reinterpret_cast<NOTIFICATIONS_DATA*>(_segment_runtimes[segment_active_index].data);

  RgbcctColor target_colour = notif_data->colour;
  RgbcctColor set_colour = notif_data->colour;

  float progress = mSupport::mapfloat(notif_data->progress, 0,255, 0.0f,1.0f);


  target_colour = RgbcctColor(0,0,notif_data->progress,0,0);


  set_colour = RgbcctColor::LinearBlend(RgbcctColor(255,0,0,0,0), target_colour, progress);



  uint8_t j = notif_data->progress++;
  
  AddLog(LOG_LEVEL_TEST, PSTR("notif_data->progress = %d"), notif_data->progress);

  for(uint16_t index = start_pixel; index<stop_pixel; index++)
  {  
    SetPixelColor(index, set_colour);
  }  

//   // // this should probably force order as random, then introduce static "inorder" option
//   // _segments[segment_active_index].transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
//   // // So colour region does not need to change each loop to prevent colour crushing
//   // _segments[segment_active_index].flags.brightness_applied_during_colour_generation = true;
  
//   // // Pick new colours
//   // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(_segments[segment_active_index].palette.id, segment_active_index);
//   // // Check if output multiplying has been set, if so, change desiredcolour array
//   // // OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
//   // // Get starting positions already on show
//   // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

//   // // Call the animator to blend from previous to new
//   // setAnimFunctionCallback_Segments_Indexed(  segment_active_index, 
//   //   [this](const AnimationParam& param){ 
//   //     this->AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(param); 
//   //   }
//   // );

  StripUpdate();

}


void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Static_Off(){};
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Fade_On(){};
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Fade_Off(){};
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Blinking(){};
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Pulsing(){};







#endif // ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS



#endif //USE_MODULE_LIGHTS_ANIMATOR



