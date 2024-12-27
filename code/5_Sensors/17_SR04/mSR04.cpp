
#include "mSR04.h"

#ifdef USE_MODULE_SENSORS_SR04

int8_t mSR04::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();  // should "pre_init" ne chanegd to pin/gpio/hardware pin ??
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
    break;
    case TASK_EVERY_MINUTE:
      EveryMinute();
    break;
    case TASK_UPTIME_1_MINUTES:
      readings.average_EMA.alpha = 2.0f / (200.0f-1.0f); // 60 samples, slower
      Config_Filters();
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

} // END function



void mSR04::Pre_Init(void)
{
  if (pCONT_pins->PinUsed(GPIO_SR04_TRIG_ID) && pCONT_pins->PinUsed(GPIO_SR04_ECHO_ID))
  {
    settings.fEnableSensor = true;
    settings.fSensorCount++;
  }
}


void mSR04::Config_Filters()
{

  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  /*  As we said in the previous post, in the exponential type filter the amount of smoothing is controlled through the alpha factor. It is not easy to find a simple relationship with the cut-off frequency, among other things, because the sampling frequency is unknown and variable.
      What we do know is that the lower the alpha factor, the greater the smoothing of the signal, and therefore, the lower the cut-off frequency. Remember that, as a negative point, the delay between the filtered signal and the original signal will also be greater.
      In a bandpass and stop band filter we will have two alpha factors that we will call Low and High that correspond, respectively, with the lower and upper cut frequency.
      Below we will see the effect of these factors with the results of the bandpass filter for different combinations of alpha Low and High applied to the same signal.
      These are the results for an alpha Low of 0.025 and alpha High of 0.5. Note that the lower filter has removed the bias after a few oscillations, while the upper filter has softened some of the high-frequency noise
  */
  readings.average_EMA.filter  = new SingleEMAFilter<float>(readings.average_EMA.alpha);  // alpha = 2/(N-1) so for N=60 ie 1/sec for 1 minute
  #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  readings.average_DEMA.filter = new DoubleEMAFilter<float>(readings.average_DEMA.alpha1, readings.average_DEMA.alpha2);
  #endif // ENABLE_DEVFEATURE_SR04_SINGLE_EMA_FILTERING

  mqtthandler_settings.flags.SendNow = true;

}


void mSR04::Init(void)
{

  if (sonar_serial==nullptr && sonar==nullptr) {
    ModeDetect();
  }

  ALOG_TST(PSTR(D_LOG_SR04 "ModeDetect Complete"));

  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  // Default weight values
  readings.average_EMA.alpha = 2.0f / (5.0f-1.0f); // faster
  // readings.average_EMA.alpha = 2.0f / (60.0f-1.0f); // 60 samples, slower
  #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  readings.average_DEMA.alpha1 = 2.0f / (100.0f-1.0f);
  readings.average_DEMA.alpha2 = 1-(2.0f / (100.0f-1.0f));
  #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

  Config_Filters();

  #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  readings.conversion_settings.flag_distance_conversion_method = EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_TEMPERATURE_COMPENSATED__ID;
  readings.temp_adj.flag_enabled = true;
  #endif

}


uint8_t mSR04::ModeDetect(void)
{
  sr04_type = 1; // default
  if (!pCONT_pins->PinUsed(GPIO_SR04_ECHO_ID))
  {
    ALOG_TST(PSTR("Sr04: TModeDetect::Error"));
    return sr04_type; 
  }

  int sr04_echo_pin = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);
  int sr04_trig_pin = (pCONT_pins->PinUsed(GPIO_SR04_TRIG_ID)) ? pCONT_pins->GetPin(GPIO_SR04_TRIG_ID) : pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);   // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
  sonar_serial = new TasmotaSerial(sr04_echo_pin, sr04_trig_pin, 1);


  if (sonar_serial->begin(SONAR_SERIAL_BAUD,1))
  {
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("SR04: Detect mode pins TX%d, RX%d"), sr04_trig_pin, sr04_echo_pin);
    if (sr04_trig_pin != -1) 
    {      
      if (PinUsed(GPIO_SR04_TRIG_ID, i)) {
        SR04[i].type = (Sr04TMiddleValue(Sr04TMode3Distance(i), Sr04TMode3Distance(i), Sr04TMode3Distance(i)) != 0) ? SR04_MODE_SER_TRANSCEIVER : SR04_MODE_TRIGGER_ECHO;
      } else {
        SR04[i].type = (Sr04TMiddleValue(Sr04TMode2Distance(i), Sr04TMode2Distance(i), Sr04TMode2Distance(i)) != 0) ? SR04_MODE_SER_RECEIVER : SR04_MODE_TRIGGER_ECHO;
      }
    } else {
      sr04_type = 2;
    }
  } else {
    sr04_type = 1;
  }

  if (sr04_type < 2) {
    delete sonar_serial;
    sonar_serial = nullptr;
    ALOG_TST(PSTR(D_LOG_SR04 "Release TasmotaSerial"));
    if (-1 == sr04_trig_pin) {
      sr04_trig_pin = pCONT_pins->GetPin(GPIO_SR04_ECHO_ID);  // if GPIO_SR04_TRIG is not configured use single PIN mode with GPIO_SR04_ECHO only
    }
    sonar = new NewPing(sr04_trig_pin, sr04_echo_pin, 100);
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_SR04 "NewPing %d %d"), sr04_trig_pin, sr04_echo_pin);

  } else {
    if (sonar_serial->hardwareSerial()) {
      // ClaimSerial();
      AddLog(LOG_LEVEL_INFO,PSTR("ClaimSerial NotEnabled %d %d"), sr04_trig_pin, sr04_echo_pin);
    }
  }

  AddLog(LOG_LEVEL_INFO,PSTR("SR04: Mode %d"), sr04_type);
  return sr04_type;

}


uint16_t mSR04::MiddleValue(uint16_t first, uint16_t second, uint16_t third)
{
  uint16_t ret = first;
  if (first > second) {
    first = second;
    second = ret;
  }

  if (third < first) {
    return first;
  } else if (third > second) {
    return second;
  } else {
    return third;
  }
}


uint16_t mSR04::Mode3Distance() 
{
  sonar_serial->write(0x55);
  sonar_serial->flush();
  return Mode2Distance();
}


uint16_t mSR04::Mode2Distance(void)
{
  sonar_serial->setTimeout(300);
  const char startByte = 0xff;

  if (!sonar_serial->find(startByte)) {
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("SR04: No start byte"));
    return NO_ECHO;
  }

  delay(5);

  uint8_t crc = sonar_serial->read();
  //read high byte
  uint16_t distance = ((uint16_t)crc) << 8;

  //read low byte
  distance += sonar_serial->read();
  crc += distance & 0x00ff;
  crc += 0x00FF;

  //check crc sum
  if (crc != sonar_serial->read()) {
    AddLog(LOG_LEVEL_ERROR,PSTR("SR04: Reading CRC error."));
    return NO_ECHO;
  }
  AddLog(LOG_LEVEL_DEV_TEST,PSTR("SR04: Distance: %d"), distance);
  
  return distance;
}


void mSR04::EverySecond(void)
{

  MeasureSensor();

}


void mSR04::EveryMinute(void)
{
  #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  SubTask_UpdateAmbientTemperature();
  #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
}

void mSR04::MeasureSensor()
{
  
  switch (sr04_type)
  {
    case 3:
      // readings.distance_cm = (float)(MiddleValue(Mode3Distance(),Mode3Distance(),Mode3Distance()))/ 10; //convert to cm
      break;
    case 2:
      while(sonar_serial->available()) sonar_serial->read(); // empty input buffer first
      // readings.distance_cm = (float)(MiddleValue(Mode2Distance(),Mode2Distance(),Mode2Distance()))/10;
      break;
    case 1:
      readings.raw.ping_value = sonar->ping_median(1);
      Serial.println(readings.raw.ping_value);
      readings.raw.distance_cm = GetDistanceFromPing(readings.raw.ping_value);
      break;
    default:
      readings.raw.ping_value = NO_ECHO;
  }
  
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
  readings.average_EMA.distance_cm  = readings.average_EMA.filter->AddValue(readings.raw.distance_cm);
  #endif
  #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
  readings.average_DEMA.distance_cm = readings.average_DEMA.filter->AddValue(readings.raw.distance_cm);
  #endif

  ALOG_TST(PSTR("GetDistanceFromPing %d (cm)"), (int)readings.raw.distance_cm);

  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}

