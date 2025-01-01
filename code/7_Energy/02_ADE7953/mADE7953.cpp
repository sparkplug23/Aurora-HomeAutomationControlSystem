#include "mADE7953.h"

#ifdef USE_MODULE_ENERGY_ADE7953

void mEnergyADE7953::Pre_Init(void)
{
  if (!pCONT_sup->I2cEnabled(XI2C_07)) { 
    return; 
  }

  if (pCONT_pins->PinUsed(GPIO_ADE7953_IRQ_ID)) {                // Irq on GPIO16 is not supported...
    pinMode(pCONT_pins->GetPin(GPIO_ADE7953_IRQ_ID), INPUT);     // Related to resetPins() - Must be set to input
    delay(100);                                                  // Need 100mS to init ADE7953
    if (pCONT_sup->I2cSetDevice(ADE7953_ADDR)) {
      if (HLW_PREF_PULSE == tkr_set->Settings.energy_usage.energy_power_calibration) {
        tkr_set->Settings.energy_usage.energy_power_calibration = ADE7953_PREF;
        tkr_set->Settings.energy_usage.energy_voltage_calibration = ADE7953_UREF;
        tkr_set->Settings.energy_usage.energy_current_calibration = ADE7953_IREF;
      }
      pCONT_sup->I2cSetActiveFound(ADE7953_ADDR, "ADE7953");
      settings.fEnableSensor = true;
      measured.init_step = 2;
      pCONT_iEnergy->Energy.phase_count = 2;                     // Handle two channels as two phases
      pCONT_iEnergy->Energy.voltage_common = true;               // Use common voltage
      // pCONT_iEnergy->Energy.frequency_common = true;             // Use common frequency
      tkr_set->runtime_var.energy_driver = D_GROUP_MODULE_ENERGY_ADE7953_ID;
    }
  }
}


int8_t mEnergyADE7953::Tasker(uint8_t function, JsonParserObject obj){
    
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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:{
      EverySecond();
    }
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
  
  return 0;

} // END function


void mEnergyADE7953::Init(void)
{

  Write(0x102, 0x0004);    // Locking the communication interface (Clear bit COMM_LOCK), Enable HPF
  Write(0x0FE, 0x00AD);    // Unlock register 0x120
  Write(0x120, 0x0030);    // Configure optimum setting

}


int mEnergyADE7953::RegSize(uint16_t reg)
{
  int size = 0;
  switch ((reg >> 8) & 0x0F) {
    case 0x03:
      size++;
    case 0x02:
      size++;
    case 0x01:
      size++;
    case 0x00:
    case 0x07:
    case 0x08:
      size++;
  }
  return size;
}

void mEnergyADE7953::Write(uint16_t reg, uint32_t val)
{
  int size = RegSize(reg);
  if (size) {
    pCONT_i2c->wire->beginTransmission(ADE7953_ADDR);
    pCONT_i2c->wire->write((reg >> 8) & 0xFF);
    pCONT_i2c->wire->write(reg & 0xFF);
    while (size--) {
      pCONT_i2c->wire->write((val >> (8 * size)) & 0xFF);  // Write data, MSB first
    }
    pCONT_i2c->wire->endTransmission();
    delayMicroseconds(5);    // Bus-free time minimum 4.7us
  }
}

int32_t mEnergyADE7953::Read(uint16_t reg)
{
	uint32_t response = 0;

  int size = RegSize(reg);
  if (size) {
    pCONT_i2c->wire->beginTransmission(ADE7953_ADDR);
    pCONT_i2c->wire->write((reg >> 8) & 0xFF);
    pCONT_i2c->wire->write(reg & 0xFF);
    pCONT_i2c->wire->endTransmission(0);
    pCONT_i2c->wire->requestFrom(ADE7953_ADDR, size);
    if (size <= pCONT_i2c->wire->available()) {
      for (uint32_t i = 0; i < size; i++) {
        response = response << 8 | pCONT_i2c->wire->read();   // receive DATA (MSB first)
      }
    }
  }
	return response;
}

