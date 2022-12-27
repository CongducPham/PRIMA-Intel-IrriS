WaziGate framework for the PRIMA INTEL-IRRIS project
====================================================

The INTEL-IRRIS gateway is based on the [WaziGate framework](https://www.waziup.io/documentation/wazigate/) from WAZIUP e.V.

INTEL-IRRIS is providing a customized WaziGate distribution so that the starter-kit can work out-of-the-box.

Check on [INTEL-IRRIS web site](http://intel-irris.eu/results) for the SD card image.

Just get the SD card image, flash it on an 8GB SD card class 10 and plug it into a Raspberry Pi (3B/3B+/4B, although 3B is recommended) with a LoRa hat.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/wazigate.jpg" width="300">

The additional code on top of the generic WaziGate framework is provided in this folder, **although the SD card image has already everything installed**.

Check for the following tutorials/videos on the gateway part:

- [Building the INTEL-IRRIS IoT platform. Part 2: edge-enabled gateway (WaziGate)](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-edge-gateway.pdf). Slides. Related videos: Video n°4.

- [Building the INTEL-IRRIS IoT platform. Part 3: the INTEL-IRRIS starter-kit (WaziGate)](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-starter-kit.pdf). Slides. These slides are targeting technology partners to show them of to build the starter-kit from the parts.

- Video n°4: [Demonstrating the INTEL-IRRIS soil sensor device & WaziGate framework for intelligent irrigation in-the-box](https://youtu.be/j-1Nk0tv0xM). **YouTube video**. 

Default configuration for INTEL-IRRIS project (works out-of-the box with the INTEL-IRRIS `Intelirris_Soil_Sensor Arduino` code)
===

- LoRaWAN mode (single channel)
- Cayenne LPP data format
- EU433 band (for Algeria and Morocco)
- 2 pre-configured device with address 26011DAA and 26011DB1
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
	- `analogInput_6` as the internal default logical sensor for battery voltage. Display will show `Battery voltage/volt, low battery whebn lower than 2.85V`

Insert the SD card in the Raspberry Pi and then power the RPI. The INTEL-IRRIS WaziGate is ready when the main INTEL-IRRIS screen appears on the OLED indicating `SOIL-AREA-1` and `SOIL-AREA-2` devices. You may see a succession of `[ Internet NO ]` and/or `[ Internet OK ]` and black screen before the main INTEL-IRRIS screen appears on the OLED.

**Note: The 2 default devices are created on first boot with the auto-configuration mechanism. Consider about 5mins as normal for boot time. If you change the frequency band, the gateway will take more time to start as it needs to boot twice. Therefore 10mins would be needed for first start to have the main INTEL-IRRIS screen on the OLED.**

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/default-dashboard" width="700">

Manual installation on top of generic WaziGate distribution
===========================================================

**The SD card image has already everything installed. Manual installation procedure is provided for information only.**

The customized WaziGate distribution consists in a pre-configured WaziGate with 1 device and either a capacitive SEN0308 sensor or a Watermark water tension sensor.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/INTEL-IRRIS-wazigate-default-dashboard.png" width="500">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/INTEL-IRRIS-wazigate-default-sensor-data.png" width="300">

It also adds an OLED screen which will quickly display sensor information to the user, without having to log in the WaziGate dashboard for the full and advanced user interface.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/oled-cycling.png" width="500">

The distribution therefore provides various tools to install and configure the generic WaziGate distribution to produce the INTEL-IRRIS WaziGate distribution that is used in the starter-kit and that should be similar to the distributed SD card image. The default soil device will then work out-of-the-box with the INTEL-IRRIS WaziGate.

Start with the generic WaziGate distribution
--------------------------------------------

Get the WaziGate image from [https://www.waziup.io/downloads/](https://www.waziup.io/downloads/). Flash it on an 8GB SD card.

Connect the Raspberry Pi (WaziGate) to your laptop/desktop which shares its Internet connection to the WaziGate to enable installation procedure. Power the WaziGate, allows for 6 to 8mins as the first boot initializes a lot of components of the WaziGate. Then, log in the WaziGate as pi user with default password `loragateway`.

	> ssh pi@wazigate.local

All these steps are explained on [https://www.waziup.io/documentation/wazigate/v2/install/](https://www.waziup.io/documentation/wazigate/v2/install/).

With the WaziGate is connected to your laptop/desktop which shares its Internet connection to the WaziGate, you can now start the INTEL-IRRIS-specific installation procedure.

First step: copy everything to the WaziGate
-------------------------------------------

Assuming you downloaded the whole repo on your laptop/desktop computer:

	> cd Gateway
	> scp -r * pi@wazigate.local:/home/pi
	
Second step: install the additional packages and the tools
----------------------------------------------------------

To have the WaziGate in EU433 frequency band:

	> ./install.sh eu433

To have the WaziGate in EU868 frequency band:

	> ./install.sh eu868
	
The script updates the RaspberryOS, installs `pip3` for `python3`, installs the `adafruit-circuitpython-ssd1306` and `python3-pil` packages for the OLED then installs the OLED service as a system service.

The last step is to configure the WaziGate with a pre-configured device matching the configuration of the INTEL-IRRIS soil sensor device Arduino program (see [https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Gateway/boot](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Gateway/boot)).

Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS

