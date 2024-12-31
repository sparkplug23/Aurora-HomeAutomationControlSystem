/**
 * @file mBusManager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 * @note This file is a derivative of the original WLED project.
 *       The CCT funtionality has been removed, and instead will be handled by effect functions.
 *       The auto white will remain, where the white channel will be adjusted based on the RGB values when desired, but otherwise will be calculated in the effects. 
 * 
 */
#ifndef BusManager_h
#define BusManager_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#include "const.h"
#include "mBusNeoWrapper.h"
#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
#define R(c) (byte((c) >> 16))
#define G(c) (byte((c) >> 8))
#define B(c) (byte(c))
#define W(c) (byte((c) >> 24))

#define GET_BIT(var,bit)    (((var)>>(bit))&0x01)
#define SET_BIT(var,bit)    ((var)|=(uint16_t)(0x0001<<(bit)))
#define UNSET_BIT(var,bit)  ((var)&=(~(uint16_t)(0x0001<<(bit))))

#define NUM_ICS_WS2812_1CH_3X(len)    (((len)+2)/3)   // 1 WS2811 IC controls 3 zones (each zone has 1 LED, W)
#define IC_INDEX_WS2812_1CH_3X(i)     ((i)/3)

#define NUM_ICS_WS2812_2CH_3X(len)    (((len)+1)*2/3) // 2 WS2811 ICs control 3 zones (each zone has 2 LEDs, CW and WW)
#define IC_INDEX_WS2812_2CH_3X(i)     ((i)*2/3)
#define WS2812_2CH_3X_SPANS_2_ICS(i)  ((i)&0x01)    // every other LED zone is on two different ICs

#ifndef LED_MILLIAMPS_DEFAULT
  #define LED_MILLIAMPS_DEFAULT 55    // common WS2812B
#else
  #if LED_MILLIAMPS_DEFAULT < 1 || LED_MILLIAMPS_DEFAULT > 100
   #warning "Unusual LED mA current, overriding with default value."
   #undef LED_MILLIAMPS_DEFAULT
   #define LED_MILLIAMPS_DEFAULT 55
  #endif
#endif


DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_1CH__CTR) "WS2812_1CH";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_1CH_X3__CTR) "WS2812_1CH_X3";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_2CH_X3__CTR) "WS2812_2CH_X3";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_WWA__CTR) "WS2812_WWA";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_RGB__CTR) "WS2812_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__GS8608__CTR) "GS8608";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2811_400KHZ__CTR) "WS2811_400KHZ";
DEFINE_PGM_CTR(PM_BUSTYPE__TM1829__CTR) "TM1829";
DEFINE_PGM_CTR(PM_BUSTYPE__SK6812_RGBW__CTR) "SK6812_RGBW";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2805_RGBWW__CTR) "WS2805_RGBWW";
DEFINE_PGM_CTR(PM_BUSTYPE__TM1814__CTR) "TM1814";
DEFINE_PGM_CTR(PM_BUSTYPE__ONOFF__CTR) "ONOFF";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_1CH__CTR) "ANALOG_1CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_2CH__CTR) "ANALOG_2CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_3CH__CTR) "ANALOG_3CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_4CH__CTR) "ANALOG_4CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_5CH__CTR) "ANALOG_5CH";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2801__CTR) "WS2801";
DEFINE_PGM_CTR(PM_BUSTYPE__APA102__CTR) "APA102";
DEFINE_PGM_CTR(PM_BUSTYPE__LPD8806__CTR) "LPD8806";
DEFINE_PGM_CTR(PM_BUSTYPE__P9813__CTR) "P9813";
DEFINE_PGM_CTR(PM_BUSTYPE__LPD6803__CTR) "LPD6803";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_DDP_RGB__CTR) "NET_DDP_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_E131_RGB__CTR) "NET_E131_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_ARTNET_RGB__CTR) "NET_ARTNET_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_DDP_RGBW__CTR) "NET_DDP_RGBW";
DEFINE_PGM_CTR(PM_BUSTYPE__RESERVED__CTR) "RESERVED";

typedef struct {
  uint8_t id;
  const char *type;
  const char *name;
} LEDType;


/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** ColorOrderMap *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

