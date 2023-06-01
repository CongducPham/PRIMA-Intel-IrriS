/*
 *  simple ping for field test by requesting an ACK from the gateway
 *  version with an OLED display using the U8g2 library
 *  
 *  Copyright (C) 2023 Congduc Pham, University of Pau, France
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
 *
 * last update: May 31st, 2023 by C. Pham
 * 
 * NEW: LoRa communicain library moved from Libelium's lib to StuartProject's lib
 * https://github.com/StuartsProjects/SX12XX-LoRa
 * to support SX126X, SX127X and SX128X chips (SX128X is LoRa in 2.4GHz band)
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
// define a dedicated ping device with address 0x26011DA0
// need to create a dedicated device on the WaziGate
//#define DEDICATED_PING_DEVICE

////////////////////////////////////////////////////////////////////
// Frequency band - do not change in SX127X_RadioSettings.h anymore
//#define BAND868
//#define BAND915 
#define BAND433

////////////////////////////////////////////////////////////////////
#define BOOT_START_MSG  "\nField tester for LoRa/LoRaWAN coverage\n"

//uncomment to use LPP format to send to WAZIGATE for instance
//so uncomment LPP only with LORAWAN to WAZIGATE
#ifdef TO_WAZIGATE
#define LORAWAN
#define WITH_RCVW
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
//#define WITH_EEPROM
#ifndef TO_WAZIGATE
//#define LORAWAN
#endif
////////////////////////////
//Use LoRaWAN AES-like encryption
//#define WITH_AES
////////////////////////////
//normal behavior is to invert IQ for RX, the normal behavior at gateway is also to invert its IQ setting, only valid with WITH_RCVW
#define INVERTIQ_ON_RX

////////////////////////////
//uncomment to use a customized frequency. TTN plan includes 868.1/868.3/868.5/867.1/867.3/867.5/867.7/867.9 for LoRa
//#define MY_FREQUENCY 868100000
//#define MY_FREQUENCY 433175000

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE NODE ADDRESS IN NO LORAWAN MODE
uint8_t node_addr=8;
//////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE TIME IN MINUTES BETWEEN 2 READING & TRANSMISSION
unsigned int idlePeriodInMin = 0;
unsigned int idlePeriodInSec = 120;
///////////////////////////////////////////////////////////////////

/********************************************************************
  _        ___    __      ___   _  _ 
 | |   ___| _ \__ \ \    / /_\ | \| |
 | |__/ _ \   / _` \ \/\/ / _ \| .` |
 |____\___/_|_\__,_|\_/\_/_/ \_\_|\_|
********************************************************************/
                                     
///////////////////////////////////////////////////////////////////
// LORAWAN
#ifdef LORAWAN
#ifdef DEDICATED_PING_DEVICE
unsigned char DevAddr[4] = {0x26, 0x01, 0x1D, 0xA0};
#else
//otherwise just take the address of the default capacitive sensor 
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
#define OLED
//#define OLED
//various predefined connection setups for OLED
// GND, VCC, SCL, SDA
//Suitable for WaziSense v2
//#define OLED_GND5A5A4 //5 as VCC
//for other boards – pin 4 is not available when LoRa RST is on pin 4
//#define OLED_GND235 //suitable even with 1 capacitive or 1 tensiometer+temp sensor attached
#define OLED_9GND876 //9 as GND

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
XLPP lpp(120);
#endif

//keep track of the number of successful transmissions
uint32_t TXPacketCount=1;
uint32_t TXPacketSuccess=0;

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
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 3, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
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

#ifdef WITH_EEPROM
struct sx1272config {

  uint8_t flag1;
  uint8_t flag2;
  uint8_t seq;
  uint8_t overwrite;
  // can add other fields such as LoRa mode,...
};

sx1272config my_sx1272config;
#endif

#ifdef WITH_RCVW
// will wait for 1s before opening the rcv window
#define DELAY_BEFORE_RCVW 1000
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
  
  delay(1000);
  // Open serial communications and wait for port to open:
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0 
  SerialUSB.begin(38400);
#else
  Serial.begin(38400);  
#endif

#ifdef OLED
#ifdef OLED_PWR_PIN
  pinMode(OLED_PWR_PIN, OUTPUT);
  digitalWrite(OLED_PWR_PIN, HIGH);
#ifdef OLED_9GND876
  //use pin 9 as ground
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
#endif
#endif
#endif
 
////////////////////////////////////////////////////////////////// 
  
