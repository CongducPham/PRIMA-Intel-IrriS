/*
 *  INTEL_IRRIS soil humidity sensor platform
 *  support limited LoRaWAN with raw LoRa SX12XX (such as RFM9X, NiveRF, ...)
 *  support RAK3172 for native LoRaWAN
 *  
 *  Copyright (C) 2016-2023 Congduc Pham, University of Pau, France
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************
 * last update: Feb. 19th, 2024 by C. Pham
 * 
 * NEW: Support native LoRaWAN module RAK3172 with AT commands
 * 
 * NEW: LoRa communicain library moved from Libelium's lib to StuartProject's lib
 * https://github.com/StuartsProjects/SX12XX-LoRa
 * to support SX126X, SX127X and SX128X chips (SX128X is LoRa in 2.4GHz band)
 *  
 */

/********************************************************************
 _____              __ _                       _   _             
/  __ \            / _(_)                     | | (_)            
| /  \/ ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __  
| |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \ 
| \__/\ (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | |
 \____/\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|
                          __/ |                                  
                         |___/                                   
********************************************************************/

//indicate in this file the radio module: SX126X, SX127X, SX128X or RAK3172
#include "RadioSettings.h"
//indicate in this file the board: simple PCB v2, IRD PCB or WaziSense v2
#include "BoardSettings.h"

////////////////////////////////////////////////////////////////////
// sends data to INTEL-IRRIS WaziGate edge-gateway
#define TO_WAZIGATE

////////////////////////////////////////////////////////////////////
// Frequency band - do not change in SX12XX_RadioSettings.h anymore
// if using a native LoRaWAN module such as RAK3172, also select band in RadioSettings.h
#define EU868
//#define AU915 
//#define EU433
//#define AS923_2

////////////////////////////
//uncomment to use a customized frequency.
//#define MY_FREQUENCY 868100000
//#define MY_FREQUENCY 433170000
//#define MY_FREQUENCY 916800000

////////////////////////////////////////////////////////////////////
#define BOOT_START_MSG  "\nINTEL-IRRIS soil humidity sensor – Oct 10th, 2023\n"

////////////////////////////////////////////////////////////////////
// uncomment to have a soil tensiometer watermark sensor
//#define WITH_WATERMARK
// only for watermark sensors, not relevant for capacitive sensors
#define WM_REF_TEMPERATURE 28.0

////////////////////////////////////////////////////////////////////
// uncomment to force the watermark to have default device address for WaziGate
//#define WM_AS_PRIMARY_SENSOR

////////////////////////////////////////////////////////////////////
// uncomment to have 2 tensiometer watermark sensor on the same device
//#define TWO_WATERMARK

////////////////////////////////////////////////////////////////////
// uncomment to have 1 soil temperature sensor ST
// using a one-wire DS18B20 sensor
//#define SOIL_TEMP_SENSOR
// only for watermark sensors, not relevant for capacitive sensors
#define LINK_SOIL_TEMP_TO_CENTIBAR
// use SEN0308 capacitive calibration for low voltage
#define SEN0308_CALIBRATION_LOW_VOLTAGE
// send millivolt with SEN0308 capacitive
//#define SEN0308_TRANSMIT_MILLIVOLT

//uncomment for an onboard SI7021 sensor
//#define SI7021_SENSOR

////////////////////////////////////////////////////////////////////
// uncomment to have an additional decagon EC-5 sensor, ONLY ON IRD_PCB
//#define SOIL_EC5_SENSOR
// uncomment to have an additional CO2 sensor, ONLY ON IRD_PCB
//#define CO2_SCD30_SENSOR

//uncomment to use LPP format to send to WAZIGATE for instance
//so uncomment LPP only with LORAWAN to WAZIGATE
#ifdef TO_WAZIGATE
#define LORAWAN
#define USE_XLPP
//#define USE_LPP
#endif

///////////////////////////////////////////////////////////////////
// COMMENT OR UNCOMMENT TO CHANGE FEATURES. 
// ONLY IF YOU KNOW WHAT YOU ARE DOING!!! OTHERWISE LEAVE AS IT IS
// 
// FOR UPLINK WITH ONLY AES ENCRYTION: uncomment WITH_AES
// FOR UPLINK TO LORAWAN CLOUD ENCAPSULATED LORAWAN FORMAT: uncomment WITH_AES & EXTDEVADDR 
// FOR UPLINK TO LORAWAN CLOUD NATIVE LORAWAN FORMAT: uncomment LORAWAN in RadioSettings.h
// FOR UPLINK TO LORAWAN CLOUD AND DOWNLINK WITH NATIVE LORAWAN FORMAT: uncomment LORAWAN & WITH_RCVW
// 
// more info: https://github.com/CongducPham/LowCostLoRaGw/blob/master/gw_full_latest/README-aes_lorawan.md
//
// DEFAULT CONFIGURATION: SF12BW125, no encryption, no LoRaWAN, no downlink
//
#define WITH_EEPROM
////////////////////////////
//add 4-byte AppKey filtering - only for non-LoRaWAN mode
//#define WITH_APPKEY
////////////////////////////
//request an ack from gateway
//#define WITH_ACK
////////////////////////////
//if you are low on program memory, comment STRING_LIB to save about 2K
//#define STRING_LIB
////////////////////////////
#define LOW_POWER
#define LOW_POWER_HIBERNATE
//#define SHOW_LOW_POWER_CYCLE //uncomment only for debugging and testing
//monitor battery voltage without additional hardware: // https://github.com/Yveaux/arduino_vcc
//if low voltage detected, device will continue to measure and transmit normally 3 times
//then, it will set transmission time interval to 4h
#define MONITOR_BAT_VOLTAGE
//enable transmission of bat voltage in (X)LPP messages
#define TRANSMIT_VOLTAGE
//always transmit bat voltage, otherwise only on low bat
#define ALWAYS_TRANSMIT_VOLTAGE
//force normal measure and transmission even if low voltage detected
//#define BYPASS_LOW_BAT
////////////////////////////

////////////////////////////
//Use LoRaWAN AES-like encryption
//#define WITH_AES
////////////////////////////
//If you want to upload to LoRaWAN cloud without pure LoRaWAN format you have to provide a 4 bytes DevAddr and uncomment #define EXTDEVADDR
//#define EXTDEVADDR
////////////////////////////
//this will enable a receive window after every transmission, uncomment it to also have LoRaWAN downlink
//#define WITH_RCVW
////////////////////////////
//normal behavior is to invert IQ for RX, the normal behavior at gateway is also to invert its IQ setting, only valid with WITH_RCVW
#define INVERTIQ_ON_RX

////////////////////////////////////////////////////////////////////
// Test generation of random values from device
//#define TEST_DEVICE_RANDOM

////////////////////////////
//when sending to a LoRaWAN gateway (e.g. running util_pkt_logger) but with no native LoRaWAN format, just to set the correct sync word
//#define PUBLIC_SYNCWORD
////////////////////////////

///////////////////////////////////////////////////////////////////
// ADD HERE OTHER PLATFORMS THAT DO NOT SUPPORT EEPROM
#if defined ARDUINO_SAM_DUE || defined _VARIANT_ARDUINO_DUE_X_ || defined __SAMD21G18A__
#undef WITH_EEPROM
#endif

///////////////////////////////////////////////////////////////////
// ADD HERE OTHER PLATFORMS THAT DO NOT SUPPORT LOW POWER LIB
#if defined ARDUINO_SAM_DUE || defined _VARIANT_ARDUINO_DUE_X_
#undef LOW_POWER
#endif
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE NODE ADDRESS IN NO LORAWAN MODE
uint8_t node_addr=8;
//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE TIME IN MINUTES BETWEEN 2 READING & TRANSMISSION
unsigned int idlePeriodInMin = 60;
unsigned int idlePeriodInSec = 0;
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE DEFAULT NUMBER OF READING FOR ATTACHED SENSORS
#define NSAMPLE 2
///////////////////////////////////////////////////////////////////

#ifdef WITH_APPKEY
///////////////////////////////////////////////////////////////////
// CHANGE HERE THE APPKEY, BUT IF GW CHECKS FOR APPKEY, MUST BE
// IN THE APPKEY LIST MAINTAINED BY GW.
uint8_t my_appKey[4]={5, 6, 7, 8};
///////////////////////////////////////////////////////////////////
#endif


/********************************************************************
  _        ___    __      ___   _  _ 
 | |   ___| _ \__ \ \    / /_\ | \| |
 | |__/ _ \   / _` \ \/\/ / _ \| .` |
 |____\___/_|_\__,_|\_/\_/_/ \_\_|\_|
********************************************************************/
                                     
///////////////////////////////////////////////////////////////////
// LORAWAN OR EXTENDED DEVICE ADDRESS FOR LORAWAN CLOUD
#if defined LORAWAN || defined EXTDEVADDR
///////////////////////////////////////////////////////////////////
//ENTER HERE your Device Address from the TTN device info (same order, i.e. msb). Example for 0x12345678
//unsigned char DevAddr[4] = { 0x12, 0x34, 0x56, 0x78 };
///////////////////////////////////////////////////////////////////

#if defined WITH_WATERMARK && not defined WM_AS_PRIMARY_SENSOR
//Watermark soil sensor device has a different address from the default address 26011DAA
//26011DB1
//if you need another address for tensiometer sensor device, use B1, B2, B3,..., BF
unsigned char DevAddr[4] = {0x26, 0x01, 0x1D, 0xB1};
#else
//default device address for WaziGate configuration, mainly for SEN0308 capacitive soil sensor device
//26011DAA
//if you need another address for capacitive sensor device, use AA, AB, AC,..., AF
unsigned char DevAddr[4] = {0x26, 0x01, 0x1D, 0xAA};
#endif
#else
///////////////////////////////////////////////////////////////////
// DO NOT CHANGE HERE
unsigned char DevAddr[4] = { 0x00, 0x00, 0x00, node_addr };
///////////////////////////////////////////////////////////////////
#endif

///////////////////////////////////////////////////////////////////

/********************************************************************
   ___  _        _ 
  / _ \| |___ __| |
 | (_) | / -_) _` |
  \___/|_\___\__,_|
********************************************************************/
                     
//uncomment if you have an OLED attached
//verify how you want to connect the OLED
//OLED is not recommended for operational deployment

//#define OLED
//various predefined connection setups for OLED
// GND, VCC, SCL, SDA
//Suitable for WaziSense v2
//#define OLED_GND5A5A4 //5 as VCC
//for other boards – pin 4 is not available when LoRa RST is on pin 4
//#define OLED_GND235 //suitable even with 1 capacitive or 1 tensiometer+temp sensor attached
//#define OLED_9GND876 //9 as GND

/********************************************************************
  ___                              _         
 / __| ___ _ _  ___ ___ _ _   _ __(_)_ _  ___
 \__ \/ -_) ' \(_-</ _ \ '_| | '_ \ | ' \(_-<
 |___/\___|_||_/__/\___/_|   | .__/_|_||_/__/
                             |_|             
********************************************************************/

//RESERVED PINS on Arduino ProMini: 10, 11, 12, 13, 4 (for LoRa RST)

//RESERVED PINS on WaziSense: 10, 11, 12, 13, 9 (for LoRa RST), 8 (LED1), 6 & 7 (power pins), A0 (bat level), 
// WaziSense pinout
// sensor power
// | + - | + - | A2 A1 G | D4 D3 G | A5 A4 D5 G |
// Capacitive wiring (x)
// | + - | + - | A2 A1 G | D4 D3 G | A5 A4 D5 G |
// |     |     | x  x  x |       x |            |
//
// 1 tensiometer wiring (x) + soil temp DS18B20 wiring (o)
// | + - | + - | A2 A1 G | D4 D3 G | A5 A4 D5 G |
// |     | o o |    x    |    x    |    x  o    |
//
// 1st tensiometer wiring (x) + 2nd tensiometer wiring (*) + soil temp DS18B20 wiring (o)
// | + - | + - | A2 A1 G | D4 D3 G | A5 A4 D5 G |
// |     | o o | *  x    | *  x    | *  x  o    |
//

#ifdef WAZISENSE
  //this is how you need to connect the analog soil humidity sensors
  #define SH1_ANALOG_PIN A2
  #define SH1_PWR_PIN A1
  //this is how you need to connect the DS18B20 soil temperature sensor
  //the analog soil humidity sensor and the DS18B20 shares the same pwr line
  #define TEMP_DIGITAL_PIN 5
  #define TEMP_PWR_PIN 6 //one of the sensor power pin
#elif defined IRD_PCB
  //this is how you need to connect the analog soil humidity sensor
  #define SH1_ANALOG_PIN A0
  #define SH1_PWR_PIN A1
  //this is how you need to connect the second analog soil humidity sensor
  #define SH2_ANALOG_PIN A3
  #define SH2_PWR_PIN A1  
  //this is how you need to connect the DS18B20 soil temperature sensor
  //the analog soil humidity sensor and the DS18B20 shares the same pwr line
  #define TEMP_DIGITAL_PIN 6
  #define TEMP_PWR_PIN A1
#else
  //this is how you need to connect the analog soil humidity sensor
  #define SH1_ANALOG_PIN A0
  #define SH1_PWR_PIN A1
  //this is how you need to connect the DS18B20 soil temperature sensor
  //the analog soil humidity sensor and the DS18B20 shares the same pwr line
  #define TEMP_DIGITAL_PIN 7
  #define TEMP_PWR_PIN A1
#endif

#ifdef WITH_WATERMARK
#ifdef WAZISENSE
  //first Watermark
  #define WM1_PWR_PIN1 A4 //SDA
  #define WM1_PWR_PIN2 3
  #define WM1_ANALOG_PIN A1
  //second Watermark
  #define WM2_PWR_PIN1 A5 //SCL 
  #define WM2_PWR_PIN2 4 
  #define WM2_ANALOG_PIN A2
#elif defined IRD_PCB
  //first Watermark
  #define WM1_PWR_PIN1 8
  #define WM1_PWR_PIN2 9
  #define WM1_ANALOG_PIN A2
  //second Watermark
  #define WM2_PWR_PIN1 7
  #define WM2_PWR_PIN2 9
  #define WM2_ANALOG_PIN A2
#else
  //first Watermark
  #define WM1_PWR_PIN1 8
  #define WM1_PWR_PIN2 9
  #define WM1_ANALOG_PIN A2
  //second Watermark
  #define WM2_PWR_PIN1 5
  #define WM2_PWR_PIN2 6
  #define WM2_ANALOG_PIN A3
#endif
#endif


/********************************************************************
  ___            _           _           
 |_ _|_ __   ___| |_   _  __| | ___  ___ 
  | || '_ \ / __| | | | |/ _` |/ _ \/ __|
  | || | | | (__| | |_| | (_| |  __/\__ \
 |___|_| |_|\___|_|\__,_|\__,_|\___||___/
********************************************************************/                                         

#ifdef WITH_SPI_COMMANDS
#include <SPI.h>
//this is the standard behaviour of library, use SPI Transaction switching
#define USE_SPI_TRANSACTION
#endif

#ifdef SX126X
#include <SX126XLT.h>                                          
#include "SX126X_RadioSettings.h"
#endif

#ifdef SX127X
#include <SX127XLT.h>                                          
#include "SX127X_RadioSettings.h"
#endif

#ifdef SX128X
#include <SX128XLT.h>                                          
#include "SX128X_RadioSettings.h"
#endif       

#include "TempInternal.h"

// Include sensors
#include "Sensor.h"
#ifdef SI7021_SENSOR
#include <Wire.h>
#include "i2c_SI7021.h"
#include "si7021_Temperature.h"
#include "si7021_Humidity.h"
#endif

#ifdef WITH_WATERMARK
#include "watermark.h"
#else
//#include "rawAnalog.h"
#include "sen0308.h"
#endif

#ifdef SOIL_TEMP_SENSOR
#include "DS18B20.h"
#endif

#ifdef CO2_SCD30_SENSOR
#include "CO2_SCD30.h"
#endif

#ifdef USE_XLPP
#include <xlpp.h>
#endif

#ifdef USE_LPP
#include <CayenneLPP.h>
#endif


///////////////////////////////////////////////////////////////////
// ENCRYPTION CONFIGURATION AND KEYS FOR LORAWAN
#if defined LORAWAN && defined CUSTOM_LORAWAN
#ifndef WITH_AES
#define WITH_AES
#endif
#endif
#ifdef WITH_AES
#include "local_lorawan.h"
#endif

#if defined LORAWAN && defined NATIVE_LORAWAN
#ifdef WITH_AT_COMMANDS
#include "native_at_lorawan.h"
#endif
#endif

// SENSORS DEFINITION 
//////////////////////////////////////////////////////////////////
// NORMALLY YOU DO NOT NEED TO CHANGE THIS SECTION

#ifdef SI7021_SENSOR
SI7021 si7021;
bool foundSI7021=false;
uint8_t si7021_temp_index;
uint8_t si7021_hum_index;
//capacitive|watermark; 2nd watermark; soil temperature; SI7021 temp+hum; 
const int max_number_of_sensors = 5;
#else
#ifdef IRD_PCB
// capacitive or watermark; 2nd watermark; soil temperature; EC5 decagon; CO2 sensor 
const int max_number_of_sensors = 5;
#else
// capacitive or watermark; 2nd watermark; soil temperature; 
const int max_number_of_sensors = 3;
#endif
#endif

uint8_t number_of_sensors=max_number_of_sensors;

uint8_t capacitive_sensor_index;
uint8_t wm1_sensor_index;
uint8_t wm2_sensor_index;
uint8_t soil_temp_sensor_index;
#define SOIL_TEMP_UNDEFINED_VALUE -99.0
double soil_temp_sensor_value=SOIL_TEMP_UNDEFINED_VALUE;

//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// IF YOU SEND A LONG STRING, INCREASE THE SIZE OF MESSAGE
#define MLENGTH 100
uint8_t message[MLENGTH];
///////////////////////////////////////////////////////////////////

//create a library class instance called LT
//to handle LoRa radio communications

#ifdef SX126X
SX126XLT LT;
#endif

#ifdef SX127X
SX127XLT LT;
#endif

#ifdef SX128X
SX128XLT LT;
#endif

#ifdef USE_XLPP 
XLPP lpp(120);
#endif

#ifdef USE_LPP 
CayenneLPP lpp(120);
#endif

//keep track of the number of successful transmissions
uint32_t TXPacketCount=0;

///////////////////////////////////////////////////////////////////
// COMMENT THIS LINE IF YOU WANT TO DYNAMICALLY SET THE NODE'S ADDR 
// OR SOME OTHER PARAMETERS BY REMOTE RADIO COMMANDS (WITH_RCVW)
// LEAVE THIS LINE UNCOMMENTED IF YOU WANT TO USE THE DEFAULT VALUE
// AND CONFIGURE YOUR DEVICE BY CHANGING MANUALLY THESE VALUES IN 
// THE SKETCH.
//
// ONCE YOU HAVE FLASHED A BOARD WITHOUT FORCE_DEFAULT_VALUE, YOU 
// WILL BE ABLE TO DYNAMICALLY CONFIGURE IT AND SAVE THIS CONFIGU-
// RATION INTO EEPROM. ON RESET, THE BOARD WILL USE THE SAVED CON-
// FIGURATION.

// IF YOU WANT TO REINITIALIZE A BOARD, YOU HAVE TO FIRST FLASH IT 
// WITH FORCE_DEFAULT_VALUE, WAIT FOR ABOUT 10s SO THAT IT CAN BOOT
// AND FLASH IT AGAIN WITHOUT FORCE_DEFAULT_VALUE. THE BOARD WILL 
// THEN USE THE DEFAULT CONFIGURATION UNTIL NEXT CONFIGURATION.

#define FORCE_DEFAULT_VALUE
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// LOW VOLTAGE MODE
//
// low voltage mode is applied when the battary voltage falls below VCC_LOW
// once detected, the device will keep MAX_LOW_VOLTAGE_INDICATION=3 normal operation cycle
// then, it will increase the measure & transmission time interval to LOW_VOLTAGE_IDLE_PERIOD_HOUR=4 hours
// the mechanism prevents the ATMega328P microcontroller to reboot constantly
// the battery voltage is transmitted to the gateway and appears on the dashboard so that
// end-user can be warned of low voltage on the deployed device

#ifdef MONITOR_BAT_VOLTAGE
// https://github.com/Yveaux/arduino_vcc
#include <Vcc.h>                     
//Set to 1.0 for calibrate, be sure to transmit when not powered by USB      
//then get reported Vcc for specific hardware    
//const float VccCorrection = 3.3/3.3;
//finally set VccCorrection to measured Vcc by multimeter divided by reported Vcc
//Measure on real INTEL-IRRIS soil devices
const float VccCorrection = 3.0/2.9;   
//other measures on real INTEL-IRRIS soil devices
//const float VccCorrection = 3.64/3.54; //with 3.6 lithium battery
//const float VccCorrection = 3.24/3.18; //with 2 AA alkaline batteries  
Vcc vcc(VccCorrection);

//to manually test low bat
//DOES support reboot but DOES NOT HAVE normal operation mode 
//#define VCC_LOW               3.6 

//to configure for test low bat
//in test mode, idlePeriodInMin is set to 1min, so transmission time interval in low voltage mode
//will be increased to 4mins so that debugging will not take to long
//#define TEST_LOW_BAT

//how many times we keep normal measure & transmission in low voltage mode
#define MAX_LOW_VOLTAGE_INDICATION 3

//the new measure & transmission time interval for low voltage mode
#define LOW_VOLTAGE_IDLE_PERIOD_HOUR 4

#ifndef VCC_LOW
#ifdef WITH_WATERMARK
//the ATMega328P reboots at about 2.65 - 2.75
#define VCC_LOW                 2.85
#else
//capacitive sensors can be impacted by low voltage, especially for very dry soil conditions
#define VCC_LOW                 2.85
#endif
#endif
uint8_t low_voltage_indication = 0;
uint8_t low_bat_counter = 0;             
float last_vcc = 0.0;
float current_vcc = 0.0;
uint16_t v_bat_tx = 0; // updated during TX, in mV
float tx_vcc = 0.0; // float in V
#endif // MONITOR_BAT_VOLTAGE

#include "TempInternal.h"

// IRD PCB with solar panel
//
#if defined IRD_PCB && defined SOLAR_BAT
#define PANEL_AUTO  0
#define PANEL_ON    1

#ifdef NIMH
#define BATT_GOOD         4350   // 4.35 V = 3x1.45 NiMH. (4.2 enough ?)
#define BATT_TEMP_MINI    -150   // no freeze protection needed for NiMH 
#define BATT_HYST          100   // start charge if pv >= v_bat + 0.1 V
#define BAT_OK            4050   // 4.05 V = 3x1.35 V
#define BAT_LOW           3600   // 3.6 V   0% NiMH en decharge sans charge solaire
#else // Lithium
#define BATT_GOOD         4050   // 4.05 V lithium 80%
#define BATT_TEMP_MINI       5   // do not charge lipo when freeze
#define BATT_HYST          100   // start charge if pv >= v_bat + 0.1 V
#define BAT_OK            3600   // 3.6 V
#define BAT_LOW           3400   // 3.4 V   0% lithium
#define BAT_FULL          4200   // 4.2 V 100% lithium
#endif
#define BAT_MINIMUM       3000   // 3.0 V pendant la radio

