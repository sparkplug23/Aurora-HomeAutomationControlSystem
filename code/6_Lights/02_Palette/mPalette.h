#ifndef _M_PALETTE_H
#define _M_PALETTE_H

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/02_Palette/mPalette_Progmem.h"
#include "2_CoreSystem/11_Languages/mLanguageProgmem.h"
#include "1_TaskerManager/mTaskerManager.h"

#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/00_Interface/FastLED/FastLED.h"

#include "internal/mPalette_Encoding_Options.h"

class mPalette 
{
  private:
    /* Prevent others from being created */
    mPalette(mPalette const& other) = delete;
    mPalette(mPalette&& other) = delete;
    /* Private constructor to prevent instancing. */
    mPalette(){ 
      // Serial.println("DEBUG_LINE_HERE_PAUSE"); delay(2000); 
      Init_Palettes();
    };
  public:
    // External function to get instance
    static mPalette* GetInstance();
    /* Here will be the instance stored. */
    static mPalette* instance;

    void Init_Palettes();

    /**************
     * COLOUR Designs by name
     * PALETTE are different colours by pixel
    **************/                            
    int16_t Get_Static_PaletteIDbyName(const char* c);
    const char* GetPaletteNameByID(uint8_t id, char* buffer, uint8_t buflen);


    /************************************************************************************************************************************
     * ************************************************************************************************************************************
     * @brief  Static palettes read from inside this class
     ************************************************************************************************************************************
     ***************************************************************************************************************************************/
   
    /**
     * @brief 
     * Single colour user options, up to all 5 rgbcct elements
     */
    enum PALETTELIST_SEGMENT__RGBCCT_COLOUR__IDS
    {
      PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID = 0, // New scene colour, static
      PALETTELIST_SEGMENT__RGBCCT_COLOUR_02__ID,
      PALETTELIST_SEGMENT__RGBCCT_COLOUR_03__ID,
      PALETTELIST_SEGMENT__RGBCCT_COLOUR_04__ID,
      PALETTELIST_SEGMENT__RGBCCT_COLOUR_05__ID,
      PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID
    };


    /**
     * @brief 
     * Generated using PALETTELIST_SEGMENT__RGBCCT_COLOUR__IDS
     **/
    enum PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__IDS
    {
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_TWO_12__ID = PALETTELIST_SEGMENT__RGBCCT_COLOUR_LENGTH__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_THREE_123__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FOUR_1234__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_FIVE_12345__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__PAIRED_REPEATED_ACTIVE__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_01_RANDOM_HUE__ID,      
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_02_RANDOM_HUE_80TO100_SATURATIONS__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_03_RANDOM_HUE_60TO100_SATURATIONS__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_04_RANDOM_HUE_60TO85_SATURATIONS__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__RANDOMISE_COLOURS_05_RANDOM_HUE_00TO100_SATURATIONS__ID,
      PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID    
    };


    /**
     * @brief 
     * Sourced from WLED, part of FastLED. Common palettes found in MATLAB are also included for heatmaps
     **/
    enum PALETTELIST_STATIC_CRGBPALETTE16__IDS
    {      
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_COLOUR__ID = PALETTELIST_SEGMENT__RGBCCT_CRGBPALETTE16_PALETTES__LENGTH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__CLOUD_COLOURS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LAVA_COLOURS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__OCEAN_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__FOREST_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__RAINBOW_STRIPE_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__PARTY_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__HEAT_COLOUR__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_PARULA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_TURBO__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_HOT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_COOL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_SPRING__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_AUTUMN__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__COLOURMAP_JET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID
    };


    /**
     * @brief 
     * Sourced from WLED, part of FastLED
     **/
    enum PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
    { //39 of them 
      // 0-9
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET__ID = PALETTELIST_STATIC_CRGBPALETTE16__LENGTH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RIVENDELL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__OCEAN_BREEZE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RED_AND_BLUE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__YELLOWOUT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ANALOGOUS__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__PINK_SPLASH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SUNSET_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ANOTHER__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BEECH__ID,
      // 10-19
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__VINTAGE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__DEPARTURE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LANDSCAPE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BEACH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RAINBOW_SHERBET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__HULT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__HULT64__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__DRYWET__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__JUL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__GRINTAGE__ID,
      // 20-29
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__REWHI__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__TERTIARY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LAVA_FIRE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ICE_FIRE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__CYANE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LIGHT_PINK__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__AUTUMN__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLUE_MAGENTA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLACK_MAGENTA_RED__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLACK_RED_MAGENTA_YELLOW__ID,
      // 30-39
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLUE_CYAN_YELLOW__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ORANGE_TEAL__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__TIAMAT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__APRIL_NIGHT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ORANGERY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SAKURA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__AURORA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__ATLANTICA__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__TEMPERATURE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__AURORA_2__ID,
      // 40-49
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RETRO_CLOWN__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__CANDY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__TOXY_REAF__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__FAIRY_REAF__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__SEMI_BLUE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__PINK_CANDY__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RED_REAF__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__AQUA_FRESH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__YELLOW_BLUE_HOT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__LITE_LIGHT__ID,
      // 50-54
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RED_FLASH__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__BLINK_RED__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__RED_SHIFT__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__CANDY_2__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__PINK_PURPLE__ID,
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT__PINK_WHITE_PURPLE__ID,


      //
      PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID
    };
    

    /**
     * @brief 
     * Instead of HTML Colours, define some popular colours for easy switching without using Custom Colour ##. Eg, Black for effects.
     */
    enum PALETTELIST_STATIC_SINGLE_COLOURS__IDS
    {
      PALETTELIST_STATIC_SINGLE_COLOUR__RED__ID = PALETTELIST_STATIC_CRGBPALETTE16_GRADIENT_LENGTH__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__ORANGE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__LIGHTORANGE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__YELLOW__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__LIMEGREEN__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__GREEN__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__CYAN__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__BLUE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__BLUEPURPLE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__PURPLE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__PINK__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__HOTPINK__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__WARMWHITE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__COLDWHITE__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__BLACK__ID,
      PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID    
    };


///// START of stored in vector

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    enum PALETTELIST_STATIC__IDS
    {
      PALETTELIST_STATIC_COLOURFUL_DEFAULT__ID = PALETTELIST_STATIC_SINGLE_COLOUR__LENGTH__ID,
      PALETTELIST_STATIC_HOLLOWEEN_OP__ID,
      PALETTELIST_STATIC_HOLLOWEEN_OGP__ID,
      PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY__ID,
      PALETTELIST_STATIC_RAINBOW__ID,
      PALETTELIST_STATIC_COMPRESSED_RAINBOW__ID,
      PALETTELIST_STATIC_RAINBOW_INVERTED__ID,
      PALETTELIST_STATIC_PASTEL_01__ID,
      PALETTELIST_STATIC_PASTEL_02__ID,
      PALETTELIST_STATIC_PASTEL_03__ID,
      PALETTELIST_STATIC_PASTEL_04__ID,
      PALETTELIST_STATIC_PASTEL_05__ID,
      PALETTELIST_STATIC_WINTER_01__ID,
      PALETTELIST_STATIC_WINTER_02__ID,
      PALETTELIST_STATIC_WINTER_03__ID,
      PALETTELIST_STATIC_WINTER_04__ID,
      PALETTELIST_STATIC_AUTUMN_GREEN__ID,
      PALETTELIST_STATIC_AUTUMN_RED__ID,
      PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBO__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGPBY__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_ROGPBY__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBO__ID,
      PALETTELIST_STATIC__FESTIVE_TRADITIONAL_RGBY__ID,
      PALETTELIST_STATIC__FESTIVE_VINTAGE_MINIBELLS__ID,
      PALETTELIST_STATIC__FESTIVE_VINTAGE_MERRYLITES__ID,
      PALETTELIST_STATIC__FESTIVE_VINTAGE_AGED_BULBS__ID,
      PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_01__ID,
      PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_02__ID,
      PALETTELIST_STATIC__FESTIVE_SNOWY_COLOURS_03__ID,
      PALETTELIST_STATIC__FESTIVE_BERRY_YELLOW__ID,
      PALETTELIST_STATIC__FESTIVE_BERRY_ORANGE__ID,
      PALETTELIST_STATIC__FESTIVE_BERRY_GREEN__ID,
      PALETTELIST_STATIC__COLOURFUL_PAIRS_01__ID,
      PALETTELIST_STATIC__COLOURFUL_COLOUR_WHITE_STRIPE__ID,
      PALETTELIST_STATIC__COLOURFUL_COLOUR_WARMWHITE_STRIPE__ID,
      PALETTELIST_STATIC__COLOURFUL_BLUE_WHITE__ID,
      PALETTELIST_STATIC__COLOURFUL_WHITE_MIXED__ID,
      PALETTELIST_STATIC__COLOURFUL_PINK_RED_YELLOW__ID,
      PALETTELIST_STATIC__COLOURFUL_PEACHY_ORANGE__ID,
      PALETTELIST_STATIC__COLOURFUL_PEACHY_YELLOW__ID,
      PALETTELIST_STATIC__COLOURFUL_GREENLESS__ID,
      PALETTELIST_STATIC__GOLDEN__ID,
      PALETTELIST_STATIC_SUNRISE_01__ID,
      PALETTELIST_STATIC_SUNRISE_02__ID,
      PALETTELIST_STATIC_SUNRISE_03__ID,
      PALETTELIST_STATIC_SUNRISE_04__ID,
      PALETTELIST_STATIC_SUNSET_01__ID,
      PALETTELIST_STATIC_SUNSET_02__ID,
      PALETTELIST_STATIC_SUNSET_RED__ID,
      PALETTELIST_STATIC_SUNSET_BEACH__ID,
      PALETTELIST_STATIC_SKY_GLOW_01__ID,      
      PALETTELIST_STATIC__COLOURFUL_WITH_CCT_01__ID,
      PALETTELIST_STATIC_CANDLE_FLAME_01__ID,
      PALETTELIST_STATIC_GRADIENT_FIRE_01__ID,
      PALETTELIST_STATIC_OCEAN_01__ID,
      PALETTELIST_STATIC_FLOWER_SWEATPEA__ID,
      PALETTELIST_STATIC_PINK_PURPLE__ID,
      PALETTELIST_STATIC_PURPLE_PINK__ID,
      PALETTELIST_STATIC__SOLAR_SKY__ID,
      PALETTELIST_STATIC_LENGTH__ID 
    };

    /**
     * @brief These below to become fully added into my methods as another encoding type
     * to be merged and named in my static list
     * These palettes probably also do not need any internal storage. They should be generated from external parameters(solar position, time, etc)
     * Or, dynamic simply need a name from progmem, but also std::vector of data as optional way to record information about itself when used. Though, this cant be done for multiple segments as they are shared. This can be fine, just a limitation is the dynamic palette does not change across segments
     * These need moved outside of this ENUM as its own list
     * */
    enum PALETTELIST_DYNAMIC__COLOUR__IDS{ // to be moved, so static list should be renamed "PredefinedList", then "DynamicList" (which should also include "Randomise") and "CustomList" 

      PALETTELIST_DYNAMIC__SOLAR_AZIMUTH__WHITE_COLOUR_TEMPERATURE_01__ID = PALETTELIST_STATIC_LENGTH__ID, // New scene colour, static
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__WHITE_COLOUR_TEMPERATURE_01__ID,
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__RGBCCT_PRIMARY_TO_SECONDARY_01__ID,
      PALETTELIST_DYNAMIC__TIMEREACTIVE__RGBCCT_PRIMARY_TO_SECONDARY_WITH_SECONDS_IN_MINUTE_01__ID, //palette will change from rgbcct01 to rgbcct02 over 60 seconds


      // Palette that reflects how the sky looks at about 10 above the horizon throughout the day.
      //   - day time, cyan blue
      //   - towards sunset, sunset colours
      //   - dusk, sunset to depp blue
      //   - optional flag will show deepest blue or off at night
      PALETTELIST_DYNAMIC__SOLAR_ELEVATION__SOLID_COLOUR_OF_SKY__ID,

      PALETTELIST_DYNAMIC__LENGTH__ID
    };

///// END of stored in vector

    /****************************************************************************************************************************************
     *****************************************************************************************************************************************
      @brief 
     * @NOTE: Only palettes below this are fixed in memory (WLED types)
     ****************************************************************************************************************************************
     *****************************************************************************************************************************************/
    #define PALETTELIST_LENGTH_OF_STATIC_IDS  PALETTELIST_STATIC_LENGTH__ID

    #define PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED  PALETTELIST_DYNAMIC__LENGTH__ID

    #define MAX_USER_DEFINED_ENCODED_PALETTES 10
    uint8_t user_defined_palette_count = 10;

    uint16_t GetPaletteListLength(){ return PALETTELIST_LENGTH_OF_PALETTES_IN_FLASH_THAT_ARE_NOT_USER_DEFINED + user_defined_palette_count; }

    /**
     * @brief IMPORTANT
     * 
     * The bit order here is CRITICAL, as it aligns with the DEFINES used in the header.
     * 
     * Reorder the defines when new bit usage is decided
     * 
     */
    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        uint16_t red_enabled : 1;
        uint16_t green_enabled : 1;
        uint16_t blue_enabled : 1;
        uint16_t white_warm_enabled : 1;

        uint16_t white_cold_enabled : 1;        
        /**
         * @brief Index can take up to three bytes
         * 
         **/
        uint16_t encoded_value_byte_width : 3; // 3 bits wide, or 0b000 gives 9 value options

        // Remove this
        uint16_t reserved1 : 1;                       // Specialised, maybe also could be removed as not useful. The effect itself should treat this index as special
        // Rename from "index_ scaled_to_segment" to "index_gradient"
        uint16_t index_gradient : 1;           // To rename, again, "index_gradient" worded as effect style, whereas it should simply be "index_gradient"
        //
        uint16_t index_is_trigger_value_exact : 1;
        uint16_t index_is_trigger_value_scaled100 : 1;  //probably remove this, why bother having 100% when 0-255 is the same

        uint16_t reserved2 : 1; // encoded_as_hsb_ids : 1; //move this to other encoded types   //deleting!!!!
        uint16_t encoded_as_crgb_palette_16 : 1;
        uint16_t encoded_as_crgb_palette_256 : 1;
        uint16_t palette_can_be_modified : 1;
        
      };
    } PALETTE_ENCODING_DATA;

    /**
     * @brief 
     * StaticPalette will not change, and are stored in memory under various encoding types
     * 
     */
    struct PALETTE_DATA{ // 6 bytes per palette
      uint16_t palettelist_id;
      // Pointer to name
      // const char* friendly_name_ctr;                   // Should I just move towards the faster way? there is no added benefit here
      // colour bytes 
      std::vector<uint8_t> data;
      // Moving away from colour_width to reduce calculation per pixel
      uint8_t number_of_colours;
      // Contains information on formatting of data buffer
      PALETTE_ENCODING_DATA encoding;
    };
    std::vector<PALETTE_DATA> static_palettes;
    std::vector<PALETTE_DATA> dynamic_palettes;
    std::vector<PALETTE_DATA> custom_palettes;

    void addStaticPalette (uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding);
    void addCustomPalette (uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding);
    void addDynamicPalette(uint16_t id, const uint8_t* data, const uint8_t length, uint16_t encoding);

    uint8_t GetColourMapSizeByPaletteID(uint8_t palette_id);
    uint8_t GetEncodedColourWidth( PALETTE_ENCODING_DATA encoded );


    // uint16_t GetNumberOfColoursInPalette(uint16_t palette_id, uint8_t pixel_width_contrained_limit = 0); 
    // uint16_t GetNumberOfColoursFromEncoded(PALETTE_ENCODING_DATA encoding, uint8_t data_in_palette);

    uint8_t GetColoursInCRGB16Palette(uint16_t palette_id);

    PALETTE_ENCODING_DATA findPaletteEncoding(uint16_t id);


    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    GetColourFromPreloadedPaletteBuffer_2023(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      uint8_t     flag_spanned_segment = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      uint8_t     flag_wrap_hard_edge = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      uint8_t     flag_crgb_exact_colour = false,
      bool flag_request_is_for_full_visual_output = false
    );
  
    [[gnu::hot]] uint32_t GetColourFromPreloadedPaletteBuffer_2025(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      uint8_t     flag_spanned_segment = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      uint8_t     flag_wrap_hard_edge = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      uint8_t     flag_crgb_exact_colour = false,
      bool flag_request_is_for_full_visual_output = false
    );
  

#ifndef ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
    RgbcctColor Get_Encoded_StaticPalette_Colour(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = false,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_crgb_exact_colour = false,
      bool     flag_forced_gradient = false
    );

    RgbcctColor SubGet_Encoded_CustomPalette_Colour(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      bool     flag_map_scaling = false, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = false,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_crgb_exact_colour = false,
      bool     flag_forced_gradient = false
    );

    
    // Dynamic palettes should do any calculations, then rely on the other methods to get colours
    RgbcctColor Get_Encoded_DynamicPalette_Colour(
      uint16_t palette_id = 0,
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = false,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_crgb_exact_colour = false,
      bool     flag_forced_gradient = false,
      bool flag_request_is_for_full_visual_output = false // eg. webui, show what the whole range would be
    );

#endif










    RgbcctColor     
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    Get_Encoded_Palette_Colour(
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t encoded_colour_width = 0,
      uint8_t colours_in_palette = 0,
      PALETTE_ENCODING_DATA encoding = {0},
      uint8_t* encoded_index = nullptr,  // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = false,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_crgb_exact_colour = false,
      bool     flag_forced_gradient = false
    );



    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    Get_Encoded_Colour_ReadBuffer_Fast(
      uint8_t* palette_elements = nullptr,
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      PALETTE_ENCODING_DATA encoding = {0},
      uint8_t encoded_colour_width = 0
    );






    uint8_t GetColoursInPalette(uint16_t palette_id);

    



};

#define mPaletteI mPalette::GetInstance() // lets investigate making mPalette NOT a singleton, though, to be included inside palette controller, it might need to be so it only has once instance

#endif

#endif // _M_PALETTE_H


