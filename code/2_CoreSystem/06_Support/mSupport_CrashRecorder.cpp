
#include "mSupport.h"

// #ifdef ESP8266
// extern "C" {
// #include <cont.h>
//   extern cont_t* g_pcont;
// }

// void mSupport::DebugFreeMem(void)
// {
//   //https://www.esp8266.com/viewtopic.php?p=69937
//   //https://i.stack.imgur.com/waoHN.gif
//   register uint32_t *sp asm("a1");

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d"), 
//       ESP.getFreeHeap(), 4 * (sp - g_pcont->stack));
//     #endif// ENABLE_LOG_LEVEL_INFO
// }

// // #endif  // ARDUINO_ESP8266_RELEASE_2_x_x

// #endif // ESP8266



#ifdef ESP8266
// All version from core 2.4.2
// https://github.com/esp8266/Arduino/pull/5018
// https://github.com/esp8266/Arduino/pull/4553

extern "C" {
#include <cont.h>
  extern cont_t* g_pcont;
}

void DebugFreeMem(void) {
  register uint32_t *sp asm("a1");

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), 4 * (sp - g_pcont->stack), XdrvMailbox.data);
}

uint32_t FreeStack(void) {
  register uint32_t *sp asm("a1");
  return 4 * (sp - g_pcont->stack);
}

void AddLogMem(const char* function) {
  register uint32_t *sp asm("a1");
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "== %s FreeRam %d, FreeStack %d"), function, ESP.getFreeHeap(), 4 * (sp - g_pcont->stack));
}

#endif  // ESP8266
#ifdef ESP32

void DebugFreeMem(void) {
  register uint8_t *sp asm("a1");

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "FreeRam %d, FreeStack %d (%s)"), ESP.getFreeHeap(), sp - pxTaskGetStackStart(NULL), XdrvMailbox.data);
}

uint32_t FreeStack(void) {
  register uint8_t *sp asm("a1");
  return sp - pxTaskGetStackStart(NULL);
}

void AddLogMem(const char* function) {
  register uint8_t *sp asm("a1");
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "== %s FreeRam %d, FreeStack %d"), function, ESP.getFreeHeap(), sp - pxTaskGetStackStart(NULL));
}

#endif  // ESP8266 - ESP32


//#ifdef ENABLE_CRASH_RECORDING

/**
 * Save crash information in RTC memory
 * This function is called automatically if ESP8266 suffers an exception
 * It should be kept quick / consise to be able to execute before hardware wdt may kick in
 */
extern "C" void custom_crash_callback(struct rst_info * rst_info, uint32_t stack, uint32_t stack_end )
{
  uint32_t addr_written = 0;      // how many addresses have we already written in RTC
  uint32_t value;                 // 4 bytes buffer to write to RTC

    #ifdef ESP8266
  for (uint32_t i = stack; i < stack_end; i += 4) {
    value = *((uint32_t*) i);     // load value from stack
    if ((value >= 0x40000000) && (value < 0x40300000)) {  // keep only addresses in code area
      ESP.rtcUserMemoryWrite(crash_rtc_offset + addr_written, (uint32_t*)&value, sizeof(value));
      addr_written++;
      if (addr_written >= crash_dump_max_len) { break; }  // we store only 31 addresses
    }
  }
  value = crash_magic + addr_written;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  #endif // ESP8266
}

// Generate a crash to test the crash recorder
void mSupport::CmndCrash(void)
{
  volatile uint32_t dummy;
  dummy = *((uint32_t*) 0x00000000);
}

// Do an infinite loop to trigger WDT watchdog
void mSupport::CmndWDT(void)
{
  volatile uint32_t dummy = 0;
  while (1) {
    dummy++;
  }
}

// This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
void mSupport::CmndBlockedLoop(void)
{
  DEBUG_PRINT_FUNCTION_NAME;
  while (1) {
    delay(1000);
  }
}

// Clear the RTC dump counter when we do a normal reboot, this avoids garbage data to stay in RTC
void mSupport::CrashDumpClear(void)
{
  DEBUG_PRINT_FUNCTION_NAME;
  return;
    #ifdef ESP8266
  uint32_t value = 0;
  ESP.rtcUserMemoryWrite(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  #endif // ESP8266
}



/*********************************************************************************************\
 * CmndCrashDump - dump the crash history - called by `Status 12`
\*********************************************************************************************/

bool mSupport::CrashFlag(void)
{
  
#ifdef ESP8266
  DEBUG_PRINT_FUNCTION_NAME;
  return ((ResetReason() == REASON_EXCEPTION_RST) || (ResetReason() == REASON_SOFT_WDT_RST));// || oswatch_blocked_loop);
#endif // ESP8266

}

void mSupport::CrashDump_AddJson(void)
{
  
#ifdef ESP8266
  char buffer[30];

  DEBUG_PRINT_FUNCTION_NAME;
  pCONT_sup->WriteBuffer_P(PSTR("\"Exception\":%d,\"Reason\":\"%s\",\"EPC\":[\"%08x\",\"%08x\",\"%08x\"],\"EXCVADDR\":\"%08x\",\"DEPC\":\"%08x\""),
    resetInfo.exccause,        // Exception Cause
    pCONT_sup->GetResetReason(buffer, sizeof(buffer)),  // Reset Reason
    resetInfo.epc1,            // Exception Progam Counter
    resetInfo.epc2,            // Exception Progam Counter - High-Priority Interrupt 1
    resetInfo.epc3,            // Exception Progam Counter - High-Priority Interrupt 2
    resetInfo.excvaddr,        // Exception Virtual Address Register - Virtual address that caused last fetch, load, or store exception
    resetInfo.depc             // Double Exception Program Counter
  );

  uint32_t value;
  ESP.rtcUserMemoryRead(crash_rtc_offset + crash_dump_max_len, (uint32_t*)&value, sizeof(value));
  if (crash_magic == (value & 0xFFFFFF00)) {
    pCONT_sup->WriteBuffer_P(PSTR(",\"CallChain\":["));
    uint32_t count = value & 0x3F;
    for (uint32_t i = 0; i < count; i++) {
      ESP.rtcUserMemoryRead(crash_rtc_offset +i, (uint32_t*)&value, sizeof(value));
      if (i > 0) { pCONT_sup->WriteBuffer_P(PSTR(",")); }
      pCONT_sup->WriteBuffer_P(PSTR("\"%08x\""), value);
    }
    pCONT_sup->WriteBuffer_P(PSTR("]"));
  }
  #endif // ESP8266
  
}

// #endif
