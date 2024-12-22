#ifndef BusWrapper_h
#define BusWrapper_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#include "NeoPixelBus.h"

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

enum EM_BUS_TYPE
{
  BUSTYPE__NONE__ID = 0,  
  /*************************** ESP8266 Neopixel methods ***/
  // RGB
  BUSTYPE__8266_U0_NEO_3__ID = 1,
  BUSTYPE__8266_U1_NEO_3__ID = 2,
  BUSTYPE__8266_DM_NEO_3__ID = 3,
  // RGBW
  BUSTYPE__8266_U0_NEO_4__ID = 5,
  BUSTYPE__8266_U1_NEO_4__ID = 6,
  BUSTYPE__8266_DM_NEO_4__ID = 7,  
  /**************************** ESP32 Neopixel methods ***/
  // RGB
  BUSTYPE__32_RN_NEO_3__ID = 23,
  BUSTYPE__32_I0_NEO_3__ID,
  BUSTYPE__32_I1_NEO_3__ID,
  BUSTYPE__32_8PARALELL_3__ID,
  BUSTYPE__32_16PARALLEL_3__ID,
  // RGBW
  BUSTYPE__32_RN_NEO_4__ID,
  BUSTYPE__32_I0_NEO_4__ID,
  BUSTYPE__32_I1_NEO_4__ID,
  BUSTYPE__32_8PARALELL_4__ID,
  BUSTYPE__32_16PARALLEL_4__ID,
  // RGBWW
  BUSTYPE__32_RN_NEO_5__ID, 
  BUSTYPE__32_I0_NEO_5__ID,
  BUSTYPE__32_I1_NEO_5__ID,
  BUSTYPE__32_8PARALELL_5__ID,
  BUSTYPE__32_16PARALLEL_5__ID,
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


#define ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
#define ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
#define ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES


// 
// #ifdef ENABLE_DEVFEATURE__NEOPIXELBUS_JULY_ONWARDS

/*** ESP8266 Neopixel methods ***/
#ifdef ESP8266
//RGB
#ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
#define NEOPIXELBUS_8266_U0_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart0Ws2813Method> //3 chan, esp8266, gpio1
#endif
#ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
#define NEOPIXELBUS_8266_U1_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1Ws2813Method> //3 chan, esp8266, gpio2
#endif 
#ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
#define NEOPIXELBUS_8266_DM_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266Dma800KbpsMethod>  //3 chan, esp8266, gpio3
#endif
// #define NEOPIXELBUS_8266_BB_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp8266BitBang800KbpsMethod> //3 chan, esp8266, bb (any pin but 16)
//RGBW
#ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
#define NEOPIXELBUS_8266_U0_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart0Ws2813Method>   //4 chan, esp8266, gpio1
#endif 
#ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
#define NEOPIXELBUS_8266_U1_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Uart1Ws2813Method>   //4 chan, esp8266, gpio2
#endif
#ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
#define NEOPIXELBUS_8266_DM_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp8266Dma800KbpsMethod>    //4 chan, esp8266, gpio3
#endif
#endif

#define ENABLE_DEVFEATURE__WS2805


/*** ESP32 Neopixel methods ***/
#ifdef ARDUINO_ARCH_ESP32
  //RGB
  #define NEOPIXELBUS_32_RN_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32RmtNWs2812xMethod>
  #define NEOPIXELBUS_32_I0_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0800KbpsMethod>
  #define NEOPIXELBUS_32_I1_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1800KbpsMethod>
  #define NEOPIXELBUS_32_8PARALLEL_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1X8Ws2812xMethod>
  #define NEOPIXELBUS_32_16PARALLEL_NEO_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0X16Ws2812xMethod>
  //RGBW
  #define NEOPIXELBUS_32_RN_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32RmtNWs2812xMethod>
  #define NEOPIXELBUS_32_I0_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0800KbpsMethod>
  #define NEOPIXELBUS_32_I1_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s1800KbpsMethod>
  #define NEOPIXELBUS_32_8PARALLEL_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s1X8Sk6812Method>
  #define NEOPIXELBUS_32_16PARALLEL_NEO_4 NeoPixelBus<NeoRgbwFeature, NeoEsp32I2s0X16Sk6812Method>
  //RGBWW (WS2805)
  // #define NEOPIXELBUS_32_RN_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s0Ws2805Method> // No RMT method
  #define NEOPIXELBUS_32_I0_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s0Ws2805Method>
  #define NEOPIXELBUS_32_I1_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s1Ws2805Method>
  #define NEOPIXELBUS_32_8PARALLEL_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s1X8Ws2805Method>
  #define NEOPIXELBUS_32_16PARALLEL_NEO_5 NeoPixelBus<NeoRgbwwFeature, NeoEsp32I2s0X16Ws2805Method>
  //400Kbps
  #define NEOPIXELBUS_32_RN_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32RmtN400KbpsMethod>
  #define NEOPIXELBUS_32_I0_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s0400KbpsMethod>
  #define NEOPIXELBUS_32_I1_400_3 NeoPixelBus<NeoRgbFeature, NeoEsp32I2s1400KbpsMethod>
  //TM1814 (RGBW)
  #define NEOPIXELBUS_32_RN_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp32RmtNTm1814Method>
  #define NEOPIXELBUS_32_I0_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp32I2s0Tm1814Method>
  #define NEOPIXELBUS_32_I1_TM1_4 NeoPixelBus<NeoWrgbTm1814Feature, NeoEsp32I2s1Tm1814Method>
#endif

//APA102
// #define NEOPIXELBUS_HS_DOT_3 NeoPixelBus<DotStarBgrFeature, DotStarSpi5MhzMethod> //hardware SPI
// #define NEOPIXELBUS_SS_DOT_3 NeoPixelBus<DotStarBgrFeature, DotStarMethod>    //soft SPI

// //LPD6803
// #define NEOPIXELBUS_HS_LPO_3 NeoPixelBus<Lpd6803RgbFeature, Lpd6803SpiMethod>
// #define NEOPIXELBUS_SS_LPO_3 NeoPixelBus<Lpd6803RgbFeature, Lpd6803Method>

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



