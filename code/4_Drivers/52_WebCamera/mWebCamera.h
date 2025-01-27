/*
  mWebCamera.h - PZEM004T v3 MODBUS

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

/**
 * @brief Note
 * 
 * https://thecustomizewindows.com/2020/03/solve-esp32-sketch-too-big-error-on-arduino-ide/
 * Stock arduinocore example for camera integrated including webpage
 * 
 */
#ifndef HEADER_DRIVERS__CAMERA_WEBCAM_V4_H
#define HEADER_DRIVERS__CAMERA_WEBCAM_V4_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_DRIVERS__CAMERA_WEBCAM_V4_ID 4052 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4

#include <esp_camera.h>
#include "esp_camera.h"

#include <Arduino.h>


// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD

#include "camera_pins.h"

// #ifndef APP_HTTP
// #define APP_HTTP
// #include "app_httpd.cpp"
// #endif //APP_HTTP
    


#include "1_TaskerManager/mTaskerInterface.h"

class mWebCamera :
  public mTaskerInterface
{

  private:
  public:
    mWebCamera(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_CTR = D_MODULE_DRIVERS_CAMERA_WEBCAM_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS__CAMERA_WEBCAM_V4_ID; }
    

// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "HACS2400";
const char* password = "af4d8bc9ab";

// void startCameraServer();

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // uint8_t found = 0;
      // uint16_t rate_measure_ms = 1000;
      // // uint8_t sensor_count = 1;
      // uint8_t active_sensor = 0;
    }settings;

    void Pre_Init();
    void Init();

    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();

    struct handler<mWebCamera> mqtthandler_settings;
    struct handler<mWebCamera> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    
      
    struct handler<mWebCamera>* mqtthandler_list[2] = {
      &mqtthandler_settings,
      &mqtthandler_state_ifchanged
    };

};

#endif // USE_MODULE_DRIVERS_CAMERA_WEBCAM

#endif
