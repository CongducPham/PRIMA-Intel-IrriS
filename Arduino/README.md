Intelirris_Soil_Sensor Arduino code
===================================

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/github-intel-irris-device.png" width="300">


This is the source code of the generic IoT sensor platform. It can handle either

- 1 soil humidity capacitive sensor (e.g. DFRobot SEN0308)

or 

- 1 Irrometer Watermark WM200 tensiometer
 
An additional soil temperature sensor (Dallas DS18B20) can be added.

It supports DIY Arduino ProMini, WaziSense and WaziDev development lines. For WaziSense and WaziDev, the embedded I2C SI7021 sensor can be activated to measure conditions in the device casing, as well as the solar charging circuit (WaziSense).

It is configured by default as single-channel LoRaWAN 1.0 device for both uplink and downlink transmissions.

See the related tutorial slides and videos.

Default configuration for INTEL-IRRIS project (works out-of-the box with the INTEL-IRRIS WaziGate SD card image)
===

- Capacitive sensor SEN0308 connected to A0 (signal) and A1 (power)
- LoRaWAN mode to WaziGate (single channel)
- Cayenne LPP data format
- EU433 band (for Algeria and Morocco)
- Device address is 26011DAA
- 1 measure and transmission every 1 hour
- Battery voltage is included in transmitted messages
- LPP channel 0 is used for soil humidity data, resulting in `temperatureSensor_0` as the internal default logical sensor on the WaziGate for soil humidity data
- LPP channel 5 is used for the soil temperature data if an DS18B20 is connected resulting in `temperatureSensor_5` as the internal default logical sensor on the WaziGate for the soil temperature data
- LPP channel 6 is used for battery voltage resulting in `analogInput_6` as the internal default logical sensor for battery voltage

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/dashboard-voltage.png" width="400">

Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS