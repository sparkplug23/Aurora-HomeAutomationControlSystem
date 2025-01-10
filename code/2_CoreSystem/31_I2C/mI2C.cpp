#include "mI2C.h"

#ifdef USE_MODULE_CORE_I2C

int8_t mI2C::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  // switch(function){
  //   /************
  //    * PERIODIC SECTION * 
  //   *******************/
  //   case TASK_EVERY_SECOND:  
  //   break;
  //    case TASK_LOOP:
  //   //   UfsExecuteCommandFileLoop();
  //   break;
  //   case TASK_EVERY_FIVE_SECOND:    
  //     #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  //     SystemTask__Execute_Module_Data_Save();
  //     #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  //   break;
  //   case TASK_EVERY_MINUTE:
  //     // #ifdef ENABLE_DEVFEATURE__SAVE_MODULE_DATA // This will in the future only occur once an hour, or before planned boot
  //     #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  //     SystemTask__Execute_Module_Data_Save();
  //     #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  //     // #endif     
  //   break;  
  //   case TASK_EVERY_FIVE_MINUTE:
  //     #ifdef ENABLE_SYSTEM_SETTINGS_IN_I2C
  //     JsonFile_Save__Stored_Module();
  //     JsonFile_Save__Stored_Secure();
  //     #endif // ENABLE_SYSTEM_SETTINGS_IN_I2C
       
  //   break;
  //   /************
  //    * COMMANDS SECTION * 
  //   *******************/
  //   case TASK_JSON_COMMAND_ID:
  //     parse_JSONCommand(obj);
  //   break;
  //   /************
  //    * MQTT SECTION * 
  //   *******************/
  //   #ifdef USE_MODULE_NETWORK_MQTT
  //   case TASK_MQTT_HANDLERS_INIT:
  //     MQTTHandler_Init();
  //   break;
  //   case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
  //     MQTTHandler_Rate();
  //   break;
  //   case TASK_MQTT_SENDER:
  //     MQTTHandler_Sender();
  //   break;
  //   case TASK_MQTT_CONNECTED:
  //     MQTTHandler_RefreshAll();
  //   break;
  //   #endif //USE_MODULE_NETWORK_MQTT

  //   // case TASK_MQTT_INIT:
  //   //   if (!TasmotaGlobal.no_autoexec) {
  //   //     UfsExecuteCommandFile(TASM_FILE_AUTOEXEC);
  //   //   }
  //   //   break;
  //   // case TASK_COMMAND:
  //   //   result = DecodeCommand(kUFSCommands, kUFSCommand);
  //   //   break;
  //   #ifdef USE_WEBSERVER
  //   case TASK_WEB_ADD_MANAGEMENT_BUTTON:
  //     if (ufs_type) {
  //     WSContentSend_PD(UFS_WEB_DIR, PSTR(D_MANAGE_FILE_SYSTEM));
  //     }
  //     break;
  //     case TASK_WEB_ADD_HANDLER:
  //     //      Webserver->on(F("/ufsd"), UfsDirectory);
  //     //      Webserver->on(F("/ufsu"), HTTP_GET, UfsDirectory);
  //     //      Webserver->on(F("/ufsu"), HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
  //     Webserver->on("/ufsd", UfsDirectory);
  //     Webserver->on("/ufsu", HTTP_GET, UfsDirectory);
  //     Webserver->on("/ufsu", HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
  //     break;
  //   #endif // USE_WEBSERVER

  //   }  

} // END Tasker



void mI2C::Pre_Init()
{
  

/**
 * @brief Configure I2C
 * 
 * 
 * This section should change "I2CBegin()" so esp8266/esp32 cna be handled with compat header
 * 
 */

  tkr_set->runtime.i2c_enabled = ( pCONT_pins->PinUsed(GPIO_I2C_SCL_ID) && pCONT_pins->PinUsed(GPIO_I2C_SDA_ID));
  if (tkr_set->runtime.i2c_enabled)
  { 
    if(wire == nullptr)
    {
      #ifdef ESP8266
      wire = new TwoWire();
      wire->begin(pCONT_pins->GetPin(GPIO_I2C_SDA_ID), pCONT_pins->GetPin(GPIO_I2C_SCL_ID)); // no return to check status
      #else
      wire = new TwoWire(0);
      ALOG_DBM( PSTR("Trying to start i2c 2-wire"));
      #ifdef ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
      if(wire->begin(pCONT_pins->GetPin(GPIO_I2C_SDA_ID), pCONT_pins->GetPin(GPIO_I2C_SCL_ID)))//, 100000))
      #else
      if(wire->begin(pCONT_pins->GetPin(GPIO_I2C_SDA_ID), pCONT_pins->GetPin(GPIO_I2C_SCL_ID), 100000))
      #endif // ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
      {
        ALOG_DBM( PSTR("STARTED to start i2c 2-wire sda%d scl%d"),pCONT_pins->GetPin(GPIO_I2C_SDA_ID),pCONT_pins->GetPin(GPIO_I2C_SCL_ID));
      }
      else
      {
        ALOG_DBM( PSTR("NOT STARTED to start i2c 2-wire"));
      }
      #endif
    }   

  } // i2c_enabled
  
}




void mI2C::Debug_I2CScan_To_Serial()
{
 
  if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID)&&pCONT_pins->PinUsed(GPIO_I2C_SDA_ID))
  {

    #ifdef ESP32
    JBI->Add("I2C_BusSpeed", wire->getClock());
    #endif

    char mqtt_data[300];
    I2cScan(mqtt_data, sizeof(mqtt_data));
    // Serial.println(mqtt_data);

    //need to escape option to function above
    // JBI->Add("I2C_Scan",          mqtt_data);

    BufferWriterI->Start();
    BufferWriterI->Append("\"I2C_Scan\":");
    BufferWriterI->Append(mqtt_data);

    ALOG_INF( PSTR("buffer=%s"), BufferWriterI->GetBufferPtr() );

  }

}

bool mI2C::I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size)
{
  uint8_t x = I2C_RETRY_COUNTER;

  i2c_buffer = 0;
  do {
    wire->beginTransmission(addr);                       // start transmission to device
    wire->write(reg);                                    // sends register address to read from
    if (0 == wire->endTransmission(false)) {             // Try to become I2C Master, send data and collect bytes, keep master status for next request...
      wire->requestFrom((int)addr, (int)size);           // send data n-bytes read
      if (wire->available() == size) {
        for (uint8_t i = 0; i < size; i++) {
          i2c_buffer = i2c_buffer << 8 | wire->read();   // receive DATA
        }
      }
    }
    x--;
  } while (wire->endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool mI2C::I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 1);
  *data = (uint8_t)i2c_buffer;
  return status;
}

bool mI2C::I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (uint16_t)i2c_buffer;
  return status;
}

bool mI2C::I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 2);
  *data = (int16_t)i2c_buffer;
  return status;
}

bool mI2C::I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16(&ldata, addr, reg);
  *data = (ldata >> 8) | (ldata << 8);
  return status;
}

bool mI2C::I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg)
{
  uint16_t ldata;
  bool status = I2cValidRead16LE(&ldata, addr, reg);
  *data = (int16_t)ldata;
  return status;
}

bool mI2C::I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg)
{
  bool status = I2cValidRead(addr, reg, 3);
  *data = i2c_buffer;
  return status;
}

uint8_t mI2C::I2cRead8(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 1);
  return (uint8_t)i2c_buffer;
}

uint16_t mI2C::I2cRead16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (uint16_t)i2c_buffer;
}

int16_t mI2C::I2cReadS16(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  return (int16_t)i2c_buffer;
}

uint16_t mI2C::I2cRead16LE(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 2);
  uint16_t temp = (uint16_t)i2c_buffer;
  return (temp >> 8) | (temp << 8);
}

int16_t mI2C::I2cReadS16_LE(uint8_t addr, uint8_t reg)
{
  return (int16_t)I2cRead16LE(addr, reg);
}

int32_t mI2C::I2cRead24(uint8_t addr, uint8_t reg)
{
  I2cValidRead(addr, reg, 3);
  return i2c_buffer;
}



bool mI2C::I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size)
{
  uint8_t x = I2C_RETRY_COUNTER;

  do {
    wire->beginTransmission((uint8_t)addr);              // start transmission to device
    wire->write(reg);                                    // sends register address to write to
    uint8_t bytes = size;
    while (bytes--) {
      wire->write((val >> (8 * bytes)) & 0xFF);          // write data
    }
    x--;
  } while (wire->endTransmission(true) != 0 && x != 0);  // end transmission
  return (x);
}

bool mI2C::I2cWrite8(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 1);
}

bool mI2C::I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val)
{
   return I2cWrite(addr, reg, val, 2);
}

int8_t mI2C::I2cReadBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len)
{
  wire->beginTransmission((uint8_t)addr);
  wire->write((uint8_t)reg);
  wire->endTransmission();
  if (len != wire->requestFrom((uint8_t)addr, (uint8_t)len)) {
    return 1;
  }
  while (len--) {
    *reg_data = (uint8_t)wire->read();
    reg_data++;
  }
  return 0;
}

int8_t mI2C::I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len)
{
  wire->beginTransmission((uint8_t)addr);
  wire->write((uint8_t)reg);
  while (len--) {
    wire->write(*reg_data);
    reg_data++;
  }
  wire->endTransmission();
  return 0;
}

void mI2C::I2cScan(char *devs, unsigned int devs_len)
{
  // Return error codes defined in twi.h and core_esp8266_si2c.c
  // I2C_OK                      0
  // I2C_SCL_HELD_LOW            1 = SCL held low by another device, no procedure available to recover
  // I2C_SCL_HELD_LOW_AFTER_READ 2 = I2C bus error. SCL held low beyond slave clock stretch time
  // I2C_SDA_HELD_LOW            3 = I2C bus error. SDA line held low by slave/another_master after n bits
  // I2C_SDA_HELD_LOW_AFTER_INIT 4 = line busy. SDA again held low by another device. 2nd master?


// #ifdef ESP8266

// DEBUG_LINE_HERE;
// delay(5000);

  uint8_t error = 0;
  uint8_t address = 0;
  uint8_t any = 0;

  snprintf_P(devs, devs_len, PSTR("{\"" D_I2CSCAN "\":\"" D_I2CSCAN_DEVICES_FOUND_AT));

  for (address = 1; address <= 127; address++) {
    wire->beginTransmission(address);
    error = wire->endTransmission();
    if (0 == error) {
      any = 1;
      snprintf_P(devs, devs_len, PSTR("%s 0x%02x"), devs, address);
    }
    else if (error != 2) {  // Seems to happen anyway using this scan
      any = 2;
      snprintf_P(devs, devs_len, PSTR("{\"" D_I2CSCAN "\":\"Error %d at 0x%02x"), error, address);
      break;
    }
  }
  if (any) {
    strncat(devs, "\"}", devs_len - strlen(devs) -1);
  }
  else {
    snprintf_P(devs, devs_len, PSTR("{\"" D_I2CSCAN "\":\"" D_I2CSCAN_NO_DEVICES_FOUND "\"}"));
  }

// #else

// snprintf_P(devs, devs_len, PSTR("{\"" D_I2CSCAN "\":\"not on esp32 yet\"}" ));
  

// #endif

}

//old function that seems to be changed
bool mI2C::I2cDevice(uint8_t addr) // This checks ALL, not just the desired address so is slow
{

  if(!wire){ return false; } // Not started
  
  ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "I2cDevice(%x)=search"),addr);

  for (uint8_t address = 1; address <= 127; address++) {
      // ALOG_TST(PSTR("I2cDevice(%x|%x)=for"),address,addr);
    wire->beginTransmission(address);
    if (!wire->endTransmission() && (address == addr)) 
    {
      ALOG_INF( PSTR("I2cDevice(%x)=true found"),addr);
      return true;
    }else
    if (!wire->endTransmission() && (address != addr))
    {
      ALOG_INF( PSTR("I2cDevice(%x) also found %x"),address,addr);
    }
  }
  
  ALOG_ERR(PSTR("I2cDevice(%x)=FALSE no response"),addr);
  
  return false;
}

bool mI2C::I2cDevice_IsConnected(uint8_t addr) // This checks ALL, not just the desired address so is slow
{

  ALOG_DBM( PSTR(DEBUG_INSERT_PAGE_BREAK "I2cDevice(%x)=starting"),addr);

  uint8_t address = addr;
  ALOG_DBM( PSTR("I2cDevice(%x)=for"),addr);
  wire->beginTransmission(address);
  if (!wire->endTransmission() && (address == addr)) 
  {
    ALOG_DBM( PSTR("I2cDevice(%x)=true"),addr);
    return true;
  }else
  if (!wire->endTransmission() && (address != addr))
  {
    ALOG_DBM(PSTR("I2cDevice(%x) also found %x"),address,addr);
  }
  ALOG_ERR( PSTR("I2cDevice(%x)=FALSE"),addr);
  
  return false;
}


void mI2C::I2cResetActive(uint32_t addr, uint32_t count)
{
  addr &= 0x7F;         // Max I2C address is 127
  count &= 0x7F;        // Max 4 x 32 bits available
  while (count-- && (addr < 128)) {
    i2c_active[addr / 32] &= ~(1 << (addr % 32));
    addr++;
  }
    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_DBG(PSTR("I2C: Active %08X,%08X,%08X,%08X"), i2c_active[0], i2c_active[1], i2c_active[2], i2c_active[3]);
    #endif // ENABLE_LOG_LEVEL_INFO
}

void mI2C::I2cSetActive(uint32_t addr, uint32_t count)
{
  addr &= 0x7F;         // Max I2C address is 127
  count &= 0x7F;        // Max 4 x 32 bits available
  while (count-- && (addr < 128)) {
    i2c_active[addr / 32] |= (1 << (addr % 32));
    addr++;
  }
  ALOG_DBG( PSTR("I2C: Active %08X,%08X,%08X,%08X"), i2c_active[0], i2c_active[1], i2c_active[2], i2c_active[3] );
}

void mI2C::I2cSetActiveFound(uint32_t addr, const char *types)
{
  I2cSetActive(addr);
  #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_INF(S_LOG_I2C_FOUND_AT, types, addr);
  #endif // ENABLE_LOG_LEVEL_INFO
}
void mI2C::I2cSetActiveFound_P(uint32_t addr, const char *types)
{
  char buffer[20];
  snprintf_P(buffer, sizeof(buffer), types);
  I2cSetActiveFound(addr, buffer);
}

bool mI2C::I2cActive(uint32_t addr)
{
  addr &= 0x7F;         // Max I2C address is 127
  if (i2c_active[addr / 32] & (1 << (addr % 32))) {
    return true;
  }
  return false;
}

bool mI2C::I2cSetDevice(uint32_t addr)
{

  ALOG_INF(PSTR("I2cSetDevice(%02X)"),addr);

  addr &= 0x7F;         // Max I2C address is 127
  if (I2cActive(addr)) {
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_INF(PSTR("I2cSetDevice already active"));
    // #endif // ENABLE_LOG_LEVEL_INFO
    return false;       // If already active report as not present;
  }

  // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  ALOG_DBM( PSTR("I2cSetDevice beginning"));
  // #endif //  ENABLE_LOG_LEVEL_INFO

  wire->beginTransmission((uint8_t)addr);

  bool result = (0 == wire->endTransmission());

  ALOG_INF(PSTR("result=%d 0 is successful"),result);
  return result;

}

bool mI2C::I2cEnabled(uint32_t i2c_index)
{

// Serial.println("I2cEnabled"); Serial.flush();
// delay(3000);
bool val = (tkr_set->runtime.i2c_enabled && bitRead(tkr_set->Settings.i2c_drivers[i2c_index / 32], i2c_index % 32));


return true; // force all working for now until i2cdrivers are configured

// Serial.println(val); Serial.flush();
  return val;

// Serial.println("val"); Serial.flush();
// Serial.println(val); Serial.flush();
// delay(3000);

  // return val;
}


#endif // USE_MODULE_CORE_I2C