float mSR04::GetDistanceFromPing(uint32_t ping_value)
{
  if(ping_value==0){ return 0; }
  float distance_cm = 0;

  // if(settings.flag_distance_conversion_method == EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_DEFAULT__ID)
  // {
  //   readings.conversion_settings.speed_of_sound = 343.0f;
  //   float speed_of_sound_cm_per_us = readings.conversion_settings.speed_of_sound/10000.0f;
  //   distance_cm = (float)(ping_value)*(speed_of_sound_cm_per_us/2.0f);
  //   return 
  // }


  // switch(readings.conversion_settings.flag_distance_conversion_method)
  // {
  //   default:
  //   case EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID:
  //     distance_cm = (float)(ping_value)/ US_ROUNDTRIP_CM;
  //   break;
  //   case EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_DEFAULT__ID:
  //   {
  //     readings.conversion_settings.speed_of_sound = 343.0f;
  //     float speed_of_sound_cm_per_us = readings.conversion_settings.speed_of_sound/10000.0f;
  //     distance_cm = (float)(ping_value)*(speed_of_sound_cm_per_us/2.0f);




// // this should be last to always return it
//   if(settings.flag_distance_conversion_method == EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID)
//   {
//     return distance_cm = (float)(ping_value)/ US_ROUNDTRIP_CM;
//   }
//   if
  
  switch(readings.conversion_settings.flag_distance_conversion_method)
  {
    default:
    case EM_DISTANCE_PING_CONVERSION_METHOD__BASIC__ID:
      distance_cm = (float)(ping_value)/ US_ROUNDTRIP_CM;
    break;
    case EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_DEFAULT__ID:
    {
      readings.conversion_settings.speed_of_sound = 343.0f;
      float speed_of_sound_cm_per_us = readings.conversion_settings.speed_of_sound/10000.0f;
      distance_cm = (float)(ping_value)*(speed_of_sound_cm_per_us/2.0f);
    }
    break;
    #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    case EM_DISTANCE_PING_CONVERSION_METHOD__SPEED_OF_SOUND_TEMPERATURE_COMPENSATED__ID:
    {
      // readings.temp_adj.ambient_temperature = 23.0f;
      // need to check flag here
      readings.conversion_settings.speed_of_sound = 331.3 + (0.606 * readings.temp_adj.ambient_temperature); 
      float speed_of_sound_cmps = readings.conversion_settings.speed_of_sound/100.0f;
      distance_cm = (float)(ping_value)*(speed_of_sound_cmps/2.0f/100.0f);
    }
    break;
    #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  }
  
  return distance_cm;

}



/**
 * @brief Check if any sensor reports temperature and has friendly name "AmbientTemperature"
 * 
 **/
#ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
void mSR04::SubTask_UpdateAmbientTemperature()
{

  ALOG_INF(PSTR("SubTask_UpdateAmbientTemperature"));

  /**
   * @brief I need a way to make this runtime safe
   **/

  float temperature = 0;
  uint8_t  device_id = 0; //ambient tank should be 0

  sensors_reading_t reading;
  pCONT->GetModule(pCONT_db18->GetModuleUniqueID())->GetSensorReading(&reading, device_id);
  temperature = reading.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);

  Serial.printf("temperature=%f\n\r", temperature);

  readings.temp_adj.ambient_temperature = temperature;

}
#endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION




uint8_t mSR04::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add(D_TYPE, sr04_type);
    
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    JBI->Object_Start("Filtered_EMA");
      JBI->Add("alpha", readings.average_EMA.alpha);
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    JBI->Object_Start("Filtered_DEMA");
      JBI->Add("alpha1", readings.average_DEMA.alpha1);
      JBI->Add("alpha2", readings.average_DEMA.alpha1);
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA

  return JBI->End();
}


uint8_t mSR04::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("Ping", readings.raw.ping_value);
    JBI->Add(D_DISTANCE "_cm", readings.raw.distance_cm);
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    JBI->Object_Start("Filtered_EMA");
      JBI->Add(D_DISTANCE "_cm", readings.average_EMA.distance_cm);
      JBI->Add("GetLowPass", readings.average_EMA.filter->GetLowPass());
      JBI->Add("GetHighPass", readings.average_EMA.filter->GetHighPass());
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    JBI->Object_Start("Filtered_DEMA");
      JBI->Add(D_DISTANCE "_cm", readings.average_DEMA.distance_cm);
      JBI->Add("GetBandPass", readings.average_DEMA.filter->GetBandPass());
      JBI->Add("GetBandStop", readings.average_DEMA.filter->GetBandStop());
    #endif // ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    JBI->Object_End();
    #ifdef ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
    JBI->Object_Start("Temperature");
      JBI->Add("Enabled", readings.temp_adj.flag_enabled);
      JBI->Add("Ambient", readings.temp_adj.ambient_temperature);
    JBI->Object_End();
    JBI->Object_Start("Conversion");
      JBI->Add("speed_of_sound", readings.conversion_settings.speed_of_sound);
      JBI->Add("flag_distance_conversion_method", readings.conversion_settings.flag_distance_conversion_method);
    JBI->Object_End();
    #endif // ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  return JBI->End();    
}


#endif
