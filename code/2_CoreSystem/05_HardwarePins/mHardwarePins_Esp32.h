#pragma once

#include <stdint.h>

/****** GPIO ESP32  **** https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * Index   |  GPIO   | DoitDevKit|      Input      |     Output     |    Hardware                                                                   |   Notes                                                                     |   Ranking  
 * ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * [00]    |    0   |            |    pulled up    |       OK       | ADC2_CH1, TOUCH1, RTC_GPIO11, CLK_OUT1, EMAC_TX_CLK                           |  ADC2 not functional with WiFi, outputs PWM signal at boot                  |   2  
 * [01]    |    1   |            |     TX pin	   |       OK       | U0TXD, CLK_OUT3, EMAC_RXD2                                                    |  Debug output at boot                                                       |   2  
 * [02]    |    2   |            |       OK        |       OK       | ADC2_CH2, TOUCH2, RTC_GPIO12, HSPIWP, HS2_DATA0, SD_DATA0                     |  ADC2 not functional with WiFi, connected to on-board LED                   |   2  
 * [03]    |    3   |            |       OK	       |     RX pin     | U0RXD, CLK_OUT2                                                               |  HIGH at boot                                                               |   2  
 * [04]    |    4   |            |       OK        |       OK       | ADC2_CH0, TOUCH0, RTC_GPIO10, HSPIHD, HS2_DATA1, SD_DATA1, EMAC_TX_ER         |  ADC2 not functional with WiFi                                              |   1  
 * [05]    |    5   |            |       OK        |       OK       | VSPICS0, HS1_DATA6, EMAC_RX_CLK                                               |  Outputs PWM signal at boot                                                 |   2  
 * [--]    |    6   |      x     |      Flash      |      Flash     | Flash CLK                                                                     |  Connected to the integrated SPI flash                                      |   N/A  
 * [--]    |    7   |      x     |      Flash      |      Flash     | Flash D0                                                                      |  Connected to the integrated SPI flash                                      |   N/A  
 * [--]    |    8   |      x     |      Flash      |      Flash     | Flash D1                                                                      |  Connected to the integrated SPI flash                                      |   N/A  
 * [06]    |    9   |      *     |      Flash      |      Flash     | Flash D2, U1RXD                                                               |  Connected to the integrated SPI flash                                      |   N/A  
 * [07]    |    10  |      *     |      Flash      |      Flash     | Flash D3, U1TXD                                                               |  Connected to the integrated SPI flash                                      |   N/A  
 * [--]    |    11  |      x     |      Flash      |      Flash     | Flash CMD                                                                     |  Connected to the integrated SPI flash                                      |   N/A  
 * [08]    |    12  |            |       OK        |       OK       | ADC2_CH5, TOUCH5, RTC_GPIO15, MTDI, HSPIQ, HS2_DATA2, SD_DATA2, EMAC_TXD3     |  Bootstrapping: If HIGH, changes flash voltage. Use caution at boot         |   2  
 * [09]    |    13  |            |       OK        |       OK       | ADC2_CH4, TOUCH4, RTC_GPIO14, MTCK, HSPID, HS2_DATA3, SD_DATA3, EMAC_RX_ER    |  ADC2 not functional with WiFi                                              |   1  
 * [10]    |    14  |            |       OK        |       OK       | ADC2_CH6, TOUCH6, RTC_GPIO16, MTMS, HSPICLK, HS2_CLK, SD_CLK, EMAC_TXD2       |  ADC2 not functional with WiFi, outputs PWM signal at boot                  |   2  
 * [11]    |    15  |            |       OK        |       OK       | ADC2_CH3, TOUCH3, MTDO, HSPICS0, RTC_GPIO13, HS2_CMD, SD_CMD, EMAC_RXD3       |  ADC2 not functional with WiFi, outputs PWM signal at boot                  |   2  
 * [12]    |    16  |            |       OK        |       OK       | HS1_DATA4, U2RXD, EMAC_CLK_OUT                                                |                                                                             |   1  
 * [13]    |    17  |            |       OK        |       OK       | HS1_DATA5, U2TXD, EMAC_CLK_OUT_180                                            |                                                                             |   1  
 * [14]    |    18  |            |       OK        |       OK       | VSPICLK, HS1_DATA7                                                            |                                                                             |   1  
 * [15]    |    19  |            |       OK        |       OK       | VSPIQ, U0CTS, EMAC_TXD0                                                       |                                                                             |   1  
 * [16]    |    21  |            |       OK        |       OK       | VSPIHD, EMAC_TX_EN, default_I2C_DATA                                          |                                                                             |   1  
 * [17]    |    22  |            |       OK        |       OK       | VSPIWP, U0RTS, EMAC_TXD1, default_I2C_CLOCK                                   |                                                                             |   1  
 * [18]    |    23  |            |       OK        |       OK       | VSPID, HS1_STROBE                                                             |                                                                             |   1  
 * [--]    |    24  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [19]    |    25  |            |       OK        |       OK       | DAC_1, ADC2_CH8, RTC_GPIO6, EMAC_RXD0                                         |  ADC2 not functional with WiFi                                              |   2  
 * [20]    |    26  |            |       OK        |       OK       | DAC_2, ADC2_CH9, RTC_GPIO7, EMAC_RXD1                                         |  ADC2 not functional with WiFi                                              |   2  
 * [21]    |    27  |            |       OK        |       OK       | ADC2_CH7, TOUCH7, RTC_GPIO17, EMAC_RX_DV                                      |                                                                             |   1  
 * [22]    |    28  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    29  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    30  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    31  |            |       --        |       --       |                                                                               |                                                                             |   N/A  
 * [22]    |    32  |            |       OK        |       OK       | XTAL_32K_P (32.768 kHz crystal oscillator input), ADC1_CH4, TOUCH9, RTC_GPIO9 |                                                                             |   1  
 * [23]    |    33  |            |       OK        |       OK       | XTAL_32K_N (32.768 kHz crystal oscillator output), ADC1_CH5, TOUCH8, RTC_GPIO8|                                                                             |   1  
 * [24]    |    34  |            |       OK        |    input only  | ADC1_CH6, RTC_GPIO4                                                           |  No pull-up                                                                 |   3  
 * [25]    |    35  |            |       OK        |    input only  | ADC1_CH7, RTC_GPIO5                                                           |  No pull-up                                                                 |   3  
 * [26]    |    36  |            |       OK        |    input only  | SENSOR_VP, ADC_H, ADC1_CH0, RTC_GPIO0                                         |  No pull-up                                                                 |   3  
 * [--]    |    37  |            |       --        |       --       |                                                                               |  No pull-up                                                                 |   N/A  
 * [--]    |    38  |            |       --        |       --       |                                                                               |  No pull-up                                                                 |   N/A  
 * [27]    |    39  |            |       OK        |    input only  | SENSOR_VN, ADC1_CH3, ADC_H, RTC_GPIO3                                         |  No pull-up                                                                 |   3  
 * Total: 28 GPIO 
 * 
 * Explanation of Rankings:

    1: Pins with no bootstrapping or output restrictions and can be used freely. (12)
    2: Pins with bootstrapping restrictions (e.g., outputting signals or must not be driven high at boot). (10)
    3: Pins that are input-only and have no internal pull-up resistors, making them less flexible. (4)
    N/A: Reserved for pins that are connected to the integrated SPI flash or unavailable for GPIO use.

    The numbering helps identify the pins most suitable for general usage (rank 1) and the ones with limitations that should be considered before use (rank 2 and 3).                     
 * ******/


