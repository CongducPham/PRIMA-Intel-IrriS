Low-cost LoRa IoT platform for the PRIMA INTEL-IRRIS project
============================================================

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/intel-irris-logo.png" width="300">

Latest news
-----------

- **NEW-Nov-21** We created the GitHub repository for the PRIMA/EU INTEL-IRRIS project. Check project [web site](http://intel-irris.eu/) and follow us on [Twitter](https://twitter.com/Intel_IrriS).


Quick start
-----------

The [Arduino sketch](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Arduino/Intelirris_Soil_Sensor) for the soil sensor device is in the [Arduino folder](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Arduino).

The INTEL-IRRIS gateway part is based on the [WaziGate](https://github.com/Waziup/WaziGate) framework.

The WaziGate generic framework is enhanced with INTEL-IRRIS specific UI and irrigation data processing. These additional contributions are in the `Gateway` folder **but the SD card image has already everything installed**.

The INTEL-IRRIS WaziGate SD card image is available on [http://intel-irris.eu/results](http://intel-irris.eu/results). It is configured by default for 433MHz frequency band. To use 868MHz frequency band, see the [868MHz auto-configuration mechanism](https://github.com/CongducPham/PRIMA-Intel-IrriS/tree/main/Gateway/boot#example-1-set-intel-irris-wazigate-in-868mhz-version).

Default configuration for the gateway
---

- LoRaWAN mode (single channel)
- Cayenne LPP data format
- EU433 band (for Algeria and Morocco)
- One pre-configured device with address 26011DAA
- Soil humidity sensor is the capacitive SEN0308
- Device name is `SOIL-AREA-1`
- `temperatureSensor_0` as the internal default logical sensor on the WaziGate for soil humidity data. Display will show `Soil Humidity Sensor/Raw value from SEN0308`
- `temperatureSensor_5` as the internal default logical sensor on the WaziGate for the soil temperature data if a DS18B20 is connected. Display will show `Soil Temperature Sensor/degree Celcius`
- `analogInput_6` as the internal default logical sensor for battery voltage. Display will show `Battery voltage/volt, low battery whebn lower than 2.85V`

Insert the SD card in the Raspberry Pi and then power the RPI. The INTEL-IRRIS WaziGate is ready when the main INTEL-IRRIS screen appears on the OLED.

**Note: The default capacitive device is created on first boot with the auto-configuration mechanism. Therefore, for the first boot, and only the first one, the gateway will take more time to start as it needs to boot twice. Consider 5-6mins as normal.**

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/dashboard-voltage.png" width="400">

Tutorials
---------

General presentation of the PRIMA INTEL-IRRIS project

- [Intel-IrriS’s presentation slides](http://cpham.perso.univ-pau.fr/LORA/INTEL-IRRIS/Intel-Irris-presentation.pdf)

1 general online tutorial on IoT, Arduino, LoRa, AI 

- [Arduino-Sensor-LoRa tutorial](https://cpham.perso.univ-pau.fr/LORA/WAZIUP/tuto/index.html)

The generic low-cost IoT framework from which the INTEL-IRRIS platform is derived

- [Low-cost IoT framework](https://github.com/CongducPham/LowCostLoRaGw)

The WaziGate edge-enabled IoT gateway framework developed by WAZIUP

- [WaziGate presentation](https://www.waziup.io/documentation/wazigate/)

INTEL-IRRIS specific tutorials

- [Building the INTEL-IRRIS IoT platform. Part 1: soil sensor device](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-IOT-platform.pdf). Slides. Related videos: Video n°1, Video n°2 and Video n°3.

- [Building the INTEL-IRRIS IoT platform. Part 2: edge-enabled gateway (WaziGate)](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-edge-gateway.pdf). Slides. Related videos: Video n°4.

- [Building the INTEL-IRRIS IoT platform. Part 3: the INTEL-IRRIS starter-kit (WaziGate)](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-starter-kit.pdf). Slides. Related videos: Video n°5.

- Video n°1: [Build the LoRa IoT microcontroller platform for the PRIMA INTEL-IRRIS project](https://youtu.be/3jdQ0Uo0phQ). **YouTube video**.

- Video n°2: [Build the LoRa IoT soil sensor device for the PRIMA INTEL-IRRIS project](https://youtu.be/zcazzDbXvHk). **YouTube video**.

- Video n°3: [Connect the SEN0308 capacitive sensor and test LoRa data transmission on the Edge IoT gateway](https://youtu.be/n0YGan7_vUc). **YouTube video**.

- Video n°4: [Demonstrating the INTEL-IRRIS soil sensor device & WaziGate framework for intelligent irrigation in-the-box](https://youtu.be/j-1Nk0tv0xM). **YouTube video**. 

- Video n°5: [Non-technical video showing the production of the starter-kit](https://youtu.be/5nznRcIoe40). **YouTube video**.

- [Building the INTEL-IRRIS IoT platform. Annex 1: ordering the PCBs](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-PCB.pdf). Slides.

- [List of hardware parts for the INTEL-IRRIS soil sensor device](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-IrriS-low-cost-sensor-hardware-parts.pdf). Document.


The starter-kit
---------

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/starter-kit-bg-white.jpg" width="300">

Enjoy!
C. Pham
Coordinator of PRIMA INTEL-IRRIS