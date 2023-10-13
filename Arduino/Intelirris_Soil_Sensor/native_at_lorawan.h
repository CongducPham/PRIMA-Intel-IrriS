/*
 *  simple lorawan lib to encode and decode lorawan packet
 *  
 *  Copyright (C) 2016-2020 Congduc Pham, University of Pau, France
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
 *****************************************************************************/
 
#ifndef NATIVE_AT_LORAWAN
#define NATIVE_AT_LORAWAN

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//#define ENABLE_OTAA

#define BOARD_BUILTIN_LED 13

#define LORAWAN_MODULE_BAUD_RATE 9600
#define MLENGTH 100

extern char serial_buff[MLENGTH];
extern unsigned char DevAddr[4];

void dumpHEXtoStr(char* toB, uint8_t* fromB, uint8_t sizeB);
long getAnswerValue(char* strBuff=NULL);
void write_lorawan_module(char* lcmd);
bool read_lorawan_module(bool saveAnswer=true, uint16_t timeout=5000, bool return_on_ok_error=true);
void lorawan_display_config();
bool lorawan_module_setup(int speed);
bool lorawan_transmit(char* buf);
void lorawan_sleep(unsigned long ms);
void lorawan_wake();
bool lorawan_receive();

#endif
