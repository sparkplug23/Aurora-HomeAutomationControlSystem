#ifndef _MSTREAMOUT_H
#define _MSTREAMOUT_H 0.5

#define D_UNIQUE_MODULE_CORE_LOGGING_ID   2008 // [(Folder_Number*100)+ID_File]

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h> //esp32
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#endif

#include "1_TaskerManager/mTaskerManager.h"

#include <Arduino.h>
#include <stdint.h>

#ifdef USE_SOFTWARE_SERIAL_DEBUG
  #include <SoftwareSerial.h>
  #define SERIAL_DEBUG Serial
#else
  #define SERIAL_DEBUG Serial
#endif

enum LoggingLevels {
  LOG_LEVEL_NONE=0, 

  /**
   *  Highest level of trace debug that will always be shown when called, but should always be disabled via ifdef calls ie "ENABLE_DEBUG_TRACE__##"
   **/
  LOG_LEVEL_DEBUG_TRACE,

  /**
   *  
   **/
  LOG_LEVEL_ERROR, 

  /**
   *  
   **/
  LOG_LEVEL_WARNING, 

  /**
   *  New level with elevated previledge - during code development use only
   **/
  LOG_LEVEL_TEST,

  /**
   *  Used to pad a message in logging to emphasis in a busy log, used only when required
   **/
  LOG_LEVEL_HIGHLIGHT,

  /**
   *  Show critical information, such as during boot for easy at a glance that the system is working as expected
   **/
  LOG_LEVEL_IMPORTANT,

  /**
   *  General information and used most often
   **/
  LOG_LEVEL_INFO,

  /**
   *  Detailed logging on commands being parseed
   **/
  LOG_LEVEL_COMMANDS,

  /**
   *  Used often, debugging in the serial monitor
   **/
  LOG_LEVEL_DEBUG, 

  /**
   *  Only used when searching for a high level issue, but keeping up performance
   **/
  LOG_LEVEL_DEBUG_MORE,

  /**
   *  Only used when searching for a crash, will cause critical slow down
   **/
  LOG_LEVEL_DEBUG_LOWLEVEL, 

  /**
   *  Enable all messages
   **/
  LOG_LEVEL_ALL
};


// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_LINE) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE    SERIAL_DEBUG.printf("DEBUG: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE   //nothing, no code
#endif


// #define ENABLE_DEBUG_DELAYS
#if defined(ENABLE_DEBUG_MANUAL_DELAYS)
#define DEBUG_DELAY(x) delay(x)
#else
#define DEBUG_DELAY(x)
#endif


// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_CHECK_AND_PRINT_NULLPTR) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_CHECK_AND_PRINT_NULLPTR(X)    if(X==nullptr){ \
                        SERIAL_DEBUG.printf("nullptr true"); }else{  SERIAL_DEBUG.printf("nullptr false"); }\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_CHECK_AND_PRINT_NULLPTR   //nothing, no code
#endif


#ifdef USE_DEBUG_PRINT
  #define DEBUG_PRINT(x) SERIAL_DEBUG.print(x)
  #define DEBUG_PRINTLN(x) SERIAL_DEBUG.println(x); \
                            Serial.flush();
  #define DEBUG_PRINTF(x...) SERIAL_DEBUG.printf(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x...)
#endif


#if defined(USE_DEBUG_PRINT_FUNCTION_NAME) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_PRINT_FUNCTION_NAME   SERIAL_DEBUG.print(__FILE__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__FUNCTION__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__LINE__);\
                                      SERIAL_DEBUG.flush();
#else
  #define DEBUG_PRINT_FUNCTION_NAME   //nothing, no code
#endif

#if defined(USE_DEBUG_PRINT_FUNCTION_NAME_TEST) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_PRINT_FUNCTION_NAME_TEST   SERIAL_DEBUG.print(__FILE__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__FUNCTION__);\
                                      SERIAL_DEBUG.print("\t");\
                                      SERIAL_DEBUG.println(__LINE__);\
                                      SERIAL_DEBUG.flush();
#else
  #define DEBUG_PRINT_FUNCTION_NAME_TEST   //nothing, no code
#endif

