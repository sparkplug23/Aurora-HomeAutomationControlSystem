#ifndef WLED_CONST_H
#define WLED_CONST_H


//RGB to RGBW conversion mode
#define RGBW_MODE_MANUAL_ONLY     0    // No automatic white channel calculation. Manual white channel slider
#define RGBW_MODE_AUTO_BRIGHTER   1    // New algorithm. Adds as much white as the darkest RGBW channel
#define RGBW_MODE_AUTO_ACCURATE   2    // New algorithm. Adds as much white as the darkest RGBW channel and subtracts this amount from each RGB channel
#define RGBW_MODE_DUAL            3    // Manual slider + auto calculation. Automatically calculates only if manual slider is set to off (0)
#define RGBW_MODE_MAX             4    // Sets white to the value of the brightest RGB channel (good for white-only LEDs without any RGB)
//#define RGBW_MODE_LEGACY        4    // Old floating algorithm. Too slow for realtime and palette support (unused)
#define AW_GLOBAL_DISABLED      255    // Global auto white mode override disabled. Per-bus setting is used

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

//realtime override modes
#define REALTIME_OVERRIDE_NONE    0
#define REALTIME_OVERRIDE_ONCE    1
#define REALTIME_OVERRIDE_ALWAYS  2

//E1.31 DMX modes
#define DMX_MODE_DISABLED         0            //not used
#define DMX_MODE_SINGLE_RGB       1            //all LEDs same RGB color (3 channels)
#define DMX_MODE_SINGLE_DRGB      2            //all LEDs same RGB color and master dimmer (4 channels)
#define DMX_MODE_EFFECT           3            //trigger standalone effects of WLED (11 channels)
#define DMX_MODE_MULTIPLE_RGB     4            //every LED is addressed with its own RGB (ledCount * 3 channels)
#define DMX_MODE_MULTIPLE_DRGB    5            //every LED is addressed with its own RGB and share a master dimmer (ledCount * 3 + 1 channels)
#define DMX_MODE_MULTIPLE_RGBW    6            //every LED is addressed with its own RGBW (ledCount * 4 channels)

//Color orders
#define COL_ORDER_GRB             0           //GRB(w),defaut
#define COL_ORDER_RGB             1           //common for WS2811
#define COL_ORDER_BRG             2
#define COL_ORDER_RBG             3
#define COL_ORDER_BGR             4
#define COL_ORDER_GBR             5
#define COL_ORDER_MAX             5



//Light capability byte (unused) 0bRCCCTTTT
//bits 0/1/2/3: specifies a type of LED driver. A single "driver" may have different chip models but must have the same protocol/behavior
//bits 4/5/6: specifies the class of LED driver - 0b000 (dec. 0-15)  unconfigured/reserved
//                                              - 0b001 (dec. 16-31) digital (data pin only)
//                                              - 0b010 (dec. 32-47) analog (PWM)
//                                              - 0b011 (dec. 48-63) digital (data + clock / SPI)
//                                              - 0b100 (dec. 64-79) unused/reserved
//                                              - 0b101 (dec. 80-95) virtual network busses
//                                              - 0b110 (dec. 96-111) unused/reserved
//                                              - 0b111 (dec. 112-127) unused/reserved
//bit 7 is reserved and set to 0

