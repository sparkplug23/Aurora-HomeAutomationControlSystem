/*
  mADCInternal.cpp
  Description: Uses the internal ADC for measurements of A0 on ESP8266, and all ESP32 channels

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

/**
 * Special method, which will use interrupt on pin change, to cause a new adc value recording
 * Test 1:
 *  - use physical manual pin change, to cause a adc reading from both adc_pins and display result on serial
 * 
 * 
 * */
#include "mADCInternal.h"

#ifdef USE_MODULE_SENSORS_ADC_INTERNAL

#ifdef ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER


#ifdef ESP32
const byte interruptPin = 25;
volatile int interruptCounter = 0;
// volatile double interruptExecuteTime = 0;
static int taskCore0 = 0;
static int taskCore1 = 1;
int numberOfInterrupts = 0;
uint16_t adc6 = 0;
uint16_t adc7 = 0;

#include <soc/sens_reg.h>
#include <soc/sens_struct.h>

  // uint16_t adc_value;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
RTC_DATA_ATTR uint16_t adcValue;
uint16_t _adcValue;
// uint16_t adcValue1;

uint16_t IRAM_ATTR adc1_get_raw_ram(adc1_channel_t channel) {
    SENS.sar_read_ctrl.sar1_dig_force = 0; // switch SARADC into RTC channel 
    SENS.sar_meas_wait2.force_xpd_sar = SENS_FORCE_XPD_SAR_PU; // adc_power_on
    // RTCIO.hall_sens.xpd_hall = false; //disable other peripherals
    SENS.sar_meas_wait2.force_xpd_amp = SENS_FORCE_XPD_AMP_PD; // channel is set in the convert function
    
	// disable FSM, it's only used by the LNA.
    SENS.sar_meas_ctrl.amp_rst_fb_fsm = 0; 
    SENS.sar_meas_ctrl.amp_short_ref_fsm = 0;
    SENS.sar_meas_ctrl.amp_short_ref_gnd_fsm = 0;
    SENS.sar_meas_wait1.sar_amp_wait1 = 1;
    SENS.sar_meas_wait1.sar_amp_wait2 = 1;
    SENS.sar_meas_wait2.sar_amp_wait3 = 1; 

    //set controller
    SENS.sar_read_ctrl.sar1_dig_force = false;      //RTC controller controls the ADC, not digital controller
    SENS.sar_meas_start1.meas1_start_force = true;  //RTC controller controls the ADC,not ulp coprocessor
    SENS.sar_meas_start1.sar1_en_pad_force = true;  //RTC controller controls the data port, not ulp coprocessor
    SENS.sar_touch_ctrl1.xpd_hall_force = true;     // RTC controller controls the hall sensor power,not ulp coprocessor
    SENS.sar_touch_ctrl1.hall_phase_force = true;   // RTC controller controls the hall sensor phase,not ulp coprocessor
    
    //start conversion
    SENS.sar_meas_start1.sar1_en_pad = (1 << channel); //only one channel is selected.
    while (SENS.sar_slave_addr1.meas_status != 0);
    SENS.sar_meas_start1.meas1_start_sar = 0;
    SENS.sar_meas_start1.meas1_start_sar = 1;
    while (SENS.sar_meas_start1.meas1_done_sar == 0);
    _adcValue = SENS.sar_meas_start1.meas1_data_sar; // set adc value!

    SENS.sar_meas_wait2.force_xpd_sar = SENS_FORCE_XPD_SAR_PD; // adc power off

    return _adcValue;
}




/************************************************************************************
******** External Pin Interrupt Triggers For ADC ************************************
 @note Helper functions, that need to be static. The singlton class instance allows setting
       a flag that is inside the class
       Hardcoded variables to make this faster for measurements
*************************************************************************************
*************************************************************************************/
void IRAM_ATTR ISR_External_Pin_ADC_Config_All_Trigger()
{
  DEBUG_ADC_ISR_EVENT_SET(LOW);
  pCONT_adc_internal->external_interrupt.flag_pin_active = true;

  /**
   * Capture both adc pins for 5 samples (no delay)
   * */
  mADCInternal::ISR_DUAL_CAPTURE* adc_p = &pCONT_adc_internal->isr_capture;

  if(adc_p->within_buffer_iter_counter < 40)
  {
    adc_p->adc_readings[adc_p->active_buffer_to_write_to_index].buffer_ch6[adc_p->within_buffer_iter_counter] = adc1_get_raw_ram(ADC1_CHANNEL_6);
    adc_p->adc_readings[adc_p->active_buffer_to_write_to_index].buffer_ch7[adc_p->within_buffer_iter_counter] = adc1_get_raw_ram(ADC1_CHANNEL_7);
    adc_p->within_buffer_iter_counter++;
  }

  DEBUG_ADC_ISR_EVENT_SET(HIGH);
}