#if defined(ENABLE_DEBUG_LINE_HERE_TRACE)
  #define DEBUG_LINE_HERE_TRACE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE_TRACE   //nothing, no code
#endif

//For single test use, no ifdefs
// #ifdef USE_DEBUG_LINE
#if defined(ENABLE_DEBUG_LINE_HERE)
  #define DEBUG_LINE_HERE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE   //nothing, no code
#endif

#if defined(ENABLE_DEBUG_LINE_HERE2)
  #define DEBUG_LINE_HERE2    SERIAL_DEBUG.printf("DEBUG HERE2: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE2   //nothing, no code
#endif

#if defined(ENABLE_DEBUG_LINE_HERE3)
  #define DEBUG_LINE_HERE3    SERIAL_DEBUG.printf("DEBUG HERE3: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE3   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_LINE_HERE_MILLIS)
  #define DEBUG_LINE_HERE_MILLIS    SERIAL_DEBUG.printf("DEBUG: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.print(__LINE__);\
                        SERIAL_DEBUG.print('-');\
                        SERIAL_DEBUG.println(millis());\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE_MILLIS   //nothing, no code
#endif


#if defined(ENABLE_DEBUG_TIME__PRINT)
    // Macro to start time measurement, creates a local variable to store the start time
    #define DEBUG_TIME__START uint32_t __debug_time_start__ = micros();
    #define DEBUG_TIME__RESET __debug_time_start__ = micros();

    // Macro to show the elapsed time with a custom message
    #define DEBUG_TIME__SHOW_MESSAGE(message) \
        SERIAL_DEBUG.printf("%s - Time elapsed: %lu us\n\r", message, micros() - __debug_time_start__); \
        SERIAL_DEBUG.flush();

    // Macro to show the elapsed time with file, function, and line information
    // #define DEBUG_TIME__SHOW \
    //     SERIAL_DEBUG.printf("(%s:%d) %luus\n\r", \
    //                         __FILE__, __LINE__, micros() - __debug_time_start__); \
    //     SERIAL_DEBUG.flush();
    // Macro to show the elapsed time with file, function, and line information  with MS
    #define DEBUG_TIME__SHOW \
        SERIAL_DEBUG.printf("(%s:%d) %luus %dms\n\r", \
                            __FILE__, __LINE__, micros() - __debug_time_start__, (micros() - __debug_time_start__)/1000); \
        SERIAL_DEBUG.flush();
    // Macro to show the elapsed time with file, function, and line information
    #define DEBUG_TIME__SHOW_F \
        SERIAL_DEBUG.printf("Time elapsed: %lu us in function %s (%s:%d)\n", \
                            micros() - __debug_time_start__, __func__, __FILE__, __LINE__); \
        SERIAL_DEBUG.flush();

#else
    #define DEBUG_TIME__START  // No operation if debug is disabled
    #define DEBUG_TIME__SHOW_MESSAGE(message)  // No operation if debug is disabled
    #define DEBUG_TIME__SHOW  // No operation if debug is disabled
#endif



// Added indexing, as nested debug points need different saved start points. 
#ifdef ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING

    #define DEBUG_LIGHTING__START_TIME_RECORDING(X) lighting_time_critical_logging.start_value[X] = micros();
    #define DEBUG_LIGHTING__SAVE_TIME_RECORDING(X, Y)  Y = micros() - lighting_time_critical_logging.start_value[X];

    #define DEBUG_LIGHTING__START_TIME_RECORDING_TASK(X) pCONT_lAni->lighting_time_critical_logging.start_value[X] = micros();
    #define DEBUG_LIGHTING__SAVE_TIME_RECORDING_TASK(X, Y)  pCONT_lAni->Y = micros() - pCONT_lAni->lighting_time_critical_logging.start_value[X];



    

#else

  #define DEBUG_LIGHTING__START_TIME_RECORDING(X) //none
  #define DEBUG_LIGHTING__SAVE_TIME_RECORDING(X,Y) //none
  #define DEBUG_LIGHTING__START_TIME_RECORDING_TASK(X) //none
  #define DEBUG_LIGHTING__SAVE_TIME_RECORDING_TASK(X,Y) //none

