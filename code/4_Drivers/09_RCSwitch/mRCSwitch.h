/*
  mPZEM004T.h - PZEM004T v3 MODBUS

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
#ifndef HEADER_DRIVERS_RF433_RCSWITCH_H
#define HEADER_DRIVERS_RF433_RCSWITCH_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_DRIVERS_RF433_RCSWITCH_ID  4009 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

#include <RCSwitch.h>

class mRCSwitch :
  public mTaskerInterface
{

  private:
  public:
    mRCSwitch(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_DRIVERS_RF433_RCSWITCH_CTR = D_MODULE_DRIVERS_RF433_RCSWITCH_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_RF433_RCSWITCH_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_RF433_RCSWITCH_ID; }
    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // uint8_t found = 0;
      // uint16_t rate_measure_ms = 1000;
      // // uint8_t sensor_count = 1;
      // uint8_t active_sensor = 0;
    }settings;

    #define D_RF_PROTOCOL "Protocol"
    #define D_RF_BITS "Bits"
    #define D_RF_DATA "Data"
    #define D_RF_PULSE "Pulse"


    #define RF_TIME_AVOID_DUPLICATE 1000  // Milliseconds
  
    RCSwitch *mySwitch = nullptr;//RCSwitch();

    void Pre_Init();
    void Init();

    void ReceiveCheck(void);
    void parse_JSONCommand(JsonParserObject obj);

    struct RECEIVED_PACKET
    {
      uint32_t data = 0;
      uint16_t bit_length = 0;
      int16_t  protocol = -1;
      uint16_t delay = 0;
      uint32_t received_time_millis = 0;
      uint32_t received_utc_time;
    }rx_pkt;

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();

    struct handler<mRCSwitch> mqtthandler_settings;
    struct handler<mRCSwitch> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    
      
    struct handler<mRCSwitch>* mqtthandler_list[2] = {
      &mqtthandler_settings,
      &mqtthandler_state_ifchanged
    };




};

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH

#endif
