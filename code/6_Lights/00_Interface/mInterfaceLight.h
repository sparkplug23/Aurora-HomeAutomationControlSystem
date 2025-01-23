#ifndef _DRIVER_INTERFACE_LIGHTS1_H
#define _DRIVER_INTERFACE_LIGHTS1_H 0.1

#define D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID 6000 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#define HUE_N2F(h) h/360.0f
#define SAT_N2F(s) s/100.0f
#define BRT_N2F(v) v/100.0f

#define HUE_F2N(h) round(h*360.0f)
#define SAT_F2N(s) round(s*100.0f)
#define BRT_F2N(v) round(v*100.0f)

#include "6_Lights/00_Interface/mBusManager.h"

#define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR  "Colour %02d"
#define D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR  "Palette %02d"

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "JsonParser.h"
#include "6_Lights/02_Palette/mPaletteLoaded.h"
#include "6_Lights/02_Palette/mPalette.h"
#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/07_Time/mTime.h"
#include "6_Lights/02_Palette/mPalette_Progmem.h"

enum LIHGT_POWER_STATE_IDS{
  LIGHT_POWER_STATE_OFF_ID=0,
  LIGHT_POWER_STATE_ON_ID,
  LIGHT_POWER_STATE_TOGGLE_ID,
  LIGHT_POWER_STATE_PAUSED,
  LIGHT_POWER_STATE_ANIMATING_ID,
  LIGHT_POWER_STATE_LENGTH_ID
};

#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
// New version of Gamma correction compute
// Instead of a table, we do a multi-linear approximation, which is close enough
// At low levels, the slope is a bit higher than actual gamma, to make changes smoother
// Internal resolution is 10 bits.

typedef struct gamma_table_t {
  uint16_t to_src;
  uint16_t to_gamma;
} gamma_table_t;

const gamma_table_t gamma_table[] = {   // don't put in PROGMEM for performance reasons
  {    1,      1 },
  {    4,      1 },
  {  209,     13 },
  {  312,     41 },
  {  457,    106 },
  {  626,    261 },
  {  762,    450 },
  {  895,    703 },
  { 1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};

// simplified Gamma table for Fade, cheating a little at low brightness
const gamma_table_t gamma_table_fast[] = {
  {   384,    192 },
  {   768,    576 },
  {  1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};
#endif // ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

// For reference, below are the computed gamma tables, via ledGamma()
// for 8 bits output:
//   0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//   1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,
//   3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,
//   6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 11,
//  11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18,
//  18, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
//  25, 26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 36, 37, 38,
//  39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53,
//  54, 55, 56, 57, 58, 59, 60, 61, 61, 62, 63, 64, 65, 67, 68, 69,
//  71, 72, 73, 75, 76, 78, 79, 80, 82, 83, 85, 86, 87, 89, 90, 91,
//  93, 94, 95, 97, 98,100,101,102,104,105,107,108,109,111,112,114,
// 116,118,120,122,124,125,127,129,131,133,135,137,139,141,143,144,
// 146,148,150,152,154,156,158,160,162,164,166,168,170,171,173,175,
// 178,180,183,185,188,190,193,195,198,200,203,205,208,210,213,215,
// 218,220,223,225,228,230,233,235,238,240,243,245,248,250,253,255
//
// and for 10 bits output:
//    0,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
//    5,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,
//    8,   8,   9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,
//   12,  12,  13,  13,  13,  14,  15,  16,  17,  18,  20,  21,  22,  23,  24,  25,
//   26,  27,  28,  29,  30,  31,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,
//   45,  47,  49,  50,  52,  54,  56,  58,  59,  61,  63,  65,  67,  68,  70,  72,
//   74,  76,  77,  79,  81,  83,  84,  86,  88,  90,  92,  93,  95,  97,  99, 101,
//  102, 104, 106, 110, 113, 117, 121, 124, 128, 132, 135, 139, 143, 146, 150, 154,
//  158, 162, 166, 169, 173, 177, 180, 184, 188, 191, 195, 199, 202, 206, 210, 213,
//  217, 221, 224, 228, 232, 235, 239, 243, 246, 250, 254, 257, 261, 267, 272, 278,
//  283, 289, 294, 300, 305, 311, 317, 322, 328, 333, 339, 344, 350, 356, 361, 367,
//  372, 378, 383, 389, 394, 400, 406, 411, 417, 422, 428, 433, 439, 444, 450, 458,
//  465, 473, 480, 488, 496, 503, 511, 518, 526, 534, 541, 549, 557, 564, 572, 579,
//  587, 595, 602, 610, 617, 627, 635, 642, 650, 657, 665, 673, 680, 688, 695, 703,
//  713, 723, 733, 743, 753, 763, 773, 783, 793, 803, 813, 823, 833, 843, 853, 863,
//  873, 883, 893, 903, 913, 923, 933, 943, 953, 963, 973, 983, 993,1003,1013,1023
//
// Output for Dimmer 0..100 values
//   0,  1,  2,  3,  3,  4,  4,  5,  5,  6,  7,  7,  8,  8,  9,
//  10, 10, 11, 12, 12, 13, 15, 17, 21, 23, 26, 28, 31, 34, 37,
//  40, 43, 49, 52, 58, 61, 67, 70, 76, 79, 84, 90, 93, 99,102,
// 110,117,128,135,146,158,166,177,184,195,202,213,221,232,239,
// 250,261,272,289,300,317,328,344,356,372,389,400,417,428,444,
// 458,480,496,518,534,557,579,595,617,635,657,673,695,713,743,
// 773,793,823,843,873,893,923,943,973,993,1023


DEFINE_PGM_CTR(PM_ANIMATION_MODE_NONE_NAME_CTR )   "None"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_CHANGE_POWER_NAME_CTR )   "Change Power"     ;   
DEFINE_PGM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
DEFINE_PGM_CTR(PM_ANIMATION_MODE_EFFECTS_NAME_CTR  )         D_EFFECTS;
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR) "Manual SetPixel";
#endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR) "state";  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR) "debug/module";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_BUS_CONFIG__CTR) "debug/busconfig";


#include "1_TaskerManager/mTaskerManager.h"


class mInterfaceLight :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mInterfaceLight(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);
    
    static constexpr const char* PM_MODULE_LIGHTS_INTERFACE_CTR = D_MODULE_LIGHTS_INTERFACE_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_INTERFACE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    // Add all data here that needs to be saved to filesystem
    struct MODULE_RUNTIME{ 


    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/


    // struct SETTINGS
    // {
      
    // }settings;

    uint8_t GetColourOrder_FromName(const char* c);
    
    void EveryLoop();
    
    void LightCalcPWMRange();
    void parseJSONObject__BusConfig(JsonParserObject obj); 
        
    void Template_Load();
    void Template_Load_DefaultConfig();
        
    uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
    uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023

    BusManager* bus_manager = nullptr;
    BusConfig* busConfigs[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES] = {nullptr};
    

    #ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();
    #endif // rules
    
    void EverySecond_AutoOff();

    void BusManager_Create_DefaultSingleNeoPixel();
    void BusManager_Create_DefaultSinglePWM_5CH();

    uint8_t light_power_state = 0;
    uint8_t light_power_Saved = 0;
    void CommandSet_LightPowerState(uint8_t value);
    bool CommandGet_LightPowerState();
    void CommandSet_Auto_Time_Off_Secs(uint16_t value);
    struct AUTO_OFF_SETTINGS{
      uint16_t time_decounter_secs = 0;
    }auto_off_settings;

    String GetColourOrderString(uint8_t colour_order);

    #ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
    uint16_t change8to10(uint8_t v);
    uint8_t change10to8(uint16_t v);
    uint16_t ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr);
    uint16_t ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr);
    uint16_t ledGamma10_10(uint16_t v);
    uint16_t ledGamma10(uint8_t v);
    uint8_t ledGamma(uint8_t v);
    void calcGammaMultiChannels(uint16_t cur_col_10[5]);
    void calcGammaBulbs(uint16_t cur_col_10[5]);
    bool isChannelGammaCorrected(uint32_t channel);
    uint16_t fadeGamma(uint32_t channel, uint16_t v);
    uint16_t fadeGammaReverse(uint32_t channel, uint16_t vg);
    #endif //ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

    uint8_t  _briRGB_Global = 255;  // in place of WLED "bri" 0..255 // Used for ws28xx
    uint8_t  _briCT_Global = 255;

    inline uint8_t getBri_Global(void) { // return the max of _briCT and _briRGB
      return (_briRGB_Global >= _briCT_Global) ? _briRGB_Global : _briCT_Global;
    }
    inline uint8_t getBriRGB_Global()
    {
      return _briRGB_Global;
    }
    inline uint8_t getBriCCT_Global()
    {
      return _briCT_Global;
    }
    void setBriRGB_Global(uint8_t bri_rgb) 
    {
      _briRGB_Global = bri_rgb;
    }
    void setBriCT_Global(uint8_t bri_ct) 
    {
      _briCT_Global = bri_ct;
    }

    void CommandSet_Brt_255(uint8_t value);
    void CommandSet_Global_BrtRGB_255(uint8_t bri, uint8_t segment_index = 0);
    void CommandSet_Global_BrtCCT_255(uint8_t bri, uint8_t segment_index = 0);

    RgbColor GetColourValueUsingMaps_FullBrightness(float value, uint8_t map_style_id = 0, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    RgbColor GetColourValueUsingMaps_AdjustedBrightness(float value, uint8_t map_style_id, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    
    /******************************************************************************************************************
     * ConstructJson
    *******************************************************************************************************************/
  
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
    uint8_t ConstructJSON_Debug_Module_Config(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
    uint8_t ConstructJSON_Debug__BusConfig(uint8_t json_level, bool json_appending);
    #endif
  
    /******************************************************************************************************************
     * MQTT
    *******************************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mInterfaceLight> mqtthandler__settings__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__state__ifchanged;
    struct handler<mInterfaceLight> mqtthandler__debug_module_config__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__debug_bus_config__teleperiod;
                
    std::vector<struct handler<mInterfaceLight>*> mqtthandler_list;

    #endif

    /******************************************************************************************************************
     * WEBSERVER
    *******************************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    #ifdef ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS

      /**
       * @brief MQTTHandler_AddWebURL_PayloadRequests
       * */
      void MQTTHandler_AddWebURL_PayloadRequests();

    #endif // ENABLE_FEATURE_MQTT__ADD_WEBURL_FOR_PAYLOAD_REQUESTS
    #endif // USE_MODULE_NETWORK_WEBSERVER



};
#endif

#endif
