#include "mUltraSonicSensor.h"

#ifdef USE_MODULE_SENSORS_ULTRASONICS

void mUltraSonicSensor::Pre_Init()
{ 

  if(
    pCONT_pins->PinUsed(GPIO_SR04_TRIG_ID) && 
    pCONT_pins->PinUsed(GPIO_SR04_ECHO_ID)
  ){
    pin_trig = pCONT_pins->GetPin(GPIO_SR04_TRIG_ID);
    pin_echo = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);
    pinMode(pin_trig, OUTPUT); // Sets the trigPin as an Output
    pinMode(pin_echo, INPUT);  // Sets the echoPin as an Input
    settings.flags.EnableSensor = true;
  }else{
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_ULTRASONIC "Pin Invalid %d"),pin_trig);
    settings.flags.EnableSensor = false;
  }

}


void mUltraSonicSensor::Init(void)
{

  memset(&averaged,0,sizeof(averaged));

  ultrasonic.threshold.narrowpercent = 2; // 5% percent of norminal
  ultrasonic.threshold.widepercent = 50; // 5% percent of norminal

  object_detected_static.tSavedCheck = millis();

  sprintf(motion_detect.detected_rtc_ctr,"--:--:--\0");
          
}


int mUltraSonicSensor::GetDurationReading(void)
{

  // If sensor is invalid, use shorter delay to read again (but not too fast)
  
  #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
  int blockingtime = ultrasonic.settings.blocking_time_ms;
  if(!ultrasonic.isvalid){
    blockingtime = ultrasonic.settings.blocking_time_ms*4; //retry 2 times longer is previous sample was wrong
  }

  // Return early
  if((abs(millis()-ultrasonic.tUltraSonicSensorReadLast)<blockingtime)){ // if not valid try again right away
   ALOG_INF(PSTR(D_LOG_ULTRASONIC "ultrasonic.tUltraSonicSensorReadLast<blockingtime %d"),blockingtime);
    return ultrasonic.duration;
  }
  #endif //   #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  // ALOG_DBG(PSTR(D_LOG_ULTRASONIC "mUltraSonicSensor::GetDurationReading"));

  float duration=0;

  // uint16_t maxDistanceCm = 1000 ; //10m
  // float speedOfSoundInCmPerMicroSec= 343*100*100000; // m/s approx

  // // Compute max delay based on max distance with 25% margin in microseconds
  //  float maxDistanceDurationMicroSec = 2.5 * maxDistanceCm / speedOfSoundInCmPerMicroSec;

  // Clears the trigPin
  digitalWrite(pin_trig, LOW);
  delayMicroseconds(2);
  // Sets the ULTRA_TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(pin_echo, HIGH, 10000);//, 4500);//, ultrasonic.settings.duration_limit_max); //10000us 10ms //default 1 second timeout

  Serial.println(duration);

  //duration is microseconds
  // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(distance);

  // ALOG_INF(PSTR(D_LOG_ULTRASONIC "duration=%d, \tDistance=%d, maxDistanceDurationMicroSec=%d"),(int)duration, distance, (int)maxDistanceDurationMicroSec);
  ALOG_INF(PSTR(D_LOG_ULTRASONIC "duration=%d, \tDistance=%d"),(int)duration, distance);


  ultrasonic.duration_raw = duration;

  // CHANGE TO USE INTERRUPT BASED METHOD, IE trigger (turn on interrupt) and have it compare start millis and triggered millis (is pulse nano or millis?)

  // ALOG_INF(PSTR(D_LOG_ULTRASONIC "duration=%d %dnew"),(int)duration, (int)(duration/58));

  #ifdef ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
  //if outside possible range
  if((duration>ultrasonic.settings.duration_limit_min)&&(duration<ultrasonic.settings.duration_limit_max)){ 
    
    //pCONT->mso->MessagePrintln("[ULTRA] SAMPLING");
    
    ALOG_INF(PSTR(D_LOG_ULTRASONIC "INSIDE DURATION"));

    // float lower = (float)ultrasonic.duration*(1-(ultrasonic.threshold.narrowpercent/100.0));//0.9;
    // float upper = (float)ultrasonic.duration*(1+(ultrasonic.threshold.narrowpercent/100.0));
    // ultrasonic.threshold.lowervalue = lower;
    // ultrasonic.threshold.uppervalue = upper;

    // // ALOG_DBM( PSTR(D_LOG_ULTRASONIC "Lower<duration|Duration<Upper: [\t%d\t%d\t%d\t%d]"),(int)lower,(int)ultrasonic.duration,(int)duration,(int)upper);

    // // New method under test
    // // GET how new duration relates to previous
    // if(duration>ultrasonic.duration){ // Positive range
    //   ultrasonic.threshold.ratio_pos = duration/ultrasonic.threshold.uppervalue;
    //   ultrasonic.threshold.relative = ultrasonic.threshold.ratio_pos;  
    //   // ALOG_DBM( PSTR(D_LOG_ULTRASONIC "duration > ultrasonic.duration"));
    // }else{ // Negative range
    //   ultrasonic.threshold.ratio_neg = -1*(ultrasonic.threshold.lowervalue/duration);
    //   ultrasonic.threshold.relative = ultrasonic.threshold.ratio_neg; 
    //   // ALOG_DBM( PSTR(D_LOG_ULTRASONIC "duration < ultrasonic.duration"));
    // }

    // // Check if its within threshold
    // if(fabsf(ultrasonic.threshold.relative)>1){ // outside range
    //   ultrasonic.threshold.outsidecount++;
    //   ultrasonic.threshold.insidecount=0;
    // }else 
    // if(fabsf(ultrasonic.threshold.relative)<=1){ // inside range
    //   ultrasonic.threshold.insidecount++;
    //   ultrasonic.threshold.outsidecount=0;
    // }

    // // Adjust threshold limits if need be
    // if(ultrasonic.threshold.outsidecount>10){
    //   // Reduce set threshold if we can
    //   if(ultrasonic.threshold.setpercent < ultrasonic.threshold.widepercent){
    //     ultrasonic.threshold.setpercent+=5; // +10% // increase width faster to resolve errors
    //   }else{
    //     ultrasonic.threshold.setpercent = ultrasonic.threshold.widepercent;
    //   }
    //   //ultrasonic.threshold.outsidecount = 10; // freeze it at limit
    // }
    // if(ultrasonic.threshold.insidecount>10){
    //   // Reduce set threshold if we can
    //   if(ultrasonic.threshold.setpercent > ultrasonic.threshold.narrowpercent){
    //     ultrasonic.threshold.setpercent--; //-1% train in slowly
    //   }else{
    //     ultrasonic.threshold.setpercent = ultrasonic.threshold.narrowpercent;
    //   }
    //   //ultrasonic.threshold.insidecount = 10
    // }

    // //FORCED LIMITS
    // if((ultrasonic.threshold.setpercent>ultrasonic.threshold.widepercent)||(ultrasonic.threshold.setpercent<ultrasonic.threshold.narrowpercent)){ultrasonic.threshold.setpercent=20;}
    // if(ultrasonic.threshold.insidecount>200){ultrasonic.threshold.insidecount=200;}
    // if(ultrasonic.threshold.outsidecount>200){ultrasonic.threshold.outsidecount=200;}

    // ALOG_DBM( PSTR(D_LOG_ULTRASONIC "fabsf(ultrasonic.threshold.relative) %f"),fabsf(ultrasonic.threshold.relative));

    // if(fabsf(ultrasonic.threshold.relative)<=1){ 
    //   ALOG_DBM( PSTR(D_LOG_ULTRASONIC "[ULTRA]if(abs(ultrasonic.threshold.relative)<=1) \t\t"));
      ultrasonic.isvalid = true;
    //   ultrasonic.ischanged = true;
    //   ultrasonic.accuracy.insidecount++;
    // }else{
    //   ALOG_DBM( PSTR(D_LOG_ULTRASONIC "[ULTRA] NOT if(abs(ultrasonic.threshold.relative)<=1)"));
    //   ultrasonic.isvalid = false;
    //   ultrasonic.ischanged = false;
    //   ultrasonic.accuracy.outsidecount++;
    // }

    ultrasonic.duration = duration; //update stored
    ultrasonic.tUltraSonicSensorReadLast = millis();

    // float total = ultrasonic.accuracy.insidecount+ultrasonic.accuracy.outsidecount;
    // if(total!=0){ // bottom of fraction reduces to only inside
    //   ultrasonic.accuracy.percent = (ultrasonic.accuracy.insidecount/total)*100;
    // }

    return ultrasonic.duration;
  }else{ // ==0 no response

  
  ALOG_INF(PSTR(D_LOG_ULTRASONIC "OUTSIDE DURATION %d"),duration);
  
    //ALOG_INF(PSTR(D_LOG_ULTRASONIC "[ULTRA] Outside viable range"));
    //pCONT->mso->MessagePrintln("[ULTRA] Outside viable range");
    ultrasonic.isvalid = false;
    ultrasonic.ischanged = false;
    ultrasonic.accuracy.outsidecount++;

    
// //debug testing
//     ultrasonic.isvalid = true;
//     ultrasonic.ischanged = true;


    return ultrasonic.duration;
  }

  #else
  
    ultrasonic.duration = duration; //update stored
    ultrasonic.tUltraSonicSensorReadLast = millis();
    ultrasonic.isvalid = true;
    ultrasonic.ischanged = true;
    ultrasonic.accuracy.insidecount++;

  #endif // ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

}

// float mUltraSonicSensor::GetDistanceMetresReading(void){
//   return ultrasonic.duration*(0.034/2);
// }
float mUltraSonicSensor::GetDistanceCMReading(void){
  return ultrasonic.duration*(0.034/2);
}
float mUltraSonicSensor::GetDistanceMMReading(void){
  return ultrasonic.duration*(0.34/2);
}


// Instead of using 343m/s for speed of sound at 20 degrees celcius, if valid,
// use the tank temperature sensor, to get speed of sound, else return 343
float mUltraSonicSensor::GetSpeedOfSoundInMetres()
{

  float speedofsound_inmps = 0;

  #ifdef USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
      #ifdef USE_MODULE_SENSORS_DS18X
      int tempsensorid = -1;
      float ambient_temperature;
      if((tempsensorid=tkr_set->GetDeviceIDbyName("SpeedOfSound_Ambient",0,(int8_t) /*EM_MODULE_SENSORS_DB18S20_ID*/ pCONT_db18->GetModuleUniqueID()))>=0){
          if(pCONT_db18->sensor[tempsensorid].reading.isvalid){
          ambient_temperature = pCONT_db18->sensor[tempsensorid].reading.val;
          
        AddLog(LOG_LEVEL_ERROR, PSTR("ambient_temperature=%d"),(int)ambient_temperature);
          // Reduce frequency of updates to stop data jumps
          if(abs(millis()-ultrasonic.tPermitTempUpdate)>60000){ultrasonic.tPermitTempUpdate = millis();
              ultrasonic.temperature = ambient_temperature;
          }
          // float insidesqrt = (ultrasonic.temperature/273.15)+1;
          // speedofsound_inmps = 331.227599 * sqrt(insidesqrt);
          speedofsound_inmps = 331.3 + (0.606 * ultrasonic.temperature); //https://en.wikipedia.org/wiki/Speed_of_sound
          }
      }else{
        AddLog(LOG_LEVEL_ERROR, PSTR("SOS missing"));
      }
      #endif
  #endif

  // Use default
  if(speedofsound_inmps==0){
    speedofsound_inmps = 343;
    #ifdef USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_ULTRASONIC "speedofsound_inmps = 343"));
    #endif
  }

  ultrasonic.speedofsound = speedofsound_inmps;
  
  ALOG_DBM( PSTR("speedofsound=%d"),(int)ultrasonic.speedofsound);

  return speedofsound_inmps;

}

