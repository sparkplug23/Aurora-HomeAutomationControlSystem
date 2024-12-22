

/**
 * @file mAnimatorLight_Mixer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * 
 * Things to do:
 * 
 * ** Openhab commands to control sequencer
 *      * 0: Stop and return to static
 *      * 1: Start sequence 1 - no timed
 *      * 2: Start sequence 2 - timed so more flashing
 *      * 3: Start sequence 3 - demo mode, so everything in 120 seconds
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

#include "6_Lights/03_Animator/mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER 


#ifdef ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE_2023

void mAnimatorLight::SubLoad_Sequencer_Device(uint8_t id)
{

  sequencer_item_list.clear(); // clear all sequences
  memset(&sequencer_runtime, 0, sizeof(sequencer_runtime)); // clear all sequences

  struct SEQUENCER_ITEM seq_t;
  

  /*********************************************************************************************************************************************************************************************************************************
   * ***************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************************
   * **** NIGHT *****************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  






  if(id == 100)  /////////////////////////////////////////////////////////////////////////////////////////////// 4-ish minute, version 2
  {

    ALOG_INF(PSTR("Sequence: Designed to be the usual effects, with animation within 4 minutes v2"));

    // 1 - Static 
    seq_t.seconds_on = 15;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 4 - Fast glows as "pops"
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Slow Glow",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":10
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 7
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1500,
            "RateMs": 2000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // // 7
    // seq_t.seconds_on = 10; // Quickly cycle throw colours, with reduced brightness to stop it strobbing
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":"Christmas Snowy 01",
    //       "Effects": {
    //         "Function":"Step Palette",
    //         "Intensity":20,
    //         "Speed":255,
    //         "Grouping":1,
    //         "Decimate":0
    //       },
    //       "Transition": {
    //         "TimeMs": 900,
    //         "RateMs": 1000
    //       },
    //       "BrightnessRGB": 100
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);


    // 8
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":173,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 2500,
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":173,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    
    // 18
    seq_t.seconds_on = 40;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Randomise 01",
          "Effects": {
            "Function":"Spanned Palette",
            "Intensity":255,
            "Speed":126,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1900,
            "RateMs": 2000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 11 - Sweep fast
    seq_t.seconds_on = 30;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Sweep Random, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Sweep Random",
            "Intensity":255,
            "Speed":230,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 4pm - 8pm == Normal flashy

    // 5 - White twinkles
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":3,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":93
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 95
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    
    // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Cold White",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":125
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 100
          },
          "BrightnessRGB": 20
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    // seq_t.seconds_on = 20;
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":0,
    //       "SegColour0":{
    //         "Hue":120,
    //         "Sat":100,
    //         "BrightnessRGB":0
    //       },
    //       "Effects": {
    //         "Function":"Twinkle Palette Two on One",
    //         "Intensity":20,
    //         "Speed":255,
    //         "Grouping":1,
    //         "Decimate":0,
    //         "Param0":125
    //       },
    //       "Transition": {
    //         "TimeMs": 0,
    //         "RateMs": 100
    //       },
    //       "BrightnessRGB": 30
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);

    // 2 - shimmer
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Shimmer, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Shimmer",
            "Intensity":255,
            "Speed":200,
            "Grouping":1,
            "Decimate":40
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


  }else


 /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Normal V3 with rotation faster *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
 

  if(id == 101)  /////////////////////////////////////////////////////////////////////////////////////////////// 4-ish minute, version 3:
  {

    ALOG_INF(PSTR("Sequence: Designed to be the usual effects, with animation within 4 minutes v2"));

    // 1 - Static 
    seq_t.seconds_on = 15;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 4 - Fast glows as "pops"
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Slow Glow",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":10
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 7
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1500,
            "RateMs": 2000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // // 7
    // seq_t.seconds_on = 10; // Quickly cycle throw colours, with reduced brightness to stop it strobbing
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":"Christmas Snowy 01",
    //       "Effects": {
    //         "Function":"Step Palette",
    //         "Intensity":20,
    //         "Speed":255,
    //         "Grouping":1,
    //         "Decimate":0
    //       },
    //       "Transition": {
    //         "TimeMs": 900,
    //         "RateMs": 1000
    //       },
    //       "BrightnessRGB": 100
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);


    // 8
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":173,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 2500,
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":173,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":2
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":7
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    
    // 18
    seq_t.seconds_on = 40;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Randomise 01",
          "Effects": {
            "Function":"Spanned Palette",
            "Intensity":255,
            "Speed":126,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1900,
            "RateMs": 2000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // // 11 - Sweep fast
    // seq_t.seconds_on = 30;
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 19, 0, 0};
    // seq_t.description = "Sweep Random, Snowy 02, 0/25";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":"Christmas Snowy 02",
    //       "Effects": {
    //         "Function":"Sweep Random",
    //         "Intensity":255,
    //         "Speed":230,
    //         "Grouping":1,
    //         "Decimate":0
    //       },
    //       "Transition": {
    //         "TimeMs": 0,
    //         "RateMs": 25
    //       },
    //       "BrightnessRGB": 100
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);

    // 4pm - 8pm == Normal flashy

    // 5 - White twinkles
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":3,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":93
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 95
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    
    // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Cold White",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":125
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 100
          },
          "BrightnessRGB": 20
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    // seq_t.seconds_on = 20;
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":0,
    //       "SegColour0":{
    //         "Hue":120,
    //         "Sat":100,
    //         "BrightnessRGB":0
    //       },
    //       "Effects": {
    //         "Function":"Twinkle Palette Two on One",
    //         "Intensity":20,
    //         "Speed":255,
    //         "Grouping":1,
    //         "Decimate":0,
    //         "Param0":125
    //       },
    //       "Transition": {
    //         "TimeMs": 0,
    //         "RateMs": 100
    //       },
    //       "BrightnessRGB": 30
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);

    // 2 - shimmer
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Shimmer, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Shimmer",
            "Intensity":255,
            "Speed":200,
            "Grouping":1,
            "Decimate":40
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


  }else

  /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Night, Palette Spanned Blend *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  if(id == 102) 
  {

    ALOG_INF(PSTR("Sequence: Night 40"));

    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Spanned Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 01"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


  }else

  
  /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Night, Static Palette Blend with Grouping *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  if(id == 103) 
  {

    ALOG_INF(PSTR("Sequence: Night 41"));

    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":150,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 3900,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 01"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    
    
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Berry Orange"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    

    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Colours Warm"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Vintage"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"IceCream Floats"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"IceCream Floats+"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Pink Red Yellow"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Greenless"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


  }else




  /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Night Colour+White Strip *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  if(id == 104) 
  {

    ALOG_INF(PSTR("Sequence: Night, Colour/White 136"));

    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colour/Bright White Stripe",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 9
    seq_t.seconds_on = 2; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colour/Bright White Stripe",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":136,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":5
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // // 10
    // seq_t.seconds_on = 20;
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "Effects": {
    //         "Intensity":11
    //       }
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);




  }else

 /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Normal V3 with rotation faster *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
 

  if(id == 105)  /////////////////////////////////////////////////////////////////////////////////////////////// 4-ish minute, version 3:
  {

    ALOG_INF(PSTR("Sequence: Reorder to fix flash after sweep was removed"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 15;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 4 - Fast glows as "pops"
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Slow Glow",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":10
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 7
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1500,
            "RateMs": 2000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // // 7
    // seq_t.seconds_on = 10; // Quickly cycle throw colours, with reduced brightness to stop it strobbing
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":"Christmas Snowy 01",
    //       "Effects": {
    //         "Function":"Step Palette",
    //         "Intensity":20,
    //         "Speed":255,
    //         "Grouping":1,
    //         "Decimate":0
    //       },
    //       "Transition": {
    //         "TimeMs": 900,
    //         "RateMs": 1000
    //       },
    //       "BrightnessRGB": 100
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);


    // 8
    seq_t.seconds_on = 60;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":173,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 2500,
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":173,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 3000,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":2
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":7
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



    // 2 - shimmer
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Shimmer, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Shimmer",
            "Intensity":255,
            "Speed":200,
            "Grouping":1,
            "Decimate":40
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);




    
    // 18
    seq_t.seconds_on = 40;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Randomise 01",
          "Effects": {
            "Function":"Spanned Palette",
            "Intensity":255,
            "Speed":126,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1900,
            "RateMs": 2000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // // 11 - Sweep fast
    // seq_t.seconds_on = 30;
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 19, 0, 0};
    // seq_t.description = "Sweep Random, Snowy 02, 0/25";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":"Christmas Snowy 02",
    //       "Effects": {
    //         "Function":"Sweep Random",
    //         "Intensity":255,
    //         "Speed":230,
    //         "Grouping":1,
    //         "Decimate":0
    //       },
    //       "Transition": {
    //         "TimeMs": 0,
    //         "RateMs": 25
    //       },
    //       "BrightnessRGB": 100
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);

    // 4pm - 8pm == Normal flashy

    // 5 - White twinkles
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":3,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":93
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 50
          },
          "BrightnessRGB": 95
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    
    // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Cold White",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":125
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 100
          },
          "BrightnessRGB": 20
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    // seq_t.seconds_on = 20;
    // seq_t.time_enabled.isArmed = true;
    // seq_t.time_enabled.start = {0, 16, 0, 0};
    // seq_t.time_enabled.end   = {0, 21, 0, 0};
    // seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":0,
    //       "SegColour0":{
    //         "Hue":120,
    //         "Sat":100,
    //         "BrightnessRGB":0
    //       },
    //       "Effects": {
    //         "Function":"Twinkle Palette Two on One",
    //         "Intensity":20,
    //         "Speed":255,
    //         "Grouping":1,
    //         "Decimate":0,
    //         "Param0":125
    //       },
    //       "Transition": {
    //         "TimeMs": 0,
    //         "RateMs": 100
    //       },
    //       "BrightnessRGB": 30
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);


  }else


  if(id == 106)  /////////////////////////////////////////////////////////////////////////////////////////////// WLED Effects, flashy
  {

    ALOG_INF(PSTR("Sequence: WLED Flashy Night"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Party 16",
          "Effects": {
            "Function":"Twinkle FastIn/SlowOut",
            "Intensity":255,
            "Speed":213,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 4 - Fast glows as "pops"
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 01",
          "Effects": {
            "Function":"Running 2",
            "Intensity":255,
            "Speed":64,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour1":{
            "Manual":[53,49,53,0,0],
            "BrightnessRGB":20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // Tri wipe, colour + cold white + warm white
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 01",
          "Effects": {
            "Function":"Tri Wipe",
            "Intensity":255,
            "Speed":151,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour0":{
            "Manual":[130,137,142,0,0],
            "BrightnessRGB":70
          },
          "SegColour1":{
            "Manual":[130,86,25,0,0],
            "BrightnessRGB":70
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // Tri wipe, colour + red + green
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 01",
          "Effects": {
            "Function":"Tri Wipe",
            "Intensity":255,
            "Speed":151,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour0":{
            "Manual":[255,0,0,0,0],
            "BrightnessRGB":100
          },
          "SegColour1":{
            "Manual":[0,255,0,0,0],
            "BrightnessRGB":100
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // Tri wipe, colour + red + green
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"IceCream Floats",
          "Effects": {
            "Function":"Tri Wipe",
            "Intensity":255,
            "Speed":151,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour0":{
            "Manual":[255,0,0,0,0],
            "BrightnessRGB":100
          },
          "SegColour1":{
            "Manual":[0,255,0,0,0],
            "BrightnessRGB":100
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);




  }else




  if(id == 107)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Flashy Night"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Rainbow 16",
          "Effects": {
            "Function":"Twinkle FastIn/SlowOut",
            "Intensity":255,
            "Speed":213,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Party 16",
          "Effects": {
            "Function":"Twinkle FastIn/SlowOut",
            "Intensity":255,
            "Speed":213,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 1 - Static 
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Party 16",
          "Effects": {
            "Function":"Twinkle FastIn/SlowOut",
            "Intensity":255,
            "Speed":255,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // Tri wipe, colour + cold white + warm white
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 19, 0, 0};
    seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 01",
          "Effects": {
            "Function":"Tri Wipe",
            "Intensity":255,
            "Speed":151,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour0":{
            "Manual":[130,137,142,0,0],
            "BrightnessRGB":70
          },
          "SegColour1":{
            "Manual":[130,86,25,0,0],
            "BrightnessRGB":70
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    
    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":150,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 3900,
            "RateMs": 4000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 01"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    
    
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Berry Orange"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);
    

    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Colours Warm"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"IceCream Floats"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"IceCream Floats+"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Pink Red Yellow"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Greenless"
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else



  if(id == 108)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Flashy Night"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Party 16",
          "Effects": {
            "Function":"Twinkle FastIn/SlowOut",
            "Intensity":255,
            "Speed":213,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else
  if(id == 109)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Flashy Night"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Rainbow 16",
          "Effects": {
            "Function":"Twinkle FastIn/SlowOut",
            "Intensity":255,
            "Speed":213,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else

  if(id == 110)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Saw Snowy 01"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 01",
          "Effects": {
            "Function":"Saw",
            "Intensity":134,
            "Speed":64,
            "Grouping":1
          },
          "SegColour0":{
            "Manual":[40,40,40,0,0]
          },
          "SegColour1":{
            "Manual":[40,40,40,0,0]
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else
  if(id == 111)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Rainbow Runner"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Rainbow 16",
          "Effects": {
            "Function":"Rainbow Runner",
            "Intensity":133,
            "Speed":64,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else
  if(id == 112)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Meteor Smooth ORange/Teal"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Orange & Teal",
          "Effects": {
            "Function":"Meteor Smooth",
            "Intensity":142,
            "Speed":70,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else

  if(id == 113)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Meteor ORange/Teal"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Orange & Teal",
          "Effects": {
            "Function":"Meteor",
            "Intensity":188,
            "Speed":55,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else
  if(id == 114)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Meteor 5 CRGB16 Repeat manual"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"*Five Colours Repeated",
          "Effects": {
            "Function":"Meteor",
            "Intensity":238,
            "Speed":55,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour0":{
            "Manual":[255,17,0,0,0]
          },
          "SegColour1":{
            "Manual":[0,255,0,0,0]
          },
          "SegColour2":{
            "Manual":[0,255,255,0,0]
          },
          "SegColour3":{
            "Manual":[255,81,0,0,0]
          },
          "SegColour4":{
            "Manual":[255,0,98,0,0]
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else
  if(id == 115)  /////////////////////////////////////////////////////////////////////////////////////////////// Twinkle WLED with some palette spanned
  {

    ALOG_INF(PSTR("Sequence: WLED Static 5 CRGB16 Repeat manual"));

    /**
     * @brief 
     * Ie make sure my effect happens so the getpixel gets the last state and only set shimmer after a bright state
     * 
     */

    // 1 - Static 
    seq_t.seconds_on = 120;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.limit.flash_level = 0;
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"*Five Colours Repeated",
          "Effects": {
            "Function":"Static",
            "Intensity":188,
            "Speed":55,
            "Grouping":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour0":{
            "Manual":[255,17,0,0,0]
          },
          "SegColour1":{
            "Manual":[0,255,0,0,0]
          },
          "SegColour2":{
            "Manual":[0,255,255,0,0]
          },
          "SegColour3":{
            "Manual":[255,81,0,0,0]
          },
          "SegColour4":{
            "Manual":[255,0,98,0,0]
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }else



  /*********************************************************************************************************************************************************************************************************************************
   * ***************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************************
   * **** Day *****************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  



  /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Day time blue/white patterns *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  if(id == 120) 
  {

    ALOG_INF(PSTR("Sequence: Daytime, blue/white for brightness"));

    // 9
    seq_t.seconds_on = 2; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Blue and White",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":190,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Blue and White",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":5
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":11
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);




  }else





  
  /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** Day2 Colour+White Strip *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  if(id == 121) 
  {

    ALOG_INF(PSTR("Sequence: Daytime, Colour/Bright White Stripe"));

    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colour/Bright White Stripe",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 9
    seq_t.seconds_on = 2; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colour/Bright White Stripe",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":190,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":5
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 20;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":11
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


  }else



  /*********************************************************************************************************************************************************************************************************************************
   * ***************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************************
   * **** DEMO, Fast 1 minute show off *****************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * ************************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  
  if(id == 140)
  {
    
    // 1
    seq_t.seconds_on = 30;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Wipe Random",
            "Intensity":255,
            "Speed":100,
            "Grouping":1,
            "Decimate":0,
            "Reverse":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
      )=====";
    sequencer_item_list.push_back(seq_t);

    // 1
    seq_t.seconds_on = 30;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Wipe Random",
            "Intensity":255,
            "Speed":180,
            "Grouping":1,
            "Decimate":0,
            "Reverse":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
      )=====";
    sequencer_item_list.push_back(seq_t);

    // 1
    seq_t.seconds_on = 30;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Wipe Random",
            "Intensity":255,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Reverse":1
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
      )=====";
    sequencer_item_list.push_back(seq_t);


    ALOG_INF(PSTR("Loaded Sequencer 2"));
  }

  else



  
  // Test Mode - New ones for easy testing
  if(id == 141)
  {
    
    // // 6 - starburst rainbow over white
    // seq_t.seconds_on = 10;
    // seq_t.description = "Fireworks Starburst, Rainbow 16, 0/23";
    // seq_t.json_command = 
    //   R"=====(
    //     {
    //       "ColourPalette":"Rainbow 16",
    //       "Effects": {
    //         "Function":"Fireworks Starburst",
    //         "Intensity":255,
    //         "Speed":255,
    //         "Grouping":20,
    //         "Decimate":0
    //       },
    //       "Transition": {
    //         "TimeMs": 0,
    //         "RateMs": 23
    //       },
    //       "SegColour1":{
    //         "Hue":20,
    //         "Sat":0,
    //         "BrightnessRGB":15
    //       },
    //       "BrightnessRGB": 100
    //     }
    //     )=====";
    // sequencer_item_list.push_back(seq_t);

    
    // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    seq_t.seconds_on = 10;
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Cold White",
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":125
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 100
          },
          "BrightnessRGB": 20
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
    seq_t.seconds_on = 10;
    seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":0,
          "SegColour0":{
            "Hue":120,
            "Sat":100,
            "BrightnessRGB":0
          },
          "Effects": {
            "Function":"Twinkle Palette Two on One",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0,
            "Param0":125
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 100
          },
          "BrightnessRGB": 30
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



    ALOG_INF(PSTR("Loaded Sequencer 2"));
  }


  else
  // Test Mode - Time testing
  if(id == 141)
  {    
    
    seq_t.seconds_on = 5;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 12, 55, 0};
    seq_t.time_enabled.end = {0, 13, 0, 0};
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 1000
          },
          "BrightnessRGB": 1
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    //
    seq_t.seconds_on = 5;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 12, 56, 0};
    seq_t.time_enabled.end = {0, 13, 10, 0};
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Blue and White",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 1000
          },
          "BrightnessRGB": 1
        }
        )=====";

    sequencer_item_list.push_back(seq_t);


    
    seq_t.seconds_on = 5;
    seq_t.time_enabled.isArmed = false;
    seq_t.time_enabled.start = {0, 13, 10, 0};
    seq_t.time_enabled.end = {0, 13, 20, 0};
    seq_t.description = "Static, Snowy 02, 0/1000";
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":127,
            "Speed":127,
            "Grouping":1,
            "Decimate":0,
            "Reverse":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 1000
          },
          "BrightnessRGB": 1
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



    ALOG_INF(PSTR("Loaded Sequencer 2"));
  }
  
  /*********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * **** *****************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************
   * *********************************************************************************************************************************************************************************************************************************  */
  else
  if(id == 142) 
  {

    ALOG_INF(PSTR("Sequence: For videos to show people, rotations"));

    // 9
    seq_t.seconds_on = 2; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Blue and White",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":190,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Blue and White",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":5
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":11
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 9
    seq_t.seconds_on = 2; //short, just preloading the rotation next
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Static Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":190,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 900,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":0,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":5
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 10
    seq_t.seconds_on = 10;
    seq_t.time_enabled.isArmed = true;
    seq_t.time_enabled.start = {0, 16, 0, 0};
    seq_t.time_enabled.end   = {0, 21, 0, 0};
    seq_t.description = "Rotating Previous, Snowy 02, 50/100";
    seq_t.json_command = 
      R"=====(
        {
          "Effects": {
            "Intensity":11
          }
        }
        )=====";
    sequencer_item_list.push_back(seq_t);



  }

  sequencer_runtime.seconds_remaining_on_item = 0; // Load now

  mqtthandler_automation_sequencer.flags.SendNow = true;


  sequencer_runtime.loaded_sequence_set = id;

}

#endif // ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE_2023

#endif // ENABLE_FEATURE_LIGHTING__SEQUENCER

#endif //USE_MODULE_LIGHTS_ANIMATOR






// /**
//  * @file mAnimatorLight_Mixer.cpp
//  * @author your name (you@domain.com)
//  * @brief 
//  * @version 0.1
//  * @date 2023-11-13
//  * 
//  * @copyright Copyright (c) 2023
//  * 
//  * 
//  * 
//  * Things to do:
//  * 
//  * ** Openhab commands to control sequencer
//  *      * 0: Stop and return to static
//  *      * 1: Start sequence 1 - no timed
//  *      * 2: Start sequence 2 - timed so more flashing
//  *      * 3: Start sequence 3 - demo mode, so everything in 120 seconds
//  * 
//  * 
//  * 
//  * 
//  * 
//  * 
//  * 
//  */

// #include "mAnimatorLight.h"

// #ifdef USE_MODULE_LIGHTS_ANIMATOR

// #ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER 


// #ifdef ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE

// void mAnimatorLight::SubLoad_Sequencer_Device(uint8_t id)
// {

//   sequencer_item_list.clear(); // clear all sequences
//   memset(&sequencer_runtime, 0, sizeof(sequencer_runtime)); // clear all sequences

//   struct SEQUENCER_ITEM seq_t;
  
//   #ifdef ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS
//   if(id == 10)
//   {

//     // 1 - Static 
//     seq_t.seconds_on = 60;
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 4 - Fast glows as "pops"
//     seq_t.seconds_on = 60;
//     seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Slow Glow",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":10
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 2 - shimmer
//     seq_t.seconds_on = 60;
//     seq_t.description = "Shimmer, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Shimmer",
//             "Intensity":255,
//             "Speed":200,
//             "Grouping":1,
//             "Decimate":40
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 5 - White twinkles
//     seq_t.seconds_on = 60;
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":3,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":93
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 7
//     seq_t.seconds_on = 60;
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 8
//     seq_t.seconds_on = 60;
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":200,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 9
//     seq_t.seconds_on = 15; //short, just preloading the rotation next
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":200,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 60;
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

      
//     // 18
//     seq_t.seconds_on = 60;
//     seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Randomise 01",
//           "Effects": {
//             "Function":"Spanned Palette",
//             "Intensity":255,
//             "Speed":126,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 5000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 11 - Sweep slow
//     seq_t.seconds_on = 60;
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":160,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 11 - Sweep fast
//     seq_t.seconds_on = 20;
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":225,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     ALOG_INF(PSTR("Loaded Sequencer 1 - Base version, for most of the time"));

//   }else
//   if(id == 11)  /////////////////////////////////////////////////////////////////////////////////////////////// Timed Normal
//   {

//     // 1 - Static 
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 4 - Fast glows as "pops"
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Slow Glow",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":10
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 2 - shimmer
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Shimmer, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Shimmer",
//             "Intensity":255,
//             "Speed":200,
//             "Grouping":1,
//             "Decimate":40
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);



//     // 5 - White twinkles
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":3,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":93
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 7
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 2500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 8
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":200,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":173,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

    
//     // 18
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Randomise 01",
//           "Effects": {
//             "Function":"Spanned Palette",
//             "Intensity":255,
//             "Speed":126,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 5000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 11 - Sweep slow
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":160,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 80
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 11 - Sweep fast
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":225,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 80
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 4pm - 8pm == Normal flashy

    
//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Cold White",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 20
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":0,
//           "SegColour0":{
//             "Hue":120,
//             "Sat":100,
//             "BrightnessRGB":0
//           },
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 30
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 4pm to 6pm == Very flashy

//     // // 12
//     // seq_t.seconds_on = 60;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 18, 0, 0};
//     // seq_t.description = "Fireworks Starburst, Rainbow 16, 0/23";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Christmas Snowy 02",
//     //       "Effects": {
//     //         "Function":"Fireworks 1D",
//     //         "Intensity":255,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 25
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);


//     // // 13
//     // seq_t.seconds_on = 60;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 18, 0, 0};
//     // seq_t.description = "Fireworks Starburst, Rainbow 16, 0/23";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Christmas Snowy 02",
//     //       "Effects": {
//     //         "Function":"Fireworks 1D No Launch",
//     //         "Intensity":255,
//     //         "Speed":126,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 25
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);


//     // // 6 - starburst rainbow over white
//     // seq_t.seconds_on = 60;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 18, 0, 0};
//     // seq_t.description = "Fireworks Starburst, Rainbow 16, 0/23";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Rainbow 16",
//     //       "Effects": {
//     //         "Function":"Fireworks Starburst",
//     //         "Intensity":255,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 23
//     //       },
//     //       "SegColour1":{
//     //         "Hue":20,
//     //         "Sat":0,
//     //         "BrightnessRGB":30
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);

    
//     // // Last - Turn off automatically, set to unloaded and off state
//     // seq_t.seconds_on = 5;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 21, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 10, 0};
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "Sequencer":{"LoadSet":0},
//     //       "ColourPalette":"Christmas Snowy 02",
//     //       "Effects": {
//     //         "Function":"Static Palette",
//     //         "Intensity":127,
//     //         "Speed":127,
//     //         "Grouping":1,
//     //         "Decimate":0,
//     //         "Reverse":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 1000
//     //       },
//     //       "BrightnessRGB": 0
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);


//     ALOG_INF(PSTR("Loaded Sequencer 1"));

//   }else
//   if(id == 12)  /////////////////////////////////////////////////////////////////////////////////////////////// Timed Shorter - for normal that someone can see it all reasonably quick
//   {

//     ALOG_INF(PSTR("Sequence: Designed to be the usual effects, with animation within 4 minutes"));

//     // 1 - Static 
//     seq_t.seconds_on = 15;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 4 - Fast glows as "pops"
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Slow Glow",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":10
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 5 - White twinkles
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":3,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":93
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 95
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 7
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 2000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 7
//     seq_t.seconds_on = 10; // Quickly cycle throw colours, with reduced brightness to stop it strobbing
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 8
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":200,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 2500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":173,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

    
//     // 18
//     seq_t.seconds_on = 40;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Randomise 01",
//           "Effects": {
//             "Function":"Spanned Palette",
//             "Intensity":255,
//             "Speed":126,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 2000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // // 11 - Sweep slow
//     // seq_t.seconds_on = 30;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Christmas Snowy 02",
//     //       "Effects": {
//     //         "Function":"Sweep Random",
//     //         "Intensity":255,
//     //         "Speed":160,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 25
//     //       },
//     //       "BrightnessRGB": 80
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);

//     // 11 - Sweep fast
//     seq_t.seconds_on = 30;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":230,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 4pm - 8pm == Normal flashy

    
//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Cold White",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 20
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":0,
//           "SegColour0":{
//             "Hue":120,
//             "Sat":100,
//             "BrightnessRGB":0
//           },
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 30
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 2 - shimmer
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Shimmer, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Shimmer",
//             "Intensity":255,
//             "Speed":200,
//             "Grouping":1,
//             "Decimate":40
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);



//   }else
//   if(id == 13)  /////////////////////////////////////////////////////////////////////////////////////////////// 4-ish minute, version 2
//   {

//     ALOG_INF(PSTR("Sequence: Designed to be the usual effects, with animation within 4 minutes v2"));

//     // 1 - Static 
//     seq_t.seconds_on = 15;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 4 - Fast glows as "pops"
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Slow Glow",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":10
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 7
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 2000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // // 7
//     // seq_t.seconds_on = 10; // Quickly cycle throw colours, with reduced brightness to stop it strobbing
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Christmas Snowy 01",
//     //       "Effects": {
//     //         "Function":"Step Palette",
//     //         "Intensity":20,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 900,
//     //         "RateMs": 1000
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);


//     // 8
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":173,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 2500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":173,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

    
//     // 18
//     seq_t.seconds_on = 40;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Randomise 01",
//           "Effects": {
//             "Function":"Spanned Palette",
//             "Intensity":255,
//             "Speed":126,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1900,
//             "RateMs": 2000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 11 - Sweep fast
//     seq_t.seconds_on = 30;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":230,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 4pm - 8pm == Normal flashy

//     // 5 - White twinkles
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":3,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":93
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 95
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);
    
//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Cold White",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 20
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     // seq_t.seconds_on = 20;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":0,
//     //       "SegColour0":{
//     //         "Hue":120,
//     //         "Sat":100,
//     //         "BrightnessRGB":0
//     //       },
//     //       "Effects": {
//     //         "Function":"Twinkle Palette Two on One",
//     //         "Intensity":20,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0,
//     //         "Param0":125
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 100
//     //       },
//     //       "BrightnessRGB": 30
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);

//     // 2 - shimmer
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Shimmer, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Shimmer",
//             "Intensity":255,
//             "Speed":200,
//             "Grouping":1,
//             "Decimate":40
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//   }else

//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Early morning low brightness *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 14) 
//   {

//     ALOG_INF(PSTR("Sequence: Morning, low brightness"));

//     // 1 - Static 
//     seq_t.seconds_on = 120;
//     seq_t.time_enabled.isArmed = false;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 5
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);




//   }else
//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Day time blue/white patterns *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 15) 
//   {

//     ALOG_INF(PSTR("Sequence: Daytime, blue/white for brightness"));

//     // 9
//     seq_t.seconds_on = 2; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Blue and White",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":190,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Blue and White",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":5
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":11
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);




//   }else





//  /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Normal V3 with rotation faster *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
 

//   if(id == 16)  /////////////////////////////////////////////////////////////////////////////////////////////// 4-ish minute, version 3:
//   {

//     ALOG_INF(PSTR("Sequence: Designed to be the usual effects, with animation within 4 minutes v2"));

//     // 1 - Static 
//     seq_t.seconds_on = 15;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.limit.flash_level = 0;
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 4 - Fast glows as "pops"
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Slow Glow Fast Pops, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Slow Glow",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":10
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 7
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1500,
//             "RateMs": 2000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // // 7
//     // seq_t.seconds_on = 10; // Quickly cycle throw colours, with reduced brightness to stop it strobbing
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Christmas Snowy 01",
//     //       "Effects": {
//     //         "Function":"Step Palette",
//     //         "Intensity":20,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 900,
//     //         "RateMs": 1000
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);


//     // 8
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Step Palette, Colourful Pairs, 1500/3000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Pairs 01",
//           "Effects": {
//             "Function":"Step Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":173,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 2500,
//             "RateMs": 3000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":173,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3000,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":2
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":7
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

    
//     // 18
//     seq_t.seconds_on = 40;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Spanned Palette, Randomise 01, 3000/5000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Randomise 01",
//           "Effects": {
//             "Function":"Spanned Palette",
//             "Intensity":255,
//             "Speed":126,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 1900,
//             "RateMs": 2000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 11 - Sweep fast
//     seq_t.seconds_on = 30;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Sweep Random, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Sweep Random",
//             "Intensity":255,
//             "Speed":230,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 4pm - 8pm == Normal flashy

//     // 5 - White twinkles
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 19, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":3,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":93
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 50
//           },
//           "BrightnessRGB": 95
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);
    
//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Cold White",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 20
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     // seq_t.seconds_on = 20;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":0,
//     //       "SegColour0":{
//     //         "Hue":120,
//     //         "Sat":100,
//     //         "BrightnessRGB":0
//     //       },
//     //       "Effects": {
//     //         "Function":"Twinkle Palette Two on One",
//     //         "Intensity":20,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0,
//     //         "Param0":125
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 100
//     //       },
//     //       "BrightnessRGB": 30
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);

//     // 2 - shimmer
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Shimmer, Snowy 02, 0/25";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Shimmer",
//             "Intensity":255,
//             "Speed":200,
//             "Grouping":1,
//             "Decimate":40
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//   }else

  
//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Day2 Colour+White Strip *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 17) 
//   {

//     ALOG_INF(PSTR("Sequence: Daytime, Colour/Bright White Stripe"));

//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colour/Bright White Stripe",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 9
//     seq_t.seconds_on = 2; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colour/Bright White Stripe",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":190,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":5
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":11
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);




//   }else


//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Night, Palette Spanned Blend *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 40) 
//   {

//     ALOG_INF(PSTR("Sequence: Night 40"));

//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Spanned Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 01"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//   }else

  
//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Night, Static Palette Blend with Grouping *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 41) 
//   {

//     ALOG_INF(PSTR("Sequence: Night 41"));

//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":150,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 3900,
//             "RateMs": 4000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 01"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);
    
    
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Berry Orange"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);
    

//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Colours Warm"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Vintage"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"IceCream Floats"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"IceCream Floats+"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Pink Red Yellow"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colourful Greenless"
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//   }else

//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Night Colour+White Strip *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 18) 
//   {

//     ALOG_INF(PSTR("Sequence: Night, Colour/White 160"));

//     // 9
//     // seq_t.seconds_on = 10; //short, just preloading the rotation next
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Colour/Bright White Stripe",
//     //       "Effects": {
//     //         "Function":"Static Palette",
//     //         "Intensity":20,
//     //         "Speed":255,
//     //         "Grouping":1,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 900,
//     //         "RateMs": 1000
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);

//     // 9
//     seq_t.seconds_on = 2; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colour/Bright White Stripe",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":160,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 60;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":5
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 8;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":7
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);




//   }else

//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** Night Colour+White Strip *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   if(id == 19) 
//   {

//     ALOG_INF(PSTR("Sequence: Night, Colour/White 136"));

//     // 9
//     seq_t.seconds_on = 10; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colour/Bright White Stripe",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 9
//     seq_t.seconds_on = 2; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Colour/Bright White Stripe",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":136,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 20;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":5
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // // 10
//     // seq_t.seconds_on = 20;
//     // seq_t.time_enabled.isArmed = true;
//     // seq_t.time_enabled.start = {0, 16, 0, 0};
//     // seq_t.time_enabled.end   = {0, 21, 0, 0};
//     // seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "Effects": {
//     //         "Intensity":11
//     //       }
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);




//   }else

//   // Demo mode
//   if(id == 3)
//   {
    
//     // 1
//     seq_t.seconds_on = 30;
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Wipe Random",
//             "Intensity":255,
//             "Speed":100,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":1
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//       )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 1
//     seq_t.seconds_on = 30;
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Wipe Random",
//             "Intensity":255,
//             "Speed":180,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":1
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//       )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 1
//     seq_t.seconds_on = 30;
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Wipe Random",
//             "Intensity":255,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":1
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 25
//           },
//           "BrightnessRGB": 100
//         }
//       )=====";
//     sequencer_item_list.push_back(seq_t);


