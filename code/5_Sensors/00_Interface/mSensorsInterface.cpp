#include "mSensorsInterface.h" 

#ifdef USE_MODULE_SENSORS_INTERFACE 

int8_t mSensorsInterface::Tasker(uint8_t function, JsonParserObject obj){
  
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
  
  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;  
    case TASK_EVERY_SECOND:{
      // Serial.println(sizeof(sensors_reading_t));
      // Serial.println(pCONT_db18->GetSensorReading());
      // Serial.println(pCONT_dht->GetSensorReading());
      // Serial.println(pCONT_db18->test123());
      // Serial.println(pCONT_dht->test123());

      // Make nicer later with json command to enable and time period to show
      if(rt.tTicker_Splash_Sensors_To_Logs-- == 1)
      {
        // Measurement level feedback will be "DebugMore" and show level should be "Debug". "Info" should be reserved for essential stuff not in mqtt
        ALOG_DBM(PSTR(">>> Sensor Readings <<<"));
        pCONT->Tasker_Interface(TASK_SENSOR_SHOW_LATEST_LOGGED_ID);
        rt.tTicker_Splash_Sensors_To_Logs = 30 ; // reset
      }
      
  


      // for(auto& pmod:pCONT->pModule)
      // {
      //   for(int sensor_id=0;sensor_id<pmod->GetSensorCount();sensor_id++)
      //   {
      //     sensors_reading_t val;
      //     pmod->GetSensorReading(&val, sensor_id);
      //     if(val.type[0])
      //     {
      //       ALOG_TST(PSTR("%S %d|%d val.data[%d]=%d"),pmod->GetModuleFriendlyName(), sensor_id, pmod->GetSensorCount(), sensor_id, (int)val.GetValue(SENSOR_TYPE_TEMPERATURE_ID));
      //     }
      //   }
      // }
     
      //   pModule[switch_index]->Tasker(function, obj);

    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_EVENT_MOTION_STARTED_ID:
      CommandEvent_Motion(1);
    break;
    case TASK_EVENT_MOTION_ENDED_ID:
      CommandEvent_Motion(0);
    break; 
    #ifdef ENABLE_DEVFEATURE_BUTTONS_SEND_EVENT_MESSAGES    
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      MQTT_Report_Event_Button();
    break;
    #endif
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_SET_POWER_ID:
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
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


void mSensorsInterface::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
}


void mSensorsInterface::Init(void)
{
  rt.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  module_state.mode = ModuleStatus::Running;
}


void mSensorsInterface::EveryLoop()
{

}


#ifdef USE_MODULE_LIGHTS_INTERFACE
/**
 * @brief Single function that applies colour map to any temperature sensors.
 * In InterfaceSensors, the type of applied map can be selected to be added to unified method directly
 * 
 * @param temperature 
 * @return RgbwwColor 
 */
RgbwwColor mSensorsInterface::GetColourValueUsingMaps_ForUnifiedSensor(float temperature) // flag_unified_sensor_colour_heatmap_type
{

  if(flag_unified_sensor_colour_heatmap_type==1) //adjusted
  {
    return pCONT_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);
  }
  else //full colour-
  {
    return pCONT_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);
  }

}
#endif // USE_MODULE_LIGHTS_INTERFACE


void mSensorsInterface::CommandEvent_Motion(uint8_t event_type)
{

  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_motion_event_ifchanged.flags.SendNow = true;
  #endif // USE_MODULE_NETWORK_MQTT

}


void mSensorsInterface::MQTT_Report_Event_Button()
{

  char event_ctr[20]; memset(event_ctr,0,sizeof(event_ctr));

  JBI->Start();
  
    sprintf(event_ctr,"%s-%d","Button",pCONT_rules->event_triggered.device_id);
    JBI->Add("Event", event_ctr);

    JBI->Add("Device", pCONT_rules->event_triggered.device_id);
    JBI->Add("Function", "ButtonPress");
    JBI->Add("Task", "Button");
    JBI->Add("State", "SHORT_PRESS");// : "LONG_PRESS");    
    // JBI->Add("value", (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    JBI->Add("Duration", 0);//tSavedTimeSincePressOn);
  JBI->End();

  pCONT_mqtt->brokers[0]->publish_device("status/sensors_interface/event",JBI->GetBufferPtr(),false);

  /**
   * If event was serviced, then clear it
   * */
  #ifndef ENABLE_DEVFEATURE_PHASEOUT_CLEARING_EVENT
  pCONT_rules->Reset(&pCONT_rules->event_triggered);  // I need to remember the last event, so simply use another flag outside of the struct as waiting and clear that if needed
  #endif 

}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mSensorsInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["MQTT"].getObject()["ScanSensors"])
	{
		JBI->Start();

		pCONT->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		bool ready_to_send = JBI->End();

		if(!ready_to_send)
		{
			// Nothing was found, create new message
			JBI->Start();
				JBI->Add("SensorScan", "No Devices Found");
			ready_to_send = JBI->End();
		}


		if(ready_to_send)
		{			
    	ALOG_TST(PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
			pCONT_mqtt->brokers[0]->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		}

	}
    
}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}

/**
 * @brief 
 * 
 * Format with multiple locations? maybe should be flipped and the name added. ie
 * 
 * {"Temperature":{"Master Bedroom":23.580,"Landing":25.020}}
 * {"Humidity":{name:12,other:13} //therefore "for each key"
 * {"lum"..}
 * 
 * Step 1: Using module pointer, request all sensor readings which will be returned in sensors_reading_t
 * Step 2: Go through all sensors_reading_t values, if configured, then append their values to the JSON array with Key based on the module pointer
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    // JBI->Add("Redunction", mqtthandler_sensor_ifchanged.flags.FrequencyRedunctionLevel);
  // return 0;
  
  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  uint16_t type_id_adjusted = 0;

  for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      if( IS_MODULE_SENSOR_SUBMODULE( pmod->GetModuleUniqueID() ) )
      {
        //Get any sensors in module
        uint8_t sensors_available = pmod->GetSensorCount();
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        uint16_t unified_sensor_reporting_invalid_reading_timeout_seconds = tkr_set->Settings.unified_interface_reporting_invalid_reading_timeout_seconds;
        // ALOG_WRN(PSTR("reading_timeout_seconds %d"), unified_sensor_reporting_invalid_reading_timeout_seconds);
                  
        if(sensors_available) 
        {
          // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

          for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
          {
            sensors_reading_t val;
            pmod->GetSensorReading(&val, sensor_id);

            if(unified_sensor_reporting_invalid_reading_timeout_seconds != 0) // Check active
            {
                              
              if(val.timestamp)
              {
                if(tkr_time->RtcTime.valid) // Only enable timestamp checks when time is valid
                {
                  uint32_t sensor_elapsed_time = tkr_time->UtcTime() - val.timestamp;
                  if(sensor_elapsed_time) // If positive and NOT val.timestamp set to 0 as skipped
                  {
                    if(sensor_elapsed_time > unified_sensor_reporting_invalid_reading_timeout_seconds)
                    {
                      #ifdef ENABLE_DEVFEATURE_UNIFIED_REPORTING_SKIPPING_INVALID_TIMEOUT_READINGS
                      ALOG_WRN(PSTR("sensor time invalid %d > %d"), sensor_elapsed_time, unified_sensor_reporting_invalid_reading_timeout_seconds);
                      continue; // skip the result in this loop
                      #else
                      DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));
                      ALOG_WRN(PSTR("sensor_elapsed_time missing %S %s %d %d"), pmod->GetModuleName(), buffer, sensor_elapsed_time, unified_sensor_reporting_invalid_reading_timeout_seconds);
                      #endif
                    }
                  }
                }
              }
            }
            
            if(val.Valid())
            {

              /**
               * @brief Special cases
               * 
               */
              if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              {
                type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;              
              }
              else
              {
                type_id_adjusted = type_id;
              }

              if(val.isFloatWaiting_WithSensorType(type_id_adjusted))
              {
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                /**
                 * @brief Modify for special cases
                 * 
                 */
                if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
                {

                  sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);                
                  // Convert into colour
                  float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
                  #ifdef USE_MODULE_LIGHTS_INTERFACE
                  
                  // Only add sensor type if any has been found
                  if(flag_level_started != true)
                  {     
                    JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                    flag_level_started = true;
                    flag_level_ended_needed = true;
                  }
                  
                  RgbColor colour  = GetColourValueUsingMaps_ForUnifiedSensor(temperature);

                  JBI->Add_FV(
                    DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                    PSTR("\"%02X%02X%02X\""),
                    colour.R, colour.G, colour.B
                  );
                  #endif // USE_MODULE_LIGHTS_INTERFACE

                }
                /**
                 * @brief As read from sensor
                 * 
                 */
                else
                {
                  // Only add sensor type if any has been found
                  if(flag_level_started != true)
                  {     
                    JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                    flag_level_started = true;
                    flag_level_ended_needed = true;
                  }
                  
                  sensor_data = val.GetFloat(type_id);
                  JBI->Add(buffer, sensor_data);
                }
                
              }

              // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              // {
              //   DEBUG_LINE_HERE;
              // Serial.println(val.GetString(type_id));
              //     sensor_data_string = val.GetString(type_id);
              // Serial.println(sensor_data_string);
              // Serial.println(sensor_data_string.c_str());
              
              // }
              
              // sensor_data_string = val.GetString(type_id);
              // if(!sensor_data_string.equals("error"))
              // {

              #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
              if(val.HasString(sensor_id))
              {
              // if((sensor_data_string = val.GetString(type_id)) != "error" )
              // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
              // {

                sensor_data_string = val.GetString(type_id);

                
                // Only add sensor type if any has been found
                if(flag_level_started != true)
                {              
                  JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                  flag_level_started = true;
                  flag_level_ended_needed = true;
                }
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                JBI->Add(buffer, sensor_data_string.c_str());
                
              }
              #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

            }
          }

        }
      } // oNLY USE SENSOR MODULES

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }

  } // END sensor_type





  
