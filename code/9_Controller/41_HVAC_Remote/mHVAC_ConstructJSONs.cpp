/*
  mHVAC.cpp - Generic Heating Controller

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
#include "mHVAC.h"

// #define USE_MODULE_CONTROLLER_HVAC_REMOTE

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE


uint8_t mHVAC::ConstructJSON_ProgramTimers(uint8_t json_level){

  // DEBUG_LINE_HERE;
  char buffer[50];

  JBI->Start();
  
  for(uint8_t zone_id=0; zone_id<settings.active_zones; zone_id++)
  {
    JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),zone_id,buffer,sizeof(buffer)));
    /**
     * If time -1, ie disabled, then just send as zero
     * */
      JBI->Add(D_TIME_ON, zone[zone_id].program_timer_method->GetTimer_Minutes() != -1 ? zone[zone_id].program_timer_method->GetTimer_Minutes() : 0);
      JBI->Add(D_TIME_ON_SECS, zone[zone_id].program_timer_method->GetTimer_Seconds());
    JBI->Object_End();
  }

  return JBI->End();
  
}


uint8_t mHVAC::ConstructJSON_ProgramTemps(uint8_t json_level){
  
  // DEBUG_LINE_HERE;
  char buffer[50];

  JBI->Start();
  
  #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  
  for(int zone_id=0; zone_id<settings.active_zones; zone_id++){

    JBI->Level_Start_P( DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),zone_id,buffer,sizeof(buffer)) );
  
      JBI->Level_Start_P(D_TEMPERATURE);   
        JBI->Add(D_CURRENT, zone[zone_id].program_temp_method->GetCurrentTemperature());
        JBI->Add(D_DESIRED, zone[zone_id].program_temp_method->GetDesiredTemperature());
        JBI->Add(D_ERROR,   zone[zone_id].program_temp_method->GetErrorTemperature());
      JBI->Object_End();
      JBI->Level_Start_P(D_STATUS);   
        // JBI->Add(D_MODE, GetTempModeByDeviceIDCtr(zone_id, buffer, sizeof(buffer)));
        // JBI->Add(D_DATA, program_temps[zone_id].status.data.ctr);
      JBI->Object_End();
      JBI->Level_Start_P(D_TIME_RUNNING); 
        JBI->Add(D_TIME_ON, zone[zone_id].program_temp_method->GetTimer_Running_Minutes() != -1 ? zone[zone_id].program_temp_method->GetTimer_Running_Minutes() : 0);
        JBI->Add(D_TIME_ON_SECS, zone[zone_id].program_temp_method->GetTimer_Running_Seconds());  
        JBI->Add(D_LIMIT, zone[zone_id].program_temp_method->GetTimer_Running_Limit_Minutes());
        JBI->Add(D_LIMIT D_SECS, zone[zone_id].program_temp_method->GetTimer_Maintaining_Limit_Seconds());        
      JBI->Object_End();  
      JBI->Level_Start_P(D_TIME_MAINTAINING); 
        JBI->Add(D_TIME_ON, zone[zone_id].program_temp_method->GetTimer_Maintaining_Minutes() != -1 ? zone[zone_id].program_temp_method->GetTimer_Maintaining_Minutes() : 0);
        JBI->Add(D_TIME_ON_SECS, zone[zone_id].program_temp_method->GetTimer_Maintaining_Seconds());  
        JBI->Add(D_LIMIT, zone[zone_id].program_temp_method->GetTimer_Maintaining_Minutes());
        JBI->Add(D_LIMIT D_SECS, zone[zone_id].program_temp_method->GetTimer_Maintaining_Limit_Seconds());  
      JBI->Object_End();  
      JBI->Level_Start_P(D_TIME_TO_HEAT);
        //  time_to_heatobj[D_SECONDS] = GetHeatingProfilesTimeSeconds(zone_id,program_temps[zone_id].temp.current,program_temps[zone_id].temp.desired);
        //       //   time_to_heatobj[D_MINUTES] = GetHeatingProfilesTimeMinutes(zone_id,program_temps[zone_id].temp.current,program_temps[zone_id].temp.desired);    
      JBI->Object_End();  
      JBI->Level_Start_P(D_SCHEDULE); 
        // JBI->Add(D_MODE, GetScheduleNameCtrbyID(program_temps[zone_id].schedule.mode_sch, buffer, sizeof(buffer)));
        // JBI->Add(D_ISRUNNING, program_temps[zone_id].schedule.fRunning);     
        // char time_ctr[20]; memset(time_ctr,'\0',sizeof(time_ctr));
        // sprintf(time_ctr, "%02d:%02d:%02d",(int)program_temps[zone_id].schedule.ontime.hour,(int)program_temps[zone_id].schedule.ontime.minute,(int)program_temps[zone_id].schedule.ontime.second);
        // scheduleobj[D_ONTIME] = time_ctr;//tkr_time->getFormattedTime();
        // char time_ctr2[20];
        // memset(time_ctr2,'\0',sizeof(time_ctr2));
        // sprintf(time_ctr2, "%02d:%02d:%02d",(int)program_temps[zone_id].schedule.offtime.hour,(int)program_temps[zone_id].schedule.offtime.minute,(int)program_temps[zone_id].schedule.offtime.second);
        // scheduleobj[D_OFFTIME] = time_ctr2;//tkr_time->getFormattedTime();   
      JBI->Object_End();  
    JBI->Object_End();
  }

  #endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

  return JBI->End();

}


uint8_t mHVAC::ConstructJSON_ProgramActive(uint8_t json_level){
  JBI->Start();
  // DEBUG_LINE_HERE;
    JBI->Level_Start_P(D_RELAY);   
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_US, GetHeatingRelay(DEVICE_US_ID));
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_DS, GetHeatingRelay(DEVICE_DS_ID));
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_IH, GetHeatingRelay(DEVICE_IH_ID));
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_WB, GetHeatingRelay(DEVICE_WB_ID));
      // JBI->Add(D_ANY,                     GetAnyHeatingRelay());      
    JBI->Object_End();
    JBI->Level_Start_P(D_TIMER);  
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].timers.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].timers.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].timers.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].timers.state); 
    JBI->Object_End();
    JBI->Level_Start_P(D_TEMPERATURE);  
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].temps.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].temps.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].temps.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].temps.state); 
    JBI->Object_End();
    JBI->Level_Start_P(D_STATUS);  
      // JBI->Add(D_MESSAGE, status_message.ctr);  //nice human message that remains the same
      // JBI->Add(D_LENGTH, status_message.len); 
      // JBI->Add(D_IMPORTANCE, status_message.importance); 
    JBI->Object_End();

    // JBI->Level_Start_P(D_STATUS "_Short");  
    //   JBI->Add(D_MESSAGE, status_message.ctr);  //nice human message that remains the same
    //   JBI->Add(D_LENGTH, status_message.len); 
    //   JBI->Add(D_IMPORTANCE, status_message.importance); 
    // JBI->Object_End();

  return JBI->End();
}


uint8_t mHVAC::ConstructJSON_HeatingRelays(uint8_t json_level){

  // DEBUG_LINE_HERE;
  char buffer[50];
  JBI->Start();
  for(int device_id=0;device_id<settings.active_zones;device_id++){
    JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID(pCONT_mry->GetModuleUniqueID(), device_id, buffer, sizeof(buffer)));
      JBI->Add_FV(D_ONTIME, PSTR("\"%02d:%02d:%02d\""),  pCONT_mry->relay_status[device_id].last.ontime.hour,  pCONT_mry->relay_status[device_id].last.ontime.minute,  pCONT_mry->relay_status[device_id].last.ontime.second);
      JBI->Add_FV(D_OFFTIME, PSTR("\"%02d:%02d:%02d\""), pCONT_mry->relay_status[device_id].last.offtime.hour,  pCONT_mry->relay_status[device_id].last.offtime.minute,  pCONT_mry->relay_status[device_id].last.offtime.second);
      JBI->Add(D_TIME_ON "_Seconds",   pCONT_mry->relay_status[device_id].time_seconds_on);
      JBI->Add(D_TIME_ON "_Mins",   pCONT_mry->relay_status[device_id].time_seconds_on/60);
    JBI->Object_End();
  }
  return JBI->End();

}


uint8_t mHVAC::ConstructJSON_ZoneSensors(uint8_t json_level){

  // DEBUG_LINE_HERE;
  char buffer[25];

  JBI->Start();

  for(int zone_id=0;zone_id<settings.active_zones;zone_id++){
    JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), zone_id, buffer, sizeof(buffer)));
      // if(zone[zone_id].sensor.temperature){ JBI->Add(D_TEMPERATURE, zone[zone_id].sensor.temperature); }
      // if(zone[zone_id].sensor.humidity)   { JBI->Add(D_HUMIDITY, zone[zone_id].sensor.humidity);       }
      JBI->Add("ModuleID",zone[zone_id].sensor.module_id);
      JBI->Add("Index",zone[zone_id].sensor.index);
    JBI->Object_End();
  }
  return JBI->End();

  // DEBUG_LINE_HERE;
}


uint8_t mHVAC::ConstructJSON_ZoneSensors_ROC1m(uint8_t json_level){

  JBI->Start();
  // DEBUG_LINE_HERE;
  return JBI->End();

}


uint8_t mHVAC::ConstructJSON_ZoneSensors_ROC10m(uint8_t json_level){

  JBI->Start();
  // DEBUG_LINE_HERE;
  return JBI->End();
}


uint8_t mHVAC::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // return 0;
  // DEBUG_LINE_HERE;
  // DEBUG_LINE_HERE;
  // DEBUG_LINE_HERE;
  // 

// Serial.printf("settings.active_zones=%d", settings.active_zones); Serial.flush();


  JBI->Start();
    JBI->Add("active_zones", settings.active_zones);

    JBI->Object_Start("Zone");
    
        // JBI->Object_Start("BitPacked_Modes_Enabled");
        //   for(int8_t zone_id=0;zone_id<settings.active_zones;zone_id++)
        //   {
        //     JBI->Array_Start_P(PSTR("Zone%d"),zone_id);
        //       for(int8_t bits=0; bits<8; bits++)
        //       {
        //         JBI->Add(bitRead(zone[zone_id].bitpacked_modes_enabled,bits));
        //       }
        //     JBI->Array_End();
        //   }         
        // JBI->Object_End();



      JBI->Object_Start("Driver");

        JBI->Array_Start("ModuleID");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(zone[i].output.module_ids[0]);
        }
        JBI->Array_End();

        JBI->Array_Start("Index");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(zone[i].output.index[0]);
        }
        JBI->Array_End();
      
      JBI->Object_End();
      
      
      JBI->Object_Start("Sensor");

        JBI->Array_Start("ModuleID");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(zone[i].sensor.module_id);
        }
        JBI->Array_End();

        JBI->Array_Start("Index");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(zone[i].sensor.index);
        }
        JBI->Array_End();
        
        JBI->Array_Start("Temperature");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(zone[i].sensor.temperature);
        }
        JBI->Array_End();
        
        JBI->Array_Start("Humidity");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(zone[i].sensor.humidity);
        }
        JBI->Array_End();
      
      JBI->Object_End();

    JBI->Object_End();

      DEBUG_LINE_HERE;
  return JBI->End();


}




#endif
