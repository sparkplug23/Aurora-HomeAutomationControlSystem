

#include "mPWM.h"

#ifdef USE_MODULE_DRIVERS_PWM


int8_t mPWM::Tasker(uint8_t function, JsonParserObject obj)
{

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

  switch(function){
    case TASK_LOOP: 
      EveryLoop();
    break;
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    case TASK_EVERY_50_MSECOND:{


    /**
     * @brief Copy neopixelanimator to create my own class, that uses template, and can either set delta to transition between numbers, or a time period
     * 
     */

      // if(pwm[0].value==0){
      //   dir = 1;
      // }else
      // if(pwm[0].value>=1023){
      //   dir = 0;
      // }

      // if(dir==1){
      //   pwm[0].value+=1;
      // }else{
      //   pwm[0].value-=1;
      // }

      
  // for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  // {
  //     analogWrite(pwm[i].pin,  pwm[i].value);

  // }

//     uint16_t pwmv = variable_blend->GetTarget();



// analogWrite(16,  pwmv);


      // analogWrite(pwm[0].pin, pwm[0].value);
    }
    break;
    case TASK_EVERY_FIVE_SECOND:
// ALOG_INF("Starting test blender");

//     var_blend->SetTarget(random(1,10));

//     var_blend->StartAnimation(0, 1000);//, nullptr);
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
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER:
      WebAppend_Root_Draw_Table();
    break; 
    case TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break; 
    #endif //USE_MODULE_NETWORK_WEBSERVER

  }

}



void mPWM::Pre_Init(void)
{

  // if(
  //   pCONT_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1_ID) && 
  //   pCONT_pins->PinUsed(GPIO_LDR_BASIC_ANALOG1_ID)
  // ){

  //   if(pCONT_pins->PinUsed(GPIO_LDR_BASIC_DIGITAL1_ID))
  //   {
  //     pinMode(pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID), INPUT);
  //   }
    
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pCONT_pins->PinUsed(GPIO_PWM1_ID,i))
    {
      pinMode(pCONT_pins->GetPin(GPIO_PWM1_ID,i), OUTPUT);   // This should be changed to configure the analog module, and then just read/return from it
      pwm[i].pin = pCONT_pins->GetPin(GPIO_PWM1_ID,i);

    }
  }



    settings.fEnableSensor = true;
  // }

}


void mPWM::Init(void)
{

  /**
   * @brief Note I could also add flag to change the scale here to much longer than 1 minute
   * 
   */
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pCONT_pins->PinUsed(GPIO_PWM1_ID,i))
    {
      pwm[i].blended_value = new LinearBlendVariable<uint16_t>(1, BLEND_DATA_MILLISECONDS);
    }
  }

  // var_blend_f  = new LinearBlendVariable<float>(1, BLEND_DATA_MILLISECONDS);
  // var_blend_u8 = new LinearBlendVariable<uint8_t>(1, BLEND_DATA_MILLISECONDS);

}



void mPWM::EveryLoop(void)
{

  // var_blend_f ->UpdateBlend();
  // var_blend_u8->UpdateBlend();

  
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pwm[i].blended_value != nullptr)
    {
      pwm[i].blended_value->UpdateBlend();
    }
  }

  /**
   * @brief Update PWM outputs
   * 
   */
  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {
    if(pCONT_pins->PinUsed(GPIO_PWM1_ID,i))
    {
      analogWrite(pwm[i].pin, pwm[i].blended_value->GetValue());
    }
  }


}


void mPWM::EverySecond(void)
{

  // ldr[0].analog_reading  = analogRead(pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID));
  // ldr[0].digital_reading = digitalRead(pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID));

  // ALOG_TST(PSTR("ldr[0].analog_reading =%d %d"), ldr[0].analog_reading, pCONT_pins->GetPin(GPIO_LDR_BASIC_ANALOG1_ID));
  // ALOG_TST(PSTR("ldr[0].digital_reading =%d %d"), ldr[0].digital_reading, pCONT_pins->GetPin(GPIO_LDR_BASIC_DIGITAL1_ID));

  // for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  // {
  //   ALOG_INF(PSTR("pwm[%d].blended_value->UpdateBlend() = %d"), i, pwm[i].blended_value->GetValue());
  // }

}




/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
void mPWM::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  char buffer[20];

  for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  {

    snprintf(buffer, sizeof(buffer), "PWM%d\0", i);
    
    if(jtok = obj["PWM_Manual"].getObject()[buffer]){
      pwm[i].value = jtok.getInt();
      ALOG_INF(PSTR("PWM%d=%d"), i, pwm[i].value);
    }
    
  }

  uint16_t blend_duration = 1000;


  if(jtok = obj["PWM_Manual"].getObject()["Duration"]){
    blend_duration = jtok.getInt();
    // var_blend->StartAnimation(0, 5000);
    // ALOG_INF(PSTR("GetTarget=%d"), var_blend->GetTarget());
  }



  // if(jtok = obj["PWM_Manual"].getObject()["Target_f"]){
  //   var_blend_f->SetTargetAndStartBlend(jtok.getInt(), blend_duration);
  //   ALOG_INF(PSTR("GetTarget=%d"), var_blend_f->GetTarget());
  // }
  // if(jtok = obj["PWM_Manual"].getObject()["Target_u8"]){
  //   var_blend_u8->SetTargetAndStartBlend(jtok.getInt(), blend_duration);
  //   ALOG_INF(PSTR("GetTarget=%d"), var_blend_u8->GetTarget());
  // }


/**
 * @brief 
 * New method of setting the PWM channels to change slowly over time, with ability later to set timescale (this can be changed at runtime?)
 * 
 */

    // uint16_t blend_duration_arr[MAX_PWM_PINS] = {0}; // default to 1 second for now

  // uint16_t blend_duration = 0; // default to 1 second for now

  // for(uint8_t i=0;i<MAX_PWM_PINS;i++)
  // {   
    
  //   blend_duration = 0; // default to 1 second for now

  //   snprintf(buffer, sizeof(buffer), "Duration%d\0", i);    
  //   if(jtok = obj["PWM_Blend"].getObject()[buffer]){
  //     blend_duration = jtok.getInt();
  //     ALOG_INF(PSTR("PWM%d blend_duration=%d"), i, blend_duration);
  //   }

  //   snprintf(buffer, sizeof(buffer), "Value%d\0", i);    
  //   if(jtok = obj["PWM_Blend"].getObject()[buffer]){
  //     pwm[i].blended_value->SetTargetAndStartBlend(jtok.getInt(), blend_duration);
  //     ALOG_INF(PSTR("PWM%d GetTarget()=%d for %d (ms)"), i, pwm[i].blended_value->GetTarget(), blend_duration);
  //   }   

  // }


  if(jtok = obj["PWM_Blend"]){ 

    ALOG_INF(PSTR("PWM_Blend is found"));

    JsonParserToken jtok_duration = 0; 
    JsonParserToken jtok_value = 0; 
    
    uint16_t duration_array[5] = {0};
    uint8_t duration_arrlen = 0;
    uint16_t value_array[5] = {0};
    uint8_t value_arrlen = 0;

    if(jtok_duration = jtok.getObject()["Duration"])
    {
        
      if(jtok_duration.isArray()){
        JsonParserArray arrobj = jtok_duration;
        for(auto v : arrobj) {
          if(duration_arrlen > 5){ break; }
          duration_array[duration_arrlen++] = v.getInt();
        }
      }

    }

    if(jtok_value = jtok.getObject()["Value"])
    {        
      if(jtok_value.isArray()){
        JsonParserArray arrobj = jtok_value;
        for(auto v : arrobj) {
          if(value_arrlen > 5){ break; }
          value_array[value_arrlen++] = v.getInt();
          // ALOG_INF(PSTR("PWM_Blend value_array[%d] =%d"), value_arrlen-1, value_array[value_arrlen-1]);
        }
      }
    }
    if(jtok_value = jtok.getObject()["ValuePercentage"])
    {        
      if(jtok_value.isArray()){
        JsonParserArray arrobj = jtok_value;
        for(auto v : arrobj) {
          if(value_arrlen > 5){ break; }
          value_array[value_arrlen] = map(v.getInt(), 0,100, pwm[value_arrlen].range_min, pwm[value_arrlen].range_max);
          // ALOG_INF(PSTR("PWM_Blend value_array[%d] =%d"), value_arrlen-1, value_array[value_arrlen-1]);
          value_arrlen++;;
        }
      }
    }

    /**
     * @brief Start blending
     * 
     */
    for(int i=0;i<5;i++)
    {
      pwm[i].blended_value->SetTargetAndStartBlend(value_array[i], duration_array[i]);
      ALOG_INF(PSTR("PWM%d GetTarget()=%d for %d (ms)"), i, pwm[i].blended_value->GetTarget(), pwm[i].blended_value->BlendDuration());
    }

  }








}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mPWM::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mPWM::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Array_Start("pins");
    for(uint8_t i=0;i<5;i++){ 
      JBI->Add(pwm[i].pin);
    }
    JBI->Array_End();

  
  

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mPWM::MQTTHandler_Init(){

  struct handler<mPWM>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mPWM::ConstructJSON_Settings;

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPWM::ConstructJSON_State;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mPWM::ConstructJSON_State;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mPWM::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mPWM::MQTTHandler_Rate()
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
void mPWM::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_WEBSERVER

void mPWM::WebAppend_Root_Draw_Table(){

  const char kTitle_TableTitles_Root[] = 
    "Switch 0" "|" 
    "Switch 1" "|" 
    "Switch 2" "|" 
    "Switch 3" "|" 
    "Switch 4" "|" 
    "Switch 5" "|" 
    "Switch 6" "|" 
    "Switch 7" "|" 
    "Switch 8" "|" ;

 tkr_web->WebAppend_Root_Draw_Table_dList(settings.switches_found,"switch_table", kTitle_TableTitles_Root); //add flag (or another function) that draws names with numbers after it

}

//append to internal buffer if any root messages table
void mPWM::WebAppend_Root_Status_Table(){

  char buffer[50];
  
  JBI->Array_Start("switch_table");// Class name
  for(int row=0;row<settings.switches_found;row++){
    JBI->Object_Start();
      JBI->Add("id",row);
      JBI->Add_FV("ih","\"%s\"", IsSwitchActive(row)?"On":"Off");
      if(IsSwitchActive(row)){
        JBI->Add("fc","#00ff00");
      }else{
        JBI->Add("fc","#ff0000");
      }
    
    JBI->Object_End();
  }
  JBI->Array_End();
  
}


#endif// USE_MODULE_NETWORK_WEBSERVER


#endif