// Defines an LED Strip and its color ordering.
struct ColorOrderMapEntry 
{
  // Mappings here sets what order to use and between which index range
  uint16_t start;
  uint16_t len;
  uint8_t colorOrder;
};


struct ColorOrderMap 
{
    
  bool add(uint16_t start, uint16_t len, uint8_t colorOrder);

  inline uint8_t count() const { return _mappings.size(); }
  inline void reserve(size_t num) { _mappings.reserve(num); }

  void reset() {
    _mappings.clear();
    _mappings.shrink_to_fit();
  }

  const ColorOrderMapEntry* get(uint8_t n) const {
    if (n >= count()) return nullptr;
    return &(_mappings[n]);
  }

  [[gnu::hot]] uint8_t getPixelColorOrder(uint16_t pix, uint8_t defaultColorOrder) const;

  private:
    std::vector<ColorOrderMapEntry> _mappings;
};

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** Bus: Parent class of BusDigital, BusPwm, and BusNetwork *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

class Bus {
  public:
    Bus(uint8_t type, uint16_t start, uint8_t aw, uint16_t len = 1, bool reversed = false, bool refresh = false)
    : _type(type)
    , _bri(255)
    , _start(start)
    , _len(len)
    , _reversed(reversed)
    , _valid(false)
    , _needsRefresh(refresh)
    , _data(nullptr) // keep data access consistent across all types of buses
    {
      #ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
      _autoWhiteMode = Bus::hasWhite(type) ? aw : RGBW_MODE_MANUAL_ONLY;
      #endif
    };

    virtual ~Bus(){}

    virtual void     show() = 0;
    virtual bool     canShow() const                          { return true; }
    virtual void     setStatusPixel(uint32_t c)                {}
    virtual void     setPixelColor(uint32_t pix, ColourBaseType c) = 0;
    virtual void     setBrightness(uint8_t b)                  { _bri = b; };
    virtual void     setColorOrder(uint8_t co)                 {}
    virtual ColourBaseType getPixelColor(uint32_t pix) const         { return 0; }
    virtual uint8_t  getPins(uint8_t* pinArray = nullptr) const { return 0; }
    virtual uint16_t getLength() const                         { return isOk() ? _len : 0; }
    virtual uint8_t  getColorOrder() const                     { return COL_ORDER_RGB; }
    virtual uint8_t  skippedLeds() const                       { return 0; }
    virtual uint16_t getFrequency() const                      { return 0U; }
    virtual uint16_t getLEDCurrent() const                     { return 0; }
    virtual uint16_t getUsedCurrent() const                    { return 0; }
    virtual uint16_t getMaxCurrent() const                     { return 0; }

    inline  bool     hasRGB() const                            { return _hasRgb; }
    inline  bool     hasWhite() const                          { return _hasWhite; }
    inline  bool     hasCCT() const                            { return _hasCCT; }
    inline  bool     isDigital() const                         { return isDigital(_type); }
    inline  bool     is2Pin() const                            { return is2Pin(_type); }
    inline  bool     isOnOff() const                           { return isOnOff(_type); }
    inline  bool     isPWM() const                             { return isPWM(_type); }
    inline  bool     isVirtual() const                         { return isVirtual(_type); }
    inline  bool     is16bit() const                           { return is16bit(_type); }
    inline  bool     mustRefresh() const                       { return mustRefresh(_type); }
    inline  void     setReversed(bool reversed)                { _reversed = reversed; }
    inline  void     setStart(uint16_t start)                  { _start = start; }
    inline  void     setAutoWhiteMode(uint8_t m)               { if (m < 5) _autoWhiteMode = m; }
    inline  uint8_t  getAutoWhiteMode() const                  { return _autoWhiteMode; }
    inline  uint8_t  getNumberOfChannels() const               { return hasWhite() + 3*hasRGB() + hasCCT(); }
    inline  uint16_t getStart() const                          { return _start; }
    inline  uint8_t  getType() const                           { return _type; }
    inline  bool     isOk() const                              { return _valid; }
    inline  bool     isReversed() const                        { return _reversed; }
    inline  bool     isOffRefreshRequired() const              { return _needsRefresh; }
    inline  bool     containsPixel(uint16_t pix) const         { return pix >= _start && pix < _start + _len; }

    static inline std::vector<LEDType> getLEDTypes()           { return {{BUSTYPE_NONE, "", PSTR("None")}}; } // not used. just for reference for derived classes
    static constexpr uint8_t getNumberOfPins(uint8_t type)     { return isVirtual(type) ? 4 : isPWM(type) ? numPWMPins(type) : is2Pin(type) + 1; } // credit @PaoloTK
    static constexpr uint8_t getNumberOfChannels(uint8_t type) { return hasWhite(type) + 3*hasRGB(type) + hasCCT(type); }
    static constexpr bool hasRGB(uint8_t type) {
      return !((type >= BUSTYPE_WS2812_1CH && type <= BUSTYPE_WS2812_WWA) || type == BUSTYPE_ANALOG_1CH || type == BUSTYPE_ANALOG_2CH || type == BUSTYPE_ONOFF);
    }
    static constexpr bool hasWhite(uint8_t type) {
      return  (type >= BUSTYPE_WS2812_1CH && type <= BUSTYPE_WS2812_WWA) ||
              type == BUSTYPE_SK6812_RGBW || type == BUSTYPE_TM1814 || type == BUSTYPE_UCS8904 ||
              type == BUSTYPE_FW1906 || type == BUSTYPE_WS2805 || type == BUSTYPE_SM16825 ||        // digital types with white channel
              (type > BUSTYPE_ONOFF && type <= BUSTYPE_ANALOG_5CH && type != BUSTYPE_ANALOG_3CH) || // analog types with white channel
              type == BUSTYPE_NET_DDP_RGBW || type == BUSTYPE_NET_ARTNET_RGBW;                   // network types with white channel
    }
    static constexpr bool hasCCT(uint8_t type) {
      return  type == BUSTYPE_WS2812_2CH_X3 || type == BUSTYPE_WS2812_WWA ||
              type == BUSTYPE_ANALOG_2CH    || type == BUSTYPE_ANALOG_5CH ||
              type == BUSTYPE_FW1906        || type == BUSTYPE_WS2805     ||
              type == BUSTYPE_SM16825;
    }
    static constexpr bool  isTypeValid(uint8_t type)  { return (type > 15 && type < 128); }
    static constexpr bool  isDigital(uint8_t type)    { return (type >= BUSTYPE_DIGITAL__MIN && type <= BUSTYPE_DIGITAL__MAX) || is2Pin(type); }
    static constexpr bool  is2Pin(uint8_t type)       { return (type >= BUSTYPE_2PIN_MIN && type <= BUSTYPE_2PIN_MAX); }
    static constexpr bool  isOnOff(uint8_t type)      { return (type == BUSTYPE_ONOFF); }
    static constexpr bool  isPWM(uint8_t type)        { return (type >= BUSTYPE_ANALOG__MIN && type <= BUSTYPE_ANALOG__MAX); }
    static constexpr bool  isVirtual(uint8_t type)    { return (type >= BUSTYPE_VIRTUAL_MIN && type <= BUSTYPE_VIRTUAL_MAX); }
    static constexpr bool  is16bit(uint8_t type)      { return type == BUSTYPE_UCS8903 || type == BUSTYPE_UCS8904 || type == BUSTYPE_SM16825; }
    static constexpr bool  mustRefresh(uint8_t type)  { return type == BUSTYPE_TM1814; }
    static constexpr int   numPWMPins(uint8_t type)   { return (type - 40); }

    static inline int16_t  getCCT()                   { return _cct; }
    static inline void     setGlobalAWMode(uint8_t m) { if (m < 5) _gAWM = m; else _gAWM = AW_GLOBAL_DISABLED; }
    static inline uint8_t  getGlobalAWMode()          { return _gAWM; }
    static inline void     setCCT(int16_t cct)        { _cct = cct; }
    static inline uint8_t  getCCTBlend()              { return _cctBlend; }
    static inline void setCCTBlend(uint8_t b) {
      _cctBlend = (std::min((int)b,100) * 127) / 100;
      //compile-time limiter for hardware that can't power both white channels at max
      #ifdef WLED_MAX_CCT_BLEND
        if (_cctBlend > WLED_MAX_CCT_BLEND) _cctBlend = WLED_MAX_CCT_BLEND;
      #endif
    }
    static void calculateCCT(uint32_t c, uint8_t &ww, uint8_t &cw);

    uint16_t _start;
    uint16_t _len;
    
    const char* getTypeName();
    const char* getTypeName(uint8_t id);

    static uint8_t getTypeIDbyName(const char* name);
  
  protected:
    uint8_t  _type;
    uint8_t  _bri;
    bool _reversed;
    bool _valid;
    bool _needsRefresh;
    bool _hasRgb;
    bool _hasWhite;
    bool _hasCCT;
    uint8_t  _autoWhiteMode;
    uint8_t  *_data;   // pixel data, part of the new doublebuffer
    // global Auto White Calculation override
    static uint8_t _gAWM;
    // _cct has the following meanings (see calculateCCT() & BusManager::setSegmentCCT()):
    //    -1 means to extract approximate CCT value in K from RGB (in calcualteCCT())
    //    [0,255] is the exact CCT value where 0 means warm and 255 cold
    //    [1900,10060] only for color correction expressed in K (colorBalanceFromKelvin())
    static int16_t _cct;
    // _cctBlend determines WW/CW blending:
    //    0 - linear (CCT 127 => 50% warm, 50% cold)
    //   63 - semi additive/nonlinear (CCT 127 => 66% warm, 66% cold)
    //  127 - additive CCT blending (CCT 127 => 100% warm, 100% cold)
    static uint8_t _cctBlend;

    uint32_t autoWhiteCalc(uint32_t c) const;
    uint8_t *allocateData(size_t size = 1);
    void     freeData() { if (_data != nullptr) free(_data); _data = nullptr; }

};


// Temporary struct for passing bus configuration to bus
struct BusConfig 
{
  uint8_t type;
  uint16_t count;
  uint16_t start;
  uint8_t colorOrder;
  bool reversed;
  uint8_t skipAmount;
  bool refreshReq;
  uint8_t autoWhite;
  uint8_t pins[5] = {LEDPIN, 255, 255, 255, 255};
  uint16_t frequency;
  bool doubleBuffer;
  uint8_t milliAmpsPerLed;
  uint16_t milliAmpsMax;


  BusConfig(uint8_t busType, uint8_t* ppins, uint16_t pstart, uint16_t len = 1, uint8_t pcolorOrder = COL_ORDER_GRB, bool rev = false, uint8_t skip = 0, byte aw=RGBW_MODE_MANUAL_ONLY, uint16_t clock_kHz=0U, bool dblBfr=false, uint8_t maPerLed=LED_MILLIAMPS_DEFAULT, uint16_t maMax=ABL_MILLIAMPS_DEFAULT)
    : count(len)
    , start(pstart)
    , colorOrder(pcolorOrder)
    , reversed(rev)
    , skipAmount(skip)
    , autoWhite(aw)
    , frequency(clock_kHz)
    , doubleBuffer(dblBfr)
    , milliAmpsPerLed(maPerLed)
    , milliAmpsMax(maMax)
  {
    refreshReq = (bool) GET_BIT(busType,7);
    type = busType & 0x7F;  // bit 7 may be/is hacked to include refresh info (1=refresh in off state, 0=no refresh)
    size_t nPins = Bus::getNumberOfPins(type);
    for (size_t i = 0; i < nPins; i++) pins[i] = ppins[i];
  }



  // BusConfig(
  //   uint8_t busType, 
  //   uint8_t* ppins, 
  //   uint16_t pstart, 
  //   uint16_t length = 1,
  //   uint8_t _ColourOrder = 0,//{COLOUR_ORDER_INIT_DISABLED},
  //   bool rev = false, 
  //   uint8_t skip = 0, 
  //   byte aw = RGBW_MODE_MANUAL_ONLY
  // ){
        
  //   refreshReq = (bool) GET_BIT(busType,7);
  //   type = busType & 0x7F;  // bit 7 may be/is hacked to include refresh info (1=refresh in off state, 0=no refresh)
  //   count = length; 
  //   start = pstart; 
  //   reversed = rev; 
  //   skipAmount = skip; 
  //   autoWhite = aw;

  //   #ifdef ESP32
  //   doubleBuffer = true; //tmp fix, force here but should be BusConfig arg
  //   #else
  //   doubleBuffer = false;
  //   #endif

  //   uint8_t nPins = 1;
  //   if (type >= BUSTYPE_NET_DDP_RGB && type < 96) nPins = 4; //virtual network bus. 4 "pins" store IP address
  //   else if (type > 47) nPins = 2;
  //   else if (type > 40 && type < 46) nPins = NUM_BUSTYPE_PWM_PINS(type);
  //   for (uint8_t i = 0; i < nPins; i++) pins[i] = ppins[i];

  //   colorOrder = _ColourOrder;

  // }

  // Validates start and length and extends total if needed
  bool adjustBounds(uint16_t& total) 
  {
    if (!count) count = 1;
    if (count > MAX_LEDS_PER_BUS) count = MAX_LEDS_PER_BUS;
    if (start >= MAX_LEDS_NEO) return false;
    // Limit length of strip if it would exceed total permissible LEDs
    if (start + count > MAX_LEDS_NEO) count = MAX_LEDS_NEO - start;
    // Extend total count accordingly
    if (start + count > total) total = start + count;
    return true;
  }

};

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusDigital *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

class BusDigital : public Bus {
  public:
    BusDigital(BusConfig &bc, uint8_t nr, const ColorOrderMap &com);
    ~BusDigital() { cleanup(); }

    void show() override;
    bool canShow() const override;
    void setBrightness(uint8_t b) override;
    void setStatusPixel(uint32_t c) override;
    [[gnu::hot]] void setPixelColor(uint32_t pix, ColourBaseType c) override;
    void setColorOrder(uint8_t colorOrder) override;
    [[gnu::hot]] ColourBaseType getPixelColor(uint32_t pix) const override;
    uint8_t  getColorOrder() const override  { return _colorOrder; }
    uint8_t  getPins(uint8_t* pinArray = nullptr) const override;
    uint8_t  skippedLeds() const override    { return _skip; }
    uint16_t getFrequency() const override   { return _frequencykHz; }
    uint16_t getLEDCurrent() const override  { return _milliAmpsPerLed; }
    uint16_t getUsedCurrent() const override { return _milliAmpsTotal; }
    uint16_t getMaxCurrent() const override  { return _milliAmpsMax; }
    void reinit();
    void cleanup();

    static std::vector<LEDType> getLEDTypes();

  private:
    uint8_t _skip;
    uint8_t _colorOrder;
    uint8_t _pins[2];
    uint8_t _iType;
    uint16_t _frequencykHz;
    uint8_t _milliAmpsPerLed;
    uint16_t _milliAmpsMax;
    void * _busPtr;
    const ColorOrderMap &_colorOrderMap;

    static uint16_t _milliAmpsTotal; // is overwitten/recalculated on each show()

    inline ColourBaseType restoreColorLossy(ColourBaseType c, uint8_t restoreBri) const {
      if (restoreBri < 255) {
        uint8_t* chan = (uint8_t*) &c;
        for (uint_fast8_t i=0; i<4; i++) {
          uint_fast16_t val = chan[i];
          chan[i] = ((val << 8) + restoreBri) / (restoreBri + 1); //adding _bri slightly improves recovery / stops degradation on re-scale
        }
      }
      return c;
    }

    uint8_t  estimateCurrentAndLimitBri();
};

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusPwm *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

class BusPwm : public Bus {
  public:
    BusPwm(BusConfig &bc);
    ~BusPwm() { cleanup(); }

    void setColorOrder(uint8_t colorOrder){ _colorOrder = colorOrder; }
    uint8_t getColorOrder(){ return _colorOrder; }

    void setPixelColor(uint32_t pix, ColourBaseType c) override;
    ColourBaseType getPixelColor(uint32_t pix) const override; //does no index check
    uint8_t  getPins(uint8_t* pinArray = nullptr) const override;
    uint16_t getFrequency() const override { return _frequency; }
    void show() override;
    void cleanup() { deallocatePins(); }
    
    byte allocateLedc(byte channels);
    void deallocateLedc(byte pos, byte channels);

    static std::vector<LEDType> getLEDTypes();   

  private:
    uint8_t _pins[OUTPUT_BUSPWM_MAX_PINS];
    uint8_t _pwmdata[OUTPUT_BUSPWM_MAX_PINS];
    RgbwwColor output_colour; // change to RgbcwColor
    #ifdef ARDUINO_ARCH_ESP32
    uint8_t _ledcStart;
    #endif
    uint8_t _depth;
    uint16_t _frequency;
    uint8_t _colorOrder = 0;
    uint8_t ledcAlloc[2] = {0x00, 0x00}; //16 LEDC channels

    void deallocatePins();
};

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusOnOff *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

class BusOnOff : public Bus {
  public:
    BusOnOff(BusConfig &bc);
    ~BusOnOff() { cleanup(); }

    void setPixelColor(uint32_t pix, ColourBaseType c) override;
    ColourBaseType getPixelColor(uint32_t pix) const override;
    uint8_t  getPins(uint8_t* pinArray) const override;
    void show() override;
    void cleanup() {  }

    static std::vector<LEDType> getLEDTypes();

  private:
    uint8_t _pin;
    uint8_t _onoffdata;
};

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusNetwork *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

class BusNetwork : public Bus {
  public:
    BusNetwork(BusConfig &bc);
    ~BusNetwork() { cleanup(); }

    bool canShow() const override  { return !_broadcastLock; } // this should be a return value from UDP routine if it is still sending data out
    void setPixelColor(uint32_t pix, ColourBaseType c) override;
    ColourBaseType getPixelColor(uint32_t pix) const override;
    uint8_t  getPins(uint8_t* pinArray = nullptr) const override;
    void show() override;
    void cleanup();

    static std::vector<LEDType> getLEDTypes();

  private:
    IPAddress _client;
    uint8_t   _UDPtype;
    uint8_t   _UDPchannels;
    bool      _broadcastLock;
};

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusManager *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

//fine tune power estimation constants for your setup
//you can set it to 0 if the ESP is powered by USB and the LEDs by external
#ifndef MA_FOR_ESP
  #ifdef ESP8266
    #define MA_FOR_ESP         80 //how much mA does the ESP use (Wemos D1 about 80mA)
  #else
    #define MA_FOR_ESP        120 //how much mA does the ESP use (ESP32 about 120mA)
  #endif
#endif

class BusManager 
{
  public:
    BusManager(){};

    static uint32_t memUsage(BusConfig &bc);
    static uint32_t memUsage(unsigned maxChannels, unsigned maxCount, unsigned minBuses);
    static uint16_t currentMilliamps() { return _milliAmpsUsed + MA_FOR_ESP; }
    static uint16_t ablMilliampsMax()  { return _milliAmpsMax; }

    int add(BusConfig &bc);

    void useParallelOutput(bool enable); // workaround for inaccessible PolyBus
    void setRequiredChannels(uint8_t channels);

    //do not call this method from system context (network callback)
    void removeAll();
    void show();
    
    static Bus* busses[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES];// = {nullptr};

    [[gnu::hot]] void setPixelColor(uint32_t pix, ColourBaseType c);
    ColourBaseType getPixelColor(uint32_t pix);
    
    static void setBrightness(uint8_t b);
    // for setSegmentCCT(), cct can only be in [-1,255] range; allowWBCorrection will convert it to K
    // WARNING: setSegmentCCT() is a misleading name!!! much better would be setGlobalCCT() or just setCCT()
    static void setSegmentCCT(int16_t cct, bool allowWBCorrection = false);
        
    bool canAllShow();

    Bus* getBus(uint8_t busNr);

    uint16_t getTotalLength(); //semi-duplicate of strip.getLengthTotal() (though that just returns strip._length, calculated in finalizeInit())

    inline void updateColorOrderMap(const ColorOrderMap &com) 
    {
      memcpy(&colorOrderMap, &com, sizeof(ColorOrderMap));
    }

    inline const ColorOrderMap& getColorOrderMap(){ return colorOrderMap; }
    static inline uint8_t getNumBusses(){return numBusses; }

  private:
    static uint8_t numBusses;
    static ColorOrderMap colorOrderMap;  
    
    static uint16_t _milliAmpsUsed;
    static uint16_t _milliAmpsMax;
    
    inline uint8_t getNumVirtualBusses() {
      int j = 0;
      for (int i=0; i<numBusses; i++) if (busses[i]->isVirtual()) j++;
      return j;
    }

};

#endif // USE_MODULE_LIGHTS_INTERFACE

#endif // BusManager_h
