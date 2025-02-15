//#ifdef ESP8266
#ifndef _MSUPPORT_H_
#define _MSUPPORT_H_

#define D_UNIQUE_MODULE_CORE_SUPPORT_ID 2006 // [(Folder_Number*100)+ID_File]

/*********************************************************************************************
 * Watchdog related
\*********************************************************************************************/
#ifdef ENABLE_FEATURE_WATCHDOG_TIMER
#ifdef ESP8266
  extern void WDT_Init();
  extern void WDT_Reset();
#endif // ESP8266
#ifdef ESP32
  #include "esp_system.h"
  #ifndef ARDUINO_ISR_ATTR
  #define ARDUINO_ISR_ATTR IRAM_ATTR 
  #endif
  extern void ARDUINO_ISR_ATTR resetModule();
  extern void WDT_Init();
  extern void WDT_Reset();
#endif // ESP32
#endif // WATCHDOG_TIMER_SECTION_GUARD_H


//using class, to save known sun position, maybe in support? azimuth, elevation, isvalid, age


#define ARRAY_SIZE(_a) (sizeof((_a)) / sizeof((_a)[0]))


#include "2_CoreSystem/mBaseConfig.h"


#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>


//move to time lib
// typedef struct TIMEREACHED_SAVED{
//   uint32_t millis_saved;
//   uint8_t RunNow; // send early
// }timereached_t;

enum FUNCHANDLER_TIME_IDS{
  FUNCHANDLER_TIME_MS_ID=0,
  FUNCHANDLER_TIME_SECS_ID=1,
  FUNCHANDLER_TIME_MINS_ID=2,
};

enum STATE_NUMBER_IDS{
  STATE_NUMBER_INVALID_ID=-1,
  STATE_NUMBER_OFF_ID=0,
  STATE_NUMBER_ON_ID,
  STATE_NUMBER_TOGGLE_ID,
  STATE_NUMBER_BLINK_ID,
  STATE_NUMBER_BLINK_OFF_ID,
  STATE_NUMBER_INCREMENT_ID, //eg fans, increase /// Also add a "STATE_NUMBER_INCREMENT_NO_WRAP_ID"
  STATE_NUMBER_DECREMENT_ID,
  STATE_NUMBER_FOLLOW_ID,
  STATE_NUMBER_FOLLOW_INV_ID,
  STATE_NUMBER_LENGTH_ID
};


#define P_PHASE_OUT() Serial.println(F("PHASE OUT"));

// Methods for disable (returning from loop early) until an uptime, network established, or network uptime > x amount
#define DEBUG_OTA_FLASH_BLOCKER_UNTIL_STABLE_RETURN_ZERO()   if(tkr_time->RtcTime.seconds_nonreset < 120){ return 0; }
#define DEBUG_OTA_FLASH_BLOCKER_UNTIL_UPTIME_X_RETURN_ZERO(X)   if(tkr_time->RtcTime.seconds_nonreset < X){ return 0; }
// #define DEBUG_OTA_FLASH_BLOCKER_UNTIL_NETWORK_UPTIME_X_RETURN_ZERO(X)   if(tkr_time->RtcTime.seconds_nonreset < X){ return 0; }


#define CALL_VOID_FUNCTION(object,ptrToMember)  ((object).*(ptrToMember))

// 8 bit packed flag 
typedef union {
  uint8_t flags;
  struct {
    uint8_t run_now   : 1;
    // 0 - secs, 1 - ms, 2 - hours
    uint8_t time_unit : 2;   
    uint8_t periodic_enabled   : 1;
    uint8_t run_always   : 1; //ie loops
    uint8_t reserved  : 4;     
  };
}FUNCTION_HANDLER_FLAGS;


template <typename Class>
struct functionhandler {
  uint32_t      saved_millis = 0;
  uint16_t      time_val = 1;
  uint8_t       module_id = 0;
  void          (Class::*function)(); // member-function to sender with one args
  FUNCTION_HANDLER_FLAGS flags;
};


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifdef ESP8266
#define AGPIO(x) (x)
#else  // ESP32
#define AGPIO(x) (x<<5)
#endif  // ESP8266 - ESP32

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "2_CoreSystem/07_Time/mTime.h"
#include "2_CoreSystem/01_Settings/mSettings.h"




//#ifdef USE_I2C
  #include <Wire.h>                         // I2C support library
//#endif  // USE_I2C

extern "C" {
extern struct rst_info resetInfo;
}


