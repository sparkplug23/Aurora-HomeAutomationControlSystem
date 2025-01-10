#include "9_Controller/41_HVAC_Remote/mHVAC.h"


#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

#ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

// Not adding Temp based controls in 2023 to home heating. Not until its very stable and only ever
// via a swapped out esp32 (ie have one with timer only as back up)


void ProgramTemperature::EverySecond(void)
{

  #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR("ProgramTemperature::EverySecond"));
  #endif

  /**
   * increment any timer variables if active (ie >0 )
   * */
  if(time_running.on >= 0) time_running.on++;
  if(time_maintaining.on >= 0) time_maintaining.on++;

  /**
   * Check (Here I will introduce other heating profiles, ie cycling the maintain period)
   * */
  if(mode_heating_enabled) CheckRunningProgram_Heating_Profile1();
  // if(mode_cooling_enabled) CheckRunningProgram_Cooling();

  // ischanged = true;
  
}

/**
 * @brief This section will
 * */
void ProgramTemperature::CheckRunningProgram_Heating_Profile1()
{

  // if timer is active
  if(time_running.on >= 0){

    /**
     * @brief Time checks - If maximum "time on" has been reached, turn EVERYTHING off
     * @note TimeMaintain includes/overlaps the "on" period
     * */
    if(time_running.on < time_running.limit){     //stay on
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_running = on [%d] < limit [%d]"),time_running.on,time_running.limit);
      isrunning_heating = true;
    }
    else{       
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Time On Exceeded - Turn Zone OFF"));
      time_running.on = -1; //deactviate
      time_maintaining.on = -1; // Turn maintaining period on
      isrunning_heating = false;
    }

    /**
     * @brief Temp checks (Has the current values reached the desired), if so, also continue if maintain is set
     * @note This week need a better method in the future, to "cycle" the maintain period on/off (with different "profiles" available)
     * @note Currently, maintain in the home heating assumes the cycling is handled by a physical real world thermostat
     * */
    if(temperature.current >= temperature.desired){   
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured = Current [%d] > Set [%d]"),(int)temperature.current,(int)temperature.desired);

      /**
       * If maintain was not counting yet, turn it on for first temp crossing above threshold
       * */
      if(time_maintaining.on == -1)
      {
        time_maintaining.on = 0;
      }

      // Check if temp should be maintained above desired point
      if((time_maintaining.on>=0)&&(time_maintaining.on < time_maintaining.limit)){
        ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_maintaining = on [%d] < limit [%d]"),time_maintaining.on,time_maintaining.limit);
        isrunning_heating = true;
      }
      // Turn off
      else{
        ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Time Maintaining Exceededon%d [%d] < limit [%d]"),time_maintaining.on, time_maintaining.on,time_maintaining.limit);
        time_running.on = -2;
        time_maintaining.on = -1;
        isrunning_heating = false;
      }
    }
    else{ 
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured = Current [%d] < Set [%d]"),(int)temperature.current,(int)temperature.desired);
    }

  }// time_on is not running
  else{ 
    ALOG_DBM( PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_on is not running"));
    // isrunning_heating = false;
  }


}

/**
 * @brief This section will
 * */
void ProgramTemperature::CheckRunningProgram_Cooling()
{

  // if timer is active
  if(time_running.on >= 0){

    /**
     *  Time checks - If limit is hit, stop, or else continue maintaining on
     * */
    if(time_running.on > time_running.limit){   
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),time_running.on,time_running.limit);
      time_running.on = -2; //deactviate
      time_maintaining.limit = -1;
      isrunning_cooling = false;
    }else
    { //stay on
      isrunning_cooling = true;
    }

    /**
     * Temp checks (Has the current values reached the desired)
     * */
    if(temperature.current <= temperature.desired){    // Also need to add a "fudge" value
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured Temp [%d] > Set Temp [%d]"),(int)temperature.current,(int)temperature.desired);
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Exceeded Set Point"));

      // Check if temp should be maintained above desired point
      if((time_maintaining.on>=0)&&(time_maintaining.on < time_maintaining.limit)){
        isrunning_cooling = true;
      }
      // Turn off
      else{
        ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Time Maintaining Exceeded"));
        time_running.on = -2;
        time_maintaining.limit = -1;
        isrunning_cooling = false;
      }
    }
    else{ 
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured Temp [%d] < Set Temp [%d]"),(int)temperature.current,(int)temperature.desired);
    }

  }// time_on is not running
  else{ 
    ALOG_DBM( PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_on is not running"));
  }


}

void mHVAC::CommandSet_ProgramTemperature_Desired_Temperature(uint8_t zone_id, float value)
{

  zone[zone_id].program_temp_method->SetDesiredTemperature(value);
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TEMPERATURE D_SET)), (int)zone[zone_id].program_temp_method->GetDesiredTemperature());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void mHVAC::CommandSet_ProgramTemperature_Mode(uint8_t zone_id, uint8_t value)
{

  zone[zone_id].program_temp_method->SetMode(value);
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TEMPERATURE,D_MODE)), zone[zone_id].program_temp_method->GetMode());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}



void mHVAC::CommandSet_ProgramTemperature_TimeRunning_Limit(uint8_t device_id, uint8_t value)
{

  //   program_temps[device_id].time_running.limit = value;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   ALOG_INF(PSTR(D_LOG_HEATING D_PARSING_MATCHED D_COMMAND_SVALUE_NVALUE_K(D_TIME_RUNNING,D_LIMIT)),program_temps[device_id].time_running.limit);
  // #endif
  //   // fForceHeatingTempsUpdate = true;
  //   data_buffer.isserviced++;
}




void mHVAC::CommandSet_ProgramTemperature_Schedule_Mode(uint8_t device_id, int8_t value)
{

  // program_temps[device_id].schedule.mode_sch = value;
  // // fForceHeatingTempsUpdate = true;

  // ALOG_TST(PSTR("mode_sch=%d"),program_temps[device_id].schedule.mode_sch);
  
  // if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID)
  // {
  //   program_temps[device_id].time_running.on = 0;
  // }else 
  // if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID)
  // {
  //   program_temps[device_id].time_running.on = -1;
  //   program_temps[device_id].time_maintaining.on = -1;
  //   SetHeater(device_id,0);
  // }
  
  // ALOG_TST(PSTR("mode_sch=%d %d %d %d"),
  // device_id,
  // program_temps[device_id].time_running.on,
  // program_temps[device_id].time_maintaining.on,
  // program_temps[device_id].schedule.mode_sch);

  //   //ALOG_INF(PSTR(D_LOG_HEATING D_PARSING_MATCHED D_COMMAND_SVALUE),D_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TEMPERATURE D_SET)), (int)value);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

}



#endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

#endif









  // // If active
  // if(time_on_seconds_decounter)
  // {
  //   // 3 options
  //   /**
  //    * heat
  //    * heat maintain
  //    * cool
  //    * cool maintain
  //    * 
  //    * maintain exceeded, standby
  //    * dont allow cycling between modes, its either hear or cool for X minutes?
  //    * 
  //    * */

  //   // If negative error, we must heat
  //   if(GetErrorTemperature()<0)
  //   {
  //     ALOG_INF(PSTR("We must heat"));
  //   }

  //   if(GetErrorTemperature()>0)
  //   {
  //     ALOG_INF(PSTR("We must cool"));
  //   }



  //   /**
  //    * @note Change
  //    * */
  //   // switch(schedule.mode_sch){
  //   //   case SCHEDULED_MANUAL_ON_ID:
  //   //     status.mode = TEMP_MODE_HVAC_ID;
  //   //     activeprograms[device_id].temps.state = ACTIVEP_ON;
  //   //     fRunTemp = true;
  //   //     break;
  //   //   // case SCHEDULED_ON_ID: //IE RUNNING
  //   //   //   status.mode = TEMP_MODE_HVAC_ID;
  //   //   //   activeprograms[device_id].temps.state = ACTIVEP_ON;
  //   //   //   fRunTemp = true;
  //   //   //   //if on, check time limits
  //   //   // break;
  //   //   // // Awaiting for time before turning on 
  //   //   // // NEEDS complete revamp, to allow advanced scheduling (15 minute increments of on and off)
  //   //   // // use binary?

  //   //   // case SCHEDULED_SET_ID:{

  //   //   //   //status.mode = TEMP_MODE_SCHEDULED;

  //   //   //   activeprograms[device_id].temps.state = ACTIVEP_SCHEDULED;

  //   //   //   //uint8_t fTimeReached = tkr_time->CheckBetween_Week_DateTimes(&schedule.ontime,&schedule.offtime);

  //   //   //   uint8_t fTimeReached = tkr_time->CheckDateTimeWeekIsNow(&schedule.ontime);

  //   //   //   schedule.untilontime = tkr_time->GetDifferenceInDateTimes(&tkr_time->RtcTime,&schedule.ontime);

  //   //   //   ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "CheckBetween_Week_DateTimes [%d]"),fTimeReached);

  //   //   //   // uint8_t hours, minutes, seconds;
  //   //   //   // tkr_time->DateTimeWeek2HHMMSS(&schedule.untilontime,&hours,&minutes,&seconds);
  //   //   //   // if is reached AND NOT run yet
  //   //   //   if(fTimeReached&&(!schedule.fHasRun)){
  //   //   //     schedule.fHasRun = 1;
  //   //   //     schedule.mode_sch = SCHEDULED_ON_ID;
  //   //   //     time_running.on = 0;
  //   //   //   }

  //   //   //   // if(schedule.fRunning){}

  //   //   //   // check times and update time until it will run (use this as seconds count check?)
  //   //   //   fRunTemp = false;
  //   //   // }
  //   //   // break;
  //   //   case SCHEDULED_OFF_ID: // off, no temp heating
  //   //     activeprograms[device_id].temps.state = ACTIVEP_OFF;
  //   //     fRunTemp = false;
  //   //   break;
  //   // }
  // }

  /*



{
  "HeatingDevice": 0,
  "Temperature": {
    "StartDesired": 26.8
  },
  "TimeRunning":{
    "Limit":40
  },
  "TimeMaintaining":{
    "Limit":1
  }
}







  */