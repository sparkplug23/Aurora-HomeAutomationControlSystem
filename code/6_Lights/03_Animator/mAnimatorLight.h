#ifndef _MANIMATORLIGHT_H
#define _MANIMATORLIGHT_H

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 6003 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#define Network WiFi

#include "6_Lights/00_Interface/mBusManager.h"

#include "2_CoreSystem/07_Time/Toki.h"

#include "DynamicBuffer.h"

// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Should nearly always be enabled as default/minimal cases
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS

#ifdef ESP32
#define PIXEL_RANGE_LIMIT 3300
#else
#define PIXEL_RANGE_LIMIT 1000
#endif 


// Temporary fixing for neopixelbusLg issue
#ifndef ANIM_BRIGHTNESS_REQUIRED
#define  false
#error "dont be default yet"
#endif


#define ENABLE_DEVFEATURE_LIGHTING__SLOW_GLOW_LEGACY_FIX


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

/**
 * @brief GetColourPalette defines to make it visually easy to read
 **/
#define PALETTE_WRAP_ON                 true
#define PALETTE_WRAP_OFF                false
#define PALETTE_DISCRETE_OFF            false
#define PALETTE_DISCRETE_ON             true
#define PALETTE_DISCRETE_DEFAULT        2 // Use the prefered method depending on the palette. Gradients will be shown across the segment, discrete will be shown as a single colours sequenced
#define PALETTE_INDEX_SPANS_SEGLEN_ON   true
#define PALETTE_INDEX_IS_INDEX_IN_PALETTE   false
#define PALETTE_SPAN_OFF                false // PALETTE_INDEX_IS_INDEX_IN_PALETTE
#define WLED_PALETTE_MAPPING_ARG_FALSE  false
#define NO_ENCODED_VALUE                nullptr
#define PALETTE_SOLID_WRAP              (paletteBlend == 1 || paletteBlend == 3)
#define SET_BRIGHTNESS                  true
#define BRIGHTNESS_ALREADY_SET          true
#define BRIGHTNESS_NOT_YET_SET          false
#define WITH_BRIGHTNESS_APPLIED         true

#define EFFECT_CONFIG_DEFAULT_OPTION__PALETTE_INDEX_CTR "pal=95" // The default forced palette

/**
 * WLED conversions
 * These are basic defines that remap temporarily from WLED in PulSar. 
 * For optimisation, these will be removed and replaced with the correct values.
 * 
 * Since these are different maps, I should use this name to enable conversion from my method to WLED method, and hence, enable the "d" which is mcol work as expected
 * 
 * Gets a single color from the currently selected palette.
 * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
 * @param mapping if true, LED position in segment is considered for color
 * @param wrap FastLED palettes will usually wrap back to the start smoothly. Set false to get a hard edge
 * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
 * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
 * @returns Single color from palette
 * 
 * 
 **/
// #define color_from_palette(i,mapping,wrap,mcol)    GetPaletteColour(i,mapping,wrap,mcol)inline uint32_t color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255) {
//   return GetPaletteColour(i, mapping, wrap, mcol, pbri);
// }


#define RgbwwColorU32(c)  RGBW32(c.R,c.G,c.B,c.WW) 


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

#define STRINGIFY(X) #X
#define TOSTRING(X) STRINGIFY(X)


// #define WLED_VERSION 0.12.0
#ifndef WLED_RELEASE_NAME
  #define WLED_RELEASE_NAME "Custom"
#endif



#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/* Not used in all effects yet */
#define WLED_FPS         42
// #define FRAMETIME_FIXED  (1000/WLED_FPS)
// #define FRAMETIME_FIXED  25//(1000/WLED_FPS)
// #define FRAMETIME_MS     24
#define FRAMETIME        25
#define FRAMETIME_STATIC        1000 // Static effects with minimal update time
#define USE_ANIMATOR 0 // tmp fix to return as zero, to enable the effect call to keep the animator running

#define FPS_UNLIMITED    0

extern bool realtimeRespectLedMaps; // used in getMappedPixelIndex()

/* Each segment uses 52 bytes of SRAM memory, so if you're application fails because of insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#ifdef ESP8266
  #define MAX_NUM_SEGMENTS    3
  /* How much data bytes all segments combined may allocate */
  #define MAX_SEGMENT_DATA  1000
#else
  #ifndef MAX_NUM_SEGMENTS
    #define MAX_NUM_SEGMENTS  6
  #endif
  #define MAX_SEGMENT_DATA  42767
#endif

#define IBN 5100

/* How much data bytes each segment should max allocate to leave enough space for other segments,
  assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
#define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / getMaxSegments())

#define indexToVStrip(index, stripNr) ((index) | (int((stripNr)+1)<<16))

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

/* How much data bytes each segment should max allocate to leave enough space for other segments,
  assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
// #define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / strip.getMaxSegments())

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
#define IS_TRANSITIONAL ((SEGMENT_I(segment_current_index).options & TRANSITIONAL) == TRANSITIONAL)
#define IS_MIRROR       ((SEGMENT_I(segment_current_index).options & MIRROR      ) == MIRROR      )
#define IS_SEGMENT_ON   ((SEGMENT_I(segment_current_index).options & SEGMENT_ON  ) == SEGMENT_ON  )
#define IS_REVERSE      ((SEGMENT_I(segment_current_index).options & REVERSE     ) == REVERSE     )
#define IS_SELECTED     ((SEGMENT_I(segment_current_index).options & SELECTED    ) == SELECTED    )

// #define MIN_SHOW_DELAY   (_frametime < 16 ? 8 : 15)
#define MINIMUM_SHOW_BACKOFF_PERIOD_MS 30//15

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


#define SEGCOLOR_U32(x)       segments[getCurrSegmentId()].segcol[x].getU32()
#define SEGCOLOR_RGBCCT(x)    segments[getCurrSegmentId()].segcol[x].WithBrightness()
#define SEGCOLOR(x)           SEGCOLOR_U32(x) // default
#define SEGMENT               segments[getCurrSegmentId()]
#define pSEGMENT              tkr_anim->segments[tkr_anim->getCurrSegmentId()]
#define pSEGCOLOR(x)          pSEGMENT.segcol[x].getU32()
#define SEGMENT_I(X)          segments[X] // can this be changed later to "getSegment(X)" and hence protect against out of bounds
#define pSEGMENT_I(X)         tkr_anim->segments[X]
#define SEGLEN                _virtualSegmentLength // This is still using the function, it just relies on calling the function prior to the effect to set this
#define pSEGLEN               tkr_anim->_virtualSegmentLength // This is still using the function, it just relies on calling the function prior to the effect to set this
#define SEG_W            segments[getCurrSegmentId()].vWidth()
#define SEG_H            segments[getCurrSegmentId()].vHeight()
#define SEGPALETTE            SEGMENT.palette_container->CRGB16Palette16_Palette.data
#define pSEGPALETTE            pSEGMENT.palette_container->CRGB16Palette16_Palette.data
#define SEGIDX                getCurrSegmentId()

// WLED Conversions
#define NUM_COLORS RGBCCTCOLOURS_SIZE

#ifndef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  #define WLED_DISABLE_2D // fix for WLED
#endif




#define SPEED_FORMULA_L  5U + (50U*(255U - SEGMENT.speed))/SEGLEN

// Macro to calculate frametime based on SEGLEN and max time in milliseconds (speed = 0 gives max time)
#define FRAMETIME_WITH_SPEED_MAX_MS(max_ms) (5U + (max_ms * (255U - SEGMENT.speed)) / SEGLEN)

// Macro to calculate frametime with a speed-dependent range between min and max times (in milliseconds)
#define FRAMETIME_WITH_SPEED(min_ms, max_ms) \
    (min_ms + ((max_ms - min_ms) * (255U - SEGMENT.speed)) / 255U)


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


#ifndef WLED_DEFINE_GLOBAL_VARS
  // #define extern
  #define _INIT(x)
  #define _INIT_N(x)
  #define _INIT_PROGMEM(x)
#else
  #define WLED_GLOBAL
  #define _INIT(x) = x
  //needed to ignore commas in array definitions
  #define UNPACK( ... ) __VA_ARGS__
  #define _INIT_N(x) UNPACK x
  #define _INIT_PROGMEM(x) PROGMEM = x
#endif


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
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR)         "debug/palette";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_CUSTOM_MAPPING_TABLE__CTR)        "debug/mapping_table";
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
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PERFORMANCE__CTR)        "debug/performance";
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

//wled_math.cpp
//float cos_t(float phi); // use float math
//float sin_t(float phi);
//float tan_t(float x);
int16_t sin16_t(uint16_t theta);
int16_t cos16_t(uint16_t theta);
uint8_t sin8_t(uint8_t theta);
uint8_t cos8_t(uint8_t theta);
float sin_approx(float theta); // uses integer math (converted to float), accuracy +/-0.0015 (compared to sinf())
float cos_approx(float theta);
float tan_approx(float x);
float atan2_t(float y, float x);
float acos_t(float x);
float asin_t(float x);
template <typename T> T atan_t(T x);
float floor_t(float x);
float fmod_t(float num, float denom);
#define sin_t sin_approx
#define cos_t cos_approx
#define tan_t tan_approx

#include <functional>
#define ANIM_FUNCTION_SIGNATURE                             std::function<void(const AnimationParam& param)>                              anim_function_callback
#define ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE           std::function<void()>                                                         anim_progress_mqtt_function_callback
#define ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED             std::function<void(const uint8_t segment_index, const AnimationParam& param)> anim_function_callback_indexed
#define SET_DIRECT_MODE()  SEGMENT.anim_function_callback = nullptr 
#define DIRECT_MODE(x)  SEGMENT.anim_function_callback = nullptr; SEGMENT.cycle_time__rate_ms = x;

#ifdef ESP8266
#define HW_RND_REGISTER RANDOM_REG32
#else // ESP32 family
#include "soc/wdev_reg.h"
#define HW_RND_REGISTER REG_READ(WDEV_RND_REG)
#endif

#define EFFECT_DEFAULT() EffectAnim__Static_Palette__NoBlend()

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
    
    static constexpr const char* PM_MODULE_LIGHTS_ANIMATOR_CTR = D_MODULE_LIGHTS_ANIMATOR_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_ANIMATOR_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID; }
    
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

    
    bool doInitBusses = false; // debug
    bool     doSerializeConfig = false; // debug
    int8_t loadLedmap = -1;
    uint8_t currentLedmap = 0;
    uint16_t desired_pixel;

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL
    uint32_t tSaved_LoopTime = millis();
    #endif

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);
    void FileSystem_JsonAppend_Save_Module();

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

char versionString[16] = TOSTRING(PROJECT_VERSION);
char releaseString[7] = WLED_RELEASE_NAME; // must include the quotes when defining, e.g -D WLED_RELEASE_NAME=\"ESP32_MULTI_USREMODS\"



    // fast (true) random numbers using hardware RNG, all functions return values in the range lowerlimit to upperlimit-1
    // note: for true random numbers with high entropy, do not call faster than every 200ns (5MHz)
    // tests show it is still highly random reading it quickly in a loop (better than fastled PRNG)
    // for 8bit and 16bit random functions: no limit check is done for best speed
    // 32bit inputs are used for speed and code size, limits don't work if inverted or out of range
    // inlining does save code size except for random(a,b) and 32bit random with limits
    // #define random hw_random // replace arduino random()
    inline uint32_t hw_random() { return HW_RND_REGISTER; };
    uint32_t hw_random(uint32_t upperlimit); // not inlined for code size
    int32_t hw_random(int32_t lowerlimit, int32_t upperlimit);
    inline uint16_t hw_random16() { return HW_RND_REGISTER; };
    inline uint16_t hw_random16(uint32_t upperlimit) { return (hw_random16() * upperlimit) >> 16; }; // input range 0-65535 (uint16_t)
    inline int16_t hw_random16(int32_t lowerlimit, int32_t upperlimit) { int32_t range = upperlimit - lowerlimit; return lowerlimit + hw_random16(range); }; // signed limits, use int16_t ranges
    inline uint8_t hw_random8() { return HW_RND_REGISTER; };
    inline uint8_t hw_random8(uint32_t upperlimit) { return (hw_random8() * upperlimit) >> 8; }; // input range 0-255
    inline uint8_t hw_random8(uint32_t lowerlimit, uint32_t upperlimit) { uint32_t range = upperlimit - lowerlimit; return lowerlimit + hw_random8(range); }; // input range 0-255



    void reset(); // tmp use wled reboot, later make sure to use the system reboot method
            
    #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    byte correctionRGB[4] = {0,0,0,0};
    uint16_t lastKelvin = 0;
    static void colorKtoRGB(uint16_t kelvin, byte* rgb);
    uint32_t colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb);
    static uint16_t approximateKelvinFromRGB(uint32_t rgb);
    #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

    unsigned long presetsModifiedTime = 0;
    const char *getPresetsFileName(bool persistent = true);

    void EveryLoop();    
     
    void EverySecond_AutoOff();
    void BootMessage();

    #ifdef ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
    // This should be made into a custom controller
    struct KeyInput_ControlLights_s{
      /**
       * @brief 
       * 0: Disabled
       * 1: Single button demo/tester
       * 2: Single button multifunction controller
       * 3: Two button controler (mode, brightness)
       */
      uint8_t mode = 1; 
      uint8_t demo_state = 0;
    }keyinput_control;
    void KeyInput__ControlLights();
    #endif
    #ifdef ENABLE_FEATURE_LIGHTS__DEMO_MODE
    void SubTask_Demo();
    #endif
                
    
    #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
    void Test_Config();
    #endif

    /************************************************************************************************
     * SECTION: Modes
     ************************************************************************************************/
    enum ANIMATION_MODE
    {    
      ANIMATION_MODE__EFFECTS = 0, // Not realtime
      ANIMATION_MODE__REALTIME_MQTT_SETPIXEL, // Using json encoded message to set the pixels directly
      ANIMATION_MODE__REALTIME_UDP,
      ANIMATION_MODE__REALTIME_HYPERION,
      ANIMATION_MODE__REALTIME_E131,
      ANIMATION_MODE__REALTIME_ADALIGHT,
      ANIMATION_MODE__REALTIME_ARTNET,
      ANIMATION_MODE__REALTIME_TPM2NET,
      ANIMATION_MODE__REALTIME_DDP,  
      ANIMATION_MODE__LENGTH_ID
    };   
    #ifdef ENABLE_FEATURE_LIGHTING__REALTIME_MODES 
    void SubTask_RealTime_SetPixel();
    int8_t GetAnimationModeIDbyName(const char* c);
    const char* GetAnimationModeName(char* buffer, uint16_t buflen);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen);
    void CommandSet_AnimationModeID(uint8_t value);
    #endif // ENABLE_FEATURE_LIGHTING__REALTIME_MODES


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index = 0);    
    
    #ifdef ENABLE_DEVFEATURE_LIGHTING__COMMANDS_NOTIFICATION_SHORTCUT
    void parsesub_json_object_notification_shortcut(JsonParserObject obj);
    #endif

    void CommandSet_CustomPalette(uint8_t index, uint16_t encoding, uint8_t* data, uint8_t data_length);
   
    void CommandSet_Effect_ColourTypeID(uint8_t id, uint8_t segment_index = 0);

    #ifdef ENABLE_DEVFEATURE_LIGHTING__COLOURHEATMAP_PALETTE
    void CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index = 0, uint8_t palette_id = 255);
    #endif

    void CommandSet_PaletteID(uint16_t value, uint8_t segment_index = 0);

    void CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index = 0);
    int16_t GetFlasherFunctionIDbyName(const char* f);
    const char* GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index = 0);
    const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen, bool return_first_option_if_not_found = false);

    void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index = 0);

    void CommandSet_HardwareColourOrderTypeByStr(const char* value, uint8_t segment_index = 0);
    void CommandSet_ColourTypeByStr(const char* value, uint8_t segment_index = 0);
    void CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index= 0);
  
    void CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index = 0);
    void CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index = 0);
    
    
    void CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new , uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);
    void CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);


    uint8_t GetNumberOfColoursInPalette(uint16_t palette_id);

    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
    **  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    void fill(uint32_t c);
    void fill(RgbwwColor c);
    void fill_ranged(uint32_t c);

// legacy to remove
    static uint32_t ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend);
    // #define color_blend ColourBlend


// Time CONFIG
#ifndef WLED_NTP_ENABLED
  #define WLED_NTP_ENABLED false
#endif
#ifndef WLED_TIMEZONE
  #define WLED_TIMEZONE 0
#endif
#ifndef WLED_UTC_OFFSET
  #define WLED_UTC_OFFSET 0
#endif
bool ntpEnabled      _INIT(WLED_NTP_ENABLED); // get internet time. Only required if you use clock overlays or time-activated macros
bool useAMPM         _INIT(false);            // 12h/24h clock format
byte currentTimezone _INIT(WLED_TIMEZONE);    // Timezone ID. Refer to timezones array in wled10_ntp.ino
int utcOffsetSecs    _INIT(WLED_UTC_OFFSET);  // Seconds to offset from UTC before timzone calculation

byte overlayCurrent _INIT(0);    // 0: no overlay 1: analog clock 2: was single-digit clock 3: was cronixie
byte overlayMin _INIT(0), overlayMax _INIT(DEFAULT_LED_COUNT - 1);   // boundaries of overlay mode

byte analogClock12pixel _INIT(0);               // The pixel in your strip where "midnight" would be
bool analogClockSecondsTrail _INIT(false);      // Display seconds as trail of LEDs instead of a single pixel
bool analogClock5MinuteMarks _INIT(false);      // Light pixels at every 5-minute position
bool analogClockSolidBlack _INIT(false);        // Show clock overlay only if all LEDs are solid black (effect is 0 and color is black)

bool countdownMode _INIT(false);                         // Clock will count down towards date
byte countdownYear _INIT(20), countdownMonth _INIT(1);   // Countdown target date, year is last two digits
byte countdownDay  _INIT(1) , countdownHour  _INIT(0);
byte countdownMin  _INIT(0) , countdownSec   _INIT(0);

uint16_t serialBaud _INIT(1152); // serial baud rate, multiply by 100
bool     serialCanRX _INIT(false);
bool     serialCanTX _INIT(false);
#ifndef WLED_MAX_BUTTONS
  #ifdef ESP8266
    #define WLED_MAX_BUTTONS 2
  #else
    #define WLED_MAX_BUTTONS 4
  #endif
#else
  #if WLED_MAX_BUTTONS < 2
    #undef WLED_MAX_BUTTONS
    #define WLED_MAX_BUTTONS 2
  #endif
#endif
byte macroNl   _INIT(0);        // after nightlight delay over
byte macroCountdown _INIT(0);
byte macroAlexaOn _INIT(0), macroAlexaOff _INIT(0);
byte macroButton[WLED_MAX_BUTTONS]        _INIT({0});
byte macroLongPress[WLED_MAX_BUTTONS]     _INIT({0});
byte macroDoublePress[WLED_MAX_BUTTONS]   _INIT({0});

// Security CONFIG
bool otaLock     _INIT(false);  // prevents OTA firmware updates without password. ALWAYS enable if system exposed to any public networks
bool wifiLock    _INIT(false);  // prevents access to WiFi settings when OTA lock is enabled
bool aOtaEnabled _INIT(true);   // ArduinoOTA allows easy updates directly from the IDE. Careful, it does not auto-disable when OTA lock is on
char settingsPIN[5] _INIT("");  // PIN for settings pages
bool correctPIN     _INIT(true);
unsigned long lastEditTime _INIT(0);


uint16_t userVar0 _INIT(0), userVar1 _INIT(0); //available for use in usermod

// countdown
unsigned long countdownTime _INIT(1514764800L);
bool countdownOverTriggered _INIT(true);

//timer
byte lastTimerMinute  _INIT(0);
byte timerHours[]     _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
int8_t timerMinutes[] _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
byte timerMacro[]     _INIT_N(({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }));
//weekdays to activate on, bit pattern of arr elem: 0b11111111: sun,sat,fri,thu,wed,tue,mon,validity
byte timerWeekday[]   _INIT_N(({ 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }));
//upper 4 bits start, lower 4 bits end month (default 28: start month 1 and end month 12)
byte timerMonth[]     _INIT_N(({28,28,28,28,28,28,28,28}));
byte timerDay[]       _INIT_N(({1,1,1,1,1,1,1,1}));
byte timerDayEnd[]		_INIT_N(({31,31,31,31,31,31,31,31}));
bool doAdvancePlaylist _INIT(false);

/*
 * color blend function, based on FastLED blend function
 * the calculation for each color is: result = (A*(amountOfA) + A + B*(amountOfB) + B) / 256 with amountOfA = 255 - amountOfB
 
 2025 version
 */
inline uint32_t color_blend(uint32_t color1, uint32_t color2, uint8_t blend) {
  // min / max blend checking is omitted: calls with 0 or 255 are rare, checking lowers overall performance
  uint32_t rb1 = color1 & 0x00FF00FF;
  uint32_t wg1 = (color1>>8) & 0x00FF00FF;
  uint32_t rb2 = color2 & 0x00FF00FF;
  uint32_t wg2 = (color2>>8) & 0x00FF00FF;
  uint32_t rb3 = ((((rb1 << 8) | rb2) + (rb2 * blend) - (rb1 * blend)) >> 8) & 0x00FF00FF;
  uint32_t wg3 = ((((wg1 << 8) | wg2) + (wg2 * blend) - (wg1 * blend))) & 0xFF00FF00;
  return rb3 | wg3;
}

    void Init_Segments();

    #ifdef ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    JsonDocument *pDoc = &doc;
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
    void SubTask_Presets();
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
      uint8_t preset; //ID of the preset to apply, since they can be any order
      uint16_t dur;   //Duration of the entry (in tenths of seconds)
      uint16_t tr;    //Duration of the transition TO this entry (in tenths of seconds)
    } ple;


    byte           playlistRepeat = 1;        //how many times to repeat the playlist (0 = infinitely)
    byte           playlistEndPreset = 0;     //what preset to apply after playlist end (0 = stay on last preset)
    byte           playlistOptions = 0;       //bit 0: shuffle playlist after each iteration. bits 1-7 TBD

    PlaylistEntry *playlistEntries = nullptr;
    byte           playlistLen = 0;               //number of playlist entries
    int8_t         playlistIndex = -1;
    uint16_t       playlistEntryDur = 0;      //duration of the current entry in tenths of seconds
    
    int16_t currentPlaylist = -1;

    uint32_t tSaved_playlist_debug = 0;


    void shufflePlaylist();
    void unloadPlaylist();
    int16_t loadPlaylist(JsonObject playlistObj, byte presetId);
    void SubTask_Playlist();
    void serializePlaylist(JsonObject sObj);    

    #endif // ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS


    bool isAsterisksOnly(const char* str, byte maxLen);


    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS
    void handleSettingsSet(AsyncWebServerRequest *request, byte subPage);
    bool handleSet(AsyncWebServerRequest *request, const String& req, bool apply=true);
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

    /******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ****** Effect Functions *************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Solid_Colour(); 
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Static_Palette();
    uint16_t EffectAnim__Static_Palette__NoBlend();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Firefly();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Flicker_Base(bool use_multi = false, uint16_t flicker_palette = 0);
    uint16_t EffectAnim__Candle_Single();
    uint16_t EffectAnim__Candle_Multiple();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Shimmering_Two_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Shimmering_Palette_Saturation();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Gradient_Palette_SegWidth();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Stepping_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__TimeBased__HourProgress();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Static_Palette_Varied();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Stepping_Palette_With_Background();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    uint16_t EffectAnim__Popping_Decay_Palette_To_Black();
    uint16_t EffectAnim__Popping_Decay_Random_To_Black();
    uint16_t EffectAnim__Popping_Decay_Palette_To_White();
    uint16_t EffectAnim__Popping_Decay_Random_To_White();
    uint16_t EffectAnim__Popping_Decay_Base(bool draw_palette_inorder, bool fade_to_black);
    #endif 
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Split_Palette_SegWidth();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    uint16_t EffectAnim__Randomise_Gradient_Palette_SegWidth();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Rotate_Base(uint16_t movement_amount=1, bool direction=false);
    uint16_t EffectAnim__Rotating_Palette();
    uint16_t EffectAnim__Rotating_Previous_Animation();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t Segments_RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction);
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    // uint16_t EffectAnim__Palette_Colour_Fade_Saturation();
    // #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Blend_Two_Palettes();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Twinkle_Palette_Onto_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Twinkle_Out_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    uint16_t EffectAnim__Twinkle_Decaying_Palette();
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    // Static
    uint16_t EffectAnim__Palette_Lit_Pattern();
    uint16_t EffectAnim__TriSegCol_Lit_Pattern();
    uint16_t EffectAnim__Palettes_Interleaved_Lit_Pattern();
    uint16_t EffectAnim__Palettes_Interleaved();
    uint16_t EffectAnim__Base_Spots(uint16_t threshold);
    uint16_t EffectAnim__Spots();
    uint16_t EffectAnim__Percent();
    // One colour changes
    uint16_t EffectAnim__Random_Colour();
    // Wipe/Sweep/Runners 
    uint16_t BaseEffectAnim__Base_Colour_Wipe(bool rev, bool useRandomColors, bool useIterateOverPalette = false);
    uint16_t EffectAnim__Colour_Wipe();
    uint16_t EffectAnim__Colour_Wipe_Random();
    uint16_t EffectAnim__Colour_Wipe_Palette();
    uint16_t EffectAnim__Colour_Sweep();
    uint16_t EffectAnim__Colour_Sweep_Random();
    uint16_t EffectAnim__Colour_Sweep_Palette();
    uint16_t EffectAnim__Dynamic();
    uint16_t EffectAnim__Dynamic_Smooth();
    uint16_t EffectAnim__TriColour_Wipe();
    uint16_t EffectAnim__Android();
    uint16_t EffectAnim__Base_Running(bool saw, bool dual=false);
    uint16_t EffectAnim__Base_Running(uint32_t color1, uint32_t color2, bool theatre = false);
    uint16_t EffectAnim__Running_Colour();
    uint16_t EffectAnim__Running_Random();
    uint16_t EffectAnim__Base_Gradient(bool loading);
    uint16_t EffectAnim__Gradient();
    uint16_t EffectAnim__Loading();
    uint16_t EffectAnim__Rolling_Balls();
    uint16_t EffectAnim__Base_Police(uint32_t color1, uint32_t color2, bool all);
    uint16_t EffectAnim__Police();
    uint16_t EffectAnim__Police_All();
    uint16_t EffectAnim__Fairy();
    uint16_t EffectAnim__Fairy_Twinkle();
    uint16_t EffectAnim__Running_Dual();
    uint16_t EffectAnim__Two_Dots();
    uint16_t EffectAnim__Two_Areas();
    uint16_t EffectAnim__Multi_Comet();
    uint16_t EffectAnim__Oscillate();
    uint16_t EffectAnim__BPM();
    uint16_t EffectAnim__Juggle();
    uint16_t EffectAnim__Palette();
    uint16_t EffectAnim__ColourWaves();
    uint16_t EffectAnim__Lake();
    void EffectAnim__Glitter_Base(uint8_t intensity, uint32_t col = ULTRAWHITE);
    uint16_t EffectAnim__Glitter();
    uint16_t EffectAnim__Meteor(); 
    uint16_t EffectAnim__Pride_2015();    
    CRGB EffectAnim__Pacifica_Base_OneLayer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);   
    uint16_t EffectAnim__Pacifica();    
    uint16_t EffectAnim__Sunrise();    
    uint16_t EffectAnim__Sinewave();    
    uint16_t EffectAnim__Flow();    
    uint16_t EffectAnim__Base_Phased(uint8_t moder);
    uint16_t EffectAnim__PhasedNoise();    
    uint16_t EffectAnim__Phased();    
    uint16_t EffectAnim__Running_Lights();    
    uint16_t EffectAnim__Rainbow_Cycle();    
    // Chase    
    uint16_t EffectAnim__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette);
    uint16_t EffectAnim__Chase_Colour();
    uint16_t EffectAnim__Chase_Random();
    uint16_t EffectAnim__Chase_Rainbow();
    uint16_t EffectAnim__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette);
    uint16_t EffectAnim__Chase_Flash();
    uint16_t EffectAnim__Chase_Flash_Random();
    uint16_t EffectAnim__Chase_Rainbow_White();
    uint16_t EffectAnim__Chase_Theater();
    uint16_t EffectAnim__Chase_Theatre_Rainbow();
    uint16_t EffectAnim__Base_Chase_TriColour(uint32_t color1, uint32_t color2);
    uint16_t EffectAnim__Chase_TriColour();
    // Breathe/Fade/Pulse
    uint16_t EffectAnim__Breath();
    uint16_t EffectAnim__Fade();
    uint16_t EffectAnim__Fade_TriColour();
    uint16_t EffectAnim__Fade_Spots();
    // Fireworks
    uint16_t EffectAnim__Fireworks();
    uint16_t EffectAnim__Exploding_Fireworks();
    uint16_t EffectAnim__Fireworks_Starburst();
    uint16_t EffectAnim__Rain();
    uint16_t EffectAnim__Tetrix();
    uint16_t EffectAnim__Fire_Flicker();
    uint16_t EffectAnim__Exploding_Fireworks_NoLaunch();
    // Sparkle/Twinkle
    uint16_t EffectAnim__Solid_Glitter();
    uint16_t EffectAnim__Popcorn();
    uint16_t EffectAnim__Plasma();
    uint16_t EffectAnim__Sparkle();
    uint16_t EffectAnim__Sparkle_Flash();
    uint16_t EffectAnim__Sparkle_Hyper();
    uint16_t EffectAnim__Twinkle();
    CRGB EffectAnim__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat);
    uint16_t EffectAnim__Base_Twinkle_Fox(bool cat);
    uint16_t EffectAnim__Twinkle_Colour();
    uint16_t EffectAnim__Twinkle_Fox();
    uint16_t EffectAnim__Twinkle_Cat();
    uint16_t EffectAnim__Twinkle_Up();
    uint16_t EffectAnim__Halloween_Eyes();
    uint16_t EffectAnim__Saw();
    uint16_t EffectAnim__Base_Dissolve(uint32_t color);
    uint16_t EffectAnim__Dissolve();
    uint16_t EffectAnim__Dissolve_Random();
    uint16_t EffectAnim__ColourFul();
    uint16_t EffectAnim__Traffic_Light();
    // Blink/Strobe
    uint16_t EffectAnim__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette);
    uint16_t EffectAnim__Blink();
    uint16_t EffectAnim__Blink_Rainbow();
    uint16_t EffectAnim__Strobe();
    uint16_t EffectAnim__Strobe_Multi();
    uint16_t EffectAnim__Strobe_Rainbow();
    uint16_t EffectAnim__Rainbow();
    uint16_t EffectAnim__Lightning();
    uint16_t EffectAnim__Fire_2012();
    uint16_t EffectAnim__Railway();
    uint16_t EffectAnim__Heartbeat();
    //Noise
    uint16_t EffectAnim__FillNoise8();
    uint16_t EffectAnim__Noise16_1();
    uint16_t EffectAnim__Noise16_2();
    uint16_t EffectAnim__Noise16_3();
    uint16_t EffectAnim__Noise16_4();
    uint16_t EffectAnim__Noise_Pal();
    // Scan
    uint16_t EffectAnim__Base_Scan(bool dual);
    uint16_t EffectAnim__Scan();
    uint16_t EffectAnim__Scan_Dual();
    uint16_t EffectAnim__Larson_Scanner();
    uint16_t EffectAnim__Larson_Scanner_Dual();
    uint16_t EffectAnim__ICU();
    uint16_t EffectAnim__Base_Ripple(uint8_t blurAmount = 0);
    uint16_t EffectAnim__Ripple();
    uint16_t EffectAnim__Ripple_Rainbow(); 
    uint16_t EffectAnim__Comet();
    uint16_t EffectAnim__Chunchun();
    uint16_t EffectAnim__Dancing_Shadows();
    uint16_t EffectAnim__Washing_Machine();
    uint16_t EffectAnim__Blends();
    uint16_t EffectAnim__TV_Simulator();
    uint16_t EffectAnim__Bouncing_Balls();
    uint16_t EffectAnim__Base_Sinelon(bool dual, bool rainbow=false);
    uint16_t EffectAnim__Sinelon();
    uint16_t EffectAnim__Sinelon_Dual();
    uint16_t EffectAnim__Sinelon_Rainbow();
    uint16_t EffectAnim__Drip();
    uint16_t EffectAnim__FlowStripe();
    uint16_t EffectAnim__WaveSins();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
    uint16_t EffectAnim__Hardware__Show_Bus();
    uint16_t EffectAnim__Hardware__Manual_Pixel_Counting();
    uint16_t EffectAnim__Hardware__View_Pixel_Range();
    uint16_t EffectAnim__Hardware__Light_Sensor_Pixel_Indexing();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    uint16_t EffectAnim__SunPositions__Sunrise_Alarm_01();
    uint16_t EffectAnim__SunPositions__Azimuth_Selects_Gradient_Of_Palette_01();
    uint16_t EffectAnim__SunPositions__Sunset_Blended_Palettes_01();
    uint16_t EffectAnim__SunPositions__DrawSun_1D_Elevation_01();
    uint16_t EffectAnim__SunPositions__DrawSun_1D_Azimuth_01();
    uint16_t EffectAnim__SunPositions__DrawSun_2D_Elevation_And_Azimuth_01();
    uint16_t EffectAnim__SunPositions__White_Colour_Temperature_CCT_Based_On_Elevation_01();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
    uint16_t EffectAnim__Manual__PixelSetElsewhere();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    void SubTask_Flasher_Animate_Function_Tester_01();
    void SubTask_Flasher_Animate_Function_Tester_02();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
    uint16_t EffectAnim__BorderWallpaper__TwoColour_Gradient();
    uint16_t EffectAnim__BorderWallpaper__FourColour_Gradient();
    uint16_t EffectAnim__BorderWallpaper__FourColour_Solid();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_TRACKING
    uint16_t EffectAnim__SolarTriggers__Sunrise_01();
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_TRACKING
    
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    uint16_t EffectAnim__Christmas_Musical__01();
    #endif 
        
    /****************************************************************************************************************************************************************************
    *** Specialised: 2D (No Audio) **********************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    uint16_t EffectAnim__2D__Blackhole();
    uint16_t EffectAnim__2D__ColouredBursts();
    uint16_t EffectAnim__2D__DNA();
    uint16_t EffectAnim__2D__DNASpiral();
    uint16_t EffectAnim__2D__Drift();
    uint16_t EffectAnim__2D__FireNoise();
    uint16_t EffectAnim__2D__Frizzles();
    uint16_t EffectAnim__2D__GameOfLife();
    uint16_t EffectAnim__2D__Hipnotic();
    uint16_t EffectAnim__2D__Julia();
    uint16_t EffectAnim__2D__Lissajous();
    uint16_t EffectAnim__2D__Matrix();
    uint16_t EffectAnim__2D__Metaballs();
    uint16_t EffectAnim__2D__Noise();
    uint16_t EffectAnim__2D__PlasmaBall();
    uint16_t EffectAnim__2D__PolarLights();
    uint16_t EffectAnim__2D__Pulser();
    uint16_t EffectAnim__2D__SinDots();
    uint16_t EffectAnim__2D__SqauredSwirl();
    uint16_t EffectAnim__2D__SunRadiation();
    uint16_t EffectAnim__2D__Tartan();
    uint16_t EffectAnim__2D__SpaceShips();
    uint16_t EffectAnim__2D__CrazyBees();
    uint16_t EffectAnim__2D__GhostRider();
    uint16_t EffectAnim__2D__FloatingBlobs();
    uint16_t EffectAnim__2D__DriftRose();
    uint16_t EffectAnim__2D__DistortionWaves();
    uint16_t EffectAnim__2D__Soap();
    uint16_t EffectAnim__2D__Octopus();
    uint16_t EffectAnim__2D__WavingCell();
    uint16_t EffectAnim__2D__ScrollingText();
    uint16_t EffectAnim__2D__DigitalClock();
    uint16_t EffectAnim__2D__PlasmaRotoZoom();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: 1D (Audio Reactive) ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
    uint16_t EffectAnim__AudioReactive__1D__FFT_Ripple_Peak();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Perline_Move();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Aurora();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Juggles();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Matripix();
    uint16_t EffectAnim__AudioReactive__1D__FFT_MidNoise();
    uint16_t EffectAnim__AudioReactive__1D__FFT_NoiseFire();
    uint16_t EffectAnim__AudioReactive__1D__FFT_NoiseMeter();
    uint16_t EffectAnim__AudioReactive__1D__FFT_PixelWave();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Plasmoid();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Puddle__Base(bool peakdetect);
    uint16_t EffectAnim__AudioReactive__1D__FFT_PuddlePeak();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Puddles();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Pixels();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Blurz();
    uint16_t EffectAnim__AudioReactive__1D__FFT_DJLight();
    uint16_t EffectAnim__AudioReactive__1D__FFT_FreqMap();
    uint16_t EffectAnim__AudioReactive__1D__FFT_FreqMatrix();
    uint16_t EffectAnim__AudioReactive__1D__FFT_FreqPixels();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Grav__Base(unsigned mode);
    uint16_t EffectAnim__AudioReactive__1D__FFT_GravCenter();
    uint16_t EffectAnim__AudioReactive__1D__FFT_GravCentric();
    uint16_t EffectAnim__AudioReactive__1D__FFT_GravMeter();
    uint16_t EffectAnim__AudioReactive__1D__FFT_GravFreq();
    uint16_t EffectAnim__AudioReactive__1D__FFT_NoiseMove();
    uint16_t EffectAnim__AudioReactive__1D__FFT_RockTaves();
    uint16_t EffectAnim__AudioReactive__1D__FFT_FreqWave();
    uint16_t EffectAnim__AudioReactive__1D__FFT_Waterfall();
    #endif
    /****************************************************************************************************************************************************************************
    *** Specialised: 2D (Audio Reactive) ****************************************************************************************************************************************
    **  Requires:     ***********************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    uint16_t EffectAnim__AudioReactive__2D__Swirl();
    uint16_t EffectAnim__AudioReactive__2D__Waverly();
    uint16_t EffectAnim__AudioReactive__2D__FFT_GED();
    uint16_t EffectAnim__AudioReactive__2D__FFT_FunkyPlank();
    uint16_t EffectAnim__AudioReactive__2D__FFT_Akemi();
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

    uint16_t EffectAnim__7SegmentDisplay__ClockTime_01();
    uint16_t EffectAnim__7SegmentDisplay__ClockTime_02();
    uint16_t EffectAnim__7SegmentDisplay__ManualNumber_01();
    uint16_t EffectAnim__7SegmentDisplay__ManualString_01();
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
      // General Level 1 Minimal Home Effects
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
      EFFECTS_FUNCTION__SOLID_COLOUR__ID,
      EFFECTS_FUNCTION__STATIC_PALETTE__ID,
      EFFECTS_FUNCTION__SPLIT_PALETTE_SEGWIDTH__ID,
      EFFECTS_FUNCTION__FIREFLY__ID,
      EFFECTS_FUNCTION__CANDLE_SINGLE__ID,
      EFFECTS_FUNCTION__CANDLE_MULTIPLE__ID,
      EFFECTS_FUNCTION__RANDOMISE_GRADIENT_PALETTE_SEGWIDTH__ID,
      EFFECTS_FUNCTION__STATIC_PALETTE_VARIED__ID,
      #endif

      // General Level 2 Flashing Basic Effects
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
      EFFECTS_FUNCTION__SHIMMERING_PALETTE_DOUBLE__ID,
      EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__ID,
      EFFECTS_FUNCTION__GRADIENT_PALETTE_SEGWIDTH__ID,
      EFFECTS_FUNCTION__ROTATING_PALETTE__ID,
      EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,
      EFFECTS_FUNCTION__STEPPING_PALETTE_WITH_BACKGROUND__ID,
      EFFECTS_FUNCTION__STEPPING_PALETTE__ID,
      EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,
      EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID,
      EFFECTS_FUNCTION__TWINKLE_OFF_PALETTE__ID,
      EFFECTS_FUNCTION__TIMEBASED__HOUR_PROGRESS__ID,
      EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__ID,
      #endif

      // General Level 3 Flashing Extended Effects
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
      EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_BLACK__ID,
      EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_BLACK__ID,
      EFFECTS_FUNCTION__POPPING_DECAY_PALETTE_TO_WHITE__ID,
      EFFECTS_FUNCTION__POPPING_DECAY_RANDOM_TO_WHITE__ID,
      EFFECTS_FUNCTION__PALETTE_LIT_PATTERN__ID,
      EFFECTS_FUNCTION__TRISEGCOL_LIT_PATTERN__ID,
      EFFECTS_FUNCTION__PALETTES_INTERLEAVED_LIT_PATTERN__ID,
      EFFECTS_FUNCTION__PALETTES_INTERLEAVED__ID,
      #endif

      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

      // General Level 4 Flashing Complete Effects
      EFFECTS_FUNCTION__SPOTS__ID,
      EFFECTS_FUNCTION__PERCENT__ID,
      EFFECTS_FUNCTION__RANDOM_COLOR__ID,       
      EFFECTS_FUNCTION__TRICOLOR_WIPE__ID,
      /**
       * Wipe/Sweep/Runners 
       **/
      EFFECTS_FUNCTION__COLOR_WIPE__ID,
      EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID,
      EFFECTS_FUNCTION__COLOR_WIPE_PALETTE__ID,
      EFFECTS_FUNCTION__COLOR_SWEEP__ID,
      EFFECTS_FUNCTION__COLOR_SWEEP_RANDOM__ID,
      EFFECTS_FUNCTION__COLOR_SWEEP_PALETTE__ID,       
      EFFECTS_FUNCTION__DYNAMIC__ID,                  
      EFFECTS_FUNCTION__DYNAMIC_SMOOTH__ID,                
      EFFECTS_FUNCTION__RUNNING_COLOR__ID,
      EFFECTS_FUNCTION__RUNNING_RANDOM__ID,      
      EFFECTS_FUNCTION__ANDROID__ID,               
      EFFECTS_FUNCTION__GRADIENT__ID,
      EFFECTS_FUNCTION__LOADING__ID,
      EFFECTS_FUNCTION__ROLLINGBALLS__ID,
      EFFECTS_FUNCTION__POLICE__ID,
      EFFECTS_FUNCTION__POLICE_ALL__ID,
      EFFECTS_FUNCTION__FAIRY__ID,
      EFFECTS_FUNCTION__TWO_DOTS__ID,
      EFFECTS_FUNCTION__TWO_AREAS__ID,                
      EFFECTS_FUNCTION__FAIRYTWINKLE__ID,                
      EFFECTS_FUNCTION__RUNNING_DUAL__ID,   
      EFFECTS_FUNCTION__MULTI_COMET__ID,
      EFFECTS_FUNCTION__OSCILLATE__ID,
      EFFECTS_FUNCTION__BPM__ID,
      EFFECTS_FUNCTION__JUGGLE__ID,
      EFFECTS_FUNCTION__PALETTE__ID,
      EFFECTS_FUNCTION__COLORWAVES__ID,
      EFFECTS_FUNCTION__LAKE__ID,
      EFFECTS_FUNCTION__GLITTER__ID,
      EFFECTS_FUNCTION__METEOR__ID,
      EFFECTS_FUNCTION__PRIDE_2015__ID,
      EFFECTS_FUNCTION__PACIFICA__ID,
      EFFECTS_FUNCTION__SUNRISE__ID,
      EFFECTS_FUNCTION__SINEWAVE__ID,
      EFFECTS_FUNCTION__FLOW__ID,
      EFFECTS_FUNCTION__RUNNING_LIGHTS__ID,
      EFFECTS_FUNCTION__RAINBOW_CYCLE__ID,

      /**
       * Chase
       **/
      EFFECTS_FUNCTION__CHASE_COLOR__ID,
      EFFECTS_FUNCTION__CHASE_RANDOM__ID,
      EFFECTS_FUNCTION__CHASE_RAINBOW__ID,
      EFFECTS_FUNCTION__CHASE_FLASH__ID,
      EFFECTS_FUNCTION__CHASE_FLASH_RANDOM__ID,
      EFFECTS_FUNCTION__CHASE_RAINBOW_WHITE__ID,
      EFFECTS_FUNCTION__CHASE_THEATER__ID,
      EFFECTS_FUNCTION__CHASE_THEATER_RAINBOW__ID,
      EFFECTS_FUNCTION__CHASE_TRICOLOR__ID,

      /**
       *  Breathe/Fade/Pulse
       **/      
      EFFECTS_FUNCTION__BREATH__ID,
      EFFECTS_FUNCTION__FADE__ID,
      EFFECTS_FUNCTION__FADE_TRICOLOR__ID,
      EFFECTS_FUNCTION__FADE_SPOTS__ID,
      
      /**
       * Sparkle/Twinkle
       **/
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
      EFFECTS_FUNCTION__HALLOWEEN_EYES__ID,
      EFFECTS_FUNCTION__SAW__ID,
      EFFECTS_FUNCTION__DISSOLVE__ID,
      EFFECTS_FUNCTION__DISSOLVE_RANDOM__ID,
      EFFECTS_FUNCTION__COLORFUL__ID,
      EFFECTS_FUNCTION__TRAFFIC_LIGHT__ID,

      /**
       * Fireworks
       **/
      EFFECTS_FUNCTION__FIREWORKS__ID,                
      EFFECTS_FUNCTION__FIREWORKS_EXPLODING__ID, 
      EFFECTS_FUNCTION__FIREWORKS_STARBURST__ID,
      EFFECTS_FUNCTION__FIREWORKS_STARBURST_GLOWS__ID, 
      EFFECTS_FUNCTION__RAIN__ID,                       
      EFFECTS_FUNCTION__TETRIX__ID,                 
      EFFECTS_FUNCTION__FIRE_FLICKER__ID,                  
      EFFECTS_FUNCTION__FIREWORKS_EXPLODING_NO_LAUNCH__ID,     

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
      EFFECTS_FUNCTION__DANCING_SHADOWS__ID,
      EFFECTS_FUNCTION__WASHING_MACHINE__ID,
      EFFECTS_FUNCTION__BLENDS__ID,
      EFFECTS_FUNCTION__TV_SIMULATOR__ID,
      EFFECTS_FUNCTION__BOUNCINGBALLS__ID,
      EFFECTS_FUNCTION__SINELON__ID,
      EFFECTS_FUNCTION__SINELON_DUAL__ID,
      EFFECTS_FUNCTION__SINELON_RAINBOW__ID,
      EFFECTS_FUNCTION__DRIP__ID,
      EFFECTS_FUNCTION__FLOWSTRIPE__ID, 
      EFFECTS_FUNCTION__WAVESINS__ID, 

      #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

      /**
       * Hardware Installation Helpers
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
      EFFECTS_FUNCTION__HARDWARE__SHOW_BUS__ID,
      EFFECTS_FUNCTION__HARDWARE__MANUAL_PIXEL_COUNTING__ID,
      EFFECTS_FUNCTION__HARDWARE__VIEW_PIXEL_RANGE__ID,
      EFFECTS_FUNCTION__HARDWARE__LIGHT_SENSOR_PIXEL_INDEXING__ID,
      #endif

      /**
       * Sun Position
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
      EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_SUNSET_BLENDED_PALETTES_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__ID,
      EFFECTS_FUNCTION__SUNPOSITIONS_WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__ID,
      #endif

      /**
       * LED Segment Clock
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
      EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
      EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
      EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,
      EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID,
      #endif
      
      /**
       * Notifications
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
      EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID,
      EFFECTS_FUNCTION__NOTIFICATION_FADE__ID,
      EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
      EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,
      #endif

      /**
       * Border/Frame/Edge Wallpapers
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
      EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__ID,
      EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__ID,
      EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__ID,
      #endif

      /**
       * Manual Pixel: Keeping as legacy, but mode change to realtime will remove this
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE
      EFFECTS_FUNCTION__MANUAL__PIXEL_SET_ELSEWHERE__ID,
      #endif

      /**
       * Christmas Multifunction Controller: Replication of vintage 8 function controllers
       * Replicate how traditional 2/4 ouput controllers and their effects look
          - Option1: Make the same LEDs maintain colour, as if they are painted glass bulbs. So a sequence of 4 RGBO would mean every 1 pixel in order would be turned off
          - Option2: Replicate the same effects, but keep as many LEDs on as possible.
          - These should be an optional flag
          - Another optional flag should be "instant" vs "filament" when turning on/off, ie keep the added fade on/off of traditional lights by keeping a 0.75 second blending
          - Another flag will be to limit palette up to only 5 outputs, like the best controllers out there, otherwise extend effect into full length.
        Palette is drawn "inorder" and animations are drawn with X outputs (4 or 5 like normal lights) to create the effect like real lights
          1 - Combination (will use param to call the others internally)
          2 - In Waves
          3 - Sequentials
          4 - Slo-Glo
          5 - Chasing / Flash
          6 - Slow Fade
          7 - Twinkle / Flash
          8 - Steady on
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__COMBINATION_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__IN_WAVES_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SEQUENTIAL_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLO_GLO_ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__CHASING_AND_FLASHING__ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__SLOW_FADE__ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__TWINKLE_AND_FLASH__ID,
      EFFECTS_FUNCTION__CHRISTMAS_MULTIFUNCTION_CONTROLLER__STEADY_ON__ID,
      #endif

      /**
       * 2D (No Audio)
       **/
      #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
      EFFECTS_FUNCTION__2D__BLACK_HOLE__ID,      
      EFFECTS_FUNCTION__2D__HIPHOTIC__ID,              
      EFFECTS_FUNCTION__2D__COLOURED_BURSTS__ID,         
      EFFECTS_FUNCTION__2D__DNA__ID,        
      EFFECTS_FUNCTION__2D__DNA_SPIRAL__ID,              
      EFFECTS_FUNCTION__2D__DRIFT__ID,            
      EFFECTS_FUNCTION__2D__FIRE_NOISE__ID,   
      EFFECTS_FUNCTION__2D__FRIZZLES__ID,    
      EFFECTS_FUNCTION__2D__GAME_OF_LIFE__ID, 
      EFFECTS_FUNCTION__2D__HIPNOTIC__ID,      
      EFFECTS_FUNCTION__2D__JULIA__ID,       
      EFFECTS_FUNCTION__2D__LISSAJOUS__ID,   
      EFFECTS_FUNCTION__2D__MATRIX__ID,      
      EFFECTS_FUNCTION__2D__METABALLS__ID,    
      EFFECTS_FUNCTION__2D__NOISE__ID,        
      EFFECTS_FUNCTION__2D__PLASMA_BALL__ID,   
      EFFECTS_FUNCTION__2D__POLAR_LIGHTS__ID,  
      EFFECTS_FUNCTION__2D__PULSER__ID,           
      EFFECTS_FUNCTION__2D__SIN_DOTS__ID,      
      EFFECTS_FUNCTION__2D__SQUARED_SWIRL__ID,   
      EFFECTS_FUNCTION__2D__SUN_RADIATION__ID,    
      EFFECTS_FUNCTION__2D__TARTAN__ID,          
      EFFECTS_FUNCTION__2D__PLASMAROTOZOOM__ID,   
      EFFECTS_FUNCTION__2D__SPACESHIPS__ID,        
      EFFECTS_FUNCTION__2D__CRAZY_BEES__ID,       
      EFFECTS_FUNCTION__2D__GHOST_RIDER__ID,            
      EFFECTS_FUNCTION__2D__FLOATING_BLOBS__ID,    
      EFFECTS_FUNCTION__2D__DRIFT_ROSE__ID,        
      EFFECTS_FUNCTION__2D__DISTORTION_WAVES__ID,       
      EFFECTS_FUNCTION__2D__SOAP__ID,   
      EFFECTS_FUNCTION__2D__OCTOPUS__ID,    
      EFFECTS_FUNCTION__2D__WAVING_CELL__ID,  
      EFFECTS_FUNCTION__2D__SCROLLING_TEXT__ID,         
      EFFECTS_FUNCTION__2D__DIGITAL_CLOCK__ID,    
      #endif


      /**
       * Audio Reactive 1D
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_RIPPLE_PEAK__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PERLINE_MOVE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_AURORA__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_CENTER__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_CENTRIC__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAVI_METER__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_JUGGLES__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_MATRIPIX__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_MID_NOISE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_FIRE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_METER__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PIXEL_WAVE__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PLASMOID__ID, 
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PUDDLE_PEAK__ID, 
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PUDDLES__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_PIXELS__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_BLURZ__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_ROCKTAVES__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_DJ_LIGHT__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_MAP__ID, 
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_MATRIX__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_PIXELS__ID,
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_FREQ_WAVE__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_GRAV_FREQ__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_NOISE_MOVE__ID,  
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_ROCK_TAVES__ID,        
      EFFECTS_FUNCTION__AUDIOREACTIVE__1D__FFT_WATERFALL__ID,   
      #endif

      /**
       * Christmas Musical
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
      EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01_ID,
      #endif

      /**
       * Audio Reactive 2D
       **/
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__SWIRL__ID,         
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__WAVERLY__ID,   
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_GED__ID,         
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_FUNKY_PLANK__ID,         
      EFFECTS_FUNCTION__AUDIOREACTIVE__2D__FFT_AKEMI__ID,   
      #endif
      EFFECTS_FUNCTION__LENGTH__ID
    };



    

    /**
     * @brief Turn random Lights off, but when they are off (and hence shunted from in series) the rest of the LEDs in that region should get brighter
     * Note: Part of why these look good is the inherent initial brightness from a cold incandenct bulb turning on, then stabilising. 
     * This will also be the case for the other traditional christmas effects... not sure how to programmatically replicate this
     * Option1: Randomly turn LEDs off, entire string gets marginally brighter
     * Option2: Divide all LED string into sections of 8 (ie. 24V filament sets) and randomly turn 1 led off in each section, and that section brightness changes
     * 
     */


    // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    // /**
    //  * @brief  linear palette group colour, changing by triggered and over a period of time (eg an alarm -30 minutes)
    //  **/
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01__ID,
    // /**
    //  * @brief  Azimuth min/max selects 0-255 for index to get from palette (so map(az, az_min, az_max, 0, 255) which is 0-colours_in_palette)
    //  **/
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__ID,
    // /**
    //  * @brief Daytime: Palette 1
    //  *       Nightime: Palette 2
    //  *        Option1: 0 means it considers the full daily movement as transition, otherwise it means 0.1-degree of movement is considered transition around horizon (ie. +- 5.5 degrees would be 55)
    //  **/
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_SUNSET_BLENDED_PALETTES_01__ID,
    // /**
    //  * @brief  Draw Sun will either draw single pixel, or bloom gradient (ie 1 pixel to 5 pixels blended) on a 1D LED strip
    //  * Option1: for background to be drawn (ie. sky)
    //  * Option2: background sky colour may be time of day reactive and change colour (ie. blue during day, black at night)
    //  **/
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_ELEVATION_01__ID,
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_1D_AZIMUTH_01__ID,    
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__ID, // 2D LED matrix, elevation and azimuth
    // /**
    //  * @brief  As the traditional solid white light, but reactive to the time of day (lightness outside)
    //  *        I will also want a way to pick what these max/min are (ie setting dusk/dawn as limits for transition for CCT colours etc)
    //  **/
    // OLD_EFFECTS_FUNCTION__SUNPOSITIONS_WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__ID, // Daywhite in daylight, and warm white at night
    // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

  // NEw effect idea, lava lamp, both 1D and matrix. Have lgihting "gravity" then also "heating" for uplifting of random colours.

  
  #define WLED_GROUP_IDS_FIRST  EFFECTS_FUNCTION__PALETTE_LIT_PATTERN__ID
  #define WLED_GROUP_IDS_LAST   EFFECTS_FUNCTION__DRIP__ID


  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  #define DEFAULT_EFFECTS_FUNCTION    EFFECTS_FUNCTION__STATIC_PALETTE__ID
  #else
  #define DEFAULT_EFFECTS_FUNCTION    0
  #endif

  uint16_t getEffectsAmount(){ return effects.config.size(); }

  void SubTask_Effects();
  void Segments_RefreshLEDIndexPattern(uint8_t segment_index = 0);
  
  byte realtimeMode = REALTIME_MODE_INACTIVE;
  bool realtimeRespectLedMaps; // used in getMappedPixelIndex()

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // uint16_t fForceUpdate : 1; // now using trigger, any update causes them all to update
      uint16_t fRunning : 1;
      uint16_t animator_first_run : 1;
      // Reserved
      uint16_t reserved : 12;
    };
  } ANIMATION_FLAGS;



  enum ColourType{ 
      // COLOUR_TYPE__NONE__ID=0, 
      COLOUR_TYPE__SINGLE__ID=1, // likely never used for me, remove
      COLOUR_TYPE__COLDWARM__ID=2,  //CCT Only
      COLOUR_TYPE__RGB__ID=3, //3   
      COLOUR_TYPE__RGBW__ID=4, //4
      COLOUR_TYPE__RGBWW__ID=5 // 5 
      
      // Previous methods that remember colour order, probably not needed or at least cct assume default of RGBWC
      // COLOUR_TYPE__RGBWC__ID, //remove
      // COLOUR_TYPE__RGBCW__ID //remove
  };

  inline uint8_t GetSizeOfPixel(ColourType colour_type)
  {
    switch(colour_type)
    {
      default:
      case ColourType::COLOUR_TYPE__RGB__ID:     return 3;
      case ColourType::COLOUR_TYPE__RGBW__ID:    return 4;
      case ColourType::COLOUR_TYPE__RGBWW__ID:  return 5;
    }
  }


  mAnimatorLight& SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE);
  void StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index = 0);
  void Segments_SetPixelColor_To_Static_Pallete(uint16_t palette_id);
  void Segments_UpdateDesiredColourFromPaletteSelected(uint16_t segment_index = 0);
  void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param);





  // #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
  // IRAM_ATTR 
  // #endif 
  // void AnimationProcess_LinearBlend_Dynamic_Buffer(const AnimationParam& param);


  // #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
  // IRAM_ATTR 
  // #endif 
  // void AnimationProcess_LinearBlend_Dynamic_Buffer_BrtNotSet(const AnimationParam& param);



  // void AnimationProcess_SingleColour_LinearBlend_Between_RgbcctSegColours(const AnimationParam& param);

  // void 
  // #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
  // IRAM_ATTR 
  // #endif 
  // SetTransitionColourBuffer_StartingColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, ColourType pixel_type, const RgbwwColor& starting_colour);
  
  // void 
  // #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
  // IRAM_ATTR 
  // #endif 
  // SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index,  ColourType pixel_type, const RgbwwColor& desired_colour);

  // void 
  // #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
  // IRAM_ATTR 
  // #endif 
  // DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();

inline RgbwwColor AdjustColourWithBrightness(const RgbwwColor& colour, uint8_t brightnessRGB) {
    // Pre-calculate the scale factor (bit-shift scaling for division by 255)
    uint16_t scale = brightnessRGB + 1; // Adding 1 to avoid division by zero and maintain full range

    // Apply brightness scaling
    RgbwwColor adjusted;
    adjusted.R = (colour.R * scale) >> 8;
    adjusted.G = (colour.G * scale) >> 8;
    adjusted.B = (colour.B * scale) >> 8;
    adjusted.WW = (colour.WW * scale) >> 8;
    adjusted.CW = (colour.CW * scale) >> 8;

    return adjusted;
}
inline uint32_t AdjustColourWithBrightness(uint32_t colour32, uint8_t brightnessRGB) {
    // Pre-calculate the scale factor (bit-shift scaling for division by 255)
    uint16_t scale = brightnessRGB + 1; // Adding 1 to avoid division by zero and maintain full range

    // Extract individual color components
    uint8_t r = (colour32 >> 16) & 0xFF; // Red
    uint8_t g = (colour32 >> 8) & 0xFF;  // Green
    uint8_t b = colour32 & 0xFF;         // Blue
    uint8_t w = (colour32 >> 24) & 0xFF; // White (if applicable)

    // Apply brightness scaling
    r = (r * scale) >> 8;
    g = (g * scale) >> 8;
    b = (b * scale) >> 8;
    w = (w * scale) >> 8;

    // Recombine components into a single uint32_t color
    return (w << 24) | (r << 16) | (g << 8) | b;
}

inline static uint32_t FadeU32(uint32_t colour32, uint8_t fade) {
    // Extract individual color components
    uint8_t r = (colour32 >> 16) & 0xFF; // Red
    uint8_t g = (colour32 >> 8) & 0xFF;  // Green
    uint8_t b = colour32 & 0xFF;         // Blue
    uint8_t w = (colour32 >> 24) & 0xFF; // White (if applicable)

    // Apply fade
    r = (r > fade) ? (r / fade) : 0;
    g = (g > fade) ? (g / fade) : 0;
    b = (b > fade) ? (b / fade) : 0;
    w = (w > fade) ? (w / fade) : 0;

    // Recombine components into a single uint32_t color
    return (w << 24) | (r << 16) | (g << 8) | b;
}


  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING
  struct TimeCriticalLogging{
    uint32_t start_value[7]; // use as temp to record start of measurment, use here for speed

    uint32_t dynamic_buffer__starting_colour;
    uint32_t dynamic_buffer__starting_colour_part1; // 5
    uint32_t dynamic_buffer__starting_colour_part2; // 6

    uint32_t dynamic_buffer__desired_colour;
    uint32_t effect_call;
    uint32_t segment_effects;

    bool time_unit_output_ms = false;
  }lighting_time_critical_logging;
  #endif // ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING


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
  static um_data_t* getAudioData();
      
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
          RgbwwColor colour;
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

    RgbwwColor IRAM_ATTR
    GetColourFromUnloadedPalette3(
      uint16_t palette_id,
      uint16_t desired_index_from_palette = 0,
      bool     flag_spanned_segment = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap_hard_edge = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      bool     flag_crgb_exact_colour = false,
      uint8_t* encoded_index = nullptr,
      bool flag_request_is_for_full_visual_output = false
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
    
    inline uint16_t crc16(const unsigned char* data_p, size_t length) {
      uint8_t x;
      uint16_t crc = 0xFFFF;
      if (!length) return 0x1D0F;
      while (length--) {
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
      }
      return crc;
    }

    // fastled beatsin: 1:1 replacements to remove the use of fastled sin16()
    // Generates a 16-bit sine wave at a given BPM that oscillates within a given range. see fastled for details.
    inline uint16_t beatsin88_t(accum88 beats_per_minute_88, uint16_t lowest = 0, uint16_t highest = 65535, uint32_t timebase = 0, uint16_t phase_offset = 0)
    {
        uint16_t beat = beat88( beats_per_minute_88, timebase);
        uint16_t beatsin (sin16_t( beat + phase_offset) + 32768);
        uint16_t rangewidth = highest - lowest;
        uint16_t scaledbeat = scale16( beatsin, rangewidth);
        uint16_t result = lowest + scaledbeat;
        return result;
    }

    // Generates a 16-bit sine wave at a given BPM that oscillates within a given range. see fastled for details.
    inline uint16_t beatsin16_t(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, uint32_t timebase = 0, uint16_t phase_offset = 0)
    {
        uint16_t beat = beat16( beats_per_minute, timebase);
        uint16_t beatsin = (sin16_t( beat + phase_offset) + 32768);
        uint16_t rangewidth = highest - lowest;
        uint16_t scaledbeat = scale16( beatsin, rangewidth);
        uint16_t result = lowest + scaledbeat;
        return result;
    }

    // Generates an 8-bit sine wave at a given BPM that oscillates within a given range. see fastled for details.
    inline uint8_t beatsin8_t(accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255, uint32_t timebase = 0, uint8_t phase_offset = 0)
    {
        uint8_t beat = beat8( beats_per_minute, timebase);
        uint8_t beatsin = sin8_t( beat + phase_offset);
        uint8_t rangewidth = highest - lowest;
        uint8_t scaledbeat = scale8( beatsin, rangewidth);
        uint8_t result = lowest + scaledbeat;
        return result;
    }

    // Temporary helper functions to be cleaned up and converted
    uint32_t crgb_to_col(CRGB crgb);
    CRGB col_to_crgb(uint32_t);

    uint8_t get_random_wheel_index(uint8_t pos);
    uint8_t sin_gap(uint16_t in);
    uint16_t triwave16(uint16_t in);
    int8_t tristate_square8(uint8_t x, uint8_t pulsewidth, uint8_t attdec);
    
    // void colorFromUint32(uint32_t in, bool secondary = false);
    // void colorFromUint24(uint32_t in, bool secondary = false);
    // void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
    // void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
    // void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb
    // void colorFromDecOrHexString(byte* rgb, char* in);
    // void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)



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

  void colorUpdated(byte callMode);
  void stateUpdated(byte callMode);
  void updateInterfaces(uint8_t callMode);

  void handleTransitions();

  
  float minf2(float v, float w);
  float maxf2(float v, float w);

  void Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index = 0);

  void SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i = 0);

  int16_t extractModeDefaults(uint8_t mode, const char *segVar);
  bool extractModeDefaults(uint8_t mode, const char *segVar, char *outBuffer, size_t bufferSize);

    void Reset_CustomPalette_NamesDefault();

  /**
   * @brief 
   *
   * 
  * Segment, 72 bytes
  **/

  // #include "6_Lights/03_Animator/SubHeader_Segments.h" // Include the segment header file section, doing this to keep it easier to see
// #pragma     once

/**
 * @brief Stores at least 5 full RgbcctColours with all internal manipulations as needed
 * @NOTE: Replaces WLED 3 colour options
 **/
#ifndef CCT_MIN_DEFAULT
  #define CCT_MIN_DEFAULT 153          // 6500K
#endif
#ifndef CCT_MAX_DEFAULT
  #define CCT_MAX_DEFAULT 500          // 2000K
#endif
struct SegmentColour {
    RgbwwColor colour; // The RgbwwColor object
    uint8_t bri_rgb;   // Brightness for RGB channels
    uint8_t bri_ww;    // Brightness for WW and CW channels

    uint16_t cct = CCT_MIN_DEFAULT;             // 153..500, default to 153 (cold white)
    uint16_t cct_min_range = CCT_MIN_DEFAULT;   // the minimum CT rendered range
    uint16_t cct_max_range = CCT_MAX_DEFAULT;   // the maximum CT rendered range

    // Constructor for easy initialization
    SegmentColour(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t ww = 0, uint8_t cw = 0, uint8_t br_rgb = 255, uint8_t br_ww = 255)
        : colour(r, g, b, ww, cw), bri_rgb(br_rgb), bri_ww(br_ww) {}
    
    // Set RGB brightness (0-255)
void setBrightnessRGB(uint8_t brightness) {
    bri_rgb = brightness;
}

// Get RGB brightness (0-255)
uint8_t getBrightnessRGB() const {
    return bri_rgb;
}

// Set WW brightness (0-255)
void setBrightnessWW(uint8_t brightness) {
    bri_ww = brightness;
}

// Get WW brightness (0-255)
uint8_t getBrightnessWW() const {
    return bri_ww;
}
// TMP FUCNTIONS TO REMOVE
// Set WW brightness (0-255)
void setBrightnessCCT(uint8_t brightness) {
    bri_ww = brightness;
}
// Get WW brightness (0-255)
uint8_t getBrightnessCCT() const {
    return bri_ww;
}


    // Apply brightness levels to the color and return a new RgbwwColor object
    RgbwwColor WithBrightness() const {
        RgbwwColor adjusted = colour;

        // Scale RGB values by bri_rgb
        uint16_t scaleRGB = bri_rgb + 1; // Prevent division by zero
        adjusted.R = (colour.R * scaleRGB) >> 8;
        adjusted.G = (colour.G * scaleRGB) >> 8;
        adjusted.B = (colour.B * scaleRGB) >> 8;

        // Scale WW and CW values by bri_ww
        uint16_t scaleWW = bri_ww + 1;
        adjusted.WW = (colour.WW * scaleWW) >> 8;
        adjusted.CW = (colour.CW * scaleWW) >> 8;

        return adjusted;
    }

    // Convert the current color with brightness applied to a 32-bit integer
    uint32_t getU32() const {
        RgbwwColor adjusted = WithBrightness();
        return (static_cast<uint32_t>(adjusted.WW) << 24) | 
            (static_cast<uint32_t>(adjusted.R) << 16) | 
            (static_cast<uint32_t>(adjusted.G) << 8) | 
            static_cast<uint32_t>(adjusted.B);
    }

    // Set CCT in Kelvin
    void setCCT_Kelvin(uint16_t _cct) {
      Serial.println("I dont want this right now");
        // Clamp CCT to valid range
        cct = (_cct < cct_min_range) ? cct_min_range : (_cct > cct_max_range ? cct_max_range : _cct);

        // Calculate Warm White and Cool White proportions
        uint8_t ww_value = ((cct - cct_min_range) * 255) / (cct_max_range - cct_min_range);
        uint8_t cw_value = 255 - ww_value;

        // Apply brightness scaling for whites
        uint16_t scaleWW = bri_ww + 1;
        colour.WW = (ww_value * scaleWW) >> 8;
        colour.CW = (cw_value * scaleWW) >> 8;

        // Store the current CCT
        _cct = cct;
    }// Set CCT as a balance between WW and CW (0-255)


    void setCCT(uint8_t cct_balance) {
        // Directly set the CCT balance
        cct = cct_balance;

    //         Cooler (bluish): cct = 255 should result in more CW and less WW.
    // Warmer (yellowish): cct = 0 should result in more WW and less CW.

        // Calculate Warm White and Cool White proportions
        colour.CW = cct;            // Warm White proportion
        colour.WW = 255 - cct;      // Cool White proportion
    }


    // Get the current CCT in Kelvin
    uint16_t getCCT_Kelvin() const {
        return cct;
    }
    // Get the current CCT in Kelvin
    uint16_t getCCT() const {
        return cct;
    }

    // Set Hue (0-360 degrees)
void setHue(uint16_t hue) {
    // Normalize hue to 0-360 range
    hue = hue % 360;

    // Convert current RGB to HSB
    HsbColor hsb = HsbColor(RgbColor(colour.R, colour.G, colour.B));
    hsb.H = static_cast<float>(hue) / 360.0f;

    // Update RGB components based on the new hue
    RgbColor newRgb = RgbColor(hsb);
    colour.R = newRgb.R;
    colour.G = newRgb.G;
    colour.B = newRgb.B;
}

// Get Hue (0-360 degrees)
uint16_t getHue() const {
    // Convert current RGB to HSB
    HsbColor hsb = HsbColor(RgbColor(colour.R, colour.G, colour.B));
    return static_cast<uint16_t>(hsb.H * 360.0f);
}

// Set Saturation (0-255)
void setSaturation(uint8_t saturation) {
    // Normalize saturation to 0-1 range
    float sat = static_cast<float>(saturation) / 255.0f;

    // Convert current RGB to HSB
    HsbColor hsb = HsbColor(RgbColor(colour.R, colour.G, colour.B));
    hsb.S = sat;

    // Update RGB components based on the new saturation
    RgbColor newRgb = RgbColor(hsb);
    colour.R = newRgb.R;
    colour.G = newRgb.G;
    colour.B = newRgb.B;
}

// Get Saturation (0-255)
uint8_t getSaturation() const {
    // Convert current RGB to HSB
    HsbColor hsb = HsbColor(RgbColor(colour.R, colour.G, colour.B));
    return static_cast<uint8_t>(hsb.S * 255.0f);
}
    
};


typedef struct Segment 
{
  public:
    uint16_t start = 0; // start means first led index within segment : start index / start X coordinate 2D (left)
    uint16_t stop  = 0; // stop means total leds within the segment (not the index of last pixel) : stop index / stop X coordinate 2D (right); segment is invalid if stop == 0
    uint16_t offset = 0;

    uint8_t  speed               = DEFAULT_SPEED; // in my effect, may be the new time_ms
    uint8_t  intensity           = 127;           // should not relate to blending, only what the effect is doing
    uint16_t cycle_time__rate_ms = 2000; // EffectTimePeriod ie EffectCycleTime

    /**
     * @brief To enable Rgbcct to U32 conversions 
     */
    bool flag_brightness_set_in_effect = false;
    
    // Speed = 0 means blending is slowest and full cycle time is spent blending
    // Speed = 255 means blending is instant
    uint16_t animator_blend_time_ms(){ return (cycle_time__rate_ms * (255-speed))/255; } // time_ms
    

    uint16_t palette_id = 0; 
    uint8_t effect_id = 0;    
    uint8_t effect_id_next = 0;   //e.g. For rotating effect, preload the initial animation and then rotate it/

    /**
     * @brief Note with union here not having a name, all options are accesible directly in Segment
     **/
    union {
      uint16_t options; //bit pattern: msb first: [transposed mirrorY reverseY] transitional (tbd) paused needspixelstate mirrored on reverse selected
      struct {
        bool    selected    : 1;  //     0 : selected
        bool    reverse     : 1;  //     1 : reversed
        bool    on          : 1;  //     2 : is On
        bool    mirror      : 1;  //     3 : mirrored
        bool    freeze      : 1;  //     4 : paused/frozen
        bool    reset       : 1;  //     5 : indicates that Segment runtime requires reset
        bool    transitional: 1;  //     6 : transitional (there is transition occuring)
        bool    reverse_y   : 1;  //     7 : reversed Y (2D)
        bool    mirror_y    : 1;  //     8 : mirrored Y (2D)
        bool    transpose   : 1;  //     9 : transposed (2D, swapped X & Y)
        uint8_t map1D2D     : 3;  // 10-12 : mapping for 1D effect on 2D (0-use as strip, 1-expand vertically, 2-circular/arc, 3-rectangular/corner, ...)
        uint8_t soundSim    : 3;  // 13-15 : 0-7 sound simulation types
      };
    };

    /**
     * @brief New setting to reduce pixels generated for speed, and will replicate the data out to proper seglen
     * For example, with 1000 led segment, if the same pattern is generated every 10 leds, then only 100 leds will be generated and pushed out 10 times on the bus
     * 
     * Decimate is a multiplier, so 0 means none, with each value being a divisor. Decimate 10 would be only generate 10% of the leds, then repeat it 10 times on the output
     */
    uint8_t decimate = 0;
    uint8_t grouping = 1;
    uint8_t  spacing = 0;

    inline void grouping_set(uint8_t g){ grouping = g > 0 ? g : 1; } // Can never be ZERO
    inline uint8_t grouping_get(void){ return grouping; } // Can never be ZERO

    /**
     * @brief
     * The colours that are generated directly within the effect function, and is used to define the type of storage buffered required to hold the colour buffer
     * This should work in tandem with another variable, which can define how the white components are handled
     * Option 1: Only RGB is created, white is ignored
     * Option 2: Effect creates colour and white components
     * Option 3: Effects creates colours, but white is generated after based on different options
     * 
     * While it describes RGB, WRGB, RGBWW, WWA etc, in reality, it is treated as the byte width for buffers directly. 
     **/
    #ifdef ENABLE_DEVFEATURE_LIGHT__FORCE_EFFECT_COLOUR_TYPE_AS_RGBCCT
    ColourType colour_width__used_in_effect_generate = ColourType::COLOUR_TYPE__RGBWW__ID; 
    #else
    ColourType colour_width__used_in_effect_generate = ColourType::COLOUR_TYPE__RGB__ID; 
    #endif

    // Define the size of the color array
    #define RGBCCTCOLOURS_SIZE 5

    // Initialize the array with default values
    SegmentColour segcol[RGBCCTCOLOURS_SIZE] = 
    {
      SegmentColour(255, 0, 0, 0, 0), // Red
      SegmentColour(0, 255, 0, 0, 0), // Green
      SegmentColour(0, 0, 255, 0, 0), // Blue
      SegmentColour(255, 0, 255, 0, 0), // Magenta
      SegmentColour(255, 255, 0, 0, 0)  // Yellow
    };


    // #define RGBCCTCOLOURS_SIZE 5
    // RgbwwColor segcol[5] = {RgbwwColor(255,0,0,0,0), RgbwwColor(0,255,0,0,0), RgbwwColor(0,0,255,0,0), RgbwwColor(255,0,255,0,0), RgbwwColor(255,255,0,0,0)};

    // void set_colors(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w )
    // {
    //   uint8_t index_t = index<RGBCCTCOLOURS_SIZE?index:0;
    //   if(index>RGBCCTCOLOURS_SIZE){ Serial.println("ERROR"); }
    //   segcol[index_t] = RgbwwColor(r,g,b,w,w);
    // }

    void set_colors(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t ww, uint8_t cw, uint8_t br_rgb = 255, uint8_t br_ww = 255) {
        if (index >= RGBCCTCOLOURS_SIZE) {
            Serial.println("ERROR: Index out of bounds");
            return;
        }

        segcol[index] = SegmentColour(r, g, b, ww, cw, br_rgb, br_ww);
    }



    ANIMATION_FLAGS flags;
    

    // Effects (Scenes & Flasher), Ambilight, Adalight    
    uint8_t animation_mode_id = 0; // rename to "effect_id"

    
    uint16_t get_transition_rate_ms() // Effect that require call for every update, must be called at FRAMETIME_MS, otherwise, can manually be set
    {
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
      if(effect_id >= WLED_GROUP_IDS_FIRST)
      {
        return FRAMETIME;
      }
      else
        return cycle_time__rate_ms;
      #endif
        return cycle_time__rate_ms;
    };

    
    /**
     * This allows the segment to have optional brightness per segment, by default is set to maximum
     * and will therefore have no effect 
     **/
    uint8_t _brightness_rgb = 255;
    uint8_t _brightness_cct = 255;

    #define _segBri _brightness_rgb //wled fix

    uint8_t _brightness_rgb_combined = 255;
    uint8_t _brightness_cct_combined = 255;
    IRAM_ATTR void UpdateBrightness();

    IRAM_ATTR void setBrightnessRGB(uint8_t bri_rgb)
    {
      _brightness_rgb = bri_rgb;
    }

    IRAM_ATTR void setBrightnessCCT(uint8_t bri_cct)
    {
      _brightness_cct = bri_cct;
    }

    IRAM_ATTR uint8_t getBrightnessRGB() const
    {
      return _brightness_rgb;
    };

    IRAM_ATTR uint8_t getBrightnessCCT() const
    {
      return _brightness_cct;
    };

    IRAM_ATTR inline uint8_t getBrightnessRGB_WithGlobalApplied() const
    {
      return _brightness_rgb_combined;
    }

    IRAM_ATTR inline uint8_t getBrightnessCCT_WithGlobalApplied() const
    {
      return _brightness_cct_combined;
    }

    #define currentBri() getBrightnessRGB()


    // Flags and states that are used during one transition and reset when completed
    struct ANIMATION_SINGLE_USE_OVERRIDES_ANYTIME
    {
      // uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 1000; //on boot
      // uint16_t cycle_time__rate_ms = 1000;
    }single_animation_override; // ie "oneshot" variables that get checked and executed one time only

    /**
     * @brief These should be moved elsewhere as the optional defaults for times, that are loaded into the above override when needed
     * 
     */
    struct ANIMATION_SINGLE_USE_OVERRIDES_TURNING_OFF
    {
      // uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 0; //on boot
      // uint16_t cycle_time__rate_ms = 1000;
    }single_animation_override_turning_off; // ie "oneshot" variables that get checked and executed one time only

    

    uint16_t groupLength() { return grouping + spacing; }
    uint16_t virtualLength()
    {
      uint16_t groupLen = groupLength();
      uint16_t vLength = (length() + groupLen - 1) / groupLen;
      if(mirror) vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED

      #ifdef ENABLE_DEVFEATURE_LIGHTS__DECIMATE
      if(decimate > 0)
      {
        vLength = (vLength + decimate - 1) / decimate;
      }
      #endif // ENABLE_DEVFEATURE_LIGHTS__DECIMATE

      return vLength;
    }
    #define vLength() virtualLength() //tmp fix    
    // inline static unsigned vWidth()                        { return virtualLength(); }

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    struct PERFORMANCE{

      uint32_t effect_build_us = 0;
      uint32_t bus_write_single_us = 0; // SetPixelColour to NPB (ignore decimate, includes initial write then times decimate amount)
      uint32_t bus_read_single_us = 0;  // GetPixelColour from NPB (ignores decimate, includes full time)
      uint32_t bus_write_total_us = 0; // SetPixelColour to NPB (ignore decimate, includes initial write then times decimate amount)
      uint32_t bus_read_total_us = 0;  // GetPixelColour from NPB (ignores decimate, includes full time)

      uint32_t millis_last_show = 0;
      uint32_t elapsed_last_show = 0;
      float fps = 0;

    }performance;
    #endif // ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE

    void setUp(uint16_t i1, uint16_t i2, uint8_t grp=1, uint8_t spc=0, uint16_t ofs=UINT16_MAX, uint16_t i1Y=0, uint16_t i2Y=1);
    
    uint8_t  cct_slider;                 //0==1900K, 255==10091K
    uint8_t  custom1, custom2;    // custom FX parameters/sliders
    struct {
      uint8_t custom3 : 5;        // reduced range slider (0-31)
      bool    check1  : 1;        // checkmark 1
      bool    check2  : 1;        // checkmark 2
      bool    check3  : 1;        // checkmark 3
    };
    
    uint8_t startY;  // start Y coodrinate 2D (top); there should be no more than 255 rows
    uint8_t stopY;   // stop Y coordinate 2D (bottom); there should be no more than 255 rows
    char *name = nullptr; // Keep, segment name to be added later by me

    // runtime data
    unsigned long next_time;  // millis() of next update
    uint32_t tSaved_EffectStartReferenceTime = 0;
    uint32_t step;  // custom "step" var
    uint32_t call;  // call counter

    uint32_t tSaved_AnimateRunTime = 0; //tmp fix ofr xmas24 then remove

    uint8_t effect_anim_section = 0; // 0 draw, 1 stop draw

    bool     _colorScaled;             // color has been scaled prior to setPixelColor() call
    bool          _modeBlend = true;          // mode/effect blending semaphore

    /**
     * @brief AUX options going forward must only be internal effect save states, and NOT user defined options (these should be "effect_option")
     * Internal multi-use variables
     * Keep at least one U32 for millis storage
     */
    // struct INTERNAL_MULTIUSE_PARAMETERS
    // {
      uint16_t aux0 = 0;  // custom var
      uint16_t aux1 = 0;  // custom var
      uint16_t aux2 = 0;
      uint32_t aux3 = 0; // Also used for random CRGBPALETTE16 timing
    // }params_internal;

    Decounter<uint16_t> auto_timeoff = Decounter<uint16_t>();

    /**
     * @brief 
     * External user controllable multi-use variables
     */
    uint16_t params_user[4] = {0};

    // CRGB* leds;     // local leds[] array (may be a pointer to global)
    // static CRGB *_globalLeds;             // global leds[] array
    static uint16_t maxWidth, maxHeight;  // these define matrix width & height (max. segment dimensions)
    
  // private:
    union {
      uint8_t  _capabilities;
      struct {
        bool    _isRGB    : 1;
        bool    _hasW     : 1;
        bool    _isCCT    : 1;
        bool    _manualW  : 1;
        uint8_t _reserved : 4;
      };
    };

    /***
     * Effect datastorage
     ***/
    byte* data;     // effect data pointer
    uint16_t _dataLen;
    static uint16_t _usedSegmentData;
    // inline byte* Data(){ return data; };
    // inline uint16_t DataLength(){ return _dataLen; };
    /***
     * dynamic colour byte buffer
     ***/
    byte* coldata;     // buffer to be used when leds needed stored in out dynamic colour methods
    uint16_t _coldataLen;
    inline byte* ColourData(){ return coldata; };
    inline uint16_t ColourDataLength(){ return _coldataLen; };





    mPaletteLoaded* palette_container = new mPaletteLoaded();
    /**
     * Each segment will have its own animator
     * This will also need to share its index into the animation so it knows what segments to run
     * */
    NeoPixelAnimator* animator = new NeoPixelAnimator(1, NEO_MILLISECONDS); //one animator for each segment, which is only init when needed or else delete


    bool LoadPalette_AsyncLock = false;
    void LoadPalette(uint8_t palette_id, mPaletteLoaded* palette_container = nullptr);

    uint32_t tSaved_LastUpdated = millis();
    uint32_t tTick_maximum_call_ms = 10;

    /**
     * Using "index" inside animator as segment index
     * */
    ANIM_FUNCTION_SIGNATURE;
    bool animation_has_anim_callback = false; //should be dafult on start but causing no animation on start right now

    Segment(uint16_t sStart=0, uint16_t sStop=30) :
      start(sStart),
      stop(sStop),
      offset(0),
      speed(DEFAULT_SPEED),
      intensity(DEFAULT_INTENSITY),
      effect_id(DEFAULT_MODE),
      options(SELECTED | SEGMENT_ON),
      grouping(1),
      spacing(0),
      cct_slider(127),
      custom1(DEFAULT_C1),
      custom2(DEFAULT_C2),
      custom3(DEFAULT_C3),
      check1(false),
      check2(false),
      check3(false),
      startY(0),
      stopY(1),
      name(nullptr),
      next_time(0),
      step(0),
      call(0),
      data(nullptr),
      coldata(nullptr),
      _capabilities(0),
      _dataLen(0),
      _coldataLen(0)
    {

      #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
      Serial.printf("-- Creating segment: %p\n", this);
      #endif
      
      refreshLightCapabilities();

      aux0 = 0;
      aux1 = 0;
      aux2 = 0;
      aux3 = 0;

      palette_container = new mPaletteLoaded(); // duplicate of above, but needed for each segment
      
    }

    Segment(uint16_t sStartX, uint16_t sStopX, uint16_t sStartY, uint16_t sStopY) : Segment(sStartX, sStopX) {
      startY = sStartY;
      stopY  = sStopY;
    }

    Segment(const Segment &orig) // copy constructor
    {
      #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
      Serial.println(F("-- Copy segment constructor --"));
      #endif

      memcpy(this, &orig, sizeof(Segment));
      name = nullptr;
      
      if (orig.name){ 
        name = new char[strlen(orig.name)+1]; 
        if (name){
          strcpy(name, orig.name); 
        }
      }

      data = nullptr; _dataLen = 0;
      if (orig.data){ 
        if (allocateData(orig._dataLen)) 
        {
          memcpy(data, orig.data, orig._dataLen); 
        }
      }
      
      coldata = nullptr; _coldataLen = 0;
      if (orig.coldata) {
        if(allocateColourData(orig._coldataLen)){
          memcpy(coldata, orig.coldata, orig._coldataLen);
        }
      }

    };


    Segment(Segment &&orig) noexcept // move constructor
    {

      #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
      Serial.println(F("-- Move segment constructor --"));
      #endif

      memcpy((void*)this, (void*)&orig, sizeof(mAnimatorLight::Segment));

      orig.name = nullptr;
      orig.data = nullptr;      orig._dataLen = 0;
      orig.coldata = nullptr;   orig._coldataLen = 0;

    }


    ~Segment() // deconstructor
    {
      #ifdef ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS
        Serial.print(F("Destroying segment:"));
        if (name) Serial.printf(" %s (%p)", name, name);
        if (data) Serial.printf(" %d (%p)", (int)_dataLen, data);
        Serial.println();
      #endif
      if (name) delete[] name;
      deallocateData();
      deallocateColourData();
    }


    Segment& operator= (const Segment &orig); // copy assignment
    Segment& operator= (Segment &&orig) noexcept; // move assignment

    #ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
    size_t getSize() const 
    { 
      return sizeof(Segment) + (data?_dataLen:0) + 
        (name?strlen(name):0) + 
        (!Segment::_globalLeds && leds?sizeof(CRGB)*length():0); 
    }
    #endif

    inline bool     getOption(uint8_t n) const { return ((options >> n) & 0x01); }
    inline bool     isSelected(void)     const { return selected; }
    inline bool     isActive(void)       const { return stop > start; }
    inline bool     is2D(void)           const { return (width()>1 && height()>1); }
    inline uint16_t width(void)          const { return stop - start; }       // segment width in physical pixels (length if 1D)
    inline uint16_t height(void)         const { return stopY - startY; }     // segment height (if 2D) in physical pixels
    inline uint16_t length(void)         const { return width() * height(); } // segment length (count) in physical pixels
    inline uint16_t groupLength(void)    const { return grouping + spacing; }
    inline uint8_t  getLightCapabilities(void) const { return 0xFF; }// force all default on _capabilities; }

    static uint16_t getUsedSegmentData(void)    { return _usedSegmentData; }
    static void     addUsedSegmentData(int len) { _usedSegmentData += len; }

    bool    setColor(uint8_t slot, uint32_t c); //returns true if changed
    bool    setColor(uint8_t slot, RgbwwColor c); //returns true if changed
    void    setCCT(uint16_t k);
    void    setOption(uint8_t n, bool val);
    void    setMode(uint8_t fx, bool loadDefaults = false);
    void    setPalette(uint8_t pal);
    uint8_t differs(const Segment& b) const;
    void    refreshLightCapabilities(void);

    static uint32_t color_blend(uint32_t,uint32_t,uint16_t,bool b16=false);
    static RgbwwColor color_blend(RgbwwColor,RgbwwColor,uint16_t,bool b16=false);
    static uint32_t color_add(uint32_t,uint32_t, bool fast=false);
    static uint32_t color_add(RgbwwColor,RgbwwColor, bool fast=false);
    static RgbwwColor color_fade(RgbwwColor c1, uint8_t amount, bool video=false);
    static uint32_t color_fade(uint32_t c1, uint8_t amount, bool video=false);

    void setRandomColor(byte* rgb);
    void colorHStoRGB(uint16_t hue, byte sat, byte* rgb);
    void colorKtoRGB(uint16_t kelvin, byte* rgb);    
    void colorCTtoRGB(uint16_t mired, byte* rgb);
    void colorXYtoRGB(float x, float y, byte* rgb);
    void colorRGBtoXY(byte* rgb, float* xy);
    void colorFromDecOrHexString(byte* rgb, char* in);
    bool colorFromHexString(byte* rgb, const char* in);

    // runtime data functions
    inline uint16_t dataSize(void) const { return _dataLen; }
    bool allocateData(size_t len);
    void deallocateData(void);

    bool allocateColourData(uint16_t len);
    void deallocateColourData(void);


    void resetIfRequired(void);
    /** 
      * Flags that before the next effect is calculated,
      * the internal segment state should be reset. 
      * Call resetIfRequired before calling the next effect function.
      * Safe to call from interrupts and network requests.
      */
    inline void markForReset(void) 
    {
      reset = true; 
    }

    // 1D strip
    [[gnu::hot]] uint16_t virtualLength(void) const;

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
    [[gnu::hot]] void setPixelColor(int n, RgbwwColor c);                  // Main function others below call
    void setPixelColor(int n, byte r, byte g, byte b, byte w = 0) 
    { 
      #ifdef ENABLE_DEVFEATURE_LIGHTS__SHOW_HEADER_SETPIXEL_OVERLOADING_CALLS
      Serial.println("setPixelColor(int n, byte r, byte g, byte b, byte w = 0");
      #endif    
      setPixelColor(n, RgbwwColor(r,g,b,w));    
    }
    void setPixelColor(uint16_t n, RgbwwColor c)
    {      
      #ifdef ENABLE_DEVFEATURE_LIGHTS__SHOW_HEADER_SETPIXEL_OVERLOADING_CALLS
      Serial.println("setPixelColor(uint16_t n, RgbwwColor c)");
      #endif    
      setPixelColor((int)n, c); 
    } // explicit conversion
    void setPixelColor(int n, CRGB c) 
    {
      #ifdef ENABLE_DEVFEATURE_LIGHTS__SHOW_HEADER_SETPIXEL_OVERLOADING_CALLS
      Serial.println("setPixelColor(int n, CRGB c)");
      #endif    
      setPixelColor(n, RgbwwColor(c.r, c.g, c.b));
    }
    void setPixelColor(int n, uint32_t c) 
    {
      #ifdef ENABLE_DEVFEATURE_LIGHTS__SHOW_HEADER_SETPIXEL_OVERLOADING_CALLS
      Serial.println("setPixelColor(int n, uint32_t c)");
      #endif    
      setPixelColor(n, RgbwwColor(R(c), G(c), B(c), W(c), W(c)));
    }  // explicit conversion
    void setPixelColor(unsigned n, uint32_t c)
    {
      #ifdef ENABLE_DEVFEATURE_LIGHTS__SHOW_HEADER_SETPIXEL_OVERLOADING_CALLS
      Serial.println("setPixelColor(unsigned n, uint32_t c)");
      #endif    
      setPixelColor((int)n, RgbwwColor(R(c), G(c), B(c), W(c), W(c)));
    }  // explicit conversion
    #ifdef USE_AA_PIXELS
    // Anti-aliasing functions
    void setPixelColor(float i, uint32_t c, bool aa = true);
    void setPixelColor(float i, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0, bool aa = true) { setPixelColor(i, RGBW32(r,g,b,w), aa); }
    void setPixelColor(float i, CRGB c, bool aa = true)                                         { setPixelColor(i, RGBW32(c.r,c.g,c.b,0), aa); }    
    #endif
    #else    
    [[gnu::hot]] void setPixelColor(int n, uint32_t c);
    void setPixelColor(int n, RgbwwColor c){ setPixelColor(n, RGBW32(c.R, c.G, c.B, c.WW)); } 
    void setPixelColor(unsigned n, uint32_t c){ setPixelColor((int)n, c); } // to keep compatibility with RGBWW
    void setPixelColor(uint16_t n, uint32_t c){ setPixelColor((int)n, c); } // to keep compatibility with RGBWW
    void setPixelColor(int n, byte r, byte g, byte b, byte w = 0) {      setPixelColor(n, RGBW32(r,g,b,w));    }
    void setPixelColor(int n, CRGB c) {            setPixelColor(n, RGBW32(c.r, c.g, c.b,0));    }
    #ifdef USE_AA_PIXELS
    // Anti-aliasing functions
    void setPixelColor(float i, uint32_t c, bool aa = true);
    void setPixelColor(float i, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0, bool aa = true) { setPixelColor(i, RGBW32(r,g,b,w), aa); }
    void setPixelColor(float i, CRGB c, bool aa = true)                                         { setPixelColor(i, RGBW32(c.r,c.g,c.b,0), aa); }
    #endif
    #endif
    
      
    [[gnu::hot]] uint32_t getPixelColor(int i) const;  
    [[gnu::hot]] RgbwwColor getPixelColorRgbww(int i) const;
    
    // 1D support functions (some implement 2D as well)
    void blur(uint8_t blur_amount, bool smear = false);
    void fill(uint32_t c);
    void fade_out(uint8_t r);
    void fadeToBlackBy(uint8_t fadeBy);
    void blendPixelColor(int n, uint32_t color, uint8_t blend);
    void blendPixelColor(int n, CRGB c, uint8_t blend)            { blendPixelColor(n, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColor(int n, uint32_t color);
    void addPixelColor(int n, byte r, byte g, byte b, byte w = 0) { addPixelColor(n, RGBW32(r,g,b,w)); } // automatically inline
    void addPixelColor(int n, CRGB c)                             { addPixelColor(n, RGBW32(c.r,c.g,c.b,0)); } // automatically inline
    void fadePixelColor(uint16_t n, uint8_t fade);


    uint8_t get_random_wheel_index(uint8_t pos);
    uint32_t color_wheel(uint8_t pos);


    [[gnu::hot]] RgbwwColor GetPaletteColour_Rgbww(
      /**
       * @brief _pixel_position
       * ** [0-SEGLEN]
       * ** [0-255]   
       */
      uint16_t pixel_position = 0,
      /**
       * @brief flag_position_scaled255
       * ** [true] : pixel_position should be between 0-255
       * ** [false]: pixel is exact, and will automatically wrap around (ie 5 pixels inside palette will be 0,1,2,3,4,0,1,2,3,4)
       */
      uint8_t     flag_position_scaled255 = false,
      /**
       * @brief flag_wrap_hard_edge
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_wrap_hard_edge = false,
      /**
       * @brief flag_crgb_exact_colour
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_crgb_exact_colour = false,
      /**
       * @brief encoded_value
       * ** [uint32_t*] : encoded value from palette
       */
      uint8_t* encoded_value = nullptr, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly

      

      bool apply_brightness = false
    );

    /**
     * @brief Depending on the build settings later, I will want to keep a Rgbcct and U32 palette method
     * Hence a new U32 palette structure will exist that always foregoes the Rgbcct and handles in U32 format
     * This may be hardcorded with a define, or use if to switch
     * #ifdef XX
     * #define GetPaletteColour GetPaletteColour_U32
     * #else
     * #define GetPaletteColour GetPaletteColourRGBCCT
     * #endif
     **/
    [[gnu::hot]] uint32_t GetPaletteColour(
      /**
       * @brief _pixel_position
       * ** [0-SEGLEN]
       * ** [0-255]   
       */
      uint16_t pixel_position = 0,
      /**
       * @brief flag_position_scaled255
       * ** [true] : pixel_position should be between 0-255
       * ** [false]: pixel is exact, and will automatically wrap around (ie 5 pixels inside palette will be 0,1,2,3,4,0,1,2,3,4)
       */
      uint8_t     flag_position_scaled255 = false,
      /**
       * @brief flag_wrap_hard_edge
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_wrap_hard_edge = false,
      /**
       * @brief flag_crgb_exact_colour
       * ** [true] : 16 palette gradients will not blend from 15 back to 0. ie 0-255 does not become 0-240 (where 0,15,31,47,63,79,95,111,127,143,159,175,191,207,223,239)
       * ** [false]: Palette16 with 16 elements, as 0-255 pixel_position, will blend around smoothly using built-in CRGBPalette16
       */
      uint8_t     flag_crgb_exact_colour = false,
      /**
       * @brief encoded_value
       * ** [uint32_t*] : encoded value from palette
       */
      uint8_t* encoded_value = nullptr, // Must be passed in as something other than 0, or else nullptr will not be checked inside properly
      /**
       * @brief apply_brightness
       * ** [false] : Apply brightness to the colour
       * ** [true]  : Get the "full" 255 range colour object
       */
      bool apply_brightness = false,

      uint8_t pbri = 255,

      uint8_t mcol = 0
    );
    
    /**
     * WLED Palette Conversion
     * 
     * Gets a single color from the currently selected palette.
     * @param i Palette Index (if mapping is true, the full palette will be _virtualSegmentLength long, if false, 255). Will wrap around automatically.
     * @param mapping if true, LED position in segment is considered for color
     * @param wrap FastLED palettes will usually wrap back to the start smoothly. Set false to get a hard edge
     * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
     * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
     * @returns Single color from palette
     * Since inline functions are expanded at compile time and do not incur runtime overhead, you can use an inline function in a header file
    */
    inline uint32_t color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255) {
      return GetPaletteColour(i, mapping, wrap, /*crgb exact skip arg*/false, /*encoded value skip arg*/nullptr, /*apply brightness skip arg*/true, pbri, mcol);
    }


    uint8_t GetPaletteDiscreteWidth(); // Rename to colours in palette

    // 2D Blur: shortcuts for bluring columns or rows only (50% faster than full 2D blur)
    inline void blurCols(fract8 blur_amount, bool smear = false) { // blur all columns
      blur2D(0, blur_amount, smear);
    }
    inline void blurRows(fract8 blur_amount, bool smear = false) { // blur all rows
      blur2D(blur_amount, 0, smear);
    }
    /** SECTION start ****************************************************************************************************************
    * * Matrix : Inside each segment  *********************************************************************************
    * *****************************************************************************************************************
    * *****************************************************************************************************************
    * *****************************************************************************************************************
    **/

    uint16_t virtualWidth(void)  const;
    #define vWidth() virtualWidth() //tmp fix
    uint16_t virtualHeight(void) const;
    #define vHeight() virtualHeight() //tmp fix
    uint16_t nrOfVStrips(void) const;


    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

    [[gnu::hot]] uint16_t XY(int x, int y);      // support function to get relative index within segment


    void setPixelColorXY(int x, int y, uint32_t c); // set relative pixel within segment with color
      void setPixelColorXY(unsigned x, unsigned y, uint32_t c)
      { 
        // Serial.println(__LINE__);
        setPixelColorXY(int(x), int(y), c); 
      }
      void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0)
      { 
        // Serial.println(__LINE__);
        setPixelColorXY(x, y, RGBW32(r,g,b,w)); 
      } // automatically inline
      void setPixelColorXY_CRGB(int x, int y, CRGB c)
      { 
        // Serial.println(__LINE__);
        setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); 
      } // automatically inline
      
      void setPixelColorXY(float x, float y, uint32_t c, bool aa = true);

      void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true)
      { 
        // Serial.println(__LINE__);
        setPixelColorXY(x, y, RGBW32(r,g,b,w), aa); 
      }
      void setPixelColorXY_CRGB(float x, float y, CRGB c, bool aa = true)
      { 
        // Serial.println(__LINE__);
        setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), aa); 
      }

    uint32_t getPixelColorXY(uint16_t x, uint16_t y) const;
    
    void blur2D(uint8_t blur_x, uint8_t blur_y, bool smear);

    // 2D support functions
    void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t color, uint8_t blend);
    void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColorXY(int x, int y, uint32_t color, bool fast = false);
    void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0, bool fast = false) { addPixelColorXY(x, y, RGBW32(r,g,b,w), fast); } // automatically inline
    void addPixelColorXY(int x, int y, CRGB c, bool fast = false)                             { addPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), fast); }
    void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade);
    void box_blur(uint16_t i, bool vertical, fract8 blur_amount); // 1D box blur (with weight)
    void blurRow(uint16_t row, fract8 blur_amount);
    void blurCol(uint16_t col, fract8 blur_amount);
    void moveX(int8_t delta, bool wrap = false);
    void moveY(int8_t delta, bool wrap = false);
    void move(uint8_t dir, uint8_t delta, bool wrap = false);
    
    void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false);
    inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) { drawCircle(cx, cy, radius, RGBW32(c.r,c.g,c.b,0), soft); }
    
void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false);
    inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) { fillCircle(cx, cy, radius, RGBW32(c.r,c.g,c.b,0), soft); }
    
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c) { drawLine(x0, y0, x1, y1, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2 = 0, int8_t rotate = 0, bool usePalGrad = false);
    inline void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    inline void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c, CRGB c2, int8_t rotate = 0, bool usePalGrad = false) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0), RGBW32(c2.r,c2.g,c2.b,0), rotate, usePalGrad); } // automatic inline
    
    
    void drawCharacter_UsingGradientPalletes(
      unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, 
  int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundGradientHorizontal
  );//unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundHorizontalGradient);
  
  
    void wu_pixel(uint32_t x, uint32_t y, CRGB c);
    void blur1d(fract8 blur_amount); // blur all rows in 1 dimension
    void blur2d(fract8 blur_amount) { blur((uint8_t)blur_amount); }
    void fill_solid(CRGB c) { fill(RGBW32(c.r,c.g,c.b,0)); }
    void nscale8(uint8_t scale);
  #else


    uint16_t XY(uint16_t x, uint16_t y)                                    { return x; }
    void setPixelColorXY(int x, int y, uint32_t c)                         { setPixelColor(x, c); }
    void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColor(x, RGBW32(r,g,b,w)); }
    void setPixelColorXY(int x, int y, CRGB c)                             { setPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    #ifdef USE_AA_PIXELS
    void setPixelColorXY(float x, float y, uint32_t c, bool aa = true)     { setPixelColor(x, c, aa); }
    void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true) { setPixelColor(x, RGBW32(r,g,b,w), aa); }
    void setPixelColorXY(float x, float y, CRGB c, bool aa = true)         { setPixelColor(x, RGBW32(c.r,c.g,c.b,0), aa); }
    #endif
    uint32_t getPixelColorXY(uint16_t x, uint16_t y)                       { return getPixelColor(x); }
    void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t c, uint8_t blend) { blendPixelColor(x, c, blend); }
    void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColor(x, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColorXY(int x, int y, uint32_t color)                     { addPixelColor(x, color); }
    void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { addPixelColor(x, RGBW32(r,g,b,w)); }
    void addPixelColorXY(int x, int y, CRGB c)                             { addPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade)            { fadePixelColor(x, fade); }
    void box_blur(uint16_t i, bool vertical, fract8 blur_amount) {}
    inline void blur2D(uint8_t blur_x, uint8_t blur_y, bool smear = false) {}
    void blurRow(uint16_t row, fract8 blur_amount) {}
    void blurCol(uint16_t col, fract8 blur_amount) {}
    inline void moveX(int delta, bool wrap = false) {}
    inline void moveY(int delta, bool wrap = false) {}
    inline void move(uint8_t dir, uint8_t delta, bool wrap = false) {}
    inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false) {}
    inline void drawCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) {}
    inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, uint32_t c, bool soft = false) {}
    inline void fillCircle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c, bool soft = false) {}
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c) {}
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c) {}
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color) {}
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB color) {}
    void wu_pixel(uint32_t x, uint32_t y, CRGB c) {}
  #endif

  /** SECTION end ****************************************************************************************************************
  * * Matrix : Inside each segment  *********************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  * *****************************************************************************************************************
  **/

/************************************************************************************
 ****** Dynamic led buffer, segment colours ********************************************************************* 
 ************************************************************************************/

inline void Set_DynamicBuffer_DesiredColour(uint16_t pixelIndex, uint32_t color) {
    size_t offset = pixelIndex * colour_width__used_in_effect_generate * 2; // Desired is the first part of the pair

    // Write desired color to the buffer
    coldata[offset + 0] = R(color); // Red
    coldata[offset + 1] = G(color); // Green
    coldata[offset + 2] = B(color); // Blue

    if (colour_width__used_in_effect_generate == 4) {
        coldata[offset + 3] = W(color); // White for WRGB
    }
}
inline void Set_DynamicBuffer_StartingColour(uint16_t pixelIndex, uint32_t color) {
    size_t offset = (pixelIndex * colour_width__used_in_effect_generate * 2) + colour_width__used_in_effect_generate; // Starting is after desired

    // Write starting color to the buffer
    coldata[offset + 0] = R(color); // Red
    coldata[offset + 1] = G(color); // Green
    coldata[offset + 2] = B(color); // Blue

    if (colour_width__used_in_effect_generate == 4) {
        coldata[offset + 3] = W(color); // White for WRGB
    }
}

inline void Update_DynamicBuffer_DesiredColour_FullBrightness() {
    // Restore full brightness (255)
    Update_DynamicBuffer_DesiredColour_Brightness(255);
}


#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
inline void Set_DynamicBuffer_DesiredColour_RgbwwColor(uint16_t pixelIndex, const RgbwwColor& color) {
    size_t offset = pixelIndex * 10; // Desired is the first part of the pair (5 bytes * 2)

    // Write desired color to the buffer
    coldata[offset + 0] = color.R;  // Red
    coldata[offset + 1] = color.G;  // Green
    coldata[offset + 2] = color.B;  // Blue
    coldata[offset + 3] = color.WW; // Warm White
    coldata[offset + 4] = color.CW; // Cool White
}
inline void Set_DynamicBuffer_StartingColour_RgbwwColor(uint16_t pixelIndex, const RgbwwColor& color) {
    size_t offset = (pixelIndex * 10) + 5; // Starting is after desired

    // Write starting color to the buffer
    coldata[offset + 0] = color.R;  // Red
    coldata[offset + 1] = color.G;  // Green
    coldata[offset + 2] = color.B;  // Blue
    coldata[offset + 3] = color.WW; // Warm White
    coldata[offset + 4] = color.CW; // Cool White
}
#endif


inline uint32_t Get_DynamicBuffer_DesiredColour(uint16_t pixelIndex) {
    size_t offset = pixelIndex * colour_width__used_in_effect_generate * 2; // Desired is the first part of the pair

    // Extract RGB or WRGB based on colour width
    if (colour_width__used_in_effect_generate == 4) { // WRGB
        return RGBW32(coldata[offset], coldata[offset + 1], coldata[offset + 2], coldata[offset + 3]); // Correct order: R, G, B, W
    } else if (colour_width__used_in_effect_generate == 3) { // RGB
        return RGBW32(coldata[offset], coldata[offset + 1], coldata[offset + 2], 0); // No white channel
    }

    return 0; // Return black if colour width is invalid
}

inline uint32_t Get_DynamicBuffer_StartingColour(uint16_t pixelIndex) {
    size_t offset = (pixelIndex * colour_width__used_in_effect_generate * 2) + colour_width__used_in_effect_generate; // Starting is after desired

    // Extract RGB or WRGB based on colour width
    if (colour_width__used_in_effect_generate == 4) { // WRGB
        return RGBW32(coldata[offset], coldata[offset + 1], coldata[offset + 2], coldata[offset + 3]); // Correct order: R, G, B, W
    } else if (colour_width__used_in_effect_generate == 3) { // RGB
        return RGBW32(coldata[offset], coldata[offset + 1], coldata[offset + 2], 0); // No white channel
    }

    return 0; // Return black if colour width is invalid
}


#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
inline RgbwwColor Get_DynamicBuffer_DesiredColour_RgbwwColor(uint16_t pixelIndex) {
    size_t offset = pixelIndex * 10; // Desired is the first part of the pair (5 bytes * 2)

    // Extract RGBWW color from the buffer
    return RgbwwColor(
        coldata[offset + 0], // Red
        coldata[offset + 1], // Green
        coldata[offset + 2], // Blue
        coldata[offset + 3], // Warm White
        coldata[offset + 4]  // Cool White
    );
}
inline RgbwwColor Get_DynamicBuffer_StartingColour_RgbwwColor(uint16_t pixelIndex) {
    size_t offset = (pixelIndex * 10) + 5; // Starting is after desired

    // Extract RGBWW color from the buffer
    return RgbwwColor(
        coldata[offset + 0], // Red
        coldata[offset + 1], // Green
        coldata[offset + 2], // Blue
        coldata[offset + 3], // Warm White
        coldata[offset + 4]  // Cool White
    );
}
#endif


inline void Update_DynamicBuffer_DesiredColour_Brightness(uint8_t bri_rgb, uint8_t bri_ww = 255) {
    for (uint16_t pixelIndex = 0; pixelIndex < virtualLength(); pixelIndex++) {
        size_t offset = pixelIndex * colour_width__used_in_effect_generate * 2; // Desired is the first part of the pair

        // Apply brightness adjustment to RGB channels
        coldata[offset + 0] = (coldata[offset + 0] * bri_rgb) / 255; // Red
        coldata[offset + 1] = (coldata[offset + 1] * bri_rgb) / 255; // Green
        coldata[offset + 2] = (coldata[offset + 2] * bri_rgb) / 255; // Blue

        
        if (colour_width__used_in_effect_generate > 3) { // Adjust white (W) if RGBW
            coldata[offset + 3] = (coldata[offset + 3] * bri_ww) / 255; // White for RGBW or Warm White (WW)
        }        
        if (colour_width__used_in_effect_generate > 4) { // Adjust warm white (WW) and cool white (CW) if RGBWW
            coldata[offset + 4] = (coldata[offset + 4] * bri_ww) / 255; // Cool White (CW)
        }
    }
}



/************************************************************************************
 ****** Higher Level Ops ********************************************************************* 
 ************************************************************************************/
inline void DynamicBuffer_StartingColour_GetAllSegment() {
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    performance.bus_read_total_us = micros();
    #endif

    for (int pixel = 0; pixel < virtualLength(); pixel++) {
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        if (colour_width__used_in_effect_generate == 5) {
            Set_DynamicBuffer_StartingColour_RgbwwColor(pixel, getPixelColorRgbww(pixel));
        } else {
        #endif
            Set_DynamicBuffer_StartingColour(pixel, getPixelColor(pixel));
            #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE
            if(pixel < ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE) {
              SERIAL_DEBUG_COL32i("StartingColour",  getPixelColor(pixel), pixel);
            }
            #endif
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        }
        #endif
    }

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    performance.bus_read_total_us = micros() - performance.bus_read_total_us;
    #endif
}

inline void DynamicBuffer_StartingColour_GetAllSegment_WithFade(uint8_t fade) {
    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    performance.bus_read_total_us = micros();
    #endif

    for (int pixel = 0; pixel < virtualLength(); pixel++) {
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        if (colour_width__used_in_effect_generate == 5) {
            // Get the RGBWW color, apply fade, and set as starting color
            RgbwwColor rgbwwColor = getPixelColorRgbww(pixel);
            rgbwwColor.R = (rgbwwColor.R > fade) ? (rgbwwColor.R / fade) : 0;
            rgbwwColor.G = (rgbwwColor.G > fade) ? (rgbwwColor.G / fade) : 0;
            rgbwwColor.B = (rgbwwColor.B > fade) ? (rgbwwColor.B / fade) : 0;
            rgbwwColor.WW = (rgbwwColor.WW > fade) ? (rgbwwColor.WW / fade) : 0;
            rgbwwColor.CW = (rgbwwColor.CW > fade) ? (rgbwwColor.CW / fade) : 0;
            Set_DynamicBuffer_StartingColour_RgbwwColor(pixel, rgbwwColor);
        } else {
        #endif
            // Get the RGB/WRGB color, apply fade, and set as starting color
            uint32_t color = getPixelColor(pixel);
            color = FadeU32(color, fade);
            Set_DynamicBuffer_StartingColour(pixel, color);
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        }
        #endif
    }

    #ifdef ENABLE_DEBUGFEATURE_LIGHTING__PERFORMANCE_METRICS_SAFE_IN_RELEASE_MODE
    performance.bus_read_total_us = micros() - performance.bus_read_total_us;
    #endif
}


inline void AnimationProcess_LinearBlend_Dynamic_BufferU32(const AnimationParam& param) {
    float progress = param.progress;
    uint8_t blendFactor = static_cast<uint8_t>(progress * 255);

    for (int i = 0; i < virtualLength(); i++) {
      // Serial.printf("buffer32 %d\n\r", i);
        if (colour_width__used_in_effect_generate == 5) {
            #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
            // Retrieve starting and desired colors for RGBWW
            RgbwwColor startRgbww = Get_DynamicBuffer_StartingColour_RgbwwColor(i);
            RgbwwColor desiredRgbww = Get_DynamicBuffer_DesiredColour_RgbwwColor(i);

            // Blend RGBWW colors and write the result
            RgbwwColor blendedRgbww = RgbwwColor::LinearBlend(startRgbww, desiredRgbww, blendFactor);
            setPixelColor(i, blendedRgbww);
            #endif
        } else {
            // Retrieve starting and desired colors for RGB/WRGB
            uint32_t startColor = Get_DynamicBuffer_StartingColour(i);
            uint32_t desiredColor = Get_DynamicBuffer_DesiredColour(i);
            // SERIAL_DEBUG_COL32i("startColor", startColor, i);
            // SERIAL_DEBUG_COL32i("desiredColor", desiredColor, i);

            // Blend RGB/WRGB colors and write the result
            uint32_t blendedColor = ColourBlend(startColor, desiredColor, blendFactor);
            // uint32_t blendedColor = desiredColor; // When debugging without a blend

            setPixelColor(i, blendedColor);

            #ifdef ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE
            if(i < ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE) {
              SERIAL_DEBUG_COL32i("blendedColor", blendedColor, i);
            }
            #endif
        }
    }
}

inline void AnimationProcess_LinearBlend_Dynamic_BufferU32_FillSegment(const AnimationParam& param) {
    float progress = param.progress;
    uint8_t blendFactor = static_cast<uint8_t>(progress * 255);

    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    Serial.println("AnimationProcess_LinearBlend_Dynamic_BufferU32_FillSegment"); Serial.flush();
    #endif

    // Retrieve the first starting and desired colors
    if (colour_width__used_in_effect_generate == 5) {
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
        // Handle RGBWW blending
        RgbwwColor startRgbww = Get_DynamicBuffer_StartingColour_RgbwwColor(0);
        RgbwwColor desiredRgbww = Get_DynamicBuffer_DesiredColour_RgbwwColor(0);

        #ifdef ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT
        desiredRgbww.WW = 0; desiredRgbww.CW = 0;
        #endif

        // Blend the two colors
        // RgbwwColor blendedRgbww = desiredRgbww;//RgbwwColor::LinearBlend(startRgbww, desiredRgbww, blendFactor);

        RgbwwColor blendedRgbww = RgbwwColor::LinearBlend(startRgbww, desiredRgbww, blendFactor);

        // blendedRgbww = RgbwwColor(0,0,0,0,255);
        
        #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
        // blendedRgbww = RgbwwColor(1,2,255,0,0);
        Serial.printf("startRgbww RGBWW %d,%d,%d,%d,%d", startRgbww.R, startRgbww.G, startRgbww.B, startRgbww.WW, startRgbww.CW); 
        Serial.printf("\t\tdesiredRgbww RGBWW %d,%d,%d,%d,%d\n\r", desiredRgbww.R, desiredRgbww.G, desiredRgbww.B, desiredRgbww.WW, desiredRgbww.CW); 
        #endif

        // Set the blended color across the segment
        for (int pixel = 0; pixel < virtualLength(); pixel++) {
          #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
          Serial.printf("blendedRgbww RGBWW %d,%d,%d,%d,%d\n\r", blendedRgbww.R, blendedRgbww.G, blendedRgbww.B, blendedRgbww.WW, blendedRgbww.CW); 
          #endif
          setPixelColor(pixel, blendedRgbww);
        }
        
        // AddLog_Array_Block(3, PSTR("Solid Colour RGBWW"), SEGMENT.Data(), SEGMENT.DataLength(), 5, false);
        #endif
    } else {
        // Handle RGB/WRGB blending
        uint32_t startColor = Get_DynamicBuffer_StartingColour(0);
        uint32_t desiredColor = Get_DynamicBuffer_DesiredColour(0);

        // Blend the two colors
        uint32_t blendedColor = ColourBlend(startColor, desiredColor, blendFactor);

        // Set the blended color across the segment
        for (uint16_t pixel = 0; pixel < virtualLength(); pixel++) {
            setPixelColor(pixel, blendedColor);
        }
    }
}



} segment;



#include <stdint.h>

inline uint32_t HueSatBrt(uint16_t hue, uint8_t sat, uint8_t brt, bool white_from_sat = false) {
  // Normalize hue to 0-360 range
  hue %= 360;

  // Scale hue to 0-255 for conversion to RGB (divide 360 into 256 steps)
  uint8_t hue_8bit = (hue * 255) / 360;

  // Calculate RGB components from Hue
  uint8_t region = hue_8bit / 43;
  uint8_t remainder = (hue_8bit - (region * 43)) * 6;

  uint8_t p = (brt * (255 - sat)) >> 8;
  uint8_t q = (brt * (255 - ((sat * remainder) >> 8))) >> 8;
  uint8_t t = (brt * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;

  uint8_t r, g, b;
  switch (region) {
    case 0:
      r = brt; g = t; b = p;
      break;
    case 1:
      r = q; g = brt; b = p;
      break;
    case 2:
      r = p; g = brt; b = t;
      break;
    case 3:
      r = p; g = q; b = brt;
      break;
    case 4:
      r = t; g = p; b = brt;
      break;
    default:
      r = brt; g = p; b = q;
      break;
  }

  // Calculate white channel based on brightness and saturation
  uint8_t w = 0;
  if (white_from_sat) {
    w = brt * (255 - sat) / 255;
  }

  // Pack into RGBW32 format (WWWWRRRRGGGGBBBB)
  return (w << 24) | (r << 16) | (g << 8) | b;
}









  RgbwwColor ColourBlend(RgbwwColor color1, RgbwwColor color2, uint8_t blend);

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
  void show(void);
  void setTargetFps(uint8_t fps);

  
  [[gnu::hot]] ColourBaseType getPixelColor(uint32_t i) const;
  void     setPixelColor(uint32_t n, ColourBaseType c);
  // using public variables to reduce code size increase due to inline function getSegment() (with bounds checking) and color transitions
  uint32_t _colors_t_PHASE_OUT[3]; // color used for effect (includes transition)
  uint16_t _virtualSegmentLength;

  std::vector<segment> segments;
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
    bool _force_update : 1; //_triggered            : 1;
  };

  inline void force_update(void) { _force_update = true; } // Forces the next frame to be computed on all active segments.


  void LoadEffects();

  show_callback _callback;

  uint16_t* customMappingTable;
  uint16_t  customMappingSize;
  
  uint32_t _lastShow;
  
  uint8_t segment_current_index;
  uint8_t _mainSegment;

  void fill2(uint32_t c) { for (int i = 0; i < _length; i++) setPixelColor(i, c); } // fill whole strip with color (inline)


  /**
   * @brief I need to add a way to either only transmit these to the webui based on a level, or add a selector for it. Just to be used for testing
   * 
   */
  enum Effect_DevStage
  {
    Release=0, // Full working for all string types (RGB, WRGB, RGBWW, 1D and 2D), should be used for production
    Beta=1,    // Working in most cases, but not  tested fully, bug fixing only
    Alpha=2,   // Works in some exact cases, still under modifications
    Dev=3,     // Incomplete and not working, for development only
    Unstable=4 // Likely will crash, has some issue that needs to be fixed
  };


    typedef uint16_t (mAnimatorLight::*EffectFunction)();        
    void addEffect(uint8_t id, EffectFunction function, const char* config = nullptr, uint8_t development_stage = Effect_DevStage::Dev); // add effect to the list; defined in FX.cpp

    struct EFFECTS
    {
      uint8_t                         count = 0;
      std::vector<EffectFunction>     function;     // SRAM footprint: 4 bytes per element
      std::vector<const char*>        config;     // 
      std::vector<uint8_t>            development_stage; // 0:stable, 1:beta, 2:alpha, 3:dev
    }effects;


    void setupEffectData(void); // add default effects to the list; defined in FX.cpp

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
    inline uint8_t getCurrSegmentId(void) { return segment_current_index; }
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

    
    inline uint16_t getMappedPixelIndex(uint16_t index) const {           // convert logical address to physical
      if (index < customMappingSize && (realtimeMode == REALTIME_MODE_INACTIVE || realtimeRespectLedMaps)) index = customMappingTable[index];
      return index;
    };

    uint32_t effect_start_time; // WLED "now", strip.now
    uint32_t timebase;
    uint32_t currentColor(uint32_t colorNew, uint8_t tNr);

    

    #ifndef ENABLE_DEVFEATURE_LIGHTING__REMOVE_RGBCCT
    void setPixelColor_Rgbcct(int i, RgbwwColor col);
    RgbwwColor getPixelColor_Rgbcct(uint16_t i);
    #endif

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
    inline void setPixelColorXY(int x, int y, uint32_t c){       setPixelColor(y * Segment::maxWidth + x, c); }
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

    
    
    
// network time
#ifndef WLED_LAT
  #define WLED_LAT 0.0f
#endif
#ifndef WLED_LON
  #define WLED_LON 0.0f
#endif
 float longitude _INIT(WLED_LON);
 float latitude _INIT(WLED_LAT);
 time_t sunrise _INIT(0);
 time_t sunset _INIT(0);
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
// bool handleIfNoneMatchCacheHeader(AsyncWebServerRequest* request);
// void setStaticContentCacheHeaders(AsyncWebServerResponse *response);
void serveIndex(AsyncWebServerRequest* request);

#ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS
// void getSettingsJS(byte subPage, Print& settingsScript);
#else
void getSettingsJS(byte subPage, char* dest); // phase out
#endif
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

bool requestJSONBufferLock(uint16_t module);
void releaseJSONBufferLock();

    #ifdef ENABLE_WEBSERVER_LIGHTING_WEBUI
void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
bool serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient = 0);

void serveJson(AsyncWebServerRequest* request);

bool  captivePortal(AsyncWebServerRequest *request);
// void  notFound(AsyncWebServerRequest *request);


#endif // ENABLE_WEBSERVER_LIGHTING_WEBUI

void setPaletteColors(JsonArray json, CRGBPalette16 palette);

bool deserializeSegment(JsonObject elem, byte it, byte presetId = 0);

int getNumVal(const String* req, uint16_t pos);
void parseNumber(const char* str, byte* val, byte minv=0, byte maxv=255);
bool getVal(JsonVariant elem, byte* val, byte minv=0, byte maxv=255);
bool getBoolVal(JsonVariant elem, bool dflt);
bool updateVal(const char* req, const char* key, byte* val, byte minv=0, byte maxv=255);
size_t printSetFormCheckbox(Print& settingsScript, const char* key, int val);
size_t printSetFormValue(Print& settingsScript, const char* key, int val);
size_t printSetFormIndex(Print& settingsScript, const char* key, int index);
size_t printSetFormValue(Print& settingsScript, const char* key, const char* val);
size_t printSetClassElementHTML(Print& settingsScript, const char* key, const int index, const char* val);

size_t printSetFormInput(Print& settingsScript, const char* key, const char* selector, int value) ;


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
volatile uint16_t jsonBufferLock = 0;

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

#ifdef ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
char serverDescription[80];
#else
char serverDescription[40];
#endif


bool syncToggleReceive     _INIT(false);   // UIs which only have a single button for sync should toggle send+receive if this is true, only send otherwise
bool simplifiedUI          _INIT(false);   // enable simplified UI

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
  M12_pCorner = 3,
  M12_sPinwheel = 4
} mapping1D2D_t;



#ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS

static void extractPin(Print& settingsScript, JsonObject &obj, const char *key);
void XML_response(Print& dest);
static void fillUMPins(Print& settingsScript, JsonObject &mods);
void appendGPIOinfo(Print& settingsScript);
void getSettingsJS(byte subPage, Print& settingsScript);

#endif

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

#ifdef ENABLE_FEATURE_LIGHTING__DMX
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

// bool cctICused          _INIT(false); // CCT IC used (Athom 15W bulbs)

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
// bool useAMPM _INIT(false);       // 12h/24h clock format
// byte currentTimezone _INIT(0);   // Timezone ID. Refer to timezones array in wled10_ntp.ino
// int utcOffsetSecs _INIT(0);      // Seconds to offset from UTC before timzone calculation

#define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
// #define MIN_SHOW_DELAY  15
#define MIN_SHOW_DELAY   2 //(_frametime < 16 ? 8 : 15)
#define DEFAULT_LED_COUNT 30


// Segment capability byte
#define SEG_CAPABILITY_RGB     0x01
#define SEG_CAPABILITY_W       0x02
#define SEG_CAPABILITY_CCT     0x04

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

// #ifdef ENABLE_FEATURE_LIGHTING__DMX
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

// REMOVED
// // transitions
// bool          transitionActive        _INIT(false);
// uint16_t      transitionDelayDefault  _INIT(transitionDelay); // default transition time (storec in cfg.json)
// uint16_t      transitionDelayTemp     _INIT(transitionDelay); // actual transition duration (overrides transitionDelay in certain cases)
// unsigned long transitionStartTime;
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
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
    uint8_t ConstructJSON_Debug__CustomMappingTable(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    uint8_t ConstructJSON_Debug_Palette_Vector(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
    uint8_t ConstructJSON_Debug_Performance(uint8_t json_level = 0, bool json_appending = true);
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
      
      std::vector<struct handler<mAnimatorLight>*> mqtthandler_list;
    
      struct handler<mAnimatorLight> mqtthandler_settings;    
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
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
      struct handler<mAnimatorLight> mqtthandler_debug__custom_mapping_table;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
      struct handler<mAnimatorLight> mqtthandler_debug_palette_vector;
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PERFORMANCE
      struct handler<mAnimatorLight> mqtthandler_debug__performance;
      #endif
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      struct handler<mAnimatorLight> mqtthandler_debug_animations_progress;
      #endif
      
      #ifdef USE_MODULE_NETWORK_WEBSERVER
      void MQTTHandler_AddWebURL_PayloadRequests();
      #endif // USE_MODULE_NETWORK_WEBSERVER
      
    #endif // USE_MODULE_NETWORK_MQTT

    

    /************************************************************************************************
     * SECTION: Development Code
     ************************************************************************************************/

    #ifdef ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE
    void TestCode_AddBus1();
    void TestCode_Add16ParallelBus1();
    #endif // ENABLE_DEVFEATURE_LIGHTING__DEVELOPING_CODE

};

#ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
// Extern declaration of the global instance
extern mAnimatorLight* tkr_extern_lAni;  // global instance of the mAnimatorLight class for performance reasons
#endif


#endif

#endif

