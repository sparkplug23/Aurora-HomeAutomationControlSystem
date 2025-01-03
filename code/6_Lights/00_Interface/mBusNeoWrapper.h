#ifndef BusWrapper_h
#define BusWrapper_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


/**
 * @brief 
 * C3: I2S0 and I2S1 methods not supported (has one I2S bus)
 * S2: I2S1 methods not supported (has one I2S bus)
 * S3: I2S0 and I2S1 methods not supported yet (has two I2S buses)
 * https://github.com/Makuna/NeoPixelBus/blob/b32f719e95ef3c35c46da5c99538017ef925c026/src/internal/Esp32_i2s.h#L4
 * https://github.com/Makuna/NeoPixelBus/blob/b32f719e95ef3c35c46da5c99538017ef925c026/src/internal/NeoEsp32RmtMethod.h#L857
**/

#if !defined(NEOPIXEL_DISABLE_I2S0_PIXELBUS) && (defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3))
#define NEOPIXEL_DISABLE_I2S0_PIXELBUS
#endif
#if !defined(NEOPIXEL_DISABLE_I2S1_PIXELBUS) && (defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S2))
#define NEOPIXEL_DISABLE_I2S1_PIXELBUS
#endif

//Hardware SPI Pins
#define P_8266_HS_MOSI 13
#define P_8266_HS_CLK  14
#define P_32_HS_MOSI   13
#define P_32_HS_CLK    14
#define P_32_VS_MOSI   23
#define P_32_VS_CLK    18


// Define masks for the RGB and white channel bits
#define COLOUR_ORDER_RGB_MASK 0x0F // Lower 4 bits for RGB swapping
#define COLOUR_ORDER_WHITE_MASK 0xF0 // Upper 4 bits for white channel swapping


enum EM_BUS_TYPE
{
  BUSTYPE__NONE__ID = 0,  
  /*************************** ESP8266 Neopixel methods ***/
  // RGB
  BUSTYPE__8266_U0_3__ID = 1,
  BUSTYPE__8266_U1_3__ID = 2,
  BUSTYPE__8266_DM_3__ID = 3,
  // RGBW
  BUSTYPE__8266_U0_4__ID = 5,
  BUSTYPE__8266_U1_4__ID = 6,
  BUSTYPE__8266_DM_4__ID = 7,  
  /**************************** ESP32 Neopixel methods ***/
  // RGB
  BUSTYPE__32_RN_3__ID = 23,
  BUSTYPE__32_I0_3__ID,
  BUSTYPE__32_I1_3__ID,
  BUSTYPE__32_I1_3P__ID,
  BUSTYPE__32_I0_3P__ID,
  // RGBW
  BUSTYPE__32_RN_4__ID,
  BUSTYPE__32_I0_4__ID,
  BUSTYPE__32_I1_4__ID,
  BUSTYPE__32_I1_4P__ID,
  BUSTYPE__32_I0_4P__ID,
  // RGBWW
  BUSTYPE__32_RN_5__ID, 
  BUSTYPE__32_I0_5__ID,
  BUSTYPE__32_I1_5__ID,
  BUSTYPE__32_I1_5P__ID,
  BUSTYPE__32_I0_5P__ID,
  // 400Kbps
  BUSTYPE__32_RN_400_3__ID,
  BUSTYPE__32_I0_400_3__ID,
  BUSTYPE__32_I1_400_3__ID,
  // APA102
  BUSTYPE__HS_DOT_3__ID, // hardware SPI
  BUSTYPE__SS_DOT_3__ID, // soft SPI
  // LPD8806
  BUSTYPE__HS_LPD_3__ID,
  BUSTYPE__SS_LPD_3__ID,
  // WS2801
  BUSTYPE__HS_WS1_3__ID,
  BUSTYPE__SS_WS1_3__ID,
  // P9813
  BUSTYPE__HS_P98_3__ID,
  BUSTYPE__SS_P98_3__ID,
  // LPD6803
  BUSTYPE__HS_LPO_3__ID,
  BUSTYPE__SS_LPO_3__ID
};


#define ENABLE_PIXELBUS_8266_U0_TYPES
#define ENABLE_PIXELBUS_8266_U1_TYPES
#define ENABLE_PIXELBUS_8266_DM_TYPES

#include "NeoPixelBusLg.h"

/*** ESP8266 Neopixel methods ***/
#ifdef ESP8266
//RGB
#ifdef ENABLE_PIXELBUS_8266_U0_TYPES
#define PIXELBUS_8266_U0_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp8266Uart0Ws2813Method, NeoGammaNullMethod> //3 chan, esp8266, gpio1
#endif
#ifdef ENABLE_PIXELBUS_8266_U1_TYPES
#define PIXELBUS_8266_U1_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp8266Uart1Ws2813Method, NeoGammaNullMethod> //3 chan, esp8266, gpio2
#endif 
#ifdef ENABLE_PIXELBUS_8266_DM_TYPES
#define PIXELBUS_8266_DM_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod, NeoGammaNullMethod>  //3 chan, esp8266, gpio3
#endif
// #define PIXELBUS_8266_BB_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod, NeoGammaNullMethod> //3 chan, esp8266, bb (any pin but 16)
//RGBW
#ifdef ENABLE_PIXELBUS_8266_U0_TYPES
#define PIXELBUS_8266_U0_4 NeoPixelBusLg<NeoRgbwFeature, NeoEsp8266Uart0Ws2813Method, NeoGammaNullMethod>   //4 chan, esp8266, gpio1
#endif 
#ifdef ENABLE_PIXELBUS_8266_U1_TYPES
#define PIXELBUS_8266_U1_4 NeoPixelBusLg<NeoRgbwFeature, NeoEsp8266Uart1Ws2813Method, NeoGammaNullMethod>   //4 chan, esp8266, gpio2
#endif
#ifdef ENABLE_PIXELBUS_8266_DM_TYPES
#define PIXELBUS_8266_DM_4 NeoPixelBusLg<NeoRgbwFeature, NeoEsp8266Dma800KbpsMethod, NeoGammaNullMethod>    //4 chan, esp8266, gpio3
#endif
#endif

/*** ESP32 Neopixel methods ***/
#ifdef ARDUINO_ARCH_ESP32
  //RGB
  #define PIXELBUS_32_RN_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp32RmtNWs2812xMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I0_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s0800KbpsMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s1800KbpsMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_3P NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s1X8Ws2812xMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I0_3P NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s0X16Ws2812xMethod, NeoGammaNullMethod>
  //RGBW
  #define PIXELBUS_32_RN_4 NeoPixelBusLg<NeoRgbwFeature, NeoEsp32RmtNWs2812xMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I0_4 NeoPixelBusLg<NeoRgbwFeature, NeoEsp32I2s0800KbpsMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_4 NeoPixelBusLg<NeoRgbwFeature, NeoEsp32I2s1800KbpsMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_4P NeoPixelBusLg<NeoRgbwFeature, NeoEsp32I2s1X8Sk6812Method, NeoGammaNullMethod>
  #define PIXELBUS_32_I0_4P NeoPixelBusLg<NeoRgbwFeature, NeoEsp32I2s0X16Sk6812Method, NeoGammaNullMethod>
  //RGBWW (WS2805)
  #define PIXELBUS_32_RN_5 NeoPixelBusLg<NeoRgbwwFeature, NeoEsp32I2s0Ws2805Method, NeoGammaNullMethod> // No RMT method
  #define PIXELBUS_32_I0_5 NeoPixelBusLg<NeoRgbwwFeature, NeoEsp32I2s0Ws2805Method, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_5 NeoPixelBusLg<NeoRgbwwFeature, NeoEsp32I2s1Ws2805Method, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_5P NeoPixelBusLg<NeoRgbwwFeature, NeoEsp32I2s1X8Ws2805Method, NeoGammaNullMethod>
  #define PIXELBUS_32_I0_5P NeoPixelBusLg<NeoRgbwwFeature, NeoEsp32I2s0X16Ws2805Method, NeoGammaNullMethod>
  //400Kbps
  #define PIXELBUS_32_RN_400_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp32RmtN400KbpsMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I0_400_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s0400KbpsMethod, NeoGammaNullMethod>
  #define PIXELBUS_32_I1_400_3 NeoPixelBusLg<NeoRgbFeature, NeoEsp32I2s1400KbpsMethod, NeoGammaNullMethod>
#endif

// 48bit & 64bit to 24bit & 32bit RGB(W) conversion
#define toRGBW32(c) (RGBW32((c>>40)&0xFF, (c>>24)&0xFF, (c>>8)&0xFF, (c>>56)&0xFF))
#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))

// handles pointer type conversion for all possible bus types
class PolyBus 
{
  private:

    /**
     * @brief 
     * 0 : No parallel, uses I0, then I1, then RMT 0 to 7 channels for 10 total
     * 1 : Uses X8 parallel on I2S1
     * 2 : Uses X16 parallel on IS20/I2S1 jointly
     */
    static bool useParallelI2S;
    static uint8_t required_channels;

  public:
  
    static inline void useParallelOutput(bool b = true) { useParallelI2S = b; }
    static inline bool isParallelOutput(void) { return useParallelI2S; }
    static inline void setRequiredChannels(uint8_t channels){ required_channels = channels; }

