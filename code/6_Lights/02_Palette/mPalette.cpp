#include "mPalette.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


mPalette* mPalette::instance = nullptr;

mPalette* mPalette::GetInstance()
{
  if (instance == nullptr){
    instance = new mPalette();
  }
  return instance;
}


/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Init
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


void mPalette::Init_Palettes()
{
  
  ALOG_DBM( PSTR("Init_Palettes") );
  
  /****************
   * StaticPalettes
   ****************/

  addStaticPalette(
    PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID, 
    PM_PALETTE_COLOURFUL_DEFAULT__DATA, 
    sizeof(PM_PALETTE_COLOURFUL_DEFAULT__DATA),
    D_PALETTE_COLOURFUL_DEFAULT_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_HOLLOWEEN_OP__ID, 
    PM_PALETTE_HOLLOWEEN_OP__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OP__DATA),
    D_PALETTE_HOLLOWEEN_OP_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_HOLLOWEEN_OGP__ID, 
    PM_PALETTE_HOLLOWEEN_OGP__DATA, 
    sizeof(PM_PALETTE_HOLLOWEEN_OGP__DATA),
    D_PALETTE_HOLLOWEEN_OGP_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_RAINBOW__ID, 
    PM_PALETTE_RAINBOW__DATA, 
    sizeof(PM_PALETTE_RAINBOW__DATA),
    D_PALETTE_RAINBOW_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_COMPRESSED_RAINBOW__ID, 
    PM_PALETTE_COMPRESSED_RAINBOW__DATA, 
    sizeof(PM_PALETTE_COMPRESSED_RAINBOW__DATA),
    D_PALETTE_COMPRESSED_RAINBOW_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_RAINBOW_INVERTED__ID, 
    PM_PALETTE_RAINBOW_INVERTED__DATA, 
    sizeof(PM_PALETTE_RAINBOW_INVERTED__DATA),
    D_PALETTE_RAINBOW_INVERTED_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_01__ID, 
    PM_PALETTE_PASTEL_01__DATA, 
    sizeof(PM_PALETTE_PASTEL_01__DATA),
    D_PALETTE_PASTEL_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_02__ID, 
    PM_PALETTE_PASTEL_02__DATA, 
    sizeof(PM_PALETTE_PASTEL_02__DATA),
    D_PALETTE_PASTEL_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_03__ID, 
    PM_PALETTE_PASTEL_03__DATA, 
    sizeof(PM_PALETTE_PASTEL_03__DATA),
    D_PALETTE_PASTEL_03_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_04__ID, 
    PM_PALETTE_PASTEL_04__DATA, 
    sizeof(PM_PALETTE_PASTEL_04__DATA),
    D_PALETTE_PASTEL_04_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PASTEL_05__ID, 
    PM_PALETTE_PASTEL_05__DATA, 
    sizeof(PM_PALETTE_PASTEL_05__DATA),
    D_PALETTE_PASTEL_05_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID, 
    PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE__DATA, 
    sizeof(PM_PALETTE_GRADIENT_PASTEL_TONES_PURPLE__DATA),
    D_PALETTE_GRADIENT_PASTEL_TONES_PURPLE_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID, 
    PM_PALETTE_HOT_PINK_NEON_WITH_NAVY__DATA, 
    sizeof(PM_PALETTE_HOT_PINK_NEON_WITH_NAVY__DATA),
    D_PALETTE_HOT_PINK_NEON_WITH_NAVY_ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC_WINTER_01__ID, 
    PM_PALETTE_WINTER_01__DATA, 
    sizeof(PM_PALETTE_WINTER_01__DATA),
    D_PALETTE_WINTER_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_WINTER_02__ID, 
    PM_PALETTE_WINTER_02__DATA, 
    sizeof(PM_PALETTE_WINTER_02__DATA),
    D_PALETTE_WINTER_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_WINTER_03__ID, 
    PM_PALETTE_WINTER_03__DATA, 
    sizeof(PM_PALETTE_WINTER_03__DATA),
    D_PALETTE_WINTER_03_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_WINTER_04__ID, 
    PM_PALETTE_WINTER_04__DATA, 
    sizeof(PM_PALETTE_WINTER_04__DATA),
    D_PALETTE_WINTER_04_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_AUTUMN_GREEN__ID, 
    PM_PALETTE_AUTUMN_GREEN__DATA, 
    sizeof(PM_PALETTE_AUTUMN_GREEN__DATA),
    D_PALETTE_AUTUMN_GREEN_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_AUTUMN_RED__ID, 
    PM_PALETTE_AUTUMN_RED__DATA, 
    sizeof(PM_PALETTE_AUTUMN_RED__DATA),
    D_PALETTE_AUTUMN_RED_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBO__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGPBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_ROGPBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_ROGPBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBO__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBO__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBY__ID, 
    PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__DATA),
    D_PALETTE__CHRISTMAS_TRADITIONAL_RGBY__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_VINTAGE_MINIBELLS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MINIBELLS__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_VINTAGE_MERRYLITES__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_MERRYLITES__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_VINTAGE_AGED_BULBS__ID, 
    PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__DATA),
    D_PALETTE__CHRISTMAS_VINTAGE_AGED_BULBS__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_01__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_01__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_02__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_03__ID, 
    PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__DATA),
    D_PALETTE__CHRISTMAS_SNOWY_COLOURS_03__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_BERRY_YELLOW__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_YELLOW__DATA),
    D_PALETTE__CHRISTMAS_BERRY_YELLOW__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_BERRY_ORANGE__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_ORANGE__DATA),
    D_PALETTE__CHRISTMAS_BERRY_ORANGE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__FESTIVE_BERRY_GREEN__ID, 
    PM_PALETTE__CHRISTMAS_BERRY_GREEN__DATA, 
    sizeof(PM_PALETTE__CHRISTMAS_BERRY_GREEN__DATA),
    D_PALETTE__CHRISTMAS_BERRY_GREEN__ENCODING
  );


  // END Christmas

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PAIRS_01__ID, 
    PM_PALETTE__COLOURFUL_PAIRS_01__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PAIRS_01__DATA),
    D_PALETTE__COLOURFUL_PAIRS_01__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_COLOUR_WHITE_STRIPE__ID, 
    PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__DATA),
    D_PALETTE__COLOURFUL_COLOUR_WHITE_STRIPE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ID, 
    PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__DATA),
    D_PALETTE__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_BLUE_WHITE__ID, 
    PM_PALETTE__COLOURFUL_BLUE_WHITE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_BLUE_WHITE__DATA),
    D_PALETTE__COLOURFUL_BLUE_WHITE__ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_WHITE_MIXED__ID, 
    PM_PALETTE__COLOURFUL_WHITE_MIXED__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_WHITE_MIXED__DATA),
    D_PALETTE__COLOURFUL_WHITE_MIXED__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PINK_RED_YELLOW__ID, 
    PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PINK_RED_YELLOW__DATA),
    D_PALETTE__COLOURFUL_PINK_RED_YELLOW__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PEACHY_ORANGE__ID, 
    PM_PALETTE__COLOURFUL_PEACHY_ORANGE__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PEACHY_ORANGE__DATA),
    D_PALETTE__COLOURFUL_PEACHY_ORANGE__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_PEACHY_YELLOW__ID, 
    PM_PALETTE__COLOURFUL_PEACHY_YELLOW__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_PEACHY_YELLOW__DATA),
    D_PALETTE__COLOURFUL_PEACHY_YELLOW__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_GREENLESS__ID, 
    PM_PALETTE__COLOURFUL_GREENLESS__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_GREENLESS__DATA),
    D_PALETTE__COLOURFUL_GREENLESS__ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC__GOLDEN__ID, 
    PM_PALETTE__GOLDEN__DATA, 
    sizeof(PM_PALETTE__GOLDEN__DATA),
    D_PALETTE__GOLDEN__ENCODING
  );


  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_01__ID, 
    PM_PALETTE_SUNRISE_01__DATA, 
    sizeof(PM_PALETTE_SUNRISE_01__DATA),
    D_PALETTE_SUNRISE_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_02__ID, 
    PM_PALETTE_SUNRISE_02__DATA, 
    sizeof(PM_PALETTE_SUNRISE_02__DATA),
    D_PALETTE_SUNRISE_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_03__ID, 
    PM_PALETTE_SUNRISE_03__DATA, 
    sizeof(PM_PALETTE_SUNRISE_03__DATA),
    D_PALETTE_SUNRISE_03_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNRISE_04__ID, 
    PM_PALETTE_SUNRISE_04__DATA, 
    sizeof(PM_PALETTE_SUNRISE_04__DATA),
    D_PALETTE_SUNRISE_04_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_01__ID, 
    PM_PALETTE_SUNSET_01__DATA, 
    sizeof(PM_PALETTE_SUNSET_01__DATA),
    D_PALETTE_SUNSET_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_02__ID, 
    PM_PALETTE_SUNSET_02__DATA, 
    sizeof(PM_PALETTE_SUNSET_02__DATA),
    D_PALETTE_SUNSET_02_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_RED__ID, 
    PM_PALETTE_SUNSET_RED__DATA, 
    sizeof(PM_PALETTE_SUNSET_RED__DATA),
    D_PALETTE_SUNSET_RED_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SUNSET_BEACH__ID, 
    PM_PALETTE_SUNSET_BEACH__DATA, 
    sizeof(PM_PALETTE_SUNSET_BEACH__DATA),
    D_PALETTE_SUNSET_BEACH_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_SKY_GLOW_01__ID, 
    PM_PALETTE_SKY_GLOW_01__DATA, 
    sizeof(PM_PALETTE_SKY_GLOW_01__DATA),
    D_PALETTE_SKY_GLOW_01_ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC__COLOURFUL_WITH_CCT_01__ID, 
    PM_PALETTE__COLOURFUL_WITH_CCT_01__DATA, 
    sizeof(PM_PALETTE__COLOURFUL_WITH_CCT_01__DATA),
    D_PALETTE__COLOURFUL_WITH_CCT_01__ENCODING
  );
  
  addStaticPalette(
    PALETTELIST_STATIC_CANDLE_FLAME_01__ID, 
    PM_PALETTE_CANDLE_FLAME_01__DATA, 
    sizeof(PM_PALETTE_CANDLE_FLAME_01__DATA),
    D_PALETTE_CANDLE_FLAME_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_GRADIENT_FIRE_01__ID, 
    PM_PALETTE_GRADIENT_FIRE_01__DATA, 
    sizeof(PM_PALETTE_GRADIENT_FIRE_01__DATA),
    D_PALETTE_GRADIENT_FIRE_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_OCEAN_01__ID, 
    PM_PALETTE_OCEAN_01__DATA, 
    sizeof(PM_PALETTE_OCEAN_01__DATA),
    D_PALETTE_OCEAN_01_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_FLOWER_SWEATPEA__ID, 
    PM_PALETTE_FLOWER_SWEATPEA__DATA, 
    sizeof(PM_PALETTE_FLOWER_SWEATPEA__DATA),
    D_PALETTE_FLOWER_SWEATPEA_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PINK_PURPLE__ID, 
    PM_PALETTE_PINK_PURPLE__DATA, 
    sizeof(PM_PALETTE_PINK_PURPLE__DATA),
    D_PALETTE_PINK_PURPLE_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC_PURPLE_PINK__ID, 
    PM_PALETTE_PURPLE_PINK__DATA, 
    sizeof(PM_PALETTE_PURPLE_PINK__DATA),
    D_PALETTE_PURPLE_PINK_ENCODING
  );

  addStaticPalette(
    PALETTELIST_STATIC__SOLAR_SKY__ID, 
    PM_PALETTE_DYNAMIC__SOLAR_SKY_01__DATA, 
    sizeof(PM_PALETTE_DYNAMIC__SOLAR_SKY_01__DATA),
    D_PALETTE_DYNAMIC__SOLAR_SKY_01__ENCODING
  );

  /****************
   * DynamicPalettes : Do all dynamic palettes have no data? actually, I may want to encode data to be used for these (not RGB data, but byte encoded data), so keep
   ****************/
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID, 
    PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA, 
    sizeof(PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA),
    D_PALETTE__SOLAR__CCT_ELEVATION_01_ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID, 
    PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA, 
    sizeof(PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA),
    D_PALETTE__SOLAR__CCT_ELEVATION_01_ENCODING
  );
  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID, 
    PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA, 
    sizeof(PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA),
    D_PALETTE__SOLAR__CCT_ELEVATION_01_ENCODING
  );

  
  addDynamicPalette(
    PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID, 
    PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA, 
    sizeof(PM_PALETTE__SOLAR__CCT_ELEVATION_01__DATA),
    D_PALETTE__SOLAR__CCT_ELEVATION_01_ENCODING
  );

  addDynamicPalette(
    PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID, 
    PM_PALETTE_DYNAMIC__SOLAR_SKY_01__DATA, 
    sizeof(PM_PALETTE_DYNAMIC__SOLAR_SKY_01__DATA),
    D_PALETTE_DYNAMIC__SOLAR_SKY_01__ENCODING
  );

  /****************
   * CustomPalettes
   ****************/

    

  addCustomPalette(
    0, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_01__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_01__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_01_ENCODING
  );
    
  
  addCustomPalette(
    1, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_02__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_02__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_02_ENCODING
  );
  
  addCustomPalette(
    2, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_03__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_03__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_03_ENCODING
  );
  
  addCustomPalette(
    3, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_04__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_04__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_04_ENCODING
  );
  
  addCustomPalette(
    4, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_05__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_05__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_05_ENCODING
  );
  
  addCustomPalette(
    5, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_06__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_06__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_06_ENCODING
  );
  
  addCustomPalette(
    6, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_07__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_07__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_07_ENCODING
  );
  
  addCustomPalette(
    7, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_08__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_08__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_08_ENCODING
  );
  
  addCustomPalette(
    8, 
    PM_PALETTE_CUSTOM_PALETTE_DEFAULT_09__DATA, 
    sizeof(PM_PALETTE_CUSTOM_PALETTE_DEFAULT_09__DATA), 
    D_PALETTE_CUSTOM_PALETTE_DEFAULT_09_ENCODING
  );
  
  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT  
  uint8_t test_data2[] = { // later use one of the static as a preload/default
    1,1,10, // 0
    2,2,20, // 1
    3,3,30, // 2
    4,4,40, // 3
    5,5,50, // 4
  };
  addDynamicPalette(                           // init this always with something basic, later to be changed and reloaded
    9, 
    test_data, 
    sizeof(test_data), 
    PALETTE_ENCODING_TYPE_RGB_NO_INDEX
  );
  #elif defined(ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT)
  uint8_t test_data2[] = { // later use one of the static as a preload/default
    0,   255,0,0, // 0
    1,   0,255,0, // 0
    2,   0,0,255, // 0
    49,  255,0,0, // 1
    55,  0,255,0, // 1
    150, 0,0,255, // 2
    200, 255,0,255, // 3
    255, 0,255,255, // 4
  };
  addCustomPalette(                           // init this always with something basic, later to be changed and reloaded
    9, 
    test_data2, 
    sizeof(test_data2), 
    PALETTE_ENCODING_TYPE_RGB_WITHINDEX_GRADIENT
  );
  #else
  addCustomPalette(
    9, 
    PM_PALETTE_COMPRESSED_RAINBOW__DATA, 
    sizeof(PM_PALETTE_COMPRESSED_RAINBOW__DATA), 
    D_PALETTE_COMPRESSED_RAINBOW_ENCODING
  );
  #endif


  
  

}



