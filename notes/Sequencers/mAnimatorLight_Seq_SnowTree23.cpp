/**
 * @file mAnimatorLight_Mixer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "6_Lights/03_Animator/mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER 


#ifdef ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE

void mAnimatorLight::SubLoad_Sequencer_Device(uint8_t id)
{

  sequencer_item_list.clear(); // clear all sequences
  memset(&sequencer_runtime, 0, sizeof(sequencer_runtime)); // clear all sequences

  struct SEQUENCER_ITEM seq_t;
  
  #ifdef ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS
  if((id == 0)||(id == 1))
  {
    id = 1; // if default, ensure its back to 1 to other functions work

    // 1 - Static 
    seq_t.seconds_on = 60;
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
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    // 3 - Slow glows
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Slow Glow",
            "Intensity":30,
            "Speed":200,
            "Grouping":1,
            "Decimate":30
          },
          "Transition": {
            "TimeMs": 500,
            "RateMs": 1000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 4 - Fast glows as "pops"
    seq_t.seconds_on = 60;
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

    // 2 - shimmer
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Shimmer",
            "Intensity":255,
            "Speed":200,
            "Grouping":1,
            "Decimate":30
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 25
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 5 - White twinkles
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
        "ColourPalette":"Cold White 02",
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
            "RateMs": 100
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 7
    seq_t.seconds_on = 60;
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
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 8
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Colourful Pairs 01",
          "Effects": {
            "Function":"Step Palette",
            "Intensity":20,
            "Speed":255,
            "Grouping":200,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 1500,
            "RateMs": 3000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 9
    seq_t.seconds_on = 10; //short, just preloading the rotation next
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
            "TimeMs": 10000,
            "RateMs": 20000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    // 10
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Rotating Previous",
            "Intensity":20,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 50,
            "RateMs": 100
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

    
    // 18
    seq_t.seconds_on = 60;
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
            "TimeMs": 3000,
            "RateMs": 5000
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);

// here good
    // 11
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Sweep Random",
            "Intensity":255,
            "Speed":127,
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


    // 12
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Fireworks 1D",
            "Intensity":255,
            "Speed":255,
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


    // 13
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Christmas Snowy 02",
          "Effects": {
            "Function":"Fireworks 1D No Launch",
            "Intensity":255,
            "Speed":126,
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


    // 6 - starburst rainbow over white
    seq_t.seconds_on = 60;
    seq_t.json_command = 
      R"=====(
        {
          "ColourPalette":"Rainbow 16",
          "Effects": {
            "Function":"Fireworks Starburst",
            "Intensity":255,
            "Speed":255,
            "Grouping":1,
            "Decimate":0
          },
          "Transition": {
            "TimeMs": 0,
            "RateMs": 23
          },
          "SegColour1":{
            "Hue":20,
            "Sat":0,
            "BrightnessRGB":30
          },
          "BrightnessRGB": 100
        }
        )=====";
    sequencer_item_list.push_back(seq_t);


    /**
     * @brief As debug method, force all seconds_on split across a total time period
     * 
     */
    // for (auto &seq_t:sequencer_item_list)
    // {
    //   seq_t.seconds_on = 120/sequencer_item_list.size();
    // }

    ALOG_INF(PSTR("Loaded Sequencer 1"));

  }else
  // Demo mode
  if(id == 2)
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


    // Last
    seq_t.seconds_on = 1;
    seq_t.json_command = 
      R"=====(
        {
          "Sequencer":{"LoadSet":1}
        }
      )=====";
    sequencer_item_list.push_back(seq_t);


    ALOG_INF(PSTR("Loaded Sequencer 2"));
  }

  sequencer_runtime.seconds_remaining_on_item = 0; // Load now

  mqtthandler_automation_sequencer.flags.SendNow = true;

  #endif // ENABBLE_FEATURE_SEQUENCE__PLAYLIST_OUTSIDE_CHRISTMAS_TREE__VERSION_ONE__NO_TIME_RESTRAINTS



  sequencer_runtime.loaded_sequence_set = id;

}

#endif // ENABLE_FEATURE_SEQUENCE__DEVICE_SNOWTREE

#endif // ENABLE_FEATURE_LIGHTING__SEQUENCER

#endif //USE_MODULE_LIGHTS_ANIMATOR