const char D_CSTRING_ERROR_MESSAGE_CTR[] = "Error";
const char PM_CSTRING_ERROR_MESSAGE_CTR[] PROGMEM = "Error";


// Returns value if its between values, if not, returns midpoint
template<typename T>
T WithinLimits(T lower, T value, T upper){ //RENAME
  // returns value if its between limits
  if((value>lower)&&(value<upper)){
    return value;
  }
  // return midpoint if above was not true
  else{
    return ((upper-lower)/2)+value; //huh?
  }
}

// Returns value if its between values, if not, returns midpoint
template<typename T>
bool IsWithinLimits(T lower, T value, T upper){
  // returns value if its between limits
  if((value>lower)&&(value<upper)){
    return true;
  }
  return false;
}
template<typename T, typename U, typename V>
bool IsWithinLimitsInclusive(T lower, U value, V upper){
  // returns value if its between limits
  if((value>=lower)&&(value<=upper)){
    return true;
  }
  return false;
}

template <typename T>
bool CheckAndClearFlag(T* flag){
  if(*flag){        // If set, clear and return true, else false
    *flag = false;
    return true;
  }
  return false;
}


template<typename A, typename B, typename C>
A safeDivideWithDefault(A original_number, B divisor, C default_when_zero) {
  return (divisor != 0) ? original_number/divisor : default_when_zero;
}


template<typename T>
T min3(T a, T b, T c) {
  return (a < b && a < c) ? a : (b < c) ? b : c;
}

template<typename T>
bool SetAndKeepHighestNumber(T* current, T new_value){
  if(new_value > *current){
    *current = new_value;
    return true; // true is changed
  }
  return false;
} 

/**
 * @brief Faster to check for false than all true
 * 
 * @tparam A 
 * @tparam B 
 * @tparam C 
 * @tparam D 
 * @param x 
 * @param xl 
 * @param y 
 * @param yl 
 * @return true 
 * @return false 
 */
template<typename A, typename B, typename C, typename D>
bool ArrayCompare(A x, B x_len, C y, D y_len)
{
  for(B i=0; i<x_len; i++)
  {
    if(i < y_len)
    {
      if(x[i] != y[i])
      {
        return false;
      }
    }
  }
  return true;


      // if(memcmp(
      //   sensor[sensor_count].address,
      //   address_to_find,
      //   sizeof(sensor[sensor_count].address
      //   ))==0){ // 0 means equal


}


template<typename A,typename B,typename C,typename D,typename E>
E mapvalue(A a, B b, C c, D d, E e){ //should be returning at largest, the size of E range
  return map(a,b,c,d,e);
}


template <typename T, typename U>
T ArrayCountUntilNull(T* buffer, U buflen){
  T count = 0;
  while(buffer != 0){
    count++;
    if(count > buflen){
      break;
    }
  }
  return count;
}

template <typename T>
T GetRandomSaturationVariation(T mean, T standard_deviation, T constrained_min = 0, T constrained_max = 0)
{

  // Get a value between (and inclusing zero) and the maximum range with SD as the centre point
  T random_value = random(0,standard_deviation);

  // Either add or subtract the value from the centre
  T result = mean + (random(0,1)?1:-1)*(int)random_value; 

  // If these are not equal, then apply contraint
  if(constrained_min != constrained_max){
    result = constrain(result, constrained_min, constrained_max);
  }

  // ALOG_INF(PSTR("result=%d"),result);

  return result; 

}


template <typename T, typename U, typename V>
bool IsWithinRange(T val_to_check, U min_val, V max_val){

  if(
    (val_to_check >= min_val)&&
    (val_to_check <= max_val)
  ){
    return true;
  }

  return false;

}



// Return new state
template <typename T, typename U>
T ModifyStateNumberIfToggled(T* command_state, U check_state){

  if(*command_state == STATE_NUMBER_TOGGLE_ID){
    if(check_state){
      *command_state = STATE_NUMBER_OFF_ID;
    }else{
      *command_state = STATE_NUMBER_ON_ID;
    }
  }
  // Serial.printf("NOW is %d\n\r",command_state);

  return *command_state; // return new state
}


#include <type_traits>  // For std::is_signed
template <typename T>
static uint8_t NumDigits(T x)
{
  static_assert(std::is_integral<T>::value, "NumDigits only works with integral types");

  if (std::is_signed<T>::value && x < 0) {
      x = -x;  // Handle negative values for signed types
  }

  uint8_t digits = 1;
  while (x >= 10) {
      x /= 10;
      digits++;
  }
  return digits;
}