void mUltraSonicSensor::AddMOTIONEventStatusSum(uint8_t status)
{
  motion_event_status_sum.arr[motion_event_status_sum.idx++] = status;
  if(motion_event_status_sum.idx > MOTION_EVENT_STATUS_SUM_MAX){motion_event_status_sum.idx=0;}
}
uint8_t mUltraSonicSensor::GetMOTIONEventStatusSum()
{
  uint8_t sum = 0;
  for(uint8_t ii=0;ii<MOTION_EVENT_STATUS_SUM_MAX;ii++){
    sum += motion_event_status_sum.arr[ii]; 
  }
  if((sum == MOTION_EVENT_STATUS_SUM_MAX) || (sum == 0)){
    return 1; // return true if all are the same ie all events are the same
  }
  return 0;
}





float mUltraSonicSensor::GetDistanceMMReadingAdjustedForTemp(void)
{
  //The speed of sound is: 343m/s = 0.0343 cm/uS = 1/29.1 cm/uS
  float speedofsound_inmetres = GetSpeedOfSoundInMetres();//@20 degrees cerr    0.34;

  //float speedofsound_inmillimetres = (speedofsound_inmetres*1000)/1000000; *1000 from m->mm, divide by 1E6 for Us
  float speedofsound_inmillimetres_permicroseconds = (speedofsound_inmetres)/1000;

  float distance_mm= (float)ultrasonic.duration*(speedofsound_inmillimetres_permicroseconds/2);
  return distance_mm;
}


