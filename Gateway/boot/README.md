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

- it means that if auto-configuration mechanism has been realized, the INTEL-IRRIS WaziGate will need more time to be operational as it needs to boot twice.


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

