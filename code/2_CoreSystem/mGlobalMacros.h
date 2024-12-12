
#ifndef _GLOBAL_MACROS_H_
#define _GLOBAL_MACROS_H_ 1

#ifdef ESP8266
#include <avr/pgmspace.h>
#endif // ESP8266
#ifdef ESP32
#define PROGMEM // is nothing, as "const" is enough in esp32 to push to progmem space
#endif

#define D_MAX_UINT8  255
#define D_MAX_UINT16 65535

#define DEFINE_PGM_CTR(X) \
  const char X[] PROGMEM =
#define DEFINE_SPGM_CTR(X) \
  static const char X[] PROGMEM =
#define DEFINE_PROGMEM_CTR2(X,Y) \
  const char X[] PROGMEM = Y;

// fixing "abs" on esp32 later platform API

// #define ABS_FUNCTION(x) static_cast<long long int>abs(x)

#define ABS_FUNCTION(x) llabs(x)

// enable additional debug output
// Need to add disables here! 
#define DEBUG_PRINT Serial.print
#define DEBUG_PRINTLN Serial.println
#define DEBUG_PRINTF Serial.printf
#define DEBUG_PRINTF_P(x...) Serial.printf_P(x)

#define DEBUG_TASMOTA_CORE

#ifdef DEBUG_TASMOTA_CORE
#define DEBUG_LOG_CORE(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_LOG_CORE(...)
#endif
#ifdef DEBUG_TASMOTA_DRIVER
#define DEBUG_LOG_DRIVER(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_LOG_DRIVER(...)
#endif
#ifdef DEBUG_TASMOTA_SENSOR
#define DEBUG_LOG_SENSOR(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_LOG_SENSOR(...)
#endif
// #ifdef DEBUG_TASMOTA_TRACE
// #define DEBUG_TRACE_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
// #else
// #define DEBUG_TRACE_LOG(...)
// #endif

#define DIGITAL_INVERT_PIN(pin) digitalWrite(pin,!digitalRead(pin))



#define SPF(param_name,string_type,parameter)  Serial.printf("\"" param_name "\"=" string_type "\n\r", parameter)
  

#define DEBUGGING_PROCEED_ONLY_AFTER_MILLIS_SHOWS_STABLE(X) \
              if(millis()<60000){ return X; } 

// #define ENABLE_DEBUG_SERIAL //USE_SOFTWARE_SERIAL_DEBUG

#if defined(ENABLE_DEBUG_SERIAL)
  // #define DEBUG_PRINT_FUNCTION_NAME   SERIAL_DEBUG.print(__FILE__);\
  //                                     SERIAL_DEBUG.print("\t");\
  //                                     SERIAL_DEBUG.println(__FUNCTION__);\
  //                                     SERIAL_DEBUG.print("\t");\
  //                                     SERIAL_DEBUG.println(__LINE__);\
  //                                     SERIAL_DEBUG.flush();
                                      
#define DEBUG_PRINT Serial.print
#define DEBUG_PRINTLN Serial.println
#define DEBUG_PRINTF Serial.printf
#else
  // #define DEBUG_PRINT_FUNCTION_NAME   //nothing, no code
  
#define DEBUG_PRINT //Serial.print
#define DEBUG_PRINTLN //Serial.println
#define DEBUG_PRINTF //Serial.printf
#endif

// #define P_CHECK(x) if(x!-nullptr)


#ifdef ESP8266
  #define APPEND_ESP_TYPE_MQTT_STRING "_esp8266"
  #define APPEND_ESP_TYPE_NAME_STRING " ESP8266"
#endif
#ifdef ESP32
  #define APPEND_ESP_TYPE_MQTT_STRING "_esp32"
  #define APPEND_ESP_TYPE_NAME_STRING " ESP32"
#endif



#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
#define R32(c) (byte((c) >> 16))
#define G32(c) (byte((c) >> 8))
#define B32(c) (byte(c))
#define W32(c) (byte((c) >> 24))


/*
// ArduinoJson relies on the type const __FlashStringHelper* to detect if a string is in Flash.

// If you use the F() macro as above, you don’t have to worry about it. However, if you use a char[] with the PROGMEM attribute, you must cast the pointer before passing it to ArduinoJson. Here is an example:

// const char myValue[] PROGMEM = "hello world";
// root["message"] = (const __FlashStringHelper*)myValue;
// This gotcha is not limited to ArduinoJson; many Arduino functions, like Serial.println(), need this cast too.
// //https://arduinojson.org/v6/api/config/enable_progmem/


// RAM:   [======    ]  56.2% (used 46040 bytes from 81920 bytes)
// Flash: [======    ]  56.4% (used 577724 bytes from 1023984 bytes)


// */

// #define USE_DEVFEATURE_JSON_COMMANDS_IN_PROGMEM




// #ifdef USE_DEVFEATURE_JSON_COMMANDS_IN_PROGMEM
//   #define CFLASH  (const __FlashStringHelper*)    //cast flash needed for AJ type check
//   //to allow json testing along with cflash, turning off progme
//   #define DEFINE_PGM_CTR(X) \
//     const char X[] PROGMEM =
// #else
//   #define CFLASH  //(const __FlashStringHelper*)    //cast flash needed for AJ type check
//   //to allow json testing along with cflash, turning off progme
//  #define DEFINE_PGM_CTR(X) \
//   const char X[] =
// #endif // USE_DEVFEATURE_JSON_COMMANDS_IN_PROGMEM
  


#define PRINT_FLUSHED(X) Serial.printf(X); \
                        Serial.println(); \
                        Serial.flush();

#define DEBUG_INSERT_PAGE_BREAK "\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r"
#define D_DEBUG_FUNCTION_NAME_CTR __func__

                      
#define SUBDEFINE_GPIO_TEMPLATE_PIN(X,Y) "\"" X "\":\"" Y "\","            
#define SUBDEFINE_GPIO_TEMPLATE_PIN_END(X,Y) "\"" X "\":\"" Y "\""


#define DEFINE_APP_SVALUE(X,Y) "\"" X "\":\"" Y "\","    //append
#define DEFINE_END_SVALUE(X,Y) "\"" X "\":\"" Y "\""   


#define STR_HELPER2(x) #x
#ifndef STR2
#define STR2(x) STR_HELPER2(x)
#endif




#define DEF_PGM_CTR(X) \
  const char X[] PROGMEM =
#define DEF_CPGM_CTR(X) \
  char X[] PROGMEM =
#define DEF_PGM_UINT8(X) \
  const uint8_t X[] PROGMEM =
#define DEF_CPGM_UINT8(X) \
  uint8_t X[] PROGMEM =


#define IS_WITHIN_RANGE(X,Y,Z) ((X>Y)&&(X<Z))
#define IS_WITHIN_RANGE_MAXINC(X,Y,Z) ((X>Y)&&(X<=Z))  // max inclusive




// #define SET_BUFFER_AS_GLOBAL_OR_LOCAL(X,Y) \
//           if(Y == nullptr){ \
//             X = pCONT_sup->global_buffer; \
//           }else{ \
//             X = Y; \
//           }

// #define BUFFER_ISREADY() strlen(data_buffer.payload.ctr)?1:0


#endif