#define BUSTYPE_NONE                 0            //light is not configured
#define BUSTYPE_RESERVED             1            //unused. Might indicate a "virtual" light
//Digital types (data pin only) (16-39)
#define BUSTYPE_DIGITAL__MIN         16            // first usable digital type
#define BUSTYPE_WS2812_1CH          18            //white-only chips (1 channel per IC) (unused)
#define BUSTYPE_WS2812_1CH_X3       19            //white-only chips (3 channels per IC)
#define BUSTYPE_WS2812_2CH_X3       20            //CCT chips (1st IC controls WW + CW of 1st zone and CW of 2nd zone, 2nd IC controls WW of 2nd zone and WW + CW of 3rd zone)
#define BUSTYPE_WS2812_WWA          21            //amber + warm + cold white
#define BUSTYPE_WS2812_RGB          22
#define BUSTYPE_GS8608              23            //same driver as WS2812, but will require signal 2x per second (else displays test pattern)
#define BUSTYPE_WS2811_400KHZ       24            //half-speed WS2812 protocol, used by very old WS2811 units
#define BUSTYPE_TM1829              25
#define BUSTYPE_UCS8903             26
#define BUSTYPE_APA106              27
#define BUSTYPE_FW1906              28            //RGB + CW + WW + unused channel (6 channels per IC)
#define BUSTYPE_UCS8904             29            //first RGBW digital type (hardcoded in busmanager.cpp, memUsage())
#define BUSTYPE_SK6812_RGBW         30
#define BUSTYPE_TM1814              31
#define BUSTYPE_WS2805              32            //RGB + WW + CW
#define BUSTYPE_TM1914              33            //RGB
#define BUSTYPE_SM16825             34            //RGB + WW + CW
#define BUSTYPE_WS2805_RGBWW        35
#define BUSTYPE_DIGITAL__MAX         39            // last usable digital type
//"Analog" types (PWM) (40-47)
#define BUSTYPE_ONOFF               40            //binary output (relays etc.)
#define BUSTYPE_ANALOG__MIN         41            // first usable analog type
#define BUSTYPE_ANALOG_1CH          41            //single channel PWM. Uses value of brightest RGBW channel
#define BUSTYPE_ANALOG_2CH          42            //analog WW + CW
#define BUSTYPE_ANALOG_3CH          43            //analog RGB
#define BUSTYPE_ANALOG_4CH          44            //analog RGBW
#define BUSTYPE_ANALOG_5CH          45            //analog RGB + WW + CW
#define BUSTYPE_ANALOG_6CH          46            //analog RGB + WW + CW
#define BUSTYPE_ANALOG__MAX         47            //analog RGB + WW + CW
//Digital types (data + clock / SPI) (48-63)
#define BUSTYPE_2PIN_MIN            48
#define BUSTYPE_WS2801              50
#define BUSTYPE_APA102              51
#define BUSTYPE_LPD8806             52
#define BUSTYPE_P9813               53
#define BUSTYPE_LPD6803             54
#define BUSTYPE_2PIN_MAX            63
//Network types (master broadcast) (80-95)
#define BUSTYPE_VIRTUAL_MIN         80
#define BUSTYPE_NET_DDP_RGB         80            //network DDP RGB bus (master broadcast bus)
#define BUSTYPE_NET_E131_RGB        81            //network E131 RGB bus (master broadcast bus, unused)
#define BUSTYPE_NET_ARTNET_RGB      82            //network ArtNet RGB bus (master broadcast bus, unused)
#define BUSTYPE_NET_DDP_RGBW        88            //network DDP RGBW bus (master broadcast bus)
#define BUSTYPE_NET_ARTNET_RGBW     89            //network ArtNet RGB bus (master broadcast bus, unused)
#define BUSTYPE_VIRTUAL_MAX         95


/**
 * @brief Changing this to function will allow the types above to be changed to ENUM
 * 
 */
#define IS_BUSTYPE_DIGITAL(t)       ((t) & 0x10) //digital are 16-31 and 48-63
// #define IS_BUSTYPE_DIGITAL(t)       ((t) > 0 && (t) < 46) //digital are 16-31 and 48-63
#define IS_BUSTYPE_PWM(t)           ((t) > 40 && (t) < 46)
#define NUM_BUSTYPE_PWM_PINS(t)     ((t) - 40) //for analog PWM 41-45 only
#define IS_BUSTYPE_2PIN(t)          ((t) > 47)

//Color orders
#define COL_ORDER_GRB             0           //GRB(w),defaut
#define COL_ORDER_RGB             1           //common for WS2811
#define COL_ORDER_BRG             2
#define COL_ORDER_RBG             3
#define COL_ORDER_BGR             4
#define COL_ORDER_GBR             5
#define COL_ORDER_MAX             5

//Button type
#define BTN_TYPE_NONE             0
#define BTN_TYPE_RESERVED         1
#define BTN_TYPE_PUSH             2
#define BTN_TYPE_PUSH_ACT_HIGH    3
#define BTN_TYPE_SWITCH           4
#define BTN_TYPE_PIR_SENSOR       5
#define BTN_TYPE_TOUCH            6
#define BTN_TYPE_ANALOG           7
#define BTN_TYPE_ANALOG_INVERTED  8

//Ethernet board types
#define WLED_NUM_ETH_TYPES        9

#define WLED_ETH_NONE             0
#define WLED_ETH_WT32_ETH01       1
#define WLED_ETH_ESP32_POE        2
#define WLED_ETH_WESP32           3
#define WLED_ETH_QUINLED          4
#define WLED_ETH_TWILIGHTLORD     5
#define WLED_ETH_ESP32DEUX        6

//Hue error codes
#define HUE_ERROR_INACTIVE        0
#define HUE_ERROR_UNAUTHORIZED    1
#define HUE_ERROR_LIGHTID         3
#define HUE_ERROR_PUSHLINK      101
#define HUE_ERROR_JSON_PARSING  250
#define HUE_ERROR_TIMEOUT       251
#define HUE_ERROR_ACTIVE        255

//Segment option byte bits
#define SEG_OPTION_SELECTED       0
#define SEG_OPTION_REVERSED       1
#define SEG_OPTION_ON             2
#define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
#define SEG_OPTION_FREEZE         4            //Segment contents will not be refreshed
#define SEG_OPTION_RESET          5            //Segment runtime requires reset
#define SEG_OPTION_TRANSITIONAL   6
#define SEG_OPTION_REVERSED_Y     7
#define SEG_OPTION_MIRROR_Y       8
#define SEG_OPTION_TRANSPOSED     9

//Segment differs return byte
#define SEG_DIFFERS_BRI        0x01 // opacity
#define SEG_DIFFERS_OPT        0x02 // all segment options except: selected, reset & transitional
#define SEG_DIFFERS_COL        0x04 // colors
#define SEG_DIFFERS_FX         0x08 // effect/mode parameters
#define SEG_DIFFERS_BOUNDS     0x10 // segment start/stop ounds
#define SEG_DIFFERS_GSO        0x20 // grouping, spacing & offset
#define SEG_DIFFERS_SEL        0x80 // selected

//Playlist option byte
#define PL_OPTION_SHUFFLE      0x01

// WLED Error modes
#define ERR_NONE         0  // All good :)
#define ERR_EEP_COMMIT   2  // Could not commit to EEPROM (wrong flash layout?)
#define ERR_NOBUF        3  // JSON buffer was not released in time, request cannot be handled at this time
#define ERR_JSON         9  // JSON parsing failed (input too large?)
#define ERR_FS_BEGIN    10  // Could not init filesystem (no partition?)
#define ERR_FS_QUOTA    11  // The FS is full or the maximum file size is reached
#define ERR_FS_PLOAD    12  // It was attempted to load a preset that does not exist
#define ERR_FS_IRLOAD   13  // It was attempted to load an IR JSON cmd, but the "ir.json" file does not exist
#define ERR_FS_GENERAL  19  // A general unspecified filesystem error occured
#define ERR_OVERTEMP    30  // An attached temperature sensor has measured above threshold temperature (not implemented)
#define ERR_OVERCURRENT 31  // An attached current sensor has measured a current above the threshold (not implemented)
#define ERR_UNDERVOLT   32  // An attached voltmeter has measured a voltage below the threshold (not implemented)

//Timer mode types
#define NL_MODE_SET               0            //After nightlight time elapsed, set to target brightness
#define NL_MODE_FADE              1            //Fade to target brightness gradually
#define NL_MODE_COLORFADE         2            //Fade to target brightness and secondary color gradually
#define NL_MODE_SUN               3            //Sunrise/sunset. Target brightness is set immediately, then Sunrise effect is started. Max 60 min.


// #define NTP_PACKET_SIZE 48

//maximum number of rendered LEDs - this does not have to match max. physical LEDs, e.g. if there are virtual busses 
#ifndef MAX_LEDS_NEO
#ifdef ESP8266
#define MAX_LEDS_NEO 1664 //can't rely on memory limit to limit this to 1600 LEDs
#else
#define MAX_LEDS_NEO 8192 // MAX_LEDS as per WLED
#endif
#endif

#ifndef MAX_LED_MEMORY
  #ifdef ESP8266
    #define MAX_LED_MEMORY 4000
  #else
    #if defined(ARDUINO_ARCH_ESP32S2) || defined(ARDUINO_ARCH_ESP32C3)
      #define MAX_LED_MEMORY 32000
    #else
      #define MAX_LED_MEMORY 64000
    #endif
  #endif
#endif

#ifndef MAX_LEDS_PER_BUS
#define MAX_LEDS_PER_BUS 4000   // may not be enough for fast LEDs (i.e. APA102)
#endif

#define OUTPUT_BUSPWM_MAX_PINS 5

// string temp buffer (now stored in stack locally)
#ifdef ESP8266
#define SETTINGS_STACK_BUF_SIZE 2048
#else
#define SETTINGS_STACK_BUF_SIZE 3096 
#endif

#ifdef WLED_USE_ETHERNET
  #define E131_MAX_UNIVERSE_COUNT 20
#else
  #ifdef ESP8266
    #define E131_MAX_UNIVERSE_COUNT 9
  #else
    #define E131_MAX_UNIVERSE_COUNT 12
  #endif
#endif

#ifndef ABL_MILLIAMPS_DEFAULT
  #define ABL_MILLIAMPS_DEFAULT 850   // auto lower brightness to stay close to milliampere limit
#else
  #if ABL_MILLIAMPS_DEFAULT == 0      // disable ABL
  #elif ABL_MILLIAMPS_DEFAULT < 250   // make sure value is at least 250
   #warning "make sure value is at least 250"
   #define ABL_MILLIAMPS_DEFAULT 250
  #endif
#endif

// According to this post by Boris Lobo who is quite an expert in the ESP32:

// The available duty levels are (2bit_num)-1, where bit_num can be 1-15.

// The maximal frequency is 80000000 / 2bit_num

// I made this table for you:

// bits	frequency (Hz)
// 8	312500
// 9	156250
// 10	78125
// 11	39062.5
// 12	19531.25
// 13	9765.625
// 14	4882.8125
// 15	2441.40625
// 16	1220.703125


// PWM settings
#ifndef WLED_PWM_FREQ
#ifdef ESP8266
  #define WLED_PWM_FREQ    880 //PWM frequency proven as good for LEDs
#else
  #define WLED_PWM_FREQ  1000//78125 //19531
#endif
#endif

#define TOUCH_THRESHOLD 32 // limit to recognize a touch, higher value means more sensitive

// Size of buffer for API JSON object (increase for more segments)
#ifdef ESP8266
  #define JSON_BUFFER_SIZE 1000//10240
#else
  #define JSON_BUFFER_SIZE 24576
#endif

//#define MIN_HEAP_SIZE (MAX_LED_MEMORY+2048)
#define MIN_HEAP_SIZE (8192)

// Maximum size of node map (list of other WLED instances)
#ifdef ESP8266
  #define WLED_MAX_NODES 24
#else
  #define WLED_MAX_NODES 150
#endif

//this is merely a default now and can be changed at runtime
#ifndef LEDPIN
#if defined(ESP8266) || (defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)) || defined(CONFIG_IDF_TARGET_ESP32C3)
  #define LEDPIN 2    // GPIO2 (D4) on Wemod D1 mini compatible boards
#else
  #define LEDPIN 16   // aligns with GPIO2 (D4) on Wemos D1 mini32 compatible boards
#endif
#endif

#ifdef WLED_ENABLE_DMX
#if (LEDPIN == 2)
  #undef LEDPIN
  #define LEDPIN 1
  #warning "Pin conflict compiling with DMX and LEDs on pin 2. The default LED pin has been changed to pin 1."
#endif
#endif

#ifndef DEFAULT_LED_COUNT
  #define DEFAULT_LED_COUNT 10
#endif

#define INTERFACE_UPDATE_COOLDOWN 2000 //time in ms to wait between websockets, alexa, and MQTT updates

#if defined(ESP8266) && defined(HW_PIN_SCL)
  #undef HW_PIN_SCL
#endif
#if defined(ESP8266) && defined(HW_PIN_SDA)
  #undef HW_PIN_SDA
#endif
#ifndef HW_PIN_SCL
  #define HW_PIN_SCL SCL
#endif
#ifndef HW_PIN_SDA
  #define HW_PIN_SDA SDA
#endif

#if defined(ESP8266) && defined(HW_PIN_CLOCKSPI)
  #undef HW_PIN_CLOCKSPI
#endif
#if defined(ESP8266) && defined(HW_PIN_DATASPI)
  #undef HW_PIN_DATASPI
#endif
#if defined(ESP8266) && defined(HW_PIN_MISOSPI)
  #undef HW_PIN_MISOSPI
#endif
#if defined(ESP8266) && defined(HW_PIN_CSSPI)
  #undef HW_PIN_CSSPI
#endif
// defaults for VSPI
#ifndef HW_PIN_CLOCKSPI
  #define HW_PIN_CLOCKSPI SCK
#endif
#ifndef HW_PIN_DATASPI
  #define HW_PIN_DATASPI MOSI
#endif
#ifndef HW_PIN_MISOSPI
  #define HW_PIN_MISOSPI MISO
#endif
#ifndef HW_PIN_CSSPI
  #define HW_PIN_CSSPI SS
#endif

// #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

#endif // guard