//#define TIME_C3             10  // wait 10 ms C3  33 nF
#define TIME_C3              1  // wait  1 ms for C3 330 pF
// to debug : replacing the PV by a lab power supply with a resistor in serial (capacitor inside)

#define STATE_MOSFETS_OFF 0
#define STATE_MOSFETS_ON  1

uint16_t v_pv = 0;
uint16_t v_bat = 0;
uint16_t last_v_bat = 0; // will also be updated during TX, instead of updating v_bat_tx
uint16_t recovery_charging = 0;
#endif // SOLAR_BAT

/*****************************
 _____           _      
/  __ \         | |     
| /  \/ ___   __| | ___ 
| |    / _ \ / _` |/ _ \
| \__/\ (_) | (_| |  __/
 \____/\___/ \__,_|\___|
*****************************/                        
                        
// we wrapped Serial.println to support the Arduino Zero or M0
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0
#define PRINTLN                   SerialUSB.println("")              
#define PRINT_CSTSTR(param)       SerialUSB.print(F(param))
#define PRINTLN_CSTSTR(param)     SerialUSB.println(F(param))
#define PRINT_STR(fmt,param)      SerialUSB.print(param)
#define PRINTLN_STR(fmt,param)    SerialUSB.println(param)
#define PRINT_VALUE(fmt,param)    SerialUSB.print(param)
#define PRINTLN_VALUE(fmt,param)  SerialUSB.println(param)
#define PRINT_HEX(fmt,param)      SerialUSB.print(param,HEX)
#define PRINTLN_HEX(fmt,param)    SerialUSB.println(param,HEX)
#define FLUSHOUTPUT               SerialUSB.flush()
#else
#define PRINTLN                   Serial.println("")
#define PRINT_CSTSTR(param)       Serial.print(F(param))
#define PRINTLN_CSTSTR(param)     Serial.println(F(param))
#define PRINT_STR(fmt,param)      Serial.print(param)
#define PRINTLN_STR(fmt,param)    Serial.println(param)
#define PRINT_VALUE(fmt,param)    Serial.print(param)
#define PRINTLN_VALUE(fmt,param)  Serial.println(param)
#define PRINT_HEX(fmt,param)      Serial.print(param,HEX)
#define PRINTLN_HEX(fmt,param)    Serial.println(param,HEX)
#define FLUSHOUTPUT               Serial.flush()
#endif

#ifdef WITH_EEPROM
#include <EEPROM.h>
#endif

#ifdef WITH_ACK
#define NB_RETRIES 2
#endif

#ifdef LOW_POWER
// this is for the Teensy36, Teensy35, Teensy31/32 & TeensyLC
// need v6 of Snooze library
#if defined __MK20DX256__ || defined __MKL26Z64__ || defined __MK64FX512__ || defined __MK66FX1M0__
  #define LOW_POWER_PERIOD 60
  #include <Snooze.h>
  SnoozeTimer timer;
  SnoozeBlock sleep_config(timer);
#elif defined ARDUINO_ESP8266_ESP01 || defined ARDUINO_ESP8266_NODEMCU || defined ESP8266
  #define LOW_POWER_PERIOD 60
  //we will use the deepSleep feature, so no additional library
#elif defined ARDUINO_ARCH_ASR650X
  #define LOW_POWER_PERIOD 60
  static TimerEvent_t wakeUp;
#else // for all other boards based on ATMega168, ATMega328P, ATMega32U4, ATMega2560, ATMega256RFR2, ATSAMD21G18A
  #define LOW_POWER_PERIOD 8
  // you need the LowPower library from RocketScream
  // https://github.com/rocketscream/Low-Power
  #include "LowPower.h"
  
  #ifdef __SAMD21G18A__
  // use the RTC library
  #include "RTCZero.h"
  /* Create an rtc object */
  RTCZero rtc;
  #endif
#endif
#endif

#ifdef OLED
#include <U8x8lib.h>
//you can also power the OLED screen with a digital pin, here pin 8
#define OLED_PWR_PIN 8
// connection may depend on the board. Use A5/A4 for most Arduino boards. On ESP8266-based board we use GPI05 and GPI04. Heltec ESP32 has embedded OLED.
#if defined ARDUINO_Heltec_WIFI_LoRa_32 || defined ARDUINO_WIFI_LoRa_32 || defined HELTEC_LORA
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
#elif defined ESP8266 || defined ARDUINO_ESP8266_ESP01
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 12, /* data=*/ 14, /* reset=*/ U8X8_PIN_NONE);
#else
#ifdef OLED_GND235
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 2
  #endif
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 3, /* data=*/ 5, /* reset=*/ U8X8_PIN_NONE);
#elif defined OLED_9GND876
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 8
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 7, /* data=*/ 6, /* reset=*/ U8X8_PIN_NONE);
#elif defined OLED_GND5A5A4
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 5
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE);      
#else
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE);
#endif
#endif
char oled_msg[20];
#endif

unsigned long nextTransmissionTime=0L;

// array containing sensors pointers
Sensor* sensor_ptrs[max_number_of_sensors];

#ifdef WITH_EEPROM
struct nodeConfig {

  uint8_t flag1;
  uint8_t flag2;
  uint8_t seq;
#ifndef LORAWAN  
  uint8_t addr;
#endif  
  unsigned int idle_period;
  uint8_t low_voltage_indication;  
  uint8_t overwrite;
  // can add other fields such as LoRa mode,...
};

nodeConfig my_nodeConfig;
#endif

#ifdef WITH_RCVW

// will wait for 1s before opening the rcv window
#define DELAY_BEFORE_RCVW 1000

//this function is provided to parse the downlink command which is assumed to be in the format /@A6#
//
long getCmdValue(int &i, char* cmdstr, char* strBuff=NULL) {
  
     char seqStr[10]="******";
    
    int j=0;
    // character '#' will indicate end of cmd value
    while ((char)cmdstr[i]!='#' && ( i < strlen(cmdstr) && j<strlen(seqStr))) {
            seqStr[j]=(char)cmdstr[i];
            i++;
            j++;
    }
    
    // put the null character at the end
    seqStr[j]='\0';
    
    if (strBuff) {
            strcpy(strBuff, seqStr);        
    }
    else
            return (atol(seqStr));
}   
#endif

#ifndef STRING_LIB
char *ftoa(char *a, double f, int precision)
{
 long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
 
 char *ret = a;
 long heiltal = (long)f;
 itoa(heiltal, a, 10);
 while (*a != '\0') a++;
 *a++ = '.';
 long desimal = abs((long)((f - heiltal) * p[precision]));
 if (desimal < p[precision-1]) {
  *a++ = '0';
 } 
 itoa(desimal, a, 10);
 return ret;
}
#endif

#ifdef LOW_POWER

void lowPower(unsigned long time_ms) {

  unsigned long waiting_t=time_ms;

  FLUSHOUTPUT;
  delay(5);

#ifdef __SAMD21G18A__

  // For Arduino M0 or Zero we use the built-in RTC
  rtc.setTime(17, 0, 0);
  rtc.setDate(1, 1, 2000);
  if (waiting_t<60000)
    rtc.setAlarmTime(17, 0, waiting_t/1000);
  else {
    uint8_t minutes=waiting_t/60000;
    uint8_t secondes=waiting_t/1000-minutes*60;
    rtc.setAlarmTime(17, minutes, secondes);
  }
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  //rtc.attachInterrupt(alarmMatch);
  rtc.standbyMode();
  
  LowPower.standby();

  PRINT_CSTSTR("SAMD21G18A wakes up from standby\n");      
  FLUSHOUTPUT;
  
#else

#if defined __MK20DX256__ || defined __MKL26Z64__ || defined __MK64FX512__ || defined __MK66FX1M0__
  // warning, setTimer accepts value from 1ms to 65535ms max
  // milliseconds
  // by default, LOW_POWER_PERIOD is 60s for those microcontrollers
  timer.setTimer(LOW_POWER_PERIOD*1000);
#endif
              
  while (waiting_t>0) {  
  
#if defined ARDUINO_AVR_MEGA2560 || defined ARDUINO_AVR_PRO || defined ARDUINO_AVR_NANO || defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_MINI || defined __AVR_ATmega32U4__    
      // ATmega2560, ATmega328P, ATmega168, ATmega32U4
      // each wake-up introduces an overhead of about 158ms
      if (waiting_t > 8158) {
        LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
        waiting_t = waiting_t - 8158;
#ifdef SHOW_LOW_POWER_CYCLE                  
              PRINT_CSTSTR("8");
#endif              
      }
      else if (waiting_t > 4158) {
        LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
        waiting_t = waiting_t - 4158;
#ifdef SHOW_LOW_POWER_CYCLE                  
              PRINT_CSTSTR("4");
#endif 
      }
      else if (waiting_t > 2158) {
        LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF); 
        waiting_t = waiting_t - 2158;
#ifdef SHOW_LOW_POWER_CYCLE                  
              PRINT_CSTSTR("2");
#endif 
      }
      else if (waiting_t > 1158) {
        LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); 
        waiting_t = waiting_t - 1158;
#ifdef SHOW_LOW_POWER_CYCLE                  
              PRINT_CSTSTR("1");
#endif 
      }      
    else {
        delay(waiting_t); 
#ifdef SHOW_LOW_POWER_CYCLE                   
              PRINT_CSTSTR("D[");
              PRINT_VALUE("%d", waiting_t);
              PRINT_CSTSTR("]\n");
#endif
        waiting_t = 0;
      }

#if defined IRD_PCB && defined SOLAR_BAT
  manage_battery(PANEL_AUTO);
#endif
  
#ifdef SHOW_LOW_POWER_CYCLE
      FLUSHOUTPUT;
      delay(1);
#endif
      
#elif defined __MK20DX256__ || defined __MKL26Z64__ || defined __MK64FX512__ || defined __MK66FX1M0__
      // Teensy31/32 & TeensyLC
      if (waiting_t < LOW_POWER_PERIOD*1000) {
        timer.setTimer(waiting_t);
        waiting_t = 0;
      }
      else
        waiting_t = waiting_t - LOW_POWER_PERIOD*1000;
                    
#ifdef LOW_POWER_HIBERNATE
      Snooze.hibernate(sleep_config);
#else            
      Snooze.deepSleep(sleep_config);
#endif
#elif defined ARDUINO_ESP8266_ESP01 || defined ARDUINO_ESP8266_NODEMCU || defined ESP8266
      //in microseconds
      //it is reported that RST pin should be connected to pin 16 to actually reset the board when deepsleep
      //timer is triggered
      if (waiting_t < LOW_POWER_PERIOD*1000) {
        ESP.deepSleep(waiting_t*1000*1000);
        waiting_t = 0;
      }
      else {
        ESP.deepSleep(LOW_POWER_PERIOD*1000*1000);
        waiting_t = waiting_t - LOW_POWER_PERIOD*1000;          
      } 
#else
      // use the delay function
      delay(waiting_t);
      waiting_t = 0;
#endif          
   }
#endif   
}
#endif

/*****************************
 _____      _               
/  ___|    | |              
\ `--.  ___| |_ _   _ _ __  
 `--. \/ _ \ __| | | | '_ \ 
/\__/ /  __/ |_| |_| | |_) |
\____/ \___|\__|\__,_| .__/ 
                     | |    
                     |_|    
******************************/

void setup() {

#if defined IRD_PCB && defined SOLAR_BAT
  manage_battery(PANEL_AUTO);
  if (v_bat < BAT_LOW)
  {
    recovery_charging = 1;
  }
#endif

#ifdef LOW_POWER
  bool low_power_status = IS_LOWPOWER;  
#ifdef __SAMD21G18A__
  rtc.begin();
#endif  
#else
  bool low_power_status = IS_NOT_LOWPOWER;
  //digitalWrite(PIN_POWER,HIGH);
#endif

#ifdef WAZISENSE
#define WAZISENSE_BUILTIN_LED1 8
#define WAZISENSE_MOSFET1 6
#define WAZISENSE_MOSFET2 7

  pinMode(WAZISENSE_BUILTIN_LED1, OUTPUT);
  digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
  delay(200);
  digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
  delay(200);
  digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
  delay(200);
  digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);    
  
  pinMode(WAZISENSE_MOSFET1, OUTPUT);
  pinMode(WAZISENSE_MOSFET2, OUTPUT);
  digitalWrite(WAZISENSE_MOSFET1, LOW);
  digitalWrite(WAZISENSE_MOSFET2, LOW);
