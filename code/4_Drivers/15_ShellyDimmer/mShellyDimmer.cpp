/*
    mShellyDimmer.cpp - Shelly Dimmer 2 support, created for Tasmota and Refactored for TBD_PROJECT_NAME
    
    Copyright (C) 2021  James Turton Created
    Copyright (C) 2021  Michael Refactored

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

#include "mShellyDimmer.h"

#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER

// /*********************************************************************************************/

void mShellyDimmer::init(void)
{

  #ifdef ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
    leading_edge = 1;
    CmndShdLeadingEdge(0);
  #endif

  transition.fader = new LinearBlendVariable<uint16_t>(1, BLEND_DATA_MILLISECONDS);

}

void mShellyDimmer::Pre_Init(){

  if (
    pCONT_pins->PinUsed(GPIO_SHELLY2_SHD_BOOT0_ID) && 
    pCONT_pins->PinUsed(GPIO_SHELLY2_SHD_RESET_INV_ID) &&
    pCONT_pins->PinUsed(GPIO_HWSERIAL0_RX_ID) && 
    pCONT_pins->PinUsed(GPIO_HWSERIAL0_TX_ID)
    ) {
      tkr_set->runtime.devices_present++;
    // TasmotaGlobal.light_type = LT_SERIAL1;
    settings.fEnableModule = true;
  }else{
    return;
  }
    
  buffer = (uint8_t *)malloc(SHD_BUFFER_SIZE);
  if (buffer != nullptr)
  {
    ShdSerial = new TasmotaSerial(pCONT_pins->GetPin(GPIO_HWSERIAL0_RX_ID), pCONT_pins->GetPin(GPIO_HWSERIAL0_TX_ID), 2, 0, SHD_BUFFER_SIZE);
    if (ShdSerial->begin(115200))
    {
      // hardware_serial_active = true;

      if (ShdSerial->hardwareSerial())
        pCONT_sup->ClaimSerial();

      ShdSerial->flush();

      ResetToAppMode();
      bool got_version = SendVersion();
      ALOG_INF(PSTR(SHD_LOGNAME "Shelly Dimmer Co-processor Version v%u.%u"), dimmer.version_major, dimmer.version_minor);
      GetSettings();
      SaveSettings();

      SendSettings();
      SyncState();
    }
  }

}


int8_t mShellyDimmer::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == TASK_PRE_INIT){
    Pre_Init();
  }else
  if(function == TASK_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
  
  





    break;
    case TASK_EVERY_50_MSECOND:

    if(transition.fader != nullptr)
    {
      uint16_t last_brightness = transition.fader->GetValue();
      transition.fader->UpdateBlend();

      uint16_t new_brightness = transition.fader->GetValue();
      if(new_brightness != last_brightness)
      {
        //update
        req_brightness = new_brightness;
        SetBrightnessReq();
        ALOG_INF(PSTR("UpdateBlend %d"), last_brightness, new_brightness);

      }

    }

    break;
    case TASK_EVERY_SECOND:  
      Poll();    
      SubTask_Power_Time_To_Remain_On_Seconds();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_SET_DEVICE_POWER:
      SetPower();
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    case TASK_EVENT_SET_POWER_ID:
      RulesEvent_Set_Power();
    break;
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
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker


/**
 * @note: Time a relay will remain ON
 * */
void mShellyDimmer::SubTask_Power_Time_To_Remain_On_Seconds()
{

  // Auto time off decounters
  if(timer_decounter.seconds == 1){ //if =1 then turn off and clear to 0
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_INF(PSTR(D_LOG_NEO "timer_decounter.seconds==1 and disable"));
    #endif       

    uint16_t new_brightness = 0;
    // req_brightness = map(brightness100, 0,100, 0,1000);
    // SetBrightnessReq();

    ALOG_INF(PSTR(D_LOG_NEO "START DIM DOWN timer_decounter.seconds==1 and disable"));


    
    transition.fader->SetTargetAndStartBlend(new_brightness, time_ms);

    // /**
    //  * @brief Also update the fader value
    //  * 
    //  */if(transition.fader != nullptr)
    // {
    //   transition.fader->SetCurrentValue(req_brightness);
    // }
    

    timer_decounter.seconds=0;

    mqtthandler_state_teleperiod.flags.SendNow = true; // To show cleared back to zero and off

  }else
  if(timer_decounter.seconds>1){ // if =1 then turn off and clear to 0
    timer_decounter.seconds--;   // decrease

    // uint8_t brightness100 = 100;
    // req_brightness = map(brightness100, 0,100, 0,1000);
    // SetBrightnessReq();
    
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_INF(PSTR(D_LOG_NEO "timer_decounter.seconds=%d dec"), timer_decounter.seconds);
    #endif

    mqtthandler_state_teleperiod.flags.SendNow = true;

  }else{
    //assumed off ie == 0
  }

}



#ifdef USE_MODULE_CORE_RULES

void mShellyDimmer::RulesEvent_Set_Power(){
  
  // ALOG_TST(PSTR("MATCHED RulesEvent_Set_Power"));

  uint8_t relay_index = tkr_rules->rules[tkr_rules->rules_active_index].command.device_id;
  uint8_t relay_state = tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[0];
  uint8_t brightness_on_value = tkr_rules->rules[tkr_rules->rules_active_index].command.value.data[1];

  // ALOG_TST(PSTR("MATCHED RulesEvent_Set_Power %d %d %d"),relay_index, relay_state, brightness_on_value);


  if(relay_state==2){
    if(req_brightness){
      req_brightness = 0;
    }else{
      req_brightness = map(brightness_on_value,0,100,0,1000);
    }
  }
  // ALOG_TST(PSTR("BBBBBBBBBBBBBMATCHED RulesEvent_Set_Power %d %d %d %d"),relay_index, relay_state, brightness_on_value, req_brightness);



  SetBrightness(req_brightness);

}
#endif // USE_MODULE_CORE_RULES


uint8_t mShellyDimmer::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();  

    JBI->Add_P(PM_TIME, 1000);

    // JBI->Add("serial_active",hardware_serial_active);

    JBI->Add("version_major",dimmer.version_major);
    JBI->Add("version_minor",dimmer.version_minor);
    JBI->Add("brightness",dimmer.brightness);
    JBI->Add("power",dimmer.power);
    JBI->Add("fade_rate",dimmer.fade_rate);

  return JBI->End();

}

uint8_t mShellyDimmer::ConstructJSON_State(uint8_t json_level, bool json_appending){
  
  JBI->Start();  

    JBI->Add_P(PM_BRIGHTNESS, map(dimmer.brightness,0,1000,0,100));
    JBI->Add("CommandGet_SecondsToRemainOn", CommandGet_SecondsToRemainOn());
    JBI->Add("CommandGet_SecondsToRemainOn_Active", timer_decounter.active);

  return JBI->End();

}


/*********************************************************************************************\
 * Internal Functions
\*********************************************************************************************/

bool mShellyDimmer::SerialSend(const uint8_t data[], uint16_t len)
{
  int retries = 3;

  ALOG_DBM(PSTR(SHD_LOGNAME "Tx Packet:"));
  
  while (retries--)
  {
    ShdSerial->write(data, len);
    ShdSerial->flush();

    // wait for any response
    uint32_t snd_time = millis();
    while(abs(millis()-snd_time)<SHD_ACK_TIMEOUT)
    // while (TimePassedSince(snd_time) < SHD_ACK_TIMEOUT)
    {
      if (SerialInput())
          return true;

      delay(1);
    }

    // timeout
    ALOG_ERR(PSTR(SHD_LOGNAME "ShdSerial send timeout"));
  }
  return false;

}


bool mShellyDimmer::SendCmd(uint8_t cmd, uint8_t *payload, uint8_t len)
{
  uint8_t data[4 + 72 + 3]; // maximum payload for 0x30 packet is 72
  uint16_t chksm;
  uint8_t pos = 0;

  data[0] = SHD_START_BYTE;
  data[1] = counter++;
  data[2] = cmd;
  data[3] = len;

  pos += 4;

  if (payload)
  {
    memcpy(data + 4, payload, len);
    pos += len;
  }

  // calculate checksum from id and onwards
  chksm = checksum(data + 1, 3 + len);
  data[pos++] = chksm >> 8;
  data[pos++] = chksm & 0xff;
  data[pos++] = SHD_END_BYTE;

  return SerialSend(data, pos);
}


void mShellyDimmer::SetBrightnessReq()
{
  // Payload format:
  // [0-1] Brightness (%) * 10
  
  #ifdef ENABLE_DEVFEATURE_SHELLY_DIMMER_FORCED_BRIGHTNESS_TO_REQUESTED
  dimmer.brightness = req_brightness; // forced until I redo the commands to my own
  #endif

  uint8_t payload[SHD_SWITCH_SIZE];

  payload[0] = req_brightness & 0xff;
  payload[1] = req_brightness >> 8;

  SendCmd(SHD_SWITCH_CMD, payload, SHD_SWITCH_SIZE);
}

void mShellyDimmer::SetBrightness(uint16_t brightness)
{
  // Payload format:
  // [0-1] Brightness (%) * 10

  uint8_t payload[SHD_SWITCH_SIZE];

  payload[0] = brightness & 0xff;
  payload[1] = brightness >> 8;

  SendCmd(SHD_SWITCH_CMD, payload, SHD_SWITCH_SIZE);
}


void mShellyDimmer::SetBrightnessFade()
{
  uint16_t delta = 0;
  if (req_brightness > dimmer.brightness)
    delta = (req_brightness - dimmer.brightness) * 0.8;
  else
    delta = (dimmer.brightness - req_brightness) * 0.8;

  // Payload format:
  // [0-1] Brightness (%) * 10
  // [2-3] Delta brightness (%) * 8
  // [4-5] 0?? ToDo(jamesturton): Find out what this word is!

  uint8_t payload[SHD_SWITCH_FADE_SIZE];

  payload[0] = req_brightness & 0xff;
  payload[1] = req_brightness >> 8;

  payload[2] = delta & 0xff;
  payload[3] = delta >> 8;

  payload[4] = 0;
  payload[5] = 0;

  SendCmd(SHD_SWITCH_FADE_CMD, payload, SHD_SWITCH_FADE_SIZE);
}


void mShellyDimmer::SendSettings()
{
  // as specified in STM32 assembly
  uint16_t fade_rate = req_fade_rate;
  if (fade_rate > 100)
      fade_rate = 100;

  // Payload format:
  // [0-1] Brightness (%) * 10
  // [2-3] Leading / trailing edge (1=leading, 2=trailing) ToDo(jamesturton): Are there any other values this can take?
  // [4-5] Fade rate (units unknown) ToDo(jamesturton): Find fade rate units
  // [6-7] Warm up brightness (%) * 10
  // [8-9] Warm up duration (ms)

  uint8_t payload[SHD_SETTINGS_SIZE];

  payload[0] = req_brightness & 0xff;
  payload[1] = req_brightness >> 8;

  payload[2] = leading_edge & 0xff;
  payload[3] = leading_edge >> 8;

  payload[4] = fade_rate & 0xff;
  payload[5] = fade_rate >> 8;

  payload[6] = warmup_brightness & 0xff;
  payload[7] = warmup_brightness >> 8;

  payload[8] = warmup_time & 0xff;
  payload[9] = warmup_time >> 8;

  SendCmd(SHD_SETTINGS_CMD, payload, SHD_SETTINGS_SIZE);
}

void mShellyDimmer::SendWarmup()
{
  // Payload format:
  // [0-1] Warm up brightness (%) * 10
  // [2-3] Warm up duration (ms)

  uint8_t payload[SHD_WARMUP_SIZE];

  payload[0] = warmup_brightness & 0xff;
  payload[1] = warmup_brightness >> 8;

  payload[2] = warmup_time & 0xff;
  payload[3] = warmup_time >> 8;

  SendCmd(SHD_WARMUP_CMD, payload, SHD_WARMUP_SIZE);
}


void mShellyDimmer::SendCalibration(uint16_t brightness, uint16_t func, uint16_t fade_rate)
{
  // Payload format:
  // ??? ToDo(jamesturton): Find calibration payload format!
  uint8_t payload[SHD_CALIBRATION_SIZE];

  memset(payload, 0, sizeof(payload));

  SendCmd(SHD_CALIBRATION1_CMD, payload, SHD_CALIBRATION_SIZE);
  SendCmd(SHD_CALIBRATION2_CMD, payload, SHD_CALIBRATION_SIZE);
}


bool mShellyDimmer::SyncState()
{
  ALOG_DBM(PSTR(SHD_LOGNAME "ShdSerial %p"), ShdSerial);
  ALOG_DBM(PSTR(SHD_LOGNAME "Set Brightness Want %d, Is %d"), req_brightness, dimmer.brightness);
  
  if (!ShdSerial)
      return false;

#ifdef SHELLY_HW_DIMMING
  // TODO(jamesturton): HW dimming seems to conflict with SW dimming. See how
  // we can disbale SW dimming when using HW dimming.
  if (Settings.light_speed != dimmer.fade_rate)
  {
      ShdSetBrightnessFade();
      ShdDebugState();
  }
  else
#endif // SHELLY_HW_DIMMING
  if (req_brightness != dimmer.brightness)
  {
    SetBrightness();
    DebugState();
  }

  return true;
}

void mShellyDimmer::DebugState()
{
#ifdef SHELLY_DIMMER_DEBUG
        ALOG_DBG(PSTR(SHD_LOGNAME "MCU v%d.%d, Brightness:%d(%d%%), Power:%d, Fade:%d"),
                            dimmer.version_major, dimmer.version_minor,
                            dimmer.brightness,
                            map(dimmer.brightness, 0, 1000, 0, 100),
                            dimmer.power,
                            dimmer.fade_rate);
#endif  // SHELLY_DIMMER_DEBUG
}

bool mShellyDimmer::PacketProcess(void)
{
  uint8_t pos = 0;
  uint8_t id, cmd, len;
  bool ret = false;

  if (buffer[pos++] != SHD_START_BYTE)
    return false;

  id = buffer[pos++];
  cmd = buffer[pos++];
  len = buffer[pos++];

  switch (cmd)
  {
    case SHD_POLL_CMD:{
      // 1 when returning fade_rate, 0 when returning wattage, brightness?
      uint16_t unknown_0 = buffer[pos + 1] << 8 |
              buffer[pos + 0];

      uint16_t brightness = buffer[pos + 3] << 8 |
              buffer[pos + 2];

      uint32_t wattage_raw = buffer[pos + 7] << 24 |
              buffer[pos + 6] << 16 |
              buffer[pos + 5] << 8 |
              buffer[pos + 4];

      uint32_t voltage_raw = buffer[pos + 11] << 24 |
              buffer[pos + 10] << 16 |
              buffer[pos + 9] << 8 |
              buffer[pos + 8];

      uint32_t current_raw = buffer[pos + 15] << 24 |
              buffer[pos + 14] << 16 |
              buffer[pos + 13] << 8 |
              buffer[pos + 12];

      uint32_t fade_rate = buffer[pos + 16];

      float wattage = 0;
      if (wattage_raw > 0)
          wattage = 880373 / (float)wattage_raw;

      float voltage = 0;
      if (voltage_raw > 0)
          voltage = 347800 / (float)voltage_raw;

      float current = 0;
      if (current_raw > 0)
          current = 1448 / (float)current_raw;

// #ifdef USE_ENERGY_SENSOR
//                 Energy.active_power[0] = wattage;
//                 Energy.voltage[0] = voltage;
//                 Energy.current[0] = current;
//                 Energy.apparent_power[0] = voltage * current;
//                 if ((voltage * current) > wattage)
//                     Energy.reactive_power[0] = sqrt((voltage * current) * (voltage * current) - wattage * wattage);
//                 else
//                     Energy.reactive_power[0] = 0;
//                 if (wattage > (voltage * current))
//                     Energy.power_factor[0] = 1;
//                 else if ((voltage * current) == 0)
//                     Energy.power_factor[0] = 0;
//                 else
//                     Energy.power_factor[0] = wattage / (voltage * current);

//                 if (last_power_check > 10 && Energy.active_power[0] > 0)
//                 {
//                     float kWhused = (float)Energy.active_power[0] * (Rtc.utc_time - last_power_check) / 36;
// #ifdef SHELLY_DIMMER_DEBUG
//                     ALOG_DBG(PSTR(SHD_LOGNAME "Adding %i mWh to todays usage from %lu to %lu"), (int)(kWhused * 10), last_power_check, Rtc.utc_time);
// #endif  // USE_ENERGY_SENSOR
//                     Energy.kWhtoday += kWhused;
//                     EnergyUpdateToday();
//                 }
//                 last_power_check = Rtc.utc_time;
// #endif  // USE_ENERGY_SENSOR

        ALOG_DBM( PSTR(SHD_LOGNAME "ShdPacketProcess: Brightness:%d Power:%lu Voltage:%lu Current:%lu Fade:%d"), brightness, wattage_raw, voltage_raw, current_raw, fade_rate);

        dimmer.brightness = brightness;
        dimmer.power = wattage_raw;
        dimmer.fade_rate = fade_rate;
      }
      break;
  case SHD_VERSION_CMD:
      {

        ALOG_DBM( PSTR(SHD_LOGNAME "ShdPacketProcess: Version: %u.%u"), buffer[pos + 1], buffer[pos]);
        
        dimmer.version_minor = buffer[pos];
        dimmer.version_major = buffer[pos + 1];
      }
      break;
  case SHD_SWITCH_CMD:
  case SHD_SWITCH_FADE_CMD:
  case SHD_SETTINGS_CMD:
  case SHD_WARMUP_CMD:
  case SHD_CALIBRATION1_CMD:
  case SHD_CALIBRATION2_CMD:
      {
          ret = (buffer[pos] == 0x01);
      }
      break;
  }

  return ret;
}

void mShellyDimmer::ResetToAppMode()
{
#ifdef SHELLY_DIMMER_DEBUG
    ALOG_DBG(PSTR(SHD_LOGNAME "Request co-processor reset in app mode"));
#endif  // SHELLY_DIMMER_DEBUG

  pinMode(pCONT_pins->GetPin(GPIO_SHELLY2_SHD_RESET_INV_ID), OUTPUT);
  digitalWrite(pCONT_pins->GetPin(GPIO_SHELLY2_SHD_RESET_INV_ID), LOW);
  pinMode(pCONT_pins->GetPin(GPIO_SHELLY2_SHD_BOOT0_ID), OUTPUT);
  digitalWrite(pCONT_pins->GetPin(GPIO_SHELLY2_SHD_BOOT0_ID), LOW);

  delay(50);

  // clear in the receive buffer
  while (Serial.available())
      Serial.read();

  digitalWrite(pCONT_pins->GetPin(GPIO_SHELLY2_SHD_RESET_INV_ID), HIGH); // pull out of reset
  delay(50); // wait 50ms fot the co-processor to come online
}

void mShellyDimmer::Poll(void)
{

  ALOG_DBM( PSTR(SHD_LOGNAME "Poll"));
  
  if(!ShdSerial)
    return;

  SendCmd(SHD_POLL_CMD, 0, 0);
  SyncState();
}

bool mShellyDimmer::SendVersion(void)
{
  #ifdef SHELLY_DIMMER_DEBUG
  ALOG_INF(PSTR(SHD_LOGNAME "Sending version command"));
  #endif  // SHELLY_DIMMER_DEBUG
  return SendCmd(SHD_VERSION_CMD, 0, 0);
}

void mShellyDimmer::GetSettings(void)
{
  char parameters[32];
  // req_brightness      = 0;
  leading_edge        = 0;
  // req_fade_rate       = 0;
  warmup_brightness   = 0;
  warmup_time         = 0;
//     if (strstr(SettingsText(SET_SHD_PARAM), ",") != nullptr)
//     {
// #ifdef SHELLY_DIMMER_DEBUG
//         ALOG_INF(PSTR(SHD_LOGNAME "Loading params: %s"), SettingsText(SET_SHD_PARAM));
// #endif  // SHELLY_DIMMER_DEBUG
//         // req_brightness      = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 1));
//         leading_edge        = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 2));
//         // req_fade_rate       = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 3));
//         warmup_brightness   = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 4));
//         warmup_time         = atoi(subStr(parameters, SettingsText(SET_SHD_PARAM), ",", 5));
//     }
}

void mShellyDimmer::SaveSettings(void)
{
  char parameters[32];
  snprintf_P(parameters, sizeof(parameters), PSTR("%d,%d,%d,%d,%d"),
              req_brightness, leading_edge, req_fade_rate, warmup_brightness, warmup_time);
  // SettingsUpdateText(SET_SHD_PARAM, parameters);
}


bool mShellyDimmer::SerialInput(void)
{
  while (ShdSerial->available())
  {
    yield();
    uint8_t serial_in_byte = ShdSerial->read();
    buffer[byte_counter] = serial_in_byte;

    int check = check_byte();

    if (check > 1)
    {
      // finished
#ifdef SHELLY_DIMMER_DEBUG
      byte_counter++;
      ALOG_DBM( PSTR(SHD_LOGNAME "Rx Packet:"));
      // AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, byte_counter);
#endif  // SHELLY_DIMMER_DEBUG
      byte_counter = 0;

      PacketProcess();

      return true;
    }
    else if (check == 0)
    {
      // wrong data
#ifdef SHELLY_DIMMER_DEBUG
      ALOG_DBG(PSTR(SHD_LOGNAME "Byte %i of received data frame is invalid. Rx Packet:"), byte_counter);
      byte_counter++;
      // AddLogBuffer(LOG_LEVEL_DEBUG_MORE, buffer, byte_counter);
#endif  // SHELLY_DIMMER_DEBUG
      byte_counter = 0;
    }
    else
    {
        byte_counter++;
    }

  }
  return false;
}

/*********************************************************************************************\
 * Helper Functions
\*********************************************************************************************/

uint16_t mShellyDimmer::checksum(uint8_t *buf, int len)
{
    uint16_t chksm = 0;
    for (uint8_t i = 0; i < len; i++)
        chksm += buf[i];
    return chksm;
}

int mShellyDimmer::check_byte()
{
    uint8_t index = byte_counter;
    uint8_t byte = buffer[index];

    if (index == 0)
        return byte == SHD_START_BYTE;

    if (index < 4)
        return 1;

    uint8_t data_length = buffer[3];
    if ((4 + data_length + 3) > SHD_BUFFER_SIZE)
        return 0;

    if (index < 4 + data_length + 1)
        return 1;

    if (index == 4 + data_length + 1)
    {
        uint16_t chksm = (buffer[index - 1] << 8 | buffer[index]);
        uint16_t chksm_calc = checksum(&buffer[1], 3 + data_length);
        if (chksm != chksm_calc)
        {
#ifdef SHELLY_DIMMER_DEBUG
            ALOG_DBG(PSTR(SHD_LOGNAME "Checksum: %x calculated: %x"), chksm, chksm_calc);
#endif  // SHELLY_DIMMER_DEBUG
            return 0;
        }

        return 1;
    }

    if (index == 4 + data_length + 2 && byte == SHD_END_BYTE)
        return index;

    return 0;
}



/*********************************************************************************************\
 * SHD firmware update Functions
\*********************************************************************************************/

#ifdef SHELLY_FW_UPGRADE

void ShdResetToDFUMode()
{
#ifdef SHELLY_DIMMER_DEBUG
    ALOG_DBG(PSTR(SHD_LOGNAME "Request co-processor reset in dfu mode"));
#endif  // SHELLY_DIMMER_DEBUG

    pinMode(Pin(GPIO_SHELLY_DIMMER_RST_INV), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), LOW);

    pinMode(Pin(GPIO_SHELLY_DIMMER_BOOT0), OUTPUT);
    digitalWrite(Pin(GPIO_SHELLY_DIMMER_BOOT0), HIGH);

    delay(50);

    // clear in the receive buffer
    while (ShdSerial.available())
        ShdSerial.read();

    digitalWrite(Pin(GPIO_SHELLY_DIMMER_RST_INV), HIGH); // pull out of reset
    delay(50); // wait 50ms fot the co-processor to come online
}

bool ShdUpdateFirmware(uint8_t* data, uint32_t size)
{
#ifdef SHELLY_DIMMER_DEBUG
    ALOG_DBG(PSTR(SHD_LOGNAME "Update firmware"));
#endif  // SHELLY_DIMMER_DEBUG

    bool ret = true;
    stm32_t *stm = stm32_init(&ShdSerial, STREAM_SERIAL, 1);
    if (stm)
    {
        off_t   offset = 0;
        uint8_t   buffer[256];
        unsigned int  len;
        const uint8_t *p_st = data;
        uint32_t  addr, start, end;
        stm32_err_t s_err;

#ifdef SHELLY_DIMMER_DEBUG
        ALOG_DBG(PSTR(SHD_LOGNAME "STM32 erase memory"));
#endif  // SHELLY_DIMMER_DEBUG

        stm32_erase_memory(stm, 0, STM32_MASS_ERASE);

        addr = stm->dev->fl_start;
        end = addr + size;
        while(addr < end && offset < size)
        {
            uint32_t left = end - addr;
            len   = sizeof(buffer) > left ? left : sizeof(buffer);
            len   = len > size - offset ? size - offset : len;

            if (len == 0)
            {
                break;
            }

            memcpy(buffer, p_st, sizeof(buffer));  // We need 4-byte bounadry flash access
            p_st += sizeof(buffer);

            s_err = stm32_write_memory(stm, addr, buffer, len);
            if (s_err != STM32_ERR_OK)
            {
                ret = false;
                break;
            }

            addr  += len;
            offset  += len;
        }
        stm32_close(stm);
    }
    return ret;
}

