#include "mHardwarePins.h"

/**
 * @brief 
 * 
 * 
 * Create new method here, look into how WLED "locks"/assigns pins.
 * I want a way here to define nicely what the pin is doing (Which module is attached, what function is it, ie digital, I2C, serial)
 * Then show which pins are available for use
 * make sure to clasify the limitations on a pin too with a "special note" mqtt message against the pin. 
 * 
 * 
 * 
 */

// #ifdef ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
// GPIOViewer gpio_viewer;
// #endif

// const uint16_t mHardwarePins::GPIO_Function_EnabledList_IDs[] PROGMEM = {
//   GPIO_NONE_ID,           // Not used
//   GPIO_KEY1_ID,           // Buttons
//   GPIO_KEY1_NP_ID,
//   GPIO_KEY1_INV_ID,
//   GPIO_KEY1_INV_NP_ID,
//   GPIO_KEY2_ID,
//   GPIO_KEY2_NP_ID,
//   GPIO_KEY2_INV_ID,
//   GPIO_KEY2_INV_NP_ID,
//   GPIO_KEY3_ID,
//   GPIO_KEY3_NP_ID,
//   GPIO_KEY3_INV_ID,
//   GPIO_KEY3_INV_NP_ID,
//   GPIO_KEY4_ID,
//   GPIO_KEY4_NP_ID,
//   GPIO_KEY4_INV_ID,
//   GPIO_KEY4_INV_NP_ID,
//   GPIO_SWT1_ID,           // User connected external switches
//   GPIO_SWT1_NP_ID,
//   GPIO_SWT2_ID,
//   GPIO_SWT2_NP_ID,
//   GPIO_SWT3_ID,
//   GPIO_SWT3_NP_ID,
//   GPIO_SWT4_ID,
//   GPIO_SWT4_NP_ID,
//   GPIO_SWT5_ID,
//   GPIO_SWT5_NP_ID,
//   GPIO_SWT6_ID,
//   GPIO_SWT6_NP_ID,
//   GPIO_SWT7_ID,
//   GPIO_SWT7_NP_ID,
//   GPIO_SWT8_ID,
//   GPIO_SWT8_NP_ID,
//   GPIO_REL1_ID,           // Relays
//   GPIO_REL1_INV_ID,
//   GPIO_REL2_ID,
//   GPIO_REL2_INV_ID,
//   GPIO_REL3_ID,
//   GPIO_REL3_INV_ID,
//   GPIO_REL4_ID,
//   GPIO_REL4_INV_ID,
//   GPIO_REL5_ID,
//   GPIO_REL5_INV_ID,
//   GPIO_REL6_ID,
//   GPIO_REL6_INV_ID,
//   GPIO_REL7_ID,
//   GPIO_REL7_INV_ID,
//   GPIO_REL8_ID,
//   GPIO_REL8_INV_ID,
//   GPIO_LED1_ID,           // Leds
//   GPIO_LED1_INV_ID,
//   GPIO_LED2_ID,
//   GPIO_LED2_INV_ID,
//   GPIO_LED3_ID,
//   GPIO_LED3_INV_ID,
//   GPIO_LED4_ID,
//   GPIO_LED4_INV_ID,
//   GPIO_PWM1_ID,           // RGB   Red   or C  Cold White
//   GPIO_PWM1_INV_ID,
//   GPIO_PWM2_ID,           // RGB   Green or CW Warm White
//   GPIO_PWM2_INV_ID,
//   GPIO_PWM3_ID,           // RGB   Blue
//   GPIO_PWM3_INV_ID,
//   GPIO_PWM4_ID,           // RGBW  (Cold) White
//   GPIO_PWM4_INV_ID,
//   GPIO_PWM5_ID,           // RGBCW Warm White
//   GPIO_PWM5_INV_ID,
//   GPIO_CNTR1_ID,          // Counters
//   GPIO_CNTR1_NP_ID,
//   GPIO_CNTR2_ID,
//   GPIO_CNTR2_NP_ID,
//   GPIO_CNTR3_ID,
//   GPIO_CNTR3_NP_ID,
//   GPIO_CNTR4_ID,
//   GPIO_CNTR4_NP_ID,
//   GPIO_HWSERIAL0_TX_ID,            // Serial interface
//   GPIO_HWSERIAL0_RX_ID,            // Serial interface
//   GPIO_HWSERIAL1_TX_ID,            // Serial interface
//   GPIO_HWSERIAL1_RX_ID,            // Serial interface
//   GPIO_HWSERIAL2_TX_ID,            // Serial interface
//   GPIO_HWSERIAL2_RX_ID,            // Serial interface
//   GPIO_SWSERIAL0_TX_ID,            // Serial interface
//   GPIO_SWSERIAL0_RX_ID,            // Serial interface
// #ifdef USE_MODULE_CORE_I2C
//   GPIO_I2C_SCL_ID,        // I2C SCL
//   GPIO_I2C_SDA_ID,        // I2C SDA
// #endif
// #ifdef USE_SPI
//   GPIO_SPI_CS_ID,         // SPI Chip Select
//   GPIO_SPI_DC_ID,         // SPI Data Direction
//   GPIO_SSPI_MISO_ID,      // Software SPI Master Input Slave Output
//   GPIO_SSPI_MOSI_ID,      // Software SPI Master Output Slave Input
//   GPIO_SSPI_SCLK_ID,      // Software SPI Serial Clock
//   GPIO_SSPI_CS_ID,        // Software SPI Chip Select
//   GPIO_SSPI_DC_ID,        // Software SPI Data or Command
// #endif
// #ifdef USE_DISPLAY
//   GPIO_BACKLIGHT_ID,      // Display backlight control
// #endif
// #ifdef USE_MODULE_DRIVER_BUZZER
//   GPIO_BUZZER_ID,         // Buzzer
//   GPIO_BUZZER_INV_ID,     // Inverted buzzer
// #endif
// #ifdef USE_MODULE_SENSORS_DHT
//   GPIO_DHT11_1OF2_ID,          // DHT11
//   GPIO_DHT11_2OF2_ID,          // DHT11
//   GPIO_DHT22_1OF2_ID,          // DHT21, DHT22, AM2301, AM2302, AM2321
//   GPIO_DHT22_2OF2_ID,          // DHT21, DHT22, AM2301, AM2302, AM2321
// #endif
// //#if defined(USE_DS18B20) || defined(USE_DS18x20) || defined(USE_DS18x20_LEGACY)
//   GPIO_DSB_1OF2_ID,            // Single wire DS18B20 or DS18S20
//   GPIO_DSB_2OF2_ID,            // Single wire DS18B20 or DS18S20
// //#endif
// #ifdef USE_WS2812
//   GPIO_RGB_DATA_ID,         // WS2812 Led string
//   GPIO_RGB_CLOCK_ID,         // WS2812 Led string
// #endif
// #ifdef USE_IR_REMOTE
//   GPIO_IRSEND_ID,         // IR remote
// #ifdef USE_IR_RECEIVE
//   GPIO_IRRECV_ID,         // IR receiver
// #endif
// #endif
// #ifdef USE_RC_SWITCH
//   GPIO_RFSEND_ID,         // RF transmitter
//   GPIO_RFRECV_ID,         // RF receiver
// #endif
// // #ifdef USE_RF_SENSOR
//   GPIO_RF_433MHZ_RX_ID,      // Rf receiver with sensor decoding
// // #endif
// #ifdef USE_MODULE_SENSORS_ULTRASONICS
//   GPIO_SR04_TRIG_ID,      // SR04 Trigger pin
//   GPIO_SR04_ECHO_ID,      // SR04 Echo pin
// #endif
// // #if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
// //   GPIO_NRG_SEL_ID,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
// //   GPIO_NRG_SEL_INV_ID,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
// //   GPIO_NRG_CF1_ID,        // HLW8012/HLJ-01 CF1 voltage / current
// //   GPIO_HLW_CF_ID,         // HLW8012 CF power
// //   GPIO_HJL_CF_ID,         // HJL-01/BL0937 CF power
// // #endif
// // #if defined(USE_ENERGY_SENSOR) && defined(USE_I2C) && defined(USE_ADE7953)
//   GPIO_ADE7953_IRQ_ID,    // ADE7953 IRQ
// // #endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
//   GPIO_MCP39F5_TX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RST_ID,    // MCP39F501 Reset (Shelly2)
// #endif
// // #if defined(USE_MODULE_ENERGY_PZEM004T_V3) || defined(USE_PZEM_DC)
//   GPIO_PZEM0XX_TX_ID,     // PZEM0XX Serial interface
// // #endif
// // #ifdef USE_PZEM004T
//   GPIO_PZEM004_RX_ID,     // PZEM004T Serial interface
// // #endif
// // #ifdef USE_MODULE_ENERGY_PZEM004T_V3
//   GPIO_PZEM0XX_RX_MODBUS_ID,     // PZEM-014_ID,016 Serial Modbus interface
// // #endif
// // #ifdef USE_PZEM_DC
//   GPIO_PZEM017_RX_ID,     // PZEM-003_ID,017 Serial Modbus interface
// // #endif
// #ifdef USE_SDM120
//   GPIO_SDM120_TX_ID,      // SDM120 Serial interface
//   GPIO_SDM120_RX_ID,      // SDM120 Serial interface
// #endif
// #ifdef USE_SDM630
//   GPIO_SDM630_TX_ID,      // SDM630 Serial interface
//   GPIO_SDM630_RX_ID,      // SDM630 Serial interface
// #endif
// #ifdef USE_SERIAL_BRIDGE
//   GPIO_SBR_TX_ID,         // Serial Bridge Serial interface
//   GPIO_SBR_RX_ID,         // Serial Bridge Serial interface
// #endif
// #ifdef USE_SENSEAIR
//   GPIO_SAIR_TX_ID,        // SenseAir Serial interface
//   GPIO_SAIR_RX_ID,        // SenseAir Serial interface
// #endif
// #ifdef USE_NOVA_SDS
//   GPIO_SDS0X1_TX_ID,      // Nova Fitness SDS011 Serial interface
//   GPIO_SDS0X1_RX_ID,      // Nova Fitness SDS011 Serial interface
// #endif
// #ifdef USE_PMS5003
//   GPIO_PMS5003_ID,        // Plantower PMS5003 Serial interface
// #endif
// #ifdef USE_TX20_WIND_SENSOR
//   GPIO_TX20_TXD_BLACK_ID, // TX20 Transmission Pin
// #endif
// #ifdef USE_MP3_PLAYER
//   GPIO_MP3_DFR562_ID,     // RB-DFR-562_ID, DFPlayer Mini MP3 Player Serial interface
// #endif
// #ifdef USE_TUYA_DIMMER
//   GPIO_TUYA_TX_ID,        // Tuya Serial interface
//   GPIO_TUYA_RX_ID,        // Tuya Serial interface
// #endif
// #ifdef USE_AZ7798
//   GPIO_AZ_TXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
//   GPIO_AZ_RXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
// #endif
// #ifdef USE_PN532_HSU
//   GPIO_PN532_TXD_ID,      // PN532 HSU Tx
//   GPIO_PN532_RXD_ID,      // PN532 HSU Rx
// #endif
// #ifdef USE_MGC3130
//   GPIO_MGC3130_XFER_ID,
//   GPIO_MGC3130_RESET_ID,
// #endif
// #ifdef USE_MAX31855
//   GPIO_MAX31855CS_ID,     // MAX31855 Serial interface
//   GPIO_MAX31855CLK_ID,    // MAX31855 Serial interface
//   GPIO_MAX31855DO_ID,     // MAX31855 Serial interface
// #endif
// #ifdef USE_SM16716
//   GPIO_SM16716_CLK_ID,    // SM16716 CLOCK
//   GPIO_SM16716_DAT_ID,    // SM16716 DATA
//   GPIO_SM16716_SEL_ID,    // SM16716 SELECT
// #endif // USE_SM16716
// #ifdef ROTARY_V1
//   GPIO_ROT1A_ID,          // Rotary switch1 A Pin
//   GPIO_ROT1B_ID,          // Rotary switch1 B Pin
//   GPIO_ROT2A_ID,          // Rotary switch2 A Pin
//   GPIO_ROT2B_ID,          // Rotary switch2 B Pin
// #endif
// #ifdef USE_ARILUX_RF
//   GPIO_ARIRFRCV_ID,       // AliLux RF Receive input
// #endif
// #ifdef USE_HRE
//   GPIO_HRE_CLOCK_ID,
//   GPIO_HRE_DATA
// #endif
// #ifdef USE_MODULE_SENSORS_DOOR
//   GPIO_DOOR_OPEN_ID,
// #endif
// #ifdef USE_SENSOR_DOOR_LOCK
//   GPIO_DOOR_LOCK_ID,
// #endif
// #ifdef USE_MODULE_DRIVERS_HBRIDGE
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_ANALOG_SENSE_ID,
// #endif
// };



/********************************************************************************************\
 * ESP8266 Module Templates
\********************************************************************************************/

#ifdef ESP8266

/**
 * @brief This contains indexing for esp8266 THEN esp8285, the index (value) is important for retrieving template from either esp8266 or esp8285 arrays
 * 
 */
const uint16_t mHardwarePins::ModuleList_Enabled_IDs[] PROGMEM = {
  MODULE_WEMOS_ID,  //esp8285
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
    MODULE_SONOFF_BASIC_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
    MODULE_SONOFF_BASIC_EXTERNAL_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801  
    MODULE_H801_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
    MODULE_MAGICHOME_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
    MODULE_SHELLY1_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    MODULE_SHELLY2P5_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    MODULE_SHELLY_DIMMER2_ID,
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    MODULE_SONOFF_IFAN03_ID,   //esp8285
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO
    MODULE_SONOFF_4CHPRO_ID   //esp8285
  #endif
  // MODULE_MAXMODULE_8266 // To allow dynamic array size

};


const char mHardwarePins::ModuleList_Names_DList[] PROGMEM
{
  D_MODULE_NAME_USERMODULE_CTR "|"
  D_MODULE_NAME_SONOFF_BASIC_CTR  "|"
  D_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR  "|"
  D_MODULE_NAME_H801_CTR          "|"
  D_MODULE_NAME_MAGICHOME_CTR     "|"
  D_MODULE_NAME_SHELLY1_CTR       "|"
  D_MODULE_NAME_SHELLY2P5_CTR     "|"
  D_MODULE_NAME_SHELLY_DIMMER2_CTR "|"
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  D_MODULE_NAME_SONOFF_IFAN03_CTR  "|"
  #endif
  D_MODULE_NAME_SONOFF_4CHPRO_CTR 
};


