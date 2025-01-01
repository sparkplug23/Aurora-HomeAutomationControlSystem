#include "mTime.h"

/**
 * @brief: Module Interface
 * */
int8_t mTime::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:    
      #ifdef ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND
      Serial.println(GetUptime().c_str());
      #endif
      WifiPollNtp();
      uptime_seconds_nonreset++;
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef ENABLE_DEBUGFEATURE_TIME__MQTT_DIRECT_PUBLISH_WITHOUT_TELEMETRY
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
    #endif // ENABLE_DEBUGFEATURE_TIME__MQTT_DIRECT_PUBLISH_WITHOUT_TELEMETRY
  } // end switch
} // END function


void mTime::Pre_Init(void)
{
  Rtc.millis = millis();
}


uint32_t mTime::UtcTime(void) 
{
  return Rtc.utc_time;
}


uint32_t mTime::LocalTime(void) 
{
  return Rtc.local_time;
}


uint32_t mTime::Midnight(void) 
{
  return Rtc.midnight;
}


bool mTime::MidnightNow(void) 
{
  if (Rtc.midnight_now) {
    Rtc.midnight_now = false;
    return true;
  }
  return false;
}


bool mTime::IsDst(void) 
{
  return (Rtc.time_timezone == tkr_set->Settings.toffset[1]);
}

// // mktime works only with local time
// time_t mTime::GetStartOfDayUTC(time_t utc_time) {
//     // Convert to a tm structure in UTC
//     struct tm* time_info = gmtime(&utc_time);

//     // Reset the hour, minute, and second to zero (midnight)
//     time_info->tm_hour = 0;
//     time_info->tm_min = 0;
//     time_info->tm_sec = 0;

//     // Convert back to time_t (UTC time)
//     return mktime(time_info);
// }
#include <ctime>

time_t my_timegm(struct tm* time_info) {
    // Save the current timezone settings
    time_t local_time;

    // Save the current timezone settings
    char* tz = getenv("TZ");
    setenv("TZ", "UTC", 1);
    tzset();

    // Convert the tm structure to time_t (UTC)
    local_time = mktime(time_info);

    // Restore the original timezone settings
    if (tz) {
        setenv("TZ", tz, 1);
    } else {
        unsetenv("TZ");
    }
    tzset();

    return local_time;
}


time_t mTime::GetStartOfDayUTC(time_t utc_time) {
    // Convert to a tm structure in UTC
    struct tm* time_info = gmtime(&utc_time);

    // Reset the hour, minute, and second to zero (midnight UTC)
    time_info->tm_hour = 0;
    time_info->tm_min = 0;
    time_info->tm_sec = 0;

    // Use timegm() to convert back to UTC time_t
    return my_timegm(time_info);  // Use timegm() instead of mktime()
}


String mTime::GetBuildDateAndTime(void) 
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char bdt[21];
  char *p;
  static const char mdate_P[] PROGMEM = __DATE__;  // "Mar  7 2017"
  char mdate[strlen_P(mdate_P)+1];      // copy on stack first
  strcpy_P(mdate, mdate_P);
  char *smonth = mdate;
  int day = 0;
  int year = 0;

  // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
  uint8_t i = 0;
  for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(nullptr, " ", &p)) {
    switch (i++) {
    case 0:  // Month
      smonth = str;
      break;
    case 1:  // Day
      day = atoi(str);
      break;
    case 2:  // Year
      year = atoi(str);
    }
  }
  char MonthNamesEnglish[sizeof(kMonthNamesEnglish)];
  strcpy_P(MonthNamesEnglish, kMonthNamesEnglish);
  int month = (strstr(MonthNamesEnglish, smonth) -MonthNamesEnglish) /3 +1;
  snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, PSTR(__TIME__));
  return String(bdt);  // 2017-03-07T11:08:02
}


String mTime::GetSyslogDate(char* mxtime) {
  // Mmm dd hh:mm:ss
  // Jan  3 09:23:45
  // Assuming the day hasn't changed yet ;-)
  uint32_t month_idx = (RtcTime.month -1) * 3;
  char month[4] = { 0 };
  strncpy_P(month, kMonthNamesEnglish + month_idx, 3);
  char dt[16];
  snprintf_P(dt, sizeof(dt), PSTR("%s %2d %s"), month, RtcTime.day_of_month, mxtime);
  return String(dt);
}


String mTime::GetDate(void) {
  // yyyy-mm-ddT
  char dt[12];
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT"), RtcTime.year, RtcTime.month, RtcTime.day_of_month);
  return String(dt);
}


String mTime::GetMinuteTime(uint32_t minutes) 
{
  char tm[6];
  snprintf_P(tm, sizeof(tm), PSTR("%02d:%02d"), minutes / 60, minutes % 60);
  return String(tm);  // 03:45
}


String mTime::GetTimeZone(void) 
{
  char tz[7];
  snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), Rtc.time_timezone / 60, abs(Rtc.time_timezone % 60));
  return String(tz);  // -03:45
}


String mTime::GetDuration(uint32_t time) 
{
  char dt[16];
  datetime_t ut;
  BreakTime(time, ut);
  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
  snprintf_P(dt, sizeof(dt), PSTR("%dT%02d:%02d:%02d"), ut.days, ut.hour, ut.minute, ut.second);
  return String(dt);  // 128T14:35:44
}


String mTime::GetDT(uint32_t time) 
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char dt[20];
  datetime_t tmpTime;
  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    tmpTime.year +1970, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);
  return String(dt);  // 2017-03-07T11:08:02
}


String mTime::GetTime(uint8_t time_type)
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;

  switch (time_type)
  {
    default:
    case DT_LOCAL_TIME:{
      
      // make another GetTime (no date function)
      time = Rtc.local_time;
      char dt[20];
      datetime_t tmpTime;
      BreakTime(time, tmpTime);
      snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), tmpTime.hour, tmpTime.minute, tmpTime.second);
      return String(dt);  // 11:08:02
    }
    break;
  }
  
}


/**
 * @brief Testing: New way of getting the TimeOfDay (TOD) in a string format
 * 
 * @param time_type 
 * @return String 
 */
String mTime::GetTimeStr(uint32_t time, bool include_day_of_week)
{
  // HH:MM:SS or WWTHH:MM:SS
        
  char dt[20];
  datetime_t tmpTime;
  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), tmpTime.hour, tmpTime.minute, tmpTime.second);
  return String(dt);  // 11:08:02

}

// might be same as above, possible phase out
String mTime::formatTimeUntil(double time_until_seconds) {
    int hours = static_cast<int>(time_until_seconds) / 3600;
    int minutes = (static_cast<int>(time_until_seconds) % 3600) / 60;
    int seconds = static_cast<int>(time_until_seconds) % 60;

    char buffer[9]; // HH:MM:SS is 8 characters + null terminator
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return String(buffer);
}



/*
 * timestamps in https://en.wikipedia.org/wiki/ISO_8601 format
 *
 *  DT_UTC     - current data and time in Greenwich, England (aka GMT)
 *  DT_LOCAL   - current date and time taking timezone into account
 *  DT_RESTART - the date and time this device last started, in local timezone
 *
 * Format:
 *  "2017-03-07T11:08:02-07:00" - if DT_LOCAL and SetOption52 = 1
 *  "2017-03-07T11:08:02"       - otherwise
 */
String mTime::GetDateAndTime(uint8_t datetime_type) 
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;
  
  switch (datetime_type) {
    case DT_LOCAL_TIME:
      time = Rtc.local_time;
      break;
    case DT_UTC:
      time = Rtc.utc_time;
      break;
    case DT_DST:
      time = Rtc.daylight_saving_time;
      break;
    case DT_STD:
      time = Rtc.standard_time;
      break;
    case DT_RESTART:
      if (Rtc.restart_time == 0) {
        return "";
      }
      time = Rtc.restart_time;
      break;
    case DT_BOOTCOUNT:
      time = tkr_set->Settings.bootcount_reset_time;
      break;
  }
  String dt = GetDT(time);  // 2017-03-07T11:08:02

  if (DT_LOCAL_MILLIS == datetime_type) {
    char ms[10];
    snprintf_P(ms, sizeof(ms), PSTR(".%03d"), RtcMillis());
    dt += ms;               // 2017-03-07T11:08:02.123
    datetime_type = DT_LOCAL;
  }

  if (DT_UTC == datetime_type) {
    dt += "Z";              // 2017-03-07T11:08:02.123Z
  }
  else if (tkr_set->Settings.flag3.time_append_timezone && (DT_LOCAL == datetime_type)) {  // SetOption52 - Append timezone to JSON time
    dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  }
  return dt;                // 2017-03-07T11:08:02-07:00 or 2017-03-07T11:08:02.123-07:00
}


uint32_t mTime::UpTime(void) 
{
  if (Rtc.restart_time) {
    return Rtc.utc_time - Rtc.restart_time;
  } else {
    return uptime_seconds_nonreset;
  }
}


uint32_t mTime::MinutesUptime(void) 
{
  return (UpTime() / 60);
}


String mTime::GetUptime(void) 
{
  return GetDuration(UpTime());
}


uint32_t mTime::MinutesPastMidnight(void) 
{
  uint32_t minutes = 0;

  if (RtcTime.valid) {
    minutes = (RtcTime.hour *60) + RtcTime.minute;
  }
  return minutes;
}


uint32_t mTime::RtcMillis(void) 
{
  return (millis() - Rtc.millis) % 1000;
}


void mTime::BreakNanoTime(uint32_t time_input, uint32_t time_nanos, datetime_t &tm) 
{
// break the given time_input into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  time_input += time_nanos / 1000000000U;
  tm.nanos = time_nanos % 1000000000U;

  uint8_t year;
  uint8_t month;
  uint8_t month_length;
  uint32_t time;
  unsigned long days;

  time = time_input;
  tm.second = time % 60;
  time /= 60;                // now it is minutes
  tm.minute = time % 60;
  time /= 60;                // now it is hours
  tm.hour = time % 24;
  time /= 24;                // now it is days
  tm.days = time;
  tm.day_of_week = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.day_of_year = time;

  for (month = 0; month < 12; month++) {
    if (1 == month) { // february
      if (LEAP_YEAR(year)) {
        month_length = 29;
      } else {
        month_length = 28;
      }
    } else {
      month_length = pgm_read_byte(&kDaysInMonth[month]);
    }

    if (time >= month_length) {
      time -= month_length;
    } else {
      break;
    }
  }
  
  #ifdef ESP32
  strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  #else
  strncpy(tm.name_of_month, kMonthNames + (month *3), 3);   // fix for esp8266, as strlcpy causes Exception (3). Should be fixable. 
  #endif 
  
  tm.month = month + 1;      // jan is month 1
  tm.day_of_month = time + 1;         // day of month
  tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
}


void mTime::BreakTime(uint32_t time_input, datetime_t &tm) 
{
  BreakNanoTime(time_input, 0, tm);
}


uint32_t mTime::MakeTime(datetime_t &tm) 
{
  // assemble time elements into datetime_t
  // note year argument is offset from 1970

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < tm.month; i++) {
    if ((2 == i) && LEAP_YEAR(tm.year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * pgm_read_byte(&kDaysInMonth[i-1]);  // monthDay array starts from 0
    }
  }
  seconds+= (tm.day_of_month - 1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.minute * SECS_PER_MIN;
  seconds+= tm.second;
  return seconds;
}


uint32_t mTime::GetUTCTime()
{
  return Rtc.utc_time;
}


uint32_t mTime::RuleToTime( TimeRule r, int yr) 
{
  datetime_t tm;
  uint32_t t;
  uint8_t m;
  uint8_t w;                // temp copies of r.month and r.week

  m = r.month;
  w = r.week;
  if (0 == w) {             // Last week = 0
    if (++m > 12) {         // for "Last", go to the next month
      m = 1;
      yr++;
    }
    w = 1;                  // and treat as first week of next month, subtract 7 days later
  }

  tm.hour = r.hour;
  tm.minute = 0;
  tm.second = 0;
  tm.day_of_month = 1;
  tm.month = m;
  tm.year = yr - 1970;
  t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
  BreakTime(t, tm);
  t += (7 * (w - 1) + (r.dow - tm.day_of_week + 7) % 7) * SECS_PER_DAY;
  if (0 == r.week) {
    t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
  }
  return t;
}


void mTime::RtcGetDaylightSavingTimes(uint32_t local_time) 
{
  datetime_t tmpTime;
  BreakTime(local_time, tmpTime);
  tmpTime.year += 1970;

  /**
   * @brief Temporary fix for the time rules, since loading of settings is erasing these values.
   * 
   */
  tkr_set->Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  tkr_set->Settings.tflag[0].week = TIME_STD_WEEK;
  tkr_set->Settings.tflag[0].dow = TIME_STD_DAY;
  tkr_set->Settings.tflag[0].month = TIME_STD_MONTH;
  tkr_set->Settings.tflag[0].hour = TIME_STD_HOUR;
  tkr_set->Settings.toffset[0] = TIME_STD_OFFSET;

  tkr_set->Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  tkr_set->Settings.tflag[1].week = TIME_DST_WEEK;
  tkr_set->Settings.tflag[1].dow = TIME_DST_DAY;
  tkr_set->Settings.tflag[1].month = TIME_DST_MONTH;
  tkr_set->Settings.tflag[1].hour = TIME_DST_HOUR;
  tkr_set->Settings.toffset[1] = TIME_DST_OFFSET;


  Rtc.daylight_saving_time = RuleToTime(tkr_set->Settings.tflag[1], tmpTime.year);
  Rtc.standard_time = RuleToTime( tkr_set->Settings.tflag[0], tmpTime.year);

  // ALOG_HGL(PSTR("RtcGetDaylightSavingTimes: %s %s"), GetDT(Rtc.daylight_saving_time).c_str(), GetDT(Rtc.standard_time).c_str());

}


uint32_t mTime::RtcTimeZoneOffset(uint32_t local_time) 
{
  
  // ALOG_INF(PSTR(D_LOG_TIME2 "RtcTimeZoneOffset"));
  tkr_set->Settings.toffset[1] = 60;//FORCED TO TEST
  tkr_set->Settings.toffset[0] = 0;//FORCED TO TEST

  int16_t timezone_minutes = tkr_set->Settings.timezone_minutes2;
  if (tkr_set->Settings.timezone2 < 0) { timezone_minutes *= -1; }
  uint32_t timezone = (tkr_set->Settings.timezone2 * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
  // ALOG_INF(PSTR(D_LOG_TIME2 "RtcTimeZoneOffset timezone %d"), timezone);
  
  if (99 == tkr_set->Settings.timezone2) 
  {
    int32_t dstoffset = tkr_set->Settings.toffset[1] * SECS_PER_MIN;
    int32_t stdoffset = tkr_set->Settings.toffset[0] * SECS_PER_MIN;
    if (tkr_set->Settings.tflag[1].hemis) {
      // Southern hemisphere
      if (
          (local_time >= (Rtc.standard_time        - dstoffset)) && 
          (local_time <  (Rtc.daylight_saving_time - stdoffset))
      ){
        timezone = stdoffset;  // Standard Time
      } else {
        timezone = dstoffset;  // Daylight Saving Time
      }
    } else {
      // Northern hemisphere
      if (
          (local_time >= (Rtc.daylight_saving_time - stdoffset)) && 
          (local_time <  (Rtc.standard_time        - dstoffset))
      ){
        timezone = dstoffset;  // Daylight Saving Time
      } else {
        timezone = stdoffset;  // Standard Time
      }
    }
  }

  return timezone;
}


void mTime::RtcSetTimeOfDay(uint32_t local_time) 
{
  // Sync Core/RTOS time to be used by file system time stamps
  struct timeval tv;
  tv.tv_sec = local_time;
  tv.tv_usec = 0;
  settimeofday(&tv, nullptr);
}


void mTime::RtcSecond(void) 
{
  static uint32_t last_sync = 0;
  static bool mutex = false;

  if (mutex) { return; }

  if (Rtc.time_synced) {
    mutex = true;

    Rtc.time_synced = false;
    Rtc.last_synced = true;
    last_sync = Rtc.utc_time;

    if (Rtc.restart_time == 0) {
      Rtc.restart_time = Rtc.utc_time - uptime_seconds_nonreset;  // save first synced time as restart time
    }

    RtcGetDaylightSavingTimes(Rtc.utc_time);

    ALOG_INF(PSTR("RTC: " D_UTC_TIME " %s, " D_DST_TIME " %s, " D_STD_TIME " %s"),
      GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());

    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      tkr_set->Settings.rules_flag.time_init = 1;
    } else {
      tkr_set->Settings.rules_flag.time_set = 1;
    }
  } else {
    if (Rtc.last_synced) {
      Rtc.last_synced = false;
      uint32_t nanos = Rtc.nanos + (millis() - Rtc.millis) * 1000000U;
      Rtc.utc_time += nanos / 1000000000U;
      Rtc.nanos = nanos % 1000000000U;
    } else
      Rtc.utc_time++;  // Increment every second
  }
  Rtc.millis = millis();

  if ((Rtc.utc_time > (2 * 60 * 60)) && (last_sync < Rtc.utc_time - (2 * 60 * 60))) {  // Every two hours a warning
    ALOG_INF(PSTR("RTC: Not synced"));
    last_sync = Rtc.utc_time;
  }

  if (Rtc.utc_time > START_VALID_TIME) {  // 2016-01-01
      Rtc.time_timezone = RtcTimeZoneOffset(Rtc.utc_time);
    Rtc.local_time = Rtc.utc_time + Rtc.time_timezone;
    Rtc.time_timezone /= 60;
    if (tkr_set->Settings.bootcount_reset_time < START_VALID_TIME) {
      tkr_set->Settings.bootcount_reset_time = Rtc.local_time;
    }
  } else {
    Rtc.local_time = Rtc.utc_time;
  }

  BreakNanoTime(Rtc.local_time, Rtc.nanos, RtcTime);
  if (RtcTime.valid) {
    if (!Rtc.midnight) {
      Rtc.midnight = Rtc.local_time - (RtcTime.hour * 3600) - (RtcTime.minute * 60) - RtcTime.second;
    }
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) {
      Rtc.midnight = Rtc.local_time;
      Rtc.midnight_now = true;
    }

    if (mutex) {  // Time is just synced and is valid
      // Sync Core/RTOS time to be used by file system time stamps
      RtcSetTimeOfDay(Rtc.local_time);
    }
  }

  RtcTime.year += 1970;

  mutex = false;
}


void mTime::RtcSync(const char* source) 
{

  Rtc.time_synced = true;
  RtcSecond();
  ALOG_INF(PSTR("RTC: Synced by %s"), source);

  pCONT->Tasker_Interface(TASK_TIME_SYNCED);
  
}


void mTime::RtcSetTime(uint32_t epoch) 
{
  if (epoch < START_VALID_TIME) {  // 2016-01-01
    Rtc.user_time_entry = false;
    ntp_force_sync = true;
  } else {
    Rtc.user_time_entry = true;
    Rtc.utc_time = epoch;
    RtcSync("Time");
  }
}


void mTime::Init(void) 
{

  TickerRtc = new Ticker();

  Rtc.utc_time = 0;
  BreakTime(Rtc.utc_time, RtcTime);
  
  #ifdef ESP32
    TickerRtc->attach_ms(1000, +[](mTime* instance){ instance->RtcSecond(); }, this);
  #else
    TickerRtc->attach   (1,            [this](void){ this->RtcSecond(); });
  #endif

  if (tkr_set->Settings.cfg_timestamp > START_VALID_TIME) {
    // Fix file timestamp while utctime is not synced
    uint32_t utc_time = tkr_set->Settings.cfg_timestamp;
    if (RtcSettings.utc_time > utc_time) {
      utc_time = RtcSettings.utc_time;
    }
    utc_time++;
    RtcGetDaylightSavingTimes(utc_time);
    uint32_t local_time = utc_time + RtcTimeZoneOffset(utc_time);
    RtcSetTimeOfDay(local_time);
   ALOG_INF(PSTR("RTC: Timestamp %s"), GetDT(local_time).c_str());
  }
  
  module_state.mode = ModuleStatus::Running;

}


inline int32_t mTime::TimeDifference(uint32_t prev, uint32_t next)
{
  return ((int32_t) (next - prev));
}


int32_t mTime::TimePassedSince(uint32_t timestamp)
{
  // Compute the number of milliSeconds passed since timestamp given.
  // Note: value can be negative if the timestamp has not yet been reached.
  return TimeDifference(timestamp, millis());
}


bool mTime::TimeReached(uint32_t timer)
{
  // Check if a certain timeout has been reached.
  const long passed = TimePassedSince(timer);
  return (passed >= 0);
}


void mTime::WifiPollNtp() 
{
  static uint8_t ntp_sync_minute = 0;
  static uint32_t ntp_run_time = 0;

  if (tkr_set->runtime.global_state.wifi_down || Rtc.user_time_entry) { return; }

  uint8_t uptime_minute = (uptime_seconds_nonreset / 60) % 60;  // 0 .. 59
  if ((ntp_sync_minute > 59) && (uptime_minute > 2)) {
    ntp_sync_minute = 1;                 // If sync prepare for a new cycle
  }
  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
  uint8_t offset = (uptime_seconds_nonreset < 30) ? RtcTime.second + ntp_run_time : (((mSupportHardware::ESP_getChipId() & 0xF) * 3) + 3) ;

  if ( (((offset == RtcTime.second) && ( (RtcTime.year < 2016) ||                  // Never synced
                                         (ntp_sync_minute == uptime_minute))) ||   // Re-sync every hour
       ntp_force_sync ) ) {                                          // Forced sync

    ntp_force_sync = false;

    ALOG_INF(PSTR("NTP: Sync time..."));

    ntp_run_time = millis();
    uint64_t ntp_nanos = WifiGetNtp();
    uint32_t ntp_time = ntp_nanos / 1000000000;
    ntp_run_time = (millis() - ntp_run_time) / 1000;

    ALOG_INF(PSTR("NTP: Runtime %d"), ntp_run_time);

    if (ntp_run_time < 5) { ntp_run_time = 0; }  // DNS timeout is around 10s
    
    ALOG_HGL(PSTR("NTP: ntp_time %d"), ntp_time);

    if (ntp_time > START_VALID_TIME) 
    {
      Rtc.utc_time = ntp_time;
      Rtc.nanos = ntp_nanos % 1000000000;
      ntp_sync_minute = 60;             // Sync so block further requests
      RtcSync("NTP");
    } 
    else 
    {
      ntp_sync_minute++;                // Try again in next minute
    }
    
  }
}


