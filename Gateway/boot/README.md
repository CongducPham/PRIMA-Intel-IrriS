Auto-configuration on boot for the INTEL-IRRIS WaziGate
====================================================

What is it?
-----------

The INTEL-IRRIS WaziGate provides a simple auto-configuration mechanism to automatically configure the gateway on boot for specific deployment settings using the base INTEL-IRRIS WaziGate SD card image: set frequency band, create pre-configured devices with pre-configured sensors,... For instance, the 433MHz version SD card image with a default pre-configured capacitive-based sensor device is now the only SD card image for download. If you need to have the INTEL-IRRIS WaziGate in 868MHz version or have pre-configuration for Watermark-based sensor devices, then you can use this simple auto-configuration mechanism.

How it works?
-----------

After flashing the INTEL-IRRIS WaziGate SD card image, you can insert the SD card in any computer (Windows, Linux, MacOS) to copy some configuration files in the `/boot` partition of the SD card that is in FAT32 format and therefore easily accessible from most operating system.

There are basically 2 configuration files you can put in this `/boot` partition:

- `intel-irris-band.txt`: simply contains either `eu868` or `eu433`
- `intel-irris-auto-config.sh`: runs a number of shell scripts to configure the WaziGate using its embedded REST API

This is how the auto-configuration mechanism works:

- when booting, the INTEL-IRRIS WaziGate executes `/home/pi/intel-irris-auto-config-main.sh` after all containers have been launched. So DO NOT modify this file!

- `/home/pi/intel-irris-auto-config-main.sh` waits for the `wazigate-edge` container to be up and running. 

- if `/boot/intel-irris-auto-config.done` exists then no new configuration will be performed. If a new auto-configuration setting needs to be realized, then be sure to remove `/boot/intel-irris-auto-config.done`.

- `/home/pi/intel-irris-auto-config-main.sh` then first looks for `/boot/intel-irris-band.txt` to configure the frequency band. If `/boot/intel-irris-band.txt` exists and contains either `eu868` or `eu433` then the corresponding band is configured for the WaziGate. Otherwise no new frequency band will be configured and the WaziGate will run with the default or last configured frequency band.

- `/home/pi/intel-irris-auto-config-main.sh` then looks for `/boot/intel-irris-auto-config.sh`. If the script exists, it will be launched. `/boot/intel-irris-auto-config.sh` typically calls some utility scripts that are in the `scripts` folder to create pre-configured devices with sensors for the INTEL-IRRIS WaziGate. You can add your additional configuration tasks in this `/boot/intel-irris-auto-config.sh` script. 

- if frequency band configuration has been realized or if `/boot/intel-irris-auto-config.sh` has been executed, `/home/pi/intel-irris-auto-config-main.sh` creates `/boot/intel-irris-auto-config.done` to indicate that the auto-configuration has been performed. The WaziGate is then rebooted.

- **it means that if auto-configuration mechanism has been realized, the INTEL-IRRIS WaziGate will need more time to be operational as it needs to boot twice. Consider 5mins as normal for each boot. Therefore 10mins would be needed for first start to have the main INTEL-IRRIS screen on the OLED indicating `SOIL-AREA-1` device.**


The default configuration on the INTEL-IRRIS WaziGate SD card image
-----------

The default configuration is to have the `Gateway/boot/` `create-default-capacitive/intel-irris-auto-config.sh` configuration in the `/boot` partition of the SD card. When you insert the SD card in a Raspberry Pi, it will automatically configure the INTEL-IRRIS WaziGate with the configuration matching the INTEL-IRRIS default soil humidity device sensor (see [https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Arduino](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Arduino)).

- LoRaWAN mode (single channel)
- Cayenne LPP data format
- EU433 band (for Algeria and Morocco)
- One pre-configured device with address 26011DAA
- Soil humidity sensor is the capacitive SEN0308
- Device name is `SOIL-AREA-1`
- `temperatureSensor_0` as the internal default logical sensor on the WaziGate for soil humidity data. Display will show `Soil Humidity Sensor/Raw value from SEN0308`
- `temperatureSensor_5` as the internal default logical sensor on the WaziGate for the soil temperature data if a DS18B20 is connected. Display will show `Soil Temperature Sensor/degree Celcius`
- `analogInput_6` as the internal default logical sensor for battery voltage. Display will show `Battery voltage/volt, low battery whebn lower than 2.85V`

Other available configuration examples
===

Example 1: set INTEL-IRRIS WaziGate in 868MHz version
-----------

- flash the INTEL-IRRIS WaziGate SD card image
- insert the SD card in any computer (Windows, Linux, MacOS)
- open the `boot` drive that should appear on your computer
- download from INTEL-IRRIS GitHub (`Gateway/boot`) `intel-irris-band-868.txt` to be copied into the `boot` drive BUT RENAMED as `intel-irris-band.txt`
- be sure that there is no `intel-irris-auto-config.done` file in the `boot` drive, otherwise delete the file
- safely eject the `boot` drive
- insert the SD card in the RPI and power the RPI

Example 2: have the INTEL-IRRIS WaziGate working with a Watermark-based device
-----------

- flash the INTEL-IRRIS WaziGate SD card image
- insert the SD card in any computer (Windows, Linux, MacOS)
- open the `boot` drive that should appear on your computer
- download from INTEL-IRRIS GitHub (`Gateway/boot`) `create-default-watermark/intel-irris-auto-config.sh` to be copied into the `boot` drive (keep same file name)
- be sure that there is no `intel-irris-auto-config.done` file in the `boot` drive, otherwise delete the file
- safely eject the `boot` drive
- insert the SD card in the RPI and power the RPI

Example 3: have the INTEL-IRRIS WaziGate working with 4 Watermark-based devices
-----------

- flash the INTEL-IRRIS WaziGate SD card image
- insert the SD card in any computer (Windows, Linux, MacOS)
- open the `boot` drive that should appear on your computer
- download from INTEL-IRRIS GitHub (`Gateway/boot`) `create-4-watermark/intel-irris-auto-config.sh` to be copied into the `boot` drive (keep same file name)
- be sure that there is no `intel-irris-auto-config.done` file in the `boot` drive, otherwise delete the file
- safely eject the `boot` drive
- insert the SD card in the RPI and power the RPI

Example 4: have the INTEL-IRRIS WaziGate working with a customized setting
-----------

- flash the INTEL-IRRIS WaziGate SD card image
- insert the SD card in any computer (Windows, Linux, MacOS)
- open the `boot` drive that should appear on your computer
- download from INTEL-IRRIS GitHub (`Gateway/boot`) `create-custom-example/intel-irris-auto-config.sh` and see how the script creates one capacitive and one tensiometer device (SOIL-AREA-1/26011DAA and SOIL-AREA-2/26011DB2)
- based on this example, you can create on your computer an `intel-irris-auto-config.sh` script that actually creates and configures devices according to your setting
- copy the file into the `boot` drive (keep same file name)
- be sure that there is no `intel-irris-auto-config.done` file in the `boot` drive, otherwise delete the file
- safely eject the `boot` drive
- insert the SD card in the RPI and power the RPI

Example 5: have the INTEL-IRRIS WaziGate working with a Watermark-based device + 1 soil temperature sensor
-----------

- flash the INTEL-IRRIS WaziGate SD card image
- insert the SD card in any computer (Windows, Linux, MacOS)
- open the `boot` drive that should appear on your computer
- download from INTEL-IRRIS GitHub (`Gateway/boot`) `create-default-watermark-st/intel-irris-auto-config.sh` to be copied into the `boot` drive (keep same file name)
- be sure that there is no `intel-irris-auto-config.done` file in the `boot` drive, otherwise delete the file
- safely eject the `boot` drive
- insert the SD card in the RPI and power the RPI


Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS

