/**

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

#include "mMAVLinkFlyingLEDS.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS

int8_t mMavlinkFlyingLEDS::Tasker(uint8_t function, JsonParserObject obj){

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

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    case TASK_LOOP:
      EveryLoop();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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



void mMavlinkFlyingLEDS::Pre_Init(void)
{

}


void mMavlinkFlyingLEDS::Init(void)
{

  module_state.mode = ModuleStatus::Running;

}


void mMavlinkFlyingLEDS::EverySecond()
{
  
  // SubTask_UpdateOLED();

  // uint16_t throttle_pwm = tkr_mavlink->pkt.rc_channels.data.chan3_raw;

  // uint16_t hue = map(throttle_pwm, 1000, 2000, 0, 360);

  // ALOG_INF(PSTR("Throttle: %d, Hue: %d"), throttle_pwm, hue);

  // HsbColor color = HsbColor(hue/360.0f, 1, 1);

  // tkr_anim->SEGMENT_I(0).RgbwwColors[0] = color;
  // tkr_anim->SEGMENT_I(0).palette_id = 0;
  // tkr_anim->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;

}

void mMavlinkFlyingLEDS::Effect_LandingLights()
{

  ALOG_INF(PSTR("Effect_LandingLights"));

  uint8_t rgbbri = map(constrain(mav.slider_left, 1000, 2000),1000,2000,0,255);
  uint8_t cctbri = map(constrain(mav.slider_left, 1000, 2000),1000,2000,0,255);


  /**
   * @brief Segment0 [Wings]
   **/
  tkr_anim->SEGMENT_I(0).RgbwwColors[0] = RgbwwColor(255,255,255,255,255); //white
  tkr_anim->SEGMENT_I(0).palette_id = 0;
  tkr_anim->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(0).speed = 255;
  tkr_anim->SEGMENT_I(0).setBrightnessRGB(0);
  tkr_anim->SEGMENT_I(0).setBrightnessCCT(255);
  /**
   * @brief Segment1 [fuselage]
   **/
  tkr_anim->SEGMENT_I(1).RgbwwColors[0] = RgbwwColor(255,255,255,255,255); //white
  tkr_anim->SEGMENT_I(1).palette_id = 0;
  tkr_anim->SEGMENT_I(1).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(1).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(1).speed = 255;
  tkr_anim->SEGMENT_I(1).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(1).setBrightnessCCT(255);
  /**
   * @brief Segment1 [Hatches] - when landing, make the hatches be chasing (for power reserve) but match rudder solid for altitude
   **/
  // For testing of full range, lets use "seconds" as the ticker
  HsbColor color_hatch_chase = HsbColor(0, 1, 1);
  // #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_TIME_FOR_RANGE_TESTING
  // hue = map(tkr_time->RtcTime.second, 0,59, 0,359);
  // ALOG_INF(PSTR("Hue: %d"), hue);
  // color_hatch_chase = HsbColor(hue/360.0f, 1, 1);
  // #else
    #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING

    float altitude = test_float;

    if(altitude > 50)
    { 
      color_hatch_chase = HsbColor(240/360.0f, 1, 1); 
    }else{
      altitude = constrain(altitude, 0, 50);
      float hue = mSupport::mapfloat(altitude, 0, 50, 0.0f, 1.0f);
      color_hatch_chase = HsbColor(hue, 1, 1);
    }

    #else
    
    
      uint32_t mode_pwm = 0;
      mMAVLink_Decoder::PacketData* message = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_GPS_RAW_INT);
      if (message != nullptr && message->data != nullptr) {
          mavlink_gps_raw_int_t* rc_channels_data = static_cast<mavlink_gps_raw_int_t*>(message->data);
          mode_pwm = rc_channels_data->alt;
      }


      
      float altitude = (float)mode_pwm/1000.0f;

      // Serial.printf("Altitude: %f\n\r", altitude);
      // Serial.println(altitude);



      if(altitude > 50)
      { 
        color_hatch_chase = HsbColor(240/360.0f, 1, 1); 
      }else{
        altitude = constrain(altitude, 0, 50);
        float hue = mSupport::mapfloat(altitude, 0, 50, 0.0f, 1.0f);
        color_hatch_chase = HsbColor(hue, 1, 1);
      }



    #endif
  // #endif
  tkr_anim->SEGMENT_I(2).RgbwwColors[0] = color_hatch_chase;
  tkr_anim->SEGMENT_I(2).RgbwwColors[1] = RgbwwColor(0,0,0,0,0); //black
  tkr_anim->SEGMENT_I(2).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  tkr_anim->SEGMENT_I(2).effect_id = mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  tkr_anim->SEGMENT_I(2).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(2).speed = 255;
  tkr_anim->SEGMENT_I(2).cycle_time__rate_ms = 25;
  tkr_anim->SEGMENT_I(2).reverse = true;
  tkr_anim->SEGMENT_I(2).setBrightnessRGB(50);
  tkr_anim->SEGMENT_I(2).setBrightnessCCT(0);
  /**
   * @brief Segment1 [Rudder] - height is colour
   **/
  HsbColor color_rudder = color_hatch_chase;
  tkr_anim->SEGMENT_I(3).RgbwwColors[0] = color_rudder;
  tkr_anim->SEGMENT_I(3).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  tkr_anim->SEGMENT_I(3).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(3).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(3).speed = 255;
  tkr_anim->SEGMENT_I(3).cycle_time__rate_ms = 25;
  tkr_anim->SEGMENT_I(3).setBrightnessRGB(50);
  tkr_anim->SEGMENT_I(3).setBrightnessCCT(0);
  /**
   * @brief Segment4 [Nose]
   **/
  tkr_anim->SEGMENT_I(4).RgbwwColors[0] = RgbwwColor(255,255,255,255,255); //white
  tkr_anim->SEGMENT_I(4).palette_id = 0;
  tkr_anim->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(4).speed = 255;
  tkr_anim->SEGMENT_I(4).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(4).setBrightnessCCT(255);



}


void mMavlinkFlyingLEDS::Effect_TakeoffLights()
{


  /**
   * @brief Segment0 [Wings]
   **/
  tkr_anim->SEGMENT_I(0).RgbwwColors[0] = RgbwwColor(0,255,0,0,0); //white
  tkr_anim->SEGMENT_I(0).palette_id = 0;
  tkr_anim->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__ANDROID__ID;
  tkr_anim->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(0).speed = 255;
  tkr_anim->SEGMENT_I(0).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(0).setBrightnessCCT(255);


  /**
   * @brief Segment1 [Rudder] - height is colour
   **/
  // HsbColor color_rudder = color_hatch_chase;
  // tkr_anim->SEGMENT_I(3).RgbwwColors[0] = color_rudder;
  tkr_anim->SEGMENT_I(3).palette_id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__YELLOWOUT__ID;
  tkr_anim->SEGMENT_I(3).effect_id = mAnimatorLight::EFFECTS_FUNCTION__RAINBOW_CYCLE__ID;
  tkr_anim->SEGMENT_I(3).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(3).speed = 255;
  tkr_anim->SEGMENT_I(3).cycle_time__rate_ms = 25;
  tkr_anim->SEGMENT_I(3).setBrightnessRGB(50);
  tkr_anim->SEGMENT_I(3).setBrightnessCCT(0);


  /**
   * @brief Segment4 [Nose]
   **/
  tkr_anim->SEGMENT_I(4).RgbwwColors[0] = RgbwwColor(0,255,0,0,0); //white
  tkr_anim->SEGMENT_I(4).palette_id = 0;
  tkr_anim->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(4).speed = 255;
  tkr_anim->SEGMENT_I(4).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(4).setBrightnessCCT(255);



}





void mMavlinkFlyingLEDS::Effect_Flight01_RandomSweep()
{

  
  /**
   * @brief Segment0 [Wings]
   **/
  tkr_anim->SEGMENT_I(0).RgbwwColors[0] = RgbwwColor(255,255,255,255,255); //white
  tkr_anim->SEGMENT_I(0).palette_id = 0;
  // tkr_anim->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID;// 71;//mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  tkr_anim->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__COLOR_SWEEP_RANDOM__ID;// 71;//mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  tkr_anim->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(0).mirror = true; // so it comes from centre out
  tkr_anim->SEGMENT_I(0).reverse = true;
  tkr_anim->SEGMENT_I(0).speed = 200;
  tkr_anim->SEGMENT_I(0).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(0).setBrightnessCCT(0);
  /**
   * @brief Segment1 [Fuselage]
   **/
  // tkr_anim->SEGMENT_I(1).RgbwwColors[0] = RgbwwColor(0,0,0,255,255); //white
  // tkr_anim->SEGMENT_I(1).RgbwwColors[1] = RgbwwColor(0,0,255,255,255); //white
  // tkr_anim->SEGMENT_I(1).RgbwwColors[2] = RgbwwColor(0,0,0,100,100); //white
  // tkr_anim->SEGMENT_I(1).palette_id = mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED + 9;
  // tkr_anim->SEGMENT_I(1).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SPLIT_PALETTE_SEGWIDTH__ID;
  // tkr_anim->SEGMENT_I(1).colour_width__used_in_effect_generate = mAnimatorLight::ColourType::COLOUR_TYPE__RGBWW__ID;
  // tkr_anim->SEGMENT_I(1).cycle_time__rate_ms = FRAMETIME_MS;
  // tkr_anim->SEGMENT_I(1).speed = 255;
  // tkr_anim->SEGMENT_I(1).setBrightnessRGB(255);
  // tkr_anim->SEGMENT_I(1).setBrightnessCCT(255);
  // mPalette::PALETTE_ENCODING_DATA encoding = {PALETTE_ENCODING_TYPE_RGBCCT_WITHINDEX_GRADIENT}; 
  // uint8_t palette_data[] = {
  //   // White forward
  //   0,    0,0,0,255,255,
  //   100,    0,0,0,255,255,
  //   110,    0,0,255,0,0,
  //   127,  0,0,255,0,0,
  //   195,  0,0,255,0,0,
  //   205,  0,0,255,0,0,
  //   255,  0,0,0,255,255
  //   // Blue forward
  //   // 0,    0,0,255,0,0,
  //   // 10,   0,0,255,0,0,
  //   // 40,   0,0,0,30,30,
  //   // 127,  0,0,0,30,30,
  //   // 215,  0,0,0,30,30,
  //   // 245,  0,0,255,0,0,
  //   // 255,  0,0,255,0,0,
  // };
  // uint8_t palette_custom_index = 9;
  // tkr_anim->CommandSet_CustomPalette(palette_custom_index, encoding.data, palette_data, ARRAY_SIZE(palette_data));
  // /**
  //  * @brief Segment1 [Fuselage] - Copy start of wings
  //  **/
  // uint32_t col32 = tkr_anim->getPixelColor(41);
  // tkr_anim->SEGMENT_I(1).RgbwwColors[0] = RgbwwColor(R(col32), G(col32), B(col32), 0, 0); //white
  // tkr_anim->SEGMENT_I(1).palette_id = 0;
  // tkr_anim->SEGMENT_I(1).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  // tkr_anim->SEGMENT_I(1).colour_width__used_in_effect_generate = mAnimatorLight::ColourType::COLOUR_TYPE__RGBWW__ID;
  // tkr_anim->SEGMENT_I(1).cycle_time__rate_ms = FRAMETIME_MS;
  // tkr_anim->SEGMENT_I(1).speed = 255;
  // tkr_anim->SEGMENT_I(1).setBrightnessRGB(255);
  // tkr_anim->SEGMENT_I(1).setBrightnessCCT(255);
  /**
   * @brief Segment1 [Fuselage] - Copy start of wings, with gradient to nose
   **/
  tkr_anim->SEGMENT_I(1).palette_id = mPalette::PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED + 9;
  tkr_anim->SEGMENT_I(1).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SPLIT_PALETTE_SEGWIDTH__ID;
  tkr_anim->SEGMENT_I(1).colour_width__used_in_effect_generate = mAnimatorLight::ColourType::COLOUR_TYPE__RGBCCT__ID;
  tkr_anim->SEGMENT_I(1).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(1).speed = 255;
  tkr_anim->SEGMENT_I(1).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(1).setBrightnessCCT(255);
  uint32_t col32 = tkr_anim->getPixelColor(41);
  mPalette::PALETTE_ENCODING_DATA encoding = {PALETTE_ENCODING_TYPE_RGBCCT_WITHINDEX_GRADIENT}; 
  uint8_t palette_data[] = {
    // White forward near nose, otherwise copy the wing
    0,    0,0,0,255,255,
    20,    0,0,0,255,255,
    55,  R(col32), G(col32), B(col32),0,0,
    127,  R(col32), G(col32), B(col32),0,0,
    200,  R(col32), G(col32), B(col32),0,0,
    235,  0,0,0,255,255,
    255,  0,0,0,255,255
  };
  uint8_t palette_custom_index = 9;
  tkr_anim->CommandSet_CustomPalette(palette_custom_index, encoding.data, palette_data, ARRAY_SIZE(palette_data));

  /**
   * @brief Segment1 [Hatches] - when landing, make the hatches be chasing (for power reserve) but match rudder solid for altitude
   **/
  // For testing of full range, lets use "seconds" as the ticker
  HsbColor color_hatch_chase = HsbColor(0, 1, 1);
  // #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_TIME_FOR_RANGE_TESTING
  // hue = map(tkr_time->RtcTime.second, 0,59, 0,359);
  // ALOG_INF(PSTR("Hue: %d"), hue);
  // color_hatch_chase = HsbColor(hue/360.0f, 1, 1);
  // #else
    #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING

    float altitude = test_float;

    if(altitude > 50)
    { 
      color_hatch_chase = HsbColor(240/360.0f, 1, 1); 
    }else{
      altitude = constrain(altitude, 0, 50);
      float hue = mSupport::mapfloat(altitude, 0, 50, 0.0f, 1.0f);
      color_hatch_chase = HsbColor(hue, 1, 1);
    }

    #else


      uint32_t mode_pwm = 0;
      mMAVLink_Decoder::PacketData* message = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_GPS_RAW_INT);
      if (message != nullptr && message->data != nullptr) {
          mavlink_gps_raw_int_t* rc_channels_data = static_cast<mavlink_gps_raw_int_t*>(message->data);
          mode_pwm = rc_channels_data->alt;
      }


      
      float altitude = (float)mode_pwm/1000.0f;

      // Serial.printf("Altitude: %f\n\r", altitude);
      // Serial.println(altitude);



      if(altitude > 120)
      { 
        color_hatch_chase = HsbColor(240/360.0f, 1, 1); 
      }else{
        altitude = constrain(altitude, 0, 120);
        float hue = mSupport::mapfloat(altitude, 0, 120, 0.0f, 1.0f);
        color_hatch_chase = HsbColor(hue, 1, 1);
      }


    
    #endif
  // #endif
  tkr_anim->SEGMENT_I(2).RgbwwColors[0] = color_hatch_chase;
  tkr_anim->SEGMENT_I(2).RgbwwColors[1] = RgbwwColor(0,0,0,0,0); //black
  tkr_anim->SEGMENT_I(2).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  tkr_anim->SEGMENT_I(2).effect_id = mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  tkr_anim->SEGMENT_I(2).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(2).speed = 255;
  tkr_anim->SEGMENT_I(2).cycle_time__rate_ms = 25;
  tkr_anim->SEGMENT_I(2).reverse = true;
  tkr_anim->SEGMENT_I(2).setBrightnessRGB(50);
  tkr_anim->SEGMENT_I(2).setBrightnessCCT(0);
  /**
   * @brief Segment1 [Rudder] - height is colour
   **/
  HsbColor color_rudder = color_hatch_chase;
  tkr_anim->SEGMENT_I(3).RgbwwColors[0] = color_rudder;
  tkr_anim->SEGMENT_I(3).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  tkr_anim->SEGMENT_I(3).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(3).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(3).speed = 255;
  tkr_anim->SEGMENT_I(3).cycle_time__rate_ms = 25;
  tkr_anim->SEGMENT_I(3).setBrightnessRGB(50);
  tkr_anim->SEGMENT_I(3).setBrightnessCCT(0);
  /**
   * @brief Segment4 [Nose]
   **/
  tkr_anim->SEGMENT_I(4).RgbwwColors[0] = RgbwwColor(0,0,0,255,255); //white
  tkr_anim->SEGMENT_I(4).palette_id = 0;
  tkr_anim->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  tkr_anim->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
  tkr_anim->SEGMENT_I(4).speed = 255;
  tkr_anim->SEGMENT_I(4).setBrightnessRGB(255);
  tkr_anim->SEGMENT_I(4).setBrightnessCCT(255);


}

