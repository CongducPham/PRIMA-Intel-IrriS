PCB v2
----

This PCB for the Arduino ProMini (3.3v, 8MHz version) can be used for prototyping and even integration purpose. Beware that A4 and A5 (which are usually SDA and SCL pin of the I2C bus) are not connected on the PCB. If you need to use them, use these 2 pins from the Arduino board itself.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-RFM95W-top.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-SX128X-top.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-RFM95W-board.jpg" width="400">

All the PCBs have footprint for an SMA connector or uFl and replicated rows for all the pins. They also have extra VCC and GND rails. They also have 4 solder pads that can be soldered together if you want to connect various DIO pins. Some of these pads are by default connected. If you want to use the connected Arduino pins for other purposes, just cut the wire between the pads with a cutter and test for discontinuity.

- for RFM95W/RFM96W. Connected by default: DIO0 to D2, DIO1 to D3, RST to D4. Not connected by default: DIO2 to D5
- for NiceRF SX1262. Connected by default: DIO1 to D3, RST to D4. Not connected by default: BUSY to D5
- for NiceRF SX1280. Connected by default: DIO1 to D2, BUSY to D3 and RST to D4. Not connected by default: DIO2 to D5 

Our modified communication library makes wiring a bit easier because only CS, MISO, MOSI, SCK, BUSY (SX126X and SX128X only) and RST are needed. However on the PCBs, DIOs pins are available to use other libraries (for LMIC stack for instance). We also indicate clearly which pin of the LoRa module you need to solder on the PCB (for instance `MOSI>`). As you can see on the picture, you can directly solder the Arduino board on the PCB, or, as we did, use intermediate headers so that the Arduino board can be easily plugged and removed.

You can download all the Gerber zipped archive and view them on an [online Gerber viewer](https://www.pcbgogo.com/GerberViewer.html).

- Arduino ProMini PCB for RFM95W/RFM96W/NiceRF SX1262 zipped Gerber archive, 2 layer board of 30x77mm [.zip](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/MySecondProMiniLoRaBreakout_2022-01-20.zip)
- Arduino ProMini PCB for NiceRF SX1280 zipped Gerber archive, 2 layer board of 30x77mm [.zip](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/MySecondProMiniLoRaBreakout_SX128X_2022-01-20.zip)

You can easily make them produced on many online PCB manufacturers. Usually, you just need to provide the zip archive and both size and number of layers are detected. You can dramatically decrease the price by using "panelize" option. We use 3x1 for ProMini PCBs. For instance, we ordered them from [JLCPCB](https://jlcpcb.com/) and the cost of 10 panels (i.e. 30 ProMini breakout) is about $4! 

PCB IRD v4.1
----

This PCB for the Arduino ProMini (3.3v, 8MHz version) can be used for prototyping and even integration purpose. It allows much simpler wiring of the soil humidity sensor, the watermark sensors and the soil temperature sensor (placeholder for associated resistors are already there).

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-4-1-raw-top.png" width="400">

This new PCB also integrates by default a low-cost solar charging circuit to add solar panel and rechargeable NiMh batteries. The whole solar circuit appears on the back of the PCB (left part of the PCB) as illustrated below.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-4-1-raw-bottom.png" width="400">

You can download the Gerber zipped archive and view them on an [online Gerber viewer](https://www.pcbgogo.com/GerberViewer.html).

- Arduino ProMini IRD PCB v4.1 for RFM95W/RFM96W zipped Gerber archive, 2 layer board of 30.89x79.5mm [.zip](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/IRD_PCB_4_1/Gerber_PCB4_1_IISS_2023_09_27.zip)

The raw PCB will of course not have the additional electronic components. While it is possible to add these component manually, in practice it is not recommended as the components are small and manually soldering them is prone to errors. 

PCBA IRD v4.1
----

Instead, the fully assembled PCB (PCBA) with solar circuit can be ordered fully assembled from PCB manufacturers.

You can download the BOM & CPL files to order the fully assembled board from PCB manufacturers. You can look at our tutorial on how to order PCB that are fully assembled by the manufacturer: [Building the INTEL-IRRIS IoT platform. Annex 1: ordering the PCBs, including PCBA](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-PCB-update-PCBA.pdf). 

- Bill of Materiels (BOM) file for the IRD PCB v4.1 RFM95W 868 [.xls](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/IRD_PCB_4_1/BOM_SMT_TB_RFM95_868_IISS_PCB4_1_wH2.xlsx)

- CPL file for the IRD PCB v4.1 [.csv](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/IRD_PCB_4_1/CPL.csv)

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-4-1-3D-top.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-4-1-3D-bottom.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-4-1-real-top-removebg-preview.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-4-1-real-bottom-removebg-preview.png" width="400">

Read [INTEL-IRRIS Newsletter #4](https://intel-irris.eu/intel-irris-newsletter-4) for a brief description and additional images.

PCB IRD v5
----

This PCB is the RAK3172 version of the IRD PCB v4.1.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-5-raw-top.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ird-pcb-5-raw-bottom.png" width="400">

You can download the Gerber zipped archive and view them on an [online Gerber viewer](https://www.pcbgogo.com/GerberViewer.html).

- Arduino ProMini IRD PCB v5 for RAK3172 zipped Gerber archive, 2 layer board of 30.89x79.5mm [.zip](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/IRD_PCB_5/Gerber_PCB5_RAK3172_IISS_2024-06-01.zip)

Because the RAK3172 module is connected to the Arduino's serial port (UART), there is a specific procedure to debug the board in order to see the text output normally printed to the serial monitor window of the Arduino IDE. See the [tutorial on using this IRD PCB v5](https://docs.google.com/viewer?url=https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/Tutorials/Intel-Irris-IOT-platform-PCBv5-PCBA.pdf).

PCBA IRD v5
----

The fully assembled PCB (PCBA) with solar circuit can be ordered fully assembled from PCB manufacturers.

You can download the BOM & CPL files to order the fully assembled board from PCB manufacturers. You can look at our tutorial on how to order PCB that are fully assembled by the manufacturer: [Building the INTEL-IRRIS IoT platform. Annex 1: ordering the PCBs, including PCBA](https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/Tutorials/Intel-Irris-PCB-update-PCBA.pdf). 

- Bill of Materiels (BOM) file for the IRD PCB v5 RAK3172 [.xls](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/IRD_PCB_5/BOM_SMT_TB_RAK3172_868_IISS_PCB5)

- CPL file for the IRD PCB v5 [.csv](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/IRD_PCB_5/CPL_PCB5_RAK3172.csv)

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/PCBA_v5_top.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/PCBA_v5_bottom.png" width="400">


PCB RAK3172 v1
-----

We have designed a very generic PCB which integrates the LoRaWAN RAK3172 radio chip to provide full LoRaWAN connectivity. It also provides simpler wiring of sensors, similarly to the IRD’s PCB. There is no solar circuit however, as the main objective is to support full LoRaWAN connectivity for large-scale and worldwide deployment scenarios.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-RAK3172-top.png" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-RAK3172-board.jpg" width="400">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/RAK_W_IRD_PCB.jpg" width="400">

Read [INTEL-IRRIS Newsletter #4](https://intel-irris.eu/intel-irris-newsletter-4) for a brief description and additional images.

