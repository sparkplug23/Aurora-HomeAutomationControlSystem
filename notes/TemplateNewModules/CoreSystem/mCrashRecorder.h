/*
  mHVAC.cpp - mBME

  Copyright (C) 2021  Michael

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
#ifndef MSENSORSBME_H
#define MSENSORSBME_H

#define D_UNIQUE_MODULE_SENSORS_BME_ID 5003 // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BME

#include <bme68x.h>
// #include "bme68x_defs.h"

#include <Wire.h>

/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280, BME680 - Pressure, Temperature, Humidity (BME280/BME680) and gas (BME680)
 *
 * Source: Heiko Krupp and Adafruit Industries
 *
 * I2C Address: 0x76 or 0x77
\*********************************************************************************************/

#define BMP180_CHIPID        0x55
#define BMP280_CHIPID        0x58
#define BME280_CHIPID        0x60
#define BME680_CHIPID        0x61 // gas

#define I2C_ADDRESS_BME280_1 0x76 // pin low
#define I2C_ADDRESS_BME280_2 0x77 // pin high


/*********************************************************************************************\
 * BMP085, BMP180, BMP280, BME280, BME680 - Pressure, Temperature, Humidity (BME280/BME680) and gas (BME680)
 *
 * Source: Heiko Krupp and Adafruit Industries
 *
 * I2C Address: 0x76 or 0x77
\*********************************************************************************************/

#define USE_BME68X

#define BMP_ADDR1            0x76
#define BMP_ADDR2            0x77

#define BMP180_CHIPID        0x55 // 85 Dec
#define BMP280_CHIPID        0x58 // 88 Dec
#define BME280_CHIPID        0x60 // 96 Dec
#define BME680_CHIPID        0x61 // 97 Dec

#define BMP_REGISTER_CHIPID  0xD0

#define BMP_REGISTER_RESET   0xE0 // Register to reset to power on defaults (used for sleep)

#define BMP_CMND_RESET       0xB6 // I2C Parameter for RESET to put BMP into reset state

#define BMP_MAX_SENSORS      2

#define BMP_MAX_SENSORS      2

class Adafruit_BME280;

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"


class mBME :
  public mTaskerInterface
{
  
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mBME(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_SENSORS_BME_CTR = D_MODULE_SENSORS_BME_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BME_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BME_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot

    }rt;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
            
    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
            
    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return bmp_count;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > bmp_count-1) {value->sensor_type.push_back(0); return ;}  
      
      value->timestamp = bmp_sensors[index].utc_measured_timestamp;

      switch (bmp_sensors[index].bmp_type) 
      {
        case BME280_CHIPID:
          value->sensor_type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
          value->data_f.push_back(bmp_sensors[index].humidity);
          // nobreak intentional
        case BMP280_CHIPID:    
        case BMP180_CHIPID:        
          value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
          value->data_f.push_back(bmp_sensors[index].temperature);
          value->sensor_type.push_back(SENSOR_TYPE_PRESSURE_ID);
          value->data_f.push_back(bmp_sensors[index].pressure);          
        break;
        case BME680_CHIPID:        
          value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
          value->data_f.push_back(bmp_sensors[index].temperature);
          value->sensor_type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
          value->data_f.push_back(bmp_sensors[index].humidity);   
          value->sensor_type.push_back(SENSOR_TYPE_PRESSURE_ID);
          value->data_f.push_back(bmp_sensors[index].pressure);
          value->sensor_type.push_back(SENSOR_TYPE_GAS_RESISTANCE_ID);
          value->data_f.push_back(bmp_sensors[index].bmp_gas_resistance);   
        break;
      }
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();    
    std::vector<struct handler<mBME>*> mqtthandler_list;
    struct handler<mBME> mqtthandler_settings;
    struct handler<mBME> mqtthandler_sensor_ifchanged;
    struct handler<mBME> mqtthandler_sensor_teleperiod;    
    #endif // USE_MODULE_NETWORK_MQTT
 
};

#endif // USE_MODULE_SENSORS_BME
#endif // Header guard
