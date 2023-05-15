Intelirris_Soil_Sensor Arduino code
===================================

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/github-intel-irris-device.png" width="300">


This is the source code of the INTEL-IRRIS soil humidity sensor device. It can handle either

- 1 soil humidity capacitive sensor (e.g. DFRobot SEN0308)

or 

- up to 2 Irrometer Watermark WM200 tensiometers
 
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

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/INTEL-IRRIS-wazigate-default-dashboard.png" width="900">

Arduino_LoRa_SX12XX_Ping_OLED Arduino code
===================================

The `Arduino_LoRa_SX12XX_Ping_OLED.ino` code is a simple Field Tester tool that can be flashed on a dedicated device to perform field coverage tests. This dedicated device can be built very similarly to an INTEL-IRRIS sensor device. 

The Field Tester device will send every 120s a `Confirmed Data Up` LoRaWAN packet to the INTEL-IRRIS WaziGate gateway to request a downlink acknowledgment packet. If the device receives such acknowledgement it would indicate that the uplink packet has been successfully received by the INTEL-IRRIS WaziGate. When deploying INTEL-IRRIS soil humidity sensor devices, the Field Tester device is useful to determine whether the device can reach the gateway, without having to check on the gateway itself.

The Field Tester device supports a small OLED screen that will indicate if the downlink acknowledgment packet has been received by the field tester device or not. 

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-oled.JPG" width="400">

By default in the code, the OLED can be connected as follows:

```
       Arduino        OLED 
           9----------GND   (Arduino 9 will serve as GND)
           8----------VCC   (Arduino 8 will provide 3.3V)
           7----------SCL   (Arduino 7 will act as SCL)
           6----------SDA   (Arduino 6 will act as SDA)
``` 

Although having a **dedicated Field Tester device is recommended**, if you do not have such spare device available, the Field Tester can be flashed temporarily on an INTEL-IRRIS device. In this case, it is easier to connect the OLED to an INTEL-IRRIS device with a capacitive device sensor where pin 9, 8, 7 and 6 are available. 

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-capacitive-1.JPG" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-capacitive-2.JPG" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-oled-case.JPG" width="400">

**You can make a dedicated spare case cover with a small hole to pass the OLED wire through. Once field test is over, put back the original cover of the INTEL-IRRIS device if needed.**

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-oled-case-1.JPG" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-oled-case-2.JPG" width="400">

If an INTEL-IRRIS device with a tensiometer & temperature sensor was to be used, then it is easier to connect as follows:

```
       Arduino        OLED 
         GND----------GND
           2----------VCC   (Arduino 2 will provide 3.3V)
           3----------SCL   (Arduino 3 will act as SCL)
           5----------SDA   (Arduino 5 will act as SDA)
```

In the code, `#define OLED_GND235` should be decommented instead of `#define OLED_9GND876`

The default configuration of the field tester device is:

- LoRaWAN mode to WaziGate (single channel)
- EU433 band (for Algeria and Morocco)
- Cayenne LPP data format
- Device address is 26011DAA (same than the capacitive device)
- 1 transmission every 120s
- LPP channel 10 is used for the packet sequence number (starting at 1) resulting in `digitalOutput_10` as the internal default logical sensor on the WaziGate to receive the packet sequence number on the WaziGate dashboard. The WaziGate dashboard will display in the default capacitive device UI card the newly discovered `digitaloutput` sensor which will hold the packet sequence.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-dashboard.png" width="400">

After testing coverage, you can simply delete the `digitaloutput` sensor of the default capacitive device.

Note that the SNR (Signal to Noise Ratio) indicated on the OLED screen is the SNR of the acknowledgement. To know the SNR of the uplink (uplink means from device to gateway) transmission (thus the quality of the uplink transmission), you need to check on the WaziGate itself, using the live LoRaWAN frame capture feature of the embedded ChirpStack Network Server User Interface. To access the ChirpStack Network Server on the WaziGate, just connect to its WiFi network, then open `http://10.42.0.1:8080` with a web browser. Log in as `admin` user with `admin` password. Then go the `Gateway` menu, click on the WaziGate gateway and then select `LIVE LORAWAN FRAME` tab.

You will see uplink packets coming in and the associated downlink acknowledgement if the WaziGate received the uplink packet. Click on `Uplink` to get details on the uplink transmission. Then look for `loRaSNR` that indicates the SNR of the uplink transmission. Here we can read `loRaSNR=5`.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/field-tester-chirpstack.png" width="950">

Here is a simple table to assess on the radio quality of the uplink transmission.

- SNR < -15 : reception can be correct but packet can be lost frequently, it is somehow the range limit
- SNR < -5 : reception can be correct most of the time
- SNR > 0: quite good radio quality, we can expect 100% packet reception
- SNR > 3: good radio quality, we can expect 100% packet reception
- SNR > 6: very good radio quality, we can expect 100% packet reception

**IMPORTANT NOTICE: it is possible that the gateway receives the uplink transmission but the Field Testerdevice can not receive the acknowledgement. Therefore, in case you don't see any acknowledgment on the Field Tester, always check on the WaziGate.**

If the radio signal is weak, you can improve it with several actions:

- try to avoid placing the gateway antenna against a thick wall
- try to raise the height of both device and gateway antenna
- try to put the gateway antenna outside, using an outdoor antenna with a antenna cable extension
- try to use a more efficient antenna for the device, or for the gateway or for both

For those who would like a more elaborated LoRaWAN Field Tester, they can look at this great project: [https://github.com/disk91/WioLoRaWANFieldTester](https://github.com/disk91/WioLoRaWANFieldTester).

Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS