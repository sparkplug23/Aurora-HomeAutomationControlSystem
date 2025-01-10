#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

/**
 * @brief Ideas
 * 
 * segment should be controllable by name, and only when matched. So instead of "Segment0":{} we should be able to do "SegName":{"Name":"something", the rest} and pass the {} like we already do
 * Though, this would work since each name needs to be different. Maybe "Seg_Name" and the name part gets pulled out from _ to EOL?
 * 
 * Another idea is being able to set a "HeatmapPalette"
 * 
 * "HeatmapPalette":{
 *    "Data":[float array],
 *    "Gradient":[0 to 255 inflection points],
 *    "PaletteID":"Jet" // so Jet will be default, otherwise it takes a full gradient palette and maps it by loading as CRGB16Palette
 * }
 * 
 * Doing this vs generating the colours in openhab would make it more flexible for the same data to easily be used on different devices by just changing the Gradient.
 * Should this also be part effect?
 * 
 */
void mAnimatorLight::parse_JSONCommand(JsonParserObject obj)
{

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::Start"));

  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];
  uint16_t isserviced_start_count = data_buffer.isserviced;

  if(isserviced_start_count != data_buffer.isserviced) //ie something was parsed inside this function
  {
    tkr_anim->force_update();
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
        Segment_AppendNew(0, 16, segment_i);
        ALOG_INF(PSTR("Created new segment%02d %dB (T%dB)"), segment_i, segments.size()-seg_size, segments.size());
      }
      
      subparse_JSONCommand(jtok.getObject(), segment_i);

      segments_found++;

    }
  }

  /**
   * @brief 
   * When commands are not inside a {"Segment#":{commands}} then assume direct control of first segment (ie 0)
   **/
  if(segments_found == 0)
  {
    ALOG_INF(PSTR(D_LOG_NEO "Assume main segment"));
    subparse_JSONCommand(obj, 0); // Legacy commands
  }

  ALOG_DBM(PSTR(D_LOG_LIGHT D_TOPIC "mAnimatorLight::parse_JSONCommand::End"));

}




/**
 * @brief Allows new segment controls, by calling this function for each segment.
 *        Calling it without any set index, will default to segment of 0 index and turn the other segments off 
 * @param obj 
 */
void mAnimatorLight::subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int16_t tmp_id = 0;
  char buffer[50];

  uint16_t isserviced_start_count = data_buffer.isserviced; // to know if anything was serviced in this function

  /**
   * @brief Exit if segment has not been created to stop errors
   **/
  if(!segments.size()){ 
    ALOG_ERR(PSTR("No segments created"));
    return; }


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

  
  if(jtok = obj[PM_COLOUR_PALETTE])
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
    ALOG_INF( PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(SEGMENT_I(segment_index).palette_id, buffer, sizeof(buffer)) );
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_PIXELRANGE])
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

      if(arrobj.size() == 4)
      {
        SEGMENT_I(segment_index).startY = arrobj[3].getInt();
        SEGMENT_I(segment_index).stopY  = arrobj[4].getInt();
      }

      ALOG_INF( PSTR(D_LOG_PIXEL "PixelRange = [%d,%d]"), SEGMENT_I(segment_index).start, SEGMENT_I(segment_index).stop );
      data_buffer.isserviced++;
    }
  }


  /*************************************************************************
   *** {"Effects":{X:Y}}
   *************************************************************************/
  if(jtok_sub = obj[PM_EFFECTS])
  {    
    JsonParserObject jobj = jtok_sub.getObject();

    if(jtok = jobj[PM_FUNCTION])
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
      ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_SVALUE_K(D_FUNCTION)), GetFlasherFunctionName(buffer, sizeof(buffer)));
      data_buffer.isserviced++;
    }


    /**
    EFFECT_SUBTYPE removed, replaced by colourtype
    **/
    if(jtok = jobj[PM_EFFECT_COLOUR_TYPE])
    {
      CommandSet_Effect_ColourTypeID(jtok.getInt(), segment_index);
      data_buffer.isserviced++;
    }
    
    if(jtok = jobj[PM_INTENSITY])
    { 
      CommandSet_Effect_Intensity(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_PIXEL D_COMMAND_2KEY_NVALUE_K(D_EFFECTS, D_INTENSITY)), jtok.getInt() );
      data_buffer.isserviced++;
    }
    

    if(jtok = jobj[PM_SPEED])
    { 
      CommandSet_Effect_Speed(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_PIXEL D_COMMAND_2KEY_NVALUE_K(D_EFFECTS, D_SPEED)), jtok.getInt() );
      data_buffer.isserviced++;
    }
    

    if(jtok = jobj[PM_REVERSE])
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_REVERSED, jtok.getInt());  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_REVERSE)), SEGMENT_I(segment_index).reverse);
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_MIRROR])
    { 
      SEGMENT_I(segment_index).setOption(SEG_OPTION_MIRROR, jtok.getInt());  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_MIRROR)), SEGMENT_I(segment_index).mirror);
      data_buffer.isserviced++;
    }


    if(jtok = jobj[PM_GROUPING])
    { 
      SEGMENT_I(segment_index).grouping = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_GROUPING)), SEGMENT_I(segment_index).grouping);
      data_buffer.isserviced++;
    }
    
    if(jtok = jobj["GroupingPerc"])
    {
      uint8_t percentage = jtok.getInt(); 
      SEGMENT_I(segment_index).grouping = map(percentage,0,100,0,SEGMENT_I(segment_index).length());    
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_GROUPING)), SEGMENT_I(segment_index).grouping);
      data_buffer.isserviced++;
    }

    
    if (jtok = jobj[PM_DECIMATE])
    {
      SEGMENT_I(segment_index).decimate = jtok.getInt();  
      ALOG_COM(PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_DECIMATE)), SEGMENT_I(segment_index).decimate);
      data_buffer.isserviced++;
    }

    if (jtok = jobj["DecimatePerc"])
    {
      uint8_t percentage = jtok.getInt();  // Get the percentage from the JSON input
      ALOG_INF(PSTR("TEST %d"), percentage);

      if (percentage == 0)
      {
        // Disable decimation
        SEGMENT_I(segment_index).decimate = 0;
        ALOG_INF(PSTR(D_LOG_PIXEL  "Decimation disabled by DecimatePerc = 0."));
        data_buffer.isserviced++;
      }
      else if (percentage > 0 && percentage <= 100)
      {
        // Calculate decimate as a replication factor
        SEGMENT_I(segment_index).decimate = 100 / percentage;
        ALOG_INF(PSTR(D_LOG_PIXEL  "DecimatePerc of %d is decimate %d"), percentage, SEGMENT_I(segment_index).decimate);
        data_buffer.isserviced++;
      }
      else
      {
        ALOG_ERR(PSTR(D_LOG_PIXEL  "Invalid DecimatePerc value. Must be between 0 and 100."));
      }
    }




    if(jtok = jobj[PM_SPACING])
    { 
      SEGMENT_I(segment_index).spacing = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_SPACING)), SEGMENT_I(segment_index).spacing);
      data_buffer.isserviced++;
    }
      
    if(jtok = jobj[PM_OFFSET])
    { 
      SEGMENT_I(segment_index).offset = jtok.getInt();  
      ALOG_COM( PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_EFFECTS D_OFFSET)), SEGMENT_I(segment_index).offset);
      data_buffer.isserviced++;
    }
      

    if(jtok = jobj[PM_RATE]){ // default to secs
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt()*1000, segment_index);
      ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_RATE)), SEGMENT_I(segment_index).cycle_time__rate_ms);
      data_buffer.isserviced++;
    }else
    if(jtok = jobj[PM_RATE_MS]){
      CommandSet_Animation_Transition_Rate_Ms(jtok.getInt(), segment_index);
      ALOG_COM( PSTR(D_LOG_LIGHT D_COMMAND_NVALUE_K(D_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms);  
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
          ALOG_INF(PSTR(D_LOG_PIXEL "Params %d = %d,%d,%d,%d"), segment_index, params_user[0], params_user[1], params_user[2], params_user[3]);
          data_buffer.isserviced++;
        }
      }
    }    

    if (jtok = jobj["Custom1"]) 
    {
      SEGMENT_I(segment_index).custom1 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Custom1 %d"), SEGMENT_I(segment_index).custom1 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Custom2"]) 
    {
      SEGMENT_I(segment_index).custom2 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Custom2 %d"), SEGMENT_I(segment_index).custom2 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Custom3"]) 
    {
      SEGMENT_I(segment_index).custom3 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Custom3 %d"), SEGMENT_I(segment_index).custom3 );
      data_buffer.isserviced++;
    }

    if (jtok = jobj["Check1"]) 
    {
      SEGMENT_I(segment_index).check1 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Check1 %d"), SEGMENT_I(segment_index).check1 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Check2"]) 
    {
      SEGMENT_I(segment_index).check2 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Check2 %d"), SEGMENT_I(segment_index).check2 );
      data_buffer.isserviced++;
    }
    if (jtok = jobj["Check3"]) 
    {
      SEGMENT_I(segment_index).check3 = jtok.getInt();
      ALOG_INF(PSTR(D_LOG_PIXEL "Check3 %d"), SEGMENT_I(segment_index).check3 );
      data_buffer.isserviced++;
    }



  } // PM_EFFECTS


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

  #ifdef ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES
/**
 * @brief Construct a new if object
 * {
  "HeatmapPalettes": {
    "Data": [1,2,3,4],
    "Gradient": [0,100,200,255],
    "Max":4,
    "Min":1,
    "Palette":"Jet 16"
  }
}
 * 
 */
  if (jtok = obj["HeatmapPalettes"]) {
      JsonParserToken jtok_sub = 0;
      
      // Read float array (data points)
      std::vector<float> heatmap_floats;
      if (jtok_sub = jtok.getObject()["Data"]) {
          if (jtok_sub.isArray()) {
              JsonParserArray arrobj = jtok_sub;
              for (int i = 0; i < arrobj.size(); ++i) {
                  heatmap_floats.push_back(arrobj[i].getFloat());
              }
              ALOG_INF(PSTR("Heatmap data length %d"), heatmap_floats.size());
          }
      }
      
      // Read Gradient mapping (array of 0-255)
      // std::vector<uint8_t> gradient_map;
      // if (jtok_sub = jtok.getObject()["Gradient"]) {
      //     if (jtok_sub.isArray()) {
      //         JsonParserArray arrobj = jtok_sub;
      //         for (int i = 0; i < arrobj.size(); ++i) {
      //             gradient_map.push_back(arrobj[i].getInt());
      //         }
      //         ALOG_INF(PSTR("Gradient length %d"), gradient_map.size());
      //     }
      // }

      uint8_t gradient_index = mSupport::mapfloat()
      
      // Read Max and Min values for the heatmap
      float max_value = 1.0f, min_value = 0.0f;
      if (jtok_sub = jtok.getObject()["Max"]) {
          max_value = jtok_sub.getFloat();
      }
      if (jtok_sub = jtok.getObject()["Min"]) {
          min_value = jtok_sub.getFloat();
      }
      
      // Read Palette to copy from
      uint8_t palette_index = 0;
      if(jtok_sub = jtok.getObject()["Palette"])
      {
        if(jtok_sub.isStr())
        {
          if((tmp_id=GetPaletteIDbyName((char*)jtok_sub.getStr()))>=0)
          {
            palette_index = tmp_id;
          }
        }else
        if(jtok_sub.isNum()){
          palette_index = jtok_sub.getInt();
        }
        
        ALOG_INF(PSTR("Using Palette Index %d"), palette_index);
        data_buffer.isserviced++;
      }

      /**
       * @brief Since we are going to copy from an existing palette, we want to get the encoding, and then get the data.
       * Since encoding is not always used, the way forward might be to :
       * ** load palette
       * ** access what was loaded, copy/modify into CustomPalette
       * ** reload and set palette as the custom option
       */
      mPalette::PALETTE_ENCODING_DATA encoding =  mPaletteI->findPaletteEncoding(palette_index);
      if(encoding.encoded_as_crgb_palette_16)
      {
        ALOG_INF(PSTR("Encoded as CRGB16Palette"));
      }
      if( // Switch to bit masking
        (encoding.red_enabled)||
        (encoding.green_enabled)||
        (encoding.blue_enabled)||
        (encoding.white_cold_enabled)||
        (encoding.white_warm_enabled)
      ){
        ALOG_INF(PSTR("Encoded as RGBWW"));
      }
      if(encoding.index_gradient)
      {
        ALOG_INF(PSTR("Encoded with gradient that I need to think of"));
      }
      Serial.println(encoding.data, BIN);


      // if()

      // // Get the encoding for the heatmap palette
      // mPalette::PALETTE_ENCODING_DATA encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX};
      // if (jtok_sub = jtok.getObject()["Encoding"]) {
      //     if (jtok_sub.isStr()) {
      //         if (strcmp(jtok_sub.getStr(),"RGB")==0) {
      //             encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX};
      //         } else if (strcmp(jtok_sub.getStr(),"gRGB")==0) {
      //             encoding = {PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT};
      //         }
      //     } else {
      //         uint16_t enc = jtok_sub.getInt();
      //         encoding = {enc};
      //     }
      //     ALOG_DBG(PSTR("Encoding %d"), encoding);
      // }

      // // Process the heatmap data points and assign colors
      // std::vector<uint8_t> final_palette_data;

      // for (size_t i = 0; i < heatmap_floats.size(); ++i) {
      //     float normalized_value = (heatmap_floats[i] - min_value) / (max_value - min_value);
      //     uint8_t gradient_index = gradient_map[i]; // This assumes a matching 1-to-1 relation of float to gradient

      //     // If gradient_index is a float between 0-1, scale it to 0-255
      //     gradient_index = uint8_t(normalized_value * 255);

      //     // Get the corresponding color from the palette
      //     uint32_t color = GetPaletteColour(palette_index, gradient_index, encoding);
          
      //     // Add the color to the final palette data (assuming RGB, 3 bytes per color)
      //     final_palette_data.push_back((color >> 16) & 0xFF); // Red
      //     final_palette_data.push_back((color >> 8) & 0xFF);  // Green
      //     final_palette_data.push_back(color & 0xFF);         // Blue
      // }

      // // Command to set the heatmap palette using the computed final_palette_data
      // CommandSet_HeatmapPalette(palette_index, encoding.data, final_palette_data);
  }




  #endif // ENABLE_DEVFEATURE_LIGHT__HEATMAP_PALETTES
  

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

      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_HUE)), SEGMENT_I(0).segcol[0].getHue360());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(0).segcol[0].getBrightnessRGB());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_HUE)), SEGMENT_I(1).segcol[0].getHue360());
      // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(1).segcol[0].getBrightnessRGB());


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
    if(jtok = obj[PM_ANIMATIONMODE]){
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
      ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
      #endif // ENABLE_LOG_LEVEL_DEBUG
    }
    #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
  
  #endif // FIRMWARE_VERSION_MIN
  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  if(jtok = obj[PM_RGB_CLOCK].getObject()[PM_MANUAL_NUMBER]){
    lcd_display_show_number = jtok.getInt();
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_COMMAND_NVALUE_K(D_MANUAL_NUMBER)), lcd_display_show_number);
  }
  
  if(jtok = obj[PM_RGB_CLOCK].getObject()["ManualString"]){
    strcpy(lcd_display_show_string, jtok.getStr());
    // CommandSet_Palette_Generation_Randomise_Brightness_Mode(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_PIXEL  D_COMMAND_SVALUE_K("ManualString")), lcd_display_show_string);
  }
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
  /**
   * @brief When these are commanded, the "animation_mode" is switched and the setpixel will be set here directly
   */
  if(jtok = obj["MQTTPixel"])
  {

    JsonParserToken jtok_sub = 0;

    realtimeMode = ANIMATION_MODE__REALTIME_MQTT_SETPIXEL;
    
    // Method 1: RGB control
    // if(jtok_sub = jtok.getObject()["Index"])
    // {
    //   index = jtok_sub.getInt();
    //   ALOG_DBM(PSTR("Index %d"), index);
    // }

    // Method 2: Set the "on pixels", which uses the configured palette on repeat
    uint8_t index = 0;
    if (jtok_sub = jtok.getObject()["OnPixels"]) {
        if (jtok_sub.isArray()) {
            ALOG_COM(PSTR("is ARRAY"));

            SEGMENT.fill(0); // Clear all to off

            uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch brightness
            ALOG_INF(PSTR("brightness %d"), brightness);

            RgbcctColor colour;
            uint16_t pixel = 0;
            JsonParserArray array = jtok_sub;

            JsonParserToken token = array;
            token.nextOne(); // Skip the array itself and move to the first element

            uint16_t count = 0; // Keep track of processed items
            while (token.isValid()) { // Process tokens while valid
                if (token.isNum()) { // Ensure the token is a number
                    int pixelIndex = token.getInt();
                    ALOG_INF(PSTR("OnPixel %d (processed %d)"), pixelIndex, count);

                    // Get the color from the palette
                    colour = SEGMENT.GetPaletteColour(pixel++, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);

brightness = 255;
// colour = RgbcctColor(255,255,255,255,255);
                    // Set the pixel color with brightness
                    SEGMENT.SetPixelColor(pixelIndex, colour.WithBrightness(brightness));

                    count++;
                } else {
                    ALOG_ERR(PSTR("Invalid token type inside array"));
                }

                // Move to the next token
                token.nextOne();
            }

            ALOG_INF(PSTR("Processed %d pixels in total"), count);

            SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
            SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

            show();
        }
    }


  }
  #endif // ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
  #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL
/**
 * @brief Handles MQTT commands for setting pixel arrays with row mappings.
 * 
 * {
  "BrightnessRGB": 255,
  "Effects":{
    "Param0":1605,
    "Param1":1610
  },
  "MQTTPixel": {
    "OnPixels": [
      [1605],   // left side
      [1606,1607,1608,1609,1611], //centres
      [2100] //right side
    ]
  }
}


{
  "BrightnessRGB": 255,
  "MQTTPixel": {
    "OnPixels": [
        [0, 31, 63, 98, 134, 173, 214, 257, 302, 352, 400, 451, 502, 553, 604, 658, 710, 763, 815, 867, 920, 971, 
        1022, 1071, 1119, 1168, 1215, 1260, 1303, 1345, 1385, 1423, 1461, 1498, 1535, 1572, 1607, 1640, 1671, 1702, 1732, 1759, 1785, 1820, 1841, 1860, 
        1877, 1892, 1904, 1913, 1922, 1930, 1938, 1945, 1952, 1958, 1964, 1969, 1974, 1978],
        [15, 47, 80, 116, 153, 193, 235, 279, 327, 376, 425, 476, 527, 578, 631, 684, 736, 789, 841, 893, 945, 996, 1046, 1095, 1144, 1191, 1237, 1281, 
        1324, 1365, 1404, 1442, 1480, 1516, 1553, 1589, 1623, 1655, 1686, 1717, 1745, 
        1772, 1802, 1829, 1850, 1868, 1884, 1898, 1917, 1926, 1934, 1941, 1948, 1955, 1961, 1967, 1971],
        [30, 62, 97, 133, 172, 213, 256, 301, 351, 399, 450, 501, 552, 603, 657, 709, 762, 814, 866, 919, 970, 1021, 1070, 1118, 1167, 1214, 1259, 1302, 1344,
        1384, 1422, 1460, 1497, 1534, 1571, 1606, 1639, 1670, 1701, 1731, 1758, 1784, 1819, 1840, 1859, 
        1876, 1891, 1903, 1912, 1921, 1929, 1937, 1944, 1951, 1957, 1963, 1968, 1973, 2099]
    ]
  }
}



 */
if (jtok = obj["MQTTPixelArrays"]) {

    JsonParserToken jtok_sub = 0;

    ALOG_INF(PSTR("MQTTPixelArrays"));

    realtimeMode = ANIMATION_MODE__REALTIME_MQTT_SETPIXEL;

    if (jtok.isArray()) {
        ALOG_INF(PSTR("Processing MQTTPixelArrays"));

        SEGMENT.fill(0); // Clear all pixels

        uint8_t brightness = SEGMENT.getBrightnessRGB_WithGlobalApplied(); // Prefetch brightness

        JsonParserArray arrayOfArrays = jtok;
        JsonParserToken outerToken = arrayOfArrays;

        outerToken.nextOne(); // Move to the first array in the array of arrays
        ALOG_INF(PSTR("Iterating over outer array of MQTTPixelArrays"));

        uint16_t outerCount = 0; // Track outer array items processed
        uint16_t paletteIndex = 0; // Start from palette index 0

        while (outerToken.isValid()) { // Iterate through the outer array
            ALOG_INF(PSTR("Outer token type: %d"), outerToken.getType());

            // Process only valid arrays
            if (outerToken.isArray()) {
                JsonParserArray innerArray = outerToken;
                JsonParserToken innerToken = innerArray;

                innerToken.nextOne(); // Move to the first element in the inner array

                int startPixel = -1, midPixel = -1, endPixel = -1;
                uint16_t pixelCount = 0;

                ALOG_INF(PSTR("Processing inner array"));

                // Process inner array tokens to get start, middle, and end pixels
                while (innerToken.isValid()) {
                    if (innerToken.isNum()) { // Check if the token is a number
                        int pixelIndex = innerToken.getInt();
                        ALOG_INF(PSTR("Inner token type: %d, value: %d"), innerToken.getType(), pixelIndex);

                        if (pixelCount == 0)
                            startPixel = pixelIndex; // Start pixel
                        else if (pixelCount == 1)
                            midPixel = pixelIndex; // Mid (center) pixel
                        else if (pixelCount == 2)
                            endPixel = pixelIndex; // End pixel

                        pixelCount++;
                    }

                    // Stop processing after three valid elements
                    if (pixelCount == 3)
                        break;

                    innerToken.nextOne(); // Move to the next token in the inner array
                }

                if (pixelCount == 3) { // Ensure all three points are provided
                    ALOG_INF(PSTR("Setting row: Start %d, Mid %d, End %d using palette index %d"),
                             startPixel, midPixel, endPixel, paletteIndex);

                    // Set pixels for the row
                    for (int i = startPixel; i <= endPixel; i++) {
                        RgbcctColor colour;

                        if (i == midPixel) {
                            // Center pixel is white
                            colour = RgbcctColor(255, 255, 255, 255, 255);
                        }else
                        if (i == endPixel) {
                            // Right pixel is white, is easier contrast between start of row and end of row
                            colour = RgbcctColor(255, 255, 255, 255, 255);
                        } else {
                            // Get color from the palette using the current palette index
                            colour = SEGMENT.GetPaletteColour(paletteIndex, PALETTE_INDEX_SPANS_SEGLEN_ON, PALETTE_WRAP_OFF, PALETTE_DISCRETE_ON, NO_ENCODED_VALUE);
                        }

                        // Apply brightness and set the pixel
                        SEGMENT.SetPixelColor(i, colour.WithBrightness(brightness));
                    }

                    paletteIndex++; // Increment palette index for the next row

                } else {
                    ALOG_ERR(PSTR("Invalid row format. Expected exactly 3 pixels (start, mid, end). Parsed Count=%d, Start=%d, Mid=%d, End=%d"),
                             pixelCount, startPixel, midPixel, endPixel);
                }

                outerCount++;
            } else {
                ALOG_INF(PSTR("Skipping non-array token in outer array: Type=%d"), outerToken.getType());
            }

            outerToken.nextOne(); // Move to the next token in the outer array
        }

        ALOG_INF(PSTR("Processed %d rows in total"), outerCount);

        SEGMENT.cycle_time__rate_ms = FRAMETIME_MS;
        SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR();

        show();
    } else {
        ALOG_ERR(PSTR("MQTTPixelArrays is not a valid array"));
    }
}
#endif // ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL




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
      ALOG_COM(PSTR(D_COMMAND_NVALUE_K("Method")), style_index);
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
  

  if(jtok_sub = obj[PM_OVERRIDE])
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
  if(jtok = obj[PM_COLOUR_TYPE])
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


  if(jtok = obj[PM_BRIGHTNESS_RGB]){ // Range 0-100
    SEGMENT_I(segment_index).setBrightnessRGB( map(jtok.getInt(), 0,100, 0,255) );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessRGB());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_RGB_255]){
    SEGMENT_I(segment_index).setBrightnessRGB( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessRGB());
    data_buffer.isserviced++;
  }


  if(jtok = obj[PM_BRIGHTNESS_CCT]){ // Range 0-100
    SEGMENT_I(segment_index).setBrightnessCCT( map(jtok.getInt(), 0,100, 0,255) );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessCCT());
    data_buffer.isserviced++;
  }else
  if(jtok = obj[PM_BRIGHTNESS_CCT_255]){
    SEGMENT_I(segment_index).setBrightnessCCT( jtok.getInt() );
    ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).getBrightnessCCT());
    data_buffer.isserviced++;
  }


/**
 * @brief 
 * This should be extended to
 * "TimeOnSecs"
 * "TimeOff":"HHMM" as hh and mm of the day
 * 
 */
  if(jtok = obj["Preset"].getObject()["Load"]){ // Range 0-100
  //   SEGMENT_I(segment_index).setBrightnessCCT( map(jtok.getInt(), 0,100, 0,255) );
  //   ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).getBrightnessCCT());
  //   data_buffer.isserviced++;
  // }
    uint8_t ps = jtok.getInt();//presetCycCurr;
    // if (root["win"].isNull() && getVal(root["ps"], &ps, 0, 0) && ps > 0 && ps < 251 && ps != currentPreset) {
      // b) preset ID only or preset that does not change state (use embedded cycling limits if they exist in getVal())
      // presetCycCurr = ps;
      #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
      unloadPlaylist();          // applying a preset unloads the playlist
      #endif
      applyPreset(ps, CALL_MODE_DIRECT_CHANGE); // async load from file system (only preset ID was specified)
      // return stateResponse;
    // }

  }

  if(jtok = obj["TimeOnSecs"]){
    SEGMENT_I(segment_index).auto_timeoff.Start(jtok.getInt());
    ALOG_INF(PSTR("auto_timeoff %d"), SEGMENT_I(segment_index).auto_timeoff.Value());
  }


  /**
   * @brief Segment Colours (ie 1-5)
   **/
  for(uint8_t colour_index=0;colour_index<6;colour_index++)
  {
    JsonParserObject seg_obj = 0;

    snprintf(buffer, sizeof(buffer), "SegColour%d", colour_index);

    // ALOG_HGL(PSTR("SEGMENT_I(segment_index).segcol[colour_index] length %d"), RGBCCTCOLOURS_SIZE);    

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
          SEGMENT_I(segment_index).segcol[colour_index].colour = RgbwwColor(array[0],array[1],array[2],array[3],array[4]);
        }

        data_buffer.isserviced++;
        // ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K("Raw")), SEGMENT_I(segment_index).segcol[colour_index].getHue360());
      }
      
      #if FIRMWARE_VERSION_MAX(0, 230)
      if(jtok = seg_obj["Manual"]){ // Needs renamed, setting colour RGBCW directly
        ALOG_ERR(PSTR("Use RGBCW command"));
      }
      #endif

    
      if(jtok = seg_obj[PM_HUE]){ // Range 0-359
        CommandSet_SegColour_RgbcctColour_Hue_360(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_HUE)), SEGMENT_I(segment_index).segcol[colour_index].getHue());
      }

      
      if(jtok = seg_obj[PM_SAT]){ // Range 0-100
        float value = mSupport::mapfloat(jtok.getFloat(), 0,100, 0,255); // Using float so sub 1% transition is possible
        CommandSet_SegColour_RgbcctColour_Sat_255( (uint8_t)value, colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_SAT)), SEGMENT_I(segment_index).segcol[colour_index].getSaturation());
        data_buffer.isserviced++; 
      }else
      if(jtok = seg_obj[PM_SAT_255]){ // Full Range 0-255
        CommandSet_SegColour_RgbcctColour_Sat_255(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_SAT255)), SEGMENT_I(segment_index).segcol[colour_index].getSaturation());
        data_buffer.isserviced++;
      }
    

      if(jtok = seg_obj[PM_CCT_PERCENTAGE]){ // Assume range 0-100    
        float value = mSupport::mapfloat(jtok.getFloat(), 0,100, CCT_MIN_DEFAULT,CCT_MAX_DEFAULT); // Using float so sub 1% transition is possible
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin( value, colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_CCT_PERCENTAGE)), SEGMENT_I(segment_index).segcol[colour_index].getCCT());
        data_buffer.isserviced++;
      }else
      if(jtok = seg_obj[PM_CCT_TEMP]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(jtok.getInt(), colour_index, segment_index);
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_CCT_PERCENTAGE)), SEGMENT_I(segment_index).segcol[colour_index].getCCT());
        data_buffer.isserviced++;
      }
    

      if(jtok = seg_obj[PM_BRIGHTNESS_RGB]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessRGB());
      }else
      if(jtok = seg_obj[PM_BRIGHTNESS_RGB_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessRGB(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_RGB_255)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessRGB());
      }

    
      if(jtok = seg_obj[PM_BRIGHTNESS_CCT]){ // Assume range 0-100
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(mapvalue(jtok.getInt(), 0,100, 0,255), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessCCT());
      }else
      if(jtok = seg_obj[PM_BRIGHTNESS_CCT_255]){ // Exact kelvin
        CommandSet_SegColour_RgbcctColour_BrightnessCCT(jtok.getInt(), colour_index, segment_index);
        data_buffer.isserviced++;
        ALOG_COM(PSTR(D_LOG_PIXEL D_COMMAND_NVALUE_K(D_BRIGHTNESS_CCT_255)), SEGMENT_I(segment_index).segcol[colour_index].getBrightnessCCT());
      }
     
    }
  
  }
 

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
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_ANIMATIONMODE)), GetAnimationModeName(buffer, sizeof(buffer)));
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
  SEGMENT_I(segment_index).colour_width__used_in_effect_generate = (ColourType)id;
  ALOG_INF(PSTR("ColourType = %d"),SEGMENT_I(segment_index).colour_width__used_in_effect_generate);
}


void mAnimatorLight::CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length)
{
  mPaletteI->addCustomPalette(index, data, data_length, encoding);
}


void mAnimatorLight::CommandSet_PaletteID(uint16_t value, uint8_t segment_index)
{
  char buffer[50];
  SEGMENT_I(segment_index).palette_id = value < mPaletteI->GetPaletteListLength() ? value : 0;  
  segment_current_index = segment_index;
  SEGMENT.LoadPalette(segments[segment_index].palette_id);
}


void mAnimatorLight::CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index)
{    
  SEGMENT_I(segment_index).cycle_time__rate_ms = value;
  ALOG_DBM( PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_NVALUE_K(D_TRANSITION, D_RATE_MS)), SEGMENT_I(segment_index).cycle_time__rate_ms );  
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setHue(hue_new);      
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setSaturation(sat_new);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setCCT_Kelvin(ct);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setBrightnessRGB(brightness);    
}


void mAnimatorLight::CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index, uint8_t segment_index)
{
  if(colour_index >= RGBCCTCOLOURS_SIZE){ return; }
  SEGMENT_I(segment_index).segcol[colour_index].setBrightnessCCT(brightness);    
}



#endif //USE_MODULE_LIGHTS_ANIMATOR




