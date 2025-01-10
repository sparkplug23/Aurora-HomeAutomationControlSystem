/**
 * @file mBME.cpp
 * @author Michael Doone (michaeldoonehub@gmail.com)
 * @brief BME280 Temperature, Humidity and Pressure sensor 
 * @version 1.0
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "mBME.h"

#ifdef USE_MODULE_SENSORS_BME


int8_t mBME::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      BmpRead();
    break;
    case TASK_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
      break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}


void mBME::Pre_Init()
{

  module_state.mode = ModuleStatus::Initialising;
  bmp_count = 0;
  
  uint8_t bmp_addresses[] = { BMP_ADDR1, BMP_ADDR2 };
  
  int bmp_sensor_size = BMP_MAX_SENSORS * sizeof(bmp_sensors_t);
  if (!bmp_sensors) {
    bmp_sensors = (bmp_sensors_t*)malloc(bmp_sensor_size);
  }
  if (!bmp_sensors) { return; }
  memset(bmp_sensors, 0, bmp_sensor_size);  // Init defaults to 0


  for (uint8_t i = 0; i < BMP_MAX_SENSORS; i++) 
  {
    
    if (pCONT_i2c->I2cActive(bmp_addresses[i])) 
    { 
      ALOG_INF(PSTR(D_LOG_BME "Addr 0x%x not found"), bmp_addresses[i]); 
      continue; 
    }
    
    uint8_t bmp_type = pCONT_i2c->I2cRead8(bmp_addresses[i], BMP_REGISTER_CHIPID);
    if (bmp_type) {
      bmp_sensors[bmp_count].i2c_address = bmp_addresses[i];
      bmp_sensors[bmp_count].bmp_type = bmp_type;
      bmp_sensors[bmp_count].bmp_model = 0;

      ALOG_INF(PSTR("i2c_address %d"), bmp_sensors[bmp_count].i2c_address);
      ALOG_INF(PSTR("bmp_type %d"), bmp_sensors[bmp_count].bmp_type);

      bool success = false;
      switch (bmp_type) {
        case BMP180_CHIPID:
          success = Bmp180Calibration(bmp_count);
          break;
        case BME280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 2  
          // No break intentional
        case BMP280_CHIPID:
          bmp_sensors[bmp_count].bmp_model++;  // 1
          success = Bmx280Calibrate(bmp_count);
          break;
        case BME680_CHIPID:
          bmp_sensors[bmp_count].bmp_model = 3;  // 3
          success = Bme680Init(bmp_count);
          break;
      }
      if (success) {
        pCONT_sup->GetTextIndexed_P(bmp_sensors[bmp_count].bmp_name, sizeof(bmp_sensors[bmp_count].bmp_name), bmp_sensors[bmp_count].bmp_model, kBmpTypes);
        pCONT_i2c->I2cSetActiveFound(bmp_sensors[bmp_count].i2c_address, bmp_sensors[bmp_count].bmp_name);
        bmp_count++;
      }
    }else{
          
      #ifdef ESP32
      AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("getErrorText =\"%s\""), pCONT_i2c->wire->getErrorText(pCONT_i2c->wire->lastError()));
      #endif 

    }
  }


  if(bmp_count){
    module_state.mode = ModuleStatus::Running;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_BME "Running"));
  }

}


void mBME::Init(void)
{
  
}

void mBME::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(bmp_count)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", bmp_count);  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++)
    {      
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%s:\"%s\", ", bmp_sensors[sensor_id].bmp_name, DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer2, sizeof(buffer2)));    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_BME "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


void mBME::BmpRead(void)
{
  for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) 
  {
    switch (bmp_sensors[bmp_idx].bmp_type) 
    {
      case BMP180_CHIPID:
        Bmp180Read(bmp_idx);
        break;
      case BMP280_CHIPID:
      case BME280_CHIPID:
        Bme280Read(bmp_idx);
        break;
      case BME680_CHIPID:
        Bme680Read(bmp_idx);
        break;
    }
    bmp_sensors[bmp_idx].ischangedtLast = millis();
    bmp_sensors[bmp_idx].utc_measured_timestamp = tkr_time->UtcTime();
  }
}


void mBME::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_BME "\"%s\""),JBI->GetBufferPtr());

}



/******************************************************************************************************************
 * @SECTION: Commands
*******************************************************************************************************************/

  

/******************************************************************************************************************
 * @SECTION: BMP180
*******************************************************************************************************************/


bool mBME::Bmp180Calibration(uint8_t bmp_idx)
{
  if (!bmp180_cal_data) {
    bmp180_cal_data = (bmp180_cal_data_t*)malloc(BMP_MAX_SENSORS * sizeof(bmp180_cal_data_t));
  }
  if (!bmp180_cal_data) { return false; }

  bmp180_cal_data[bmp_idx].cal_ac1 = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_AC1);
  bmp180_cal_data[bmp_idx].cal_ac2 = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_AC2);
  bmp180_cal_data[bmp_idx].cal_ac3 = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_AC3);
  bmp180_cal_data[bmp_idx].cal_ac4 = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_AC4);
  bmp180_cal_data[bmp_idx].cal_ac5 = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_AC5);
  bmp180_cal_data[bmp_idx].cal_ac6 = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_AC6);
  bmp180_cal_data[bmp_idx].cal_b1  = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_VB1);
  bmp180_cal_data[bmp_idx].cal_b2  = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_VB2);
  bmp180_cal_data[bmp_idx].cal_mc  = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_MC);
  bmp180_cal_data[bmp_idx].cal_md  = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_MD);

  // Check for Errors in calibration data. Value never is 0x0000 or 0xFFFF
  if (!bmp180_cal_data[bmp_idx].cal_ac1 |
      !bmp180_cal_data[bmp_idx].cal_ac2 |
      !bmp180_cal_data[bmp_idx].cal_ac3 |
      !bmp180_cal_data[bmp_idx].cal_ac4 |
      !bmp180_cal_data[bmp_idx].cal_ac5 |
      !bmp180_cal_data[bmp_idx].cal_ac6 |
      !bmp180_cal_data[bmp_idx].cal_b1 |
      !bmp180_cal_data[bmp_idx].cal_b2 |
      !bmp180_cal_data[bmp_idx].cal_mc |
      !bmp180_cal_data[bmp_idx].cal_md
  ){
    return false;
  }

  if ((bmp180_cal_data[bmp_idx].cal_ac1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac3 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac4 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac5 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_ac6 == 0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b1 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_b2 == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_mc == (int16_t)0xFFFF) |
      (bmp180_cal_data[bmp_idx].cal_md == (int16_t)0xFFFF)
  ){
    return false;
  }
  return true;
}

void mBME::Bmp180Read(uint8_t bmp_idx)
{
  if (!bmp180_cal_data)
  { 
    return; 
  }

  pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BMP180_REG_CONTROL, BMP180_TEMPERATURE);
  delay(5); // 5ms conversion time
  int ut = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BMP180_REG_RESULT);
  int32_t xt1 = (ut - (int32_t)bmp180_cal_data[bmp_idx].cal_ac6) * ((int32_t)bmp180_cal_data[bmp_idx].cal_ac5) >> 15;
  int32_t xt2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_mc << 11) / (xt1 + (int32_t)bmp180_cal_data[bmp_idx].cal_md);
  int32_t bmp180_b5 = xt1 + xt2;
  bmp_sensors[bmp_idx].temperature = ((bmp180_b5 + 8) >> 4) / 10.0f;

  pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BMP180_REG_CONTROL, BMP180_PRESSURE3); // Highest resolution
  delay(2 + (4 << BMP180_OSS));                                 // 26ms conversion time at ultra high resolution
  uint32_t up = pCONT_i2c->I2cRead24(bmp_sensors[bmp_idx].i2c_address, BMP180_REG_RESULT);
  up >>= (8 - BMP180_OSS);

  int32_t b6 = bmp180_b5 - 4000;
  int32_t x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b2 * ((b6 * b6) >> 12)) >> 11;
  int32_t x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac2 * b6) >> 11;
  int32_t x3 = x1 + x2;
  int32_t b3 = ((((int32_t)bmp180_cal_data[bmp_idx].cal_ac1 * 4 + x3) << BMP180_OSS) + 2) >> 2;

  x1 = ((int32_t)bmp180_cal_data[bmp_idx].cal_ac3 * b6) >> 13;
  x2 = ((int32_t)bmp180_cal_data[bmp_idx].cal_b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  uint32_t b4 = ((uint32_t)bmp180_cal_data[bmp_idx].cal_ac4 * (uint32_t)(x3 + 32768)) >> 15;
  uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> BMP180_OSS);

  int32_t p;
  if (b7 < 0x80000000) {
    p = (b7 * 2) / b4;
  }
  else {
    p = (b7 / b4) * 2;
  }
  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  p += ((x1 + x2 + (int32_t)3791) >> 4);
  bmp_sensors[bmp_idx].pressure = (float)p / 100.0f;  // convert to mbar
}


/******************************************************************************************************************
 * @SECTION: BMP280
*******************************************************************************************************************/


bool mBME::Bmx280Calibrate(uint8_t bmp_idx)
{

  if (!Bme280CalibrationData) 
  {
    Bme280CalibrationData = (Bme280CalibrationData_t*)malloc(BMP_MAX_SENSORS * sizeof(Bme280CalibrationData_t));
  }
  if (!Bme280CalibrationData)
  { 
    return false; 
  }

  Bme280CalibrationData[bmp_idx].dig_T1 = pCONT_i2c->I2cRead16LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_T1);
  Bme280CalibrationData[bmp_idx].dig_T2 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_T2);
  Bme280CalibrationData[bmp_idx].dig_T3 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_T3);
  Bme280CalibrationData[bmp_idx].dig_P1 = pCONT_i2c->I2cRead16LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P1);
  Bme280CalibrationData[bmp_idx].dig_P2 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P2);
  Bme280CalibrationData[bmp_idx].dig_P3 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P3);
  Bme280CalibrationData[bmp_idx].dig_P4 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P4);
  Bme280CalibrationData[bmp_idx].dig_P5 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P5);
  Bme280CalibrationData[bmp_idx].dig_P6 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P6);
  Bme280CalibrationData[bmp_idx].dig_P7 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P7);
  Bme280CalibrationData[bmp_idx].dig_P8 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P8);
  Bme280CalibrationData[bmp_idx].dig_P9 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_P9);
  if (BME280_CHIPID == bmp_sensors[bmp_idx].bmp_type) 
  {  // #1051
    Bme280CalibrationData[bmp_idx].dig_H1 = pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H1);
    Bme280CalibrationData[bmp_idx].dig_H2 = pCONT_i2c->I2cReadS16_LE(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H2);
    Bme280CalibrationData[bmp_idx].dig_H3 = pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H3);
    Bme280CalibrationData[bmp_idx].dig_H4 = (pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H4) << 4) | (pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H4 + 1) & 0xF);
    Bme280CalibrationData[bmp_idx].dig_H5 = (pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H5 + 1) << 4) | (pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H5) >> 4);
    Bme280CalibrationData[bmp_idx].dig_H6 = (int8_t)pCONT_i2c->I2cRead8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_DIG_H6);
    pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_CONTROL, 0x00);      // sleep mode since writes to config can be ignored in normal mode (Datasheet 5.4.5/6 page 27) // Set before CONTROL_meas (DS 5.4.3)
    pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_CONTROLHUMID, 0x01); // 1x oversampling
    pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_CONFIG, 0xA0);       // 1sec standby between measurements (to limit self heating), IIR filter off
    pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_CONTROL, 0x27);      // 1x oversampling, normal mode
  } 
  else 
  {
    pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_CONTROL, 0xB7);      // 16x oversampling, normal mode (Adafruit)
  }

  return true;
}

void mBME::Bme280Read(uint8_t bmp_idx)
{
  if (!Bme280CalibrationData)
  {
    return; 
  }

  int32_t adc_T = pCONT_i2c->I2cRead24(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  int32_t vart1 = ((((adc_T >> 3) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1 << 1))) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_T2)) >> 11;
  int32_t vart2 = (((((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1)) * ((adc_T >> 4) - ((int32_t)Bme280CalibrationData[bmp_idx].dig_T1))) >> 12) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_T3)) >> 14;
  int32_t t_fine = vart1 + vart2;
  float T = (t_fine * 5 + 128) >> 8;
  bmp_sensors[bmp_idx].temperature = T / 100.0f;

  int32_t adc_P = pCONT_i2c->I2cRead24(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  int64_t var1 = ((int64_t)t_fine) - 128000;
  int64_t var2 = var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P6;
  var2 = var2 + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P5) << 17);
  var2 = var2 + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P3) >> 8) + ((var1 * (int64_t)Bme280CalibrationData[bmp_idx].dig_P2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)Bme280CalibrationData[bmp_idx].dig_P1) >> 33;
  if (0 == var1) {
    return; // avoid exception caused by division by zero
  }
  int64_t p = 1048576 - adc_P;
  p = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)Bme280CalibrationData[bmp_idx].dig_P8) * p) >> 19;
  p = ((p + var1 + var2) >> 8) + (((int64_t)Bme280CalibrationData[bmp_idx].dig_P7) << 4);
  bmp_sensors[bmp_idx].pressure = (float)p / 25600.0f;

  if (BMP280_CHIPID == bmp_sensors[bmp_idx].bmp_type) { return; }

  int32_t adc_H = pCONT_i2c->I2cRead16(bmp_sensors[bmp_idx].i2c_address, BME280_REGISTER_HUMIDDATA);

  int32_t v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r = (((((adc_H << 14) - (((int32_t)Bme280CalibrationData[bmp_idx].dig_H4) << 20) -
    (((int32_t)Bme280CalibrationData[bmp_idx].dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
    (((((((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H6)) >> 10) *
    (((v_x1_u32r * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
    ((int32_t)2097152)) * ((int32_t)Bme280CalibrationData[bmp_idx].dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
    ((int32_t)Bme280CalibrationData[bmp_idx].dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  float h = (v_x1_u32r >> 12);
  bmp_sensors[bmp_idx].humidity = h / 1024.0f;
}


/******************************************************************************************************************
 * @SECTION: BMP680 support by Bosch https://github.com/BoschSensortec/BME68x-Sensor-API
*******************************************************************************************************************/


void mBME::Bme68x_Delayus(uint32_t period, void *intf_ptr) {
  delayMicroseconds(period);
}

int8_t Bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t*)intf_ptr;
  return pCONT_i2c->I2cReadBuffer(dev_addr, reg_addr, reg_data, (uint16_t)len);
}

int8_t Bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
  uint8_t dev_addr = *(uint8_t*)intf_ptr;
  return pCONT_i2c->I2cWriteBuffer(dev_addr, reg_addr, (uint8_t *)reg_data, (uint16_t)len);
}

bool mBME::Bme680Init(uint8_t bmp_idx) {
  if (!bme_dev) {
    bme_heatr_conf = (bme68x_heatr_conf*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_heatr_conf));
    bme_conf = (bme68x_conf*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_conf));
    bme_dev = (bme68x_dev*)malloc(BMP_MAX_SENSORS * sizeof(bme68x_dev));
  }
  if (!bme_dev) { return false; }

  bme_dev[bmp_idx].intf_ptr = &bmp_sensors[bmp_idx].i2c_address;
  bme_dev[bmp_idx].intf = BME68X_I2C_INTF;
  bme_dev[bmp_idx].read = Bme68x_i2c_read;
  bme_dev[bmp_idx].write = Bme68x_i2c_write;
  bme_dev[bmp_idx].delay_us = Bme68x_Delayus;
  // amb_temp can be set to 25 prior to configuring the gas sensor
  // or by performing a few temperature readings without operating the gas sensor.
  bme_dev[bmp_idx].amb_temp = 25;
  int8_t rslt = bme68x_init(&bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  //  ALOG_DBG(PSTR("BME: Gas variant %d"), bme_dev[bmp_idx].variant_id);

  //  rslt = bme68x_get_conf(&bme_conf[bmp_idx], &bme_dev[bmp_idx]);
  //  if (rslt != BME68X_OK) { return false; }
  // Set the temperature, pressure and humidity settings
  bme_conf[bmp_idx].os_hum = BME68X_OS_2X;
  bme_conf[bmp_idx].os_pres = BME68X_OS_4X;
  bme_conf[bmp_idx].os_temp = BME68X_OS_8X;
  bme_conf[bmp_idx].filter = BME68X_FILTER_SIZE_3;
  bme_conf[bmp_idx].odr = BME68X_ODR_NONE;          // This parameter defines the sleep duration after each profile
  rslt = bme68x_set_conf(&bme_conf[bmp_idx], &bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  // Set the gas sensor settings
  bme_heatr_conf[bmp_idx].enable = BME68X_ENABLE;
  // Create a ramp heat waveform in 3 steps
  bme_heatr_conf[bmp_idx].heatr_temp = 320;  // degree Celsius
  bme_heatr_conf[bmp_idx].heatr_dur = 150;   // milliseconds
  rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heatr_conf[bmp_idx], &bme_dev[bmp_idx]);
  if (rslt != BME68X_OK) { return false; }

  bmp_sensors[bmp_idx].bme680_state = 0;

  return true;
}


void mBME::Bme680Read(uint8_t bmp_idx)
{
  if (!bme_dev) { return; }

  int8_t rslt = BME68X_OK;

  if (BME680_CHIPID == bmp_sensors[bmp_idx].bmp_type) {
    if (0 == bmp_sensors[bmp_idx].bme680_state) {
      // Trigger the next measurement if you would like to read data out continuously
      rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme_dev[bmp_idx]);
      if (rslt != BME68X_OK) { return; }
      bmp_sensors[bmp_idx].bme680_state = 1;
    } else {
      bmp_sensors[bmp_idx].bme680_state = 0;

      struct bme68x_data data;
      uint8_t n_fields;
      rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme_dev[bmp_idx]);
      if (rslt != BME68X_OK) { return; }

      bmp_sensors[bmp_idx].temperature = data.temperature;          // Temperature in degree celsius
      bmp_sensors[bmp_idx].humidity = data.humidity;                // Humidity in % relative humidity
      bmp_sensors[bmp_idx].pressure = data.pressure / 100.0f;        // Pressure in Pascal (converted to hPa)
      // Avoid using measurements from an unstable heating setup
      if (data.status & BME68X_GASM_VALID_MSK) {
        bmp_sensors[bmp_idx].bmp_gas_resistance = data.gas_resistance / 1000.0f;  // Gas resistance in Ohms (converted to kOhm)
      } else {
        bmp_sensors[bmp_idx].bmp_gas_resistance = 0;
      }
    }
  }
  return;
}


/********************************************************************************************/

#ifdef USE_DEEPSLEEP

void mBME::BMP_EnterSleep(void)
{
  if (DeepSleepEnabled()) {
    for (uint32_t bmp_idx = 0; bmp_idx < bmp_count; bmp_idx++) {
      switch (bmp_sensors[bmp_idx].bmp_type) {
        case BMP180_CHIPID:
        case BMP280_CHIPID:
        case BME280_CHIPID:
          pCONT_i2c->I2cWrite8(bmp_sensors[bmp_idx].i2c_address, BMP_REGISTER_RESET, BMP_CMND_RESET);
          break;
        default:
          break;
      }
    }
  }
}

#endif // USE_DEEPSLEEP


/******************************************************************************************************************
 * @SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mBME::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  char buffer[50];
  JBI->Start();
    JBI->Add_P(PM_SENSOR_COUNT, bmp_count);

    for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++){
      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));   
        JBI->Add_P(PM_TYPE,bmp_sensors[sensor_id].bmp_type);
      JBI->Object_End();
    }

  return JBI->End();

}

uint8_t mBME::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();

  char buffer[50];

  for(uint8_t sensor_id = 0; sensor_id<bmp_count; sensor_id++){
    if(
      bmp_sensors[sensor_id].ischanged_over_threshold || 
      (json_level >  JSON_LEVEL_IFCHANGED) || 
      (json_level == JSON_LEVEL_SHORT)
    ){
      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer)));   
        JBI->Add_P(PM_TEMPERATURE, bmp_sensors[sensor_id].temperature);
        JBI->Add_P(PM_HUMIDITY, bmp_sensors[sensor_id].humidity);
        JBI->Add_P(PM_PRESSURE, bmp_sensors[sensor_id].pressure);
        JBI->Add_P(PM_ALTITUDE, bmp_sensors[sensor_id].altitude);
        if(bmp_sensors[sensor_id].bmp_type == BME680_CHIPID)
        {
          JBI->Add_P(PM_GAS, bmp_sensors[sensor_id].bmp_gas_resistance);
        }
        JBI->Add_P(PM_UTC_TIME, bmp_sensors[sensor_id].utc_measured_timestamp);
        uint32_t sensor_elapsed_time = tkr_time->UtcTime() - bmp_sensors[sensor_id].utc_measured_timestamp;
        JBI->Add_P(PM_AGE, sensor_elapsed_time);
      JBI->Object_End();
    }
  }
  
  return JBI->End();

}

  
/******************************************************************************************************************
 * @SECTION: MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mBME::MQTTHandler_Init()
{

  struct handler<mBME>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0; // as 0, with SendNow=false, will trigger to send for the first time
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->GetConfigPeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = pCONT_mqtt->GetTelePeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false; // Handled by MQTTHandler_Rate
  ptr->tRateSecs = pCONT_mqtt->GetIfChangedPeriod_SubModule(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_SENSORS_BME
