PCBs
----

The PCB for the Arduino ProMini (3.3v, 8MHz version) can be used for prototyping and even integration purpose. Beware that A4 and A5 (which are usually SDA and SCL pin of the I2C bus) are not connected on the PCB. If you need to use them, use these 2 pins from the Arduino board itself.

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-RFM95W-top.png" width="300">

<img src="https://github.com/CongducPham/PRIMA-Intel-IrriS/blob/main/images/ProMini-SX128X-top.png" width="300">

All the PCBs have footprint for an SMA connector or uFl and replicated rows for all the pins. They also have extra VCC and GND rails. They also have 4 solder pads that can be soldered together if you want to connect various DIO pins. Some of these pads are by default connected. If you want to use the connected Arduino pins for other purposes, just cut the wire between the pads with a cutter and test for discontinuity.

- for RFM95W. Connected by default: DIO0 to D2, DIO1 to D3, RST to D4. Not connected by default: DIO2 to D5
- for NiceRF SX1262. Connected by default: DIO1 to D3, RST to D4. Not connected by default: BUSY to D5
- for NiceRF SX1280. Connected by default: DIO1 to D2, BUSY to D3 and RST to D4. Not connected by default: DIO2 to D5 

Our modified communication library makes wiring a bit easier because only CS, MISO, MOSI, SCK, BUSY (SX126X and SX128X only) and RST are needed. However on the PCBs, DIOs pins are available to use other libraries (for LMIC stack for instance). We also indicate clearly which pin of the LoRa module you need to solder on the PCB (for instance `MOSI>`). As you can see on the picture, you can directly solder the Arduino board on the PCB, or, as we did, use intermediate headers so that the Arduino board can be easily plugged and removed.

You can download all the Gerber zipped archive and view them on an [online Gerber viewer](https://www.pcbgogo.com/GerberViewer.html).

- Arduino ProMini v3 PCB for RFM95W/NiceRF SX1262 zipped Gerber archive, 2 layer board of 30x77mm [.zip](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/MySecondProMiniLoRaBreakout_2021-11-14.zip)
- Arduino ProMini v2 PCB for NiceRF SX1280 zipped Gerber archive, 2 layer board of 30x77mm [.zip](https://github.com/CongducPham/PRIMA-Intel-IrriS/raw/main/PCBs/MySecondProMiniLoRaBreakout_SX128X_2021-11-14.zip)

You can easily make them produced on many online PCB manufacturers. Usually, you just need to provide the zip archive and both size and number of layers are detected. You can dramatically decrease the price by using "panelize" option. We use 3x1 for ProMini PCBs. For instance, we ordered them from [JLCPCB](https://jlcpcb.com/) and the cost of 10 panels (i.e. 30 ProMini breakout) is about $4! 