#ifdef OLED
  digitalWrite(OLED_PWR_PIN, HIGH);
  delay(200);
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  //u8x8.setFont(u8x8_font_pxplustandynewtv_r);
  u8x8.drawString(0, 0, "FIELD TESTER");

  delay(2000);
#ifdef LOW_POWER  
  digitalWrite(OLED_PWR_PIN, LOW);  
#endif  
#endif  
  
  // Print a start message
  PRINT_CSTSTR(BOOT_START_MSG);

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
    delay(500);
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

    my_sx1272config.flag1=0x12;
    my_sx1272config.flag2=0x35;   
    my_sx1272config.seq=LT.readTXSeqNo(); 
    my_sx1272config.overwrite=0;
    EEPROM.put(0, my_sx1272config);  

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

  PRINTLN;
  PRINTLN_CSTSTR("Field tester ready");

#ifdef OLED
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  //u8x8.setFont(u8x8_font_pxplustandynewtv_r);
  u8x8.drawString(0, 0, "FIELD TESTER");
  sprintf(oled_msg,"SF%dBW%d", LT.getLoRaSF(), LT.returnBandwidth()/1000);
  u8x8.drawString(0, 1, oled_msg); 
#endif  
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
  uint8_t r_size;
      
  while (1) {

#if defined USE_XLPP || defined USE_LPP
    // Create lpp payload.
    lpp.reset();
    //we use channel 10 for the field tester
    uint8_t ch=10;
#endif
      
    r_size=sprintf((char*)message, "PING");

    PRINT_CSTSTR("Sending ");
    PRINT_STR("%s",(char*)(message));
    PRINTLN;

#if defined USE_XLPP
    PRINT_CSTSTR("use LPP format for transmission to WaziGate\n");
    lpp.addDigitalOutput(ch, TXPacketCount);
#else
    PRINT_CSTSTR("Real payload size is ");
    PRINT_VALUE("%d", r_size);
    PRINTLN;

    LT.printASCIIPacket(message, r_size);
    PRINTLN;
#endif      
    int pl=r_size;
      
#ifdef OLED
    u8x8.clearLine(3);
    u8x8.drawString(0, 3, "Sending Ping....");
    u8x8.clearLine(4);
    u8x8.clearLine(5);
    u8x8.clearLine(6);
#endif

#ifdef WITH_AES
#ifdef LORAWAN
    PRINT_CSTSTR("end-device uses native LoRaWAN packet format\n");
#if defined USE_XLPP
    pl=local_aes_lorawan_create_pkt(lpp.buf, lpp.len, app_key_offset);   
#else
    pl=local_aes_lorawan_create_pkt(message, pl, app_key_offset);      
#endif        
#else
    PRINT_CSTSTR("end-device uses encapsulated LoRaWAN packet format only for encryption\n");
    pl=local_aes_lorawan_create_pkt(message, pl, app_key_offset);      
#endif
#endif

    startSend=millis();

    //LT.CarrierSense();

    uint8_t p_type=PKT_TYPE_DATA | PKT_FLAG_ACK_REQ;
      
    PRINT_CSTSTR("Will request an ACK\n"); 

#ifdef LORAWAN
    //will return packet length sent if OK, otherwise 0 if transmit error
    //we use raw format for LoRaWAN
#if defined USE_XLPP
    if (LT.transmit(lpp.buf, pl, 10000, MAX_DBM, WAIT_TX))
#else      
    if (LT.transmit(message, pl, 10000, MAX_DBM, WAIT_TX))
#endif       
#else
    //will return packet length sent if OK, otherwise 0 if transmit error
    if (LT.transmitAddressed(message, pl, p_type, DEFAULT_DEST_ADDR, node_addr, 10000, MAX_DBM, WAIT_TX))  
#endif
    {
      endSend=millis();                                         
      uint16_t localCRC = LT.CRCCCITT(message, r_size, 0xFFFF);
      PRINT_CSTSTR("CRC,");
      PRINT_HEX("%d", localCRC);

#ifndef LORAWAN
      if (LT.readAckStatus()) {
        PRINT_CSTSTR("\nReceived ACK from gateway!\n");
        TXPacketSuccess++;
        sprintf((char*)message,"SNR at gw=%d   ", LT.readPacketSNRinACK());
        PRINTLN;
        PRINT_STR("%s", (char*)message);   

#ifdef OLED
        u8x8.clearLine(3);             
        u8x8.drawString(0, 3, "Get ACK from gw");
        u8x8.drawString(0, 4, (char*)message);
#endif
        sprintf((char*)message,"gw->SNR=%d:%d", LT.readPacketSNR(), LT.readPacketRSSI());
        PRINTLN;
        PRINT_STR("%s", (char*)message);

#ifdef OLED
        u8x8.clearLine(5);             
        u8x8.drawString(0, 5, (char*)message);
#endif
      }
      else {
        PRINTLN;
        PRINT_CSTSTR("No ACK from gw!");
#ifdef OLED
        u8x8.clearLine(3);          
        u8x8.drawString(0, 3, "No ACK from gw!");      
#endif         
      }
#endif      
    }
    else
    {
      endSend=millis();
      //if there was an error transmitting packet
      uint16_t IRQStatus;
      IRQStatus = LT.readIrqStatus();
      PRINTLN;                      
      PRINT_CSTSTR("SendError,");
      PRINT_CSTSTR(",IRQreg,");
      PRINT_HEX("%d", IRQStatus);
      LT.printIrqStatus();
#ifdef OLED          
      u8x8.clearLine(3);
      u8x8.drawString(0, 3, "Error sending");  
#endif          
    }

///////////////////////////////////////////////////////////////////
// DOWNLINK BLOCK 
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
          //PRINTLN_VALUE("%d", endSend);
          //PRINTLN_VALUE("%d", millis());        
          //PRINT_CSTSTR("Wait for ");
          //PRINT_VALUE("%d", (endSend+rxw*DELAY_BEFORE_RCVW) - millis());
          //PRINTLN;

          PRINT_CSTSTR("Wait for incoming packet-RX");
          PRINTLN_VALUE("%d", rxw);
                    
          //target 1s which is RX1 for LoRaWAN in most regions
          //then target 1s more which is RX2 for LoRaWAN in most regions
          //while (millis()-endSend < rxw*DELAY_BEFORE_RCVW)
          //  ;

          //we take it safely so we open the receive window much earlier
          //to not miss the downlink packet from the gw
                      
          // wait for incoming packets
          RXPacketL = LT.receive(message, sizeof(message), 1800, WAIT_RX);

          //PRINTLN_VALUE("%d", millis());
          
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
#elif defined BAND915
              //change freq to 923.3 as we are targeting RX2 window
              PRINT_CSTSTR("Set downlink frequency to 923.3MHz\n");
              LT.setRfFrequency(923300000, Offset);
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
        PRINT_CSTSTR("\nReceived ACK from gateway!\n");

        TXPacketSuccess++;
        
        sprintf((char*)message,"gw->SNR=%d:%d", LT.readPacketSNR(), LT.readPacketRSSI());
        PRINTLN;
        PRINT_STR("%s", (char*)message);

#ifdef OLED
        u8x8.clearLine(3);             
        u8x8.drawString(0, 3, "Get ACK from gw");
        u8x8.clearLine(5);             
        u8x8.drawString(0, 5, (char*)message);        
#endif    
      }
      else {
        PRINT_CSTSTR("\nNo ACK from gw!\n");
#ifdef OLED
        u8x8.clearLine(3);          
        u8x8.drawString(0, 3, "No ACK from gw!");      
#endif
      }