uint64_t mTime::WifiGetNtp(void) 
{
  static uint8_t ntp_server_id = 0;

 ALOG_INF(PSTR("NTP: Start NTP Sync %d ..."), ntp_server_id);

  IPAddress time_server_ip;

  char fallback_ntp_server[2][32];
  ext_snprintf_P(fallback_ntp_server[0], sizeof(fallback_ntp_server[0]), PSTR("%_I"), tkr_set->Settings.ipv4_address[1]);  // #17984
  ext_snprintf_P(fallback_ntp_server[1], sizeof(fallback_ntp_server[1]), PSTR("%d.pool.ntp.org"), random(0,3));

  char* ntp_server;
  for (uint32_t i = 0; i < MAX_NTP_SERVERS +2; i++) {
    if (ntp_server_id >= MAX_NTP_SERVERS +2) { ntp_server_id = 0; }
    ntp_server = (ntp_server_id < MAX_NTP_SERVERS) ? tkr_set->SettingsText(SET_NTPSERVER1 + ntp_server_id) : fallback_ntp_server[ntp_server_id - MAX_NTP_SERVERS];
    if (strlen(ntp_server)) {
      break;
    }
    ntp_server_id++;
  }

  ALOG_HGL(PSTR("ntp_server %s"), ntp_server);
  
  #ifdef USE_MODULE_NETWORK_MQTT
  if (!pCONT_wif->WifiHostByName(ntp_server, time_server_ip)) {
    ntp_server_id++;
    ALOG_DBG(PSTR("NTP: Unable to resolve '%s'"), ntp_server);
    return 0;
  }
  #else
  return 0; //tmp solution to no networking
  #endif
  
  WiFiUDP udp;

  uint32_t attempts = 3;
  while (attempts > 0) {
    uint32_t port = random(1025, 65535);   // Create a random port for the UDP connection.

    #ifdef USE_IPV6
    if (udp.begin(IPAddress(IPv6), port) != 0)
    #else
    if (udp.begin(port) != 0) 
    #endif
    {
      break;
    }
    attempts--;
  }
  if (0 == attempts) { return 0; }

  while (udp.parsePacket() > 0) {          // Discard any previously received packets
    yield();
  }

  const uint32_t NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
  uint8_t packet_buffer[NTP_PACKET_SIZE];  // Buffer to hold incoming & outgoing packets
  memset(packet_buffer, 0, NTP_PACKET_SIZE);
  packet_buffer[0]  = 0b11100011;          // LI, Version, Mode
  packet_buffer[1]  = 0;                   // Stratum, or type of clock
  packet_buffer[2]  = 6;                   // Polling Interval
  packet_buffer[3]  = 0xEC;                // Peer Clock Precision
  packet_buffer[12] = 49;
  packet_buffer[13] = 0x4E;
  packet_buffer[14] = 49;
  packet_buffer[15] = 52;

  if (udp.beginPacket(time_server_ip, 123) == 0) {  // NTP requests are to port 123
    ntp_server_id++;                                // Next server next time
    udp.stop();
    return 0;
  }
  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();


  uint32_t begin_wait = millis();
  while (!TimeReached(begin_wait + 1000)) {         // Wait up to one second
    uint32_t size        = udp.parsePacket();
    uint32_t remote_port = udp.remotePort();

    if ((size >= NTP_PACKET_SIZE) && (remote_port == 123)) {
      udp.read(packet_buffer, NTP_PACKET_SIZE);     // Read packet into the buffer
      udp.stop();

      if ((packet_buffer[0] & 0b11000000) == 0b11000000) {
        // Leap-Indicator: unknown (clock unsynchronized)
        // See: https://github.com/letscontrolit/ESPEasy/issues/2886#issuecomment-586656384
        ALOG_INF(PSTR("NTP: IP %_I unsynced"), (uint32_t)time_server_ip);
        ntp_server_id++;                            // Next server next time
        return 0;
      }
  Serial.println("packet_buffer");
  for(int i=0;i<48;i++){ Serial.print(packet_buffer[i], HEX); Serial.print(' ');}
  Serial.println("packet_buffer");

      // Convert four bytes starting at location 40 to a long integer (seconds since 1900)
uint32_t secs_since_1900 = ((uint32_t)packet_buffer[40] << 24) |
                           ((uint32_t)packet_buffer[41] << 16) |
                           ((uint32_t)packet_buffer[42] << 8) |
                            (uint32_t)packet_buffer[43];

Serial.print("Seconds since 1900: "); Serial.println(secs_since_1900);

if (secs_since_1900 == 0) {
    // No time stamp received
    ntp_server_id++;
    return 0;
}

// Convert the next four bytes into the fractional part of the timestamp
uint32_t tmp_fraction = ((uint32_t)packet_buffer[44] << 24) |
                        ((uint32_t)packet_buffer[45] << 16) |
                        ((uint32_t)packet_buffer[46] << 8) |
                         (uint32_t)packet_buffer[47];

Serial.print("Fractional part (raw): "); Serial.println(tmp_fraction);

// Convert fractional part from 32-bit fixed point (2^-32) to nanoseconds (1e-9)
uint64_t fraction = (((uint64_t)tmp_fraction) * 1000000000ULL) >> 32;


// Subtract the NTP epoch (1900-01-01) to Unix epoch (1970-01-01)
uint64_t unix_seconds = ((uint64_t)secs_since_1900) - 2208988800ULL;


// Combine the seconds and fractional parts to get the final result in nanoseconds
uint64_t result = (unix_seconds * 1000000000ULL) + fraction;

#ifdef ESP32
Serial.print("Fractional part (nanoseconds): "); Serial.println(fraction);
Serial.print("Unix seconds: "); Serial.println(unix_seconds);
Serial.print("Final NTP result (nanoseconds): "); Serial.println(result);
#endif


      return result;
    }
    delay(10);
  }
  // Timeout.
  ALOG_INF(PSTR("NTP: No reply from %_I"), (uint32_t)time_server_ip);
  udp.stop();
  ntp_server_id++;                                  // Next server next time
  return 0;
}


/**
 * Breaking time to return parts of time
 * */
uint8_t mTime::hour(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.hour;
}


uint8_t mTime::minute(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.minute;
}


uint8_t mTime::second(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.second;
}