/*********************************************************************************************\
 * Templates with 12 usable pins (ESP8266)
\*********************************************************************************************/
// !!! Update ModuleTemplate_Names_DList above in the same order as below
const mytmplt8266 mHardwarePins::ModuleTemplate_GPIO_Map[MODULE_MAXMODULE_8266] PROGMEM = {
  // Generic option always first
  { // D_MODULE_NAME_USERMODULE_CTR
    
    //phase into split esp82xx arrays, but MUST be done with ifdef's to enable long term conversion and testing
    
    //MODULE_WEMOS_ID,         // Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
    GPIO_USER_ID,        // GPIO00 D3 Wemos Button Shield
    GPIO_USER_ID,        // GPIO01 TX Serial RXD
    GPIO_USER_ID,        // GPIO02 D4 Wemos DHT Shield
    GPIO_USER_ID,        // GPIO03 RX Serial TXD and Optional sensor
    GPIO_USER_ID,        // GPIO04 D2 Wemos I2C SDA
    GPIO_USER_ID,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_USER_ID,        // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    GPIO_USER_ID,        // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_USER_ID,        // GPIO12 D6
    GPIO_USER_ID,        // GPIO13 D7
    GPIO_USER_ID,        // GPIO14 D5
    GPIO_USER_ID,        // GPIO15 D8
    GPIO_USER_ID,        // GPIO16 D0 Wemos Wake
    GPIO_FLAG_ADC0_ID    // ADC0 A0 Analog input
  },
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC
  { // MODULE_SONOFF_BASIC_ID    // Sonoff Basic (ESP8266)
    GPIO_KEY1_ID,     // GPIO00 Button
    GPIO_USER_ID,     // GPIO01 Serial RXD and Optional sensor
    GPIO_LED2_INV_ID, // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_USER_ID,     // GPIO03 Serial TXD and Optional sensor
    GPIO_USER_ID,     // GPIO04 Optional sensor
    0,                // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)                             PHASE OUT !!!! ON 8266
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)                             PHASE OUT !!!! ON 8266
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1_ID,     // GPIO12 Red Led and Relay (0 = Off_ID, 1 = On)
    GPIO_LED1_INV_ID, // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER_ID,     // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    0                 // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_BASIC_EXTERNAL
  { // MODULE_SONOFF_BASIC_EXTERNAL_ID    // Sonoff Basic (ESP8266)
    GPIO_KEY1_ID,     // GPIO00 Button
    GPIO_USER_ID,     // GPIO01 Serial RXD and Optional sensor
    GPIO_LED2_INV_ID, // GPIO02 Only available on newer Sonoff Basic R2 V1
    GPIO_KEY2_ID,     // GPIO03 Serial TXD and Optional sensor
    GPIO_USER_ID,     // GPIO04 Optional sensor
    0,                // GPIO05
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_REL1_ID,     // GPIO12 Red Led and Relay (0 = Off_ID, 1 = On)
    GPIO_LED1_INV_ID, // GPIO13 Green Led (0 = On, 1 = Off) - Link and Power status
    GPIO_USER_ID,     // GPIO14 Optional sensor
    0,                // GPIO15
    0,                // GPIO16
    0                 // ADC0 Analog input
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_H801
  { // MODULE_H801_ID,            // Lixada H801 Wifi (ESP8266)
    GPIO_KEY1_ID,        // GPIO00 E-FW Button   // TEMPORARILY FORCING AS A KEY1 HERE, REMOVE WHEN GPIOC IS PROPERLY ADDED INTO CODE
    GPIO_LED1_ID,        // GPIO01 Green LED - Link and Power status
    GPIO_USER_ID,        // GPIO02 TX and Optional sensor - Pin next to TX on the PCB
    GPIO_USER_ID,        // GPIO03 RX and Optional sensor - Pin next to GND on the PCB
    GPIO_PWM5_ID,        // GPIO04 W2 - PWM5
    GPIO_LED2_INV_ID,    // GPIO05 Red LED
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3_ID,        // GPIO12 Blue
    GPIO_PWM2_ID,        // GPIO13 Green
    GPIO_PWM4_ID,        // GPIO14 W1 - PWM4
    GPIO_PWM1_ID,        // GPIO15 Red
    0, // GPIO16
    0 // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_MAGICHOME
  { //MODULE_MAGICHOME_ID,       // Magic Home (aka Flux-light) (ESP8266) and Arilux LC10 (ESP8285)
                      // https://www.aliexpress.com/item/Magic-Home-Mini-RGB-RGBW-Wifi-Controller-For-Led-Strip-Panel-light-Timing-Function-16million-colors/32686853650.html
    0,
    GPIO_USER_ID,        // GPIO01 Serial RXD and Optional sensor
    GPIO_LED1_INV_ID,    // GPIO02 Blue onboard LED - Link and Power status
    GPIO_USER_ID,        // GPIO03 Serial TXD and Optional sensor
    GPIO_ARIRFRCV_ID,    // GPIO04 IR or RF receiver (optional) (Arilux LC10)
    GPIO_PWM2_ID,        // GPIO05 RGB LED Green
                      // GPIO06 (SD_CLK   Flash)
                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                      // GPIO11 (SD_CMD   Flash)
    GPIO_PWM3_ID,        // GPIO12 RGB LED Blue
    GPIO_USER_ID,        // GPIO13 RGBW LED White (optional - set to PWM4 for Cold White or Warm White as used on Arilux LC10)
    GPIO_PWM1_ID,        // GPIO14 RGB LED Red
    GPIO_LED4_INV_ID,    // GPIO15 RF receiver control (Arilux LC10)
    0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY1
  { //MODULE_SHELLY1_ID,        // Shelly1 Open Source (ESP8266 - 2MB) - https://shelly.cloud/shelly1-open-source/
     0,                // GPIO00 - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,                // GPIO01 Serial RXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     0,
     0,                // GPIO03 Serial TXD - Can be changed to GPIO_USER, only if Shelly is powered with 12V DC
     GPIO_REL1_ID,        // GPIO04 Relay (0 = Off, 1 = On)
     GPIO_SWT1_NP_ID,     // GPIO05 SW pin
                       // GPIO06 (SD_CLK   Flash)
                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
     0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
     0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                       // GPIO11 (SD_CMD   Flash)
     0, 0, 0, 0, 0, 0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5  
  {  //MODULE_SHELLY2P5_ID,        // Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
    GPIO_LED1_INV_ID,     // GPIO00 LED1i
    0,                    // GPIO01 None
    GPIO_KEY1_ID,         // GPIO02 Button1
    0,                    // GPIO03 None
    GPIO_REL1_ID,         // GPIO04 Relay1
    GPIO_SWT2_NP_ID,      // GPIO05 Switch2n
                          // GPIO06 (SD_CLK   Flash)
                          // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                          // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                    // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
    0,                    // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
                          // GPIO11 (SD_CMD   Flash)
    GPIO_I2C_SDA_ID,      // GPIO12 I2C_SDA
    GPIO_SWT1_NP_ID,      // GPIO13 Switch1n
    GPIO_I2C_SCL_ID,      // GPIO14 I2C_SCL
    GPIO_REL2_ID,         // GPIO15 Relay2
    GPIO_ADE7953_IRQ_ID,  // GPIO16 ADE7953_IRQ
    GPIO_FLAG_ADC0_TEMP   // GPIOA0 Tempurature
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
  {      //MODULE_SHELLY_DIMMER2_ID                      // SHELLY_DIMMER2 - Shelly Dimmer 2 (ESP8285)
    0,                         // GPIO00 None
    GPIO_HWSERIAL0_TX_ID,      // GPIO01 ESP_TXD Serial RXD connection to Dimmer MCU
    0,                         // GPIO02 None 
    GPIO_HWSERIAL0_RX_ID,      // GPIO03 ESP_RXD Serial TXD connection to Dimmer MCU
    GPIO_SHELLY2_SHD_BOOT0_ID, // GPIO04 SHD Boot 0 
    GPIO_SHELLY2_SHD_RESET_INV_ID, // GPIO05 SHD Reset
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    0,                         // GPIO09 None 
    0,                         // GPIO10 None 
                               // GPIO11 (SD_CMD   Flash)
    GPIO_SWT2_NP_ID,           // GPIO12 Switch2n 
    0,                         // GPIO13 None 
    GPIO_SWT1_NP_ID,           // GPIO14 Switch1n 
    0,                         // GPIO15 None
    GPIO_LED1_INV_ID,          // GPIO16 Led1i 
    GPIO_FLAG_ADC0_TEMP        // ADC Temperature
  },
  #endif

  /**
   * @brief ESP8285 below this point, and will be moved into its own array
   * 
   */
  #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
  {          //MODULE_SONOFF_IFAN03_ID                  // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
    GPIO_KEY1_ID,              // GPIO00 WIFI_KEY0 Button 1
    GPIO_HWSERIAL0_TX_ID,      // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
    0,                         // GPIO02 ESP_LOG
    GPIO_HWSERIAL0_RX_ID,      // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
    0,                         // GPIO04 DEBUG_RX
    0,                         // GPIO05 DEBUG_TX
                               // GPIO06 (SD_CLK   Flash)
                               // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                               // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_REL1_INV_ID,          // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
    GPIO_BUZZER_INV_ID,        // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
                               // GPIO11 (SD_CMD   Flash)
    GPIO_REL3_ID,              // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
    GPIO_LED1_INV_ID,          // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
    GPIO_REL2_ID,              // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
    GPIO_REL4_ID,              // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
    0,                         // GPIO16 None 
    0                          // A0
  },
  #endif
  #ifdef USE_MODULE_TEMPLATE_SONOFF_4CHPRO  
  {                           // SONOFF_4CH - Sonoff 4CH (ESP8285)
    GPIO_KEY1_ID,          // GPIO00 Button 1
    GPIO_USER_ID,          // GPIO01 Serial RXD and Optional sensor
    GPIO_RF_433MHZ_RX_ID,          // GPIO02 Optional sensor
    GPIO_USER_ID,          // GPIO03 Serial TXD and Optional sensor
    GPIO_REL3_ID,          // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
    GPIO_REL2_ID,          // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
                        // GPIO06 (SD_CLK   Flash)
                        // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
                        // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
    GPIO_KEY2_ID,          // GPIO09 Button 2
    GPIO_KEY3_ID,          // GPIO10 Button 3
                        // GPIO11 (SD_CMD   Flash)
    GPIO_REL1_ID,          // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
    GPIO_LED1_INV_ID,      // GPIO13 Blue Led (0 = On, 1 = Off)
    GPIO_KEY4_ID,          // GPIO14 Button 4
    GPIO_REL4_ID,          // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
    0, 0
  },
  #endif //  USE_MODULE_TEMPLATE_SONOFF_4CHPRO



};


/*********************************************************************************************\
 * Templates with 14 usable pins (ESP8285)
\*********************************************************************************************/

// const mytmplt8285 kModules8285[MODULE_MAXMODULE_8285 - MODULE_WEMOS] PROGMEM = {
//    { //MODULE_WEMOS_ID,         // Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
//     GPIO_USER_ID,        // GPIO00 D3 Wemos Button Shield
//     GPIO_USER_ID,        // GPIO01 TX Serial RXD
//     GPIO_USER_ID,        // GPIO02 D4 Wemos DHT Shield
//     GPIO_USER_ID,        // GPIO03 RX Serial TXD and Optional sensor
//     GPIO_USER_ID,        // GPIO04 D2 Wemos I2C SDA
//     GPIO_USER_ID,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
//                       // GPIO06 (SD_CLK   Flash)
//                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//     GPIO_USER_ID,        // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//     GPIO_USER_ID,        // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                       // GPIO11 (SD_CMD   Flash)
//     GPIO_USER_ID,        // GPIO12 D6
//     GPIO_USER_ID,        // GPIO13 D7
//     GPIO_USER_ID,        // GPIO14 D5
//     GPIO_USER_ID,        // GPIO15 D8
//     GPIO_USER_ID,        // GPIO16 D0 Wemos Wake
//     GPIO_FLAG_ADC0_ID    // ADC0 A0 Analog input
//   },


//   {                      // SONOFF_4CH - Sonoff 4CH (ESP8285)
//     GPIO_USER_ID,        // GPIO00 D3 Wemos Button Shield
//     GPIO_USER_ID,        // GPIO01 TX Serial RXD
//     GPIO_USER_ID,        // GPIO02 D4 Wemos DHT Shield
//     GPIO_USER_ID,        // GPIO03 RX Serial TXD and Optional sensor
//     GPIO_USER_ID,        // GPIO04 D2 Wemos I2C SDA
//     GPIO_USER_ID,        // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
//                       // GPIO06 (SD_CLK   Flash)
//                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//     GPIO_USER_ID,        // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//     GPIO_USER_ID,        // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//                       // GPIO11 (SD_CMD   Flash)
//     GPIO_USER_ID,        // GPIO12 D6
//     GPIO_USER_ID,        // GPIO13 D7
//     GPIO_USER_ID,        // GPIO14 D5
//     GPIO_USER_ID,        // GPIO15 D8
//     GPIO_USER_ID,        // GPIO16 D0 Wemos Wake
//     GPIO_FLAG_ADC0_ID    // ADC0 A0 Analog input
//   },

//   // {                     // WEMOS - Any ESP8266/ESP8285 device like WeMos and NodeMCU hardware (ESP8266)
//   //   GPI8_USER,          // GPIO00 D3 Wemos Button Shield
//   //   GPI8_USER,          // GPIO01 TX Serial RXD
//   //   GPI8_USER,          // GPIO02 D4 Wemos DHT Shield
//   //   GPI8_USER,          // GPIO03 RX Serial TXD and Optional sensor
//   //   GPI8_USER,          // GPIO04 D2 Wemos I2C SDA
//   //   GPI8_USER,          // GPIO05 D1 Wemos I2C SCL / Wemos Relay Shield (0 = Off, 1 = On) / Wemos WS2812B RGB led Shield
//   //                       // GPIO06 (SD_CLK   Flash)
//   //                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//   //                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//   //   GPI8_USER,          // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
//   //   GPI8_USER,          // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
//   //                       // GPIO11 (SD_CMD   Flash)
//   //   GPI8_USER,          // GPIO12 D6
//   //   GPI8_USER,          // GPIO13 D7
//   //   GPI8_USER,          // GPIO14 D5
//   //   GPI8_USER,          // GPIO15 D8
//   //   GPI8_USER,          // GPIO16 D0 Wemos Wake
//   //   GPI8_USER           // ADC0 A0 Analog input
//   // },
//   // {                     // SONOFF_4CH - Sonoff 4CH (ESP8285)
//   //   GPI8_KEY1,          // GPIO00 Button 1
//   //   GPI8_USER,          // GPIO01 Serial RXD and Optional sensor
//   //   GPI8_USER,          // GPIO02 Optional sensor
//   //   GPI8_USER,          // GPIO03 Serial TXD and Optional sensor
//   //   GPI8_REL3,          // GPIO04 Sonoff 4CH Red Led and Relay 3 (0 = Off, 1 = On)
//   //   GPI8_REL2,          // GPIO05 Sonoff 4CH Red Led and Relay 2 (0 = Off, 1 = On)
//   //                       // GPIO06 (SD_CLK   Flash)
//   //                       // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
//   //                       // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
//   //   GPI8_KEY2,          // GPIO09 Button 2
//   //   GPI8_KEY3,          // GPIO10 Button 3
//   //                       // GPIO11 (SD_CMD   Flash)
//   //   GPI8_REL1,          // GPIO12 Red Led and Relay 1 (0 = Off, 1 = On) - Link and Power status
//   //   GPI8_LED1_INV,      // GPIO13 Blue Led (0 = On, 1 = Off)
//   //   GPI8_KEY4,          // GPIO14 Button 4
//   //   GPI8_REL4,          // GPIO15 Red Led and Relay 4 (0 = Off, 1 = On)
//   //   0, 0
//   // },
// //   {                     // SONOFF_IFAN03 - Sonoff iFan03 (ESP8285)
// //     GPI8_KEY1,          // GPIO00 WIFI_KEY0 Button 1
// //     GPI8_TXD,           // GPIO01 ESP_TXD Serial RXD connection to P0.5 of RF microcontroller
// //     0,                  // GPIO02 ESP_LOG
// //     GPI8_RXD,           // GPIO03 ESP_RXD Serial TXD connection to P0.4 of RF microcontroller
// //     0,                  // GPIO04 DEBUG_RX
// //     0,                  // GPIO05 DEBUG_TX
// //                         // GPIO06 (SD_CLK   Flash)
// //                         // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
// //                         // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
// //     GPI8_REL1_INV,      // GPIO09 WIFI_O0 Relay 1 (0 = Off, 1 = On) controlling the light
// //     GPI8_BUZZER_INV,    // GPIO10 WIFI_O4 Buzzer (0 = Off, 1 = On)
// //                         // GPIO11 (SD_CMD   Flash)
// //     GPI8_REL3,          // GPIO12 WIFI_O2 Relay 3 (0 = Off, 1 = On) controlling the fan
// //     GPI8_LED1_INV,      // GPIO13 WIFI_CHK Blue Led on PCA (0 = On, 1 = Off) - Link and Power status
// //     GPI8_REL2,          // GPIO14 WIFI_O1 Relay 2 (0 = Off, 1 = On) controlling the fan
// //     GPI8_REL4,          // GPIO15 WIFI_O3 Relay 4 (0 = Off, 1 = On) controlling the fan
// //     0, 0
// //   }
// // };

#endif // ESP8266


#ifdef ESP32
/********************************************************************************************\
 * ESP32 Module templates
\********************************************************************************************/

#define USER_MODULE        255

// #define USE_MODULE_TEMPLATE_CAM_AITHINKER

#define D_MODULE_NAME__ESP32_DEVKIT__CTR "ESP32-DevKit"
DEFINE_PGM_CTR(PM_MODULE_NAME__ESP32_DEVKIT__CTR)        D_MODULE_NAME__ESP32_DEVKIT__CTR;
#ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  #define            D_MODULE_NAME_CAM_AITHINKER_CTR          "Webcam AIThinker"
  DEFINE_PGM_CTR(PM_MODULE_NAME_CAM_AITHINKER_CTR)        D_MODULE_NAME_CAM_AITHINKER_CTR;
#endif // USE_MODULE_TEMPLATE_CAM_AITHINKER

// Default module settings
const uint16_t mHardwarePins::ModuleList_Enabled_IDs[] PROGMEM = {
  MODULE_DOIT_DEVKIT,
  #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  MODULE_CAM_AITHINKER_ID,
  #endif
};

// // !!! Update this list in the same order as kModuleNiceList !!!
const char mHardwarePins::ModuleList_Names_DList[] PROGMEM =
  D_MODULE_NAME__ESP32_DEVKIT__CTR "|"
  #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  D_MODULE_NAME_CAM_AITHINKER_CTR "|"
  #endif 
  ;

// !!! Update this list in the same order as SupportedModules !!!
const mytmplt mHardwarePins::ModuleTemplate_GPIO_Map[] PROGMEM = {
  {                              // WEMOS - Espressif ESP32-DevKitC - Any ESP32 device like WeMos and NodeMCU hardware (ESP32)
    GPIO_USER_ID,            // 0       (I)O                GPIO0, ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK
    GPIO_USER_ID,            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
    GPIO_USER_ID,            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
    GPIO_USER_ID,            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
    GPIO_USER_ID,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
    GPIO_USER_ID,            // 5       IO                  GPIO5, VSPICS0, HS1_DATA6, EMAC_RX_CLK
                                 // 6       IO                  GPIO6, Flash CLK
                                 // 7       IO                  GPIO7, Flash D0
                                 // 8       IO                  GPIO8, Flash D1
    GPIO_USER_ID,            // 9       IO                  GPIO9, Flash D2, U1RXD
    GPIO_USER_ID,            // 10      IO                  GPIO10, Flash D3, U1TXD
                                 // 11      IO                  GPIO11, Flash CMD
    GPIO_USER_ID,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
    GPIO_USER_ID,            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
    GPIO_USER_ID,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
    GPIO_USER_ID,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
    GPIO_USER_ID,            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
    GPIO_USER_ID,            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
    GPIO_USER_ID,            // 18      IO                  GPIO18, VSPICLK, HS1_DATA7
    GPIO_USER_ID,            // 19      IO                  GPIO19, VSPIQ, U0CTS, EMAC_TXD0
    0,                           // 20
    GPIO_USER_ID,            // 21      IO                  GPIO21, VSPIHD, EMAC_TX_EN
    GPIO_USER_ID,            // 22      IO      LED         GPIO22, VSPIWP, U0RTS, EMAC_TXD1
    GPIO_USER_ID,            // 23      IO                  GPIO23, VSPID, HS1_STROBE
    0,                           // 24
    GPIO_USER_ID,            // 25      IO                  GPIO25, DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0
    GPIO_USER_ID,            // 26      IO                  GPIO26, DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1
    GPIO_USER_ID,            // 27      IO                  GPIO27, ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV
    0,                           // 28
    0,                           // 29
    0,                           // 30
    0,                           // 31
    GPIO_USER_ID,            // 32      IO                  GPIO32, XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9
    GPIO_USER_ID,            // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
    GPIO_USER_ID,            // 34      I   NO PULLUP       GPIO34, ADC1_CH6, RTC_GPIO4
    GPIO_USER_ID,            // 35      I   NO PULLUP       GPIO35, ADC1_CH7, RTC_GPIO5
    GPIO_USER_ID,            // 36      I   NO PULLUP       GPIO36, SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0
    0,                           // 37          NO PULLUP
    0,                           // 38          NO PULLUP
    GPIO_USER_ID,            // 39      I   NO PULLUP       GPIO39, SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3
    0                            // Flag
  },
  #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
  {                              // ESP32_CAM_AITHINKER - Any ESP32 device with webcam (ESP32)
    GPIO_WEBCAM_XCLK_ID,     // 0       (I)O                GPIO0, CAM_XCLK
    GPIO_USER_ID,            // 1       IO     TXD0         GPIO1, U0TXD, CLK_OUT3, EMAC_RXD2
    GPIO_USER_ID,            // 2       IO                  GPIO2, ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0
    GPIO_USER_ID,            // 3       IO     RXD0         GPIO3, U0RXD, CLK_OUT2
    GPIO_USER_ID,            // 4       IO                  GPIO4, ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER
    GPIO_WEBCAM_DATA1_ID,     // 5       IO                  GPIO5, CAM_DATA1
                                 // 6       IO                  GPIO6, Flash CLK
                                 // 7       IO                  GPIO7, Flash D0
                                 // 8       IO                  GPIO8, Flash D1
    GPIO_USER_ID,            // 9       IO                  GPIO9, Flash D2, U1RXD
    GPIO_USER_ID,            // 10      IO                  GPIO10, Flash D3, U1TXD
                                 // 11      IO                  GPIO11, Flash CMD
    GPIO_USER_ID,            // 12      (I)O                GPIO12, ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3       (If driven High, flash voltage (VDD_SDIO) is 1.8V not default 3.3V. Has internal pull-down, so unconnected = Low = 3.3V. May prevent flashing and/or booting if 3.3V flash is connected and pulled high. See ESP32 datasheet for more details.)
    GPIO_USER_ID,            // 13      IO                  GPIO13, ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER
    GPIO_USER_ID,            // 14      IO                  GPIO14, ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2
    GPIO_USER_ID,            // 15      (I)O                GPIO15, ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3         (If driven Low, silences boot messages from normal boot. Has internal pull-up, so unconnected = High = normal output.)
    GPIO_USER_ID,            // 16      IO                  GPIO16, HS1_DATA4, U2RXD, EMAC_CLK_OUT
    GPIO_USER_ID,            // 17      IO                  GPIO17, HS1_DATA5, U2TXD, EMAC_CLK_OUT_180
    GPIO_WEBCAM_DATA2_ID,  // 18      IO                  GPIO18, CAM_DATA2
    GPIO_WEBCAM_DATA3_ID,  // 19      IO                  GPIO19, CAM_DATA3
    0,                           // 20
    GPIO_WEBCAM_DATA4_ID,  // 21      IO                  GPIO21, CAM_DATA4
    GPIO_WEBCAM_PCLK_ID,     // 22      IO      LED         GPIO22, CAM_PCLK
    GPIO_WEBCAM_HREF_ID,     // 23      IO                  GPIO23, CAM_HREF
    0,                           // 24
    GPIO_WEBCAM_VSYNC_ID,    // 25      IO                  GPIO25, CAM_VSYNC
    GPIO_WEBCAM_SIOD_ID,     // 26      IO                  GPIO26, CAM_SIOD
    GPIO_WEBCAM_SIOC_ID,     // 27      IO                  GPIO27, CAM_SIOC
    0,                           // 28
    0,                           // 29
    0,                           // 30
    0,                           // 31
    GPIO_WEBCAM_PWDN_ID,     // 32      IO                  GPIO32, CAM_PWDN
    GPIO_USER_ID,            // 33      IO                  GPIO33, XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8
    GPIO_WEBCAM_DATA7_ID,  // 34      I   NO PULLUP       GPIO34, CAM_DATA7
    GPIO_WEBCAM_DATA8_ID,  // 35      I   NO PULLUP       GPIO35, CAM_DATA8
    GPIO_WEBCAM_DATA5_ID,  // 36      I   NO PULLUP       GPIO36, CAM_DATA5
    0,                           // 37          NO PULLUP
    0,                           // 38          NO PULLUP
    GPIO_WEBCAM_DATA6_ID,  // 39      I   NO PULLUP       GPIO39, CAM_DATA6
    0                            // Flag
  },
  #endif  // USE_MODULE_TEMPLATE_CAM_AITHINKER

};

/*********************************************************************************************\
 Known templates

{"NAME":"AITHINKER CAM","GPIO":[4992,1,1,1,1,5088,1,1,1,1,1,1,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":1}
{"NAME":"Olimex ESP32-PoE","GPIO":[1,1,1,1,1,1,0,0,5536,1,1,1,1,0,5600,0,0,0,0,5568,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
{"NAME":"wESP32","GPIO":[1,1,1,1,1,1,0,0,0,1,1,1,5568,5600,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1],"FLAG":0,"BASE":1}
{"NAME":"Denky (Teleinfo)","GPIO":[1,1,1,1,5664,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1376,1,1,0,0,0,0,1,5632,1,1,1,0,0,1],"FLAG":0,"BASE":1}

\*********************************************************************************************/

#endif  // ESP32



int8_t mHardwarePins::Tasker(uint8_t function, JsonParserObject obj){

  // DEBUG_PRINT_FUNCTION_NAME_TEST;
  // DEBUG_PRINTF("works=%d\n\r",works);
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_EVERY_MINUTE:
      //ModuleSettings_ShowTemplateLog();
      //ModuleSettings_ShowActiveTemplate();
    break;
    case TASK_LOOP:{

      #ifdef ENABLE_DEBUGFEATURE_SET_PINS_MANUAL_STATES
  
      uint16_t pin = 0;

      // pin = 4;  pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 16; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 17; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 18; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 19; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 21; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 22; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 23; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);

      // pin = 2;  pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 13; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 14; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 27; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 26; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 25; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 33; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);
      // pin = 32; pinMode(pin, OUTPUT); digitalWrite(pin, HIGH);



      #endif // ENABLE_DEBUGFEATURE_SET_PINS_MANUAL_STATES


    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
  }

}

void mHardwarePins::parse_JSONCommand(JsonParserObject obj){}


void mHardwarePins::Pre_Init(){

  #ifdef DEBUG_PIN1_GPIO
    DEBUG_PIN1_INIT();
  #endif
  #ifdef DEBUG_PIN2_GPIO
    DEBUG_PIN2_INIT();
  #endif
  #ifdef DEBUG_PIN3_GPIO
    DEBUG_PIN3_INIT();
  #endif
  #ifdef DEBUG_PIN4_GPIO
    DEBUG_PIN4_INIT();
  #endif
  #ifdef DEBUG_PIN5_GPIO
    DEBUG_PIN5_INIT();
  #endif
  #ifdef DEBUG_PIN6_GPIO
    DEBUG_PIN6_INIT();
  #endif
  #ifdef DEBUG_PIN7_GPIO
    DEBUG_PIN7_INIT();
  #endif
  #ifdef DEBUG_PIN8_GPIO
    DEBUG_PIN8_INIT();
  #endif


  #ifdef USE_FEATURE_DEBUG_PIN_INIT_LOGIC_LEVEL_SEQUENCE_CHECK

  DEBUG_PIN1_SET(1);
  DEBUG_PIN2_SET(1);
  DEBUG_PIN3_SET(1);
  DEBUG_PIN4_SET(1);
  DEBUG_PIN5_SET(1);
  DEBUG_PIN6_SET(1);
  DEBUG_PIN7_SET(1);
  DEBUG_PIN8_SET(1);

  DEBUG_PIN1_SET(0);
  delay(100);
  DEBUG_PIN2_SET(0);
  delay(100);
  DEBUG_PIN3_SET(0);
  delay(100);
  DEBUG_PIN4_SET(0);
  delay(100);
  DEBUG_PIN5_SET(0);
  delay(100);
  DEBUG_PIN6_SET(0);
  delay(100);
  DEBUG_PIN7_SET(0);
  delay(100);
  DEBUG_PIN8_SET(0);
  
  #endif // USE_FEATURE_DEBUG_PIN_INIT_LOGIC_LEVEL_SEQUENCE_CHECK

}


void mHardwarePins::ModuleSettings_ShowTemplateLog()
{
  // AddLog(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),

  // char buffer[50];


  DEBUG_LINE;
  
    // const char* buffer2 = GetModuleNameByID(pCONT_set->Settings.user_template.base, buffer);
    // AddLog(LOG_LEVEL_INFO,PSTR("user_template2.base=%s"),buffer2);

}

void mHardwarePins::ModuleSettings_ShowActiveTemplate()
{
  // AddLog(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),
  char buffer[50];


}


void mHardwarePins::ModuleSettings_FlashSerial()
{
  // AddLog(LOG_LEVEL_INFO,
  //   PSTR(
  //     "Module Full: %s\n\r"
  //     "Name: %s\n\r"
  //     "GPIO: \n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d\n\r\t%s:%d"
  //     "FLAGS: %d"
  //     "BASE: %d"
  //   ),
#ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
// ALOG_TST(PSTR("ARRAY_SIZE(pCONT_set->Settings.user_template.hardware.gp.io)=%d"),ARRAY_SIZE(pCONT_set->Settings.user_template.hardware.gp.io));
#endif // ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  uint8_t real_gpio = 0;
  // DEBUG_PRINTF("\n\r");
  // DEBUG_PRINTF("Template: %s\n\r", pCONT_set->Settings.user_template.full_ctr);
  // DEBUG_PRINTF("Name: %s\n\r", pCONT_set->Settings.user_template.hardware.name);
  for(uint8_t gpio=0;gpio<ARRAY_SIZE(pCONT_set->Settings.user_template.hardware.gp.io);gpio++){
    //get proper gpio number

//change to pin getter
    real_gpio = GetPinByIndex(gpio);
    // switch(gpio){
    //   case 0: real_gpio = 0; break;
    //   case 1: real_gpio = 1; break;
    //   case 2: real_gpio = 2; break;
    //   case 3: real_gpio = 3; break;
    //   case 4: real_gpio = 4; break;
    //   case 5: real_gpio = 5; break;
    //   case 6: real_gpio = 9; break;
    //   case 7: real_gpio = 10; break;
    //   case 8: real_gpio = 12; break;
    //   case 9: real_gpio = 13; break;
    //   case 10: real_gpio = 14; break;
    //   case 11: real_gpio = 15; break;
    //   case 12: real_gpio = 16; break;
    // }

#ifdef ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
    AddLog(LOG_LEVEL_DEV_TEST,PSTR("\t%d(%d):%d"),gpio,real_gpio,pCONT_set->Settings.user_template.hardware.gp.io[gpio]);
#endif
  }
  // DEBUG_PRINTF("FLAGS: %08X\n\r",pCONT_set->Settings.user_template.flags);
  // DEBUG_PRINTF("BASE : %08X\n\r",pCONT_set->Settings.user_template.base);
  // DEBUG_PRINTF("\n\r\n\r"); 

}

int8_t mHardwarePins::GetRealPinNumberFromName(const char* c){

  int8_t pin = -1;
  #ifdef ESP8266
  //nodemcu/wemos named
  if     (strcmp(c,"D3")==0){ pin = 0; }
  else if(strcmp(c,"TX")==0){ pin = 1; }
  else if(strcmp(c,"D4")==0){ pin = 2; }
  else if(strcmp(c,"RX")==0){ pin = 3; }
  else if(strcmp(c,"D2")==0){ pin = 4; }
  else if(strcmp(c,"D1")==0){ pin = 5; }
  else if(strcmp(c,"SD2")==0){ pin = 9; }
  else if(strcmp(c,"SD3")==0){ pin = 10; }
  else if(strcmp(c,"D6")==0){ pin = 12; }
  else if(strcmp(c,"D7")==0){ pin = 13; }
  else if(strcmp(c,"D5")==0){ pin = 14; }
  else if(strcmp(c,"D8")==0){ pin = 15; }
  else if(strcmp(c,"D0")==0){ pin = 16; }
  // else if(strcmp(c,"A0")==0){ pin = 17; }
  else if(strcmp(c,"LBI")==0){ 
    #ifdef LED_BUILTIN
    pin = LED_BUILTIN; 
    #else
    pin = -1;
    #endif   
  }
  // numbered
  else if(strcmp(c,"0")==0){ pin = 0; }
  else if(strcmp(c,"1")==0){ pin = 1; }
  else if(strcmp(c,"2")==0){ pin = 2; }
  else if(strcmp(c,"3")==0){ pin = 3; }
  else if(strcmp(c,"4")==0){ pin = 4; }
  else if(strcmp(c,"5")==0){ pin = 5; }
  else if(strcmp(c,"9")==0){ pin = 9; }
  else if(strcmp(c,"10")==0){ pin = 10; }
  else if(strcmp(c,"12")==0){ pin = 12; }
  else if(strcmp(c,"13")==0){ pin = 13; }
  else if(strcmp(c,"14")==0){ pin = 14; }
  else if(strcmp(c,"15")==0){ pin = 15; }
  else if(strcmp(c,"16")==0){ pin = 16; }
  else{
    pin = -1;
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_ERROR, PSTR("\t\tGetRealPinNumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  }

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_DBM( PSTR("GetRealPinNumberFromName = %d"), pin);
    #endif // ENABLE_LOG_LEVEL_INFO
  #endif // ESP8266

  #ifdef ESP32
  
  // Check for pin_array matching
  char buffer[10];
  for(uint8_t i=0; i<ARRAY_SIZE(gpio_pin_by_index); i++)
  {
    sprintf(buffer,"%d\0",gpio_pin_by_index[i]);
    
      // ALOG_INF(PSTR("NOOOOOOOOOmatched pin %d %d %s %s"),pin,i,c,buffer);

    if(strcmp(c,buffer)==0)
    {
      pin = gpio_pin_by_index[i];
      ALOG_DBM( PSTR("matched pin %d %d"),pin,i);
      break;
    }else{

    }
    
  }

  // Names for pins
  if(strcmp(c,"LBI")==0){ 
    pin = 2;
  }

  // if(pin<0)
  // {
  //   pin = -1;
  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_ERROR, PSTR("\t\tGetRealPinNumberFromName = %d PIN UNKNOWN for \"%s\""), pin, c);
  //   #endif // ENABLE_LOG_LEVEL_COMMANDS
  // }

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_DBM( PSTR("GetRealPinNumberFromName = %d"), pin);
    #endif // ENABLE_LOG_LEVEL_INFO
  #endif // ESP32

  return pin;

}

// int mHardwarePins::jsoneq(const char *json, jsmntok_t *tok, const char *s) {
//   if (tok->type == JSMN_STRING && (int)strlen(s) == tok->len &&
//       strncmp(json + tok->start, s, tok->len) == 0) {
//     return 0;
//   }
//   return -1;
// }



// Return true/false if pin can be used (check for flash pins etc)
bool mHardwarePins::UsuableGPIOPin(uint8_t pin)
{
  // Valid
  if ((pin >= 0) && (pin <= 5)){
    return true;
  }
  // GPIO
  else if (((pin >= 6) && (pin <= 8))||(pin == 11)){
    return false; // Flash pins
  }
  // flash pins on 8265
  else if ((pin == 9) || (pin == 10)){ //(MODULE_WEMOS_ID == pCONT_set->Settings.module) // not on esp8625
    return true; // Flash pins
  }
  // Valid
  else if ((pin >= 12) && (pin <= 16)){
    return true;
  }
  
  return false;
}


uint32_t mHardwarePins::GetPin(uint32_t gpio, uint32_t index) {  // I dont think this should be returning uint32_t here? #D10APR22
  return Pin(gpio, index);
}


/**
 * @brief Searches for physical external pin number that a GPIO function is attached to ie "GetFunctionAttachedToPin"
 * @note The use of IRAM_ATTR means function resides in instruction RAM, so will execute faster than without IRAM_ATTR which would have to retrieve array from flash each time into ram again (slower)
 * @param gpio sensor, driver (eg Relay)
 * @param index if more than one gpio was set (eg Relay1, Relay2)
 * @return -1 if none found, or the pin number real_pin
 */
#ifdef ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED
int16_t IRAM_ATTR mHardwarePins::Pin(uint32_t gpio, uint32_t index, bool enable_debug)
#else
int16_t IRAM_ATTR mHardwarePins::Pin(uint32_t gpio, uint32_t index)
#endif
{
  //  ALOG_TST(PSTR("mHardwarePins::Pin(uint32_t gpio, uint32_t index)(%d,%d)"),gpio,index);
   


  uint16_t real_gpio = gpio + index;

// DEBUG_LINE_HERE;

  if(real_gpio == GPIO_NONE_ID)
  {
    
// DEBUG_LINE_HERE;
    return -1;
  }

  // Get any pins with gpio attached
  for(uint16_t index_pin=0; index_pin<ARRAY_SIZE(pin_attached_gpio_functions); index_pin++)
  {
    #ifdef ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED
    if(enable_debug)
    {
      Serial.printf("real_gpio=%d ->? gpio_functions[%d] ==  %d\n\r",
      real_gpio,
      index_pin,
      pin_attached_gpio_functions[index_pin]
      );
    }
    #endif
    // Function (ie func_gpio) that is attached to pin (saved in array), search for the func_gpio
    if(pin_attached_gpio_functions[index_pin] == real_gpio) // gpio has been used for a pin
    {
      // Serial.printf("%d %d %d\n\r", ARRAY_SIZE(pin_attached_gpio_functions), ARRAY_SIZE(gpio_pin_by_index), index_pin);
      
      #ifdef ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED
      if(enable_debug)
      {
        ALOG_TST(PSTR("Pin(%d,%d)pin_attached_gpio_functions[i] == real_gpio%d %d"),gpio,index,real_gpio, gpio_pin_by_index[index_pin]);
      }
      #endif

      return gpio_pin_by_index[index_pin]; // real_pin
    }
  }

// DEBUG_LINE_HERE;
  return -1; // No pin was assigned with GPIO  
}

/**
 * @brief Convert the real_pin number to its indexed position within pin_array
 * @note 
 * @param real_pin physical external pin number
 * @return index position of pin in array
 */
int8_t mHardwarePins::ConvertRealPinToIndexPin(uint8_t real_pin){
  for(int index_pin=0;index_pin<MAX_GPIO_PIN;index_pin++)
  {
    if(real_pin == gpio_pin_by_index[index_pin])
    {
      return index_pin;
    }
  }
  return -1;
}

/**
 * @brief Convert the real_pin number to its indexed position within pin_array
 * @note 
 * @param real_pin physical external pin number
 * @return index position of pin in array
 */
int8_t mHardwarePins::ConvertIndexPinToRealPin(uint8_t index_pin){
  return gpio_pin_by_index[index_pin];
}

/**
 * @brief Returns true/false is the gpio is used on any pin
 * @note 
 * @param gpio sensor, driver (eg Relay)
 * @param index if more than one gpio was set (eg Relay1, Relay2)
 * @return true/false if GPIO is found
 * 
 * place "enable_debug" with debug flag later
 * 
 * 
 */
// #define ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED


#ifdef ENABLE_DEBUFEATURE_HARDWAREPINS__ENABLE_DEBUG_ON_PINUSED
boolean mHardwarePins::PinUsed(uint32_t gpio, uint32_t index, bool enable_debug)
{
  return (Pin(gpio, index, enable_debug) >= 0);
}
#else
boolean mHardwarePins::PinUsed(uint32_t gpio, uint32_t index)
{
  return (Pin(gpio, index) >= 0);
}
#endif

/**
 * @brief Assigns a pin with a GPIO
 * @note 
 * @param lpin The real_pin number GPIO#
 * @param gpio The actual hardware attached to the pin (eg led, relay, sensor data)
 * @return none
 */
void mHardwarePins::SetPin(uint32_t real_pin, uint32_t gpio) {

  // Adjust real_pin to internal indexpin
  int8_t internal_pin_index = ConvertRealPinToIndexPin(real_pin);

  // uint8_t pin_count = ARRAY_SIZE(gpio_pin_by_index);
  // uint8_t

  // if((internal_pin_index < MAX_USER_PINS) && (internal_pin_index>=0))
  // {
  if(internal_pin_index != -1) // -1 means unset pin
  {

    ALOG_DBM( PSTR("SetPin real_pin=%d  internal_index=%d gpio=%d"),real_pin,internal_pin_index,gpio);

    pin_attached_gpio_functions[internal_pin_index] = gpio;

    // ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "SetPin real_pin=%d  internal_index=%d gpio=%d\t\n\r=====%d%d"),real_pin,internal_pin_index,gpio,pin_attached_gpio_functions[internal_pin_index],internal_pin_index);


  }
  else
  {
    ALOG_ERR( PSTR("Error SetPin %d %d<%d"), gpio, internal_pin_index, MAX_USER_PINS);
  }
}



void mHardwarePins::DigitalWrite(uint32_t gpio_pin, uint32_t state)
{
  DigitalWrite(gpio_pin, 0, state);
  // if (pCONT_set->pin[gpio_pin] < 99) {
        // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_RELAYS "DigitalWrite(%d[%d],%d)"),pCONT_set->pin[gpio_pin],gpio_pin,state);
    // digitalWrite(pCONT_set->pin[gpio_pin], state &1);
  // }
}
/**
 * @note GPIO_FUNC is used here to retrieve the real pin to be set, it is not digitalWrite(physical_pin, val)
 * */
void mHardwarePins::DigitalWrite(uint32_t gpio_pin, uint32_t index, uint32_t state)
{
  // if (PinUsed(gpio_pin, index)) {
  //   digitalWrite(GetPin(gpio_pin, index), state &1);
  // }else{
  // // }
  // digitalWrite(gpio_pin, state &1);

  uint8_t real_pin = Pin(gpio_pin, index);

    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_DBM(PSTR("DigitalWrite gpio_pin=%d, real_pin=%d, index=%d, state=%d"),gpio_pin,real_pin, index, state &1);
    #endif // ENABLE_LOG_LEVEL_COMMANDS
  // if (PinUsed(gpio_pin, index)) {
    digitalWrite(real_pin, state &1);
  // }
}


bool mHardwarePins::DigitalRead(uint32_t gpio_pin, uint32_t index)
{
  // if (PinUsed(gpio_pin, index)) {
  //   digitalWrite(GetPin(gpio_pin, index), state &1);
  // }else{
  // // }
  // digitalWrite(gpio_pin, state &1);

  uint8_t real_pin = Pin(gpio_pin, index);

  //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   ALOG_TST(PSTR("DigitalWrite gpio_pin=%d, real_pin=%d, index=%d, state=%d"),gpio_pin,real_pin, index, state &1);
  //   #endif // ENABLE_LOG_LEVEL_COMMANDS
  // // if (PinUsed(gpio_pin, index)) {
  //   digitalWrite(real_pin, state &1);
  // // }

  return digitalRead(real_pin);
}

// #ifdef USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD


int8_t mHardwarePins::GetPinByIndex(uint8_t index)
{
  return gpio_pin_by_index[index];
}

int8_t mHardwarePins::GetPinIndexedLocation(uint8_t pin_number)
{
  for(uint8_t index = 0;index<MAX_USER_PINS;index++)
  {
    if(GetPinByIndex(index) == pin_number)
    {
      return index;
    }
  }
  return -1;
}

bool mHardwarePins::SetPinFunction(int8_t gpio_pin_number, int8_t pin_function)
{
  int8_t gpio_pin_index_location = GetPinIndexedLocation(gpio_pin_number);
  if(gpio_pin_index_location>=0) // Valid pin option
  {
    pin_attached_gpio_functions[gpio_pin_index_location] = pin_function;
    return true;
  }
  return false;
}

// #endif // USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD

uint8_t mHardwarePins::ModuleNr()
{
  // 0    = User module (255)
  // 1 up = Template module 0 up
  return (USER_MODULE == pCONT_set->Settings.module) ? 0 : pCONT_set->Settings.module +1;
}

bool mHardwarePins::ValidTemplateModule(uint8_t index)
{
  for (uint8_t i = 0; i < sizeof(ModuleList_Enabled_IDs); i++) {
    if (index == pgm_read_byte(ModuleList_Enabled_IDs + i)) {
      return true;
    }
  }
  return false;
}

bool mHardwarePins::ValidModule(uint8_t index)
{
  if (index == USER_MODULE) { return true; }
  return ValidTemplateModule(index);
}

bool mHardwarePins::ValidUserGPIOFunction(uint8_t* pin_array, uint8_t index)
{
  // Outside valid gpio function range
  if((pin_array[index] >= GPIO_SENSOR_END_ID) && (pin_array[index] < GPIO_USER_ID)){
      return false;
    }else{
      return true;
    }
}

bool mHardwarePins::ValidUserGPIOFunction(uint16_t* pin_array, uint8_t index)
{
  // Outside valid gpio function range
  // if((pin_array[index] >= GPIO_SENSOR_END_ID) && (pin_array[index] < GPIO_USER_ID)){
  //     return false;
  //   }else{
      return true;
    // }
}

const char* mHardwarePins::ModuleName()
{
  return AnyModuleName(pCONT_set->Settings.module);
}
const char* mHardwarePins::AnyModuleName(uint8_t index)
{

// ERROR // if(buffer == nullptr){ buffer = pCONT_sup->global_buffer; }
// dont save to nullptr, either take nullptr or gloabl
 
 // char* pointer;
 // if(buffer == nullptr){ pointer = pCONT_sup->global_buffer; }
 // if(buffer != nullptr){ pointer = buffer;// = pCONT_sup->global_buffer; }

 
  // if (USER_MODULE == index) {
  // // ALOG_TST(PSTR(D_LOG_HTTP "USER_MODULE == index"));
  //   return PM_SEARCH_NOMATCH;//pCONT_set->Settings.user_template.hardware.name; //returns pointer
  // } else {
    // ALOG_TST(PSTR(D_LOG_HTTP "USER_MODULE != %d index %s"),index,ModuleTemplate_GPIO_Map[index].name);
    // return ModuleTemplate_GPIO_Map[index].name;

    //Change to use function

    return GetModuleNameByID(index);
    // return pCONT_sup->GetTextIndexed_P(buffer, buflen, index, ModuleList_Names_DList);

    


  // }
}



#ifdef ESP8266
// void GetInternalTemplate(void* ptr, uint32_t module, uint32_t option) {
//   uint8_t module_template = pgm_read_byte(kModuleTemplateList + module);

// //  ALOG_DBG(PSTR("DBG: Template %d, Option %d"), module_template, option);

//   // template8 = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc
//   uint8_t template8[sizeof(mytmplt8285)] = { GPIO_NONE };
//   if (module_template < TMP_WEMOS) {
//     memcpy_P(&template8, &kModules8266[module_template], 6);
//     memcpy_P(&template8[8], &kModules8266[module_template].gp.io[6], 6);
//   } else {
//     memcpy_P(&template8, &kModules8285[module_template - TMP_WEMOS], sizeof(template8));
//   }

// //  AddLogBuffer(LOG_LEVEL_DEBUG, (uint8_t *)&template8, sizeof(mytmplt8285));

//   // template16  = GPIO 0,1,2,3,4,5,9,10,12,13,14,15,16,Adc,Flg
//   uint16_t template16[(sizeof(mytmplt) / 2)] = { GPIO_NONE };
//   TemplateConvert(template8, template16);

//   uint32_t index = 0;
//   uint32_t size = sizeof(mycfgio);      // template16[module_template].gp
//   switch (option) {
//     case 2: {
//       index = (sizeof(mytmplt) / 2) -1; // template16[module_template].flag
//       size = 2;
//       break;
//     }
//     case 3: {
//       size = sizeof(mytmplt);           // template16[module_template]
//       break;
//     }
//   }
//   memcpy(ptr, &template16[index], size);

// //  ALOG_DBG(PSTR("FNC: GetInternalTemplate option %d"), option);
// //  AddLogBufferSize(LOG_LEVEL_DEBUG, (uint8_t *)ptr, size / 2, 2);
// }
#endif  // ESP8266

gpio_flag mHardwarePins::ModuleFlag()
{
  gpio_flag flag;

  if (USER_MODULE == pCONT_set->Settings.module) {
    flag = pCONT_set->Settings.user_template.hardware.flag;
  } else {
    memcpy_P(&flag, &ModuleTemplate_GPIO_Map[pCONT_set->Settings.module].flag, sizeof(gpio_flag));
  }

  return flag;
}

void mHardwarePins::ModuleDefault(uint8_t module)
{
  if (USER_MODULE == module) { module = MODULE_DEFAULT; }  // Generic
  pCONT_set->Settings.user_template.base = module;
  memcpy_P(&pCONT_set->Settings.user_template.hardware, &ModuleTemplate_GPIO_Map[module], sizeof(mytmplt));

  // if (USER_MODULE == module) { module = WEMOS; }  // Generic
  // Settings.user_template_base = module;

// #ifdef ESP32
//   module = ModuleTemplate(module);
// #endif

//   char name[TOPSZ];
//   SettingsUpdateText(SET_TEMPLATE_NAME, GetTextIndexed(name, sizeof(name), module, ModuleTemplate_Names_DList));
// #ifdef ESP8266
//   GetInternalTemplate(&Settings.user_template, module, 3);
// #endif  // ESP8266
// #ifdef ESP32
//   memcpy_P(&Settings.user_template, &ModuleTemplate_GPIO_Map[module], sizeof(mytmplt));
// #endif  // ESP32
}

void mHardwarePins::SetModuleType()
{
  pCONT_set->runtime.my_module_type = (USER_MODULE == pCONT_set->Settings.module) ? pCONT_set->Settings.user_template.base : pCONT_set->Settings.module;
}

/**
 * @brief Fixed GPIO to U16 range
 * 
 * @param pin 
 * @param gpio 
 * @return uint16_t 
 */
uint16_t mHardwarePins::ValidPin_AdjustGPIO(uint8_t pin, uint16_t gpio)
{
  uint16_t result = gpio;

  // #ifdef ESP8266

  // DEBUG_LINE;
  // if (((pin > 5) && (pin < 9)) || (11 == pin)) {
  //   result = GPIO_NONE_ID;  // Disable flash pins GPIO6, GPIO7, GPIO8 and GPIO11
  // }
  // DEBUG_LINE;
  // // need to add my other boards here
  // if ((MODULE_WEMOS_ID == pCONT_set->Settings.module) && (!pCONT_set->Settings.flag_network.user_esp8285_enable)) {
  //   if ((pin == 9) || (pin == 10)) { result = GPIO_NONE_ID; }  // Disable possible flash GPIO9 and GPIO10
  // }
  // DEBUG_LINE;

  // #endif

  return result;
}

bool mHardwarePins::ValidGPIO(uint8_t pin, uint16_t gpio)
{
  return (GPIO_USER_ID == ValidPin_AdjustGPIO(pin, gpio));  // Only allow GPIO_USER pins
}

bool mHardwarePins::GetUsedInModule(uint8_t val, uint8_t *arr)
{
  int offset = 0;

  if (!val) { return false; }  // None

  // if ((val >= GPIO_REL1_ID) && (val < GPIO_REL1_ID + MAX_RELAYS)) {
  //   offset = (GPIO_REL1_INV_ID - GPIO_REL1_ID);
  // }
  // if ((val >= GPIO_REL1_INV_ID) && (val < GPIO_REL1_INV_ID + MAX_RELAYS)) {
  //   offset = -(GPIO_REL1_INV_ID - GPIO_REL1_ID);
  // }

  // if ((val >= GPIO_LED1_ID) && (val < GPIO_LED1_ID + MAX_LEDS)) {
  //   offset = (GPIO_LED1_INV_ID - GPIO_LED1_ID);
  // }
  // if ((val >= GPIO_LED1_INV_ID) && (val < GPIO_LED1_INV_ID + MAX_LEDS)) {
  //   offset = -(GPIO_LED1_INV_ID - GPIO_LED1_ID);
  // }

  if ((val >= GPIO_PWM1_ID) && (val < GPIO_PWM1_ID + MAX_PWMS)) {
    offset = (GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
  }
  if ((val >= GPIO_PWM1_INV_ID) && (val < GPIO_PWM1_INV_ID + MAX_PWMS)) {
    offset = -(GPIO_PWM1_INV_ID - GPIO_PWM1_ID);
  }

  if ((val >= GPIO_CNTR1_ID) && (val < GPIO_CNTR1_ID + MAX_COUNTERS)) {
    offset = (GPIO_CNTR1_NP_ID - GPIO_CNTR1_ID);
  }
  if ((val >= GPIO_CNTR1_NP_ID) && (val < GPIO_CNTR1_NP_ID + MAX_COUNTERS)) {
    offset = -(GPIO_CNTR1_NP_ID - GPIO_CNTR1_ID);
  }

  for (uint8_t i = 0; i < MAX_GPIO_PIN; i++) {
    if (arr[i] == val) { return true; }
    if (arr[i] == val + offset) { return true; }
  }
  return false;
}



void mHardwarePins::TemplateJson()
{
  // Response_P(PSTR("{\"" D_NAME "\":\"%s\",\"" D_GPIO "\":["), pCONT_set->Settings.user_template.name);
  // for (uint8_t i = 0; i < sizeof(pCONT_set->Settings.user_template.hardware.gp); i++) {
  //   ResponseAppend_P(PSTR("%s%d"), (i>0)?",":"", pCONT_set->Settings.user_template.hardware.gp.io[i]);
  // }
  // ResponseAppend_P(PSTR("],\"" D_FLAG "\":%d,\"" D_BASE "\":%d}"), pCONT_set->Settings.user_template.flag, pCONT_set->Settings.user_template_base +1);
}






int16_t mHardwarePins::GetModuleIDbyName(const char* c){
  if(*c=='\0'){ return -1; }
  #ifdef ESP8266
    if(strcmp_P(c,PM_MODULE_NAME_SONOFF_BASIC_CTR)==0){    return MODULE_SONOFF_BASIC_ID; }
    if(strcmp_P(c,PM_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR)==0){    return MODULE_SONOFF_BASIC_EXTERNAL_ID; }
    else if(strcmp_P(c,PM_MODULE_NAME_H801_CTR)==0){            return MODULE_H801_ID; }
    else if(strcmp_P(c,PM_MODULE_NAME_MAGICHOME_CTR)==0){       return MODULE_MAGICHOME_ID; }
    else if(strcmp_P(c,PM_MODULE_NAME_SHELLY1_CTR)==0){         return MODULE_SHELLY1_ID; }
    else if(strcmp_P(c,PM_MODULE_NAME_SHELLY2P5_CTR)==0){         return MODULE_SHELLY2P5_ID; }
    else if(strcmp_P(c,PM_MODULE_NAME_SHELLY_DIMMER2_CTR)==0){         return MODULE_SHELLY_DIMMER2_ID; }
    #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_IFAN03_CTR)==0){         return MODULE_SONOFF_IFAN03_ID; }
    #endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
    else if(strcmp_P(c,PM_MODULE_NAME_SONOFF_4CHPRO_CTR)==0){         return MODULE_SONOFF_4CHPRO_ID; }
  #endif
  #ifdef ESP32
    else if(strcmp_P(c,PM_MODULE_NAME__ESP32_DEVKIT__CTR)==0){         return MODULE_DOIT_DEVKIT; }
    #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
    else if(strcmp_P(c,PM_MODULE_NAME_CAM_AITHINKER_CTR)==0){         return MODULE_CAM_AITHINKER_ID; }
    #endif    
  #endif

  else if(strcmp_P(c,PM_MODULE_NAME_USERMODULE_CTR)==0){      return USER_MODULE; }

  return -1;

}

/*
Change to return pointer, not a buffer 
*/
const char* mHardwarePins::GetModuleNameByID(uint8_t id){
  switch(id){
    default:
    #ifdef ESP8266
      case MODULE_SONOFF_BASIC_ID:               return PM_MODULE_NAME_SONOFF_BASIC_CTR;
      case MODULE_SONOFF_BASIC_EXTERNAL_ID:      return PM_MODULE_NAME_SONOFF_BASIC_EXTERNAL_CTR;
      #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
      case MODULE_SONOFF_IFAN03_ID:              return PM_MODULE_NAME_SONOFF_IFAN03_CTR;
      #endif
      case MODULE_SONOFF_4CHPRO_ID:              return PM_MODULE_NAME_SONOFF_4CHPRO_CTR;
      case MODULE_H801_ID:                       return PM_MODULE_NAME_H801_CTR;
      case MODULE_MAGICHOME_ID:                  return PM_MODULE_NAME_MAGICHOME_CTR;
      case MODULE_SHELLY1_ID:                    return PM_MODULE_NAME_SHELLY1_CTR;
      case MODULE_SHELLY2P5_ID:                  return PM_MODULE_NAME_SHELLY2P5_CTR;
      case MODULE_SHELLY_DIMMER2_ID:             return PM_MODULE_NAME_SHELLY_DIMMER2_CTR;
    #endif  // ESP8266
    #ifdef ESP32
      case MODULE_DOIT_DEVKIT:                   return PM_MODULE_NAME__ESP32_DEVKIT__CTR;
      #ifdef USE_MODULE_TEMPLATE_CAM_AITHINKER
      case MODULE_CAM_AITHINKER_ID:              return PM_MODULE_NAME_CAM_AITHINKER_CTR;
      #endif    
    #endif
    case USER_MODULE:                            return PM_MODULE_NAME_USERMODULE_CTR;

  }
}


// Function: Checks for gpio function as stored in progmem, returning its identifier if true, else -1
// Returns : #>=0:id found, -1: not found
// Notes: "else" not included in if structure as return is used 
//
int16_t mHardwarePins::GetGPIOFunctionIDbyName(const char* c){
  if(*c=='\0'){ return -1; }

  char buffer[50];  // Buffer to hold the formatted string (e.g., "SWT32" or "SWT1 Inv")
  
  if(strcmp_P(c,PM_GPIO_FUNCTION_NONE_CTR)==0){       return GPIO_NONE_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_DATA1_CTR)==0){   return GPIO_RGB_DATA_ID; } // force legacy pin version for now
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_DATA_CTR)==0){   return GPIO_RGB_DATA_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_RGB_CLOCK_CTR)==0){  return GPIO_RGB_CLOCK_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_UNUSED_FORCED_HIGH_CTR)==0){   return GPIO_UNUSED_FORCED_HIGH_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_UNUSED_FORCED_LOW_CTR)==0){  return GPIO_UNUSED_FORCED_LOW_ID; }

  



  #ifdef USE_MODULE_SENSORS_DHT
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_1_CTR)==0){  return GPIO_DHT22_1OF2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DHT22_2_CTR)==0){  return GPIO_DHT22_2OF2_ID; }
  #endif

  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_1_CTR)==0){  return GPIO_DSB_1OF2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DS18X20_2_CTR)==0){  return GPIO_DSB_2OF2_ID; }
  #endif

  
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR)==0){  return GPIO_LDR_BASIC_DIGITAL1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR)==0){   return GPIO_LDR_BASIC_ANALOG1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR)==0){  return GPIO_LDR_BASIC_DIGITAL2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR)==0){   return GPIO_LDR_BASIC_ANALOG2_ID; }
  #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  #ifdef USE_MODULE_SENSORS_SWITCHES
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_NP_CTR)==0){  return GPIO_SWT1_NP_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_NP_CTR)==0){  return GPIO_SWT2_NP_ID; }

  
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_INV_CTR)==0){  return GPIO_SWT1_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_INV_CTR)==0){  return GPIO_SWT2_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT3_INV_CTR)==0){  return GPIO_SWT3_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT4_INV_CTR)==0){  return GPIO_SWT4_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT5_INV_CTR)==0){  return GPIO_SWT5_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT6_INV_CTR)==0){  return GPIO_SWT6_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT7_INV_CTR)==0){  return GPIO_SWT7_INV_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT8_INV_CTR)==0){  return GPIO_SWT8_INV_ID; }

  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT1_CTR)==0){  return GPIO_SWT1_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT2_CTR)==0){  return GPIO_SWT2_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT3_CTR)==0){  return GPIO_SWT3_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT4_CTR)==0){  return GPIO_SWT4_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT5_CTR)==0){  return GPIO_SWT5_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT6_CTR)==0){  return GPIO_SWT6_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT7_CTR)==0){  return GPIO_SWT7_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SWT8_CTR)==0){  return GPIO_SWT8_ID; }

  // Check for "SWT1" to "SWT32"
  for (int i = 1; i <= 32; i++) {
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_ID + (i - 1);  // Return the corresponding ID (GPIO_SWT1_ID to GPIO_SWT32_ID)
    }

    // Check inverted switches "SWT1 Inv" to "SWT32 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_INV_ID + (i - 1);  // Return the corresponding inverted switch ID
    }

    // Check non-pull switches "SWT1_NP" to "SWT32_NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_NP_ID + (i - 1);  // Return the corresponding non-pull switch ID
    }

    // Check inverted non-pull switches "SWT1 Inv NP" to "SWT32 Inv NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_SWT1_INV_NP_ID + (i - 1);  // Return the corresponding inverted non-pull switch ID
    }
  }
  #endif

  #ifdef USE_MODULE_DRIVERS_RELAY
  if(strcmp_P(c,PM_GPIO_FUNCTION_REL1_INV_CTR)==0){  return GPIO_REL1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL2_INV_CTR)==0){  return GPIO_REL2_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL3_INV_CTR)==0){  return GPIO_REL3_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL4_INV_CTR)==0){  return GPIO_REL4_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL1_CTR)==0){       return GPIO_REL1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL2_CTR)==0){       return GPIO_REL2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL3_CTR)==0){       return GPIO_REL3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_REL4_CTR)==0){       return GPIO_REL4_ID; }
  #endif
  
  #ifdef USE_MODULE_CONTROLLER_FAN
  else if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_PWM1_CTR)==0){  return GPIO_FAN_PWM1_ID; }
  #endif

  #ifdef USE_MODULE_CORE_I2C
  if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SCL_CTR)==0){  return GPIO_I2C_SCL_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_I2C_SDA_CTR)==0){  return GPIO_I2C_SDA_ID; }
  #endif

  
  #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
  else if(strcmp_P(c,PM_GPIO_FUNCTION_OLED_RESET_CTR)==0){  return GPIO_OLED_RESET_ID; }
  #endif

  #ifdef USE_MODULE_SENSORS_PIR
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_CTR)==0){  return GPIO_PIR_1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_CTR)==0){  return GPIO_PIR_2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_CTR)==0){  return GPIO_PIR_3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_1_INV_CTR)==0){  return GPIO_PIR_1_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_2_INV_CTR)==0){  return GPIO_PIR_2_INV_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PIR_3_INV_CTR)==0){  return GPIO_PIR_3_INV_ID; }
  #endif
  
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
  else if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_INPUT_CTR)==0){  return GPIO_CHIME_INPUT_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_CHIME_RINGER_CTR)==0){  return GPIO_CHIME_RINGER_ID; }
  #endif

  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  else if(strcmp_P(c,PM_GPIO_FUNCTION_BUZZER_CTR)==0){  return GPIO_BUZZER_ID; }
  #endif


  




  // else if(strcmp_P(c,PM_GPIO_FUNCTION_IRSEND_CTR)==0){  return GPIO_IRSEND_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_FAN_IRSEND_CTR)==0){  return GPIO_FAN_IRSEND_ID; }


  #ifdef USE_MODULE_DRIVERS_PWM
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM1_CTR)==0){  return GPIO_PWM1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM2_CTR)==0){  return GPIO_PWM2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM3_CTR)==0){  return GPIO_PWM3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM4_CTR)==0){  return GPIO_PWM4_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PWM5_CTR)==0){  return GPIO_PWM5_ID; }
  #endif

  #ifdef USE_MODULE_CORE__SERIAL
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR)==0){  return GPIO_HWSERIAL0_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR)==0){  return GPIO_HWSERIAL0_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR)==0){  return GPIO_HWSERIAL1_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR)==0){  return GPIO_HWSERIAL1_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR)==0){  return GPIO_HWSERIAL2_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR)==0){  return GPIO_HWSERIAL2_RX_ID; }
  #endif

  #ifdef ESP32
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL1_RING_BUFFER_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL1_RING_BUFFER_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR)==0){  return GPIO_HWSERIAL2_RING_BUFFER_TX_ID; }
  #endif
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR)==0){  return GPIO_GPS_SERIAL0_TX_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR)==0){  return GPIO_GPS_SERIAL0_RX_ID; }
  #ifdef ESP32
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR)==0){  return GPIO_GPS_SERIAL1_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR)==0){  return GPIO_GPS_SERIAL1_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR)==0){  return GPIO_GPS_SERIAL2_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR)==0){  return GPIO_GPS_SERIAL2_RX_ID; }
  #endif // 
  #endif

  
  //else if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_CTR)==0){  return GPIO_LEDLNK_ID; }
  //else if(strcmp_P(c,PM_GPIO_FUNCTION_LEDLNK_INV_CTR)==0){  return GPIO_LEDLNK_INV_ID; }


  #ifdef USE_MODULE_SENSORS_DOOR
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_OPEN_CTR)==0){  return GPIO_DOOR_OPEN_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_DOOR_LOCK_CTR)==0){  return GPIO_DOOR_LOCK_ID; }
  #endif

  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR)==0){  return GPIO_SERIAL_DEBUG_TX_ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR)==0){  return GPIO_SERIAL_DEBUG_RX_ID; }


  


  #ifdef USE_MODULE_DRIVERS_LEDS
  for (int i = 1; i <= 8; i++) {
    // Check normal LEDs "LED1" to "LED8"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_LED1_ID + (i - 1);  // Return the corresponding LED ID
    }

    // Check inverted LEDs "LED1 Inv" to "LED8 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_LED_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_LED1_INV_ID + (i - 1);  // Return the corresponding inverted LED ID
    }
  }
  #endif
  

  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_TX_CTR)==0){  return GPIO_PZEM0XX_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM004_RX_CTR)==0){  return GPIO_PZEM004_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR)==0){  return GPIO_PZEM0XX_RX_MODBUS_ID; }
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_ECHO_CTR)==0){  return GPIO_SR04_ECHO_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SR04_TRIG_CTR)==0){  return GPIO_SR04_TRIG_ID; }
  #endif
  // #ifdef USE_MODULE_SENSORS_ULTRASONICS
  // else if(strcmp_P(c,PM_GPIO_FUNCTION__ROTARY_ENCODER_A__CTR)==0){  return GPIO__ROTARY_ENCODER_A__ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION__ROTARY_ENCODER_B__CTR)==0){  return GPIO__ROTARY_ENCODER_B__ID; }
  // #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS

  for (int i = 1; i <= 32; i++) {
    // Check normal keys "KEY1" to "KEY32"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_ID + (i - 1);  // Return the corresponding key ID
    }

    // Check inverted keys "KEY1 Inv" to "KEY32 Inv"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_INV_ID + (i - 1);  // Return the corresponding inverted key ID
    }

    // Check non-pull keys "KEY1_NP" to "KEY32_NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_NP_ID + (i - 1);  // Return the corresponding non-pull key ID
    }

    // Check inverted non-pull keys "KEY1 Inv NP" to "KEY32 Inv NP"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, i);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_INV_NP_ID + (i - 1);  // Return the corresponding inverted non-pull key ID
    }

    // Check touch keys "KEY1_TOUCH" to "KEY32_TOUCH"
    snprintf_P(buffer, sizeof(buffer), PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, i);
    ALOG_INF(PSTR("c=%s to %s"),c,buffer);
    if (strcmp_P(c, buffer) == 0) {
      return GPIO_KEY1_TOUCH_ID + (i - 1);  // Return the corresponding touch key ID
    }
  }

  #endif

  // else if(strcmp_P(c,PM_GPIO_FUNCTION_KEY1_PULLDOWN_CTR)==0){  return GPIO_KEY1_PULLDOWN_ID; }


  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  else if(strcmp_P(c,PM_GPIO_FUNCTION__RF_433MHZ_RX__CTR)==0){  return GPIO_RF_433MHZ_RX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__RF_433MHZ_TX__CTR)==0){  return GPIO_RF_433MHZ_TX_ID; }
  #endif

  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  else if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_TX_CTR)==0){  return GPIO_NEXTION_TX_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_NEXTION_RX_CTR)==0){  return GPIO_NEXTION_RX_ID; }
  #endif

  #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_POWER_KEY__CTR)==0){  return GPIO_FUNCTION__FONA_POWER_KEY__ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_POWER_STATUS__CTR)==0){  return GPIO_FUNCTION__FONA_POWER_STATUS__ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR)==0){  return GPIO_FUNCTION__FONA_NETWORK_STATUS__ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_RESET__CTR)==0){  return GPIO_FUNCTION__FONA_RESET__ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_UART_TX__CTR)==0){  return GPIO_FUNCTION__FONA_UART_TX__ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_UART_RX__CTR)==0){  return GPIO_FUNCTION__FONA_UART_RX__ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR)==0){  return GPIO_FUNCTION__FONA_RING_INDICATOR__ID; }
  #endif // 

  
  // else if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR)==0){  return GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_TX__CTR)==0){  return GPIO_FUNCTION__MODEM_TX__ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_RX__CTR)==0){  return GPIO_FUNCTION__MODEM_RX__ID; }
  // else if(strcmp_P(c,PM_GPIO_FUNCTION__MODEM_POWER__CTR)==0){  return GPIO_FUNCTION__MODEM_POWER__ID; }



  #ifdef USE_MODULE_DRIVERS_HBRIDGE
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR)==0){  return GPIO_HBRIDGE_L9110_IA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR)==0){  return GPIO_HBRIDGE_L9110_IB_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR)==0){  return GPIO_HBRIDGE_L9110_OA_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR)==0){  return GPIO_HBRIDGE_L9110_OB_ID; }
  #endif


  // else if(strcmp_P(c,PM_GPIO_FUNCTION_ANALOG_POSITION_CTR)==0){  return GPIO_ANALOG_POSITION_ID; }

  /******************************************************************************************************************
   * Energy
  *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  else if(strcmp_P(c,PM_GPIO_FUNCTION_ADE7953_IRQ_CTR)==0){  return GPIO_ADE7953_IRQ_ID; }
  #endif

  
  #ifdef ESP32
  if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH4_CTR)==0){  return GPIO_ADC1_CH4_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH6_CTR)==0){  return GPIO_ADC1_CH6_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_ADC1_CH7_CTR)==0){  return GPIO_ADC1_CH7_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR)==0){  return GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID; }

  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  else if(strcmp_P(c,PM_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR)==0){  return GPIO_FUNCTION_RXON_SAMPLING_ENABLED_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR)==0){  return GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_ID; }
  #endif //
  

  // Webcam
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR)==0){  return GPIO_WEBCAM_DATA1_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR)==0){  return GPIO_WEBCAM_DATA2_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR)==0){  return GPIO_WEBCAM_DATA3_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR)==0){  return GPIO_WEBCAM_DATA4_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR)==0){  return GPIO_WEBCAM_DATA5_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR)==0){  return GPIO_WEBCAM_DATA6_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR)==0){  return GPIO_WEBCAM_DATA7_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR)==0){  return GPIO_WEBCAM_DATA8_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR)==0){   return GPIO_WEBCAM_XCLK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR)==0){   return GPIO_WEBCAM_PCLK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_HREF_CTR)==0){   return GPIO_WEBCAM_HREF_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR)==0){  return GPIO_WEBCAM_VSYNC_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR)==0){   return GPIO_WEBCAM_SIOD_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR)==0){   return GPIO_WEBCAM_SIOC_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR)==0){   return GPIO_WEBCAM_PWDN_ID; }

  /**
   * SD Card
   **/
  #ifdef USE_MODULE_DRIVERS_SDCARD
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR)==0){    return GPIO_FUNCTION_SDCARD_VSPI_CSO_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR)==0){    return GPIO_FUNCTION_SDCARD_VSPI_CLK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR)==0){   return GPIO_FUNCTION_SDCARD_VSPI_MOSI_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR)==0){   return GPIO_FUNCTION_SDCARD_VSPI_MISO_ID; }

  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR)==0){    return GPIO_FUNCTION_SDCARD_HSPI_CSO_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR)==0){    return GPIO_FUNCTION_SDCARD_HSPI_CLK_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR)==0){   return GPIO_FUNCTION_SDCARD_HSPI_MOSI_ID; }
  else if(strcmp_P(c,PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR)==0){   return GPIO_FUNCTION_SDCARD_HSPI_MISO_ID; }
  #endif // USE_MODULE_DRIVERS_SDCARD


  #endif // ESP32


  return -1;

}


const char* mHardwarePins::GetGPIOFunctionNamebyID(uint16_t id, char* buffer, uint8_t buflen) 
{

    // ALOG_INF(PSTR("---------------id %d"),id);

    #ifdef USE_MODULE_SENSORS_BUTTONS
    // Check for KEY1 to KEY32
    if (id >= GPIO_KEY1_ID && id <= GPIO_KEY32_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_CTR, id - GPIO_KEY1_ID + 1);
      return buffer;
    }
    // Check for KEY1_INV to KEY32_INV
    else if (id >= GPIO_KEY1_INV_ID && id <= GPIO_KEY32_INV_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_CTR, id - GPIO_KEY1_INV_ID + 1);
      return buffer;
    }
    // Check for KEY1_NP to KEY32_NP
    else if (id >= GPIO_KEY1_NP_ID && id <= GPIO_KEY32_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_NP_CTR, id - GPIO_KEY1_NP_ID + 1);
      return buffer;
    }
    // Check for KEY1_INV_NP to KEY32_INV_NP
    else if (id >= GPIO_KEY1_INV_NP_ID && id <= GPIO_KEY32_INV_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_INV_NP_CTR, id - GPIO_KEY1_INV_NP_ID + 1);
      return buffer;
    }
    // Check for KEY1_TOUCH to KEY32_TOUCH
    else if (id >= GPIO_KEY1_TOUCH_ID && id <= GPIO_KEY32_TOUCH_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_KEY_NUM_TOUCH_CTR, id - GPIO_KEY1_TOUCH_ID + 1);
      return buffer;
    }
    #endif // USE_MODULE_SENSORS_BUTTONS

    #ifdef USE_MODULE_SENSORS_SWITCHES
    // Check for SWT1 to SWT32
    if (id >= GPIO_SWT1_ID && id <= GPIO_SWT32_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_CTR, id - GPIO_SWT1_ID + 1);
      return buffer;
    }    
    // Check for SWT1_INV to SWT32_INV
    else if (id >= GPIO_SWT1_INV_ID && id <= GPIO_SWT32_INV_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_CTR, id - GPIO_SWT1_INV_ID + 1);
      return buffer;
    }    
    // Check for SWT1_NP to SWT32_NP
    else if (id >= GPIO_SWT1_NP_ID && id <= GPIO_SWT32_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_NP_CTR, id - GPIO_SWT1_NP_ID + 1);
      return buffer;
    }    
    // Check for SWT1_INV_NP to SWT32_INV_NP
    else if (id >= GPIO_SWT1_INV_NP_ID && id <= GPIO_SWT32_INV_NP_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_SWT_NUM_INV_NP_CTR, id - GPIO_SWT1_INV_NP_ID + 1);
      return buffer;
    }
    #endif // USE_MODULE_SENSORS_SWITCHES

    #ifdef USE_MODULE_DRIVERS_LEDS
    // Check for LED1 to LED8
    if (id >= GPIO_LED1_ID && id <= GPIO_LED8_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_CTR, id - GPIO_LED1_ID + 1);
      return buffer;
    }

    // Check for LED1_INV to LED8_INV
    else if (id >= GPIO_LED1_INV_ID && id <= GPIO_LED8_INV_ID) {
      snprintf_P(buffer, buflen, PM_GPIO_FUNCTION_LED_NUM_INV_CTR, id - GPIO_LED1_INV_ID + 1);
      return buffer;
    }
    #endif


    // Step 2: Use the progmem function for other IDs
    PGM_P progmem_str = GetGPIOFunctionNamebyID_P(id);
    if (progmem_str != PM_SEARCH_NOMATCH) {
        // Copy the progmem string to the buffer
        strncpy_P(buffer, progmem_str, buflen - 1);
        buffer[buflen - 1] = '\0'; // Ensure null-termination
        return buffer;
    }
    
    // Default case if no match found
    snprintf_P(buffer, buflen, PSTR("No Match"));
    return buffer;
}


PGM_P mHardwarePins::GetGPIOFunctionNamebyID_P(uint16_t id){

ALOG_ERR(PSTR("GetGPIOFunctionNamebyID_P GPIO needs moved %d"), id);

  switch(id){
    default:           return PM_SEARCH_NOMATCH;
    case GPIO_NONE_ID: return PM_GPIO_FUNCTION_NONE_CTR;
    // case GPIO_NONE_ID: return PM_GPIO_NONE_CTR;

    // case GPIO_RGB_DATA_ID: return PM_GPIO_FUNCTION_RGB_DATA_CTR;



    #ifdef USE_MODULE_SENSORS_BUTTONS
    // case GPIO_KEY1_ID: return PM_GPIO_FUNCTION_KEY1_CTR;
    // case GPIO_KEY2_ID: return PM_GPIO_FUNCTION_KEY2_CTR;
    // case GPIO_KEY3_ID: return PM_GPIO_FUNCTION_KEY3_CTR;
    
    // case GPIO_KEY1_INV_ID: return PM_GPIO_FUNCTION_KEY1_INV_CTR;
    // case GPIO_KEY2_INV_ID: return PM_GPIO_FUNCTION_KEY2_INV_CTR;
    // case GPIO_KEY3_INV_ID: return PM_GPIO_FUNCTION_KEY3_INV_CTR;
    // case GPIO_KEY4_INV_ID: return PM_GPIO_FUNCTION_KEY4_INV_CTR;
    // case GPIO_KEY5_INV_ID: return PM_GPIO_FUNCTION_KEY5_INV_CTR;
    // case GPIO_KEY6_INV_ID: return PM_GPIO_FUNCTION_KEY6_INV_CTR;
    // case GPIO_KEY7_INV_ID: return PM_GPIO_FUNCTION_KEY7_INV_CTR;
    // case GPIO_KEY8_INV_ID: return PM_GPIO_FUNCTION_KEY8_INV_CTR;

    // case GPIO_KEY1_TOUCH_ID: return PM_GPIO_FUNCTION_KEY1_TOUCH_CTR;
    // case GPIO_KEY2_TOUCH_ID: return PM_GPIO_FUNCTION_KEY2_TOUCH_CTR;
    // case GPIO_KEY3_TOUCH_ID: return PM_GPIO_FUNCTION_KEY3_TOUCH_CTR;
    // case GPIO_KEY4_TOUCH_ID: return PM_GPIO_FUNCTION_KEY4_TOUCH_CTR;
    // case GPIO_KEY5_TOUCH_ID: return PM_GPIO_FUNCTION_KEY5_TOUCH_CTR;
    // case GPIO_KEY6_TOUCH_ID: return PM_GPIO_FUNCTION_KEY6_TOUCH_CTR;
    // case GPIO_KEY7_TOUCH_ID: return PM_GPIO_FUNCTION_KEY7_TOUCH_CTR;
    // case GPIO_KEY8_TOUCH_ID: return PM_GPIO_FUNCTION_KEY8_TOUCH_CTR;
    #endif

    #ifdef USE_MODULE_SENSORS_SWITCHES
    // case GPIO_SWT1_ID: return PM_GPIO_FUNCTION_SWT1_CTR; 
    // case GPIO_SWT2_ID: return PM_GPIO_FUNCTION_SWT2_CTR; 
    // case GPIO_SWT3_ID: return PM_GPIO_FUNCTION_SWT3_CTR; 


    // case  GPIO_SWT1_NP_ID: return PM_GPIO_FUNCTION_SWT1_NP_CTR;        
    //   GPIO_SWT1_NP_ID,
    //   GPIO_SWT2_ID,
    // case  GPIO_SWT2_NP_ID: return PM_GPIO_FUNCTION_SWT2_NP_CTR;        
    //   GPIO_SWT3_ID,
    //   GPIO_SWT3_NP_ID,
    //   GPIO_SWT4_ID,
    //   GPIO_SWT4_NP_ID,
    //   GPIO_SWT5_ID,
    //   GPIO_SWT5_NP_ID,
    //   GPIO_SWT6_ID,
    //   GPIO_SWT6_NP_ID,
    //   GPIO_SWT7_ID,
    //   GPIO_SWT7_NP_ID,
    //   GPIO_SWT8_ID,
    //   GPIO_SWT8_NP_ID,

    #endif


    #ifdef USE_MODULE_DRIVERS_RELAY
    case  GPIO_REL1_ID: return PM_GPIO_FUNCTION_REL1_CTR;           // Relays
    case  GPIO_REL1_INV_ID: return PM_GPIO_FUNCTION_REL1_INV_CTR;           // Relays
    case  GPIO_REL2_ID: return PM_GPIO_FUNCTION_REL2_CTR;           // Relays
    case  GPIO_REL2_INV_ID: return PM_GPIO_FUNCTION_REL2_INV_CTR;           // Relays
    case  GPIO_REL3_ID: return PM_GPIO_FUNCTION_REL3_CTR;           // Relays
    case  GPIO_REL3_INV_ID: return PM_GPIO_FUNCTION_REL3_INV_CTR;           // Relays
    case  GPIO_REL4_ID: return PM_GPIO_FUNCTION_REL4_CTR;           // Relays
    case  GPIO_REL4_INV_ID: return PM_GPIO_FUNCTION_REL4_INV_CTR;           // Relays
    #endif


    #ifdef USE_MODULE_CONTROLLER_FAN
    case  GPIO_FAN_PWM1_ID: return PM_GPIO_FUNCTION_FAN_PWM1_CTR;           // Relays
    #endif 
    
    #if defined(USE_MODULE_DISPLAYS_OLED_SH1106) || defined(USE_MODULE_DISPLAYS_OLED_SSD1306)
    case  GPIO_OLED_RESET_ID: return PM_GPIO_FUNCTION_OLED_RESET_CTR;           // Relays
    #endif

    
    #ifdef USE_MODULE_DRIVERS_PWM
    case GPIO_PWM1_ID:      return PM_GPIO_FUNCTION_PWM1_CTR;
    case GPIO_PWM1_INV_ID:  return PM_GPIO_FUNCTION_PWM1_INV_CTR;
    case GPIO_PWM2_ID:      return PM_GPIO_FUNCTION_PWM2_CTR;
    case GPIO_PWM2_INV_ID:  return PM_GPIO_FUNCTION_PWM2_INV_CTR;
    case GPIO_PWM3_ID:      return PM_GPIO_FUNCTION_PWM3_CTR;
    case GPIO_PWM3_INV_ID:  return PM_GPIO_FUNCTION_PWM3_INV_CTR;
    case GPIO_PWM4_ID:      return PM_GPIO_FUNCTION_PWM4_CTR;
    case GPIO_PWM4_INV_ID:  return PM_GPIO_FUNCTION_PWM4_INV_CTR;
    case GPIO_PWM5_ID:      return PM_GPIO_FUNCTION_PWM5_CTR;
    case GPIO_PWM5_INV_ID:  return PM_GPIO_FUNCTION_PWM5_INV_CTR;
    #endif



//   GPIO_CNTR1_ID,          // Counters
//   GPIO_CNTR1_NP_ID,
//   GPIO_CNTR2_ID,
//   GPIO_CNTR2_NP_ID,
//   GPIO_CNTR3_ID,
//   GPIO_CNTR3_NP_ID,
//   GPIO_CNTR4_ID,
//   GPIO_CNTR4_NP_ID,


#ifdef USE_MODULE_CORE_I2C
  case GPIO_I2C_SCL_ID: return PM_GPIO_FUNCTION_I2C_SCL_CTR;
  case GPIO_I2C_SDA_ID: return PM_GPIO_FUNCTION_I2C_SDA_CTR;
#endif
// #ifdef USE_SPI
//   GPIO_SPI_CS_ID,         // SPI Chip Select
//   GPIO_SPI_DC_ID,         // SPI Data Direction
//   GPIO_SSPI_MISO_ID,      // Software SPI Master Input Slave Output
//   GPIO_SSPI_MOSI_ID,      // Software SPI Master Output Slave Input
//   GPIO_SSPI_SCLK_ID,      // Software SPI Serial Clock
//   GPIO_SSPI_CS_ID,        // Software SPI Chip Select
//   GPIO_SSPI_DC_ID,        // Software SPI Data or Command
// #endif
// #ifdef USE_DISPLAY
//   GPIO_BACKLIGHT_ID,      // Display backlight control
// #endif

  // case GPIO_ANALOG_ID: return PM_GPIO_FUNCTION_ANALOG_CTR;

  #ifdef USE_MODULE_SENSORS_DHT
  case GPIO_DHT11_1OF2_ID: return PM_GPIO_FUNCTION_DHT11_1_CTR;
  case GPIO_DHT11_2OF2_ID: return PM_GPIO_FUNCTION_DHT11_2_CTR;
  case GPIO_DHT22_1OF2_ID: return PM_GPIO_FUNCTION_DHT22_1_CTR;
  case GPIO_DHT22_2OF2_ID: return PM_GPIO_FUNCTION_DHT22_2_CTR;
  #endif // USE_MODULE_SENSORS_DHT

//   GPIO_SI7021_ID,         // iTead SI7021

  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  case GPIO_DSB_1OF2_ID: return PM_GPIO_FUNCTION_DS18X20_1_CTR;
  case GPIO_DSB_2OF2_ID: return PM_GPIO_FUNCTION_DS18X20_2_CTR;
  #endif

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  case GPIO_LDR_BASIC_DIGITAL1_ID: return PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR;
  case GPIO_LDR_BASIC_ANALOG1_ID:  return PM_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR;
  case GPIO_LDR_BASIC_DIGITAL2_ID: return PM_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR;
  case GPIO_LDR_BASIC_ANALOG2_ID:  return PM_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR;
  #endif // USE_MODULE_SENSORS_LDR_BASIC

  
  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  case GPIO_BUZZER_ID: return PM_GPIO_FUNCTION_BUZZER_CTR;
  #endif


// #ifdef USE_WS2812
//   GPIO_RGB_DATA_ID,         // WS2812 Led string
//   GPIO_RGB_CLOCK_ID,         // WS2812 Led string
// #endif
// #ifdef USE_IR_REMOTE
//   GPIO_IRSEND_ID,         // IR remote
// #ifdef USE_IR_RECEIVE
//   GPIO_IRRECV_ID,         // IR receiver
// #endif
// #endif
// #ifdef USE_RC_SWITCH
//   GPIO_RFSEND_ID,         // RF transmitter
//   GPIO_RFRECV_ID,         // RF receiver
// #endif
// #ifdef USE_RF_SENSOR
//   GPIO_RF_SENSOR_ID,      // Rf receiver with sensor decodingGPIO_RF_433MHZ_RX_ID
// #endif
// #ifdef USE_SR04
//   GPIO_SR04_TRIG_ID,      // SR04 Trigger pin
//   GPIO_SR04_ECHO_ID,      // SR04 Echo pin
// #endif
// #ifdef USE_TM1638
//   GPIO_TM16CLK_ID,        // TM1638 Clock
//   GPIO_TM16DIO_ID,        // TM1638 Data I/O
//   GPIO_TM16STB_ID,        // TM1638 Strobe
// #endif
// #ifdef USE_HX711
//   GPIO_HX711_SCK_ID,      // HX711 Load Cell clock
//   GPIO_HX711_DAT_ID,      // HX711 Load Cell data
// #endif
// #if defined(USE_ENERGY_SENSOR) && defined(USE_HLW8012)
//   GPIO_NRG_SEL_ID,        // HLW8012/HLJ-01 Sel output (1 = Voltage)
//   GPIO_NRG_SEL_INV_ID,    // HLW8012/HLJ-01 Sel output (0 = Voltage)
//   GPIO_NRG_CF1_ID,        // HLW8012/HLJ-01 CF1 voltage / current
//   GPIO_HLW_CF_ID,         // HLW8012 CF power
//   GPIO_HJL_CF_ID,         // HJL-01/BL0937 CF power
// #endif


//   GPIO_CSE7766_TX_ID,     // CSE7766 Serial interface (S31 and Pow R2)
//   GPIO_CSE7766_RX_ID,     // CSE7766 Serial interface (S31 and Pow R2)
// #if defined(USE_ENERGY_SENSOR) && defined(USE_MCP39F501)
//   GPIO_MCP39F5_TX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RX_ID,     // MCP39F501 Serial interface (Shelly2)
//   GPIO_MCP39F5_RST_ID,    // MCP39F501 Reset (Shelly2)
// #endif


#if defined(USE_MODULE_ENERGY_PZEM004T_V3) || defined(USE_PZEM_AC) || defined(USE_PZEM_DC)
  case GPIO_PZEM0XX_TX_ID: return PM_GPIO_FUNCTION_PZEM0XX_TX_CTR;
#endif
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  case  GPIO_PZEM004_RX_ID: return PM_GPIO_FUNCTION_PZEM004_RX_CTR;     
#endif   
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  case  GPIO_PZEM0XX_RX_MODBUS_ID: return PM_GPIO_FUNCTION_PZEM0XX_MODBUS__RX_CTR;  
#endif
// // #ifdef USE_PZEM_DC
//   GPIO_PZEM017_RX_ID,     // PZEM-003_ID,017 Serial Modbus interface
// // #endif


// #ifdef USE_SDM120
//   GPIO_SDM120_TX_ID,      // SDM120 Serial interface
//   GPIO_SDM120_RX_ID,      // SDM120 Serial interface
// #endif
// #ifdef USE_SDM630
//   GPIO_SDM630_TX_ID,      // SDM630 Serial interface
//   GPIO_SDM630_RX_ID,      // SDM630 Serial interface
// #endif
// #ifdef USE_SERIAL_BRIDGE
//   GPIO_SBR_TX_ID,         // Serial Bridge Serial interface
//   GPIO_SBR_RX_ID,         // Serial Bridge Serial interface
// #endif
// #ifdef USE_MHZ19
//   GPIO_MHZ_TXD_ID,        // MH-Z19 Serial interface
//   GPIO_MHZ_RXD_ID,        // MH-Z19 Serial interface
// #endif
// #ifdef USE_SENSEAIR
//   GPIO_SAIR_TX_ID,        // SenseAir Serial interface
//   GPIO_SAIR_RX_ID,        // SenseAir Serial interface
// #endif
// #ifdef USE_NOVA_SDS
//   GPIO_SDS0X1_TX_ID,      // Nova Fitness SDS011 Serial interface
//   GPIO_SDS0X1_RX_ID,      // Nova Fitness SDS011 Serial interface
// #endif
// #ifdef USE_PMS5003
//   GPIO_PMS5003_ID,        // Plantower PMS5003 Serial interface
// #endif
// #ifdef USE_TX20_WIND_SENSOR
//   GPIO_TX20_TXD_BLACK_ID, // TX20 Transmission Pin
// #endif
// #ifdef USE_MP3_PLAYER
//   GPIO_MP3_DFR562_ID,     // RB-DFR-562_ID, DFPlayer Mini MP3 Player Serial interface
// #endif
// #ifdef USE_TUYA_DIMMER
//   GPIO_TUYA_TX_ID,        // Tuya Serial interface
//   GPIO_TUYA_RX_ID,        // Tuya Serial interface
// #endif
// #ifdef USE_AZ7798
//   GPIO_AZ_TXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
//   GPIO_AZ_RXD_ID,         // AZ-Instrument 7798 CO2 datalogger Serial interface
// #endif
// #ifdef USE_PN532_HSU
//   GPIO_PN532_TXD_ID,      // PN532 HSU Tx
//   GPIO_PN532_RXD_ID,      // PN532 HSU Rx
// #endif
// #ifdef USE_MGC3130
//   GPIO_MGC3130_XFER_ID,
//   GPIO_MGC3130_RESET_ID,
// #endif
// #ifdef USE_MAX31855
//   GPIO_MAX31855CS_ID,     // MAX31855 Serial interface
//   GPIO_MAX31855CLK_ID,    // MAX31855 Serial interface
//   GPIO_MAX31855DO_ID,     // MAX31855 Serial interface
// #endif
//   GPIO_DI_ID,             // my92x1 PWM input
//   GPIO_DCKI_ID,           // my92x1 CLK input
// #ifdef USE_SM16716
//   GPIO_SM16716_CLK_ID,    // SM16716 CLOCK
//   GPIO_SM16716_DAT_ID,    // SM16716 DATA
//   GPIO_SM16716_SEL_ID,    // SM16716 SELECT
// #endif // USE_SM16716
// #ifdef ROTARY_V1
//   GPIO_ROT1A_ID,          // Rotary switch1 A Pin
//   GPIO_ROT1B_ID,          // Rotary switch1 B Pin
//   GPIO_ROT2A_ID,          // Rotary switch2 A Pin
//   GPIO_ROT2B_ID,          // Rotary switch2 B Pin
// #endif
// #ifdef USE_ARILUX_RF
//   GPIO_ARIRFRCV_ID,       // AliLux RF Receive input
// #endif
// #ifdef USE_HRE
//   GPIO_HRE_CLOCK_ID,
//   GPIO_HRE_DATA
// #endif
// #ifdef USE_SENSOR_DOOR_LOCK
//   GPIO_DOOR_LOCK_ID,
// #endif
// #ifdef USE_MODULE_DRIVERS_HBRIDGE
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_IA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_L9110_OA_ID,
//    GPIO_HBRIDGE_ANALOG_SENSE_ID,
// #endif

  /******************************************************************************************************************
   * Network
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Drivers
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Sensors
  *******************************************************************************************************************/

#ifdef USE_MODULE_SENSORS_DOOR
    case GPIO_DOOR_OPEN_ID: return PM_GPIO_FUNCTION_DOOR_OPEN_CTR;
#endif
    // case GPIO_PIR_1_ID: return PM_GPIO_FUNCTION_PIR_1_CTR;
    // case GPIO_PIR_1_NP_ID: return PM_GPIO_FUNCTION_PIR_1_NP_CTR;
    // case GPIO_PIR_1_INV_ID: return PM_GPIO_FUNCTION_PIR_1_INV_CTR;

    // case GPIO_PIR_2_ID: return PM_GPIO_FUNCTION_PIR_2_CTR;
    // case GPIO_PIR_2_NP_ID: return PM_GPIO_FUNCTION_PIR_2_NP_CTR;
    // case GPIO_PIR_2_INV_ID: return PM_GPIO_FUNCTION_PIR_2_INV_CTR;

    // case GPIO_PIR_3_ID: return PM_GPIO_FUNCTION_PIR_3_CTR;
    // case GPIO_PIR_3_NP_ID: return PM_GPIO_FUNCTION_PIR_3_NP_CTR;
    // case GPIO_PIR_3_INV_ID: return PM_GPIO_FUNCTION_PIR_3_INV_CTR;

  /******************************************************************************************************************
   * Lights
  *******************************************************************************************************************/

  /******************************************************************************************************************
   * Energy
  *******************************************************************************************************************/

  #ifdef USE_MODULE_ENERGY_ADE7953
  case  GPIO_ADE7953_IRQ_ID: return PM_GPIO_FUNCTION_ADE7953_IRQ_CTR;        // ADE7953 IRQ
  #endif


  /******************************************************************************************************************
   * Displays
  *******************************************************************************************************************/
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  case GPIO_NEXTION_RX_ID: return PM_GPIO_FUNCTION_NEXTION_RX_CTR;
  case GPIO_NEXTION_TX_ID: return PM_GPIO_FUNCTION_NEXTION_TX_CTR;
  #endif // USE_MODULE_DISPLAYS_NEXTION


  // case GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__ID: return PM_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR;
  // case GPIO_FUNCTION__MODEM_TX__ID: return PM_GPIO_FUNCTION__MODEM_TX__CTR;
  // case GPIO_FUNCTION__MODEM_RX__ID: return PM_GPIO_FUNCTION__MODEM_RX__CTR;
  // case GPIO_FUNCTION__MODEM_POWER__ID: return PM_GPIO_FUNCTION__MODEM_POWER__CTR;







  #ifdef ESP32

  
  case GPIO_HWSERIAL0_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL0_RX_CTR;
  case GPIO_HWSERIAL0_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL0_TX_CTR;
  case GPIO_HWSERIAL1_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_RX_CTR;
  case GPIO_HWSERIAL1_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_TX_CTR;
  case GPIO_HWSERIAL2_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_RX_CTR;
  case GPIO_HWSERIAL2_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_TX_CTR;



  case GPIO_HWSERIAL1_RING_BUFFER_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR;
  case GPIO_HWSERIAL1_RING_BUFFER_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR;
  case GPIO_HWSERIAL2_RING_BUFFER_RX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR;
  case GPIO_HWSERIAL2_RING_BUFFER_TX_ID: return PM_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR;






  // Webcam
  case GPIO_WEBCAM_DATA1_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA1_CTR;
  case GPIO_WEBCAM_DATA2_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA2_CTR;
  case GPIO_WEBCAM_DATA3_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA3_CTR;
  case GPIO_WEBCAM_DATA4_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA4_CTR;
  case GPIO_WEBCAM_DATA5_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA5_CTR;
  case GPIO_WEBCAM_DATA6_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA6_CTR;
  case GPIO_WEBCAM_DATA7_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA7_CTR;
  case GPIO_WEBCAM_DATA8_ID: return PM_GPIO_FUNCTION_WEBCAM_DATA8_CTR;
  case GPIO_WEBCAM_XCLK_ID: return PM_GPIO_FUNCTION_WEBCAM_XCLK_CTR;
  case GPIO_WEBCAM_PCLK_ID: return PM_GPIO_FUNCTION_WEBCAM_PCLK_CTR;
  case GPIO_WEBCAM_HREF_ID: return PM_GPIO_FUNCTION_WEBCAM_HREF_CTR;
  case GPIO_WEBCAM_VSYNC_ID: return PM_GPIO_FUNCTION_WEBCAM_VSYNC_CTR;
  case GPIO_WEBCAM_SIOD_ID: return PM_GPIO_FUNCTION_WEBCAM_SIOD_CTR;
  case GPIO_WEBCAM_SIOC_ID: return PM_GPIO_FUNCTION_WEBCAM_SIOC_CTR;
  case GPIO_WEBCAM_PWDN_ID: return PM_GPIO_FUNCTION_WEBCAM_PWDN_CTR;


  /**
   * SD Card
   **/
  #ifdef USE_MODULE_DRIVERS_SDCARD
  case GPIO_FUNCTION_SDCARD_VSPI_CSO_ID:    return PM_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR;
  case GPIO_FUNCTION_SDCARD_VSPI_CLK_ID:    return PM_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR;
  case GPIO_FUNCTION_SDCARD_VSPI_MOSI_ID:   return PM_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR;
  case GPIO_FUNCTION_SDCARD_VSPI_MISO_ID:   return PM_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_CSO_ID:    return PM_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_CLK_ID:    return PM_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_MOSI_ID:   return PM_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR;
  case GPIO_FUNCTION_SDCARD_HSPI_MISO_ID:   return PM_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR;
  #endif


  #endif // ESP32


//    case GPIO_LEDLNK_INV_ID: return PM_GPIO_FUNCTION_LEDLNK_INV_CTR;

  }
}





