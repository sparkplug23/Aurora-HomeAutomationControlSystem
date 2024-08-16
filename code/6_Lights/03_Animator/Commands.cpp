#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

void mAnimatorLight::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::Start"));

  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];
  uint16_t isserviced_start_count = data_buffer.isserviced;

  if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  {
    SEGMENT_I(0).flags.fForceUpdate = true;
  }

  uint8_t segments_found = 0;


  for(uint8_t segment_i = 0; segment_i < MAX_NUM_SEGMENTS; segment_i++)
  {
    snprintf(buffer, sizeof(buffer), "Segment%d", segment_i);
    if(jtok = obj[buffer])
    { 

      ALOG_INF(PSTR("Seg: \"%s\""), buffer);

      if(segment_i >= segments.size())
      { 
        uint16_t seg_size = segments.size();
        Segment_AppendNew(0, 100, segment_i);
        ALOG_INF(PSTR("Created new segment%02d %dB (T%dB)"), segment_i, segments.size()-seg_size, segments.size());
      }
      
      data_buffer.isserviced += subparse_JSONCommand(jtok.getObject(), segment_i);

      segments_found++;

    }
  }

  /**
   * @brief 
   * When commands are not inside a {"Segment#":{commands}} then assume direct control of first segment (ie 0)
   **/
  if(segments_found == 0)
  {
    data_buffer.isserviced += subparse_JSONCommand(obj, 0); // Legacy commands
  }

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::End"));

}




/**
 * @brief Allows new segment controls, by calling this function for each segment.
 *        Calling it without any set index, will default to segment of 0 index and turn the other segments off 
 * @param obj 
 */
uint8_t mAnimatorLight::subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int16_t tmp_id = 0;
  char buffer[50];

  uint16_t isserviced_start_count = data_buffer.isserviced; // to know if anything was serviced in this function

  /**
   * @brief Exit if segment has not been created to stop errors
   **/
  if(!segments.size()){ return 0; }


  if (jtok = obj["PaletteMappingValues"]) { 
    if (jtok.isArray()) {
      // Pre-allocate space in the vector to avoid repeated memory allocation
      auto& mapping_values = SEGMENT_I(segment_index).palette_container->mapping_values;
      mapping_values.clear(); // reset old map
      mapping_values.reserve(16); // reserve space for 16 elements
      JsonParserArray arrobj = jtok;
      for (auto v : arrobj) {
        if (mapping_values.size() >= 16) { break; }
        mapping_values.push_back(v.getFloat());
      }
      data_buffer.isserviced++;
    }
  }

  
  if(jtok = obj[PM_JSON_COLOUR_PALETTE])
  {
    if(jtok.isStr())
    {
      if(strcmp(jtok.getStr(),"+")==0)
      {
        CommandSet_PaletteID( SEGMENT_I(segment_index).palette_id + 1, segment_index);
      }else
      if(strcmp(jtok.getStr(),"-")==0)
      {
        CommandSet_PaletteID( SEGMENT_I(segment_index).palette_id - 1, segment_index);
      }else
      if((tmp_id=GetPaletteIDbyName((char*)jtok.getStr()))>=0)
      {
        CommandSet_PaletteID(tmp_id, segment_index);
      }
    }else
    if(jtok.isNum()){
      CommandSet_PaletteID(jtok.getInt(), segment_index);
    }
    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(SEGMENT_I(segment_index).palette_id, buffer, sizeof(buffer)) );
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_JSON_PIXELRANGE])
  { 
    if(jtok.isArray())
    {
      JsonParserArray arrobj = jtok;
      SEGMENT_I(segment_index).start = arrobj[0].getInt();
      SEGMENT_I(segment_index).stop  = arrobj[1].getInt();
      
      if(SEGMENT_I(segment_index).stop > PIXEL_RANGE_LIMIT)
      {
        ALOG_ERR( PSTR("stop %d exceeds max %d"), SEGMENT_I(segment_index).stop, PIXEL_RANGE_LIMIT);
        SEGMENT_I(segment_index).stop = PIXEL_RANGE_LIMIT;
      }

      ALOG_COM( PSTR(D_LOG_PIXEL "PixelRange = [%d,%d]"), SEGMENT_I(segment_index).start, SEGMENT_I(segment_index).stop );
      data_buffer.isserviced++;
    }
  }


  /*************************************************************************
   *** {"Effects":{X:Y}}
   *************************************************************************/
  if(jtok_sub = obj[PM_JSON_EFFECTS])
  {    
    JsonParserObject jobj = jtok_sub.getObject();

    if(jtok = jobj[PM_JSON_FUNCTION])
    { 
      if(jtok.isStr())
      {
        if((tmp_id=GetFlasherFunctionIDbyName(jtok.getStr()))>=0)
        {
          CommandSet_Flasher_FunctionID(tmp_id, segment_index);
        }
      }else
      if(jtok.isNum())
      {
        CommandSet_Flasher_FunctionID(jtok.getInt(), segment_index);
      }
      ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_SVALUE_K(D_JSON_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_JSON_EFFECT_COLOUR_TYPE])
    {
      CommandSet_Effect_ColourTypeID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_JSON_INTENSITY])
    { 
      CommandSet_Effect_Intensity(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_2KEY_NVALUE_K(D_JSON_EFFECTS, D_JSON_INTENSITY)), jtok.getInt() );
      data_buffer.isserviced++;
    }
    

    if(jtok = jobj[PM_JSON_SPEED])
    { 
      CommandSet_Effect_Speed(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_PIXEL D_JSON_COMMAND_2KEY_NVALUE_K(D_JSON_EFFECTS, D_JSON_SPEED)), jtok.getInt() );
      data_buffer.isserviced++;
    }
    

    if(jtok = jobj[PM_JSON_REVERSE])
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_REVERSED, jtok.getInt());  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_REVERSE)), SEGMENT_I(segment_index).reverse);
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_JSON_MIRROR])
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_MIRROR, jtok.getInt());  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_MIRROR)), SEGMENT_I(segment_index).mirror);
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_JSON_GROUPING])
    { 
      SEGMENT_I(segment_index).grouping = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_GROUPING)), SEGMENT_I(segment_index).grouping);
      data_buffer.isserviced++;
    }

    
    if(jtok = jobj[PM_JSON_DECIMATE])
    {
      SEGMENT_I(segment_index).decimate = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_DECIMATE)), SEGMENT_I(segment_index).decimate);
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_JSON_SPACING])
    { 
      SEGMENT_I(segment_index).spacing = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_EFFECTS D_SPACING)), SEGMENT_I(segment_index).spacing);
      data_buffer.isserviced++;
    }
      

    if(jtok = jobj[PM_JSON_RATE]){ // default to secs
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000, segment_index);
      ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RATE)), SEGMENT_I(segment_index).cycle_time__rate_ms);
      data_buffer.isserviced++;
    }else
    if(jtok = jobj[PM_JSON_RATE_MS]){
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms);  
      data_buffer.isserviced++;
    }

      
    if (jtok = jobj["Params"]) { 
      if (jtok.isArray()) {
        JsonParserArray arrobj = jtok;
        uint8_t array[4] = {0};  // Initialize the array to store values
        arrobj.getArray(array);  // Retrieve array elements

        // Assign retrieved values to SEGMENT_I params_user
        auto& params_user = SEGMENT_I(segment_index).params_user;
        std::copy(array, array + 4, params_user);

        ALOG_COM(PSTR(D_LOG_PIXEL "Params %d = %d,%d,%d,%d"), segment_index, params_user[0], params_user[1], params_user[2], params_user[3]);
        data_buffer.isserviced++;
      }
    }else{
      char keyname[10];
      for(int i=0;i<4;i++)
      {
        snprintf(keyname, sizeof(keyname), "Param%d", i);
        if(jtok = jobj[keyname])
        {
          auto& params_user = SEGMENT_I(segment_index).params_user;
          params_user[i] = jtok.getInt();
          ALOG_COM(PSTR(D_LOG_PIXEL "Params %d = %d,%d,%d,%d"), segment_index, params_user[0], params_user[1], params_user[2], params_user[3]);
          data_buffer.isserviced++;
        }
      }
    }    

  } // PM_JSON_EFFECTS


  /**
   * @brief Example commands
   * 
    {
      "CustomPalette": {
        "Data": [
          0,      255,      255,      255,
          150,      0,      255,      0,
          255,      0,      0,      255
        ],
        "Encoding": "gRGB",
        "Index": 9
      }
    }
    {
      "CustomPalette": {
        "Data": [
          255,      255,      255,
          0,      255,      0,
          0,      0,      255
        ],
        "Encoding": "RGB",
        "Index": 8
      }
    }
   * 
   */
  if(jtok = obj["CustomPalette"])
  {
    JsonParserToken jtok_sub = 0;

    uint8_t index = 0;
    if(jtok_sub = jtok.getObject()["Index"])
    {
      index = jtok_sub.getInt();
      ALOG_DBM(PSTR("Index %d"), index);
    }

    mPalette::PALETTE_ENCODING_DATA encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX}; // 0b00000111 0x07
    if(jtok_sub = jtok.getObject()["Encoding"])
    {
      if(jtok_sub.isStr())
      {
        if(strcmp(jtok_sub.getStr(),"RGB")==0)
        {
          encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX};
        }
        else
        if(strcmp(jtok_sub.getStr(),"gRGB")==0)
        {
          encoding = {PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT};
        }
      }
      else
      {
        uint16_t enc = jtok_sub.getInt();
        encoding = {enc};
      }
      ALOG_DBG(PSTR("Encoding %d"), encoding);
    }


    if (jtok_sub = jtok.getObject()["Data"])
    {
      if (jtok_sub.isArray())
      {
        ALOG_DBM(PSTR("Data length %d"), jtok_sub.size());
        // Create a JsonParserArray object to use array-specific methods
        JsonParserArray arrobj = jtok_sub;
        // Define an array large enough to store up to 255 elements
        uint8_t array[255] = {0};
        // Use the getArray method directly to fill the array
        arrobj.getArray(array);
        // Calculate the actual length (since `getArray` stops at the array's size limit)
        uint8_t data_length = arrobj.size() < 255 ? arrobj.size() : 255;
        // Perform your command with the filled array and its length
        CommandSet_CustomPalette(index, encoding.data, array, data_length);
        data_buffer.isserviced++;
      }
    }
  }
  

  #if FIRMWARE_VERSION_MIN(1,1)

    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION_SHORTCUT
    if(jtok = obj["LightNotif"])
    { 
      // parsesub_LightNotif(obj);
      if(jtok.isArray())
      {
        JsonParserArray arrobj = jtok;
        for(auto v : arrobj) 
        {
          // bool result = v.isObject();
          // Serial.println(v.isObject());

          parsesub_json_object_notification_shortcut(v.getObject());

          // JsonParserObject obj1 = v.getObject();
          // JsonParserToken jtok1 = 0;


          // if(jtok1 = obj1["SegIndex"])
          // {
          //   ALOG_INF(PSTR("SegIndex %d"),jtok1.getInt());
          // }

        }
      }

      // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(0).rgbcctcolors[0].getHue360());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(0).rgbcctcolors[0].getBrightnessRGB());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(1).rgbcctcolors[0].getHue360());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(1).rgbcctcolors[0].getBrightnessRGB());


      // "LightNotif":
      //   [
      //     {
      //       "SegIndex":0,
      //       "Colour":[123,100,100],
      //       "Data":[start, stop, notif_effect, period_ms, repeats, duty_cycle, timeout_seconds]
      //     },
      //     {
      //       "SegIndex":0,
      //       "Colour":[123,100,100],
      //       "Data":[start, stop, notif_effect, period_ms, repeats, duty_cycle, timeout_seconds]
      //     }
      //   ]
      //     "Values":[segindex, start, stop, hue, sat, brightness, effect, period_ms, repeats, duty_cycle, timeout_seconds]
      //   ],
      //   [
      //     "values":[start, stop, period_ms, repeats, duty_cycle, timeout_seconds],
      //     "colour":[120,100,100]
      //   ]
      // }
    }
    #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION


    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
    if(jtok = obj[PM_JSON_ANIMATIONMODE]){
      if(jtok.isStr()){
        if((tmp_id=GetAnimationModeIDbyName(jtok.getStr()))>=0){
          CommandSet_AnimationModeID(tmp_id);
          data_buffer.isserviced++;
        }
      }else
      if(jtok.isNum()){      
        CommandSet_AnimationModeID(jtok.getInt());
        data_buffer.isserviced++;
      }
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
  
  #endif // FIRMWARE_VERSION_MIN
  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  if(jtok = obj[PM_JSON_RGB_CLOCK].getObject()[PM_JSON_MANUAL_NUMBER]){
    lcd_display_show_number = jtok.getInt();
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_JSON_COMMAND_NVALUE_K(D_JSON_MANUAL_NUMBER)), lcd_display_show_number);
  }
  
  if(jtok = obj[PM_JSON_RGB_CLOCK].getObject()["ManualString"]){
    strcpy(lcd_display_show_string, jtok.getStr());
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_JSON_COMMAND_SVALUE_K("ManualString")), lcd_display_show_string);
  }
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK


  #if FIRMWARE_VERSION_MIN(0,126)
  #ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
  // This is an extension of the custom palette, but is autogenerated based on values based in to offload colour conversion for openhab etc
  // Hue// Sat// ColourHeatMap:[style, cold_point, now_temp, hot_point]
  if(jtok = obj["ColourHeatMap_Palette"])
  {

    uint8_t style_index  = 0;
    if(jtok_sub = jtok.getObject()["Method"]) //different heatmap conversion method
    {
      style_index = jtok_sub.getInt();
      ALOG_COM(PSTR(D_JSON_COMMAND_NVALUE_K("Method")), style_index);
    }

    if(jtok_sub = jtok.getObject()["Value"])
    {
      if(jtok_sub.isArray())
      {
        ALOG_COM(PSTR("is ARRAY"));

        float array_val[4] = {0}; uint8_t array_ii = 0;
        JsonParserArray array = jtok_sub;
        for(auto v : array)
        {
          ALOG_COM(PSTR("v=%d"), (int)v.getFloat());
          array_val[array_ii++] = v.getFloat();
        }
        CommandSet_ColourHeatMap_Palette(array_val, array_ii, style_index); // Dont pass palette, then assumes active        
      }

    }
  }
  #endif // ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
  #endif // FIRMWARE_VERSION_MIN
  

  if(jtok_sub = obj[PM_JSON_OVERRIDE])
  {  

    if(jtok = jtok_sub.getObject()["Animation"].getObject()["TimeMs"])
    {
      SEGMENT_I(segment_index).single_animation_override.time_ms = jtok.getInt();
      data_buffer.isserviced++;
    }

    if(jtok = jtok_sub.getObject()["Animation_Off"].getObject()["TimeMs"])
    {
      SEGMENT_I(segment_index).single_animation_override_turning_off.time_ms = jtok.getInt();
      data_buffer.isserviced++;
    }

  }


  /**
   * @brief Phasing out to effect ColourType which is defined above in effect subcommands
   **/    
  #if FIRMWARE_VERSION_MAX(0, 240)
  if(jtok = obj[PM_JSON_COLOUR_TYPE])
  {
    ALOG_WRN(PSTR("Phase out ColourType"));
    if(jtok.isNum()){
      CommandSet_Effect_ColourTypeID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
  }
  #else
  #error "removed"
  #endif


  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB]){ // Range 0-100
    float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
    SEGMENT_I(segment_index).setBrightnessRGB( (uint8_t)value );
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessRGB());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_RGB_255]){
    SEGMENT_I(segment_index).setBrightnessRGB( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessRGB());
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT]){ // Range 0-100
    float percentage = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
    SEGMENT_I(segment_index).setBrightnessCCT( (uint8_t)percentage );
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessCCT());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_JSON_BRIGHTNESS_CCT_255]){
    SEGMENT_I(segment_index).setBrightnessCCT( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).getBrightnessCCT());
    data_buffer.isserviced++;
  }


  #if FIRMWARE_VERSION_MAX(0, 120)
  if(jtok = obj["TimeOnSecs"]){
    SEGMENT_I(segment_index).auto_timeoff.Start(jtok.getInt());
    ALOG_INF(PSTR("auto_timeoff %d"), SEGMENT_I(segment_index).auto_timeoff.Value());
  }
  #endif


  /**
   * @brief Segment Colours (ie 1-5)
   **/
  for(uint8_t colour_index=0;colour_index<6;colour_index++)
  {
    JsonParserObject seg_obj = 0;

    snprintf(buffer, sizeof(buffer), "SegColour%d", colour_index);

    // ALOG_HGL(PSTR("SEGMENT_I(segment_index).rgbcctcolors[colour_index] length %d"), RGBCCTCOLOURS_SIZE);    

    if(seg_obj = obj[buffer].getObject())
    {
      
      ALOG_COM(PSTR(D_LOG_PIXEL "SegColour[%d] = %s"), colour_index, buffer);

      if(jtok = seg_obj["RGBWC"])
      {
                
        if(jtok.isArray()){
          uint8_t array[5];
          uint8_t arrlen = 0;
          JsonParserArray arrobj = jtok;
          for(auto v : arrobj){
            if(arrlen > 5){ break; }
            array[arrlen++] = v.getInt();
          }
          SEGMENT_I(segment_index).rgbcctcolors[colour_index] = RgbcctColor(array[0],array[1],array[2],array[3],array[4]);
        }

        data_buffer.isserviced++;
        // ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K("Raw")), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getHue360());
      }
      
      #if FIRMWARE_VERSION_MAX(0, 230)
      if(jtok = seg_obj["Manual"]){ // Needs renamed, setting colour RGBCW directly
        ALOG_ERR(PSTR("Use RGBCW command"));
      }
      #endif

    
      if(jtok = seg_obj[PM_JSON_HUE]){ // Range 0-359
        CommandSet_SegColour_RgbcctColour_Hue_360(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_HUE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getHue360());
      }

      
      if(jtok = seg_obj[PM_JSON_SAT]){ // Range 0-100
        float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
        CommandSet_SegColour_RgbcctColour_Sat_255( (uint8_t)value, colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_SAT)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getSat255());
        data_buffer.isserviced++; 
      }else
      if(jtok = seg_obj[PM_JSON_SAT_255]){ // Full Range 0-255
        CommandSet_SegColour_RgbcctColour_Sat_255(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_SAT255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getSat255());
        data_buffer.isserviced++;
      }
    

      if(jtok = seg_obj[PM_JSON_CCT_PERCENTAGE]){ // Assume range 0-100    
        float value = mSupport::mapfloat(jtok.getFloat(), 0,100, CCT_MIN_DEFAULT,CCT_MAX_DEFAULT); // Using float so sub 1% transition is possible
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin( value, colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_PERCENTAGE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getCCT());
        data_buffer.isserviced++;
      }else
      if(jtok = seg_obj[PM_JSON_CCT_TEMP]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_PERCENTAGE)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getCCT());
        data_buffer.isserviced++;
      }
    

      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_RGB]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessRGB());
      }else
      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_RGB_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_RGB_255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessRGB());
      }

    
      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_CCT]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessCCT());
      }else
      if(jtok = seg_obj[PM_JSON_BRIGHTNESS_CCT_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_JSON_COMMAND_NVALUE_K(D_JSON_BRIGHTNESS_CCT_255)), SEGMENT_I(segment_index).rgbcctcolors[colour_index].getBrightnessCCT());
      }
     
    }
  
  }

  #ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER

  if(jtok = obj["Sequencer"].getObject()["LoadSet"])
  {
    Load_Sequencer(jtok.getInt());
  }
  if(jtok = obj["Sequencer"].getObject()["EnableTimeRestraints"])
  {
    sequencer_runtime.Enable_TimeRestraints = jtok.getInt();
  }
  if(jtok = obj["Sequencer"].getObject()["ItemTime"])
  {
    SetSequenceTimes(jtok.getInt());
  }  

  if(jtok = obj["Sequencer"].getObject()["LimitFlashing"])
  {
    sequencer_runtime.remote_openhab_limit_flashing = jtok.getInt();
  }  
  #endif // ENABLE_FEATURE_LIGHTING__SEQUENCER
 

  /***********************************************************************************************************************************************************************************
   *********************************************************************************************************************************************************************************** 
  ******* NOTE: Debug commands only, not part of official commands ****************************************************************************************************************** 
  ************************************************* *********************************************************************************************************************************
  ***********************************************************************************************************************************************************************************/

  #ifdef ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  if(jtok = obj["Debug"])
  {

    JsonParserObject jObj_debug = jtok.getObject();

    if(jtok = jObj_debug["Option0"])
    { 
      SEGMENT_I(segment_index).params_internal.aux0 = jtok.getInt();  
    }
    if(jtok = jObj_debug["Option1"])
    { 
      SEGMENT_I(segment_index).params_internal.aux1 = jtok.getInt();  
    }
    if(jtok = jObj_debug["Option2"])
    { 
      SEGMENT_I(segment_index).params_internal.aux2 = jtok.getInt();  
    }
    if(jtok = jObj_debug["Option3"])
    {
      SEGMENT_I(segment_index).params_internal.aux3 = jtok.getInt();  
    }

    if(jtok = jObj_debug["Options"]){ 
      if(jtok.isArray()){
        uint8_t array[4];
        uint8_t arrlen = 0;
        JsonParserArray arrobj = jtok;
        for(auto v : arrobj) {
          if(arrlen > 4){ break; }
          switch(arrlen)
          {
            case 0: SEGMENT_I(segment_index).params_internal.aux0 = v.getInt(); break;
            case 1: SEGMENT_I(segment_index).params_internal.aux1 = v.getInt(); break;
            case 2: SEGMENT_I(segment_index).params_internal.aux2 = v.getInt(); break;
            case 3: SEGMENT_I(segment_index).params_internal.aux3 = v.getInt(); break;
          }
          arrlen++;        
        }
        SEGMENT_I(segment_index).reset = false; // Do not reset if options have been set
        ALOG_COM(PSTR(D_LOG_PIXEL "AuxOptions Segment[%d] flag%d = %d,%d,%d,%d"), segment_index, SEGMENT_I(segment_index).reset, SEGMENT_I(segment_index).params_internal.aux0, SEGMENT_I(segment_index).params_internal.aux1, SEGMENT_I(segment_index).params_internal.aux2, SEGMENT_I(segment_index).params_internal.aux3 );
        data_buffer.isserviced++;
      }
    }

    #ifdef ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    if(jtok = obj["Bus"].getObject()["Init"])
    { 
      doInitBusses = true;
      TestCode_AddBus1();   
    }
    if(jtok = obj["Bus"].getObject()["Bus16Test"])
    { 
      doInitBusses = true;
      TestCode_Add16ParallelBus1();
    }

    #endif // ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    
      
    if(jtok = jObj_debug["addSegment"]){

      uint8_t id = jtok.getInt();

      uint16_t start = id;
      uint16_t stop = 100;//STRIP_SIZE_MAX;

      Serial.println();

      ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

      Segment_AppendNew(start, stop, jtok.getInt());

    }


    if(jtok = jObj_debug["removeSegment"]){  
      
      uint8_t start = 0;
      uint8_t stop = 100;
      uint8_t id = jtok.getInt();
      uint8_t force = 0;

      Serial.println();

      segments[jtok.getInt()].stop = 0; // to disable segment

      ALOG_INF(PSTR("getSegmentsNum() %d|%d %d"), id, getSegmentsNum(), segments[jtok.getInt()].stop);

      // remove all inactive segments (from the back)
      if(id==255){ purgeSegments(true); }
      else{        purgeSegments(false); }

    }

    if(jtok = jObj_debug["removeSegment2"]){  

      uint8_t start = 0;
      uint8_t stop = 100;
      uint8_t id = jtok.getInt();
      uint8_t force = 0;

      Serial.println();

      segments[jtok.getInt()].stop = 0; // to disable segment

      ALOG_INF(PSTR("getSegmentsNum() %d|%d"), id, getSegmentsNum());

      // remove all inactive segments (from the back)
      int deleted = 0;
      if (segments.size() <= 1) return 0;
      for (size_t i = segments.size()-1; i > 0; i--)
        if (segments[i].stop == 0 || force) {
          DEBUG_PRINT(F("Purging segment segment: ")); DEBUG_PRINTLN(i);
          deleted++;
          segments.erase(segments.begin() + i);
        }else{
          
        }
      if (deleted) {
        
        segments.shrink_to_fit();
        if (_mainSegment >= segments.size()){
          
          setMainSegmentId(0);
        }
      }

    }


  } // end Debug options

  #endif // ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS options above to be moved into debug methods

  /**
   * @brief 
   * # Issue : Caused effects to reset when non lighting commands happened
   * Fix is to only reset effects if lighting commands are updated
   */
  if(isserviced_start_count != data_buffer.isserviced)
  {
    ALOG_INF(PSTR("isserviced_start_count %d data_buffer.isserviced %d"), isserviced_start_count, data_buffer.isserviced);
    // If segment commands updated, some effects may need reset
    SEGMENT_I(segment_index).call = 0; 
  }

  //  If command source was webui, then override changes
  if(data_buffer.flags.source_id == DATA_BUFFER_FLAG_SOURCE_WEBUI)
  {
    // segment_animation_override.time_ms = 100;
  }
  
} // END PARSE COMMANDS


        



#ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
/**
 * @brief Method for setting colour of rgbcct palette using a value within a range. Use case: colour map ambient temperature into colour hue
 * 
 * @param array_val   : [min_point, current, max_point] . alternate style may use multiple "inflection" points (ie not linear with multi slope conversions)
 * @param array_length 
 * @param style_index : default 0   : 10 to 20 celsius is cyan 180 to red 0
 * @param palette_id  : default 255 : use the active palette id
 */
void mAnimatorLight::CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index, uint8_t palette_id)
{

  for(uint8_t ii=0;ii<array_length;ii++){ Serial.printf("%d=%f\n\r", ii, array_val[ii]); }

  switch(style_index)
  {
    /**
     * @brief 
     * In this case, 10 to 20 Celsius will be from hue 180 to hue 0 (cyan to red)
       Then above or below those ranges, will remain fixed at the red/cyan
     */
    default:
    case 0:
    {

      float hue_f = mSupport::mapfloat(array_val[1], array_val[0], array_val[2], 180.0f, 0.0f);
        hue_f = constrain(hue_f, 0.0f, 180.0f);
      uint16_t hue_i = (uint16_t)hue_f;

      // CommandSet_ActiveSolidPalette_Hue_360(hue_i);
      // CommandSet_ActiveSolidPalette_Sat_255(255);

    }
    break;
    /**
     * @brief 
     * In this case, 10 to 20 Celsius will be from hue 180 to hue 0 (cyan to red)
       Then above or below those ranges, will go slightly pink and dark blue as solid colours
     */
    case 1:
    
      float hue_f = mSupport::mapfloat(array_val[1], array_val[0], array_val[2], 180.0f, 0.0f);

      Serial.println(hue_f);

      if(array_val[1] < array_val[0]) // force dark blue
      {
        ALOG_INF(PSTR("Force Dark Blue"));
        hue_f = 240.0f;
      }else
      if(array_val[1] > array_val[2]) // force slightly hot pink
      {
        ALOG_INF(PSTR("Force Hot Pink"));
        hue_f = 350.0f;
      }
      else // Just constrain within range
      {
        ALOG_INF(PSTR("Constrain"));
        hue_f = constrain(hue_f, 0.0f, 180.0f);
      }

      uint16_t hue_i = (uint16_t)hue_f;

      ALOG_COM(PSTR("Hue = %d"), hue_i);

      // CommandSet_ActiveSolidPalette_Hue_360(hue_i);
      // CommandSet_ActiveSolidPalette_Sat_255(255);


    break;
  }

}
#endif // ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE

#ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
void mAnimatorLight::CommandSet_AnimationModeID(uint8_t value){

  uint8_t segment_index = 0;

  char buffer[60];

  SEGMENT_I(0).animation_mode_id = value;  // this is wrong
          
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
  #endif

}
const char* mAnimatorLight::GetAnimationModeName(char* buffer, uint16_t buflen){
  return GetAnimationModeNameByID(  SEGMENT_I(0).animation_mode_id, buffer, buflen);
}
const char* mAnimatorLight::GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen){
  switch(id){
    default:
    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    case ANIMATION_MODE_REALTIME_ADALIGHT:     memcpy_P(buffer, PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR, sizeof(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)); break;
    #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
      case ANIMATION_MODE_NOTIFICATIONS_ID: memcpy_P(buffer, PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR,sizeof(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)); break;
    #endif
    case ANIMATION_MODE__EFFECTS:       memcpy_P(buffer, PM_ANIMATION_MODE_EFFECTS_NAME_CTR, sizeof(PM_ANIMATION_MODE_EFFECTS_NAME_CTR)); break;
    #ifdef ENABLE_PIXEL_FUNCTION_WLED_EFFECTS
    case ANIMATION_MODE_WLED_ID:       memcpy_P(buffer, PM_ANIMATION_MODE_WLED_NAME_CTR, sizeof(PM_ANIMATION_MODE_WLED_NAME_CTR)); break;
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    case ANIMATION_MODE_MANUAL_SETPIXEL_ID:         memcpy_P(buffer, PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR , sizeof(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR)); break;
    #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  }
  return buffer;
} 
int8_t mAnimatorLight::GetAnimationModeIDbyName(const char* c){

  if(*c=='\0'){
    return -1;
  }
  #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
  if(strcmp_P(c,PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)==0){  return ANIMATION_MODE_NOTIFICATIONS_ID; }
  #endif
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  if(strstr_P(c,PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR)){      return ANIMATION_MODE_REALTIME_ADALIGHT; }
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  if(strcmp_P(c,PM_ANIMATION_MODE_EFFECTS_NAME_CTR)==0){        return ANIMATION_MODE__EFFECTS; }
  // #ifdef ENABLE_PIXEL_FUNCTION_WLED_EFFECTS
  // if(strcmp_P(c,PM_ANIMATION_MODE_EFFECTS_NAME_CTR)==0){        return ANIMATION_MODE_WLED_ID; }
  // #endif
  return -1;
}
#endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE


void mAnimatorLight::CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index)
{
  SEGMENT_I(segment_index).intensity = value;
}


void mAnimatorLight::CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index)
{
  SEGMENT_I(segment_index).speed = value;
}


void mAnimatorLight::CommandSet_Effect_ColourTypeID(uint8_t id, uint8_t segment_index)
{  
  SEGMENT_I(segment_index).colour_type__used_in_effect_generate = (ColourType)id;
  ALOG_INF(PSTR("ColourType = %d"),SEGMENT_I(segment_index).colour_type__used_in_effect_generate);
}


void mAnimatorLight::CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length)
{
  mPaletteI->addCustomPalette(index, data, data_length, encoding);
}


void mAnimatorLight::CommandSet_PaletteID(uint16_t value, uint8_t segment_index)
{
  char buffer[50];
  SEGMENT_I(segment_index).palette_id = value < mPaletteI->GetPaletteListLength() ? value : 0;  
  _segment_index_primary = segment_index;
  SEGMENT.LoadPalette(segments[segment_index].palette_id);
}


void mAnimatorLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index)
{    
  SEGMENT_I(segment_index).cycle_time__rate_ms = value;
  ALOG_DBM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TRANSITION, D_JSON_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms );  
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setHue360(hue_new);      
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setSat255(sat_new);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setCCT_Kelvin(ct);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessRGB(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).rgbcctcolors[colour_index].setBrightnessCCT(brightness);    
}



#endif //USE_MODULE_LIGHTS_ANIMATOR




