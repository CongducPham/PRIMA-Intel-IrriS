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