void mEnergyADE7953::GetData(void)
{
  int32_t reg[2][4];
  for (uint32_t i = 0; i < sizeof(Ade7953Registers)/sizeof(uint16_t); i++) {
    int32_t value = Read(Ade7953Registers[i]);
    if (8 == i) {
      measured.voltage_rms = value;  // RMS voltage (Both relays)
    } else if (9 == i) {
      measured.period = value;  // period
    } else {
      reg[i >> 2][i &3] = value;
    }
  }
  ALOG_DBM( PSTR("ADE: %d, %d, [%d, %d, %d, %d], [%d, %d, %d, %d]"),
    measured.voltage_rms, measured.period,
    reg[0][0], reg[0][1], reg[0][2], reg[0][3],
    reg[1][0], reg[1][1], reg[1][2], reg[1][3]);

  uint32_t apparent_power[2] = { 0, 0 };
  uint32_t reactive_power[2] = { 0, 0 };

  for (uint32_t channel = 0; channel < 2; channel++) {
    measured.current_rms[channel] = reg[channel][0];
    if (measured.current_rms[channel] < 2000) {           // No load threshold (20mA)
      measured.current_rms[channel] = 0;
      measured.active_power[channel] = 0;
    } else {
      measured.active_power[channel] = abs(reg[channel][1]);
      apparent_power[channel] = abs(reg[channel][2]);
      reactive_power[channel] = abs(reg[channel][3]);
    }
  }

  uint32_t current_rms_sum = measured.current_rms[0] + measured.current_rms[1];
  uint32_t active_power_sum = measured.active_power[0] + measured.active_power[1];

  ALOG_DBM( PSTR("ADE: U %d, C %d, I %d + %d = %d, P %d + %d = %d"),
    measured.voltage_rms, measured.period,
    measured.current_rms[0], measured.current_rms[1], current_rms_sum,
    measured.active_power[0], measured.active_power[1], active_power_sum);

  if (pCONT_iEnergy->Energy.power_on) {  // Powered on
    pCONT_iEnergy->Energy.voltage[0] = (float)measured.voltage_rms / tkr_set->Settings.energy_usage.energy_voltage_calibration;
    pCONT_iEnergy->Energy.frequency[0] = 223750.0f / ( (float)measured.period + 1);

    for (uint32_t channel = 0; channel < 2; channel++) {
      pCONT_iEnergy->Energy.data_valid[channel] = 0;
      pCONT_iEnergy->Energy.active_power[channel] = (float)measured.active_power[channel] / (tkr_set->Settings.energy_usage.energy_power_calibration / 10);
      pCONT_iEnergy->Energy.reactive_power[channel] = (float)reactive_power[channel] / (tkr_set->Settings.energy_usage.energy_power_calibration / 10);
      pCONT_iEnergy->Energy.apparent_power[channel] = (float)apparent_power[channel] / (tkr_set->Settings.energy_usage.energy_power_calibration / 10);
      if (0 == pCONT_iEnergy->Energy.active_power[channel]) {
        pCONT_iEnergy->Energy.current[channel] = 0;
      } else {
        pCONT_iEnergy->Energy.current[channel] = (float)measured.current_rms[channel] / (tkr_set->Settings.energy_usage.energy_current_calibration * 10);
      }
    }
/*
  } else {  // Powered off
    pCONT_iEnergy->Energy.data_valid[0] = ENERGY_WATCHDOG;
    pCONT_iEnergy->Energy.data_valid[1] = ENERGY_WATCHDOG;
*/
  }

  // if (active_power_sum) {
  //   pCONT_iEnergy->Energy.kWhtoday_delta += ((active_power_sum * (100000 / (tkr_set->Settings.energy_usage.energy_power_calibration / 10))) / 3600);
  //   EnergyUpdateToday();
  // }
}

void mEnergyADE7953::EverySecond(void)
{
	if (measured.init_step) {
    if (1 == measured.init_step) {
      Init();
	  }
    measured.init_step--;
	}	else {
		GetData();
	}
}


bool mEnergyADE7953::Command(void)
{
  bool serviced = true;

  // uint32_t channel = (2 == XdrvMailbox.index) ? 1 : 0;
  // uint32_t value = (uint32_t)(CharToFloat(XdrvMailbox.data) * 100);  // 1.23 = 123

  // if (CMND_POWERCAL == pCONT_iEnergy->Energy.command_code) {
  //   if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_PREF; }
  //   // Service in xdrv_03_pCONT_iEnergy->Energy.ino
  // }
  // else if (CMND_VOLTAGECAL == pCONT_iEnergy->Energy.command_code) {
  //   if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_UREF; }
  //   // Service in xdrv_03_pCONT_iEnergy->Energy.ino
  // }
  // else if (CMND_CURRENTCAL == pCONT_iEnergy->Energy.command_code) {
  //   if (1 == XdrvMailbox.payload) { XdrvMailbox.payload = ADE7953_IREF; }
  //   // Service in xdrv_03_pCONT_iEnergy->Energy.ino
  // }
  // else if (CMND_POWERSET == pCONT_iEnergy->Energy.command_code) {
  //   if (XdrvMailbox.data_len && measured.active_power[channel]) {
  //     if ((value > 100) && (value < 200000)) {  // Between 1W and 2000W
  //       Settings.energy_power_calibration = (measured.active_power[channel] * 1000) / value;  // 0.00 W
  //     }
  //   }
  // }
  // else if (CMND_VOLTAGESET == pCONT_iEnergy->Energy.command_code) {
  //   if (XdrvMailbox.data_len && measured.voltage_rms) {
  //     if ((value > 10000) && (value < 26000)) {  // Between 100V and 260V
  //       Settings.energy_voltage_calibration = (measured.voltage_rms * 100) / value;  // 0.00 V
  //     }
  //   }
  // }
  // else if (CMND_CURRENTSET == pCONT_iEnergy->Energy.command_code) {
  //   if (XdrvMailbox.data_len && measured.current_rms[channel]) {
  //     if ((value > 2000) && (value < 1000000)) {  // Between 20mA and 10A
  //       Settings.energy_current_calibration = ((measured.current_rms[channel] * 100) / value) * 100;  // 0.00 mA
  //     }
  //   }
  // }
  // else serviced = false;  // Unknown command

  return serviced;
}



uint8_t mEnergyADE7953::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


uint8_t mEnergyADE7953::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_VOLTAGE, measured.voltage_rms);
    JBI->Add("period", measured.period);
    JBI->Array_Start(D_CURRENT);
      JBI->Add(measured.current_rms[0]);
      JBI->Add(measured.current_rms[1]);
    JBI->Array_End();
    JBI->Array_Start(D_ACTIVE_POWERUSAGE);
      JBI->Add(measured.active_power[0]);
      JBI->Add(measured.active_power[1]);
    JBI->Array_End();
    JBI->Add("initstep", measured.init_step);
  return JBI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mEnergyADE7953::MQTTHandler_Init(){

  struct handler<mEnergyADE7953>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC2HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyADE7953::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC2HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyADE7953::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyADE7953::ConstructJSON_Sensor;
  
} 


void mEnergyADE7953::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} 


void mEnergyADE7953::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mEnergyADE7953::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mEnergyADE7953>* list_ptr[] = {
    &mqtthandler_settings,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, EM_MODULE_ENERGY_ADE7953_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id);

}

////////////////////// END OF MQTT /////////////////////////

#endif