/**
 * @brief Keeping this separate so I can easily have ifdef debug methods to test the lighting
 * Also have a timeout, if mavlink data is old then revert to failsafe LEDs
 * 
 */
void mMavlinkFlyingLEDS::Update_ControlDataFromMAVLink()
{


  mMAVLink_Decoder::PacketData* packet_rc_channels = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_RC_CHANNELS);
  if (packet_rc_channels != nullptr && packet_rc_channels->data != nullptr) {
    mavlink_rc_channels_t* rc_channels_data = static_cast<mavlink_rc_channels_t*>(packet_rc_channels->data);
    mav.buttons_6P = rc_channels_data->chan14_raw;
    mav.slider_left = rc_channels_data->chan15_raw;
    mav.slider_left = rc_channels_data->chan15_raw;
    mav.slider_right = rc_channels_data->chan16_raw;
  }


  /**
   * @brief Override for debug, set values I want
   * 
   */
  #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING
    mav.slider_left = 2;
    mav.slider_right = 900;
  #endif


  
}

/**
 * @brief 
 * LEFT_SLIDER:  Mode select, with 5 positions
 * * BOTTOM:      Landing Lights
 * * MID-BOTTOM:  Landing Basic   [so I can transition from Centre/Inflight, to basic, then full landing]
 * * CENTRE:      Inflight Lighting (based on RS)
 * * MID-TOP:     Takeoff Lights
 * * TOP:         Inflight Lighting default (keeping this so its always a set basic colour as backup)
 * RIGHT_SLIDER: Mode intensity (or alternate versions)
 * 
 */
void mMavlinkFlyingLEDS::EveryLoop()
{

  Update_ControlDataFromMAVLink();


  uint16_t mode_pwm = mav.buttons_6P;

  // ALOG_INF(PSTR("buttons_6P = %d"), mav.buttons_6P);

  // return;
  
  enum SWITCH_POSITION__MODE{
    Bottom__Landing_Full = 1,
    MidBottom__Landing_Basic_And_Failsafe = 1,
    Centre__Inflight_Multimode = 2,
    MidTop__Takeoff = 3,
    Top__Inflight_Default = 4
  };
  
  // Slider Left
  // uint8_t mode = 0; // should be failsafe as "landing" lighting mode
  // if     (mode_pwm <= 1000){ mode = 0; }                            // Bottom
  // else if((mode_pwm > 1000) && (mode_pwm <= 1400)){ mode = 1; } // Mid-Bottom
  // else if((mode_pwm > 1400) && (mode_pwm <= 1600)){ mode = Centre__Inflight_Multimode; } // Centre
  // else if((mode_pwm > 1600) && (mode_pwm <= 2000)){ mode = 3; } // Mid-Top
  // else if(mode_pwm > 2000){ mode = 4; }     

  // Slider right, 6 options
  uint8_t mode = 0; // should be failsafe as "landing" lighting mode
  if     (mode_pwm <= 1000){ mode = 1; }                            // Bottom
  else if((mode_pwm > 1000) && (mode_pwm <= 1250)){ mode = 2; } // Mid-Bottom
  else if((mode_pwm > 1250) && (mode_pwm <= 1500)){ mode = 3; } // Centre
  else if((mode_pwm > 1500) && (mode_pwm <= 1700)){ mode = 4; } // Mid-Top
  else if((mode_pwm > 1700) && (mode_pwm <= 1850)){ mode = 5; } // Mid-Top
  else if(mode_pwm > 1850){ mode = 6; }                             // Top

  #ifdef ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE
  mode = ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE;
  #endif

  ALOG_INF(PSTR("buttons_6P %d, mode = %d"), mav.buttons_6P, mode);
  // if(effect_manual)
  // {
    // mode = effect_mode;
  // }

  // if(effect_manual == true)
  // {
    switch(mode)
    {
      case Bottom__Landing_Full: // Landing Lights
        Effect_LandingLights();
      break;
      default:
        ALOG_INF(PSTR("Error: effect_mode %d not found"), mode);
      // case MidBottom__Landing_Basic_And_Failsafe:
      //   effect_manual = false;
      // break;
      case Centre__Inflight_Multimode: // Inflight Lighting

        /**
         * @brief Slider Right in multimode just switches between the different effects
         * 
         */
        // if(IS_WITHIN_RANGE_MAXINC(0, 1000, mav.slider_right))
        // {
          Effect_Flight01_RandomSweep();
        // }

      break;
      case MidTop__Takeoff: // Takeoff Lights
        Effect_TakeoffLights();
      break;
      case 4:
        
        tkr_anim->SEGMENT_I(4).RgbwwColors[0] = RgbwwColor(255,0,0,0,0); //white
        tkr_anim->SEGMENT_I(4).palette_id = 0;
        tkr_anim->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
        tkr_anim->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
        tkr_anim->SEGMENT_I(4).speed = 200;
        tkr_anim->SEGMENT_I(4).setBrightnessRGB(255);
        tkr_anim->SEGMENT_I(4).setBrightnessCCT(0);

      break;
      case 5:
        
        tkr_anim->SEGMENT_I(4).RgbwwColors[0] = RgbwwColor(0,255,0,0,0); //white
        tkr_anim->SEGMENT_I(4).palette_id = 0;
        tkr_anim->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
        tkr_anim->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
        tkr_anim->SEGMENT_I(4).speed = 200;
        tkr_anim->SEGMENT_I(4).setBrightnessRGB(255);
        tkr_anim->SEGMENT_I(4).setBrightnessCCT(0);

      break;
      case 6:
        
        tkr_anim->SEGMENT_I(4).RgbwwColors[0] = RgbwwColor(0,0,255,0,0); //white
        tkr_anim->SEGMENT_I(4).palette_id = 0;
        tkr_anim->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
        tkr_anim->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
        tkr_anim->SEGMENT_I(4).speed = 200;
        tkr_anim->SEGMENT_I(4).setBrightnessRGB(255);
        tkr_anim->SEGMENT_I(4).setBrightnessCCT(0);


        /**
         * @brief Segment1 [wing] - height is colour
         **/
        // HsbColor color_rudder = color_hatch_chase;
        // tkr_anim->SEGMENT_I(3).RgbwwColors[0] = color_rudder;
        tkr_anim->SEGMENT_I(0).palette_id = mPalette::PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__YELLOWOUT__ID;
        tkr_anim->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__RAINBOW_CYCLE__ID;
        tkr_anim->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
        tkr_anim->SEGMENT_I(0).speed = 255;
        tkr_anim->SEGMENT_I(0).cycle_time__rate_ms = 25;
        tkr_anim->SEGMENT_I(0).setBrightnessRGB(255);
        tkr_anim->SEGMENT_I(0).setBrightnessCCT(0);

      break;




      // case Top__Inflight_Default: // Flight 01 - Random Sweep
      //   Effect_Flight01_RandomSweep();
      // break;
    }
  // }

/**
 * @brief For now, make the right slider the master brightness
 * 
 */
  uint8_t rgbbri = map(constrain(mav.slider_right, 1000, 2000),1000,2000,0,255);
  uint8_t cctbri = map(constrain(mav.slider_right, 1000, 2000),1000,2000,0,255);

  // ALOG_INF(PSTR("mav.slider_right %d rgbbri = %d %d"), mav.slider_right, rgbbri, cctbri );

  pCONT_iLight->setBriRGB_Global(rgbbri);
  pCONT_iLight->setBriCT_Global(cctbri);


  // ALOG_INF(PSTR("Throttle: %d, Hue: %d, Mode %d"), mode_pwm, hue, mode);

}

/******************************************************************************************************************
 * mTempSensorOLEDBath_Commands.cpp
*******************************************************************************************************************/


void mMavlinkFlyingLEDS::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];

  if(jtok = obj["MAVEffect"])
  {
    
    effect_manual = true;
    effect_mode = jtok.getInt();

    switch(effect_mode)
    {
      default:
      case 0:
        effect_manual = false;
      break;
      case 1: // Landing Lights
        Effect_LandingLights();
      break;
      case 2: // Takeoff Lights
        Effect_TakeoffLights();
      break;
      case 3: // Flight 01 - Random Sweep
        Effect_Flight01_RandomSweep();
      break;
    }

    ALOG_COM(PSTR("MAVEffect: %d"), effect_mode);
    
  }

  if(jtok = obj["MAVTestF"])
  {
    test_float = jtok.getFloat();
    // ALOG_COM(PSTR("MAVTestF: %f"), test_float);
  }

    
}


/******************************************************************************************************************
 * mTempSensorOLEDBath_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mMavlinkFlyingLEDS::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  return JBI->End();

}

uint8_t mMavlinkFlyingLEDS::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mMavlinkFlyingLEDS::MQTTHandler_Init()
{

  struct handler<mMavlinkFlyingLEDS>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMavlinkFlyingLEDS::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mMavlinkFlyingLEDS::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mMavlinkFlyingLEDS::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMavlinkFlyingLEDS::MQTTHandler_Rate()
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
void mMavlinkFlyingLEDS::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS