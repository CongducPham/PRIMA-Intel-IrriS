/*
 *  simple lorawan lib to encode and decode lorawan packet
 *  
 *  Copyright (C) 2016-2024 Congduc Pham, University of Pau, France
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
 
#ifndef LOCAL_LORAWAN
#define LOCAL_LORAWAN

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "AES-128_V10.h"
#include "Encrypt_V31.h"

#define AES_SHOWB64
#ifdef AES_SHOWB64
#include "Base64.h" 
#endif

extern uint8_t node_addr;
extern unsigned char DevAddr[4];

/* FROM lorabase.h of LMIC */

enum {
    // Data frame format
    OFF_DAT_HDR      = 0,
    OFF_DAT_ADDR     = 1,
    OFF_DAT_FCT      = 5,
    OFF_DAT_SEQNO    = 6,
    OFF_DAT_OPTS     = 8,
};

enum {
    // Bitfields in frame control octet
    FCT_ADREN  = 0x80,
    FCT_ADRARQ = 0x40,
    FCT_ACK    = 0x20,
    FCT_MORE   = 0x10,   // also in DN direction: Class B indicator
    FCT_OPTLEN = 0x0F,
};

/*------------------------ */

void local_lorawan_init();
uint8_t local_aes_lorawan_create_pkt(uint8_t* message, uint8_t pl, uint8_t app_key_offset);
int8_t local_lorawan_decode_pkt(uint8_t* message, uint8_t pl);

#endif
