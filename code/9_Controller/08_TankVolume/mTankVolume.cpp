#include "mTankVolume.h"

/**
 * @brief 
 * Uses:
 *  - SR04 to detect height of liquid
 *  - DS20B20 as optional temperature adjustment for speed of sound on SR04 (should this in in SR04? as just a temp to set?)
 * 
 * Using SR04 (and temp sensor), the height of liquid in tank is given using basic volume equation (options for cylinder or square tanks)
 * 
 * Look into feeding the SR04 into a new "averaging" class, with limits etc that can be set
 */

#ifdef USE_MODULE_CONTROLLER_TANKVOLUME

int8_t mTankVolume::Tasker(uint8_t function, JsonParserObject obj){

  int8_t function_result = 0;

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
      SubTask_UpdateTankVolume();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

}


void mTankVolume::Init(void)
{
  tank.volume_litres_notusable = TANK_LITRES_BELOW_PIPE;

  tank.height_of_tank_cm = 120.0f;


}

void mTankVolume::Pre_Init()
{

  // if SR04 is enabled 
  settings.fEnableSensor = true;

}



void mTankVolume::EverySecond()
{



}





float mTankVolume::GetDistanceCMReading(void){
  return 0; //return pCONT_ult->ultrasonic.duration*(0.034/2);
}
float mTankVolume::GetDistanceMMReading(void){
  return 0; //return pCONT_ult->ultrasonic.duration*(0.34/2);
}
float mTankVolume::GetOilHeightCMReading(void){
  // float distance_mm= (float)pCONT_ult->ultrasonic.duration*(0.034/2);
  return 0; //return (TANK_HEIGHT_CM-distance_mm);
}
float mTankVolume::GetOilHeightMMReading(void){
  // float distance_mm= (float)pCONT_ult->ultrasonic.duration*(0.34/2);
  return 0; //return (TANK_HEIGHT_MM-distance_mm);
}
float mTankVolume::GetOilHeightMMReadingAdjustedFromTemp(void){
  // float distance_mm= (float)pCONT_ult->GetDistanceMMReadingAdjustedForTemp();
  return 0; //return (TANK_HEIGHT_MM-distance_mm);
}
float mTankVolume::GetOilHeightCMReading(int duration){
  // float distance_mm= (float)duration*(0.034/2);
  return 0; //return (TANK_HEIGHT_CM-distance_mm);
}
float mTankVolume::GetOilHeightMMReading(int duration){
  // float distance_mm= (float)duration*(0.34/2);
  return 0; //return (TANK_HEIGHT_MM-distance_mm);
}



float mTankVolume::GetOilHeightMMReadingAdjustedFromTemp(int distance_from_top){
  return 0; //return (TANK_HEIGHT_MM-distance_from_top);
}
float mTankVolume::GetOilHeightCMReadingAdjustedFromTemp(int distance_from_top){
  return 0; //return (TANK_HEIGHT_CM-distance_from_top);
}





// Will measure the sensors in 3 ways
// 1 exact value (instantaneous)
// X times per tick (fast smoothing)
// X times capturing 1 per tick (slow smoothing over time)
void mTankVolume::SubTask_UltraSonicAverageToOilTank(){
  
  // // instant
  // oiltank.ptr = &oiltank.instant;
  // oiltank.ptr->final.distance_from_bottom_mm = GetOilHeightMMReadingAdjustedFromTemp(pCONT_ult->averaged.instant.final.distance_mm);
  // oiltank.ptr->final.distance_from_bottom_cm = GetOilHeightCMReadingAdjustedFromTemp(pCONT_ult->averaged.instant.final.distance_cm);
  // oiltank.ptr->ischanged = true;
  // oiltank.ptr->isvalid = true;
  // oiltank.ptr->captured.tLastChanged = millis();

  // // 1 minute
  // oiltank.ptr = &oiltank.smooth_1m;
  // oiltank.ptr->final.distance_from_bottom_mm = GetOilHeightMMReadingAdjustedFromTemp(pCONT_ult->averaged.smooth_1m.final.distance_mm);
  // oiltank.ptr->final.distance_from_bottom_cm = GetOilHeightCMReadingAdjustedFromTemp(pCONT_ult->averaged.smooth_1m.final.distance_cm);
  // oiltank.ptr->ischanged = true;
  // oiltank.ptr->isvalid = true;
  // oiltank.ptr->captured.tLastChanged = millis();

  // // 1 hour
  // oiltank.ptr = &oiltank.smooth_1hr;
  // oiltank.ptr->final.distance_from_bottom_mm = GetOilHeightMMReadingAdjustedFromTemp(pCONT_ult->averaged.smooth_1hr.final.distance_mm);
  // oiltank.ptr->final.distance_from_bottom_cm = GetOilHeightCMReadingAdjustedFromTemp(pCONT_ult->averaged.smooth_1hr.final.distance_cm);
  // oiltank.ptr->ischanged = true;
  // oiltank.ptr->isvalid = true;
  // oiltank.ptr->captured.tLastChanged = millis();

}


void mTankVolume::SubTask_CopyAveragedSensorValues(){

  // oiltank.instant.ultrasonic_readings.distance_cm = pCONT_ult->averaged.instant.final.distance_cm;
  // oiltank.smooth_1m.ultrasonic_readings.distance_cm = pCONT_ult->averaged.smooth_1m.final.distance_cm;
  // oiltank.smooth_1hr.ultrasonic_readings.distance_cm = pCONT_ult->averaged.smooth_1hr.final.distance_cm;

}


