/*
  mRemoteDevice.cpp - Allows other hardware to report sensor readings remotely, to be used internally
  Methods include TCP, UDP, MQTT

  eg. Upstairs DHT to be changed to remote via BME sensor instead

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
#include "mRemoteDevice.h"

#ifdef USE_MODULE_SENSORS_REMOTE_DEVICE

const char* mRemoteDevice::PM_MODULE_SENSORS_REMOTE_DEVICE_CTR = D_MODULE_SENSORS_REMOTE_DEVICE_CTR;
const char* mRemoteDevice::PM_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR = D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR;

int8_t mRemoteDevice::Tasker(uint8_t function, JsonParserObject obj){
  
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    case FUNC_LOOP:
      EveryLoop();
    break;



    case FUNC_MQTT_CONNECTED:

      #ifdef REMOTE_SENSOR_1_MQTT_TOPIC
        AddLog(LOG_LEVEL_TEST, PSTR( "Subscribe to" "\"%s\""),REMOTE_SENSOR_1_MQTT_TOPIC);
        pCONT_mqtt->pubsub->subscribe(REMOTE_SENSOR_1_MQTT_TOPIC);//(PSTR("group_all/#"));
      #endif

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  } // END switch
  
}// END Tasker


void mRemoteDevice::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];
  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  //create a dynamic list of remotedevices as needed, storing their data in a sensortype (although large memory impact, allows for different types)
  
  /***
    //Devicename, and somehow the types of things it receives? Use by name? yes, so its basically json keys
    // RemoteDevice1 = "Bedroom,Temperature,Humidity"
    // RemoteDevice2 = "Bedroom,Humdiity"
   * */
/**
  D_JSON_REMOTE_SENSOR_NAME
  {
   "RemoteSensorNames":["Bedroom"],
   //just search for all, cycling around their names
   "RemoteSensorAttribrutes":["Temperature","Humidity"]   //ie list parameters to search for
  }
*/


  if(jtok = obj[REMOTE_SENSOR_JSON_NAME])
  {

    JsonParserObject sensor_obj = jtok;
    JsonParserToken jtok2 = 0;
    
    sensors_reading_t* value = &sensor_data;
    // Reset as the incoming data will replace everything to have one instance of each value
    value->type.clear();
    value->data.clear();

    if(jtok2 = sensor_obj[D_JSON_TEMPERATURE])
    {
      value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data.push_back(jtok2.getFloat());
    }
    
    if(jtok2 = sensor_obj[D_JSON_HUMIDITY])
    {
      value->type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
      value->data.push_back(jtok2.getFloat());
    }

    if(jtok2 = sensor_obj[D_JSON_PRESSURE])
    {
      value->type.push_back(SENSOR_TYPE_PRESSURE_ID);
      value->data.push_back(jtok2.getFloat());
    }

    ALOG_DBM( PSTR("Remote Read %d" ), (int)value->data[0]);
    //  
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
    //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
    //   #endif // ENABLE_LOG_LEVEL_DEBUG

    D_MQTT_COMMAND_HANDLED_COUNT_INC;

  }

}

void mRemoteDevice::Pre_Init(void){

  settings.fEnableSensor = true;
  settings.sensor_active_count = 1;
  
  if(settings.sensor_active_count){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "DHT Sensor Enabled"));
  }

}

void mRemoteDevice::Init(void){

}


void mRemoteDevice::EveryLoop(){
  
  // for (int sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){

  //   if(mTime::TimeReachedNonReset(&sensor[sensor_id].instant.tSavedMeasureClimate,1000)){
  //     if(!settings.sensor_active_count){ // Retry init if lost after found during boot
  //       init(); //search again
  //     }else{
  //         SplitTask_UpdateClimateSensors(sensor_id,DONTREQUIRE_COMPLETE);
  //         if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_DONE_ID){ // when its finished, reset timer
  //           sensor[sensor_id].instant.sUpdateClimateSensors=SPLIT_TASK_SUCCESS_ID;
  //           sensor[sensor_id].instant.tSavedMeasureClimate = millis();
  //         }else
  //         if(sensor[sensor_id].instant.sUpdateClimateSensors==SPLIT_TASK_ERROR_ID){ 
  //           sensor[sensor_id].instant.tSavedMeasureClimate = millis();//+5000; //backoff for 5 seconds
  //         }
  //     }

  //   }
  // }//end for

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/


uint8_t mRemoteDevice::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JBI->Add("SensorCount", settings.sensor_active_count);
    // JBI->Array_Start("Pin");
    //   JBI->Add(pin[0]);
    //   JBI->Add(pin[1]);
    // JBI->Array_End();
  return JsonBuilderI->End();

}


// /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

uint8_t mRemoteDevice::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  char buffer[50];

  JsonBuilderI->Start();

  sensors_reading_t* data = &sensor_data;
  
  JBI->Add(D_JSON_TEMPERATURE, data->GetValue(SENSOR_TYPE_TEMPERATURE_ID));
  JBI->Add(D_JSON_HUMIDITY,    data->GetValue(SENSOR_TYPE_RELATIVE_HUMIDITY_ID));
  JBI->Add(D_JSON_PRESSURE,    data->GetValue(SENSOR_TYPE_PRESSURE_ID));

  // for(uint8_t sensor_id=0;sensor_id<settings.sensor_active_count;sensor_id++){
  //   if((sensor[sensor_id].instant.ischanged || (json_level>JSON_LEVEL_IFCHANGED))
  //     //  &&(sensor[sensor_id].instant.isvalid)  
  //     ){

  //     JsonBuilderI->Level_Start_P(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_DHT_ID,sensor_id,buffer,sizeof(buffer)));   
  //       JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].instant.temperature);
  //       JsonBuilderI->Add(D_JSON_HUMIDITY,    sensor[sensor_id].instant.humidity);
  //       JsonBuilderI->Level_Start(D_JSON_ISCHANGEDMETHOD);
  //         JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
  //         JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].instant.ischangedtLast)/1000));
  //       JsonBuilderI->Level_End();   
  //     JsonBuilderI->Level_End(); 
  //   }

  // }
  //   for(int dht_id=0;dht_id<2;dht_id++){
  //   JBI->Level_Start(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_DHT_ID, dht_id, name_buffer_tmp, sizeof(name_buffer_tmp)));
  //     JBI->Add(D_JSON_TEMPERATURE, pCONT_dht->sensor[dht_id].instant.temperature);
  //     JBI->Add(D_JSON_HUMIDITY, pCONT_dht->sensor[dht_id].instant.humidity);
  //     JBI->Add(D_JSON_ISVALID, pCONT_dht->sensor[dht_id].instant.isvalid);
  //     JBI->Add(D_JSON_ISCHANGED, pCONT_dht->sensor[dht_id].instant.ischanged);
  //     // json1["iserrored"] = pCONT->mhs->climate.ptr->iserrored;
  //     // json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.seconds_nonreset-pCONT->mhs->climate.ptr->raw.captureupsecs);
  //     // json1["heatindex"] = pCONT->mhs->climate.ptr->raw.heatIndex; // DONT KNOW WHAT THEY ARE
  //     // json1["dewpoint"] = pCONT->mhs->climate.ptr->raw.dewPoint;
  //   JBI->Level_End();
  // }
  
  return JsonBuilderI->End();

}


void mRemoteDevice::MQTTHandler_Init(){

  struct handler<mRemoteDevice>* ptr;
  
  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRemoteDevice::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mRemoteDevice::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//pCONT_set->Settings.sensors.ifchanged_secs;
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mRemoteDevice::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mRemoteDevice::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mRemoteDevice::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mRemoteDevice::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_REMOTE_DEVICE_ID, handle, id);
  }
}

#endif