float mUltraSonicSensor::GetDistanceMMReadingAdjustedForTemp(int duration)
{

  //The speed of sound is: 343m/s = 0.0343 cm/uS = 1/29.1 cm/uS
  float speedofsound_inmetres = GetSpeedOfSoundInMetres();//@20 degrees cerr    0.34;

  //stored_speedofsound_inmetres = speedofsound_inmetres;
  //Serial.print("[ULTRA] speedofsound_inmetres>> "); Serial.println(speedofsound_inmetres);
  //float speedofsound_inmillimetres = (speedofsound_inmetres*1000)/1000000; *1000 from m->mm, divide by 1E6 for Us
  float speedofsound_inmillimetres_permicroseconds = (speedofsound_inmetres)/1000;

  float newtest = (duration/2)*0.034;
  ALOG_TST(PSTR("newtest=%d"),(int)newtest);

  //Serial.print("[ULTRA] speedofsound_inmillimetres_permicroseconds>> "); Serial.println(speedofsound_inmillimetres_permicroseconds);
  //float sos = GetSpeedOfSoundInMetres();

  float distance_mm= (float)duration*(speedofsound_inmillimetres_permicroseconds/2);
  return distance_mm;
}


float mUltraSonicSensor::GetDistanceCMReadingAdjustedForTemp(void)
{
  return GetDistanceMMReadingAdjustedForTemp()/10;
}


