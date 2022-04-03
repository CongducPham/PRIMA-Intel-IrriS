/*
 *  INTEL_IRRIS soil humidity sensor platform
 *  extended version with AES and custom Carrier Sense features
 *  
 *  Copyright (C) 2016-2021 Congduc Pham, University of Pau, France
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
 * last update: March 10th, 2022 by C. Pham
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

////////////////////////////////////////////////////////////////////
// sends data to INTEL-IRRIS WaziGate edge-gateway
#define TO_WAZIGATE

////////////////////////////////////////////////////////////////////
// Frequency band - do not change in SX127X_RadioSettings.h anymore
//#define BAND868
//#define BAND900
#define BAND433

////////////////////////////////////////////////////////////////////
// Test device
//#define TEST_DEVICE_RANDOM

////////////////////////////////////////////////////////////////////
// uncomment to have a soil tensiometer watermark sensor
//#define WITH_WATERMARK
////////////////////////////////////////////////////////////////////
// uncomment to force the watermark to have default device address for WaziGate
//#define WM_AS_PRIMARY_SENSOR

////////////////////////////////////////////////////////////////////
// uncomment to have 1 soil temperature sensor ST
// using a one-wire DS18B20 sensor
//#define SOIL_TEMP_SENSOR

////////////////////////////////////////////////////////////////////
// WAZISENSE and WAZIDEV v1.4 boards have
//  - an embedded SI7021 sensor
// WAZISENSE has an integrated solar panel level monitoring circuit 
//  - input voltage comming from solar panel is exposed on pin A2
// WAZIDEV has a battery voltage level monitoring circuit
//  - exposed on pin A7, and D7 must then be at LOW level
////////////////////////////////////////////////////////////////////

//choose either WAZISENSE or WAZIDEV14, or NONE of them for DIY ProMini
//#define WAZISENSE
//#define WAZIDEV14

//can be uncommented for both WAZISENSE and WAZIDEV14
//#define SI7021_SENSOR

//uncomment to use XLPP format to send to WAZIGATE for instance
//so uncomment XLPP only with LORAWAN to WAZIGATE
#ifdef TO_WAZIGATE
#define LORAWAN
#define USE_XLPP
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
//request an ack from gateway - only for non-LoRaWAN mode
//#define WITH_ACK
////////////////////////////
//if you are low on program memory, comment STRING_LIB to save about 2K
//#define STRING_LIB
////////////////////////////
#define LOW_POWER
#define LOW_POWER_HIBERNATE
//#define SHOW_LOW_POWER_CYCLE //uncomment only for debugging and testing
////////////////////////////
//Use native LoRaWAN packet format to send to LoRaWAN gateway - beware it does not mean you device is a full LoRaWAN device
#ifndef TO_WAZIGATE
//#define LORAWAN
#endif
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
////////////////////////////
//uncomment to use a customized frequency. TTN plan includes 868.1/868.3/868.5/867.1/867.3/867.5/867.7/867.9 for LoRa
//#define MY_FREQUENCY 868100000
//#define MY_FREQUENCY 433170000
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
//Watermark soil sensor device has always this address which is different from the default address
//26011DBB
unsigned char DevAddr[4] = {0x26, 0x01, 0x1D, 0xBB};
#else
//default device address for WaziGate, mainly for SEN0308 soil sensor device
//26011DAA
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
//#define OLED_GND234
//#define OLED_7GND654 //7 as GND
//#define OLED_GND13_12_11

//For WaziDev without SOLAR_PANEL_LEVEL
#define OLED_9GND876 //9 as GND
//For WaziDev with SOLAR_PANEL_LEVEL, cannot use D7
//#define OLED_A3GNDA2_A1_A0 //A3 as GND

//Suitable for WaziSense
//#define OLED_GND579

/********************************************************************
  ___                              _         
 / __| ___ _ _  ___ ___ _ _   _ __(_)_ _  ___
 \__ \/ -_) ' \(_-</ _ \ '_| | '_ \ | ' \(_-<
 |___/\___|_||_/__/\___/_|   | .__/_|_||_/__/
                             |_|             
********************************************************************/