#endif

#ifdef WITH_AT_COMMANDS
  pinMode(BOARD_BUILTIN_LED, OUTPUT);
#endif
  
  delay(1000);
  // Open serial communications and wait for port to open:
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0 
  SerialUSB.begin(38400);
#else
  Serial.begin(38400);  
#endif

#if defined IRD_PCB && defined SOLAR_BAT
  manage_battery(PANEL_AUTO);
#endif

#ifdef OLED_PWR_PIN
  pinMode(OLED_PWR_PIN, OUTPUT);
  digitalWrite(OLED_PWR_PIN, HIGH);
#ifdef OLED_9GND876
  //use pin 9 as ground
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
#endif
#endif

  uint8_t sensor_index=0;
  
  //////////////////////////////////////////////////////////////////
// ADD YOUR SENSORS HERE   
// Sensor(nomenclature, is_analog, is_connected, is_low_power, pin_read, pin_power, pin_trigger=-1)
  //WM1 or SH1
#ifdef WITH_WATERMARK
  sensor_ptrs[sensor_index] = new watermark("WM1", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) WM1_ANALOG_PIN, (uint8_t) WM1_PWR_PIN1, (uint8_t) WM1_PWR_PIN2 /*use pin trigger as second power pin*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  wm1_sensor_index=sensor_index;
  sensor_index++;
#ifdef TWO_WATERMARK
  //WM2
  sensor_ptrs[sensor_index] = new watermark("WM2", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) WM2_ANALOG_PIN, (uint8_t) WM2_PWR_PIN1, (uint8_t) WM2_PWR_PIN2 /*use pin trigger as second power pin*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  wm2_sensor_index=sensor_index;
  sensor_index++;
#endif
#else
  sensor_ptrs[sensor_index] = new sen0308("SH1", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) SH1_ANALOG_PIN, (uint8_t) SH1_PWR_PIN /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  sensor_ptrs[sensor_index]->set_warmup_time(200);
  capacitive_sensor_index=sensor_index;
  sensor_index++;
#endif
#ifdef SOIL_EC5_SENSOR
  // SH2 // IRD_PCB
  sensor_ptrs[sensor_index] = new rawAnalog("SH2", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) SH2_ANALOG_PIN, (uint8_t) SH2_PWR_PIN /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  sensor_ptrs[sensor_index]->set_warmup_time(200);
  sensor_index++;
#endif  
#ifdef SOIL_TEMP_SENSOR
  //ST
  sensor_ptrs[sensor_index] = new DS18B20("ST", IS_NOT_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) TEMP_DIGITAL_PIN, (uint8_t) TEMP_PWR_PIN /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
#ifdef WAZISENSE  
  //it is because the soil temp is attached to a mosfet sensor pin
  sensor_ptrs[sensor_index]->set_warmup_time(1500);
#endif
  soil_temp_sensor_index=sensor_index;
  sensor_index++;
#endif
#ifdef CO2_SCD30_SENSOR
  //CO2  // IRD_PCB
  sensor_ptrs[sensor_index] = new CO2_SCD30((char*)"CO2", IS_NOT_ANALOG, IS_CONNECTED, low_power_status, -1, (uint8_t) TEMP_PWR_PIN /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample( 1);
  sensor_index++;
#endif
  
#ifdef SI7021_SENSOR
  if (si7021.initialize()) {
    PRINTLN_CSTSTR("SI7021 Sensor found");
    foundSI7021=true;
  } else {
    PRINTLN_CSTSTR("SI7021 Sensor missing");
  }    
  sensor_ptrs[sensor_index] = new si7021_Temperature((char*)"SIT", IS_NOT_ANALOG, foundSI7021, IS_NOT_LOWPOWER, -1, -1 /*no pin trigger*/);
  si7021_temp_index=sensor_index;
  sensor_index++;
  sensor_ptrs[sensor_index] = new si7021_Humidity((char*)"SIH", IS_NOT_ANALOG, foundSI7021, IS_NOT_LOWPOWER, -1, -1 /*no pin trigger*/);
  si7021_hum_index=sensor_index;
  sensor_index++;
#endif

  //we ajust to get the real number of sensors
  number_of_sensors=sensor_index;
////////////////////////////////////////////////////////////////// 
  
#ifdef OLED
  digitalWrite(OLED_PWR_PIN, HIGH);
  delay(200);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  //u8x8.setFont(u8x8_font_pxplustandynewtv_r);
  u8x8.drawString(0, 0, "PRIMA IntelIrriS");
#ifdef WAZISENSE
  u8x8.drawString(0, 1, "with WaziSense  ");
#else
  u8x8.drawString(0, 1, "with DIY IoT    ");
#endif
  u8x8.drawString(0, 2, "SoilHumidity N/A");

  delay(2000);
#ifdef LOW_POWER  
  digitalWrite(OLED_PWR_PIN, LOW);  
#endif  
#endif  
  
  // Print a start message
  PRINT_CSTSTR(BOOT_START_MSG);

#ifdef WAZISENSE
  PRINT_CSTSTR("WaziSense board\n");  
#endif

#ifdef ARDUINO_AVR_PRO
  PRINT_CSTSTR("Arduino Pro Mini detected\n");  
#endif
#ifdef ARDUINO_AVR_NANO
  PRINT_CSTSTR("Arduino Nano detected\n");   
#endif
#ifdef ARDUINO_AVR_MINI
  PRINT_CSTSTR("Arduino MINI/Nexus detected\n");  
#endif
#ifdef ARDUINO_AVR_MEGA2560
  PRINT_CSTSTR("Arduino Mega2560 detected\n");  
#endif
#ifdef ARDUINO_SAM_DUE
  PRINT_CSTSTR("Arduino Due detected\n");  
#endif
#ifdef __MK66FX1M0__
  PRINT_CSTSTR("Teensy36 MK66FX1M0 detected\n");
#endif
#ifdef __MK64FX512__
  PRINT_CSTSTR("Teensy35 MK64FX512 detected\n");
#endif
#ifdef __MK20DX256__
  PRINT_CSTSTR("Teensy31/32 MK20DX256 detected\n");
#endif
#ifdef __MKL26Z64__
  PRINT_CSTSTR("TeensyLC MKL26Z64 detected\n");
#endif
#if defined ARDUINO_SAMD_ZERO && not defined ARDUINO_SAMD_FEATHER_M0
  PRINT_CSTSTR("Arduino M0/Zero detected\n");
#endif
#ifdef ARDUINO_AVR_FEATHER32U4 
  PRINT_CSTSTR("Adafruit Feather32U4 detected\n"); 
#endif
#ifdef  ARDUINO_SAMD_FEATHER_M0
  PRINT_CSTSTR("Adafruit FeatherM0 detected\n");
#endif

// See http://www.nongnu.org/avr-libc/user-manual/using_tools.html
// for the list of define from the AVR compiler

#ifdef __AVR_ATmega328P__
  PRINT_CSTSTR("ATmega328P detected\n");
#endif 
#ifdef __AVR_ATmega32U4__
  PRINT_CSTSTR("ATmega32U4 detected\n");
#endif 
#ifdef __AVR_ATmega2560__
  PRINT_CSTSTR("ATmega2560 detected\n");
#endif 
#ifdef __SAMD21G18A__ 
  PRINT_CSTSTR("SAMD21G18A ARM Cortex-M0+ detected\n");
#endif
#ifdef __SAM3X8E__ 
  PRINT_CSTSTR("SAM3X8E ARM Cortex-M3 detected\n");
#endif

#if defined WITH_SPI_COMMANDS
  //start SPI bus communication
  SPI.begin();
#endif
  
  //setup hardware pins used by device, then check if device is found
#ifdef SX126X
  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, SW, LORA_DEVICE))
#endif

#ifdef SX127X
  if (LT.begin(NSS, NRESET, DIO0, DIO1, DIO2, LORA_DEVICE))
#endif

#ifdef SX128X
  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, DIO2, DIO3, RX_EN, TX_EN, LORA_DEVICE))
#endif

#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS 
  if (lorawan_module_setup(LORAWAN_MODULE_BAUD_RATE))
#endif  
  {
    PRINT_CSTSTR("LoRa Device found\n");                                  
    delay(500);
  }
  else
  {
    PRINT_CSTSTR("No device responding\n");
    while (1)
      ;
  }

#if defined RAW_LORA && defined WITH_SPI_COMMANDS
/*******************************************************************************************************
  Based from SX12XX example - Stuart Robinson 
*******************************************************************************************************/

  //The function call list below shows the complete setup for the LoRa device using the
  // information defined in the Settings.h file.
  //The 'Setup LoRa device' list below can be replaced with a single function call;
  //LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, Optimisation);

  //***************************************************************************************************
  //Setup LoRa device
  //***************************************************************************************************
  //got to standby mode to configure device
  LT.setMode(MODE_STDBY_RC);
#ifdef SX126X
  LT.setRegulatorMode(USE_DCDC);
  LT.setPaConfig(0x04, PAAUTO, LORA_DEVICE);
  LT.setDIO3AsTCXOCtrl(TCXO_CTRL_3_3V);
  LT.calibrateDevice(ALLDevices);                //is required after setting TCXO
  LT.calibrateImage(DEFAULT_CHANNEL);
  LT.setDIO2AsRfSwitchCtrl();
#endif
#ifdef SX128X
  LT.setRegulatorMode(USE_LDO);
#endif
  //set for LoRa transmissions                              
  LT.setPacketType(PACKET_TYPE_LORA);
  //set the operating frequency                 
#ifdef MY_FREQUENCY
  LT.setRfFrequency(MY_FREQUENCY, Offset);
#else  
  LT.setRfFrequency(DEFAULT_CHANNEL, Offset);                   
#endif
//run calibration after setting frequency
#ifdef SX127X
  LT.calibrateImage(0);
#endif
  //set LoRa modem parameters
#if defined SX126X || defined SX127X
  LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate, Optimisation);
#endif
#ifdef SX128X
  LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate);
#endif                                     
  //where in the SX buffer packets start, TX and RX
  LT.setBufferBaseAddress(0x00, 0x00);
  //set packet parameters
#if defined SX126X || defined SX127X                     
  LT.setPacketParams(8, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL);
#endif
#ifdef SX128X
  LT.setPacketParams(12, LORA_PACKET_VARIABLE_LENGTH, 255, LORA_CRC_ON, LORA_IQ_NORMAL, 0, 0);
#endif
  //syncword, LORA_MAC_PRIVATE_SYNCWORD = 0x12, or LORA_MAC_PUBLIC_SYNCWORD = 0x34 
#if defined SX126X || defined SX127X
#if defined PUBLIC_SYNCWORD || defined LORAWAN
  LT.setSyncWord(LORA_MAC_PUBLIC_SYNCWORD);              
#else
  LT.setSyncWord(LORA_MAC_PRIVATE_SYNCWORD);
#endif
  //set for highest sensitivity at expense of slightly higher LNA current
  LT.setHighSensitivity();  //set for maximum gain
#endif
#ifdef SX126X
  //set for IRQ on TX done and timeout on DIO1
  LT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_TX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);
#endif
#ifdef SX127X
  //set for IRQ on RX done
  LT.setDioIrqParams(IRQ_RADIO_ALL, IRQ_TX_DONE, 0, 0);
  LT.setPA_BOOST(PA_BOOST);
#endif
#ifdef SX128X
  LT.setDioIrqParams(IRQ_RADIO_ALL, (IRQ_TX_DONE + IRQ_RX_TX_TIMEOUT), 0, 0);
#endif   

  if (IQ_Setting==LORA_IQ_INVERTED) {
    LT.invertIQ(true);
    PRINT_CSTSTR("Invert I/Q on RX\n");
  }
  else {
    LT.invertIQ(false);
    PRINT_CSTSTR("Normal I/Q\n");
  }  
   
  //***************************************************************************************************

  PRINTLN;
  //reads and prints the configured LoRa settings, useful check
  LT.printModemSettings();                               
  PRINTLN;
  //reads and prints the configured operating settings, useful check
  LT.printOperatingSettings();                           
  PRINTLN;
  PRINTLN;
#if defined SX126X || defined SX127X  
  //print contents of device registers, normally 0x00 to 0x4F
  LT.printRegisters(0x00, 0x4F);
#endif                       
#ifdef SX128X
  //print contents of device registers, normally 0x900 to 0x9FF 
  LT.printRegisters(0x900, 0x9FF);
#endif                         

/*******************************************************************************************************
  End from SX12XX example - Stuart Robinson 
*******************************************************************************************************/
#endif

#ifdef WITH_EEPROM
#if defined ARDUINO_ESP8266_ESP01 || defined ARDUINO_ESP8266_NODEMCU || defined ARDUINO_ARCH_ASR650X
  EEPROM.begin(512);
#endif
  // get config from EEPROM
  EEPROM.get(0, my_nodeConfig);

  // found a valid config?
  if (my_nodeConfig.flag1==0x12 && my_nodeConfig.flag2==0x35) {
    PRINT_CSTSTR("Get back previous sx1272 config\n");
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
    // TODO with LoRaWAN frame counter
    // Currently, RAK3172 cannot be set with an arbitrary frame counter
    // it is set to 0 after a power up 
#else     
    // set sequence number for SX1272 library
    LT.setTXSeqNo(my_nodeConfig.seq);
    PRINT_CSTSTR("Using packet sequence number of ");
    PRINT_VALUE("%d", LT.readTXSeqNo());
    PRINTLN;
#endif
    //when low voltage is detected, the device will still measure and transmit to indicate the low voltage
    //it will do so MAX_LOW_VOLTAGE_INDICATION times and this will be indicated in the low_voltage_indication
    //variable saved in EEPROM
    low_voltage_indication=my_nodeConfig.low_voltage_indication;     

#ifdef FORCE_DEFAULT_VALUE
    PRINT_CSTSTR("Forced to use default parameters\n");
    my_nodeConfig.flag1=0x12;
    my_nodeConfig.flag2=0x35;
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
    my_nodeConfig.seq=0; 
#else       
    my_nodeConfig.seq=LT.readTXSeqNo(); 
#endif
#ifndef LORAWAN    
    my_nodeConfig.addr=node_addr;
#endif    
    my_nodeConfig.idle_period=idlePeriodInMin;
    my_nodeConfig.low_voltage_indication=low_voltage_indication;    
    my_nodeConfig.overwrite=0;
    EEPROM.put(0, my_nodeConfig);
#else
#ifndef LORAWAN
    // get back the node_addr
    if (my_nodeConfig.addr!=0 && my_nodeConfig.overwrite==1) {
      
        PRINT_CSTSTR("Used stored address\n");
        node_addr=my_nodeConfig.addr;        
    }
    else
        PRINT_CSTSTR("Stored node addr is null\n"); 
#endif
    // get back the idle period
    if (my_nodeConfig.idle_period!=0 && my_nodeConfig.overwrite==1) {
      
        PRINT_CSTSTR("Used stored idle period\n");
        idlePeriodInMin=my_nodeConfig.idle_period;        
    }
    else
        PRINT_CSTSTR("Stored idle period is null\n");                 
#endif  

#if defined WITH_AES && not defined EXTDEVADDR && not defined LORAWAN
    DevAddr[3] = (unsigned char)node_addr;
#endif

#ifndef LORAWAN
    PRINT_CSTSTR("Using node addr of ");
    PRINT_VALUE("%d", node_addr);
    PRINTLN;   
#endif

    PRINT_CSTSTR("Using idle period of ");
    PRINT_VALUE("%d", idlePeriodInMin);
    PRINTLN;     
  }
  else {
    // otherwise, write config and start over
    my_nodeConfig.flag1=0x12;
    my_nodeConfig.flag2=0x35;
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
    my_nodeConfig.seq=0; 
#else      
    my_nodeConfig.seq=LT.readTXSeqNo();
#endif    
#ifndef LORAWAN     
    my_nodeConfig.addr=node_addr;
#endif    
    my_nodeConfig.idle_period=idlePeriodInMin;
    my_nodeConfig.low_voltage_indication=0;
    my_nodeConfig.overwrite=0;
  }
#endif

#if defined RAW_LORA
  PRINT_CSTSTR("Setting Power: ");
  PRINT_VALUE("%d", MAX_DBM);
  PRINTLN;
#endif  

#ifndef LORAWAN
  LT.setDevAddr(node_addr);
  PRINT_CSTSTR("node addr: ");
  PRINT_VALUE("%d", node_addr);
  PRINTLN;
#endif  

#ifdef SX126X
  PRINT_CSTSTR("SX126X");
#endif
#ifdef SX127X
  PRINT_CSTSTR("SX127X");
#endif
#ifdef SX128X
  PRINT_CSTSTR("SX128X");
#endif
#ifdef RAK3172
  PRINT_CSTSTR("RAK3172");
#endif
  
  // Print a success message
  PRINT_CSTSTR(" successfully configured\n");

#ifdef MONITOR_BAT_VOLTAGE

#ifdef TEST_LOW_BAT
  idlePeriodInMin=1;
#endif

  // check batterie first time
  current_vcc = (double)((uint16_t)(vcc.Read_Volts()*100))/100.00;
  //initialized last_vcc on boot
  last_vcc = current_vcc;
  PRINT_CSTSTR("Battery voltage on startup is ");
  PRINTLN_VALUE("%f", current_vcc);

  //TODO: 0.15 guard value has not been really tested
  if (low_voltage_indication && current_vcc > VCC_LOW+0.15) {
#ifdef WITH_EEPROM
    // reset low_voltage_indication
    low_voltage_indication=0;
    my_nodeConfig.low_voltage_indication=0;
    EEPROM.put(0, my_nodeConfig);
#endif 
  }
   
  PRINT_CSTSTR("low_voltage_indication=");
  PRINTLN_VALUE("%d", low_voltage_indication);  
#endif

#if defined IRD_PCB && defined SOLAR_BAT
  manage_battery(PANEL_AUTO);
#endif

#ifdef WITH_AES
  local_lorawan_init();
#endif  
}

#if defined IRD_PCB && defined SOLAR_BAT
// read micro controler temperature
#define TEMP_INTERNAL  
#define SOLAR_PANEL_ANA   A7  // analog input
#define SOLAR_PANEL_PIN   5   // mosfet command Q4

//////////////////////////////////////////////////////////////
//
uint16_t solar_analogRead( void)
{
  uint16_t v;
  
  v = analogRead( SOLAR_PANEL_ANA);
  v = (uint16_t) ((uint32_t) v * 3300 / 1023); // 10 bits
  v = (uint16_t) ((uint32_t) v * 5300 / 1000); // R5 430k R4 100k /5.3 15.3 V maxi

  return v;
}

uint16_t internal_read_volt( void)
{
  uint16_t v;
  // v = ((uint16_t)(vcc.Read_Volts()*100))/100.0;
  v = (uint16_t) (vcc.Read_Volts()*1000);
  return v;
}


void manage_battery( uint8_t force_on)
{
  static uint8_t v_state = STATE_MOSFETS_OFF;
#ifdef TEMP_INTERNAL
  int8_t tempInternal;
  
  tempInternal = GetTempInternal();
#endif

  switch( v_state)
  {
    case STATE_MOSFETS_OFF:
    default:
      v_pv = solar_analogRead();

      // connect battery for measure : pannel can consume 0.5 mA at night
      pinMode( SOLAR_PANEL_PIN, OUTPUT);
      digitalWrite( SOLAR_PANEL_PIN, HIGH);      
      delay( TIME_C3);  
      v_bat = solar_analogRead();

#ifdef TEMP_INTERNAL
      if ((v_pv > (v_bat + BATT_HYST)  &&  v_bat < BATT_GOOD  &&  tempInternal > BATT_TEMP_MINI)  ||  force_on)
#else
      if ((v_pv > (v_bat + BATT_HYST)  &&  v_bat < BATT_GOOD)  ||  force_on)
#endif
      {
        // sun or daylight OR radio
        v_state = STATE_MOSFETS_ON;
        break;
      }

      pinMode( SOLAR_PANEL_PIN, OUTPUT);
      digitalWrite( SOLAR_PANEL_PIN, LOW);
      break;

    case STATE_MOSFETS_ON:
      v_bat = solar_analogRead();
      if (v_bat > BATT_GOOD  &&  !force_on)
      {
        // battery stop charge
        pinMode( SOLAR_PANEL_PIN, OUTPUT);
        digitalWrite( SOLAR_PANEL_PIN, LOW);
        v_state = STATE_MOSFETS_OFF;
        break;
      }

      // disconnect battery for solar measure
      pinMode( SOLAR_PANEL_PIN, OUTPUT);
      digitalWrite( SOLAR_PANEL_PIN, LOW);      
      delay( TIME_C3);
      v_pv = solar_analogRead();
      if (v_pv <= v_bat)
      {
        // night
        v_state = STATE_MOSFETS_OFF;
        break;
      }

      pinMode( SOLAR_PANEL_PIN, OUTPUT);
      digitalWrite( SOLAR_PANEL_PIN, HIGH);
      break;
  }

#ifdef SHOW_LOW_POWER_CYCLE                   
  PRINT_CSTSTR("PV=");
  PRINT_VALUE("%d", v_pv);
  PRINT_CSTSTR("bat=");
  PRINT_VALUE("%d", v_bat);
  PRINT_CSTSTR("r=");
  PRINT_VALUE("%d", last_v_bat);
#ifdef TEMP_INTERNAL
  PRINT_CSTSTR("T=");
  PRINT_VALUE("%d", tempInternal);
#endif
  PRINT_CSTSTR(" ");
  PRINT_VALUE("%d", v_state);
  PRINT_CSTSTR("\n");
#endif
}

#ifndef NIMH
// lipo cell battery level
// return percent (can be more than 100% if voltage is too hi)
//
uint8_t bat_level( uint16_t vbat)
{
  if (vbat <= BAT_LOW)
  {
    return 0;
  }

  return (uint8_t) ((vbat - BAT_LOW) / ((BAT_FULL - BAT_LOW) / 100));
}
#endif
#endif // SOLAR_BAT