//   // for (
//     uint16_t type_id = SENSOR_TYPE_TEMPERATURE_ID; 
//   // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
//   // {

//     // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
//     /**
//      * @brief Check by sensor reported type
//      **/
//     for(auto& pmod:pCONT->pModule)
//     {
//       //Get any sensors in module
//       uint8_t sensors_available = pmod->GetSensorCount();
//       // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
//       if(sensors_available)
//       {
//         // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

//         for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//         {
//           sensors_reading_t val;
//           pmod->GetSensorReading(&val, sensor_id);
          
//           if(val.Valid())
//           {

//             if(val.isFloatWaiting_WithSensorType(type_id))
//             {

//             // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
//             // {
//             // if(val.HasFloat(sensor_id))
//             // {

//               sensor_data = val.GetFloat(type_id);

//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {     
//                 JBI->Object_Start("TemperatureHeatMapBrightness");//PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


//               // Convert into colour
//               float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//               RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

//               JBI->Add_FV(
//                 DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
//                 PSTR("\"%02X%02X%02X\""),
//                 colour.R, colour.G, colour.B
//               );


//               // JBI->Add(buffer, sensor_data);
              
//             }


// // {
// //   "Temperature":{
// //     "BedroomDesk-DHT1":"00FF33",
// //     "BedroomDesk-DHT1":27.900,
// //     "BedroomDesk-DHT2":"00FF33",
// //     "BedroomDesk-DHT2":28.000,
// //     "BedroomDesk-BME":"00FF1D",
// //     "BedroomDesk-BME":28.750,
// //     "BedroomDesk-BME2":"00FF22",
// //     "BedroomDesk-BME2":28.510,
// //     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


//             // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
//             // {
//             //   DEBUG_LINE_HERE;
//             // Serial.println(val.GetString(type_id));
//             //     sensor_data_string = val.GetString(type_id);
//             // Serial.println(sensor_data_string);
//             // Serial.println(sensor_data_string.c_str());
            
//             // }
            
//             // sensor_data_string = val.GetString(type_id);
//             // if(!sensor_data_string.equals("error"))
//             // {

//             #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
//             if(val.HasString(sensor_id))
//             {
//             // if((sensor_data_string = val.GetString(type_id)) != "error" )
//             // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
//             // {

//               sensor_data_string = val.GetString(type_id);

              
//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {              
//                 JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

//               JBI->Add(buffer, sensor_data_string.c_str());
              
//             }
//             #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

//           }
//         }

//       }

//     } // END modules checking
    
//     /**
//      * @brief Only finish Level if type was added at least once
//      * 
//      * @return * Only 
//      */
//     if(flag_level_ended_needed)
//     {
//       JBI->Object_End();
//       flag_level_ended_needed = false;
//       flag_level_started = false;     // closed level
//     }





