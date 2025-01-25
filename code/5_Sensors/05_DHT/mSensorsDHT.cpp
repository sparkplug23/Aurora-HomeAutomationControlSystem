/*
  mHVAC.cpp - mSensorsDHT

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
#include "mSensorsDHT.h"

#ifdef USE_MODULE_SENSORS_DHT

int8_t mSensorsDHT::Tasker(uint8_t function, JsonParserObject obj){
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    case TASK_LOOP:
      EveryLoop();
    break;
    case TASK_SENSOR_SHOW_LATEST_LOGGED_ID:
      ShowSensor_AddLog();
    break;
    /************
     * WEBPAGE SECTION * 
    *******************/
    // #ifdef USE_MODULE_NETWORK_WEBSERVER
    // case TASK_WEB_ADD_HANDLER:
    //   WebPage_Root_AddHandlers();
    // break;
    // case TASK_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    // break;
    // case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    // break;
    // #endif //USE_MODULE_NETWORK_WEBSERVER
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



void mSensorsDHT::Pre_Init(void){

  settings.fEnableSensor = false;
  settings.sensor_active_count = 0;
  
  if (pCONT_pins->PinUsed(GPIO_DHT11_1OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT11_1OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT11);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT11_1of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT11_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT11_2OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT11);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT11_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_1OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT22_1OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_1of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }else{
    
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_DHT "DHT Sensor 1 not found"));
    // delay(2000);
  }
  if (pCONT_pins->PinUsed(GPIO_DHT22_2OF2_ID)) {  // not set when 255
    pin[settings.sensor_active_count] = pCONT_pins->GetPin(GPIO_DHT22_2OF2_ID);
    sensor[settings.sensor_active_count].dht = new DHTesp;
    sensor[settings.sensor_active_count].dht->setup(pin[settings.sensor_active_count], DHTesp::DHT22);
    AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_DHT "DHT22_2of2 Pin[%d] %d"),settings.sensor_active_count,pin[settings.sensor_active_count]);
    settings.sensor_active_count++;
  }
  else{
    
    AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_DHT "DHT Sensor 1 not found"));
    // delay(2000);
  }
  // {"flag_serial_set_tx_set":0,"GPIO":{"DHT22_1":25},
  // "p in_attached_gpio_functions":
  // [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
  // "user_template_io":
  // [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
  // "getpin":
  // [-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,25,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]}

  if(settings.sensor_active_count){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "DHT Sensor Enabled"));
  }

}

void mSensorsDHT::Init(void){

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
    sensor[sensor_id].instant.tSavedMeasureClimate = millis();
    sensor[sensor_id].instant.temperature = -1;
    sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_NOT_RUNNING_ID;
    // sensor[sensor_id].name_ptr = sensor_unset_ctr;
  }
  
}

/**
 * Needs complete rewrite
 * */
void mSensorsDHT::SplitTask_UpdateClimateSensors(uint8_t sensor_id, uint8_t require_completion){

  unsigned long timeout = millis();
  do{

    //if(!fWithinLimit){
      unsigned long tmp = (abs(millis()-sensor[sensor_id].instant.tWithinLimit));
      if(tmp<=1000){
        break;
      }
      // else{
      //   Serial.print("retryingtoosoon-"); Serial.println(tmp);
      // }
    //}

    switch(sensor[sensor_id].instant.sUpdateClimateSensors){
      case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
        // do nothing
      //break;
      default:
      case SPLIT_TASK_TIMEOUT_ID:
      case SPLIT_TASK_SEC1_ID:{

        TempAndHumidity newValues = sensor[sensor_id].dht->getTempAndHumidity();
        ComfortState cf;

        // Check if any reads failed and exit early (to try again).
        if (
          (sensor[sensor_id].dht->getStatus() != 0) ||
          (newValues.temperature == -1.0f)
        ){
          AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_DHT "Read error"));
          sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_ERROR_ID;
          sensor[sensor_id].instant.isvalid = false;
        }else{
          sensor[sensor_id].instant.isvalid = true;

          if(sensor[sensor_id].instant.temperature != newValues.temperature){
            sensor[sensor_id].instant.ischanged = true; // check if updated

          }else{
            sensor[sensor_id].instant.ischanged = false;
          }

          if((fabsf(sensor[sensor_id].instant.temperature-newValues.temperature)>0.1)||
            (sensor[sensor_id].instant.temperature != newValues.temperature)&&(abs(millis()-sensor[sensor_id].instant.ischangedtLast)>60000)){
            sensor[sensor_id].instant.ischanged_over_threshold = true; // check if updated
            sensor[sensor_id].instant.ischangedtLast = millis();
          }else{
            sensor[sensor_id].instant.ischanged_over_threshold = false;
          }

          sensor[sensor_id].instant.temperature = newValues.temperature;  
          sensor[sensor_id].instant.humidity = newValues.humidity;

          //make precision as stored setting
          // dtostrf(sensor[sensor_id].instant.temperature,3,1,sensor[sensor_id].temperature_ctr);
          // dtostrf(sensor[sensor_id].humidity,3,1,sensor[sensor_id].humidity_ctr);

          sensor[sensor_id].instant.heatIndex = sensor[sensor_id].dht->computeHeatIndex(newValues.temperature, newValues.humidity);
          sensor[sensor_id].instant.dewPoint = sensor[sensor_id].dht->computeDewPoint(newValues.temperature, newValues.humidity);
          sensor[sensor_id].instant.cr = sensor[sensor_id].dht->getComfortRatio(cf, newValues.temperature, newValues.humidity);

          ALOG_DBM( PSTR(D_LOG_DHT "temperature %d"),(int)sensor[sensor_id].instant.temperature);
      
          sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_DONE_ID;
        }
        
      }
      break;
      case SPLIT_TASK_DONE_ID: //exiting
        sensor[sensor_id].instant.fWithinLimit = 1;
      break;
      break;
    } // end switch

    if(require_completion){ //delay required if we are going to do multiple calls
    //  delay(100);
    }

    if(abs(millis()-timeout)>=2000){
      // Serial.println("if(abs(millis()-timeout)>=2000){");
      sensor[sensor_id].instant.sUpdateClimateSensors = SPLIT_TASK_TIMEOUT_ID;
      break;
    }
    // else{
      
    //   Serial.println("ELSE if(abs(millis()-timeout)>=2000){");
    // }

  }while(require_completion); // loops once even if false

  DEBUG_LINE;

}//end function

void mSensorsDHT::EveryLoop(){
  
  for (int sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){

    if(mTime::TimeReachedNonReset(&sensor[sensor_id].instant.tSavedMeasureClimate,1000)){
      if(!settings.sensor_active_count){ // Retry init if lost after found during boot
        init(); //search again
      }else{
        SplitTask_UpdateClimateSensors(sensor_id,DONTREQUIRE_COMPLETE);
        if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
          sensor[sensor_id].instant.sUpdateClimateSensors=SPLIT_TASK_SUCCESS_ID;
          sensor[sensor_id].instant.tSavedMeasureClimate = millis();
        }else
        if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_ERROR_ID){ 
          sensor[sensor_id].instant.tSavedMeasureClimate = millis();//+5000; //backoff for 5 seconds
        }
      }

    }
  }//end for

}

void mSensorsDHT::ShowSensor_AddLog()
{
  
  ConstructJSON_Sensor(JSON_LEVEL_SHORT);
  ALOG_INF(PSTR(D_LOG_DHT "\"%s\""),JBI->GetBufferPtr());

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mSensorsDHT::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("SensorCount", settings.sensor_active_count);
    JBI->Array_Start("Pin");
      JBI->Add(pin[0]);
      JBI->Add(pin[1]);
    JBI->Array_End();
  return JBI->End();

}


// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mSensorsDHT::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[50];

  JBI->Start();
  for(uint8_t sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){
    if(
      sensor[sensor_id].instant.ischanged || 
      (json_level >  JSON_LEVEL_IFCHANGED) || 
      (json_level == JSON_LEVEL_SHORT)
    ){

      JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),sensor_id,buffer,sizeof(buffer)));   
        JBI->Add(D_TEMPERATURE, sensor[sensor_id].instant.temperature);
        JBI->Add(D_HUMIDITY,    sensor[sensor_id].instant.humidity);
        if(json_level >=  JSON_LEVEL_DETAILED)
        {     
          JBI->Object_Start(D_ISCHANGEDMETHOD);
            JBI->Add(D_TYPE, D_SIGNIFICANTLY);
            JBI->Add(D_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].instant.ischangedtLast)/1000));
          JBI->Object_End();   
        }
      JBI->Object_End(); 
    }

  }
    
  return JBI->End();

}


  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsDHT::MQTTHandler_Init()
{

  struct handler<mSensorsDHT>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSensorsDHT::ConstructJSON_Sensor;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsDHT::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsDHT::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mSensorsDHT::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT


/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


// #ifdef USE_MODULE_NETWORK_WEBSERVER
// void mSensorsDHT::WebAppend_Root_Status_Table_Draw(){

//   for(int ii=0;ii<settings.sensor_active_count;ii++){ //add number in name? List needed? also hold user defined name?
    
//       char name_buffer_tmp[25];
//       // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), ii, name_buffer);

//       DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), ii, name_buffer_tmp, sizeof(name_buffer_tmp));

//       uint8_t multiline_enabled = false;


//     JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//       JBI->Append_P(PSTR("<td>DHT%s %s %s</td>"), "22",multiline_enabled?"Temperature":"Climate",name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
//       JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
      
//     if(multiline_enabled){
//       JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//       JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//         JBI->Append_P(PSTR("<td>DHT%s Humidity %s</td>"), "22", name_buffer_tmp);//pCONT_sup->GetTextIndexed_P(listheading, sizeof(listheading), ii, kTitle_TableTitles_Root));//"Animation List Tester");      //titles are fixed, so send them here using getindex
//     }
//     JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_CLASS_TYPE_2V,"tab_dht","?");   
//     JBI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   }



// }


// //append to internal buffer if any root messages table
// void mSensorsDHT::WebAppend_Root_Status_Table_Data(){
  
//   uint8_t sensor_counter = 0;

//   // JBI->Append_P(PSTR("\"%s\":["),PSTR("tab_dht")); 
  
//   JBI->Array_Start("tab_dht");// Class name

//   for(int row=0;row<(2*settings.sensor_active_count);row++){
//     switch(row%2){
//       default:
//       case 0:{
        
//         char float_ctr[10];
//         char colour_ctr[10];
//         char table_row[25]; memset(table_row,0,sizeof(table_row));       

//         char value_ctr[8];
//         pCONT_sup->dtostrfd(sensor[sensor_counter].instant.temperature,2,value_ctr);

//         sprintf(table_row,"%s&deg;%c",value_ctr,pCONT_sup->TempUnit());
        
//         if(sensor[sensor_counter].instant.temperature<=25){
//           sprintf(colour_ctr,"%s","#00ff00"); //create variable/use webcolour ids
//         }else
//         if(sensor[sensor_counter].instant.temperature>25){
//           sprintf(colour_ctr,"%s","#fcba03");
//         }else{
//           sprintf(colour_ctr,"%s","#ffffff");
//         }
    
//         // JBI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
//         //   table_row, colour_ctr
//         // );
        
//         JBI->Object_Start();
//           JBI->Add("id",row);
//           JBI->Add("ih",table_row);
//           JBI->Add("fc",colour_ctr);
//         JBI->Object_End();

//       }break;
//       case 1:{      

//         char float_ctr[10];
//         char colour_ctr[10];
//         char table_row[25]; memset(table_row,0,sizeof(table_row));        
        
//         char value_ctr[8];
//         pCONT_sup->dtostrfd(sensor[sensor_counter].instant.humidity,2,value_ctr);

//         sprintf(table_row,"%s %%",value_ctr);
        
//         if(sensor[sensor_counter].instant.humidity>70){
//           sprintf(colour_ctr,"%s","#ff0000"); //create variable/use webcolour ids
//         }else
//         {
//           sprintf(colour_ctr,"%s","#ffffff");
//         }
    
//         // JBI->Append_P(PSTR("{\"id\":%d,\"ih\":\"%s\",\"fc\":\"%s\"},"),row,
//         //   table_row, colour_ctr
//         // );
        
//         JBI->Object_Start();
//           JBI->Add("id",row);
//           JBI->Add("ih",table_row);
//           JBI->Add("fc",colour_ctr);
//         JBI->Object_End();

//         sensor_counter++;
//       }break;
//     }
//   }
//   // *tkr_web->buffer_writer_internal = (*tkr_web->buffer_writer_internal) - 1;// remove extra comma
//   // JBI->Append_P(PSTR("],")); 

//   JBI->Array_End();
// }


// void mSensorsDHT::WebPage_Root_AddHandlers(){

//   /**
//    * Pages
//    * */

// }

// #endif // USE_MODULE_NETWORK_WEBSERVER







#endif
