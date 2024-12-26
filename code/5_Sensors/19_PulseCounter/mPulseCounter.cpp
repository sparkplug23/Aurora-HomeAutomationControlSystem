#include "mPulseCounter.h"

#ifdef USE_MODULE_SENSORS_PULSE_COUNTER

void mPulseCounter::Pre_Init(void){

  fEnableSensor = false;
  fSensorCount = 0;

  memset(&sensor,0,sizeof(sensor));
  
  if (pCONT_pins->GetPin(GPIO_PULSE_COUNTER] < 99) {  // not set when 255
    pin[fSensorCount] = pCONT_pins->GetPin(GPIO_PULSE_COUNTER];

    pinMode(pin[fSensorCount],INPUT_PULLUP);

    // sensor[fSensorCount].dht = new DHTesp;
    // sensor[fSensorCount].dht->setup(pin[fSensorCount], DHTesp::DHT11);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "GPIO_PULSE_COUNTER Pin[%d] %d"),fSensorCount,pin[fSensorCount]);
    fSensorCount++;
  }

  if(fSensorCount){
    fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "DHT Sensor Enabled"));
  }

}

void mPulseCounter::Init(void){

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
    sensor[sensor_id].instant.tSavedMeasureClimate = millis();
    // sensor[sensor_id].instant.temperature = -1;
    sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_NOT_RUNNING_ID;
    // sensor[sensor_id].name_ptr = sensor_unset_ctr;
  }
  
  uint8_t buffer_length = 0;
  memset(name_buffer,0,sizeof(name_buffer));
  for(int ii=0;ii<MAX_SENSORS;ii++){
    buffer_length+=sprintf(name_buffer+buffer_length,"%s%d|","sens",ii);
  }

}

void mPulseCounter::SplitTask_UpdatePulseCounterSensors(uint8_t sensor_id, uint8_t require_completion){



  // unsigned long timeout = millis();
  // do{

  //   //if(!fWithinLimit){
  //     unsigned long tmp = (abs(millis()-sensor[sensor_id].instant.tWithinLimit));
  //     if(tmp<=1000){
  //       break;
  //     }else{
  //       //Serial.print("retryingtoosoon-"); Serial.println(tmp);
  //     }
  //   //}

  //   switch(sensor[sensor_id].instant.sUpdateClimateSensors){
  //     case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
  //       // do nothing
  //     //break;
  //     default:
  //     case SPLIT_TASK_TIMEOUT_ID:
  //     case SPLIT_TASK_SEC1_ID:{

  //       TempAndHumidity newValues = sensor[sensor_id].dht->getTempAndHumidity();
  //       ComfortState cf;

  //       // Check if any reads failed and exit early (to try again).
  //       if (sensor[sensor_id].dht->getStatus() != 0) {
  //         //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "Read error"));
  //         sensor[sensor_id].instant.isvalid = false;
  //       }else{
  //         sensor[sensor_id].instant.isvalid = true;

  //         if(sensor[sensor_id].instant.temperature != newValues.temperature){
  //           sensor[sensor_id].instant.ischanged = true; // check if updated

  //         }else{
  //           sensor[sensor_id].instant.ischanged = false;
  //         }

  //         if((fabsf(sensor[sensor_id].instant.temperature-newValues.temperature)>0.1)||
  //           (sensor[sensor_id].instant.temperature != newValues.temperature)&&(abs(millis()-sensor[sensor_id].instant.ischangedtLast)>60000)){
  //           sensor[sensor_id].instant.ischanged_over_threshold = true; // check if updated
  //           sensor[sensor_id].instant.ischangedtLast = millis();
  //         }else{
  //           sensor[sensor_id].instant.ischanged_over_threshold = false;
  //         }

  //         sensor[sensor_id].instant.temperature = newValues.temperature;  
  //         sensor[sensor_id].instant.humidity = newValues.humidity;

  //         //make precision as stored setting
  //         // dtostrf(sensor[sensor_id].instant.temperature,3,1,sensor[sensor_id].temperature_ctr);
  //         // dtostrf(sensor[sensor_id].humidity,3,1,sensor[sensor_id].humidity_ctr);

  //         sensor[sensor_id].instant.heatIndex = sensor[sensor_id].dht->computeHeatIndex(newValues.temperature, newValues.humidity);
  //         sensor[sensor_id].instant.dewPoint = sensor[sensor_id].dht->computeDewPoint(newValues.temperature, newValues.humidity);
  //         sensor[sensor_id].instant.cr = sensor[sensor_id].dht->getComfortRatio(cf, newValues.temperature, newValues.humidity);

  //         ALOG_DBM( PSTR(D_LOG_DHT "temperature %d"),(int)sensor[sensor_id].instant.temperature);
      
  //         sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_DONE_ID;
  //       }
        
  //     }
  //     break;
  //     case SPLIT_TASK_DONE_ID: //exiting
  //       sensor[sensor_id].instant.fWithinLimit = 1;
  //     break;
  //     break;
  //   } // end switch

  //   if(require_completion){ //delay required if we are going to do multiple calls
  //   //  delay(100);
  //   }

  //   if(abs(millis()-timeout)>=2000){
  //     sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_TIMEOUT_ID;
  //     break;
  //   }

  // }while(require_completion); // loops once even if false

}//end function






void mPulseCounter::Tasker(uint8_t function, JsonParserObject obj){
  
  // some functions must run regardless
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
  if(!fEnableSensor){ return; }

  switch(function){
    case TASK_INIT:
      Init();
    break;
    case TASK_LOOP:
      EveryLoop();
    break;
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    case TASK_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Status_Table_Draw();
    break;
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table_Data();
    break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
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
  } // END switch

}// END Tasker

uint8_t mPulseCounter::Change_Detected(uint8_t sensor_id){
  return digitalRead(pin[sensor_id]);
}



void mPulseCounter::EveryLoop(){

// to be done with interrupts later!


// for(
  uint8_t sensor_id=0;
  //sensor_id<sensors_active;sensor_id++){
  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_PIR "PIR %s %d"),Change_Detected_Ctr(sensor_id),sensor_id);
    if(Change_Detected(sensor_id)!=sensor[sensor_id].instant.state){
  // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_PIR "PIR %s %d"),"Change_Detected_Ctr(sensor_id)",sensor_id);
      //if(pCONT_time->RtcTime.seconds_nonreset<20){ break; }
      // pCONT->mqt->publish_device("status/motion/event",Change_Detected_Ctr(sensor_id),false);

      sensor[sensor_id].instant.state = Change_Detected(sensor_id);

     

      if(sensor[sensor_id].instant.state){
        sensor[sensor_id].instant.timing_rise_ms = millis() - sensor[sensor_id].instant.tDetectTime;
        sensor[sensor_id].instant.tDetectTime = millis(); 
        memcpy(sensor[sensor_id].instant.detected_rtc_ctr,pCONT_time->RtcTime.hhmmss_ctr,sizeof(pCONT_time->RtcTime.hhmmss_ctr));
        sensor[sensor_id].instant.isactive = true;
      }else{
        sensor[sensor_id].instant.timing_fall_ms = millis() - sensor[sensor_id].instant.tEndedTime;
        sensor[sensor_id].instant.tEndedTime = millis();
        sensor[sensor_id].instant.isactive = false;

      }

      sensor[sensor_id].instant.tDeltaTime = sensor[sensor_id].instant.tEndedTime - sensor[sensor_id].instant.tDetectTime;
      
      if(!WithinLimits(settings.min_reading_ms,(uint16_t)sensor[sensor_id].instant.tDeltaTime,settings.max_reading_ms)){
        AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_PIR "PIR %s %d"),"Out of range",sensor_id);
        sensor[sensor_id].instant.ischanged = true;
        mqtthandler_sensor_ifchanged.flags.SendNow = true;
      }


      // ALOG_DBG(PSTR(D_LOG_PIR "pir_detect \"%s\""),Change_Detected_Ctr(sensor_id));
      
    }



    if(sensor[sensor_id].instant.tDeltaTime < 60000){ //1 minute
      sensor[sensor_id].instant.isvalid = true;
    }else
    if(abs(millis()-sensor[sensor_id].instant.tDetectTime)==60000){ //1 minute  //temp fix, send 0 to finish
      sensor[sensor_id].instant.isvalid = true;
      sensor[sensor_id].instant.ischanged = true;
      sensor[sensor_id].instant.tDeltaTime = 0; //clear
    }else{
      sensor[sensor_id].instant.isvalid = false;
    }



  // }

  
  // for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
  //   if(mTime::TimeReachedNonReset(&sensor[sensor_id].instant.tSavedMeasureClimate,1000)){
  //     if(!fSensorCount){ // Retry init if failed
  //       init(); //search again
  //     }else{
  //         SplitTask_UpdatePulseCounterSensors(sensor_id,DONTREQUIRE_COMPLETE);
  //         if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
  //           sensor[sensor_id].instant.sUpdateClimateSensors=SPLIT_TASK_SUCCESS_ID;
  //           sensor[sensor_id].instant.tSavedMeasureClimate = millis();
  //         }
  //     }
  //   }
  // }//end for
}

void mPulseCounter::WebAppend_Root_Status_Table_Draw(){

  // for(int ii=0;ii<fSensorCount;ii++){ //add number in name? List needed? also hold user defined name?
    
  //     char name_buffer_tmp[25];
  //     pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), ii, name_buffer);

  //   pCONT_web->AppendBuffer_PI2(PM_WEBAPPEND_TABLE_ROW_START_0V);
  //     pCONT_web->AppendBuffer_PI2(PSTR("<td>DHT%s Temperature %s</td>"), "22",name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
  //     pCONT_web->AppendBuffer_PI2(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
  //   pCONT_web->AppendBuffer_PI2(PM_WEBAPPEND_TABLE_ROW_END_0V);
  //   pCONT_web->AppendBuffer_PI2(PM_WEBAPPEND_TABLE_ROW_START_0V);
  //     pCONT_web->AppendBuffer_PI2(PSTR("<td>DHT%s Humidity %s</td>"), "22", name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
  //     pCONT_web->AppendBuffer_PI2(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
  //   pCONT_web->AppendBuffer_PI2(PM_WEBAPPEND_TABLE_ROW_END_0V);
  // }
}


//append to internal buffer if any root messages table
void mPulseCounter::WebAppend_Root_Status_Table_Data(){
  
  // uint8_t sensor_counter = 0;

  // pCONT_web->AppendBuffer_PI2(PSTR("\"%s\":["),PSTR("tab_dht")); 
  // for(int row=0;row<(2*fSensorCount);row++){
  //   switch(row%2){
  //     default:
  //     case 0:{
        
  //       char float_ctr[10];
  //       char colour_ctr[10];
  //       char table_row[25]; memset(table_row,0,sizeof(table_row));       

  //       char value_ctr[8];
  //       pCONT_sup->dtostrfd(sensor[sensor_counter].instant.temperature,2,value_ctr);

  //       sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
        
  //       if(sensor[sensor_counter].instant.temperature<=25){
  //         sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
  //       }else
  //       if(sensor[sensor_counter].instant.temperature>25){
  //         sprintf(colour_ctr,"%s","#fcba03");
  //       }else{
  //         sprintf(colour_ctr,"%s","#ffffff");
  //       }
    
  //       pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
  //         table_row, colour_ctr
  //       );
  //     }break;
  //     case 1:{      

  //       char float_ctr[10];
  //       char colour_ctr[10];
  //       char table_row[25]; memset(table_row,0,sizeof(table_row));        
        
  //       char value_ctr[8];
  //       pCONT_sup->dtostrfd(sensor[sensor_counter].instant.humidity,2,value_ctr);

  //       sprintf(table_row,"%s %%",value_ctr);
        
  //       if(sensor[sensor_counter].instant.humidity>70){
  //         sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
  //       }else
  //       {
  //         sprintf(colour_ctr,"%s","#ffffff");
  //       }
    
  //       pCONT_web->AppendBuffer_PI2(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
  //         table_row, colour_ctr
  //       );
  //       sensor_counter++;
  //     }break;
  //   }
  // }
  // *pCONT_web->buffer_writer_internal = (*pCONT_web->buffer_writer_internal) - 1;// remove extra comma
  // pCONT_web->AppendBuffer_PI2(PSTR("],")); 

}