// Will measure the sensors in 3 ways
// 1 exact value (instantaneous)
// X times per tick (fast smoothing)
// X times capturing 1 per tick (slow smoothing over time)
void mUltraSonicSensor::SubTask_UltraSonicAverage()
{

  // ALOG_TST(PSTR("mUltraSonicSensor::SubTask_UltraSonicAverage"));

  // 1 SECOND
  if(abs(millis()-averaged.instant.captured.tSaved)>1000){averaged.instant.captured.tSaved=millis();
    averaged.instant.final.distance_mm = GetDistanceMMReadingAdjustedForTemp(); //pin32

    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_ULTRASONIC "distance_mm=%d"),(int)averaged.instant.final.distance_mm);
    // #endif // ENABLE_LOG_LEVEL_DEBUG

    averaged.instant.final.distance_cm = averaged.instant.final.distance_mm/10; //pin32
    averaged.instant.isvalid = true;
    averaged.instant.captured.tLastChanged = millis();
  }

  // 1 MINUTE
  averaged.ptr = &averaged.smooth_1m;
  if(abs(millis()-averaged.ptr->captured.tSaved)>1000){averaged.ptr->captured.tSaved=millis(); //ONE MINUTE

    // Store new reading
    int temp = 0;
    averaged.ptr->captured.readings[averaged.ptr->captured.readIndex++] = GetDistanceMMReadingAdjustedForTemp();

    // Handle index
    if(averaged.ptr->captured.readIndex >= ADCSENSORS_SMOOTHSLOW_NUMREADINGS){averaged.ptr->captured.readIndex = 0; averaged.ptr->isvalid = true;}
    // Calculate original average
    averaged.ptr->captured.total=0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / ADCSENSORS_SMOOTHSLOW_NUMREADINGS;  // calculate the average:
    // Calculate deviation of original readings from average: deviation = |value-average|
    averaged.ptr->averaging.deviationarray[ADCSENSORS_SMOOTHSLOW_NUMREADINGS];
    averaged.ptr->averaging.deviationsum = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      averaged.ptr->averaging.deviationarray[ii] = abs(averaged.ptr->captured.readings[ii]-averaged.ptr->captured.average);
      averaged.ptr->averaging.deviationsum += averaged.ptr->averaging.deviationarray[ii];
    }
    // calculate the average deviation: averageDeviation = (sum of deviation)/number
    averaged.ptr->averaging.deviationaverage = averaged.ptr->averaging.deviationsum / ADCSENSORS_SMOOTHSLOW_NUMREADINGS;
    // Discard any outliers that have high deviations from original average: deviation > 2*averageDeviation
    averaged.ptr->averaging.isoutlier[ADCSENSORS_SMOOTHSLOW_NUMREADINGS];
    averaged.ptr->averaging.outliercount = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      if(averaged.ptr->averaging.deviationarray[ii]>1*averaged.ptr->averaging.deviationaverage){ //if an outlier
        averaged.ptr->averaging.isoutlier[ii] = 1;
        averaged.ptr->averaging.outliercount++;
      }else{
        averaged.ptr->averaging.isoutlier[ii] = 0;
      }
    }
    // Recalculate new average without outliers
    averaged.ptr->averaging.usablecount = 0;
    averaged.ptr->captured.total = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSLOW_NUMREADINGS;ii++){
      if(!averaged.ptr->averaging.isoutlier[ii]){
        averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
        averaged.ptr->averaging.usablecount++;
      }
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / averaged.ptr->averaging.usablecount;  // calculate the average:
    averaged.ptr->averaging.usableratio = (averaged.ptr->averaging.usablecount/ADCSENSORS_SMOOTHSLOW_NUMREADINGS)*100;

    // Set average to invalid if avergae deviation is too large
    // if(averaged.ptr->averaging.deviationaverage<10){
    //   averaged.ptr->isvalid = true;
    // }else{
    //   averaged.ptr->isvalid = false;
    // }

    //store
    averaged.ptr->final.distance_mm = averaged.ptr->captured.average;
    averaged.ptr->final.distance_cm = averaged.ptr->captured.average/10;
    averaged.ptr->ischanged = true;

    averaged.ptr->captured.tLastChanged = millis();

  }

  // 1 HOUR
  averaged.ptr = &averaged.smooth_1hr;
  if(abs(millis()-averaged.ptr->captured.tSaved)>60000){averaged.ptr->captured.tSaved=millis(); //ONE MINUTE

    // Store new reading
    averaged.ptr->captured.readings[averaged.ptr->captured.readIndex++] = GetDistanceMMReadingAdjustedForTemp();

    // Handle index
    if(averaged.ptr->captured.readIndex >= ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS){averaged.ptr->captured.readIndex = 0; averaged.ptr->isvalid = true;}
    // Calculate original average
    averaged.ptr->captured.total=0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;  // calculate the average:
    // Calculate deviation of original readings from average: deviation = |value-average|
    averaged.ptr->averaging.deviationarray[ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS];
    averaged.ptr->averaging.deviationsum = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      averaged.ptr->averaging.deviationarray[ii] = abs(averaged.ptr->captured.readings[ii]-averaged.ptr->captured.average);
      averaged.ptr->averaging.deviationsum += averaged.ptr->averaging.deviationarray[ii];
    }
    // calculate the average deviation: averageDeviation = (sum of deviation)/number
    averaged.ptr->averaging.deviationaverage = averaged.ptr->averaging.deviationsum / ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;
    // Discard any outliers that have high deviations from original average: deviation > 2*averageDeviation
    averaged.ptr->averaging.isoutlier[ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS];
    averaged.ptr->averaging.outliercount = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      if(averaged.ptr->averaging.deviationarray[ii]>1*averaged.ptr->averaging.deviationaverage){ //if an outlier
        averaged.ptr->averaging.isoutlier[ii] = 1;
        averaged.ptr->averaging.outliercount++;
      }else{
        averaged.ptr->averaging.isoutlier[ii] = 0;
      }
    }
    // Recalculate new average without outliers
    averaged.ptr->averaging.usablecount = 0;
    averaged.ptr->captured.total = 0;
    for(int ii=0;ii<ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS;ii++){
      if(!averaged.ptr->averaging.isoutlier[ii]){
        averaged.ptr->captured.total += averaged.ptr->captured.readings[ii];
        averaged.ptr->averaging.usablecount++;
      }
    }
    averaged.ptr->captured.average = averaged.ptr->captured.total / averaged.ptr->averaging.usablecount;  // calculate the average:
    averaged.ptr->averaging.usableratio = (averaged.ptr->averaging.usablecount/ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS)*100;

    // Set average to invalid if avergae deviation is too large
    // if(averaged.ptr->averaging.deviationaverage<10){
    //   averaged.ptr->isvalid = true;
    // }else{
    //   averaged.ptr->isvalid = false;
    // }

    //store
    averaged.ptr->final.distance_mm = averaged.ptr->captured.average;
    averaged.ptr->final.distance_cm = averaged.ptr->captured.average/10;
    averaged.ptr->ischanged = true;

    averaged.ptr->captured.tLastChanged = millis();

  }

} // END function averaging




