/**
 * @file mAnimatorLight_Segments_Effects.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

// // color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
// color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON));     


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
//      - a , seperates slider controls (max 5) or color controls (max 3). Palette has only one value
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

	/** Between @ And first ; is the slider values
	 * Slider 0: Speed
	 * Slider 1: Intensity
	 * Slider 2: Custom 1
	 * Slider 3: Custom 2
	 * Slider 4: Custom 3
	 * Slider 5: Time Period (ie Cycle Time, previously rate_ms)
	 * Slider 6: Grouping 
	 * Checkbox 0: Option 1
	 * Checkbox 1: Option 2 = layered icon, for layering effects
	 * Checkbox 2: Option 3
	 * 
	 */

	/** Between the first ; and the second ; is the color values (! means default name, nothing means leave out, otherwise the name of the color)
	 * Colour Circle 0: !, blank, or name
	 * Colour Circle 1: !, blank, or name
	 * Colour Circle 0: !, blank, or name
	 * Colour Circle 2: !, blank, or name
	 * Colour Circle 3: !, blank, or name
	 */

	/** Between the second ; and third ; is the palette values
	 * Colour Circle 0: !, blank, or name
	 * Colour Circle 1: !, blank, or name
	 * Colour Circle 0: !, blank, or name
	 * Colour Circle 2: !, blank, or name
	 * Colour Circle 3: !, blank, or name
	 */

/*
static const char PM_EFFECT_CONFIG__##[] PROGMEM = 
  "Effect Name@Speed,Intensity,Custom1Star,Custom2Cog,Custom3Visibility,EffectPeriod,Grouping,Checkbox1Palette,Checkbox2Layers,Checkbox3Favourite;Fx,Bg,Custom,!,!;pal=21,etp=1000";


*/

// m12=0
// pal=10
#define EFFECT_CONFIG_DEFAULT_OPTION__PALETTE_INDEX_CTR "pal=95" // The default forced palette
// 01
// ix=16
// o2
// sx=64
// 12


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Solid Colour
 * @description:   : For full RGBCCT colour
 * 
 * @param Intensity: None
 * @param Speed    : None
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Solid_Colour()
{
  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2) ){ DEBUG_LINE_HERE; return; } // Pixel_Width * Two_Channels

  RgbcctColor desired_colour = SEGMENT.GetPaletteColour();
  desired_colour = RgbcctColor::ApplyBrightnesstoRgbcctColour( desired_colour, SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied() );

  if(SEGMENT.colour_type__used_in_effect_generate == ColourType::COLOUR_TYPE__RGBW__ID)
  {
    desired_colour.W1 = desired_colour.W2 = max(desired_colour.W1, desired_colour.W2); // to ensure we ignore the CCT_TempPercentage
  }

  RgbcctColor starting_colour = SEGMENT.GetPixelColor(0);
  
  SetTransitionColourBuffer_DesiredColour (SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type__used_in_effect_generate, desired_colour); 
  SetTransitionColourBuffer_StartingColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type__used_in_effect_generate, starting_colour);

  // ALOG_ERR( PSTR("startin_colour = %d,%d,%d,%d,%d"), starting_colour.R,starting_colour.G,starting_colour.B,starting_colour.CW,starting_colour.WW);
  // ALOG_ERR( PSTR("desired_colour = %d,%d,%d,%d,%d"), desired_colour.R,desired_colour.G,desired_colour.B,desired_colour.CW,desired_colour.WW);
 
  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SOLID_COLOUR[] PROGMEM = "Solid Colour@;!,!,!,!,!;pal=0,etp=3000";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Static Palette
 * @description:   : Palettes should be showed all as banded/descrete, regardless of type
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Static_Palette()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count

  DEBUG_LIGHTING__START_TIME_RECORDING(3)
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  DEBUG_LIGHTING__SAVE_TIME_RECORDING(3, lighting_time_critical_logging.dynamic_buffer__starting_colour);

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  DEBUG_LIGHTING__START_TIME_RECORDING(3)
  RgbcctColor colour;
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }
  DEBUG_LIGHTING__SAVE_TIME_RECORDING(3, lighting_time_critical_logging.dynamic_buffer__desired_colour);

  // ALOG_ERR( PSTR("GetPixelColor(0) = %d,%d,%d,%d,%d"), SEGMENT.GetPixelColor(0).R,SEGMENT.GetPixelColor(0).G,SEGMENT.GetPixelColor(0).B,SEGMENT.GetPixelColor(0).CW,SEGMENT.GetPixelColor(0).WW);

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__STATIC_PALETTE[] PROGMEM = "Static Palette@!,!,,,,!,!;C1,C2,C3,C4,C5;etp=1000";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Static Palette
 * @description:   : This should be the same as static palette, but includes "aging" of the colours, so making them all be very slight variations of each other. This may be a palette feature later
 * 
 * The way to get around these changing every time when I dont want them to, is instead (like rotation)
 * draw them once, then block the effect off until its reset, this will show will still output the same colours, but will not change them
 * 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Static_Palette_Vintage()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  if(SEGMENT.effect_anim_section == 0)
  {

    uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

    RgbcctColor colour = RgbcctColor();
    for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
    {
      colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
      colour.Variance(SEGMENT.intensity);
      SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
    }

    SEGMENT.effect_anim_section = 1; // To stop redraw unless effect is reset
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__STATIC_PALETTE_VINTAGE[] PROGMEM = "Static Palette Aged@,Colour Variance,Dim Random;!,!,!,!,!;etp=1000,ix=15";//,v0=0"; // param0 used for brightness dimmer variation is off by default, no change
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Spanned Static Palette
 * @description:   : Palettes should all span the full segment, regardless of type
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Spanned_Static_Palette()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {    
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SPANNED_PALETTE[] PROGMEM = "Spanned Palette@;!,!,!,!,!;etp=1000";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name            : Slow Glow 
 * @description:    : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param Intensity : 0-255 is how many should pixels should randomly change (0-255 scaled to 0-pixel_count)
 * @param Speed     : None
 * @param CycleTime : Period of time (ms) between updates
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
void mAnimatorLight::EffectAnim__Slow_Glow()
{
    // Preallocate data buffer, directly using virtual length
    if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength() )) { return; }

    uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

    // Compute how many pixels to update based on intensity
    uint16_t pixels_to_update = mapvalue(SEGMENT.intensity, 0, 255, 0, SEGMENT.virtualLength());

    uint16_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

    // If it's the first run, update all pixels
    if (SEGMENT.flags.animator_first_run) {
        pixels_to_update = SEGMENT.virtualLength();
    }

    DEBUG_LIGHTING__START_TIME_RECORDING(4);

    // Precompute all random values for pixel indices and color palette
    uint16_t random_pixel_indices[pixels_to_update];
    uint16_t random_palette_indices[pixels_to_update];

    for (uint16_t i = 0; i < pixels_to_update; i++) {
        random_pixel_indices[i] = (SEGMENT.flags.animator_first_run) ? i : random(0, SEGMENT.virtualLength());
        random_palette_indices[i] = random(0, pixels_in_map);
    }

    // Update all pixels
    for (uint16_t iter = 0; iter < pixels_to_update; iter++) {
        uint16_t pixel_index = random_pixel_indices[iter];
        uint16_t desired_pixel = random_palette_indices[iter];

        // Get the color from the palette (optimized by preloading the palette index)
        RgbcctColor colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);

        #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
        ALOG_DBM(LOG_LEVEL_TEST, 
            PSTR("New palettePixel=%d, pixel_index=v%d SL%d | SVL%d | DL%d, colour=%d,%d,%dT%d"), 
            desired_pixel, pixel_index, 
            SEGMENT.length(), SEGMENT.virtualLength(), SEGMENT.DataLength(),
            colour.R, colour.G, colour.B,
            SEGMENT.colour_type__used_in_effect_generate
        );
        #endif // ENABLE__DEBUG_POINT__ANIMATION_EFFECTS

        // Direct buffer access for updating the color, avoid function call overhead
        SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
    }

    DEBUG_LIGHTING__SAVE_TIME_RECORDING(4, lighting_time_critical_logging.dynamic_buffer__desired_colour);

    // Start time recording for the second section
    DEBUG_LIGHTING__START_TIME_RECORDING(3);
    // Directly update the buffer with starting colors
    DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
    DEBUG_LIGHTING__SAVE_TIME_RECORDING(3, lighting_time_critical_logging.dynamic_buffer__starting_colour);

    // Set up the animation callback function
    SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param) { 
        this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    });
}

static const char PM_EFFECT_CONFIG__SLOW_GLOW[] PROGMEM = "Slow Glow@Blend Speed,Pixels Changing,,,,Repeat Rate (ms);!,!,!,!,!;etp=5000,ix=50"; // 6 sliders (speed/intensity/custom1/custom2/custom3/cycle time) + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Candle Flicker Base Function
 * @description:   : Randomly changes colours of pixels, and blends to the new one
 *                   Values close to 100 (speed) produce 5Hz flicker, which looks very candle-y
 *                   Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
 *                   and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-cand
 * @note           : derived from WLED effects
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
/**
 * @description:   : Flickers pixels by the same amount towards black
 **/
void mAnimatorLight::EffectAnim__Candle_Single()
{
  EffectAnim__Flicker_Base(false, mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__CANDLE_SINGLE[] PROGMEM = "Candle@!,!;!,!;!;1;sx=96,ix=224,etp=23";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
/**
 * @description:   : Flickers by multiple levels towards black
 **/
void mAnimatorLight::EffectAnim__Candle_Multiple()
{
  EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__BLACK__ID);
}
static const char PM_EFFECT_CONFIG__CANDLE_MULTIPLE[] PROGMEM = "Candles@Speed,Intensity;!,!,!,!,!;ix=224,etp=23"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

/**
 * @description:   : Flickers by multiple levels towards black
 **/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Shimmering_Palette_Saturation()
{
  EffectAnim__Flicker_Base(true,  mPalette::PALETTELIST_STATIC_SINGLE_COLOUR__COLDWHITE__ID);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_PALETTE_SATURATION[] PROGMEM = "Shimmering Saturation@Speed,Intensity;!,!,!,!,!;"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

/**
 * @description:   : Flicker between primary and secondary palette
 * 
     * Desc: Animation, that fades from selected palette to anothor palette,
     *       The intensity of fade (closer to palette B) will depend on intensity value
     *       ie intensity of 255 means Palette A (primary) can fade into palette B (set by option)
     * 
     * Using maps, the transition could have an option to be nonlinear, ie
     * 
     * lower 10% and upper 10% are accurate
     * 10->90% is either capped, or squeezed near 10% and 90% marks. That way even with randomness, either palette 1 or 2 is mostly showed with less probably of "white out" in the middle 
     * 
     * 
     * 
     * 
 * */
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Shimmering_Two_Palette() // Also add another here (or really segcolour is also it) to flicker into a second palette!! this will require direct load of second palette
{
  
  EffectAnim__Flicker_Base(true, SEGMENT.params_user[0]);
}
static const char PM_EFFECT_CONFIG__SHIMMERING_TWO_PALETTES[] PROGMEM = "Shimmering Palettes@Speed,Intensity,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
/**
 * @description:   : Base function for flickering
 * */
// void mAnimatorLight::EffectAnim__Flicker_Base(bool use_multi, uint16_t flicker_palette_id )
// {

//   uint8_t pixels_in_palette = GetNumberOfColoursInPalette(SEGMENT.palette_id);

//   RgbcctColor colour_pri;
//   RgbcctColor colour_sec;
//   RgbcctColor colour_out;

//   if (use_multi)
//   {
//     uint16_t dataSize = (SEGLEN -1) * 3;
//     if(!SEGMENT.allocateData(dataSize) ){ return; }
//   }

//   // max. flicker range controlled by intensity
//   uint8_t valrange = SEGMENT.intensity;
//   uint8_t rndval = valrange >> 1; // divide by 2

//   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//   ALOG_DBG(PSTR("step=%d"),    SEGMENT.step);
//   ALOG_DBG(PSTR("valrange=%d"),valrange);
//   ALOG_DBG(PSTR("rndval=%d"),  rndval);
//   #endif

//   uint8_t pixel_palette_counter = 0;

//   // step (how much to move closer to target per frame) coarsely set by speed
//   uint8_t speedFactor = 4;
//   if (SEGMENT.speed > 252) { // epilepsy
//     speedFactor = 1;
//   } else 
//   if (SEGMENT.speed > 99) { // regular candle (mode called every ~25 ms, so 4 frames to have a new target every 100ms)
//     speedFactor = 2;
//   } else 
//   if (SEGMENT.speed > 49) { // slower fade
//     speedFactor = 3;
//   } //else 4 (slowest)

//   uint16_t numCandles = (use_multi) ? SEGLEN : 1;
//   #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//   ALOG_DBG(PSTR("numCandles=%d"), numCandles);
//   #endif

//   for (uint16_t i = 0; i < numCandles; i++)
//   {
//     #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//     ALOG_DBG(PSTR("i=%d|%d"),i,numCandles);
//     #endif

//     uint16_t d = 0; //data location

//     uint8_t s        = SEGMENT.params_internal.aux0, 
//             s_target = SEGMENT.params_internal.aux1, 
//             fadeStep = SEGMENT.step;

//     if (i > 0) {
//       d = (i-1) *3;
//       s = SEGMENT.data[d]; 
//       s_target = SEGMENT.data[d+1]; 
//       fadeStep = SEGMENT.data[d+2];
//     }
//     if (fadeStep == 0) { //init vals
//       s = 128; s_target = 130 + random8(4); fadeStep = 1;
//     }

//     bool newTarget = false;
//     if (s_target > s) { //fade up

//       #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//       ALOG_DBG(PSTR("fade up s_target > s %d=%d"), s_target, s);
//       #endif

//       s = qadd8(s, fadeStep);
//       if (s >= s_target) newTarget = true;
//     } else {
//       s = qsub8(s, fadeStep);
//       if (s <= s_target) newTarget = true;
          
//       #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
//       ALOG_DBG(PSTR("fade down=%d"),s);
//       #endif

//     }

//     if (newTarget) {
//       s_target = random8(rndval) + random8(rndval);
//       if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
//       uint8_t offset = (255 - valrange) >> 1;
//       s_target += offset;

//       uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
//       fadeStep = dif >> speedFactor;
//       if (fadeStep == 0) fadeStep = 1;
//     }

//     // flicker_palette_id = mPalette::PALETTELIST_HTML_COLOUR__White__ID;

//     /**
//      * Apply colour to output: different per pixel
//      **/
//     if(i > 0) 
//     {
//       // colour_pri = SEGMENT.GetPaletteColour(pixel_palette_counter);    
//       colour_pri = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);//, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);

//       colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, i, true, true, true);




//       colour_out = ColourBlend(colour_pri, colour_sec, s); // s = flicker level (i.e. brightness)

//       // if(pixel_palette_counter++ >= pixels_in_palette-1)
//       // {
//       //   pixel_palette_counter = 0;
//       // }

//       SEGMENT.SetPixelColor(i, colour_out);

//       SEGMENT.data[d  ] = s; 
//       SEGMENT.data[d+1] = s_target; 
//       SEGMENT.data[d+2] = fadeStep;

//     } 
//     /**
//      * Single mode, one colour applied across all leds??????????
//      * */
//     else
//     {
      
//       for(uint16_t p = 0;
//                  p < SEGLEN;
//                  p++
//       ){

//         // colour_pri = SEGMENT.GetPaletteColour(pixel_palette_counter); 
//         colour_pri = SEGMENT.GetPaletteColour(p, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);//, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);

//         colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, p);

//         colour_out = ColourBlend(colour_pri, colour_sec, s); // s = flicker level (i.e. brightness)

//         // if(pixel_palette_counter++ >= pixels_in_palette-1)
//         // {
//         //   pixel_palette_counter = 0;
//         // }

//         SEGMENT.SetPixelColor(p, colour_out);

//       }

//       SEGMENT.params_internal.aux0 = s; 
//       SEGMENT.params_internal.aux1 = s_target; 
//       SEGMENT.step = fadeStep;

//     }
//   }

//   SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

// }
void mAnimatorLight::EffectAnim__Flicker_Base(bool use_multi, uint16_t flicker_palette_id)
{
    uint8_t pixels_in_primary_palette = GetNumberOfColoursInPalette(SEGMENT.palette_id);
    uint8_t pixels_in_secondary_palette = GetNumberOfColoursInPalette(flicker_palette_id);

    RgbcctColor colour_pri;
    RgbcctColor colour_sec;
    RgbcctColor colour_out;

    bool single_secondary_color = (pixels_in_secondary_palette == 1);

    if (use_multi)
    {
        uint16_t dataSize = (SEGLEN - 1) * 3;
        if (!SEGMENT.allocateData(dataSize)) { return; }
    }

    // Load the single secondary color once if the palette contains only one color
    if (single_secondary_color)
    {
        colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, 0, true, true, true);
    }

    // max. flicker range controlled by intensity
    uint8_t valrange = SEGMENT.intensity;
    uint8_t rndval = valrange >> 1; // divide by 2

    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBG(PSTR("step=%d"), SEGMENT.step);
    ALOG_DBG(PSTR("valrange=%d"), valrange);
    ALOG_DBG(PSTR("rndval=%d"), rndval);
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
    } // else slowest (speedFactor = 4)

    uint16_t numCandles = (use_multi) ? SEGLEN : 1;

    #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
    ALOG_DBG(PSTR("numCandles=%d"), numCandles);
    #endif

    for (uint16_t i = 0; i < numCandles; i++)
    {
        #ifdef ENABLE__DEBUG_POINT__ANIMATION_EFFECTS
        ALOG_DBG(PSTR("i=%d|%d"), i, numCandles);
        #endif

        uint16_t d = 0; // data location

        uint8_t s = SEGMENT.params_internal.aux0, 
                s_target = SEGMENT.params_internal.aux1, 
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
            colour_pri = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);

            // If only one color in secondary palette, reuse it, otherwise get a new color for each pixel
            if (!single_secondary_color) {
                colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, i, true, true, true);
            }

            // Blend primary and secondary color based on flicker intensity
            colour_out = ColourBlend(colour_pri, colour_sec, s);

            SEGMENT.SetPixelColor(i, colour_out);

            SEGMENT.data[d] = s;
            SEGMENT.data[d + 1] = s_target;
            SEGMENT.data[d + 2] = fadeStep;
        }
        else
        {
            for (uint16_t p = 0; p < SEGLEN; p++)
            {
                // Get primary color for each pixel
                colour_pri = SEGMENT.GetPaletteColour(p, PALETTE_INDEX_SPANS_SEGLEN_ON, true, true);

                // If only one color in secondary palette, reuse it, otherwise get a new color for each pixel
                if (!single_secondary_color) {
                    colour_sec = GetColourFromUnloadedPalette3(flicker_palette_id, p, true, true, true);
                }

                // Blend primary and secondary color based on flicker intensity
                colour_out = ColourBlend(colour_pri, colour_sec, s);

                SEGMENT.SetPixelColor(p, colour_out);
            }

            SEGMENT.params_internal.aux0 = s;
            SEGMENT.params_internal.aux1 = s_target;
            SEGMENT.step = fadeStep;
        }
    }

    SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);
}

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Based on accidently effect
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
/**
 * @description:   : 
 **/
void mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_Black()
{
  EffectAnim__Popping_Decay_Base(true, true);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_BLACK[] PROGMEM = "Popping Palette Fade@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
/**
 * @description:   : 
 **/
void mAnimatorLight::EffectAnim__Popping_Decay_Random_To_Black()
{
  EffectAnim__Popping_Decay_Base(false, true);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_BLACK[] PROGMEM = "Popping Random Black@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
/**
 * @description:   : 
 **/
void mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_White()
{
  EffectAnim__Popping_Decay_Base(true, false);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_WHITE[] PROGMEM = "Popping Palette White@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
/**
 * @description:   : 
 **/
void mAnimatorLight::EffectAnim__Popping_Decay_Random_To_White()
{
  EffectAnim__Popping_Decay_Base(false, false);
}
static const char PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_WHITE[] PROGMEM = "Popping Random White@,,,,,Repeat Rate (ms);!,!,!,!,!;!";


/**
 * @description:   : Base function for pop in, fade out
 * */
void mAnimatorLight::EffectAnim__Popping_Decay_Base(bool draw_palette_inorder, bool fade_to_black)
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



  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength() )){ return; }
  
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
  RgbcctColor colour;

  /**
   * @brief On first run, all leds need set once
   * */
  if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
    pixels_to_update = SEGMENT.virtualLength(); // only the virtual length
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  for(uint16_t iter = 0; 
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
    ALOG_DBM(LOG_LEVEL_TEST, 
      PSTR("New palettePixel=%d, pixel_index=v%d SL%d | SVL%d | DL%d, colour=%d,%d,%dT%d"), 
        desired_pixel, pixel_index, 
        SEGMENT.length(), SEGMENT.virtualLength(), SEGMENT.DataLength(),
        colour.R, colour.G, colour.B,
        SEGMENT.colour_type__used_in_effect_generate
    ); 
    #endif // ENABLE__DEBUG_POINT__ANIMATION_EFFECTS

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

  }



  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel_WithFade(2);



  /**
   * @brief Step 3: Fade out colours, but make sure not to fade the new ones (hence step 2 and 3 should be randomly together, or too complex, just make sure pop_in is greater than fade_out)
   * 
   */








  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );




















//   ALOG_INF(PSTR("s=%d\t a0=%d\t a1=%d\t a2=%d"),    SEGMENT.step, SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1, SEGMENT.params_internal.aux2);

//   uint8_t pixels_in_palette = GetNumberOfColoursInPalette(SEGMENT.palette_id);

//   RgbcctColor colour_pri;
//   RgbcctColor colour_sec;
//   RgbcctColor colour_out;

//   // if (draw_palette_inorder)
//   // {
//   //   uint16_t dataSize = (SEGLEN-1) *3;
//   //   if(!SEGMENT.allocateData(dataSize) ){ return; }
//   // }

  
//   if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength() )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
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

//     uint8_t s        = SEGMENT.params_internal.aux0, 
//             s_target = SEGMENT.params_internal.aux1, 
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

//     uint16_t* drawmode = &SEGMENT.params_internal.aux2;
//     // uint16_t* drawmode = &SEGMENT.params_internal.aux3;

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
//         SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour);
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
//         SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel_to_update, SEGMENT.colour_type__used_in_effect_generate, colour);
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
//         SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour_out);
//       }
//     }


//     SEGMENT.params_internal.aux0 = s; 
//     SEGMENT.params_internal.aux1 = s_target; 
//     SEGMENT.step = fadeStep;

  
//   DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

//   SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );



  // SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Static Gradient Palette
 * @note : Palette will be blended across total length of segment
 *         If Palette contains gradient information, these will be used as inflection points, otherwise, span with equal spacing
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 * line 2583
 * 
 * delete, replaced by spanned palette
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Static_Gradient_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength();

  ALOG_ERR( "delete, replaced by spanned palette" );
  
  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }
  
  uint16_t start_pixel = 0;
  uint16_t end_pixel = 100;
  RgbcctColor start_colour = RgbcctColor();
  RgbcctColor end_colour = RgbcctColor();
  RgbcctColor out_colour = RgbcctColor();
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


      float progress = 0;
      for(
        int 
        index=start_pixel_position;
        index<=end_pixel_position;
        index++){
        
          progress = mSupport::mapfloat(index,start_pixel_position,end_pixel_position,0,1);
          
          out_colour = RgbcctColor::LinearBlend(start_colour, end_colour, progress);
     
          SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), index, SEGMENT.colour_type__used_in_effect_generate, out_colour.WithBrightness(brightness) );
      }

  }

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__STATIC_GRADIENT_PALETTE[] PROGMEM = "Static Gradient Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rotating Palette
 * @note : Moves 1D animation along 
 * 
 * @param aux0: direction
 * @param aux1: saved start position that is moved with each rotation
 * @param aux2 
 * @param intensity: 
 *        ** 0: No new colours will be redrawn. Previous state will be moved
 *        ** 1: Redraw palette EACH time, since randomised palettes may be updating in the background. Keep record of "starting_pixel_index" in aux1
 *        ** 2-255: With blending enabled, how large to rotate/step across the segment. 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Rotating_Palette()
{

  uint16_t* region_p             = &SEGMENT.params_internal.aux0;
  uint16_t* movement_direction_p = &SEGMENT.params_internal.aux1;
  
  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  enum EFFECTSREGION
  {
      EFFECTS_REGION_COLOUR_SELECT_ID=0,
      EFFECTS_REGION_ANIMATE_ID
  };      

  switch(*region_p){
    case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
    {
      
      ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
           
      ALOG_DBM( PSTR("Segment: %d\t(%d,%d),(%d)"), SEGIDX, SEGMENT.start, SEGMENT.stop, SEGMENT.palette_id);

      RgbcctColor colour = RgbcctColor();
      for (uint16_t pixel = 0; pixel < SEGLEN; pixel++)
      {

        colour = SEGMENT.GetPaletteColour(pixel); //mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
                
        SEGMENT.SetPixelColor(pixel, colour.WithBrightness(brightness), BRIGHTNESS_ALREADY_SET);

        #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
        // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
        #endif

      }
      
      *region_p = EFFECTS_REGION_ANIMATE_ID;
    }
    break; //not into next right away
    case EFFECTS_REGION_ANIMATE_ID: //shift along
    {

      ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_REGION_ANIMATE_ID"));

// #define SEG_STOP_INDEX   strip->_virtualSegmentLength-1  //prob wrong now!
//       if(*movement_direction_p==1)
//       { // direction==1 move right ie AWAY from start
//         /**
//          * @brief Save first, move pixels towards last, assert last
//          **/
//         RgbcctColor colourlast = SEGMENT.GetPixelColor(SEG_STOP_INDEX); 
//         for(int32_t p = SEG_STOP_INDEX-1; p >= 0; p--){ //must be signed
//           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p-1));
//         }
//         SEGMENT.SetPixelColor(0, colourlast);
//       }
//       else
//       {
//         /**
//          * @brief Save last, move pixels back to first, assert first
//          **/
//         RgbcctColor colourfirst = SEGMENT.GetPixelColor(0); 
//         for(uint16_t p = 0; p <= SEG_STOP_INDEX; p++){ 
//           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p+1));
//         }
//         SEGMENT.SetPixelColor(SEG_STOP_INDEX, colourfirst);
//       }

    }
    break;
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__ROTATING_PALETTE[] PROGMEM = "Rotating Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


// #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */

// void mAnimatorLight::EffectAnim__Rotating_Palette()
// {

//   uint16_t* region_p             = &SEGMENT.params_internal.aux0;
//   uint16_t* movement_direction_p = &SEGMENT.params_internal.aux1;
  
//   enum EFFECTSREGION
//   {
//       EFFECTS_REGION_COLOUR_SELECT_ID=0,
//       EFFECTS_REGION_ANIMATE_ID
//   };      

//   switch(*region_p){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//     {
      
//       ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
           
//       ALOG_DBM( PSTR("Segment: %d\t(%d,%d),(%d)"), SEGIDX, SEGMENT.start, SEGMENT.stop, SEGMENT.palette_id);

//       RgbcctColor colour = RgbcctColor();
//       for (uint16_t pixel = 0; pixel < SEGLEN; pixel++)
//       {

//         // colour = mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
//         colour = SEGMENT.GetPaletteColour(pixel);
        
//         colour = ApplyBrightnesstoDesiredColourWithGamma(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());
        
//         SEGMENT.SetPixelColor(pixel, colour, BRIGHTNESS_ALREADY_SET);

//         #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//         ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
//         #endif

//       }
      
//       *region_p = EFFECTS_REGION_ANIMATE_ID;
//     }
//     break; //not into next right away
//     case EFFECTS_REGION_ANIMATE_ID: //shift along
//     {

//       ALOG_DBM(PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_REGION_ANIMATE_ID"));

// // #define SEG_STOP_INDEX   strip->_virtualSegmentLength-1  //prob wrong now!
// //       if(*movement_direction_p==1)
// //       { // direction==1 move right ie AWAY from start
// //         /**
// //          * @brief Save first, move pixels towards last, assert last
// //          **/
// //         RgbcctColor colourlast = SEGMENT.GetPixelColor(SEG_STOP_INDEX); 
// //         for(int32_t p = SEG_STOP_INDEX-1; p >= 0; p--){ //must be signed
// //           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p-1));
// //         }
// //         SEGMENT.SetPixelColor(0, colourlast);
// //       }
// //       else
// //       {
// //         /**
// //          * @brief Save last, move pixels back to first, assert first
// //          **/
// //         RgbcctColor colourfirst = SEGMENT.GetPixelColor(0); 
// //         for(uint16_t p = 0; p <= SEG_STOP_INDEX; p++){ 
// //           SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p+1));
// //         }
// //         SEGMENT.SetPixelColor(SEG_STOP_INDEX, colourfirst);
// //       }

//     }
//     break;
//   }

//   SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

// }
// static const char PM_EFFECT_CONFIG__ROTATING_PALETTE[] PROGMEM = "Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
// #endif


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

/**
 * DESRIPTION: Current pixels are rotated set by previous palette. With seqencing/playlists, this enables cascading effect
 * Para: 
 * 
 * Intensity slider will be how many pixels to move by.
 * 0of255, will mean the minimum which is 1 pixel
 * 255 will be scaled from the segment length, perhaps half but needs visual testing for what makes sense. Half would mean alternate the entire pixels
 * 
 * 
 * */
void mAnimatorLight::EffectAnim__Rotating_Previous_Animation()
{

  // ALOG_INF(PSTR("EffectAnim__Rotating_Previous_Animation"));

  uint16_t* movement_direction_p = &SEGMENT.params_internal.aux0;  

  // if(*movement_direction_p==1)
  // { // direction==1 move right ie AWAY from start
  //   /**
  //    * @brief Save first, move pixels towards last, assert last
  //    **/
  //   RgbcctColor colourlast = SEGMENT.GetPixelColor(SEG_STOP_INDEX); 
  //   for(int32_t p = SEG_STOP_INDEX-1; p >= 0; p--){ //must be signed
  //     SEGMENT.SetPixelColor(p, SEGMENT.GetPixelColor(p-1));
  //   }
  //   SEGMENT.SetPixelColor(0, colourlast);
  // }
  // else
  // {
    RgbcctColor colour = SEGMENT.GetPixelColor(0); 

    #ifndef ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY



    RgbcctColor colourfirst = SEGMENT.GetPixelColor(0); 
    RgbcctColor colour = SEGMENT.GetPixelColor(0); 
    // RgbcctColor colour1 = SEGMENT.GetPixelColor(0); 
    // colourfirst.debug_print("colourfirst");
    for(uint16_t p = 0; p < SEGLEN-1; p++){ //move them all ONCE towards first pixel
    // for(uint16_t p = 0; p < 10; p++){ //move them all ONCE towards first pixel
      colour = SEGMENT.GetPixelColor(p+1);
      // colour1 = SEGMENT.GetPixelColor(p+1);
      // ALOG_INF(PSTR("p0 %d=%d,%d,%d"),p, colour.R, colour.G, colour.B);
      // ALOG_INF(PSTR("p1 %d=%d,%d,%d"),p, colour1.R, colour1.G, colour1.B);
    // colour.debug_print("colour");
      SEGMENT.SetPixelColor(p, colour, BRIGHTNESS_ALREADY_SET);
    }
    SEGMENT.SetPixelColor(SEGLEN-1, colourfirst); // Insert saved first pixel into last pixel as "wrap around"

    SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR(); // Change this to be function that sets transition up
    return;


    #endif


    #ifdef ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY





    /**
     * @brief 
     * 
     * 
     * 
     * Should test option, if segment size remains the same, then dont destroy and recreate on webui command load of palette 
     * 
     * 
     * 
     * 
     * 
     * 
     * 
     * 
     *
     * @brief Save last, move pixels back to first, assert first
     * 
     * I think the brightness is suppressing out the colour
     * 
     **/

    uint8_t intensity_value = SEGMENT.intensity;
    uint16_t pixels_to_move = map(intensity_value, 0,255, 1,SEGLEN/2); // 0of255, will mean the minimum which is 1 pixel

    /**STEP 1: Save the START section that will be overwritten
     * @brief JUMP  section:    0 - pixels_to_move  Jump is saved, then inserted at the end for the wrap
     *        SLIDE section:    (pixels_to_move+1) - SEGLEN   Slide is moved one pixel at a time
     */
    uint16_t jump_end_pixel = pixels_to_move; // ie 0-jump_end_pixel 

    // ALOG_INF(PSTR("intensity_value %d, pixels_to_move %d"), intensity_value, pixels_to_move);

    std::vector<RgbcctColor> colours_saved;
    for(uint16_t pixel = 0; pixel < jump_end_pixel; pixel++)
    {
      // ALOG_INF(PSTR("Ap %d, %d/%d"), pixel, pixel, jump_end_pixel);
      colours_saved.push_back(SEGMENT.GetPixelColor(pixel));
    }
      // ALOG_INF(PSTR("colours_saved %d"), colours_saved.size() );

    /* STEP 2: Move the rest of the pixels from END, into START
    *
    * So if 100 leds total, and we are moving the last 50, then the 50th needs moved to 0, 51st to 1 etc
    * So the shift would be getPixel(50) to setPixel(0), getPixel(51) to setPixel(1) etc
    * 
    */
    for(uint16_t pixel = 0; pixel < SEGLEN-jump_end_pixel; pixel++){ // 100-50
      colour = SEGMENT.GetPixelColor(pixel+jump_end_pixel);
      SEGMENT.SetPixelColor(pixel, colour, BRIGHTNESS_ALREADY_SET);
      // ALOG_INF(PSTR("Bp %d->%d"), pixel+jump_end_pixel, pixel);
    }

    /** STEP 3: Insert the saved pixels at the end
     * @brief 
     * 
     * 
     */
    for(uint16_t pixel = 0; pixel < colours_saved.size(); pixel++){ // 100-50
      colour = colours_saved[pixel];
      SEGMENT.SetPixelColor(pixel+(SEGLEN-jump_end_pixel), colour, BRIGHTNESS_ALREADY_SET);
      // ALOG_INF(PSTR("Cp %d->%d,  %d, %d/%d"),pixel, pixel+(SEGLEN-jump_end_pixel), pixel, pixel, jump_end_pixel);
    }


  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR(); // Change this to be function that sets transition up

    #endif
}
// static const char PM_EFFECT_CONFIG__POPCORN[] PROGMEM = "!,!,,,,,,,Overlay;!,!,!;!;;m12=1"; //bar
static const char PM_EFFECT_CONFIG__ROTATING_PREVIOUS_ANIMATION[] PROGMEM = "Rotating Previous@!,!,,,,Repeat Rate (ms);!,!,!,!,!;etp=23,ix=1"; // 7 sliders (speed/intensity/custom1/custom2/custom3/time/rate) + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Step_Through_Palette
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Stepping_Palette()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }

  uint16_t* region_p          = &SEGMENT.params_internal.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.params_internal.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.params_internal.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

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

  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  RgbcctColor colour;

  // ALOG_INF(PSTR("SEGMENT %d,%d"), SEGMENT.start, SEGMENT.stop);

  // indexing from start to stop does not work when segment is not the first (seg0)

  for(uint16_t index=0;
                index<=SEGMENT.length();
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, desired_pixel, &pixel_position);
    
    colour = RgbcctColor::ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index, SEGMENT.colour_type__used_in_effect_generate, colour);
        
  } 




  // ALOG_INF(PSTR("step %d,%d,%d"), colour.R, colour.G, colour.B);

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__STEPPING_PALETTE[] PROGMEM = "Stepping Palette@BlendSpeed,ix,,,,Repeat Rate (ms);!,!,!,!,!;!;etp=1000"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Step_Through_Palette
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__TimeBased__HourProgress()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, RgbcctColor(uint32_t(0)) );
  }

  uint16_t seconds_into_hour = pCONT_time->RtcTime.second + (pCONT_time->RtcTime.minute * 60); // Using seconds for improved accuracy
  uint16_t progress = map(seconds_into_hour, 0,3550, 1,SEGLEN); // using 3550 to enable 50 seconds on full display

  for(uint16_t pixel = 0; pixel < progress; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__TIMEBASED__HOUR_PROGRESS[] PROGMEM = "Hour Progress@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Step_Through_Palette
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
void mAnimatorLight::EffectAnim__Stepping_Palette_With_Background()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }

  uint16_t* region_p          = &SEGMENT.params_internal.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.params_internal.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.params_internal.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

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

  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  RgbcctColor colour;

  for(uint16_t index=SEGMENT.start;
                index<=SEGMENT.stop;
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, desired_pixel, &pixel_position);
    
    colour = RgbcctColor::ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index++, SEGMENT.colour_type__used_in_effect_generate, SEGCOLOR_RGBCCT(0));
    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index++, SEGMENT.colour_type__used_in_effect_generate, SEGCOLOR_RGBCCT(0));
    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index, SEGMENT.colour_type__used_in_effect_generate, colour);
        
  } 

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }
  
  
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__STEPPING_PALETTE_WITH_BACKGROUND[] PROGMEM = "Stepping Palette Background@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Blend Palette to White
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
    // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    // EFFECTS_FUNCTION__STATIC_PALETTE_GRADIENT__ID,
    // #endif
    /**
     * Desc: Animation, that fades from selected palette to white,
     *       The intensity of fade (closer to white, ie loss of saturation) will depend on intensity value
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
// #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
// void mAnimatorLight::EffectAnim__Palette_Colour_Fade_Saturation()
// {

//   uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length();

//   if (!SEGMENT.allocateData(dataSize)){    
//     ALOG_ERR( PM_MEMORY_INSUFFICIENT );
//     SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
//     return;
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

//     SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour);

//     #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
//     #endif

//   }
  
//     //SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type__used_in_effect_generate, RgbColor(255));


//   // Get starting positions already on show
//   DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

//   // Call the animator to blend from previous to new
//   SetSegment_AnimFunctionCallback(  SEGIDX, 
//     [this](const AnimationParam& param){ 
//       this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
//     }
//   );

// }
// #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Blend Palette Between Another Palette
 * @note : Using a static palette (in order), the effect will change from static_palette (palette_id) to a different palette (palette_id saved as aux0)
 *         The "0%", ie least saturated, will be set by the intensity value scaled from 255 to 100 range
 * 
 * @param  "Intensity" :: 0-255 value is mapped into 0-100 and controls the amount to blend into the second one. ie 0%=palette_id and 100%=aux0_palette
 * @param  "Time"      :: If unset (ie set to 0), then the speed will assume the DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD as 10 seconds
 *         "Speed"     :: If the time is set to zero, then the Speed will scale from sweep of saturation time between DEFAULT_MINIMUM_ANIMATION_SPEED_PERIOD (1 second) and DEFAULT_MAXIMUM_ANIMATION_SPEED_PERIOD (10 second)
 *          
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Blend_Two_Palettes()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels
  //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
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
  RgbcctColor colour = RgbcctColor();

  /**
   * @brief On first run, all leds need set once
   * 
   */
  if(SEGMENT.flags.animator_first_run){ // replace random, so all pixels are placed on first run:: I may also want this as a flag only, as I may want to go from static_Effect to slow_glow with easy/slow transition
    pixels_to_update = SEGMENT.length();
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 


  for(uint16_t iter = 0; 
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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

  }

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__BLEND_TWO_PALETTES[] PROGMEM = "Blend Two Palettes@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED





/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : NOTDONE Blend Palette Between Another Palette
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
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Twinkle_Palette_Onto_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength();

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 
  
  /**
   * @brief Step 1: Draw palette 1 as base
   * 
   */
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel);    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

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

  RgbcctColor overdraw_colour = RgbcctColor();

  
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

    overdraw_colour = GetColourFromUnloadedPalette3(SEGMENT.params_user[0], random_pic); // We want seglen, because gradient palettes should be drawn in their expected gradient
    // overdraw_colour = GetColourFromUnloadedPalette3(0, random(0,palette_over_length));

    // if(pixel==0) ALOG_INF(PSTR("overdraw %d %d %d %d %d"), SEGMENT.params_user[0], random_pic, overdraw_colour[0], overdraw_colour[1], overdraw_colour[2]);

    /**
     * @brief If palette is one of the segment colours, then use its built in brightness as the palette over brightness
     **/
    if(palette_to_draw_ontop < mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
    {
      overdraw_colour = overdraw_colour.WithBrightness(); // Use already set SegColour 
    }else{
      overdraw_colour = overdraw_colour.WithBrightness(SEGMENT.params_user[1]); // Param1 is brightness control of Palette2
    }

    // overdraw_colour.debug_print("overdraw_colour");

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random(0,SEGMENT.length()), SEGMENT.colour_type__used_in_effect_generate, overdraw_colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(SEGMENT.palette_id), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI[] PROGMEM = "Twinkle Palette Two on One@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED



#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Twinkle_Out_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength();

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }

  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 
  
  /**
   * @brief Step 1: Draw palette 1 as base
   * 
   */
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

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

  RgbcctColor overdraw_colour = RgbcctColor();








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

    overdraw_colour = RgbcctColor();  //GetColourFromUnloadedPalette2(SEGMENT.params_user[0], random_pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.params_internal.aux0, pixel);

    // overdraw_colour.debug_print("overdraw_colour");

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel, SEGMENT.colour_type__used_in_effect_generate, overdraw_colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__TWINKLE_OUT_PALETTE[] PROGMEM = "Twinkle Out Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : NOTDONE Blend Palette Between Another Palette
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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Twinkle_Decaying_Palette()
{
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.virtualLength();

// effect not written yet, copied from another

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__SOLID_COLOUR__ID;
    return;
  }
  
  uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch to save time 

  /**
   * @brief Step 1: Draw palette 1
   * 
   */
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; 
                pixel < SEGMENT.virtualLength(); 
                pixel++
  ){

    colour = SEGMENT.GetPaletteColour(pixel); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, pixel);
        
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

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
  colour = RgbcctColor(255,255,255);
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

    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), random_pixel, SEGMENT.colour_type__used_in_effect_generate, colour);

    #ifdef ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // ALOG_INF( PSTR("sIndexIO=%d %d,%d\t%d,pC %d, R%d"), SEGIDX, SEGMENT.start, SEGMENT.stop, pixel, GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr), colour.R );
    #endif

  }



  /**
   * @brief Update outputs
   * 
   */

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__TWINKLE_DECAYING_PALETTE[] PROGMEM = "Twinkle Decaying Palette@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Sunrise_Alarm_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__Sunrise_Alarm_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__SUNRISE_ALARM_01[] PROGMEM = "Sun Alarm 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : SunPositions__Azimuth_Selects_Gradient_Of_Palette_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness));
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01[] PROGMEM = "Sun Azimuth Palette 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : EffectAnim__SunPositions__Sunset_Blended_Palettes_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__Sunset_Blended_Palettes_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__SUNSET_BLENDED_PALETTES_01[] PROGMEM = "SunSet Blended Palettes 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : EffectAnim__SunPositions__DrawSun_1D_Elevation_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Elevation_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness));
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_ELEVATION_01[] PROGMEM = "Sun 1D Elevation 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : EffectAnim__SunPositions__DrawSun_1D_Elevation_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Azimuth_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_AZIMUTH_01[] PROGMEM = "Sun 1D Azimuth 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01[] PROGMEM = "Sun 2D El/Az 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01
 * @description:   : 
 * 
 * @param Intensity: None
 * @param Speed    : None
 * @param rate     : None
 * @param time     : Blend time on first/only update
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
void mAnimatorLight::EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__SUNPOSITIONS__WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01[] PROGMEM = "Sun White Corrected El 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS



































































/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend
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
void mAnimatorLight::EffectAnim__SunPositions_Elevation_Palette_Progress_Step()
{

// for sun thing
// I will still use this function to get the raw colour, I just need another intermediate function that does the conversion with sun elevation
// also add sun elevation and azimuth into settings struct, that way I can update it locally or via command 

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_TST(PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  SEGMENT.transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

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
 * @name : EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend
 * @note : Randomly changes colours of pixels, and blends to the new one
 *  
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS2
void mAnimatorLight::EffectAnim__SunPositions_Elevation_Palette_Progress_LinearBlend()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_TST(PSTR("Not Enough Memory"));
    #endif // ENABLE_LOG_LEVEL_INFO
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // this should probably force order as random, then introduce static "inorder" option
  SEGMENT.transition.order_id = TRANSITION_ORDER__RANDOM__ID;  
  // So colour region does not need to change each loop to prevent colour crushing
  SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  // Pick new colours
  DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

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
void mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01()
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

//   pCONT_lAni->force_update();

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
 * @name : Sequential
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
void mAnimatorLight::SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01()
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
 * @name : RGB CLock
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
  if ( pCONT_time->second(t) % 2 == 0 ) 
    LCDDisplay_showDots(2, pCONT_time->second(t) * 4.25);                                // show : between hours and minutes on even seconds with the color cycling through the palette once per minute
  lastSecond = pCONT_time->second(t);
}


void mAnimatorLight::LCDDisplay_showSegment(byte segment, byte color_index, byte segDisplay) {
  
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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel_index, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

  }
  
}


void mAnimatorLight::LCDDisplay_showDigit(byte digit, byte color, byte pos) {
  // This draws numbers using the according segments as defined on top of the sketch (0 - 9)
  for (byte i = 0; i < 7; i++) {
    if (digits[digit][i] != 0) LCDDisplay_showSegment(i, color, pos);
  }
}


void mAnimatorLight::LCDDisplay_showDots(byte dots, byte color) {

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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), startPos, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );


    if ( dots == 2 ) 
    {

      RgbcctColor colour = RgbcctColor();
      colour = SEGMENT.GetPaletteColour(color);      
      SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), startPos + 1, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );

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
void mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_01()
{

  ALOG_DBM( PSTR("_segments[%d].colour_type__used_in_effect_generate = %d"), SEGIDX, SEGMENT.colour_type__used_in_effect_generate);
    
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_DBM(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    ALOG_ERR(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  for(int i=0;i<SEGLEN;i++)
  {    
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type__used_in_effect_generate, RgbcctColor());
  }

  uint8_t colour_offset = 1;
  if(SEGMENT.palette_id < 83)
    colour_offset = 50;

  ALOG_DBM(PSTR("colour_offset = %d"), colour_offset);

  LCDDisplay_displayTime(pCONT_time->Rtc.local_time, 0, colour_offset);

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

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

void mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01()
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
void mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_02(){
//   // So colour region does not need to change each loop to prevent colour crushing
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;
//   // Pick new colours
//   //Display on all pixels
//   UpdateDesiredColourFromPaletteSelected();
  // pCONT_set->Settings.light_settings.type = ADDRESSABLE_SK6812;
    
  // SEGMENT.colour_type__used_in_effect_generate = RgbcctColor::ColourType::LIGHT_TYPE__RGBW__ID;
  
  ALOG_TST(PSTR("02    SEGMENT.colour_type__used_in_effect_generate = %d"), SEGMENT.colour_type__used_in_effect_generate);

    
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    ALOG_TST(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }

  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  
  // for(int i=0;i<93;i++){animation_colours[i].DesiredColour = RgbcctColor();}
  uint8_t segment_index=0;

  for(int i=0;i<93;i++){
    // animation_colours[i].DesiredColour = RgbcctColor();
    // }
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type__used_in_effect_generate, RgbwColor(0,0,0,0));
  
  }

  LCDDisplay_displayTime(pCONT_time->Rtc.local_time, 0, colorOffset);
  
  // if (overlayMode == 1) LCDDisplay_colorOverlay();
  // for (byte i = 0; i < LED_COUNT; i++) {                                                                    // check each led...
  //   if (animation_colours[i].DesiredColour.CalculateBrightness())  {

  //     RgbcctColor colour = RgbcctColor();

  //     animation_colours[i].DesiredColour = ColorFromPalette_WithLoad(pCONT_iLight->animation.palette_id, startColor + (colorOffset * i));      
  //     SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type__used_in_effect_generate, RgbwColor(0,0,0,0));



  //   }
  // }    


  // LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  // LCDDisplay_showDigit((lcd_display_show_number % 10), 0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)
  

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

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

void mAnimatorLight::ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02()
{   
  
  JBI->Add("lcd_display_show_number", lcd_display_show_number);
  
}


/****
 * Changes pixels randomly to new colour, with slow blending
 * Requires new colour calculation each call
 * 02 trying lib method with mapping
 */
void mAnimatorLight::EffectAnim__7SegmentDisplay__ManualNumber_01()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    ALOG_TST(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
    
  
  ALOG_TST(PSTR("Numbers_Basic    _segments[].colour_type__used_in_effect_generate = %d"), SEGMENT.colour_type__used_in_effect_generate);
  ALOG_TST(PSTR("lcd_display_show_number = %d"), lcd_display_show_number);
  /**
   * @brief Reset all new colours so only new sections of clock are lit
   **/
  for(int i=0;i<SEGLEN;i++){
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type__used_in_effect_generate, RgbwColor(0,0,0,0));
  }


  uint8_t digit_count = mSupport::NumDigits(lcd_display_show_number);

  LCDDisplay_showDigit((lcd_display_show_number / 10), 0+1, 1 );                   // minutes thankfully don't differ between 12h/24h, so this can be outside the above if/else
  LCDDisplay_showDigit((lcd_display_show_number % 10),   0, 0 );                   // each digit is drawn with an increasing color (*2, *3, *4, *5) (*6 and *7 for seconds only in HH:MM:SS)

  // Get starting positions already on show
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

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
void mAnimatorLight::EffectAnim__7SegmentDisplay__ManualString_01()
{
  
  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  ALOG_TST(PSTR("dataSize = %d"), dataSize);

  if (!SEGMENT.allocateData(dataSize))
  {
    ALOG_TST(PSTR("Not Enough Memory"));
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID; // Default
  }
  
  // So colour region does not need to change each loop to prevent colour crushing
  // SEGMENT.flags.brightness_applied_during_colour_generation = true;
  
  ALOG_TST(PSTR("Numbers_Basic    _segments[].colour_type__used_in_effect_generate = %d"), SEGMENT.colour_type__used_in_effect_generate);
  ALOG_TST(PSTR("lcd_display_show_number = %d"), lcd_display_show_number);

  /**
   * @brief Reset all new colours so only new sections of clock are lit
   **/
  for(int i=0;i<SEGLEN;i++){
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), i, SEGMENT.colour_type__used_in_effect_generate, RgbwColor(0,0,0,0));
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
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // Call the animator to blend from previous to new
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );

}
static const char PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALSTRING_01[] PROGMEM = "Seven-Segment String 01@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;



#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Christmas style "slow fade brightness"
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturation fade (all together)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturation fade (wipe)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Saturatoin fade (randomly selected)
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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
//       if(pCONT_iLight->animation.palette_id++>=mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_07_ID){
//         pCONT_iLight->animation.palette_id = mPaletteI->PALETTELIST_STATIC_GRADIENT_SUNLEVEL_GROUP01_01_ID;
//       }


// AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette_id=%d"),pCONT_iLight->animation.palette_id);


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
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for

// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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

//   AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette_id=%d"),pCONT_iLight->animation.palette_id);

//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = true;

//   pCONT_iLight->settings.light_size_count = 100;
  
//   // Get all the current state
//   // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = SEGMENT.GetPixelColor(index);
//   // }
//     for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//     animation_colours[index].DesiredColour = RgbcctColor();//,0,30,0,0);//,0,10,0,0);
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
//       effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
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
//   //     effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
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
//         //   // if(pCONT_iLight->RgbColorto32bit(SEGMENT.GetPixelColor(random_pixel_index))>0){
//         //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//         //   // }else
//         //   // // Previously off, pick new state
//         //   // {
//         //     if(colour_or_black){ // 80% in favour of black/off
//         //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
//         //     }else{
//         //       uint8_t desired_pixel = random(0,pixels-1);
//         //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);  
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
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for

// }




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
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
//   //   ALOG_INF(PSTR("ConstructJSONBody_Animation_Progress__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01 FIRST RUN"));
//   //   delay(5000);
//   // }else{
//   //   ALOG_INF(PSTR("NOT SETTING"));

//   }
//   #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
//   // delay(5000);

//   // pCONT_iLight->animation.palette_id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values

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
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette_id);
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
//   // ALOG_INF(PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // ALOG_INF(PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // ALOG_INF(PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

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
//   //   SEGMENT.SetPixelColor(pixel, updatedColor);
//   // } // END for

//   // DEBUG_LINE_HERE;

//   JBI->Add("test2","debug2");

//     // SEGMENT.SetPixelColor(0, RgbColor(0,random(0,255),0));
// }

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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
// #ifdef USE_MODULE_SENSORS_SUN_TRACKING
//   #ifdef USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
//   sun_elevation = (float)pCONT_solar->solar_position_testing.elevation;
//   #else
//   sun_elevation = (float)pCONT_solar->solar_position.elevation;
//   #endif
// #endif


//   // star* stars = reinterpret_cast<star*>(pCONT_iLight->effects_data_buffer);
//   EFFECT_CONFIG* effect_config2 = reinterpret_cast<EFFECT_CONFIG*>(pCONT_iLight->effects_data_buffer);
//   //should be memcpy be used to insure size if not exceeded? or sizeof check? pointer is faster

//   AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette_id=%d"),pCONT_iLight->animation.palette_id);


//   // pCONT_iLight->settings.light_size_count = 1;
  
//   // Get all the current state
//   // for(uint16_t index=0; index<pCONT_iLight->settings.light_size_count; index++ ){
//   //   animation_colours[index].DesiredColour = SEGMENT.GetPixelColor(index);
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

//     SEGMENT.rgbcct_controller->setRGB(0,0,blue);

//     // ALOG_INF(PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




//     uint8_t brightness_255 = map(sun_elevation,-50,10,255,0);

//     SEGMENT.rgbcct_controller->setBrightnessRGB255(brightness_255);

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
//     SEGMENT.rgbcct_controller->setCCT(cct_val);


//     uint8_t brightness_255 = map(sun_elevation,-10,25,0,255);

//     SEGMENT.rgbcct_controller->setBrightnessCCT255(brightness_255);

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
//   //     effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
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
//   //     effect_config2->index_circle_center, //D_RGB_ORANGE_WARM_1
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
//         //   // if(pCONT_iLight->RgbColorto32bit(SEGMENT.GetPixelColor(random_pixel_index))>0){
//         //   //   animation_colours[random_pixel_index].DesiredColour = RgbColor(0,0,0);
//         //   // }else
//         //   // // Previously off, pick new state
//         //   // {
//         //     if(colour_or_black){ // 80% in favour of black/off
//         //       animation_colours[random_pixel_index].DesiredColour = RgbColor(0,255,0);
//         //     }else{
//         //       uint8_t desired_pixel = random(0,pixels-1);
//         //       colour_random = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel);  
//         //       //   colour_random = ApplyBrightnesstoDesiredColour(colour_random,pCONT_iLight->getBriRGB());
//         //       animation_colours[random_pixel_index].DesiredColour =  colour_random;//RgbColor(0,0,255);//
//         //    }
//         //   // }
//         // }

// pCONT_iLight->animation.palette_id = 10;

//  // ALOG_TST(PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   // ALOG_TST(PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();
//   // GetPixelColourHardwareInterface();

//   // ALOG_TST(PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

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
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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

//   // AddLog(LOG_LEVEL_TEST,PSTR("pCONT_iLight->animation.palette_id=%d"),pCONT_iLight->animation.palette_id);

// /**
//  * 
//  * 
//  * */


//   // pCONT_iLight->CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(10);

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
//     hsb.H = pCONT_iLight->HUE_N2F(mapvalue(sun_elevation,10,-5,35,0)); // yellow to red
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

//     // ALOG_INF(PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

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
//     hsb.H =  pCONT_iLight->HUE_N2F(mapvalue(sun_elevation,-5,-10,180,240)); // yellow to red
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

//     // ALOG_INF(PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

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
//     hsb.H = pCONT_iLight->HUE_N2F(240); // yellow to red
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

//     // ALOG_INF(PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);

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

//   //   // ALOG_INF(PSTR("elevation=%d, cct_temp=%d %d"),(int)sun_elevation, elev_perc, cct_val);




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




//   pCONT_iLight->animation.palette_id = 10;

//  // ALOG_TST(PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
//   // Set up colours
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   // ALOG_TST(PSTR("DesiredColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();
//   // GetPixelColourHardwareInterface();

//   // ALOG_TST(PSTR("StartingColour2=%d,%d,%d,%d,%d"), animation_colours_rgbcct.StartingColor.R,animation_colours_rgbcct.StartingColor.G,animation_colours_rgbcct.StartingColor.B,animation_colours_rgbcct.StartingColor.WC,animation_colours_rgbcct.StartingColor.WW);
    

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
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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

//   pCONT_iLight->animation.palette_id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//  // ALOG_TST(PSTR("SubTask_Flasher_Animate_Function__Solid_Static_Single_Colour"));
//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
//   // Brightness is generated internally, and rgbcct solid palettes are output values
//   pCONT_iLight->animation.flags.brightness_applied_during_colour_generation = false;



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
//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID);
//   uint8_t pixels_max = GetNumberOfColoursInPalette(palette_p);
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
//   //   ALOG_INF(PSTR("sun_error[%d] = %d %d \t %d"),i, indexing,  (int)sun_error[i], palette_indexes[i]);
//   //   // Also record zero-crossing point
//   //   if((indexing-90) == 0)
//   //   {
//   //     zero_crossing_index = i;
//   //     ALOG_INF(PSTR("zero_crossing_index = %d FOUND"), zero_crossing_index);
//   //   }
//   // }

//   //     ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "zero_crossing_index = %d FOUND"), zero_crossing_index);
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

// //     ALOG_INF(PSTR("Checking POSITIVE elevation\n\r"));

// //     uint8_t smallest_error_index = 0;
// //     float smallest_elev_error = fabs(sun_error[zero_crossing_index]);
// //     for(int i=zero_crossing_index+1;i<pixels_max;i++) // skip first
// //     {
// //       if(fabs(sun_error[i])<smallest_elev_error)
// //       {
// //         smallest_error_index = i;
// //         smallest_elev_error = fabs(sun_error[i]);
// //       }
// //       // ALOG_INF(PSTR("sun_error[%d] = %d\t smallest_elev[%d]=%d"),
// //       //     i, (int)sun_error[i], smallest_error_index, (int)smallest_elev_error );
// //     }

// //     /**
// //      * previous colour will always equal the matched index (bring to tersary)
// //      * Work out the transitions
// //      * */
// //     if(sun_error[smallest_error_index] == 0) // EXACT ERROR
// //     {
// //       ALOG_INF(PSTR("sun_error EXACT"));
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
// //       ALOG_INF(PSTR("sun_error CLOSE"));
// //       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
// //         ALOG_INF(PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
        
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

// //     ALOG_INF(PSTR("Checking NEGATIVE elevation"));
    
// //     uint8_t smallest_error_index = 0;
// //     float smallest_elev_error = fabs(sun_error[0]);
// //     for(int i=1;i<zero_crossing_index;i++) // skip first
// //     {
// //       if(fabs(sun_error[i])<smallest_elev_error)
// //       {
// //         smallest_error_index = i;
// //         smallest_elev_error = fabs(sun_error[i]);
// //       }
// //       ALOG_INF(PSTR("sun_error[%d] = %d smallest_elev_error%d smallest_error_index%d"),i, (int)sun_error[i], (int)smallest_elev_error, smallest_error_index);
// //     }

// //     /**
// //      * previous colour will always equal the matched index (bring to tersary)
// //      * Work out the transitions
// //      * */
// //     if(sun_error[smallest_error_index] == 0)
// //     {
// //       ALOG_INF(PSTR("sun_error[smallest_error_index] == 0"));
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
// //       ALOG_INF(PSTR("sun_error[smallest_error_index] != 0"));
// //       if (sun_is_ascending){ // sun going up, means next will be bigger in elevation, in palette index, means index larger (since palette starts with negatives)
// //       ALOG_INF(PSTR("sun_error[smallest_error_index] != 0 sun_is_ascending"));
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
//     ALOG_INF(PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//     c = c_blended; 
//     ALOG_INF(PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//     c = c_next; 
//     ALOG_INF(PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

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
// //     ALOG_INF(PSTR("rgbcct[%d]=\t%d\t%d\t%d,%d,%d,%d,%d"),desired_index,indexing,adjusted_index,c.R,c.G,c.B,c.WW,c.WC);
// //   }

//   // delay(100);

  
//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;//mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput()){ // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }
  
//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();
  
//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){ 
//     this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
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

//   // pCONT_iLight->animation.palette_id = mPaletteI->PALETTELIST_STATIC_SOLID_RGBCCT_SUN_ELEVATION_WITH_DEGREES_INDEX_01_ID;

//   // Set palette pointer
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);

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

//   /**
//    * Sun elevation indexing is stored in palettes index location.
//    * The current sun elevation shall be searched against for nearest match, then depending on accesending or decending sun the nearest match and nearest next match will be linearblended as current show colour
//    * */

//   /**
//    * Get total pixels in palette
//    * */
//   mPalette::PALETTELIST::PALETTE *palette_p = mPaletteI->GetPalettePointerByID(pCONT_iLight->animation.palette_id);
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
//   // ALOG_INF(PSTR("rgbcct_p\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);
//   c = c_blended; 
//   // ALOG_INF(PSTR("rgbcct_b\t%d,%d,%d,%d,%d (progress %d"),c.R,c.G,c.B,c.WW,c.WC, (int)(progress_between_colours*100));
//   c = c_upper; 
//   // ALOG_INF(PSTR("rgbcct_n\t%d,%d,%d,%d,%d"),c.R,c.G,c.B,c.WW,c.WC);

//   /**
//    * Load new colour into animation
//    * */

//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   animation_colours_rgbcct.DesiredColour  = c_blended;

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
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
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor());
//       // }

//       pCONT_iLight->rgbcct_controller.setBrightnessRGB255(map(flashersettings.brightness_min, 0,100, 0,255));


//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor());
//       // }else{
//         UpdateStartingColourWithGetPixel();
//       // }
//       flashersettings.region = EFFECTS_REGION_ANIMATE_ID;
//     break;
//     case EFFECTS_REGION_ANIMATE_ID: //shift along




    
//         // UpdateStartingColourWithGetPixel();

//         // uint16_t index_random = random(0,pCONT_iLight->settings.light_size_count);

//         // HsbColor hsb = SEGMENT.GetPixelColor(index_random);

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


//   //       HsbColor hsb = SEGMENT.GetPixelColor(index_random);

//   //       if(hsb.B < pCONT_iLight->BrtN2F(flashersettings.brightness_max)){
//   //         hsb.B = pCONT_iLight->BrtN2F(flashersettings.brightness_max);
//   //         // animation_colours[index_random].DesiredColour = hsb;
//   //       }

//   //       SEGMENT.SetPixelColor(index_random, hsb);



//   //       // for (uint16_t ii = 0; ii < random_amount; ii++){
//   //       //   SEGMENT.SetPixelColor(
//   //       //     random(0,pCONT_iLight->settings.light_size_count), 
//   //       //     HsbColor(pCONT_iLight->HUE_N2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
//   //       //   );
//   //       // }
//   //     break;
//   //     case 0:
//   //     case 100:
//   //       for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //         SEGMENT.SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
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
//     SEGMENT.SetPixelColor(pixel, updatedColor);
//   } // END for
  

// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
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
//  * *************************************************************************************************************************************************************/

// void mAnimatorLight::SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05()
// {
 
//   // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_NEO "SubTask_Flasher_Animate_Function_SunPositions_Solid_Colour_Based_On_Sun_Elevation_05"));

//   pCONT_iLight->animation.palette_id = mPaletteI->PALETTELIST_VARIABLE_RGBCCT_COLOUR_01_ID;

//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);
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
//   animation_colours_rgbcct.DesiredColour  = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr);
//   pCONT_iLight->animation.flags.fForceUpdate = true;

//   // ALOG_TST(PSTR("DesiredColour1=%d,%d,%d,%d,%d"), animation_colours_rgbcct.DesiredColour.R,animation_colours_rgbcct.DesiredColour.G,animation_colours_rgbcct.DesiredColour.B,animation_colours_rgbcct.DesiredColour.WC,animation_colours_rgbcct.DesiredColour.WW);
    
//   if(!pCONT_iLight->rgbcct_controller.getApplyBrightnessToOutput())
//   { // If not already applied, do it using global values
//     animation_colours_rgbcct.DesiredColour = RgbcctColor::ApplyBrightnesstoRgbcctColour(
//       animation_colours_rgbcct.DesiredColour, 
//       pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(),
//       pCONT_iLight->rgbcct_controller.getBrightnessCCT255()
//     );
//   }

//   animation_colours_rgbcct.StartingColor = SEGMENT.GetPixelColor();

//   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_ANIMATE"));
//   this->setAnimFunctionCallback([this](const AnimationParam& param){
//       this->AnimationProcess_Generic_RGBCCT_Single_Colour_All(param); });
   
// }


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Sequential
 * @note : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random(){

//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID:{ //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
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
//   uint8_t brightness_as_percentage = map(pCONT_iLight->rgbcct_controller.getBrightnessRGB_WithGlobalApplied(), 0,255, 0,100);
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
//           SEGMENT.SetPixelColor(
//             random(0,pCONT_iLight->settings.light_size_count), 
//             HsbColor(pCONT_iLight->HUE_N2F(30),pCONT_iLight->SatN2F(90),pCONT_iLight->BrtN2F(random(0,brightness_as_percentage)))
//           );
//         }
//       break;
//       case 0:
//       case 20:
//       case 40:
//       case 60:
//       case 80: //go back to coloured
//         for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//           SEGMENT.SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
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
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/



// void mAnimatorLight::SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random(){
  
//   switch(flashersettings.region){
//     case EFFECTS_REGION_COLOUR_SELECT_ID: //set colours
//       AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL EFFECTS_COLOUR_SELECT"));
//       // if(flashersettings.flags.enable_endingcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_endingcolour_as_alternate"));
//       //   UpdateDesiredColourWithSingleColour(RgbcctColor());
//       // }
//       UpdateDesiredColourFromPaletteSelected();
//       // if(flashersettings.flags.enable_startcolour_as_alternate){
//       //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_NEO "EFFECTS_SEQUENTIAL flashersettings.flags.enable_startcolour_as_alternate"));
//       //   UpdateStartingColourWithSingleColour(RgbcctColor());
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
//   mPaletteI->SetPaletteListPtrFromID(pCONT_iLight->animation.palette_id);

  
//   // for (uint16_t pixel = 0; pixel < pCONT_iLight->settings.light_size_count; pixel++){
//   //   RgbTypeColor updatedColor = RgbTypeColor::LinearBlend(
//   //       animation_colours[pixel].StartingColor,
//   //       animation_colours[pixel].DesiredColour,
//   //       param.progress);    
//   //   SEGMENT.SetPixelColor(pixel, updatedColor);
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
//           desired_pixel = random(0,GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr));
//           // get colour from palette
//           flash_colour = mPaletteI->GetColourFromPalette(mPaletteI->static_palettes.ptr,desired_pixel,&pixel_position);
//           flash_colour = RgbcctColor::ApplyBrightnesstoRgbcctColour(flash_colour,flashed_brightness);
//           SEGMENT.SetPixelColor(
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
//           SEGMENT.SetPixelColor(pixel, animation_colours[pixel].DesiredColour);
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


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


//each needs 12 bytes
//Spark type is used for popcorn, 1D fireworks, and drip
typedef struct Spark {
  float pos;
  float vel;
  uint16_t col;
  uint8_t colIndex;
} spark;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Tri state pattern
 * @note : Converted from WLED Effects
 * 
 * @param intensity: Controls width of pattern
 * @param Speed    : None
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Tri_Static_Pattern()
{

  uint8_t segSize = (SEGMENT.intensity >> 5) +1;
  uint8_t currSeg = 0;
  uint16_t currSegCount = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) 
  {

    if ( currSeg % 3 == 0 ) {
      SEGMENT.SetPixelColor(i, GetSegmentColour(0, SEGIDX));
    } else if( currSeg % 3 == 1) {
      SEGMENT.SetPixelColor(i, GetSegmentColour(1, SEGIDX));
    } else {
      SEGMENT.SetPixelColor(i, (GetSegmentColour(2, SEGIDX).CalculateBrightness() > 0 ? GetSegmentColour(2, SEGIDX) : WHITE));
    }   

    currSegCount += 1;
    if (currSegCount >= segSize) {
      currSeg +=1;
      currSegCount = 0;
    }

  }

  SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);  

}
static const char PM_EFFECT_CONFIG__TRI_STATIC_PATTERN[] PROGMEM = "Static Pattern Tri@,Size;1,2,3;;;pal=6";



/********************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************
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
*******************************************************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::BaseEffectAnim__Base_Colour_Wipe(bool rev, bool useRandomColors, bool useIterateOverPalette)
{

  // useIterateOverPalette = true;
  // useRandomColors = false;
DEBUG_TIME__START

//speed of 128, cycletime = 19800
  uint32_t cycleTime = 750 + (255 - SEGMENT.speed)*150;
  // millis() % total_cycle_time ==> gives remainder, and therefore process of animation from 0->cycleTime is 0->100%
  uint32_t perc = millis_at_start_of_effect_update % cycleTime;
  // prog gets the process from 0->65535 again
  uint16_t prog = (perc * 65535) / cycleTime;
   
  // ALOG_INF(PSTR(" SEGMENT.speed = %d"),  SEGMENT.speed);
  // ALOG_INF(PSTR("cycleTime = %d"), cycleTime);
  // ALOG_INF(PSTR("perc = %d"), perc);
  // ALOG_INF(PSTR("prog = %d"), prog);


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
      SEGMENT.params_internal.aux0 = random8();
      ALOG_INF(PSTR("random8() %d %d %d"), SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1, SEGMENT.call);
      SEGMENT.step = 3;
    }
    if (SEGMENT.step == 1) { //if flag set, change to new random color
      SEGMENT.params_internal.aux1 = get_random_wheel_index(SEGMENT.params_internal.aux0);
      SEGMENT.step = 2;
    }
    if (SEGMENT.step == 3) {
      SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux1);
      SEGMENT.step = 0;
    }
    // ALOG_INF(PSTR("useRandomColors %d %d %d"), SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1, SEGMENT.call);
  }else
  if(useIterateOverPalette)
  {
  
    uint8_t colours_in_palette = SEGMENT.GetPaletteDiscreteWidth();

    if (SEGMENT.call == 0) {
      SEGMENT.params_internal.aux0 = 0;
      SEGMENT.params_internal.aux1 = 1;
      SEGMENT.step = 3;
    }
    if (SEGMENT.step == 1) { // Reverse
      SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0 + 1; // aux1 should be leading aux0, so the next colour
      SEGMENT.step = 2;
    }
    if (SEGMENT.step == 3) { // Forward
      SEGMENT.params_internal.aux0 = SEGMENT.params_internal.aux1 + 1;
      SEGMENT.step = 0;
    }
    if(SEGMENT.params_internal.aux0 >= colours_in_palette) SEGMENT.params_internal.aux0 = 0; // Wrap/Constrain
    if(SEGMENT.params_internal.aux1 >= colours_in_palette) SEGMENT.params_internal.aux1 = 0; // Wrap/Constrain
    // The wipe colour then needs to become the base colour on the next effect
    // ALOG_INF(PSTR("Colours in palette %d %d %d"), SEGMENT.GetPaletteDiscreteWidth(), SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1);
  }

  uint16_t ledIndex = (prog * SEGLEN) >> 15;
  uint16_t rem = 0;
  rem = (prog * SEGLEN) * 2; //mod 0xFFFF
  rem /= (SEGMENT.intensity +1);
  if (rem > 255) rem = 255;

  uint32_t col_wipe = 0;
  if(useRandomColors)
  {
    col_wipe = color_wheel(SEGMENT.params_internal.aux1);
  }
  else
  if(useIterateOverPalette)
  {
    col_wipe = SEGMENT.GetPaletteColour(SEGMENT.params_internal.aux1, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE).getU32();
  }
  else
  {
    col_wipe = SEGCOLOR_U32(1);
  }

  // ALOG_INF(PSTR("aux0 %d aux1 %d"), SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1);

  // DEBUG_TIME__RESET
  
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint16_t indexPixel = (rev && back) ? SEGLEN -1 -i : i;

    // uint32_t col0 = useRandomColors ? color_wheel(SEGMENT.params_internal.aux0) : SEGMENT.GetPaletteColour(indexPixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    
    uint32_t col_base = 0;
    if(useRandomColors)
    {
      col_base = color_wheel(SEGMENT.params_internal.aux0);
    }
    else
    if(useIterateOverPalette)
    {
      col_base = SEGMENT.GetPaletteColour(SEGMENT.params_internal.aux0, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE).getU32();
    }
    else
    {
      col_base = SEGMENT.GetPaletteColour(indexPixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE).getU32();
    }

    if (i < ledIndex) 
    {
      SEGMENT.SetPixelColor(indexPixel, back ? col_wipe : col_base);
    } else
    {
      SEGMENT.SetPixelColor(indexPixel, back ? col_base : col_wipe);
      if (i == ledIndex) SEGMENT.SetPixelColor(indexPixel, ColourBlend(back ? col_base : col_wipe, back ? col_wipe : col_base, rem));
    }
  } 
  // DEBUG_TIME__SHOW_MESSAGE("wrtcols")


  SetSegment_AnimFunctionCallback_WithoutAnimator(SEGIDX);

  DEBUG_TIME__SHOW

}






/*
 * Lights all LEDs one after another.
 */
void mAnimatorLight::EffectAnim__Colour_Wipe()
{
  BaseEffectAnim__Base_Colour_Wipe(false, false);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE[] PROGMEM = "Wipe Over@!,!;!,!;!";

/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
void mAnimatorLight::EffectAnim__Colour_Wipe_Random()
{
  BaseEffectAnim__Base_Colour_Wipe(false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE_RANDOM[] PROGMEM = "Wipe Random@!;;!";

/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
void mAnimatorLight::EffectAnim__Colour_Wipe_Palette()
{
  BaseEffectAnim__Base_Colour_Wipe(false, false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_WIPE_PALETTE[] PROGMEM = "Wipe Palette@!;;!";


/*
 * Lights all LEDs one after another. Swipe is wipe, but back and forth
 */
void mAnimatorLight::EffectAnim__Colour_Sweep()
{
  BaseEffectAnim__Base_Colour_Wipe(true, false);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP[] PROGMEM = "Sweep Over@!,!;!,!;!";


/*
 * Random color introduced alternating from start and end of strip->
 */
void mAnimatorLight::EffectAnim__Colour_Sweep_Random()
{
  BaseEffectAnim__Base_Colour_Wipe(true, true);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP_RANDOM[] PROGMEM = "Sweep Random@!;;!";

/*
 * Random color introduced alternating from start and end of strip->
 */
void mAnimatorLight::EffectAnim__Colour_Sweep_Palette()
{
  BaseEffectAnim__Base_Colour_Wipe(true, false, true);
}
static const char PM_EFFECT_CONFIG__COLOR_SWEEP_PALETTE[] PROGMEM = "Sweep Palette@!;;!";




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
uint32_t mAnimatorLight::color_wheel(uint8_t pos) {

#ifdef ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED

  // if (SEGMENT.palette_id){ // when default, so it skips this, causes brightness error
  //   // Again this assumes palette colour index is ranged 0 to 255, so I NEED to fix mine to be the same
  //   return SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
  // }else
  // {

  //   ALOG_ERR(PSTR("Likely brightness error"));
  
    pos = 255 - pos;
    if(pos < 85) {
      return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
    } else if(pos < 170) {
      pos -= 85;
      return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
    } else {
      pos -= 170;
      return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
    }

  // }


#else
  float hue_f = mSupport::mapfloat(pos, 0, 255, 0.0f, 1.0f);
  HsbColor colh = HsbColor(hue_f,1,1);
  return RgbcctColor::GetU32Colour(colh);
#endif



}

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Chase
 ****************************************************************************************************************************
 ****************************************************************************************************************************/

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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

void mAnimatorLight::EffectAnim__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{
  
// void mAnimatorLight::chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette) {
  uint16_t counter = millis() * ((SEGMENT.speed >> 2) + 1);
  uint16_t a = counter * SEGLEN  >> 16;

  bool chase_random = (SEGMENT.effect_id == EFFECTS_FUNCTION__CHASE_RANDOM__ID);
  if (chase_random) {
    if (a < SEGMENT.step) //we hit the start again, choose new color for Chase random
    {
      SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0; //store previous random color
      SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0);
    }
    color1 = color_wheel(SEGMENT.params_internal.aux0);
  }
  SEGMENT.step = a;

  // Use intensity setting to vary chase up to 1/2 string length
  uint8_t size = 1 + (SEGMENT.intensity * SEGLEN >> 10);

  uint16_t b = a + size; //"trail" of chase, filled with color1 
  if (b > SEGLEN) b -= SEGLEN;
  uint16_t c = b + size;
  if (c > SEGLEN) c -= SEGLEN;

  //background
  if (do_palette)
  {
    for(uint16_t i = 0; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  } else fill(color1);

  //if random, fill old background between a and end
  if (chase_random)
  {
    color1 = color_wheel(SEGMENT.params_internal.aux1);
    for (uint16_t i = a; i < SEGLEN; i++)
      SEGMENT.SetPixelColor(i, color1);
  }

  //fill between points a and b with color2
  if (a < b)
  {
    for (uint16_t i = a; i < b; i++)
      SEGMENT.SetPixelColor(i, color2);
  } else {
    for (uint16_t i = a; i < SEGLEN; i++) //fill until end
      SEGMENT.SetPixelColor(i, color2);
    for (uint16_t i = 0; i < b; i++) //fill from start until b
      SEGMENT.SetPixelColor(i, color2);
  }

  //fill between points b and c with color2
  if (b < c)
  {
    for (uint16_t i = b; i < c; i++)
      SEGMENT.SetPixelColor(i, color3);
  } else {
    for (uint16_t i = b; i < SEGLEN; i++) //fill until end
      SEGMENT.SetPixelColor(i, color3);
    for (uint16_t i = 0; i < c; i++) //fill from start until c
      SEGMENT.SetPixelColor(i, color3);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/*
 * Bicolor chase, more primary color.
 */
// void mAnimatorLight::mode_chase_color(void) {
//   return chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), true);
// }

void mAnimatorLight::EffectAnim__Chase_Colour()
{
  EffectAnim__Base_Chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), true);
}
static const char PM_EFFECT_CONFIG__CHASE_COLOR[] PROGMEM = "Chase@!,Width;!,!,!;!";



/*
 * Primary running followed by random color.
 */
// void mAnimatorLight::mode_chase_random(void) {
//   return chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), false);
// }

// void mAnimatorLight::EffectAnim__Chase_Random()
// {
//   EffectAnim__Base_Chase(SEGCOLOR_U32(1), (SEGCOLOR_U32(2)) ? SEGCOLOR_U32(2) : SEGCOLOR_U32(0), SEGCOLOR_U32(0), false);
// }

/*
 * Primary, secondary running on rainbow.
 */
// void mAnimatorLight::mode_chase_rainbow(void) {
//   return chase();
// }
void mAnimatorLight::EffectAnim__Chase_Rainbow()
{
  uint8_t color_sep = 256 / SEGLEN;
  uint8_t color_index = SEGMENT.call & 0xFF;
  uint32_t color = color_wheel(((SEGMENT.step * color_sep) + color_index) & 0xFF);

  EffectAnim__Base_Chase(color, SEGCOLOR_U32(0), SEGCOLOR_U32(1), false);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__CHASE_RAINBOW[] PROGMEM = "Chase Rainbow@!,Width;!,!;!";


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

void mAnimatorLight::EffectAnim__Chase_Flash()
// Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette)
{
  uint8_t flash_step = SEGMENT.call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
  }

  uint16_t delay = 10 + ((30 * (uint16_t)(255 - SEGMENT.speed)) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    if(flash_step % 2 == 0) {
      uint16_t n = SEGMENT.step;
      uint16_t m = (SEGMENT.step + 1) % SEGLEN;
      SEGMENT.SetPixelColor( n, SEGCOLOR_U32(1));
      SEGMENT.SetPixelColor( m, SEGCOLOR_U32(1));
      delay = 20;
    } else {
      delay = 30;
    }
  } else {
    SEGMENT.step = (SEGMENT.step + 1) % SEGLEN;
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__CHASE_FLASH[] PROGMEM = "Chase Flash@!;Bg,Fx;!";


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

void mAnimatorLight::EffectAnim__Chase_Flash_Random()
{
  uint8_t flash_step = SEGMENT.call % ((FLASH_COUNT * 2) + 1);

  for(uint16_t i = 0; i < SEGMENT.step; i++) {
    SEGMENT.SetPixelColor(i, color_wheel(SEGMENT.params_internal.aux0));
  }

  uint16_t delay = 1 + ((10 * (uint16_t)(255 - SEGMENT.speed)) / SEGLEN);
  if(flash_step < (FLASH_COUNT * 2)) {
    uint16_t n = SEGMENT.step;
    uint16_t m = (SEGMENT.step + 1) % SEGLEN;
    if(flash_step % 2 == 0) {
      SEGMENT.SetPixelColor( n, SEGCOLOR_U32(0));
      SEGMENT.SetPixelColor( m, SEGCOLOR_U32(0));
      delay = 20;
    } else {
      SEGMENT.SetPixelColor( n, color_wheel(SEGMENT.params_internal.aux0));
      SEGMENT.SetPixelColor( m, SEGCOLOR_U32(1));
      delay = 30;
    }
  } else {
    SEGMENT.step = (SEGMENT.step + 1) % SEGLEN;

    if(SEGMENT.step == 0) {
      SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0);
    }
  }
  // return delay;

  SEGMENT.animation_has_anim_callback = false; // When no animation callback is needed
          Serial.print("@4");
//  pCONT_iLight->ShowInterface();
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__CHASE_FLASH_RANDOM[] PROGMEM = "Chase Flash Rnd@!;!,!;!";


// /*
//  * Primary running on rainbow.
//  */
// void mAnimatorLight::mode_chase_rainbow_white(void) {
//   uint16_t n = SEGMENT.step;
//   uint16_t m = (SEGMENT.step + 1) % SEGLEN;
//   uint32_t color2 = color_wheel(((n * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);
//   uint32_t color3 = color_wheel(((m * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);

//   return chase(SEGCOLOR_U32(0), color2, color3, false);
// }
void mAnimatorLight::EffectAnim__Chase_Rainbow_White()
{

  uint16_t n = SEGMENT.step;
  uint16_t m = (SEGMENT.step + 1) % SEGLEN;
  uint32_t color2 = color_wheel(((n * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);
  uint32_t color3 = color_wheel(((m * 256 / SEGLEN) + (SEGMENT.call & 0xFF)) & 0xFF);

  EffectAnim__Base_Chase(SEGCOLOR_U32(0), color2, color3, false);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__CHASE_RAINBOW_WHITE[] PROGMEM = "Rainbow Runner@!,Size;Bg;!";


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

void mAnimatorLight::EffectAnim__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette)
{


  byte gap = 2 + ((255 - SEGMENT.intensity) >> 5);
  uint32_t cycleTime = 50 + (255 - SEGMENT.speed)*2;
  uint32_t it = millis() / cycleTime;
  if (it != SEGMENT.step) //new color
  {
    SEGMENT.params_internal.aux0 = (SEGMENT.params_internal.aux0 +1) % gap;
    SEGMENT.step = it;
  }
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    if((i % gap) == SEGMENT.params_internal.aux0) {
      if (do_palette)
      {
        SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
      } else {
        SEGMENT.SetPixelColor(i, color1);
      }
    } else {
      SEGMENT.SetPixelColor(i, color2);
    }
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  return;
}


/*
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 */
void mAnimatorLight::EffectAnim__Chase_Theater(void) {
  return EffectAnim__Base_Chase_Theater(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true);
}
static const char PM_EFFECT_CONFIG__THEATER_CHASE[] PROGMEM = "Theater@!,Gap size;!,!;!";


/*
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 */
void mAnimatorLight::EffectAnim__Chase_Theatre_Rainbow(void) {
  return EffectAnim__Base_Chase_Theater(color_wheel(SEGMENT.step), SEGCOLOR_U32(1), false);
}
static const char PM_EFFECT_CONFIG__THEATER_CHASE_RAINBOW[] PROGMEM = "Theater Rainbow@!,Gap size;,!;!";




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

void mAnimatorLight::EffectAnim__Base_Chase_TriColour(uint32_t color1, uint32_t color2)
{
  
  uint32_t cycleTime = 50 + (255 - SEGMENT.speed)*2;
  uint32_t it = millis() / cycleTime;
  uint8_t width = (1 + SEGMENT.intensity/32) * 3; //value of 1-8 for each colour
  uint8_t index = it % width;
  
  for(uint16_t i = 0; i < SEGLEN; i++, index++) {
    if(index > width-1) index = 0;

    uint32_t color = color1;
    if(index > width*2/3-1){
      // color = RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)); // Functionally the same
      color = SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON).getU32();      
    }
    else if(index > width/3-1) color = color2;

    SEGMENT.SetPixelColor(SEGLEN - i -1, color);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}


/*
 * Tricolor chase mode
 */
void mAnimatorLight::EffectAnim__Chase_TriColour(void) {

  return EffectAnim__Base_Chase_TriColour(SEGCOLOR_U32(2), SEGCOLOR_U32(0));
}
static const char PM_EFFECT_CONFIG__TRICOLOR_CHASE[] PROGMEM = "Tri Chase@!,Size;1,2,3;!";


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

void mAnimatorLight::EffectAnim__Chase_Random()
{

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.step == it){return;}// return FRAMETIME_MS;

  for(uint16_t i = SEGLEN -1; i > 0; i--) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPixelColor(i-1));
  }
  uint32_t color = SEGMENT.GetPixelColor(0).getU32();
  if (SEGLEN > 1) color = SEGMENT.GetPixelColor(1).getU32();
  uint8_t r = random8(6) != 0 ? (color >> 16 & 0xFF) : random8();
  uint8_t g = random8(6) != 0 ? (color >> 8  & 0xFF) : random8();
  uint8_t b = random8(6) != 0 ? (color       & 0xFF) : random8();
  SEGMENT.SetPixelColor(0, r, g, b);

  SEGMENT.step = it;
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  return;
}
static const char PM_EFFECT_CONFIG__CHASE_RANDOM[] PROGMEM = "Stream 2@!,Width;!,,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effect
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Breath()
{

  uint16_t var = 0;
  uint16_t counter = (millis() * ((SEGMENT.speed >> 3) +10));
  counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
  if (counter < 16384) {
    if (counter > 8192) counter = 8192 - (counter - 8192);
    var = sin16(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
  }
  
  uint8_t lum = 30 + var;
  for(uint16_t i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON), lum) );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__BREATH[] PROGMEM = "Breath@!;!,!;!;01";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fade
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Fade()
{
  uint16_t counter = (millis() * ((SEGMENT.speed >> 3) +10));
  uint8_t lum = triwave16(counter) >> 8;

  for(uint16_t i = 0; i < SEGLEN; i++)
  {
    SEGMENT.SetPixelColor(
      i, 
      ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON), lum)
    );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FADE[] PROGMEM = "Fade@!;!,!;!;01";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks Base Function
 * @note : Converted from WLED Effects
 * 
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Fireworks()
{
  
  const uint16_t width  = SEGMENT.is2D() ? SEGMENT.virtualWidth() : SEGMENT.virtualLength();
  const uint16_t height = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.setUpLeds();  // lossless getPixelColor()
    SEGMENT.fill(SEGCOLOR_U32(1));
    SEGMENT.params_internal.aux0 = UINT16_MAX;
    SEGMENT.params_internal.aux1 = UINT16_MAX;
  }

  SEGMENT.fade_out(128);

  bool valid1 = (SEGMENT.params_internal.aux0 < width*height);
  bool valid2 = (SEGMENT.params_internal.aux1 < width*height);
  uint32_t sv1 = 0, sv2 = 0;
  if (valid1) sv1 = SEGMENT.is2D() ? SEGMENT.getPixelColorXY(SEGMENT.params_internal.aux0%width, SEGMENT.params_internal.aux0/width) : SEGMENT.getPixelColor(SEGMENT.params_internal.aux0); // get spark color
  if (valid2) sv2 = SEGMENT.is2D() ? SEGMENT.getPixelColorXY(SEGMENT.params_internal.aux1%width, SEGMENT.params_internal.aux1/width) : SEGMENT.getPixelColor(SEGMENT.params_internal.aux1);
  // if (!SEGMENT.step) SEGMENT.blur(16);
  if (valid1) { if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(SEGMENT.params_internal.aux0%width, SEGMENT.params_internal.aux0/width, sv1); else SEGMENT.setPixelColor(SEGMENT.params_internal.aux0, sv1); } // restore spark color after blur
  if (valid2) { if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(SEGMENT.params_internal.aux1%width, SEGMENT.params_internal.aux1/width, sv2); else SEGMENT.setPixelColor(SEGMENT.params_internal.aux1, sv2); } // restore old spark color after blur

  for (int i=0; i<MAX(1, width/20); i++) {
    if (random8(129 - (SEGMENT.intensity >> 1)) == 0) {
      uint16_t index = random16(width*height);
      uint16_t j = index % width, k = index / width;
      uint32_t col = SEGMENT.GetPaletteColour(random8(), false, false, 0).getU32();
      if (SEGMENT.is2D()) SEGMENT.setPixelColorXY(j, k, col, SET_BRIGHTNESS);
      else                SEGMENT.setPixelColor(index, col, SET_BRIGHTNESS);
      SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0;  // old spark
      SEGMENT.params_internal.aux0 = index;        // remember where spark occured
    }
  }
  
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FIREWORKS[] PROGMEM = "Fireworks@,Frequency;!,!;!;12;ix=137," EFFECT_CONFIG_DEFAULT_OPTION__PALETTE_INDEX_CTR ""; // ix(itensity)=192, pal=randomise01


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Fireworks Starburst
 * @note : Converted from WLED Effects
 * 
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
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

void mAnimatorLight::EffectAnim__Fireworks_Starburst()
{

  uint8_t numStars = 1 + (SEGLEN >> 3);
  if (numStars > 15) numStars = 15;
  uint16_t dataSize = sizeof(star) * numStars;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }
  
  uint32_t it = millis();
  
  star* stars = reinterpret_cast<star*>(SEGMENT.data);
  
  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time
     
  for (int j = 0; j < numStars; j++)
  {
    // speed to adjust chance of a burst, max is nearly always.
    if (random8((144-(SEGMENT.speed >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.  
      uint16_t startPos = random16(SEGLEN-1);
      float multiplier = (float)(random8())/255.0 * 1.0;

      stars[j].color = col_to_crgb(color_wheel(random8()));
      stars[j].pos = startPos; 
      stars[j].vel = maxSpeed * (float)(random8())/255.0 * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = random8(3,6 + (SEGMENT.intensity >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }
  
  fill(SEGCOLOR_U32(1), SET_BRIGHTNESS);
  
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
      c = col_to_crgb(ColourBlend(WHITE, crgb_to_col(c), 254.5f*((age / particleIgnition))));
    } else {
      // Figure out how much to fade and shrink the star based on 
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = col_to_crgb(SEGCOLOR_U32(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        byte f = 254.5f*fade;
        c = col_to_crgb(ColourBlend(crgb_to_col(c), SEGCOLOR_U32(1), f));
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
        if (end > SEGLEN) end = SEGLEN;    
        for (int p = start; p < end; p++) {
          SEGMENT.SetPixelColor(p, c.r, c.g, c.b);
        }
      }
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


static const char PM_EFFECT_CONFIG__STARBURST[] PROGMEM = "Fireworks Starburst@Chance,Fragments,,,,,,,Overlay;,!;!;;" EFFECT_CONFIG_DEFAULT_OPTION__PALETTE_INDEX_CTR "";


/**
 * @brief To introduce much slower fade out, have it burst and fade out very slowly
 * 
 */
void mAnimatorLight::EffectAnim__Fireworks_Starburst_Glows()
{
 
  uint8_t numStars = 1 + (SEGLEN >> 3);
  if (numStars > 15) numStars = 15;
  uint16_t dataSize = sizeof(star) * numStars;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }
  
  uint32_t it = millis();
  
  star* stars = reinterpret_cast<star*>(SEGMENT.data);
  
  float          maxSpeed                = 375.0f;  // Max velocity
  float          particleIgnition        = 250.0f;  // How long to "flash"
  float          particleFadeTime        = 1500.0f; // Fade out time
     
  for (int j = 0; j < numStars; j++)
  {
    // speed to adjust chance of a burst, max is nearly always.
    if (random8((144-(SEGMENT.speed >> 1))) == 0 && stars[j].birth == 0)
    {
      // Pick a random color and location.  
      uint16_t startPos = random16(SEGLEN-1);
      float multiplier = (float)(random8())/255.0 * 1.0;

      stars[j].color = col_to_crgb(color_wheel(random8()));
      stars[j].pos = startPos; 
      stars[j].vel = maxSpeed * (float)(random8())/255.0 * multiplier;
      stars[j].birth = it;
      stars[j].last = it;
      // more fragments means larger burst effect
      int num = random8(3,6 + (SEGMENT.intensity >> 5));

      for (int i=0; i < STARBURST_MAX_FRAG; i++) {
        if (i < num) stars[j].fragment[i] = startPos;
        else stars[j].fragment[i] = -1;
      }
    }
  }
  
  fill(SEGCOLOR_U32(1), SET_BRIGHTNESS);
  
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
      c = col_to_crgb(ColourBlend(WHITE, crgb_to_col(c), 254.5f*((age / particleIgnition))));
    } else {
      // Figure out how much to fade and shrink the star based on 
      // its age relative to its lifetime
      if (age > particleIgnition + particleFadeTime) {
        fade = 1.0f;                  // Black hole, all faded out
        stars[j].birth = 0;
        c = col_to_crgb(SEGCOLOR_U32(1));
      } else {
        age -= particleIgnition;
        fade = (age / particleFadeTime);  // Fading star
        byte f = 254.5f*fade;
        c = col_to_crgb(ColourBlend(crgb_to_col(c), SEGCOLOR_U32(1), f));
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
        if (end > SEGLEN) end = SEGLEN;    
        for (int p = start; p < end; p++) {
          SEGMENT.SetPixelColor(p, c.r, c.g, c.b);
        }
      }
    }
  }

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__STARBURST_GLOWS[] PROGMEM = "Fireworks Starburst Glows@Chance,Fragments,,,,,,,Overlay;,!;!;" EFFECT_CONFIG_DEFAULT_OPTION__PALETTE_INDEX_CTR ",m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effects
 *
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Exploding_Fireworks()
{

  //allocate segment data
  uint16_t numSparks = 2 + (SEGLEN >> 1); 
  if (numSparks > 80) numSparks = 80;

  uint16_t dataSize = sizeof(spark) * numSparks;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }

  fill(BLACK, SET_BRIGHTNESS);
  
  bool actuallyReverse = SEGMENT.getOption(SEG_OPTION_REVERSED);
  //have fireworks start in either direction based on intensity
  SEGMENT.setOption(SEG_OPTION_REVERSED, SEGMENT.step);
  
  Spark* sparks = reinterpret_cast<Spark*>(SEGMENT.data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004 - (SEGMENT.speed/800000.0); // m/s/s
  gravity *= SEGLEN;
  
  if (SEGMENT.params_internal.aux0 < 2) { //FLARE
    if (SEGMENT.params_internal.aux0 == 0) { //init flare
      flare->pos = 0;
      uint16_t peakHeight = 75 + random8(180); //0-255
      peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
      flare->vel = sqrt(-2.0 * gravity * peakHeight);
      flare->col = 255; //brightness
      SEGMENT.params_internal.aux0 = 1; 
    }
    
    // launch 
    if (flare->vel > 12 * gravity) {
      // flare
      SEGMENT.SetPixelColor(int(flare->pos),flare->col,flare->col,flare->col);
  
      flare->pos += flare->vel;
      flare->pos = constrain(flare->pos, 0, SEGLEN-1);
      flare->vel += gravity;
      flare->col -= 2;
    } else {
      SEGMENT.params_internal.aux0 = 2;  // ready to explode
    }
  } else if (SEGMENT.params_internal.aux0 < 4) {
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
    if (SEGMENT.params_internal.aux0 == 2) {
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos = flare->pos; 
        sparks[i].vel = (float(random16(0, 20000)) / 10000.0) - 0.9; // from -0.9 to 1.1
        sparks[i].col = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright 
        //sparks[i].col = constrain(sparks[i].col, 0, 345); 
        sparks[i].colIndex = random8();
        sparks[i].vel *= flare->pos/SEGLEN; // proportional to height 
        sparks[i].vel *= -gravity *50;
      } 
      //sparks[1].col = 345; // this will be our known spark 
      dying_gravity = gravity/2; 
      SEGMENT.params_internal.aux0 = 3;
    }
  
    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos += sparks[i].vel; 
        sparks[i].vel += dying_gravity; 
        if (sparks[i].col > 3) sparks[i].col -= 4; 

        if (sparks[i].pos > 0 && sparks[i].pos < SEGLEN) {
          uint16_t prog = sparks[i].col;
          uint32_t spColor = (SEGMENT.palette_id) ? color_wheel(sparks[i].colIndex) : SEGCOLOR_U32(0);
          // uint32_t spColor = RgbcctColor::GetU32Colour(RgbColor(0,255,10));
          CRGB c = HTMLColorCode::Black; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = col_to_crgb(ColourBlend(spColor, WHITE, (prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = col_to_crgb(ColourBlend(BLACK, spColor, prog - 45));
            uint8_t cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          SEGMENT.SetPixelColor(int(sparks[i].pos), c.red, c.green, c.blue);
        }
      }
      dying_gravity *= .99; // as sparks burn out they fall slower
    } else {
      SEGMENT.params_internal.aux0 = 6 + random8(10); //wait for this many frames
    }
  } else {
    SEGMENT.params_internal.aux0--;
    if (SEGMENT.params_internal.aux0 < 4) {
      SEGMENT.params_internal.aux0 = 0; //back to flare
      SEGMENT.step = (SEGMENT.intensity > random8()); //decide firing side
    }
  }

  SEGMENT.setOption(SEG_OPTION_REVERSED, actuallyReverse);
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__EXPLODING_FIREWORKS[] PROGMEM = "Fireworks 1D@Gravity,Firing side;!,!;!;12;pal=10,ix=128";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Exploding fireworks effect
 * @note : Converted from WLED Effects
 * 
 * Needs edited so the "launch" portion is properly skipped (ie no time with no pixels on)
 *
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 * @param aux0 Current center index of firework (which blurs outwards)
 * @param aux1 Previous location of center index of firework
 * @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Exploding_Fireworks_NoLaunch()
{

  //allocate segment data
  uint16_t numSparks = 2 + (SEGLEN >> 1); 
  if (numSparks > 80) numSparks = 80;

  uint16_t dataSize = sizeof(spark) * numSparks;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = DEFAULT_EFFECTS_FUNCTION;
    return;
  }

  fill(BLACK, SET_BRIGHTNESS);
  
  bool actuallyReverse = SEGMENT.getOption(SEG_OPTION_REVERSED);
  //have fireworks start in either direction based on intensity
  SEGMENT.setOption(SEG_OPTION_REVERSED, SEGMENT.step);
  
  Spark* sparks = reinterpret_cast<Spark*>(SEGMENT.data);
  Spark* flare = sparks; //first spark is flare data

  float gravity = -0.0004 - (SEGMENT.speed/800000.0); // m/s/s
  gravity *= SEGLEN;
  
  if (SEGMENT.params_internal.aux0 < 2) 
  { //FLARE

    // if (SEGMENT.params_internal.aux0 == 0) { //init flare
      flare->pos = 0;
      uint16_t peakHeight = 75 + random8(180); //0-255
      peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
      flare->vel = sqrt(-2.0 * gravity * peakHeight);
      flare->col = 255; //brightness
      SEGMENT.params_internal.aux0 = 1; 
    // }
    
    // // launch 
    // if (flare->vel > 12 * gravity) {
    //   // flare
    //   SEGMENT.SetPixelColor(int(flare->pos),flare->col,flare->col,flare->col);
  
      flare->pos += flare->vel;
      flare->pos = constrain(flare->pos, 0, SEGLEN-1);
      flare->vel += gravity;
      flare->col -= 2;

      /**
       * @brief Starts in the middle
       **/
      // flare->pos = SEGLEN/2;
      /**
       * @brief Starts randomly
       **/
      flare->pos = random(0,SEGLEN);

    // } else {
      SEGMENT.params_internal.aux0 = 2;  // ready to explode
    // }

  }
  
  
  // else // REMOVING ELSE SO THE FLARE TO EXPLODE IS THE SAME FRAME
  
  
  
  if (SEGMENT.params_internal.aux0 < 4) 
  {
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
    if (SEGMENT.params_internal.aux0 == 2) {
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos = flare->pos; 
        sparks[i].vel = (float(random16(0, 20000)) / 10000.0) - 0.9; // from -0.9 to 1.1
        sparks[i].col = 345;//abs(sparks[i].vel * 750.0); // set colors before scaling velocity to keep them bright 
        //sparks[i].col = constrain(sparks[i].col, 0, 345); 
        sparks[i].colIndex = random8();
        sparks[i].vel *= flare->pos/SEGLEN; // proportional to height 
        sparks[i].vel *= -gravity *50;
      } 
      //sparks[1].col = 345; // this will be our known spark 
      dying_gravity = gravity/2; 
      SEGMENT.params_internal.aux0 = 3;
    }
  
    if (sparks[1].col > 4) {//&& sparks[1].pos > 0) { // as long as our known spark is lit, work with all the sparks
      for (int i = 1; i < nSparks; i++) { 
        sparks[i].pos += sparks[i].vel; 
        sparks[i].vel += dying_gravity; 
        if (sparks[i].col > 3) sparks[i].col -= 4; 

        if (sparks[i].pos > 0 && sparks[i].pos < SEGLEN) {
          uint16_t prog = sparks[i].col;
          uint32_t spColor = (SEGMENT.palette_id) ? color_wheel(sparks[i].colIndex) : SEGCOLOR_U32(0);
          // uint32_t spColor = RgbcctColor::GetU32Colour(RgbColor(0,255,10));
          CRGB c = HTMLColorCode::Black; //HeatColor(sparks[i].col);
          if (prog > 300) { //fade from white to spark color
            c = col_to_crgb(ColourBlend(spColor, WHITE, (prog - 300)*5));
          } else if (prog > 45) { //fade from spark color to black
            c = col_to_crgb(ColourBlend(BLACK, spColor, prog - 45));
            uint8_t cooling = (300 - prog) >> 5;
            c.g = qsub8(c.g, cooling);
            c.b = qsub8(c.b, cooling * 2);
          }
          SEGMENT.SetPixelColor(int(sparks[i].pos), c.red, c.green, c.blue);
        }
      }
      dying_gravity *= .99; // as sparks burn out they fall slower
    } else {
      SEGMENT.params_internal.aux0 = 6 + random8(10); //wait for this many frames
    }
  } 
  else 
  {
    SEGMENT.params_internal.aux0--;
    if (SEGMENT.params_internal.aux0 < 4) {
      SEGMENT.params_internal.aux0 = 0; //back to flare
      SEGMENT.step = (SEGMENT.intensity > random8()); //decide firing side
    }
  }

  SEGMENT.setOption(SEG_OPTION_REVERSED, actuallyReverse);
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__EXPLODING_FIREWORKS_NOLAUNCH[] PROGMEM = "Fireworks 1D No Launch@Gravity,Firing side;!,!;!;12;pal=10,ix=128";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
 * Exploding fireworks effect
 * adapted from: http://www.anirama.com/1000leds/1d-fireworks/
 *
 * Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 @param aux0 Current center index of firework (which blurs outwards)
 @param aux1 Previous location of center index of firework
 @param aux2 Iter to wrap palette
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Rain()
{
  
  SEGMENT.step += FRAMETIME_MS;
  if (SEGMENT.step > SPEED_FORMULA_L) 
  {
    SEGMENT.step = 0;
    //shift all leds right
    RgbcctColor ctemp = SEGMENT.GetPixelColor(0);
    for(uint16_t i = 0; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPixelColor(i+1));
    }
    SEGMENT.SetPixelColor(SEGLEN - 1, ctemp);
    SEGMENT.params_internal.aux0++;
    SEGMENT.params_internal.aux1++;
    if (SEGMENT.params_internal.aux0 == 0)      SEGMENT.params_internal.aux0 = UINT16_MAX;
    if (SEGMENT.params_internal.aux1 == 0)      SEGMENT.params_internal.aux0 = UINT16_MAX;
    if (SEGMENT.params_internal.aux0 == SEGLEN) SEGMENT.params_internal.aux0 = 0;
    if (SEGMENT.params_internal.aux1 == SEGLEN) SEGMENT.params_internal.aux1 = 0;
  }
  
  EffectAnim__Fireworks();
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();  

}
static const char PM_EFFECT_CONFIG__RAIN[] PROGMEM = "Rain@!,Spawning rate;!,!;!;12;ix=128";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

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
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *
 *******************************************************************************************************************************************************************************************************************
 ******************************************************************************************************************/
void mAnimatorLight::EffectAnim__Sparkle() // Firework_Rain
{
  
  for(uint16_t i = 0; i < SEGLEN; i++) 
  {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON));
  }
  uint32_t cycleTime = 10 + (255 - SEGMENT.speed)*2;
  uint32_t it = millis() / cycleTime;
  if (it != SEGMENT.step)
  {
    SEGMENT.params_internal.aux0 = random16(SEGLEN); // aux0 stores the random led index
    SEGMENT.step = it;
  }
  
  SEGMENT.SetPixelColor(SEGMENT.params_internal.aux0, SEGCOLOR_RGBCCT(0));  

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SPARKLE[] PROGMEM = "Sparkle@!,,,,,,,,Overlay;!,!;!;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : 
 * @note : 
 * 
 * Lights all LEDs in the color. Flashes single col 1 pixels randomly. (List name: Sparkle Dark)
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Sparkle_Flash() // Firework_Rain
{
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON));
  }

  if(random8(5) == 0) {
    SEGMENT.params_internal.aux0 = random16(SEGLEN); // aux0 stores the random led index
    SEGMENT.SetPixelColor(SEGMENT.params_internal.aux0, SEGCOLOR_RGBCCT(1));
    SEGMENT.cycle_time__rate_ms = 20;//return 20;
  } 
  SEGMENT.cycle_time__rate_ms =  20 + (uint16_t)(255-SEGMENT.speed);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FLASH_SPARKLE[] PROGMEM = "Sparkle Dark@!,!,,,,,,,Overlay;Bg,Fx;!;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * 
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
   Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Sparkle_Hyper() // Firework_Rain
{

  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON));
  }

  if(random8(5) < 2) {
    for(uint16_t i = 0; i < MAX(1, SEGLEN/3); i++) {
      SEGMENT.SetPixelColor(random16(SEGLEN), SEGCOLOR_U32(1));
    }
    SEGMENT.cycle_time__rate_ms =  20;
  }
  SEGMENT.cycle_time__rate_ms =  20 + (uint16_t)(255-SEGMENT.speed);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HYPER_SPARKLE[] PROGMEM = "Sparkle+@!,!,,,,,,,Overlay;Bg,Fx;!;;m12=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Rain effect, which uses firework
 * @note : Converted from WLED Effects
 * Twinkling LEDs running. Inspired by https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/Rain.h
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Twinkle_Up() // Firework_Rain
{
  
  random16_set_seed(535);                                 // The randomizer needs to be re-set each time through the loop in order for the same 'random' numbers to be the same each time through.

  for (int i = 0; i<SEGLEN; i++) {
    uint8_t ranstart = random8();                         // The starting value (aka brightness) for each pixel. Must be consistent each time through the loop for this to work.
    uint8_t pixBri = sin8(ranstart + 16 * millis()/(256-SEGMENT.speed));
    if (random8() > SEGMENT.intensity) pixBri = 0;
    // SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_U32(1), RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i*20, nullptr, false, PALETTE_SOLID_WRAP, 0)), pixBri));
    // CustomPalettes not working now since i*20 is not scaled into 255, so only my first pixel is showing
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON), pixBri) );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TWINKLE_UP[] PROGMEM = "Twinkle Up@!,Intensity;!,!;!;;m12=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effect 
 * Lights all LEDs in one random color up. Then switches them to the next random color.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Random_Colour()
{
  uint32_t cycleTime = 200 + (255 - SEGMENT.speed)*50;
  uint32_t it = millis() / cycleTime;
  uint32_t rem = millis() % cycleTime;
  uint16_t fadedur = (cycleTime * SEGMENT.intensity) >> 8;

  uint32_t fade = 255;
  if (fadedur) {
    fade = (rem * 255) / fadedur;
    if (fade > 255) fade = 255;
  }

  if (SEGMENT.call == 0) {
    SEGMENT.params_internal.aux0 = random8();
    SEGMENT.step = 2;
  }
  if (it != SEGMENT.step) //new color
  {
    SEGMENT.params_internal.aux1 = SEGMENT.params_internal.aux0;
    SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0); //aux0 will store our random color wheel index
    SEGMENT.step = it;
  }

  fill(ColourBlend(color_wheel(SEGMENT.params_internal.aux1), color_wheel(SEGMENT.params_internal.aux0), fade), true);
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RANDOM_COLOR[] PROGMEM = "Random Colors@!,Fade time;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Rainbow_Cycle()
{

/*
 * Cycles a rainbow over the entire string of LEDs.
 */
// void mAnimatorLight::mode_rainbow_cycle(void) {
  uint16_t counter = (millis() * ((SEGMENT.speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    //intensity/29 = 0 (1/16) 1 (1/8) 2 (1/4) 3 (1/2) 4 (1) 5 (2) 6 (4) 7 (8) 8 (16)
    uint8_t index = (i * (16 << (SEGMENT.intensity /29)) / SEGLEN) + counter;
    SEGMENT.SetPixelColor(i, color_wheel(index));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RAINBOW_CYCLE[] PROGMEM = "Rainbow Cylcle@!,Size;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Running lights effect with smooth sine transition base.
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Base_Running(bool saw)
{
  uint8_t x_scale = SEGMENT.intensity >> 2;
  uint32_t counter = (millis() * SEGMENT.speed) >> 9;

  for(uint16_t i = 0; i < SEGLEN; i++) {
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
    SEGMENT.SetPixelColor(
      i, 
      // ColourBlend(RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)), SEGCOLOR_U32(1), s)
      ColourBlend( SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(), SEGCOLOR_U32(1), s)
    );
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Running lights effect with smooth sine transition.
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Running_Lights()
{
  EffectAnim__Base_Running(false);
}
static const char PM_EFFECT_CONFIG__RUNNING_LIGHTS[] PROGMEM = "Running@!,Wave width;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Running lights effect with sawtooth transition.
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Saw()
{
  EffectAnim__Base_Running(true);
}
static const char PM_EFFECT_CONFIG__SAW[] PROGMEM = "Saw@!,Width;!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Blink several LEDs in random colors on, reset, repeat.
 *               Inspired by www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Twinkle()
{
  fill(SEGCOLOR_U32(1));

  uint32_t cycleTime = 20 + (255 - SEGMENT.speed)*5;
  uint32_t it = millis() / cycleTime;
  if (it != SEGMENT.step)
  {
    uint16_t maxOn = map(SEGMENT.intensity, 0, 255, 1, SEGLEN); // make sure at least one LED is on
    if (SEGMENT.params_internal.aux0 >= maxOn)
    {
      SEGMENT.params_internal.aux0 = 0;
      SEGMENT.params_internal.aux1 = random16(); // new seed for our PRNG
    }
    SEGMENT.params_internal.aux0++;
    SEGMENT.step = it;
  }
  
  uint16_t PRNG16 = SEGMENT.params_internal.aux1;

  for (uint16_t i = 0; i < SEGMENT.params_internal.aux0; i++)
  {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 13849; // next 'random' number
    uint32_t p = (uint32_t)SEGLEN * (uint32_t)PRNG16;
    uint16_t j = p >> 16;
    SEGMENT.SetPixelColor(j, SEGMENT.GetPaletteColour(j, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TWINKLE[] PROGMEM = "Twinkle@!,!;!,!;!;;m12=0"; //pixels


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Dissolve function
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Dissolve(uint32_t color)
{

//tmp added
uint8_t _brightness = pCONT_iLight->getBriRGB_Global();

// void mAnimatorLight::dissolve(uint32_t color) {
  bool wa = (SEGCOLOR_U32(1) != 0 && _brightness < 255); //workaround, can't compare getPixel to color if not full brightness
  
  for (uint16_t j = 0; j <= SEGLEN / 15; j++)
  {
    if (random8() <= SEGMENT.intensity) {
      for (uint8_t times = 0; times < 10; times++) //attempt to spawn a new pixel 5 times
      {
        uint16_t i = random16(SEGLEN);
        if (SEGMENT.params_internal.aux0) { //dissolve to primary/palette
          if ( SEGMENT.GetPixelColor(i).getU32() == SEGCOLOR_U32(1) || wa)
          {
            if (color == SEGCOLOR_U32(0))
            {
              SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
            } else { SEGMENT.SetPixelColor(i, color); }     
            break; //only spawn 1 new pixel per frame per 50 LEDs
          }
        } else { //dissolve to secondary
          if ( SEGMENT.GetPixelColor(i).getU32() != SEGCOLOR_U32(1)) { SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1)); break; }
        }
      }
    }
  }

  if (SEGMENT.call > (255 - SEGMENT.speed) + 15) 
  {
    SEGMENT.params_internal.aux0 = !SEGMENT.params_internal.aux0;
    SEGMENT.call = 0;
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Blink several LEDs on and then off
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Dissolve()
{
  EffectAnim__Base_Dissolve(SEGCOLOR_U32(0));
}
static const char PM_EFFECT_CONFIG__DISSOLVE[] PROGMEM = "Dissolve@Repeat speed,Dissolve speed,,,,Random;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Running
 * @description: Blink several LEDs on and then off in random colors
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Dissolve_Random()
{
  EffectAnim__Base_Dissolve(color_wheel(random8()));
}
static const char PM_EFFECT_CONFIG__DISSOLVE_RANDOM[] PROGMEM = "Dissolve Random@Repeat speed,Dissolve speed;,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Android loading circle
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Android()
{
  
  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  if (SEGMENT.params_internal.aux1 > ((float)SEGMENT.intensity/255.0)*(float)SEGLEN)
  {
    SEGMENT.params_internal.aux0 = 1;
  } else
  {
    if (SEGMENT.params_internal.aux1 < 2) SEGMENT.params_internal.aux0 = 0;
  }

  uint16_t a = SEGMENT.step;
  
  if (SEGMENT.params_internal.aux0 == 0)
  {
    if (SEGMENT.call %3 == 1) {a++;}
    else {SEGMENT.params_internal.aux1++;}
  } else
  {
    a++;
    if (SEGMENT.call %3 != 1) SEGMENT.params_internal.aux1--;
  }
  
  if (a >= SEGLEN) a = 0;

  if (a + SEGMENT.params_internal.aux1 < SEGLEN)
  {
    for(int i = a; i < a+SEGMENT.params_internal.aux1; i++) {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
  } else
  {
    for(int i = a; i < SEGLEN; i++) {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
    for(int i = 0; i < SEGMENT.params_internal.aux1 - (SEGLEN -a); i++) {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
  }
  SEGMENT.step = a;
  
  SEGMENT.cycle_time__rate_ms =  3 + ((8 * (uint32_t)(255 - SEGMENT.speed)) / SEGLEN);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__ANDROID[] PROGMEM = "Android@!,Width;!,!;!;;m12=1"; //vertical


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Red - Amber - Green - Blue lights running
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__ColourFul()
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
        cols[i] = SEGMENT.GetPaletteColour(i*fac, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();  
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
  uint32_t it = millis_at_start_of_effect_update / cycleTime;
  if (it != SEGMENT.step)
  {
    if (SEGMENT.speed > 0) SEGMENT.params_internal.aux0++;
    if (SEGMENT.params_internal.aux0 >= numColors) SEGMENT.params_internal.aux0 = 0;
    SEGMENT.step = it;
  }

  for (int i = 0; i < SEGLEN; i+= numColors)
  {
    for (int j = 0; j < numColors; j++) SEGMENT.setPixelColor(i + j, cols[SEGMENT.params_internal.aux0 + j]);
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COLORFUL[] PROGMEM = "Colourful@!,Saturation;1,2,3;!";



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

void mAnimatorLight::EffectAnim__Traffic_Light()
{
  for(uint16_t i=0; i < SEGLEN; i++)
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  uint32_t mdelay = 500;
  for (int i = 0; i < SEGLEN-2 ; i+=3)
  {
    switch (SEGMENT.params_internal.aux0)
    {
      case 0: SEGMENT.SetPixelColor(i, 0x00FF0000); mdelay = 150 + (100 * (uint32_t)(255 - SEGMENT.speed));break;
      case 1: SEGMENT.SetPixelColor(i, 0x00FF0000); mdelay = 150 + (20 * (uint32_t)(255 - SEGMENT.speed)); SEGMENT.SetPixelColor(i+1, 0x00EECC00); break;
      case 2: SEGMENT.SetPixelColor(i+2, 0x0000FF00); mdelay = 150 + (100 * (uint32_t)(255 - SEGMENT.speed));break;
      case 3: SEGMENT.SetPixelColor(i+1, 0x00EECC00); mdelay = 150 + (20 * (uint32_t)(255 - SEGMENT.speed));break;
    }
  }

  if (millis() - SEGMENT.step > mdelay)
  {
    SEGMENT.params_internal.aux0++;
    if (SEGMENT.params_internal.aux0 == 1 && SEGMENT.intensity > 140) SEGMENT.params_internal.aux0 = 2; //skip Red + Amber, to get US-style sequence
    if (SEGMENT.params_internal.aux0 > 3) SEGMENT.params_internal.aux0 = 0;
    SEGMENT.step = millis();
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TRAFFIC_LIGHT[] PROGMEM = "Traffic Light@!,US style;,!;!";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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

void mAnimatorLight::EffectAnim__Base_Running(uint32_t color1, uint32_t color2)
{
  uint8_t pxw = 1 + (SEGMENT.intensity >> 5);
  uint32_t cycleTime = 35 + (255 - SEGMENT.speed);
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.speed == 0) it = 0;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    if((i + SEGMENT.params_internal.aux0) % (pxw*2) < pxw) {
      if (color1 == SEGCOLOR_U32(0))
      {
        SEGMENT.SetPixelColor(SEGLEN -i -1, SEGMENT.GetPaletteColour(SEGLEN -i -1, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
      } else
      {
        SEGMENT.SetPixelColor(SEGLEN -i -1, color1);
      }
    } else {
      SEGMENT.SetPixelColor(SEGLEN -i -1, color2);
    }
  }

  if (it != SEGMENT.step )
  {
    SEGMENT.params_internal.aux0 = (SEGMENT.params_internal.aux0 +1) % (pxw*2);
    SEGMENT.step = it;
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

/*
 * Alternating color/sec pixels running.
 */
void mAnimatorLight::EffectAnim__Running_Colour(void) {
  return EffectAnim__Base_Running(SEGCOLOR_U32(0), SEGCOLOR_U32(1));
}
static const char PM_EFFECT_CONFIG__RUNNING_COLOR[] PROGMEM = "Running 2@!,Width;!,!;!";


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

void mAnimatorLight::EffectAnim__Running_Random()
{

  uint32_t cycleTime = 25 + (3 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.params_internal.aux1 == it){return;}// return FRAMETIME_MS;

  for(uint16_t i=SEGLEN-1; i > 0; i--) {
    SEGMENT.SetPixelColor( i, SEGMENT.GetPixelColor( i - 1));
  }

  if(SEGMENT.step == 0) {
    SEGMENT.params_internal.aux0 = get_random_wheel_index(SEGMENT.params_internal.aux0);
    SEGMENT.SetPixelColor(0, color_wheel(SEGMENT.params_internal.aux0));
  }

  SEGMENT.step++;
  if (SEGMENT.step > ((255-SEGMENT.intensity) >> 4))
  {
    SEGMENT.step = 0;
  }

  SEGMENT.params_internal.aux1 = it;
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RUNNING_RANDOM[] PROGMEM = "Stream@!,Zone size;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE




#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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

void mAnimatorLight::EffectAnim__Base_Gradient(bool loading)
{

  uint16_t counter = millis() * ((SEGMENT.speed >> 2) + 1);
  uint16_t pp = counter * SEGLEN >> 16;
  if (SEGMENT.call == 0) pp = 0;
  float val; //0.0 = sec 1.0 = pri
  float brd = loading ? SEGMENT.intensity : SEGMENT.intensity/2;
  if (brd <1.0) brd = 1.0;
  int p1 = pp-SEGLEN;
  int p2 = pp+SEGLEN;

  for(uint16_t i = 0; i < SEGLEN; i++)
  {
    if (loading)
    {
      val = abs(((i>pp) ? p2:pp) -i);
    } else {
      val = MIN(abs(pp-i),MIN(abs(p1-i),abs(p2-i)));
    }
    val = (brd > val) ? val/brd * 255 : 255;
    SEGMENT.SetPixelColor(
      i, 
      ColourBlend(SEGCOLOR_U32(0), SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(), val)
    );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/*
 * Gradient run
 */
void mAnimatorLight::EffectAnim__Gradient(void) {
  return EffectAnim__Base_Gradient(false);
}
static const char PM_EFFECT_CONFIG__GRADIENT[] PROGMEM = "Gradient@!,Spread;!,!;!;;ix=16";



/*
 * Gradient run with hard transition
 */
void mAnimatorLight::EffectAnim__Loading(void) {
  return EffectAnim__Base_Gradient(true);
}
static const char PM_EFFECT_CONFIG__LOADING[] PROGMEM = "Loading@!,Fade;!,!;!;;ix=16";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * //American Police Light with all LEDs Red and Blue 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Police(uint32_t color1, uint32_t color2, bool all)
{

  uint16_t counter = millis() * ((SEGMENT.speed >> 2) +1);
  uint16_t idexR = (counter * SEGLEN) >> 16;
  if (idexR >= SEGLEN) idexR = 0;

  uint16_t topindex = SEGLEN >> 1;
  uint16_t idexB = (idexR > topindex) ? idexR - topindex : idexR + topindex;
  if (SEGMENT.call == 0) SEGMENT.params_internal.aux0 = idexR;
  if (idexB >= SEGLEN) idexB = 0; //otherwise overflow on odd number of LEDs

  if (all) { //different algo, ensuring immediate fill
    if (idexB > idexR) {
      fill(color2);
      for (uint16_t i = idexR; i < idexB; i++) SEGMENT.SetPixelColor(i, color1);
    } else {
      fill(color1);
      for (uint16_t i = idexB; i < idexR; i++) SEGMENT.SetPixelColor(i, color2);
    } 
  } else { //regular dot-only mode
    uint8_t size = 1 + SEGMENT.intensity >> 3;
    if (size > SEGLEN/2) size = 1+ SEGLEN/2;
    for (uint8_t i=0; i <= size; i++) {
      SEGMENT.SetPixelColor(idexR+i, color1);
      SEGMENT.SetPixelColor(idexB+i, color2);
    }
    if (SEGMENT.params_internal.aux0 != idexR) {
      uint8_t gap = (SEGMENT.params_internal.aux0 < idexR)? idexR - SEGMENT.params_internal.aux0:SEGLEN - SEGMENT.params_internal.aux0 + idexR;
      for (uint8_t i = 0; i <= gap ; i++) {
        if ((idexR - i) < 0) idexR = SEGLEN-1 + i;
        if ((idexB - i) < 0) idexB = SEGLEN-1 + i;
        SEGMENT.SetPixelColor(idexR-i, color1);
        SEGMENT.SetPixelColor(idexB-i, color2);
      }
      SEGMENT.params_internal.aux0 = idexR;
    }
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


//American Police Light with all LEDs Red and Blue 
void mAnimatorLight::EffectAnim__Polce_All()
{  
  return EffectAnim__Base_Police(RED, BLUE, true);
}
static const char PM_EFFECT_CONFIG__POLICE_ALL[] PROGMEM = "Police All@!,Width;,Bg;0";


//Police Lights Red and Blue 
void mAnimatorLight::EffectAnim__Police()
{
  fill(SEGCOLOR_U32(1));
  return EffectAnim__Base_Police(RED, BLUE, false);
}
static const char PM_EFFECT_CONFIG__POLICE[] PROGMEM = "Police@!,Width;,Bg;0";


//Police All with custom colors
void mAnimatorLight::EffectAnim__Two_Areas()
{  
  return EffectAnim__Base_Police(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true);
}
static const char PM_EFFECT_CONFIG__TWO_AREAS[] PROGMEM = "Two Areas@!,Dot size,,,,,,,Overlay;1,2,Bg;!";


//Police Lights with custom colors 
void mAnimatorLight::EffectAnim__Two_Dots()
{
  fill(SEGCOLOR_U32(2));
  uint32_t color2 = (SEGCOLOR_U32(1) == SEGCOLOR_U32(2)) ? SEGCOLOR_U32(0) : SEGCOLOR_U32(1);

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  return EffectAnim__Base_Police(SEGCOLOR_U32(0), color2, false);
}
static const char PM_EFFECT_CONFIG__TWO_DOTS[] PROGMEM = "Two Dots@!,Dot size,,,,,,,Overlay;1,2,Bg;!";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


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

void mAnimatorLight::EffectAnim__TriColour()
{
  uint32_t cycleTime = 1000 + (255 - SEGMENT.speed)*200;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t ledIndex = (prog * SEGLEN * 3) >> 16;
  uint16_t ledOffset = ledIndex;

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    SEGMENT.SetPixelColor(i, 
      // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i, nullptr, true, PALETTE_SOLID_WRAP, 2)
      SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)
    );
  }
  
  if(ledIndex < SEGLEN) { //wipe from 0 to 1
    for (uint16_t i = 0; i < SEGLEN; i++)
    {
      SEGMENT.SetPixelColor(i, (i > ledOffset)? SEGCOLOR_U32(0) : SEGCOLOR_U32(1));
    }
  } else if (ledIndex < SEGLEN*2) { //wipe from 1 to 2
    ledOffset = ledIndex - SEGLEN;
    for (uint16_t i = ledOffset +1; i < SEGLEN; i++)
    {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1));
    }
  } else //wipe from 2 to 0
  {
    ledOffset = ledIndex - SEGLEN*2;
    for (uint16_t i = 0; i <= ledOffset; i++)
    {
      SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TRICOLOR_WIPE[] PROGMEM = "TriColour@!;1,2,3;!";



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

void mAnimatorLight::EffectAnim__Fade_TriColour()
{
  uint16_t counter = millis() * ((SEGMENT.speed >> 3) +1);
  uint32_t prog = (counter * 768) >> 16;

  uint32_t color1 = 0, color2 = 0;
  byte stage = 0;

  if(prog < 256) {
    color1 = SEGCOLOR_U32(0);
    color2 = SEGCOLOR_U32(1);
    stage = 0;
  } else if(prog < 512) {
    color1 = SEGCOLOR_U32(1);
    color2 = SEGCOLOR_U32(2);
    stage = 1;
  } else {
    color1 = SEGCOLOR_U32(2);
    color2 = SEGCOLOR_U32(0);
    stage = 2;
  }

  byte stp = prog; // % 256
  uint32_t color = 0;
  for(uint16_t i = 0; i < SEGLEN; i++) {
    if (stage == 2) {
      color = ColourBlend(
        // RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i, nullptr, true, PALETTE_SOLID_WRAP, 2)),
        SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(),        
        color2, stp);
    } else if (stage == 1) {
      color = ColourBlend(color1, 
        // RgbcctColor::GetU32Colour(mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i, nullptr, true, PALETTE_SOLID_WRAP, 2)),
        SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(),        
        stp);
    } else {
      color = ColourBlend(color1, color2, stp);
    }
    SEGMENT.SetPixelColor(i, color);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__TRICOLOR_FADE[] PROGMEM = "TriColour Fade@!;1,2,3;!";



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

void mAnimatorLight::EffectAnim__Multi_Comet()
{

  uint32_t cycleTime = 10 + (uint32_t)(255 - SEGMENT.speed);
  uint32_t it = millis() / cycleTime;
  if (SEGMENT.step == it){
    SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
    return;
  }
  if (!SEGMENT.allocateData(sizeof(uint16_t) * 8)){return;}
  
  SEGMENT.fade_out(SEGMENT.intensity);
  
  uint16_t* comets = reinterpret_cast<uint16_t*>(SEGMENT.data);

  for(uint8_t i=0; i < 8; i++) {
    if(comets[i] < SEGLEN) {
      uint16_t index = comets[i];
      if (SEGCOLOR_U32(2) != 0)
      {
        SEGMENT.SetPixelColor(index, i % 2 ? SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) : SEGCOLOR_RGBCCT(2) );
      } else
      {
        SEGMENT.SetPixelColor(index, SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
      }
      comets[i]++;
    } else {
      if(!random(SEGLEN)) {
        comets[i] = 0;
      }
    }
  }

  SEGMENT.step = it;
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__MULTI_COMET[] PROGMEM = "Multi Comet@!,Speed;1,2,3;!";




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

void mAnimatorLight::EffectAnim__Oscillate()
{

  uint8_t numOscillators = 3;
  uint16_t dataSize = sizeof(oscillator) * numOscillators;

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }
  
  Oscillator* oscillators = reinterpret_cast<Oscillator*>(SEGMENT.data);

  if (SEGMENT.call == 0)
  {
    // oscillators[0] = {(uint16_t)SEGLEN/4,   SEGLEN/8,  1, 1};
    oscillators[0].pos = SEGLEN/4;
    oscillators[0].size = SEGLEN/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 1;
    // oscillators[1] = {SEGLEN/4*3, SEGLEN/8,  1, 2};
    oscillators[0].pos = SEGLEN/4*3;
    oscillators[0].size = SEGLEN/8;
    oscillators[0].dir = 1;
    oscillators[0].speed = 2;
    // oscillators[2] = {SEGLEN/4*2, SEGLEN/8, -1, 1};
    oscillators[0].pos = SEGLEN/4*2;
    oscillators[0].size = SEGLEN/8;
    oscillators[0].dir = -1;
    oscillators[0].speed = 1;

    
  }

  uint32_t cycleTime = 20 + (2 * (uint32_t)(255 - SEGMENT.speed));
  uint32_t it = millis() / cycleTime;

  for(uint8_t i = 0; i < numOscillators; i++) {
    // if the counter has increased, move the oscillator by the random step
    if (it != SEGMENT.step) oscillators[i].pos += oscillators[i].dir * oscillators[i].speed;
    oscillators[i].size = SEGLEN/(3+SEGMENT.intensity/8);
    if((oscillators[i].dir == -1) && (oscillators[i].pos <= 0)) {
      oscillators[i].pos = 0;
      oscillators[i].dir = 1;
      // make bigger steps for faster speeds
      oscillators[i].speed = SEGMENT.speed > 100 ? random8(2, 4):random8(1, 3);
    }
    if((oscillators[i].dir == 1) && (oscillators[i].pos >= (SEGLEN - 1))) {
      oscillators[i].pos = SEGLEN - 1;
      oscillators[i].dir = -1;
      oscillators[i].speed = SEGMENT.speed > 100 ? random8(2, 4):random8(1, 3);
    }
  }

  for(uint16_t i=0; i < SEGLEN; i++) {
    uint32_t color = BLACK;
    for(uint8_t j=0; j < numOscillators; j++) {
      if(i >= oscillators[j].pos - oscillators[j].size && i <= oscillators[j].pos + oscillators[j].size) {
        color = (color == BLACK) ? SEGCOLOR_U32(j) : ColourBlend(color, SEGCOLOR_U32(j), 128);
      }
    }
    SEGMENT.SetPixelColor(i, color);
  }
 
  SEGMENT.step = it;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__OSCILLATE[] PROGMEM = "Oscillate@";




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

void mAnimatorLight::EffectAnim__Pride_2015()
{
  uint16_t duration = 10 + SEGMENT.speed;
  uint16_t sPseudotime = SEGMENT.step;
  uint16_t sHue16 = SEGMENT.params_internal.aux0;

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

  for (uint16_t i = 0 ; i < SEGLEN; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);
    fastled_col = col_to_crgb( SEGMENT.GetPixelColor(i).getU32() );

    nblend(fastled_col, newcolor, 64);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.step = sPseudotime;
  SEGMENT.params_internal.aux0 = sHue16;
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__PRIDE_2015[] PROGMEM = "Pride@!;;";




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
//eight colored dots, weaving in and out of sync with each other
// void mAnimatorLight::mode_juggle(void){

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Juggle()
{

  // ALOG_INF(PSTR("EffectAnim__Juggle %d %dms"), SEGMENT.call, SEGMENT.cycle_time__rate_ms);

  if (SEGMENT.call == 0) {
    SEGMENT.setUpLeds();  // lossless getPixelColor()
    SEGMENT.fill(BLACK);
  }

  SEGMENT.fadeToBlackBy(192 - (3*SEGMENT.intensity/4));

  CRGB fastled_col;
  byte dothue = 0;
  for (int i = 0; i < 8; i++) {
    uint16_t index = 0 + beatsin88((16 + SEGMENT.speed)*(i + 7), 0, SEGLEN -1);
    fastled_col = CRGB(SEGMENT.getPixelColor(index));
    fastled_col |= (SEGMENT.palette_id==0)?CHSV(dothue, 220, 255):ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, dothue, 255);
    SEGMENT.setPixelColor(index, fastled_col);
    dothue += 32;
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__JUGGLES[] PROGMEM = "Juggle@!,Trail;;!;;sx=64,ix=128"; // Pixels, Beatsin

// getVal(elem["sx"], &seg._speed);
//   getVal(elem["ix"], &seg._intensity);

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

// void mAnimatorLight::mode_palette()
// {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Palette()
{
  uint16_t counter = 0;
  if (SEGMENT.speed != 0) 
  {
    counter = (millis() * ((SEGMENT.speed >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && SEGMENT.speed == 0));
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint8_t colorIndex = (i * 255 / SEGLEN) - counter;
    
    if (noWrap) colorIndex = map(colorIndex, 0, 255, 0, 240); //cut off blend at palette "end"
    
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(colorIndex, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));   

  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__PALETTE[] PROGMEM = "Palette@Cycle speed;;!;;c3=0,o2=0";


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
// // in step 3 above) (Effect intensity = Sparking).



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__ColourWaves()
{
  uint16_t duration = 10 + SEGMENT.speed;
  uint16_t sPseudotime = SEGMENT.step;
  uint16_t sHue16 = SEGMENT.params_internal.aux0;

  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  // uint16_t hueinc16 = beatsin88(113, 300, 1500);
  uint16_t hueinc16 = beatsin88(113, 60, 300)*SEGMENT.intensity*10/255;  // Use the intensity Slider for the hues

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for ( uint16_t i = 0 ; i < SEGLEN; i++) {
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

    CRGB newcolor = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, hue8, bri8);
    fastled_col = SEGMENT.GetPixelColor(i).getU32();

    nblend(fastled_col, newcolor, 128);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.step = sPseudotime;
  SEGMENT.params_internal.aux0 = sHue16;
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COLORWAVES[] PROGMEM = "Colour Waves@!,Hue;!;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * colored stripes pulsing at a defined Beats-Per-Minute (BPM)
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__BPM()
{

  CRGB fastled_col;
  uint32_t stp = (millis() / 20) & 0xFF;
  uint8_t beat = beatsin8(SEGMENT.speed, 64, 255);
  for (uint16_t i = 0; i < SEGLEN; i++) {
    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, stp + (i * 2), beat - stp + (i * 10));
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__BPM[] PROGMEM = "BPM@!;!;!;;sx=64";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * based on https://gist.github.com/kriegsman/5408ecd397744ba0393e
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Twinkle_Colour()
{
  uint16_t dataSize = (SEGLEN+7) >> 3; //1 bit per LED
  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }
  
  CRGB fastled_col, prev;
  fract8 fadeUpAmount = 8 + (SEGMENT.speed/4), fadeDownAmount = 5 + (SEGMENT.speed/7);
  for (uint16_t i = 0; i < SEGLEN; i++) {
    fastled_col = SEGMENT.GetPixelColor(i).getU32();
    prev = fastled_col;
    uint16_t index = i >> 3;
    uint8_t  bitNum = i & 0x07;
    bool fadeUp = bitRead(SEGMENT.data[index], bitNum);
    
    if (fadeUp) {
      CRGB incrementalColor = fastled_col;
      incrementalColor.nscale8_video( fadeUpAmount);
      fastled_col += incrementalColor;

      if (fastled_col.red == 255 || fastled_col.green == 255 || fastled_col.blue == 255) {
        bitWrite(SEGMENT.data[index], bitNum, false);
      }
      SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);

      // if (RgbcctColor::GetU32Colour(SEGMENT.GetPixelColor(i)) == prev) //fix "stuck" pixels
      // {
      //   fastled_col += fastled_col;
      //   SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
      // }
    } else {
      fastled_col.nscale8( 255 - fadeDownAmount);
      SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
    }
  }

  for (uint16_t j = 0; j <= SEGLEN / 50; j++)
  {
    if (random8() <= SEGMENT.intensity) {
      for (uint8_t times = 0; times < 5; times++) //attempt to spawn a new pixel 5 times
      {
        int i = random16(SEGLEN);
        if(SEGMENT.GetPixelColor(i).getU32() == 0) {
          fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, random8(), 64, NOBLEND);
          uint16_t index = i >> 3;
          uint8_t  bitNum = i & 0x07;
          bitWrite(SEGMENT.data[index], bitNum, true);
          SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
          break; //only spawn 1 new pixel per frame per 50 LEDs
        }
      }
    }
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COLORTWINKLE[] PROGMEM = "Colortwinkles@Fade speed,Spawn speed;;!;;m12=0"; //pixels



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

//Calm effect, like a lake at night
// void mAnimatorLight::mode_lake() {
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Lake()
{
  uint8_t sp = SEGMENT.speed/10;
  int wave1 = beatsin8(sp +2, -64,64);
  int wave2 = beatsin8(sp +1, -64,64);
  uint8_t wave3 = beatsin8(sp +2,   0,80);
  CRGB fastled_col;

  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    int index = cos8((i*15)+ wave1)/2 + cubicwave8((i*23)+ wave2)/2;           
    uint8_t lum = (index > wave3) ? index - wave3 : 0;
    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, map(index,0,255,0,240), lum, LINEARBLEND);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__LAKE[] PROGMEM = "Lake@!;Fx;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * meteor effect
// send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Meteor()
{
  if (!SEGMENT.allocateData(SEGLEN)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }

  byte* trail = SEGMENT.data;
  
  byte meteorSize= 1+ SEGLEN / 10;
  uint16_t counter = millis() * ((SEGMENT.speed >> 2) +8);
  uint16_t in = counter * SEGLEN >> 16;

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < SEGLEN; i++) {
    if (random8() <= 255 - SEGMENT.intensity)
    {
      byte meteorTrailDecay = 128 + random8(127);
      trail[i] = scale8(trail[i], meteorTrailDecay);
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).WithBrightness(trail[i]) );
    }
  }

  // draw meteor
  for(int j = 0; j < meteorSize; j++) {
    uint16_t index = in + j;
    if(index >= SEGLEN) {
      index = (in + j - SEGLEN);
    }
    trail[index] = 240;
    SEGMENT.SetPixelColor(index, SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__METEOR[] PROGMEM = "Meteor@!,Trail length;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * send a meteor from begining to to the end of the strip with a trail that randomly decays.
// adapted from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectMeteorRain
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Metoer_Smooth()
{
  if (!SEGMENT.allocateData(SEGLEN)){return;}
  
  byte* trail = SEGMENT.data;
  
  byte meteorSize= 1+ SEGLEN / 10;
  uint16_t in = map((SEGMENT.step >> 6 & 0xFF), 0, 255, 0, SEGLEN -1);

  // fade all leds to colors[1] in LEDs one step
  for (uint16_t i = 0; i < SEGLEN; i++) {
    if (trail[i] != 0 && random8() <= 255 - SEGMENT.intensity)
    {
      int change = 3 - random8(12); //change each time between -8 and +3
      trail[i] += change;
      if (trail[i] > 245) trail[i] = 0;
      if (trail[i] > 240) trail[i] = 240;
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).WithBrightness(trail[i]) );
    }
  }
  
  // draw meteor
  for(int j = 0; j < meteorSize; j++) {  
    uint16_t index = in + j;   
    if(in + j >= SEGLEN) {
      index = (in + j - SEGLEN);
    }
    trail[index] = 240;
    SEGMENT.SetPixelColor(index, SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }

  SEGMENT.step += SEGMENT.speed +1;
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__METEOR_SMOOTH[] PROGMEM = "Meteor Smooth@!,Trail length;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * //  TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
//
//  TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
//  Colors are chosen from a palette. Read more about this effect using the link above!
// If COOL_LIKE_INCANDESCENT is set to 1, colors will
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#define COOL_LIKE_INCANDESCENT 1

CRGB mAnimatorLight::EffectAnim__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat)
{
  // Overall twinkle speed (changed)
  uint16_t ticks = ms / SEGMENT.params_internal.aux0;
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8(slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).
  // Default is 5.
  uint8_t twinkleDensity = (SEGMENT.intensity >> 5) +1;

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
    c = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, hue, bright, NOBLEND);
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
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  
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

void mAnimatorLight::EffectAnim__Base_Twinkle_Fox(bool cat)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  // Calculate speed
  if (SEGMENT.speed > 100) SEGMENT.params_internal.aux0 = 3 + ((255 - SEGMENT.speed) >> 3);
  else SEGMENT.params_internal.aux0 = 22 + ((100 - SEGMENT.speed) >> 1);

  // Set up the background color, "bg".
  CRGB bg;
  bg = col_to_crgb(SEGCOLOR_U32(1));
  uint8_t bglight = bg.getAverageLight();
  if (bglight > 64) {
    bg.nscale8_video(16); // very bright, so scale to 1/16th
  } else if (bglight > 16) {
    bg.nscale8_video(64); // not that bright, so scale to 1/4th
  } else {
    bg.nscale8_video(86); // dim, scale to 1/3rd.
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for (uint16_t i = 0; i < SEGLEN; i++) {
  
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((millis() * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = EffectAnim__Base_Twinkle_Fox_One_Twinkle(myclock30, myunique8, cat);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if (deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      SEGMENT.SetPixelColor(i, c.red, c.green, c.blue);
    } else if (deltabright > 0) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      SEGMENT.SetPixelColor(i, ColourBlend(crgb_to_col(bg), crgb_to_col(c), deltabright * 8));
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      SEGMENT.SetPixelColor(i, bg.r, bg.g, bg.b);
    }
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Twinkle_Fox()
{
  return EffectAnim__Base_Twinkle_Fox(false);
}
static const char PM_EFFECT_CONFIG__TWINKLE_FOX[] PROGMEM = "Twinkle SlowIn/SlowOut@!,Twinkle rate;;!";

void mAnimatorLight::EffectAnim__Twinkle_Cat()
{
  return EffectAnim__Base_Twinkle_Fox(true);
}
static const char PM_EFFECT_CONFIG__TWINKLE_CAT[] PROGMEM = "Twinkle FastIn/SlowOut@!,Twinkle rate;;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Spots(uint16_t threshold)
{

  fill(SEGCOLOR_U32(1));
  
  uint16_t maxZones = SEGLEN >> 2;
  uint16_t zones = 1 + ((SEGMENT.intensity * maxZones) >> 8);
  uint16_t zoneLen = SEGLEN / zones;
  uint16_t offset = (SEGLEN - zones * zoneLen) >> 1;

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint16_t wave = triwave16((i * 0xFFFF) / zoneLen);
      if (wave > threshold) {
        uint16_t index = 0 + pos + i;
        uint8_t s = (wave - threshold)*255 / (0xFFFF - threshold);
        SEGMENT.SetPixelColor(index, ColourBlend( SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), SEGCOLOR_RGBCCT(1), 255-s) );
      }
    }
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


//intensity slider sets number of "lights", speed sets LEDs per light
void mAnimatorLight::EffectAnim__Spots()
{
  EffectAnim__Base_Spots((255 - SEGMENT.speed) << 8);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SPOTS[] PROGMEM = "Spots@Spread,Width,,,,,,,Overlay;!,!;!";


//intensity slider sets number of "lights", LEDs per light fade in and out
void mAnimatorLight::EffectAnim__Fade_Spots()
{
  uint16_t counter = millis() * ((SEGMENT.speed >> 2) +8);
  uint16_t t = triwave16(counter);
  uint16_t tr = (t >> 1) + (t >> 2);
//   return spots_base(tr);
  EffectAnim__Base_Spots(tr);
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SPOTS_FADE[] PROGMEM = "Spots Fade@Spread,Width,,,,Repeat Rate (ms),,Overlay;!,!;!"; // 7 sliders + 4 options before first ;


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
uint16_t mAnimatorLight::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

uint16_t mAnimatorLight::mode_palette()
{
  uint16_t counter = 0;
  if (SEGMENT.speed != 0) 
  {
    counter = (millis() * ((SEGMENT.speed >> 3) +1)) & 0xFFFF;
    counter = counter >> 8;
  }
  
  bool noWrap = (paletteBlend == 2 || (paletteBlend == 0 && SEGMENT.speed == 0));
  RgbcctColor colour;
  for (uint16_t i = 0; i < SEGLEN; i++)
  {
    uint8_t colorIndex = (i * 255 / SEGLEN) - counter;
    
    colour = SEGMENT.GetPaletteColour(colorIndex, PALETTE_SPAN_OFF, PALETTE_WRAP_ON);
    colour = RgbcctColor::ApplyBrightnesstoRgbcctColour( colour, SEGMENT.getBrightnessRGB_WithGlobalApplied(), SEGMENT.getBrightnessCCT_WithGlobalApplied() );
    
    SEGMENT.SetPixelColor(i, colour);
  }
  return FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Glitter()
{

  mode_palette();

  if (SEGMENT.intensity > random8())
  {
    SEGMENT.SetPixelColor(random16(SEGLEN), ULTRAWHITE);
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__GLITTER[] PROGMEM = "Glitter@!,!,,,,,,,Overlay;1,2,Glitter color;!;;pal=25,m12=0"; //pixels


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

void mAnimatorLight::EffectAnim__Popcorn()
{

  //allocate segment data
  uint16_t maxNumPopcorn = 24; 
  uint16_t dataSize = sizeof(spark) * maxNumPopcorn;
  if (!SEGMENT.allocateData(dataSize))
  {
    return;
  }// return EffectAnim__Solid_Colour(); //allocation failed
  
  Spark* popcorn = reinterpret_cast<Spark*>(SEGMENT.data);

  float gravity = -0.0001 - (SEGMENT.speed/200000.0); // m/s/s
  gravity *= SEGLEN;

  bool hasCol2 = SEGCOLOR_U32(2);
  fill(hasCol2 ? BLACK : SEGCOLOR_U32(1));

  uint8_t numPopcorn = SEGMENT.intensity*maxNumPopcorn/255;

  if (numPopcorn == 0) numPopcorn = 1;

  for(uint8_t i = 0; i < numPopcorn; i++) {
    bool isActive = popcorn[i].pos >= 0.0f;

    if (isActive) { // if kernel is active, update its position
      popcorn[i].pos += popcorn[i].vel;
      popcorn[i].vel += gravity;
      uint32_t col = color_wheel(popcorn[i].colIndex);
      if (!SEGMENT.palette_id && popcorn[i].colIndex < NUM_COLORS)
      {
        col = SEGCOLOR_U32(popcorn[i].colIndex);
      }
      
      uint16_t ledIndex = popcorn[i].pos;
      if (ledIndex < SEGLEN) SEGMENT.SetPixelColor(ledIndex, col);
    } else { // if kernel is inactive, randomly pop it
      if (random8() < 2) { // POP!!!
        popcorn[i].pos = 0.01f;
        
        uint16_t peakHeight = 128 + random8(128); //0-255
        peakHeight = (peakHeight * (SEGLEN -1)) >> 8;
        popcorn[i].vel = sqrt(-2.0 * gravity * peakHeight);
        
        if (SEGMENT.palette_id)
        {
          popcorn[i].colIndex = random8();
        } else {
          byte col = random8(0, NUM_COLORS);
          if (!hasCol2 || !SEGCOLOR_U32(col)) col = 0;
          popcorn[i].colIndex = col;
        }
      }
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}
static const char PM_EFFECT_CONFIG__POPCORN[] PROGMEM = "Popcorn@!,!,,,,,,,Overlay;!,!,!;!;;m12=1"; //bar
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
void mAnimatorLight::EffectAnim__Plasma()
{

  uint8_t thisPhase = beatsin8(6,-64,64);                       // Setting phase change for a couple of waves.
  uint8_t thatPhase = beatsin8(7,-64,64);

  for (int i = 0; i < SEGMENT.length(); i++) {   // For each of the LED's in the strand, set color &  brightness based on a wave as follows:
    uint8_t colorIndex = cubicwave8((i*(1+ 3*(SEGMENT.speed >> 5)))+(thisPhase) & 0xFF)/2   // factor=23 // Create a wave and add a phase change and add another wave with its own phase change.
                             + cos8((i*(1+ 2*(SEGMENT.speed >> 5)))+(thatPhase) & 0xFF)/2;  // factor=15 // Hey, you can even change the frequencies if you wish.
    uint8_t thisBright = qsub8(colorIndex, beatsin8(6,0, (255 - SEGMENT.intensity)|0x01 ));
    CRGB color = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, colorIndex, thisBright, LINEARBLEND);
    SEGMENT.SetPixelColor(i, color.red, color.green, color.blue);
  }
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
} 
static const char PM_EFFECT_CONFIG__PLASMA[] PROGMEM = "Plasma@Phase,!;!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

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
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Percent()
{
	uint8_t percent = MAX(0, MIN(200, SEGMENT.intensity));
	uint16_t active_leds = (percent < 100) ? SEGLEN * percent / 100.0
                                         : SEGLEN * (200 - percent) / 100.0;
  
  uint8_t size = (1 + ((SEGMENT.speed * SEGLEN) >> 11));
  if (SEGMENT.speed == 255) size = 255;
    
  if (percent < 100) {
    for (uint16_t i = 0; i < SEGLEN; i++) {
	  	if (i < SEGMENT.step) {
        SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
	  	}
	  	else {
        SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1));
	  	}
	  }
  } else {
    for (uint16_t i = 0; i < SEGLEN; i++) {
	  	if (i < (SEGLEN - SEGMENT.step)) {
        SEGMENT.SetPixelColor(i, SEGCOLOR_U32(1));
	  	}
	  	else {
        SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
	  	}
	  }
  }

  if(active_leds > SEGMENT.step) {  // smooth transition to the target value
    SEGMENT.step += size;
    if (SEGMENT.step > active_leds) SEGMENT.step = active_leds;
  } else if (active_leds < SEGMENT.step) {
    if (SEGMENT.step > size) SEGMENT.step -= size; else SEGMENT.step = 0;
    if (SEGMENT.step < active_leds) SEGMENT.step = active_leds;
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}
static const char PM_EFFECT_CONFIG__PERCENT[] PROGMEM = "Percent@,% of fill,,,,One color;!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
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
// The speed and scale and motion each layer varies slowly within independent 
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
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

void mAnimatorLight::EffectAnim__Pacifica()
{
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
    pacifica_palette_1 = SEGMENT.palette_container->CRGB16Palette16_Palette.data;
    pacifica_palette_2 = SEGMENT.palette_container->CRGB16Palette16_Palette.data;
    pacifica_palette_3 = SEGMENT.palette_container->CRGB16Palette16_Palette.data;
  }

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  uint16_t sCIStart1 = SEGMENT.params_internal.aux0, sCIStart2 = SEGMENT.params_internal.aux1, sCIStart3 = SEGMENT.step, sCIStart4 = SEGMENT.step >> 16;
  //static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  uint32_t deltams = 26 + (SEGMENT.speed >> 3);
  
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));
  SEGMENT.params_internal.aux0 = sCIStart1; SEGMENT.params_internal.aux1 = sCIStart2;
  SEGMENT.step = sCIStart4; SEGMENT.step = (SEGMENT.step << 16) + sCIStart3;

  // Clear out the LED array to a dim background blue-green
  //fill(132618);

  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < SEGLEN; i++) {
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

    SEGMENT.SetPixelColor(i, c.red, c.green, c.blue);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__PACIFICA[] PROGMEM = "Pacifica@!,Angle;;!;;pal=51";

// Add one layer of waves into the led array
CRGB mAnimatorLight::pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{

  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale >> 1) + 20;
  
  waveangle += ((120 + SEGMENT.intensity) * i); //original 250 * i
  uint16_t s16 = sin16(waveangle) + 32768;
  uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += (cs * i);
  uint16_t sindex16 = sin16(ci) + 32768;
  uint8_t sindex8 = scale16(sindex16, 240);
  return ColorFromPalette_WithLoad(p, sindex8, bri, LINEARBLEND);
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

void mAnimatorLight::EffectAnim__Solid_Glitter()
{

  fill(SEGCOLOR_U32(0));

  if (SEGMENT.intensity > random8())
  {
    SEGMENT.SetPixelColor(random16(SEGLEN), ULTRAWHITE);
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SOLID_GLITTER[] PROGMEM = "Solid Glitter@,!;Bg,,Glitter color;;;m12=0";


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

void mAnimatorLight::EffectAnim__Sunrise()
{
  
  //speed 0 - static sun
  //speed 1 - 60: sunrise time in minutes
  //speed 60 - 120 : sunset time in minutes - 60;
  //speed above: "breathing" rise and set
  if (SEGMENT.call == 0 || SEGMENT.speed != SEGMENT.params_internal.aux0) {
	  SEGMENT.step = millis(); //save starting time, millis() because now can change from sync
    SEGMENT.params_internal.aux0 = SEGMENT.speed;
  }
  
  fill(0);
  uint16_t stage = 0xFFFF;
  
  uint32_t s10SinceStart = (millis() - SEGMENT.step) /100; //tenths of seconds
  
  if (SEGMENT.speed > 120) { //quick sunrise and sunset
	  uint16_t counter = (millis() >> 1) * (((SEGMENT.speed -120) >> 1) +1);
	  stage = triwave16(counter);
  } else if (SEGMENT.speed) { //sunrise
	  uint8_t durMins = SEGMENT.speed;
	  if (durMins > 60) durMins -= 60;
	  uint32_t s10Target = durMins * 600;
	  if (s10SinceStart > s10Target) s10SinceStart = s10Target;
	  stage = map(s10SinceStart, 0, s10Target, 0, 0xFFFF);
	  if (SEGMENT.speed > 60) stage = 0xFFFF - stage; //sunset
  }
  
  for (uint16_t i = 0; i <= SEGLEN/2; i++)
  {
    //default palette is Fire
    uint32_t c = SEGMENT.GetPaletteColour(0, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(); // background

    uint16_t wave = triwave16((i * stage) / SEGLEN);

    wave = (wave >> 8) + ((wave * SEGMENT.intensity) >> 15);

    if (wave > 240) { //clipped, full white sun
      c = SEGMENT.GetPaletteColour(240, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    } else { //transition
      c = SEGMENT.GetPaletteColour(wave, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    }
    SEGMENT.SetPixelColor(i, c);
    SEGMENT.SetPixelColor(SEGLEN - i - 1, c);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SUNRISE[] PROGMEM = "Sunrise@Time [min],Width;;!;;sx=60";

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * 


// Sine waves that have controllable phase change speed, frequency and cutoff. By Andrew Tuline.
// SEGMENT.speed ->Speed, SEGMENT.intensity -> Frequency (SEGMENT.fft1 -> Color change, SEGMENT.fft2 -> PWM cutoff)
//
void mAnimatorLight::mode_sinewave(void) {             // Adjustable sinewave. By Andrew Tuline

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Sinewave()
{
  
  //#define qsuba(x, b)  ((x>b)?x-b:0)               // Analog Unsigned subtraction macro. if result <0, then => 0

  uint16_t colorIndex = millis() /32;//(256 - SEGMENT.fft1);  // Amount of colour change.

  SEGMENT.step += SEGMENT.speed/16;                   // Speed of animation.
  uint16_t freq = SEGMENT.intensity/4;//SEGMENT.fft2/8;                       // Frequency of the signal.

  for (int i=0; i<SEGLEN; i++) {                    // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int pixBri = cubicwave8((i*freq)+SEGMENT.step); //qsuba(cubicwave8((i*freq)+SEGMENT.step), (255-SEGMENT.intensity)); // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    //setPixCol(i, i*colorIndex/255, pixBri);
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(i*colorIndex/255, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), pixBri) );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__SINEWAVE[] PROGMEM = "Sine@";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Best of both worlds from Palette and Spot effects. By Aircoookie
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Flow()
{
  
  uint16_t counter = 0;
  if (SEGMENT.speed != 0) 
  {
    counter = millis() * ((SEGMENT.speed >> 2) +1);
    counter = counter >> 8;
  }
  
  uint16_t maxZones = SEGLEN / 6; //only looks good if each zone has at least 6 LEDs
  uint16_t zones = (SEGMENT.intensity * maxZones) >> 8;
  if (zones & 0x01) zones++; //zones must be even
  if (zones < 2) zones = 2;
  uint16_t zoneLen = SEGLEN / zones;
  uint16_t offset = (SEGLEN - zones * zoneLen) >> 1;

  fill(SEGMENT.GetPaletteColour(-counter, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE));

  for (uint16_t z = 0; z < zones; z++)
  {
    uint16_t pos = offset + z * zoneLen;
    for (uint16_t i = 0; i < zoneLen; i++)
    {
      uint8_t colorIndex = (i * 255 / zoneLen) - counter;
      uint16_t led = (z & 0x01) ? i : (zoneLen -1) -i;
      if (IS_REVERSE) led = (zoneLen -1) -led;
      SEGMENT.SetPixelColor(pos + led, SEGMENT.GetPaletteColour(colorIndex, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FLOW[] PROGMEM = "Flow@!,Zones;;!;;m12=1"; //vertical
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Static
 ****************************************************************************************************************************
 ****************************************************************************************************************************/

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
void mAnimatorLight::EffectAnim__Static_Pattern()
{
  
  uint16_t lit = 1 + SEGMENT.speed;
  uint16_t unlit = 1 + SEGMENT.intensity;
  bool drawingLit = true;
  uint16_t cnt = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, 
      (drawingLit) ? SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32() : SEGCOLOR_U32(1)
    );
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__STATIC_PATTERN[] PROGMEM = "Static Pattern Lit@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Blink/strobe function
 * @note : Converted from WLED Effects
 * 
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette)
{

  uint32_t cycleTime = (255 - SEGMENT.speed)*20;
  uint32_t onTime = FRAMETIME;
  if (!strobe) onTime += ((cycleTime * SEGMENT.intensity) >> 8);
  cycleTime += FRAMETIME*2;
  uint32_t it = millis_at_start_of_effect_update / cycleTime;
  uint32_t rem = millis_at_start_of_effect_update % cycleTime;

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
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }
  else
  {
    SEGMENT.fill(color); // alternates between colours passed in unless do_pal is set
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 *  Normal blinking. 50% on/off time.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Blink()
{
  EffectAnim__Base_Blink(SEGCOLOR_U32(0), SEGCOLOR_U32(1), false, true);  
}
static const char PM_EFFECT_CONFIG__BLINK[] PROGMEM = "Blink@!,Duty cycle;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Classic Blink effect. Cycling through the rainbow.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Blink_Rainbow()
{
  EffectAnim__Base_Blink(color_wheel(SEGMENT.call & 0xFF), SEGCOLOR_U32(1), false, false);  
}
static const char PM_EFFECT_CONFIG__BLINK_RAINBOW[] PROGMEM = "Blink Rainbow@Frequency,Blink duration;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Classic Strobe effect.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Strobe()
{
  EffectAnim__Base_Blink(SEGCOLOR_U32(0), SEGCOLOR_U32(1), true, true);
}
static const char PM_EFFECT_CONFIG__STROBE[] PROGMEM = "Strobe@!;!,!;!;01";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Strobe effect with different strobe count and pause, controlled by speed.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
void mAnimatorLight::EffectAnim__Strobe_Multi()
{
  for(uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  uint16_t delay = 50 + 20*(uint16_t)(255-SEGMENT.speed);
  uint16_t count = 2 * ((SEGMENT.speed / 10) + 1);
  if(SEGMENT.step < count) {
    if((SEGMENT.step & 1) == 0) {
      for(uint16_t i = 0; i < SEGLEN; i++) {
        SEGMENT.SetPixelColor(i, SEGCOLOR_U32(0));
      }
      delay = 20;
    } else {
      delay = 50;
    }
  }
  SEGMENT.step = (SEGMENT.step + 1) % (count + 1);  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__MULTI_STROBE[] PROGMEM = "Strobe Multi@!,!;!,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 * Classic Strobe effect. Cycling through the rainbow.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Strobe_Rainbow()
{
  EffectAnim__Base_Blink(color_wheel(SEGMENT.call & 0xFF), SEGCOLOR_U32(1), true, false);
}
static const char PM_EFFECT_CONFIG__STROBE_RAINBOW[] PROGMEM = "Strobe Rainbow@!;,!;!;01";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Cycles all LEDs at once through a rainbow.
 * Note: SEGMENT.intensity < 128 = pastel rainbow, SEGMENT.intensity > 128 = full saturation rainbow
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Rainbow()
{

  uint16_t counter = (millis() * ((SEGMENT.speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  if (SEGMENT.intensity < 128){
    fill(ColourBlend(color_wheel(counter),WHITE,128-SEGMENT.intensity));
  } else {
    fill(color_wheel(counter));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RAINBOW[] PROGMEM = "Rainbow@!,Saturation;;!";

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Lightning()
{
 uint16_t ledstart = random16(SEGLEN);               // Determine starting location of flash
  uint16_t ledlen = 1 + random16(SEGLEN -ledstart);   // Determine length of flash (not to go beyond NUM_LEDS-1)
  uint8_t bri = 255/random8(1, 3);

  if (SEGMENT.params_internal.aux1 == 0) //init, leader flash
  {
    SEGMENT.params_internal.aux1 = random8(4, 4 + SEGMENT.intensity/20); //number of flashes
    SEGMENT.params_internal.aux1 *= 2;

    bri = 52; //leader has lower brightness
    SEGMENT.params_internal.aux0 = 200; //200ms delay after leader
  }

  fill(SEGCOLOR_U32(1));

  if (SEGMENT.params_internal.aux1 > 3 && !(SEGMENT.params_internal.aux1 & 0x01)) { //flash on even number >2
    for (int i = ledstart; i < ledstart + ledlen; i++)
    {
      SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
    SEGMENT.params_internal.aux1--;

    SEGMENT.step = millis();
    //return random8(4, 10); // each flash only lasts one frame/every 24ms... originally 4-10 milliseconds
  } else {
    if (millis() - SEGMENT.step > SEGMENT.params_internal.aux0) {
      SEGMENT.params_internal.aux1--;
      if (SEGMENT.params_internal.aux1 < 2) SEGMENT.params_internal.aux1 = 0;

      SEGMENT.params_internal.aux0 = (50 + random8(100)); //delay between flashes
      if (SEGMENT.params_internal.aux1 == 2) {
        SEGMENT.params_internal.aux0 = (random8(255 - SEGMENT.speed) * 100); // delay between strikes
      }
      SEGMENT.step = millis();
    }
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__LIGHTNING[] PROGMEM = "Lightning@!,!,,,,,,,Overlay;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : mode_fire_2012()
 * 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on SEGLEN; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above) (Speed = COOLING), and SPARKING (used
// in step 3 above) (Effect Intensity = Sparking).
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Fire_2012()
{

  uint32_t it = millis() >> 5; //div 32

  if (!SEGMENT.allocateData(SEGLEN)){return;} // return EffectAnim__Solid_Colour(); //allocation failed
  
  byte* heat = SEGMENT.data;

  if (it != SEGMENT.step)
  {
    // Step 1.  Cool down every cell a little
    for (uint16_t i = 0; i < SEGLEN; i++) {
      SEGMENT.data[i] = qsub8(heat[i],  random8(0, (((20 + SEGMENT.speed /3) * 10) / SEGLEN) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint16_t k= SEGLEN -1; k > 1; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() <= SEGMENT.intensity) {
      uint8_t y = random8(7);
      if (y < SEGLEN) heat[y] = qadd8(heat[y], random8(160,255));
    }
    SEGMENT.step = it;
  }

  // Step 4.  Map from heat cells to LED colors
  for (uint16_t j = 0; j < SEGLEN; j++) {
    CRGB color = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, MIN(heat[j],240), 255, LINEARBLEND);
    SEGMENT.SetPixelColor(j, color.red, color.green, color.blue);
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FIRE_2012[] PROGMEM = "Fire 2012@Cooling,Spark rate,,,Boost;;!;1;sx=64,ix=160,m12=1";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Railway Crossing / Christmas Fairy lights
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Railway()
{

  uint16_t dur = 40 + (255 - SEGMENT.speed) * 10;
  uint16_t rampdur = (dur * SEGMENT.intensity) >> 8;
  if (SEGMENT.step > dur)
  {
    //reverse direction
    SEGMENT.step = 0;
    SEGMENT.params_internal.aux0 = !SEGMENT.params_internal.aux0;
  }
  uint8_t pos = 255;
  if (rampdur != 0)
  {
    uint16_t p0 = (SEGMENT.step * 255) / rampdur;
    if (p0 < 255) pos = p0;
  }
  if (SEGMENT.params_internal.aux0) pos = 255 - pos;
  for (uint16_t i = 0; i < SEGLEN; i += 2)
  {
    SEGMENT.SetPixelColor(i, SEGMENT.GetPaletteColour(255 - pos, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    if (i < SEGLEN -1)
    {
      SEGMENT.SetPixelColor(i + 1, SEGMENT.GetPaletteColour(pos, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  }
  SEGMENT.step += FRAMETIME_MS;
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__RAILWAY[] PROGMEM = "Railway@!,Smoothness;1,2;!";


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

void mAnimatorLight::EffectAnim__Heartbeat()
{
  uint8_t bpm = 40 + (SEGMENT.speed >> 4);
  uint32_t msPerBeat = (60000 / bpm);
  uint32_t secondBeat = (msPerBeat / 3);

  uint32_t bri_lower = SEGMENT.params_internal.aux1;
  bri_lower = bri_lower * 2042 / (2048 + SEGMENT.intensity);
  SEGMENT.params_internal.aux1 = bri_lower;

  unsigned long beatTimer = millis() - SEGMENT.step;
  if((beatTimer > secondBeat) && !SEGMENT.params_internal.aux0) { // time for the second beat?
    SEGMENT.params_internal.aux1 = UINT16_MAX; //full bri
    SEGMENT.params_internal.aux0 = 1;
  }
  if(beatTimer > msPerBeat) { // time to reset the beat timer?
    SEGMENT.params_internal.aux1 = UINT16_MAX; //full bri
    SEGMENT.params_internal.aux0 = 0;
    SEGMENT.step = millis();
  }

  for (uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, ColourBlend( SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32(), SEGCOLOR_U32(1), 255 - (SEGMENT.params_internal.aux1 >> 8)));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HEARTBEAT[] PROGMEM = "Heartbeat@!,!;!,!;!;01;m12=1";


/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Noise
 ****************************************************************************************************************************
 ****************************************************************************************************************************/


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__FillNoise8()
{
  if (SEGMENT.call == 0) SEGMENT.step = random16(12345);
  CRGB fastled_col;
  for (uint16_t i = 0; i < SEGLEN; i++) {
    uint8_t index = inoise8(i * SEGLEN, SEGMENT.step + i * SEGLEN);
    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, 255, LINEARBLEND);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.step += beatsin8(SEGMENT.speed, 1, 6); //10,1,4

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__FILLNOISE8[] PROGMEM = "Fill Noise@!;!;!";

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_1()
{

  uint16_t scale = 320;                                      // the "zoom factor" for the noise
  CRGB fastled_col;
  SEGMENT.step += (1 + SEGMENT.speed/16);

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = beatsin8(11);                           // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = SEGMENT.step/42;             // the y position becomes slowly incremented


    uint16_t real_x = (i + shift_x) * scale;                  // the x position of the noise field swings @ 17 bpm
    uint16_t real_y = (i + shift_y) * scale;                  // the y position becomes slowly incremented
    uint32_t real_z = SEGMENT.step;                          // the z position becomes quickly incremented

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                         // map LED color based on noise data

    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_1[] PROGMEM = "Noise 1@!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_2()
{
  uint16_t scale = 1000;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  SEGMENT.step += (1 + (SEGMENT.speed >> 1));

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = SEGMENT.step >> 6;                         // x as a function of time
    uint16_t shift_y = SEGMENT.step/42;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field

    uint8_t noise = inoise16(real_x, 0, 4223) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_2[] PROGMEM = "Noise 2@!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_3()
{
  uint16_t scale = 800;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  SEGMENT.step += (1 + SEGMENT.speed);

  for (uint16_t i = 0; i < SEGLEN; i++) {

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (i + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (i + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = SEGMENT.step*8;  

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_3[] PROGMEM = "Noise 3@!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : https://github.com/aykevl/ledstrip-spark/blob/master/ledstrip->ino
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise16_4()
{
  CRGB fastled_col;
  uint32_t stp = (millis() * SEGMENT.speed) >> 7;
  for (uint16_t i = 0; i < SEGLEN; i++) {
    int16_t index = inoise16(uint32_t(i) << 12, stp);
    fastled_col = ColorFromPalette_WithLoad(SEGMENT.palette_container->CRGB16Palette16_Palette.data, index);
    SEGMENT.SetPixelColor(i, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISE16_4[] PROGMEM = "Noise 4@!;!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Peaceful noise that's slow and with gradually changing palettes. Does not support WLED palettes or default colours or controls.
 *         Slow noise palette by Andrew Tuline.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Noise_Pal()
{
  uint16_t scale = 15 + (SEGMENT.intensity >> 2); //default was 30
  //#define scale 30

  uint16_t dataSize = sizeof(CRGBPalette16) * 2; //allocate space for 2 Palettes
  if (!SEGMENT.allocateData(dataSize)){return;}// return EffectAnim__Solid_Colour(); //allocation failed

  CRGBPalette16* palettes = reinterpret_cast<CRGBPalette16*>(SEGMENT.data);

  uint16_t changePaletteMs = 4000 + SEGMENT.speed *10; //between 4 - 6.5sec
  if (millis() - SEGMENT.step > changePaletteMs)
  {
    SEGMENT.step = millis();

    uint8_t baseI = random8();
    palettes[1] = CRGBPalette16(CHSV(baseI+random8(64), 255, random8(128,255)), CHSV(baseI+128, 255, random8(128,255)), CHSV(baseI+random8(92), 192, random8(128,255)), CHSV(baseI+random8(92), 255, random8(128,255)));
  }

  CRGB color;

  //EVERY_N_MILLIS(10) { //(don't have to time this, effect function is only called every 24ms)
  nblendPaletteTowardPalette(palettes[0], palettes[1], 48);               // Blend towards the target palette over 48 iterations.

  if (SEGMENT.palette_id > 0) palettes[0] = SEGMENT.palette_container->CRGB16Palette16_Palette.data;

  for(int i = 0; i < SEGLEN; i++) {
    uint8_t index = inoise8(i*scale, SEGMENT.params_internal.aux0+i*scale);                // Get a value from the noise function. I'm using both x and y axis.
    color = ColorFromPalette_WithLoad(palettes[0], index, 255, LINEARBLEND);       // Use the my own palette.
    SEGMENT.SetPixelColor(i, color.red, color.green, color.blue);
  }

  SEGMENT.params_internal.aux0 += beatsin8(10,1,4);                                        // Moving along the distance. Vary it a bit with a sine wave.

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__NOISEPAL[] PROGMEM = "Noise Pal@!,Scale;;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : We're making sine waves here. By Andrew Tuline.
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__Base_Phased(uint8_t moder)
{
  uint8_t allfreq = 16;                                          // Base frequency.
  //float* phasePtr = reinterpret_cast<float*>(SEGMENT.step);       // Phase change value gets calculated.
  static float phase = 0;//phasePtr[0];
  uint8_t cutOff = (255-SEGMENT.intensity);                      // You can change the number of pixels.  AKA intensity (was 192).
  uint8_t modVal = 5;//SEGMENT.fft1/8+1;                         // You can change the modulus. AKA FFT1 (was 5).

  uint8_t index = millis()/64;                                    // Set color rotation speed
  phase += SEGMENT.speed/32.0;                                   // You can change the speed of the wave. AKA SPEED (was .4)
  //phasePtr[0] = phase; 

  for (int i = 0; i < SEGLEN; i++) {
    if (moder == 1) modVal = (inoise8(i*10 + i*10) /16);         // Let's randomize our mod length with some Perlin noise.
    uint16_t val = (i+1) * allfreq;                              // This sets the frequency of the waves. The +1 makes sure that leds[0] is used.
    if (modVal == 0) modVal = 1;
    val += phase * (i % modVal +1) /2;                           // This sets the varying phase change of the waves. By Andrew Tuline.
    uint8_t b = cubicwave8(val);                                 // Now we make an 8 bit sinewave.
    b = (b > cutOff) ? (b - cutOff) : 0;                         // A ternary operator to cutoff the light.
    SEGMENT.SetPixelColor(i, ColourBlend(SEGCOLOR_RGBCCT(1), SEGMENT.GetPaletteColour(index, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE), b));
    index += 256 / SEGLEN;
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


void mAnimatorLight::EffectAnim__PhasedNoise(void) {
  return EffectAnim__Base_Phased(1);
}
static const char PM_EFFECT_CONFIG__PHASEDNOISE[] PROGMEM = "Phased Noise@!,!;!,!;!";


void mAnimatorLight::EffectAnim__Phased(void) {
  return EffectAnim__Base_Phased(0);
}
static const char PM_EFFECT_CONFIG__PHASED[] PROGMEM = "Phased@!,!;!,!;!";



/****************************************************************************************************************************
 **************************************************************************************************************************** 
 * Scan
 ****************************************************************************************************************************
 ****************************************************************************************************************************/



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Base function for scanners
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Scan(bool dual)
{

  uint32_t cycleTime = 750 + (255 - SEGMENT.speed)*150;
  uint32_t perc = millis() % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t size = 1 + ((SEGMENT.intensity * SEGLEN) >> 9);
  uint16_t ledIndex = (prog * ((SEGLEN *2) - size *2)) >> 16;

  fill(SEGCOLOR_U32(1));

  int led_offset = ledIndex - (SEGLEN - size);
  led_offset = abs(led_offset);

  if (dual) {
    for (uint16_t j = led_offset; j < led_offset + size; j++) {
      uint16_t i2 = SEGLEN -1 -j;
      SEGMENT.SetPixelColor(i2, SEGMENT.GetPaletteColour(i2, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) ); // mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i2, nullptr, true, PALETTE_SOLID_WRAP, (SEGCOLOR_U32(2))? 2:0)
    }
  }

  for (uint16_t j = led_offset; j < led_offset + size; j++) {
    SEGMENT.SetPixelColor(j, SEGMENT.GetPaletteColour(j, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}


/*
 * Runs a single pixel back and forth.
 */
void mAnimatorLight::EffectAnim__Scan()
{
  EffectAnim__Base_Scan(false);
}
static const char PM_EFFECT_CONFIG__SCAN[] PROGMEM = "Scan@!,# of dots,,,,,,,Overlay;!,!,!;!";


/*
 * Runs two pixel back and forth in opposite directions.
 */
void mAnimatorLight::EffectAnim__Scan_Dual()
{
  EffectAnim__Base_Scan(true);
}
static const char PM_EFFECT_CONFIG__DUAL_SCAN[] PROGMEM = "Scan Dual@!,# of dots,,,,,,,Overlay;!,!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Larson_Scanner(bool dual)
{
  
  uint16_t counter = millis() * ((SEGMENT.speed >> 2) +8);
  uint16_t index = counter * SEGLEN  >> 16;

  SEGMENT.fade_out(SEGMENT.intensity);

  if (SEGMENT.step > index && SEGMENT.step - index > SEGLEN/2) {
    SEGMENT.params_internal.aux0 = !SEGMENT.params_internal.aux0;
  }
  
  for (uint16_t i = SEGMENT.step; i < index; i++) {
    uint16_t j = (SEGMENT.params_internal.aux0)?i:SEGLEN-1-i;
    SEGMENT.SetPixelColor( j, SEGMENT.GetPaletteColour(j, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  }
  if (dual) {
    uint32_t c;
    if (SEGCOLOR_U32(2) != 0) {
      c = SEGCOLOR_U32(2);
    } else {
      c = SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    }

    for (uint16_t i = SEGMENT.step; i < index; i++) {
      uint16_t j = (SEGMENT.params_internal.aux0)?SEGLEN-1-i:i;
      SEGMENT.SetPixelColor(j, c);
    }
  }

  SEGMENT.step = index;
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}


/*
 * K.I.T.T.
 */
void mAnimatorLight::EffectAnim__Larson_Scanner()
{
  EffectAnim__Base_Larson_Scanner(false);
}
static const char PM_EFFECT_CONFIG__LARSON_SCANNER[] PROGMEM = "Larson Scanner@!,Fade rate;!,!;!;;m12=0";


/*
 * Creates two Larson scanners moving in opposite directions
 * Custom mode by Keith Lord: https://github.com/kitesurfer1404/WS2812FX/blob/master/src/custom/DualLarson.h
 */
void mAnimatorLight::EffectAnim__Larson_Scanner_Dual(void){
  return EffectAnim__Base_Larson_Scanner(true);
}
static const char PM_EFFECT_CONFIG__DUAL_LARSON_SCANNER[] PROGMEM = "Larson Scanner Dual@!,Fade rate;!,!,!;!;;m12=0";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__ICU()
{
  uint16_t dest = SEGMENT.step & 0xFFFF;
  uint8_t space = (SEGMENT.intensity >> 3) +2;

  fill(SEGCOLOR_U32(1));

  byte pindex = map(dest, 0, SEGLEN-SEGLEN/space, 0, 255);
  uint32_t col = SEGMENT.GetPaletteColour(pindex, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
  
  SEGMENT.SetPixelColor(dest, col);
  SEGMENT.SetPixelColor(dest + SEGLEN/space, col);

  if(SEGMENT.params_internal.aux0 == dest) { // pause between eye movements
    if(random8(6) == 0) { // blink once in a while
      SEGMENT.SetPixelColor(dest, SEGCOLOR_U32(1));
      SEGMENT.SetPixelColor(dest + SEGLEN/space, SEGCOLOR_U32(1));
      SEGMENT.cycle_time__rate_ms = 200;
      return;
    }
    SEGMENT.params_internal.aux0 = random16(SEGLEN-SEGLEN/space);
    SEGMENT.cycle_time__rate_ms = 1000 + random16(2000);
    return;
  }

  if(SEGMENT.params_internal.aux0 > SEGMENT.step) {
    SEGMENT.step++;
    dest++;
  } else if (SEGMENT.params_internal.aux0 < SEGMENT.step) {
    SEGMENT.step--;
    dest--;
  }

  SEGMENT.SetPixelColor(dest, col);
  SEGMENT.SetPixelColor(dest + SEGLEN/space, col);

  SEGMENT.cycle_time__rate_ms = SPEED_FORMULA_L;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__ICU[] PROGMEM = "ICU@!,!,,,,,,,Overlay;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : //Water ripple
//propagation velocity from speed
//drop rate from intensity
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//4 bytes
typedef struct Ripple {
  uint8_t state;
  uint8_t color;
  uint16_t pos;
} ripple;

void mAnimatorLight::EffectAnim__Base_Ripple(bool rainbow)
{

  uint16_t maxRipples = 1 + (SEGLEN >> 2);
  if (maxRipples > 100) maxRipples = 100;
  uint16_t dataSize = sizeof(ripple) * maxRipples;

  if (!SEGMENT.allocateData(dataSize)){return;} //return EffectAnim__Solid_Colour(); //allocation failed
 
  Ripple* ripples = reinterpret_cast<Ripple*>(SEGMENT.data);

  // ranbow background or chosen background, all very dim.
  if (rainbow) {
    if (SEGMENT.call ==0) {
      SEGMENT.params_internal.aux0 = random8();
      SEGMENT.params_internal.aux1 = random8();
    }
    if (SEGMENT.params_internal.aux0 == SEGMENT.params_internal.aux1) {
      SEGMENT.params_internal.aux1 = random8();
    }
    else if (SEGMENT.params_internal.aux1 > SEGMENT.params_internal.aux0) {
      SEGMENT.params_internal.aux0++;
    } else {
      SEGMENT.params_internal.aux0--;
    }
    fill(ColourBlend(color_wheel(SEGMENT.params_internal.aux0),BLACK,235));
  } else {
    fill(SEGCOLOR_U32(1));
  }
  
  //draw wave
  for (uint16_t i = 0; i < maxRipples; i++)
  {
    uint16_t ripplestate = ripples[i].state;
    if (ripplestate)
    {
      uint8_t rippledecay = (SEGMENT.speed >> 4) +1; //faster decay if faster propagation
      uint16_t rippleorigin = ripples[i].pos;
      uint32_t col = SEGMENT.GetPaletteColour(ripples[i].color, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();

      uint16_t propagation = ((ripplestate/rippledecay -1) * SEGMENT.speed);
      int16_t propI = propagation >> 8;
      uint8_t propF = propagation & 0xFF;
      int16_t left = rippleorigin - propI -1;
      uint8_t amp = (ripplestate < 17) ? triwave8((ripplestate-1)*8) : map(ripplestate,17,255,255,2);

      for (int16_t v = left; v < left +4; v++)
      {
        uint8_t mag = scale8(cubicwave8((propF>>2)+(v-left)*64), amp);
        if (v < SEGLEN && v >= 0)
        {
          SEGMENT.SetPixelColor(v, ColourBlend( SEGMENT.GetPixelColor(v).getU32(), col, mag ));
        }
        int16_t w = left + propI*2 + 3 -(v-left);
        if (w < SEGLEN && w >= 0)
        {
          SEGMENT.SetPixelColor(w, ColourBlend( SEGMENT.GetPixelColor(w).getU32(), col, mag ));
        }
      }  
      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else //randomly create new wave
    {
      if (random16(IBN + 10000) <= SEGMENT.intensity)
      {
        ripples[i].state = 1;
        ripples[i].pos = random16(SEGLEN);
        ripples[i].color = random8(); //color
      }
    }
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Ripple(void) {
  return EffectAnim__Base_Ripple(false);
}
static const char PM_EFFECT_CONFIG__RIPPLE[] PROGMEM = "Ripple@!,Wave #,,,,,,,Overlay;,!;!;12";

void mAnimatorLight::EffectAnim__Ripple_Rainbow(void) {
  return EffectAnim__Base_Ripple(true);
}
static const char PM_EFFECT_CONFIG__RIPPLE_RAINBOW[] PROGMEM = "Ripple Rainbow@!,Wave #;;!;12";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Firing comets from one end. "Lighthouse"
 *
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Comet()
{

  uint16_t counter = millis() * ((SEGMENT.speed >>2) +1);
  uint16_t index = counter * SEGLEN >> 16;
  if (SEGMENT.call == 0) SEGMENT.params_internal.aux0 = index;

  SEGMENT.fade_out(SEGMENT.intensity);

  SEGMENT.SetPixelColor( index, SEGMENT.GetPaletteColour(index, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE) );
  
  if (index > SEGMENT.params_internal.aux0) {
    for (uint16_t i = SEGMENT.params_internal.aux0; i < index ; i++) {
       SEGMENT.SetPixelColor( i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }
  } else if (index < SEGMENT.params_internal.aux0 && index < 10) {
    for (uint16_t i = 0; i < index ; i++) {
       SEGMENT.SetPixelColor( i, SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
    }      
  }
  SEGMENT.params_internal.aux0 = index++;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__COMET[] PROGMEM = "Comet@!,Fade rate;!,!;!";


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Dots waving around in a sine/pendulum motion.
 *         Little pixel birds flying in a circle. By Aircoookie
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Chunchun()
{
  fill(SEGCOLOR_U32(1));
  uint16_t counter = millis()*(6 + (SEGMENT.speed >> 4));
  uint16_t numBirds = SEGLEN >> 2;
  uint16_t span = SEGMENT.intensity << 8;

  for (uint16_t i = 0; i < numBirds; i++)
  {
    counter -= span/numBirds;
    int megumin = sin16(counter) + 0x8000;
    uint32_t bird = (megumin * SEGLEN) >> 16;
    SEGMENT.SetPixelColor(bird, SEGMENT.GetPaletteColour((i * 255)/ numBirds, PALETTE_SPAN_OFF, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__CHUNCHUN[] PROGMEM = "Chunchun@!,Gap size;!,!;!";

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
 * @note : Bouncing Balls Effect
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Bouncing_Balls()
{
  //allocate segment data
  uint16_t maxNumBalls = 16; 
  uint16_t dataSize = sizeof(ball) * maxNumBalls;
  if (!SEGMENT.allocateData(dataSize)){return;}// return EffectAnim__Solid_Colour(); //allocation failed
  
  Ball* balls = reinterpret_cast<Ball*>(SEGMENT.data);
  
  // number of balls based on intensity setting to max of 7 (cycles colors)
  // non-chosen color is a random color
  uint8_t numBalls = int(((SEGMENT.intensity * (maxNumBalls - 0.8f)) / 255) + 1);
  
  float gravity                           = -9.81; // standard value of gravity
  float impactVelocityStart               = sqrt( -2 * gravity);

  unsigned long time = millis();

  if (SEGMENT.call == 0) {
    for (uint8_t i = 0; i < maxNumBalls; i++) balls[i].lastBounceTime = time;
  }
  
  bool hasCol2 = SEGCOLOR_U32(2);
  fill(hasCol2 ? BLACK : SEGCOLOR_U32(1));
  
  for (uint8_t i = 0; i < numBalls; i++) {
    float timeSinceLastBounce = (time - balls[i].lastBounceTime)/((255-SEGMENT.speed)*8/256 +1);
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
    
    uint32_t color = SEGCOLOR_U32(0);
    if (SEGMENT.palette_id) {
      color = color_wheel(i*(256/MAX(numBalls, 8)));
    } else if (hasCol2) {
      color = SEGCOLOR_U32(i % NUM_COLORS);
    }

    uint16_t pos = round(balls[i].height * (SEGLEN - 1));
    SEGMENT.SetPixelColor(pos, color);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__BOUNCINGBALLS[] PROGMEM = "Bouncing Balls@Gravity,# of balls,,,,,,,Overlay;!,!,!;!;1;m12=1"; //bar



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Base_Sinelon(bool dual, bool rainbow)
{

  SEGMENT.fade_out(SEGMENT.intensity);
  uint16_t pos = beatsin16(SEGMENT.speed/10,0,SEGLEN-1);
  if (SEGMENT.call == 0) SEGMENT.params_internal.aux0 = pos;
  uint32_t color1 = SEGMENT.GetPaletteColour(pos, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();

  uint32_t color2 = SEGCOLOR_U32(2);
  if (rainbow) {
    color1 = color_wheel((pos & 0x07) * 32);
  }
  SEGMENT.SetPixelColor(pos, color1);
  if (dual) {
    if (!color2) color2 = SEGMENT.GetPaletteColour(pos, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32();
    if (rainbow) color2 = color1; //rainbow
    SEGMENT.SetPixelColor(SEGLEN-1-pos, color2);
  }
  if (SEGMENT.params_internal.aux0 != pos) { 
    if (SEGMENT.params_internal.aux0 < pos) {
      for (uint16_t i = SEGMENT.params_internal.aux0; i < pos ; i++) {
        SEGMENT.SetPixelColor(i, color1);
        if (dual) SEGMENT.SetPixelColor(SEGLEN-1-i, color2);
      }
    } else {
      for (uint16_t i = SEGMENT.params_internal.aux0; i > pos ; i--) {
        SEGMENT.SetPixelColor(i, color1);
        if (dual) SEGMENT.SetPixelColor(SEGLEN-1-i, color2);
      }
    }
    SEGMENT.params_internal.aux0 = pos;
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}

void mAnimatorLight::EffectAnim__Sinelon(void) {
  return EffectAnim__Base_Sinelon(false);
}
static const char PM_EFFECT_CONFIG__SINELON[] PROGMEM = "Sinelon@!,Trail;!,!,!;!";

void mAnimatorLight::EffectAnim__Sinelon_Dual(void) {
  return EffectAnim__Base_Sinelon(true);
}
static const char PM_EFFECT_CONFIG__SINELON_DUAL[] PROGMEM = "Sinelon Dual@!,Trail;!,!,!;!";

void mAnimatorLight::EffectAnim__Sinelon_Rainbow(void) {
  return EffectAnim__Base_Sinelon(true, true);
}
static const char PM_EFFECT_CONFIG__SINELON_RAINBOW[] PROGMEM = "Sinelon Rainbow@!,Trail;,,!;!";



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Drip Effect
 * @note : Part of WLED Effects
 * @note : Drip Effect ported of: https://www.youtube.com/watch?v=sru2fXh4r7k

//values close to 100 produce 5Hz flicker, which looks very candle-y
//Inspired by https://github.com/avanhanegem/ArduinoCandleEffectNeoPixel
//and https://cpldcpu.wordpress.com/2016/01/05/reverse-engineering-a-real-candle/

 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::EffectAnim__Drip()
{


  // fill(SEGCOLOR_U32(0));
  // for(int i=0;i<505;i++)
  // {
  //   if(i%10==0)
  //   {
  //     SEGMENT.SetPixelColor(i, RgbcctColor(20,20,20,0,0));// water source
  //   }
  // }
  // SEGMENT.SetPixelColor(0,   RgbcctColor(255,0,0,0,0));// water source
  // SEGMENT.SetPixelColor(1,   RgbcctColor(0,25,0,0,0));// water source
  // SEGMENT.SetPixelColor(2,   RgbcctColor(25,0,0,0,0));// water source
  // SEGMENT.SetPixelColor(100, RgbcctColor(0,255,0,0,0));// water source
  // SEGMENT.SetPixelColor(200, RgbcctColor(0,0,255,0,0));// water source
  // SEGMENT.SetPixelColor(300, RgbcctColor(255,0,255,0,0));// water source
  // SEGMENT.SetPixelColor(400, RgbcctColor(0,255,255,0,0));// water source
  // SEGMENT.SetPixelColor(500, RgbcctColor(255,255,20,0,0));// water source
  // SEGMENT.SetPixelColor(600, RgbcctColor(100,0,0,0,0));// water source
  // SEGMENT.SetPixelColor(700, RgbcctColor(0,100,0,0,0));// water source
  // SEGMENT.SetPixelColor(750, RgbcctColor(0,100,100,0,0));// water source
  // SEGMENT.SetPixelColor(757, RgbcctColor(5,0,5,0,0));// water source
  // SEGMENT.SetPixelColor(800, RgbcctColor(0,0,100,0,0));// water source
  // SEGMENT.SetPixelColor(900, RgbcctColor(25,0,25,0,0));// water source
  // SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  // SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  // return;



  //allocate segment data
  uint16_t numDrops = 4; 
  uint16_t dataSize = sizeof(spark) * numDrops;
  if (!SEGMENT.allocateData(dataSize)) 
  {
    return;
   }
   // mode_static(); //allocation failed

  fill(SEGCOLOR_U32(1));
  
  Spark* drops = reinterpret_cast<Spark*>(SEGMENT.data);

  numDrops = 1 + (SEGMENT.intensity >> 6);

  float gravity = -0.001 - (SEGMENT.speed/50000.0);
  gravity *= SEGLEN;
  int sourcedrop = 12;

  for (int j=0;j<numDrops;j++) {
    if (drops[j].colIndex == 0) { //init
      drops[j].pos = SEGLEN-1;    // start at end
      drops[j].vel = 0;           // speed
      drops[j].col = sourcedrop;  // brightness
      drops[j].colIndex = 1;      // drop state (0 init, 1 forming, 2 falling, 5 bouncing) 
    }
    
    SEGMENT.SetPixelColor(SEGLEN-1,ColourBlend(BLACK,SEGCOLOR_U32(0), sourcedrop));// water source
    if (drops[j].colIndex==1) {
      if (drops[j].col>255) drops[j].col=255;
      SEGMENT.SetPixelColor(int(drops[j].pos),ColourBlend(BLACK,SEGCOLOR_U32(0),drops[j].col));
      
      drops[j].col += map(SEGMENT.speed, 0, 255, 1, 6); // swelling
      
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
          SEGMENT.SetPixelColor(int(drops[j].pos)+i,ColourBlend(BLACK,SEGCOLOR_U32(0),drops[j].col/i)); //spread pixel with fade while falling
        }
        
        if (drops[j].colIndex > 2) {       // during bounce, some water is on the floor
          SEGMENT.SetPixelColor(0,ColourBlend(SEGCOLOR_U32(0),BLACK,drops[j].col));
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

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__DRIP[] PROGMEM = "Drip@Gravity,# of drips,,,,,,,Overlay;!,!;!;;m12=1"; //bar
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Test case used for developing new animations
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester_01()
{


 SEGMENT.palette_container->CRGB16Palette16_Palette.data = Test_p;
 pCONT_lAni->_virtualSegmentLength = 50;
 pCONT_lAni->paletteBlend = 3;
 
uint8_t index = 15;

uint8_t paletteIndex = 0; 

CRGB colour;
      uint8_t pbri = 255;

 uint16_t j = 0;
 uint16_t i = 0;
    pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->GetColourFromPreloadedPalette (SEGMENT.palette_id, i, nullptr, true, false, 10));
  for(uint16_t i = 0; i < 50; i++) {
    
    j = i;//map(i,0,50,0,255);
  //  pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
    // pCONT_lAni->SEGMENT.SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  index = i;
  
paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
colour = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);

uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


    pCONT_lAni->SEGMENT.SetPixelColor(i, col);
  
  }
    // pCONT_lAni->SEGMENT.SetPixelColor(0, RgbColor(255,0,0));
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;

// CRGB colour;
//  = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);
// ALOG_TST(PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);







  // uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  // //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  // if (!SEGMENT.allocateData(dataSize))
  // {
  //   ALOG_TST(PSTR("Not Enough Memory"));
  //   SEGMENT.effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
  // }
  
  
  // // Pick new colours
  // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  
  // // Get starting positions already on show
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // // Call the animator to blend from previous to new
  // SetSegment_AnimFunctionCallback(  SEGIDX, 
  //   [this](const AnimationParam& param){ 
  //     this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
  //   }
  // );


  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  

}


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Test case used for developing new animations
 * @note : Shows pixels from palette, in order. Gradients can either be displayed over total length of segment, or repeated by X pixels
 * 
 * @param : "cycle_time__rate_ms" : How often it changes
 * @param : "time_ms" : How often it changes
 * @param : "pixels to update" : How often it changes
 * @param : "cycle_time__rate_ms" : How often it changes 
 * 
 * Currently using to test all palettes as they are converted into one method
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Flasher_Animate_Function_Tester_02()
{


  ALOG_INF( PSTR("SubTask_Flasher_Animate_Function_Tester_02") );


  // SEGMENT.palette_container->CRGB16Palette16_Palette.data = Test_p;
  // pCONT_lAni->_virtualSegmentLength = 50;
  // pCONT_lAni->paletteBlend = 3;

  // uint8_t index = 15;

  // uint8_t paletteIndex = 0; 

  // CRGB colour;
  // uint8_t pbri = 255;

  // uint16_t j = 0;
  // uint16_t i = 0;
  // pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->color_from_pale tte_Intermediate(i, true, false, 10));
  // for(uint16_t i = 0; i < 50; i++) {

  //   j = i;//map(i,0,50,0,255);
  //   //  pCONT_lAni->SEGMENT.SetPixelColor(j, mPaletteI->color_from_palette_internal(0, i, true, true, 10));
  //   // pCONT_lAni->SEGMENT.SetPixelColor(i, mPaletteI->color_from_palette_internal(i, true, true, 10));
  //   index = i;

  //   paletteIndex = i*((255/16)-1);//map(i,);//((255/16)*index)-1; 
  //   colour = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);

  //   uint32_t col = colour.r*65536 +  colour.g*256 +  colour.b;


  //   pCONT_lAni->SEGMENT.SetPixelColor(i, col, true);

  // }

  uint8_t colours_in_palette = 0;

  RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(pCONT_lAni->SEGMENT_I(0).palette_id,0,nullptr,true,true,false,255,&colours_in_palette);

  // RgbcctColor colour = mPaletteI->GetColourFromPaletteAdvanced(mPaletteI->PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID,0,nullptr,true,true,255,&colours_in_palette);

  // SEGMENT.palette_id = mPaletteI->PALETTELIST_VARIABLE_CRGBPALETTE16__BASIC_COLOURS_PRIMARY_SECONDARY_TERTIARY__ID;

  ALOG_INF( PSTR("pID{%d}, colours_in_palette = %d"), pCONT_lAni->SEGMENT_I(0).palette_id, colours_in_palette );

  // SEGMENT.SetPixelColor(0,colour,true);
//  for(int i=0;i<50;i++)
//   {
//     SEGMENT.SetPixelColor(i, RgbColor(0));
//   }

  for(int i=0;i<256;i++)
  {

    colour = mPaletteI->GetColourFromPaletteAdvanced(pCONT_lAni->SEGMENT_I(0).palette_id,i,nullptr,true,true,true,255,&colours_in_palette);

    SEGMENT.SetPixelColor(i, colour, true);

    // ALOG_INF( PSTR("pID{%d}, colours_in_palette = %d"),pCONT_lAni->SEGMENT_I(0).palette_id, colours_in_palette );

    // if(i>colours_in_palette)
    // {
    //   break;
    // }


  }



    // pCONT_lAni->SEGMENT.SetPixelColor(0, RgbColor(255,0,0));
  // SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;

// CRGB colour;
//  = ColorFromPalette_WithLoad( Test_p, paletteIndex, pbri, NOBLEND);
// ALOG_TST(PSTR("colour %d %d,%d,%d"),paletteIndex,colour.r,colour.g,colour.b);


// DEBUG_LINE_HERE;
#ifdef ENABLE_DEVFEATURE_DEBUG_SERIAL__ANIMATION_OUTPUT
          Serial.print("@1");
          #endif 
pCONT_iLight->ShowInter face();




  // uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  // //ALOG_TST(PSTR("dataSize = %d"), dataSize);

  // if (!SEGMENT.allocateData(dataSize))
  // {
  //   ALOG_TST(PSTR("Not Enough Memory"));
  //   SEGMENT.effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID; // Default
  // }
  
  
  // // Pick new colours
  // DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(SEGMENT.palette_id, SEGIDX);
  
  // // Get starting positions already on show
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  // // Call the animator to blend from previous to new
  // SetSegment_AnimFunctionCallback(  SEGIDX, 
  //   [this](const AnimationParam& param){ 
  //     this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
  //   }
  // );


  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Notification style
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
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Static()
{
  SubTask_Segment_Animate_Function__Notification_Base(true, false, false); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Blinking()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, true, false); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Fade()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, false, true); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Pulsing()
{
  SubTask_Segment_Animate_Function__Notification_Base(false, true, true); 
}
void mAnimatorLight::SubTask_Segment_Animate_Function__Notification_Base(bool flag_static, bool flag_alternate_onoff, bool flag_blend)
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate)*2) ){ return; }

  uint16_t   period_ms            = SEGMENT.params_user[0];
  uint16_t*  repeats_desired      = &SEGMENT.params_user[1];
  uint16_t*  duty_cycle_desired   = &SEGMENT.params_user[2];
  uint16_t*  auto_seconds_timeout = &SEGMENT.params_user[3];

  // ALOG_INF(PSTR("duty_cycle_desired=%d"),*duty_cycle_desired);
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "SubTask_Segment_Animate_Function__Notification_Base %d of %d"),getCurrSegmentId(), segments.size());
  

  // if(getCurrSegmentId()==1){delay(5000);}
  
  uint16_t* repeat_counter_p        = &SEGMENT.params_internal.aux0;
  uint16_t* blink_state_p           = &SEGMENT.params_internal.aux1;
  uint16_t* blink_counter_p         = &SEGMENT.params_internal.aux2;
  uint16_t* autotimeout_last_millis = &SEGMENT.params_internal.aux3; // cant use tSaved as its U16, so just compare against previous millis

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
  SEGMENT.cycle_time__rate_ms     = period_ms+FRAMETIME_MS;
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

    SetTransitionColourBuffer_DesiredColour (SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type__used_in_effect_generate, desired_colour); 
    SetTransitionColourBuffer_StartingColour(SEGMENT.Data(), SEGMENT.DataLength(), 0, SEGMENT.colour_type__used_in_effect_generate, starting_colour);

    SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(param); } );

  }

};

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
 * @note : 
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
void mAnimatorLight::EffectAnim__BorderWallpaper__TwoColour_Gradient()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
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
    

    std::vector<RgbcctColor> corner_colours;

  }edge_settings;



  edge_settings.corner_colours.push_back(RgbcctColor(255,0,0,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(255,0,0,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(0,255,0,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(0,255,0,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(0,0,255,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(0,0,255,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(255,0,255,0,0));
  edge_settings.corner_colours.push_back(RgbcctColor(255,0,255,0,0));






  // RgbcctColor edge1_colour = RgbcctColor(255,0,0,0,0);
  // RgbcctColor edge2_colour = RgbcctColor(0,255,0,0,0);
  // RgbcctColor edge3_colour = RgbcctColor(0,0,255,0,0);
  // RgbcctColor edge4_colour = RgbcctColor(255,0,255,0,0);


  RgbcctColor edge1_colour = RgbcctColor(0,100,100,0,0);
  RgbcctColor edge2_colour = RgbcctColor(D_RGB_ORANGE_PASTEL_90SAT  0,0);
  RgbcctColor edge3_colour = RgbcctColor(D_RGB_ORANGE_PASTEL_90SAT  0,0);
  RgbcctColor edge4_colour = RgbcctColor(D_RGB_ORANGE_PASTEL_90SAT  0,0);

  float progress;
  RgbcctColor colour;
  
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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
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
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), edge_start_t + pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }




  // RgbcctColor colour = RgbcctColor();
  // for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  // {    
  //   colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
  //   SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour);
  // }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );






























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
 * @name : ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
 * @note : 
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
void mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Gradient()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {    
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT[] PROGMEM = "Border Wallpaper FourColour@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
 * @note : 
 * 
 * @param aux0 
 * @param aux1 
 * @param aux2 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
void mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Solid()
{

  if (!SEGMENT.allocateData( GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGLEN )){ return; } // Pixel_Width * Two_Channels * Pixel_Count
    
  RgbcctColor colour = RgbcctColor();
  for(uint16_t pixel = 0; pixel < SEGLEN; pixel++)
  {    
    colour = SEGMENT.GetPaletteColour(pixel, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE);
    SetTransitionColourBuffer_DesiredColour(SEGMENT.Data(), SEGMENT.DataLength(), pixel, SEGMENT.colour_type__used_in_effect_generate, colour.WithBrightness(brightness) );
  }

  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

  SetSegment_AnimFunctionCallback( SEGIDX, [this](const AnimationParam& param){ this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); } );

}
static const char PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_SOLID[] PROGMEM = "BW 4s@,,,,,Repeat Rate (ms);!,!,!,!,!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS


/**
 * @brief
 * Uses a segments four RGBCCT colours to describe the four corners of a rear/edge lit backlight (e.g. a gradient ambilight)
 **/

// // Limit ambilight to addressible type, else I will just use "scene"
// void mAnimatorLight::init_Ambilight(){

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

// void mAnimatorLight::SubTask_Ambilight_Main(){

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




// void mAnimatorLight::Ambilight_Sides(){
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


// // void mAnimatorLight::parsesub_ModeAmbilight(){

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
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
void mAnimatorLight::EffectAnim__Hardware__Show_Bus()
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

    float huef = mSupport::mapfloat((float)bus_i, 0.0f, (float)buscount, 0.0f, 1.0f);
    float satf = 1.0f;

    if(bus_i%2)
      satf = 1.0f;

    RgbcctColor colour = HsbColor(huef,satf,1.0f);

    for(uint16_t i = start; i < start + length; i++)
    {
      if(i < start + 1 + bus_i) // let the number of one white pixels be the bus index
      {
        SEGMENT.SetPixelColor(i, RgbcctColor(255,255,255));
        ALOG_INF(PSTR("Bus i < start  %d (%d/%d\t%d)\ti=%d hue100=%d"), bus_i, start, length, start + length, i, uint16_t(huef*100));
      }else{
        SEGMENT.SetPixelColor(i, colour);
      }
    }


  }

  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__SHOW_BUS[] PROGMEM = "Debug Visualise Bus@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
void mAnimatorLight::EffectAnim__Hardware__Manual_Pixel_Counting()
{
  
  SEGMENT.fill(RgbcctColor(5,5,5).getU32());

  uint16_t hue_list[] = {0,120,240,340,50,180,14,350,280};
  uint8_t used_hue = 0;  
  
  RgbcctColor colour = RgbcctColor();


  for (uint16_t i = 0; i < SEGLEN; i++)
  {

    if((i%10)==0) // Every 10th should be bright white
    {
      
      if((i%20)==0) // If its every 50th, then use hue map instead of the bright white
      {

        float hue = (float)hue_list[used_hue++]/360.0f;
        // Serial.println(hue);

        SEGMENT.SetPixelColor(i, HsbColor(hue,1.0f,1.0f));
        
        // ALOG_INF(PSTR("50i %d"), i);

        if(used_hue >= ARRAY_SIZE(hue_list))
          used_hue = 0;

      }else{

        SEGMENT.SetPixelColor(i, RgbColor(50));

      }

    }


  }




  // SEGMENT.SetPixelColor(0, RgbcctColor(255,0,0));
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__MANUAL_PIXEL_COUNTING[] PROGMEM = "Debug Pixel Counting@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
void mAnimatorLight::EffectAnim__Hardware__View_Pixel_Range()
{

  SEGMENT.fill(SEGCOLOR_U32(0));
  
  for (uint32_t i = SEGMENT.params_user[0]; i < SEGMENT.params_user[1]; i++)
  {
    SEGMENT.SetPixelColor(i,SEGCOLOR_U32(1));
  }
    
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__VIEW_PIXEL_RANGE[] PROGMEM = "Debug Pixel Range@Fg size,Bg size;Fg,!;!;;pal=0";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
void mAnimatorLight::EffectAnim__Hardware__Light_Sensor_Pixel_Indexing()
{
  
  uint16_t lit = 1 + SEGMENT.speed;
  uint16_t unlit = 1 + SEGMENT.intensity;
  bool drawingLit = true;
  uint16_t cnt = 0;

  for (uint16_t i = 0; i < SEGLEN; i++) {
    SEGMENT.SetPixelColor(i, 
      (drawingLit) ? SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32() : SEGCOLOR_U32(1)
    );
    cnt++;
    if (cnt >= ((drawingLit) ? lit : unlit)) {
      cnt = 0;
      drawingLit = !drawingLit;
    }
  }
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING[] PROGMEM = "Debug Light Sensor Indexing@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING




/*******************************************************************************************************************
********************************************************************************************************************
************ END OF AMBILIGHT DEFINITIONS ********************************************************************************************
********************************************************************************************************************
********************************************************************************************************************/

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name : Name
 * @note : Converted from WLED Effects
 * Speed slider sets amount of LEDs lit, intensity sets unlit
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
void mAnimatorLight::EffectAnim__Manual__PixelSetElsewhere()
{
  
  // uint16_t lit = 1 + SEGMENT.speed;
  // uint16_t unlit = 1 + SEGMENT.intensity;
  // bool drawingLit = true;
  // uint16_t cnt = 0;

  // for (uint16_t i = 0; i < SEGLEN; i++) {
  //   SEGMENT.SetPixelColor(i, 
  //     (drawingLit) ? RgbcctColor::GetU32Colour(SEGMENT.GetPaletteColour(i, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE)) : SEGCOLOR_U32(1)
  //   );
  //   cnt++;
  //   if (cnt >= ((drawingLit) ? lit : unlit)) {
  //     cnt = 0;
  //     drawingLit = !drawingLit;
  //   }
  // }

  // SEGMENT.SetPixelColor(0, RgbcctColor(255,0,0));
  // SEGMENT.SetPixelColor(1, RgbcctColor(0,255,0));
  // SEGMENT.SetPixelColor(2, RgbcctColor(0,0,255));
  
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  
}
static const char PM_EFFECT_CONFIG__MANUAL__PIXEL_SET_ELSEWHERE__INDEXING[] PROGMEM = "Debug Pixel Set Manually@Fg size,Bg size;Fg,!;!;;pal=19";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE








  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
// const uint32_t MUSIC_TIMING[] = {

// 0,
// 7,
// 240,
// 248,
// 488,
// 500,
// 739,
// 747,
// 1239,
// 1247,
// 1738,
// 1746,
// 2478,
// 2486,
// 2726,
// 2738,
// 3218,
// 3225,
// 3717,
// 3725,
// 4217,
// 4224,
// 4716,
// 4724,
// 5216,
// 5223,
// 5715,
// 5723,
// 6455,
// 6463,
// 6704,
// 6714,
// 7202,
// 7214,
// 7694,
// 7702,
// 8694,
// 8701,
// 9273,
// 9281,
// 9853,
// 9861,
// 10433,
// 10441,
// 11013,
// 11020,
// 11592,
// 11600,
// 12172,
// 12180,
// 12471,
// 12479,
// 12752,
// 12759,
// 13331,
// 13339,
// 13911,
// 13918,
// 14490,
// 14498,
// 15070,
// 15077,
// 15649,
// 15657,
// 16229,
// 16236,
// 16816,
// 16828,
// 17108,
// 17116,
// 17396,
// 17408,
// 17976,
// 17987,
// 18556,
// 18567,
// 19136,
// 19146,
// 19726,
// 19733,
// 20006,
// 20013,
// 20305,
// 20313,
// 20586,
// 20593,
// 20885,
// 20893,
// 21465,
// 21472,
// 22044,
// 22052,
// 22324,
// 22332,
// 22624,
// 22632,
// 22904,
// 22912,
// 23203,
// 23211,
// 23783,
// 23791,
// 24363,
// 24371,
// 24643,
// 24650,
// 24942,
// 24950,
// 25223,
// 25230,
// 25522,
// 25530,
// 26102,
// 26110,
// 26390,
// 26401,
// 26682,
// 26689,
// 27261,
// 27269,
// 27841,
// 27849,
// 28420,
// 28428,
// 29000,
// 29008,
// 29579,
// 29588,
// 30159,
// 30167,
// 30739,
// 30747,
// 31319,
// 31326,
// 31898,
// 31906,
// 32478,
// 32485,
// 33636,
// 33645,
// 34304,
// 34316,
// 34635,
// 34644,
// 34975,
// 34983,
// 35464,
// 35475,
// 35635,
// 35642,
// 35954,
// 35963,
// 36295,
// 36302,
// 36634,
// 36641,
// 36954,
// 36962,
// 37293,
// 37301,
// 37614,
// 37621,
// 37953,
// 37961,
// 38282,
// 38293,
// 38952,
// 38960,
// 39272,
// 39280,
// 39612,
// 39620,
// 39940,
// 39952,
// 40271,
// 40279,
// 40611,
// 40618,
// 40931,
// 40939,
// 41271,
// 41278,
// 41590,
// 41598,
// 41930,
// 41938,
// 42270,
// 42277,
// 43589,
// 43597,
// 44168,
// 44176,
// 44456,
// 44468,
// 44748,
// 44756,
// 45036,
// 45048,
// 45907,
// 45915,
// 46196,
// 46207,
// 46486,
// 46494,
// 46786,
// 46793,
// 47365,
// 47373,
// 47646,
// 47653,
// 47945,
// 47953,
// 48225,
// 48232,
// 48524,
// 48532,
// 49384,
// 49392,
// 49683,
// 49691,
// 49964,
// 49971,
// 50263,
// 50271,
// 50843,
// 50850,
// 51422,
// 51430,
// 51862,
// 51869,
// 52002,
// 52010,
// 52581,
// 52589,
// 53161,
// 53169,
// 53601,
// 53608,
// 53740,
// 53748,
// 54320,
// 54328,
// 54609,
// 54620,
// 54900,
// 54907,
// 55189,
// 55199,
// 55479,
// 55487,
// 56059,
// 56067,
// 56639,
// 56646,
// 57218,
// 57226,
// 57798,
// 57805,
// 58457,
// 58465,
// 58797,
// 58805,
// 59125,
// 59136,
// 59796,
// 59803,
// 60116,
// 60124,
// 60456,
// 60463,
// 60783,
// 60795,
// 61115,
// 61123,
// 61774,
// 61783,
// 62434,
// 62442,
// 62774,
// 62782,
// 63103,
// 63114,
// 63433,
// 63441,
// 63773,
// 63780,
// 64093,
// 64101,
// 64433,
// 64440,
// 64761,
// 64772,
// 65092,
// 65100,
// 65751,
// 65759,
// 66419,
// 66431,
// 66750,
// 66758,
// 67090,
// 67097,
// 67410,
// 67418,
// 67750,
// 67757,
// 68069,
// 68077,
// 68410,
// 68417,
// 68738,
// 68749,
// 69069,
// 69077,
// 69728,
// 69737,
// 70396,
// 70408,
// 71068,
// 71075,
// 71727,
// 71735,
// 72387,
// 72395,
// 73047,
// 73054,
// 74374,
// 74385,
// 74866,
// 74873,
// 75625,
// 75632,
// 75865,
// 75872,
// 76364,
// 76372,
// 76864,
// 76872,
// 77364,
// 77372,
// 77864,
// 77871,
// 78352,
// 78363,
// 78603,
// 78611,
// 78843,
// 78851,
// 79103,
// 79110,
// 79343,
// 79351,
// 79603,
// 79610,
// 79842,
// 79850,
// 80342,
// 80350,
// 80582,
// 80590,
// 80842,
// 80849,
// 81341,
// 81349,
// 81841,
// 81848,
// 82329,
// 82341,
// 82821,
// 82828,
// 83580,
// 83588,
// 83820,
// 83828,
// 84320,
// 84328,
// 84560,
// 84567,
// 84819,
// 84827,
// 85559,
// 85567,
// 85819,
// 85826,
// 86307,
// 86318,
// 86558,
// 86566,
// 86798,
// 86806,
// 87058,
// 87066,
// 87298,
// 87306,
// 87797,
// 87806,
// 88047,
// 88058,
// 88297,
// 88305,
// 88797,
// 88805,
// 89296,
// 89304,
// 89796,
// 89804,
// 90285,
// 90296,
// 90776,
// 90784,
// 91275,
// 91284,
// 91535,
// 91543,
// 92025,
// 92035,
// 92275,
// 92283,
// 92774,
// 92783,
// 93274,
// 93282,
// 93774,
// 93782,
// 94262,
// 94274,
// 94753,
// 94761,
// 95253,
// 95261,
// 95513,
// 95521,
// 95753,
// 95760,
// 96002,
// 96012,
// 96253,
// 96260,
// 96752,
// 96760,
// 97252,
// 97259,
// 97751,
// 97759,
// 98240,
// 98251,
// 98731,
// 98739,
// 99231,
// 99238,
// 99490,
// 99498,
// 99990,
// 99997,
// 100069

// };



const uint32_t MUSIC_TIMING[] = {
0,
7.972,
499.402,
507.345,
1239.155,
1246.915,
1486.988,
1498.504,
1978.485,
1986.135,
2477.608,
2485.89,
2977.531,
2985.234,
3476.742,
3485.012,
3976.526,
3984.354,
4216.1,
4224.27,
4476.055,
4483.668,
4715.713,
4723.855,
4975.646,
4983.101,
5215.28,
5223.448,
5475.251,
5482.568,
5963.141,
5974.626,
6214.72,
6222.387,
6454.313,
6462.491,
6954.357,
6962.023,
7453.624,
7461.892,
7953.691,
7961.352,
8452.793,
8461.126,
9192.867,
9200.642,
9452.332,
9460.077,
9940.75,
9952.133,
10191.923,
10200.067,
10432.143,
10439.687,
11191.14,
11199.189,
11431.529,
11439.092,
11930.861,
11939.134,
12178.845,
12190.851,
12430.409,
12438.662,
12670.817,
12678.358,
12930.18,
12938.213,
13430.08,
13437.6,
13669.854,
13678.009,
13919.112,
13929.893,
14409.374,
14417.651,
14909.454,
14917.125,
15408.764,
15417.023,
15908.825,
15916.49,
16408.118,
16416.381,
16907.866,
16915.69,
17147.64,
17155.794,
17647.684,
17655.34,
17907.063,
17914.782,
18395.001,
18406.844,
18886.432,
18894.685,
19386.443,
19394.129,
19885.738,
19893.997,
20385.762,
20393.437,
20884.842,
20893.168,
21125.234,
21132.779,
21384.583,
21392.539,
21624.889,
21632.418,
21884.245,
21892.017,
22372.704,
22384.026,
22863.555,
22871.818,
23363.529,
23371.224,
23862.805,
23871.069,
24362.828,
24370.49,
24861.942,
24870.356,
25102.289,
25109.86,
25601.557,
25609.85,
25861.549,
25869.021,
26350.888,
26360.99,
26848.94,
26860.828,
27340.425,
27348.692,
27840.485,
27848.167,
28339.784,
28348.051,
28839.847,
28847.528,
29079.292,
29087.475,
29339.325,
29346.962,
29578.959,
29587.14,
29838.977,
29846.462,
30338.364,
30346.149,
30826.407,
30838.185,
31817.477,
31825.872,
32817.334,
32825.151,
33316.531,
33324.865,
33816.602,
33824.242,
34316.244,
34324.005,
34556.58,
34564.162,
34804.449,
34816.025,
35296.2,
35303.891,
35795.556,
35803.833,
36295.647,
36303.341,
36794.925,
36803.228,
37055.158,
37059.831,


};

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Slow Glow 
 * @description:   : Randomly changes colours of pixels, and blends to the new one
 * 
 * @param intensity: 0-255 is how many should pixels should randomly change (0-255 scaled to 0-pixel_count)
 * @param Speed    : None
 * @param rate     : Period of time (ms) between updates
 * @param time     : Blend time
 * 
 * 
 * Method 1: Inside the music function, when aux0 is reseting, then "playTrack" should be started where another driver for the music should be called.
 *           Wait until finished, or with timeout, and then use the start time as the animation sync start and play the effect until the end.
 * 
 * Method 2: Could be started same as above. I may want to call any other effect and simple apply the music as a trigger. Or introduce animation trigger methods where
 *            - using speed/intensity
 *            - using sound reactive, which uses a microphone to then call each animation
 *            - using prerecorded tracks, ie christmas music controller
 * 
 *            Method 2 would allow animations to be applied to any current and future animation, and simply be triggering calls for the animation to update. 
 * 
 *            Playlists in this case could actually be used to literally link sound/animations together.
 * 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
void mAnimatorLight::EffectAnim__Christmas_Musical__01()
{

  uint16_t dataSize = GetSizeOfPixel(SEGMENT.colour_type__used_in_effect_generate) * 2 * SEGMENT.length(); //allocate space for 10 test pixels

  if (!SEGMENT.allocateData(dataSize)){    
    ALOG_ERR( PM_MEMORY_INSUFFICIENT );
    SEGMENT.effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
    return;
  }



  if(SEGMENT.params_internal.aux0 == 0)
  {

  }
  if(SEGMENT.params_internal.aux0 == ARRAY_SIZE(MUSIC_TIMING))
  {
    SEGMENT.params_internal.aux0 = 0;
    SEGMENT.tSaved_EffectStartReferenceTime = millis();
    
  }

  SEGMENT.params_internal.aux0++;


  uint32_t time_from_reference = millis() - SEGMENT.tSaved_EffectStartReferenceTime;


  // ALOG_INF(PSTR("EffectAnim__Christmas_Musical__01 aux0 = %d %d/%d"), SEGMENT.params_internal.aux0, 
  //   SEGMENT.tSaved_EffectStartReferenceTime,
  //   time_from_reference

  
  
  // );



  uint32_t next_time = 0;
  if(SEGMENT.params_internal.aux0 < ARRAY_SIZE(MUSIC_TIMING))
  {
    next_time = MUSIC_TIMING[SEGMENT.params_internal.aux0+1] - MUSIC_TIMING[SEGMENT.params_internal.aux0];

      
    // ALOG_INF(PSTR("EffectAnim__Christmas_Musical__01 aux0 = %d|%d next=%d"), 
    // MUSIC_TIMING[SEGMENT.params_internal.aux0+1], MUSIC_TIMING[SEGMENT.params_internal.aux0],
    // next_time
    // );





  }

  
  // for (uint16_t i = 0; i < c; i++) //fill from start until c
    // SEGMENT.SetPixelColor(0, RgbcctColor( SEGMENT.params_internal.aux0,0,0  ));
  // }

  SEGMENT.cycle_time__rate_ms = next_time;
  SEGMENT.time_ms = 0;
  // SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
  



  // ALOG_INF(PSTR("EffectAnim__Christmas_Musical__01 aux0 = %d %d/%d"), SEGMENT.params_internal.aux0, 
  //   SEGMENT.tSaved_EffectStartReferenceTime,
  //   time_from_reference
  // );


  uint16_t* region_p          = &SEGMENT.params_internal.aux0;
  uint16_t* indexes_active_p  = &SEGMENT.params_internal.aux1; // shared_flasher_parameters_segments.indexes.active
  uint16_t* indexes_counter_p = &SEGMENT.params_internal.aux2; // shared_flasher_parameters_segments.indexes.counter

  desired_pixel=0;
    
  uint8_t pixel_position = 0;
  uint8_t pixels_in_map = GetNumberOfColoursInPalette(SEGMENT.palette_id);

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

  // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEO "counter = %d/%d/%d"), counter,index_1,index_2);

  RgbcctColor colour;

  for(uint16_t index=SEGMENT.start;
                index<=SEGMENT.stop;
                index++
  ){

    if(counter^=1){
      desired_pixel = *indexes_counter_p ? index_2 : index_1;
    }else{
      desired_pixel = *indexes_counter_p ? index_1 : index_2;
    }
    
    colour = SEGMENT.GetPaletteColour(desired_pixel, PALETTE_SPAN_OFF, PALETTE_WRAP_OFF, PALETTE_DISCRETE_OFF, &pixel_position);
    
    colour = RgbcctColor::ApplyBrightnesstoRgbcctColour(colour, SEGMENT.getBrightnessRGB_WithGlobalApplied());

    SetTransitionColourBuffer_DesiredColour(SEGMENT.data, SEGMENT.DataLength(), index, SEGMENT.colour_type__used_in_effect_generate, colour);
        
  } 

//messy
  if(++*indexes_active_p>pixels_in_map-1){
    *indexes_active_p=0;
  }
  
  
  DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  
  SetSegment_AnimFunctionCallback(  SEGIDX, 
    [this](const AnimationParam& param){ 
      this->AnimationProcess_LinearBlend_Dynamic_Buffer(param); 
    }
  );


}
static const char PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01[] PROGMEM = "CHRISTMAS_MUSICAL_01@,,,,,Repeat Rate (ms);!,!,!,!,!;!"; // 7 sliders + 4 options before first ;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING

//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/
//********************************************************************************************************************************************************************************************************************/


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
/*
  Aurora effect
*/

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
      ttl = random(500, 1501);
      basecolor = color;
      basealpha = random(60, 101) / (float)100;
      age = 0;
      width = random(segment_length / 20, segment_length / W_WIDTH_FACTOR); //half of width to make math easier
      if (!width) width = 1;
      center = random(101) / (float)100 * segment_length;
      goingleft = random(0, 2) == 0;
      speed_factor = (random(10, 31) / (float)100 * W_MAX_SPEED / 255);
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

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EffectAnim__1D__Aurora()
{

// uint16_t mode_aurora(void) {
  //aux1 = Wavecount
  //aux2 = Intensity in last loop

  AuroraWave* waves;

//TODO: I am not sure this is a correct way of handling memory allocation since if it fails on 1st run
// it will display static effect but on second run it may crash ESP since data will be nullptr

  if(SEGMENT.params_internal.aux0 != SEGMENT.intensity || SEGMENT.call == 0) {
    //Intensity slider changed or first call
    SEGMENT.params_internal.aux1 = map(SEGMENT.intensity, 0, 255, 2, W_MAX_COUNT);
    SEGMENT.params_internal.aux0 = SEGMENT.intensity;

    if(!SEGMENT.allocateData(sizeof(AuroraWave) * SEGMENT.params_internal.aux1)) { // 26 on 32 segment ESP32, 9 on 16 segment ESP8266
      return EffectAnim__Solid_Colour(); //allocation failed
    }

    waves = reinterpret_cast<AuroraWave*>(SEGMENT.data);

    for (int i = 0; i < SEGMENT.params_internal.aux1; i++) {
      waves[i].init(SEGLEN, CRGB(SEGMENT.color_from_palette(random8(), false, false, random(0, 3))));
    }
  } else {
    waves = reinterpret_cast<AuroraWave*>(SEGMENT.data);
  }

  for (int i = 0; i < SEGMENT.params_internal.aux1; i++) {
    //Update values of wave
    waves[i].update(SEGLEN, SEGMENT.speed);

    if(!(waves[i].stillAlive())) {
      //If a wave dies, reinitialize it starts over.
      waves[i].init(SEGLEN, CRGB(SEGMENT.color_from_palette(random8(), false, false, random(0, 3))));
    }
  }

  uint8_t backlight = 1; //dimmer backlight if less active colors
  if (SEGCOLOR_U32(0)) backlight++;
  if (SEGCOLOR_U32(1)) backlight++;
  if (SEGCOLOR_U32(2)) backlight++;
  //Loop through LEDs to determine color
  for (int i = 0; i < SEGLEN; i++) {
    CRGB mixedRgb = CRGB(backlight, backlight, backlight);

    //For each LED we must check each wave if it is "active" at this position.
    //If there are multiple waves active on a LED we multiply their values.
    for (int  j = 0; j < SEGMENT.params_internal.aux1; j++) {
      CRGB rgb = waves[j].getColorForLED(i);

      if(rgb != CRGB(0)) {
        mixedRgb += rgb;
      }
    }

    SEGMENT.setPixelColor(i, mixedRgb[0], mixedRgb[1], mixedRgb[2]);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char _data_FX_MODE_AURORA[] PROGMEM = "Aurora@!,!;1,2,3;!;;sx=24,pal=50";

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO

// WLED-SR effects
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
void mAnimatorLight::EffectAnim__1D__PerlinMove()
{
/////////////////////////
//     Perlin Move     //
/////////////////////////
// 16 bit perlinmove. Use Perlin Noise instead of sinewaves for movement. By Andrew Tuline.
// Controls are speed, # of pixels, faderate.
// uint16_t mode_perlinmove(void) {
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  SEGMENT.fade_out(255-SEGMENT.custom1);
  for (int i = 0; i < SEGMENT.intensity/16 + 1; i++) {
    uint16_t locn = inoise16(millis()*128/(260-SEGMENT.speed)+i*15000, millis()*128/(260-SEGMENT.speed)); // Get a new pixel location from moving noise.
    uint16_t pixloc = map(locn, 50*256, 192*256, 0, SEGLEN-1);                                            // Map that to the length of the strand, and ensure we don't go over.
    SEGMENT.setPixelColor(pixloc, SEGMENT.color_from_palette(pixloc%255, false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_perlinmove()
static const char _data_FX_MODE_PERLINMOVE[] PROGMEM = "Perlin Move@!,# of pixels,Fade rate;!,!;!";

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     Waveins         //
/////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
void mAnimatorLight::EffectAnim__1D__Waveins()
{
// Uses beatsin8() + phase shifting. By: Andrew Tuline
// uint16_t mode_wavesins(void) {

  for (int i = 0; i < SEGLEN; i++) {
    uint8_t bri = sin8(millis()/4 + i * SEGMENT.intensity);
    uint8_t index = beatsin8(SEGMENT.speed, SEGMENT.custom1, SEGMENT.custom1+SEGMENT.custom2, 0, i * (SEGMENT.custom3<<3)); // custom3 is reduced resolution slider
    //SEGMENT.setPixelColor(i, ColorFromPalette_WithLoad(SEGPALETTE, index, bri, LINEARBLEND));
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0, bri));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_waveins()
static const char _data_FX_MODE_WAVESINS[] PROGMEM = "Wavesins@!,Brightness variation,Starting color,Range of colors,Color variation;!;!";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////////
//     Flow Stripe          //
//////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
// By: ldirko  https://editor.soulmatelights.com/gallery/392-flow-led-stripe , modifed by: Andrew Tuline
void mAnimatorLight::EffectAnim__1D__FlowStripe()
{
// uint16_t mode_FlowStripe(void) {

  const uint16_t hl = SEGLEN * 10 / 13;
  uint8_t hue = millis() / (SEGMENT.speed+1);
  uint32_t t = millis() / (SEGMENT.intensity/8+1);

  for (int i = 0; i < SEGLEN; i++) {
    int c = (abs(i - hl) / hl) * 127;
    c = sin8(c);
    c = sin8(c / 2 + t);
    byte b = sin8(c + t/8);
    SEGMENT.setPixelColor(i, CHSV(b + hue, 255, 255));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_FlowStripe()
static const char _data_FX_MODE_FLOWSTRIPE[] PROGMEM = "Flow Stripe@Hue speed,Effect speed;;";

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//***************************  2D routines  ***********************************
#define XY(x,y) SEGMENT.XY(x,y)


// Black hole  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Blackhole()
{
// uint16_t mode_2DBlackHole(void) {            // By: Stepko https://editor.soulmatelights.com/gallery/1012 , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();
  uint16_t x, y;

  SEGMENT.fadeToBlackBy(16 + (SEGMENT.speed>>3)); // create fading trails
  unsigned long t = millis()/128;                 // timebase
  // outer stars
  for (size_t i = 0; i < 8; i++) {
    x = beatsin8(SEGMENT.custom1>>3,   0, cols - 1, 0, ((i % 2) ? 128 : 0) + t * i);
    y = beatsin8(SEGMENT.intensity>>3, 0, rows - 1, 0, ((i % 2) ? 192 : 64) + t * i);
    SEGMENT.addPixelColorXY(x, y, SEGMENT.color_from_palette(i*32, false, PALETTE_SOLID_WRAP, SEGMENT.check1?0:255));
  }
  // inner stars
  for (size_t i = 0; i < 4; i++) {
    x = beatsin8(SEGMENT.custom2>>3, cols/4, cols - 1 - cols/4, 0, ((i % 2) ? 128 : 0) + t * i);
    y = beatsin8(SEGMENT.custom3   , rows/4, rows - 1 - rows/4, 0, ((i % 2) ? 192 : 64) + t * i);
    SEGMENT.addPixelColorXY(x, y, SEGMENT.color_from_palette(255-i*64, false, PALETTE_SOLID_WRAP, SEGMENT.check1?0:255));
  }
  // central white dot
  SEGMENT.setPixelColorXY_CRGB(cols/2, rows/2, WHITE);
  // blur everything a bit
  SEGMENT.blur(16);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DBlackHole()
static const char _data_FX_MODE_2DBLACKHOLE[] PROGMEM = "Black Hole@Fade rate,Outer Y freq.,Outer X freq.,Inner X freq.,Inner Y freq.,Solid;!;!;2;pal=11";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

////////////////////////////
//     2D Colored Bursts  //
////////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__ColouredBursts()
{
// uint16_t mode_2DColoredBursts() {              // By: ldirko   https://editor.soulmatelights.com/gallery/819-colored-bursts , modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.params_internal.aux0 = 0; // start with red hue
  }

  bool dot = SEGMENT.check3;
  bool grad = SEGMENT.check1;

  byte numLines = SEGMENT.intensity/16 + 1;

  SEGMENT.params_internal.aux0++;  // hue
  SEGMENT.fadeToBlackBy(40);
  for (size_t i = 0; i < numLines; i++) {
    byte x1 = beatsin8(2 + SEGMENT.speed/16, 0, (cols - 1));
    byte x2 = beatsin8(1 + SEGMENT.speed/16, 0, (cols - 1));
    byte y1 = beatsin8(5 + SEGMENT.speed/16, 0, (rows - 1), 0, i * 24);
    byte y2 = beatsin8(3 + SEGMENT.speed/16, 0, (rows - 1), 0, i * 48 + 64);
    CRGB color = ColorFromPalette_WithLoad(SEGPALETTE, i * 255 / numLines + (SEGMENT.params_internal.aux0&0xFF), 255, LINEARBLEND);

    byte xsteps = abs8(x1 - y1) + 1;
    byte ysteps = abs8(x2 - y2) + 1;
    byte steps = xsteps >= ysteps ? xsteps : ysteps;
    //Draw gradient line
    for (size_t j = 1; j <= steps; j++) {
      uint8_t rate = j * 255 / steps;
      byte dx = lerp8by8(x1, y1, rate);
      byte dy = lerp8by8(x2, y2, rate);
      //SEGMENT.setPixelColorXY_CRGB(dx, dy, grad ? color.nscale8_video(255-rate) : color); // use addPixelColorXY for different look
      SEGMENT.addPixelColorXY(dx, dy, color); // use setPixelColorXY for different look
      if (grad) SEGMENT.fadePixelColorXY(dx, dy, rate);
    }

    if (dot) { //add white point at the ends of line
      SEGMENT.setPixelColorXY_CRGB(x1, x2, WHITE);
      SEGMENT.setPixelColorXY_CRGB(y1, y2, DARKSLATEGRAY);
    }
  }
  if (SEGMENT.custom3) SEGMENT.blur(SEGMENT.custom3/2);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DColoredBursts()
static const char _data_FX_MODE_2DCOLOREDBURSTS[] PROGMEM = "Colored Bursts@Speed,# of lines,,,Blur,Gradient,,Dots;;!;2;c3=16";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////
//      2D DNA     //
/////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__DNA()
{
// uint16_t mode_2Ddna(void) {         // dna originally by by ldirko at https://pastebin.com/pCkkkzcs. Updated by Preyy. WLED conversion by Andrew Tuline.
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  SEGMENT.fadeToBlackBy(64);
  for (int i = 0; i < cols; i++) {
    SEGMENT.setPixelColorXY_CRGB(i, beatsin8(SEGMENT.speed/8, 0, rows-1, 0, i*4    ), ColorFromPalette_WithLoad(SEGPALETTE, i*5+millis()/17, beatsin8(5, 55, 255, 0, i*10), LINEARBLEND));
    SEGMENT.setPixelColorXY_CRGB(i, beatsin8(SEGMENT.speed/8, 0, rows-1, 0, i*4+128), ColorFromPalette_WithLoad(SEGPALETTE, i*5+128+millis()/17, beatsin8(5, 55, 255, 0, i*10+128), LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.intensity>>3);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Ddna()
static const char _data_FX_MODE_2DDNA[] PROGMEM = "DNA@Scroll speed,Blur;;!;2";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D DNA Spiral   //
/////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__DNASpiral()
{
// uint16_t mode_2DDNASpiral() {               // By: ldirko  https://editor.soulmatelights.com/gallery/810 , modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t speeds = SEGMENT.speed/2 + 1;
  uint8_t freq = SEGMENT.intensity/8;

  uint32_t ms = millis() / 20;
  SEGMENT.fadeToBlackBy(135);

  for (int i = 0; i < rows; i++) {
    uint16_t x  = beatsin8(speeds, 0, cols - 1, 0, i * freq) + beatsin8(speeds - 7, 0, cols - 1, 0, i * freq + 128);
    uint16_t x1 = beatsin8(speeds, 0, cols - 1, 0, 128 + i * freq) + beatsin8(speeds - 7, 0, cols - 1, 0, 128 + 64 + i * freq);
    uint8_t hue = (i * 128 / rows) + ms;
    // skip every 4th row every now and then (fade it more)
    if ((i + ms / 8) & 3) {
      // draw a gradient line between x and x1
      x = x / 2; x1 = x1 / 2;
      uint8_t steps = abs8(x - x1) + 1;
      for (size_t k = 1; k <= steps; k++) {
        uint8_t rate = k * 255 / steps;
        uint8_t dx = lerp8by8(x, x1, rate);
        //SEGMENT.setPixelColorXY_CRGB(dx, i, ColorFromPalette_WithLoad(SEGPALETTE, hue, 255, LINEARBLEND).nscale8_video(rate));
        SEGMENT.addPixelColorXY(dx, i, ColorFromPalette_WithLoad(SEGPALETTE, hue, 255, LINEARBLEND)); // use setPixelColorXY for different look
        SEGMENT.fadePixelColorXY(dx, i, rate);
      }
      SEGMENT.setPixelColorXY_CRGB(x, i, DARKSLATEGRAY);
      SEGMENT.setPixelColorXY_CRGB(x1, i, WHITE);
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DDNASpiral()
static const char _data_FX_MODE_2DDNASPIRAL[] PROGMEM = "DNA Spiral@Scroll speed,Y frequency;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Drift        //
/////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Drift()
{
// uint16_t mode_2DDrift() {              // By: Stepko   https://editor.soulmatelights.com/gallery/884-drift , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  SEGMENT.fadeToBlackBy(128);
  const uint16_t maxDim = MAX(cols, rows)/2;
  unsigned long t = millis() / (32 - (SEGMENT.speed>>3));
  unsigned long t_20 = t/20; // softhack007: pre-calculating this gives about 10% speedup
  for (float i = 1; i < maxDim; i += 0.25) {
    float angle = radians(t * (maxDim - i));
    uint16_t myX = (cols>>1) + (uint16_t)(sin_t(angle) * i) + (cols%2);
    uint16_t myY = (rows>>1) + (uint16_t)(cos_t(angle) * i) + (rows%2);
    SEGMENT.setPixelColorXY_CRGB(myX, myY, ColorFromPalette_WithLoad(SEGPALETTE, (i * 20) + t_20, 255, LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.intensity>>3);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DDrift()
static const char _data_FX_MODE_2DDRIFT[] PROGMEM = "Drift@Rotation speed,Blur amount;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////
//     2D Firenoise     //
//////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__FireNoise()
{
// uint16_t mode_2Dfirenoise(void) {               // firenoise2d. By Andrew Tuline. Yet another short routine.
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint16_t xscale = SEGMENT.intensity*4;
  uint32_t yscale = SEGMENT.speed*8;
  uint8_t indexx = 0;

  SEGPALETTE = CRGBPalette16( CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0),
                              CRGB::Red, CRGB::Red, CRGB::Red, CRGB::DarkOrange,
                              CRGB::DarkOrange,CRGB::DarkOrange, CRGB::Orange, CRGB::Orange,
                              CRGB::Yellow, CRGB::Orange, CRGB::Yellow, CRGB::Yellow);

  for (int j=0; j < cols; j++) {
    for (int i=0; i < rows; i++) {
      indexx = inoise8(j*yscale*rows/255, i*xscale+millis()/4);                                           // We're moving along our Perlin map.
      SEGMENT.setPixelColorXY_CRGB(j, i, ColorFromPalette_WithLoad(SEGPALETTE, min(i*(indexx)>>4, 255), i*255/cols, LINEARBLEND)); // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    } // for i
  } // for j

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Dfirenoise()
static const char _data_FX_MODE_2DFIRENOISE[] PROGMEM = "Firenoise@X scale,Y scale;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////////
//     2D Frizzles          //
//////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Frizzles()
{
// uint16_t mode_2DFrizzles(void) {                 // By: Stepko https://editor.soulmatelights.com/gallery/640-color-frizzles , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  SEGMENT.fadeToBlackBy(16);
  for (size_t i = 8; i > 0; i--) {
    SEGMENT.addPixelColorXY(beatsin8(SEGMENT.speed/8 + i, 0, cols - 1),
                            beatsin8(SEGMENT.intensity/8 - i, 0, rows - 1),
                            ColorFromPalette_WithLoad(SEGPALETTE, beatsin8(12, 0, 255), 255, LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.custom1>>3);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DFrizzles()
static const char _data_FX_MODE_2DFRIZZLES[] PROGMEM = "Frizzles@X frequency,Y frequency,Blur;;!;2";

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////////////////////////
//   2D Cellular Automata Game of life   //
///////////////////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
typedef struct ColorCount {
  CRGB color;
  int8_t count;
} colorCount;

void mAnimatorLight::EffectAnim__2D__GameOfLife()
{
// uint16_t mode_2Dgameoflife(void) { // Written by Ewoud Wijma, inspired by https://natureofcode.com/book/chapter-7-cellular-automata/ and https://github.com/DougHaber/nlife-color
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();
  const uint16_t dataSize = sizeof(CRGB) * SEGMENT.length();  // using width*height prevents reallocation if mirroring is enabled
  const uint16_t crcBufferLen = 2; //(SEGMENT.width() + SEGMENT.height())*71/100; // roughly sqrt(2)/2 for better repetition detection (Ewowi)

  if (!SEGMENT.allocateData(dataSize + sizeof(uint16_t)*crcBufferLen)) return EffectAnim__Solid_Colour(); //allocation failed
  CRGB *prevLeds = reinterpret_cast<CRGB*>(SEGMENT.data);
  uint16_t *crcBuffer = reinterpret_cast<uint16_t*>(SEGMENT.data + dataSize); 

  CRGB backgroundColor = SEGCOLOR_U32(1);

  if (SEGMENT.call == 0 || strip.now - SEGMENT.step > 3000) {
    SEGMENT.step = strip.now;
    SEGMENT.params_internal.aux0 = 0;
    random16_set_seed(millis()>>2); //seed the random generator

    //give the leds random state and colors (based on intensity, colors from palette or all posible colors are chosen)
    for (int x = 0; x < cols; x++) for (int y = 0; y < rows; y++) {
      uint8_t state = random8()%2;
      if (state == 0)
        SEGMENT.setPixelColorXY_CRGB(x,y, backgroundColor);
      else
        SEGMENT.setPixelColorXY_CRGB(x,y, SEGMENT.color_from_palette(random8(), false, PALETTE_SOLID_WRAP, 255));
    }

    for (int y = 0; y < rows; y++) for (int x = 0; x < cols; x++) prevLeds[XY(x,y)] = Black;
    memset(crcBuffer, 0, sizeof(uint16_t)*crcBufferLen);
  } else if (strip.now - SEGMENT.step < FRAMETIME_FIXED * (uint32_t)map(SEGMENT.speed,0,255,64,4)) {
    // update only when appropriate time passes (in 42 FPS slots)
    SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
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
      int16_t xx = x+i, yy = y+j;
      if (x+i < 0) xx = cols-1; else if (x+i >= cols) xx = 0;
      if (y+j < 0) yy = rows-1; else if (y+j >= rows) yy = 0;

      uint16_t xy = XY(xx, yy); // previous cell xy to check
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
    if      ((col != bgc) && (neighbors <  2)) SEGMENT.setPixelColorXY_CRGB(x,y, bgc); // Loneliness
    else if ((col != bgc) && (neighbors >  3)) SEGMENT.setPixelColorXY_CRGB(x,y, bgc); // Overpopulation
    else if ((col == bgc) && (neighbors == 3)) {                                  // Reproduction
      // find dominant color and assign it to a cell
      colorCount dominantColorCount = {backgroundColor, 0};
      for (int i=0; i<9 && colorsCount[i].count != 0; i++)
        if (colorsCount[i].count > dominantColorCount.count) dominantColorCount = colorsCount[i];
      // assign the dominant color w/ a bit of randomness to avoid "gliders"
      if (dominantColorCount.count > 0 && random8(128)) SEGMENT.setPixelColorXY_CRGB(x,y, dominantColorCount.color);
    } else if ((col == bgc) && (neighbors == 2) && !random8(128)) {               // Mutation
      SEGMENT.setPixelColorXY_CRGB(x,y, SEGMENT.color_from_palette(random8(), false, PALETTE_SOLID_WRAP, 255));
    }
    // else do nothing!
  } //x,y

  // calculate CRC16 of leds
  uint16_t crc = crc16((const unsigned char*)prevLeds, dataSize);
  // check if we had same CRC and reset if needed
  bool repetition = false;
  for (int i=0; i<crcBufferLen && !repetition; i++) repetition = (crc == crcBuffer[i]); // (Ewowi)
  // same CRC would mean image did not change or was repeating itself
  if (!repetition) SEGMENT.step = strip.now; //if no repetition avoid reset
  // remember CRCs across frames
  crcBuffer[SEGMENT.params_internal.aux0] = crc;
  ++SEGMENT.params_internal.aux0 %= crcBufferLen;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Dgameoflife()
static const char _data_FX_MODE_2DGAMEOFLIFE[] PROGMEM = "Game Of Life@!;!,!;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Hiphotic     //
/////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Hipnotic()
{
// uint16_t mode_2DHiphotic() {                        //  By: ldirko  https://editor.soulmatelights.com/gallery/810 , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();
  const uint32_t a = strip.now / ((SEGMENT.custom3>>1)+1);

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      SEGMENT.setPixelColorXY_CRGB(x, y, SEGMENT.color_from_palette(sin8(cos8(x * SEGMENT.speed/16 + a / 3) + sin8(y * SEGMENT.intensity/16 + a / 4) + a), false, PALETTE_SOLID_WRAP, 0));
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DHiphotic()
static const char _data_FX_MODE_2DHIPHOTIC[] PROGMEM = "Hiphotic@X scale,Y scale,,,Speed;!;!;2";
#endif //   #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Julia        //
/////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
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

void mAnimatorLight::EffectAnim__2D__Julia()
{
// uint16_t mode_2DJulia(void) {                           // An animated Julia set by Andrew Tuline.
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (!SEGMENT.allocateData(sizeof(julia))) return EffectAnim__Solid_Colour();
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

  reAl += sin_t((float)millis()/305.f)/20.f;
  imAg += sin_t((float)millis()/405.f)/20.f;

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
        SEGMENT.setPixelColorXY_CRGB(i, j, 0);
      } else {
        SEGMENT.setPixelColorXY_CRGB(i, j, SEGMENT.color_from_palette(iter*255/maxIterations, false, PALETTE_SOLID_WRAP, 0));
      }
      x += dx;
    }
    y += dy;
  }
//  SEGMENT.blur(64);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DJulia()
static const char _data_FX_MODE_2DJULIA[] PROGMEM = "Julia@,Max iterations per pixel,X center,Y center,Area size;!;!;2;ix=24,c1=128,c2=128,c3=16";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////////
//     2D Lissajous         //
//////////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Lissajous()
{
// uint16_t mode_2DLissajous(void) {            // By: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  SEGMENT.fadeToBlackBy(SEGMENT.intensity);
  uint_fast16_t phase = (millis() * (1 + SEGMENT.custom3)) /32;  // allow user to control rotation speed

  //for (int i=0; i < 4*(cols+rows); i ++) {
  for (int i=0; i < 256; i ++) {
    //float xlocn = float(sin8(now/4+i*(SEGMENT.speed>>5))) / 255.0f;
    //float ylocn = float(cos8(now/4+i*2)) / 255.0f;
    uint_fast8_t xlocn = sin8(phase/2 + (i*SEGMENT.speed)/32);
    uint_fast8_t ylocn = cos8(phase/2 + i*2);
    xlocn = (cols < 2) ? 1 : (map(2*xlocn, 0,511, 0,2*(cols-1)) +1) /2;    // softhack007: "(2* ..... +1) /2" for proper rounding
    ylocn = (rows < 2) ? 1 : (map(2*ylocn, 0,511, 0,2*(rows-1)) +1) /2;    // "rows > 1" is needed to avoid div/0 in map()
    SEGMENT.setPixelColorXY_CRGB((uint8_t)xlocn, (uint8_t)ylocn, SEGMENT.color_from_palette(millis()/100+i, false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DLissajous()
static const char _data_FX_MODE_2DLISSAJOUS[] PROGMEM = "Lissajous@X frequency,Fade rate,,,Speed;!;!;2;;c3=15";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////
//    2D Matrix      //
///////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Matrix()
{
// uint16_t mode_2Dmatrix(void) {                  // Matrix2D. By Jeremy Williams. Adapted by Andrew Tuline & improved by merkisoft and ewowi, and softhack007.
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.params_internal.aux0 = SEGMENT.params_internal.aux1 = UINT16_MAX;
    SEGMENT.step = 0;
  }

  uint8_t fade = map(SEGMENT.custom1, 0, 255, 50, 250);    // equals trail size
  uint8_t speed = (256-SEGMENT.speed) >> map(MIN(rows, 150), 0, 150, 0, 3);    // slower speeds for small displays

  CRGB spawnColor;
  CRGB trailColor;
  if (SEGMENT.check1) {
    spawnColor = SEGCOLOR_U32(0);
    trailColor = SEGCOLOR_U32(1);
  } else {
    spawnColor = CRGB(175,255,175);
    trailColor = CRGB(27,130,39);
  }

  if (strip.now - SEGMENT.step >= speed) {
    SEGMENT.step = strip.now;
    // find out what color value is returned by gPC for a "falling code" example pixel
    // the color values returned may differ from the previously set values, due to
    // - auto brightness limiter (dimming)
    // - lossy color buffer (when not using global buffer)
    // - color balance correction
    // - segment opacity
    CRGB oldSpawnColor = spawnColor;
    if ((SEGMENT.params_internal.aux0 < cols) && (SEGMENT.params_internal.aux1 < rows)) {                     // we have a hint from last run
        oldSpawnColor = SEGMENT.getPixelColorXY(SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1);  // find color of previous spawns
        SEGMENT.params_internal.aux1 ++;                                                     // our sample pixel will be one row down the next time
    }
    if ((oldSpawnColor == Black) || (oldSpawnColor == trailColor)) oldSpawnColor = spawnColor; // reject "black", as it would mean that ALL pixels create trails

    // move pixels one row down. Falling codes keep color and add trail pixels; all others pixels are faded
    for (int row=rows-1; row>=0; row--) {
      for (int col=0; col<cols; col++) {
        CRGB pix = SEGMENT.getPixelColorXY(col, row);
        if (pix == oldSpawnColor) {  // this comparison may still fail due to overlays changing pixels, or due to gaps (2d-gaps.json)
          SEGMENT.setPixelColorXY_CRGB(col, row, trailColor);  // create trail
          if (row < rows-1) SEGMENT.setPixelColorXY_CRGB(col, row+1, spawnColor);
        } else {
          // fade other pixels
          if (pix != Black) SEGMENT.setPixelColorXY_CRGB(col, row, pix.nscale8(fade)); // optimization: don't fade black pixels
        }
      }
    }

    // check for empty screen to ensure code spawn
    bool emptyScreen = (SEGMENT.params_internal.aux1 >= rows); // empty screen means that the last falling code has moved out of screen area

    // spawn new falling code
    if (random8() <= SEGMENT.intensity || emptyScreen) {
      uint8_t spawnX = random8(cols);
      SEGMENT.setPixelColorXY_CRGB(spawnX, 0, spawnColor);
      // update hint for next run
      SEGMENT.params_internal.aux0 = spawnX;
      SEGMENT.params_internal.aux1 = 0;
    }
  } // if millis

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Dmatrix()
static const char _data_FX_MODE_2DMATRIX[] PROGMEM = "Matrix@!,Spawning rate,Trail,,,Custom color;Spawn,Trail;;2";

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Metaballs    //
/////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Metaballs()
{
// uint16_t mode_2Dmetaballs(void) {   // Metaballs by Stefan Petrick. Cannot have one of the dimensions be 2 or less. Adapted by Andrew Tuline.
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  float speed = 0.25f * (1+(SEGMENT.speed>>6));

  // get some 2 random moving points
  uint8_t x2 = map(inoise8(strip.now * speed, 25355, 685), 0, 255, 0, cols-1);
  uint8_t y2 = map(inoise8(strip.now * speed, 355, 11685), 0, 255, 0, rows-1);

  uint8_t x3 = map(inoise8(strip.now * speed, 55355, 6685), 0, 255, 0, cols-1);
  uint8_t y3 = map(inoise8(strip.now * speed, 25355, 22685), 0, 255, 0, rows-1);

  // and one Lissajou function
  uint8_t x1 = beatsin8(23 * speed, 0, cols-1);
  uint8_t y1 = beatsin8(28 * speed, 0, rows-1);

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      // calculate distances of the 3 points from actual pixel
      // and add them together with weightening
      uint16_t dx = abs(x - x1);
      uint16_t dy = abs(y - y1);
      uint16_t dist = 2 * sqrt16((dx * dx) + (dy * dy));

      dx = abs(x - x2);
      dy = abs(y - y2);
      dist += sqrt16((dx * dx) + (dy * dy));

      dx = abs(x - x3);
      dy = abs(y - y3);
      dist += sqrt16((dx * dx) + (dy * dy));

      // inverse result
      byte color = dist ? 1000 / dist : 255;

      // map color between thresholds
      if (color > 0 and color < 60) {
        SEGMENT.setPixelColorXY_CRGB(x, y, SEGMENT.color_from_palette(map(color * 9, 9, 531, 0, 255), false, PALETTE_SOLID_WRAP, 0));
      } else {
        SEGMENT.setPixelColorXY_CRGB(x, y, SEGMENT.color_from_palette(0, false, PALETTE_SOLID_WRAP, 0));
      }
      // show the 3 points, too
      SEGMENT.setPixelColorXY_CRGB(x1, y1, WHITE);
      SEGMENT.setPixelColorXY_CRGB(x2, y2, WHITE);
      SEGMENT.setPixelColorXY_CRGB(x3, y3, WHITE);
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Dmetaballs()
static const char _data_FX_MODE_2DMETABALLS[] PROGMEM = "Metaballs@!;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//    2D Noise      //
//////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Noise()
{
// uint16_t mode_2Dnoise(void) {                  // By Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  const uint16_t scale  = SEGMENT.intensity+2;

  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < cols; x++) {
      uint8_t pixelHue8 = inoise8(x * scale, y * scale, millis() / (16 - SEGMENT.speed/16));
      SEGMENT.setPixelColorXY_CRGB(x, y, ColorFromPalette_WithLoad(SEGPALETTE, pixelHue8));
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Dnoise()
static const char _data_FX_MODE_2DNOISE[] PROGMEM = "Noise2D@!,Scale;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////////
//     2D Plasma Ball       //
//////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__PlasmaBall()
{
  // uint16_t mode_2DPlasmaball(void) {                   // By: Stepko https://editor.soulmatelights.com/gallery/659-plasm-ball , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  SEGMENT.fadeToBlackBy(SEGMENT.custom1>>2);
  uint_fast32_t t = (millis() * 8) / (256 - SEGMENT.speed);  // optimized to avoid float
  for (int i = 0; i < cols; i++) {
    uint16_t thisVal = inoise8(i * 30, t, t);
    uint16_t thisMax = map(thisVal, 0, 255, 0, cols-1);
    for (int j = 0; j < rows; j++) {
      uint16_t thisVal_ = inoise8(t, j * 30, t);
      uint16_t thisMax_ = map(thisVal_, 0, 255, 0, rows-1);
      uint16_t x = (i + thisMax_ - cols / 2);
      uint16_t y = (j + thisMax - cols / 2);
      uint16_t cx = (i + thisMax_);
      uint16_t cy = (j + thisMax);

      SEGMENT.addPixelColorXY(i, j, ((x - y > -2) && (x - y < 2)) ||
                                    ((cols - 1 - x - y) > -2 && (cols - 1 - x - y < 2)) ||
                                    (cols - cx == 0) ||
                                    (cols - 1 - cx == 0) ||
                                    ((rows - cy == 0) ||
                                    (rows - 1 - cy == 0)) ? ColorFromPalette_WithLoad(SEGPALETTE, beat8(5), thisVal, LINEARBLEND) : Black);
    }
  }
  SEGMENT.blur(SEGMENT.custom2>>5);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DPlasmaball()
static const char _data_FX_MODE_2DPLASMABALL[] PROGMEM = "Plasma Ball@Speed,,Fade,Blur;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

////////////////////////////////
//  2D Polar Lights           //
////////////////////////////////
//static float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
//  return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
//}
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__PolarLights()
{
// uint16_t mode_2DPolarLights(void) {        // By: Kostyantyn Matviyevskyy  https://editor.soulmatelights.com/gallery/762-polar-lights , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  CRGBPalette16 auroraPalette  = {0x000000, 0x003300, 0x006600, 0x009900, 0x00cc00, 0x00ff00, 0x33ff00, 0x66ff00, 0x99ff00, 0xccff00, 0xffff00, 0xffcc00, 0xff9900, 0xff6600, 0xff3300, 0xff0000};

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.step = 0;
  }

  float adjustHeight = (float)map(rows, 8, 32, 28, 12); // maybe use mapf() ???
  uint16_t adjScale = map(cols, 8, 64, 310, 63);
/*
  if (SEGMENT.params_internal.aux1 != SEGMENT.custom1/12) {   // Hacky palette rotation. We need that black.
    SEGMENT.params_internal.aux1 = SEGMENT.custom1/12;
    for (int i = 0; i < 16; i++) {
      long ilk;
      ilk = (long)currentPalette[i].r << 16;
      ilk += (long)currentPalette[i].g << 8;
      ilk += (long)currentPalette[i].b;
      ilk = (ilk << SEGMENT.params_internal.aux1) | (ilk >> (24 - SEGMENT.params_internal.aux1));
      currentPalette[i].r = ilk >> 16;
      currentPalette[i].g = ilk >> 8;
      currentPalette[i].b = ilk;
    }
  }
*/
  uint16_t _scale = map(SEGMENT.intensity, 0, 255, 30, adjScale);
  byte _speed = map(SEGMENT.speed, 0, 255, 128, 16);

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      SEGMENT.step++;
      SEGMENT.setPixelColorXY_CRGB(x, y, ColorFromPalette_WithLoad(auroraPalette,
                                      qsub8(
                                        inoise8((SEGMENT.step%2) + x * _scale, y * 16 + SEGMENT.step % 16, SEGMENT.step / _speed),
                                        fabsf((float)rows / 2.0f - (float)y) * adjustHeight)));
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DPolarLights()
static const char _data_FX_MODE_2DPOLARLIGHTS[] PROGMEM = "Polar Lights@!,Scale;;;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Pulser       //
/////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Pulser()
{
// uint16_t mode_2DPulser(void) {                       // By: ldirko   https://editor.soulmatelights.com/gallery/878-pulse-test , modifed by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  SEGMENT.fadeToBlackBy(8 - (SEGMENT.intensity>>5));
  uint32_t a = strip.now / (18 - SEGMENT.speed / 16);
  uint16_t x = (a / 14) % cols;
  uint16_t y = map((sin8(a * 5) + sin8(a * 4) + sin8(a * 2)), 0, 765, rows-1, 0);
  SEGMENT.setPixelColorXY_CRGB(x, y, ColorFromPalette_WithLoad(SEGPALETTE, map(y, 0, rows-1, 0, 255), 255, LINEARBLEND));

  SEGMENT.blur(1 + (SEGMENT.intensity>>4));

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DPulser()
static const char _data_FX_MODE_2DPULSER[] PROGMEM = "Pulser@!,Blur;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Sindots      //
/////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__SinDots()
{
// uint16_t mode_2DSindots(void) {                             // By: ldirko   https://editor.soulmatelights.com/gallery/597-sin-dots , modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  SEGMENT.fadeToBlackBy(SEGMENT.custom1>>3);

  byte t1 = millis() / (257 - SEGMENT.speed); // 20;
  byte t2 = sin8(t1) / 4 * 2;
  for (int i = 0; i < 13; i++) {
    byte x = sin8(t1 + i * SEGMENT.intensity/8)*(cols-1)/255;  // max index now 255x15/255=15!
    byte y = sin8(t2 + i * SEGMENT.intensity/8)*(rows-1)/255;  // max index now 255x15/255=15!
    SEGMENT.setPixelColorXY_CRGB(x, y, ColorFromPalette_WithLoad(SEGPALETTE, i * 255 / 13, 255, LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.custom2>>3);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DSindots()
static const char _data_FX_MODE_2DSINDOTS[] PROGMEM = "Sindots@!,Dot distance,Fade rate,Blur;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////////
//     2D Squared Swirl     //
//////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__SqauredSwirl()
{
// custom3 affects the blur amount.
// uint16_t mode_2Dsquaredswirl(void) {            // By: Mark Kriegsman. https://gist.github.com/kriegsman/368b316c55221134b160
                                                          // Modifed by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  const uint8_t kBorderWidth = 2;

  SEGMENT.fadeToBlackBy(24);
  uint8_t blurAmount = SEGMENT.custom3>>1; // reduced resolution slider
  SEGMENT.blur(blurAmount);

  // Use two out-of-sync sine waves
  uint8_t i = beatsin8(19, kBorderWidth, cols-kBorderWidth);
  uint8_t j = beatsin8(22, kBorderWidth, cols-kBorderWidth);
  uint8_t k = beatsin8(17, kBorderWidth, cols-kBorderWidth);
  uint8_t m = beatsin8(18, kBorderWidth, rows-kBorderWidth);
  uint8_t n = beatsin8(15, kBorderWidth, rows-kBorderWidth);
  uint8_t p = beatsin8(20, kBorderWidth, rows-kBorderWidth);

  uint16_t ms = millis();

  SEGMENT.addPixelColorXY(i, m, ColorFromPalette_WithLoad(SEGPALETTE, ms/29, 255, LINEARBLEND));
  SEGMENT.addPixelColorXY(j, n, ColorFromPalette_WithLoad(SEGPALETTE, ms/41, 255, LINEARBLEND));
  SEGMENT.addPixelColorXY(k, p, ColorFromPalette_WithLoad(SEGPALETTE, ms/73, 255, LINEARBLEND));

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2Dsquaredswirl()
static const char _data_FX_MODE_2DSQUAREDSWIRL[] PROGMEM = "Squared Swirl@,,,,Blur;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////////////
//     2D Sun Radiation     //
//////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__SunRadiation()
{
// uint16_t mode_2DSunradiation(void) {                   // By: ldirko https://editor.soulmatelights.com/gallery/599-sun-radiation  , modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (!SEGMENT.allocateData(sizeof(byte)*(cols+2)*(rows+2))) return EffectAnim__Solid_Colour(); //allocation failed
  byte *bump = reinterpret_cast<byte*>(SEGMENT.data);

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  unsigned long t = millis() / 4;
  int index = 0;
  uint8_t someVal = SEGMENT.speed/4;             // Was 25.
  for (int j = 0; j < (rows + 2); j++) {
    for (int i = 0; i < (cols + 2); i++) {
      byte col = (inoise8_raw(i * someVal, j * someVal, t)) / 2;
      bump[index++] = col;
    }
  }

  int yindex = cols + 3;
  int16_t vly = -(rows / 2 + 1);
  for (int y = 0; y < rows; y++) {
    ++vly;
    int16_t vlx = -(cols / 2 + 1);
    for (int x = 0; x < cols; x++) {
      ++vlx;
      int8_t nx = bump[x + yindex + 1] - bump[x + yindex - 1];
      int8_t ny = bump[x + yindex + (cols + 2)] - bump[x + yindex - (cols + 2)];
      byte difx = abs8(vlx * 7 - nx);
      byte dify = abs8(vly * 7 - ny);
      int temp = difx * difx + dify * dify;
      int col = 255 - temp / 8; //8 its a size of effect
      if (col < 0) col = 0;
      SEGMENT.setPixelColorXY_CRGB(x, y, HeatColor(col / (3.0f-(float)(SEGMENT.intensity)/128.f)));
    }
    yindex += (cols + 2);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DSunradiation()
static const char _data_FX_MODE_2DSUNRADIATION[] PROGMEM = "Sun Radiation@Variance,Brightness;;;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Tartan       //
/////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__Tartan()
{
// uint16_t mode_2Dtartan(void) {          // By: Elliott Kember  https://editor.soulmatelights.com/gallery/3-tartan , Modified by: Andrew Tuline
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t hue, bri;
  size_t intensity;
  int offsetX = beatsin16(3, -360, 360);
  int offsetY = beatsin16(2, -360, 360);
  int sharpness = SEGMENT.custom3 / 8; // 0-3

  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      hue = x * beatsin16(10, 1, 10) + offsetY;
      intensity = bri = sin8(x * SEGMENT.speed/2 + offsetX);
      for (int i=0; i<sharpness; i++) intensity *= bri;
      intensity >>= 8*sharpness;
      SEGMENT.setPixelColorXY_CRGB(x, y, ColorFromPalette_WithLoad(SEGPALETTE, hue, intensity, LINEARBLEND));
      hue = y * 3 + offsetX;
      intensity = bri = sin8(y * SEGMENT.intensity/2 + offsetY);
      for (int i=0; i<sharpness; i++) intensity *= bri;
      intensity >>= 8*sharpness;
      SEGMENT.addPixelColorXY(x, y, ColorFromPalette_WithLoad(SEGPALETTE, hue, intensity, LINEARBLEND));
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DTartan()
static const char _data_FX_MODE_2DTARTAN[] PROGMEM = "Tartan@X scale,Y scale,,,Sharpness;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D spaceships   //
/////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
void mAnimatorLight::EffectAnim__2D__SpaceShips()
{
// uint16_t mode_2Dspaceships(void) {    //// Space ships by stepko (c)05.02.21 [https://editor.soulmatelights.com/gallery/639-space-ships], adapted by Blaz Kristan (AKA blazoncek)
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  uint32_t tb = strip.now >> 12;  // every ~4s
  if (tb > SEGMENT.step) {
    int8_t dir = ++SEGMENT.params_internal.aux0;
    dir  += (int)random8(3)-1;
    if      (dir > 7) SEGMENT.params_internal.aux0 = 0;
    else if (dir < 0) SEGMENT.params_internal.aux0 = 7;
    else              SEGMENT.params_internal.aux0 = dir;
    SEGMENT.step = tb + random8(4);
  }

  SEGMENT.fadeToBlackBy(map(SEGMENT.speed, 0, 255, 248, 16));
  SEGMENT.move(SEGMENT.params_internal.aux0, 1);

  for (size_t i = 0; i < 8; i++) {
    byte x = beatsin8(12 + i, 2, cols - 3);
    byte y = beatsin8(15 + i, 2, rows - 3);
    CRGB color = ColorFromPalette_WithLoad(SEGPALETTE, beatsin8(12 + i, 0, 255), 255);
    SEGMENT.addPixelColorXY(x, y, color);
    if (cols > 24 || rows > 24) {
      SEGMENT.addPixelColorXY(x+1, y, color);
      SEGMENT.addPixelColorXY(x-1, y, color);
      SEGMENT.addPixelColorXY(x, y+1, color);
      SEGMENT.addPixelColorXY(x, y-1, color);
    }
  }
  SEGMENT.blur(SEGMENT.intensity>>3);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char _data_FX_MODE_2DSPACESHIPS[] PROGMEM = "Spaceships@!,Blur;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Crazy Bees   //
/////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
//// Crazy bees by stepko (c)12.02.21 [https://editor.soulmatelights.com/gallery/651-crazy-bees], adapted by Blaz Kristan (AKA blazoncek)
#define MAX_BEES 5
void mAnimatorLight::EffectAnim__2D__CrazyBees()
{
// uint16_t mode_2Dcrazybees(void) {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  byte n = MIN(MAX_BEES, (rows * cols) / 256 + 1);

  typedef struct Bee {
    uint8_t posX, posY, aimX, aimY, hue;
    int8_t deltaX, deltaY, signX, signY, error;
    void aimed(uint16_t w, uint16_t h) {
      random16_set_seed(millis());
      aimX = random8(0, w);
      aimY = random8(0, h);
      hue = random8();
      deltaX = abs(aimX - posX);
      deltaY = abs(aimY - posY);
      signX = posX < aimX ? 1 : -1;
      signY = posY < aimY ? 1 : -1;
      error = deltaX - deltaY;
    };
  } bee_t;

  if (!SEGMENT.allocateData(sizeof(bee_t)*MAX_BEES)) return EffectAnim__Solid_Colour(); //allocation failed
  bee_t *bee = reinterpret_cast<bee_t*>(SEGMENT.data);

  if (SEGMENT.call == 0) {
    for (size_t i = 0; i < n; i++) {
      bee[i].posX = random8(0, cols);
      bee[i].posY = random8(0, rows);
      bee[i].aimed(cols, rows);
    }
  }

  if (millis() > SEGMENT.step) {
    SEGMENT.step = millis() + (FRAMETIME * 16 / ((SEGMENT.speed>>4)+1));

    SEGMENT.fadeToBlackBy(32);

    for (size_t i = 0; i < n; i++) {
      SEGMENT.addPixelColorXY(bee[i].aimX + 1, bee[i].aimY, CHSV(bee[i].hue, 255, 255));
      SEGMENT.addPixelColorXY(bee[i].aimX, bee[i].aimY + 1, CHSV(bee[i].hue, 255, 255));
      SEGMENT.addPixelColorXY(bee[i].aimX - 1, bee[i].aimY, CHSV(bee[i].hue, 255, 255));
      SEGMENT.addPixelColorXY(bee[i].aimX, bee[i].aimY - 1, CHSV(bee[i].hue, 255, 255));
      if (bee[i].posX != bee[i].aimX || bee[i].posY != bee[i].aimY) {
        SEGMENT.setPixelColorXY_CRGB(bee[i].posX, bee[i].posY, CRGB(CHSV(bee[i].hue, 60, 255)));
        int8_t error2 = bee[i].error * 2;
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
    SEGMENT.blur(SEGMENT.intensity>>4);
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char _data_FX_MODE_2DCRAZYBEES[] PROGMEM = "Crazy Bees@!,Blur;;;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//     2D Ghost Rider  //
/////////////////////////  
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
//// Ghost Rider by stepko (c)2021 [https://editor.soulmatelights.com/gallery/716-ghost-rider], adapted by Blaz Kristan (AKA blazoncek)
#define LIGHTERS_AM 64  // max lighters (adequate for 32x32 matrix)
void mAnimatorLight::EffectAnim__2D__GhostRider()
{
// uint16_t mode_2Dghostrider(void) {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

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

  if (!SEGMENT.allocateData(sizeof(lighter_t))) return EffectAnim__Solid_Colour(); //allocation failed
  lighter_t *lighter = reinterpret_cast<lighter_t*>(SEGMENT.data);

  const size_t maxLighters = min(cols + rows, LIGHTERS_AM);

  if (SEGMENT.params_internal.aux0 != cols || SEGMENT.params_internal.aux1 != rows) {
    SEGMENT.params_internal.aux0 = cols;
    SEGMENT.params_internal.aux1 = rows;
    random16_set_seed(strip.now);
    lighter->angleSpeed = random8(0,20) - 10;
    lighter->gAngle = random16();
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

  if (millis() > SEGMENT.step) {
    SEGMENT.step = millis() + 1024 / (cols+rows);

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
      lighter->time[i] += random8(5, 20);
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
        lighter->Angle[i] = lighter->gAngle + random(-10, 10);
        lighter->time[i] = 0;
        lighter->reg[i] = false;
      } else {
        lighter->lightersPosX[i] += -7 * sin_t(radians(lighter->Angle[i]));
        lighter->lightersPosY[i] += -7 * cos_t(radians(lighter->Angle[i]));
      }
      SEGMENT.wu_pixel(lighter->lightersPosX[i] * 256 / 10, lighter->lightersPosY[i] * 256 / 10, ColorFromPalette_WithLoad(SEGPALETTE, (256 - lighter->time[i])));
    }
    SEGMENT.blur(SEGMENT.intensity>>3);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char _data_FX_MODE_2DGHOSTRIDER[] PROGMEM = "Ghost Rider@Fade rate,Blur;;!;2";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

////////////////////////////
//     2D Floating Blobs  //
////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
//// Floating Blobs by stepko (c)2021 [https://editor.soulmatelights.com/gallery/573-blobs], adapted by Blaz Kristan (AKA blazoncek)
#define MAX_BLOBS 8

void mAnimatorLight::EffectAnim__2D__FloatingBlobs()
{
  // uint16_t mode_2Dfloatingblobs(void) {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  typedef struct Blob {
    float x[MAX_BLOBS], y[MAX_BLOBS];
    float sX[MAX_BLOBS], sY[MAX_BLOBS]; // speed
    float r[MAX_BLOBS];
    bool grow[MAX_BLOBS];
    byte color[MAX_BLOBS];
  } blob_t;

  uint8_t Amount = (SEGMENT.intensity>>5) + 1; // NOTE: be sure to update MAX_BLOBS if you change this

  if (!SEGMENT.allocateData(sizeof(blob_t))) return EffectAnim__Solid_Colour(); //allocation failed
  blob_t *blob = reinterpret_cast<blob_t*>(SEGMENT.data);

  if (SEGMENT.params_internal.aux0 != cols || SEGMENT.params_internal.aux1 != rows) {
    SEGMENT.params_internal.aux0 = cols; // re-initialise if virtual size changes
    SEGMENT.params_internal.aux1 = rows;
    //SEGMENT.fill(BLACK);
    for (size_t i = 0; i < MAX_BLOBS; i++) {
      blob->r[i]  = random8(1, cols>8 ? (cols/4) : 2);
      blob->sX[i] = (float) random8(3, cols) / (float)(256 - SEGMENT.speed); // speed x
      blob->sY[i] = (float) random8(3, rows) / (float)(256 - SEGMENT.speed); // speed y
      blob->x[i]  = random8(0, cols-1);
      blob->y[i]  = random8(0, rows-1);
      blob->color[i] = random8();
      blob->grow[i]  = (blob->r[i] < 1.f);
      if (blob->sX[i] == 0) blob->sX[i] = 1;
      if (blob->sY[i] == 0) blob->sY[i] = 1;
    }
  }

  SEGMENT.fadeToBlackBy((SEGMENT.custom2>>3)+1);

  // Bounce balls around
  for (size_t i = 0; i < Amount; i++) {
    if (SEGMENT.step < millis()) blob->color[i] = add8(blob->color[i], 4); // slowly change color
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
    if (blob->r[i] > 1.f) SEGMENT.fill_circle(blob->x[i], blob->y[i], roundf(blob->r[i]), c);
    else                  SEGMENT.setPixelColorXY_CRGB(blob->x[i], blob->y[i], c);
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
      blob->sX[i] = (float)random8(3, cols) / (256 - SEGMENT.speed);
      blob->x[i]  = 0.01f;
    } else if (blob->x[i] > (float)cols - 1.01f) {
      blob->sX[i] = (float)random8(3, cols) / (256 - SEGMENT.speed);
      blob->sX[i] = -blob->sX[i];
      blob->x[i]  = (float)cols - 1.01f;
    }
    // bounce y
    if (blob->y[i] < 0.01f) {
      blob->sY[i] = (float)random8(3, rows) / (256 - SEGMENT.speed);
      blob->y[i]  = 0.01f;
    } else if (blob->y[i] > (float)rows - 1.01f) {
      blob->sY[i] = (float)random8(3, rows) / (256 - SEGMENT.speed);
      blob->sY[i] = -blob->sY[i];
      blob->y[i]  = (float)rows - 1.01f;
    }
  }
  SEGMENT.blur(SEGMENT.custom1>>2);

  if (SEGMENT.step < millis()) SEGMENT.step = millis() + 2000; // change colors every 2 seconds

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
#undef MAX_BLOBS
static const char _data_FX_MODE_2DBLOBS[] PROGMEM = "Blobs@!,# blobs,Blur,Trail;!;!;2;c1=8";
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

////////////////////////////
//     2D Drift Rose      //
////////////////////////////
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
//// Drift Rose by stepko (c)2021 [https://editor.soulmatelights.com/gallery/1369-drift-rose-pattern], adapted by Blaz Kristan (AKA blazoncek)

void mAnimatorLight::EffectAnim__2D__DriftRose()
{
  
  // uint16_t mode_2Ddriftrose(void) {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  const float CX = (cols-cols%2)/2.f - .5f;
  const float CY = (rows-rows%2)/2.f - .5f;
  const float L = min(cols, rows) / 2.f;

  SEGMENT.fadeToBlackBy(32+(SEGMENT.speed>>3));
  for (size_t i = 1; i < 37; i++) {
    uint32_t x = (CX + (sin_t(radians(i * 10)) * (beatsin8(i, 0, L*2)-L))) * 255.f;
    uint32_t y = (CY + (cos_t(radians(i * 10)) * (beatsin8(i, 0, L*2)-L))) * 255.f;
    SEGMENT.wu_pixel(x, y, CHSV(i * 10, 255, 255));
  }
  SEGMENT.blur((SEGMENT.intensity>>4)+1);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char _data_FX_MODE_2DDRIFTROSE[] PROGMEM = "Drift Rose@Fade,Blur;;;2";

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/********************     audio enhanced routines     ************************/
///////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

/* use the following code to pass AudioReactive usermod variables to effect

  uint8_t  *binNum = (uint8_t*)&SEGMENT.params_internal.aux1, *maxVol = (uint8_t*)(&SEGMENT.params_internal.aux1+1); // just in case assignment
  bool      samplePeak = false;
  float     FFT_MajorPeak = 1.0;
  uint8_t  *fftResult = nullptr;
  float    *fftBin = nullptr;
  um_data_t *um_data;
  if (usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    volumeSmth    = *(float*)   um_data->u_data[0];
    volumeRaw     = *(float*)   um_data->u_data[1];
    fftResult     =  (uint8_t*) um_data->u_data[2];
    samplePeak    = *(uint8_t*) um_data->u_data[3];
    FFT_MajorPeak = *(float*)   um_data->u_data[4];
    my_magnitude  = *(float*)   um_data->u_data[5];
    maxVol        =  (uint8_t*) um_data->u_data[6];  // requires UI element (SEGMENT.customX?), changes source element
    binNum        =  (uint8_t*) um_data->u_data[7];  // requires UI element (SEGMENT.customX?), changes source element
    fftBin        =  (float*)   um_data->u_data[8];
  } else {
    // add support for no audio data
    um_data = simulateSound(SEGMENT.soundSim);
  }
*/


// a few constants needed for AudioReactive effects

// for 22Khz sampling
#define MAX_FREQUENCY   11025    // sample frequency / 2 (as per Nyquist criterion)
#define MAX_FREQ_LOG10  4.04238f // log10(MAX_FREQUENCY)

// for 20Khz sampling
//#define MAX_FREQUENCY   10240
//#define MAX_FREQ_LOG10  4.0103f

// for 10Khz sampling
//#define MAX_FREQUENCY   5120
//#define MAX_FREQ_LOG10  3.71f


/////////////////////////////////
//     * Ripple Peak           //
/////////////////////////////////
void mAnimatorLight::EffectAnim__AudioReactive__1D__Ripple_Peak()
{
  // uint16_t mode_ripplepeak(void) {                // * Ripple peak. By Andrew Tuline.
                                                          // This currently has no controls.
  #define maxsteps 16                                     // Case statement wouldn't allow a variable.

  uint16_t maxRipples = 16;
  uint16_t dataSize = sizeof(Ripple) * maxRipples;
  if (!SEGMENT.allocateData(dataSize)) return EffectAnim__Solid_Colour(); //allocation failed
  Ripple* ripples = reinterpret_cast<Ripple*>(SEGMENT.data);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  uint8_t samplePeak    = *(uint8_t*)um_data->u_data[3];
  #ifdef ESP32
  float   FFT_MajorPeak = *(float*)  um_data->u_data[4];
  #endif
  uint8_t *maxVol       =  (uint8_t*)um_data->u_data[6];
  uint8_t *binNum       =  (uint8_t*)um_data->u_data[7];

  // printUmData();

  if (SEGMENT.call == 0) {
    SEGMENT.params_internal.aux0 = 255;
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
        ripples[i].pos = random16(SEGLEN);
        #ifdef ESP32
          if (FFT_MajorPeak > 1)                          // log10(0) is "forbidden" (throws exception)
          ripples[i].color = (int)(log10f(FFT_MajorPeak)*128);
          else ripples[i].color = 0;
        #else
          ripples[i].color = random8();
        #endif
        ripples[i].state = 0;
        break;

      case 0:
        SEGMENT.setPixelColor(ripples[i].pos, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(ripples[i].color, false, PALETTE_SOLID_WRAP, 0), SEGMENT.params_internal.aux0));
        ripples[i].state++;
        break;

      case maxsteps:                                      // At the end of the ripples. 254 is an inactive mode.
        ripples[i].state = 254;
        break;

      default:                                            // Middle of the ripples.
        SEGMENT.setPixelColor((ripples[i].pos + ripples[i].state + SEGLEN) % SEGLEN, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(ripples[i].color, false, PALETTE_SOLID_WRAP, 0), SEGMENT.params_internal.aux0/ripples[i].state*2));
        SEGMENT.setPixelColor((ripples[i].pos - ripples[i].state + SEGLEN) % SEGLEN, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(ripples[i].color, false, PALETTE_SOLID_WRAP, 0), SEGMENT.params_internal.aux0/ripples[i].state*2));
        ripples[i].state++;                               // Next step.
        break;
    } // switch step
  } // for i

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_ripplepeak()
static const char _data_FX_MODE_RIPPLEPEAK[] PROGMEM = "Ripple Peak@Fade rate,Max # of ripples,Select bin,Volume (min);!,!;!;1v;c2=0,m12=0,si=0"; // Pixel, Beatsin

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : 2D Swirl
 * @description:   : By: Mark Kriegsman https://gist.github.com/kriegsman/5adca44e14ad025e6d3b , modified by Andrew Tuline
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
void mAnimatorLight::EffectAnim__AudioReactive__2D__Swirl()
{
  
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  const uint8_t borderWidth = 2;

  SEGMENT.blur(SEGMENT.custom1);

  uint8_t  i = beatsin8( 27*SEGMENT.speed/255, borderWidth, cols - borderWidth);
  uint8_t  j = beatsin8( 41*SEGMENT.speed/255, borderWidth, rows - borderWidth);
  uint8_t ni = (cols - 1) - i;
  uint8_t nj = (cols - 1) - j;
  uint16_t ms = millis();

  um_data_t *um_data;
  // if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  // }
  float   volumeSmth  = *(float*)   um_data->u_data[0]; //ewowi: use instead of sampleAvg???
  int16_t volumeRaw   = *(int16_t*) um_data->u_data[1];

  SEGMENT.addPixelColorXY( i, j, ColorFromPalette_WithLoad(SEGPALETTE, (ms / 11 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 11, 200, 255);
  SEGMENT.addPixelColorXY( j, i, ColorFromPalette_WithLoad(SEGPALETTE, (ms / 13 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 13, 200, 255);
  SEGMENT.addPixelColorXY(ni,nj, ColorFromPalette_WithLoad(SEGPALETTE, (ms / 17 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 17, 200, 255);
  SEGMENT.addPixelColorXY(nj,ni, ColorFromPalette_WithLoad(SEGPALETTE, (ms / 29 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 29, 200, 255);
  SEGMENT.addPixelColorXY( i,nj, ColorFromPalette_WithLoad(SEGPALETTE, (ms / 37 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 37, 200, 255);
  SEGMENT.addPixelColorXY(ni, j, ColorFromPalette_WithLoad(SEGPALETTE, (ms / 41 + volumeSmth*4), volumeRaw * SEGMENT.intensity / 64, LINEARBLEND)); //CHSV( ms / 41, 200, 255);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__SWIRL__INDEXING[] PROGMEM = "Swirl@!,Sensitivity,Blur;,Bg Swirl;!;2v;ix=64,si=0"; // Beatsin // TODO: color 1 unused?
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : 2D Waverly
 * @description:   : By: Stepko, https://editor.soulmatelights.com/gallery/652-wave , modified by Andrew Tuline
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
void mAnimatorLight::EffectAnim__AudioReactive__2D__Waverly()
{
  // uint16_t mode_2DWaverly(void) {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  um_data_t *um_data;
  // if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  // }
  float   volumeSmth  = *(float*)   um_data->u_data[0];

  SEGMENT.fadeToBlackBy(SEGMENT.speed);

  long t = millis() / 2;
  for (int i = 0; i < cols; i++) {
    uint16_t thisVal = (1 + SEGMENT.intensity/64) * inoise8(i * 45 , t , t)/2;
    // use audio if available
    if (um_data) {
      thisVal /= 32; // reduce intensity of inoise8()
      thisVal *= volumeSmth;
    }
    uint16_t thisMax = map(thisVal, 0, 512, 0, rows);

    for (int j = 0; j < thisMax; j++) {
      SEGMENT.addPixelColorXY(i, j, ColorFromPalette_WithLoad(SEGPALETTE, map(j, 0, thisMax, 250, 0), 255, LINEARBLEND));
      SEGMENT.addPixelColorXY((cols - 1) - i, (rows - 1) - j, ColorFromPalette_WithLoad(SEGPALETTE, map(j, 0, thisMax, 250, 0), 255, LINEARBLEND));
    }
  }
  SEGMENT.blur(16);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DWaverly()
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__WAVERLY__INDEXING[] PROGMEM = "Waverly@Amplification,Sensitivity;;!;2v;ix=64,si=0"; // Beatsin
#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D_TODO


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


// float version of map()
static float mapf(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Gravity struct requited for GRAV* effects
typedef struct Gravity {
  int    topLED;
  int    gravityCounter;
} gravity;

///////////////////////
//   * GRAVCENTER    //
///////////////////////

void mAnimatorLight::EffectAnim__AudioReactive__1D__GravCenter()
{
  // uint16_t mode_gravcenter(void) {                // Gravcenter. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();

  const uint16_t dataSize = sizeof(gravity);
  if (!SEGMENT.allocateData(dataSize)) return EffectAnim__Solid_Colour(); //allocation failed
  Gravity* gravcen = reinterpret_cast<Gravity*>(SEGMENT.data);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth  = *(float*)  um_data->u_data[0];

  //SEGMENT.fade_out(240);
  SEGMENT.fade_out(251);  // 30%

  float segmentSampleAvg = volumeSmth * (float)SEGMENT.intensity / 255.0f;
  segmentSampleAvg *= 0.125; // divide by 8, to compensate for later "sensitivity" upscaling

  float mySampleAvg = mapf(segmentSampleAvg*2.0, 0, 32, 0, (float)SEGLEN/2.0f); // map to pixels available in current segment
  uint16_t tempsamp = constrain(mySampleAvg, 0, SEGLEN/2);     // Keep the sample from overflowing.
  uint8_t gravity = 8 - SEGMENT.speed/32;

  for (int i=0; i<tempsamp; i++) {
    uint8_t index = inoise8(i*segmentSampleAvg+millis(), 5000+i*segmentSampleAvg);
    SEGMENT.setPixelColor(i+SEGLEN/2, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0), segmentSampleAvg*8));
    SEGMENT.setPixelColor(SEGLEN/2-i-1, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0), segmentSampleAvg*8));
  }

  if (tempsamp >= gravcen->topLED)
    gravcen->topLED = tempsamp-1;
  else if (gravcen->gravityCounter % gravity == 0)
    gravcen->topLED--;

  if (gravcen->topLED >= 0) {
    SEGMENT.setPixelColor(gravcen->topLED+SEGLEN/2, SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0));
    SEGMENT.setPixelColor(SEGLEN/2-1-gravcen->topLED, SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0));
  }
  gravcen->gravityCounter = (gravcen->gravityCounter + 1) % gravity;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_gravcenter()
static const char _data_FX_MODE_GRAVCENTER[] PROGMEM = "Gravcenter@Rate of fall,Sensitivity;!,!;!;1v;ix=128,m12=2,si=0"; // Circle, Beatsin

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D_TODO


///////////////////////
//   * GRAVCENTRIC   //
///////////////////////
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

void mAnimatorLight::EffectAnim__AudioReactive__1D__GravCentric()
{
  
  // uint16_t mode_gravcentric(void) {                     // Gravcentric. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();

  uint16_t dataSize = sizeof(gravity);
  if (!SEGMENT.allocateData(dataSize)) return EffectAnim__Solid_Colour();     //allocation failed
  Gravity* gravcen = reinterpret_cast<Gravity*>(SEGMENT.data);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth  = *(float*)  um_data->u_data[0];

  // printUmData();

  //SEGMENT.fade_out(240);
  //SEGMENT.fade_out(240); // twice? really?
  SEGMENT.fade_out(253);  // 50%

  float segmentSampleAvg = volumeSmth * (float)SEGMENT.intensity / 255.0f;
  segmentSampleAvg *= 0.125f; // divide by 8, to compensate for later "sensitivity" upscaling

  float mySampleAvg = mapf(segmentSampleAvg*2.0, 0.0f, 32.0f, 0.0f, (float)SEGLEN/2.0f); // map to pixels availeable in current segment
  int tempsamp = constrain(mySampleAvg, 0, SEGLEN/2);     // Keep the sample from overflowing.
  uint8_t gravity = 8 - SEGMENT.speed/32;

  for (int i=0; i<tempsamp; i++) {
    uint8_t index = segmentSampleAvg*24+millis()/200;
    SEGMENT.setPixelColor(i+SEGLEN/2, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
    SEGMENT.setPixelColor(SEGLEN/2-1-i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  if (tempsamp >= gravcen->topLED)
    gravcen->topLED = tempsamp-1;
  else if (gravcen->gravityCounter % gravity == 0)
    gravcen->topLED--;

  if (gravcen->topLED >= 0) {
    SEGMENT.setPixelColor(gravcen->topLED+SEGLEN/2, CRGB::Gray);
    SEGMENT.setPixelColor(SEGLEN/2-1-gravcen->topLED, CRGB::Gray);
  }
  gravcen->gravityCounter = (gravcen->gravityCounter + 1) % gravity;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_gravcentric()
static const char _data_FX_MODE_GRAVCENTRIC[] PROGMEM = "Gravcentric@Rate of fall,Sensitivity;!,!;!;1v;ix=128,m12=3,si=0"; // Corner, Beatsin


#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO


///////////////////////
//   * GRAVIMETER    //
///////////////////////
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

void mAnimatorLight::EffectAnim__AudioReactive__1D__GraviMeter()
{
  
// uint16_t mode_gravimeter(void) {                // Gravmeter. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();

  uint16_t dataSize = sizeof(gravity);
  if (!SEGMENT.allocateData(dataSize)) return EffectAnim__Solid_Colour(); //allocation failed
  Gravity* gravcen = reinterpret_cast<Gravity*>(SEGMENT.data);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth  = *(float*)  um_data->u_data[0];

  //SEGMENT.fade_out(240);
  SEGMENT.fade_out(249);  // 25%

  float segmentSampleAvg = volumeSmth * (float)SEGMENT.intensity / 255.0;
  segmentSampleAvg *= 0.25; // divide by 4, to compensate for later "sensitivity" upscaling

  float mySampleAvg = mapf(segmentSampleAvg*2.0, 0, 64, 0, (SEGLEN-1)); // map to pixels availeable in current segment
  int tempsamp = constrain(mySampleAvg,0,SEGLEN-1);       // Keep the sample from overflowing.
  uint8_t gravity = 8 - SEGMENT.speed/32;

  for (int i=0; i<tempsamp; i++) {
    uint8_t index = inoise8(i*segmentSampleAvg+millis(), 5000+i*segmentSampleAvg);
    SEGMENT.setPixelColor(i, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0), segmentSampleAvg*8));
  }

  if (tempsamp >= gravcen->topLED)
    gravcen->topLED = tempsamp;
  else if (gravcen->gravityCounter % gravity == 0)
    gravcen->topLED--;

  if (gravcen->topLED > 0) {
    SEGMENT.setPixelColor(gravcen->topLED, SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0));
  }
  gravcen->gravityCounter = (gravcen->gravityCounter + 1) % gravity;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_gravimeter()
static const char _data_FX_MODE_GRAVIMETER[] PROGMEM = "Gravimeter@Rate of fall,Sensitivity;!,!;!;1v;ix=128,m12=2,si=0"; // Circle, Beatsin


#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   * JUGGLES      //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

void mAnimatorLight::EffectAnim__AudioReactive__1D__Juggles()
{
  
// uint16_t mode_juggles(void) {                   // Juggles. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  SEGMENT.fade_out(224); // 6.25%
  uint16_t my_sampleAgc = fmax(fmin(volumeSmth, 255.0), 0);

  for (size_t i=0; i<SEGMENT.intensity/32+1U; i++) {
    SEGMENT.setPixelColor(beatsin16(SEGMENT.speed/4+i*2,0,SEGLEN-1), Segment::color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(millis()/4+i*2, false, PALETTE_SOLID_WRAP, 0), my_sampleAgc));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_juggles()
static const char _data_FX_MODE_JUGGLES[] PROGMEM = "Juggles@!,# of balls;!,!;!;1v;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   * MATRIPIX     //
//////////////////////

#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__Matripix()
{
  
// uint16_t mode_matripix(void) {                  // Matripix. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  int16_t volumeRaw    = *(int16_t*)um_data->u_data[1];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500 % 16;
  if(SEGMENT.params_internal.aux0 != secondHand) {
    SEGMENT.params_internal.aux0 = secondHand;

    int pixBri = volumeRaw * SEGMENT.intensity / 64;
    for (int i = 0; i < SEGLEN-1; i++) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // shift left
    SEGMENT.setPixelColor(SEGLEN-1, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0), pixBri));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_matripix()
static const char _data_FX_MODE_MATRIPIX[] PROGMEM = "Matripix@!,Brightness;!,!;!;1v;ix=64,m12=2,si=1"; //,rev=1,mi=1,rY=1,mY=1 Circle, WeWillRockYou, reverseX

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   * MIDNOISE     //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__MidNoise()
{
  
// uint16_t mode_midnoise(void) {                  // Midnoise. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
// Changing xdist to SEGMENT.params_internal.aux0 and ydist to SEGMENT.params_internal.aux1.

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  SEGMENT.fade_out(SEGMENT.speed);
  SEGMENT.fade_out(SEGMENT.speed);

  float tmpSound2 = volumeSmth * (float)SEGMENT.intensity / 256.0;  // Too sensitive.
  tmpSound2 *= (float)SEGMENT.intensity / 128.0;              // Reduce sensitivity/length.

  int maxLen = mapf(tmpSound2, 0, 127, 0, SEGLEN/2);
  if (maxLen >SEGLEN/2) maxLen = SEGLEN/2;

  for (int i=(SEGLEN/2-maxLen); i<(SEGLEN/2+maxLen); i++) {
    uint8_t index = inoise8(i*volumeSmth+SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1+i*volumeSmth);  // Get a value from the noise function. I'm using both x and y axis.
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.params_internal.aux0=SEGMENT.params_internal.aux0+beatsin8(5,0,10);
  SEGMENT.params_internal.aux1=SEGMENT.params_internal.aux1+beatsin8(4,0,10);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_midnoise()
static const char _data_FX_MODE_MIDNOISE[] PROGMEM = "Midnoise@Fade rate,Max. length;!,!;!;1v;ix=128,m12=1,si=0"; // Bar, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   * NOISEFIRE    //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__NoiseFire()
{
  
// I am the god of hellfire. . . Volume (only) reactive fire routine. Oh, look how short this is.
// uint16_t mode_noisefire(void) {                 // Noisefire. By Andrew Tuline.
  CRGBPalette16 myPal = CRGBPalette16(CHSV(0,255,2),    CHSV(0,255,4),    CHSV(0,255,8), CHSV(0, 255, 8),  // Fire palette definition. Lower value = darker.
                                      CHSV(0, 255, 16), CRGB::Red,        CRGB::Red,     CRGB::Red,
                                      CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Orange,  CRGB::Orange,
                                      CRGB::Yellow,     CRGB::Orange,     CRGB::Yellow,  CRGB::Yellow);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  if (SEGMENT.call == 0) SEGMENT.fill(BLACK);

  for (int i = 0; i < SEGLEN; i++) {
    uint16_t index = inoise8(i*SEGMENT.speed/64,millis()*SEGMENT.speed/64*SEGLEN/255);  // X location is constant, but we move along the Y at the rate of millis(). By Andrew Tuline.
    index = (255 - i*256/SEGLEN) * index/(256-SEGMENT.intensity);                       // Now we need to scale index so that it gets blacker as we get close to one of the ends.
                                                                                        // This is a simple y=mx+b equation that's been scaled. index/128 is another scaling.

    CRGB color = ColorFromPalette_WithLoad(myPal, index, volumeSmth*2, LINEARBLEND);     // Use the my own palette.
    SEGMENT.setPixelColor(i, color);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_noisefire()
static const char _data_FX_MODE_NOISEFIRE[] PROGMEM = "Noisefire@!,!;;;1v;m12=2,si=0"; // Circle, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////
//   * Noisemeter    //
///////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__NoiseMeter()
{
  
// uint16_t mode_noisemeter(void) {                // Noisemeter. By Andrew Tuline.

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth   = *(float*)  um_data->u_data[0];
  int16_t volumeRaw    = *(int16_t*)um_data->u_data[1];

  //uint8_t fadeRate = map(SEGMENT.speed,0,255,224,255);
  uint8_t fadeRate = map(SEGMENT.speed,0,255,200,254);
  SEGMENT.fade_out(fadeRate);

  float tmpSound2 = volumeRaw * 2.0 * (float)SEGMENT.intensity / 255.0;
  int maxLen = mapf(tmpSound2, 0, 255, 0, SEGLEN); // map to pixels availeable in current segment              // Still a bit too sensitive.
  if (maxLen <0) maxLen = 0;
  if (maxLen >SEGLEN) maxLen = SEGLEN;

  for (int i=0; i<maxLen; i++) {                                    // The louder the sound, the wider the soundbar. By Andrew Tuline.
    uint8_t index = inoise8(i*volumeSmth+SEGMENT.params_internal.aux0, SEGMENT.params_internal.aux1+i*volumeSmth);  // Get a value from the noise function. I'm using both x and y axis.
    SEGMENT.setPixelColor(i, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.params_internal.aux0+=beatsin8(5,0,10);
  SEGMENT.params_internal.aux1+=beatsin8(4,0,10);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_noisemeter()
static const char _data_FX_MODE_NOISEMETER[] PROGMEM = "Noisemeter@Fade rate,Width;!,!;!;1v;ix=128,m12=2,si=0"; // Circle, Beatsin


#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
//////////////////////
//   * PIXELWAVE    //
//////////////////////
void mAnimatorLight::EffectAnim__AudioReactive__1D__PixelWave()
{
 
//  uint16_t mode_pixelwave(void) {                 // Pixelwave. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  int16_t volumeRaw    = *(int16_t*)um_data->u_data[1];

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500+1 % 16;
  if (SEGMENT.params_internal.aux0 != secondHand) {
    SEGMENT.params_internal.aux0 = secondHand;

    int pixBri = volumeRaw * SEGMENT.intensity / 64;

    SEGMENT.setPixelColor(SEGLEN/2, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0), pixBri));
    for (int i = SEGLEN - 1; i > SEGLEN/2; i--)   SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); //move to the left
    for (int i = 0; i < SEGLEN/2; i++)            SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // move to the right
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_pixelwave()
static const char _data_FX_MODE_PIXELWAVE[] PROGMEM = "Pixelwave@!,Sensitivity;!,!;!;1v;ix=64,m12=2,si=0"; // Circle, Beatsin


#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
//////////////////////
//   * PLASMOID     //
//////////////////////
typedef struct Plasphase {
  int16_t    thisphase;
  int16_t    thatphase;
} plasphase;

void mAnimatorLight::EffectAnim__AudioReactive__1D__Plasmoid()
{
 
// uint16_t mode_plasmoid(void) {                  // Plasmoid. By Andrew Tuline.
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment
  if (!SEGMENT.allocateData(sizeof(plasphase))) return EffectAnim__Solid_Colour(); //allocation failed
  Plasphase* plasmoip = reinterpret_cast<Plasphase*>(SEGMENT.data);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  SEGMENT.fadeToBlackBy(32);

  plasmoip->thisphase += beatsin8(6,-4,4);                          // You can change direction and speed individually.
  plasmoip->thatphase += beatsin8(7,-4,4);                          // Two phase values to make a complex pattern. By Andrew Tuline.

  for (int i = 0; i < SEGLEN; i++) {                          // For each of the LED's in the strand, set a brightness based on a wave as follows.
    // updated, similar to "plasma" effect - softhack007
    uint8_t thisbright = cubicwave8(((i*(1 + (3*SEGMENT.speed/32)))+plasmoip->thisphase) & 0xFF)/2;
    thisbright += cos8(((i*(97 +(5*SEGMENT.speed/32)))+plasmoip->thatphase) & 0xFF)/2; // Let's munge the brightness a bit and animate it all with the phases.

    uint8_t colorIndex=thisbright;
    if (volumeSmth * SEGMENT.intensity / 64 < thisbright) {thisbright = 0;}

    SEGMENT.addPixelColor(i, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(colorIndex, false, PALETTE_SOLID_WRAP, 0), thisbright));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_plasmoid()
static const char _data_FX_MODE_PLASMOID[] PROGMEM = "Plasmoid@Phase,# of pixels;!,!;!;1v;sx=128,ix=128,m12=0,si=0"; // Pixels, Beatsin


#endif //   ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////
//   * PUDDLEPEAK    //
///////////////////////
// Andrew's crappy peak detector. If I were 40+ years younger, I'd learn signal processing.

#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__PuddlePeak()
{
 
// uint16_t mode_puddlepeak(void) {                // Puddlepeak. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();

  uint16_t size = 0;
  uint8_t fadeVal = map(SEGMENT.speed,0,255, 224, 254);
  uint16_t pos = random(SEGLEN);                          // Set a random starting position.

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  uint8_t samplePeak = *(uint8_t*)um_data->u_data[3];
  uint8_t *maxVol    =  (uint8_t*)um_data->u_data[6];
  uint8_t *binNum    =  (uint8_t*)um_data->u_data[7];
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  if (SEGMENT.call == 0) {
    SEGMENT.custom1 = *binNum;
    SEGMENT.custom2 = *maxVol * 2;
  }

  *binNum = SEGMENT.custom1;                              // Select a bin.
  *maxVol = SEGMENT.custom2 / 2;                          // Our volume comparator.

  SEGMENT.fade_out(fadeVal);

  if (samplePeak == 1) {
    size = volumeSmth * SEGMENT.intensity /256 /4 + 1;    // Determine size of the flash based on the volume.
    if (pos+size>= SEGLEN) size = SEGLEN - pos;
  }

  for (int i=0; i<size; i++) {                            // Flash the LED's.
    SEGMENT.setPixelColor(pos+i, SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_puddlepeak()
static const char _data_FX_MODE_PUDDLEPEAK[] PROGMEM = "Puddlepeak@Fade rate,Puddle size,Select bin,Volume (min);!,!;!;1v;c2=0,m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   * PUDDLES      //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__Puddles()
{
 
// uint16_t mode_puddles(void) {                   // Puddles. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  uint16_t size = 0;
  uint8_t fadeVal = map(SEGMENT.speed, 0, 255, 224, 254);
  uint16_t pos = random16(SEGLEN);                        // Set a random starting position.

  SEGMENT.fade_out(fadeVal);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  int16_t volumeRaw    = *(int16_t*)um_data->u_data[1];

  if (volumeRaw > 1) {
    size = volumeRaw * SEGMENT.intensity /256 /8 + 1;        // Determine size of the flash based on the volume.
    if (pos+size >= SEGLEN) size = SEGLEN - pos;
  }

  for (int i=0; i<size; i++) {                          // Flash the LED's.
    SEGMENT.setPixelColor(pos+i, SEGMENT.color_from_palette(millis(), false, PALETTE_SOLID_WRAP, 0));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_puddles()
static const char _data_FX_MODE_PUDDLES[] PROGMEM = "Puddles@Fade rate,Puddle size;!,!;!;1v;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//     * PIXELS     //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__Pixels()
{
 
// uint16_t mode_pixels(void) {                    // Pixels. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();

  if (!SEGMENT.allocateData(32*sizeof(uint8_t))) return EffectAnim__Solid_Colour(); //allocation failed
  uint8_t *myVals = reinterpret_cast<uint8_t*>(SEGMENT.data); // Used to store a pile of samples because WLED frame rate and WLED sample rate are not synchronized. Frame rate is too low.

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   volumeSmth   = *(float*)  um_data->u_data[0];

  myVals[millis()%32] = volumeSmth;    // filling values semi randomly

  SEGMENT.fade_out(64+(SEGMENT.speed>>1));

  for (int i=0; i <SEGMENT.intensity/8; i++) {
    uint16_t segLoc = random16(SEGLEN);                    // 16 bit for larger strands of LED's.
    SEGMENT.setPixelColor(segLoc, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(myVals[i%32]+i*4, false, PALETTE_SOLID_WRAP, 0), volumeSmth));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_pixels()
static const char _data_FX_MODE_PIXELS[] PROGMEM = "Pixels@Fade rate,# of pixels;!,!;!;1v;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

///////////////////////////////
//     BEGIN FFT ROUTINES    //
///////////////////////////////

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//    ** Blurz      //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Blurz()
{
 
 
//  uint16_t mode_blurz(void) {                    // Blurz. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  // even with 1D effect we have to take logic for 2D segments for allocation as fill_solid() fills whole segment

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.params_internal.aux0 = 0;
  }

  int fadeoutDelay = (256 - SEGMENT.speed) / 32;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fade_out(SEGMENT.speed);

  SEGMENT.step += FRAMETIME;
  if (SEGMENT.step > SPEED_FORMULA_L) {
    uint16_t segLoc = random16(SEGLEN);
    SEGMENT.setPixelColor(segLoc, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(2*fftResult[SEGMENT.params_internal.aux0%16]*240/max(1, SEGLEN-1), false, PALETTE_SOLID_WRAP, 0), 2*fftResult[SEGMENT.params_internal.aux0%16]));
    ++(SEGMENT.params_internal.aux0) %= 16; // make sure it doesn't cross 16

    SEGMENT.step = 1;
    SEGMENT.blur(SEGMENT.intensity);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_blurz()
static const char _data_FX_MODE_BLURZ[] PROGMEM = "Blurz@Fade rate,Blur;!,Color mix;!;1f;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

/////////////////////////
//   ** DJLight        //
/////////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_DJLight()
{
 
//  uint16_t mode_DJLight(void) {                   // Written by ??? Adapted by Will Tatam.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  const int mid = SEGLEN / 2;

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500+1 % 64;
  if (SEGMENT.params_internal.aux0 != secondHand) {                        // Triggered millis timing.
    SEGMENT.params_internal.aux0 = secondHand;

    CRGB color = CRGB(fftResult[15]/2, fftResult[5]/2, fftResult[0]/2); // 16-> 15 as 16 is out of bounds
    SEGMENT.setPixelColor(mid, color.fadeToBlackBy(map(fftResult[4], 0, 255, 255, 4)));     // TODO - Update

    for (int i = SEGLEN - 1; i > mid; i--)   SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); // move to the left
    for (int i = 0; i < mid; i++)            SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // move to the right
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_DJLight()
static const char _data_FX_MODE_DJLIGHT[] PROGMEM = "DJ Light@Speed;;;1f;m12=2,si=0"; // Circle, Beatsin
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

////////////////////
//   ** Freqmap   //
////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqMap()
{
 
 
// uint16_t mode_freqmap(void) {                   // Map FFT_MajorPeak to SEGLEN. Would be better if a higher framerate.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  // Start frequency = 60 Hz and log10(60) = 1.78
  // End frequency = MAX_FREQUENCY in Hz and lo10(MAX_FREQUENCY) = MAX_FREQ_LOG10

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float my_magnitude  = *(float*)um_data->u_data[5] / 4.0f;
  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1;                                         // log10(0) is "forbidden" (throws exception)

  if (SEGMENT.call == 0) SEGMENT.fill(BLACK);
  int fadeoutDelay = (256 - SEGMENT.speed) / 32;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fade_out(SEGMENT.speed);

  int locn = (log10f((float)FFT_MajorPeak) - 1.78f) * (float)SEGLEN/(MAX_FREQ_LOG10 - 1.78f);  // log10 frequency range is from 1.78 to 3.71. Let's scale to SEGLEN.
  if (locn < 1) locn = 0; // avoid underflow

  if (locn >=SEGLEN) locn = SEGLEN-1;
  uint16_t pixCol = (log10f(FFT_MajorPeak) - 1.78f) * 255.0f/(MAX_FREQ_LOG10 - 1.78f);   // Scale log10 of frequency values to the 255 colour index.
  if (FFT_MajorPeak < 61.0f) pixCol = 0;                                                 // handle underflow

  uint16_t bright = (int)my_magnitude;

  SEGMENT.setPixelColor(locn, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(SEGMENT.intensity+pixCol, false, PALETTE_SOLID_WRAP, 0), bright));

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_freqmap()
static const char _data_FX_MODE_FREQMAP[] PROGMEM = "Freqmap@Fade rate,Starting color;!,!;!;1f;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////
//   ** Freqmatrix   //
///////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqMatrix()
{
 
// uint16_t mode_freqmatrix(void) {                // Freqmatrix. By Andreas Pleschung.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float volumeSmth    = *(float*)um_data->u_data[0];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500 % 16;
  if(SEGMENT.params_internal.aux0 != secondHand) {
    SEGMENT.params_internal.aux0 = secondHand;

    uint8_t sensitivity = map(SEGMENT.custom3, 0, 31, 1, 10); // reduced resolution slider
    int pixVal = (volumeSmth * SEGMENT.intensity * sensitivity) / 256.0f;
    if (pixVal > 255) pixVal = 255;

    float intensity = map(pixVal, 0, 255, 0, 100) / 100.0f;  // make a brightness from the last avg

    CRGB color = Black;

    if (FFT_MajorPeak > MAX_FREQUENCY) FFT_MajorPeak = 1;
    // MajorPeak holds the freq. value which is most abundant in the last sample.
    // With our sampling rate of 10240Hz we have a usable freq range from roughly 80Hz to 10240/2 Hz
    // we will treat everything with less than 65Hz as 0

    if (FFT_MajorPeak < 80) {
      color = Black;
    } else {
      int upperLimit = 80 + 42 * SEGMENT.custom2;
      int lowerLimit = 80 + 3 * SEGMENT.custom1;
      uint8_t i =  lowerLimit!=upperLimit ? map(FFT_MajorPeak, lowerLimit, upperLimit, 0, 255) : FFT_MajorPeak;  // may under/overflow - so we enforce uint8_t
      uint16_t b = 255 * intensity;
      if (b > 255) b = 255;
      color = CHSV(i, 240, (uint8_t)b); // implicit conversion to RGB supplied by FastLED
    }

    // shift the pixels one pixel up
    SEGMENT.setPixelColor(0, color);
    for (int i = SEGLEN - 1; i > 0; i--) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); //move to the left
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_freqmatrix()
static const char _data_FX_MODE_FREQMATRIX[] PROGMEM = "Freqmatrix@Speed,Sound effect,Low bin,High bin,Sensitivity;;;1f;m12=3,si=0"; // Corner, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   ** Freqpixels  //
//////////////////////
// Start frequency = 60 Hz and log10(60) = 1.78
// End frequency = 5120 Hz and lo10(5120) = 3.71
//  SEGMENT.speed select faderate
//  SEGMENT.intensity select colour index
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqPixels()
{
  
  // uint16_t mode_freqpixels(void) {                // Freqpixel. By Andrew Tuline.
  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float my_magnitude  = *(float*)um_data->u_data[5] / 16.0f;
  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1.0f; // log10(0) is "forbidden" (throws exception)

  // this code translates to speed * (2 - speed/255) which is a) speed*2 or b) speed (when speed is 255)
  // and since fade_out() can only take 0-255 it will behave incorrectly when speed > 127
  //uint16_t fadeRate = 2*SEGMENT.speed - SEGMENT.speed*SEGMENT.speed/255;    // Get to 255 as quick as you can.
  uint16_t fadeRate = SEGMENT.speed*SEGMENT.speed; // Get to 255 as quick as you can.
  fadeRate = map(fadeRate, 0, 65535, 1, 255);

  int fadeoutDelay = (256 - SEGMENT.speed) / 64;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fade_out(fadeRate);

  uint8_t pixCol = (log10f(FFT_MajorPeak) - 1.78f) * 255.0f/(MAX_FREQ_LOG10 - 1.78f);  // Scale log10 of frequency values to the 255 colour index.
  if (FFT_MajorPeak < 61.0f) pixCol = 0;                                               // handle underflow
  for (int i=0; i < SEGMENT.intensity/32+1; i++) {
    uint16_t locn = random16(0,SEGLEN);
    SEGMENT.setPixelColor(locn, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(SEGMENT.intensity+pixCol, false, PALETTE_SOLID_WRAP, 0), (int)my_magnitude));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_freqpixels()
static const char _data_FX_MODE_FREQPIXELS[] PROGMEM = "Freqpixels@Fade rate,Starting color and # of pixels;!,!,;!;1f;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   ** Freqwave    //
//////////////////////
// Assign a color to the central (starting pixels) based on the predominant frequencies and the volume. The color is being determined by mapping the MajorPeak from the FFT
// and then mapping this to the HSV color circle. Currently we are sampling at 10240 Hz, so the highest frequency we can look at is 5120Hz.
//
// SEGMENT.custom1: the lower cut off point for the FFT. (many, most time the lowest values have very little information since they are FFT conversion artifacts. Suggested value is close to but above 0
// SEGMENT.custom2: The high cut off point. This depends on your sound profile. Most music looks good when this slider is between 50% and 100%.
// SEGMENT.custom3: "preamp" for the audio signal for audio10.
//
// I suggest that for this effect you turn the brightness to 95%-100% but again it depends on your soundprofile you find yourself in.
// Instead of using colorpalettes, This effect works on the HSV color circle with red being the lowest frequency
//
// As a compromise between speed and accuracy we are currently sampling with 10240Hz, from which we can then determine with a 512bin FFT our max frequency is 5120Hz.
// Depending on the music stream you have you might find it useful to change the frequency mapping.
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_FreqWave()
{
// uint16_t mode_freqwave(void) {                  // Freqwave. By Andreas Pleschung.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float FFT_MajorPeak = *(float*)um_data->u_data[4];
  float volumeSmth    = *(float*)um_data->u_data[0];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500 % 16;
  if(SEGMENT.params_internal.aux0 != secondHand) {
    SEGMENT.params_internal.aux0 = secondHand;

    float sensitivity = mapf(SEGMENT.custom3, 1, 31, 1, 10); // reduced resolution slider
    float pixVal = volumeSmth * (float)SEGMENT.intensity / 256.0f * sensitivity;
    if (pixVal > 255) pixVal = 255;

    float intensity = mapf(pixVal, 0, 255, 0, 100) / 100.0f;  // make a brightness from the last avg

    CRGB color = 0;

    if (FFT_MajorPeak > MAX_FREQUENCY) FFT_MajorPeak = 1.0f;
    // MajorPeak holds the freq. value which is most abundant in the last sample.
    // With our sampling rate of 10240Hz we have a usable freq range from roughly 80Hz to 10240/2 Hz
    // we will treat everything with less than 65Hz as 0

    if (FFT_MajorPeak < 80) {
      color = Black;
    } else {
      int upperLimit = 80 + 42 * SEGMENT.custom2;
      int lowerLimit = 80 + 3 * SEGMENT.custom1;
      uint8_t i =  lowerLimit!=upperLimit ? map(FFT_MajorPeak, lowerLimit, upperLimit, 0, 255) : FFT_MajorPeak; // may under/overflow - so we enforce uint8_t
      uint16_t b = 255.0 * intensity;
      if (b > 255) b=255;
      color = CHSV(i, 240, (uint8_t)b); // implicit conversion to RGB supplied by FastLED
    }

    SEGMENT.setPixelColor(SEGLEN/2, color);

    // shift the pixels one pixel outwards
    for (int i = SEGLEN - 1; i > SEGLEN/2; i--)   SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i-1)); //move to the left
    for (int i = 0; i < SEGLEN/2; i++)            SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // move to the right
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_freqwave()
static const char _data_FX_MODE_FREQWAVE[] PROGMEM = "Freqwave@Speed,Sound effect,Low bin,High bin,Pre-amp;;;1f;m12=2,si=0"; // Circle, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

///////////////////////
//    ** Gravfreq    //
///////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_GravFreq()
{
// uint16_t mode_gravfreq(void) {                  // Gravfreq. By Andrew Tuline.
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  uint16_t dataSize = sizeof(gravity);
  if (!SEGMENT.allocateData(dataSize)) return EffectAnim__Solid_Colour(); //allocation failed
  Gravity* gravcen = reinterpret_cast<Gravity*>(SEGMENT.data);

  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  float   FFT_MajorPeak = *(float*)um_data->u_data[4];
  float   volumeSmth    = *(float*)um_data->u_data[0];
  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1;                                         // log10(0) is "forbidden" (throws exception)

  SEGMENT.fade_out(250);

  float segmentSampleAvg = volumeSmth * (float)SEGMENT.intensity / 255.0f;
  segmentSampleAvg *= 0.125f; // divide by 8,  to compensate for later "sensitivity" upscaling

  float mySampleAvg = mapf(segmentSampleAvg*2.0f, 0,32, 0, (float)SEGLEN/2.0f); // map to pixels availeable in current segment
  int tempsamp = constrain(mySampleAvg,0,SEGLEN/2);     // Keep the sample from overflowing.
  uint8_t gravity = 8 - SEGMENT.speed/32;

  for (int i=0; i<tempsamp; i++) {

    //uint8_t index = (log10((int)FFT_MajorPeak) - (3.71-1.78)) * 255; //int? shouldn't it be floor() or similar
    uint8_t index = (log10f(FFT_MajorPeak) - (MAX_FREQ_LOG10 - 1.78f)) * 255; //int? shouldn't it be floor() or similar

    SEGMENT.setPixelColor(i+SEGLEN/2, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
    SEGMENT.setPixelColor(SEGLEN/2-i-1, SEGMENT.color_from_palette(index, false, PALETTE_SOLID_WRAP, 0));
  }

  if (tempsamp >= gravcen->topLED)
    gravcen->topLED = tempsamp-1;
  else if (gravcen->gravityCounter % gravity == 0)
    gravcen->topLED--;

  if (gravcen->topLED >= 0) {
    SEGMENT.setPixelColor(gravcen->topLED+SEGLEN/2, CRGB::Gray);
    SEGMENT.setPixelColor(SEGLEN/2-1-gravcen->topLED, CRGB::Gray);
  }
  gravcen->gravityCounter = (gravcen->gravityCounter + 1) % gravity;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_gravfreq()
static const char _data_FX_MODE_GRAVFREQ[] PROGMEM = "Gravfreq@Rate of fall,Sensitivity;!,!;!;1f;ix=128,m12=0,si=0"; // Pixels, Beatsin
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   ** Noisemove   //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_NoiseMove()
{
// uint16_t mode_noisemove(void) {                 // Noisemove.    By: Andrew Tuline
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  int fadeoutDelay = (256 - SEGMENT.speed) / 96;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fadeToBlackBy(4+ SEGMENT.speed/4);

  uint8_t numBins = map(SEGMENT.intensity,0,255,0,16);    // Map slider to fftResult bins.
  for (int i=0; i<numBins; i++) {                         // How many active bins are we using.
    uint16_t locn = inoise16(millis()*SEGMENT.speed+i*50000, millis()*SEGMENT.speed);   // Get a new pixel location from moving noise.
    locn = map(locn, 7500, 58000, 0, SEGLEN-1);           // Map that to the length of the strand, and ensure we don't go over.
    SEGMENT.setPixelColor(locn, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(i*64, false, PALETTE_SOLID_WRAP, 0), fftResult[i % 16]*4));
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_noisemove()
static const char _data_FX_MODE_NOISEMOVE[] PROGMEM = "Noisemove@Speed of perlin movement,Fade rate;!,!;!;1f;m12=0,si=0"; // Pixels, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

//////////////////////
//   ** Rocktaves   //
//////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_RockTaves()
{
// uint16_t mode_rocktaves(void) {                 // Rocktaves. Same note from each octave is same colour.    By: Andrew Tuline
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();
  um_data_t *um_data;
  if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  }
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

  uint16_t i = map(beatsin8(8+octCount*4, 0, 255, 0, octCount*8), 0, 255, 0, SEGLEN-1);
  i = constrain(i, 0, SEGLEN-1);
  SEGMENT.addPixelColor(i, color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette((uint8_t)frTemp, false, PALETTE_SOLID_WRAP, 0), volTemp));

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_rocktaves()
static const char _data_FX_MODE_ROCKTAVES[] PROGMEM = "Rocktaves@;!,!;!;1f;m12=1,si=0"; // Bar, Beatsin

#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waterfall
 * @description:   : Combines peak detection with FFT_MajorPeak and FFT_Magnitude.
 *                   By: Andrew Tuline
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
void mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Waterfall()
{
  
  if (SEGLEN == 1) return EffectAnim__Solid_Colour();

  um_data_t *um_data;
  // if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  // }
  uint8_t samplePeak    = *(uint8_t*)um_data->u_data[3];
  float   FFT_MajorPeak = *(float*)  um_data->u_data[4];
  uint8_t *maxVol       =  (uint8_t*)um_data->u_data[6];
  uint8_t *binNum       =  (uint8_t*)um_data->u_data[7];
  float   my_magnitude  = *(float*)   um_data->u_data[5] / 8.0f;

  if (FFT_MajorPeak < 1) FFT_MajorPeak = 1;                                         // log10(0) is "forbidden" (throws exception)

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
    SEGMENT.params_internal.aux0 = 255;
    SEGMENT.custom1 = *binNum;
    SEGMENT.custom2 = *maxVol * 2;
  }

  *binNum = SEGMENT.custom1;                              // Select a bin.
  *maxVol = SEGMENT.custom2 / 2;                          // Our volume comparator.

  uint8_t secondHand = micros() / (256-SEGMENT.speed)/500 + 1 % 16;
  if (SEGMENT.params_internal.aux0 != secondHand) {                        // Triggered millis timing.
    SEGMENT.params_internal.aux0 = secondHand;

    //uint8_t pixCol = (log10f((float)FFT_MajorPeak) - 2.26f) * 177;  // 10Khz sampling - log10 frequency range is from 2.26 (182hz) to 3.7 (5012hz). Let's scale accordingly.
    uint8_t pixCol = (log10f(FFT_MajorPeak) - 2.26f) * 150;           // 22Khz sampling - log10 frequency range is from 2.26 (182hz) to 3.967 (9260hz). Let's scale accordingly.
    if (FFT_MajorPeak < 182.0f) pixCol = 0;                           // handle underflow

    if (samplePeak) {
      SEGMENT.setPixelColor(SEGLEN-1, CHSV(92,92,92));
    } else {
      SEGMENT.setPixelColor(SEGLEN-1, Segment::color_blend(SEGCOLOR_U32(1), SEGMENT.color_from_palette(pixCol+SEGMENT.intensity, false, PALETTE_SOLID_WRAP, 0), (int)my_magnitude));
    }
    for (int i = 0; i < SEGLEN-1; i++) SEGMENT.setPixelColor(i, SEGMENT.getPixelColor(i+1)); // shift left
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_WATERFALL[] PROGMEM = "Waterfall@!,Adjust color,Select bin,Volume (min);!,!;!;1f;c2=0,m12=2,si=0"; // Circles, Beatsin
#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : 2D GEQ
 * @description:   : Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
void mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_GED()
{
// uint16_t mode_2DGEQ(void) { // By Will Tatam. Code reduction by Ewoud Wijma.
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const int NUM_BANDS = map(SEGMENT.custom1, 0, 255, 1, 16);
  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  if (!SEGMENT.allocateData(cols*sizeof(uint16_t))) return EffectAnim__Solid_Colour(); //allocation failed
  uint16_t *previousBarHeight = reinterpret_cast<uint16_t*>(SEGMENT.data); //array of previous bar heights per frequency band

  um_data_t *um_data;
  // if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  // }
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) for (int i=0; i<cols; i++) previousBarHeight[i] = 0;

  bool rippleTime = false;
  if (millis() - SEGMENT.step >= (256U - SEGMENT.intensity)) {
    SEGMENT.step = millis();
    rippleTime = true;
  }

  int fadeoutDelay = (256 - SEGMENT.speed) / 64;
  if ((fadeoutDelay <= 1 ) || ((SEGMENT.call % fadeoutDelay) == 0)) SEGMENT.fadeToBlackBy(SEGMENT.speed);

  for (int x=0; x < cols; x++) {
    uint8_t  band       = map(x, 0, cols-1, 0, NUM_BANDS - 1);
    if (NUM_BANDS < 16) band = map(band, 0, NUM_BANDS - 1, 0, 15); // always use full range. comment out this line to get the previous behaviour.
    band = constrain(band, 0, 15);
    uint16_t colorIndex = band * 17;
    uint16_t barHeight  = map(fftResult[band], 0, 255, 0, rows); // do not subtract -1 from rows here
    if (barHeight > previousBarHeight[x]) previousBarHeight[x] = barHeight; //drive the peak up

    uint32_t ledColor = BLACK;
    for (int y=0; y < barHeight; y++) {
      if (SEGMENT.check1) //color_vertical / color bars toggle
        colorIndex = map(y, 0, rows-1, 0, 255);

      ledColor = SEGMENT.color_from_palette(colorIndex, false, PALETTE_SOLID_WRAP, 0);
      SEGMENT.setPixelColorXY_CRGB(x, rows-1 - y, ledColor);
    }
    if (previousBarHeight[x] > 0)
      SEGMENT.setPixelColorXY_CRGB(x, rows - previousBarHeight[x], (SEGCOLOR_U32(2) != BLACK) ? SEGCOLOR_U32(2) : ledColor);

    if (rippleTime && previousBarHeight[x]>0) previousBarHeight[x]--;    //delay/ripple effect
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DGEQ()
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_GED__INDEXING[] PROGMEM = "GEQ@Fade speed,Ripple decay,# of bands,,,Color bars;!,,Peaks;!;2f;c1=255,c2=64,pal=11,si=0"; // Beatsin

#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : 2D Funky plank 
 * @description:   : Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
void mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_FunkyPlank()
{

  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  int NUMB_BANDS = map(SEGMENT.custom1, 0, 255, 1, 16);
  int barWidth = (cols / NUMB_BANDS);
  int bandInc = 1;
  if (barWidth == 0) {
    // Matrix narrower than fft bands
    barWidth = 1;
    bandInc = (NUMB_BANDS / cols);
  }

  um_data_t *um_data;
  // if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    // add support for no audio
    um_data = simulateSound(SEGMENT.soundSim);
  // }
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];

  if (SEGMENT.call == 0) {
    SEGMENT.fill(BLACK);
  }

  uint8_t secondHand = micros()/(256-SEGMENT.speed)/500+1 % 64;
  if (SEGMENT.params_internal.aux0 != secondHand) {                        // Triggered millis timing.
    SEGMENT.params_internal.aux0 = secondHand;

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
        SEGMENT.setPixelColorXY_CRGB(j, i, SEGMENT.getPixelColorXY(j, i-1));
      }
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__INDEXING[] PROGMEM = "Funky Plank@Scroll speed,,# of bands;;;2f;si=0"; // Beatsin
#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D

/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D

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

void mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_Akemi()
{
// uint16_t mode_2DAkemi(void) {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  uint16_t counter = (millis_at_start_of_effect_update * ((SEGMENT.speed >> 2) +2)) & 0xFFFF;
  counter = counter >> 8;

  const float lightFactor  = 0.15f;
  const float normalFactor = 0.4f;

  um_data_t *um_data;
  // if (!usermods.getUMData(&um_data, USERMOD_ID_AUDIOREACTIVE)) {
    um_data = simulateSound(SEGMENT.soundSim);
  // }
  uint8_t *fftResult = (uint8_t*)um_data->u_data[2];
  float base = fftResult[0]/255.0f;

  //draw and color Akemi
  for (int y=0; y < rows; y++) for (int x=0; x < cols; x++) {
    CRGB color;
    CRGB soundColor = ORANGE;
    CRGB faceColor  = SEGMENT.color_wheel(counter);
    CRGB armsAndLegsColor = SEGCOLOR_U32(1) > 0 ? SEGCOLOR_U32(1) : 0xFFE0A0; //default warmish white 0xABA8FF; //0xFF52e5;//
    uint8_t ak = pgm_read_byte_near(akemi + ((y * 32)/rows) * 32 + (x * 32)/cols); // akemi[(y * 32)/rows][(x * 32)/cols]
    switch (ak) {
      case 3: armsAndLegsColor.r *= lightFactor;  armsAndLegsColor.g *= lightFactor;  armsAndLegsColor.b *= lightFactor;  color = armsAndLegsColor; break; //light arms and legs 0x9B9B9B
      case 2: armsAndLegsColor.r *= normalFactor; armsAndLegsColor.g *= normalFactor; armsAndLegsColor.b *= normalFactor; color = armsAndLegsColor; break; //normal arms and legs 0x888888
      case 1: color = armsAndLegsColor; break; //dark arms and legs 0x686868
      case 6: faceColor.r *= lightFactor;  faceColor.g *= lightFactor;  faceColor.b *= lightFactor;  color=faceColor; break; //light face 0x31AAFF
      case 5: faceColor.r *= normalFactor; faceColor.g *= normalFactor; faceColor.b *= normalFactor; color=faceColor; break; //normal face 0x0094FF
      case 4: color = faceColor; break; //dark face 0x007DC6
      case 7: color = SEGCOLOR_U32(2) > 0 ? SEGCOLOR_U32(2) : 0xFFFFFF; break; //eyes and mouth default white
      case 8: if (base > 0.4) {soundColor.r *= base; soundColor.g *= base; soundColor.b *= base; color=soundColor;} else color = armsAndLegsColor; break;
      default: color = BLACK; break;
    }

    if (SEGMENT.intensity > 128 && fftResult && fftResult[0] > 128) { //dance if base is high
      SEGMENT.setPixelColorXY_CRGB(x, 0, BLACK);
      SEGMENT.setPixelColorXY_CRGB(x, y+1, color);
    } else
      SEGMENT.setPixelColorXY_CRGB(x, y, color);
  }

  //add geq left and right
  if (um_data && fftResult) {
    for (int x=0; x < cols/8; x++) {
      uint16_t band = x * cols/8;
      band = constrain(band, 0, 15);
      uint16_t barHeight = map(fftResult[band], 0, 255, 0, 17*rows/32);
      CRGB color = SEGMENT.GetPaletteColour((band * 35), WLED_PALETTE_MAPPING_ARG_FALSE, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32() ;//SEGMENT.color_from_palette((band * 35), false, PALETTE_SOLID_WRAP, 0);

      for (int y=0; y < barHeight; y++) {
        SEGMENT.setPixelColorXY_CRGB(x, rows/2-y, color);
        SEGMENT.setPixelColorXY_CRGB(cols-1-x, rows/2-y, color);
      }
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
} // mode_2DAkemi
static const char PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_AKEMI__INDEXING[] PROGMEM = "Akemi@Color speed,Dance;Head palette,Arms & Legs,Eyes & Mouth;Face palette;2f;si=0"; //beatsin
#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Distortion waves - ldirko
 * @description:   : https://editor.soulmatelights.com/gallery/1089-distorsion-waves
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
void mAnimatorLight::EffectAnim__2D__DistortionWaves()
{
  
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  uint8_t speed = SEGMENT.speed/32;
  uint8_t scale = SEGMENT.intensity/32;

  uint8_t  w = 2;

  uint16_t a  = millis()/32;
  uint16_t a2 = a/2;
  uint16_t a3 = a/3;

  uint16_t cx =  beatsin8(10-speed,0,cols-1)*scale;
  uint16_t cy =  beatsin8(12-speed,0,rows-1)*scale;
  uint16_t cx1 = beatsin8(13-speed,0,cols-1)*scale;
  uint16_t cy1 = beatsin8(15-speed,0,rows-1)*scale;
  uint16_t cx2 = beatsin8(17-speed,0,cols-1)*scale;
  uint16_t cy2 = beatsin8(14-speed,0,rows-1)*scale;
  
  uint16_t xoffs = 0;
  for (int x = 0; x < cols; x++) {
    xoffs += scale;
    uint16_t yoffs = 0;

    for (int y = 0; y < rows; y++) {
       yoffs += scale;

      byte rdistort = cos8((cos8(((x<<3)+a )&255)+cos8(((y<<3)-a2)&255)+a3   )&255)>>1; 
      byte gdistort = cos8((cos8(((x<<3)-a2)&255)+cos8(((y<<3)+a3)&255)+a+32 )&255)>>1; 
      byte bdistort = cos8((cos8(((x<<3)+a3)&255)+cos8(((y<<3)-a) &255)+a2+64)&255)>>1; 

      byte valueR = rdistort+ w*  (a- ( ((xoffs - cx)  * (xoffs - cx)  + (yoffs - cy)  * (yoffs - cy))>>7  ));
      byte valueG = gdistort+ w*  (a2-( ((xoffs - cx1) * (xoffs - cx1) + (yoffs - cy1) * (yoffs - cy1))>>7 ));
      byte valueB = bdistort+ w*  (a3-( ((xoffs - cx2) * (xoffs - cx2) + (yoffs - cy2) * (yoffs - cy2))>>7 ));

      valueR = gamma8(cos8(valueR));
      valueG = gamma8(cos8(valueG));
      valueB = gamma8(cos8(valueB));

      SEGMENT.setPixelColorXY_CRGB(x, y, RGBW32(valueR, valueG, valueB, 0)); 
    }
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__2D__DISTORTION_WAVES__INDEXING[] PROGMEM = "Distortion Waves@!,Scale;;;2";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko
 *                   Idea from https://www.youtube.com/watch?v=DiHBgITrZck&ab_channel=StefanPetrick
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
void mAnimatorLight::EffectAnim__2D__Soap()
{
// uint16_t mode_2Dsoap() {
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  const size_t dataSize = SEGMENT.width() * SEGMENT.height() * sizeof(uint8_t); // prevent reallocation if mirrored or grouped
  if (!SEGMENT.allocateData(dataSize + sizeof(uint32_t)*3)) return EffectAnim__Solid_Colour(); //allocation failed

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
    *noise32_x = random16();
    *noise32_y = random16();
    *noise32_z = random16();
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
  if (SEGMENT.call == 0 || SEGMENT.params_internal.aux0 != cols || SEGMENT.params_internal.aux1 != rows) {
    SEGMENT.params_internal.aux0 = cols;
    SEGMENT.params_internal.aux1 = rows;
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        SEGMENT.setPixelColorXY_CRGB(i, j, ColorFromPalette_WithLoad(SEGPALETTE,~noise3d[XY(i,j)]*3));
      }
    }
  }

  int zD;
  int zF;
  int amplitude;
  int8_t shiftX = 0; //(SEGMENT.custom1 - 128) / 4;
  int8_t shiftY = 0; //(SEGMENT.custom2 - 128) / 4;
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
      CRGB PixelA = Black;
      if ((zD >= 0) && (zD < cols)) PixelA = SEGMENT.getPixelColorXY(zD, y);
      else                          PixelA = ColorFromPalette_WithLoad(SEGPALETTE, ~noise3d[XY(abs(zD),y)]*3);
      CRGB PixelB = Black;
      if ((zF >= 0) && (zF < cols)) PixelB = SEGMENT.getPixelColorXY(zF, y);
      else                          PixelB = ColorFromPalette_WithLoad(SEGPALETTE, ~noise3d[XY(abs(zF),y)]*3);
      ledsbuff[x] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for (int x = 0; x < cols; x++) SEGMENT.setPixelColorXY_CRGB(x, y, ledsbuff[x]);
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
      CRGB PixelA = Black;
      if ((zD >= 0) && (zD < rows)) PixelA = SEGMENT.getPixelColorXY(x, zD);
      else                          PixelA = ColorFromPalette_WithLoad(SEGPALETTE, ~noise3d[XY(x,abs(zD))]*3); 
      CRGB PixelB = Black;
      if ((zF >= 0) && (zF < rows)) PixelB = SEGMENT.getPixelColorXY(x, zF);
      else                          PixelB = ColorFromPalette_WithLoad(SEGPALETTE, ~noise3d[XY(x,abs(zF))]*3);
      ledsbuff[y] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for (int y = 0; y < rows; y++) SEGMENT.setPixelColorXY_CRGB(x, y, ledsbuff[y]);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__2D__SOAP__INDEXING[] PROGMEM = "Soap@!,Smoothness;;!;2";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : 2D Octopus
 * @description:   :  
 *                   Idea from https://www.youtube.com/watch?v=HsA-6KIbgto&ab_channel=GreatScott%21
 *                   Octopus (https://editor.soulmatelights.com/gallery/671-octopus)
 *                   Stepko and Sutaburosu
 *                   Adapted for WLED by @blazoncek
 * @note           :  2D, NoSound
 * 
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
void mAnimatorLight::EffectAnim__2D__Octopus()
{
  
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();
  const uint8_t mapp = 180 / MAX(cols,rows);

  typedef struct {
    uint8_t angle;
    uint8_t radius;
  } map_t;

  const size_t dataSize = SEGMENT.width() * SEGMENT.height() * sizeof(map_t); // prevent reallocation if mirrored or grouped
  if (!SEGMENT.allocateData(dataSize + 2)) return EffectAnim__Solid_Colour(); //allocation failed

  map_t *rMap = reinterpret_cast<map_t*>(SEGMENT.data);
  uint8_t *offsX = reinterpret_cast<uint8_t*>(SEGMENT.data + dataSize);
  uint8_t *offsY = reinterpret_cast<uint8_t*>(SEGMENT.data + dataSize + 1);

  // re-init if SEGMENT dimensions or offset changed
  if (SEGMENT.call == 0 || SEGMENT.params_internal.aux0 != cols || SEGMENT.params_internal.aux1 != rows || SEGMENT.custom1 != *offsX || SEGMENT.custom2 != *offsY) {
    SEGMENT.step = 0; // t
    SEGMENT.params_internal.aux0 = cols;
    SEGMENT.params_internal.aux1 = rows;
    *offsX = SEGMENT.custom1;
    *offsY = SEGMENT.custom2;
    const int C_X = (cols / 2) + ((SEGMENT.custom1 - 128)*cols)/255;
    const int C_Y = (rows / 2) + ((SEGMENT.custom2 - 128)*rows)/255;
    for (int x = 0; x < cols; x++) {
      for (int y = 0; y < rows; y++) {
        rMap[XY(x, y)].angle  = 40.7436f * atan2f((y - C_Y), (x - C_X));  // avoid 128*atan2()/PI
        rMap[XY(x, y)].radius = hypotf((x - C_X), (y - C_Y)) * mapp;      //thanks Sutaburosu
      }
    }
  }

  SEGMENT.step += SEGMENT.speed / 32 + 1;  // 1-4 range
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      byte angle = rMap[XY(x,y)].angle;
      byte radius = rMap[XY(x,y)].radius;
      //CRGB c = CHSV(SEGMENT.step / 2 - radius, 255, sin8(sin8((angle * 4 - radius) / 4 + SEGMENT.step) + radius - SEGMENT.step * 2 + angle * (SEGMENT.custom3/3+1)));
      uint16_t intensity2 = sin8(sin8((angle * 4 - radius) / 4 + SEGMENT.step/2) + radius - SEGMENT.step + angle * (SEGMENT.custom3/4+1));
      intensity2 = map(intensity2*intensity2, 0, 65535, 0, 255); // add a bit of non-linearity for cleaner display
      CRGB c = ColorFromPalette_WithLoad(SEGPALETTE, SEGMENT.step / 2 - radius, intensity2);
      SEGMENT.setPixelColorXY_CRGB(x, y, c);
    }
  }
  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__2D__OCTOPUS__INDEXING[] PROGMEM = "Octopus@!,,Offset X,Offset Y,Legs;;!;2;";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D


/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
void mAnimatorLight::EffectAnim__2D__WavingCell()
{
  
  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();

  uint32_t t = millis()/(257-SEGMENT.speed);
  uint8_t aX = SEGMENT.custom1/16 + 9;
  uint8_t aY = SEGMENT.custom2/16 + 1;
  uint8_t aZ = SEGMENT.custom3 + 1;
  for (int x = 0; x < cols; x++) for (int y = 0; y <rows; y++)
    SEGMENT.setPixelColorXY_CRGB(x, y, ColorFromPalette_WithLoad(SEGPALETTE, ((sin8((x*aX)+sin8((y+t)*aY))+cos8(y*aZ))+1)+t));

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();
}
static const char PM_EFFECT_CONFIG__2D__WAVING_CELL__INDEXING[] PROGMEM = "Waving Cell@!,,Amplitude 1,Amplitude 2,Amplitude 3;;!;2"; // anything is "2D" if the function will only run when matrix is enabled
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D



/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

////////////////////////////
//     2D Scrolling text  //
////////////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
void mAnimatorLight::EffectAnim__2D__ScrollingText()
{
// void mAnimatorLight::EffectAnim__Matrix__2D_Scrolling_Text(void) 
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
  // SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

  // return;

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
  int  AmPmHour = pCONT_time->RtcTime.hour;// hour(localTime);
  bool isitAM = true;
  if (useAMPM) {
    if (AmPmHour > 11) { AmPmHour -= 12; isitAM = false; }
    if (AmPmHour == 0) { AmPmHour  = 12; }
    sprintf_P(sec, PSTR(" %2s"), (isitAM ? "AM" : "PM"));
  } else {
    sprintf_P(sec, PSTR(":%02d"), pCONT_time->RtcTime.second);//(localTime));
  }

  // DEBUG_LINE_HERE;
  if (!strlen(text)) { // fallback if empty segment name: display date and time
    
    // pCONT_time->GetDateAndTime(DT_DST).c_str();

    // sprintf_P(text, PSTR("%s %d, %d %d:%02d%s"), monthShortStr(month(localTime)), pCONT_time->RtcTime.days, pCONT_time->RtcTime.year, AmPmHour, pCONT_time->RtcTime.minute, sec);
    sprintf_P(text, PSTR("Test Message"));
  } else {
    if      (!strncmp_P(text,PSTR("#DATE"),5)) sprintf_P(text, zero?PSTR("%02d.%02d.%04d"):PSTR("%d.%d.%d"),   pCONT_time->RtcTime.days,   pCONT_time->RtcTime.month,  pCONT_time->RtcTime.year);
    else if (!strncmp_P(text,PSTR("#DDMM"),5)) sprintf_P(text, zero?PSTR("%02d.%02d")     :PSTR("%d.%d"),      pCONT_time->RtcTime.days,   pCONT_time->RtcTime.month);
    else if (!strncmp_P(text,PSTR("#MMDD"),5)) sprintf_P(text, zero?PSTR("%02d/%02d")     :PSTR("%d/%d"),      pCONT_time->RtcTime.month, pCONT_time->RtcTime.days);
    else if (!strncmp_P(text,PSTR("#TIME"),5)) sprintf_P(text, zero?PSTR("%02d:%02d%s")   :PSTR("%2d:%02d%s"), AmPmHour,         pCONT_time->RtcTime.minute, sec);
    else if (!strncmp_P(text,PSTR("#HHMM"),5)) sprintf_P(text, zero?PSTR("%02d:%02d")     :PSTR("%d:%02d"),    AmPmHour,         pCONT_time->RtcTime.minute);
    else if (!strncmp_P(text,PSTR("#HH"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         AmPmHour);
    else if (!strncmp_P(text,PSTR("#MM"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         pCONT_time->RtcTime.minute);
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
        if (SEGMENT.params_internal.aux0 == 0) SEGMENT.params_internal.aux0  = width + cols - 1;
        else                --SEGMENT.params_internal.aux0;
      } else                ++SEGMENT.params_internal.aux0 %= width + cols;
    } else                    SEGMENT.params_internal.aux0  = (cols + width)/2;
    ++SEGMENT.params_internal.aux1 &= 0xFF; // color shift
    SEGMENT.step = now + map(SEGMENT.speed, 0, 255, 250, 50); // shift letters every ~250ms to ~50ms
  }

  // DEBUG_LINE_HERE;
  if (!SEGMENT.check2) SEGMENT.fade_out(255 - (SEGMENT.custom1>>4));  // trail

  // DEBUG_LINE_HERE;
  for (int i = 0; i < numberOfLetters; i++) {
    int xoffset = int(cols) - int(SEGMENT.params_internal.aux0) + rotLW*i;
    if (xoffset + rotLW < 0) continue; // don't draw characters off-screen
    uint32_t col1 = SEGMENT.GetPaletteColour(SEGMENT.params_internal.aux1, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF).getU32(); //SEGMENT.color_from_palette(SEGMENT.params_internal.aux1, false, PALETTE_SOLID_WRAP, 0);
    uint32_t col2 = BLACK;
    if (SEGMENT.check1 && SEGMENT.palette_id == 0) {
      col1 = SEGCOLOR_U32(0); //SEGCOLOR_U32(0);
      col2 = SEGCOLOR_U32(2); //SEGCOLOR_U32(2);
    }
    SEGMENT.drawCharacter(text[i], xoffset, yoffset, letterWidth, letterHeight, col1, col2, map(SEGMENT.custom3, 0, 31, -2, 2));
  }
  // // DEBUG_LINE_HERE;

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

}
static const char PM_EFFECT_CONFIG__2D__SCROLLING_TEXT__INDEXING[] PROGMEM = "Scrolling Text@!,Y Offset,Trail,Font size,Rotate,Gradient,Overlay,Reverse;!,!,Gradient;!;2;ix=128,c1=0,rev=0,mi=0,rY=0,mY=0";
#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D


////////////////////////////
//     2D DigitalClock text  //
////////////////////////////
#ifdef  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
void mAnimatorLight::EffectAnim__2D__DigitalClock()
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
  int  AmPmHour = pCONT_time->RtcTime.hour;// hour(localTime);
  bool isitAM = true;
  if (useAMPM) {
    if (AmPmHour > 11) { AmPmHour -= 12; isitAM = false; }
    if (AmPmHour == 0) { AmPmHour  = 12; }
    sprintf_P(sec, PSTR(" %2s"), (isitAM ? "AM" : "PM"));
  } else {
    sprintf_P(sec, PSTR(":%02d"), pCONT_time->RtcTime.second);//(localTime));
  }
  
  sprintf_P(text, PSTR("#HHMM"));

  // DEBUG_LINE_HERE;
  if (!strlen(text)) { // fallback if empty segment name: display date and time
    
    // pCONT_time->GetDateAndTime(DT_DST).c_str();

    // sprintf_P(text, PSTR("%s %d, %d %d:%02d%s"), monthShortStr(month(localTime)), pCONT_time->RtcTime.days, pCONT_time->RtcTime.year, AmPmHour, pCONT_time->RtcTime.minute, sec);
    sprintf_P(text, PSTR("Test Message"));
  } else {
    if      (!strncmp_P(text,PSTR("#DATE"),5)) sprintf_P(text, zero?PSTR("%02d.%02d.%04d"):PSTR("%d.%d.%d"),   pCONT_time->RtcTime.days,   pCONT_time->RtcTime.month,  pCONT_time->RtcTime.year);
    else if (!strncmp_P(text,PSTR("#DDMM"),5)) sprintf_P(text, zero?PSTR("%02d.%02d")     :PSTR("%d.%d"),      pCONT_time->RtcTime.days,   pCONT_time->RtcTime.month);
    else if (!strncmp_P(text,PSTR("#MMDD"),5)) sprintf_P(text, zero?PSTR("%02d/%02d")     :PSTR("%d/%d"),      pCONT_time->RtcTime.month, pCONT_time->RtcTime.days);
    else if (!strncmp_P(text,PSTR("#TIME"),5)) sprintf_P(text, zero?PSTR("%02d:%02d%s")   :PSTR("%2d:%02d%s"), AmPmHour,         pCONT_time->RtcTime.minute, sec);
    else if (!strncmp_P(text,PSTR("#HHMM"),5)) sprintf_P(text, zero?PSTR("%02d:%02d")     :PSTR("%d:%02d"),    AmPmHour,         pCONT_time->RtcTime.minute);
    else if (!strncmp_P(text,PSTR("#HH"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         AmPmHour);
    else if (!strncmp_P(text,PSTR("#MM"),3))   sprintf_P(text, zero?PSTR("%02d")          :PSTR("%d"),         pCONT_time->RtcTime.minute);
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

  SEGMENT.params_internal.aux0  = (cols + width)/2; // Center the text or set to any fixed offset

  bool solidPerChar                 = !SEGMENT.check1;
  bool horizontalGradient           = !SEGMENT.check2;
  bool backgroundGradientHorizontal = !SEGMENT.check3;

  // if (!SEGMENT.check2) 
  SEGMENT.fade_out(255 - (SEGMENT.custom1>>4));  // trail

  for (int i = 0; i < numberOfLetters; i++) {
    int xoffset = int(cols) - int(SEGMENT.params_internal.aux0) + rotLW * i;
    if (xoffset + rotLW < 0) continue; // don't draw characters off-screen
    // Use the new function with the desired flags (solidPerChar and horizontalGradient)
    SEGMENT.drawCharacter_UsingGradientPalletes(text[i], xoffset, yoffset, letterWidth, letterHeight, map(SEGMENT.custom3, 0, 31, -2, 2), solidPerChar, horizontalGradient, backgroundGradientHorizontal);
  }

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

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
#endif //  ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D




/********************************************************************************************************************************************************************************************************************
 *******************************************************************************************************************************************************************************************************************
 * @name           : Waving Cell
 * @description:   :  
 *                   Inspired by @Stepko (https://editor.soulmatelights.com/gallery/1704-wavingcells)
 *                   Adapted for WLED by @blazoncek
 * @note           :  
 * 
 * @param intensity: 
 * @param speed    : 
 * @param rate     : None
 * @param time     : None
 * @param aux0     : 
 * @param aux1     : 
 * @param aux2     : 
 * @param aux3     : 
 *******************************************************************************************************************************************************************************************************************
 ********************************************************************************************************************************************************************************************************************/

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
/////////////////////
//      2D DNA     //
/////////////////////
void mAnimatorLight::EffectAnim__2D__DNA() // dna originally by by ldirko at https://pastebin.com/pCkkkzcs. Updated by Preyy. WLED conversion by Andrew Tuline.
{

  if (!isMatrix) return EffectAnim__Solid_Colour(); // not a 2D set-up

  const uint16_t cols = SEGMENT.virtualWidth();
  const uint16_t rows = SEGMENT.virtualHeight();
  
  SEGMENT.fadeToBlackBy(64);
  for (int i = 0; i < cols; i++) 
  {
    SEGMENT.setPixelColorXY_CRGB(i, beatsin8(SEGMENT.speed/8, 0, rows-1, 0, i*4    ), ColorFromPalette_WithLoad(SEGPALETTE, i*5+millis()/17, beatsin8(5, 55, 255, 0, i*10), LINEARBLEND));
    SEGMENT.setPixelColorXY_CRGB(i, beatsin8(SEGMENT.speed/8, 0, rows-1, 0, i*4+128), ColorFromPalette_WithLoad(SEGPALETTE, i*5+128+millis()/17, beatsin8(5, 55, 255, 0, i*10+128), LINEARBLEND));
  }
  SEGMENT.blur(SEGMENT.intensity>>3);

  SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
  SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

} // mode_2Ddna()
static const char PM_EFFECT_CONFIG__2D__DNA__INDEXING[] PROGMEM = "DNA@Scroll speed,Blur;;!;2";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D





/**
 * @brief 
 * 
 * Effect Ideas:
 * 
 * (1) Wipe Random from the top, that fills pixel by pixel until it reaches an end stop on the bottom. This end may jump in single pixels, or in multiples (ie grouping=200). The effect would be banded falling tetris blocks and when the banding is complete, restart
 *     - Using a background colour, either black or white for the "unfilled" part will give nice effects
 * (2) A vertical strip shows the sky as horizon to top. Hence, instead of the moving gradient, 16 key points on it will be set and change depending on the time of day to reflect the darkening sunset.
 * 
 */
void mAnimatorLight::LoadEffects()
{

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__SOLID_COLOUR__ID,                  
            &mAnimatorLight::EffectAnim__Solid_Colour,                    
            PM_EFFECT_CONFIG__SOLID_COLOUR, 
            Effect_DevStage::Release);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__STATIC_PALETTE__ID,                
            &mAnimatorLight::EffectAnim__Static_Palette,                  
            PM_EFFECT_CONFIG__STATIC_PALETTE, 
            Effect_DevStage::Release);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__SPANNED_PALETTE__ID,                            
            &mAnimatorLight::EffectAnim__Spanned_Static_Palette,  
            PM_EFFECT_CONFIG__SPANNED_PALETTE, 
            Effect_DevStage::Beta);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__SLOW_GLOW__ID,                     
            &mAnimatorLight::EffectAnim__Slow_Glow,                       
            PM_EFFECT_CONFIG__SLOW_GLOW, 
            Effect_DevStage::Release);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__CANDLE_SINGLE__ID,            
            &mAnimatorLight::EffectAnim__Candle_Single,                   
            PM_EFFECT_CONFIG__CANDLE_SINGLE, 
            Effect_DevStage::Alpha);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__CANDLE_MULTIPLE__ID,          
            &mAnimatorLight::EffectAnim__Candle_Multiple,                 
            PM_EFFECT_CONFIG__CANDLE_MULTIPLE, 
            Effect_DevStage::Alpha);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__SHIMMERING_PALETTE_DOUBLE__ID,     
            &mAnimatorLight::EffectAnim__Shimmering_Two_Palette,          
            PM_EFFECT_CONFIG__SHIMMERING_TWO_PALETTES, 
            Effect_DevStage::Dev);
  #endif  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__ID, &mAnimatorLight::EffectAnim__Shimmering_Palette_Saturation,   PM_EFFECT_CONFIG__SHIMMERING_PALETTE_SATURATION);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__ID,       &mAnimatorLight::EffectAnim__Static_Gradient_Palette,         PM_EFFECT_CONFIG__STATIC_GRADIENT_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__ROTATING_PALETTE__ID,              &mAnimatorLight::EffectAnim__Rotating_Palette,                PM_EFFECT_CONFIG__ROTATING_PALETTE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,   &mAnimatorLight::EffectAnim__Rotating_Previous_Animation,     PM_EFFECT_CONFIG__ROTATING_PREVIOUS_ANIMATION);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__STEPPING_PALETTE_WITH_BACKGROUND__ID,           &mAnimatorLight::EffectAnim__Stepping_Palette_With_Background,                      PM_EFFECT_CONFIG__STEPPING_PALETTE_WITH_BACKGROUND);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__STEPPING_PALETTE__ID,                           &mAnimatorLight::EffectAnim__Stepping_Palette,                      PM_EFFECT_CONFIG__STEPPING_PALETTE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,      &mAnimatorLight::EffectAnim__Blend_Two_Palettes,                    PM_EFFECT_CONFIG__BLEND_TWO_PALETTES);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID, &mAnimatorLight::EffectAnim__Twinkle_Palette_Onto_Palette,          PM_EFFECT_CONFIG__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__TWINKLE_OFF_PALETTE__ID,                        &mAnimatorLight::EffectAnim__Twinkle_Out_Palette,          PM_EFFECT_CONFIG__TWINKLE_OUT_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  addEffect(EFFECTS_FUNCTION__STATIC_PALETTE_VINTAGE__ID,                
            &mAnimatorLight::EffectAnim__Static_Palette_Vintage,                  
            PM_EFFECT_CONFIG__STATIC_PALETTE_VINTAGE, 
            Effect_DevStage::Dev);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  addEffect(EFFECTS_FUNCTION__TIMEBASED__HOUR_PROGRESS__ID,  
            &mAnimatorLight::EffectAnim__TimeBased__HourProgress,             
            PM_EFFECT_CONFIG__TIMEBASED__HOUR_PROGRESS); 
  #endif  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__ID,                   
            &mAnimatorLight::EffectAnim__Twinkle_Decaying_Palette,              
            PM_EFFECT_CONFIG__TWINKLE_DECAYING_PALETTE);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_BLACK__ID,                      &mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_Black,PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_BLACK, Effect_DevStage::Unstable);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_BLACK__ID,                       &mAnimatorLight::EffectAnim__Popping_Decay_Random_To_Black,                                                PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_BLACK);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_WHITE__ID,                       &mAnimatorLight::EffectAnim__Popping_Decay_Palette_To_White,                                                PM_EFFECT_CONFIG__POPPING_DECAY_PALETTE_TO_WHITE);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_WHITE__ID,                       &mAnimatorLight::EffectAnim__Popping_Decay_Random_To_White,                                                PM_EFFECT_CONFIG__POPPING_DECAY_RANDOM_TO_WHITE);
  #endif 
  // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING   
  // addEffect(EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__ID,            &mAnimatorLight::SubTask_Flasher_Animate_Function__Static_Palette_Spanning_Segment, PM_EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__NAME_CTR); 
  // #endif        
  /**
   * Static
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__STATIC_PATTERN__ID,       &mAnimatorLight::EffectAnim__Static_Pattern,                        PM_EFFECT_CONFIG__STATIC_PATTERN);
  addEffect(EFFECTS_FUNCTION__TRI_STATIC_PATTERN__ID,   &mAnimatorLight::EffectAnim__Tri_Static_Pattern,                    PM_EFFECT_CONFIG__TRI_STATIC_PATTERN);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__SPOTS__ID,                &mAnimatorLight::EffectAnim__Spots,                                 PM_EFFECT_CONFIG__SPOTS);
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__PERCENT__ID,              &mAnimatorLight::EffectAnim__Percent,                               PM_EFFECT_CONFIG__PERCENT);
  #endif
  /**
   * One Colour
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__RANDOM_COLOR__ID,         &mAnimatorLight::EffectAnim__Random_Colour,                         PM_EFFECT_CONFIG__RANDOM_COLOR);
  #endif
  /**
   * Wipe/Sweep/Runners 
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__COLOR_WIPE__ID,           &mAnimatorLight::EffectAnim__Colour_Wipe,                           PM_EFFECT_CONFIG__COLOR_WIPE);
  addEffect(EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID,    &mAnimatorLight::EffectAnim__Colour_Wipe_Random,                    PM_EFFECT_CONFIG__COLOR_WIPE_RANDOM);
  addEffect(EFFECTS_FUNCTION__COLOR_WIPE_PALETTE__ID,   &mAnimatorLight::EffectAnim__Colour_Wipe_Palette,                   PM_EFFECT_CONFIG__COLOR_WIPE_PALETTE);
  addEffect(EFFECTS_FUNCTION__COLOR_SWEEP__ID,          &mAnimatorLight::EffectAnim__Colour_Sweep,                          PM_EFFECT_CONFIG__COLOR_SWEEP);
  addEffect(EFFECTS_FUNCTION__COLOR_SWEEP_RANDOM__ID,   &mAnimatorLight::EffectAnim__Colour_Sweep_Random,                   PM_EFFECT_CONFIG__COLOR_SWEEP_RANDOM);
  addEffect(EFFECTS_FUNCTION__COLOR_SWEEP_PALETTE__ID,  &mAnimatorLight::EffectAnim__Colour_Sweep_Palette,                  PM_EFFECT_CONFIG__COLOR_SWEEP_PALETTE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Fireworks
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  addEffect(EFFECTS_FUNCTION__FIREWORKS__ID,                     &mAnimatorLight::EffectAnim__Fireworks,                    PM_EFFECT_CONFIG__FIREWORKS);
  addEffect(EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID,           &mAnimatorLight::EffectAnim__Exploding_Fireworks,          PM_EFFECT_CONFIG__EXPLODING_FIREWORKS);
  addEffect(EFFECTS_FUNCTION__FIREWORKS_STARBURST__ID,           &mAnimatorLight::EffectAnim__Fireworks_Starburst,          PM_EFFECT_CONFIG__STARBURST);
  addEffect(EFFECTS_FUNCTION__FIREWORKS_STARBURST_GLOWS__ID,     &mAnimatorLight::EffectAnim__Fireworks_Starburst_Glows,    PM_EFFECT_CONFIG__STARBURST_GLOWS);
  addEffect(EFFECTS_FUNCTION__RAIN__ID,                          &mAnimatorLight::EffectAnim__Rain,                         PM_EFFECT_CONFIG__RAIN);
  addEffect(EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID, &mAnimatorLight::EffectAnim__Exploding_Fireworks_NoLaunch, PM_EFFECT_CONFIG__EXPLODING_FIREWORKS_NOLAUNCH);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__TRICOLOR_WIPE__ID,         &mAnimatorLight::EffectAnim__TriColour,          PM_EFFECT_CONFIG__TRICOLOR_WIPE);
  addEffect(EFFECTS_FUNCTION__ANDROID__ID,               &mAnimatorLight::EffectAnim__Android,            PM_EFFECT_CONFIG__ANDROID);
  addEffect(EFFECTS_FUNCTION__RUNNING_COLOR__ID,         &mAnimatorLight::EffectAnim__Running_Colour,     PM_EFFECT_CONFIG__RUNNING_COLOR);
  addEffect(EFFECTS_FUNCTION__RUNNING_RANDOM__ID,        &mAnimatorLight::EffectAnim__Running_Random,     PM_EFFECT_CONFIG__RUNNING_RANDOM);
  addEffect(EFFECTS_FUNCTION__GRADIENT__ID,              &mAnimatorLight::EffectAnim__Gradient,           PM_EFFECT_CONFIG__GRADIENT);
  addEffect(EFFECTS_FUNCTION__LOADING__ID,               &mAnimatorLight::EffectAnim__Loading,            PM_EFFECT_CONFIG__LOADING);
  addEffect(EFFECTS_FUNCTION__POLICE__ID,                &mAnimatorLight::EffectAnim__Police,             PM_EFFECT_CONFIG__POLICE);
  addEffect(EFFECTS_FUNCTION__POLICE_ALL__ID,            &mAnimatorLight::EffectAnim__Polce_All,          PM_EFFECT_CONFIG__POLICE_ALL);
  addEffect(EFFECTS_FUNCTION__TWO_DOTS__ID,              &mAnimatorLight::EffectAnim__Two_Dots,           PM_EFFECT_CONFIG__TWO_DOTS);
  addEffect(EFFECTS_FUNCTION__TWO_AREAS__ID,             &mAnimatorLight::EffectAnim__Two_Areas,          PM_EFFECT_CONFIG__TWO_AREAS);
  addEffect(EFFECTS_FUNCTION__MULTI_COMET__ID,           &mAnimatorLight::EffectAnim__Multi_Comet,        PM_EFFECT_CONFIG__MULTI_COMET); // PM_EFFECTS_FUNCTION__[A-Z_]+__NAME_CTR,\s*
  addEffect(EFFECTS_FUNCTION__OSCILLATE__ID,             &mAnimatorLight::EffectAnim__Oscillate,          PM_EFFECT_CONFIG__OSCILLATE);
  addEffect(EFFECTS_FUNCTION__BPM__ID,                   &mAnimatorLight::EffectAnim__BPM,                PM_EFFECT_CONFIG__BPM);
  addEffect(EFFECTS_FUNCTION__JUGGLE__ID,                &mAnimatorLight::EffectAnim__Juggle,             PM_EFFECT_CONFIG__JUGGLES);
  addEffect(EFFECTS_FUNCTION__PALETTE__ID,               &mAnimatorLight::EffectAnim__Palette,            PM_EFFECT_CONFIG__PALETTE);
  addEffect(EFFECTS_FUNCTION__COLORWAVES__ID,            &mAnimatorLight::EffectAnim__ColourWaves,        PM_EFFECT_CONFIG__COLORWAVES);
  addEffect(EFFECTS_FUNCTION__LAKE__ID,                  &mAnimatorLight::EffectAnim__Lake,               PM_EFFECT_CONFIG__LAKE);
  addEffect(EFFECTS_FUNCTION__GLITTER__ID,               &mAnimatorLight::EffectAnim__Glitter,            PM_EFFECT_CONFIG__GLITTER);
  addEffect(EFFECTS_FUNCTION__METEOR__ID,                &mAnimatorLight::EffectAnim__Meteor,             PM_EFFECT_CONFIG__METEOR);
  addEffect(EFFECTS_FUNCTION__METEOR_SMOOTH__ID,         &mAnimatorLight::EffectAnim__Metoer_Smooth,      PM_EFFECT_CONFIG__METEOR_SMOOTH);
  addEffect(EFFECTS_FUNCTION__PRIDE_2015__ID,            &mAnimatorLight::EffectAnim__Pride_2015,         PM_EFFECT_CONFIG__PRIDE_2015);
  addEffect(EFFECTS_FUNCTION__PACIFICA__ID,              &mAnimatorLight::EffectAnim__Pacifica,           PM_EFFECT_CONFIG__PACIFICA);
  addEffect(EFFECTS_FUNCTION__SUNRISE__ID,               &mAnimatorLight::EffectAnim__Sunrise,            PM_EFFECT_CONFIG__SUNRISE);
  addEffect(EFFECTS_FUNCTION__SINEWAVE__ID,              &mAnimatorLight::EffectAnim__Sinewave,           PM_EFFECT_CONFIG__SINEWAVE);
  addEffect(EFFECTS_FUNCTION__FLOW__ID,                  &mAnimatorLight::EffectAnim__Flow,               PM_EFFECT_CONFIG__FLOW);
  addEffect(EFFECTS_FUNCTION__RUNNING_LIGHTS__ID,        &mAnimatorLight::EffectAnim__Running_Lights,     PM_EFFECT_CONFIG__RUNNING_LIGHTS);
  addEffect(EFFECTS_FUNCTION__RAINBOW_CYCLE__ID,         &mAnimatorLight::EffectAnim__Rainbow_Cycle,      PM_EFFECT_CONFIG__RAINBOW_CYCLE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Chase
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__CHASE_COLOR__ID,           &mAnimatorLight::EffectAnim__Chase_Colour,          PM_EFFECT_CONFIG__CHASE_COLOR);
  addEffect(EFFECTS_FUNCTION__CHASE_RANDOM__ID,          &mAnimatorLight::EffectAnim__Chase_Random,          PM_EFFECT_CONFIG__CHASE_RANDOM);
  addEffect(EFFECTS_FUNCTION__CHASE_RAINBOW__ID,         &mAnimatorLight::EffectAnim__Chase_Rainbow,         PM_EFFECT_CONFIG__CHASE_RAINBOW);
  addEffect(EFFECTS_FUNCTION__CHASE_FLASH__ID,           &mAnimatorLight::EffectAnim__Chase_Flash,           PM_EFFECT_CONFIG__CHASE_FLASH);
  addEffect(EFFECTS_FUNCTION__CHASE_FLASH_RANDOM__ID,    &mAnimatorLight::EffectAnim__Chase_Flash_Random,    PM_EFFECT_CONFIG__CHASE_FLASH_RANDOM);
  addEffect(EFFECTS_FUNCTION__CHASE_RAINBOW_WHITE__ID,   &mAnimatorLight::EffectAnim__Chase_Rainbow_White,   PM_EFFECT_CONFIG__CHASE_RAINBOW_WHITE);
  addEffect(EFFECTS_FUNCTION__CHASE_THEATER__ID,         &mAnimatorLight::EffectAnim__Chase_Theater,         PM_EFFECT_CONFIG__THEATER_CHASE);
  addEffect(EFFECTS_FUNCTION__CHASE_THEATER_RAINBOW__ID, &mAnimatorLight::EffectAnim__Chase_Theatre_Rainbow, PM_EFFECT_CONFIG__THEATER_CHASE_RAINBOW);
  addEffect(EFFECTS_FUNCTION__CHASE_TRICOLOR__ID,        &mAnimatorLight::EffectAnim__Chase_TriColour,       PM_EFFECT_CONFIG__TRICOLOR_CHASE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   *  Breathe/Fade/Pulse
   **/    
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__BREATH__ID,                &mAnimatorLight::EffectAnim__Breath,                PM_EFFECT_CONFIG__BREATH);
  addEffect(EFFECTS_FUNCTION__FADE__ID,                  &mAnimatorLight::EffectAnim__Fade,                  PM_EFFECT_CONFIG__FADE);
  addEffect(EFFECTS_FUNCTION__FADE_TRICOLOR__ID,         &mAnimatorLight::EffectAnim__Fade_TriColour,        PM_EFFECT_CONFIG__TRICOLOR_FADE);
  addEffect(EFFECTS_FUNCTION__FADE_SPOTS__ID,            &mAnimatorLight::EffectAnim__Fade_Spots,            PM_EFFECT_CONFIG__SPOTS_FADE);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Sparkle/Twinkle
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__SOLID_GLITTER__ID,   &mAnimatorLight::EffectAnim__Solid_Glitter,               PM_EFFECT_CONFIG__SOLID_GLITTER);
  addEffect(EFFECTS_FUNCTION__POPCORN__ID,         &mAnimatorLight::EffectAnim__Popcorn,                     PM_EFFECT_CONFIG__POPCORN);
  addEffect(EFFECTS_FUNCTION__PLASMA__ID,          &mAnimatorLight::EffectAnim__Plasma,                      PM_EFFECT_CONFIG__PLASMA);
  addEffect(EFFECTS_FUNCTION__SPARKLE__ID,         &mAnimatorLight::EffectAnim__Sparkle,                     PM_EFFECT_CONFIG__SPARKLE);
  addEffect(EFFECTS_FUNCTION__FLASH_SPARKLE__ID,   &mAnimatorLight::EffectAnim__Sparkle_Flash,               PM_EFFECT_CONFIG__FLASH_SPARKLE);
  addEffect(EFFECTS_FUNCTION__HYPER_SPARKLE__ID,   &mAnimatorLight::EffectAnim__Sparkle_Hyper,               PM_EFFECT_CONFIG__HYPER_SPARKLE);
  addEffect(EFFECTS_FUNCTION__TWINKLE__ID,         &mAnimatorLight::EffectAnim__Twinkle,                     PM_EFFECT_CONFIG__TWINKLE);
  addEffect(EFFECTS_FUNCTION__COLORTWINKLE__ID,    &mAnimatorLight::EffectAnim__Twinkle_Colour,              PM_EFFECT_CONFIG__COLORTWINKLE);
  addEffect(EFFECTS_FUNCTION__TWINKLE_FOX__ID,     &mAnimatorLight::EffectAnim__Twinkle_Fox,                 PM_EFFECT_CONFIG__TWINKLE_FOX);
  addEffect(EFFECTS_FUNCTION__TWINKLE_CAT__ID,     &mAnimatorLight::EffectAnim__Twinkle_Cat,                 PM_EFFECT_CONFIG__TWINKLE_CAT);
  addEffect(EFFECTS_FUNCTION__TWINKLE_UP__ID,      &mAnimatorLight::EffectAnim__Twinkle_Up,                  PM_EFFECT_CONFIG__TWINKLE_UP);
  addEffect(EFFECTS_FUNCTION__SAW__ID,             &mAnimatorLight::EffectAnim__Saw,                         PM_EFFECT_CONFIG__SAW);
  addEffect(EFFECTS_FUNCTION__DISSOLVE__ID,        &mAnimatorLight::EffectAnim__Dissolve,                    PM_EFFECT_CONFIG__DISSOLVE);
  addEffect(EFFECTS_FUNCTION__DISSOLVE_RANDOM__ID, &mAnimatorLight::EffectAnim__Dissolve_Random,             PM_EFFECT_CONFIG__DISSOLVE_RANDOM);
  addEffect(EFFECTS_FUNCTION__COLORFUL__ID,        &mAnimatorLight::EffectAnim__ColourFul,                   PM_EFFECT_CONFIG__COLORFUL);
  addEffect(EFFECTS_FUNCTION__TRAFFIC_LIGHT__ID,   &mAnimatorLight::EffectAnim__Traffic_Light,               PM_EFFECT_CONFIG__TRAFFIC_LIGHT);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Blink/Strobe
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__BLINK__ID,           &mAnimatorLight::EffectAnim__Blink,                       PM_EFFECT_CONFIG__BLINK);
  addEffect(EFFECTS_FUNCTION__BLINK_RAINBOW__ID,   &mAnimatorLight::EffectAnim__Blink_Rainbow,               PM_EFFECT_CONFIG__BLINK_RAINBOW);
  addEffect(EFFECTS_FUNCTION__STROBE__ID,          &mAnimatorLight::EffectAnim__Strobe,                      PM_EFFECT_CONFIG__STROBE);
  addEffect(EFFECTS_FUNCTION__MULTI_STROBE__ID,    &mAnimatorLight::EffectAnim__Strobe_Multi,                PM_EFFECT_CONFIG__MULTI_STROBE);
  addEffect(EFFECTS_FUNCTION__STROBE_RAINBOW__ID,  &mAnimatorLight::EffectAnim__Strobe_Rainbow,              PM_EFFECT_CONFIG__STROBE_RAINBOW);
  addEffect(EFFECTS_FUNCTION__RAINBOW__ID,         &mAnimatorLight::EffectAnim__Rainbow,                     PM_EFFECT_CONFIG__RAINBOW);
  addEffect(EFFECTS_FUNCTION__LIGHTNING__ID,       &mAnimatorLight::EffectAnim__Lightning,                   PM_EFFECT_CONFIG__LIGHTNING);
  addEffect(EFFECTS_FUNCTION__FIRE_2012__ID,       &mAnimatorLight::EffectAnim__Fire_2012,                   PM_EFFECT_CONFIG__FIRE_2012);
  addEffect(EFFECTS_FUNCTION__RAILWAY__ID,         &mAnimatorLight::EffectAnim__Railway,                     PM_EFFECT_CONFIG__RAILWAY);
  addEffect(EFFECTS_FUNCTION__HEARTBEAT__ID,       &mAnimatorLight::EffectAnim__Heartbeat,                   PM_EFFECT_CONFIG__HEARTBEAT);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Noise
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__FILLNOISE8__ID,      &mAnimatorLight::EffectAnim__FillNoise8,                  PM_EFFECT_CONFIG__FILLNOISE8);
  addEffect(EFFECTS_FUNCTION__NOISE16_1__ID,       &mAnimatorLight::EffectAnim__Noise16_1,                   PM_EFFECT_CONFIG__NOISE16_1);
  addEffect(EFFECTS_FUNCTION__NOISE16_2__ID,       &mAnimatorLight::EffectAnim__Noise16_2,                   PM_EFFECT_CONFIG__NOISE16_2);
  addEffect(EFFECTS_FUNCTION__NOISE16_3__ID,       &mAnimatorLight::EffectAnim__Noise16_3,                   PM_EFFECT_CONFIG__NOISE16_3);
  addEffect(EFFECTS_FUNCTION__NOISE16_4__ID,       &mAnimatorLight::EffectAnim__Noise16_4,                   PM_EFFECT_CONFIG__NOISE16_4);
  addEffect(EFFECTS_FUNCTION__NOISEPAL__ID,        &mAnimatorLight::EffectAnim__Noise_Pal,                   PM_EFFECT_CONFIG__NOISEPAL);
  addEffect(EFFECTS_FUNCTION__PHASEDNOISE__ID,     &mAnimatorLight::EffectAnim__PhasedNoise,                 PM_EFFECT_CONFIG__PHASEDNOISE);
  addEffect(EFFECTS_FUNCTION__PHASED__ID,          &mAnimatorLight::EffectAnim__Phased,                      PM_EFFECT_CONFIG__PHASED);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  /**
   * Scan
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  addEffect(EFFECTS_FUNCTION__SCAN__ID,                &mAnimatorLight::EffectAnim__Scan,                    PM_EFFECT_CONFIG__SCAN);
  addEffect(EFFECTS_FUNCTION__DUAL_SCAN__ID,           &mAnimatorLight::EffectAnim__Scan_Dual,               PM_EFFECT_CONFIG__DUAL_SCAN);
  addEffect(EFFECTS_FUNCTION__LARSON_SCANNER__ID,      &mAnimatorLight::EffectAnim__Larson_Scanner,          PM_EFFECT_CONFIG__LARSON_SCANNER);
  addEffect(EFFECTS_FUNCTION__DUAL_LARSON_SCANNER__ID, &mAnimatorLight::EffectAnim__Larson_Scanner_Dual,     PM_EFFECT_CONFIG__DUAL_LARSON_SCANNER);
  addEffect(EFFECTS_FUNCTION__ICU__ID,                 &mAnimatorLight::EffectAnim__ICU,                     PM_EFFECT_CONFIG__ICU);
  addEffect(EFFECTS_FUNCTION__RIPPLE__ID,              &mAnimatorLight::EffectAnim__Ripple,                  PM_EFFECT_CONFIG__RIPPLE);
  addEffect(EFFECTS_FUNCTION__RIPPLE_RAINBOW__ID,      &mAnimatorLight::EffectAnim__Ripple_Rainbow,          PM_EFFECT_CONFIG__RIPPLE_RAINBOW);
  addEffect(EFFECTS_FUNCTION__COMET__ID,               &mAnimatorLight::EffectAnim__Comet,                   PM_EFFECT_CONFIG__COMET);
  addEffect(EFFECTS_FUNCTION__CHUNCHUN__ID,            &mAnimatorLight::EffectAnim__Chunchun,                PM_EFFECT_CONFIG__CHUNCHUN);
  addEffect(EFFECTS_FUNCTION__BOUNCINGBALLS__ID,       &mAnimatorLight::EffectAnim__Bouncing_Balls,          PM_EFFECT_CONFIG__BOUNCINGBALLS);
  addEffect(EFFECTS_FUNCTION__SINELON__ID,             &mAnimatorLight::EffectAnim__Sinelon,                 PM_EFFECT_CONFIG__SINELON);
  addEffect(EFFECTS_FUNCTION__SINELON_DUAL__ID,        &mAnimatorLight::EffectAnim__Sinelon_Dual,            PM_EFFECT_CONFIG__SINELON_DUAL);
  addEffect(EFFECTS_FUNCTION__SINELON_RAINBOW__ID,     &mAnimatorLight::EffectAnim__Sinelon_Rainbow,         PM_EFFECT_CONFIG__SINELON_RAINBOW);
  addEffect(EFFECTS_FUNCTION__DRIP__ID,                &mAnimatorLight::EffectAnim__Drip,                    PM_EFFECT_CONFIG__DRIP);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
  addEffect(EFFECTS_FUNCTION__HARDWARE__SHOW_BUS__ID,                      &mAnimatorLight::EffectAnim__Hardware__Show_Bus,                    PM_EFFECT_CONFIG__HARDWARE__SHOW_BUS);
  addEffect(EFFECTS_FUNCTION__HARDWARE__MANUAL_PIXEL_COUNTING__ID,         &mAnimatorLight::EffectAnim__Hardware__Manual_Pixel_Counting,       PM_EFFECT_CONFIG__HARDWARE__MANUAL_PIXEL_COUNTING);
  addEffect(EFFECTS_FUNCTION__HARDWARE__VIEW_PIXEL_RANGE__ID,              &mAnimatorLight::EffectAnim__Hardware__View_Pixel_Range,            PM_EFFECT_CONFIG__HARDWARE__VIEW_PIXEL_RANGE);
  addEffect(EFFECTS_FUNCTION__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING__ID,   &mAnimatorLight::EffectAnim__Hardware__Light_Sensor_Pixel_Indexing, PM_EFFECT_CONFIG__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING);  
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01__ID,                                     &mAnimatorLight::EffectAnim__SunPositions__Sunrise_Alarm_01,                                               PM_EFFECT_CONFIG__SUNPOSITIONS__SUNRISE_ALARM_01); 
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__ID,               &mAnimatorLight::EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01,                         PM_EFFECT_CONFIG__SUNPOSITIONS__AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01); 
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_SUNSET_BLENDED_PALETTES_01__ID,                           &mAnimatorLight::EffectAnim__SunPositions__Sunset_Blended_Palettes_01,                                     PM_EFFECT_CONFIG__SUNPOSITIONS__SUNSET_BLENDED_PALETTES_01); 
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_01__ID,                              &mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Elevation_01,                                        PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_ELEVATION_01); 
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_01__ID,                                &mAnimatorLight::EffectAnim__SunPositions__DrawSun_1D_Azimuth_01,                                          PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_1D_AZIMUTH_01); 
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__ID,                  &mAnimatorLight::EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01,                            PM_EFFECT_CONFIG__SUNPOSITIONS__DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01); 
  addEffect(EFFECTS_FUNCTION__SUNPOSITIONS_WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__ID,   &mAnimatorLight::EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01,             PM_EFFECT_CONFIG__SUNPOSITIONS__WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01);   
  #endif          
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK  
  addEffect(EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,            &mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_01,                PM_EFFECT_CONFIG__7SEGMENTDISPLAY__CLOCKTIME_01);
  addEffect(EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,            &mAnimatorLight::EffectAnim__7SegmentDisplay__ClockTime_02,                PM_EFFECT_CONFIG__7SEGMENTDISPLAY__CLOCKTIME_02);
  addEffect(EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,  &mAnimatorLight::EffectAnim__7SegmentDisplay__ManualNumber_01,             PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALNUMBER_01); 
  addEffect(EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID,  &mAnimatorLight::EffectAnim__7SegmentDisplay__ManualString_01,             PM_EFFECT_CONFIG__7SEGMENTDISPLAY__MANUALSTRING_01); 
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK     
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS 
  case EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID:
    SubTask_Segment_Animate_Function__Notification_Static();
  break;
  case EFFECTS_FUNCTION__NOTIFICATION_FADE__ID:
    SubTask_Segment_Animate_Function__Notification_Fade();
  break;
  case EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID:
    SubTask_Segment_Animate_Function__Notification_Blinking();
  break;
  case EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID:
    SubTask_Segment_Animate_Function__Notification_Pulsing();
  break;
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  /**
   * BorderWallpaper
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
  addEffect(EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__ID,     &mAnimatorLight::EffectAnim__BorderWallpaper__TwoColour_Gradient,                     PM_EFFECT_CONFIG__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT);
  addEffect(EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__ID,    &mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Gradient,                    PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT);
  addEffect(EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__ID,       &mAnimatorLight::EffectAnim__BorderWallpaper__FourColour_Solid,                       PM_EFFECT_CONFIG__BORDER_WALLPAPER__FOURCOLOUR_SOLID);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
  /**
   * Manual Methods (ie basic or limited effect generation, mostly updates output)
   **/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
  addEffect(EFFECTS_FUNCTION__MANUAL__PIXEL_SET_ELSEWHERE__ID,   &mAnimatorLight::EffectAnim__Manual__PixelSetElsewhere, PM_EFFECT_CONFIG__MANUAL__PIXEL_SET_ELSEWHERE__INDEXING);  
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
  
  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
  void EffectAnim__1D__Aurora();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
  void EffectAnim__1D__PerlinMove();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
  void EffectAnim__1D__Waveins();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
  void EffectAnim__1D__FlowStripe();
  #endif
  /****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *** Specialised: 2D (No Audio) **********************************************************************************************************************************************
  **  Requires:     ***********************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Blackhole();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__ColouredBursts();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__DNA();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__DNASpiral();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Drift();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__FireNoise();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Frizzles();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__GameOfLife();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Hipnotic();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Julia();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Lissajous();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Matrix();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Metaballs();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Noise();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__PlasmaBall();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__PolarLights();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Pulser();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__SinDots();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__SqauredSwirl();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__SunRadiation();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__Tartan();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__SpaceShips();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__CrazyBees();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__GhostRider();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__FloatingBlobs();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D_TODO
  void EffectAnim__2D__DriftRose();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__DISTORTION_WAVES__ID,   &mAnimatorLight::EffectAnim__2D__DistortionWaves, PM_EFFECT_CONFIG__2D__DISTORTION_WAVES__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__SOAP__ID,   &mAnimatorLight::EffectAnim__2D__Soap, PM_EFFECT_CONFIG__2D__SOAP__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__OCTOPUS__ID,   &mAnimatorLight::EffectAnim__2D__Octopus, PM_EFFECT_CONFIG__2D__OCTOPUS__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__WAVING_CELL__ID,   &mAnimatorLight::EffectAnim__2D__WavingCell, PM_EFFECT_CONFIG__2D__WAVING_CELL__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__SCROLLING_TEXT__ID,   &mAnimatorLight::EffectAnim__2D__ScrollingText, PM_EFFECT_CONFIG__2D__SCROLLING_TEXT__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__DIGITAL_CLOCK__ID,   &mAnimatorLight::EffectAnim__2D__DigitalClock, PM_EFFECT_CONFIG__2D__DIGITAL_CLOCK__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
  addEffect(EFFECTS_FUNCTION__2D__DNA__ID,   &mAnimatorLight::EffectAnim__2D__DNA, PM_EFFECT_CONFIG__2D__DNA__INDEXING);  
  #endif
  /****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *** Specialised: 1D (Audio Reactive) ****************************************************************************************************************************************
  **  Requires:     ***********************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__Ripple_Peak();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__GravCenter();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__GravCentric();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__GraviMeter();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__Juggles();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__Matripix();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__MidNoise();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__NoiseFire();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__NoiseMeter();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__PixelWave();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__Plasmoid();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__PuddlePeak();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__Puddles();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__Pixels();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_Blurz();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_DJLight();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_FreqMap();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_FreqMatrix();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_FreqPixels();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_FreqWave();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_GravFreq();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_NoiseMove();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D_TODO
  void EffectAnim__AudioReactive__1D__FFT_RockTaves();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_WATERFALL__ID,   &mAnimatorLight::EffectAnim__AudioReactive__1D__FFT_Waterfall, PM_EFFECT_CONFIG__AUDIOREACTIVE__1D__FFT_WATERFALL); 
  #endif
  /****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *** Specialised: 2D (Audio Reactive) ************************************************************************************************************************************************
  **  Requires:     ***********************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************
  *****************************************************************************************************************************************************************************/
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__SWIRL__ID,   &mAnimatorLight::EffectAnim__AudioReactive__2D__Swirl, PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__SWIRL__INDEXING); 
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__WAVERLY__ID,   &mAnimatorLight::EffectAnim__AudioReactive__2D__Waverly, PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__WAVERLY__INDEXING); 
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_GED__ID,   &mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_GED, PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_GED__INDEXING); 
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__ID,   &mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_FunkyPlank, PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__INDEXING);  
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  addEffect(EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_AKEMI__ID,   &mAnimatorLight::EffectAnim__AudioReactive__2D__FFT_Akemi, PM_EFFECT_CONFIG__AUDIOREACTIVE__2D__FFT_AKEMI__INDEXING);  
  #endif


/**
 * @brief Effect X: New effect of "Static Palette Mix"
 * 
 * The aim is to make the "Colour and Warm White Stripe" but in a way I can mix them. 
 * 
 * Palette1 ABCD
 * Palette2 abcde
 * 
 * So mix would be AaBbCcDdAe (notice shorter palettes that mix already)
 * 
 */










  /**
   * Development effects without full code 
   **/   
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
  addEffect(EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01_ID,                       &mAnimatorLight::EffectAnim__Christmas_Musical__01,                 PM_EFFECT_CONFIG__CHRISTMAS_MUSICAL_01);
  #endif 
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
  case EFFECTS_FUNCTION__TESTER_01__ID:
    SubTask_Flasher_Animate_Function_Tester_01();
  break; 
  case EFFECTS_FUNCTION__TESTER_02__ID:
    SubTask_Flasher_Animate_Function_Tester_02();
  break; 
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
}


#endif //USE_MODULE_LIGHTS_ANIMATOR




/***
 * 
 * 
 * Conversion mapping for WLED to mine
 * 
 * 
 * 
 * 
 WLED                                  ------->                              PulSar


SEGENV                                                                       SEGMENT
isMatrix                                                               isMatrix
color_from_palette                                                           c
SEGMENT.params_internal.aux1                                                                 SEGMENT.params_internal.aux1
SEGCOLOR                                    SEGCOLOR_U32                                  
SEGMENT.color_from_palette((band * 35), false, PALETTE_SOLID_WRAP, 0);                       SEGMENT.GetPaletteColour((band * 35), WLED_PALETTE_MAPPING_ARG_FALSE, PALETTE_WRAP_ON, PALETTE_DISCRETE_OFF, NO_ENCODED_VALUE).getU32()      

#define color_from_palette(a,b,c,d)    GetPaletteColour(a,b,c,d).getU32()

 * 
 * 
 * 
*/