//////////////////////////////////////////////////////////////
// called by loop
//
void measure_and_send( void)
{
  long startSend;
  long endSend;
  uint8_t app_key_offset=0;

#if defined WITH_APPKEY && not defined LORAWAN
      app_key_offset = sizeof(my_appKey);
      // set the app key in the payload
      memcpy(message,my_appKey,app_key_offset);
#endif

#ifdef OLED
      //at startup and only every 20 transmitted packets
#ifdef LOW_POWER      
      if (TXPacketCount == 0 || (TXPacketCount % 20) == 19) {
#endif        
        digitalWrite(OLED_PWR_PIN, HIGH);
        u8x8.begin();
        u8x8.setFont(u8x8_font_chroma48medium8_r);
        //u8x8.setFont(u8x8_font_pxplustandynewtv_r);
        u8x8.drawString(0, 0, "PRIMA IntelIrriS");
#ifdef WAZISENSE
        u8x8.drawString(0, 1, "with WaziSense  ");
#else
        u8x8.drawString(0, 1, "with DIY IoT    ");
#endif
#ifdef LOW_POWER
      }
#endif      
#endif

      uint8_t r_size;

#if defined USE_XLPP || defined USE_LPP
      // Create lpp payload.
      lpp.reset();
      uint8_t ch=0;
#endif

      char final_str[80] = "\\!";
      
      //this is how we can wake up some sensors in advance in case they need a longer warmup time
      //digitalWrite(sensor_ptrs[4]->get_pin_power(),HIGH);

#ifdef SI7021_SENSOR
      //here we chose to handle separately the SI7021
      //as it is an integrated sensors for both temperature and humidity
      float humidity, temperature;
      si7021.getHumidity(humidity);
      si7021.getTemperature(temperature);
      si7021.triggerMeasurement();
      //we just inject the data into the corresponding sensor object
      sensor_ptrs[si7021_temp_index]->set_data((double)temperature);
      sensor_ptrs[si7021_hum_index]->set_data((double)humidity);            
#endif     

      for (int i=0; i<number_of_sensors; i++) {
        //there might be specific pre-init operations for some sensors
        sensor_ptrs[i]->pre_init();   
      }

#if defined WITH_WATERMARK && defined SOIL_TEMP_SENSOR
      // we get the soil temperature in advance so that we can use it later for the watermark
      soil_temp_sensor_value=sensor_ptrs[soil_temp_sensor_index]->get_value();
#endif
      
      // main loop for sensors, actually, you don't have to edit anything here
      // just add a predefined sensor if needed or provide a new sensor class instance for a handle a new physical sensor
      for (int i=0; i<number_of_sensors; i++) {

          if (sensor_ptrs[i]->get_is_connected() || sensor_ptrs[i]->has_fake_data()) {

#ifdef STRING_LIB
              if (i==0) {
                  sprintf(final_str, "%s%s/%s", final_str, sensor_ptrs[i]->get_nomenclature(), String(sensor_ptrs[i]->get_value()).c_str());
              } 
              else {
                  sprintf(final_str, "%s/%s/%s", final_str, sensor_ptrs[i]->get_nomenclature(), String(sensor_ptrs[i]->get_value()).c_str());
              }
#else
              char float_str[10];
              double tmp_value;
              
#if defined WITH_WATERMARK && defined SOIL_TEMP_SENSOR
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"ST",2)==0)
                //because we already got it previously, so we just avoid reading it again
                tmp_value=soil_temp_sensor_value;                
              else
#endif          
                //get the value from the sensor    
                tmp_value=sensor_ptrs[i]->get_value();

              //TEST
              //tmp_value += 0.25;

#if not defined WITH_WATERMARK && defined SEN0308_TRANSMIT_MILLIVOLT
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"SH1",3)==0) {
                tmp_value=(double)(((sen0308*)sensor_ptrs[i])->convert_to_millivolt(tmp_value, 1023));
              }  
#endif
                          
              ftoa(float_str, tmp_value, 2);
          
              if (i==0) {
                  sprintf(final_str, "%s%s/%s", final_str, sensor_ptrs[i]->get_nomenclature(), float_str);
              } 
              else {
                  sprintf(final_str, "%s/%s/%s", final_str, sensor_ptrs[i]->get_nomenclature(), float_str);
              }

#ifdef WITH_WATERMARK
              //the first Watermark
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"WM1",3)==0) {
#ifdef LINK_SOIL_TEMP_TO_CENTIBAR                
                if (soil_temp_sensor_value!=SOIL_TEMP_UNDEFINED_VALUE)
                  ftoa(float_str, sensor_ptrs[i]->convert_value(tmp_value, soil_temp_sensor_value, -1.0), 2);
                else  
#endif                
                  //taking 28°C as the default soil temperature
                  ftoa(float_str, sensor_ptrs[i]->convert_value(tmp_value, WM_REF_TEMPERATURE, -1.0), 2);
                sprintf(final_str, "%s/CB1/%s", final_str, float_str);
              }  
#ifdef TWO_WATERMARK
              //the second Watermark
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"WM2",3)==0) {
#ifdef LINK_SOIL_TEMP_TO_CENTIBAR                
                if (soil_temp_sensor_value!=SOIL_TEMP_UNDEFINED_VALUE)
                  ftoa(float_str, sensor_ptrs[i]->convert_value(tmp_value, soil_temp_sensor_value, -1.0), 2);
                else  
#endif                
                  //taking 28°C as the default soil temperature
                  ftoa(float_str, sensor_ptrs[i]->convert_value(tmp_value, WM_REF_TEMPERATURE, -1.0), 2);                
                sprintf(final_str, "%s/CB2/%s", final_str, float_str);
              }  
#endif              
#endif              

#if defined USE_XLPP || defined USE_LPP
              //TODO
              //there is an issue with current XLPP so we use addTemperature() for all values, except for voltage
#ifdef TEST_DEVICE_RANDOM              
              randomSeed(analogRead(2));
#ifdef WITH_WATERMARK
              //test 1
              //tmp_value=55+random(50,200);
              //test 2
              tmp_value=0.0+random(130,330);
#else
              //test 1
              //tmp_value=212.5+random(1,5);
              //test 2
              tmp_value=0.0+random(250,550);
#endif              
#endif
 
#ifdef WITH_WATERMARK
              //the first Watermark
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"WM1",3)==0) {
                //tmp_value=110.0; // for testing, i.e. 1100omhs
#ifdef LINK_SOIL_TEMP_TO_CENTIBAR                
                if (soil_temp_sensor_value!=SOIL_TEMP_UNDEFINED_VALUE)
                  lpp.addTemperature(ch, sensor_ptrs[i]->convert_value(tmp_value, soil_temp_sensor_value, -1.0));
                else
#endif                  
                  // here we convert to centibar, using a mean temperature of 28°C
                  lpp.addTemperature(ch, sensor_ptrs[i]->convert_value(tmp_value, WM_REF_TEMPERATURE, -1.0));
                ch++;
                                 
                //Note: for watermark, raw data is scaled by dividing by 10 because addAnalogInput() will not accept
                //large values while resistance value for watermark can go well beyond 3000
                //TODO when wazigate LPP decoding bug is fixed, we could use un-scaled value                                    
                lpp.addTemperature(ch, tmp_value);
                ch++;
              }
#ifdef TWO_WATERMARK
              //the second Watermark
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"WM2",3)==0) {
                //tmp_value=110.0; // for testing, i.e. 1100omhs
#ifdef LINK_SOIL_TEMP_TO_CENTIBAR                
                if (soil_temp_sensor_value!=SOIL_TEMP_UNDEFINED_VALUE)
                  lpp.addTemperature(ch, sensor_ptrs[i]->convert_value(tmp_value, soil_temp_sensor_value, -1.0));
                else
#endif                
                  // here we convert to centibar, using a mean temperature of 28°C
                  lpp.addTemperature(ch, sensor_ptrs[i]->convert_value(tmp_value, WM_REF_TEMPERATURE, -1.0));
                ch++;
                
                //Note: for watermark, raw data is scaled by dividing by 10 because addAnalogInput() will not accept
                //large values while resistance value for watermark can go well beyond 3000
                //TODO when wazigate LPP decoding bug is fixed, we could use un-scaled value                                    
                lpp.addTemperature(ch, tmp_value);
                ch++;
              }
#endif   

#ifdef SOIL_TEMP_SENSOR
              //the soil temperature sensor
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"ST",2)==0) {
                //we always use channel 5 for soil temperature
                lpp.addTemperature(5, tmp_value);
              }
#endif

#ifdef CO2_SCD30_SENSOR
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"CO2",3)==0) {
                //we always use channel 8 for co2
                lpp.addTemperature(8, tmp_value);
              }
#endif


#else

#ifdef SOIL_TEMP_SENSOR
              //the soil temperature sensor
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"ST",2)==0) {
                //we always use channel 5 for soil temperature
                ch=5;
              }
#endif

#ifdef CO2_SCD30_SENSOR
              if (strncmp(sensor_ptrs[i]->get_nomenclature(),"CO2",3)==0) {
                //we always use channel 8 for co2
                lpp.addTemperature(8, tmp_value);
              }
#endif
              //the soil capacitive sensor
              //tmp_value=250.0; // for testing
#if defined MONITOR_BAT_VOLTAGE && defined SEN0308_CALIBRATION_LOW_VOLTAGE
              tmp_value=sensor_ptrs[i]->convert_value(tmp_value, (double)last_vcc, 0.0);
#endif
              lpp.addTemperature(ch, tmp_value);    
#endif
#endif
              
#ifdef OLED
#ifdef LOW_POWER
              if (TXPacketCount == 0 || (TXPacketCount % 20) == 19) {
#endif                
                // don't show the '\!' characters
                sprintf(oled_msg, "%s/%s", sensor_ptrs[i]->get_nomenclature(), float_str); 
                u8x8.drawString(0, 2+i, oled_msg);
#ifdef LOW_POWER                
              }  
#endif
#endif   
#endif              
          }
      }

#if defined IRD_PCB && defined SOLAR_BAT
      manage_battery( PANEL_AUTO);
#endif

      for (int i=0; i<number_of_sensors; i++) {
        //there might be specific post-init operations for some sensors
        sensor_ptrs[i]->post_init();   
      }

#ifdef MONITOR_BAT_VOLTAGE
#if defined USE_XLPP || defined USE_LPP
//here we transmit the last measured vcc, measured from last transmission cycle
#if defined TRANSMIT_VOLTAGE && defined ALWAYS_TRANSMIT_VOLTAGE
  #if (defined IRD_PCB && defined SOLAR_BAT)
      // if SOLAR_BAT, the min battery voltage is transmitted
      PRINT_CSTSTR("Adding battery voltage (IRD_PCBA: ");
      if (last_v_bat==0)
        last_v_bat=v_bat;
      PRINTLN_VALUE("%f", (float) last_v_bat / 1000.0);
      lpp.addAnalogInput(6, (float) last_v_bat / 1000.0);
      #ifdef NIMH
      //lpp.addAnalogInput(7, (float) v_bat / 1000.0);   // volt
      #else
      //lpp.addAnalogInput(7, (float) v_bat / 1000.0);   // volt pour test
      //lpp.addAnalogInput(7, (float) bat_level(v_bat)); // percent for lithium
      #endif
  #else     
      //here we transmit the voltage measured right after TX, not the voltage measured during TX
      lpp.addAnalogInput(6, last_vcc);
      PRINTLN_VALUE("%f", last_vcc);
  #endif      
#elif defined TRANSMIT_VOLTAGE
  #if (defined IRD_PCB && defined SOLAR_BAT)
      if (last_v_bat==0)
        last_v_bat=v_bat;
      if ((float) last_v_bat / 1000.0 < VCC_LOW) {
        lpp.addAnalogInput(6, (float) last_v_bat / 1000.0);
      }
  #else
      if (last_vcc < VCC_LOW) {
        lpp.addAnalogInput(6, last_vcc);
      }
  #endif    
#endif      
#endif
#endif

#ifdef OLED
#ifdef LOW_POWER
      if (TXPacketCount == 0 || (TXPacketCount % 20) == 19) {
        delay(2000);
        digitalWrite(OLED_PWR_PIN, LOW);
        //need to wait long enough otherwise there will be not enough power for transmission
        delay(1000);
      }
#endif      
#endif
      
      r_size=sprintf((char*)message+app_key_offset, final_str);

      PRINT_CSTSTR("Sending ");
      PRINT_STR("%s",(char*)(message+app_key_offset));
      PRINTLN;

      PRINT_CSTSTR("Real payload size is ");
      PRINTLN_VALUE("%d", r_size);

#if defined USE_XLPP || defined USE_LPP
      PRINT_CSTSTR("use LPP format for transmission to gateway\n");