#endif // ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER

#endif // ESP32


int8_t mADCInternal::Tasker(uint8_t function, JsonParserObject obj)
{

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
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;   
    case TASK_EVERY_SECOND:
    {
      Update_Channel1_ADC_Readings();
      int reading = adc1_get_raw(ADC1_CHANNEL_7);
      Serial.println(reading);
      average_DEMA.value = average_DEMA.filter->AddValue(reading); 
      Serial.println(average_DEMA.value);
    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
    //  parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
      break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      pCONT_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      pCONT_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      pCONT_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return FUNCTION_RESULT_SUCCESS_ID;

}


void mADCInternal::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;
  
  

#ifdef ESP32
  if(pCONT_pins->PinUsed(GPIO_ADC1_CH6_ID))
  {
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH6_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_6;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
    settings.fSensorCount++;
  }

  if(pCONT_pins->PinUsed(GPIO_ADC1_CH7_ID))
  {
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH7_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_7;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
    settings.fSensorCount++;
  }

  // Special pin set here
  if(pCONT_pins->PinUsed(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID))
  {
    external_interrupt.trigger_pin = pCONT_pins->GetPin(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID);
    external_interrupt.flag_enabled = true;
  }

  average_DEMA.alpha1 = 2.0f / (100.0f-1.0f);
  average_DEMA.alpha2 = 1-(2.0f / (100.0f-1.0f));
  average_DEMA.filter = new DoubleEMAFilter<float>(average_DEMA.alpha1, average_DEMA.alpha2);
  
#endif // ESP32


  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  }

}


void mADCInternal::Init(void){


  #ifdef ESP32
  #ifdef ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  if(external_interrupt.flag_enabled)
  {
    pinMode(external_interrupt.trigger_pin, INPUT);
    attachInterrupt(external_interrupt.trigger_pin, ISR_External_Pin_ADC_Config_All_Trigger, FALLING);
  }
  #endif // ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER

  // Configure all channel atten and width
  for(int i=0; i<settings.fSensorCount; i++)
  {
    switch(adc_config[i].input_pin)
    {
      // case 32:
      //   adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
      //   adc1_config_width(ADC_WIDTH_BIT_12);
      //   ALOG_TST(PSTR("ADC1_CHANNEL_4 set"));
      // break;
      case 34:
        adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        ALOG_TST(PSTR("ADC1_CHANNEL_6 set"));
      break;
      case 35:
        adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        ALOG_TST(PSTR("ADC1_CHANNEL_7 set"));
      break;
    }

  }
#endif // ESP32

}

void mADCInternal::EveryLoop(){
    
#ifdef ESP32
  if(external_interrupt.flag_pin_active)
  {
    external_interrupt.flag_pin_active = false;
    // Serial.println("external_interrupt.flag_pin_active");
    // Update_Channel1_ADC_Readings();
    // ALOG_TST(PSTR("adc_levels = \t%d\t%d %d"), readings[0].adc_level, readings[1].adc_level, isr_capture.active_buffer_to_write_to_index);
  } 

#endif // ESP32
  // if(isr_capture.active_buffer_to_write_to_index!=0){
  // ALOG_TST(PSTR("isr_capture.active_buffer_to_write_to_index=%d"),isr_capture.active_buffer_to_write_to_index);
  // }

}


void mADCInternal::Update_Channel1_ADC_Readings()
{

#ifdef ESP32
  #ifdef ENABLE_SMOOTHING_ON_ADC_READING
  for(int i = 0;i<settings.fSensorCount; i++)
  {
    samples.clear();

    // collect samples from ADC
    while(samples.size() < readings[i].adcSampleCount_){
      samples.push_back(adc1_get_raw((adc1_channel_t)adc_config[i].channel_id));
      ets_delay_us(1); // too long!?
    }
    // average the collected samples
    readings[i].adc_level = (std::accumulate(samples.begin(), samples.end(), 0) / samples.size());
    // ALOG_TST(PSTR("readings[%d].adc_level = %d"),i,readings[i].adc_level);

    readings[i].samples_between_resets++;

    // Add nice function that adds reading into memory
    if(readings[i].stored_values.index < STORED_VALUE_ADC_MAX){
      readings[i].stored_values.adc[readings[i].stored_values.index++] = readings[i].adc_level;
    }else{
      // ALOG_TST(PSTR("readings[%d].stored_values.index = %d OVERFLOW"),i,readings[i].stored_values.index);
      // readings[i].stored_values.index = 0;
    }

  }
  #else
    readings[0].adc_level = adc1_get_raw(ADC1_CHANNEL_6);
    readings[0].utc_measured_timestamp = tkr_time->UtcTime();
    ets_delay_us(1);
    readings[1].adc_level = adc1_get_raw(ADC1_CHANNEL_7);
    readings[1].utc_measured_timestamp = tkr_time->UtcTime();
    ALOG_INF(PSTR("adc_level = \t%d\t%d"), readings[0].adc_level, readings[1].adc_level);
  #endif

#endif // ESP32
}


uint8_t mADCInternal::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_SENSOR_COUNT, settings.fSensorCount);
  return JBI->End();

}

uint8_t mADCInternal::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  // if(!json_appending)
  // { 
    JBI->Start(); 
  // }

  char buffer[50];

  
    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_2,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);

    // ALOG_TST(PSTR("adc = [%d,%d,%d,%d,%d,%d,%d,%d]"),
    //   adc1_get_raw(ADC1_CHANNEL_0),
    //   adc1_get_raw(ADC1_CHANNEL_1),
    //   adc1_get_raw(ADC1_CHANNEL_2),
    //   adc1_get_raw(ADC1_CHANNEL_3),
    //   adc1_get_raw(ADC1_CHANNEL_4),
    //   adc1_get_raw(ADC1_CHANNEL_5),
    //   adc1_get_raw(ADC1_CHANNEL_6),
    //   adc1_get_raw(ADC1_CHANNEL_7)
    // );

    // Update_Channel1_ADC_Readings();
    
  JBI->Array_Start("chADC1");
  // for(int i=0;i<1;i++){
    JBI->Add(adc1_get_raw(ADC1_CHANNEL_7));//readings[i].adc_level);
  // }
  JBI->Array_End();

  if(average_DEMA.value>0)
  {
    JBI->Add("dma_smoothed", average_DEMA.value);
    JBI->Add("Percentage", constrain(map(average_DEMA.value, 1375,2280,  0,100),0,100)); //Inverse!
    JBI->Add("Percentage_Raw", map(average_DEMA.value, 1375,2280,  0,100) ); //Inverse!
  }
  
  JBI->Add("DigitalPin", digitalRead(23));

  // JBI->Array_Start("ADC1");
  // for(int i=0;i<8;i++){
  //   JBI->Add(adc1_get_raw((adc1_channel_t)i));
  // }
  // JBI->Array_End();

  // JBI->Array_Start("stored_values.index");
  // for(int i=0;i<2;i++){
  //   JBI->Add(readings[0].stored_values.index);
  // }
  // JBI->Array_End();
  // JBI->Array_Start("samples_between_resets");
  // for(int i=0;i<2;i++){
  //   JBI->Add(readings[i].samples_between_resets);
  // }
  // JBI->Array_End();

  // uint16_t send_size = 0;
  // send_size = 10; //STORED_VALUE_ADC_MAX

  // JBI->Array_Start("adc0");
  // for(int i=0;i<send_size;i++){
  //   JBI->Add(readings[0].stored_values.adc[i]);
  // }
  // JBI->Array_End();
  // JBI->Array_Start("adc1");
  // for(int i=0;i<send_size;i++){
  //   JBI->Add(readings[1].stored_values.adc[i]);
  // }
  // JBI->Array_End();

  JBI->End();

  
  return 5;//json_appending ? JBI->Length() : JBI->End();

}


#ifdef USE_MODULE_NETWORK_MQTT

void mADCInternal::MQTTHandler_Init(){

  struct handler<mADCInternal>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//pCONT_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;//FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} 

#endif // USE_MODULE_NETWORK_MQTT

#endif





    // readings[0].adc_level = adc1_get_raw(ADC1_CHANNEL_6);
    // ets_delay_us(1);
    // readings[1].adc_level = adc1_get_raw(ADC1_CHANNEL_7);



    // ALOG_TST(PSTR("adc_level = \t%d\t%d"),readings[0].adc_level,readings[1].adc_level);


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // int val = adc1_get_raw(ADC1_CHANNEL_4);
    // printf("4val%d\n", val );


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);
    // val = adc1_get_raw(ADC1_CHANNEL_7);
    // printf("7val%d\n", val );


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // val = adc1_get_raw(ADC1_CHANNEL_6);
    // printf("6val%d\n", val );

    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_2,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);

    // ALOG_TST(PSTR("adc = [%d,%d,%d,%d,%d,%d,%d,%d]"),
    //   adc1_get_raw(ADC1_CHANNEL_0),
    //   adc1_get_raw(ADC1_CHANNEL_1),
    //   adc1_get_raw(ADC1_CHANNEL_2),
    //   adc1_get_raw(ADC1_CHANNEL_3),
    //   adc1_get_raw(ADC1_CHANNEL_4),
    //   adc1_get_raw(ADC1_CHANNEL_5),
    //   adc1_get_raw(ADC1_CHANNEL_6),
    //   adc1_get_raw(ADC1_CHANNEL_7)
    // );


    //   int read_raw;
    //   adc2_config_channel_atten( ADC2_CHANNEL_6, ADC_ATTEN_0db );

    //   esp_err_t r = adc2_get_raw( ADC2_CHANNEL_6, ADC_WIDTH_12Bit, &read_raw);
    //   if ( r == ESP_OK ) {
    //       printf("%d\n", read_raw );
    //   } else if ( r == ESP_ERR_TIMEOUT ) {
    //       printf("ADC2 used by Wi-Fi.\n");
    //   }

// #include "mSensorsAnalog.h"

// #ifdef USE_MODULE_SENSORS_ANALOG

// // Make basic class, report adc via mqtt (for tree level)


// void mSensorsAnalog::Pre_Init(void){

//   // sensors_active = 0;

//   // // Using ++ means always the lowest index will be the active sensor, ie, PIR2 can be 0 when PIR1 is not defined
//   // if (pCONT_pins->GetPin(GPIO_PIR_1OF2_ID] < 99) {  // not set when 255
//   //   pin[sensors_active++] = pCONT_pins->GetPin(GPIO_PIR_1OF2_ID];
//   //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[GPIO_PIR1] %d"),pCONT_pins->GetPin(GPIO_PIR_1OF2_ID]);
//   // }
//   // if (pCONT_pins->GetPin(GPIO_PIR_2OF2_ID] < 99) {  // not set when 255
//   //   pin[sensors_active++] = pCONT_pins->GetPin(GPIO_PIR_2OF2_ID];
//   //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "pin[GPIO_PIR2] %d"),pCONT_pins->GetPin(GPIO_PIR_2OF2_ID]);
//   // }

//   // fEnableSensor = false;

//   // for(uint8_t sensor_id=0;sensor_id<sensors_active;sensor_id++){
//   //   if(pin[sensor_id]>=0){
//   //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_PIR "Pin Set %d"),pin[sensor_id]);
//   //     pinMode(pin[sensor_id],INPUT_PULLUP);
//   //     fEnableSensor = true; // true if any are active
//   //   }else{
//   //     AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_PIR "Pin Invalid %d"),pin[sensor_id]);
//   //   }
//   // }

//   pinMode(A0, INPUT);

// }


// void mSensorsAnalog::Init(){

  
// }


// void mSensorsAnalog::EveryLoop(){
  

//   // Use short timer to automatically clear event
//   //if > 1 sec
//   //clear struct
// }

// int8_t mSensorsAnalog::Tasker(uint8_t function, JsonParserObject obj){

//   // Run even when sensor is disabled (Will set fEnableSensor)
//   switch(function){
//     case TASK_PRE_INIT:
//       // Pre_Init();
//     break;
//   }
  
//   // if(!fEnableSensor){ 
//   //   //Serial.println("!fEnableSensor"); 
//   // return 0; }

//   switch(function){
//     /************
//      * INIT SECTION * 
//     *******************/
//     case TASK_INIT:
//       // Init();
//     break;
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//     case TASK_LOOP:
//     // Serial.println("TASK_LOOP");
//       // EveryLoop();
//     break;   
//     case TASK_EVERY_SECOND:
//       // ALOG_TST(PSTR("mSensorsAnalog"));
//     break;  
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     // case TASK_JSON_COMMAND_ID:
//     //   parse_JSONCommand(obj);
//     // break;
//     // case TASK_TEMPLATE_DEVICE_EXECUTE_LOAD:
//     //   // parsesub_JSONCommands();
//     // break;  
//     /************
//      * MQTT SECTION * 
//     *******************/
//     #ifdef USE_MODULE_NETWORK_MQTT
//     case TASK_MQTT_HANDLERS_INIT:
//     case TASK_MQTT_HANDLERS_RESET:
//       MQTTHandler_Init();
//     break;
//     case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
//       MQTTHandler_Rate();
//     break;
//     case TASK_MQTT_SENDER:
//       MQTTHandler_Sender();
//     break;
//     #endif //USE_MODULE_NETWORK_MQTT
//   }

// } // END function




// // //relay:1,[1,2],"all",["fountain,bulbs,led"]
// // void mSensorsAnalog::parse_JSONCommand(){ //parse_Command() and pass packet (topic/len/payload/len structure)
  
// //   // Check if instruction is for me
// //   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/motion")>=0){
// //       ALOG_INF(PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_RELAYS));
// //       pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
// //   }else{
// //     return; // not meant for here
// //   }

// //   // AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::parse_JSONCommand()");

// //   ALOG_DBG(PSTR(D_LOG_RELAYS "Command: " "\"%s\""),data_buffer.payload.ctr);

// //   StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
// //   DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
// //   JsonObject obj = doc.as<JsonObject>();

// //   //parsesub_JSONCommands(&obj);

// // }


// // // void mSensorsAnalog::parsesub_JSONCommands(JsonObject& _obj){

// // //   //If its null (nothing passed), try global parameter
// // //   if(_obj == nullptr){
// // //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "obj == nullptr"));
// // //     if(tkr_set->pObj != nullptr){
// // //       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "tkr_set->pObj != nullptr"));
// // //       _obj = tkr_set->pObj;
// // //     }else{
// // //       AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT "No Object"));
// // //       return;
// // //     }
// // //   }

// // //   // Create local dereferenced variable
// // //   JsonObject obj = (*_obj); 



// // // }




// // #ifdef USE_MODULE_NETWORK_MQTT

// /*********************************************************************************************************************************************
// ******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/


// uint8_t mSensorsAnalog::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

//   JBI->Start();
//     JBI->Add("analog", analogRead(A0));
//   return JBI->End();
  
// }



// // /************ CONSTRUCT JSON BUILDERS *****************************************************************************************************************************/

// uint8_t mSensorsAnalog::ConstructJSON_Sensor(uint8_t json_level){
  
//   JBI->Start();
//     JBI->Add("analog", analogRead(A0));

//     uint8_t percentage = mapvalue(analogRead(A0), 1024,320, 0,100);

//     JBI->Add("percentage", percentage);
//     JBI->Object_Start("limits");
//       JBI->Add("min", 1024);    
//       JBI->Add("max", 320);
//     JBI->Object_End();

//   return JBI->End();

// }


// /*********************************************************************************************************************************************
// ******** MQTT Stuff **************************************************************************************************************************************
// **********************************************************************************************************************************************
// ********************************************************************************************************************************************/

// void mSensorsAnalog::MQTTHandler_Init(){

//   struct handler<mSensorsAnalog>* ptr;

//   ptr = &mqtthandler_settings;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = SEC_IN_HOUR; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mSensorsAnalog::ConstructJSON_Settings;

//   ptr = &mqtthandler_sensor_ifchanged;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 60; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mSensorsAnalog::ConstructJSON_Sensor;
  
// } 


// // void mSey

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mSensorsAnalog::MQTTHandler_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mSensorsAnalog::MQTTHandler_Rate()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
//   }
// }

// /**
//  * @brief Check all handlers if they require action
//  * */
// void mSensorsAnalog::MQTTHandler_Sender()
// {
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
//   }
// }

// #endif



// #include "arduino.h"

// #include <driver/adc.h>

// #include "soc/rtc_wdt.h"

// #include <vector>

// /*
//  * This is an example to read analog data at high frequency using the I2S peripheral
//  * Run a wire between pins 27 & 32
//  * The readings from the device will be 12bit (0-4096) 
//  */
// #include <driver/i2s.h>


// // CREATE AS NEW DRIVER INSTEAD OF NORMAL ADCINTERNAL

// /**
//  * 
//  * Triggers when sample rate is reached?
//  */
// #define SAMPLES_BEFORE_SHOWING 1000
// #define I2S_SAMPLE_RATE 44100 // 2x my sample rate (though it may be mistimed, so I will want to use higher and use the last X samples)
// #define ADC_INPUT ADC1_CHANNEL_4 //pin 32
// // #define OUTPUT_PIN 27
// // #define OUTPUT_VALUE 2000
// // #define READ_DELAY 10 //microseconds

// uint16_t adc_reading;
// uint16_t adc_reading_left;
// uint16_t adc_reading_right;

// // i2s_pin_config_t pin_config = {
// //   .bck_io_num = 26, //this is BCK pin
// //   .ws_io_num = 25, // this is LRCK pin
// //   .data_out_num = -1, // this is DATA output pin
// //   .data_in_num = 32   //Not used
// // };


// void i2sInit()
// {
//    i2s_config_t i2s_config = {
//     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
//     .sample_rate =  I2S_SAMPLE_RATE,              // The format of the signal using ADC_BUILT_IN
//     .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
//     .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//     .communication_format = I2S_COMM_FORMAT_I2S_MSB,
//     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, //high priority 
//     .dma_buf_count = 10,
//     /**
//      * Samples to fill into DMA before triggering interrupt
//      * Filling with 10000, should give an interrupt every 1 second at 10000 sample rate
//      * */
//     .dma_buf_len = 10,//8,  //Interrupt level 1 = how many samples to take
//     .use_apll = false,
//     .tx_desc_auto_clear = false,
//     .fixed_mclk = 0
//    };
//    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
//    i2s_set_adc_mode(ADC_UNIT_1, ADC1_CHANNEL_4);
//   //  SET_PERI_REG_MASK(SYSCON_SARADC_CTRL2_REG, SYSCON_SARADC_SAR1_INV); //added
//    i2s_adc_enable(I2S_NUM_0);
// }
// //
// //uint16_t readings[10] = {0};
// //uint8_t reading_index = 0;

// bool flag_new_Value = false;

// void reader(void *pvParameters)
// {

//   uint32_t read_counter = 0;
//   uint64_t read_sum = 0;
//   uint64_t read_sum_left = 0;
//   uint64_t read_sum_right = 0;
// // The 4 high bits are the channel, and the data is inverted
//   uint16_t offset = (int)ADC_INPUT * 0x1000 + 0xFFF;
//   size_t bytes_read;
//   uint16_t buffer[10] = {0};

//   // std::vector<uint16_t> samples;

//   /**
//    * 
//    * This would be my interrupt to get the value
//    */
   
//   while(1)
//   {

//     /**
//      * Keep a rolling average of the last X adc_readings
//      * */

//     // delay(1000);

//     // samples.clear();
    
//     /**
//      * Try read from the DMA the last readings
//      * */
//     i2s_read(I2S_NUM_0, &buffer, sizeof(buffer), &bytes_read, 15); //portMAX_DELAY=15 here

//     if (bytes_read == sizeof(buffer))
//     {
//       // read_sum += offset - buffer[0];
//       // read_sum += offset - buffer[1];

//       // read_sum_left  += offset - buffer[0]; 
//       // read_sum_right  += offset - buffer[1]; 

//     // samples.clear();


//       uint64_t average = 0;
//       for(int i=0;i<10;i++)
//       {
//         average += offset - buffer[i];
//       }
//       average /= 10;

//       // read_counter++;
//       // Serial.printf("%d\t%d\t%d\t%d\t%d\t%d\n\r",
//       //   offset-buffer[0],
//       //   offset-buffer[1],offset -buffer[2],offset -buffer[3],offset -buffer[4],offset -buffer[5]);

//       Serial.printf("average=%d\n\r", average);



//     } else {
//       Serial.println("buffer empty");
//     }


    
// //     // i2s_read(I2S_NUM_0, (char*)i2s_read_buff,NUM_SAMPLES * sizeof(uint16_t), &bytes_read, portMAX_DELAY);
    
// //     /**
// //      * If enough bytes have been read 
// //      * */
// //     if (bytes_read == sizeof(buffer))
// //     {
// //       read_sum += offset - buffer[0];
// //       read_sum += offset - buffer[1];

// //       read_sum_left  += offset - buffer[0]; 
// //       read_sum_right  += offset - buffer[1]; 

// //       read_counter++;
// //     } else {
// //       Serial.println("buffer empty");
// //     }

// //     if(read_counter > (I2S_SAMPLE_RATE-10))
// //     {

      
// //       // Serial.printf("read_sum = %d, read_counter = %d, bytes_read=%d\n\r",(uint32_t)read_sum,(uint16_t)read_counter, bytes_read);


// //     }


// // //    if(reading_index<10)
// // //    {
// // //      readings
// // //
// // //      
// // //    }

// // //    Serial.println(read_counter);

// //     if (read_counter == I2S_SAMPLE_RATE) {

// //       /**
// //        * 
// //        * Only show when the samples have reached the desired count, ie how many samples
// //        */