#ifdef WAZISENSE
//uncomment to transmit data related to solar panel level 
//#define SOLAR_PANEL_LEVEL
//this is how you need to connect the analog soil humidity sensors
#define SH1_ANALOG_PIN A6
#define SH1_PWR_PIN 6
#define TEMP_DIGITAL_PIN 7
#define TEMP_PWR_PIN A7
#else
#ifdef WAZIDEV14
//uncomment to transmit data related to battery voltage level 
//#define BAT_LEVEL
#endif
//this is how you need to connect the analog soil humidity sensors
#define SH1_ANALOG_PIN A0
#define SH1_PWR_PIN A1
#define TEMP_DIGITAL_PIN 6
#define TEMP_PWR_PIN 7

#ifdef WITH_WATERMARK
#define WM_PWR_PIN1 8
#define WM_PWR_PIN2 9
#define WM_ANALOG_PIN A0
#endif

#endif


/********************************************************************
  ___            _           _           
 |_ _|_ __   ___| |_   _  __| | ___  ___ 
  | || '_ \ / __| | | | |/ _` |/ _ \/ __|
  | || | | | (__| | |_| | (_| |  __/\__ \
 |___|_| |_|\___|_|\__,_|\__,_|\___||___/
********************************************************************/                                         

#include <SPI.h>
//this is the standard behaviour of library, use SPI Transaction switching
#define USE_SPI_TRANSACTION  
//indicate in this file the radio module: SX126X, SX127X or SX128X
#include "RadioSettings.h"

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

// Include sensors
#include "Sensor.h"
#include "rawAnalog.h"
#ifdef SI7021_SENSOR
#include <Wire.h>
#include "i2c_SI7021.h"
#include "si7021_Temperature.h"
#include "si7021_Humidity.h"
#endif

#ifdef SOIL_TEMP_SENSOR
#include "DS18B20.h"
#endif

#ifdef WITH_WATERMARK
#include "watermark.h"
#endif

#ifdef USE_XLPP
#include <xlpp.h>
#endif

///////////////////////////////////////////////////////////////////
// ENCRYPTION CONFIGURATION AND KEYS FOR LORAWAN
#ifdef LORAWAN
#ifndef WITH_AES
#define WITH_AES
#endif
#endif
#ifdef WITH_AES
#include "local_lorawan.h"
#endif

// SENSORS DEFINITION 
//////////////////////////////////////////////////////////////////
// NORMALLY YOU DO NOT NEED TO CHANGE THIS SECTION
#if defined WAZISENSE || defined WAZIDEV14
  #ifdef SI7021_SENSOR
    SI7021 si7021;
    bool foundSI7021=false;
    uint8_t si7021_temp_index;
    uint8_t si7021_hum_index;
    #if defined SOLAR_PANEL_LEVEL || defined BAT_LEVEL
      //soil sensor(s) + SI7021 temp/hum + solar|battery level
      #ifdef SOIL_TEMP_SENSOR
        const int number_of_sensors = 5;
      #else
        const int number_of_sensors = 4;
      #endif
    #else
       //soil sensor(s) + SI7021 temp/hum
      #ifdef SOIL_TEMP_SENSOR
        const int number_of_sensors = 4;
      #else
        const int number_of_sensors = 3;
      #endif
    #endif
  #else
    #if defined SOLAR_PANEL_LEVEL || defined BAT_LEVEL
      //soil sensor(s) + solar|battery level
      #ifdef SOIL_TEMP_SENSOR
        const int number_of_sensors = 3;
      #else
        const int number_of_sensors = 2;
      #endif
    #else
       //soil sensor(s)
      #ifdef SOIL_TEMP_SENSOR
        const int number_of_sensors = 2;
      #else
        const int number_of_sensors = 1;
      #endif
    #endif  
  #endif
#else
  //soil sensor(s) on regular ProMini PCB version
  #ifdef SOIL_TEMP_SENSOR
    const int number_of_sensors = 2;
  #else
    const int number_of_sensors = 1;
  #endif
#endif
//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// IF YOU SEND A LONG STRING, INCREASE THE SIZE OF MESSAGE
uint8_t message[80];
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
XLPP xlpp(120);
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
#ifdef OLED_GND234
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 2
  #endif
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 3, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
#elif defined OLED_9GND876
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 8
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 7, /* data=*/ 6, /* reset=*/ U8X8_PIN_NONE);
#elif defined OLED_7GND654
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 6
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);  
#elif defined OLED_GND13_12_11
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 13
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 12, /* data=*/ 11, /* reset=*/ U8X8_PIN_NONE); 
#elif defined OLED_GND579
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN 5
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 7, /* data=*/ 9, /* reset=*/ U8X8_PIN_NONE); 
#elif defined OLED_A3GNDA2_A1_A0
  #ifdef OLED_PWR_PIN
    #undef OLED_PWR_PIN
    #define OLED_PWR_PIN A2
  #endif  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ A1, /* data=*/ A0, /* reset=*/ U8X8_PIN_NONE); 
#else
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ A5, /* data=*/ A4, /* reset=*/ U8X8_PIN_NONE);
#endif
#endif
char oled_msg[20];
#endif

unsigned long nextTransmissionTime=0L;

// array containing sensors pointers
Sensor* sensor_ptrs[number_of_sensors];

#ifdef WITH_EEPROM
struct sx1272config {

  uint8_t flag1;
  uint8_t flag2;
  uint8_t seq;
  uint8_t addr;
  unsigned int idle_period;  
  uint8_t overwrite;
  // can add other fields such as LoRa mode,...
};

sx1272config my_sx1272config;
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
  
#ifdef LOW_POWER
  bool low_power_status = IS_LOWPOWER;  
#ifdef __SAMD21G18A__
  rtc.begin();
#endif  
#else
  bool low_power_status = IS_NOT_LOWPOWER;
  //digitalWrite(PIN_POWER,HIGH);
#endif
  
  delay(1000);
  // Open serial communications and wait for port to open:
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0 
  SerialUSB.begin(38400);
#else
  Serial.begin(38400);  
#endif

#ifdef OLED_PWR_PIN
  pinMode(OLED_PWR_PIN, OUTPUT);
  digitalWrite(OLED_PWR_PIN, HIGH);
#ifdef OLED_9GND876
  //use pin 9 as ground
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
#elif defined OLED_7GND654
  //use pin 7 as ground
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
#elif defined OLED_A3GNDA2_A1_A0
  //use pin A3 as ground
  pinMode(A3, OUTPUT);
  analogWrite(A3, LOW);  
#endif
#endif

  uint8_t sensor_index=0;
  
  //////////////////////////////////////////////////////////////////
// ADD YOUR SENSORS HERE   
// Sensor(nomenclature, is_analog, is_connected, is_low_power, pin_read, pin_power, pin_trigger=-1)
  //SH1
#ifdef WITH_WATERMARK
  sensor_ptrs[sensor_index] = new watermark("WM", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) WM_ANALOG_PIN, (uint8_t) WM_PWR_PIN1, (uint8_t) WM_PWR_PIN2 /*use pin trigger as second power pin*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  sensor_index++;
#else
  sensor_ptrs[sensor_index] = new rawAnalog("SH1", IS_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) SH1_ANALOG_PIN, (uint8_t) SH1_PWR_PIN /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  sensor_ptrs[sensor_index]->set_warmup_time(200);
  sensor_index++;
#endif  
#ifdef SOIL_TEMP_SENSOR
  //ST
  sensor_ptrs[sensor_index] = new DS18B20((char*)"DS", IS_NOT_ANALOG, IS_CONNECTED, low_power_status, (uint8_t) TEMP_DIGITAL_PIN, (uint8_t) TEMP_PWR_PIN /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(NSAMPLE);
  sensor_index++;
#endif

#if defined WAZISENSE && defined SOLAR_PANEL_LEVEL  
  sensor_ptrs[sensor_index] = new rawAnalog("SPL", IS_ANALOG, IS_CONNECTED, IS_NOT_LOWPOWER, (uint8_t) A2, -1 /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(1);
  sensor_index++;  
#endif
#if defined WAZIDEV14 && defined BAT_LEVEL
  sensor_ptrs[sensor_index] = new rawAnalog("BAT", IS_ANALOG, IS_CONNECTED, IS_NOT_LOWPOWER, (uint8_t) A7, -1 /*no pin trigger*/);
  sensor_ptrs[sensor_index]->set_n_sample(1);
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
#elif defined WAZIDEV14  
  u8x8.drawString(0, 1, "with WaziDev1.4 ");
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
  PRINT_CSTSTR("LoRa soil humidity sensor, extended version\n");

#ifdef WAZISENSE
  PRINT_CSTSTR("WaziSense board\n");
#ifdef SOLAR_PANEL_LEVEL  
  //for the solar panel monitoring circuit
  pinMode(A2, INPUT);
#endif    
#endif

#ifdef WAZIDEV14
  PRINT_CSTSTR("WaziDev v1.4 board\n");
  pinMode(7, OUTPUT);
#ifdef BAT_LEVEL  
  //for the bat level monitoring circuit
  pinMode(A7, INPUT);
  //need to put D7 low to activate the monitoring circuit
  digitalWrite(7, LOW);
#else
  //keep D7 HIGH to de-activate the bat level monitoring circuit
  digitalWrite(7, HIGH);  
#endif    
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

  //start SPI bus communication
  SPI.begin();
  
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
  {
    PRINT_CSTSTR("LoRa Device found\n");                                  
    delay(1000);
  }
  else
  {
    PRINT_CSTSTR("No device responding\n");
    while (1){ }
  }

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

#ifdef WITH_EEPROM
#if defined ARDUINO_ESP8266_ESP01 || defined ARDUINO_ESP8266_NODEMCU || defined ARDUINO_ARCH_ASR650X
  EEPROM.begin(512);
#endif
  // get config from EEPROM
  EEPROM.get(0, my_sx1272config);

  // found a valid config?
  if (my_sx1272config.flag1==0x12 && my_sx1272config.flag2==0x35) {
    PRINT_CSTSTR("Get back previous sx1272 config\n");
    // set sequence number for SX1272 library
    LT.setTXSeqNo(my_sx1272config.seq);
    PRINT_CSTSTR("Using packet sequence number of ");
    PRINT_VALUE("%d", LT.readTXSeqNo());
    PRINTLN;    

#ifdef FORCE_DEFAULT_VALUE
    PRINT_CSTSTR("Forced to use default parameters\n");
    my_sx1272config.flag1=0x12;
    my_sx1272config.flag2=0x35;   
    my_sx1272config.seq=LT.readTXSeqNo(); 
    my_sx1272config.addr=node_addr;
    my_sx1272config.idle_period=idlePeriodInMin;    
    my_sx1272config.overwrite=0;
    EEPROM.put(0, my_sx1272config);
#else
    // get back the node_addr
    if (my_sx1272config.addr!=0 && my_sx1272config.overwrite==1) {
      
        PRINT_CSTSTR("Used stored address\n");
        node_addr=my_sx1272config.addr;        
    }
    else
        PRINT_CSTSTR("Stored node addr is null\n"); 

    // get back the idle period
    if (my_sx1272config.idle_period!=0 && my_sx1272config.overwrite==1) {
      
        PRINT_CSTSTR("Used stored idle period\n");
        idlePeriodInMin=my_sx1272config.idle_period;        
    }
    else
        PRINT_CSTSTR("Stored idle period is null\n");                 
#endif  

#if defined WITH_AES && not defined EXTDEVADDR && not defined LORAWAN
    DevAddr[3] = (unsigned char)node_addr;
#endif            
    PRINT_CSTSTR("Using node addr of ");
    PRINT_VALUE("%d", node_addr);
    PRINTLN;   

    PRINT_CSTSTR("Using idle period of ");
    PRINT_VALUE("%d", idlePeriodInMin);
    PRINTLN;     
  }
  else {
    // otherwise, write config and start over
    my_sx1272config.flag1=0x12;
    my_sx1272config.flag2=0x35;  
    my_sx1272config.seq=LT.readTXSeqNo(); 
    my_sx1272config.addr=node_addr;
    my_sx1272config.idle_period=idlePeriodInMin;
    my_sx1272config.overwrite=0;
  }
#endif

  PRINT_CSTSTR("Setting Power: ");
  PRINT_VALUE("%d", MAX_DBM);
  PRINTLN;

  LT.setDevAddr(node_addr);
  PRINT_CSTSTR("node addr: ");
  PRINT_VALUE("%d", node_addr);
  PRINTLN;

#ifdef SX126X
  PRINT_CSTSTR("SX126X");
#endif
#ifdef SX127X
  PRINT_CSTSTR("SX127X");
#endif
#ifdef SX128X
  PRINT_CSTSTR("SX128X");
#endif 
  
  // Print a success message
  PRINT_CSTSTR(" successfully configured\n");

  //printf_begin();
  delay(500);
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
  long startSend;
  long endSend;
  uint8_t app_key_offset=0;
  int e;
  float sensor_value;
  