#endif               

    sprintf((char*)message,"PingNum=%d", TXPacketCount);
    PRINTLN;
    PRINT_STR("%s", (char*)message);
    
#ifdef OLED
    u8x8.clearLine(6);
    u8x8.drawString(0, 6, (char*)message);
#endif

    sprintf((char*)message,"Success=%d", TXPacketSuccess);
    PRINTLN;
    PRINT_STR("%s", (char*)message);

#ifdef OLED
    u8x8.clearLine(7);
    u8x8.drawString(0, 7, (char*)message);        
#endif

    TXPacketCount++;
    PRINTLN;  

#ifdef WITH_EEPROM
    // save packet number for next packet in case of reboot     
    my_sx1272config.seq=LT.readTXSeqNo();
    EEPROM.put(0, my_sx1272config);
#endif
    PRINT_CSTSTR("\nLoRa pkt size ");
    PRINT_VALUE("%d", pl);
    
    PRINT_CSTSTR("\nLoRa pkt seq ");   
    PRINT_VALUE("%d", LT.readTXSeqNo()-1);
    
    PRINT_CSTSTR("\nLoRa Sent in ");
    PRINT_VALUE("%ld", endSend-startSend);
          
    delay(idlePeriodInMin*60*1000+idlePeriodInSec*1000);   
  }          
}