void mTankVolume::SubTask_UpdateTankVolume()
{

  float distance_from_sensor_cm = pCONT_sr04->readings.average_EMA.distance_cm;
  float height_cm = tank.height_of_tank_cm - distance_from_sensor_cm;


  // float height_mm = pCONT_sr04->readings.average_EMA.distance_cm/1000.0f;
  float volume_cm3 = GetHeightToVolume_Custom_MyTank(height_cm);

  tank.volume_litres = volume_cm3;
  

  mqtthandler_litres_ifchanged.flags.SendNow = true;

} // end of

float mTankVolume::GetHeightToVolume_Custom_MyTank(float height_cm)
{
  
  float volume_cm3 = 0;
  // float height_mm = pCONT_sr04->readings.average_EMA.distance_cm/10.0f;

  // // Above tank radius split
  if(height_cm == TANK_MAINBOTTOM_THRESHOLD_HEIGHT_CM)
  {
    // Need to account for smaller tank diameter on bottom AND nozzle height
    volume_cm3 = TANK_MAIN_RADIUSSQUARED_TIMES_PI_CM * (height_cm-TANK_MAINBOTTOM_THRESHOLD_HEIGHT_CM);
    // Bottom section ADD on
    volume_cm3 += TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_CM * TANK_MAINBOTTOM_THRESHOLD_HEIGHT_CM;
  }
  else
  {
    // Can just use height, since its below 170 anyway
    volume_cm3 = TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_CM * height_cm;
    // oiltank.ptr->final.volume_of_oil_mm3 = TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_MM * (height_mm);
  }

  // 1000 cm3 or cc, is 1 Litre
  float litres_in_tank = volume_cm3/1000;
  // oiltank.ptr->final.millilitres_in_tank = oiltank.ptr->final.volume_of_oil_mm3/1000;

  if(tank.volume_litres_notusable>0)
  {
    // tank.volume_litres_usable = litres_in_tank - TANK_LITRES_BELOW_PIPE;
    tank.volume_litres_usable = litres_in_tank - tank.volume_litres_notusable;

  }


  return litres_in_tank;

}


void mTankVolume::SubTask_RecordLitresOverDays(void){

  // // Record litre level at midnight, everyday, and save to eeprom

  // // Things to save
  // // 365 days
  // // Last saved index 

  // // save values
  // if(abs(millis()-tRecordLitresOverDays)>=5000){tRecordLitresOverDays=millis();

  //   for(int i=0;i<5;i++){
  //     //EEPROM.put(EEPROM_GET_DAYS_ADDRESS(i), i*100); //add,int
  //   }

  // }

  // // Record litres today
  //  if(abs(millis()-tCheckTime)>=10000){ tCheckTime=millis(); // only check for time larger than checkbetween time

  //     // Check to within 5 seconds
  //     if(tkr_time->CheckBetweenSOD(tkr_time->ConvertHHMMSStoSOD(14,0,0),tkr_time->ConvertHHMMSStoSOD(14,0,11))){ // MIDNIGHT
  //     //  for(int i=0;i<5;i++){pCONT->mso->MessagePrint("\t INSIDE TIME");pCONT->mso->MessagePrintln(tkr_time->RtcTime.Wday);}
  //       if(oiltank.smooth_1m.isvalid){
  //       EEPROM.put(EEPROM_GET_DAYS_ADDRESS(tkr_time->RtcTime.Wday),(int)oiltank.smooth_1m.final.litres_in_tank); //add,int
  //       EEPROM.commit();
  //       }
  //     }else{
  //     //  pCONT->mso->MessagePrint("\t OUTSIDE TIME");
  //     }

  // }


  // if(abs(millis()-tReadStoredLitres)>5000){tReadStoredLitres=millis();

  //   int val;

  //     for(int i=0;i<7;i++){
  //        EEPROM.get(EEPROM_GET_DAYS_ADDRESS(i),val);
  //        history.perweekdays[i].litres_in_tank = val;
  //     }

  //    //
  //    // EEPROM.get(0,val);
  //    // history.perweekdays[0].litres_in_tank = val;

  //     history.perweekdays[0].litres_change = history.perweekdays[0].litres_in_tank-history.perweekdays[6].litres_in_tank;
  //     history.perweekdays[1].litres_change = history.perweekdays[1].litres_in_tank-history.perweekdays[0].litres_in_tank;
  //     history.perweekdays[2].litres_change = history.perweekdays[2].litres_in_tank-history.perweekdays[1].litres_in_tank;
  //     history.perweekdays[3].litres_change = history.perweekdays[3].litres_in_tank-history.perweekdays[2].litres_in_tank;
  //     history.perweekdays[4].litres_change = history.perweekdays[4].litres_in_tank-history.perweekdays[3].litres_in_tank;
  //     history.perweekdays[5].litres_change = history.perweekdays[5].litres_in_tank-history.perweekdays[4].litres_in_tank;
  //     history.perweekdays[6].litres_change = history.perweekdays[6].litres_in_tank-history.perweekdays[5].litres_in_tank;

  //     uint8_t today = tkr_time->RtcTime.Wday;
  //     uint8_t yday;
  //     if(tkr_time->RtcTime.Wday==0){yday=6;}else{yday=tkr_time->RtcTime.Wday-1;}
  //     uint8_t tomorrow = tkr_time->RtcTime.Wday+1;
  //     if(tomorrow>6){tomorrow=0;}

  //     history.overview.litres_change = history.perweekdays[tomorrow].litres_in_tank-history.perweekdays[today].litres_in_tank;
  //     history.litres_per_day = history.overview.litres_change/7;
  //     history.days_remaining = oiltank.smooth_1m.final.litres_of_usable_oil/history.litres_per_day;

  // }

}


#endif