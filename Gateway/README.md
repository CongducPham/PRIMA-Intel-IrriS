WaziGate framework for the PRIMA INTEL-IRRIS project
====================================================

The INTEL-IRRIS gateway is based on the [WaziGate framework](https://www.waziup.io/documentation/wazigate/) from WAZIUP e.V.

INTEL-IRRIS is providing a customized WaziGate distribution so that the starter-kit can work out-of-the-box.

Check on [INTEL-IRRIS web site](http://intel-irris.eu/results) for the SD card image in either EU433 or EU868 frequency band.

Just get the SD card image, flash it on an 8GB SD card class 10 and plug it into a Raspberry Pi (3B/3B+/4B, although 3B is recommended) with a LoRa hat.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/wazigate.jpg" width="300">

The additional code on top of the generic WaziGate framework is provided in this folder, although the SD card image has already everything installed.

Check for the following tutorials/videos on the gateway part:

- [Building the INTEL-IRRIS IoT platform. Part 2: edge-enabled gateway (WaziGate)](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-edge-gateway.pdf). Slides. Related videos: Video n°4.

- [Building the INTEL-IRRIS IoT platform. Part 3: the INTEL-IRRIS starter-kit (WaziGate)](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-starter-kit.pdf). Slides.

- Video n°4: [Demonstrating the INTEL-IRRIS soil sensor device & WaziGate framework for intelligent irrigation in-the-box](https://youtu.be/j-1Nk0tv0xM). **YouTube video**. 

Enjoy!
C. Pham
Coordinator of PRIMA Intel-IrriS

Manuel installation on top of generic WaziGate distribution
====================================================

These are various tools that provide additional features for the INTEL-IRRIS WaziGate.

Start with the generic WaziGate distribution.

After installation, you will get the default configuration for the INTEL-IRRIS WaziGate that is used in the starter-kit. The default soil device will then work out-of-the-box with the INTEL-IRRIS WaziGate.

First step: copy everything to the WaziGate
-------------------------------

Assuming RPI's address is 192.168.2.2:

	> scp -r * pi@192.168.2.2:/home/pi
	
Second step: install the additional packages and the tools
---------------------------------------------

To have the WaziGate in EU433 frequency band:

	> ./install.sh eu433

To have the WaziGate in EU868 frequency band:

	> ./install.sh eu868