int mTime::hourFormat12(time_t time) 
{ // the hour for the given time in 12 hour format
  // refreshCache(t);
  datetime_t time_temp;
  BreakTime(time, time_temp);
  if( time_temp.hour == 0 )
    return 12; // 12 midnight
  else if( time_temp.hour  > 12)
    return time_temp.hour - 12 ;
  else
    return time_temp.hour ;
}

// Time elapsed function that updates the time when true
bool mTime::TimeReached(uint32_t* tSaved, uint32_t ElapsedTime){

  unsigned long long elapsed = millis()-*tSaved;

  if(
    llabs(elapsed)>=ElapsedTime
  ){ *tSaved=millis();
    return true;
  }
  return false;
}


bool mTime::TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime){
  if(
    (ABS_FUNCTION(millis()-tSaved->millis)>=ElapsedTime)
    ||(tSaved->run == true)    
    ){ 
      tSaved->millis=millis();
      tSaved->run = false;
    return true;
  }
  return false;
}


bool mTime::TimeReachedNonReset(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime){
  if(
    (ABS_FUNCTION(millis()-tSaved->millis)>=ElapsedTime)
    ||(tSaved->run == true)    
    ){ 
      //tSaved->millis=millis();
      tSaved->run = false;
    return true;
  }
  return false;
}


// Time elapsed function that updates the time when true
bool mTime::TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime){
  if(ABS_FUNCTION(millis()-*tSaved)>=ElapsedTime){
    return true;
  }
  return false;
}


// Time elapsed function that updates the time when true
uint32_t mTime::MillisElapsed(uint32_t* tSaved){
  return labs(millis()-*tSaved);
}


/**
 * @brief Get the millis from saved millis count to now
 * */
uint32_t mTime::MillisElapsed(uint32_t tSaved)
{
  // uint64_t time = millis()-tSaved;
  if(millis()<=tSaved)
    return 0;
  return labs(millis()-tSaved);
}


// Time elapsed function that updates the time when true
// WARNING: doesnt allow for wrap around
bool mTime::MillisReached(uint32_t* tTarget){
  if(millis()>*tTarget){
    return true;
  }
  return false;
}


#ifdef USE_SUNRISE
/*********************************************************************************************\
 * Sunrise and sunset (+13k code)
 *
 * https://forum.arduino.cc/index.php?topic=218280.0
 * Source: C-Programm von http://lexikon.astronomie.info/equation_of_time/neu.html
 *         Rewrite for Arduino by 'jurs' for German Arduino forum
\*********************************************************************************************/

const float pi2 = TWO_PI;
const float pi = PI;
const float RAD = DEG_TO_RAD;

// Compute the Julian date from the Calendar date, using only unsigned ints for code compactness
// Warning this formula works only from 2000 to 2099, after 2100 we get 1 day off per century.
// In astronomy, a Julian year (symbol: a) is a unit of measurement of time defined as exactly 365.25 days of 86400 SI seconds each
uint32_t mTime::JulianDate(const datetime_t &now) {
  // https://en.wikipedia.org/wiki/Julian_day

  uint32_t Year = now.year;             // Year ex:2020
  uint32_t Month = now.month;            // 1..12
  uint32_t Day = now.day_of_week;     // 1..31
  uint32_t Julian;                          // Julian day number

  if (Month <= 2) {
    Month += 12;
    Year -= 1;
  }
  // Warning, this formula works only for the 20th century, afterwards be are off by 1 day - which does not impact Sunrise much
  // Julian = (1461 * Year + 6884472) / 4 + (153 * Month - 457) / 5 + Day -1 -13;
  Julian = (1461 * Year + 6884416) / 4 + (153 * Month - 457) / 5 + Day;   // -1 -13 included in 6884472 - 14*4 = 6884416
  return Julian;
}

// Force value in the 0..pi2 range
float mTime::InPi(float x)
{
  return pCONT_sup->ModulusRangef(x, 0.0f, pi2);
}

// Time formula
// Tdays is the number of days since Jan 1 2000, and replaces T as the Tropical Century. T = Tdays / 36525.0
float mTime::TimeFormula(float *declination_of_sun, uint32_t Tdays) {
  float RA_Mean = 18.71506921f + (2400.0513369f / 36525.0f) * Tdays;    // we keep only first order value as T is between 0.20 and 0.30
  float M = InPi( (pi2 * 0.993133f) + (pi2 * 99.997361f / 36525.0f) * Tdays);
  float L = InPi( (pi2 * 0.7859453f) + M + (6893.0f * sinf(M) + 72.0f * sinf(M+M) + (6191.2f / 36525.0f) * Tdays) * (pi2 / 1296.0e3f));

  float cos_eps = 0.91750f;     // precompute cos(eps)
  float sin_eps = 0.39773f;     // precompute sin(eps)

  float RA = atanf(tanf(L) * cos_eps);
  if (RA < 0.0f) RA += pi;
  if (L > pi) RA += pi;
  RA = RA * (24.0f/pi2);
  *declination_of_sun = asinf(sin_eps * sinf(L));
  RA_Mean = pCONT_sup->ModulusRangef(RA_Mean, 0.0f, 24.0f);
  float dRA = pCONT_sup->ModulusRangef(RA_Mean - RA, -12.0f, 12.0f);
  dRA = dRA * 1.0027379f;
  return dRA;
}

void mTime::DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down)
{
  const uint32_t JD2000 = 2451545;
  uint32_t JD = JulianDate(RtcTime);
  uint32_t Tdays = JD - JD2000;           // number of days since Jan 1 2000 (n)

  const float h = SUNRISE_DAWN_ANGLE * RAD;
  const float sin_h = sinf(h);    // let GCC pre-compute the sin() at compile time // \phi  is the north latitude of the observer (north is positive, south is negative) on the Earth.

  float lat = tkr_set->Settings.sensors.latitude / (1000000.0f / RAD); // geographische Breite
  float lon = ((float) tkr_set->Settings.sensors.longitude)/1000000;
  
  /**
   * The Earth rotates at an angular velocity of 15°/hour. 
   * Therefore, the expression {\displaystyle \omega _{\circ }\times {\frac {\mathrm {15} ^{\circ }}{hour}}}{\displaystyle \omega _{\circ }\times {\frac {\mathrm {15} ^{\circ }}{hour}}} 
   * gives the interval of time before and after local solar noon that sunrise or sunset will occur.
   * https://en.wikipedia.org/wiki/Sunrise_equation
   * */

  float timezone = ((float)Rtc.time_timezone) / 60;

  float declination_of_sun;
  // https://en.wikipedia.org/wiki/Equation_of_time ie. difference between "calender time" and "true sun position time"
  float equation_of_time = TimeFormula(&declination_of_sun, Tdays);  //declination_of_sun = \delta  is the declination of the sun.
  // This is the equation from above with corrections for atmospherical refraction and solar disc diameter.
  float time_difference = acosf((sin_h - sinf(lat)*sinf(declination_of_sun)) / (cosf(lat)*cosf(declination_of_sun))) * (12.0f / pi); //hour angle //ωo is the hour angle from the observer's zenith;
  
  //local time = midday, downfall = sun transition
  float local_time = 12.0f - time_difference - equation_of_time; //12 hours = transition times (twice a day)
  float downfall_local_time = 12.0f + time_difference - equation_of_time;

  // Sunrise/Sunset transitions which are related by degrees of rotation
  float rise_world_time = local_time - lon / 15.0f;
  float downfall_world_time = downfall_local_time - lon / 15.0f;

  float rise = rise_world_time + timezone + (1/120.0f);         // In hours, with rounding to nearest minute (1/60 * .5)
  rise = pCONT_sup->ModulusRangef(rise, 0.0f, 24.0f);        // force 0 <= x < 24.0
  int rise_hours = (int)rise;
  int rise_minutes = (int)(60.0f * fmodf(rise, 1.0f));

  float downfall = downfall_world_time + timezone;
  downfall = pCONT_sup->ModulusRangef(downfall, 0.0f, 24.0f);
  int downfall_hours = (int)downfall;
  int downfall_minutes = (int)(60.0f * fmodf(downfall, 1.0f));

  // Return transit times
  *hour_up = rise_hours;
  *minute_up = rise_minutes;
  *hour_down = downfall_hours;
  *minute_down = downfall_minutes;
}