  // Begin & initialize the PixelSettings for TM1814 strips.
  template <class T>
  static void beginTM1814(void* busPtr) {
    T tm1814_strip = static_cast<T>(busPtr);
    tm1814_strip->Begin();
    // Max current for each LED (22.5 mA).
    // tm1814_strip->SetPixelSettings(NeoTm1814Settings(/*R*/225, /*G*/225, /*B*/225, /*W*/225));
  }
  static void begin(void* busPtr, uint8_t busType, uint8_t* pins) 
  {
    
    #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::begin busPtr UNSET, busType %d, pin[0] %d\n\r", busType, pins[0]);
    #endif

    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->Begin(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->Begin(); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->Begin(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->Begin(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->Begin(); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->Begin(); break;
      #endif

      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->Begin(); break;
      #endif

      case BUSTYPE__32_RN_400_3__ID: (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->Begin(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->Begin(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->Begin(); break;
      #endif

      // case BUSTYPE__32_RN_TM1_4__ID: beginTM1814<NEOPIXELBUS_32_RN_TM1_4*>(busPtr); break;
      // case BUSTYPE__32_RN_TM2_3__ID: (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->Begin(); break;
      // #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      // case BUSTYPE__32_I0_TM1_4__ID: beginTM1814<NEOPIXELBUS_32_I0_TM1_4*>(busPtr); break;
      // case BUSTYPE__32_I0_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->Begin(); break;
      // #endif
      // #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      // case BUSTYPE__32_I1_TM1_4__ID: beginTM1814<NEOPIXELBUS_32_I1_TM1_4*>(busPtr); break;
      // case BUSTYPE__32_I1_TM2_3__ID: (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->Begin(); break;
      // #endif
    #endif
    }
  };
  static void* create(uint8_t busType, uint8_t* pins, uint16_t len, uint8_t channel) 
  {
    // #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    DEBUG_PRINTF("PolyBus::create busType %d, pin[0] %d, len %d, channel %d\n\r", busType, pins[0], len, channel);
    // #endif
    
    void* busPtr = nullptr;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_U0_NEO_3(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_U1_NEO_3(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_DM_NEO_3(len, pins[0]); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: busPtr = new NEOPIXELBUS_8266_BB_NEO_3(len, pins[0]); break;
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_U0_NEO_4(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_U1_NEO_4(len, pins[0]); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_DM_NEO_4(len, pins[0]); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: busPtr = new NEOPIXELBUS_8266_BB_NEO_4(len, pins[0]); break;
      // case BUSTYPE__8266_U0_400_3__ID: busPtr = new NEOPIXELBUS_8266_U0_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_U1_400_3__ID: busPtr = new NEOPIXELBUS_8266_U1_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_DM_400_3__ID: busPtr = new NEOPIXELBUS_8266_DM_400_3(len, pins[0]); break;
      // case BUSTYPE__8266_BB_400_3__ID: busPtr = new NEOPIXELBUS_8266_BB_400_3(len, pins[0]); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: busPtr = new NEOPIXELBUS_32_RN_NEO_3(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_3(len, pins[0]); break;
      #endif      
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: busPtr = new NEOPIXELBUS_32_8PARALLEL_NEO_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: busPtr = new NEOPIXELBUS_32_16PARALLEL_NEO_3(len, pins[0]); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: busPtr = new NEOPIXELBUS_32_RN_NEO_4(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: busPtr = new NEOPIXELBUS_32_8PARALLEL_NEO_4(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: busPtr = new NEOPIXELBUS_32_16PARALLEL_NEO_4(len, pins[0]); break;
      #endif

      case BUSTYPE__32_RN_NEO_5__ID: 
      // busPtr = new NEOPIXELBUS_32_RN_NEO_5(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: busPtr = new NEOPIXELBUS_32_I0_NEO_5(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: busPtr = new NEOPIXELBUS_32_I1_NEO_5(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: busPtr = new NEOPIXELBUS_32_8PARALLEL_NEO_5(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: busPtr = new NEOPIXELBUS_32_16PARALLEL_NEO_5(len, pins[0]); break;
      #endif

      case BUSTYPE__32_RN_400_3__ID: busPtr = new NEOPIXELBUS_32_RN_400_3(len, pins[0], (NeoBusChannel)channel); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: busPtr = new NEOPIXELBUS_32_I0_400_3(len, pins[0]); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: busPtr = new NEOPIXELBUS_32_I1_400_3(len, pins[0]); break;
      #endif

      // case BUSTYPE__32_RN_TM1_4__ID: busPtr = new NEOPIXELBUS_32_RN_TM1_4(len, pins[0], (NeoBusChannel)channel); break;
      // case BUSTYPE__32_RN_TM2_3__ID: busPtr = new NEOPIXELBUS_32_RN_TM2_3(len, pins[0], (NeoBusChannel)channel); break;
      // #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      // case BUSTYPE__32_I0_TM1_4__ID: busPtr = new NEOPIXELBUS_32_I0_TM1_4(len, pins[0]); break;
      // case BUSTYPE__32_I0_TM2_3__ID: busPtr = new NEOPIXELBUS_32_I0_TM2_3(len, pins[0]); break;
      // #endif
      // #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      // case BUSTYPE__32_I1_TM1_4__ID: busPtr = new NEOPIXELBUS_32_I1_TM1_4(len, pins[0]); break;
      // case BUSTYPE__32_I1_TM2_3__ID: busPtr = new NEOPIXELBUS_32_I1_TM2_3(len, pins[0]); break;
      // #endif

    #endif
      // // for 2-wire: pins[1] is clk, pins[0] is dat.  begin expects (len, clk, dat)
      // case BUSTYPE__HS_DOT_3__ID: busPtr = new NEOPIXELBUS_HS_DOT_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_DOT_3__ID: busPtr = new NEOPIXELBUS_SS_DOT_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__HS_LPO_3__ID: busPtr = new NEOPIXELBUS_HS_LPO_3(len, pins[1], pins[0]); break;
      // case BUSTYPE__SS_LPO_3__ID: busPtr = new NEOPIXELBUS_SS_LPO_3(len, pins[1], pins[0]); break;
    }
      
    begin(busPtr, busType, pins);
    
    return busPtr;
  };

  static  
  #ifdef USE_DEVFEATURE_IRAM__NEO_PIXEL_BUS_INTERFACING
  IRAM_ATTR
  #endif 
  void show(void* busPtr, uint8_t busType, bool consistent = true) 
  {
    
    // #ifdef ENABLE_DEBUGFEATURE__16PIN_PARALLEL_OUTPUT
    // DEBUG_PRINTF("PolyBus::show busType %d\n\r", busType);
    // #endif 
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->Show(); break;
      #endif      
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->Show(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->Show(); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->Show(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->Show(); break;
      // case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->Show(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->Show();       
      break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->Show(); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->Show(); break;
      #endif

      case BUSTYPE__32_RN_NEO_5__ID: 
      // (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->Show(); break;
      #endif

      case BUSTYPE__32_RN_400_3__ID: (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->Show(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->Show(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->Show(); break;
      #endif

    #endif
    }


  };


  static 
  #ifdef USE_DEVFEATURE_IRAM__NEO_PIXEL_BUS_INTERFACING
  IRAM_ATTR
  #endif 
  bool canShow(void* busPtr, uint8_t busType) 
  {
    
    //DEBUG_PRINTF("PolyBus::canShow busType %d\n\r", busType);

    switch (busType) {
      case BUSTYPE__NONE__ID: return true;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->CanShow(); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: return (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U0_400_3__ID: return (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_U1_400_3__ID: return (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_DM_400_3__ID: return (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->CanShow(); break;
      // case BUSTYPE__8266_BB_400_3__ID: return (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->CanShow(); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: return (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: return (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->CanShow(); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: return (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: return (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->CanShow(); break;
      #endif

      case BUSTYPE__32_RN_NEO_5__ID: 
      // return (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: return (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: return (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: return (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: return (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->CanShow(); break;
      #endif

      case BUSTYPE__32_RN_400_3__ID: return (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->CanShow(); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: return (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->CanShow(); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: return (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->CanShow(); break;
      #endif

    #endif

    }
    return true;
  };


// Define masks for the RGB and white channel bits
#define COLOUR_ORDER_RGB_MASK 0x0F // Lower 4 bits for RGB swapping
#define COLOUR_ORDER_WHITE_MASK 0xF0 // Upper 4 bits for white channel swapping

// Function to set pixel color based on color ordering
static void 
#ifdef USE_DEVFEATURE_IRAM__NEO_PIXEL_BUS_INTERFACING
IRAM_ATTR
#endif  
setPixelColor(void* busPtr, uint8_t busType, uint16_t pix, RgbcctColor colour_internal, uint8_t colour_order = 0x00) 
{

  // #ifdef ENABLE_DEVFEATURE_LIGHTING__TEMPORARY_DISABLE_CODE_FOR_SPEED_TESTING
  // DEBUG_TIME__START
  // uint32_t __debug_time_start__ = micros();
  // #endif

    RgbcctColor colour_hardware = colour_internal; // Start with original

  // Debug feature to map a large number of virtual pixels to a smaller physical display
  #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
  pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer pixels
  #endif

    // Apply RGB reordering based on the lower 4 bits
    switch (colour_order & COLOUR_ORDER_RGB_MASK) { // Lower 4 bits for RGB order
        case 0: /* RGB */ break; // Already RGB, no changes needed
        case 1: // GRB
            colour_hardware.R = colour_internal.G;
            colour_hardware.G = colour_internal.R;
            break;
        case 2: // BRG
            colour_hardware.R = colour_internal.B;
            colour_hardware.B = colour_internal.R;
            break;
        case 3: // BGR
            colour_hardware.R = colour_internal.B;
            colour_hardware.G = colour_internal.G;
            colour_hardware.B = colour_internal.R;
            break;
        case 4: // GBR
            colour_hardware.R = colour_internal.G;
            colour_hardware.B = colour_internal.R;
            break;
        case 5: // RBG
            colour_hardware.G = colour_internal.B;
            colour_hardware.B = colour_internal.G;
            break;
        default: /* RGB (case 0) */ break;
    }

    // Apply white channel reordering based on the upper 4 bits
    switch (colour_order & COLOUR_ORDER_WHITE_MASK) {
        case 0x10: // Only W1
            colour_hardware.WW = colour_internal.WW;
            colour_hardware.CW = 0;
            break;
        case 0x20: // Only W2
            colour_hardware.CW = colour_internal.CW;
            colour_hardware.WW = 0;
            break;
        case 0x30: // W1 and W2
            colour_hardware.WW = colour_internal.WW;
            colour_hardware.CW = colour_internal.CW;
            break;
        case 0x40: // W2 and W1 (reverse)
            colour_hardware.WW = colour_internal.CW;
            colour_hardware.CW = colour_internal.WW;
            break;
        default: // No whites
            colour_hardware.WW = 0;
            colour_hardware.CW = 0;
            break;
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

    // Debug pixel color value log
    #ifdef ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS
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

    // Now, set the pixel color based on busType
    
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      // case BUSTYPE__8266_U0_400_3__ID: (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_U1_400_3__ID: (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_DM_400_3__ID: (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
      // case BUSTYPE__8266_BB_400_3__ID: (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->SetPixelColor(pix, RgbColor(colour_hardware.R,colour_hardware.G,colour_hardware.B)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

      case BUSTYPE__32_RN_NEO_5__ID: 
      // (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif

      case BUSTYPE__32_RN_400_3__ID: (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->SetPixelColor(pix, colour_hardware); break;
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


static  
#ifdef USE_DEVFEATURE_IRAM__NEO_PIXEL_BUS_INTERFACING
IRAM_ATTR
#endif 
RgbcctColor getPixelColor(void* busPtr, uint8_t busType, uint16_t pix, uint8_t colour_order) 
{

  #ifdef ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS
  // Modulo operation for debugging larger pixel counts on a smaller display
  pix = pix % ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS; // Map to fewer physical pixels
  #endif

    RgbcctColor col; 
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: col = (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U0_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_U1_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_DM_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__8266_BB_400_3__ID: col = (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr))->GetPixelColor(pix); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: 
      
      // auto p = (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->GetPixelColor(pix);
      
      col = (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: col = (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: col = (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr))->GetPixelColor(pix); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: col = (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: col = (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr))->GetPixelColor(pix); break;
      #endif

#ifdef ENABLE_DEVFEATURE__WS2805
      case BUSTYPE__32_RN_NEO_5__ID: 
      // col = (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: col = (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: col = (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: col = (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: col = (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr))->GetPixelColor(pix); break;
      #endif
#endif // ENABLE_DEVFEATURE__WS2805

      case BUSTYPE__32_RN_400_3__ID: col = (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr))->GetPixelColor(pix); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr))->GetPixelColor(pix); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr))->GetPixelColor(pix); break;
      #endif

      // case BUSTYPE__32_RN_TM1_4__ID: col = (static_cast<NEOPIXELBUS_32_RN_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__32_RN_TM2_3__ID: col = (static_cast<NEOPIXELBUS_32_RN_TM2_3*>(busPtr))->GetPixelColor(pix); break;
      // #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      // case BUSTYPE__32_I0_TM1_4__ID: col = (static_cast<NEOPIXELBUS_32_I0_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__32_I0_TM2_3__ID: col = (static_cast<NEOPIXELBUS_32_I0_TM2_3*>(busPtr))->GetPixelColor(pix); break;
      // #endif
      // #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      // case BUSTYPE__32_I1_TM1_4__ID: col = (static_cast<NEOPIXELBUS_32_I1_TM1_4*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__32_I1_TM2_3__ID: col = (static_cast<NEOPIXELBUS_32_I1_TM2_3*>(busPtr))->GetPixelColor(pix); break;
      // #endif

    #endif

      // case BUSTYPE__HS_DOT_3__ID: col = (static_cast<NEOPIXELBUS_HS_DOT_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_DOT_3__ID: col = (static_cast<NEOPIXELBUS_SS_DOT_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__HS_LPO_3__ID: col = (static_cast<NEOPIXELBUS_HS_LPO_3*>(busPtr))->GetPixelColor(pix); break;
      // case BUSTYPE__SS_LPO_3__ID: col = (static_cast<NEOPIXELBUS_SS_LPO_3*>(busPtr))->GetPixelColor(pix); break;
    }

    // Extract RGB and white channel order from the 8-bit colour_order value
    uint8_t rgb_order = colour_order & 0x0F;      // Lower 4 bits for RGB
    uint8_t white_order = (colour_order >> 4) & 0x0F; // Upper 4 bits for white channels

    // Reorder RGB channels based on the rgb_order
    RgbcctColor color_internal;
    switch (rgb_order) {
      case 0:  // RGB
        color_internal.R = col.R;
        color_internal.G = col.G;
        color_internal.B = col.B;
        break;
      case 1:  // GRB
        color_internal.R = col.G;
        color_internal.G = col.R;
        color_internal.B = col.B;
        break;
      case 2:  // BRG
        color_internal.R = col.B;
        color_internal.G = col.R;
        color_internal.B = col.G;
        break;
      case 3:  // BGR
        color_internal.R = col.B;
        color_internal.G = col.G;
        color_internal.B = col.R;
        break;
      case 4:  // GBR
        color_internal.R = col.G;
        color_internal.G = col.B;
        color_internal.B = col.R;
        break;
      case 5:  // RBG
        color_internal.R = col.R;
        color_internal.G = col.B;
        color_internal.B = col.G;
        break;
      default:  // Default to RGB
        color_internal.R = col.R;
        color_internal.G = col.G;
        color_internal.B = col.B;
        break;
    }

    // Handle white channel behavior based on the white_order
    switch (white_order) {
      case 1:  // Only W1
        color_internal.WW = col.WW;
        break;
      case 2:  // W1 and W2
        color_internal.WW = col.WW;
        color_internal.CW = col.CW;
        break;
      case 3:  // W2 and W1 (swapped)
        color_internal.WW = col.CW;
        color_internal.CW = col.WW;
        break;
      default:  // No white channels
        color_internal.WW = 0;
        color_internal.CW = 0;
        break;
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

    return color_internal;
}



  static void cleanup(void* busPtr, uint8_t busType) 
  {

    // DEBUG_PRINTF("PolyBus::cleanup busType %d\n\r", busType);
    
    if (busPtr == nullptr) return;
    switch (busType) {
      case BUSTYPE__NONE__ID: break;
    #ifdef ESP8266
    
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_NEO_3*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_NEO_3*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_NEO_3*>(busPtr)); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_NEO_3*>(busPtr)); break;
      #ifdef ENABLE_NEOPIXELBUS_8266_U0_NEO_TYPES
      case BUSTYPE__8266_U0_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_U0_NEO_4*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_U1_NEO_TYPES
      case BUSTYPE__8266_U1_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_U1_NEO_4*>(busPtr)); break;
      #endif
      #ifdef ENABLE_NEOPIXELBUS_8266_DM_NEO_TYPES
      case BUSTYPE__8266_DM_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_DM_NEO_4*>(busPtr)); break;
      #endif
      // case BUSTYPE__8266_BB_NEO_4__ID: delete (static_cast<NEOPIXELBUS_8266_BB_NEO_4*>(busPtr)); break;
      // case BUSTYPE__8266_U0_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_U0_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_U1_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_U1_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_DM_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_DM_400_3*>(busPtr)); break;
      // case BUSTYPE__8266_BB_400_3__ID: delete (static_cast<NEOPIXELBUS_8266_BB_400_3*>(busPtr)); break;
    #endif
    #ifdef ARDUINO_ARCH_ESP32

      case BUSTYPE__32_RN_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_RN_NEO_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_3__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_3__ID: delete (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_3__ID: delete (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_3*>(busPtr)); break;
      #endif

      case BUSTYPE__32_RN_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_RN_NEO_4*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_4__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_4__ID: delete (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_4*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_4__ID: delete (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_4*>(busPtr)); break;
      #endif

      case BUSTYPE__32_RN_NEO_5__ID: 
      // delete (static_cast<NEOPIXELBUS_32_RN_NEO_5*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_NEO_5__ID: delete (static_cast<NEOPIXELBUS_32_I0_NEO_5*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_NEO_5__ID: delete (static_cast<NEOPIXELBUS_32_I1_NEO_5*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_8PARALELL_5__ID: delete (static_cast<NEOPIXELBUS_32_8PARALLEL_NEO_5*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_16PARALLEL_5__ID: delete (static_cast<NEOPIXELBUS_32_16PARALLEL_NEO_5*>(busPtr)); break;
      #endif

      case BUSTYPE__32_RN_400_3__ID: delete (static_cast<NEOPIXELBUS_32_RN_400_3*>(busPtr)); break;
      #ifndef NEOPIXEL_DISABLE_I2S0_PIXELBUS
      case BUSTYPE__32_I0_400_3__ID: delete (static_cast<NEOPIXELBUS_32_I0_400_3*>(busPtr)); break;
      #endif
      #ifndef NEOPIXEL_DISABLE_I2S1_PIXELBUS
      case BUSTYPE__32_I1_400_3__ID: delete (static_cast<NEOPIXELBUS_32_I1_400_3*>(busPtr)); break;
      #endif

    #endif

    }
  }

  //gives back the internal type index (I_XX_XXX_X above) for the input 
  static    
  #ifdef USE_DEVFEATURE_IRAM__NEO_PIXEL_BUS_INTERFACING
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
                    return BUSTYPE__8266_U0_NEO_3__ID + offset_method_inside_group;
                case BUSTYPE_SK6812_RGBW:
                    return BUSTYPE__8266_U0_NEO_4__ID + offset_method_inside_group;
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
                    #if defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS)
                        if (num < 2) {
                            offset_method_inside_group = num + 1;  // To skip that RMT was entered first in enum
                        } else if (num < 9) {
                            offset_method_inside_group = num - 7;
                        } else {
                            return BUSTYPE__NONE__ID;
                        }
                    #elif defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE)
                        if (num < 8) {
                            offset_method_inside_group = 4;  // Handled inside library automatically for I2S1 types
                        } else {
                            return BUSTYPE__NONE__ID;
                        }
                    #elif defined(ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE)
                        if (num < 16) {
                            offset_method_inside_group = 5;  // Handled inside library automatically for I2S1 types
                        } else {
                            return BUSTYPE__NONE__ID;
                        }
                    #elif defined(ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS)
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
                #endif // ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_AUTO_CHANNEL_SWITCHING

            #endif

            switch (busType) {
                case BUSTYPE_WS2812_RGB:
                case BUSTYPE_WS2812_WWA:
                    return BUSTYPE__32_RN_NEO_3__ID + offset_method_inside_group;
                case BUSTYPE_SK6812_RGBW:
                    return BUSTYPE__32_RN_NEO_4__ID + offset_method_inside_group;
                case BUSTYPE_WS2805_RGBWW:
                    return BUSTYPE__32_RN_NEO_5__ID + offset_method_inside_group;
                case BUSTYPE_WS2811_400KHZ:
                    return BUSTYPE__32_RN_400_3__ID + offset_method_inside_group;
                // case BUSTYPE_TM1814:
                //     return BUSTYPE__32_RN_TM1_4__ID + offset_method_inside_group;
                // case BUSTYPE_TM1829:
                //     return BUSTYPE__32_RN_TM2_3__ID + offset_method_inside_group;
            }

        #endif
    }

    return BUSTYPE__NONE__ID;
}

};

#endif // USE_MODULE_LIGHTS_ANIMATOR

#endif // guard