void mUltraSonicSensor::MQQTSendObjectDetected(void)
{

  if(motion_detect.ischanged){ motion_detect.ischanged=false;

    D_DATA_BUFFER_CLEAR();

    char buffer[50];
      
    JBI->Start();
      JBI->Add("location", DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), 0, buffer, sizeof(buffer)));
      JBI->Add("time", tkr_time->RtcTime.hhmmss_ctr);
    JBI->End();

    if(motion_detect.isactive){
      pCONT_mqtt->publish_device("status/motion/detected",JBI->GetBufferPtr(),false);
    }else{
      pCONT_mqtt->publish_device("status/motion/over",JBI->GetBufferPtr(),false);
    }

  }

}



int8_t mUltraSonicSensor::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }
  
  if(!settings.flags.EnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  
  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
            
      if(mTime::TimeReachedNonReset(&ultrasonic.tReadLast,1000)){//ultrasonic.settings.measure_rate_ms)){
        GetDurationReading(); 

        // float newdist = (5000*0.034)/2;
        
        // ALOG_TST(PSTR(D_LOG_ULTRASONIC "duration = %d %dms %dmetres %d"),
        //   ultrasonic.duration,
        //   ultrasonic.settings.measure_rate_ms,
        //   (int)newdist,
        //   (int)GetDistanceCMReading()
        // );
        
        if(ultrasonic.isvalid){ // stop trying
          ultrasonic.tReadLast = millis();
          fUpdateCalculations = true;
         }
      }

      if(ultrasonic.isvalid){
        SubTask_UltraSonicAverage();
        pCONT->Tasker_Interface(TASK_SENSOR_UPDATED); // Tell other dependent modules we have changed
      }

     // SubTask_DetectMotion();
         
    break;
    case TASK_EVERY_SECOND:
    
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_SENDER:  
      MQTTHandler_Sender();
    break;

    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
    break;
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
    break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
  }

} // END Tasker


#ifdef USE_MODULE_NETWORK_WEBSERVER
void mUltraSonicSensor::WebAppend_Root_Status_Table_Draw(){
  
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Ultrasonic Distance");
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_ult","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    BufferWriterI->Append_P(PSTR("<td>%s</td>"), "Ultrasonic Event");
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_ult","?");   
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);

}


void mUltraSonicSensor::WebAppend_Root_Status_Table_Data(){
  
  char float_ctr[10];

  JBI->Array_Start("tab_ult");// Class name
  for(int row=0;row<2;row++){
    JBI->Object_Start();
      JBI->Add("id",row);
      switch(row){
        default:
        case 0:
          dtostrf(GetDistanceCMReading()/100,3,2,float_ctr);
          JBI->Add_FV("ih",PSTR("\"%s m\""),float_ctr);
        break;
        case 1: JBI->Add_P("ih",motion_detect.detected_rtc_ctr); break;
      }  
    JBI->Object_End();
  }
  JBI->Array_End();

}
    #endif// USE_MODULE_NETWORK_WEBSERVER


void mUltraSonicSensor::SubTask_DetectMotion(){
  
  #ifdef DEVICE_LIVINGROOMSENSOR
    //Alerts if sensor value changes from previous
    if(mTime::TimeReached(&object_detected_static.tSavedCheck,10000)){
      float distancecm = GetDistanceCMReading();
      
        ALOG_INF(PSTR(D_LOG_PIR "distancecm=%d"),(int)distancecm);
      // in limits
      // if(WITHINLIMITS(object_detected_static.trigger_cm_min,
      //                 distancecm,
      //                 object_detected_static.trigger_cm_max)){
      if(distancecm<250){
        ALOG_INF(PSTR(D_LOG_PIR "object_detected_static \"%s\""),"within");
        object_detected_static.ispresent = true;
        AddMOTIONEventStatusSum(true);
      }else{
        ALOG_INF(PSTR(D_LOG_PIR "object_detected_static \"%s\""),"outside");
        object_detected_static.ispresent = false;
        AddMOTIONEventStatusSum(false);
      }

      // if(!GetMOTIONEventStatusSum()){ // If consecutive events are the same, then return true
      //   return;
      // }
      
      if(motion_detect.state!=object_detected_static.ispresent){
        ALOG_INF(PSTR(D_LOG_PIR "IF motion_detect"));

        pCONT_mqtt->publish_device("status/motion/event",object_detected_static.ispresent?"Present":"Not Present",false);
        
        
        motion_detect.state = object_detected_static.ispresent;
        motion_detect.tDetectTime = millis();

        mqtthandler_sensor_ifchanged.flags.SendNow = true;
        
        if(motion_detect.state){ 
          motion_detect.isactive = true;
          // motion_detect.wasactive = false; //toggle as "previous state"
          memcpy(motion_detect.detected_rtc_ctr,tkr_time->RtcTime.hhmmss_ctr,sizeof(tkr_time->RtcTime.hhmmss_ctr));
        }else{
          motion_detect.isactive = false;
          // motion_detect.wasactive = true; //toggle as "previous state"
        }

        // If this and previous state where the same
        // if(motion_detect.wasactive ++ motion_detect.wasactive){

        // }



        ALOG_DBG(PSTR(D_LOG_PIR "motion_detect"));
        motion_detect.ischanged = true;
      }else{
        ALOG_INF(PSTR(D_LOG_PIR "ELSE motion_detect"));

      }

    }
  #endif

}




void mUltraSonicSensor::parse_JSONCommand(){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/ultrasonic")>=0){
      ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return; // not meant for here
  } 

} // END FUNCTION


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mUltraSonicSensor::ConstructJSON_Settings(uint8_t json_level){

  JBI->Start();  

    // // root["json_teleperiod_level"] = tkr_set->Get_Json_Level_Name();

  return JBI->End();

}