//     // Last
//     seq_t.seconds_on = 1;
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Sequencer":{"LoadSet":1}
//         }
//       )=====";
//     sequencer_item_list.push_back(seq_t);


//     ALOG_INF(PSTR("Loaded Sequencer 2"));
//   }

//   else



  
//   // Test Mode - New ones for easy testing
//   if(id == 20)
//   {
    
//     // // 6 - starburst rainbow over white
//     // seq_t.seconds_on = 10;
//     // seq_t.description = "Fireworks Starburst, Rainbow 16, 0/23";
//     // seq_t.json_command = 
//     //   R"=====(
//     //     {
//     //       "ColourPalette":"Rainbow 16",
//     //       "Effects": {
//     //         "Function":"Fireworks Starburst",
//     //         "Intensity":255,
//     //         "Speed":255,
//     //         "Grouping":20,
//     //         "Decimate":0
//     //       },
//     //       "Transition": {
//     //         "TimeMs": 0,
//     //         "RateMs": 23
//     //       },
//     //       "SegColour1":{
//     //         "Hue":20,
//     //         "Sat":0,
//     //         "BrightnessRGB":15
//     //       },
//     //       "BrightnessRGB": 100
//     //     }
//     //     )=====";
//     // sequencer_item_list.push_back(seq_t);

    
//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 10;
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Cold White",
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 20
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 5 - Twinkle colours onto white (inverted twinkle) - over white, then try over dim green
//     seq_t.seconds_on = 10;
//     seq_t.description = "Twinkle Snowy on White, Snowy 02, 0/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":0,
//           "SegColour0":{
//             "Hue":120,
//             "Sat":100,
//             "BrightnessRGB":0
//           },
//           "Effects": {
//             "Function":"Twinkle Palette Two on One",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0,
//             "Param0":125
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 100
//           },
//           "BrightnessRGB": 30
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);



//     ALOG_INF(PSTR("Loaded Sequencer 2"));
//   }


//   else
//   // Test Mode - Time testing
//   if(id == 21)
//   {    
    
//     seq_t.seconds_on = 5;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 12, 55, 0};
//     seq_t.time_enabled.end = {0, 13, 0, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 1
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     //
//     seq_t.seconds_on = 5;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 12, 56, 0};
//     seq_t.time_enabled.end = {0, 13, 10, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Blue and White",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 1
//         }
//         )=====";

//     sequencer_item_list.push_back(seq_t);


    
//     seq_t.seconds_on = 5;
//     seq_t.time_enabled.isArmed = false;
//     seq_t.time_enabled.start = {0, 13, 10, 0};
//     seq_t.time_enabled.end = {0, 13, 20, 0};
//     seq_t.description = "Static, Snowy 02, 0/1000";
//     seq_t.json_command = 
//       R"=====(
//         {
//         "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":127,
//             "Speed":127,
//             "Grouping":1,
//             "Decimate":0,
//             "Reverse":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 1
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);



//     ALOG_INF(PSTR("Loaded Sequencer 2"));
//   }
  
//   /*********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * **** *****************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************
//    * *********************************************************************************************************************************************************************************************************************************  */
//   else
//   if(id == 22) 
//   {

//     ALOG_INF(PSTR("Sequence: For videos to show people, rotations"));

//     // 9
//     seq_t.seconds_on = 2; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Blue and White",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":190,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Blue and White",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":5
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":11
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 9
//     seq_t.seconds_on = 2; //short, just preloading the rotation next
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Static Palette, G=200,Snowy 02, 10000/20000";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Static Palette",
//             "Intensity":20,
//             "Speed":255,
//             "Grouping":190,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 900,
//             "RateMs": 1000
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "ColourPalette":"Christmas Snowy 02",
//           "Effects": {
//             "Function":"Rotating Previous",
//             "Intensity":0,
//             "Speed":255,
//             "Grouping":1,
//             "Decimate":0
//           },
//           "Transition": {
//             "TimeMs": 0,
//             "RateMs": 20
//           },
//           "BrightnessRGB": 100
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);


//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":5
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);

//     // 10
//     seq_t.seconds_on = 10;
//     seq_t.time_enabled.isArmed = true;
//     seq_t.time_enabled.start = {0, 16, 0, 0};
//     seq_t.time_enabled.end   = {0, 21, 0, 0};
//     seq_t.description = "Rotating Previous, Snowy 02, 50/100";
//     seq_t.json_command = 
//       R"=====(
//         {
//           "Effects": {
//             "Intensity":11
//           }
//         }
//         )=====";
//     sequencer_item_list.push_back(seq_t);



//   }

//   sequencer_runtime.seconds_remaining_on_item = 0; // Load now

//   mqtthandler_automation_sequencer.flags.SendNow = true;

//   #endif // ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS



//   sequencer_runtime.loaded_sequence_set = id;

// }

// #endif // ENABLE_FEATURE_SEQUENCE__DEVICE_OUTSIDETREE

// #endif // ENABLE_FEATURE_LIGHTING__SEQUENCER

// #endif //USE_MODULE_LIGHTS_ANIMATOR
