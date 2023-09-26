#include "mDB18x20.h"

#ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023

const char* mDS18X::PM_MODULE_SENSORS_DB18_CTR = D_MODULE_SENSORS_DB18S20_CTR;
const char* mDS18X::PM_MODULE_SENSORS_DB18_FRIENDLY_CTR = D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR;

// Should this be a controller? Since the underlying input is from switches sensor. This would also mean motion should be controller.
// But then how does motion + others become an "event"
// Event should be anything really, not just switch ie power level threshold crossed


int8_t mDoorSensor::Tasker(uint8_t function, JsonParserObject obj)
{
  
  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:

      // if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
      // {
      //   AddLog(LOG_LEVEL_TEST, PSTR("DoorLockPin=%d"), digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
      // }

    break;
    case FUNC_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER23
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
      break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
      break;
    #endif //USE_MODULE_NETWORK_WEBSERVER23
  }

} // END function



void mDoorSensor::Pre_Init(void){
  
  settings.fEnableSensor = false;

  if(pCONT_pins->PinUsed(GPIO_DOOR_OPEN_ID)) {  // not set when 255
    // pin_open = pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID);
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID), INPUT_PULLUP);
    settings.fEnableSensor = true;
    settings.fSensorCount = 1;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID));
    //disable pir code
  }


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    pinMode(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID), INPUT_PULLUP);
    settings.fEnableSensor = true;
    settings.fSensorCount = 1;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID));
    //disable pir code
  }

}

/**
 * @brief "LOW" is closed, HIGH is open
 * 
 * @return uint8_t 
 */
uint8_t mDoorSensor::IsDoorOpen(){
  return (digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID))==HIGH);
}
uint8_t mDoorSensor::IsLock_Locked(){
  return (digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID))==LOW);
}


void mDoorSensor::init(void){

  door_detect.state = IsDoorOpen();
  lock_detect.state = IsLock_Locked();

}



void mDoorSensor::EveryLoop(){

  /**
   * @brief Reed Switch Door Position
   **/
  if((IsDoorOpen()!=door_detect.state)&&mTime::TimeReachedNonReset(&door_detect.tDetectTimeforDebounce,100)){

    AddLog(LOG_LEVEL_TEST, PSTR("IsDoorOpen()"));

    door_detect.state = IsDoorOpen();
    door_detect.tDetectTimeforDebounce = millis();
    if(door_detect.state){ 
      door_detect.isactive = true;
      door_detect.detected_time = pCONT_time->GetTimeShortNow();
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_STARTED_ID, 0, door_detect.isactive);
    }else{ 
      door_detect.isactive = false;
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_ENDED_ID, 0, door_detect.isactive);
    }
    door_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

  }

  /**
   * @brief Door lock  
   **/
  if((IsLock_Locked()!=lock_detect.state)&&mTime::TimeReachedNonReset(&lock_detect.tDetectTimeforDebounce,100)){

    AddLog(LOG_LEVEL_TEST, PSTR("IsLock_Locked()"));

    lock_detect.state = IsLock_Locked();
    lock_detect.tDetectTimeforDebounce = millis();
    if(lock_detect.state){ 
      lock_detect.isactive = true;
      lock_detect.detected_time = pCONT_time->GetTimeShortNow();
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_STARTED_ID, 1, lock_detect.isactive);
    }else{ 
      lock_detect.isactive = false;
      pCONT_rules->NewEventRun( GetModuleUniqueID(), FUNC_EVENT_MOTION_ENDED_ID, 1, lock_detect.isactive);
    }
    lock_detect.ischanged = true;
    mqtthandler_sensor_ifchanged.flags.SendNow = true;
    mqtthandler_sensor_teleperiod.flags.SendNow = true;

  }



}


const char* mDoorSensor::IsDoorOpen_Ctr(char* buffer, uint8_t buflen){
  if(door_detect.isactive){
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_OPENED_CTR, sizeof(PM_EVENT_DOOR_OPENED_CTR));
  }else{
    snprintf_P(buffer, buflen, PM_EVENT_DOOR_CLOSED_CTR, sizeof(PM_EVENT_DOOR_CLOSED_CTR));
  }
  return buffer;
}


void mDoorSensor::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_BME "\"%s\""),JBI->GetBufferPtr());

}




uint8_t mDoorSensor::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    //JsonBuilderI->Add_P(PM_JSON_SENSORCOUNT, settings.);
  return JsonBuilderI->End();

}


uint8_t mDoorSensor::ConstructJSON_Sensor(uint8_t json_level){
  
  char buffer[50];

  JsonBuilderI->Start();
  JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),0,buffer,sizeof(buffer)));
  JsonBuilderI->Add("Position", IsDoorOpen_Ctr(buffer, sizeof(buffer))); // give telemetry update of position
  
  if(json_level >= JSON_LEVEL_IFCHANGED){
    JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTimetoCtr(&door_detect.detected_time, buffer, sizeof(buffer)));
    JsonBuilderI->Add(D_JSON_EVENT, IsDoorOpen_Ctr(buffer, sizeof(buffer)));
  }

  JBI->Add("DoorOpenPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_OPEN_ID)));

    
  JBI->Add("IsDoorOpen", IsDoorOpen());


  if(pCONT_pins->PinUsed(GPIO_DOOR_LOCK_ID)) // phase out in favour of basic switch? if so, doorsensor can become similar to motion that is non-resetting
  {
    JBI->Add("DoorLockPin", digitalRead(pCONT_pins->GetPin(GPIO_DOOR_LOCK_ID)));
    JBI->Add("IsLock_Locked", IsLock_Locked());
  }

  return JsonBuilderI->End();

}



/*
  xsns_05_ds18x20.ino - DS18x20 temperature sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef ESP8266
#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature - Multiple sensors
\*********************************************************************************************/

#define XSNS_05              5

//#define USE_DS18x20_RECONFIGURE    // When sensor is lost keep retrying or re-configure
//#define DS18x20_USE_ID_AS_NAME      // Use last 3 bytes for naming of sensors

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_WRITE_EEPROM      0x48
#define W1_WRITE_SCRATCHPAD  0x4E
#define W1_READ_SCRATCHPAD   0xBE

#ifndef DS18X20_MAX_SENSORS // DS18X20_MAX_SENSORS fallback to 8 if not defined in user_config_override.h
#define DS18X20_MAX_SENSORS  8
#endif

const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";

uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };

struct {
  float temperature;
  float temp_sum;
  uint16_t numread;
  uint8_t address[8];
  uint8_t index;
  uint8_t valid;
} ds18x20_sensor[DS18X20_MAX_SENSORS];

struct {
#ifdef W1_PARASITE_POWER
  uint32_t w1_power_until = 0;
  uint8_t current_sensor = 0;
#endif
  char name[17];
  uint8_t sensors = 0;
  uint8_t input_mode = 0;    // INPUT or INPUT_PULLUP (=2)
  int8_t pin = 0;            // Shelly GPIO3 input only
  int8_t pin_out = 0;        // Shelly GPIO00 output only
  bool dual_mode = false;    // Single pin mode
} DS18X20Data;

/*********************************************************************************************\
 * Embedded tuned OneWire library
\*********************************************************************************************/

#define W1_MATCH_ROM         0x55
#define W1_SEARCH_ROM        0xF0

uint8_t onewire_last_discrepancy = 0;
uint8_t onewire_last_family_discrepancy = 0;
bool onewire_last_device_flag = false;
unsigned char onewire_rom_id[8] = { 0 };

/*------------------------------------------------------------------------------------------*/

uint8_t OneWireReset(void) {
  uint8_t retries = 125;

  if (!DS18X20Data.dual_mode) {
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
    do {
      if (--retries == 0) {
        return 0;
      }
      delayMicroseconds(2);
    } while (!digitalRead(DS18X20Data.pin));
    pinMode(DS18X20Data.pin, OUTPUT);
    digitalWrite(DS18X20Data.pin, LOW);
    delayMicroseconds(480);
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
    delayMicroseconds(70);
    uint8_t r = !digitalRead(DS18X20Data.pin);
    delayMicroseconds(410);
    return r;
  } else {
    digitalWrite(DS18X20Data.pin_out, HIGH);
    do {
      if (--retries == 0) {
        return 0;
      }
      delayMicroseconds(2);
    } while (!digitalRead(DS18X20Data.pin));
    digitalWrite(DS18X20Data.pin_out, LOW);
    delayMicroseconds(480);
    digitalWrite(DS18X20Data.pin_out, HIGH);
    delayMicroseconds(70);
    uint8_t r = !digitalRead(DS18X20Data.pin);
    delayMicroseconds(410);
    return r;
  }
}

void OneWireWriteBit(uint8_t v) {
  static const uint8_t delay_low[2] = { 65, 10 };
  static const uint8_t delay_high[2] = { 5, 55 };

  v &= 1;
  if (!DS18X20Data.dual_mode) {
    digitalWrite(DS18X20Data.pin, LOW);
    pinMode(DS18X20Data.pin, OUTPUT);
    delayMicroseconds(delay_low[v]);
    digitalWrite(DS18X20Data.pin, HIGH);
  } else {
    digitalWrite(DS18X20Data.pin_out, LOW);
    delayMicroseconds(delay_low[v]);
    digitalWrite(DS18X20Data.pin_out, HIGH);
  }
  delayMicroseconds(delay_high[v]);
}

uint8_t OneWire1ReadBit(void) {
  pinMode(DS18X20Data.pin, OUTPUT);
  digitalWrite(DS18X20Data.pin, LOW);
  delayMicroseconds(3);
  pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
  delayMicroseconds(10);
  uint8_t r = digitalRead(DS18X20Data.pin);
  delayMicroseconds(53);
  return r;
}

uint8_t OneWire2ReadBit(void) {
  digitalWrite(DS18X20Data.pin_out, LOW);
  delayMicroseconds(3);
  digitalWrite(DS18X20Data.pin_out, HIGH);
  delayMicroseconds(10);
  uint8_t r = digitalRead(DS18X20Data.pin);
  delayMicroseconds(53);
  return r;
}

/*------------------------------------------------------------------------------------------*/

void OneWireWrite(uint8_t v) {
  for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
    OneWireWriteBit((bit_mask & v) ? 1 : 0);
  }
}

uint8_t OneWireRead(void) {
  uint8_t r = 0;

  if (!DS18X20Data.dual_mode) {
    for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
      if (OneWire1ReadBit()) {
        r |= bit_mask;
      }
    }
  } else {
    for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
      if (OneWire2ReadBit()) {
        r |= bit_mask;
      }
    }
  }
  return r;
}

void OneWireSelect(const uint8_t rom[8]) {
  OneWireWrite(W1_MATCH_ROM);
  for (uint32_t i = 0; i < 8; i++) {
    OneWireWrite(rom[i]);
  }
}

uint8_t OneWireSearch(uint8_t *newAddr) {
  uint8_t id_bit_number = 1;
  uint8_t last_zero = 0;
  uint8_t rom_byte_number = 0;
  uint8_t search_result = 0;
  uint8_t id_bit;
  uint8_t cmp_id_bit;
  unsigned char rom_byte_mask = 1;
  unsigned char search_direction;

  if (!onewire_last_device_flag) {
    if (!OneWireReset()) {
      onewire_last_discrepancy = 0;
      onewire_last_device_flag = false;
      onewire_last_family_discrepancy = 0;
      return false;
    }
    OneWireWrite(W1_SEARCH_ROM);
    do {
      if (!DS18X20Data.dual_mode) {
        id_bit     = OneWire1ReadBit();
        cmp_id_bit = OneWire1ReadBit();
      } else {
        id_bit     = OneWire2ReadBit();
        cmp_id_bit = OneWire2ReadBit();
      }
      if ((id_bit == 1) && (cmp_id_bit == 1)) {
        break;
      } else {
        if (id_bit != cmp_id_bit) {
          search_direction = id_bit;
        } else {
          if (id_bit_number < onewire_last_discrepancy) {
            search_direction = ((onewire_rom_id[rom_byte_number] & rom_byte_mask) > 0);
          } else {
            search_direction = (id_bit_number == onewire_last_discrepancy);
          }
          if (search_direction == 0) {
            last_zero = id_bit_number;
            if (last_zero < 9) {
              onewire_last_family_discrepancy = last_zero;
            }
          }
        }
        if (search_direction == 1) {
          onewire_rom_id[rom_byte_number] |= rom_byte_mask;
        } else {
          onewire_rom_id[rom_byte_number] &= ~rom_byte_mask;
        }
        OneWireWriteBit(search_direction);
        id_bit_number++;
        rom_byte_mask <<= 1;
        if (rom_byte_mask == 0) {
          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < 8);
    if (!(id_bit_number < 65)) {
      onewire_last_discrepancy = last_zero;
      if (onewire_last_discrepancy == 0) {
        onewire_last_device_flag = true;
      }
      search_result = true;
    }
  }
  if (!search_result || !onewire_rom_id[0]) {
    onewire_last_discrepancy = 0;
    onewire_last_device_flag = false;
    onewire_last_family_discrepancy = 0;
    search_result = false;
  }
  for (uint32_t i = 0; i < 8; i++) {
    newAddr[i] = onewire_rom_id[i];
  }
  return search_result;
}

bool OneWireCrc8(uint8_t *addr) {
  uint8_t crc = 0;
  uint8_t len = 8;

  while (len--) {
    uint8_t inbyte = *addr++;          // from 0 to 7
    for (uint32_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) {
        crc ^= 0x8C;
      }
      inbyte >>= 1;
    }
  }
  return (crc == *addr);               // addr 8
}

/********************************************************************************************/

void Ds18x20Init(void) {
  DS18X20Data.pin = Pin(GPIO_DSB);
  DS18X20Data.input_mode = Settings->flag3.ds18x20_internal_pullup ? INPUT_PULLUP : INPUT;  // SetOption74 - Enable internal pullup for single DS18x20 sensor

  if (PinUsed(GPIO_DSB_OUT)) {
    DS18X20Data.pin_out = Pin(GPIO_DSB_OUT);
    DS18X20Data.dual_mode = true;    // Dual pins mode as used by Shelly
    pinMode(DS18X20Data.pin_out, OUTPUT);
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
  }

  onewire_last_discrepancy = 0;
  onewire_last_device_flag = false;
  onewire_last_family_discrepancy = 0;
  for (uint32_t i = 0; i < 8; i++) {
    onewire_rom_id[i] = 0;
  }

  uint64_t ids[DS18X20_MAX_SENSORS];
  DS18X20Data.sensors = 0;
  while (DS18X20Data.sensors < DS18X20_MAX_SENSORS) {
    if (!OneWireSearch(ds18x20_sensor[DS18X20Data.sensors].address)) {
      break;
    }
    if (OneWireCrc8(ds18x20_sensor[DS18X20Data.sensors].address) &&
       ((ds18x20_sensor[DS18X20Data.sensors].address[0] == DS18S20_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS1822_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS18B20_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == MAX31850_CHIPID))) {
      ds18x20_sensor[DS18X20Data.sensors].index = DS18X20Data.sensors;
      ids[DS18X20Data.sensors] = ds18x20_sensor[DS18X20Data.sensors].address[0];  // Chip id
      for (uint32_t j = 6; j > 0; j--) {
        ids[DS18X20Data.sensors] = ids[DS18X20Data.sensors] << 8 | ds18x20_sensor[DS18X20Data.sensors].address[j];
      }
      DS18X20Data.sensors++;
    }
  }
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    for (uint32_t j = i + 1; j < DS18X20Data.sensors; j++) {
      if (ids[ds18x20_sensor[i].index] > ids[ds18x20_sensor[j].index]) {  // Sort ascending
        std::swap(ds18x20_sensor[i].index, ds18x20_sensor[j].index);
      }
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), DS18X20Data.sensors);
}

void Ds18x20Convert(void) {
  OneWireReset();
#ifdef W1_PARASITE_POWER
  // With parasite power address one sensor at a time
  if (++DS18X20Data.current_sensor >= DS18X20Data.sensors)
    DS18X20Data.current_sensor = 0;
  OneWireSelect(ds18x20_sensor[DS18X20Data.current_sensor].address);
#else
  OneWireWrite(W1_SKIP_ROM);           // Address all Sensors on Bus
#endif
  OneWireWrite(W1_CONVERT_TEMP);       // start conversion, no parasite power on at the end
//  delay(750);                          // 750ms should be enough for 12bit conv
}

bool Ds18x20Read(uint8_t sensor) {
  float temperature;
  uint8_t data[9];
  int8_t sign = 1;

  uint8_t index = ds18x20_sensor[sensor].index;
  if (ds18x20_sensor[index].valid) { ds18x20_sensor[index].valid--; }
  for (uint32_t retry = 0; retry < 3; retry++) {
    OneWireReset();
    OneWireSelect(ds18x20_sensor[index].address);
    OneWireWrite(W1_READ_SCRATCHPAD);
    for (uint32_t i = 0; i < 9; i++) {
      data[i] = OneWireRead();
    }
    if (OneWireCrc8(data)) {
      switch(ds18x20_sensor[index].address[0]) {
        case DS18S20_CHIPID: {
          int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
          temperature = ConvertTemp(tempS * 0.0625f - 0.250f);
          break;
        }
        case DS1822_CHIPID:
        case DS18B20_CHIPID: {
          if (data[4] != 0x7F) {
            data[4] = 0x7F;                 // Set resolution to 12-bit
            OneWireReset();
            OneWireSelect(ds18x20_sensor[index].address);
            OneWireWrite(W1_WRITE_SCRATCHPAD);
            OneWireWrite(data[2]);          // Th Register
            OneWireWrite(data[3]);          // Tl Register
            OneWireWrite(data[4]);          // Configuration Register
            OneWireSelect(ds18x20_sensor[index].address);
            OneWireWrite(W1_WRITE_EEPROM);  // Save scratchpad to EEPROM
#ifdef W1_PARASITE_POWER
            DS18X20Data.w1_power_until = millis() + 10; // 10ms specified duration for EEPROM write
#endif
          }
          uint16_t temp12 = (data[1] << 8) + data[0];
          if (temp12 > 2047) {
            temp12 = (~temp12) +1;
            sign = -1;
          }
          temperature = ConvertTemp(sign * temp12 * 0.0625f);  // Divide by 16
          break;
        }
        case MAX31850_CHIPID: {
          int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
          temperature = ConvertTemp(temp14 * 0.0625f);  // Divide by 16
          break;
        }
      }
      ds18x20_sensor[index].temperature = temperature;
      if (Settings->flag5.ds18x20_mean) {
        if (ds18x20_sensor[index].numread++ == 0) {
          ds18x20_sensor[index].temp_sum = 0;
        }
        ds18x20_sensor[index].temp_sum += temperature;
      }
      ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
      return true;
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void Ds18x20Name(uint8_t sensor) {
  uint8_t index = sizeof(ds18x20_chipids);
  while (--index) {
    if (ds18x20_sensor[ds18x20_sensor[sensor].index].address[0] == ds18x20_chipids[index]) {
      break;
    }
  }
  GetTextIndexed(DS18X20Data.name, sizeof(DS18X20Data.name), index, kDs18x20Types);
  if (DS18X20Data.sensors > 1) {
#ifdef DS18x20_USE_ID_AS_NAME
    char address[17];
    for (uint32_t j = 0; j < 3; j++) {
      sprintf(address+2*j, "%02X", ds18x20_sensor[ds18x20_sensor[sensor].index].address[3-j]);  // Only last 3 bytes
    }
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%s"), DS18X20Data.name, IndexSeparator(), address);
#else
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%d"), DS18X20Data.name, IndexSeparator(), sensor +1);
#endif
  }
}

/********************************************************************************************/

void EverySecond(void) {
  if (!DS18X20Data.sensors) { return; }

#ifdef W1_PARASITE_POWER
  // skip access if there is still an eeprom write ongoing
  unsigned long now = millis();
  if (now < DS18X20Data.w1_power_until) { return; }
#endif
  if (TasmotaGlobal.uptime & 1
#ifdef W1_PARASITE_POWER
      // if more than 1 sensor and only parasite power: convert every cycle
      || DS18X20Data.sensors >= 2
#endif
  ) {
    // 2mS
    Ds18x20Convert();          // Start conversion, takes up to one second
  } else {
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
      // 12mS per device
      if (!Ds18x20Read(i)) {   // Read temperature
        Ds18x20Name(i);
        AddLogMissed(DS18X20Data.name, ds18x20_sensor[ds18x20_sensor[i].index].valid);
#ifdef USE_DS18x20_RECONFIGURE
        if (!ds18x20_sensor[ds18x20_sensor[i].index].valid) {
          memset(&ds18x20_sensor, 0, sizeof(ds18x20_sensor));
          Ds18x20Init();       // Re-configure
        }
#endif  // USE_DS18x20_RECONFIGURE
      }
    }
  }
}

void Ds18x20Show(bool json) {
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    uint8_t index = ds18x20_sensor[i].index;

    if (ds18x20_sensor[index].valid) {   // Check for valid temperature
      Ds18x20Name(i);

      if (json) {
        if (Settings->flag5.ds18x20_mean) {
          if ((0 == TasmotaGlobal.tele_period) && ds18x20_sensor[index].numread) {
            ds18x20_sensor[index].temperature = ds18x20_sensor[index].temp_sum / ds18x20_sensor[index].numread;
            ds18x20_sensor[index].numread = 0;
          }
        }
        char address[17];
        for (uint32_t j = 0; j < 6; j++) {
          sprintf(address+2*j, "%02X", ds18x20_sensor[index].address[6-j]);  // Skip sensor type and crc
        }
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f}"),
          DS18X20Data.name, address, Settings->flag2.temperature_resolution, &ds18x20_sensor[index].temperature);
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
          DomoticzFloatSensor(DZ_TEMP, ds18x20_sensor[index].temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
          KnxSensor(KNX_TEMPERATURE, ds18x20_sensor[index].temperature);
        }
#endif  // USE_KNX
#ifdef USE_WEBSERVER
      } else {
        WSContentSend_Temp(DS18X20Data.name, ds18x20_sensor[index].temperature);
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns05(uint8_t function) {
  bool result = false;

  if (PinUsed(GPIO_DSB)) {
    switch (function) {
      case FUNC_INIT:
        Ds18x20Init();
        break;
      case FUNC_EVERY_SECOND:
        Ds18x20EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ds18x20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20
#endif  // ESP8266





#endif