#else
#if defined RAW_LORA && defined WITH_SPI_COMMANDS
      LT.printASCIIPacket(message, r_size);
      PRINTLN;
#endif      
#endif      
      int pl=r_size+app_key_offset;

#ifdef RAW_LORA
      uint8_t p_type=PKT_TYPE_DATA;
#if defined WITH_AES
      // indicate that payload is encrypted
      p_type = p_type | PKT_FLAG_DATA_ENCRYPTED;
#endif
#ifdef WITH_APPKEY
      // indicate that we have an appkey
      p_type = p_type | PKT_FLAG_DATA_WAPPKEY;
#endif     
#endif

/**********************************  
  ___   _____ _____ 
 / _ \ |  ___/  ___|
/ /_\ \| |__ \ `--. 
|  _  ||  __| `--. \
| | | || |___/\__/ /
\_| |_/\____/\____/ 
***********************************/
//use AES (LoRaWAN-like) encrypting
///////////////////////////////////
#ifdef WITH_AES
#ifdef LORAWAN
      PRINT_CSTSTR("end-device uses native LoRaWAN packet format\n");
#if defined USE_XLPP
      pl=local_aes_lorawan_create_pkt(lpp.buf, lpp.len, app_key_offset);
#elif defined USE_LPP 
      pl=local_aes_lorawan_create_pkt(lpp.getBuffer(), lpp.getSize(), app_key_offset);      
#else
      pl=local_aes_lorawan_create_pkt(message, pl, app_key_offset);      
#endif        
#else
      PRINT_CSTSTR("end-device uses encapsulated LoRaWAN packet format only for encryption\n");
      pl=local_aes_lorawan_create_pkt(message, pl, app_key_offset);      
#endif
#endif
///////////////////////////////////

#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
      PRINT_CSTSTR("plain payload hex\n");
#if defined USE_XLPP
      dumpHEXtoStr(serial_buff, lpp.buf, lpp.len);
#elif defined USE_LPP 
      dumpHEXtoStr(serial_buff, lpp.getBuffer(), lpp.getSize());     
#else
      dumpHEXtoStr(serial_buff, (char*)message, r_size);
#endif
      PRINTLN_STR("%s", serial_buff);
#endif

#if defined IRD_PCB && defined SOLAR_BAT
      manage_battery( PANEL_ON); // use solar to reduce battery current
      last_v_bat = v_bat;        // reset minimum battery value
#endif

      startSend=millis();

#if defined RAW_LORA && defined WITH_SPI_COMMANDS
      //LT.CarrierSense();
#endif      
      
#if defined RAW_LORA && defined WITH_ACK
      p_type=PKT_TYPE_DATA | PKT_FLAG_ACK_REQ;
      PRINTLN_CSTSTR("%s","Will request an ACK");         
#endif

#ifdef WAZISENSE
      digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
      delay(50);
      digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
      delay(50);
      digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
      delay(50);
      digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
#endif

#ifdef CUSTOM_LORAWAN
      //will return packet length sent if OK, otherwise 0 if transmit error
      //we use raw format for LoRaWAN
#if defined USE_XLPP
#if defined IRD_PCB && defined SOLAR_BAT
      if (LT.transmit(lpp.buf, pl, 10000, MAX_DBM, WAIT_TX, &solar_analogRead, &last_v_bat))
#elif defined MONITOR_BAT_VOLTAGE
      if (LT.transmit(lpp.buf, pl, 10000, MAX_DBM, WAIT_TX, &internal_read_volt, &v_bat_tx))
#else
      if (LT.transmit(lpp.buf, pl, 10000, MAX_DBM, WAIT_TX))
#endif      
#elif defined USE_LPP 
#if defined IRD_PCB && defined SOLAR_BAT
      if (LT.transmit(lpp.getBuffer(), pl, 10000, MAX_DBM, WAIT_TX, &solar_analogRead, &last_v_bat))
#elif defined MONITOR_BAT_VOLTAGE
      if (LT.transmit(lpp.getBuffer(), pl, 10000, MAX_DBM, WAIT_TX, &internal_read_volt, &v_bat_tx))
#else
      if (LT.transmit(lpp.getBuffer(), pl, 10000, MAX_DBM, WAIT_TX))
#endif      
#else
#if defined IRD_PCB && defined SOLAR_BAT
      if (LT.transmit(message, pl, 10000, MAX_DBM, WAIT_TX, &solar_analogRead, &last_v_bat))
#elif defined MONITOR_BAT_VOLTAGE
      if (LT.transmit(message, pl, 10000, MAX_DBM, WAIT_TX, &internal_read_volt, &v_bat_tx))
#else      
      if (LT.transmit(message, pl, 10000, MAX_DBM, WAIT_TX))
#endif      
#endif
#elif defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
      if (lorawan_transmit(serial_buff))        
#else
      //will return packet length sent if OK, otherwise 0 if transmit error
      if (LT.transmitAddressed(message, pl, p_type, DEFAULT_DEST_ADDR, node_addr, 10000, MAX_DBM, WAIT_TX))  
#endif
      {
#ifdef MONITOR_BAT_VOLTAGE
        //not 100% reliable because the board can reboot right after transmission because of
        //low voltage, so last_vcc = vcc.Read_Volts(); is not executed
        last_vcc = (double)((uint16_t)(vcc.Read_Volts()*100))/100.0;     
#endif                
        endSend = millis();                                                  
        TXPacketCount++;

#ifdef WAZISENSE
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
        delay(500);
        digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
#endif

#if defined RAW_LORA && defined WITH_SPI_COMMANDS        
        uint16_t localCRC = LT.CRCCCITT(message, pl, 0xFFFF);
        PRINT_CSTSTR("CRC,");
        PRINT_HEX("%d", localCRC);      
        
        if (LT.readAckStatus()) {
          PRINTLN;
          PRINT_CSTSTR("Received ACK from ");
          PRINTLN_VALUE("%d", LT.readRXSource());
          PRINT_CSTSTR("SNR of transmitted pkt is ");
          PRINTLN_VALUE("%d", LT.readPacketSNRinACK());          
        }
#endif
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
        //nothing particular right now
#endif                  
      }
      else
      {
#ifdef MONITOR_BAT_VOLTAGE
        //not 100% reliable because the board can reboot right after transmission because of
        //low voltage, so last_vcc = vcc.Read_Volts(); is not executed
        last_vcc = (double)((uint16_t)(vcc.Read_Volts()*100))/100.0;     
#endif        
        endSend=millis();
        
#ifdef WAZISENSE
        //error
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, HIGH);
        delay(200);
        digitalWrite(WAZISENSE_BUILTIN_LED1, LOW);
#endif

#if defined RAW_LORA && defined WITH_SPI_COMMANDS               
        //if here there was an error transmitting packet
        uint16_t IRQStatus;
        IRQStatus = LT.readIrqStatus();
        PRINT_CSTSTR("SendError,");
        PRINT_CSTSTR(",IRQreg,");
        PRINT_HEX("%d", IRQStatus);
        LT.printIrqStatus();
#endif
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
        //nothing particular right now   
#endif      

#if defined IRD_PCB && defined SOLAR_BAT
      manage_battery( PANEL_ON); // last_v_bat is updated in LT.transmit if modified in library
#endif        
      }

///////////////////////////////////////////////////////////////////
// DOWNLINK BLOCK - EDIT ONLY NEW COMMAND SECTION
// 
///////////////////////////////////////////////////////////////////

#ifdef WITH_RCVW

uint8_t RXPacketL=0;

#ifndef NATIVE_LORAWAN 
#ifdef LORAWAN 
      uint8_t rxw_max=2;
#else
      uint8_t rxw_max=1;
#endif

#ifdef INVERTIQ_ON_RX
      // Invert I/Q
      PRINTLN_CSTSTR("Inverting I/Q for RX");
      LT.invertIQ(true);
#endif
      uint8_t rxw=1;
                   
      do {
          PRINT_CSTSTR("Wait for ");
          PRINTLN_VALUE("%d", (endSend+rxw*DELAY_BEFORE_RCVW) - millis());
          
          PRINT_CSTSTR("Wait for incoming packet-RX");
          PRINTLN_VALUE("%d", rxw);

          //target 1s which is RX1 for LoRaWAN in most regions
          //then target 1s more which is RX2 for LoRaWAN in most regions
          while (millis()-endSend < rxw*DELAY_BEFORE_RCVW)
            ;
            
          // wait for incoming packets
          RXPacketL = LT.receive(message, sizeof(message), 850, WAIT_RX);
          
          //we received something in RX1
          if (RXPacketL && rxw==1)
            rxw=rxw_max+1;
          else
            // try RX2 only if we are in LoRaWAN mode and nothing has been received in RX1
            if (++rxw<=rxw_max) {
#ifdef EU868
              //change freq to 869.525 as we are targeting RX2 window
              PRINT_CSTSTR("Set downlink frequency to 869.525MHz\n");
              LT.setRfFrequency(869525000, Offset);
#elif defined AU915
              //change freq to 923.3 as we are targeting RX2 window
              PRINT_CSTSTR("Set downlink frequency to 923.3MHz\n");
              LT.setRfFrequency(923300000, Offset);
#elif defined EU433
              //change freq to 434.665 as we are targeting RX2 window
              PRINT_CSTSTR("Set downlink frequency to 434.665MHz\n");
              LT.setRfFrequency(434655000, Offset);
#elif defined BAND2400
              //no changes in 2400 band              
#endif
              //change to SF12 as we are targeting RX2 window
              //valid for EU868 and EU433 band        
              PRINT_CSTSTR("Set to SF12\n");
#if defined SX126X || defined SX127X
              LT.setModulationParams(LORA_SF12, Bandwidth, CodeRate, Optimisation);
#endif
#ifdef SX128X
              LT.setModulationParams(LORA_SF12, Bandwidth, CodeRate);
#endif            
            }
            else {
#ifdef LORAWAN              
              //set back to the reception frequency
              PRINT_CSTSTR("Set back frequency\n");
#ifdef MY_FREQUENCY
              LT.setRfFrequency(MY_FREQUENCY, Offset);
#else  
              LT.setRfFrequency(DEFAULT_CHANNEL, Offset);                   
#endif              
              //set back the SF
              PRINT_CSTSTR("Set back SF\n");
#if defined SX126X || defined SX127X
              LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate, Optimisation);
#endif
#ifdef SX128X
              LT.setModulationParams(SpreadingFactor, Bandwidth, CodeRate);
#endif
#endif              
          }
      } while (rxw<=rxw_max);

#ifdef INVERTIQ_ON_RX
      // Invert I/Q
      PRINTLN_CSTSTR("I/Q back to normal");
      LT.invertIQ(false);
#endif      