void mPalette::addStaticPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  PALETTE_DATA palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif

  // palette_tmp.friendly_name_ctr = name;
  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;
  
  uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
  palette_tmp.number_of_colours = length / encoded_colour_width;

  uint16_t id_adj = id - PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID; // First static ID

  if (id_adj < custom_palettes.size()) {
    static_palettes[id_adj] = palette_tmp;
  } else {
    static_palettes.push_back(palette_tmp);
  }

}

void mPalette::addDynamicPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  // ALOG_INF(PSTR("addDynamicPalette %d"), id);

  PALETTE_DATA palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif
  
  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
  ALOG_INF(PSTR("addDynamicPalette bytes added %d"), palette_tmp.data.size());
  #endif

  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;

  // Should be changed to a funciton that calculates the number of colours from the length and encoding, but only called at boottime
  if(length != 0)
  {
    uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
    palette_tmp.number_of_colours = length / encoded_colour_width;
  }else{
    if(palette_tmp.encoding.encoded_as_crgb_palette_16)
    {
      palette_tmp.number_of_colours = 16; // to be moved into some kind of default value
    }
    
  }

  // dynamic id is offset by the number of static palettes
  uint8_t id_adj = id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;

  if (id_adj < dynamic_palettes.size()) {
    dynamic_palettes[id_adj] = palette_tmp;
  } else {
    dynamic_palettes.push_back(palette_tmp);
  }

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
  ALOG_INF(PSTR("addDynamicPalette bytes added[3] %d"), dynamic_palettes[3].data.size());
  for(int i=0;i<length;i++){ Serial.print( dynamic_palettes[3].data[i]);Serial.print( "," ); } Serial.println();
  #endif

}


void mPalette::addCustomPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding)
{

  PALETTE_DATA palette_tmp;

  #ifdef ESP32
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data[i]); }
  #else // ESP8266
  uint8_t data_t[length];
  memcpy_P(data_t, data, length);
  for(int i=0;i<length;i++){ palette_tmp.data.push_back(data_t[i]); }
  #endif

  palette_tmp.encoding = {encoding};
  palette_tmp.palettelist_id = id;
  
  uint8_t encoded_colour_width  = GetEncodedColourWidth( palette_tmp.encoding );  
  palette_tmp.number_of_colours = length / encoded_colour_width;

  uint16_t id_adj = id;// - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; // First static ID

  if (id_adj < custom_palettes.size()) {
    custom_palettes[id_adj] = palette_tmp;
    ALOG_DBM(PSTR("addCustomPalette %d"), id_adj);
    
    // #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    ALOG_DBM(PSTR("addCustomPalette bytes added[3] %d"), custom_palettes[id_adj].data.size());
    // for(int i=0;i<length;i++){ Serial.print( custom_palettes[id_adj].data[i]); Serial.print( "," ); } Serial.println();
    // #endif

  } else {
    custom_palettes.push_back(palette_tmp);
  }

}


mPalette::PALETTE_ENCODING_DATA mPalette::findPaletteEncoding(uint16_t id)
{
    // Search in static palettes
    for (const auto& palette : static_palettes) {
        if (palette.palettelist_id == id) {
            ALOG_INF(PSTR("Found in static palettes with ID: %d"), id);
            return palette.encoding;
        }
    }

    // Search in dynamic palettes
    for (const auto& palette : dynamic_palettes) {
        if (palette.palettelist_id == id) {
            ALOG_INF(PSTR("Found in dynamic palettes with ID: %d"), id);
            return palette.encoding;
        }
    }

    // Search in custom palettes
    for (const auto& palette : custom_palettes) {
        if (palette.palettelist_id == id) {
            ALOG_INF(PSTR("Found in custom palettes with ID: %d"), id);
            return palette.encoding;
        }
    }

    // If not found, return a default encoding type or error code
    ALOG_INF(PSTR("Palette with ID: %d not found"), id);
    return {0}; // You could also return an error code here if needed
}

/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Main requests to get colours from palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/

RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::GetColourFromPreloadedPaletteBuffer_2023(
  uint16_t palette_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,    
  uint8_t* encoded_value,  
  uint8_t flag_spanned_segment, 
  uint8_t flag_wrap_hard_edge,        
  uint8_t flag_crgb_exact_colour,
  bool flag_request_is_for_full_visual_output
){
  DEBUG_PIN4_SET(0);
  
  RgbcctColor colour = RgbcctColor();

  // This block merges all dynamic palette handling directly into this function
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){
    
    uint8_t segIdx = tkr_anim->segment_current_index;
    if (segIdx >= tkr_anim->segments.size()) {
      segIdx = 0;
    }

    CRGB fastled_col;
    uint16_t pixel_position_adjust = _pixel_position;

    if (flag_crgb_exact_colour) {
      uint8_t pixels_in_crgb16palette = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size();
      pixel_position_adjust = _pixel_position % pixels_in_crgb16palette;
      pixel_position_adjust = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[pixel_position_adjust];
    }

    if (flag_spanned_segment) {
      if (tkr_anim->_virtualSegmentLength == 1) {
        pixel_position_adjust = 0;
      } else {
        pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
      }
    }

    if (flag_wrap_hard_edge) {
      pixel_position_adjust = scale8(pixel_position_adjust, 240);
    }

    fastled_col = ColorFromPalette(pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.data, pixel_position_adjust, 255, NOBLEND);
    colour = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b);
  } 
  // Static single color handling
  /**************************************************************
   * 
   * PALETTELIST_STATIC_SINGLE_COLOURS__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  

  
    uint8_t adjusted_id  = palette_id - PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    const uint8_t* data  = PM_STATIC_SINGLE_COLOURS__DATA;
    uint8_t adjust_buf_i =  adjusted_id*3;
    colour = RgbcctColor(data[adjust_buf_i], data[adjust_buf_i+1], data[adjust_buf_i+2]);

  }
  
  /**************************************************************
   * 
   * PALETTELIST_SEGMENT__RGBCCT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
  ){  
  

    uint8_t adjusted_id = palette_id - PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
    uint8_t segIdx = tkr_anim->segment_current_index;

    if(segIdx >= tkr_anim->segments.size() ){ segIdx = 0; } 
    if(adjusted_id < RGBCCTCOLOURS_SIZE)
    {
      colour = tkr_anim->segments[segIdx].rgbcctcolors[adjusted_id];
    }
    // colour.debug_print("rgbcctcolors");
  }
  else if (palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID && 
           palette_id < PALETTELIST_STATIC_LENGTH__ID) {

    // if(millis()%10==0) ALOG_INF(PSTR("pal %d"), palette_id);

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID; 

    uint8_t encoded_colour_width = GetEncodedColourWidth(static_palettes[palette_adjusted_id].encoding);
    uint8_t colours_in_palette = static_palettes[palette_adjusted_id].data.size() / encoded_colour_width;
    palette_buffer = &static_palettes[palette_adjusted_id].data[0];

    colour = Get_Encoded_Palette_Colour(
      palette_buffer,
      _pixel_position,
      encoded_colour_width,
      colours_in_palette,
      static_palettes[palette_adjusted_id].encoding,
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour,
      false
    );
  }
  // Custom palette handling
  else if (palette_id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED && 
           palette_id < GetPaletteListLength()) {

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;

    uint8_t encoded_colour_width = GetEncodedColourWidth(custom_palettes[palette_adjusted_id].encoding);
    uint8_t colours_in_palette = custom_palettes[palette_adjusted_id].data.size() / encoded_colour_width;
    palette_buffer = &custom_palettes[palette_adjusted_id].data[0];

    colour = Get_Encoded_Palette_Colour(
      palette_buffer,
      _pixel_position,
      encoded_colour_width,
      colours_in_palette,
      custom_palettes[palette_adjusted_id].encoding,
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour,
      false
    );
  }
  // Dynamic palettes inline switch-case
  else 
  if(
    ((palette_id >= PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  
    uint8_t palette_adjusted_id_rel0 = palette_id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;
    uint8_t encoded_colour_width  = GetEncodedColourWidth(dynamic_palettes[palette_adjusted_id_rel0].encoding);   
    uint8_t colours_in_palette = dynamic_palettes[palette_adjusted_id_rel0].data.size() / encoded_colour_width;
    palette_buffer = &dynamic_palettes[palette_adjusted_id_rel0].data[0];
    PALETTE_ENCODING_DATA encoding = dynamic_palettes[palette_adjusted_id_rel0].encoding;

    switch(palette_id) {
      case PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID: {
          
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING
        float azimuth = pCONT_solar->Get_Azimuth();
        #else
        float azimuth = 0;
        #endif
        float progress = mSupport::mapfloat(azimuth, 0, 360, 0.0f, 1.0f);

        RgbcctColor colour1 = pSEGMENT.rgbcctcolors[0];
        RgbcctColor colour2 = pSEGMENT.rgbcctcolors[1];
        return RgbcctColor::LinearBlend(colour1, colour2, progress);
      }
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID: {
        
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING
        float elevation = pCONT_solar->Get_Elevation();
        float el_min = pCONT_solar->Get_Elevation_Min();
        float el_max = pCONT_solar->Get_Elevation_Max();
        #else
        float elevation = 0;
        float el_min = -30;
        float el_max = 30;
        #endif

        float progress = mSupport::mapfloat(elevation, el_min, el_max, 0.0f, 1.0f);
        RgbcctColor colour1 = pSEGMENT.rgbcctcolors[0];
        RgbcctColor colour2 = pSEGMENT.rgbcctcolors[1];

        return RgbcctColor::LinearBlend(colour1, colour2, progress);
      }
      // Add additional cases for other dynamic palettes here, as needed
      default:
        ALOG_ERR(PSTR("Bad Palette ID"));
        return RgbcctColor(0, 0, 0); // Default to black if palette is not found
    }
  }
  else {
    ALOG_INF(PSTR("Missing %d"), palette_id);
  }

  DEBUG_PIN4_SET(1);
  return colour;
}

uint32_t mPalette::GetColourFromPreloadedPaletteBuffer_2025(
  uint16_t palette_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,    
  uint8_t* encoded_value,  
  uint8_t flag_spanned_segment, 
  uint8_t flag_wrap_hard_edge,        
  uint8_t flag_crgb_exact_colour,
  bool flag_request_is_for_full_visual_output
){
  DEBUG_PIN4_SET(0);
  
  RgbcctColor colour = RgbcctColor();

  // This block merges all dynamic palette handling directly into this function
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){
    
    uint8_t segIdx = tkr_anim->segment_current_index;
    if (segIdx >= tkr_anim->segments.size()) {
      segIdx = 0;
    }

    CRGB fastled_col;
    uint16_t pixel_position_adjust = _pixel_position;

    if (flag_crgb_exact_colour) {
      uint8_t pixels_in_crgb16palette = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index.size();
      pixel_position_adjust = _pixel_position % pixels_in_crgb16palette;
      pixel_position_adjust = pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.encoded_index[pixel_position_adjust];
    }

    if (flag_spanned_segment) {
      if (tkr_anim->_virtualSegmentLength == 1) {
        pixel_position_adjust = 0;
      } else {
        pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
      }
    }

    if (flag_wrap_hard_edge) {
      pixel_position_adjust = scale8(pixel_position_adjust, 240);
    }

    fastled_col = ColorFromPalette(pSEGMENT_I(segIdx).palette_container->CRGB16Palette16_Palette.data, pixel_position_adjust, 255, NOBLEND);
    colour = RgbcctColor(fastled_col.r, fastled_col.g, fastled_col.b);
  } 
  // Static single color handling
  /**************************************************************
   * 
   * PALETTELIST_STATIC_SINGLE_COLOURS__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  

  
    uint8_t adjusted_id  = palette_id - PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    const uint8_t* data  = PM_STATIC_SINGLE_COLOURS__DATA;
    uint8_t adjust_buf_i =  adjusted_id*3;
    colour = RgbcctColor(data[adjust_buf_i], data[adjust_buf_i+1], data[adjust_buf_i+2]);

  }
  
  /**************************************************************
   * 
   * PALETTELIST_SEGMENT__RGBCCT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
  ){  
  

    uint8_t adjusted_id = palette_id - PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
    uint8_t segIdx = tkr_anim->segment_current_index;

    if(segIdx >= tkr_anim->segments.size() ){ segIdx = 0; } 
    if(adjusted_id < RGBCCTCOLOURS_SIZE)
    {
      colour = tkr_anim->segments[segIdx].rgbcctcolors[adjusted_id];
    }
    // colour.debug_print("rgbcctcolors");
  }
  else if (palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID && 
           palette_id < PALETTELIST_STATIC_LENGTH__ID) {

    // if(millis()%10==0) ALOG_INF(PSTR("pal %d"), palette_id);

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID; 

    uint8_t encoded_colour_width = GetEncodedColourWidth(static_palettes[palette_adjusted_id].encoding);
    uint8_t colours_in_palette = static_palettes[palette_adjusted_id].data.size() / encoded_colour_width;
    palette_buffer = &static_palettes[palette_adjusted_id].data[0];

    colour = Get_Encoded_Palette_Colour(
      palette_buffer,
      _pixel_position,
      encoded_colour_width,
      colours_in_palette,
      static_palettes[palette_adjusted_id].encoding,
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour,
      false
    );
  }
  // Custom palette handling
  else if (palette_id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED && 
           palette_id < GetPaletteListLength()) {

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED;

    uint8_t encoded_colour_width = GetEncodedColourWidth(custom_palettes[palette_adjusted_id].encoding);
    uint8_t colours_in_palette = custom_palettes[palette_adjusted_id].data.size() / encoded_colour_width;
    palette_buffer = &custom_palettes[palette_adjusted_id].data[0];

    colour = Get_Encoded_Palette_Colour(
      palette_buffer,
      _pixel_position,
      encoded_colour_width,
      colours_in_palette,
      custom_palettes[palette_adjusted_id].encoding,
      encoded_value,
      flag_spanned_segment,
      flag_wrap_hard_edge,
      flag_crgb_exact_colour,
      false
    );
  }
  // Dynamic palettes inline switch-case
  else 
  if(
    ((palette_id >= PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  
    uint8_t palette_adjusted_id_rel0 = palette_id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;
    uint8_t encoded_colour_width  = GetEncodedColourWidth(dynamic_palettes[palette_adjusted_id_rel0].encoding);   
    uint8_t colours_in_palette = dynamic_palettes[palette_adjusted_id_rel0].data.size() / encoded_colour_width;
    palette_buffer = &dynamic_palettes[palette_adjusted_id_rel0].data[0];
    PALETTE_ENCODING_DATA encoding = dynamic_palettes[palette_adjusted_id_rel0].encoding;

    switch(palette_id) {
      case PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID: {
          
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING
        float azimuth = pCONT_solar->Get_Azimuth();
        #else
        float azimuth = 0;
        #endif
        float progress = mSupport::mapfloat(azimuth, 0, 360, 0.0f, 1.0f);

        RgbcctColor colour1 = pSEGMENT.rgbcctcolors[0];
        RgbcctColor colour2 = pSEGMENT.rgbcctcolors[1];
        return RgbcctColor::LinearBlend(colour1, colour2, progress).getU32();
      }
      case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID: {
        
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING
        float elevation = pCONT_solar->Get_Elevation();
        float el_min = pCONT_solar->Get_Elevation_Min();
        float el_max = pCONT_solar->Get_Elevation_Max();
        #else
        float elevation = 0;
        float el_min = -30;
        float el_max = 30;
        #endif

        float progress = mSupport::mapfloat(elevation, el_min, el_max, 0.0f, 1.0f);
        RgbcctColor colour1 = pSEGMENT.rgbcctcolors[0];
        RgbcctColor colour2 = pSEGMENT.rgbcctcolors[1];

        return RgbcctColor::LinearBlend(colour1, colour2, progress).getU32();
      }
      // Add additional cases for other dynamic palettes here, as needed
      default:
        ALOG_ERR(PSTR("Bad Palette ID"));
        return 0; // Default to black if palette is not found
    }
  }
  else {
    ALOG_INF(PSTR("Missing %d"), palette_id);
  }

  DEBUG_PIN4_SET(1);
  return colour.getU32();
}







/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: private/internal requests to get colours from palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/

#ifndef ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING

/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor  
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::Get_Encoded_StaticPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){
    
  // ALOG_ERR(PSTR("Get_Encoded_StaticPalette_Colour (%d,%d,%d,%d,%d,%d,%d)"), palette_adjusted_id, palette_buffer, _pixel_position, encoded_value, flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_colour, flag_force_gradient );

  
  uint8_t encoded_colour_width  = GetEncodedColourWidth(static_palettes[palette_adjusted_id].encoding);   
  uint8_t colours_in_palette = static_palettes[palette_adjusted_id].data.size()/encoded_colour_width;
  palette_buffer = &static_palettes[palette_adjusted_id].data[0];
  PALETTE_ENCODING_DATA encoding = static_palettes[palette_adjusted_id].encoding;
  

  return Get_Encoded_Palette_Colour(
    palette_buffer,
    _pixel_position,
    encoded_colour_width,
    colours_in_palette,
    encoding,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour,
    flag_force_gradient
  );

}

/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::SubGet_Encoded_CustomPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){

  uint8_t encoded_colour_width  = GetEncodedColourWidth(custom_palettes[palette_adjusted_id].encoding);   
  uint8_t colours_in_palette = custom_palettes[palette_adjusted_id].data.size()/encoded_colour_width;
  palette_buffer = &custom_palettes[palette_adjusted_id].data[0];
  PALETTE_ENCODING_DATA encoding = custom_palettes[palette_adjusted_id].encoding;

  return Get_Encoded_Palette_Colour(
    palette_buffer,
    _pixel_position,
    encoded_colour_width,
    colours_in_palette,
    encoding,
    encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    flag_crgb_exact_colour,
    flag_force_gradient
  );

}


/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 * 
 * Only static palettes should be within the palette class, the dynamic and custom palettes should be in the main code ?
 * Or, the parts that control the dynamic palettes (aux values) if moved into palettes probably makes it totally sustainable.
 * But then we have multiple for different segments, but if done right then the effect will just need to respect the single use palette
 * But then we have RGBCCT colours, which are all different per segment.
 * But that spans 1/2/3/4/5 grouped from those, which use these unique palettes and are therefore unique to the segment.
 * 
 * For now, I think dynamic should be made internal to the palette class and use an internal parameter in the place of aux
 * 
 * 
 * I should add to dynamic palettes, an optional flag for "getting for webui view" and hence instead of showing the exact colour, it would show a demo of the possible colours. Eg Sky could be the full colour spectrum. It would mean a higher level option would select full 16 colours and send crgb16 palette view
 * Or all dynamic palettes must have how it may look stored in memory, and that is used when the webui calls it. 
 */
RgbcctColor mPalette::Get_Encoded_DynamicPalette_Colour(
  uint16_t palette_adjusted_id,
  uint8_t* palette_buffer,
  uint16_t _pixel_position,  
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient,
  bool     flag_request_is_for_full_visual_output
){

/**
 * @brief New Sep24
 * Dynamic palettes must still load from stored memory, if nothing is required it can be overwritten below, but this will serve as webui views. 
 * So load, then if webui request, exist before generating the live part.
 * 
 */

  // ATTENTION: Adjusting palette back here, so it correctly reads fro dynamic_palette, but switch below requires the ID without adjustment

  uint8_t palette_adjusted_id_rel0 = palette_adjusted_id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;

  // ALOG_INF(PSTR("palette_adjusted_id_rel0 %d"), palette_adjusted_id_rel0);
  // Serial.flush();

  uint8_t encoded_colour_width  = GetEncodedColourWidth(dynamic_palettes[palette_adjusted_id_rel0].encoding);   
  uint8_t colours_in_palette = dynamic_palettes[palette_adjusted_id_rel0].data.size()/encoded_colour_width;
  palette_buffer = &dynamic_palettes[palette_adjusted_id_rel0].data[0];
  PALETTE_ENCODING_DATA encoding = dynamic_palettes[palette_adjusted_id_rel0].encoding;

  // ALOG_INF(PSTR("color_from_palette %d"), colours_in_palette);
  
  RgbcctColor loaded_colour;

  
  
  // _pixel_position = 0;

  // loaded_colour = Get_Encoded_Palette_Colour(
  //   palette_buffer,
  //   _pixel_position,
  //   encoded_colour_width,
  //   colours_in_palette,
  //   encoding,
  //   encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  //   flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  //   flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  //   flag_crgb_exact_colour,
  //   flag_force_gradient
  // );

  // loaded_colour.debug_print("loaded dynamic 0");
  // // This load seciton above, will likely need to be called below X times depending on the values I want. 

  // _pixel_position = 255;

  // loaded_colour = Get_Encoded_Palette_Colour(
  //   palette_buffer,
  //   _pixel_position,
  //   encoded_colour_width,
  //   colours_in_palette,
  //   encoding,
  //   encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  //   flag_map_scaling, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  //   flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  //   flag_crgb_exact_colour,
  //   flag_force_gradient
  // );

  // loaded_colour.debug_print("loaded dynamic 255");
  // // This load seciton above, will likely need to be called below X times depending on the values I want. 




// Here, I likely want to work conversions, then call the other methods that are needed.
// For simplicity, the required colours will be requested from the static options that are in progmem.

// // #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
// ALOG_INF(PSTR("Get_Encoded_DynamicPalette_Colour %d"), palette_adjusted_id);
// #endif 

// if(encoded_value != nullptr)
// {
//   ALOG_INF(PSTR("encoded_value != nullptr"));
// }
// else{
//   ALOG_INF(PSTR("encoded_value == nullptr"));
// }

/*
// Should I make it here that one of the matlab colourmaps can be loaded into here, and then some the float vector is actually saved with the palettes
// rather than the strip code? oh!
// 1) matlab palettes should be called through this, 
// 2) if the vector has been set, then it will enable the palette to be rescaled? though, this may not enable the temperature celius, or really its just added by command.
// This would limit scalled palettes to be only 1, which is probably enough. so lets just use a vector that is cleared when not using matlab palettes. 
// Should any palette be allowed to use this rescale? may be too complex and best to stick with matlab ones, otherwise the palette creation tool will be used
// and this is just for exact mapping from a value into colourmap.
// maybe it makes more sense to have another GetColourFromPalette that does the colour mapping, so it really is another functionality.


GetColourFromPalette_WithColourMapScale(normal colour map, pass in vector float list))


*/

  switch(palette_adjusted_id) //switch back to palette_NOT_adjusted_id
  { 
    default:
      ALOG_ERR(PSTR("Bad Palette ID"));
    case PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID:{

      #ifdef USE_MODULE_SENSORS_SUN_TRACKING
      float azimuth = pCONT_solar->Get_Azimuth();
      #else
      float azimuth = 0;
      #endif

      float progress = mSupport::mapfloat(azimuth, 0,360, 0.0f, 1.0f);
      
      RgbcctColor colour1 = pSEGMENT_I(0).rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT_I(0).rgbcctcolors[1];      

      #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
      Serial.println(azimuth);
      Serial.println(progress);
      #endif

      return RgbcctColor::LinearBlend(colour1, colour2, progress);

    }
    break;
    
    /**
     * @brief Only around the +-10% of horizon should the transition occur. Anything above/below should remain at fully cold/warm white
     * 
     */
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID:
    {

      #ifdef USE_MODULE_SENSORS_SUN_TRACKING
      float elevation = pCONT_solar->Get_Elevation();
      float el_min = pCONT_solar->Get_Elevation_Min();
      float el_max = pCONT_solar->Get_Elevation_Max();
      #else
      float elevation = 0;
      float el_min = -30;
      float el_max = 30;
      #endif

      // Determine pixel length and position for full visual output
      uint16_t pixel_length = tkr_anim->_virtualSegmentLength;
      RgbcctColor colour_out = RgbcctColor();  

      if (flag_request_is_for_full_visual_output) 
      {

        // Map the pixel position to the elevation range (el_min to el_max)
        float mapped_elevation = mSupport::mapfloat(_pixel_position, 0.0f, 16.0f, el_min, el_max);

        // Debug output for pixel_position_adjust and mapped_elevation
        ALOG_INF(PSTR("Full Visual Output: Pixel Position: %d, Mapped Elevation: %d"), _pixel_position, (int)mapped_elevation);

        // Based on the mapped elevation, assign cold or warm white, or blend between
        float elevation_transition_degrees = 10; // degrees around which to blend

        if (fabs(mapped_elevation) > elevation_transition_degrees) {
            if (mapped_elevation >= 0) {
                colour_out.setCCT_Kelvin(CCT_MIN_DEFAULT); // Cold White
                colour_out.setRGB(255, 255, 255);          // Full RGB for Cold White
            } else {
                colour_out.setCCT_Kelvin(CCT_MAX_DEFAULT); // Warm White
                colour_out.setRGB(0xFF, 0x52, 0x18);       // Warm Orange for Warm White
            }
        } else {
            float progress = mSupport::mapfloat(
                mapped_elevation,
                elevation_transition_degrees, 
                -1 * elevation_transition_degrees, 
                0.0f, 
                1.0f
            );

            RgbcctColor colour1 = RgbcctColor();
            colour1.setCCT_Kelvin(CCT_MIN_DEFAULT);  // Cold White
            colour1.setRGB(255, 255, 255);           // Full RGB for Cold White

            RgbcctColor colour2 = RgbcctColor();
            colour2.setCCT_Kelvin(CCT_MAX_DEFAULT);  // Warm White
            colour2.setRGB(0xFF, 0x52, 0x18);        // Warm Orange for Warm White

            colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
          }
      } else {
          // Default elevation-based calculation
          float elevation_transition_degrees = 20; // degrees above/below horizon to transition between warm/cold white
          float elevation_transition_offset = 0;   // Offset for the center of the transition

          // Adjusted center point for the transition
          float transition_min = elevation_transition_offset - elevation_transition_degrees / 2.0f;
          float transition_max = elevation_transition_offset + elevation_transition_degrees / 2.0f;

          if (fabs(elevation - elevation_transition_offset) > elevation_transition_degrees / 2.0f) {
              if (elevation >= transition_max) {
                  colour_out.setCCT_Kelvin(CCT_MIN_DEFAULT);  // Cold White
                  colour_out.setRGB(255, 255, 255);           // Full RGB for Cold White
              } else if (elevation <= transition_min) {
                  colour_out.setCCT_Kelvin(CCT_MAX_DEFAULT);  // Warm White
                  colour_out.setRGB(0xFF, 0x52, 0x18);        // Warm Orange for Warm White
              }
          } else {
              float progress = mSupport::mapfloat(
                  elevation,
                  transition_max,
                  transition_min,
                  0.0f,
                  1.0f
              );

              RgbcctColor colour1 = RgbcctColor();
              colour1.setCCT_Kelvin(CCT_MIN_DEFAULT);  // Cold White
              colour1.setRGB(255, 255, 255);           // Full RGB for Cold White

              RgbcctColor colour2 = RgbcctColor();
              colour2.setCCT_Kelvin(CCT_MAX_DEFAULT);  // Warm White
              colour2.setRGB(0xFF, 0x52, 0x18);        // Warm Orange for Warm White

              colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
          }
      }

      #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
      Serial.println(elevation);
      colour_out.debug_print("colour_out");
      #endif

      return colour_out;
      
    }
    break;
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID:
    {
        // Define the transition width around the horizon (±X degrees)
        float elevation_transition_degrees = 10.0f; // Transition over 20 degrees total (10 above and 10 below)

        #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
        Serial.println("PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID");
        #endif

        // Retrieve the two colors for blending
        RgbcctColor colour1 = pSEGMENT.rgbcctcolors[0]; // Daytime color
        RgbcctColor colour2 = pSEGMENT.rgbcctcolors[1]; // Nighttime color

        // Get the current solar elevation
        #ifdef USE_MODULE_SENSORS_SUN_TRACKING
        float elevation = pCONT_solar->Get_Elevation();
        #else
        float elevation = 0;
        #endif

        RgbcctColor colour_out;

        if (flag_request_is_for_full_visual_output) {
            // Full visual output: map _pixel_position to a range of 0 to 255 for a full transition demo
            uint16_t pixel_length = tkr_anim->_virtualSegmentLength;
            uint16_t pixel_position_adjust = (_pixel_position * 255) / (pixel_length - 1);

            // Simulate a full visual gradient across the entire day-night transition
            float progress;
            if (pixel_position_adjust <= 110) {
                // Solid day color
                colour_out = colour1;
            } else if (pixel_position_adjust >= 154) {
                // Solid night color
                colour_out = colour2;
            } else if (pixel_position_adjust == 111) {
                // Hard gradient stop at 111
                colour_out = RgbcctColor::LinearBlend(colour1, colour2, 0.0f); // Hard stop at the beginning of transition
            } else if (pixel_position_adjust >= 111 && pixel_position_adjust <= 153) {
                // Blend between day and night color between 111 and 153
                progress = mSupport::mapfloat(pixel_position_adjust, 111.0f, 153.0f, 0.0f, 1.0f);
                colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
            }

            #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
            Serial.print("Full Visual Output: ");
            Serial.print("Pixel Position Adjusted: ");
            Serial.println(pixel_position_adjust);
            colour_out.debug_print("colour_out");
            #endif
        } else {
            // Standard elevation-based calculation
            if (fabs(elevation) > elevation_transition_degrees) {
                // Outside transition zone: fixed day or night color
                if (elevation >= 0) { 
                    colour_out = colour1; // Daytime
                } else { 
                    colour_out = colour2; // Nighttime
                }
            } else {
                // Inside transition zone: blend between daytime and nighttime colors
                float progress = mSupport::mapfloat(
                    elevation, 
                    elevation_transition_degrees,    // Upper bound for transition
                    -elevation_transition_degrees,   // Lower bound for transition
                    0.0f,                            // Start of the blend
                    1.0f                             // End of the blend
                );

                colour_out = RgbcctColor::LinearBlend(colour1, colour2, progress);
            }

            #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
            Serial.println(elevation);
            colour_out.debug_print("colour_out");
            #endif
        }

        // Return the final blended or fixed color
        return colour_out;
    }
    break;



    case PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID:
    {

      float progress = mSupport::mapfloat(pCONT_time->RtcTime.second, 0,59, 0.0f, 1.0f);
      
      #ifdef ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
      Serial.println(progress);
      #endif


      RgbcctColor colour1 = pSEGMENT.rgbcctcolors[0];
      RgbcctColor colour2 = pSEGMENT.rgbcctcolors[1];      

      return RgbcctColor::LinearBlend(colour1, colour2, progress);

    }
    case PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID:
    {

      uint16_t pixel_length = tkr_anim->_virtualSegmentLength;

      uint16_t rescaled_palette_index;
      
      // Check if we're in preview mode
      if (flag_request_is_for_full_visual_output) {

        // Preview mode: Direct mapping without zoom, using the full range of the palette
        // Directly map the pixel position into the full palette range (0-255)
        rescaled_palette_index = _pixel_position;

        ALOG_INF(PSTR("preview %d"), rescaled_palette_index);

      } else {

        // Running mode: Apply zoom based on elevation    

        #ifdef USE_MODULE_SENSORS_SUN_TRACKING
        float elevation = pCONT_solar->Get_Elevation();
        float el_min = pCONT_solar->Get_Elevation_Min();
        float el_max = pCONT_solar->Get_Elevation_Max();
        // Serial.print(elevation); Serial.print("|"); Serial.print(el_min); Serial.print("|"); Serial.println(el_max);
        #else
        float elevation = 0;
        float el_min = -30;
        float el_max = 30;
        #endif


        // Step 1: Convert the pixel position into the 0-255 range
        uint16_t pixel_position_adjust = (_pixel_position * 255) / (pixel_length - 1);

        // Debug output for scaled pixel position
        // ALOG_INF(PSTR("Pixel Position Adjusted: %d"), pixel_position_adjust);

        // Calculate 40% of the elevation range for zoom
        float zoom_range_perc_as_ratio = 0.1f;
        float elevation_offset = zoom_range_perc_as_ratio * (el_max - el_min);

        // Calculate the elevation window around the current value
        float el_minus_40 = elevation - elevation_offset;
        float el_plus_40 = elevation + elevation_offset;

        // Ensure the elevation window is clamped within the valid elevation range
        if (el_minus_40 < el_min) el_minus_40 = el_min;
        if (el_plus_40 > el_max) el_plus_40 = el_max;

        // Debug output for elevation and its range
        // ALOG_INF(PSTR("Elevation: %d, el_min: %d, el_max: %d, el_minus_40: %d, el_plus_40: %d"),
        //     (int)elevation, (int)el_min, (int)el_max, (int)el_minus_40, (int)el_plus_40);

        // Step 3: Now, map this elevation range into the palette (0 to 255)
        uint16_t palette_start = (uint16_t)mSupport::mapfloat(el_minus_40, el_min, el_max, 0.0f, 255.0f);
        uint16_t palette_end = (uint16_t)mSupport::mapfloat(el_plus_40, el_min, el_max, 0.0f, 255.0f);

        // Debug output for palette start/end
        // ALOG_INF(PSTR("Palette Start: %d, Palette End: %d"), palette_start, palette_end);

        // Make sure palette_start is less than palette_end
        if (palette_start >= palette_end) {
            palette_start = (palette_end > 0) ? palette_end - 1 : 0;
        }

        // Step 4: Map the pixel position into the zoomed palette range
        rescaled_palette_index = (uint16_t)mSupport::mapfloat(
            pixel_position_adjust, 0.0f, 255.0f, palette_start, palette_end
        );

        // Debug output for the rescaled palette index
        // ALOG_INF(PSTR("Rescaled Palette Index: %d"), rescaled_palette_index);

        // Ensure rescaled_palette_index is clamped between 0 and 255
        rescaled_palette_index = constrain(rescaled_palette_index, 0, 255);
      }

      // Fetch the color using the rescaled palette index, regardless of mode
      RgbcctColor loaded_colour = Get_Encoded_Palette_Colour(
          palette_buffer,
          rescaled_palette_index,    // Pass the manually scaled 0-255 value here
          encoded_colour_width,
          colours_in_palette,
          encoding,
          encoded_value,
          false,                     // Set flag_map_scaling to false, as we are doing the scaling ourselves
          flag_wrap_hard_edge,
          flag_crgb_exact_colour,
          flag_force_gradient
      );

      return loaded_colour;

    }
    break;
    /***
     * 
     * New way to show temps as colours
     * 
     * Encoded my way
     *  - list of colours, with gradient position evening divided by how many there is
     *  - this may simply be a loaded normal RGB palette, but with gradient position conversion working
     *  - though, I may wish to set the inflection points, ie my X sensors may not be evening spaced. So in this case, may be best to manually work out the even conversion myself
     *  - grad,R,G,B, ( may wish to add white channels here), so really, back to my original palettes that are "encoded" with the first 6 bytes describing what the data holds.
     * 
     * CRGB16Palette
     *  - this will only permit 16 or 4 colours, and must be evenly spaced
     *  - so not going to work for this case 
     * 
     * 
     * PALETTELIST_VARIABLE_GENERIC_01__ID was the old name
     * 
     * 
     * 
     * 
     * 
    */
    /**
     * Using sensor readings, to generate colour palettes
     * This will be part of the new "sensor_struct" to hold all types
     *   Should it do like wled effects, memory buffer with different struct types?
     *    GetSensor(ptr,TEMP_ID) which will use memory location 0, to know how the struct is encoded and return the temperature from bytes 1-5 as float for example
     * This way, one struct pointer can be used to pass "sensor X" as a task source for this custom controller
     * */
    // case PALETTELIST_DYNAMIC__ENCODED_GENERIC__ID:{
      
    //   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES
    //   ALOG_INF(PSTR("Get_Encoded_DynamicPalette_Colour::ENCODED %d"), palette_adjusted_id);
    //   #endif 

    //   uint16_t palette_id = PALETTELIST_DYNAMIC__ENCODED_GENERIC__ID;
    //   uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;

    //   uint8_t encoded_colour_width  = GetEncodedColourWidth(dynamic_palettes[palette_id_adj].encoding);   
    //   uint8_t colours_in_palette = dynamic_palettes[palette_id_adj].data.size()/encoded_colour_width;
    //   palette_buffer = &dynamic_palettes[palette_id_adj].data[0];
    //   PALETTE_ENCODING_DATA encoding = dynamic_palettes[palette_id_adj].encoding;

    //   // uint8_t flag_map_scaling = true;//??

    //   // ALOG_INF(PSTR("encoded_colour_width \t%d"),encoded_colour_width);
    //   // ALOG_INF(PSTR("addDynamicPalette bytes added[3] %d"), dynamic_palettes[3].data.size());
    //   // for(int i=0;i<dynamic_palettes[3].data.size();i++){ Serial.print( dynamic_palettes[3].data[i]);Serial.print( "," ); } Serial.println();
  

    //   RgbcctColor colour;
      

    //   colour =  Get_Encoded_Palette_Colour(
    //     palette_buffer,
    //     _pixel_position,
    //     encoded_colour_width,
    //     colours_in_palette,
    //     encoding,
    //     encoded_value,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
    //     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
    //     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
    //     flag_crgb_exact_colour,
    //     flag_force_gradient
    //   );

    //   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES
    //   if(encoded_value != nullptr)
    //   {
    //     ALOG_INF(PSTR("Get_Encoded_Palette_Colour encoded_value != nullptr %d"), *encoded_value);
    //   }
    //   else{
    //     ALOG_INF(PSTR("encoded_value == nullptr"));
    //   }
    //   #endif // ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES


    //   return colour;






    //   #else

    //   RgbcctColor colour_out = RgbcctColor(255,0,255,0,0);
    //   RgbcctColor colour;

    //   // uint8_t test_data[] = { //if vector, then data length is inherited from vector
    //   //   // 6,5,
    //   //   0,0,0, // 0
    //   //   255,0,0, // 1
    //   //   0,255,0, // 2
    //   //   0,0,255, // 3
    //   //   255,255,0, // 4
    //   //   255,0,255, // 5
    //   // };

      
    //     uint16_t encoded_colour_width = 0;

    //     uint16_t palette_id = PALETTELIST_DYNAMIC__ENCODED_GENERIC__ID;
    //     uint16_t palette_id_adj = palette_id - mPalette::PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;


    //     mPalette::PALETTE_DATA pal = mPaletteI->dynamic_palettes[constrain(palette_id_adj,0,mPaletteI->dynamic_palettes.size()-1)];

    //     #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    //     ALOG_INF(PSTR("--------------palette_id_adj %d"), palette_id_adj);
    //     Serial.println(pal.encoding.data, BIN);
    //     #endif // ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS

    //     // PALETTE_ENCODING_DATA encoding = dynamic_palettes[palette_adjusted_id].encoding;

    //     // /**
    //     //  * @brief temp force load the data in
    //     //  * 
    //     //  */
    //     // pal.data.clear();
    //     // pal.data.assign(test_data, test_data + sizeof(test_data) / sizeof(test_data[0])); // load the data in
    //     // pal.encoding = {PALETTE_ENCODING_TYPE_RGB_NO_INDEX}; // force the encoding in




    //     if(pal.encoding.red_enabled){ encoded_colour_width++; }
    //     if(pal.encoding.green_enabled){ encoded_colour_width++; }
    //     if(pal.encoding.blue_enabled){ encoded_colour_width++; }
    //     if(pal.encoding.white_warm_enabled){ encoded_colour_width++; }

    //     if(pal.encoding.white_cold_enabled){ encoded_colour_width++; }
    //     if(pal.encoding.encoded_value_byte_width){ encoded_colour_width += pal.encoding.encoded_value_byte_width; }

    //     // if(pal.encoding.index_exact){ encoded_colour_width++; }
    //     if(pal.encoding.index_gradient){ encoded_colour_width++; }
    //     if(pal.encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
    //     if(pal.encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
        
    //     // if(pal.encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
    //     if(pal.encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
    //     if(pal.encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
    //     if(pal.encoding.palette_can_be_modified){ encoded_colour_width++; }


    //     if(encoded_colour_width==0)
    //     {
    //       // ALOG_ERR(PSTR("encoded_colour_width==0, crash errorAA =%S"), pal.friendly_name_ctr);
    //       return 0;
    //     }
    //     // ALOG_INF(PSTR("============  %d %d %d"),  pal.data.size(), encoded_colour_width, palette_colour_count);

            
      
    //     uint8_t  palette_colour_count = pal.data.size()/encoded_colour_width; 

    //     #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    //     ALOG_INF(PSTR("============  %d %d %d"),  pal.data.size(), encoded_colour_width, palette_colour_count);
    //     #endif // ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS

        
      
    //   /**
    //    * [0] colour_count, used with map_type to generate map_size (ie colour_map_size = pixel_width*pixel_count)
    //    * [1] colour_encoded_length      eg (6=rgb no_index, )
    //    * [2] data field 0
    //     * */

    //   uint16_t before = _pixel_position;
    //   uint16_t pixel_position_adjust = _pixel_position;
    //   uint8_t colours_in_palette = 5;



    //   // uint16_t before = pixel_position_adjust;
    //   // if(not gradient)
    // //   ALOG_INF(PSTR("colours_in_palette %d"), colours_in_palette);
    // // 
    // // delay(3000);
    //   // pixel_position_adjust %= colours_in_palette; // convert incoming pixels into repeating 0-15 numbers.
    //   pixel_position_adjust %= colours_in_palette; 
    //   // else // if gradient, then this same thing should happen but scale into 255 range
    //   // ALOG_INF(PSTR("pixel_position_adjust %d/%d"), before, pixel_position_adjust);
    
    //   colour = Get_Encoded_Colour_ReadBuffer_Fast(
    //     palette_buffer,
    //     pixel_position_adjust,  
    //     encoded_value,
    //     pal.encoding,
    //     encoded_colour_width
    //   );



    //   // if(not gradient)
    //   #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    //   ALOG_INF(PSTR("encoding %x"), pal.encoding);
    //   ALOG_INF(PSTR("encoded_value %d"), encoded_value);
    //   ALOG_INF(PSTR("pixel_position_adjust %d|%d %d,%d,%d"), before, pixel_position_adjust, colour.R, colour.G, colour.B);
    //   #endif // ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // // 
    // // delay(3000);

    //   // if(pixel_position_adjust < colours_in_palette)
    //   // {
    //   //   uint8_t colour_index = pixel_position_adjust*3 + 2;
    //   //   colour_out = RgbcctColor(test_data[colour_index], test_data[colour_index+1], test_data[colour_index+2]);
    //   // }
  

    //   return colour;

    // }break;
  }

  return RgbcctColor(255,0,0,0,0);

}

#endif // ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING






/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Main requests to get/set name/id of palettes
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


int16_t mPalette::Get_Static_PaletteIDbyName(const char* c)
{

  if(*c=='\0'){
    return -1;
  }

  

  char buffer[100] = {0};
  int16_t index_found = -1;
  int16_t id = -1;

  ALOG_INF( PSTR("Get_Static_PaletteIDbyName A") ); 


  for(
    uint8_t ii=0;
            ii<(PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID-PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_SEGMENT__RGBCCT_SOLID_COLOUR__NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_SEGMENT__RGBCCT_SOLID_COLOUR__NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;            
    }
  }


  ALOG_INF( PSTR("Get_Static_PaletteIDbyName B") ); 

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_CRGBPALETTE16_USER__LENGTH__ID
   * 
  ***************************************************************/


  for(
    uint8_t ii=0;
            ii<(PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID-PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID-PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID-PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;            
    }
  }


  /**************************************************************
   * 
   * PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID-PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID);
            ii++
  ){    
    ALOG_DBM( PSTR("s> %d %s \"%S\""), ii, c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_P(c, PM_STATIC_SINGLE_COLOUR_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;            
    }
  }


  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  for( // loops relative to exact palette id
    uint8_t ii=0;
            ii<(PALETTELIST_STATIC_LENGTH__ID - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID);
            ii++
  ){
    // ALOG_INF( PSTR("s> %d %s \"%S\"\n\r\n\r"), ii, c, PM_STATIC__PALETTES_NAMES_CTR ); 
    if((id=mSupport::GetCommandID16_MultipleSubMatches_P(c, PM_STATIC__PALETTES_NAMES_CTR))>=0)
    {
      ALOG_INF( PSTR("MATCH \"%s\" %d %d"), c, ii, id ); 
      return id+PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;            
    }
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC__COLOUR__IDS
   * 
  ***************************************************************/
  for(
    uint8_t ii=0;
            ii<(PALETTELIST_DYNAMIC__LENGTH__ID-PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID);
            ii++
  ){  
    ALOG_DBM(PSTR("s> %d %s \"%S\""), ii, c, PM_DYNAMIC_PALETTES_NAMES_CTR); 

    // Check if "c" starts with "Live "
    const char* adjusted_c = c;
    const char* live_prefix = "Live ";
    size_t live_prefix_len = strlen(live_prefix);

    if (strncmp(c, live_prefix, live_prefix_len) == 0) {
        adjusted_c = c + live_prefix_len;  // Skip the "Live " prefix
    }

    // Call GetCommandID16_P with the adjusted string
    if ((id = mSupport::GetCommandID16_P(adjusted_c, PM_DYNAMIC_PALETTES_NAMES_CTR)) >= 0) {
        ALOG_INF(PSTR("MATCH \"%s\" %d %d"), adjusted_c, ii, id); 
        return id + PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;            
    }
  }




  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * 
  ***************************************************************/
  for( // loops relative to 0
    uint8_t ii=0;
            ii<(MAX_USER_DEFINED_ENCODED_PALETTES);
            ii++
  ){

    memset(buffer,0,sizeof(buffer));
    sprintf_P(buffer, PSTR(D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR), ii + 1);
    
    ALOG_DBM( PSTR("s> \"%s\""), buffer ); 
    // Default names
    if(strcmp(c,buffer)==0){
      return ii+PALETTELIST_LENGTH_OF_STATIC_IDS; // new palettes start at the end of the predefined ones
    }
    
  }

  /**************************************************************
   * 
   * Final check, palette id was given as string number
   * 
  ***************************************************************/
  // uint8_t found_index = (!strlen(c)) ? 0 : atoi(c);
  // if(WithinLimits(found_index, (uint8_t)0, (uint8_t)PALETTELIST_STATIC_LENGTH__ID)){
  //   return found_index;
  // }

  return -1; // Must be -1 to show name not found
}


const char* mPalette::GetPaletteNameByID(uint8_t palette_id, char* buffer, uint8_t buflen)
{


  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_SEGMENT__RGBCCT_SOLID_COLOUR__NAMES_CTR);   
  }


  /**************************************************************
   * 
   * PALETTELIST_SEGMENTS_STORED_FIXED_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID) && (palette_id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, adjusted_id, PM_STATIC_CRGBPALETTE16_GRADIENT_NAMES_CTR);   
    // Serial.printf("BName id%d|a%d \"%s\"\n", palette_id,adjusted_id, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_DYNAMIC_
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){           
    uint16_t adjusted_id = palette_id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID;
    // Prefixing "Live" to these palettes
    sprintf(buffer, "Live ");
    mSupport::GetTextIndexed_P(&buffer[5], buflen-5, adjusted_id, PM_DYNAMIC_PALETTES_NAMES_CTR);
  }


  /**************************************************************
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_SINGLE_COLOUR_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );
  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < PALETTELIST_STATIC_LENGTH__ID))
  ){           
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID;
    mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC__PALETTES_NAMES_CTR);   
  }

  /**************************************************************
   * 
   * PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID)    && (palette_id < PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID))
  ){  
    uint16_t adjusted_id = palette_id - PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
    sprintf_P(buffer, PSTR(D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR), adjusted_id + 1); // names are 1-10
  }

  /**************************************************************
   * 
   * PALETTELIST_VARIABLE_GENERIC__IDS - undefined user names, default naming
   * 
  ***************************************************************/
  // If stored in RAM, and user editable, then it resides in DeviceNameBuffer and not within mPalette class


  return buffer;

}



/*********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************
 * SECTION: Internal helpers
 *********************************************************************************************************************************************************************************
 *********************************************************************************************************************************************************************************/


/**
 * Refresh value stored in palette, and return new value
 * */
uint8_t mPalette::GetColourMapSizeByPaletteID(uint8_t palette_id){
  
  uint8_t new_size = 1; // assumed 1 at least
  
  if((palette_id>=PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID)&&(palette_id<PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)){

    mPalette::PALETTE_DATA *ptr_tmp = &static_palettes[palette_id];

    new_size = 5; // only 1*5

    // ptr_tmp
    ALOG_ERR(PSTR("Removed code, needs refactored"));
    
    // ptr_tmp->data_length = new_size;

  }else
  if((palette_id>=PALETTELIST_LENGTH_OF_STATIC_IDS)&&(palette_id<GetPaletteListLength())){

    // PALETTE *ptr_tmp = &static_palettes[palette_id];
    
    new_size = 5; // ASSUME 10 RIGHT NOW  
    ALOG_ERR(PSTR("ERROR ON SIZE GET £$%"));
    // ptr_tmp->data_length; // currently refreshed when init is triggered, about this may need changing, leaving unchanged for now
  }
  else
  {

    new_size = 1;//ptr_tmp->data_length;
    ALOG_INF(PSTR("GetColourMapSizeByPaletteID ELSE  ERORRRRRRRRRRRRRRRRRRRRRRR"));
    

  }

  return new_size;
}




uint8_t mPalette::GetColoursInCRGB16Palette(uint16_t palette_id)
{

  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID;
    // mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    uint8_t colour_count = pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

    // ALOG_INF(PSTR("colour_count=%d, pal%d"), colour_count, palette_id);

    return colour_count;

  }


  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))
  ){  
    uint16_t palette_id_adj = palette_id - PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID;
    // mSupport::GetTextIndexed_P(buffer, buflen, palette_id_adj, PM_STATIC_CRGBPALETTE16_NAMES_CTR);   
    // ALOG_DBG( PSTR("BName id%d|a%d \"%s\""), palette_id,palette_id_adj, buffer );

    return pSEGMENT_I(0).palette_container->CRGB16Palette16_Palette.encoded_index.size();

  }

  return 16;

}







// uint16_t 
// #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
// IRAM_ATTR 
// #endif 
// mPalette::GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit)
// {
//   PALETTE_DATA pal = static_palettes[palette_id];
//   return GetNumberOfColoursFromEncoded(pal.encoding, pal.data.size());
// }


// uint16_t  
// #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
// IRAM_ATTR 
// #endif 
// mPalette::GetNumberOfColoursFromEncoded(PALETTE_ENCODING_DATA encoding, uint8_t data_in_palette)
// {
//   uint16_t encoded_colour_width = GetEncodedColourWidth(encoding);
//   return data_in_palette/encoded_colour_width;   
// }


/**
 * @brief Get elements in a colour type
 * 
 * @param ptr 
 * @param pixel_width_contrained_limit 
 * @return uint16_t 
 */
uint8_t mPalette::GetEncodedColourWidth( PALETTE_ENCODING_DATA encoding )
{

  uint16_t encoded_colour_width = 0;

  if(encoding.red_enabled){ encoded_colour_width++; }
  if(encoding.green_enabled){ encoded_colour_width++; }
  if(encoding.blue_enabled){ encoded_colour_width++; }
  if(encoding.white_warm_enabled){ encoded_colour_width++; }

  if(encoding.white_cold_enabled){ encoded_colour_width++; }
  if(encoding.encoded_value_byte_width){ encoded_colour_width += encoding.encoded_value_byte_width; }

  // if(encoding.index_exact){ encoded_colour_width++; }
  if(encoding.index_gradient){ encoded_colour_width++; }
  if(encoding.index_is_trigger_value_exact){ encoded_colour_width++; }
  if(encoding.index_is_trigger_value_scaled100){ encoded_colour_width++; }
  
  // if(encoding.encoded_as_hsb_ids){ encoded_colour_width++; }
  if(encoding.encoded_as_crgb_palette_16){ encoded_colour_width++; }
  if(encoding.encoded_as_crgb_palette_256){ encoded_colour_width++; }
  if(encoding.palette_can_be_modified){ encoded_colour_width++; }

  // ALOG_INF(PSTR("encoded_colour_width=%d"),encoded_colour_width);

  return encoded_colour_width;

}

uint8_t mPalette::GetColoursInPalette(uint16_t palette_id)
{


  // ALOG_INF(PSTR("Bflag_crgb_exact_colour=%d"), flag_crgb_exact_colour);
    
  /**************************************************************
   * 
   * "CRGBPALETTE16"
   * 
   * PALETTELIST_STATIC_CRGBPALETTE16__IDS
   * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
   * PALETTELIST_DYNAMIC_CRGBPALETTE16_PALETTES__IDS
   * 
  ***************************************************************/
  if(
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID) && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID)) ||
    ((palette_id >= PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID))  ||
    ((palette_id >= PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID)    && (palette_id < PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID))
  ){
    return 16;
  } // END of CRGBPalette's


  /**************************************************************
   * 
   * PALETTELIST_STATIC_SINGLE_COLOURS__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID) && (palette_id < PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID)
  ){  
    return 1;

  }

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID) && (palette_id < PALETTELIST_STATIC_LENGTH__ID))
  ){  
    uint16_t palette_adjusted_id = palette_id - PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID; // adjust back into correct indexing
    return static_palettes[palette_adjusted_id].number_of_colours;  
  } // end of my palettes

  /**************************************************************
   * 
   * PALETTELIST_STATIC__IDS
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID) && (palette_id < PALETTELIST_DYNAMIC__LENGTH__ID))
  ){  
    
    uint16_t palette_adjusted_id = palette_id - PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID; // adjust back into correct indexing
    return dynamic_palettes[palette_adjusted_id].number_of_colours;  

  } // end of my palettes


  /**************************************************************
   * 
   * PALETTELIST_SEGMENT__RGBCCT_COLOUR__IDS
   * 
  ***************************************************************/
  else
  if(
    (palette_id >= PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID) && (palette_id < PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID)
  ){  
    return 1;
  }


  /**************************************************************
   * 
   * Custom palettes: custom_palettes 
   * 
  ***************************************************************/
  else
  if(
    ((palette_id >= PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED)  && (palette_id < GetPaletteListLength())) // Custom palettes
  ){   

    uint16_t palette_adjusted_id = palette_id - PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED; // adjust back into correct indexing
    return custom_palettes[palette_adjusted_id].number_of_colours;  
      
  }

  
  /**************************************************************
   * 
   * Errors
   * 
  ***************************************************************/
  else
  {
    ALOG_INF(PSTR("Missing %d"), palette_id);
  }


}


#ifdef ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING

RgbcctColor 
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::Get_Encoded_Colour_ReadBuffer_Fast(
  uint8_t* palette_buffer,
  uint16_t pixel_position,  
  uint8_t* return_encoded_value,
  PALETTE_ENCODING_DATA encoding,
  uint8_t encoded_colour_width
) {
  // Precompute the index where the color starts (considering the gradient byte)
  uint16_t index_relative = pixel_position * encoded_colour_width;

  // Handle the gradient byte if gradient is enabled
  if (encoding.index_gradient) {
    // If the gradient is enabled, read it and skip over it to access the color data
    if (return_encoded_value != nullptr) {
      *return_encoded_value = palette_buffer[index_relative];  // store gradient value
    }
    index_relative++; // Move past the gradient byte to reach color data
  }

  // Fetch the color components based on the encoding flags
  uint8_t red   = (encoding.red_enabled)   ? palette_buffer[index_relative]   : 0;
  uint8_t green = (encoding.green_enabled) ? palette_buffer[index_relative+1] : 0;
  uint8_t blue  = (encoding.blue_enabled)  ? palette_buffer[index_relative+2] : 0;
  uint8_t white_cold = (encoding.white_cold_enabled) ? palette_buffer[index_relative+3] : 0;
  uint8_t white_warm = (encoding.white_warm_enabled) ? palette_buffer[index_relative+4] : 0;

  // Return the final color based on the decoded values
  return RgbcctColor(red, green, blue, white_cold, white_warm);
}

#else
/**
 * @brief private function
 * 
 * By having this as a subfunction, it should enable iterative multiple calls function the main GetColour
 * The modulo inside is mostly to stop overruns, with proper indexing it should not be needed
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor mPalette::Get_Encoded_Colour_ReadBuffer_Fast(
  uint8_t* palette_buffer,
  uint16_t pixel_position,  
  uint8_t* return_encoded_value,
  PALETTE_ENCODING_DATA encoding,
  uint8_t encoded_colour_width
){

  uint16_t index_relative = 0; // get expected pixel position

  #ifdef ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  Serial.println(encoding.data, BIN);
  // Serial.println(encoding.encoded_as_hsb_ids);
  Serial.println(encoding.index_gradient);
  #endif // ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
  
//       

// if(return_encoded_value != nullptr)
// {
//   ALOG_INF(PSTR("encoded_valuec != nullptr"));
// }
// else{
//   ALOG_INF(PSTR("encoded_valuec == nullptr"));

// }

  
  if( // Switch to bit masking
    (encoding.red_enabled)||
    (encoding.green_enabled)||
    (encoding.blue_enabled)||
    (encoding.white_cold_enabled)||
    (encoding.white_warm_enabled)
  ){
    
    // ALOG_INF(PSTR("enabled %d,%d,%d,%d,%d"), encoding.red_enabled, encoding.green_enabled, encoding.blue_enabled, encoding.white_cold_enabled, encoding.white_warm_enabled);
    // ALOG_INF(PSTR("palette_elements %d"), palette_elements[0]);   
    // ALOG_INF(PSTR("p = %d, r = %d, v = %d|%d, w=%d"), pixel_position, index_relative, palette_elements[index_relative], palette_elements[index_relative+1],colour_width);

    // Get Start of Colour Information by adjusting for indexing
    index_relative = pixel_position*encoded_colour_width; // get expected pixel position
      
  
    // ALOG_INF(PSTR("index_relativeA=%d"),index_relative);

    if(palette_buffer == nullptr){ ALOG_ERR(PSTR("palette_buffer is null")); return RgbcctColor(); }

      // 
    if(encoding.index_gradient)
    {
      // ALOG_INF(PSTR("index_relativeB=%d"),index_relative);
      if(return_encoded_value != nullptr){
        *return_encoded_value = palette_buffer[index_relative];
        // ALOG_INF(PSTR("return_encoded_value=%d"),*return_encoded_value);
      }
      // Set the index to move beyond the indexing information
      index_relative++;
    }
      // 
  
    
    return RgbcctColor(
      encoding.red_enabled         ? palette_buffer[index_relative  ] : 0,
      encoding.green_enabled       ? palette_buffer[index_relative+1] : 0,
      encoding.blue_enabled        ? palette_buffer[index_relative+2] : 0,
      encoding.white_cold_enabled  ? palette_buffer[index_relative+3] : 0,
      encoding.white_warm_enabled  ? palette_buffer[index_relative+4] : 0
    );

  }

  return RgbcctColor();

}
#endif // ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING




#ifdef ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING

/*
Simplified branching:
    Consolidated the logic for determining if the palette should act as a gradient or a discrete sequence into fewer branches.
    Made sure the key flags (flag_spanned_segment, flag_force_gradient, flag_crgb_exact_colour) are handled clearly and minimally.
Optimized handling of gradient palettes:
    The gradient logic is separated from the discrete palette logic, improving clarity.
    The loop for searching within the gradient range is simplified, and gradient scaling is handled directly using mapfloat as before.
Performance improvements:
    Eliminated redundant calculations, such as unnecessary recalculations of pixel_position_adjust.
    The gradient palette vector is now populated in a single pass, and boundary conditions are checked in an efficient loop.
*/
RgbcctColor  
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::Get_Encoded_Palette_Colour(
  uint8_t* palette_buffer,
  uint16_t _pixel_position, 
  uint8_t encoded_colour_width,
  uint8_t colours_in_palette,
  PALETTE_ENCODING_DATA encoding,
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, 
  bool     flag_wrap_hard_edge,        
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){
  RgbcctColor colour;
  uint16_t pixel_position_adjust = _pixel_position;

  // Handling discrete sequence palettes (non-gradient)
  bool is_forced_to_get_discrete = flag_crgb_exact_colour;
  bool is_not_gradient = (encoding.index_gradient == false);
  bool is_basic_sequence_palette = is_forced_to_get_discrete || (is_not_gradient && !flag_spanned_segment);

  if (is_basic_sequence_palette) {
    // Handle non-gradient palette colors, with or without segment spanning
    if (flag_spanned_segment && !is_forced_to_get_discrete) {
      pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
    }

    // Map pixel position to color index in the palette
    pixel_position_adjust %= colours_in_palette;

    colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      pixel_position_adjust,  
      encoded_value,
      encoding,
      encoded_colour_width
    );
    return colour;
  }

  // Handle gradient palettes or forced gradient
  if (encoding.index_gradient || flag_force_gradient) {
    if (flag_spanned_segment) {
      pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
    }

    // Set boundaries for gradient mapping
    uint8_t lower_limit = colours_in_palette / 2;
    uint8_t upper_limit = 255 - (colours_in_palette / 2);
    
    std::vector<uint8_t> gradient_palettes(colours_in_palette);
    if (encoding.index_gradient) {
      for (uint8_t i = 0; i < colours_in_palette; ++i) {
        Get_Encoded_Colour_ReadBuffer_Fast(
          palette_buffer,
          i,  
          &gradient_palettes[i],
          encoding,
          encoded_colour_width
        );
      }
    } else {
      for (uint8_t i = 0; i < colours_in_palette; ++i) {
        gradient_palettes[i] = map(i, 0, colours_in_palette - 1, lower_limit, upper_limit);
      }
    }

    // Search for lower and upper boundaries within gradient
    uint8_t lower_boundary_i = 0, upper_boundary_i = 0;
    uint8_t lower_boundary_v = 0, upper_boundary_v = 0;
    float progress = 0;

    if (pixel_position_adjust < lower_limit) {
      lower_boundary_i = 0;
      upper_boundary_i = 1;
      lower_boundary_v = gradient_palettes[lower_boundary_i];
      upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    } else if (pixel_position_adjust > upper_limit) {
      lower_boundary_i = gradient_palettes.size() - 1;
      upper_boundary_i = gradient_palettes.size(); // ignored
      lower_boundary_v = gradient_palettes[lower_boundary_i];
      upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    } else {
      for (uint8_t i = 0; i < gradient_palettes.size() - 1; ++i) {
        if (pixel_position_adjust >= gradient_palettes[i] && pixel_position_adjust < gradient_palettes[i + 1]) {
          lower_boundary_i = i;
          upper_boundary_i = i + 1;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
          progress = mSupport::mapfloat(pixel_position_adjust, lower_boundary_v, upper_boundary_v, 0.0f, 1.0f);
          break;
        }
      }
    }

    // Blend between lower and upper color boundaries
    RgbcctColor lower_colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      lower_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );
    RgbcctColor upper_colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      upper_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );

    colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

    // Set the encoded value if applicable
    if (encoded_value != nullptr) {
      *encoded_value = (pixel_position_adjust < 255) ? lower_boundary_v : upper_boundary_v;
    }

    return colour;
  }

  // Handle simple spanned palettes
  if (flag_spanned_segment) {
    pixel_position_adjust = (_pixel_position * 255) / (tkr_anim->_virtualSegmentLength - 1);
  }

  uint8_t palette_index = scale8(pixel_position_adjust, colours_in_palette - 1);

  colour = Get_Encoded_Colour_ReadBuffer_Fast(
    palette_buffer,
    palette_index,  
    encoded_value,
    encoding,
    encoded_colour_width
  );

  return colour;
}

#else



/**
 * @brief private function
 * 
 * Attempting to make addressing of all palettes into 255 range so it works easier with WLED
 * 0-255, regardless of length. Hence, 5 pixels in a palette would address as 0,50,100,150,200 (the 255/5), no longer as 0,1,2,3,4. This would make things more uniform when getting colours from any palette
 * 
 * 
 * Dont bother with subfunctions, keep it tight with this even if code has to repeat. Code cost without function call penalty
 * 
 * @param palette_id 
 * @param seg_i 
 * @return RgbcctColor 
 */
RgbcctColor  
#ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
IRAM_ATTR 
#endif 
mPalette::Get_Encoded_Palette_Colour(
  uint8_t* palette_buffer,
  uint16_t _pixel_position, 
  uint8_t encoded_colour_width,
  uint8_t colours_in_palette,
  PALETTE_ENCODING_DATA encoding,
  uint8_t* encoded_value, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
  bool     flag_spanned_segment, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
  bool     flag_wrap_hard_edge,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
  bool     flag_crgb_exact_colour,
  bool     flag_force_gradient
){
    
// if(encoded_value != nullptr)
// {
//   ALOG_INF(PSTR("encoded_valueA != nullptr"));
// }
// else{
//   ALOG_INF(PSTR("encoded_valueA == nullptr"));

// }

// ALOG_INF(PSTR("flag_spanned_segment=%d"), flag_spanned_segment);


  // ALOG_ERR(PSTR("Get_Encoded_StaticPalette_Colour (%d,%d,%d,%d,%d)"), _pixel_position, encoded_value, flag_spanned_segment, flag_wrap_hard_edge, flag_crgb_exact_colour, flag_force_gradient );


// The issue now is that "SPAN" should not be forcing my palettes to show across the thing, but react based on their type. If they are gradient, then span, if they are not, then should still be sequential

  /**
   * @brief 
   * 
   * 
   * I think I need a new flag, "flag_spanned_segment" is confusing. 
   * Right now it means to scale the incoming index, which is the SEGLEN, into the 255 palette range.
   * 
   * But, what happens when the incoming data is the same, but the effect wants to force a normal pallete to be gradient.
   * I maybe need a "flag_force_gradient"? That way an effect can ask for it? Though in theory I do not want this, the effect
   * should want the gradient/discrete palette to be the selection of the user, and not be forcing it. No palette forced conversion should happen other than to make sure
   * the effect is visually correct. So no "FORCE it" should happen.
   * 
   */
      

  RgbcctColor colour;
  // if(palette_adjusted_id>static_palettes.size())
  // {
  //   ALOG_ERR(PSTR("PALETTE EXCEEDS VECTOR SIZE"));
  //   return colour;
  // }

  // PALETTE_DATA *ptr = &static_palettes[palette_adjusted_id];  
  // uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  
  // uint8_t colour_width  = GetEncodedColourWidth(ptr->encoding); 


  // uint8_t colour_width  = GetEncodedColourWidth(static_palettes[palette_adjusted_id].encoding);   
  // uint8_t pixels_in_map = static_palettes[palette_adjusted_id].data.size()/colour_width;
  // palette_buffer = &static_palettes[palette_adjusted_id].data[0];
  // PALETTE_ENCODING_DATA encoding = static_palettes[palette_adjusted_id].encoding;

  uint16_t pixel_position_adjust = _pixel_position;

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * DISCRETE SEQUENCE, is_basic_sequence_palette_with_increasing_indexing
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */
  bool is_forced_to_get_discrete = flag_crgb_exact_colour;
  bool is_not_gradient = (encoding.index_gradient == false);
  /**
   * @brief NOTE
   * 
   * For original sequential palettes that are not gradients, the incoming index must remain always "increasing integer iterator"  (e.g., 0,1,2,3,4,5..LEN).
   * Therefore, to get the palette colour, the modulo can simply be used to get the sequence of colours.
   * If segment_span is on, then the incoming index is scaled into 255 range, and may not be a "increasing integer iterator" and must first be converted back.
   *  - Should this ever happen? If spanned is set, then is a sequence even the desired effect? or, should it just be displayed as 0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3...? So ranges across it. Likely dont want this.
   * 
   * 
   */
  bool is_basic_sequence_palette_with_increasing_indexing = is_forced_to_get_discrete || is_not_gradient && !flag_spanned_segment;

  // ALOG_INF(PSTR("flags %d %d %d"), is_forced_to_get_discrete, is_not_gradient, is_basic_sequence_palette_with_increasing_indexing);

      
  
  if(is_basic_sequence_palette_with_increasing_indexing) // forced, or originally normal discrete
  {
      
    // 0-SEGLEN would need to become iterative of the palette    0-100    what if 0-100*pixels  so 0-500, then modulo by 100
    // o,1,2,3,4, 0,1,2,3,4, 0,1,2,3,4
    // but because of 255 range, these values should be scaled into 255
    //0-100 into 0-25500,    so 25500%100 would be 

    // Serial.println("Checking here it is not a gradient, if it is not, then the incoming 0-SEGLEN needs to still be treating to get the sequential of my palettes");
    // Serial.println("spanned segment needs better definition, it is confusing. It should be OFF most of the time as default. The incoming 0-SEGLEN should be assumed to be scaled to 255");
    
    // pixel_position_adjust = (_pixel_position*255)/(tkr_anim->_virtualSegmentLength -1); 
    // pixel_position_adjust = map(pixel_position_adjust, 0,255, 0,tkr_anim->SEGLEN);//  pixels_in_map; // convert incoming pixels into repeating 0-15 numbers.

    if(flag_spanned_segment && !is_forced_to_get_discrete) // if incoming index is SEGWIDTH, then it needs converted first. Or, does it need converted back?
    {
      // pixel_position_adjust = (_pixel_position*255)/(tkr_anim->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
      // pixel_position_adjust = scale8(pixel_position_adjust, pixels_in_map); //gradient type when exact value is needed needs scaled into full range
    }
    
    uint16_t before = pixel_position_adjust;
    // if(not gradient)
  //   ALOG_INF(PSTR("colours_in_palette %d"), colours_in_palette);
  // 
  // delay(3000);
    pixel_position_adjust %= colours_in_palette; // convert incoming pixels into repeating 0-15 numbers.
    // else // if gradient, then this same thing should happen but scale into 255 range
    // ALOG_INF(PSTR("pixel_position_adjust %d/%d"), before, pixel_position_adjust);
  
    colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      pixel_position_adjust,  
      encoded_value,
      encoding,
      encoded_colour_width
    );
    
    return colour;

  }
  

  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * Now, just handle the GRADIENTS
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */
  if(encoding.index_gradient || flag_force_gradient) // if spanned, inside checks for gradient handling
  {

    
      // 
    /**
     * @brief Convert incoming indes (0-SEGLEN) back into 255 range (0-255)
     * If it is mine, no matter what, convert into 255 range
     **/
    if(flag_spanned_segment)
    {
      if(tkr_anim->_virtualSegmentLength==1)
      {
        pixel_position_adjust = 0;
      }else{
        pixel_position_adjust = (_pixel_position*255)/(tkr_anim->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
      }
      // pixel_position_adjust = (_pixel_position*255)/(tkr_anim->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    }

      
      // ALOG_INF(PSTR("index_gradient"));
      
    // uint8_t pixel_position_adjust = _pixel_position;

    //   if (flag_map_scaling) pixel_position_adjust = (_pixel_position*255)/(tkr_anim->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    // if (!flag_wrap_hard_edge){
    //   // uint8_t pixel_end_boundary = map(pixels_in_map-1, 0,pixels_in_map-1, 0,255); // depends on the number of pixels
    //   pixel_position_adjust = scale8(pixel_position_adjust, 240); //cut off blend at palette "end", 240, or 15/16*255=>240/255, so drop last 16th (15 to wrapped 0) gradient of colour
    // }
      // ALOG_INF(PSTR("_pixel_position %d/%d/%d"), _pixel_position, tkr_anim->_virtualSegmentLength, pixel_position_adjust);

      
    /**
     *  Last step, convert back
     * **/
    // pixel_position_adjust = map(pixel_position_adjust, 0,255, 0,pixels_in_map-1);
    // pixel_position_adjust = constrain(pixel_position_adjust, 0,pixels_in_map-1); // to protect again out of bounds



      // ALOG_INF(PSTR("here2 pixel_position_adjust %d"), pixel_position_adjust);

    uint8_t edge_size = colours_in_palette;
    uint8_t lower_limit = edge_size/2;
    uint8_t upper_limit = 255-(edge_size/2);

    // uint8_t edge_size = 1;//255/pixels_in_map;
    // uint8_t lower_limit = 0;//edge_size/2;
    // uint8_t upper_limit = 255;//255-(edge_size/2);

    bool palette_contains_gradient_indexes = false;

    std::vector<uint8_t> gradient_palettes;
    uint8_t encoded_value2 = 0;

    /**
     * @brief Only "index_gradient" index is a gradient
     * 
     */
    if(encoding.index_gradient)
    {
      // ALOG_INF(PSTR("colours_in_palette %d"), colours_in_palette);
      for(uint8_t pix_i=0; pix_i<colours_in_palette; pix_i++)
      {
        Get_Encoded_Colour_ReadBuffer_Fast( // since the gradient exists already, this is some recursive call
            palette_buffer,
            pix_i,  
            &encoded_value2,
            encoding,
            encoded_colour_width
          );
        gradient_palettes.push_back(encoded_value2);
      }
      // Serial.print("Read existing encoded values: "); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,",gradient_palettes[v]); } Serial.println();
    }else{
      for(uint8_t pix_i=0; pix_i<colours_in_palette; pix_i++)
      {
        gradient_palettes.push_back(map(pix_i, 0,colours_in_palette-1, lower_limit,upper_limit));
      }
      // Serial.print("No existing gradients, create equidistant values: "); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,%s",gradient_palettes[v], v<gradient_palettes.size()-1?"":"\n\r"); } Serial.println();
    }
    
    // ALOG_INF(PSTR("pixel_position (adjusted) %d(%d)"), _pixel_position, pixel_position_adjust);


      // 
  
    // Search for lower boundary
    uint8_t desired_pixel_scaled = pixel_position_adjust;
    uint8_t lower_boundary_i = 0;
    uint8_t upper_boundary_i = 0;
    uint8_t lower_boundary_v = 0;
    uint8_t upper_boundary_v = 0;
    bool lower_boundary_found = 0;

    float progress = 0;

    /**
     * @brief 
     * Uses current scaled gradient value, and searches the palettes scaled boundaries to get the region of palette to scale between
     * >>Example<<
     * Palette of 5 colours, index 0,  1,   2,   3,   4
     * Scaled to 255       , grad  0, 64, 128, 191, 255
     * Desired scaled value (200)                  ^        191-255 should be scaled into 0.0f to 1.0f, and the colour between these as LinearBlend(colour_lower, colour_upper, ratio)    
     * 
     * Since the for loop checks the current and next index in the same loop, it will check all but the final/end boundary
     * Therefore, this will always be the end/255 value, and should be checked first. Only if not the final, should the loop be used to search for the boundary.
     * Based on fastled, there may be a divisor was to achieve this?? Note that above I am creating the range, then searching with it. Surely I simply need to assume the index and then create its equiavlent mapped scaled value just once?
     * Leave as optimise problem.
     * 
     * If pixels of 5 is the palette
     * and 150 (as 250 range) is asked for as gradient.
     * map(150, 0,255, 0,5-1) would give lower boundary?     
     * 
     */
    if(desired_pixel_scaled < lower_limit)   
    {
      // ALOG_INF(PSTR("(desired_pixel_scaled < lower_limit) "));
      // Stick with first colour
      lower_boundary_i = 0;
      upper_boundary_i = 1;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    }else
    if(desired_pixel_scaled > upper_limit)
    {
      // ALOG_INF(PSTR("(desired_pixel_scaled > lower_limit) "));
      // Stick with last colour
      lower_boundary_i = gradient_palettes.size()-1;
      upper_boundary_i = gradient_palettes.size(); //ignored
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
      progress = 0;
    }
    else // Search
    {
      // ALOG_INF(PSTR("(desired_pixel_scaled   lower_limit) else "));
      lower_boundary_i = 0; //default for errors
      upper_boundary_i = 1;
      bool found_match = false;
      for(uint8_t v=0;v<gradient_palettes.size()-1;v++) // Using the indexes expect the final one
      {      
        // ALOG_INF(PSTR("v>>>>>>>> [%d]  %d<%d<%d"), v, gradient_palettes[v], pixel_position_adjust, gradient_palettes[v+1]);

        uint8_t lower_index = v;
        uint8_t upper_index = v+1;

        if(
          (pixel_position_adjust >= gradient_palettes[lower_index])&&    // Greater than lower/current boundary
          (pixel_position_adjust < gradient_palettes[upper_index])     // Smaller than upper/next boundary
        ){
          lower_boundary_i = lower_index;
          upper_boundary_i = upper_index;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
          progress = mSupport::mapfloat(desired_pixel_scaled, lower_boundary_v,upper_boundary_v, 0.0f, 1.0f);
          // ALOG_INF(PSTR("WITHIN ======================BREAK %d   ||| %d larger %d? (%d|%d)"), v, desired_pixel_scaled, gradient_palettes[v], lower_boundary_i, upper_boundary_i);
          found_match = true;
          break; // found lower boundary index
        }
        else{
          // ALOG_INF(PSTR("OUTSIDE %d is still lower than %d for index %d"), gradient_palettes[v], desired_pixel_scaled, v);
        }
      }
      if(!found_match)
      {
        // ALOG_INF(PSTR("ERROR: No match found for %d"), desired_pixel_scaled);
        // Assume its at the top range, so just force the last grad pair
          lower_boundary_i = gradient_palettes.size()-2;
          upper_boundary_i = gradient_palettes.size()-1;
          lower_boundary_v = gradient_palettes[lower_boundary_i];
          upper_boundary_v = gradient_palettes[upper_boundary_i];
          progress = mSupport::mapfloat(desired_pixel_scaled, lower_boundary_v,upper_boundary_v, 0.0f, 1.0f);



      }
    }
    // Serial.print("gradient_palettes"); for(uint8_t v=0;v<gradient_palettes.size();v++){ Serial.printf("%d,",gradient_palettes[v]); } Serial.println();
    // ALOG_INF(PSTR("v>>>>>>>> [%d|%d]  %d|%d p%d"), _pixel_position, desired_pixel_scaled, lower_boundary_v, upper_boundary_v, (int)(progress*100));

      // 
  
    // RgbcctColor lower_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, lower_boundary_i); 
    // RgbcctColor upper_colour = Get_StaticPalette_Encoded_Colour_ReadBuffer(palette_adjusted_id, palette_buffer, upper_boundary_i);
    
    RgbcctColor lower_colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      lower_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );
    RgbcctColor upper_colour = Get_Encoded_Colour_ReadBuffer_Fast(
      palette_buffer,
      upper_boundary_i,  
      nullptr,
      encoding,
      encoded_colour_width
    );

    colour = RgbcctColor::LinearBlend(lower_colour, upper_colour, progress);

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES
    lower_colour.debug_print("lower_colour");
    upper_colour.debug_print("upper_colour");
    colour.debug_print("  out_colour");
    ALOG_INF(PSTR("lower_boundary_i|upper_boundary_i %d|%d"), lower_boundary_i, upper_boundary_i);
    ALOG_INF(PSTR("lower_boundary_v|upper_boundary_v %d|%d"), lower_boundary_v, upper_boundary_v);
    #endif 

    if(encoded_value != nullptr)
    {
      if(desired_pixel_scaled < 255)
        *encoded_value = lower_boundary_v;
      else
        *encoded_value = upper_boundary_v; //final one should be the last, ie, 255
    }

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES
    if(encoded_value != nullptr)
    {
      ALOG_INF(PSTR("!!!!!!!!!!!!!!!!!!!!!!!!!!!!encoded_valueF != nullptr %d"), *encoded_value);
    }
    else{
      ALOG_INF(PSTR("encoded_valueF == nullptr"));
    }
    #endif

    return colour;

  } // if(ptr->encoding.index_gradient || flag_force_gradient) 

  
      // 
  
  /******************************************************************************************************************************************************
   * *******************************************************************************************************************************************************************************
   * 
   * Not a gradient palette, but needs to be spanned => Just show across the span without blending
   * 
   * ******************************************************************************************************************************************************
   * ******************************************************************************************************************************************************************************* */

  // Serial.println("What is this for then? It was either discrete, or gradient, so what is this?");
      
  if(flag_spanned_segment)
  {
      
    if(tkr_anim->_virtualSegmentLength==1)
    {
      pixel_position_adjust = 0;
    }else{
      pixel_position_adjust = (_pixel_position*255)/(tkr_anim->_virtualSegmentLength -1);  // This scales out segment_index to segment_length as 0 to 255
    }
  }

      // 
  
  // PALETTE_DATA *ptr = &static_palettes[palette_adjusted_id];
  // uint8_t pixels_in_map = GetNumberOfColoursInPalette(palette_adjusted_id);  

  uint8_t palette_index = scale8(pixel_position_adjust, colours_in_palette-1); //assumes original index is 0-255, and scales to 0-15

  // ALOG_INF(PSTR("pixel_position|palette_index %d->%d  %d %d"), pixel_position_adjust, palette_index, pixels_in_map, palette_index % 5);
  
      


  colour = Get_Encoded_Colour_ReadBuffer_Fast(
    palette_buffer,
    palette_index,  
    encoded_value,
    encoding,
    encoded_colour_width
  );
      

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES
    if(encoded_value != nullptr)
    {
      ALOG_INF(PSTR("encoded_valueD != nullptr %d"), *encoded_value);
    }
    else{
      ALOG_INF(PSTR("encoded_valueD == nullptr"));
    }
    #endif // ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES

      
  

  return colour;

}


#endif // ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING

#endif // header gaurd
