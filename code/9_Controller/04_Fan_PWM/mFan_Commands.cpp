#include "mFan.h"

#ifdef USE_MODULE_CONTROLLER_FAN

void mFan::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];
  
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */

  if(jtok = obj[PM_FANSPEED_MANUAL]){
  //   if(jtok.isStr()){
  //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  //       CommandSet_PaletteID(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
    if(jtok.isNum()){
      CommandSet_FanSpeed_Manual(jtok.getInt());
      set_fan_pwm = jtok.getInt();
      set_fan_speed = map(jtok.getInt(),1,1023,0,3);
      data_buffer.isserviced++;
    }
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  

  if(jtok = obj[PM_FANSPEED]){
  //   if(jtok.isStr()){
  //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
  //       CommandSet_PaletteID(tmp_id);
  //       data_buffer.isserviced++;
  //     }
  //   }else
    if(jtok.isNum()){
      CommandSet_FanSpeed_Manual(map(jtok.getInt(),0,3,1,1023)); //fix
      set_fan_pwm = map(jtok.getInt(),0,3,1,1023);
      set_fan_speed = jtok.getInt();
      data_buffer.isserviced++;
    }
  //   #ifdef ENABLE_LOG_LEVEL_DEBUG
  //   ALOG_DBG(PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette_id, buffer, sizeof(buffer)));
  //   #endif // ENABLE_LOG_LEVEL_DEBUG
  }
  
  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}


void mFan::CommandSet_FanSpeed_Manual(uint16_t value){

  analogWrite(pin, value);

AddLog(LOG_LEVEL_DEV_TEST,PSTR("pwm %d value = %d"),pin,value);


} 

#endif // USE_MODULE_LIGHTS_INTERFACE