//   // for (
//     // uint16_t
//     type_id = SENSOR_TYPE_TEMPERATURE_ID; 
//   // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
//   // {

//     // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
//     /**
//      * @brief Check by sensor reported type
//      **/
//     for(auto& pmod:pCONT->pModule)
//     {
//       //Get any sensors in module
//       uint8_t sensors_available = pmod->GetSensorCount();
//       // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
//       if(sensors_available)
//       {
//         // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

//         for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//         {
//           sensors_reading_t val;
//           pmod->GetSensorReading(&val, sensor_id);
          
//           if(val.Valid())
//           {

//             if(val.isFloatWaiting_WithSensorType(type_id))
//             {

//             // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
//             // {
//             // if(val.HasFloat(sensor_id))
//             // {

//               sensor_data = val.GetFloat(type_id);

//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {     
//                 JBI->Level_Start_P(PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


//               // Convert into colour
//               float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//               RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);

//               JBI->Add_FV(
//                 DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
//                 PSTR("\"%02X%02X%02X\""),
//                 colour.R, colour.G, colour.B
//               );


//               // JBI->Add(buffer, sensor_data);
              
//             }


// // {
// //   "Temperature":{
// //     "BedroomDesk-DHT1":"00FF33",
// //     "BedroomDesk-DHT1":27.900,
// //     "BedroomDesk-DHT2":"00FF33",
// //     "BedroomDesk-DHT2":28.000,
// //     "BedroomDesk-BME":"00FF1D",
// //     "BedroomDesk-BME":28.750,
// //     "BedroomDesk-BME2":"00FF22",
// //     "BedroomDesk-BME2":28.510,
// //     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


//             // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
//             // {
//             //   DEBUG_LINE_HERE;
//             // Serial.println(val.GetString(type_id));
//             //     sensor_data_string = val.GetString(type_id);
//             // Serial.println(sensor_data_string);
//             // Serial.println(sensor_data_string.c_str());
            
//             // }
            
//             // sensor_data_string = val.GetString(type_id);
//             // if(!sensor_data_string.equals("error"))
//             // {

//             #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
//             if(val.HasString(sensor_id))
//             {
//             // if((sensor_data_string = val.GetString(type_id)) != "error" )
//             // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
//             // {

//               sensor_data_string = val.GetString(type_id);

              
//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {              
//                 JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

//               JBI->Add(buffer, sensor_data_string.c_str());
              
//             }
//             #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

//           }
//         }

//       }

//     } // END modules checking
    
//     /**
//      * @brief Only finish Level if type was added at least once
//      * 
//      * @return * Only 
//      */
//     if(flag_level_ended_needed)
//     {
//       JBI->Object_End();
//       flag_level_ended_needed = false;
//       flag_level_started = false;     // closed level
//     }

    JBI->Add("Rate", mqtthandler_sensor_ifchanged.tRateSecs);

  return JBI->End();
    
}




/**
 * @brief For any connected temperature sensors, display their colour as a full brightness rgb colour
 * */
uint8_t mSensorsInterface::ConstructJSON_SensorTemperatureColours(uint8_t json_level, bool json_appending){


  JBI->Start();

  // return 0;
  
  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  // for (
    uint16_t type_id = SENSOR_TYPE_TEMPERATURE_ID; 
  // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      //Get any sensors in module
      uint8_t sensors_available = pmod->GetSensorCount();
      // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
      if(sensors_available)
      {
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
        {
          sensors_reading_t val;
          pmod->GetSensorReading(&val, sensor_id);
          
          if(val.Valid())
          {

            if(val.isFloatWaiting_WithSensorType(type_id))
            {

            // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
            // {
            // if(val.HasFloat(sensor_id))
            // {

              sensor_data = val.GetFloat(type_id);

              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {     
                JBI->Object_Start(PM_TEMPERATURE_HEATMAP_RGBSTRING);//PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              // sensor_data = pCONT_debug->debug_data.input_float1;//map(pCONT_debug->debug_data.input_float1, 0,59, 0,70);

              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP // I need to make a version that does not require lighting at all, so simple colour conversion
              uint32_t colour  = GetColourValueUsingMaps_FullBrightness(temperature, 0);
              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                R32(colour), G32(colour), B32(colour)
              );
              #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

              // ALOG_INF(PSTR("%s %d \"%02X%02X%02X\""), buffer, (int)(sensor_data*1000), R32(colour), G32(colour), B32(colour));

              // JBI->Add(buffer, sensor_data);
              
            }


// {
//   "Temperature":{
//     "BedroomDesk-DHT1":"00FF33",
//     "BedroomDesk-DHT1":27.900,
//     "BedroomDesk-DHT2":"00FF33",
//     "BedroomDesk-DHT2":28.000,
//     "BedroomDesk-BME":"00FF1D",
//     "BedroomDesk-BME":28.750,
//     "BedroomDesk-BME2":"00FF22",
//     "BedroomDesk-BME2":28.510,
//     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


            // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
            // {
            //   DEBUG_LINE_HERE;
            // Serial.println(val.GetString(type_id));
            //     sensor_data_string = val.GetString(type_id);
            // Serial.println(sensor_data_string);
            // Serial.println(sensor_data_string.c_str());
            
            // }
            
            // sensor_data_string = val.GetString(type_id);
            // if(!sensor_data_string.equals("error"))
            // {

            #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
            if(val.HasString(sensor_id))
            {
            // if((sensor_data_string = val.GetString(type_id)) != "error" )
            // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
            // {

              sensor_data_string = val.GetString(type_id);

              
              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {              
                JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              JBI->Add(buffer, sensor_data_string.c_str());
              
            }
            #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

          }
        }

      }

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }





  // for (
    // uint16_t
    type_id = SENSOR_TYPE_TEMPERATURE_ID; 
  // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      //Get any sensors in module
      uint8_t sensors_available = pmod->GetSensorCount();
      // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
      if(sensors_available)
      {
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
        {
          sensors_reading_t val;
          pmod->GetSensorReading(&val, sensor_id);
          
          if(val.Valid())
          {

            if(val.isFloatWaiting_WithSensorType(type_id))
            {

            // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
            // {
            // if(val.HasFloat(sensor_id))
            // {

              sensor_data = val.GetFloat(type_id);

              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {     
                JBI->Level_Start_P(PM_TEMPERATURE_HEATMAP_ADJUSTED_BRIGHTNESS_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
              uint32_t colour  = GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                R32(colour), G32(colour), B32(colour)
              );
              #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP


              // JBI->Add(buffer, sensor_data);
              
            }


// {
//   "Temperature":{
//     "BedroomDesk-DHT1":"00FF33",
//     "BedroomDesk-DHT1":27.900,
//     "BedroomDesk-DHT2":"00FF33",
//     "BedroomDesk-DHT2":28.000,
//     "BedroomDesk-BME":"00FF1D",
//     "BedroomDesk-BME":28.750,
//     "BedroomDesk-BME2":"00FF22",
//     "BedroomDesk-BME2":28.510,
//     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


            // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
            // {
            //   DEBUG_LINE_HERE;
            // Serial.println(val.GetString(type_id));
            //     sensor_data_string = val.GetString(type_id);
            // Serial.println(sensor_data_string);
            // Serial.println(sensor_data_string.c_str());
            
            // }
            
            // sensor_data_string = val.GetString(type_id);
            // if(!sensor_data_string.equals("error"))
            // {

            #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
            if(val.HasString(sensor_id))
            {
            // if((sensor_data_string = val.GetString(type_id)) != "error" )
            // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
            // {

              sensor_data_string = val.GetString(type_id);

              
              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {              
                JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              JBI->Add(buffer, sensor_data_string.c_str());
              
            }
            #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

          }
        }

      }

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }









    






  // } // END sensor_type

  return JBI->End();

}



#ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
/**
 * @brief future global way to map colours using palette
 * 
 * Option in the future to apply different colours to the map.
 * 
 * Global temp colours reporting should have flag otpion to pick between full/adjusted brightness inside primary unified sensor.
 * I should still produce them all for the sub mqtt colours
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return uint32_t colour packed W,R,G,B 
 */
uint32_t mSensorsInterface::GetColourValueUsingMaps_AdjustedBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

    // Generate Hue and Brt values
    if(value<20){
      hue = 240;
      brt = 10;
    }else
    if((value>=20)&&(value<50)){
      hue = mSupport::mapfloat(value, 20,50, 180,0);
      brt = mSupport::mapfloat(value, 20,50, 10,100);
    }else
    if((value>=50)&&(value<60)){      
      hue = mSupport::mapfloat(value, 50,60, 359,345);
      brt = 100;
    }else
    if(value>=60){
      hue = 340;
      brt = 100;
    }

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;
  
  HsbToRgb(hue/360.0f,1.0f,1.0f,&r,&g,&b);

  return RGBW32(r,g,b,w);



}

/**
 * @brief maximum sat/brt, hue changes
 * 
 * In the future, simply create "profiles" that have common temp to colour mappings
 * ie "Hot Water", "Room Comfort"
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
uint32_t mSensorsInterface::GetColourValueUsingMaps_FullBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 100;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

  // Generate Hue and Brt values
  if(value<15){
    hue = 240;
    brt = 100;
  }else
  if((value>=15)&&(value<25)){  // BLUE to GREEN
    hue = mSupport::mapfloat(value, 15,25, 240,120);
  }else
  if((value>=25)&&(value<30)){  //         GREEN to YELLOW
    hue = mSupport::mapfloat(value, 25,30, 120,60);
  }else
  if((value>=30)&&(value<35)){  //                  YELLOW to ORANGE
    hue = mSupport::mapfloat(value, 30,35, 60,17);
  }else
  if((value>=35)&&(value<40)){  //                            ORANGE to RED
    hue = mSupport::mapfloat(value, 35,40, 17,0);
  }else
  if((value>=40)&&(value<50)){  //                                      RED
    hue = 0;
  }else
  if((value>=50)&&(value<60)){  //                                      RED to PINK
    hue = mSupport::mapfloat(value, 50,60, 360,340);
  }else
  if(value>=60){   // PINK
    hue = 340;
  }

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;

  // hue = 180;

  // char buffer[20];
  // mSupport::float2CString(value, JSON_VARIABLE_FLOAT_PRECISION_LENGTH, buffer);       
  // ALOG_INF(PSTR("value=%s, hue=%d"), buffer, hue);
  
  HsbToRgb((float)hue/360.0f,1.0f,1.0f,&r,&g,&b);
  // HsbToRgb(0.0f,1.0f,1.0f,&r,&g,&b);

  return  RGBW32(r,g,b,w);

}


void mSensorsInterface::HsbToRgb(float h, float s, float v, uint8_t* r8, uint8_t* g8, uint8_t* b8)
{
  float r, g, b;

  if (s == 0.0f)
  {
    r = g = b = v; // achromatic or black
  }
  else
  {
    if (h < 0.0f)
    {
      h += 1.0f;
    }
    else if (h >= 1.0f)
    {
      h -= 1.0f;
    }
    h *= 6.0f;
    int i = (int)h;
    float f = h - i;
    float q = v * (1.0f - s * f);
    float p = v * (1.0f - s);
    float t = v * (1.0f - s * (1.0f - f));
    switch (i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:
      r = v;
      g = p;
      b = q;
      break;
    }
  }

  *r8 = r * 255;
  *g8 = g * 255;
  *b8 = b * 255;

}

#endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

float mSensorsInterface::ConvertTemp(float c)
{
  float result = c;
  // if (!isnan(c) && Settings.flag_system.temperature_conversion) {
  //   result = c * 1.8 + 32;  // Fahrenheit
  // }
  return result;
}


char mSensorsInterface::TempUnit(void)
{
  return (tkr_set->Settings.flag_system.temperature_conversion) ? 'F' : 'C';
}


float mSensorsInterface::ConvertPressure(float p)
{
  // float result = p;

  // if (!isnan(p) && Settings.flag_system.pressure_conversion) {
  //   result = p * 0.75006375541921;  // mmHg
  // }
  // return result;
}

String mSensorsInterface::PressureUnit(void)
{
  // return (Settings.flag_system.pressure_conversion) ? String(D_UNIT_MILLIMETER_MERCURY) : String(D_UNIT_PRESSURE);
}


uint8_t mSensorsInterface::ConstructJSON_Motion_Event(uint8_t json_level, bool json_appending){

  char buffer[100];

  JBI->Start();

  /**
   * @brief Motion Event : PIR module
   **/
  #ifdef USE_MODULE_SENSORS_PIR
  if(pCONT_rules->event_triggered.module_id == pCONT_motion->GetModuleUniqueID())
  {
    uint16_t device_id   = pCONT_rules->event_triggered.device_id;
    uint16_t state_id = pCONT_rules->event_triggered.value.data[0];  

    JBI->Add(D_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( pCONT_motion->GetModuleUniqueID(), device_id, buffer, sizeof(buffer))); 
    JBI->Add("Time", tkr_time->GetTimeStr(tkr_time->Rtc.local_time).c_str());
    JBI->Add("UTCTime", tkr_time->Rtc.local_time);
    JBI->Add(D_EVENT, state_id ? "detected": "over");
    JBI->Add("Sensor", pCONT_motion->GetModuleName());

  }
  #endif // USE_MODULE_SENSORS_PIR

  return JBI->End();
    
}



    // JBI->Add("motion", 0);

    /*****
     * 
     * 
     * 
     * 
     * 
     * 
     * 2024
     * Keeping this may be a unified way that motion of any type is reported under interface, PIR, ultrasonics etc
     * "Motion" module should perhaps be renamed PIR
     * 
     * 
     * 
     */

    
  //   for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
  //   if(pir_detect[sensor_id].ischanged){ 
      
  //     pir_detect[sensor_id].ischanged = false;
      
  //     JBI->Add(D_LOCATION, DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
  //     JBI->Add(D_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
  //     JBI->Add(D_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
  //triggering sensor (via module id)
  // also module name
  /// also sensor index

/**
 * 
 * Module needs a way to provide known format of type motion, then GetMotion will return a struct with all the important info
 * Should maybe be called GetSensor and GetDriver and internally there are different
 * 
 * 
 * */



  //     //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
  //     //if any mtion flag remains, then set mqtt again
      
  //   }
  // }






// /**
//  * @brief MOTION Events, will replace all motion type events.
//  * The motion (PIR), ultrasonic change to trigger motion, will be controlled via the rule engine
//  * Thus, any state change (or later mqtt topic input or url etc) can be treated internally as a motion change, no longer a pin state
//  * 
//  * This should remain an option, so other sensors via rule can trigger a motion event (e.g. ultrasonics)
//  * Basic GPIO change (eg PIR) will be decoupled from sensors to remove complexity of this simple type
//  * 
//  * @note Information about trigger, will be contained within the stored event
//  * @param DeviceName gives the index to the stored location index from sensor list
//  * @param State gives the ON/Started (1) or OFF/Ended (0)
//  * */
// void mMotion::RulesEvent_Motion_Change()
// {

//   ALOG_INF(PSTR("RulesEvent_Motion_Change"));


//   // for(
// /**
//  * Rhis "rules" should be changed, unless rules are to be on all the time
//  * */


//     uint8_t sensor_id = pCONT_rules->rules[pCONT_rules->rules_active_index].command.device_id;

//     /**
//      * If command state is follow (now value 2), then use trigger.data[0] as destination.data[0]
//      * */
//     uint8_t trigger_state = pCONT_rules->rules[pCONT_rules->rules_active_index].trigger.value.data[0];
//     uint8_t command_state_in = pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];
//     uint8_t newevent_command_state_in = pCONT_rules->event_triggered.value.data[0];
//     uint8_t command_state_out = 0;

//     uint8_t current_module_id = pCONT_rules->event_triggered.module_id;

// // enum SwitchModeOptions_IDS {
// //   TOGGLE, 
// //   FOLLOW, 
// //   FOLLOW_INV, 
// //   PUSHBUTTON, 
// //   PUSHBUTTON_INV, 
// //   PUSHBUTTONHOLD, 
// //   PUSHBUTTONHOLD_INV, 
// //   PUSHBUTTON_TOGGLE, 
// //   MAX_SWITCH_OPTION
// // };

// // pCONT_rules->ShowRuleAddLogByIndex();
// // pCONT_rules->ShowRuleEvent_AddLog();

// //     if(command_state_in == SWITCHMODE_FOLLOW_ID)
// //     {
// //       ALOG_TST(PSTR("SWITCHMODE_FOLLOW_ID"));
// //     }
// // else{
// //       ALOG_TST(PSTR("ELSE SWITCHMODE_FOLLOW_ID"));

// // }

// // ALOG_TST(PSTR("trigger_state=%d"),trigger_state);
// // ALOG_TST(PSTR("command_state_in=%d"),command_state_in);
// // ALOG_TST(PSTR("newevent_command_state_in=%d"),newevent_command_state_in);
// // ALOG_TST(PSTR("command_state_out=%d"), command_state_out);

// // ALOG_INF( PSTR("\t\t\t\t\t current_module_id=%d"), current_module_id );


// switch(command_state_in)
// {
//   default: //force off
//     command_state_out = 0;
//     break;
//   case STATE_NUMBER_FOLLOW_ID: 
//     command_state_out = newevent_command_state_in;
//     ALOG_INF( PSTR("STATE_NUMBER_FOLLOW_ID command_state_out = %d"), command_state_out );
//     break;
//   case STATE_NUMBER_FOLLOW_INV_ID: 
//     command_state_out = newevent_command_state_in?0:1;
//     break;
//   case STATE_NUMBER_OFF_ID: 
//     command_state_out = 0;
//     break;
//   case STATE_NUMBER_ON_ID: 
//     command_state_out = 1;
//     break;
// }


// // ALOG_TST(PSTR("Bommand_state_out=%d"), command_state_out);

// char buffer[100];

// // ALOG_TST(PSTR("state=[%d->%d]\"%s\""), newevent_command_state_in, command_state_out, pCONT_sup->GetState_Name_by_ID(command_state_out, buffer, sizeof(buffer)));


//     // sensor_id<settings.sensors_active;sensor_id++)
//   // {

//   //   if(PIR_Detected(sensor_id)!=pir_detect[sensor_id].state)
//   //   {

// // 1 ie HIGH will ALWAYS mean active, the inversion should be handled on the trigger/switch side

//       pir_detect[sensor_id].state = command_state_out;//pCONT_rules->rules[pCONT_rules->rules_active_index].command.value.data[0];//PIR_Detected(sensor_id);
      
      
//       AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].state=%d %d %d %d %d"),
//       pir_detect[sensor_id].state, trigger_state, command_state_in, command_state_out, newevent_command_state_in);


//       if(pir_detect[sensor_id].state)
//       {
//         pir_detect[sensor_id].tDetectTime = millis(); 
//         pir_detect[sensor_id].detected_time = tkr_time->LocalTime();
//         pir_detect[sensor_id].isactive = true;

//         // #ifdef ENABLE_LOG_LEVEL_DEBUG
//         // AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].state=%d"),pir_detect[sensor_id].state);
//         // #endif
//         // AddLog(LOG_LEVEL_DEBUG,PSTR("pir_detect[sensor_id].detected_time=%d"),pir_detect[sensor_id].detected_time);
        
//         // #ifdef USE_MODULE_CORE_RULES
//         // pCONT_rules->New_Event(E M_MODULE_SENSORS_MOTION_ID, sensor_id);
//         // #endif
//         // pCONT->Tasker_Interface(TASK_EVENT_MOTION_STARTED_ID);

//       }
//       else
//       {
//         pir_detect[sensor_id].tEndedTime = millis();
//         pir_detect[sensor_id].detected_time = tkr_time->LocalTime();
//         pir_detect[sensor_id].isactive = false;

//         // #ifdef USE_MODULE_CORE_RULES
//         // pCONT_rules->New_Event(E M_MODULE_SENSORS_MOTION_ID, sensor_id);
//         // #endif
//         // pCONT->Tasker_Interface(TASK_EVENT_MOTION_ENDED_ID);

//       }

// /**
//  * @brief Remember what triggered this so the device name can be retrieved in sender
//  **/
//       pir_detect[sensor_id].device_name.unique_module_id = pCONT_rules->event_triggered.module_id;
//       pir_detect[sensor_id].device_name.device_id = pCONT_rules->event_triggered.device_id;
//       pir_detect[sensor_id].ischanged = true;
//       mqtthandler_sensor_ifchanged.flags.SendNow = true;
//   //   }
//   // }

//   ALOG_TST(PSTR(DEBUG_INSERT_PAGE_BREAK "MOTION Event %d"),sensor_id);

// }


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsInterface::MQTTHandler_Init(){

  struct handler<mSensorsInterface>* ptr;
 
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_temperature_colours;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_SensorTemperatureColours;
  mqtthandler_list.push_back(ptr);

  //motion events
  ptr = &mqtthandler_motion_event_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Motion_Event;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsInterface::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsInterface::MQTTHandler_Rate()
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
void mSensorsInterface::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif
