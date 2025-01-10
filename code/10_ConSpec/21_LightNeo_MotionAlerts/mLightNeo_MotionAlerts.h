/*
  mLightNeo_MotionAlerts.h 
  
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
#ifndef HEADER__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__H
#define HEADER__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__ID   10021 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mLightNeo_MotionAlerts :
  public mTaskerInterface
{

  private:
  public:
  
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mLightNeo_MotionAlerts(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__CTR = D_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__ID; }    
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    #if defined(ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS) && defined(USE_MODULE_CORE_FILESYSTEM)
    void Load_Module(bool erase = false);
    void Save_Module(void);
    bool Default_Module(void);
    #endif // USE_MODULE_CORE_FILESYSTEM

    /**
     * @brief
     **/
    struct MODULE_RUNTIME{ // these will be saved and recovered on boot

    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void EverySecond();
    void Handle_ButtonEvents();
    void Handle_MotionEvents();

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void   parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true); //default is true
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mLightNeo_MotionAlerts>*> mqtthandler_list;
    struct handler<mLightNeo_MotionAlerts> mqtthandler_settings;
    struct handler<mLightNeo_MotionAlerts> mqtthandler_state_ifchanged;
    struct handler<mLightNeo_MotionAlerts> mqtthandler_state_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT




    

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

#endif