void mPulseCounter::WebPage_Root_AddHandlers(){

  /**
   * Pages
   * */

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mPulseCounter::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

    D_DATA_BUFFER_CLEAR();
    StaticJsonDocument<400> doc;
    JsonObject root = doc.to<JsonObject>();

    root["SensorCount"] = fSensorCount;

    root["name_buffer"] = name_buffer;

    root["sens_tele_rate"] =mqtthandler_sensor_teleperiod.tRateSecs;
    root["sens_ifchanged_rate"] =mqtthandler_sensor_ifchanged.tRateSecs;
    root["sett_tele_rate"] =mqtthandler_settings.tRateSecs;


    data_buffer.payload.len = measureJson(root)+1;
    serializeJson(doc,data_buffer.payload.ctr);
    
  return (data_buffer.payload.len>3?1:0);

}



// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mPulseCounter::ConstructJSON_Sensor(uint8_t json_level){

  // clear entire mqtt packet
  D_DATA_BUFFER_CLEAR();

  uint8_t ischanged=false;

  StaticJsonDocument<300> doc;
  JsonObject root = doc.to<JsonObject>();


for(uint8_t sensor_id=0;sensor_id<fSensorCount;sensor_id++){

  ischanged = sensor[sensor_id].instant.ischanged;

  //if none changed, return early
  // if(!ischanged){
  //   return;
  // }name_buffer
  
  if(ischanged || (json_level>JSON_LEVEL_IFCHANGED)){

  
  char name_buffer_tmp[25];
  pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), sensor_id, name_buffer);

  


  //if(ischanged||pCONT->mqt->fSendAllData||pCONT->mqt->fSendSingleFunctionData){
    if(sensor[sensor_id].instant.isvalid){
      JsonObject sens1 = root.createNestedObject(name_buffer_tmp);//sensor[sensor_id].name_ptr);
      sens1["rise_ms"] = sensor[sensor_id].instant.timing_rise_ms;
      sens1["fall_ms"] = sensor[sensor_id].instant.timing_fall_ms;
      sens1["detect_ms"] = sensor[sensor_id].instant.tDetectTime;
      sens1["ended_ms"] = sensor[sensor_id].instant.tEndedTime;
      sens1["delta_ms"] = sensor[sensor_id].instant.tDeltaTime;
    }
  //}
  sensor[sensor_id].instant.ischanged = false; //temp fix

    // JsonObject method = root.createNestedObject("ischangedmethod");
    // //method["type"] = "significantly"; //or "any"
    // method["value"] = 0.1;
    // method["tLast"] = round(abs(millis()-sensor[0].instant.ischangedtLast)/1000);

}//if changed level




}

  data_buffer.payload.len = measureJson(root)+1;
  serializeJson(doc,data_buffer.payload.ctr);
  
  return (data_buffer.payload.len>3?1:0);

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mPulseCounter::MQTTHandler_Init(){

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mPulseCounter::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPulseCounter::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.ifchanged_secs;
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPulseCounter::ConstructJSON_Sensor;
  
} 


void mPulseCounter::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} 


void mPulseCounter::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mPulseCounter::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mSensorsDHT>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_DHT_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );
  uint8_t flag_handle_all = false, handler_found = false
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; ptr=&mqtthandler_settings; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; ptr=&mqtthandler_sensor_teleperiod; break;
      // No specialised needed
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Comm and(*this,D_MODULE_SENSORS_PULSECOUNTER_ID,ptr); }
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}

////////////////////// END OF MQTT /////////////////////////

#endif