bool ShdPresent(void) {
  return present;
}

uint32_t ShdFlash(uint8_t* data, size_t size) {
#ifdef SHELLY_DIMMER_DEBUG
  ALOG_INF(PSTR(SHD_LOGNAME "Updating firmware v%u.%u with %u bytes"), dimmer.version_major, dimmer.version_minor, size);
#endif  // SHELLY_DIMMER_DEBUG

  ShdSerial.end();
  ShdSerial.begin(115200, SERIAL_8E1);
  ShdResetToDFUMode();
  bool error = !ShdUpdateFirmware(data, size);  // Allow flash access without ESP.flashRead
  ShdSerial.end();
  ShdResetToAppMode();
  ShdSerial.begin(115200, SERIAL_8N1);
  ShdSendVersion();

  return error;
}

#endif // SHELLY_FW_UPGRADE


/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mShellyDimmer::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  uint16_t new_brightness = 0;
    

  if(jtok = obj[PM_BRIGHTNESS]){

    req_brightness = map(jtok.getInt(),0,100,0,1000);
    SetBrightnessReq();

  }

  if(jtok = obj["BrightnessFade"]){

    req_brightness = jtok.getInt();
    SetBrightnessFade();

  }


  if(jtok = obj["ShellyDimmer"].getObject()["Brightness"]){

    new_brightness = map(jtok.getInt(),0,100,0,1000);

  }



  
  if(jtok = obj["ShellyDimmer"].getObject()["TransitionTimeSec"]){      //lets add way to set via sec/millis and automatically create new blender for it

    time_ms = jtok.getInt()*1000;
    
    transition.fader->SetTargetAndStartBlend(new_brightness, time_ms);

  }
  // if(jtok = obj["ShellyDimmer"].getObject()["TransitionTimeSec"]){

  //   duration = jtok.getInt()*1000;
  //   transition.fader->SetTargetAndStartBlend(new_brightness, duration);

  // }

  if(jtok = obj["ShellyDimmer"].getObject()["TriggerEdge"]){

    CmndShdLeadingEdge( jtok.getInt());

  }
  
  if(jtok = obj["ShellyDimmer"].getObject()[PM_TIME_ON]){
    CommandSet_Timer_Decounter(jtok.getInt());
  }else
  if(jtok = obj["ShellyDimmer"].getObject()[PM_TIME_ON_SECS]){
    CommandSet_Timer_Decounter(jtok.getInt());
  }else
  if(jtok = obj["ShellyDimmer"].getObject()[PM_TIME_ON_MINUTES]){
    CommandSet_Timer_Decounter(jtok.getInt()*60);
  }
  
  mqtthandler_state_teleperiod.flags.SendNow = true;

}


/**********************************************************************************************
 *********************************************************************************************
  Parameter: TimerDecounter
 *********************************************************************************************
 ********************************************************************************************/

void mShellyDimmer::CommandSet_Timer_Decounter(uint16_t time_secs)
{
  timer_decounter.seconds = time_secs;
  timer_decounter.active = time_secs > 0 ? true : false;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "%d" D_UNIT_SECOND), timer_decounter.seconds);  
  #endif
}

uint16_t mShellyDimmer::CommandGet_SecondsToRemainOn()
{
  // relay_status[relay_id].timer_decounter.seconds = time_secs;
  // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
  // #endif

  return timer_decounter.seconds;
}



bool mShellyDimmer::SetChannels(void)
{
#ifdef SHELLY_DIMMER_DEBUG
    ALOG_DBM( PSTR(SHD_LOGNAME "SetChannels:"));
    // AddLogBuffer(LOG_LEVEL_DEBUG_MORE, (uint8_t *)XdrvMailbox.data, XdrvMailbox.data_len);
#endif  // SHELLY_DIMMER_DEBUG

    // uint16_t brightness = ((uint32_t *)XdrvMailbox.data)[0];
    // // Use dimmer_hw_min and dimmer_hw_max to constrain our values if the light should be on
    // if (brightness > 0)
    //     brightness = changeUIntScale(brightness, 0, 255, Settings.dimmer_hw_min * 10, Settings.dimmer_hw_max * 10);
    // req_brightness = brightness;

    // ShdDebugState();

    return SyncState();
}

bool mShellyDimmer::SetPower(void)
{
  #ifdef SHELLY_DIMMER_DEBUG
  // ALOG_INF(PSTR(SHD_LOGNAME "Set Power, Power 0x%02x"), XdrvMailbox.index);
  #endif  // SHELLY_DIMMER_DEBUG

  req_on = 1;//(bool)XdrvMailbox.index;
  return SyncState();
}



/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

void mShellyDimmer::CmndShdLeadingEdge(uint8_t edge_type)
{
    if (edge_type == 0 || edge_type == 1)
    {
        leading_edge = 2 - edge_type;
        // pCN Settings.shd_leading_edge = edge_type;
#ifdef SHELLY_DIMMER_DEBUG
        if (leading_edge == 1)
            ALOG_DBG(PSTR(SHD_LOGNAME "Set to trailing edge"));
        else
            ALOG_DBG(PSTR(SHD_LOGNAME "Set to leading edge"));
#endif  // SHELLY_DIMMER_DEBUG
        SendSettings();
    }
    SaveSettings();
    // ResponseCmndNumber(Settings.shd_leading_edge);
}



#ifdef SHELLY_CMDS

const char kShdCommands[] PROGMEM = D_PRFX_SHD "|"  // Prefix
  D_CMND_LEADINGEDGE "|"  D_CMND_WARMUPBRIGHTNESS "|" D_CMND_WARMUPTIME;

void (* const ShdCommand[])(void) PROGMEM = {
  &CmndShdLeadingEdge, &CmndShdWarmupBrightness, &CmndShdWarmupTime };

void CmndShdWarmupBrightness(void)
{
    if ((10 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 100))
    {
        warmup_brightness = XdrvMailbox.payload * 10;
        Settings.shd_warmup_brightness = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        ALOG_DBG(PSTR(SHD_LOGNAME "Set warmup brightness to %d%%"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_warmup_brightness);
}

void CmndShdWarmupTime(void)
{
    if ((20 <= XdrvMailbox.payload) && (XdrvMailbox.payload <= 200))
    {
        warmup_time = XdrvMailbox.payload;
        Settings.shd_warmup_time = XdrvMailbox.payload;
#ifdef SHELLY_DIMMER_DEBUG
        ALOG_DBG(PSTR(SHD_LOGNAME "Set warmup time to %dms"), XdrvMailbox.payload);
#endif  // SHELLY_DIMMER_DEBUG
        ShdSendSettings();
    }
    ShdSaveSettings();
    ResponseCmndNumber(Settings.shd_warmup_time);
}

#endif // SHELLY_CMDS

/******************************************************************************************************************************
*******************************************************************************************************************************
****************** CommandSet_ReadFile *****************************************************************************************
*******************************************************************************************************************************
*******************************************************************************************************************************/

// void mShellyDimmer::CommandSet_ReadFile(const char* filename){

//   readFile(SD_MMC, filename);

//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_SDCARD D_COMMAND_SVALUE_K("ReadFile")), filename);
//   #endif // ENABLE_LOG_LEVEL_COMMANDS

// } 

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mShellyDimmer::MQTTHandler_Init(){

  struct handler<mShellyDimmer>* ptr;
    
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR;//pCONT_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_Settings;

  
  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_State;

//   ptr = &mqtthandler_sensdebug_teleperiod;
//   ptr->tSavedLastSent = 0;
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_mqtt->dt.ifchanged_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_Debug;

} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mShellyDimmer::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mShellyDimmer::MQTTHandler_Rate()
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
void mShellyDimmer::MQTTHandler_Sender()
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