#define MAX_GPIO_PIN       40   // Number of supported GPIO
#define MIN_FLASH_PINS     4    // Number of flash chip pins unusable for configuration (GPIO6, 7, 8 and 11)
#define MAX_USER_PINS      36   // MAX_GPIO_PIN - MIN_FLASH_PINS
#define WEMOS_MODULE       0    // Wemos module
// #define USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD


// #ifdef USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD

const uint8_t gpio_pin_by_index[MAX_USER_PINS] = {
    0, 1, 2, 3, 4, 5,
    // 6, 7, 8,
    9, 10, 
    // 11, 
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 
    // 24, 
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 
    // 37, 38, 
    39 
};

// int8_t GetPinByIndex(uint8_t index)
// {
//     return gpio_pin_by_index[index];
// }
// int8_t GetPinIndexedLocation(uint8_t pin_number)
// {
//     for(uint8_t index = 0;index<MAX_USER_PINS;index++)
//     {
//         if(GetPinByIndex(index) == pin_number)
//         {
//             return index;
//         }
//     }
//     return -1;
// }
// bool SetPinFunction(int8_t gpio_pin_number, int8_t pin_function)
// {
//     int8_t gpio_pin_index_location = GetPinIndexedLocation(gpio_pin_number);
//     if(gpio_pin_index_location>=0) // Valid pin option
//     {
//         pCONT_pins->pin_attached_gpio_functions[gpio_pin_index_location] = pin_function;
//     }
// }
// #else

// int8_t GetPinByIndex(uint8_t index)
// {
//     // If statements must be executed in accending order
//     if(index <= 5){
//         return index;
//     }else
//     if(index <= 8){
//         return -1;      // invalid pin
//     }else
//     if(index <= 10){
//         return index-3; // 3 pins skipped
//     }else
//     if(index <= 11){
//         return -1;      // invalid pin
//     }else
//     if(index <= 23){
//         return index-4; // 4 pins skipped
//     }else
//     if(index <= 24){
//         return -1;      // invalid pin
//     }else
//     if(index <= 36){
//         return index-5; // 5 pins skipped
//     }else
//     if(index <= 38){
//         return -1;      // invalid pin
//     }else
//     if(index <= 39){
//         return index-7; // 7 pins skipped
//     }
//     else{
//         return -1;
//     }
// }

// #endif // USE_DEVFEATURE_GPIO_INDEX_ARRAY_METHOD

// This can also be achieved by if checks, ie if(pin in range) pin -= 5;
// If I name the array&function the same, or with a macro, I can switch between both methods
