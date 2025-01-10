#include "mMoistureSensor.h"

#ifdef USE_MODULE_SENSORS_MOISTURE

void mMoistureSensor::init(void){
  //PIR_DETECT_INIT();

  pinMode(MOISTURE_DIGITAL_PIN,INPUT);
  // pinMode(MOISTURE_DIGITAL_PIN,INPUT); //MOISTURE_ANALOG_PIN

}

void mMoistureSensor::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    case TASK_INIT:
      init();
    break;
    case TASK_LOOP:

      // LED_BLUE_SET(!PIR_DETECTED());

      // if(PIR_DETECTED()!=pir_detect.state){
      //   pCONT->mqt->publish_device("status/motion/event",PIR_DETECTED_CTR,false);
      //   pir_detect.state = PIR_DETECTED();
      //   pir_detect.tDetectTime = millis();
      //   if(pir_detect.state){ 
      //     pir_detect.isactive = true;
      //   }else{
      //     pir_detect.isactive = false;
      //   }
      //   //ALOG_DBG(PSTR(D_LOG_PIR "pir_detect \"%s\""),ONOFF_CTR(pir_detect));

      //   pir_detect.ischanged = true;
      // }

      // Use short timer to automatically clear event
      //if > 1 sec
      //clear struct

    break;
    case TASK_MQTT_SENDER:
      SubTasker_MQTTSender();
    break;
  }

} // END function

#ifdef USE_MODULE_NETWORK_MQTT

// NEW METHOD -- first senders then on internals
void mMoistureSensor::SubTasker_MQTTSender(){

  if(mTime::TimeReached(&tSavedMeasure,rateMeasure*1000)){
    MQTTSendMoistureSensorIfChanged();
  }


}

//#ifdef USE_MODULE_SENSORS_PIR // Motion Sensing -- NEEDS MOVED INTO ITS OWN FUNCTION
void mMoistureSensor::MQTTSendMoistureSensorIfChanged(){

  D_DATA_BUFFER_CLEAR();

  StaticJsonDocument<300> doc;
  JsonObject root = doc.to<JsonObject>();

  // root["location"] = "MOTIONALERT_PAYLOAD_CTR";
  // root["time"] = tkr_time->RtcTime.hhmmss_ctr;

  root["digital"] = digitalRead(MOISTURE_DIGITAL_PIN);
  root["analog"] = analogRead(MOISTURE_ANALOG_PIN);

  root["min"] = MIN_ADC_BOUNDARY;
  root["max"] = MAX_ADC_BOUNDARY;

  uint16_t adc_level = analogRead(MOISTURE_ANALOG_PIN);
  uint16_t adc_level_flipped = MAX_ADC_BOUNDARY-adc_level;
  float adc_mapped = mSupport::mapfloat(adc_level,MIN_ADC_BOUNDARY,MAX_ADC_BOUNDARY,100,0);
  float adc_mapped_constrained = constrain(adc_mapped,0,100);

  root["adc_level_flipped"] = adc_level_flipped;
  root["adc_mapped"] = adc_mapped;
  root["percentage"] = adc_mapped_constrained;

  data_buffer.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer.payload.ctr);

  pCONT->mqt->publish_device("status/moisture",data_buffer.payload.ctr,false);

}

void mMoistureSensor::AddToJsonObject_AddHardware(JsonObject root){
  JsonObject obj = root.createNestedObject("moisture");
    obj["type"] = "PIR_DETECTOR_NAME_CTR";
    obj["pin"] = PIR_DETECT_PIN;
}

#endif

#endif
