Auto-configuration on boot for the INTEL-IRRIS WaziGate
====================================================

What is it?
-----------

The INTEL-IRRIS WaziGate provides a simple auto-configuration mechanism to automatically update and/or configure the gateway on boot for specific deployment settings using the base INTEL-IRRIS WaziGate SD card image: set frequency band, create pre-configured devices with pre-configured sensors,... For instance, the 433MHz version SD card image with a default pre-configured capacitive-based sensor device is now the only SD card image for download. If you need to have the INTEL-IRRIS WaziGate in 868MHz version or have pre-configuration for Watermark-based sensor devices, then you can use this simple auto-configuration mechanism.

How it works?
-----------

After flashing the INTEL-IRRIS WaziGate SD card image, you can insert the SD card (you may need an SD card to USB adapter) in any computer (Windows, Linux, MacOS) to copy some configuration files in the `/boot` partition of the SD card. The `/boot` partition is in FAT32 format and therefore can easily be accessed (including Copy/Paste operation) from most operating system without any additional software driver. It will usually appear as an additional drive named `boot` on your operating system.

There are basically 3 configuration files you can put in this `/boot` partition:

- `gateway.zip`: a .zip archive with the latest `Gateway` content of the [INTEL-IRRIS GitHub](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Gateway)
- `intel-irris-band.txt`: simply contains either `eu868` or `eu433`
- `intel-irris-auto-config.sh`: a script that mainly configures the WaziGate using its embedded REST API to create devices and sensors

This is how the auto-configuration mechanism works:

- when booting, the INTEL-IRRIS WaziGate executes `/home/pi/intel-irris-auto-config-main.sh` after all containers have been launched. So DO NOT modify this file!

- `/home/pi/intel-irris-auto-config-main.sh` waits for the `wazigate-edge` container to be up and running. 

- if `/boot/gateway.zip` exists then the archive will be unzipped to the `/home/pi` folder, therefore updating (and overwriting) the whole INTEL-IRRIS WaziGate distribution. It is a good solution to update an **existing INTEL-IRRIS WaziGate** without having to re-flash an entire SD card. See `Get latest INTEL-IRRIS WaziGate distribution` section below.

- if `/boot/intel-irris-auto-config.done` exists then no new configuration will be performed. If a new auto-configuration setting needs to be realized, then be sure to remove `/boot/intel-irris-auto-config.done`.

- `/home/pi/intel-irris-auto-config-main.sh` then first looks for `/boot/intel-irris-band.txt` to configure the frequency band. If `/boot/intel-irris-band.txt` exists and contains either `eu868` or `eu433` then the corresponding band is configured for the WaziGate. Otherwise no new frequency band will be configured and the WaziGate will run with the default or last configured frequency band.

- `/home/pi/intel-irris-auto-config-main.sh` then looks for `/boot/intel-irris-auto-config.sh`. If the script exists, it will be launched. `/boot/intel-irris-auto-config.sh` typically calls some utility scripts that are in the `scripts` folder to create pre-configured devices with sensors for the INTEL-IRRIS WaziGate. You can add your additional configuration tasks in this `/boot/intel-irris-auto-config.sh` script. 

- if frequency band configuration has been realized the WaziGate will need to reboot.

- **it means that if frequency band is changed the INTEL-IRRIS WaziGate will need more time to be operational as it needs to boot twice. Consider 5mins as normal for each boot. Therefore 10mins would be needed for first start to have the main INTEL-IRRIS screen on the OLED indicating `SOIL-AREA-1` device.**


The default configuration on the INTEL-IRRIS WaziGate SD card image
-----------

The default configuration is to have the `Gateway/boot/create-starter-kit-demo-capacitive-watermark-st-iiwa-ha/intel-irris-auto-config.sh` configuration in the `/boot` partition of the SD card. When you insert the SD card in a Raspberry Pi, it will automatically configure the INTEL-IRRIS WaziGate with the starter-kit configuration (see [https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Arduino](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Arduino)).

- LoRaWAN mode (single channel)
- Cayenne LPP data format
- EU433 band (for Algeria and Morocco)
- 2 pre-configured devices with address 26011DAA and 26011DB1
- 26011DAA is a soil humidity device with the capacitive SEN0308 sensor
	- Device name is `SOIL-AREA-1`
	- `temperatureSensor_0` as the internal default logical sensor on the WaziGate for soil humidity data. Display will show `Soil Humidity Sensor/Raw value from SEN0308`
	- `temperatureSensor_5` as the internal default logical sensor on the WaziGate for the soil temperature data if a DS18B20 is connected. Display will show `Soil Temperature Sensor/degree Celcius`
	- `analogInput_6` as the internal default logical sensor for battery voltage. Display will show `Battery voltage/volt, low battery whebn lower than 2.85V`
- 26011DB1 is a soil humidity device with the Watermark WM200 tensiometer sensor
	- Device name is `SOIL-AREA-2`
	- `temperatureSensor_0` as the internal default logical sensor on the WaziGate for soil humidity data. It provides the converted resistance value in centibar, Taking into account the soil temperature data. Display will show `Soil Humidity Sensor/centibars from WM200`
	- `temperatureSensor_1` as the internal default logical sensor on the WaziGate for soil humidity data. It provides the raw resistance value measured from the Watermark sensor. The value is scaled down by 10, so to get the real resistance value one must multiply by 10. Display will show `Soil Humidity Sensor/scaled value from WM200 real=x10`	
	- `temperatureSensor_5` as the internal default logical sensor on the WaziGate for the soil temperature data if a DS18B20 is connected. Display will show `Soil Temperature Sensor/degree Celcius`
	- `analogInput_6` as the internal default logical sensor for battery voltage. Display will show `Battery voltage/volt, low battery when lower than 2.85V`


Get latest INTEL-IRRIS WaziGate distribution as `gateway.zip`
---

This procedure is for updating an **existing INTEL-IRRIS WaziGate** without having to re-flash an entire SD card. So, first, shutdown your INTEL-IRRIS WaziGate, then take the SD card out of the RaspberryPi and use an SD card to USB adapter to connect the SD card to your laptop/computer. In most operating system, the `/boot` partition will of the SD card will appear as a `boot` drive.

Second, there are several methods to get only the `Gateway` folder of PRIMA INTEL-IRRIS GitHub: clone repository, use svn ckeckout, ... The simplest method to build the `gateway.zip` archive to update your INTEL-IRRIS WaziGate distribution is to go to [https://download-directory.github.io/](https://download-directory.github.io/), copy/paste this `Gateway` folder url `https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Gateway` and hit Enter to only get the `Gateway` folder. Or simply click [here](https://download-directory.github.io?url=https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Gateway) to start the download. You will get an `CongducPham PRIMA-Intel-IrriS main Gateway.zip` zip file in your download folder that you can then rename in `gateway.zip`. Then, copy this `gateway.zip` archive to the `boot` drive on your laptop/computer which is the `/boot` partition of the SD card.

Then, simply eject the `boot` drive, remove the SD card, insert it in your RaspberryPi and power it. Your INTEL-IRRIS WaziGate will update itself on boot.

Other available configuration examples
===

Example 1: set INTEL-IRRIS WaziGate in 868MHz version
-----------

- flash the INTEL-IRRIS WaziGate SD card image
- insert the SD card in any computer (Windows, Linux, MacOS)
- open the `boot` drive that should appear on your computer
- download from INTEL-IRRIS GitHub (`Gateway/boot`) `intel-irris-band-868.txt` to be copied into the `boot` drive **BUT RENAMED** as `intel-irris-band.txt`
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

