// #pragma     once



typedef struct Segment {
  public:
    uint16_t start = 0; // start means first led index within segment : start index / start X coordinate 2D (left)
    uint16_t stop  = 0; // stop means total leds within the segment (not the index of last pixel) : stop index / stop X coordinate 2D (right); segment is invalid if stop == 0
    uint16_t offset = 0;

    uint8_t  speed               = DEFAULT_SPEED; // in my effect, may be the new time_ms
    uint8_t  intensity           = 127;           // should not relate to blending, only what the effect is doing
    uint16_t cycle_time__rate_ms = 2000; // EffectTimePeriod ie EffectCycleTime
    
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
     * Decimate is a multiplier, so 0 means none, with each value being a divisor. Decimate 10 would be only generate 10% of the leds
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
     **/
    #ifdef ENABLE_DEVFEATURE_LIGHT__FORCE_EFFECT_COLOUR_TYPE_AS_RGBCCT
    ColourType colour_type__used_in_effect_generate = ColourType::COLOUR_TYPE__RGBCCT__ID; 
    #else
    ColourType colour_type__used_in_effect_generate = ColourType::COLOUR_TYPE__RGB__ID; 
    #endif

    /**
     * @brief Stores at least 5 full RgbcctColours with all internal manipulations as needed
     * @NOTE: Replaces WLED 3 colour options
     **/
    #define RGBCCTCOLOURS_SIZE 5
    RgbcctColor rgbcctcolors[5] = {RgbcctColor(255,0,0,0,0), RgbcctColor(0,255,0,0,0), RgbcctColor(0,0,255,0,0), RgbcctColor(255,0,255,0,0), RgbcctColor(255,255,0,0,0)};

    void set_colors(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w )
    {
      uint8_t index_t = index<RGBCCTCOLOURS_SIZE?index:0;
      if(index>RGBCCTCOLOURS_SIZE){ Serial.println("ERROR"); }
      rgbcctcolors[index_t] = RgbcctColor(r,g,b,w,w);
    }

    ANIMATION_FLAGS flags;
    

    // Effects (Scenes & Flasher), Ambilight, Adalight    
    uint8_t animation_mode_id = 0; // rename to "effect_id"

    
    uint16_t get_transition_rate_ms() // Effect that require call for every update, must be called at FRAMETIME_MS, otherwise, can manually be set
    {
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
      if(effect_id >= WLED_GROUP_IDS_FIRST)
      {
        return FRAMETIME_MS;
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

    uint8_t effect_anim_section = 0; // 0 draw, 1 stop draw


    /**
     * @brief AUX options going forward must only be internal effect save states, and NOT user defined options (these should be "effect_option")
     * Internal multi-use variables
     * Keep at least one U32 for millis storage
     */
    struct INTERNAL_MULTIUSE_PARAMETERS
    {
      uint16_t aux0 = 0;  // custom var
      uint16_t aux1 = 0;  // custom var
      uint16_t aux2 = 0;
      uint32_t aux3 = 0; // Also used for random CRGBPALETTE16 timing
    }params_internal;

    Decounter<uint16_t> auto_timeoff = Decounter<uint16_t>();

    /**
     * @brief 
     * External user controllable multi-use variables
     */
    uint16_t params_user[4] = {0};

    byte* data;     // effect data pointer
    CRGB* leds;     // local leds[] array (may be a pointer to global)
    static CRGB *_globalLeds;             // global leds[] array
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
    uint16_t _dataLen;
    static uint16_t _usedSegmentData;

    inline uint16_t DataLength(){ return _dataLen; };
    inline byte* Data(){ return data; };

    /**
     * Each segment will have its own animator
     * This will also need to share its index into the animation so it knows what segments to run
     * */
    NeoPixelAnimator* animator = new NeoPixelAnimator(1, NEO_MILLISECONDS); //one animator for each segment, which is only init when needed or else delete

    mPaletteLoaded* palette_container = new mPaletteLoaded();

    void LoadPalette(uint8_t palette_id, mPaletteLoaded* palette_container = nullptr);

    uint32_t tSaved_LastUpdated = millis();
    uint32_t tTick_maximum_call_ms = 10;

    /**
     * Using "index" inside animator as segment index
     * */
    ANIM_FUNCTION_SIGNATURE;
    bool animation_has_anim_callback = false; //should be dafult on start but causing no animation on start right now

    // uint32_t tSaved_AnimateRunTime = millis();

    Segment(uint16_t sStart=0, uint16_t sStop=30) :
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
      leds(nullptr),
      _capabilities(0),
      _dataLen(0)
    {
      start = sStart;
      stop = sStop;
      refreshLightCapabilities();

      params_internal.aux0 = 0;
      params_internal.aux1 = 0;
      params_internal.aux2 = 0;
      params_internal.aux3 = 0;

      palette_container = new mPaletteLoaded(); // duplicate of above, but needed for each segment

      // colour_type__used_in_effect_generate = RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID;
      // if(bus)
      // need to make this white here


      
    }

    Segment(uint16_t sStartX, uint16_t sStopX, uint16_t sStartY, uint16_t sStopY) : Segment(sStartX, sStopX) {
      startY = sStartY;
      stopY  = sStopY;
    }

    /**
     * @brief NOTE: copy, such as "for(auto seg:segments)" will require creation and destruction of the segment everytime in huge blocks and for performance reasons index iteration is preferred 
     * 
     * @param orig 
     */
    Segment(const Segment &orig) // copy constructor
     {
      Serial.println(F("-- Copy segment constructor --"));

      memcpy(this, &orig, sizeof(Segment));
      name = nullptr;
      data = nullptr;
      _dataLen = 0;
      if (leds && !Segment::_globalLeds) leds = nullptr;
      if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
      if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
      if (orig.leds && !Segment::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
    
    };


    Segment(Segment &&orig) noexcept // move constructor
    {

      Serial.println(F("-- Move segment constructor --"));
      memcpy(this, &orig, sizeof(mAnimatorLight::Segment));
      orig.name = nullptr;
      orig.data = nullptr;
      orig._dataLen = 0;
      orig.leds = nullptr;
      
    }


    ~Segment() 
    {

      #ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
        Serial.print(F("Destroying segment:"));
        if (name) Serial.printf(" %s (%p)", name, name);
        if (data) Serial.printf(" %d (%p)", (int)_dataLen, data);
        if (leds) Serial.printf(" [%u]", length()*sizeof(CRGB));
        Serial.println();
      #endif
      if (!Segment::_globalLeds && leds) free(leds);
      if (name) delete[] name;
      deallocateData();

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
    bool    setColor(uint8_t slot, RgbcctColor c); //returns true if changed
    void    setCCT(uint16_t k);
    void    setOption(uint8_t n, bool val);
    void    setMode(uint8_t fx, bool loadDefaults = false);
    void    setPalette(uint8_t pal);
    uint8_t differs(Segment& b) const;
    void    refreshLightCapabilities(void);

    static uint32_t color_blend(uint32_t,uint32_t,uint16_t,bool b16=false);
    static uint32_t color_add(uint32_t,uint32_t, bool fast=false);
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
    void setUpLeds(void);   // set up leds[] array for loseless getPixelColor()

    // 1D strip
    uint16_t virtualLength(void) const;
    void setPixelColor(int n, uint32_t c); // set relative pixel within segment with color
    void setPixelColor(int n, byte r, byte g, byte b, byte w = 0) { setPixelColor(n, RGBW32(r,g,b,w)); } // automatically inline
    void setPixelColor(int n, CRGB c)                             { setPixelColor(n, RGBW32(c.r,c.g,c.b,0)); } // automatically inline
    void setPixelColor(float i, uint32_t c, bool aa = true);
    void setPixelColor(float i, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0, bool aa = true) { setPixelColor(i, RGBW32(r,g,b,w), aa); }
    void setPixelColor(float i, CRGB c, bool aa = true)                                         { setPixelColor(i, RGBW32(c.r,c.g,c.b,0), aa); }
    uint32_t getPixelColor(int i);
    
    RgbcctColor GetPixelColor(uint16_t indexPixel = 0);       
    
    void SetPixelColor(uint16_t indexPixel, RgbcctColor color, bool flag_brightness_already_applied = false);
    void SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, bool flag_brightness_already_applied = false);  
    void SetPixelColor(uint16_t indexPixel, uint32_t color, bool flag_brightness_already_applied = false);
    

    // 1D support functions (some implement 2D as well)
    void blur(uint8_t bluramount);
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



    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    GetPaletteColour(
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

    uint8_t GetPaletteDiscreteWidth(); // Rename to colours in palette

    /** SECTION start ****************************************************************************************************************
    * * Matrix : Inside each segment  *********************************************************************************
    * *****************************************************************************************************************
    * *****************************************************************************************************************
    * *****************************************************************************************************************
    **/

    uint16_t virtualWidth(void)  const;
    uint16_t virtualHeight(void) const;
    uint16_t nrOfVStrips(void) const;


    #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

    uint16_t XY(uint16_t x, uint16_t y); // support function to get relative index within segment


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
      void setPixelColorXY(float x, float y, CRGB c, bool aa = true)
      { 
        // Serial.println(__LINE__);
        setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), aa); 
      }

    uint32_t getPixelColorXY(uint16_t x, uint16_t y);

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
    void draw_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c);
    void fill_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c) { drawLine(x0, y0, x1, y1, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color, uint32_t col2 = 0, int8_t rotate = 0);
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c, CRGB c2, int8_t rotate = 0) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0), RGBW32(c2.r,c2.g,c2.b,0), rotate); } // automatic inline
    
    
    void drawCharacter_UsingGradientPalletes(
      unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, 
  int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundGradientHorizontal
  );//unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, int8_t rotate, bool solidPerChar, bool horizontalGradient, bool backgroundHorizontalGradient);
  
  
    void wu_pixel(uint32_t x, uint32_t y, CRGB c);
    void blur1d(fract8 blur_amount); // blur all rows in 1 dimension
    void blur2d(fract8 blur_amount) { blur(blur_amount); }
    void fill_solid(CRGB c) { fill(RGBW32(c.r,c.g,c.b,0)); }
    void nscale8(uint8_t scale);
  #else
    uint16_t XY(uint16_t x, uint16_t y)                                    { return x; }
    void setPixelColorXY(int x, int y, uint32_t c)                         { setPixelColor(x, c); }
    void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColor(x, RGBW32(r,g,b,w)); }
    void setPixelColorXY(int x, int y, CRGB c)                             { setPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    void setPixelColorXY(float x, float y, uint32_t c, bool aa = true)     { setPixelColor(x, c, aa); }
    void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true) { setPixelColor(x, RGBW32(r,g,b,w), aa); }
    void setPixelColorXY(float x, float y, CRGB c, bool aa = true)         { setPixelColor(x, RGBW32(c.r,c.g,c.b,0), aa); }
    uint32_t getPixelColorXY(uint16_t x, uint16_t y)                       { return getPixelColor(x); }
    void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t c, uint8_t blend) { blendPixelColor(x, c, blend); }
    void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColor(x, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColorXY(int x, int y, uint32_t color)                     { addPixelColor(x, color); }
    void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { addPixelColor(x, RGBW32(r,g,b,w)); }
    void addPixelColorXY(int x, int y, CRGB c)                             { addPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade)            { fadePixelColor(x, fade); }
    void box_blur(uint16_t i, bool vertical, fract8 blur_amount) {}
    void blurRow(uint16_t row, fract8 blur_amount) {}
    void blurCol(uint16_t col, fract8 blur_amount) {}
    void moveX(int8_t delta) {}
    void moveY(int8_t delta) {}
    void move(uint8_t dir, uint8_t delta) {}
    void fill_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c) {}
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


} segment;