// Return new state
template <typename T, typename U>
T ConvertStateNumberIfToggled(T command_state, U check_state){

  //Serial.printf("FLIP TO BE %d\n\r",command_state);
  if(command_state == STATE_NUMBER_TOGGLE_ID){
    if(check_state){
      //Serial.println("FLIP TO BE OFF");
      command_state = STATE_NUMBER_OFF_ID;
      // check_state = STATE_NUMBER_OFF_ID;
    }else{
      //Serial.println("FLIP TO BE ON");
      command_state = STATE_NUMBER_ON_ID;
      // check_state = STATE_NUMBER_ON_ID;
    }
  }
  // Serial.printf("NOW is %d\n\r",command_state);

  return command_state; // return new state
}

#include <string>

template <typename T>
std::string toBinaryString(T value, size_t bitCount = sizeof(T) * 8) {
    static_assert(std::is_integral<T>::value, "Type must be an integral type");

    std::string result;
    result.reserve(bitCount);  // Reserve space for the bits

    for (size_t i = 0; i < bitCount; ++i) {
        // Extract the bit from the value, starting from the MSB
        result += (value & (1 << (bitCount - 1 - i))) ? '1' : '0';
    }

    return result;
}

extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end);

extern uint32_t ResetReason_g(void);

extern void SafeMode_StartAndAwaitOTA(uint8_t seconds_to_wait = 0 /*default of zero, is indefinitely */);

#ifdef ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_DEBUG Serial
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1
#include <TinyGsmClient.h>
extern void SafeMode_CellularConnectionAndSendLocation();
#endif // ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID


const uint32_t crash_magic = 0x53415400;   // Stack trace magic number (TASx)
const uint32_t crash_rtc_offset = 32;      // Offset in RTC memory skipping OTA used block
const uint32_t crash_dump_max_len = 31;    // Dump only 31 call addresses to satisfy max JSON length of about 600 characters

#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mSupport :
  public mTaskerInterface
{
  public:
    mSupport(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    static constexpr const char* PM_MODULE_CORE_SUPPORT_CTR = D_MODULE_CORE_SUPPORT_CTR;
    PGM_P GetModuleName(){ return PM_MODULE_CORE_SUPPORT_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_SUPPORT_ID; }
    

    void CheckResetConditions();

    #ifdef USE_ARDUINO_OTA
      /*********************************************************************************************\
       * Allow updating via the Arduino OTA-protocol.
       *
       * - Once started disables current wifi clients and udp
       * - Perform restart when done to re-init wifi clients
      \*********************************************************************************************/

      bool arduino_ota_triggered = false;
      uint16_t arduino_ota_progress_dot_count = 0;
      bool ota_init_success = false;

      void ArduinoOTAInit(void);
      void ArduinoOtaLoop(void);
    #endif // USE_ARDUINO_OTA


    /****
     * "class-less" Pointer Member function that takes the struct handler which contains if/when a mqtt payload should
     * be sent. If a payload should sent, the formatted publish function is called.
     * */
    template<typename T>
    void FunctionHandler_Call(T& class_ptr, uint16_t unique_id, functionhandler<T>* handler_ptr)
    {
      if(handler_ptr == nullptr){
        return;
      }

      uint16_t time_multiplier = 0;
      switch(handler_ptr->flags.time_unit){
        default:
        case FUNCHANDLER_TIME_MS_ID:   time_multiplier = 1;     break;
        case FUNCHANDLER_TIME_SECS_ID: time_multiplier = 1000;  break;
        case FUNCHANDLER_TIME_MINS_ID: time_multiplier = 60000; break;
      }

      if(handler_ptr->flags.periodic_enabled){
        if(abs(millis()-handler_ptr->saved_millis)>=handler_ptr->time_val*time_multiplier){ handler_ptr->saved_millis=millis();
          handler_ptr->flags.run_now = true;
        }
      }
      if(handler_ptr->flags.run_now || handler_ptr->flags.run_always){ handler_ptr->flags.run_now = false;    
        handler_ptr->saved_millis = millis();    
        // (class_ptr.*handler_ptr->function)(); //manual
        CALL_VOID_FUNCTION(class_ptr,handler_ptr->function)();
      }
    };

    void parse_JSONCommand(JsonParserObject obj);

    void AppendDList(char* buffer, const char* to_add);
    void AppendDList(char* buffer, uint16_t buflen, const char* formatP, ...);

    float CharToFloat(const char *str);
    char* ReplaceChar(char* p, char find, char replace) ;
    char* ReplaceCommaWithDot(char* p) ;
    
    struct LOOP_PERFORMANCE{
      uint32_t loop_counter=0;
      uint32_t cycles_per_sec=0;
      uint32_t tSaved;
    }activity;
    
    uint32_t ResetReason(void);
    String GetResetReason(void);
    const char* GetResetReason(char* buffer, uint8_t buflen);


    size_t strchrspn(const char *str1, int character);
    static char* float2CString(float number, unsigned char prec, char *s);
    
    #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
    char* RemoveSpace(char* p);
    char* TrimSpace(char *p);
    char* RemoveControlCharacter(char* p);
    char* ReplaceChar(char* p, char find, char replace);
    char* ReplaceCommaWithDot(char* p);    
    char* Unescape(char* buffer, uint32_t* size);
    #endif

    char* LowerCase(char* dest, const char* source);
    char* UpperCase(char* dest, const char* source);
    char* UpperCase_P(char* dest, const char* source);
    char* Trim(char* p);

    bool ValidIpAddress(const char* str);
    bool ParseIPv4(uint32_t* addr, const char* str);
    
    #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
    bool NewerVersion(char* version_str);
    #endif
    
    float ModulusRangef(float f, float a, float b);
    double FastPrecisePow(double a, double b);
    float FastPrecisePowf(const float x, const float y);
    
    char* GetTextIndexedTemp(char* destination, size_t destination_size, uint16_t index, const char* haystack);
    static char* GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack);
    static char* GetTextIndexed_P(char* destination, size_t destination_size, uint16_t index, const char* haystack);
        
    static int16_t GetCommandID16_P(const char* needle, const char* haystack, char* destination = nullptr, size_t destination_size = 0);
    static int16_t GetCommandID16_MultipleSubMatches_P(const char* needle, const char* haystack, char* destination = nullptr, size_t destination_size = 0);

    int GetCommandCode(char* destination, size_t destination_size, const char* needle, const char* haystack);
    
    #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
    void SetSerialBaudrate(int baudrate);
    void SerialSendRaw(char *codes);
    uint32_t GetHash(const char *buffer, size_t size);
    #endif

    void ClaimSerial(void);
    void ShowSource(int source);

    uint16_t WriteBuffer_P(const char* formatP, ...);
    uint16_t WriteBuffer_P(char* buffer, const char* formatP, ...);
    void WriteBuffer_P(char* buffer, uint16_t* length, const char* formatP, ...);
    static void appendToBuffer(char* buffer, size_t bufferSize, const char* format, ...);
    static void removeTrailingComma(char* buffer);

    void CommandSet_Restart(int8_t command);

    void CrashDump_AddJson();
    void CmndCrash(void);
    void CmndWDT(void);
    void CmndBlockedLoop(void);
    void CrashDumpClear(void);
    bool CrashFlag(void);
    void CrashDump(void);
    
    int Response_P(const char* format, ...);
    int ResponseAppend_P(const char* format, ...);

    void PerformEverySecond(void);

    char* GetState_Name_by_ID(uint8_t id, char* buffer, uint8_t buflen);
    int8_t GetStateNumber(const char *state_text);

    char* GetStateText(uint32_t state);
    

    void SleepDelay(uint32_t mseconds);

    #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
    void SetPulseTimer(uint32_t index, uint32_t time);
    uint32_t GetPulseTimer(uint32_t index);
    #endif

    static int mSearchCtrIndexOf(const char* toSearch, const char* toFind);
    
    static uint16_t NumCtr2Num(char* numctr, uint8_t numberlength);
    static float roundfloat(float in, uint8_t dec);
    static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
    #define mapf mSupport::mapfloat // used by WLED
        
    /**
     * @brief Timing for main loop
     **/
    uint32_t tSavedLoop1Sec = millis();
    uint32_t tSavedLoop50mSec = millis();
    uint32_t tSavedLoop100mSec = millis();
    uint32_t tSavedLoop250mSec = millis();
    uint32_t loop_runtime_millis = millis();
    uint32_t loop_start_millis = millis();
    uint32_t loop_delay_temp = millis();
    uint32_t loops_per_second = millis();
    uint32_t this_cycle_ratio = millis();

    #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
    uint8_t GetNormalDistributionRandom(uint8_t mean, uint8_t standard_deviation, uint8_t constrained_min = 0, uint8_t constrained_max = 0);
    #endif

    static int32_t safeDivideInt(int32_t num, int32_t den);

    char GetVersionBranchTypeCharNameByID(uint8_t id);
    void init_FirmwareVersion();

};

#endif