// //       // Get final average
// //       adc_reading = read_sum / I2S_SAMPLE_RATE / 2;  // by 2 because of left and right channel

// //       adc_reading_left = read_sum_left / I2S_SAMPLE_RATE / 2;  // by 2 because of left and right channel
// //       adc_reading_right = read_sum_right / I2S_SAMPLE_RATE / 2;  // by 2 because of left and right channel

      
// // //      Serial.printf("avg: %d millis: ", adc_reading);
// //       // Serial.println("reseting to zero");
// //       read_counter = 0;
// //       read_sum = 0;
// //       read_sum_left= 0;
// //       read_sum_right= 0;

// //       flag_new_Value = true;
      
// //       /**
// //        * Only restart when needed
// //        */
// //     //  i2s_adc_disable(I2S_NUM_0);
// //     //  delay(1000);
// //     //  i2s_adc_enable(I2S_NUM_0);


      
// //     }
//   }

// }

// void setup() {
//   Serial.begin(115200);

  
//   // // Put a signal out on pin 
//   // uint32_t freq = ledcSetup(0, I2S_SAMPLE_RATE, 10);
//   // Serial.printf("Output frequency: %d\n", freq);
//   // ledcWrite(0, OUTPUT_VALUE/4);
//   // ledcAttachPin(OUTPUT_PIN, 0);

  
//   // Initialize the I2S peripheral
//   i2sInit();
//   // Create a task that will read the data
//   xTaskCreatePinnedToCore(reader, "ADC_reader", 2048, NULL, 1, NULL, 1);
  
// }

// void loop() {

//   if(flag_new_Value)
//   {
//     flag_new_Value = false;
//     Serial.printf("%d\tADC reading: %d........   %d:%d\n\r", millis(),  adc_reading, adc_reading_left, adc_reading_right);
//   }

// }





// //   #define DEBUG_PIN1_GPIO     23
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// // const byte interruptPin = 25;
// // volatile int interruptCounter = 0;
// // // volatile double interruptExecuteTime = 0;
// // static int taskCore0 = 0;
// // static int taskCore1 = 1;
// // int numberOfInterrupts = 0;
// // uint16_t adc6 = 0;
// // uint16_t adc7 = 0;

// // #include <soc/sens_reg.h>
// // #include <soc/sens_struct.h>

// //  int IRAM_ATTR local_adc1_read(int channel) {
// //     uint16_t adc_value;
// //     SENS.sar_meas_start1.sar1_en_pad = (1 << channel); // only one channel is selected
// //     while (SENS.sar_slave_addr1.meas_status != 0);
// //     SENS.sar_meas_start1.meas1_start_sar = 0;
// //     SENS.sar_meas_start1.meas1_start_sar = 1;
// //     while (SENS.sar_meas_start1.meas1_done_sar == 0);
// //     adc_value = SENS.sar_meas_start1.meas1_data_sar;
// //     return adc_value;
// // }

// //   uint16_t adc_value;

// // portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
// // RTC_DATA_ATTR uint16_t adcValue;
// // uint16_t _adcValue;
// // uint16_t adcValue1;

// // void IRAM_ATTR adc1_get_raw_ram(adc1_channel_t channel) {
// //     SENS.sar_read_ctrl.sar1_dig_force = 0; // switch SARADC into RTC channel 
// //     SENS.sar_meas_wait2.force_xpd_sar = SENS_FORCE_XPD_SAR_PU; // adc_power_on
// //     // RTCIO.hall_sens.xpd_hall = false; //disable other peripherals
// //     SENS.sar_meas_wait2.force_xpd_amp = SENS_FORCE_XPD_AMP_PD; // channel is set in the convert function
    
// // 	// disable FSM, it's only used by the LNA.
// //     SENS.sar_meas_ctrl.amp_rst_fb_fsm = 0; 
// //     SENS.sar_meas_ctrl.amp_short_ref_fsm = 0;
// //     SENS.sar_meas_ctrl.amp_short_ref_gnd_fsm = 0;
// //     SENS.sar_meas_wait1.sar_amp_wait1 = 1;
// //     SENS.sar_meas_wait1.sar_amp_wait2 = 1;
// //     SENS.sar_meas_wait2.sar_amp_wait3 = 1; 

// //     //set controller
// //     SENS.sar_read_ctrl.sar1_dig_force = false;      //RTC controller controls the ADC, not digital controller
// //     SENS.sar_meas_start1.meas1_start_force = true;  //RTC controller controls the ADC,not ulp coprocessor
// //     SENS.sar_meas_start1.sar1_en_pad_force = true;  //RTC controller controls the data port, not ulp coprocessor
// //     SENS.sar_touch_ctrl1.xpd_hall_force = true;     // RTC controller controls the hall sensor power,not ulp coprocessor
// //     SENS.sar_touch_ctrl1.hall_phase_force = true;   // RTC controller controls the hall sensor phase,not ulp coprocessor
    
// //     //start conversion
// //     SENS.sar_meas_start1.sar1_en_pad = (1 << channel); //only one channel is selected.
// //     while (SENS.sar_slave_addr1.meas_status != 0);
// //     SENS.sar_meas_start1.meas1_start_sar = 0;
// //     SENS.sar_meas_start1.meas1_start_sar = 1;
// //     while (SENS.sar_meas_start1.meas1_done_sar == 0);
// //     _adcValue = SENS.sar_meas_start1.meas1_data_sar; // set adc value!

// //     SENS.sar_meas_wait2.force_xpd_sar = SENS_FORCE_XPD_SAR_PD; // adc power off
// // }


// // void core1Task(){// void * pvParameters ){
// //       Serial.print("An interrupt has occurred. Total: ");
// //       Serial.print(numberOfInterrupts);
// //       // Serial.print(" at :");
// //       // Serial.print(interruptExecuteTime);
// //       Serial.print(" on Core :");
// //       Serial.println(_adcValue);
// //       // Serial.println(xPortGetCoreID());
// // }


// // void IRAM_ATTR handleInterrupt(){//void * pvParameters) {
// //   portENTER_CRITICAL_ISR(&mux);
// //   // Serial.print(" on Core :");
// //   DEBUG_PIN1_SET(0);
// //   /**
// //    * adc1_ch6
// //    * */
// //   adc1_get_raw_ram(ADC1_CHANNEL_6);
// //   adcValue1 = _adcValue;

// //   DEBUG_PIN1_SET(1);
// //   // interruptExecuteTime = millis(); // this is the important part for me
// //   interruptCounter++;
// //   portEXIT_CRITICAL_ISR(&mux);
// // }

 
// // void main_task(void *pvParameter)
// // {
  
// // rtc_wdt_protect_off();
// // rtc_wdt_disable();

// //   pinMode(interruptPin, INPUT_PULLUP);
// //   attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  
// //         adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );
// //         adc1_config_width(ADC_WIDTH_BIT_12);
// //         adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11 );
// //         adc1_config_width(ADC_WIDTH_BIT_12);

// //   while(1) {
// //     // printf( "Task1 %d\n\r", interruptCounter ); //CONFIG_HELLO_MESSAGE );
// //     core1Task();

// //     vTaskDelay(1000 / portTICK_RATE_MS);
// //   }

// // }


// // void setup() {
 
// //   Serial.begin(115200);
// //   Serial.println("Monitoring interrupts: ");
// // DEBUG_PIN1_INIT();


// // analogSetClockDiv(1);


// //     xTaskCreatePinnedToCore(&main_task, "main_task", 2048, NULL, 1, NULL, 1);
  
// // //  xTaskCreatePinnedToCore(
// // //                  handleInterrupt,   /* Function to implement the task */
// // //                  "handleinterrupt", /* Name of the task */
// // //                  10000,      /* Stack size in words */
// // //                  NULL,       /* Task input parameter */
// // //                  0,          /* Priority of the task */
// // //                  NULL,       /* Task handle. */
// // //                  taskCore0);  /* Core where the task should run */
// // //  xTaskCreatePinnedToCore(
// // //                  core1Task,   /* Function to implement the task */
// // //                  "core1Task", /* Name of the task */
// // //                  10000,      /* Stack size in words */
// // //                  NULL,       /* Task input parameter */
// // //                  0,          /* Priority of the task */
// // //                  NULL,       /* Task handle. */
// // //                  taskCore1);  /* Core where the task should run */
// // }
 
 
// // void loop() {
 
// //   if(interruptCounter>0){ // greater than 0 means there was a interrupt fired
// //        portENTER_CRITICAL(&mux);
// //       interruptCounter--; // decrease to reset the value
// //       portEXIT_CRITICAL(&mux);
// //       numberOfInterrupts++; // count the numbers of interrupts fired at this moment
// // }
// //   if (numberOfInterrupts >= 25){
// //     numberOfInterrupts = 0;
// //     esp_restart(); // restart example by cleaning all data and resetting MCU
// //   }
// // }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
   Copyright (c) 2020, my_name_here
   All rights reserved.
   https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf#uart-reg-summ
*/