#endif // ENABLE_DEBUGFEATURE_LIGHTING__TIME_CRITICAL_RECORDING











#if defined(ENABLE_DEBUG_LINE_HERE)
  #define DEBUG_LINE_HERE_MARKER    SERIAL_DEBUG.printf("DEBUG HERE: -------------------------------------------\n\r");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();
#else
  #define DEBUG_LINE_HERE_MARKER   //nothing, no code
#endif

#if !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE_HERE_PAUSE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();\
                        delay(5000);

#else
  #define DEBUG_LINE_HERE_PAUSE   //nothing, no code
#endif

#if !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_LINE_HERE_SHORT_PAUSE    SERIAL_DEBUG.printf("DEBUG HERE: ");\
                        SERIAL_DEBUG.print(__FILE__);\
                        SERIAL_DEBUG.println(__LINE__);\
                        SERIAL_DEBUG.flush();\
                        delay(1000);

#else
  #define DEBUG_LINE_HERE_PAUSE   //nothing, no code
#endif



#define DEBUG_LINE_HERE_WITH_INT(x)    Serial.printf("DEBUG HERE: [%d] ", x);\
                      Serial.print(__FILE__);\
                      Serial.println(__LINE__);\
                      Serial.flush();
#define DEBUG_LINE_HERE_VALUE(x)    Serial.printf("DEBUG HERE: ");\
                      Serial.print(x);\
                      Serial.print(" ");\
                      Serial.print(__FILE__);\
                      Serial.println(__LINE__);\
                      Serial.flush();

// Can only be used when hardware serial is enabled
#if defined(USE_DEBUG_HOLD_POINT) && !defined(USE_SOFTWARE_SERIAL_DEBUG)
  #define DEBUG_HOLD_POINT   while(1) { \
                              SERIAL_DEBUG.printf("Debug Hold Point: ");\
                              SERIAL_DEBUG.print(__FILE__);\
                              SERIAL_DEBUG.println(__LINE__);\
                              SERIAL_DEBUG.flush(); \
                              delay(1000); \
                            }
#else
  #define DEBUG_HOLD_POINT   //nothing, no code
#endif



#ifdef USE_DEBUG_LINE_LED
  #define DEBUG_LINE_LED    pinMode(D4,OUTPUT);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);\
                            delay(500);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);\
                            delay(500);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);\
                            delay(500);\
                            digitalWrite(D4,LOW);\
                            delay(500);\
                            digitalWrite(D4,HIGH);
#else
  #define DEBUG_LINE_LED   //nothing, no code
#endif


/**
 * @brief Compact ways of adding a log and compile disabling it
 **/
#ifdef ENABLE_LOG_LEVEL_ERROR
#define ALOG_ERR(...) AddLog(LOG_LEVEL_ERROR, __VA_ARGS__)
#else
#define ALOG_ERR(...)
#endif

#ifdef ENABLE_LOG_LEVEL_WARNING
#define ALOG_WRN(...) AddLog(LOG_LEVEL_WARNING, __VA_ARGS__)
#else
#define ALOG_WRN(...)
#endif

#ifdef ENABLE_LOG_LEVEL_IMPORTANT
#define ALOG_IMP(...) AddLog(LOG_LEVEL_IMPORTANT, __VA_ARGS__)
#else
#define ALOG_IMP(...)
#endif

#ifdef ENABLE_LOG_LEVEL_INFO
#define ALOG_INF(...) AddLog(LOG_LEVEL_INFO, __VA_ARGS__)
#else
#define ALOG_INF(...)
#endif

#ifdef ENABLE_LOG_LEVEL_DEBUG
#define ALOG_DBG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define ALOG_DBG(...)
#endif

#ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
#define ALOG_DBM(...) AddLog(LOG_LEVEL_DEBUG_MORE,  __VA_ARGS__)
#else
#define ALOG_DBM(...)
#endif

#ifdef ENABLE_LOG_LEVEL__DEBUG_TRACE
#define ALOG_TRA(...) AddLog(LOG_LEVEL_DEBUG_TRACE, __VA_ARGS__)
#else
#define ALOG_TRA(...)
#endif

#ifdef ENABLE_LOG_LEVEL_COMMANDS
#define ALOG_COM(...) AddLog(LOG_LEVEL_COMMANDS, __VA_ARGS__)
#else
#define ALOG_COM(...)
#endif

#ifdef ENABLE_LOG_LEVEL_HIGHLIGHT
#define ALOG_HGL(...) AddLog(LOG_LEVEL_HIGHLIGHT, __VA_ARGS__)
#else
#define ALOG_HGL(...)
#endif

#ifdef ENABLE_LOG_LEVEL_TEST
#define ALOG_TST(...) AddLog(LOG_LEVEL_TEST, __VA_ARGS__)
#else
#define ALOG_TST(...)
#endif


#define ALOG_DEBUG_LINE_HERE ALOG_DBG(PSTR("DP:%s|%d"),__FILE__,__LINE__);


#define DEBUG_CRITICAL_FORCE_CRASH  volatile uint32_t dummy; \
                              dummy = *((uint32_t*) 0x00000000);


#include "2_CoreSystem/07_Time/mTime.h"
class mTime;

#ifdef ESP8266
extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}
#endif // ESP8266


#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/01_Settings/mSettings.h"


#define D_LOG_LEVEL_NONE_CTR            "NON"
#define D_LOG_LEVEL_DEBUG_TRACE_CTR     "DTR"
#define D_LOG_LEVEL_ERROR_CTR           "ERR"
#define D_LOG_LEVEL_WARN_CTR            "WRN"
#define D_LOG_LEVEL_TEST_CTR            "TST"
#define D_LOG_LEVEL_IMPORTANT_CTR       "IMP"
#define D_LOG_LEVEL_INFO_CTR            "INF"
#define D_LOG_LEVEL_HIGHLIGHT_CTR       "HGL"
#define D_LOG_LEVEL_COMMANDS_CTR        "INP"
#define D_LOG_LEVEL_DEBUG_CTR           "DBG"
#define D_LOG_LEVEL_DEBUG_MORE_CTR      "DBM"
#define D_LOG_LEVEL_DEBUG_LOWLEVEL_CTR  "DBL"
#define D_LOG_LEVEL_ALL_CTR             "ALL"

DEFINE_PGM_CTR(PM_LOG_LEVEL_NONE_CTR)            D_LOG_LEVEL_NONE_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_TRACE_CTR)     D_LOG_LEVEL_DEBUG_TRACE_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_ERROR_CTR)           D_LOG_LEVEL_ERROR_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_WARN_CTR)            D_LOG_LEVEL_WARN_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_TEST_CTR)            D_LOG_LEVEL_TEST_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_HIGHLIGHT_CTR)       D_LOG_LEVEL_HIGHLIGHT_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_IMPORTANT_CTR)       D_LOG_LEVEL_IMPORTANT_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_INFO_CTR)            D_LOG_LEVEL_INFO_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_COMMANDS_CTR)        D_LOG_LEVEL_COMMANDS_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_CTR)           D_LOG_LEVEL_DEBUG_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_MORE_CTR)      D_LOG_LEVEL_DEBUG_MORE_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_DEBUG_LOWLEVEL_CTR)  D_LOG_LEVEL_DEBUG_LOWLEVEL_CTR;
DEFINE_PGM_CTR(PM_LOG_LEVEL_ALL_CTR)             D_LOG_LEVEL_ALL_CTR;


//https://eli.thegreenplace.net/2014/variadic-templates-in-c/

// template<typename T, typename U>
// void AddLog_Array3(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len);


// /**
//  * @brief Form into message then pass into normal AddLog
//  * 
//  * @tparam T 
//  * @tparam U 
//  * @param loglevel 
//  * @param name_ctr 
//  * @param arr 
//  * @param arr_len 
//  */
// template<typename T, typename U>
// void AddLog_Array3(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {

//   // form into string then send to normal addlog

//   // char buffer[100] = {0}; // short buffer
//   // uint16_t buflen = 0;

//   // buflen += snprintf(buffer+buflen, sizeof(buffer), "AddLog_Array3 %s = ", name_ctr);

//   // AddLog(loglevel, "%s", buffer);


//   // #ifndef DISABLE_SERIAL_LOGGING
//   // SERIAL_DEBUG.printf("%s = ",name_ctr);
//   // for(T index=0;index<arr_len;index++){
//   //   SERIAL_DEBUG.printf("%d,", arr[index]);
//   // }
//   // SERIAL_DEBUG.printf("\n\r");
//   // #endif

// };




void AddLog(uint8_t loglevel, PGM_P formatP, ...);


void AddLog(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, PGM_P formatP, ...);




// void AddLog(PGM_P formatP, ...);
// void AddSerialLog_mP2(uint8_t loglevel, PGM_P formatP, ...);
int Response_mP(const char* format, ...);
int ResponseAppend_mP(const char* format, ...);
void AddLog_NoTime(uint8_t loglevel, PGM_P formatP, ...);


// template<typename T, typename U>
// void AddLog_Array(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {

//   // if(
//   //   (loglevel>pCONT_set->Settings.logging.serial_level)&&
//   //   (loglevel>pCONT_set->Settings.logging.web_level)
//   //   ){
//   //   return;
//   // }  

//   // if(loglevel>pCONT_set->Settings.logging.serial_level){
//   //   return;
//   // }
  
//   #ifndef DISABLE_SERIAL_LOGGING
//   SERIAL_DEBUG.printf("%s = ",name_ctr);
//   for(T index=0;index<arr_len;index++){
//     SERIAL_DEBUG.printf("%d,", arr[index]);
//   }
//   SERIAL_DEBUG.printf("\n\r");
//   #endif

// }

// template<typename T, typename U>
// void AddLog_Array(const char* name_ctr, T* arr, U arr_len)
// {
  
//   #ifndef DISABLE_SERIAL_LOGGING
//   SERIAL_DEBUG.printf("%s = ",name_ctr);
//   for(T index=0;index<arr_len;index++){
//     SERIAL_DEBUG.printf("%d,", arr[index]);
//   }
//   SERIAL_DEBUG.printf("\n\r");
//   #endif

// }


// /**
//  * @brief Form into message then pass into normal AddLog
//  * 
//  * @tparam T 
//  * @tparam U 
//  * @param loglevel 
//  * @param name_ctr 
//  * @param arr 
//  * @param arr_len 
//  */
// template<typename T, typename U>
// void AddLog_Array2(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {

//   // form into string then send to normal addlog

//   char buffer[100] = {0}; // short buffer
//   uint16_t buflen = 0;

//   buflen += snprintf(buffer+buflen, sizeof(buffer), "AddLog_Array2 %s = ", name_ctr);

//   AddLog(loglevel, buffer);


//   // #ifndef DISABLE_SERIAL_LOGGING
//   // SERIAL_DEBUG.printf("%s = ",name_ctr);
//   // for(T index=0;index<arr_len;index++){
//   //   SERIAL_DEBUG.printf("%d,", arr[index]);
//   // }
//   // SERIAL_DEBUG.printf("\n\r");
//   // #endif

// }


enum ERROR_MESSAGE_TYPES
{
  ERROR_MESSAGE_TYPE_INVALID_FORMAT=0
};

void ErrorMessage_P(uint8_t error_type, const char* message);
void ErrorMessage(uint8_t error_type, const char* message);

template<typename T, typename U>
void AddLog_Array(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
{
    // Create a buffer to store the log message
    char logBuffer[512];  // Adjust the size if needed
    char* logPointer = logBuffer;

    // Add the array name to the log message
    size_t written = snprintf(logPointer, sizeof(logBuffer), "%s = ", name_ctr);
    logPointer += written;

    // Add array elements to the log message
    for (U index = 0; index < arr_len && (logPointer - logBuffer) < sizeof(logBuffer) - 10; ++index) {  // Limit size to avoid buffer overflow
        written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d,", arr[index]);
        logPointer += written;
    }

    // Null-terminate the string
    if ((logPointer - logBuffer) < sizeof(logBuffer)) {
        *logPointer = '\0';
    }

    // Pass the formatted string to AddLog
    AddLog(loglevel, PSTR("%s"), logBuffer);
}
template<typename T, typename U>
void AddLog_Array_Block(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len, U arr_width, bool use_tabs)
{
    // Create a buffer to store the log message
    char logBuffer[512];  // Adjust the size if needed
    char* logPointer = logBuffer;

    // Add the array name to the log message and start a new line
    size_t written = snprintf(logPointer, sizeof(logBuffer), "\r\n%s =\r\n", name_ctr);
    logPointer += written;

    // Add array elements to the log message
    for (U index = 0; index < arr_len && (logPointer - logBuffer) < sizeof(logBuffer) - 10; ++index) {
        // Write the value to the buffer
        if (use_tabs) {
            written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d\t", arr[index]);
        } else {
            written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d,", arr[index]);
        }
        logPointer += written;

        // Insert a new line every 'arr_width' values
        if ((index + 1) % arr_width == 0) {
            written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "\r\n");
            logPointer += written;
        }
    }

    // Null-terminate the string
    if ((logPointer - logBuffer) < sizeof(logBuffer)) {
        *logPointer = '\0';
    }

    // Pass the formatted string to AddLog
    AddLog(loglevel, PSTR("%s"), logBuffer);
}


// template<typename T, typename U>
// void AddLog_Array_P(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {
//   T ch;
//   #ifndef DISABLE_SERIAL_LOGGING
//   SERIAL_DEBUG.printf("%S = ",name_ctr);
//   for(T index=0;index<arr_len;index++){
//     ch = pgm_read_byte(arr + index);
//     SERIAL_DEBUG.printf("%d,", ch);
//   }
//   SERIAL_DEBUG.printf("\n\r");
//   #endif
// }


// template<typename T, typename U>
// void AddLog_Array_P(const char* name_ctr, T* arr, U arr_len)
// {
//   T ch;
//   #ifndef DISABLE_SERIAL_LOGGING
//   SERIAL_DEBUG.printf("%S = ",name_ctr);
//   for(T index=0;index<arr_len;index++){
//     ch = pgm_read_byte(arr + index);
//     SERIAL_DEBUG.printf("%d,", ch);
//   }
//   SERIAL_DEBUG.printf("\n\r");
//   #endif
// }

template<typename T, typename U>
void AddLog_Array_P(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
{
    // Create a buffer to store the log message
    char logBuffer[512];  // Adjust the size if needed
    char* logPointer = logBuffer;

    // Add the array name to the log message
    size_t written = snprintf(logPointer, sizeof(logBuffer), "%s = ", name_ctr);
    logPointer += written;

    // Add array elements to the log message
    for (U index = 0; index < arr_len && (logPointer - logBuffer) < sizeof(logBuffer) - 10; ++index) {  // Limit size to avoid buffer overflow
        int value = pgm_read_byte(arr + index);
        written = snprintf(logPointer, sizeof(logBuffer) - (logPointer - logBuffer), "%d,", value);
        logPointer += written;
    }

    // Null-terminate the string
    if ((logPointer - logBuffer) < sizeof(logBuffer)) {
        *logPointer = '\0';
    }

    // Pass the formatted string to AddLog
    AddLog(loglevel, PSTR("%s"), logBuffer);
}



// template<typename T>
// void AddLog_Array(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, const char* name_ctr, T* arr, T arr_len)//}, uint8_t fWithIndex = 0, uint8_t fVertical = 0)
// {
//   uint32_t time_now = *tSaved; //to allow compile of newer esp32
//   if(abs(
    
//     static_cast<long long>(millis()-time_now)
    
//     )>=limit_ms){ *tSaved=millis();
//     AddLog_Array(loglevel,name_ctr,arr,arr_len);
//   }
// }


// #define TEST_SINGLETON

#ifdef TEST_SINGLETON
// mLogging* mLogging::mso3 = nullptr;
#endif

#include "1_TaskerManager/mTaskerInterface.h"

class mLogging :
  public mTaskerInterface
{
public:
    mLogging(){}; // Class constructor
    
    void init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);


    static constexpr const char* PM_MODULE_CORE_LOGGING_CTR = D_MODULE_CORE_LOGGING_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_LOGGING_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_LOGGING_ID; }

    void handleTelnet();

    enum DEBUG_OUTPUT_IDS{
      DEBUG_OUTPUT_HARDWARE_ID=0,
      DEBUG_OUTPUT_SOFTWARE_ID
    };

     
    #ifdef USE_SOFTWARE_SERIAL_DEBUG
      uint8_t fDebugOutputMode = DEBUG_OUTPUT_SOFTWARE_ID;
    #else
      uint8_t fDebugOutputMode = DEBUG_OUTPUT_HARDWARE_ID;
    #endif

     
    #ifdef DISABLE_SERIAL_LOGGING
      uint8_t fDebugSerialMode = 1;
    #else
      uint8_t fDebugSerialMode = 0;
    #endif
    //mode to include
    // OFF, ON, TIMED_10_MINUTES_FROM_BOOT, TIMED_MINUTES_FROM_USER_REQUEST

 
    void GetLog(uint8_t idx, char** entry_pp, size_t* len_p);
    void SetSeriallog(uint8_t loglevel);
    void Syslog(void);
    void AddLogAddLog(uint8_t loglevel);
    void AddLog(uint8_t loglevel, const char *formatP);
    void AddLog(uint8_t loglevel, const char *formatP, const char *formatP2);
    void AddLog_P2(uint8_t loglevel, PGM_P formatP, ...);
    void AddLogBuffer(uint8_t loglevel, uint8_t *buffer, int count);
    void AddLogSerial(uint8_t loglevel);
    void AddLogMissed(char *sensor, uint8_t misses);

    
/****
 * Internal buffers, should I move these elsewhere? They are not settings, logs??
*/

#ifndef WEB_LOG_SIZE
#define WEB_LOG_SIZE 200       // Max number of characters in weblog
#endif // WEB_LOG_SIZE
#ifndef LOG_BUFFER_SIZE
#ifdef ESP8266
#define LOG_BUFFER_SIZE 400 //if debug is enabled, push this to 1000, if not, keep at much smaller 300
#else //esp32
#define LOG_BUFFER_SIZE 1000
#endif
#endif // LOG_BUFFER_SIZE
char log_data[LOG_BUFFER_SIZE];                       // Logging
char web_log[WEB_LOG_SIZE] = {'\0'};        // Web log buffer - REMEMBERS EVERYTHING for new load
uint8_t web_log_index = 1;                  // Index in Web log buffer (should never be 0)

// template<typename T, typename U>
// void AddLog_Array4(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len);

// template<typename T, typename U>
// void AddLog_Array4(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {


//   char buffer[100] = {0}; // short buffer
//   uint16_t buflen = 0;

//   buflen += snprintf(buffer+buflen, sizeof(buffer), "AddLog_Array4 %s = ", name_ctr);

//   AddLog(loglevel, buffer);



// }



    const char* GetLogLevelNamebyID(uint8_t id);
    uint8_t GetLogLevelIDbyName(const char* name);

    void StartTelnetServer();

    #define TELNET_PORT 23

    WiFiServer* TelnetServer;
    WiFiClient Telnet;

    bool telnet_started = false;
    
};

// // need to work out type for this to be generic using json methods
// template<typename T, typename U>
// void AddLog_Array5(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {
  
//   char buffer[100] = {0}; // short buffer
//   uint16_t buflen = 0;

//   buflen += snprintf(buffer+buflen, sizeof(buffer), "AddLog_Array4 %s = ", name_ctr);

//   for(T index=0;index<arr_len;index++)
//   {
//     buflen += snprintf(buffer+buflen, sizeof(buffer), "%d%s", 
//       arr[index], 
//       index==arr_len ? "," : ""      
//     );
//   }

//   AddLog(loglevel, "%s", buffer);

// }


// template<typename T, typename U>
// void AddLog_Array_Int(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {
  
//   char buffer[100] = {0}; // short buffer
//   uint16_t buflen = 0;

//   buflen += snprintf(buffer+buflen, sizeof(buffer), "Array_Int %s = ", name_ctr);

//   for(T index=0;index<arr_len;index++)
//   {
//     buflen += snprintf(buffer+buflen, sizeof(buffer), "%d%s", 
//       arr[index], 
//       index<(arr_len-1) ? "," : ""      
//     );
//   }

//   AddLog(loglevel, "%s", buffer);

// }



#endif // header guard
