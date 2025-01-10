#include "9_Controller/40_HVAC/internal/mProgramTemperatue.h"









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