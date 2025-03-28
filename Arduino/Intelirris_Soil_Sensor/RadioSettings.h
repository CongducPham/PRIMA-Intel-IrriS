///////////////////////////////////////////////////////////////////////////////////////////////////////////
// please uncomment only 1 choice 
//#define SX126X
#define SX127X
//#define SX128X
//#define RAK3172
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef RAK3172
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// please uncomment only 1 choice
#define EU868
//#define AU915
//#define EU433
//#define AS923_2
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

//To explicitely activate the local LoRaWAN software stack if you do not have a LoRaWAN radio module
#define LORAWAN

#ifdef RAK3172
  #define LORAWAN
  #define TO_LORAWAN_GW
  #define NATIVE_LORAWAN
  #define WITH_AT_COMMANDS
#else
  #define RAW_LORA
  #ifdef LORAWAN
    #define CUSTOM_LORAWAN
  #endif
  #define WITH_SPI_COMMANDS
#endif