  static void begin(void* busPtr, uint8_t busType, uint8_t* pins) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::begin busPtr UNSET, busType %d, pin[0] %d\n\r", busType, pins[0]);
    #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->Begin(); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->Begin(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: (static_cast<PIXELBUS_32_RN_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_RN_4__ID: (static_cast<PIXELBUS_32_RN_4*>(busPtr))->Begin(); break;
      case BUSTYPE__32_RN_400_3__ID: (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: (static_cast<PIXELBUS_32_I0_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_4__ID: (static_cast<PIXELBUS_32_I0_4*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_5__ID: (static_cast<PIXELBUS_32_I0_5*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_400_3__ID: (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_3P__ID: (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_4P__ID: (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I0_5P__ID: (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->Begin(); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->Begin(); break;
      #endif
    #endif
    }
  };


  static void* create(uint8_t busType, uint8_t* pins, uint16_t len, uint8_t channel) 
  {
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::create busType %d, pin[0] %d, len %d, channel %d\n\r", busType, pins[0], len, channel);
    #endif
    
    void* busPtr = nullptr;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: busPtr = new PIXELBUS_8266_U0_3(len, pins[0]); break;
      case BUSTYPE__8266_U1_3__ID: busPtr = new PIXELBUS_8266_U1_3(len, pins[0]); break;
      case BUSTYPE__8266_DM_3__ID: busPtr = new PIXELBUS_8266_DM_3(len, pins[0]); break;
      case BUSTYPE__8266_U0_4__ID: busPtr = new PIXELBUS_8266_U0_4(len, pins[0]); break;
      case BUSTYPE__8266_U1_4__ID: busPtr = new PIXELBUS_8266_U1_4(len, pins[0]); break;
      case BUSTYPE__8266_DM_4__ID: busPtr = new PIXELBUS_8266_DM_4(len, pins[0]); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: busPtr = new PIXELBUS_32_RN_3(len, pins[0], (NeoBusChannel)channel); break;
      case BUSTYPE__32_RN_4__ID: busPtr = new PIXELBUS_32_RN_4(len, pins[0], (NeoBusChannel)channel); break;
      case BUSTYPE__32_RN_5__ID:  busPtr = new PIXELBUS_32_RN_5(len, pins[0]); break;
      case BUSTYPE__32_RN_400_3__ID: busPtr = new PIXELBUS_32_RN_400_3(len, pins[0], (NeoBusChannel)channel); break;     
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: busPtr = new PIXELBUS_32_I0_3(len, pins[0]); break;
      case BUSTYPE__32_I0_4__ID: busPtr = new PIXELBUS_32_I0_4(len, pins[0]); break;
      case BUSTYPE__32_I0_5__ID: busPtr = new PIXELBUS_32_I0_5(len, pins[0]); break;
      case BUSTYPE__32_I0_400_3__ID: busPtr = new PIXELBUS_32_I0_400_3(len, pins[0]); break;
      case BUSTYPE__32_I0_3P__ID: busPtr = new PIXELBUS_32_I0_3P(len, pins[0]); break;
      case BUSTYPE__32_I0_4P__ID: busPtr = new PIXELBUS_32_I0_4P(len, pins[0]); break;
      case BUSTYPE__32_I0_5P__ID: busPtr = new PIXELBUS_32_I0_5P(len, pins[0]); break;
      #endif      
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: busPtr = new PIXELBUS_32_I1_3(len, pins[0]); break;
      case BUSTYPE__32_I1_4__ID: busPtr = new PIXELBUS_32_I1_4(len, pins[0]); break;
      case BUSTYPE__32_I1_5__ID: busPtr = new PIXELBUS_32_I1_5(len, pins[0]); break;
      case BUSTYPE__32_I1_400_3__ID: busPtr = new PIXELBUS_32_I1_400_3(len, pins[0]); break;
      case BUSTYPE__32_I1_3P__ID: busPtr = new PIXELBUS_32_I1_3P(len, pins[0]); break;
      case BUSTYPE__32_I1_4P__ID: busPtr = new PIXELBUS_32_I1_4P(len, pins[0]); break;
      case BUSTYPE__32_I1_5P__ID: busPtr = new PIXELBUS_32_I1_5P(len, pins[0]); break;
      #endif
    #endif
    }
      
    begin(busPtr, busType, pins);
    
    return busPtr;
  };

  static void show(void* busPtr, uint8_t busType, bool consistent = true) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);
    #endif 
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->Show(); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->Show(); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->Show(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: (static_cast<PIXELBUS_32_RN_3*>(busPtr))->Show();   
      case BUSTYPE__32_RN_4__ID: (static_cast<PIXELBUS_32_RN_4*>(busPtr))->Show(); break;  
      case BUSTYPE__32_RN_5__ID: (static_cast<PIXELBUS_32_RN_5*>(busPtr))->Show(); break;
      case BUSTYPE__32_RN_400_3__ID: (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: (static_cast<PIXELBUS_32_I0_3*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_4__ID: (static_cast<PIXELBUS_32_I0_4*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_5__ID: (static_cast<PIXELBUS_32_I0_5*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_400_3__ID: (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_3P__ID: (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_4P__ID: (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->Show(); break;
      case BUSTYPE__32_I0_5P__ID: (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->Show(); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->Show(); break;
      #endif
    #endif
    }

  };


  static bool canShow(void* busPtr, uint8_t busType) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::canShow busType %d\n\r", busType);
    #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: return true;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: return (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_3__ID: return (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_3__ID: return (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_BB_3__ID: return (static_cast<PIXELBUS_8266_BB_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U0_4__ID: return (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_U1_4__ID: return (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__8266_DM_4__ID: return (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->CanShow(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: return (static_cast<PIXELBUS_32_RN_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_RN_4__ID: return (static_cast<PIXELBUS_32_RN_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_RN_5__ID:  return (static_cast<PIXELBUS_32_RN_5*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_RN_400_3__ID: return (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: return (static_cast<PIXELBUS_32_I0_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_4__ID: return (static_cast<PIXELBUS_32_I0_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_5__ID: return (static_cast<PIXELBUS_32_I0_5*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_400_3__ID: return (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_3P__ID: return (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_4P__ID: return (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I0_5P__ID: return (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: return (static_cast<PIXELBUS_32_I1_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_4__ID: return (static_cast<PIXELBUS_32_I1_4*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_5__ID: return (static_cast<PIXELBUS_32_I1_5*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_400_3__ID: return (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_3P__ID: return (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_4P__ID: return (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->CanShow(); break;
      case BUSTYPE__32_I1_5P__ID: return (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->CanShow(); break;
      #endif
    #endif

    }
    return true;
  };

  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  // Function to set pixel color based on color ordering
  static void setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, RgbwwColor c, uint8_t co, uint16_t wwcw = 0)
  {
    
    #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
    Serial.printf("rgbww function %d,%d,%d\n\r", c.R, c.G, c.B, c.WW, c.CW); Serial.flush();
    #endif

  // DEBUG_LINE_HERE;
    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // DEBUG_TIME__START
    // uint32_t __debug_time_start__ = micros();
    // #endif

    // Debug feature to map a large number of virtual pixels to a smaller physical display
    #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
    pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer pixels
    #endif

    RgbwwColor col = c;

    // Reorder RGB channels based on the provided color order
    switch (co & COLOUR_ORDER_RGB_MASK) {
      default: break;                                      // Default: GRB
      case 1:  col.G = c.R; col.R = c.G; break;            // RGB
      case 2:  col.G = c.B; col.B = c.G; break;            // BRG
      case 3:  col.G = c.R; col.R = c.B; c.B = c.G; break; // RBG
      case 4:  col.G = c.B; col.R = c.G; c.B = c.R; break; // BGR
      case 5:  col.R = c.B; col.B = c.R; break;            // GBR
    }

    // Handle white channel reordering or swapping
    switch (co >> COLOUR_ORDER_WHITE_MASK) {
      default: break;                 // Default: no swap
      case 4: std::swap(col.WW, col.CW); break; // Swap WW and CW
    }

    // Optional logging for debugging
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix == 0){ // Just log for the first pixel
        Serial.printf("set colour R=%d, G=%d, B=%d, CW=%d, WW=%d %d/%d/%d/%d/%d\n\r",
            (colour_order & 0x07),   // Red
            ((colour_order >> 3) & 0x07), // Green
            ((colour_order >> 6) & 0x07), // Blue
            colour_hardware.CW,
            colour_hardware.WW,
            colour_internal.R,
            colour_internal.G,
            colour_internal.B,
            colour_internal.CW,
            colour_internal.WW
        );
    }
    #endif    
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS // Debug pixel color value log
    if (pix < 1) { // Just log for the first pixel
        Serial.printf("Polybus::setPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", pix, 
            col.R,col.G,col.B,col.WW,col.CW
        );
    }
    #endif
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->SetPixelColor(pix, col); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID:      (static_cast<PIXELBUS_32_RN_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_RN_4__ID:      (static_cast<PIXELBUS_32_RN_4*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_RN_5__ID:   (static_cast<PIXELBUS_32_RN_5*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      case BUSTYPE__32_RN_400_3__ID:  (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID:      (static_cast<PIXELBUS_32_I0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4__ID:      (static_cast<PIXELBUS_32_I0_4*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I0_5__ID:      (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I0_400_3__ID:  (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_3P__ID:     (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4P__ID:     (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I0_5P__ID:     (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->SetPixelColor(pix, col); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->SetPixelColor(pix, RgbwColor(col)); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->SetPixelColor(pix, col); break;
      #endif
    #endif

    }

    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // #define SERIAL_DEBUG Serial
    // if(pix==0)
    // SERIAL_DEBUG.printf("(%s:%d) %luus %dms\n\r", \
    //                         __FILE__, __LINE__, micros() - __debug_time_start__, (micros() - __debug_time_start__)/1000); \
    //     SERIAL_DEBUG.flush();
    // #endif
}


static RgbwwColor getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint8_t co)
{
  RgbwwColor col;

  #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
  // Modulo operation for debugging larger pixel counts on a smaller display
  pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer physical pixels
  #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: col = (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_3__ID: col = (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_3__ID: col = (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U0_4__ID: col = (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_4__ID: col = (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_4__ID: col = (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: col = (static_cast<PIXELBUS_32_RN_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_4__ID: col = (static_cast<PIXELBUS_32_RN_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_5__ID:  col = (static_cast<PIXELBUS_32_RN_5*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_400_3__ID: col = (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: col = (static_cast<PIXELBUS_32_I0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4__ID: col = (static_cast<PIXELBUS_32_I0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5__ID: col = (static_cast<PIXELBUS_32_I0_5*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_400_3__ID: col = (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_3P__ID: col = (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4P__ID: col = (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5P__ID: col = (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: col = (static_cast<PIXELBUS_32_I1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4__ID: col = (static_cast<PIXELBUS_32_I1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5__ID: col = (static_cast<PIXELBUS_32_I1_5*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_400_3__ID: col = (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_3P__ID: col = (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4P__ID: col = (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5P__ID: col = (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->GetPixelColor(pix); break;
      #endif
    #endif
    }

    // lower nibble contains RGB swap information
    switch (co & COLOUR_ORDER_RGB_MASK) {
      case 1: std::swap(col.R, col.G); break; // Swap Red and Green
      case 2: std::swap(col.R, col.B); break; // Swap Red and Blue
      case 3: std::swap(col.G, col.B); break; // Swap Green and Blue
      default: break; // No swap
    }

    // upper nibble contains W swap information
   
    switch (co >> COLOUR_ORDER_WHITE_MASK) {
      case 1: std::swap(col.WW, col.CW); break;
      default: break;
    }

    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if (pix < 5) { // Just first few pixels
      Serial.printf("get colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
        rgb_order,
        white_order,
        color_internal.R,
        color_internal.G,
        color_internal.B,
        color_internal.CW,
        color_internal.WW
      );
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    return col;
}


  #else

  // Function to set pixel color based on color ordering
  static void setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint32_t c, uint8_t co, uint16_t wwcw = 0)
  {

    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // DEBUG_TIME__START
    // uint32_t __debug_time_start__ = micros();
    // #endif

    // Debug feature to map a large number of virtual pixels to a smaller physical display
    #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
    pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer pixels
    #endif

    uint8_t r = c >> 16;
    uint8_t g = c >> 8;
    uint8_t b = c >> 0;
    uint8_t w = c >> 24;
    RgbwColor col;
    uint8_t cctWW = wwcw & 0xFF, cctCW = (wwcw>>8) & 0xFF;

    // reorder channels to selected order
    switch (co & COLOUR_ORDER_RGB_MASK) {
      default: col.G = g; col.R = r; col.B = b; break; //0 = GRB, default
      case  1: col.G = r; col.R = g; col.B = b; break; //1 = RGB, common for WS2811
      case  2: col.G = b; col.R = r; col.B = g; break; //2 = BRG
      case  3: col.G = r; col.R = b; col.B = g; break; //3 = RBG
      case  4: col.G = b; col.R = g; col.B = r; break; //4 = BGR
      case  5: col.G = g; col.R = b; col.B = r; break; //5 = GBR
    }
    // upper nibble contains W swap information
    switch (co >> COLOUR_ORDER_WHITE_MASK) {
      default: col.W = w;                break; // no swapping
      case  1: col.W = col.B; col.B = w; break; // swap W & B
      case  2: col.W = col.G; col.G = w; break; // swap W & G
      case  3: col.W = col.R; col.R = w; break; // swap W & R
      case  4: std::swap(cctWW, cctCW);  break; // swap WW & CW
    }

    // Optional logging for debugging
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if(pix == 0){ // Just log for the first pixel
        Serial.printf("set colour R=%d, G=%d, B=%d, CW=%d, WW=%d %d/%d/%d/%d/%d\n\r",
            (colour_order & 0x07),   // Red
            ((colour_order >> 3) & 0x07), // Green
            ((colour_order >> 6) & 0x07), // Blue
            colour_hardware.CW,
            colour_hardware.WW,
            colour_internal.R,
            colour_internal.G,
            colour_internal.B,
            colour_internal.CW,
            colour_internal.WW
        );
    }
    #endif    
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS // Debug pixel color value log
    if (pix < 1) { // Just log for the first pixel
        Serial.printf("setPixelColor[%d] R=%d, G=%d, B=%d, WW=%d, CW=%d\n\r", pix, 
            colour_internal.R, 
            colour_internal.G, 
            colour_internal.B, 
            colour_internal.WW, 
            colour_internal.CW
        );
    }
    #endif
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->SetPixelColor(pix, col); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID:      (static_cast<PIXELBUS_32_RN_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_RN_4__ID:      (static_cast<PIXELBUS_32_RN_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_RN_5__ID:   (static_cast<PIXELBUS_32_RN_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      case BUSTYPE__32_RN_400_3__ID:  (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID:      (static_cast<PIXELBUS_32_I0_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4__ID:      (static_cast<PIXELBUS_32_I0_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I0_5__ID:      (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      case BUSTYPE__32_I0_400_3__ID:  (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_3P__ID:     (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I0_4P__ID:     (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I0_5P__ID:     (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->SetPixelColor(pix, RgbColor(col)); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->SetPixelColor(pix, col); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->SetPixelColor(pix, RgbwwColor(col)); break;
      #endif
    #endif

    }

    // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
    // #define SERIAL_DEBUG Serial
    // if(pix==0)
    // SERIAL_DEBUG.printf("(%s:%d) %luus %dms\n\r", \
    //                         __FILE__, __LINE__, micros() - __debug_time_start__, (micros() - __debug_time_start__)/1000); \
    //     SERIAL_DEBUG.flush();
    // #endif
}


static uint32_t getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint8_t co)
{
  RgbwColor col(0,0,0,0);

  #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
  // Modulo operation for debugging larger pixel counts on a smaller display
  pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer physical pixels
  #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: col = (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_3__ID: col = (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_3__ID: col = (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U0_4__ID: col = (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_U1_4__ID: col = (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__8266_DM_4__ID: col = (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: col = (static_cast<PIXELBUS_32_RN_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_4__ID: col = (static_cast<PIXELBUS_32_RN_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__32_RN_5__ID:  col = (static_cast<PIXELBUS_32_RN_5*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_RN_400_3__ID: col = (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: col = (static_cast<PIXELBUS_32_I0_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4__ID: col = (static_cast<PIXELBUS_32_I0_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I0_5*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      case BUSTYPE__32_I0_400_3__ID: col = (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_3P__ID: col = (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_4P__ID: col = (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I0_5P__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: col = (static_cast<PIXELBUS_32_I1_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4__ID: col = (static_cast<PIXELBUS_32_I1_4*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I1_5*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      case BUSTYPE__32_I1_400_3__ID: col = (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_3P__ID: col = (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_4P__ID: col = (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->GetPixelColor(pix); break;
      case BUSTYPE__32_I1_5P__ID: { RgbwwColor c = (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->GetPixelColor(pix); col = RGBW32(c.R,c.G,c.B,max(c.WW,c.CW)); } break; // will not return original W
      #endif
    #endif
    }

    // upper nibble contains W swap information
    uint8_t w = col.W;
    switch (co >> COLOUR_ORDER_WHITE_MASK) {
      case 1: col.W = col.B; col.B = w; break; // swap W & B
      case 2: col.W = col.G; col.G = w; break; // swap W & G
      case 3: col.W = col.R; col.R = w; break; // swap W & R
    }
    switch (co & COLOUR_ORDER_RGB_MASK) {
      //                    W               G              R               B
      default: return ((col.W << 24) | (col.G << 8) | (col.R << 16) | (col.B)); //0 = GRB, default
      case  1: return ((col.W << 24) | (col.R << 8) | (col.G << 16) | (col.B)); //1 = RGB, common for WS2811
      case  2: return ((col.W << 24) | (col.B << 8) | (col.R << 16) | (col.G)); //2 = BRG
      case  3: return ((col.W << 24) | (col.B << 8) | (col.G << 16) | (col.R)); //3 = RBG
      case  4: return ((col.W << 24) | (col.R << 8) | (col.B << 16) | (col.G)); //4 = BGR
      case  5: return ((col.W << 24) | (col.G << 8) | (col.B << 16) | (col.R)); //5 = GBR
    }

    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    if (pix < 5) { // Just first few pixels
      Serial.printf("get colour_order R=%d, G=%d, B=%d, CW=%d, WW=%d\n\r",
        rgb_order,
        white_order,
        color_internal.R,
        color_internal.G,
        color_internal.B,
        color_internal.CW,
        color_internal.WW
      );
    }
    #endif // ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    return 0;
}

#endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  static void setBrightness(void* busPtr, uint8_t busType, uint8_t b) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);
    #endif 
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      case BUSTYPE__8266_U0_3__ID: (static_cast<PIXELBUS_8266_U0_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_U1_3__ID: (static_cast<PIXELBUS_8266_U1_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_DM_3__ID: (static_cast<PIXELBUS_8266_DM_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_U0_4__ID: (static_cast<PIXELBUS_8266_U0_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_U1_4__ID: (static_cast<PIXELBUS_8266_U1_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__8266_DM_4__ID: (static_cast<PIXELBUS_8266_DM_4*>(busPtr))->SetLuminance(b); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: (static_cast<PIXELBUS_32_RN_3*>(busPtr))->SetLuminance(b);   
      case BUSTYPE__32_RN_4__ID: (static_cast<PIXELBUS_32_RN_4*>(busPtr))->SetLuminance(b); break;  
      case BUSTYPE__32_RN_5__ID: (static_cast<PIXELBUS_32_RN_5*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_RN_400_3__ID: (static_cast<PIXELBUS_32_RN_400_3*>(busPtr))->SetLuminance(b); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: (static_cast<PIXELBUS_32_I0_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_4__ID: (static_cast<PIXELBUS_32_I0_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_5__ID: (static_cast<PIXELBUS_32_I0_5*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_400_3__ID: (static_cast<PIXELBUS_32_I0_400_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_3P__ID: (static_cast<PIXELBUS_32_I0_3P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_4P__ID: (static_cast<PIXELBUS_32_I0_4P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I0_5P__ID: (static_cast<PIXELBUS_32_I0_5P*>(busPtr))->SetLuminance(b); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: (static_cast<PIXELBUS_32_I1_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_4__ID: (static_cast<PIXELBUS_32_I1_4*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_5__ID: (static_cast<PIXELBUS_32_I1_5*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_400_3__ID: (static_cast<PIXELBUS_32_I1_400_3*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_3P__ID: (static_cast<PIXELBUS_32_I1_3P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_4P__ID: (static_cast<PIXELBUS_32_I1_4P*>(busPtr))->SetLuminance(b); break;
      case BUSTYPE__32_I1_5P__ID: (static_cast<PIXELBUS_32_I1_5P*>(busPtr))->SetLuminance(b); break;
      #endif
    #endif
    }

  };



  static void cleanup(void* busPtr, uint8_t busType) 
  {

    // DEBUG_PRINTF("PolyBus::cleanup busType %d\n\r", busType);
    
    if (busPtr == nullptr) return;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266    
      case BUSTYPE__8266_U0_3__ID: delete (static_cast<PIXELBUS_8266_U0_3*>(busPtr)); break;
      case BUSTYPE__8266_U1_3__ID: delete (static_cast<PIXELBUS_8266_U1_3*>(busPtr)); break;
      case BUSTYPE__8266_DM_3__ID: delete (static_cast<PIXELBUS_8266_DM_3*>(busPtr)); break;
      case BUSTYPE__8266_U0_4__ID: delete (static_cast<PIXELBUS_8266_U0_4*>(busPtr)); break;
      case BUSTYPE__8266_U1_4__ID: delete (static_cast<PIXELBUS_8266_U1_4*>(busPtr)); break;
      case BUSTYPE__8266_DM_4__ID: delete (static_cast<PIXELBUS_8266_DM_4*>(busPtr)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      case BUSTYPE__32_RN_3__ID: delete (static_cast<PIXELBUS_32_RN_3*>(busPtr)); break;
      case BUSTYPE__32_RN_4__ID: delete (static_cast<PIXELBUS_32_RN_4*>(busPtr)); break;
      // case BUSTYPE__32_RN_5__ID: delete (static_cast<PIXELBUS_32_RN_5*>(busPtr)); break
      case BUSTYPE__32_RN_400_3__ID: delete (static_cast<PIXELBUS_32_RN_400_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_3__ID: delete (static_cast<PIXELBUS_32_I0_3*>(busPtr)); break;
      case BUSTYPE__32_I0_4__ID: delete (static_cast<PIXELBUS_32_I0_4*>(busPtr)); break;
      case BUSTYPE__32_I0_5__ID: delete (static_cast<PIXELBUS_32_I0_5*>(busPtr)); break;
      case BUSTYPE__32_I0_400_3__ID: delete (static_cast<PIXELBUS_32_I0_400_3*>(busPtr)); break;
      case BUSTYPE__32_I0_3P__ID: delete (static_cast<PIXELBUS_32_I0_3P*>(busPtr)); break;
      case BUSTYPE__32_I0_4P__ID: delete (static_cast<PIXELBUS_32_I0_4P*>(busPtr)); break;
      case BUSTYPE__32_I0_5P__ID: delete (static_cast<PIXELBUS_32_I0_5P*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_3__ID: delete (static_cast<PIXELBUS_32_I1_3*>(busPtr)); break;
      case BUSTYPE__32_I1_4__ID: delete (static_cast<PIXELBUS_32_I1_4*>(busPtr)); break;
      case BUSTYPE__32_I1_5__ID: delete (static_cast<PIXELBUS_32_I1_5*>(busPtr)); break;
      case BUSTYPE__32_I1_400_3__ID: delete (static_cast<PIXELBUS_32_I1_400_3*>(busPtr)); break;
      case BUSTYPE__32_I1_3P__ID: delete (static_cast<PIXELBUS_32_I1_3P*>(busPtr)); break;
      case BUSTYPE__32_I1_4P__ID: delete (static_cast<PIXELBUS_32_I1_4P*>(busPtr)); break;
      case BUSTYPE__32_I1_5P__ID: delete (static_cast<PIXELBUS_32_I1_5P*>(busPtr)); break;
      #endif
    #endif
    }
  }

  //gives back the internal type index (I_XX_XXX_X above) for the input 
  static    
  #ifdef USE_DEVFEATURE_IRAM__PIXEL_BUS_INTERFACING
  IRAM_ATTR
  #endif 
  uint8_t getI(uint8_t busType, uint8_t* pins, uint8_t num = 0) 
  {
    // DEBUG_PRINTF("PolyBus::getI busType %d\n\r", busType);

    if (!IS_BUSTYPE_DIGITAL(busType)) return BUSTYPE__NONE__ID;

    if (IS_BUSTYPE_2PIN(busType)) { //SPI LED chips
        bool isHSPI = false;

        #ifdef ESP8266
            if (pins[0] == P_8266_HS_MOSI && pins[1] == P_8266_HS_CLK) isHSPI = true;
        #else
            // temporary hack to limit use of hardware SPI to a single SPI peripheral (HSPI): 
            // only allow ESP32 hardware serial on segment 0
            // SPI global variable is normally linked to VSPI on ESP32 (or FSPI C3, S3)
            if (!num) isHSPI = true;
        #endif

        uint8_t t = BUSTYPE__NONE__ID;
        switch (busType) {
            case BUSTYPE_APA102:  t = BUSTYPE__SS_DOT_3__ID; break;
            case BUSTYPE_LPD8806: t = BUSTYPE__SS_LPD_3__ID; break;
            case BUSTYPE_LPD6803: t = BUSTYPE__SS_LPO_3__ID; break;
            case BUSTYPE_WS2801:  t = BUSTYPE__SS_WS1_3__ID; break;
            case BUSTYPE_P9813:   t = BUSTYPE__SS_P98_3__ID; break;
            default: t = BUSTYPE__NONE__ID;
        }
        if (t > BUSTYPE__NONE__ID && isHSPI) t--; //hardware SPI has one smaller ID than software
        return t;
    } else {
        #ifdef ESP8266
            uint8_t offset_method_inside_group = pins[0] - 1; // for driver: 0 = uart0, 1 = uart1, 2 = dma, 3 = bitbang
            if (offset_method_inside_group > 3) offset_method_inside_group = 3;

            switch (busType) {
                case BUSTYPE_WS2812_RGB:
                case BUSTYPE_WS2812_WWA:
                    return BUSTYPE__8266_U0_3__ID + offset_method_inside_group;
                case BUSTYPE_SK6812_RGBW:
                    return BUSTYPE__8266_U0_4__ID + offset_method_inside_group;
            }
        #else //ESP32
            uint8_t offset_method_inside_group = 0;

            #if defined(CONFIG_IDF_TARGET_ESP32S2)
                // ESP32-S2 only has 4 RMT channels
                if (num > 4) return I_NONE;
                if (num > 3) offset_method_inside_group = 1;  // only one I2S

            #elif defined(CONFIG_IDF_TARGET_ESP32C3)
                // On ESP32-C3 only the first 2 RMT channels are usable for transmitting
                if (num > 1) return I_NONE;
                //if (num > 1) offset_method_inside_group = 1; // I2S not supported yet (only 1 I2S)

            #elif defined(CONFIG_IDF_TARGET_ESP32S3)
                // On ESP32-S3 only the first 4 RMT channels are usable for transmitting
                if (num > 3) return I_NONE;
                //if (num > 3) offset_method_inside_group = num -4; // I2S not supported yet

            #else

                #ifdef ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

                  if(useParallelI2S)
                  {
                    Serial.println("BUS DETECT: useParallelI2S");
                    Serial.printf("required_channels %d %d\n\r", required_channels, num);
                    if(required_channels <= 8 && num < 8)
                    { 
                      offset_method_inside_group = 3;  // Handled inside library automatically for I2S1 types
                    }else
                    if(required_channels <= 16 && num < 16)
                    {
                      offset_method_inside_group = 4;  // Handled inside library automatically for I2S1 types
                    }
                    else {
                      return BUSTYPE__NONE__ID;
                    }
                    Serial.printf("offset_method_inside_group %d\n\r", offset_method_inside_group);
                    
                  }
                  else // I0, I1, RMT0 to RMT7
                  {
                    Serial.printf("BUS DETECT: No Parallel %d\n\r", num);
                    if (num < 2) {
                      offset_method_inside_group = num + 1;
                    } else if (num < 9) {
                      offset_method_inside_group = num - 7;
                    } else {
                      return BUSTYPE__NONE__ID;
                    }
                  }

                #else
                    #if defined(ENABLE_PIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS)
                        if (num < 2) {
                            offset_method_inside_group = num + 1;  // To skip that RMT was entered first in enum
                        } else if (num < 9) {
                            offset_method_inside_group = num - 7;
                        } else {
                            return BUSTYPE__NONE__ID;
                        }
                    #elif defined(ENABLE_PIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE)
                        if (num < 8) {
                            offset_method_inside_group = 4;  // Handled inside library automatically for I2S1 types
                        } else {
                            return BUSTYPE__NONE__ID;
                        }
                    #elif defined(ENABLE_PIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE)
                        if (num < 16) {
                            offset_method_inside_group = 5;  // Handled inside library automatically for I2S1 types
                        } else {
                            return BUSTYPE__NONE__ID;
                        }
                    #elif defined(ENABLE_PIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS)
                        if (num > 9) {
                            return BUSTYPE__NONE__ID;
                        }
                        if (num > 7) {
                            offset_method_inside_group = num - 7;
                        }
                        #warning "RMT methods cause flickering on ESP32, use I2S methods instead -- needs debugging"
                    #else
                    #error "2024: No method defined"
                    #endif
                #endif // ENABLE_PIXELBUS_BUSMETHODS__I2S_AUTO_CHANNEL_SWITCHING

            #endif

            switch (busType) {
                case BUSTYPE_WS2812_RGB:
                case BUSTYPE_WS2812_WWA:
                    return BUSTYPE__32_RN_3__ID + offset_method_inside_group;
                case BUSTYPE_SK6812_RGBW:
                    return BUSTYPE__32_RN_4__ID + offset_method_inside_group;
                case BUSTYPE_WS2805_RGBWW:
                    return BUSTYPE__32_RN_5__ID + offset_method_inside_group;
                case BUSTYPE_WS2811_400KHZ:
                    return BUSTYPE__32_RN_400_3__ID + offset_method_inside_group;
            }

        #endif
    }

    return BUSTYPE__NONE__ID;
}

};

#endif // USE_MODULE_LIGHTS_ANIMATOR

#endif // guard
