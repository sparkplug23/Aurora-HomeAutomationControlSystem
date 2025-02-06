/**
 * @file mAnimatorLight_Segments_Effects.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

// control HTML elements for Slider and Color Control (original ported form WLED-SR)
// Technical notes
// ===============
// If an effect name is followed by an @, slider and color control is effective.
// If not effective then:
//      - For AC effects (id<128) 2 sliders and 3 colors and the palette will be shown
//      - For SR effects (id>128) 5 sliders and 3 colors and the palette will be shown
// If effective (@)
//      - a ; seperates slider controls (left) from color controls (middle) and palette control (right)
//      - if left, middle or right is empty no controls are shown
//      - a , seperates slider controls (max 5, then 3) or color controls via checkboxes(max 3). Palette has only one value
//      - a ! means that the default is used.
//             - For sliders: Effect speeds, Effect intensity, Custom 1, Custom 2, Custom 3
//             - For colors: Fx color, Background color, Custom
//             - For palette: prompt for color palette OR palette ID if numeric (will hide palette selection)
//
// Note: If palette is on and no colors are specified 1,2 and 3 is shown in each color circle.
//       If a color is specified, the 1,2 or 3 is replaced by that specification.
// Note: Effects can override default pattern behaviour
//       - FadeToBlack can override the background setting
//       - Defining SEGCOL(<i>) can override a specific palette using these values (e.g. Color Gradient)
//////////////,,,,,,,!,!
/***************************************************************************************************************************************************
static const char PM_EFFECT_CONFIG__##[] PROGMEM = 
  "Effect Name@Speed,Intensity,Custom1Star,Custom2Cog,Custom3Visibility,Checkbox1Palette,Checkbox2Layers,Checkbox3Favourite,EffectPeriod,Grouping;SEGCOL_Fx,SEGCOL_Bg,SEGCOL_Col3,SEGCOL_Col4,SEGCOL_Col5;pal=21,ep=1000~DEBUG_LEVEL";

////////////////////////////////////////////////////////////////////////////////////////////////////////
Effect Name
@  //////////////////////////////////////// Delimiter is "@", 5 sliders + 3 checkboxes + 2 sliders
Speed,                Slider1
Intensity,            Slider2
Custom1Star,          Slider3
Custom2Cog,           Slider4
Custom3Visibility,    Slider5
Checkbox1Palette,     Checkbox1
Checkbox2Layers,      Checkbox2
Checkbox3Favourite,   Checkbox3
EffectPeriod,         Slider6
Grouping              Slider7
; ////////////////////////////////////////// Delimiter is ";", Section 1: Five segment colour names below
SEGCOL_Fx,
SEGCOL_Bg,
SEGCOL_Col3,
SEGCOL_Col4,
SEGCOL_Col5
; ////////////////////////////////////////// Delimiter is ";", Section 2: ??
name for the palette icon, "!" means use default name and include palette selection
; ////////////////////////////////////////// Delimiter is ";", Section 3 below is 012vf that adds icons into effect name
012vf 0=0D ie Solid, 1=1D ie strip, 2=2D ie matrix, v=volume, f=frequency
; ////////////////////////////////////////// Delimiter is ";", Section 4: special commands below that are xml keys=value pairs, with comma delimiter
pal=21,
ep=1000
~ ////////////////////////////////////////// Delimiter is "~" for debug level below, single number/digit
DEBUG_LEVEL
////////////////////////////////////////////////////////////////////////////////////////////////////////

***************************************************************************************************************************************************/


/***************************************************************************************************************************************************
List of special commands that can load a default setting.
pal=0  | palette
ix=127 | intensity
sx=127 | speed

// m12=0
// pal=10
// 01
// ix=16
// o2
// sx=64
// 12

***************************************************************************************************************************************************/


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : Solid Colour
 * @description:   : For full RGBCCT colour
 * 
 * Keeping both U32/RGBWW modes here, as its a useful demonstration of both coldata methods
 * The lower level wrapper will handle the colour conversion 
 * 
 * @param Intensity: None
 * @param Speed    : None
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Solid_Colour() 
{
 
  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2) ){ DEBUG_LINE_HERE; return USE_ANIMATOR; } // Pixel_Width * Two_Channels

Serial.println(SEGMENT.colour_width__used_in_effect_generate);
  // Retrieve the desired color from the palette
  if (SEGMENT.colour_width__used_in_effect_generate == ColourType::COLOUR_TYPE__RGBWW__ID) {
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
    RgbwwColor desiredColour = SEGMENT.GetPaletteColour_Rgbww();
    RgbwwColor startingColour = SEGMENT.getPixelColorRgbww(0);

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    // desiredColour = RgbwwColor(1,2,3,4,5);
    // startingColour = RgbwwColor(6,7,8,9,10);
    Serial.printf("Solid Colour --------------------------------------------------%d\n\r", desiredColour);
    #endif
    
    // Set the desired and starting colors in the transition buffer
    SEGMENT.Set_DynamicBuffer_DesiredColour_RgbwwColor(0, desiredColour);
    SEGMENT.Set_DynamicBuffer_StartingColour_RgbwwColor(0, startingColour);
    #endif

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    // Serial.printf("Solid Colour RGBWW %d,%d,%d\n\r", desiredColour.R, desiredColour.G, desiredColour.B); 
    AddLog_Array_Block(3, PSTR("Data()"), SEGMENT.Data(), SEGMENT.DataLength(), 5, true);
    #endif
  } else {
    // Handle RGB/WRGB cases
    uint32_t desiredColour  = SEGMENT.GetPaletteColour(0, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE, ANIM_BRIGHTNESS_REQUIRED);
    uint32_t startingColour = SEGMENT.getPixelColor(0);

    // SERIAL_DEBUG_COL32i("des", desiredColour, 0);
    // SERIAL_DEBUG_COL32i("sta", startingColour, 0);

    // Set the desired and starting colors in the transition buffer
    SEGMENT.Set_DynamicBuffer_DesiredColour(0, desiredColour);
    SEGMENT.Set_DynamicBuffer_StartingColour(0, startingColour);
    // AddLog_Array_Block(3, PSTR("ColourData()"), SEGMENT.ColourData(), SEGMENT.ColourDataLength(), 6, true);
    // AddLog_Array_Block(3, PSTR("Data()"), SEGMENT.ColourData(), 24, 6, true);
  }

  // Set up the animation function callback
  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
    #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32_FillSegment_BrightnessAlreadySet(param);
      #else
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32_FillSegment(param);
      #endif
  });

  return USE_ANIMATOR;

}
// Enable the colour palette, with default segcol "Colour 01", with 1 second refresh and no blend for instant colour change and low memory usage
static const char PM_EFFECT_CONFIG__SOLID_COLOUR[] PROGMEM = "Solid Colour@;;!;pal=0,sx=255,ep=1000"; //"NAME@Speed,Intensity,Custom1,C2,C3,Checkbox1,CB2,CB3,EffectPeriod,Grouping;"


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : Static Palette
 * @description:   : Palettes should be showed all as banded/descrete, regardless of type
 * 
 * This version will be limited to U32, another will be made called "Static CCT Palette" which will work on all 5 pixels and Rgbww methods
 * 
 * Version should be made that has no blend, and hence, no dynamic buffer (ie, just set the colour)
 * 
 * Multiple Versions of this may need to exist
 * _U32_Blend: U32 optimised with blending enabled (desired/starting pixels, uses memory)
 * _Rgbww_Blend: Rgbww enabled for ws2805 and full rgbww palettes (performance will be lower)
 * _U32_Instant: No buffer, uses much less memory and therefore most efficient. This can be a version of the above, if the speed is 0 ie instant
 * 
 *
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Static_Palette()
{
  // Serial.printf("EffectAnim__Static_Palette A %d\n\r", SEGLEN);

  /**
   * @brief When speed is not instantaneous (max 255), then it should blend based on that speed
   **/
  if(SEGMENT.speed==255)
  {

    uint32_t colour;
    for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
    {
      colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
      SEGMENT.setPixelColor(pixel, colour);
    }

    // SERIAL_DEBUG_COL32i("last", colour, 0);

    return FRAMETIME;
  }
  /**
   * @brief Speed will be used to calculate the refresh/cycle time
   **/
  else
  {

    if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return USE_ANIMATOR; } // Pixel_Width * Two_Channels * Pixel_Count

    SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();
    uint32_t colour;
    for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
    {
      colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE, ANIM_BRIGHTNESS_REQUIRED);
      SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour);
      #ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_COLOURS    
      Serial.printf("Static Colour --------------------------------------------------%d,%d,%d,%d\n\r", R(colour), G(colour), B(colour), W(colour));
      #endif
    }

    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32_BrightnessAlreadySet(param);
      #else
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
      #endif
    });

    return USE_ANIMATOR;

  }

}
static const char PM_EFFECT_CONFIG__STATIC_PALETTE[] PROGMEM = "Static@1,2,3,4,5,6,7,8,Period,Grouping;;name of palette;1;sx=255,ep=1000,paln=Snowy 02";

/**
 * @brief For low memory situations, call this function as default but direct method only (so maybe another method should be picked that does not use the animator??)
 * 
 */
uint16_t mAnimatorLight::EffectAnim__Static_Palette__NoBlend()
{
  ALOG_ERR(PSTR("I should not be here with exception of the case that picks it internally"));
  SEGMENT.speed = 0; // instantaneous, so invoke no memory for blending
  return EffectAnim__Static_Palette();
}
// No Effect Config as its for internal call only


/*******************************************************************************************************************************************************************************************************************
 * @description : Static palette, with variation in the colour based on the intensity. The aim is to replicate vintage lights that have aged due to the heat of the bulb. We may also need a vintage red shift.
 * @note : 
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Static_Palette_Varied()
{
  if (SEGMENT.effect_anim_section == 0)
  {
    uint8_t variance = SEGMENT.intensity;
    uint8_t r, g, b, w;

    uint32_t colour;
    for (uint16_t pixel = 0; pixel < SEGLEN; pixel++)
    {
      colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE, ANIM_BRIGHTNESS_REQUIRED);

      // Use int16_t for intermediate calculations to prevent overflow
      int16_t r_varied = R(colour) + random(-variance, variance);
      int16_t g_varied = G(colour) + random(-variance, variance);
      int16_t b_varied = B(colour) + random(-variance, variance);
      int16_t w_varied = W(colour) + random(-variance, variance);

      // Constrain the values to 0-255
      r = constrain(r_varied, 0, 255);
      g = constrain(g_varied, 0, 255);
      b = constrain(b_varied, 0, 255);
      w = constrain(w_varied, 0, 255);

      // Set the pixel colour
      SEGMENT.setPixelColor(pixel, RGBW32(r, g, b, w));
    }

    SEGMENT.effect_anim_section = 1; // To stop redraw unless effect is reset
  }

  return FRAMETIME_STATIC;
}

static const char PM_EFFECT_CONFIG__STATIC_PALETTE_VARIED[] PROGMEM = "Colours Aged@,Colour Variance;ix=10";

/*******************************************************************************************************************************************************************************************************************
 * @description : This will span the palette across the segment, but will not blend the colours, so a 5 colour palette will become AAABBBCCCDDDEEE with grouped/repeated colours
 * @note : all palettes should be got in descrete mode, so that they are not blended, included normally blended palettes (eg Rainbow 16 should be 16 discrete colours)
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Split_Palette_SegWidth()
{
  // Allocate colour data for dynamic buffer
  if (!SEGMENT.allocateColourData(SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN)) {
    return USE_ANIMATOR;
  }

  // Retrieve the current dynamic buffer state
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Calculate the reverse offset based on intensity
  uint16_t offset_shift = map(SEGMENT.intensity, 255, 0, 0, SEGLEN);

  uint32_t colour = 0;
  for (uint16_t pixel = 0; pixel < SEGLEN; pixel++) {
    // Adjust the pixel index with the offset and wrap around using % SEGLEN
    uint16_t adjusted_index = (pixel + offset_shift) % SEGLEN;
    // Retrieve the palette colour for the adjusted index
    colour = SEGMENT.GetPaletteColour(adjusted_index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE, ANIM_BRIGHTNESS_REQUIRED);
    // Set the desired colour in the dynamic buffer
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour);
  }

  // Set the animation function callback for linear blending
  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
    #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
    SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32_BrightnessAlreadySet(param);
    #else
    SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    #endif
  });

  return USE_ANIMATOR;
}
static const char PM_EFFECT_CONFIG__SPLIT_PALETTE_SEGWIDTH[] PROGMEM = "Bands@,Offset;!,!,!,!,!;ix=0,ep=1000,paln=RGBY";


/*******************************************************************************************************************************************************************************************************************
 * @description : All palettes are forced to create static gradient palettes. If palette was discrete, it will be spanned across the segment with linear blending
 * @note : related to split_palette_segwidth, but it uses grouping without linear blending
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Gradient_Palette_SegWidth()
{
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength();

  ALOG_ERR( "delete, replaced by spanned palette" );
  
  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return USE_ANIMATOR;
  }
  
  uint16_t start_pixel = 0;
  uint16_t end_pixel = 100;
  uint32_t start_colour = 0;//RgbcctColor();
  uint32_t end_colour = 0;//RgbcctColor();
  uint32_t out_colour = 0;//RgbcctColor();
  uint8_t start_pixel_position = 255, end_pixel_position = 255;

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);


  /**
   * @brief Move across by pixels in map, and then either use encoded value or just use pixel in map 
   * 
   */
    for(uint8_t grad_pair_index=0;
                grad_pair_index<pixels_in_map;
                grad_pair_index++
    ){

      uint8_t desired_index_upper = 0;
      if(grad_pair_index<pixels_in_map-1) // ie not last one
      {
        desired_index_upper = grad_pair_index+1; // use next
      }else{
        desired_index_upper = 0; //assume its the first and wrap back
      }
      
      // start_colour = mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, grad_pair_index,   &start_pixel_position);      
      start_colour = SEGMENT.GetPaletteColour(grad_pair_index, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &start_pixel_position);      

      // end_colour   = mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, desired_index_upper, &end_pixel_position);
      end_colour = SEGMENT.GetPaletteColour(desired_index_upper, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &end_pixel_position);

      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "grad_pair_index %d|%d  %d|%d"),grad_pair_index,pixels_in_map, grad_pair_index,desired_index_upper);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%02d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "     end_pixel_position %d"),end_pixel_position);

      /**
       * @brief No start/stop mapping with segments I think
       *  */
      uint16_t start_pixel = 0;
      uint16_t stop_pixel  = SEGLEN;

      mPalette::PALETTE_DATA* ptr = &mPaletteI->static_palettes[SEGMENT.palette_id]  ;
      
      // mPalette:PALETTE_DATA* ptr = &mPaletteI->static_palettes[SEGMENT.palette_id];
      
      if(ptr->encoding.index_gradient)
      {
        start_pixel_position = map(start_pixel_position, 0,255, start_pixel,stop_pixel);
        end_pixel_position   = map(end_pixel_position,   0,255, start_pixel,stop_pixel);
      }
      else
      { //
        start_pixel_position = map(grad_pair_index, 0,pixels_in_map, start_pixel,stop_pixel);
        end_pixel_position   = map(grad_pair_index+1, 0,pixels_in_map, start_pixel,stop_pixel);
      }

      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "%d start_pixel_position %d"),grad_pair_index,start_pixel_position);
      // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "end_pixel_position %d"),end_pixel_position);


      uint8_t progress = 0;
      for(
        int 
        index=start_pixel_position;
        index<=end_pixel_position;
        index++){
        
          // progress = mSupport::mapfloat(index,start_pixel_position,end_pixel_position,0,1);          
          // out_colour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);

          
          progress = map(index,start_pixel_position,end_pixel_position,0,255);   
          out_colour = ColourBlend(out_colour, out_colour, progress);
     
          // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), index, SEGMENT.colour_width__used_in_effect_generate, out_colour.WithBrightness(brightness) );
          
          SEGMENT.Set_DynamicBuffer_DesiredColour(index, out_colour);
      }

  }

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });

    return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__GRADIENT_PALETTE_SEGWIDTH[] PROGMEM = "Gradient@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;

/**
 * @description:   : Enables easier config of random spanned palettes, or, optionally will change between palettes
 **/
uint16_t mAnimatorLight::EffectAnim__Randomise_Gradient_Palette_SegWidth()
{

  uint16_t palette_id = SEGMENT.palette_id;
  if(!IsWithinLimitsInclusive(
    mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID, 
    palette_id, 
    mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID))
  {
    ALOG_INF(PSTR("restoring to default")); // though I dont want this when running without iterative mode, so consider that
    SEGMENT.palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID;
  }

  
  uint32_t new_colour_rate_ms = 1000 + (((uint32_t)(255-SEGMENT.intensity))*100);


  /***
    Intensity ranges from 0 to 255.
    The term (255 - intensity) ranges from 255 (when intensity = 0) to 0 (when intensity = 255).
    The multiplication ((255 - intensity) * 100) ranges from 255 * 100 = 25500 ms to 0 * 100 = 0 ms.
    Adding 1000 gives the final range for new_colour_rate_ms:
      Minimum: 1000 + 0 = 1000 ms (1 second).
      Maximum: 1000 + 25500 = 26500 ms (26.5 seconds).
    Therefore, this needs to ensure that the update rate, cycle time, is at least this amount
   */
  if(new_colour_rate_ms > SEGMENT.cycle_time__rate_ms)
  {
    ALOG_INF(PSTR("new_colour_rate_ms > SEGMENT.cycle_time__rate_ms %d > %d"), new_colour_rate_ms, SEGMENT.cycle_time__rate_ms);
    SEGMENT.cycle_time__rate_ms = new_colour_rate_ms + 100;
  }



  /*
  First option, would be to limit between those 5 randomise gradients
  also to ensure that the random period is larger than the cycle period


  Second option,
  This function could be used to iterate over palettes by index, hence is random change would actually be to change palette.
  This could be done via presets, but perhaps nicer as an effect.A0

  To make this work, I should consider a new param01,2,3,4 paramARR to have a vector way of loading data or various length. 
  
  */

  return EffectAnim__Split_Palette_SegWidth();  // this should probably actually use gradient palette 
}
static const char PM_EFFECT_CONFIG__RANDOMISE_GRADIENT_PALETTE_SEGWIDTH[] PROGMEM = "Randomise Gradient@!,!;!,!;!;1;sx=96,ix=224,ep=23";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description            : Firefly
 * @description:    : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param Intensity : 0-255 is how many should pixels should randomly change (0-255 scaled to 0-pixel_count)
 * @param Speed     : None
 * @param CycleTime : Period of time (ms) between updates
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Firefly()
{
    // Allocate data buffer for starting and desired colors
    if (!SEGMENT.allocateColourData(SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength())) { return USE_ANIMATOR; } // Allocation failed

    // Compute how many pixels to update based on intensity
    uint16_t pixels_to_update = 0;
    if (SEGMENT.flags.animator_first_run && (SEGMENT.params_user[0]==0)) { 
      pixels_to_update = SEGMENT.virtualLength();  // update all on first run, if optional user flag has not disabled it (when it is desired this effect writes over an existing effect)
    }else{
      pixels_to_update = mapvalue(SEGMENT.intensity, 0, 255, 0, SEGMENT.virtualLength());
    }

    uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);
    DEBUG_LIGHTING__START_TIME_RECORDING(4);
    
    // Start time recording for updating starting colors
    DEBUG_LIGHTING__START_TIME_RECORDING(3);
    SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();
    DEBUG_LIGHTING__SAVE_TIME_RECORDING(3, lighting_time_critical_logging.dynamic_buffer__starting_colour);


    // Precompute all random values for pixel indices and palette indices
    // This may cause crashing with excess memory, so find another way perhaps. do directly?
    uint16_t random_pixel_indices[pixels_to_update];
    uint16_t random_palette_indices[pixels_to_update];

    for (uint16_t i = 0; i < pixels_to_update; i++) {
        random_pixel_indices[i] = (SEGMENT.flags.animator_first_run) ? i : random(0, SEGMENT.virtualLength());
        random_palette_indices[i] = random(0, pixels_in_map);
    }

    // uint32_t test_col = 0;

    // Update all pixels with the new desired colors
    for (uint16_t iter = 0; iter < pixels_to_update; iter++) {
      uint16_t pixel_index   = random_pixel_indices[iter];
      uint16_t desired_pixel = random_palette_indices[iter];

      // Get the WRGB from palette
      uint32_t colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE, ANIM_BRIGHTNESS_REQUIRED);

      // test_col = RGBW32(SEGMENT.custom1,0,0,0);

      SEGMENT.Set_DynamicBuffer_DesiredColour(pixel_index, colour);
    }

    DEBUG_LIGHTING__SAVE_TIME_RECORDING(4, lighting_time_critical_logging.dynamic_buffer__desired_colour);

    
    // uint8_t* segmentData = SEGMENT.Data();
    // uint16_t segmentDataLength = SEGMENT.DataLength();
    // Serial.printf("seg: \n\r");
    // for(int i = 0; i < 40; i++)
    // {
    //   Serial.printf("%d,", segmentData[i]);
    // }
    // Serial.println();

    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32_BrightnessAlreadySet(param);
      #else
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
      #endif
    });

    return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__FIREFLY[] PROGMEM = "FireFly@Blend Speed,Pixels Changing,,,,,,,!;;!;1;ep=5000,ix=50";


/*******************************************************************************************************************************************************************************************************************
 * @description :Base function for flickering
 *        Modified version from WLED to enable other secondary colours or palettes
          values close to 100 produce 5Hz flicker, which looks very candle-y
          Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
          and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/
          Candles should remain RGBWW capable so I can have true candle flickers
 * @note : Converted from WLED Effects "candle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Flicker_Base(bool use_multi, uint16_t flicker_palette_id)
{
  uint8_t pixels_in_primary_palette = GetNumberOfColoursInPalette(SEGMENT.palette_id);
  uint8_t pixels_in_secondary_palette = GetNumberOfColoursInPalette(flicker_palette_id);

  ColourBaseType colour_pri = RGBW32(255,0,0,0);
  ColourBaseType colour_sec = RGBW32(0,255,0,0);
  ColourBaseType colour_out = RGBW32(0,0,255,0);

  bool single_secondary_color = (pixels_in_secondary_palette == 1);

  if (use_multi)
  {
    uint16_t dataSize = (SEGLEN - 1) * 3;
    if (!SEGMENT.allocateData(dataSize)) { return EFFECT_DEFAULT(); }
  }

  // Load the single secondary color once if the palette contains only one color
  if (single_secondary_color)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
    colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, 0, true, true, true);
    #else
    RgbwwColor c = GetColourFromUnloadedPalette3(flicker_palette_id, 0, true, true, true);
    colour_sec = RGBW32(c.R, c.G, c.B, c.WW);
    #endif
  }

  // max. flicker range controlled by intensity
  uint8_t valrange = SEGMENT.intensity;
  uint8_t rndval = valrange >> 1; // divide by 2

  #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
  ALOG_INF(PSTR("step=%d"), SEGMENT.step);
  ALOG_INF(PSTR("valrange=%d"), valrange);
  ALOG_INF(PSTR("rndval=%d"), rndval);
  #endif

  uint8_t pixel_palette_counter = 0;

  // step (how much to move closer to target per frame) coarsely set by speed
  uint8_t speedFactor = 4;
  if (SEGMENT.speed > 252) { // epilepsy
    speedFactor = 1;
  }
  else if (SEGMENT.speed > 99) { // regular candle
    speedFactor = 2;
  }
  else if (SEGMENT.speed > 49) { // slower fade
    speedFactor = 3;
  }

  uint16_t numCandles = (use_multi) ? SEGLEN : 1;

  #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
  ALOG_INF(PSTR("numCandles=%d"), numCandles);
  #endif

  for (uint16_t i = 0; i < numCandles; i++)
  {
    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBG(PSTR("i=%d|%d"), i, numCandles);
    #endif

    uint16_t d = 0; // data location

    uint8_t s = SEGMENT.aux0, 
            s_target = SEGMENT.aux1, 
            fadeStep = SEGMENT.step;

    if (i > 0) {
      d = (i - 1) * 3;
      s = SEGMENT.data[d];
      s_target = SEGMENT.data[d + 1];
      fadeStep = SEGMENT.data[d + 2];
    }
    if (fadeStep == 0) { // init vals
      s = 128; s_target = 130 + random8(4); fadeStep = 1;
    }

    bool newTarget = false;
    if (s_target > s) { // fade up
      s = qadd8(s, fadeStep);
      if (s >= s_target) newTarget = true;
    }
    else { // fade down
      s = qsub8(s, fadeStep);
      if (s <= s_target) newTarget = true;
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

    /**
     * Apply colour to output: different per pixel
     **/
    if (i > 0) 
    {
      // Get primary color
      #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
      colour_pri = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);
      #else
      // RgbwwColor c = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true); 
      // colour_pri = RGBW32(c.R, c.G, c.B, c.WW);
      colour_pri = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true); 
      #endif

      #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
      if (!single_secondary_color) {
          colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, i, true, true, true);
      }
      #else
      if (!single_secondary_color) {
        RgbwwColor c2 = GetColourFromUnloadedPalette3(flicker_palette_id, i, true, true, true);
        colour_sec = RGBW32(c2.R, c2.G, c2.B, c2.WW);
        // colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, i, true, true, true);
      }
      #endif

      // Blend primary and secondary color based on flicker intensity
      colour_out = ColourBlend(colour_pri, colour_sec, s);

      SEGMENT.setPixelColor(i, colour_out);

      SEGMENT.data[d] = s;
      SEGMENT.data[d + 1] = s_target;
      SEGMENT.data[d + 2] = fadeStep;
    }
    else
    {
      for (uint16_t p = 0; p < SEGLEN; p++)
      {
        // Get primary color for each pixel
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        colour_pri = SEGMENT.GetPaletteColour(p, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);
        #else
        // RgbwwColor c = SEGMENT.GetPaletteColour(p, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);
        // colour_pri = RGBW32(c.R, c.G, c.B, c.WW);
        colour_pri = SEGMENT.GetPaletteColour(p, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);
        #endif

        // If only one color in secondary palette, reuse it, otherwise get a new color for each pixel
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        if (!single_secondary_color) {
          colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, p, true, true, true);
        }
        #else
        if (!single_secondary_color) {
          RgbwwColor c2 = GetColourFromUnloadedPalette3(flicker_palette_id, p, true, true, true);
          colour_sec = RGBW32(c2.R, c2.G, c2.B, c2.WW);
        }
        #endif

        // Blend primary and secondary color based on flicker intensity
        colour_out = ColourBlend(colour_pri, colour_sec, s);

        SEGMENT.setPixelColor(p, colour_out);
      }

      SEGMENT.aux0 = s;
      SEGMENT.aux1 = s_target;
      SEGMENT.step = fadeStep;
    }
  }

  return FRAMETIME;
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Flickers pixels by the same amount towards black
 * @note : Converted from WLED Effects "mode_candle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Candle_Single()
{
  return EffectAnim__Flicker_Base(false, mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__CANDLE_SINGLE[] PROGMEM = "Candle@!,!;!,!;!;1;sx=96,ix=224,ep=23";


/*******************************************************************************************************************************************************************************************************************
 * @description : Flickers by multiple levels towards black
 * @note : Converted from WLED Effects "mode_candle_multi"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Candle_Multiple()
{
  return EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__CANDLE_MULTIPLE[] PROGMEM = "Candles@Speed,Intensity;!,!,!,!,!;sx=180,ix=224,paln=Warm White"; // 7 sliders + 4 options before first ;


/*******************************************************************************************************************************************************************************************************************
 * @description : Flickers by multiple levels towards white, showing a shimmer of saturation
 * @note : Modified from WLED candle_multi
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Shimmering_Palette_Saturation()
{
  return EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__COLDWHITE__ID);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_PALETTE_SATURATION[] PROGMEM = "Shimmering Saturation@Speed,Intensity;!,!,!,!,!;"; // 7 sliders + 4 options before first ;


/*******************************************************************************************************************************************************************************************************************
 * @description : Flicker between primary and secondary palette
 * 
           Desc: Animation, that fades from selected palette to anothor palette,
                 The intensity of fade (closer to palette B) will depend on intensity value
                 ie intensity of 255 means Palette A (primary) can fade into palette B (set by option)
           
           Using maps, the transition could have an option to be nonlinear, ie
           
           lower 10% and upper 10% are accurate
           10->90% is either capped, or squeezed near 10% and 90% marks. That way even with randomness, either palette 1 or 2 is mostly showed with less probably of "white out" in the middle 
           
 * @note : Modified from WLED candle_multi
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Shimmering_Two_Palette() // Also add another here (or really segcolour is also it) to flicker into a second palette!! this will require direct load of second palette
{
  return EffectAnim__Flicker_Base(true, SEGMENT.params_user[0]);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_TWO_PALETTES[] PROGMEM = "Shimmering Palettes@Speed,Intensity,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;


#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME



#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : Based on accidently effect
 * @description:   : Colours are added (with various methods) then fade off
 * @note           : 
 * 
 * Ideas:
 * - param to enable "instant" or popping in, or to have the introduction of new colours actually fade on before decaying away (hence, the fade direction may be needed? may be too complex)
 * - fade off method, to have all fade together (using intensity), intensity should be for how many to pop-in, speed for how quickly they fade away
 * - params could be used to signifying all fade together, or later, randomly pick fade rates. Though, fade together is probably visually nicer. Option would be good
 * 
 * 
 * 

Popping_Decay_Palette
  - "Static Palette" pops in, in order, but fades out 
Popping_Decay_Random
  - "Randomly add from palette" pops in, but fades out
Popping_Decay_Palette_Hue
  - "Static Palette" pops in, in order, but fades to white (make the white dimmer too so its more brightness levelled)
Popping_Decay_Random_Hue
  - "Randomly add from palette" pops in, but fades to white (make the white dimmer too so its more brightness levelled)




 * 
 * @param intensity: Depth of variation from max/min brightness
 * @param speed    : How often it occurs
 * @param rate     : None
 * @param time     : None
 * @param aux0     : brightness saved
 * @param aux1     : target brightness
 * @param aux2     : (U) Second target palette
 * @param aux3     : Reserved for random palette refresh rate
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
/**
 * @description:   : 
 **/
uint16_t mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_Black()
{
  return EffectAnim__Popping_Decay_Base(true, true);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_BLACK[] PROGMEM = "Pop Fade@,,,,,,,!,!;!,!,!,!,!;!";
/**
 * @description:   : 
 **/
uint16_t mAnimatorLight::EffectAnim__Popping_Decay_Random_To_Black()
{
  return EffectAnim__Popping_Decay_Base(false, true);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_BLACK[] PROGMEM = "Pop Fade Random@!,!,,,,,,,!;"; // SX,IX,,,,,,,EP,,
/**
 * @description:   : 
 **/
uint16_t mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_White()
{
  return EffectAnim__Popping_Decay_Base(true, false);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_WHITE[] PROGMEM = "Pop White@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
/**
 * @description:   : 
 **/
uint16_t mAnimatorLight::EffectAnim__Popping_Decay_Random_To_White()
{
  return EffectAnim__Popping_Decay_Base(false, false);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_WHITE[] PROGMEM = "Pop White Random@,,,,,Repeat Rate (ms);!,!,!,!,!;;";


/**
 * @description:   : Base function for pop in, fade out
 * */
uint16_t mAnimatorLight::EffectAnim__Popping_Decay_Base(bool draw_palette_inorder, bool fade_to_black)
{

  // Redo later: Popping will probably be the "new thing for 2023" on the outside tree. I want it to pop colours in, then fade them out.


  // options
  // pop_in:   instant
  // fade_out: fade rate (e.g. half brightness), depending on the intensity either randomly fade or fade them all.

  /**
   * @brief Step 1: Redraw buffer by preloading last state
   * 
   */


  /**
   * @brief Step 2: Pop_in random colours at full brightness
   * 
   */



  if (!SEGMENT.allocateData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength() )){ return USE_ANIMATOR; }
  
  /**
   * @brief Intensity describes the amount of pixels to update
   *  Intensity 0-255 ==> LED length 1 to length (since we cant have zero)
   **/
  uint16_t pixels_to_update = mapvalue(
                                        SEGMENT.intensity, 
                                        0,255, 
                                        0,SEGMENT.virtualLength() // scaled over the virtual length
                                      );

  uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

  uint16_t pixel_index = 0;
  uint32_t colour;

  /**
   * @brief On first run, all leds need set once
   * */
  if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
    pixels_to_update = SEGMENT.virtualLength(); // only the virtual length
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  for(unsigned iter = 0; 
                iter < pixels_to_update; 
                iter++
  ){
    /**
     * @brief 
     * min: lower bound of the random value, inclusive.
     * max: upper bound of the random value, exclusive.
    **/
    pixel_index = random(0, SEGMENT.virtualLength()+1); // Indexing must be relative to buffer, virtual length only

    // On first run, I need a new way to set them all.
    if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
      pixel_index = iter;
    }
    
    desired_pixel = random(0, pixels_in_map);
  
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);

    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBM(LOG_LEVEL_DEV_TEST, 
      PSTR("New palettePixel=%d, pixel_index=v%d SL%d | SVL%d | DL%d, colour=%d,%d,%dT%d"), 
        desired_pixel, pixel_index, 
        SEGMENT.length(), SEGMENT.virtualLength(), SEGMENT.DataLength(),
        colour.R, colour.G, colour.B,
        SEGMENT.colour_width__used_in_effect_generate
    ); 
    #endif // ENABLE__DEBUG_POINT__ANIMATION_EFFECTS

    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
    
    // colour = FadeU32(colour,brightness);

      SEGMENT.Set_DynamicBuffer_DesiredColour(pixel_index, colour);

  }



  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel_WithFade(2);
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment_WithFade(2);



  /**
   * @brief Step 3: Fade out colours, but make sure not to fade the new ones (hence step 2 and 3 should be randomly together, or too complex, just make sure pop_in is greater than fade_out)
   * 
   */






SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  // SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ 
  //   this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );




















//   ALOG_INF(PSTR("s=%d\t a0=%d\t a1=%d\t a2=%d"),    SEGMENT.step, SEGMENT.aux0, SEGMENT.aux1, SEGMENT.aux2);

//   uint8_t pixels_in_palette = GetNumberOfColoursInPalette(SEGMENT.palette_id);

//   RgbcctColor colour_pri;
//   RgbcctColor colour_sec;
//   RgbcctColor colour_out;

//   // if (draw_palette_inorder)
//   // {
//   //   uint16_t dataSize = (SEGLEN-1) *3;
//   //   if(!SEGMENT.allocateData(dataSize) ){ return FRAMETIME; }
//   // }

  
//   if (!SEGMENT.allocateData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength() )){ return FRAMETIME; } // Pixel_Width * Two_Channels * Pixel_Count
    
//   RgbcctColor colour = RgbcctColor();



//   //max. flicker range controlled by intensity
//   uint8_t valrange = SEGMENT.intensity;
//   uint8_t rndval = valrange >> 1;

//   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//   ALOG_DBG(PSTR("step=%d"),    SEGMENT.step);
//   ALOG_DBG(PSTR("valrange=%d"),valrange);
//   ALOG_DBG(PSTR("rndval=%d"),  rndval);
//   #endif

//   uint8_t pixel_palette_counter = 0;

//   //step (how much to move closer to target per frame) coarsely set by speed
//   uint8_t speedFactor = 4;

//     uint16_t i = 0;

//     #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//     ALOG_DBG(PSTR("i=%d|%d"),i,numCandles);
//     #endif

//     uint16_t d = 0; //data location

//     uint8_t s        = SEGMENT.aux0, 
//             s_target = SEGMENT.aux1, 
//             fadeStep = SEGMENT.step;

//     // if (i > 0) {
//     //   d = (i-1) *3;
//     //   s = SEGMENT.data[d]; 
//     //   s_target = SEGMENT.data[d+1]; 
//     //   fadeStep = SEGMENT.data[d+2];
//     // }

//     // if (fadeStep == 0) { //init vals
//     //   s = 128; s_target = 130 + random8(4); fadeStep = 1;
//     // }

//     // bool newTarget = false;
//     // if (s_target > s) { //fade up

//     //   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//     //   ALOG_DBG(PSTR("fade up s_target > s %d=%d"), s_target, s);
//     //   #endif

//     //   s = qadd8(s, fadeStep);
//     //   if (s >= s_target) newTarget = true;
//     // } else {
//     //   s = qsub8(s, fadeStep);
//     //   if (s <= s_target) newTarget = true;
          
//     //   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//     //   ALOG_DBG(PSTR("fade down=%d"),s);
//     //   #endif

//     // }

//     // if (newTarget) {
//     //   s_target = random8(rndval) + random8(rndval);
//     //   if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
//     //   uint8_t offset = (255 - valrange) >> 1;
//     //   s_target += offset;

//     //   uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
//     //   fadeStep = dif >> speedFactor;
//     //   if (fadeStep == 0) fadeStep = 1;
//     // }

//     /**
//      * @brief aux2
//      * 
//      */

//     bool redraw_all_pixels = false;
//     bool draw_random_pixels = false;
//     bool do_fade = false;

//     uint16_t* drawmode = &SEGMENT.aux2;
//     // uint16_t* drawmode = &SEGMENT.aux3;

// // Redraw and fade should happen on alternate cycles

// // Buffer mode not going to work, since I need this effect to be called more often than the fade amount
// // Another counter should therefore only update on certain intervals



//     /*
//     @ redraw when aux2 is zero
//     */
//     if(*drawmode == 0)
//     {
//       redraw_all_pixels = false;
//       do_fade = false;
//       *drawmode = *drawmode + 1;
//     }else
//     if(*drawmode>10) // Reset
//     {
//       *drawmode = 0;
//       do_fade = false;
//       *drawmode = *drawmode + 1;
//     }else{
//       // do_fade = true;
//       // draw_random_pixels = true;

//       if(*drawmode % 2)
//       {
//         do_fade = true;
//       }else{
//         draw_random_pixels = true;
//       }

//       *drawmode = *drawmode + 1;

//     }

//     /**
//      * @brief ReDraw all (either as option, or first run of animation)
//      * 
//      */
//     if(redraw_all_pixels)
//     {

//       ALOG_INF(PSTR("redraw_all_pixels"));

//       pixel_palette_counter = 0;      
//       for(uint16_t pixel = 0; pixel < SEGMENT.virtualLength(); pixel++)
//       {
//         if(draw_palette_inorder)
//         {
//           // Cycle through palette index pal_i
//           if(pixel_palette_counter++ >= pixels_in_palette-1){ pixel_palette_counter = 0; }
//         }
//         else
//         {
//           pixel_palette_counter = random(0, pixels_in_palette-1); // Randon colour from palette
//         }

//         colour = SEGMENT.GetPaletteColour(pixel_palette_counter);      
//         colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
//         SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour);
//       }

//     }


//     /**
//      * @brief ReDraw all (either as option, or first run of animation)
//      * 
//      * needs revamped, so that:
//      * ** picks random index in range, and uses that to either show the original (palette static) or selects random colour
//     * ** the intensity gives how many random colour should be updated in SEGLEN, 
//      */
//     if(draw_random_pixels)
//     {

//       ALOG_INF(PSTR("draw_random_pixels"));

//       uint16_t redraw_count = map(random(0,SEGMENT.intensity), 0,255, 0,SEGLEN);
//       uint16_t random_pixel_to_update = 0;

      
//       ALOG_INF(PSTR("redraw_count %d"), redraw_count);



//       pixel_palette_counter = 0;      
//       for(uint16_t pixel = 0; pixel < redraw_count; pixel++)
//       {
//         if(draw_palette_inorder)
//         {
//           pixel_palette_counter = pixel;   //pixel from palette
//           random_pixel_to_update = pixel; //pixel into output, these should be the same when in order
//         }
//         else
//         {
//           pixel_palette_counter = random(0, pixels_in_palette-1); // Randon colour from palette
//           random_pixel_to_update = map(random(0,SEGMENT.intensity), 0,255, 0,SEGLEN); //repick a random pixel to change within segment
//         }

//         colour = SEGMENT.GetPaletteColour(pixel_palette_counter);      
//         colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
//         SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel_to_update, SEGMENT.colour_width__used_in_effect_generate, colour);
//         do_fade = false ; // block fade on update cycle, since fade relies on getting from the commited neopixel bus
//       }

//     }

//     /**
//      * @brief Decay test
//      * 
//      */
//     if(do_fade)
//     {
      
//       ALOG_INF(PSTR("do_fade"));

//       for(uint16_t pixel = 0; pixel < SEGMENT.virtualLength(); pixel++)
//       {
//         colour_out = SEGMENT.GetPixelColor(pixel);
//         colour_out.Fade(2);// = RgbcctColor::ApplyBrightnesstoRgbcctColour(colour_out, pCONT_iLight->getBriRGB_Global());       
//         SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour_out);
//       }
//     }


//     SEGMENT.aux0 = s; 
//     SEGMENT.aux1 = s_target; 
//     SEGMENT.step = fadeStep;

  
//   SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

//   SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });



  // SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

  return USE_ANIMATOR;

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED /////////////////////////////////////////////////////////////////////////////////////////////




#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC /////////////////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************************************************************************************************************************************************
 * @description : Rotates by getting and setting pixel colours, with wrap around
 * @note : 
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Rotate_Base(uint16_t movement_amount, bool direction)
{
  if (movement_amount == 0) 
  {
    ALOG_ERR(PSTR("I should not be here"));
    return SPEED_FORMULA_L; // No rotation needed
  }

  // ALOG_INF(PSTR("EffectAnim__Rotate_Base(%d,%d)"), movement_amount, direction);
  // Save the edge colours to handle larger rotations
  std::vector<uint32_t> edgePixels(movement_amount);

  if (direction) // Forward (right) rotation
  {
    for (uint16_t step = 0; step < movement_amount; step++)
    {
      // Save the last pixel(s) being rotated out
      for (uint16_t j = 0; j < movement_amount; j++)
      {
        edgePixels[j] = SEGMENT.getPixelColor(SEGLEN - 1 - j);
      }

      // Shift pixels forward
      for (int16_t i = SEGLEN - 1; i >= movement_amount; i--)
      {
        SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i - movement_amount));
      }

      // Wrap the saved edge pixels to the start
      for (uint16_t j = 0; j < movement_amount; j++)
      {
        SEGMENT.setPixelColor(j, edgePixels[movement_amount - 1 - j]);
      }
    }
  }
  else // Backward (left) rotation
  {
    for (uint16_t step = 0; step < movement_amount; step++)
    {
      // Save the first pixel(s) being rotated out
      for (uint16_t j = 0; j < movement_amount; j++)
      {
        edgePixels[j] = SEGMENT.getPixelColor(j);
      }

      // Shift pixels backward
      for (uint16_t i = 0; i < SEGLEN - movement_amount; i++)
      {
        SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i + movement_amount));
      }

      // Wrap the saved edge pixels to the end
      for (uint16_t j = 0; j < movement_amount; j++)
      {
        SEGMENT.setPixelColor(SEGLEN - movement_amount + j, edgePixels[j]);
      }
    }
  }

  return FRAMETIME_WITH_SPEED(5,1000); // from 5ms to 1000ms per frame
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Rotates the currently displayed colours
 * @note : 
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Rotating_Previous_Animation()
{
  uint16_t pixels_to_move = map(SEGMENT.intensity, 0,255, 1,SEGLEN/2); // maxmimum of half the segment length (which would equate to toggling effect)
  bool direction          = !SEGMENT.check3; // true for right, false for left
  return EffectAnim__Rotate_Base(pixels_to_move, direction);
}
static const char PM_EFFECT_CONFIG__ROTATING_PREVIOUS_ANIMATION[] PROGMEM = "Rotate Previous@!,Shift,,,,,,Reverse;;!;!;ix=0,sx=240";


/*******************************************************************************************************************************************************************************************************************
 * @description : Will rotate a palette, redrawing the palette on each wrap stop errors/loss of colour
 * @note : Combined with random/live palettes, this will enable new palettes at the end of every rotation
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Rotating_Palette()
{
  uint16_t pixels_to_move = map(SEGMENT.intensity, 0,255, 1,SEGLEN/2); // maxmimum of half the segment length (which would equate to toggling effect)
  bool direction          = !SEGMENT.check3; // true for right, false for left

  // ALOG_INF(PSTR("call %d"), SEGMENT.call);

  // Repaint the palette when SEGMENT.call == 0 (before the first rotation loop)
  if (SEGMENT.call <= 1) // It is reset to 0 below, but effect caller will already incrememnt it by 1 on recall
  {
    ALOG_INF(PSTR("Repainting palette %d"), SEGMENT.call);
    uint32_t colour;
    for (uint16_t pixel = 0; pixel < SEGLEN; pixel++)
    {
      // Get palette colour for the current pixel
      colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
      // Set pixel colour
      SEGMENT.setPixelColor(pixel, colour);
    }

    // SEGMENT.setPixelColor(0,RGBW32(255,255,255,255)); // Debug pixel, to show the start of the segment
  }

  // Call the base rotation function
  uint16_t frametime = EffectAnim__Rotate_Base(pixels_to_move, direction);

  if(pixels_to_move > 1){ SEGMENT.call += (pixels_to_move - 1); } // Increment the call counter by the movement amount (excluding the already incrementing call)
  if (SEGMENT.call >= SEGLEN){ SEGMENT.call = 0;  }               // Reset SEGMENT.call after a full rotation

  return frametime;
}
static const char PM_EFFECT_CONFIG__ROTATING_PALETTE[] PROGMEM = "Rotate Palette@!,Shift,,,,,,Reverse;!;!;ix=0,sx=40,paln=Rainbow 16";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Step_Through_Palette
 * @note : Picks sequential colours from palette and steps through them ie Red, Green, Blue, Orange  would be (R,G) (B,G) (B,O) with the next palette colour in order, keeping the same colour for two steps 
 *  
 * Future Change: Make it so more than two can be shown, ie if 5 colours exist, then have "Intensity" (as percentage) select how many colours to remain visible
 * 
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * 
     * Note: allocate_buffer is used as transition data
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Stepping_Palette()
{

  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return USE_ANIMATOR;
  }

  uint16_t* region_p          = &SEGMENT.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);
  
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

  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  uint32_t colour;

  // ALOG_INF(PSTR("SEGMENT %d,%d"), SEGMENT.start, SEGMENT.stop);

  // indexing from start to stop does not work when segment is not the first (seg0)

  for(unsigned index=0;
                index<=SEGMENT.length();
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, desired_pixel, &pixel_position);
    
    // colour = RgbcctColor::ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SEGMENT.Set_DynamicBuffer_DesiredColour(index, colour);
        
  } 




  // ALOG_INF(PSTR("step %d,%d,%d"), colour.R, colour.G, colour.B);

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }

  
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });

    return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__STEPPING_PALETTE[] PROGMEM = "Stepping Palette@!,,,,,,,,!;;!;ep=1000,ix=127"; 


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Step_Through_Palette
 * @note : Picks sequential colours from palette and steps through them ie Red, Green, Blue, Orange  would be (R,G) (B,G) (B,O) with the next palette colour in order, keeping the same colour for two steps 
 *  
 * Future Change: Make it so more than two can be shown, ie if 5 colours exist, then have "Intensity" (as percentage) select how many colours to remain visible
 * 
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * 
     * Note: allocate_buffer is used as transition data
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__TimeBased__HourProgress()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return FRAMETIME; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  uint32_t colour = 0;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, 0);
  }

  uint16_t seconds_into_hour = tkr_time->RtcTime.second + (tkr_time->RtcTime.minute * 60); // Using seconds for improved accuracy
  uint16_t progress = map(seconds_into_hour, 0,3550, 1,SEGLEN); // using 3550 to enable 50 seconds on full display

  for(uint16_t pixel = 0; pixel < progress; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour);
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__TIMEBASED__HOUR_PROGRESS[] PROGMEM = "Hour Progress@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Step_Through_Palette
 * @note : Picks sequential colours from palette and steps through them ie Red, Green, Blue, Orange  would be (R,G) (B,G) (B,O) with the next palette colour in order, keeping the same colour for two steps 
 *  
 * Future Change: Make it so more than two can be shown, ie if 5 colours exist, then have "Intensity" (as percentage) select how many colours to remain visible
 * 
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * 
     * Note: allocate_buffer is used as transition data
     * 
     * New for 2023, add to outside tree.
     * Similar to step_palette over the white strip palette
     * Apply the stepping palette over the background colour (e.g white), and use the intensity option to set how many background pixels between palette colours
     * eg. 5 white that are on dim (for brightness correction) and then palette changing single colours (or two colours?) with white infill
     * 
     * Slider1: The spacing between palette colours over background, ie how many backfill pixels
     * Slider2: How many of colours of the palette to show at once; 0/255 is single palette, and 255/255 is all (but one) palette over background
     * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Stepping_Palette_With_Background()
{

  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return USE_ANIMATOR;
  }

  uint16_t* region_p          = &SEGMENT.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEO "pixels_in_map= %d"),pixels_in_map);
  
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

  AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  uint32_t colour;

  for(unsigned index=SEGMENT.start;
                index<=SEGMENT.stop;
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, desired_pixel, &pixel_position);
    
    // colour = RgbcctColor::ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    // SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index++, SEGMENT.colour_width__used_in_effect_generate, SEGCOLOR_RGBCCT(0));
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index++, SEGMENT.colour_width__used_in_effect_generate, SEGCOLOR_RGBCCT(0));
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index, SEGMENT.colour_width__used_in_effect_generate, colour);
    SEGMENT.Set_DynamicBuffer_DesiredColour(index++, SEGCOLOR_U32(0));
    SEGMENT.Set_DynamicBuffer_DesiredColour(index++, SEGCOLOR_U32(0));
    SEGMENT.Set_DynamicBuffer_DesiredColour(index, colour);
        
  } 

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }
  
  
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__STEPPING_PALETTE_WITH_BACKGROUND[] PROGMEM = "Stepping Palette Background@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Blend Palette to White
 * @note : Using a static palette (in order), the effect will change from static_palette (100% saturation) to cold_white (0% saturation)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the minimum saturation. 100% Intensity means it can fade to complete white
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
     * Desc: Spread palette across segment
     * If gradient value exists, then use it to spread colour across segment
     * If no index in palette, then spread palette equal distant across palette
     * */
    // EFFECTS_FUNCTION__STATIC_PALETTE_GRADIENT__ID,
    /**
     * Desc: Animation, that fades from selected palette to white,
     *       The intensity of fade (closer to white, ie loss of saturation) will depend on intensity value
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
// uint16_t mAnimatorLight::EffectAnim__Palette_Colour_Fade_Saturation()
// {

//   uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length();

//   if (!SEGMENT.allocateColourData(dataSize)){    
//     ALOG_ERR( PM_MEMORY_INSUFFICIENT );
//     SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
//     return FRAMETIME;
//   }
  
//   /**
//    * @brief Using 
//    * 
//    */

//   // if(aux0 < 1000)
//   // {

//   // }
  

//   uint8_t saturation_255 = 200;
//   float   saturation     = saturation_255/255.0f;
//   HsbColor colour_hsb = HsbColor(RgbcctColor());
  
//   RgbcctColor colour = RgbcctColor();
//   for(uint16_t pixel = 0; 
//                pixel < SEGLEN; 
//                pixel++
//   ){

//     colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);

//     colour_hsb = RgbColor(colour); // to HSB
//     colour_hsb.S = saturation;
//     colour = colour_hsb;
    
//     colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

//     SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour);

//     #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
//     #endif

//   }
  
//     //SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_width__used_in_effect_generate, RgbColor(255));


//   // Get starting positions already on show
//   SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

//   // Call the animator to blend from previous to new
//   SetSegment_AnimFunctionCallback(  SEGIDX, 
//     [this](const AnimationParam& param){ 
//       this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
//     }
//   );

// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Blend Palette Between Another Palette
 * @note : Using a static palette (in order), the effect will change from static_palette (palette_id) to a different palette (palette_id saved as aux0)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the amount to blend into the second one. ie 0%=palette_id and 100%=aux0_palette
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Blend_Two_Palettes()
{

  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels
  //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return USE_ANIMATOR;
  }
    
  /**
   * @brief Intensity describes the amount of pixels to update
   *  Intensity 0-255 ==> LED length 1 to length (since we cant have zero)
   **/
  uint16_t pixels_to_update = mapvalue(
                                        SEGMENT.intensity, 
                                        0,255, 
                                        0,SEGMENT.length()
                                      );

  uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

  uint16_t pixel_index = 0;
  uint32_t colour = 0;//RgbcctColor();

  /**
   * @brief On first run, all leds need set once
   * 
   */
  if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
    pixels_to_update = SEGMENT.length();
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 


  for(unsigned iter = 0; 
                iter < pixels_to_update; 
                iter++
  ){
    /**
     * @brief 
     * min: lower bound of the random value, inclusive (optional).
     * max: upper bound of the random value, exclusive.
    **/
    pixel_index = random(0, SEGMENT.length()+1); // Indexing must be relative to buffer

    // On first run, I need a new way to set them all.
    if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
      pixel_index = iter;
    }
    
    desired_pixel = random(0, pixels_in_map);
  
    colour = SEGMENT.GetPaletteColour(desired_pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, desired_pixel);

    #ifdef DEBUG_TRACE_ANIMATOR_SEGMENTS
    ALOG_TST(PSTR("desiredpixel%d, colour=%d,%d,%d"), desired_pixel, colour.R, colour.G, colour.B); 
    #endif // DEBUG_TRACE_ANIMATOR_SEGMENTS
    
    // 2us
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel_index, colour);

  }

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });

    return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__BLEND_TWO_PALETTES[] PROGMEM = "Blend Two Palettes@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : NOTDONE Blend Palette Between Another Palette
 * @note : Using a static palette (in order), the effect will change from static_palette (palette_id) to a different palette (palette_id saved as aux0)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the amount to blend into the second one. ie 0%=palette_id and 100%=aux0_palette
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
 * This effect later needs intensity to define the brightness difference between the base palette and the secondary "flash" palette
 * The speed must also be made to work like it does for WLED, instead of setting a manual ratems
 * 
     * Desc: Draws palette_primary in order, then randomly takes from a second palette (saved by aux0)
     *       This will allow white palettes to work, or else applying coloured twinkles over a white palette too
     * 
     * 
     * This needs split, this version should be "Glow palette over another" in NPB animations, then "Twinkle" should be instant version without animator
     * Then have a special case that is white twinkles without setting the palette 2
     * Lets create brand new ones, then phase these out when happy to remove confusion with old and new?

These twinkles require brightness to be set within the effect, and use a flag, force the Luminescence to be set to 255, and then the brightness to be set by the effect
Edited: Jan 2025: With NPBLg, since brightness is set by the bus, we must instead set the base palette brightness here and twinkle palette is the full brightness


params_user[0]: palette to twinkle over
params_user[1]: brightness of twinkle palette
params_user[2]: base brightness


 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle_Palette_Onto_Palette()
{
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength();

  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return USE_ANIMATOR;
  }

  // For backwards comptability, if param2 is not set, then default base brightness to 100
  uint8_t brightness_base = SEGMENT.params_user[2] > 0 ? SEGMENT.params_user[2] : 100;

  /**
   * @brief Step 1: Draw palette 1 as base
   **/
  uint32_t colour = 0;
  for(uint16_t pixel = 0; 
               pixel < SEGMENT.virtualLength(); 
               pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel);    
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
    colour = AdjustColourWithBrightness(colour,brightness_base);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }

  /**
   * @brief Step 2: Draw over randomly with palette 2
   * Intensity will pick how many to randomly pick
   * 
   * For xmas2022, forced as white
   */

  uint32_t overdraw_colour = 0;//RgbcctColor();

  
  uint16_t pixels_to_update = mapvalue(
                                      SEGMENT.intensity, 
                                      0,255, 
                                      0,SEGMENT.length()
                                    );

  uint16_t palette_to_draw_ontop = SEGMENT.params_user[0];
  uint16_t palette_over_length = GetNumberOfColoursInPalette(palette_to_draw_ontop)-1; //since we need 0 to length-1 (eg 6 colours, means indexs 0 to 5)

  // ALOG_INF(PSTR("palette_over_length %d"), palette_over_length);
  // ALOG_INF(PSTR("[0] %d"), SEGMENT.params_user[0]);
  // ALOG_INF(PSTR("[1] %d"), SEGMENT.params_user[1]);

  for(uint16_t pixel = 0; 
                pixel < pixels_to_update; 
                pixel++
  ){

    // colour = RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
    
    // if(SEGMENT.flags.brightness_applied_during_colour_generation){
    //   colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    // }

    /**
     * @brief 
     * Contrary to expected, where we may want to select only from the colours by length in palette,
     * we actually want to select by the segment length, and hence, gradients will be picked randomly on full calculated length.
     * Discrete palettes, should still be retrieved as solid colours, as the modulo is used to wrap/iter over the palettes that are not gradients
     */
    uint16_t random_pic = random(0,SEGLEN);

    RgbwwColor c = GetColourFromUnloadedPalette3(SEGMENT.params_user[0], random_pic); // We want seglen, because gradient palettes should be drawn in their expected gradient
    
    overdraw_colour = RGBW32(c.R, c.G, c.B, c.WW);
    
    
    // overdraw_colour = GetColourFromUnloadedPalette3(0, random(0,palette_over_length));

    // if(pixel==0) ALOG_INF(PSTR("overdraw %d %d %d %d %d"), SEGMENT.params_user[0], random_pic, overdraw_colour[0], overdraw_colour[1], overdraw_colour[2]);

    /**
     * @brief If palette is one of the segment colours, then use its built in brightness as the palette over brightness
     **/
    if(palette_to_draw_ontop < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
    {
      overdraw_colour = AdjustColourWithBrightness(overdraw_colour,255);//.WithBrightness(); // Use already set SegColour 
    }else{
      // overdraw_colour = overdraw_colour.WithBrightness(SEGMENT.params_user[1]); // Param1 is brightness control of Palette2
      overdraw_colour = AdjustColourWithBrightness(overdraw_colour,SEGMENT.params_user[1]);
    }

    // overdraw_colour.debug_print("overdraw_colour");

    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random(0,SEGMENT.length()), SEGMENT.colour_width__used_in_effect_generate, overdraw_colour);
    SEGMENT.Set_DynamicBuffer_DesiredColour(random(0,SEGMENT.length()), overdraw_colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(SEGMENT.palette_id), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });

return USE_ANIMATOR;

}
/**Default
 * Twinkle White over Colour Default Palette
 * param[0]:    (I change my palette orders that much, although inefficient, I will use the palette name) "paln" where "pal" is the index, and paln is the name version
 * 
 */
static const char PM_EFFECT_CONFIG__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI[] PROGMEM = "Twinkle Palette Two on One@,,,,,Repeat Rate (ms);!,!,!,!,!;pal=Snowy 02,ep=ft"; // 7 sliders + 4 options before first ;



uint16_t mAnimatorLight::EffectAnim__Twinkle_Out_Palette()
{
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength();

  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return USE_ANIMATOR;
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 
  
  /**
   * @brief Step 1: Draw palette 1 as base
   * 
   */
  uint32_t colour = 0;//RgbcctColor();
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);    
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }

  /**
   * @brief Step 2: Draw over randomly with palette 2
   * Intensity will pick how many to randomly pick
   * 
   * For xmas2022, forced as white
   */

  uint32_t overdraw_colour = 0;//RgbcctColor();








  // colour = RgbcctColor(255,255,255);
  uint16_t random_pixel = 0;
  
  uint16_t pixels_to_update = mapvalue(
                                      SEGMENT.intensity, 
                                      0,255, 
                                      0,SEGMENT.length()
                                    );

  for(uint16_t pixel = 0; 
                pixel < pixels_to_update; 
                pixel++
  ){

    // colour = RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
    
    // if(SEGMENT.flags.brightness_applied_during_colour_generation){
    //   colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    // }

    random_pixel = random(0, SEGMENT.length()); // Indexing must be relative to buffer

    overdraw_colour = 0;//RgbcctColor();  //GetColourFromUnloadedPalette2(SEGMENT.params_user[0], random_pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.aux0, pixel);

    // overdraw_colour.debug_print("overdraw_colour");

    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel, SEGMENT.colour_width__used_in_effect_generate, overdraw_colour);
    SEGMENT.Set_DynamicBuffer_DesiredColour(random_pixel, overdraw_colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });


return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__TWINKLE_OUT_PALETTE[] PROGMEM = "Twinkle Out Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : NOTDONE Blend Palette Between Another Palette
 * @note : 
 * 
 * Is this reduntant with the popping
 * 
 * 
     * Desc: Picks colours from palette and randomly adds them as full brightness
     *       With each call of the animation (as new colours are added), all pixels will
     *       decay in brightness (easiest by half each time until 0).
     *       Note: The decay must happen first, so the new draws are still at full brightness
     *       This will require saving the total output in the buffer.
     * 
     * Para: 
     * 
     * ^^ This is the one I aciddenly made on and recorded on video. 
     * 
     * I simply want random leds to turn on, then everything evenally fades together until off.
     * The amount of on/darkness will depend on intensity
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle_Decaying_Palette()
{
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.virtualLength();

// effect not written yet, copied from another

  if (!SEGMENT.allocateColourData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return USE_ANIMATOR;
  }
  
  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  /**
   * @brief Step 1: Draw palette 1
   * 
   */
  uint32_t colour = 0;//RgbcctColor();
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
        
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }

  /**
   * @brief Step 2: Draw over randomly with palette 2
   * Intensity will pick how many to randomly pick
   * 
   * For xmas2022, forced as white
   */
  colour = RGBW32(255,255,255,255);// RgbcctColor(255,255,255);
  uint16_t random_pixel = 0;
  
  uint16_t pixels_to_update = mapvalue(
                                      SEGMENT.intensity, 
                                      0,255, 
                                      0,SEGMENT.length()
                                    );

  for(uint16_t pixel = 0; 
                pixel < pixels_to_update; 
                pixel++
  ){

    // colour = RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
    
    // if(SEGMENT.flags.brightness_applied_during_colour_generation){
    //   colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
    // }

    random_pixel = random(0, SEGMENT.length()); // Indexing must be relative to buffer

    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel, SEGMENT.colour_width__used_in_effect_generate, colour);
    SEGMENT.Set_DynamicBuffer_DesiredColour(random_pixel, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  
    SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) {
      SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param);
    });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__TWINKLE_DECAYING_PALETTE[] PROGMEM = "Twinkle Decaying Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC  SECITON END ////////////////////////////////////////////////////////




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : Sunrise_Alarm_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__Sunrise_Alarm_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return USE_ANIMATOR; } // Pixel_Width * Two_Channels * Pixel_Count
    
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__SUNRISE_ALARM_01[] PROGMEM = "Sun Alarm 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : SunPositions__Azimuth_Selects_Gradient_Of_Palette_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return USE_ANIMATOR; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01[] PROGMEM = "Sun Azimuth Palette 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : EffectAnim__SunPositions__Sunset_Blended_Palettes_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__Sunset_Blended_Palettes_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return USE_ANIMATOR; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__SUNSET_BLENDED_PALETTES_01[] PROGMEM = "SunSet Blended Palettes 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : EffectAnim__SunPositions__DrawSun_1D_Elevation_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Elevation_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return USE_ANIMATOR; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_ELEVATION_01[] PROGMEM = "Sun 1D Elevation 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : EffectAnim__SunPositions__DrawSun_1D_Elevation_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Azimuth_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return USE_ANIMATOR; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_AZIMUTH_01[] PROGMEM = "Sun 1D Azimuth 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return EFFECT_DEFAULT(); } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01[] PROGMEM = "Sun 2D El/Az 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description           : EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
uint16_t mAnimatorLight::EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return EFFECT_DEFAULT(); } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour); 
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

  return USE_ANIMATOR;

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01[] PROGMEM = "Sun White Corrected El 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS



































































/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS2 
uint16_t mAnimatorLight::EffectAnim__SunPositions_Elevation_Palette_Progress_Step()
{

// for sun thing
// I will still use this function to get the raw colour, I just need another intermediate function that does the conversion with sun elevation
// also add sun elevation and azimuth into settings struct, that way I can update it locally or via command 

  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_TST(PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  SEGMENT.transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS2
uint16_t mAnimatorLight::EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend()
{

  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_TST(PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  SEGMENT.transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


// /**************************************************************************************************************************************************************
//  * @brief  Solid_Colour_Based_On_Sun_Elevation_02
//  * @note   From -10 to noon, CCT will range from yellow to daywhite
//  * @note   From -5 to dusk, blue will go from 0 to max_brightness 
//  * 
//  * @note   Gloabl brightness will be manual, or controlled indirectly eg via mqtt
//  * 
//  * @note   Using RgbcctColour palette that is designed for each point in elevation
//  * *************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS2
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01()
{
 
//  #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_02"));

//   // pCONT_iLight->animation.palette_id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   uint8_t segment_index = SEGIDX;
//   uint16_t start_pixel = SEGMENT.start;
//   uint16_t end_pixel = SEGMENT.stop;


//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(SEGMENT.palette_id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   SEGMENT.flags.brightness_applied_during_colour_generation = false;

//   /**
//    * Solar data to use, defined here for testing or simulations
//    * */
// float sun_elevation = 0;
// #ifdef USE_MODULE_SENSORS_SUN_TRACKING
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
//   mPalette::PALETTE* ptr = &mPaletteI->static_palettes[SEGMENT.palette_id];
//   uint8_t pixels_max = GetNumberOfColoursInPalette(palette_p);
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
//   // ALOG_INF(PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // ALOG_INF(PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // ALOG_INF(PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   tkr_anim->force_update();

// //set desired colour
//   // SEGMENT.active_rgbcct_colour_p->  = c_blended;

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!SEGMENT.rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       SEGMENT.rgbcct_controller->getBrightnessRGB_WithGlobalApplied(),
//       SEGMENT.rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   // this->setAnimFunctionCallback([this](const AnimationParam& param){
//   //     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//         // Call the animator to blend from previous to new
//   SetSegment_AnimFunctionCallback(  SEGIDX, 
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(param);
//     }
//   );

//   #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
   
}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS2
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01()
{
 
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

// #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD


//   SEGMENT.palette_id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

//   mPaletteI->SetPaletteListPtrFromID(SEGMENT.palette_id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values

// float sun_elevation = 0;
// #ifdef USE_MODULE_SENSORS_SUN_TRACKING
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif

//   if(sun_elevation < -20)
//   {
//     SEGMENT.rgbcct_controller->setCCT(pCONT_iLight->get_CTRangeMax());      
//   }else
//   if(sun_elevation > 20)
//   {
//     SEGMENT.rgbcct_controller->setCCT(pCONT_iLight->get_CTRangeMin());      
//   }else{
//     // Convert elevation into percentage
//     uint8_t elev_perc = map(sun_elevation,-20,20,0,100);
//     // Convert percentage into cct
//     uint16_t cct_val = mapvalue(elev_perc, 0,100, pCONT_iLight->get_CTRangeMax(),pCONT_iLight->get_CTRangeMin());
 
//     // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "cct_val=%d"),cct_val);
//     // Set the colour temp
//     SEGMENT.rgbcct_controller->setCCT(cct_val);    
//   }

//   SEGMENT.flags.brightness_applied_during_colour_generation = false;
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);
//   SEGMENT.flags.fForceUpdate = true;

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!SEGMENT.rgbcct_controller->getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       SEGMENT.rgbcct_controller->getBrightnessRGB_WithGlobalApplied(),
//       SEGMENT.rgbcct_controller->getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   // this->setAnimFunctionCallback([this](const AnimationParam& param){
//   //     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });

//         // Call the animator to blend from previous to new
//   SetSegment_AnimFunctionCallback(  SEGIDX, 
//     [this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(param);
//     }
//   );
   
// #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS 


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : RGB CLock
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

uint16_t mAnimatorLight::LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing) {
  byte posOffset = 0;                                                                     // this offset will be used to move hours and minutes...
  if ( LED_DIGITS / 2 > 2 ) posOffset = 2;                                                // ... to the left so we have room for the seconds when there's 6 digits available
  if ( displayMode == 0 ) {                                                               // if 12h mode is selected...
    if ( tkr_time->hourFormat12(t) >= 10 ){
      LCDDisplay_showDigit(1, color + colorSpacing * 2, 3 + posOffset);   // ...and hour > 10, display 1 at position 3
    }
    LCDDisplay_showDigit((tkr_time->hourFormat12(t) % 10), color + colorSpacing * 3, 2  + posOffset);          // display 2nd digit of HH anyways
  } else if ( displayMode == 1 ) {                                                        // if 24h mode is selected...
    if ( tkr_time->hour(t) > 9 ) LCDDisplay_showDigit(tkr_time->hour(t) / 10, color + colorSpacing * 2, 3 + posOffset);  // ...and hour > 9, show 1st digit at position 3 (this is to avoid a leading 0 from 0:00 - 9:00 in 24h mode)
    LCDDisplay_showDigit(tkr_time->hour(t) % 10, color + colorSpacing * 3, 2 + posOffset);                     // again, display 2nd digit of HH anyways
  }
  LCDDisplay_showDigit((tkr_time->minute(t) / 10), color + colorSpacing * 4, 1 + posOffset);                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((tkr_time->minute(t) % 10), color + colorSpacing * 5, 0 + posOffset);                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  if ( posOffset > 0 ) {
    LCDDisplay_showDigit((tkr_time->second(t) / 10), color + colorSpacing * 6, 1);
    LCDDisplay_showDigit((tkr_time->second(t) % 10), color + colorSpacing * 7, 0);
  }
  if ( tkr_time->second(t) % 2 == 0 ) 
    LCDDisplay_showDots(2, tkr_time->second(t) * 4.25);                                // show : between hours and minutes on even seconds with the color cycling through the palette once per minute
  lastSecond = tkr_time->second(t);
}


uint16_t mAnimatorLight::LCDDisplay_showSegment(byte segment, byte color_index, byte segDisplay) {
  
  // This shows the segments from top of the sketch on a given position (segDisplay).
  // pos 0 is the most right one (seen from the front) where data in is connected to the arduino
  byte leds_per_segment = 1 + abs( segGroups[segment][1] - segGroups[segment][0] );            // get difference between 2nd and 1st value in array to get led count for this segment
  if ( segDisplay % 2 != 0 ) segment += 7;                                                  // if segDisplay/position is odd add 7 to segment

  uint16_t pixel_index = 0;

  for (byte i = 0; i < leds_per_segment; i++) 
  {                                             // fill all leds inside current segment with color
    // animation_colours[( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);

    pixel_index = ( segGroups[segment][0] + ( segDisplay / 2 ) * ( LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS ) ) + i;

    RgbcctColor colour = RgbcctColor();
    colour = SEGMENT.GetPaletteColour(color_index);      
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );

  }
  
}


uint16_t mAnimatorLight::LCDDisplay_showDigit(byte digit, byte color, byte pos) {
  // This draws numbers using the according segments as defined on top of the sketch (0 - 9)
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) LCDDisplay_showSegment(i, color, pos);
  }
}


uint16_t mAnimatorLight::LCDDisplay_showDots(byte dots, byte color) {

  // // in 12h mode and while in setup upper dots resemble AM, all dots resemble PM
  // byte startPos = LED_PER_DIGITS_STRIP;
  // if ( LED_BETWEEN_DIGITS_STRIPS % 2 == 0 ) {                                                                 // only SE/TE should have a even amount here (0/2 leds between digits)
  //   animation_colours[startPos].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //   if ( dots == 2 ) animation_colours[startPos + 1].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  // } else {                                                                                                    // Regular and XL have 5 leds between digits
  //   animation_colours[startPos].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //   animation_colours[startPos + 1].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //   if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 1].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //     }
  //   if ( dots == 2 ) {
  //     animation_colours[startPos + 3].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //     animation_colours[startPos + 4].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //     if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 3].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 4].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //     }
  //   }
  // }



  // in 12h mode and while in setup upper dots resemble AM, all dots resemble PM
  byte startPos = LED_PER_DIGITS_STRIP;
  if ( LED_BETWEEN_DIGITS_STRIPS % 2 == 0 ) {                                                                 // only SE/TE should have a even amount here (0/2 leds between digits)


    RgbcctColor colour = RgbcctColor();
    colour = SEGMENT.GetPaletteColour(color);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), startPos, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );


    if ( dots == 2 ) 
    {

      RgbcctColor colour = RgbcctColor();
      colour = SEGMENT.GetPaletteColour(color);      
      SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), startPos + 1, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );

    }
  } 
  // else {                                                                                                    // Regular and XL have 5 leds between digits
  //   animation_colours[startPos].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //   animation_colours[startPos + 1].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //   if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 1].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //     }
  //   if ( dots == 2 ) {
  //     animation_colours[startPos + 3].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //     animation_colours[startPos + 4].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);
  //     if ( LED_DIGITS / 3 > 1 ) {
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 3].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //       animation_colours[startPos + LED_PER_DIGITS_STRIP + LED_BETWEEN_DIGITS_STRIPS + 4].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, color);//colour;// = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, color, brightness, LINEARBLEND);
  //     }
  //   }
  // }


}


/****
 * @brief Basic clock with no animations
 */
uint16_t mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_01()
{

  ALOG_DBM( PSTR("_segments[%d].colour_width__used_in_effect_generate = %d"), SEGIDX, SEGMENT.colour_width__used_in_effect_generate);
    
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_DBM(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize))
  {
    ALOG_ERR(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION; // Default
  }

  for(int i=0;i<SEGLEN;i++)
  {    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_width__used_in_effect_generate, RgbcctColor());
  }

  uint8_t colour_offset = 1;
  if(SEGMENT.palette_id < 83)
    colour_offset = 50;

  ALOG_DBM(PSTR("colour_offset = %d"), colour_offset);

  LCDDisplay_displayTime(tkr_time->Rtc.local_time, 0, colour_offset);

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
    [this](void){
      this->ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
    }
  );
  #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK

}
static const char PM_EFFECT_CONFIG__7SEGMENTDISPLAY__CLOCKTIME_01[] PROGMEM = "Clock Basic 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;

uint16_t mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}


/**************************************************************************************************************************************************************
***************************************************************************************************************************************************************
********* EffectAnim__7SegmentDisplay__ClockTime_02 *************************************************************************************************************************
***************************************************************************************************************************************************************
***************************************************************************************************************************************************************/

/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
uint16_t mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_02(){
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   //Display on all pixels
//   UpdateDesiredColourFromPaletteSelected();
  // tkr_set->Settings.light_settings.type = ADDRESSABLE_SK6812;
    
  // SEGMENT.colour_width__used_in_effect_generate = RgbcctColor::ColourType::LIGHT_TYPE__RGBW__ID;
  
  ALOG_TST(PSTR("02    SEGMENT.colour_width__used_in_effect_generate = %d"), SEGMENT.colour_width__used_in_effect_generate);

    
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize))
  {
    ALOG_TST(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION; // Default
  }

  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  
  // for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor();}
  uint8_t segment_index=0;

  for(int i=0;i<93;i++){
    // animation_colours[i].DesiredColour = RgbcctColor();
    // }
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_width__used_in_effect_generate, RgbwColor(0,0,0,0));
  
  }

  LCDDisplay_displayTime(tkr_time->Rtc.local_time, 0, colorOffset);
  
  // if (overlayMode == 1) LCDDisplay_colorOverlay();
  // for (byte i = 0; i < LED_COUNT; i++) {                                                                    // check each led...
  //   if (animation_colours[i].DesiredColour.CalculateBrightness())  {

  //     RgbcctColor colour = RgbcctColor();

  //     animation_colours[i].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, startColor + (colorOffset * i));      
  //     SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_width__used_in_effect_generate, RgbwColor(0,0,0,0));



  //   }
  // }    


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10), 0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  this->setCallback_ConstructJSONBody_Debug_Animations_Progress(
    [this](void){
      this->ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();
    }
  );
  #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK


}


static const char PM_EFFECT_CONFIG__7SEGMENTDISPLAY__CLOCKTIME_02[] PROGMEM = "Clock Basic 02@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;

uint16_t mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
uint16_t mAnimatorLight::EffectAnim__7SegmentDisplay__ManualNumber_01()
{

  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize))
  {
    ALOG_TST(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION; // Default
  }
    
  
  ALOG_TST(PSTR("Numbers_Basic    _segments[].colour_width__used_in_effect_generate = %d"), SEGMENT.colour_width__used_in_effect_generate);
  ALOG_TST(PSTR("lcd_display_show_number = %d"), lcd_display_show_number);
  /**
   * @brief Reset all new colours so only new sections of clock are lit
   **/
  for(int i=0;i<SEGLEN;i++){
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_width__used_in_effect_generate, RgbwColor(0,0,0,0));
  }


  uint8_t digit_count = mSupport::NumDigits(lcd_display_show_number);

  LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALNUMBER_01[] PROGMEM = "Seven-Segment Number 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
uint16_t mAnimatorLight::EffectAnim__7SegmentDisplay__ManualString_01()
{
  
  uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateColourData(dataSize))
  {
    ALOG_TST(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION; // Default
  }
  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  ALOG_TST(PSTR("Numbers_Basic    _segments[].colour_width__used_in_effect_generate = %d"), SEGMENT.colour_width__used_in_effect_generate);
  ALOG_TST(PSTR("lcd_display_show_number = %d"), lcd_display_show_number);

  /**
   * @brief Reset all new colours so only new sections of clock are lit
   **/
  for(int i=0;i<SEGLEN;i++){
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_width__used_in_effect_generate, RgbwColor(0,0,0,0));
  }

  uint8_t digit_count = strlen(lcd_display_show_string);

  if(digit_count > 4)
  {
    ALOG_ERR(PSTR("too big"));
  }

  uint8_t pos = 0;
  uint8_t number_show = 0;

  for(int i=0;i<digit_count;i++)
  {
    pos = digit_count - 1 - i;
    if(lcd_display_show_string[i] != ' ')
    {
      LCDDisplay_showDigit(lcd_display_show_string[i]-48, 0, pos ); 
    }
  }
  

  // Get starting positions already on show
  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALSTRING_01[] PROGMEM = "Seven-Segment String 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;



#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK


/*
***   WLED Effects Below  ********************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************************
*/


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/******************************************************************************************************************************************************************************************************************
   @brief: Colour_Wipe_Base
   @note : Converted from WLED Effects
           LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
           if (bool rev == true) then LEDs are turned off in reverse order
   @note   Rate: FRAME_RATE

          aux0: is a 8bit (255) value that stores where in the colour wheel the (old) draw colour is coming from
          aux1: saves the 2nd colour (new) to be drawn
          step: saves what part of the animation is happening (e.g. create, move, rotate etc)

          Intensity: Smoothness [0=Instant, 255=Blended]

          Wipe "Over": WLED style, seg1 colour is drawn over the active palette that is spanned over the segment
          Wipe Random: WLED style, picks random colours and wipes them
          Wipe Palette: Mine, pick each colour from active palette and wipe with them

   @param rev             Reversed
   @param useRandomColors 
********************************************************************************************************************************************************************************************************************/
//each needs 20 bytes
//Spark type is used for popcorn, 1D fireworks, and drip
typedef struct Spark {
  float pos, posX;
  float vel, velX;
  uint16_t col;
  uint8_t colIndex;
} spark;
uint16_t mAnimatorLight::BaseEffectAnim__Base_Colour_Wipe(bool rev, bool useRandomColors, bool useIterateOverPalette)
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  
  // Speed of 128, cycletime = 19800
  uint32_t cycleTime = 750 + (255 - SEGMENT.speed)*150;
  // millis() % total_cycle_time ==> gives remainder, and therefore process of animation from 0->cycleTime is 0->100%
  uint32_t perc = effect_start_time % cycleTime;
  // prog gets the process from 0->65535 again
  uint16_t prog = (perc * 65535) / cycleTime;
   
  bool back = (prog > 32767); // if in second half of 16 bit, then take away the first half to scale the 2nd into the first as positive counter
  if (back) {
    prog -= 32767;
    if (SEGMENT.step == 0) SEGMENT.step = 1;
  } else {
    if (SEGMENT.step == 2) SEGMENT.step = 3; // Trigger color change
  }
  
  if (useRandomColors) {
    // First call ie create colours to later be animated
    if (SEGMENT.call == 0) {
      SEGMENT.aux0 = hw_random8();
      ALOG_INF(PSTR("hw_random8() %d %d %d"), SEGMENT.aux0, SEGMENT.aux1, SEGMENT.call);
      SEGMENT.step = 3;
    }
    if (SEGMENT.step == 1) { //if flag set, change to new random color
      SEGMENT.aux1 = get_random_wheel_index(SEGMENT.aux0);
      SEGMENT.step = 2;
    }
    if (SEGMENT.step == 3) {
      SEGMENT.aux0 = get_random_wheel_index(SEGMENT.aux1);
      SEGMENT.step = 0;
    }
    // ALOG_INF(PSTR("useRandomColors %d %d %d"), SEGMENT.aux0, SEGMENT.aux1, SEGMENT.call);
  }else
  if(useIterateOverPalette)
  {
  
    uint8_t colours_in_palette = SEGMENT.GetPaletteDiscreteWidth();

    if (SEGMENT.call == 0) {
      SEGMENT.aux0 = 0;
      SEGMENT.aux1 = 1;
      SEGMENT.step = 3;
    }
    if (SEGMENT.step == 1) { // Reverse
      SEGMENT.aux1 = SEGMENT.aux0 + 1; // aux1 should be leading aux0, so the next colour
      SEGMENT.step = 2;
    }
    if (SEGMENT.step == 3) { // Forward
      SEGMENT.aux0 = SEGMENT.aux1 + 1;
      SEGMENT.step = 0;
    }
    if(SEGMENT.aux0 >= colours_in_palette) SEGMENT.aux0 = 0; // Wrap/Constrain
    if(SEGMENT.aux1 >= colours_in_palette) SEGMENT.aux1 = 0; // Wrap/Constrain
    // The wipe colour then needs to become the base colour on the next effect
    // ALOG_INF(PSTR("Colours in palette %d %d %d"), SEGMENT.GetPaletteDiscreteWidth(), SEGMENT.aux0, SEGMENT.aux1);
  }

  uint16_t ledIndex = (prog * SEGLEN) >> 15;
  uint16_t rem = 0;
  rem = (prog * SEGLEN) * 2; //mod 0xFFFF
  rem /= (SEGMENT.intensity +1);
  if (rem > 255) rem = 255;

  uint32_t col_wipe = 0;
  if(useRandomColors)
  {
    col_wipe = SEGMENT.color_wheel(SEGMENT.aux1);
  }
  else
  if(useIterateOverPalette)
  {
    col_wipe = SEGMENT.GetPaletteColour(SEGMENT.aux1, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
  }
  else
  {
    col_wipe = SEGCOLOR(1);
  }

  // ALOG_INF(PSTR("aux0 %d aux1 %d"), SEGMENT.aux0, SEGMENT.aux1);
  
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint16_t indexPixel = (rev && back) ? SEGLEN -1 -i : i;

    uint32_t col_base = 0;
    if(useRandomColors)
    {
      col_base = SEGMENT.color_wheel(SEGMENT.aux0);
    }
    else
    if(useIterateOverPalette)
    {
      col_base = SEGMENT.GetPaletteColour(SEGMENT.aux0, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    }
    else
    {
      col_base = SEGMENT.GetPaletteColour(indexPixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    }

    if (i < ledIndex) 
    {
      SEGMENT.setPixelColor(indexPixel, back ? col_wipe : col_base);
    } else
    {
      SEGMENT.setPixelColor(indexPixel, back ? col_base : col_wipe);
      if (i == ledIndex) SEGMENT.setPixelColor(indexPixel, ColourBlend(back ? col_base : col_wipe, back ? col_wipe : col_base, rem));
    }
  } 

  return FRAMETIME;

}


/**
 * Lights all LEDs one after another.
 * @note : mode_color_wipe
 */
uint16_t mAnimatorLight::EffectAnim__Colour_Wipe()
{
  return BaseEffectAnim__Base_Colour_Wipe(false, false);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE[] PROGMEM = "Wipe Over@!,!;!,!;!";

/**
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 * @note : mode_color_wipe_random
 */
uint16_t mAnimatorLight::EffectAnim__Colour_Wipe_Random()
{
  return BaseEffectAnim__Base_Colour_Wipe(false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE_RANDOM[] PROGMEM = "Wipe Random@!;;!";

/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
uint16_t mAnimatorLight::EffectAnim__Colour_Wipe_Palette()
{
  return BaseEffectAnim__Base_Colour_Wipe(false, false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE_PALETTE[] PROGMEM = "Wipe Palette@!;;!";


/**
 * Lights all LEDs one after another. Swipe is wipe, but back and forth
 * @note : mode_color_sweep
 */
uint16_t mAnimatorLight::EffectAnim__Colour_Sweep()
{
  return BaseEffectAnim__Base_Colour_Wipe(true, false);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP[] PROGMEM = "Sweep Over@!,!;!,!;!";


/*
 * Random color introduced alternating from start and end of strip
 */
uint16_t mAnimatorLight::EffectAnim__Colour_Sweep_Random()
{
  return BaseEffectAnim__Base_Colour_Wipe(true, true);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP_RANDOM[] PROGMEM = "Sweep Random@!;;!";

/*
 * Random color introduced alternating from start and end of strip->
 */
uint16_t mAnimatorLight::EffectAnim__Colour_Sweep_Palette()
{
  return BaseEffectAnim__Base_Colour_Wipe(true, false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP_PALETTE[] PROGMEM = "Sweep Palette@!;;!";


/********************************************************************************************************************************************************************************************************************
 * @description : Lights every LED in a random color. Changes all LED at the same time to new random colors.
 * @note : Converted from WLED Effects "mode_dynamic"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Dynamic()
{
  
  if (!SEGMENT.allocateData(SEGLEN)) return EFFECT_DEFAULT(); //allocation failed

  if(SEGMENT.call == 0) {
    //SEGMENT.fill(BLACK);
    for (unsigned i = 0; i < SEGLEN; i++) SEGMENT.data[i] = hw_random8();
  }

  uint32_t cycleTime = 50 + (255 - SEGMENT.speed)*15;
  uint32_t it = effect_start_time / cycleTime;
  if (it != SEGMENT.step && SEGMENT.speed != 0) //new color
  {
    for (unsigned i = 0; i < SEGLEN; i++) {
      if (hw_random8() <= SEGMENT.intensity) SEGMENT.data[i] = hw_random8(); // random color index
    }
    SEGMENT.step = it;
  }

  if (SEGMENT.check1) {
    for (unsigned i = 0; i < SEGLEN; i++) {
      SEGMENT.blendPixelColor(i, SEGMENT.color_wheel(SEGMENT.data[i]), 16);
    }
  } else {
    for (unsigned i = 0; i < SEGLEN; i++) {
      SEGMENT.setPixelColor(i, SEGMENT.color_wheel(SEGMENT.data[i]));
    }
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__DYNAMIC[] PROGMEM = "Dynamic@!,!,,,,Smooth;;!";


/********************************************************************************************************************************************************************************************************************
 * @description : Effect "Dynamic" with smooth color-fading
 * @note : Converted from WLED Effects "mode_dynamic_smooth"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Dynamic_Smooth(void) {
  bool old = SEGMENT.check1;
  SEGMENT.check1 = true;
  EffectAnim__Dynamic();
  SEGMENT.check1 = old;
  return FRAMETIME;
 }
static const char PM_EFFECT_CONFIG__DYNAMIC_SMOOTH[] PROGMEM = "Dynamic Smooth@!,!;;!";




// #ifndef ENABLE_DEVFEATURE_REMOVE__color_from_palette__
// /*
//  * Gets a single color from the currently selected palette.
//  * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
//  * @param mapping if true, LED position in segment is considered for color
//  * @param wrap FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
//  * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
//  * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
//  * @returns Single color from palette
//  */
// uint32_t mAnimatorLight::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
// {
  
//   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   
//   ALOG_DBG(PSTR("f::color_from_palette to be phased out"));
//   #endif

//   if (SEGMENT.palette_id == 0 && mcol < 3) return SEGCOLOR_U32(mcol); //WS2812FX default
//   uint8_t paletteIndex = i;
//   if (mapping) paletteIndex = (i*255)/(_virtualSegmentLength -1);  // This scales out segment_index to SEGLEN as 0 to 255
//   // ALOG_TST(PSTR("paletteIndex=%d"),paletteIndex);
//   if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
//   CRGB fastled_col;
//   fastled_col = ColorFromPalette_WithLoad( SEGMENT.palette_container->CRGB16Palette16_Palette.data, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);
//   return  fastled_col.r*65536 +  fastled_col.g*256 +  fastled_col.b;

// }
// #endif // ENABLE_DEVFEATURE_REMOVE__color_from_palette__


/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
// uint32_t mAnimatorLight::color_wheel(uint8_t pos) const{

// #ifdef ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED

//   // if (SEGMENT.palette_id){ // when default, so it skips this, causes brightness error
//   //   // Again this assumes palette colour index is ranged 0 to 255, so I NEED to fix mine to be the same
//   //   return SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
//   // }else
//   // {

//   //   ALOG_ERR(PSTR("Likely brightness error"));
  
//     pos = 255 - pos;
//     if(pos < 85) {
//       return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
//     } else if(pos < 170) {
//       pos -= 85;
//       return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
//     } else {
//       pos -= 170;
//       return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
//     }

//   // }


// #else
//   float hue_f = mSupport::mapfloat(pos, 0, 255, 0.0f, 1.0f);
//   HsbColor colh = HsbColor(hue_f,1,1);
//   return RgbcctColor::GetU32Colour(colh);
// #endif



// }

/*******************************************************************************************************************************************************************************************************************
 * color chase function.
 * color1 = background color
 * color2 and color3 = colors of two adjacent leds
 * @note : Converted from WLED Effects "chase"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{

  uint16_t counter = effect_start_time * ((SEGMENT.speed >> 2) + 1);
  uint16_t a = (counter * SEGLEN) >> 16;

  bool chase_random = (SEGMENT.effect_id == EFFECTS_FUNCTION__CHASE_RAINBOW__ID);
  if (chase_random) {
    if (a < SEGMENT.step) //we hit the start again, choose new color for Chase random
    {
      SEGMENT.aux1 = SEGMENT.aux0; //store previous random color
      SEGMENT.aux0 = get_random_wheel_index(SEGMENT.aux0);
    }
    color1 = SEGMENT.color_wheel(SEGMENT.aux0);
  }
  SEGMENT.step = a;

  // Use intensity setting to vary chase up to 1/2 string length
  unsigned size = 1 + ((SEGMENT.intensity * SEGLEN) >> 10);

  uint16_t b = a + size; // "trail" of chase, filled with color1
  if (b > SEGLEN) b -= SEGLEN;
  uint16_t c = b + size;
  if (c > SEGLEN) c -= SEGLEN;

  //background
  if (do_palette)
  {
    for (unsigned i = 0; i < SEGLEN; i++) {
      SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
    }
  } else SEGMENT.fill(color1);

  //if random, fill old background between a and end
  if (chase_random)
  {
    color1 = SEGMENT.color_wheel(SEGMENT.aux1);
    for (unsigned i = a; i < SEGLEN; i++)
      SEGMENT.setPixelColor(i, color1);
  }

  //fill between points a and b with color2
  if (a < b)
  {
    for (unsigned i = a; i < b; i++)
      SEGMENT.setPixelColor(i, color2);
  } else {
    for (unsigned i = a; i < SEGLEN; i++) //fill until end
      SEGMENT.setPixelColor(i, color2);
    for (unsigned i = 0; i < b; i++) //fill from start until b
      SEGMENT.setPixelColor(i, color2);
  }

  //fill between points b and c with color2
  if (b < c)
  {
    for (unsigned i = b; i < c; i++)
      SEGMENT.setPixelColor(i, color3);
  } else {
    for (unsigned i = b; i < SEGLEN; i++) //fill until end
      SEGMENT.setPixelColor(i, color3);
    for (unsigned i = 0; i < c; i++) //fill from start until c
      SEGMENT.setPixelColor(i, color3);
  }

  return FRAMETIME;
  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Bicolor chase, more primary color.
 * @note : Converted from WLED Effects "mode_chase_color"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chase_Colour()
{
  EffectAnim__Base_Chase(SEGCOLOR(1), (SEGCOLOR(2)) ? SEGCOLOR(2) : SEGCOLOR(0), SEGCOLOR(0), true);
}
static const char PM_EFFECT_CONFIG__CHASE_COLOR[] PROGMEM = "Chase@!,Width;!,!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Primary, secondary running on rainbow.
 * @note : Converted from WLED Effects "mode_chase_rainbow"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chase_Rainbow()
{
  unsigned color_sep = 256 / SEGLEN;
  if (color_sep == 0) color_sep = 1;                                           // correction for segments longer than 256 LEDs
  unsigned color_index = SEGMENT.call & 0xFF;
  uint32_t color = SEGMENT.color_wheel(((SEGMENT.step * color_sep) + color_index) & 0xFF);

  EffectAnim__Base_Chase(color, SEGCOLOR(0), SEGCOLOR(1), false);
  SET_DIRECT_MODE();
  
}
static const char PM_EFFECT_CONFIG__CHASE_RAINBOW[] PROGMEM = "Chase Rainbow@!,Width;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Sec flashes running on prim.
 * @note : Converted from WLED Effects "mode_chase_flash"
 ********************************************************************************************************************************************************************************************************************/
#define FLASH_COUNT 4
uint16_t mAnimatorLight::EffectAnim__Chase_Flash(){

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned flash_step = SEGMENT.call % ((FLASH_COUNT * 2) + 1);

  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  unsigned delay = 10 + ((30 * (uint16_t)(255 - SEGMENT.speed)) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    if(flash_step % 2 == 0) {
      unsigned n = SEGMENT.step;
      unsigned m = (SEGMENT.step + 1) % SEGLEN;
      SEGMENT.setPixelColor( n, SEGCOLOR(1));
      SEGMENT.setPixelColor( m, SEGCOLOR(1));
      delay = 20;
    } else {
      delay = 30;
    }
  } else {
    SEGMENT.step = (SEGMENT.step + 1) % SEGLEN;
  }

  return delay;
  
}
static const char PM_EFFECT_CONFIG__CHASE_FLASH[] PROGMEM = "Chase Flash@!;Bg,Fx;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Prim flashes running, followed by random color.
 * @note : Converted from WLED Effects "mode_chase_flash_random"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chase_Flash_Random()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned flash_step = SEGMENT.call % ((FLASH_COUNT * 2) + 1);

  for (int i = 0; i < SEGMENT.aux1; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_wheel(SEGMENT.aux0));
  }

  unsigned delay = 1 + ((10 * (uint16_t)(255 - SEGMENT.speed)) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    unsigned n = SEGMENT.aux1;
    unsigned m = (SEGMENT.aux1 + 1) % SEGLEN;
    if(flash_step % 2 == 0) {
      SEGMENT.setPixelColor( n, SEGCOLOR(0));
      SEGMENT.setPixelColor( m, SEGCOLOR(0));
      delay = 20;
    } else {
      SEGMENT.setPixelColor( n, SEGMENT.color_wheel(SEGMENT.aux0));
      SEGMENT.setPixelColor( m, SEGCOLOR(1));
      delay = 30;
    }
  } else {
    SEGMENT.aux1 = (SEGMENT.aux1 + 1) % SEGLEN;

    if (SEGMENT.aux1 == 0) {
      SEGMENT.aux0 = get_random_wheel_index(SEGMENT.aux0);
    }
  }
  
  return delay;

}
static const char PM_EFFECT_CONFIG__CHASE_FLASH_RANDOM[] PROGMEM = "Chase Flash Rnd@!;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Primary running on rainbow.
 * @note : Converted from WLED Effects "mode_chase_rainbow_white"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chase_Rainbow_White()
{

  uint16_t n = SEGMENT.step;
  uint16_t m = (SEGMENT.step + 1) % SEGLEN;
  uint32_t color2 = SEGMENT.color_wheel(((n * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);
  uint32_t color3 = SEGMENT.color_wheel(((m * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);

  EffectAnim__Base_Chase(SEGCOLOR(0), color2, color3, false);

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__CHASE_RAINBOW_WHITE[] PROGMEM = "Rainbow Runner@!,Size;Bg;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Alternating pixels running function.
 * @note : running
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool theatre)
{

  int width = (theatre ? 3 : 1) + (SEGMENT.intensity >> 4);  // window
  uint32_t cycleTime = 50 + (255 - SEGMENT.speed);
  uint32_t it = effect_start_time / cycleTime;
  bool usePalette = color1 == SEGCOLOR(0);

  for (unsigned i = 0; i < SEGLEN; i++) {
    uint32_t col = color2;
    if (usePalette) color1 = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    if (theatre) {
      if ((i % width) == SEGMENT.aux0) col = color1;
    } else {
      int pos = (i % (width<<1));
      if ((pos < SEGMENT.aux0-width) || ((pos >= SEGMENT.aux0) && (pos < SEGMENT.aux0+width))) col = color1;
    }
    SEGMENT.setPixelColor(i,col);
  }

  if (it != SEGMENT.step) {
    SEGMENT.aux0 = (SEGMENT.aux0 +1) % (theatre ? width : (width<<1));
    SEGMENT.step = it;
  }

  return FRAMETIME;
  
}


/**
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 * @note : mode_theater_chase
 */
uint16_t mAnimatorLight::EffectAnim__Chase_Theater(void) {
  return EffectAnim__Base_Chase_Theater(SEGCOLOR(0), SEGCOLOR(1), true);
}
static const char PM_EFFECT_CONFIG__THEATER_CHASE[] PROGMEM = "Theater@!,Gap size;!,!;!";


/**
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 * @note : mode_theater_chase_rainbow
 */
uint16_t mAnimatorLight::EffectAnim__Chase_Theatre_Rainbow(void) {
  return EffectAnim__Base_Chase_Theater( SEGMENT.color_wheel(SEGMENT.step), SEGCOLOR(1), true);
}
static const char PM_EFFECT_CONFIG__THEATER_CHASE_RAINBOW[] PROGMEM = "Theater Rainbow@!,Gap size;,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Tricolor chase function
 * @note : Converted from WLED Effects "tricolor_chase"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Chase_TriColour(uint32_t color1, uint32_t color2)
{
  uint32_t cycleTime = 50 + ((255 - SEGMENT.speed)<<1);
  uint32_t it = effect_start_time / cycleTime;  // iterator
  unsigned width = (1 + (SEGMENT.intensity>>4)); // value of 1-16 for each colour
  unsigned index = it % (width*3);

  for (unsigned i = 0; i < SEGLEN; i++, index++) {
    if (index > (width*3)-1) index = 0;

    uint32_t color = color1;
    if (index > (width<<1)-1) color = SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1);
    else if (index > width-1) color = color2;

    SEGMENT.setPixelColor(SEGLEN - i -1, color);
  }
  return FRAMETIME;
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Tricolor chase mode
 * @note : Converted from WLED Effects "mode_tricolor_chase"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chase_TriColour(void) {

  return EffectAnim__Base_Chase_TriColour(SEGCOLOR(2), SEGCOLOR(0));
}
static const char PM_EFFECT_CONFIG__TRICOLOR_CHASE[] PROGMEM = "Chase 3@!,Size;1,2,3;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Running random pixels ("Stream 2")
 *         Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/RandomChase.h
 * @note : Converted from WLED Effects "mode_chase_random"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chase_Random()
{
  if (SEGMENT.call == 0) {
    SEGMENT.step = RGBW32(random8(), random8(), random8(), 0);
    SEGMENT.aux0 = random16();
  }
  unsigned prevSeed = random16_get_seed(); // save seed so we can restore it at the end of the function
  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = effect_start_time / cycleTime;
  uint32_t color = SEGMENT.step;
  random16_set_seed(SEGMENT.aux0);

  for (unsigned i = SEGLEN -1; i > 0; i--) {
    uint8_t r = random8(6) != 0 ? (color >> 16 & 0xFF) : random8();
    uint8_t g = random8(6) != 0 ? (color >> 8  & 0xFF) : random8();
    uint8_t b = random8(6) != 0 ? (color       & 0xFF) : random8();
    color = RGBW32(r, g, b, 0);
    SEGMENT.setPixelColor(i, color);
    if (i == SEGLEN -1U && SEGMENT.aux1 != (it & 0xFFFFU)) { //new first color in next frame
      SEGMENT.step = color;
      SEGMENT.aux0 = random16_get_seed();
    }
  }

  SEGMENT.aux1 = it & 0xFFFF;

  random16_set_seed(prevSeed); // restore original seed so other effects can use "random" PRNG
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__CHASE_RANDOM[] PROGMEM = "Stream 2@!;;";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Exploding fireworks effect
 * @note : Converted from WLED Effect
 * @note : mode_breath
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Breath()
{

  unsigned var = 0;
  unsigned counter = (effect_start_time * ((SEGMENT.speed >> 3) +10)) & 0xFFFFU;
  counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
  if (counter < 16384) {
    if (counter > 8192) counter = 8192 - (counter - 8192);
    var = sin16_t(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
  }
  
  uint8_t lum = 30 + var;
  for(unsigned i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.setPixelColor(i, ColourBlend(SEGCOLOR(1), SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON), lum) );
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__BREATH[] PROGMEM = "Breath@!;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Fade
 * @note : mode_fade
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fade()
{
  uint16_t counter = (effect_start_time * ((SEGMENT.speed >> 3) +10));
  uint8_t lum = triwave16(counter) >> 8;

  for(unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, ColourBlend(SEGCOLOR_U32(1), SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON), lum) );
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__FADE[] PROGMEM = "Fade@!;!,!;!;01";


/*******************************************************************************************************************************************************************************************************************
 * @description : Exploding fireworks
 * @note : Converted from WLED Effects "mode_fireworks"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fireworks()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const uint16_t width  = SEGMENT.is2D() ? SEG_W : SEGLEN;
  const uint16_t height = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.aux0 = UINT16_MAX;
    SEGMENT.aux1 = UINT16_MAX;
  }
  SEGMENT.fade_out(128);

  uint8_t x = SEGMENT.aux0%width, y = SEGMENT.aux0/width; // 2D coordinates stored in upper and lower byte
  if (!SEGMENT.step) {
    // fireworks mode (blur flares)
    bool valid1 = (SEGMENT.aux0 < width*height);
    bool valid2 = (SEGMENT.aux1 < width*height);
    uint32_t sv1 = 0, sv2 = 0;
    if (valid1) sv1 = SEGMENT.is2D() ? SEGMENT.getPixelColorXY(x, y) : SEGMENT.getPixelColor(SEGMENT.aux0); // get spark color
    if (valid2) sv2 = SEGMENT.is2D() ? SEGMENT.getPixelColorXY(x, y) : SEGMENT.getPixelColor(SEGMENT.aux1);
    SEGMENT.blur(16); // used in mode_rain()
    if (valid1) { if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(x, y, sv1); else SEGMENT.setPixelColor(SEGMENT.aux0, sv1); } // restore spark color after blur
    if (valid2) { if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(x, y, sv2); else SEGMENT.setPixelColor(SEGMENT.aux1, sv2); } // restore old spark color after blur
  }

  for (int i=0; i<max(1, width/20); i++) {
    if (hw_random8(129 - (SEGMENT.intensity >> 1)) == 0) {
      uint16_t index = hw_random16(width*height);
      x = index % width;
      y = index / width;
      uint32_t col = SEGMENT.color_from_palette(hw_random8(), false, false, 0);
      if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(x, y, col);
      else                SEGMENT.setPixelColor(index, col);
      SEGMENT.aux1 = SEGMENT.aux0;  // old spark
      SEGMENT.aux0 = index;        // remember where spark occurred
    }
  }
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__FIREWORKS[] PROGMEM = "Fireworks@,Frequency;!,!;!;12;ix=137," EFFECT_CONFIG_DEFAULT_OPTION__PALETTE_INDEX_CTR ""; // ix(itensity)=192, pal=randomise01


/*******************************************************************************************************************************************************************************************************************
 * @description : Fireworks in starburst effect
 *         Based on the video: https://www.reddit.com/r/arduino/comments/c3sd46/i_made_this_fireworks_effect_for_my_led_strips/
           Speed sets frequency of new starbursts, intensity is the intensity of the burst
 * @note : Converted from WLED Effects "mode_fireworks"
 ********************************************************************************************************************************************************************************************************************/
#ifdef ESP8266
  #define STARBURST_MAX_FRAG   8 //52 bytes / star
#else
  #define STARBURST_MAX_FRAG  10 //60 bytes / star
#endif
//each needs 20+STARBURST_MAX_FRAG*4 bytes
typedef struct particle {
  CRGB     color;
  uint32_t birth  =0;
  uint32_t last   =0;
  float    vel    =0;
  uint16_t pos    =-1;
  float    fragment[STARBURST_MAX_FRAG];
} star;
uint16_t mAnimatorLight::EffectAnim__Fireworks_Starburst()
{

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned maxData = FAIR_DATA_PER_SEG; //ESP8266: 256 ESP32: 640
  unsigned segs = getActiveSegmentsNum();
  if (segs <= (getMaxSegments() /2)) maxData *= 2; //ESP8266: 512 if <= 8 segs ESP32: 1280 if <= 16 segs
  if (segs <= (getMaxSegments() /4)) maxData *= 2; //ESP8266: 1024 if <= 4 segs ESP32: 2560 if <= 8 segs
  unsigned maxStars = maxData / sizeof(star); //ESP8266: max. 4/9/19 stars/seg, ESP32: max. 10/21/42 stars/seg

  unsigned numStars = 1 + (SEGLEN >> 3);
  if (numStars > maxStars) numStars = maxStars;
  unsigned dataSize = sizeof(star) * numStars;

  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  uint32_t it = effect_start_time;

  star* stars = reinterpret_cast<star*>(SEGMENT.data);

  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time

  for (unsigned j = 0; j < numStars; j++)
  {
    // speed to adjust chance of a burst, max is nearly always.
    if (hw_random8((144-(SEGMENT.speed >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.
      unsigned startPos = hw_random16(SEGLEN-1);
      float multiplier = (float)(hw_random8())/255.0f * 1.0f;

      stars[j].color = CRGB(SEGMENT.color_wheel(hw_random8()));
      stars[j].pos = startPos;
      stars[j].vel = maxSpeed * (float)(hw_random8())/255.0f * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = hw_random8(3,6 + (SEGMENT.intensity >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1));

  for (unsigned j=0; j<numStars; j++)
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
      c = CRGB(color_blend(WHITE, RGBW32(c.r,c.g,c.b,0), uint8_t(254.5f*((age / particleIgnition)))));
    } else {
      // Figure out how much to fade and shrink the star based on
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = CRGB(SEGCOLOR(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        c = CRGB(color_blend(RGBW32(c.r,c.g,c.b,0), SEGCOLOR(1), uint8_t(254.5f*fade)));
      }
    }

    float particleSize = (1.0f - fade) * 2.0f;

    for (size_t index=0; index < STARBURST_MAX_FRAG*2; index++) {
      bool mirrored = index & 0x1;
      unsigned i = index >> 1;
      if (stars[j].fragment[i] > 0) {
        float loc = stars[j].fragment[i];
        if (mirrored) loc -= (loc-stars[j].pos)*2;
        unsigned start = loc - particleSize;
        unsigned end = loc + particleSize;
        if (start < 0) start = 0;
        if (start == end) end++;
        if (end > SEGLEN) end = SEGLEN;
        for (unsigned p = start; p < end; p++) {
          SEGMENT.setPixelColor(p, c);
        }
      }
    }
  }
  return FRAMETIME;
  
}
#undef STARBURST_MAX_FRAG
static const char PM_EFFECT_CONFIG__STARBURST[] PROGMEM = "Fireworks Starburst@Chance,Fragments,,,,,,,Overlay;,!;!;;pal=95,m12=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : Exploding fireworks effect
 *         adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 *         adapted for 2D WLED by blazoncek (Blaz Kristan (AKA blazoncek))
 * @note : Converted from WLED Effects "mode_exploding_fireworks"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Exploding_Fireworks()
{

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const int cols = SEGMENT.is2D() ? SEG_W : 1;
  const int rows = SEGMENT.is2D() ? SEG_H : SEGLEN;

  //allocate segment data
  unsigned maxData = FAIR_DATA_PER_SEG; //ESP8266: 256 ESP32: 640
  unsigned segs = getActiveSegmentsNum();
  if (segs <= (getMaxSegments() /2)) maxData *= 2; //ESP8266: 512 if <= 8 segs ESP32: 1280 if <= 16 segs
  if (segs <= (getMaxSegments() /4)) maxData *= 2; //ESP8266: 1024 if <= 4 segs ESP32: 2560 if <= 8 segs
  int maxSparks = maxData / sizeof(spark); //ESP8266: max. 21/42/85 sparks/seg, ESP32: max. 53/106/213 sparks/seg

  unsigned numSparks = min(5 + ((rows*cols) >> 1), maxSparks);
  unsigned dataSize = sizeof(spark) * numSparks;
  if (!SEGMENT.allocateData(dataSize + sizeof(float))) return EFFECT_DEFAULT(); //allocation failed
  float *dying_gravity = reinterpret_cast<float*>(SEGMENT.data + dataSize);

  if (dataSize != SEGMENT.aux1) { //reset to flare if sparks were reallocated (it may be good idea to reset segment if bounds change)
    *dying_gravity = 0.0f;
    SEGMENT.aux0 = 0;
    SEGMENT.aux1 = dataSize;
  }

  SEGMENT.fade_out(252);

  Spark* sparks = reinterpret_cast<Spark*>(SEGMENT.data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004f - (SEGMENT.speed/800000.0f); // m/s/s
  gravity *= rows;

  if (SEGMENT.aux0 < 2) { //FLARE
    if (SEGMENT.aux0 == 0) { //init flare
      flare->pos = 0;
      flare->posX = SEGMENT.is2D() ? hw_random16(2,cols-3) : (SEGMENT.intensity > hw_random8()); // will enable random firing side on 1D
      unsigned peakHeight = 75 + hw_random8(180); //0-255
      peakHeight = (peakHeight * (rows -1)) >> 8;
      flare->vel = sqrtf(-2.0f * gravity * peakHeight);
      flare->velX = SEGMENT.is2D() ? (hw_random8(9)-4)/64.0f : 0; // no X velocity on 1D
      flare->col = 255; //brightness
      SEGMENT.aux0 = 1;
    }

    // launch
    if (flare->vel > 12 * gravity) {
      // flare
      if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(unsigned(flare->posX), rows - uint16_t(flare->pos) - 1, flare->col, flare->col, flare->col);
      else                SEGMENT.setPixelColor((flare->posX > 0.0f) ? rows - int(flare->pos) - 1 : int(flare->pos), flare->col, flare->col, flare->col);
      flare->pos  += flare->vel;
      flare->pos  = constrain(flare->pos, 0, rows-1);
      if (SEGMENT.is2D()) {
        flare->posX += flare->velX;
        flare->posX = constrain(flare->posX, 0, cols-1);
      }
      flare->vel  += gravity;
      flare->col  -= 2;
    } else {
      SEGMENT.aux0 = 2;  // ready to explode
    }
  } else if (SEGMENT.aux0 < 4) {
    /*
     * Explode!
     *
     * Explosion happens where the flare ended.
     * Size is proportional to the height.
     */
    unsigned nSparks = flare->pos + hw_random8(4);
    nSparks = std::max(nSparks, 4U);  // This is not a standard constrain; numSparks is not guaranteed to be at least 4
    nSparks = std::min(nSparks, numSparks);

    // initialize sparks
    if (SEGMENT.aux0 == 2) {
      for (unsigned i = 1; i < nSparks; i++) {
        sparks[i].pos  = flare->pos;
        sparks[i].posX = flare->posX;
        sparks[i].vel  = (float(hw_random16(20001)) / 10000.0f) - 0.9f; // from -0.9 to 1.1
        sparks[i].vel *= rows<32 ? 0.5f : 1; // reduce velocity for smaller strips
        sparks[i].velX = SEGMENT.is2D() ? (float(hw_random16(20001)) / 10000.0f) - 1.0f : 0; // from -1 to 1
        sparks[i].col  = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright
        //sparks[i].col = constrain(sparks[i].col, 0, 345);
        sparks[i].colIndex = hw_random8();
        sparks[i].vel  *= flare->pos/rows; // proportional to height
        sparks[i].velX *= SEGMENT.is2D() ? flare->posX/cols : 0; // proportional to width
        sparks[i].vel  *= -gravity *50;
      }
      //sparks[1].col = 345; // this will be our known spark
      *dying_gravity = gravity/2;
      SEGMENT.aux0 = 3;
    }

    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (unsigned i = 1; i < nSparks; i++) {
        sparks[i].pos  += sparks[i].vel;
        sparks[i].posX += sparks[i].velX;
        sparks[i].vel  += *dying_gravity;
        sparks[i].velX += SEGMENT.is2D() ? *dying_gravity : 0;
        if (sparks[i].col > 3) sparks[i].col -= 4;

        if (sparks[i].pos > 0 && sparks[i].pos < rows) {
          if (SEGMENT.is2D() && !(sparks[i].posX >= 0 && sparks[i].posX < cols)) continue;
          unsigned prog = sparks[i].col;
          uint32_t spColor = (SEGMENT.palette_id) ? SEGMENT.color_wheel(sparks[i].colIndex) : SEGCOLOR(0);
          CRGBW c = BLACK; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = color_blend(spColor, WHITE, uint8_t((prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = color_blend(BLACK, spColor, uint8_t(prog - 45));
            unsigned cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(int(sparks[i].posX), rows - int(sparks[i].pos) - 1, c);
          else                SEGMENT.setPixelColor(int(sparks[i].posX) ? rows - int(sparks[i].pos) - 1 : int(sparks[i].pos), c);
        }
      }
      if (SEGMENT.check3) SEGMENT.blur(16);
      *dying_gravity *= .8f; // as sparks burn out they fall slower
    } else {
      SEGMENT.aux0 = 6 + hw_random8(10); //wait for this many frames
    }
  } else {
    SEGMENT.aux0--;
    if (SEGMENT.aux0 < 4) {
      SEGMENT.aux0 = 0; //back to flare
    }
  }

  return FRAMETIME;

}
#undef MAX_SPARKS
static const char PM_EFFECT_CONFIG__EXPLODING_FIREWORKS[] PROGMEM = "Fireworks 1D@Gravity,Firing side;!,!;!;12;pal=10,ix=128";


/*******************************************************************************************************************************************************************************************************************
 * @description : Modified version of exploding fireworks that skips the launch
 * @note : Modified from WLED Effects
 * 
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Exploding_Fireworks_NoLaunch()
{


  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const int cols = SEGMENT.is2D() ? SEG_W : 1;
  const int rows = SEGMENT.is2D() ? SEG_H : SEGLEN;

  //allocate segment data
  unsigned maxData = FAIR_DATA_PER_SEG; //ESP8266: 256 ESP32: 640
  unsigned segs = getActiveSegmentsNum();
  if (segs <= (getMaxSegments() /2)) maxData *= 2; //ESP8266: 512 if <= 8 segs ESP32: 1280 if <= 16 segs
  if (segs <= (getMaxSegments() /4)) maxData *= 2; //ESP8266: 1024 if <= 4 segs ESP32: 2560 if <= 8 segs
  int maxSparks = maxData / sizeof(spark); //ESP8266: max. 21/42/85 sparks/seg, ESP32: max. 53/106/213 sparks/seg

  unsigned numSparks = min(5 + ((rows*cols) >> 1), maxSparks);
  unsigned dataSize = sizeof(spark) * numSparks;
  if (!SEGMENT.allocateData(dataSize + sizeof(float))) return EFFECT_DEFAULT(); //allocation failed
  float *dying_gravity = reinterpret_cast<float*>(SEGMENT.data + dataSize);

  if (dataSize != SEGMENT.aux1) { //reset to flare if sparks were reallocated (it may be good idea to reset segment if bounds change)
    *dying_gravity = 0.0f;
    SEGMENT.aux0 = 0;
    SEGMENT.aux1 = dataSize;
  }

  SEGMENT.fade_out(252);

  Spark* sparks = reinterpret_cast<Spark*>(SEGMENT.data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004f - (SEGMENT.speed/800000.0f); // m/s/s
  gravity *= rows;

  if (SEGMENT.aux0 < 2) { //FLARE
    
    // Iinit flare
    flare->pos = 0;
    flare->posX = SEGMENT.is2D() ? hw_random16(2,cols-3) : (SEGMENT.intensity > hw_random8()); // will enable random firing side on 1D
    unsigned peakHeight = 75 + hw_random8(180); //0-255
    peakHeight = (peakHeight * (rows -1)) >> 8;
    flare->vel = sqrtf(-2.0f * gravity * peakHeight);
    flare->velX = SEGMENT.is2D() ? (hw_random8(9)-4)/64.0f : 0; // no X velocity on 1D
    flare->col = 255; //brightness
    SEGMENT.aux0 = 1;
    
    // Skipping Launch
    flare->pos  += flare->vel;
    flare->pos  = constrain(flare->pos, 0, rows-1);
    flare->pos = random(0,SEGLEN); // make the start of the explosion random as we have skipped flare
    if (SEGMENT.is2D()) {
      flare->posX += flare->velX;
      flare->posX = constrain(flare->posX, 0, cols-1);
    }
    flare->vel  += gravity;
    flare->col  -= 2;

    // Make sure we will now explode immediately
    SEGMENT.aux0 = 2;  // ready to explode
  
  } // END of flare, which is modified to skip any launch
    
  // Immediately run explosion in this frame  // } else 
  if (SEGMENT.aux0 < 4) {
    /*
     * Explode!
     *
     * Explosion happens where the flare ended.
     * Size is proportional to the height.
     */
    unsigned nSparks = flare->pos + hw_random8(4);
    nSparks = std::max(nSparks, 4U);  // This is not a standard constrain; numSparks is not guaranteed to be at least 4
    nSparks = std::min(nSparks, numSparks);

    // initialize sparks
    if (SEGMENT.aux0 == 2) {
      for (unsigned i = 1; i < nSparks; i++) {
        sparks[i].pos  = flare->pos;
        sparks[i].posX = flare->posX;
        sparks[i].vel  = (float(hw_random16(20001)) / 10000.0f) - 0.9f; // from -0.9 to 1.1
        sparks[i].vel *= rows<32 ? 0.5f : 1; // reduce velocity for smaller strips
        sparks[i].velX = SEGMENT.is2D() ? (float(hw_random16(20001)) / 10000.0f) - 1.0f : 0; // from -1 to 1
        sparks[i].col  = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright
        //sparks[i].col = constrain(sparks[i].col, 0, 345);
        sparks[i].colIndex = hw_random8();
        sparks[i].vel  *= flare->pos/rows; // proportional to height
        sparks[i].velX *= SEGMENT.is2D() ? flare->posX/cols : 0; // proportional to width
        sparks[i].vel  *= -gravity *50;
      }
      //sparks[1].col = 345; // this will be our known spark
      *dying_gravity = gravity/2;
      SEGMENT.aux0 = 3;
    }

    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (unsigned i = 1; i < nSparks; i++) {
        sparks[i].pos  += sparks[i].vel;
        sparks[i].posX += sparks[i].velX;
        sparks[i].vel  += *dying_gravity;
        sparks[i].velX += SEGMENT.is2D() ? *dying_gravity : 0;
        if (sparks[i].col > 3) sparks[i].col -= 4;

        if (sparks[i].pos > 0 && sparks[i].pos < rows) {
          if (SEGMENT.is2D() && !(sparks[i].posX >= 0 && sparks[i].posX < cols)) continue;
          unsigned prog = sparks[i].col;
          uint32_t spColor = (SEGMENT.palette_id) ? SEGMENT.color_wheel(sparks[i].colIndex) : SEGCOLOR(0);
          CRGBW c = BLACK; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = color_blend(spColor, WHITE, uint8_t((prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = color_blend(BLACK, spColor, uint8_t(prog - 45));
            unsigned cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(int(sparks[i].posX), rows - int(sparks[i].pos) - 1, c);
          else                SEGMENT.setPixelColor(int(sparks[i].posX) ? rows - int(sparks[i].pos) - 1 : int(sparks[i].pos), c);
        }
      }
      if (SEGMENT.check3) SEGMENT.blur(16);
      *dying_gravity *= .8f; // as sparks burn out they fall slower
    } else {
      SEGMENT.aux0 = 6 + hw_random8(10); //wait for this many frames
    }
  } else {
    SEGMENT.aux0--;
    if (SEGMENT.aux0 < 4) {
      SEGMENT.aux0 = 0; //back to flare
    }
  }

  return FRAMETIME;

}
static const char PM_EFFECT_CONFIG__EXPLODING_FIREWORKS_NOLAUNCH[] PROGMEM = "Fireworks Flareless@Gravity,Firing side;!,!;!;12;pal=10,ix=128";


/*******************************************************************************************************************************************************************************************************************
 * @description : Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 * @note : Converted from WLED Effects "mode_rain"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Rain()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const unsigned width  = SEG_W;
  const unsigned height = SEG_H;
  SEGMENT.step += FRAMETIME;
  if (SEGMENT.call && SEGMENT.step > SPEED_FORMULA_L) {
    SEGMENT.step = 1;
    if (SEGMENT.is2D()) {
      //uint32_t ctemp[width];
      //for (int i = 0; i<width; i++) ctemp[i] = SEGMENT.getPixelColorXY(i, height-1);
      SEGMENT.move(6, 1, true);  // move all pixels down
      //for (int i = 0; i<width; i++) SEGMENT.setPixelColorXY(i, 0, ctemp[i]); // wrap around
      SEGMENT.aux0 = (SEGMENT.aux0 % width) + (SEGMENT.aux0 / width + 1) * width;
      SEGMENT.aux1 = (SEGMENT.aux1 % width) + (SEGMENT.aux1 / width + 1) * width;
    } else {
      //shift all leds left
      uint32_t ctemp = SEGMENT.getPixelColor(0);
      for (unsigned i = 0; i < SEGLEN - 1; i++) {
        SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1));
      }
      SEGMENT.setPixelColor(SEGLEN -1, ctemp); // wrap around
      SEGMENT.aux0++;  // increase spark index
      SEGMENT.aux1++;
    }
    if (SEGMENT.aux0 == 0) SEGMENT.aux0 = UINT16_MAX; // reset previous spark position
    if (SEGMENT.aux1 == 0) SEGMENT.aux0 = UINT16_MAX; // reset previous spark position
    if (SEGMENT.aux0 >= width*height) SEGMENT.aux0 = 0;     // ignore
    if (SEGMENT.aux1 >= width*height) SEGMENT.aux1 = 0;
  }
  return EffectAnim__Fireworks();
}
static const char PM_EFFECT_CONFIG__RAIN[] PROGMEM = "Rain@!,Spawning rate;!,!;!;12;ix=128";


/*******************************************************************************************************************************************************************************************************************
 * @description : Tetris or Stacking (falling bricks) Effect
 *         by Blaz Kristan (AKA blazoncek) (https://github.com/blazoncek, https://blaz.at/home)
 * @note : Converted from WLED Effects "mode_tetrix"
 ********************************************************************************************************************************************************************************************************************/
//20 bytes
typedef struct Tetris {
  float    pos;
  float    speed;
  uint8_t  col;   // color index
  uint16_t brick; // brick size in pixels
  uint16_t stack; // stack size in pixels
  uint32_t step;  // 2D-fication of SEGMENT.step (state)
} tetris;

uint16_t mAnimatorLight::EffectAnim__Tetrix(void) {

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned strips = SEGMENT.nrOfVStrips(); // allow running on virtual strips (columns in 2D segment)
  unsigned dataSize = sizeof(tetris);
  if (!SEGMENT.allocateData(dataSize * strips)) return EFFECT_DEFAULT(); //allocation failed
  Tetris* drops = reinterpret_cast<Tetris*>(SEGMENT.data);

  //if (SEGMENT.call == 0) SEGMENT.fill(SEGCOLOR(1));  // will fill entire segment (1D or 2D), then use drop->step = 0 below

  // virtualStrip idea by @ewowi (Ewoud Wijma)
  // requires virtual strip # to be embedded into upper 16 bits of index in setPixelcolor()
  // the following functions will not work on virtual strips: fill(), fade_out(), fadeToBlack(), blur()
  struct virtualStrip {
    static void runStrip(size_t stripNr, Tetris *drop) {
      // initialize dropping on first call or segment full
      if (pSEGMENT.call == 0) {
        drop->stack = 0;                  // reset brick stack size
        drop->step = tkr_anim->effect_start_time + 2000;     // start by fading out strip
        if (pSEGMENT.check1) drop->col = 0;// use only one color from palette
      }

      if (drop->step == 0) {              // init brick
        // speed calculation: a single brick should reach bottom of strip in X seconds
        // if the speed is set to 1 this should take 5s and at 255 it should take 0.25s
        // as this is dependant on SEGLEN it should be taken into account and the fact that effect runs every FRAMETIME s
        int speed = pSEGMENT.speed ? pSEGMENT.speed : tkr_anim->hw_random8(1,255);
        speed = map(speed, 1, 255, 5000, 250); // time taken for full (SEGLEN) drop
        drop->speed = float(tkr_anim->SEGLEN * FRAMETIME) / float(speed); // set speed
        drop->pos   = tkr_anim->SEGLEN;             // start at end of segment (no need to subtract 1)
        if (!pSEGMENT.check1) drop->col = tkr_anim->hw_random8(0,15)<<4;   // limit color choices so there is enough HUE gap
        drop->step  = 1;                  // drop state (0 init, 1 forming, 2 falling)
        drop->brick = (pSEGMENT.intensity ? (pSEGMENT.intensity>>5)+1 : tkr_anim->hw_random8(1,5)) * (1+(tkr_anim->SEGLEN>>6));  // size of brick
      }

      if (drop->step == 1) {              // forming
        if (tkr_anim->hw_random8()>>6) {               // random drop
          drop->step = 2;                 // fall
        }
      }

      if (drop->step == 2) {              // falling
        if (drop->pos > drop->stack) {    // fall until top of stack
          drop->pos -= drop->speed;       // may add gravity as: speed += gravity
          if (int(drop->pos) < int(drop->stack)) drop->pos = drop->stack;
          for (unsigned i = unsigned(drop->pos); i < tkr_anim->SEGLEN; i++) {
            uint32_t col = i < unsigned(drop->pos)+drop->brick ? pSEGMENT.color_from_palette(drop->col, false, false, 0) : pSEGCOLOR(1);
            pSEGMENT.setPixelColor(indexToVStrip(i, stripNr), col);
          }
        } else {                          // we hit bottom
          drop->step = 0;                 // proceed with next brick, go back to init
          drop->stack += drop->brick;     // increase the stack size
          if (drop->stack >= tkr_anim->SEGLEN) drop->step = tkr_anim->effect_start_time + 2000; // fade out stack
        }
      }

      if (drop->step > 2) {               // fade strip
        drop->brick = 0;                  // reset brick size (no more growing)
        if (drop->step > tkr_anim->effect_start_time) {
          // allow fading of virtual strip
          for (unsigned i = 0; i < tkr_anim->SEGLEN; i++) pSEGMENT.blendPixelColor(indexToVStrip(i, stripNr), pSEGCOLOR(1), 25); // 10% blend
        } else {
          drop->stack = 0;                // reset brick stack size
          drop->step = 0;                 // proceed with next brick
          if (pSEGMENT.check1) drop->col += 8;   // gradually increase palette index
        }
      }
    }
  };

  for (unsigned stripNr=0; stripNr<strips; stripNr++)
    virtualStrip::runStrip(stripNr, &drops[stripNr]);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__TETRIX[] PROGMEM = "Tetrix@!,Width,,,,One color;!,!;!;;sx=0,ix=0,pal=11,m12=1";


/*******************************************************************************************************************************************************************************************************************
 * @description : Fire flicker
 * @note : Converted from WLED Effects "mode_fire_flicker"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fire_Flicker(void) {
  uint32_t cycleTime = 40 + (255 - SEGMENT.speed);
  uint32_t it = effect_start_time / cycleTime;
  if (SEGMENT.step == it) return FRAMETIME;

  byte w = (SEGCOLOR(0) >> 24);
  byte r = (SEGCOLOR(0) >> 16);
  byte g = (SEGCOLOR(0) >>  8);
  byte b = (SEGCOLOR(0)      );
  byte lum = (SEGMENT.palette_id == 0) ? MAX(w, MAX(r, MAX(g, b))) : 255;
  lum /= (((256-SEGMENT.intensity)/16)+1);
  for (unsigned i = 0; i < SEGLEN; i++) {
    byte flicker = hw_random8(lum);
    if (SEGMENT.palette_id == 0) {
      SEGMENT.setPixelColor(i, MAX(r - flicker, 0), MAX(g - flicker, 0), MAX(b - flicker, 0), MAX(w - flicker, 0));
    } else {
      SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0, 255 - flicker));
    }
  }

  SEGMENT.step = it;
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__FIRE_FLICKER[] PROGMEM = "Fire Flicker@!,!;!;!;01";


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Sparkle/Twinkle
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 * @note : Converted from WLED Effects "mode_sparkle"
 *******************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sparkle()
{
  if (!SEGMENT.check2) for(unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }
  uint32_t cycleTime = 10 + (255 - SEGMENT.speed)*2;
  uint32_t it = effect_start_time / cycleTime;
  if (it != SEGMENT.step)
  {
    SEGMENT.aux0 = hw_random16(SEGLEN); // aux0 stores the random led index
    SEGMENT.step = it;
  }

  SEGMENT.setPixelColor(SEGMENT.aux0, SEGCOLOR(0));

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__SPARKLE[] PROGMEM = "Sparkle@!,,,,,,,,Overlay;!,!;!;;m12=0";


/*******************************************************************************************************************************************************************************************************************
 * Lights all LEDs in the color. Flashes single col 1 pixels randomly. (List name: Sparkle Dark)
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 * @note : Converted from WLED Effects "mode_flash_sparkle"
 *********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sparkle_Flash() // Firework_Rain
{
  if (!SEGMENT.check2) for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  if (effect_start_time - SEGMENT.aux0 > SEGMENT.step) {
    if(hw_random8((255-SEGMENT.intensity) >> 4) == 0) {
      SEGMENT.setPixelColor(hw_random16(SEGLEN), SEGCOLOR(1)); //flash
    }
    SEGMENT.step = effect_start_time;
    SEGMENT.aux0 = 255-SEGMENT.speed;
  }

  SEGMENT.cycle_time__rate_ms =  FRAMETIME;
  SET_DIRECT_MODE();
  
}
static const char PM_EFFECT_CONFIG__FLASH_SPARKLE[] PROGMEM = "Sparkle Dark@!,!,,,,,,,Overlay;Bg,Fx;!;;m12=0";


/*******************************************************************************************************************************************************************************************************************
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
   Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 * @note : Converted from WLED Effects "mode_hyper_sparkle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sparkle_Hyper() // Firework_Rain
{
  if (!SEGMENT.check2) for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
  }

  if (effect_start_time - SEGMENT.aux0 > SEGMENT.step) {
    if (hw_random8((255-SEGMENT.intensity) >> 4) == 0) {
      int len = max(1, (int)SEGLEN/3);
      for (int i = 0; i < len; i++) {
        SEGMENT.setPixelColor(hw_random16(SEGLEN), SEGCOLOR(1));
      }
    }
    SEGMENT.step = effect_start_time;
    SEGMENT.aux0 = 255-SEGMENT.speed;
  }

  SEGMENT.cycle_time__rate_ms =  FRAMETIME;
  SET_DIRECT_MODE();
  
}
static const char PM_EFFECT_CONFIG__HYPER_SPARKLE[] PROGMEM = "Sparkle+@!,!,,,,,,,Overlay;Bg,Fx;!;;m12=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : A very short twinkle routine with fade-in and dual controls. By Andrew Tuline.
 * @note : Converted from WLED Effects "mode_twinkleup"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle_Up() // Firework_Rain
{
  unsigned prevSeed = random16_get_seed();      // save seed so we can restore it at the end of the function
  random16_set_seed(535);                       // The randomizer needs to be re-set each time through the loop in order for the same 'random' numbers to be the same each time through.

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned ranstart = random8();               // The starting value (aka brightness) for each pixel. Must be consistent each time through the loop for this to work.
    unsigned pixBri = sin8_t(ranstart + 16 * effect_start_time/(256-SEGMENT.speed));
    if (random8() > SEGMENT.intensity) pixBri = 0;
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(random8()+effect_start_time/100, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  random16_set_seed(prevSeed); // restore original seed so other effects can use "random" PRNG
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__TWINKLE_UP[] PROGMEM = "Twinkle Up@!,Intensity;!,!;!;;m12=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : Lights all LEDs up in one random color. Then switches them to the next random color.
 * @note : Converted from WLED Effect "mode_random_color"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Random_Colour()
{
  uint32_t cycleTime = 200 + (255 - SEGMENT.speed)*50;
  uint32_t it = effect_start_time / cycleTime;
  uint32_t rem = effect_start_time % cycleTime;
  unsigned fadedur = (cycleTime * SEGMENT.intensity) >> 8;

  uint32_t fade = 255;
  if (fadedur) {
    fade = (rem * 255) / fadedur;
    if (fade > 255) fade = 255;
  }

  if (SEGMENT.call == 0) {
    SEGMENT.aux0 = hw_random8();
    SEGMENT.step = 2;
  }
  if (it != SEGMENT.step) //new color
  {
    SEGMENT.aux1 = SEGMENT.aux0;
    SEGMENT.aux0 = get_random_wheel_index(SEGMENT.aux0); //aux0 will store our random color wheel index
    SEGMENT.step = it;
  }

  SEGMENT.fill(color_blend(SEGMENT.color_wheel(SEGMENT.aux1), SEGMENT.color_wheel(SEGMENT.aux0), uint8_t(fade)));
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__RANDOM_COLOR[] PROGMEM = "Random Colors@!,Fade time;;!;01";

/*******************************************************************************************************************************************************************************************************************
 * @description : Cycles a rainbow over the entire string of LEDs.
 * @note : Converted from WLED Effect "mode_rainbow_cycle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Rainbow_Cycle()
{

  uint16_t counter = (millis() * ((SEGMENT.speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;
  
  for(unsigned i = 0; i < SEGLEN; i++) {
    //intensity/29 = 0 (1/16) 1 (1/8) 2 (1/4) 3 (1/2) 4 (1) 5 (2) 6 (4) 7 (8) 8 (16)
    uint8_t index = (i * (16 << (SEGMENT.intensity /29)) / SEGLEN) + counter;
    SEGMENT.setPixelColor(i, SEGMENT.color_wheel(index));
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__RAINBOW_CYCLE[] PROGMEM = "Rainbow Cylcle@!,Size;;!";

/*******************************************************************************************************************************************************************************************************************
 * @description : Running lights effect with smooth sine transition base.
 * @note : Converted from WLED Effects "running"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Running(bool saw, bool dual)
{

  unsigned x_scale = SEGMENT.intensity >> 2;
  uint32_t counter = (effect_start_time * SEGMENT.speed) >> 9;

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned a = i*x_scale - counter;
    if (saw) {
      a &= 0xFF;
      if (a < 16)
      {
        a = 192 + a*8;
      } else {
        a = map(a,16,255,64,192);
      }
      a = 255 - a;
    }
    uint8_t s = dual ? sin_gap(a) : sin8_t(a);
    uint32_t ca = color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), s);
    if (dual) {
      unsigned b = (SEGLEN-1-i)*x_scale - counter;
      uint8_t t = sin_gap(b);
      uint32_t cb = color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), t);
      ca = color_blend(ca, cb, uint8_t(127));
    }
    SEGMENT.setPixelColor(i, ca);
  }

  return FRAMETIME;
  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Running lights effect with smooth sine transition.
 * @note : Converted from WLED Effects "mode_running_lights"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Running_Lights()
{
  EffectAnim__Base_Running(false);
}
static const char PM_EFFECT_CONFIG__RUNNING_LIGHTS[] PROGMEM = "Running@!,Wave width;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Running lights effect with sawtooth transition.
 * @note : Converted from WLED Effects "mode_saw"
 *********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Saw()
{
  EffectAnim__Base_Running(true);
}
static const char PM_EFFECT_CONFIG__SAW[] PROGMEM = "Saw@!,Width;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Blink several LEDs in random colors on, reset, repeat.
 *               Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 * @note : Converted from WLED Effects "mode_twinkle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle()
{
  
  SEGMENT.fade_out(224);

  uint32_t cycleTime = 20 + (255 - SEGMENT.speed)*5;
  uint32_t it = effect_start_time / cycleTime;
  if (it != SEGMENT.step)
  {
    unsigned maxOn = map(SEGMENT.intensity, 0, 255, 1, SEGLEN); // make sure at least one LED is on
    if (SEGMENT.aux0 >= maxOn)
    {
      SEGMENT.aux0 = 0;
      SEGMENT.aux1 = hw_random(); //new seed for our PRNG
    }
    SEGMENT.aux0++;
    SEGMENT.step = it;
  }

  unsigned PRNG16 = SEGMENT.aux1;

  for (unsigned i = 0; i < SEGMENT.aux0; i++)
  {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 13849; // next 'random' number
    uint32_t p = (uint32_t)SEGLEN * (uint32_t)PRNG16;
    unsigned j = p >> 16;
    SEGMENT.setPixelColor(j, SEGMENT.color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__TWINKLE[] PROGMEM = "Twinkle@!,!;!,!;!;;m12=0"; //pixels


/*******************************************************************************************************************************************************************************************************************
 * @description: Dissolve function
 * @note : Converted from WLED Effects
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Dissolve(uint32_t color)
{
  unsigned dataSize = (SEGLEN+7) >> 3; //1 bit per LED
  if (!SEGMENT.allocateData(dataSize)) return EffectAnim__Static_Palette(); //allocation failed

  if (SEGMENT.call == 0) {
    memset(SEGMENT.data, 0xFF, dataSize); // start by fading pixels up
    SEGMENT.aux0 = 1;
  }

  for (unsigned j = 0; j <= SEGLEN / 15; j++) {
    if (hw_random8() <= SEGMENT.intensity) {
      for (size_t times = 0; times < 10; times++) { //attempt to spawn a new pixel 10 times
        unsigned i = hw_random16(SEGLEN);
        unsigned index = i >> 3;
        unsigned bitNum = i & 0x07;
        bool fadeUp = bitRead(SEGMENT.data[index], bitNum);
        if (SEGMENT.aux0) { //dissolve to primary/palette
          if (fadeUp) {
            if (color == SEGCOLOR(0)) {
              SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
            } else {
              SEGMENT.setPixelColor(i, color);
            }
            bitWrite(SEGMENT.data[index], bitNum, false);
            break; //only spawn 1 new pixel per frame per 50 LEDs
          }
        } else { //dissolve to secondary
          if (!fadeUp) {
            SEGMENT.setPixelColor(i, SEGCOLOR(1)); break;
            bitWrite(SEGMENT.data[index], bitNum, true);
          }
        }
      }
    }
  }

  if (SEGMENT.step > (255 - SEGMENT.speed) + 15U) {
    SEGMENT.aux0 = !SEGMENT.aux0;
    SEGMENT.step = 0;
    memset(SEGMENT.data, (SEGMENT.aux0 ? 0xFF : 0), dataSize); // switch fading
  } else {
    SEGMENT.step++;
  }

  return FRAMETIME;
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description: Blink several LEDs on and then off
 * @note : Converted from WLED Effects "mode_dissolve"
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Dissolve()
{
  EffectAnim__Base_Dissolve(SEGMENT.check1 ? SEGMENT.color_wheel(hw_random8()) : SEGCOLOR(0));
}
static const char PM_EFFECT_CONFIG__DISSOLVE[] PROGMEM = "Dissolve@Repeat speed,Dissolve speed,,,,Random;!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description: Blink several LEDs on and then off in random colors
 * @note : Converted from WLED Effects "mode_dissolve_random"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Dissolve_Random()
{
  EffectAnim__Base_Dissolve( SEGMENT.color_wheel(hw_random8()) );
}
static const char PM_EFFECT_CONFIG__DISSOLVE_RANDOM[] PROGMEM = "Dissolve Random@Repeat speed,Dissolve speed;,!;!";

/*******************************************************************************************************************************************************************************************************************
 * @description : Android loading circle
 * @note : Converted from WLED Effects "mode_android"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Android()
{
  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }

  if (SEGMENT.aux1 > (SEGMENT.intensity*SEGLEN)/255)
  {
    SEGMENT.aux0 = 1;
  } else
  {
    if (SEGMENT.aux1 < 2) SEGMENT.aux0 = 0;
  }

  unsigned a = SEGMENT.step & 0xFFFFU;

  if (SEGMENT.aux0 == 0)
  {
    if (SEGMENT.call %3 == 1) {a++;}
    else {SEGMENT.aux1++;}
  } else
  {
    a++;
    if (SEGMENT.call %3 != 1) SEGMENT.aux1--;
  }

  if (a >= SEGLEN) a = 0;

  if (a + SEGMENT.aux1 < SEGLEN)
  {
    for (unsigned i = a; i < a+SEGMENT.aux1; i++) {
      SEGMENT.setPixelColor(i, SEGCOLOR(0));
    }
  } else
  {
    for (unsigned i = a; i < SEGLEN; i++) {
      SEGMENT.setPixelColor(i, SEGCOLOR(0));
    }
    for (unsigned i = 0; i < SEGMENT.aux1 - (SEGLEN -a); i++) {
      SEGMENT.setPixelColor(i, SEGCOLOR(0));
    }
  }
  SEGMENT.step = a;

  SEGMENT.cycle_time__rate_ms = 3 + ((8 * (uint32_t)(255 - SEGMENT.speed)) / SEGLEN);
  SET_DIRECT_MODE();

}
static const char PM_EFFECT_CONFIG__ANDROID[] PROGMEM = "Android@!,Width;!,!;!;;m12=1"; //vertical


/*******************************************************************************************************************************************************************************************************************
 * @description : Red - Amber - Green - Blue lights running
 * @note : Converted from WLED Effects "mode_colorful"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__ColourFul()
{

  uint8_t numColors = 4; //3, 4, or 5
  uint32_t cols[9]{0x00FF0000,0x00EEBB00,0x0000EE00,0x000077CC};
  if (SEGMENT.intensity > 160 || SEGMENT.palette_id) { //palette or color
    // If RGBCCT palette, use the first 3 colors
    if (SEGMENT.palette_id == mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) {
      numColors = 5;
      for (size_t i = 0; i < 5; i++) cols[i] = SEGCOLOR_U32(i);
    } 
    else
    {
      uint16_t fac = 80;
      // if (SEGMENT.palette_id == mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__C9__ID) {numColors = 5; fac = 61;} //C9 2 has 5 colors
      for (size_t i = 0; i < numColors; i++) 
      {
        cols[i] = SEGMENT.GetPaletteColour(i*fac, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);  
      }
    }
  } 
  else 
  if (SEGMENT.intensity < 80) //pastel (easter) colors
  {
    cols[0] = 0x00FF8040;
    cols[1] = 0x00E5D241;
    cols[2] = 0x0077FF77;
    cols[3] = 0x0077F0F0;
  }
  for (size_t i = numColors; i < numColors*2 -1U; i++) cols[i] = cols[i-numColors];

  uint32_t cycleTime = 50 + (8 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = effect_start_time / cycleTime;
  if (it != SEGMENT.step)
  {
    if (SEGMENT.speed > 0) SEGMENT.aux0++;
    if (SEGMENT.aux0 >= numColors) SEGMENT.aux0 = 0;
    SEGMENT.step = it;
  }

  for (int i = 0; i < SEGLEN; i+= numColors)
  {
    for (int j = 0; j < numColors; j++) SEGMENT.setPixelColor(i + j, cols[SEGMENT.aux0 + j]);
  }
  
  DIRECT_MODE(FRAMETIME);
  
}
static const char PM_EFFECT_CONFIG__COLORFUL[] PROGMEM = "Colourful@!,Saturation;1,2,3;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Emulates a traffic light.
 * @note : Converted from WLED Effects "mode_traffic_light"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Traffic_Light()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  for (unsigned i=0; i < SEGLEN; i++)
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  uint32_t mdelay = 500;
  for (unsigned i = 0; i < SEGLEN-2 ; i+=3)
  {
    switch (SEGMENT.aux0)
    {
      case 0: SEGMENT.setPixelColor(i, 0x00FF0000); mdelay = 150 + (100 * (uint32_t)(255 - SEGMENT.speed));break;
      case 1: SEGMENT.setPixelColor(i, 0x00FF0000); mdelay = 150 + (20 * (uint32_t)(255 - SEGMENT.speed)); SEGMENT.setPixelColor(i+1, 0x00EECC00); break;
      case 2: SEGMENT.setPixelColor(i+2, 0x0000FF00); mdelay = 150 + (100 * (uint32_t)(255 - SEGMENT.speed));break;
      case 3: SEGMENT.setPixelColor(i+1, 0x00EECC00); mdelay = 150 + (20 * (uint32_t)(255 - SEGMENT.speed));break;
    }
  }

  if (effect_start_time - SEGMENT.step > mdelay)
  {
    SEGMENT.aux0++;
    if (SEGMENT.aux0 == 1 && SEGMENT.intensity > 140) SEGMENT.aux0 = 2; //skip Red + Amber, to get US-style sequence
    if (SEGMENT.aux0 > 3) SEGMENT.aux0 = 0;
    SEGMENT.step = effect_start_time;
  }

  DIRECT_MODE(FRAMETIME);
  
}
static const char PM_EFFECT_CONFIG__TRAFFIC_LIGHT[] PROGMEM = "Traffic Light@!,US style;,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Alternating pixels running function.
 * @note : Converted from WLED Effects "running"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Running(uint32_t color1, uint32_t color2, bool theatre)
{
  int width = (theatre ? 3 : 1) + (SEGMENT.intensity >> 4);  // window
  uint32_t cycleTime = 50 + (255 - SEGMENT.speed);
  uint32_t it = effect_start_time / cycleTime;
  bool usePalette = color1 == SEGCOLOR(0);

  for (unsigned i = 0; i < SEGLEN; i++) {
    uint32_t col = color2;
    if (usePalette) color1 = SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0);
    if (theatre) {
      if ((i % width) == SEGMENT.aux0) col = color1;
    } else {
      int pos = (i % (width<<1));
      if ((pos < SEGMENT.aux0-width) || ((pos >= SEGMENT.aux0) && (pos < SEGMENT.aux0+width))) col = color1;
    }
    SEGMENT.setPixelColor(i,col);
  }

  if (it != SEGMENT.step) {
    SEGMENT.aux0 = (SEGMENT.aux0 +1) % (theatre ? width : (width<<1));
    SEGMENT.step = it;
  }
  DIRECT_MODE(FRAMETIME);
    
}

/*
 * Alternating color/sec pixels running.
 */
uint16_t mAnimatorLight::EffectAnim__Running_Colour(void) {
  return EffectAnim__Base_Running(SEGCOLOR(0), SEGCOLOR(1));
}
static const char PM_EFFECT_CONFIG__RUNNING_COLOR[] PROGMEM = "Running 2@!,Width;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects "mode_running_random"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Running_Random()
{

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = effect_start_time / cycleTime;
  if (SEGMENT.call == 0) SEGMENT.aux0 = hw_random(); // random seed for PRNG on start

  unsigned zoneSize = ((255-SEGMENT.intensity) >> 4) +1;
  uint16_t PRNG16 = SEGMENT.aux0;

  unsigned z = it % zoneSize;
  bool nzone = (!z && it != SEGMENT.aux1);
  for (unsigned i=SEGLEN-1; i > 0; i--) {
    if (nzone || z >= zoneSize) {
      unsigned lastrand = PRNG16 >> 8;
      int16_t diff = 0;
      while (abs(diff) < 42) { // make sure the difference between adjacent colors is big enough
        PRNG16 = (uint16_t)(PRNG16 * 2053) + 13849; // next zone, next 'random' number
        diff = (PRNG16 >> 8) - lastrand;
      }
      if (nzone) {
        SEGMENT.aux0 = PRNG16; // save next starting seed
        nzone = false;
      }
      z = 0;
    }
    SEGMENT.setPixelColor(i, SEGMENT.color_wheel(PRNG16 >> 8));
    z++;
  }

  SEGMENT.aux1 = it;
  DIRECT_MODE(FRAMETIME);
  
}
static const char PM_EFFECT_CONFIG__RUNNING_RANDOM[] PROGMEM = "Stream@!,Zone size;;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Gradient run base function
 * @note : Converted from WLED Effects "gradient_base"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Gradient(bool loading)
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  uint16_t counter = effect_start_time * ((SEGMENT.speed >> 2) + 1);
  uint16_t pp = (counter * SEGLEN) >> 16;
  if (SEGMENT.call == 0) pp = 0;
  int val; //0 = sec 1 = pri
  int brd = 1 + loading ? SEGMENT.intensity/2 : SEGMENT.intensity/4;
  //if (brd < 1) brd = 1;
  int p1 = pp-SEGLEN;
  int p2 = pp+SEGLEN;

  for (int i = 0; i < (int)SEGLEN; i++) {
    if (loading) {
      val = abs(((i>pp) ? p2:pp) - i);
    } else {
      val = min(abs(pp-i),min(abs(p1-i),abs(p2-i)));
    }
    val = (brd > val) ? (val * 255) / brd : 255;
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(0), SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1), uint8_t(val)));
  }

  return FRAMETIME;
  
}

/*******************************************************************************************************************************************************************************************************************
 * @description : Gradient run
 * @note : Converted from WLED Effects "mode_gradient"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Gradient(void) {
  return EffectAnim__Base_Gradient(false);
}
static const char PM_EFFECT_CONFIG__GRADIENT[] PROGMEM = "Gradient@!,Spread;!,!;!;;ix=16";


/*******************************************************************************************************************************************************************************************************************
 * @description : Gradient run with hard transition
 * @note : Converted from WLED Effects "mode_loading"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Loading(void) {
  return EffectAnim__Base_Gradient(true);
}
static const char PM_EFFECT_CONFIG__LOADING[] PROGMEM = "Loading@!,Fade;!,!;!;;ix=16";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects
 * //American Police Light with all LEDs Red and Blue 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

uint16_t mAnimatorLight::EffectAnim__Base_Police(uint32_t color1, uint32_t color2, bool all)
{

  uint16_t counter = millis() * ((SEGMENT.speed >> 2) +1);
  uint16_t idexR = (counter * SEGLEN) >> 16;
  if (idexR >= SEGLEN) idexR = 0;

  uint16_t topindex = SEGLEN >> 1;
  uint16_t idexB = (idexR > topindex) ? idexR - topindex : idexR + topindex;
  if (SEGMENT.call == 0) SEGMENT.aux0 = idexR;
  if (idexB >= SEGLEN) idexB = 0; //otherwise overflow on odd number of LEDs

  if (all) { //different algo, ensuring immediate fill
    if (idexB > idexR) {
      fill(color2);
      for (uint16_t i = idexR; i < idexB; i++) SEGMENT.setPixelColor(i, color1);
    } else {
      fill(color1);
      for (uint16_t i = idexB; i < idexR; i++) SEGMENT.setPixelColor(i, color2);
    } 
  } else { //regular dot-only mode
    uint8_t size = 1 + SEGMENT.intensity >> 3;
    if (size > SEGLEN/2) size = 1+ SEGLEN/2;
    for (uint8_t i=0; i <= size; i++) {
      SEGMENT.setPixelColor(idexR+i, color1);
      SEGMENT.setPixelColor(idexB+i, color2);
    }
    if (SEGMENT.aux0 != idexR) {
      uint8_t gap = (SEGMENT.aux0 < idexR)? idexR - SEGMENT.aux0:SEGLEN - SEGMENT.aux0 + idexR;
      for (uint8_t i = 0; i <= gap ; i++) {
        if ((idexR - i) < 0) idexR = SEGLEN-1 + i;
        if ((idexB - i) < 0) idexB = SEGLEN-1 + i;
        SEGMENT.setPixelColor(idexR-i, color1);
        SEGMENT.setPixelColor(idexB-i, color2);
      }
      SEGMENT.aux0 = idexR;
    }
  }
  
  return FRAMETIME;
  
}


//American Police Light with all LEDs Red and Blue 
uint16_t mAnimatorLight::EffectAnim__Police_All()
{  
  return EffectAnim__Base_Police(RED, BLUE, true);
}
static const char PM_EFFECT_CONFIG__POLICE_ALL[] PROGMEM = "Police All@!,Width;,Bg;0";


//Police Lights Red and Blue 
uint16_t mAnimatorLight::EffectAnim__Police()
{
  fill(SEGCOLOR_U32(1));
  return EffectAnim__Base_Police(RED, BLUE, false);
}
static const char PM_EFFECT_CONFIG__POLICE[] PROGMEM = "Police@!,Width;,Bg;0";


//Police All with custom colors
uint16_t mAnimatorLight::EffectAnim__Two_Areas()
{  
  return EffectAnim__Base_Police(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true);
}
static const char PM_EFFECT_CONFIG__TWO_AREAS[] PROGMEM = "Two Areas@!,Dot size,,,,,,,Overlay;1,2,Bg;!";

/*******************************************************************************************************************************************************************************************************************
 * @description : Two dots running
 * @note : Converted from WLED Effects "mode_two_dots"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Two_Dots()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned delay = 1 + (FRAMETIME<<3) / SEGLEN;  // longer segments should change faster
  uint32_t it = effect_start_time / map(SEGMENT.speed, 0, 255, delay<<4, delay);
  unsigned offset = it % SEGLEN;
  unsigned width = ((SEGLEN*(SEGMENT.intensity+1))>>9); //max width is half the strip
  if (!width) width = 1;
  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(2));
  const uint32_t color1 = SEGCOLOR(0);
  const uint32_t color2 = (SEGCOLOR(1) == SEGCOLOR(2)) ? color1 : SEGCOLOR(1);
  for (unsigned i = 0; i < width; i++) {
    unsigned indexR = (offset + i) % SEGLEN;
    unsigned indexB = (offset + i + (SEGLEN>>1)) % SEGLEN;
    SEGMENT.setPixelColor(indexR, color1);
    SEGMENT.setPixelColor(indexB, color2);
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__TWO_DOTS[] PROGMEM = "Two Dots@!,Dot size,,,,,,,Overlay;1,2,Bg;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Fairy, inspired by https://www.youtube.com/watch?v=zeOw5MZWq24
 * @note : Converted from WLED Effects "mode_fairy"
 ********************************************************************************************************************************************************************************************************************/

//4 bytes
typedef struct Flasher {
  uint16_t stateStart;
  uint8_t stateDur;
  bool stateOn;
} flasher;

#define FLASHERS_PER_ZONE 6
#define MAX_SHIMMER 92

uint16_t mAnimatorLight::EffectAnim__Fairy(void) {

  //set every pixel to a 'random' color from palette (using seed so it doesn't change between frames)
  uint16_t PRNG16 = 5100 + getCurrSegmentId();
  for (unsigned i = 0; i < SEGLEN; i++) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; //next 'random' number
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(PRNG16 >> 8, false, false, 0));
  }

  //amount of flasher pixels depending on intensity (0: none, 255: every LED)
  if (SEGMENT.intensity == 0) return FRAMETIME;
  unsigned flasherDistance = ((255 - SEGMENT.intensity) / 28) +1; //1-10
  unsigned numFlashers = (SEGLEN / flasherDistance) +1;

  unsigned dataSize = sizeof(flasher) * numFlashers;
  if (!SEGMENT.allocateData(dataSize)) return FRAMETIME; //allocation failed
  Flasher* flashers = reinterpret_cast<Flasher*>(SEGMENT.data);
  unsigned now16 = effect_start_time & 0xFFFF;

  //Up to 11 flashers in one brightness zone, afterwards a new zone for every 6 flashers
  unsigned zones = numFlashers/FLASHERS_PER_ZONE;
  if (!zones) zones = 1;
  unsigned flashersInZone = numFlashers/zones;
  uint8_t flasherBri[FLASHERS_PER_ZONE*2 -1];

  for (unsigned z = 0; z < zones; z++) {
    unsigned flasherBriSum = 0;
    unsigned firstFlasher = z*flashersInZone;
    if (z == zones-1) flashersInZone = numFlashers-(flashersInZone*(zones-1));

    for (unsigned f = firstFlasher; f < firstFlasher + flashersInZone; f++) {
      unsigned stateTime = uint16_t(now16 - flashers[f].stateStart);
      //random on/off time reached, switch state
      if (stateTime > flashers[f].stateDur * 10) {
        flashers[f].stateOn = !flashers[f].stateOn;
        if (flashers[f].stateOn) {
          flashers[f].stateDur = 12 + hw_random8(12 + ((255 - SEGMENT.speed) >> 2)); //*10, 250ms to 1250ms
        } else {
          flashers[f].stateDur = 20 + hw_random8(6 + ((255 - SEGMENT.speed) >> 2)); //*10, 250ms to 1250ms
        }
        //flashers[f].stateDur = 51 + hw_random8(2 + ((255 - SEGMENT.speed) >> 1));
        flashers[f].stateStart = now16;
        if (stateTime < 255) {
          flashers[f].stateStart -= 255 -stateTime; //start early to get correct bri
          flashers[f].stateDur += 26 - stateTime/10;
          stateTime = 255 - stateTime;
        } else {
          stateTime = 0;
        }
      }
      if (stateTime > 255) stateTime = 255; //for flasher brightness calculation, fades in first 255 ms of state
      //flasherBri[f - firstFlasher] = (flashers[f].stateOn) ? 255-SEGMENT.gamma8((510 - stateTime) >> 1) : SEGMENT.gamma8((510 - stateTime) >> 1);
      flasherBri[f - firstFlasher] = (flashers[f].stateOn) ? stateTime : 255 - (stateTime >> 0);
      flasherBriSum += flasherBri[f - firstFlasher];
    }
    //dim factor, to create "shimmer" as other pixels get less voltage if a lot of flashers are on
    unsigned avgFlasherBri = flasherBriSum / flashersInZone;
    unsigned globalPeakBri = 255 - ((avgFlasherBri * MAX_SHIMMER) >> 8); //183-255, suitable for 1/5th of LEDs flashers

    for (unsigned f = firstFlasher; f < firstFlasher + flashersInZone; f++) {
      uint8_t bri = (flasherBri[f - firstFlasher] * globalPeakBri) / 255;
      PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; //next 'random' number
      unsigned flasherPos = f*flasherDistance;
      SEGMENT.setPixelColor(flasherPos, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(PRNG16 >> 8, false, false, 0), bri));
      for (unsigned i = flasherPos+1; i < flasherPos+flasherDistance && i < SEGLEN; i++) {
        PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; //next 'random' number
        SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(PRNG16 >> 8, false, false, 0, globalPeakBri));
      }
    }
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__FAIRY[] PROGMEM = "Fairy@!,# of flashers;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Fairytwinkle. Like Colortwinkle, but starting from all lit and not relying on getPixelColor
 *         Warning: Uses 4 bytes of segment data per pixel
 * @note : Converted from WLED Effects "mode_fairytwinkle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fairy_Twinkle(void) {

  unsigned dataSize = sizeof(flasher) * SEGLEN;
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed
  Flasher* flashers = reinterpret_cast<Flasher*>(SEGMENT.data);
  unsigned now16 = effect_start_time & 0xFFFF;
  uint16_t PRNG16 = 5100 + getCurrSegmentId();

  unsigned riseFallTime = 400 + (255-SEGMENT.speed)*3;
  unsigned maxDur = riseFallTime/100 + ((255 - SEGMENT.intensity) >> 2) + 13 + ((255 - SEGMENT.intensity) >> 1);

  for (unsigned f = 0; f < SEGLEN; f++) {
    uint16_t stateTime = now16 - flashers[f].stateStart;
    //random on/off time reached, switch state
    if (stateTime > flashers[f].stateDur * 100) {
      flashers[f].stateOn = !flashers[f].stateOn;
      bool init = !flashers[f].stateDur;
      if (flashers[f].stateOn) {
        flashers[f].stateDur = riseFallTime/100 + ((255 - SEGMENT.intensity) >> 2) + hw_random8(12 + ((255 - SEGMENT.intensity) >> 1)) +1;
      } else {
        flashers[f].stateDur = riseFallTime/100 + hw_random8(3 + ((255 - SEGMENT.speed) >> 6)) +1;
      }
      flashers[f].stateStart = now16;
      stateTime = 0;
      if (init) {
        flashers[f].stateStart -= riseFallTime; //start lit
        flashers[f].stateDur = riseFallTime/100 + hw_random8(12 + ((255 - SEGMENT.intensity) >> 1)) +5; //fire up a little quicker
        stateTime = riseFallTime;
      }
    }
    if (flashers[f].stateOn && flashers[f].stateDur > maxDur) flashers[f].stateDur = maxDur; //react more quickly on intensity change
    if (stateTime > riseFallTime) stateTime = riseFallTime; //for flasher brightness calculation, fades in first 255 ms of state
    unsigned fadeprog = 255 - ((stateTime * 255) / riseFallTime);
    uint8_t flasherBri = (flashers[f].stateOn) ? 255-gamma8(fadeprog) : gamma8(fadeprog);
    unsigned lastR = PRNG16;
    unsigned diff = 0;
    while (diff < 0x4000) { //make sure colors of two adjacent LEDs differ enough
      PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; //next 'random' number
      diff = (PRNG16 > lastR) ? PRNG16 - lastR : lastR - PRNG16;
    }
    SEGMENT.setPixelColor(f, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(PRNG16 >> 8, false, false, 0), flasherBri));
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__FAIRYTWINKLE[] PROGMEM = "Fairytwinkle@!,!;!,!;!;;m12=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : Custom mode by Aircoookie. Color Wipe, but with 3 colors
 * @note : Converted from WLED Effects "mode_tricolor_wipe"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__TriColour_Wipe()
{
  uint32_t cycleTime = 1000 + (255 - SEGMENT.speed)*200;
  uint32_t perc = effect_start_time % cycleTime;
  unsigned prog = (perc * 65535) / cycleTime;
  unsigned ledIndex = (prog * SEGLEN * 3) >> 16;
  unsigned ledOffset = ledIndex;

  for (unsigned i = 0; i < SEGLEN; i++)
  {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 2));
  }

  if(ledIndex < SEGLEN) { //wipe from 0 to 1
    for (unsigned i = 0; i < SEGLEN; i++)
    {
      SEGMENT.setPixelColor(i, (i > ledOffset)? SEGCOLOR(0) : SEGCOLOR(1));
    }
  } else if (ledIndex < SEGLEN*2) { //wipe from 1 to 2
    ledOffset = ledIndex - SEGLEN;
    for (unsigned i = ledOffset +1; i < SEGLEN; i++)
    {
      SEGMENT.setPixelColor(i, SEGCOLOR(1));
    }
  } else //wipe from 2 to 0
  {
    ledOffset = ledIndex - SEGLEN*2;
    for (unsigned i = 0; i <= ledOffset; i++)
    {
      SEGMENT.setPixelColor(i, SEGCOLOR(0));
    }
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__TRICOLOR_WIPE[] PROGMEM = "Tri Wipe@!;1,2,3;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Fades between 3 colors
 *         Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/TriFade.h
 *         Modified by Aircoookie
 * @note : Converted from WLED Effects "mode_tricolor_fade"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fade_TriColour()
{
  unsigned counter = effect_start_time * ((SEGMENT.speed >> 3) +1);
  uint16_t prog = (counter * 768) >> 16;

  uint32_t color1 = 0, color2 = 0;
  unsigned stage = 0;

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
  for (unsigned i = 0; i < SEGLEN; i++) {
    uint32_t color;
    if (stage == 2) {
      color = color_blend(SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), color2, stp);
    } else if (stage == 1) {
      color = color_blend(color1, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 2), stp);
    } else {
      color = color_blend(color1, color2, stp);
    }
    SEGMENT.setPixelColor(i, color);
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__TRICOLOR_FADE[] PROGMEM = "Tri Fade@!;1,2,3;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Running lights in opposite directions.
 *                Idea: Make the gap width controllable with a third slider in the future
 * @note : Converted from WLED Effects "mode_running_dual"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Running_Dual()
{
  return EffectAnim__Base_Running(false, true);
}
static const char PM_EFFECT_CONFIG__RUNNING_DUAL[] PROGMEM = "Running Dual@!,Wave width;L,!,R;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Creates random comets
 *         Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/MultiComet.h
 * @note : Converted from WLED Effects "mode_multi_comet"
 ********************************************************************************************************************************************************************************************************************/
#define MAX_COMETS 8
uint16_t mAnimatorLight::EffectAnim__Multi_Comet()
{
  uint32_t cycleTime = 10 + (uint32_t)(255 - SEGMENT.speed);
  uint32_t it = effect_start_time / cycleTime;
  if (SEGMENT.step == it) return FRAMETIME;
  if (!SEGMENT.allocateData(sizeof(uint16_t) * MAX_COMETS)) return EFFECT_DEFAULT(); //allocation failed

  SEGMENT.fade_out(SEGMENT.intensity/2 + 128);

  uint16_t* comets = reinterpret_cast<uint16_t*>(SEGMENT.data);

  for (unsigned i=0; i < MAX_COMETS; i++) {
    if(comets[i] < SEGLEN) {
      unsigned index = comets[i];
      if (SEGCOLOR(2) != 0)
      {
        SEGMENT.setPixelColor(index, i % 2 ? SEGMENT.color_from_palette(index, true, PALETTE_SOLID_WRAP, 0) : SEGCOLOR(2));
      } else
      {
        SEGMENT.setPixelColor(index, SEGMENT.color_from_palette(index, true, PALETTE_SOLID_WRAP, 0));
      }
      comets[i]++;
    } else {
      if(!hw_random16(SEGLEN)) {
        comets[i] = 0;
      }
    }
  }

  SEGMENT.step = it;
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__MULTI_COMET[] PROGMEM = "Multi Comet@!,Speed;!,!;!;1";


/*******************************************************************************************************************************************************************************************************************
 * @description : Oscillating bars of color, updated with standard framerate
 * @note : Converted from WLED Effects "mode_oscillate"
 ********************************************************************************************************************************************************************************************************************/
typedef struct Oscillator {
  int16_t pos;
  int8_t  size;
  int8_t  dir;
  int8_t  speed;
} oscillator;

uint16_t mAnimatorLight::EffectAnim__Oscillate()
{

  constexpr unsigned numOscillators = 3;
  constexpr unsigned dataSize = sizeof(oscillator) * numOscillators;

  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  Oscillator* oscillators = reinterpret_cast<Oscillator*>(SEGMENT.data);

  if (SEGMENT.call == 0)
  {    
    oscillators[0].pos = SEGLEN/4;     oscillators[0].size = SEGLEN/8;  oscillators[0].dir = 1;    oscillators[0].speed = 1; // oscillators[0] = {(uint16_t)(SEGLEN/4),   (uint8_t)(SEGLEN/8),  1, 1};
    oscillators[0].pos = SEGLEN/4*3;   oscillators[0].size = SEGLEN/8;  oscillators[0].dir = 1;    oscillators[0].speed = 2; // oscillators[1] = {(uint16_t)(SEGLEN/4*3), (uint8_t)(SEGLEN/8),  1, 2};
    oscillators[0].pos = SEGLEN/4*2;   oscillators[0].size = SEGLEN/8;  oscillators[0].dir = -1;   oscillators[0].speed = 1; // oscillators[2] = {(uint16_t)(SEGLEN/4*2), (uint8_t)(SEGLEN/8), -1, 1};    
  }

  uint32_t cycleTime = 20 + (2 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = effect_start_time / cycleTime;

  for (unsigned i = 0; i < numOscillators; i++) {
    // if the counter has increased, move the oscillator by the random step
    if (it != SEGMENT.step) oscillators[i].pos += oscillators[i].dir * oscillators[i].speed;
    oscillators[i].size = SEGLEN/(3+SEGMENT.intensity/8);
    if((oscillators[i].dir == -1) && (oscillators[i].pos <= 0)) {
      oscillators[i].pos = 0;
      oscillators[i].dir = 1;
      // make bigger steps for faster speeds
      oscillators[i].speed = SEGMENT.speed > 100 ? hw_random8(2, 4):hw_random8(1, 3);
    }
    if((oscillators[i].dir == 1) && (oscillators[i].pos >= (SEGLEN - 1))) {
      oscillators[i].pos = SEGLEN - 1;
      oscillators[i].dir = -1;
      oscillators[i].speed = SEGMENT.speed > 100 ? hw_random8(2, 4):hw_random8(1, 3);
    }
  }

  for (unsigned i = 0; i < SEGLEN; i++) {
    uint32_t color = BLACK;
    for (unsigned j = 0; j < numOscillators; j++) {
      if(i >= (unsigned)oscillators[j].pos - oscillators[j].size && i <= oscillators[j].pos + oscillators[j].size) {
        color = (color == BLACK) ? SEGCOLOR(j) : color_blend(color, SEGCOLOR(j), uint8_t(128));
      }
    }
    SEGMENT.setPixelColor(i, color);
  }

  SEGMENT.step = it;
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__OSCILLATE[] PROGMEM = "Oscillate@";


/*******************************************************************************************************************************************************************************************************************
 * @description : Pride2015
 *         Animated, ever-changing rainbows.
 *         by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
 * @note : Converted from WLED Effects "mode_pride_2015"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Pride_2015()
{
  unsigned duration = 10 + SEGMENT.speed;
  unsigned sPseudotime = SEGMENT.step;
  unsigned sHue16 = SEGMENT.aux0;

  uint8_t sat8 = beatsin88_t( 87, 220, 250);
  uint8_t brightdepth = beatsin88_t( 341, 96, 224);
  unsigned brightnessthetainc16 = beatsin88_t( 203, (25 * 256), (40 * 256));
  unsigned msmultiplier = beatsin88_t(147, 23, 60);

  unsigned hue16 = sHue16;//gHue * 256;
  unsigned hueinc16 = beatsin88_t(113, 1, 3000);

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88_t( 400, 5,9);
  unsigned brightnesstheta16 = sPseudotime;

  for (unsigned i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    unsigned b16 = sin16_t( brightnesstheta16  ) + 32768;

    unsigned bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);
    SEGMENT.blendPixelColor(i, newcolor, 64);
  }
  SEGMENT.step = sPseudotime;
  SEGMENT.aux0 = sHue16;

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__PRIDE_2015[] PROGMEM = "Pride@!;;";


/*******************************************************************************************************************************************************************************************************************
 * @description : eight colored dots, weaving in and out of sync with each other
 * @note : Converted from WLED Effects "mode_juggle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Juggle()
{

  if (SEGLEN == 1) return EFFECT_DEFAULT();

  SEGMENT.fadeToBlackBy(192 - (3*SEGMENT.intensity/4));
  CRGB fastled_col;
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    int index = 0 + beatsin88_t((16 + SEGMENT.speed)*(i + 7), 0, SEGLEN -1);
    fastled_col = CRGB(SEGMENT.getPixelColor(index));
    fastled_col |= (SEGMENT.palette_id==0)?CHSV(dothue, 220, 255):CRGB(ColorFromPalette(SEGPALETTE, dothue, 255));
    SEGMENT.setPixelColor(index, fastled_col);
    dothue += 32;
  }
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__JUGGLES[] PROGMEM = "Juggle@!,Trail;;!;;sx=64,ix=128"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_palette"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Palette()
{
  // Set up some compile time constants so that we can handle integer and float based modes using the same code base.
#ifdef ESP8266
  using mathType = int32_t;
  using wideMathType = int64_t;
  using angleType = unsigned;
  constexpr mathType sInt16Scale             = 0x7FFF;
  constexpr mathType maxAngle                = 0x8000;
  constexpr mathType staticRotationScale     = 256;
  constexpr mathType animatedRotationScale   = 1;
  constexpr int16_t (*sinFunction)(uint16_t) = &sin16_t;
  constexpr int16_t (*cosFunction)(uint16_t) = &cos16_t;
#else
  using mathType = float;
  using wideMathType = float;
  using angleType = float;
  constexpr mathType sInt16Scale           = 1.0f;
  constexpr mathType maxAngle              = M_PI / 256.0;
  constexpr mathType staticRotationScale   = 1.0f;
  constexpr mathType animatedRotationScale = M_TWOPI / double(0xFFFF);
  constexpr float (*sinFunction)(float)    = &sin_t;
  constexpr float (*cosFunction)(float)    = &cos_t;
#endif
  const bool isMatrix = isMatrix;
  const int cols = SEG_W;
  const int rows = isMatrix ? SEG_H : getActiveSegmentsNum();

  const int  inputShift           = SEGMENT.speed;
  const int  inputSize            = SEGMENT.intensity;
  const int  inputRotation        = SEGMENT.custom1;
  const bool inputAnimateShift    = SEGMENT.check1;
  const bool inputAnimateRotation = SEGMENT.check2;
  const bool inputAssumeSquare    = SEGMENT.check3;

  const angleType theta = (!inputAnimateRotation) ? ((inputRotation + 128) * maxAngle / staticRotationScale) : (((effect_start_time * ((inputRotation >> 4) +1)) & 0xFFFF) * animatedRotationScale);
  const mathType sinTheta = sinFunction(theta);
  const mathType cosTheta = cosFunction(theta);

  const mathType maxX    = std::max(1, cols-1);
  const mathType maxY    = std::max(1, rows-1);
  // Set up some parameters according to inputAssumeSquare, so that we can handle anamorphic mode using the same code base.
  const mathType maxXIn  =  inputAssumeSquare ? maxX : mathType(1);
  const mathType maxYIn  =  inputAssumeSquare ? maxY : mathType(1);
  const mathType maxXOut = !inputAssumeSquare ? maxX : mathType(1);
  const mathType maxYOut = !inputAssumeSquare ? maxY : mathType(1);
  const mathType centerX = sInt16Scale * maxXOut / mathType(2);
  const mathType centerY = sInt16Scale * maxYOut / mathType(2);
  // The basic idea for this effect is to rotate a rectangle that is filled with the palette along one axis, then map our
  // display to it, to find what color a pixel should have.
  // However, we want a) no areas of solid color (in front of or behind the palette), and b) we want to make use of the full palette.
  // So the rectangle needs to have exactly the right size. That size depends on the rotation.
  // This scale computation here only considers one dimension. You can think of it like the rectangle is always scaled so that
  // the left and right most points always match the left and right side of the display.
  const mathType scale = std::abs(sinTheta) + (std::abs(cosTheta) * maxYOut / maxXOut);
  // 2D simulation:
  // If we are dealing with a 1D setup, we assume that each segment represents one line on a 2-dimensional display.
  // The function is called once per segments, so we need to handle one line at a time.
  const int yFrom = isMatrix ? 0 : getCurrSegmentId();
  const int yTo   = isMatrix ? maxY : yFrom;
  for (int y = yFrom; y <= yTo; ++y) {
    // translate, scale, rotate
    const mathType ytCosTheta = mathType((wideMathType(cosTheta) * wideMathType(y * sInt16Scale - centerY * maxYIn))/wideMathType(maxYIn * scale));
    for (int x = 0; x < cols; ++x) {
      // translate, scale, rotate
      const mathType xtSinTheta = mathType((wideMathType(sinTheta) * wideMathType(x * sInt16Scale - centerX * maxXIn))/wideMathType(maxXIn * scale));
      // Map the pixel coordinate to an imaginary-rectangle-coordinate.
      // The y coordinate doesn't actually matter, as our imaginary rectangle is filled with the palette from left to right,
      // so all points at a given x-coordinate have the same color.
      const mathType sourceX = xtSinTheta + ytCosTheta + centerX;
      // The computation was scaled just right so that the result should always be in range [0, maxXOut], but enforce this anyway
      // to account for imprecision. Then scale it so that the range is [0, 255], which we can use with the palette.
      int colorIndex = (std::min(std::max(sourceX, mathType(0)), maxXOut * sInt16Scale) * 255) / (sInt16Scale * maxXOut);
      // inputSize determines by how much we want to scale the palette:
      // values < 128 display a fraction of a palette,
      // values > 128 display multiple palettes.
      if (inputSize <= 128) {
        colorIndex = (colorIndex * inputSize) / 128;
      } else {
        // Linear function that maps colorIndex 128=>1, 256=>9.
        // With this function every full palette repetition is exactly 16 configuration steps wide.
        // That allows displaying exactly 2 repetitions for example.
        colorIndex = ((inputSize - 112) * colorIndex) / 16;
      }
      // Finally, shift the palette a bit.
      const int paletteOffset = (!inputAnimateShift) ? (inputShift) : (((effect_start_time * ((inputShift >> 3) +1)) & 0xFFFF) >> 8);
      colorIndex -= paletteOffset;
      const uint32_t color = SEGMENT.color_wheel((uint8_t)colorIndex);
      if (isMatrix) {
        SEGMENT.setPixelColorXY(x, y, color);
      } else {
        SEGMENT.setPixelColor(x, color);
      }
    }
  }
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__PALETTE[] PROGMEM = "Palette@Shift,Size,Rotation,,,Animate Shift,Animate Rotation,Anamorphic;;!;12;ix=112,c1=0,o1=1,o2=0,o3=1";


/*******************************************************************************************************************************************************************************************************************
 * @description : ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
 *         This function draws color waves with an ever-changing,
 *         widely-varying set of parameters, using a color palette.
 * @note : Converted from WLED Effects "mode_colorwaves"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__ColourWaves()
{
  unsigned duration = 10 + SEGMENT.speed;
  unsigned sPseudotime = SEGMENT.step;
  unsigned sHue16 = SEGMENT.aux0;

  unsigned brightdepth = beatsin88_t(341, 96, 224);
  unsigned brightnessthetainc16 = beatsin88_t( 203, (25 * 256), (40 * 256));
  unsigned msmultiplier = beatsin88_t(147, 23, 60);

  unsigned hue16 = sHue16;//gHue * 256;
  unsigned hueinc16 = beatsin88_t(113, 60, 300)*SEGMENT.intensity*10/255;  // Use the Intensity Slider for the hues

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88_t(400, 5, 9);
  unsigned brightnesstheta16 = sPseudotime;

  for (unsigned i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;
    unsigned h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    unsigned b16 = sin16_t(brightnesstheta16) + 32768;

    unsigned bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    SEGMENT.blendPixelColor(i, SEGMENT.color_from_palette(hue8, false, PALETTE_SOLID_WRAP, 0, bri8), 128); // 50/50 mix
  }
  SEGMENT.step = sPseudotime;
  SEGMENT.aux0 = sHue16;

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__COLORWAVES[] PROGMEM = "Colour Waves@!,Hue;!;!pal=15";


/*******************************************************************************************************************************************************************************************************************
 * @description : Colored stripes pulsing at a defined Beats-Per-Minute (BPM)
 * @note : Converted from WLED Effects "mode_bpm"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__BPM()
{
  uint32_t stp = (effect_start_time / 20) & 0xFF;
  uint8_t beat = beatsin8_t(SEGMENT.speed, 64, 255);
  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(stp + (i * 2), false, PALETTE_SOLID_WRAP, 0, beat - stp + (i * 10)));
  }
  return FRAMETIME;   
}
static const char PM_EFFECT_CONFIG__BPM[] PROGMEM = "BPM@!;!;!;;sx=64";


/*******************************************************************************************************************************************************************************************************************
 * @description : Based on https://gist.github.com/kriegsman/5408ecd397744ba0393e
 * @note : Converted from WLED Effects "mode_colortwinkle"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle_Colour()
{
  unsigned dataSize = (SEGLEN+7) >> 3; //1 bit per LED
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  CRGBW col, prev;
  fract8 fadeUpAmount = getBrightness()>28 ? 8 + (SEGMENT.speed>>2) : 68-getBrightness();
  fract8 fadeDownAmount = getBrightness()>28 ? 8 + (SEGMENT.speed>>3) : 68-getBrightness();
  for (unsigned i = 0; i < SEGLEN; i++) {
    CRGBW cur = SEGMENT.getPixelColor(i);
    prev = cur;
    unsigned index = i >> 3;
    unsigned  bitNum = i & 0x07;
    bool fadeUp = bitRead(SEGMENT.data[index], bitNum);

    if (fadeUp) {
      CRGBW incrementalColor = SEGMENT.color_fade(cur, fadeUpAmount, true);
      col = SEGMENT.color_add(cur, incrementalColor);

      if (col.r == 255 || col.g == 255 || col.b == 255) {
        bitWrite(SEGMENT.data[index], bitNum, false);
      }

      if (cur == prev) {  //fix "stuck" pixels
        SEGMENT.color_add(col, col);
        SEGMENT.setPixelColor(i, col);
      }
      else SEGMENT.setPixelColor(i, col);
    }
    else {
      col = SEGMENT.color_fade(cur, 255 - fadeDownAmount);
      SEGMENT.setPixelColor(i, col);
    }
  }

  for (unsigned j = 0; j <= SEGLEN / 50; j++) {
    if (hw_random8() <= SEGMENT.intensity) {
      for (unsigned times = 0; times < 5; times++) { //attempt to spawn a new pixel 5 times
        int i = hw_random16(SEGLEN);
        if (SEGMENT.getPixelColor(i) == 0) {
          unsigned index = i >> 3;
          unsigned  bitNum = i & 0x07;
          bitWrite(SEGMENT.data[index], bitNum, true);
          SEGMENT.setPixelColor(i, ColorFromPalette(SEGPALETTE, hw_random8(), 64, NOBLEND));
          break; //only spawn 1 new pixel per frame per 50 LEDs
        }
      }
    }
  }
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__COLORTWINKLE[] PROGMEM = "Colortwinkles@Fade speed,Spawn speed;;!;;m12=0"; //pixels


/*******************************************************************************************************************************************************************************************************************
 * @description : Calm effect, like a lake at night
 * @note : Converted from WLED Effects "mode_lake"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Lake()
{
  unsigned sp = SEGMENT.speed/10;
  int wave1 = beatsin8_t(sp +2, -64,64);
  int wave2 = beatsin8_t(sp +1, -64,64);
  int wave3 = beatsin8_t(sp +2,   0,80);

  for (unsigned i = 0; i < SEGLEN; i++)
  {
    int index = cos8_t((i*15)+ wave1)/2 + cubicwave8((i*23)+ wave2)/2;
    uint8_t lum = (index > wave3) ? index - wave3 : 0;
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, false, 0, lum));
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__LAKE[] PROGMEM = "Lake@!;Fx;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : meteor effect & meteor smooth (merged by @dedehai)
           send a meteor from begining to to the end of the strip with a trail that randomly decays.
           adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
 * @note : Converted from WLED Effects "mode_meteor"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Meteor()
{

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  if (!SEGMENT.allocateData(SEGLEN)) return EFFECT_DEFAULT(); //allocation failed
  const bool meteorSmooth = SEGMENT.check3;
  byte* trail = SEGMENT.data;

  const unsigned meteorSize = 1 + SEGLEN / 20; // 5%
  uint16_t meteorstart;
  if(meteorSmooth) meteorstart = map((SEGMENT.step >> 6 & 0xFF), 0, 255, 0, SEGLEN -1);
  else {
    unsigned counter = effect_start_time * ((SEGMENT.speed >> 2) + 8);
    meteorstart = (counter * SEGLEN) >> 16;
  }

  const int max = SEGMENT.palette_id==5 || !SEGMENT.check1 ? 240 : 255;
  // fade all leds to colors[1] in LEDs one step
  for (unsigned i = 0; i < SEGLEN; i++) {
    uint32_t col;
    if (hw_random8() <= 255 - SEGMENT.intensity) {
      if(meteorSmooth) { 
        int change = trail[i] + 4 - hw_random8(24); //change each time between -20 and +4
        trail[i] = constrain(change, 0, max);
        col = SEGMENT.check1 ? SEGMENT.color_from_palette(i, true, false, 0,  trail[i]) : SEGMENT.color_from_palette(trail[i], false, true, 255);
        }
        else {
        trail[i] = scale8(trail[i], 128 + hw_random8(127));
        int index = trail[i];
        int idx = 255;
        int bri = SEGMENT.palette_id==35 || SEGMENT.palette_id==36 ? 255 : trail[i];
        if (!SEGMENT.check1) {
          idx = 0;
          index = map(i,0,SEGLEN,0,max);
          bri = trail[i];
        }
        col = SEGMENT.color_from_palette(index, false, false, idx, bri);  // full brightness for Fire
      }
      SEGMENT.setPixelColor(i, col);
    }
  }

  // draw meteor
  for (unsigned j = 0; j < meteorSize; j++) {
    unsigned index = (meteorstart + j) % SEGLEN;
    if(meteorSmooth) {
        trail[index] = max;
        uint32_t col = SEGMENT.check1 ? SEGMENT.color_from_palette(index, true, false, 0, trail[index]) : SEGMENT.color_from_palette(trail[index], false, true, 255);
        SEGMENT.setPixelColor(index, col);
    }
    else{
      int idx = 255;
      int i = trail[index] = max;
      if (!SEGMENT.check1) {
        i = map(index,0,SEGLEN,0,max);
        idx = 0;
      }
      uint32_t col = SEGMENT.color_from_palette(i, false, false, idx, 255); // full brightness
      SEGMENT.setPixelColor(index, col);
    }
  }

  SEGMENT.step += SEGMENT.speed +1;
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__METEOR[] PROGMEM = "Meteor@!,Trail,,,,Gradient,,Smooth;;!;1";


/*******************************************************************************************************************************************************************************************************************
 * @description : This function loops over each pixel, calculates the adjusted 'clock' that this pixel should use, and calls
 *         "CalculateOneTwinkle" on each pixel.  It then displays either the twinkle color of the background color, 
 *         whichever is brighter.
 * @note : Converted from WLED Effects "twinklefox_base"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Twinkle_Fox(bool cat)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  // Calculate speed
  if (SEGMENT.speed > 100) SEGMENT.aux0 = 3 + ((255 - SEGMENT.speed) >> 3);
  else SEGMENT.aux0 = 22 + ((100 - SEGMENT.speed) >> 1);

  // Set up the background color, "bg".
  CRGB bg = CRGB(SEGCOLOR(1));
  unsigned bglight = bg.getAverageLight();
  if (bglight > 64) {
    bg.nscale8_video(16); // very bright, so scale to 1/16th
  } else if (bglight > 16) {
    bg.nscale8_video(64); // not that bright, so scale to 1/4th
  } else {
    bg.nscale8_video(86); // dim, scale to 1/3rd.
  }

  unsigned backgroundBrightness = bg.getAverageLight();

  for (unsigned i = 0; i < SEGLEN; i++) {

    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    unsigned myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    unsigned myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((effect_start_time * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    unsigned  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = EffectAnim__Base_Twinkle_Fox_One_Twinkle(myclock30, myunique8, cat);

    unsigned cbright = c.getAverageLight();
    int deltabright = cbright - backgroundBrightness;
    if (deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      SEGMENT.setPixelColor(i, c);
    } else if (deltabright > 0) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      SEGMENT.setPixelColor(i, color_blend(RGBW32(bg.r,bg.g,bg.b,0), RGBW32(c.r,c.g,c.b,0), uint8_t(deltabright * 8)));
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      SEGMENT.setPixelColor(i, bg);
    }
  }
  return FRAMETIME;
  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
 *         TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
 *         Colors are chosen from a palette. Read more about this effect using the link above!
 * @note : Converted from WLED Effects "twinklefox_one_twinkle"
 ********************************************************************************************************************************************************************************************************************/
CRGB mAnimatorLight::EffectAnim__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat)
{
  // Overall twinkle speed (changed)
  unsigned ticks = ms / SEGMENT.aux0;
  unsigned fastcycle8 = uint8_t(ticks);
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8_t(slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);

  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).
  // Default is 5.
  unsigned twinkleDensity = (SEGMENT.intensity >> 5) +1;

  unsigned bright = 0;
  if (((slowcycle8 & 0x0E)/2) < twinkleDensity) {
    unsigned ph = fastcycle8;
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

  unsigned hue = slowcycle8 - salt;
  CRGB c;
  if (bright > 0) {
    c = ColorFromPalette(SEGPALETTE, hue, bright, NOBLEND);
    if (!SEGMENT.check1) {
      // This code takes a pixel, and if its in the 'fading down'
      // part of the cycle, it adjusts the color a little bit like the
      // way that incandescent bulbs fade toward 'red' as they dim.
      if (fastcycle8 >= 128)
      {
        unsigned cooling = (fastcycle8 - 128) >> 4;
        c.g = qsub8(c.g, cooling);
        c.b = qsub8(c.b, cooling * 2);
      }
    }
  } else {
    c = CRGB::Black;
  }
  return c;  
  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_twinklefox"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle_Fox()
{
  return EffectAnim__Base_Twinkle_Fox(false);
}
static const char PM_EFFECT_CONFIG__TWINKLE_FOX[] PROGMEM = "Twinkle SlowIn/SlowOut@!,Twinkle rate,,,,Cool;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_twinklecat"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Twinkle_Cat()
{
  return EffectAnim__Base_Twinkle_Fox(true);
}
static const char PM_EFFECT_CONFIG__TWINKLE_CAT[] PROGMEM = "Twinkle FastIn/SlowOut@!,Twinkle rate,,,,Cool;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_halloween_eyes"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Halloween_Eyes()
{
  enum eyeState : uint8_t {
    initializeOn = 0,
    on,
    blink,
    initializeOff,
    off,

    count
  };
  struct EyeData {
    eyeState state;
    uint8_t color;
    uint16_t startPos;
    // duration + endTime could theoretically be replaced by a single endTime, however we would lose
    // the ability to end the animation early when the user reduces the animation time.
    uint16_t duration;
    uint32_t startTime;
    uint32_t blinkEndTime;
  };

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const unsigned maxWidth = isMatrix ? SEG_W : SEGLEN;
  const unsigned HALLOWEEN_EYE_SPACE = MAX(2, isMatrix ? SEG_W>>4: SEGLEN>>5);
  const unsigned HALLOWEEN_EYE_WIDTH = HALLOWEEN_EYE_SPACE/2;
  unsigned eyeLength = (2*HALLOWEEN_EYE_WIDTH) + HALLOWEEN_EYE_SPACE;
  if (eyeLength >= maxWidth) return EFFECT_DEFAULT(); //bail if segment too short

  if (!SEGMENT.allocateData(sizeof(EyeData))) return EFFECT_DEFAULT(); //allocation failed
  EyeData& data = *reinterpret_cast<EyeData*>(SEGMENT.data);

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1)); //fill background

  data.state = static_cast<eyeState>(data.state % eyeState::count);
  unsigned duration = max(uint16_t{1u}, data.duration);
  const uint32_t elapsedTime = effect_start_time - data.startTime;

  switch (data.state) {
    case eyeState::initializeOn: {
      // initialize the eyes-on state:
      // - select eye position and color
      // - select a duration
      // - immediately switch to eyes on state.

      data.startPos = hw_random16(0, maxWidth - eyeLength - 1);
      data.color = hw_random8();
      if (isMatrix) SEGMENT.offset = hw_random16(SEG_H-1); // a hack: reuse offset since it is not used in matrices
      duration = 128u + hw_random16(SEGMENT.intensity*64u);
      data.duration = duration;
      data.state = eyeState::on;
      [[fallthrough]];
    }
    case eyeState::on: {
      // eyes-on steate:
      // - fade eyes in for some time
      // - keep eyes on until the pre-selected duration is over
      // - randomly switch to the blink (sub-)state, and initialize it with a blink duration (more precisely, a blink end time stamp)
      // - never switch to the blink state if the animation just started or is about to end

      unsigned start2ndEye = data.startPos + HALLOWEEN_EYE_WIDTH + HALLOWEEN_EYE_SPACE;
      // If the user reduces the input while in this state, limit the duration.
      duration = min(duration, (128u + (SEGMENT.intensity * 64u)));

      constexpr uint32_t minimumOnTimeBegin = 1024u;
      constexpr uint32_t minimumOnTimeEnd = 1024u;
      const uint32_t fadeInAnimationState = elapsedTime * uint32_t{256u * 8u} / duration;
      const uint32_t backgroundColor = SEGCOLOR(1);
      const uint32_t eyeColor = SEGMENT.color_from_palette(data.color, false, false, 0);
      uint32_t c = eyeColor;
      if (fadeInAnimationState < 256u) {
        c = color_blend(backgroundColor, eyeColor, uint8_t(fadeInAnimationState));
      } else if (elapsedTime > minimumOnTimeBegin) {
        const uint32_t remainingTime = (elapsedTime >= duration) ? 0u : (duration - elapsedTime);
        if (remainingTime > minimumOnTimeEnd) {
          if (hw_random8() < 4u)
          {
            c = backgroundColor;
            data.state = eyeState::blink;
            data.blinkEndTime = effect_start_time + hw_random8(8, 128);
          }
        }
      }

      if (c != backgroundColor) {
        // render eyes
        for (unsigned i = 0; i < HALLOWEEN_EYE_WIDTH; i++) {
          if (isMatrix) {
            SEGMENT.setPixelColorXY(data.startPos + i, (unsigned)SEGMENT.offset, c);
            SEGMENT.setPixelColorXY(start2ndEye   + i, (unsigned)SEGMENT.offset, c);
          } else {
            SEGMENT.setPixelColor(data.startPos + i, c);
            SEGMENT.setPixelColor(start2ndEye   + i, c);
          }
        }
      }
      break;
    }
    case eyeState::blink: {
      // eyes-on but currently blinking state:
      // - wait until the blink time is over, then switch back to eyes-on

      if (effect_start_time >= data.blinkEndTime) {
        data.state = eyeState::on;
      }
      break;
    }
    case eyeState::initializeOff: {
      // initialize eyes-off state:
      // - select a duration
      // - immediately switch to eyes-off state

      const unsigned eyeOffTimeBase = SEGMENT.speed*128u;
      duration = eyeOffTimeBase + hw_random16(eyeOffTimeBase);
      data.duration = duration;
      data.state = eyeState::off;
      [[fallthrough]];
    }
    case eyeState::off: {
      // eyes-off state:
      // - not much to do here

      // If the user reduces the input while in this state, limit the duration.
      const unsigned eyeOffTimeBase = SEGMENT.speed*128u;
      duration = min(duration, (2u * eyeOffTimeBase));
      break;
    }
    case eyeState::count: {
      // Can't happen, not an actual state.
      data.state = eyeState::initializeOn;
      break;
    }
  }

  if (elapsedTime > duration) {
    // The current state duration is over, switch to the next state.
    switch (data.state) {
      case eyeState::initializeOn:
      case eyeState::on:
      case eyeState::blink:
        data.state = eyeState::initializeOff;
        break;
      case eyeState::initializeOff:
      case eyeState::off:
      case eyeState::count:
      default:
        data.state = eyeState::initializeOn;
        break;
    }
    data.startTime = effect_start_time;
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__HALLOWEEN_EYES[] PROGMEM = "Halloween Eyes@Eye off time,Eye on time,,,,,,,Overlay;!,!;!;12";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "spots_base"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Spots(uint16_t threshold)
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1));

  unsigned maxZones = SEGLEN >> 2;
  unsigned zones = 1 + ((SEGMENT.intensity * maxZones) >> 8);
  unsigned zoneLen = SEGLEN / zones;
  unsigned offset = (SEGLEN - zones * zoneLen) >> 1;

  for (unsigned z = 0; z < zones; z++)
  {
    unsigned pos = offset + z * zoneLen;
    for (unsigned i = 0; i < zoneLen; i++)
    {
      unsigned wave = triwave16((i * 0xFFFF) / zoneLen);
      if (wave > threshold) {
        unsigned index = 0 + pos + i;
        unsigned s = (wave - threshold)*255 / (0xFFFF - threshold);
        SEGMENT.setPixelColor(index, color_blend(SEGMENT.color_from_palette(index, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), uint8_t(255-s)));
      }
    }
  }

  return FRAMETIME;  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : intensity slider sets number of "lights", speed sets LEDs per light
 * @note : Converted from WLED Effects "mode_spots"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Spots()
{
  return EffectAnim__Base_Spots((255 - SEGMENT.speed) << 8);  
}
static const char PM_EFFECT_CONFIG__SPOTS[] PROGMEM = "Spots@Spread,Width,,,,,,,Overlay;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : intensity slider sets number of "lights", LEDs per light fade in and out
 * @note : Converted from WLED Effects "mode_spots_fade"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fade_Spots()
{
  unsigned counter = effect_start_time * ((SEGMENT.speed >> 2) +8);
  unsigned t = triwave16(counter);
  unsigned tr = (t >> 1) + (t >> 2);
  return EffectAnim__Base_Spots(tr);  
}
static const char PM_EFFECT_CONFIG__SPOTS_FADE[] PROGMEM = "Spots Fade@Spread,Width,,,,Repeat Rate (ms),,Overlay;!,!;!"; // 7 sliders + 4 options before first ;



/*******************************************************************************************************************************************************************************************************************
 * @description : utility function that will add random glitter to SEGMENT
 * @note : Converted from WLED Effects "mode_glitter"
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Glitter_Base(uint8_t intensity, uint32_t col) {
  if (intensity > hw_random8()) SEGMENT.setPixelColor(hw_random16(SEGLEN), col);
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Glitter with palette background, inspired by https://gist.github.com/kriegsman/062e10f7f07ba8518af6
 * @note : Converted from WLED Effects "mode_glitter"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Glitter()
{
  if (!SEGMENT.check2) { // use "* Color 1" palette for solid background (replacing "Solid glitter")
    unsigned counter = 0;
    if (SEGMENT.speed != 0) {
      counter = (effect_start_time * ((SEGMENT.speed >> 3) +1)) & 0xFFFF;
      counter = counter >> 8;
    }

    bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && SEGMENT.speed == 0));
    for (unsigned i = 0; i < SEGLEN; i++) {
      unsigned colorIndex = (i * 255 / SEGLEN) - counter;
      if (noWrap) colorIndex = map(colorIndex, 0, 255, 0, 240); //cut off blend at palette "end"
      SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(colorIndex, false, true, 255));
    }
  }
  EffectAnim__Glitter_Base(SEGMENT.intensity, SEGCOLOR(2) ? SEGCOLOR(2) : ULTRAWHITE);
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__GLITTER[] PROGMEM = "Glitter@!,!,,,,,Overlay;,,Glitter color;!;;pal=25,m12=0"; //pixels


/*******************************************************************************************************************************************************************************************************************
 * @description : Solid colour background with glitter (can be replaced by Glitter)
 * @note : Converted from WLED Effects "mode_solid_glitter"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Solid_Glitter()
{
  SEGMENT.fill(SEGCOLOR(0));
  EffectAnim__Glitter_Base(SEGMENT.intensity, SEGCOLOR(2) ? SEGCOLOR(2) : ULTRAWHITE);
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__SOLID_GLITTER[] PROGMEM = "Solid Glitter@,!;Bg,,Glitter color;;;m12=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : POPCORN modified from https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Popcorn.h with palette background, inspired by https://gist.github.com/kriegsman/062e10f7f07ba8518af6
 * @note : Converted from WLED Effects "mode_popcorn"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Popcorn()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  //allocate segment data
  unsigned strips = SEGMENT.nrOfVStrips();  
  uint16_t maxNumPopcorn = 24; 
  unsigned usablePopcorns = maxNumPopcorn;
  if (usablePopcorns * strips * sizeof(spark) > FAIR_DATA_PER_SEG) usablePopcorns = FAIR_DATA_PER_SEG / (strips * sizeof(spark)) + 1; // at least 1 popcorn per vstrip
  unsigned dataSize = sizeof(spark) * usablePopcorns; // on a matrix 64x64 this could consume a little less than 27kB when Bar expansion is used
  if (!SEGMENT.allocateData(dataSize * strips)) return EFFECT_DEFAULT(); //allocation failed

  Spark* popcorn = reinterpret_cast<Spark*>(SEGMENT.data);

  bool hasCol2 = SEGCOLOR(2);
  if (!SEGMENT.check2) SEGMENT.fill(hasCol2 ? BLACK : SEGCOLOR(1));

  struct virtualStrip {
    static void runStrip(uint16_t stripNr, Spark* popcorn, unsigned usablePopcorns) {
      float gravity = -0.0001f - (pSEGMENT.speed/200000.0f); // m/s/s
      gravity *= pSEGLEN;

      unsigned numPopcorn = pSEGMENT.intensity * usablePopcorns / 255;
      if (numPopcorn == 0) numPopcorn = 1;

      for(unsigned i = 0; i < numPopcorn; i++) {
        if (popcorn[i].pos >= 0.0f) { // if kernel is active, update its position
          popcorn[i].pos += popcorn[i].vel;
          popcorn[i].vel += gravity;
        } else { // if kernel is inactive, randomly pop it
          if (tkr_anim->hw_random8() < 2) { // POP!!!
            popcorn[i].pos = 0.01f;

            unsigned peakHeight = 128 + tkr_anim->hw_random8(128); //0-255
            peakHeight = (peakHeight * (pSEGLEN -1)) >> 8;
            popcorn[i].vel = sqrtf(-2.0f * gravity * peakHeight);

            if (pSEGMENT.palette_id)
            {
              popcorn[i].colIndex = tkr_anim->hw_random8();
            } else {
              byte col = tkr_anim->hw_random8(0, NUM_COLORS);
              if (!pSEGCOLOR(2) || !pSEGCOLOR(col)) col = 0;
              popcorn[i].colIndex = col;
            }
          }
        }
        if (popcorn[i].pos >= 0.0f) { // draw now active popcorn (either active before or just popped)
          uint32_t col = pSEGMENT.color_wheel(popcorn[i].colIndex);
          if (!pSEGMENT.palette_id && popcorn[i].colIndex < NUM_COLORS) col = pSEGCOLOR(popcorn[i].colIndex);
          unsigned ledIndex = popcorn[i].pos;
          if (ledIndex < pSEGLEN) pSEGMENT.setPixelColor(indexToVStrip(ledIndex, stripNr), col);
        }
      }
    }
  };

  for (unsigned stripNr=0; stripNr<strips; stripNr++)
    virtualStrip::runStrip(stripNr, &popcorn[stripNr * usablePopcorns], usablePopcorns);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__POPCORN[] PROGMEM = "Popcorn@!,!,,,,,,,Overlay;!,!,!;!;;m12=1"; //bar


/*******************************************************************************************************************************************************************************************************************
 * @description : Plasma Effect adapted from https://github.com/atuline/FastLED-Demos/blob/master/plasma/plasma.ino 
 * @note : Converted from WLED Effects "mode_plasma"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Plasma()
{
  // initialize phases on start
  if (SEGMENT.call == 0) {
    SEGMENT.aux0 = hw_random8(0,2);  // add a bit of randomness
  }
  unsigned thisPhase = beatsin8_t(6+SEGMENT.aux0,-64,64);
  unsigned thatPhase = beatsin8_t(7+SEGMENT.aux0,-64,64);

  for (unsigned i = 0; i < SEGLEN; i++) {   // For each of the LED's in the strand, set color &  brightness based on a wave as follows:
    unsigned colorIndex = cubicwave8((i*(2+ 3*(SEGMENT.speed >> 5))+thisPhase) & 0xFF)/2   // factor=23 // Create a wave and add a phase change and add another wave with its own phase change.
                              + cos8_t((i*(1+ 2*(SEGMENT.speed >> 5))+thatPhase) & 0xFF)/2;  // factor=15 // Hey, you can even change the frequencies if you wish.
    unsigned thisBright = qsub8(colorIndex, beatsin8_t(7,0, (128 - (SEGMENT.intensity>>1))));
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(colorIndex, false, PALETTE_SOLID_WRAP, 0, thisBright));
  }

  return FRAMETIME;  
} 
static const char PM_EFFECT_CONFIG__PLASMA[] PROGMEM = "Plasma@Phase,!;!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Percentage display Intensity values from 0-100 turn on the leds.
 * @note : Converted from WLED Effects "mode_percent"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Percent()
{
	unsigned percent = SEGMENT.intensity;
  percent = constrain(percent, 0, 200);
  unsigned active_leds = (percent < 100) ? roundf(SEGLEN * percent / 100.0f)
                                         : roundf(SEGLEN * (200 - percent) / 100.0f);

  unsigned size = (1 + ((SEGMENT.speed * SEGLEN) >> 11));
  if (SEGMENT.speed == 255) size = 255;

  if (percent <= 100) {
    for (unsigned i = 0; i < SEGLEN; i++) {
    	if (i < SEGMENT.aux1) {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,0,100,0,255), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    	else {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    }
  } else {
    for (unsigned i = 0; i < SEGLEN; i++) {
    	if (i < (SEGLEN - SEGMENT.aux1)) {
        SEGMENT.setPixelColor(i, SEGCOLOR(1));
    	}
    	else {
        if (SEGMENT.check1)
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(map(percent,100,200,255,0), false, false, 0));
        else
          SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    	}
    }
  }

  if(active_leds > SEGMENT.aux1) {  // smooth transition to the target value
    SEGMENT.aux1 += size;
    if (SEGMENT.aux1 > active_leds) SEGMENT.aux1 = active_leds;
  } else if (active_leds < SEGMENT.aux1) {
    if (SEGMENT.aux1 > size) SEGMENT.aux1 -= size; else SEGMENT.aux1 = 0;
    if (SEGMENT.aux1 < active_leds) SEGMENT.aux1 = active_leds;
  }

 	return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__PERCENT[] PROGMEM = "Percent@,% of fill,,,,One color;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description :  "Pacifica"
            Gentle, blue-green ocean waves.
            December 2019, Mark Kriegsman and Mary Corey March.
            For Dan.


            In this animation, there are four "layers" of waves of light.

            Each layer moves independently, and each is scaled separately.

            All four wave layers are added together on top of each other, and then
            another filter is applied that adds "whitecaps" of brightness where the
            waves line up with each other more.  Finally, another pass is taken
            over the led array to 'deepen' (dim) the blues and greens.

            The speed and scale and motion each layer varies slowly within independent
            hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
            with a lot of oddly specific numeric ranges.

            These three custom blue-green color palettes were inspired by the colors found in
            the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7

            Modified for WLED, based on https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino

            Add one layer of waves into the led array
 * @note : Converted from WLED Effects "mode_pacifica"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Pacifica()
{
  uint32_t nowOld = effect_start_time;

  CRGBPalette16 pacifica_palette_1 =
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
  CRGBPalette16 pacifica_palette_2 =
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
  CRGBPalette16 pacifica_palette_3 =
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

  if (SEGMENT.palette_id) {
    pacifica_palette_1 = SEGPALETTE;
    pacifica_palette_2 = SEGPALETTE;
    pacifica_palette_3 = SEGPALETTE;
  }

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  unsigned sCIStart1 = SEGMENT.aux0, sCIStart2 = SEGMENT.aux1, sCIStart3 = SEGMENT.step & 0xFFFF, sCIStart4 = (SEGMENT.step >> 16);
  uint32_t deltams = (FRAMETIME >> 2) + ((FRAMETIME * SEGMENT.speed) >> 7);
  uint64_t deltat = (effect_start_time >> 2) + ((effect_start_time * SEGMENT.speed) >> 7);
  effect_start_time = deltat;

  unsigned speedfactor1 = beatsin16_t(3, 179, 269);
  unsigned speedfactor2 = beatsin16_t(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88_t(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88_t(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88_t(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88_t(257,4,6));
  SEGMENT.aux0 = sCIStart1; SEGMENT.aux1 = sCIStart2;
  SEGMENT.step = (sCIStart4 << 16) | (sCIStart3 & 0xFFFF);

  // Clear out the LED array to a dim background blue-green
  //SEGMENT.fill(132618);

  unsigned basethreshold = beatsin8_t( 9, 55, 65);
  unsigned wave = beat8( 7 );

  for (unsigned i = 0; i < SEGLEN; i++) {
    CRGB c = CRGB(2, 6, 10);
    // Render each of four layers, with different scales and speeds, that vary over time
    c += EffectAnim__Pacifica_Base_OneLayer(i, pacifica_palette_1, sCIStart1, beatsin16_t(3, 11 * 256, 14 * 256), beatsin8_t(10, 70, 130), 0-beat16(301));
    c += EffectAnim__Pacifica_Base_OneLayer(i, pacifica_palette_2, sCIStart2, beatsin16_t(4,  6 * 256,  9 * 256), beatsin8_t(17, 40,  80),   beat16(401));
    c += EffectAnim__Pacifica_Base_OneLayer(i, pacifica_palette_3, sCIStart3,                         6 * 256 , beatsin8_t(9, 10,38)   , 0-beat16(503));
    c += EffectAnim__Pacifica_Base_OneLayer(i, pacifica_palette_3, sCIStart4,                         5 * 256 , beatsin8_t(8, 10,28)   ,   beat16(601));

    // Add extra 'white' to areas where the four layers of light have lined up brightly
    unsigned threshold = scale8( sin8_t( wave), 20) + basethreshold;
    wave += 7;
    unsigned l = c.getAverageLight();
    if (l > threshold) {
      unsigned overage = l - threshold;
      unsigned overage2 = qadd8(overage, overage);
      c += CRGB(overage, overage2, qadd8(overage2, overage2));
    }

    //deepen the blues and greens
    c.blue  = scale8(c.blue,  145);
    c.green = scale8(c.green, 200);
    c |= CRGB( 2, 5, 7);

    SEGMENT.setPixelColor(i, c);
  }

  effect_start_time = nowOld;
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__PACIFICA[] PROGMEM = "Pacifica@!,Angle;;!;;pal=51";


/*******************************************************************************************************************************************************************************************************************
 * @description : Add one layer of waves into the led array
 * @note : Converted from WLED Effects "pacifica_one_layer"
 ********************************************************************************************************************************************************************************************************************/
CRGB mAnimatorLight::EffectAnim__Pacifica_Base_OneLayer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  unsigned ci = cistart;
  unsigned waveangle = ioff;
  unsigned wavescale_half = (wavescale >> 1) + 20;

  waveangle += ((120 + SEGMENT.intensity) * i); //original 250 * i
  unsigned s16 = sin16_t(waveangle) + 32768;
  unsigned cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += (cs * i);
  unsigned sindex16 = sin16_t(ci) + 32768;
  unsigned sindex8 = scale16(sindex16, 240);
  return CRGB(ColorFromPalette(p, sindex8, bri, LINEARBLEND));
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Mode simulates a gradual sunrise
 * @note : Converted from WLED Effects "mode_sunrise"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sunrise()
{
  
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  //speed 0 - static sun
  //speed 1 - 60: sunrise time in minutes
  //speed 60 - 120 : sunset time in minutes - 60;
  //speed above: "breathing" rise and set
  if (SEGMENT.call == 0 || SEGMENT.speed != SEGMENT.aux0) {
    SEGMENT.step = millis(); //save starting time, millis() because effect_start_time can change from sync
    SEGMENT.aux0 = SEGMENT.speed;
  }

  SEGMENT.fill(BLACK);
  unsigned stage = 0xFFFF;

  uint32_t s10SinceStart = (millis() - SEGMENT.step) /100; //tenths of seconds

  if (SEGMENT.speed > 120) { //quick sunrise and sunset
    unsigned counter = (effect_start_time >> 1) * (((SEGMENT.speed -120) >> 1) +1);
    stage = triwave16(counter);
  } else if (SEGMENT.speed) { //sunrise
    unsigned durMins = SEGMENT.speed;
    if (durMins > 60) durMins -= 60;
    uint32_t s10Target = durMins * 600;
    if (s10SinceStart > s10Target) s10SinceStart = s10Target;
    stage = map(s10SinceStart, 0, s10Target, 0, 0xFFFF);
    if (SEGMENT.speed > 60) stage = 0xFFFF - stage; //sunset
  }

  for (unsigned i = 0; i <= SEGLEN/2; i++)
  {
    //default palette is Fire    
    unsigned wave = triwave16((i * stage) / SEGLEN);
    wave = (wave >> 8) + ((wave * SEGMENT.intensity) >> 15);
    uint32_t c;
    if (wave > 240) { //clipped, full white sun
      c = SEGMENT.color_from_palette( 240, false, true, 255);
    } else { //transition
      c = SEGMENT.color_from_palette(wave, false, true, 255);
    }
    SEGMENT.setPixelColor(i, c);
    SEGMENT.setPixelColor(SEGLEN - i - 1, c);
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__SUNRISE[] PROGMEM = "Sunrise@Time [min],Width;;!;;pal=35,sx=60";


/*******************************************************************************************************************************************************************************************************************
 * @description : Sine waves that have controllable phase change speed, frequency and cutoff. By Andrew Tuline.
 *         SEGMENT.speed ->Speed, SEGMENT.intensity -> Frequency (SEGMENT.fft1 -> Color change, SEGMENT.fft2 -> PWM cutoff)
 * @note : Converted from WLED Effects "mode_sinewave"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sinewave()
{
  unsigned colorIndex = effect_start_time/32; //(256 - SEGMENT.fft1);  // Amount of colour change.

  SEGMENT.step += SEGMENT.speed/16;                   // Speed of animation.
  unsigned freq = SEGMENT.intensity/4; //SEGMENT.fft2/8;                       // Frequency of the signal.

  for (unsigned i = 0; i < SEGLEN; i++) {                 // For each of the LED's in the strand, set a brightness based on a wave as follows:
    uint8_t pixBri = cubicwave8((i*freq)+SEGMENT.step);//qsuba(cubicwave8((i*freq)+SEGMENT.step), (255-SEGMENT.intensity)); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    //setPixCol(i, i*colorIndex/255, pixBri);
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i*colorIndex/255, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__SINEWAVE[] PROGMEM = "Sine@!,Scale;;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Best of both worlds from Palette and Spot effects. By Aircoookie
 * @note : Converted from WLED Effects "mode_flow"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Flow()
{
  
  unsigned counter = 0;
  if (SEGMENT.speed != 0)
  {
    counter = effect_start_time * ((SEGMENT.speed >> 2) +1);
    counter = counter >> 8;
  }

  unsigned maxZones = SEGLEN / 6; //only looks good if each zone has at least 6 LEDs
  unsigned zones = (SEGMENT.intensity * maxZones) >> 8;
  if (zones & 0x01) zones++; //zones must be even
  if (zones < 2) zones = 2;
  unsigned zoneLen = SEGLEN / zones;
  unsigned offset = (SEGLEN - zones * zoneLen) >> 1;

  SEGMENT.fill(SEGMENT.color_from_palette(-counter, false, true, 255));

  for (unsigned z = 0; z < zones; z++)
  {
    unsigned pos = offset + z * zoneLen;
    for (unsigned i = 0; i < zoneLen; i++)
    {
      unsigned colorIndex = (i * 255 / zoneLen) - counter;
      unsigned led = (z & 0x01) ? i : (zoneLen -1) -i;
      if (SEGMENT.reverse) led = (zoneLen -1) -led;
      SEGMENT.setPixelColor(pos + led, SEGMENT.color_from_palette(colorIndex, false, true, 255));
    }
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__FLOW[] PROGMEM = "Flow@!,Zones;;!;;m12=1"; //vertical


/*******************************************************************************************************************************************************************************************************************
 * @description : Speed slider sets amount of LEDs lit, intensity sets unlit
 * @note : Converted from WLED Effects "EFFECT_DEFAULT_pattern"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Palette_Lit_Pattern()
{
  unsigned lit = 1 + SEGMENT.speed;
  unsigned unlit = 1 + SEGMENT.intensity;
  bool drawingLit = true;
  unsigned cnt = 0;

  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, (drawingLit) ? SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0) : SEGCOLOR(1));
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__PALETTE_LIT_PATTERN[] PROGMEM = "Palette Lit Pattern@Fg size,Bg size;Fg,!;!;;pal=19";


/*******************************************************************************************************************************************************************************************************************
 * @description : Speed slider sets amount of LEDs lit
 * @note : Converted from WLED Effects "mode_tri_static_pattern"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__TriSegCol_Lit_Pattern()
{
  unsigned segSize = (SEGMENT.intensity >> 5) +1;
  unsigned currSeg = 0;
  unsigned currSegCount = 0;

  for (unsigned i = 0; i < SEGLEN; i++) {
    if ( currSeg % 3 == 0 ) {
      SEGMENT.setPixelColor(i, SEGCOLOR(0));
    } else if( currSeg % 3 == 1) {
      SEGMENT.setPixelColor(i, SEGCOLOR(1));
    } else {
      SEGMENT.setPixelColor(i, SEGCOLOR(2));
    }
    currSegCount += 1;
    if (currSegCount >= segSize) {
      currSeg +=1;
      currSegCount = 0;
    }
  }

  return FRAMETIME; 

}
static const char PM_EFFECT_CONFIG__TRISEGCOL_LIT_PATTERN[] PROGMEM = "Pattern TriSegLit@,Size;1,2,3,4;";


/*******************************************************************************************************************************************************************************************************************
 * @description : New effect of "Static Palette Mix"
 * 
 * The aim is to make the "Colour and Warm White Stripe" but in a way I can mix them. 
 * 
 * Palette1 ABCD
 * Palette2 abcde
 * 
 * So mix would be AaBbCcDdAe (notice shorter palettes that mix already)
 * 
 * Sliders control lit and width
 * 
 * @note : 
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Palettes_Interleaved_Lit_Pattern()
{
  
  // Get lengths of both palettes
  uint16_t palette1Length = mPaletteI->GetColoursInPalette(SEGMENT.palette_id); // Palette 1
  uint16_t palette2Length = mPaletteI->GetColoursInPalette(SEGMENT.aux0); // Palette 2
  
  uint16_t lit = 1 + SEGMENT.speed; // Lit pattern duration
  uint16_t unlit = 1 + SEGMENT.intensity; // Unlit pattern duration

  bool drawingLit = true; // Flag to track whether lit or unlit
  uint16_t cnt = 0; // Counter for current lit/unlit streak
  
  // Track indices for palettes
  uint16_t index1 = 0; // Palette 1 index
  uint16_t index2 = 0; // Palette 2 index

  for (uint16_t i = 0; i < SEGLEN; i++) {
    uint32_t colour;

    // Interleave palettes
    if (i % 2 == 0) {
      // Use Palette 1 for even indices
      colour = SEGMENT.GetPaletteColour(index1, PALETTE_INDEX_IS_INDEX_IN_PALETTE, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
      index1 = (index1 + 1) % palette1Length; // Wrap Palette 1
    } else {
      // Use Palette 2 for odd indices
      colour = SEGMENT.GetPaletteColour(index2, PALETTE_INDEX_IS_INDEX_IN_PALETTE, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
      index2 = (index2 + 1) % palette2Length; // Wrap Palette 2
    }

    // Set pixel colour based on drawingLit
    SEGMENT.setPixelColor(i, (drawingLit) ? colour : SEGCOLOR_U32(1));

    // Update lit/unlit counter
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__PALETTES_INTERLEAVED_LIT_PATTERN[] PROGMEM = "Palettes InterLit@Fg size,Bg size;Fg,!;!;;pal=19";


/*******************************************************************************************************************************************************************************************************************
 * @description : New effect of "Static Palette Mix"
 * 
 * The aim is to make the "Colour and Warm White Stripe" but in a way I can mix them. 
 * 
 * Palette1 ABCD
 * Palette2 abcde
 * 
 * So mix would be AaBbCcDdAe (notice shorter palettes that mix already)
 * @note : 
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Palettes_Interleaved()
{
  // Get lengths of both palettes
  uint16_t palette1Length = mPaletteI->GetColoursInPalette(SEGMENT.palette_id); // Palette 1
  uint16_t palette2Length = mPaletteI->GetColoursInPalette(SEGMENT.intensity); // Palette 2

  // Track indices for palettes
  uint16_t index1 = 0; // Palette 1 index
  uint16_t index2 = 0; // Palette 2 index

  for (uint16_t i = 0; i < SEGLEN; i++) {
    uint32_t colour;

    // Alternate between Palette 1 and Palette 2
    if (i % 2 == 0) {
      // Use Palette 1 for even indices
      colour = SEGMENT.GetPaletteColour(index1, PALETTE_INDEX_IS_INDEX_IN_PALETTE, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
      index1 = (index1 + 1) % palette1Length; // Wrap Palette 1
    } else {
      // Use Palette 2 for odd indices
      colour = SEGMENT.GetPaletteColour(index2, PALETTE_INDEX_IS_INDEX_IN_PALETTE, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
      index2 = (index2 + 1) % palette2Length; // Wrap Palette 2
    }

    // Set pixel colour
    SEGMENT.setPixelColor(i, colour);
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__PALETTES_INTERLEAVED[] PROGMEM = "Palettes Interleaved@Fg size,Bg size;Fg,!;!;;pal=19";


/*******************************************************************************************************************************************************************************************************************
 * @description : Blink/strobe function
 * @note : Converted from WLED Effects
 * 
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette)
{

  uint32_t cycleTime = (255 - SEGMENT.speed)*20;
  uint32_t onTime = FRAMETIME;
  if (!strobe) onTime += ((cycleTime * SEGMENT.intensity) >> 8);
  cycleTime += FRAMETIME*2;
  uint32_t it = effect_start_time / cycleTime;
  uint32_t rem = effect_start_time % cycleTime;

  bool on = false;
  if (it != SEGMENT.step //new iteration, force on state for one frame, even if set time is too brief
      || rem <= onTime) {
    on = true;
  }

  SEGMENT.step = it; //save previous iteration

  uint32_t color = on ? color1 : color2;
  if (color == color1 && do_palette)
  {
    for (int i = 0; i < SEGLEN; i++) {
      SEGMENT.setPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }
  else
  {
    SEGMENT.fill(color); // alternates between colours passed in unless do_pal is set
  }
  
  return FRAMETIME;
  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Normal blinking. 50% on/off time. Intensity sets duty cycle.
 * @note : Converted from WLED Effects "mode_blink"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Blink()
{
  EffectAnim__Base_Blink(SEGCOLOR(0), SEGCOLOR(1), false, true);  
}
static const char PM_EFFECT_CONFIG__BLINK[] PROGMEM = "Blink@!,Duty cycle;!,!;!;01";


/*******************************************************************************************************************************************************************************************************************
 * @description : Classic Blink effect. Cycling through the rainbow.
 * @note : Converted from WLED Effects "mode_blink_rainbow"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Blink_Rainbow()
{
  EffectAnim__Base_Blink( SEGMENT.color_wheel(SEGMENT.call & 0xFF), SEGCOLOR(1), false, false );  
}
static const char PM_EFFECT_CONFIG__BLINK_RAINBOW[] PROGMEM = "Blink Rainbow@Frequency,Blink duration;!,!;!;01";


/*******************************************************************************************************************************************************************************************************************
 * @description : Classic Strobe effect.
 * @note : Converted from WLED Effects "mode_strobe"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Strobe()
{
  EffectAnim__Base_Blink(SEGCOLOR(0), SEGCOLOR(1), true, true);
}
static const char PM_EFFECT_CONFIG__STROBE[] PROGMEM = "Strobe@!;!,!;!;01";


/*******************************************************************************************************************************************************************************************************************
 * @description : Strobe effect with different strobe count and pause, controlled by speed.
 * @note : Converted from WLED Effects "mode_multi_strobe"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Strobe_Multi()
{
  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 1));
  }

  SEGMENT.aux0 = 50 + 20*(uint16_t)(255-SEGMENT.speed);
  unsigned count = 2 * ((SEGMENT.intensity / 10) + 1);
  if(SEGMENT.aux1 < count) {
    if((SEGMENT.aux1 & 1) == 0) {
      SEGMENT.fill(SEGCOLOR(0));
      SEGMENT.aux0 = 15;
    } else {
      SEGMENT.aux0 = 50;
    }
  }

  if (effect_start_time - SEGMENT.aux0 > SEGMENT.step) {
    SEGMENT.aux1++;
    if (SEGMENT.aux1 > count) SEGMENT.aux1 = 0;
    SEGMENT.step = effect_start_time;
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__MULTI_STROBE[] PROGMEM = "Strobe Multi@!,!;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : 
 * Classic Strobe effect. Cycling through the rainbow.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Strobe_Rainbow()
{
  EffectAnim__Base_Blink( SEGMENT.color_wheel(SEGMENT.call & 0xFF), SEGCOLOR_U32(1), true, false);
}
static const char PM_EFFECT_CONFIG__STROBE_RAINBOW[] PROGMEM = "Strobe Rainbow@!;,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects
 * Cycles all LEDs at once through a rainbow.
 * Note: SEGMENT.intensity < 128 = pastel rainbow, SEGMENT.intensity > 128 = full saturation rainbow
 * @note : mode_rainbow
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Rainbow()
{

  uint16_t counter = (millis() * ((SEGMENT.speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  if (SEGMENT.intensity < 128){
    SEGMENT.fill(ColourBlend(SEGMENT.color_wheel(counter),WHITE,uint8_t(128-SEGMENT.intensity)));
  } else {
    SEGMENT.fill(SEGMENT.color_wheel(counter));
  }

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__RAINBOW[] PROGMEM = "Colorloop@!,Saturation;;!;01";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_lightning"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Lightning()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned ledstart = hw_random16(SEGLEN);               // Determine starting location of flash
  unsigned ledlen = 1 + hw_random16(SEGLEN -ledstart);   // Determine length of flash (not to go beyond NUM_LEDS-1)
  uint8_t bri = 255/hw_random8(1, 3);

  if (SEGMENT.aux1 == 0) //init, leader flash
  {
    SEGMENT.aux1 = hw_random8(4, 4 + SEGMENT.intensity/20); //number of flashes
    SEGMENT.aux1 *= 2;

    bri = 52; //leader has lower brightness
    SEGMENT.aux0 = 200; //200ms delay after leader
  }

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1));

  if (SEGMENT.aux1 > 3 && !(SEGMENT.aux1 & 0x01)) { //flash on even number >2
    for (unsigned i = ledstart; i < ledstart + ledlen; i++)
    {
      SEGMENT.setPixelColor(i,SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0, bri));
    }
    SEGMENT.aux1--;

    SEGMENT.step = effect_start_time;
    //return hw_random8(4, 10); // each flash only lasts one frame/every 24ms... originally 4-10 milliseconds
  } else {
    if (effect_start_time - SEGMENT.step > SEGMENT.aux0) {
      SEGMENT.aux1--;
      if (SEGMENT.aux1 < 2) SEGMENT.aux1 = 0;

      SEGMENT.aux0 = (50 + hw_random8(100)); //delay between flashes
      if (SEGMENT.aux1 == 2) {
        SEGMENT.aux0 = (hw_random8(255 - SEGMENT.speed) * 100); // delay between strikes
      }
      SEGMENT.step = effect_start_time;
    }
  }
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__LIGHTNING[] PROGMEM = "Lightning@!,!,,,,,,,Overlay;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description :This basic one-dimensional 'fire' simulation works roughly as follows:
          There's a underlying array of 'heat' cells, that model the temperature
          at each point along the line.  Every cycle through the simulation, 
          four steps are performed:
          1) All cells cool down a little bit, losing heat to the air
          2) The heat from each cell drifts 'up' and diffuses a little
          3) Sometimes randomly new 'sparks' of heat are added at the bottom
          4) The heat from each cell is rendered as a color into the leds array
              The heat-to-color mapping uses a black-body radiation approximation.

          Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).

          This simulation scales it self a bit depending on SEGLEN; it should look
          "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 

          I recommend running this simulation at anywhere from 30-100 frames per second,
          meaning an interframe delay of about 10-35 milliseconds.

          Looks best on a high-density LED setup (60+ pixels/meter).


          There are two main parameters you can play with to control the look and
          feel of your fire: COOLING (used in step 1 above) (Speed = COOLING), and SPARKING (used
          in step 3 above) (Effect Intensity = Sparking).
 * @note : Converted from WLED Effects "mode_fire_2012"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Fire_2012()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const unsigned strips = SEGMENT.nrOfVStrips();
  if (!SEGMENT.allocateData(strips * SEGLEN)) return EFFECT_DEFAULT(); //allocation failed
  byte* heat = SEGMENT.data;

  const uint32_t it = effect_start_time >> 5; //div 32

  struct virtualStrip {
    static void runStrip(uint16_t stripNr, byte* heat, uint32_t it) {

      const uint8_t ignition = MAX(3,pSEGLEN/10);  // ignition area: 10% of segment length or minimum 3 pixels

      // Step 1.  Cool down every cell a little
      for (unsigned i = 0; i < pSEGLEN; i++) {
        uint8_t cool = (it != pSEGMENT.step) ? tkr_anim->hw_random8((((20 + pSEGMENT.speed/3) * 16) / pSEGLEN)+2) : tkr_anim->hw_random8(4);
        uint8_t minTemp = (i<ignition) ? (ignition-i)/4 + 16 : 0;  // should not become black in ignition area
        uint8_t temp = qsub8(heat[i], cool);
        heat[i] = temp<minTemp ? minTemp : temp;
      }

      if (it != pSEGMENT.step) {
        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for (int k = pSEGLEN -1; k > 1; k--) {
          heat[k] = (heat[k - 1] + (heat[k - 2]<<1) ) / 3;  // heat[k-2] multiplied by 2
        }

        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if (tkr_anim->hw_random8() <= pSEGMENT.intensity) {
          uint8_t y = tkr_anim->hw_random8(ignition);
          uint8_t boost = (17+pSEGMENT.custom3) * (ignition - y/2) / ignition; // integer math!
          heat[y] = qadd8(heat[y], tkr_anim->hw_random8(96+2*boost,207+boost));
        }
      }

      // Step 4.  Map from heat cells to LED colors
      for (unsigned j = 0; j < pSEGLEN; j++) {
        pSEGMENT.setPixelColor(indexToVStrip(j, stripNr), ColorFromPalette(pSEGPALETTE, min(heat[j], byte(240)), 255, NOBLEND));
      }
    }
  };

  for (unsigned stripNr=0; stripNr<strips; stripNr++)
    virtualStrip::runStrip(stripNr, &heat[stripNr * SEGLEN], it);

  if (SEGMENT.is2D()) {
    uint8_t blurAmount = SEGMENT.custom2 >> 2;
    if (blurAmount > 48) blurAmount += blurAmount-48;             // extra blur when slider > 192  (bush burn)
    if (blurAmount < 16) SEGMENT.blurCols(SEGMENT.custom2 >> 1);  // no side-burn when slider < 64 (faster)
    else SEGMENT.blur(blurAmount);
  }

  if (it != SEGMENT.step)
    SEGMENT.step = it;

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__FIRE_2012[] PROGMEM = "Fire 2012@Cooling,Spark rate,,,Boost;;!;1;sx=64,ix=160,m12=1";


/********************************************************************************************************************************************************************************************************************
 * @description : Railway Crossing / Christmas Fairy lights
 * @note : Converted from WLED Effects "mode_railway"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Railway()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned dur = (256 - SEGMENT.speed) * 40;
  uint16_t rampdur = (dur * SEGMENT.intensity) >> 8;
  if (SEGMENT.step > dur)
  {
    //reverse direction
    SEGMENT.step = 0;
    SEGMENT.aux0 = !SEGMENT.aux0;
  }
  unsigned pos = 255;
  if (rampdur != 0)
  {
    unsigned p0 = (SEGMENT.step * 255) / rampdur;
    if (p0 < 255) pos = p0;
  }
  if (SEGMENT.aux0) pos = 255 - pos;
  for (unsigned i = 0; i < SEGLEN; i += 2)
  {
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(255 - pos, false, false, 255)); // do not use color 1 or 2, always use palette
    if (i < SEGLEN -1)
    {
      SEGMENT.setPixelColor(i + 1, SEGMENT.color_from_palette(pos, false, false, 255)); // do not use color 1 or 2, always use palette
    }
  }
  SEGMENT.step += FRAMETIME;
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__RAILWAY[] PROGMEM = "Railway@!,Smoothness;1,2;!;;pal=3";


/********************************************************************************************************************************************************************************************************************
 * @description : Modulates the brightness similar to a heartbeat
 *         (unimplemented?) tries to draw an ECG approximation on a 2D matrix
 * @note : Converted from WLED Effects "mode_heartbeat"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Heartbeat()
{
  unsigned bpm = 40 + (SEGMENT.speed >> 3);
  uint32_t msPerBeat = (60000L / bpm);
  uint32_t secondBeat = (msPerBeat / 3);
  uint32_t bri_lower = SEGMENT.aux1;
  unsigned long beatTimer = effect_start_time - SEGMENT.step;

  bri_lower = bri_lower * 2042 / (2048 + SEGMENT.intensity);
  SEGMENT.aux1 = bri_lower;

  if ((beatTimer > secondBeat) && !SEGMENT.aux0) { // time for the second beat?
    SEGMENT.aux1 = UINT16_MAX; //3/4 bri
    SEGMENT.aux0 = 1;
  }
  if (beatTimer > msPerBeat) { // time to reset the beat timer?
    SEGMENT.aux1 = UINT16_MAX; //full bri
    SEGMENT.aux0 = 0;
    SEGMENT.step = effect_start_time;
  }

  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, color_blend(SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), SEGCOLOR(1), uint8_t(255 - (SEGMENT.aux1 >> 8))));
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__HEARTBEAT[] PROGMEM = "Heartbeat@!,!;!,!;!;01;m12=1";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_fillnoise8"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__FillNoise8()
{
  if (SEGMENT.call == 0) SEGMENT.step = hw_random();
  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned index = inoise8(i * SEGLEN, SEGMENT.step + i * SEGLEN);
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }
  SEGMENT.step += beatsin8_t(SEGMENT.speed, 1, 6); //10,1,4

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__FILLNOISE8[] PROGMEM = "Fill Noise@!;!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_noise16_1"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Noise16_1()
{

  unsigned scale = 320;                                       // the "zoom factor" for the noise
  SEGMENT.step += (1 + SEGMENT.speed/16);

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned shift_x = beatsin8_t(11);                          // the x position of the noise field swings @ 17 bpm
    unsigned shift_y = SEGMENT.step/42;                        // the y position becomes slowly incremented
    unsigned real_x = (i + shift_x) * scale;                  // the x position of the noise field swings @ 17 bpm
    unsigned real_y = (i + shift_y) * scale;                  // the y position becomes slowly incremented
    uint32_t real_z = SEGMENT.step;                            // the z position becomes quickly incremented
    unsigned noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down
    unsigned index = sin8_t(noise * 3);                         // map LED color based on noise data

    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__NOISE16_1[] PROGMEM = "Noise 1@!;!;!;;pal=20";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_noise16_2"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Noise16_2()
{
  unsigned scale = 1000;                                        // the "zoom factor" for the noise
  SEGMENT.step += (1 + (SEGMENT.speed >> 1));

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned shift_x = SEGMENT.step >> 6;                        // x as a function of time
    uint32_t real_x = (i + shift_x) * scale;                    // calculate the coordinates within the noise field
    unsigned noise = inoise16(real_x, 0, 4223) >> 8;            // get the noise data and scale it down
    unsigned index = sin8_t(noise * 3);                           // map led color based on noise data

    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0, noise));
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__NOISE16_2[] PROGMEM = "Noise 2@!;!;!;;pal=43";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_noise16_3"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Noise16_3()
{
  unsigned scale = 800;                                       // the "zoom factor" for the noise
  SEGMENT.step += (1 + SEGMENT.speed);

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned shift_x = 4223;                                  // no movement along x and y
    unsigned shift_y = 1234;
    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = SEGMENT.step*8;
    unsigned noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down
    unsigned index = sin8_t(noise * 3);                         // map led color based on noise data

    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0, noise));
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__NOISE16_3[] PROGMEM = "Noise 3@!;!;!;;pal=35";


/*******************************************************************************************************************************************************************************************************************
 * @description : https://github.com/aykevl/ledstrip-spark/blob/master/ledino
 * @note : Converted from WLED Effects "mode_noise16_4"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Noise16_4()
{
  uint32_t stp = (effect_start_time * SEGMENT.speed) >> 7;
  for (unsigned i = 0; i < SEGLEN; i++) {
    int index = inoise16(uint32_t(i) << 12, stp);
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__NOISE16_4[] PROGMEM = "Noise 4@!;!;!;;pal=26";

/*******************************************************************************************************************************************************************************************************************
 * @description : Peaceful noise that's slow and with gradually changing palettes. Does not support WLED palettes or default colours or controls. Slow noise palette by Andrew Tuline.
 * @note : Converted from WLED Effects "mode_noisepal"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Noise_Pal()
{
  unsigned scale = 15 + (SEGMENT.intensity >> 2); //default was 30
  //#define scale 30

  unsigned dataSize = sizeof(CRGBPalette16) * 2; //allocate space for 2 Palettes (2 * 16 * 3 = 96 bytes)
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  CRGBPalette16* palettes = reinterpret_cast<CRGBPalette16*>(SEGMENT.data);

  unsigned changePaletteMs = 4000 + SEGMENT.speed *10; //between 4 - 6.5sec
  if (effect_start_time - SEGMENT.step > changePaletteMs)
  {
    SEGMENT.step = effect_start_time;

    unsigned baseI = hw_random8();
    palettes[1] = CRGBPalette16(CHSV(baseI+hw_random8(64), 255, hw_random8(128,255)), CHSV(baseI+128, 255, hw_random8(128,255)), CHSV(baseI+hw_random8(92), 192, hw_random8(128,255)), CHSV(baseI+hw_random8(92), 255, hw_random8(128,255)));
  }

  //EVERY_N_MILLIS(10) { //(don't have to time this, effect function is only called every 24ms)
  nblendPaletteTowardPalette(palettes[0], palettes[1], 48);               // Blend towards the target palette over 48 iterations.

  if (SEGMENT.palette_id > 0) palettes[0] = SEGPALETTE;

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned index = inoise8(i*scale, SEGMENT.aux0+i*scale);                // Get a value from the noise function. I'm using both x and y axis.
    SEGMENT.setPixelColor(i,  ColorFromPalette(palettes[0], index, 255, LINEARBLEND));  // Use my own palette.
  }

  SEGMENT.aux0 += beatsin8_t(10,1,4);                                        // Moving along the distance. Vary it a bit with a sine wave.

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__NOISEPAL[] PROGMEM = "Noise Pal@!,Scale;;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Effects by Andrew Tuline. Making sine waves here.
 * @note : Converted from WLED Effects "phased_base"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Phased(uint8_t moder)
{
  unsigned allfreq = 16;                                          // Base frequency.
  float *phase = reinterpret_cast<float*>(&SEGMENT.step);         // Phase change value gets calculated (float fits into unsigned long).
  unsigned cutOff = (255-SEGMENT.intensity);                      // You can change the number of pixels.  AKA INTENSITY (was 192).
  unsigned modVal = 5;//SEGMENT.fft1/8+1;                         // You can change the modulus. AKA FFT1 (was 5).

  unsigned index = effect_start_time/64;                                  // Set color rotation speed
  *phase += SEGMENT.speed/32.0;                                  // You can change the speed of the wave. AKA SPEED (was .4)

  for (unsigned i = 0; i < SEGLEN; i++) {
    if (moder == 1) modVal = (inoise8(i*10 + i*10) /16);         // Let's randomize our mod length with some Perlin noise.
    unsigned val = (i+1) * allfreq;                              // This sets the frequency of the waves. The +1 makes sure that led 0 is used.
    if (modVal == 0) modVal = 1;
    val += *phase * (i % modVal +1) /2;                          // This sets the varying phase change of the waves. By Andrew Tuline.
    unsigned b = cubicwave8(val);                                 // Now we make an 8 bit sinewave.
    b = (b > cutOff) ? (b - cutOff) : 0;                         // A ternary operator to cutoff the light.
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(index, false, false, 0), uint8_t(b)));
    index += 256 / SEGLEN;
    if (SEGLEN > 256) index ++;                                  // Correction for segments longer than 256 LEDs
  }

  return FRAMETIME;  
}


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_phased"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Phased(void) {
  return EffectAnim__Base_Phased(0);
}
static const char PM_EFFECT_CONFIG__PHASED[] PROGMEM = "Phased@!,!;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_phased_noise"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__PhasedNoise(void) {
  return EffectAnim__Base_Phased(1);
}
static const char PM_EFFECT_CONFIG__PHASEDNOISE[] PROGMEM = "Phased Noise@!,!;!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : Base function for scanners
 * @note : Converted from WLED Effects "scan"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Scan(bool dual)
{

  if (SEGLEN == 1) return EFFECT_DEFAULT();
  uint32_t cycleTime = 750 + (255 - SEGMENT.speed)*150;
  uint32_t perc = effect_start_time% cycleTime;
  int prog = (perc * 65535) / cycleTime;
  int size = 1 + ((SEGMENT.intensity * SEGLEN) >> 9);
  int ledIndex = (prog * ((SEGLEN *2) - size *2)) >> 16;

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1));

  int led_offset = ledIndex - (SEGLEN - size);
  led_offset = abs(led_offset);

  if (dual) {
    for (int j = led_offset; j < led_offset + size; j++) {
      unsigned i2 = SEGLEN -1 -j;
      SEGMENT.setPixelColor(i2, SEGMENT.color_from_palette(i2, true, PALETTE_SOLID_WRAP, (SEGCOLOR(2))? 2:0));
    }
  }

  for (int j = led_offset; j < led_offset + size; j++) {
    SEGMENT.setPixelColor(j, SEGMENT.color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;  
}


/********************************************************************************************************************************************************************************************************************
 * @description : Runs a single pixel back and forth.
 * @note : Converted from WLED Effects "mode_scan"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Scan()
{
  EffectAnim__Base_Scan(false);
}
static const char PM_EFFECT_CONFIG__SCAN[] PROGMEM = "Scan@!,# of dots,,,,,,,Overlay;!,!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : Runs two pixel back and forth in opposite directions.
 * @note : Converted from WLED Effects "mode_dual_scan"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Scan_Dual()
{
  EffectAnim__Base_Scan(true);
}
static const char PM_EFFECT_CONFIG__DUAL_SCAN[] PROGMEM = "Scan Dual@!,# of dots,,,,,,,Overlay;!,!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : K.I.T.T.
 * @note : Converted from WLED Effects "mode_larson_scanner"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Larson_Scanner()
{

  if (SEGLEN == 1) return EFFECT_DEFAULT();

  const unsigned speed  = FRAMETIME * map(SEGMENT.speed, 0, 255, 96, 2); // map into useful range
  const unsigned pixels = SEGLEN / speed; // how many pixels to advance per frame

  SEGMENT.fade_out(255-SEGMENT.intensity);

  if (SEGMENT.step > effect_start_time) return FRAMETIME;  // we have a pause

  unsigned index = SEGMENT.aux1 + pixels;
  // are we slow enough to use frames per pixel?
  if (pixels == 0) {
    const unsigned frames = speed / SEGLEN; // how many frames per 1 pixel
    if (SEGMENT.step++ < frames) return FRAMETIME;
    SEGMENT.step = 0;
    index++;
  }

  if (index > SEGLEN) {

    SEGMENT.aux0 = !SEGMENT.aux0; // change direction
    SEGMENT.aux1 = 0;            // reset position
    // set delay
    if (SEGMENT.aux0 || SEGMENT.check2) SEGMENT.step = effect_start_time + SEGMENT.custom1 * 25; // multiply by 25ms
    else SEGMENT.step = 0;

  } else {

    // paint as many pixels as needed
    for (unsigned i = SEGMENT.aux1; i < index; i++) {
      unsigned j = (SEGMENT.aux0) ? i : SEGLEN - 1 - i;
      uint32_t c = SEGMENT.color_from_palette(j, true, PALETTE_SOLID_WRAP, 0);
      SEGMENT.setPixelColor(j, c);
      if (SEGMENT.check1) {
        SEGMENT.setPixelColor(SEGLEN - 1 - j, SEGCOLOR(2) ? SEGCOLOR(2) : c);
      }
    }
    SEGMENT.aux1 = index;
  }
  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__LARSON_SCANNER[] PROGMEM = "Scanner@!,Trail,Delay,,,Dual,Bi-delay;!,!,!;!;;m12=0,c1=0";


/********************************************************************************************************************************************************************************************************************
 * @description : Creates two Larson scanners moving in opposite directions
 *         Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/DualLarson.h
 * @note : Converted from WLED Effects "mode_dual_larson_scanner"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Larson_Scanner_Dual(void){
  SEGMENT.check1 = true;
  EffectAnim__Larson_Scanner();
}
static const char PM_EFFECT_CONFIG__DUAL_LARSON_SCANNER[] PROGMEM = "Scanner Dual@!,Trail,Delay,,,Dual,Bi-delay;!,!,!;!;;m12=0,c1=0";


/********************************************************************************************************************************************************************************************************************
 * @description : ICU mode
 * @note : Converted from WLED Effects "mode_icu"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__ICU()
{
  unsigned dest = SEGMENT.step & 0xFFFF;
  unsigned space = (SEGMENT.intensity >> 3) +2;

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1));

  byte pindex = map(dest, 0, SEGLEN-SEGLEN/space, 0, 255);
  uint32_t col = SEGMENT.color_from_palette(pindex, false, false, 0);

  SEGMENT.setPixelColor(dest, col);
  SEGMENT.setPixelColor(dest + SEGLEN/space, col);

  if(SEGMENT.aux0 == dest) { // pause between eye movements
    if(hw_random8(6) == 0) { // blink once in a while
      SEGMENT.setPixelColor(dest, SEGCOLOR(1));
      SEGMENT.setPixelColor(dest + SEGLEN/space, SEGCOLOR(1));
      return 200;
    }
    SEGMENT.aux0 = hw_random16(SEGLEN-SEGLEN/space);
    return 1000 + hw_random16(2000);
  }

  if(SEGMENT.aux0 > SEGMENT.step) {
    SEGMENT.step++;
    dest++;
  } else if (SEGMENT.aux0 < SEGMENT.step) {
    SEGMENT.step--;
    dest--;
  }

  SEGMENT.setPixelColor(dest, col);
  SEGMENT.setPixelColor(dest + SEGLEN/space, col);

  return SPEED_FORMULA_L;  
}
static const char PM_EFFECT_CONFIG__ICU[] PROGMEM = "ICU@!,!,,,,,,,Overlay;!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : Water ripple
 *         propagation velocity from speed
 *         drop rate from intensity
 * @note : Converted from WLED Effects "ripple_base"
 ********************************************************************************************************************************************************************************************************************/
//4 bytes
typedef struct Ripple {
  uint8_t state;
  uint8_t color;
  uint16_t pos;
} ripple;
uint16_t mAnimatorLight::EffectAnim__Base_Ripple(uint8_t blurAmount)
{
  #ifdef ESP8266
  int MAX_RIPPLES = 56;
  #else
  int MAX_RIPPLES = 100;
  #endif
  unsigned maxRipples = min(1 + (int)(SEGLEN >> 2), MAX_RIPPLES);  // 56 max for 16 segment ESP8266
  unsigned dataSize = sizeof(ripple) * maxRipples;

  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  Ripple* ripples = reinterpret_cast<Ripple*>(SEGMENT.data);

  //draw wave
  for (unsigned i = 0; i < maxRipples; i++) {
    unsigned ripplestate = ripples[i].state;
    if (ripplestate) {
      unsigned rippledecay = (SEGMENT.speed >> 4) +1; //faster decay if faster propagation
      unsigned rippleorigin = ripples[i].pos;
      uint32_t col = SEGMENT.color_from_palette(ripples[i].color, false, false, 255);
      unsigned propagation = ((ripplestate/rippledecay - 1) * (SEGMENT.speed + 1));
      int propI = propagation >> 8;
      unsigned propF = propagation & 0xFF;
      unsigned amp = (ripplestate < 17) ? triwave8((ripplestate-1)*8) : map(ripplestate,17,255,255,2);

      #ifndef WLED_DISABLE_2D
      if (SEGMENT.is2D()) {
        propI /= 2;
        unsigned cx = rippleorigin >> 8;
        unsigned cy = rippleorigin & 0xFF;
        unsigned mag = scale8(sin8_t((propF>>2)), amp);
        if (propI > 0) SEGMENT.drawCircle(cx, cy, propI, color_blend(SEGMENT.getPixelColorXY(cx + propI, cy), col, mag), true);
      } else
      #endif
      {
        int left = rippleorigin - propI -1;
        int right = rippleorigin + propI +2;
        for (int v = 0; v < 4; v++) {
          uint8_t mag = scale8(cubicwave8((propF>>2) + v * 64), amp);
          SEGMENT.setPixelColor(left + v, color_blend(SEGMENT.getPixelColor(left + v), col, mag)); // TODO
          SEGMENT.setPixelColor(right - v, color_blend(SEGMENT.getPixelColor(right - v), col, mag)); // TODO
        }
      }
      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else {//randomly create new wave
      if (hw_random16(IBN + 10000) <= (SEGMENT.intensity >> (SEGMENT.is2D()*3))) {
        ripples[i].state = 1;
        ripples[i].pos = SEGMENT.is2D() ? ((hw_random8(SEG_W)<<8) | (hw_random8(SEG_H))) : hw_random16(SEGLEN);
        ripples[i].color = hw_random8(); //color
      }
    }
  }
  SEGMENT.blur(blurAmount);
  return FRAMETIME;  
}


/********************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_ripple"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Ripple(void) {
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  if(SEGMENT.custom1 || SEGMENT.check2) // blur or overlay
    SEGMENT.fade_out(250);
  else
    SEGMENT.fill(SEGCOLOR(1));

  return EffectAnim__Base_Ripple(SEGMENT.custom1>>1);
}
static const char PM_EFFECT_CONFIG__RIPPLE[] PROGMEM = "Ripple@!,Wave #,Blur,,,,Overlay;,!;!;12;c1=0";


/********************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_ripple_rainbow"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Ripple_Rainbow(void) {
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  if (SEGMENT.call ==0) {
    SEGMENT.aux0 = hw_random8();
    SEGMENT.aux1 = hw_random8();
  }
  if (SEGMENT.aux0 == SEGMENT.aux1) {
    SEGMENT.aux1 = hw_random8();
  } else if (SEGMENT.aux1 > SEGMENT.aux0) {
    SEGMENT.aux0++;
  } else {
    SEGMENT.aux0--;
  }
  SEGMENT.fill(color_blend(SEGMENT.color_wheel(SEGMENT.aux0),BLACK,uint8_t(235)));
  return EffectAnim__Base_Ripple();
}
static const char PM_EFFECT_CONFIG__RIPPLE_RAINBOW[] PROGMEM = "Ripple Rainbow@!,Wave #;;!;12";


/********************************************************************************************************************************************************************************************************************
 * @description : Firing comets from one end. "Lighthouse"
 * @note : Converted from WLED Effects "mode_comet"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Comet()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned counter = (effect_start_time * ((SEGMENT.speed >>2) +1)) & 0xFFFF;
  unsigned index = (counter * SEGLEN) >> 16;
  if (SEGMENT.call == 0) SEGMENT.aux0 = index;

  SEGMENT.fade_out(SEGMENT.intensity);

  SEGMENT.setPixelColor( index, SEGMENT.color_from_palette(index, true, PALETTE_SOLID_WRAP, 0));
  if (index > SEGMENT.aux0) {
    for (unsigned i = SEGMENT.aux0; i < index ; i++) {
       SEGMENT.setPixelColor( i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  } else if (index < SEGMENT.aux0 && index < 10) {
    for (unsigned i = 0; i < index ; i++) {
       SEGMENT.setPixelColor( i, SEGMENT.color_from_palette(i, true, PALETTE_SOLID_WRAP, 0));
    }
  }
  SEGMENT.aux0 = index++;

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__COMET[] PROGMEM = "Comet@!,Fade rate;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Dots waving around in a sine/pendulum motion.
 *         Little pixel birds flying in a circle. By Aircoookie
 * @note : Converted from WLED Effects "mode_chunchun"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Chunchun()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  SEGMENT.fade_out(254); // add a bit of trail
  unsigned counter = effect_start_time * (6 + (SEGMENT.speed >> 4));
  unsigned numBirds = 2 + (SEGLEN >> 3);  // 2 + 1/8 of a segment
  unsigned span = (SEGMENT.intensity << 8) / numBirds;

  for (unsigned i = 0; i < numBirds; i++)
  {
    counter -= span;
    unsigned megumin = sin16_t(counter) + 0x8000;
    unsigned bird = uint32_t(megumin * SEGLEN) >> 16;
    bird = constrain(bird, 0U, SEGLEN-1U);
    SEGMENT.setPixelColor(bird, SEGMENT.color_from_palette((i * 255)/ numBirds, false, false, 0)); // no palette wrapping
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__CHUNCHUN[] PROGMEM = "Chunchun@!,Gap size;!,!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Spotlights moving back and forth that cast dancing shadows.
 *                Shine this through tree branches/leaves or other close-up objects that cast
 *                interesting shadows onto a ceiling or tarp. By Steve Pomeroy @xxv
 * @note : Converted from WLED Effects "mode_dancing_shadows"
 ********************************************************************************************************************************************************************************************************************/
//13 bytes
typedef struct Spotlight {
  float speed;
  uint8_t colorIdx;
  int16_t position;
  unsigned long lastUpdateTime;
  uint8_t width;
  uint8_t type;
} spotlight;

#define SPOT_TYPE_SOLID       0
#define SPOT_TYPE_GRADIENT    1
#define SPOT_TYPE_2X_GRADIENT 2
#define SPOT_TYPE_2X_DOT      3
#define SPOT_TYPE_3X_DOT      4
#define SPOT_TYPE_4X_DOT      5
#define SPOT_TYPES_COUNT      6
#ifdef ESP8266
  #define SPOT_MAX_COUNT 17          //Number of simultaneous waves
#else
  #define SPOT_MAX_COUNT 49          //Number of simultaneous waves
#endif
uint16_t mAnimatorLight::EffectAnim__Dancing_Shadows()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned numSpotlights = map(SEGMENT.intensity, 0, 255, 2, SPOT_MAX_COUNT);  // 49 on 32 segment ESP32, 17 on 16 segment ESP8266
  bool initialize = SEGMENT.aux0 != numSpotlights;
  SEGMENT.aux0 = numSpotlights;

  unsigned dataSize = sizeof(spotlight) * numSpotlights;
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed
  Spotlight* spotlights = reinterpret_cast<Spotlight*>(SEGMENT.data);

  SEGMENT.fill(BLACK);

  unsigned long time = effect_start_time;
  bool respawn = false;

  for (size_t i = 0; i < numSpotlights; i++) {
    if (!initialize) {
      // advance the position of the spotlight
      int delta = (float)(time - spotlights[i].lastUpdateTime) *
                  (spotlights[i].speed * ((1.0 + SEGMENT.speed)/100.0));

      if (abs(delta) >= 1) {
        spotlights[i].position += delta;
        spotlights[i].lastUpdateTime = time;
      }

      respawn = (spotlights[i].speed > 0.0 && spotlights[i].position > (int)(SEGLEN + 2))
             || (spotlights[i].speed < 0.0 && spotlights[i].position < -(spotlights[i].width + 2));
    }

    if (initialize || respawn) {
      spotlights[i].colorIdx = hw_random8();
      spotlights[i].width = hw_random8(1, 10);

      spotlights[i].speed = 1.0/hw_random8(4, 50);

      if (initialize) {
        spotlights[i].position = hw_random16(SEGLEN);
        spotlights[i].speed *= hw_random8(2) ? 1.0 : -1.0;
      } else {
        if (hw_random8(2)) {
          spotlights[i].position = SEGLEN + spotlights[i].width;
          spotlights[i].speed *= -1.0;
        }else {
          spotlights[i].position = -spotlights[i].width;
        }
      }

      spotlights[i].lastUpdateTime = time;
      spotlights[i].type = hw_random8(SPOT_TYPES_COUNT);
    }

    uint32_t color = SEGMENT.color_from_palette(spotlights[i].colorIdx, false, false, 255);
    int start = spotlights[i].position;

    if (spotlights[i].width <= 1) {
      if (start >= 0 && start < (int)SEGLEN) {
        SEGMENT.blendPixelColor(start, color, 128);
      }
    } else {
      switch (spotlights[i].type) {
        case SPOT_TYPE_SOLID:
          for (size_t j = 0; j < spotlights[i].width; j++) {
            if ((start + j) >= 0 && (start + j) < SEGLEN) {
              SEGMENT.blendPixelColor(start + j, color, 128);
            }
          }
        break;

        case SPOT_TYPE_GRADIENT:
          for (size_t j = 0; j < spotlights[i].width; j++) {
            if ((start + j) >= 0 && (start + j) < SEGLEN) {
              SEGMENT.blendPixelColor(start + j, color, cubicwave8(map(j, 0, spotlights[i].width - 1, 0, 255)));
            }
          }
        break;

        case SPOT_TYPE_2X_GRADIENT:
          for (size_t j = 0; j < spotlights[i].width; j++) {
            if ((start + j) >= 0 && (start + j) < SEGLEN) {
              SEGMENT.blendPixelColor(start + j, color, cubicwave8(2 * map(j, 0, spotlights[i].width - 1, 0, 255)));
            }
          }
        break;

        case SPOT_TYPE_2X_DOT:
          for (size_t j = 0; j < spotlights[i].width; j += 2) {
            if ((start + j) >= 0 && (start + j) < SEGLEN) {
              SEGMENT.blendPixelColor(start + j, color, 128);
            }
          }
        break;

        case SPOT_TYPE_3X_DOT:
          for (size_t j = 0; j < spotlights[i].width; j += 3) {
            if ((start + j) >= 0 && (start + j) < SEGLEN) {
              SEGMENT.blendPixelColor(start + j, color, 128);
            }
          }
        break;

        case SPOT_TYPE_4X_DOT:
          for (size_t j = 0; j < spotlights[i].width; j += 4) {
            if ((start + j) >= 0 && (start + j) < SEGLEN) {
              SEGMENT.blendPixelColor(start + j, color, 128);
            }
          }
        break;
      }
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__DANCING_SHADOWS[] PROGMEM = "Dancing Shadows@!,# of shadows;!;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Imitates a washing machine, rotating same waves forward, then pause, then backward.  By Stefan Seegel
 * @note : Converted from WLED Effects "mode_washing_machine"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Washing_Machine()
{
  int speed = tristate_square8(effect_start_time >> 7, 90, 15);

  SEGMENT.step += (speed * 2048) / (512 - SEGMENT.speed);

  for (unsigned i = 0; i < SEGLEN; i++) {
    uint8_t col = sin8_t(((SEGMENT.intensity / 25 + 1) * 255 * i / SEGLEN) + (SEGMENT.step >> 7));
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(col, false, PALETTE_SOLID_WRAP, 3));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__WASHING_MACHINE[] PROGMEM = "Washing Machine@!,!;;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : Blends random colors across palette
  Modified, originally by Mark Kriegsman https://gist.github.com/kriegsman/1f7ccbbfa492a73c015e
 * @note : Converted from WLED Effects "mode_blends"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Blends()
{
  unsigned pixelLen = SEGLEN > UINT8_MAX ? UINT8_MAX : SEGLEN;
  unsigned dataSize = sizeof(uint32_t) * (pixelLen + 1);  // max segment length of 56 pixels on 16 segment ESP8266
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed
  uint32_t* pixels = reinterpret_cast<uint32_t*>(SEGMENT.data);
  uint8_t blendSpeed = map(SEGMENT.intensity, 0, UINT8_MAX, 10, 128);
  unsigned shift = (effect_start_time * ((SEGMENT.speed >> 3) +1)) >> 8;

  for (unsigned i = 0; i < pixelLen; i++) {
    pixels[i] = color_blend(pixels[i], SEGMENT.color_from_palette(shift + quadwave8((i + 1) * 16), false, PALETTE_SOLID_WRAP, 255), blendSpeed);
    shift += 3;
  }

  unsigned offset = 0;
  for (unsigned i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, pixels[offset++]);
    if (offset >= pixelLen) offset = 0;
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__BLENDS[] PROGMEM = "Blends@Shift speed,Blend speed;;!";


/*******************************************************************************************************************************************************************************************************************
 * @description : TV Simulator
  Modified and adapted to WLED by Def3nder, based on "Fake TV Light for Engineers" by Phillip Burgess https://learn.adafruit.com/fake-tv-light-for-engineers/arduino-sketch

 * @note : Converted from WLED Effects "mode_tv_simulator"
 ********************************************************************************************************************************************************************************************************************/
//43 bytes
typedef struct TvSim {
  uint32_t totalTime = 0;
  uint32_t fadeTime  = 0;
  uint32_t startTime = 0;
  uint32_t elapsed   = 0;
  uint32_t pixelNum  = 0;
  uint16_t sliderValues = 0;
  uint32_t sceeneStart    = 0;
  uint32_t sceeneDuration = 0;
  uint16_t sceeneColorHue = 0;
  uint8_t  sceeneColorSat = 0;
  uint8_t  sceeneColorBri = 0;
  uint8_t  actualColorR = 0;
  uint8_t  actualColorG = 0;
  uint8_t  actualColorB = 0;
  uint16_t pr = 0; // Prev R, G, B
  uint16_t pg = 0;
  uint16_t pb = 0;
} tvSim;
uint16_t mAnimatorLight::EffectAnim__TV_Simulator()
{
  int nr, ng, nb, r, g, b, i, hue;
  uint8_t  sat, bri, j;

  if (!SEGMENT.allocateData(sizeof(tvSim))) return EFFECT_DEFAULT(); //allocation failed
  TvSim* tvSimulator = reinterpret_cast<TvSim*>(SEGMENT.data);

  uint8_t colorSpeed     = map(SEGMENT.speed,     0, UINT8_MAX,  1, 20);
  uint8_t colorIntensity = map(SEGMENT.intensity, 0, UINT8_MAX, 10, 30);

  i = SEGMENT.speed << 8 | SEGMENT.intensity;
  if (i != tvSimulator->sliderValues) {
    tvSimulator->sliderValues = i;
    SEGMENT.aux1 = 0;
  }

    // create a new sceene
    if (((effect_start_time - tvSimulator->sceeneStart) >= tvSimulator->sceeneDuration) || SEGMENT.aux1 == 0) {
      tvSimulator->sceeneStart    = effect_start_time;                                               // remember the start of the new sceene
      tvSimulator->sceeneDuration = hw_random16(60* 250* colorSpeed, 60* 750 * colorSpeed);    // duration of a "movie sceene" which has similar colors (5 to 15 minutes with max speed slider)
      tvSimulator->sceeneColorHue = hw_random16(   0, 768);                                    // random start color-tone for the sceene
      tvSimulator->sceeneColorSat = hw_random8 ( 100, 130 + colorIntensity);                   // random start color-saturation for the sceene
      tvSimulator->sceeneColorBri = hw_random8 ( 200, 240);                                    // random start color-brightness for the sceene
      SEGMENT.aux1 = 1;
      SEGMENT.aux0 = 0;
    }

    // slightly change the color-tone in this sceene
    if (SEGMENT.aux0 == 0) {
      // hue change in both directions
      j = hw_random8(4 * colorIntensity);
      hue = (hw_random8() < 128) ? ((j < tvSimulator->sceeneColorHue)       ? tvSimulator->sceeneColorHue - j : 767 - tvSimulator->sceeneColorHue - j) :  // negative
                                ((j + tvSimulator->sceeneColorHue) < 767 ? tvSimulator->sceeneColorHue + j : tvSimulator->sceeneColorHue + j - 767) ;  // positive

      // saturation
      j = hw_random8(2 * colorIntensity);
      sat = (tvSimulator->sceeneColorSat - j) < 0 ? 0 : tvSimulator->sceeneColorSat - j;

      // brightness
      j = hw_random8(100);
      bri = (tvSimulator->sceeneColorBri - j) < 0 ? 0 : tvSimulator->sceeneColorBri - j;

      // calculate R,G,B from HSV
      // Source: https://blog.adafruit.com/2012/03/14/constant-brightness-hsb-to-rgb-algorithm/
      { // just to create a local scope for  the variables
        uint8_t temp[5], n = (hue >> 8) % 3;
        uint8_t x = ((((hue & 255) * sat) >> 8) * bri) >> 8;
        uint8_t s = (  (256 - sat) * bri) >> 8;
        temp[0] = temp[3] =       s;
        temp[1] = temp[4] =   x + s;
        temp[2] =           bri - x;
        tvSimulator->actualColorR = temp[n + 2];
        tvSimulator->actualColorG = temp[n + 1];
        tvSimulator->actualColorB = temp[n    ];
      }
    }
    // Apply gamma correction, further expand to 16/16/16
    nr = (uint8_t)gamma8(tvSimulator->actualColorR) * 257; // New R/G/B
    ng = (uint8_t)gamma8(tvSimulator->actualColorG) * 257;
    nb = (uint8_t)gamma8(tvSimulator->actualColorB) * 257;

  if (SEGMENT.aux0 == 0) {  // initialize next iteration
    SEGMENT.aux0 = 1;

    // randomize total duration and fade duration for the actual color
    tvSimulator->totalTime = hw_random16(250, 2500);                   // Semi-random pixel-to-pixel time
    tvSimulator->fadeTime  = hw_random16(0, tvSimulator->totalTime);   // Pixel-to-pixel transition time
    if (hw_random8(10) < 3) tvSimulator->fadeTime = 0;                 // Force scene cut 30% of time

    tvSimulator->startTime = effect_start_time;
  } // end of initialization

  // how much time is elapsed ?
  tvSimulator->elapsed = effect_start_time - tvSimulator->startTime;

  // fade from prev color to next color
  if (tvSimulator->elapsed < tvSimulator->fadeTime) {
    r = map(tvSimulator->elapsed, 0, tvSimulator->fadeTime, tvSimulator->pr, nr);
    g = map(tvSimulator->elapsed, 0, tvSimulator->fadeTime, tvSimulator->pg, ng);
    b = map(tvSimulator->elapsed, 0, tvSimulator->fadeTime, tvSimulator->pb, nb);
  } else { // Avoid divide-by-zero in map()
    r = nr;
    g = ng;
    b = nb;
  }

  // set strip color
  for (i = 0; i < (int)SEGLEN; i++) {
    SEGMENT.setPixelColor(i, r >> 8, g >> 8, b >> 8);  // Quantize to 8-bit
  }

  // if total duration has passed, remember last color and restart the loop
  if ( tvSimulator->elapsed >= tvSimulator->totalTime) {
    tvSimulator->pr = nr; // Prev RGB = new RGB
    tvSimulator->pg = ng;
    tvSimulator->pb = nb;
    SEGMENT.aux0 = 0;
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__TV_SIMULATOR[] PROGMEM = "TV Simulator@!,!;;!;01";


/*******************************************************************************************************************************************************************************************************************
 * @description : Bouncing Balls Effect
 * @note : Converted from WLED Effects "mode_bouncing_balls"
 ********************************************************************************************************************************************************************************************************************/
//each needs 12 bytes
//Spark type is used for popcorn and 1D fireworks
typedef struct Ball {
  unsigned long lastBounceTime;
  float impactVelocity;
  float height;
} ball;
uint16_t mAnimatorLight::EffectAnim__Bouncing_Balls()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  //allocate segment data
  const unsigned strips = SEGMENT.nrOfVStrips(); // adapt for 2D
  const size_t maxNumBalls = 16;
  unsigned dataSize = sizeof(ball) * maxNumBalls;
  if (!SEGMENT.allocateData(dataSize * strips)) return EFFECT_DEFAULT(); //allocation failed

  Ball* balls = reinterpret_cast<Ball*>(SEGMENT.data);

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(2) ? BLACK : SEGCOLOR(1));

  // virtualStrip idea by @ewowi (Ewoud Wijma)
  // requires virtual strip # to be embedded into upper 16 bits of index in setPixelColor()
  // the following functions will not work on virtual strips: fill(), fade_out(), fadeToBlack(), blur()
  struct virtualStrip {
    static void runStrip(size_t stripNr, Ball* balls) {
      // number of balls based on intensity setting to max of 7 (cycles colors)
      // non-chosen color is a random color
      unsigned numBalls = (pSEGMENT.intensity * (maxNumBalls - 1)) / 255 + 1; // minimum 1 ball
      const float gravity = -9.81f; // standard value of gravity
      const bool hasCol2 = pSEGCOLOR(2);
      const unsigned long time = tkr_anim->effect_start_time;

      if (pSEGMENT.call == 0) {
        for (size_t i = 0; i < maxNumBalls; i++) balls[i].lastBounceTime = time;
      }

      for (size_t i = 0; i < numBalls; i++) {
        float timeSinceLastBounce = (time - balls[i].lastBounceTime)/((255-pSEGMENT.speed)/64 +1);
        float timeSec = timeSinceLastBounce/1000.0f;
        balls[i].height = (0.5f * gravity * timeSec + balls[i].impactVelocity) * timeSec; // avoid use pow(x, 2) - its extremely slow !

        if (balls[i].height <= 0.0f) {
          balls[i].height = 0.0f;
          //damping for better effect using multiple balls
          float dampening = 0.9f - float(i)/float(numBalls * numBalls); // avoid use pow(x, 2) - its extremely slow !
          balls[i].impactVelocity = dampening * balls[i].impactVelocity;
          balls[i].lastBounceTime = time;

          if (balls[i].impactVelocity < 0.015f) {
            float impactVelocityStart = sqrtf(-2.0f * gravity) * tkr_anim->hw_random8(5,11)/10.0f; // randomize impact velocity
            balls[i].impactVelocity = impactVelocityStart;
          }
        } else if (balls[i].height > 1.0f) {
          continue; // do not draw OOB ball
        }

        uint32_t color = pSEGCOLOR(0);
        if (pSEGMENT.palette_id) {
          color = pSEGMENT.color_wheel(i*(256/MAX(numBalls, 8)));
        } else if (hasCol2) {
          color = pSEGCOLOR(i % NUM_COLORS);
        }

        int pos = roundf(balls[i].height * (pSEGLEN - 1));
        #ifdef WLED_USE_AA_PIXELS
        if (SEGLEN<32) SEGMENT.setPixelColor(indexToVStrip(pos, stripNr), color); // encode virtual strip into index
        else           SEGMENT.setPixelColor(balls[i].height + (stripNr+1)*10.0f, color);
        #else
        pSEGMENT.setPixelColor(indexToVStrip(pos, stripNr), color); // encode virtual strip into index
        #endif
      }
    }
  };

  for (unsigned stripNr=0; stripNr<strips; stripNr++)
    virtualStrip::runStrip(stripNr, &balls[stripNr * maxNumBalls]);

  return FRAMETIME;

}
static const char PM_EFFECT_CONFIG__BOUNCINGBALLS[] PROGMEM = "Bouncing Balls@Gravity,# of balls,,,,,,,Overlay;!,!,!;!;1;m12=1"; //bar


/*******************************************************************************************************************************************************************************************************************
 * @description : bouncing balls on a track track Effect modified from Aircoookie's bouncing balls
 *         Courtesy of pjhatch (https://github.com/pjhatch)
 *         https://github.com/Aircoookie/WLED/pull/1039
 * @note : Converted from WLED Effects "rolling_balls"
 ********************************************************************************************************************************************************************************************************************/
// modified for balltrack mode
typedef struct RollingBall {
  unsigned long lastBounceUpdate;
  float mass; // could fix this to be = 1. if memory is an issue
  float velocity;
  float height;
} rball_t;

uint16_t mAnimatorLight::EffectAnim__Rolling_Balls(void) {
  //allocate segment data
  const unsigned maxNumBalls = 16; // 255/16 + 1
  unsigned dataSize = sizeof(rball_t) * maxNumBalls;
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  rball_t *balls = reinterpret_cast<rball_t *>(SEGMENT.data);

  // number of balls based on intensity setting to max of 16 (cycles colors)
  // non-chosen color is a random color
  unsigned numBalls = SEGMENT.intensity/16 + 1;
  bool hasCol2 = SEGCOLOR(2);

  if (SEGMENT.call == 0) {
    SEGMENT.fill(hasCol2 ? BLACK : SEGCOLOR(1));                    // start clean
    for (unsigned i = 0; i < maxNumBalls; i++) {
      balls[i].lastBounceUpdate = effect_start_time;
      balls[i].velocity = 20.0f * float(hw_random16(1000, 10000))/10000.0f;  // number from 1 to 10
      if (hw_random8()<128) balls[i].velocity = -balls[i].velocity;    // 50% chance of reverse direction
      balls[i].height = (float(hw_random16(0, 10000)) / 10000.0f);     // from 0. to 1.
      balls[i].mass   = (float(hw_random16(1000, 10000)) / 10000.0f);  // from .1 to 1.
    }
  }

  float cfac = float(scale8(8, 255-SEGMENT.speed) +1)*20000.0f; // this uses the Aircoookie conversion factor for scaling time using speed slider

  if (SEGMENT.check3) SEGMENT.fade_out(250); // 2-8 pixel trails (optional)
  else {
  	if (!SEGMENT.check2) SEGMENT.fill(hasCol2 ? BLACK : SEGCOLOR(1)); // don't fill with background color if user wants to see trails
  }

  for (unsigned i = 0; i < numBalls; i++) {
    float timeSinceLastUpdate = float((effect_start_time - balls[i].lastBounceUpdate))/cfac;
    float thisHeight = balls[i].height + balls[i].velocity * timeSinceLastUpdate; // this method keeps higher resolution
    // test if intensity level was increased and some balls are way off the track then put them back
    if (thisHeight < -0.5f || thisHeight > 1.5f) {
      thisHeight = balls[i].height = (float(hw_random16(0, 10000)) / 10000.0f); // from 0. to 1.
      balls[i].lastBounceUpdate = effect_start_time;
    }
    // check if reached ends of the strip
    if ((thisHeight <= 0.0f && balls[i].velocity < 0.0f) || (thisHeight >= 1.0f && balls[i].velocity > 0.0f)) {
      balls[i].velocity = -balls[i].velocity; // reverse velocity
      balls[i].lastBounceUpdate = effect_start_time;
      balls[i].height = thisHeight;
    }
    // check for collisions
    if (SEGMENT.check1) {
      for (unsigned j = i+1; j < numBalls; j++) {
        if (balls[j].velocity != balls[i].velocity) {
          //  tcollided + balls[j].lastBounceUpdate is acutal time of collision (this keeps precision with long to float conversions)
          float tcollided = (cfac*(balls[i].height - balls[j].height) +
                balls[i].velocity*float(balls[j].lastBounceUpdate - balls[i].lastBounceUpdate))/(balls[j].velocity - balls[i].velocity);

          if ((tcollided > 2.0f) && (tcollided < float(effect_start_time - balls[j].lastBounceUpdate))) { // 2ms minimum to avoid duplicate bounces
            balls[i].height = balls[i].height + balls[i].velocity*(tcollided + float(balls[j].lastBounceUpdate - balls[i].lastBounceUpdate))/cfac;
            balls[j].height = balls[i].height;
            balls[i].lastBounceUpdate = (unsigned long)(tcollided + 0.5f) + balls[j].lastBounceUpdate;
            balls[j].lastBounceUpdate = balls[i].lastBounceUpdate;
            float vtmp = balls[i].velocity;
            balls[i].velocity = ((balls[i].mass - balls[j].mass)*vtmp              + 2.0f*balls[j].mass*balls[j].velocity)/(balls[i].mass + balls[j].mass);
            balls[j].velocity = ((balls[j].mass - balls[i].mass)*balls[j].velocity + 2.0f*balls[i].mass*vtmp)             /(balls[i].mass + balls[j].mass);
            thisHeight = balls[i].height + balls[i].velocity*(effect_start_time - balls[i].lastBounceUpdate)/cfac;
          }
        }
      }
    }

    uint32_t color = SEGCOLOR(0);
    if (SEGMENT.palette_id) {
      //color = SEGMENT.color_wheel(i*(256/MAX(numBalls, 8)));
      color = SEGMENT.color_from_palette(i*255/numBalls, false, PALETTE_SOLID_WRAP, 0);
    } else if (hasCol2) {
      color = SEGCOLOR(i % NUM_COLORS);
    }

    if (thisHeight < 0.0f) thisHeight = 0.0f;
    if (thisHeight > 1.0f) thisHeight = 1.0f;
    unsigned pos = round(thisHeight * (SEGLEN - 1));
    SEGMENT.setPixelColor(pos, color);
    balls[i].lastBounceUpdate = effect_start_time;
    balls[i].height = thisHeight;
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__ROLLINGBALLS[] PROGMEM = "Rolling Balls@!,# of balls,,,,Collisions,Overlay,Trails;!,!,!;!;1;m12=1"; //bar


/********************************************************************************************************************************************************************************************************************
 * @description : Sinelon stolen from FASTLED examples
 * @note : Converted from WLED Effects "sinelon_base"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Base_Sinelon(bool dual, bool rainbow)
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  SEGMENT.fade_out(SEGMENT.intensity);
  unsigned pos = beatsin16_t(SEGMENT.speed/10,0,SEGLEN-1);
  if (SEGMENT.call == 0) SEGMENT.aux0 = pos;
  uint32_t color1 = SEGMENT.color_from_palette(pos, true, false, 0);
  uint32_t color2 = SEGCOLOR(2);
  if (rainbow) {
    color1 = SEGMENT.color_wheel((pos & 0x07) * 32);
  }
  SEGMENT.setPixelColor(pos, color1);
  if (dual) {
    if (!color2) color2 = SEGMENT.color_from_palette(pos, true, false, 0);
    if (rainbow) color2 = color1; //rainbow
    SEGMENT.setPixelColor(SEGLEN-1-pos, color2);
  }
  if (SEGMENT.aux0 != pos) {
    if (SEGMENT.aux0 < pos) {
      for (unsigned i = SEGMENT.aux0; i < pos ; i++) {
        SEGMENT.setPixelColor(i, color1);
        if (dual) SEGMENT.setPixelColor(SEGLEN-1-i, color2);
      }
    } else {
      for (unsigned i = SEGMENT.aux0; i > pos ; i--) {
        SEGMENT.setPixelColor(i, color1);
        if (dual) SEGMENT.setPixelColor(SEGLEN-1-i, color2);
      }
    }
    SEGMENT.aux0 = pos;
  }

  return FRAMETIME;  
}


/********************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_sinelon"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sinelon(void) {
  return EffectAnim__Base_Sinelon(false);
}
static const char PM_EFFECT_CONFIG__SINELON[] PROGMEM = "Sinelon@!,Trail;!,!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_sinelon_dual"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sinelon_Dual(void) {
  return EffectAnim__Base_Sinelon(true);
}
static const char PM_EFFECT_CONFIG__SINELON_DUAL[] PROGMEM = "Sinelon Dual@!,Trail;!,!,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_sinelon_rainbow"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Sinelon_Rainbow(void) {
  return EffectAnim__Base_Sinelon(false, true);
}
static const char PM_EFFECT_CONFIG__SINELON_RAINBOW[] PROGMEM = "Sinelon Rainbow@!,Trail;,,!;!";


/********************************************************************************************************************************************************************************************************************
 * @description : Drip Effect ported of: https://www.youtube.com/watch?v=sru2fXh4r7k
 * @note : Converted from WLED Effects "mode_drip"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__Drip()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  //allocate segment data
  unsigned strips = SEGMENT.nrOfVStrips();
  const int maxNumDrops = 4;
  unsigned dataSize = sizeof(spark) * maxNumDrops;
  if (!SEGMENT.allocateData(dataSize * strips)) return EFFECT_DEFAULT(); //allocation failed
  Spark* drops = reinterpret_cast<Spark*>(SEGMENT.data);

  if (!SEGMENT.check2) SEGMENT.fill(SEGCOLOR(1));

  struct virtualStrip {
    static void runStrip(uint16_t stripNr, Spark* drops) {

      unsigned numDrops = 1 + (pSEGMENT.intensity >> 6); // 255>>6 = 3

      float gravity = -0.0005f - (pSEGMENT.speed/50000.0f);
      gravity *= max(1, (int)pSEGLEN-1);
      int sourcedrop = 12;

      for (unsigned j=0;j<numDrops;j++) {
        if (drops[j].colIndex == 0) { //init
          drops[j].pos = pSEGLEN-1;    // start at end
          drops[j].vel = 0;           // speed
          drops[j].col = sourcedrop;  // brightness
          drops[j].colIndex = 1;      // drop state (0 init, 1 forming, 2 falling, 5 bouncing)
        }

        pSEGMENT.setPixelColor(indexToVStrip(pSEGLEN-1, stripNr), tkr_anim->color_blend(BLACK,pSEGCOLOR(0), uint8_t(sourcedrop)));// water source
        if (drops[j].colIndex==1) {
          if (drops[j].col>255) drops[j].col=255;
          pSEGMENT.setPixelColor(indexToVStrip(uint16_t(drops[j].pos), stripNr), tkr_anim->color_blend(BLACK,pSEGCOLOR(0),uint8_t(drops[j].col)));

          drops[j].col += map(pSEGMENT.speed, 0, 255, 1, 6); // swelling

          if (tkr_anim->hw_random8() < drops[j].col/10) {               // random drop
            drops[j].colIndex=2;               //fall
            drops[j].col=255;
          }
        }
        if (drops[j].colIndex > 1) {           // falling
          if (drops[j].pos > 0) {              // fall until end of segment
            drops[j].pos += drops[j].vel;
            if (drops[j].pos < 0) drops[j].pos = 0;
            drops[j].vel += gravity;           // gravity is negative

            for (int i=1;i<7-drops[j].colIndex;i++) { // some minor math so we don't expand bouncing droplets
              unsigned pos = constrain(unsigned(drops[j].pos) +i, 0, pSEGLEN-1); //this is BAD, returns a pos >= SEGLEN occasionally
              pSEGMENT.setPixelColor(indexToVStrip(pos, stripNr), tkr_anim->color_blend(BLACK,pSEGCOLOR(0),uint8_t(drops[j].col/i))); //spread pixel with fade while falling
            }

            if (drops[j].colIndex > 2) {       // during bounce, some water is on the floor
              pSEGMENT.setPixelColor(indexToVStrip(0, stripNr), tkr_anim->color_blend(pSEGCOLOR(0),BLACK,uint8_t(drops[j].col)));
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
    }
  };

  for (unsigned stripNr=0; stripNr<strips; stripNr++)
    virtualStrip::runStrip(stripNr, &drops[stripNr*maxNumDrops]);

  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__DRIP[] PROGMEM = "Drip@Gravity,# of drips,,,,,,,Overlay;!,!;!;;m12=1"; //bar


/********************************************************************************************************************************************************************************************************************
 * @description : Flow Stripe
 * @note : Converted from WLED Effects "mode_FlowStripe"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__FlowStripe()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const int hl = SEGLEN * 10 / 13;
  uint8_t hue = effect_start_time / (SEGMENT.speed+1);
  uint32_t t = effect_start_time / (SEGMENT.intensity/8+1);

  for (unsigned i = 0; i < SEGLEN; i++) {
    int c = (abs((int)i - hl) / hl) * 127;
    c = sin8_t(c);
    c = sin8_t(c / 2 + t);
    byte b = sin8_t(c + t/8);
    SEGMENT.setPixelColor(i, CHSV(b + hue, 255, 255));
  }

  return FRAMETIME;  
}
static const char PM_EFFECT_CONFIG__FLOWSTRIPE[] PROGMEM = "Flow Stripe@Hue speed,Effect speed;;";


/********************************************************************************************************************************************************************************************************************
 * @description : Wavesins Uses beatsin8() + phase shifting. By: Andrew Tuline
 * @note : Converted from WLED Effects "mode_wavesins"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__WaveSins()
{
  for (unsigned i = 0; i < SEGLEN; i++) {
    uint8_t bri = sin8_t(effect_start_time/4 + i * SEGMENT.intensity);
    uint8_t index = beatsin8_t(SEGMENT.speed, SEGMENT.custom1, SEGMENT.custom1+SEGMENT.custom2, 0, i * (SEGMENT.custom3<<3)); // custom3 is reduced resolution slider
    //SEGMENT.setPixelColor(i, ColorFromPalette(SEGPALETTE, index, bri, LINEARBLEND));
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0, bri));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__WAVESINS[] PROGMEM = "Wavesins@!,Brightness variation,Starting color,Range of colors,Color variation;!;!";








#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

// #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
// /********************************************************************************************************************************************************************************************************************
//  *******************************************************************************************************************************************************************************************************************
//  * @description : Test case used for developing new animations
//  * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
//  * 
//  * @param : "cycle_time__rate_ms" : How often it changes
//  * @param : "time_ms" : How often it changes
//  * @param : "pixels to update" : How often it changes
//  * @param : "cycle_time__rate_ms" : How often it changes 
//  * 
//  *******************************************************************************************************************************************************************************************************************
//  ********************************************************************************************************************************************************************************************************************/
// uint16_t mAnimatorLight::SubTask_Flasher_Animate_Function_Tester_01()
// {


//  SEGMENT.palette_container->CRGB16Palette16_Palette.data = Test_p;
//  tkr_anim->_virtualSegmentLength = 50;
//  tkr_anim->paletteBlend = 3;
 
// uint8_t index = 15;

// uint8_t paletteIndex = 0; 

// CRGB colour;
//       uint8_t pbri = 255;

//  uint16_t j = 0;
//  uint16_t i = 0;
//     tkr_anim->SEGMENT.setPixelColor(j, mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i, nullptr, true, false, 10));
//   for(unsigned i = 0; i < 50; i++) {
    
//     j = i;//map(i,0,50,0,255);
//   //  tkr_anim->SEGMENT.setPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
//     // tkr_anim->SEGMENT.setPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
//   index = i;
  
// paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
// colour = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);

// uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


//     tkr_anim->SEGMENT.setPixelColor(i, col);
  
//   }
//     // tkr_anim->SEGMENT.setPixelColor(0, RgbColor(255,0,0));
//   SEGMENT.cycle_time__rate_ms = FRAMETIME;

// // CRGB colour;
// //  = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);
// // ALOG_TST(PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);







//   // uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

//   // //ALOG_TST(PSTR("dataSize = %d"), dataSize);

//   // if (!SEGMENT.allocateData(dataSize))
//   // {
//   //   ALOG_TST(PSTR("Not Enough Memory"));
//   //   SEGMENT.effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
//   // }
  
  
//   // // Pick new colours
//   //  SEGMENT.color_wheelBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  
//   // // Get starting positions already on show
//   // SEGMENT. SEGMENT.color_wheelBuffer_StartingColour_GetAllSegment();

//   // // Call the animator to blend from previous to new
//   // SetSegment_AnimFunctionCallback(  SEGIDX, 
//   //   [this](const AnimationParam& param){ 
//   //     this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
//   //   }
//   // );


//   SET_DIRECT_MODE();
  

// }


// /********************************************************************************************************************************************************************************************************************
//  *******************************************************************************************************************************************************************************************************************
//  * @description : Test case used for developing new animations
//  * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
//  * 
//  * @param : "cycle_time__rate_ms" : How often it changes
//  * @param : "time_ms" : How often it changes
//  * @param : "pixels to update" : How often it changes
//  * @param : "cycle_time__rate_ms" : How often it changes 
//  * 
//  * Currently using to test all palettes as they are converted into one method
//  * 
//  *******************************************************************************************************************************************************************************************************************
//  ********************************************************************************************************************************************************************************************************************/
// uint16_t mAnimatorLight::SubTask_Flasher_Animate_Function_Tester_02()
// {


//   ALOG_INF( PSTR("SubTask_Flasher_Animate_Function_Tester_02") );


//   // SEGMENT.palette_container->CRGB16Palette16_Palette.data = Test_p;
//   // tkr_anim->_virtualSegmentLength = 50;
//   // tkr_anim->paletteBlend = 3;

//   // uint8_t index = 15;

//   // uint8_t paletteIndex = 0; 

//   // CRGB colour;
//   // uint8_t pbri = 255;

//   // uint16_t j = 0;
//   // uint16_t i = 0;
//   // tkr_anim->SEGMENT.setPixelColor(j, mPaletteI->color_from_pale tte_Intermediate(i, true, false, 10));
//   // for(unsigned i = 0; i < 50; i++) {

//   //   j = i;//map(i,0,50,0,255);
//   //   //  tkr_anim->SEGMENT.setPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
//   //   // tkr_anim->SEGMENT.setPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
//   //   index = i;

//   //   paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
//   //   colour = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);

//   //   uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


//   //   tkr_anim->SEGMENT.setPixelColor(i, col, true);

//   // }

//   uint8_t colours_in_palette = 0;

//   RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(tkr_anim->SEGMENT_I(0).palette_id,0,nullptr,true,true,false,255,&colours_in_palette);

//   // RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(mPaletteI->PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID,0,nullptr,true,true,255,&colours_in_palette);

//   // SEGMENT.palette_id = mPaletteI->PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;

//   ALOG_INF( PSTR("pID{%d}, colours_in_palette = %d"), tkr_anim->SEGMENT_I(0).palette_id, colours_in_palette );

//   // SEGMENT.setPixelColor(0,colour,true);
// //  for(int i=0;i<50;i++)
// //   {
// //     SEGMENT.setPixelColor(i, RgbColor(0));
// //   }

//   for(int i=0;i<256;i++)
//   {

//     colour = mPaletteI->GetColourFromPaletteAdvanced(tkr_anim->SEGMENT_I(0).palette_id,i,nullptr,true,true,true,255,&colours_in_palette);

//     SEGMENT.setPixelColor(i, colour, true);

//     // ALOG_INF( PSTR("pID{%d}, colours_in_palette = %d"),tkr_anim->SEGMENT_I(0).palette_id, colours_in_palette );

//     // if(i>colours_in_palette)
//     // {
//     //   break;
//     // }


//   }



//     // tkr_anim->SEGMENT.setPixelColor(0, RgbColor(255,0,0));
//   // SEGMENT.cycle_time__rate_ms = FRAMETIME;

// // CRGB colour;
// //  = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);
// // ALOG_TST(PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


// // DEBUG_LINE_HERE;
// #ifdef ENABLE_DEVFEATURE_DEBUG_SERIAL__ANIMATION_OUTPUT
//           Serial.print("@1");
//           #endif 
// pCONT_iLight->ShowInter face();




//   // uint16_t dataSize = SEGMENT.colour_width__used_in_effect_generate * 2 * SEGMENT.length(); //allocate space for 10 test pixels

//   // //ALOG_TST(PSTR("dataSize = %d"), dataSize);

//   // if (!SEGMENT.allocateData(dataSize))
//   // {
//   //   ALOG_TST(PSTR("Not Enough Memory"));
//   //   SEGMENT.effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
//   // }
  
  
//   // // Pick new colours
//   // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  
//   // // Get starting positions already on show
//   // SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

//   // // Call the animator to blend from previous to new
//   // SetSegment_AnimFunctionCallback(  SEGIDX, 
//   //   [this](const AnimationParam& param){ 
//   //     this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
//   //   }
//   // );


//   SET_DIRECT_MODE();
  
// }
// #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Notification style
 * @note : 
 *  
 * params_user
 *  - val0: period of blink (total time between new flashes)
 *  - val1: desired number of repeats
 *  - val2: percentage duty cycle, 0-100% is used to split ON and OFF half-cycle... blink/pulse mode may only differ depending on the animation time to new value, maybe merge modes when both work by themselves
 * 
 * 
 * fade on/off: time_ms is period, cycle_time__rate_ms is period, repeats or not, 
 * blink on/off: time_ms is 0, cycle_time__rate_ms is split_period
 * pulse on/off: time_ms is split period, cycle_time__rate_ms is split_period
 * 
 * 
 * << Example JSON Command >>
 
  {
    "Effects": {
      "Function": 22,
      "Intensity": 50,
      "Params": [
        1000,
        10,
        50
      ]
    },
    "BrightnessRGB": 100,
    "SegColour0": {
      "Hue": 120,
      "Sat": 100,
      "BrightnessRGB": 100
    }
  }

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Static()
{
  SubTask_Segment_Animate_Function__Notification_Base(true, false, false); 
}
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Blinking()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, true, false); 
}
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Fade()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, false, true); 
}
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Pulsing()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, true, true); 
}
uint16_t mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Base(bool flag_static, bool flag_alternate_onoff, bool flag_blend)
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate*2) ){ return FRAMETIME; }

  uint16_t   period_ms            = SEGMENT.params_user[0];
  uint16_t*  repeats_desired      = &SEGMENT.params_user[1];
  uint16_t*  duty_cycle_desired   = &SEGMENT.params_user[2];
  uint16_t*  auto_seconds_timeout = &SEGMENT.params_user[3];

  // ALOG_INF(PSTR("duty_cycle_desired=%d"),*duty_cycle_desired);
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "SubTask_Segment_Animate_Function__Notification_Base %d of %d"),getCurrSegmentId(), segments.size());
  

  // if(getCurrSegmentId()==1){delay(5000);}
  
  uint16_t* repeat_counter_p        = &SEGMENT.aux0;
  uint16_t* blink_state_p           = &SEGMENT.aux1;
  uint16_t* blink_counter_p         = &SEGMENT.aux2;
  uint16_t* autotimeout_last_millis = &SEGMENT.aux3; // cant use tSaved as its U16, so just compare against previous millis

  RgbcctColor starting_colour     = RgbcctColor();
  RgbcctColor desired_colour      = SEGCOLOR_RGBCCT(0);
  bool        flag_set_animator   = false;

  /********************************************************************************************************
   * @brief Static
   ********************************************************************************************************/

  if(flag_static)
  {
    flag_set_animator = true;
  }

  // Default update rates
  SEGMENT.time_ms     = 0;  
  SEGMENT.cycle_time__rate_ms     = period_ms+FRAMETIME;
  SEGMENT.tSaved_AnimateRunTime -= period_ms; // force into past to make it happen now, temp solution

  /********************************************************************************************************
   * @brief Blinking and Pulsing
   ********************************************************************************************************/

  // Alternate between primary and secondary colours as targets
  if(flag_alternate_onoff)
  {
    if(*blink_state_p)
    {
      starting_colour = RgbcctColor();
      desired_colour  = SEGCOLOR_RGBCCT(0);
      *blink_state_p = 0;
    }
    else
    {
      starting_colour = SEGCOLOR_RGBCCT(0);
      desired_colour  = RgbcctColor();
      *blink_state_p = 1;
    }
  }else{
    starting_colour = RgbcctColor();
    desired_colour  = SEGCOLOR_RGBCCT(0);
  }

  /**
   * @brief 
   * Assumes aux values where reset on JsonCommand
   **/
  if(*repeat_counter_p < *repeats_desired)
  {
    
    ALOG_DBM( PSTR("*repeat_counter_p %d < %d *repeats_desired"), *repeat_counter_p, *repeats_desired );

    /**
     * @brief Rough for now
     **/
    if(flag_alternate_onoff)
    {

      SEGMENT.tSaved_AnimateRunTime -= period_ms; // force into past to make it happen now, temp solution
      
      if(*duty_cycle_desired==0)
      {
        SEGMENT.cycle_time__rate_ms     = period_ms/2;
        *repeat_counter_p += 1;
      }else{

        uint16_t duty_blink_t      = constrain(*duty_cycle_desired,0,100);
        float duty_blink_on_ratio  = duty_blink_t/100.0f;
        float duty_blink_off_ratio = (100-duty_blink_t)/100.0f;

        if(*blink_state_p == 1) // ON
        {
          SEGMENT.cycle_time__rate_ms     = period_ms*duty_blink_on_ratio;
          *repeat_counter_p += 1;
        }else{
          SEGMENT.cycle_time__rate_ms     = period_ms*duty_blink_off_ratio;
        }
        
        ALOG_DBM( PSTR("duty_blink %d/%d -> %d"), (uint16_t)period_ms*duty_blink_on_ratio, (uint16_t)period_ms*duty_blink_off_ratio, SEGMENT.cycle_time__rate_ms);
      }

    }else{
      *repeat_counter_p += 1;
    }

    /**
     * @brief Blend time can only be set after "rate" is calculated
     **/
    if(flag_blend){
      SEGMENT.time_ms     = SEGMENT.cycle_time__rate_ms;   
    }else{
      SEGMENT.time_ms     = 0; 
    }

    flag_set_animator = true;
    ALOG_DBM( PSTR("*repeat_counter_p %d < %d *repeats_desired"), *repeat_counter_p, *repeats_desired );
  }
  // If exceeded repeat counter, then reduce update rate
  else
  {
    ALOG_DBM( PSTR("*repeat_counter_p %d < %d *repeats_desired DONE"), *repeat_counter_p, *repeats_desired );
    SEGMENT.cycle_time__rate_ms = 1000; // Reduce refresh rate
    SEGMENT.time_ms = 0;
  }

  // Check timeout
  if((millis()-*autotimeout_last_millis)>1000) // 1 second tick
  {
    *autotimeout_last_millis = (uint16_t)(millis()); // update for next call
    if(*auto_seconds_timeout>1)
    {
      *auto_seconds_timeout -= 1;
      ALOG_DBM( PSTR("auto_seconds_timeout = %d, %d"), *autotimeout_last_millis, *auto_seconds_timeout );
    }else
    if(*auto_seconds_timeout==1)
    {
      ALOG_DBM( PSTR("auto_seconds_timeout = %d, %d   OFF"), *autotimeout_last_millis, *auto_seconds_timeout );
      desired_colour = RgbcctColor(); // off
      SEGMENT.time_ms = 500;
      SEGMENT.cycle_time__rate_ms = 1000;
    }
  }

  if(flag_set_animator)
  {
    desired_colour.debug_print("desired_colour");
    starting_colour.debug_print("starting_colour");

    
    // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K("effect >"D_HUE)), desired_colour.getHue360());


    // Serial.println(desired_colour.raw[0]);
    // Serial.println(desired_colour.raw[1]);
    // Serial.println(desired_colour.raw[2]);
    // Serial.println(desired_colour.raw[3]);
    // Serial.println(desired_colour.raw[4]);

    SetTransitionColourBuffer_DesiredColour (SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_width__used_in_effect_generate, desired_colour); 
    SetTransitionColourBuffer_StartingColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_width__used_in_effect_generate, starting_colour);

    SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(param); } );

  }

};

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
 * @note : 
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
uint16_t mAnimatorLight::EffectAnim__BorderWallpaper__TwoColour_Gradient()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return FRAMETIME; } // Pixel_Width * Two_Channels * Pixel_Count
    
  struct EDGE_SETTINGS{
    /**
     * @brief Sets how wide the gradient should be. 
     * 255: Means the entire edge is a gradient, so from edge pixel to the other edge pixel will be a full linear gradient (e.g. top and edge colours are fully blended on the edge)
     * 0:   Means the corner pixels meet in the middle of the edge, without any gradient (e.g. top and bottom edge colours meet in the middle)
     **/
    uint8_t gradient_width = 255; 
    uint8_t start_pixel_location = 0; // 0 = bottom left, 1 = bottom right, 2 = top right, 3 = top left
    uint8_t pixel_direction = 0; // 0 = clockwise, 1 = anticlockwise
    // uint16_t edge_lengths[4] = {44,23,42,23}; // 2 extra pixels on centre inlay
    uint16_t edge_lengths[4] = {44,67,109,133}; // 2 extra pixels on centre inlay
    

    std::vector<RgbwwColor> corner_colours;

  }edge_settings;



  edge_settings.corner_colours.push_back(RgbwwColor(255,0,0,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(255,0,0,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(0,255,0,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(0,255,0,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(0,0,255,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(0,0,255,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(255,0,255,0,0));
  edge_settings.corner_colours.push_back(RgbwwColor(255,0,255,0,0));






  // RgbcctColor edge1_colour = RgbcctColor(255,0,0,0,0);
  // RgbcctColor edge2_colour = RgbcctColor(0,255,0,0,0);
  // RgbcctColor edge3_colour = RgbcctColor(0,0,255,0,0);
  // RgbcctColor edge4_colour = RgbcctColor(255,0,255,0,0);


  RgbwwColor edge1_colour = RgbwwColor(0,100,100,0,0);
  RgbwwColor edge2_colour = RgbwwColor(D_RGB_ORANGE_PASTEL_90SAT  0,0);
  RgbwwColor edge3_colour = RgbwwColor(D_RGB_ORANGE_PASTEL_90SAT  0,0);
  RgbwwColor edge4_colour = RgbwwColor(D_RGB_ORANGE_PASTEL_90SAT  0,0);

  float progress;
  RgbwwColor colour;
  
  uint16_t edge_start_t = 0;
  uint16_t edge_stop_t = 0;
  uint16_t edge_length = 0;


  /**
   * @brief Side1
   **/
  edge_start_t = 0;
  edge_stop_t  = edge_settings.edge_lengths[0];
  edge_length = edge_stop_t - edge_start_t;
  for(uint16_t pixel = 0; pixel < edge_length; pixel++)
  {
    colour = edge1_colour;
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
  
    SEGMENT.Set_DynamicBuffer_DesiredColour(edge_start_t + pixel, RgbwwColorU32(colour) );
  }


  /**
   * @brief Side2
   **/
  edge_start_t = edge_settings.edge_lengths[0];
  edge_stop_t  = edge_settings.edge_lengths[1];
  edge_length = edge_stop_t - edge_start_t;
  for(uint16_t pixel = 0; pixel < edge_length; pixel++)
  {
    colour = edge2_colour;
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
  
  
    SEGMENT.Set_DynamicBuffer_DesiredColour(edge_start_t + pixel, RgbwwColorU32(colour) );
  
  }


  /**
   * @brief Side3
   **/
  edge_start_t = edge_settings.edge_lengths[1];
  edge_stop_t  = edge_settings.edge_lengths[2];
  edge_length = edge_stop_t - edge_start_t;
  for(uint16_t pixel = 0; pixel < edge_length; pixel++)
  {
    colour = edge3_colour;
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
  
  
    SEGMENT.Set_DynamicBuffer_DesiredColour(edge_start_t + pixel, RgbwwColorU32(colour) );
  
  }


  /**
   * @brief Side3
   **/
  edge_start_t = edge_settings.edge_lengths[2];
  edge_stop_t  = edge_settings.edge_lengths[3];
  edge_length = edge_stop_t - edge_start_t;
  for(uint16_t pixel = 0; pixel < edge_length; pixel++)
  {
    colour = edge4_colour;
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
  
  
    SEGMENT.Set_DynamicBuffer_DesiredColour(edge_start_t + pixel, RgbwwColorU32(colour) );
  
  }




  // RgbcctColor colour = RgbcctColor();
  // for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  // {    
  //   colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
  //   SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour);
  // }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });






























//   #ifdef USE_DEVFEATURE_PIXEL0_BOTTOM_RIGHT

//   float progress;
//   RgbcctColor colour_tmp;

//   uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//   uint8_t bottom_start = 0;
//   for(int bottom=0;bottom<bottom_size;bottom++){
//     animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//   }

//   uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//   uint8_t left_start = 33;
//   for(int left=0;left<left_size;left++){
//     //if(ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage>=0){ //if set
//       progress = (float)(left)/(float)(left_size);
//       colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                          RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                          progress);
//     //}
//     animation_colours[left_start+left].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
//   }

//   uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//   uint8_t top_start = 52;
//   for(int top=0;top<top_size;top++){
//     animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//   }

//   uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//   uint8_t right_start = 85;
//   for(int right=0;right<right_size;right++){
//     progress = (float)(right)/(float)(right_size);
//     colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                        RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                        progress);
//     animation_colours[right_start+right].DesiredColour = colour_tmp;
//   }

  
//      // #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
//       for(int i=0;i<STRIP_SIZE_MAX;i++)
//       {
//         // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//           // animation_colours[i].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global());

// animation_colours[i].DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global(), pCONT_iLight->getBriCCT_Global());

//         // }
//       }
//  //     #endif



//   #endif

  
//   #ifdef USE_DEVFEATURE_PIXEL0_BOTTOM_LEFT_ANTICLOCKWISE_TO_BE_FEATURE_OPTION


//   switch(1)
//   {
//     case 0: // method 4 different sides
//     { 

//       float progress;
//       RgbcctColor colour_tmp;

//       uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       uint8_t bottom_start = 0;
//       for(int bottom=0;bottom<bottom_size;bottom++){
//         animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//       }

//       uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       uint8_t right_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       for(int right=0;right<right_size;right++){
//         //if(ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage>=0){ //if set
//           // progress = (float)(right)/(float)(right_size);
//           // colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//           //                                    RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//           //                                    progress);
//         //}
//         // animation_colours[right_start+right].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//         animation_colours[right_start+right].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//       }

//       uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//       uint8_t top_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size+ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       for(int top=0;top<top_size;top++){
//         animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//       }

//       uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//       uint8_t left_start = top_start + top_size;
//       for(int left=0;left<left_size;left++){
//         // progress = (float)(left)/(float)(left_size);
//         // colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//         //                                    RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//         //                                    progress);
//         // animation_colours[left_start+left].DesiredColour = colour_tmp;
//         animation_colours[left_start+left].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].left.colour;;
//       }

//       // animation_colours[0].DesiredColour = RgbColor(255,0,0);
//       // animation_colours[43].DesiredColour = RgbColor(255,0,0);

//       // animation_colours[44].DesiredColour = RgbColor(0,255,0);
//       // animation_colours[66].DesiredColour = RgbColor(0,255,0);

//       // animation_colours[67].DesiredColour = RgbColor(0,0,255);
//       // animation_colours[108].DesiredColour = RgbColor(0,0,255);

//       // animation_colours[109].DesiredColour = RgbwColor(0,0,0,255);
//       // animation_colours[130].DesiredColour = RgbwColor(0,0,0,255);
      
//       // animation_colours[131].DesiredColour = RgbwColor(255,0,0,255);
//       // animation_colours[132].DesiredColour = RgbwColor(0,255,0,255);
//       // animation_colours[133].DesiredColour = RgbwColor(0,0,255,255);
//       // animation_colours[131].DesiredColour = RgbwColor(255,0,255,255);
      
//       }
//       break;
//       case 1: //blend top and bottom colour only, sides are linear
//       {


//       float progress;
//       RgbcctColor colour_tmp;

//       uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       uint8_t bottom_start = 0;
//       for(int bottom=0;bottom<bottom_size;bottom++){
//         animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//       }

//       uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       uint8_t right_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       for(int right=0;right<right_size;right++){
//         if(ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage>=0){ //if set
//           progress = (float)(right)/(float)(right_size);
//           colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                              RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                              progress);
//         }
//         animation_colours[right_start+right].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//         // animation_colours[right_start+right].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//       }

//       uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//       uint8_t top_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size+ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       for(int top=0;top<top_size;top++){
//         animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//       }

//       uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//       uint8_t left_start = top_start + top_size;
//       for(int left=0;left<left_size;left++){
//         progress = (float)(left)/(float)(left_size);
//         colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                            RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                            progress);
//         animation_colours[left_start+left].DesiredColour = colour_tmp;
//         // animation_colours[left_start+left].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].left.colour;;
//       }

//       #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
//       for(int i=0;i<STRIP_SIZE_MAX;i++)
//       {
//         // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//           // animation_colours[i].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global());

// animation_colours[i].DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global(), pCONT_iLight->getBriCCT_Global());

//         // }
//       }
//       #endif

//       }
//       break;
//   }//end switch

//   #endif





}
static const char PM_EFFECT_CONFIG__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT[] PROGMEM = "Border Wallpaper TwoColour@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
 * @note : 
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
uint16_t mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Gradient()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return FRAMETIME; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour = 0;//RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {    
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
  
  
  
    SEGMENT.Set_DynamicBuffer_DesiredColour(pixel, colour );
  
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

}
static const char PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT[] PROGMEM = "Border Wallpaper FourColour@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
 * @note : 
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
uint16_t mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Solid()
{

  if (!SEGMENT.allocateColourData( SEGMENT.colour_width__used_in_effect_generate * 2 * SEGLEN )){ return FRAMETIME; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint32_t colour = 0;//RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {    
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    // SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_width__used_in_effect_generate, colour.WithBrightness(brightness) );
  
  
  
    SEGMENT.Set_DynamicBuffer_DesiredColour( pixel, colour );
  }

  SEGMENT.DynamicBuffer_StartingColour_GetAllSegment();

  SetSegment_AnimFunctionCallback(SEGIDX, [this](const AnimationParam& param) { SEGMENT.AnimationProcess_LinearBlend_Dynamic_BufferU32(param); });

}
static const char PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_SOLID[] PROGMEM = "BW 4s@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS


/**
 * @brief
 * Uses a segments four RGBCCT colours to describe the four corners of a rear/edge lit backlight (e.g. a gradient ambilight)
 **/

// // Limit ambilight to addressible type, else I will just use "scene"
// uint16_t mAnimatorLight::init_Ambilight(){

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(pCONT_iLight->HUE_N2F(8),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HUE_N2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HUE_N2F(330),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(50));



//   #ifdef   DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE
  
//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbcctColor(255,175,0,255,0);//HsbColor(pCONT_iLight->HUE_N2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbcctColor(0,0,0,100,0);//HsbColor(pCONT_iLight->HUE_N2F(0),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HUE_N2F(340),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HUE_N2F(120),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 42;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 44; // 2 extra pixels on centre inlay
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 23;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 23;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 0;
//   ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage = 0;


//   #endif // DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE

//   #ifdef DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H
//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(pCONT_iLight->HUE_N2F(8),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(pCONT_iLight->HUE_N2F(240),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(pCONT_iLight->HUE_N2F(330),pCONT_iLight->SatN2F(100),pCONT_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(pCONT_iLight->HUE_N2F(20),pCONT_iLight->SatN2F(95),pCONT_iLight->BrtN2F(50));




//   #endif // DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H
  

// }

// uint16_t mAnimatorLight::SubTask_Ambilight_Main(){

//   // Add mode to allow orientations, for when screens rotate so they respect top/bottom

//   // if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
//   //   ALOG_DBG(PSTR(D_LOG_NEO "ambilight"));
//   // }

  
//   // ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbColor(255,0,0);
//   // ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbColor(0,255,0); 
//   // ambilightsettings.screens[SCREEN_CENTRE].left.colour   = RgbColor(0,0,255);
//   // ambilightsettings.screens[SCREEN_CENTRE].right.colour  = RgbColor(255,0,255);


//   //switch modes : USE presets
//   switch(ambilightsettings.ambilight_mode){
//     case AMBILIGHT_PRESETS_ID:

//     break;
//     case AMBILIGHT_SIDES_ID: // IE DELL of dual tone from the past
//       // not even splits, setting split point (ie bottom edge only 0%, 25% way up sides, half way 50%)
//       if(abs(millis()-ambilightsettings.tSavedUpdate)>ambilightsettings.ratemsSavedUpdate){ambilightsettings.tSavedUpdate=millis();
//         ALOG_DBG(PSTR(D_LOG_NEO "ambilight"));
//         Ambilight_Sides();
//         StartAnimationAsBlendFromStartingColorToDesiredColor();
//       }
//     break;
//     case AMBILIGHT_INPUT_STREAM_ID:{
//       //serial input
//     }break;
//   }

// } // END function




// uint16_t mAnimatorLight::Ambilight_Sides(){
//   #ifdef ENABLE_LOG_LEVEL_DEBUG
//   ALOG_DBG(PSTR(D_LOG_NEO "f::Ambilight_Sides()"));
//   #endif


//   #ifdef USE_DEVFEATURE_PIXEL0_BOTTOM_RIGHT

//   float progress;
//   RgbcctColor colour_tmp;

//   uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//   uint8_t bottom_start = 0;
//   for(int bottom=0;bottom<bottom_size;bottom++){
//     animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//   }

//   uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//   uint8_t left_start = 33;
//   for(int left=0;left<left_size;left++){
//     //if(ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage>=0){ //if set
//       progress = (float)(left)/(float)(left_size);
//       colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                          RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                          progress);
//     //}
//     animation_colours[left_start+left].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].left.colour;
//   }

//   uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//   uint8_t top_start = 52;
//   for(int top=0;top<top_size;top++){
//     animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//   }

//   uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//   uint8_t right_start = 85;
//   for(int right=0;right<right_size;right++){
//     progress = (float)(right)/(float)(right_size);
//     colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                        RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                        progress);
//     animation_colours[right_start+right].DesiredColour = colour_tmp;
//   }

  
//      // #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
//       for(int i=0;i<STRIP_SIZE_MAX;i++)
//       {
//         // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//           // animation_colours[i].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global());

// animation_colours[i].DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global(), pCONT_iLight->getBriCCT_Global());

//         // }
//       }
//  //     #endif



//   #endif

  
//   #ifdef USE_DEVFEATURE_PIXEL0_BOTTOM_LEFT_ANTICLOCKWISE_TO_BE_FEATURE_OPTION


//   switch(1)
//   {
//     case 0: // method 4 different sides
//     { 

//       float progress;
//       RgbcctColor colour_tmp;

//       uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       uint8_t bottom_start = 0;
//       for(int bottom=0;bottom<bottom_size;bottom++){
//         animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//       }

//       uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       uint8_t right_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       for(int right=0;right<right_size;right++){
//         //if(ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage>=0){ //if set
//           // progress = (float)(right)/(float)(right_size);
//           // colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//           //                                    RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//           //                                    progress);
//         //}
//         // animation_colours[right_start+right].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//         animation_colours[right_start+right].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//       }

//       uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//       uint8_t top_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size+ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       for(int top=0;top<top_size;top++){
//         animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//       }

//       uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//       uint8_t left_start = top_start + top_size;
//       for(int left=0;left<left_size;left++){
//         // progress = (float)(left)/(float)(left_size);
//         // colour_tmp = RgbColor::LinearBlend(RgbColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//         //                                    RgbColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//         //                                    progress);
//         // animation_colours[left_start+left].DesiredColour = colour_tmp;
//         animation_colours[left_start+left].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].left.colour;;
//       }

//       // animation_colours[0].DesiredColour = RgbColor(255,0,0);
//       // animation_colours[43].DesiredColour = RgbColor(255,0,0);

//       // animation_colours[44].DesiredColour = RgbColor(0,255,0);
//       // animation_colours[66].DesiredColour = RgbColor(0,255,0);

//       // animation_colours[67].DesiredColour = RgbColor(0,0,255);
//       // animation_colours[108].DesiredColour = RgbColor(0,0,255);

//       // animation_colours[109].DesiredColour = RgbwColor(0,0,0,255);
//       // animation_colours[130].DesiredColour = RgbwColor(0,0,0,255);
      
//       // animation_colours[131].DesiredColour = RgbwColor(255,0,0,255);
//       // animation_colours[132].DesiredColour = RgbwColor(0,255,0,255);
//       // animation_colours[133].DesiredColour = RgbwColor(0,0,255,255);
//       // animation_colours[131].DesiredColour = RgbwColor(255,0,255,255);
      
//       }
//       break;
//       case 1: //blend top and bottom colour only, sides are linear
//       {


//       float progress;
//       RgbcctColor colour_tmp;

//       uint8_t bottom_size = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       uint8_t bottom_start = 0;
//       for(int bottom=0;bottom<bottom_size;bottom++){
//         animation_colours[bottom_start+bottom].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].bottom.colour;
//       }

//       uint8_t right_size = ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       uint8_t right_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size;
//       for(int right=0;right<right_size;right++){
//         if(ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage>=0){ //if set
//           progress = (float)(right)/(float)(right_size);
//           colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                              RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                              progress);
//         }
//         animation_colours[right_start+right].DesiredColour = colour_tmp;//ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//         // animation_colours[right_start+right].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].right.colour;
//       }

//       uint8_t top_size = ambilightsettings.screens[SCREEN_CENTRE].top.size;
//       uint8_t top_start = ambilightsettings.screens[SCREEN_CENTRE].bottom.size+ambilightsettings.screens[SCREEN_CENTRE].right.size;
//       for(int top=0;top<top_size;top++){
//         animation_colours[top_start+top].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].top.colour;
//       }

//       uint8_t left_size = ambilightsettings.screens[SCREEN_CENTRE].left.size;
//       uint8_t left_start = top_start + top_size;
//       for(int left=0;left<left_size;left++){
//         progress = (float)(left)/(float)(left_size);
//         colour_tmp = RgbcctColor::LinearBlend(RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].top.colour),
//                                            RgbcctColor(ambilightsettings.screens[SCREEN_CENTRE].bottom.colour),
//                                            progress);
//         animation_colours[left_start+left].DesiredColour = colour_tmp;
//         // animation_colours[left_start+left].DesiredColour = ambilightsettings.screens[SCREEN_CENTRE].left.colour;;
//       }

//       #ifndef ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS
//       for(int i=0;i<STRIP_SIZE_MAX;i++)
//       {
//         // if(pCONT_iLight->animation.flags.brightness_applied_during_colour_generation){
//           // animation_colours[i].DesiredColour = ApplyBrightnesstoDesiredColourWithGamma(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global());

// animation_colours[i].DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(animation_colours[i].DesiredColour, pCONT_iLight->getBriRGB_Global(), pCONT_iLight->getBriCCT_Global());

//         // }
//       }
//       #endif

//       }
//       break;
//   }//end switch

//   #endif



// }


// // uint16_t mAnimatorLight::parsesub_ModeAmbilight(){

// //    // Create local dereferenced variable
// //   // JsonObject obj = (*_obj); 

// //   int8_t tmp_id = 0;
  

// //   char buffer[50];

// // // // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// // //   if(!obj[D_NAME].isNull()){ 
// // //     const char* scenectr = obj[D_NAME];
// // //     if((tmp_id=GetSceneIDbyName(scenectr))>=0){
// // //       scene.name_id = tmp_id;
// // //       pCONT_iLight->animation.mode_id = ANIMATION_MODE_SCENE_ID;
// // //       ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_SVALUE),D_NAME,GetSceneName(buffer));
// // //       // Response_mP(S_JSON_COMMAND_SVALUE,D_NAME,GetSceneName(buffer));
// // //       data_buffer.isserviced++;
// // //     }else{
// // //       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_SVALUE),D_NAME,scenectr);
// // //     }
// // //   }

// // //   // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT

// // //   // USe pointers
// // //   //side
// // //   // struct AMBILIGHT_SCREEN_SETTINGS.SCREENS[0]::
// // //   // ambilightsettings.screens[SCREEN_CENTRE].top
// // //   //screen
  
// // //   if(!obj[F("top")][F(D_HUE)].isNull()){ 
// // //     uint16_t hue = obj[F("top")][F(D_HUE)];
// // //     // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_NVALUE),D_HUE,hue);
// // //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.H = pCONT_iLight->HUE_N2F(hue);
// // //     // ALOG_DBG(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_FVALUE),D_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_HUE,ambilightsettings.screens[SCREEN_CENTRE].top.colour.H);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("top")][F(D_SAT)].isNull()){ 
// // //     uint8_t sat = obj[F("top")][F(D_SAT)];
// // //     // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_NVALUE),D_SAT,sat);
// // //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.S = pCONT_iLight->SatN2F(sat);
// // //     // ALOG_DBG(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_FVALUE),D_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_SAT,ambilightsettings.screens[SCREEN_CENTRE].top.colour.S);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("top")][F(D_BRT)].isNull()){ 
// // //     uint8_t brt = obj[F("top")][F(D_BRT)];
// // //     // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_NVALUE),D_BRT,brt);
// // //     ambilightsettings.screens[SCREEN_CENTRE].top.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// // //     // ALOG_DBG(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_FVALUE),D_BRT,pCONT_iLight->animation.brightness);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_BRT,pCONT_iLight->animation.brightness);
// // //     data_buffer.isserviced++;
// // //   }



// // //   if(!obj[F("bottom")][F(D_HUE)].isNull()){ 
// // //     uint16_t hue = obj[F("bottom")][F(D_HUE)];
// // //     // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_NVALUE),D_HUE,hue);
// // //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H = pCONT_iLight->HUE_N2F(hue);
// // //     // ALOG_DBG(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_FVALUE),D_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_HUE,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.H);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("bottom")][F(D_SAT)].isNull()){ 
// // //     uint8_t sat = obj[F("bottom")][F(D_SAT)];
// // //     // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_NVALUE),D_SAT,sat);
// // //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S = pCONT_iLight->SatN2F(sat);
// // //     // ALOG_DBG(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_FVALUE),D_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_SAT,ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.S);
// // //     data_buffer.isserviced++;
// // //   }
// // //   if(!obj[F("bottom")][F(D_BRT)].isNull()){ 
// // //     uint8_t brt = obj[F("bottom")][F(D_BRT)];
// // //     // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_NVALUE),D_BRT,brt);
// // //     ambilightsettings.screens[SCREEN_CENTRE].bottom.colour.B = pCONT_iLight->animation.brightness = pCONT_iLight->BrtN2F(brt);
// // //     // ALOG_DBG(PSTR(D_LOG_NEO D_PARSING_MATCHED D_COMMAND_FVALUE),D_BRT,pCONT_iLight->animation.brightness);
// // //     // Response_mP(S_JSON_COMMAND_FVALUE,D_BRT,pCONT_iLight->animation.brightness);
// // //     data_buffer.isserviced++;
// // //   }




// // //   if(!obj[D_RGB].isNull()){
// // //     const char* rgbpacked = obj[D_RGB];
// // //     uint32_t colour32bit = 0;
// // //     if(rgbpacked[0]=='#'){ colour32bit = (long) strtol( &rgbpacked[1], NULL, 16);
// // //     }else{ colour32bit = (long) strtol( &rgbpacked[0], NULL, 16); }
// // //     RgbColor rgb;
// // //     rgb.R = colour32bit >> 16; //RGB
// // //     rgb.G = colour32bit >> 8 & 0xFF; //RGB
// // //     rgb.B = colour32bit & 0xFF; //RGB
// // //     // #ifndef ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// // //     // scene.colour = HsbColor(RgbColor(rgb.R,rgb.G,rgb.B));
// // //     // // ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_RGB ":%s " D_NEOPIXEL_RGB ":%d,%d,%d " D_NEOPIXEL_HSB ":%d,%d,%d"),
// // //     // //   rgbpacked,rgb.R,rgb.G,rgb.B,scene.colour.H,scene.colour.S,scene.colour.B);
// // //     // #endif //ENABLE_DEVFEATURE_LIGHTING_SCENE_OBJECT_TO_STRUCT
// // //   }

// // //   // TIME with different units
// // //   if(!obj[D_TIME].isNull()){ //default to secs
// // //     pCONT_iLight->animation.time_ms.val = obj["time"];
// // //     pCONT_iLight->animation.time_ms.val *= 1000;
// // //     ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.time_ms.val);  
// // //   }else
// // //   if(!obj[D_TIME].isNull()){
// // //     pCONT_iLight->animation.time_ms.val = obj["time_secs"];
// // //     pCONT_iLight->animation.time_ms.val *= 1000;
// // //     ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.time_ms.val);  
// // //   }else
// // //   if(!obj[D_TIME_MS].isNull()){
// // //     pCONT_iLight->animation.time_ms.val = obj["time_ms"];
// // //     ALOG_INF(PSTR(D_LOG_NEO D_PARSING_MATCHED D_NEOPIXEL_TIME "%d" D_UNIT_MILLISECOND),pCONT_iLight->animation.time_ms.val);  
// // //   }

  
// // // Flash colour needs to NOT be a scene, but part of a manual direct mode
// // // if(strstr(scenectr,"FLASHCOLOUR")){
// // //       scene_reseting.name_id = scene.name_id; // remember scene to return to
// // //       scene.name_id = MODE_SINGLECOLOUR_FLASHCOLOUR_ID;
// // //     }
// // //     // Parse out flashcolour info if it exists
// // //     if(scene.name_id == MODE_SINGLECOLOUR_FLASHCOLOUR_ID){

// // //       if(tempctr = obj["hue"]){
// // //         scene_reseting.hue = obj["hue"];
// // //       }
// // //       if(tempctr = obj["sat"]){
// // //         scene_reseting.sat = obj["sat"];
// // //       }
// // //       if(tempctr = obj["brt"]){
// // //         scene_reseting.brt = obj["brt"];
// // //       }
// // //       if(tempctr = obj["time"]){
// // //         scene_reseting.tOnTime = obj["time"];
// // //       }
// // //     }
  
  



// // //   // create easier names
// // //   // char* topic_ctr = data_buffer.topic.ctr;
// // //   // uint8_t topic_len = data_buffer.topic.len;
// // //   // char* payload_ctr = data_buffer.payload.ctr;
// // //   // uint8_t payload_len = data_buffer.payload.len;

// // //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/center",sizeof("/center")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /center");

// // //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// // //     uint16_t index = 0;

// // //     ambilightsettings.colour.found_idx = 0;

// // //     StaticJsonDocument<300> doc;
// // //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// // //     JsonObject root = doc.as<JsonObject>();

// // //     if(root["RGB"].is<JsonArray>()){

// // //       JsonArray colourarray = root["RGB"];
// // //       int array_length = colourarray.size();

// // //       const char* pixelcolour;
// // //       for(JsonVariantConst v : colourarray) {
// // //         pixelcolour = v.as<const char*>();

// // //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //           ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB


// // //         ambilightsettings.colour.found_idx++;
// // //         index++;
// // //       }
// // //       pCONT->mso->MessagePrint("ENDambilightsettings.col.found_idx");
// // //       pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //       pCONT->mso->MessagePrintln(index);

// // //     }
// // //     // else if(root["RGB"].is<const char*>()){ //one colour = string
// // //     //
// // //     //   const char* pixelcolour;
// // //     //   pixelcolour = root["RGB"];
// // //     //
// // //     //   //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// // //     //   if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //     //   }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }
// // //     //
// // //     //   ambilightsettings.colour.r[ambilightsettings.colour.found_idx] = colour32bit >> 16; //RGB
// // //     //   ambilightsettings.colour.g[ambilightsettings.colour.found_idx] = colour32bit >> 8 & 0xFF; //RGB
// // //     //   ambilightsettings.colour.b[ambilightsettings.colour.found_idx] = colour32bit & 0xFF; //RGB
// // //     //
// // //     //   // Serial.println(parsed.col.r[parsed.col.found_idx]);
// // //     //   // Serial.println(parsed.col.g[parsed.col.found_idx]);
// // //     //   // Serial.println(parsed.col.b[parsed.col.found_idx]);
// // //     //
// // //     //   ambilightsettings.colour.found_idx++;
// // //     // }

// // //     pCONT->mso->MessagePrint("ambilightsettings.colour.found_idx");
// // //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //     //pCONT_iLight->settings.light_size_count
// // //     //ambilightsettings.colour.found_idx
// // //     for(int i=0;i<index;i++){
// // //         SetPixelColor(i, RgbColor(ambilightsettings.colour.rgb[i].R,ambilightsettings.colour.rgb[i].G,ambilightsettings.colour.rgb[i].B));
// // //     }stripbus->Show();


// // // } // END center


// // // #ifdef DEVICE_RGBDELL

// // //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/left",sizeof("/left")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /left");

// // //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// // //     ambilightsettings.colour.found_idx = 0;

// // //     StaticJsonDocument<300> doc;
// // //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// // //     JsonObject root = doc.as<JsonObject>();

// // //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// // //     if(root["RGB"].is<JsonArray>()){
// // //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// // //       JsonArray colourarray = root["RGB"];
// // //       int array_length = colourarray.size();

// // //       const char* pixelcolour;
// // //       for(JsonVariantConst v : colourarray) {
// // //         pixelcolour = v.as<const char*>();

// // //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// // //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// // //         ambilightsettings.colour.found_idx++;
// // //       }

// // //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// // //       const char* pixelcolour;
// // //       pixelcolour = root["RGB"];

// // //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// // //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// // //       ambilightsettings.colour.found_idx++;
// // //     }

// // //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //     //pCONT_iLight->settings.light_size_count
// // //     //ambilightsettings.colour.found_idx
// // //     //  for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// // //     //    SetPixelColor(i,RgbColor(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// // //     //  }
// // //     // /stripbus->Show();

// // //     uint32_t c; //colourrgb
// // //     pinMode(RGB_DATA_LEFT_PIN,OUTPUT);
// // //     pinMode(RGB_CLOCK_LEFT_PIN,OUTPUT);

// // //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);
// // //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// // //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// // //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// // //       shiftOut(RGB_DATA_LEFT_PIN, RGB_CLOCK_LEFT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// // //     }
// // //     digitalWrite(RGB_DATA_LEFT_PIN,LOW);digitalWrite(RGB_CLOCK_LEFT_PIN,LOW);



// // // } // END left



// // //   if(mSupport::memsearch(data_buffer.topic.ctr,data_buffer.topic.len,"/right",sizeof("/right")-1)>=0){pCONT->mso->MessagePrintln("MATCHED /right");

// // //     memset(&parsed,0,sizeof(parsed)); // clear parsing struct

// // //     ambilightsettings.colour.found_idx = 0;

// // //     StaticJsonDocument<300> doc;
// // //     DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// // //     JsonObject root = doc.as<JsonObject>();

// // //   // PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM - PARSE PIXEL NUM -

// // //     if(root["RGB"].is<JsonArray>()){
// // //       //pCONT->mso->MessagePrintln("colour arr ");//Serial.println(rgbname_ctr);

// // //       JsonArray colourarray = root["RGB"];
// // //       int array_length = colourarray.size();

// // //       const char* pixelcolour;
// // //       for(JsonVariantConst v : colourarray) {
// // //         pixelcolour = v.as<const char*>();

// // //         //pCONT->mso->MessagePrintln("pixelcolour"); //pCONT->mso->MessagePrintln(pixelcolour);

// // //         if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //         }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //         ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrint(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G);pCONT->mso->MessagePrint("-");
// // //          pCONT->mso->MessagePrintln(ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B);

// // //         ambilightsettings.colour.found_idx++;
// // //       }

// // //     }else if(root["RGB"].is<const char*>()){ //one colour = string

// // //       const char* pixelcolour;
// // //       pixelcolour = root["RGB"];

// // //       //Serial.println("pixelcolour"); Serial.println(pixelcolour);
// // //       if(pixelcolour[0]=='#'){ colour32bit = (long) strtol( &pixelcolour[1], NULL, 16);
// // //       }else{ colour32bit = (long) strtol( &pixelcolour[0], NULL, 16); }

// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].R = colour32bit >> 16; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].G = colour32bit >> 8 & 0xFF; //RGB
// // //       ambilightsettings.colour.rgb[ambilightsettings.colour.found_idx].B = colour32bit & 0xFF; //RGB

// // //       // Serial.println(parsed.col.r[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.g[parsed.col.found_idx]);
// // //       // Serial.println(parsed.col.b[parsed.col.found_idx]);

// // //       ambilightsettings.colour.found_idx++;
// // //     }

// // //     pCONT->mso->MessagePrintln(ambilightsettings.colour.found_idx);
// // //     //pCONT_iLight->settings.light_size_count
// // //     //ambilightsettings.colour.found_idx
// // //     // for(int i=0;i<ambilightsettings.colour.found_idx;i++){
// // //     //   mrgbneo_ani->setPixelColor(i,mrgbneo_ani->Color(ambilightsettings.colour.r[i],ambilightsettings.colour.g[i],ambilightsettings.colour.b[i]));    //turn every third pixel on
// // //     // }
// // //     // mrgbneo_ani->setBrightness(255);
// // //     // mrgbneo_ani->show();

// // //     uint32_t c; //colourrgb
// // //     pinMode(RGB_DATA_RIGHT_PIN,OUTPUT);
// // //     pinMode(RGB_CLOCK_RIGHT_PIN,OUTPUT);

// // //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);
// // //     for(int ii=0;ii<ambilightsettings.colour.found_idx;ii++){
// // //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].R);
// // //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].G);
// // //       shiftOut(RGB_DATA_RIGHT_PIN, RGB_CLOCK_RIGHT_PIN, MSBFIRST, (uint8_t)ambilightsettings.colour.rgb[ii].B);
// // //     }
// // //     digitalWrite(RGB_DATA_RIGHT_PIN,LOW);digitalWrite(RGB_CLOCK_RIGHT_PIN,LOW);



// // // } // END left

// // //       //fShowPanelUpdate = true;
// // //     //}


// // // //  }

// // //   //switch modes : USE serial input stream
// // //   //send as json array?
// // //   //RGB = [[r,g,b],[r,g,b],[r,g,b]]
// // //   //SETTINGS = {pixel ratio, 17,10}{pixel direction, CW}{startposition, bottom right}{timeout,10}{minbrightness,10}{fade,0}

// // //   pCONT_iLight->animation.mode_id = MODE_AMBILIGHT_ID;
// // //   fForcePanelUpdate = true;

// //   // return 0;

// // } // END FUNCTION





  /**
   * @brief 
   * 
   * Ideas
   * 
   * (1) Flash pixels on a bus in sequence. The pixels on that bus should also have colours in 50s of RGB for easy counting.
   * (2) Enable turning on pixels within a range only (when indexed at 0, this will also include showing progress). Base/off pixels should be option, either dark red or off. drawing over should be white, with different colours showing the 50th (so first red = 50, second green = 100)
   *          50 - red
   *         100 - green
   *         150 - blue
   *         200 - magenta
   *         250 - yellow
   *         300 - cyan
   *         350 - orange
   *         400 - hotpink
   *         450 - purple
   *         500 - limegreen, then after this repeat pattern
   * (3) Indentify pixel with light sensor. A new method I should create a 3d box that the light gets inserted into/held against, with an "ident" button (starts sequence).
   *        - another button to also "remove led", so one to add and another too remove from section.
   *        - then via mqtt commands, trigger which group the current ident values are being added to (vectorise the IDs as vector of vectors) 
   * 
   * 
   * 
   */


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
uint16_t mAnimatorLight::EffectAnim__Hardware__Show_Bus()
{

  // ALOG_INF(PSTR("EffectAnim__Hardware__Show_Bus"));
  
  uint16_t lit = 1 + SEGMENT.speed;
  uint16_t unlit = 1 + SEGMENT.intensity;
  bool drawingLit = true;
  uint16_t cnt = 0;

  uint8_t buscount = pCONT_iLight->bus_manager->getNumBusses();

  for(uint8_t bus_i = 0; bus_i < buscount; bus_i++)
  {

    uint16_t start = pCONT_iLight->bus_manager->getBus(bus_i)->getStart();
    uint16_t length = pCONT_iLight->bus_manager->getBus(bus_i)->getLength();

    ALOG_INF(PSTR("EffectAnim__Hardware__Show_Bus %d/%d (%d/%d\t%d)"), bus_i, buscount, start, length, start + length);

    uint16_t hue = map(bus_i, 0, buscount, 0, 360);
    uint8_t sat = 255;

    if(bus_i%2)
      sat = 255;

    uint32_t colour = HueSatBrt(hue,sat,1.0f);

    for(unsigned i = start; i < start + length; i++)
    {
      if(i < start + 1 + bus_i) // let the number of one white pixels be the bus index
      {
        SEGMENT.setPixelColor(i, RGBW32(255,255,255,255) );
        ALOG_INF(PSTR("Bus i < start  %d (%d/%d\t%d)\ti=%d hue=%d"), bus_i, start, length, start + length, i, hue);
      }else{
        SEGMENT.setPixelColor(i, colour);
      }
    }


  }

  
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__SHOW_BUS[] PROGMEM = "Debug Visualise Bus@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
uint16_t mAnimatorLight::EffectAnim__Hardware__Manual_Pixel_Counting()
{
  
  SEGMENT.fill(RGBW32(5,5,5,0));

  uint16_t hue_list[] = {0,120,240,340,50,180,14,350,280};
  uint8_t used_hue = 0;  
  
  uint32_t colour = 0;//RgbcctColor();


  for (uint16_t i = 0; i < SEGLEN; i++)
  {

    if((i%10)==0) // Every 10th should be bright white
    {
      
      if((i%20)==0) // If its every 50th, then use hue map instead of the bright white
      {

        float hue = (float)hue_list[used_hue++]/360.0f;
        // Serial.println(hue);

        SEGMENT.setPixelColor(i, HsbColor(hue,1.0f,1.0f));
        
        // ALOG_INF(PSTR("50i %d"), i);

        if(used_hue >= ARRAY_SIZE(hue_list))
          used_hue = 0;

      }else{

        SEGMENT.setPixelColor(i, RgbColor(50));

      }

    }


  }




  // SEGMENT.setPixelColor(0, RgbcctColor(255,0,0));
  
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__MANUAL_PIXEL_COUNTING[] PROGMEM = "Debug Pixel Counting@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
uint16_t mAnimatorLight::EffectAnim__Hardware__View_Pixel_Range()
{

  SEGMENT.fill(SEGCOLOR_U32(0));
  
  for (uint32_t i = SEGMENT.params_user[0]; i < SEGMENT.params_user[1]; i++)
  {
    SEGMENT.setPixelColor(i,SEGCOLOR_U32(1));
  }
    
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__VIEW_PIXEL_RANGE[] PROGMEM = "Debug Pixel Range@Fg size,Bg size;Fg,!;!;;pal=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @description : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
uint16_t mAnimatorLight::EffectAnim__Hardware__Light_Sensor_Pixel_Indexing()
{
  
  uint16_t lit = 1 + SEGMENT.speed;
  uint16_t unlit = 1 + SEGMENT.intensity;
  bool drawingLit = true;
  uint16_t cnt = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.setPixelColor(i, 
      (drawingLit) ? SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) : SEGCOLOR_U32(1)
    );
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }
  
  return FRAMETIME;
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING[] PROGMEM = "Debug Light Sensor Indexing@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING


/*******************************************************************************************************************************************************************************************************************
 * @description : Manual__PixelSetElsewhere
 * @note : Temporary effect before realtime can take over
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
uint16_t mAnimatorLight::EffectAnim__Manual__PixelSetElsewhere()
{  
  return FRAMETIME; 
}
static const char PM_EFFECT_CONFIG__MANUAL__PIXEL_SET_ELSEWHERE__INDEXING[] PROGMEM = "Debug Pixel Set Manually@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE



//***************************  2D routines  ***********************************
#ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX //////////////////////////////////////////////////////////////////////////////////////////////////////

#define XY(x,y) SEGMENT.XY(x,y)

/*******************************************************************************************************************************************************************************************************************
 * @description : By: Stepko https://editor.soulmatelights.com/gallery/1012 , Modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DBlackHole"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Blackhole()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;
  int x, y;

  SEGMENT.fadeToBlackBy(16 + (SEGMENT.speed>>3)); // create fading trails
  unsigned long t = effect_start_time/128;                 // timebase
  // outer stars
  for (size_t i = 0; i < 8; i++) {
    x = beatsin8_t(SEGMENT.custom1>>3,   0, cols - 1, 0, ((i % 2) ? 128 : 0) + t * i);
    y = beatsin8_t(SEGMENT.intensity>>3, 0, rows - 1, 0, ((i % 2) ? 192 : 64) + t * i);
    SEGMENT.addPixelColorXY(x, y, SEGMENT.color_from_palette(i*32, false, PALETTE_SOLID_WRAP, SEGMENT.check1?0:255));
  }
  // inner stars
  for (size_t i = 0; i < 4; i++) {
    x = beatsin8_t(SEGMENT.custom2>>3, cols/4, cols - 1 - cols/4, 0, ((i % 2) ? 128 : 0) + t * i);
    y = beatsin8_t(SEGMENT.custom3   , rows/4, rows - 1 - rows/4, 0, ((i % 2) ? 192 : 64) + t * i);
    SEGMENT.addPixelColorXY(x, y, SEGMENT.color_from_palette(255-i*64, false, PALETTE_SOLID_WRAP, SEGMENT.check1?0:255));
  }
  // central white dot
  SEGMENT.setPixelColorXY(cols/2, rows/2, WHITE);
  // blur everything a bit
  if (SEGMENT.check3) SEGMENT.blur(16, cols*rows < 100);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__BLACK_HOLE__INDEXING[] PROGMEM = "Black Hole@Fade rate,Outer Y freq.,Outer X freq.,Inner X freq.,Inner Y freq.,Solid,,Blur;!;!;2;ix=127,pal=11";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: ldirko   https://editor.soulmatelights.com/gallery/819-colored-bursts , modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DColoredBursts"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__ColouredBursts()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.aux0 = 0; // start with red hue
  }

  bool dot = SEGMENT.check3;
  bool grad = SEGMENT.check1;

  byte numLines = SEGMENT.intensity/16 + 1;

  SEGMENT.aux0++;  // hue
  SEGMENT.fadeToBlackBy(40 - SEGMENT.check2 * 8);
  for (size_t i = 0; i < numLines; i++) {
    byte x1 = beatsin8_t(2 + SEGMENT.speed/16, 0, (cols - 1));
    byte x2 = beatsin8_t(1 + SEGMENT.speed/16, 0, (rows - 1));
    byte y1 = beatsin8_t(5 + SEGMENT.speed/16, 0, (cols - 1), 0, i * 24);
    byte y2 = beatsin8_t(3 + SEGMENT.speed/16, 0, (rows - 1), 0, i * 48 + 64);
    uint32_t color = ColorFromPalette(SEGPALETTE, i * 255 / numLines + (SEGMENT.aux0&0xFF), 255, LINEARBLEND);

    byte xsteps = abs8(x1 - y1) + 1;
    byte ysteps = abs8(x2 - y2) + 1;
    byte steps = xsteps >= ysteps ? xsteps : ysteps;
    //Draw gradient line
    for (size_t j = 1; j <= steps; j++) {
      uint8_t rate = j * 255 / steps;
      byte dx = lerp8by8(x1, y1, rate);
      byte dy = lerp8by8(x2, y2, rate);
      //SEGMENT.setPixelColorXY(dx, dy, grad ? color.nscale8_video(255-rate) : color); // use addPixelColorXY for different look
      SEGMENT.addPixelColorXY(dx, dy, color); // use setPixelColorXY for different look
      if (grad) SEGMENT.fadePixelColorXY(dx, dy, rate);
    }

    if (dot) { //add white point at the ends of line
      SEGMENT.setPixelColorXY(x1, x2, WHITE);
      SEGMENT.setPixelColorXY(y1, y2, DARKSLATEGRAY);
    }
  }
  SEGMENT.blur(SEGMENT.custom3>>1, SEGMENT.check2);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__COLOURED_BURSTS__INDEXING[] PROGMEM = "Colored Bursts@Speed,# of lines,,,Blur,Gradient,,Dots;;!;2;c3=16";


/*******************************************************************************************************************************************************************************************************************
 * @description : dna originally by by ldirko at https://pastebin.com/pCkkkzcs. Updated by Preyy. WLED conversion by Andrew Tuline.
 * @note : Converted from WLED Effects "mode_2Ddna"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__DNA()
{    
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  SEGMENT.fadeToBlackBy(64);
  for (int i = 0; i < cols; i++) {
    SEGMENT.setPixelColorXY(i, beatsin8_t(SEGMENT.speed/8, 0, rows-1, 0, i*4    ), ColorFromPalette(SEGPALETTE, i*5+effect_start_time/17, beatsin8_t(5, 55, 255, 0, i*10), LINEARBLEND));
    SEGMENT.setPixelColorXY(i, beatsin8_t(SEGMENT.speed/8, 0, rows-1, 0, i*4+128), ColorFromPalette(SEGPALETTE, i*5+128+effect_start_time/17, beatsin8_t(5, 55, 255, 0, i*10+128), LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.intensity / (8 - (SEGMENT.check1 * 2)), SEGMENT.check1);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__DNA__INDEXING[] PROGMEM = "DNA@Scroll speed,Blur,,,,Smear;;!;2;ix=0,sx=1";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: ldirko  https://editor.soulmatelights.com/gallery/512-dna-spiral-variation , modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DDNASpiral"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__DNASpiral()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  unsigned speeds = SEGMENT.speed/2 + 7;
  unsigned freq = SEGMENT.intensity/8;

  uint32_t ms = effect_start_time / 20;
  SEGMENT.fadeToBlackBy(135);

  for (int i = 0; i < rows; i++) {
    int x  = beatsin8_t(speeds, 0, cols - 1, 0, i * freq) + beatsin8_t(speeds - 7, 0, cols - 1, 0, i * freq + 128);
    int x1 = beatsin8_t(speeds, 0, cols - 1, 0, 128 + i * freq) + beatsin8_t(speeds - 7, 0, cols - 1, 0, 128 + 64 + i * freq);
    unsigned hue = (i * 128 / rows) + ms;
    // skip every 4th row every now and then (fade it more)
    if ((i + ms / 8) & 3) {
      // draw a gradient line between x and x1
      x = x / 2; x1 = x1 / 2;
      unsigned steps = abs8(x - x1) + 1;
      bool positive = (x1 >= x);                         // direction of drawing
      for (size_t k = 1; k <= steps; k++) {
        unsigned rate = k * 255 / steps;
        //unsigned dx = lerp8by8(x, x1, rate);
        unsigned dx = positive? (x + k-1) : (x - k+1);   // behaves the same as "lerp8by8" but does not create holes
        //SEGMENT.setPixelColorXY(dx, i, ColorFromPalette(SEGPALETTE, hue, 255, LINEARBLEND).nscale8_video(rate));
        SEGMENT.addPixelColorXY(dx, i, ColorFromPalette(SEGPALETTE, hue, 255, LINEARBLEND)); // use setPixelColorXY for different look
        SEGMENT.fadePixelColorXY(dx, i, rate);
      }
      SEGMENT.setPixelColorXY(x, i, DARKSLATEGRAY);
      SEGMENT.setPixelColorXY(x1, i, WHITE);
    }
  }
  SEGMENT.blur(((uint16_t)SEGMENT.custom1 * 3) / (6 + SEGMENT.check1), SEGMENT.check1);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__DNA_SPIRAL__INDEXING[] PROGMEM = "DNA Spiral@Scroll speed,Y frequency,Blur,,,Smear;;!;2;c1=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: Stepko   https://editor.soulmatelights.com/gallery/884-drift , Modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DDrift"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Drift()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  const int colsCenter = (cols>>1) + (cols%2);
  const int rowsCenter = (rows>>1) + (rows%2);

  SEGMENT.fadeToBlackBy(128);
  const float maxDim = MAX(cols, rows)/2;
  unsigned long t = effect_start_time / (32 - (SEGMENT.speed>>3));
  unsigned long t_20 = t/20; // softhack007: pre-calculating this gives about 10% speedup
  for (float i = 1.0f; i < maxDim; i += 0.25f) {
    float angle = radians(t * (maxDim - i));
    int mySin = sin_t(angle) * i;
    int myCos = cos_t(angle) * i;
    SEGMENT.setPixelColorXY(colsCenter + mySin, rowsCenter + myCos, ColorFromPalette(SEGPALETTE, (i * 20) + t_20, 255, LINEARBLEND));
    if (SEGMENT.check1) SEGMENT.setPixelColorXY(colsCenter + myCos, rowsCenter + mySin, ColorFromPalette(SEGPALETTE, (i * 20) + t_20, 255, LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.intensity>>(3 - SEGMENT.check2), SEGMENT.check2);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__DRIFT__INDEXING[] PROGMEM = "Drift@Rotation speed,Blur,,,,Twin,Smear;;!;2;ix=0";

/*******************************************************************************************************************************************************************************************************************
 * @description : firenoise2d. By Andrew Tuline. Yet another short routine.
 * @note : Converted from WLED Effects "mode_2Dfirenoise"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__FireNoise()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  unsigned xscale = SEGMENT.intensity*4;
  unsigned yscale = SEGMENT.speed*8;
  unsigned indexx = 0;

  CRGBPalette16 pal = SEGPALETTE;//SEGMENT.check1 ? SEGPALETTE : SEGMENT.loadPalette(pal, 35);  
  for (int j=0; j < cols; j++) {
    for (int i=0; i < rows; i++) {
      indexx = inoise8(j*yscale*rows/255, i*xscale+effect_start_time/4);                                               // We're moving along our Perlin map.
      SEGMENT.setPixelColorXY(j, i, ColorFromPalette(pal, min(i*indexx/11, 225U), i*255/rows, LINEARBLEND));   // With that value, look up the 8 bit colour palette value and assign it to the current LED.    
    } // for i
  } // for j

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__FIRE_NOISE__INDEXING[] PROGMEM = "Firenoise@X scale,Y scale,,,,Palette;;!;2;pal=66";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: Stepko https://editor.soulmatelights.com/gallery/640-color-frizzles , Modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DFrizzles"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Frizzles()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  SEGMENT.fadeToBlackBy(16 + SEGMENT.check1 * 10);
  for (size_t i = 8; i > 0; i--) {
    SEGMENT.addPixelColorXY(beatsin8_t(SEGMENT.speed/8 + i, 0, cols - 1),
                            beatsin8_t(SEGMENT.intensity/8 - i, 0, rows - 1),
                            ColorFromPalette(SEGPALETTE, beatsin8_t(12, 0, 255), 255, LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.custom1 >> (3 + SEGMENT.check1), SEGMENT.check1);
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__FRIZZLES__INDEXING[] PROGMEM = "Frizzles@X frequency,Y frequency,Blur,,,Smear;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : 2D Cellular Automata Game of life
 *                Written by Ewoud Wijma, inspired by https://natureofcode.com/book/chapter-7-cellular-automata/ and https://github.com/DougHaber/nlife-color
 * @note : Converted from WLED Effects "mode_2Dgameoflife"
 ********************************************************************************************************************************************************************************************************************/
typedef struct ColorCount {
  CRGB color;
  int8_t count;
} colorCount;
uint16_t mAnimatorLight::EffectAnim__2D__GameOfLife()
{
if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;
  const unsigned dataSize = sizeof(CRGB) * SEGMENT.length();  // using width*height prevents reallocation if mirroring is enabled
  const int crcBufferLen = 2; //(SEGMENT.width() + SEGMENT.height())*71/100; // roughly sqrt(2)/2 for better repetition detection (Ewowi)

  if (!SEGMENT.allocateData(dataSize + sizeof(uint16_t)*crcBufferLen)) return EFFECT_DEFAULT(); //allocation failed
  CRGB *prevLeds = reinterpret_cast<CRGB*>(SEGMENT.data);
  uint16_t *crcBuffer = reinterpret_cast<uint16_t*>(SEGMENT.data + dataSize); 

  CRGB backgroundColor = SEGCOLOR(1);

  if (SEGMENT.call == 0 || effect_start_time - SEGMENT.step > 3000) {
    SEGMENT.step = effect_start_time;
    SEGMENT.aux0 = 0;

    //give the leds random state and colors (based on intensity, colors from palette or all posible colors are chosen)
    for (int x = 0; x < cols; x++) for (int y = 0; y < rows; y++) {
      unsigned state = hw_random8()%2;
      if (state == 0)
        SEGMENT.setPixelColorXY(x,y, backgroundColor);
      else
        SEGMENT.setPixelColorXY(x,y, SEGMENT.color_from_palette(hw_random8(), false, PALETTE_SOLID_WRAP, 255));
    }

    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) prevLeds[XY(x,y)] = CRGB::Black;
    memset(crcBuffer, 0, sizeof(uint16_t)*crcBufferLen);
  } else if (effect_start_time - SEGMENT.step < FRAMETIME * (uint32_t)map(SEGMENT.speed,0,255,64,4)) {
    // update only when appropriate time passes (in 42 FPS slots)
    return FRAMETIME;
  }

  //copy previous leds (save previous generation)
  //NOTE: using lossy getPixelColor() is a benefit as endlessly repeating patterns will eventually fade out causing a reset
  for (int x = 0; x < cols; x++) for (int y = 0; y < rows; y++) prevLeds[XY(x,y)] = SEGMENT.getPixelColorXY(x,y);

  //calculate new leds
  for (int x = 0; x < cols; x++) for (int y = 0; y < rows; y++) {

    colorCount colorsCount[9]; // count the different colors in the 3*3 matrix
    for (int i=0; i<9; i++) colorsCount[i] = {backgroundColor, 0}; // init colorsCount

    // iterate through neighbors and count them and their different colors
    int neighbors = 0;
    for (int i = -1; i <= 1; i++) for (int j = -1; j <= 1; j++) { // iterate through 3*3 matrix
      if (i==0 && j==0) continue; // ignore itself
      // wrap around segment
      int xx = x+i, yy = y+j;
      if (x+i < 0) xx = cols-1; else if (x+i >= cols) xx = 0;
      if (y+j < 0) yy = rows-1; else if (y+j >= rows) yy = 0;

      unsigned xy = XY(xx, yy); // previous cell xy to check
      // count different neighbours and colors
      if (prevLeds[xy] != backgroundColor) {
        neighbors++;
        bool colorFound = false;
        int k;
        for (k=0; k<9 && colorsCount[i].count != 0; k++)
          if (colorsCount[k].color == prevLeds[xy]) {
            colorsCount[k].count++;
            colorFound = true;
          }
        if (!colorFound) colorsCount[k] = {prevLeds[xy], 1}; //add new color found in the array
      }
    } // i,j

    // Rules of Life
    uint32_t col = uint32_t(prevLeds[XY(x,y)]) & 0x00FFFFFF;  // uint32_t operator returns RGBA, we want RGBW -> cut off "alpha" byte
    uint32_t bgc = RGBW32(backgroundColor.r, backgroundColor.g, backgroundColor.b, 0);
    if      ((col != bgc) && (neighbors <  2)) SEGMENT.setPixelColorXY(x,y, bgc); // Loneliness
    else if ((col != bgc) && (neighbors >  3)) SEGMENT.setPixelColorXY(x,y, bgc); // Overpopulation
    else if ((col == bgc) && (neighbors == 3)) {                                  // Reproduction
      // find dominant color and assign it to a cell
      colorCount dominantColorCount = {backgroundColor, 0};
      for (int i=0; i<9 && colorsCount[i].count != 0; i++)
        if (colorsCount[i].count > dominantColorCount.count) dominantColorCount = colorsCount[i];
      // assign the dominant color w/ a bit of randomness to avoid "gliders"
      if (dominantColorCount.count > 0 && hw_random8(128)) SEGMENT.setPixelColorXY(x,y, dominantColorCount.color);
    } else if ((col == bgc) && (neighbors == 2) && !hw_random8(128)) {               // Mutation
      SEGMENT.setPixelColorXY(x,y, SEGMENT.color_from_palette(hw_random8(), false, PALETTE_SOLID_WRAP, 255));
    }
    // else do nothing!
  } //x,y

  // calculate CRC16 of leds
  uint16_t crc = crc16((const unsigned char*)prevLeds, dataSize);
  // check if we had same CRC and reset if needed
  bool repetition = false;
  for (int i=0; i<crcBufferLen && !repetition; i++) repetition = (crc == crcBuffer[i]); // (Ewowi)
  // same CRC would mean image did not change or was repeating itself
  if (!repetition) SEGMENT.step = effect_start_time; //if no repetition avoid reset
  // remember CRCs across frames
  crcBuffer[SEGMENT.aux0] = crc;
  ++SEGMENT.aux0 %= crcBufferLen;

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__GAME_OF_LIFE__INDEXING[] PROGMEM = "Game Of Life@!;!,!;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: ldirko  https://editor.soulmatelights.com/gallery/810 , Modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DHiphotic"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Hipnotic()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;
  const uint32_t a = effect_start_time / ((SEGMENT.custom3>>1)+1);

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      SEGMENT.setPixelColorXY(x, y, SEGMENT.color_from_palette(sin8_t(cos8_t(x * SEGMENT.speed/16 + a / 3) + sin8_t(y * SEGMENT.intensity/16 + a / 4) + a), false, PALETTE_SOLID_WRAP, 0));
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__HIPNOTIC__INDEXING[] PROGMEM = "Hiphotic@X scale,Y scale,,,Speed;!;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : An animated Julia set by Andrew Tuline.
 * @note : Converted from WLED Effects "mode_2DJulia"
 ********************************************************************************************************************************************************************************************************************/
// Sliders are:
// intensity = Maximum number of iterations per pixel.
// Custom1 = Location of X centerpoint
// Custom2 = Location of Y centerpoint
// Custom3 = Size of the area (small value = smaller area)
typedef struct Julia {
  float xcen;
  float ycen;
  float xymag;
} julia;
uint16_t mAnimatorLight::EffectAnim__2D__Julia()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (!SEGMENT.allocateData(sizeof(julia))) return EFFECT_DEFAULT();
  Julia* julias = reinterpret_cast<Julia*>(SEGMENT.data);

  float reAl;
  float imAg;

  if (SEGMENT.call == 0) {           // Reset the center if we've just re-started this animation.
    julias->xcen = 0.;
    julias->ycen = 0.;
    julias->xymag = 1.0;

    SEGMENT.custom1 = 128;              // Make sure the location widgets are centered to start.
    SEGMENT.custom2 = 128;
    SEGMENT.custom3 = 16;
    SEGMENT.intensity = 24;
  }

  julias->xcen  = julias->xcen  + (float)(SEGMENT.custom1 - 128)/100000.f;
  julias->ycen  = julias->ycen  + (float)(SEGMENT.custom2 - 128)/100000.f;
  julias->xymag = julias->xymag + (float)((SEGMENT.custom3 - 16)<<3)/100000.f; // reduced resolution slider
  if (julias->xymag < 0.01f) julias->xymag = 0.01f;
  if (julias->xymag > 1.0f) julias->xymag = 1.0f;

  float xmin = julias->xcen - julias->xymag;
  float xmax = julias->xcen + julias->xymag;
  float ymin = julias->ycen - julias->xymag;
  float ymax = julias->ycen + julias->xymag;

  // Whole set should be within -1.2,1.2 to -.8 to 1.
  xmin = constrain(xmin, -1.2f, 1.2f);
  xmax = constrain(xmax, -1.2f, 1.2f);
  ymin = constrain(ymin, -0.8f, 1.0f);
  ymax = constrain(ymax, -0.8f, 1.0f);

  float dx;                       // Delta x is mapped to the matrix size.
  float dy;                       // Delta y is mapped to the matrix size.

  int maxIterations = 15;         // How many iterations per pixel before we give up. Make it 8 bits to match our range of colours.
  float maxCalc = 16.0;           // How big is each calculation allowed to be before we give up.

  maxIterations = SEGMENT.intensity/2;


  // Resize section on the fly for some animaton.
  reAl = -0.94299f;               // PixelBlaze example
  imAg = 0.3162f;

  reAl += (float)sin16_t(effect_start_time * 34) / 655340.f;
  imAg += (float)sin16_t(effect_start_time * 26) / 655340.f;

  dx = (xmax - xmin) / (cols);     // Scale the delta x and y values to our matrix size.
  dy = (ymax - ymin) / (rows);

  // Start y
  float y = ymin;
  for (int j = 0; j < rows; j++) {

    // Start x
    float x = xmin;
    for (int i = 0; i < cols; i++) {

      // Now we test, as we iterate z = z^2 + c does z tend towards infinity?
      float a = x;
      float b = y;
      int iter = 0;

      while (iter < maxIterations) {    // Here we determine whether or not we're out of bounds.
        float aa = a * a;
        float bb = b * b;
        float len = aa + bb;
        if (len > maxCalc) {            // |z| = sqrt(a^2+b^2) OR z^2 = a^2+b^2 to save on having to perform a square root.
          break;  // Bail
        }

       // This operation corresponds to z -> z^2+c where z=a+ib c=(x,y). Remember to use 'foil'.
        b = 2*a*b + imAg;
        a = aa - bb + reAl;
        iter++;
      } // while

      // We color each pixel based on how long it takes to get to infinity, or black if it never gets there.
      if (iter == maxIterations) {
        SEGMENT.setPixelColorXY(i, j, 0);
      } else {
        SEGMENT.setPixelColorXY(i, j, SEGMENT.color_from_palette(iter*255/maxIterations, false, PALETTE_SOLID_WRAP, 0));
      }
      x += dx;
    }
    y += dy;
  }
  if(SEGMENT.check1)
    SEGMENT.blur(100, true);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__JULIA__INDEXING[] PROGMEM = "Julia@,Max iterations per pixel,X center,Y center,Area size, Blur;!;!;2;ix=24,c1=128,c2=128,c3=16";

 
/*******************************************************************************************************************************************************************************************************************
 * @description : By: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DLissajous"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Lissajous()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  SEGMENT.fadeToBlackBy(SEGMENT.intensity);
  uint_fast16_t phase = (effect_start_time * (1 + SEGMENT.custom3)) /32;  // allow user to control rotation speed

  //for (int i=0; i < 4*(cols+rows); i ++) {
  for (int i=0; i < 256; i ++) {
    //float xlocn = float(sin8_t(now/4+i*(SEGMENT.speed>>5))) / 255.0f;
    //float ylocn = float(cos8_t(now/4+i*2)) / 255.0f;
    uint_fast8_t xlocn = sin8_t(phase/2 + (i*SEGMENT.speed)/32);
    uint_fast8_t ylocn = cos8_t(phase/2 + i*2);
    xlocn = (cols < 2) ? 1 : (map(2*xlocn, 0,511, 0,2*(cols-1)) +1) /2;    // softhack007: "(2* ..... +1) /2" for proper rounding
    ylocn = (rows < 2) ? 1 : (map(2*ylocn, 0,511, 0,2*(rows-1)) +1) /2;    // "rows > 1" is needed to avoid div/0 in map()
    SEGMENT.setPixelColorXY((uint8_t)xlocn, (uint8_t)ylocn, SEGMENT.color_from_palette(effect_start_time/100+i, false, PALETTE_SOLID_WRAP, 0));
  }
  SEGMENT.blur(SEGMENT.custom1 >> (1 + SEGMENT.check1 * 3), SEGMENT.check1);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__LISSAJOUS__INDEXING[] PROGMEM = "Lissajous@X frequency,Fade rate,Blur,,Speed,Smear;!;!;2;c1=0";


/*******************************************************************************************************************************************************************************************************************
 * @description : Matrix2D. By Jeremy Williams. Adapted by Andrew Tuline & improved by merkisoft and ewowi, and softhack007.
 * @note : Converted from WLED Effects "mode_2Dmatrix"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Matrix()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  unsigned dataSize = (SEGMENT.length()+7) >> 3; //1 bit per LED for trails
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.step = 0;
  }

  uint8_t fade = map(SEGMENT.custom1, 0, 255, 50, 250);    // equals trail size
  uint8_t speed = (256-SEGMENT.speed) >> map(min(rows, 150), 0, 150, 0, 3);    // slower speeds for small displays

  uint32_t spawnColor;
  uint32_t trailColor;
  if (SEGMENT.check1) {
    spawnColor = SEGCOLOR(0);
    trailColor = SEGCOLOR(1);
  } else {
    spawnColor = RGBW32(175,255,175,0);
    trailColor = RGBW32(27,130,39,0);
  }

  bool emptyScreen = true;
  if (effect_start_time - SEGMENT.step >= speed) {
    SEGMENT.step = effect_start_time;
    // move pixels one row down. Falling codes keep color and add trail pixels; all others pixels are faded
    // TODO: it would be better to paint trails idividually instead of relying on fadeToBlackBy()
    SEGMENT.fadeToBlackBy(fade);
    for (int row = rows-1; row >= 0; row--) {
      for (int col = 0; col < cols; col++) {
        unsigned index = XY(col, row) >> 3;
        unsigned bitNum = XY(col, row) & 0x07;
        if (bitRead(SEGMENT.data[index], bitNum)) {
          SEGMENT.setPixelColorXY(col, row, trailColor);  // create trail
          bitClear(SEGMENT.data[index], bitNum);
          if (row < rows-1) {
            SEGMENT.setPixelColorXY(col, row+1, spawnColor);
            index = XY(col, row+1) >> 3;
            bitNum = XY(col, row+1) & 0x07;
            bitSet(SEGMENT.data[index], bitNum);
            emptyScreen = false;
          }
        }
      }
    }

    // spawn new falling code
    if (hw_random8() <= SEGMENT.intensity || emptyScreen) {
      uint8_t spawnX = hw_random8(cols);
      SEGMENT.setPixelColorXY(spawnX, 0, spawnColor);
      // update hint for next run
      unsigned index = XY(spawnX, 0) >> 3;
      unsigned bitNum = XY(spawnX, 0) & 0x07;
      bitSet(SEGMENT.data[index], bitNum);
    }
  }

  return FRAMETIME;
} 
static const char PM_EFFECT_CONFIG__2D__MATRIX__INDEXING[] PROGMEM = "Matrix@!,Spawning rate,Trail,,,Custom color;Spawn,Trail;;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : Metaballs by Stefan Petrick. Cannot have one of the dimensions be 2 or less. Adapted by Andrew Tuline.
 * @note : Converted from WLED Effects "mode_2Dmetaballs"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Metaballs()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  float speed = 0.25f * (1+(SEGMENT.speed>>6));

  // get some 2 random moving points
  int x2 = map(inoise8(effect_start_time * speed, 25355, 685), 0, 255, 0, cols-1);
  int y2 = map(inoise8(effect_start_time * speed, 355, 11685), 0, 255, 0, rows-1);

  int x3 = map(inoise8(effect_start_time * speed, 55355, 6685), 0, 255, 0, cols-1);
  int y3 = map(inoise8(effect_start_time * speed, 25355, 22685), 0, 255, 0, rows-1);

  // and one Lissajou function
  int x1 = beatsin8_t(23 * speed, 0, cols-1);
  int y1 = beatsin8_t(28 * speed, 0, rows-1);

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      // calculate distances of the 3 points from actual pixel
      // and add them together with weightening
      unsigned dx = abs(x - x1);
      unsigned dy = abs(y - y1);
      unsigned dist = 2 * sqrt16((dx * dx) + (dy * dy));

      dx = abs(x - x2);
      dy = abs(y - y2);
      dist += sqrt16((dx * dx) + (dy * dy));

      dx = abs(x - x3);
      dy = abs(y - y3);
      dist += sqrt16((dx * dx) + (dy * dy));

      // inverse result
      int color = dist ? 1000 / dist : 255;

      // map color between thresholds
      if (color > 0 and color < 60) {
        SEGMENT.setPixelColorXY(x, y, SEGMENT.color_from_palette(map(color * 9, 9, 531, 0, 255), false, PALETTE_SOLID_WRAP, 0));
      } else {
        SEGMENT.setPixelColorXY(x, y, SEGMENT.color_from_palette(0, false, PALETTE_SOLID_WRAP, 0));
      }
      // show the 3 points, too
      SEGMENT.setPixelColorXY(x1, y1, WHITE);
      SEGMENT.setPixelColorXY(x2, y2, WHITE);
      SEGMENT.setPixelColorXY(x3, y3, WHITE);
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__METABALLS__INDEXING[] PROGMEM = "Metaballs@!;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : By Andrew Tuline
 * @note : Converted from WLED Effects "mode_2Dnoise"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Noise()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  const unsigned scale  = SEGMENT.intensity+2;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      uint8_t pixelHue8 = inoise8(x * scale, y * scale, effect_start_time / (16 - SEGMENT.speed/16));
      SEGMENT.setPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, pixelHue8));
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__NOISE__INDEXING[] PROGMEM = "Noise2D@!,Scale;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: Stepko https://editor.soulmatelights.com/gallery/659-plasm-ball , Modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DPlasmaball"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__PlasmaBall()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  SEGMENT.fadeToBlackBy(SEGMENT.custom1>>2);
  uint_fast32_t t = (effect_start_time * 8) / (256 - SEGMENT.speed);  // optimized to avoid float
  for (int i = 0; i < cols; i++) {
    unsigned thisVal = inoise8(i * 30, t, t);
    unsigned thisMax = map(thisVal, 0, 255, 0, cols-1);
    for (int j = 0; j < rows; j++) {
      unsigned thisVal_ = inoise8(t, j * 30, t);
      unsigned thisMax_ = map(thisVal_, 0, 255, 0, rows-1);
      int x = (i + thisMax_ - cols / 2);
      int y = (j + thisMax - cols / 2);
      int cx = (i + thisMax_);
      int cy = (j + thisMax);

      SEGMENT.addPixelColorXY(i, j, ((x - y > -2) && (x - y < 2)) ||
                                    ((cols - 1 - x - y) > -2 && (cols - 1 - x - y < 2)) ||
                                    (cols - cx == 0) ||
                                    (cols - 1 - cx == 0) ||
                                    ((rows - cy == 0) ||
                                    (rows - 1 - cy == 0)) ? ColorFromPalette(SEGPALETTE, beat8(5), thisVal, LINEARBLEND) : CRGB::Black);
    }
  }
  SEGMENT.blur(SEGMENT.custom2>>5);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__PLASMA_BALL__INDEXING[] PROGMEM = "Plasma Ball@Speed,,Fade,Blur;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : / By: Kostyantyn Matviyevskyy  https://editor.soulmatelights.com/gallery/762-polar-lights , Modified by: Andrew Tuline & @dedehai (palette support)
 * @note : Converted from WLED Effects "mode_2DPolarLights"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__PolarLights()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.step = 0;
  }

  float adjustHeight = (float)map(rows, 8, 32, 28, 12); // maybe use mapf() ???
  unsigned adjScale = map(cols, 8, 64, 310, 63);
  unsigned _scale = map(SEGMENT.intensity, 0, 255, 30, adjScale);
  int _speed = map(SEGMENT.speed, 0, 255, 128, 16);

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      SEGMENT.step++;
      uint8_t palindex = qsub8(inoise8((SEGMENT.step%2) + x * _scale, y * 16 + SEGMENT.step % 16, SEGMENT.step / _speed), fabsf((float)rows / 2.0f - (float)y) * adjustHeight);
      uint8_t palbrightness = palindex;
      if(SEGMENT.check1) palindex = 255 - palindex; //flip palette
      SEGMENT.setPixelColorXY(x, y, SEGMENT.color_from_palette(palindex, false, false, 255, palbrightness));
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__POLAR_LIGHTS__INDEXING[] PROGMEM = "Polar Lights@!,Scale,,,,Flip Palette;;!;2;pal=71";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: ldirko   https://editor.soulmatelights.com/gallery/878-pulse-test , modifed by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DPulser"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Pulser()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  SEGMENT.fadeToBlackBy(8 - (SEGMENT.intensity>>5));
  uint32_t a = effect_start_time / (18 - SEGMENT.speed / 16);
  int x = (a / 14) % cols;
  int y = map((sin8_t(a * 5) + sin8_t(a * 4) + sin8_t(a * 2)), 0, 765, rows-1, 0);
  SEGMENT.setPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, map(y, 0, rows-1, 0, 255), 255, LINEARBLEND));

  SEGMENT.blur(SEGMENT.intensity>>4);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__PULSER__INDEXING[] PROGMEM = "Pulser@!,Blur;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: ldirko   https://editor.soulmatelights.com/gallery/597-sin-dots , modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__SinDots()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  SEGMENT.fadeToBlackBy((SEGMENT.custom1>>3) + (SEGMENT.check1 * 24));

  byte t1 = effect_start_time / (257 - SEGMENT.speed); // 20;
  byte t2 = sin8_t(t1) / 4 * 2;
  for (int i = 0; i < 13; i++) {
    int x = sin8_t(t1 + i * SEGMENT.intensity/8)*(cols-1)/255;  // max index now 255x15/255=15!
    int y = sin8_t(t2 + i * SEGMENT.intensity/8)*(rows-1)/255;  // max index now 255x15/255=15!
    SEGMENT.setPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, i * 255 / 13, 255, LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.custom2 >> (3 + SEGMENT.check1), SEGMENT.check1);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__SIN_DOTS__INDEXING[] PROGMEM = "Sindots@!,Dot distance,Fade rate,Blur,,Smear;;!;2;";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: Mark Kriegsman. https://gist.github.com/kriegsman/368b316c55221134b160. Modifed by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2Dsquaredswirl"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__SqauredSwirl()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  const uint8_t kBorderWidth = 2;

  SEGMENT.fadeToBlackBy(1 + SEGMENT.intensity / 5);
  SEGMENT.blur(SEGMENT.custom3>>1);

  // Use two out-of-sync sine waves
  int i = beatsin8_t(19, kBorderWidth, cols-kBorderWidth);
  int j = beatsin8_t(22, kBorderWidth, cols-kBorderWidth);
  int k = beatsin8_t(17, kBorderWidth, cols-kBorderWidth);
  int m = beatsin8_t(18, kBorderWidth, rows-kBorderWidth);
  int n = beatsin8_t(15, kBorderWidth, rows-kBorderWidth);
  int p = beatsin8_t(20, kBorderWidth, rows-kBorderWidth);

  SEGMENT.addPixelColorXY(i, m, ColorFromPalette(SEGPALETTE, effect_start_time/29, 255, LINEARBLEND));
  SEGMENT.addPixelColorXY(j, n, ColorFromPalette(SEGPALETTE, effect_start_time/41, 255, LINEARBLEND));
  SEGMENT.addPixelColorXY(k, p, ColorFromPalette(SEGPALETTE, effect_start_time/73, 255, LINEARBLEND));

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__SQUARED_SWIRL__INDEXING[] PROGMEM = "Squared Swirl@,Fade,,,Blur;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: ldirko https://editor.soulmatelights.com/gallery/599-sun-radiation  , modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DSunradiation"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__SunRadiation()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (!SEGMENT.allocateData(sizeof(byte)*(cols+2)*(rows+2))) return EFFECT_DEFAULT(); //allocation failed
  byte *bump = reinterpret_cast<byte*>(SEGMENT.data);

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  unsigned long t = effect_start_time / 4;
  unsigned index = 0;
  uint8_t someVal = SEGMENT.speed/4;             // Was 25.
  for (int j = 0; j < (rows + 2); j++) {
    for (int i = 0; i < (cols + 2); i++) {
      byte col = (inoise8_raw(i * someVal, j * someVal, t)) / 2;
      bump[index++] = col;
    }
  }

  int yindex = cols + 3;
  int vly = -(rows / 2 + 1);
  for (int y = 0; y < rows; y++) {
    ++vly;
    int vlx = -(cols / 2 + 1);
    for (int x = 0; x < cols; x++) {
      ++vlx;
      int nx = bump[x + yindex + 1] - bump[x + yindex - 1];
      int ny = bump[x + yindex + (cols + 2)] - bump[x + yindex - (cols + 2)];
      unsigned difx = abs8(vlx * 7 - nx);
      unsigned dify = abs8(vly * 7 - ny);
      int temp = difx * difx + dify * dify;
      int col = 255 - temp / 8; //8 its a size of effect
      if (col < 0) col = 0;
      SEGMENT.setPixelColorXY(x, y, HeatColor(col / (3.0f-(float)(SEGMENT.intensity)/128.f)));
    }
    yindex += (cols + 2);
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__SUN_RADIATION__INDEXING[] PROGMEM = "Sun Radiation@Variance,Brightness;;;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : By: Elliott Kember  https://editor.soulmatelights.com/gallery/3-tartan , Modified by: Andrew Tuline
 * @note : Converted from WLED Effects "mode_2Dtartan"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Tartan()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t hue, bri;
  size_t intensity;
  int offsetX = beatsin16_t(3, -360, 360);
  int offsetY = beatsin16_t(2, -360, 360);
  int sharpness = SEGMENT.custom3 / 8; // 0-3

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      hue = x * beatsin16_t(10, 1, 10) + offsetY;
      intensity = bri = sin8_t(x * SEGMENT.speed/2 + offsetX);
      for (int i=0; i<sharpness; i++) intensity *= bri;
      intensity >>= 8*sharpness;
      SEGMENT.setPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, hue, intensity, LINEARBLEND));
      hue = y * 3 + offsetX;
      intensity = bri = sin8_t(y * SEGMENT.intensity/2 + offsetY);
      for (int i=0; i<sharpness; i++) intensity *= bri;
      intensity >>= 8*sharpness;
      SEGMENT.addPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, hue, intensity, LINEARBLEND));
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__TARTAN__INDEXING[] PROGMEM = "Tartan@X scale,Y scale,,,Sharpness;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : Space ships by stepko (c)05.02.21 [https://editor.soulmatelights.com/gallery/639-space-ships], adapted by Blaz Kristan (AKA blazoncek)
 * @note : Converted from WLED Effects "mode_2Dspaceships"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__SpaceShips()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  uint32_t tb = effect_start_time >> 12;  // every ~4s
  if (tb > SEGMENT.step) {
    int dir = ++SEGMENT.aux0;
    dir  += (int)hw_random8(3)-1;
    if      (dir > 7) SEGMENT.aux0 = 0;
    else if (dir < 0) SEGMENT.aux0 = 7;
    else              SEGMENT.aux0 = dir;
    SEGMENT.step = tb + hw_random8(4);
  }

  SEGMENT.fadeToBlackBy(map(SEGMENT.speed, 0, 255, 248, 16));
  SEGMENT.move(SEGMENT.aux0, 1);

  for (size_t i = 0; i < 8; i++) {
    int x = beatsin8_t(12 + i, 2, cols - 3);
    int y = beatsin8_t(15 + i, 2, rows - 3);
    uint32_t color = ColorFromPalette(SEGPALETTE, beatsin8_t(12 + i, 0, 255), 255);
    SEGMENT.addPixelColorXY(x, y, color);
    if (cols > 24 || rows > 24) {
      SEGMENT.addPixelColorXY(x+1, y, color);
      SEGMENT.addPixelColorXY(x-1, y, color);
      SEGMENT.addPixelColorXY(x, y+1, color);
      SEGMENT.addPixelColorXY(x, y-1, color);
    }
  }
  SEGMENT.blur(SEGMENT.intensity >> 3, SEGMENT.check1);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__SPACESHIPS__INDEXING[] PROGMEM = "Spaceships@!,Blur,,,,Smear;;!;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : Crazy bees by stepko (c)12.02.21 [https://editor.soulmatelights.com/gallery/651-crazy-bees], adapted by Blaz Kristan (AKA blazoncek)
 * @note : Converted from WLED Effects "mode_2Dcrazybees"
 ********************************************************************************************************************************************************************************************************************/
#define MAX_BEES 5
uint16_t mAnimatorLight::EffectAnim__2D__CrazyBees()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  byte n = MIN(MAX_BEES, (rows * cols) / 256 + 1);

  typedef struct Bee {
    uint8_t posX, posY, aimX, aimY, hue;
    int8_t deltaX, deltaY, signX, signY, error;
    void aimed(uint16_t w, uint16_t h) {
      //random16_set_seed(millis());
      aimX   = random8(0, w);
      aimY   = random8(0, h);
      hue    = random8();
      deltaX = abs(aimX - posX);
      deltaY = abs(aimY - posY);
      signX  = posX < aimX ? 1 : -1;
      signY  = posY < aimY ? 1 : -1;
      error  = deltaX - deltaY;
    };
  } bee_t;

  if (!SEGMENT.allocateData(sizeof(bee_t)*MAX_BEES)) return EFFECT_DEFAULT(); //allocation failed
  bee_t *bee = reinterpret_cast<bee_t*>(SEGMENT.data);

  if (SEGMENT.call == 0) {
    random16_set_seed(effect_start_time);
    for (size_t i = 0; i < n; i++) {
      bee[i].posX = random8(0, cols);
      bee[i].posY = random8(0, rows);
      bee[i].aimed(cols, rows);
    }
  }

  if (effect_start_time > SEGMENT.step) {
    SEGMENT.step = effect_start_time + (FRAMETIME * 16 / ((SEGMENT.speed>>4)+1));
    SEGMENT.fadeToBlackBy(32 + ((SEGMENT.check1*SEGMENT.intensity) / 25));
    SEGMENT.blur(SEGMENT.intensity / (2 + SEGMENT.check1 * 9), SEGMENT.check1);
    for (size_t i = 0; i < n; i++) {
      uint32_t flowerCcolor = SEGMENT.color_from_palette(bee[i].hue, false, true, 255);
      SEGMENT.addPixelColorXY(bee[i].aimX + 1, bee[i].aimY, flowerCcolor);
      SEGMENT.addPixelColorXY(bee[i].aimX, bee[i].aimY + 1, flowerCcolor);
      SEGMENT.addPixelColorXY(bee[i].aimX - 1, bee[i].aimY, flowerCcolor);
      SEGMENT.addPixelColorXY(bee[i].aimX, bee[i].aimY - 1, flowerCcolor);
      if (bee[i].posX != bee[i].aimX || bee[i].posY != bee[i].aimY) {
        SEGMENT.setPixelColorXY(bee[i].posX, bee[i].posY, CRGB(CHSV(bee[i].hue, 60, 255)));
        int error2 = bee[i].error * 2;
        if (error2 > -bee[i].deltaY) {
          bee[i].error -= bee[i].deltaY;
          bee[i].posX += bee[i].signX;
        }
        if (error2 < bee[i].deltaX) {
          bee[i].error += bee[i].deltaX;
          bee[i].posY += bee[i].signY;
        }
      } else {
        bee[i].aimed(cols, rows);
      }
    }
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__CRAZYBEES[] PROGMEM = "Crazy Bees@!,Blur,,,,Smear;;!;2;pal=11,ix=0";
#undef MAX_BEES


/*******************************************************************************************************************************************************************************************************************
 * @description : Ghost Rider by stepko (c)2021 [https://editor.soulmatelights.com/gallery/716-ghost-rider], adapted by Blaz Kristan (AKA blazoncek)
 * @note : Converted from WLED Effects "mode_2Dghostrider"
 ********************************************************************************************************************************************************************************************************************/
#define LIGHTERS_AM 64  // max lighters (adequate for 32x32 matrix)
uint16_t mAnimatorLight::EffectAnim__2D__GhostRider()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  typedef struct Lighter {
    int16_t  gPosX;
    int16_t  gPosY;
    uint16_t gAngle;
    int8_t   angleSpeed;
    uint16_t lightersPosX[LIGHTERS_AM];
    uint16_t lightersPosY[LIGHTERS_AM];
    uint16_t Angle[LIGHTERS_AM];
    uint16_t time[LIGHTERS_AM];
    bool     reg[LIGHTERS_AM];
    int8_t   Vspeed;
  } lighter_t;

  if (!SEGMENT.allocateData(sizeof(lighter_t))) return EFFECT_DEFAULT(); //allocation failed
  lighter_t *lighter = reinterpret_cast<lighter_t*>(SEGMENT.data);

  const size_t maxLighters = min(cols + rows, LIGHTERS_AM);

  if (SEGMENT.aux0 != cols || SEGMENT.aux1 != rows) {
    SEGMENT.aux0 = cols;
    SEGMENT.aux1 = rows;
    lighter->angleSpeed = hw_random8(0,20) - 10;
    lighter->gAngle = hw_random16();
    lighter->Vspeed = 5;
    lighter->gPosX = (cols/2) * 10;
    lighter->gPosY = (rows/2) * 10;
    for (size_t i = 0; i < maxLighters; i++) {
      lighter->lightersPosX[i] = lighter->gPosX;
      lighter->lightersPosY[i] = lighter->gPosY + i;
      lighter->time[i] = i * 2;
      lighter->reg[i] = false;
    }
  }

  if (effect_start_time > SEGMENT.step) {
    SEGMENT.step = effect_start_time + 1024 / (cols+rows);

    SEGMENT.fadeToBlackBy((SEGMENT.speed>>2)+64);

    CRGB color = CRGB::White;
    SEGMENT.wu_pixel(lighter->gPosX * 256 / 10, lighter->gPosY * 256 / 10, color);

    lighter->gPosX += lighter->Vspeed * sin_t(radians(lighter->gAngle));
    lighter->gPosY += lighter->Vspeed * cos_t(radians(lighter->gAngle));
    lighter->gAngle += lighter->angleSpeed;
    if (lighter->gPosX < 0)               lighter->gPosX = (cols - 1) * 10;
    if (lighter->gPosX > (cols - 1) * 10) lighter->gPosX = 0;
    if (lighter->gPosY < 0)               lighter->gPosY = (rows - 1) * 10;
    if (lighter->gPosY > (rows - 1) * 10) lighter->gPosY = 0;
    for (size_t i = 0; i < maxLighters; i++) {
      lighter->time[i] += hw_random8(5, 20);
      if (lighter->time[i] >= 255 ||
        (lighter->lightersPosX[i] <= 0) ||
          (lighter->lightersPosX[i] >= (cols - 1) * 10) ||
          (lighter->lightersPosY[i] <= 0) ||
          (lighter->lightersPosY[i] >= (rows - 1) * 10)) {
        lighter->reg[i] = true;
      }
      if (lighter->reg[i]) {
        lighter->lightersPosY[i] = lighter->gPosY;
        lighter->lightersPosX[i] = lighter->gPosX;
        lighter->Angle[i] = lighter->gAngle + ((int)hw_random8(20) - 10);
        lighter->time[i] = 0;
        lighter->reg[i] = false;
      } else {
        lighter->lightersPosX[i] += -7 * sin_t(radians(lighter->Angle[i]));
        lighter->lightersPosY[i] += -7 * cos_t(radians(lighter->Angle[i]));
      }
      SEGMENT.wu_pixel(lighter->lightersPosX[i] * 256 / 10, lighter->lightersPosY[i] * 256 / 10, ColorFromPalette(SEGPALETTE, (256 - lighter->time[i])));
    }
    SEGMENT.blur(SEGMENT.intensity>>3);
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__GHOST_RIDER__INDEXING[] PROGMEM = "Ghost Rider@Fade rate,Blur;;!;2";
#undef LIGHTERS_AM


/*******************************************************************************************************************************************************************************************************************
 * @description : Floating Blobs by stepko (c)2021 [https://editor.soulmatelights.com/gallery/573-blobs], adapted by Blaz Kristan (AKA blazoncek)
 * @note : Converted from WLED Effects "mode_2Dfloatingblobs"
 ********************************************************************************************************************************************************************************************************************/
#define MAX_BLOBS 8
uint16_t mAnimatorLight::EffectAnim__2D__FloatingBlobs()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  typedef struct Blob {
    float x[MAX_BLOBS], y[MAX_BLOBS];
    float sX[MAX_BLOBS], sY[MAX_BLOBS]; // speed
    float r[MAX_BLOBS];
    bool grow[MAX_BLOBS];
    byte color[MAX_BLOBS];
  } blob_t;

  size_t Amount = (SEGMENT.intensity>>5) + 1; // NOTE: be sure to update MAX_BLOBS if you change this

  if (!SEGMENT.allocateData(sizeof(blob_t))) return EFFECT_DEFAULT(); //allocation failed
  blob_t *blob = reinterpret_cast<blob_t*>(SEGMENT.data);

  if (SEGMENT.aux0 != cols || SEGMENT.aux1 != rows) {
    SEGMENT.aux0 = cols; // re-initialise if virtual size changes
    SEGMENT.aux1 = rows;
    //SEGMENT.fill(BLACK);
    for (size_t i = 0; i < MAX_BLOBS; i++) {
      blob->r[i]  = hw_random8(1, cols>8 ? (cols/4) : 2);
      blob->sX[i] = (float) hw_random8(3, cols) / (float)(256 - SEGMENT.speed); // speed x
      blob->sY[i] = (float) hw_random8(3, rows) / (float)(256 - SEGMENT.speed); // speed y
      blob->x[i]  = hw_random8(0, cols-1);
      blob->y[i]  = hw_random8(0, rows-1);
      blob->color[i] = hw_random8();
      blob->grow[i]  = (blob->r[i] < 1.f);
      if (blob->sX[i] == 0) blob->sX[i] = 1;
      if (blob->sY[i] == 0) blob->sY[i] = 1;
    }
  }

  SEGMENT.fadeToBlackBy((SEGMENT.custom2>>3)+1);

  // Bounce balls around
  for (size_t i = 0; i < Amount; i++) {
    if (SEGMENT.step < effect_start_time) blob->color[i] = add8(blob->color[i], 4); // slowly change color
    // change radius if needed
    if (blob->grow[i]) {
      // enlarge radius until it is >= 4
      blob->r[i] += (fabsf(blob->sX[i]) > fabsf(blob->sY[i]) ? fabsf(blob->sX[i]) : fabsf(blob->sY[i])) * 0.05f;
      if (blob->r[i] >= MIN(cols/4.f,2.f)) {
        blob->grow[i] = false;
      }
    } else {
      // reduce radius until it is < 1
      blob->r[i] -= (fabsf(blob->sX[i]) > fabsf(blob->sY[i]) ? fabsf(blob->sX[i]) : fabsf(blob->sY[i])) * 0.05f;
      if (blob->r[i] < 1.f) {
        blob->grow[i] = true;
      }
    }
    uint32_t c = SEGMENT.color_from_palette(blob->color[i], false, false, 0);
    if (blob->r[i] > 1.f) SEGMENT.fillCircle(roundf(blob->x[i]), roundf(blob->y[i]), roundf(blob->r[i]), c);
    else                  SEGMENT.setPixelColorXY((int)roundf(blob->x[i]), (int)roundf(blob->y[i]), c);
    // move x
    if (blob->x[i] + blob->r[i] >= cols - 1) blob->x[i] += (blob->sX[i] * ((cols - 1 - blob->x[i]) / blob->r[i] + 0.005f));
    else if (blob->x[i] - blob->r[i] <= 0)   blob->x[i] += (blob->sX[i] * (blob->x[i] / blob->r[i] + 0.005f));
    else                                     blob->x[i] += blob->sX[i];
    // move y
    if (blob->y[i] + blob->r[i] >= rows - 1) blob->y[i] += (blob->sY[i] * ((rows - 1 - blob->y[i]) / blob->r[i] + 0.005f));
    else if (blob->y[i] - blob->r[i] <= 0)   blob->y[i] += (blob->sY[i] * (blob->y[i] / blob->r[i] + 0.005f));
    else                                     blob->y[i] += blob->sY[i];
    // bounce x
    if (blob->x[i] < 0.01f) {
      blob->sX[i] = (float)hw_random8(3, cols) / (256 - SEGMENT.speed);
      blob->x[i]  = 0.01f;
    } else if (blob->x[i] > (float)cols - 1.01f) {
      blob->sX[i] = (float)hw_random8(3, cols) / (256 - SEGMENT.speed);
      blob->sX[i] = -blob->sX[i];
      blob->x[i]  = (float)cols - 1.01f;
    }
    // bounce y
    if (blob->y[i] < 0.01f) {
      blob->sY[i] = (float)hw_random8(3, rows) / (256 - SEGMENT.speed);
      blob->y[i]  = 0.01f;
    } else if (blob->y[i] > (float)rows - 1.01f) {
      blob->sY[i] = (float)hw_random8(3, rows) / (256 - SEGMENT.speed);
      blob->sY[i] = -blob->sY[i];
      blob->y[i]  = (float)rows - 1.01f;
    }
  }
  SEGMENT.blur(SEGMENT.custom1>>2);

  if (SEGMENT.step < effect_start_time) SEGMENT.step = effect_start_time + 2000; // change colors every 2 seconds

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__FLOATING_BLOBS__INDEXING[] PROGMEM = "Blobs@!,# blobs,Blur,Trail;!;!;2;c1=8";
#undef MAX_BLOBS
  

/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_2Dscrollingtext"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__ScrollingText()
{

  #define USE_NEW_SCROLL_BEFORE_PHASE_OUT

  #ifdef USE_NEW_SCROLL_BEFORE_PHASE_OUT

  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  unsigned letterWidth, rotLW;
  unsigned letterHeight, rotLH;
  switch (map(SEGMENT.custom2, 0, 255, 1, 5)) {
    default:
    case 1: letterWidth = 4; letterHeight =  6; break;
    case 2: letterWidth = 5; letterHeight =  8; break;
    case 3: letterWidth = 6; letterHeight =  8; break;
    case 4: letterWidth = 7; letterHeight =  9; break;
    case 5: letterWidth = 5; letterHeight = 12; break;
  }
  // letters are rotated
  if (((SEGMENT.custom3+1)>>3) % 2) {
    rotLH = letterWidth;
    rotLW = letterHeight;
  } else {
    rotLW = letterWidth;
    rotLH = letterHeight;
  }

  char text[WLED_MAX_SEGNAME_LEN+1] = {'\0'};
  if (SEGMENT.name) for (size_t i=0,j=0; i<strlen(SEGMENT.name); i++) if (SEGMENT.name[i]>31 && SEGMENT.name[i]<128) text[j++] = SEGMENT.name[i];
  const bool zero = strchr(text, '0') != nullptr;

  char sec[5];
  int  AmPmHour = tkr_time->hour(localTime);
  bool isitAM = true;
  if (useAMPM) {
    if (AmPmHour > 11) { AmPmHour -= 12; isitAM = false; }
    if (AmPmHour == 0) { AmPmHour  = 12; }
    sprintf_P(sec, PSTR(" %2s"), (isitAM ? "AM" : "PM"));
  } else {
    sprintf_P(sec, PSTR(":%02d"), tkr_time->second(localTime));
  }

  if (!strlen(text)) { // fallback if empty segment name: display date and time
    sprintf_P(text, PSTR("%s %d, %d %d:%02d%s"), tkr_time->monthShortStr(tkr_time->month(localTime)), tkr_time->day(localTime), tkr_time->year(localTime), AmPmHour, tkr_time->minute(localTime), sec);
  } else {
    if      (!strncmp_P(text,PSTR("#DATE"),5)) sprintf_P(text, zero?PSTR("%02d.%02d.%04d"):PSTR("%d.%d.%d"),   tkr_time->day(localTime),   tkr_time->month(localTime),  tkr_time->year(localTime));
    else if (!strncmp_P(text,PSTR("#DDMM"),5)) sprintf_P(text, zero?PSTR("%02d.%02d")     :PSTR("%d.%d"),      tkr_time->day(localTime),   tkr_time->month(localTime));
    else if (!strncmp_P(text,PSTR("#MMDD"),5)) sprintf_P(text, zero?PSTR("%02d/%02d")     :PSTR("%d/%d"),      tkr_time->month(localTime), tkr_time->day(localTime));
    else if (!strncmp_P(text,PSTR("#TIME"),5)) sprintf_P(text, zero?PSTR("%02d:%02d%s")   :PSTR("%2d:%02d%s"), AmPmHour,         tkr_time->minute(localTime), sec);
    else if (!strncmp_P(text,PSTR("#HHMM"),5)) sprintf_P(text, zero?PSTR("%02d:%02d")     :PSTR("%d:%02d"),    AmPmHour,         tkr_time->minute(localTime));
    else if (!strncmp_P(text,PSTR("#HH"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         AmPmHour);
    else if (!strncmp_P(text,PSTR("#MM"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         tkr_time->minute(localTime));
  }

  const int  numberOfLetters = strlen(text);
  int width = (numberOfLetters * rotLW);
  int yoffset = map(SEGMENT.intensity, 0, 255, -rows/2, rows/2) + (rows-rotLH)/2;
  if (width <= cols) {
    // scroll vertically (e.g. ^^ Way out ^^) if it fits
    int speed = map(SEGMENT.speed, 0, 255, 5000, 1000);
    int frac = effect_start_time % speed + 1;
    if (SEGMENT.intensity == 255) {
      yoffset = (2 * frac * rows)/speed - rows;
    } else if (SEGMENT.intensity == 0) {
      yoffset = rows - (2 * frac * rows)/speed;
    }
  }

  if (SEGMENT.step < effect_start_time) {
    // calculate start offset
    if (width > cols) {
      if (SEGMENT.check3) {
        if (SEGMENT.aux0 == 0) SEGMENT.aux0  = width + cols - 1;
        else                --SEGMENT.aux0;
      } else                ++SEGMENT.aux0 %= width + cols;
    } else                    SEGMENT.aux0  = (cols + width)/2;
    ++SEGMENT.aux1 &= 0xFF; // color shift
    SEGMENT.step = effect_start_time + map(SEGMENT.speed, 0, 255, 250, 50); // shift letters every ~250ms to ~50ms
  }

  if (!SEGMENT.check2) SEGMENT.fade_out(255 - (SEGMENT.custom1>>4));  // trail
  bool usePaletteGradient = false;
  uint32_t col1 = SEGMENT.color_from_palette(SEGMENT.aux1, false, PALETTE_SOLID_WRAP, 0);
  uint32_t col2 = BLACK;
  if (SEGMENT.check1) { // use gradient
    if(SEGMENT.palette_id == 0) { // use colors for gradient
    col1 = SEGCOLOR(0);
    col2 = SEGCOLOR(2);
    }
    else usePaletteGradient = true;
  }

  for (int i = 0; i < numberOfLetters; i++) {
    int xoffset = int(cols) - int(SEGMENT.aux0) + rotLW*i;
    if (xoffset + rotLW < 0) continue; // don't draw characters off-screen
    SEGMENT.drawCharacter(text[i], xoffset, yoffset, letterWidth, letterHeight, col1, col2, map(SEGMENT.custom3, 0, 31, -2, 2), usePaletteGradient);
  }

  return FRAMETIME;

#else
  /////////////////////// OLD BELOW
// uint16_t mAnimatorLight::EffectAnim__Matrix__2D_Scrolling_Text(void) 
// {

  // DEBUG_LINE_HERE;

  ALOG_INF(PSTR("EffectAnim__Matrix__2D_Scrolling_Text"));


  // setPixelColorXY_CRGB(0,0,RGBW32(0,0,255,0));
  // setPixelColorXY_CRGB(0,1,RGBW32(0,0,254,0));
  // setPixelColorXY_CRGB(0,2,RGBW32(0,0,253,0));

  // setPixelColorXY_CRGB(1,0,RGBW32(0,255,0,0));
  // setPixelColorXY_CRGB(1,1,RGBW32(0,255,0,0));
  // setPixelColorXY_CRGB(1,2,RGBW32(0,255,0,0));

  // for (int i=2;i<16;i++)
  // {
  //   setPixelColorXY_CRGB(i,i,RGBW32(  map(i, 2,16, 0,255), map(i, 2,16, 255,0),0,0));
  // }

  // ALOG_INF(PSTR("EffectAnim__Matrix__2D_Scrolling_Text --------------------------------- EFFECT END"));

  // SEGMENT.cycle_time__rate_ms = 1000;
  // SET_DIRECT_MODE();

  // return FRAMETIME;

  // DEBUG_LINE_HERE;
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  // DEBUG_LINE_HERE;

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  int letterWidth, rotLW;
  int letterHeight, rotLH;
  switch (map(SEGMENT.custom2, 0, 255, 1, 5)) {
    default:
    case 1: letterWidth = 4; letterHeight =  6; break;
    case 2: letterWidth = 5; letterHeight =  8; break;
    case 3: letterWidth = 6; letterHeight =  8; break;
    case 4: letterWidth = 7; letterHeight =  9; break;
    case 5: letterWidth = 5; letterHeight = 12; break;
  }
  // DEBUG_LINE_HERE;
  // letters are rotated
  if (((SEGMENT.custom3+1)>>3) % 2) {
    rotLH = letterWidth;
    rotLW = letterHeight;
  } else {
    rotLW = letterWidth;
    rotLH = letterHeight;
  }
  // DEBUG_LINE_HERE;

  char text[WLED_MAX_SEGNAME_LEN+1] = {'\0'};
  if (SEGMENT.name) for (size_t i=0,j=0; i<strlen(SEGMENT.name); i++) if (SEGMENT.name[i]>31 && SEGMENT.name[i]<128) text[j++] = SEGMENT.name[i];
  const bool zero = strchr(text, '0') != nullptr;

  // DEBUG_LINE_HERE;
  char sec[5];
  int  AmPmHour = tkr_time->RtcTime.hour;// hour(localTime);
  bool isitAM = true;
  if (useAMPM) {
    if (AmPmHour > 11) { AmPmHour -= 12; isitAM = false; }
    if (AmPmHour == 0) { AmPmHour  = 12; }
    sprintf_P(sec, PSTR(" %2s"), (isitAM ? "AM" : "PM"));
  } else {
    sprintf_P(sec, PSTR(":%02d"), tkr_time->RtcTime.second);//(localTime));
  }

  // DEBUG_LINE_HERE;
  if (!strlen(text)) { // fallback if empty segment name: display date and time
    
    // tkr_time->GetDateAndTime(DT_DST).c_str();

    // sprintf_P(text, PSTR("%s %d, %d %d:%02d%s"), monthShortStr(month(localTime)), tkr_time->RtcTime.days, tkr_time->RtcTime.year, AmPmHour, tkr_time->RtcTime.minute, sec);
    sprintf_P(text, PSTR("Test Message"));
  } else {
    if      (!strncmp_P(text,PSTR("#DATE"),5)) sprintf_P(text, zero?PSTR("%02d.%02d.%04d"):PSTR("%d.%d.%d"),   tkr_time->RtcTime.days,   tkr_time->RtcTime.month,  tkr_time->RtcTime.year);
    else if (!strncmp_P(text,PSTR("#DDMM"),5)) sprintf_P(text, zero?PSTR("%02d.%02d")     :PSTR("%d.%d"),      tkr_time->RtcTime.days,   tkr_time->RtcTime.month);
    else if (!strncmp_P(text,PSTR("#MMDD"),5)) sprintf_P(text, zero?PSTR("%02d/%02d")     :PSTR("%d/%d"),      tkr_time->RtcTime.month, tkr_time->RtcTime.days);
    else if (!strncmp_P(text,PSTR("#TIME"),5)) sprintf_P(text, zero?PSTR("%02d:%02d%s")   :PSTR("%2d:%02d%s"), AmPmHour,         tkr_time->RtcTime.minute, sec);
    else if (!strncmp_P(text,PSTR("#HHMM"),5)) sprintf_P(text, zero?PSTR("%02d:%02d")     :PSTR("%d:%02d"),    AmPmHour,         tkr_time->RtcTime.minute);
    else if (!strncmp_P(text,PSTR("#HH"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         AmPmHour);
    else if (!strncmp_P(text,PSTR("#MM"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         tkr_time->RtcTime.minute);
  }

  // DEBUG_LINE_HERE;
  const int  numberOfLetters = strlen(text);
  const unsigned long now = millis(); // reduce millis() calls
  int width = (numberOfLetters * rotLW);
  int yoffset = map(SEGMENT.intensity, 0, 255, -rows/2, rows/2) + (rows-rotLH)/2;
  if (width <= cols) {
    // scroll vertically (e.g. ^^ Way out ^^) if it fits
    int speed = map(SEGMENT.speed, 0, 255, 5000, 1000);
    int frac = now % speed + 1;
    if (SEGMENT.intensity == 255) {
      yoffset = (2 * frac * rows)/speed - rows;
    } else if (SEGMENT.intensity == 0) {
      yoffset = rows - (2 * frac * rows)/speed;
    }
  }

  // DEBUG_LINE_HERE;
  if (SEGMENT.step < now) {
    // calculate start offset
    if (width > cols) {
      if (SEGMENT.check3) {
        if (SEGMENT.aux0 == 0) SEGMENT.aux0  = width + cols - 1;
        else                --SEGMENT.aux0;
      } else                ++SEGMENT.aux0 %= width + cols;
    } else                    SEGMENT.aux0  = (cols + width)/2;
    ++SEGMENT.aux1 &= 0xFF; // color shift
    SEGMENT.step = now + map(SEGMENT.speed, 0, 255, 250, 50); // shift letters every ~250ms to ~50ms
  }

  // DEBUG_LINE_HERE;
  if (!SEGMENT.check2) SEGMENT.fade_out(255 - (SEGMENT.custom1>>4));  // trail

  // DEBUG_LINE_HERE;
  for (int i = 0; i < numberOfLetters; i++) {
    int xoffset = int(cols) - int(SEGMENT.aux0) + rotLW*i;
    if (xoffset + rotLW < 0) continue; // don't draw characters off-screen
    uint32_t col1 = SEGMENT.GetPaletteColour(SEGMENT.aux1, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF); //SEGMENT.color_from_palette(SEGMENT.aux1, false, PALETTE_SOLID_WRAP, 0);
    uint32_t col2 = BLACK;
    if (SEGMENT.check1 && SEGMENT.palette_id == 0) {
      col1 = SEGCOLOR_U32(0); //SEGCOLOR_U32(0);
      col2 = SEGCOLOR_U32(2); //SEGCOLOR_U32(2);
    }
    SEGMENT.drawCharacter(text[i], xoffset, yoffset, letterWidth, letterHeight, col1, col2, map(SEGMENT.custom3, 0, 31, -2, 2));
  }
  // // DEBUG_LINE_HERE;

  return FRAMETIME;

  #endif

}
static const char PM_EFFECT_CONFIG__2D__SCROLLING_TEXT__INDEXING[] PROGMEM = "Scrolling Text@!,Y Offset,Trail,Font size,Rotate,Gradient,Overlay,Reverse;!,!,Gradient;!;2;ix=128,c1=0,rev=0,mi=0,rY=0,mY=0";



/*******************************************************************************************************************************************************************************************************************
 * @description : Drift Rose by stepko (c)2021 [https://editor.soulmatelights.com/gallery/1369-drift-rose-pattern], adapted by Blaz Kristan (AKA blazoncek) improved by @dedehai 
 * @note : Converted from WLED Effects "mode_2Ddriftrose"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__DriftRose()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  const float CX = (cols-cols%2)/2.f - .5f;
  const float CY = (rows-rows%2)/2.f - .5f;
  const float L = min(cols, rows) / 2.f;

  SEGMENT.fadeToBlackBy(32+(SEGMENT.speed>>3));
  for (size_t i = 1; i < 37; i++) {
    float angle = radians(i * 10);
    uint32_t x = (CX + (sin_t(angle) * (beatsin8_t(i, 0, L*2)-L))) * 255.f;
    uint32_t y = (CY + (cos_t(angle) * (beatsin8_t(i, 0, L*2)-L))) * 255.f;
    if(SEGMENT.palette_id == 0) SEGMENT.wu_pixel(x, y, CHSV(i * 10, 255, 255));
    else SEGMENT.wu_pixel(x, y, ColorFromPalette(SEGPALETTE, i * 10));
  }
  SEGMENT.blur(SEGMENT.intensity >> 4, SEGMENT.check1);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__DRIFT_ROSE__INDEXING[] PROGMEM = "Drift Rose@Fade,Blur,,,,Smear;;!;2;pal=11";



/*******************************************************************************************************************************************************************************************************************
 * @description : Created from ScrollingText to produce a fixed matrix clock
 * @note : Converted from WLED Effects
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__DigitalClock()
{

  ALOG_INF(PSTR("EffectAnim__2D__DigitalClock"));

  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  int letterWidth, rotLW;
  int letterHeight, rotLH;
  switch (map(SEGMENT.custom2, 0, 255, 1, 5)) {
    default:
    case 1: letterWidth = 4; letterHeight =  6; break;
    case 2: letterWidth = 5; letterHeight =  8; break;
    case 3: letterWidth = 6; letterHeight =  8; break;
    case 4: letterWidth = 7; letterHeight =  9; break;
    case 5: letterWidth = 5; letterHeight = 12; break;
  }
  // DEBUG_LINE_HERE;
  // letters are rotated
  if (((SEGMENT.custom3+1)>>3) % 2) {
    rotLH = letterWidth;
    rotLW = letterHeight;
  } else {
    rotLW = letterWidth;
    rotLH = letterHeight;
  }
  // DEBUG_LINE_HERE;

  char text[WLED_MAX_SEGNAME_LEN+1] = {'\0'};
  if (SEGMENT.name) for (size_t i=0,j=0; i<strlen(SEGMENT.name); i++) if (SEGMENT.name[i]>31 && SEGMENT.name[i]<128) text[j++] = SEGMENT.name[i];
  const bool zero = strchr(text, '0') != nullptr;

  // DEBUG_LINE_HERE;
  char sec[5];
  int  AmPmHour = tkr_time->RtcTime.hour;// hour(localTime);
  bool isitAM = true;
  if (useAMPM) {
    if (AmPmHour > 11) { AmPmHour -= 12; isitAM = false; }
    if (AmPmHour == 0) { AmPmHour  = 12; }
    sprintf_P(sec, PSTR(" %2s"), (isitAM ? "AM" : "PM"));
  } else {
    sprintf_P(sec, PSTR(":%02d"), tkr_time->RtcTime.second);//(localTime));
  }
  
  sprintf_P(text, PSTR("#HHMM"));

  // DEBUG_LINE_HERE;
  if (!strlen(text)) { // fallback if empty segment name: display date and time
    
    // tkr_time->GetDateAndTime(DT_DST).c_str();

    // sprintf_P(text, PSTR("%s %d, %d %d:%02d%s"), monthShortStr(month(localTime)), tkr_time->RtcTime.days, tkr_time->RtcTime.year, AmPmHour, tkr_time->RtcTime.minute, sec);
    sprintf_P(text, PSTR("Test Message"));
  } else {
    if      (!strncmp_P(text,PSTR("#DATE"),5)) sprintf_P(text, zero?PSTR("%02d.%02d.%04d"):PSTR("%d.%d.%d"),   tkr_time->RtcTime.days,   tkr_time->RtcTime.month,  tkr_time->RtcTime.year);
    else if (!strncmp_P(text,PSTR("#DDMM"),5)) sprintf_P(text, zero?PSTR("%02d.%02d")     :PSTR("%d.%d"),      tkr_time->RtcTime.days,   tkr_time->RtcTime.month);
    else if (!strncmp_P(text,PSTR("#MMDD"),5)) sprintf_P(text, zero?PSTR("%02d/%02d")     :PSTR("%d/%d"),      tkr_time->RtcTime.month, tkr_time->RtcTime.days);
    else if (!strncmp_P(text,PSTR("#TIME"),5)) sprintf_P(text, zero?PSTR("%02d:%02d%s")   :PSTR("%2d:%02d%s"), AmPmHour,         tkr_time->RtcTime.minute, sec);
    else if (!strncmp_P(text,PSTR("#HHMM"),5)) sprintf_P(text, zero?PSTR("%02d:%02d")     :PSTR("%d:%02d"),    AmPmHour,         tkr_time->RtcTime.minute);
    else if (!strncmp_P(text,PSTR("#HH"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         AmPmHour);
    else if (!strncmp_P(text,PSTR("#MM"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         tkr_time->RtcTime.minute);
  }

  // DEBUG_LINE_HERE;
  const int  numberOfLetters = strlen(text);
  const unsigned long now = millis(); // reduce millis() calls
  int width = (numberOfLetters * rotLW);
  int yoffset = map(SEGMENT.intensity, 0, 255, -rows/2, rows/2) + (rows-rotLH)/2;
  if (width <= cols) {
    // scroll vertically (e.g. ^^ Way out ^^) if it fits
    int speed = map(SEGMENT.speed, 0, 255, 5000, 1000);
    int frac = now % speed + 1;
    if (SEGMENT.intensity == 255) {
      yoffset = (2 * frac * rows)/speed - rows;
    } else if (SEGMENT.intensity == 0) {
      yoffset = rows - (2 * frac * rows)/speed;
    }
  }

  SEGMENT.aux0  = (cols + width)/2; // Center the text or set to any fixed offset

  bool solidPerChar                 = !SEGMENT.check1;
  bool horizontalGradient           = !SEGMENT.check2;
  bool backgroundGradientHorizontal = !SEGMENT.check3;

  // if (!SEGMENT.check2) 
  SEGMENT.fade_out(255 - (SEGMENT.custom1>>4));  // trail

  for (int i = 0; i < numberOfLetters; i++) {
    int xoffset = int(cols) - int(SEGMENT.aux0) + rotLW * i;
    if (xoffset + rotLW < 0) continue; // don't draw characters off-screen
    // Use the new function with the desired flags (solidPerChar and horizontalGradient)
    SEGMENT.drawCharacter_UsingGradientPalletes(text[i], xoffset, yoffset, letterWidth, letterHeight, map(SEGMENT.custom3, 0, 31, -2, 2), solidPerChar, horizontalGradient, backgroundGradientHorizontal);
  }

  return FRAMETIME;

}
static const char PM_EFFECT_CONFIG__2D__DIGITAL_CLOCK__INDEXING[] PROGMEM = 
"Digital Text Clock@Bkg Brightness,Y Offset,Bkg Palette,Font size,Rotate,,,Gradient,Font Grad Hor,Bkg Hor;!,!,!,!,!;!;2;ix=128,c1=0,mi=0,rY=0,mY=0,o1=0,o2=1,o3=1";
//       Name      @ Sliders Speed,   Inten,       Cus1,     Cus2,  Cus3,,,    Opt1,         Opt2,   Opt3;5SegCol  ; ; ;default commands   
/** 
 * Effect Name (From Start to @)
 * Name@
 * Between @ And first ; is the slider values
 * Slider 0: Speed     = With scroll here, it should be background brightness
 * Slider 1: Intensity = Y Offset
 * Slider 2: Custom 1  = Background Palette ID
 * Slider 3: Custom 2  = Font Size
 * Slider 4: Custom 3  = Rotate Font
 * Slider 5: Time Period (ie Cycle Time, previously rate_ms)
 * Slider 6: Grouping (unused)
 * Checkbox 0: Option 1 = Use Gradient for Font (true) or each character is solid (false)
 * Checkbox 1: Option 2 = Font Gradient Horizontal
 * Checkbox 2: Option 3 = Background Gradient Horizontal **/


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_2Dplasmarotozoom"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__PlasmaRotoZoom()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  uint8_t speed = SEGMENT.speed/32;
  uint8_t scale = SEGMENT.intensity/32;

  uint8_t  w = 2;

  unsigned a  = effect_start_time/32;
  unsigned a2 = a/2;
  unsigned a3 = a/3;

  unsigned cx =  beatsin8_t(10-speed,0,cols-1)*scale;
  unsigned cy =  beatsin8_t(12-speed,0,rows-1)*scale;
  unsigned cx1 = beatsin8_t(13-speed,0,cols-1)*scale;
  unsigned cy1 = beatsin8_t(15-speed,0,rows-1)*scale;
  unsigned cx2 = beatsin8_t(17-speed,0,cols-1)*scale;
  unsigned cy2 = beatsin8_t(14-speed,0,rows-1)*scale;
  
  unsigned xoffs = 0;
  for (int x = 0; x < cols; x++) {
    xoffs += scale;
    unsigned yoffs = 0;

    for (int y = 0; y < rows; y++) {
       yoffs += scale;

      byte rdistort = cos8_t((cos8_t(((x<<3)+a )&255)+cos8_t(((y<<3)-a2)&255)+a3   )&255)>>1; 
      byte gdistort = cos8_t((cos8_t(((x<<3)-a2)&255)+cos8_t(((y<<3)+a3)&255)+a+32 )&255)>>1; 
      byte bdistort = cos8_t((cos8_t(((x<<3)+a3)&255)+cos8_t(((y<<3)-a) &255)+a2+64)&255)>>1; 

      byte valueR = rdistort+ w*  (a- ( ((xoffs - cx)  * (xoffs - cx)  + (yoffs - cy)  * (yoffs - cy))>>7  ));
      byte valueG = gdistort+ w*  (a2-( ((xoffs - cx1) * (xoffs - cx1) + (yoffs - cy1) * (yoffs - cy1))>>7 ));
      byte valueB = bdistort+ w*  (a3-( ((xoffs - cx2) * (xoffs - cx2) + (yoffs - cy2) * (yoffs - cy2))>>7 ));

      valueR = gamma8(cos8_t(valueR));
      valueG = gamma8(cos8_t(valueG));
      valueB = gamma8(cos8_t(valueB));

      SEGMENT.setPixelColorXY(x, y, RGBW32(valueR, valueG, valueB, 0)); 
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__PLASMAROTOZOOM__INDEXING[] PROGMEM = "Rotozoomer@!,Scale,,,,Alt;;!;2;pal=54";


/*******************************************************************************************************************************************************************************************************************
 * @description : Distortion waves - ldirko https://editor.soulmatelights.com/gallery/1089-distorsion-waves adapted for WLED by @blazoncek
 * @note : Converted from WLED Effects "mode_2Ddistortionwaves"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__DistortionWaves()
{
  
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  uint8_t speed = SEGMENT.speed/32;
  uint8_t scale = SEGMENT.intensity/32;

  uint8_t  w = 2;

  unsigned a  = effect_start_time/32;
  unsigned a2 = a/2;
  unsigned a3 = a/3;

  unsigned cx =  beatsin8_t(10-speed,0,cols-1)*scale;
  unsigned cy =  beatsin8_t(12-speed,0,rows-1)*scale;
  unsigned cx1 = beatsin8_t(13-speed,0,cols-1)*scale;
  unsigned cy1 = beatsin8_t(15-speed,0,rows-1)*scale;
  unsigned cx2 = beatsin8_t(17-speed,0,cols-1)*scale;
  unsigned cy2 = beatsin8_t(14-speed,0,rows-1)*scale;
  
  unsigned xoffs = 0;
  for (int x = 0; x < cols; x++) {
    xoffs += scale;
    unsigned yoffs = 0;

    for (int y = 0; y < rows; y++) {
       yoffs += scale;

      byte rdistort = cos8_t((cos8_t(((x<<3)+a )&255)+cos8_t(((y<<3)-a2)&255)+a3   )&255)>>1; 
      byte gdistort = cos8_t((cos8_t(((x<<3)-a2)&255)+cos8_t(((y<<3)+a3)&255)+a+32 )&255)>>1; 
      byte bdistort = cos8_t((cos8_t(((x<<3)+a3)&255)+cos8_t(((y<<3)-a) &255)+a2+64)&255)>>1; 

      byte valueR = rdistort+ w*  (a- ( ((xoffs - cx)  * (xoffs - cx)  + (yoffs - cy)  * (yoffs - cy))>>7  ));
      byte valueG = gdistort+ w*  (a2-( ((xoffs - cx1) * (xoffs - cx1) + (yoffs - cy1) * (yoffs - cy1))>>7 ));
      byte valueB = bdistort+ w*  (a3-( ((xoffs - cx2) * (xoffs - cx2) + (yoffs - cy2) * (yoffs - cy2))>>7 ));

      valueR = gamma8(cos8_t(valueR));
      valueG = gamma8(cos8_t(valueG));
      valueB = gamma8(cos8_t(valueB));

      SEGMENT.setPixelColorXY(x, y, RGBW32(valueR, valueG, valueB, 0)); 
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__DISTORTION_WAVES__INDEXING[] PROGMEM = "Distortion Waves@!,Scale;;;2";


/*******************************************************************************************************************************************************************************************************************
 * @description : @Stepko Idea from https://www.youtube.com/watch?v=DiHBgITrZck&ab_channel=StefanPetrick adapted for WLED by @blazoncek
 * @note : Converted from WLED Effects "mode_2Dsoap"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Soap()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  const size_t dataSize = SEGMENT.width() * SEGMENT.height() * sizeof(uint8_t); // prevent reallocation if mirrored or grouped
  if (!SEGMENT.allocateData(dataSize + sizeof(uint32_t)*3)) return EFFECT_DEFAULT(); //allocation failed

  uint8_t  *noise3d   = reinterpret_cast<uint8_t*>(SEGMENT.data);
  uint32_t *noise32_x = reinterpret_cast<uint32_t*>(SEGMENT.data + dataSize);
  uint32_t *noise32_y = reinterpret_cast<uint32_t*>(SEGMENT.data + dataSize + sizeof(uint32_t));
  uint32_t *noise32_z = reinterpret_cast<uint32_t*>(SEGMENT.data + dataSize + sizeof(uint32_t)*2);
  const uint32_t scale32_x = 160000U/cols;
  const uint32_t scale32_y = 160000U/rows;
  const uint32_t mov = MIN(cols,rows)*(SEGMENT.speed+2)/2;
  const uint8_t  smoothness = MIN(250,SEGMENT.intensity); // limit as >250 produces very little changes

  // init
  if (SEGMENT.call == 0) {
    *noise32_x = hw_random();
    *noise32_y = hw_random();
    *noise32_z = hw_random();
  } else {
    *noise32_x += mov;
    *noise32_y += mov;
    *noise32_z += mov;
  }

  for (int i = 0; i < cols; i++) {
    int32_t ioffset = scale32_x * (i - cols / 2);
    for (int j = 0; j < rows; j++) {
      int32_t joffset = scale32_y * (j - rows / 2);
      uint8_t data = inoise16(*noise32_x + ioffset, *noise32_y + joffset, *noise32_z) >> 8;
      noise3d[XY(i,j)] = scale8(noise3d[XY(i,j)], smoothness) + scale8(data, 255 - smoothness);
    }
  }
  // init also if dimensions changed
  if (SEGMENT.call == 0 || SEGMENT.aux0 != cols || SEGMENT.aux1 != rows) {
    SEGMENT.aux0 = cols;
    SEGMENT.aux1 = rows;
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        SEGMENT.setPixelColorXY(i, j, ColorFromPalette(SEGPALETTE,~noise3d[XY(i,j)]*3));
      }
    }
  }

  int zD;
  int zF;
  int amplitude;
  int shiftX = 0; //(SEGMENT.custom1 - 128) / 4;
  int shiftY = 0; //(SEGMENT.custom2 - 128) / 4;
  CRGB ledsbuff[MAX(cols,rows)];

  amplitude = (cols >= 16) ? (cols-8)/8 : 1;
  for (int y = 0; y < rows; y++) {
    int amount   = ((int)noise3d[XY(0,y)] - 128) * 2 * amplitude + 256*shiftX;
    int delta    = abs(amount) >> 8;
    int fraction = abs(amount) & 255;
    for (int x = 0; x < cols; x++) {
      if (amount < 0) {
        zD = x - delta;
        zF = zD - 1;
      } else {
        zD = x + delta;
        zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black;
      if ((zD >= 0) && (zD < cols)) PixelA = SEGMENT.getPixelColorXY(zD, y);
      else                          PixelA = ColorFromPalette(SEGPALETTE, ~noise3d[XY(abs(zD),y)]*3);
      CRGB PixelB = CRGB::Black;
      if ((zF >= 0) && (zF < cols)) PixelB = SEGMENT.getPixelColorXY(zF, y);
      else                          PixelB = ColorFromPalette(SEGPALETTE, ~noise3d[XY(abs(zF),y)]*3);
      ledsbuff[x] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for (int x = 0; x < cols; x++) SEGMENT.setPixelColorXY(x, y, ledsbuff[x]);
  }

  amplitude = (rows >= 16) ? (rows-8)/8 : 1;
  for (int x = 0; x < cols; x++) {
    int amount   = ((int)noise3d[XY(x,0)] - 128) * 2 * amplitude + 256*shiftY;
    int delta    = abs(amount) >> 8;
    int fraction = abs(amount) & 255;
    for (int y = 0; y < rows; y++) {
      if (amount < 0) {
        zD = y - delta;
        zF = zD - 1;
      } else {
        zD = y + delta;
        zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black;
      if ((zD >= 0) && (zD < rows)) PixelA = SEGMENT.getPixelColorXY(x, zD);
      else                          PixelA = ColorFromPalette(SEGPALETTE, ~noise3d[XY(x,abs(zD))]*3); 
      CRGB PixelB = CRGB::Black;
      if ((zF >= 0) && (zF < rows)) PixelB = SEGMENT.getPixelColorXY(x, zF);
      else                          PixelB = ColorFromPalette(SEGPALETTE, ~noise3d[XY(x,abs(zF))]*3);
      ledsbuff[y] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for (int y = 0; y < rows; y++) SEGMENT.setPixelColorXY(x, y, ledsbuff[y]);
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__SOAP__INDEXING[] PROGMEM = "Soap@!,Smoothness;;!;2;pal=11";


/*******************************************************************************************************************************************************************************************************************
 * @description : Idea from https://www.youtube.com/watch?v=HsA-6KIbgto&ab_channel=GreatScott%21 Octopus (https://editor.soulmatelights.com/gallery/671-octopus) Stepko and Sutaburosu adapted for WLED by @blazoncek
 * @note : Converted from WLED Effects "mode_2Doctopus"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__Octopus()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;
  const uint8_t mapp = 180 / MAX(cols,rows);

  typedef struct {
    uint8_t angle;
    uint8_t radius;
  } map_t;

  const size_t dataSize = SEGMENT.width() * SEGMENT.height() * sizeof(map_t); // prevent reallocation if mirrored or grouped
  if (!SEGMENT.allocateData(dataSize + 2)) return EFFECT_DEFAULT(); //allocation failed

  map_t *rMap = reinterpret_cast<map_t*>(SEGMENT.data);
  uint8_t *offsX = reinterpret_cast<uint8_t*>(SEGMENT.data + dataSize);
  uint8_t *offsY = reinterpret_cast<uint8_t*>(SEGMENT.data + dataSize + 1);

  // re-init if SEGMENT dimensions or offset changed
  if (SEGMENT.call == 0 || SEGMENT.aux0 != cols || SEGMENT.aux1 != rows || SEGMENT.custom1 != *offsX || SEGMENT.custom2 != *offsY) {
    SEGMENT.step = 0; // t
    SEGMENT.aux0 = cols;
    SEGMENT.aux1 = rows;
    *offsX = SEGMENT.custom1;
    *offsY = SEGMENT.custom2;
    const int C_X = (cols / 2) + ((SEGMENT.custom1 - 128)*cols)/255;
    const int C_Y = (rows / 2) + ((SEGMENT.custom2 - 128)*rows)/255;
    for (int x = 0; x < cols; x++) {
      for (int y = 0; y < rows; y++) {
        int dx = (x - C_X);
        int dy = (y - C_Y);
        rMap[XY(x, y)].angle  = int(40.7436f * atan2_t(dy, dx));  // avoid 128*atan2()/PI
        rMap[XY(x, y)].radius = sqrtf(dx * dx + dy * dy) * mapp; //thanks Sutaburosu
      }
    }
  }

  SEGMENT.step += SEGMENT.speed / 32 + 1;  // 1-4 range
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      byte angle = rMap[XY(x,y)].angle;
      byte radius = rMap[XY(x,y)].radius;
      //CRGB c = CHSV(SEGMENT.step / 2 - radius, 255, sin8_t(sin8_t((angle * 4 - radius) / 4 + SEGMENT.step) + radius - SEGMENT.step * 2 + angle * (SEGMENT.custom3/3+1)));
      unsigned intensity = sin8_t(sin8_t((angle * 4 - radius) / 4 + SEGMENT.step/2) + radius - SEGMENT.step + angle * (SEGMENT.custom3/4+1));
      intensity = map((intensity*intensity) & 0xFFFF, 0, 65535, 0, 255); // add a bit of non-linearity for cleaner display
      SEGMENT.setPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, SEGMENT.step / 2 - radius, intensity));
    }
  }
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__OCTOPUS__INDEXING[] PROGMEM = "Octopus@!,,Offset X,Offset Y,Legs,fasttan;;!;2;";


/*******************************************************************************************************************************************************************************************************************
 * @description : Waving Cell @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells) adapted for WLED by @blazoncek, improvements by @dedehai
 * @note : Converted from WLED Effects "mode_2Dwavingcell"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__2D__WavingCell()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  uint32_t t = (effect_start_time*(SEGMENT.speed + 1))>>3;
  uint32_t aX = SEGMENT.custom1/16 + 9;
  uint32_t aY = SEGMENT.custom2/16 + 1;
  uint32_t aZ = SEGMENT.custom3 + 1;
   for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      uint32_t wave = sin8_t((x * aX) + sin8_t((((y<<8) + t) * aY)>>8)) + cos8_t(y * aZ); // bit shifts to increase temporal resolution
      uint8_t colorIndex = wave + (t>>(8-(SEGMENT.check2*3)));
      SEGMENT.setPixelColorXY(x, y, ColorFromPalette(SEGPALETTE, colorIndex));
    }
  }
  SEGMENT.blur(SEGMENT.intensity);
  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__2D__WAVING_CELL__INDEXING[] PROGMEM = "Waving Cell@!,Blur,Amplitude 1,Amplitude 2,Amplitude 3,,Flow;;!;2;ix=0";


#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO   END OF SECTION //////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////
/********************     audio enhanced routines     ************************/
///////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D


mAnimatorLight::um_data_t* mAnimatorLight::getAudioData() {
  um_data_t *um_data;
  // if (!UsermodManager::getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = tkr_anim->simulateSound(pSEGMENT.soundSim);
  // }
  return um_data;
}


#endif



#ifdef   ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D     /////////////////////////////////////////////////////////////////////////////////////////////////////////



/*******************************************************************************************************************************************************************************************************************
 * @description : AuroraWave base
 * @note : Converted from WLED Effects 
 ********************************************************************************************************************************************************************************************************************/

//CONFIG
#ifdef ESP8266
  #define W_MAX_COUNT  9          //Number of simultaneous waves
#else
  #define W_MAX_COUNT 20          //Number of simultaneous waves
#endif
#define W_MAX_SPEED 6             //Higher number, higher speed
#define W_WIDTH_FACTOR 6          //Higher number, smaller waves

//24 bytes
class AuroraWave {
  private:
    uint16_t ttl;
    CRGB basecolor;
    float basealpha;
    uint16_t age;
    uint16_t width;
    float center;
    bool goingleft;
    float speed_factor;
    bool alive = true;

  public:
    void init(uint32_t segment_length, CRGB color) {
      ttl = tkr_anim->hw_random16(500, 1501);
      basecolor = color;
      basealpha = tkr_anim->hw_random8(60, 101) / (float)100;
      age = 0;
      width = tkr_anim->hw_random16(segment_length / 20, segment_length / W_WIDTH_FACTOR); //half of width to make math easier
      if (!width) width = 1;
      center = tkr_anim->hw_random8(101) / (float)100 * segment_length;
      goingleft = tkr_anim->hw_random8(0, 2) == 0;
      speed_factor = (tkr_anim->hw_random8(10, 31) / (float)100 * W_MAX_SPEED / 255);
      alive = true;
    }

    CRGB getColorForLED(int ledIndex) {
      if(ledIndex < center - width || ledIndex > center + width) return 0; //Position out of range of this wave

      CRGB rgb;

      //Offset of this led from center of wave
      //The further away from the center, the dimmer the LED
      float offset = ledIndex - center;
      if (offset < 0) offset = -offset;
      float offsetFactor = offset / width;

      //The age of the wave determines it brightness.
      //At half its maximum age it will be the brightest.
      float ageFactor = 0.1;
      if((float)age / ttl < 0.5) {
        ageFactor = (float)age / (ttl / 2);
      } else {
        ageFactor = (float)(ttl - age) / ((float)ttl * 0.5);
      }

      //Calculate color based on above factors and basealpha value
      float factor = (1 - offsetFactor) * ageFactor * basealpha;
      rgb.r = basecolor.r * factor;
      rgb.g = basecolor.g * factor;
      rgb.b = basecolor.b * factor;

      return rgb;
    };

    //Change position and age of wave
    //Determine if its sill "alive"
    void update(uint32_t segment_length, uint32_t speed) {
      if(goingleft) {
        center -= speed_factor * speed;
      } else {
        center += speed_factor * speed;
      }

      age++;

      if(age > ttl) {
        alive = false;
      } else {
        if(goingleft) {
          if(center + width < 0) {
            alive = false;
          }
        } else {
          if(center - width > segment_length) {
            alive = false;
          }
        }
      }
    };

    bool stillAlive() {
      return alive;
    };
};


/*******************************************************************************************************************************************************************************************************************
 * @description : Aurora
 * @note : Converted from WLED Effects "mode_aurora"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Aurora()
{
  //aux1 = Wavecount
  //aux2 = Intensity in last loop

  AuroraWave* waves;

  //TODO: I am not sure this is a correct way of handling memory allocation since if it fails on 1st run
  // it will display static effect but on second run it may crash ESP since data will be nullptr

  if(SEGMENT.aux0 != SEGMENT.intensity || SEGMENT.call == 0) {
    //Intensity slider changed or first call
    SEGMENT.aux1 = map(SEGMENT.intensity, 0, 255, 2, W_MAX_COUNT);
    SEGMENT.aux0 = SEGMENT.intensity;

    if(!SEGMENT.allocateData(sizeof(AuroraWave) * SEGMENT.aux1)) { // 26 on 32 segment ESP32, 9 on 16 segment ESP8266
      return EFFECT_DEFAULT(); //allocation failed
    }

    waves = reinterpret_cast<AuroraWave*>(SEGMENT.data);

    for (int i = 0; i < SEGMENT.aux1; i++) {
      waves[i].init(SEGLEN, CRGB(SEGMENT.color_from_palette(hw_random8(), false, false, hw_random8(0, 3))));
    }
  } else {
    waves = reinterpret_cast<AuroraWave*>(SEGMENT.data);
  }

  for (int i = 0; i < SEGMENT.aux1; i++) {
    //Update values of wave
    waves[i].update(SEGLEN, SEGMENT.speed);

    if(!(waves[i].stillAlive())) {
      //If a wave dies, reinitialize it starts over.
      waves[i].init(SEGLEN, CRGB(SEGMENT.color_from_palette(hw_random8(), false, false, hw_random8(0, 3))));
    }
  }

  uint8_t backlight = 1; //dimmer backlight if less active colors
  if (SEGCOLOR(0)) backlight++;
  if (SEGCOLOR(1)) backlight++;
  if (SEGCOLOR(2)) backlight++;
  //Loop through LEDs to determine color
  for (unsigned i = 0; i < SEGLEN; i++) {
    CRGB mixedRgb = CRGB(backlight, backlight, backlight);

    //For each LED we must check each wave if it is "active" at this position.
    //If there are multiple waves active on a LED we multiply their values.
    for (int  j = 0; j < SEGMENT.aux1; j++) {
      CRGB rgb = waves[j].getColorForLED(i);

      if(rgb != CRGB(0)) {
        mixedRgb += rgb;
      }
    }

    SEGMENT.setPixelColor(i, mixedRgb[0], mixedRgb[1], mixedRgb[2]);
  }

  return FRAMETIME;

}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_AURORA[] PROGMEM = "Aurora@!,!;1,2,3;!;;sx=24,pal=50";



/*******************************************************************************************************************************************************************************************************************
 * @description : Ripple peak. By Andrew Tuline. // This currently has no controls.
 * @note : Converted from WLED Effects "mode_ripplepeak"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Ripple_Peak()
{
  #define MAXSTEPS 16                                     // Case statement wouldn't allow a variable.

  unsigned maxRipples = 16;
  unsigned dataSize = sizeof(Ripple) * maxRipples;
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed
  Ripple* ripples = reinterpret_cast<Ripple*>(SEGMENT.data);

  um_data_t *um_data = getAudioData();
  uint8_t samplePeak    = *(uint8_t*)um_data->u_data[3];
  #ifdef ESP32
  float   FFT_MajorPeak = *(float*)  um_data->u_data[4];
  #endif
  uint8_t *maxVol       =  (uint8_t*)um_data->u_data[6];
  uint8_t *binNum       =  (uint8_t*)um_data->u_data[7];

  // printUmData();

  if (SEGMENT.call == 0) {
    SEGMENT.custom1 = *binNum;
    SEGMENT.custom2 = *maxVol * 2;
  }

  *binNum = SEGMENT.custom1;                              // Select a bin.
  *maxVol = SEGMENT.custom2 / 2;                          // Our volume comparator.

  SEGMENT.fade_out(240);                                  // Lower frame rate means less effective fading than FastLED
  SEGMENT.fade_out(240);

  for (int i = 0; i < SEGMENT.intensity/16; i++) {   // Limit the number of ripples.
    if (samplePeak) ripples[i].state = 255;

    switch (ripples[i].state) {
      case 254:     // Inactive mode
        break;

      case 255:                                           // Initialize ripple variables.
        ripples[i].pos = hw_random16(SEGLEN);
        #ifdef ESP32
          if (FFT_MajorPeak > 1)                          // log10(0) is "forbidden" (throws exception)
          ripples[i].color = (int)(log10f(FFT_MajorPeak)*128);
          else ripples[i].color = 0;
        #else
          ripples[i].color = hw_random8();
        #endif
        ripples[i].state = 0;
        break;

      case 0:
        SEGMENT.setPixelColor(ripples[i].pos, SEGMENT.color_from_palette(ripples[i].color, false, PALETTE_SOLID_WRAP, 0));
        ripples[i].state++;
        break;

      case MAXSTEPS:                                      // At the end of the ripples. 254 is an inactive mode.
        ripples[i].state = 254;
        break;

      default:                                            // Middle of the ripples.
        SEGMENT.setPixelColor((ripples[i].pos + ripples[i].state + SEGLEN) % SEGLEN, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(ripples[i].color, false, PALETTE_SOLID_WRAP, 0), uint8_t(2*255/ripples[i].state)));
        SEGMENT.setPixelColor((ripples[i].pos - ripples[i].state + SEGLEN) % SEGLEN, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(ripples[i].color, false, PALETTE_SOLID_WRAP, 0), uint8_t(2*255/ripples[i].state)));
        ripples[i].state++;                               // Next step.
        break;
    } // switch step
  } // for i

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_RIPPLE_PEAK[] PROGMEM = "Ripple Peak@Fade rate,Max # of ripples,Select bin,Volume (min);!,!;!;1v;c2=0,m12=0,si=0"; // Pixel, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 16 bit perlinmove. Use Perlin Noise instead of sinewaves for movement. By Andrew Tuline.
           Controls are speed, # of pixels, faderate.
 * @note : Converted from WLED Effects "mode_ripplepeak"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Perline_Move()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  SEGMENT.fade_out(255-SEGMENT.custom1);
  for (int i = 0; i < SEGMENT.intensity/16 + 1; i++) {
    unsigned locn = inoise16(effect_start_time*128/(260-SEGMENT.speed)+i*15000, effect_start_time*128/(260-SEGMENT.speed)); // Get a new pixel location from moving noise.
    unsigned pixloc = map(locn, 50*256, 192*256, 0, SEGLEN-1);                                            // Map that to the length of the strand, and ensure we don't go over.
    SEGMENT.setPixelColor(pixloc, SEGMENT.color_from_palette(pixloc%255, false, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PERLINE_MOVE[] PROGMEM = "Ripple Peak@Fade rate,Max # of ripples,Select bin,Volume (min);!,!;!;1v;c2=0,m12=0,si=0"; // Pixel, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Gravcenter effects By Andrew Tuline.
 *         Gravcenter base function for Gravcenter (0), Gravcentric (1), Gravimeter (2), Gravfreq (3) (merged by @dedehai)
 * @note : Converted from WLED Effects "mode_gravcenter_base"
 ********************************************************************************************************************************************************************************************************************/
// Gravity struct requited for GRAV* effects
typedef struct Gravity {
  int    topLED;
  int    gravityCounter;
} gravity;
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Grav__Base(unsigned mode)
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();

  const unsigned dataSize = sizeof(gravity);
  if (!SEGMENT.allocateData(dataSize)) return EFFECT_DEFAULT(); //allocation failed
  Gravity* gravcen = reinterpret_cast<Gravity*>(SEGMENT.data);

  um_data_t *um_data = getAudioData();
  float   volumeSmth  = *(float*)  um_data->u_data[0];

  if(mode == 1) SEGMENT.fade_out(253);  // //Gravcentric
  else if(mode == 2) SEGMENT.fade_out(249);  // Gravimeter
  else if(mode == 3) SEGMENT.fade_out(250);  // Gravfreq
  else SEGMENT.fade_out(251);  // Gravcenter

  float mySampleAvg;
  int tempsamp;
  float segmentSampleAvg = volumeSmth * (float)SEGMENT.intensity / 255.0f;

  if(mode == 2) { //Gravimeter
    segmentSampleAvg *= 0.25; // divide by 4, to compensate for later "sensitivity" upscaling
    mySampleAvg = mapf(segmentSampleAvg*2.0, 0, 64, 0, (SEGLEN-1)); // map to pixels availeable in current segment
    tempsamp = constrain(mySampleAvg,0,SEGLEN-1);       // Keep the sample from overflowing.
  }
  else { // Gravcenter or Gravcentric or Gravfreq
    segmentSampleAvg *= 0.125f; // divide by 8, to compensate for later "sensitivity" upscaling
    mySampleAvg = mapf(segmentSampleAvg*2.0, 0.0f, 32.0f, 0.0f, (float)SEGLEN/2.0f); // map to pixels availeable in current segment
    tempsamp = constrain(mySampleAvg, 0, SEGLEN/2);     // Keep the sample from overflowing.
  }

  uint8_t gravity = 8 - SEGMENT.speed/32;
  int offset = 1;
  if(mode == 2) offset = 0;  // Gravimeter
  if (tempsamp >= gravcen->topLED) gravcen->topLED = tempsamp-offset;
  else if (gravcen->gravityCounter % gravity == 0) gravcen->topLED--;
  
  if(mode == 1) {  //Gravcentric
    for (int i=0; i<tempsamp; i++) {
      uint8_t index = segmentSampleAvg*24+effect_start_time/200;
      SEGMENT.setPixelColor(i+SEGLEN/2, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
      SEGMENT.setPixelColor(SEGLEN/2-1-i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
    }
    if (gravcen->topLED >= 0) {
      SEGMENT.setPixelColor(gravcen->topLED+SEGLEN/2, HTMLColorCode::Gray);
      SEGMENT.setPixelColor(SEGLEN/2-1-gravcen->topLED, HTMLColorCode::Gray);
    }
  }
  else if(mode == 2) { //Gravimeter
    for (int i=0; i<tempsamp; i++) {
      uint8_t index = inoise8(i*segmentSampleAvg+effect_start_time, 5000+i*segmentSampleAvg);
      SEGMENT.setPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0), uint8_t(segmentSampleAvg*8)));
    }
    if (gravcen->topLED > 0) {
      SEGMENT.setPixelColor(gravcen->topLED, SEGMENT.color_from_palette(effect_start_time, false, PALETTE_SOLID_WRAP, 0));
    }
  }
  else if(mode == 3) { //Gravfreq
    for (int i=0; i<tempsamp; i++) {
      float   FFT_MajorPeak = *(float*)um_data->u_data[4]; // used in mode 3: Gravfreq
      if (FFT_MajorPeak < 1) FFT_MajorPeak = 1;
      uint8_t index = (log10f(FFT_MajorPeak) - (MAX_FREQ_LOG10 - 1.78f)) * 255;
      SEGMENT.setPixelColor(i+SEGLEN/2, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
      SEGMENT.setPixelColor(SEGLEN/2-i-1, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
    }
    if (gravcen->topLED >= 0) {
      SEGMENT.setPixelColor(gravcen->topLED+SEGLEN/2, HTMLColorCode::Gray);
      SEGMENT.setPixelColor(SEGLEN/2-1-gravcen->topLED, HTMLColorCode::Gray);
    }
  }
  else { //Gravcenter
    for (int i=0; i<tempsamp; i++) {
      uint8_t index = inoise8(i*segmentSampleAvg+effect_start_time, 5000+i*segmentSampleAvg);
      SEGMENT.setPixelColor(i+SEGLEN/2, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0), uint8_t(segmentSampleAvg*8)));
      SEGMENT.setPixelColor(SEGLEN/2-i-1, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0), uint8_t(segmentSampleAvg*8)));
    }
    if (gravcen->topLED >= 0) {
      SEGMENT.setPixelColor(gravcen->topLED+SEGLEN/2, SEGMENT.color_from_palette(effect_start_time, false, PALETTE_SOLID_WRAP, 0));
      SEGMENT.setPixelColor(SEGLEN/2-1-gravcen->topLED, SEGMENT.color_from_palette(effect_start_time, false, PALETTE_SOLID_WRAP, 0));
    }
  } 
  gravcen->gravityCounter = (gravcen->gravityCounter + 1) % gravity;

  return FRAMETIME;
}

/*******************************************************************************************************************************************************************************************************************
 * @description : Gravfreq
 * @note : Converted from WLED Effects "mode_gravfreq"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravCenter(void) {                  // Gravcenter. By Andrew Tuline.
  return EffectAnim__AudioReactive__1D__FFT_Grav__Base(0);
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_CENTER[] PROGMEM = "Gravcenter@Rate of fall,Sensitivity;!,!;!;1v;ix=128,m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Gravfreq
 * @note : Converted from WLED Effects "mode_gravfreq"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravCentric(void) {                  // Gravcentric. By Andrew Tuline.
  return EffectAnim__AudioReactive__1D__FFT_Grav__Base(1);
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_CENTRIC[] PROGMEM = "Gravcentric@Rate of fall,Sensitivity;!,!;!;1v;ix=128,m12=3,si=0"; // Corner, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Gravfreq
 * @note : Converted from WLED Effects "mode_gravfreq"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravMeter(void) {                  // Gravmeter. By Andrew Tuline.
  return EffectAnim__AudioReactive__1D__FFT_Grav__Base(2);
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_METER[] PROGMEM = "Gravimeter@Rate of fall,Sensitivity;!,!;!;1v;ix=128,m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Gravfreq
 * @note : Converted from WLED Effects "mode_gravfreq"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravFreq(void) {                  // Gravfreq. By Andrew Tuline.
  return EffectAnim__AudioReactive__1D__FFT_Grav__Base(3);
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_FREQ[] PROGMEM = "Gravfreq@Rate of fall,Sensitivity;!,!;!;1f;ix=128,m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Gravfreq
 * @note : Converted from WLED Effects "mode_gravfreq"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Juggles()
{
  um_data_t *um_data = getAudioData();
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  SEGMENT.fade_out(224); // 6.25%
  uint8_t my_sampleAgc = fmax(fmin(volumeSmth, 255.0), 0);

  for (size_t i=0; i<SEGMENT.intensity/32+1U; i++) {
    // if SEGLEN equals 1, we will always set color to the first and only pixel, but the effect is still good looking
    SEGMENT.setPixelColor(beatsin16_t(SEGMENT.speed/4+i*2,0,SEGLEN-1), color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(effect_start_time/4+i*2, false, PALETTE_SOLID_WRAP, 0), my_sampleAgc));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_JUGGLES[] PROGMEM = "Juggles@!,# of balls;!,!;!;01v;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Gravfreq
 * @note : Converted from WLED Effects "mode_matripix"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Matripix()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment

  um_data_t *um_data = getAudioData();
  int volumeRaw    = *(int16_t*)um_data->u_data[1];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500 % 16;
  if(SEGMENT.aux0 != secondHand) {
    SEGMENT.aux0 = secondHand;

    uint8_t pixBri = volumeRaw * SEGMENT.intensity / 64;
    for (unsigned i = 0; i < SEGLEN-1; i++) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // shift left
    SEGMENT.setPixelColor(SEGLEN-1, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(effect_start_time, false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_MATRIPIX[] PROGMEM = "Matripix@!,Brightness;!,!;!;1v;ix=64,m12=2,si=1"; //,rev=1,mi=1,rY=1,mY=1 Circle, WeWillRockYou, reverseX


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_midnoise"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_MidNoise()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  // Changing xdist to SEGMENT.aux0 and ydist to SEGMENT.aux1.

  um_data_t *um_data = getAudioData();
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  SEGMENT.fade_out(SEGMENT.speed);
  SEGMENT.fade_out(SEGMENT.speed);

  float tmpSound2 = volumeSmth * (float)SEGMENT.intensity / 256.0;  // Too sensitive.
  tmpSound2 *= (float)SEGMENT.intensity / 128.0;              // Reduce sensitivity/length.

  unsigned maxLen = mapf(tmpSound2, 0, 127, 0, SEGLEN/2);
  if (maxLen >SEGLEN/2) maxLen = SEGLEN/2;

  for (unsigned i=(SEGLEN/2-maxLen); i<(SEGLEN/2+maxLen); i++) {
    uint8_t index = inoise8(i*volumeSmth+SEGMENT.aux0, SEGMENT.aux1+i*volumeSmth);  // Get a value from the noise function. I'm using both x and y axis.
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.aux0=SEGMENT.aux0+beatsin8_t(5,0,10);
  SEGMENT.aux1=SEGMENT.aux1+beatsin8_t(4,0,10);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_MID_NOISE[] PROGMEM = "Midnoise@Fade rate,Max. length;!,!;!;1v;ix=128,m12=1,si=0"; // Bar, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Volume (only) reactive fire routine. 
 * @note : Converted from WLED Effects "mode_noisefire"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseFire()
{
  CRGBPalette16 myPal = CRGBPalette16(CHSV(0,255,2),    CHSV(0,255,4),    CHSV(0,255,8), CHSV(0, 255, 8),  // Fire palette definition. Lower value = darker.
                                      CHSV(0, 255, 16), CRGB::Red,        CRGB::Red,     CRGB::Red,
                                      CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Orange,  CRGB::Orange,
                                      CRGB::Yellow,     CRGB::Orange,     CRGB::Yellow,  CRGB::Yellow);

  um_data_t *um_data = getAudioData();
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  if (SEGMENT.call == 0) SEGMENT.fill(BLACK);

  for (unsigned i = 0; i < SEGLEN; i++) {
    unsigned index = inoise8(i*SEGMENT.speed/64,effect_start_time*SEGMENT.speed/64*SEGLEN/255);  // X location is constant, but we move along the Y at the rate of millis(). By Andrew Tuline.
    index = (255 - i*256/SEGLEN) * index/(256-SEGMENT.intensity);                       // Now we need to scale index so that it gets blacker as we get close to one of the ends.
                                                                                        // This is a simple y=mx+b equation that's been scaled. index/128 is another scaling.

    SEGMENT.setPixelColor(i, ColorFromPalette(myPal, index, volumeSmth*2, LINEARBLEND)); // Use my own palette.
  }

  return FRAMETIME;;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_NOISE_FIRE[] PROGMEM = "Noisefire@!,!;;;01v;m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : . 
 * @note : Converted from WLED Effects "mode_noisemeter"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseMeter()
{
  um_data_t *um_data = getAudioData();
  float   volumeSmth   = *(float*)  um_data->u_data[0];
  int volumeRaw    = *(int16_t*)um_data->u_data[1];

  //uint8_t fadeRate = map(SEGMENT.speed,0,255,224,255);
  uint8_t fadeRate = map(SEGMENT.speed,0,255,200,254);
  SEGMENT.fade_out(fadeRate);

  float tmpSound2 = volumeRaw * 2.0 * (float)SEGMENT.intensity / 255.0;
  unsigned maxLen = mapf(tmpSound2, 0, 255, 0, SEGLEN); // map to pixels availeable in current segment              // Still a bit too sensitive.
  if (maxLen < 0) maxLen = 0;
  if (maxLen > SEGLEN) maxLen = SEGLEN;

  for (unsigned i=0; i<maxLen; i++) {                                    // The louder the sound, the wider the soundbar. By Andrew Tuline.
    uint8_t index = inoise8(i*volumeSmth+SEGMENT.aux0, SEGMENT.aux1+i*volumeSmth);  // Get a value from the noise function. I'm using both x and y axis.
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.aux0+=beatsin8_t(5,0,10);
  SEGMENT.aux1+=beatsin8_t(4,0,10);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_NOISE_METER[] PROGMEM = "Noisemeter@Fade rate,Width;!,!;!;1v;ix=128,m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : . 
 * @note : Converted from WLED Effects "mode_pixelwave"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_PixelWave()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  um_data_t *um_data = getAudioData();
  int volumeRaw    = *(int16_t*)um_data->u_data[1];

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500+1 % 16;
  if (SEGMENT.aux0 != secondHand) {
    SEGMENT.aux0 = secondHand;

    uint8_t pixBri = volumeRaw * SEGMENT.intensity / 64;

    SEGMENT.setPixelColor(SEGLEN/2, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(effect_start_time, false, PALETTE_SOLID_WRAP, 0), pixBri));
    for (unsigned i = SEGLEN - 1; i > SEGLEN/2; i--) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); //move to the left
    for (unsigned i = 0; i < SEGLEN/2; i++)          SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // move to the right
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PIXEL_WAVE[] PROGMEM = "Pixelwave@!,Sensitivity;!,!;!;1v;ix=64,m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : . 
 * @note : Converted from WLED Effects "mode_plasmoid"
 ********************************************************************************************************************************************************************************************************************/

typedef struct Plasphase {
  int16_t    thisphase;
  int16_t    thatphase;
} plasphase;

uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Plasmoid()
{
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment
  if (!SEGMENT.allocateData(sizeof(plasphase))) return EFFECT_DEFAULT(); //allocation failed
  Plasphase* plasmoip = reinterpret_cast<Plasphase*>(SEGMENT.data);

  um_data_t *um_data = getAudioData();
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  SEGMENT.fadeToBlackBy(32);

  plasmoip->thisphase += beatsin8_t(6,-4,4);                          // You can change direction and speed individually.
  plasmoip->thatphase += beatsin8_t(7,-4,4);                          // Two phase values to make a complex pattern. By Andrew Tuline.

  for (unsigned i = 0; i < SEGLEN; i++) {                          // For each of the LED's in the strand, set a brightness based on a wave as follows.
    // updated, similar to "plasma" effect - softhack007
    uint8_t thisbright = cubicwave8(((i*(1 + (3*SEGMENT.speed/32)))+plasmoip->thisphase) & 0xFF)/2;
    thisbright += cos8_t(((i*(97 +(5*SEGMENT.speed/32)))+plasmoip->thatphase) & 0xFF)/2; // Let's munge the brightness a bit and animate it all with the phases.

    uint8_t colorIndex=thisbright;
    if (volumeSmth * SEGMENT.intensity / 64 < thisbright) {thisbright = 0;}

    SEGMENT.addPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(colorIndex, false, PALETTE_SOLID_WRAP, 0), thisbright));
  }

  return FRAMETIME;
} 
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PLASMOID[] PROGMEM = "Plasmoid@Phase,# of pixels;!,!;!;1v;sx=128,ix=128,m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Puddles/Puddlepeak By Andrew Tuline. Merged by @dedehai
 * @note : Converted from WLED Effects "mode_puddles_base"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Puddle__Base(bool peakdetect) {
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  unsigned size = 0;
  uint8_t fadeVal = map(SEGMENT.speed, 0, 255, 224, 254);
  unsigned pos = hw_random16(SEGLEN);                          // Set a random starting position.
  SEGMENT.fade_out(fadeVal);

  um_data_t *um_data = getAudioData();
  int volumeRaw    = *(int16_t*)um_data->u_data[1];
  uint8_t samplePeak = *(uint8_t*)um_data->u_data[3];
  uint8_t *maxVol    =  (uint8_t*)um_data->u_data[6];
  uint8_t *binNum    =  (uint8_t*)um_data->u_data[7];
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  if(peakdetect) {                                          // puddles peak
    *binNum = SEGMENT.custom1;                              // Select a bin.
    *maxVol = SEGMENT.custom2 / 2;                          // Our volume comparator.
    if (samplePeak == 1) {
      size = volumeSmth * SEGMENT.intensity /256 /4 + 1;    // Determine size of the flash based on the volume.
      if (pos+size>= SEGLEN) size = SEGLEN - pos;
    }
  }
  else {                                                    // puddles  
    if (volumeRaw > 1) {
      size = volumeRaw * SEGMENT.intensity /256 /8 + 1;     // Determine size of the flash based on the volume.
      if (pos+size >= SEGLEN) size = SEGLEN - pos;
    } 
  }
  
  for (unsigned i=0; i<size; i++) {                          // Flash the LED's.
    SEGMENT.setPixelColor(pos+i, SEGMENT.color_from_palette(effect_start_time, false, PALETTE_SOLID_WRAP, 0));
  }

  return FRAMETIME;
} 


/*******************************************************************************************************************************************************************************************************************
 * @description : . 
 * @note : Converted from WLED Effects "mode_puddlepeak"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_PuddlePeak(void) {                // Puddlepeak. By Andrew Tuline.
  return EffectAnim__AudioReactive__1D__FFT_Puddle__Base(true);
} 
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PUDDLE_PEAK[] PROGMEM = "Puddlepeak@Fade rate,Puddle size,Select bin,Volume (min);!,!;!;1v;c2=0,m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : . 
 * @note : Converted from WLED Effects "mode_puddles"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Puddles(void) {                   // Puddles. By Andrew Tuline.
  return EffectAnim__AudioReactive__1D__FFT_Puddle__Base(false);
} 
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PUDDLES[] PROGMEM = "Puddles@Fade rate,Puddle size;!,!;!;1v;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_pixels"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Pixels()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();

  if (!SEGMENT.allocateData(32*sizeof(uint8_t))) return EFFECT_DEFAULT(); //allocation failed
  uint8_t *myVals = reinterpret_cast<uint8_t*>(SEGMENT.data); // Used to store a pile of samples because WLED frame rate and WLED sample rate are not synchronized. Frame rate is too low.

  um_data_t *um_data = getAudioData();
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  myVals[effect_start_time%32] = volumeSmth;    // filling values semi randomly

  SEGMENT.fade_out(64+(SEGMENT.speed>>1));

  for (int i=0; i <SEGMENT.intensity/8; i++) {
    unsigned segLoc = hw_random16(SEGLEN);                    // 16 bit for larger strands of LED's.
    SEGMENT.setPixelColor(segLoc, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(myVals[i%32]+i*4, false, PALETTE_SOLID_WRAP, 0), uint8_t(volumeSmth)));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PIXELS[] PROGMEM = "Pixels@Fade rate,# of pixels;!,!;!;1v;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Ripple peak. By Andrew Tuline. // This currently has no controls.
 * @note : Converted from WLED Effects "mode_blurz"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Blurz()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment

  um_data_t *um_data = getAudioData();
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.aux0 = 0;
  }

  int fadeoutDelay = (256 - SEGMENT.speed) / 32;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fade_out(SEGMENT.speed);

  SEGMENT.step += FRAMETIME;
  if (SEGMENT.step > SPEED_FORMULA_L) {
    unsigned segLoc = hw_random16(SEGLEN);
    SEGMENT.setPixelColor(segLoc, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(2*fftResult[SEGMENT.aux0%16]*240/max(1, (int)SEGLEN-1), false, PALETTE_SOLID_WRAP, 0), uint8_t(2*fftResult[SEGMENT.aux0%16])));
    ++(SEGMENT.aux0) %= 16; // make sure it doesn't cross 16

    SEGMENT.step = 1;
    SEGMENT.blur(SEGMENT.intensity); // note: blur > 210 results in a alternating pattern, this could be fixed by mapping but some may like it (very old bug)
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_BLURZ[] PROGMEM = "Blurz@Fade rate,Blur;!,Color mix;!;1f;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_DJLight"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_DJLight()
{
 
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  const int mid = SEGLEN / 2;

  um_data_t *um_data = getAudioData();
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500+1 % 64;
  if (SEGMENT.aux0 != secondHand) {                        // Triggered millis timing.
    SEGMENT.aux0 = secondHand;

    CRGB color = CRGB(fftResult[15]/2, fftResult[5]/2, fftResult[0]/2); // 16-> 15 as 16 is out of bounds
    SEGMENT.setPixelColor(mid, color.fadeToBlackBy(map(fftResult[4], 0, 255, 255, 4)));     // TODO - Update

    for (int i = SEGLEN - 1; i > mid; i--)   SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); // move to the left
    for (int i = 0; i < mid; i++)            SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // move to the right
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_DJ_LIGHT[] PROGMEM = "DJ Light@Speed;;;1f;m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_freqmap"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqMap()
{
  if (SEGLEN == 1) return EFFECT_DEFAULT();
  // Start frequency = 60 Hz and log10(60) = 1.78
  // End frequency = MAX_FREQUENCY in Hz and lo10(MAX_FREQUENCY) = MAX_FREQ_LOG10

  um_data_t *um_data = getAudioData();
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float my_magnitude  = *(float*)um_data->u_data[5] / 4.0f;
  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1;                                         // log10(0) is "forbidden" (throws exception)

  if (SEGMENT.call == 0) SEGMENT.fill(BLACK);
  int fadeoutDelay = (256 - SEGMENT.speed) / 32;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fade_out(SEGMENT.speed);

  int locn = (log10f((float)FFT_MajorPeak) - 1.78f) * (float)SEGLEN/(MAX_FREQ_LOG10 - 1.78f);  // log10 frequency range is from 1.78 to 3.71. Let's scale to SEGLEN.
  if (locn < 1) locn = 0; // avoid underflow

  if (locn >= (int)SEGLEN) locn = SEGLEN-1;
  unsigned pixCol = (log10f(FFT_MajorPeak) - 1.78f) * 255.0f/(MAX_FREQ_LOG10 - 1.78f);   // Scale log10 of frequency values to the 255 colour index.
  if (FFT_MajorPeak < 61.0f) pixCol = 0;                                                 // handle underflow

  uint8_t bright = (uint8_t)my_magnitude;

  SEGMENT.setPixelColor(locn, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(SEGMENT.intensity+pixCol, false, PALETTE_SOLID_WRAP, 0), bright));

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_MAP[] PROGMEM = "Freqmap@Fade rate,Starting color;!,!;!;1f;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_freqmatrix"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqMatrix()
{
  // No need to prevent from executing on single led strips, we simply change pixel 0 each time and avoid the shift
  um_data_t *um_data = getAudioData();
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float volumeSmth    = *(float*)um_data->u_data[0];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500 % 16;
  if(SEGMENT.aux0 != secondHand) {
    SEGMENT.aux0 = secondHand;

    uint8_t sensitivity = map(SEGMENT.custom3, 0, 31, 1, 10); // reduced resolution slider
    int pixVal = (volumeSmth * SEGMENT.intensity * sensitivity) / 256.0f;
    if (pixVal > 255) pixVal = 255;

    float intensity = map(pixVal, 0, 255, 0, 100) / 100.0f;  // make a brightness from the last avg

    CRGB color = CRGB::Black;

    if (FFT_MajorPeak > MAX_FREQUENCY) FFT_MajorPeak = 1;
    // MajorPeak holds the freq. value which is most abundant in the last sample.
    // With our sampling rate of 10240Hz we have a usable freq range from roughly 80Hz to 10240/2 Hz
    // we will treat everything with less than 65Hz as 0

    if (FFT_MajorPeak < 80) {
      color = CRGB::Black;
    } else {
      int upperLimit = 80 + 42 * SEGMENT.custom2;
      int lowerLimit = 80 + 3 * SEGMENT.custom1;
      uint8_t i =  lowerLimit!=upperLimit ? map(FFT_MajorPeak, lowerLimit, upperLimit, 0, 255) : FFT_MajorPeak;  // may under/overflow - so we enforce uint8_t
      unsigned b = 255 * intensity;
      if (b > 255) b = 255;
      color = CHSV(i, 240, (uint8_t)b); // implicit conversion to RGB supplied by FastLED
    }

    // shift the pixels one pixel up
    SEGMENT.setPixelColor(0, color);
    // if SEGLEN equals 1 this loop won't execute
    for (int i = SEGLEN - 1; i > 0; i--) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); //move to the left
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_MATRIX[] PROGMEM = "Freqmatrix@Speed,Sound effect,Low bin,High bin,Sensitivity;;;1f;m12=3,si=0"; // Corner, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * // Start frequency = 60 Hz and log10(60) = 1.78
// End frequency = 5120 Hz and lo10(5120) = 3.71
//  SEGMENT.speed select faderate
//  SEGMENT.intensity select colour index
 * @note : Converted from WLED Effects "mode_freqpixels"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqPixels()
{
  um_data_t *um_data = getAudioData();
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float my_magnitude  = *(float*)um_data->u_data[5] / 16.0f;
  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1.0f; // log10(0) is "forbidden" (throws exception)

  // this code translates to speed * (2 - speed/255) which is a) speed*2 or b) speed (when speed is 255)
  // and since fade_out() can only take 0-255 it will behave incorrectly when speed > 127
  //uint16_t fadeRate = 2*SEGMENT.speed - SEGMENT.speed*SEGMENT.speed/255;    // Get to 255 as quick as you can.
  unsigned fadeRate = SEGMENT.speed*SEGMENT.speed; // Get to 255 as quick as you can.
  fadeRate = map(fadeRate, 0, 65535, 1, 255);

  int fadeoutDelay = (256 - SEGMENT.speed) / 64;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fade_out(fadeRate);

  uint8_t pixCol = (log10f(FFT_MajorPeak) - 1.78f) * 255.0f/(MAX_FREQ_LOG10 - 1.78f);  // Scale log10 of frequency values to the 255 colour index.
  if (FFT_MajorPeak < 61.0f) pixCol = 0;                                               // handle underflow
  for (int i=0; i < SEGMENT.intensity/32+1; i++) {
    unsigned locn = hw_random16(0,SEGLEN);
    SEGMENT.setPixelColor(locn, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(SEGMENT.intensity+pixCol, false, PALETTE_SOLID_WRAP, 0), (uint8_t)my_magnitude));
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_PIXELS[] PROGMEM = "Freqpixels@Fade rate,Starting color and # of pixels;!,!,;!;1f;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Assign a color to the central (starting pixels) based on the predominant frequencies and the volume. The color is being determined by mapping the MajorPeak from the FFT
           and then mapping this to the HSV color circle. Currently we are sampling at 10240 Hz, so the highest frequency we can look at is 5120Hz.

           SEGMENT.custom1: the lower cut off point for the FFT. (many, most time the lowest values have very little information since they are FFT conversion artifacts. Suggested value is close to but above 0
           SEGMENT.custom2: The high cut off point. This depends on your sound profile. Most music looks good when this slider is between 50% and 100%.
           SEGMENT.custom3: "preamp" for the audio signal for audio10.

           I suggest that for this effect you turn the brightness to 95%-100% but again it depends on your soundprofile you find yourself in.
           Instead of using colorpalettes, This effect works on the HSV color circle with red being the lowest frequency

           As a compromise between speed and accuracy we are currently sampling with 10240Hz, from which we can then determine with a 512bin FFT our max frequency is 5120Hz.
           Depending on the music stream you have you might find it useful to change the frequency mapping.
 * @note : Converted from WLED Effects "mode_freqwave"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqWave()
{
  // As before, this effect can also work on single pixels, we just lose the shifting effect
  um_data_t *um_data = getAudioData();
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float volumeSmth    = *(float*)um_data->u_data[0];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500 % 16;
  if(SEGMENT.aux0 != secondHand) {
    SEGMENT.aux0 = secondHand;

    float sensitivity = mapf(SEGMENT.custom3, 1, 31, 1, 10); // reduced resolution slider
    float pixVal = min(255.0f, volumeSmth * (float)SEGMENT.intensity / 256.0f * sensitivity);
    float intensity = mapf(pixVal, 0.0f, 255.0f, 0.0f, 100.0f) / 100.0f;  // make a brightness from the last avg

    CRGB color = 0;

    if (FFT_MajorPeak > MAX_FREQUENCY) FFT_MajorPeak = 1.0f;
    // MajorPeak holds the freq. value which is most abundant in the last sample.
    // With our sampling rate of 10240Hz we have a usable freq range from roughly 80Hz to 10240/2 Hz
    // we will treat everything with less than 65Hz as 0

    if (FFT_MajorPeak < 80) {
      color = CRGB(0);
    } else {
      int upperLimit = 80 + 42 * SEGMENT.custom2;
      int lowerLimit = 80 + 3 * SEGMENT.custom1;
      uint8_t i =  lowerLimit!=upperLimit ? map(FFT_MajorPeak, lowerLimit, upperLimit, 0, 255) : FFT_MajorPeak; // may under/overflow - so we enforce uint8_t
      unsigned b = min(255.0f, 255.0f * intensity);
      color = CHSV(i, 240, (uint8_t)b); // implicit conversion to RGB supplied by FastLED
    }

    SEGMENT.setPixelColor(SEGLEN/2, color);

    // shift the pixels one pixel outwards
    // if SEGLEN equals 1 these loops won't execute
    for (unsigned i = SEGLEN - 1; i > SEGLEN/2; i--) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); //move to the left
    for (unsigned i = 0; i < SEGLEN/2; i++)          SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // move to the right
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_WAVE[] PROGMEM = "Freqwave@Speed,Sound effect,Low bin,High bin,Pre-amp;;;01f;m12=2,si=0"; // Circle, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 
 * @note : Converted from WLED Effects "mode_noisemove"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseMove()
{
  um_data_t *um_data = getAudioData();
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  int fadeoutDelay = (256 - SEGMENT.speed) / 96;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fadeToBlackBy(4+ SEGMENT.speed/4);

  uint8_t numBins = map(SEGMENT.intensity,0,255,0,16);    // Map slider to fftResult bins.
  for (int i=0; i<numBins; i++) {                         // How many active bins are we using.
    unsigned locn = inoise16(effect_start_time*SEGMENT.speed+i*50000, effect_start_time*SEGMENT.speed);   // Get a new pixel location from moving noise.
    // if SEGLEN equals 1 locn will be always 0, hence we set the first pixel only
    locn = map(locn, 7500, 58000, 0, SEGLEN-1);           // Map that to the length of the strand, and ensure we don't go over.
    SEGMENT.setPixelColor(locn, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(i*64, false, PALETTE_SOLID_WRAP, 0), uint8_t(fftResult[i % 16]*4)));
  }

  return FRAMETIME;
} // mode_noisemove()
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_NOISE_MOVE[] PROGMEM = "Noisemove@Speed of perlin movement,Fade rate;!,!;!;1f;m12=0,si=0"; // Pixels, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Rocktaves. Same note from each octave is same colour.    By: Andrew Tuline
 * @note : Converted from WLED Effects "mode_rocktaves"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_RockTaves()
{
  um_data_t *um_data = getAudioData();
  float   FFT_MajorPeak = *(float*)  um_data->u_data[4];
  float   my_magnitude  = *(float*)   um_data->u_data[5] / 16.0f;

  SEGMENT.fadeToBlackBy(16);                              // Just in case something doesn't get faded.

  float frTemp = FFT_MajorPeak;
  uint8_t octCount = 0;                                   // Octave counter.
  uint8_t volTemp = 0;

  volTemp = 32.0f + my_magnitude * 1.5f;                  // brightness = volume (overflows are handled in next lines)
  if (my_magnitude < 48) volTemp = 0;                     // We need to squelch out the background noise.
  if (my_magnitude > 144) volTemp = 255;                  // everything above this is full brightness

  while ( frTemp > 249 ) {
    octCount++;                                           // This should go up to 5.
    frTemp = frTemp/2;
  }

  frTemp -= 132.0f;                                       // This should give us a base musical note of C3
  frTemp  = fabsf(frTemp * 2.1f);                         // Fudge factors to compress octave range starting at 0 and going to 255;

  unsigned i = map(beatsin8_t(8+octCount*4, 0, 255, 0, octCount*8), 0, 255, 0, SEGLEN-1);
  i = constrain(i, 0U, SEGLEN-1U);
  SEGMENT.addPixelColor(i, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette((uint8_t)frTemp, false, PALETTE_SOLID_WRAP, 0), volTemp));

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_ROCKTAVES[] PROGMEM = "Rocktaves@;!,!;!;01f;m12=1,si=0"; // Bar, Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : Combines peak detection with FFT_MajorPeak and FFT_Magnitude.
 * @note : Converted from WLED Effects "mode_waterfall"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Waterfall()
{
  
  um_data_t *um_data = getAudioData();
  uint8_t samplePeak    = *(uint8_t*)um_data->u_data[3];
  float   FFT_MajorPeak = *(float*)  um_data->u_data[4];
  uint8_t *maxVol       =  (uint8_t*)um_data->u_data[6];
  uint8_t *binNum       =  (uint8_t*)um_data->u_data[7];
  float   my_magnitude  = *(float*)   um_data->u_data[5] / 8.0f;

  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1;                                         // log10(0) is "forbidden" (throws exception)

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.aux0 = 255;
    SEGMENT.custom1 = *binNum;
    SEGMENT.custom2 = *maxVol * 2;
  }

  *binNum = SEGMENT.custom1;                              // Select a bin.
  *maxVol = SEGMENT.custom2 / 2;                          // Our volume comparator.

  uint8_t secondHand = micros() / (256-SEGMENT.speed)/500 + 1 % 16;
  if (SEGMENT.aux0 != secondHand) {                        // Triggered millis timing.
    SEGMENT.aux0 = secondHand;

    //uint8_t pixCol = (log10f((float)FFT_MajorPeak) - 2.26f) * 177;  // 10Khz sampling - log10 frequency range is from 2.26 (182hz) to 3.7 (5012hz). Let's scale accordingly.
    uint8_t pixCol = (log10f(FFT_MajorPeak) - 2.26f) * 150;           // 22Khz sampling - log10 frequency range is from 2.26 (182hz) to 3.967 (9260hz). Let's scale accordingly.
    if (FFT_MajorPeak < 182.0f) pixCol = 0;                           // handle underflow

    if (samplePeak) {
      SEGMENT.setPixelColor(SEGLEN-1, CHSV(92,92,92));
    } else {
      SEGMENT.setPixelColor(SEGLEN-1, color_blend(SEGCOLOR(1), SEGMENT.color_from_palette(pixCol+SEGMENT.intensity, false, PALETTE_SOLID_WRAP, 0), (uint8_t)my_magnitude));
    }
    // loop will not execute if SEGLEN equals 1
    for (unsigned i = 0; i < SEGLEN-1; i++) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // shift left
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_WATERFALL[] PROGMEM = "Waterfall@!,Adjust color,Select bin,Volume (min);!,!;!;01f;c2=0,m12=2,si=0"; // Circles, Beatsin

#endif //ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D     /////////////////////////////////////////////////////////////////////////////////////////////////////////  



#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D ////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************************************************************************************************************************************************
 * @description : 2D Swirl By: Mark Kriegsman https://gist.github.com/kriegsman/5adca44e14ad025e6d3b , modified by Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DSwirl"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__2D__Swirl()
{
  
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  const uint8_t borderWidth = 2;

  SEGMENT.blur(SEGMENT.custom1);

  int  i = beatsin8_t( 27*SEGMENT.speed/255, borderWidth, cols - borderWidth);
  int  j = beatsin8_t( 41*SEGMENT.speed/255, borderWidth, rows - borderWidth);
  int ni = (cols - 1) - i;
  int nj = (cols - 1) - j;

  um_data_t *um_data = getAudioData();
  float volumeSmth  = *(float*)   um_data->u_data[0]; //ewowi: use instead of sampleAvg???
  int   volumeRaw   = *(int16_t*) um_data->u_data[1];

  SEGMENT.addPixelColorXY( i, j, ColorFromPalette(SEGPALETTE, (effect_start_time / 11 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 11, 200, 255);
  SEGMENT.addPixelColorXY( j, i, ColorFromPalette(SEGPALETTE, (effect_start_time / 13 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 13, 200, 255);
  SEGMENT.addPixelColorXY(ni,nj, ColorFromPalette(SEGPALETTE, (effect_start_time / 17 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 17, 200, 255);
  SEGMENT.addPixelColorXY(nj,ni, ColorFromPalette(SEGPALETTE, (effect_start_time / 29 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 29, 200, 255);
  SEGMENT.addPixelColorXY( i,nj, ColorFromPalette(SEGPALETTE, (effect_start_time / 37 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 37, 200, 255);
  SEGMENT.addPixelColorXY(ni, j, ColorFromPalette(SEGPALETTE, (effect_start_time / 41 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 41, 200, 255);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__SWIRL__INDEXING[] PROGMEM = "Swirl@!,Sensitivity,Blur;,Bg Swirl;!;2v;ix=64,si=0"; // Beatsin // TODO: color 1 unused?


/*******************************************************************************************************************************************************************************************************************
 * @description : By: Stepko, https://editor.soulmatelights.com/gallery/652-wave , modified by Andrew Tuline
 * @note : Converted from WLED Effects "mode_2DWaverly"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__2D__Waverly()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  um_data_t *um_data = getAudioData();
  float   volumeSmth  = *(float*)   um_data->u_data[0];

  SEGMENT.fadeToBlackBy(SEGMENT.speed);

  long t = effect_start_time / 2;
  for (int i = 0; i < cols; i++) {
    unsigned thisVal = (1 + SEGMENT.intensity/64) * inoise8(i * 45 , t , t)/2;
    // use audio if available
    if (um_data) {
      thisVal /= 32; // reduce intensity of inoise8()
      thisVal *= volumeSmth;
    }
    int thisMax = map(thisVal, 0, 512, 0, rows);

    for (int j = 0; j < thisMax; j++) {
      SEGMENT.addPixelColorXY(i, j, ColorFromPalette(SEGPALETTE, map(j, 0, thisMax, 250, 0), 255, LINEARBLEND));
      SEGMENT.addPixelColorXY((cols - 1) - i, (rows - 1) - j, ColorFromPalette(SEGPALETTE, map(j, 0, thisMax, 250, 0), 255, LINEARBLEND));
    }
  }
  if (SEGMENT.check3) SEGMENT.blur(16, cols*rows < 100);

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__WAVERLY__INDEXING[] PROGMEM = "Waverly@Amplification,Sensitivity,,,,,Blur;;!;2v;ix=64,si=0"; // Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 2D GEQ By Will Tatam. Code reduction by Ewoud Wijma.
 * @note : Converted from WLED Effects "mode_2DGEQ"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_GED()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int NUM_BANDS = map(SEGMENT.custom1, 0, 255, 1, 16);
  const int cols = SEG_W;
  const int rows = SEG_H;

  if (!SEGMENT.allocateData(cols*sizeof(uint16_t))) return EFFECT_DEFAULT(); //allocation failed
  uint16_t *previousBarHeight = reinterpret_cast<uint16_t*>(SEGMENT.data); //array of previous bar heights per frequency band

  um_data_t *um_data = getAudioData();
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) for (int i=0; i<cols; i++) previousBarHeight[i] = 0;

  bool rippleTime = false;
  if (effect_start_time - SEGMENT.step >= (256U - SEGMENT.intensity)) {
    SEGMENT.step = effect_start_time;
    rippleTime = true;
  }

  int fadeoutDelay = (256 - SEGMENT.speed) / 64;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fadeToBlackBy(SEGMENT.speed);

  for (int x=0; x < cols; x++) {
    uint8_t  band       = map(x, 0, cols, 0, NUM_BANDS);
    if (NUM_BANDS < 16) band = map(band, 0, NUM_BANDS - 1, 0, 15); // always use full range. comment out this line to get the previous behaviour.
    band = constrain(band, 0, 15);
    unsigned colorIndex = band * 17;
    int barHeight  = map(fftResult[band], 0, 255, 0, rows); // do not subtract -1 from rows here
    if (barHeight > previousBarHeight[x]) previousBarHeight[x] = barHeight; //drive the peak up

    uint32_t ledColor = BLACK;
    for (int y=0; y < barHeight; y++) {
      if (SEGMENT.check1) //color_vertical / color bars toggle
        colorIndex = map(y, 0, rows-1, 0, 255);

      ledColor = SEGMENT.color_from_palette(colorIndex, false, PALETTE_SOLID_WRAP, 0);
      SEGMENT.setPixelColorXY(x, rows-1 - y, ledColor);
    }
    if (previousBarHeight[x] > 0)
      SEGMENT.setPixelColorXY(x, rows - previousBarHeight[x], (SEGCOLOR(2) != BLACK) ? SEGCOLOR(2) : ledColor);

    if (rippleTime && previousBarHeight[x]>0) previousBarHeight[x]--;    //delay/ripple effect
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_GED__INDEXING[] PROGMEM = "GEQ@Fade speed,Ripple decay,# of bands,,,Color bars;!,,Peaks;!;2f;c1=255,c2=64,pal=11,si=0"; // Beatsin


/*******************************************************************************************************************************************************************************************************************
 * @description : 2D Funky plank
 * @note : Converted from WLED Effects "mode_2DFunkyPlank"
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_FunkyPlank()
{
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  int NUMB_BANDS = map(SEGMENT.custom1, 0, 255, 1, 16);
  int barWidth = (cols / NUMB_BANDS);
  int bandInc = 1;
  if (barWidth == 0) {
    // Matrix narrower than fft bands
    barWidth = 1;
    bandInc = (NUMB_BANDS / cols);
  }

  um_data_t *um_data = getAudioData();
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500+1 % 64;
  if (SEGMENT.aux0 != secondHand) {                        // Triggered millis timing.
    SEGMENT.aux0 = secondHand;

    // display values of
    int b = 0;
    for (int band = 0; band < NUMB_BANDS; band += bandInc, b++) {
      int hue = fftResult[band % 16];
      int v = map(fftResult[band % 16], 0, 255, 10, 255);
      for (int w = 0; w < barWidth; w++) {
         int xpos = (barWidth * b) + w;
         SEGMENT.setPixelColorXY_CRGB(xpos, 0, CHSV(hue, 255, v));
      }
    }

    // Update the display:
    for (int i = (rows - 1); i > 0; i--) {
      for (int j = (cols - 1); j >= 0; j--) {
        SEGMENT.setPixelColorXY(j, i, SEGMENT.getPixelColorXY(j, i-1));
      }
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__INDEXING[] PROGMEM = "Funky Plank@Scroll speed,,# of bands;;;2f;si=0"; // Beatsin

/*******************************************************************************************************************************************************************************************************************
 * @description : 2D Akemi
 * @note : Converted from WLED Effects "mode_2DAkemi"
 ********************************************************************************************************************************************************************************************************************/
static uint8_t akemi[] PROGMEM = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,2,2,3,3,3,3,3,3,2,2,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,2,3,3,0,0,0,0,0,0,3,3,2,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,2,3,0,0,0,6,5,5,4,0,0,0,3,2,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,2,3,0,0,6,6,5,5,5,5,4,4,0,0,3,2,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,2,3,0,6,5,5,5,5,5,5,5,5,4,0,3,2,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,2,3,0,6,5,5,5,5,5,5,5,5,5,5,4,0,3,2,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,3,2,0,6,5,5,5,5,5,5,5,5,5,5,4,0,2,3,0,0,0,0,0,0,0,
  0,0,0,0,0,0,3,2,3,6,5,5,7,7,5,5,5,5,7,7,5,5,4,3,2,3,0,0,0,0,0,0,
  0,0,0,0,0,2,3,1,3,6,5,1,7,7,7,5,5,1,7,7,7,5,4,3,1,3,2,0,0,0,0,0,
  0,0,0,0,0,8,3,1,3,6,5,1,7,7,7,5,5,1,7,7,7,5,4,3,1,3,8,0,0,0,0,0,
  0,0,0,0,0,8,3,1,3,6,5,5,1,1,5,5,5,5,1,1,5,5,4,3,1,3,8,0,0,0,0,0,
  0,0,0,0,0,2,3,1,3,6,5,5,5,5,5,5,5,5,5,5,5,5,4,3,1,3,2,0,0,0,0,0,
  0,0,0,0,0,0,3,2,3,6,5,5,5,5,5,5,5,5,5,5,5,5,4,3,2,3,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,6,5,5,5,5,5,7,7,5,5,5,5,5,4,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,6,5,5,5,5,5,5,5,5,5,5,5,5,4,0,0,0,0,0,0,0,0,0,
  1,0,0,0,0,0,0,0,0,6,5,5,5,5,5,5,5,5,5,5,5,5,4,0,0,0,0,0,0,0,0,2,
  0,2,2,2,0,0,0,0,0,6,5,5,5,5,5,5,5,5,5,5,5,5,4,0,0,0,0,0,2,2,2,0,
  0,0,0,3,2,0,0,0,6,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,2,2,0,0,0,
  0,0,0,3,2,0,0,0,6,5,5,5,5,5,5,5,5,5,5,5,5,5,5,4,0,0,0,2,3,0,0,0,
  0,0,0,0,3,2,0,0,0,0,3,3,0,3,3,0,0,3,3,0,3,3,0,0,0,0,2,2,0,0,0,0,
  0,0,0,0,3,2,0,0,0,0,3,2,0,3,2,0,0,3,2,0,3,2,0,0,0,0,2,3,0,0,0,0,
  0,0,0,0,0,3,2,0,0,3,2,0,0,3,2,0,0,3,2,0,0,3,2,0,0,2,3,0,0,0,0,0,
  0,0,0,0,0,3,2,2,2,2,0,0,0,3,2,0,0,3,2,0,0,0,3,2,2,2,3,0,0,0,0,0,
  0,0,0,0,0,0,3,3,3,0,0,0,0,3,2,0,0,3,2,0,0,0,0,3,3,3,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

uint16_t mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_Akemi()
{
  
  if (!isMatrix || !SEGMENT.is2D()) return EFFECT_DEFAULT(); // not a 2D set-up

  const int cols = SEG_W;
  const int rows = SEG_H;

  unsigned counter = (effect_start_time * ((SEGMENT.speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  const float lightFactor  = 0.15f;
  const float normalFactor = 0.4f;

  um_data_t *um_data = getAudioData();
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];
  float base = fftResult[0]/255.0f;

  //draw and color Akemi
  for (int y=0; y < rows; y++) for (int x=0; x < cols; x++) {
    CRGB color;
    CRGB soundColor = CRGB::Orange;
    CRGB faceColor  = CRGB(SEGMENT.color_wheel(counter));
    CRGB armsAndLegsColor = CRGB(SEGCOLOR(1) > 0 ? SEGCOLOR(1) : 0xFFE0A0); //default warmish white 0xABA8FF; //0xFF52e5;//
    uint8_t ak = pgm_read_byte_near(akemi + ((y * 32)/rows) * 32 + (x * 32)/cols); // akemi[(y * 32)/rows][(x * 32)/cols]
    switch (ak) {
      case 3: armsAndLegsColor.r *= lightFactor;  armsAndLegsColor.g *= lightFactor;  armsAndLegsColor.b *= lightFactor;  color = armsAndLegsColor; break; //light arms and legs 0x9B9B9B
      case 2: armsAndLegsColor.r *= normalFactor; armsAndLegsColor.g *= normalFactor; armsAndLegsColor.b *= normalFactor; color = armsAndLegsColor; break; //normal arms and legs 0x888888
      case 1: color = armsAndLegsColor; break; //dark arms and legs 0x686868
      case 6: faceColor.r *= lightFactor;  faceColor.g *= lightFactor;  faceColor.b *= lightFactor;  color=faceColor; break; //light face 0x31AAFF
      case 5: faceColor.r *= normalFactor; faceColor.g *= normalFactor; faceColor.b *= normalFactor; color=faceColor; break; //normal face 0x0094FF
      case 4: color = faceColor; break; //dark face 0x007DC6
      case 7: color = SEGCOLOR(2) > 0 ? SEGCOLOR(2) : 0xFFFFFF; break; //eyes and mouth default white
      case 8: if (base > 0.4) {soundColor.r *= base; soundColor.g *= base; soundColor.b *= base; color=soundColor;} else color = armsAndLegsColor; break;
      default: color = BLACK; break;
    }

    if (SEGMENT.intensity > 128 && fftResult && fftResult[0] > 128) { //dance if base is high
      SEGMENT.setPixelColorXY(x, 0, BLACK);
      SEGMENT.setPixelColorXY(x, y+1, color);
    } else
      SEGMENT.setPixelColorXY(x, y, color);
  }

  //add geq left and right
  if (um_data && fftResult) {
    int xMax = cols/8;
    for (int x=0; x < xMax; x++) {
      unsigned band = map(x, 0, max(xMax,4), 0, 15);  // map 0..cols/8 to 16 GEQ bands
      band = constrain(band, 0, 15);
      int barHeight = map(fftResult[band], 0, 255, 0, 17*rows/32);
      uint32_t color = SEGMENT.color_from_palette((band * 35), false, PALETTE_SOLID_WRAP, 0);

      for (int y=0; y < barHeight; y++) {
        SEGMENT.setPixelColorXY(x, rows/2-y, color);
        SEGMENT.setPixelColorXY(cols-1-x, rows/2-y, color);
      }
    }
  }

  return FRAMETIME;
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_AKEMI__INDEXING[] PROGMEM = "Akemi@Color speed,Dance;Head palette,Arms & Legs,Eyes & Mouth;Face palette;2f;si=0"; //beatsin


#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D END SECTION /////////////////////////////////////////////////////////////////////////////

/**
 * @brief 
 * 
 * Effect Ideas:
 * 
 * (1) Wipe Random from the top, that fills pixel by pixel until it reaches an end stop on the bottom. This end may jump in single pixels, or in multiples (ie grouping=200). The effect would be banded falling tetris blocks and when the banding is complete, restart
 *     - Using a background colour, either black or white for the "unfilled" part will give nice effects
 * (2) A vertical strip shows the sky as horizon to top. Hence, instead of the moving gradient, 16 key points on it will be set and change depending on the time of day to reflect the darkening sunset.
 * 
 * All effects below are in order, and should over time be added (uncommented) so they build
 * 
 * 
 */
void mAnimatorLight::LoadEffects()
{
  // General Level 1 Minimal Home Effects
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__SOLID_COLOUR__ID,
            &mAnimatorLight::EffectAnim__Solid_Colour,
            PM_EFFECT_CONFIG__SOLID_COLOUR,
            Effect_DevStage::Release);

  addEffect(EFFECTS_FUNCTION__STATIC_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Static_Palette,
            PM_EFFECT_CONFIG__STATIC_PALETTE,
            Effect_DevStage::Release);

  addEffect(EFFECTS_FUNCTION__SPLIT_PALETTE_SEGWIDTH__ID,
            &mAnimatorLight::EffectAnim__Split_Palette_SegWidth,
            PM_EFFECT_CONFIG__SPLIT_PALETTE_SEGWIDTH,
            Effect_DevStage::Alpha);

  addEffect(EFFECTS_FUNCTION__RANDOMISE_GRADIENT_PALETTE_SEGWIDTH__ID,
            &mAnimatorLight::EffectAnim__Randomise_Gradient_Palette_SegWidth,
            PM_EFFECT_CONFIG__RANDOMISE_GRADIENT_PALETTE_SEGWIDTH,
            Effect_DevStage::Alpha);

  addEffect(EFFECTS_FUNCTION__GRADIENT_PALETTE_SEGWIDTH__ID,
            &mAnimatorLight::EffectAnim__Gradient_Palette_SegWidth,
            PM_EFFECT_CONFIG__GRADIENT_PALETTE_SEGWIDTH);

  addEffect(EFFECTS_FUNCTION__FIREFLY__ID,
            &mAnimatorLight::EffectAnim__Firefly,
            PM_EFFECT_CONFIG__FIREFLY,
            Effect_DevStage::Release);

  addEffect(EFFECTS_FUNCTION__CANDLE_SINGLE__ID,
            &mAnimatorLight::EffectAnim__Candle_Single,
            PM_EFFECT_CONFIG__CANDLE_SINGLE,
            Effect_DevStage::Beta);

  addEffect(EFFECTS_FUNCTION__CANDLE_MULTIPLE__ID,
            &mAnimatorLight::EffectAnim__Candle_Multiple,
            PM_EFFECT_CONFIG__CANDLE_MULTIPLE,
            Effect_DevStage::Beta);
            
  addEffect(EFFECTS_FUNCTION__STATIC_PALETTE_VARIED__ID,
            &mAnimatorLight::EffectAnim__Static_Palette_Varied,
            PM_EFFECT_CONFIG__STATIC_PALETTE_VARIED,
            Effect_DevStage::Dev);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

  // General Level 2 Flashing Basic Effects
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__SHIMMERING_PALETTE_DOUBLE__ID,
            &mAnimatorLight::EffectAnim__Shimmering_Two_Palette,
            PM_EFFECT_CONFIG__SHIMMERING_TWO_PALETTES,
            Effect_DevStage::Dev);

  addEffect(EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__ID,
            &mAnimatorLight::EffectAnim__Shimmering_Palette_Saturation,
            PM_EFFECT_CONFIG__SHIMMERING_PALETTE_SATURATION);

  addEffect(EFFECTS_FUNCTION__ROTATING_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Rotating_Palette,
            PM_EFFECT_CONFIG__ROTATING_PALETTE,
            Effect_DevStage::Alpha);

  addEffect(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,
            &mAnimatorLight::EffectAnim__Rotating_Previous_Animation,
            PM_EFFECT_CONFIG__ROTATING_PREVIOUS_ANIMATION,
            Effect_DevStage::Alpha);

  addEffect(EFFECTS_FUNCTION__STEPPING_PALETTE_WITH_BACKGROUND__ID,
            &mAnimatorLight::EffectAnim__Stepping_Palette_With_Background,
            PM_EFFECT_CONFIG__STEPPING_PALETTE_WITH_BACKGROUND);

  addEffect(EFFECTS_FUNCTION__STEPPING_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Stepping_Palette,
            PM_EFFECT_CONFIG__STEPPING_PALETTE);

  addEffect(EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Blend_Two_Palettes,
            PM_EFFECT_CONFIG__BLEND_TWO_PALETTES);

  addEffect(EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Palette_Onto_Palette,
            PM_EFFECT_CONFIG__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI);

  addEffect(EFFECTS_FUNCTION__TWINKLE_OFF_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Out_Palette,
            PM_EFFECT_CONFIG__TWINKLE_OUT_PALETTE);
            
  addEffect(EFFECTS_FUNCTION__TIMEBASED__HOUR_PROGRESS__ID,
            &mAnimatorLight::EffectAnim__TimeBased__HourProgress,
            PM_EFFECT_CONFIG__TIMEBASED__HOUR_PROGRESS);

  addEffect(EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Decaying_Palette,
            PM_EFFECT_CONFIG__TWINKLE_DECAYING_PALETTE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

  // General Level 3 Flashing Extended Effects
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_BLACK__ID,
            &mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_Black,
            PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_BLACK,
            Effect_DevStage::Unstable);

  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_BLACK__ID,
            &mAnimatorLight::EffectAnim__Popping_Decay_Random_To_Black,
            PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_BLACK);

  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_WHITE__ID,
            &mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_White,
            PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_WHITE);

  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_WHITE__ID,
            &mAnimatorLight::EffectAnim__Popping_Decay_Random_To_White,
            PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_WHITE);

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

  // General Level 4 Flashing Complete Effects
  
  addEffect(EFFECTS_FUNCTION__PALETTE_LIT_PATTERN__ID,
            &mAnimatorLight::EffectAnim__Palette_Lit_Pattern,
            PM_EFFECT_CONFIG__PALETTE_LIT_PATTERN);

  addEffect(EFFECTS_FUNCTION__TRISEGCOL_LIT_PATTERN__ID,
            &mAnimatorLight::EffectAnim__TriSegCol_Lit_Pattern,
            PM_EFFECT_CONFIG__TRISEGCOL_LIT_PATTERN);

  addEffect(EFFECTS_FUNCTION__PALETTES_INTERLEAVED_LIT_PATTERN__ID,
            &mAnimatorLight::EffectAnim__Palettes_Interleaved_Lit_Pattern,
            PM_EFFECT_CONFIG__PALETTES_INTERLEAVED_LIT_PATTERN);

  addEffect(EFFECTS_FUNCTION__PALETTES_INTERLEAVED__ID,
            &mAnimatorLight::EffectAnim__Palettes_Interleaved,
            PM_EFFECT_CONFIG__PALETTES_INTERLEAVED);

  addEffect(EFFECTS_FUNCTION__SPOTS__ID,
            &mAnimatorLight::EffectAnim__Spots,
            PM_EFFECT_CONFIG__SPOTS);

  addEffect(EFFECTS_FUNCTION__PERCENT__ID,
            &mAnimatorLight::EffectAnim__Percent,
            PM_EFFECT_CONFIG__PERCENT);

  addEffect(EFFECTS_FUNCTION__RANDOM_COLOR__ID,         
            &mAnimatorLight::EffectAnim__Random_Colour,                         
            PM_EFFECT_CONFIG__RANDOM_COLOR);

  addEffect(EFFECTS_FUNCTION__TRICOLOR_WIPE__ID,
            &mAnimatorLight::EffectAnim__TriColour_Wipe,
            PM_EFFECT_CONFIG__TRICOLOR_WIPE);
  /**
   * Wipe/Sweep/Runners 
   **/
  addEffect(EFFECTS_FUNCTION__COLOR_WIPE__ID,
            &mAnimatorLight::EffectAnim__Colour_Wipe,
            PM_EFFECT_CONFIG__COLOR_WIPE);

  addEffect(EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID,
            &mAnimatorLight::EffectAnim__Colour_Wipe_Random,
            PM_EFFECT_CONFIG__COLOR_WIPE_RANDOM);

  addEffect(EFFECTS_FUNCTION__COLOR_WIPE_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Colour_Wipe_Palette,
            PM_EFFECT_CONFIG__COLOR_WIPE_PALETTE);

  addEffect(EFFECTS_FUNCTION__COLOR_SWEEP__ID,
            &mAnimatorLight::EffectAnim__Colour_Sweep,
            PM_EFFECT_CONFIG__COLOR_SWEEP);

  addEffect(EFFECTS_FUNCTION__COLOR_SWEEP_RANDOM__ID,
            &mAnimatorLight::EffectAnim__Colour_Sweep_Random,
            PM_EFFECT_CONFIG__COLOR_SWEEP_RANDOM);

  addEffect(EFFECTS_FUNCTION__COLOR_SWEEP_PALETTE__ID,
            &mAnimatorLight::EffectAnim__Colour_Sweep_Palette,
            PM_EFFECT_CONFIG__COLOR_SWEEP_PALETTE);

  addEffect(EFFECTS_FUNCTION__DYNAMIC__ID,
            &mAnimatorLight::EffectAnim__Dynamic,
            PM_EFFECT_CONFIG__DYNAMIC);

  addEffect(EFFECTS_FUNCTION__DYNAMIC_SMOOTH__ID,
            &mAnimatorLight::EffectAnim__Dynamic_Smooth,
            PM_EFFECT_CONFIG__DYNAMIC_SMOOTH);
            
  addEffect(EFFECTS_FUNCTION__RUNNING_COLOR__ID,
            &mAnimatorLight::EffectAnim__Running_Colour,
            PM_EFFECT_CONFIG__RUNNING_COLOR);

  addEffect(EFFECTS_FUNCTION__RUNNING_RANDOM__ID,
            &mAnimatorLight::EffectAnim__Running_Random,
            PM_EFFECT_CONFIG__RUNNING_RANDOM);
  
  addEffect(EFFECTS_FUNCTION__ANDROID__ID,               
            &mAnimatorLight::EffectAnim__Android,            
            PM_EFFECT_CONFIG__ANDROID);  

  addEffect(EFFECTS_FUNCTION__GRADIENT__ID,
            &mAnimatorLight::EffectAnim__Gradient,
            PM_EFFECT_CONFIG__GRADIENT);

  addEffect(EFFECTS_FUNCTION__LOADING__ID,
            &mAnimatorLight::EffectAnim__Loading,
            PM_EFFECT_CONFIG__LOADING);

  addEffect(EFFECTS_FUNCTION__ROLLINGBALLS__ID,
            &mAnimatorLight::EffectAnim__Rolling_Balls,
            PM_EFFECT_CONFIG__ROLLINGBALLS);

  addEffect(EFFECTS_FUNCTION__POLICE__ID,
            &mAnimatorLight::EffectAnim__Police,
            PM_EFFECT_CONFIG__POLICE);

  addEffect(EFFECTS_FUNCTION__POLICE_ALL__ID,
            &mAnimatorLight::EffectAnim__Police_All,
            PM_EFFECT_CONFIG__POLICE_ALL);

  addEffect(EFFECTS_FUNCTION__FAIRY__ID,
            &mAnimatorLight::EffectAnim__Fairy,
            PM_EFFECT_CONFIG__FAIRY);

  addEffect(EFFECTS_FUNCTION__TWO_DOTS__ID,
            &mAnimatorLight::EffectAnim__Two_Dots,
            PM_EFFECT_CONFIG__TWO_DOTS);

  addEffect(EFFECTS_FUNCTION__TWO_AREAS__ID,
            &mAnimatorLight::EffectAnim__Two_Areas,
            PM_EFFECT_CONFIG__TWO_AREAS);
            
  addEffect(EFFECTS_FUNCTION__FAIRYTWINKLE__ID,
            &mAnimatorLight::EffectAnim__Fairy_Twinkle,
            PM_EFFECT_CONFIG__FAIRYTWINKLE);
            
  addEffect(EFFECTS_FUNCTION__RUNNING_DUAL__ID,
            &mAnimatorLight::EffectAnim__Running_Dual,
            PM_EFFECT_CONFIG__RUNNING_DUAL);            

  addEffect(EFFECTS_FUNCTION__MULTI_COMET__ID,
            &mAnimatorLight::EffectAnim__Multi_Comet,
            PM_EFFECT_CONFIG__MULTI_COMET);

  addEffect(EFFECTS_FUNCTION__OSCILLATE__ID,
            &mAnimatorLight::EffectAnim__Oscillate,
            PM_EFFECT_CONFIG__OSCILLATE);

  addEffect(EFFECTS_FUNCTION__BPM__ID,
            &mAnimatorLight::EffectAnim__BPM,
            PM_EFFECT_CONFIG__BPM);

  addEffect(EFFECTS_FUNCTION__JUGGLE__ID,
            &mAnimatorLight::EffectAnim__Juggle,
            PM_EFFECT_CONFIG__JUGGLES);

  addEffect(EFFECTS_FUNCTION__PALETTE__ID,
            &mAnimatorLight::EffectAnim__Palette,
            PM_EFFECT_CONFIG__PALETTE);

  addEffect(EFFECTS_FUNCTION__COLORWAVES__ID,
            &mAnimatorLight::EffectAnim__ColourWaves,
            PM_EFFECT_CONFIG__COLORWAVES);

  addEffect(EFFECTS_FUNCTION__LAKE__ID,
            &mAnimatorLight::EffectAnim__Lake,
            PM_EFFECT_CONFIG__LAKE);

  addEffect(EFFECTS_FUNCTION__GLITTER__ID,
            &mAnimatorLight::EffectAnim__Glitter,
            PM_EFFECT_CONFIG__GLITTER);

  addEffect(EFFECTS_FUNCTION__METEOR__ID,
            &mAnimatorLight::EffectAnim__Meteor,
            PM_EFFECT_CONFIG__METEOR);

  addEffect(EFFECTS_FUNCTION__PRIDE_2015__ID,
            &mAnimatorLight::EffectAnim__Pride_2015,
            PM_EFFECT_CONFIG__PRIDE_2015);

  addEffect(EFFECTS_FUNCTION__PACIFICA__ID,
            &mAnimatorLight::EffectAnim__Pacifica,
            PM_EFFECT_CONFIG__PACIFICA);

  addEffect(EFFECTS_FUNCTION__SUNRISE__ID,
            &mAnimatorLight::EffectAnim__Sunrise,
            PM_EFFECT_CONFIG__SUNRISE);

  addEffect(EFFECTS_FUNCTION__SINEWAVE__ID,
            &mAnimatorLight::EffectAnim__Sinewave,
            PM_EFFECT_CONFIG__SINEWAVE);

  addEffect(EFFECTS_FUNCTION__FLOW__ID,
            &mAnimatorLight::EffectAnim__Flow,
            PM_EFFECT_CONFIG__FLOW);

  addEffect(EFFECTS_FUNCTION__RUNNING_LIGHTS__ID,
            &mAnimatorLight::EffectAnim__Running_Lights,
            PM_EFFECT_CONFIG__RUNNING_LIGHTS);

  addEffect(EFFECTS_FUNCTION__RAINBOW_CYCLE__ID,
            &mAnimatorLight::EffectAnim__Rainbow_Cycle,
            PM_EFFECT_CONFIG__RAINBOW_CYCLE);

  /**
   * Chase
   **/
  addEffect(EFFECTS_FUNCTION__CHASE_COLOR__ID,
            &mAnimatorLight::EffectAnim__Chase_Colour,
            PM_EFFECT_CONFIG__CHASE_COLOR);

  addEffect(EFFECTS_FUNCTION__CHASE_RANDOM__ID,
            &mAnimatorLight::EffectAnim__Chase_Random,
            PM_EFFECT_CONFIG__CHASE_RANDOM);

  addEffect(EFFECTS_FUNCTION__CHASE_RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Chase_Rainbow,
            PM_EFFECT_CONFIG__CHASE_RAINBOW);

  addEffect(EFFECTS_FUNCTION__CHASE_FLASH__ID,
            &mAnimatorLight::EffectAnim__Chase_Flash,
            PM_EFFECT_CONFIG__CHASE_FLASH);

  addEffect(EFFECTS_FUNCTION__CHASE_FLASH_RANDOM__ID,
            &mAnimatorLight::EffectAnim__Chase_Flash_Random,
            PM_EFFECT_CONFIG__CHASE_FLASH_RANDOM);

  addEffect(EFFECTS_FUNCTION__CHASE_RAINBOW_WHITE__ID,
            &mAnimatorLight::EffectAnim__Chase_Rainbow_White,
            PM_EFFECT_CONFIG__CHASE_RAINBOW_WHITE);

  addEffect(EFFECTS_FUNCTION__CHASE_THEATER__ID,
            &mAnimatorLight::EffectAnim__Chase_Theater,
            PM_EFFECT_CONFIG__THEATER_CHASE);

  addEffect(EFFECTS_FUNCTION__CHASE_THEATER_RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Chase_Theatre_Rainbow,
            PM_EFFECT_CONFIG__THEATER_CHASE_RAINBOW);

  addEffect(EFFECTS_FUNCTION__CHASE_TRICOLOR__ID,
            &mAnimatorLight::EffectAnim__Chase_TriColour,
            PM_EFFECT_CONFIG__TRICOLOR_CHASE);

  /**
   *  Breathe/Fade/Pulse
   **/      
  addEffect(EFFECTS_FUNCTION__BREATH__ID,
            &mAnimatorLight::EffectAnim__Breath,
            PM_EFFECT_CONFIG__BREATH);

  addEffect(EFFECTS_FUNCTION__FADE__ID,
            &mAnimatorLight::EffectAnim__Fade,
            PM_EFFECT_CONFIG__FADE);

  addEffect(EFFECTS_FUNCTION__FADE_TRICOLOR__ID,
            &mAnimatorLight::EffectAnim__Fade_TriColour,
            PM_EFFECT_CONFIG__TRICOLOR_FADE);

  addEffect(EFFECTS_FUNCTION__FADE_SPOTS__ID,
            &mAnimatorLight::EffectAnim__Fade_Spots,
            PM_EFFECT_CONFIG__SPOTS_FADE);
  
  /**
   * Sparkle/Twinkle
   **/
  addEffect(EFFECTS_FUNCTION__SOLID_GLITTER__ID,
            &mAnimatorLight::EffectAnim__Solid_Glitter,
            PM_EFFECT_CONFIG__SOLID_GLITTER);

  addEffect(EFFECTS_FUNCTION__POPCORN__ID,
            &mAnimatorLight::EffectAnim__Popcorn,
            PM_EFFECT_CONFIG__POPCORN);

  addEffect(EFFECTS_FUNCTION__PLASMA__ID,
            &mAnimatorLight::EffectAnim__Plasma,
            PM_EFFECT_CONFIG__PLASMA);

  addEffect(EFFECTS_FUNCTION__SPARKLE__ID,
            &mAnimatorLight::EffectAnim__Sparkle,
            PM_EFFECT_CONFIG__SPARKLE);

  addEffect(EFFECTS_FUNCTION__FLASH_SPARKLE__ID,
            &mAnimatorLight::EffectAnim__Sparkle_Flash,
            PM_EFFECT_CONFIG__FLASH_SPARKLE);

  addEffect(EFFECTS_FUNCTION__HYPER_SPARKLE__ID,
            &mAnimatorLight::EffectAnim__Sparkle_Hyper,
            PM_EFFECT_CONFIG__HYPER_SPARKLE);

  addEffect(EFFECTS_FUNCTION__TWINKLE__ID,
            &mAnimatorLight::EffectAnim__Twinkle,
            PM_EFFECT_CONFIG__TWINKLE);

  addEffect(EFFECTS_FUNCTION__COLORTWINKLE__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Colour,
            PM_EFFECT_CONFIG__COLORTWINKLE);

  addEffect(EFFECTS_FUNCTION__TWINKLE_FOX__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Fox,
            PM_EFFECT_CONFIG__TWINKLE_FOX);

  addEffect(EFFECTS_FUNCTION__TWINKLE_CAT__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Cat,
            PM_EFFECT_CONFIG__TWINKLE_CAT);

  addEffect(EFFECTS_FUNCTION__TWINKLE_UP__ID,
            &mAnimatorLight::EffectAnim__Twinkle_Up,
            PM_EFFECT_CONFIG__TWINKLE_UP);

  addEffect(EFFECTS_FUNCTION__HALLOWEEN_EYES__ID,
            &mAnimatorLight::EffectAnim__Halloween_Eyes,
            PM_EFFECT_CONFIG__HALLOWEEN_EYES);

  addEffect(EFFECTS_FUNCTION__SAW__ID,
            &mAnimatorLight::EffectAnim__Saw,
            PM_EFFECT_CONFIG__SAW);

  addEffect(EFFECTS_FUNCTION__DISSOLVE__ID,
            &mAnimatorLight::EffectAnim__Dissolve,
            PM_EFFECT_CONFIG__DISSOLVE);

  addEffect(EFFECTS_FUNCTION__DISSOLVE_RANDOM__ID,
            &mAnimatorLight::EffectAnim__Dissolve_Random,
            PM_EFFECT_CONFIG__DISSOLVE_RANDOM);

  addEffect(EFFECTS_FUNCTION__COLORFUL__ID,
            &mAnimatorLight::EffectAnim__ColourFul,
            PM_EFFECT_CONFIG__COLORFUL);

  addEffect(EFFECTS_FUNCTION__TRAFFIC_LIGHT__ID,
            &mAnimatorLight::EffectAnim__Traffic_Light,
            PM_EFFECT_CONFIG__TRAFFIC_LIGHT);

  /**
   * Fireworks
   **/
  addEffect(EFFECTS_FUNCTION__FIREWORKS__ID,                     
            &mAnimatorLight::EffectAnim__Fireworks,                    
            PM_EFFECT_CONFIG__FIREWORKS);

  addEffect(EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID,           
            &mAnimatorLight::EffectAnim__Exploding_Fireworks,          
            PM_EFFECT_CONFIG__EXPLODING_FIREWORKS);

  addEffect(EFFECTS_FUNCTION__FIREWORKS_STARBURST__ID,           
            &mAnimatorLight::EffectAnim__Fireworks_Starburst,          
            PM_EFFECT_CONFIG__STARBURST);

  addEffect(EFFECTS_FUNCTION__RAIN__ID,                          
            &mAnimatorLight::EffectAnim__Rain,                         
            PM_EFFECT_CONFIG__RAIN);

  addEffect(EFFECTS_FUNCTION__TETRIX__ID,                          
            &mAnimatorLight::EffectAnim__Tetrix,                         
            PM_EFFECT_CONFIG__TETRIX);

  addEffect(EFFECTS_FUNCTION__FIRE_FLICKER__ID,                          
            &mAnimatorLight::EffectAnim__Fire_Flicker,                         
            PM_EFFECT_CONFIG__FIRE_FLICKER);
  
  addEffect(EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID,     
            &mAnimatorLight::EffectAnim__Exploding_Fireworks_NoLaunch,    
            PM_EFFECT_CONFIG__EXPLODING_FIREWORKS_NOLAUNCH);

  /**
   * Blink/Strobe
   **/
  addEffect(EFFECTS_FUNCTION__BLINK__ID,
            &mAnimatorLight::EffectAnim__Blink,
            PM_EFFECT_CONFIG__BLINK);

  addEffect(EFFECTS_FUNCTION__BLINK_RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Blink_Rainbow,
            PM_EFFECT_CONFIG__BLINK_RAINBOW);

  addEffect(EFFECTS_FUNCTION__STROBE__ID,
            &mAnimatorLight::EffectAnim__Strobe,
            PM_EFFECT_CONFIG__STROBE);

  addEffect(EFFECTS_FUNCTION__MULTI_STROBE__ID,
            &mAnimatorLight::EffectAnim__Strobe_Multi,
            PM_EFFECT_CONFIG__MULTI_STROBE);

  addEffect(EFFECTS_FUNCTION__STROBE_RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Strobe_Rainbow,
            PM_EFFECT_CONFIG__STROBE_RAINBOW);

  addEffect(EFFECTS_FUNCTION__RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Rainbow,
            PM_EFFECT_CONFIG__RAINBOW);

  addEffect(EFFECTS_FUNCTION__LIGHTNING__ID,
            &mAnimatorLight::EffectAnim__Lightning,
            PM_EFFECT_CONFIG__LIGHTNING);

  addEffect(EFFECTS_FUNCTION__FIRE_2012__ID,
            &mAnimatorLight::EffectAnim__Fire_2012,
            PM_EFFECT_CONFIG__FIRE_2012);

  addEffect(EFFECTS_FUNCTION__RAILWAY__ID,
            &mAnimatorLight::EffectAnim__Railway,
            PM_EFFECT_CONFIG__RAILWAY);

  addEffect(EFFECTS_FUNCTION__HEARTBEAT__ID,
            &mAnimatorLight::EffectAnim__Heartbeat,
            PM_EFFECT_CONFIG__HEARTBEAT);

  /**
   * Noise
   **/
  addEffect(EFFECTS_FUNCTION__FILLNOISE8__ID,
            &mAnimatorLight::EffectAnim__FillNoise8,
            PM_EFFECT_CONFIG__FILLNOISE8);

  addEffect(EFFECTS_FUNCTION__NOISE16_1__ID,
            &mAnimatorLight::EffectAnim__Noise16_1,
            PM_EFFECT_CONFIG__NOISE16_1,
            Effect_DevStage::Release);

  addEffect(EFFECTS_FUNCTION__NOISE16_2__ID,
            &mAnimatorLight::EffectAnim__Noise16_2,
            PM_EFFECT_CONFIG__NOISE16_2);

  addEffect(EFFECTS_FUNCTION__NOISE16_3__ID,
            &mAnimatorLight::EffectAnim__Noise16_3,
            PM_EFFECT_CONFIG__NOISE16_3);

  addEffect(EFFECTS_FUNCTION__NOISE16_4__ID,
            &mAnimatorLight::EffectAnim__Noise16_4,
            PM_EFFECT_CONFIG__NOISE16_4);

  addEffect(EFFECTS_FUNCTION__NOISEPAL__ID,
            &mAnimatorLight::EffectAnim__Noise_Pal,
            PM_EFFECT_CONFIG__NOISEPAL);

  addEffect(EFFECTS_FUNCTION__PHASEDNOISE__ID,
            &mAnimatorLight::EffectAnim__PhasedNoise,
            PM_EFFECT_CONFIG__PHASEDNOISE);

  addEffect(EFFECTS_FUNCTION__PHASED__ID,
            &mAnimatorLight::EffectAnim__Phased,
            PM_EFFECT_CONFIG__PHASED);

  /**
   * Scan
   **/
  addEffect(EFFECTS_FUNCTION__SCAN__ID,
            &mAnimatorLight::EffectAnim__Scan,
            PM_EFFECT_CONFIG__SCAN);

  addEffect(EFFECTS_FUNCTION__DUAL_SCAN__ID,
            &mAnimatorLight::EffectAnim__Scan_Dual,
            PM_EFFECT_CONFIG__DUAL_SCAN);

  addEffect(EFFECTS_FUNCTION__LARSON_SCANNER__ID,
            &mAnimatorLight::EffectAnim__Larson_Scanner,
            PM_EFFECT_CONFIG__LARSON_SCANNER);

  addEffect(EFFECTS_FUNCTION__DUAL_LARSON_SCANNER__ID,
            &mAnimatorLight::EffectAnim__Larson_Scanner_Dual,
            PM_EFFECT_CONFIG__DUAL_LARSON_SCANNER);

  addEffect(EFFECTS_FUNCTION__ICU__ID,
            &mAnimatorLight::EffectAnim__ICU,
            PM_EFFECT_CONFIG__ICU);

  addEffect(EFFECTS_FUNCTION__RIPPLE__ID,
            &mAnimatorLight::EffectAnim__Ripple,
            PM_EFFECT_CONFIG__RIPPLE);

  addEffect(EFFECTS_FUNCTION__RIPPLE_RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Ripple_Rainbow,
            PM_EFFECT_CONFIG__RIPPLE_RAINBOW);

  addEffect(EFFECTS_FUNCTION__COMET__ID,
            &mAnimatorLight::EffectAnim__Comet,
            PM_EFFECT_CONFIG__COMET);

  addEffect(EFFECTS_FUNCTION__CHUNCHUN__ID,
            &mAnimatorLight::EffectAnim__Chunchun,
            PM_EFFECT_CONFIG__CHUNCHUN);

  addEffect(EFFECTS_FUNCTION__DANCING_SHADOWS__ID,
            &mAnimatorLight::EffectAnim__Dancing_Shadows,
            PM_EFFECT_CONFIG__DANCING_SHADOWS);

  addEffect(EFFECTS_FUNCTION__WASHING_MACHINE__ID,
            &mAnimatorLight::EffectAnim__Washing_Machine,
            PM_EFFECT_CONFIG__WASHING_MACHINE);

  addEffect(EFFECTS_FUNCTION__BLENDS__ID,
            &mAnimatorLight::EffectAnim__Blends,
            PM_EFFECT_CONFIG__BLENDS);

  addEffect(EFFECTS_FUNCTION__TV_SIMULATOR__ID,
            &mAnimatorLight::EffectAnim__TV_Simulator,
            PM_EFFECT_CONFIG__TV_SIMULATOR);

  addEffect(EFFECTS_FUNCTION__BOUNCINGBALLS__ID,
            &mAnimatorLight::EffectAnim__Bouncing_Balls,
            PM_EFFECT_CONFIG__BOUNCINGBALLS);

  addEffect(EFFECTS_FUNCTION__SINELON__ID,
            &mAnimatorLight::EffectAnim__Sinelon,
            PM_EFFECT_CONFIG__SINELON);

  addEffect(EFFECTS_FUNCTION__SINELON_DUAL__ID,
            &mAnimatorLight::EffectAnim__Sinelon_Dual,
            PM_EFFECT_CONFIG__SINELON_DUAL);

  addEffect(EFFECTS_FUNCTION__SINELON_RAINBOW__ID,
            &mAnimatorLight::EffectAnim__Sinelon_Rainbow,
            PM_EFFECT_CONFIG__SINELON_RAINBOW);

  addEffect(EFFECTS_FUNCTION__DRIP__ID,
            &mAnimatorLight::EffectAnim__Drip,
            PM_EFFECT_CONFIG__DRIP);
            
  addEffect(EFFECTS_FUNCTION__FLOWSTRIPE__ID,  
            &mAnimatorLight::EffectAnim__FlowStripe, 
            PM_EFFECT_CONFIG__FLOWSTRIPE);
            
  addEffect(EFFECTS_FUNCTION__WAVESINS__ID,  
            &mAnimatorLight::EffectAnim__WaveSins, 
            PM_EFFECT_CONFIG__WAVESINS);
  
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

  /**
   * Hardware Installation Helpers
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
  addEffect(EFFECTS_FUNCTION__HARDWARE__SHOW_BUS__ID,
            &mAnimatorLight::EffectAnim__Hardware__Show_Bus,
            PM_EFFECT_CONFIG__HARDWARE__SHOW_BUS);

  addEffect(EFFECTS_FUNCTION__HARDWARE__MANUAL_PIXEL_COUNTING__ID,
            &mAnimatorLight::EffectAnim__Hardware__Manual_Pixel_Counting,
            PM_EFFECT_CONFIG__HARDWARE__MANUAL_PIXEL_COUNTING);

  addEffect(EFFECTS_FUNCTION__HARDWARE__VIEW_PIXEL_RANGE__ID,
            &mAnimatorLight::EffectAnim__Hardware__View_Pixel_Range,
            PM_EFFECT_CONFIG__HARDWARE__VIEW_PIXEL_RANGE);

  addEffect(EFFECTS_FUNCTION__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING__ID,
            &mAnimatorLight::EffectAnim__Hardware__Light_Sensor_Pixel_Indexing,
            PM_EFFECT_CONFIG__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING);
  #endif

  /**
   * Sun Position
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__Sunrise_Alarm_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__SUNRISE_ALARM_01);

  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01);

  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_SUNSET_BLENDED_PALETTES_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__Sunset_Blended_Palettes_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__SUNSET_BLENDED_PALETTES_01);

  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Elevation_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_ELEVATION_01);

  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Azimuth_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_AZIMUTH_01);

  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01);

  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__ID,
            &mAnimatorLight::EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01,
            PM_EFFECT_CONFIG__SUNPOSITIONS__WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01);
  #endif

  /**
   * LED Segment Clock
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  addEffect(EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
            &mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_01,
            PM_EFFECT_CONFIG__7SEGMENTDISPLAY__CLOCKTIME_01);

  addEffect(EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
            &mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_02,
            PM_EFFECT_CONFIG__7SEGMENTDISPLAY__CLOCKTIME_02);

  addEffect(EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,
            &mAnimatorLight::EffectAnim__7SegmentDisplay__ManualNumber_01,
            PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALNUMBER_01);

  addEffect(EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID,
            &mAnimatorLight::EffectAnim__7SegmentDisplay__ManualString_01,
            PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALSTRING_01);
  #endif
  
  /**
   * Notifications
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  addEffect(EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID,
            &mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Static,
            PM_EFFECT_CONFIG__NOTIFICATION_STATIC);

  addEffect(EFFECTS_FUNCTION__NOTIFICATION_FADE__ID,
            &mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Fade,
            PM_EFFECT_CONFIG__NOTIFICATION_FADE);

  addEffect(EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
            &mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Blinking,
            PM_EFFECT_CONFIG__NOTIFICATION_BLINKING);

  addEffect(EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,
            &mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Pulsing,
            PM_EFFECT_CONFIG__NOTIFICATION_PULSING);
  #endif

  /**
   * Border/Frame/Edge Wallpapers
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
  addEffect(EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__ID,
            &mAnimatorLight::EffectAnim__BorderWallpaper__TwoColour_Gradient,
            PM_EFFECT_CONFIG__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT);

  addEffect(EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__ID,
            &mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Gradient,
            PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT);

  addEffect(EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__ID,
            &mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Solid,
            PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_SOLID);
  #endif

  /**
   * Manual Pixel: Keeping as legacy, but mode change to realtime will remove this
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
  addEffect(EFFECTS_FUNCTION__MANUAL__PIXEL_SET_ELSEWHERE__ID,
            &mAnimatorLight::EffectAnim__Manual__PixelSetElsewhere,
            PM_EFFECT_CONFIG__MANUAL__PIXEL_SET_ELSEWHERE__INDEXING);
  #endif

  /**
   * Christmas Multifunction Controller: Replication of vintage 8 function controllers
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER
  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__COMBINATION_ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);  
            
  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__IN_WAVES_ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);

  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SEQUENTIAL_ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);

  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLO_GLO_ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);

  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__CHASING_AND_FLASHING__ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);

  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLOW_FADE__ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);

  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__TWINKLE_AND_FLASH__ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);

  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__STEADY_ON__ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);
  #endif

  /**
   * 2D (No Audio)
   **/
  #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
  addEffect(EFFECTS_FUNCTION__2D__BLACK_HOLE__ID,        
            &mAnimatorLight::EffectAnim__2D__Blackhole,     
            PM_EFFECT_CONFIG__2D__BLACK_HOLE__INDEXING);

  addEffect(EFFECTS_FUNCTION__2D__COLOURED_BURSTS__ID,   
            &mAnimatorLight::EffectAnim__2D__ColouredBursts,     
            PM_EFFECT_CONFIG__2D__COLOURED_BURSTS__INDEXING);
  
  addEffect(EFFECTS_FUNCTION__2D__DNA__ID,        
            &mAnimatorLight::EffectAnim__2D__DNA,     
            PM_EFFECT_CONFIG__2D__DNA__INDEXING);

  addEffect(EFFECTS_FUNCTION__2D__DNA_SPIRAL__ID,        
            &mAnimatorLight::EffectAnim__2D__DNASpiral,     
            PM_EFFECT_CONFIG__2D__DNA_SPIRAL__INDEXING);
  
  addEffect(EFFECTS_FUNCTION__2D__DRIFT__ID,            
            &mAnimatorLight::EffectAnim__2D__Drift,         
            PM_EFFECT_CONFIG__2D__DRIFT__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__FIRE_NOISE__ID,       
            &mAnimatorLight::EffectAnim__2D__FireNoise,     
            PM_EFFECT_CONFIG__2D__FIRE_NOISE__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__FRIZZLES__ID,         
            &mAnimatorLight::EffectAnim__2D__Frizzles,      
            PM_EFFECT_CONFIG__2D__FRIZZLES__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__GAME_OF_LIFE__ID,     
            &mAnimatorLight::EffectAnim__2D__GameOfLife,    
            PM_EFFECT_CONFIG__2D__GAME_OF_LIFE__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__HIPNOTIC__ID,         
            &mAnimatorLight::EffectAnim__2D__Hipnotic,      
            PM_EFFECT_CONFIG__2D__HIPNOTIC__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__JULIA__ID,            
            &mAnimatorLight::EffectAnim__2D__Julia,         
            PM_EFFECT_CONFIG__2D__JULIA__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__LISSAJOUS__ID,        
            &mAnimatorLight::EffectAnim__2D__Lissajous,     
            PM_EFFECT_CONFIG__2D__LISSAJOUS__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__MATRIX__ID,           
            &mAnimatorLight::EffectAnim__2D__Matrix,        
            PM_EFFECT_CONFIG__2D__MATRIX__INDEXING);    
            
  addEffect(EFFECTS_FUNCTION__2D__METABALLS__ID,        
            &mAnimatorLight::EffectAnim__2D__Metaballs,     
            PM_EFFECT_CONFIG__2D__METABALLS__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__NOISE__ID,            
            &mAnimatorLight::EffectAnim__2D__Noise,         
            PM_EFFECT_CONFIG__2D__NOISE__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__PLASMA_BALL__ID,      
            &mAnimatorLight::EffectAnim__2D__PlasmaBall,    
            PM_EFFECT_CONFIG__2D__PLASMA_BALL__INDEXING);   
  
  addEffect(EFFECTS_FUNCTION__2D__POLAR_LIGHTS__ID,     
            &mAnimatorLight::EffectAnim__2D__PolarLights,   
            PM_EFFECT_CONFIG__2D__POLAR_LIGHTS__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__PULSER__ID,           
            &mAnimatorLight::EffectAnim__2D__Pulser,        
            PM_EFFECT_CONFIG__2D__PULSER__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__SIN_DOTS__ID,         
            &mAnimatorLight::EffectAnim__2D__SinDots,       
            PM_EFFECT_CONFIG__2D__SIN_DOTS__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__SQUARED_SWIRL__ID,    
            &mAnimatorLight::EffectAnim__2D__SqauredSwirl,  
            PM_EFFECT_CONFIG__2D__SQUARED_SWIRL__INDEXING);    
    
  addEffect(EFFECTS_FUNCTION__2D__SUN_RADIATION__ID,    
            &mAnimatorLight::EffectAnim__2D__SunRadiation,  
            PM_EFFECT_CONFIG__2D__SUN_RADIATION__INDEXING);    
    
  addEffect(EFFECTS_FUNCTION__2D__TARTAN__ID,           
            &mAnimatorLight::EffectAnim__2D__Tartan,        
            PM_EFFECT_CONFIG__2D__TARTAN__INDEXING);    
    
  addEffect(EFFECTS_FUNCTION__2D__SPACESHIPS__ID,       
            &mAnimatorLight::EffectAnim__2D__SpaceShips,    
            PM_EFFECT_CONFIG__2D__SPACESHIPS__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__CRAZY_BEES__ID,       
            &mAnimatorLight::EffectAnim__2D__CrazyBees,     
            PM_EFFECT_CONFIG__2D__CRAZYBEES); 
  
  addEffect(EFFECTS_FUNCTION__2D__GHOST_RIDER__ID,      
            &mAnimatorLight::EffectAnim__2D__GhostRider,    
            PM_EFFECT_CONFIG__2D__GHOST_RIDER__INDEXING);   
  
  addEffect(EFFECTS_FUNCTION__2D__FLOATING_BLOBS__ID,   
            &mAnimatorLight::EffectAnim__2D__FloatingBlobs, 
            PM_EFFECT_CONFIG__2D__FLOATING_BLOBS__INDEXING);      
    
  addEffect(EFFECTS_FUNCTION__2D__SCROLLING_TEXT__ID,   
            &mAnimatorLight::EffectAnim__2D__ScrollingText, 
            PM_EFFECT_CONFIG__2D__SCROLLING_TEXT__INDEXING);  

  addEffect(EFFECTS_FUNCTION__2D__DRIFT_ROSE__ID,       
            &mAnimatorLight::EffectAnim__2D__DriftRose,     
            PM_EFFECT_CONFIG__2D__DRIFT_ROSE__INDEXING);              
  
  addEffect(EFFECTS_FUNCTION__2D__DIGITAL_CLOCK__ID,   
            &mAnimatorLight::EffectAnim__2D__DigitalClock, 
            PM_EFFECT_CONFIG__2D__DIGITAL_CLOCK__INDEXING);  

  addEffect(EFFECTS_FUNCTION__2D__PLASMAROTOZOOM__ID,       
            &mAnimatorLight::EffectAnim__2D__PlasmaRotoZoom,    
            PM_EFFECT_CONFIG__2D__PLASMAROTOZOOM__INDEXING);    

  addEffect(EFFECTS_FUNCTION__2D__DISTORTION_WAVES__ID, 
            &mAnimatorLight::EffectAnim__2D__DistortionWaves, 
            PM_EFFECT_CONFIG__2D__DISTORTION_WAVES__INDEXING);    
  
  addEffect(EFFECTS_FUNCTION__2D__SOAP__ID,   
            &mAnimatorLight::EffectAnim__2D__Soap, 
            PM_EFFECT_CONFIG__2D__SOAP__INDEXING);
  
  addEffect(EFFECTS_FUNCTION__2D__OCTOPUS__ID,   
            &mAnimatorLight::EffectAnim__2D__Octopus, 
            PM_EFFECT_CONFIG__2D__OCTOPUS__INDEXING);  
    
  addEffect(EFFECTS_FUNCTION__2D__WAVING_CELL__ID,   
            &mAnimatorLight::EffectAnim__2D__WavingCell, 
            PM_EFFECT_CONFIG__2D__WAVING_CELL__INDEXING);    
    
  #endif


  /**
   * Audio Reactive 1D
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_RIPPLE_PEAK__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Ripple_Peak,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_RIPPLE_PEAK);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PERLINE_MOVE__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Perline_Move,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PERLINE_MOVE);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_AURORA__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Aurora,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_AURORA);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_CENTER__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravCenter,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_CENTER);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_CENTRIC__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravCentric,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_CENTRIC);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAVI_METER__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravMeter,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_METER);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_FREQ__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravFreq, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_GRAV_FREQ);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_JUGGLES__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Juggles,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_JUGGLES);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_MATRIPIX__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Matripix,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_MATRIPIX);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_MID_NOISE__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_MidNoise,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_MID_NOISE);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_FIRE__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseFire,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_NOISE_FIRE);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_METER__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseMeter,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_NOISE_METER);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PIXEL_WAVE__ID,
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_PixelWave,
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PIXEL_WAVE);
            
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PLASMOID__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Plasmoid, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PLASMOID);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PUDDLE_PEAK__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_PuddlePeak, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PUDDLE_PEAK);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PUDDLES__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Puddles, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PUDDLES);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PIXELS__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Pixels, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_PIXELS);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_BLURZ__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Blurz, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_BLURZ);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_ROCKTAVES__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_RockTaves, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_ROCKTAVES);

  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_DJ_LIGHT__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_DJLight, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_DJ_LIGHT);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_MAP__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqMap, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_MAP);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_MATRIX__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqMatrix, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_MATRIX);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_PIXELS__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqPixels, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_PIXELS);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_WAVE__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqWave, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_FREQ_WAVE);
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_MOVE__ID,  
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseMove, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_NOISE_MOVE);
    
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_WATERFALL__ID,   
            &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Waterfall, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_WATERFALL); 
  #endif

  /**
   * Christmas Musical
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER_WITH_MUSIC
  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01_ID,
            &mAnimatorLight::EffectAnim__Christmas_Musical__01,
            PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);
  #endif

  /**
   * Audio Reactive 2D
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__SWIRL__ID,   
            &mAnimatorLight::EffectAnim__AudioReactive__2D__Swirl, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__SWIRL__INDEXING); 
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__WAVERLY__ID,   
            &mAnimatorLight::EffectAnim__AudioReactive__2D__Waverly, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__WAVERLY__INDEXING);   
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_GED__ID,   
            &mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_GED, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_GED__INDEXING); 
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__ID,   
            &mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_FunkyPlank, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__INDEXING);  
  
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_AKEMI__ID,   
            &mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_Akemi, 
            PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_AKEMI__INDEXING);  
  #endif

  uint16_t effectCount = effects.function.size();
  uint16_t effects_in_header_length = EFFECTS_FUNCTION__LENGTH__ID;

  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "AnimatorLight: Effects %d/%d"),effectCount, effects_in_header_length);  

}


#endif //USE_MODULE_LIGHTS_ANIMATOR




/**
 * @brief 
 * Create effect like Luminance example, only use three colour method for it. Ie to replicate, the centre would be black. The linearbelnd from edge to centre is the thing that will change, using a sinwave so its nonlinear. will look good.
 * 
 */