char* mTime::GetSunTimeAtHorizon(uint32_t dawn, char* buffer, uint8_t buflen)
{

  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  snprintf_P(buffer, buflen, PSTR("%02d:%02d"), hour[dawn], minute[dawn]);
  return buffer;

}


uint16_t mTime::SunMinutes(uint32_t dawn)
{
  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  return (hour[dawn] *60) + minute[dawn];

}


#endif  // USE_SUNRISE



#ifdef ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
uint32_t mTime::EncodeTimeShortToU32(const time_short_t& time) 
{
  return (time.day_of_week << 24) | (time.hour << 16) | (time.minute << 8) | time.second;
}

time_short_t mTime::DecodeU32ToTimeShort(uint32_t encoded_time) 
{
  time_short_t time;
  time.day_of_week = (encoded_time >> 24) & 0xFF;
  time.hour        = (encoded_time >> 16) & 0xFF;
  time.minute      = (encoded_time >> 8) & 0xFF;
  time.second      = encoded_time & 0xFF;
  return time;
}

const char* mTime::GetFormattedTime(uint32_t encoded_time, char* buffer, uint8_t buflen, bool include_weekday) 
{
  time_short_t time = DecodeU32ToTimeShort(encoded_time);
  if (include_weekday) {
    snprintf_P(buffer, buflen, PSTR("D%d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
                time.day_of_week, time.hour, time.minute, time.second);
  } else {
    snprintf_P(buffer, buflen, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
                time.hour, time.minute, time.second);
  }
  return buffer;
}

time_short_t mTime::MakeTimeShort(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day_of_week) 
{
  time_short_t time;
  time.hour = hour;
  time.minute = minute;
  time.second = second;
  time.day_of_week = day_of_week;
  return time;
}

String mTime::GetTimeStrFromTimeShort(const time_short_t& time, bool include_day_of_week) {
    char dt[20];
    if (include_day_of_week) {
        snprintf_P(dt, sizeof(dt), PSTR("D%dT%02d:%02d:%02d"), 
                   time.day_of_week, time.hour, time.minute, time.second);
    } else {
        snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), 
                   time.hour, time.minute, time.second);
    }
    return String(dt);  // e.g., "D3T11:08:02" or "11:08:02"
}

#endif




#ifdef ENABLE_PHASEOUT_TIME__LEGACY_CODE

uint32_t mTime::GetTimeOfDay_Seconds(void){
  return 0;//RtcTime.Dseconds;
}

const char* mTime::ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds_tod, char* buffer, uint8_t buflen)
{
  
  seconds_tod = seconds_tod % (24 * 3600);
  int hours = seconds_tod / 3600;

  seconds_tod %= 3600;
  int minutes = seconds_tod / 60;

  seconds_tod %= 60;
  int seconds = seconds_tod;

  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              hours, minutes, seconds
  );
  return buffer;

}


time_short_t mTime::GetTimeShortNow(){

  time_short_t now;

  now.day_of_week = RtcTime.day_of_week;
  now.hour = RtcTime.hour;
  now.minute = RtcTime.minute;
  now.second = RtcTime.second;

  return now;

}

uint32_t mTime::GetTimeShortNowU32()
{

  // time_short_t now;

  // now.day_of_week = RtcTime.day_of_week;
  // now.hour = RtcTime.hour;
  // now.minute = RtcTime.minute;
  // now.second = RtcTime.second;

  uint32_t now = (RtcTime.day_of_week<<24) | (RtcTime.hour<<16) | (RtcTime.minute<<8) | (RtcTime.second);
  // uint32_t now2 = reinterpret_cast<uint32_t>(now);

  return now;

}

const char* mTime::ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen)
{
  
  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              time->hour, time->minute, time->second
  );
  return buffer;
}

// const char* mTime::ConvertShortTimetoCtr(time_short_t time, char* buffer, uint8_t buflen)
// {
  
//   snprintf_P(buffer, buflen, 
//               PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
//               time.hour, time.minute, time.second
//   );
//   return buffer;
// }



/**
 * @brief 
 * */
const char* mTime::ConvertU32TimetoCtr(uint32_t* _time, char* buffer, uint8_t buflen, bool flag_include_weekday ) //pointer is not needed!
{
  // typedef struct time_short{
  //   uint8_t day_of_week; // week day [0-7]
  //   uint8_t hour;   // [0-23]
  //   uint8_t minute; // [0-59]
  //   uint8_t second; // [0-59]
  // }time_short_t;

  uint32_t time = *_time;
  uint8_t day_of_week   = (uint8_t)((time & 0xFF000000) >> 24);
  uint8_t hour   = (uint8_t)((time & 0x00FF0000) >> 16);
  uint8_t minute = (uint8_t)((time & 0x0000FF00) >>  8);
  uint8_t second = (uint8_t)((time & 0x000000FF)      );

  // time_short_t* time = reinterpret_cast<time_short_t*>(_time);
  // uint8_t day_of_week = _time
  if(flag_include_weekday)
{


  snprintf_P(buffer, buflen, 
              PSTR("D%d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              day_of_week, hour, minute, second
  );

}else
{

  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              hour, minute, second
  );
}
  return buffer;
}



// const char* mTime::ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen)
// {

//   // time_short_t* time = reinterpret_cast<time_short_t*>(_time);
  
//   snprintf_P(buffer, buflen, 
//               PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
//               time->hour, time->minute, time->second
//   );
//   return buffer;
// }


time_short_t mTime::Parse_Time_TimeShortCtr_To_TimeShort(const char* time_ctr){

  bool includes_week = false;
  time_short_t time_s = {255, 255, 255, 255}; //invalid time of 255 max range

  if(strstr(time_ctr, "D")){
    //wwDHH:MM:SS // 11 bytes
    includes_week = true;
    if(
      (strlen(time_ctr)!=11) && 
      (time_ctr[2]!='D')
    ){
      
    #ifdef ENABLE_LOG_LEVEL_ERROR
      ALOG_TST(PSTR("Invalid time"));
    #endif // ENABLE_LOG_LEVEL_INFO
      return time_s;
    }
  }else{
    //HH:MM:SS // 8 bytes
    if(
      (strlen(time_ctr)!=8)
    ){
      return time_s;
    }
    includes_week = false;
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_TST(PSTR("Parse_Time_TimeShortCtr_To_TimeShort NOT D found"));
    #endif //  ENABLE_LOG_LEVEL_INFO
  }

  if(includes_week){
    time_s.day_of_week   = (uint8_t) strtol( &time_ctr[0], NULL, 10);  //days 1-7 so 0 means none set
    time_s.hour   = (uint8_t) strtol( &time_ctr[3], NULL, 10);
    time_s.minute = (uint8_t) strtol( &time_ctr[6], NULL, 10);
    time_s.second = (uint8_t) strtol( &time_ctr[9], NULL, 10);
  }else{
    time_s.day_of_week   = 0;
    time_s.hour   = (uint8_t) strtol( &time_ctr[0], NULL, 10);
    time_s.minute = (uint8_t) strtol( &time_ctr[3], NULL, 10);
    time_s.second = (uint8_t) strtol( &time_ctr[6], NULL, 10);
  }

  return time_s;

}


uint32_t mTime::ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss){
  return ((hh*3600)+(mm*60)+(ss));
}


uint8_t mTime::CheckBetweenSOD(uint32_t start, uint32_t end){

  if((RtcTime.Dseconds > start)&&(RtcTime.Dseconds < end)){
    return 1;
  }

  return 0;
  //return ((start > RtcTime.SOD)&&(RtcTime.SOD < end)) ? true : false;
}


//PROBABLY breaks on week rollover!! Needs fixed
// New datetime checker for week only (day_of_week,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sow = (start->day_of_week*SEC2DAY)+(start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sow = (end->day_of_week*SEC2DAY)+(end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = start_sow-RtcTime.Wseconds;
  int32_t time_until_end = end_sow-RtcTime.Wseconds;

    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_DBG(PSTR(D_LOG_TIME "CheckBetween_Week_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
    start->hour,start->minute,start->second,time_until_start,
    RtcTime.Dseconds,
    time_until_end,end->hour,end->minute,end->second
  ); 
    #endif// ENABLE_LOG_LEVEL_INFO

  if((start_sow < RtcTime.Wseconds)&&(RtcTime.Wseconds < end_sow)){
    return 1;
  }
  return 0;

}

// New datetime checker for week only (day_of_week,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = RtcTime.Dseconds-start_sod; 
  int32_t time_until_end = end_sod-RtcTime.Dseconds;

  //need to add check if start>end, then add 24 hours

  // #ifdef SERIAL_DEBUG_HIGH_LEVEL

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_DBG(PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
      start->hour,start->minute,start->second,time_until_start,
      RtcTime.Dseconds,
      time_until_end,end->hour,end->minute,end->second
    ); 
    
    ALOG_DBG(PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d<%02d (%02d) | %02d<%02d (%02d)"),
      start_sod,RtcTime.Dseconds,(start_sod < RtcTime.Dseconds?1:0),
      RtcTime.Dseconds,end_sod,(RtcTime.Dseconds < end_sod)?1:0
    );
    #endif// ENABLE_LOG_LEVEL_INFO
    
   if((start_sod < RtcTime.Dseconds)&&(RtcTime.Dseconds < end_sod)){ //now>start AND now<END
     return 1;
   }
  return 0;

}


bool mTime::IsShortTimeWithinRange(time_short start, time_short end)
{

  uint32_t start_sod = (start.hour*SEC2HOUR)+(start.minute*SEC2MIN)+(start.second);
  uint32_t end_sod =   (end.hour*SEC2HOUR)+(end.minute*SEC2MIN)+(end.second);

  uint32_t time_of_day_secs_now = RtcTime.Dseconds;
  int32_t time_until_start = time_of_day_secs_now-start_sod; 
  int32_t time_until_end = end_sod-time_of_day_secs_now;

  bool flag_24hrs_added = false;
  bool flag_time_period_over_midnight = false;
  bool flag_within_time_window = false;

  // if times are equal, return early as false
  if(start_sod == end_sod) return false;

  /**
   * Check if condition has time going over midnight into next day
   * */
  if(end_sod < start_sod)
  {
    flag_time_period_over_midnight = true;
  }
  
  // ALOG_INF(PSTR(D_LOG_TIME "end<start, flag_time_period_over_midnight = %d"), flag_time_period_over_midnight);

  if(flag_time_period_over_midnight)
  {
    /**
     * Check if current time is within current, or next day (after midnight)
     * If tod < end, it is already inside time window
     * */
    if(time_of_day_secs_now < end_sod)
    {
      flag_within_time_window = true;
    }
    /**
     * If not inside next day window, lets check previous day (ie from start to midnight)
     * */
    else
    if(time_of_day_secs_now > start_sod)
    {
      flag_within_time_window = true;
    }
  }
  /**
   * time window spans same day ie start before end, on same day
   * */
  else
  {

    if((start_sod < time_of_day_secs_now)&&(time_of_day_secs_now < end_sod)){ //now>start AND now<END
      flag_within_time_window = true;
    }else{
      flag_within_time_window = false;
    }

  }

  #ifdef ENABLE_LOG_LEVEL_INFO
  // ALOG_INF(PSTR(D_LOG_TIME "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
  //   start.hour,start.minute,start.second,time_until_start,
  //   RtcTime.Dseconds,
  //   time_until_end,end.hour,end.minute,end.second
  // );
  
  ALOG_INF(PSTR(D_LOG_TIME 
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d %d)"
    "\n\r\t%02d:%02d:%02d (%d seconds)"
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d) %s %s"),   
    start.hour,start.minute,start.second, start_sod, time_until_start,
    RtcTime.hour,RtcTime.minute,RtcTime.second, RtcTime.Dseconds, time_of_day_secs_now,
    end.hour,end.minute,end.second, end_sod, time_until_end,
    // flag_24hrs_added?"+24hrs added":"",
    flag_time_period_over_midnight?"Window Across Midnight":"",
    flag_within_time_window?"WITHIN":"OUTSIDE"
  );
  #endif// ENABLE_LOG_LEVEL_INFO

  return flag_within_time_window;




}

// New datetime checker for week only (day_of_week,hours,minutes,seconds)
int8_t mTime::CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  uint32_t time_of_day_secs_now = RtcTime.Dseconds;
  int32_t time_until_start = time_of_day_secs_now-start_sod; 
  int32_t time_until_end = end_sod-time_of_day_secs_now;

  bool flag_24hrs_added = false;
  bool flag_time_period_over_midnight = false;
  bool flag_within_time_window = false;

  //if times are equal, return early as false
  if(start_sod == end_sod) return false;

  /**
   * Check if condition has time going over midnight into next day
   * */
  if(end_sod < start_sod)
  {
    flag_time_period_over_midnight = true;
  }
  
  // ALOG_INF(PSTR(D_LOG_TIME "end<start, flag_time_period_over_midnight = %d"), flag_time_period_over_midnight);

  if(flag_time_period_over_midnight)
  {
    /**
     * Check if current time is within current, or next day (after midnight)
     * If tod < end, it is already inside time window
     * */
    if(time_of_day_secs_now < end_sod)
    {
      flag_within_time_window = true;
    }
    /**
     * If not inside next day window, lets check previous day (ie from start to midnight)
     * */
    else
    if(time_of_day_secs_now > start_sod)
    {
      flag_within_time_window = true;
    }
  }
  /**
   * time window spans same day ie start before end, on same day
   * */
  else
  {

    if((start_sod < time_of_day_secs_now)&&(time_of_day_secs_now < end_sod)){ //now>start AND now<END
      flag_within_time_window = true;
    }else{
      flag_within_time_window = false;
    }

  }


  /**
   * If end of window is less than start of window, assume its the next day, so add 24hrs
   * need to add check if start>end, then add 24 hours
   * */
  // if(end_sod < start_sod){
  //   /**
  //    * TOD now should only be added to "now" if it has went beyond midnight
  //    * eg 9pm to 3am, 9pm check time should be after, but tod_now is only +24 when after 0am
  //    * */
  //   if(time_of_day_secs_now < end_sod){
  //     time_of_day_secs_now += SECS_PER_DAY; // Also need the time_now needs moved forward into next bracket
  //     end_sod += SECS_PER_DAY; // bring start to be the next day by adding 24 hours
  //   }

  //   flag_24hrs_added = true;
  //   ALOG_INF(PSTR(D_LOG_TIME "end<start, Add 24 hours"));
  // }


  #ifdef ENABLE_LOG_LEVEL_INFO
  // ALOG_INF(PSTR(D_LOG_TIME "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
  //   start->hour,start->minute,start->second,time_until_start,
  //   RtcTime.Dseconds,
  //   time_until_end,end->hour,end->minute,end->second
  // );
  
  ALOG_INF(PSTR(D_LOG_TIME 
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d %d)"
    "\n\r\t%02d:%02d:%02d (%d seconds)"
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d) %s %s"),   
    start->hour,start->minute,start->second, start_sod, time_until_start,
    RtcTime.hour,RtcTime.minute,RtcTime.second, RtcTime.Dseconds, time_of_day_secs_now,
    end->hour,end->minute,end->second, end_sod, time_until_end,
    // flag_24hrs_added?"+24hrs added":"",
    flag_time_period_over_midnight?"Window Across Midnight":"",
    flag_within_time_window?"WITHIN":"OUTSIDE"
  );
  #endif// ENABLE_LOG_LEVEL_INFO

  return flag_within_time_window;
  
}



// New datetime checker for week only (day_of_week,hours,minutes,seconds)
uint8_t mTime::CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window){ //window default of 0

  uint16_t dt_sow = (dt->day_of_week*SEC2DAY)+(dt->hour*SEC2HOUR)+(dt->minute*SEC2MIN)+(dt->second);

  if(
    (dt_sow == RtcTime.Wseconds)||
    (((dt_sow-window) > RtcTime.Wseconds))&&(((dt_sow+window) < RtcTime.Wseconds))
    ){
    return 1;
  }
  return 0;

}


// keep method, but add another
uint8_t mTime::CheckBetweenDateTimes(datetime_t* start, datetime_t* end){

  // uint32_t start_soy = (GetDayOfYear(start->day_of_month,start->month)*86400)+(start->hour*3600)+(start->minute*60)+(start->second);
  // uint32_t end_soy = (GetDayOfYear(end->day_of_month,end->month)*86400)+(end->hour*3600)+(end->minute*60)+(end->second);

  //  if((RtcTime.Yseconds > start_soy)&&(RtcTime.Yseconds < end_soy)){
  //    return 1;
  //  }

  return 0;
  //return ((start > RtcTime.SOD)&&(RtcTime.SOD < end)) ? true : false;
}


// now > future
datetime_t mTime::GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2){

  datetime_t datetime_new;
  memset(&datetime_new,0,sizeof(datetime_new));

  //check if new dt1 > dt2 THEN add day
  uint32_t dt1_sow = (dt1->day_of_week*SEC2DAY)+(dt1->hour*SEC2HOUR)+(dt1->minute*SEC2MIN)+(dt1->second);
  uint32_t dt2_sow = (dt2->day_of_week*SEC2DAY)+(dt2->hour*SEC2HOUR)+(dt2->minute*SEC2MIN)+(dt2->second);
  int32_t diff_sow = dt2_sow - dt1_sow;

    // pCONT->mso->MessagePrint("dt1_sow");pCONT->mso->MessagePrintln(dt1_sow);
    //   pCONT->mso->MessagePrint("dt2_sow");pCONT->mso->MessagePrintln(dt2_sow);
    //   pCONT->mso->MessagePrint("Bdiff_sow");pCONT->mso->MessagePrintln(diff_sow);

  if(diff_sow<0){ //tomorrow = future < now //if negative, add day
      //pCONT->mso->MessagePrintln("dt1_sow > dt2_sow");
    diff_sow += SEC2DAY; //add day
  }

  // pCONT->mso->MessagePrint("Adiff_sow");
  // pCONT->mso->MessagePrintln(diff_sow);

  // break new seconds into datetime
  AddSecondsToDateTime(&datetime_new,(uint32_t)diff_sow);

  // datetime_new.day_of_month = abs(dt1->day_of_month-dt2->day_of_month);
  // datetime_new.day_of_year = abs(dt1->day_of_year-dt2->day_of_year);
  // datetime_new.month = abs(dt1->month-dt2->month);
  // datetime_new.year = abs(dt1->year-dt2->year);
  // datetime_new.hour = abs(dt1->hour-dt2->hour);
  // datetime_new.second = abs(dt1->second-dt2->second);
  // datetime_new.minute = abs(dt1->minute-dt2->minute);
  // //datetime_new.week = abs(dt1->week-dt2->week);
  // datetime_new.day_of_week = abs(dt1->day_of_week-dt2->day_of_week);

  // pCONT->mso->MessagePrint("dt1->day_of_week");pCONT->mso->MessagePrintln(dt1->day_of_week);
  // pCONT->mso->MessagePrint("dt2->day_of_week");pCONT->mso->MessagePrintln(dt2->day_of_week);
  // pCONT->mso->MessagePrint("datetime_new.day_of_week");pCONT->mso->MessagePrintln(datetime_new.day_of_week);
  //
  // if(start_sow>end_sow){ //tomorrow
  //   pCONT->mso->MessagePrintln("  if(start_sow>end_sow){ //tomorrow");
  //   AddSecondsToDateTime(&datetime_new,SEC2DAY); //add day
  // }

  return datetime_new;

}


uint8_t mTime::AddSecondsToDateTime(datetime_t* dt_t, uint32_t sec){

  uint8_t years = sec / SEC2YR;
  if (years){
    sec = sec % SEC2YR;
    dt_t->year+=years;
  }

  uint8_t months = sec / SEC2MONTH;
  if (months){
    sec = sec % SEC2MONTH;
    dt_t->month+=months;
  }

  uint8_t weeks = sec / SEC2WEEK;
  if (weeks){
    sec = sec % SEC2WEEK;
    dt_t->week+=weeks;
  }

  uint8_t days = sec / SEC2DAY;
  if (days){
    sec = sec % SEC2DAY;
    dt_t->day_of_week+=days;
  }

  uint8_t hours = sec / SEC2HOUR;
  if (hours){
    sec = sec % SEC2HOUR;
    dt_t->hour+=hours;
  }

  uint8_t minutes = sec / SEC2MIN;
  if (minutes){
    sec = sec % SEC2MIN;
    dt_t->minute+=minutes;
  }

  int seconds = sec; //seconds should now be less than 60 because of minutes
  dt_t->second += seconds;

  return 0;
}


uint32_t mTime::GetSecondsOfDayFromDateTime(datetime_t* dt_t){
  return ((dt_t->hour*SEC2HOUR)+(dt_t->minute*SEC2MIN)+(dt_t->second));
}

#endif // ENABLE_PHASEOUT_TIME__LEGACY_CODE




