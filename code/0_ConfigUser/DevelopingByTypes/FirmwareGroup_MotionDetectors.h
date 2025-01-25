#ifndef _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H
#define _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are unique with pzem style
 *  ** So not the consumer unit, but standalone basic MOTION_DETECTORS devices that may have additional sensors but are many for 
 *     tracking or controller power (eg treadmill, christmas lights)
 *  ** Will include 100A and 10A variants.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

//    ;;;;;;;;;;;; ESP32 ;;;;;;;;;;;;;;;;
// #define DEVICE_TESTGROUP__MOTION_DETECTORS__SOCKET_ENERGY_01 // for use with outdoor christmas lights


/***
 * 
 * 
 * New device needs to be made that includes as many testers of motion on one board
 * 

 PIR Large
 PIR Small
 TOF
 Ultrasonic
 24GHz Radar

 Use a 144/m strip added to the PCB, where each LED can be used to show when motion is detected by each device
  
  
  
 */


//----------------------------------------


#endif // _CONFIG__FIRMWARE_GROUP__MOTION_DETECTORS_H

