#ifndef BOARDSETTINGS_H
#define BOARDSETTINGS_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

////////////////////////////////////////////////////////////////////
//uncomment for WAZISENSE v2 board
//#define WAZISENSE

////////////////////////////////////////////////////////////////////
//uncomment for IRD PCB board
//#define IRD_PCB

////////////////////////////////////////////////////////////////////
// uncomment only if the IRD PCB is running on solar panel
// MUST be commented if running on alkaline battery
// code for SOLAR_BAT has been written by Jean-François Printanier from IRD
#define SOLAR_BAT
// do not change if you are not knowing what you are doing
#define NIMH

// For powering humdity sensor and DS18B20
#if defined IRD_PCB && defined SOLAR_BAT

#define PWR_SOFT_START_LOW   20
#define PWR_SOFT_START_HIGH  20

#define PWR_LOW     HIGH  // mosfet P
#define PWR_HIGH    LOW   // mosfet P

void power_soft_start( uint8_t pin);

#else

#define PWR_LOW     LOW   // gpio
#define PWR_HIGH    HIGH  // gpio

#endif

#endif // BOARDSETTINGS_H