#endif //#ifndef NATIVE_LORAWAN

      // we have received a downlink message
      //
      if (RXPacketL) {  
        int i=0;
        long cmdValue;

#if defined RAW_LORA && defined WITH_SPI_COMMANDS
#ifndef LORAWAN
        char print_buff[50];

        sprintf((char*)print_buff, "^p%d,%d,%d,%d,%d,%d,%d\n",        
                   LT.readRXDestination(),
                   LT.readRXPacketType(),                   
                   LT.readRXSource(),
                   LT.readRXSeqNo(),                   
                   RXPacketL,
                   LT.readPacketSNR(),
                   LT.readPacketRSSI());                                   
        PRINT_STR("%s",(char*)print_buff);         

        PRINT_CSTSTR("frame hex\n"); 
        
        for ( i=0 ; i<RXPacketL; i++) {               
          if (message[i]<16)
            PRINT_CSTSTR("0");
          PRINT_HEX("%X", message[i]);
          PRINT_CSTSTR(" ");       
        }
        PRINTLN;

        message[i]=(char)'\0';
        // in non-LoRaWAN, we try to print the characters
        PRINT_STR("%s",(char*)message);
        i=0;            
#else
        i=local_lorawan_decode_pkt(message, RXPacketL);
                      
        //set the null character at the end of the payload in case it is a string
        message[RXPacketL-4]=(char)'\0';       
#endif
#endif
        PRINTLN;
        FLUSHOUTPUT;
       
        // commands have following format /@A6#
        //
        if (i>=0 && message[i]=='/' && message[i+1]=='@') {

            char cmdstr[15];
            // copy the downlink payload, up to sizeof(cmdstr)
            strncpy(cmdstr,(char*)(message+i),sizeof(cmdstr)); 
                
            PRINT_CSTSTR("Parsing command\n");
            PRINT_STR("%s", cmdstr);
            PRINTLN;      
            i=2;   

            switch ((char)cmdstr[i]) {

#ifndef LORAWAN
                  // set the node's address, /@A10# to set the address to 10 for instance
                  case 'A': 

                      i++;
                      cmdValue=getCmdValue(i, cmdstr);
                      
                      // cannot set addr greater than 255
                      if (cmdValue > 255)
                              cmdValue = 255;
                      // cannot set addr lower than 2 since 0 is broadcast and 1 is for gateway
                      if (cmdValue < 2)
                              cmdValue = node_addr;
                      // set node addr        
                      node_addr=cmdValue; 
#ifdef WITH_AES 
                      DevAddr[3] = (unsigned char)node_addr;
#endif
                      LT.setDevAddr(node_addr);
                      PRINT_CSTSTR("Set LoRa node addr to ");
                      PRINT_VALUE("%d", node_addr);  
                      PRINTLN;     

#ifdef WITH_EEPROM
                      // save new node_addr in case of reboot
                      my_nodeConfig.addr=node_addr;
                      my_nodeConfig.overwrite=1;
                      EEPROM.put(0, my_nodeConfig);
#endif
                      break;        
#endif
                  // set the time between 2 transmissions, /@I10# to set to 10 minutes for instance
                  case 'I': 

                      i++;
                      cmdValue=getCmdValue(i, cmdstr);

                      // cannot set addr lower than 1 minute
                      if (cmdValue < 1)
                              cmdValue = idlePeriodInMin;
                      // idlePeriodInMin      
                      idlePeriodInMin=cmdValue; 
                      
                      PRINT_CSTSTR("Set duty-cycle to ");
                      PRINT_VALUE("%d", idlePeriodInMin);  
                      PRINTLN;         

#ifdef WITH_EEPROM
                      // save new idle_period in case of reboot
                      my_nodeConfig.idle_period=idlePeriodInMin;
                      my_nodeConfig.overwrite=1;
                      EEPROM.put(0, my_nodeConfig);
#endif

                      break;  

                  // Toggle a LED to illustrate an actuation example
                  // command syntax is /@L2# for instance
                  case 'L': 

                      i++;
                      cmdValue=getCmdValue(i, cmdstr);
                      
                      PRINT_CSTSTR("Toggle LED on pin ");
                      PRINT_VALUE("%ld", cmdValue);
                      PRINTLN;

                      // warning, there is no check on the pin number
                      // /@L2# for instance will toggle LED connected to digital pin number 2
                      pinMode(cmdValue, OUTPUT);
                      digitalWrite(cmdValue, HIGH);
                      delay(500);
                      digitalWrite(cmdValue, LOW);
                      delay(500);
                      digitalWrite(cmdValue, HIGH);
                      delay(500);
                      digitalWrite(cmdValue, LOW);
                      
                      break;
                            
                  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
                  // add here new commands
                  //  

                  //
                  /////////////////////////////////////////////////////////////////////////////////////////////////////////////

                  default:
      
                    PRINT_CSTSTR("Unrecognized cmd\n");       
                    break;
            }
        }          
      }
      else
        PRINT_CSTSTR("No downlink\n");        
#endif         

#ifdef WITH_EEPROM
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
      my_nodeConfig.seq=TXPacketCount;
#else
      // save packet number for next packet in case of reboot     
      my_nodeConfig.seq=LT.readTXSeqNo();
#endif      
      EEPROM.put(0, my_nodeConfig);
#endif
      PRINTLN;
      PRINT_CSTSTR("LoRa pkt size ");
      PRINT_VALUE("%d", pl);
      PRINTLN;

#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
      PRINT_CSTSTR("LoRa pkt seq ");   
      PRINT_VALUE("%d", TXPacketCount?0:TXPacketCount-1);
      PRINTLN; 
#else      
      PRINT_CSTSTR("LoRa pkt seq ");   
      PRINT_VALUE("%d", LT.readTXSeqNo()-1);
      PRINTLN;
#endif    
      PRINT_CSTSTR("LoRa Sent in ");
      PRINT_VALUE("%ld", endSend-startSend);
      PRINTLN;
}  

/*****************************
 _                       
| |                      
| |     ___   ___  _ __  
| |    / _ \ / _ \| '_ \ 
| |___| (_) | (_) | |_) |
\_____/\___/ \___/| .__/ 
                  | |    
                  |_|    
*****************************/

void loop(void)
{
#ifndef LOW_POWER
  // 600000+random(15,60)*1000
  if (millis() > nextTransmissionTime) {
#else
      //time for next wake up
      nextTransmissionTime=millis()+((idlePeriodInSec==0)?(unsigned long)idlePeriodInMin*60*1000:(unsigned long)idlePeriodInSec*1000);      
      //PRINTLN_VALUE("%ld",nextTransmissionTime);
      //PRINTLN_VALUE("%ld",(idlePeriodInSec==0)?(unsigned long)idlePeriodInMin*60*1000:(unsigned long)idlePeriodInSec*1000);

#if defined IRD_PCB && defined SOLAR_BAT
      if (recovery_charging  &&  v_bat > BAT_OK) {
        recovery_charging = 0;
      }

      if (!recovery_charging  &&  v_bat > BAT_LOW  &&  (last_v_bat > BAT_MINIMUM  ||  last_v_bat == 0)) {
        measure_and_send();
      }
      else {
        PRINT_CSTSTR("!LOW BATTERY ");
        PRINTLN_VALUE("%d", recovery_charging);
      }
#elif defined MONITOR_BAT_VOLTAGE        

      current_vcc = (double)((uint16_t)(vcc.Read_Volts()*100))/100.0;
      tx_vcc = (double)(v_bat_tx)/1000.0;

      PRINT_CSTSTR("BATTERY-->");
      PRINT_VALUE("%f", current_vcc); //now, right after sleep
      PRINT_CSTSTR(" | ");
      PRINTLN_VALUE("%f", last_vcc); //right after last transmit

      PRINT_CSTSTR("BATT_TX-->");
      PRINTLN_VALUE("%f", tx_vcc); //updated during last transmit


#ifdef BYPASS_LOW_BAT
      measure_and_send();
#else      
      if (current_vcc < VCC_LOW || last_vcc < VCC_LOW || tx_vcc < VCC_LOW) {
        PRINT_CSTSTR("!LOW BATTERY-->");
        PRINT_VALUE("%f", current_vcc);
        PRINT_CSTSTR(" | ");
        PRINT_VALUE("%f", last_vcc);
        PRINT_CSTSTR(" | ");
        PRINTLN_VALUE("%f", tx_vcc);
        
        PRINT_CSTSTR("low_voltage_indication=");
        PRINTLN_VALUE("%d", low_voltage_indication);
    
        if (low_voltage_indication < MAX_LOW_VOLTAGE_INDICATION) {
          //we will still measure and transmit 3 times to warn end-user as soon as possible
          //and overcome possible packet transmission losses
          low_voltage_indication++;
#ifdef WITH_EEPROM
          // save new low_voltage_indication
          my_nodeConfig.low_voltage_indication=low_voltage_indication;
          EEPROM.put(0, my_nodeConfig);
#endif          
        }
        else {

          if (low_bat_counter++==0) { 
            //increase transmission time to 4 hours when low voltage, if it is smaller than 4 hours            
            if (idlePeriodInMin < 240)
              PRINTLN_CSTSTR("Set nextTransmissionTime to 4h");             
#ifdef TEST_LOW_BAT
              //for testing, we use idlePeriodInMin=1min so new transmission time interval is 4mins
              //if the board reboots righ after transmission, then it will actually be 3mins
              //as the initial idlePeriodInMin would be missing                          
              nextTransmissionTime = millis() + (LOW_VOLTAGE_IDLE_PERIOD_HOUR - (unsigned long)idlePeriodInMin) * 60 * 1000;
#else
              //otherwise, it is increased to 4 hours
              //if the board reboots righ after transmission, then it will actually be 3h
              //as the initial idlePeriodInMin would be missing
              nextTransmissionTime = millis() + (LOW_VOLTAGE_IDLE_PERIOD_HOUR * 60 - (unsigned long)idlePeriodInMin) * 60 * 1000;
#endif
            PRINT_CSTSTR("low_bat_counter=");
            PRINTLN_VALUE("%d", low_bat_counter);   
          }       
        }
      }

      if (last_vcc > VCC_LOW || low_bat_counter==2 || low_voltage_indication <= MAX_LOW_VOLTAGE_INDICATION) {
        if (low_bat_counter==2) {
          PRINTLN_CSTSTR("Force measure and transmission");
          low_bat_counter=0;
        }

        //disable low_voltage_indication
        if (low_voltage_indication == MAX_LOW_VOLTAGE_INDICATION) {
          low_voltage_indication = MAX_LOW_VOLTAGE_INDICATION+1;
#ifdef WITH_EEPROM
          // set new low_voltage_indication
          my_nodeConfig.low_voltage_indication=low_voltage_indication;
          EEPROM.put(0, my_nodeConfig);
#endif          
        }          
        
        if (low_voltage_indication && last_vcc > VCC_LOW) {
#ifdef WITH_EEPROM
          // reset low_voltage_indication
          my_nodeConfig.low_voltage_indication=0;
          EEPROM.put(0, my_nodeConfig);
#endif                 
        }
        measure_and_send();
      }
#endif
#else
      measure_and_send();
#endif
#endif

///////////////////////////////////////////////////////////////////
// LOW-POWER BLOCK - DO NOT EDIT
// 
///////////////////////////////////////////////////////////////////

#if defined LOW_POWER && not defined ARDUINO_SAM_DUE
      PRINT_CSTSTR("Switch to power saving mode\n");

      //how much do we still have to wait, in millisec?
      unsigned long now_millis=millis();

      //PRINTLN_VALUE("%ld",now_millis);
      //PRINTLN_VALUE("%ld",nextTransmissionTime);
      
      if (millis() > nextTransmissionTime)
        nextTransmissionTime=millis()+1000;
        
      unsigned long waiting_t = nextTransmissionTime-now_millis;

      //PRINTLN_VALUE("%ld",waiting_t);
      FLUSHOUTPUT;

      // first power down the radio module
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
      // sleep until we wake the module with an AT command on the serial line
      lorawan_sleep(0 /*waiting_t*/);
#else
      //CONFIGURATION_RETENTION=RETAIN_DATA_RAM on SX128X
      //parameter is ignored on SX127X
      LT.setSleep(CONFIGURATION_RETENTION);
#endif

      // then power down the microcontroller
      lowPower(waiting_t);
      
      PRINT_CSTSTR("Wake from power saving mode\n");
#if defined NATIVE_LORAWAN && defined WITH_AT_COMMANDS
      lorawan_wake();
#else      
      LT.wake(); 
#endif           
#else
      PRINTLN;
      PRINT_CSTSTR("Will send next value at\n");
      // can use a random part also to avoid collision
      nextTransmissionTime=millis()+((idlePeriodInSec==0)?(unsigned long)idlePeriodInMin*60*1000:(unsigned long)idlePeriodInSec*1000);
      //+(unsigned long)random(15,60)*1000;
      PRINT_VALUE("%ld", nextTransmissionTime);
      PRINTLN;
  }
#endif
}