uint8_t mUltraSonicSensor::ConstructJSON_Sensors(uint8_t json_level){

  JBI->Start(); 
    JBI->Object_Start(D_SENSOR);
      JBI->Add(D_ISVALID, ultrasonic.isvalid);
      JBI->Add(D_DURATION, ultrasonic.duration);
      JBI->Add(D_DURATION_RAW, ultrasonic.duration_raw);
      JBI->Add(D_TEMPERATURE, ultrasonic.temperature);
      JBI->Add(D_TEMPERATURE "_Age", abs(millis()-ultrasonic.tPermitTempUpdate));
      JBI->Add(D_SPEEDOFSOUND, ultrasonic.speedofsound);
      JBI->Add(D_LASTREAD, abs(millis()-ultrasonic.tUltraSonicSensorReadLast));
      if(json_level >= JSON_LEVEL_DETAILED){
        JBI->Object_Start(D_ACCURACY);
          JBI->Add(D_INSIDE, ultrasonic.accuracy.insidecount);
          JBI->Add(D_OUTSIDE, ultrasonic.accuracy.outsidecount);
          JBI->Add(D_PERCENTAGE, ultrasonic.accuracy.percent);
        JBI->Object_End(); // D_ACCURACY
        JBI->Level_Start_P(  PSTR(D_THRESHOLD));
          JBI->Add_P(          PSTR(D_SET     D_PERCENT), ultrasonic.threshold.setpercent);
          JBI->Add_P(          PSTR(D_NARROW  D_PERCENT), ultrasonic.threshold.narrowpercent);
          JBI->Add_P(          PSTR(D_WIDE    D_PERCENT), ultrasonic.threshold.widepercent);
          JBI->Add_P(          PSTR(D_LOWER   D_VALUE),   ultrasonic.threshold.lowervalue);
          JBI->Add_P(          PSTR(D_UPPER   D_VALUE),   ultrasonic.threshold.uppervalue);
          JBI->Add_P(          PSTR(D_INSIDE  D_COUNT),   ultrasonic.threshold.insidecount);
          JBI->Add_P(          PSTR(D_OUTSIDE D_COUNT),   ultrasonic.threshold.outsidecount);
          JBI->Level_Start_P(  PSTR(D_RATIO));
            JBI->Add_P(          PSTR(D_POSITIVE),   ultrasonic.threshold.ratio_pos);
            JBI->Add_P(          PSTR(D_NEGATIVE),   ultrasonic.threshold.ratio_pos);
            JBI->Add_P(          PSTR(D_RELATIVE),   ultrasonic.threshold.ratio_pos);
          JBI->Object_End(); // D_RATIO
        JBI->Object_End(); // D_THRESHOLD
      }
    JBI->Object_End(); // D_SENSOR
    JBI->Object_Start(D_INSTANT);
      JBI->Add(D_ISVALID, averaged.instant.isvalid);
      JBI->Add(D_DISTANCE "_mm", averaged.instant.final.distance_mm);
    JBI->Object_End();   // D_INSTANT 
    JBI->Object_Start(D_SMOOTH "_1m");
      JBI->Add(D_ISVALID, averaged.smooth_1m.isvalid);
      JBI->Add(D_DISTANCE "_mm", averaged.smooth_1m.final.distance_mm);
      if(json_level >= JSON_LEVEL_DETAILED){
      JBI->Add(D_DEVIATION, averaged.smooth_1m.averaging.deviationaverage);
      JBI->Add(D_OUTLIERS, averaged.smooth_1m.averaging.outliercount);
      JBI->Add(D_RATIO, averaged.smooth_1m.averaging.usableratio);
      }
    JBI->Object_End();  // D_SMOOTH "_1m"
    JBI->Object_Start(D_SMOOTH "_1hr");
      JBI->Add(D_ISVALID, averaged.smooth_1hr.isvalid);
      JBI->Add(D_DISTANCE "_mm", averaged.smooth_1hr.final.distance_mm);
      if(json_level >= JSON_LEVEL_DETAILED){
      JBI->Add(D_DEVIATION, averaged.smooth_1hr.averaging.deviationaverage);
      JBI->Add(D_OUTLIERS, averaged.smooth_1hr.averaging.outliercount);
      JBI->Add(D_RATIO, averaged.smooth_1hr.averaging.usableratio);
      }
    JBI->Object_End(); // D_SMOOTH "_1hr"

  return JBI->End();

}

uint8_t mUltraSonicSensor::ConstructJSON_SensorsAveraged(uint8_t json_level){

  JBI->Start();  
  // Serial.println("ConstructJSON_SensorsAveraged");

  // D_DATA_BUFFER_CLEAR();
  // DynamicJsonDocument doc(200);
  // JsonObject root = doc.to<JsonObject>();

  // // root["json_teleperiod_level"] = "test";//tkr_set->Get_Json_Level_Name();

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return JBI->End();
}

/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mUltraSonicSensor::MQTTHandler_Init(){

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_Sensors;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10;//pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_Sensors;
  
  ptr = &mqtthandler_averaged_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_AVERAGED_CTR;
  ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_SensorsAveraged;
  
  ptr = &mqtthandler_averaged_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_AVERAGED_CTR;
  ptr->ConstructJSON_function = &mUltraSonicSensor::ConstructJSON_SensorsAveraged;

} 


void mUltraSonicSensor::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} 


void mUltraSonicSensor::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mUltraSonicSensor::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID,
    MQTT_HANDLER_MODULE_AVERAGED_IFCHANGED_ID
  };
  
  struct handler<mUltraSonicSensor>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod,
    &mqtthandler_averaged_ifchanged
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_SENSORS_ULTRASONIC_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );
  
}


////////////////////// END OF MQTT /////////////////////////

#endif
