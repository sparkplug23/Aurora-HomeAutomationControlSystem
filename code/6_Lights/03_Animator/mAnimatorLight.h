#ifndef _AnimatorDALIGHTNEOPIXEL_H
#define _AnimatorDALIGHTNEOPIXEL_H 7.0

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 6003

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#include "6_Lights/00_Interface/mBusManager.h"

#include "2_CoreSystem/02_Time/Toki.h"


// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
#define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Should nearly always be enabled as default/minimal cases
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS


#define PIXEL_RANGE_LIMIT 3000

#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
  #ifndef PIXEL_COUNTS
    #define PIXEL_COUNTS DEFAULT_LED_COUNT
  #endif
  #ifndef DATA_PINS
    #define DATA_PINS LEDPIN
  #endif
  #define DATA_PINS_BUSSES 23, 22
  #ifndef DEFAULT_LED_TYPE
    #define DEFAULT_LED_TYPE BUSTYPE_WS2812_RGB
  #endif
  #ifndef DEFAULT_LED_COLOR_ORDER
    #define DEFAULT_LED_COLOR_ORDER COL_ORDER_GRB  //default to GRB
  #endif
#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

#define MAX_PIXELBUS_DIGITAL_PINS 16

/**
 * @brief GetColourPalette defines to make it visually easy to read
 **/
#define PALETTE_WRAP_ON                 true
#define PALETTE_WRAP_OFF                false
#define PALETTE_DISCRETE_OFF            false
#define PALETTE_DISCRETE_ON             true
#define PALETTE_DISCRETE_DEFAULT        2 // Use the prefered method depending on the palette. Gradients will be shown across the segment, discrete will be shown as a single colours sequenced
#define PALETTE_INDEX_SPANS_SEGLEN_ON   true
#define PALETTE_SPAN_OFF                false
#define WLED_PALETTE_MAPPING_ARG_FALSE  false
#define NO_ENCODED_VALUE                nullptr
#define PALETTE_SOLID_WRAP              (paletteBlend == 1 || paletteBlend == 3)
#define SET_BRIGHTNESS                  true
#define BRIGHTNESS_ALREADY_SET          true

/**
 * WLED conversions
 * These are basic defines that remap temporarily from WLED in PulSar. 
 * For optimisation, these will be removed and replaced with the correct values.
 **/
#define color_from_palette(a,b,c,d)    GetPaletteColour(a,b,c,d).getU32()
// #define ColorFromPalette_WithLoad3(a,b,c)   (if( SEGMENT.palette_id != SEGMENT.palette_container->loaded_palette_id){ LoadPalette(SEGMENT.palette_id, getCurrSegmentId()); ColorFromPalette(a,b,c); })
// #define ColorFromPalette_WithLoad4(a,b,c,d) (if( SEGMENT.palette_id != SEGMENT.palette_container->loaded_palette_id){ LoadPalette(SEGMENT.palette_id, getCurrSegmentId()); ColorFromPalette(a,b,c,d); })


#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/* Not used in all effects yet */
#define WLED_FPS         42
#define FRAMETIME_FIXED  (1000/WLED_FPS)
#define FRAMETIME_MS     24
#define FRAMETIME        25//getFrameTime()

/* Each segment uses 52 bytes of SRAM memory, so if you're application fails because of insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#ifdef ESP8266
  #define MAX_NUM_SEGMENTS    3
  /* How much data bytes all segments combined may allocate */
  #define MAX_SEGMENT_DATA  1000
#else
  #ifndef MAX_NUM_SEGMENTS
    #define MAX_NUM_SEGMENTS  6
  #endif
  #define MAX_SEGMENT_DATA  32767
#endif

#define IBN 5100

/* How much data bytes each segment should max allocate to leave enough space for other segments,
  assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
// #define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / strip.getMaxSegments())

#define MIN_SHOW_DELAY   (_frametime < 16 ? 8 : 15)


// options
// bit    7: segment is in transition mode
// bits 4-6: TBD
// bit    3: mirror effect within segment
// bit    2: segment is on
// bit    1: reverse segment
// bit    0: segment is selected
#define NO_OPTIONS   (uint8_t)0x00
#define TRANSITIONAL (uint8_t)0x80
#define MIRROR       (uint8_t)0x08
#define SEGMENT_ON   (uint8_t)0x04
#define REVERSE      (uint8_t)0x02
#define SELECTED     (uint8_t)0x01
#define IS_TRANSITIONAL ((SEGMENT_I(_segment_index_primary).options & TRANSITIONAL) == TRANSITIONAL)
#define IS_MIRROR       ((SEGMENT_I(_segment_index_primary).options & MIRROR      ) == MIRROR      )
#define IS_SEGMENT_ON   ((SEGMENT_I(_segment_index_primary).options & SEGMENT_ON  ) == SEGMENT_ON  )
#define IS_REVERSE      ((SEGMENT_I(_segment_index_primary).options & REVERSE     ) == REVERSE     )
#define IS_SELECTED     ((SEGMENT_I(_segment_index_primary).options & SELECTED    ) == SELECTED    )

// #define MIN_SHOW_DELAY   (_frametime < 16 ? 8 : 15)
#define MINIMUM_SHOW_BACKOFF_PERIOD_MS 30//15

/* How much data bytes all segments combined may allocate */
#ifdef ESP8266
#define MAX_SEGMENT_DATA 2048
#else
// #define MAX_SEGMENT_DATA 8192
// Really this value should be split across all segments, since I may want one very large (2000+) pixel or many smaller segments but multiples of them.
#define MAX_SEGMENT_DATA 19000 //6*MAX_PIXELS = 
#endif

// #define ANIMATOR_UNSET() (seg.anim_function_callback == nullptr)

#define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
#define MIN_SHOW_DELAY  15
#define DEFAULT_LED_COUNT 30

#define DEFAULT_BRIGHTNESS (uint8_t)127
#define DEFAULT_MODE       (uint8_t)0
#define DEFAULT_SPEED      (uint8_t)128
#define DEFAULT_INTENSITY  (uint8_t)128
#define DEFAULT_COLOR      (uint32_t)0xFFAA00
#define DEFAULT_C1         (uint8_t)128
#define DEFAULT_C2         (uint8_t)128
#define DEFAULT_C3         (uint8_t)16


//Segment option byte bits
#define SEG_OPTION_SELECTED       0
#define SEG_OPTION_REVERSED       1
#define SEG_OPTION_ON             2
#define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
#define SEG_OPTION_NONUNITY       4            //Indicates that the effect does not use FRAMETIME_MS or needs getPixelColor
#define SEG_OPTION_TRANSITIONAL   7

// some common colors
// white, red, green, blue
#define RED        (uint32_t)0xFF0000
#define GREEN      (uint32_t)0x00FF00
#define BLUE       (uint32_t)0x0000FF
#define WHITE      (uint32_t)0xFFFFFF
#define BLACK      (uint32_t)0x000000
#define YELLOW     (uint32_t)0xFFFF00
#define CYAN       (uint32_t)0x00FFFF
#define MAGENTA    (uint32_t)0xFF00FF
#define PURPLE     (uint32_t)0x400080
#define ORANGE     (uint32_t)0xFF3000
#define PINK       (uint32_t)0xFF1493
#define ULTRAWHITE (uint32_t)0xFFFFFFFF
#define DARKSLATEGRAY (uint32_t)0x2F4F4F
#define DARKSLATEGREY (uint32_t)0x2F4F4F

#define NUM_COLORS 3 

#define SEGIDX           getCurrSegmentId()

/**
 * @brief 
 * Note: WLED used full colour RGB SEGCOLOR_U32, here SEGCOLOR_U32 going forward will also have brightness applied otherwise it is assumed as 100% brightness and therefore the same as WLED
 * 
 */
#define SEGCOLOR_U32(x)      RgbcctColor::GetU32ColourBrightnessApplied(segments[getCurrSegmentId()].rgbcctcolors[x])
#define SEGCOLOR_RGBCCT(x)   segments[getCurrSegmentId()].rgbcctcolors[x].GetColourWithBrightnessApplied()

#define SEGMENT          segments[getCurrSegmentId()]
#define SEGMENT_I(X)     segments[X]
#define pSEGMENT_I(X)    pCONT_lAni->segments[X]
#define SEGLEN           _virtualSegmentLength // This is still using the function, it just relies on calling the function prior to the effect to set this
#define SEGPALETTE       SEGMENT.palette_container->CRGB16Palette16_Palette.data

#define SPEED_FORMULA_L  5U + (50U*(255U - SEGMENT._speed))/SEGLEN


#include "6_Lights/02_Palette/mPalette_Progmem.h"
#include "6_Lights/02_Palette/mPalette.h"
#include "6_Lights/02_Palette/mPaletteLoaded.h"
#include "6_Lights/00_Interface/mInterfaceLight.h"

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
    #include <SPIFFSEditor.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif

#include <DNSServer.h>

#include "math.h"
#include "fastmath.h"

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/02_Palette/mPaletteLoaded.h"
class mPaletteLoaded;

#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/00_Interface/FastLED/FastLED.h"


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR)       "segments";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MATRIX_CTR)       "matrix";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR)              "playlists";
#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR)        "mode_ambilight";
#endif
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR)   "mode_setpixel";
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR)   "presets";
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PLAYLISTS_CTR)   "playlists";
#endif
#ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SEQUENCER)   "sequencer";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR)         "debug/palette";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS__CTR)        "debug/segments";
#endif 
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE_VECTOR__CTR)        "debug/palette_vector";
#endif 
#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR)    "debug/animation_progress";
#endif 


#ifdef ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define ARDUINOJSON_DECODE_UNICODE 0
  #include "3_Network/21_WebServer/ArduinoJson-v6.h"
#endif // ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6


#ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI
  #include "3_Network/21_WebServer/AsyncJson-v6.h"
  #include "webpages_generated/html_ui.h"
  #ifdef WLED_ENABLE_SIMPLE_UI
    #include "webpages_generated/html_simple.h"
  #endif
  #include "webpages_generated/html_settings.h"
  #include "webpages_generated/html_other.h"
  #ifdef WLED_ENABLE_PIXART
    #include "webpages_generated/html_pixart.h"
  #endif
  #ifndef WLED_DISABLE_PXMAGIC
    #include "webpages_generated/html_pxmagic.h"
  #endif
  #include "webpages_generated/html_cpal.h"
#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI


#include <functional>
#define ANIM_FUNCTION_SIGNATURE                             std::function<void(const AnimationParam& param)>                              anim_function_callback
#define ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE           std::function<void()>                                                         anim_progress_mqtt_function_callback
#define ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED             std::function<void(const uint8_t segment_index, const AnimationParam& param)> anim_function_callback_indexed
#define SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR()  SEGMENT.anim_function_callback = nullptr 


class mAnimatorLight :
  public mTaskerInterface
{

  public:
  /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mAnimatorLight(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);
    
    static const char* PM_MODULE_LIGHTS_ANIMATOR_CTR;
    static const char* PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_ANIMATOR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mAnimatorLight);    };
    #endif

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct DATA_STORAGE
    {

    }
    dt;


    struct DATA_RUNTIME
    {

    }
    rt;

    
    uint8_t light_power_state = 0;
    uint8_t light_power_Saved = 0;
    bool    pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer
    bool    fade_initialized = false;      // dont't fade at startup
    bool doInitBusses = false; // debug
    bool     doSerializeConfig = false; // debug
    int8_t loadLedmap = -1;
    struct AUTO_OFF_SETTINGS{
      uint16_t time_decounter_secs = 0;
    }auto_off_settings;

    uint16_t fAnyLEDsOnOffCount = 0;
    uint8_t blocking_force_animate_to_complete = true;
    uint8_t fPixelsUpdated = false;
    uint16_t desired_pixel;

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Restore_Module(void);
    void FileSystem_JsonAppend_Save_Module();

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void StartAnimation_AsAnimUpdateMemberFunction();
    
    const char* GetAnimationStatusCtr(char* buffer, uint8_t buflen);
            
    #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    byte correctionRGB[4] = {0,0,0,0};
    uint16_t lastKelvin = 0;
    void colorKtoRGB(uint16_t kelvin, byte* rgb);
    uint32_t colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb);
    uint16_t approximateKelvinFromRGB(uint32_t rgb);
    #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

    void EveryLoop();    
    #ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();
    #endif // rules
     
    void EverySecond_AutoOff();
    void BootMessage();
                

    RgbcctColor ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness);
    
    #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
    void Test_Config();
    #endif

    #ifdef ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION
    struct POWER_RATING{
      float Average_mA_Per_Pixel_Step = 0.0784; //  20/255    //MAKE PROGMEM
      // uint8_t G_Channel_mA = 20;
      // uint8_t B_Channel_mA = 20;
      // uint8_t W_Channel_mA = 20;
      float current_mA;
      uint8_t power;
    }power_rating;

    template<typename T>
    void swap(T& x, T& y)
    {
      T tmp = x;
      x = y;
      y = tmp;
    }

    timereached_t tSavedCalculatePowerUsage;
    #endif // ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION


    /************************************************************************************************
     * SECTION: Modes
     ************************************************************************************************/

    enum ANIMATION_MODE
    {
      ANIMATION_MODE__DISABLED,      
      ANIMATION_MODE__EFFECTS,       // PulSar and WLED
      ANIMATION_MODE__MQTT_SETPIXEL, // Using json encoded message to set the pixels directly
      ANIMATION_MODE__REALTIME_UDP,
      ANIMATION_MODE__REALTIME_HYPERION,
      ANIMATION_MODE__REALTIME_E131,
      #ifdef ENABLE_FEATURE_PIXEL_MODE__REALTIME_ADALIGHT
      ANIMATION_MODE__REALTIME_ADALIGHT,
      #endif
      ANIMATION_MODE__REALTIME_ARTNET,
      ANIMATION_MODE__REALTIME_TPM2NET,
      ANIMATION_MODE__REALTIME_DDP,  
      ANIMATION_MODE__LENGTH_ID
    };             
    
    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE
    int8_t GetAnimationModeIDbyName(const char* c);
    const char* GetAnimationModeName(char* buffer, uint16_t buflen);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen);
    void CommandSet_AnimationModeID(uint8_t value);
    #endif // ENABLE_DEVFEATURE_LIGHTING__COMMANDS_CHANGE_ANIMATION_MODE


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    uint8_t subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index = 255);    
    
    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION_SHORTCUT
    void parsesub_json_object_notification_shortcut(JsonParserObject obj);
    #endif

    void CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length);
    void CommandSet_LightPowerState(uint8_t value);
    bool CommandGet_LightPowerState();
    void CommandSet_Auto_Time_Off_Secs(uint16_t value);
   
    void CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index = 0);
    const char* GetColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen);

    #ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
    void CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index = 0, uint8_t palette_id = 255);
    #endif

    void Set_Segment_ColourType(uint8_t segment_index, uint8_t light_type);

    void CommandSet_PaletteID(uint8_t value, uint8_t segment_index = 0);

    void CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index = 0);
    int8_t GetFlasherFunctionIDbyName(const char* f);
    const char* GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index = 0);
    const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen, bool return_first_option_if_not_found = false);

    void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index = 0);

    void CommandSet_HardwareColourOrderTypeByStr(const char* value, uint8_t segment_index = 0);
    void CommandSet_ColourTypeByStr(const char* value, uint8_t segment_index = 0);
    // const char* GetHardwareColourTypeName(char* buffer, uint8_t buflen, uint8_t segment_index= 0);
    // const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen, uint8_t segment_index= 0);

    void CommandSet_Animation_Transition_Time_Ms(uint16_t value, uint8_t segment_index= 0);
    void CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index= 0);
  
    void CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index = 0);
    void CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index = 0);
    
    
    void CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new , uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_Manual(uint8_t* values, uint8_t value_count, uint8_t colour_index = 0, uint8_t segment_index = 0);


    uint8_t GetNumberOfColoursInPalette(uint16_t palette_id);

    /************************************************************************************************
     * SECTION: Files
     ************************************************************************************************/


    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
    **  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/


    void fill(uint32_t c, bool apply_brightness = false);
    void fill(RgbcctColor c, bool apply_brightness = false);
    void fill_ranged(uint32_t c, bool apply_brightness = false); 

    uint32_t color_wheel(uint8_t pos);
    uint32_t ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend);

    void Init_Pins();
    void Init_Segments();



    #ifdef ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    #endif // ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6


    #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS


    volatile byte presetToApply = 0;
    volatile byte callModeToApply = 0;
    volatile byte presetToSave = 0;
    volatile int8_t saveLedmap = -1;
    char quickLoad[9] = {0};
    char saveName[33] = {0};
    bool includeBri = true, segBounds = true, selectedOnly = false, playlistSave = false;;

    //Notifier callMode
    #define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
    #define CALL_MODE_DIRECT_CHANGE  1
    #define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
    #define CALL_MODE_NOTIFICATION   3
    #define CALL_MODE_NIGHTLIGHT     4
    #define CALL_MODE_NO_NOTIFY      5
    #define CALL_MODE_FX_CHANGED     6     //no longer used
    #define CALL_MODE_HUE            7
    #define CALL_MODE_PRESET_CYCLE   8
    #define CALL_MODE_BLYNK          9     //no longer used
    #define CALL_MODE_ALEXA         10
    #define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
    #define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

    const char *getFileName(bool persist = true);
    void doSaveState();
    bool getPresetName(byte index, String& name);
    void initPresetsFile();
    bool applyPreset(byte index, byte callMode = CALL_MODE_DIRECT_CHANGE);
    void applyPresetWithFallback(uint8_t presetID, uint8_t callMode, uint8_t effectID = 0, uint8_t paletteID = 0);
    void handlePresets();
    inline bool applyTemporaryPreset() {return applyPreset(255);};

    #ifdef ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
    void savePreset(byte index, const char* pname = nullptr, JsonObject saveobj = JsonObject());
    inline void saveTemporaryPreset() {savePreset(255);};
    #endif // ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

    void deletePreset(byte index);
    #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS

    /******************************************************************************************************************************************************************************
    **** Playlists ***************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS

    typedef struct PlaylistEntry 
    {
      uint8_t preset; //ID of the preset to apply
      uint16_t dur;   //Duration of the entry (in tenths of seconds)
      uint16_t tr;    //Duration of the transition TO this entry (in tenths of seconds)
    } ple;


    byte           playlistRepeat = 1;        //how many times to repeat the playlist (0 = infinitely)
    byte           playlistEndPreset = 0;     //what preset to apply after playlist end (0 = stay on last preset)
    byte           playlistOptions = 0;       //bit 0: shuffle playlist after each iteration. bits 1-7 TBD

    PlaylistEntry *playlistEntries = nullptr;
    byte           playlistLen;               //number of playlist entries
    int8_t         playlistIndex = -1;
    uint16_t       playlistEntryDur = 0;      //duration of the current entry in tenths of seconds


    void shufflePlaylist();
    void unloadPlaylist();
    int16_t loadPlaylist(JsonObject playlistObj, byte presetId);
    void handlePlaylist();
    void serializePlaylist(JsonObject sObj);
    
    int16_t currentPlaylist = -1;
    

    #endif // ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS

    #ifdef ENABLE_DEVFEATURE_LIGHTING__SETTINGS
    //set.cpp
    bool isAsterisksOnly(const char* str, byte maxLen);
    void handleSettingsSet(AsyncWebServerRequest *request, byte subPage);
    bool handleSet(AsyncWebServerRequest *request, const String& req, bool apply=true);
    #endif // ENABLE_DEVFEATURE_LIGHTING__SETTINGS



    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ****** Effect Functions *************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Solid_Colour(); 
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Static_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Slow_Glow();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Flicker_Base(bool use_multi = false, uint16_t flicker_palette = 0);
    void EffectAnim__Candle_Single();
    void EffectAnim__Candle_Multiple();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Shimmering_Two_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Shimmering_Palette_Saturation();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Static_Gradient_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Stepping_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__TimeBased__HourProgress();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Static_Palette_Vintage();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Stepping_Palette_With_Background();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    void EffectAnim__Popping_Decay_Palette_To_Black();
    void EffectAnim__Popping_Decay_Random_To_Black();
    void EffectAnim__Popping_Decay_Palette_To_White();
    void EffectAnim__Popping_Decay_Random_To_White();
    void EffectAnim__Popping_Decay_Base(bool draw_palette_inorder, bool fade_to_black);
    #endif 
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    void EffectAnim__Spanned_Static_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Rotating_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void EffectAnim__Rotating_Previous_Animation();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    void Segments_RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction);
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    // void EffectAnim__Palette_Colour_Fade_Saturation();
    // #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    void EffectAnim__Blend_Two_Palettes();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    void EffectAnim__Twinkle_Palette_Onto_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    void EffectAnim__Twinkle_Out_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    void EffectAnim__Twinkle_Decaying_Palette();
    #endif
    // Static
    void EffectAnim__Static_Pattern();
    void EffectAnim__Tri_Static_Pattern();
    void EffectAnim__Base_Spots(uint16_t threshold);
    void EffectAnim__Spots();
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    void EffectAnim__Percent();
    #endif
    // One colour changes
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    void EffectAnim__Random_Colour();
    #endif
    // Wipe/Sweep/Runners 
    void BaseEffectAnim__Base_Colour_Wipe(bool rev, bool useRandomColors, bool useIterateOverPalette = false);
    void EffectAnim__Colour_Wipe();
    void EffectAnim__Colour_Wipe_Random();
    void EffectAnim__Colour_Wipe_Palette();
    void EffectAnim__Colour_Sweep();
    void EffectAnim__Colour_Sweep_Random();
    void EffectAnim__Colour_Sweep_Palette();
    void EffectAnim__TriColour();
    void EffectAnim__Android();
    void EffectAnim__Base_Running(bool saw);
    void EffectAnim__Base_Running(uint32_t color1, uint32_t color2);
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    void EffectAnim__Running_Colour();
    void EffectAnim__Running_Random();
    void EffectAnim__Base_Gradient(bool loading);
    void EffectAnim__Gradient();
    void EffectAnim__Loading();
    void EffectAnim__Base_Police(uint32_t color1, uint32_t color2, bool all);
    void EffectAnim__Police();
    void EffectAnim__Polce_All();
    void EffectAnim__Two_Dots();
    void EffectAnim__Two_Areas();
    void EffectAnim__Multi_Comet();
    void EffectAnim__Oscillate();
    void EffectAnim__BPM();
    void EffectAnim__Juggle();
    void EffectAnim__Palette();
    void EffectAnim__ColourWaves();
    void EffectAnim__Lake();
    void EffectAnim__Glitter();
    void EffectAnim__Meteor();
    void EffectAnim__Metoer_Smooth();    
    void EffectAnim__Pride_2015();    
    CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);   
    void EffectAnim__Pacifica();    
    void EffectAnim__Sunrise();    
    void EffectAnim__Sinewave();    
    void EffectAnim__Flow();    
    void EffectAnim__Base_Phased(uint8_t moder);
    void EffectAnim__PhasedNoise();    
    void EffectAnim__Phased();    
    void EffectAnim__Running_Lights();    
    void EffectAnim__Rainbow_Cycle();    
    // Chase    
    void EffectAnim__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette);
    void EffectAnim__Chase_Colour();
    void EffectAnim__Chase_Random();
    void EffectAnim__Chase_Rainbow();
    void EffectAnim__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette);
    void EffectAnim__Chase_Flash();
    void EffectAnim__Chase_Flash_Random();
    void EffectAnim__Chase_Rainbow_White();
    void EffectAnim__Chase_Theater();
    void EffectAnim__Chase_Theatre_Rainbow();
    void EffectAnim__Base_Chase_TriColour(uint32_t color1, uint32_t color2);
    void EffectAnim__Chase_TriColour();
    // Breathe/Fade/Pulse
    void EffectAnim__Breath();
    void EffectAnim__Fade();
    void EffectAnim__Fade_TriColour();
    void EffectAnim__Fade_Spots();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    // Fireworks
    void EffectAnim__Fireworks();
    void EffectAnim__Exploding_Fireworks();
    void EffectAnim__Fireworks_Starburst();
    void EffectAnim__Fireworks_Starburst_Glows();
    void EffectAnim__Rain();
    void EffectAnim__Exploding_Fireworks_NoLaunch();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    // Sparkle/Twinkle
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    void EffectAnim__Solid_Glitter();
    void EffectAnim__Popcorn();
    void EffectAnim__Plasma();
    void EffectAnim__Sparkle();
    void EffectAnim__Sparkle_Flash();
    void EffectAnim__Sparkle_Hyper();
    void EffectAnim__Twinkle();
    CRGB EffectAnim__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat);
    void EffectAnim__Base_Twinkle_Fox(bool cat);
    void EffectAnim__Twinkle_Colour();
    void EffectAnim__Twinkle_Fox();
    void EffectAnim__Twinkle_Cat();
    void EffectAnim__Twinkle_Up();
    void EffectAnim__Saw();
    void EffectAnim__Base_Dissolve(uint32_t color);
    void EffectAnim__Dissolve();
    void EffectAnim__Dissolve_Random();
    void EffectAnim__ColourFul();
    void EffectAnim__Traffic_Light();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE  
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    // Blink/Strobe
    void EffectAnim__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette);
    void EffectAnim__Blink();
    void EffectAnim__Blink_Rainbow();
    void EffectAnim__Strobe();
    void EffectAnim__Strobe_Multi();
    void EffectAnim__Strobe_Rainbow();
    void EffectAnim__Rainbow();
    void EffectAnim__Lightning();
    void EffectAnim__Fire_2012();
    void EffectAnim__Railway();
    void EffectAnim__Heartbeat();
    //Noise
    void EffectAnim__FillNoise8();
    void EffectAnim__Noise16_1();
    void EffectAnim__Noise16_2();
    void EffectAnim__Noise16_3();
    void EffectAnim__Noise16_4();
    void EffectAnim__Noise_Pal();
    // Scan
    void EffectAnim__Base_Scan(bool dual);
    void EffectAnim__Scan();
    void EffectAnim__Scan_Dual();
    void EffectAnim__Base_Larson_Scanner(bool dual);
    void EffectAnim__Larson_Scanner();
    void EffectAnim__Larson_Scanner_Dual();
    void EffectAnim__ICU();
    void EffectAnim__Base_Ripple(bool rainbow);
    void EffectAnim__Ripple();
    void EffectAnim__Ripple_Rainbow(); 
    void EffectAnim__Comet();
    void EffectAnim__Chunchun();
    void EffectAnim__Bouncing_Balls();
    void EffectAnim__Base_Sinelon(bool dual, bool rainbow=false);
    void EffectAnim__Sinelon();
    void EffectAnim__Sinelon_Dual();
    void EffectAnim__Sinelon_Rainbow();
    void EffectAnim__Drip();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
    void EffectAnim__Hardware__Show_Bus();
    void EffectAnim__Hardware__Manual_Pixel_Counting();
    void EffectAnim__Hardware__View_Pixel_Range();
    void EffectAnim__Hardware__Light_Sensor_Pixel_Indexing();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    void EffectAnim__SunPositions__Sunrise_Alarm_01();
    void EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01();
    void EffectAnim__SunPositions__Sunset_Blended_Palettes_01();
    void EffectAnim__SunPositions__DrawSun_1D_Elevation_01();
    void EffectAnim__SunPositions__DrawSun_1D_Azimuth_01();
    void EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01();
    void EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
    void EffectAnim__Manual__PixelSetElsewhere();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    void SubTask_Flasher_Animate_Function_Tester_01();
    void SubTask_Flasher_Animate_Function_Tester_02();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
    void EffectAnim__BorderWallpaper__TwoColour_Gradient();
    void EffectAnim__BorderWallpaper__FourColour_Gradient();
    void EffectAnim__BorderWallpaper__FourColour_Solid();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_TRACKING
    void EffectAnim__SolarTriggers__Sunrise_01();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_TRACKING
    
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    void EffectAnim__Christmas_Musical__01();
    #endif 
    

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
    *** Specialised: 2D (No Audio) **********************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
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
    void EffectAnim__2D__DistortionWaves();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    void EffectAnim__2D__Soap();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    void EffectAnim__2D__Octopus();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    void EffectAnim__2D__WavingCell();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    void EffectAnim__2D__ScrollingText();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    void EffectAnim__2D__DNA();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: 1D (Audio Reactive) ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
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
    void EffectAnim__AudioReactive__1D__FFT_Waterfall();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: 2D (Audio Reactive) ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    void EffectAnim__AudioReactive__2D__Swirl();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    void EffectAnim__AudioReactive__2D__Waverly();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    void EffectAnim__AudioReactive__2D__FFT_GED();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    void EffectAnim__AudioReactive__2D__FFT_FunkyPlank();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    void EffectAnim__AudioReactive__2D__FFT_Akemi();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: Notifcations ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
    void SubTask_Segment_Animate_Function__Notification_Static();
    void SubTask_Segment_Animate_Function__Notification_Fade();
    void SubTask_Segment_Animate_Function__Notification_Blinking();
    void SubTask_Segment_Animate_Function__Notification_Pulsing();
    void SubTask_Segment_Animate_Function__Notification_Base(bool flag_static = false, bool flag_blink = false, bool flag_pulse = false);
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
    /****************************************************************************************************************************************************************************
    *** Specialised: Notifcations ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    
    #define LED_DIGITS 4                             // 4 or 6 digits, can only be an even number as...
    // #define LED_PER_DIGITS_STRIP 47                  // ...two digits are made out of one piece of led strip with 47 leds...
    #define LED_PER_DIGITS_STRIP 44                  // OR...two digits are made out of TWO pieces of led strip with 44 leds...
    #define LED_BETWEEN_DIGITS_STRIPS 5              // 5 leds between above strips - and all this gives us LED_COUNT... :D
    #define LED_COUNT ( LED_DIGITS / 2 ) * LED_PER_DIGITS_STRIP + ( LED_DIGITS / 3 ) * LED_BETWEEN_DIGITS_STRIPS

    /* Segment order, seen from the front:

        <  A  >
      /\       /\
      F        B
      \/       \/
        <  G  >
      /\       /\
      E        C
      \/       \/
        <  D  >

    */

    byte segGroups[14][2] = {         // 14 segments per strip, each segment has 1-x led(s). So lets assign them in a way we get something similar for both digits
      // right (seen from front) digit. This is which led(s) can be seen in which of the 7 segments (two numbers: First and last led inside the segment, same on TE):
      { 13, 15 },                     // top, a
      { 10, 12 },                     // top right, b
      {  6, 8  },                     // bottom right, c
      {  3, 5  },                     // bottom, d
      {  0, 2  },                     // bottom left, e
      { 16, 18 },                     // top left, f
      { 19, 21 },                     // center, g
      // left (seen from front) digit
      { 35, 37 },                     // top, a
      { 38, 40 },                     // top right, b
      { 22, 24 },                     // bottom right, c
      { 25, 27 },                     // bottom, d
      { 28, 30 },                     // bottom left, e
      { 32, 34 },                     // top left, f
      { 41, 43 }                      // center, g
    };

    // Using above arrays it's very easy to "talk" to the segments. Simply use 0-6 for the first 7 segments, add 7 (7-13) for the following ones per strip/two digits
    byte digits[14][7] = {                    // Lets define 10 numbers (0-9) with 7 segments each, 1 = segment is on, 0 = segment is off
      {   1,   1,   1,   1,   1,   1,   0 },  // 0 -> Show segments a - f, don't show g (center one)
      {   0,   1,   1,   0,   0,   0,   0 },  // 1 -> Show segments b + c (top and bottom right), nothing else
      {   1,   1,   0,   1,   1,   0,   1 },  // 2 -> and so on...
      {   1,   1,   1,   1,   0,   0,   1 },  // 3
      {   0,   1,   1,   0,   0,   1,   1 },  // 4
      {   1,   0,   1,   1,   0,   1,   1 },  // 5
      {   1,   0,   1,   1,   1,   1,   1 },  // 6
      {   1,   1,   1,   0,   0,   0,   0 },  // 7
      {   1,   1,   1,   1,   1,   1,   1 },  // 8
      {   1,   1,   1,   1,   0,   1,   1 },  // 9
      {   0,   0,   0,   1,   1,   1,   1 },  // t -> some letters from here on (index 10-13, so this won't interfere with using digits 0-9 by using index 0-9
      {   0,   0,   0,   0,   1,   0,   1 },  // r
      {   0,   1,   1,   1,   0,   1,   1 },  // y
      {   0,   1,   1,   1,   1,   0,   1 }   // d
    };


    uint16_t lcd_display_show_number = 0;
    char lcd_display_show_string[5] = {0}; //temporary solution, will be removed once newer commend to save effect runtime struct works
    byte displayMode = 1;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)
    byte lastSecond = 0;
    byte startColor = 0;                             // "index" for the palette color used for drawing
    byte colorOffset = 32;                           // default distance between colors on the color palette used between digits/leds (in overlayMode)


    void LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing);
    void LCDDisplay_showDigit(byte digit, byte color, byte pos);
    void LCDDisplay_showSegment(byte segment, byte color, byte segDisplay);
    void LCDDisplay_showDots(byte dots, byte color);

    void EffectAnim__7SegmentDisplay__ClockTime_01();
    void EffectAnim__7SegmentDisplay__ClockTime_02();
    void EffectAnim__7SegmentDisplay__ManualNumber_01();
    void EffectAnim__7SegmentDisplay__ManualString_01();
    void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
    void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();

    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
      

    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ****** Effect Enums *************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/



    enum EFFECTS_FUNCTION__IDS
    {
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__SOLID_COLOUR__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__STATIC_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__SPANNED_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__SLOW_GLOW__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__CANDLE_SINGLE__ID,
    EFFECTS_FUNCTION__CANDLE_MULTIPLE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__SHIMMERING_PALETTE__ID,
    EFFECTS_FUNCTION__SHIMMERING_PALETTE_DOUBLE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__TIMEBASED__HOUR_PROGRESS__ID,
    #endif    
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__STATIC_PALETTE_VINTAGE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION_BLENDED__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__STEPPING_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__STEPPING_PALETTE_WITH_BACKGROUND__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__TWINKLE_OFF_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS 
    EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID,
    EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
    EFFECTS_FUNCTION__NOTIFICATION_FADE__ID,
    EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,    
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
        
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_BLACK__ID,
    EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_BLACK__ID,
    EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_WHITE__ID,
    EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_WHITE__ID,
    #endif
        
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01_ID,
    #endif

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Christmas Multifunction Effects  ***************************************************************************************************************************************************************************
    **  Replicate how traditional 2/4 ouput controllers and their effects look
          - Option1: Make the same LEDs maintain colour, as if they are painted glass bulbs. So a sequence of 4 RGBO would mean every 1 pixel in order would be turned off
          - Option2: Replicate the same effects, but keep as many LEDs on as possible.
    **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/  
    /**
     * Palette is drawn "inorder" and animations are drawn with X outputs (4 or 5 like normal lights) to create the effect like real lights
     *
      1 - Combination (omited, can be performed using mixer)
      2 - In Waves
      3 - Sequentials
      4 - Slo-Glo
      5 - Chasing / Flash
      6 - Slow Fade
      7 - Twinkle / Flash
      8 - Steady on (already added)
     * */
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL__FUNCTION_CONTROLLER_IN_WAVES_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL__FUNCTION_CONTROLLER__SEQUENTIAL_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL__FUNCTION_CONTROLLER__SLO_GLO_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL__FUNCTION_CONTROLLER__CHASING_AND_FLASHING_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL__FUNCTION_CONTROLLER__SLOW_FADE_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL__FUNCTION_CONTROLLER__TWINKLE_AND_FLASH_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL____XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL____XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL____XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    // EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__  ID,
    #endif
    /**
     * @brief Turn random Lights off, but when they are off (and hence shunted from in series) the rest of the LEDs in that region should get brighter
     * Note: Part of why these look good is the inherent initial brightness from a cold incandenct bulb turning on, then stabilising. 
     * This will also be the case for the other traditional christmas effects... not sure how to programmatically replicate this
     * Option1: Randomly turn LEDs off, entire string gets marginally brighter
     * Option2: Divide all LED string into sections of 8 (ie. 24V filament sets) and randomly turn 1 led off in each section, and that section brightness changes
     * 
     */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__CHRISTMAS_TRADITIONAL_TWINKLE_LIGHTS_ID,
    #endif


    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: WLED Animations converted  ***************************************************************************************************************************************************************************
    **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/  
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    /**
     * Static
     **/
    EFFECTS_FUNCTION__STATIC_PATTERN__ID,
    EFFECTS_FUNCTION__TRI_STATIC_PATTERN__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__SPOTS__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__PERCENT__ID,
    #endif
    /**
     * One Colour
     **/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__RANDOM_COLOR__ID,
    #endif
    /**
     * Wipe/Sweep/Runners 
     **/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__COLOR_WIPE__ID,
    EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID, // 1 direction only
    EFFECTS_FUNCTION__COLOR_WIPE_PALETTE__ID, // 1 direction only
    EFFECTS_FUNCTION__COLOR_SWEEP__ID,
    EFFECTS_FUNCTION__COLOR_SWEEP_RANDOM__ID, //start to end to start again
    EFFECTS_FUNCTION__COLOR_SWEEP_PALETTE__ID, //start to end to start again
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     * Fireworks
     **/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__FIREWORKS__ID,
    EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID,
    EFFECTS_FUNCTION__FIREWORKS_STARBURST__ID,
    EFFECTS_FUNCTION__FIREWORKS_STARBURST_GLOWS__ID,
    EFFECTS_FUNCTION__RAIN__ID,
    EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__TRICOLOR_WIPE__ID,
    EFFECTS_FUNCTION__ANDROID__ID,
    EFFECTS_FUNCTION__RUNNING_COLOR__ID,
    EFFECTS_FUNCTION__RUNNING_RANDOM__ID,
    EFFECTS_FUNCTION__GRADIENT__ID,
    EFFECTS_FUNCTION__LOADING__ID,
    EFFECTS_FUNCTION__POLICE__ID,
    EFFECTS_FUNCTION__POLICE_ALL__ID,
    EFFECTS_FUNCTION__TWO_DOTS__ID,
    EFFECTS_FUNCTION__TWO_AREAS__ID,
    EFFECTS_FUNCTION__MULTI_COMET__ID,
    EFFECTS_FUNCTION__OSCILLATE__ID,
    EFFECTS_FUNCTION__BPM__ID,
    EFFECTS_FUNCTION__JUGGLE__ID,
    EFFECTS_FUNCTION__PALETTE__ID,
    EFFECTS_FUNCTION__COLORWAVES__ID,
    EFFECTS_FUNCTION__LAKE__ID,
    EFFECTS_FUNCTION__GLITTER__ID,
    EFFECTS_FUNCTION__METEOR__ID, 
    EFFECTS_FUNCTION__METEOR_SMOOTH__ID,
    EFFECTS_FUNCTION__PRIDE_2015__ID,
    EFFECTS_FUNCTION__PACIFICA__ID,
    EFFECTS_FUNCTION__SUNRISE__ID,
    EFFECTS_FUNCTION__SINEWAVE__ID,
    EFFECTS_FUNCTION__FLOW__ID,
    EFFECTS_FUNCTION__RUNNING_LIGHTS__ID,
    EFFECTS_FUNCTION__RAINBOW_CYCLE__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     * Chase
     **/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__CHASE_COLOR__ID,
    EFFECTS_FUNCTION__CHASE_RANDOM__ID,
    EFFECTS_FUNCTION__CHASE_RAINBOW__ID, 
    EFFECTS_FUNCTION__CHASE_FLASH__ID,
    EFFECTS_FUNCTION__CHASE_FLASH_RANDOM__ID, 
    EFFECTS_FUNCTION__CHASE_RAINBOW_WHITE__ID,
    EFFECTS_FUNCTION__CHASE_THEATER__ID,
    EFFECTS_FUNCTION__CHASE_THEATER_RAINBOW__ID,
    EFFECTS_FUNCTION__CHASE_TRICOLOR__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     *  Breathe/Fade/Pulse
     **/    
    EFFECTS_FUNCTION__BREATH__ID,
    EFFECTS_FUNCTION__FADE__ID,
    EFFECTS_FUNCTION__FADE_TRICOLOR__ID,
    EFFECTS_FUNCTION__FADE_SPOTS__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     * Sparkle/Twinkle
     **/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__SOLID_GLITTER__ID,
    EFFECTS_FUNCTION__POPCORN__ID,
    EFFECTS_FUNCTION__PLASMA__ID,
    EFFECTS_FUNCTION__SPARKLE__ID,
    EFFECTS_FUNCTION__FLASH_SPARKLE__ID,
    EFFECTS_FUNCTION__HYPER_SPARKLE__ID,
    EFFECTS_FUNCTION__TWINKLE__ID,
    EFFECTS_FUNCTION__COLORTWINKLE__ID,
    EFFECTS_FUNCTION__TWINKLE_FOX__ID,
    EFFECTS_FUNCTION__TWINKLE_CAT__ID,
    EFFECTS_FUNCTION__TWINKLE_UP__ID,
    EFFECTS_FUNCTION__SAW__ID,
    EFFECTS_FUNCTION__DISSOLVE__ID,
    EFFECTS_FUNCTION__DISSOLVE_RANDOM__ID,
    EFFECTS_FUNCTION__COLORFUL__ID,
    EFFECTS_FUNCTION__TRAFFIC_LIGHT__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE            
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     * Blink/Strobe
     **/
    EFFECTS_FUNCTION__BLINK__ID,
    EFFECTS_FUNCTION__BLINK_RAINBOW__ID,
    EFFECTS_FUNCTION__STROBE__ID,
    EFFECTS_FUNCTION__MULTI_STROBE__ID,
    EFFECTS_FUNCTION__STROBE_RAINBOW__ID, 
    EFFECTS_FUNCTION__RAINBOW__ID,
    EFFECTS_FUNCTION__LIGHTNING__ID,     
    EFFECTS_FUNCTION__FIRE_2012__ID,
    EFFECTS_FUNCTION__RAILWAY__ID,
    EFFECTS_FUNCTION__HEARTBEAT__ID, 
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     * Noise
     **/
    EFFECTS_FUNCTION__FILLNOISE8__ID,
    EFFECTS_FUNCTION__NOISE16_1__ID,
    EFFECTS_FUNCTION__NOISE16_2__ID,
    EFFECTS_FUNCTION__NOISE16_3__ID,
    EFFECTS_FUNCTION__NOISE16_4__ID,
    EFFECTS_FUNCTION__NOISEPAL__ID,
    EFFECTS_FUNCTION__PHASEDNOISE__ID,
    EFFECTS_FUNCTION__PHASED__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    /**
     * Scan
     **/
    EFFECTS_FUNCTION__SCAN__ID,
    EFFECTS_FUNCTION__DUAL_SCAN__ID,
    EFFECTS_FUNCTION__LARSON_SCANNER__ID,
    EFFECTS_FUNCTION__DUAL_LARSON_SCANNER__ID,
    EFFECTS_FUNCTION__ICU__ID,
    EFFECTS_FUNCTION__RIPPLE__ID,
    EFFECTS_FUNCTION__RIPPLE_RAINBOW__ID,
    EFFECTS_FUNCTION__COMET__ID,
    EFFECTS_FUNCTION__CHUNCHUN__ID,
    EFFECTS_FUNCTION__BOUNCINGBALLS__ID,
    EFFECTS_FUNCTION__SINELON__ID,
    EFFECTS_FUNCTION__SINELON_DUAL__ID,
    EFFECTS_FUNCTION__SINELON_RAINBOW__ID,
    EFFECTS_FUNCTION__DRIP__ID,   
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE  
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
    EFFECTS_FUNCTION__HARDWARE__SHOW_BUS__ID,
    EFFECTS_FUNCTION__HARDWARE__MANUAL_PIXEL_COUNTING__ID,
    EFFECTS_FUNCTION__HARDWARE__VIEW_PIXEL_RANGE__ID,
    EFFECTS_FUNCTION__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Sun positions to control animation***************************************************************************************************************************************************************************
    **  Requires:    Internet to know time and location for sun calculations **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/


    // These should actually be made into palettes, so they can be used with any effect


    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    /**
     * @brief  linear palette group colour, changing by triggered and over a period of time (eg an alarm -30 minutes)
     **/
    EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01__ID,
    /**
     * @brief  Azimuth min/max selects 0-255 for index to get from palette (so map(az, az_min, az_max, 0, 255) which is 0-colours_in_palette)
     **/
    EFFECTS_FUNCTION__SUNPOSITIONS_AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__ID,
    /**
     * @brief Daytime: Palette 1
     *       Nightime: Palette 2
     *        Option1: 0 means it considers the full daily movement as transition, otherwise it means 0.1-degree of movement is considered transition around horizon (ie. +- 5.5 degrees would be 55)
     **/
    EFFECTS_FUNCTION__SUNPOSITIONS_SUNSET_BLENDED_PALETTES_01__ID,
    /**
     * @brief  Draw Sun will either draw single pixel, or bloom gradient (ie 1 pixel to 5 pixels blended) on a 1D LED strip
     * Option1: for background to be drawn (ie. sky)
     * Option2: background sky colour may be time of day reactive and change colour (ie. blue during day, black at night)
     **/
    EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_01__ID,
    EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_01__ID,    
    EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__ID, // 2D LED matrix, elevation and azimuth
    /**
     * @brief  As the traditional solid white light, but reactive to the time of day (lightness outside)
     *        I will also want a way to pick what these max/min are (ie setting dusk/dawn as limits for transition for CCT colours etc)
     **/
    EFFECTS_FUNCTION__SUNPOSITIONS_WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__ID, // Daywhite in daylight, and warm white at night
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
    EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
    EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,
    EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Border Wallpapers i.e. Edge colours for displays or pictures  ***************************************************************************************************************************************************************************
    **  Requires:     **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
    EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__ID,
    EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__ID,
    EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS


    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Border Wallpapers i.e. Edge colours for displays or pictures  ***************************************************************************************************************************************************************************
    **  Requires:     **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
    EFFECTS_FUNCTION__MANUAL__PIXEL_SET_ELSEWHERE__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE


    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE_TODO
    EFFECTS_FUNCTION__1D__AURORA__ID,
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
    EFFECTS_FUNCTION__2D__DISTORTION_WAVES__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    EFFECTS_FUNCTION__2D__SOAP__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    EFFECTS_FUNCTION__2D__OCTOPUS__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    EFFECTS_FUNCTION__2D__WAVING_CELL__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    EFFECTS_FUNCTION__2D__SCROLLING_TEXT__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    EFFECTS_FUNCTION__2D__DNA__ID,
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
    EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_WATERFALL__ID,
    #endif
    /****************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    *** Specialised: 2D (Audio Reactive) ************************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    EFFECTS_FUNCTION__AUDIOREACTIVE__2D__SWIRL__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    EFFECTS_FUNCTION__AUDIOREACTIVE__2D__WAVERLY__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_GED__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_AKEMI__ID,
    #endif

    
    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Border Wallpapers i.e. Edge colours for displays or pictures  ***************************************************************************************************************************************************************************
    **  Requires:     **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/



    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    /**
     * Palette developing
     * */
    /**
     * Designing and quick test of animations before creating its own animaiton profile
     * */
    EFFECTS_FUNCTION__TESTER_01__ID,
    EFFECTS_FUNCTION__TESTER_02__ID,
    EFFECTS_FUNCTION__STATIC_PALETTE_NEW__ID,
    EFFECTS_FUNCTION__SLOW_GLOW_NEW__ID,
    /**
     * @brief Testing can animation_colours be moved into the dynamic data array
     * 
     */
    // EFFECTS_FUNCTION__STATIC_PALETTE_DYNAMIC_METHOD__ID,
    // EFFECTS_FUNCTION__SLOW_GLOW_DYNAMIC_METHOD__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING

    // Length
    EFFECTS_FUNCTION__LENGTH__ID
  };         


  // NEw effect idea, lava lamp, both 1D and matrix. Have lgihting "gravity" then also "heating" for uplifting of random colours.

  
  #define WLED_GROUP_IDS_FIRST  EFFECTS_FUNCTION__STATIC_PATTERN__ID
  #define WLED_GROUP_IDS_LAST   EFFECTS_FUNCTION__DRIP__ID


  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  #define DEFAULT_EFFECTS_FUNCTION    EFFECTS_FUNCTION__STATIC_PALETTE__ID
  #else
  #define DEFAULT_EFFECTS_FUNCTION    0
  #endif

  uint16_t getEffectsAmount(){ return effects.config.size(); }

  void SubTask_Segments_Effects();
  void Segments_RefreshLEDIndexPattern(uint8_t segment_index = 0);
  
  byte realtimeMode = REALTIME_MODE_INACTIVE;

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      uint16_t fForceUpdate : 1; 
      uint16_t fRunning : 1;
      uint16_t animator_first_run : 1;
      // Reserved
      uint16_t reserved : 12;
    };
  } ANIMATION_FLAGS;



  struct TransitionColourPairs
  {
    RgbcctColor StartingColour;
    RgbcctColor DesiredColour;
  };
  enum ColourType{ 
      // COLOUR_TYPE__NONE__ID=0, 
      COLOUR_TYPE__SINGLE__ID, // likely never used for me, remove
      COLOUR_TYPE__COLDWARM__ID,  //CCT Only
      COLOUR_TYPE__RGB__ID,   
      COLOUR_TYPE__RGBW__ID, 
      COLOUR_TYPE__RGBCCT__ID, // CW/WW 
      
      // Previous methods that remember colour order, probably not needed or at least cct assume default of RGBWC
      COLOUR_TYPE__RGBWC__ID, 
      COLOUR_TYPE__RGBCW__ID
  };

  // TransitionColourPairs* 
  void GetTransitionColourBuffer(byte* allocated_buffer, uint16_t buflen, uint16_t pixel_index, ColourType pixel_type, mAnimatorLight::TransitionColourPairs* pair_test);
  bool SetTransitionColourBuffer(byte* allocated_buffer, uint16_t buflen, uint16_t pixel_index, ColourType pixel_type, RgbcctColor starting_colour, RgbcctColor desired_colour);

  uint8_t GetSizeOfPixel(ColourType colour_type);

  /**
   * @brief New method that allows getting WLED basic 3 colours, but also applying the colour when getting, the same as my palette method
   * 
   */
  RgbcctColor GetSegmentColour(uint8_t colour_index, uint8_t segment_index = 0);


  mAnimatorLight& SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE);
  void StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index = 0);
  void Segments_SetPixelColor_To_Static_Pallete(uint16_t palette_id);
  void Segments_UpdateDesiredColourFromPaletteSelected(uint16_t segment_index = 0);
  void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param);
  void AnimationProcess_LinearBlend_Dynamic_Buffer(const AnimationParam& param);
  void AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(const AnimationParam& param);
  void AnimationProcess_SingleColour_LinearBlend_Between_RgbcctSegColours(const AnimationParam& param);

  void SetTransitionColourBuffer_StartingColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, ColourType pixel_type, RgbcctColor starting_colour);
  void SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index,  ColourType pixel_type, RgbcctColor starting_colour);

  void DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  void DynamicBuffer_Segments_UpdateStartingColourWithGetPixel_WithFade(uint8_t fade = 0);
  void DynamicBuffer_Segments_UpdateStartingColourWithGetPixel_FromBus();
  void Segments_Dynamic_Buffer_UpdateStartingColourWithGetPixel();


  #ifdef ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
  //um_manager.cpp
  typedef enum UM_Data_Types {
    UMT_BYTE = 0,
    UMT_UINT16,
    UMT_INT16,
    UMT_UINT32,
    UMT_INT32,
    UMT_FLOAT,
    UMT_DOUBLE,
    UMT_BYTE_ARR,
    UMT_UINT16_ARR,
    UMT_INT16_ARR,
    UMT_UINT32_ARR,
    UMT_INT32_ARR,
    UMT_FLOAT_ARR,
    UMT_DOUBLE_ARR
  } um_types_t;
  typedef struct UM_Exchange_Data {
    // should just use: size_t arr_size, void **arr_ptr, byte *ptr_type
    size_t       u_size;                 // size of u_data array
    um_types_t  *u_type;                 // array of data types
    void       **u_data;                 // array of pointers to data
    UM_Exchange_Data() {
      u_size = 0;
      u_type = nullptr;
      u_data = nullptr;
    }
    ~UM_Exchange_Data() {
      if (u_type) delete[] u_type;
      if (u_data) delete[] u_data;
    }
  } um_data_t;
  const unsigned int um_data_size = sizeof(um_data_t);  // 12 bytes
  um_data_t* simulateSound(uint8_t simulationId);
      
  //Usermod IDs
  #define USERMOD_ID_RESERVED               0     //Unused. Might indicate no usermod present
  #define USERMOD_ID_UNSPECIFIED            1     //Default value for a general user mod that does not specify a custom ID
  #define USERMOD_ID_EXAMPLE                2     //Usermod "usermod_v2_example.h"
  #define USERMOD_ID_TEMPERATURE            3     //Usermod "usermod_temperature.h"
  #define USERMOD_ID_FIXNETSERVICES         4     //Usermod "usermod_Fix_unreachable_netservices.h"
  #define USERMOD_ID_PIRSWITCH              5     //Usermod "usermod_PIR_sensor_switch.h"
  #define USERMOD_ID_IMU                    6     //Usermod "usermod_mpu6050_imu.h"
  #define USERMOD_ID_FOUR_LINE_DISP         7     //Usermod "usermod_v2_four_line_display.h
  #define USERMOD_ID_ROTARY_ENC_UI          8     //Usermod "usermod_v2_rotary_encoder_ui.h"
  #define USERMOD_ID_AUTO_SAVE              9     //Usermod "usermod_v2_auto_save.h"
  #define USERMOD_ID_DHT                   10     //Usermod "usermod_dht.h"
  #define USERMOD_ID_MODE_SORT             11     //Usermod "usermod_v2_mode_sort.h"
  #define USERMOD_ID_VL53L0X               12     //Usermod "usermod_vl53l0x_gestures.h"
  #define USERMOD_ID_MULTI_RELAY           13     //Usermod "usermod_multi_relay.h"
  #define USERMOD_ID_ANIMATED_STAIRCASE    14     //Usermod "Animated_Staircase.h"
  #define USERMOD_ID_RTC                   15     //Usermod "usermod_rtc.h"
  #define USERMOD_ID_ELEKSTUBE_IPS         16     //Usermod "usermod_elekstube_ips.h"
  #define USERMOD_ID_SN_PHOTORESISTOR      17     //Usermod "usermod_sn_photoresistor.h"
  #define USERMOD_ID_BATTERY               18     //Usermod "usermod_v2_battery.h"
  #define USERMOD_ID_PWM_FAN               19     //Usermod "usermod_PWM_fan.h"
  #define USERMOD_ID_BH1750                20     //Usermod "usermod_bh1750.h"
  #define USERMOD_ID_SEVEN_SEGMENT_DISPLAY 21     //Usermod "usermod_v2_seven_segment_display.h"
  #define USERMOD_RGB_ROTARY_ENCODER       22     //Usermod "rgb-rotary-encoder.h"
  #define USERMOD_ID_QUINLED_AN_PENTA      23     //Usermod "quinled-an-penta.h"
  #define USERMOD_ID_SSDR                  24     //Usermod "usermod_v2_seven_segment_display_reloaded.h"
  #define USERMOD_ID_CRONIXIE              25     //Usermod "usermod_cronixie.h"
  #define USERMOD_ID_WIZLIGHTS             26     //Usermod "wizlights.h"
  #define USERMOD_ID_WORDCLOCK             27     //Usermod "usermod_v2_word_clock.h"
  #define USERMOD_ID_MY9291                28     //Usermod "usermod_MY9291.h"
  #define USERMOD_ID_SI7021_MQTT_HA        29     //Usermod "usermod_si7021_mqtt_ha.h"
  #define USERMOD_ID_BME280                30     //Usermod "usermod_bme280.h
  #define USERMOD_ID_SMARTNEST             31     //Usermod "usermod_smartnest.h"
  #define USERMOD_ID_AUDIOREACTIVE         32     //Usermod "audioreactive.h"
  #define USERMOD_ID_ANALOG_CLOCK          33     //Usermod "Analog_Clock.h"
  #define USERMOD_ID_PING_PONG_CLOCK       34     //Usermod "usermod_v2_ping_pong_clock.h"
  #define USERMOD_ID_ADS1115               35     //Usermod "usermod_ads1115.h"
  #define USERMOD_ID_BOBLIGHT              36     //Usermod "boblight.h"
  #define USERMOD_ID_SD_CARD               37     //Usermod "usermod_sd_card.h"
  #define USERMOD_ID_PWM_OUTPUTS           38     //Usermod "usermod_pwm_outputs.h
  #define USERMOD_ID_SHT                   39     //Usermod "usermod_sht.h
  #define USERMOD_ID_KLIPPER               40     //Usermod Klipper percentage
  #define USERMOD_ID_WIREGUARD             41     //Usermod "wireguard.h"
  #define USERMOD_ID_INTERNAL_TEMPERATURE  42     //Usermod "usermod_internal_temperature.h"
  #define USERMOD_ID_LDR_DUSK_DAWN         43     //Usermod "usermod_LDR_Dusk_Dawn_v2.h"
  #define USERMOD_ID_STAIRWAY_WIPE         44     //Usermod "stairway-wipe-usermod-v2.h"

  class Usermod {
    protected:
      um_data_t *um_data; // um_data should be allocated using new in (derived) Usermod's setup() or constructor
    public:
      Usermod() { um_data = nullptr; }
      virtual ~Usermod() { if (um_data) delete um_data; }
      virtual void setup() = 0; // pure virtual, has to be overriden
      virtual void loop() = 0;  // pure virtual, has to be overriden
      virtual void handleOverlayDraw() {}                                      // called after all effects have been processed, just before strip.show()
      virtual bool handleButton(uint8_t b) { return false; }                   // button overrides are possible here
      virtual bool getUMData(um_data_t **data) { if (data) *data = nullptr; return false; }; // usermod data exchange [see examples for audio effects]
      virtual void connected() {}                                              // called when WiFi is (re)connected
      virtual void appendConfigData() {}                                       // helper function called from usermod settings page to add metadata for entry fields
      virtual void addToJsonState(JsonObject& obj) {}                          // add JSON objects for WLED state
      virtual void addToJsonInfo(JsonObject& obj) {}                           // add JSON objects for UI Info page
      virtual void readFromJsonState(JsonObject& obj) {}                       // process JSON messages received from web server
      virtual void addToConfig(JsonObject& obj) {}                             // add JSON entries that go to cfg.json
      virtual bool readFromConfig(JsonObject& obj) { return true; } // Note as of 2021-06 readFromConfig() now needs to return a bool, see usermod_v2_example.h
      virtual void onMqttConnect(bool sessionPresent) {}                       // fired when MQTT connection is established (so usermod can subscribe)
      virtual bool onMqttMessage(char* topic, char* payload) { return false; } // fired upon MQTT message received (wled topic)
      virtual void onUpdateBegin(bool) {}                                      // fired prior to and after unsuccessful firmware update
      virtual void onStateChange(uint8_t mode) {}                              // fired upon WLED state change
      virtual uint16_t getId() {return USERMOD_ID_UNSPECIFIED;}
  };

  #define WLED_MAX_USERMODS 4

  class UsermodManager {
    private:
      Usermod* ums[WLED_MAX_USERMODS];
      byte numMods = 0;

    public:
      void loop();
      void handleOverlayDraw();
      bool handleButton(uint8_t b);
      bool getUMData(um_data_t **um_data, uint8_t mod_id = USERMOD_ID_RESERVED); // USERMOD_ID_RESERVED will poll all usermods
      void setup();
      void connected();
      void appendConfigData();
      void addToJsonState(JsonObject& obj);
      void addToJsonInfo(JsonObject& obj);
      void readFromJsonState(JsonObject& obj);
      void addToConfig(JsonObject& obj);
      bool readFromConfig(JsonObject& obj);
      void onMqttConnect(bool sessionPresent);
      bool onMqttMessage(char* topic, char* payload);
      void onUpdateBegin(bool);
      void onStateChange(uint8_t);
      bool add(Usermod* um);
      Usermod* lookup(uint16_t mod_id);
      byte getModCount() {return numMods;};
  };

  UsermodManager usermods = UsermodManager();

  #endif // ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD



    /*****************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Subtask:   SEQUENCER   *****************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER // Legacy mixer/playlist to enable hardcoded playlists while the file system playlists are underdevelopment

    void Init_Sequencer();
    void handleSequencer();
    void Load_Sequencer(uint8_t id);
    void SubLoad_Sequencer_Device(uint8_t id);
    void SetSequenceTimes(uint16_t secs);

    enum FLASH_LEVEL{
      Static=0,
      Gentle=1,
      Flashing=2,
      FastFlashing=3
    }FlashLevel;

    struct SEQUENCER_ITEM
    {
      uint16_t seconds_on = 10;
      struct TIMES {
        struct time_short start = {0};
        struct time_short end = {0};
        bool isArmed = false;
      }time_enabled;
      struct DISABLE{
        uint8_t flash_level = 0; // let use 3, 0 is static, 1 is gentle, 2 is flashing, 3 is fast flashing

      }limit;
      String json_command;
      String description;
    }sequencer_item;
    std::vector<struct SEQUENCER_ITEM> sequencer_item_list;

    struct SEQUENCER{
      uint16_t seconds_remaining_on_item = 0;
      uint8_t active_sequence_index = 0;
      uint32_t tSaved_Tick = 0;
      uint8_t loaded_sequence_set = 1; // 0 means disabled 
      bool Enable_TimeRestraints = false;
      uint8_t remote_openhab_limit_flashing = 1; // default is no block. Openhab will send enable at 9am, and disable at 8pm
    }sequencer_runtime;

    #endif // ENABLE_FEATURE_LIGHTING__SEQUENCER


    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Ambilight   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    // Move completely into its own class, with its own tasker
    /**************
     * Ambilight is light patterns around screens or pictures
     * PRESETS - patterns
     * INPUT_STREAM - set leds directly through mqtt (wallpaper) or serial lighting
    **************/ 
    enum AMBILIGHT_MODES_IDS{
      AMBILIGHT_PRESETS_ID=0,
      AMBILIGHT_SIDES_ID,
      AMBILIGHT_INPUT_STREAM_ID,
      AMBILIGHT_LENGTH_ID
    };
    int8_t GetAmbilightModeIDbyName(const char* c);
    const char* GetAmbilightModeName(char* buffer);

    /*******AMBILIGHT*********************************************************************************************/

    void SubTask_Ambilight_Main();
    void Ambilight_Sides();
    void Ambilight_Presets();
    void Ambilight_InputStream();
    void init_Ambilight();
    #define AMBILIGHT_SCREENS_CONNECTED 1

    /**
     * 
     * Long term, multiple screens will be used as their own subclass, containing all the info about that screen instead of struct arrays
     * */
    enum AMBILIGHT_SCREEN_INDEX{SCREEN_CENTRE=0,SCREEN_LEFT=1,SCREEN_RIGHT=2};

    struct AMBILIGHT_SCREEN_SETTINGS{
      uint8_t ambilight_mode = AMBILIGHT_SIDES_ID;
      uint32_t tSavedUpdate = millis();
      uint32_t ratemsSavedUpdate = 1000;
      uint8_t fUpdate = false;
      struct SCREENS{
        struct EDGE{
          RgbcctColor colour;
          int8_t blend_between_sides_gradient_percentage = -1; // -1 is unset/none/solid, 0% is bottom/left, 100% is top/right
          uint8_t size = 5;
        };
        struct EDGE top;
        struct EDGE bottom;
        struct EDGE left;
        struct EDGE right;
        struct FIRST_PIXEL{
          //uint8_t bottom_right_clockwise = store as packed bit? 
        }start_pixel;
      }screens[AMBILIGHT_SCREENS_CONNECTED]; //0,1,2 where 0 is centre and only screen

    }ambilightsettings;

    #endif

    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Palette related   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    GetColourFromUnloadedPalette2(
      uint16_t palette_id,
      uint16_t desired_index_from_palette = 0,
      bool     flag_spanned_segment = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_crgb_exact_colour = false,
      uint8_t* encoded_index = nullptr
    );

    CRGB ColorFromPalette_WithLoad(const CRGBPalette16 &pal, uint8_t index, uint8_t brightness = (uint8_t)255U, TBlendType blendType = LINEARBLEND);
    

    const char* GetPaletteNameByID(uint16_t palette_id, char* buffer = nullptr, uint8_t buflen = 0);
    int16_t GetPaletteIDbyName(char* buffer);
    
    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Helper functions   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/


    // Temporary helper functions to be cleaned up and converted
    uint32_t crgb_to_col(CRGB fastled);
    CRGB col_to_crgb(uint32_t);
    uint8_t get_random_wheel_index(uint8_t pos);
    uint16_t triwave16(uint16_t in);
    uint16_t mode_palette();
    void colorFromUint32(uint32_t in, bool secondary = false);
    void colorFromUint24(uint32_t in, bool secondary = false);
    void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
    void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
    void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb
    void colorFromDecOrHexString(byte* rgb, char* in);
    void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)



    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** To sort   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
    **********************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/




  void Segment_SubTask_Flasher_Animate_Function__TEST_SolidRandom();
  void Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index = 0); 
  


  void setValuesFromMainSeg();
  void resetTimebase();
  void setAllLeds();
  void setLedsStandard(bool justColors = false);
  bool colorChanged();
  void colorUpdated(int callMode);
  void handleTransitions();

  
  float minf2(float v, float w);
  float maxf2(float v, float w);

  void Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index = 0);

  void SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i = 0);

  int16_t extractModeDefaults(uint8_t mode, const char *segVar);

    void Reset_CustomPalette_NamesDefault();

  /**
   * @brief 
   *
   * 
  * Segment, 72 bytes
  **/

  #include "6_Lights/03_Animator/SubHeader_Segments.h" // Include the segment header file section, doing this to keep it easier to see




  RgbcctColor ColourBlend(RgbcctColor color1, RgbcctColor color2, uint8_t blend);

  typedef void (*mode_ptr)(void); // pointer to mode function


  typedef void (*show_callback)(void); // pre show callback
  typedef struct ModeData {
    uint8_t     _id;   // mode (effect) id
    mode_ptr    _fcn;  // mode (effect) function
    const char *_data; // mode (effect) name and its UI control data
    ModeData(uint8_t id, void (*fcn)(void), const char *data) : _id(id), _fcn(fcn), _data(data) {}
  } mode_data_t;

  void finalizeInit();
  void service(void);
  void setMode(uint8_t segid, uint8_t m);
  void setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0);
  void setColor(uint8_t slot, uint32_t c);
  void setCCT(uint16_t k);
  void setBrightness(uint8_t b, bool direct = false);
  void setRange(uint16_t i, uint16_t i2, uint32_t col);
  void setTransitionMode(bool t);
  void purgeSegments(bool force = false);
  void setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t grouping = 1, uint8_t spacing = 0, uint16_t offset = UINT16_MAX, uint16_t startY=0, uint16_t stopY=1);
  void setMainSegmentId(uint8_t n);
  void restartRuntime();
  void resetSegments2();
  void makeAutoSegments(bool forceReset = false);
  void fixInvalidSegments();
  void setPixelColor(int n, uint32_t c);
  void show(void);
  void setTargetFps(uint8_t fps);

  // using public variables to reduce code size increase due to inline function getSegment() (with bounds checking) and color transitions
  uint32_t _colors_t_PHASE_OUT[3]; // color used for effect (includes transition)
  uint16_t _virtualSegmentLength;

  std::vector<segment_new> segments;
  friend class Segment;

  uint16_t _length;
  uint8_t  _brightness;

  uint8_t  _targetFps;
  uint16_t _frametime;
  uint16_t _cumulativeFps;

  // will require only 1 byte
  struct {
    bool _isServicing          : 1;
    bool _isOffRefreshRequired : 1; //periodic refresh is required for the strip to remain off.
    bool _hasWhiteChannel      : 1;
    bool _triggered            : 1;
  };

  void LoadEffects();

  show_callback _callback;

  uint16_t* customMappingTable;
  uint16_t  customMappingSize;
  
  uint32_t _lastShow;
  
  uint8_t _segment_index_primary;
  uint8_t _mainSegment;

  void fill2(uint32_t c) { for (int i = 0; i < _length; i++) setPixelColor(i, c); } // fill whole strip with color (inline)


  enum Effect_DevStage
  {
    Release=0,
    Beta=1,
    Alpha=2,
    Dev=3,
    Unstable=4
  };


    typedef void (mAnimatorLight::*RequiredFunction)();        
    void addEffect(uint8_t id, RequiredFunction function, const char* config = nullptr, uint8_t development_stage = Effect_DevStage::Dev); // add effect to the list; defined in FX.cpp

    struct EFFECTS
    {
      uint8_t                         count = 0;
      std::vector<RequiredFunction>   function;     // SRAM footprint: 4 bytes per element
      std::vector<const char*>        config;     // 
      std::vector<uint8_t>            development_stage; // 0:stable, 1:beta, 2:alpha, 3:dev
    }effects;


    void setupEffectData(void); // add default effects to the list; defined in FX.cpp

    // outsmart the compiler :) by correctly overloading
    // inline void setPixelColor(int n, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) { 
    //   Serial.println(__LINE__);
    //   setPixelColor(n, RGBW32(r,g,b,w)); }
    // inline void setPixelColor(int n, CRGB c) { 
      
    //   Serial.println(__LINE__);
    //   setPixelColor(n, c.red, c.green, c.blue); }

    // inline void setPixelColorXY(int x, int y, uint32_t c)   { setPixelColor(y * Segment::maxWidth + x, c); }
    // inline void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColorXY(x, y, RGBW32(r,g,b,w)); }
    // inline void setPixelColorXY(int x, int y, CRGB c)       { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }

    // inline void setPixelColorXY(int x, int y, uint32_t c)   { 
    //   Serial.println(__LINE__);setPixelColor(y * Segment::maxWidth + x, c); }
    // inline void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) {
    //   Serial.println(__LINE__); setPixelColorXY(x, y, RGBW32(r,g,b,w)); }
    // inline void setPixelColorXY(int x, int y, CRGB c)       {
    //   Serial.println(__LINE__); setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }


    inline void trigger(void) { _triggered = true; } // Forces the next frame to be computed on all active segments.
    inline void setShowCallback(show_callback cb) { _callback = cb; }
    inline void appendSegment(const Segment &seg = Segment()) {

      Serial.println("DEBUG_LINE_HERE;");
       segments.push_back(seg); 
       
       }

    bool
      checkSegmentAlignment(void),
      hasRGBWBus(void),
      hasCCTBus(void),
      // return true if the strip is being sent pixel updates
      isUpdating(void),
      useLedsArray = false;
    
    bool deserializeMap(uint8_t n=0);

    inline bool isServicing(void) { return _isServicing; }
    inline bool hasWhiteChannel(void) {return _hasWhiteChannel;}
    inline bool isOffRefreshRequired(void) {return _isOffRefreshRequired;}

    uint8_t
      paletteFade,
      paletteBlend,
      milliampsPerLed,
      cctBlending,
      getActiveSegmentsNum(void),
      getFirstSelectedSegId(void),
      getLastActiveSegmentId(void),
      setPixelSegment(uint8_t n);

    inline uint8_t getBrightness(void) { return _brightness; }
    inline uint8_t getMaxSegments(void) { return MAX_NUM_SEGMENTS; }  // returns maximum number of supported segments (fixed value)
    inline uint8_t getSegmentsNum(void) { return segments.size(); }  // returns currently present segments
    inline uint8_t getCurrSegmentId(void) { return _segment_index_primary; }
    inline uint8_t getMainSegmentId(void) { return _mainSegment; }
    inline uint8_t getPaletteCount() { return 13 + GRADIENT_PALETTE_COUNT; }  // will only return built-in palette count
    inline uint8_t getTargetFps() { return _targetFps; }
    inline uint8_t getModeCount() { return effects.count; }

    uint16_t
      ablMilliampsMax,
      currentMilliamps,
      getLengthPhysical(void),
      getFps();

    inline uint16_t getFrameTime(void) { return _frametime; }
    inline uint16_t getMinShowDelay(void) { return MIN_SHOW_DELAY; }
    inline uint16_t getLengthTotal(void) { return _length; }

    uint32_t _now;
    uint32_t timebase;
    uint32_t currentColor(uint32_t colorNew, uint8_t tNr);
    uint32_t getPixelColor(uint16_t);

    inline uint32_t getLastShow(void) { return _lastShow; }
    inline uint32_t segColor(uint8_t i) { return _colors_t_PHASE_OUT[i]; }

    const char* getModeData(uint8_t id = 0) { return (id && id<effects.count) ? effects.config[id] : PSTR("Solid"); }
    const char* getModeData_Config(uint8_t id = 0) { return (id<effects.count) ? effects.config[id] : PSTR("Unknown"); }

    const char** getModeDataSrc(void) { return &(effects.config[0]); } // vectors use arrays for underlying data

    Segment&        getSegment(uint8_t id);
    inline Segment& getFirstSelectedSeg(void) { return segments[getFirstSelectedSegId()]; }
    inline Segment& getMainSegment(void)      { return segments[getMainSegmentId()]; }
    inline Segment* getSegments(void)         { return &(segments[0]); }

  /******************************************************************************************************************
  * * Matrix : Global 2D settings ****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  **/
 
  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    bool isMatrix = true;
  #else
    bool isMatrix = false;
  #endif

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    #define WLED_MAX_PANELS 64
    uint8_t  panels;

    typedef struct panel_t {
      uint16_t xOffset; // x offset relative to the top left of matrix in LEDs
      uint16_t yOffset; // y offset relative to the top left of matrix in LEDs
      uint8_t  width;   // width of the panel
      uint8_t  height;  // height of the panel
      union {
        uint8_t options;
        struct {
          bool bottomStart : 1; // starts at bottom?
          bool rightStart  : 1; // starts on right?
          bool vertical    : 1; // is vertical?
          bool serpentine  : 1; // is serpentine?
        };
      };
      panel_t()
        : xOffset(0)
        , yOffset(0)
        , width(8)
        , height(8)
        , options(0)
      {}
    } Panel;
    std::vector<Panel> panel;

    void setUpMatrix();

    // outsmart the compiler :) by correctly overloading
    inline void setPixelColorXY(int x, int y, uint32_t c)   { setPixelColor(y * Segment::maxWidth + x, c); }
      inline void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColorXY(x, y, RGBW32(r,g,b,w)); }
      inline void setPixelColorXY(int x, int y, CRGB c)       { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }

    inline uint32_t getPixelColorXY(uint16_t x, uint16_t y) { return getPixelColor(isMatrix ? y * Segment::maxWidth + x : x);}

  #endif


  /******************************************************************************************************************
  * * ?? ****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  **/






























    void loadCustomPalettes(void);
    std::vector<CRGBPalette16> customPalettes; // TODO: move custom palettes out of WS2812FX class

    void estimateCurrentAndLimitBri(void);

    Toki toki = Toki();
    WiFiUDP notifierUdp, rgbUdp, notifier2Udp;
    bool e131NewData = false;
    byte currentPreset = 0;

    class NeoGammaWLEDMethod {
      public:
        static uint8_t Correct(uint8_t value);      // apply Gamma to single channel
        static uint32_t Correct32(uint32_t color);  // apply Gamma to RGBW32 color (WLED specific, not used by NPB)
        static void calcGammaTable(float gamma);    // re-calculates & fills gamma table
        static inline uint8_t rawGamma8(uint8_t val) { return gammaT[val]; }  // get value from Gamma table (WLED specific, not used by NPB)
      private:
        static uint8_t gammaT[];
    };
    #define gamma32(c) NeoGammaWLEDMethod::Correct32(c)
    #define gamma8(c)  NeoGammaWLEDMethod::rawGamma8(c)

    #ifndef WLED_USE_REAL_MATH
      // template <typename T> T atan_t(T x);
      static float cos_t(float phi);
      static float sin_t(float x);
      static float tan_t(float x);
      static float acos_t(float x);
      static float asin_t(float x);
      static float floor_t(float x);
      static float fmod_t(float num, float denom);
    #else
      #include <math.h>
      #define sin_t sin
      #define cos_t cos
      #define tan_t tan
      #define asin_t asin
      #define acos_t acos
      #define atan_t atan
      #define fmod_t fmod
      #define floor_t floor
    #endif

    #include <map>
    #include <IPAddress.h>

    #define NODE_TYPE_ID_UNDEFINED        0
    #define NODE_TYPE_ID_ESP8266         82
    #define NODE_TYPE_ID_ESP32           32
    #define NODE_TYPE_ID_ESP32S2         33
    #define NODE_TYPE_ID_ESP32S3         34
    #define NODE_TYPE_ID_ESP32C3         35

    /*********************************************************************************************\
    * NodeStruct
    \*********************************************************************************************/
    struct NodeStruct
    {
      String    nodeName;
      IPAddress ip;
      uint8_t   age;
      uint8_t   nodeType;
      uint32_t  build;

      NodeStruct() : age(0), nodeType(0), build(0)
      {
        for (uint8_t i = 0; i < 4; ++i) { ip[i] = 0; }
      }
    };
    typedef std::map<uint8_t, NodeStruct> NodesMap;


    #define ARDUINOJSON_DECODE_UNICODE 0
    #include "3_Network/21_WebServer/AsyncJson-v6.h"
    #include "3_Network/21_WebServer/ArduinoJson-v6.h"

void serializeNetworks(JsonObject root);
    
void toggleOnOff();
byte scaledBri(byte in);

//udp.cpp
void notify(byte callMode, bool followUp=false);
uint8_t realtimeBroadcast(uint8_t type, IPAddress client, uint16_t length, uint8_t *buffer, uint8_t bri=255, bool isRGBW=false);
void realtimeLock(uint32_t timeoutMs, byte md = REALTIME_MODE_GENERIC);
void exitRealtime();
void handleNotifications();
void setRealtimePixel(uint16_t i, byte r, byte g, byte b, byte w);
void refreshNodeList();
void sendSysInfoUDP();


bool deserializeConfig(JsonObject doc, bool fromFS = false);


void getStringFromJson(char* dest, const char* src, size_t len);
// Temp buffer
char* obuf;
uint16_t olen = 0;

bool oappend(const char* txt);
bool oappendi(int i);
void sappends(char stype, const char* key, char* val);
void sappend(char stype, const char* key, int val);



#ifdef USE_MODULE_NETWORK_WEBSERVER
void serveSettingsJS(AsyncWebServerRequest* request);
void serveSettings(AsyncWebServerRequest* request, bool post = false);
bool handleIfNoneMatchCacheHeader(AsyncWebServerRequest* request);
void setStaticContentCacheHeaders(AsyncWebServerResponse *response);
void serveIndex(AsyncWebServerRequest* request);
void getSettingsJS(byte subPage, char* dest);
void WebPage_Root_AddHandlers();
#endif // USE_MODULE_NETWORK_WEBSERVER


void serializeSegment(JsonObject& root, mAnimatorLight::Segment& seg, byte id, bool forPreset = false, bool segmentBounds = true);
void serializeState(JsonObject root, bool forPreset = false, bool includeBri = true, bool segmentBounds = true, bool selectedSegmentsOnly = false);
void serializeInfo(JsonObject root);
void serializeModeData(JsonArray root);
void serializePalettes(JsonObject root, int page);



    
//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro



void serializeModeNames(JsonArray arr, bool flag_get_first_name_only = true);

bool requestJSONBufferLock(uint8_t module);
void releaseJSONBufferLock();

    #ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI
void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
bool serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient = 0);

void serveJson(AsyncWebServerRequest* request);

bool  captivePortal(AsyncWebServerRequest *request);
void  notFound(AsyncWebServerRequest *request);


#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI

void setPaletteColors(JsonArray json, CRGBPalette16 palette);

bool deserializeSegment(JsonObject elem, byte it, byte presetId = 0);

void parseNumber(const char* str, byte* val, byte minv=0, byte maxv=255);
bool getVal(JsonVariant elem, byte* val, byte minv=0, byte maxv=255);

bool colorFromHexString(byte* rgb, const char* in);
bool deserializeState(JsonObject root, byte callMode = CALL_MODE_DIRECT_CHANGE, byte presetId = 0);

bool isIp(String str);


#define JSON_PATH_STATE      1
#define JSON_PATH_INFO       2
#define JSON_PATH_STATE_INFO 3
#define JSON_PATH_NODES      4
#define JSON_PATH_PALETTES   5
#define JSON_PATH_FXDATA     6
#define JSON_PATH_NETWORKS   7
#define JSON_PATH_EFFECTS    8

// global ArduinoJson buffer
volatile uint8_t jsonBufferLock = 0;

bool doReboot = false;

//realtime modes
#define REALTIME_MODE_INACTIVE    0
#define REALTIME_MODE_GENERIC     1
#define REALTIME_MODE_UDP         2
#define REALTIME_MODE_HYPERION    3
#define REALTIME_MODE_E131        4
#define REALTIME_MODE_ADALIGHT    5
#define REALTIME_MODE_ARTNET      6
#define REALTIME_MODE_TPM2NET     7
#define REALTIME_MODE_DDP         8

#ifndef WLED_MAX_BUTTONS
  #ifdef ESP8266
    #define WLED_MAX_BUTTONS 2
  #else
    #define WLED_MAX_BUTTONS 4
  #endif
#endif

#ifndef WLED_MAX_SEGNAME_LEN
  #ifdef ESP8266
    #define WLED_MAX_SEGNAME_LEN 32
  #else
    #define WLED_MAX_SEGNAME_LEN 64
  #endif
#else
  #if WLED_MAX_SEGNAME_LEN<32
    #undef WLED_MAX_SEGNAME_LEN
    #define WLED_MAX_SEGNAME_LEN 32
  #else
    #warning WLED UI does not support modified maximum segment name length!
  #endif
#endif

#define WLED_GLOBAL

// #ifndef WLED_DEFINE_GLOBAL_VARS
// # define extern
// # define _INIT(x)
// # define _INIT_N(x)
// #else
# define WLED_GLOBAL
# define _INIT(x) = x

//needed to ignore commas in array definitions
#define UNPACK( ... ) __VA_ARGS__
#define _INIT_N(x) UNPACK x
// #endif

// User Interface CONFIG
// #ifndef SERVERNAME
char serverDescription[60];// = {"WLED"};  // Name of module - use default
// #else
// char serverDescription[33] _INIT(SERVERNAME);  // use predefined name
// #endif
bool syncToggleReceive     _INIT(false);   // UIs which only have a single button for sync should toggle send+receive if this is true, only send otherwise
bool simplifiedUI          _INIT(false);   // enable simplified UI
byte cacheInvalidate       _INIT(0);       // used to invalidate browser cache when switching from regular to simplified UI

#ifndef WLED_DISABLE_ESPNOW
bool enable_espnow_remote _INIT(false);
char linked_remote[13] = {0};//   _INIT("");
char last_signal_src[13] = {0};//   _INIT("");
#endif


char ntpServerName[33] = {0};//  _INIT("0.wled.pool.ntp.org");   // NTP server to use

// #define MDNS_NAME DEVICENAME_CTR ".local"
#define CLIENT_SSID "HACS2400"
#define CLIENT_PASS "af4d8bc9ab"
//Access point behavior
#define AP_BEHAVIOR_BOOT_NO_CONN          0     //Open AP when no connection after boot
#define AP_BEHAVIOR_NO_CONN               1     //Open when no connection (either after boot or if connection is lost)
#define AP_BEHAVIOR_ALWAYS                2     //Always open
#define AP_BEHAVIOR_BUTTON_ONLY           3     //Only when button pressed for 6 sec

// AP and OTA default passwords (for maximum security change them!)
char apPass[65]  = {0};//  _INIT(CLIENT_SSID);
char otaPass[33]  = {0};// _INIT("");

// WiFi CONFIG (all these can be changed via web UI, no need to set them here)
char clientSSID[33]   = {0};//_INIT(CLIENT_SSID);
char clientPass[65]  = {0};// _INIT(CLIENT_PASS);
// char cmDNS[] _INIT(MDNS_NAME);                       // mDNS address (*.local, replaced by wledXXXXXX if default is used)
char apSSID[33]  = {0};// _INIT("");                             // AP off by default (unless setup)
byte apChannel _INIT(1);                               // 2.4GHz WiFi AP channel (1-13)
byte apHide    _INIT(0);                               // hidden AP SSID
byte apBehavior _INIT(AP_BEHAVIOR_BOOT_NO_CONN);       // access point opens when no connection after boot by default
IPAddress staticIP   ;//   _INIT_N(((  0,   0,  0,  0))); // static IP of ESP
IPAddress staticGateway ;//_INIT_N(((  0,   0,  0,  0))); // gateway (router) IP
IPAddress staticSubnet;//  _INIT_N(((255, 255, 255, 0))); // most common subnet in home networks
#ifdef ARDUINO_ARCH_ESP32
bool noWifiSleep _INIT(true);                          // disabling modem sleep modes will increase heat output and power usage, but may help with connection issues
#else
bool noWifiSleep _INIT(false);
#endif

typedef enum mapping1D2D {
  M12_Pixels = 0,
  M12_pBar = 1,
  M12_pArc = 2,
  M12_pCorner = 3
} mapping1D2D_t;

// Settings sub page IDs
#define SUBPAGE_MENU              0
#define SUBPAGE_WIFI              1
#define SUBPAGE_LEDS              2
#define SUBPAGE_UI                3
#define SUBPAGE_SYNC              4
#define SUBPAGE_TIME              5
#define SUBPAGE_SEC               6
#define SUBPAGE_DMX               7
#define SUBPAGE_UM                8
#define SUBPAGE_UPDATE            9
#define SUBPAGE_2D               10
#define SUBPAGE_LOCK            251
#define SUBPAGE_PINREQ          252
#define SUBPAGE_CSS             253
#define SUBPAGE_JS              254
#define SUBPAGE_WELCOME         255

// string temp buffer (now stored in stack locally)
#ifdef ESP8266
#define SETTINGS_STACK_BUF_SIZE 2048
#else
#define SETTINGS_STACK_BUF_SIZE 3608  // warning: quite a large value for stack
#endif

#ifdef WLED_USE_ETHERNET
  #ifdef WLED_ETH_DEFAULT                                          // default ethernet board type if specified
    int ethernetType _INIT(WLED_ETH_DEFAULT);          // ethernet board type
  #else
    int ethernetType _INIT(WLED_ETH_NONE);             // use none for ethernet board type if default not defined
  #endif
#endif


#ifdef ENABLE_DEVFEATURE_LIGHTS__LOAD_HARDCODED_BUSCONFIG_ON_BOOT__16PIN_PARALLEL_OUTPUT_FOR_SNOWTREE
void BusConfig_ManualLoad_16Pin();
#endif // ENABLE_DEVFEATURE_LIGHTS__LOAD_HARDCODED_BUSCONFIG_ON_BOOT__16PIN_PARALLEL_OUTPUT_FOR_SNOWTREE


// Timer mode types
#define NL_MODE_SET               0            //After nightlight time elapsed, set to target brightness
#define NL_MODE_FADE              1            //Fade to target brightness gradually
#define NL_MODE_COLORFADE         2            //Fade to target brightness and secondary color gradually
#define NL_MODE_SUN               3            //Sunrise/sunset. Target brightness is set immediately, then Sunrise effect is started. Max 60 min.

// LED CONFIG
bool turnOnAtBoot _INIT(true);                // turn on LEDs at power-up
byte bootPreset   _INIT(0);                   // save preset to load after power-up

//if true, a segment per bus will be created on boot and LED settings save
//if false, only one segment spanning the total LEDs is created,
//but not on LED settings save if there is more than one segment currently
bool autoSegments    _INIT(false);
bool correctWB       _INIT(false); // CCT color correction of RGB color
bool cctFromRgb      _INIT(false); // CCT is calculated from RGB instead of using seg.cct
bool gammaCorrectCol _INIT(true ); // use gamma correction on colors
bool gammaCorrectBri _INIT(false); // use gamma correction on brightness
float gammaCorrectVal _INIT(2.8f); // gamma correction value

byte col[4]    _INIT_N(({ 255, 160, 0, 0 }));  // current RGB(W) primary color. col[] should be updated if you want to change the color.
byte colSec[4] = UNPACK ({ 0, 0, 0, 0 });      // current RGB(W) secondary color
// byte briS     _INIT(128);                     // default brightness

byte nightlightTargetBri _INIT(0);      // brightness after nightlight is over
byte nightlightDelayMins _INIT(60);
byte nightlightMode      _INIT(NL_MODE_FADE); // See const.h for available modes. Was nightlightFade
bool fadeTransition      _INIT(true);   // enable crossfading color transition
uint16_t transitionDelay _INIT(750);    // default crossfade duration in ms

byte briMultiplier _INIT(100);          // % of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)

// // User Interface CONFIG
// #ifndef SERVERNAME
// char serverDescription[33] _INIT("WLED");  // Name of module - use default
// #else
// char serverDescription[33] _INIT(SERVERNAME);  // use predefined name
// #endif

// //Button type
// #define BTN_TYPE_NONE             0
// #define BTN_TYPE_RESERVED         1
// #define BTN_TYPE_PUSH             2
// #define BTN_TYPE_PUSH_ACT_HIGH    3
// #define BTN_TYPE_SWITCH           4
// #define BTN_TYPE_PIR_SENSOR       5
// #define BTN_TYPE_TOUCH            6
// #define BTN_TYPE_ANALOG           7
// #define BTN_TYPE_ANALOG_INVERTED  8

// // Sync CONFIG
// NodesMap Nodes;
bool nodeListEnabled _INIT(true);
bool nodeBroadcastEnabled _INIT(true);

// byte buttonType[WLED_MAX_BUTTONS]  _INIT({BTN_TYPE_PUSH});
#if defined(IRTYPE) && defined(IRPIN)
byte irEnabled      _INIT(IRTYPE); // Infrared receiver
#else
byte irEnabled      _INIT(0);     // Infrared receiver disabled
#endif
bool irApplyToAllSelected _INIT(true); //apply IR to all selected segments

uint16_t udpPort    _INIT(21324); // WLED notifier default port
uint16_t udpPort2   _INIT(65506); // WLED notifier supplemental port
uint16_t udpRgbPort _INIT(19446); // Hyperion port

uint8_t syncGroups    _INIT(0x01);                    // sync groups this instance syncs (bit mapped)
uint8_t receiveGroups _INIT(0x01);                    // sync receive groups this instance belongs to (bit mapped)
bool receiveNotificationBrightness _INIT(true);       // apply brightness from incoming notifications
bool receiveNotificationColor      _INIT(true);       // apply color
bool receiveNotificationEffects    _INIT(true);       // apply effects setup
bool receiveSegmentOptions         _INIT(false);      // apply segment options
bool receiveSegmentBounds          _INIT(false);      // apply segment bounds (start, stop, offset)
bool notifyDirect _INIT(false);                       // send notification if change via UI or HTTP API
bool notifyButton _INIT(false);                       // send if updated by button or infrared remote
bool notifyAlexa  _INIT(false);                       // send notification if updated via Alexa
bool notifyMacro  _INIT(false);                       // send notification for macro
bool notifyHue    _INIT(true);                        // send notification if Hue light changes
uint8_t udpNumRetries _INIT(0);                       // Number of times a UDP sync message is retransmitted. Increase to increase reliability

uint16_t realtimeTimeoutMs _INIT(2500);               // ms timeout of realtime mode before returning to normal mode
int arlsOffset _INIT(0);                              // realtime LED offset
bool receiveDirect _INIT(true);                       // receive UDP realtime
bool arlsDisableGammaCorrection _INIT(true);          // activate if gamma correction is handled by the source
bool arlsForceMaxBri _INIT(false);                    // enable to force max brightness if source has very dark colors that would be black

#ifdef WLED_USE_ETHERNET
  #define E131_MAX_UNIVERSE_COUNT 20
#else
  #ifdef ESP8266
    #define E131_MAX_UNIVERSE_COUNT 9
  #else
    #define E131_MAX_UNIVERSE_COUNT 12
  #endif
#endif

//E1.31 DMX modes
#define DMX_MODE_DISABLED         0            //not used
#define DMX_MODE_SINGLE_RGB       1            //all LEDs same RGB color (3 channels)
#define DMX_MODE_SINGLE_DRGB      2            //all LEDs same RGB color and master dimmer (4 channels)
#define DMX_MODE_EFFECT           3            //trigger standalone effects of WLED (11 channels)
#define DMX_MODE_MULTIPLE_RGB     4            //every LED is addressed with its own RGB (ledCount * 3 channels)
#define DMX_MODE_MULTIPLE_DRGB    5            //every LED is addressed with its own RGB and share a master dimmer (ledCount * 3 + 1 channels)
#define DMX_MODE_MULTIPLE_RGBW    6            //every LED is addressed with its own RGBW (ledCount * 4 channels)

#ifdef WLED_ENABLE_DMX
 #ifdef ESP8266
  DMXESPSerial dmx;
 #else //ESP32
  SparkFunDMX dmx;
 #endif
uint16_t e131ProxyUniverse _INIT(0);                  // output this E1.31 (sACN) / ArtNet universe via MAX485 (0 = disabled)
#endif
uint16_t e131Universe _INIT(1);                       // settings for E1.31 (sACN) protocol (only DMX_MODE_MULTIPLE_* can span over consequtive universes)
uint16_t e131Port _INIT(5568);                        // DMX in port. E1.31 default is 5568, Art-Net is 6454
// byte e131Priority _INIT(0);                           // E1.31 port priority (if != 0 priority handling is active)
// E131Priority highPriority _INIT(3);                   // E1.31 highest priority tracking, init = timeout in seconds
byte DMXMode _INIT(DMX_MODE_MULTIPLE_RGB);            // DMX mode (s.a.)
uint16_t DMXAddress _INIT(1);                         // DMX start address of fixture, a.k.a. first Channel [for E1.31 (sACN) protocol]
uint16_t DMXSegmentSpacing _INIT(0);                  // Number of void/unused channels between each segments DMX channels
byte e131LastSequenceNumber[E131_MAX_UNIVERSE_COUNT]; // to detect packet loss
bool e131Multicast _INIT(false);                      // multicast or unicast
bool e131SkipOutOfSequence _INIT(false);              // freeze instead of flickering
uint16_t pollReplyCount _INIT(0);                     // count number of replies for ArtPoll node report

// // mqtt
// unsigned long lastMqttReconnectAttempt _INIT(0);  // used for other periodic tasks too
// #ifndef WLED_DISABLE_MQTT
// // AsyncMqttClient *mqtt _INIT(NULL);
// bool mqttEnabled _INIT(false);
// char mqttStatusTopic[40] = {0};//_INIT("");            // this must be global because of async handlers
// char mqttDeviceTopic[33] = {0};//_INIT("");            // main MQTT topic (individual per device, default is wled/mac)
// char mqttGroupTopic[33] = {0};//_INIT("wled/all");     // second MQTT topic (for example to group devices)
// char mqttServer[33] = {0};//_INIT("");                 // both domains and IPs should work (no SSL)
// char mqttUser[41]= {0};// _INIT("");                   // optional: username for MQTT auth
// char mqttPass[65]= {0};// _INIT("");                   // optional: password for MQTT auth
// char mqttClientID[41]= {0};// _INIT("");               // override the client ID
// uint16_t mqttPort _INIT(1883);
// bool retainMqttMsg _INIT(false);               // retain brightness and color
// #define WLED_MQTT_CONNECTED (mqtt != nullptr && mqtt->connected())
// #else
// #define WLED_MQTT_CONNECTED false
// #endif


// #ifndef WLED_DISABLE_HUESYNC
// bool huePollingEnabled _INIT(false);           // poll hue bridge for light state
// uint16_t huePollIntervalMs _INIT(2500);        // low values (< 1sec) may cause lag but offer quicker response
// char hueApiKey[47]  = {0};//_INIT("api");               // key token will be obtained from bridge
// byte huePollLightId _INIT(1);                  // ID of hue lamp to sync to. Find the ID in the hue app ("about" section)
// IPAddress hueIP;// _INIT_N(((0, 0, 0, 0))); // IP address of the bridge
// bool hueApplyOnOff _INIT(true);
// bool hueApplyBri _INIT(true);
// bool hueApplyColor _INIT(true);
// #endif

// uint16_t serialBaud _INIT(1152); // serial baud rate, multiply by 100

// // Time CONFIG
time_t localTime _INIT(0);
// bool ntpEnabled _INIT(false);    // get internet time. Only required if you use clock overlays or time-activated macros
bool useAMPM _INIT(false);       // 12h/24h clock format
// byte currentTimezone _INIT(0);   // Timezone ID. Refer to timezones array in wled10_ntp.ino
// int utcOffsetSecs _INIT(0);      // Seconds to offset from UTC before timzone calculation

#define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
#define MIN_SHOW_DELAY  15
#define DEFAULT_LED_COUNT 30

// byte overlayCurrent _INIT(0);    // 0: no overlay 1: analog clock 2: was single-digit clock 3: was cronixie
// byte overlayMin _INIT(0), overlayMax _INIT(DEFAULT_LED_COUNT - 1);   // boundaries of overlay mode

// // byte analogClock12pixel _INIT(0);               // The pixel in your strip where "midnight" would be
// // bool analogClockSecondsTrail _INIT(false);      // Display seconds as trail of LEDs instead of a single pixel
// // bool analogClock5MinuteMarks _INIT(false);      // Light pixels at every 5-minute position

// // bool countdownMode _INIT(false);                         // Clock will count down towards date
// // byte countdownYear _INIT(20), countdownMonth _INIT(1);   // Countdown target date, year is last two digits
// // byte countdownDay  _INIT(1) , countdownHour  _INIT(0);
// // byte countdownMin  _INIT(0) , countdownSec   _INIT(0);


// // byte macroNl   _INIT(0);        // after nightlight delay over
// // byte macroCountdown _INIT(0);
// // byte macroAlexaOn _INIT(0), macroAlexaOff _INIT(0);
// // byte macroButton[WLED_MAX_BUTTONS]        _INIT({0});
// // byte macroLongPress[WLED_MAX_BUTTONS]     _INIT({0});
// // byte macroDoublePress[WLED_MAX_BUTTONS]   _INIT({0});

// // // Security CONFIG
// bool otaLock     _INIT(false);  // prevents OTA firmware updates without password. ALWAYS enable if system exposed to any public networks
// // bool wifiLock    _INIT(false);  // prevents access to WiFi settings when OTA lock is enabled
// // bool aOtaEnabled _INIT(true);   // ArduinoOTA allows easy updates directly from the IDE. Careful, it does not auto-disable when OTA lock is on
// char settingsPIN[5]  = {0};//_INIT("");  // PIN for settings pages
// bool correctPIN     _INIT(true);
// // unsigned long lastEditTime _INIT(0);

// // uint16_t userVar0 _INIT(0), userVar1 _INIT(0); //available for use in usermod

// #ifdef WLED_ENABLE_DMX
//   // dmx CONFIG
//   byte DMXChannels _INIT(7);        // number of channels per fixture
//   byte DMXFixtureMap[15] _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
//   // assigns the different channels to different functions. See wled21_dmx.ino for more information.
//   uint16_t DMXGap _INIT(10);          // gap between the fixtures. makes addressing easier because you don't have to memorize odd numbers when climbing up onto a rig.
//   uint16_t DMXStart _INIT(10);        // start address of the first fixture
//   uint16_t DMXStartLED _INIT(0);      // LED from which DMX fixtures start
// #endif

// internal global variable declarations
// wifi
bool apActive _INIT(false);
bool forceReconnect _INIT(false);
uint32_t lastReconnectAttempt _INIT(0);
bool interfacesInited _INIT(false);
bool wasConnected _INIT(false);

// color
byte lastRandomIndex _INIT(0);        // used to save last random color so the new one is not the same

// transitions
bool          transitionActive        _INIT(false);
uint16_t      transitionDelayDefault  _INIT(transitionDelay); // default transition time (storec in cfg.json)
uint16_t      transitionDelayTemp     _INIT(transitionDelay); // actual transition duration (overrides transitionDelay in certain cases)
unsigned long transitionStartTime;
float         tperLast                _INIT(0.0f);            // crossfade transition progress, 0.0f - 1.0f
bool          jsonTransitionOnce      _INIT(false);           // flag to override transitionDelay (playlist, JSON API: "live" & "seg":{"i"} & "tt")
uint8_t       randomPaletteChangeTime _INIT(5);               // amount of time [s] between random palette changes (min: 1s, max: 255s)

// nightlight
bool nightlightActive _INIT(false);
bool nightlightActiveOld _INIT(false);
uint32_t nightlightDelayMs _INIT(10);
byte nightlightDelayMinsDefault _INIT(nightlightDelayMins);
unsigned long nightlightStartTime;
byte briNlT _INIT(0);                     // current nightlight brightness
byte colNlT[4] _INIT_N(({ 0, 0, 0, 0 }));        // current nightlight color

// brightness
unsigned long lastOnTime _INIT(0);
bool offMode             _INIT(!turnOnAtBoot);
// byte bri                 _INIT(briS);          // global brightness (set)
byte briOld              _INIT(0);             // global brightnes while in transition loop (previous iteration)
byte briT                _INIT(0);             // global brightness during transition
byte briLast             _INIT(128);           // brightness before turned off. Used for toggle function
byte whiteLast           _INIT(128);           // white channel before turned off. Used for toggle function

#define TOUCH_THRESHOLD 32 // limit to recognize a touch, higher value means more sensitive

// button
bool buttonPublishMqtt                            _INIT(false);
bool buttonPressedBefore[WLED_MAX_BUTTONS]        _INIT({false});
bool buttonLongPressed[WLED_MAX_BUTTONS]          _INIT({false});
unsigned long buttonPressedTime[WLED_MAX_BUTTONS] _INIT({0});
unsigned long buttonWaitTime[WLED_MAX_BUTTONS]    _INIT({0});
bool disablePullUp                                _INIT(false);
byte touchThreshold                               _INIT(TOUCH_THRESHOLD);

//Notifier callMode
#define CALL_MODE_INIT           0     //no updates on init, can be used to disable updates
#define CALL_MODE_DIRECT_CHANGE  1
#define CALL_MODE_BUTTON         2     //default button actions applied to selected segments
#define CALL_MODE_NOTIFICATION   3
#define CALL_MODE_NIGHTLIGHT     4
#define CALL_MODE_NO_NOTIFY      5
#define CALL_MODE_FX_CHANGED     6     //no longer used
#define CALL_MODE_HUE            7
#define CALL_MODE_PRESET_CYCLE   8
#define CALL_MODE_BLYNK          9     //no longer used
#define CALL_MODE_ALEXA         10
#define CALL_MODE_WS_SEND       11     //special call mode, not for notifier, updates websocket only
#define CALL_MODE_BUTTON_PRESET 12     //button/IR JSON preset/macro

unsigned long lastInterfaceUpdate _INIT(0);
byte interfaceUpdateCallMode _INIT(CALL_MODE_INIT);


// notifications
bool notifyDirectDefault _INIT(notifyDirect);
bool receiveNotifications _INIT(true);
unsigned long notificationSentTime _INIT(0);
byte notificationSentCallMode _INIT(CALL_MODE_INIT);
uint8_t notificationCount _INIT(0);

// effects
byte effectCurrent _INIT(0);
byte effectSpeed _INIT(128);
byte effectIntensity _INIT(128);
byte effectPalette _INIT(0);
bool stateChanged _INIT(false);

// network
bool udpConnected _INIT(false), udp2Connected _INIT(false), udpRgbConnected _INIT(false);

// ui style
bool showWelcomePage _INIT(false);

//playlists
// int16_t currentPlaylist _INIT(-1);
//still used for "PL=~" HTTP API command
byte presetCycCurr _INIT(0);
byte presetCycMin _INIT(1);
byte presetCycMax _INIT(5);

#ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
// dns server
DNSServer dnsServer;
#endif // ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL


//realtime override modes
#define REALTIME_OVERRIDE_NONE    0
#define REALTIME_OVERRIDE_ONCE    1
#define REALTIME_OVERRIDE_ALWAYS  2

// realtime
byte realtimeOverride _INIT(REALTIME_OVERRIDE_NONE);
IPAddress realtimeIP;// _INIT_N(((0, 0, 0, 0)));
unsigned long realtimeTimeout _INIT(0);
uint8_t tpmPacketCount _INIT(0);
uint16_t tpmPayloadFrameSize _INIT(0);
bool useMainSegmentOnly _INIT(false);


    /************************************************************************************************
     * SECTION: ConstructJSON
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Segments(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Playlist(uint8_t json_level = 0, bool json_appending = true);
    /**
     * @brief Each mode
     */
    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    uint8_t ConstructJSON_Mode_Ambilight(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    uint8_t ConstructJSON_Mode_SetManual(uint8_t json_level = 0, bool json_appending = true); // probably falls into the E131 type, but here set my mqtt
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    uint8_t ConstructJSON_Auto_Presets(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER
    uint8_t ConstructJSON_Sequencer(uint8_t json_level = 0, bool json_appending = true);
    #endif  
    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS 
    uint8_t ConstructJSON_Matrix(uint8_t json_level = 0, bool json_appending = true);
    #endif
    /**
     * @brief Debug 
     */
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    uint8_t ConstructJSON_Debug_Palette(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    uint8_t ConstructJSON_Debug_Segments(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    uint8_t ConstructJSON_Debug_Palette_Vector(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      uint8_t ConstructJSON_Debug_Animations_Progress(uint8_t json_level = 0, bool json_appending = true);  
      ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
      mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
    #endif

    /************************************************************************************************
     * SECTION: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT
      void MQTTHandler_Init();
      void MQTTHandler_Set_RefreshAll();
      void MQTTHandler_Set_DefaultPeriodRate();  
      void MQTTHandler_Sender();
      #ifdef USE_MODULE_NETWORK_WEBSERVER
      void MQTTHandler_AddWebURL_PayloadRequests();
      #ifdef ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
      void MQTTHandler_AddWebURL_PayloadRequests2();
      #endif // ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
      #endif // USE_MODULE_NETWORK_WEBSERVER
      
      std::vector<struct handler<mAnimatorLight>*> mqtthandler_list;
    
      struct handler<mAnimatorLight> mqtthandler_settings_teleperiod;    
      struct handler<mAnimatorLight> mqtthandler_segments_teleperiod;  
      struct handler<mAnimatorLight> mqtthandler_playlists_teleperiod;
      /**
       * @brief Each mode
       **/
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      struct handler<mAnimatorLight> mqtthandler_mode_ambilight_teleperiod;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      struct handler<mAnimatorLight> mqtthandler_manual_setpixel;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
      struct handler<mAnimatorLight> mqtthandler_automation_presets;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
      struct handler<mAnimatorLight> mqtthandler_automation_playlists;
      #endif   
      #ifdef ENABLE_FEATURE_LIGHTING__SEQUENCER
      struct handler<mAnimatorLight> mqtthandler_automation_sequencer;
      #endif    
      #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS 
      struct handler<mAnimatorLight> mqtthandler_matrix_teleperiod;
      #endif
      /**
       * @brief Debug
       **/
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
      struct handler<mAnimatorLight> mqtthandler_debug_palette;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
      struct handler<mAnimatorLight> mqtthandler_debug_segments;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
      struct handler<mAnimatorLight> mqtthandler_debug_palette_vector;
      #endif
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      struct handler<mAnimatorLight> mqtthandler_debug_animations_progress;
      #endif
    #endif // USE_MODULE_NETWORK_MQTT

    

    /************************************************************************************************
     * SECTION: Development Code
     ************************************************************************************************/

    #ifdef ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    void TestCode_AddBus1();
    void TestCode_Add16ParallelBus1();
    #endif // ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE

};

#endif

#endif


/**
 * @brief 
 * 
 

 It sounds like you're working through an interesting challenge in harmonizing your original animation parameters with the WLED framework, which uses a slightly different approach. Here are some thoughts and suggestions on how you might approach this:

Maintain rate and intensity concepts: Your original design with transition rate and transition time clearly defined how often and how long transitions occur. The concept of rate as a maximum bound (e.g., fading lights over 60 seconds) is intuitive and useful for scenarios where precise control over the effect duration is desired.

Map to WLED’s Parameters: Since WLED uses speed and intensity differently, you could adapt your system to fit within these constraints but still retain some original control:

Speed: Map this to your transition rate. WLED's speed generally affects how quickly an effect progresses or cycles. You could map the inverse of your rate to WLED’s speed scale (0-255), where a lower rate value (more frequent changes) corresponds to a higher speed value.
Intensity: This could be directly used to represent the transition time as a percentage of the transition rate. In WLED, intensity often adjusts the vividness or extent of an effect, but you could repurpose it to control the duration of the transition relative to the cycle.
Handling cycleTime Internally: If you maintain rate in your interface but calculate cycleTime internally based on rate and intensity, it could simplify the user experience while giving you the flexibility to manage how effects are rendered based on the underlying WLED engine. Essentially, cycleTime could be derived as:

cycleTime
=
rate
×
(
intensity
100
)
cycleTime=rate×( 
100
intensity
​
 )
This keeps the user-facing parameters simpler and consistent with your original design ethos.

Special Cases for Certain Effects: For effects like the ambient kitchen lights, where a very slow transition is needed, having rate as a standalone parameter that directly translates to duration (e.g., 60s) makes sense. Here, intensity might be less about the speed of transition and more about how gradual or smooth the transition appears.

Configuration Flexibility: Allow for different configurations where rate and intensity could independently control different aspects of the transition based on the type of effect. For instance, a slow fade might primarily use rate for duration and intensity for the smoothness of the fade, whereas a quick sparkle effect might use intensity to control the brightness variation and rate for the speed of changes.

By considering these aspects, you can create a system that leverages the best parts of both your original design and the WLED framework, maintaining user-friendly controls while ensuring that the underlying technical implementation can adapt dynamically to various effects and scenarios.

 */