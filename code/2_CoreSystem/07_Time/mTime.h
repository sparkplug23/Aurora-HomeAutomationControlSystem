#ifndef MODULE_CORE_TIME_H
#define MODULE_CORE_TIME_H
  
#define D_UNIQUE_MODULE_CORE_TIME_ID  2007 // [(Folder_Number*100)+ID_File]

#include <Arduino.h>
#include <Ticker.h>

enum WeekInMonthOptions {Last, First, Second, Third, Fourth};
enum DayOfTheWeekOptions {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum MonthNamesOptions {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum HemisphereOptions {North, South};
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_DST, DT_STD, DT_RESTART, DT_ENERGY, DT_BOOTCOUNT, DT_LOCAL_MILLIS, DT_TIMEZONE, DT_SUNRISE, DT_SUNSET, DT_LOCAL_TIME };

typedef struct datetime{
  uint32_t nanos;
  uint8_t  second; // [0-59]
  uint8_t  minute; // [0-59]
  uint8_t  hour;   // [0-23]
  uint8_t  day_of_week; // week day [0-7]
  uint8_t  day_of_month; // month day [0-30]
  uint8_t  month;  // [1-12]
  char     name_of_month[4];
  uint8_t  week;   // [1-52]
  uint16_t year;  // year 20XX
  uint16_t day_of_year; // year day [0-365]
  uint32_t Dseconds; //seconds into day from midnight [0-86400]
  uint32_t Wseconds; // seconds into week [0-604800]
  uint32_t Yseconds; // seconds into year [0-31536000]
  uint32_t seconds_nonreset; // seconds counting infinitely [0-1e+32]  
  uint32_t days;
  uint32_t valid;
}datetime_t;


typedef struct time_short{ // used for basic time info only for week period - RENAME "datetime_t"
  uint8_t day_of_week; // week day [0-7], weekday 1-7 is Mon to Sun, 0 is all days
  uint8_t hour;   // [0-23]
  uint8_t minute; // [0-59]
  uint8_t second; // [0-59]
}time_short_t;


typedef struct weektime{ // used for basic time info only for week period - RENAME "datetime_t"
  uint8_t  day_of_week; // week day [0-7]
  uint8_t  hour;   // [0-23]
  uint8_t  minute; // [0-59]
  uint8_t  second; // [0-59]
  uint16_t millis; // millis into 1 second [0-1000]
}weektime_t;


typedef struct TIMEREACHED_HANDLER{
  uint32_t millis = 0;
  uint8_t run = false;
  void trigger()
  {
    run = true;
  }
  // void reset()
  // {
  //   millis = millis();
  //   run = false;
  // }
  void trigger_with_backoff_ms(uint32_t backoff_ms)
  {
    millis = backoff_ms;
  }
}timereached_t;


#define SEC_IN_CENT  3153600000
#define SEC_IN_DEC   315360000
#define SEC_IN_YR    31536000
#define SEC_IN_MONTH 2592000
#define SEC_IN_WEEK  604800
#define SEC_IN_DAY   86400
#define SEC_IN_HOUR  3600
#define SEC_IN_MIN   60


enum TIME_UNITS_IDS{
  TIME_UNIT_SECONDS_ID = 0,
  TIME_UNIT_MILLISECONDS_ID
};


template <typename TIME, typename UNIT>
uint32_t ConvertTimeToMilliSecondsWithUnit(TIME time_secs, UNIT unit){
  if(unit == TIME_UNIT_SECONDS_ID){
    time_secs *= 1000;
  }
  return time_secs;
}


#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/01_Settings/mSettings.h"

#include "1_TaskerManager/mTaskerInterface.h"

#include "3_Network/10_MQTT/mMQTT.h"

static const uint8_t kDaysInMonth[]    PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";

class mTime :
  public mTaskerInterface
{

  private:
  public:
  
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mTime(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_CORE_TIME_CTR = D_MODULE_CORE_TIME_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_TIME_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TIME_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = 0;    // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct DATA_STORAGE
    {

    }
    dt;

    /*********************************************************************************************\
     * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
     *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
    \*********************************************************************************************/

    const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

    const uint32_t SECS_PER_MIN = 60UL;
    const uint32_t SECS_PER_HOUR = 3600UL;
    const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
    const uint32_t MINS_PER_HOUR = 60UL;

    #define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

    struct RTC {
      uint32_t utc_time = 0;
      uint32_t local_time = 0;
      uint32_t daylight_saving_time = 0;
      uint32_t standard_time = 0;
      uint32_t midnight = 0;
      uint32_t restart_time = 0;
      uint32_t nanos = 0;
      uint32_t millis = 0;
      int32_t time_timezone = 0;
      bool time_synced = false;
      bool last_synced = false;
      bool midnight_now = false;
      bool user_time_entry = false;               // Override NTP by user setting
    } Rtc;

    Ticker* TickerRtc = nullptr;
    datetime_t RtcTime;            
    uint32_t uptime_seconds_nonreset = 0; // Counting every second until 4294967295 = 130 year
    bool ntp_force_sync;                      // Force NTP sync
    uint16_t lastday_run = 0; // Used to keep the last midnight time, this stops multiple midnight runs at the same midnight
    

    uint32_t UtcTime(void);
    uint32_t LocalTime(void);
    uint32_t Midnight(void);
    bool MidnightNow(void);
    bool IsDst(void);
    String GetBuildDateAndTime(void);
    String GetSyslogDate(char* mxtime);
    String GetDate(void);
    String GetMinuteTime(uint32_t minutes);
    String GetTimeZone(void);
    String GetDuration(uint32_t time);
    String GetDT(uint32_t time);
    String GetDateAndTime(uint8_t time_type);
    String GetTime(uint8_t time_type = DT_LOCAL);
    uint32_t UpTime(void);
    uint32_t MinutesUptime(void);
    String GetUptime(void);
    uint32_t MinutesPastMidnight(void);
    uint32_t RtcMillis(void);
    void BreakNanoTime(uint32_t time_input, uint32_t time_nanos, datetime_t &tm);
    void BreakTime(uint32_t time_input, datetime_t &tm);
    uint32_t MakeTime(datetime_t &tm);
    uint32_t RuleToTime(TimeRule r, int yr);
    void RtcGetDaylightSavingTimes(uint32_t local_time);
    uint32_t RtcTimeZoneOffset(uint32_t local_time);
    void RtcSetTimeOfDay(uint32_t local_time);
    void RtcSecond(void);
    void RtcSync(const char* source);
    void RtcSetTime(uint32_t epoch);
    void WifiPollNtp();
    uint64_t WifiGetNtp(void);
    inline int32_t TimeDifference(uint32_t prev, uint32_t next);
    int32_t TimePassedSince(uint32_t timestamp);
    bool TimeReached(uint32_t timer);
    static bool TimeReached(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
    static bool TimeReached(TIMEREACHED_HANDLER* tSaved);
    static bool MillisReached(uint32_t* tTarget);
    static bool TimeReachedNonReset(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
    static uint32_t MillisElapsed(uint32_t* tSaved);
    static uint32_t MillisElapsed(uint32_t tSaved);

    String GetTimeStr(uint32_t time, bool include_day_of_week = false);
    String formatTimeUntil(double time_until_seconds);
    
    time_t GetStartOfDayUTC(time_t utc_time);


    #ifdef ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
    static uint32_t EncodeTimeShortToU32(const time_short_t& time);
    static time_short_t DecodeU32ToTimeShort(uint32_t encoded_time);
    static const char* GetFormattedTime(uint32_t encoded_time, char* buffer, uint8_t buflen, bool include_weekday);
    static time_short_t MakeTimeShort(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day_of_week = 0);
    static String GetTimeStrFromTimeShort(const time_short_t& time, bool include_day_of_week = false); 
    #endif




    uint8_t hour(uint32_t time);
    uint8_t minute(uint32_t time);
    uint8_t second(uint32_t time);
    int hourFormat12(time_t t);
    uint32_t GetUTCTime();

    #ifdef USE_SUNRISE
      uint32_t JulianDate(const datetime_t &now);      
      // Force value in the 0..pi2 range
      float InPi(float x);
      float TimeFormula(float *DK, uint32_t Tdays) ;
      void DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down);
      // void ApplyTimerOffsets(Timer *duskdawn);
      char* GetSunTimeAtHorizon(uint32_t dawn, char* buffer, uint8_t buflen);
      uint16_t SunMinutes(uint32_t dawn);
    #endif //  USE_SUNRISE

    /************************************************************************************************
     * SECTION: Legacy code
     ************************************************************************************************/
    #ifdef ENABLE_PHASEOUT_TIME__LEGACY_CODE
    
    uint32_t GetTimeOfDay_Seconds(void);
    static const char* ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds, char* buffer, uint8_t buflen);
    static const char* ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen);
    static const char* ConvertU32TimetoCtr(uint32_t* _time, char* buffer, uint8_t buflen, bool flag_include_weekday = false);
    static time_short_t Parse_Time_TimeShortCtr_To_TimeShort(const char* time_ctr);    
    
    time_short_t GetTimeShortNow();
    uint32_t GetTimeShortNowU32();

    uint32_t ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss);
    uint8_t CheckBetweenSOD(uint32_t start, uint32_t end);

    datetime_t GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2);

    uint8_t CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end);
    uint8_t CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end);
    int8_t  CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end);

    datetime_t GetTimefromCtr(const char* c);

    uint16_t DaysInThisMonth();
    uint8_t CheckBetweenDateTimes(datetime_t* start, datetime_t* end);
    bool IsShortTimeWithinRange(time_short start, time_short end);
    uint8_t AddSecondsToDateTime(datetime_t* start, uint32_t seconds);
    uint8_t CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window = 0);
    uint32_t GetSecondsOfDayFromDateTime(datetime_t* dt_t);
    
    #endif // ENABLE_PHASEOUT_TIME__LEGACY_CODE

};

// #endif // USE_MODULE_CORE_TIME

#endif // header