#ifndef LOW_POWER
  // 600000+random(15,60)*1000
  if (millis() > nextTransmissionTime) {
#else
      //time for next wake up
      nextTransmissionTime=millis()+((idlePeriodInSec==0)?(unsigned long)idlePeriodInMin*60*1000:(unsigned long)idlePeriodInSec*1000);
      //PRINTLN_VALUE("%ld",nextTransmissionTime);
      //PRINTLN_VALUE("%ld",(idlePeriodInSec==0)?(unsigned long)idlePeriodInMin*60*1000:(unsigned long)idlePeriodInSec*1000);
#endif      
      
#if defined WITH_APPKEY && not defined LORAWAN
      app_key_offset = sizeof(my_appKey);
      // set the app key in the payload
      memcpy(message,my_appKey,app_key_offset);
#endif

#ifdef WAZISENSE
      int A2value=analogRead(A2);
      PRINT_CSTSTR("Solar panel is at level ");
      PRINTLN_VALUE("%d", A2value);

      pinMode(8, OUTPUT);
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(8, LOW);
      delay(200);
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(8, LOW);
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
#elif defined WAZIDEV14  
        u8x8.drawString(0, 1, "with WaziDev1.4 ");
#else
        u8x8.drawString(0, 1, "with DIY IoT    ");
#endif
#ifdef LOW_POWER
      }
#endif      
#endif

      uint8_t r_size;

#ifdef USE_XLPP
      // Create xlpp payload.
      xlpp.reset();
#endif

      char final_str[80] = "\\!";
      
      //this is how we can wake up some sensors in advance in case they need a longer warmup time
      //digitalWrite(sensor_ptrs[4]->get_pin_power(),HIGH);

#ifdef SI7021_SENSOR
      //here chose to handle separately the SI7021
      //as it is an integrated sensors for both temperature and humidity
      float humidity, temperature;
      si7021.getHumidity(humidity);
      si7021.getTemperature(temperature);
      si7021.triggerMeasurement();
      //we just inject the data into the corresponding sensor object
      sensor_ptrs[si7021_temp_index]->set_data((double)temperature);
      sensor_ptrs[si7021_hum_index]->set_data((double)humidity);            
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
              double tmp_value=sensor_ptrs[i]->get_value();
                          
              ftoa(float_str, tmp_value, 2);
          
              if (i==0) {
                  sprintf(final_str, "%s%s/%s", final_str, sensor_ptrs[i]->get_nomenclature(), float_str);
              } 
              else {
                  sprintf(final_str, "%s/%s/%s", final_str, sensor_ptrs[i]->get_nomenclature(), float_str);
              }

#ifdef USE_XLPP
              //TODO
              //there is an issue with current XLPP so we use addTemperature() for all values
#ifdef TEST_DEVICE_RANDOM              
              randomSeed(analogRead(2));
              //test 1
              //tmp_value=212.5+random(1,5);
              //test 2
              tmp_value=0.0+random(250,550);
#endif                          
              xlpp.addTemperature(i, tmp_value);
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

#ifdef USE_XLPP
      PRINT_CSTSTR("use XLPP format for transmission to WaziGate");
      PRINTLN;
#else
      PRINT_CSTSTR("Real payload size is ");
      PRINT_VALUE("%d", r_size);
      PRINTLN;

      LT.printASCIIPacket(message, r_size);
      PRINTLN;
#endif      
      int pl=r_size+app_key_offset;

      uint8_t p_type=PKT_TYPE_DATA;
      
#if defined WITH_AES
      // indicate that payload is encrypted
      p_type = p_type | PKT_FLAG_DATA_ENCRYPTED;
#endif

#ifdef WITH_APPKEY
      // indicate that we have an appkey
      p_type = p_type | PKT_FLAG_DATA_WAPPKEY;
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
#ifdef USE_XLPP
      pl=local_aes_lorawan_create_pkt(xlpp.buf, xlpp.len, app_key_offset);
#else
      pl=local_aes_lorawan_create_pkt(message, pl, app_key_offset);      
#endif        
#else
      PRINT_CSTSTR("end-device uses encapsulated LoRaWAN packet format only for encryption\n");
      pl=local_aes_lorawan_create_pkt(message, pl, app_key_offset);      
#endif
#endif
///////////////////////////////////

      startSend=millis();

      LT.CarrierSense();
      
#ifdef WITH_ACK
      p_type=PKT_TYPE_DATA | PKT_FLAG_ACK_REQ;
      PRINTLN_CSTSTR("%s","Will request an ACK");         
#endif

#ifdef LORAWAN
      //will return packet length sent if OK, otherwise 0 if transmit error
      //we use raw format for LoRaWAN
#ifdef USE_XLPP
      if (LT.transmit(xlpp.buf, pl, 10000, MAX_DBM, WAIT_TX))
#else
      if (LT.transmit(message, pl, 10000, MAX_DBM, WAIT_TX))
#endif       
#else
      //will return packet length sent if OK, otherwise 0 if transmit error
      if (LT.transmitAddressed(message, pl, p_type, DEFAULT_DEST_ADDR, node_addr, 10000, MAX_DBM, WAIT_TX))  
#endif
      {
        endSend = millis();                                          
        TXPacketCount++;
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
      }
      else
      {
        endSend=millis();
        //if here there was an error transmitting packet
        uint16_t IRQStatus;
        IRQStatus = LT.readIrqStatus();
        PRINT_CSTSTR("SendError,");
        PRINT_CSTSTR(",IRQreg,");
        PRINT_HEX("%d", IRQStatus);
        LT.printIrqStatus(); 
      }
          
#ifdef WITH_EEPROM
      // save packet number for next packet in case of reboot     
      my_sx1272config.seq=LT.readTXSeqNo();
      EEPROM.put(0, my_sx1272config);
#endif
      PRINTLN;
      PRINT_CSTSTR("LoRa pkt size ");
      PRINT_VALUE("%d", pl);
      PRINTLN;
      
      PRINT_CSTSTR("LoRa pkt seq ");   
      PRINT_VALUE("%d", LT.readTXSeqNo()-1);
      PRINTLN;
    
      PRINT_CSTSTR("LoRa Sent in ");
      PRINT_VALUE("%ld", endSend-startSend);
      PRINTLN;

///////////////////////////////////////////////////////////////////
// DOWNLINK BLOCK - EDIT ONLY NEW COMMAND SECTION
// 
///////////////////////////////////////////////////////////////////

#ifdef WITH_RCVW
      
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
      uint8_t RXPacketL;
                              
      do {
          PRINT_CSTSTR("Wait for ");
          PRINT_VALUE("%d", (endSend+rxw*DELAY_BEFORE_RCVW) - millis());
          PRINTLN;
    
          //target 1s which is RX1 for LoRaWAN in most regions
          //then target 1s more which is RX2 for LoRaWAN in most regions
          while (millis()-endSend < rxw*DELAY_BEFORE_RCVW)
            ;
          
          PRINT_CSTSTR("Wait for incoming packet-RX");
          PRINT_VALUE("%d", rxw);
          PRINTLN;
            
          // wait for incoming packets
          RXPacketL = LT.receive(message, sizeof(message), 850, WAIT_RX);
          
          //we received something in RX1
          if (RXPacketL && rxw==1)
            rxw=rxw_max+1;
          else
            // try RX2 only if we are in LoRaWAN mode and nothing has been received in RX1
            if (++rxw<=rxw_max) {
#ifdef BAND868
              //change freq to 869.525 as we are targeting RX2 window
              PRINT_CSTSTR("Set downlink frequency to 869.525MHz\n");
              LT.setRfFrequency(869525000, Offset);
#elif defined BAND900
              //TODO?
#elif defined BAND433
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
      // we have received a downlink message
      //
      if (RXPacketL) {  
        int i=0;
        long cmdValue;

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
                      my_sx1272config.addr=node_addr;
                      my_sx1272config.overwrite=1;
                      EEPROM.put(0, my_sx1272config);
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
                      // save new node_addr in case of reboot
                      my_sx1272config.idle_period=idlePeriodInMin;
                      my_sx1272config.overwrite=1;
                      EEPROM.put(0, my_sx1272config);
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

///////////////////////////////////////////////////////////////////
// LOW-POWER BLOCK - DO NOT EDIT
// 
///////////////////////////////////////////////////////////////////

#if defined LOW_POWER && not defined ARDUINO_SAM_DUE
      PRINT_CSTSTR("Switch to power saving mode\n");

      //CONFIGURATION_RETENTION=RETAIN_DATA_RAM on SX128X
      //parameter is ignored on SX127X
      LT.setSleep(CONFIGURATION_RETENTION);

      //how much do we still have to wait, in millisec?
      unsigned long now_millis=millis();

      //PRINTLN_VALUE("%ld",now_millis);
      //PRINTLN_VALUE("%ld",nextTransmissionTime);
      
      if (millis() > nextTransmissionTime)
        nextTransmissionTime=millis()+1000;
        
      unsigned long waiting_t = nextTransmissionTime-now_millis;

      //PRINTLN_VALUE("%ld",waiting_t);
      FLUSHOUTPUT;

      lowPower(waiting_t);
      
      PRINT_CSTSTR("Wake from power saving mode\n");
      LT.wake();      
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
